# [ArgPromotion] Crash when `DISubprogram` has null type and `DW_CC_nocall` is added

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/186557

## Body

Here is a crash case when reviewing https://github.com/llvm/llvm-project/pull/178973:

```llvm
%struct.pair = type { i32, i32 }

define internal void @test(ptr %X) !dbg !4 {
  %1 = load ptr, ptr %X, align 8
  %2 = load i32, ptr %1, align 8
  call void @sink(i32 %2)
  ret void
}

declare void @sink(i32)

define void @caller(ptr %Y, ptr %P) {
  call void @test(ptr %Y), !dbg !3
  ret void
}

!llvm.module.flags = !{!0}
!llvm.dbg.cu = !{!1}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus, file: !2, producer: "clang version 3.5.0 ", isOptimized: false, runtimeVersion: 0, emissionKind: LineTablesOnly)
!2 = !DIFile(filename: "test.c", directory: "")
!3 = !DILocation(line: 8, scope: !4)
!4 = distinct !DISubprogram(name: "test", scope: null, file: !2, line: 3, type: null, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !1)
```

Backtrace:

```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=argpromotion <source>
1.	Running pass "cgscc(argpromotion)" on module "<source>"
 #0 0x0000000005b7e288 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b7e288)
 #1 0x0000000005b7b0b4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b7b0b4)
 #2 0x0000000005b7b22a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000079de03242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000003276d12 llvm::DISubroutineType::cloneImpl() const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3276d12)
 #5 0x000000000327b193 doPromotion(llvm::Function*, llvm::AnalysisManager<llvm::Function>&, llvm::DenseMap<llvm::Argument*, llvm::SmallVector<std::pair<long, (anonymous namespace)::ArgPart>, 4u>, llvm::DenseMapInfo<llvm::Argument*, void>, llvm::detail::DenseMapPair<llvm::Argument*, llvm::SmallVector<std::pair<long, (anonymous namespace)::ArgPart>, 4u>>> const&) ArgumentPromotion.cpp:0:0
 #6 0x000000000327c781 promoteArguments(llvm::Function*, llvm::AnalysisManager<llvm::Function>&, unsigned int, bool) ArgumentPromotion.cpp:0:0
 #7 0x000000000327e885 llvm::ArgumentPromotionPass::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x327e885)
 #8 0x00000000031293fe llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::ArgumentPromotionPass, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31293fe)
 #9 0x0000000004ed7892 llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ed7892)
#10 0x00000000031280ce llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31280ce)
#11 0x0000000004ed9211 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ed9211)
#12 0x0000000000f184ae llvm::detail::PassModel<llvm::Module, llvm::ModuleToPostOrderCGSCCPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf184ae)
#13 0x00000000058e4131 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58e4131)
#14 0x000000000096bcea llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96bcea)
#15 0x000000000095fd85 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x95fd85)
#16 0x000079de03229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#17 0x000079de03229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#18 0x0000000000956a65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x956a65)
Program terminated with signal: SIGSEGV
```

Compiler Explorer: https://godbolt.org/z/P693GhzoY

> Note: This is a review assisted with a self-built agent. The reproducer was validated manually. Please let me know if anything is wrong.

**Bug Triggering Analysis:**
The provided test case triggers a crash in the `ArgumentPromotion` pass. The crash occurs because the `DISubprogram` metadata attached to the `@test` function has a `type: null` field (or no `type` field). When `ArgumentPromotion` promotes the arguments of `@test`, it attempts to update the calling convention of the function's debug info by calling `SP->getType()->cloneWithCC(llvm::dwarf::DW_CC_nocall)`. Since `SP->getType()` returns `nullptr` when the `type` field is null, this results in a null pointer dereference and a segmentation fault.

**Fix Weakness Analysis:**
The weakness in the fix is the assumption that `SP->getType()` always returns a valid `DISubroutineType` pointer. In LLVM IR, it is perfectly valid for a `DISubprogram` to have a null `type` field (e.g., in degraded debug info or specific language frontends). The verifier accepts `type: null` for `DISubprogram`. The fix blindly dereferences the result of `SP->getType()` without checking if it is null, leading to the crash. This same bug was also copied from `DeadArgumentElimination`, which has the exact same vulnerability.

cc @yonghong-song 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-debuginfo

Author: Yunbo Ni (cardigan1008)

<details>
Here is a crash case when reviewing https://github.com/llvm/llvm-project/pull/178973:

```llvm
%struct.pair = type { i32, i32 }

define internal void @<!-- -->test(ptr %X) !dbg !4 {
  %1 = load ptr, ptr %X, align 8
  %2 = load i32, ptr %1, align 8
  call void @<!-- -->sink(i32 %2)
  ret void
}

declare void @<!-- -->sink(i32)

define void @<!-- -->caller(ptr %Y, ptr %P) {
  call void @<!-- -->test(ptr %Y), !dbg !3
  ret void
}

!llvm.module.flags = !{!0}
!llvm.dbg.cu = !{!1}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus, file: !2, producer: "clang version 3.5.0 ", isOptimized: false, runtimeVersion: 0, emissionKind: LineTablesOnly)
!2 = !DIFile(filename: "test.c", directory: "")
!3 = !DILocation(line: 8, scope: !4)
!4 = distinct !DISubprogram(name: "test", scope: null, file: !2, line: 3, type: null, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !1)
```

Backtrace:

```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=argpromotion &lt;source&gt;
1.	Running pass "cgscc(argpromotion)" on module "&lt;source&gt;"
 #<!-- -->0 0x0000000005b7e288 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b7e288)
 #<!-- -->1 0x0000000005b7b0b4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b7b0b4)
 #<!-- -->2 0x0000000005b7b22a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000079de03242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000003276d12 llvm::DISubroutineType::cloneImpl() const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3276d12)
 #<!-- -->5 0x000000000327b193 doPromotion(llvm::Function*, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, llvm::DenseMap&lt;llvm::Argument*, llvm::SmallVector&lt;std::pair&lt;long, (anonymous namespace)::ArgPart&gt;, 4u&gt;, llvm::DenseMapInfo&lt;llvm::Argument*, void&gt;, llvm::detail::DenseMapPair&lt;llvm::Argument*, llvm::SmallVector&lt;std::pair&lt;long, (anonymous namespace)::ArgPart&gt;, 4u&gt;&gt;&gt; const&amp;) ArgumentPromotion.cpp:0:0
 #<!-- -->6 0x000000000327c781 promoteArguments(llvm::Function*, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, unsigned int, bool) ArgumentPromotion.cpp:0:0
 #<!-- -->7 0x000000000327e885 llvm::ArgumentPromotionPass::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x327e885)
 #<!-- -->8 0x00000000031293fe llvm::detail::PassModel&lt;llvm::LazyCallGraph::SCC, llvm::ArgumentPromotionPass, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31293fe)
 #<!-- -->9 0x0000000004ed7892 llvm::PassManager&lt;llvm::LazyCallGraph::SCC, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ed7892)
#<!-- -->10 0x00000000031280ce llvm::detail::PassModel&lt;llvm::LazyCallGraph::SCC, llvm::PassManager&lt;llvm::LazyCallGraph::SCC, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31280ce)
#<!-- -->11 0x0000000004ed9211 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4ed9211)
#<!-- -->12 0x0000000000f184ae llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToPostOrderCGSCCPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf184ae)
#<!-- -->13 0x00000000058e4131 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58e4131)
#<!-- -->14 0x000000000096bcea llvm::runPassPipeline(llvm::StringRef, llvm::Module&amp;, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef&lt;llvm::PassPlugin&gt;, llvm::ArrayRef&lt;std::function&lt;void (llvm::PassBuilder&amp;)&gt;&gt;, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96bcea)
#<!-- -->15 0x000000000095fd85 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x95fd85)
#<!-- -->16 0x000079de03229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->17 0x000079de03229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->18 0x0000000000956a65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x956a65)
Program terminated with signal: SIGSEGV
```

Compiler Explorer: https://godbolt.org/z/P693GhzoY

&gt; Note: This is a review assisted with a self-built agent. The reproducer was validated manually. Please let me know if anything is wrong.

**Bug Triggering Analysis:**
The provided test case triggers a crash in the `ArgumentPromotion` pass. The crash occurs because the `DISubprogram` metadata attached to the `@<!-- -->test` function has a `type: null` field (or no `type` field). When `ArgumentPromotion` promotes the arguments of `@<!-- -->test`, it attempts to update the calling convention of the function's debug info by calling `SP-&gt;getType()-&gt;cloneWithCC(llvm::dwarf::DW_CC_nocall)`. Since `SP-&gt;getType()` returns `nullptr` when the `type` field is null, this results in a null pointer dereference and a segmentation fault.

**Fix Weakness Analysis:**
The weakness in the fix is the assumption that `SP-&gt;getType()` always returns a valid `DISubroutineType` pointer. In LLVM IR, it is perfectly valid for a `DISubprogram` to have a null `type` field (e.g., in degraded debug info or specific language frontends). The verifier accepts `type: null` for `DISubprogram`. The fix blindly dereferences the result of `SP-&gt;getType()` without checking if it is null, leading to the crash. This same bug was also copied from `DeadArgumentElimination`, which has the exact same vulnerability.

cc @<!-- -->yonghong-song 
</details>


---

### Comment 2 - Michael137

Same question/doubt as i had in: https://github.com/llvm/llvm-project/pull/184299

What produces such type-less subprograms other than hand-crafted IR? It sounds like we should consider disallowing it since we already assume non-nullness in various places

---

### Comment 3 - cardigan1008

Hi @Michael137 , unlike https://github.com/llvm/llvm-project/issues/184003, this is a mutated version of test from https://github.com/llvm/llvm-project/pull/178973. Feel free to close it if you consider it unnecessary to fix, and I'll adjust to avoid submitting such cases in the future. 

---

### Comment 4 - yonghong-song

Yes, the debuginfo is incomplete. We should fix the debuginfo instead.

---

