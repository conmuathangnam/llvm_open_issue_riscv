# [DA] Assertion `BitWidth == RHS.BitWidth && "Comparison requires equal bit widths"'

**Author:** scui-ibm
**URL:** https://github.com/llvm/llvm-project/issues/165014
**Status:** Closed
**Labels:** crash, llvm:analysis
**Closed Date:** 2025-12-04T21:01:46Z

## Body

test case:

```
define void @f1() {
entry:
  br label %for.1.header

for.1.header:                                     ; preds = %for.2.end, %entry
  br label %for.1.body

for.1.body:                                       ; preds = %for.1.body, %whiledo
  %0 = phi i32 [ 0, %for.1.header ], [ 1, %for.1.body ]
  store i32 0, ptr null, align 4
  %1 = icmp ult i32 %0, 1
  br i1 %1, label %for.1.body, label %for.1.end

for.1.end:                                        ; preds = %for.1.body
  br label %for.2.body

for.2.body:                                       ; preds = %for.2.body, %for.1.end
  %2 = load i32, ptr null, align 4
  br i1 false, label %for.2.body, label %exit

exit:                                             ; preds = %for.2.body
  ret void
}
```

Compiled with:

`opt -disable-output "-passes=print<da>" -aa-pipeline=basic-aa`

assertion:

```
opt: /home/scui/llvm/dev/llvm-project/llvm/include/llvm/ADT/APInt.h:1057: bool llvm::APInt::operator==(const APInt &) const: Assertion `BitWidth == RHS.BitWidth && "Comparison requires equal bit widths"' failed.
PLEASE submit a bug report to https://ibm.biz/openxlcpp-support and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: opt -disable-output -passes=print<da> -aa-pipeline=basic-aa t.ll
1.	Running pass "function(print<da>)" on module "t.ll"
2.	Running pass "print<da>" on function "f1"
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  opt               0x000015d26eb9268c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int, void*) + 124
1  opt               0x000015d26eb92e34
2  opt               0x000015d26eb8efbc llvm::sys::RunSignalHandlers() + 284
3  opt               0x000015d26eb93b4c
4  linux-vdso64.so.1 0x00007afbe77b0444 __kernel_sigtramp_rt64 + 0
5  libc.so.6         0x00007afbe6f455fc pthread_kill + 396
6  libc.so.6         0x00007afbe6edbd5c gsignal + 44
7  libc.so.6         0x00007afbe6eb645c abort + 340
8  libc.so.6         0x00007afbe6ecd824
9  libc.so.6         0x00007afbe6ecd8c4 __assert_fail + 100
10 opt               0x000015d26edf0ed8 llvm::ICmpInst::compare(llvm::APInt const&, llvm::APInt const&, llvm::CmpInst::Predicate) + 424
11 opt               0x000015d26f59f858 llvm::ScalarEvolution::SimplifyICmpOperands(llvm::CmpPredicate&, llvm::SCEV const*&, llvm::SCEV const*&, unsigned int) + 1240
12 opt               0x000015d26f5a8c00 llvm::ScalarEvolution::isKnownPredicate(llvm::CmpPredicate, llvm::SCEV const*, llvm::SCEV const*) + 80
13 opt               0x000015d26fbdb8d0 llvm::DependenceInfo::haveSameSD(llvm::Loop const*, llvm::Loop const*) const + 336
14 opt               0x000015d26fbdbbf0 llvm::DependenceInfo::establishNestingLevels(llvm::Instruction const*, llvm::Instruction const*) + 688
15 opt               0x000015d26fbf4184 llvm::DependenceInfo::depends(llvm::Instruction*, llvm::Instruction*, bool) + 3236
16 opt               0x000015d26fbd7130
17 opt               0x000015d26fbd7940 llvm::DependenceAnalysisPrinterPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) + 576
18 opt               0x000015d2711803cc
19 opt               0x000015d26eebb360 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) + 480
20 opt               0x000015d270653d7c
21 opt               0x000015d26eec1084 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) + 580
22 opt               0x000015d2705d79ac
23 opt               0x000015d26eeb9b20 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) + 480
24 opt               0x000015d2705cb81c llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) + 16892
25 opt               0x000015d26eb52fc8 optMain + 8776
26 opt               0x000015d26eb4b2c0 main + 32
27 libc.so.6         0x00007afbe6eb6ca4
28 libc.so.6         0x00007afbe6eb6eec __libc_start_main + 428
```

