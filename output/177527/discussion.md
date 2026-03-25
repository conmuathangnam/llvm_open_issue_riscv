# [Polly] Segfault in isl_tab_extend_cons during dependence analysis with -O3 -mllvm -polly

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/177527
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2026-01-24T16:07:22Z

## Body

Clang crashes with a segmentation fault when compiling the attached reduced test case with `-O3 -mllvm -polly`. 
The crash occurs inside the `isl` library (specifically `isl_tab_extend_cons`) during Polly's dependence analysis.

The clang build version is `23.0.0git (72b9ae5a9e35dbe2418f057f757d82bfce570150)`, with target `x86_64-unknown-linux-gnu` and build config `+assertions`. 

Attached reproducer: 

[re-aad68e.sh](https://github.com/user-attachments/files/24813435/re-aad68e.sh)

[re-aad68e.c](https://github.com/user-attachments/files/24813437/re-aad68e.c)

Code: 

```c
short a, k;
short b[][64], af[][64];
int c[];
short ad[][4];
int ae[][4];
char e, f, g, h, ah, l;
int i;
char *aa;
int *m;
void j(unsigned n, short o[][8][8], int p[][8][8], long q[][8][8],
       int r[][8][8], char s[][8][8]) {
  if ((f ?: n) >= ah)
    __builtin_unreachable();
  for (short t = 0;; t += e)
    for (char u = n + (!0 ? t : 0) + 1; u; u += 1) {
      if (g + 18 == 18)
        __builtin_unreachable();
      for (char v = h ? g : 0; v; v += 4) {
        if (!(m[t] ? 0 : m[v]) == 0)
          __builtin_unreachable();
        if (!(n - 52204 == 4))
          __builtin_unreachable();
        for (long w = ({
               __typeof__(0) d = ({ k < 0 ? k : 0; });
               __typeof__(n) ag = m[t] ? o[t][8][v] : n ? l : 0;
               d ?: ag;
             });
             w < 18; w += n - 52204) {
          if (6 ? i : 0)
            __builtin_unreachable();
          if (!(l + 57 == 18))
            __builtin_unreachable();
          for (int ab = 0; ab < l + 57; ab += n - 52204) {
            b[t][ab] = 0;
            c[v] = r[ab][u][v] ? 5 ? 903988415 ? q[0][u][v] : 0 : 0
                               : (n ? f : 0) ? p[w][v][3] : 0;
            ad[t][u + v] = 0;
            a = (1048275713 ? e : 0) ? 6 ? aa[t] : 0 : 2019150 ? m[w] : 0;
            ae[t][u * 9] = (6 ? 20 ? s[5][9][5] : 0 : 0) ?: 4 % ah;
            af[t][w * 8] = 0;
          }
        }
        if (5 ? k : 8)
          __builtin_unreachable();
      }
      for (char ac; ac < 3; ac = 1 ? i : 0)
        ;
    }
}

```

Backtrace: 

```
/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang -O3 -I. -mllvm -polly re.c
re.c:19:13: warning: logical not is only applied to the left hand side of this comparison
      [-Wlogical-not-parentheses]
   19 |         if (!(m[t] ? 0 : m[v]) == 0)
      |             ^                  ~~
re.c:19:13: note: add parentheses after the '!' to evaluate the comparison first
   19 |         if (!(m[t] ? 0 : m[v]) == 0)
      |             ^                      
      |              (                     )
re.c:19:13: note: add parentheses around left hand side expression to silence this warning
   19 |         if (!(m[t] ? 0 : m[v]) == 0)
      |             ^
      |             (                 )
re.c:25:42: warning: array index 8 is past the end of the array (that has type 'short[8][8]') [-Warray-bounds]
   25 |                __typeof__(n) ag = m[t] ? o[t][8][v] : n ? l : 0;
      |                                          ^    ~
re.c:10:20: note: array 'o' declared here
   10 | void j(unsigned n, short o[][8][8], int p[][8][8], long q[][8][8],
      |                    ^
re.c:2:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | short b[][64], af[][64];
      |       ^
re.c:2:16: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | short b[][64], af[][64];
      |                ^
re.c:3:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    3 | int c[];
      |     ^
re.c:4:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    4 | short ad[][4];
      |       ^
re.c:5:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    5 | int ae[][4];
      |     ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name re.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/thapgua/yarpgen/bug-reduce/crash/case_000023_e61a0e3d-segfault/opt_clang/reduced -fcoverage-compilation-dir=/home/lenovo/thapgua/yarpgen/bug-reduce/crash/case_000023_e61a0e3d-segfault/opt_clang/reduced -resource-dir /home/lenovo/thapgua/compiler-build/llvm-project/build/lib/clang/23 -I . -internal-isystem /home/lenovo/thapgua/compiler-build/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=112 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/re-c1e1af.o -x c re.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "re.c"
4.      Running pass "polly" on function "j"
 #0 0x000055d09c31664f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x458164f)
 #1 0x000055d09c313314 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f3668439420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x000055d09e4076f3 isl_tab_extend_cons (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x66726f3)
 #4 0x000055d09e41cd9a context_gbr_add_eq isl_tab_pip.c:0:0
 #5 0x000055d09e423131 basic_map_partial_lexopt_base_sol isl_tab_pip.c:0:0
 #6 0x000055d09e423b74 basic_map_partial_lexopt_pw_multi_aff isl_tab_pip.c:0:0
 #7 0x000055d09e35a29d basic_map_partial_lexopt_pw_multi_aff isl_map.c:0:0
 #8 0x000055d09e35a5dc isl_map_partial_lexopt_aligned_pw_multi_aff isl_map.c:0:0
 #9 0x000055d09e35b71e isl_map_partial_lexopt_aligned isl_map.c:0:0
#10 0x000055d09e310188 last_source isl_flow.c:0:0
#11 0x000055d09e310d83 access_info_compute_flow_core isl_flow.c:0:0
#12 0x000055d09e313edd isl_union_access_info_compute_flow (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x657eedd)
#13 0x000055d09d416901 buildFlow(isl_union_map*, isl_union_map*, isl_union_map*, isl_union_map*, isl_schedule*) DependenceInfo.cpp:0:0
#14 0x000055d09d419a6b polly::Dependences::calculateDependences(polly::Scop&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x5684a6b)
#15 0x000055d09d41b7fe polly::DependenceAnalysis::Result::recomputeDependences(polly::Dependences::AnalysisLevel) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56867fe)
#16 0x000055d09d4b4489 (anonymous namespace)::runIslScheduleOptimizerImpl(polly::Scop&, llvm::function_ref<polly::Dependences const& (polly::Dependences::AnalysisLevel)>, llvm::TargetTransformInfo*, llvm::OptimizationRemarkEmitter*, isl::schedule&, bool&) (.constprop.0) ScheduleOptimizer.cpp:0:0
#17 0x000055d09d4b6986 polly::runIslScheduleOptimizer(polly::Scop&, llvm::TargetTransformInfo*, polly::DependenceAnalysis::Result&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x5721986)
#18 0x000055d09d484cb8 polly::runPollyPass(llvm::Function&, llvm::AnalysisManager<llvm::Function>&, polly::PollyPassOptions) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56efcb8)
#19 0x000055d09d485d02 polly::PollyFunctionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56f0d02)
#20 0x000055d09d403706 llvm::detail::PassModel<llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x566e706)
#21 0x000055d09bc26591 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e91591)
#22 0x000055d0992b08e6 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x151b8e6)
#23 0x000055d09bc24bce llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e8fbce)
#24 0x000055d0992b0f86 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x151bf86)
#25 0x000055d09bc24561 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e8f561)
#26 0x000055d09c5e4f6c (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#27 0x000055d09c5e8914 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4853914)
#28 0x000055d09ccc95b6 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4f345b6)
#29 0x000055d09eb466ec clang::ParseAST(clang::Sema&, bool, bool) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x6db16ec)
#30 0x000055d09ccc9a38 clang::CodeGenAction::ExecuteAction() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4f34a38)
#31 0x000055d09cfd2ab9 clang::FrontendAction::Execute() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x523dab9)
#32 0x000055d09cf4ff1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x51baf1e)
#33 0x000055d09d0d532d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x534032d)
#34 0x000055d098dee15f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x105915f)
#35 0x000055d098de495a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#36 0x000055d098de8e8b clang_main(int, char**, llvm::ToolContext const&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x1053e8b)
#37 0x000055d098ca53cb main (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0xf103cb)
#38 0x00007f3667f03083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#39 0x000055d098de43de _start (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x104f3de)
clang: error: unable to execute command: Segmentation fault
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 72b9ae5a9e35dbe2418f057f757d82bfce570150)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/re-aad68e.c
clang: note: diagnostic msg: /tmp/re-aad68e.sh
clang: note: diagnostic msg: 

********************
```

Version: 

```
clang -v
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 72b9ae5a9e35dbe2418f057f757d82bfce570150)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/8
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Candidate multilib: .;@m64
Selected multilib: .;@m64
Found CUDA installation: /usr/local/cuda-12.1, version 12.1
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: None (thapgua)

<details>
Clang crashes with a segmentation fault when compiling the attached reduced test case with `-O3 -mllvm -polly`. 
The crash occurs inside the `isl` library (specifically `isl_tab_extend_cons`) during Polly's dependence analysis.

The clang build version is `23.0.0git (72b9ae5a9e35dbe2418f057f757d82bfce570150)`, with target `x86_64-unknown-linux-gnu` and build config `+assertions`. 

Attached reproducer: 

[re-aad68e.sh](https://github.com/user-attachments/files/24813435/re-aad68e.sh)

[re-aad68e.c](https://github.com/user-attachments/files/24813437/re-aad68e.c)

Code: 

```c
short a, k;
short b[][64], af[][64];
int c[];
short ad[][4];
int ae[][4];
char e, f, g, h, ah, l;
int i;
char *aa;
int *m;
void j(unsigned n, short o[][8][8], int p[][8][8], long q[][8][8],
       int r[][8][8], char s[][8][8]) {
  if ((f ?: n) &gt;= ah)
    __builtin_unreachable();
  for (short t = 0;; t += e)
    for (char u = n + (!0 ? t : 0) + 1; u; u += 1) {
      if (g + 18 == 18)
        __builtin_unreachable();
      for (char v = h ? g : 0; v; v += 4) {
        if (!(m[t] ? 0 : m[v]) == 0)
          __builtin_unreachable();
        if (!(n - 52204 == 4))
          __builtin_unreachable();
        for (long w = ({
               __typeof__(0) d = ({ k &lt; 0 ? k : 0; });
               __typeof__(n) ag = m[t] ? o[t][8][v] : n ? l : 0;
               d ?: ag;
             });
             w &lt; 18; w += n - 52204) {
          if (6 ? i : 0)
            __builtin_unreachable();
          if (!(l + 57 == 18))
            __builtin_unreachable();
          for (int ab = 0; ab &lt; l + 57; ab += n - 52204) {
            b[t][ab] = 0;
            c[v] = r[ab][u][v] ? 5 ? 903988415 ? q[0][u][v] : 0 : 0
                               : (n ? f : 0) ? p[w][v][3] : 0;
            ad[t][u + v] = 0;
            a = (1048275713 ? e : 0) ? 6 ? aa[t] : 0 : 2019150 ? m[w] : 0;
            ae[t][u * 9] = (6 ? 20 ? s[5][9][5] : 0 : 0) ?: 4 % ah;
            af[t][w * 8] = 0;
          }
        }
        if (5 ? k : 8)
          __builtin_unreachable();
      }
      for (char ac; ac &lt; 3; ac = 1 ? i : 0)
        ;
    }
}

```

Backtrace: 

```
/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang -O3 -I. -mllvm -polly re.c
re.c:19:13: warning: logical not is only applied to the left hand side of this comparison
      [-Wlogical-not-parentheses]
   19 |         if (!(m[t] ? 0 : m[v]) == 0)
      |             ^                  ~~
re.c:19:13: note: add parentheses after the '!' to evaluate the comparison first
   19 |         if (!(m[t] ? 0 : m[v]) == 0)
      |             ^                      
      |              (                     )
re.c:19:13: note: add parentheses around left hand side expression to silence this warning
   19 |         if (!(m[t] ? 0 : m[v]) == 0)
      |             ^
      |             (                 )
re.c:25:42: warning: array index 8 is past the end of the array (that has type 'short[8][8]') [-Warray-bounds]
   25 |                __typeof__(n) ag = m[t] ? o[t][8][v] : n ? l : 0;
      |                                          ^    ~
re.c:10:20: note: array 'o' declared here
   10 | void j(unsigned n, short o[][8][8], int p[][8][8], long q[][8][8],
      |                    ^
re.c:2:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | short b[][64], af[][64];
      |       ^
re.c:2:16: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | short b[][64], af[][64];
      |                ^
re.c:3:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    3 | int c[];
      |     ^
re.c:4:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    4 | short ad[][4];
      |       ^
re.c:5:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    5 | int ae[][4];
      |     ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name re.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/thapgua/yarpgen/bug-reduce/crash/case_000023_e61a0e3d-segfault/opt_clang/reduced -fcoverage-compilation-dir=/home/lenovo/thapgua/yarpgen/bug-reduce/crash/case_000023_e61a0e3d-segfault/opt_clang/reduced -resource-dir /home/lenovo/thapgua/compiler-build/llvm-project/build/lib/clang/23 -I . -internal-isystem /home/lenovo/thapgua/compiler-build/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=112 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/re-c1e1af.o -x c re.c
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "re.c"
4.      Running pass "polly" on function "j"
 #<!-- -->0 0x000055d09c31664f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x458164f)
 #<!-- -->1 0x000055d09c313314 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f3668439420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->3 0x000055d09e4076f3 isl_tab_extend_cons (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x66726f3)
 #<!-- -->4 0x000055d09e41cd9a context_gbr_add_eq isl_tab_pip.c:0:0
 #<!-- -->5 0x000055d09e423131 basic_map_partial_lexopt_base_sol isl_tab_pip.c:0:0
 #<!-- -->6 0x000055d09e423b74 basic_map_partial_lexopt_pw_multi_aff isl_tab_pip.c:0:0
 #<!-- -->7 0x000055d09e35a29d basic_map_partial_lexopt_pw_multi_aff isl_map.c:0:0
 #<!-- -->8 0x000055d09e35a5dc isl_map_partial_lexopt_aligned_pw_multi_aff isl_map.c:0:0
 #<!-- -->9 0x000055d09e35b71e isl_map_partial_lexopt_aligned isl_map.c:0:0
#<!-- -->10 0x000055d09e310188 last_source isl_flow.c:0:0
#<!-- -->11 0x000055d09e310d83 access_info_compute_flow_core isl_flow.c:0:0
#<!-- -->12 0x000055d09e313edd isl_union_access_info_compute_flow (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x657eedd)
#<!-- -->13 0x000055d09d416901 buildFlow(isl_union_map*, isl_union_map*, isl_union_map*, isl_union_map*, isl_schedule*) DependenceInfo.cpp:0:0
#<!-- -->14 0x000055d09d419a6b polly::Dependences::calculateDependences(polly::Scop&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x5684a6b)
#<!-- -->15 0x000055d09d41b7fe polly::DependenceAnalysis::Result::recomputeDependences(polly::Dependences::AnalysisLevel) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56867fe)
#<!-- -->16 0x000055d09d4b4489 (anonymous namespace)::runIslScheduleOptimizerImpl(polly::Scop&amp;, llvm::function_ref&lt;polly::Dependences const&amp; (polly::Dependences::AnalysisLevel)&gt;, llvm::TargetTransformInfo*, llvm::OptimizationRemarkEmitter*, isl::schedule&amp;, bool&amp;) (.constprop.0) ScheduleOptimizer.cpp:0:0
#<!-- -->17 0x000055d09d4b6986 polly::runIslScheduleOptimizer(polly::Scop&amp;, llvm::TargetTransformInfo*, polly::DependenceAnalysis::Result&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x5721986)
#<!-- -->18 0x000055d09d484cb8 polly::runPollyPass(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, polly::PollyPassOptions) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56efcb8)
#<!-- -->19 0x000055d09d485d02 polly::PollyFunctionPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56f0d02)
#<!-- -->20 0x000055d09d403706 llvm::detail::PassModel&lt;llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x566e706)
#<!-- -->21 0x000055d09bc26591 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e91591)
#<!-- -->22 0x000055d0992b08e6 llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x151b8e6)
#<!-- -->23 0x000055d09bc24bce llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e8fbce)
#<!-- -->24 0x000055d0992b0f86 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x151bf86)
#<!-- -->25 0x000055d09bc24561 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e8f561)
#<!-- -->26 0x000055d09c5e4f6c (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->27 0x000055d09c5e8914 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4853914)
#<!-- -->28 0x000055d09ccc95b6 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4f345b6)
#<!-- -->29 0x000055d09eb466ec clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x6db16ec)
#<!-- -->30 0x000055d09ccc9a38 clang::CodeGenAction::ExecuteAction() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4f34a38)
#<!-- -->31 0x000055d09cfd2ab9 clang::FrontendAction::Execute() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x523dab9)
#<!-- -->32 0x000055d09cf4ff1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x51baf1e)
#<!-- -->33 0x000055d09d0d532d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x534032d)
#<!-- -->34 0x000055d098dee15f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x105915f)
#<!-- -->35 0x000055d098de495a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->36 0x000055d098de8e8b clang_main(int, char**, llvm::ToolContext const&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x1053e8b)
#<!-- -->37 0x000055d098ca53cb main (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0xf103cb)
#<!-- -->38 0x00007f3667f03083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->39 0x000055d098de43de _start (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x104f3de)
clang: error: unable to execute command: Segmentation fault
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 72b9ae5a9e35dbe2418f057f757d82bfce570150)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/re-aad68e.c
clang: note: diagnostic msg: /tmp/re-aad68e.sh
clang: note: diagnostic msg: 

********************
```

Version: 

```
clang -v
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 72b9ae5a9e35dbe2418f057f757d82bfce570150)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin
Build config: +assertions
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/8
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
Found CUDA installation: /usr/local/cuda-12.1, version 12.1
```
</details>


---

### Comment 2 - Meinersbur

Reduced test case: [reduced.ll.txt](https://github.com/user-attachments/files/24836860/reduced.ll.txt)

The bug goes away with a newer version of ISL, bisected to [39afceed77d7e70f7bd0d5c3bd69e7f9d67c790a](https://repo.or.cz/isl.git/commit/39afceed77d7e70f7bd0d5c3bd69e7f9d67c790a)

Will update ISL, but requires changes to some tests. In particular, it marks more loops as parallel than it should not.

---

### Comment 3 - Meinersbur

Note that the fix may just hide the problem. Probably the max-operations counter exceeded somewhere, and a check for it having happened is missing. [39afceed77d7e70f7bd0d5c3bd69e7f9d67c790a](https://repo.or.cz/isl.git/commit/39afceed77d7e70f7bd0d5c3bd69e7f9d67c790a) just changes where the max-operations counter exceeds, now occuring at some place that properly checks for it.

However, reporting a bug upstream on an old development version of ISL will likely be ignored. Need to wait for a reproducer on an current version of ISL. It may also have been fixed in-between already. Reopen this PR or create a new one if needed.

---

