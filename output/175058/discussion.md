# "EVL used by unexpected VPInstruction" error from VPlanVerifier for RV32

**Author:** asb
**URL:** https://github.com/llvm/llvm-project/issues/175058
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-01-13T07:55:18Z

## Body

This is a separate issue to #175028

Given this test case:
```llvm
define void @buf2img(ptr %imgX, i32 %size_x) {
entry:
  br label %for.cond70

for.cond70:                                       ; preds = %for.cond74, %entry
  %buf.addr.0 = phi ptr [ null, %entry ], [ %buf.addr.1, %for.cond74 ]
  br label %for.cond74

for.cond74:                                       ; preds = %for.body77, %for.cond70
  %buf.addr.1 = phi ptr [ %buf.addr.0, %for.cond70 ], [ %incdec.ptr, %for.body77 ]
  %i.1 = phi i32 [ 0, %for.cond70 ], [ %inc82, %for.body77 ]
  %cmp75 = icmp slt i32 %i.1, %size_x
  br i1 %cmp75, label %for.body77, label %for.cond70

for.body77:                                       ; preds = %for.cond74
  %incdec.ptr = getelementptr i8, ptr %buf.addr.1, i32 1
  %0 = load i8, ptr %buf.addr.1, align 1
  %conv78 = zext i8 %0 to i16
  store i16 %conv78, ptr %imgX, align 2
  %inc82 = add i32 %i.1, 1
  br label %for.cond74
}
```

We get (on HEAD):
```
$ ./build/rvdev/bin/clang --target=riscv32-linux-gnu -c -march=rv32gcv -O3 tc2.ll 
warning: overriding the module target triple with riscv32-unknown-linux-gnu
      [-Woverride-module]
EVL used by unexpected VPInstruction
EVL VPValue is not used correctly
clang: ../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7476: DenseMap<const SCEV *, Value *> llvm::LoopVectorizationPlanner::executePlan(ElementCount, unsigned int, VPlan &, InnerLoopVectorizer &, DominatorTree *, bool): Assertion `verifyVPlanIsValid(BestVPlan, true ) && "final VPlan is invalid"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: ./build/rvdev/bin/clang --target=riscv32-linux-gnu -c -march=rv32gcv -O3 tc2.ll
1.	Optimizer
2.	Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "tc2.ll"
3.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "buf2img"
 #0 0x00007e01b0aa7579 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) build/rvdev/../../llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00007e01b0aa7b9b PrintStackTraceSignalHandler(void*) build/rvdev/../../llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00007e01b0aa5631 llvm::sys::RunSignalHandlers() build/rvdev/../../llvm/lib/Support/Signals.cpp:108:5
 #3 0x00007e01b0aa6d0b llvm::sys::CleanupOnSignal(unsigned long) build/rvdev/../../llvm/lib/Support/Unix/Signals.inc:377:1
 #4 0x00007e01b08ec062 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) build/rvdev/../../llvm/lib/Support/CrashRecoveryContext.cpp:0:7
 #5 0x00007e01b08ec4c1 CrashRecoverySignalHandler(int) build/rvdev/../../llvm/lib/Support/CrashRecoveryContext.cpp:391:1
 #6 0x00007e01afe3e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #7 0x00007e01afe9890c (/usr/lib/libc.so.6+0x9890c)
 #8 0x00007e01afe3e3a0 raise (/usr/lib/libc.so.6+0x3e3a0)
 #9 0x00007e01afe2557a abort (/usr/lib/libc.so.6+0x2557a)
#10 0x00007e01afe254e3 __assert_perror_fail (/usr/lib/libc.so.6+0x254e3)
#11 0x00007e01b6987dc8 llvm::LoopVectorizationPlanner::executePlan(llvm::ElementCount, unsigned int, llvm::VPlan&, llvm::InnerLoopVectorizer&, llvm::DominatorTree*, bool) build/rvdev/../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:0:3
#12 0x00007e01b69939e0 llvm::LoopVectorizePass::processLoop(llvm::Loop*) build/rvdev/../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10090:5
#13 0x00007e01b69988fc llvm::LoopVectorizePass::runImpl(llvm::Function&) build/rvdev/../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10143:30
#14 0x00007e01b6998d7e llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) build/rvdev/../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10181:32
#15 0x00007e01ae25a8d4 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) build/rvdev/../../llvm/include/llvm/IR/PassManagerInternal.h:91:17
...
```
I can add `VPInstruction::PtrAdd` to the list of explicitly allowed instructions at around line 182 in VPlanVerifier.cpp, compilation works and and llvm-test-suite builds and runs for rv32v with evl tail folding (if the verifier workaround suggested in #175028 is also applied). I am not familiar enough with VPlan to know if this is a reasonable fix though, hence the issue rather than a patch.

CC @lukel97 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Alex Bradbury (asb)

<details>
This is a separate issue to #<!-- -->175028

Given this test case:
```llvm
define void @<!-- -->buf2img(ptr %imgX, i32 %size_x) {
entry:
  br label %for.cond70

for.cond70:                                       ; preds = %for.cond74, %entry
  %buf.addr.0 = phi ptr [ null, %entry ], [ %buf.addr.1, %for.cond74 ]
  br label %for.cond74

for.cond74:                                       ; preds = %for.body77, %for.cond70
  %buf.addr.1 = phi ptr [ %buf.addr.0, %for.cond70 ], [ %incdec.ptr, %for.body77 ]
  %i.1 = phi i32 [ 0, %for.cond70 ], [ %inc82, %for.body77 ]
  %cmp75 = icmp slt i32 %i.1, %size_x
  br i1 %cmp75, label %for.body77, label %for.cond70

for.body77:                                       ; preds = %for.cond74
  %incdec.ptr = getelementptr i8, ptr %buf.addr.1, i32 1
  %0 = load i8, ptr %buf.addr.1, align 1
  %conv78 = zext i8 %0 to i16
  store i16 %conv78, ptr %imgX, align 2
  %inc82 = add i32 %i.1, 1
  br label %for.cond74
}
```

We get (on HEAD):
```
$ ./build/rvdev/bin/clang --target=riscv32-linux-gnu -c -march=rv32gcv -O3 tc2.ll 
warning: overriding the module target triple with riscv32-unknown-linux-gnu
      [-Woverride-module]
EVL used by unexpected VPInstruction
EVL VPValue is not used correctly
clang: ../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7476: DenseMap&lt;const SCEV *, Value *&gt; llvm::LoopVectorizationPlanner::executePlan(ElementCount, unsigned int, VPlan &amp;, InnerLoopVectorizer &amp;, DominatorTree *, bool): Assertion `verifyVPlanIsValid(BestVPlan, true ) &amp;&amp; "final VPlan is invalid"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: ./build/rvdev/bin/clang --target=riscv32-linux-gnu -c -march=rv32gcv -O3 tc2.ll
1.	Optimizer
2.	Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "tc2.ll"
3.	Running pass "loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;" on function "buf2img"
 #<!-- -->0 0x00007e01b0aa7579 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) build/rvdev/../../llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00007e01b0aa7b9b PrintStackTraceSignalHandler(void*) build/rvdev/../../llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00007e01b0aa5631 llvm::sys::RunSignalHandlers() build/rvdev/../../llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00007e01b0aa6d0b llvm::sys::CleanupOnSignal(unsigned long) build/rvdev/../../llvm/lib/Support/Unix/Signals.inc:377:1
 #<!-- -->4 0x00007e01b08ec062 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) build/rvdev/../../llvm/lib/Support/CrashRecoveryContext.cpp:0:7
 #<!-- -->5 0x00007e01b08ec4c1 CrashRecoverySignalHandler(int) build/rvdev/../../llvm/lib/Support/CrashRecoveryContext.cpp:391:1
 #<!-- -->6 0x00007e01afe3e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #<!-- -->7 0x00007e01afe9890c (/usr/lib/libc.so.6+0x9890c)
 #<!-- -->8 0x00007e01afe3e3a0 raise (/usr/lib/libc.so.6+0x3e3a0)
 #<!-- -->9 0x00007e01afe2557a abort (/usr/lib/libc.so.6+0x2557a)
#<!-- -->10 0x00007e01afe254e3 __assert_perror_fail (/usr/lib/libc.so.6+0x254e3)
#<!-- -->11 0x00007e01b6987dc8 llvm::LoopVectorizationPlanner::executePlan(llvm::ElementCount, unsigned int, llvm::VPlan&amp;, llvm::InnerLoopVectorizer&amp;, llvm::DominatorTree*, bool) build/rvdev/../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:0:3
#<!-- -->12 0x00007e01b69939e0 llvm::LoopVectorizePass::processLoop(llvm::Loop*) build/rvdev/../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10090:5
#<!-- -->13 0x00007e01b69988fc llvm::LoopVectorizePass::runImpl(llvm::Function&amp;) build/rvdev/../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10143:30
#<!-- -->14 0x00007e01b6998d7e llvm::LoopVectorizePass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) build/rvdev/../../llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10181:32
#<!-- -->15 0x00007e01ae25a8d4 llvm::detail::PassModel&lt;llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) build/rvdev/../../llvm/include/llvm/IR/PassManagerInternal.h:91:17
...
```
I can add `VPInstruction::PtrAdd` to the list of explicitly allowed instructions at around line 182 in VPlanVerifier.cpp, compilation works and and llvm-test-suite builds and runs for rv32v with evl tail folding (if the verifier workaround suggested in #<!-- -->175028 is also applied). I am not familiar enough with VPlan to know if this is a reasonable fix though, hence the issue rather than a patch.

CC @<!-- -->lukel97 
</details>


---

