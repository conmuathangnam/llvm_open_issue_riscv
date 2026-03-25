# [LoopLoadElimination] Clang crashes at -O2/s when compiling with profile data using PGO: unhandled irreducible control flow

**Author:** zxt5
**URL:** https://github.com/llvm/llvm-project/issues/187643
**Status:** Open
**Labels:** loopoptim, PGO, crash

## Body

small.c
```c
#define a() 0
int b, c;
int d() {
  int j, e = 0, f;
  if (b > 2147483647)
    goto g;
  do {
  g:
    if (c) {
      if (a() <= c)
        return 1;
      if (e == f) {
        e = 1;
        f = 0;
        continue;
      }
    }
    if (f < 3)
      f++;
    else {
      int h;
      for (int i = 0; i < 1; i++)
        f = 0;
      j++;
    }
  } while (1);
}
int main() {}
```

This code triggers clang to crash when compiling with the profile data collected as follows:

```
$ clang-trunk -O2 -fprofile-instr-generate small.c 
$ LLVM_PROFILE_FILE="test.profraw"  ./a.out
$ llvm-profdata merge -o test.profdata test.profraw
```

Then, the crash happens when compiling with `test.profdata` using PGO:

```
$ clang-trunk -O2 -fprofile-instr-use=test.profdata small.c
unhandled irreducible control flow
UNREACHABLE executed at /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Analysis/BlockFrequencyInfoImpl.h:1345!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/x27zhou/compiler-nightly/install/llvm/bin/clang-23 -cc1 -triple aarch64-unknown-linux-gnu -O2 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=non-leaf-no-reserve -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -enable-tlsdesc -target-cpu generic -target-feature +v8a -target-feature +fp-armv8 -target-feature +neon -target-abi aapcs -debugger-tuning=gdb -fdebug-compilation-dir=/data/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz2-output-2026-03-19-19-50-18/crash-clang-trunk-12838/reduction1 -fprofile-instrument-use=clang -fprofile-instrument-use-path=test.profdata -fcoverage-compilation-dir=/data/x27zhou/fuzz-pgo/PGOFuzz/pgofuzz2-output-2026-03-19-19-50-18/crash-clang-trunk-12838/reduction1 -resource-dir /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23 -internal-isystem /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/aarch64-linux-gnu/11/../../../../aarch64-linux-gnu/include -internal-externc-isystem /usr/include/aarch64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=288 -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -target-feature +outline-atomics -target-feature -fmv -faddrsig -fdwarf2-cfi-asm -o /tmp/small-c10eaf.o -x c small.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O2>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "small.c"
4.      Running pass "loop-load-elim" on function "d"
 #0 0x0000aaaae3b72bf8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:884:3
 #1 0x0000aaaae3b7019c llvm::sys::RunSignalHandlers() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #2 0x0000aaaae3b70860 SignalHandler(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:448:14
 #3 0x0000aaaae3b70958 int const* std::__find_if<int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>>(int const*, int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>) /usr/include/c++/11/bits/stl_algobase.h:2114:23
 #4 0x0000aaaae3b70958 int const* std::find<int const*, int>(int const*, int const*, int const&) /usr/include/c++/11/bits/stl_algo.h:3884:28
 #5 0x0000aaaae3b70958 bool llvm::is_contained<int const (&) [10], int>(int const (&) [10], int const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:1953:21
 #6 0x0000aaaae3b70958 SignalHandlerTerminate(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:464:25
 #7 0x0000ffff95f2b7dc (linux-vdso.so.1+0x7dc)
 #8 0x0000ffff95a92008 (/lib/aarch64-linux-gnu/libc.so.6+0x82008)
 #9 0x0000ffff95a4a83c gsignal (/lib/aarch64-linux-gnu/libc.so.6+0x3a83c)
#10 0x0000ffff95a37134 abort (/lib/aarch64-linux-gnu/libc.so.6+0x27134)
#11 0x0000aaaae3ad183c (/data/x27zhou/compiler-nightly/install/llvm/bin/clang-23+0x3cf183c)
#12 0x0000aaaae2aff604 llvm::detail::DenseMapPair<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode> const* llvm::DenseMapBase<llvm::DenseMap<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode>>, llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode>>::doFind<llvm::BasicBlock const*>(llvm::BasicBlock const* const&) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/DenseMap.h:625:14
#13 0x0000aaaae2aff604 llvm::DenseMapBase<llvm::DenseMap<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode>>, llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode>>::contains(llvm::BasicBlock const*) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/DenseMap.h:170:18
#14 0x0000aaaae2aff604 llvm::DenseMapBase<llvm::DenseMap<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode>>, llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode, llvm::DenseMapInfo<llvm::BasicBlock const*, void>, llvm::detail::DenseMapPair<llvm::BasicBlock const*, llvm::BlockFrequencyInfoImplBase::BlockNode>>::count(llvm::BasicBlock const*) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/DenseMap.h:175:20
#15 0x0000aaaae2aff604 llvm::BlockFrequencyInfoImpl<llvm::BasicBlock>::calculate(llvm::Function const&, llvm::BranchProbabilityInfo const&, llvm::LoopInfo const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Analysis/BlockFrequencyInfoImpl.h:1133:23
#16 0x0000aaaae2b00564 llvm::cl::opt_storage<llvm::GVDAGType, false, false>::getValue() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/CommandLine.h:1437:38
#17 0x0000aaaae2b00564 llvm::cl::opt_storage<llvm::GVDAGType, false, false>::operator llvm::GVDAGType() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/CommandLine.h:1441:46
#18 0x0000aaaae2b00564 llvm::BlockFrequencyInfo::calculate(llvm::Function const&, llvm::BranchProbabilityInfo const&, llvm::LoopInfo const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Analysis/BlockFrequencyInfo.cpp:190:38
#19 0x0000aaaae2b007a0 llvm::BlockFrequencyAnalysis::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Analysis/BlockFrequencyInfo.cpp:338:1
#20 0x0000aaaae4b36f30 std::_MakeUniq<llvm::detail::AnalysisResultModel<llvm::Function, llvm::BlockFrequencyAnalysis, llvm::BlockFrequencyInfo, llvm::AnalysisManager<llvm::Function>::Invalidator, true>>::__single_object std::make_unique<llvm::detail::AnalysisResultModel<llvm::Function, llvm::BlockFrequencyAnalysis, llvm::BlockFrequencyInfo, llvm::AnalysisManager<llvm::Function>::Invalidator, true>, llvm::BlockFrequencyInfo>(llvm::BlockFrequencyInfo&&) /usr/include/c++/11/bits/unique_ptr.h:962:69
#21 0x0000aaaae4b36f30 llvm::detail::AnalysisPassModel<llvm::Function, llvm::BlockFrequencyAnalysis, llvm::AnalysisManager<llvm::Function>::Invalidator>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:316:65
#22 0x0000aaaae3550b9c __gnu_cxx::new_allocator<std::_List_node<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>>>::allocate(unsigned long, void const*) /usr/include/c++/11/ext/new_allocator.h:127:41
#23 0x0000aaaae3550b9c std::allocator_traits<std::allocator<std::_List_node<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>>>>::allocate(std::allocator<std::_List_node<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>>>&, unsigned long) /usr/include/c++/11/bits/alloc_traits.h:464:28
#24 0x0000aaaae3550b9c std::__cxx11::_List_base<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>, std::allocator<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>>>::_M_get_node() /usr/include/c++/11/bits/stl_list.h:443:44
#25 0x0000aaaae3550b9c std::_List_node<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>>* std::__cxx11::list<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>, std::allocator<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>>>::_M_create_node<llvm::AnalysisKey*&, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>(llvm::AnalysisKey*&, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>&&) /usr/include/c++/11/bits/stl_list.h:635:32
#26 0x0000aaaae3550b9c void std::__cxx11::list<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>, std::allocator<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>>>::_M_insert<llvm::AnalysisKey*&, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>(std::_List_iterator<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>>, llvm::AnalysisKey*&, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>&&) /usr/include/c++/11/bits/stl_list.h:1912:32
#27 0x0000aaaae3550b9c std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>& std::__cxx11::list<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>, std::allocator<std::pair<llvm::AnalysisKey*, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>>>::emplace_back<llvm::AnalysisKey*&, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>>(llvm::AnalysisKey*&, std::unique_ptr<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>, std::default_delete<llvm::detail::AnalysisResultConcept<llvm::Function, llvm::AnalysisManager<llvm::Function>::Invalidator>>>&&) /usr/include/c++/11/bits/stl_list.h:1228:19
#28 0x0000aaaae3550b9c llvm::AnalysisManager<llvm::Function>::getResultImpl(llvm::AnalysisKey*, llvm::Function&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:148:28
#29 0x0000aaaae750fa58 llvm::BlockFrequencyAnalysis::Result& llvm::AnalysisManager<llvm::Function>::getResult<llvm::BlockFrequencyAnalysis>(llvm::Function&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManager.h:421:55
#30 0x0000aaaae750fa58 llvm::LoopLoadEliminationPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Scalar/LoopLoadElimination.cpp:694:44
#31 0x0000aaaae4b32e10 llvm::detail::PassModel<llvm::Function, llvm::LoopLoadEliminationPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:92:3
#32 0x0000aaaae3553af4 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:18
#33 0x0000aaaae1134120 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:92:3
#34 0x0000aaaae3553f08 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/PassManager.cpp:132:41
#35 0x0000aaaae1134ad0 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:92:3
#36 0x0000aaaae3551ba0 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:18
#37 0x0000aaaae3f1a3e0 llvm::SmallPtrSetImplBase::~SmallPtrSetImplBase() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:89:5
#38 0x0000aaaae3f1a3e0 llvm::SmallPtrSetImpl<llvm::AnalysisKey*>::~SmallPtrSetImpl() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:368:35
#39 0x0000aaaae3f1a3e0 llvm::SmallPtrSet<llvm::AnalysisKey*, 2u>::~SmallPtrSet() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:527:7
#40 0x0000aaaae3f1a3e0 llvm::PreservedAnalyses::~PreservedAnalyses() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/Analysis.h:112:7
#41 0x0000aaaae3f1a3e0 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1209:12
#42 0x0000aaaae3f1cf34 actionRequiresCodeGen /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:321:68
#43 0x0000aaaae3f1cf34 RunCodegenPipeline /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1218:29
#44 0x0000aaaae3f1cf34 emitAssembly /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1301:21
#45 0x0000aaaae3f1cf34 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1475:25
#46 0x0000aaaae4505984 std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>::~unique_ptr() /usr/include/c++/11/bits/unique_ptr.h:360:12
#47 0x0000aaaae4505984 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:312:20
#48 0x0000aaaae5ac6430 clang::ParseAST(clang::Sema&, bool, bool) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Parse/ParseAST.cpp:183:34
#49 0x0000aaaae477878c clang::FrontendAction::Execute() /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1330:38
#50 0x0000aaaae471a720 llvm::Error::getPtr() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#51 0x0000aaaae471a720 llvm::Error::operator bool() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:241:22
#52 0x0000aaaae471a720 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:989:42
#53 0x0000aaaae485088c std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/11/bits/shared_ptr_base.h:1296:16
#54 0x0000aaaae485088c std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/11/bits/shared_ptr_base.h:993:69
#55 0x0000aaaae485088c std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/11/bits/shared_ptr_base.h:979:2
#56 0x0000aaaae485088c clang::CompilerInvocation::getFrontendOpts() /data/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#57 0x0000aaaae485088c clang::CompilerInstance::getFrontendOpts() /data/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:310:39
#58 0x0000aaaae485088c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:319:29
#59 0x0000aaaae0d0c1d0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/cc1_main.cpp:302:40
#60 0x0000aaaae0d047c4 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:229:20
#61 0x0000aaaae0d07110 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::release() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#62 0x0000aaaae0d07110 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::~IntrusiveRefCntPtr() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#63 0x0000aaaae0d07110 clang_main(int, char**, llvm::ToolContext const&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:277:26
#64 0x0000aaaae0c23474 main /data/x27zhou/compiler-nightly/build/llvm/tools/clang/tools/driver/clang-driver.cpp:17:20
#65 0x0000ffff95a37400 (/lib/aarch64-linux-gnu/libc.so.6+0x27400)
#66 0x0000ffff95a374d8 __libc_start_main (/lib/aarch64-linux-gnu/libc.so.6+0x274d8)
#67 0x0000aaaae0d03b30 _start (/data/x27zhou/compiler-nightly/install/llvm/bin/clang-23+0xf23b30)
clang-trunk: error: unable to execute command: Aborted (core dumped)
clang-trunk: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 4262045ba98d11c930fec0fe02584b072ace19bd)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/x27zhou/compiler-nightly/install/llvm/bin
Build config: +assertions
clang-trunk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-trunk: note: diagnostic msg: /tmp/small-490374.c
clang-trunk: note: diagnostic msg: /tmp/small-490374.sh
clang-trunk: note: diagnostic msg: 

********************
```





