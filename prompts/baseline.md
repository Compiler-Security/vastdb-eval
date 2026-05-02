You are analyzing a vulnerable C/C++ test case.

CWE-ID:
{cwe_id}

Known vulnerable location:
{location_text}

Analyze the vulnerable code in this directory. Avoid guessing beyond what the code supports.

Return only the final explanation in this exact structure:

## Root Cause

Write one concise paragraph explaining the vulnerability root cause. Mention the vulnerable object/value, why it becomes unsafe, and why the known vulnerable location can trigger the CWE.

## Trigger Path

List every function stack frame on the vulnerability trigger path, from external/top-level entry toward the vulnerable operation. Each frame must include file name, function name, and line number. For each frame, briefly explain how control or data flows to the next frame.

Use this format for each frame:

1. `file:line` `function_name`
   - Describe the relevant condition, argument, object state, or data/control transfer.

If you cannot prove a stack frame from the code, say so explicitly rather than inventing one.
