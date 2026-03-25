# [X86] UNREACHABLE executed at /root/llvm-project/llvm/lib/Target/X86/X86FlagsCopyLowering.cpp:712!

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/169987
**Status:** Open
**Labels:** backend:X86, crash

## Body

Note: using `-mllvm -disable-machine-dce -O1`

Reproducer:
https://godbolt.org/z/964T15rh3
```cpp
void foo(char *out) {
  asm(""
      : "=@ccb"(out[0]), "=@ccl"(out[1]), "=@ccz"(out[2]), "=@ccbe"(out[4]),
        "=@ccge"(out[5]), "=@ccle"(out[6]));
}
```

Backtrace:
```console
Unlowered EFLAGS copy!
UNREACHABLE executed at /root/llvm-project/llvm/lib/Target/X86/X86FlagsCopyLowering.cpp:712!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -mllvm -disable-machine-dce -O1 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 EFLAGS copy lowering' on function '@_Z3fooPc'
 #0 0x0000000004273358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4273358)
 #1 0x0000000004270784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4270784)
 #2 0x00000000041b44b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070ab38e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070ab38e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070ab38e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070ab38e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000041c018a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c018a)
 #8 0x0000000002b17dda (anonymous namespace)::X86FlagsCopyLoweringPass::runOnMachineFunction(llvm::MachineFunction&) X86FlagsCopyLowering.cpp:0:0
 #9 0x00000000035284e9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#10 0x0000000003b6ced6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6ced6)
#11 0x0000000003b6d281 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6d281)
#12 0x0000000003b6daef llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6daef)
#13 0x0000000004531746 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4531746)
#14 0x0000000004bc1f37 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc1f37)
#15 0x000000000695755c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695755c)
#16 0x0000000004bc2398 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc2398)
#17 0x0000000004eadba5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eadba5)
#18 0x0000000004e2dd5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2dd5e)
#19 0x0000000004fa6bdd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fa6bdd)
#20 0x0000000000dc7650 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc7650)
#21 0x0000000000dbe0ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#22 0x0000000000dbe24d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#23 0x0000000004c2ab89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x00000000041b4954 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b4954)
#25 0x0000000004c2b19f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x0000000004beb992 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4beb992)
#27 0x0000000004bec93e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bec93e)
#28 0x0000000004bf3d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf3d75)
#29 0x0000000000dc3a71 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3a71)
#30 0x0000000000c72444 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc72444)
#31 0x000070ab38e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#32 0x000070ab38e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#33 0x0000000000dbdb65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbdb65)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (k-arrows)

<details>
Note: using `-mllvm -disable-machine-dce -O1`

Reproducer:
https://godbolt.org/z/964T15rh3
```cpp
void foo(char *out) {
  asm(""
      : "=@<!-- -->ccb"(out[0]), "=@<!-- -->ccl"(out[1]), "=@<!-- -->ccz"(out[2]), "=@<!-- -->ccbe"(out[4]),
        "=@<!-- -->ccge"(out[5]), "=@<!-- -->ccle"(out[6]));
}
```

Backtrace:
```console
Unlowered EFLAGS copy!
UNREACHABLE executed at /root/llvm-project/llvm/lib/Target/X86/X86FlagsCopyLowering.cpp:712!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -mllvm -disable-machine-dce -O1 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 EFLAGS copy lowering' on function '@<!-- -->_Z3fooPc'
 #<!-- -->0 0x0000000004273358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4273358)
 #<!-- -->1 0x0000000004270784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4270784)
 #<!-- -->2 0x00000000041b44b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070ab38e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070ab38e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070ab38e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070ab38e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00000000041c018a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c018a)
 #<!-- -->8 0x0000000002b17dda (anonymous namespace)::X86FlagsCopyLoweringPass::runOnMachineFunction(llvm::MachineFunction&amp;) X86FlagsCopyLowering.cpp:0:0
 #<!-- -->9 0x00000000035284e9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->10 0x0000000003b6ced6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6ced6)
#<!-- -->11 0x0000000003b6d281 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6d281)
#<!-- -->12 0x0000000003b6daef llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6daef)
#<!-- -->13 0x0000000004531746 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4531746)
#<!-- -->14 0x0000000004bc1f37 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc1f37)
#<!-- -->15 0x000000000695755c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695755c)
#<!-- -->16 0x0000000004bc2398 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc2398)
#<!-- -->17 0x0000000004eadba5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eadba5)
#<!-- -->18 0x0000000004e2dd5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2dd5e)
#<!-- -->19 0x0000000004fa6bdd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fa6bdd)
#<!-- -->20 0x0000000000dc7650 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc7650)
#<!-- -->21 0x0000000000dbe0ca ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->22 0x0000000000dbe24d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->23 0x0000000004c2ab89 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->24 0x00000000041b4954 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b4954)
#<!-- -->25 0x0000000004c2b19f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->26 0x0000000004beb992 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4beb992)
#<!-- -->27 0x0000000004bec93e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bec93e)
#<!-- -->28 0x0000000004bf3d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf3d75)
#<!-- -->29 0x0000000000dc3a71 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3a71)
#<!-- -->30 0x0000000000c72444 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc72444)
#<!-- -->31 0x000070ab38e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->32 0x000070ab38e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->33 0x0000000000dbdb65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbdb65)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

