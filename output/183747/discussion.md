# AArch64: 'Unsupported library call operation' for uitofp/fptosi i256 <-> double

**Author:** koko1123
**URL:** https://github.com/llvm/llvm-project/issues/183747
**Status:** Open
**Labels:** backend:AArch64, crash

## Body

## Description

The AArch64 backend crashes with `LLVM ERROR: Unsupported library call operation!` when lowering `uitofp i256 %val to double` or `fptoui double %val to i256`. This works on x86_64 but fails on AArch64.

This is a regression -- Zig 0.15.2 (which bundles LLVM 19) can compile this in some contexts, but Zig 0.16.0-dev (LLVM 21.1) crashes consistently.

## Reproducer (LLVM IR)

```llvm
define double @u256_to_f64(i256 %val) {
  %result = uitofp i256 %val to double
  ret double %result
}

define i256 @f64_to_u256(double %val) {
  %result = fptoui double %val to i256
  ret i256 %result
}
```

```bash
# Save as repro.ll and run:
llc -mtriple=aarch64 repro.ll -o /dev/null
# LLVM ERROR: Unsupported library call operation!
```

## Reproducer (Zig, which emits the above IR)

```zig
const std = @import("std");

const ETHER: u256 = 1_000_000_000_000_000_000;

fn parseEther(ether: f64) u256 {
    const wei_f = ether * @as(f64, @floatFromInt(ETHER));
    return @intFromFloat(wei_f);
}

test "parse ether" {
    const x = parseEther(1.0);
    try std.testing.expect(x > 0);
}
```

```bash
zig test repro.zig  # on aarch64-macos
# LLVM ERROR: Unsupported library call operation!
```

## Context

This affects real-world code in Ethereum libraries that use u256 (the native word size for the EVM) with float conversions for unit formatting (wei <-> ether). The workaround is to route through u128 intermediates, but the x86_64 backend handles i256 float conversions correctly so this appears to be an AArch64-specific gap.

## Related issues

- [ziglang/zig#18820](https://github.com/ziglang/zig/issues/18820) -- Zig tracker for this LLVM limitation
- [StrobeLabs/eth.zig#9](https://github.com/StrobeLabs/eth.zig/issues/9) -- Real-world impact on Ethereum library
- #91979 -- Similar "Unsupported library call operation" on AArch64 (closed as x86_fp80-specific, but the underlying lowering gap is related)
- #52675 -- Similar crash with _ExtInt types

## Environment

- LLVM 21.1 (bundled in Zig 0.16.0-dev.2565+684032671)
- Target: aarch64-macos (Apple Silicon M-series)
- Also affects: aarch64-linux (per ziglang/zig#18820)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Amlandeep Bhadra (koko1123)

<details>
## Description

The AArch64 backend crashes with `LLVM ERROR: Unsupported library call operation!` when lowering `uitofp i256 %val to double` or `fptoui double %val to i256`. This works on x86_64 but fails on AArch64.

This is a regression -- Zig 0.15.2 (which bundles LLVM 19) can compile this in some contexts, but Zig 0.16.0-dev (LLVM 21.1) crashes consistently.

## Reproducer (LLVM IR)

```llvm
define double @<!-- -->u256_to_f64(i256 %val) {
  %result = uitofp i256 %val to double
  ret double %result
}

define i256 @<!-- -->f64_to_u256(double %val) {
  %result = fptoui double %val to i256
  ret i256 %result
}
```

```bash
# Save as repro.ll and run:
llc -mtriple=aarch64 repro.ll -o /dev/null
# LLVM ERROR: Unsupported library call operation!
```

## Reproducer (Zig, which emits the above IR)

```zig
const std = @<!-- -->import("std");

const ETHER: u256 = 1_000_000_000_000_000_000;

fn parseEther(ether: f64) u256 {
    const wei_f = ether * @<!-- -->as(f64, @<!-- -->floatFromInt(ETHER));
    return @<!-- -->intFromFloat(wei_f);
}

test "parse ether" {
    const x = parseEther(1.0);
    try std.testing.expect(x &gt; 0);
}
```

```bash
zig test repro.zig  # on aarch64-macos
# LLVM ERROR: Unsupported library call operation!
```

## Context

This affects real-world code in Ethereum libraries that use u256 (the native word size for the EVM) with float conversions for unit formatting (wei &lt;-&gt; ether). The workaround is to route through u128 intermediates, but the x86_64 backend handles i256 float conversions correctly so this appears to be an AArch64-specific gap.

## Related issues

- [ziglang/zig#<!-- -->18820](https://github.com/ziglang/zig/issues/18820) -- Zig tracker for this LLVM limitation
- [StrobeLabs/eth.zig#<!-- -->9](https://github.com/StrobeLabs/eth.zig/issues/9) -- Real-world impact on Ethereum library
- #<!-- -->91979 -- Similar "Unsupported library call operation" on AArch64 (closed as x86_fp80-specific, but the underlying lowering gap is related)
- #<!-- -->52675 -- Similar crash with _ExtInt types

## Environment

- LLVM 21.1 (bundled in Zig 0.16.0-dev.2565+684032671)
- Target: aarch64-macos (Apple Silicon M-series)
- Also affects: aarch64-linux (per ziglang/zig#<!-- -->18820)
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 22 or `main` branch? Only most recent release is maintained.


---

### Comment 3 - koko1123

Tested on LLVM 22.1.0 (official release binary, `LLVM-22.1.0-macOS-ARM64.tar.xz`). Still crashes:

```
$ llc --version
LLVM (http://llvm.org/):
  LLVM version 22.1.0
  Optimized build.
  Default target: arm64-apple-darwin25.1.0
  Host CPU: apple-m4

$ llc -mtriple=aarch64 repro.ll -o /dev/null
LLVM ERROR: unsupported library call operation
```

Crash is in `DAGTypeLegalizer::ExpandIntOp_XINT_TO_FP` -> `TargetLowering::makeLibCall` when trying to find a libcall for `uitofp i256 -> double` on AArch64.

Stack trace:
```
 #8 llvm::TargetLowering::makeLibCall(..., RTLIB::LibcallImpl, ...)
 #9 llvm::DAGTypeLegalizer::ExpandIntOp_XINT_TO_FP(llvm::SDNode*)
#10 llvm::DAGTypeLegalizer::ExpandIntegerOperand(llvm::SDNode*, unsigned int)
#11 llvm::DAGTypeLegalizer::run()
#12 llvm::SelectionDAG::LegalizeTypes()
#13 llvm::SelectionDAGISel::CodeGenAndEmitDAG()
```

Also confirmed on LLVM 21.1.8 (Homebrew). Both crash identically on the same reproducer in the issue description.

---

### Comment 4 - koko1123

tagging @Mario-SO for visbility 

---

### Comment 5 - MacDue

Can't reproduce on `main`: https://godbolt.org/z/j1brKrMxd 

---

