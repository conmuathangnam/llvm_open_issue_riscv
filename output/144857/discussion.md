# [llvm] LLVM backend crashes when using `__builtin_ia32_prefetchi`

**Author:** MacroModel
**URL:** https://github.com/llvm/llvm-project/issues/144857
**Status:** Closed
**Labels:** backend:X86, clang:frontend, crash-on-invalid
**Closed Date:** 2025-07-22T07:25:18Z

## Body

see: https://godbolt.org/z/aPPfjjbs3

```cpp
void x86_perfchi(void* p) noexcept
{
    __builtin_ia32_prefetchi(p, 0);
}
```

```
fatal error: error in backend: Cannot select: 0x1e933290: ch = Prefetch<(load (s8) from %ir.0)> 0x1e849900, 0x1e9331b0, TargetConstant:i32<0>, TargetConstant:i32<0>, TargetConstant:i32<0>, example.cpp:3:5
  0x1e9331b0: i64,ch = CopyFromReg 0x1e849900, Register:i64 %0
In function: _Z11x86_perfchiPv
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 -O1 -march=znver5 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@_Z11x86_perfchiPv'
 #0 0x0000000003c27a08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c27a08)
 #1 0x0000000003c25b4c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c25b4c)
 #2 0x0000000003b766b3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b766b3)
 #3 0x0000000003c1d61e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1d61e)
 #4 0x0000000000d9c80b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003b80573 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b80573)
 #6 0x00000000051e9e2a llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e9e2a)
 #7 0x00000000051efcf9 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51efcf9)
 #8 0x00000000026fb3fa (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
 #9 0x00000000051e73e4 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e73e4)
#10 0x00000000051f58d2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51f58d2)
#11 0x00000000051f8235 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51f8235)
#12 0x00000000051fa0e6 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51fa0e6)
#13 0x00000000051e6f71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e6f71)
#14 0x000000000300f2d8 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#15 0x0000000003584aa2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3584aa2)
#16 0x0000000003584d31 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3584d31)
#17 0x0000000003586679 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3586679)
#18 0x0000000003ed69d0 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ed69d0)
#19 0x0000000004572f3c clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4572f3c)
#20 0x00000000061d13ec clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61d13ec)
#21 0x0000000004573b0d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4573b0d)
#22 0x000000000488570a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x488570a)
#23 0x000000000480134b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x480134b)
#24 0x000000000497a193 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x497a193)
#25 0x0000000000d9e785 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd9e785)
#26 0x0000000000d9674d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x00000000045e5819 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003b765b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b765b3)
#29 0x00000000045e5a39 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x00000000045a86ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45a86ed)
#31 0x00000000045a96a1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45a96a1)
#32 0x00000000045b3f8c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b3f8c)
#33 0x0000000000d9b081 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd9b081)
#34 0x0000000000c4db64 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4db64)
#35 0x00007a37a3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007a37a3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000d961e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd961e5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (MacroModel)

<details>
see: https://godbolt.org/z/aPPfjjbs3

```cpp
void x86_perfchi(void* p) noexcept
{
    __builtin_ia32_prefetchi(p, 0);
}
```

```
fatal error: error in backend: Cannot select: 0x1e933290: ch = Prefetch&lt;(load (s8) from %ir.0)&gt; 0x1e849900, 0x1e9331b0, TargetConstant:i32&lt;0&gt;, TargetConstant:i32&lt;0&gt;, TargetConstant:i32&lt;0&gt;, example.cpp:3:5
  0x1e9331b0: i64,ch = CopyFromReg 0x1e849900, Register:i64 %0
In function: _Z11x86_perfchiPv
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 -O1 -march=znver5 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->_Z11x86_perfchiPv'
 #<!-- -->0 0x0000000003c27a08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c27a08)
 #<!-- -->1 0x0000000003c25b4c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c25b4c)
 #<!-- -->2 0x0000000003b766b3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b766b3)
 #<!-- -->3 0x0000000003c1d61e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1d61e)
 #<!-- -->4 0x0000000000d9c80b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003b80573 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b80573)
 #<!-- -->6 0x00000000051e9e2a llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e9e2a)
 #<!-- -->7 0x00000000051efcf9 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51efcf9)
 #<!-- -->8 0x00000000026fb3fa (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
 #<!-- -->9 0x00000000051e73e4 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e73e4)
#<!-- -->10 0x00000000051f58d2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51f58d2)
#<!-- -->11 0x00000000051f8235 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51f8235)
#<!-- -->12 0x00000000051fa0e6 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51fa0e6)
#<!-- -->13 0x00000000051e6f71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e6f71)
#<!-- -->14 0x000000000300f2d8 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->15 0x0000000003584aa2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3584aa2)
#<!-- -->16 0x0000000003584d31 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3584d31)
#<!-- -->17 0x0000000003586679 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3586679)
#<!-- -->18 0x0000000003ed69d0 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ed69d0)
#<!-- -->19 0x0000000004572f3c clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4572f3c)
#<!-- -->20 0x00000000061d13ec clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61d13ec)
#<!-- -->21 0x0000000004573b0d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4573b0d)
#<!-- -->22 0x000000000488570a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x488570a)
#<!-- -->23 0x000000000480134b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x480134b)
#<!-- -->24 0x000000000497a193 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x497a193)
#<!-- -->25 0x0000000000d9e785 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd9e785)
#<!-- -->26 0x0000000000d9674d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000045e5819 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003b765b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b765b3)
#<!-- -->29 0x00000000045e5a39 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x00000000045a86ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45a86ed)
#<!-- -->31 0x00000000045a96a1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45a96a1)
#<!-- -->32 0x00000000045b3f8c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b3f8c)
#<!-- -->33 0x0000000000d9b081 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd9b081)
#<!-- -->34 0x0000000000c4db64 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4db64)
#<!-- -->35 0x00007a37a3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007a37a3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000d961e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd961e5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

### Comment 2 - phoebewang

We have `prefetchit0` and `prefetchit1` instructions which map to _MM_HINT_T0(3) and _MM_HINT_T1(2). The argument 0 is not supported.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MacroModel)

<details>
see: https://godbolt.org/z/aPPfjjbs3

```cpp
void x86_perfchi(void* p) noexcept
{
    __builtin_ia32_prefetchi(p, 0);
}
```

```
fatal error: error in backend: Cannot select: 0x1e933290: ch = Prefetch&lt;(load (s8) from %ir.0)&gt; 0x1e849900, 0x1e9331b0, TargetConstant:i32&lt;0&gt;, TargetConstant:i32&lt;0&gt;, TargetConstant:i32&lt;0&gt;, example.cpp:3:5
  0x1e9331b0: i64,ch = CopyFromReg 0x1e849900, Register:i64 %0
In function: _Z11x86_perfchiPv
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 -O1 -march=znver5 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->_Z11x86_perfchiPv'
 #<!-- -->0 0x0000000003c27a08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c27a08)
 #<!-- -->1 0x0000000003c25b4c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c25b4c)
 #<!-- -->2 0x0000000003b766b3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b766b3)
 #<!-- -->3 0x0000000003c1d61e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1d61e)
 #<!-- -->4 0x0000000000d9c80b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003b80573 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b80573)
 #<!-- -->6 0x00000000051e9e2a llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e9e2a)
 #<!-- -->7 0x00000000051efcf9 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51efcf9)
 #<!-- -->8 0x00000000026fb3fa (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
 #<!-- -->9 0x00000000051e73e4 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e73e4)
#<!-- -->10 0x00000000051f58d2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51f58d2)
#<!-- -->11 0x00000000051f8235 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51f8235)
#<!-- -->12 0x00000000051fa0e6 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51fa0e6)
#<!-- -->13 0x00000000051e6f71 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e6f71)
#<!-- -->14 0x000000000300f2d8 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->15 0x0000000003584aa2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3584aa2)
#<!-- -->16 0x0000000003584d31 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3584d31)
#<!-- -->17 0x0000000003586679 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3586679)
#<!-- -->18 0x0000000003ed69d0 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ed69d0)
#<!-- -->19 0x0000000004572f3c clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4572f3c)
#<!-- -->20 0x00000000061d13ec clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61d13ec)
#<!-- -->21 0x0000000004573b0d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4573b0d)
#<!-- -->22 0x000000000488570a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x488570a)
#<!-- -->23 0x000000000480134b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x480134b)
#<!-- -->24 0x000000000497a193 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x497a193)
#<!-- -->25 0x0000000000d9e785 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd9e785)
#<!-- -->26 0x0000000000d9674d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000045e5819 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003b765b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b765b3)
#<!-- -->29 0x00000000045e5a39 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x00000000045a86ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45a86ed)
#<!-- -->31 0x00000000045a96a1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45a96a1)
#<!-- -->32 0x00000000045b3f8c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b3f8c)
#<!-- -->33 0x0000000000d9b081 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd9b081)
#<!-- -->34 0x0000000000c4db64 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4db64)
#<!-- -->35 0x00007a37a3a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007a37a3a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000d961e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd961e5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

