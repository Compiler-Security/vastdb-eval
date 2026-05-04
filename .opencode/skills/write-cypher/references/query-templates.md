# VAST DB Cypher Query Templates

Use these templates before inventing new Cypher

# Code Structure

## Location to Operations

Given a source file name and line number, return operations mapped to that source location.

- Normal location format: `['F', file, line, column]`.
- Macro-expanded location format: `['M', 2, 'F', file_expand, line_expand, column_expand, 'F', file_macro, line_macro, column_macro]`.

```cypher
MATCH (op:OPERATIONS)
WHERE (op.location[0] = 'F'
        AND op.location[1] ENDS WITH $file_name
        AND toInteger(op.location[2]) = $line)
    OR (op.location[0] = 'M'
        AND op.location[3] ENDS WITH $file_name
        AND toInteger(op.location[4]) = $line)
RETURN op.uid, op.name, op.location
ORDER BY op.uid
LIMIT 50;
```

## Macro Location to Operations

Given a file name and line number of a macro definition, return its related operations

```cypher
MATCH (op:OPERATIONS)
WHERE op.location[0] = 'M'
  AND op.location[7] ENDS WITH $file_name
  AND toInteger(op.location[8]) = $line
RETURN op.uid, op.name, op.location
ORDER BY op.uid
LIMIT 50;
```

## One-step Enclosing

Given an operation uid, return its enclosing operation, such as `builtin.module`, `hl.func`, `core.scope`, `hl.if`

```cypher
MATCH (:OPERATIONS {uid: $op_uid})<-[ie:CHILDINST]-()<-[:CHILDBLOCK]-()<-[re:CHILDREGION]-(enclos:OPERATIONS)
RETURN enclos.uid, enclos.name, enclos.location, ie.idx, re.idx;
```

## Enclosing Function

Given an operation uid, return its enclosing function

```cypher
MATCH (:OPERATIONS {uid: $op_uid})
  (()<-[:CHILDINST]-()<-[:CHILDBLOCK]-()<-[:CHILDREGION]-()){1,}
  (func:OPERATIONS {name: "hl.func"})
RETURN func.uid, func.func_name, func.location;
```

## One-step Child Operations

Given an operation uid, return operations directly contained in its immediate child regions and blocks.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[re:CHILDREGION]->()-[:CHILDBLOCK]->()-[ri:CHILDINST]->(child:OPERATIONS)
ORDER BY re.idx, ri.idx
RETURN child.uid, child.name, child.location, re.idx, ri.idx
LIMIT 100;
```

## Child Operations

Given an operation uid, return operations contained under it at any nesting depth.

- Add a `WHERE` filter when only certain operation kinds are needed, such as `hl.call`.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})
  (()-[:CHILDREGION]->()-[:CHILDBLOCK]->()-[:CHILDINST]->()){1,}
  (child:OPERATIONS)
RETURN child.uid, child.name, child.location
LIMIT 200;
```

```cypher
MATCH (:OPERATIONS {uid: $op_uid})
  (()-[:CHILDREGION]->()-[:CHILDBLOCK]->()-[:CHILDINST]->()){1,}
  (child:OPERATIONS)
WHERE child.name IN ["hl.call"]
RETURN child.uid, child.name, child.location
LIMIT 200;
```

## Name to Function

Given a function name, return a matching `hl.func` operation.

- Use `func_name` when the name comes from VAST DB query results. It is the demangled function name with qualifiers and prototype, such as `NS::C::foo(int *)`.
- Use `fs_raw_name` when the name comes from source code. It is the source-level function name, such as `foo`.
- Prefer a function definition; if no definition exists, return a declaration. `is_def` indicates whether the returned `hl.func` has a body.

```cypher
MATCH (func:OPERATIONS {name: "hl.func", func_name: $func_name})
OPTIONAL MATCH (func)-[e:CHILDREGION]->()-[:CHILDBLOCK]->()
WITH func, e IS NOT NULL AS is_def
ORDER BY is_def DESC LIMIT 1
RETURN func.uid, func.func_name, func.location, is_def;
```

```cypher
MATCH (func:OPERATIONS {name: "hl.func", fs_raw_name: $func_raw_name})
OPTIONAL MATCH (func)-[e:CHILDREGION]->()-[:CHILDBLOCK]->()
WITH func, e IS NOT NULL AS is_def
ORDER BY is_def DESC
WITH func.func_name AS func_name, collect(func)[0] AS func, collect(is_def)[0] AS is_def
RETURN func.uid, func.func_name, func.location, is_def;
```

## Name to Variable

Given a variable name, return matching `hl.var` operations.

```cypher
MATCH (var:OPERATIONS {name: "hl.var"})
WHERE var.varName ENDS WITH $var_name
RETURN var.uid, var.varName, var.location;
```

## Name to Struct/Union/Class

Given a struct/union/class name, return matching `hl.struct`/`hl.union`/`hl.class` operations.

- For struct in C, op name is `hl.struct`
- For struct and class in C++, op name is `hl.class`
- For union, op name is `hl.union`

```cypher
MATCH (s:OPERATIONS {name: "hl.struct"})
WHERE s.structName ENDS WITH $struct_name
RETURN s.uid, s.structName, s.location;
```

```cypher
MATCH (c:OPERATIONS {name: "hl.class"})
WHERE c.className ENDS WITH $class_name
RETURN c.uid, c.className, c.location;
```

```cypher
MATCH (u:OPERATIONS {name: "hl.union"})
WHERE u.unionName ENDS WITH $union_name
RETURN u.uid, u.unionName, u.location;
```

## Name to Typedef

Given a typedef/using name, return matching `hl.typedef` operations

```cypher
MATCH (td:OPERATIONS {name: "hl.typedef"})-[:ARGATTR {idx: 0}]->(s:STRINGATTRS)
WHERE s.value ENDS WITH $typedef_name
RETURN td.uid, s.value, td.location;
```

# Call Chain

## Direct Caller

Given a callee `hl.func` uid, return caller `hl.func` operations and `hl.call` operations.

```cypher
MATCH (callee:OPERATIONS {uid: $callee_uid, name: "hl.func"})
MATCH (:STRINGATTRS {hash: callee.hash})<-[:ARGATTR]-(call:OPERATIONS {name: "hl.call"})
  (()<-[:CHILDINST]-()<-[:CHILDBLOCK]-()<-[:CHILDREGION]-()){1,}
  (caller:OPERATIONS {name: "hl.func"})
ORDER BY caller.uid
RETURN caller.uid, caller.func_name, caller.location, call.uid, call.location;
```

## Call Operation to Callee

Given an `hl.call` uid, return its callee `hl.func` operation.

- Prefer a function definition; if no definition exists, return a declaration. `is_def` indicates whether the returned callee has a body.

```cypher
MATCH (call:OPERATIONS {uid: $call_uid, name: "hl.call"})-[:ARGATTR {idx: 0}]->(s:STRINGATTRS)
MATCH (callee:OPERATIONS {name: "hl.func", hash: s.hash})
OPTIONAL MATCH (callee)-[e:CHILDREGION]->()-[:CHILDBLOCK]->()
WITH callee, e IS NOT NULL AS is_def
ORDER BY is_def DESC LIMIT 1
RETURN callee.uid, callee.func_name, callee.location, is_def;
```

## Direct Callees

Given a caller `hl.func` uid, return each direct `hl.call` operation inside it and the resolved callee `hl.func` operation.

- This combines `Child Operations` with `Call Operation to Callee`.
- For each call, prefer a callee function definition; if no definition exists, return a declaration. `is_def` indicates whether the returned callee has a body.

```cypher
MATCH (:OPERATIONS {uid: $caller_uid, name: "hl.func"})
  (()-[:CHILDREGION]->()-[:CHILDBLOCK]->()-[:CHILDINST]->()){1,}
  (call:OPERATIONS)
WHERE call.name IN ["hl.call"]
CALL {
  WITH call
  MATCH (call)-[:ARGATTR {idx: 0}]->(s:STRINGATTRS)
  MATCH (callee:OPERATIONS {name: "hl.func", hash: s.hash})
  OPTIONAL MATCH (callee)-[e:CHILDREGION]->()-[:CHILDBLOCK]->()
  WITH callee, e IS NOT NULL AS is_def
  ORDER BY is_def DESC LIMIT 1
  RETURN callee, is_def
}
RETURN callee.uid, callee.func_name, callee.location, is_def, call.uid, call.location;
```

# Data Flow

## One-step Use to Def

Given an operation uid and an operand index, return the operation that directly defines that operand value.

- This follows one `ARGVALUE -> VALUES <- RESVALUE` step.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[:ARGVALUE {idx: $arg_idx}]->(v:VALUES)<-[:RESVALUE]-(def:OPERATIONS)
RETURN def.uid, def.name, def.location;
```

## Use to Def

Given an operation uid and an operand index, walk backward through def-use links and return the source definition operation.

- The final definition is limited to `hl.var`, `hl.param`, `hl.call`, `hl.indirect_call` or `hl.globref`.
- Intermediate operations are returned as `mid_infos`.
- Paths with an intermediate `hl.call` or `hl.indirect_call` are excluded.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[:ARGVALUE {idx: $arg_idx}]->()
  (()<-[:RESVALUE]-(mids)-[mid_args:ARGVALUE]->()){0,}
  ()<-[:RESVALUE]-(def:OPERATIONS)
WHERE def.name IN ["hl.var", "hl.param", "hl.call", "hl.indirect_call", "hl.globref"]
  AND NONE(mid IN mids WHERE mid.name IN ["hl.call", "hl.indirect_call"])
RETURN
  def.uid, def.name, def.location,
  [
    i IN range(0, size(mids) - 1) |
    {
      uid: mids[i].uid,
      name: mids[i].name,
      location: mids[i].location,
      argidx: mid_args[i].idx
    }
  ] AS mid_infos;
```

## One-step Def to Use

Given an operation uid, return operations that directly use its result value.

- Most operations have one result value, so this template does not filter `RESVALUE` by edge `idx`.
- This follows one `RESVALUE -> VALUES <- ARGVALUE` step.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[:RESVALUE]->(v:VALUES)<-[ae:ARGVALUE]-(use:OPERATIONS)
RETURN use.uid, use.name, use.location, ae.idx;
```

## Global Symbol References

Given a global variable or function name, return `hl.globalref` operations that produce symbol-reference values.

- Operations that use a global variable or function consume the result value of `hl.globalref`, not a result value from the global `hl.var` or `hl.func`.
- For function symbols, this means function-pointer uses, not direct `hl.call` calls.
- For function symbols, `$sym_name` is the `func_name`, not the `fs_raw_name`.

```cypher
MATCH (ref:OPERATIONS {name: "hl.globalref", symName: $sym_name})
RETURN ref.uid, ref.location;
```

## Def to Call Argument

Given an operation uid, follow its result value forward and return call operations that receive it as an argument.

- Intermediate operations are returned as `mid_infos`.
- Paths with an intermediate `hl.call` or `hl.indirect_call` are excluded.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[:RESVALUE]->(:VALUES)
  (()<-[mid_args:ARGVALUE]-(mids)-[:RESVALUE]->()){0,}
  ()<-[arg:ARGVALUE]-(call:OPERATIONS)
WHERE call.name IN ["hl.call", "hl.indirect_call"]
  AND NONE(mid IN mids WHERE mid.name IN ["hl.call", "hl.indirect_call"])
RETURN
  call.uid, call.name, call.location, arg.idx,
  [
    i IN range(0, size(mids) - 1) |
    {
      uid: mids[i].uid,
      name: mids[i].name,
      location: mids[i].location,
      argidx: mid_args[i].idx
    }
  ] AS mid_infos;
```

## Def to Assign

Given an operation uid, follow its result value forward and return `hl.assign` operations where it becomes the LHS destination.

- The returned `value_op` defines the RHS source value assigned into that destination.
- Intermediate operations are returned as `mid_infos`.
- Paths with an intermediate `hl.call` or `hl.indirect_call` are excluded.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[:RESVALUE]->(:VALUES)
  (()<-[mid_args:ARGVALUE]-(mids)-[:RESVALUE]->()){0,}
  ()<-[:ARGVALUE {idx: 1}]-(assign_op:OPERATIONS)-[:ARGVALUE {idx: 0}]->(:VALUES)<-[:RESVALUE]-(value_op:OPERATIONS)
WHERE assign_op.name IN ["hl.assign"]
  AND NONE(mid IN mids WHERE mid.name IN ["hl.call", "hl.indirect_call"])
RETURN
  assign_op.uid, assign_op.name, assign_op.location,
  value_op.uid, value_op.name, value_op.location,
  [
    i IN range(0, size(mids) - 1) |
    {
      uid: mids[i].uid,
      name: mids[i].name,
      location: mids[i].location,
      argidx: mid_args[i].idx
    }
  ] AS mid_infos;
```
