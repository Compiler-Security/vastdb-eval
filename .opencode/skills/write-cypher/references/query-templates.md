# VAST DB Cypher Query Templates

Use these templates before inventing new Cypher

# Code structure

## Location to Operations

Given a source file name and line number, return operations mapped to that source location.

- Normal location format: `['F', file, line, column]`.
- Macro-expanded location format: `['M', 2, 'F', file_expand, line_expand, column_expand, 'F', file_macro, line_macro, column_macro]`.

```cypher
MATCH (op:OPERATIONS)
WHERE (op.location[0] = 'F' AND op.location[1] ENDS WITH $file_name AND toInteger(op.location[2]) = $line) 
OR (op.location[0] = 'M' AND op.location[3] ENDS WITH $file_name AND toInteger(op.location[4]) = $line) 
RETURN op.uid, op.name, op.location
ORDER BY op.uid
LIMIT 50
```

## Macro Location to Operations

Given a file name and line number of a macro, return its related operations

```cypher
MATCH (op:OPERATIONS)
WHERE op.location[0] = 'M' AND op.location[7] ENDS WITH $file_name AND toInteger(op.location[8]) = $line
RETURN op.uid, op.name, op.location
ORDER BY op.uid
LIMIT 50
```

## Enclosing Function

Given an operation uid, return its enclosing function

```cypher
MATCH (:OPERATIONS {uid: $op_uid}) 
(()<-[:CHILDINST]-()<-[:CHILDBLOCK]-()<-[:CHILDREGION]-()){1,} (func {name: "hl.func"})
RETURN func.uid, func.func_name, func.fs_raw_name, func.location;
```

## Name to Function

Given a function name, return matching `hl.func` operations that are function definitions.

- Use `func_name` when the name comes from VAST DB query results. It is the demangled function name with qualifiers and prototype, such as `NS::C::foo(int *)`.
- Use `fs_raw_name` when the name comes from source code. It is the source-level function name, such as `foo`.
- The `CHILDREGION -> CHILDBLOCK` check filters out declarations and keeps function definitions.

```cypher
MATCH (func:OPERATIONS {name: "hl.func", func_name: $func_name})
WHERE (func)-[:CHILDREGION]->()-[:CHILDBLOCK]->()
RETURN func.uid, func.func_name, func.location;
```

```cypher
MATCH (func:OPERATIONS {name: "hl.func", fs_raw_name: $func_raw_name})
WHERE (func)-[:CHILDREGION]->()-[:CHILDBLOCK]->()
RETURN func.uid, func.func_name, func.location;
```

## Name to Global Variable


## Name to Struct/Union/Class


## Name to Typedef


# Call Chain

## Direct Caller

Given a callee uid, return its callers and call operations

```cypher
MATCH (callee:OPERATIONS {uid: $callee_uid, name: "hl.func"})
MATCH (:STRINGATTRS {hash: callee.hash})<-[:ARGATTR]-(call:OPERATIONS {name: "hl.call"})
(()<-[:CHILDINST]-()<-[:CHILDBLOCK]-()<-[:CHILDREGION]-()){1,} (caller:OPERATIONS {name: "hl.func"})
ORDER BY caller.uid
RETURN caller.uid, caller.func_name, caller.fs_raw_name, caller.location, call.uid, call.location;
```

# Data Flow


# Other

## Operations Near Source Location

Use when the exact source line may map to nearby MLIR operations.

```cypher
MATCH (op:OPERATIONS)
WHERE op.location[1] ENDS WITH $file_name
  AND toInteger(op.location[2]) >= $line_start
  AND toInteger(op.location[2]) <= $line_end
RETURN op.uid AS uid, op.name AS name, op.func_name AS func_name, op.fs_raw_name AS raw_name,
       op.location AS location
ORDER BY toInteger(op.location[2]), uid
LIMIT 100
```

## Direct Function Body Operations

Use after finding a function uid. Lists operations directly inside the function body blocks.

```cypher
MATCH (f:OPERATIONS {uid: $func_uid})
  -[:CHILDREGION]->(:REGIONS)
  -[:CHILDBLOCK]->(:BLOCKS)
  -[ie:CHILDINST]->(op:OPERATIONS)
RETURN op.uid AS uid, op.name AS name, op.location AS location, ie.idx AS idx
ORDER BY ie.idx
LIMIT 200
```

## Nested Function Body Operations

Use when the interesting operation may be nested under `hl.if`, loops, scopes, or expression regions.

```cypher
MATCH (f:OPERATIONS {uid: $func_uid})
  -[:CHILDREGION|CHILDBLOCK|CHILDINST*]->(op:OPERATIONS)
WHERE op.uid <> f.uid
RETURN DISTINCT op.uid AS uid, op.name AS name, op.location AS location
ORDER BY uid
LIMIT 300
```

## Direct Callees In Function

Use to list direct callees inside a function.

```cypher
MATCH (f:OPERATIONS {uid: $func_uid})
  -[:CHILDREGION|CHILDBLOCK|CHILDINST*]->(call:OPERATIONS {name: 'hl.call'})
OPTIONAL MATCH (call)-[:ARGATTR {idx: 0}]->(callee_attr:STRINGATTRS)
RETURN DISTINCT call.uid AS call_uid, coalesce(call.callee, callee_attr.value) AS callee,
       call.location AS location
ORDER BY callee, call_uid
LIMIT 100
```

## Operation Operands

Use to inspect input values and their defining operations.

```cypher
MATCH (op:OPERATIONS {uid: $op_uid})-[ae:ARGVALUE]->(v:VALUES)
OPTIONAL MATCH (def:OPERATIONS)-[re:RESVALUE]->(v)
RETURN ae.idx AS arg_idx, v.uid AS value_uid, def.uid AS def_uid, def.name AS def_name,
       re.idx AS def_result_idx, def.location AS def_location
ORDER BY ae.idx
```

## Operation Results And Uses

Use to inspect output values and operations that consume them.

```cypher
MATCH (op:OPERATIONS {uid: $op_uid})-[re:RESVALUE]->(v:VALUES)
OPTIONAL MATCH (v)<-[ae:ARGVALUE]-(use:OPERATIONS)
RETURN re.idx AS result_idx, v.uid AS value_uid, use.uid AS use_uid, use.name AS use_name,
       ae.idx AS use_arg_idx, use.location AS use_location
ORDER BY re.idx, use_uid, use_arg_idx
LIMIT 100
```

## Delete Operations

Use for C++ delete-related vulnerabilities.

```cypher
MATCH (del:OPERATIONS)
WHERE del.name IN ['hl.delete', 'hl.delete.array', 'hl.delete.virtual']
OPTIONAL MATCH (del)-[ae:ARGVALUE {idx: 0}]->(ptr:VALUES)
OPTIONAL MATCH (def:OPERATIONS)-[re:RESVALUE]->(ptr)
RETURN del.uid AS delete_uid, del.name AS delete_op, del.location AS delete_location,
       ptr.uid AS ptr_value_uid, def.uid AS ptr_def_uid, def.name AS ptr_def_name,
       def.location AS ptr_def_location
ORDER BY delete_uid
LIMIT 100
```
