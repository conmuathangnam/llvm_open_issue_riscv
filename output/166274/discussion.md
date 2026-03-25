# [Polly] crash on function "svt_av1_lowbd_inv_txfm2d_add_ssse3"

**Author:** Andarwinux
**URL:** https://github.com/llvm/llvm-project/issues/166274
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2025-11-14T18:46:17Z

## Body

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.   Program arguments: /build/install/llvmbin/clang -march=native -I/build/src_packages/svtav1-psy/. -I/build/src_packages/svtav1-psy/Source/API -I/build/src_packages/svtav1-psy/Source/Lib/Codec -I/build/src_packages/svtav1-psy/Source/Lib/C_DEFAULT -I/build/src_packages/svtav1-psy/third_party/fastfeat -I/build/src_packages/svtav1-psy/Source/Lib/ASM_SSE2 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_SSSE3 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_SSE4_1 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_AVX2 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_AVX512 -O3 -o Source/Lib/ASM_SSSE3/CMakeFiles/ASM_SSSE3.dir/av1_inv_txfm_ssse3.c.obj -c /build/src_packages/svtav1-psy/Source/Lib/ASM_SSSE3/av1_inv_txfm_ssse3.c --target=x86_64-pc-windows-gnu -mllvm -polly
1.   <eof> parser at end of file
2.   Optimizer
3.   Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "/build/src_packages/svtav1-psy/Source/Lib/ASM_SSSE3/av1_inv_txfm_ssse3.c"
4.   Running pass "polly" on function "svt_av1_lowbd_inv_txfm2d_add_ssse3"
#0 0x00005e5c058e5e8e PrintStackTraceSignalHandler(void*) (.llvm.12027265274519014927) Signals.cpp:0:0
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 733530fc9a87e5f435475d6a8c461cfcc26bc1c6)
Target: x86_64-pc-windows-gnu
Thread model: posix
InstalledDir: /build/install/llvmbin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/av1_inv_txfm_ssse3-286b09.c
clang: note: diagnostic msg: /tmp/av1_inv_txfm_ssse3-286b09.sh
clang: note: diagnostic msg:

********************
```

[av1_inv_txfm_ssse3-286b09.sh](https://github.com/user-attachments/files/23320427/av1_inv_txfm_ssse3-286b09.sh)
[av1_inv_txfm_ssse3-286b09.c](https://github.com/user-attachments/files/23320428/av1_inv_txfm_ssse3-286b09.c)

## Comments

### Comment 1 - Andarwinux

@Meinersbur Could you take a look at this? It seems to be caused by the latest Polly commit.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-polly

Author: None (Andarwinux)

<details>
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.   Program arguments: /build/install/llvmbin/clang -march=native -I/build/src_packages/svtav1-psy/. -I/build/src_packages/svtav1-psy/Source/API -I/build/src_packages/svtav1-psy/Source/Lib/Codec -I/build/src_packages/svtav1-psy/Source/Lib/C_DEFAULT -I/build/src_packages/svtav1-psy/third_party/fastfeat -I/build/src_packages/svtav1-psy/Source/Lib/ASM_SSE2 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_SSSE3 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_SSE4_1 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_AVX2 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_AVX512 -O3 -o Source/Lib/ASM_SSSE3/CMakeFiles/ASM_SSSE3.dir/av1_inv_txfm_ssse3.c.obj -c /build/src_packages/svtav1-psy/Source/Lib/ASM_SSSE3/av1_inv_txfm_ssse3.c --target=x86_64-pc-windows-gnu -mllvm -polly
1.   &lt;eof&gt; parser at end of file
2.   Optimizer
3.   Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "/build/src_packages/svtav1-psy/Source/Lib/ASM_SSSE3/av1_inv_txfm_ssse3.c"
4.   Running pass "polly" on function "svt_av1_lowbd_inv_txfm2d_add_ssse3"
#<!-- -->0 0x00005e5c058e5e8e PrintStackTraceSignalHandler(void*) (.llvm.12027265274519014927) Signals.cpp:0:0
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 733530fc9a87e5f435475d6a8c461cfcc26bc1c6)
Target: x86_64-pc-windows-gnu
Thread model: posix
InstalledDir: /build/install/llvmbin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/av1_inv_txfm_ssse3-286b09.c
clang: note: diagnostic msg: /tmp/av1_inv_txfm_ssse3-286b09.sh
clang: note: diagnostic msg:

********************
```

[av1_inv_txfm_ssse3-286b09.sh](https://github.com/user-attachments/files/23320427/av1_inv_txfm_ssse3-286b09.sh)
[av1_inv_txfm_ssse3-286b09.c](https://github.com/user-attachments/files/23320428/av1_inv_txfm_ssse3-286b09.c)
</details>


---

### Comment 3 - Meinersbur

The patch was reverted in a22d1c2225543aa9ae7882f6b1a97ee7b2c95574. I will look into your reproducer before retrying

---

### Comment 4 - Andarwinux

> The patch was reverted. I will look into your reproducer before retrying

Yes, so I closed the issue later, but it wasn't until LLVM finished rebuilding that I discovered Polly would still crash in other ways elsewhere:

```
Assertion "!is_error()" failed at /build/src_packages/llvm/polly/lib/External/isl/include/isl/isl-noexceptions.h:78
  IMPLEMENTATION ERROR: Unhandled error state
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /build/install/llvmbin/clang -march=native -I/build/src_packages/svtav1-psy/. -I/build/src_packages/svtav1-psy/Source/API -I/build/src_packages/svtav1-psy/Source/Lib/Codec -I/build/src_packages/svtav1-psy/Source/Lib/C_DEFAULT -I/build/src_packages/svtav1-psy/third_party/fastfeat -I/build/src_packages/svtav1-psy/Source/Lib/Globals -I/build/src_packages/svtav1-psy/Source/Lib/ASM_SSE2 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_SSSE3 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_SSE4_1 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_AVX2 -I/build/src_packages/svtav1-psy/Source/Lib/ASM_AVX512 -I/build/src_packages/svtav1-psy/third_party/cpuinfo/include -O3 -DNDEBUG -c -o Source/Lib/Codec/CMakeFiles/CODEC.dir/global_me.c.obj /build/src_packages/svtav1-psy/Source/Lib/Codec/global_me.c --target=x86_64-pc-windows-gnu --sysroot=/build/install/x86_64-w64-mingw32 -O3 -mllvm -polly
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly::CodePreparationPass,polly::FunctionToScopPassAdaptor<llvm::PassManager<Scop, ScopAnalysisManager, ScopStandardAnalysisResults &, SPMUpdater &>>,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,dfa-jump-threading,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "/build/src_packages/svtav1-psy/Source/Lib/Codec/global_me.c"
4.      Running pass "polly::FunctionToScopPassAdaptor<llvm::PassManager<Scop, ScopAnalysisManager, ScopStandardAnalysisResults &, SPMUpdater &>>" on function "svt_aom_global_motion_estimation"
#0 0x00005d8a8e339eee PrintStackTraceSignalHandler(void*) (.llvm.12027265274519014927) Signals.cpp:0:0
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git cdd230ef5fd58d38993370fc5b045a2f5a95af9d)
Target: x86_64-pc-windows-gnu
Thread model: posix
InstalledDir: /build/install/llvmbin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/global_me-e0d660.c
clang: note: diagnostic msg: /tmp/global_me-e0d660.sh
clang: note: diagnostic msg:

********************
```

[global_me-e0d660.c](https://github.com/user-attachments/files/23328906/global_me-e0d660.c)
[global_me-e0d660.sh](https://github.com/user-attachments/files/23328907/global_me-e0d660.sh)

---

### Comment 5 - Meinersbur

Cases that are distinct from the initial report should be reported in a new issue. This issue # will be closed as soon as the original issue is fixed.

---

### Comment 6 - Meinersbur

The reproducer consists of two SCoPs. After the first one is successfully optimzed, it caused an aliasing detection problem for this second. This makes the second SCoP "disappear" and resulted in a null pointer dereference.

The semantics of an optimized SCoP should be the same as the original code, I think that SCEV is unable to determine the loop exit value of `%i11` of the polly-optimized code in the new regression tests, which makes the an array subscriupt expression to be SCEVUnknown (SCEV would need to support ternary conditional expressions to support the runtime check that Polly inserts).

Fixed in the re-apply in https://github.com/llvm/llvm-project/pull/167560 (specifically [this commit](https://github.com/llvm/llvm-project/pull/167560/commits/4fbf72bf7c82fdbae86977f7f2e262e50563b0ea)).

---

### Comment 7 - Andarwinux

Thanks! I can confirm no crashes in the new PR.

---

### Comment 8 - Meinersbur

Closed by #167560

---

