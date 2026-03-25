# [AArch64] crash when specifying -O2 -mllvm -pre-RA-sched=fast

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/172774
**Status:** Open
**Labels:** backend:AArch64, crash

## Body

Reproducer:
https://godbolt.org/z/P9M84Ej6q
```cpp
void foo(double *l, double *r, const double *m, int istart, int iend) {
  constexpr double alpha[3][3] = {{1.0 / 3.0, -7.0 / 6.0, 11.0 / 6.0},
                                  {-1.0 / 6.0, 5.0 / 6.0, 1.0 / 3.0},
                                  {1.0 / 3.0, 5.0 / 6.0, -1.0 / 6.0}};

  for (int i = istart; i <= iend; ++i) {
    const double q0 = m[i];
    double &q1 = l[i];
    double &q2 = r[i];

    q1 = (alpha[0][0] * q0 + alpha[0][1] * q0 + alpha[0][2] * q0);
    q1 = q2;
  }
}
```

Backtrace:
```console
free(): double free detected in tcache 2
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -O2 -mllvm -pre-RA-sched=fast <source> -isystem/opt/compiler-explorer/libs/google-benchmark/v1.4.0/include
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'AArch64 Instruction Selection' on function '@_Z3fooPdS_PKdii'
 #0 0x0000000003d10978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d10978)
 #1 0x0000000003d0e34c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0e34c)
 #2 0x0000000003c552d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c20b9a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007c20b9a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007c20b9a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007c20b9a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007c20b9a89677 (/lib/x86_64-linux-gnu/libc.so.6+0x89677)
 #8 0x00007c20b9aa0cfc (/lib/x86_64-linux-gnu/libc.so.6+0xa0cfc)
 #9 0x00007c20b9aa30ab (/lib/x86_64-linux-gnu/libc.so.6+0xa30ab)
#10 0x00007c20b9aa4bdb (/lib/x86_64-linux-gnu/libc.so.6+0xa4bdb)
#11 0x00007c20b9aa5909 __libc_realloc (/lib/x86_64-linux-gnu/libc.so.6+0xa5909)
#12 0x0000000003c8b675 llvm::SmallVectorBase<unsigned int>::grow_pod(void*, unsigned long, unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c8b675)
#13 0x000000000330b870 llvm::SUnit::addPred(llvm::SDep const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x330b870)
#14 0x00000000053293fc (anonymous namespace)::ScheduleDAGFast::ListScheduleBottomUp() ScheduleDAGFast.cpp:0:0
#15 0x000000000532b08c (anonymous namespace)::ScheduleDAGFast::Schedule() ScheduleDAGFast.cpp:0:0
#16 0x000000000523ce35 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x523ce35)
#17 0x000000000523f700 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x523f700)
#18 0x00000000052416e5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x52416e5)
#19 0x0000000005230381 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5230381)
#20 0x000000000310bbcd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x310bbcd)
#21 0x000000000366dfc2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366dfc2)
#22 0x000000000366e251 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366e251)
#23 0x000000000366fac7 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366fac7)
#24 0x0000000003fa69bb clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa69bb)
#25 0x00000000045d9b2b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d9b2b)
#26 0x00000000062235bc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62235bc)
#27 0x00000000045da685 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45da685)
#28 0x00000000048ea59a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48ea59a)
#29 0x00000000048684eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48684eb)
#30 0x00000000049e071b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49e071b)
#31 0x0000000000dccd85 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdccd85)
#32 0x0000000000dc4b7b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#33 0x0000000000dc4c1d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#34 0x0000000004654e19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x0000000003c556f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556f3)
#36 0x0000000004655039 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x0000000004617ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617ae2)
#38 0x00000000046189c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46189c1)
#39 0x00000000046214ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46214ac)
#40 0x0000000000dc9649 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc9649)
#41 0x0000000000c7ac34 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc7ac34)
#42 0x00007c20b9a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#43 0x00007c20b9a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#44 0x0000000000dc4615 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4615)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/P9M84Ej6q
```cpp
void foo(double *l, double *r, const double *m, int istart, int iend) {
  constexpr double alpha[3][3] = {{1.0 / 3.0, -7.0 / 6.0, 11.0 / 6.0},
                                  {-1.0 / 6.0, 5.0 / 6.0, 1.0 / 3.0},
                                  {1.0 / 3.0, 5.0 / 6.0, -1.0 / 6.0}};

  for (int i = istart; i &lt;= iend; ++i) {
    const double q0 = m[i];
    double &amp;q1 = l[i];
    double &amp;q2 = r[i];

    q1 = (alpha[0][0] * q0 + alpha[0][1] * q0 + alpha[0][2] * q0);
    q1 = q2;
  }
}
```

Backtrace:
```console
free(): double free detected in tcache 2
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -O2 -mllvm -pre-RA-sched=fast &lt;source&gt; -isystem/opt/compiler-explorer/libs/google-benchmark/v1.4.0/include
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->_Z3fooPdS_PKdii'
 #<!-- -->0 0x0000000003d10978 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d10978)
 #<!-- -->1 0x0000000003d0e34c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d0e34c)
 #<!-- -->2 0x0000000003c552d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c20b9a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007c20b9a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007c20b9a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007c20b9a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007c20b9a89677 (/lib/x86_64-linux-gnu/libc.so.6+0x89677)
 #<!-- -->8 0x00007c20b9aa0cfc (/lib/x86_64-linux-gnu/libc.so.6+0xa0cfc)
 #<!-- -->9 0x00007c20b9aa30ab (/lib/x86_64-linux-gnu/libc.so.6+0xa30ab)
#<!-- -->10 0x00007c20b9aa4bdb (/lib/x86_64-linux-gnu/libc.so.6+0xa4bdb)
#<!-- -->11 0x00007c20b9aa5909 __libc_realloc (/lib/x86_64-linux-gnu/libc.so.6+0xa5909)
#<!-- -->12 0x0000000003c8b675 llvm::SmallVectorBase&lt;unsigned int&gt;::grow_pod(void*, unsigned long, unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c8b675)
#<!-- -->13 0x000000000330b870 llvm::SUnit::addPred(llvm::SDep const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x330b870)
#<!-- -->14 0x00000000053293fc (anonymous namespace)::ScheduleDAGFast::ListScheduleBottomUp() ScheduleDAGFast.cpp:0:0
#<!-- -->15 0x000000000532b08c (anonymous namespace)::ScheduleDAGFast::Schedule() ScheduleDAGFast.cpp:0:0
#<!-- -->16 0x000000000523ce35 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x523ce35)
#<!-- -->17 0x000000000523f700 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x523f700)
#<!-- -->18 0x00000000052416e5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x52416e5)
#<!-- -->19 0x0000000005230381 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5230381)
#<!-- -->20 0x000000000310bbcd llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x310bbcd)
#<!-- -->21 0x000000000366dfc2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366dfc2)
#<!-- -->22 0x000000000366e251 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366e251)
#<!-- -->23 0x000000000366fac7 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x366fac7)
#<!-- -->24 0x0000000003fa69bb clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa69bb)
#<!-- -->25 0x00000000045d9b2b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d9b2b)
#<!-- -->26 0x00000000062235bc clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62235bc)
#<!-- -->27 0x00000000045da685 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45da685)
#<!-- -->28 0x00000000048ea59a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48ea59a)
#<!-- -->29 0x00000000048684eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48684eb)
#<!-- -->30 0x00000000049e071b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49e071b)
#<!-- -->31 0x0000000000dccd85 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdccd85)
#<!-- -->32 0x0000000000dc4b7b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->33 0x0000000000dc4c1d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->34 0x0000000004654e19 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->35 0x0000000003c556f3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c556f3)
#<!-- -->36 0x0000000004655039 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->37 0x0000000004617ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4617ae2)
#<!-- -->38 0x00000000046189c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46189c1)
#<!-- -->39 0x00000000046214ac clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x46214ac)
#<!-- -->40 0x0000000000dc9649 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc9649)
#<!-- -->41 0x0000000000c7ac34 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc7ac34)
#<!-- -->42 0x00007c20b9a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->43 0x00007c20b9a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->44 0x0000000000dc4615 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc4615)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - davemgreen

Does -pre-RA-sched=fast get used often? And are you using in for a specific reason? Thanks

---

