# Assertion `I->use_empty() && "trying to erase instruction with users."' failed.

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/174020
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-12-31T20:58:57Z

## Body

To reproduce run opt with the test below (-passes=slp-vectorizer):
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @wombat(i32 %arg, i32 %arg1, i64 %arg2) #0 gc "statepoint-example" {
bb:
  br label %bb3

bb3:                                              ; preds = %bb3, %bb
  %phi = phi i64 [ 0, %bb3 ], [ 0, %bb ]
  %phi4 = phi i64 [ %add39, %bb3 ], [ 0, %bb ]
  %or = or i64 %phi, 0
  %trunc = trunc i64 %or to i32
  %or5 = or i64 %phi, 0
  %mul = mul i64 %or, %or5
  %or6 = or i64 0, 0
  %or7 = or i64 0, 0
  %mul8 = mul i64 %or6, %or7
  %shl = shl i32 0, 1
  %or9 = or i64 %phi, 0
  %trunc10 = trunc i64 %or9 to i32
  %or11 = or i32 %trunc10, 0
  %or12 = or i64 %phi, 0
  %mul13 = mul i64 %or9, %or12
  %add = add i64 %mul, %mul8
  %sext = sext i32 %shl to i64
  %add14 = add i64 %add, %sext
  %xor = xor i32 0, %trunc
  %sext15 = sext i32 %xor to i64
  %add16 = add i64 %add14, %sext15
  %shl17 = shl i32 %shl, 0
  %add18 = add i64 1, 0
  %trunc19 = trunc i64 %add18 to i32
  %mul20 = mul i64 0, 0
  %add21 = add i64 %mul13, %add16
  %sext22 = sext i32 %shl17 to i64
  %add23 = add i64 %add21, %sext22
  %xor24 = xor i32 %arg, %trunc10
  %sext25 = sext i32 %xor24 to i64
  %add26 = add i64 %add23, %sext25
  %trunc27 = trunc i64 %arg2 to i32
  %add28 = add i64 %mul20, %add26
  %sext29 = sext i32 %shl to i64
  %add30 = add i64 %add28, %sext29
  %xor31 = xor i32 %arg1, %trunc19
  %sext32 = sext i32 %xor31 to i64
  %add33 = add i64 %add30, %sext32
 %shl34 = shl i32 %arg1, 0
  %sext35 = sext i32 %shl34 to i64
  %add36 = add i64 %add33, %sext35
  %xor37 = xor i32 %arg, %trunc27
  %sext38 = sext i32 %xor37 to i64
  %add39 = add i64 %add36, %sext38
  br i1 false, label %bb40, label %bb3

bb40:                                             ; preds = %bb3
  %phi41 = phi i64 [ %add39, %bb3 ]
  ret void
}

attributes #0 = { "target-cpu"="znver2" }
```
Reproducer: https://godbolt.org/z/fWzdn43sd


Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer <source>
1.	Running pass "function(slp-vectorizer)" on module "<source>"
2.	Running pass "slp-vectorizer" on function "wombat"
 #0 0x0000000005a65f78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a65f78)
 #1 0x0000000005a62e24 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000753390242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007533902969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000753390242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007533902287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000075339022871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000753390239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000378f07f llvm::slpvectorizer::BoUpSLP::~BoUpSLP() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x378f07f)
 #9 0x00000000038861d6 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#10 0x0000000003886c5b llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3886c5b)
#11 0x0000000003036a7e llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3036a7e)
#12 0x0000000005841e31 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5841e31)
#13 0x0000000000f19f8e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf19f8e)
#14 0x00000000058403ea llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58403ea)
#15 0x000000000097dfce llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97dfce)
#16 0x000000000583fda1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x583fda1)
#17 0x0000000000988280 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x988280)
#18 0x000000000097c178 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97c178)
#19 0x0000753390229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x0000753390229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x0000000000972c15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x972c15)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

