---
name: write-cypher
description: Use when working with VAST DB to write Cypher queries for code analysis. Covers query patterns for finding operations by source location, finding an operation's enclosing function, locating symbols, exploring call relationships, and tracing def-use data flow. Load understand-mlir-schema when schema details are needed to construct queries or interpret query results.
---

# Write Cypher

## Overview

Use this skill for VAST DB Cypher query patterns. Prefer reusable templates, and construct new queries when no template matches the analysis need.

## Workflow

1. Read `references/query-templates.md`. If a template matches the task, reuse it as-is.
2. If no template matches, construct a new Cypher query using the VAST DB schema and the closest related template as guidance.

## Query Strategy

- Load `understand-mlir-schema` when query construction or result interpretation depends on schema details.
- Use small exploratory queries with `LIMIT` when schema details such as operation names, properties, node and relationship labels are uncertain.
- When starting from a source location, first map it to operation `uid`s. Use those `uid`s as stable handles for follow-up queries such as enclosing functions, call relationships, and def-use data flow.
- Return only properties needed for the analysis. Common fields are node `uid`, relationship `idx`, operation `name`, `location`, `func_name`, `fs_raw_name`, `varName`, `className`, type `type`, and attribute `value`.
- Order indexed relationships by `idx`.

## Query Rules

- Generate read-only queries by default. Do not use `CREATE`, `MERGE`, `SET`, `DELETE`, or `REMOVE`.
- Use parameter placeholders such as `$func_name`, `$op_uid`, `$file_name`, `$line`, and `$callee`.
- Do not invent node and relationship labels, properties, or operation names. Verify unfamiliar schema details with `understand-mlir-schema` or exploratory queries.

## Common Patterns

Read `references/query-templates.md` for reusable Cypher templates.
