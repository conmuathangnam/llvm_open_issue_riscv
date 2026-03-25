# [MC][MIPS] Clang crashes when assembling MIPS `div`/`rem` with register-name symbol as divisor

**Author:** venkyqz
**URL:** https://github.com/llvm/llvm-project/issues/185363
**Status:** Open
**Labels:** backend:MIPS, llvm:mc, crash

## Body

## Summary

`llvm-mc` crashes when assembling MIPS `div`/`divu`/`rem`/`remu` (MIPS32) and `ddiv`/`ddivu`/`drem`/`dremu` (MIPS64) pseudo-instructions with a MIPS register-class name (e.g., `f0`, `t0`, `lo`) as the divisor operand. **Both debug and release builds crash** — debug crashes with SIGABRT (assertion failure), release crashes with SIGSEGV (null/invalid pointer dereference).


---

## Reproduction

**Godbolt Link**
+ https://godbolt.org/z/n154hjoqc

**Test File (`poc.s`):**

```asm
.text
div $t0, $t1, f0
```

**Debug Build Output:**

```
llvm-mc: /path/to/llvm/lib/Target/Mips/AsmParser/MipsAsmParser.cpp:4204:
Assertion `(RtOp.isReg() || RtOp.isImm()) && "expected register or immediate operand kind"' failed.
```

---

## Root Cause

`div`/`rem` pseudo-instructions use `simm32_relaxed` as the divisor operand type. MIPS register-class names are parsed as symbol references (MCExpr), which is neither `isReg()` nor `isImm()`.

```cpp
// MipsAsmParser.cpp:4204
assert((RtOp.isReg() || RtOp.isImm()) &&
       "expected register or immediate operand kind");
if (RtOp.isReg())
  RtReg = RtOp.getReg();
else
  ImmValue = RtOp.getImm(); // ← called even when isExpr() (release: UB)
```

**Debug build:** The `assert` fires → SIGABRT (exit 134).

**Release build:**  exit with 0.

---

## Impact

| Build Type | Behavior | Exit Code |
|------------|----------|-----------|
| Debug | Assertion failure (SIGABRT) | 134 |
| Release | Exit | 0 |

**Affected Instructions:**

| Instruction | Architecture | Debug | Release |
|-------------|--------------|-------|---------|
| `div $rd, $rs, f0` | MIPS32 | 134 | 0 |
| `divu $rd, $rs, f0` | MIPS32 | 134 | 0 |
| `rem $rd, $rs, f0` | MIPS32 | 134 | 0 |
| `remu $rd, $rs, f0` | MIPS32 | 134 | 0 |
| `ddiv $rd, $rs, f0` | MIPS64 | 134 | 0 |
| `ddivu $rd, $rs, f0` | MIPS64 | 134 | 0 |
| `drem $rd, $rs, f0` | MIPS64 | 134 | 0|
| `dremu $rd, $rs, f0` | MIPS64 | 134 | 0 |

**Trigger Symbols:** Any MIPS register-class name: `f0`–`f31`, `t0`–`t9`, `lo`, `hi`, `w0`–`w31`, `fcc0`–`fcc7`, `ac0`–`ac3`.


