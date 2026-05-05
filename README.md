## 项目简介

本项目用于批量评估大模型对 C/C++ 漏洞测试用例的根因分析能力。每个测试用例包含漏洞版本 `bad/`、修复版本 `good/` 和漏洞元信息 `cve.json`。

实验会对同一份 `bad/` 代码运行两类 agent：

1. baseline agent：禁用 LSP，不使用 VAST DB/MCP。
2. vastdb agent：禁用 LSP；先用普通文本搜索定位候选代码，候选过多或需要精确调用关系时，再结合 `loc-mcp-server`、`understand-mlir-schema` 和 `write-cypher` 查询 VAST DB。

最后由 judge agent 对两份分析结果进行结构化评分。实验需求和输出含义见 [doc/doc.md](doc/doc.md)。

---

## 项目结构

- `run_eval.py`：实验入口脚本。
- `scripts/opencode_runner.mjs`：opencode SDK runner，由 `run_eval.py` 调用。
- `testcases/`：测试用例集合。
  - `bad/`：漏洞版本编译单元。
  - `good/`：修复版本编译单元。
  - `cve.json`：漏洞元信息。
- `configs/`：本机运行配置。
  - `*.json.example`：可提交的示例配置。
  - `*.json`：本机真实配置，包含密钥或本机路径，默认被 `.gitignore` 忽略。
- `prompts/`：baseline、vastdb、judge 的 prompt 和 judge schema。
- `.opencode/skills/`：VAST DB 辅助 skill，包括 `understand-mlir-schema` 和 `write-cypher`。
- `doc/`：说明文档。
  - [doc/doc.md](doc/doc.md)：实验需求、运行方式、输入输出。
  - [doc/modules.md](doc/modules.md)：脚本结构和维护入口。

---

## 使用说明

1. 安装运行依赖。

   ```bash
   npm install
   ```

   还需要确保 Docker、CMake、`wllvm`/`wllvm++`、VAST 工具链和 `loc-mcp-server` 可用。

2. 复制并填写配置文件。

   ```bash
   cp configs/env.json.example configs/env.json
   cp configs/opencode_baseline.json.example configs/opencode_baseline.json
   cp configs/opencode_vastdb.json.example configs/opencode_vastdb.json
   cp configs/opencode_judge.json.example configs/opencode_judge.json
   ```

   至少需要填写 provider `apiKey`，并把 `configs/env.json` 中的 VAST 路径改成本机路径。

3. 运行测试用例。

   ```bash
   python3 run_eval.py 1025 2
   python3 run_eval.py 1025 1..5
   python3 run_eval.py fail
   python3 run_eval.py negative
   python3 run_eval.py all --jobs 4
   ```

4. 查看结果。

   单个测试用例结果写入：

   ```text
   outputs/CWD-<CWD-ID>/CWD-<CWD-ID>-<ID2>/results/
   ```

   全局进度和汇总写入：

   ```text
   outputs/progress.log
   outputs/summary.json
   outputs/cur_summary.json
   ```
