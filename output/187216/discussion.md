# [Polly] Segmentation fault in isl_tab_extend_cons during calculateDependences (Similar to #177527)

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/187216
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2026-03-23T11:04:23Z

## Body

Clang crashes with a segmentation fault when compiling a C++ file with -O3 and Polly enabled (-mllvm -polly) on a polly pass. The backtrace is very similar to that of issue #177527, so it appears the bug may have come back or this is a closely related regression.

Environment and compiler version: 

```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git f1b16eaeddbbcc8239ad454309779d42adcfe4f1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/lenovo/compiler-build/llvm-project/build/bin
Build config: +assertions
```

The ISL version is `isl-0.27-86-gcf471c16`. 

Attached files: 

[crash-57487b.sh](https://github.com/user-attachments/files/26081404/crash-57487b.sh)

[crash-57487b.cpp](https://github.com/user-attachments/files/26081406/crash-57487b.cpp)

Backtrace: 

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name crash.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/lenovo/yarpgen/bug-submit/26-3-18 -fcoverage-compilation-dir=/home/lenovo/lenovo/yarpgen/bug-submit/26-3-18 -resource-dir /home/lenovo/lenovo/compiler-build/llvm-project/build/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /home/lenovo/lenovo/compiler-build/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=158 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/crash-c32a25.o -x c++ crash.cpp
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "crash.cpp"
4.      Running pass "polly" on function "_Z1ucsicPA7_A7_iS1_PA4_A4_sPA0_A0_sS7_PA0_A10_iPA0_i"
 #0 0x000056340eadc67f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x46fe67f)
 #1 0x000056340ead9264 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000056340ead936c SignalHandlerTerminate(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f5de272c420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x0000563410c5df53 isl_tab_extend_cons (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x687ff53)
 #5 0x0000563410c731fa context_gbr_add_eq isl_tab_pip.c:0:0
 #6 0x0000563410c7929a basic_map_partial_lexopt_base_sol isl_tab_pip.c:0:0
 #7 0x0000563410c79dd5 basic_map_partial_lexopt_pw_multi_aff isl_tab_pip.c:0:0
 #8 0x0000563410c7a9eb basic_map_partial_lexopt_intersected_pw_multi_aff isl_tab_pip.c:0:0
 #9 0x0000563410baad3d basic_map_partial_lexopt_pw_multi_aff isl_map.c:0:0
#10 0x0000563410bab07c isl_map_partial_lexopt_aligned_pw_multi_aff isl_map.c:0:0
#11 0x0000563410bac2de isl_map_partial_lexopt_aligned isl_map.c:0:0
#12 0x0000563410b60428 last_source isl_flow.c:0:0
#13 0x0000563410b61023 access_info_compute_flow_core isl_flow.c:0:0
#14 0x0000563410b6417d isl_union_access_info_compute_flow (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x678617d)
#15 0x000056340fd77991 buildFlow(isl_union_map*, isl_union_map*, isl_union_map*, isl_union_map*, isl_schedule*) DependenceInfo.cpp:0:0
#16 0x000056340fd7ab72 polly::Dependences::calculateDependences(polly::Scop&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x599cb72)
#17 0x000056340fd7c88e polly::DependenceAnalysis::Result::recomputeDependences(polly::Dependences::AnalysisLevel) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x599e88e)
#18 0x000056340fe15cc9 (anonymous namespace)::runIslScheduleOptimizerImpl(polly::Scop&, llvm::function_ref<polly::Dependences const& (polly::Dependences::AnalysisLevel)>, llvm::TargetTransformInfo*, llvm::OptimizationRemarkEmitter*, isl::schedule&, bool&) (.constprop.0) ScheduleOptimizer.cpp:0:0
#19 0x000056340fe181c6 polly::runIslScheduleOptimizer(polly::Scop&, llvm::TargetTransformInfo*, polly::DependenceAnalysis::Result&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5a3a1c6)
#20 0x000056340fde6060 polly::runPollyPass(llvm::Function&, llvm::AnalysisManager<llvm::Function>&, polly::PollyPassOptions) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5a08060)
#21 0x000056340fde7122 polly::PollyFunctionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5a09122)
#22 0x000056340fd64a86 llvm::detail::PassModel<llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5986a86)
#23 0x000056340e3da9d1 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x3ffc9d1)
#24 0x000056340b939936 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x155b936)
#25 0x000056340e3d900e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x3ffb00e)
#26 0x000056340b93a026 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x155c026)
#27 0x000056340e3d89a1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x3ffa9a1)
#28 0x000056340ef18a58 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#29 0x000056340ef1c4dc clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x4b3e4dc)
#30 0x000056340f611d6d clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5233d6d)
#31 0x00005634113bfb0c clang::ParseAST(clang::Sema&, bool, bool) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x6fe1b0c)
#32 0x000056340f6121e8 clang::CodeGenAction::ExecuteAction() (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x52341e8)
#33 0x000056340f92b459 clang::FrontendAction::Execute() (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x554d459)
#34 0x000056340f8a6a2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x54c8a2e)
#35 0x000056340fa2fdfd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5651dfd)
#36 0x000056340b45c065 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x107e065)
#37 0x000056340b45282a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#38 0x000056340b456d5b clang_main(int, char**, llvm::ToolContext const&) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x1078d5b)
#39 0x000056340b30e7b1 main (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0xf307b1)
#40 0x00007f5de21f6083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#41 0x000056340b4522ae _start (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x10742ae)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git f1b16eaeddbbcc8239ad454309779d42adcfe4f1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/lenovo/compiler-build/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/crash-57487b.cpp
clang: note: diagnostic msg: /tmp/crash-57487b.sh
clang: note: diagnostic msg: 

********************
```



Code: 

```cpp
char e;
extern char q;
long r;
int ac;
extern short ad[];
extern bool ae[][100];
extern char af[];
extern int ag[][100];
template < typename _Tp > _Tp max( _Tp ,  _Tp );
short a, ah;
int *b;
short *c;
char *d;
void u(char f, short g, int i, char ai, int j[][7][7], int k[][7][7],
       short l[][4][4], short m[][0][0], short n[][0][0], int o[][0][10],
       int p[][0]) {
  char h;
  e = ai;
  for (short s; s < 1; s += 3)
    [[assume(ai - 1 == 0)]];
  for (int t = 0; t < 4; t = 7) {
    [[assume(f)]];
    [[assume(!i ? 0 : 7)]];
  }
  if (f)
    if ((f ? g ? i : 6 : 0) ? 8 ? ai ^ ah : 0 : 4073709551615 ? (long)i : 0);
    else
      for (char v = 0; v < 10; v++) {
        for (int w = 1; w; w = h)
          [[assume(g)]];
            for (char x = 0; x < 10; x++) {
          for (int y = 0; y < 10; y += 3) {
            ad[v] = l[3][3][1] ? l[3][1][3] : m[v][3][1];
            ae[v][y] = p[x][x] ?: b[x];
          }
          for (int z = 0; z < 10; z++)
            for (char aa = 2; aa < 8; aa += 3) {
              af[x] |= (7 ? a : 0) ? 0 : n[6][6][z];
              q += 8 ? o[v][x][z] ? j[x][z][5] : 0 : 0;
              ag[v][x * 10 + z] = ah | c[3] ? 0 : d[4];
              r = 0;
            }
        }
      }
  for (char ab(g);; ab = i) {
    max((unsigned)k[ab][ab][ab], 9U);
    ac = ((2 ? f : 0));
  }
}

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: None (thapgua)

<details>
Clang crashes with a segmentation fault when compiling a C++ file with -O3 and Polly enabled (-mllvm -polly) on a polly pass. The backtrace is very similar to that of issue #<!-- -->177527, so it appears the bug may have come back or this is a closely related regression.

Environment and compiler version: 

```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git f1b16eaeddbbcc8239ad454309779d42adcfe4f1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/lenovo/compiler-build/llvm-project/build/bin
Build config: +assertions
```

The ISL version is `isl-0.27-86-gcf471c16`. 

Attached files: 

[crash-57487b.sh](https://github.com/user-attachments/files/26081404/crash-57487b.sh)

[crash-57487b.cpp](https://github.com/user-attachments/files/26081406/crash-57487b.cpp)

Backtrace: 

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name crash.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/lenovo/yarpgen/bug-submit/26-3-18 -fcoverage-compilation-dir=/home/lenovo/lenovo/yarpgen/bug-submit/26-3-18 -resource-dir /home/lenovo/lenovo/compiler-build/llvm-project/build/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /home/lenovo/lenovo/compiler-build/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=158 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/crash-c32a25.o -x c++ crash.cpp
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "crash.cpp"
4.      Running pass "polly" on function "_Z1ucsicPA7_A7_iS1_PA4_A4_sPA0_A0_sS7_PA0_A10_iPA0_i"
 #<!-- -->0 0x000056340eadc67f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x46fe67f)
 #<!-- -->1 0x000056340ead9264 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000056340ead936c SignalHandlerTerminate(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f5de272c420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x0000563410c5df53 isl_tab_extend_cons (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x687ff53)
 #<!-- -->5 0x0000563410c731fa context_gbr_add_eq isl_tab_pip.c:0:0
 #<!-- -->6 0x0000563410c7929a basic_map_partial_lexopt_base_sol isl_tab_pip.c:0:0
 #<!-- -->7 0x0000563410c79dd5 basic_map_partial_lexopt_pw_multi_aff isl_tab_pip.c:0:0
 #<!-- -->8 0x0000563410c7a9eb basic_map_partial_lexopt_intersected_pw_multi_aff isl_tab_pip.c:0:0
 #<!-- -->9 0x0000563410baad3d basic_map_partial_lexopt_pw_multi_aff isl_map.c:0:0
#<!-- -->10 0x0000563410bab07c isl_map_partial_lexopt_aligned_pw_multi_aff isl_map.c:0:0
#<!-- -->11 0x0000563410bac2de isl_map_partial_lexopt_aligned isl_map.c:0:0
#<!-- -->12 0x0000563410b60428 last_source isl_flow.c:0:0
#<!-- -->13 0x0000563410b61023 access_info_compute_flow_core isl_flow.c:0:0
#<!-- -->14 0x0000563410b6417d isl_union_access_info_compute_flow (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x678617d)
#<!-- -->15 0x000056340fd77991 buildFlow(isl_union_map*, isl_union_map*, isl_union_map*, isl_union_map*, isl_schedule*) DependenceInfo.cpp:0:0
#<!-- -->16 0x000056340fd7ab72 polly::Dependences::calculateDependences(polly::Scop&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x599cb72)
#<!-- -->17 0x000056340fd7c88e polly::DependenceAnalysis::Result::recomputeDependences(polly::Dependences::AnalysisLevel) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x599e88e)
#<!-- -->18 0x000056340fe15cc9 (anonymous namespace)::runIslScheduleOptimizerImpl(polly::Scop&amp;, llvm::function_ref&lt;polly::Dependences const&amp; (polly::Dependences::AnalysisLevel)&gt;, llvm::TargetTransformInfo*, llvm::OptimizationRemarkEmitter*, isl::schedule&amp;, bool&amp;) (.constprop.0) ScheduleOptimizer.cpp:0:0
#<!-- -->19 0x000056340fe181c6 polly::runIslScheduleOptimizer(polly::Scop&amp;, llvm::TargetTransformInfo*, polly::DependenceAnalysis::Result&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5a3a1c6)
#<!-- -->20 0x000056340fde6060 polly::runPollyPass(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, polly::PollyPassOptions) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5a08060)
#<!-- -->21 0x000056340fde7122 polly::PollyFunctionPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5a09122)
#<!-- -->22 0x000056340fd64a86 llvm::detail::PassModel&lt;llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5986a86)
#<!-- -->23 0x000056340e3da9d1 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x3ffc9d1)
#<!-- -->24 0x000056340b939936 llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x155b936)
#<!-- -->25 0x000056340e3d900e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x3ffb00e)
#<!-- -->26 0x000056340b93a026 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x155c026)
#<!-- -->27 0x000056340e3d89a1 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x3ffa9a1)
#<!-- -->28 0x000056340ef18a58 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->29 0x000056340ef1c4dc clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x4b3e4dc)
#<!-- -->30 0x000056340f611d6d clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5233d6d)
#<!-- -->31 0x00005634113bfb0c clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x6fe1b0c)
#<!-- -->32 0x000056340f6121e8 clang::CodeGenAction::ExecuteAction() (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x52341e8)
#<!-- -->33 0x000056340f92b459 clang::FrontendAction::Execute() (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x554d459)
#<!-- -->34 0x000056340f8a6a2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x54c8a2e)
#<!-- -->35 0x000056340fa2fdfd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x5651dfd)
#<!-- -->36 0x000056340b45c065 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x107e065)
#<!-- -->37 0x000056340b45282a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->38 0x000056340b456d5b clang_main(int, char**, llvm::ToolContext const&amp;) (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x1078d5b)
#<!-- -->39 0x000056340b30e7b1 main (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0xf307b1)
#<!-- -->40 0x00007f5de21f6083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->41 0x000056340b4522ae _start (/home/lenovo/lenovo/compiler-build/llvm-project/build/bin/clang-22+0x10742ae)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git f1b16eaeddbbcc8239ad454309779d42adcfe4f1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/lenovo/compiler-build/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/crash-57487b.cpp
clang: note: diagnostic msg: /tmp/crash-57487b.sh
clang: note: diagnostic msg: 

********************
```



Code: 

```cpp
char e;
extern char q;
long r;
int ac;
extern short ad[];
extern bool ae[][100];
extern char af[];
extern int ag[][100];
template &lt; typename _Tp &gt; _Tp max( _Tp ,  _Tp );
short a, ah;
int *b;
short *c;
char *d;
void u(char f, short g, int i, char ai, int j[][7][7], int k[][7][7],
       short l[][4][4], short m[][0][0], short n[][0][0], int o[][0][10],
       int p[][0]) {
  char h;
  e = ai;
  for (short s; s &lt; 1; s += 3)
    [[assume(ai - 1 == 0)]];
  for (int t = 0; t &lt; 4; t = 7) {
    [[assume(f)]];
    [[assume(!i ? 0 : 7)]];
  }
  if (f)
    if ((f ? g ? i : 6 : 0) ? 8 ? ai ^ ah : 0 : 4073709551615 ? (long)i : 0);
    else
      for (char v = 0; v &lt; 10; v++) {
        for (int w = 1; w; w = h)
          [[assume(g)]];
            for (char x = 0; x &lt; 10; x++) {
          for (int y = 0; y &lt; 10; y += 3) {
            ad[v] = l[3][3][1] ? l[3][1][3] : m[v][3][1];
            ae[v][y] = p[x][x] ?: b[x];
          }
          for (int z = 0; z &lt; 10; z++)
            for (char aa = 2; aa &lt; 8; aa += 3) {
              af[x] |= (7 ? a : 0) ? 0 : n[6][6][z];
              q += 8 ? o[v][x][z] ? j[x][z][5] : 0 : 0;
              ag[v][x * 10 + z] = ah | c[3] ? 0 : d[4];
              r = 0;
            }
        }
      }
  for (char ab(g);; ab = i) {
    max((unsigned)k[ab][ab][ab], 9U);
    ac = ((2 ? f : 0));
  }
}

```
</details>


---

### Comment 2 - Meinersbur

I submitted an upstream bug report: https://groups.google.com/g/isl-development/c/qX9VaoCj9Js/m/kwMdl4uZAQAJ

As I mentioned in #177527, the updated version ISL indeed seemed to have only changed the operations counter, not fixed the bug. Thanks for the reproducer with the current version of ISL.

---

