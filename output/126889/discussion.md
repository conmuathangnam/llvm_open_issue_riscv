# Zero-Extend is dropped in DAG->DAG pass

**Author:** wthaller-blackshark
**URL:** https://github.com/llvm/llvm-project/issues/126889
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2025-02-18T04:26:06Z

## Body

Observed with llc 18.1.0, 18.1.3, 19.1.0 and with whatever `trunk` is on godbolt as of Feb 12,2025.
Not observed with llc 17.1.0.

The "X86 DAG->DAG Instruction Selection" pass drops `zext` in a situation where it is needed.

I have code that loads an i8 from a pointer (function `bad` below), passes that to a function (`helper`) that takes an i8, and then performs `1.0 / (uint64_t(1) << x)`.
The DAG->DAG instruction selection pass on the first function changes the 8-bit load to a 32-bit load, and the DAG->DAG instruction selection pass on the called function optimizes the instruction sequence for the above expression but chooses not to do a zero extend.

If I put the same code into a single function (function `good`), the load is compiled as a `movzx`.

```
target triple = "x86_64-pc-linux-gnu"

define double @bad(ptr %0)  {
  %2 = getelementptr i8, ptr %0, i64 16
  %3 = load i8, ptr %2, align 8
  %result = tail call fastcc double @helper(i8 %3)
  ret double %result
}

define double @helper(i8 %0){
  %2 = zext i8 %0 to i64
  %3 = shl nuw i64 1, %2
  %4 = uitofp i64 %3 to double
  %5 = fdiv double 1.000000e+00, %4
  ret double %5
}

define double @good(ptr %0)  {
  %2 = getelementptr i8, ptr %0, i64 16
  %3 = load i8, ptr %2, align 8
  %4 = zext i8 %3 to i64
  %5 = shl nuw i64 1, %4
  %6 = uitofp i64 %5 to double
  %result = fdiv double 1.000000e+00, %6
  ret double %result
}
```

Output (directives removed for clarity):
```
bad:                                    # @bad
        mov     edi, dword ptr [rdi + 16]
        jmp     helper@PLT                      # TAILCALL
helper:                                 # @helper
        shl     rdi, 52
        movabs  rax, 4607182418800017408
        sub     rax, rdi
        movq    xmm0, rax
        ret
good:                                   # @good
        movzx   eax, byte ptr [rdi + 16]
        xor     rax, 1023
        shl     rax, 52
        movq    xmm0, rax
        ret
```

Note the difference between the `movzx` in `good` and the `mov` in `bad`.

