# [RISC-V] Assertion `From.getParent() == To.getParent() && !From.hasImplicitDef()' failed.

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/147986
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-07-11T17:57:47Z

## Body

Testcase:
```llvm ir
; ModuleID = 'reduced.bc'
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define fastcc <vscale x 4 x i8> @c(<vscale x 4 x i16> %0, <vscale x 4 x i1> %active.lane.mask) #0 {
for.cond2.us156.us.us.preheader:
  %1 = call <vscale x 4 x i16> @llvm.umin.nxv4i16(<vscale x 4 x i16> %0, <vscale x 4 x i16> splat (i16 255))
  %2 = trunc <vscale x 4 x i16> %1 to <vscale x 4 x i8>
  %3 = select <vscale x 4 x i1> %active.lane.mask, <vscale x 4 x i8> %2, <vscale x 4 x i8> zeroinitializer
  ret <vscale x 4 x i8> %3
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <vscale x 4 x i16> @llvm.umin.nxv4i16(<vscale x 4 x i16>, <vscale x 4 x i16>) #1

attributes #0 = { "target-features"="+v" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

Commands/backtrace
```
 /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc ./reduced.ll
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/Target/RISCV/RISCVVectorPeephole.cpp:524: bool isSafeToMove(const llvm::MachineInstr&, const llvm::MachineInstr&): Assertion `From.getParent() == To.getParent() && !From.hasImplicitDef()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc ./reduced.ll
1.      Running pass 'Function Pass Manager' on module './reduced.ll'.
2.      Running pass 'RISC-V Vector Peephole Optimization' on function '@c'
 #0 0x0000557dec7f1d02 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1eb4d02)
 #1 0x0000557dec7ee9ff llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1eb19ff)
 #2 0x0000557dec7eeb4c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007d7242045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007d724209eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007d724209eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007d724209eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007d724204527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007d72420288ff abort ./stdlib/abort.c:81:7
 #9 0x00007d724202881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007d724203b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000557deaf043bf (anonymous namespace)::RISCVVectorPeephole::ensureDominates(llvm::MachineOperand const&, llvm::MachineInstr&) const (.isra.0) RISCVVectorPeephole.cpp:0:0
#12 0x0000557deaf061df (anonymous namespace)::RISCVVectorPeephole::runOnMachineFunction(llvm::MachineFunction&) RISCVVectorPeephole.cpp:0:0
#13 0x0000557deb6254b3 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#14 0x0000557debcbdcb1 llvm::FPPassManager::runOnFunction(llvm::Function&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1380cb1)
#15 0x0000557debcbe101 llvm::FPPassManager::runOnModule(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1381101)
#16 0x0000557debcbea54 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1381a54)
#17 0x0000557deae7408b compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#18 0x0000557dead861d7 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x4491d7)
#19 0x00007d724202a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#20 0x00007d724202a28b call_init ./csu/../csu/libc-start.c:128:20
#21 0x00007d724202a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#22 0x0000557deae68ff5 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x52bff5)
Aborted (core dumped)
```

Godbolt: https://godbolt.org/z/o8j3r6qxs

Found via fuzzer (C program before reduction)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Edwin Lu (ewlu)

<details>
Testcase:
```llvm ir
; ModuleID = 'reduced.bc'
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define fastcc &lt;vscale x 4 x i8&gt; @<!-- -->c(&lt;vscale x 4 x i16&gt; %0, &lt;vscale x 4 x i1&gt; %active.lane.mask) #<!-- -->0 {
for.cond2.us156.us.us.preheader:
  %1 = call &lt;vscale x 4 x i16&gt; @<!-- -->llvm.umin.nxv4i16(&lt;vscale x 4 x i16&gt; %0, &lt;vscale x 4 x i16&gt; splat (i16 255))
  %2 = trunc &lt;vscale x 4 x i16&gt; %1 to &lt;vscale x 4 x i8&gt;
  %3 = select &lt;vscale x 4 x i1&gt; %active.lane.mask, &lt;vscale x 4 x i8&gt; %2, &lt;vscale x 4 x i8&gt; zeroinitializer
  ret &lt;vscale x 4 x i8&gt; %3
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare &lt;vscale x 4 x i16&gt; @<!-- -->llvm.umin.nxv4i16(&lt;vscale x 4 x i16&gt;, &lt;vscale x 4 x i16&gt;) #<!-- -->1

attributes #<!-- -->0 = { "target-features"="+v" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

Commands/backtrace
```
 /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc ./reduced.ll
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/Target/RISCV/RISCVVectorPeephole.cpp:524: bool isSafeToMove(const llvm::MachineInstr&amp;, const llvm::MachineInstr&amp;): Assertion `From.getParent() == To.getParent() &amp;&amp; !From.hasImplicitDef()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc ./reduced.ll
1.      Running pass 'Function Pass Manager' on module './reduced.ll'.
2.      Running pass 'RISC-V Vector Peephole Optimization' on function '@<!-- -->c'
 #<!-- -->0 0x0000557dec7f1d02 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1eb4d02)
 #<!-- -->1 0x0000557dec7ee9ff llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1eb19ff)
 #<!-- -->2 0x0000557dec7eeb4c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007d7242045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007d724209eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007d724209eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007d724209eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007d724204527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007d72420288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007d724202881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007d724203b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000557deaf043bf (anonymous namespace)::RISCVVectorPeephole::ensureDominates(llvm::MachineOperand const&amp;, llvm::MachineInstr&amp;) const (.isra.0) RISCVVectorPeephole.cpp:0:0
#<!-- -->12 0x0000557deaf061df (anonymous namespace)::RISCVVectorPeephole::runOnMachineFunction(llvm::MachineFunction&amp;) RISCVVectorPeephole.cpp:0:0
#<!-- -->13 0x0000557deb6254b3 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->14 0x0000557debcbdcb1 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1380cb1)
#<!-- -->15 0x0000557debcbe101 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1381101)
#<!-- -->16 0x0000557debcbea54 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1381a54)
#<!-- -->17 0x0000557deae7408b compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->18 0x0000557dead861d7 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x4491d7)
#<!-- -->19 0x00007d724202a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->20 0x00007d724202a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->21 0x00007d724202a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->22 0x0000557deae68ff5 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x52bff5)
Aborted (core dumped)
```

Godbolt: https://godbolt.org/z/o8j3r6qxs

Found via fuzzer (C program before reduction)
</details>


---

