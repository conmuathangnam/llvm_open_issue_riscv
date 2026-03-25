# Assertion failure in X86RegisterInfo.cpp:560

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/25051
**Status:** Closed
**Labels:** backend:X86, bugzilla, llvm:codegen, confirmed, crash
**Closed Date:** 2025-08-11T15:53:46Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [24677](https://llvm.org/bz24677) |
| Version | trunk |
| OS | Linux |
| Attachments | [Preprocessed source file](https://user-images.githubusercontent.com/60944935/143751959-b97f88d3-e7f0-44a0-b5a4-57b244e0a3b0.gz), [Run script](https://user-images.githubusercontent.com/60944935/143751960-806a251c-63bd-4dd8-846d-7f4c0e07ae3f.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @RKSimon |

## Extended Description 

Compiling the gcc testsuite test gcc.c-torture/compile/20031023-4.c at -O0 gives:
```console
clang-3.8: /work/sources/llvm/current/lib/Target/X86/X86RegisterInfo.cpp:560: virtual void llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger*) const: Assertion `(!Is64Bit || isInt<32>((long long)FIOffset + Imm)) && "Requesting 64-bit offset in 32-bit immediate!"' failed.
0  clang-3.8       0x00000000029083d5 llvm::sys::PrintStackTrace(llvm::raw_ostream&) + 44
1  clang-3.8       0x0000000002908752
2  clang-3.8       0x000000000290701b llvm::sys::RunSignalHandlers() + 138
3  clang-3.8       0x0000000002907430
4  libpthread.so.0 0x00007f1802a68430
5  libc.so.6       0x00007f1801c159c8 gsignal + 56
6  libc.so.6       0x00007f1801c1765a abort + 362
7  libc.so.6       0x00007f1801c0e187
8  libc.so.6       0x00007f1801c0e232
9  clang-3.8       0x0000000001cce13f llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::bundle_iterator<llvm::MachineInstr, llvm::ilist_iterator<llvm::MachineInstr> >, int, unsigned int, llvm::RegScavenger*) const + 1199
10 clang-3.8       0x00000000021bd1e4
11 clang-3.8       0x00000000021bca36
12 clang-3.8       0x00000000021ba388
13 clang-3.8       0x000000000212c5c7 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) + 95
14 clang-3.8       0x00000000024e8a51 llvm::FPPassManager::runOnFunction(llvm::Function&) + 315
15 clang-3.8       0x00000000024e8bc8 llvm::FPPassManager::runOnModule(llvm::Module&) + 118
16 clang-3.8       0x00000000024e8ef7
17 clang-3.8       0x00000000024e9572 llvm::legacy::PassManagerImpl::run(llvm::Module&) + 264
18 clang-3.8       0x00000000024e9777 llvm::legacy::PassManager::run(llvm::Module&) + 39
19 clang-3.8       0x0000000002a56e18
20 clang-3.8       0x0000000002a56ecc clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::raw_pwrite_stream*) + 127
21 clang-3.8       0x0000000003215b5e
22 clang-3.8       0x0000000003aaf828 clang::ParseAST(clang::Sema&, bool, bool) + 812
23 clang-3.8       0x0000000002eaa0c3 clang::ASTFrontendAction::ExecuteAction() + 321
24 clang-3.8       0x0000000003214fc0 clang::CodeGenAction::ExecuteAction() + 1326
25 clang-3.8       0x0000000002ea9b67 clang::FrontendAction::Execute() + 135
26 clang-3.8       0x0000000002e693ca clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 782
27 clang-3.8       0x0000000002fa6ec3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 1120
28 clang-3.8       0x0000000001403e12 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 989
29 clang-3.8       0x00000000013fa9ae
30 clang-3.8       0x00000000013fafdf main + 1309
31 libc.so.6       0x00007f1801c01700 __libc_start_main + 240
32 clang-3.8       0x00000000013f91e9 _start + 41
Stack dump:
0.	Program arguments: /home/nickc/work/builds/llvm/current/native/bin/clang-3.8 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name 20031023-4.c -mrelocation-model static -mthread-model posix -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -coverage-file /home/nickc/work/builds/gcc/current/x86_64-pc-linux-gnu/gcc/testsuite/gcc/20031023-4.o -resource-dir /home/nickc/work/builds/llvm/current/native/bin/../lib/clang/3.8.0 -internal-isystem /usr/local/include -internal-isystem /home/nickc/work/builds/llvm/current/native/bin/../lib/clang/3.8.0/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -O0 -w -fdebug-compilation-dir /home/nickc/work/builds/gcc/current/x86_64-pc-linux-gnu/gcc/testsuite/gcc -ferror-limit 19 -fmessage-length 74 -mstackrealign -fobjc-runtime=gcc -fdiagnostics-show-option -o 20031023-4.o -x c /work/sources/gcc/current/gcc/testsuite/gcc.c-torture/compile/20031023-4.c 
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '/work/sources/gcc/current/gcc/testsuite/gcc.c-torture/compile/20031023-4.c'.
4.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@baz'
clang-3.8: error: unable to execute command: Aborted (core dumped)
clang-3.8: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 3.8.0 (trunk 246619)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /work/builds/llvm/current/native/bin
```

## Comments

### Comment 1 - Endilll

Still reproduces as of unreleased Clang 17: https://godbolt.org/z/odoMbK9Ts
Reduced by C-Reduce:
```
int baz() {
  char s[2147483645];
  s[16] = 0;
}
```
Assertion:
```
clang++: /home/tonik/llvm-project/llvm/lib/Target/X86/X86RegisterInfo.cpp:854:
virtual bool llvm::X86RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger *) const:
Assertion `(!Is64Bit || isInt<32>((long long)FIOffset + Imm)) && "Requesting 64-bit offset in 32-bit immediate!"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-16.0.0-assert/bin/clang++ -gdwarf-4 -g -o /tmp/compiler-explorer-compiler202369-989231-tlsxex.rzsyr/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-12.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@baz'
 #0 0x0000558407ccc247 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x38b8247)
 #1 0x0000558407cca0ce llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x38b60ce)
 #2 0x0000558407ccb64d llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x38b764d)
 #3 0x0000558407c49558 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x00007fa25ea54df0 __restore_rt (/lib64/libc.so.6+0x54df0)
 #5 0x00007fa25eaa154c __pthread_kill_implementation (/lib64/libc.so.6+0xa154c)
 #6 0x00007fa25ea54d46 gsignal (/lib64/libc.so.6+0x54d46)
 #7 0x00007fa25ea287f3 abort (/lib64/libc.so.6+0x287f3)
 #8 0x00007fa25ea2871b _nl_load_domain.cold (/lib64/libc.so.6+0x2871b)
 #9 0x00007fa25ea4dce6 (/lib64/libc.so.6+0x4dce6)
#10 0x0000558406982f4e llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, int, unsigned int, llvm::RegScavenger*) const crtstuff.c:0:0
#11 0x00005584071ff764 (anonymous namespace)::PEI::replaceFrameIndices(llvm::MachineBasicBlock*, llvm::MachineFunction&, int&) PrologEpilogInserter.cpp:0:0
#12 0x00005584071fd00c (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#13 0x00005584070e4ccc llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x2cd0ccc)
#14 0x00005584075ebc58 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x31d7c58)
#15 0x00005584075f3c21 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x31dfc21)
#16 0x00005584075ec66c llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x31d866c)
#17 0x0000558408018f98 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x3c04f98)
#18 0x0000558408dd3a19 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) crtstuff.c:0:0
#19 0x0000558409b4c0c0 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x57380c0)
#20 0x0000558408768f60 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x4354f60)
#21 0x00005584086dc89f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x42c889f)
#22 0x000055840883a793 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x4426793)
#23 0x000055840589524b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x148124b)
#24 0x0000558405891a6b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#25 0x00005584085505c2 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_1>(long) Job.cpp:0:0
#26 0x0000558407c4929b llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x383529b)
#27 0x000055840854ff20 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x413bf20)
#28 0x000055840851399e clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x40ff99e)
#29 0x0000558408513c4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x40ffc4e)
#30 0x0000558408532350 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x411e350)
#31 0x0000558405890b06 clang_main(int, char**) (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x147cb06)
#32 0x00007fa25ea3feb0 __libc_start_call_main (/lib64/libc.so.6+0x3feb0)
#33 0x00007fa25ea3ff60 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3ff60)
#34 0x000055840588db9e _start (/opt/compiler-explorer/clang-16.0.0-assert/bin/clang+++0x1479b9e)
clang-16: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 2 - shafik

Possibly related: https://github.com/llvm/llvm-project/issues/57317

We have mismatches with assumptions on array sizes in many places.

---

