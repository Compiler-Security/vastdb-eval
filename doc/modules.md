# Script Structure

本文档面向接手维护脚本的人，说明 `run_eval.py` 和 `scripts/opencode_runner.mjs` 的职责划分、交互方式，以及主要函数分组。实验需求和运行方式见 `doc/doc.md`。

## 总体结构

项目里和执行流程直接相关的代码只有两层：

- `run_eval.py`：Python 主控脚本，负责选择测试用例、准备工作目录、管理 Neo4j 容器、写入 VAST DB、调用 agent、解析输出、记录日志和汇总结果。
- `scripts/opencode_runner.mjs`：Node SDK runner，负责把 Python 生成的 `opencode_request.json` 转成 opencode SDK v2 调用，并把 SDK 返回值作为 JSON 打印给 Python。

二者通过文件和 stdout 交互：

1. `run_eval.py` 为某个阶段创建 `results/<stage>/opencode_request.json`。
2. `run_eval.py` 执行：

   ```text
   node scripts/opencode_runner.mjs results/<stage>/opencode_request.json
   ```

3. `opencode_runner.mjs` 读取请求，启动 opencode server/session，订阅 `event.subscribe()` 实时事件，调用 `session.prompt()`，再把完整 runner result 以一行 JSON 输出到 stdout。
4. `run_eval.py` 从 stdout 中解析 runner result，抽取最终答案、usage 和诊断信息，写入 `output.txt`/`output.json` 和 `log.json`。

Python 不直接 import opencode SDK；Node runner 不负责 Docker、CMake、测试用例选择或最终日志格式。

## `run_eval.py`

`run_eval.py` 是主控层。它的函数大致按执行顺序排列，可以按下面几组理解。

### 常量与数据模型

- `ROOT`、`TESTCASES_DIR`、`OUTPUTS_DIR`、`CONFIG_PATH`、`PROMPTS_DIR` 等常量定义项目路径。`ROOT` 使用 `Path(__file__).resolve().parent`，因此通过软链接启动时仍以真实脚本目录为项目根。
- `OPENCODE_CONFIG_PATHS` 定义 baseline/vastdb/judge 三个阶段对应的 opencode config 文件。
- `EvalError` 是带 `stage` 的业务异常，用于把失败归类到 `prepare`、`docker`、`db`、`baseline`、`vastdb`、`judge` 等阶段。
- `TestCase` 封装一个测试用例的派生属性：源目录、工作目录、容器名、bolt/http 端口、两位 ID 等。后续逻辑尽量通过这个对象取路径和端口。

### 通用工具函数

- `now_iso()` 返回 UTC 时间，用于 agent `log.json` 的 `started_at`/`ended_at`。
- `now_east8_iso()` 返回东八区时间，用于 `progress.log`、`run.json`、`summary.json` 和 `cur_summary.json`。
- `format_duration_seconds()` 把秒数格式化为 `XmYYs`，用于 agent log 和 summary 平均耗时。
- `read_json()` / `write_json()` 统一 JSON 读写和错误格式。
- `stringify_env()` 把 JSON object 中的环境变量值转为字符串，并跳过 `null`。
- `render_template()` 读取 prompt 模板并用 `str.format()` 注入 `cwe_id`、`location_text` 等字段。

### 参数、用例选择与依赖检查

- `parse_args()` 定义 CLI：`selector`、可选 `case`、`--jobs`。
- `parse_case_id()` 校验 case id 是数字。
- `discover_all_cases()` 扫描 `testcases/CWD-*/CWD-*-*`，用于 `all`。
- `select_cases()` 把 CLI 参数解析成 `TestCase` 列表，支持单例、范围和 `all`。
- `check_runtime_dependencies()` 在真正运行前检查 `node`、`docker`、`cmake` 和 `@opencode-ai/sdk`。`wllvm`、VAST 工具链、`loc-mcp-server` 等由对应阶段实际执行时暴露错误。

### 输入准备与 `cve.json`

- `copy_case_to_work()` 每次运行前删除整个 `$work`，然后重新复制 `cve.json`、`bad/`、`good/`。这是保证结果不受旧 `results/` 或旧构建产物影响的关键函数。
- `parse_cve()` 从 `$work/cve.json` 读取 `CWE-ID` 和漏洞位置。位置支持 `source`/`sink` 或 `location`，并转换成 prompt 中使用的文本。

### 子进程执行与清理

- `run_process()` 是普通命令执行封装。它使用独立进程组启动子进程，捕获 stdout/stderr；正常结束、传入 `timeout` 参数并触发 Python 子进程超时，或收到 Ctrl-C 时，都会清理子进程组，避免命令留下后台子进程。
- `kill_process_group()` 先发 `SIGTERM`，等待 5 秒；如果进程组仍未退出，再发 `SIGKILL` 并最多等待 5 秒。
- `run_cleanup_process()` 用于 Docker cleanup。主线程执行 cleanup 时临时忽略 Ctrl-C，避免用户第二次 Ctrl-C 打断 `docker rm -f` 或 `docker stop`。

维护注意：agent 阶段目前调用 `run_process(cmd, workdir)` 时没有传 Python 层 timeout，所以不存在完整的 agent wall-clock 超时。

### Neo4j Docker 管理

- `docker_inspect_running()` 调用 `docker inspect`，返回 `true`、`false` 或 `None`。`None` 表示容器不存在或 inspect 失败。
- `wait_for_neo4j_ready()` 通过 host 上的 `cypher-shell -a bolt://localhost:<bolt_port> "RETURN 1"` 轮询 Neo4j readiness，直到查询成功或 90 秒超时。
- `free_local_port()` 为 opencode SDK server 选择空闲本机端口。
- `ensure_neo4j()` 是容器生命周期入口：
  - 容器已运行：等待 readiness check 通过并复用。
  - 容器存在但未运行：`docker start` 后等待 readiness check 通过。
  - 容器不存在：`docker run -d` 创建，注入 `env.json.neo4j` 和固定的 `NEO4J_AUTH=none`，映射 bolt/http 端口，等待 readiness check 通过。
  - 返回值表示容器是否本次新建，这会决定是否需要写库。
- `stop_neo4j()` 执行 `docker stop -t 2`。
- `remove_neo4j()` 执行 `docker rm -f`，只用于本次新建容器的写库失败或写库阶段 Ctrl-C。

所有 Docker 事件都通过 `append_run_record()` 写入 `results/run/run.json`。

### VAST DB 写入

- `write_database()` 删除并重建 `$work/build`，然后执行 CMake configure 和 build。
- configure 阶段使用 `env.json.vast`，并额外设置 `WLLVM_CONFIGURE_ONLY=1` 和 `VAST_OUTPUT_FILE=$work/build/wllvm.log`。
- build 阶段使用 `env.json.vast`，并额外设置 `VASTDB_NEO4J_ADDRESS=neo4j:@localhost:<bolt_port>` 和 `VAST_OUTPUT_FILE=$work/build/wllvm.log`。
- build 阶段完成后，`write_database()` 会读取 `VAST_OUTPUT_FILE`。如果日志中包含 `Failed to generate bitcode`，即使 `cmake --build build` 返回 0，也会按写库失败处理。
- 只有 `ensure_neo4j()` 返回本次新建容器时才调用该函数。

写库失败会抛出 `EvalError("db", ...)`，上层会删除不完整容器。

### opencode config 构造

- `make_opencode_config(stage, case)` 读取 `configs/opencode_<stage>.json`。
- 该函数要求 config 中已有顶层 `model`，脚本不会运行时补模型。
- vastdb 阶段会向 `mcp.loc-mcp-server.environment` 注入 `NEO4J_URI=bolt://localhost:<bolt_port>`。
- baseline 和 judge 不会自动禁用或删除 MCP；它们是否有 MCP 完全取决于对应 config 文件。
- vastdb 阶段的两阶段检索策略不在 Python 中硬编码，而是写在 `prompts/vastdb.md` 中：先文本搜索，候选过多或需要精确 caller/callee、函数定义、调用点、数据流时，再通过 `.opencode/skills/understand-mlir-schema` 和 `.opencode/skills/write-cypher` 生成 VAST DB 查询。

`normalize_command()` 当前没有被主流程调用，可以视为遗留 helper。

### SDK runner 输出解析

这组函数只处理 Node runner 返回的 JSON，不再负责实际 SDK 调用。

- `parse_runner_stdout(stdout)` 扫描 stdout 中的 JSON 行，返回最后一个形如 runner result 的对象。当前 runner 正常只输出一行；保留“最后一个”是为了容忍未来出现中间诊断事件。
- `extract_assistant_text(runner_result)` 从 `runner_result.response` 中提取普通文本答案。
- `extract_latest_assistant_text(value)` 是递归提取实现，收集 assistant 上下文里的 text/content，返回最后一段。
- `extract_structured_output(runner_result)` 从 `runner_result.response` 中找 `StructuredOutput` tool part。
- `find_structured_tool_output(value)` 递归查找 `type == "tool"` 且 `tool == "StructuredOutput"` 的 part，并返回 `state.input`。
- `extract_usage(runner_result)` 从 `runner_result.messages.data[].info` 累加 `tokens.total`、`tokens.input`、`tokens.output` 和 `cost`。它只返回汇总，不保留每条 message 明细。

输出抽取的边界是：普通 agent 用 assistant text；带 `format` 的 judge 优先用 structured output。

### Agent 执行

- `run_agent()` 是 baseline/vastdb/judge 的统一执行入口。
- 它先调用 `make_opencode_config()`，构造 request：
  - `title`
  - `prompt`
  - `agent`
  - `directory`
  - `config`
  - `port`
  - `timeout`
  - 可选 `format`
- request 写入 `results/<stage>/opencode_request.json`。
- 随后执行 Node runner，解析 runner result，抽取最终输出并写入 `output.txt` 或 `output.json`。
- 最后写 `results/<stage>/log.json`，其中包含 process 信息、SDK 诊断信息、usage、总耗时、最终输出和错误。
- 如果 Node runner 非零退出，或最终输出为空，函数写入错误日志并抛出 `EvalError(stage, ...)`。
- `mask_command()` 目前只在命令参数过长时做简单替换；实际请求文件路径通常不会被脱敏。

### Judge 输出校验

- `validate_judge_output()` 读取 `results/judge/output.json`，用宽松 JSON 解析后校验结构。
- `parse_json_lenient()` 支持两种兜底：
  - 剥离 JSON fenced block。
  - 从额外文本中截取最外层 JSON object。
- 校验成功后，如果原文件不是格式化 JSON，会重写为规范 JSON。

### 运行记录与错误记录

- `log_progress()` 追加全局 `outputs/progress.log`。每条记录是东八区时间、case、stage、event，因此并行执行多个测试用例时也能在同一个文件里区分来源。
- `reset_progress_log()` 在每次脚本运行开始执行用例前清空 `outputs/progress.log`。
- `run_progress_stage()` 包装一个阶段，进入时写 `start`，无论正常返回还是抛出异常，离开时都会写 `finish`。
- `append_run_record()` 追加 `$work/results/run/run.json`，记录 Docker、写数据库命令和非 agent 阶段失败事件。
- `run_database_process()` 包装写数据库阶段的 configure/build 命令。它分别写入 `write_database_configure` 或 `write_database_build` 事件，记录 `command`、`cwd`、传给子进程的 `env` 覆盖项和 returncode/stdout/stderr；如果 build 阶段通过 `VAST_OUTPUT_FILE` 发现 bitcode 生成失败，则在 build 事件中记录 `failure_reason`；如果执行被异常打断，则在同一个事件类型中记录 error 后继续抛出。
- `record_failure()` 统一记录失败：
  - agent 阶段失败写入或补充 `results/<stage>/log.json`。
  - 非 agent 阶段失败写入 `run.json` 的 `failure` 事件。

### 流水线编排

- `run_case(case, cfg)` 是单个测试用例的完整流水线：
  1. 准备 `$work`。
  2. 解析 `cve.json`。
  3. 启动/复用 Neo4j。
  4. 必要时写库。
  5. 渲染并运行 baseline prompt。
  6. 渲染并运行 vastdb prompt。
  7. 渲染 judge prompt，读取 `judge_schema.json`，运行 judge。
  8. 校验 judge 输出。
  9. 根据失败阶段决定删除或停止容器。
- `main()` 负责全局参数解析、依赖检查、串行/并行调度，并实时维护 `outputs/summary.json` 和 `outputs/cur_summary.json`。
- `build_case_metrics()` 为单个 testcase 读取一次 judge 输出和各 agent `log.json`，生成内存中的统计输入；这些信息不会写回 summary 的 `results`。
- `build_metrics_cache()` 启动时为旧 `summary.json` 里的累计 results 构建内存统计缓存；本次运行中，每个 testcase 完成后只更新该 testcase 的 metrics。
- `build_score_statistics()` 根据内存 metrics 统计 baseline/vastdb 的 correct 数、平均分，以及 vastdb 分数高于 baseline 的用例数。
- `read_judge_output()` 根据 case 名定位 `results/judge/output.json`，并做 summary 统计所需的轻量校验。
- `build_average_duration_statistics()` 根据内存 metrics 计算 baseline/vastdb/judge 三个 agent 阶段的平均耗时。
- `build_statistics()` 组装 summary 文件中的 `statistics` 字段。
- `results_with_statistics()` 在写 summary 前为每条 result 附加 per-case `statistics`，包括该 testcase 的 score 和阶段耗时；这些字段只出现在 summary 输出里，不改变 `run_case()` 返回的原始 result。
- `load_summary_results()` 读取旧 `summary.json` 中的累计 `results`，用于本次运行的增量 upsert。
- `write_summaries()` 每次 testcase 完成后重写累计 `summary.json` 和本次 `cur_summary.json`，同时刷新 ok/failed/statistics/pending。
- `print_result()` 输出单个 case 的 `OK`/`FAIL` 状态。

并行执行使用 `ThreadPoolExecutor`，并行粒度是测试用例；单个测试用例内部仍是串行。summary 的 `results` 写入前会按 `CWD-ID` 和 testcase 的第二段 ID 排序。

## `scripts/opencode_runner.mjs`

Node runner 是一个很薄的 SDK 适配层。它不理解测试用例，也不做业务判断，只负责把 request JSON 变成 SDK 调用，并把 SDK 返回值结构化交回 Python。

### 请求输入

`readRequest()` 从命令行第一个参数读取 JSON 文件。该文件由 `run_eval.py` 写入，关键字段包括：

- `title`：session 标题。
- `prompt`：本次 agent 的完整 prompt 文本。
- `agent`：当前固定传 `build`。
- `directory`：opencode session 的工作目录。
- `config`：阶段 opencode inline config。
- `port`：opencode server 本机端口。
- `timeout`：传给 `createOpencode()` 的 SDK/OpenCode 请求超时。
- `format`：judge 阶段使用的 structured output schema。

### 模型解析与诊断查询

- `parseModel(model)` 把 `provider/model` 拆成 `providerID` 和 `modelID`。这个结果只用于 `client.tool.list()` 查询工具列表，不负责指定实际模型。
- `collectToolListing(client, model, directory)` best-effort 调用：
  - `client.tool.ids(...)`
  - `client.tool.list(...)`
- `collectSkills(opencode, directory)` best-effort 调用 `client.app.skills(...)`。

这两个诊断函数失败时返回 `{ ok: false, error: ... }`，不会让 runner 失败。

### SDK 调用主流程

`main()` 执行完整 SDK 流程：

1. 读取 request。
2. 从 request config 中取 `model`，用于后续 tool listing 查询。
3. 调用 `createOpencode({ hostname, port, config, timeout })` 启动本地 opencode server。
4. 构造 `query = { directory }`，后续 config、MCP、session、messages 查询都使用相同目录。
5. 查询 resolved config：`client.config.get(query)`。
6. 查询 MCP 状态：`client.mcp.status(query)`。
7. 创建 session：`client.session.create({ ...query, title })`。
8. 记录 tool listing 和 skills。
9. 如果 request 提供 `realtime` 路径，启动 `client.event.subscribe(query)`：
   - 原始事件写入 `realtime_events.jsonl`。
   - 根据 `message.part.updated` 判断 part 是否可落盘，写入 `step-start`、`step-finish` 和已结束的 part 到 `realtime_messages.jsonl`。
   - `message.part.delta` 的 text 增量追加到 `realtime_output.txt`。
   - 单条 event 解析失败只记录到 `realtime.errors`，不影响 `session.prompt()` 结果。
   - 停止订阅时先 abort，最多等待 5 秒；如果事件流没有及时结束，也会继续关闭输出文件。
10. 构造 prompt body：
   - `parts: [{ type: "text", text: request.prompt }]`
   - 可选 `agent`
   - 可选 `format`
11. 调用 `client.session.prompt(...)`。
12. 调用 `client.session.messages(...)` 取完整消息列表。
13. 停止实时事件订阅。
14. 打印一行 JSON runner result。
15. 在 `finally` 中关闭 opencode server。关闭后还有一个 3 秒 force-exit 兜底，用于避免 Node 进程因为残留 handle 挂住。

成功返回的 runner result 包含：

- `ok`
- `started_at` / `ended_at`
- `server_url`
- `session_id`
- `config_model`
- `prompt_agent`
- `requested_config`
- `resolved_config`
- `mcp_status`
- `tool_listing`
- `skills`
- `realtime`
- `response`
- `messages`

如果任何关键 SDK 调用失败，`main().catch(...)` 会打印 `{ ok: false, error: ... }`，并以 exit code `1` 退出。Python 侧会把这视为 agent 阶段失败。

## 交接时最常改的地方

- 调整实验需求或输出格式：优先改 `prompts/*.md` 和 `prompts/judge_schema.json`。
- 调整模型、provider、权限、LSP、MCP：改 `configs/opencode_<stage>.json`。
- 调整 Neo4j/VAST 环境变量：改 `configs/env.json`。
- 改 Docker 或写库流程：看 `ensure_neo4j()`、`write_database()`、`run_case()`。
- 改 agent 输出抽取或 log 结构：看 `run_agent()`、`parse_runner_stdout()`、`extract_assistant_text()`、`extract_structured_output()`、`extract_usage()`。
- 改 SDK 调用方式：看 `scripts/opencode_runner.mjs` 的 `main()`。
