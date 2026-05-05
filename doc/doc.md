# VASTDB Agent Evaluation

本文档描述 `run_eval.py` 当前实现和实验需求。脚本用于批量运行 C/C++ 漏洞测试用例，对比 baseline agent 和 vastdb agent 的根因分析结果，并用 judge agent 生成结构化评分。

## 实验需求

每个测试用例包含一个漏洞版本 `bad/`、一个修复版本 `good/` 和漏洞元信息 `cve.json`。实验要求在同一份 `bad/` 代码上运行两类分析 agent：

- baseline agent：禁用 LSP，只使用常规代码阅读和搜索能力，不配置 VAST DB 相关 MCP，用于模拟没有图数据库辅助的漏洞根因分析。
- vastdb agent：禁用 LSP；先做常规代码阅读和文本搜索，在候选项过多或需要精确 caller/callee、函数定义、调用点、数据流信息时，启用 `loc-mcp-server`，并结合 `.opencode/skills/` 中的 `understand-mlir-schema` 和 `write-cypher` 生成 Cypher 查询，利用 VAST DB 获取辅助证据。

两个分析 agent 必须输出同样结构的文本结果：

- 一段简要根因分析。
- 漏洞触发路径上的函数栈帧列表；每个栈帧需要包含文件名、函数名、行号，并解释控制流或数据流如何传递到下一帧。

judge agent 读取 baseline/vastdb 两份输出、`bad/`、`good/` 和 `cve.json`，对两份解释分别判断是否正确、给出 0 到 10 分和简要理由。judge 输出必须是结构化 JSON。

## 执行流程

每个测试用例按固定顺序执行：

1. 清空并重建工作目录 `$work`，复制 `cve.json`、`bad/`、`good/`。
2. 启动或复用该用例对应的 Neo4j Docker 容器。
3. 如果容器是本次新建的，构建 `bad/` 并写入 VAST DB。
4. 运行 baseline agent：在 `$work/bad` 中分析漏洞，禁用 LSP，不使用 VAST DB/MCP。
5. 运行 vastdb agent：在 `$work/bad` 中分析漏洞，禁用 LSP；先用文本搜索建立候选 trigger path，必要时再使用 `loc-mcp-server` 和 VAST DB skills 精确查询调用链/数据流。
6. 运行 judge agent：在 `$work` 中读取 `bad/`、`good/`、`cve.json` 和两份 agent 输出，生成 JSON 评分。
7. 根据结果清理容器。

单个测试用例内部串行执行；多个测试用例可以通过 `--jobs` 并行执行。

## 运行方式

入口脚本：

```bash
python run_eval.py <CWD-ID> <ID>
python run_eval.py <CWD-ID> <START-ID>..<END-ID>
python run_eval.py all
python run_eval.py all --jobs 4
```

示例：

```bash
python run_eval.py 1025 2
python run_eval.py 1025 1..5
```

`<ID>` 可以写成 `2` 或 `02`。脚本内部会把它格式化为两位目录名，例如 `CWD-1025-02`。

脚本路径通过 `Path(__file__).resolve().parent` 推导项目根目录，因此即使用软链接调用脚本，也会以真实脚本所在目录作为项目根。

## 依赖

需要先安装 Node 依赖：

```bash
npm install
```

运行时依赖包括：

- `python3`
- `node`
- `@opencode-ai/sdk`
- Docker CLI 和可用 Docker daemon
- `cmake`
- `wllvm` / `wllvm++`
- VAST 写库所需工具链
- vastdb 阶段需要可用的 `loc-mcp-server`

脚本启动时会显式检查 `node`、`docker`、`cmake` 和 `@opencode-ai/sdk`。其他工具在对应阶段执行时由实际命令报错。

## 目录约定

测试用例源目录：

```text
testcases/CWD-<CWD-ID>/CWD-<CWD-ID>-<ID2>
```

每个测试用例必须包含：

```text
cve.json
bad/
good/
```

工作目录：

```text
outputs/CWD-<CWD-ID>/CWD-<CWD-ID>-<ID2>
```

每次运行某个测试用例前，脚本会删除整个 `$work` 并重新复制输入文件，因此旧的 `results/`、`build/` 和 agent 临时文件都会被清空。

## Neo4j 容器

每个测试用例使用独立容器：

```text
vastdb-eval-<CWD-ID>-<ID2>
```

端口公式：

```text
bolt = 40000 + (<CWD-ID> - 1000) * 100 + <ID>
http = 45000 + (<CWD-ID> - 1000) * 100 + <ID>
```

端口映射：

```text
<bolt>:7687
<http>:7474
```

容器处理逻辑：

- 容器已运行：直接复用，并等待 Neo4j readiness check 通过。
- 容器存在但未运行：执行 `docker start`，再等待 Neo4j readiness check 通过。
- 容器不存在：执行 `docker run -d` 创建，再等待 Neo4j readiness check 通过。

readiness check 使用 host 上的 `cypher-shell -a bolt://localhost:<bolt_port> "RETURN 1"`。只有该查询成功后，脚本才会继续执行后续阶段。

`docker run` 会把 `configs/env.json` 中的 `neo4j` 字段展开为 `-e KEY=VALUE`，并固定设置 `-e NEO4J_AUTH=none`。镜像默认是 `neo4j:latest`，可用 `configs/env.json` 顶层 `neo4j_image` 字段覆盖。

## 数据库写入

只有容器是本次新建时，脚本才会写数据库；复用已有容器时跳过写库。

写库在 `$work` 下执行：

```bash
cmake -S $work/bad -B build -DCMAKE_C_COMPILER=wllvm -DCMAKE_CXX_COMPILER=wllvm++
cmake --build build
```

configure 阶段会合并 `configs/env.json` 的 `vast` 字段，并额外设置：

```text
WLLVM_CONFIGURE_ONLY=1
VAST_OUTPUT_FILE=$work/build/wllvm.log
```

build 阶段同样会合并 `configs/env.json` 的 `vast` 字段，并额外设置：

```text
VASTDB_NEO4J_ADDRESS=neo4j:@localhost:<bolt_port>
VAST_OUTPUT_FILE=$work/build/wllvm.log
```

build 阶段的成功判定不只依赖 `cmake --build build` 的返回码。脚本还会读取 `VAST_OUTPUT_FILE`；如果其中出现 `Failed to generate bitcode`，即使 CMake 返回 0，也判定数据库写入失败。

## Agent 配置

三个阶段分别从以下文件读取 SDK inline config：

```text
configs/opencode_baseline.json
configs/opencode_vastdb.json
configs/opencode_judge.json
```

这些文件本身就是 opencode config object，分别定义该阶段的 `model`、provider、权限、LSP、MCP 等配置。

opencode 会先按自身规则加载本地或全局配置，再叠加 SDK 传入的 inline config。

vastdb 阶段会在加载 `configs/opencode_vastdb.json` 后注入：

```text
mcp.loc-mcp-server.environment.NEO4J_URI=bolt://localhost:<bolt_port>
```

`apiKey`、`baseURL` 和静态 provider 配置写在各阶段 opencode config 中。

Python 主脚本与 Node SDK runner 的分工见 `doc/modules.md`。

## Prompt

prompt 文件保存在：

```text
prompts/baseline.md
prompts/vastdb.md
prompts/judge.md
prompts/judge_schema.json
```

脚本从 `$work/cve.json` 读取 `CWE-ID` 和漏洞位置，并注入 baseline/vastdb prompt。漏洞位置支持两种形式：

```json
{
  "source": "...",
  "sink": "..."
}
```

或：

```json
{
  "location": "..."
}
```

`source`/`sink` 会被作为定位锚点传入 prompt，但 prompt 要求 agent 不把它们当成保证成立的时间顺序。

baseline/vastdb prompt 都要求输出：

```text
## Root Cause

...

## Trigger Path

1. `file:line` `function_name`
   - ...
```

其中 `Root Cause` 是简要根因分析；`Trigger Path` 是带文件名、函数名和行号的漏洞触发路径栈帧。

judge prompt 读取 baseline/vastdb 输出、`bad/`、`good/` 和 `cve.json`。它的 JSON 格式要求不写在 prompt 文本中，而是通过 `prompts/judge_schema.json` 作为 SDK structured output schema 指定。

## Agent 行为

### baseline

- 工作目录：`$work/bad`
- Config：`configs/opencode_baseline.json`
- Prompt：`prompts/baseline.md`
- 不配置 VAST DB 相关 MCP
- 当前配置禁用 LSP

baseline 是无 VAST DB 辅助的对照组。它只根据代码和 prompt 中给出的 CWE/漏洞位置锚点完成分析，不能依赖 LSP 或图数据库调用链/数据流查询。

### vastdb

- 工作目录：`$work/bad`
- Config：`configs/opencode_vastdb.json`
- Prompt：`prompts/vastdb.md`
- 启用 `loc-mcp-server`
- 当前配置禁用 LSP

vastdb 是实验组。它与 baseline 分析同一份 `bad/` 代码，但可以额外使用 VAST DB 相关能力。推荐路径是先通过普通文本搜索缩小候选范围；当需要数据库辅助时，先用 `understand-mlir-schema` 理解图数据库 schema，再用 `write-cypher` 编写和执行只读 Cypher 查询，通过 `loc-mcp-server` 获取函数定义、caller/callee、调用点或数据流信息。

skills 由 opencode 根据工作目录向上发现；因此从 `$work/bad` 启动时可以发现项目根目录下的 `.opencode/skills/`。

### judge

- 工作目录：`$work`
- Config：`configs/opencode_judge.json`
- Prompt：`prompts/judge.md`
- Format schema：`prompts/judge_schema.json`

judge 是裁判组。它不复用前两个 agent 的 session，而是重新读取两份输出、漏洞版本、修复版本和漏洞元信息；必要时可用配置允许的工具核对真实根因。

schema 要求输出：

```json
{
  "baseline": {
    "correct": true,
    "score": 8,
    "reason": "..."
  },
  "vastdb": {
    "correct": true,
    "score": 9,
    "reason": "..."
  }
}
```

脚本会校验 `baseline` 和 `vastdb` 都包含：

- `correct`: boolean
- `score`: 0 到 10 的 number
- `reason`: string

## 输出处理

agent 不需要自己写 `output.txt` 或 `output.json`。脚本从 SDK runner 返回结果中抽取输出并写文件。

- baseline/vastdb 输出写入 `results/<stage>/output.txt`。
- judge 输出写入 `results/judge/output.json`。
- judge 优先使用 SDK structured output；如果没有拿到结构化结果，脚本会回退到 response 文本并做宽松 JSON 解析。

宽松 JSON 解析支持：

- 支持剥离 JSON fenced block。
- 支持从额外文本中截取 JSON object。
- 解析成功后重写为格式化 JSON。

具体抽取逻辑见 `doc/modules.md`。

## 日志

每个测试用例结果目录：

```text
$work/results/
  baseline/
    opencode_request.json
    realtime_events.jsonl
    realtime_messages.jsonl
    realtime_output.txt
    log.json
    output.txt
  vastdb/
    opencode_request.json
    realtime_events.jsonl
    realtime_messages.jsonl
    realtime_output.txt
    log.json
    output.txt
  judge/
    opencode_request.json
    realtime_events.jsonl
    realtime_messages.jsonl
    realtime_output.txt
    log.json
    output.json
  run/
    run.json
```

### log.json

`results/<stage>/log.json` 记录：

- stage、case、工作目录、开始/结束时间、总耗时。
- Node runner 命令状态。
- SDK 请求和返回的主要诊断信息，包括 config、MCP status、tool listing、skills、实时记录文件、response、messages。
- usage 汇总。
- 最终输出内容。
- 错误信息。

每次 agent 调用都会在同目录生成实际 SDK 请求记录：

```text
results/<stage>/opencode_request.json
```

同时会生成三份实时记录：

- `results/<stage>/realtime_events.jsonl`：通过 SDK `event.subscribe()` 记录的原始 SSE 事件，每行一个 JSON。
- `results/<stage>/realtime_messages.jsonl`：runner 根据 `message.part.updated` 记录 `step-start`、`step-finish` 和已结束的 part；每行是一个纯 part 对象。
- `results/<stage>/realtime_output.txt`：从 `message.part.delta` 事件中追加的 text 增量，便于运行时观察模型正在输出的文本。

实时 message 解析是诊断旁路：解析单条 event 失败只会记录到 `log.json` 的 `sdk.realtime.errors`，不会导致 agent 阶段失败。

usage 汇总包含 `tokens.total`、`tokens.input`、`tokens.output` 和 `cost`。

### run.json

运行事件追加到：

```text
$work/results/run/run.json
```

当前事件包括：

- `docker_inspect`
- `docker_start`
- `docker_run`
- `docker_wait_ready`
- `write_database_configure`
- `write_database_build`
- `write_database_skip`
- `docker_rm`
- `docker_stop`
- `failure`

事件时间使用东八区。

写数据库相关事件会记录 `command`、`cwd`、传给子进程的 `env` 覆盖项，以及 `returncode`、`stdout`、`stderr`；如果 build 阶段通过 `VAST_OUTPUT_FILE` 发现 bitcode 生成失败，还会记录 `failure_reason`；如果命令被异常打断，则记录 `error_type` 和 `error`。

### progress.log

进度日志：

```text
outputs/progress.log
```

该文件是一次脚本运行的全局进度日志，不属于单个测试用例。每次开始执行所选测试用例前会清空；并行执行时，所有测试用例都追加到同一个文件，每条记录都带 case 名。

格式：

```text
<time+08:00>\t<CWD-case>\t<stage>\t<event>
```

`event` 为 `start` 或 `finish`。脚本进入阶段时记录 `start`，阶段正常完成或抛出异常离开时记录 `finish`。

记录阶段：

- `load database`
- `write database`
- `baseline agent`
- `vastdb agent`
- `judge agent`

如果复用已有容器，脚本会跳过写库，也就不会写 `write database` 进度。

### summary.json / cur_summary.json

脚本会维护两个汇总文件：

```text
outputs/summary.json
outputs/cur_summary.json
```

`summary.json` 是累计汇总。每次 `run_eval.py` 执行时会加载旧文件，按 testcase 名称更新 `results` 中已有记录；如果 testcase 不存在，则创建新记录。

`cur_summary.json` 只记录本次 `run_eval.py` 选择的 testcase 结果。

两个文件都会在运行开始时初始化，并在每个 testcase 执行完成后实时重写一次。

顶层字段：

- `time`：本次 `run_eval.py` 的启动时间。
- `jobs`：本次 `run_eval.py` 的并行度。
- `total`：运行开始时确定，运行过程中不变化。`summary.json` 中是累计 testcase 数，`cur_summary.json` 中是本次选择的 testcase 数。
- `ok`：当前 `results` 中 `ok=true` 的 testcase 数。
- `failed`：当前 `results` 中 `ok=false` 的 testcase 数。运行过程中，尚未完成的 testcase 不计入 `failed`。
- `pending`：本次运行尚未完成的 testcase 数。
- `statistics`：根据当前 `results` 实时计算的统计信息。
- `results`：测试用例结果列表。每条 result 除 `case`、`ok`、`stage`、`error` 外，还包含该 testcase 的 `statistics`。

因此运行过程中：

- `cur_summary.json` 满足 `ok + failed + pending = total`。
- `summary.json` 的 `pending` 只表示本次运行中未完成的 testcase；累计历史结果已经在 `results` 中，不属于 pending。

`statistics` 字段包括：

- `score.baseline.correct` / `score.vastdb.correct`：baseline/vastdb 各自正确的 testcase 数。
- `score.baseline.avg_score` / `score.vastdb.avg_score`：baseline/vastdb 各自平均分。
- `score.vastdb_better`：vastdb 分数高于 baseline 的 testcase 数。
- `avg_duration`：baseline/vastdb/judge 三个 agent 阶段的平均耗时。

每条 result 中的 `statistics` 字段包括：

- `score.baseline` / `score.vastdb`：该 testcase 的 correct 和 score；如果没有合法 judge 输出，则为 `null`，并在同一条 result 的 `statistics.score_error` 中记录原因。
- `duration`：该 testcase 的 baseline/vastdb/judge 阶段耗时。

两个文件中的 `results` 都按 `CWD-ID` 和 testcase 的第二段 ID 排序。

judge 汇总只统计成功完成且能读取合法 `results/judge/output.json` 的测试用例；失败或缺失 judge 输出的用例计入 `skipped`。

## 错误处理与清理

单个测试用例失败会返回失败状态，并继续执行其他并行任务。

容器清理规则：

- 写数据库阶段失败或被 Ctrl-C 打断，且容器是本次新建的：执行 `docker rm -f <container>`。
- 其他成功或失败路径：执行 `docker stop -t 2 <container>`。

清理命令会尽量避免被二次 Ctrl-C 打断。

## 配置文件

运行环境配置：

```text
configs/env.json
```

结构：

```json
{
  "neo4j": {
    "KEY": "VALUE"
  },
  "vast": {
    "KEY": "VALUE"
  },
  "neo4j_image": "neo4j:latest"
}
```

字段含义：

- `neo4j`：展开为 `docker run -e KEY=VALUE`。
- `vast`：合并进写库阶段的 `cmake` 子进程环境。
- `neo4j_image`：可选，覆盖默认 Neo4j 镜像。

opencode 阶段配置：

```text
configs/opencode_baseline.json
configs/opencode_vastdb.json
configs/opencode_judge.json
```

这些文件通常包含本机 provider、`apiKey` 和 `baseURL`，应由 `.gitignore` 忽略。
