# Assertion `all_of(Bundles, [](const ScheduleBundle *Bundle) { return Bundle->isScheduled(); }) && "must be scheduled at this point"' failed.

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/176658
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2026-01-18T21:37:58Z

## Body

To reproduce run opt with the test below (-passes=slp-vectorizer -slp-threshold=-99999).
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define i32 @wombat() #0 gc "statepoint-example" {
bb:
  br label %bb3

bb1:                                              ; preds = %bb3
  %phi = phi i32 [ %or, %bb3 ]
  %phi2 = phi i32 [ %ashr7, %bb3 ]
  ret i32 0

bb3:                                              ; preds = %bb3, %bb
  %phi4 = phi i32 [ 0, %bb ], [ %or, %bb3 ]
  %phi5 = phi i32 [ 0, %bb ], [ %or8, %bb3 ]
  %load = load i64, ptr addrspace(1) null, align 8
  %trunc = trunc i64 %load to i32
  %or = or i32 %phi4, %trunc
  %trunc6 = trunc i64 0 to i32
  %and = and i32 %trunc6, 0
  %ashr = ashr i32 0, %and
  %ashr7 = ashr i32 %phi5, %and
  %or8 = or i32 %ashr7, 0
  br i1 false, label %bb1, label %bb3
}

attributes #0 = { "target-features"="+prfchw,-cldemote,+avx,+aes,+sahf,+pclmul,-xop,+crc32,-amx-fp8,+xsaves,-avx512fp16,-usermsr,-sm4,-egpr,+sse4.1,-avx10.1,-avx512ifma,+xsave,+sse4.2,-tsxldtrk,-sm3,-ptwrite,-widekl,-movrs,+invpcid,+64bit,+xsavec,-avx512vpopcntdq,+cmov,-avx512vp2intersect,+avx512cd,+movbe,-avxvnniint8,-ccmp,-amx-int8,-kl,-sha512,-avxvnni,+rtm,+adx,+avx2,-hreset,-movdiri,-serialize,-vpclmulqdq,+avx512vl,-uintr,-cf,+clflushopt,-raoint,-cmpccxadd,+bmi,-amx-tile,+sse,-gfni,-avxvnniint16,-amx-fp16,-zu,-ndd,+xsaveopt,+rdrnd,+avx512f,-amx-bf16,-avx512bf16,-avx512vnni,-push2pop2,+cx8,+avx512bw,+sse3,+pku,-nf,-amx-tf32,-amx-avx512,+fsgsbase,-clzero,-mwaitx,-lwp,+lzcnt,-sha,-movdir64b,-ppx,-wbnoinvd,-enqcmd,-avxneconvert,-tbm,-pconfig,-amx-complex,+ssse3,+cx16,-avx10.2,+bmi2,+fma,+popcnt,-avxifma,+f16c,-avx512bitalg,-rdpru,+clwb,+mmx,+sse2,+rdseed,-avx512vbmi2,-prefetchi,-amx-movrs,-rdpid,-fma4,-avx512vbmi,-shstk,-vaes,-waitpkg,-sgx,+fxsr,+avx512dq,-sse4a,-avx512f" }
```
Reproducer: https://godbolt.org/z/EsrKx1G99

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -slp-threshold=-99999 <source>
1.	Running pass "function(slp-vectorizer)" on module "<source>"
2.	Running pass "slp-vectorizer" on function "wombat"
 #0 0x0000000005a776e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a776e8)
 #1 0x0000000005a745c4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000077e368c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000077e368c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000077e368c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000077e368c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000077e368c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000077e368c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000037f427a llvm::slpvectorizer::BoUpSLP::scheduleBlock(llvm::slpvectorizer::BoUpSLP const&, llvm::slpvectorizer::BoUpSLP::BlockScheduling*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37f427a)
 #9 0x000000000384885a llvm::slpvectorizer::BoUpSLP::vectorizeTree(llvm::SmallDenseSet<llvm::Value*, 4u, llvm::DenseMapInfo<llvm::Value*, void>> const&, llvm::Instruction*, llvm::ArrayRef<std::tuple<llvm::Value*, unsigned int, bool>>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x384885a)
#10 0x000000000384ce24 llvm::slpvectorizer::BoUpSLP::vectorizeTree() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x384ce24)
#11 0x000000000386dbb3 llvm::SLPVectorizerPass::tryToVectorizeList(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x386dbb3)
#12 0x000000000387247c bool tryToVectorizeSequence<llvm::Value>(llvm::SmallVectorImpl<llvm::Value*>&, llvm::function_ref<bool (llvm::Value*, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, bool)>, bool, llvm::slpvectorizer::BoUpSLP&) (.constprop.0) SLPVectorizer.cpp:0:0
#13 0x000000000387370c llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x387370c)
#14 0x000000000387b626 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#15 0x000000000387c17b llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x387c17b)
#16 0x00000000030232ee llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x30232ee)
#17 0x00000000057eb4e1 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57eb4e1)
#18 0x0000000000f07e1e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf07e1e)
#19 0x00000000057e9a9a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57e9a9a)
#20 0x000000000096469e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96469e)
#21 0x00000000057e9451 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57e9451)
#22 0x000000000096e85a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96e85a)
#23 0x0000000000962908 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x962908)
#24 0x000077e368c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x000077e368c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000959585 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x959585)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

