# [LLVM][RISCV][ICE] Compiler crash at Assertion Failure at (!From->hasAnyUseOfValue(i) ||, file SelectionDAG.cpp:12171 since a652979b483da6e5a45ebf6428be408de66ac857

**Author:** whileone-sk
**URL:** https://github.com/llvm/llvm-project/issues/158121
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-09-15T01:30:09Z

## Body

Assertion '(!From->hasAnyUseOfValue(i) || From->getValueType(i) == To->getValueType(i)) && "Cannot use this version of ReplaceAllUsesWith!"' failed.: 
It's a failed assertion within the Clang compiler's source code, specifically in the SelectionDAG.cpp.

RUN script red-a48470.sh to reproduce the issue.

source [red-a48470.c](https://github.com/user-attachments/files/22280348/red-a48470.c)
script [red-a48470.sh](https://github.com/user-attachments/files/22280349/red-a48470.sh)

Attached backtrace.
[verbose-log.txt](https://github.com/user-attachments/files/22280789/verbose-log.txt)
[red.c](https://github.com/user-attachments/files/22280586/red.c)


This issue was Found via fuzzer.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Sujay (whileone-sk)

<details>
Assertion '(!From-&gt;hasAnyUseOfValue(i) || From-&gt;getValueType(i) == To-&gt;getValueType(i)) &amp;&amp; "Cannot use this version of ReplaceAllUsesWith!"' failed.: 
It's a failed assertion within the Clang compiler's source code, specifically in the SelectionDAG.cpp.

RUN script red-a48470.sh to reproduce the issue.

source [red-a48470.c](https://github.com/user-attachments/files/22280348/red-a48470.c)
script [red-a48470.sh](https://github.com/user-attachments/files/22280349/red-a48470.sh)

Attached backtrace.
[verbose-log.txt](https://github.com/user-attachments/files/22280789/verbose-log.txt)
[red.c](https://github.com/user-attachments/files/22280586/red.c)
</details>


---

### Comment 2 - whileone-sk

crash backtrace
``` #0 0x0000567d17b46472 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x1f99472)
 #1 0x0000567d17b4316f llvm::sys::RunSignalHandlers() (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x1f9616f)
 #2 0x0000567d17b432bc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000750ae4645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000750ae469eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000750ae469eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000750ae469eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000750ae464527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000750ae46288ff abort ./stdlib/abort.c:81:7
 #9 0x0000750ae462881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000750ae463b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000567d1902e287 llvm::SelectionDAG::ReplaceAllUsesWith(llvm::SDNode*, llvm::SDNode*) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x3481287)
#12 0x0000567d18f0cf6c (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
#13 0x0000567d18f0e12d llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x336112d)
#14 0x0000567d19053b76 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x34a6b76)
#15 0x0000567d19057eaf llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x34aaeaf)
#16 0x0000567d190593ba llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x34ac3ba)
#17 0x0000567d19043917 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x3496917)
#18 0x0000567d16e80b13 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#19 0x0000567d1756f31e llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x19c231e)
#20 0x0000567d1756f761 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x19c2761)
#21 0x0000567d175700b2 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x19c30b2)
#22 0x0000567d17de49fa (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) BackendUtil.cpp:0:0
#23 0x0000567d17de56ca clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x22386ca)
#24 0x0000567d1846959e clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x28bc59e)
#25 0x0000567d1a0b19dc clang::ParseAST(clang::Sema&, bool, bool) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x45049dc)
#26 0x0000567d187abfc7 clang::FrontendAction::Execute() (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x2bfefc7)
#27 0x0000567d1872c3bd clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x2b7f3bd)
#28 0x0000567d188a0a2b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0x2cf3a2b)
#29 0x0000567d167f266b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0xc4566b)
#30 0x0000567d167e7f77 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#31 0x0000567d167ec90a clang_main(int, char**, llvm::ToolContext const&) (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0xc3f90a)
#32 0x0000567d166eb4aa main (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0xb3e4aa)
#33 0x0000750ae462a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#34 0x0000750ae462a28b call_init ./csu/../csu/libc-start.c:128:20
#35 0x0000750ae462a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#36 0x0000567d167e74f5 _start (/home/skothadiya/CIFUZZR/riscv-gnu-toolchain-build/bin/clang-22+0xc3a4f5)
clang: error: unable to execute command: Aborted (core dumped)```

---

