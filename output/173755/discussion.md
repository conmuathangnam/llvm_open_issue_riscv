# [RISC-V] Clang crashes when running pass 'Induction Variable Users' on basic block

**Author:** ams-x9v2b7kq
**URL:** https://github.com/llvm/llvm-project/issues/173755
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-01-06T13:51:42Z

## Body

## [RISC-V] Clang crashes when running pass 'Induction Variable Users' on basic block

### code link: https://godbolt.org/z/93TYxfr1r
```c++
char a, h;
int i, j, k;
bool l;
extern bool m[][2];
extern char n[][4];
extern bool o[][1][1][1];
extern char p[];
#include <vector>
void q() {
  for (short b = 0; b < 24; b++) {
    for (char c = 1; c; c += 2)
      for (int d = 0; d < 4; d += 2)
        j = std::min(h ? m[d][d] : 0, m[b][b] ? 0 : o[b][b][c][b]);
    for (char e = 0; e < 24; e += 2)
      for (bool f = 0; f < 1; f = 1) {
        for (long g = 1; g < 20; g++) {
          l = n[b][g] ? n[b][e * 4 + g] : n[b][g * 5];
          k = a ?: m[2][1];
        }
        p[+e] &= i;
      }
  }
}

```

```
fatal error: error in backend: Cannot implicitly convert a scalable size to a fixed-width size in `TypeSize::operator ScalarTy()`
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/2025-compiler/gcc-16-20251221/bin/clang++ --gcc-toolchain=/home/2025-compiler/gcc-16-20251221 -O3 -march=rv64gcv_zvfh -Werror=uninitialized -Werror=implicitly-unsigned-literal -Wno-pass-failed -c func.cpp -o func.o
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'func.cpp'.
4.	Running pass 'Loop Pass Manager' on function '@_Z4testv'
5.	Running pass 'Induction Variable Users' on basic block '%865'
 #0 0x000055bc626ff852 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de9852)
 #1 0x000055bc626fd2fc llvm::sys::CleanupOnSignal(unsigned long) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de72fc)
 #2 0x000055bc6263deb7 llvm::CrashRecoveryContext::HandleExit(int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1d27eb7)
 #3 0x000055bc626f3c72 llvm::sys::Process::Exit(int, bool) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1dddc72)
 #4 0x000055bc6158428f LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x000055bc626483c3 llvm::report_fatal_error(llvm::Twine const&, bool) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1d323c3)
 #6 0x000055bc6264857f (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1d3257f)
 #7 0x000055bc6191162d (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xffb62d)
 #8 0x000055bc61911c5f llvm::ScalarEvolution::getConstantMultipleImpl(llvm::SCEV const*, llvm::Instruction const*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xffbc5f)
 #9 0x000055bc619111ef llvm::ScalarEvolution::getConstantMultiple(llvm::SCEV const*, llvm::Instruction const*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xffb1ef)
#10 0x000055bc6191159e llvm::ScalarEvolution::getMinTrailingZeros(llvm::SCEV const*, llvm::Instruction const*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xffb59e)
#11 0x000055bc6191e47d llvm::ScalarEvolution::getRangeRef(llvm::SCEV const*, llvm::ScalarEvolution::RangeSignHint, unsigned int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x100847d)
#12 0x000055bc6193590e llvm::ScalarEvolution::getMinusSCEV(llvm::SCEV const*, llvm::SCEV const*, llvm::SCEV::NoWrapFlags, unsigned int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x101f90e)
#13 0x000055bc6194384d llvm::ScalarEvolution::createNodeForSelectOrPHIInstWithICmpInstCond(llvm::Type*, llvm::ICmpInst*, llvm::Value*, llvm::Value*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x102d84d)
#14 0x000055bc61943dd9 llvm::ScalarEvolution::createNodeForSelectOrPHI(llvm::Value*, llvm::Value*, llvm::Value*, llvm::Value*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x102ddd9)
#15 0x000055bc6191c6e1 llvm::ScalarEvolution::createSCEVIter(llvm::Value*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x10066e1)
#16 0x000055bc6669c58d llvm::IVUsers::AddUsersIfInteresting(llvm::Instruction*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x5d8658d)
#17 0x000055bc6669c84e llvm::IVUsers::AddUsersIfInteresting(llvm::Instruction*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x5d8684e)
#18 0x000055bc6669c84e llvm::IVUsers::AddUsersIfInteresting(llvm::Instruction*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x5d8684e)
#19 0x000055bc6669c84e llvm::IVUsers::AddUsersIfInteresting(llvm::Instruction*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x5d8684e)
#20 0x000055bc6669ca78 llvm::IVUsers::IVUsers(llvm::Loop*, llvm::AssumptionCache*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::ScalarEvolution*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x5d86a78)
#21 0x000055bc6669cbe7 llvm::IVUsersWrapperPass::runOnLoop(llvm::Loop*, llvm::LPPassManager&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x5d86be7)
#22 0x000055bc666f8eba llvm::LPPassManager::runOnFunction(llvm::Function&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x5de2eba)
#23 0x000055bc6214d849 llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1837849)
#24 0x000055bc6214d9f4 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x18379f4)
#25 0x000055bc6214cd77 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1836d77)
#26 0x000055bc6298537b clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x206f37b)
#27 0x000055bc62f650a9 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x264f0a9)
#28 0x000055bc649ab08c clang::ParseAST(clang::Sema&, bool, bool) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x409508c)
#29 0x000055bc632a0d8e clang::FrontendAction::Execute() (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x298ad8e)
#30 0x000055bc632226d3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x290c6d3)
#31 0x000055bc633845af clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x2a6e5af)
#32 0x000055bc61586a7d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc70a7d)
#33 0x000055bc6157e25a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x000055bc6157e2b5 int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#35 0x000055bc62fe15bd void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x000055bc6263ddb7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1d27db7)
#37 0x000055bc62fe1cb1 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x000055bc62fa4ed6 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268eed6)
#39 0x000055bc62fa5f39 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268ff39)
#40 0x000055bc62fb784c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x26a184c)
#41 0x000055bc61583111 clang_main(int, char**, llvm::ToolContext const&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc6d111)
#42 0x000055bc614851da main (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xb6f1da)
#43 0x00007ff5b7b4a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#44 0x00007ff5b7b4a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#45 0x000055bc6157dc85 _start (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc67c85)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 22.0.0git
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /home/2025-compiler/gcc-16-20251221/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/func-5b8dd7.cpp
clang++: note: diagnostic msg: /tmp/func-5b8dd7.sh
clang++: note: diagnostic msg:

********************
```

## Comments

### Comment 1 - topperc

An assertions build fails in the vectorizer. https://godbolt.org/z/KhE3193o7

This hits the same assertion as #173761 does on an asserts build.

---

### Comment 2 - fhahn

IIUC this should be fixed after landing https://github.com/llvm/llvm-project/pull/174416.

Please re-open with an updated reproducer if it still is an issue.

When fuzzing/reducing would probably be good to build with assertions/run verifier

---

