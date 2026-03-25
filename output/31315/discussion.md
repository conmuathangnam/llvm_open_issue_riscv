# Assertion failure in X86RegisterInfo.cpp

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/31315
**Status:** Closed
**Labels:** backend:X86, bugzilla, llvm:codegen, confirmed, crash
**Closed Date:** 2025-12-15T09:16:40Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [31967](https://llvm.org/bz31967) |
| Version | trunk |
| OS | Linux |
| Attachments | [Preprocessed test case.](https://user-images.githubusercontent.com/60944935/143755104-0d5f4f0d-1b35-41bd-b0b6-5038ba81a605.gz), [Driver script for the test case](https://user-images.githubusercontent.com/60944935/143755105-082d8f21-ad04-4bfe-8abd-c0bfb7238d70.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @RKSimon |

## Extended Description 


Compiling the attached test case with the attached command line produces the following assertion failure when running clang built from trunk (15 Feb 2017):
```
clang-4.0: /work/sources/llvm/current/lib/Target/X86/X86RegisterInfo.cpp:735: virtual void llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger*) const: Assertion `(!Is64Bit || isInt<32>((long long)FIOffset + Imm)) && "Requesting 64-bit offset in 32-bit immediate!"' failed.
```
The stack backtrace looks like this:
```
#0 0x00000000034cc20c llvm::sys::PrintStackTrace(llvm::raw_ostream&) /work/sources/llvm/current/lib/Support/Unix/Signals.inc:402:0
#1 0x00000000034cc29f PrintStackTraceSignalHandler(void*) /work/sources/llvm/current/lib/Support/Unix/Signals.inc:466:0
#2 0x00000000034ca802 llvm::sys::RunSignalHandlers() /work/sources/llvm/current/lib/Support/Signals.cpp:44:0
#3 0x00000000034cbb83 SignalHandler(int) /work/sources/llvm/current/lib/Support/Unix/Signals.inc:256:0
#4 0x00007ff04c73e5c0 __restore_rt (/lib64/libpthread.so.0+0x115c0)
#5 0x00007ff04b2a691f __GI_raise /usr/src/debug/glibc-2.24-33-ge9e69e4/signal/../sysdeps/unix/sysv/linux/raise.c:58:0
#6 0x00007ff04b2a851a __GI_abort /usr/src/debug/glibc-2.24-33-ge9e69e4/stdlib/abort.c:91:0
#7 0x00007ff04b29eda7 __assert_fail_base /usr/src/debug/glibc-2.24-33-ge9e69e4/assert/assert.c:92:0
#8 0x00007ff04b29ee52 (/lib64/libc.so.6+0x2de52)
#9 0x00000000024b8794 llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, int, unsigned int, llvm::RegScavenger*) const /work/sources/llvm/current/lib/Target/X86/X86RegisterInfo.cpp:734:0
#10 0x0000000002b9ec6a (anonymous namespace)::PEI::replaceFrameIndices(llvm::MachineBasicBlock*, llvm::MachineFunction&, int&) /work/sources/llvm/current/lib/CodeGen/PrologEpilogInserter.cpp:1128:0
#11 0x0000000002b9e50d (anonymous namespace)::PEI::replaceFrameIndices(llvm::MachineFunction&) /work/sources/llvm/current/lib/CodeGen/PrologEpilogInserter.cpp:1032:0
#12 0x0000000002b9b0ea (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&) /work/sources/llvm/current/lib/CodeGen/PrologEpilogInserter.cpp:222:0
#13 0x0000000002ad4554 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) /work/sources/llvm/current/lib/CodeGen/MachineFunctionPass.cpp:62:0
#14 0x0000000002e90afb llvm::FPPassManager::runOnFunction(llvm::Function&) /work/sources/llvm/current/lib/IR/LegacyPassManager.cpp:1513:0
#15 0x0000000002e90c72 llvm::FPPassManager::runOnModule(llvm::Module&) /work/sources/llvm/current/lib/IR/LegacyPassManager.cpp:1534:0
#16 0x0000000002e90fbf (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /work/sources/llvm/current/lib/IR/LegacyPassManager.cpp:1590:0
#17 0x0000000002e91673 llvm::legacy::PassManagerImpl::run(llvm::Module&) /work/sources/llvm/current/lib/IR/LegacyPassManager.cpp:1693:0
#18 0x0000000002e9186b llvm::legacy::PassManager::run(llvm::Module&) /work/sources/llvm/current/lib/IR/LegacyPassManager.cpp:1725:0
#19 0x000000000375dc11 (anonymous namespace)::EmitAssemblyHelper::EmitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >) /work/sources/llvm/current/tools/clang/lib/CodeGen/BackendUtil.cpp:727:0
#20 0x000000000375f48d clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::DataLayout const&, llvm::Module*, clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >) /work/sources/llvm/current/tools/clang/lib/CodeGen/BackendUtil.cpp:997:0
#21 0x000000000415fc85 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /work/sources/llvm/current/tools/clang/lib/CodeGen/CodeGenAction.cpp:243:0
#22 0x0000000004d50f21 clang::ParseAST(clang::Sema&, bool, bool) /work/sources/llvm/current/tools/clang/lib/Parse/ParseAST.cpp:161:0
#23 0x0000000003d0e575 clang::ASTFrontendAction::ExecuteAction() /work/sources/llvm/current/tools/clang/lib/Frontend/FrontendAction.cpp:568:0
#24 0x000000000415de26 clang::CodeGenAction::ExecuteAction() /work/sources/llvm/current/tools/clang/lib/CodeGen/CodeGenAction.cpp:973:0
#25 0x0000000003d0dfe3 clang::FrontendAction::Execute() /work/sources/llvm/current/tools/clang/lib/Frontend/FrontendAction.cpp:472:0
#26 0x0000000003cb39a5 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /work/sources/llvm/current/tools/clang/lib/Frontend/CompilerInstance.cpp:951:0
#27 0x0000000003e3b18b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /work/sources/llvm/current/tools/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:249:0
#28 0x0000000001868da7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /work/sources/llvm/current/tools/clang/tools/driver/cc1_main.cpp:221:0
#29 0x000000000185eb5f ExecuteCC1Tool(llvm::ArrayRef<char const*>, llvm::StringRef) /work/sources/llvm/current/tools/clang/tools/driver/driver.cpp:299:0
#30 0x000000000185f6ee main /work/sources/llvm/current/tools/clang/tools/driver/driver.cpp:380:0
#31 0x00007ff04b291401 __libc_start_main /usr/src/debug/glibc-2.24-33-ge9e69e4/csu/../csu/libc-start.c:323:0
#32 0x000000000185c55a _start (/work/builds/llvm/current/native/bin/clang-4.0+0x185c55a)
Stack dump:
0.	Program arguments: /work/builds/llvm/current/native/bin/clang-4.0 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all -disable-free -main-file-name 20031023-4.c -mrelocation-model static -mthread-model posix -mdisable-fp-elim -fmath-errno -masm-verbose -mconstructor-aliases -munwind-tables -fuse-init-array -target-cpu x86-64 -dwarf-column-info -debugger-tuning=gdb -coverage-notes-file /work/builds/llvm/current/native/tests/20031023-4.gcno -resource-dir /work/builds/llvm/current/native/bin/../lib/clang/5.0.0 -internal-isystem /usr/local/include -internal-isystem /work/builds/llvm/current/native/bin/../lib/clang/5.0.0/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -O0 -w -fdebug-compilation-dir /work/builds/llvm/current/native/tests -ferror-limit 19 -fmessage-length 130 -fobjc-runtime=gcc -fdiagnostics-show-option -o 20031023-4.o -x c /work/sources/gcc/current/gcc/testsuite/gcc.c-torture/compile/20031023-4.c 
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '/work/sources/gcc/current/gcc/testsuite/gcc.c-torture/compile/20031023-4.c'.
4.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@baz'
```

## Comments

### Comment 1 - Endilll

Preprocessed reproducer:
```
extern void abort (void);

int __attribute__((noinline))
foo (const char *s)
{
  if (!s)
    return 1;
  if (s[0] != 'a')
    abort ();
  s += 0x80000000UL - 1;
  if (s[0] != 'b')
    abort ();
  return 0;
}

int (*fn) (const char *) = foo;

int __attribute__((noinline))
bar (void)
{
  char s[0x80000000UL];
  s[0] = 'a';
  s[0x80000000UL - 1] = 'b';
  foo (s);
  foo (s);
  return 0;
}

int __attribute__((noinline))
baz (long i)
{
  if (i)
    return fn (0);
  else
    {
      char s[0x80000000UL];
      s[0] = 'a';
      s[0x80000000UL - 1] = 'b';
      foo (s);
      foo (s);
      return fn (0);
    }
}

int
main (void)
{
  if (bar ())
    abort ();
  if (baz (0) != 1)
    abort ();
  if (baz (1) != 1)
    abort ();
  return 0;
}
```

---

### Comment 2 - Endilll

Reduced by C-Reduce in C mode:
```
int baz() {
  char s[2147483648];
  s[80000000] = 'b';
}
```
Unlike unreduced test case, the one above doesn't crash in C++ mode. Here's reduction for C++ mode:
```
void baz() {
  int i;
  char s[2147483648];
  s[80000000] = 'b';
}
```

---

### Comment 3 - Endilll

Reduced test case for C mode doesn't crash in C++ mode since Clang 9: https://godbolt.org/z/Gfb9GPc4E
Crash in C++ mode: https://godbolt.org/z/W4aYrPYKn
```
clang++: /root/llvm-project/llvm/lib/Target/X86/X86RegisterInfo.cpp:901:
virtual bool llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger*) const:
Assertion `(!Is64Bit || isInt<32>((long long)FIOffset + Imm)) && "Requesting 64-bit offset in 32-bit immediate!"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@_Z3bazv'
 #0 0x000055a513c9ac1a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a9ac1a)
 #1 0x000055a513c9884c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a9884c)
 #2 0x000055a513be7020 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f6047f03420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f60479d000b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f60479af859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f60479af729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f60479c0fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x000055a5128bea0f llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, int, unsigned int, llvm::RegScavenger*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x26bea0f)
 #9 0x000055a51322cc23 (anonymous namespace)::PEI::replaceFrameIndices(llvm::MachineBasicBlock*, llvm::MachineFunction&, int&) PrologEpilogInserter.cpp:0:0
#10 0x000055a5132311b9 (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#11 0x000055a5131015a0 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.72) MachineFunctionPass.cpp:0:0
#12 0x000055a51367d7de llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x347d7de)
#13 0x000055a51367db19 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x347db19)
#14 0x000055a51367e920 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x347e920)
#15 0x000055a513edae2b clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cdae2b)
#16 0x000055a514d68753 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b68753)
#17 0x000055a51603a459 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e3a459)
#18 0x000055a514d67560 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b67560)
#19 0x000055a5146892e9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44892e9)
#20 0x000055a51461dc42 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x441dc42)
#21 0x000055a514760c9f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4560c9f)
#22 0x000055a511422cc3 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1222cc3)
#23 0x000055a51141ade8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#24 0x000055a5144912f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#25 0x000055a513be74a5 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39e74a5)
#26 0x000055a514493eee clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4293eee)
#27 0x000055a51446060b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x426060b)
#28 0x000055a5144611e3 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42611e3)
#29 0x000055a514469fd6 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4269fd6)
#30 0x000055a511420a8e clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1220a8e)
#31 0x000055a511351cb1 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1151cb1)
#32 0x00007f60479b1083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#33 0x000055a51141a4da _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x121a4da)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```
Crash in C mode: https://godbolt.org/z/sKx5f9dqe
```
<source>:4:1: warning: non-void function does not return a value [-Wreturn-type]
    4 | }
      | ^
clang++: /root/llvm-project/llvm/lib/Target/X86/X86RegisterInfo.cpp:901:
virtual bool llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger*) const:
Assertion `(!Is64Bit || isInt<32>((long long)FIOffset + Imm)) && "Requesting 64-bit offset in 32-bit immediate!"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@baz'
 #0 0x0000559d6189ac1a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a9ac1a)
 #1 0x0000559d6189884c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a9884c)
 #2 0x0000559d617e7020 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f899fc72420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f899f73f00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f899f71e859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f899f71e729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f899f72ffd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000559d604bea0f llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, int, unsigned int, llvm::RegScavenger*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x26bea0f)
 #9 0x0000559d60e2cc23 (anonymous namespace)::PEI::replaceFrameIndices(llvm::MachineBasicBlock*, llvm::MachineFunction&, int&) PrologEpilogInserter.cpp:0:0
#10 0x0000559d60e311b9 (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#11 0x0000559d60d015a0 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.72) MachineFunctionPass.cpp:0:0
#12 0x0000559d6127d7de llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x347d7de)
#13 0x0000559d6127db19 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x347db19)
#14 0x0000559d6127e920 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x347e920)
#15 0x0000559d61adae2b clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3cdae2b)
#16 0x0000559d62968753 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b68753)
#17 0x0000559d63c3a459 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e3a459)
#18 0x0000559d62967560 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b67560)
#19 0x0000559d622892e9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44892e9)
#20 0x0000559d6221dc42 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x441dc42)
#21 0x0000559d62360c9f clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4560c9f)
#22 0x0000559d5f022cc3 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1222cc3)
#23 0x0000559d5f01ade8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#24 0x0000559d620912f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#25 0x0000559d617e74a5 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39e74a5)
#26 0x0000559d62093eee clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4293eee)
#27 0x0000559d6206060b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x426060b)
#28 0x0000559d620611e3 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42611e3)
#29 0x0000559d62069fd6 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4269fd6)
#30 0x0000559d5f020a8e clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1220a8e)
#31 0x0000559d5ef51cb1 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x1151cb1)
#32 0x00007f899f720083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#33 0x0000559d5f01a4da _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x121a4da)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 4 - llvmbot

@llvm/issue-subscribers-clang-codegen

---

### Comment 5 - Endilll

Possibly related: #57317

---

### Comment 6 - wesleywiser

This was fixed by #152239.

---

### Comment 7 - RKSimon

> This was fixed by [#152239](https://github.com/llvm/llvm-project/pull/152239).

Confirmed - closing

---

