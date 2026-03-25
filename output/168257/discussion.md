# RISCV Legalizer miscompiles with global-isel

**Author:** hiraditya
**URL:** https://github.com/llvm/llvm-project/issues/168257

## Body

This test case is same as #168256 except the global-isel flag

$ llc -x mir -mtriple=riscv64  -verify-machineinstrs -global-isel

Repro

$ llc  -x mir -mtriple=riscv64  -verify-machineinstrs

```llvm
--- |
  define i64 @test_64bit_ops(i64 %a, i64 %b) {
  entry:
    %shl = shl i64 %a, 32
    %or = or i64 %shl, %b
    ret i64 %or
  }
...
---
name:            test_64bit_ops
alignment:       4
tracksRegLiveness: true
registers:
  - { id: 0, class: gpr }
  - { id: 1, class: gpr }
  - { id: 2, class: gpr }
  - { id: 3, class: gpr }
liveins:
  - { reg: '$x10', virtual-reg: '%0' }
  - { reg: '$x11', virtual-reg: '%1' }
body:             |
  bb.0.entry:
    liveins: $x10, $x11

    %0:gpr = COPY $x10
    %1:gpr = COPY $x11
    %2:gpr = SLLI %0, 32
    %3:gpr = OR %2, %1
    %4:gpr = COPY %3
    PseudoRET implicit $x10
...
```


Output

```


# After Legalizer
# Machine code for function test_64bit_ops: IsSSA, TracksLiveness, Legalized
Function Live Ins: $x10 in %0, $x11 in %1, $x10, $x11

bb.0.entry:
  liveins: $x10, $x11
  PseudoRET implicit $x10

bb.2.entry:
  liveins: $x10, $x11
  %5:_(s64) = COPY $x10
  %6:_(s64) = COPY $x11
  %7:_(s64) = G_CONSTANT i64 32
  %8:_(s64) = G_SHL %5:_, %7:_(s64)
  %9:_(s64) = G_OR %8:_, %6:_
  $x10 = COPY %9:_(s64)
  PseudoRET implicit $x10

# End machine code for function test_64bit_ops.

*** Bad machine code: MBB has allocatable live-in, but isn't entry, landing-pad, or inlineasm-br-indirect-target. ***
- function:    test_64bit_ops
- basic block: %bb.2 entry (0x35f3df38)
- p. register: $x10

*** Bad machine code: MBB has allocatable live-in, but isn't entry, landing-pad, or inlineasm-br-indirect-target. ***
- function:    test_64bit_ops
- basic block: %bb.2 entry (0x35f3df38)
- p. register: $x11
LLVM ERROR: Found 2 machine code errors.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:

```

https://godbolt.org/z/er68aavqj

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: AdityaK (hiraditya)

<details>
This test case is same as #<!-- -->168256 except the global-isel flag

$ llc -x mir -mtriple=riscv64  -verify-machineinstrs -global-isel

Repro

$ llc  -x mir -mtriple=riscv64  -verify-machineinstrs

```llvm
--- |
  define i64 @<!-- -->test_64bit_ops(i64 %a, i64 %b) {
  entry:
    %shl = shl i64 %a, 32
    %or = or i64 %shl, %b
    ret i64 %or
  }
...
---
name:            test_64bit_ops
alignment:       4
tracksRegLiveness: true
registers:
  - { id: 0, class: gpr }
  - { id: 1, class: gpr }
  - { id: 2, class: gpr }
  - { id: 3, class: gpr }
liveins:
  - { reg: '$x10', virtual-reg: '%0' }
  - { reg: '$x11', virtual-reg: '%1' }
body:             |
  bb.0.entry:
    liveins: $x10, $x11

    %0:gpr = COPY $x10
    %1:gpr = COPY $x11
    %2:gpr = SLLI %0, 32
    %3:gpr = OR %2, %1
    %4:gpr = COPY %3
    PseudoRET implicit $x10
...
```


Output

```


# After Legalizer
# Machine code for function test_64bit_ops: IsSSA, TracksLiveness, Legalized
Function Live Ins: $x10 in %0, $x11 in %1, $x10, $x11

bb.0.entry:
  liveins: $x10, $x11
  PseudoRET implicit $x10

bb.2.entry:
  liveins: $x10, $x11
  %5:_(s64) = COPY $x10
  %6:_(s64) = COPY $x11
  %7:_(s64) = G_CONSTANT i64 32
  %8:_(s64) = G_SHL %5:_, %7:_(s64)
  %9:_(s64) = G_OR %8:_, %6:_
  $x10 = COPY %9:_(s64)
  PseudoRET implicit $x10

# End machine code for function test_64bit_ops.

*** Bad machine code: MBB has allocatable live-in, but isn't entry, landing-pad, or inlineasm-br-indirect-target. ***
- function:    test_64bit_ops
- basic block: %bb.2 entry (0x35f3df38)
- p. register: $x10

*** Bad machine code: MBB has allocatable live-in, but isn't entry, landing-pad, or inlineasm-br-indirect-target. ***
- function:    test_64bit_ops
- basic block: %bb.2 entry (0x35f3df38)
- p. register: $x11
LLVM ERROR: Found 2 machine code errors.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:

```

https://godbolt.org/z/er68aavqj
</details>


---

