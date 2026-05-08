# VAST DB Cypher Query Templates

Use these templates before inventing new Cypher.

# Navigation Guide

Use these templates to move between source locations and operation `uid`s.

- Use source locations to read code.
- Use operation `uid`s as query handles.
- Template outputs should expose operation `uid`s for follow-up queries and source `location`s for source reading.
- Start from a source location with `Location to Operations`.
- From a use operation, use `Use to Def` first. If it does not return a useful definition, use `One-step Use to Def` repeatedly.
- From a definition operation, use `Def to Call Argument`, `Def to Assigned`, or `Def to Assign`.
- From a function operation, use `Direct Caller`.
- From an `hl.call` operation, use `Call Operation to Callee`.
- From a name found while reading source, use the `Name to ...` templates.

# Location to Operations

## Location to Operations

Input: source file name and line number.
Output: operations mapped to that source location.

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

Input: macro definition file name and line number.
Output: operations mapped to that macro definition location.

```cypher
MATCH (op:OPERATIONS)
WHERE op.location[0] = 'M'
  AND op.location[7] ENDS WITH $file_name
  AND toInteger(op.location[8]) = $line
RETURN op.uid, op.name, op.location
ORDER BY op.uid
LIMIT 50;
```

# Name to Operations

Use these templates when source reading reveals a relevant name. The output operation `uid` can be used for later VAST DB queries; the output `location` should be read in source.

## Name to Function

Input: function name.
Output: matching `hl.func` operations.

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

Input: variable name.
Output: matching `hl.var` operations.

```cypher
MATCH (var:OPERATIONS {name: "hl.var"})
WHERE var.varName ENDS WITH $var_name
RETURN var.uid, var.varName, var.location;
```

## Name to Struct/Union/Class

Input: struct, union, or class name.
Output: matching `hl.struct`, `hl.union`, or `hl.class` operations.

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

Input: typedef or using name.
Output: matching `hl.typedef` operations.

```cypher
MATCH (td:OPERATIONS {name: "hl.typedef"})-[:ARGATTR {idx: 0}]->(s:STRINGATTRS)
WHERE s.value ENDS WITH $typedef_name
RETURN td.uid, s.value, td.location;
```

# Call Chain

## Direct Caller

Input: callee `hl.func` operation uid.
Output: caller `hl.func` operations and `hl.call` operations.

```cypher
MATCH (callee:OPERATIONS {uid: $callee_uid, name: "hl.func"})
MATCH (:STRINGATTRS {hash: callee.hash})<-[:ARGATTR {idx: 0}]-(call:OPERATIONS {name: "hl.call"})
  (()<-[:CHILDINST]-()<-[:CHILDBLOCK]-()<-[:CHILDREGION]-()){1,}
  (caller:OPERATIONS {name: "hl.func"})
ORDER BY caller.uid
RETURN caller.uid, caller.func_name, caller.location, call.uid, call.location;
```

## Call Operation to Callee

Input: `hl.call` operation uid.
Output: callee `hl.func` operation.

- Prefer a function definition; if no definition exists, return a declaration. `is_def` indicates whether the returned callee has a body.

```cypher
MATCH (call:OPERATIONS {uid: $call_uid, name: "hl.call"})-[:ARGATTR {idx: 0}]->(s:STRINGATTRS)
MATCH (callee:OPERATIONS {name: "hl.func", hash: s.hash})
OPTIONAL MATCH (callee)-[e:CHILDREGION]->()-[:CHILDBLOCK]->()
WITH callee, e IS NOT NULL AS is_def
ORDER BY is_def DESC LIMIT 1
RETURN callee.uid, callee.func_name, callee.location, is_def;
```

# Dataflow: Use-Def Chain

## Use to Def

Input: use operation uid and operand index.
Output: source definition operation found by walking backward through def-use links.

- The final definition is limited to `hl.var`, `hl.param`, `hl.call`, `hl.indirect_call` or `hl.globalref`.
- Intermediate operations are returned as `mid_infos`.
- Paths with an intermediate `hl.call` or `hl.indirect_call` are excluded.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[:ARGVALUE {idx: $arg_idx}]->()
  (()<-[:RESVALUE]-(mids)-[mid_args:ARGVALUE]->()){0,}
  ()<-[:RESVALUE]-(def:OPERATIONS)
WHERE def.name IN ["hl.var", "hl.param", "hl.call", "hl.indirect_call", "hl.globalref"]
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

## One-step Use to Def

Input: use operation uid and operand index.
Output: operation that directly defines that operand value.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[:ARGVALUE {idx: $arg_idx}]->(v:VALUES)<-[:RESVALUE]-(def:OPERATIONS)
RETURN def.uid, def.name, def.location;
```

## Global Reference to Symbol

Input: `hl.globalref` operation uid.
Output: referenced `hl.func` or `hl.var` operations.

```cypher
MATCH (ref:OPERATIONS {uid: $op_uid, name: "hl.globalref"})
MATCH (func:OPERATIONS {name: "hl.func", func_name: ref.symName})
OPTIONAL MATCH (func)-[e:CHILDREGION]->()-[:CHILDBLOCK]->()
WITH func, e IS NOT NULL AS is_def
ORDER BY is_def DESC LIMIT 1
RETURN func.uid, func.func_name, func.location, is_def;
```

```cypher
MATCH (ref:OPERATIONS {uid: $op_uid, name: "hl.globalref"})
MATCH (var:OPERATIONS {name: "hl.var", varName: ref.symName})
RETURN var.uid, var.varName, var.location;
```

# Dataflow: Def-Use Chain

## Def to Call Argument

Input: definition operation uid.
Output: call operations that receive its result value as an argument.

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

## Def to Assigned

Input: definition operation uid.
Output: `hl.assign` operations where its result value becomes the LHS destination, plus the RHS source calculation operation.

- The returned `value_op` calculates the RHS source value assigned into that destination.
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

## Def to Assign

Input: definition operation uid.
Output: `hl.assign` operations where its result value becomes the RHS source assigned into another location, plus the LHS target calculation operation.

- The returned `target_op` calculates the LHS destination receiving this value.
- Intermediate operations are returned as `mid_infos`.
- Paths with an intermediate `hl.call` or `hl.indirect_call` are excluded.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[:RESVALUE]->(:VALUES)
  (()<-[mid_args:ARGVALUE]-(mids)-[:RESVALUE]->()){0,}
  ()<-[:ARGVALUE {idx: 0}]-(assign_op:OPERATIONS)-[:ARGVALUE {idx: 1}]->(:VALUES)<-[:RESVALUE]-(target_op:OPERATIONS)
WHERE assign_op.name IN ["hl.assign"]
  AND NONE(mid IN mids WHERE mid.name IN ["hl.call", "hl.indirect_call"])
RETURN
  assign_op.uid, assign_op.name, assign_op.location,
  target_op.uid, target_op.name, target_op.location,
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

Input: definition operation uid.
Output: operations that directly use its result value.

- Most operations have one result value, so this template does not filter `RESVALUE` by edge `idx`.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[:RESVALUE]->(v:VALUES)<-[ae:ARGVALUE]-(use:OPERATIONS)
RETURN use.uid, use.name, use.location, ae.idx;
```

## Global Symbol to Reference

Input: global `hl.var` or `hl.func` operation uid.
Output: `hl.globalref` operations that produce symbol-reference values for that global symbol.

- Operations that use a global variable or function consume the result value of `hl.globalref`, not a result value from the global `hl.var` or `hl.func`.
- For function symbols, this means function-pointer uses, not direct `hl.call` calls.

```cypher
MATCH (sym:OPERATIONS {uid: $op_uid, name: "hl.func"})
MATCH (ref:OPERATIONS {name: "hl.globalref", symName: sym.func_name})
RETURN ref.uid, ref.location;
```

```cypher
MATCH (sym:OPERATIONS {uid: $op_uid, name: "hl.var"})
MATCH (ref:OPERATIONS {name: "hl.globalref", symName: sym.varName})
RETURN ref.uid, ref.location;
```

# Code Structure

## Enclosing Function

Input: operation uid.
Output: enclosing `hl.func` operation.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})
  (()<-[:CHILDINST]-()<-[:CHILDBLOCK]-()<-[:CHILDREGION]-()){1,}
  (func:OPERATIONS {name: "hl.func"})
RETURN func.uid, func.func_name, func.location;
```

## One-step Enclosing

Input: operation uid.
Output: immediately enclosing operation, such as `builtin.module`, `hl.func`, `core.scope`, or `hl.if`.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})<-[ie:CHILDINST]-()<-[:CHILDBLOCK]-()<-[re:CHILDREGION]-(enclos:OPERATIONS)
RETURN enclos.uid, enclos.name, enclos.location, ie.idx, re.idx;
```

## One-step Child Operations

Input: operation uid.
Output: operations directly contained in its immediate child regions and blocks.

```cypher
MATCH (:OPERATIONS {uid: $op_uid})-[re:CHILDREGION]->()-[:CHILDBLOCK]->()-[ri:CHILDINST]->(child:OPERATIONS)
ORDER BY re.idx, ri.idx
RETURN child.uid, child.name, child.location, re.idx, ri.idx
LIMIT 100;
```
