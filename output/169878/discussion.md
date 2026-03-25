# [clang] ICE in X86 DAG->DAG Instruction Selection when processing inline assembly with invalid ARM SVE instruction

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/169878
**Status:** Open
**Labels:** backend:X86, crash, llvm:SelectionDAG

## Body

### Link for quick verification: https://godbolt.org/z/nYhqdjvoj
When I use ARM SVE instructions in x86 inline assembly with potentially unaligned memory, Clang crashes with an internal compiler error (ICE) instead of reporting a diagnostic about the invalid instruction set or the alignment issue.
```c
typedef float __v4sf __attribute__((__vector_size__(16)));

int main() {
    unsigned char arr[32] = {0};

    __v4sf xvec1;
    __asm__("ldff1 %x[x], %[mem]\n" 
            : [x]"=V"(xvec1) 
            : [mem]"m"(*((float (*)[4])&arr))
            
            );

   return 0;
}
```

## The stack dump is: 
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 26975.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=153 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/26975-1f98f3.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26975.fuzz
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26975.fuzz'.
4.      Running pass 'X86 DAG->DAG Instruction Selection' on function '@main'
 #0 0x000060651cf5fc2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x000060651cf60457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x000060651cf59f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x000060651cf5a1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x000070e90dd7d330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000070e90ddd6b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000070e90ddd6b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000070e90ddd6b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000070e90dd7d27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000070e90dd608ff abort ./stdlib/abort.c:81:7
#10 0x000070e90dd6081b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x000070e90dd73517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x000060651ed67991 llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&, llvm::BasicBlock const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb499991)
#13 0x000060651eda938f llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4db38f)
#14 0x000060651ed57abd llvm::SelectionDAGBuilder::visit(unsigned int, llvm::User const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb489abd)
#15 0x000060651edab238 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4dd238)
#16 0x000060651ee9796d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5c996d)
#17 0x000060651ee9863e llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5ca63e)
#18 0x000060651ee9b11e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5cd11e)
#19 0x000060651acbec0c (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) X86ISelDAGToDAG.cpp:0:0
#20 0x000060651ee8538d llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5b738d)
#21 0x000060651ba82586 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x81b4586)
#22 0x000060651c47c6a9 llvm::FPPassManager::runOnFunction(llvm::Function&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae6a9)
#23 0x000060651c47ce89 llvm::FPPassManager::runOnModule(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8baee89)
#24 0x000060651c47bb77 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8badb77)
#25 0x000060651c47c469 llvm::legacy::PassManager::run(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae469)
#26 0x000060651d38b49e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9abd49e)
#27 0x000060651dd743a2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a63a2)
#28 0x000060652043feb5 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb71eb5)
#29 0x000060651e18792b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#30 0x000060651dd75360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#31 0x000060651e18f9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#32 0x000060651e0d5f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#33 0x000060651e317f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#34 0x0000606518086523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#35 0x0000606518076c58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x000060651807cb28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#37 0x0000606517ed7b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#38 0x000070e90dd621ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#39 0x000070e90dd6228b call_init ./csu/../csu/libc-start.c:128:20
#40 0x000070e90dd6228b __libc_start_main ./csu/../csu/libc-start.c:347:5
#41 0x0000606518074f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
```

## The Clang version I used:
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
### Link for quick verification: https://godbolt.org/z/c1MP8xf8r
When I use ARM SVE instructions in x86 inline assembly with potentially unaligned memory, Clang crashes with an internal compiler error (ICE) instead of reporting a diagnostic about the invalid instruction set or the alignment issue.
```c
typedef float __v4sf __attribute__((__vector_size__(16)));

int main() {
    unsigned char arr[32] = {0};

    __v4sf xvec1;
    __asm__("ldff1 %x[x], %[mem]\n" 
            : [x]"=V"(xvec1) 
            : [mem]"m"(*((float (*)[4])&amp;arr))
            
            );

   return 0;
}
```

## The stack dump is: 
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 26975.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace/exps -fcoverage-compilation-dir=/workspace/exps -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=153 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/26975-1f98f3.o -x c /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26975.fuzz
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_choose_3_1_exp/cases/26975.fuzz'.
4.      Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->main'
 #<!-- -->0 0x000060651cf5fc2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x000060651cf60457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x000060651cf59f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x000060651cf5a1c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x000070e90dd7d330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x000070e90ddd6b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000070e90ddd6b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000070e90ddd6b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000070e90dd7d27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000070e90dd608ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000070e90dd6081b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x000070e90dd73517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x000060651ed67991 llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&amp;, llvm::BasicBlock const*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb499991)
#<!-- -->13 0x000060651eda938f llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4db38f)
#<!-- -->14 0x000060651ed57abd llvm::SelectionDAGBuilder::visit(unsigned int, llvm::User const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb489abd)
#<!-- -->15 0x000060651edab238 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb4dd238)
#<!-- -->16 0x000060651ee9796d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5c996d)
#<!-- -->17 0x000060651ee9863e llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5ca63e)
#<!-- -->18 0x000060651ee9b11e llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5cd11e)
#<!-- -->19 0x000060651acbec0c (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) X86ISelDAGToDAG.cpp:0:0
#<!-- -->20 0x000060651ee8538d llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5b738d)
#<!-- -->21 0x000060651ba82586 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x81b4586)
#<!-- -->22 0x000060651c47c6a9 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae6a9)
#<!-- -->23 0x000060651c47ce89 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8baee89)
#<!-- -->24 0x000060651c47bb77 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8badb77)
#<!-- -->25 0x000060651c47c469 llvm::legacy::PassManager::run(llvm::Module&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae469)
#<!-- -->26 0x000060651d38b49e clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9abd49e)
#<!-- -->27 0x000060651dd743a2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a63a2)
#<!-- -->28 0x000060652043feb5 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb71eb5)
#<!-- -->29 0x000060651e18792b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->30 0x000060651dd75360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->31 0x000060651e18f9cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->32 0x000060651e0d5f46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->33 0x000060651e317f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->34 0x0000606518086523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->35 0x0000606518076c58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x000060651807cb28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->37 0x0000606517ed7b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->38 0x000070e90dd621ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->39 0x000070e90dd6228b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->40 0x000070e90dd6228b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->41 0x0000606518074f55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
clang: error: unable to execute command: Aborted (core dumped)
```

## The Clang version I used:
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

### Comment 2 - abhishek-kaushik22

Weird that the non-assertion build has the intended behavior.

```
<source>:9:13: error: invalid operand in inline asm: 'ldff1 ${0:x}, $1
'
    9 |     __asm__("ldff1 %x[x], %[mem]\n" 
      |             ^
<source>:9:13: error: unknown token in expression
<inline asm>:1:8: note: instantiated into assembly here
    1 |         ldff1 , -56(%rsp)
      |               ^
2 errors generated.
Compiler returned: 1
```

https://godbolt.org/z/azxh89E1r

---

