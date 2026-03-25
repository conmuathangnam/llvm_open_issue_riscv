# [Polly] Assertion failure in ScopBuilder::calculateMinMaxAccess with -O3 -mllvm -polly

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/174147
**Status:** Closed
**Labels:** polly, crash
**Closed Date:** 2026-01-14T14:19:20Z

## Body

Clang with polly crashes on valid with an assertion failure when compiling the attached C file in the latest version
with `-O3 -mllvm -polly`, which happens inside Polly's `ScopBuilder::calculateMinMaxAccess`.

Attached Files: 

[driver-2e3465.c](https://github.com/user-attachments/files/24401601/driver-2e3465.c)
[driver-2e3465.sh](https://github.com/user-attachments/files/24401602/driver-2e3465.sh)
[driver.c](https://github.com/user-attachments/files/24401603/driver.c)

Compiled with commit hash `56f5fda5776a9693a79ccea4e23ae1295df81a69` on Dec. 25, with 
cmake option `cmake '-DCMAKE_BUILD_TYPE=Release' '-DLLVM_ENABLE_ASSERTIONS=ON' '-DLLVM_ENABLE_PROJECTS=clang;polly' ../llvm`

Code
```c
#include <stdio.h>
int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
    n[][1][15], p[][1][15], q[][1][15];
short b[];
int d[];
short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
int x[][15][15], y[][15][15];
_Bool o[][1][1];
_Bool s[][1];
void z() {
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        a[t][u][v] = 6;
  for (size_t t = 0; t < 5; ++t)
    b[t] = 8;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        c[t][u][v] = 70;
  for (size_t t = 0; t < 5; ++t)
    d[t] = 400431475742631619;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        e[t][u][v] = 30;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        f[t][u][v] = 80048846733619458;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        g[t][u][v] = 3;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        w[t][u][v] = 2;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        i[t][u][v] = 9;
  for (size_t t = 0; t < 5;)
    for (size_t u = 0; u < 5; ++u)
      for (t = 0; t < 15; ++t)
        for (size_t v = 0; v < 15; ++v)
          x[t][u][v] = 9;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        j[t][u][v] = 5;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        k[t][u][v] = 7;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        l[t][u][v] = 5;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        m[t][u][v] = 901674895436748;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        n[t][u][v] = 6018762;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 5; ++u)
      for (size_t v = 0; v < 5; ++v)
        o[t][u][v] = 0;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        p[t][u][v] = 801932439917686;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        q[t][u][v] = 6;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        r[t][u][v] = 9;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 5; ++u)
      s[t][u] = 1;
  for (size_t t = 0; t < 5; ++t)
    for (size_t u = 0; u < 15; ++u)
      for (size_t v = 0; v < 15; ++v)
        y[t][u][v] = 2;
}
```

diagnostic message: 
```
driver.c:22:12: warning: implicit conversion from 'long' to 'int' changes value from 400431475742631619 to 1015441091
      [-Wconstant-conversion]
   22 |     d[t] = 400431475742631619;
      |          ~ ^~~~~~~~~~~~~~~~~~
driver.c:30:22: warning: implicit conversion from 'long' to 'int' changes value from 80048846733619458 to -2109951742
      [-Wconstant-conversion]
   30 |         f[t][u][v] = 80048846733619458;
      |                    ~ ^~~~~~~~~~~~~~~~~
driver.c:63:22: warning: implicit conversion from 'long' to 'int' changes value from 901674895436748 to -1948750900
      [-Wconstant-conversion]
   63 |         m[t][u][v] = 901674895436748;
      |                    ~ ^~~~~~~~~~~~~~~
driver.c:75:22: warning: implicit conversion from 'long' to 'int' changes value from 801932439917686 to 1916212342
      [-Wconstant-conversion]
   75 |         p[t][u][v] = 801932439917686;
      |                    ~ ^~~~~~~~~~~~~~~
driver.c:2:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |     ^
driver.c:2:17: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                 ^
driver.c:2:29: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                             ^
driver.c:2:41: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                                         ^
driver.c:2:53: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                                                     ^
driver.c:2:65: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                                                                 ^
driver.c:3:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    3 |     n[][1][15], p[][1][15], q[][1][15];
      |     ^
driver.c:3:17: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    3 |     n[][1][15], p[][1][15], q[][1][15];
      |                 ^
driver.c:3:29: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    3 |     n[][1][15], p[][1][15], q[][1][15];
      |                             ^
driver.c:4:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    4 | short b[];
      |       ^
driver.c:5:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    5 | int d[];
      |     ^
driver.c:6:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |       ^
driver.c:6:19: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                   ^
driver.c:6:31: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                               ^
driver.c:6:43: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                                           ^
driver.c:6:55: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                                                       ^
driver.c:6:67: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                                                                   ^
driver.c:7:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    7 | int x[][15][15], y[][15][15];
      |     ^
driver.c:7:18: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    7 | int x[][15][15], y[][15][15];
      |                  ^
driver.c:8:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    8 | _Bool o[][1][1];
      |       ^
driver.c:9:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    9 | _Bool s[][1];
      |       ^
clang-22: /home/lenovo/thapgua/compiler-build/llvm-project/polly/include/polly/Support/ISLTools.h:33: void islAssert(const isl::size&): Assertion `!Size.is_error()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name driver.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/thapgua/yarpgen/bug-reduce/crash/reduce_case_000123_31940bc1_func_20251231_200251 -fcoverage-compilation-dir=/home/lenovo/thapgua/yarpgen/bug-reduce/crash/reduce_case_000123_31940bc1_func_20251231_200251 -resource-dir /home/lenovo/thapgua/compiler-build/llvm-project/build/lib/clang/22 -I . -internal-isystem /home/lenovo/thapgua/compiler-build/llvm-project/build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=125 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/driver-16f908.o -x c driver.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "driver.c"
4.      Running pass "polly" on function "z"
 #0 0x00005642858f20cf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x45460cf)
 #1 0x00005642858eed84 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007ff216e89420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #3 0x00007ff21697200b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #4 0x00007ff216951859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #5 0x00007ff216951729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #6 0x00007ff216962fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #7 0x0000564286a259f3 (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56799f3)
 #8 0x0000564286a2a62d polly::ScopBuilder::calculateMinMaxAccess(llvm::SmallVector<polly::MemoryAccess*, 4u>, llvm::SmallVector<std::pair<isl::pw_multi_aff, isl::pw_multi_aff>, 4u>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x567e62d)
 #9 0x0000564286a46ade polly::ScopBuilder::buildAliasGroup(llvm::SmallVector<polly::MemoryAccess*, 4u>&, llvm::DenseSet<polly::ScopArrayInfo const*, llvm::DenseMapInfo<polly::ScopArrayInfo const*, void>>) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569aade)
#10 0x0000564286a474c6 polly::ScopBuilder::buildAliasGroups() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569b4c6)
#11 0x0000564286a47738 polly::ScopBuilder::buildAliasChecks() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569b738)
#12 0x0000564286a48860 polly::ScopBuilder::buildScop(llvm::Region&, llvm::AssumptionCache&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569c860)
#13 0x0000564286a49772 polly::ScopBuilder::ScopBuilder(llvm::Region*, llvm::AssumptionCache&, llvm::AAResults&, llvm::DataLayout const&, llvm::DominatorTree&, llvm::LoopInfo&, polly::ScopDetection&, llvm::ScalarEvolution&, llvm::OptimizationRemarkEmitter&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569d772)
#14 0x0000564286a22f82 polly::ScopInfo::recompute() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x5676f82)
#15 0x0000564286a52035 polly::runPollyPass(llvm::Function&, llvm::AnalysisManager<llvm::Function>&, polly::PollyPassOptions) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56a6035)
#16 0x0000564286a54062 polly::PollyFunctionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56a8062)
#17 0x00005642869d1926 llvm::detail::PassModel<llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x5625926)
#18 0x000056428520d031 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e61031)
#19 0x00005642828b31b6 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x15071b6)
#20 0x000056428520b66e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e5f66e)
#21 0x00005642828b3856 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x1507856)
#22 0x000056428520b001 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e5f001)
#23 0x0000564285bc0138 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#24 0x0000564285bc3e32 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4817e32)
#25 0x00005642862a06b6 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4ef46b6)
#26 0x00005642880f483c clang::ParseAST(clang::Sema&, bool, bool) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x6d4883c)
#27 0x00005642862a0b38 clang::CodeGenAction::ExecuteAction() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4ef4b38)
#28 0x00005642865a58c9 clang::FrontendAction::Execute() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x51f98c9)
#29 0x00005642865234ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x51774ee)
#30 0x00005642866a792d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x52fb92d)
#31 0x00005642823f658f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x104a58f)
#32 0x00005642823ece0a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#33 0x00005642823f133b clang_main(int, char**, llvm::ToolContext const&) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x104533b)
#34 0x00005642822aa2cb main (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0xefe2cb)
#35 0x00007ff216953083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#36 0x00005642823ec88e _start (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x104088e)
clang: error: unable to execute command: Aborted
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 56f5fda5776a9693a79ccea4e23ae1295df81a69)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/driver-2e3465.c
clang: note: diagnostic msg: /tmp/driver-2e3465.sh
clang: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-polly

Author: None (thapgua)

<details>
Clang with polly crashes on valid with an assertion failure when compiling the attached C file in the latest version
with `-O3 -mllvm -polly`, which happens inside Polly's `ScopBuilder::calculateMinMaxAccess`.

Attached Files: 

[driver-2e3465.c](https://github.com/user-attachments/files/24401601/driver-2e3465.c)
[driver-2e3465.sh](https://github.com/user-attachments/files/24401602/driver-2e3465.sh)
[driver.c](https://github.com/user-attachments/files/24401603/driver.c)

Compiled with commit hash `56f5fda5776a9693a79ccea4e23ae1295df81a69` on Dec. 25, with 
cmake option `cmake '-DCMAKE_BUILD_TYPE=Release' '-DLLVM_ENABLE_ASSERTIONS=ON' '-DLLVM_ENABLE_PROJECTS=clang;polly' ../llvm`

Code
```c
#include &lt;stdio.h&gt;
int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
    n[][1][15], p[][1][15], q[][1][15];
short b[];
int d[];
short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
int x[][15][15], y[][15][15];
_Bool o[][1][1];
_Bool s[][1];
void z() {
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        a[t][u][v] = 6;
  for (size_t t = 0; t &lt; 5; ++t)
    b[t] = 8;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        c[t][u][v] = 70;
  for (size_t t = 0; t &lt; 5; ++t)
    d[t] = 400431475742631619;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        e[t][u][v] = 30;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        f[t][u][v] = 80048846733619458;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        g[t][u][v] = 3;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        w[t][u][v] = 2;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        i[t][u][v] = 9;
  for (size_t t = 0; t &lt; 5;)
    for (size_t u = 0; u &lt; 5; ++u)
      for (t = 0; t &lt; 15; ++t)
        for (size_t v = 0; v &lt; 15; ++v)
          x[t][u][v] = 9;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        j[t][u][v] = 5;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        k[t][u][v] = 7;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        l[t][u][v] = 5;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        m[t][u][v] = 901674895436748;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        n[t][u][v] = 6018762;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 5; ++u)
      for (size_t v = 0; v &lt; 5; ++v)
        o[t][u][v] = 0;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        p[t][u][v] = 801932439917686;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        q[t][u][v] = 6;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        r[t][u][v] = 9;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 5; ++u)
      s[t][u] = 1;
  for (size_t t = 0; t &lt; 5; ++t)
    for (size_t u = 0; u &lt; 15; ++u)
      for (size_t v = 0; v &lt; 15; ++v)
        y[t][u][v] = 2;
}
```

diagnostic message: 
```
driver.c:22:12: warning: implicit conversion from 'long' to 'int' changes value from 400431475742631619 to 1015441091
      [-Wconstant-conversion]
   22 |     d[t] = 400431475742631619;
      |          ~ ^~~~~~~~~~~~~~~~~~
driver.c:30:22: warning: implicit conversion from 'long' to 'int' changes value from 80048846733619458 to -2109951742
      [-Wconstant-conversion]
   30 |         f[t][u][v] = 80048846733619458;
      |                    ~ ^~~~~~~~~~~~~~~~~
driver.c:63:22: warning: implicit conversion from 'long' to 'int' changes value from 901674895436748 to -1948750900
      [-Wconstant-conversion]
   63 |         m[t][u][v] = 901674895436748;
      |                    ~ ^~~~~~~~~~~~~~~
driver.c:75:22: warning: implicit conversion from 'long' to 'int' changes value from 801932439917686 to 1916212342
      [-Wconstant-conversion]
   75 |         p[t][u][v] = 801932439917686;
      |                    ~ ^~~~~~~~~~~~~~~
driver.c:2:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |     ^
driver.c:2:17: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                 ^
driver.c:2:29: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                             ^
driver.c:2:41: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                                         ^
driver.c:2:53: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                                                     ^
driver.c:2:65: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    2 | int a[][1][15], c[][1][15], e[][1][15], f[][1][15], g[][1][15], m[][1][15],
      |                                                                 ^
driver.c:3:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    3 |     n[][1][15], p[][1][15], q[][1][15];
      |     ^
driver.c:3:17: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    3 |     n[][1][15], p[][1][15], q[][1][15];
      |                 ^
driver.c:3:29: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    3 |     n[][1][15], p[][1][15], q[][1][15];
      |                             ^
driver.c:4:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    4 | short b[];
      |       ^
driver.c:5:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    5 | int d[];
      |     ^
driver.c:6:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |       ^
driver.c:6:19: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                   ^
driver.c:6:31: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                               ^
driver.c:6:43: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                                           ^
driver.c:6:55: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                                                       ^
driver.c:6:67: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    6 | short w[][1][15], i[][1][15], j[][1][15], k[][1][15], l[][1][15], r[][1][15];
      |                                                                   ^
driver.c:7:5: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    7 | int x[][15][15], y[][15][15];
      |     ^
driver.c:7:18: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    7 | int x[][15][15], y[][15][15];
      |                  ^
driver.c:8:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    8 | _Bool o[][1][1];
      |       ^
driver.c:9:7: warning: tentative array definition assumed to have one element [-Wtentative-definition-array]
    9 | _Bool s[][1];
      |       ^
clang-22: /home/lenovo/thapgua/compiler-build/llvm-project/polly/include/polly/Support/ISLTools.h:33: void islAssert(const isl::size&amp;): Assertion `!Size.is_error()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name driver.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/thapgua/yarpgen/bug-reduce/crash/reduce_case_000123_31940bc1_func_20251231_200251 -fcoverage-compilation-dir=/home/lenovo/thapgua/yarpgen/bug-reduce/crash/reduce_case_000123_31940bc1_func_20251231_200251 -resource-dir /home/lenovo/thapgua/compiler-build/llvm-project/build/lib/clang/22 -I . -internal-isystem /home/lenovo/thapgua/compiler-build/llvm-project/build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=125 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -polly -faddrsig -fdwarf2-cfi-asm -o /tmp/driver-16f908.o -x c driver.c
1.      &lt;eof&gt; parser at end of file
2.      Optimizer
3.      Running pass "function&lt;eager-inv&gt;(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,polly,sroa&lt;modify-cfg&gt;,early-cse&lt;memssa&gt;,speculative-execution&lt;only-if-divergent-target&gt;,jump-threading,correlated-propagation,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm&lt;no-allowspeculation&gt;,loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,licm&lt;allowspeculation&gt;,simple-loop-unswitch&lt;nontrivial;trivial&gt;),simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa&lt;modify-cfg&gt;,vector-combine,mldst-motion&lt;no-split-footer-bb&gt;,gvn&lt;&gt;,sccp,bdce,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm&lt;allowspeculation&gt;),coro-elide,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop(loop-rotate&lt;header-duplication;no-prepare-for-lto&gt;,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize&lt;no-interleave-forced-only;no-vectorize-forced-only;&gt;,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,simplifycfg&lt;bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables&gt;,slp-vectorizer,vector-combine,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-unroll&lt;O3&gt;,transform-warning,sroa&lt;preserve-cfg&gt;,infer-alignment,instcombine&lt;max-iterations=1;no-verify-fixpoint&gt;,loop-mssa(licm&lt;allowspeculation&gt;),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg&lt;bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables&gt;)" on module "driver.c"
4.      Running pass "polly" on function "z"
 #<!-- -->0 0x00005642858f20cf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x45460cf)
 #<!-- -->1 0x00005642858eed84 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007ff216e89420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->3 0x00007ff21697200b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #<!-- -->4 0x00007ff216951859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #<!-- -->5 0x00007ff216951729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #<!-- -->6 0x00007ff216962fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #<!-- -->7 0x0000564286a259f3 (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56799f3)
 #<!-- -->8 0x0000564286a2a62d polly::ScopBuilder::calculateMinMaxAccess(llvm::SmallVector&lt;polly::MemoryAccess*, 4u&gt;, llvm::SmallVector&lt;std::pair&lt;isl::pw_multi_aff, isl::pw_multi_aff&gt;, 4u&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x567e62d)
 #<!-- -->9 0x0000564286a46ade polly::ScopBuilder::buildAliasGroup(llvm::SmallVector&lt;polly::MemoryAccess*, 4u&gt;&amp;, llvm::DenseSet&lt;polly::ScopArrayInfo const*, llvm::DenseMapInfo&lt;polly::ScopArrayInfo const*, void&gt;&gt;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569aade)
#<!-- -->10 0x0000564286a474c6 polly::ScopBuilder::buildAliasGroups() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569b4c6)
#<!-- -->11 0x0000564286a47738 polly::ScopBuilder::buildAliasChecks() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569b738)
#<!-- -->12 0x0000564286a48860 polly::ScopBuilder::buildScop(llvm::Region&amp;, llvm::AssumptionCache&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569c860)
#<!-- -->13 0x0000564286a49772 polly::ScopBuilder::ScopBuilder(llvm::Region*, llvm::AssumptionCache&amp;, llvm::AAResults&amp;, llvm::DataLayout const&amp;, llvm::DominatorTree&amp;, llvm::LoopInfo&amp;, polly::ScopDetection&amp;, llvm::ScalarEvolution&amp;, llvm::OptimizationRemarkEmitter&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x569d772)
#<!-- -->14 0x0000564286a22f82 polly::ScopInfo::recompute() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x5676f82)
#<!-- -->15 0x0000564286a52035 polly::runPollyPass(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;, polly::PollyPassOptions) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56a6035)
#<!-- -->16 0x0000564286a54062 polly::PollyFunctionPass::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x56a8062)
#<!-- -->17 0x00005642869d1926 llvm::detail::PassModel&lt;llvm::Function, polly::PollyFunctionPass, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x5625926)
#<!-- -->18 0x000056428520d031 llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e61031)
#<!-- -->19 0x00005642828b31b6 llvm::detail::PassModel&lt;llvm::Function, llvm::PassManager&lt;llvm::Function, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;, llvm::AnalysisManager&lt;llvm::Function&gt;&gt;::run(llvm::Function&amp;, llvm::AnalysisManager&lt;llvm::Function&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x15071b6)
#<!-- -->20 0x000056428520b66e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e5f66e)
#<!-- -->21 0x00005642828b3856 llvm::detail::PassModel&lt;llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x1507856)
#<!-- -->22 0x000056428520b001 llvm::PassManager&lt;llvm::Module, llvm::AnalysisManager&lt;llvm::Module&gt;&gt;::run(llvm::Module&amp;, llvm::AnalysisManager&lt;llvm::Module&gt;&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x3e5f001)
#<!-- -->23 0x0000564285bc0138 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;&amp;, std::unique_ptr&lt;llvm::ToolOutputFile, std::default_delete&lt;llvm::ToolOutputFile&gt;&gt;&amp;, clang::BackendConsumer*) BackendUtil.cpp:0:0
#<!-- -->24 0x0000564285bc3e32 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4817e32)
#<!-- -->25 0x00005642862a06b6 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4ef46b6)
#<!-- -->26 0x00005642880f483c clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x6d4883c)
#<!-- -->27 0x00005642862a0b38 clang::CodeGenAction::ExecuteAction() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x4ef4b38)
#<!-- -->28 0x00005642865a58c9 clang::FrontendAction::Execute() (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x51f98c9)
#<!-- -->29 0x00005642865234ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x51774ee)
#<!-- -->30 0x00005642866a792d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x52fb92d)
#<!-- -->31 0x00005642823f658f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x104a58f)
#<!-- -->32 0x00005642823ece0a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->33 0x00005642823f133b clang_main(int, char**, llvm::ToolContext const&amp;) (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x104533b)
#<!-- -->34 0x00005642822aa2cb main (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0xefe2cb)
#<!-- -->35 0x00007ff216953083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->36 0x00005642823ec88e _start (/home/lenovo/thapgua/compiler-build/llvm-project/build/bin/clang-22+0x104088e)
clang: error: unable to execute command: Aborted
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 56f5fda5776a9693a79ccea4e23ae1295df81a69)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/lenovo/thapgua/compiler-build/llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/driver-2e3465.c
clang: note: diagnostic msg: /tmp/driver-2e3465.sh
clang: note: diagnostic msg: 

********************
```
</details>


---

### Comment 2 - rengolin

@tobiasgrosser 

---

