# clang: ICE in X86 DAG->DAG Instruction Selection when using inline assembly with 512-bit vector types and 'xchg' instruction

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/170024
**Status:** Closed
**Labels:** backend:X86, crash, llvm:SelectionDAG
**Closed Date:** 2026-01-16T11:57:53Z

## Body

### Link for quick verification: https://godbolt.org/z/o4fW3sKjT (crash both on C and C++)
```c
typedef __int128 __attribute__((__vector_size__(64))) V;

void swap(V *a, V *b) {
  // Induce xchg-like behavior using inline assembly to trigger register swaps.
  asm volatile(
    "xchg %[regA], %[regB]"
    : [regA] "+r" (*a), [regB] "+r" (*b)
    :
    : "memory");
}
```
When compiling code that uses inline assembly with the 'xchg' instruction operating on 512-bit vector types (defined as
 `__int128 __attribute__((__vector_size__(64))))`, the LLVM compiler crashes with an internal compiler error during the X86 DAG->DAG instruction selection phase.

## The stack dump:
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 2327.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace -fcoverage-compilation-dir=/workspace -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=190 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/2327-00d403.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/2327.fuzz
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/2327.fuzz'.
4.      Running pass 'X86 DAG->DAG Instruction Selection' on function '@_Z4swapPDv4_nS0_'
 #0 0x0000646f80099c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #1 0x0000646f8009a457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #2 0x0000646f80093f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #3 0x0000646f800941c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x00007303e9f62330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007303e9fbbb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007303e9fbbb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007303e9fbbb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007303e9f6227e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007303e9f458ff abort ./stdlib/abort.c:81:7
#10 0x00007303e9f4581b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007303e9f58517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x0000646f82050866 llvm::TargetLowering::ParseConstraints(llvm::DataLayout const&, llvm::TargetRegisterInfo const*, llvm::CallBase const&) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb648866)
#13 0x0000646f81e0c566 llvm::FunctionLoweringInfo::set(llvm::Function const&, llvm::MachineFunction&, llvm::SelectionDAG*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb404566)
#14 0x0000646f81fd5056 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5cd056)
#15 0x0000646f7ddf8c0c (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&) X86ISelDAGToDAG.cpp:0:0
#16 0x0000646f81fbf38d llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5b738d)
#17 0x0000646f7ebbc586 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x81b4586)
#18 0x0000646f7f5b66a9 llvm::FPPassManager::runOnFunction(llvm::Function&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae6a9)
#19 0x0000646f7f5b6e89 llvm::FPPassManager::runOnModule(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8baee89)
#20 0x0000646f7f5b5b77 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8badb77)
#21 0x0000646f7f5b6469 llvm::legacy::PassManager::run(llvm::Module&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae469)
#22 0x0000646f804c549e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9abd49e)
#23 0x0000646f80eae3a2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a63a2)
#24 0x0000646f83579eb5 clang::ParseAST(clang::Sema&, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb71eb5)
#25 0x0000646f812c192b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#26 0x0000646f80eaf360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#27 0x0000646f812c99cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#28 0x0000646f8120ff46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#29 0x0000646f81451f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#30 0x0000646f7b1c0523 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#31 0x0000646f7b1b0c58 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#32 0x0000646f7b1b6b28 clang_main(int, char**, llvm::ToolContext const&) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#33 0x0000646f7b011b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#34 0x00007303e9f471ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#35 0x00007303e9f4728b call_init ./csu/../csu/libc-start.c:128:20
#36 0x00007303e9f4728b __libc_start_main ./csu/../csu/libc-start.c:347:5
#37 0x0000646f7b1aef55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

### Comment 1 - topperc

I spent some time looking at this. This patch fixes this case but I think there are still more cases that can fail.

```
diff --git a/llvm/lib/CodeGen/SelectionDAG/TargetLowering.cpp b/llvm/lib/CodeGen/SelectionDAG/TargetLowering.cpp
index 783ec4b0bd21..13d30d7c895a 100644
--- a/llvm/lib/CodeGen/SelectionDAG/TargetLowering.cpp
+++ b/llvm/lib/CodeGen/SelectionDAG/TargetLowering.cpp
@@ -5961,9 +5961,8 @@ TargetLowering::ParseConstraints(const DataLayout &DL,
       // corresponding argument.
       assert(!Call.getType()->isVoidTy() && "Bad inline asm!");
       if (auto *STy = dyn_cast<StructType>(Call.getType())) {
-        OpInfo.ConstraintVT =
-            getAsmOperandValueType(DL, STy->getElementType(ResNo))
-                .getSimpleVT();
+        EVT VT = getAsmOperandValueType(DL, STy->getElementType(ResNo));
+        OpInfo.ConstraintVT = VT.isSimple() ? VT.getSimpleVT() : MVT::Other;
       } else {
         assert(ResNo == 0 && "Asm only has one result!");
         OpInfo.ConstraintVT =
diff --git a/llvm/lib/Target/X86/X86ISelLowering.cpp b/llvm/lib/Target/X86/X86ISelLowering.cpp
index d46f0f1572f1..83a46994e450 100644
--- a/llvm/lib/Target/X86/X86ISelLowering.cpp
+++ b/llvm/lib/Target/X86/X86ISelLowering.cpp
@@ -62137,7 +62137,7 @@ X86TargetLowering::getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
         return std::make_pair(0U, useEGPRInlineAsm(Subtarget)
                                       ? &X86::GR32RegClass
                                       : &X86::GR32_NOREX2RegClass);
-      if (VT != MVT::f80 && !VT.isVector())
+      if (VT != MVT::f80 && !VT.isVector() && VT != MVT::Other)
         return std::make_pair(0U, useEGPRInlineAsm(Subtarget)
                                       ? &X86::GR64RegClass
                                       : &X86::GR64_NOREX2RegClass);
```

---

### Comment 2 - topperc

I think someone more involved with X86 like @phoebewang or @RKSimon should take this.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/o4fW3sKjT (crash both on C and C++)
```c
typedef __int128 __attribute__((__vector_size__(64))) V;

void swap(V *a, V *b) {
  // Induce xchg-like behavior using inline assembly to trigger register swaps.
  asm volatile(
    "xchg %[regA], %[regB]"
    : [regA] "+r" (*a), [regB] "+r" (*b)
    :
    : "memory");
}
```
When compiling code that uses inline assembly with the 'xchg' instruction operating on 512-bit vector types (defined as
 `__int128 __attribute__((__vector_size__(64))))`, the LLVM compiler crashes with an internal compiler error during the X86 DAG-&gt;DAG instruction selection phase.

## The stack dump:
```
0.      Program arguments: /workspace/install/llvm/build_21.1.4/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name 2327.fuzz -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/workspace -fcoverage-compilation-dir=/workspace -resource-dir /workspace/install/llvm/build_21.1.4/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /workspace/install/llvm/build_21.1.4/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=190 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/2327-00d403.o -x c++ /workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/2327.fuzz
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/workspace/exps/featurefuzz/generate_feature_fine_tuning_qwen_4b_input_2_feature_1_exp/cases/2327.fuzz'.
4.      Running pass 'X86 DAG-&gt;DAG Instruction Selection' on function '@<!-- -->_Z4swapPDv4_nS0_'
 #<!-- -->0 0x0000646f80099c2a llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9691c2a)
 #<!-- -->1 0x0000646f8009a457 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #<!-- -->2 0x0000646f80093f57 llvm::sys::RunSignalHandlers() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x968bf57)
 #<!-- -->3 0x0000646f800941c7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->4 0x00007303e9f62330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007303e9fbbb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007303e9fbbb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007303e9fbbb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007303e9f6227e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007303e9f458ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007303e9f4581b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007303e9f58517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000646f82050866 llvm::TargetLowering::ParseConstraints(llvm::DataLayout const&amp;, llvm::TargetRegisterInfo const*, llvm::CallBase const&amp;) const (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb648866)
#<!-- -->13 0x0000646f81e0c566 llvm::FunctionLoweringInfo::set(llvm::Function const&amp;, llvm::MachineFunction&amp;, llvm::SelectionDAG*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb404566)
#<!-- -->14 0x0000646f81fd5056 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5cd056)
#<!-- -->15 0x0000646f7ddf8c0c (anonymous namespace)::X86DAGToDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) X86ISelDAGToDAG.cpp:0:0
#<!-- -->16 0x0000646f81fbf38d llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xb5b738d)
#<!-- -->17 0x0000646f7ebbc586 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x81b4586)
#<!-- -->18 0x0000646f7f5b66a9 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae6a9)
#<!-- -->19 0x0000646f7f5b6e89 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8baee89)
#<!-- -->20 0x0000646f7f5b5b77 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8badb77)
#<!-- -->21 0x0000646f7f5b6469 llvm::legacy::PassManager::run(llvm::Module&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x8bae469)
#<!-- -->22 0x0000646f804c549e clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x9abd49e)
#<!-- -->23 0x0000646f80eae3a2 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a63a2)
#<!-- -->24 0x0000646f83579eb5 clang::ParseAST(clang::Sema&amp;, bool, bool) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xcb71eb5)
#<!-- -->25 0x0000646f812c192b clang::ASTFrontendAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8b992b)
#<!-- -->26 0x0000646f80eaf360 clang::CodeGenAction::ExecuteAction() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa4a7360)
#<!-- -->27 0x0000646f812c99cf clang::FrontendAction::Execute() (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa8c19cf)
#<!-- -->28 0x0000646f8120ff46 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xa807f46)
#<!-- -->29 0x0000646f81451f7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0xaa49f7b)
#<!-- -->30 0x0000646f7b1c0523 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47b8523)
#<!-- -->31 0x0000646f7b1b0c58 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->32 0x0000646f7b1b6b28 clang_main(int, char**, llvm::ToolContext const&amp;) (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47aeb28)
#<!-- -->33 0x0000646f7b011b4a main (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x4609b4a)
#<!-- -->34 0x00007303e9f471ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->35 0x00007303e9f4728b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x00007303e9f4728b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->37 0x0000646f7b1aef55 _start (/workspace/install/llvm/build_21.1.4/bin/clang-21+0x47a6f55)
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

