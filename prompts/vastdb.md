You are analyzing a vulnerable C/C++ test case. You have normal text-search/code-reading tools and must use VAST DB location tools to identify or validate relevant code context.

CWE-ID:
{cwe_id}

Known vulnerable location:
{location_text}

Analyze the vulnerable code in this directory. Avoid guessing beyond what the code supports. Treat the known vulnerable location text as anchors, not as a guaranteed chronological source-to-sink ordering; inspect the code to determine the actual trigger order.

# Workflow

1. Use text search/read tools only enough to locate the known vulnerable anchors in source code.
2. Use VAST DB as a code navigation tool. Before writing VAST DB Cypher queries, load the `write-cypher` skill and follow its Required Navigation Attempt and Navigation State Machine. Load the `understand-mlir-schema` skill when you need schema details to construct queries or interpret returned records.
3. For each source location anchor, run a VAST DB navigation attempt: map the source location to operation `uid`s, select the relevant operation `uid`, run `Dataflow: Use-Def Chain`, then run at least one next query from the returned definition.
4. Do not stop after `Dataflow: Use-Def Chain`. After it returns a definition, continue with the next query required by that definition kind in the Navigation State Machine.
5. Read source locations returned by VAST DB before drawing conclusions from them.
6. Do not stop after only location lookup, name lookup, enclosing-function lookup, or call-chain lookup. If a query does not return a useful path, continue with a narrower one-step navigation query from the current operation `uid` before falling back to text-only analysis.

# Output Schema

Return only the final explanation in this exact structure:

## Root Cause

Write one concise paragraph explaining the vulnerability root cause. Mention the vulnerable object/value, why it becomes unsafe, and why the known vulnerable location can trigger the CWE.

## Trigger Path

List every function stack frame on the vulnerability trigger path, from external/top-level entry toward the vulnerable operation. Each frame must include file name, function name, and line number. For each frame, briefly explain how control or data flows to the next frame.

Use this format for each frame:

1. `file:line` `function_name`
   - Describe the relevant condition, argument, object state, or data/control transfer.

If you cannot prove a stack frame from either the code or VAST DB evidence, say so explicitly rather than inventing one. Do not output placeholder text such as `file:line` or `function_name`.
