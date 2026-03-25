# [risc-v] Compiler crashes with "Incomplete scavenging after 2nd pass" when passing/returning large struct/array

**Author:** algebraic-arima
**URL:** https://github.com/llvm/llvm-project/issues/180199
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2026-02-19T00:42:42Z

## Body

## Result and crash backtrace

```
fatal error: error in backend: Incomplete scavenging after 2nd pass
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -S --target=riscv32-unknown-elf test.ll -o test.s
1.	Code generation
2.	Running pass 'Function Pass Manager' on module 'test.ll'.
3.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@main'
 #0 0x000070ce4bda63bf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xda63bf)
 #1 0x000070ce4bda44f9 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xda44f9)
 #2 0x000070ce4bcefff3 (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xcefff3)
 #3 0x000070ce4bceffa2 (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xceffa2)
 #4 0x000070ce4bda0c70 llvm::sys::Process::Exit(int, bool) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xda0c70)
 #5 0x00005a22252e291b (/usr/lib/llvm-18/bin/clang+0x1491b)
 #6 0x000070ce4bcfe01c llvm::report_fatal_error(llvm::Twine const&, bool) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xcfe01c)
 #7 0x000070ce4bcfdefd (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xcfdefd)
 #8 0x000070ce4c3170f2 llvm::scavengeFrameVirtualRegs(llvm::MachineFunction&, llvm::RegScavenger&) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0x13170f2)
 #9 0x000070ce4c2a3caf (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0x12a3caf)
#10 0x000070ce4c1826b9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0x11826b9)
#11 0x000070ce4bef7772 llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xef7772)
#12 0x000070ce4befd2f4 llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xefd2f4)
#13 0x000070ce4bef7e9f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xef7e9f)
#14 0x000070ce54417310 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1c17310)
#15 0x000070ce547a4c31 clang::CodeGenAction::ExecuteAction() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1fa4c31)
#16 0x000070ce5520662c clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a0662c)
#17 0x000070ce551830b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29830b4)
#18 0x000070ce5528263d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a8263d)
#19 0x00005a22252e242e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-18/bin/clang+0x1442e)
#20 0x00005a22252df894 (/usr/lib/llvm-18/bin/clang+0x11894)
#21 0x000070ce54e33972 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2633972)
#22 0x000070ce4bceff77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xceff77)
#23 0x000070ce54e33237 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2633237)
#24 0x000070ce54dfb518 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25fb518)
#25 0x000070ce54dfb77f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25fb77f)
#26 0x000070ce54e17c20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2617c20)
#27 0x00005a22252df1ec clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-18/bin/clang+0x111ec)
#28 0x00005a22252ec383 main (/usr/lib/llvm-18/bin/clang+0x1e383)
#29 0x000070ce4a82a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#30 0x000070ce4a82a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#31 0x00005a22252dc255 _start (/usr/lib/llvm-18/bin/clang+0xe255)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: riscv32-unknown-unknown-elf
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

## Steps to reproduce

- create `test.ll`:
```llvm
@mat = global [24 x [24 x i32]] zeroinitializer

define [24 x [24 x i32]] @init([24 x [24 x i32]] %.0) {
    ret [24 x [24 x i32]] %.0
}

define void @main() {
entry:
    %.1 = load [24 x [24 x i32]], ptr @mat
    %.2 = call [24 x [24 x i32]] @init([24 x [24 x i32]] %.1)
    store [24 x [24 x i32]] %.2, ptr @mat
    ret void 
}
```
- use clang to compile the llvm IR code:
```
clang -S --target=riscv32-unknown-elf test.ll -o test.s
```

### Environment Information
- OS: Ubuntu24.04
- Clang version: clang version 18.1.3

## Additional Information

The issue occurs during Prologue/Epilogue Insertion & Frame Finalization pass which related to register scavenging. The issue disappears when struct size is reduced (e.g., to [3 x [3 x i32]]). Same code compiles successfully for other targets (e.g., x86_64).

## Technical Analysis

I encountered this problem while reviewing students' compiler project. 

Struct passing is syntactically valid in LLVM. While it is not recommended and inefficient to pass large data structures (higher register pressure during regalloc), the problem should be handled, either successfully compile using stack-based parameter passing for large data structures, or emit a clear diagnostic error about unsupported passing. Instead, clang crashes with internal failure, which may indicate a potential bug in risc-v backend's register scavenger.


## Comments

### Comment 1 - topperc

I think I understand the issue but I don't know how to fix it.

In order to spill registers, we may need a temporary register to calculate the address of where to spill the register. We use register scavenging to find a free register. If we are unable to find a free register we will do an emergency spill. To do an emergency spill we need the location of the emergency spill to be near(with 2047 bytes) the current stack pointer. If it is not, then we need a temporary register to calculate that address too.

The stack layout tries to keep the emergency spill slot near the stack pointer. Unfortunately the space needed for call frame is even closer to the stack pointer. In this case that is 2276 bytes of of space. That makes the emergency spill slot more than 2048 bytes away from the stack pointer.

---

### Comment 2 - topperc

We can disable the reserved call area with this something like this(Ignore the comment that mentions ARM. I copy/pasted for testing), but then we crash in emitEpilog because we think we need to use FP to restore the stack pointer.

```
@@ -1860,7 +1864,16 @@ void RISCVFrameLowering::processFunctionBeforeFrameFinalized(
 // by the frame pointer.
 // Let eliminateCallFramePseudoInstr preserve stack space for it.
 bool RISCVFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
-  return !MF.getFrameInfo().hasVarSizedObjects() &&
+  const MachineFrameInfo &MFI = MF.getFrameInfo();
+  unsigned CFSize = MFI.getMaxCallFrameSize();
+  // It's not always a good idea to include the call frame as part of the
+  // stack frame. ARM (especially Thumb) has small immediate offset to
+  // address the stack frame. So a large call frame can cause poor codegen
+  // and may even makes it impossible to scavenge a register.
+  if (CFSize >= ((1 << 12) - 1) / 2)  // Half of imm12
+    return false;
+
+  return !MFI.hasVarSizedObjects() &&
          !(hasFP(MF) && hasRVVFrameObject(MF));
 }

```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Fangke Li (algebraic-arima)

<details>
## Result and crash backtrace

```
fatal error: error in backend: Incomplete scavenging after 2nd pass
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -S --target=riscv32-unknown-elf test.ll -o test.s
1.	Code generation
2.	Running pass 'Function Pass Manager' on module 'test.ll'.
3.	Running pass 'Prologue/Epilogue Insertion &amp; Frame Finalization' on function '@<!-- -->main'
 #<!-- -->0 0x000070ce4bda63bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xda63bf)
 #<!-- -->1 0x000070ce4bda44f9 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xda44f9)
 #<!-- -->2 0x000070ce4bcefff3 (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xcefff3)
 #<!-- -->3 0x000070ce4bceffa2 (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xceffa2)
 #<!-- -->4 0x000070ce4bda0c70 llvm::sys::Process::Exit(int, bool) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xda0c70)
 #<!-- -->5 0x00005a22252e291b (/usr/lib/llvm-18/bin/clang+0x1491b)
 #<!-- -->6 0x000070ce4bcfe01c llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xcfe01c)
 #<!-- -->7 0x000070ce4bcfdefd (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xcfdefd)
 #<!-- -->8 0x000070ce4c3170f2 llvm::scavengeFrameVirtualRegs(llvm::MachineFunction&amp;, llvm::RegScavenger&amp;) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0x13170f2)
 #<!-- -->9 0x000070ce4c2a3caf (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0x12a3caf)
#<!-- -->10 0x000070ce4c1826b9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0x11826b9)
#<!-- -->11 0x000070ce4bef7772 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xef7772)
#<!-- -->12 0x000070ce4befd2f4 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xefd2f4)
#<!-- -->13 0x000070ce4bef7e9f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xef7e9f)
#<!-- -->14 0x000070ce54417310 clang::EmitBackendOutput(clang::DiagnosticsEngine&amp;, clang::HeaderSearchOptions const&amp;, clang::CodeGenOptions const&amp;, clang::TargetOptions const&amp;, clang::LangOptions const&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1c17310)
#<!-- -->15 0x000070ce547a4c31 clang::CodeGenAction::ExecuteAction() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1fa4c31)
#<!-- -->16 0x000070ce5520662c clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a0662c)
#<!-- -->17 0x000070ce551830b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29830b4)
#<!-- -->18 0x000070ce5528263d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a8263d)
#<!-- -->19 0x00005a22252e242e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-18/bin/clang+0x1442e)
#<!-- -->20 0x00005a22252df894 (/usr/lib/llvm-18/bin/clang+0x11894)
#<!-- -->21 0x000070ce54e33972 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2633972)
#<!-- -->22 0x000070ce4bceff77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-18/bin/../lib/libLLVM.so.18.1+0xceff77)
#<!-- -->23 0x000070ce54e33237 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2633237)
#<!-- -->24 0x000070ce54dfb518 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25fb518)
#<!-- -->25 0x000070ce54dfb77f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25fb77f)
#<!-- -->26 0x000070ce54e17c20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2617c20)
#<!-- -->27 0x00005a22252df1ec clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-18/bin/clang+0x111ec)
#<!-- -->28 0x00005a22252ec383 main (/usr/lib/llvm-18/bin/clang+0x1e383)
#<!-- -->29 0x000070ce4a82a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->30 0x000070ce4a82a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->31 0x00005a22252dc255 _start (/usr/lib/llvm-18/bin/clang+0xe255)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: riscv32-unknown-unknown-elf
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

## Steps to reproduce

- create `test.ll`:
```llvm
@<!-- -->mat = global [24 x [24 x i32]] zeroinitializer

define [24 x [24 x i32]] @<!-- -->init([24 x [24 x i32]] %.0) {
    ret [24 x [24 x i32]] %.0
}

define void @<!-- -->main() {
entry:
    %.1 = load [24 x [24 x i32]], ptr @<!-- -->mat
    %.2 = call [24 x [24 x i32]] @<!-- -->init([24 x [24 x i32]] %.1)
    store [24 x [24 x i32]] %.2, ptr @<!-- -->mat
    ret void 
}
```
- use clang to compile the llvm IR code:
```
clang -S --target=riscv32-unknown-elf test.ll -o test.s
```

### Environment Information
- OS: Ubuntu24.04
- Clang version: clang version 18.1.3

## Additional Information

The issue occurs during Prologue/Epilogue Insertion &amp; Frame Finalization pass which related to register scavenging. The issue disappears when struct size is reduced (e.g., to [3 x [3 x i32]]). Same code compiles successfully for other targets (e.g., x86_64).

## Technical Analysis

I encountered this problem while reviewing students' compiler project. 

Struct passing is syntactically valid in LLVM. While it is not recommended and inefficient to pass large data structures (higher register pressure during regalloc), the problem should be handled, either successfully compile using stack-based parameter passing for large data structures, or emit a clear diagnostic error about unsupported passing. Instead, clang crashes with internal failure, which may indicate a potential bug in risc-v backend's register scavenger.

</details>


---

