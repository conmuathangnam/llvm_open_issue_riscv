# Instruction does not dominate all uses! LLVM ERROR: Broken module found, compilation aborted! Yet another.

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/163404
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-10-17T13:00:01Z

## Body

To reproduce run opt with the following test, using -passes=slp-vectorizer -slp-threshold=-99999
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @wombat() #0 gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb16, %bb1, %bb
  %phi = phi i32 [ 0, %bb ], [ 0, %bb1 ], [ %phi17, %bb16 ]
  %phi2 = phi i32 [ 0, %bb ], [ 0, %bb1 ], [ %phi18, %bb16 ]
  %phi3 = phi i32 [ 0, %bb ], [ poison, %bb16 ], [ 0, %bb1 ]
  %phi4 = phi i32 [ 0, %bb ], [ poison, %bb16 ], [ 0, %bb1 ]
  br i1 false, label %bb1, label %bb5

bb5:                                              ; preds = %bb16, %bb1
  %phi6 = phi i32 [ %phi17, %bb16 ], [ 0, %bb1 ]
  %phi7 = phi i32 [ %phi19, %bb16 ], [ 0, %bb1 ]
  %phi8 = phi double [ 0.000000e+00, %bb16 ], [ 0.000000e+00, %bb1 ]
  switch i32 0, label %bb21 [
    i32 4, label %bb21
    i32 1, label %bb21
    i32 0, label %bb9
  ]

bb9:                                              ; preds = %bb21, %bb5
  %phi10 = phi i32 [ %phi6, %bb21 ], [ 0, %bb5 ]
  %phi11 = phi i32 [ %phi7, %bb21 ], [ 0, %bb5 ]
  %phi12 = phi i32 [ 0, %bb21 ], [ 0, %bb5 ]
  %phi13 = phi double [ 0.000000e+00, %bb21 ], [ 0.000000e+00, %bb5 ]
  switch i32 0, label %bb15 [
    i32 1, label %bb14
    i32 0, label %bb16
  ]

bb14:                                             ; preds = %bb9
  br label %bb16

bb15:                                             ; preds = %bb9
  %add = add i32 0, %phi10
  br label %bb16

bb16:                                             ; preds = %bb15, %bb14, %bb9
  %phi17 = phi i32 [ %add, %bb15 ], [ %phi10, %bb14 ], [ 0, %bb9 ]
  %phi18 = phi i32 [ %phi11, %bb15 ], [ 0, %bb14 ], [ 0, %bb9 ]
  %phi19 = phi i32 [ %phi12, %bb15 ], [ %phi12, %bb14 ], [ 0, %bb9 ]
  %phi20 = phi double [ 0.000000e+00, %bb15 ], [ 0.000000e+00, %bb14 ], [ 0.000000e+00, %bb9 ]
  br i1 false, label %bb5, label %bb1

bb21:                                             ; preds = %bb5, %bb5, %bb5
  br label %bb9
}

attributes #0 = { "target-features"="+prfchw,-cldemote,+avx,+aes,+sahf,+pclmul,-xop,+crc32,-amx-fp8,+xsaves,-avx512fp16,-usermsr,-sm4,-egpr,+sse4.1,-avx10.1,-avx512ifma,+xsave,+sse4.2,-tsxldtrk,-sm3,-ptwrite,-widekl,-movrs,+invpcid,+64bit,+xsavec,-avx512vpopcntdq,+cmov,-avx512vp2intersect,+avx512cd,+movbe,-avxvnniint8,-ccmp,-amx-int8,-kl,-sha512,-avxvnni,+rtm,+adx,+avx2,-hreset,-movdiri,-serialize,-vpclmulqdq,+avx512vl,-uintr,-cf,+clflushopt,-raoint,-cmpccxadd,+bmi,-amx-tile,+sse,-gfni,-avxvnniint16,-amx-fp16,-zu,-ndd,+xsaveopt,+rdrnd,+avx512f,-amx-bf16,-avx512bf16,-avx512vnni,-push2pop2,+cx8,+avx512bw,+sse3,+pku,-nf,-amx-tf32,-amx-avx512,+fsgsbase,-clzero,-mwaitx,-lwp,+lzcnt,-sha,-movdir64b,-ppx,-wbnoinvd,-enqcmd,-amx-transpose,-avxneconvert,-tbm,-pconfig,-amx-complex,+ssse3,+cx16,-avx10.2,+bmi2,+fma,+popcnt,-avxifma,+f16c,-avx512bitalg,-rdpru,+clwb,+mmx,+sse2,+rdseed,-avx512vbmi2,-prefetchi,-amx-movrs,-rdpid,-fma4,-avx512vbmi,-shstk,-vaes,-waitpkg,-sgx,+fxsr,+avx512dq,-sse4a,-avx512f" }
```
Reproducer: https://godbolt.org/z/rqexMdzW8

Stack dump:
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -slp-threshold=-99999 <source>
1.	Running pass "verify" on module "<source>"
 #0 0x000000000594c8f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x594c8f8)
 #1 0x00000000059497a4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000070d580e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000070d580e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000070d580e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000070d580e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000000000083379d llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x000000000587b3f1 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x587b3f1)
 #8 0x0000000005768c88 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5768c88)
 #9 0x000000000097812e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97812e)
#10 0x0000000005726041 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5726041)
#11 0x000000000098226a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98226a)
#12 0x00000000009764ee optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9764ee)
#13 0x000070d580e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x000070d580e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x000000000096d845 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96d845)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


