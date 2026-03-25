# Instruction does not dominate all uses!...LLVM ERROR: Broken module found, compilation aborted! (once again)

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/168512
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-11-20T01:15:35Z

## Body

To reproduce run opt with the test below (-passes=slp-vectorizer -slp-threshold=-99999):

```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define i32 @wombat() #0 gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb24, %bb
  %phi = phi i32 [ %lshr25, %bb24 ], [ 0, %bb ]
  %phi2 = phi i32 [ %or26, %bb24 ], [ 0, %bb ]
  %phi3 = phi i32 [ 0, %bb24 ], [ 0, %bb ]
  br i1 false, label %bb4, label %bb11

bb4:                                              ; preds = %bb1
  %phi5 = phi double [ 0.000000e+00, %bb1 ]
  %phi6 = phi i32 [ poison, %bb1 ]
  %phi7 = phi i32 [ poison, %bb1 ]
  %phi8 = phi double [ 0.000000e+00, %bb1 ]
  %phi9 = phi i32 [ %phi2, %bb1 ]
  %phi10 = phi i32 [ %phi, %bb1 ]
  br label %bb19

bb11:                                             ; preds = %bb1
  br i1 false, label %bb12, label %bb16

bb12:                                             ; preds = %bb11
  %or = or i32 0, %phi3
  br label %bb13

bb13:                                             ; preds = %bb12
  %phi14 = phi i32 [ %phi, %bb12 ]
  %phi15 = phi i32 [ %or, %bb12 ]
  br label %bb16

bb16:                                             ; preds = %bb13, %bb11
  %phi17 = phi i32 [ 0, %bb11 ], [ %phi14, %bb13 ]
  %phi18 = phi i32 [ 0, %bb11 ], [ %phi15, %bb13 ]
  br label %bb19

bb19:                                             ; preds = %bb16, %bb4
  %phi20 = phi i32 [ 0, %bb4 ], [ %phi17, %bb16 ]
  %phi21 = phi i32 [ 0, %bb4 ], [ %phi18, %bb16 ]
  %phi22 = phi double [ 0.000000e+00, %bb4 ], [ 0.000000e+00, %bb16 ]
  %or23 = or i32 %phi21, 0
  br label %bb24

bb24:                                             ; preds = %bb19
  %lshr = lshr i32 %phi20, 0
  %and = and i32 %lshr, 0
  %lshr25 = lshr i32 %phi, %and
  %or26 = or i32 0, %or23
  br label %bb1
}

attributes #0 = { "target-features"="+prfchw,-cldemote,+avx,+aes,+sahf,+pclmul,-xop,+crc32,-amx-fp8,+xsaves,-avx512fp16,-usermsr,-sm4,-egpr,+sse4.1,-avx10.1,-avx512ifma,+xsave,+sse4.2,-tsxldtrk,-sm3,-ptwrite,-widekl,-movrs,-invpcid,+64bit,+xsavec,-avx512vpopcntdq,+cmov,-avx512vp2intersect,-avx512cd,+movbe,-avxvnniint8,-ccmp,-amx-int8,-kl,-sha512,-avxvnni,-rtm,+adx,+avx2,-hreset,-movdiri,-serialize,-vpclmulqdq,-avx512vl,-uintr,-cf,+clflushopt,-raoint,-cmpccxadd,+bmi,-amx-tile,+sse,-gfni,-avxvnniint16,-amx-fp16,-zu,-ndd,+xsaveopt,+rdrnd,-avx512f,-amx-bf16,-avx512bf16,-avx512vnni,-push2pop2,+cx8,-avx512bw,+sse3,-pku,-nf,-amx-tf32,-amx-avx512,+fsgsbase,+clzero,-mwaitx,-lwp,+lzcnt,+sha,-movdir64b,-ppx,+wbnoinvd,-enqcmd,-avxneconvert,-tbm,-pconfig,-amx-complex,+ssse3,+cx16,-avx10.2,+bmi2,+fma,+popcnt,-avxifma,+f16c,-avx512bitalg,-rdpru,+clwb,+mmx,+sse2,+rdseed,-avx512vbmi2,-prefetchi,-amx-movrs,+rdpid,-fma4,-avx512vbmi,-shstk,-vaes,-waitpkg,-sgx,+fxsr,-avx512dq,+sse4a" }
```
Reproducer: https://godbolt.org/z/3GjPaK9b8

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -slp-threshold=-99999 <source>
1.	Running pass "verify" on module "<source>"
 #0 0x0000000005977768 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5977768)
 #1 0x0000000005974614 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000073d16ec42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000073d16ec969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000073d16ec42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000073d16ec287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000000000082da77 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000058abe41 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58abe41)
 #8 0x0000000005797b68 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5797b68)
 #9 0x000000000097321e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97321e)
#10 0x0000000005756121 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5756121)
#11 0x000000000097d4a8 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::RTLIB::RuntimeLibcallsInfo&, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97d4a8)
#12 0x00000000009714fd optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9714fd)
#13 0x000073d16ec29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x000073d16ec29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x0000000000968745 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x968745)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

