# [DSE] Assertion failure with clang using sized dead_on_return

**Author:** boomanaiden154
**URL:** https://github.com/llvm/llvm-project/issues/180361
**Status:** Closed
**Labels:** crash, llvm:transforms
**Closed Date:** 2026-02-07T19:43:58Z

## Body

We hit an assertion failure in DSE from the following minimized test case:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #0

define void @_ZN4llvm19LegacyLegalizerInfoD2Ev(ptr dead_on_return(90056) %this, i64 %arraydestroy.elementPast.add) {
entry:
  %arraydestroy.element.ptr = getelementptr i8, ptr %this, i64 %arraydestroy.elementPast.add
  tail call void @llvm.memset.p0.i64(ptr %arraydestroy.element.ptr, i8 0, i64 0, i1 false)
  ret void
}

attributes #0 = { nocallback nofree nounwind willreturn memory(argmem: write) }
```
with the following command line:
```
opt -passes="dse" -disable-output <input path>
```

Stack trace looks like the following:
```
opt: /home/gha/llvm-project/llvm/lib/Transforms/Scalar/DeadStoreElimination.cpp:1230: bool (anonymous namespace)::DSEState::isInvisibleToCallerAfterRet(const Value *, const Value *, const LocationSize): Assertion `BaseValue == V' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./bin/opt -passes=dse ./reduced.ll
1.      Running pass "function(dse)" on module "./reduced.ll"
2.      Running pass "dse" on function "_ZN4llvm19LegacyLegalizerInfoD2Ev"
 #0 0x000055cf10cb8868 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (./bin/opt+0x4af8868)
 #1 0x000055cf10cb5fa5 llvm::sys::RunSignalHandlers() (./bin/opt+0x4af5fa5)
 #2 0x000055cf10cb9631 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f4e67768330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007f4e677c1b2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #5 0x00007f4e6776827e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #6 0x00007f4e6774b8ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #7 0x00007f4e6774b81b (/lib/x86_64-linux-gnu/libc.so.6+0x2881b)
 #8 0x00007f4e6775e517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
 #9 0x000055cf0fb4221a (anonymous namespace)::DSEState::isInvisibleToCallerAfterRet(llvm::Value const*, llvm::Value const*, llvm::LocationSize) DeadStoreElimination.cpp:0:0
#10 0x000055cf0fb3768c eliminateDeadStores(llvm::Function&, llvm::AAResults&, llvm::MemorySSA&, llvm::DominatorTree&, llvm::PostDominatorTree&, llvm::TargetLibraryInfo const&, llvm::LoopInfo const&) DeadStoreElimination.cpp:0:0
#11 0x000055cf0fb34215 llvm::DSEPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0x3974215)
#12 0x000055cf0e754bed llvm::detail::PassModel<llvm::Function, llvm::DSEPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0x2594bed)
#13 0x000055cf10a79c37 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0x48b9c37)
#14 0x000055cf0ce6ce2d llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (./bin/opt+0xcace2d)
#15 0x000055cf10a7dc71 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (./bin/opt+0x48bdc71)
#16 0x000055cf0c97cc0d llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (./bin/opt+0x7bcc0d)
#17 0x000055cf10a78d07 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (./bin/opt+0x48b8d07)
#18 0x000055cf0c974872 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (./bin/opt+0x7b4872)
#19 0x000055cf0c96770d optMain (./bin/opt+0x7a770d)
#20 0x00007f4e6774d1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#21 0x00007f4e6774d28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#22 0x000055cf0c960a05 _start (./bin/opt+0x7a0a05)
Aborted (core dumped)
```

