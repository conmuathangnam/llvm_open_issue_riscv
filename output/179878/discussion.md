# [opt] -passes=rewrite-statepoints-for-gc causes Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"'

**Author:** fraglantia
**URL:** https://github.com/llvm/llvm-project/issues/179878
**Status:** Open
**Labels:** backend:X86, crash

## Body

Reproducer: https://godbolt.org/z/hT5ev9d7a

```
opt: /root/llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = llvm::InvokeInst; From = llvm::CallBase]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=module(rewrite-statepoints-for-gc) <source>
1.	Running pass "rewrite-statepoints-for-gc" on module "<source>"
 #0 0x0000000005aefe98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5aefe98)
 #1 0x0000000005aecd74 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000078b184e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000078b184e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000078b184e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000078b184e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000078b184e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000078b184e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000490ac5c makeStatepointExplicitImpl(llvm::CallBase*, llvm::SmallVectorImpl<llvm::Value*> const&, llvm::SmallVectorImpl<llvm::Value*> const&, (anonymous namespace)::PartiallyConstructedSafepointRecord&, std::vector<(anonymous namespace)::DeferredReplacement, std::allocator<(anonymous namespace)::DeferredReplacement>>&, llvm::MapVector<llvm::Value*, llvm::Value*, llvm::DenseMap<llvm::Value*, unsigned int, llvm::DenseMapInfo<llvm::Value*, void>, llvm::detail::DenseMapPair<llvm::Value*, unsigned int>>, llvm::SmallVector<std::pair<llvm::Value*, llvm::Value*>, 0u>> const&, llvm::GCStrategy*) (.isra.0) RewriteStatepointsForGC.cpp:0:0
 #9 0x000000000491a3a4 insertParsePoints(llvm::Function&, llvm::DominatorTree&, llvm::TargetTransformInfo&, llvm::SmallVectorImpl<llvm::CallBase*>&, llvm::MapVector<llvm::Value*, llvm::Value*, llvm::DenseMap<llvm::Value*, unsigned int, llvm::DenseMapInfo<llvm::Value*, void>, llvm::detail::DenseMapPair<llvm::Value*, unsigned int>>, llvm::SmallVector<std::pair<llvm::Value*, llvm::Value*>, 0u>>&, llvm::MapVector<llvm::Value*, bool, llvm::DenseMap<llvm::Value*, unsigned int, llvm::DenseMapInfo<llvm::Value*, void>, llvm::detail::DenseMapPair<llvm::Value*, unsigned int>>, llvm::SmallVector<std::pair<llvm::Value*, bool>, 0u>>&) RewriteStatepointsForGC.cpp:0:0
#10 0x000000000491b953 llvm::RewriteStatepointsForGC::runOnFunction(llvm::Function&, llvm::DominatorTree&, llvm::TargetTransformInfo&, llvm::TargetLibraryInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x491b953)
#11 0x000000000491c952 llvm::RewriteStatepointsForGC::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x491c952)
#12 0x00000000030585de llvm::detail::PassModel<llvm::Module, llvm::RewriteStatepointsForGC, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x30585de)
#13 0x000000000585e411 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x585e411)
#14 0x0000000000971fba llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971fba)
#15 0x0000000000966068 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x966068)
#16 0x000078b184e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#17 0x000078b184e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#18 0x000000000095cce5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x95cce5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Steve Gustaman (fraglantia)

<details>
Reproducer: https://godbolt.org/z/hT5ev9d7a

```
opt: /root/llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = llvm::InvokeInst; From = llvm::CallBase]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S --passes=module(rewrite-statepoints-for-gc) &lt;source&gt;
1.	Running pass "rewrite-statepoints-for-gc" on module "&lt;source&gt;"
 #<!-- -->0 0x0000000005aefe98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5aefe98)
 #<!-- -->1 0x0000000005aecd74 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000078b184e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000078b184e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000078b184e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000078b184e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000078b184e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000078b184e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x000000000490ac5c makeStatepointExplicitImpl(llvm::CallBase*, llvm::SmallVectorImpl&lt;llvm::Value*&gt; const&amp;, llvm::SmallVectorImpl&lt;llvm::Value*&gt; const&amp;, (anonymous namespace)::PartiallyConstructedSafepointRecord&amp;, std::vector&lt;(anonymous namespace)::DeferredReplacement, std::allocator&lt;(anonymous namespace)::DeferredReplacement&gt;&gt;&amp;, llvm::MapVector&lt;llvm::Value*, llvm::Value*, llvm::DenseMap&lt;llvm::Value*, unsigned int, llvm::DenseMapInfo&lt;llvm::Value*, void&gt;, llvm::detail::DenseMapPair&lt;llvm::Value*, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::Value*, llvm::Value*&gt;, 0u&gt;&gt; const&amp;, llvm::GCStrategy*) (.isra.0) RewriteStatepointsForGC.cpp:0:0
 #<!-- -->9 0x000000000491a3a4 insertParsePoints(llvm::Function&amp;, llvm::DominatorTree&amp;, llvm::TargetTransformInfo&amp;, llvm::SmallVectorImpl&lt;llvm::CallBase*&gt;&amp;, llvm::MapVector&lt;llvm::Value*, llvm::Value*, llvm::DenseMap&lt;llvm::Value*, unsigned int, llvm::DenseMapInfo&lt;llvm::Value*, void&gt;, llvm::detail::DenseMapPair&lt;llvm::Value*, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::Value*, llvm::Value*&gt;, 0u&gt;&gt;&amp;, llvm::MapVector&lt;llvm::Value*, bool, llvm::DenseMap&lt;llvm::Value*, unsigned int, llvm::DenseMapInfo&lt;llvm::Value*, void&gt;, llvm::detail::DenseMapPair&lt;llvm::Value*, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::Value*, bool&gt;, 0u&gt;&gt;&amp;) RewriteStatepointsForGC.cpp:0:0
#<!-- -->10 0x000000000491b953 llvm::RewriteStatepointsForGC::runOnFunction(llvm::Function&amp;, llvm::DominatorTree&amp;, llvm::TargetTransformInfo&amp;, llvm::TargetLibraryInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x491b953)
#<!-- -->11 0x000000000491c952 llvm::RewriteStatepointsForGC::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x491c952)
#<!-- -->12 0x00000000030585de llvm::detail::PassModel&lt;llvm::Module, llvm::RewriteStatepointsForGC, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x30585de)
#<!-- -->13 0x000000000585e411 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x585e411)
#<!-- -->14 0x0000000000971fba llvm::runPassPipeline(llvm::StringRef, llvm::Module&amp;, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef&lt;llvm::PassPlugin&gt;, llvm::ArrayRef&lt;std::function&lt;void (llvm::PassBuilder&amp;)&gt;&gt;, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971fba)
#<!-- -->15 0x0000000000966068 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x966068)
#<!-- -->16 0x000078b184e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->17 0x000078b184e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->18 0x000000000095cce5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x95cce5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - SakshamSinghal20

I would like to work on this could you please assign this to me?

---

