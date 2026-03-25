# [MC][X86] Clang crashes when assembling x86-64 AVX-512 `vfpclassbf16` with Intel-syntax OFFSET symbol

**Author:** venkyqz
**URL:** https://github.com/llvm/llvm-project/issues/185365
**Status:** Open
**Labels:** backend:X86, llvm:mc, crash

## Body

## Summary

`llvm-mc` (debug build) crashes with an assertion failure when assembling AVX10.2 BF16 floating-point classification instruction `vfpclassbf16` with Intel-syntax `OFFSET sym` as the category-mask immediate. The release build silently accepts the input.

---

## Reproduction

**Godbolt Link**
+ https://godbolt.org/z/ToY1Mh9vx

**Test File (`poc.s`):**

```asm
.intel_syntax noprefix
vfpclassbf16 k0, xmm0, OFFSET sym
```

**Commands:**

```bash
# Debug Build - Crashes with assertion
cat > /tmp/poc.s << 'EOF'
.intel_syntax noprefix
vfpclassbf16 k0, xmm0, OFFSET sym
EOF
llvm-mc - --triple=x86_64-unknown-linux-gnu \
  -x86-asm-syntax=intel \
  -mattr=+avx512f,+avx512dq,+avx512vl,+avx10.2,+avx10.2-512 -filetype=asm \
  /tmp/poc.s
# Exit 134, Assertion failed

# Release Build - Silent acceptance
llvm-mc - --triple=x86_64-unknown-linux-gnu \
  -x86-asm-syntax=intel \
  -mattr=+avx512f,+avx512dq,+avx512vl,+avx10.2,+avx10.2-512 -filetype=asm \
  /tmp/poc.s
# Exit 0
```

**Debug Build Output:**

```
Assertion `isImm() && "This is not an immediate"' failed.
UNREACHABLE executed at .../llvm/include/llvm/MC/MCInst.h:85
```

---

## Root Cause

Intel syntax `OFFSET sym` creates an `MCSymbolRefExpr` that bypasses parser immediate-range validation. The MCOperand is typed as `Expr` (not `Imm`), so `isImm()` returns false.

The crash occurs at `X86InstComments.cpp:1034`:

```cpp
CASE_FPCLASS_PACKED(FPCLASSBF16, r)
CASE_FPCLASS_PACKED(FPCLASSPH, r)
CASE_FPCLASS_PACKED(FPCLASSPS, r)
... {
  // NOT guarded by isImm() check!
  uint8_t Categories = MI->getOperand(NumOperands - 1).getImm(); // line 1034
```

**Why some variants don't crash:** XMM/YMM masked variants (`VFPCLASSBF16Z128rik`, `VFPCLASSBF16Z256rik`) are not in the switch and fall through to `default: return false`, never calling `getImm()`.

---

## Impact

| Build Type | Behavior | Security Risk |
|------------|----------|---------------|
| Debug | Assertion failure (Exit 134) | Detectable |
| Release | Silent acceptance (Exit 0) | **Undetectable in CI/CD** |

**Affected Instructions (debug=134, release=0):**

| Instruction | Opcode |
|-------------|--------|
| `vfpclassbf16 k0, xmm0, OFFSET sym` | VFPCLASSBF16Z128ri |
| `vfpclassbf16 k0, ymm0, OFFSET sym` | VFPCLASSBF16Z256ri |
| `vfpclassbf16 k0, zmm0, OFFSET sym` | VFPCLASSBF16Zri |
| `vfpclassbf16 k0 {k1}, zmm0, OFFSET sym` | VFPCLASSBF16Zrik |

**Not Affected (db=0, rl=0):**
- `vfpclassbf16 k0 {k1}, xmm0, OFFSET sym` (VFPCLASSBF16Z128rik)
- `vfpclassbf16 k0 {k1}, ymm0, OFFSET sym` (VFPCLASSBF16Z256rik)

**CPU Features Required:** `+avx512f,+avx512dq,+avx512vl,+avx10.2,+avx10.2-512`


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: Qi Zhang (venkyqz)

<details>
## Summary

`llvm-mc` (debug build) crashes with an assertion failure when assembling AVX10.2 BF16 floating-point classification instruction `vfpclassbf16` with Intel-syntax `OFFSET sym` as the category-mask immediate. The release build silently accepts the input.

---

## Reproduction

**Godbolt Link**
+ https://godbolt.org/z/ToY1Mh9vx

**Test File (`poc.s`):**

```asm
.intel_syntax noprefix
vfpclassbf16 k0, xmm0, OFFSET sym
```

**Commands:**

```bash
# Debug Build - Crashes with assertion
cat &gt; /tmp/poc.s &lt;&lt; 'EOF'
.intel_syntax noprefix
vfpclassbf16 k0, xmm0, OFFSET sym
EOF
llvm-mc - --triple=x86_64-unknown-linux-gnu \
  -x86-asm-syntax=intel \
  -mattr=+avx512f,+avx512dq,+avx512vl,+avx10.2,+avx10.2-512 -filetype=asm \
  /tmp/poc.s
# Exit 134, Assertion failed

# Release Build - Silent acceptance
llvm-mc - --triple=x86_64-unknown-linux-gnu \
  -x86-asm-syntax=intel \
  -mattr=+avx512f,+avx512dq,+avx512vl,+avx10.2,+avx10.2-512 -filetype=asm \
  /tmp/poc.s
# Exit 0
```

**Debug Build Output:**

```
Assertion `isImm() &amp;&amp; "This is not an immediate"' failed.
UNREACHABLE executed at .../llvm/include/llvm/MC/MCInst.h:85
```

---

## Root Cause

Intel syntax `OFFSET sym` creates an `MCSymbolRefExpr` that bypasses parser immediate-range validation. The MCOperand is typed as `Expr` (not `Imm`), so `isImm()` returns false.

The crash occurs at `X86InstComments.cpp:1034`:

```cpp
CASE_FPCLASS_PACKED(FPCLASSBF16, r)
CASE_FPCLASS_PACKED(FPCLASSPH, r)
CASE_FPCLASS_PACKED(FPCLASSPS, r)
... {
  // NOT guarded by isImm() check!
  uint8_t Categories = MI-&gt;getOperand(NumOperands - 1).getImm(); // line 1034
```

**Why some variants don't crash:** XMM/YMM masked variants (`VFPCLASSBF16Z128rik`, `VFPCLASSBF16Z256rik`) are not in the switch and fall through to `default: return false`, never calling `getImm()`.

---

## Impact

| Build Type | Behavior | Security Risk |
|------------|----------|---------------|
| Debug | Assertion failure (Exit 134) | Detectable |
| Release | Silent acceptance (Exit 0) | **Undetectable in CI/CD** |

**Affected Instructions (debug=134, release=0):**

| Instruction | Opcode |
|-------------|--------|
| `vfpclassbf16 k0, xmm0, OFFSET sym` | VFPCLASSBF16Z128ri |
| `vfpclassbf16 k0, ymm0, OFFSET sym` | VFPCLASSBF16Z256ri |
| `vfpclassbf16 k0, zmm0, OFFSET sym` | VFPCLASSBF16Zri |
| `vfpclassbf16 k0 {k1}, zmm0, OFFSET sym` | VFPCLASSBF16Zrik |

**Not Affected (db=0, rl=0):**
- `vfpclassbf16 k0 {k1}, xmm0, OFFSET sym` (VFPCLASSBF16Z128rik)
- `vfpclassbf16 k0 {k1}, ymm0, OFFSET sym` (VFPCLASSBF16Z256rik)

**CPU Features Required:** `+avx512f,+avx512dq,+avx512vl,+avx10.2,+avx10.2-512`

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Qi Zhang (venkyqz)

<details>
## Summary

`llvm-mc` (debug build) crashes with an assertion failure when assembling AVX10.2 BF16 floating-point classification instruction `vfpclassbf16` with Intel-syntax `OFFSET sym` as the category-mask immediate. The release build silently accepts the input.

---

## Reproduction

**Godbolt Link**
+ https://godbolt.org/z/ToY1Mh9vx

**Test File (`poc.s`):**

```asm
.intel_syntax noprefix
vfpclassbf16 k0, xmm0, OFFSET sym
```

**Commands:**

```bash
# Debug Build - Crashes with assertion
cat &gt; /tmp/poc.s &lt;&lt; 'EOF'
.intel_syntax noprefix
vfpclassbf16 k0, xmm0, OFFSET sym
EOF
llvm-mc - --triple=x86_64-unknown-linux-gnu \
  -x86-asm-syntax=intel \
  -mattr=+avx512f,+avx512dq,+avx512vl,+avx10.2,+avx10.2-512 -filetype=asm \
  /tmp/poc.s
# Exit 134, Assertion failed

# Release Build - Silent acceptance
llvm-mc - --triple=x86_64-unknown-linux-gnu \
  -x86-asm-syntax=intel \
  -mattr=+avx512f,+avx512dq,+avx512vl,+avx10.2,+avx10.2-512 -filetype=asm \
  /tmp/poc.s
# Exit 0
```

**Debug Build Output:**

```
Assertion `isImm() &amp;&amp; "This is not an immediate"' failed.
UNREACHABLE executed at .../llvm/include/llvm/MC/MCInst.h:85
```

---

## Root Cause

Intel syntax `OFFSET sym` creates an `MCSymbolRefExpr` that bypasses parser immediate-range validation. The MCOperand is typed as `Expr` (not `Imm`), so `isImm()` returns false.

The crash occurs at `X86InstComments.cpp:1034`:

```cpp
CASE_FPCLASS_PACKED(FPCLASSBF16, r)
CASE_FPCLASS_PACKED(FPCLASSPH, r)
CASE_FPCLASS_PACKED(FPCLASSPS, r)
... {
  // NOT guarded by isImm() check!
  uint8_t Categories = MI-&gt;getOperand(NumOperands - 1).getImm(); // line 1034
```

**Why some variants don't crash:** XMM/YMM masked variants (`VFPCLASSBF16Z128rik`, `VFPCLASSBF16Z256rik`) are not in the switch and fall through to `default: return false`, never calling `getImm()`.

---

## Impact

| Build Type | Behavior | Security Risk |
|------------|----------|---------------|
| Debug | Assertion failure (Exit 134) | Detectable |
| Release | Silent acceptance (Exit 0) | **Undetectable in CI/CD** |

**Affected Instructions (debug=134, release=0):**

| Instruction | Opcode |
|-------------|--------|
| `vfpclassbf16 k0, xmm0, OFFSET sym` | VFPCLASSBF16Z128ri |
| `vfpclassbf16 k0, ymm0, OFFSET sym` | VFPCLASSBF16Z256ri |
| `vfpclassbf16 k0, zmm0, OFFSET sym` | VFPCLASSBF16Zri |
| `vfpclassbf16 k0 {k1}, zmm0, OFFSET sym` | VFPCLASSBF16Zrik |

**Not Affected (db=0, rl=0):**
- `vfpclassbf16 k0 {k1}, xmm0, OFFSET sym` (VFPCLASSBF16Z128rik)
- `vfpclassbf16 k0 {k1}, ymm0, OFFSET sym` (VFPCLASSBF16Z256rik)

**CPU Features Required:** `+avx512f,+avx512dq,+avx512vl,+avx10.2,+avx10.2-512`

</details>


---

