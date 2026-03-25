# [analyzer] Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/159341
**Status:** Open
**Labels:** clang:static analyzer, crash

## Body

Reproducer:
https://godbolt.org/z/9Y14YbbTh
```cpp
class A {
public:
  int x;

  A() {}
  ~A() {}

};

void test() {
  const int &x = A().*&A::x;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::ento::TypedValueRegion; From = const clang::ento::MemRegion]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++03 --analyze <source>
1.	<eof> parser at end of file
2.	While analyzing stack: 
	#0 Calling test()
 #0 0x0000000004187508 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4187508)
 #1 0x0000000004184934 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4184934)
 #2 0x00000000040c8fc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000756484c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000756484c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000756484c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000756484c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000756484c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000756484c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000067048b5 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67048b5)
#10 0x0000000006704985 clang::ento::ExprEngine::ProcessImplicitDtor(clang::CFGImplicitDtor, clang::ento::ExplodedNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6704985)
#11 0x00000000066b38cb clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b38cb)
#12 0x00000000066b3fb8 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3fb8)
#13 0x00000000066b4321 clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr<clang::ento::ProgramState const>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b4321)
#14 0x000000000627c007 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet<clang::Decl const*, llvm::DenseMapInfo<clang::Decl const*, void>>*) AnalysisConsumer.cpp:0:0
#15 0x000000000627da7f (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&) AnalysisConsumer.cpp:0:0
#16 0x000000000683512c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x683512c)
#17 0x0000000004dac3f5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dac3f5)
#18 0x0000000004d2838e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2838e)
#19 0x0000000004ea17fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea17fd)
#20 0x0000000000db6300 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb6300)
#21 0x0000000000dacf1a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#22 0x0000000004b2a0d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#23 0x00000000040c9464 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40c9464)
#24 0x0000000004b2a6ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#25 0x0000000004aebb32 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aebb32)
#26 0x0000000004aecade clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecade)
#27 0x0000000004af4205 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af4205)
#28 0x0000000000db277a clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb277a)
#29 0x0000000000c64464 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc64464)
#30 0x0000756484c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#31 0x0000756484c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#32 0x0000000000dac9c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac9c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/9Y14YbbTh
```cpp
class A {
public:
  int x;

  A() {}
  ~A() {}

};

void test() {
  const int &amp;x = A().*&amp;A::x;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::ento::TypedValueRegion; From = const clang::ento::MemRegion]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++03 --analyze &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling test()
 #<!-- -->0 0x0000000004187508 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4187508)
 #<!-- -->1 0x0000000004184934 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4184934)
 #<!-- -->2 0x00000000040c8fc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000756484c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000756484c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000756484c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000756484c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000756484c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000756484c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000067048b5 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67048b5)
#<!-- -->10 0x0000000006704985 clang::ento::ExprEngine::ProcessImplicitDtor(clang::CFGImplicitDtor, clang::ento::ExplodedNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6704985)
#<!-- -->11 0x00000000066b38cb clang::ento::CoreEngine::HandlePostStmt(clang::CFGBlock const*, unsigned int, clang::ento::ExplodedNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b38cb)
#<!-- -->12 0x00000000066b3fb8 clang::ento::CoreEngine::dispatchWorkItem(clang::ento::ExplodedNode*, clang::ProgramPoint, clang::ento::WorkListUnit const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3fb8)
#<!-- -->13 0x00000000066b4321 clang::ento::CoreEngine::ExecuteWorkList(clang::LocationContext const*, unsigned int, llvm::IntrusiveRefCntPtr&lt;clang::ento::ProgramState const&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b4321)
#<!-- -->14 0x000000000627c007 (anonymous namespace)::AnalysisConsumer::HandleCode(clang::Decl*, unsigned int, clang::ento::ExprEngine::InliningModes, llvm::DenseSet&lt;clang::Decl const*, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;&gt;*) AnalysisConsumer.cpp:0:0
#<!-- -->15 0x000000000627da7f (anonymous namespace)::AnalysisConsumer::HandleTranslationUnit(clang::ASTContext&amp;) AnalysisConsumer.cpp:0:0
#<!-- -->16 0x000000000683512c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x683512c)
#<!-- -->17 0x0000000004dac3f5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dac3f5)
#<!-- -->18 0x0000000004d2838e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2838e)
#<!-- -->19 0x0000000004ea17fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea17fd)
#<!-- -->20 0x0000000000db6300 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb6300)
#<!-- -->21 0x0000000000dacf1a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->22 0x0000000004b2a0d9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->23 0x00000000040c9464 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40c9464)
#<!-- -->24 0x0000000004b2a6ef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->25 0x0000000004aebb32 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aebb32)
#<!-- -->26 0x0000000004aecade clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecade)
#<!-- -->27 0x0000000004af4205 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af4205)
#<!-- -->28 0x0000000000db277a clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb277a)
#<!-- -->29 0x0000000000c64464 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc64464)
#<!-- -->30 0x0000756484c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->31 0x0000756484c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->32 0x0000000000dac9c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac9c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Note: using `-std=c++03`

---

### Comment 3 - k-arrows

The similar assertion failure goes back to clang-7.0.0:
https://godbolt.org/z/6Yf44677a

---

