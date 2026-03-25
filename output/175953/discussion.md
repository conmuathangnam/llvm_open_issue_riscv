# [Polly] Assertion `!is_error()` failed in `normalizeValInst` / `ForwardOpTree` with `builtin_assume`

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/175953
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2026-01-27T19:37:44Z

## Body

Clang with polly crashes on valid with an assertion failure when compiling the attached C file 
with `-O3 -mllvm -polly`.

Clang compiled with commit hash `56f5fda5776a9693a79ccea4e23ae1295df81a69`, and
cmake option `cmake '-DCMAKE_BUILD_TYPE=Release' '-DLLVM_ENABLE_ASSERTIONS=ON' '-DLLVM_ENABLE_PROJECTS=clang;polly' ../llvm`

Attached File:

[func-28f15b.sh](https://github.com/user-attachments/files/24614645/func-28f15b.sh)

[func-28f15b.c](https://github.com/user-attachments/files/24614648/func-28f15b.c)

Reduced Code: 

```c
char a, b;
int c, aj, ak, d, f, e, at, au, g;
long h, al;
short k, l, m, am, i;
long an[];
char ao[][25];
int ap[][225];
long aq[][225];
int ar[];
int *as;
_Bool n;
void j(int o[][5][5], int p[][15], _Bool q[][5], int r[][15][15]) {
  for (char s; s < 8; s = g += 2)
    a = au;
  for (_Bool t; t < d; t = at)
    ;
  __builtin_assume((char)f + 121 == 3);
  for (char u = f + 121; u; u += n) {
    __builtin_assume(at);
    __builtin_assume(f - d);
    for (int ab = 0; ab < 4073665287550ULL; ab += 4072487988786)
      __builtin_assume(g);
    for (int ac = 0; ac < 5; ac += 3)
      __builtin_assume(a);
    __builtin_assume((char)i + 120 - 1 == 13);
    for (char ad = 1; ad < (char)i + 120; ad++) {
      for (char ae = 4; ae < 14; ae++) {
        c = 0;
        b = q[u][ad];
      }
      if (o[2][u][2])
        for (; f;)
          ;
      for (short ag = 4; ag < i + 6008; ag += 4)
        for (char ah = 0; ah < 15; ah += 2) {
          h = ({ h > e ? h : e; });
          if (as[ag]) {
            k = 0;
            l = p[ag][ad];
            m = 0;
            for (char ai = 0; ai < 5; ai += 3) {
              an[ah] = r[u][ad + 1][u - 2] < r[ag - 3][ad + 1][u - 2];
              ao[u][ad * 5] = 0;
            }
            aj = ({ aj ?: e; });
            ak += 2;
            ap[u][ad * 15 + ag] = 0;
            al = am = 0;
            aq[ad][(u - 3) * 15 + ag] = 0;
          }
        }
      ar[u + u] = 9;
    }
  }
}
```


diagnostic message:
```
func.c:5:6: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    5 | long an[];
      |      ^
func.c:6:6: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | char ao[][25];
      |      ^
func.c:7:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    7 | int ap[][225];
      |     ^
func.c:8:6: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    8 | long aq[][225];
      |      ^
func.c:9:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    9 | int ar[];
      |     ^
Assertion "!is_error()" failed at /home/lenovo/augpath/compiler-build/llvm-project/polly/lib/External/isl/include/isl/isl-noexceptions.h:78
  IMPLEMENTATION ERROR: Unhandled error state
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name func.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/augpath/yarpgen/bug-reduce/crash/opt_clang-anothertry-0426/opt-further -fcoverage-compilation-dir=/home/lenovo/augpath/yarpgen/bug-reduce/crash/opt_clang-anothertry-0426/opt-further -resource-dir /home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22 -internal-isystem /home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=169 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/func-8968ba.o -x c func.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "func.c"
4.      Running pass "polly" on function "j"
 #0 0x000055b0f92300cf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x45460cf)
 #1 0x000055b0f922cd84 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fb359563420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x00007fb35904c00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #4 0x00007fb35902b859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #5 0x000055b0fa3eca6d normalizeValInst(isl::union_map, llvm::DenseSet<llvm::PHINode*, llvm::DenseMapInfo<llvm::PHINode*, void>> const&, isl::union_map) ZoneAlgo.cpp:0:0
 #6 0x000055b0fa3f252f polly::ZoneAlgorithm::makeNormalizedValInst(llvm::Value*, polly::ScopStmt*, llvm::Loop*, bool) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x570852f)
 #7 0x000055b0fa3dda15 (anonymous namespace)::ForwardOpTreeImpl::forwardTree(polly::ScopStmt*, llvm::Value*, polly::ScopStmt*, llvm::Loop*) ForwardOpTree.cpp:0:0
 #8 0x000055b0fa3df038 polly::runForwardOpTree(polly::Scop&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x56f5038)
 #9 0x000055b0fa3910a8 polly::runPollyPass(llvm::Function&, llvm::AnalysisManager<llvm::Function>&, polly::PollyPassOptions) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x56a70a8)
#10 0x000055b0fa392062 polly::PollyFunctionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x56a8062)
#11 0x000055b0fa30f926 llvm::detail::PassModel<llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x5625926)
#12 0x000055b0f8b4b031 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x3e61031)
#13 0x000055b0f61f11b6 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x15071b6)
#14 0x000055b0f8b4966e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x3e5f66e)
#15 0x000055b0f61f1856 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x1507856)
#16 0x000055b0f8b49001 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x3e5f001)
#17 0x000055b0f94fe138 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#18 0x000055b0f9501e32 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x4817e32)
#19 0x000055b0f9bde6b6 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x4ef46b6)
#20 0x000055b0fba3283c clang::ParseAST(clang::Sema&, bool, bool) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x6d4883c)
#21 0x000055b0f9bdeb38 clang::CodeGenAction::ExecuteAction() (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x4ef4b38)
#22 0x000055b0f9ee38c9 clang::FrontendAction::Execute() (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x51f98c9)
#23 0x000055b0f9e614ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x51774ee)
#24 0x000055b0f9fe592d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x52fb92d)
#25 0x000055b0f5d3458f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x104a58f)
#26 0x000055b0f5d2ae0a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#27 0x000055b0f5d2f33b clang_main(int, char**, llvm::ToolContext const&) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x104533b)
#28 0x000055b0f5be82cb main (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0xefe2cb)
#29 0x00007fb35902d083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#30 0x000055b0f5d2a88e _start (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x104088e)
clang: error: unable to execute command: Aborted
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 56f5fda5776a9693a79ccea4e23ae1295df81a69)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/augpath/compiler-build/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/func-28f15b.c
clang: note: diagnostic msg: /tmp/func-28f15b.sh
clang: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: None (thapgua)

<details>
Clang with polly crashes on valid with an assertion failure when compiling the attached C file 
with `-O3 -mllvm -polly`.

Clang compiled with commit hash `56f5fda5776a9693a79ccea4e23ae1295df81a69`, and
cmake option `cmake '-DCMAKE_BUILD_TYPE=Release' '-DLLVM_ENABLE_ASSERTIONS=ON' '-DLLVM_ENABLE_PROJECTS=clang;polly' ../llvm`

Attached File:

[func-28f15b.sh](https://github.com/user-attachments/files/24614645/func-28f15b.sh)

[func-28f15b.c](https://github.com/user-attachments/files/24614648/func-28f15b.c)

Reduced Code: 

```c
char a, b;
int c, aj, ak, d, f, e, at, au, g;
long h, al;
short k, l, m, am, i;
long an[];
char ao[][25];
int ap[][225];
long aq[][225];
int ar[];
int *as;
_Bool n;
void j(int o[][5][5], int p[][15], _Bool q[][5], int r[][15][15]) {
  for (char s; s &lt; 8; s = g += 2)
    a = au;
  for (_Bool t; t &lt; d; t = at)
    ;
  __builtin_assume((char)f + 121 == 3);
  for (char u = f + 121; u; u += n) {
    __builtin_assume(at);
    __builtin_assume(f - d);
    for (int ab = 0; ab &lt; 4073665287550ULL; ab += 4072487988786)
      __builtin_assume(g);
    for (int ac = 0; ac &lt; 5; ac += 3)
      __builtin_assume(a);
    __builtin_assume((char)i + 120 - 1 == 13);
    for (char ad = 1; ad &lt; (char)i + 120; ad++) {
      for (char ae = 4; ae &lt; 14; ae++) {
        c = 0;
        b = q[u][ad];
      }
      if (o[2][u][2])
        for (; f;)
          ;
      for (short ag = 4; ag &lt; i + 6008; ag += 4)
        for (char ah = 0; ah &lt; 15; ah += 2) {
          h = ({ h &gt; e ? h : e; });
          if (as[ag]) {
            k = 0;
            l = p[ag][ad];
            m = 0;
            for (char ai = 0; ai &lt; 5; ai += 3) {
              an[ah] = r[u][ad + 1][u - 2] &lt; r[ag - 3][ad + 1][u - 2];
              ao[u][ad * 5] = 0;
            }
            aj = ({ aj ?: e; });
            ak += 2;
            ap[u][ad * 15 + ag] = 0;
            al = am = 0;
            aq[ad][(u - 3) * 15 + ag] = 0;
          }
        }
      ar[u + u] = 9;
    }
  }
}
```


diagnostic message:
```
func.c:5:6: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    5 | long an[];
      |      ^
func.c:6:6: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | char ao[][25];
      |      ^
func.c:7:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    7 | int ap[][225];
      |     ^
func.c:8:6: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    8 | long aq[][225];
      |      ^
func.c:9:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    9 | int ar[];
      |     ^
Assertion "!is_error()" failed at /home/lenovo/augpath/compiler-build/llvm-project/polly/lib/External/isl/include/isl/isl-noexceptions.h:78
  IMPLEMENTATION ERROR: Unhandled error state
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name func.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/augpath/yarpgen/bug-reduce/crash/opt_clang-anothertry-0426/opt-further -fcoverage-compilation-dir=/home/lenovo/augpath/yarpgen/bug-reduce/crash/opt_clang-anothertry-0426/opt-further -resource-dir /home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22 -internal-isystem /home/lenovo/augpath/compiler-build/llvm-project/build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=169 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/func-8968ba.o -x c func.c
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "func.c"
4.      Running pass "polly" on function "j"
 #<!-- -->0 0x000055b0f92300cf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x45460cf)
 #<!-- -->1 0x000055b0f922cd84 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007fb359563420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->3 0x00007fb35904c00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #<!-- -->4 0x00007fb35902b859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #<!-- -->5 0x000055b0fa3eca6d normalizeValInst(isl::union_map, llvm::DenseSet&lt;llvm::PHINode*, llvm::DenseMapInfo&lt;llvm::PHINode*, void&gt;&gt; const&amp;, isl::union_map) ZoneAlgo.cpp:0:0
 #<!-- -->6 0x000055b0fa3f252f polly::ZoneAlgorithm::makeNormalizedValInst(llvm::Value*, polly::ScopStmt*, llvm::Loop*, bool) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x570852f)
 #<!-- -->7 0x000055b0fa3dda15 (anonymous namespace)::ForwardOpTreeImpl::forwardTree(polly::ScopStmt*, llvm::Value*, polly::ScopStmt*, llvm::Loop*) ForwardOpTree.cpp:0:0
 #<!-- -->8 0x000055b0fa3df038 polly::runForwardOpTree(polly::Scop&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x56f5038)
 #<!-- -->9 0x000055b0fa3910a8 polly::runPollyPass(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, polly::PollyPassOptions) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x56a70a8)
#<!-- -->10 0x000055b0fa392062 polly::PollyFunctionPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x56a8062)
#<!-- -->11 0x000055b0fa30f926 llvm::detail::PassModel&lt;llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x5625926)
#<!-- -->12 0x000055b0f8b4b031 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x3e61031)
#<!-- -->13 0x000055b0f61f11b6 llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x15071b6)
#<!-- -->14 0x000055b0f8b4966e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x3e5f66e)
#<!-- -->15 0x000055b0f61f1856 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x1507856)
#<!-- -->16 0x000055b0f8b49001 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x3e5f001)
#<!-- -->17 0x000055b0f94fe138 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->18 0x000055b0f9501e32 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x4817e32)
#<!-- -->19 0x000055b0f9bde6b6 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x4ef46b6)
#<!-- -->20 0x000055b0fba3283c clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x6d4883c)
#<!-- -->21 0x000055b0f9bdeb38 clang::CodeGenAction::ExecuteAction() (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x4ef4b38)
#<!-- -->22 0x000055b0f9ee38c9 clang::FrontendAction::Execute() (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x51f98c9)
#<!-- -->23 0x000055b0f9e614ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x51774ee)
#<!-- -->24 0x000055b0f9fe592d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x52fb92d)
#<!-- -->25 0x000055b0f5d3458f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x104a58f)
#<!-- -->26 0x000055b0f5d2ae0a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->27 0x000055b0f5d2f33b clang_main(int, char**, llvm::ToolContext const&amp;) (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x104533b)
#<!-- -->28 0x000055b0f5be82cb main (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0xefe2cb)
#<!-- -->29 0x00007fb35902d083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->30 0x000055b0f5d2a88e _start (/home/lenovo/augpath/compiler-build/llvm-project/build/bin/clang-22+0x104088e)
clang: error: unable to execute command: Aborted
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 56f5fda5776a9693a79ccea4e23ae1295df81a69)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/augpath/compiler-build/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/func-28f15b.c
clang: note: diagnostic msg: /tmp/func-28f15b.sh
clang: note: diagnostic msg: 

********************
```
</details>


---

### Comment 2 - Meinersbur

Thanks for the report. This particular crash disappeared with #177776, but only because the max-operation limit is hit somewhere else. The missing error state check is added in #178281.

---

