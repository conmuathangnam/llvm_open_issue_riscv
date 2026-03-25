# Assertion `V && "Dereferencing deleted ValueHandle"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/162228
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Reproducer:
https://godbolt.org/z/zcvvYhznq
```cpp
int foo __asm__("foo") __attribute__((used));
short bar __asm__("foo") = 42;
```

Baktrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/IR/ValueHandle.h:95: llvm::Value& llvm::ValueHandleBase::operator*() const: Assertion `V && "Dereferencing deleted ValueHandle"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Per-file LLVM IR generation
 #0 0x00000000041d20a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41d20a8)
 #1 0x00000000041cf4d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41cf4d4)
 #2 0x0000000004113668 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071908c442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000071908c4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000071908c442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000071908c4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000071908c42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000071908c439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000004749049 emitUsed(clang::CodeGen::CodeGenModule&, llvm::StringRef, std::vector<llvm::WeakTrackingVH, std::allocator<llvm::WeakTrackingVH>>&) CodeGenModule.cpp:0:0
#10 0x00000000047b95ba clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47b95ba)
#11 0x0000000004b17f3e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) ModuleBuilder.cpp:0:0
#12 0x0000000004b13f45 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b13f45)
#13 0x00000000068a504c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68a504c)
#14 0x0000000004b14728 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b14728)
#15 0x0000000004e00195 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e00195)
#16 0x0000000004d81dee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d81dee)
#17 0x0000000004ef748d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef748d)
#18 0x0000000000dbc110 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbc110)
#19 0x0000000000db2c7a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#20 0x0000000000db2dfd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#21 0x0000000004b7d8d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#22 0x0000000004113b04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4113b04)
#23 0x0000000004b7deef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#24 0x0000000004b3f1f2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3f1f2)
#25 0x0000000004b4019e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4019e)
#26 0x0000000004b478d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b478d5)
#27 0x0000000000db8639 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8639)
#28 0x0000000000c6a194 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6a194)
#29 0x000071908c429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#30 0x000071908c429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#31 0x0000000000db2715 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2715)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/zcvvYhznq
```cpp
int foo __asm__("foo") __attribute__((used));
short bar __asm__("foo") = 42;
```

Baktrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/IR/ValueHandle.h:95: llvm::Value&amp; llvm::ValueHandleBase::operator*() const: Assertion `V &amp;&amp; "Dereferencing deleted ValueHandle"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Per-file LLVM IR generation
 #<!-- -->0 0x00000000041d20a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41d20a8)
 #<!-- -->1 0x00000000041cf4d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41cf4d4)
 #<!-- -->2 0x0000000004113668 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000071908c442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000071908c4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000071908c442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000071908c4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000071908c42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000071908c439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000004749049 emitUsed(clang::CodeGen::CodeGenModule&amp;, llvm::StringRef, std::vector&lt;llvm::WeakTrackingVH, std::allocator&lt;llvm::WeakTrackingVH&gt;&gt;&amp;) CodeGenModule.cpp:0:0
#<!-- -->10 0x00000000047b95ba clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47b95ba)
#<!-- -->11 0x0000000004b17f3e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&amp;) ModuleBuilder.cpp:0:0
#<!-- -->12 0x0000000004b13f45 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b13f45)
#<!-- -->13 0x00000000068a504c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68a504c)
#<!-- -->14 0x0000000004b14728 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b14728)
#<!-- -->15 0x0000000004e00195 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e00195)
#<!-- -->16 0x0000000004d81dee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d81dee)
#<!-- -->17 0x0000000004ef748d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef748d)
#<!-- -->18 0x0000000000dbc110 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbc110)
#<!-- -->19 0x0000000000db2c7a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->20 0x0000000000db2dfd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->21 0x0000000004b7d8d9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->22 0x0000000004113b04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4113b04)
#<!-- -->23 0x0000000004b7deef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->24 0x0000000004b3f1f2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3f1f2)
#<!-- -->25 0x0000000004b4019e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4019e)
#<!-- -->26 0x0000000004b478d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b478d5)
#<!-- -->27 0x0000000000db8639 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8639)
#<!-- -->28 0x0000000000c6a194 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6a194)
#<!-- -->29 0x000071908c429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->30 0x000071908c429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->31 0x0000000000db2715 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2715)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Crash in ValueHandle.h goes back to clang-11.0.0:
https://godbolt.org/z/4ozhh17dG

---

