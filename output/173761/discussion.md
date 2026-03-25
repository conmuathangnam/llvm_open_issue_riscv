# [LV] Invariance assertion hit in WidenGEP

**Author:** ams-x9v2b7kq
**URL:** https://github.com/llvm/llvm-project/issues/173761
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-01-06T13:05:39Z

## Body

## [RISC-V] Clang crashes when running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@_Z1yv'

### code link: https://godbolt.org/z/4E35bd6do
The test case is reduced by C-Reduce.

```c++
int a, j, k, m, o, p, q, r;
char l;
short n, u;
int s[24][6];
extern int t[], v[];
extern bool w[][4][4][4];
char x[36];
void y() {
  for (short b = 0; b < 31; b += 2) {
    for (char c = 0; c < j; c = 3)
      ;
    for (bool d = 0; d < 1; d = r ?: t[b]) {
      for (int e(k); e; e += n)
        for (int f(a); f; f += 4)
          o = v[b];
#pragma clang loop vectorize_predicate(enable)
      for (short g = 0; g < m; g++) {
        p = q <= w[g][g][b][b];
        for (int h = 0; h < 24; h += 4)
          ;
        for (int i = 0; i < 24; i += 4) {
          u = k ?: s[i][i];
          x[i] = w[b][d][g][i] ? w[b][b][b][i] : l;
        }
      }
    }
  }
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/2025-compiler/gcc-16-20251221/bin/clang++ --gcc-toolchain=/home/2025-compiler/gcc-16-20251221 -O3 -march=rv64gcv_zvfh -Werror=uninitialized -Werror=implicitly-unsigned-literal -Wno-pass-failed -c func.cpp -o func.o
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'func.cpp'.
4.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@_Z1yv'
 #0 0x0000556adac9d852 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de9852)
 #1 0x0000556adac9b2fc llvm::sys::CleanupOnSignal(unsigned long) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de72fc)
 #2 0x0000556adabdb988 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f1e6d335330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000556ada2d6578 llvm::EVT::getExtendedSizeInBits() const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1422578)
 #5 0x0000556ad9c836aa legalizeScatterGatherIndexType(llvm::SDLoc, llvm::SDValue&, llvm::ISD::MemIndexType&, llvm::TargetLowering::DAGCombinerInfo&) (.isra.0) RISCVISelLowering.cpp:0:0
 #6 0x0000556ad9cf3f7c llvm::RISCVTargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xe3ff7c)
 #7 0x0000556adbef6b89 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
 #8 0x0000556adbef8126 (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
 #9 0x0000556adbefb466 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x3047466)
#10 0x0000556adc01a389 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x3166389)
#11 0x0000556adc01d00d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x316900d)
#12 0x0000556adc01ebe5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x316abe5)
#13 0x0000556adc00d7a9 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x31597a9)
#14 0x0000556ada0f6ac0 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1242ac0)
#15 0x0000556ada6eb849 llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1837849)
#16 0x0000556ada6eb9f4 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x18379f4)
#17 0x0000556ada6ead77 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1836d77)
#18 0x0000556adaf2337b clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x206f37b)
#19 0x0000556adb5030a9 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x264f0a9)
#20 0x0000556adcf4908c clang::ParseAST(clang::Sema&, bool, bool) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x409508c)
#21 0x0000556adb83ed8e clang::FrontendAction::Execute() (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x298ad8e)
#22 0x0000556adb7c06d3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x290c6d3)
#23 0x0000556adb9225af clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x2a6e5af)
#24 0x0000556ad9b24a7d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc70a7d)
#25 0x0000556ad9b1c25a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000556ad9b1c2b5 int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x0000556adb57f5bd void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000556adabdbdb7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1d27db7)
#29 0x0000556adb57fcb1 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x0000556adb542ed6 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268eed6)
#31 0x0000556adb543f39 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268ff39)
#32 0x0000556adb55584c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x26a184c)
#33 0x0000556ad9b21111 clang_main(int, char**, llvm::ToolContext const&) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc6d111)
#34 0x0000556ad9a231da main (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xb6f1da)
#35 0x00007f1e6d31a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#36 0x00007f1e6d31a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#37 0x0000556ad9b1bc85 _start (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc67c85)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /home/2025-compiler/gcc-16-20251221/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/func-90a260.cpp
clang++: note: diagnostic msg: /tmp/func-90a260.sh
clang++: note: diagnostic msg:

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: None (ams-x9v2b7kq)

<details>
## [RISC-V] Clang crashes when running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_Z1yv'

### code link: https://godbolt.org/z/4E35bd6do
The test case is reduced by C-Reduce.

```c++
int a, j, k, m, o, p, q, r;
char l;
short n, u;
int s[24][6];
extern int t[], v[];
extern bool w[][4][4][4];
char x[36];
void y() {
  for (short b = 0; b &lt; 31; b += 2) {
    for (char c = 0; c &lt; j; c = 3)
      ;
    for (bool d = 0; d &lt; 1; d = r ?: t[b]) {
      for (int e(k); e; e += n)
        for (int f(a); f; f += 4)
          o = v[b];
#pragma clang loop vectorize_predicate(enable)
      for (short g = 0; g &lt; m; g++) {
        p = q &lt;= w[g][g][b][b];
        for (int h = 0; h &lt; 24; h += 4)
          ;
        for (int i = 0; i &lt; 24; i += 4) {
          u = k ?: s[i][i];
          x[i] = w[b][d][g][i] ? w[b][b][b][i] : l;
        }
      }
    }
  }
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/2025-compiler/gcc-16-20251221/bin/clang++ --gcc-toolchain=/home/2025-compiler/gcc-16-20251221 -O3 -march=rv64gcv_zvfh -Werror=uninitialized -Werror=implicitly-unsigned-literal -Wno-pass-failed -c func.cpp -o func.o
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'func.cpp'.
4.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_Z1yv'
 #<!-- -->0 0x0000556adac9d852 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de9852)
 #<!-- -->1 0x0000556adac9b2fc llvm::sys::CleanupOnSignal(unsigned long) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de72fc)
 #<!-- -->2 0x0000556adabdb988 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f1e6d335330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000556ada2d6578 llvm::EVT::getExtendedSizeInBits() const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1422578)
 #<!-- -->5 0x0000556ad9c836aa legalizeScatterGatherIndexType(llvm::SDLoc, llvm::SDValue&amp;, llvm::ISD::MemIndexType&amp;, llvm::TargetLowering::DAGCombinerInfo&amp;) (.isra.0) RISCVISelLowering.cpp:0:0
 #<!-- -->6 0x0000556ad9cf3f7c llvm::RISCVTargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&amp;) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xe3ff7c)
 #<!-- -->7 0x0000556adbef6b89 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
 #<!-- -->8 0x0000556adbef8126 (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
 #<!-- -->9 0x0000556adbefb466 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x3047466)
#<!-- -->10 0x0000556adc01a389 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x3166389)
#<!-- -->11 0x0000556adc01d00d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x316900d)
#<!-- -->12 0x0000556adc01ebe5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x316abe5)
#<!-- -->13 0x0000556adc00d7a9 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x31597a9)
#<!-- -->14 0x0000556ada0f6ac0 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1242ac0)
#<!-- -->15 0x0000556ada6eb849 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1837849)
#<!-- -->16 0x0000556ada6eb9f4 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x18379f4)
#<!-- -->17 0x0000556ada6ead77 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1836d77)
#<!-- -->18 0x0000556adaf2337b clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x206f37b)
#<!-- -->19 0x0000556adb5030a9 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x264f0a9)
#<!-- -->20 0x0000556adcf4908c clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x409508c)
#<!-- -->21 0x0000556adb83ed8e clang::FrontendAction::Execute() (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x298ad8e)
#<!-- -->22 0x0000556adb7c06d3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x290c6d3)
#<!-- -->23 0x0000556adb9225af clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x2a6e5af)
#<!-- -->24 0x0000556ad9b24a7d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc70a7d)
#<!-- -->25 0x0000556ad9b1c25a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000556ad9b1c2b5 int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000556adb57f5bd void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000556adabdbdb7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1d27db7)
#<!-- -->29 0x0000556adb57fcb1 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000556adb542ed6 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268eed6)
#<!-- -->31 0x0000556adb543f39 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268ff39)
#<!-- -->32 0x0000556adb55584c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x26a184c)
#<!-- -->33 0x0000556ad9b21111 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc6d111)
#<!-- -->34 0x0000556ad9a231da main (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xb6f1da)
#<!-- -->35 0x00007f1e6d31a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->36 0x00007f1e6d31a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->37 0x0000556ad9b1bc85 _start (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc67c85)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /home/2025-compiler/gcc-16-20251221/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/func-90a260.cpp
clang++: note: diagnostic msg: /tmp/func-90a260.sh
clang++: note: diagnostic msg:

********************
```
</details>


---

### Comment 2 - XChy

It seems not to be a RISC-V backend problem, as illegal IR appears:
```llvm
%72 = select <vscale x 4 x i1> %68, ptr %26, <vscale x 4 x ptr> shufflevector (<vscale x 4 x ptr> insertelement (<vscale x 4 x ptr> poison, ptr @l, i64 0), <vscale x 4 x ptr> poison, <vscale x 4 x i32> zeroinitializer)
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: None (ams-x9v2b7kq)

<details>
## [RISC-V] Clang crashes when running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_Z1yv'

### code link: https://godbolt.org/z/4E35bd6do
The test case is reduced by C-Reduce.

```c++
int a, j, k, m, o, p, q, r;
char l;
short n, u;
int s[24][6];
extern int t[], v[];
extern bool w[][4][4][4];
char x[36];
void y() {
  for (short b = 0; b &lt; 31; b += 2) {
    for (char c = 0; c &lt; j; c = 3)
      ;
    for (bool d = 0; d &lt; 1; d = r ?: t[b]) {
      for (int e(k); e; e += n)
        for (int f(a); f; f += 4)
          o = v[b];
#pragma clang loop vectorize_predicate(enable)
      for (short g = 0; g &lt; m; g++) {
        p = q &lt;= w[g][g][b][b];
        for (int h = 0; h &lt; 24; h += 4)
          ;
        for (int i = 0; i &lt; 24; i += 4) {
          u = k ?: s[i][i];
          x[i] = w[b][d][g][i] ? w[b][b][b][i] : l;
        }
      }
    }
  }
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/2025-compiler/gcc-16-20251221/bin/clang++ --gcc-toolchain=/home/2025-compiler/gcc-16-20251221 -O3 -march=rv64gcv_zvfh -Werror=uninitialized -Werror=implicitly-unsigned-literal -Wno-pass-failed -c func.cpp -o func.o
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module 'func.cpp'.
4.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_Z1yv'
 #<!-- -->0 0x0000556adac9d852 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de9852)
 #<!-- -->1 0x0000556adac9b2fc llvm::sys::CleanupOnSignal(unsigned long) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1de72fc)
 #<!-- -->2 0x0000556adabdb988 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f1e6d335330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000556ada2d6578 llvm::EVT::getExtendedSizeInBits() const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1422578)
 #<!-- -->5 0x0000556ad9c836aa legalizeScatterGatherIndexType(llvm::SDLoc, llvm::SDValue&amp;, llvm::ISD::MemIndexType&amp;, llvm::TargetLowering::DAGCombinerInfo&amp;) (.isra.0) RISCVISelLowering.cpp:0:0
 #<!-- -->6 0x0000556ad9cf3f7c llvm::RISCVTargetLowering::PerformDAGCombine(llvm::SDNode*, llvm::TargetLowering::DAGCombinerInfo&amp;) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xe3ff7c)
 #<!-- -->7 0x0000556adbef6b89 (anonymous namespace)::DAGCombiner::combine(llvm::SDNode*) DAGCombiner.cpp:0:0
 #<!-- -->8 0x0000556adbef8126 (anonymous namespace)::DAGCombiner::Run(llvm::CombineLevel) DAGCombiner.cpp:0:0
 #<!-- -->9 0x0000556adbefb466 llvm::SelectionDAG::Combine(llvm::CombineLevel, llvm::BatchAAResults*, llvm::CodeGenOptLevel) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x3047466)
#<!-- -->10 0x0000556adc01a389 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x3166389)
#<!-- -->11 0x0000556adc01d00d llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x316900d)
#<!-- -->12 0x0000556adc01ebe5 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x316abe5)
#<!-- -->13 0x0000556adc00d7a9 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x31597a9)
#<!-- -->14 0x0000556ada0f6ac0 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1242ac0)
#<!-- -->15 0x0000556ada6eb849 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1837849)
#<!-- -->16 0x0000556ada6eb9f4 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x18379f4)
#<!-- -->17 0x0000556ada6ead77 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1836d77)
#<!-- -->18 0x0000556adaf2337b clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x206f37b)
#<!-- -->19 0x0000556adb5030a9 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x264f0a9)
#<!-- -->20 0x0000556adcf4908c clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x409508c)
#<!-- -->21 0x0000556adb83ed8e clang::FrontendAction::Execute() (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x298ad8e)
#<!-- -->22 0x0000556adb7c06d3 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x290c6d3)
#<!-- -->23 0x0000556adb9225af clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x2a6e5af)
#<!-- -->24 0x0000556ad9b24a7d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc70a7d)
#<!-- -->25 0x0000556ad9b1c25a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000556ad9b1c2b5 int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x0000556adb57f5bd void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000556adabdbdb7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x1d27db7)
#<!-- -->29 0x0000556adb57fcb1 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x0000556adb542ed6 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268eed6)
#<!-- -->31 0x0000556adb543f39 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x268ff39)
#<!-- -->32 0x0000556adb55584c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0x26a184c)
#<!-- -->33 0x0000556ad9b21111 clang_main(int, char**, llvm::ToolContext const&amp;) (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc6d111)
#<!-- -->34 0x0000556ad9a231da main (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xb6f1da)
#<!-- -->35 0x00007f1e6d31a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->36 0x00007f1e6d31a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->37 0x0000556ad9b1bc85 _start (/home/2025-compiler/gcc-16-20251221/bin/clang+++0xc67c85)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /home/2025-compiler/gcc-16-20251221/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/func-90a260.cpp
clang++: note: diagnostic msg: /tmp/func-90a260.sh
clang++: note: diagnostic msg:

********************
```
</details>


---

### Comment 4 - topperc

The IR is invalid. Using an assertions build fails in the vectorizer. https://godbolt.org/z/bvEozPqd6

---

### Comment 5 - ElvisWang123

Seems relate to #167880 (#166740). 

The  VPlan before narrowToSingleSclaar().
```
<x1> vector loop: {
  vector.body:
    EMIT vp<%6> = CANONICAL-INDUCTION ir<0>, vp<%index.next>
    ir<%g.0137> = WIDEN-INDUCTION ir<0>, ir<1>, vp<%0>
    EMIT vp<%7> = WIDEN-CANONICAL-INDUCTION vp<%6>
    EMIT vp<%8> = icmp ule vp<%7>, vp<%3>
    WIDEN-CAST ir<%idxprom29> = sext ir<%g.0137> to i64
    WIDEN-GEP Inv[Var] ir<%arrayidx30> = getelementptr inbounds ir<@w>, ir<%idxprom29>
    WIDEN-GEP Var[Var] ir<%arrayidx32> = getelementptr inbounds ir<%arrayidx30>, ir<%idxprom29>
    WIDEN-GEP Var[Inv] ir<%arrayidx34> = getelementptr inbounds nuw ir<%arrayidx32>, ir<%indvars.iv>
    WIDEN-GEP Var[Inv] ir<%arrayidx36> = getelementptr inbounds nuw ir<%arrayidx34>, ir<%indvars.iv>
    WIDEN ir<%8> = load ir<%arrayidx36>, vp<%8> (!tbaa !12)
    WIDEN-GEP Inv[Var] ir<%arrayidx64> = getelementptr inbounds ir<%arrayidx59>, ir<%idxprom29>
    WIDEN ir<%9> = load ir<%arrayidx64>, vp<%8> (!tbaa !12)
    WIDEN-CAST ir<%loadedv67.us> = trunc nuw ir<%9> to i1
    WIDEN-SELECT ir<%cond82.in.in.us> = select ir<%loadedv67.us>, ir<%arrayidx74>, ir<@l>
    WIDEN ir<%cond82.in.us> = load ir<%cond82.in.in.us>, vp<%8> (!tbaa !16)
    WIDEN store ir<@x>, ir<%cond82.in.us>, vp<%8> (!tbaa !16)
    WIDEN-GEP Var[Inv] ir<%arrayidx66.us.1> = getelementptr inbounds nuw ir<%arrayidx64>, ir<4>
    WIDEN ir<%10> = load ir<%arrayidx66.us.1>, vp<%8> (!tbaa !12)
    WIDEN-CAST ir<%loadedv67.us.1> = trunc nuw ir<%10> to i1
    WIDEN-GEP Var[Inv] ir<%arrayidx66.us.2> = getelementptr inbounds nuw ir<%arrayidx64>, ir<8>
    WIDEN-GEP Var[Inv] ir<%arrayidx66.us.3> = getelementptr inbounds nuw ir<%arrayidx64>, ir<12>
    WIDEN-GEP Var[Inv] ir<%arrayidx66.us.4> = getelementptr inbounds nuw ir<%arrayidx64>, ir<16>
 =>   WIDEN-GEP Inv[Inv] ir<%arrayidx76.us.1.arrayidx76.1> = getelementptr inbounds nuw ir<%arrayidx74>, ir<4>.  <= assertion hit
 =>   WIDEN-GEP Inv[Inv] ir<%arrayidx76.us.2.arrayidx76.2> = getelementptr inbounds nuw ir<%arrayidx74>, ir<8>.  <= assertion hit
 =>   WIDEN-GEP Inv[Inv] ir<%arrayidx76.us.3.arrayidx76.3> = getelementptr inbounds nuw ir<%arrayidx74>, ir<12>   <= assertion hit
 =>   WIDEN-GEP Inv[Inv] ir<%arrayidx76.us.4.arrayidx76.4> = getelementptr inbounds nuw ir<%arrayidx74>, ir<16>   <= assertion hit
    ...
```
It seems that the code prevents extra broadcast also prevent these WIden-GEP converted to scalar geps.
The operand `%arrayidx74` has multiple use, so it won't narrow this widen-gep to scalar gep.
https://github.com/llvm/llvm-project/blob/44e32ccf0343e5df6bb0afa7121fdd50979b696e/llvm/lib/Transforms/Vectorize/VPlanTransforms.cpp#L1652-L1674

CC. @artagnon @fhahn 

---

### Comment 6 - artagnon

Thanks, will take a look first thing on Monday.

---

### Comment 7 - fhahn

May be best to releax the assertion for now?

---

### Comment 8 - topperc

> May be best to releax the assertion for now?

The initial report was due to invalid IR being generated. Relaxing the assert won't fix the invalid IR right?

---

### Comment 9 - fhahn

I meant relaxing the assertion by instead checking if it holds, and if not handle the additional case.

---

### Comment 10 - artagnon

I don't think we have a choice but to restore the logic (https://github.com/llvm/llvm-project/pull/174416), as this particular case does indeed introduce extra broadcasts, which would be counter to the narrowToSingleScalars objective.

---

