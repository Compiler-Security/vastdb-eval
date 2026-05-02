---
name: vast-db-schema
description: Provide the canonical graph schema for VAST MLIR (HighLevel and LowLevel dialects) stored in Neo4j, and define how an agent should generate Cypher queries against it
---

# Skill: MLIR Graph Schema (Cypher Querying)

## Purpose
Provide the canonical graph schema for VAST MLIR (HighLevel and LowLevel dialects) stored in Neo4j, and define how an agent should generate Cypher queries against it.

## When to use
Use this skill whenever you need to generate Cypher queries for the MLIR graph (operations, values, attributes, types, regions, blocks, vtables, vtts).

## Required inputs
- User’s query intent (what they want to find)
- Optional filters: operation name, funcName, varName, fileName, tag, uid

## Files to read
- `skills/vast-db-schema/schema-mlir-graph.md` (the concrete MLIR schema)
- When querying `OPERATIONS.name`, also read `skills/vast-db-schema/operations-catalog.md`
- When querying `TYPES.type`, also read `skills/vast-db-schema/types-catalog.md`
- For preset/fixed queries, read `skills/vast-db-schema/query-templates.md` and use the matching template verbatim (e.g., CALL_GRAPH_BY_CALLER_UID, COUNT_OPERATIONS)

## Output expectations
- Produce a Cypher query (or small set of queries) that strictly follows the schema.
- Use only labels, relationship types, and properties defined in the schema file.
- Prefer parameterized queries using `$param` placeholders.
- Use `MATCH`/`WHERE` for reads, `MERGE` for idempotent writes.
- If the request is ambiguous, choose the safest read-only query and explain assumptions.

## Query guidelines
- Use `OPERATIONS.uid` as the primary key when available; otherwise combine `name` and optional standard fields.
- For operation arguments, traverse `(:OPERATIONS)-[:ARGVALUE|ARGATTR|ARGBLOCK|ARGTYPE]->(...)` and use `idx` to preserve order.
- For operation results, traverse `(:OPERATIONS)-[:RESVALUE]->(:VALUES)`.
- For regions and blocks, traverse `(:OPERATIONS)-[:CHILDREGION]->(:REGIONS)-[:CHILDBLOCK]->(:BLOCKS)`.
- For block instructions and block arguments, traverse `(:BLOCKS)-[:CHILDINST]->(:OPERATIONS)` and `(:BLOCKS)-[:BLOCKARG]->(:VALUES)`.
- For value types, traverse `(:VALUES)-[:TYPE]->(:TYPES)`.
- For composite types, traverse `(:TYPES)-[:ARG]->(:TYPES)`.
- For attrs, use `INTATTRS|FLOATATTRS|STRINGATTRS` and, when applicable, `(:INTATTRS|:FLOATATTRS)-[:TYPE]->(:TYPES)`.
- Module ops are the only ones that can reach `VTABLES` and `VTTS` via `ARGVTABLE` / `ARGVTT`.

## Cypher output format
- Provide a single `cypher` code block.
- If multiple queries are needed, separate with blank lines and label each query with a comment.

## Safety
- Do not generate write queries unless explicitly requested.
- Do not invent labels or properties not present in the schema.
