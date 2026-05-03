# TYPES Catalog

This document enumerates all type definitions in the VAST MLIR framework, organized by dialect.

---

## TYPES Node Specification

**Description:** an MLIR type
**Primary key:** `uid`

### Properties

| Property | Type | Required | Description |
|----------|------|----------|-------------|
| `type` | string | Yes | Type mnemonic (e.g., "hl.int", "i32", "core.fn") |
| `has_dl` | bool | No | Whether type has data layout attributes |
| `bw` | int | No | Bitwidth (when has_dl is true) |
| `align` | int | No | Alignment (when has_dl is true) |
| `record_name` | string | No | Record name (for hl.record types only) |

### Subtypes

Types with children (pointer targets, function parameters, etc.) use indexed ARG relationships:

| Relationship | Target | Multiplicity | Description |
|-------------|--------|---------------|-------------|
| `ARG {idx}` | TYPES | many | Child types (e.g., pointed-to type, function params/results) |

---

## Document Format

For each type, the entry follows this structure:

```markdown
### «Mnemonic»

**Description:** «what this type represents»

**Dialect:** HighLevel | LowLevel | Core | MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true/false |
| bw | (if applicable) |
| align | (if applicable) |
| record_name | (if applicable) |

**Subtypes:** «relationship format or "None»»

**Notes:** «any additional information»
```

---

## Table of Contents

- [`hl` - High-Level Dialect](#hl---high-level-dialect)
  - [Void and Bool](#void-and-bool)
  - [Integer](#integer)
  - [Floating-Point](#floating-point)
  - [Pointer and Reference](#pointer-and-reference)
  - [Aggregate](#aggregate)
  - [Type Wrapper and Modifier](#type-wrapper-and-modifier)
  - [Other](#other)
- [`ll` - Low-Level Dialect](#ll---low-level-dialect)
- [`core` - Core Dialect](#core---core-dialect)
- [MLIR Built-in Types](#mlir-built-in-types)
  - [Integer](#integer-1)
  - [Floating-Point](#floating-point-1)
  - [Other](#other-1)
  - [MemRef](#memref)
  - [Vector](#vector)
  - [Complex](#complex)
  - [Tuple](#tuple)
  - [Function](#function)

---

## `hl` - High-Level Dialect

High-level types map directly to C/C++ language types.

---

### Void and Bool

---

#### `hl.void`

**Description:** Void type. Represents empty return type.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** None

**Notes:** Used for functions that return no value.

---

#### `hl.bool`

**Description:** Boolean type. Represents C++ `bool`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 8 |
| align | 1 |

**Subtypes:** None

**Notes:** Typically stored as 8 bits.

---

### Integer

---

#### `hl.char`

**Description:** Character type. Represents C++ `char`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 8 |
| align | 1 |

**Subtypes:** None

---

#### `hl.wchar`

**Description:** Wide character type. Represents C++ `wchar_t`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 16 or 32 (platform dependent) |
| align | 2 or 4 |

**Subtypes:** None

---

#### `hl.char16`

**Description:** 16-bit character type. Represents C++ `char16_t`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 16 |
| align | 2 |

**Subtypes:** None

---

#### `hl.char32`

**Description:** 32-bit character type. Represents C++ `char32_t`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 32 |
| align | 4 |

**Subtypes:** None

---

#### `hl.short`

**Description:** Short integer type. Represents C++ `short`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 16 |
| align | 2 |

**Subtypes:** None

---

#### `hl.int`

**Description:** Integer type. Represents C++ `int`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 32 |
| align | 4 |

**Subtypes:** None

---

#### `hl.long`

**Description:** Long integer type. Represents C++ `long`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 32 or 64 (platform dependent) |
| align | 4 or 8 |

**Subtypes:** None

---

#### `hl.longlong`

**Description:** Long long integer type. Represents C++ `long long`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 64 |
| align | 8 |

**Subtypes:** None

---

#### `hl.int128`

**Description:** 128-bit integer type. Represents C++ `__int128`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 128 |
| align | 16 |

**Subtypes:** None

---

#### `hl.bitint`

**Description:** Bit-precise integer type. Represents C++ `BitInt`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |

**Subtypes:** None

**Notes:** Width is specified dynamically.

---

#### `hl.atomic`

**Description:** Atomic type. Atomic integer type with specified width.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |

**Subtypes:** None

**Notes:** Used for atomic operations.

---

### Floating-Point

---

#### `hl.half`

**Description:** Half-precision floating-point. IEEE 754 binary16.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 16 |
| align | 2 |

**Subtypes:** None

---

#### `hl.bfloat16`

**Description:** Brain floating-point. Google BF16 format.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 16 |
| align | 2 |

**Subtypes:** None

---

#### `hl.float`

**Description:** Single-precision floating-point. IEEE 754 binary32.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 32 |
| align | 4 |

**Subtypes:** None

---

#### `hl.double`

**Description:** Double-precision floating-point. IEEE 754 binary64.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 64 |
| align | 8 |

**Subtypes:** None

---

#### `hl.longdouble`

**Description:** Long double floating-point. Platform dependent (x87 80-bit on x86).

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 80 (typical) |
| align | 16 (typical) |

**Subtypes:** None

---

#### `hl.float128`

**Description:** Quadruple-precision floating-point. IEEE 754 binary128.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 128 |
| align | 16 |

**Subtypes:** None

---

#### `hl.complex`

**Description:** Complex number type. Complex with element type.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Element type (integer or floating-point)

**Notes:** Example: `hl.complex<hl.float>` for `std::complex<float>`.

---

### Pointer and Reference

---

#### `hl.ptr`

**Description:** Pointer type. Represents C++ pointer (`T*`).

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Pointed-to type

**Notes:** Address space may be specified.

---

#### `hl.member_field`

**Description:** Member data pointer type. Pointer to data member (`int Class::*`).

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Class type, `ARG {1}` - Member type

---

#### `hl.member_method`

**Description:** Member function pointer type. Pointer to member function.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Class type, `ARG {1}` - Function type

---

#### `hl.lvalue`

**Description:** Lvalue reference type. Represents C++ lvalue reference (`T&`).

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Referenced type

---

#### `hl.rvalue`

**Description:** Rvalue reference type. Represents C++ rvalue reference (`T&&`).

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Referenced type

---

### Aggregate

---

#### `hl.array`

**Description:** Array type. Represents C++ array (`T[N]`).

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Element type

**Notes:** Size is specified separately via size parameter.

---

#### `hl.vector`

**Description:** Vector type. SIMD vector type.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | (element bitwidth * element count) |
| align | (element alignment) |

**Subtypes:** `ARG {0}` - Element type

---

#### `hl.record`

**Description:** Record type. Base type for struct/class/union/enum.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |
| record_name | Yes (required) |

**Subtypes:** None (record definition is separate)

**Notes:** Concrete subtypes are `hl.struct`, `hl.union`, `hl.class`, `hl.enum`. The `record_name` property identifies the specific record.

---

### Type Wrapper and Modifier

---

#### `hl.typedef`

**Description:** Typedef type. Named type alias.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Underlying type

---

#### `hl.elaborated`

**Description:** Elaborated type. Type with full qualification.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Underlying type

---

#### `hl.typeof.type`

**Description:** TypeOf type. `typeof(T)` type.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** None

---

#### `hl.typeof.expr`

**Description:** TypeOf expression type. `typeof(expr)` type.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** None

---

#### `hl.nullptr_t`

**Description:** Null pointer type. C++ `std::nullptr_t`.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 64 |
| align | 8 |

**Subtypes:** None

---

### Other

---

#### `hl.label`

**Description:** Label type. Basic block label type.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** None

---

#### `hl.paren`

**Description:** Paren type. Parenthesized type wrapper.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Wrapped type

---

#### `hl.decayed`

**Description:** Decayed type. Array to pointer decay result.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Decayed-to type

---

#### `hl.attributed`

**Description:** Attributed type. Type with attributes.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Underlying type

---

#### `hl.adjusted`

**Description:** Adjusted type. Type adjustment wrapper.

**Dialect:** HighLevel

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Original type, `ARG {1}` - Adjusted type

---

## `ll` - Low-Level Dialect

The low-level dialect uses MLIR built-in types primarily. Custom low-level types are rare.

---

## `core` - Core Dialect

---

#### `core.fn`

**Description:** Function type. Represents function signature.

**Dialect:** Core

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {idx}` - Parameter types (indexed), with last element as return type if non-variadic

**Notes:** Variadic functions indicated by last `ARG` being empty or ellipsis type.

**Examples:**
```mlir
!core.fn<!hl.bool ()>           // bool foo()
!core.fn<!hl.int (!hl.char)>    // int foo(char)
!core.fn<!i32 (!i32, ...)>      // int foo(int, ...)
```

---

## MLIR Built-in Types

MLIR built-in types use no dialect prefix.

---

### Integer

---

#### `i1`

**Description:** 1-bit integer.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 1 |
| align | 1 |

**Subtypes:** None

---

#### `i8`

**Description:** 8-bit integer.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 8 |
| align | 1 |

**Subtypes:** None

---

#### `i16`

**Description:** 16-bit integer.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 16 |
| align | 2 |

**Subtypes:** None

---

#### `i32`

**Description:** 32-bit integer.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 32 |
| align | 4 |

**Subtypes:** None

---

#### `i64`

**Description:** 64-bit integer.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 64 |
| align | 8 |

**Subtypes:** None

---

#### `i128`

**Description:** 128-bit integer.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 128 |
| align | 16 |

**Subtypes:** None

---

#### `i256`

**Description:** 256-bit integer.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 256 |
| align | 32 |

**Subtypes:** None

---

#### `si8`, `si16`, `si32`, `si64`

**Description:** Signed integer types (two's complement).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | (width matches digit count) |

**Subtypes:** None

---

#### `ui8`, `ui16`, `ui32`, `ui64`

**Description:** Unsigned integer types.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | (width matches digit count) |

**Subtypes:** None

---

### Floating-Point

---

#### `f8E4M3FN`

**Description:** 8-bit FP8 (E4M3 format with FNUZ).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 8 |
| align | 1 |

**Subtypes:** None

---

#### `f8E5M2`

**Description:** 8-bit FP8 (E5M2 format).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 8 |
| align | 1 |

**Subtypes:** None

---

#### `f16`

**Description:** 16-bit floating-point (IEEE 754 binary16).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 16 |
| align | 2 |

**Subtypes:** None

---

#### `bf16`

**Description:** Brain floating-point (BF16).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 16 |
| align | 2 |

**Subtypes:** None

---

#### `f32`

**Description:** 32-bit floating-point (IEEE 754 binary32).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 32 |
| align | 4 |

**Subtypes:** None

---

#### `f64`

**Description:** 64-bit floating-point (IEEE 754 binary64).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 64 |
| align | 8 |

**Subtypes:** None

---

#### `f80`

**Description:** 80-bit floating-point (x87 extended precision).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 80 |
| align | 16 |

**Subtypes:** None

---

#### `f128`

**Description:** 128-bit floating-point (IEEE 754 binary128).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | 128 |
| align | 16 |

**Subtypes:** None

---

### Other

---

#### `index`

**Description:** Index type. Platform-dependent integer for array indices.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | (platform dependent, typically 64) |
| align | (platform dependent) |

**Subtypes:** None

---

#### `none`

**Description:** None type. Unit type for operations returning no value.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** None

---

### MemRef

---

#### `memref`

**Description:** MemRef type. Memory reference with shape and layout.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | (element bitwidth) |
| align | (element alignment) |

**Subtypes:** `ARG {0}` - Element type, `ARG {idx+1}` - Dimension sizes (optional)

**Notes:** Layout and memory space optional.

**Examples:**
```mlir
!memref<f32>           // 0-d memref
!memref<4 x f32>       // 1-d memref of 4 floats
!memref<? x f32>       // 1-d memref with dynamic size
!memref<* x f32>       // Unbounded memref
!memref<4 x ? x f32>   // 2-d with one dynamic dimension
```

---

### Vector

---

#### `vector`

**Description:** Vector type. SIMD vector with fixed dimensions.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | (element bitwidth * total elements) |
| align | (element alignment) |

**Subtypes:** `ARG {0}` - Element type, `ARG {idx+1}` - Dimension sizes

**Examples:**
```mlir
vector<4 x f32>      // 4-element float vector
vector<2 x 3 x i32>  // 2x3 matrix of i32
```

---

### Complex

---

#### `complex`

**Description:** Complex number type.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | true |
| bw | (2 * element bitwidth) |
| align | (element alignment) |

**Subtypes:** `ARG {0}` - Element type (floating-point)

**Examples:**
```mlir
complex<f32>   // complex with two f32
complex<f64>   // complex with two f64
```

---

### Tuple

---

#### `tuple`

**Description:** Tuple type. Fixed-size collection of heterogeneous types.

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {idx}` - Element types (indexed)

**Examples:**
```mlir
tuple<i32, f32>           // pair
tuple<i8, i16, i32>       // triple
tuple<tuple<i32, i32>>    // nested
```

---

### Function

---

#### `function`

**Description:** Function type. Function signature (deprecated, use `core.fn`).

**Dialect:** MLIR

**Properties:**
| Property | Value |
|----------|-------|
| has_dl | false |

**Subtypes:** `ARG {0}` - Return type, `ARG {idx+1}` - Parameter types

**Notes:** Syntax: `() -> ()`, `(T1) -> T2`, `(T1, T2) -> T3`

---

## Usage Notes

- Type names are stored in the `type` property of TYPES nodes
- When storing in Neo4j, use the mnemonic string as the `type` value
- Index recommended: `TYPES.uid` and `TYPES.type`
- For parametric types, child types are stored via `ARG {idx}` relationships

---

## Type Width Reference

| Type Category | Typical Bitwidths |
|--------------|-------------------|
| Boolean | 8 |
| Character | 8, 16, 32 |
| Short | 16 |
| Integer | 32, 64 |
| Float | 16, 32, 64, 80, 128 |
| Pointer | 64 (typical) |
| Index | 64 (typical) |

---

(End of file - total 742 lines)
