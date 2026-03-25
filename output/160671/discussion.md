# Instruction does not dominate all uses! LLVM ERROR: Broken module found, compilation aborted!

**Author:** TatyanaDoubts
**URL:** https://github.com/llvm/llvm-project/issues/160671
**Status:** Closed
**Labels:** llvm:SLPVectorizer, crash
**Closed Date:** 2025-09-25T22:09:35Z

## Body

To reproduce run opt with the test below, using `-passes=slp-vectorizer -slp-threshold=-99999 `
```
; ModuleID = './reduced.ll'
source_filename = "./reduced.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128-ni:1-p2:32:8:8:32-ni:2"
target triple = "x86_64-unknown-linux-gnu"

define i64 @wombat(i32 %arg) #0 gc "statepoint-example" {
bb:
  %freeze = freeze i32 0
  br i1 false, label %bb1, label %bb1

bb1:                                              ; preds = %bb, %bb
  %load = load i32, ptr addrspace(1) null, align 4
  %icmp = icmp eq i32 0, 0
  %icmp2 = icmp ult i32 0, 0
  %add = add i32 %freeze, 0
  %icmp3 = icmp ult i32 %add, 0
  %icmp4 = icmp ugt i32 0, 0
  %icmp5 = icmp ult i32 %freeze, 0
  %icmp6 = icmp ult i32 0, 0
  %icmp7 = icmp ult i32 %add, %arg
  %and = and i1 %icmp7, false
  %icmp8 = icmp ugt i32 0, 0
  %icmp9 = icmp ugt i32 %arg, 0
  %icmp10 = icmp ult i32 %freeze, %load
  br i1 false, label %bb11, label %bb12

bb11:                                             ; preds = %bb1
  %zext = zext i32 %add to i64
  ret i64 0

bb12:                                             ; preds = %bb1
  %zext13 = zext i32 %add to i64
  ret i64 0
}

attributes #0 = { "target-features"="+prfchw,-cldemote,+avx,+aes,+sahf,+pclmul,-xop,+crc32,-amx-fp8,+xsaves,-avx512fp16,-usermsr,-sm4,-egpr,+sse4.1,-avx10.1,-avx512ifma,+xsave,+sse4.2,-tsxldtrk,-sm3,-ptwrite,-widekl,-movrs,-invpcid,+64bit,+xsavec,-avx512vpopcntdq,+cmov,-avx512vp2intersect,-avx512cd,+movbe,-avxvnniint8,-ccmp,-amx-int8,-kl,-sha512,-avxvnni,-rtm,+adx,+avx2,-hreset,-movdiri,-serialize,-vpclmulqdq,-avx512vl,-uintr,-cf,+clflushopt,-raoint,-cmpccxadd,+bmi,-amx-tile,+sse,-gfni,-avxvnniint16,-amx-fp16,-zu,-ndd,+xsaveopt,+rdrnd,-avx512f,-amx-bf16,-avx512bf16,-avx512vnni,-push2pop2,+cx8,-avx512bw,+sse3,-pku,-nf,-amx-tf32,-amx-avx512,+fsgsbase,+clzero,-mwaitx,-lwp,+lzcnt,+sha,-movdir64b,-ppx,+wbnoinvd,-enqcmd,-amx-transpose,-avxneconvert,-tbm,-pconfig,-amx-complex,+ssse3,+cx16,-avx10.2,+bmi2,+fma,+popcnt,-avxifma,+f16c,-avx512bitalg,-rdpru,+clwb,+mmx,+sse2,+rdseed,-avx512vbmi2,-prefetchi,-amx-movrs,+rdpid,-fma4,-avx512vbmi,-shstk,-vaes,-waitpkg,-sgx,+fxsr,-avx512dq,+sse4a" }
```
Reproducer: https://godbolt.org/z/nY8fvjv8f

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=slp-vectorizer -slp-threshold=-99999 <source>
1.	Running pass "verify" on module "<source>"
 #0 0x00000000058da2e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58da2e8)
 #1 0x00000000058d7194 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007cfd67242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007cfd672969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007cfd67242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007cfd672287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000000000082fa8d llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x00000000058112c1 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x58112c1)
 #8 0x0000000005701d78 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5701d78)
 #9 0x000000000097292e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97292e)
#10 0x00000000056bf8e1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x56bf8e1)
#11 0x000000000097cb18 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97cb18)
#12 0x0000000000970cb7 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x970cb7)
#13 0x00007cfd67229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x00007cfd67229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x0000000000967dc5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x967dc5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

