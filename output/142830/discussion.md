# LLVM crash "Instruction does not dominate all uses" introduced recently

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/142830
**Status:** Closed
**Labels:** llvm:codegen, crash-on-valid
**Closed Date:** 2025-06-06T11:38:32Z

## Body

A recent commit (around  62fd4d18e52e61fe1e67cbf9d1c4355a34f84325 - 24b97756decb7bf0e26dcf0e30a7a9aaf27f417c, but I haven't yet pinpointed the culprit) introduced a crash in LLVM: https://gcc.godbolt.org/z/YT8dcvY9e

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.foo = type { %struct.eggs }
%struct.eggs = type { [2 x float] }

define float @wibble.1(ptr %arg) {
bb:
  %load = load float, ptr %arg, align 4
  ret float %load
}

define i64 @foo.2() {
bb:
  %load = load i64, ptr inttoptr (i64 8 to ptr), align 8
  ret i64 %load
}

define ptr @ham.3(ptr %arg, i64 %arg1) {
bb:
  %load = load ptr, ptr %arg, align 8
  %getelementptr = getelementptr %struct.foo, ptr %load, i64 %arg1
  ret ptr %getelementptr
}

define float @wombat(ptr %arg) {
bb:
  %call = call float @eggs(ptr %arg)
  ret float %call
}

define float @eggs(ptr %arg) {
bb:
  %call = call float @barney(ptr %arg)
  ret float %call
}

define float @barney(ptr %arg) {
bb:
  %load = load float, ptr %arg, align 4
  ret float %load
}

define float @bar.4(ptr %arg) {
bb:
  %call = call float @wombat.5(ptr %arg)
  ret float %call
}

define float @wombat.5(ptr %arg) {
bb:
  %call = call float @wombat(ptr %arg)
  %fneg = fneg float %call
  ret float %fneg
}

define <2 x float> @barney.6(ptr %arg, ptr %arg1) {
bb:
  %load = load float, ptr %arg1, align 4
  %call = call float @wibble.1(ptr %arg)
  %fsub = fsub float %load, %call
  %fcmp = fcmp oeq float %fsub, 0.000000e+00
  br i1 %fcmp, label %bb2, label %bb3

bb2:                                              ; preds = %bb3, %bb
  ret <2 x float> zeroinitializer

bb3:                                              ; preds = %bb
  %call4 = call float @wibble.1(ptr %arg)
  %fsub5 = fsub float 0.000000e+00, %call4
  %fmul = fmul float %fsub5, 0.000000e+00
  %fcmp6 = fcmp olt float %fmul, 0.000000e+00
  br i1 %fcmp6, label %bb7, label %bb2

bb7:                                              ; preds = %bb3
  %load8 = load <2 x float>, ptr %arg, align 4
  ret <2 x float> %load8
}

define double @snork(ptr %arg, ptr %arg1) {
bb:
  %alloca = alloca %struct.foo, align 4
  %call = call <2 x float> @barney.6(ptr %arg, ptr %arg1)
  store <2 x float> %call, ptr %alloca, align 4
  %call2 = call float @bar.4(ptr %alloca)
  %fpext = fpext float %call2 to double
  ret double %fpext
}

define double @pluto(ptr %arg) {
bb:
  br label %bb1

bb1:                                              ; preds = %bb4, %bb
  %phi = phi i32 [ 0, %bb ], [ %add, %bb4 ]
  %phi2 = phi double [ 1.000000e+00, %bb ], [ %select, %bb4 ]
  %zext = zext i32 %phi to i64
  %call = call i64 @foo.2()
  %icmp = icmp ugt i64 %call, %zext
  br i1 %icmp, label %bb4, label %bb3

bb3:                                              ; preds = %bb1
  ret double %phi2

bb4:                                              ; preds = %bb1
  %call5 = call ptr @ham.3(ptr %arg, i64 %zext)
  %add = add nsw i32 %phi, 1
  %zext6 = zext i32 %add to i64
  %call7 = call i64 @foo.2()
  %urem = urem i64 %zext6, %call7
  %call8 = call ptr @ham.3(ptr %arg, i64 %urem)
  %call9 = call double @snork(ptr %call5, ptr %call8)
  %fcmp = fcmp ogt double %phi2, 0.000000e+00
  %select = select i1 %fcmp, double %call9, double 0.000000e+00
  br label %bb1
}
```

```
Instruction does not dominate all uses!
  %indvars.iv.next = add i64 %indvars.iv, 1
  %sunkaddr16 = mul i64 %indvars.iv.next, 8
in function pluto
fatal error: error in backend: Broken function found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -xir -O3 <source>
1.	Code generation
2.	Running pass 'Function Pass Manager' on module '<source>'.
3.	Running pass 'Module Verifier' on function '@pluto'
 #0 0x0000000003fd31e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd31e8)
 #1 0x0000000003fd0e74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fd0e74)
 #2 0x0000000003f16136 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f16136)
 #3 0x0000000003fc861e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc861e)
 #4 0x0000000000db7010 LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003f20f13 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f20f13)
 #6 0x0000000003f21071 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f21071)
 #7 0x00000000039c7fe2 (anonymous namespace)::VerifierLegacyPass::runOnFunction(llvm::Function&) Verifier.cpp:0:0
 #8 0x0000000003911bbf llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3911bbf)
 #9 0x0000000003911f71 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3911f71)
#10 0x0000000003912811 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3912811)
#11 0x0000000004291169 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4291169)
#12 0x0000000004988ac1 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4988ac1)
#13 0x0000000004c81405 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81405)
#14 0x0000000004bfea3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfea3e)
#15 0x0000000004d735c9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d735c9)
#16 0x0000000000db970f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb970f)
#17 0x0000000000db031a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#18 0x00000000049ef259 void llvm::function_ref<void()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#19 0x0000000003f16074 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f16074)
#20 0x00000000049ef86f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#21 0x00000000049b13bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b13bd)
#22 0x00000000049b244e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49b244e)
#23 0x00000000049ba545 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ba545)
#24 0x0000000000db5c1f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5c1f)
#25 0x0000000000c3a524 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc3a524)
#26 0x00007437c8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#27 0x00007437c8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#28 0x0000000000dafdc5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdafdc5)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```



## Comments

### Comment 1 - alexfh

@fhahn @lukel97 

---

### Comment 2 - topperc

opt-bisect-limit says that it fails after CodeGenPrepare

---

### Comment 3 - alexfh

The culprit is 24b97756decb7bf0e26dcf0e30a7a9aaf27f417c and the later recommit (f98bdd94e61721ebe11108956e63510692db34b2) doesn't help.

---

### Comment 4 - fhahn

I don't think the patch is at fault, AFAICT the IR after peeling is valid: https://llvm.godbolt.org/z/5han5TYzK

---

### Comment 5 - fhahn

This is a bug in CodeGenPrepare: https://gcc.godbolt.org/z/3fWT6W6MP

---

### Comment 6 - fhahn

CGP fix: https://github.com/llvm/llvm-project/pull/142949

---

