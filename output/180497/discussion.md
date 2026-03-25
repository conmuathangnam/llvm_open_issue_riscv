# [AArch64] getPromotedVTForPredicate: Unreachable "unexpected element count for vector"

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/180497
**Status:** Open
**Labels:** backend:AArch64, crash

## Body

This IR:

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define <vscale x 1 x i1> @_Z4testbjhyhixbixjthshiPhPjPxS1_PbPA17_A17_yS_PA17_A17_aPA17_A17_xPA17_A17_bSB_PS3_SB_S5_PA24_sS_S2_PA24_SG_PiS_PA18_A18_jPtPA18_A18_t() #0 {
entry:
  %broadcast.splatinsert1188 = insertelement <vscale x 1 x i1> zeroinitializer, i1 false, i64 0
  ret <vscale x 1 x i1> %broadcast.splatinsert1188
}

attributes #0 = { "target-cpu"="grace" }
```

Triggers this assert / unreachable:

```
unexpected element count for vector
UNREACHABLE executed at llvm-project/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp:227!
```

Stack trace:

```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@_Z4testbjhyhixbixjthshiPhPjPxS1_PbPA17_A17_yS_PA17_A17_aPA17_A17_xPA17_A17_bSB_PS3_SB_S5_PA24_sS_S2_PA24_SG_PiS_PA18_A18_jPtPA18_A18_t'
 #0 0x00000000042e2088 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42e2088)
 #1 0x00000000042def64 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000071a0f8842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000071a0f88969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000071a0f8842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000071a0f88287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000042238fa (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42238fa)
 #7 0x0000000000b16d73 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xb16d73)
 #8 0x0000000000b7d917 llvm::AArch64TargetLowering::LowerINSERT_VECTOR_ELT(llvm::SDValue, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xb7d917)
 #9 0x0000000000be06e3 llvm::AArch64TargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xbe06e3)
```

See also: https://godbolt.org/z/8nna63zq9

This can be triggered from source-code.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Sjoerd Meijer (sjoerdmeijer)

<details>
This IR:

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define &lt;vscale x 1 x i1&gt; @<!-- -->_Z4testbjhyhixbixjthshiPhPjPxS1_PbPA17_A17_yS_PA17_A17_aPA17_A17_xPA17_A17_bSB_PS3_SB_S5_PA24_sS_S2_PA24_SG_PiS_PA18_A18_jPtPA18_A18_t() #<!-- -->0 {
entry:
  %broadcast.splatinsert1188 = insertelement &lt;vscale x 1 x i1&gt; zeroinitializer, i1 false, i64 0
  ret &lt;vscale x 1 x i1&gt; %broadcast.splatinsert1188
}

attributes #<!-- -->0 = { "target-cpu"="grace" }
```

Triggers this assert / unreachable:

```
unexpected element count for vector
UNREACHABLE executed at llvm-project/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp:227!
```

Stack trace:

```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->_Z4testbjhyhixbixjthshiPhPjPxS1_PbPA17_A17_yS_PA17_A17_aPA17_A17_xPA17_A17_bSB_PS3_SB_S5_PA24_sS_S2_PA24_SG_PiS_PA18_A18_jPtPA18_A18_t'
 #<!-- -->0 0x00000000042e2088 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42e2088)
 #<!-- -->1 0x00000000042def64 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000071a0f8842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000071a0f88969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000071a0f8842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000071a0f88287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000042238fa (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x42238fa)
 #<!-- -->7 0x0000000000b16d73 (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xb16d73)
 #<!-- -->8 0x0000000000b7d917 llvm::AArch64TargetLowering::LowerINSERT_VECTOR_ELT(llvm::SDValue, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xb7d917)
 #<!-- -->9 0x0000000000be06e3 llvm::AArch64TargetLowering::LowerOperation(llvm::SDValue, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0xbe06e3)
```

See also: https://godbolt.org/z/8nna63zq9

This can be triggered from source-code.
</details>


---

### Comment 2 - woruyu

I would like to fix it!

---

