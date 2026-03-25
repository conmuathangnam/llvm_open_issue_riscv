# Instruction does not dominate all uses!... LLVM ERROR: Broken module found, compilation aborted! (yet another)

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/167366
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-11-11T20:06:04Z

## Body

To reproduce run opt with the test below (-passes=slp-vectorizer -slp-threshold=-99999):
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @wombat() #0 gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb14, %bb10, %bb
  %phi = phi i32 [ 0, %bb ], [ %or16, %bb14 ], [ 0, %bb10 ]
  %phi2 = phi i32 [ 0, %bb ], [ %or15, %bb14 ], [ 0, %bb10 ]
  br label %bb3

bb3:                                              ; preds = %bb1
  %phi4 = phi i32 [ poison, %bb1 ]
  %phi5 = phi float [ 0.000000e+00, %bb1 ]
  %phi6 = phi i32 [ poison, %bb1 ]
  %phi7 = phi i32 [ %phi, %bb1 ]
  %phi8 = phi float [ 0.000000e+00, %bb1 ]
  %phi9 = phi i32 [ %phi2, %bb1 ]
  br label %bb10

bb10:                                             ; preds = %bb14, %bb3
  %phi11 = phi i32 [ 0, %bb3 ], [ %phi11, %bb14 ]
  %phi12 = phi float [ 0.000000e+00, %bb3 ], [ 0.000000e+00, %bb14 ]
  %phi13 = phi i32 [ 0, %bb3 ], [ %or15, %bb14 ]
  switch i32 0, label %bb14 [
    i32 0, label %bb1
  ]

bb14:                                             ; preds = %bb10
  %or = or i32 %phi13, %phi11
  %or15 = or i32 %or, 0
  %or16 = or i32 %phi11, 0
  br i1 false, label %bb1, label %bb10
}

attributes #0 = { "target-features"="+prfchw,-cldemote,+avx,+aes,+sahf,+pclmul,-xop,+crc32,-amx-fp8,+xsaves,-avx512fp16,-usermsr,-sm4,-egpr,+sse4.1,-avx10.1,-avx512ifma,+xsave,+sse4.2,-tsxldtrk,-sm3,-ptwrite,-widekl,-movrs,-invpcid,+64bit,+xsavec,-avx512vpopcntdq,+cmov,-avx512vp2intersect,-avx512cd,+movbe,-avxvnniint8,-ccmp,-amx-int8,-kl,-sha512,-avxvnni,-rtm,+adx,+avx2,-hreset,-movdiri,-serialize,-vpclmulqdq,-avx512vl,-uintr,-cf,+clflushopt,-raoint,-cmpccxadd,+bmi,-amx-tile,+sse,-gfni,-avxvnniint16,-amx-fp16,-zu,-ndd,+xsaveopt,+rdrnd,-avx512f,-amx-bf16,-avx512bf16,-avx512vnni,-push2pop2,+cx8,-avx512bw,+sse3,-pku,-nf,-amx-tf32,-amx-avx512,+fsgsbase,+clzero,-mwaitx,-lwp,+lzcnt,+sha,-movdir64b,-ppx,+wbnoinvd,-enqcmd,-avxneconvert,-tbm,-pconfig,-amx-complex,+ssse3,+cx16,-avx10.2,+bmi2,+fma,+popcnt,-avxifma,+f16c,-avx512bitalg,-rdpru,+clwb,+mmx,+sse2,+rdseed,-avx512vbmi2,-prefetchi,-amx-movrs,+rdpid,-fma4,-avx512vbmi,-shstk,-vaes,-waitpkg,-sgx,+fxsr,-avx512dq,+sse4a" }
```
Reproducer: https://godbolt.org/z/6bzdEoWb3

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -slp-threshold=-99999 <source>
1.	Running pass "verify" on module "<source>"
 #0 0x00000000059af7a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59af7a8)
 #1 0x00000000059ac654 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000078a228842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000078a2288969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000078a228842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000078a2288287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000000000839a0d llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000058dc2d1 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58dc2d1)
 #8 0x00000000057c8b88 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57c8b88)
 #9 0x000000000097f88e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97f88e)
#10 0x00000000057860e1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57860e1)
#11 0x0000000000989a5a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x989a5a)
#12 0x000000000097dcd1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97dcd1)
#13 0x000078a228829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x000078a228829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x00000000009750e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9750e5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

