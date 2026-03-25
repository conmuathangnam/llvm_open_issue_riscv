# [CodeGen][RISC-V] Assertion '(FrameSDOps.empty() || MF.getFrameInfo().adjustsStack()) && "AdjustsStack not set in presence of a frame pseudo instruction."' failed.

**Author:** patrick-rivos
**URL:** https://github.com/llvm/llvm-project/issues/89285
**Status:** Open
**Labels:** llvm:codegen, crash

## Body

Testcase:
```c
char a, o;
short b, k, p;
int c, d, g, h, i, j, l, m;
extern int e[];
long n;
_Bool f[5];
void q() {
  for (unsigned r = 0; r < 24; r++) {
    for (short s = 4; s; s += 4)
      ;
    for (int t = 0; t < l; t += 9093) {
      a = i ? o ?: h : o;
      b = ({ b ?: j; });
      for (signed char u = 0; u < 5; u += 2)
        ;
    }
    for (int v = 0; v < l; v += p % l) {
      if (h)
        d = i ? k : m ? o : 0;
      e[r * 25 + v * 25 + r] = 0;
    }
    if (g)
      c = l ? i ?: n : k;
    f[r] = 0;
  }
}
```
Godbolt: https://godbolt.org/z/qETrEoaqh

Reduced LLVM IR:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define void @q(i32 %0, i64 %1, ptr %f, i1 %tobool10.not) {
entry:
  %cond74 = select i1 %tobool10.not, i64 %1, i64 1
  tail call void @llvm.memset.p0.i64(ptr %f, i8 0, i64 24, i1 false)
  %cond55.conv48 = select i1 %tobool10.not, i32 %0, i32 1
  %conv79 = trunc i64 %cond74 to i32
  store i32 %cond55.conv48, ptr null, align 4
  store i32 %conv79, ptr %f, align 4
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #0

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: write) }
```
Godbolt: https://godbolt.org/z/WGK8d6MbG

Backtrace:
```
> /scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc reduced.ll
llc: /scratch/tc-testing/tc-apr-17/llvm/llvm/lib/CodeGen/PrologEpilogInserter.cpp:375: void {anonymous}::PEI::calculateCallFrameInfo(llvm::MachineFunction&): Assertion `(FrameSDOps.empty() || MF.getFrameInfo().adjustsStack()) && "AdjustsStack not set in presence of a frame pseudo instruction."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@q'
 #0 0x00005e6f82f4eee0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc+0x1d04ee0)
 #1 0x00005e6f82f4c2ef llvm::sys::RunSignalHandlers() (/scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc+0x1d022ef)
 #2 0x00005e6f82f4c445 SignalHandler(int) Signals.cpp:0:0
 #3 0x00007c689ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007c689ea969fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007c689ea969fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007c689ea969fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007c689ea42476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007c689ea287f3 abort ./stdlib/abort.c:81:7
 #9 0x00007c689ea2871b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007c689ea39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x00005e6f820500f1 (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#12 0x00005e6f81eb80c7 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#13 0x00005e6f824aded6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc+0x1263ed6)
#14 0x00005e6f824ae129 llvm::FPPassManager::runOnModule(llvm::Module&) (/scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc+0x1264129)
#15 0x00005e6f824aea95 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc+0x1264a95)
#16 0x00005e6f817d1a8a compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#17 0x00005e6f8171c7c6 main (/scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc+0x4d27c6)
#18 0x00007c689ea29d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#19 0x00007c689ea29e40 call_init ./csu/../csu/libc-start.c:128:20
#20 0x00007c689ea29e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#21 0x00005e6f817c8315 _start (/scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc+0x57e315)
zsh: IOT instruction (core dumped)  /scratch/tc-testing/tc-apr-17/build-rv64gcv/build-llvm-linux/bin/llc
```

Found via fuzzer.

## Comments

### Comment 1 - svs-quic

Issue bisected to #78001
cc: [JonPsson1](https://github.com/JonPsson1)

For some reason we don't see any of the frame pseudo instructions during FinalizeIsel for RV64.

Setting setAdjustsStack while lowering calls seems to help but I am not sure if that is a good solution.
```
--- a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
+++ b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
@@ -18994,8 +18994,11 @@ SDValue RISCVTargetLowering::LowerCall(CallLoweringInfo &CLI,
     ByValArgs.push_back(FIPtr);
   }
 
-  if (!IsTailCall)
+  if (!IsTailCall) {
+    if (Subtarget.is64Bit())
+      MF.getFrameInfo().setAdjustsStack(true);
     Chain = DAG.getCALLSEQ_START(Chain, NumBytes, 0, CLI.DL);
+  }
```



---

### Comment 2 - agostonrobert

I guess this will be fixed by https://github.com/llvm/llvm-project/pull/96046

---

