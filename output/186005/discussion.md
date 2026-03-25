# "VPlan cost model and legacy cost model disagreed" assert for rv32gc_zve32f_zvl128b

**Author:** asb
**URL:** https://github.com/llvm/llvm-project/issues/186005

## Body

This is specifically for rv32gc_zve32f_zvl128. rv32gcv is fine. See also: the now fixed #175909

tc.ll:
```llvm
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-unknown-linux-gnu"

@itrans8x8.a = internal global [8 x i32] zeroinitializer

define void @itrans8x8(ptr %img, i32 %joff, ptr %m723, ptr %arrayidx25, ptr %arrayidx28, ptr %itrans8x8.a, ptr %0, ptr %arrayidx33, ptr %1, ptr %arrayidx35, ptr %2) #0 {
entry:
  br label %for.cond19

for.cond19:                                       ; preds = %for.body22, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc111, %for.body22 ]
  %cmp20 = icmp ult i32 %i.0, 8
  br i1 %cmp20, label %for.body22, label %for.cond113

for.body22:                                       ; preds = %for.cond19
  %arrayidx252 = getelementptr [64 x i8], ptr %m723, i32 %i.0
  %3 = load i32, ptr %arrayidx252, align 4
  %add29 = or i32 %3, 1
  store i32 %add29, ptr %itrans8x8.a, align 4
  %4 = load i32, ptr %arrayidx252, align 4
  %sub = sub i32 0, %4
  store i32 %sub, ptr %0, align 4
  %5 = load i32, ptr %arrayidx252, align 4
  %shr = ashr i32 %5, 1
  %sub34 = or i32 %shr, 1
  store i32 %sub34, ptr %1, align 4
  %6 = load i32, ptr %img, align 4
  %shr37 = ashr i32 %6, 1
  %add38 = or i32 %shr37, 1
  store i32 %add38, ptr %2, align 4
  store i32 0, ptr %img, align 4
  store i32 0, ptr %2, align 4
  store i32 0, ptr %img, align 4
  %arrayidx43 = getelementptr i8, ptr %arrayidx252, i32 12
  %7 = load i32, ptr %arrayidx43, align 4
  %8 = load i32, ptr %arrayidx252, align 4
  %9 = or i32 %7, %8
  %10 = or i32 %9, 1
  %sub51 = sub i32 0, %10
  store i32 %sub51, ptr %arrayidx35, align 4
  %11 = load i32, ptr %arrayidx43, align 4
  %12 = load i32, ptr %arrayidx252, align 4
  %shr58 = ashr i32 %11, 1
  %13 = or i32 %shr58, 1
  %14 = or i32 %12, %13
  %sub59 = or i32 %14, %joff
  store i32 %sub59, ptr %arrayidx33, align 4
  %arrayidx64 = getelementptr i8, ptr %arrayidx252, i32 20
  %15 = load i32, ptr %arrayidx252, align 4
  %add63 = or i32 %15, 1
  %add65 = add i32 %add63, 1
  %16 = load i32, ptr %arrayidx64, align 4
  %shr67 = ashr i32 %16, 1
  %add68 = or i32 %add65, %shr67
  store i32 %add68, ptr getelementptr inbounds nuw (i8, ptr @itrans8x8.a, i32 20), align 4
  store i32 0, ptr %arrayidx25, align 4
  store i32 0, ptr %arrayidx28, align 4
  %17 = load i32, ptr getelementptr inbounds nuw (i8, ptr @itrans8x8.a, i32 20), align 4
  %shr82 = ashr i32 %17, 1
  %add83 = or i32 %shr82, 1
  store i32 %add83, ptr %arrayidx25, align 4
  %inc111 = add i32 %i.0, 1
  br label %for.cond19

for.cond113:                                      ; preds = %for.cond19
  ret void
}

attributes #0 = { "target-features"="+zve32f,+zvl128b" }

```

Reproduce:
```
./build/rvrel/bin/opt "-passes=default<O3>" -S tc.ll
opt: ../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7283: VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || !Legal->getLAI()->getSymbolicStrides().empty() || UsesEVLGatherScatter || planContainsAdditionalSimplifications( getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) && " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./llvm-project/build/allrel/bin/opt -passes=default<O3> -S tc.ll
1.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "tc.ll"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "itrans8x8"
 #0 0x000055985e131866 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./llvm-project/build/allrel/bin/opt+0x4ed2866)
 #1 0x000055985e12ec85 llvm::sys::RunSignalHandlers() (./llvm-project/build/allrel/bin/opt+0x4ecfc85)
 #2 0x000055985e132984 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007effd364d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #4 0x00007effd36a7a2c (/usr/lib/libc.so.6+0x98a2c)
 #5 0x00007effd364d1a0 raise (/usr/lib/libc.so.6+0x3e1a0)
 #6 0x00007effd36345fe abort (/usr/lib/libc.so.6+0x255fe)
 #7 0x00007effd3635697 (/usr/lib/libc.so.6+0x26697)
 #8 0x00007effd36450b2 __assert_fail (/usr/lib/libc.so.6+0x360b2)
 #9 0x000055985f810109 (./llvm-project/build/allrel/bin/opt+0x65b1109)
#10 0x000055985f821add llvm::LoopVectorizePass::processLoop(llvm::Loop*) (./llvm-project/build/allrel/bin/opt+0x65c2add)
#11 0x000055985f82c63b llvm::LoopVectorizePass::runImpl(llvm::Function&) (./llvm-project/build/allrel/bin/opt+0x65cd63b)
#12 0x000055985f82cf42 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./llvm-project/build/allrel/bin/opt+0x65cdf42)
...
```

CC @lukel97 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Alex Bradbury (asb)

<details>
This is specifically for rv32gc_zve32f_zvl128. rv32gcv is fine. See also: the now fixed #<!-- -->175909

tc.ll:
```llvm
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-unknown-linux-gnu"

@<!-- -->itrans8x8.a = internal global [8 x i32] zeroinitializer

define void @<!-- -->itrans8x8(ptr %img, i32 %joff, ptr %m723, ptr %arrayidx25, ptr %arrayidx28, ptr %itrans8x8.a, ptr %0, ptr %arrayidx33, ptr %1, ptr %arrayidx35, ptr %2) #<!-- -->0 {
entry:
  br label %for.cond19

for.cond19:                                       ; preds = %for.body22, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc111, %for.body22 ]
  %cmp20 = icmp ult i32 %i.0, 8
  br i1 %cmp20, label %for.body22, label %for.cond113

for.body22:                                       ; preds = %for.cond19
  %arrayidx252 = getelementptr [64 x i8], ptr %m723, i32 %i.0
  %3 = load i32, ptr %arrayidx252, align 4
  %add29 = or i32 %3, 1
  store i32 %add29, ptr %itrans8x8.a, align 4
  %4 = load i32, ptr %arrayidx252, align 4
  %sub = sub i32 0, %4
  store i32 %sub, ptr %0, align 4
  %5 = load i32, ptr %arrayidx252, align 4
  %shr = ashr i32 %5, 1
  %sub34 = or i32 %shr, 1
  store i32 %sub34, ptr %1, align 4
  %6 = load i32, ptr %img, align 4
  %shr37 = ashr i32 %6, 1
  %add38 = or i32 %shr37, 1
  store i32 %add38, ptr %2, align 4
  store i32 0, ptr %img, align 4
  store i32 0, ptr %2, align 4
  store i32 0, ptr %img, align 4
  %arrayidx43 = getelementptr i8, ptr %arrayidx252, i32 12
  %7 = load i32, ptr %arrayidx43, align 4
  %8 = load i32, ptr %arrayidx252, align 4
  %9 = or i32 %7, %8
  %10 = or i32 %9, 1
  %sub51 = sub i32 0, %10
  store i32 %sub51, ptr %arrayidx35, align 4
  %11 = load i32, ptr %arrayidx43, align 4
  %12 = load i32, ptr %arrayidx252, align 4
  %shr58 = ashr i32 %11, 1
  %13 = or i32 %shr58, 1
  %14 = or i32 %12, %13
  %sub59 = or i32 %14, %joff
  store i32 %sub59, ptr %arrayidx33, align 4
  %arrayidx64 = getelementptr i8, ptr %arrayidx252, i32 20
  %15 = load i32, ptr %arrayidx252, align 4
  %add63 = or i32 %15, 1
  %add65 = add i32 %add63, 1
  %16 = load i32, ptr %arrayidx64, align 4
  %shr67 = ashr i32 %16, 1
  %add68 = or i32 %add65, %shr67
  store i32 %add68, ptr getelementptr inbounds nuw (i8, ptr @<!-- -->itrans8x8.a, i32 20), align 4
  store i32 0, ptr %arrayidx25, align 4
  store i32 0, ptr %arrayidx28, align 4
  %17 = load i32, ptr getelementptr inbounds nuw (i8, ptr @<!-- -->itrans8x8.a, i32 20), align 4
  %shr82 = ashr i32 %17, 1
  %add83 = or i32 %shr82, 1
  store i32 %add83, ptr %arrayidx25, align 4
  %inc111 = add i32 %i.0, 1
  br label %for.cond19

for.cond113:                                      ; preds = %for.cond19
  ret void
}

attributes #<!-- -->0 = { "target-features"="+zve32f,+zvl128b" }

```

Reproduce:
```
./build/rvrel/bin/opt "-passes=default&lt;O3&gt;" -S tc.ll
opt: ../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7283: VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || !Legal-&gt;getLAI()-&gt;getSymbolicStrides().empty() || UsesEVLGatherScatter || planContainsAdditionalSimplifications( getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) &amp;&amp; " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: ./llvm-project/build/allrel/bin/opt -passes=default&lt;O3&gt; -S tc.ll
1.	Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "tc.ll"
2.	Running pass "loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;" on function "itrans8x8"
 #<!-- -->0 0x000055985e131866 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (./llvm-project/build/allrel/bin/opt+0x4ed2866)
 #<!-- -->1 0x000055985e12ec85 llvm::sys::RunSignalHandlers() (./llvm-project/build/allrel/bin/opt+0x4ecfc85)
 #<!-- -->2 0x000055985e132984 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007effd364d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #<!-- -->4 0x00007effd36a7a2c (/usr/lib/libc.so.6+0x98a2c)
 #<!-- -->5 0x00007effd364d1a0 raise (/usr/lib/libc.so.6+0x3e1a0)
 #<!-- -->6 0x00007effd36345fe abort (/usr/lib/libc.so.6+0x255fe)
 #<!-- -->7 0x00007effd3635697 (/usr/lib/libc.so.6+0x26697)
 #<!-- -->8 0x00007effd36450b2 __assert_fail (/usr/lib/libc.so.6+0x360b2)
 #<!-- -->9 0x000055985f810109 (./llvm-project/build/allrel/bin/opt+0x65b1109)
#<!-- -->10 0x000055985f821add llvm::LoopVectorizePass::processLoop(llvm::Loop*) (./llvm-project/build/allrel/bin/opt+0x65c2add)
#<!-- -->11 0x000055985f82c63b llvm::LoopVectorizePass::runImpl(llvm::Function&amp;) (./llvm-project/build/allrel/bin/opt+0x65cd63b)
#<!-- -->12 0x000055985f82cf42 llvm::LoopVectorizePass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (./llvm-project/build/allrel/bin/opt+0x65cdf42)
...
```

CC @<!-- -->lukel97 
</details>


---

### Comment 2 - lukel97

This loop generates a VPInstruction::LastActiveLane which is hard wired to return an i64 type, so the VPlan cost model correctly returns that this is an illegal type. The legacy cost model doesn't seem to detect this.

We could either teach the legacy cost model to detect the invalid cost, or relax VPInstruction::{First,Last}ActiveLane to use the canonical IV type. I'll give the latter a try tomorrow and see if there was a specific reason as to why it was hardwired to i64 to begin with

---

