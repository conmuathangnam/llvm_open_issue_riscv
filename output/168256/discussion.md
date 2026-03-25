# RISCV Instruction selection crash

**Author:** hiraditya
**URL:** https://github.com/llvm/llvm-project/issues/168256

## Body

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
# After Instruction Selection
# Machine code for function test_64bit_ops: IsSSA, NoPHIs, TracksLiveness
Function Live Ins: $x10 in %0, $x11 in %1, $x10 in %5, $x11 in %6

bb.0.entry:
  liveins: $x10, $x11, $x10, $x11, $x10, $x11
  %6:gpr = COPY $x11
  %5:gpr = COPY $x10
  %1:gpr = COPY $x11
  %0:gpr = COPY $x10
  %0:gpr = COPY $x10
  %1:gpr = COPY $x11
  %2:gpr = SLLI %0:gpr, 32
  %3:gpr = OR %2:gpr, %1:gpr
  %4:gpr = COPY %3:gpr
  PseudoRET implicit $x10

bb.1.entry:
  %7:gpr = SLLI %5:gpr, 32
  %8:gpr = OR killed %7:gpr, %6:gpr
  $x10 = COPY %8:gpr
  PseudoRET implicit $x10

# End machine code for function test_64bit_ops.

*** Bad machine code: Multiple virtual register defs in SSA form ***
- function:    test_64bit_ops
- basic block: %bb.0 entry (0x3c8824c8)
- instruction: %1:gpr = COPY $x11
- operand 0:   %1:gpr

*** Bad machine code: Multiple virtual register defs in SSA form ***
- function:    test_64bit_ops
- basic block: %bb.0 entry (0x3c8824c8)
- instruction: %0:gpr = COPY $x10
- operand 0:   %0:gpr

*** Bad machine code: Multiple virtual register defs in SSA form ***
- function:    test_64bit_ops
- basic block: %bb.0 entry (0x3c8824c8)
- instruction: %0:gpr = COPY $x10
- operand 0:   %0:gpr

*** Bad machine code: Multiple virtual register defs in SSA form ***
- function:    test_64bit_ops
- basic block: %bb.0 entry (0x3c8824c8)
- instruction: %1:gpr = COPY $x11
- operand 0:   %1:gpr
LLVM ERROR: Found 4 machine code errors.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -x mir -mtriple=riscv64 -verify-machineinstrs <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'Verify generated machine code' on function '@test_64bit_ops'
 #0 0x0000000003c04f68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3c04f68)

```


https://godbolt.org/z/dE64Weqbj

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: AdityaK (hiraditya)

<details>
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
# After Instruction Selection
# Machine code for function test_64bit_ops: IsSSA, NoPHIs, TracksLiveness
Function Live Ins: $x10 in %0, $x11 in %1, $x10 in %5, $x11 in %6

bb.0.entry:
  liveins: $x10, $x11, $x10, $x11, $x10, $x11
  %6:gpr = COPY $x11
  %5:gpr = COPY $x10
  %1:gpr = COPY $x11
  %0:gpr = COPY $x10
  %0:gpr = COPY $x10
  %1:gpr = COPY $x11
  %2:gpr = SLLI %0:gpr, 32
  %3:gpr = OR %2:gpr, %1:gpr
  %4:gpr = COPY %3:gpr
  PseudoRET implicit $x10

bb.1.entry:
  %7:gpr = SLLI %5:gpr, 32
  %8:gpr = OR killed %7:gpr, %6:gpr
  $x10 = COPY %8:gpr
  PseudoRET implicit $x10

# End machine code for function test_64bit_ops.

*** Bad machine code: Multiple virtual register defs in SSA form ***
- function:    test_64bit_ops
- basic block: %bb.0 entry (0x3c8824c8)
- instruction: %1:gpr = COPY $x11
- operand 0:   %1:gpr

*** Bad machine code: Multiple virtual register defs in SSA form ***
- function:    test_64bit_ops
- basic block: %bb.0 entry (0x3c8824c8)
- instruction: %0:gpr = COPY $x10
- operand 0:   %0:gpr

*** Bad machine code: Multiple virtual register defs in SSA form ***
- function:    test_64bit_ops
- basic block: %bb.0 entry (0x3c8824c8)
- instruction: %0:gpr = COPY $x10
- operand 0:   %0:gpr

*** Bad machine code: Multiple virtual register defs in SSA form ***
- function:    test_64bit_ops
- basic block: %bb.0 entry (0x3c8824c8)
- instruction: %1:gpr = COPY $x11
- operand 0:   %1:gpr
LLVM ERROR: Found 4 machine code errors.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -x mir -mtriple=riscv64 -verify-machineinstrs &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'Verify generated machine code' on function '@<!-- -->test_64bit_ops'
 #<!-- -->0 0x0000000003c04f68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3c04f68)

```


https://godbolt.org/z/dE64Weqbj
</details>


---

### Comment 2 - s-barannikov

How did you get this MIR? The LLVM code snippet compiles just fine and produces different MIR after instruction selection with -O0/-O2

---

### Comment 3 - hiraditya

I got it from print-after-all but it could be  a problem with the mir. 

---

### Comment 4 - lenary

Did you tell llc to only start *after* the pass that emitted the MIR shown? That MIR shown is post-isel, because it contains real RISC-V instructions.

---

