# [RISCV] Codegen crashes on splat vectors with illegal types

**Author:** mshockwave
**URL:** https://github.com/llvm/llvm-project/issues/133020
**Status:** Closed
**Labels:** backend:RISC-V, crash-on-valid
**Closed Date:** 2025-03-26T19:07:04Z

## Body

Given the following IR:
``` llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define void @foo(<3 x float> %0) #0 {
entry:
  %1 = extractelement <3 x float> %0, i64 0
  %broadcast.splatinsert = insertelement <vscale x 2 x float> zeroinitializer, float %1, i64 0
  %broadcast.splat = shufflevector <vscale x 2 x float> %broadcast.splatinsert, <vscale x 2 x float> zeroinitializer, <vscale x 2 x i32> zeroinitializer
  %interleaved.vec = tail call <vscale x 4 x float> @llvm.vector.interleave2.nxv4f32(<vscale x 2 x float> zeroinitializer, <vscale x 2 x float> %broadcast.splat)
  tail call void @llvm.vp.store.nxv4f32.p0(<vscale x 4 x float> %interleaved.vec, ptr null, <vscale x 4 x i1> splat (i1 true), i32 0)
  ret void
}

attributes #0 = { "target-features"="+64bit,+a,+c,+d,+experimental,+experimental-zicfilp,+experimental-zicfiss,+f,+m,+relax,+v,+zaamo,+zalrsc,+zba,+zbb,+zbs,+zca,+zcd,+zcmop,+zicsr,+zifencei,+zimop,+zmmul,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvl128b,+zvl32b,+zvl64b,-zvl1024b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl4096b,-zvl512b,-zvl65536b,-zvl8192b" }
```
And this command:
```
llc -O3 input.ll
```
The compiler crashes during RISC-V-specific DAG combining.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Min-Yih Hsu (mshockwave)

<details>
Given the following IR:
``` llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define void @<!-- -->foo(&lt;3 x float&gt; %0) #<!-- -->0 {
entry:
  %1 = extractelement &lt;3 x float&gt; %0, i64 0
  %broadcast.splatinsert = insertelement &lt;vscale x 2 x float&gt; zeroinitializer, float %1, i64 0
  %broadcast.splat = shufflevector &lt;vscale x 2 x float&gt; %broadcast.splatinsert, &lt;vscale x 2 x float&gt; zeroinitializer, &lt;vscale x 2 x i32&gt; zeroinitializer
  %interleaved.vec = tail call &lt;vscale x 4 x float&gt; @<!-- -->llvm.vector.interleave2.nxv4f32(&lt;vscale x 2 x float&gt; zeroinitializer, &lt;vscale x 2 x float&gt; %broadcast.splat)
  tail call void @<!-- -->llvm.vp.store.nxv4f32.p0(&lt;vscale x 4 x float&gt; %interleaved.vec, ptr null, &lt;vscale x 4 x i1&gt; splat (i1 true), i32 0)
  ret void
}

attributes #<!-- -->0 = { "target-features"="+64bit,+a,+c,+d,+experimental,+experimental-zicfilp,+experimental-zicfiss,+f,+m,+relax,+v,+zaamo,+zalrsc,+zba,+zbb,+zbs,+zca,+zcd,+zcmop,+zicsr,+zifencei,+zimop,+zmmul,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvl128b,+zvl32b,+zvl64b,-zvl1024b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl4096b,-zvl512b,-zvl65536b,-zvl8192b" }
```
And this command:
```
llc -O3 input.ll
```
The compiler crashes during RISC-V-specific DAG combining.
</details>


---

