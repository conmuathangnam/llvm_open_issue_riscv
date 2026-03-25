# Large _BitInt constant leads to clang crash

**Author:** pascal-cuoq
**URL:** https://github.com/llvm/llvm-project/issues/162322
**Status:** Open
**Labels:** backend:X86, c23, confirmed, crash, llvm:SelectionDAG

## Body

In clang 21.1.0 and in CE's "trunk" as of today, the following program leads to a crash (https://gcc.godbolt.org/z/8s3zGxf6q ):
```c
#include <stdio.h>
#include <fenv.h>

int main(void) {
    // Enable access to the floating-point environment
    #pragma STDC FENV_ACCESS ON

    // Set rounding mode to downward (toward -∞)
    if (fesetround(FE_DOWNWARD) != 0) {
        perror("Failed to set rounding mode");
        return 1;
    }

    double d = (double) 999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999wb;

    printf("Rounding mode set to downward.\n");
    printf("Converted to double: %.17g\n", d);
}
```
Commandline options: `-std=c23 -frounding-math -lm`

Backtrace in "trunk":
```
fatal error: error in backend: unsupported library call operation
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c23 -frounding-math -lm <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@main'
 #0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bfa6c8)
 #1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bf809c)
 #2 0x0000000003b48913 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b48913)
 #3 0x0000000003bef8ee llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bef8ee)
 #4 0x0000000000da3edb LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003b52843 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b52843)
 #6 0x0000000003b529d9 (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b529d9)
 #7 0x0000000005218e61 llvm::TargetLowering::makeLibCall(llvm::SelectionDAG&, llvm::RTLIB::Libcall, llvm::EVT, llvm::ArrayRef<llvm::SDValue>, llvm::TargetLowering::MakeLibCallOptions, llvm::SDLoc const&, llvm::SDValue) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x5218e61)
 #8 0x00000000052fc154 llvm::DAGTypeLegalizer::ExpandIntOp_XINT_TO_FP(llvm::SDNode*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x52fc154)
 #9 0x00000000052fee78 llvm::DAGTypeLegalizer::ExpandIntegerOperand(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x52fee78)
#10 0x000000000523e5c9 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x523e5c9)
#11 0x000000000523ea31 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x523ea31)
#12 0x00000000051ac3f2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x51ac3f2)
#13 0x00000000051af546 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x51af546)
#14 0x00000000051b0d46 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x51b0d46)
#15 0x000000000519d7c1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x519d7c1)
#16 0x0000000002fd6b0d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x2fd6b0d)
#17 0x0000000003550fd2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3550fd2)
#18 0x0000000003551261 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3551261)
#19 0x0000000003552ba9 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3552ba9)
#20 0x0000000003ea98f0 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3ea98f0)
#21 0x000000000454906c clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x454906c)
#22 0x000000000618f19c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x618f19c)
#23 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x454985d)
#24 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x485b83a)
#25 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x47d655b)
#26 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x494e513)
#27 0x0000000000da5f25 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xda5f25)
#28 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x00000000045bb869 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b48813)
#31 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x457eddd)
#33 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x457fd91)
#34 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x458d29c)
#35 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xda2751)
#36 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xc526f4)
#37 0x00007a4a0ee29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x00007a4a0ee29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xd9d8c5)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Pascal Cuoq (pascal-cuoq)

<details>
In clang 21.1.0 and in CE's "trunk" as of today, the following program leads to a crash (https://gcc.godbolt.org/z/8s3zGxf6q ):
```
#include &lt;stdio.h&gt;
#include &lt;fenv.h&gt;

int main(void) {
    // Enable access to the floating-point environment
    #pragma STDC FENV_ACCESS ON

    // Set rounding mode to downward (toward -∞)
    if (fesetround(FE_DOWNWARD) != 0) {
        perror("Failed to set rounding mode");
        return 1;
    }

    double d = (double) 999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999wb;

    printf("Rounding mode set to downward.\n");
    printf("Converted to double: %.17g\n", d);
}
```
Commandline options: `-std=c23 -frounding-math -lm`

Backtrace in "trunk":
```
fatal error: error in backend: unsupported library call operation
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-21.1.0/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c23 -frounding-math -lm &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->main'
 #<!-- -->0 0x0000000003bfa6c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bfa6c8)
 #<!-- -->1 0x0000000003bf809c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bf809c)
 #<!-- -->2 0x0000000003b48913 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b48913)
 #<!-- -->3 0x0000000003bef8ee llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3bef8ee)
 #<!-- -->4 0x0000000000da3edb LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003b52843 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b52843)
 #<!-- -->6 0x0000000003b529d9 (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b529d9)
 #<!-- -->7 0x0000000005218e61 llvm::TargetLowering::makeLibCall(llvm::SelectionDAG&amp;, llvm::RTLIB::Libcall, llvm::EVT, llvm::ArrayRef&lt;llvm::SDValue&gt;, llvm::TargetLowering::MakeLibCallOptions, llvm::SDLoc const&amp;, llvm::SDValue) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x5218e61)
 #<!-- -->8 0x00000000052fc154 llvm::DAGTypeLegalizer::ExpandIntOp_XINT_TO_FP(llvm::SDNode*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x52fc154)
 #<!-- -->9 0x00000000052fee78 llvm::DAGTypeLegalizer::ExpandIntegerOperand(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x52fee78)
#<!-- -->10 0x000000000523e5c9 llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x523e5c9)
#<!-- -->11 0x000000000523ea31 llvm::SelectionDAG::LegalizeTypes() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x523ea31)
#<!-- -->12 0x00000000051ac3f2 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x51ac3f2)
#<!-- -->13 0x00000000051af546 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x51af546)
#<!-- -->14 0x00000000051b0d46 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x51b0d46)
#<!-- -->15 0x000000000519d7c1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x519d7c1)
#<!-- -->16 0x0000000002fd6b0d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x2fd6b0d)
#<!-- -->17 0x0000000003550fd2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3550fd2)
#<!-- -->18 0x0000000003551261 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3551261)
#<!-- -->19 0x0000000003552ba9 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3552ba9)
#<!-- -->20 0x0000000003ea98f0 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3ea98f0)
#<!-- -->21 0x000000000454906c clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x454906c)
#<!-- -->22 0x000000000618f19c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x618f19c)
#<!-- -->23 0x000000000454985d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x454985d)
#<!-- -->24 0x000000000485b83a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x485b83a)
#<!-- -->25 0x00000000047d655b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x47d655b)
#<!-- -->26 0x000000000494e513 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x494e513)
#<!-- -->27 0x0000000000da5f25 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xda5f25)
#<!-- -->28 0x0000000000d9de2d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x00000000045bb869 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000000003b48813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x3b48813)
#<!-- -->31 0x00000000045bba89 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x000000000457eddd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x457eddd)
#<!-- -->33 0x000000000457fd91 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x457fd91)
#<!-- -->34 0x000000000458d29c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0x458d29c)
#<!-- -->35 0x0000000000da2751 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xda2751)
#<!-- -->36 0x0000000000c526f4 main (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xc526f4)
#<!-- -->37 0x00007a4a0ee29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x00007a4a0ee29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000d9d8c5 _start (/opt/compiler-explorer/clang-21.1.0/bin/clang+0xd9d8c5)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```
</details>


---

### Comment 2 - RKSimon

```ll
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define double @reduced() {
  %conv = call double @llvm.experimental.constrained.sitofp.f64.i1656(i1656 999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999, metadata !"round.dynamic", metadata !"fpexcept.strict")
  ret double %conv
}
```

---

