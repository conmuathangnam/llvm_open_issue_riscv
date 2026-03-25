# clang crashes on x86_64-linux-gnu: Assertion `i == DstIdx || !MI->getOperand(i).isReg() || MI->getOperand(i).getReg() != RegA' failed

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/118247

## Body

Compiler Expoloer: https://godbolt.org/z/zc6exovje

```console
clang: /root/llvm-project/llvm/lib/CodeGen/TwoAddressInstructionPass.cpp:1594: void {anonymous}::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, {anonymous}::TwoAddressInstructionImpl::TiedPairList&, unsigned int&): Assertion `i == DstIdx || !MI->getOperand(i).isReg() || MI->getOperand(i).getReg() != RegA' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Two-Address instruction pass' on function '@foo'
 #0 0x0000000003bf5af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bf5af8)
 #1 0x0000000003bf37fc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bf37fc)
 #2 0x0000000003b40ee8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000789338a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000789338a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000789338a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000789338a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000789338a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000789338a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000032486e7 (anonymous namespace)::TwoAddressInstructionImpl::processTiedPairs(llvm::MachineInstr*, llvm::SmallVector<std::pair<unsigned int, unsigned int>, 4u>&, unsigned int&) TwoAddressInstructionPass.cpp:0:0
#10 0x0000000003249e7d (anonymous namespace)::TwoAddressInstructionImpl::run() TwoAddressInstructionPass.cpp:0:0
#11 0x000000000324c35c (anonymous namespace)::TwoAddressInstructionLegacyPass::runOnMachineFunction(llvm::MachineFunction&) TwoAddressInstructionPass.cpp:0:0
#12 0x0000000002f4c5e9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#13 0x0000000003552800 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3552800)
#14 0x0000000003552bb1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3552bb1)
#15 0x0000000003553467 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3553467)
#16 0x0000000003ea6ac5 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ea6ac5)
#17 0x0000000004578bbe clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4578bbe)
#18 0x000000000651c9ec clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x651c9ec)
#19 0x0000000004578f98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4578f98)
#20 0x0000000004833eb9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4833eb9)
#21 0x00000000047b3dce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x47b3dce)
#22 0x000000000491edde clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x491edde)
#23 0x0000000000caf871 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcaf871)
#24 0x0000000000ca73fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x00000000045bc1f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003b41394 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b41394)
#27 0x00000000045bc7ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x0000000004582a7d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4582a7d)
#29 0x0000000004583b6d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4583b6d)
#30 0x000000000458af25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x458af25)
#31 0x0000000000cac6a3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcac6a3)
#32 0x0000000000b7fe54 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xb7fe54)
#33 0x0000789338a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x0000789338a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000ca6eae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xca6eae)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
extern void abort (void);

void
__attribute__((noinline))
foo (unsigned int *y)
{
   unsigned int c0, c1, c2, d0, d1, d2;
   d0 = 0; d1 = 0; d2 = 0; c0 = c1 = c2 = 0;

   __asm__ ("movl $7, %k0; movl $8, %k1; movl $9, %k2"
            : "+r" (d0), "+r" (d1), "+r" (d2));
   __asm__ ("movl %3, %0; movl %4, %1; movl %5, %2"
            : "+d" (c0), "+d" (c1), "+d" (c2), "+d" (d0), "+d" (d1), "+d" (d2));
   y[0] = c0;
   y[1] = c1;
   y[2] = c2;
}

int
main (void)
{
  unsigned int y[3];
  foo (y);
  if (y[0] != 7 || y[1] != 8 || y[2] != 9)
    abort ();
  return 0;
}
```

## Comments

### Comment 1 - shafik

This goes back to clang-2.7: https://godbolt.org/z/4vPvYT6Gq

---

### Comment 2 - keinflue

Reduces to (https://godbolt.org/z/63nzdGdW8)
```cpp
void foo()
{
  int a, b;
  asm("" : "+d" (a), "+d" (b));
}
```

---

