# clang: trunk: Compiler crash in MachineFrameInfo::getObjectOffset with large alloca allocation

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/169887
**Status:** Open
**Labels:** backend:X86, crash

## Body

### Link for quick verification: https://godbolt.org/z/na6bbKqMG
**The clang version I used is clang(trunk)**

When I used SIZE_MAX(the maximum value of size_t) as the argument to __builtin_alloca, the compiler crashed because it couldn't handle the extremely large stack allocation.
```c
#include <stdarg.h>

void test_vector_and_alloca() {
    char *buf = (char *)__builtin_alloca(__SIZE_MAX__); 
    buf[0] = 'A'; 
}
```

## The stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@_Z22test_vector_and_allocav'
 #0 0x0000000004273358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4273358)
 #1 0x0000000004270784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4270784)
 #2 0x00000000041b44b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000767d47442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000767d474969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000767d47442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000767d474287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000767d4742871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000767d47439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000002b366ab llvm::X86FrameLowering::getFrameIndexReference(llvm::MachineFunction const&, int, llvm::Register&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2b366ab)
#10 0x0000000002a64b29 llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, int, unsigned int, llvm::RegScavenger*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2a64b29)
#11 0x00000000036b6d0f (anonymous namespace)::PEIImpl::run(llvm::MachineFunction&) (.constprop.0) PrologEpilogInserter.cpp:0:0
#12 0x00000000036b7daa (anonymous namespace)::PEILegacy::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#13 0x00000000035284e9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#14 0x0000000003b6ced6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6ced6)
#15 0x0000000003b6d281 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6d281)
#16 0x0000000003b6daef llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6daef)
#17 0x0000000004531746 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4531746)
#18 0x0000000004bc1f37 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc1f37)
#19 0x000000000695755c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695755c)
#20 0x0000000004bc2398 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc2398)
#21 0x0000000004eadba5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eadba5)
#22 0x0000000004e2dd5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2dd5e)
#23 0x0000000004fa6bdd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fa6bdd)
#24 0x0000000000dc7650 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc7650)
#25 0x0000000000dbe0ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000dbe24d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x0000000004c2ab89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x00000000041b4954 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b4954)
#29 0x0000000004c2b19f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000004beb992 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4beb992)
#31 0x0000000004bec93e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bec93e)
#32 0x0000000004bf3d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf3d75)
#33 0x0000000000dc3a71 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3a71)
#34 0x0000000000c72444 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc72444)
#35 0x0000767d47429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x0000767d47429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000dbdb65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbdb65)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/na6bbKqMG
**The clang version I used is clang(trunk)**

When I used SIZE_MAX(the maximum value of size_t) as the argument to __builtin_alloca, the compiler crashed because it couldn't handle the extremely large stack allocation.
```c
#include &lt;stdarg.h&gt;

void test_vector_and_alloca() {
    char *buf = (char *)__builtin_alloca(__SIZE_MAX__); 
    buf[0] = 'A'; 
}
```

## The stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'Prologue/Epilogue Insertion &amp; Frame Finalization' on function '@<!-- -->_Z22test_vector_and_allocav'
 #<!-- -->0 0x0000000004273358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4273358)
 #<!-- -->1 0x0000000004270784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4270784)
 #<!-- -->2 0x00000000041b44b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000767d47442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000767d474969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000767d47442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000767d474287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000767d4742871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000767d47439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000002b366ab llvm::X86FrameLowering::getFrameIndexReference(llvm::MachineFunction const&amp;, int, llvm::Register&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2b366ab)
#<!-- -->10 0x0000000002a64b29 llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;, int, unsigned int, llvm::RegScavenger*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2a64b29)
#<!-- -->11 0x00000000036b6d0f (anonymous namespace)::PEIImpl::run(llvm::MachineFunction&amp;) (.constprop.0) PrologEpilogInserter.cpp:0:0
#<!-- -->12 0x00000000036b7daa (anonymous namespace)::PEILegacy::runOnMachineFunction(llvm::MachineFunction&amp;) PrologEpilogInserter.cpp:0:0
#<!-- -->13 0x00000000035284e9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->14 0x0000000003b6ced6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6ced6)
#<!-- -->15 0x0000000003b6d281 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6d281)
#<!-- -->16 0x0000000003b6daef llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b6daef)
#<!-- -->17 0x0000000004531746 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4531746)
#<!-- -->18 0x0000000004bc1f37 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc1f37)
#<!-- -->19 0x000000000695755c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695755c)
#<!-- -->20 0x0000000004bc2398 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc2398)
#<!-- -->21 0x0000000004eadba5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eadba5)
#<!-- -->22 0x0000000004e2dd5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e2dd5e)
#<!-- -->23 0x0000000004fa6bdd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fa6bdd)
#<!-- -->24 0x0000000000dc7650 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc7650)
#<!-- -->25 0x0000000000dbe0ca ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000dbe24d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004c2ab89 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x00000000041b4954 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b4954)
#<!-- -->29 0x0000000004c2b19f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004beb992 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4beb992)
#<!-- -->31 0x0000000004bec93e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bec93e)
#<!-- -->32 0x0000000004bf3d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf3d75)
#<!-- -->33 0x0000000000dc3a71 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc3a71)
#<!-- -->34 0x0000000000c72444 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc72444)
#<!-- -->35 0x0000767d47429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x0000767d47429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000dbdb65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbdb65)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

</details>


---

