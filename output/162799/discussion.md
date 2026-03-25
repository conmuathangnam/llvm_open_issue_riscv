# Instruction does not dominate all uses, LLVM ERROR: Broken module found, compilation aborted! Yet another

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/162799
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-10-12T20:41:16Z

## Body

To reproduce run opt with the following test, using -passes=slp-vectorizer -slp-threshold=-99999 
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @wombat(i32 %arg, i32 %arg1) #0 gc "statepoint-example" {
bb:
  br label %bb6

bb2:                                              ; preds = %bb19
  %phi = phi i32 [ %phi21, %bb19 ]
  %phi3 = phi i32 [ %phi22, %bb19 ]
  %phi4 = phi i32 [ %phi23, %bb19 ]
  %phi5 = phi i32 [ %phi24, %bb19 ]
  ret void

bb6:                                              ; preds = %bb27, %bb26, %bb25, %bb
  %phi7 = phi i32 [ 0, %bb ], [ %phi24, %bb26 ], [ %phi24, %bb27 ], [ 0, %bb25 ]
  %phi8 = phi i32 [ 0, %bb ], [ %arg1, %bb26 ], [ %phi23, %bb27 ], [ 0, %bb25 ]
  %phi9 = phi i32 [ 0, %bb ], [ %phi22, %bb26 ], [ %phi20, %bb27 ], [ 0, %bb25 ]
  %phi10 = phi i32 [ 0, %bb ], [ %phi21, %bb26 ], [ %phi21, %bb27 ], [ 0, %bb25 ]
  switch i8 0, label %bb11 [
    i8 0, label %bb28
  ]

bb11:                                             ; preds = %bb28, %bb6
  %phi12 = phi i32 [ 0, %bb28 ], [ 0, %bb6 ]
  %phi13 = phi i32 [ %phi10, %bb28 ], [ 0, %bb6 ]
  %phi14 = phi i32 [ %phi9, %bb28 ], [ 0, %bb6 ]
  %phi15 = phi i32 [ %phi8, %bb28 ], [ 0, %bb6 ]
  %phi16 = phi i32 [ %phi7, %bb28 ], [ 0, %bb6 ]
  switch i8 0, label %bb19 [
    i8 1, label %bb17
    i8 0, label %bb18
  ]

bb17:                                             ; preds = %bb11
  %add = add i32 %phi16, 0
  br label %bb19

bb18:                                             ; preds = %bb11
  br label %bb19

bb19:                                             ; preds = %bb18, %bb17, %bb11
  %phi20 = phi i32 [ 0, %bb17 ], [ %arg, %bb18 ], [ %phi12, %bb11 ]
  %phi21 = phi i32 [ %phi13, %bb17 ], [ %phi12, %bb18 ], [ 0, %bb11 ]
  %phi22 = phi i32 [ %phi14, %bb17 ], [ 0, %bb18 ], [ 0, %bb11 ]
  %phi23 = phi i32 [ %phi15, %bb17 ], [ %arg, %bb18 ], [ %arg, %bb11 ]
  %phi24 = phi i32 [ %add, %bb17 ], [ %phi16, %bb18 ], [ %phi16, %bb11 ]
  br i1 false, label %bb2, label %bb25

bb25:                                             ; preds = %bb19
  switch i8 0, label %bb6 [
    i8 0, label %bb26
    i8 1, label %bb27
    i8 6, label %bb27
  ]

bb26:                                             ; preds = %bb25
  br label %bb6

bb27:                                             ; preds = %bb25, %bb25
  br label %bb6

bb28:                                             ; preds = %bb6
  br label %bb11
}

attributes #0 = { "target-features"="+prfchw,-cldemote,+avx,+aes,+sahf,+pclmul,-xop,+crc32,-amx-fp8,+xsaves,-avx512fp16,-usermsr,-sm4,-egpr,+sse4.1,-avx10.1,-avx512ifma,+xsave,+sse4.2,-tsxldtrk,-sm3,-ptwrite,-widekl,-movrs,-invpcid,+64bit,+xsavec,-avx512vpopcntdq,+cmov,-avx512vp2intersect,-avx512cd,+movbe,-avxvnniint8,-ccmp,-amx-int8,-kl,-sha512,-avxvnni,-rtm,+adx,+avx2,-hreset,-movdiri,-serialize,-vpclmulqdq,-avx512vl,-uintr,-cf,+clflushopt,-raoint,-cmpccxadd,+bmi,-amx-tile,+sse,-gfni,-avxvnniint16,-amx-fp16,-zu,-ndd,+xsaveopt,+rdrnd,-avx512f,-amx-bf16,-avx512bf16,-avx512vnni,-push2pop2,+cx8,-avx512bw,+sse3,-pku,-nf,-amx-tf32,-amx-avx512,+fsgsbase,+clzero,-mwaitx,-lwp,+lzcnt,+sha,-movdir64b,-ppx,+wbnoinvd,-enqcmd,-amx-transpose,-avxneconvert,-tbm,-pconfig,-amx-complex,+ssse3,+cx16,-avx10.2,+bmi2,+fma,+popcnt,-avxifma,+f16c,-avx512bitalg,-rdpru,+clwb,+mmx,+sse2,+rdseed,-avx512vbmi2,-prefetchi,-amx-movrs,+rdpid,-fma4,-avx512vbmi,-shstk,-vaes,-waitpkg,-sgx,+fxsr,-avx512dq,+sse4a" }
```

Reproducer: https://godbolt.org/z/axM4e97rq

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -slp-threshold=-99999 <source>
1.	Running pass "verify" on module "<source>"
 #0 0x0000000005915ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5915ae8)
 #1 0x0000000005912994 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000761e7ee42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000761e7ee969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000761e7ee42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000761e7ee287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000008322ef llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x000000000584be91 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x584be91)
 #8 0x000000000573c998 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x573c998)
 #9 0x00000000009766ce llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9766ce)
#10 0x00000000056f9da1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x56f9da1)
#11 0x000000000098080a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98080a)
#12 0x0000000000974a57 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x974a57)
#13 0x0000761e7ee29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x0000761e7ee29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x000000000096bb65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96bb65)
Program terminated with signal: SIGSEGV
Compiler returned: 139


## Comments

### Comment 1 - dtcxzyw

Bisected to 2d7b55a028139dde58a994a65895841190dfde96

---

