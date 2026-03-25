# [clang] Crash at -O3 with PGO due to Floating point exception in InstructionCost

**Author:** zxt5
**URL:** https://github.com/llvm/llvm-project/issues/182789
**Status:** Open
**Labels:** vectorizers, crash

## Body

small.c

```c
long a;
int b;
int c[];
int f() {
  int d = 2;
  b = 0;
  for (; b < 32; b++) {
    if (b < 16 || 0 >= d)
      continue;
    int e = !!(a & b);
    if (e)
      ;
    else
      c[b] = b;
  }
  return 0;
}
void main() {}
```

This code crashes when compiled at -O3 with PGO.

Steps to reproduce:

1. Generate the profile using [profile.txt](https://github.com/user-attachments/files/25474074/profile.txt):

```
llvm-profdata merge -o test.profdata profile.txt
```

2. Compile at -O3 with PGO:

```
clang-trunk -O3 -fprofile-instr-use=test.profdata small.c
```

Crash backtrace:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/x27zhou/compiler-nightly/install/llvm-without-sanitizer/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz-output-2026-02-22-00-32-55/crash-clang-trunk-26327 -fprofile-instrument-use=clang -fprofile-instrument-use-path=test.profdata -fcoverage-compilation-dir=/home/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz-output-2026-02-22-00-32-55/crash-clang-trunk-26327 -resource-dir /data/x27zhou/compiler-nightly/install/llvm-without-sanitizer/lib/clang/23 -internal-isystem /data/x27zhou/compiler-nightly/install/llvm-without-sanitizer/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=265 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -o /tmp/small-ef76bd.o -x c small.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "small.c"
4.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "f"
 #0 0x000055b5950cf347 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:13
 #1 0x000055b5950cd053 llvm::sys::RunSignalHandlers() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #2 0x000055b5950d01e5 SignalHandler(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:448:38
 #3 0x000055b5950d0048 int const* std::__find_if<int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>>(int const*, int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>, std::random_access_iterator_tag) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_algobase.h:2067:8
 #4 0x000055b5950d0048 int const* std::__find_if<int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>>(int const*, int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_algobase.h:2112:14
 #5 0x000055b5950d0048 int const* std::find<int const*, int>(int const*, int const*, int const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_algo.h:3851:14
 #6 0x000055b5950d0048 bool llvm::is_contained<int const (&) [10], int>(int const (&) [10], int const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:1953:12
 #7 0x000055b5950d0048 SignalHandlerTerminate(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:464:7
 #8 0x00007f896a3ff520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #9 0x000055b595339102 llvm::InstructionCost::operator/=(llvm::InstructionCost const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/InstructionCost.h:158:11
#10 0x000055b595339102 llvm::InstructionCost::operator/=(long) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/InstructionCost.h:164:11
#11 0x000055b595339102 llvm::VPReplicateRecipe::computeCost(llvm::ElementCount, llvm::VPCostContext&) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/VPlanRecipes.cpp:3573:12
#12 0x000055b595326d0a llvm::VPRecipeBase::cost(llvm::ElementCount, llvm::VPCostContext&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/VPlanRecipes.cpp:287:18
#13 0x000055b5952eea44 llvm::InstructionCost::operator+=(llvm::InstructionCost const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/InstructionCost.h:104:9
#14 0x000055b5952eea44 llvm::VPBasicBlock::cost(llvm::ElementCount, llvm::VPCostContext&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/VPlan.cpp:782:10
#15 0x000055b5952eecd8 llvm::VPRegionBlock::cost(llvm::ElementCount, llvm::VPCostContext&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/VPlan.cpp:830:16
#16 0x000055b5952eeebb llvm::InstructionCost::operator+=(llvm::InstructionCost const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/InstructionCost.h:104:9
#17 0x000055b5952eeebb llvm::VPRegionBlock::cost(llvm::ElementCount, llvm::VPCostContext&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/VPlan.cpp:809:12
#18 0x000055b5952f188e llvm::InstructionCost::isValid() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/InstructionCost.h:80:39
#19 0x000055b5952f188e llvm::VPlan::cost(llvm::ElementCount, llvm::VPCostContext&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/VPlan.cpp:1023:13
#20 0x000055b5952a5639 llvm::InstructionCost::propagateState(llvm::InstructionCost const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/InstructionCost.h:58:19
#21 0x000055b5952a5639 llvm::InstructionCost::operator+=(llvm::InstructionCost const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/InstructionCost.h:100:5
#22 0x000055b5952a5639 llvm::LoopVectorizationPlanner::cost(llvm::VPlan&, llvm::ElementCount) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7065:8
#23 0x000055b5952b2f54 llvm::LoopVectorizationPlanner::computeBestVF() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:7291:51
#24 0x000055b5952c0d31 llvm::LoopVectorizePass::processLoop(llvm::Loop*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:9676:12
#25 0x000055b5952cadd4 llvm::LoopVectorizePass::runImpl(llvm::Function&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:9954:30
#26 0x000055b5952cb68f llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:9992:32
#27 0x000055b595ffa65d llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#28 0x000055b594b24427 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:8
#29 0x000055b592a2cffd llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#30 0x000055b594b2800e llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/PassManager.cpp:132:23
#31 0x000055b592a2dc9d llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#32 0x000055b594b23507 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:8
#33 0x000055b59541294e llvm::SmallPtrSetImplBase::isSmall() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:248:33
#34 0x000055b59541294e llvm::SmallPtrSetImplBase::~SmallPtrSetImplBase() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:89:10
#35 0x000055b59541294e llvm::PreservedAnalyses::~PreservedAnalyses() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/Analysis.h:112:7
#36 0x000055b59541294e (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1208:5
#37 0x000055b59540a5a4 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1217:7
#38 0x000055b59540a5a4 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1300:3
#39 0x000055b59540a5a4 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1472:13
#40 0x000055b595967351 std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/unique_ptr.h:395:6
#41 0x000055b595967351 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:312:3
#42 0x000055b596e3cec7 __gnu_cxx::__normal_iterator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>*, std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>::__normal_iterator(std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>* const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_iterator.h:1073:20
#43 0x000055b596e3cec7 std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>::begin() /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_vector.h:869:16
#44 0x000055b596e3cec7 void clang::finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>(std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>&, clang::Sema const&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Sema/TemplateInstCallback.h:54:16
#45 0x000055b596e3cec7 clang::ParseAST(clang::Sema&, bool, bool) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Parse/ParseAST.cpp:190:3
#46 0x000055b595bd55c6 clang::FrontendAction::Execute() /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1326:10
#47 0x000055b595b475ff llvm::Error::getPtr() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:278:42
#48 0x000055b595b475ff llvm::Error::operator bool() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#49 0x000055b595b475ff clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:23
#50 0x000055b595cc87c5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:310:25
#51 0x000055b59262e9c5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/cc1_main.cpp:304:15
#52 0x000055b59262aec0 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:229:12
#53 0x000055b592629ff6 clang_main(int, char**, llvm::ToolContext const&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:277:12
#54 0x000055b592639d4d main /data/x27zhou/compiler-nightly/build/llvm-without-sanitizer/tools/clang/tools/driver/clang-driver.cpp:17:10
#55 0x00007f896a3e6d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#56 0x00007f896a3e6e40 call_init ./csu/../csu/libc-start.c:128:20
#57 0x00007f896a3e6e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#58 0x000055b5926287d5 _start (/data/x27zhou/compiler-nightly/install/llvm-without-sanitizer/bin/clang-23+0xf797d5)
clang: error: unable to execute command: Floating point exception (core dumped)
clang: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git d5b94174971f4ce5d8659e4ade27e21858221514)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/x27zhou/compiler-nightly/install/llvm-without-sanitizer/bin
Build config: +assertions
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/small-7fdf85.c
clang: note: diagnostic msg: /tmp/small-7fdf85.sh
clang: note: diagnostic msg: 

********************
```

## Comments

### Comment 1 - zxt5

This is bisected to [0c4f809](https://github.com/llvm/llvm-project/commit/0c4f8094939d2a2b50b6cd062cd1473a0315457f), which is committed by @fhahn .

---

