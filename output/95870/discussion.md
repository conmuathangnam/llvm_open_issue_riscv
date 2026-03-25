# [RISC-V] Assertion: 'AdjustsStack not set in presence of a frame pseudo instruction.' at -O1 with -fwrapv

**Author:** patrick-rivos
**URL:** https://github.com/llvm/llvm-project/issues/95870
**Status:** Open
**Labels:** llvm:codegen, crash

## Body

Testcase:
```c
int a, f;
char b, g, h;
extern _Bool c[], e[];
extern int d[];
short i, j;
long k, l;
_Bool e[3];
void m() {
  for (short n = 0; n < 13; n++) {
    for (_Bool o = 0; o < (_Bool)l; o = 1)
      for (int p = j % h - 6; p; p += 4)
        a = c[2 + n * n];
    if ((_Bool)(f ? i > g : g)) {
      d[n] = 0;
      b = f ?: k;
    }
    e[n] = 0;
  }
  for (int q = 3; q; q++)
    ;
  for (int r = 3; r; r++)
    ;
  for (;;)
    ;
}
```
Godbolt: https://godbolt.org/z/1T7EWTGEY

Reduced LLVM IR:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define void @m(i32 %arg.0, i64 %arg.1, ptr %arg.2) {
entry:
  %icmp.0 = icmp eq i32 %arg.0, 0
  %sele.0 = select i1 %icmp.0, i64 %arg.1, i64 1
  %trun.0 = trunc i64 %sele.0 to i8
  call void @llvm.memset.p0.i64(ptr %arg.2, i8 0, i64 13, i1 false)
  %sele.1 = select i1 %icmp.0, i32 %arg.0, i32 1
  %icmp.1 = icmp eq i32 %sele.1, 0
  br i1 %icmp.1, label %if.end, label %if.then

if.then:                                          ; preds = %arg.2ntry
  store i8 %trun.0, ptr null, align 1
  br label %if.end

if.end:                                           ; preds = %if.then, %arg.2ntry
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #0

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: write) }
```
Godbolt: https://godbolt.org/z/1Wah8h1W3

Backtrace:
```bash
> /scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc reduced.ll
llc: /scratch/tc-testing/tc-jun-17/llvm/llvm/lib/CodeGen/PrologEpilogInserter.cpp:375: void {anonymous}::PEI::calculateCallFrameInfo(llvm::MachineFunction&): Assertion `(FrameSDOps.empty() || MF.getFrameInfo().adjustsStack()) && "AdjustsStack not set in presence of a frame pseudo instruction."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@m'
 #0 0x00005f1bdba777e0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc+0x1d5e7e0)
 #1 0x00005f1bdba74bef llvm::sys::RunSignalHandlers() (/scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc+0x1d5bbef)
 #2 0x00005f1bdba74d45 SignalHandler(int) Signals.cpp:0:0
 #3 0x00007a4118042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a41180969fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007a41180969fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007a41180969fc pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007a4118042476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007a41180287f3 abort ./stdlib/abort.c:81:7
 #9 0x00007a411802871b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007a4118039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#11 0x00005f1bdab55dfb (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#12 0x00005f1bda9ae917 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#13 0x00005f1bdafc1476 llvm::FPPassManager::runOnFunction(llvm::Function&) (/scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc+0x12a8476)
#14 0x00005f1bdafc16c9 llvm::FPPassManager::runOnModule(llvm::Module&) (/scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc+0x12a86c9)
#15 0x00005f1bdafc2035 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc+0x12a9035)
#16 0x00005f1bda2b67de compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#17 0x00005f1bda1ff95e main (/scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc+0x4e695e)
#18 0x00007a4118029d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#19 0x00007a4118029e40 call_init ./csu/../csu/libc-start.c:128:20
#20 0x00007a4118029e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#21 0x00005f1bda2ad085 _start (/scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc+0x594085)
zsh: IOT instruction (core dumped)  /scratch/tc-testing/tc-jun-17/build-rv64gcv/build-llvm-linux/bin/llc
```

Found via fuzzer.

## Comments

### Comment 1 - jacquesguan

I create a pr  https://github.com/llvm/llvm-project/pull/96046 to fix it.

---

### Comment 2 - agostonrobert

Is it a duplicate of https://github.com/llvm/llvm-project/issues/89285 ?

---

