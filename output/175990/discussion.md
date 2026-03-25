# [LV] crash with PGO

**Author:** Andarwinux
**URL:** https://github.com/llvm/llvm-project/issues/175990
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-01-15T09:10:23Z

## Body

[test.zip](https://github.com/user-attachments/files/24619392/test.zip)

```
clang -O3 -c mov.i -fprofile-use=pgo.profdata 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /build/install/llvmbin/clang -O3 -c mov.i -fprofile-use=pgo.profdata
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "src/libavformat/mov.c"
4.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "mov_read_header"
#0 0x00005ce47e8a4eee PrintStackTraceSignalHandler(void*) (.llvm.14462126317361604948) Signals.cpp:0:0
```

## Comments

### Comment 1 - Andarwinux

[mov.ll.txt](https://github.com/user-attachments/files/24631971/mov.ll.txt)

```
clang -c -O3 -x ir mov.ll.txt

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /build/install/llvmbin/clang -c -O3 -x ir mov.ll.txt
1.      Optimizer
2.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "mov.ll.txt"
3.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "mov_read_header"
#0 0x00005d671aee592e PrintStackTraceSignalHandler(void*) (.llvm.14462126317361604948) Signals.cpp:0:0
```

---

### Comment 2 - Andarwinux

2abd6d6d7ac54d40302dfc946d01572a12963ee5 @huntergr-arm

---

