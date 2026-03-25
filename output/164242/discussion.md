# Assertion `all_of(Bundles, [](const ScheduleBundle *Bundle) { return Bundle->isScheduled(); }) && "must be scheduled at this point"' failed.

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/164242
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-10-20T22:54:27Z

## Body

To reproduce run opt with the test below using -passes=slp-vectorizer -slp-threshold=-99999:
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @wombat() #0 gc "statepoint-example" {
bb:
  br i1 false, label %bb1, label %bb6

bb1:                                              ; preds = %bb
  %add = add i32 0, 0
  %shl = shl i32 %add, 0
  %sub = sub i32 0, 1
  %add2 = add i32 %sub, %shl
  %add3 = add i32 0, 0
  %shl4 = shl i32 %add3, 0
  %ashr = ashr i32 %shl4, 1
  %add5 = add i32 0, 0
  br label %bb6

bb6:                                              ; preds = %bb1, %bb
  %phi = phi i32 [ poison, %bb ], [ %add2, %bb1 ]
  %phi7 = phi i32 [ 0, %bb ], [ %ashr, %bb1 ]
  %phi8 = phi i32 [ 0, %bb ], [ %add2, %bb1 ]
  %phi9 = phi i32 [ 0, %bb ], [ %add5, %bb1 ]
  %or = or i32 %phi8, 0
  ret void
}

attributes #0 = { "target-features"="+prfchw,-cldemote,+avx,+aes,+sahf,+pclmul,-xop,+crc32,-amx-fp8,+xsaves,-avx512fp16,-usermsr,-sm4,-egpr,+sse4.1,-avx10.1,-avx512ifma,+xsave,+sse4.2,-tsxldtrk,-sm3,-ptwrite,-widekl,-movrs,-invpcid,+64bit,+xsavec,-avx512vpopcntdq,+cmov,-avx512vp2intersect,-avx512cd,+movbe,-avxvnniint8,-ccmp,-amx-int8,-kl,-sha512,-avxvnni,-rtm,+adx,+avx2,-hreset,-movdiri,-serialize,-vpclmulqdq,-avx512vl,-uintr,-cf,+clflushopt,-raoint,-cmpccxadd,+bmi,-amx-tile,+sse,-gfni,-avxvnniint16,-amx-fp16,-zu,-ndd,+xsaveopt,+rdrnd,-avx512f,-amx-bf16,-avx512bf16,-avx512vnni,-push2pop2,+cx8,-avx512bw,+sse3,-pku,-nf,-amx-tf32,-amx-avx512,+fsgsbase,+clzero,-mwaitx,-lwp,+lzcnt,+sha,-movdir64b,-ppx,+wbnoinvd,-enqcmd,-amx-transpose,-avxneconvert,-tbm,-pconfig,-amx-complex,+ssse3,+cx16,-avx10.2,+bmi2,+fma,+popcnt,-avxifma,+f16c,-avx512bitalg,-rdpru,+clwb,+mmx,+sse2,+rdseed,-avx512vbmi2,-prefetchi,-amx-movrs,+rdpid,-fma4,-avx512vbmi,-shstk,-vaes,-waitpkg,-sgx,+fxsr,-avx512dq,+sse4a" }
```
Reproducer: https://godbolt.org/z/rf8Y3ezGe

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -slp-threshold=-99999 <source>
1.	Running pass "function(slp-vectorizer)" on module "<source>"
2.	Running pass "slp-vectorizer" on function "wombat"
 #0 0x0000000005945ce8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5945ce8)
 #1 0x0000000005942b94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007ed06f642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007ed06f6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007ed06f642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007ed06f6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007ed06f62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007ed06f639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000037732cf llvm::slpvectorizer::BoUpSLP::scheduleBlock(llvm::slpvectorizer::BoUpSLP const&, llvm::slpvectorizer::BoUpSLP::BlockScheduling*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37732cf)
 #9 0x000000000378dbba llvm::slpvectorizer::BoUpSLP::vectorizeTree(llvm::SmallDenseSet<llvm::Value*, 4u, llvm::DenseMapInfo<llvm::Value*, void>> const&, llvm::Instruction*, llvm::ArrayRef<std::tuple<llvm::Value*, unsigned int, bool>>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x378dbba)
#10 0x0000000003792012 llvm::slpvectorizer::BoUpSLP::vectorizeTree() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3792012)
#11 0x00000000037b2fa8 llvm::SLPVectorizerPass::tryToVectorizeList(llvm::ArrayRef<llvm::Value*>, llvm::slpvectorizer::BoUpSLP&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37b2fa8)
#12 0x00000000037b7c9f bool tryToVectorizeSequence<llvm::Value>(llvm::SmallVectorImpl<llvm::Value*>&, llvm::function_ref<bool (llvm::Value*, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, llvm::Value*)>, llvm::function_ref<bool (llvm::ArrayRef<llvm::Value*>, bool)>, bool, llvm::slpvectorizer::BoUpSLP&) (.constprop.0) SLPVectorizer.cpp:0:0
#13 0x00000000037b900b llvm::SLPVectorizerPass::vectorizeChainsInBlock(llvm::BasicBlock*, llvm::slpvectorizer::BoUpSLP&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37b900b)
#14 0x00000000037c0e26 llvm::SLPVectorizerPass::runImpl(llvm::Function&, llvm::ScalarEvolution*, llvm::TargetTransformInfo*, llvm::TargetLibraryInfo*, llvm::AAResults*, llvm::LoopInfo*, llvm::DominatorTree*, llvm::AssumptionCache*, llvm::DemandedBits*, llvm::OptimizationRemarkEmitter*) (.part.0) SLPVectorizer.cpp:0:0
#15 0x00000000037c193b llvm::SLPVectorizerPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x37c193b)
#16 0x0000000002f8895e llvm::detail::PassModel<llvm::Function, llvm::SLPVectorizerPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2f8895e)
#17 0x00000000057202b1 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57202b1)
#18 0x0000000000ef5fce llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xef5fce)
#19 0x000000000571e81a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x571e81a)
#20 0x000000000097887e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97887e)
#21 0x000000000571e1d1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x571e1d1)
#22 0x000000000098298a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98298a)
#23 0x0000000000976c0e optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x976c0e)
#24 0x00007ed06f629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x00007ed06f629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x000000000096df65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96df65)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

