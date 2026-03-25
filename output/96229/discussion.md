# Unexpected Assertion `llvm::any_of(Markers, [Declare](auto *Assign) { return DebugVariableAggregate(Assign) == DebugVariableAggregate(Declare); })' failure

**Author:** sscalpone
**URL:** https://github.com/llvm/llvm-project/issues/96229

## Body

I'm seeing this assertion failure when building `git@github.com:fortran-lang/stdlib.git`

Using the default options when compiling `test/string/test_string_functions.f90`.

The default options are -g -O2.

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @_QMtest_string_functionsPtest_padl() {
  %1 = alloca [0 x i8], i64 0, align 1
  br label %3

2:                                                ; No predecessors!
    #dbg_declare(ptr %1, !4, !DIExpression(), !9)
  br label %3

3:                                                ; preds = %2, %0
  ret void
}

!llvm.module.flags = !{!0}
!llvm.dbg.cu = !{!1}

!0 = !{i32 2, !"Debug Info Version", i32 3}
!1 = distinct !DICompileUnit(language: DW_LANG_Fortran95, file: !2, producer: "flang version 19.0.0 (/proj/build/llvm/Linux_x86_64/flang 0ec567c370df86893a22bf59d2716f6e553ca63b)", isOptimized: true, runtimeVersion: 0, emissionKind: FullDebug, globals: !3)
!2 = !DIFile(filename: "test_string_functions.f90", directory: "/local/home/sscalpone/stdlib/stdlib/build/test/string/../../../test/string")
!3 = !{}
!4 = !DILocalVariable(name: "string", scope: !5, file: !2, line: 549, type: !8)
!5 = distinct !DISubprogram(name: "test_padl", linkageName: "_QMtest_string_functionsPtest_padl", scope: !6, file: !2, line: 503, type: !7, scopeLine: 503, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !1)
!6 = !DIModule(scope: !1, name: "test_string_functions", file: !2, line: 15)
!7 = distinct !DISubroutineType(cc: DW_CC_normal, types: !3)
!8 = !DIBasicType(name: "integer", size: 32, encoding: DW_ATE_signed)
!9 = !DILocation(line: 549, column: 9, scope: !5)
```
```
% clang -O2 reduced.ll
clang-19: /proj/build/llvm/Linux_x86_64/llvm/lib/IR/DebugInfo.cpp:2336: llvm::AssignmentTrackingPass::runOnFunction(llvm::Function&)::<lambda(const auto:5&, auto:6&)> [with auto:5 = llvm::SmallVector<llvm::DbgVariableRecord*>; auto:6 = llvm::SmallPtrSet<llvm::DbgVariableRecord*, 2>]: Assertion `llvm::any_of(Markers, [Declare](auto *Assign) { return DebugVariableAggregate(Assign) == DebugVariableAggregate(Declare); })' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name reduced.ll -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/local/home/sscalpone/stdlib/stdlib/build/test/string -fcoverage-compilation-dir=/local/home/sscalpone/stdlib/stdlib/build/test/string -resource-dir /proj/nv/llvm/Linux_x86_64/llvm-3044/lib/clang/19 -O2 -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/reduced-dfafb1.o -x ir reduced.ll
1.      Optimizer
 #0 0x000056512dfa00db llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0x23a00db)
 #1 0x000056512df9d634 SignalHandler(int) Signals.cpp:0:0
 #2 0x00007fa255854db0 __restore_rt (/lib64/libc.so.6+0x54db0)
 #3 0x00007fa2558a154c __pthread_kill_implementation (/lib64/libc.so.6+0xa154c)
 #4 0x00007fa255854d06 gsignal (/lib64/libc.so.6+0x54d06)
 #5 0x00007fa2558287f3 abort (/lib64/libc.so.6+0x287f3)
 #6 0x00007fa25582871b _nl_load_domain.cold (/lib64/libc.so.6+0x2871b)
 #7 0x00007fa25584dca6 (/lib64/libc.so.6+0x4dca6)
 #8 0x000056512d93bbbc llvm::AssignmentTrackingPass::runOnFunction(llvm::Function&) (.part.0) DebugInfo.cpp:0:0
 #9 0x000056512d93c6ca llvm::AssignmentTrackingPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0x1d3c6ca)
#10 0x000056512e222742 llvm::detail::PassModel<llvm::Module, llvm::AssignmentTrackingPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0x2622742)
#11 0x000056512da6518d llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0x1e6518d)
#12 0x000056512e234dc0 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#13 0x000056512e2384b9 (anonymous namespace)::EmitAssemblyHelper::EmitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) BackendUtil.cpp:0:0
#14 0x000056512e238eb3 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0x2638eb3)
#15 0x000056512e8ba8a2 clang::CodeGenAction::ExecuteAction() (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0x2cba8a2)
#16 0x000056512eb4d1e1 clang::FrontendAction::Execute() (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0x2f4d1e1)
#17 0x000056512ead18de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0x2ed18de)
#18 0x000056512ec3cdfe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0x303cdfe)
#19 0x000056512ca9494c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0xe9494c)
#20 0x000056512ca8de0a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#21 0x000056512ca916ee clang_main(int, char**, llvm::ToolContext const&) (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0xe916ee)
#22 0x000056512c9c0347 main (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0xdc0347)
#23 0x00007fa25583feb0 __libc_start_call_main (/lib64/libc.so.6+0x3feb0)
#24 0x00007fa25583ff60 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3ff60)
#25 0x000056512ca8d89e _start (/proj/nv/llvm/Linux_x86_64/llvm-3044/bin/clang-19+0xe8d89e)
clang: error: unable to execute command: Aborted (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.0.0git (/proj/build/llvm/Linux_x86_64/clang 0ec567c370df86893a22bf59d2716f6e553ca63b)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /proj/nv/llvm/Linux_x86_64/llvm-3044/bin
Build config: +assertions
clang: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

## Comments

### Comment 1 - sscalpone

@abidh Any ideas for next steps?

---

### Comment 2 - abidh

> @abidh Any ideas for next steps?

Thanks for reporting this. If a type is unhandled in debug info currently, we use a place holder integer type for it. I think this integer type is having problem with a 0 length array type in alloca. I changed the alloca to the following and the problem goes away.

` %1 = alloca [4 x i8], i64 0, align 1`

As this problem seems to be related to string, I already have a PR open (#95906) which handles allocatable strings and I am working on another to handle assumed length character arrays. I believe those will help with such issues.

In any case, if you could let me know how to run this testsuite with flang then I will investigate to see if I could find a quick solution or workaround for you. I tried running the single file `test/string/test_string_functions.f90` and it fails due to missing modules and my quick attempt to run the whole testsuite using cmake failed. So a recipe to run the testsuite will be very helpful.

---

### Comment 3 - tarunprabhu

@sscalpone, Does this failure still occur?

---

