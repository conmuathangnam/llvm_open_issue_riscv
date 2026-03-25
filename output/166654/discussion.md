# Crash in AMDGPU backend

**Author:** laura240406
**URL:** https://github.com/llvm/llvm-project/issues/166654
**Status:** Open
**Labels:** backend:AMDGPU, crash, llvm:SelectionDAG

## Body

I've played around with compiling the ChaCha20 code from Wikipedia for multiple LLVM backends and the command `clang -c -o main.o main.c -O2 -target amdgcn` crashes with:

```
fatal error: error in backend: Cannot select: 0x3bd6f220: i32,ch = load<(load (s32) from %ir.8, !tbaa !3)> # D:1 0x3bd09580, 0x3bd6fae0, undef:i64
  0x3bd6fae0: i64 = add nuw # D:1 0x3bd6f0d0, Constant:i64<16>
    0x3bd6f0d0: i64 = build_pair # D:1 0x3bd6ee30, 0x3bd6ef10
      0x3bd6ee30: i32,ch = CopyFromReg # D:1 0x3bd09580, Register:i32 %71
      0x3bd6ef10: i32,ch = CopyFromReg # D:1 0x3bd09580, Register:i32 %72
In function: chacha_block
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -c -o main.o main.c -O2 -target amdgcn
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'CallGraph Pass Manager' on module 'main.c'.
4.	Running pass 'AMDGPU DAG->DAG Pattern Instruction Selection' on function '@chacha_block'
 #0 0x00007ff65426f50b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xc6f50b)
 #1 0x00007ff65426d0b4 llvm::sys::CleanupOnSignal(unsigned long) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xc6d0b4)
 #2 0x00007ff654158902 llvm::CrashRecoveryContext::HandleExit(int) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xb58902)
 #3 0x00007ff65426400e llvm::sys::Process::Exit(int, bool) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xc6400e)
 #4 0x0000000000414467 (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x414467)
 #5 0x00007ff6541737fc llvm::report_fatal_error(llvm::Twine const&, bool) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xb737fc)
 #6 0x00007ff654f5a19d llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x195a19d)
 #7 0x00007ff654f5d262 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x195d262)
 #8 0x00007ff654f55b19 llvm::SelectionDAGISel::DoInstructionSelection() (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x1955b19)
 #9 0x00007ff654f628d5 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x19628d5)
#10 0x00007ff654f653d0 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x19653d0)
#11 0x00007ff654f67588 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x1967588)
#12 0x00007ff654f556c9 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x19556c9)
#13 0x00007ff65483c888 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x123c888)
#14 0x00007ff654459ed9 llvm::FPPassManager::runOnFunction(llvm::Function&) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xe59ed9)
#15 0x00007ff6562de672 (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x2cde672)
#16 0x00007ff6544593f4 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xe593f4)
#17 0x00007ff65f1719b1 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x23719b1)
#18 0x00007ff65f54c699 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x274c699)
#19 0x00007ff65da4879c clang::ParseAST(clang::Sema&, bool, bool) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0xc4879c)
#20 0x00007ff65ff60dba clang::FrontendAction::Execute() (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x3160dba)
#21 0x00007ff65fed72ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x30d72ae)
#22 0x00007ff660002857 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x3202857)
#23 0x0000000000416aa9 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x416aa9)
#24 0x000000000040e863 (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x40e863)
#25 0x00007ff65fab2d59 (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2cb2d59)
#26 0x00007ff654158813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xb58813)
#27 0x00007ff65fab3494 (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2cb3494)
#28 0x00007ff65fa7499b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2c7499b)
#29 0x00007ff65fa75962 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2c75962)
#30 0x00007ff65fa8968c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2c8968c)
#31 0x00000000004131d4 clang_main(int, char**, llvm::ToolContext const&) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x4131d4)
#32 0x000000000040e273 main (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x40e273)
#33 0x00007ff65303cbd7 __libc_start_call_main (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x2abd7)
#34 0x00007ff65303cc95 __libc_start_main@GLIBC_2.2.5 (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x2ac95)
#35 0x000000000040e2b1 _start (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x40e2b1)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 21.1.1
Target: amdgcn
Thread model: posix
InstalledDir: /gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/main-77f709.c
clang: note: diagnostic msg: /tmp/main-77f709.sh
clang: note: diagnostic msg:

********************
```

It asked me to attach the following files:
[main-77f709.c](https://github.com/user-attachments/files/23376697/main-77f709.c)
[main-77f709.sh](https://github.com/user-attachments/files/23376709/main-77f709.sh)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-amdgpu

Author: Laura Kirsch (jakiki6)

<details>
I've played around with compiling the ChaCha20 code from Wikipedia for multiple LLVM backends and the command `clang -c -o main.o main.c -O2 -target amdgcn` crashes with:

```
fatal error: error in backend: Cannot select: 0x3bd6f220: i32,ch = load&lt;(load (s32) from %ir.8, !tbaa !3)&gt; # D:1 0x3bd09580, 0x3bd6fae0, undef:i64
  0x3bd6fae0: i64 = add nuw # D:1 0x3bd6f0d0, Constant:i64&lt;16&gt;
    0x3bd6f0d0: i64 = build_pair # D:1 0x3bd6ee30, 0x3bd6ef10
      0x3bd6ee30: i32,ch = CopyFromReg # D:1 0x3bd09580, Register:i32 %71
      0x3bd6ef10: i32,ch = CopyFromReg # D:1 0x3bd09580, Register:i32 %72
In function: chacha_block
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -c -o main.o main.c -O2 -target amdgcn
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'CallGraph Pass Manager' on module 'main.c'.
4.	Running pass 'AMDGPU DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->chacha_block'
 #<!-- -->0 0x00007ff65426f50b llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xc6f50b)
 #<!-- -->1 0x00007ff65426d0b4 llvm::sys::CleanupOnSignal(unsigned long) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xc6d0b4)
 #<!-- -->2 0x00007ff654158902 llvm::CrashRecoveryContext::HandleExit(int) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xb58902)
 #<!-- -->3 0x00007ff65426400e llvm::sys::Process::Exit(int, bool) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xc6400e)
 #<!-- -->4 0x0000000000414467 (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x414467)
 #<!-- -->5 0x00007ff6541737fc llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xb737fc)
 #<!-- -->6 0x00007ff654f5a19d llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x195a19d)
 #<!-- -->7 0x00007ff654f5d262 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x195d262)
 #<!-- -->8 0x00007ff654f55b19 llvm::SelectionDAGISel::DoInstructionSelection() (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x1955b19)
 #<!-- -->9 0x00007ff654f628d5 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x19628d5)
#<!-- -->10 0x00007ff654f653d0 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x19653d0)
#<!-- -->11 0x00007ff654f67588 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x1967588)
#<!-- -->12 0x00007ff654f556c9 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x19556c9)
#<!-- -->13 0x00007ff65483c888 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x123c888)
#<!-- -->14 0x00007ff654459ed9 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xe59ed9)
#<!-- -->15 0x00007ff6562de672 (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0x2cde672)
#<!-- -->16 0x00007ff6544593f4 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xe593f4)
#<!-- -->17 0x00007ff65f1719b1 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x23719b1)
#<!-- -->18 0x00007ff65f54c699 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x274c699)
#<!-- -->19 0x00007ff65da4879c clang::ParseAST(clang::Sema&amp;, bool, bool) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0xc4879c)
#<!-- -->20 0x00007ff65ff60dba clang::FrontendAction::Execute() (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x3160dba)
#<!-- -->21 0x00007ff65fed72ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x30d72ae)
#<!-- -->22 0x00007ff660002857 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x3202857)
#<!-- -->23 0x0000000000416aa9 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x416aa9)
#<!-- -->24 0x000000000040e863 (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x40e863)
#<!-- -->25 0x00007ff65fab2d59 (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2cb2d59)
#<!-- -->26 0x00007ff654158813 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/gnu/store/w4wnzqzh26gyq8j95m806a6kdapr8cdh-llvm-21.1.1/lib/libLLVM.so.21.1+0xb58813)
#<!-- -->27 0x00007ff65fab3494 (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2cb3494)
#<!-- -->28 0x00007ff65fa7499b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2c7499b)
#<!-- -->29 0x00007ff65fa75962 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2c75962)
#<!-- -->30 0x00007ff65fa8968c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/lib/libclang-cpp.so.21.1+0x2c8968c)
#<!-- -->31 0x00000000004131d4 clang_main(int, char**, llvm::ToolContext const&amp;) (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x4131d4)
#<!-- -->32 0x000000000040e273 main (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x40e273)
#<!-- -->33 0x00007ff65303cbd7 __libc_start_call_main (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x2abd7)
#<!-- -->34 0x00007ff65303cc95 __libc_start_main@<!-- -->GLIBC_2.2.5 (/gnu/store/yj053cys0724p7vs9kir808x7fivz17m-glibc-2.41/lib/libc.so.6+0x2ac95)
#<!-- -->35 0x000000000040e2b1 _start (/gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin/clang-21+0x40e2b1)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 21.1.1
Target: amdgcn
Thread model: posix
InstalledDir: /gnu/store/bhqfbm7psfynv0r2l9wqbjzj1xi2j0k2-clang-21.1.1/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/main-77f709.c
clang: note: diagnostic msg: /tmp/main-77f709.sh
clang: note: diagnostic msg:

********************
```

It asked me to attach the following files:
[main-77f709.c](https://github.com/user-attachments/files/23376697/main-77f709.c)
[main-77f709.sh](https://github.com/user-attachments/files/23376709/main-77f709.sh)
</details>


---

### Comment 2 - arsenm

This is trying to codegen flat addressing on the default dummy target, which isn't meaningful. This isn't supposed to work (we should have a real diagnostic for this though, instead of hitting the backend error). You need to use -target amdgcn-amd-amdhsa and -mcpu with a real target 

---

