# [Clang] Clang crashes when using SSE4.1 intrinsics without -msse4.1 flag

**Author:** Xniao
**URL:** https://github.com/llvm/llvm-project/issues/151623

## Body

Clang crashes when using SSE4.1 intrinsics (like _mm_blendv_epi8) without the appropriate target flag (-msse4.1), while GCC correctly provides a clear diagnostic message.
Repro: https://godbolt.org/z/x1Wvc98b1

## Program
```cpp
#include <immintrin.h>

const int8_t scalarValue = 1;
__m128i vector1 = _mm_set1_epi8(scalarValue);
__m128i vector2 = _mm_set1_epi8(2);
__m128i vector3 = _mm_set1_epi8(3);
__m128i resultVector = _mm_blendv_epi8(vector2, vector3, vector1);
```

## Stack Dump
```bash
fatal error: error in backend: Cannot select: 0x3f174100: v16i8 = X86ISD::BLENDV 0x3f173f40, 0x3f173e60, 0x3f173d80, example.cpp:7:24
  0x3f173f40: v16i8,ch = CopyFromReg 0x3f07fd50, Register:v16i8 %5, example.cpp:7:24
  0x3f173e60: v16i8,ch = CopyFromReg 0x3f07fd50, Register:v16i8 %4, example.cpp:7:24
  0x3f173d80: v16i8,ch = CopyFromReg 0x3f07fd50, Register:v16i8 %3, example.cpp:7:24
In function: __cxx_global_var_init
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@__cxx_global_var_init'
 #0 0x0000000003ca5fb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca5fb8)
 #1 0x0000000003ca398c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca398c)
 #2 0x0000000003bf41d3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf41d3)
 #3 0x0000000003c9b1de llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9b1de)
 #4 0x0000000000db8b9b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003bfe103 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bfe103)
 #6 0x00000000052686aa llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x52686aa)
 #7 0x000000000526e581 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x526e581)
 #8 0x000000000276156a (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
 #9 0x0000000005265804 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5265804)
#10 0x0000000005274152 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5274152)
#11 0x0000000005277116 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5277116)
#12 0x0000000005278916 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5278916)
#13 0x0000000005265391 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5265391)
#14 0x0000000003086c5d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3086c5d)
#15 0x00000000035ffee2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35ffee2)
#16 0x0000000003600171 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3600171)
#17 0x0000000003601ab9 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3601ab9)
#18 0x0000000003f4f0c0 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f4f0c0)
#19 0x00000000045f019c clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f019c)
#20 0x000000000625873c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625873c)
#21 0x00000000045f098d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f098d)
#22 0x00000000048fa40a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48fa40a)
#23 0x000000000487546b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487546b)
#24 0x00000000049ee543 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ee543)
#25 0x0000000000dbabf5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbabf5)
#26 0x0000000000db2b4d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x00000000046628f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003bf40d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf40d3)
#29 0x0000000004662b19 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000000004625fbd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625fbd)
#31 0x0000000004626f71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4626f71)
#32 0x000000000463370c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463370c)
#33 0x0000000000db7411 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7411)
#34 0x0000000000c67984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67984)
#35 0x00007176a7c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007176a7c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000db25e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb25e5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Jireh Guo (Xniao)

<details>
Clang crashes when using SSE4.1 intrinsics (like _mm_blendv_epi8) without the appropriate target flag (-msse4.1), while GCC correctly provides a clear diagnostic message.
Repro: https://godbolt.org/z/x1Wvc98b1

## Program
```cpp
#include &lt;immintrin.h&gt;

const int8_t scalarValue = 1;
__m128i vector1 = _mm_set1_epi8(scalarValue);
__m128i vector2 = _mm_set1_epi8(2);
__m128i vector3 = _mm_set1_epi8(3);
__m128i resultVector = _mm_blendv_epi8(vector2, vector3, vector1);
```

## Stack Dump
```bash
fatal error: error in backend: Cannot select: 0x3f174100: v16i8 = X86ISD::BLENDV 0x3f173f40, 0x3f173e60, 0x3f173d80, example.cpp:7:24
  0x3f173f40: v16i8,ch = CopyFromReg 0x3f07fd50, Register:v16i8 %5, example.cpp:7:24
  0x3f173e60: v16i8,ch = CopyFromReg 0x3f07fd50, Register:v16i8 %4, example.cpp:7:24
  0x3f173d80: v16i8,ch = CopyFromReg 0x3f07fd50, Register:v16i8 %3, example.cpp:7:24
In function: __cxx_global_var_init
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->__cxx_global_var_init'
 #<!-- -->0 0x0000000003ca5fb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca5fb8)
 #<!-- -->1 0x0000000003ca398c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca398c)
 #<!-- -->2 0x0000000003bf41d3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf41d3)
 #<!-- -->3 0x0000000003c9b1de llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9b1de)
 #<!-- -->4 0x0000000000db8b9b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003bfe103 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bfe103)
 #<!-- -->6 0x00000000052686aa llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x52686aa)
 #<!-- -->7 0x000000000526e581 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x526e581)
 #<!-- -->8 0x000000000276156a (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
 #<!-- -->9 0x0000000005265804 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5265804)
#<!-- -->10 0x0000000005274152 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5274152)
#<!-- -->11 0x0000000005277116 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5277116)
#<!-- -->12 0x0000000005278916 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5278916)
#<!-- -->13 0x0000000005265391 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5265391)
#<!-- -->14 0x0000000003086c5d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3086c5d)
#<!-- -->15 0x00000000035ffee2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35ffee2)
#<!-- -->16 0x0000000003600171 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3600171)
#<!-- -->17 0x0000000003601ab9 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3601ab9)
#<!-- -->18 0x0000000003f4f0c0 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f4f0c0)
#<!-- -->19 0x00000000045f019c clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f019c)
#<!-- -->20 0x000000000625873c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625873c)
#<!-- -->21 0x00000000045f098d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f098d)
#<!-- -->22 0x00000000048fa40a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48fa40a)
#<!-- -->23 0x000000000487546b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487546b)
#<!-- -->24 0x00000000049ee543 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ee543)
#<!-- -->25 0x0000000000dbabf5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbabf5)
#<!-- -->26 0x0000000000db2b4d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000046628f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003bf40d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf40d3)
#<!-- -->29 0x0000000004662b19 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004625fbd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625fbd)
#<!-- -->31 0x0000000004626f71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4626f71)
#<!-- -->32 0x000000000463370c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463370c)
#<!-- -->33 0x0000000000db7411 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7411)
#<!-- -->34 0x0000000000c67984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67984)
#<!-- -->35 0x00007176a7c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007176a7c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000db25e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb25e5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jireh Guo (Xniao)

<details>
Clang crashes when using SSE4.1 intrinsics (like _mm_blendv_epi8) without the appropriate target flag (-msse4.1), while GCC correctly provides a clear diagnostic message.
Repro: https://godbolt.org/z/x1Wvc98b1

## Program
```cpp
#include &lt;immintrin.h&gt;

const int8_t scalarValue = 1;
__m128i vector1 = _mm_set1_epi8(scalarValue);
__m128i vector2 = _mm_set1_epi8(2);
__m128i vector3 = _mm_set1_epi8(3);
__m128i resultVector = _mm_blendv_epi8(vector2, vector3, vector1);
```

## Stack Dump
```bash
fatal error: error in backend: Cannot select: 0x3f174100: v16i8 = X86ISD::BLENDV 0x3f173f40, 0x3f173e60, 0x3f173d80, example.cpp:7:24
  0x3f173f40: v16i8,ch = CopyFromReg 0x3f07fd50, Register:v16i8 %5, example.cpp:7:24
  0x3f173e60: v16i8,ch = CopyFromReg 0x3f07fd50, Register:v16i8 %4, example.cpp:7:24
  0x3f173d80: v16i8,ch = CopyFromReg 0x3f07fd50, Register:v16i8 %3, example.cpp:7:24
In function: __cxx_global_var_init
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->__cxx_global_var_init'
 #<!-- -->0 0x0000000003ca5fb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca5fb8)
 #<!-- -->1 0x0000000003ca398c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca398c)
 #<!-- -->2 0x0000000003bf41d3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf41d3)
 #<!-- -->3 0x0000000003c9b1de llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9b1de)
 #<!-- -->4 0x0000000000db8b9b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003bfe103 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bfe103)
 #<!-- -->6 0x00000000052686aa llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x52686aa)
 #<!-- -->7 0x000000000526e581 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x526e581)
 #<!-- -->8 0x000000000276156a (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
 #<!-- -->9 0x0000000005265804 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5265804)
#<!-- -->10 0x0000000005274152 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5274152)
#<!-- -->11 0x0000000005277116 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5277116)
#<!-- -->12 0x0000000005278916 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5278916)
#<!-- -->13 0x0000000005265391 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5265391)
#<!-- -->14 0x0000000003086c5d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3086c5d)
#<!-- -->15 0x00000000035ffee2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35ffee2)
#<!-- -->16 0x0000000003600171 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3600171)
#<!-- -->17 0x0000000003601ab9 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3601ab9)
#<!-- -->18 0x0000000003f4f0c0 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f4f0c0)
#<!-- -->19 0x00000000045f019c clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f019c)
#<!-- -->20 0x000000000625873c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625873c)
#<!-- -->21 0x00000000045f098d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f098d)
#<!-- -->22 0x00000000048fa40a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48fa40a)
#<!-- -->23 0x000000000487546b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487546b)
#<!-- -->24 0x00000000049ee543 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ee543)
#<!-- -->25 0x0000000000dbabf5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbabf5)
#<!-- -->26 0x0000000000db2b4d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x00000000046628f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003bf40d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf40d3)
#<!-- -->29 0x0000000004662b19 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000000004625fbd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625fbd)
#<!-- -->31 0x0000000004626f71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4626f71)
#<!-- -->32 0x000000000463370c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463370c)
#<!-- -->33 0x0000000000db7411 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7411)
#<!-- -->34 0x0000000000c67984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67984)
#<!-- -->35 0x00007176a7c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007176a7c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000db25e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb25e5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

### Comment 3 - RKSimon

Not really DAG related - for some reason clang isn't testing target bits for intrinsics used in globals

---

### Comment 4 - Xniao

Here is another similar case that exhibits the same crash pattern.
Reproducer: https://godbolt.org/z/GT787c1jx  

This involves a different SSE4.2 intrinsic and also crashes with "Cannot select: " when compiled without -msse4.2 flag, but compiles successfully with the flag.

## Program
```cpp
#include <cstdint>
#include <immintrin.h>

const uint32_t crc32_seed = 0xFFFFFFFF;
uint32_t input_data = 0x12345678;
uint32_t result =  _mm_crc32_u32(crc32_seed, input_data);
```

## Stack Dump
```bash
fatal error: error in backend: Cannot select: intrinsic %llvm.x86.sse42.crc32.32.32
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@__cxx_global_var_init'
 #0 0x0000000003ff2c88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff2c88)
 #1 0x0000000003ff00b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ff00b4)
 #2 0x0000000003f34e06 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f34e06)
 #3 0x0000000003fe759e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fe759e)
 #4 0x0000000000daaec0 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003f3fb03 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f3fb03)
 #6 0x00000000056554c1 llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56554c1)
 #7 0x000000000565a021 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x565a021)
 #8 0x0000000002a40927 (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
 #9 0x00000000056513fb llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x56513fb)
#10 0x0000000005660980 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5660980)
#11 0x0000000005663c74 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5663c74)
#12 0x0000000005665285 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5665285)
#13 0x0000000005650bef llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5650bef)
#14 0x0000000003302c19 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#15 0x0000000003924e6f llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3924e6f)
#16 0x0000000003925221 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3925221)
#17 0x0000000003925a91 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3925a91)
#18 0x000000000429e59e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429e59e)
#19 0x000000000499a073 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499a073)
#20 0x00000000066a334c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a334c)
#21 0x000000000499a458 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499a458)
#22 0x0000000004c8ee15 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8ee15)
#23 0x0000000004c0b0de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0b0de)
#24 0x0000000004d83f61 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d83f61)
#25 0x0000000000dad5ef cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdad5ef)
#26 0x0000000000da425a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x0000000004a01f79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003f34d44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f34d44)
#29 0x0000000004a0258f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x00000000049c47cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c47cd)
#31 0x00000000049c585e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c585e)
#32 0x00000000049cd2a5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cd2a5)
#33 0x0000000000da9a85 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda9a85)
#34 0x0000000000c5dde4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc5dde4)
#35 0x00007b5950a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007b5950a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000da3d05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda3d05)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

---

### Comment 5 - abhishek-kaushik22

I see this comment in `CodeGenFunction::checkTargetFeatures`

```
  // Get the current enclosing function if it exists. If it doesn't
  // we can't check the target features anyhow.
  const FunctionDecl *FD = dyn_cast_or_null<FunctionDecl>(CurCodeDecl);
  if (!FD)
    return;
```

Does this mean this can't be fixed from the frontend?

---

### Comment 6 - efriedma-quic

> Does this mean this can't be fixed from the frontend?

The comment is not really correct.  We currently don't have any attributes to specify target features for C++ global initializers, but they should implicitly use whatever features are specified on the command-line.



---

