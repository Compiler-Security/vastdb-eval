# VAST Cypher Query Templates

This file contains fixed Cypher templates to be used verbatim when applicable.

## COUNT_OPERATIONS
Use when the user asks for the total number of OPERATIONS nodes.

```cypher
// Count all OPERATIONS
MATCH (o:OPERATIONS)
RETURN count(o) AS operations_count
```

## CALLEE_NAMES_BY_FUNCTION_UID
Use when the user asks which functions are called by a given function operation (identified by `OPERATIONS.uid`). The callee name is stored in the `hl.call` op's `ARGATTR` at `idx = 0`.

```cypher
// Given a function op uid, list called function names via hl.call ARGATTR idx=0
MATCH (f:OPERATIONS {uid: $func_uid})
  -[:CHILDINST|:CHILDBLOCK|:CHILDREGION*]->(call:OPERATIONS {name: 'hl.call'})
  -[e:ARGATTR {idx: 0}]->(s:STRINGATTRS)
RETURN DISTINCT s.value AS callee_name
ORDER BY callee_name
```

## FUNCTION_UID_BY_NAME
Use when the user asks to find a function op uid by function name. This template assumes `OPERATIONS.fs_raw_name` exists (an implementation-defined field) and filters `name = 'hl.func'` with a prefix match.

```cypher
// Find hl.func uid(s) by fs_raw_name prefix
MATCH (f:OPERATIONS {name: 'hl.func', fs_raw_name: $func_name})
RETURN f.uid AS uid, f.fs_raw_name AS name
```

