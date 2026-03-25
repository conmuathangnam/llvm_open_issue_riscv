# [LV][SVE] Crashed when optimization remarks are enabled: "Fell off the end of a type-switch"

**Author:** kdupontbe
**URL:** https://github.com/llvm/llvm-project/issues/182047
**Status:** Open
**Labels:** vectorizers, crash

## Body

Test case fails to compile when optimization remarks are enabled.
The following is a reproducer

`$ cat tsvc_bigger-b607b9.c`

```c
# 1 "<built-in>"
# 1 "tsvc_bigger.c"
#define LEN_1D 100
int c[LEN_1D];
int d[LEN_1D];

int a = 0;
float b = 0.0;
void fn1() {
  for (int i = 0; i < LEN_1D; i++)
    if (d[i])
      b += c[i];
  b += a;
}
```

```
tsvc_bigger.c:8:3: remark: Scalable vectorization is explicitly disabled [-Rpass-analysis=loop-vectorize]
    8 |   for (int i = 0; i < LEN_1D; i++)
      |   ^
clang: /path/to/llvm-project/llvm/include/llvm/ADT/TypeSwitch.h:121: ResultT llvm::TypeSwitch<const llvm::VPRecipeBase *, unsigned int>::operator unsigned int() [T = const llvm::VPRecipeBase *, ResultT = unsigned int]: Assertion `result && "Fell off the end of a type-switch"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /path/to/llvm-project/install/bin/clang -S -emit-llvm -O3 -ffast-math --target=riscv32-unknown-elf -march=rv32imafd_zve32x_zvl64b -menable-experimental-extensions -mllvm -riscv-v-vector-bits-min=64 -mllvm -scalable-vectorization=off -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize tsvc_bigger.c
1.	<eof> parser at end of file
2.	Optimizer
3.	Running pass "function<eager-inv>(float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop(evl-iv-simplify),loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "tsvc_bigger.c"
4.	Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "fn1"
 #0 0x000055555eb4df6a llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000055555eb4e52b PrintStackTraceSignalHandler(void*) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:918:1
 #2 0x000055555eb4bfcd llvm::sys::RunSignalHandlers() /path/to/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000055555eb4d789 llvm::sys::CleanupOnSignal(unsigned long) /path/to/llvm-project/llvm/lib/Support/Unix/Signals.inc:374:1
 #4 0x000055555ea88bc7 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /path/to/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:20
 #5 0x000055555ea88fab CrashRecoverySignalHandler(int) /path/to/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
 #6 0x00007ffff7bce630 __restore_rt sigaction.c:0:0
 #7 0x00007ffff6cdf387 raise (/lib64/libc.so.6+0x36387)
 #8 0x00007ffff6ce0a78 abort (/lib64/libc.so.6+0x37a78)
 #9 0x00007ffff6cd81a6 __assert_fail_base (/lib64/libc.so.6+0x2f1a6)
#10 0x00007ffff6cd8252 (/lib64/libc.so.6+0x2f252)
#11 0x0000555560757131 llvm::TypeSwitch<llvm::VPRecipeBase const*, unsigned int>::operator unsigned int() /path/to/llvm-project/llvm/include/llvm/ADT/TypeSwitch.h:122:23
#12 0x0000555560755364 llvm::LoopVectorizationPlanner::emitInvalidCostRemarks(llvm::OptimizationRemarkEmitter*) /path/to/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:4067:14
#13 0x0000555560777681 llvm::LoopVectorizePass::processLoop(llvm::Loop*) /path/to/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10097:33
...
#48 0x000055555c446429 _start (/path/to/llvm-project/install/bin/clang+0x6ef2429)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.1.8 (https://github.com/llvm/llvm-project.git 2078da43e25a4623cab2d0d60decddf709aaea28)
Target: riscv32-unknown-unknown-elf
Thread model: posix
InstalledDir: /path/to/llvm-project/install/bin
Build config: +unoptimized, +assertions
```
The input IR: https://godbolt.org/z/6W6WGq94c

## Comments

### Comment 1 - tudinhh

Hi, I've checked this issue and had a pull request for it.


---

### Comment 2 - kdupontbe

Thanks for looking into the issue

---

