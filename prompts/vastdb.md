You are analyzing a vulnerable C/C++ test case. You have normal text-search/code-reading tools and must use VAST DB location tools to identify or validate relevant code context.

CWE-ID:
{cwe_id}

Known vulnerable location:
{location_text}

Analyze the vulnerable code in this directory. Avoid guessing beyond what the code supports. Treat the known vulnerable location text as anchors, not as a guaranteed chronological source-to-sink ordering; inspect the code to determine the actual trigger order.

Use this workflow:

1. First inspect the local code with text search/read tools to identify the relevant functions, line numbers, and plausible trigger path.
2. Then, use the available skill to understand the VAST DB schema, generate database queries, and execute a cypher query to retrieve call-chain or data-flow information. **At least execute a cypher query**.
3. If query fail, return the best code-backed analysis anyway. Do not refuse, do not leave placeholder frames, and do not say the path cannot be proven solely because database-backed evidence was unavailable.
4. Use VAST DB evidence as supporting evidence, not as a prerequisite for answering.

Return only the final explanation in this exact structure:

## Root Cause

Write one concise paragraph explaining the vulnerability root cause. Mention the vulnerable object/value, why it becomes unsafe, and why the known vulnerable location can trigger the CWE.

## Trigger Path

List every function stack frame on the vulnerability trigger path, from external/top-level entry toward the vulnerable operation. Each frame must include file name, function name, and line number. For each frame, briefly explain how control or data flows to the next frame.

Use this format for each frame:

1. `file:line` `function_name`
   - Describe the relevant condition, argument, object state, or data/control transfer.

If you cannot prove a stack frame from either the code or VAST DB evidence, say so explicitly rather than inventing one. Do not output placeholder text such as `file:line` or `function_name`.
