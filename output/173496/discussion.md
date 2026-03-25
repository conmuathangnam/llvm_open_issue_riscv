# Clang-17 With Assertion Crashes with Inline Assembly

**Author:** cactusBalll
**URL:** https://github.com/llvm/llvm-project/issues/173496
**Status:** Closed
**Labels:** duplicate, backend:X86, llvm:codegen, crash
**Closed Date:** 2025-12-25T02:32:15Z

## Body

Compiler Explore：https://godbolt.org/z/cY5csW58o
Related to #118247 possibly.
```c++
int main() {
    unsigned arg1 = 100;
    unsigned arg2 = 200;
    
    asm volatile (
        "movl %[arg1], %%ecx\n\t"
        "addl %[arg2], %[arg1]\n\t"
        "addl %[arg2], %%ecx\n\t"
        "rcl $0, %%ecx"
        : [arg1] "+&abdSD" (arg1), [arg2] "+&abdSD" (arg2)
        :
        : "cc", "ecx"
    );
    return 0;
} 
```
```
clang++: /root/llvm-project/llvm/lib/CodeGen/TwoAddressInstructionPass.cpp:1596: void {anonymous}::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, {anonymous}::TwoAddressInstructionImpl::TiedPairList&, unsigned int&): Assertion `i == DstIdx || !MI->getOperand(i).isReg() || MI->getOperand(i).getReg() != RegA' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -O3 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Two-Address instruction pass' on function '@main'
 #0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000739506a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000739506a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000739506a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000739506a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000739506a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000739506a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000003643191 (anonymous namespace)::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, llvm::SmallVector<std::pair<unsigned int, unsigned int>, 4u>&, unsigned int&) TwoAddressInstructionPass.cpp:0:0
#10 0x0000000003644dbd (anonymous namespace)::TwoAddressInstructionImpl::run() TwoAddressInstructionPass.cpp:0:0
#11 0x0000000003646e77 (anonymous namespace)::TwoAddressInstructionLegacyPass::runOnMachineFunction(llvm::MachineFunction&) TwoAddressInstructionPass.cpp:0:0
#12 0x000000000331d873 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#13 0x0000000003944d8f llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3944d8f)
#14 0x0000000003945141 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3945141)
#15 0x00000000039459b1 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x39459b1)
#16 0x00000000042d2791 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x42d2791)
#17 0x00000000049cf623 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cf623)
#18 0x00000000066ca95c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca95c)
#19 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#20 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#21 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#22 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#23 0x0000000000d93cff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#24 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x0000000004a36b79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#27 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#29 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#30 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#31 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)
#32 0x0000000000c444d4 main (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xc444d4)
#33 0x0000739506a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x0000739506a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000d8a365 _start (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd8a365)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (cactusBalll)

<details>
Compiler Explore：https://godbolt.org/z/cY5csW58o
Related to #<!-- -->118247 possibly.
```c++
int main() {
    unsigned arg1 = 100;
    unsigned arg2 = 200;
    
    asm volatile (
        "movl %[arg1], %%ecx\n\t"
        "addl %[arg2], %[arg1]\n\t"
        "addl %[arg2], %%ecx\n\t"
        "rcl $0, %%ecx"
        : [arg1] "+&amp;abdSD" (arg1), [arg2] "+&amp;abdSD" (arg2)
        :
        : "cc", "ecx"
    );
    return 0;
} 
```
```
clang++: /root/llvm-project/llvm/lib/CodeGen/TwoAddressInstructionPass.cpp:1596: void {anonymous}::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, {anonymous}::TwoAddressInstructionImpl::TiedPairList&amp;, unsigned int&amp;): Assertion `i == DstIdx || !MI-&gt;getOperand(i).isReg() || MI-&gt;getOperand(i).getReg() != RegA' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -O3 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'Two-Address instruction pass' on function '@<!-- -->main'
 #<!-- -->0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #<!-- -->1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #<!-- -->2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000739506a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000739506a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000739506a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000739506a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000739506a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000739506a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000003643191 (anonymous namespace)::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, llvm::SmallVector&lt;std::pair&lt;unsigned int, unsigned int&gt;, 4u&gt;&amp;, unsigned int&amp;) TwoAddressInstructionPass.cpp:0:0
#<!-- -->10 0x0000000003644dbd (anonymous namespace)::TwoAddressInstructionImpl::run() TwoAddressInstructionPass.cpp:0:0
#<!-- -->11 0x0000000003646e77 (anonymous namespace)::TwoAddressInstructionLegacyPass::runOnMachineFunction(llvm::MachineFunction&amp;) TwoAddressInstructionPass.cpp:0:0
#<!-- -->12 0x000000000331d873 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->13 0x0000000003944d8f llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3944d8f)
#<!-- -->14 0x0000000003945141 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3945141)
#<!-- -->15 0x00000000039459b1 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x39459b1)
#<!-- -->16 0x00000000042d2791 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x42d2791)
#<!-- -->17 0x00000000049cf623 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cf623)
#<!-- -->18 0x00000000066ca95c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca95c)
#<!-- -->19 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#<!-- -->20 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#<!-- -->21 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#<!-- -->22 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#<!-- -->23 0x0000000000d93cff cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#<!-- -->24 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->25 0x0000000004a36b79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->26 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#<!-- -->27 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->28 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#<!-- -->29 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#<!-- -->30 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#<!-- -->31 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)
#<!-- -->32 0x0000000000c444d4 main (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xc444d4)
#<!-- -->33 0x0000739506a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->34 0x0000739506a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->35 0x0000000000d8a365 _start (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd8a365)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - topperc

> Related to https://github.com/llvm/llvm-project/issues/118247 possibly.

Yes, it's the same issue.

---

