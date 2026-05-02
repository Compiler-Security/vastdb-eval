# VAST OPERATIONS Catalog

This document enumerates all operation names defined in the VAST MLIR framework, organized by dialect.

---

## OPERATIONS Node Specification

**Description:** an MLIR operation node
**Primary key:** `uid`

### Standard Properties

| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `func_name` | string | No | Function name (for global functions like `hl.func` or `ll.func`) |
| `varName` | string | No | Global variable name for global variable declaration like `hl.var` |
| `mod_name` | string | No | File name (for `builtin.module`) |

### Standard Relations

| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {idx}` | VALUES | many | Operand values (indexed) |
| `ARGATTR {idx}` | INTATTRS/FLOATATTRS/STRINGATTRS | many | Operand attributes (indexed) |
| `ARGBLOCK {idx}` | BLOCKS | many | Operand blocks (indexed) |
| `ARGTYPE {idx}` | TYPES | many | Operand types (indexed) |
| `RESVALUE {idx}` | VALUES | many | Result values (indexed) |
| `CHILDREGION {idx}` | REGIONS | many | Child regions (indexed) |
| `FUNCTYPE` | TYPES | single | Function type (global functions only) |
| `VARTYPE` | TYPES | single | Variable type (global variables only) |
| `ARGVTABLE` | VTABLES | many | Virtual tables (Module ops only) |
| `ARGVTT` | VTTS | many | VTTs (Module ops only) |

---

## Document Format

For each operation, the entry follows this structure:

```markdown
### «Category Name»

#### «Operation Name» (`«name»`)

**Description:** «what this operation represents»

**C/C++ Analog:** «corresponding C/C++ construct if applicable»

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| (operation-specific properties, or "None" if only standard properties) |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| (operation-specific relations, or "None" if only standard relations) |

**Notes:** «any additional implementation notes»
```

---

## Table of Contents

- [`vast::hl` - High-Level Dialect](#vasthl---high-level-dialect)
  - [Declarations](#declarations)
  - [Reference and Access](#reference-and-access)
  - [Binary Operations](#binary-operations)
  - [Assignment Operations](#assignment-operations)
  - [Unary Operations](#unary-operations)
  - [Comparison and Logic](#comparison-and-logic)
  - [Control Flow](#control-flow)
  - [Function Calls](#function-calls)
  - [C++ Specific](#c-specific)
  - [Cast Operations](#cast-operations)
  - [Expressions](#expressions)
- [`vast::ll` - Low-Level Dialect](#vastll---low-level-dialect)
  - [Control Flow](#control-flow-1)
  - [Functions](#functions-1)
  - [Memory Operations](#memory-operations-1)
  - [Scopes](#scopes-1)
- [`vast::core` - Core Dialect](#vastcore---core-dialect)
  - [Logical Operations](#logical-operations-1)
  - [Control Flow](#control-flow-2)
  - [Other](#other-1)

---

## `vast::hl` - High-Level Dialect

The high-level dialect represents operations that map directly to C/C++ language constructs.

### Declarations

---

#### `hl.class`

**Description:** Class declaration. Represents a C++ class definition.

**C/C++ Analog:** `class Name { ... };`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `className` | string | Yes | Class name |
| `dynamic` | bool | Yes | Is virtual class |


**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Base classes region  |
| `CHILDREGION {1}` | REGIONS | single | Class body region |
| `ARGTYPE {0}` | TYPES | single | Self type (class type) |

**Notes:** Contains nested operations for fields.

---

#### `hl.enum`

**Description:** Enumeration declaration. Represents a C/C++ enum type definition.

**C/C++ Analog:** `enum Name { ... };`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `has_underlying_type` | bool | No | If enum decl has specified underlying type |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Enum body region |
| `ARGATTR {0}` | STRINGATTRS | single | Enum name |
| `ARGTYPE {1}` | TYPE | single | Underlying type of Enum if `has_underlying_type` is true |

**Notes:** Nested operations represent enum constants.

---

#### `hl.enum.const`

**Description:** Enumeration constant. Represents a named value within an enumeration.

**C/C++ Analog:** `enum { VALUE = 42 };`

**Properties:**

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Enum const initialize region |
| `ARGATTR {0}` | STRINGATTRS | single | Enum const Name |
| `ARGATTR {1}` | INTATTRS | signle | Enum const value |

**Notes:** Typically nested within `hl.enum`.

---

#### `hl.field`

**Description:** Structure or class field declaration. Represents a member variable within a struct or class.

**C/C++ Analog:** `struct S { int field; };`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `fieldName` | string | Yes | Field name |
| `bits` | int | No | Field bit width |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGTYPE {0}` | TYPES | single | Field type |

**Notes:** Used within RecordDeclOp (`hl.struct`/`hl.class`).

---

#### `hl.func`

**Description:** Function declaration. Represents a function definition, including its signature and body.

**C/C++ Analog:** `int foo(int x, double y) { ... }`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `func_name` | string | Yes | Function name |
| `linkage` | uint32 | No | Linkage specifier (see below) |
| `visibility` | string | No | Visibility specifier: `Public` (default), `Private`, `Nested` |
| `fs_raw_name` | string | Yes | Function name without parameter list and return type (part of function signature) |
| `fs_class_name` | string | Yes | Which class is this function belongs to |
| `fs_prototype` | string | Yes | Type of this function (in string) |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `FUNCTYPE` | TYPES | single | Function type |
| `CHILDREGION {0}` | REGIONS | single | Function body region |

**Notes:**
- Body contained in CHILDREGION
- Linkage values (from `GlobalLinkageKind`):
  - `0` - ExternalLinkage (default): No linkage info; can be referenced from other translation units; must have exactly one definition across all translation units
  - `1` - AvailableExternallyLinkage: Like ExternalLinkage, but the definition only exists in the defining translation unit and is not visible in others; mainly used for inline optimization
  - `2` - LinkOnceAnyLinkage: Any-linkage once; at most one definition of the same name is kept across all translation units; allows cross-translation-unit merging (GCC/Clang)
  - `3` - LinkOnceODRLinkage: ODR-satisfying once linkage; the same name must have exactly one definition across all translation units (MSVC)
  - `4` - WeakAnyLinkage: Any weak linkage; at most one definition of the same symbol is kept across all translation units; linker may choose any (GCC/Clang)
  - `5` - WeakODRLinkage: ODR-satisfying weak linkage; the same symbol must have exactly one definition (MSVC)
  - `6` - AppendingLinkage: Append linkage; only for arrays; used to concatenate global arrays from different translation units (C++ static initialization)
  - `7` - InternalLinkage: Internal linkage; only visible within the current translation unit; similar to C static functions
  - `8` - PrivateLinkage: Private linkage; completely invisible to the linker; used for compiler-internal temporary symbols
  - `9` - ExternalWeakLinkage: External weak linkage; uses strong definition if it exists, otherwise uses weak definition (see `__attribute__((weak))`)
  - `10` - CommonLinkage: Common linkage; similar to uninitialized external variables; multiple translation units may each provide a definition (C tentative definitions)
- Visibility values (from `Visibility`):
  - `Public`: The symbol is public and may be referenced anywhere internal or external to the visible references in the IR
  - `Private`: The symbol is private and may only be referenced by SymbolRefAttrs local to the operations within the current symbol table
  - `Nested`: The symbol is visible to the current IR, which may include operations in symbol tables above the one that owns the current symbol; allows referencing a symbol outside of its current symbol table, while retaining the ability to observe all uses
- Constructor/destructor detection:
  - If `fs_raw_name == fs_class_name`, this is a **constructor**
  - If `'~' + fs_raw_name == fs_class_name`, this is a **destructor**

---

#### `hl.globalref`

**Description:** Global reference. References a global variable or function defined elsewhere.

**C/C++ Analog:** `global_var = 1;`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `symName` | string | Yes | Referenced name |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `RESVALUE` | VALUES | single | A lvalue to referenced variable |

---

#### `hl.param`

**Description:** Parameter declaration. Allocates local variable for a function parameter value.

**C/C++ Analog:** `void f(int x) { }` — `x` is a parameter

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `paramName` | string | Yes | Parameter name |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Function parameter value |
| `RESVALUE {0}` | VALUES | single | A lvalue to newly allocated local variable |

**Notes:** `hl.param` represents allocating a stack address for function parameters and storing the parameter values in the newly allocated address. It is among the first few instructions of a function.

---

#### `hl.struct`

**Description:** Structure declaration. Represents a C struct definition.

**C/C++ Analog:** `struct S { int x; };`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `structName` | string | Yes | Struct name |
| `non_trivial_for_calls` | bool | Yes | Whether the struct has non-trivial copy/move constructor, destructor, or default constructor per C++ ABI; if true, the type cannot be passed directly in registers and must be passed by reference or on the stack |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Struct body region |
| `ARGTYPE {0}` | TYPES | single | Self type |

---

#### `hl.union`

**Description:** Union declaration. Represents a C union definition.

**C/C++ Analog:** `union S { int x; float y; };`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `unionName` | string | Yes | Union name |
| `non_trivial_for_calls` | bool | Yes | Whether the union has non-trivial copy/move constructor, destructor, or default constructor per C++ ABI; if true, the type cannot be passed directly in registers and must be passed by reference or on the stack |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Union body region |
| `ARGTYPE {0}` | TYPES | single | Self type |

---

#### `hl.var`

**Description:** Variable declaration. Represents a variable definition.

**C/C++ Analog:** `int x = 42;` or `static int counter;`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `varName` | string | Yes | Variable name |
| `has_init` | bool | Yes | True if variable declaration has init region |
| `variable_sizes` | uint64 | Yes | Variable dimension |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `RESVALUE {0}` | VALUES | single | Result value (a lvalue to the variable) |
| `CHILDREGION {0}` | REGIONS | single | Initialize region |
| `ARGVALUE {idx}` | VALUES | multiple | Variable dimensions' size |

**Notes:**
- To find **top-level (global) variables**, traverse from `builtin.module`:
  ```cypher
  MATCH (m:OPERATIONS {name: 'builtin.module'})
  MATCH (m)-[:CHILDREGION]->(r:REGIONS)-[:CHILDBLOCK]->(b:BLOCKS)-[:CHILDINST]->(o:OPERATIONS {name: 'hl.var'})
  RETURN count(DISTINCT o)
  ```
- Local/static variables are nested inside function regions and are not accessible via this path.

---

### Reference and Access

---

#### `hl.ref`

**Description:** Declaration reference. References a previously declared variable, function, or type.

**C/C++ Analog:** Using a variable name `x` to refer to its declaration

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Referenced declaration |
| `RESVALUE {0}` | VALUES | single | Result value |

**Notes:** Resolved to actual declaration during semantic analysis.

---

#### `hl.addressof`

**Description:** Address-of operator. Takes the address of a variable or function (`&x`).

**C/C++ Analog:** `&x`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Operand (variable or function) |
| `RESVALUE {0}` | VALUES | single | Result value (pointer to operand) |

**Notes:** Result type is pointer to operand type.

---

#### `hl.deref`

**Description:** Dereference operator. Accesses the value pointed to by a pointer (`*ptr`).

**C/C++ Analog:** `*ptr`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Pointer operand |
| `RESVALUE {0}` | VALUES | single | Result value (pointee) |

**Notes:** Result type is pointee type.

---

#### `hl.member`

**Description:** Field reference. Accesses a member field of a struct or class.

**C/C++ Analog:** `obj.field`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `index` | int | No | Index of field |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Base struct/class value |
| `RESVALUE {0}` | VALUES | single | Result value (lvalue) |
| `ARGATTR {0}` | STRINGATTRS | single | Field name |

**Notes:** Compile-time field access (non-virtual).

---

#### `hl.subscript`

**Description:** Array or pointer subscript operation. Accesses element at index (`arr[i]`).

**C/C++ Analog:** `arr[i]` or `ptr[i]`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Base array/pointer |
| `ARGVALUE {1}` | VALUES | single | Index value |
| `RESVALUE {0}` | VALUES | single | Result value (lvalue) |

**Notes:** Result type is element type.

---

### Binary Operations

---

#### `hl.addi`

**Description:** Integer addition. Binary `+` operator for integer types.

**C/C++ Analog:** `a + b` (integer)

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Left operand |
| `ARGVALUE {1}` | VALUES | single | Right operand |
| `RESVALUE {0}` | VALUES | single | Result value |

---

#### `hl.addf`

**Description:** Floating-point addition. Binary `+` operator for floating-point types.

**C/C++ Analog:** `a + b` (floating-point)

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Left operand |
| `ARGVALUE {1}` | VALUES | single | Right operand |
| `RESVALUE {0}` | VALUES | single | Result value |

---

#### Other Binary Operations

For the following binary operations, the structure is similar to `AddIOp`/`AddFOp`:

| Name Property | C/C++ Analog |
|--------------|--------------|
| `hl.subi` | `a - b` (integer) |
| `hl.subf` | `a - b` (float) |
| `hl.muli` | `a * b` (integer) |
| `hl.mulf` | `a * b` (float) |
| `hl.divi` | `a / b` (integer) |
| `hl.divs` | `a / b` (signed) |
| `hl.divu` | `a / b` (unsigned) |
| `hl.divf` | `a / b` (float) |
| `hl.remi` | `a % b` (integer) |
| `hl.rems` | `a % b` (signed) |
| `hl.remu` | `a % b` (unsigned) |
| `hl.remf` | `a % b` (float) |
| `hl.binshl` | `a << b` |
| `hl.binarshr` | `a >> b` (arithmetic) |
| `hl.binlshr` | `a >> b` (logical) |
| `hl.binand` | `a & b` |
| `hl.binor` | `a \| b` |
| `hl.binxor` | `a ^ b` |
| `hl.binland` | `a && b` |
| `hl.binlor` | `a \|\| b` |
| `hl.bincomma` | `a, b` |

**Standard Relations for Binary Operations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Left operand |
| `ARGVALUE {1}` | VALUES | single | Right operand |
| `RESVALUE {0}` | VALUES | single | Result value |

---

### Assignment Operations

---

#### `hl.assign`

**Description:** Simple assignment. Assigns right-hand value to left-hand variable (`=`).

**C/C++ Analog:** `x = y`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | LHS (destination) |
| `ARGVALUE {1}` | VALUES | single | RHS (source) |
| `RESVALUE {0}` | VALUES | single | Result value |

---

#### Compound Assignment Operations

For the following compound assignment operations, the structure is similar to `AssignOp`:

| Name Property | C/C++ Analog |
|--------------|--------------|
| `hl.assign.addi` | `x += y` (integer) |
| `hl.assign.fadd` | `x += y` (float) |
| `hl.assign.subi` | `x -= y` (integer) |
| `hl.assign.fsub` | `x -= y` (float) |
| `hl.assign.muli` | `x *= y` (integer) |
| `hl.assign.fmul` | `x *= y` (float) |
| `hl.assign.sdiv` | `x /= y` (signed) |
| `hl.assign.udiv` | `x /= y` (unsigned) |
| `hl.assign.fdiv` | `x /= y` (float) |
| `hl.assign.srem` | `x %= y` (signed) |
| `hl.assign.urem` | `x %= y` (unsigned) |
| `hl.assign.frem` | `x %= y` (float) |
| `hl.assign.bin.and` | `x &= y` |
| `hl.assign.bin.or` | `x \|= y` |
| `hl.assign.bin.xor` | `x ^= y` |
| `hl.assign.bin.shl` | `x <<= y` |
| `hl.assign.bin.lshr` | `x >>= y` (logical) |
| `hl.assign.bin.ashr` | `x >>= y` (arithmetic) |

**Standard Relations for Assignment Operations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | LHS (destination) |
| `ARGVALUE {1}` | VALUES | single | RHS (source) |
| `RESVALUE {0}` | VALUES | single | Result value |

---

### Unary Operations

---

#### `hl.minus`

**Description:** Unary minus. Negates a numeric value (`-x`).

**C/C++ Analog:** `-x`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Operand |
| `RESVALUE {0}` | VALUES | single | Result value |

---

#### Other Unary Operations

| Name Property | C/C++ Analog |
|--------------|--------------|
| `hl.plus` | `+x` |
| `hl.not` | `~x` (bitwise NOT) |
| `hl.lnot` | `!x` (logical NOT) |
| `hl.preinc` | `++x` |
| `hl.postinc` | `x++` |
| `hl.predec` | `--x` |
| `hl.postdec` | `x--` |

**Standard Relations for Unary Operations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Operand |
| `RESVALUE {0}` | VALUES | single | Result value |

---

### Comparison and Logic

---

#### `hl.cmp`

**Description:** Comparison operation. Generic comparison operator (`<`, `>`, `<=`, `>=`).

**C/C++ Analog:** `a < b`, `a > b`, `a <= b`, `a >= b`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `predicate` | string | Yes | Comparison predicate: `eq` (0), `ne` (1), `slt` (2), `sle` (3), `sgt` (4), `sge` (5), `ult` (6), `ule` (7), `ugt` (8), `uge` (9) |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Left operand |
| `ARGVALUE {1}` | VALUES | single | Right operand |
| `RESVALUE {0}` | VALUES | single | Result value (boolean) |
| `ARGATTR {0}` | INTATTRS | single | Predicate attribute |

---

#### `hl.fcmp`

**Description:** Floating-point comparison. Comparison for float types.

**C/C++ Analog:** `a < b`, `a > b`, etc. (floating-point)

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `predicate` | string | Yes | Comparison predicate: `ffalse` (0), `oeq` (1), `ogt` (2), `oge` (3), `olt` (4), `ole` (5), `one` (6), `ord` (7), `uno` (8), `ueq` (9), `ugt` (10), `uge` (11), `ult` (12), `ule` (13), `une` (14), `ftrue` (15) |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Left operand |
| `ARGVALUE {1}` | VALUES | single | Right operand |
| `RESVALUE {0}` | VALUES | single | Result value (boolean) |

---

#### `hl.cond`

**Description:** Binary conditional. C ternary conditional operator (`?:`, a.k.a. min/max).

**C/C++ Analog:** `cond ? a : b`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Condition region |
| `CHILDREGION {1}` | REGIONS | single | True region |
| `CHILDREGION {2}` | REGIONS | single | False region |
| `RESVALUE {0}` | VALUES | single | Result value |

**Notes:** Result type matches true/false value types.

---

### Control Flow

---

#### `hl.if`

**Description:** If statement. Conditional branch execution.

**C/C++ Analog:** `if (cond) { ... } else { ... }`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Condition region |
| `CHILDREGION {1}` | REGIONS | single | Then region |
| `CHILDREGION {2}` | REGIONS | single | Else region |

**Notes:** Result values from regions represent if-else result.

---

#### `hl.while`

**Description:** While loop. Pre-condition loop construct.

**C/C++ Analog:** `while (cond) { ... }`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Condition region |
| `CHILDREGION {1}` | REGIONS | single | Body region |

**Notes:** Condition region yields boolean; body region yields loop carried values.

---

#### `hl.for`

**Description:** For loop. Classical for-loop with init/condition/inc.

**C/C++ Analog:** `for (init; cond; inc) { body }`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Conditional region |
| `CHILDREGION {1}` | REGIONS | single | Increase region |
| `CHILDREGION {2}` | REGIONS | single | Loop body region |


**Notes:** Init operations are out of `hl.for` operation

---

#### `hl.return`

**Description:** Return statement. Returns from function with optional value.

**C/C++ Analog:** `return x;` or `return;`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Return value (if any) |

**Notes:** No result values (terminator).

---

#### `hl.value_yield`

**Description:** Value yield. Terminates a value region (e.g., `hl.var` init region).

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Yielded value |

**Notes:** Used at the end of value regions to yield the computed value.

---

#### `hl.cond_yield`

**Description:** Conditional yield. Terminates a condition region (e.g., `hl.if` condition region).

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Condition value |

**Notes:** Used at the end of condition regions to yield the boolean condition.

---

#### `hl.do`

**Description:** Do-while loop. Post-condition loop construct.

**C/C++ Analog:** `do { ... } while (cond)`

**Properties:**

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Condition region |
| `CHILDREGION {1}` | REGIONS | single | Body region |

---

#### `hl.switch`

**Description:** Switch statement. Multi-way branch selection.

**C/C++ Analog:** `switch (x) { ... }`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Switch condition |
| `CHILDREGION {1}` | REGIONS | single | Cases region |

---

#### `hl.case`

**Description:** Case statement. Single case in switch.

**C/C++ Analog:** `case n:`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Case value |
| `CHILDREGION {1}` | REGIONS | single | Case region |

**Notes:** Must be nested within `hl.switch`.

---

#### `hl.default`

**Description:** Default case. Default branch in switch.

**C/C++ Analog:** `default:`

**Properties:**

**Relations:**

**Notes:** Must be nested within `hl.switch`.

---

#### `hl.break`

**Description:** Break statement. Exits nearest loop or switch.

**C/C++ Analog:** `break;`

**Properties:**

**Relations:**

**Notes:** Terminator operation.

---

#### `hl.continue`

**Description:** Continue statement. Jumps to next iteration of loop.

**C/C++ Analog:** `continue;`

**Properties:**

**Relations:**

**Notes:** Terminator operation.

---

#### `hl.label`

**Description:** Label. Creates a labeled point in code that can be the target of `hl.goto`.

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `label_name` | string | Yes | Label name |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `RESVALUE {0}` | VALUES | single | Label value (used by `hl.goto`) |

**Notes:** The result value is used as the operand for `hl.goto` to reference this label.

---

#### `hl.goto`

**Description:** Goto statement. Unconditional jump to label.

**C/C++ Analog:** `goto label;`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Label value (return value of `hl.label`) |


**Notes:** Terminator operation.

---

#### `hl.skip`

**Description:** Skip statement. Empty statement (no-op).

**C/C++ Analog:** `;` (empty statement)

**Properties:**

**Relations:**

---

#### `hl.unreachable`

**Description:** Unreachable statement. Marks code as unreachable.

**C/C++ Analog:** `__builtin_unreachable()`

**Properties:**

**Relations:**

**Notes:** Terminator operation.

---

### Function Calls

---

#### `hl.call`

**Description:** Function call. Direct function invocation.

**C/C++ Analog:** `foo(arg1, arg2)`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `callee` | string | Yes | Name of function being called |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {idx}` | VALUES | many | Call arguments |
| `FUNCTYPE` | TYPES | single | Function type |

**Notes:** Result type matches function return type.

---

#### `hl.indirect_call`

**Description:** Indirect function call. Function pointer call.

**C/C++ Analog:** `(*func_ptr)(arg1, arg2)`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Function pointer |
| `ARGVALUE {idx+1}` | VALUES | many | Call arguments |

**Notes:** Callee is determined at runtime.

---

#### Other Function Call Operations

| Name Property | Description |
|--------------|-------------|
| `hl.call.virtual` | Virtual function call via vtable |
| `hl.funcref` | Reference to a function |
| `hl.method.ref` | Reference to a class method |
| `hl.virtual_method.ref` | Reference to a virtual method |

---

### C++ Specific

---

#### `hl.new`

**Description:** New operator. Allocates single object (`new T`).

**C/C++ Analog:** `new T` or `new T(args)`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `placement_args` | int | No | Number of placement arguments |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {idx+0}` | VALUES | many | Constructor arguments |
| `ARGATTR {0}` | STRINGATTRS | single | Constructor function name |
| `ARGTYPE {1}` | TYPES | single | New created type |
| `RESVALUE {0}` | VALUES | single | A ssa value pointed to created object |

**Notes:** Result type is pointer to allocated type.

---

#### `hl.delete`

**Description:** Delete operator. Deallocates single object (`delete ptr`).

**C/C++ Analog:** `delete ptr`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Pointer to delete |
| `ARGATTR {0}` | STRINGATTRS | single | Destructor function name |

**Notes:** Does not return a value.

---

#### Other C++ Specific Operations

| Name Property | C/C++ Analog |
|--------------|--------------|
| `hl.new.array` | `new T[n]` |
| `hl.delete.array` | `delete[] ptr` |
| `hl.delete.virtual` | Virtual destructor |
| `hl.call.virtual` | `obj.method()` |
| `hl.try` | `try { ... } catch { ... }` |
| `hl.catch` | `catch (e) { ... }` |
| `hl.throw` | `throw expr;` |
| `hl.rethrow` | `throw;` |
| `hl.VTT` | VTT for inheritance |
| `hl.vtable` | Virtual table |

---

### Cast Operations

#### `hl.cast`, `hl.cstyle_cast`, `builtin_bitcast`

**Description:** Cast operations

**C/C++ Analog:** `long `

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| castKind | int | Yes | Cast kind: `Dependent=0`, `BitCast=1`, `LValueBitCast=2`, `LValueToRValueBitCast=3`, **`LValueToRValue=4` (load instruction)**, `NoOp=5`, `BaseToDerived=6`, `DerivedToBase=7`, `UncheckedDerivedToBase=8`, `Dynamic=9`, `ToUnion=10`, `ArrayToPointerDecay=11`, `FunctionToPointerDecay=12`, `NullToPointer=13`, `NullToMemberPointer=14`, `BaseToDerivedMemberPointer=15`, `DerivedToBaseMemberPointer=16`, `MemberPointerToBoolean=17`, `ReinterpretMemberPointer=18`, `UserDefinedConversion=19`, `ConstructorConversion=20`, `IntegralToPointer=21`, `PointerToIntegral=22`, `PointerToBoolean=23`, `ToVoid=24`, `VectorSplat=25`, `IntegralCast=26`, `IntegralToBoolean=27`, `IntegralToFloating=28`, `FloatingToFixedPoint=29`, `FixedPointToFloating=30`, `FixedPointCast=31`, `FixedPointToIntegral=32`, `IntegralToFixedPoint=33`, `FixedPointToBoolean=34`, `FloatingToIntegral=35`, `FloatingToBoolean=36`, `BooleanToSignedIntegral=37`, `FloatingCast=38`, `CPointerToObjCPointerCast=39`, `BlockPointerToObjCPointerCast=40`, `AnyPointerToBlockPointerCast=41`, `ObjCObjectLValueCast=42`, `FloatingRealToComplex=43`, `FloatingComplexToReal=44`, `FloatingComplexToBoolean=45`, `FloatingComplexCast=46`, `FloatingComplexToIntegralComplex=47`, `IntegralRealToComplex=48`, `IntegralComplexToReal=49`, `IntegralComplexToBoolean=50`, `IntegralComplexCast=51`, `IntegralComplexToFloatingComplex=52`, `ARCProduceObject=53`, `ARCConsumeObject=54`, `ARCReclaimReturnedObject=55`, `ARCExtendBlockObject=56`, `AtomicToNonAtomic=57`, `NonAtomicToAtomic=58`, `CopyAndAutoreleaseBlockObject=59`, `BuiltinFnToFnPtr=60`, `ZeroToOCLOpaqueType=61`, `AddressSpaceConversion=62`, `IntToOCLSampler=63`, `MatrixCast=64` |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Value to cast |
| `RESVALUE {0}` | VALUES | single | Cast result |

---

### Expressions

---

#### `hl.const`

**Description:** Constant value. Represents literal constants.

**C/C++ Analog:** `42`, `3.14`, `"hello"`

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `val_kind` | uint32 | Yes | Value kind: `VOID=0`, `FALSE=1`, `TRUE=2`, `INT=3`, `FLOAT=4`, `STRING=5` |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGATTR {0}` | INTATTRS/FLOATATTRS/STRINGATTRS | single | Constant value |
| `RESVALUE {0}` | VALUES | single | SSA value representing the constant |

---

#### `hl.enumref`

**Description:** Enum constant reference. References a named value within an enumeration.

**C/C++ Analog:** `MyEnum::VALUE`

**Properties:**

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGATTR {0}` | STRINGATTRS | single | Enum constant name |
| `RESVALUE {0}` | VALUES | single | Result value |

**Notes:** Used to reference enum constants in expressions.

---

#### `hl.expr`

**Description:** Expression wrapper. Wraps a child expression with additional semantics.

**C/C++ Analog:** General expression wrapper

**Properties:**

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Wrapped expression |
| `RESVALUE {0}` | VALUES | single | Result value |

**Notes:** Provides a generic container for expressions.

---

#### `hl.initlist`

**Description:** Initializer list. Represents a braced initializer list.

**C/C++ Analog:** `{a, b, c}` or `Type{a, b, c}`

**Properties:**

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {idx}` | VALUES | many | Elements of initializer list |
| `RESVALUE {0}` | VALUES | single | Result value |

**Notes:** Used for aggregate initialization.

---

#### `hl.imag`

**Description:** Imaginary part. Extracts the imaginary part of a complex number.

**C/C++ Analog:** `imag(x)`

**Properties:**

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Complex value |
| `RESVALUE {0}` | VALUES | single | Result value |

**Notes:** Result type is the imaginary component type.

---

#### `hl.real`

**Description:** Real part. Extracts the real part of a complex number.

**C/C++ Analog:** `real(x)`

**Properties:**

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Complex value |
| `RESVALUE {0}` | VALUES | single | Result value |

**Notes:** Result type is the real component type.

---

#### `hl.compound_literal`

**Description:** Compound literal. C99 compound literal expression.

**C/C++ Analog:** `(type){ initializer }`

**Properties:**

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Initializer region |
| `RESVALUE {0}` | VALUES | single | Result value |

**Notes:** Creates an anonymous object with initializer.

---

## `vast::ll` - Low-Level Dialect

The low-level dialect represents operations that map to LLVM IR-like constructs.

### Control Flow

---

#### `ll.br`

**Description:** Unconditional branch. Transfers control to a target basic block.

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGBLOCK {0}` | BLOCKS | single | Target block |

**Notes:** Terminator operation (no result values).

---

#### `ll.cond_br`

**Description:** Conditional branch. Branches to one of two blocks based on condition.

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Condition |
| `ARGBLOCK {0}` | BLOCKS | single | True target block |
| `ARGBLOCK {1}` | BLOCKS | single | False target block |

**Notes:** Terminator operation.

---

### Functions

---

#### `ll.func`

**Description:** Function definition. Defines a function with body at low-level.

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `func_name` | string | Yes | Function name |
| `linkage` | uint32 | No | Linkage specifier (same values as `hl.func`) |
| `visibility` | string | No | Visibility specifier: `Public` (default), `Private`, `Nested` |
| `fs_raw_name` | string | Yes | Function name without parameter list and return type |
| `fs_class_name` | string | Yes | Which class this function belongs to |
| `fs_prototype` | string | Yes | Type of this function (in string) |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `FUNCTYPE` | TYPES | single | Function type |
| `CHILDREGION {0}` | REGIONS | single | Function body region |
| `ARGVALUE {idx}` | VALUES | many | Function arguments |

**Notes:**
- Body contained in CHILDREGION
- Linkage, visibility, and constructor/destructor detection are identical to `hl.func`
- Constructor/destructor detection: `fs_raw_name == fs_class_name` (constructor), `'~' + fs_raw_name == fs_class_name` (destructor)

---

### Memory Operations

---

#### `ll.alloca`

**Description:** Stack allocation. Allocates variable on stack.

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `varName` | string | Yes | Name of local variable |

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {idx}` | VALUES | many | Variable dimensions' size |
| `RESVALUE {0}` | VALUES | single | Result value (pointer to allocated type) |

**Notes:** Result is pointer to allocated type.

---

#### `ll.load`

**Description:** Load instruction. Reads from memory.

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Pointer to load from |
| `RESVALUE {0}` | VALUES | single | Result value (loaded value) |

**Notes:** Result type is the pointed-to type.

---

#### `ll.store`

**Description:** Store instruction. Writes to memory.

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARGVALUE {0}` | VALUES | single | Value to store |
| `ARGVALUE {1}` | VALUES | single | Destination pointer |
| `RESVALUE {0}` | VALUES | single | Result value (the stored value) |

**Notes:** Returns the stored value.

---

#### Other Memory Operations

| Name Property | Description |
|--------------|-------------|
| `ll.arg_alloca` | Argument allocation |
| `ll.struct_gep` | Struct field address |
| `ll.subscript` | Array/pointer subscript |
| `ll.uninitialized_var` | Uninitialized variable |
| `ll.initialize_var` | Variable initialization |

---

## `vast::core` - Core Dialect

Core operations that are fundamental to the VAST framework.

---

### Control Flow

---

#### `core.scope`

**Description:** Scope operation. Defines a lexical scope.

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `CHILDREGION {0}` | REGIONS | single | Scope body |

**Notes:** Similar to ll.Scope but at core level.

---

#### `core.implicit.return`

**Description:** Implicit return. Implicit return from `main()`.

**Properties:**
| Property | Type | Required | Description |
|----------|------|----------|-------------|

**Relations:**
| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|

**Notes:** Used when function doesn't have explicit return.

---

## Usage Notes

- Operation names are fully qualified with their dialect namespace (e.g., `hl.func`)
- When storing in Neo4j, use the `name` property (e.g., `hl.func`)
- All operations follow MLIR's operation interface conventions
- Index recommended: `OPERATIONS.uid`

---

## Relationship Cardinality Notation

- **single**: Exactly one relationship of this type
- **many**: Zero or more relationships of this type
- **{idx}**: Indexed relationship (order matters)

---

## Property Types

- **int**: Integer value
- **string**: Text value
- **bool**: Boolean value (`true`/`false`)
- **int (attr_type)**: Special integer for attribute type encoding
