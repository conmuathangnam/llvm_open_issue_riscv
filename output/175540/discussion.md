# clang crashed with asm goto : error in backend: Broken module found, compilation aborted!

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/175540
**Status:** Closed
**Labels:** clang:frontend, crash, inline-asm
**Closed Date:** 2026-01-13T22:05:14Z

## Body

clang crashed with `asm goto`
Compiler Explorer: https://godbolt.org/z/fah4xhzKa

```
$ cat small.c
int b;
void *c;
void e() {
  __asm__ goto("" : : : : d);
  int(*a)[b] = c;
d:
  a[0];
}
```

Crashed with `-O0`:
```
<source>:7:6: warning: expression result unused [-Wunused-value]
    7 |   a[0];
      |   ~ ~^
Instruction does not dominate all uses!
  %1 = zext i32 %0 to i64, !dbg !25
  %4 = mul nsw i64 0, %1, !dbg !35
fatal error: error in backend: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O0 <source>
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "verify" on module "<source>"
 #0 0x000000000430abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x430abe8)
 #1 0x0000000004308014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4308014)
 #2 0x000000000424af66 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x424af66)
 #3 0x00000000042ff4de llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42ff4de)
 #4 0x0000000000de6190 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000004255183 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4255183)
 #6 0x00000000042552e1 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42552e1)
 #7 0x0000000003c82e38 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c82e38)
 #8 0x00000000045bc5ee llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45bc5ee)
 #9 0x0000000003c4a251 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c4a251)
#10 0x00000000045c8acc (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#11 0x00000000045cc3f0 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45cc3f0)
#12 0x0000000004c6d697 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c6d697)
#13 0x0000000006a3fdbc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a3fdbc)
#14 0x0000000004c6daf8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c6daf8)
#15 0x0000000004f66ee5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f66ee5)
#16 0x0000000004ee6d1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ee6d1e)
#17 0x000000000506158d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x506158d)
#18 0x0000000000de8a9e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde8a9e)
#19 0x0000000000ddf46a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#20 0x0000000000ddf5ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#21 0x0000000004ce0b39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#22 0x000000000424aea4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x424aea4)
#23 0x0000000004ce114f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#24 0x0000000004ca1ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca1ae2)
#25 0x0000000004ca2a8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca2a8e)
#26 0x0000000004ca9ee5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca9ee5)
#27 0x0000000000de4e11 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde4e11)
#28 0x0000000000c8f104 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc8f104)
#29 0x00007f60ec829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#30 0x00007f60ec829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#31 0x0000000000ddef05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xddef05)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
clang crashed with `asm goto`
Compiler Explorer: https://godbolt.org/z/fah4xhzKa

```
$ cat small.c
int b;
void *c;
void e() {
  __asm__ goto("" : : : : d);
  int(*a)[b] = c;
d:
  a[0];
}
```

Crashed with `-O0`:
```
&lt;source&gt;:7:6: warning: expression result unused [-Wunused-value]
    7 |   a[0];
      |   ~ ~^
Instruction does not dominate all uses!
  %1 = zext i32 %0 to i64, !dbg !25
  %4 = mul nsw i64 0, %1, !dbg !35
fatal error: error in backend: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O0 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Optimizer
3.	Running pass "verify" on module "&lt;source&gt;"
 #<!-- -->0 0x000000000430abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x430abe8)
 #<!-- -->1 0x0000000004308014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4308014)
 #<!-- -->2 0x000000000424af66 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x424af66)
 #<!-- -->3 0x00000000042ff4de llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42ff4de)
 #<!-- -->4 0x0000000000de6190 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000004255183 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4255183)
 #<!-- -->6 0x00000000042552e1 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42552e1)
 #<!-- -->7 0x0000000003c82e38 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c82e38)
 #<!-- -->8 0x00000000045bc5ee llvm::detail::PassModel&lt;llvm::Module, llvm::VerifierPass, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45bc5ee)
 #<!-- -->9 0x0000000003c4a251 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c4a251)
#<!-- -->10 0x00000000045c8acc (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->11 0x00000000045cc3f0 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45cc3f0)
#<!-- -->12 0x0000000004c6d697 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c6d697)
#<!-- -->13 0x0000000006a3fdbc clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a3fdbc)
#<!-- -->14 0x0000000004c6daf8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c6daf8)
#<!-- -->15 0x0000000004f66ee5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f66ee5)
#<!-- -->16 0x0000000004ee6d1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ee6d1e)
#<!-- -->17 0x000000000506158d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x506158d)
#<!-- -->18 0x0000000000de8a9e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde8a9e)
#<!-- -->19 0x0000000000ddf46a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->20 0x0000000000ddf5ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->21 0x0000000004ce0b39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->22 0x000000000424aea4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x424aea4)
#<!-- -->23 0x0000000004ce114f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->24 0x0000000004ca1ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca1ae2)
#<!-- -->25 0x0000000004ca2a8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca2a8e)
#<!-- -->26 0x0000000004ca9ee5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca9ee5)
#<!-- -->27 0x0000000000de4e11 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde4e11)
#<!-- -->28 0x0000000000c8f104 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc8f104)
#<!-- -->29 0x00007f60ec829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->30 0x00007f60ec829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->31 0x0000000000ddef05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xddef05)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

### Comment 2 - EugeneZelenko

Duplicate of #113325?

---

### Comment 3 - shafik

> Duplicate of [#113325](https://github.com/llvm/llvm-project/issues/113325)?

This one starts in clang-19

---

