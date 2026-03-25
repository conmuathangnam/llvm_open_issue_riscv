# Instruction does not dominate all uses! ..LLVM ERROR: Broken module found, compilation aborted!

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/165151
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-10-26T19:54:17Z

## Body

To reproduce run opt with the test below (-passes=slp-vectorizer):
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define void @wombat(i32 %arg) #0 gc "statepoint-example" {
bb:
  br label %bb1

bb1:                                              ; preds = %bb
  %sub = sub i32 0, %arg
  %add = add i32 0, 0
  %add2 = add i32 0, 0
  %add3 = add i32 0, 0
  br i1 false, label %bb8, label %bb4

bb4:                                              ; preds = %bb1
  %add5 = add i32 %add3, 0
  %add6 = add i32 0, 0
  %add7 = add i32 0, 0
  br label %bb8

bb8:                                              ; preds = %bb4, %bb1
  %phi = phi i32 [ %sub, %bb4 ], [ %sub, %bb1 ]
  %phi9 = phi i32 [ %add5, %bb4 ], [ %add, %bb1 ]
  %phi10 = phi i32 [ %add6, %bb4 ], [ %add2, %bb1 ]
  %phi11 = phi i32 [ %add7, %bb4 ], [ %add3, %bb1 ]
  ret void
}

attributes #0 = { "target-features"="+prfchw,-cldemote,+avx,+aes,+sahf,+pclmul,-xop,+crc32,-amx-fp8,+xsaves,-avx512fp16,-usermsr,-sm4,-egpr,+sse4.1,-avx10.1,-avx512ifma,+xsave,+sse4.2,-tsxldtrk,-sm3,-ptwrite,-widekl,-movrs,-invpcid,+64bit,+xsavec,-avx512vpopcntdq,+cmov,-avx512vp2intersect,-avx512cd,+movbe,-avxvnniint8,-ccmp,-amx-int8,-kl,-sha512,-avxvnni,-rtm,+adx,+avx2,-hreset,-movdiri,-serialize,-vpclmulqdq,-avx512vl,-uintr,-cf,+clflushopt,-raoint,-cmpccxadd,+bmi,-amx-tile,+sse,-gfni,-avxvnniint16,-amx-fp16,-zu,-ndd,+xsaveopt,+rdrnd,-avx512f,-amx-bf16,-avx512bf16,-avx512vnni,-push2pop2,+cx8,-avx512bw,+sse3,-pku,-nf,-amx-tf32,-amx-avx512,+fsgsbase,+clzero,-mwaitx,-lwp,+lzcnt,+sha,-movdir64b,-ppx,+wbnoinvd,-enqcmd,-amx-transpose,-avxneconvert,-tbm,-pconfig,-amx-complex,+ssse3,+cx16,-avx10.2,+bmi2,+fma,+popcnt,-avxifma,+f16c,-avx512bitalg,-rdpru,+clwb,+mmx,+sse2,+rdseed,-avx512vbmi2,-prefetchi,-amx-movrs,+rdpid,-fma4,-avx512vbmi,-shstk,-vaes,-waitpkg,-sgx,+fxsr,-avx512dq,+sse4a" }
```
Reproducer: https://godbolt.org/z/1czjdsGz4

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer <source>
1.	Running pass "verify" on module "<source>"
 #0 0x0000000005974be8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5974be8)
 #1 0x0000000005971a94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000079b58c842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000079b58c8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000079b58c842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000079b58c8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000008362db llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000058a24c1 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58a24c1)
 #8 0x000000000578ee38 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x578ee38)
 #9 0x000000000097ba6e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97ba6e)
#10 0x000000000574c471 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x574c471)
#11 0x0000000000985c3a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985c3a)
#12 0x0000000000979eb1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979eb1)
#13 0x000079b58c829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x000079b58c829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x00000000009712c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9712c5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

