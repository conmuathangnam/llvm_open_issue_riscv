# clang assertion failure at O2 "Assertion `(PDiffI->getUnitInc() >= 0) == (PNew >= POld) && "PSet overflow/underflow"' failed."

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/149144

## Body

Compiler Explorer: https://godbolt.org/z/ocW956K5d

console:
```console
clang: /root/llvm-project/llvm/lib/CodeGen/RegisterPressure.cpp:1170: void llvm::RegPressureTracker::getUpwardPressureDelta(const llvm::MachineInstr*, llvm::PressureDiff&, llvm::RegPressureDelta&, llvm::ArrayRef<llvm::PressureChange>, llvm::ArrayRef<unsigned int>) const: Assertion `(PDiffI->getUnitInc() >= 0) == (PNew >= POld) && "PSet overflow/underflow"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O2 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Machine Instruction Scheduler' on function '@foo'
 #0 0x0000000003fb2168 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fb2168)
 #1 0x0000000003faf594 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3faf594)
 #2 0x0000000003ef3d68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007011d2c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007011d2c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007011d2c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007011d2c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007011d2c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007011d2c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000034d6d39 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x34d6d39)
#10 0x0000000003361935 llvm::GenericScheduler::initCandidate(llvm::GenericSchedulerBase::SchedCandidate&, llvm::SUnit*, bool, llvm::RegPressureTracker const&, llvm::RegPressureTracker&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3361935)
#11 0x0000000003361aa3 llvm::GenericScheduler::pickNodeFromQueue(llvm::SchedBoundary&, llvm::GenericSchedulerBase::CandPolicy const&, llvm::RegPressureTracker const&, llvm::GenericSchedulerBase::SchedCandidate&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3361aa3)
#12 0x000000000336ca78 llvm::GenericScheduler::pickNode(bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x336ca78)
#13 0x0000000003373a2c llvm::ScheduleDAGMILive::schedule() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3373a2c)
#14 0x000000000335b5f6 llvm::impl_detail::MachineSchedulerBase::scheduleRegions(llvm::ScheduleDAGInstrs&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x335b5f6)
#15 0x00000000033652f6 llvm::impl_detail::MachineSchedulerImpl::run(llvm::MachineFunction&, llvm::TargetMachine const&, llvm::impl_detail::MachineSchedulerImpl::RequiredAnalyses const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x33652f6)
#16 0x00000000033660dd (anonymous namespace)::MachineSchedulerLegacy::runOnMachineFunction(llvm::MachineFunction&) MachineScheduler.cpp:0:0
#17 0x00000000032b46e3 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#18 0x00000000038dc70f llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x38dc70f)
#19 0x00000000038dcac1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x38dcac1)
#20 0x00000000038dd331 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x38dd331)
#21 0x000000000426b061 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x426b061)
#22 0x0000000004966f83 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4966f83)
#23 0x000000000665ca3c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x665ca3c)
#24 0x0000000004967388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4967388)
#25 0x0000000004c5ad75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c5ad75)
#26 0x0000000004bd782e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bd782e)
#27 0x0000000004d4e821 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d4e821)
#28 0x0000000000d933bf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd933bf)
#29 0x0000000000d89f9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x00000000049ce879 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003ef4204 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ef4204)
#32 0x00000000049cee8f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000004990d9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4990d9d)
#34 0x0000000004991e2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4991e2e)
#35 0x000000000499aa75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x499aa75)
#36 0x0000000000d8f86f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd8f86f)
#37 0x0000000000c44414 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc44414)
#38 0x00007011d2c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007011d2c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000d89a45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd89a45)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
code:
```c
int foo(char  how, const void *set, void *oset) {
  int resultvar;
  asm volatile(""
               : "=a"(resultvar)
               : "0b0"(14), "b"(how), "c"((set)), "d"((oset)), "S"(65 / 8)
               : "memory", "cc");
  return resultvar;
}
```

## Comments

### Comment 1 - XChy

The problem is that SelectionDAGBuilder allocates the same/overlapped register for `"0b0"(14), "b"(how)`. Since SelectionDAGBuilder picks `b` for the first input operand among the alternative constraints, and has to pick `b` again for the second input operand, which leads to a conflict.
I'm unsure whether there is a specific rule for selecting an alternative constraint when a conflict is possible. However, the current implementation appears not to address the conflict.

---

### Comment 2 - XChy

cc @RKSimon @topperc 

---

