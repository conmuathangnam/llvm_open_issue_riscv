# [analyzer] crash when passing null literal to _Atomic void* parameter

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/187925
**Status:** Open
**Labels:** clang:static analyzer, crash, regression:17

## Body

Reproducer:
https://godbolt.org/z/PG8vj8Mn3
```c
void foo(void* _Atomic);

void bar(int) {
  foo(0);
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/StaticAnalyzer/Core/PathSensitive/SValBuilder.h:349: clang::ento::loc::ConcreteInt clang::ento::SValBuilder::makeNullWithType(clang::QualType): Assertion `(type->isPointerType() || type->isObjCObjectPointerType() || type->isBlockPointerType() || type->isNullPtrType() || type->isReferenceType()) && "makeNullWithType must use pointer type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c --analyze <source>
1.	<eof> parser at end of file
2.	While analyzing stack: 
	#0 Calling bar
3.	<source>:4:7: Error evaluating statement
4.	<source>:4:7: Error evaluating statement
 #0 0x0000000004450868 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4450868)
 #1 0x000000000444d694 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444d694)
 #2 0x000000000444dcb4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444dcb4)
 #3 0x000000000438d058 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x00007dc004c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007dc004c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007dc004c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007dc004c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007dc004c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007dc004c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x00000000066ef319 clang::ento::SValBuilder::makeNullWithType(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ef319)
#11 0x0000000006b249e5 clang::ento::ExprEngine::VisitCast(clang::CastExpr const*, clang::Expr const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b249e5)
#12 0x0000000006b12218 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b12218)
#13 0x0000000006b12dfd clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b12dfd)
#14 0x0000000006acedd7 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6acedd7)
#15 0x0000000006acf141 clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6acf141)
#16 0x00000000066820d5 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet<clang::Decl const*, llvm::DenseMapInfo<clang::Decl const*, void>>*) AnalysisConsumer.cpp:0:0
#17 0x0000000006683ead (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&) AnalysisConsumer.cpp:0:0
#18 0x0000000006ca212c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca212c)
#19 0x0000000005256835 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5256835)
#20 0x00000000051d432e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d432e)
#21 0x000000000535281d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x535281d)
#22 0x0000000000de37e2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde37e2)
#23 0x0000000000dda11a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#24 0x0000000000dda29d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#25 0x0000000004fa4cf9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x000000000438d494 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438d494)
#27 0x0000000004fa5326 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x0000000004f64402 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f64402)
#29 0x0000000004f653ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f653ae)
#30 0x0000000004f6cf65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6cf65)
#31 0x0000000000ddfb44 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddfb44)
#32 0x0000000000c97b3a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97b3a)
#33 0x00007dc004c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x00007dc004c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000dd9bb5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9bb5)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/PG8vj8Mn3
```cpp
void foo(void* _Atomic);

void bar(int) {
  foo(0);
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/StaticAnalyzer/Core/PathSensitive/SValBuilder.h:349: clang::ento::loc::ConcreteInt clang::ento::SValBuilder::makeNullWithType(clang::QualType): Assertion `(type-&gt;isPointerType() || type-&gt;isObjCObjectPointerType() || type-&gt;isBlockPointerType() || type-&gt;isNullPtrType() || type-&gt;isReferenceType()) &amp;&amp; "makeNullWithType must use pointer type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c --analyze &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling bar
3.	&lt;source&gt;:4:7: Error evaluating statement
4.	&lt;source&gt;:4:7: Error evaluating statement
 #<!-- -->0 0x0000000004450868 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4450868)
 #<!-- -->1 0x000000000444d694 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444d694)
 #<!-- -->2 0x000000000444dcb4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444dcb4)
 #<!-- -->3 0x000000000438d058 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x00007dc004c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007dc004c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007dc004c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007dc004c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007dc004c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007dc004c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x00000000066ef319 clang::ento::SValBuilder::makeNullWithType(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ef319)
#<!-- -->11 0x0000000006b249e5 clang::ento::ExprEngine::VisitCast(clang::CastExpr const*, clang::Expr const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b249e5)
#<!-- -->12 0x0000000006b12218 clang::ento::ExprEngine::Visit(clang::Stmt const*, clang::ento::ExplodedNode*, clang::ento::ExplodedNodeSet&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b12218)
#<!-- -->13 0x0000000006b12dfd clang::ento::ExprEngine::ProcessStmt(clang::Stmt const*, clang::ento::ExplodedNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b12dfd)
#<!-- -->14 0x0000000006acedd7 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6acedd7)
#<!-- -->15 0x0000000006acf141 clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6acf141)
#<!-- -->16 0x00000000066820d5 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet&lt;clang::Decl const*, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;*) AnalysisConsumer.cpp:0:0
#<!-- -->17 0x0000000006683ead (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&amp;) AnalysisConsumer.cpp:0:0
#<!-- -->18 0x0000000006ca212c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca212c)
#<!-- -->19 0x0000000005256835 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5256835)
#<!-- -->20 0x00000000051d432e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d432e)
#<!-- -->21 0x000000000535281d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x535281d)
#<!-- -->22 0x0000000000de37e2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde37e2)
#<!-- -->23 0x0000000000dda11a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->24 0x0000000000dda29d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->25 0x0000000004fa4cf9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->26 0x000000000438d494 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438d494)
#<!-- -->27 0x0000000004fa5326 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->28 0x0000000004f64402 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f64402)
#<!-- -->29 0x0000000004f653ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f653ae)
#<!-- -->30 0x0000000004f6cf65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6cf65)
#<!-- -->31 0x0000000000ddfb44 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddfb44)
#<!-- -->32 0x0000000000c97b3a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97b3a)
#<!-- -->33 0x00007dc004c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->34 0x00007dc004c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->35 0x0000000000dd9bb5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9bb5)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-17:
https://godbolt.org/z/GK6n56rs8

---

