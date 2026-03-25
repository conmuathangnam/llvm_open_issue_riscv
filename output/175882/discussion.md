# [clang] Crash at -O3 with PGO: Assertion `!NodePtr->isKnownSentinel()' failed.

**Author:** zxt5
**URL:** https://github.com/llvm/llvm-project/issues/175882

## Body

small.c:
```
long long c;
char d, k;
short e, l;
short f[];
#define g(a, b)                                                                \
  {                                                                            \
    __typeof__(a) h = a;                                                       \
    __typeof__(b) i = b;                                                       \
    h > i ? h : i;                                                             \
  }
#define j(a, b)                                                                \
  {                                                                            \
    __typeof__(a) h = 0;                                                       \
    __typeof__(b) i = b;                                                       \
    h < i ? h : i;                                                             \
  }
int m, q = 403137266536869, r;
short *n;
long *o;
char s[];
short t[8][18];
long long aa[][18][18];
int u[][18][18][18];
void test(long p, int q, int r, char s[], short t[][18], long long aa[][18][18],
          int u[][18][18][18]) {
  if (1) {
    for (_Bool v = 0; v < 1; v = p)
      ;
    for (int w = 0; w < 18; w += 3) {
      for (char x = 0; x < 8; x += 3)
        for (int y = 0; y < 8; y += 3)
          ;
      for (short z = 0; z < 1; z += 0 ? 0 : 40 ? p : 0) {
        for (long ab = 0; ab < 7; ab = 91)
          for (char ac = 0; ac < 18; ac += 2) {
            for (_Bool ad = t[1][ab] - 1; ad < 1; ad = 1)
              c = l ? n[w] ? s[w] : 0 : u[w][z][w][1];
            for (_Bool ae = q; ae < 1; ae = 1) {
              if (k ? o[ab] : s[2])
                f[ab + ae] &= d = (j(0, aa[1][ab][3]));
              else
                ;
              e = (g(e, (short)r));
            }
          }
        j(0, 0 ? 0 : 0)
      }
      for (int af = 4; af < 6; af += 4)
        ;
      for (int ag = 0; ag < 7; ag += 2)
        2 == m ? 0 : !(0 ? 0 : 0);
      for (int ah = 0 ? 0 < 0 : 0 ? 0 : 0; ah < 0;)
        if (0 ? 0 : 0) {
          if (0)
            0 ? 0 : 0;
          0 ? 0 : 0;
          j(0, 0 > 0)
        }
      !0;
    }
    g(0, (g(0, 0)))
  }
}
void main() { test(60386248197294407, q, r, s, t, aa, u); }
```

This code crashes when compiled at -O3 with PGO.

Steps to reproduce:

1. Generate the profile using [profile.txt](https://github.com/user-attachments/files/24604818/profile.txt):

```
llvm-profdata merge -o test.profdata profile.txt
```

2. Compile at -O3 with PGO:

```
clang-trunk -O3 -fprofile-instr-use=test.profdata small.c
```

Crash:

```
clang-22: /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:322: llvm::ilist_iterator_w_bits::reference llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>::operator*() const [OptionsT = llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, IsReverse = false, IsConst = false]: Assertion `!NodePtr->isKnownSentinel()' failed.
```

Backtrace:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/x27zhou/compiler-nightly/install/llvm/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/x27zhou/fuzz-pgo/outcome-2026-01-13-06-17-27/bug-exit-16-id-137261/reduction/reduce-profile -fprofile-instrument-use=clang -fprofile-instrument-use-path=test.profdata -fcoverage-compilation-dir=/home/x27zhou/fuzz-pgo/outcome-2026-01-13-06-17-27/bug-exit-16-id-137261/reduction/reduce-profile -resource-dir /home/x27zhou/compiler-nightly/install/llvm/lib/clang/22 -internal-isystem /home/x27zhou/compiler-nightly/install/llvm/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=259 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -o /tmp/small-147daf.o -x c small.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,chr,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>,drop-unnecessary-assumes,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,slp-vectorizer,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O3>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "small.c"
4.      Running pass "chr" on function "main"
 #0 0x000055eecfa0cc1b backtrace (/home/x27zhou/compiler-nightly/install/llvm/bin/clang-22+0x4e63c1b)
 #1 0x000055eed85ebe88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:846:8
 #2 0x000055eed85e3da0 llvm::sys::RunSignalHandlers() /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Signals.cpp:0:5
 #3 0x000055eed85ef0b7 SignalHandler(int, siginfo_t*, void*) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007f90b4604520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007f90b46589fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f90b46589fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f90b46589fc pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f90b4604476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f90b45ea7f3 abort ./stdlib/abort.c:81:7
#10 0x00007f90b45ea71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007f90b45fbe96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#12 0x000055eed6e600d9 llvm::BasicBlock::isInstrOrderValid() const /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/BasicBlock.h:744:12
#13 0x000055eed6e600d9 llvm::BasicBlock::validateInstrOrdering() const /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/BasicBlock.cpp:1113:8
#14 0x000055eed6e600d9 llvm::BasicBlock::invalidateOrders() /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/BasicBlock.h:749:5
#15 0x000055eed6e600d9 void llvm::invalidateParentIListOrdering<llvm::BasicBlock>(llvm::BasicBlock*) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/BasicBlock.cpp:136:7
#16 0x000055eed6e600d9 llvm::SymbolTableListTraits<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>::transferNodesFromList(llvm::SymbolTableListTraits<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>&, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/SymbolTableListTraitsImpl.h:93:3
#17 0x000055eed6e69792 llvm::iplist_impl<llvm::simple_ilist<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>, llvm::SymbolTableListTraits<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>>::transfer(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::iplist_impl<llvm::simple_ilist<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>, llvm::SymbolTableListTraits<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>>&, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/ilist.h:0:11
#18 0x000055eed6e69792 llvm::iplist_impl<llvm::simple_ilist<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>, llvm::SymbolTableListTraits<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>>::splice(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::iplist_impl<llvm::simple_ilist<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>, llvm::SymbolTableListTraits<llvm::Instruction, llvm::ilist_iterator_bits<true>, llvm::ilist_parent<llvm::BasicBlock>>>&, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/ilist.h:276:24
#19 0x000055eed6e69792 llvm::BasicBlock::splice(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::BasicBlock*, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/BasicBlock.cpp:1022:17
#20 0x000055eed6e67f21 llvm::BasicBlock::splitBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::Twine const&, bool) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/BasicBlock.cpp:574:8
#21 0x000055eed86bb8cb SplitBlockImpl(llvm::BasicBlock*, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::DomTreeUpdater*, llvm::DominatorTree*, llvm::LoopInfo*, llvm::MemorySSAUpdater*, llvm::Twine const&, bool) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Utils/BasicBlockUtils.cpp:0:26
#22 0x000055eed86bb222 llvm::SplitBlock(llvm::BasicBlock*, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, false>, llvm::DominatorTree*, llvm::LoopInfo*, llvm::MemorySSAUpdater*, llvm::Twine const&, bool) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Utils/BasicBlockUtils.cpp:1056:10
#23 0x000055eedcc4c5ff llvm::SplitBlock(llvm::BasicBlock*, llvm::Instruction*, llvm::DominatorTree*, llvm::LoopInfo*, llvm::MemorySSAUpdater*, llvm::Twine const&, bool) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Transforms/Utils/BasicBlockUtils.h:334:10
#24 0x000055eedcc4c5ff (anonymous namespace)::CHR::transformScopes((anonymous namespace)::CHRScope*, llvm::DenseSet<llvm::PHINode*, llvm::DenseMapInfo<llvm::PHINode*, void>>&) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Instrumentation/ControlHeightReduction.cpp:1731:7
#25 0x000055eedcc18c5c (anonymous namespace)::CHR::transformScopes(llvm::SmallVectorImpl<(anonymous namespace)::CHRScope*>&) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Instrumentation/ControlHeightReduction.cpp:0:5
#26 0x000055eedcc18c5c (anonymous namespace)::CHR::run() /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Instrumentation/ControlHeightReduction.cpp:2076:7
#27 0x000055eedcc18c5c llvm::ControlHeightReductionPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Transforms/Instrumentation/ControlHeightReduction.cpp:2114:48
#28 0x000055eedb536b62 llvm::detail::PassModel<llvm::Function, llvm::ControlHeightReductionPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#29 0x000055eed72f53f8 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:8
#30 0x000055eed0823f82 llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#31 0x000055eed7301882 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/PassManager.cpp:132:23
#32 0x000055eed0827592 llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerInternal.h:91:5
#33 0x000055eed72f1ee8 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/PassManagerImpl.h:80:8
#34 0x000055eed8c54373 llvm::SmallPtrSetImplBase::isSmall() const /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:248:33
#35 0x000055eed8c54373 llvm::SmallPtrSetImplBase::~SmallPtrSetImplBase() /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/SmallPtrSet.h:89:10
#36 0x000055eed8c54373 llvm::PreservedAnalyses::~PreservedAnalyses() /home/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/IR/Analysis.h:112:7
#37 0x000055eed8c54373 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1209:5
#38 0x000055eed8c36bf4 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:0:0
#39 0x000055eed8c36bf4 (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1301:3
#40 0x000055eed8c36bf4 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1473:13
#41 0x000055eed9cfb2d9 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:310:3
#42 0x000055eede8db796 clang::ParseAST(clang::Sema&, bool, bool) /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Parse/ParseAST.cpp:0:13
#43 0x000055eed9d0b6b1 clang::CodeGenAction::ExecuteAction() /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:0:30
#44 0x000055eeda5cfa05 clang::FrontendAction::Execute() /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:10
#45 0x000055eeda3bb436 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:0:33
#46 0x000055eeda916108 std::__shared_ptr<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2>::get() const /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/shared_ptr_base.h:1666:16
#47 0x000055eeda916108 std::__shared_ptr_access<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/shared_ptr_base.h:1363:66
#48 0x000055eeda916108 std::__shared_ptr_access<clang::CompilerInvocation, (__gnu_cxx::_Lock_policy)2, false, false>::operator->() const /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/shared_ptr_base.h:1357:9
#49 0x000055eeda916108 clang::CompilerInstance::getFrontendOpts() /home/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:301:12
#50 0x000055eeda916108 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/x27zhou/compiler-nightly/src/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:311:14
#51 0x000055eecfaa2e36 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/cc1_main.cpp:0:15
#52 0x000055eecfa98552 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /home/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:226:12
#53 0x000055eecfa95258 clang_main(int, char**, llvm::ToolContext const&) /home/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:0:12
#54 0x000055eecfac8b50 main /home/x27zhou/compiler-nightly/build/llvm/tools/clang/tools/driver/clang-driver.cpp:17:10
#55 0x00007f90b45ebd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#56 0x00007f90b45ebe40 call_init ./csu/../csu/libc-start.c:128:20
#57 0x00007f90b45ebe40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#58 0x000055eecf9d10e5 _start (/home/x27zhou/compiler-nightly/install/llvm/bin/clang-22+0x4e280e5)
clang-trunk: error: unable to execute command: Aborted (core dumped)
clang-trunk: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git a0e0775d91859d6ca32efc5fb8644b6281e98593)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/x27zhou/compiler-nightly/install/llvm/bin
Build config: +assertions, +asan
clang-trunk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-trunk: note: diagnostic msg: /tmp/small-649437.c
clang-trunk: note: diagnostic msg: /tmp/small-649437.sh
clang-trunk: note: diagnostic msg: 

********************
```

Generated by fuzzer.




## Comments

### Comment 1 - aviralgarg05

I would like to solve this issue, please assign 

---

### Comment 2 - Harikaraja

@EugeneZelenko I would like to work on it, could you please assign me?


---

### Comment 3 - EugeneZelenko

@Harikaraja: I think will be good idea to prepare draft pull request first.

---

### Comment 4 - Harikaraja

@zxt5 I’m not able to reproduce this issue on my end.

I tested using LLVM commit:
d023577ef98490a097108eacaecd2dd114fd7974 (main)

Steps followed:
1. Built clang from the above commit.
2.  Merged the profile using `llvm-profdata merge` to produce `test.profdata`.
3.  Compiled  with:
   ../bin/clang -O3 -fprofile-instr-use=test.profdata small.c

The compilation completes successfully without triggering the reported error or assertion.

Please let me know if there are additional flags required to reproduce this issue ?


---

### Comment 5 - zxt5

It still crashes on my end. I just tried on [b84ffe0](https://github.com/llvm/llvm-project/commit/b84ffe040babfd8541613e41dd394a1baa9404fb).

No extra step is needed. Just obtain `test.profdata` from `profile.txt` and use it to compile with PGO at -O3.


---

### Comment 6 - Harikaraja

@zxt5 can you try on this commit https://github.com/llvm/llvm-project/commit/d023577ef98490a097108eacaecd2dd114fd7974

May be sometimes subsequent commits can fix the error

---

### Comment 7 - boomanaiden154

This reproduces for me:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @main() !prof !29 {
entry:
  br i1 false, label %for.cond7.preheader.i.split.us.split.split.us, label %for.cond7.preheader.i.split.us.split.split

for.cond7.preheader.i.split.us.split.split.us:    ; preds = %entry
  %cond125.i.us.us.us174.us.us.us = select i1 false, i16 0, i16 0, !prof !30
  ret void

for.cond7.preheader.i.split.us.split.split:       ; preds = %entry
  %cond125.i.us.us = select i1 false, i16 0, i16 0, !prof !30
  br i1 false, label %cond.end102.i.us.us.1, label %cond.false101.i.us.us.1, !prof !31

cond.false101.i.us.us.1:                          ; preds = %for.cond7.preheader.i.split.us.split.split
  br label %cond.end102.i.us.us.1

cond.end102.i.us.us.1:                            ; preds = %cond.false101.i.us.us.1, %for.cond7.preheader.i.split.us.split.split
  %conv116.i.us.us.1 = sext i16 %cond125.i.us.us to i32
  %cmp118.i.us.us.1 = icmp slt i32 0, %conv116.i.us.us.1
  %cond125.i.us.us.1 = select i1 %cmp118.i.us.us.1, i16 0, i16 0, !prof !30
  ret void
}

!llvm.module.flags = !{!0}

!0 = !{i32 1, !"ProfileSummary", !1}
!1 = !{!2, !3, !4, !5, !6, !7, !8, !9, !10, !11}
!2 = !{!"ProfileFormat", !"InstrProf"}
!3 = !{!"TotalCount", i64 9279}
!4 = !{!"MaxCount", i64 4374}
!5 = !{!"MaxInternalCount", i64 4374}
!6 = !{!"MaxFunctionCount", i64 64}
!7 = !{!"NumCounts", i64 39}
!8 = !{!"NumFunctions", i64 2}
!9 = !{!"IsPartialProfile", i64 0}
!10 = !{!"PartialProfileRatio", double 0.000000e+00}
!11 = !{!"DetailedSummary", !12}
!12 = !{!13, !14, !15, !16, !17, !18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28}
!13 = !{i32 10000, i64 4374, i32 2}
!14 = !{i32 100000, i64 4374, i32 2}
!15 = !{i32 200000, i64 4374, i32 2}
!16 = !{i32 300000, i64 4374, i32 2}
!17 = !{i32 400000, i64 4374, i32 2}
!18 = !{i32 500000, i64 4374, i32 2}
!19 = !{i32 600000, i64 4374, i32 2}
!20 = !{i32 700000, i64 4374, i32 2}
!21 = !{i32 800000, i64 4374, i32 2}
!22 = !{i32 900000, i64 4374, i32 2}
!23 = !{i32 950000, i64 374, i32 3}
!24 = !{i32 990000, i64 64, i32 4}
!25 = !{i32 999000, i64 2, i32 25}
!26 = !{i32 999900, i64 1, i32 28}
!27 = !{i32 999990, i64 1, i32 28}
!28 = !{i32 999999, i64 1, i32 28}
!29 = !{!"function_entry_count", i64 64}
!30 = !{!"branch_weights", i32 1, i32 375}
!31 = !{!"branch_weights", i32 4375, i32 1}
```
with
```bash
opt -passes="require<profile-summary>,chr" -disable-output reduced.ll
```

is the reduced IR test case.

---

### Comment 8 - zxt5

> [@zxt5](https://github.com/zxt5) can you try on this commit [d023577](https://github.com/llvm/llvm-project/commit/d023577ef98490a097108eacaecd2dd114fd7974)

@Harikaraja I just tried and I can reproduce on this commit.

And I can also reproduce with the above IR test case. Can you reproduce with the IR?


---

### Comment 9 - Harikaraja

@zxt5  I am able to replicate the error with IR test case. 

Thanks.

> > [@zxt5](https://github.com/zxt5) can you try on this commit [d023577](https://github.com/llvm/llvm-project/commit/d023577ef98490a097108eacaecd2dd114fd7974)
> 
> [@Harikaraja](https://github.com/Harikaraja) I just tried and I can reproduce on this commit.
> 
> And I can also reproduce with the above IR test case. Can you reproduce with the IR?



---

