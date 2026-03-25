# [RISC-V] LLVM ERROR: Cannot scalarize scalable vector stores

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/174154

## Body

Reproducer: https://godbolt.org/z/5ozxjecKj
C reproducer: https://godbolt.org/z/1ezav689r
Testcase:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define i56 @backsmith_pure_4(i64 %0) #0 {
entry:
  %1 = extractelement <vscale x 16 x i56> zeroinitializer, i64 %0
  ret i56 %1
}

attributes #0 = { "target-features"="+v" }
```

Dump:
```
fatal error: error in backend: Cannot scalarize scalable vector stores
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -Os -march=rv64gcv -w <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@backsmith_pure_4'
 #0 0x0000000003d47bb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d47bb8)
 #1 0x0000000003d4558c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d4558c)
 #2 0x0000000003c8c703 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c8c703)
 #3 0x0000000003d3cdce llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d3cdce)
 #4 0x0000000000de111b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003c957f3 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c957f3)
 #6 0x0000000003c95958 (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c95958)
 #7 0x00000000052d3c60 llvm::TargetLowering::expandUnalignedLoad(llvm::LoadSDNode*, llvm::SelectionDAG&) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x52d3c60)
 #8 0x0000000005310e3d (anonymous namespace)::VectorLegalizer::Expand(llvm::SDNode*, llvm::SmallVectorImpl<llvm::SDValue>&) LegalizeVectorOps.cpp:0:0
 #9 0x0000000005314035 (anonymous namespace)::VectorLegalizer::LegalizeOp(llvm::SDValue) LegalizeVectorOps.cpp:0:0
#10 0x000000000531b71c llvm::SelectionDAG::LegalizeVectors() (/opt/compiler-explorer/clang-trunk/bin/clang+0x531b71c)
#11 0x000000000527b21c llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+0x527b21c)
#12 0x000000000527dc80 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x527dc80)
#13 0x000000000527fc65 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x527fc65)
#14 0x000000000526e831 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x526e831)
#15 0x000000000314077d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x314077d)
#16 0x00000000036a2072 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x36a2072)
#17 0x00000000036a2301 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x36a2301)
#18 0x00000000036a3b77 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x36a3b77)
#19 0x0000000003fdf21e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fdf21e)
#20 0x0000000004615bbb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4615bbb)
#21 0x000000000626dd1c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x626dd1c)
#22 0x0000000004616715 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4616715)
#23 0x0000000004926d4a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4926d4a)
#24 0x00000000048a4bfb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x48a4bfb)
#25 0x0000000004a1d15b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4a1d15b)
#26 0x0000000000de325d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xde325d)
#27 0x0000000000ddb0db ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#28 0x0000000000ddb17d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#29 0x00000000046902f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000000003c8c603 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c8c603)
#31 0x0000000004690519 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x0000000004653702 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4653702)
#33 0x00000000046545e1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x46545e1)
#34 0x000000000465d02c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x465d02c)
#35 0x0000000000ddfba9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xddfba9)
#36 0x0000000000c90424 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc90424)
#37 0x00007bcd42429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x00007bcd42429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000ddab75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xddab75)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
ASM generation compiler returned: 70
fatal error: error in backend: Cannot scalarize scalable vector stores
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Execution build compiler returned: 70
Build failed
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Hongyu Chen (XChy)

<details>
Reproducer: https://godbolt.org/z/5ozxjecKj
C reproducer: https://godbolt.org/z/1ezav689r
Testcase:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

define i56 @<!-- -->backsmith_pure_4(i64 %0) #<!-- -->0 {
entry:
  %1 = extractelement &lt;vscale x 16 x i56&gt; zeroinitializer, i64 %0
  ret i56 %1
}

attributes #<!-- -->0 = { "target-features"="+v" }
```

Dump:
```
fatal error: error in backend: Cannot scalarize scalable vector stores
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -Os -march=rv64gcv -w &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->backsmith_pure_4'
 #<!-- -->0 0x0000000003d47bb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d47bb8)
 #<!-- -->1 0x0000000003d4558c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d4558c)
 #<!-- -->2 0x0000000003c8c703 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c8c703)
 #<!-- -->3 0x0000000003d3cdce llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d3cdce)
 #<!-- -->4 0x0000000000de111b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003c957f3 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c957f3)
 #<!-- -->6 0x0000000003c95958 (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c95958)
 #<!-- -->7 0x00000000052d3c60 llvm::TargetLowering::expandUnalignedLoad(llvm::LoadSDNode*, llvm::SelectionDAG&amp;) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x52d3c60)
 #<!-- -->8 0x0000000005310e3d (anonymous namespace)::VectorLegalizer::Expand(llvm::SDNode*, llvm::SmallVectorImpl&lt;llvm::SDValue&gt;&amp;) LegalizeVectorOps.cpp:0:0
 #<!-- -->9 0x0000000005314035 (anonymous namespace)::VectorLegalizer::LegalizeOp(llvm::SDValue) LegalizeVectorOps.cpp:0:0
#<!-- -->10 0x000000000531b71c llvm::SelectionDAG::LegalizeVectors() (/opt/compiler-explorer/clang-trunk/bin/clang+0x531b71c)
#<!-- -->11 0x000000000527b21c llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+0x527b21c)
#<!-- -->12 0x000000000527dc80 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x527dc80)
#<!-- -->13 0x000000000527fc65 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x527fc65)
#<!-- -->14 0x000000000526e831 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x526e831)
#<!-- -->15 0x000000000314077d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x314077d)
#<!-- -->16 0x00000000036a2072 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x36a2072)
#<!-- -->17 0x00000000036a2301 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x36a2301)
#<!-- -->18 0x00000000036a3b77 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x36a3b77)
#<!-- -->19 0x0000000003fdf21e clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fdf21e)
#<!-- -->20 0x0000000004615bbb clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4615bbb)
#<!-- -->21 0x000000000626dd1c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x626dd1c)
#<!-- -->22 0x0000000004616715 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4616715)
#<!-- -->23 0x0000000004926d4a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x4926d4a)
#<!-- -->24 0x00000000048a4bfb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x48a4bfb)
#<!-- -->25 0x0000000004a1d15b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4a1d15b)
#<!-- -->26 0x0000000000de325d cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xde325d)
#<!-- -->27 0x0000000000ddb0db ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->28 0x0000000000ddb17d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->29 0x00000000046902f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000000003c8c603 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c8c603)
#<!-- -->31 0x0000000004690519 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x0000000004653702 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4653702)
#<!-- -->33 0x00000000046545e1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x46545e1)
#<!-- -->34 0x000000000465d02c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x465d02c)
#<!-- -->35 0x0000000000ddfba9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xddfba9)
#<!-- -->36 0x0000000000c90424 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc90424)
#<!-- -->37 0x00007bcd42429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x00007bcd42429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000ddab75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xddab75)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
ASM generation compiler returned: 70
fatal error: error in backend: Cannot scalarize scalable vector stores
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Execution build compiler returned: 70
Build failed
```
</details>


---

### Comment 2 - topperc

There's not really an easy way to fix this. We can only support scalar vectors with element types that are valid element widths in the RISC-V vector spec. The vectorizer cost model supposed to return invalid cost for anything else.

AArch64 SVE gives the same error.

---

