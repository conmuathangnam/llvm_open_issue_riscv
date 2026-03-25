# [opt] -passes=scalarizer causes Assertion `idx < size()'

**Author:** fraglantia
**URL:** https://github.com/llvm/llvm-project/issues/179880
**Status:** Closed
**Labels:** crash, llvm:transforms
**Closed Date:** 2026-02-21T17:55:32Z

## Body

Reproducer: https://godbolt.org/z/MhnqEWPs9

```
opt: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:297: T& llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::operator[](llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type) [with T = llvm::Value*; <template-parameter-1-2> = void; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::reference = llvm::Value*&; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type = long unsigned int]: Assertion `idx < size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=module(cgscc(function(scalarizer))) <source>
1.	Running pass "cgscc(function(scalarizer))" on module "<source>"
2.	Running pass "scalarizer" on function "f13"
 #0 0x0000000005aefe98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5aefe98)
 #1 0x0000000005aecd74 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000704cdba42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000704cdba969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000704cdba42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000704cdba287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000704cdba2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000704cdba39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004950030 (anonymous namespace)::Scatterer::operator[](unsigned int) Scalarizer.cpp:0:0
 #9 0x00000000049543e8 (anonymous namespace)::ScalarizerVisitor::visitExtractElementInst(llvm::ExtractElementInst&) Scalarizer.cpp:0:0
#10 0x0000000004957ea2 (anonymous namespace)::ScalarizerVisitor::visit(llvm::Function&) (.constprop.0) Scalarizer.cpp:0:0
#11 0x00000000049597ff llvm::ScalarizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x49597ff)
#12 0x000000000305920e llvm::detail::PassModel<llvm::Function, llvm::ScalarizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x305920e)
#13 0x00000000058604a1 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58604a1)
#14 0x0000000000f0ce4e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf0ce4e)
#15 0x0000000004e6c3c2 llvm::CGSCCToFunctionPassAdaptor::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4e6c3c2)
#16 0x0000000000f0bbae llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::CGSCCToFunctionPassAdaptor, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf0bbae)
#17 0x0000000004e64bf2 llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4e64bf2)
#18 0x0000000003058f2e llvm::detail::PassModel<llvm::LazyCallGraph::SCC, llvm::PassManager<llvm::LazyCallGraph::SCC, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&>::run(llvm::LazyCallGraph::SCC&, llvm::AnalysisManager<llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&>&, llvm::LazyCallGraph&, llvm::CGSCCUpdateResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3058f2e)
#19 0x0000000004e66571 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4e66571)
#20 0x0000000000f0bb8e llvm::detail::PassModel<llvm::Module, llvm::ModuleToPostOrderCGSCCPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf0bb8e)
#21 0x000000000585e411 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x585e411)
#22 0x0000000000971fba llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971fba)
#23 0x0000000000966068 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x966068)
#24 0x0000704cdba29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x0000704cdba29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x000000000095cce5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x95cce5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

Possible duplicate of #126938

## Comments

### Comment 1 - bala-bhargav

@fraglantia 
can i work on it ??

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Steve Gustaman (fraglantia)

<details>
Reproducer: https://godbolt.org/z/MhnqEWPs9

```
opt: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:297: T&amp; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::operator[](llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type) [with T = llvm::Value*; &lt;template-parameter-1-2&gt; = void; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::reference = llvm::Value*&amp;; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type = long unsigned int]: Assertion `idx &lt; size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=module(cgscc(function(scalarizer))) &lt;source&gt;
1.	Running pass "cgscc(function(scalarizer))" on module "&lt;source&gt;"
2.	Running pass "scalarizer" on function "f13"
 #<!-- -->0 0x0000000005aefe98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5aefe98)
 #<!-- -->1 0x0000000005aecd74 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000704cdba42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000704cdba969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000704cdba42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000704cdba287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000704cdba2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000704cdba39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000004950030 (anonymous namespace)::Scatterer::operator[](unsigned int) Scalarizer.cpp:0:0
 #<!-- -->9 0x00000000049543e8 (anonymous namespace)::ScalarizerVisitor::visitExtractElementInst(llvm::ExtractElementInst&amp;) Scalarizer.cpp:0:0
#<!-- -->10 0x0000000004957ea2 (anonymous namespace)::ScalarizerVisitor::visit(llvm::Function&amp;) (.constprop.0) Scalarizer.cpp:0:0
#<!-- -->11 0x00000000049597ff llvm::ScalarizerPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x49597ff)
#<!-- -->12 0x000000000305920e llvm::detail::PassModel&lt;llvm::Function, llvm::ScalarizerPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x305920e)
#<!-- -->13 0x00000000058604a1 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58604a1)
#<!-- -->14 0x0000000000f0ce4e llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf0ce4e)
#<!-- -->15 0x0000000004e6c3c2 llvm::CGSCCToFunctionPassAdaptor::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4e6c3c2)
#<!-- -->16 0x0000000000f0bbae llvm::detail::PassModel&lt;llvm::LazyCallGraph::SCC, llvm::CGSCCToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf0bbae)
#<!-- -->17 0x0000000004e64bf2 llvm::PassManager&lt;llvm::LazyCallGraph::SCC, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4e64bf2)
#<!-- -->18 0x0000000003058f2e llvm::detail::PassModel&lt;llvm::LazyCallGraph::SCC, llvm::PassManager&lt;llvm::LazyCallGraph::SCC, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;&gt;::run(llvm::LazyCallGraph::SCC&amp;, llvm::AnalysisManager&lt;llvm::LazyCallGraph::SCC, llvm::LazyCallGraph&amp;&gt;&amp;, llvm::LazyCallGraph&amp;, llvm::CGSCCUpdateResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3058f2e)
#<!-- -->19 0x0000000004e66571 llvm::ModuleToPostOrderCGSCCPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4e66571)
#<!-- -->20 0x0000000000f0bb8e llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToPostOrderCGSCCPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf0bb8e)
#<!-- -->21 0x000000000585e411 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x585e411)
#<!-- -->22 0x0000000000971fba llvm::runPassPipeline(llvm::StringRef, llvm::Module&amp;, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef&lt;llvm::PassPlugin&gt;, llvm::ArrayRef&lt;std::function&lt;void (llvm::PassBuilder&amp;)&gt;&gt;, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971fba)
#<!-- -->23 0x0000000000966068 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x966068)
#<!-- -->24 0x0000704cdba29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x0000704cdba29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x000000000095cce5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x95cce5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

Possible duplicate of #<!-- -->126938
</details>


---

