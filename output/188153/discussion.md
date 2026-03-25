# [CodeGenPrepare] Crash at -O1 when compiling with profile data using PGO: Assertion `BB && "dominator tree block must be non-null"' failed.

**Author:** zxt5
**URL:** https://github.com/llvm/llvm-project/issues/188153
**Status:** Open
**Labels:** llvm:codegen, PGO, crash

## Body

small.c
```c
int a, b, e, l, ak;
int *c, *f;
long d;
int g() {
  for (; e;)
    while (b++)
      for (; d; d++)
        for (int h; h; h++)
          for (int i; i; i += a)
            for (int j; j; j++) {
              *c = f[a];
              if (b)
                return *(int *)sizeof(int);
            }
}
void k(int m, int n, long r) {
  int o = sizeof(int);
  int *p = (int *)((char *)r + m + n);
  p[o] = 2;
}
int ad(int m) {
  long ac = m + m;
  g();
  k(ac, ac, ac + 5);
}
void ai(short m) {
  if ((short)ad(m + 3) + m)
    l = 0;
}
int f1() {
  ai(ak + 2);
  for (; a < 1;)
    ;
  return 0;
}
int main() {}
```

profile.txt
```
f1
1160280
2
1
0
```

Clang crashes when compiling the above code at -O1 with profile data using PGO.

Generate the profile data with:

```
$ llvm-profdata merge -o test.profdata profile.txt
```

Compile at -O1 with PGO:

```
$ clang-trunk -O1 -fprofile-instr-use=test.profdata small.c 
```

Crash:

```                                                   
clang-23: /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/GenericDomTree.h:394: std::optional<unsigned int> llvm::DominatorTreeBase<llvm::BasicBlock, false>::getNodeIndex(const NodeT *) const [NodeT = llvm::BasicBlock, IsPostDom = false]: Assertion `BB && "dominator tree block must be non-null"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /data/x27zhou/compiler-nightly/install/llvm/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -O1 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name small.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/x27zhou/fuzz-pgo/PGOFuzz/triage/crash-clang-trunk-152370/reduction -fprofile-instrument-use=clang -fprofile-instrument-use-path=test.profdata -fcoverage-compilation-dir=/home/x27zhou/fuzz-pgo/PGOFuzz/triage/crash-clang-trunk-152370/reduction -resource-dir /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23 -internal-isystem /data/x27zhou/compiler-nightly/install/llvm/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=201 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/small-59e6cc.o -x c small.c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'small.c'.
4.      Running pass 'CodeGen Prepare' on function '@f1'
 #0 0x0000556b6ae5d367 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:13
 #1 0x0000556b6ae5b00a llvm::sys::RunSignalHandlers() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #2 0x0000556b6ae5e205 SignalHandler(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:448:38
 #3 0x0000556b6ae5e068 int const* std::__find_if<int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>>(int const*, int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>, std::random_access_iterator_tag) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_algobase.h:2067:8
 #4 0x0000556b6ae5e068 int const* std::__find_if<int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>>(int const*, int const*, __gnu_cxx::__ops::_Iter_equals_val<int const>) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_algobase.h:2112:14
 #5 0x0000556b6ae5e068 int const* std::find<int const*, int>(int const*, int const*, int const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_algo.h:3851:14
 #6 0x0000556b6ae5e068 bool llvm::is_contained<int const (&) [10], int>(int const (&) [10], int const&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/ADT/STLExtras.h:1953:12
 #7 0x0000556b6ae5e068 SignalHandlerTerminate(int, siginfo_t*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/Support/Unix/Signals.inc:464:7
 #8 0x00007fc781d64520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #9 0x00007fc781db89fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
#10 0x00007fc781db89fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
#11 0x00007fc781db89fc pthread_kill ./nptl/pthread_kill.c:89:10
#12 0x00007fc781d64476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
#13 0x00007fc781d4a7f3 abort ./stdlib/abort.c:81:7
#14 0x00007fc781d4a71b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#15 0x00007fc781d5be96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#16 0x0000556b6a7e9ce8 llvm::DominatorTreeBase<llvm::BasicBlock, false>::getNodeIndex(llvm::BasicBlock const*) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/GenericDomTree.h:387:7
#17 0x0000556b6a7e9ce8 llvm::DominatorTreeBase<llvm::BasicBlock, false>::getNode(llvm::BasicBlock const*) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/GenericDomTree.h:430:20
#18 0x0000556b6a7e9ce8 llvm::DominatorTreeBase<llvm::BasicBlock, false>::isReachableFromEntry(llvm::BasicBlock const*) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/GenericDomTree.h:485:33
#19 0x0000556b6a7f3218 llvm::DominatorTree::dominates(llvm::Value const*, llvm::Instruction const*) const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/Dominators.cpp:153:8
#20 0x0000556b6a2206ff (anonymous namespace)::CodeGenPrepare::optimizeMemoryInst(llvm::Instruction*, llvm::Value*, llvm::Type*, unsigned int) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/CodeGenPrepare.cpp:5955:76
#21 0x0000556b6a218e44 (anonymous namespace)::CodeGenPrepare::optimizeInst(llvm::Instruction*, (anonymous namespace)::ModifyDT&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/CodeGenPrepare.cpp:0:0
#22 0x0000556b6a20ff0c (anonymous namespace)::CodeGenPrepare::optimizeBlock(llvm::BasicBlock&, (anonymous namespace)::ModifyDT&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/CodeGenPrepare.cpp:9094:18
#23 0x0000556b6a20ff0c (anonymous namespace)::CodeGenPrepare::_run(llvm::Function&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/CodeGenPrepare.cpp:657:22
#24 0x0000556b6a20e5c9 (anonymous namespace)::CodeGenPrepareLegacyPass::runOnFunction(llvm::Function&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/CodeGen/CodeGenPrepare.cpp:528:14
#25 0x0000556b6a864435 llvm::FPPassManager::runOnFunction(llvm::Function&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:27
#26 0x0000556b6a86a591 llvm::FPPassManager::runOnModule(llvm::Module&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:13
#27 0x0000556b6a864ad4 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:27
#28 0x0000556b6a864ad4 llvm::legacy::PassManagerImpl::run(llvm::Module&) /data/x27zhou/compiler-nightly/src/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:44
#29 0x0000556b6b1a643b (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1278:9
#30 0x0000556b6b1a643b (anonymous namespace)::EmitAssemblyHelper::emitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1301:3
#31 0x0000556b6b1a643b clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1475:13
#32 0x0000556b6b713be1 std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/unique_ptr.h:395:6
#33 0x0000556b6b713be1 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:312:3
#34 0x0000556b6cc589a7 __gnu_cxx::__normal_iterator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>*, std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>::__normal_iterator(std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>* const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_iterator.h:1073:20
#35 0x0000556b6cc589a7 std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>::begin() /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/bits/stl_vector.h:869:16
#36 0x0000556b6cc589a7 void clang::finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>(std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>&, clang::Sema const&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/include/clang/Sema/TemplateInstCallback.h:54:16
#37 0x0000556b6cc589a7 clang::ParseAST(clang::Sema&, bool, bool) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Parse/ParseAST.cpp:190:3
#38 0x0000556b6b99d8e6 clang::FrontendAction::Execute() /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1330:10
#39 0x0000556b6b90d8af llvm::Error::getPtr() const /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:278:42
#40 0x0000556b6b90d8af llvm::Error::operator bool() /data/x27zhou/compiler-nightly/src/llvm-project/llvm/include/llvm/Support/Error.h:241:16
#41 0x0000556b6b90d8af clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:989:23
#42 0x0000556b6ba943f5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:318:25
#43 0x0000556b6835b578 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/cc1_main.cpp:302:15
#44 0x0000556b68357a60 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:229:12
#45 0x0000556b68356b5c clang_main(int, char**, llvm::ToolContext const&) /data/x27zhou/compiler-nightly/src/llvm-project/clang/tools/driver/driver.cpp:277:12
#46 0x0000556b68366a3d main /data/x27zhou/compiler-nightly/build/llvm/tools/clang/tools/driver/clang-driver.cpp:17:10
#47 0x00007fc781d4bd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#48 0x00007fc781d4be40 call_init ./csu/../csu/libc-start.c:128:20
#49 0x00007fc781d4be40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#50 0x0000556b68355325 _start (/data/x27zhou/compiler-nightly/install/llvm/bin/clang-23+0xf6c325)
clang-trunk: error: unable to execute command: Aborted (core dumped)
clang-trunk: error: clang frontend command failed with exit code -2 (use -v to see invocation)
clang version 23.0.0git (https://github.com/llvm/llvm-project.git 252eb2a743f56f1f34048d8c311bad6037052232)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/x27zhou/compiler-nightly/install/llvm/bin
Build config: +assertions
clang-trunk: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-trunk: note: diagnostic msg: /tmp/small-ecab83.c
clang-trunk: note: diagnostic msg: /tmp/small-ecab83.sh
clang-trunk: note: diagnostic msg: 

********************
```




