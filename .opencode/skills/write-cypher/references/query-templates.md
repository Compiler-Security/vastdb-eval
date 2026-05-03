# VAST DB Cypher Query Templates

Use these templates before inventing new Cypher. Keep queries read-only unless the user explicitly asks to modify the database.

## Function By Name

Use when the user gives a C/C++ function name and needs the corresponding `hl.func` or `ll.func`.

```cypher
MATCH (f:OPERATIONS)
WHERE f.name IN ['hl.func', 'll.func']
  AND (f.func_name = $func_name OR f.fs_raw_name = $func_name)
RETURN f.uid AS uid, f.name AS op, f.func_name AS func_name, f.fs_raw_name AS raw_name,
       f.fs_class_name AS class_name, f.location AS location
ORDER BY uid
LIMIT 20
```

## Operations By Source Location

Use when the user gives a file and line number. `location` is normally `['F', file, line, column]`.

```cypher
MATCH (op:OPERATIONS)
WHERE op.location[1] ENDS WITH $file_name
  AND toInteger(op.location[2]) = $line
RETURN op.uid AS uid, op.name AS name, op.func_name AS func_name, op.fs_raw_name AS raw_name,
       op.location AS location
ORDER BY uid
LIMIT 50
```

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

## Enclosing Functions For Operation

Use when an operation uid is known and you need the containing function.

```cypher
MATCH (f:OPERATIONS)-[:CHILDREGION|CHILDBLOCK|CHILDINST*]->(op:OPERATIONS {uid: $op_uid})
WHERE f.name IN ['hl.func', 'll.func']
RETURN DISTINCT f.uid AS func_uid, f.name AS func_op, f.func_name AS func_name,
       f.fs_raw_name AS raw_name, f.location AS func_location
ORDER BY func_uid
LIMIT 20
```

## Direct Calls By Callee

Use for direct `hl.call` sites. The callee may be stored as `call.callee` or as `ARGATTR {idx:0}` depending on database version.

```cypher
MATCH (call:OPERATIONS {name: 'hl.call'})
OPTIONAL MATCH (call)-[:ARGATTR {idx: 0}]->(callee_attr:STRINGATTRS)
WHERE call.callee = $callee OR callee_attr.value = $callee
RETURN call.uid AS uid, call.name AS name, coalesce(call.callee, callee_attr.value) AS callee,
       call.location AS location
ORDER BY uid
LIMIT 100
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

## Direct Callers Of Function Name

Use to find functions containing calls to a callee name.

```cypher
MATCH (caller:OPERATIONS)-[:CHILDREGION|CHILDBLOCK|CHILDINST*]->(call:OPERATIONS {name: 'hl.call'})
OPTIONAL MATCH (call)-[:ARGATTR {idx: 0}]->(callee_attr:STRINGATTRS)
WHERE caller.name IN ['hl.func', 'll.func']
  AND (call.callee = $callee OR callee_attr.value = $callee)
RETURN DISTINCT caller.uid AS caller_uid, caller.func_name AS caller_name,
       caller.fs_raw_name AS caller_raw_name, call.uid AS call_uid, call.location AS call_location
ORDER BY caller_name, call_uid
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

## Load Store Operations

Use for low-level memory reads/writes.

```cypher
MATCH (op:OPERATIONS)
WHERE op.name IN ['ll.load', 'll.store']
RETURN op.uid AS uid, op.name AS name, op.location AS location
ORDER BY uid
LIMIT 100
```
