# [MC][X86] Clang crashes when assembling x86-64 AVX-512 `vfpclassps`/`vfpclasspd` with symbol as category immediate

**Author:** venkyqz
**URL:** https://github.com/llvm/llvm-project/issues/185364
**Status:** Open
**Labels:** backend:X86, llvm:mc, crash

## Body

## Summary

`llvm-mc` (debug build) crashes with an assertion failure when assembling AVX-512 floating-point classification instructions (`vfpclassps`, `vfpclasspd`, `vfpclassss`, `vfpclasssd`) with a symbolic expression instead of an integer literal for the 8-bit category-mask immediate. The release build silently succeeds.

---

## Reproduction

**Godbolt Link**
+ https://godbolt.org/z/oG85r9K7T

**Test File (`poc.s`):**

```asm
.text
vfpclassps $f0, %zmm1, %k1
```

**Commands:**

```bash
# Debug Build - Crashes with assertion
echo 'vfpclassps $f0, %zmm1, %k1' | llvm-mc - \
  --triple=x86_64-unknown-linux-gnu -filetype=asm
# Exit 134, Assertion failed

# Release Build - Silent miscompilation with wrong comment
echo 'vfpclassps $f0, %zmm1, %k1' | llvm-mc - \
  --triple=x86_64-unknown-linux-gnu -filetype=asm
# Exit 0, wrong comment based on garbage data
```

**Debug Build Output:**

```
llvm-mc: MCInst.h:85: int64_t llvm::MCOperand::getImm() const:
Assertion `isImm() && "This is not an immediate"' failed.

Stack dump:
  #9 llvm::MCOperand::getImm() const MCInst.h:85
  #10 printFPCLASSComments(...) X86InstComments.cpp:1034
  #11 llvm::EmitAnyX86InstComments(...) X86InstComments.cpp:1083
  #12 X86ATTInstPrinter::printInst(...) X86ATTInstPrinter.cpp:63
```

---

## Root Cause

In AT&T syntax, `$f0` (or any `$symbol`) is accepted in the immediate position. For `vfpclassps`, the 8-bit category-mask immediate receives an `MCExpr` operand.

The instruction printer calls `printFPCLASSComments` to generate a human-readable comment. At line 1034:

```cpp
// X86InstComments.cpp:1034
uint8_t Categories = MI->getOperand(NumOperands - 1).getImm();
```

There is **no `isImm()` guard** before this call. In debug builds, `MCOperand::getImm()` asserts and aborts. In release builds, the pointer to the MCExpr is reinterpreted as `int64_t`, cast to `uint8_t`, and used as a category bitmask — producing a spurious but plausible-looking comment.

---

## Proposed Fix

Add an early-return guard in `printFPCLASSComments` before accessing the immediate:

```cpp
// X86InstComments.cpp, before line 1034
if (!MI->getOperand(NumOperands - 1).isImm())
  return false;

uint8_t Categories = MI->getOperand(NumOperands - 1).getImm();
```

---

## Impact

| Build Type | Behavior | Security Risk |
|------------|----------|---------------|
| Debug | Assertion failure (Exit 134) | Detectable |
| Release | Silent miscompilation (Exit 0) | **Undetectable in CI/CD** |

**Affected Instructions:**

| Instruction | Debug | Release |
|-------------|-------|---------|
| `vfpclassps $f0, %xmm1, %k1` | 134 | 0 |
| `vfpclassps $f0, %ymm1, %k1` | 134 | 0 |
| `vfpclassps $f0, %zmm1, %k1` | 134 | 0 |
| `vfpclasspd $f0, %xmm1, %k1` | 134 | 0 |
| `vfpclasspd $f0, %ymm1, %k1` | 134 | 0 |
| `vfpclasspd $f0, %zmm1, %k1` | 134 | 0 |
| `vfpclassss $f0, %xmm1, %k1` | 134 | 0 |
| `vfpclasssd $f0, %xmm1, %k1` | 134 | 0 |


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Qi Zhang (venkyqz)

<details>
## Summary

`llvm-mc` (debug build) crashes with an assertion failure when assembling AVX-512 floating-point classification instructions (`vfpclassps`, `vfpclasspd`, `vfpclassss`, `vfpclasssd`) with a symbolic expression instead of an integer literal for the 8-bit category-mask immediate. The release build silently succeeds.

---

## Reproduction

**Godbolt Link**
+ https://godbolt.org/z/oG85r9K7T

**Test File (`poc.s`):**

```asm
.text
vfpclassps $f0, %zmm1, %k1
```

**Commands:**

```bash
# Debug Build - Crashes with assertion
echo 'vfpclassps $f0, %zmm1, %k1' | llvm-mc - \
  --triple=x86_64-unknown-linux-gnu -filetype=asm
# Exit 134, Assertion failed

# Release Build - Silent miscompilation with wrong comment
echo 'vfpclassps $f0, %zmm1, %k1' | llvm-mc - \
  --triple=x86_64-unknown-linux-gnu -filetype=asm
# Exit 0, wrong comment based on garbage data
```

**Debug Build Output:**

```
llvm-mc: MCInst.h:85: int64_t llvm::MCOperand::getImm() const:
Assertion `isImm() &amp;&amp; "This is not an immediate"' failed.

Stack dump:
  #<!-- -->9 llvm::MCOperand::getImm() const MCInst.h:85
  #<!-- -->10 printFPCLASSComments(...) X86InstComments.cpp:1034
  #<!-- -->11 llvm::EmitAnyX86InstComments(...) X86InstComments.cpp:1083
  #<!-- -->12 X86ATTInstPrinter::printInst(...) X86ATTInstPrinter.cpp:63
```

---

## Root Cause

In AT&amp;T syntax, `$f0` (or any `$symbol`) is accepted in the immediate position. For `vfpclassps`, the 8-bit category-mask immediate receives an `MCExpr` operand.

The instruction printer calls `printFPCLASSComments` to generate a human-readable comment. At line 1034:

```cpp
// X86InstComments.cpp:1034
uint8_t Categories = MI-&gt;getOperand(NumOperands - 1).getImm();
```

There is **no `isImm()` guard** before this call. In debug builds, `MCOperand::getImm()` asserts and aborts. In release builds, the pointer to the MCExpr is reinterpreted as `int64_t`, cast to `uint8_t`, and used as a category bitmask — producing a spurious but plausible-looking comment.

---

## Proposed Fix

Add an early-return guard in `printFPCLASSComments` before accessing the immediate:

```cpp
// X86InstComments.cpp, before line 1034
if (!MI-&gt;getOperand(NumOperands - 1).isImm())
  return false;

uint8_t Categories = MI-&gt;getOperand(NumOperands - 1).getImm();
```

---

## Impact

| Build Type | Behavior | Security Risk |
|------------|----------|---------------|
| Debug | Assertion failure (Exit 134) | Detectable |
| Release | Silent miscompilation (Exit 0) | **Undetectable in CI/CD** |

**Affected Instructions:**

| Instruction | Debug | Release |
|-------------|-------|---------|
| `vfpclassps $f0, %xmm1, %k1` | 134 | 0 |
| `vfpclassps $f0, %ymm1, %k1` | 134 | 0 |
| `vfpclassps $f0, %zmm1, %k1` | 134 | 0 |
| `vfpclasspd $f0, %xmm1, %k1` | 134 | 0 |
| `vfpclasspd $f0, %ymm1, %k1` | 134 | 0 |
| `vfpclasspd $f0, %zmm1, %k1` | 134 | 0 |
| `vfpclassss $f0, %xmm1, %k1` | 134 | 0 |
| `vfpclasssd $f0, %xmm1, %k1` | 134 | 0 |

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: Qi Zhang (venkyqz)

<details>
## Summary

`llvm-mc` (debug build) crashes with an assertion failure when assembling AVX-512 floating-point classification instructions (`vfpclassps`, `vfpclasspd`, `vfpclassss`, `vfpclasssd`) with a symbolic expression instead of an integer literal for the 8-bit category-mask immediate. The release build silently succeeds.

---

## Reproduction

**Godbolt Link**
+ https://godbolt.org/z/oG85r9K7T

**Test File (`poc.s`):**

```asm
.text
vfpclassps $f0, %zmm1, %k1
```

**Commands:**

```bash
# Debug Build - Crashes with assertion
echo 'vfpclassps $f0, %zmm1, %k1' | llvm-mc - \
  --triple=x86_64-unknown-linux-gnu -filetype=asm
# Exit 134, Assertion failed

# Release Build - Silent miscompilation with wrong comment
echo 'vfpclassps $f0, %zmm1, %k1' | llvm-mc - \
  --triple=x86_64-unknown-linux-gnu -filetype=asm
# Exit 0, wrong comment based on garbage data
```

**Debug Build Output:**

```
llvm-mc: MCInst.h:85: int64_t llvm::MCOperand::getImm() const:
Assertion `isImm() &amp;&amp; "This is not an immediate"' failed.

Stack dump:
  #<!-- -->9 llvm::MCOperand::getImm() const MCInst.h:85
  #<!-- -->10 printFPCLASSComments(...) X86InstComments.cpp:1034
  #<!-- -->11 llvm::EmitAnyX86InstComments(...) X86InstComments.cpp:1083
  #<!-- -->12 X86ATTInstPrinter::printInst(...) X86ATTInstPrinter.cpp:63
```

---

## Root Cause

In AT&amp;T syntax, `$f0` (or any `$symbol`) is accepted in the immediate position. For `vfpclassps`, the 8-bit category-mask immediate receives an `MCExpr` operand.

The instruction printer calls `printFPCLASSComments` to generate a human-readable comment. At line 1034:

```cpp
// X86InstComments.cpp:1034
uint8_t Categories = MI-&gt;getOperand(NumOperands - 1).getImm();
```

There is **no `isImm()` guard** before this call. In debug builds, `MCOperand::getImm()` asserts and aborts. In release builds, the pointer to the MCExpr is reinterpreted as `int64_t`, cast to `uint8_t`, and used as a category bitmask — producing a spurious but plausible-looking comment.

---

## Proposed Fix

Add an early-return guard in `printFPCLASSComments` before accessing the immediate:

```cpp
// X86InstComments.cpp, before line 1034
if (!MI-&gt;getOperand(NumOperands - 1).isImm())
  return false;

uint8_t Categories = MI-&gt;getOperand(NumOperands - 1).getImm();
```

---

## Impact

| Build Type | Behavior | Security Risk |
|------------|----------|---------------|
| Debug | Assertion failure (Exit 134) | Detectable |
| Release | Silent miscompilation (Exit 0) | **Undetectable in CI/CD** |

**Affected Instructions:**

| Instruction | Debug | Release |
|-------------|-------|---------|
| `vfpclassps $f0, %xmm1, %k1` | 134 | 0 |
| `vfpclassps $f0, %ymm1, %k1` | 134 | 0 |
| `vfpclassps $f0, %zmm1, %k1` | 134 | 0 |
| `vfpclasspd $f0, %xmm1, %k1` | 134 | 0 |
| `vfpclasspd $f0, %ymm1, %k1` | 134 | 0 |
| `vfpclasspd $f0, %zmm1, %k1` | 134 | 0 |
| `vfpclassss $f0, %xmm1, %k1` | 134 | 0 |
| `vfpclasssd $f0, %xmm1, %k1` | 134 | 0 |

</details>


---

