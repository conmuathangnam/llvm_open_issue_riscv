# [AArch64][GlobalISel] Incorrect codegen for atomic store of bitcast half to i16 - reads NZCV instead of FP register

**Author:** IanButterworth
**URL:** https://github.com/llvm/llvm-project/issues/171494
**Status:** Closed
**Labels:** backend:AArch64, miscompilation, llvm:globalisel
**Closed Date:** 2025-12-13T10:48:07Z

## Body

## Description

GlobalISel on AArch64 generates incorrect code when storing an atomic i16 that was bitcast from half. Instead of using `fmov` to move the half value from the FP register to a GPR, it generates `mrs w8, NZCV` which reads the CPU condition flags.

## Reproducer

```llvm
define void @atomic_store_half(ptr %addr, half %val) {
  %ival = bitcast half %val to i16
  store atomic i16 %ival, ptr %addr release, align 2
  ret void
}
```

## Commands

```bash
# Buggy (GlobalISel):
$ llc -O0 -global-isel -mtriple=arm64-apple-darwin test.ll -o -

# Correct (SelectionDAG):
$ llc -O0 -global-isel=false -mtriple=arm64-apple-darwin test.ll -o -
```

## Expected output

```asm
_atomic_store_half:
    fmov    w8, s0          ; move half from FP register to GPR
    stlrh   w8, [x0]        ; atomic store release
    ret
```

## Actual output (GlobalISel)

```asm
_atomic_store_half:
    mrs     w8, NZCV        ; BUG: reads CPU flags instead of the half value!
    stlrh   w8, [x0]        ; stores garbage
    ret
```

## Analysis

GlobalISel fails to properly lower the `bitcast half %val to i16` when the result is used in an atomic store. Instead of generating `fmov w8, s0` to move the half value from the floating-point register (h0/s0) to a general-purpose register, it generates `mrs w8, NZCV` which reads the CPU condition flags.

## Tested with

- LLVM 20.1.8 (Julia's bundled LLVM) on arm64-apple-darwin
- LLVM 21.1.7 (Homebrew) on arm64-apple-darwin

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Ian Butterworth (IanButterworth)

<details>
## Description

GlobalISel on AArch64 generates incorrect code when storing an atomic i16 that was bitcast from half. Instead of using `fmov` to move the half value from the FP register to a GPR, it generates `mrs w8, NZCV` which reads the CPU condition flags.

## Reproducer

```llvm
define void @<!-- -->atomic_store_half(ptr %addr, half %val) {
  %ival = bitcast half %val to i16
  store atomic i16 %ival, ptr %addr release, align 2
  ret void
}
```

## Commands

```bash
# Buggy (GlobalISel):
$ llc -O0 -global-isel -mtriple=arm64-apple-darwin test.ll -o -

# Correct (SelectionDAG):
$ llc -O0 -mtriple=arm64-apple-darwin test.ll -o -
```

## Expected output

```asm
_atomic_store_half:
    fmov    w8, s0          ; move half from FP register to GPR
    stlrh   w8, [x0]        ; atomic store release
    ret
```

## Actual output (GlobalISel)

```asm
_atomic_store_half:
    mrs     w8, NZCV        ; BUG: reads CPU flags instead of the half value!
    stlrh   w8, [x0]        ; stores garbage
    ret
```

## Analysis

GlobalISel fails to properly lower the `bitcast half %val to i16` when the result is used in an atomic store. Instead of generating `fmov w8, s0` to move the half value from the floating-point register (h0/s0) to a general-purpose register, it generates `mrs w8, NZCV` which reads the CPU condition flags.

## Tested with

- LLVM 20.1.8 (Julia's bundled LLVM) on arm64-apple-darwin
- LLVM 21.1.7 (Homebrew) on arm64-apple-darwin
</details>


---

