# Crash at libc.so: (anonymous namespace)::GeneratedRTChecks::getCost() LoopVectorize.cpp:0:0

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/164254
**Status:** Closed
**Labels:** crash, llvm:transforms
**Closed Date:** 2025-12-15T18:41:16Z

## Body

To reproduce run opt using -passes=loop-vectorize with the test below:
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @wombat(ptr addrspace(1) %arg, i64 %arg1) gc "statepoint-example" {
bb:
  br label %bb3

bb2:                                              ; preds = %bb7
  ret void

bb3:                                              ; preds = %bb7, %bb
  br label %bb4

bb4:                                              ; preds = %bb4, %bb3
  %phi = phi i64 [ %add6, %bb4 ], [ %arg1, %bb3 ]
  %phi5 = phi i64 [ %add, %bb4 ], [ 0, %bb3 ]
  %add = add i64 0, 0
  store i64 0, ptr addrspace(1) %arg, align 8
  %load = load i64, ptr addrspace(1) null, align 8
  %add6 = add i64 %phi, -3
  %icmp = icmp slt i64 0, %phi
  br i1 %icmp, label %bb4, label %bb7

bb7:                                              ; preds = %bb4
  br i1 %icmp, label %bb3, label %bb2, !llvm.loop !0
}

!0 = distinct !{!0, !1, !2, !3}
!1 = !{!"llvm.loop.peeled.count", i32 1}
!2 = !{!"llvm.loop.estimated_trip_count", i32 0}
!3 = !{!"llvm.loop.unroll.disable"}
```
Reproducer: https://godbolt.org/z/W5W91TEhM

Stack dump:
```
	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-vectorize <source>
1.	Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "<source>"
2.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "wombat"
 #0 0x0000000005945ce8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5945ce8)
 #1 0x0000000005942b94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007cec2dc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000000003670b82 (anonymous namespace)::GeneratedRTChecks::getCost() LoopVectorize.cpp:0:0
 #4 0x000000000369a7c0 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x369a7c0)
 #5 0x000000000369d130 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x369d130)
 #6 0x000000000369d85b llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x369d85b)
 #7 0x0000000002f8868e llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2f8868e)
 #8 0x00000000057202b1 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57202b1)
 #9 0x0000000000ef5fce llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xef5fce)
#10 0x000000000571e81a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x571e81a)
#11 0x000000000097887e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97887e)
#12 0x000000000571e1d1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x571e1d1)
#13 0x000000000098298a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98298a)
#14 0x0000000000976c0e optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x976c0e)
#15 0x00007cec2dc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#16 0x00007cec2dc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#17 0x000000000096df65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96df65)
Program terminated with signal: SIGFPE
Compiler returned: 136
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-libc

Author: None (TatyanaDoubts)

<details>
To reproduce run opt using -passes=loop-vectorize with the test below:
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @<!-- -->wombat(ptr addrspace(1) %arg, i64 %arg1) gc "statepoint-example" {
bb:
  br label %bb3

bb2:                                              ; preds = %bb7
  ret void

bb3:                                              ; preds = %bb7, %bb
  br label %bb4

bb4:                                              ; preds = %bb4, %bb3
  %phi = phi i64 [ %add6, %bb4 ], [ %arg1, %bb3 ]
  %phi5 = phi i64 [ %add, %bb4 ], [ 0, %bb3 ]
  %add = add i64 0, 0
  store i64 0, ptr addrspace(1) %arg, align 8
  %load = load i64, ptr addrspace(1) null, align 8
  %add6 = add i64 %phi, -3
  %icmp = icmp slt i64 0, %phi
  br i1 %icmp, label %bb4, label %bb7

bb7:                                              ; preds = %bb4
  br i1 %icmp, label %bb3, label %bb2, !llvm.loop !0
}

!0 = distinct !{!0, !1, !2, !3}
!1 = !{!"llvm.loop.peeled.count", i32 1}
!2 = !{!"llvm.loop.estimated_trip_count", i32 0}
!3 = !{!"llvm.loop.unroll.disable"}
```
Reproducer: https://godbolt.org/z/W5W91TEhM

Stack dump:
```
	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-vectorize &lt;source&gt;
1.	Running pass "function(loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;)" on module "&lt;source&gt;"
2.	Running pass "loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;" on function "wombat"
 #<!-- -->0 0x0000000005945ce8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5945ce8)
 #<!-- -->1 0x0000000005942b94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007cec2dc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000000003670b82 (anonymous namespace)::GeneratedRTChecks::getCost() LoopVectorize.cpp:0:0
 #<!-- -->4 0x000000000369a7c0 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x369a7c0)
 #<!-- -->5 0x000000000369d130 llvm::LoopVectorizePass::runImpl(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x369d130)
 #<!-- -->6 0x000000000369d85b llvm::LoopVectorizePass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x369d85b)
 #<!-- -->7 0x0000000002f8868e llvm::detail::PassModel&lt;llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2f8868e)
 #<!-- -->8 0x00000000057202b1 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57202b1)
 #<!-- -->9 0x0000000000ef5fce llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xef5fce)
#<!-- -->10 0x000000000571e81a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x571e81a)
#<!-- -->11 0x000000000097887e llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97887e)
#<!-- -->12 0x000000000571e1d1 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x571e1d1)
#<!-- -->13 0x000000000098298a llvm::runPassPipeline(llvm::StringRef, llvm::Module&amp;, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef&lt;llvm::PassPlugin&gt;, llvm::ArrayRef&lt;std::function&lt;void (llvm::PassBuilder&amp;)&gt;&gt;, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98298a)
#<!-- -->14 0x0000000000976c0e optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x976c0e)
#<!-- -->15 0x00007cec2dc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->16 0x00007cec2dc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->17 0x000000000096df65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96df65)
Program terminated with signal: SIGFPE
Compiler returned: 136
```
</details>


---

### Comment 2 - TatyanaDoubts

Fault commit for this bug is commit afb262855e755b499a733c2b84b6a1cb789b3b1f

Author: Joel E. Denny <[jdenny.ornl@gmail.com](mailto:jdenny.ornl@gmail.com)>
@jdenny-ornl 

Date:   Thu Oct 2 12:07:55 2025 -0400

 

    [LoopPeel] Fix branch weights' effect on block frequencies (#128785)

 

    [LoopPeel] Fix branch weights' effect on block frequencies

 

    This patch implements the LoopPeel changes discussed in [[RFC] Fix Loop

    Transformations to Preserve Block

    Frequencies]([[RFC] Fix Loop Transformations to Preserve Block Frequencies](https://discourse.llvm.org/t/rfc-fix-loop-transformations-to-preserve-block-frequencies/85785) ).

 

    In summary, a loop's latch block can have branch weight metadata that

    encodes an estimated trip count that is derived from application profile

    data. Initially, the loop body's block frequencies agree with the

    estimated trip count, as expected. However, sometimes loop

    transformations adjust those branch weights in a way that correctly

    maintains the estimated trip count but that corrupts the block

    frequencies. This patch addresses that problem in LoopPeel, which it

    changes to:

 

Maintain branch weights consistently with the original loop for the

    sake of preserving the total frequency of the original loop body.

Store the new estimated trip count in the

    `llvm.loop.estimated_trip_count` metadata, introduced by PR #148758.

---

### Comment 3 - jdenny-ornl

> Fault commit for this bug is commit [afb2628](https://github.com/llvm/llvm-project/commit/afb262855e755b499a733c2b84b6a1cb789b3b1f)

Thanks for the report.

For me, the above reproducer still reproduces at 5bbf72400cfe, the immediately preceding commit.

I think the problem is actually 0e3c5566c0c6, which changes getLoopEstimatedTripCount so it no longer guarantees its result is non-zero.  I will work on a fix.

The reproducer above explicitly sets `llvm.loop.estimated_trip_count` to zero.  Where did that reproducer come from?

---

### Comment 4 - TatyanaDoubts

The reproducer was extracted from the crash dump of java test generated by Fuzzer.

---

### Comment 5 - jdenny-ornl

If possible, would you please try PR #167792?  It will not fix the reproducer attached to this issue.  Instead, it makes the `!{!"llvm.loop.estimated_trip_count", i32 0}` there invalid.  However, my expectation is that it will fix your original java reproducer because the `!{!"llvm.loop.estimated_trip_count", i32 0}` will no longer be generated.

---

### Comment 6 - jdenny-ornl

> If possible, would you please try PR [#167792](https://github.com/llvm/llvm-project/pull/167792)?

I've rewritten that PR.  Please try it now.  Sorry for the churn.

> It will not fix the reproducer attached to this issue.

It does now, but please try your original java reproducer too.


---

### Comment 7 - jdenny-ornl

@TatyanaDoubts PR #167792 has been merged into main.  Let us know if it resolves the issue for you.

---

### Comment 8 - TatyanaDoubts

PR https://github.com/llvm/llvm-project/pull/167792 fixed the issue.

---

### Comment 9 - jdenny-ornl

Thanks for confirming.

---

