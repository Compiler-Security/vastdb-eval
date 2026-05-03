---
name: write-cypher
description: Use when asked to find callers or callees of a function, trace where a value flows (def-use chains), locate operations at a specific source line, find all call sites of a function, or identify the enclosing function for a given operation. This skill generates read-only Cypher queries against the VAST MLIR Neo4j graph and executes them via the MCP tool loc-mcp-server run_cypher.
---

# Write Cypher

## Overview

Use this skill to write read-only Cypher queries against the VAST MLIR Neo4j graph and execute them via `loc-mcp-server` `run_cypher`. You must invoke `understand-mlir-schema` first to learn the node labels, relationships, and properties before writing any query.

## Workflow

1. Invoke `understand-mlir-schema` to learn the database structure.
2. Restate the query intent in database terms: target operation, function, source/sink location, value, type, caller/callee, or enclosing scope.
3. Read `references/query-templates.md` and reuse a matching template when possible.
4. Prefer a small exploratory query first, then refine it after seeing records.
5. Execute the Cypher through `loc-mcp-server` `run_cypher`; do not only print the query.

## Query Rules

- Generate read-only queries by default. Do not use `CREATE`, `MERGE`, `SET`, `DELETE`, or `REMOVE` unless the user explicitly asks to modify the database.
- Use parameter placeholders such as `$func_name`, `$op_uid`, `$file_name`, `$line`, and `$callee`.
- Return compact evidence: operation `uid`, `name`, `location`, function name, relevant edge `idx`, and matched attr/value/type fields.
- Order indexed relationships by `idx`.
- Use `LIMIT` for broad exploratory queries.
- Do not invent labels, relationship types, properties, or operation names not found in the references.

## Common Patterns

When you have a specific source line (file + line number), start by matching operations at that location. Operations carry a `location` field in the form `['F', file, line, column]`, so you can pin down exactly which operations belong to a given source line, then use their `uid` to expand into call chains, data flow, or enclosing functions.

Match operations by source file and line:

```cypher
MATCH (op:OPERATIONS)
WHERE op.location[1] ENDS WITH $file_name
  AND toInteger(op.location[2]) = $line
RETURN op.uid AS uid, op.name AS name, op.func_name AS func_name, op.location AS location
ORDER BY uid
LIMIT 50
```

Find a function by name:

```cypher
MATCH (f:OPERATIONS)
WHERE f.name IN ['hl.func', 'll.func']
  AND (f.func_name = $func_name OR f.fs_raw_name = $func_name)
RETURN f.uid AS uid, f.name AS op, f.func_name AS func_name, f.fs_raw_name AS raw_name, f.location AS location
LIMIT 20
```

List operations inside a function:

```cypher
MATCH (f:OPERATIONS {uid: $func_uid})
  -[:CHILDREGION]->(:REGIONS)
  -[:CHILDBLOCK]->(:BLOCKS)
  -[ie:CHILDINST]->(op:OPERATIONS)
RETURN op.uid AS uid, op.name AS name, op.location AS location, ie.idx AS idx
ORDER BY ie.idx
LIMIT 200
```

Find uses of a value:

```cypher
MATCH (def:OPERATIONS)-[re:RESVALUE]->(v:VALUES)<-[ae:ARGVALUE]-(use:OPERATIONS)
WHERE def.uid = $def_op_uid
RETURN v.uid AS value_uid, re.idx AS result_idx, use.uid AS use_uid, use.name AS use_name,
       ae.idx AS arg_idx, use.location AS use_location
ORDER BY use_uid, ae.idx
LIMIT 100
```

Find direct call sites by callee name:

```cypher
MATCH (call:OPERATIONS {name: 'hl.call'})
WHERE call.callee = $callee
RETURN call.uid AS uid, call.name AS name, call.callee AS callee, call.location AS location
LIMIT 100
```

## References

- `references/query-templates.md`: reusable Cypher templates for common query patterns.
