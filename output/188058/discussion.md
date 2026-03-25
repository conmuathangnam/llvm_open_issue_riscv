# Assertion failure "Instruction does not dominate all uses!" when block captures variables of non-trivially destructible type used in static local initializer

**Author:** ahatanak
**URL:** https://github.com/llvm/llvm-project/issues/188058
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Clang crashes with an LLVM IR verification error when a block literal captures variables of a type with a non-trivial destructor and is used to initialize a static const local variable.

$ cat test.cpp

```c++
struct S {
  ~S();
};

void test() {
  S s1, s2;

  static const int i = ^(void) {
    (void)s1;
    (void)s2;
    return 0;
  }();
}
```

$ clang++ -std=c++20 -fblocks test.cpp -c
```
Instruction does not dominate all uses!
  %block.captured = getelementptr inbounds nuw <{ ptr, i32, i32, ptr, ptr, %struct.S, %struct.S }>, ptr %block, i32 0, i32 5, !dbg !31
  call void @_ZN1SD1Ev(ptr noundef nonnull align 1 dereferenceable(1) %block.captured) #1, !dbg !32
fatal error: error in backend: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -fblocks <source>
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "verify" on module "<source>"
 #0 0x0000000004450f68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4450f68)
 #1 0x000000000444dd94 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444dd94)
 #2 0x000000000444e3b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444e3b4)
 #3 0x000000000438dc56 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438dc56)
 #4 0x000000000444583e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444583e)
 #5 0x0000000000de10f0 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #6 0x0000000004397d63 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4397d63)
 #7 0x0000000004397ec1 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4397ec1)
 #8 0x0000000003db3f18 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3db3f18)
 #9 0x000000000486bb6e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486bb6e)
#10 0x0000000003d7b191 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d7b191)
#11 0x00000000048781fc (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#12 0x000000000487bc17 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x487bc17)
#13 0x0000000004f3bf9e clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f3bf9e)
#14 0x0000000006ca37ec clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca37ec)
#15 0x0000000004f3c3e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f3c3e8)
#16 0x00000000052570e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52570e5)
#17 0x00000000051d4bde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d4bde)
#18 0x0000000005353a2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5353a2d)
#19 0x0000000000de3a22 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde3a22)
#20 0x0000000000dda35a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#21 0x0000000000dda4dd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#22 0x0000000004fa55a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#23 0x000000000438db94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438db94)
#24 0x0000000004fa5bd6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#25 0x0000000004f64cb2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f64cb2)
#26 0x0000000004f65c5e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f65c5e)
#27 0x0000000004f6d815 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6d815)
#28 0x0000000000ddfd84 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddfd84)
#29 0x0000000000c97d7a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97d7a)
#30 0x0000758086a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#31 0x0000758086a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#32 0x0000000000dd9df5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9df5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Akira Hatanaka (ahatanak)

<details>
Clang crashes with an LLVM IR verification error when a block literal captures variables of a type with a non-trivial destructor and is used to initialize a static const local variable.

$ cat test.cpp
```
struct S {
  ~S();
};

void test() {
  S s1, s2;

  static const int i = ^(void) {
    (void)s1;
    (void)s2;
    return 0;
  }();
}
```

$ clang++ -std=c++20 -fblocks test.cpp -c
```
Instruction does not dominate all uses!
  %block.captured = getelementptr inbounds nuw &lt;{ ptr, i32, i32, ptr, ptr, %struct.S, %struct.S }&gt;, ptr %block, i32 0, i32 5, !dbg !31
  call void @<!-- -->_ZN1SD1Ev(ptr noundef nonnull align 1 dereferenceable(1) %block.captured) #<!-- -->1, !dbg !32
fatal error: error in backend: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -fblocks &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Optimizer
3.	Running pass "verify" on module "&lt;source&gt;"
 #<!-- -->0 0x0000000004450f68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4450f68)
 #<!-- -->1 0x000000000444dd94 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444dd94)
 #<!-- -->2 0x000000000444e3b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444e3b4)
 #<!-- -->3 0x000000000438dc56 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438dc56)
 #<!-- -->4 0x000000000444583e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x444583e)
 #<!-- -->5 0x0000000000de10f0 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->6 0x0000000004397d63 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4397d63)
 #<!-- -->7 0x0000000004397ec1 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4397ec1)
 #<!-- -->8 0x0000000003db3f18 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3db3f18)
 #<!-- -->9 0x000000000486bb6e llvm::detail::PassModel&lt;llvm::Module, llvm::VerifierPass, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486bb6e)
#<!-- -->10 0x0000000003d7b191 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d7b191)
#<!-- -->11 0x00000000048781fc (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->12 0x000000000487bc17 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x487bc17)
#<!-- -->13 0x0000000004f3bf9e clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f3bf9e)
#<!-- -->14 0x0000000006ca37ec clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca37ec)
#<!-- -->15 0x0000000004f3c3e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f3c3e8)
#<!-- -->16 0x00000000052570e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52570e5)
#<!-- -->17 0x00000000051d4bde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d4bde)
#<!-- -->18 0x0000000005353a2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5353a2d)
#<!-- -->19 0x0000000000de3a22 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde3a22)
#<!-- -->20 0x0000000000dda35a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->21 0x0000000000dda4dd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->22 0x0000000004fa55a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->23 0x000000000438db94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x438db94)
#<!-- -->24 0x0000000004fa5bd6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->25 0x0000000004f64cb2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f64cb2)
#<!-- -->26 0x0000000004f65c5e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f65c5e)
#<!-- -->27 0x0000000004f6d815 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6d815)
#<!-- -->28 0x0000000000ddfd84 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddfd84)
#<!-- -->29 0x0000000000c97d7a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc97d7a)
#<!-- -->30 0x0000758086a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->31 0x0000758086a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->32 0x0000000000dd9df5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9df5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

