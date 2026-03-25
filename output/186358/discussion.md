# [Coroutines] coro-split crashes when salvaging a swiftasync entry-value dbg expression

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/186358
**Status:** Open
**Labels:** crash, coroutines

## Body

Here is a crash case when reviewing https://github.com/llvm/llvm-project/pull/176766:

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-unknown"

@coroutineATu = global <{ i32, i32 }> <{ i32 trunc (i64 sub (i64 ptrtoint (ptr @coroutineA to i64), i64 ptrtoint (ptr @coroutineATu to i64)) to i32), i32 16 }>, align 8

; Function Attrs: presplitcoroutine
define swifttailcc void @coroutineA(ptr swiftasync %arg, double %0) #0 !dbg !1 {
  %var_with_dbg_value = alloca ptr, align 8
  %var_with_dbg_declare = alloca ptr, align 8
    #dbg_declare(ptr %arg, !5, !DIExpression(DW_OP_LLVM_entry_value, 1), !7)
    #dbg_declare_value(double %0, !5, !DIExpression(), !7)
  %i2 = call token @llvm.coro.id.async(i32 16, i32 16, i32 0, ptr nonnull @coroutineATu)
  %i3 = call ptr @llvm.coro.begin(token %i2, ptr null)
  %i7 = call ptr @llvm.coro.async.resume(), !dbg !7
  %i10 = call { ptr } (i32, ptr, ptr, ...) @llvm.coro.suspend.async.sl_p0s(i32 0, ptr %i7, ptr nonnull @__swift_async_resume_get_context, ptr nonnull @coroutineA.1, ptr %i7, i64 0, i64 0, ptr %arg), !dbg !7
  call void @dont_optimize(ptr %var_with_dbg_value, ptr %var_with_dbg_declare), !dbg !7
  unreachable, !dbg !7
}

define weak_odr hidden ptr @__swift_async_resume_get_context(ptr %arg) !dbg !8 {
  ret ptr %arg, !dbg !9
}

define hidden swifttailcc void @coroutineA.1(ptr %arg, i64 %arg1, i64 %arg2, ptr %arg3) !dbg !10 {
  ret void, !dbg !11
}

declare void @dont_optimize(ptr, ptr)

; Function Attrs: nomerge nounwind
declare ptr @llvm.coro.async.resume() #1

; Function Attrs: nounwind
declare ptr @llvm.coro.begin(token, ptr writeonly) #2

; Function Attrs: nounwind
declare token @llvm.coro.id.async(i32, i32, i32, ptr) #2

; Function Attrs: nomerge nounwind
declare { ptr } @llvm.coro.suspend.async.sl_p0s(i32, ptr, ptr, ...) #1

attributes #0 = { presplitcoroutine }
attributes #1 = { nomerge nounwind }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = distinct !DISubprogram(scope: null, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!2 = distinct !DICompileUnit(language: DW_LANG_Swift, file: !3, isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!3 = !DIFile(filename: "blah", directory: "")
!4 = !{}
!5 = !DILocalVariable(scope: !1, type: !6)
!6 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "Klass")
!7 = !DILocation(line: 0, scope: !1)
!8 = distinct !DISubprogram(scope: null, spFlags: DISPFlagDefinition, unit: !2)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(scope: null, spFlags: DISPFlagDefinition, unit: !2)
!11 = !DILocation(line: 0, scope: !10)
```

Compiler Explorer: https://godbolt.org/z/a5jf1f8fq

Crash:

```sh
opt: /root/llvm-project/llvm/lib/IR/DIExpressionOptimizer.cpp:378: llvm::DIExpression* llvm::DIExpression::foldConstantMath(): Assertion `Result->isValid() && "concatenated expression is not valid"' failed.
```

Backtrace: 

```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -S -passes=module(coro-early),cgscc(coro-split,simplifycfg) <source>
1.	Running pass "cgscc(coro-split,function(simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;no-switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>))" on module "<source>"
2.	While splitting coroutine @coroutineA
 #0 0x0000000005b7e288 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b7e288)
 #1 0x0000000005b7b0b4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b7b0b4)
 #2 0x0000000005b7b22a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007b67d3642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b67d36969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b67d3642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b67d36287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b67d362871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b67d3639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000057db697 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57db697)
#10 0x000000000325aa86 llvm::coro::salvageDebugInfo(llvm::SmallDenseMap<llvm::Argument*, llvm::AllocaInst*, 4u, llvm::DenseMapInfo<llvm::Argument*, void>, llvm::detail::DenseMapPair<llvm::Argument*, llvm::AllocaInst*>>&, llvm::DbgVariableRecord&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x325aa86)
#11 0x0000000003242952 llvm::CoroSplitPass::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3242952)
#12 0x000000000312935e llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::CoroSplitPass, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x312935e)
#13 0x0000000004ed7892 llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ed7892)
#14 0x00000000031280ce llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31280ce)
#15 0x0000000004ed9211 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ed9211)
#16 0x0000000000f184ae llvm::detail::PassModel<llvm::Module, llvm::ModuleToPostOrderCGSCCPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf184ae)
#17 0x00000000058e4131 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58e4131)
#18 0x000000000096bcea llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96bcea)
#19 0x000000000095fd85 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x95fd85)
#20 0x00007b67d3629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x00007b67d3629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x0000000000956a65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x956a65)
Program terminated with signal: SIGSEGV
```

> Note: This is a review assisted with a self-built agent. The reproducer was validated manually. Please let me know if anything is wrong.

**Bug Triggering Analysis:**
The provided test case triggers a crash in `opt` during the `coro-split` pass. The crash occurs in `llvm::DIExpression::foldConstantMath()` because the concatenated expression is not valid.
The root cause is in `salvageDebugInfoImpl` in `CoroFrame.cpp`.
When the debug expression for a `swiftasync` argument already contains `DW_OP_LLVM_entry_value`, `WillUseEntryValue` evaluates to `false` (because of `!Expr->isEntryValue()`).
Since `WillUseEntryValue` is `false`, the code falls through to the block that creates an alloca:
```cpp
  if (StorageAsArg && !WillUseEntryValue && IsSingleLocationExpression) {
    // ... creates alloca ...
    Expr = DIExpression::prepend(Expr, DIExpression::DerefBefore);
  }
```
This prepends `DW_OP_deref` to the expression. The expression was `!DIExpression(DW_OP_LLVM_entry_value, 1)`. Prepending `DW_OP_deref` makes it `!DIExpression(DW_OP_deref, DW_OP_LLVM_entry_value, 1)`.
Then, `Expr = Expr->foldConstantMath();` is called. `foldConstantMath` asserts that the resulting expression is valid. However, `DW_OP_LLVM_entry_value` must appear at the beginning of the expression (or immediately following `DW_OP_LLVM_arg 0`). Since `DW_OP_deref` is now at the beginning, the expression is invalid, and the assertion fails.

**Fix Weakness Analysis:**
The fix introduced the `WillUseEntryValue` boolean to correctly determine when to use `EntryValue` and when to create an alloca. However, it failed to account for the case where the expression *already* is an `EntryValue`.
If it's already an `EntryValue`, `WillUseEntryValue` is false. The fix assumes that if `WillUseEntryValue` is false, it *must* create an alloca to preserve debug info. But creating an alloca for an existing `EntryValue` expression and prepending `DW_OP_deref` creates an invalid DWARF expression, leading to a crash. The logic should probably avoid creating an alloca if the expression is already an `EntryValue`, or handle it differently.

cc @felipepiovezan 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-coroutines

Author: Yunbo Ni (cardigan1008)

<details>
Here is a crash case when reviewing https://github.com/llvm/llvm-project/pull/176766:

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-unknown"

@<!-- -->coroutineATu = global &lt;{ i32, i32 }&gt; &lt;{ i32 trunc (i64 sub (i64 ptrtoint (ptr @<!-- -->coroutineA to i64), i64 ptrtoint (ptr @<!-- -->coroutineATu to i64)) to i32), i32 16 }&gt;, align 8

; Function Attrs: presplitcoroutine
define swifttailcc void @<!-- -->coroutineA(ptr swiftasync %arg, double %0) #<!-- -->0 !dbg !1 {
  %var_with_dbg_value = alloca ptr, align 8
  %var_with_dbg_declare = alloca ptr, align 8
    #dbg_declare(ptr %arg, !5, !DIExpression(DW_OP_LLVM_entry_value, 1), !7)
    #dbg_declare_value(double %0, !5, !DIExpression(), !7)
  %i2 = call token @<!-- -->llvm.coro.id.async(i32 16, i32 16, i32 0, ptr nonnull @<!-- -->coroutineATu)
  %i3 = call ptr @<!-- -->llvm.coro.begin(token %i2, ptr null)
  %i7 = call ptr @<!-- -->llvm.coro.async.resume(), !dbg !7
  %i10 = call { ptr } (i32, ptr, ptr, ...) @<!-- -->llvm.coro.suspend.async.sl_p0s(i32 0, ptr %i7, ptr nonnull @<!-- -->__swift_async_resume_get_context, ptr nonnull @<!-- -->coroutineA.1, ptr %i7, i64 0, i64 0, ptr %arg), !dbg !7
  call void @<!-- -->dont_optimize(ptr %var_with_dbg_value, ptr %var_with_dbg_declare), !dbg !7
  unreachable, !dbg !7
}

define weak_odr hidden ptr @<!-- -->__swift_async_resume_get_context(ptr %arg) !dbg !8 {
  ret ptr %arg, !dbg !9
}

define hidden swifttailcc void @<!-- -->coroutineA.1(ptr %arg, i64 %arg1, i64 %arg2, ptr %arg3) !dbg !10 {
  ret void, !dbg !11
}

declare void @<!-- -->dont_optimize(ptr, ptr)

; Function Attrs: nomerge nounwind
declare ptr @<!-- -->llvm.coro.async.resume() #<!-- -->1

; Function Attrs: nounwind
declare ptr @<!-- -->llvm.coro.begin(token, ptr writeonly) #<!-- -->2

; Function Attrs: nounwind
declare token @<!-- -->llvm.coro.id.async(i32, i32, i32, ptr) #<!-- -->2

; Function Attrs: nomerge nounwind
declare { ptr } @<!-- -->llvm.coro.suspend.async.sl_p0s(i32, ptr, ptr, ...) #<!-- -->1

attributes #<!-- -->0 = { presplitcoroutine }
attributes #<!-- -->1 = { nomerge nounwind }
attributes #<!-- -->2 = { nounwind }

!llvm.module.flags = !{!0}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = distinct !DISubprogram(scope: null, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!2 = distinct !DICompileUnit(language: DW_LANG_Swift, file: !3, isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)
!3 = !DIFile(filename: "blah", directory: "")
!4 = !{}
!5 = !DILocalVariable(scope: !1, type: !6)
!6 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "Klass")
!7 = !DILocation(line: 0, scope: !1)
!8 = distinct !DISubprogram(scope: null, spFlags: DISPFlagDefinition, unit: !2)
!9 = !DILocation(line: 0, scope: !8)
!10 = distinct !DISubprogram(scope: null, spFlags: DISPFlagDefinition, unit: !2)
!11 = !DILocation(line: 0, scope: !10)
```

Compiler Explorer: https://godbolt.org/z/a5jf1f8fq

Crash:

```sh
opt: /root/llvm-project/llvm/lib/IR/DIExpressionOptimizer.cpp:378: llvm::DIExpression* llvm::DIExpression::foldConstantMath(): Assertion `Result-&gt;isValid() &amp;&amp; "concatenated expression is not valid"' failed.
```

Backtrace: 

```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -S -passes=module(coro-early),cgscc(coro-split,simplifycfg) &lt;source&gt;
1.	Running pass "cgscc(coro-split,function(simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;no-switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;))" on module "&lt;source&gt;"
2.	While splitting coroutine @<!-- -->coroutineA
 #<!-- -->0 0x0000000005b7e288 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b7e288)
 #<!-- -->1 0x0000000005b7b0b4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b7b0b4)
 #<!-- -->2 0x0000000005b7b22a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007b67d3642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b67d36969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b67d3642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b67d36287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b67d362871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b67d3639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000057db697 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57db697)
#<!-- -->10 0x000000000325aa86 llvm::coro::salvageDebugInfo(llvm::SmallDenseMap&lt;llvm::Argument*, llvm::AllocaInst*, 4u, llvm::DenseMapInfo&lt;llvm::Argument*, void&gt;, llvm::detail::DenseMapPair&lt;llvm::Argument*, llvm::AllocaInst*&gt;&gt;&amp;, llvm::DbgVariableRecord&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x325aa86)
#<!-- -->11 0x0000000003242952 llvm::CoroSplitPass::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3242952)
#<!-- -->12 0x000000000312935e llvm::detail::PassModel&lt;llvm::LazyCallGraph::SCC, llvm::CoroSplitPass, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x312935e)
#<!-- -->13 0x0000000004ed7892 llvm::PassManager&lt;llvm::LazyCallGraph::SCC, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ed7892)
#<!-- -->14 0x00000000031280ce llvm::detail::PassModel&lt;llvm::LazyCallGraph::SCC, llvm::PassManager&lt;llvm::LazyCallGraph::SCC, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31280ce)
#<!-- -->15 0x0000000004ed9211 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ed9211)
#<!-- -->16 0x0000000000f184ae llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToPostOrderCGSCCPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf184ae)
#<!-- -->17 0x00000000058e4131 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58e4131)
#<!-- -->18 0x000000000096bcea llvm::runPassPipeline(llvm::StringRef, llvm::Module&amp;, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef&lt;llvm::PassPlugin&gt;, llvm::ArrayRef&lt;std::function&lt;void (llvm::PassBuilder&amp;)&gt;&gt;, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96bcea)
#<!-- -->19 0x000000000095fd85 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x95fd85)
#<!-- -->20 0x00007b67d3629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->21 0x00007b67d3629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->22 0x0000000000956a65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x956a65)
Program terminated with signal: SIGSEGV
```

&gt; Note: This is a review assisted with a self-built agent. The reproducer was validated manually. Please let me know if anything is wrong.

**Bug Triggering Analysis:**
The provided test case triggers a crash in `opt` during the `coro-split` pass. The crash occurs in `llvm::DIExpression::foldConstantMath()` because the concatenated expression is not valid.
The root cause is in `salvageDebugInfoImpl` in `CoroFrame.cpp`.
When the debug expression for a `swiftasync` argument already contains `DW_OP_LLVM_entry_value`, `WillUseEntryValue` evaluates to `false` (because of `!Expr-&gt;isEntryValue()`).
Since `WillUseEntryValue` is `false`, the code falls through to the block that creates an alloca:
```cpp
  if (StorageAsArg &amp;&amp; !WillUseEntryValue &amp;&amp; IsSingleLocationExpression) {
    // ... creates alloca ...
    Expr = DIExpression::prepend(Expr, DIExpression::DerefBefore);
  }
```
This prepends `DW_OP_deref` to the expression. The expression was `!DIExpression(DW_OP_LLVM_entry_value, 1)`. Prepending `DW_OP_deref` makes it `!DIExpression(DW_OP_deref, DW_OP_LLVM_entry_value, 1)`.
Then, `Expr = Expr-&gt;foldConstantMath();` is called. `foldConstantMath` asserts that the resulting expression is valid. However, `DW_OP_LLVM_entry_value` must appear at the beginning of the expression (or immediately following `DW_OP_LLVM_arg 0`). Since `DW_OP_deref` is now at the beginning, the expression is invalid, and the assertion fails.

**Fix Weakness Analysis:**
The fix introduced the `WillUseEntryValue` boolean to correctly determine when to use `EntryValue` and when to create an alloca. However, it failed to account for the case where the expression *already* is an `EntryValue`.
If it's already an `EntryValue`, `WillUseEntryValue` is false. The fix assumes that if `WillUseEntryValue` is false, it *must* create an alloca to preserve debug info. But creating an alloca for an existing `EntryValue` expression and prepending `DW_OP_deref` creates an invalid DWARF expression, leading to a crash. The logic should probably avoid creating an alloca if the expression is already an `EntryValue`, or handle it differently.

cc @<!-- -->felipepiovezan 
</details>


---

