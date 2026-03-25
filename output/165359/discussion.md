# [LV][SVE] Crashed when optimization remarks are enabled: "Fell off the end of a type-switch"

**Author:** yus3710-fj
**URL:** https://github.com/llvm/llvm-project/issues/165359
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-11-13T16:12:42Z

## Body

Some Fortran benchmarks (e.g., SPEC2017 and NPB) fail to compile when optimization remarks are enabled.
The following is a reproducer.

```
$ cat repro.f90
subroutine repro(iter, v, sum)
  implicit none

  integer iter, i, m
  double precision  v(5), sum(5)

  do i = 1, iter
    do m = 1, 5
      sum(m) = sum(m) + v(m)
    end do
  end do
end subroutine
$ flang -O2 -march=armv8-a+sve repro.f90 -Rpass=vector
Fell off the end of a type-switch
UNREACHABLE executed at /path/to/llvm-project/llvm/include/llvm/ADT/TypeSwitch.h:126!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /path/to/build/bin/flang -fc1 -triple aarch64-unknown-linux-gnu -emit-obj -fcolor-diagnostics -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu generic -target-feature +outline-atomics -target-feature +v8a -target-feature +fp-armv8 -target-feature +fullfp16 -target-feature +neon -target-feature +sve -vectorize-loops -vectorize-slp -Rpass=vector -resource-dir /path/to/build/lib/clang/22 -mframe-pointer=non-leaf -O2 -o /tmp/repro-737c08.o -x f95 repro.f90
1.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O2>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "FIRModule"
2.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "repro_"
 #0 0x0000ffff71b64710 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/path/to/build/lib/libLLVM.so.22.0git+0xa8f710)
 #1 0x0000ffff71b61c9c llvm::sys::RunSignalHandlers() (/path/to/build/lib/libLLVM.so.22.0git+0xa8cc9c)
 #2 0x0000ffff71b61df4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000ffff86f6d7f0 (linux-vdso.so.1+0x7f0)
 #4 0x0000ffff70cb37b8 __pthread_kill_implementation /usr/src/debug/glibc-2.34-168.el9_6.23.aarch64/nptl/pthread_kill.c:44:76
 #5 0x0000ffff70c6bb3c gsignal /usr/src/debug/glibc-2.34-168.el9_6.23.aarch64/signal/../sysdeps/posix/raise.c:27:6
 #6 0x0000ffff70c58074 abort /usr/src/debug/glibc-2.34-168.el9_6.23.aarch64/stdlib/abort.c:81:7
 #7 0x0000ffff71a7694c (/path/to/build/lib/libLLVM.so.22.0git+0x9a194c)
 #8 0x0000ffff736ff774 llvm::LoopVectorizationPlanner::emitInvalidCostRemarks(llvm::OptimizationRemarkEmitter*) (/path/to/build/lib/libLLVM.so.22.0git+0x262a774)
 #9 0x0000ffff73708cf8 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/path/to/build/lib/libLLVM.so.22.0git+0x2633cf8)
#10 0x0000ffff7370b27c llvm::LoopVectorizePass::runImpl(llvm::Function&) (/path/to/build/lib/libLLVM.so.22.0git+0x263627c)
#11 0x0000ffff7370b80c llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/path/to/build/lib/libLLVM.so.22.0git+0x263680c)
  :
#28 0x000000000081c2b0 _start (/path/to/build/bin/flang+0x81c2b0)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git c8f5c602c897d2345c1cfd8d886c1325598dbdc6)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /path/to/build/bin
Build config: +assertions
```

The input IR: https://godbolt.org/z/z1P6Wxj7d

## Comments

### Comment 1 - bababuck

Don't see anyone working on this, will take a look.

---

