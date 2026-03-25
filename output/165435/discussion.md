# Instruction does not dominate all uses! ...LLVM ERROR: Broken module found, compilation aborted!

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/165435
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-10-29T18:50:55Z

## Body

To reproduce run opt with the test below (-passes=slp-vectorizer -slp-threshold=-99999):
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define double @wombat() #0 gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb4, %bb
  %phi = phi i32 [ 0, %bb ], [ 0, %bb4 ]
  %phi2 = phi i32 [ 0, %bb ], [ 0, %bb4 ]
  %phi3 = phi i32 [ 0, %bb ], [ %or5, %bb4 ]
  br label %bb4

bb4:                                              ; preds = %bb1
  %or = or i32 %phi2, 0
  %mul = mul i32 0, 1
  %or5 = or i32 %phi3, %mul
  %and = and i32 %or, 0
  %or6 = or i32 %phi2, 1
  br i1 false, label %bb7, label %bb1

bb7:                                              ; preds = %bb4
  %phi8 = phi i32 [ %phi, %bb4 ]
  %phi9 = phi i32 [ %or, %bb4 ]
  %phi10 = phi i32 [ %or5, %bb4 ]
  %phi11 = phi i32 [ %or6, %bb4 ]
  ret double 0.000000e+00
}

attributes #0 = { "target-features"="+prfchw,-cldemote,+avx,+aes,+sahf,+pclmul,-xop,+crc32,-amx-fp8,+xsaves,-avx512fp16,-usermsr,-sm4,-egpr,+sse4.1,-avx10.1,-avx512ifma,+xsave,+sse4.2,-tsxldtrk,-sm3,-ptwrite,-widekl,-movrs,-invpcid,+64bit,+xsavec,-avx512vpopcntdq,+cmov,-avx512vp2intersect,-avx512cd,+movbe,-avxvnniint8,-ccmp,-amx-int8,-kl,-sha512,-avxvnni,-rtm,+adx,+avx2,-hreset,-movdiri,-serialize,-vpclmulqdq,-avx512vl,-uintr,-cf,+clflushopt,-raoint,-cmpccxadd,+bmi,-amx-tile,+sse,-gfni,-avxvnniint16,-amx-fp16,-zu,-ndd,+xsaveopt,+rdrnd,-avx512f,-amx-bf16,-avx512bf16,-avx512vnni,-push2pop2,+cx8,-avx512bw,+sse3,-pku,-nf,-amx-tf32,-amx-avx512,+fsgsbase,+clzero,-mwaitx,-lwp,+lzcnt,+sha,-movdir64b,-ppx,+wbnoinvd,-enqcmd,-amx-transpose,-avxneconvert,-tbm,-pconfig,-amx-complex,+ssse3,+cx16,-avx10.2,+bmi2,+fma,+popcnt,-avxifma,+f16c,-avx512bitalg,-rdpru,+clwb,+mmx,+sse2,+rdseed,-avx512vbmi2,-prefetchi,-amx-movrs,+rdpid,-fma4,-avx512vbmi,-shstk,-vaes,-waitpkg,-sgx,+fxsr,-avx512dq,+sse4a" }
```
Reproducer: https://godbolt.org/z/8cecz9WfT

Stack dump:
```
Instruction does not dominate all uses!
  %4 = insertelement <4 x i32> <i32 poison, i32 0, i32 0, i32 1>, i32 %mul, i32 0
  %2 = shufflevector <4 x i32> %4, <4 x i32> <i32 poison, i32 0, i32 0, i32 0>, <4 x i32> <i32 0, i32 5, i32 6, i32 7>
LLVM ERROR: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -slp-threshold=-99999 <source>
1.	Running pass "verify" on module "<source>"
 #0 0x0000000005977d28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5977d28)
 #1 0x0000000005974bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000078133d042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000078133d0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000078133d042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000078133d0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000000000836711 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000058a4841 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58a4841)
 #8 0x0000000005790d58 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5790d58)
 #9 0x000000000097bebe llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97bebe)
#10 0x000000000574e391 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574e391)
#11 0x000000000098608a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98608a)
#12 0x000000000097a301 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97a301)
#13 0x000078133d029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x000078133d029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x0000000000971715 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x971715)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

