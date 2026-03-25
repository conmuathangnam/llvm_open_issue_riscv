# clang: [X86] Assertion failure in FollowCopyChain during SelectionDAG construction for asm goto with callbr landing pad

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170100
**Status:** Open
**Labels:** backend:X86, crash, llvm:SelectionDAG, regression:17

## Body

### Link for quick verification: https://godbolt.org/z/f96YzY3eY (crash both on C and C++)
```c
float f;

void test_asm_goto() {
  asm goto("" : "=r" (f) : : : lab);
lab:
  f = 0;
}
```
When using output operands in `asm goto`, the compiler went crash. The code could be compiled with the optimization flag.

## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:12719: llvm::Register FollowCopyChain(llvm::MachineRegisterInfo&, llvm::Register): Assertion `MI->getOpcode() == TargetOpcode::COPY && "start of copy chain MUST be COPY"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 37514.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=167 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/37514-7072aa.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/37514.fuzz
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/37514.fuzz'.
4.      Running pass 'X86 DAG->DAG Instruction Selection' on function '@_Z13test_asm_gotov'
 #0 0x00005a5be8b54c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x00005a5be8b55457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x00005a5be8b4ef57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x00005a5be8b4f1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007d6d44a1f330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007d6d44a78b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007d6d44a78b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007d6d44a78b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007d6d44a1f27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007d6d44a028ff abort ./stdlib/abort.c:81:7
#10 0x00007d6d44a0281b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007d6d44a15517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005a5bea942c92 llvm::SelectionDAGBuilder::visitCallBrLandingPad(llvm::CallInst const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb47fc92)
#13 0x00005a5bea9972e7 llvm::SelectionDAGBuilder::visitIntrinsicCall(llvm::CallInst const&, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4d42e7)
#14 0x00005a5bea99e473 llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4db473)
#15 0x00005a5bea94cabd llvm::SelectionDAGBuilder::visit(unsigned int, llvm::User const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb489abd)
#16 0x00005a5bea9a0238 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4dd238)
#17 0x00005a5beaa8c96d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5c996d)
#18 0x00005a5beaa8dd62 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5cad62)
#19 0x00005a5beaa9011e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5cd11e)
#20 0x00005a5be68b3c0c (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) X86ISelDAGToDAG.cpp:0:0
#21 0x00005a5beaa7a38d llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5b738d)
#22 0x00005a5be7677586 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x81b4586)
#23 0x00005a5be80716a9 llvm::FPPassManager::runOnFunction(llvm::Function&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae6a9)
#24 0x00005a5be8071e89 llvm::FPPassManager::runOnModule(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8baee89)
#25 0x00005a5be8070b77 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8badb77)
#26 0x00005a5be8071469 llvm::legacy::PassManager::run(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae469)
#27 0x00005a5be8f8049e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9abd49e)
#28 0x00005a5be99693a2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a63a2)
#29 0x00005a5bec034eb5 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb71eb5)
#30 0x00005a5be9d7c92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#31 0x00005a5be996a360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#32 0x00005a5be9d849cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#33 0x00005a5be9ccaf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#34 0x00005a5be9f0cf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#35 0x00005a5be3c7b523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#36 0x00005a5be3c6bc58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#37 0x00005a5be3c71b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#38 0x00005a5be3accb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#39 0x00007d6d44a041ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#40 0x00007d6d44a0428b call_init ./csu/../csu/libc-start.c:128:20
#41 0x00007d6d44a0428b __libc_start_main ./csu/../csu/libc-start.c:347:5
#42 0x00005a5be3c69f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```

## The Clang version:
```
clang version 21.1.4 (https://github.com/llvm/llvm-project.git 222fc11f2b8f25f6a0f4976272ef1bb7bf49521d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /workspace/install/llvm/build_21.1.4/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/f96YzY3eY (crash both on C and C++)
```c
float f;

void test_asm_goto() {
  asm goto("" : "=r" (f) : : : lab);
lab:
  f = 0;
}
```
When using output operands in `asm goto`, the compiler went crash. The code could be compiled with the optimization flag.

## Stack dump:
```
clang-21: /workspace/install/llvm/src/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:12719: llvm::Register FollowCopyChain(llvm::MachineRegisterInfo&amp;, llvm::Register): Assertion `MI-&gt;getOpcode() == TargetOpcode::COPY &amp;&amp; "start of copy chain MUST be COPY"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 37514.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fmessage-length=167 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/37514-7072aa.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/37514.fuzz
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/37514.fuzz'.
4.      Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->_Z13test_asm_gotov'
 #<!-- -->0 0x00005a5be8b54c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x00005a5be8b55457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x00005a5be8b4ef57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x00005a5be8b4f1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007d6d44a1f330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007d6d44a78b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007d6d44a78b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007d6d44a78b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007d6d44a1f27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007d6d44a028ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007d6d44a0281b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007d6d44a15517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005a5bea942c92 llvm::SelectionDAGBuilder::visitCallBrLandingPad(llvm::CallInst const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb47fc92)
#<!-- -->13 0x00005a5bea9972e7 llvm::SelectionDAGBuilder::visitIntrinsicCall(llvm::CallInst const&amp;, unsigned int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4d42e7)
#<!-- -->14 0x00005a5bea99e473 llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4db473)
#<!-- -->15 0x00005a5bea94cabd llvm::SelectionDAGBuilder::visit(unsigned int, llvm::User const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb489abd)
#<!-- -->16 0x00005a5bea9a0238 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4dd238)
#<!-- -->17 0x00005a5beaa8c96d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5c996d)
#<!-- -->18 0x00005a5beaa8dd62 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5cad62)
#<!-- -->19 0x00005a5beaa9011e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5cd11e)
#<!-- -->20 0x00005a5be68b3c0c (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) X86ISelDAGToDAG.cpp:0:0
#<!-- -->21 0x00005a5beaa7a38d llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5b738d)
#<!-- -->22 0x00005a5be7677586 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x81b4586)
#<!-- -->23 0x00005a5be80716a9 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae6a9)
#<!-- -->24 0x00005a5be8071e89 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8baee89)
#<!-- -->25 0x00005a5be8070b77 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8badb77)
#<!-- -->26 0x00005a5be8071469 llvm::legacy::PassManager::run(llvm::Module&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae469)
#<!-- -->27 0x00005a5be8f8049e clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9abd49e)
#<!-- -->28 0x00005a5be99693a2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a63a2)
#<!-- -->29 0x00005a5bec034eb5 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb71eb5)
#<!-- -->30 0x00005a5be9d7c92b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->31 0x00005a5be996a360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->32 0x00005a5be9d849cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->33 0x00005a5be9ccaf46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->34 0x00005a5be9f0cf7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->35 0x00005a5be3c7b523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->36 0x00005a5be3c6bc58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->37 0x00005a5be3c71b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->38 0x00005a5be3accb4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->39 0x00007d6d44a041ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->40 0x00007d6d44a0428b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->41 0x00007d6d44a0428b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->42 0x00005a5be3c69f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang++: error: unable to execute command: Aborted (core dumped)
```

## The Clang version:
```
clang version 21.1.4 (https://github.com/llvm/llvm-project.git 222fc11f2b8f25f6a0f4976272ef1bb7bf49521d)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /workspace/install/llvm/build_21.1.4/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```
</details>


---

### Comment 2 - bermondd

I'll try to look into this, if that's okay 

---

### Comment 3 - keinflue

Regression since Clang 17: https://godbolt.org/z/fW61G4YM5

---

