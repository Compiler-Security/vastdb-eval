---
name: write-cypher
description: Use before writing or executing Cypher queries against VAST DB for code analysis. Supports code-navigation queries that map source locations to operation uid handles, find enclosing functions, locate symbol definitions or references, find direct callers/callees, and trace def-use relationships to return source locations for source reading. Load understand-mlir-schema when schema details are needed to construct queries or interpret query results.
---

# Overview

Use this skill for VAST DB Cypher query patterns. Treat `references/query-templates.md` as the first source for query shapes. Construct a new query only when no template matches the analysis need.

Use VAST DB as a code navigation tool. The normal loop is: source location or name -> operation `uid` -> next source location. Read source code at returned locations; use operation `uid`s as handles for follow-up VAST DB queries.

# Workflow

1. Before the first `run_cypher` call, read `references/query-templates.md`.
2. For each query, choose a template from `references/query-templates.md` before writing Cypher.
3. If a template matches, reuse its query shape as-is and only substitute parameters or add narrow filters.
4. If no template matches, construct a new Cypher query using the VAST DB schema and the closest related template as guidance.
5. Return source locations for source reading and operation `uid`s for follow-up VAST DB queries.

# Required Navigation Attempt

For each source location anchor, a VAST DB navigation attempt is incomplete if it only runs `Location to Operations`, name lookup, enclosing-function lookup, or call-chain lookup.

Default source-location attempt:

1. Use `Location to Operations` or `Macro Location to Operations`.
2. Select the operation `uid` matching the source construct under analysis.
3. Use that `uid` with `Dataflow: Use-Def Chain`.
4. Run at least one next query from the returned definition according to the Navigation State Machine.

The default source-location attempt does not stop at `Dataflow: Use-Def Chain`. After `Dataflow: Use-Def Chain` returns a definition, continue with the next query required by that definition kind in the Navigation State Machine.

# Navigation Contract

Query Inputs:

- `location`: comes from the initial vulnerability source/sink/location or from earlier VAST DB results.
- `name`: comes from source reading at locations returned by earlier VAST DB results.
- `uid`: comes from earlier VAST DB results.

Query Outputs:

- `location`: marks source lines that deserve more attention during source reading.
- `uid`: is the handle for the next VAST DB query.
- `name`: helps choose the next query.

# Navigation State Machine

## Location to Operations

Templates: `Location to Operations`, `Macro Location to Operations`.

Use this group to map a source location to operation `uid`s.

Choose:

- `Location to Operations`: use a source file name and line number to find operations mapped to that source location.
- `Macro Location to Operations`: use a macro definition file name and line number to find operations mapped to that macro location.

A source line usually maps to many operations. Select the operation matching the code element under analysis, then use its `uid` as the handle for later queries:

- Member access: `hl.member`
- Array index: `hl.subscript`
- Dereference: `hl.deref`
- Function call argument: `hl.call`
- Function parameter: `hl.param`

Next query:

- Use the selected operation `uid` with `Dataflow: Use-Def Chain`.

## Dataflow: Use-Def Chain

Templates: `Use to Def`, `One-step Use to Def`, `Global Reference to Symbol`.

Use this group when you have a use operation `uid` and need to find where that value comes from.

Choose:

- `Use to Def`: use a use operation `uid` and operand index to find the source definition operation.
- `One-step Use to Def`: use a use operation `uid` and operand index to find the directly defining operation.
- `Global Reference to Symbol`: use an `hl.globalref` operation `uid` to find the referenced global `hl.var` or `hl.func` operation.

Useful definition operations include:

- `hl.var`: local variable definition.
- `hl.param`: function parameter definition.
- `hl.call`: function return value definition.
- `hl.globalref`: global symbol reference value. Treat the `hl.globalref` result as the definition handle for later uses of that global symbol.

Next query:

- When the returned definition is `hl.var`, `hl.param`, or `hl.globalref`, use its `uid` with `Dataflow: Def-Use Chain`.
- When the returned definition is `hl.param`, use its `uid` with `Enclosing Function`, then use the returned function `uid` with `Direct Caller`.
- When the returned definition is `hl.call`, use its `uid` with `Call Operation to Callee`.
- When the returned definition is `hl.globalref`, use its `uid` with `Global Reference to Symbol`, then use the returned global symbol `uid` with `Global Symbol to Reference`.

## Dataflow: Def-Use Chain

Templates: `Def to Call Argument`, `Def to Assigned`, `Def to Assign`, `One-step Def to Use`, `Global Symbol to Reference`.

Use this group when you have a definition operation `uid` and need to find where the value goes next.

Choose:

- `Def to Call Argument`: use a definition operation `uid` to find calls receiving that value as an argument.
- `Def to Assigned`: use a definition operation `uid` to find assignments where that value becomes the LHS destination; returns the RHS source calculation operation.
- `Def to Assign`: use a definition operation `uid` to find assignments where that value becomes the RHS source; returns the LHS target calculation operation.
- `One-step Def to Use`: use a definition operation `uid` to find operations that directly consume its result value.
- `Global Symbol to Reference`: use a global `hl.var` or `hl.func` operation `uid` to find `hl.globalref` operations that reference it.

Next query:

- When the returned operation is `hl.call`, use its `uid` with `Call Operation to Callee`.
- When the returned operation is an assignment RHS, use its `uid` with `Use to Def`.
- When the returned operation is an assignment LHS, use its `uid` with `Use to Def` to find the new definition, then use that definition `uid` with `Dataflow: Def-Use Chain`.

## Call Chain

Templates: `Direct Caller`, `Call Operation to Callee`.

Use this group to move between function definitions and call operations.

Choose:

- `Direct Caller`: use a callee `hl.func` operation `uid` to find caller functions and call operations.
- `Call Operation to Callee`: use an `hl.call` operation `uid` to find the callee `hl.func` operation.

Next query:

- When the returned operation is a caller function, use its `uid` with `Direct Caller` to continue moving up the call chain.
- When the returned operation is a call operation, use its `uid` with `Dataflow: Use-Def Chain`.
- When the returned operation is a callee function, use its `uid` with `One-step Child Operations` to find parameters such as `hl.param`.

## Name to Operations

Templates: `Name to Function`, `Name to Variable`, `Name to Struct/Union/Class`, `Name to Typedef`.

Use this group when source reading reveals a relevant function, variable, record type, or typedef name. Names usually come from source locations returned by earlier VAST DB queries.

Choose:

- `Name to Function`: use a function name to find matching `hl.func` operations.
- `Name to Variable`: use a variable name to find matching `hl.var` operations.
- `Name to Struct/Union/Class`: use a record type name to find matching `hl.struct`, `hl.union`, or `hl.class` operations.
- `Name to Typedef`: use a typedef or using name to find matching `hl.typedef` operations.

Next query:

- When the returned handle is a function `uid`, use it with `Call Chain` or `Code Structure`.
- When the returned handle is a variable `uid`, use it with `Dataflow: Def-Use Chain`.

## Code Structure

Templates: `Enclosing Function`, `One-step Enclosing`, `One-step Child Operations`.

Use this group to move between an operation and its surrounding or directly contained operations.

Choose:

- `Enclosing Function`: use an operation `uid` to find the containing `hl.func` operation.
- `One-step Enclosing`: use an operation `uid` to find the immediately enclosing operation.
- `One-step Child Operations`: use a function or structured operation `uid` to find directly contained operations such as `hl.param`.

Next query:

- When the returned handle is an enclosing function `uid`, use it with `Direct Caller`.
- When the returned handle is a child operation `uid`, use it with `Dataflow: Use-Def Chain`, `Dataflow: Def-Use Chain`, `Call Chain`, or `Code Structure`.

# Query Rules

- Load `understand-mlir-schema` when query construction or result interpretation depends on schema details.
- Use small exploratory queries with `LIMIT` when schema details such as operation names, properties, node and relationship labels are uncertain.
- Return only properties needed for navigation and analysis. Common fields are operation `uid`, operation `location`, function `func_name`, variable `varName`, operation `name`, and relationship `idx`.
- Order indexed relationships by `idx`.
- Generate read-only queries by default. Do not use `CREATE`, `MERGE`, `SET`, `DELETE`, or `REMOVE`.
- Use parameter placeholders such as `$func_name`, `$op_uid`, `$file_name`, `$line`, and `$callee`.
- Do not call `run_cypher` before reading `references/query-templates.md`.
- Do not invent node and relationship labels, properties, or operation names. Verify unfamiliar schema details with `understand-mlir-schema` or exploratory queries.

# Common Patterns

Read `references/query-templates.md` for reusable Cypher templates.
