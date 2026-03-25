# [clang] Assertion failure "Bitfield access unit is not clipped" with packed struct + zero-width bitfield (MinGW targets)

**Author:** Z3rox-dev
**URL:** https://github.com/llvm/llvm-project/issues/181505
**Status:** Open
**Labels:** clang:codegen, crash

## Body

## Summary

Clang crashes with an assertion failure in `CGRecordLayoutBuilder.cpp` when compiling
a packed struct that contains a non-bitfield member smaller than `int`, followed by a
bitfield, a zero-width bitfield (`unsigned : 0`), and another bitfield.

The crash happens at **all optimization levels** (-O0, -O1, -O2, -O3, -Os) and is
**specific to MinGW targets** (`*-w64-windows-gnu`). Linux and MSVC targets are not affected.

## Reproducer

```c
#pragma pack(1)
struct S {
    char f1;
    unsigned f3 : 1;
    unsigned : 0;
    unsigned f4 : 1;
};
struct S g;
```

**Compile with:** `clang -c -std=c99 reproducer.c`

## Assertion

```
Assertion failed: M.Offset >= Tail && "Bitfield access unit is not clipped",
file clang/lib/CodeGen/CGRecordLayoutBuilder.cpp, line 960
```

Stack trace points to `CGRecordLowering::checkBitfieldClipping()` called from
`CGRecordLowering::lower()` → `CodeGenTypes::ComputeRecordLayout()`.

## Environment

- **Clang version:** 23.0.0git (built from source, trunk)
- **Build config:** Release + assertions
- **GCC 15.2.0:** Compiles the same code without any issue

## Affected targets

| Target | Result |
|--------|--------|
| `x86_64-w64-windows-gnu` (MinGW 64-bit) | **CRASH** |
| `i686-w64-windows-gnu` (MinGW 32-bit) | **CRASH** |
| `x86_64-linux-gnu` | OK |
| `x86_64-pc-windows-msvc` | OK |

## Key ingredients for the crash

All of these are required simultaneously:

1. `#pragma pack(N)` where N < `alignof(int)` — `pack(1)` and `pack(2)` both crash
2. A **non-bitfield member** smaller than `int` (`char` or `short`; `int` does NOT crash)
3. A **zero-width bitfield** (`unsigned : 0`)
4. At least one bitfield **after** the zero-width bitfield

Removing any one of these ingredients prevents the crash.

## Variations tested

| Struct definition | Result |
|---|---|
| `pack(1), char + bf + :0 + bf` | **CRASH** |
| `pack(2), char + bf + :0 + bf` | **CRASH** |
| `pack(1), short + bf + :0 + bf` | **CRASH** |
| `pack(1), int + bf + :0 + bf` | OK |
| No pragma pack, `char + bf + :0 + bf` | OK |
| `pack(1), char + bf + bf` (no `:0`) | OK |
| `pack(1), char + bf + :0` (no field after `:0`) | OK |

## Analysis

The assertion fires in `checkBitfieldClipping()` which verifies that bitfield
access units computed by `accumulateBitfields()` do not overlap. The combination
of `#pragma pack` with a non-bitfield member smaller than the natural alignment
causes the access units to be computed with overlapping offsets, but only under
MinGW's bitfield layout rules (which differ from the Itanium/MSVC ABI).

## Related issue

[#148838](https://github.com/llvm/llvm-project/issues/148838) has the same assertion
but is triggered through LLDB with C++ `[[no_unique_address]]` — a different code path.

## How this was found

Found via [CSmith](https://embed.cs.utah.edu/csmith/) fuzzing (seed 1027636316).
Reduced from a 108KB generated file to the 8-line reproducer above.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Giovanni B. (Z3rox-dev)

<details>
## Summary

Clang crashes with an assertion failure in `CGRecordLayoutBuilder.cpp` when compiling
a packed struct that contains a non-bitfield member smaller than `int`, followed by a
bitfield, a zero-width bitfield (`unsigned : 0`), and another bitfield.

The crash happens at **all optimization levels** (-O0, -O1, -O2, -O3, -Os) and is
**specific to MinGW targets** (`*-w64-windows-gnu`). Linux and MSVC targets are not affected.

## Reproducer

```c
#pragma pack(1)
struct S {
    char f1;
    unsigned f3 : 1;
    unsigned : 0;
    unsigned f4 : 1;
};
struct S g;
```

**Compile with:** `clang -c -std=c99 reproducer.c`

## Assertion

```
Assertion failed: M.Offset &gt;= Tail &amp;&amp; "Bitfield access unit is not clipped",
file clang/lib/CodeGen/CGRecordLayoutBuilder.cpp, line 960
```

Stack trace points to `CGRecordLowering::checkBitfieldClipping()` called from
`CGRecordLowering::lower()` → `CodeGenTypes::ComputeRecordLayout()`.

## Environment

- **Clang version:** 23.0.0git (built from source, trunk)
- **Build config:** Release + assertions
- **GCC 15.2.0:** Compiles the same code without any issue

## Affected targets

| Target | Result |
|--------|--------|
| `x86_64-w64-windows-gnu` (MinGW 64-bit) | **CRASH** |
| `i686-w64-windows-gnu` (MinGW 32-bit) | **CRASH** |
| `x86_64-linux-gnu` | OK |
| `x86_64-pc-windows-msvc` | OK |

## Key ingredients for the crash

All of these are required simultaneously:

1. `#pragma pack(N)` where N &lt; `alignof(int)` — `pack(1)` and `pack(2)` both crash
2. A **non-bitfield member** smaller than `int` (`char` or `short`; `int` does NOT crash)
3. A **zero-width bitfield** (`unsigned : 0`)
4. At least one bitfield **after** the zero-width bitfield

Removing any one of these ingredients prevents the crash.

## Variations tested

| Struct definition | Result |
|---|---|
| `pack(1), char + bf + :0 + bf` | **CRASH** |
| `pack(2), char + bf + :0 + bf` | **CRASH** |
| `pack(1), short + bf + :0 + bf` | **CRASH** |
| `pack(1), int + bf + :0 + bf` | OK |
| No pragma pack, `char + bf + :0 + bf` | OK |
| `pack(1), char + bf + bf` (no `:0`) | OK |
| `pack(1), char + bf + :0` (no field after `:0`) | OK |

## Analysis

The assertion fires in `checkBitfieldClipping()` which verifies that bitfield
access units computed by `accumulateBitfields()` do not overlap. The combination
of `#pragma pack` with a non-bitfield member smaller than the natural alignment
causes the access units to be computed with overlapping offsets, but only under
MinGW's bitfield layout rules (which differ from the Itanium/MSVC ABI).

## Related issue

[#<!-- -->148838](https://github.com/llvm/llvm-project/issues/148838) has the same assertion
but is triggered through LLDB with C++ `[[no_unique_address]]` — a different code path.

## How this was found

Found via [CSmith](https://embed.cs.utah.edu/csmith/) fuzzing (seed 1027636316).
Reduced from a 108KB generated file to the 8-line reproducer above.

</details>


---

