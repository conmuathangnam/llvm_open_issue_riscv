# [RISCV] Crashed at -O1: Assertion `!Nodes.empty() && "Nodes list is empty!"' failed.

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/166759
**Status:** Open
**Labels:** crash, llvm:analysis

## Body

This code crashed at `-O1 -ffixed-x5 -msave-restore`:

```C
int a;
void b();
int c() {
  if (a)
    b();
  return 0;
}
```

Crash:

```sh
clang-22: /data/yunboni/utils/compilers/repos/llvm-project/llvm/include/llvm/Support/GenericDomTree.h:563: NodeT *llvm::DominatorTreeBase<llvm:
:MachineBasicBlock, false>::findNearestCommonDominator(iterator_range<IteratorTy>) const [NodeT = llvm::MachineBasicBlock, IsPostDom = false, I
teratorTy = llvm::MachineBasicBlock **]: Assertion `!Nodes.empty() && "Nodes list is empty!"' failed. 
```

Backtrace:

```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -O1 -ffixed-x5 -msave-restore <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Shrink Wrapping analysis' on function '@c'
 #0 0x0000000003d21258 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d21258)
 #1 0x0000000003d1ec2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d1ec2c)
 #2 0x0000000003c649b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000731b01442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000332d120 (anonymous namespace)::ShrinkWrapImpl::run(llvm::MachineFunction&) ShrinkWrap.cpp:0:0
 #5 0x000000000332efba (anonymous namespace)::ShrinkWrapLegacy::runOnMachineFunction(llvm::MachineFunction&) (.part.0) ShrinkWrap.cpp:0:0
 #6 0x00000000030fd8ad llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x30fd8ad)
 #7 0x0000000003671de2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3671de2)
 #8 0x0000000003672071 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3672071)
 #9 0x0000000003673953 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3673953)
#10 0x0000000003fbe5a2 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fbe5a2)
#11 0x00000000045f65eb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45f65eb)
#12 0x0000000006276b1c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6276b1c)
#13 0x00000000045f7145 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x45f7145)
#14 0x00000000048f5e6a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x48f5e6a)
#15 0x00000000048754ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x48754ab)
#16 0x00000000049edccb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x49edccb)
#17 0x0000000000de0165 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xde0165)
#18 0x0000000000dd80cb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#19 0x0000000000dd816d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#20 0x00000000046653d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#21 0x0000000003c64dd3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c64dd3)
#22 0x00000000046655f9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#23 0x00000000046281f2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x46281f2)
#24 0x00000000046290d1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x46290d1)
#25 0x0000000004631f7c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4631f7c)
#26 0x0000000000ddcb99 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xddcb99)
#27 0x0000000000c87324 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc87324)
#28 0x0000731b01429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#29 0x0000731b01429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#30 0x0000000000dd7b65 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdd7b65)
```

Compiler Explorer: https://godbolt.org/z/MTMsdnjvW

Compiler version:

```sh
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 4c2a9c4ba3796799032c12673510279d51c65370)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/yunboni/utils/compilers/llvm_latest_rvv/bin
Build config: +assertions
```

## Comments

### Comment 1 - sathvikreddy853

Hey @EugeneZelenko, I would like to work on this issue.

---

