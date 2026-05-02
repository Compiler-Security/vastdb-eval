# VAST MLIR Graph Schema (HighLevel/LowLevel Dialects)

## Overview
- Database: neo4j
- Purpose: store VAST MLIR (HighLevel and LowLevel dialects) as a graph for analysis
- Version: v1
- Owner: vast::db / vast/Database

## Naming Conventions
- Node labels: UPPER_SNAKE plural (e.g., OPERATIONS, VALUES)
- Relationship types: UPPER_SNAKE (e.g., ARGVALUE, CHILDREGION)
- Properties: lower_snake for new fields, legacy fields as-is (e.g., regType)
- Primary keys: `uid` on all node types

## Node Definitions

### OPERATIONS
Description: an MLIR operation node
Primary key: uid

Properties:
- `uid`: int — unique operation id (required)
- `name`: string — operation name (required)
- `func_name`: string — function name for global functions like `hl.func` and `ll.func` (optional)
- `varName`: string — global variable name for global variable declaration like `hl.var` (optional)
- `mod_name`: string — file name for `builtin.module` operation
- `location`: Normally a 4-element vector where the first element is the string `"F"`, the second is the file name, the third is the line number, and the fourth is the column number.
- Additional implementation-defined fields are allowed as long as they do not conflict with the standard fields above.

### VALUES
Description: an MLIR SSA value
Primary key: uid

Properties:
- `uid`: int — unique value id (required)

### INTATTRS
Description: integer attribute (core::IntegerAttr or mlir::IntegerAttr)
Primary key: uid

Properties:
- `uid`: int — unique attr id (required)
- `value`: int — integer value (required)
- `attr_type`: int — 0 for core::IntegerAttr, 1 for mlir::IntegerAttr (required)

### FLOATATTRS
Description: float attribute (core::FloatAttr or mlir::FloatAttr)
Primary key: uid

Properties:
- `uid`: int — unique attr id (required)
- `value`: float — float value (required)
- `attr_type`: int — 2 for core::FloatAttr, 3 for mlir::FloatAttr (required)

### STRINGATTRS
Description: string attribute (mlir::StringAttr)
Primary key: uid

Properties:
- `uid`: int — unique attr id (required)
- `value`: string — string value (required)
- `attr_type`: int — 4 for mlir::StringAttr (required)

### TYPES
Description: MLIR type (including composite types)
Primary key: uid

Properties:
- `uid`: int — unique type id (required)
- `type`: string — type name/identifier (required)
- `has_dl`: bool — data layout flag (optional)
- `bw`: int — bitwidth (optional)
- `align`: int — alignment (optional)

### REGIONS
Description: MLIR region
Primary key: uid

Properties:
- `uid`: int — unique region id (required)
- `regType`: bool — true for GraphRegion, false for SSACFG (required)

### BLOCKS
Description: MLIR block
Primary key: uid

Properties:
- `uid`: int — unique block id (required)

### VTABLES
Description: virtual table for a class
Primary key: uid

Properties:
- `uid`: int — unique vtable id (required)
- `vtable_name`: string — class name (required)
- `components_flat`: Array<string> — flattened rows, ints are decimal strings (required)
- `components_sizes`: Array<int> — sizes for each row (required)

### VTTS
Description: VTT (virtual table table)
Primary key: uid

Properties:
- `uid`: int — unique vtt id (required)
- `vtt_name`: string — class name (required)
- `components_flat`: Array<string> — flattened rows (4 items per row) (required)

## Relationship Definitions

### Operation-related edges
- `(:OPERATIONS)-[:ARGVALUE {idx}]->(:VALUES)`
- `(:OPERATIONS)-[:ARGATTR {idx}]->(:INTATTRS|:FLOATATTRS|:STRINGATTRS)`
- `(:OPERATIONS)-[:ARGBLOCK {idx}]->(:BLOCKS)`
- `(:OPERATIONS)-[:ARGTYPE {idx}]->(:TYPES)`
- `(:OPERATIONS)-[:RESVALUE {idx}]->(:VALUES)`
- `(:OPERATIONS)-[:CHILDREGION {idx}]->(:REGIONS)`
- `(:OPERATIONS)-[:FUNCTYPE]->(:TYPES)`
- `(:OPERATIONS)-[:VARTYPE]->(:TYPES)`
- `(:OPERATIONS)-[:ARGVTABLE]->(:VTABLES)`
- `(:OPERATIONS)-[:ARGVTT]->(:VTTS)`

### Region-related edges
- `(:REGIONS)-[:CHILDBLOCK {idx}]->(:BLOCKS)`

### Block-related edges
- `(:BLOCKS)-[:CHILDINST {idx}]->(:OPERATIONS)`
- `(:BLOCKS)-[:BLOCKARG {idx}]->(:VALUES)`

### Value-related edges
- `(:VALUES)-[:TYPE]->(:TYPES)`

### Type-related edges
- `(:TYPES)-[:ARG {idx}]->(:TYPES)`

### Attr-related edges
- `(:INTATTRS|:FLOATATTRS)-[:TYPE]->(:TYPES)`

## Additional Semantics

### OPERATIONS
- Global functions must have a `FUNCTYPE` edge to their function type.
- Global variables must have a `VARTYPE` edge to their variable type.
- Only Module ops have edges to `VTABLES` and `VTTS`.

### ATTRS
- ATTR nodes are unique across the database and should be deduplicated.
- For composite attrs, split into simple attrs; TypeAttr should use `ARGTYPE` to point at `TYPES`.
- `attr_type` values:
  - 0: core::IntegerAttr
  - 1: mlir::IntegerAttr
  - 2: core::FloatAttr
  - 3: mlir::FloatAttr
  - 4: mlir::StringAttr
- All non-string attrs have a `:TYPE` edge to their `TYPES` node.

### TYPES
- TYPE nodes are unique across the database and should be deduplicated via query-then-create.
- Composite types must connect to their component types with `:ARG` edges.

### REGIONS
- `regType=true` indicates GraphRegion; `regType=false` indicates SSACFG.

### VTABLES
- Only Module ops can point to VTABLES.
- `components_flat` stores rows flattened as strings; integers are decimal strings.
- `components_sizes` stores the size of each row.

### VTTS
- Only Module ops can point to VTTS.
- Each row has exactly 4 items; stored in `components_flat`.

## Constraints & Indexes (Recommended)

Indexes:
- `OPERATIONS.uid`
- `VALUES.uid`
- `INTATTRS.uid`
- `FLOATATTRS.uid`
- `STRINGATTRS.uid`
- `TYPES.uid`
- `REGIONS.uid`
- `BLOCKS.uid`
- `VTABLES.uid`
- `VTTS.uid`

## Example Queries

```cypher
// Find global functions by name within a file
MATCH (o:OPERATIONS {name: "hl.func", func_name: $func_name})
RETURN o.uid, o.func_name;
```

```cypher
// Find module ops by fileName
MATCH (o:OPERATIONS {name: "builtin.module", mod_name: $file_name})
RETURN o.uid, o.mod_name;
```

```cypher
// Get a global function's type
MATCH (o:OPERATIONS {name: "hl.func", func_name: $func_name})-[:FUNCTYPE]->(t:TYPES)
RETURN o.uid, o.func_name, t.uid, t.type
```

```cypher
// Get an operation's operand values in order
MATCH (o:OPERATIONS {uid: $op_uid})-[e:ARGVALUE]->(v:VALUES)
RETURN v.uid, e.idx
ORDER BY e.idx
```

```cypher
// Get an operation's result types in order
MATCH (o:OPERATIONS {uid: $op_uid})-[e:RESVALUE]->(v:VALUES)-[:TYPE]->(t:TYPES)
RETURN t.uid, t.type, e.idx
ORDER BY e.idx
```

```cypher
// Walk from an op to its region and blocks in order
MATCH (o:OPERATIONS {uid: $op_uid})-[re:CHILDREGION]->(r:REGIONS)-[be:CHILDBLOCK]->(b:BLOCKS)
RETURN r.uid, b.uid, e.idx
ORDER BY re.idx, be.idx
```

```cypher
// Get instructions in a block in order
MATCH (b:BLOCKS {uid: $block_uid})-[e:CHILDINST]->(o:OPERATIONS)
RETURN o.uid, o.name, e.idx
ORDER BY e.idx
```

```cypher
// Get VTABLEs for a module op
MATCH (m:OPERATIONS {uid: $module_uid})-[:ARGVTABLE]->(vt:VTABLES)
RETURN vt.uid, vt.vtable_name, vt.components_flat, vt.components_sizes
```

```cypher
// Get VTTs for a module op
MATCH (m:OPERATIONS {uid: $module_uid})-[:ARGVTT]->(vtt:VTTS)
RETURN vtt.uid, vtt.vtt_name, vtt.components_flat
```

```cypher
// Get all operations inside a operation
MATCH (op:OPERATIONS {uid: $op_uid})
-[:CHILDINST|:CHILDBLOCK|:CHILDREGION*]->(child :OPERATIONS)
RETURN child
ORDER BY child.uid
```
