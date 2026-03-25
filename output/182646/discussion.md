# [LoopVectorize] Assertion VPlan cost model and legacy cost model disagreed failed in computeBestVF()

**Author:** thapgua
**URL:** https://github.com/llvm/llvm-project/issues/182646
**Status:** Open
**Labels:** vectorizers, crash

## Body

Clang crashes with an assertion failure in `LoopVectorize.cpp` during the loop vectorization pass. The assertion is: `VPlan cost model and legacy cost model disagreed` with optimization level `-O3`. 

Compiler version: 

```
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 21b346144033994685edd93f73cae7782d0b1381)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
```

Attached files: 

[bug-4b508d.cpp](https://github.com/user-attachments/files/25455091/bug-4b508d.cpp)

[bug-4b508d.sh](https://github.com/user-attachments/files/25455093/bug-4b508d.sh)

Backtrace: 

```
clang-23: /data/lenovo/compiler-build/llvm/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7338: llvm::VectorizationFactor llvm::LoopVectorizationPlanner::computeBestVF(): Assertion `(BestFactor.Width == LegacyVF.Width || BestPlan.hasEarlyExit() || !Legal->getLAI()->getSymbolicStrides().empty() || UsesEVLGatherScatter || planContainsAdditionalSimplifications( getPlanFor(BestFactor.Width), CostCtx, OrigLoop, BestFactor.Width) || planContainsAdditionalSimplifications( getPlanFor(LegacyVF.Width), CostCtx, OrigLoop, LegacyVF.Width)) && " VPlan cost model and legacy cost model disagreed"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lenovo/bug-report/26-2-21 -fcoverage-compilation-dir=/home/lenovo/bug-report/26-2-21 -resource-dir /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /data/lenovo/compiler-build/llvm/llvm-project/build/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fmessage-length=86 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -o /tmp/bug-5d63ca.o -x c++ bug.cpp
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "bug.cpp"
4.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "_Z1cibPA3_A3_b"
 #0 0x000057f364d40402 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4656402)
 #1 0x000057f364d3d05f llvm::sys::RunSignalHandlers() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x465305f)
 #2 0x000057f364d3d1ac SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000057f364d3d26c SignalHandlerTerminate(int, siginfo_t*, void*) Signals.cpp:0:0
 #4 0x0000778140645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x000077814069eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #6 0x000077814064527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #7 0x00007781406288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #8 0x000077814062881b (/lib/x86_64-linux-gnu/libc.so.6+0x2881b)
 #9 0x000077814063b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#10 0x000057f365001303 llvm::LoopVectorizationPlanner::computeBestVF() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4917303)
#11 0x000057f365002fe4 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4918fe4)
#12 0x000057f365005cd1 llvm::LoopVectorizePass::runImpl(llvm::Function&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x491bcd1)
#13 0x000057f365006390 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x491c390)
#14 0x000057f366101cb5 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x5a17cb5)
#15 0x000057f364633ec5 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f49ec5)
#16 0x000057f361b9bd95 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14b1d95)
#17 0x000057f364632611 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f48611)
#18 0x000057f361b9c435 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x14b2435)
#19 0x000057f364632dfd llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x3f48dfd)
#20 0x000057f36516ee9e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#21 0x000057f365172c57 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x4a88c57)
#22 0x000057f3658188d0 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x512e8d0)
#23 0x000057f3675bbf5c clang::ParseAST(clang::Sema&, bool, bool) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x6ed1f5c)
#24 0x000057f365b55e77 clang::FrontendAction::Execute() (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x546be77)
#25 0x000057f365ad944d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x53ef44d)
#26 0x000057f365c5360e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x556960e)
#27 0x000057f3616eaa6f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0x1000a6f)
#28 0x000057f3616dff27 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x000057f3616e4c7f clang_main(int, char**, llvm::ToolContext const&) (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xffac7f)
#30 0x000057f361569380 main (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xe7f380)
#31 0x000077814062a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#32 0x000077814062a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#33 0x000057f3616df495 _start (/data/lenovo/compiler-build/llvm/llvm-project/build/bin/clang-23+0xff5495)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 21b346144033994685edd93f73cae7782d0b1381)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/lenovo/compiler-build/llvm/llvm-project/build/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/bug-4b508d.cpp
clang++: note: diagnostic msg: /tmp/bug-4b508d.sh
clang++: note: diagnostic msg: 

********************
```

Source code: 

```cpp
extern char a[][3][3];
extern bool b[][3];
void c(int d, bool e, bool f[][3][3]) {
  [[assume(d + 1430399086 == 12)]];
  for (int g;;)
    for (char h;;)
#pragma clang loop vectorize_predicate(enable)
      for (int i = 1; i < 10; i += d - 2864568219) {
        a[g][g][i] = 0;
        b[g][g] = f[i][i][h] ?: e;
      }
}
```


## Comments

### Comment 1 - aviralgarg05

I would like to solve this issue, please assign me

---

### Comment 2 - oToToT

I think we assign issues after there is a PR and we check that it is headed in the right direction.

---

### Comment 3 - fhahn

@thapgua is this still crashing for you? The reduced source does not seem to crash any longer: https://clang.godbolt.org/z/P7YEc7jc3

---

