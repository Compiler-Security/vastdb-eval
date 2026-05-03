---
name: understand-mlir-schema
description: Use to understand the VAST DB schema: what node labels exist (OPERATIONS, VALUES, TYPES, etc.), how nodes connect through relationships (RESVALUE, ARGVALUE, CHILDREGION, etc.), and what properties each node kind carries. Use this before writing Cypher queries against VAST DB. For query patterns, load write-cypher.
---

# Understand MLIR Schema

## Overview

This skill describes the VAST DB schema. Use it to check node labels, relationship types, and properties before writing or interpreting Cypher queries. Do not guess labels or relationships; look them up here.

## Core Schema

### Node Labels

- `OPERATIONS` — MLIR operations. Properties include `uid`, `name`, `func_name`, `fs_raw_name`, `varName`, `mod_name`, and `location`. The `location` field has the form `['F', file, line, column]`.
- `VALUES` — SSA values. Only property is `uid`.
- `INTATTRS` — Integer attributes. Properties: `uid`, `value` (int), `attr_type` (int).
- `FLOATATTRS` — Float attributes. Properties: `uid`, `value` (float), `attr_type` (int).
- `STRINGATTRS` — String attributes. Properties: `uid`, `value` (string), `attr_type` (int).
- `TYPES` — MLIR types. Properties: `uid`, `type` (type mnemonic), `has_dl` (bool), `bw` (int bitwidth), `align` (int), `record_name` (string, for hl.record only).
- `REGIONS` — MLIR regions. Properties: `uid`, `regType` (bool: true for GraphRegion, false for SSACFG).
- `BLOCKS` — MLIR blocks. Only property is `uid`.
- `VTABLES` — Virtual tables. Properties: `uid`, `vtable_name`, `components_flat`, `components_sizes`.
- `VTTS` — VTT (virtual table table). Properties: `uid`, `vtt_name`, `components_flat`.

### Key Relationships

Operation operands and results:
- `(:OPERATIONS)-[:ARGVALUE {idx}]->(:VALUES)` — operation consumes a value as operand
- `(:OPERATIONS)-[:RESVALUE {idx}]->(:VALUES)` — operation produces a value as result
- `(:OPERATIONS)-[:ARGATTR {idx}]->(:INTATTRS|FLOATATTRS|STRINGATTRS)` — operation has an attribute
- `(:OPERATIONS)-[:ARGBLOCK {idx}]->(:BLOCKS)` — operation references a block (e.g., branch targets)
- `(:OPERATIONS)-[:ARGTYPE {idx}]->(:TYPES)` — operation has a type operand

Containment (operation bodies):
- `(:OPERATIONS)-[:CHILDREGION {idx}]->(:REGIONS)` — operation contains a region
- `(:REGIONS)-[:CHILDBLOCK {idx}]->(:BLOCKS)` — region contains a block
- `(:BLOCKS)-[:CHILDINST {idx}]->(:OPERATIONS)` — block contains an instruction (operation)
- `(:BLOCKS)-[:BLOCKARG {idx}]->(:VALUES)` — block has a block argument

Type system:
- `(:VALUES)-[:TYPE]->(:TYPES)` — value has a type
- `(:TYPES)-[:ARG {idx}]->(:TYPES)` — composite type has component types
- `(:INTATTRS|:FLOATATTRS)-[:TYPE]->(:TYPES)` — non-string attribute has a type

Function and variable types:
- `(:OPERATIONS)-[:FUNCTYPE]->(:TYPES)` — global function's type signature
- `(:OPERATIONS)-[:VARTYPE]->(:TYPES)` — global variable's type

Virtual tables (only from `builtin.module` operations):
- `(:OPERATIONS)-[:ARGVTABLE]->(:VTABLES)` — module has a vtable
- `(:OPERATIONS)-[:ARGVTT]->(:VTTS)` — module has a VTT

## References

- `references/schema-mlir-graph.md` — canonical node labels, relationships, properties, constraints, and example queries.
- `references/operations-catalog.md` — every operation name (`hl.func`, `hl.call`, `ll.load`, etc.) with its specific properties and relationships.
- `references/types-catalog.md` — every type mnemonic (`hl.int`, `i32`, `core.fn`, etc.) with its properties and subtype structure.
