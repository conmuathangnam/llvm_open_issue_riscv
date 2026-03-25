# HIPSPV toolchain not tested on macOS CI — missing aligned allocation case crashes compiler

**Author:** pvelesko
**URL:** https://github.com/llvm/llvm-project/issues/186313
**Status:** Open
**Labels:** clang:driver, crash

## Body

## Summary

The HIPSPV toolchain (`spirv64` / `spirv64-unknown-chipstar` targets) has no macOS CI test coverage, which allowed a crash to go undetected: `alignedAllocMinVersion()` in `AlignedAllocation.h` hits `llvm_unreachable` for `Triple::ChipStar` because it has no `case` in the switch.

## How to reproduce

On macOS, compile any C++17 code targeting chipStar:

```
clang++ -target x86_64-apple-darwin --offload=spirv64-unknown-chipstar test.hip
```

The Darwin host toolchain's `addClangTargetOptions()` checks aligned allocation support for the device triple's OS, calls `alignedAllocMinVersion(Triple::ChipStar)`, and crashes.

## Root cause

- `hipspv-toolchain.hip` is only marked `UNSUPPORTED: system-windows, system-cygwin` — it's not unsupported on macOS, but no macOS CI bot runs it
- The `ChipStar` OS type was added to the `Triple` enum but `AlignedAllocation.h` was not updated with a corresponding `case`
- Linux CI doesn't catch this because the Linux host toolchain doesn't check aligned allocation availability

## Suggestion

1. Add `case llvm::Triple::ChipStar:` to `alignedAllocMinVersion()` (fix already prepared in https://github.com/llvm/llvm-project/pull/183991 and CHIP-SPV fork)
2. Consider adding macOS test coverage for HIPSPV targets to catch similar issues in the future (e.g., a macOS bot that runs `check-clang-driver` with the SPIR-V backend enabled)

## Affected files

- `clang/include/clang/Basic/AlignedAllocation.h` — missing switch case
- `clang/lib/Driver/ToolChains/HIPSPV.cpp` — `HostTC->addClangTargetOptions()` delegation exposes the crash on macOS

## Comments

### Comment 1 - pvelesko

Checked the upstream buildbot config ([builders.py](https://github.com/llvm/llvm-zorg/blob/main/buildbot/osuosl/master/config/builders.py)):

- Both macOS builders (`llvm-clang-x86_64-darwin`, `llvm-clang-aarch64-darwin`) use `-DLLVM_TARGETS_TO_BUILD=X86` / `AArch64` — **SPIRV is not included**
- The only builder with SPIRV is `intel-sycl-gpu` (Linux)
- So `REQUIRES: spirv-registered-target` causes all HIPSPV tests to be skipped on macOS CI

The fix for the crash itself is trivial (add the switch case), but preventing future regressions would require either adding SPIRV to an existing macOS builder's target list or creating a new one.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-driver

Author: Paulius Velesko (pvelesko)

<details>
## Summary

The HIPSPV toolchain (`spirv64` / `spirv64-unknown-chipstar` targets) has no macOS CI test coverage, which allowed a crash to go undetected: `alignedAllocMinVersion()` in `AlignedAllocation.h` hits `llvm_unreachable` for `Triple::ChipStar` because it has no `case` in the switch.

## How to reproduce

On macOS, compile any C++17 code targeting chipStar:

```
clang++ -target x86_64-apple-darwin --offload=spirv64-unknown-chipstar test.hip
```

The Darwin host toolchain's `addClangTargetOptions()` checks aligned allocation support for the device triple's OS, calls `alignedAllocMinVersion(Triple::ChipStar)`, and crashes.

## Root cause

- `hipspv-toolchain.hip` is only marked `UNSUPPORTED: system-windows, system-cygwin` — it's not unsupported on macOS, but no macOS CI bot runs it
- The `ChipStar` OS type was added to the `Triple` enum but `AlignedAllocation.h` was not updated with a corresponding `case`
- Linux CI doesn't catch this because the Linux host toolchain doesn't check aligned allocation availability

## Suggestion

1. Add `case llvm::Triple::ChipStar:` to `alignedAllocMinVersion()` (fix already prepared in https://github.com/llvm/llvm-project/pull/183991 and CHIP-SPV fork)
2. Consider adding macOS test coverage for HIPSPV targets to catch similar issues in the future (e.g., a macOS bot that runs `check-clang-driver` with the SPIR-V backend enabled)

## Affected files

- `clang/include/clang/Basic/AlignedAllocation.h` — missing switch case
- `clang/lib/Driver/ToolChains/HIPSPV.cpp` — `HostTC-&gt;addClangTargetOptions()` delegation exposes the crash on macOS
</details>


---

