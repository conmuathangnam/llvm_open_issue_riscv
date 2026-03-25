# [LV] "argument of incompatible type" failed

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/160069
**Status:** Closed
**Labels:** duplicate, vectorizers, crash
**Closed Date:** 2025-09-22T10:58:02Z

## Body

This input:

```
#include <algorithm>
extern char a, c;
long e, b;
short test_arr_5, f;
int *g;
void h(int i[], unsigned arr_14[][9]) {
  for (int j;;)
    for (bool d;;)
#pragma clang loop vectorize_predicate(enable)
      for (char k(0); k < (char)b + 4; k += 2) {
        for (int l = 2; l < 17; l += 3LL) {
          a = 0;
          e -= 5 / g[j];
        }
        c *= f;
        for (short i_4; std::max((unsigned)i[k], arr_14[j][d]);)
          ;
      }
}
```

runs into this assert:

```
clang++: llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From *) [To = llvm::InsertElementInst, From = llvm::Value]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
```

The back trace shows:

```
 #0 0x0000aaaab3bb9670 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x0000aaaab3bb9b90 PrintStackTraceSignalHandler(void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:917:1
 #2 0x0000aaaab3bb7ce8 llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x0000aaaab3bb8f10 llvm::sys::CleanupOnSignal(unsigned long) llvm-project/llvm/lib/Support/Unix/Signals.inc:374:1
 #4 0x0000aaaab3adbd40 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:71:7
 #5 0x0000aaaab3adc174 CrashRecoverySignalHandler(int) llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:5
 #6 0x0000fffff7fb09d0 (linux-vdso.so.1+0x9d0)
 #7 0x0000fffff7a1f200 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #8 0x0000fffff79da67c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x0000fffff79c7130 abort ./stdlib/abort.c:81:7
#10 0x0000fffff79d3fd0 __assert_fail_base ./assert/assert.c:89:7
#11 0x0000fffff79d4040 __assert_perror_fail ./assert/assert-perr.c:31:1
#12 0x0000aaaab5f8d308 decltype(auto) llvm::cast<llvm::InsertElementInst, llvm::Value>(llvm::Value*) llvm-project/llvm/include/llvm/Support/Casting.h:579:10
#13 0x0000aaaab5f82c2c llvm::VPPredInstPHIRecipe::execute(llvm::VPTransformState&) llvm-project/llvm/lib/Transforms/Vectorize/VPlanRecipes.cpp:3277:24
#14 0x0000aaaab5f3839c llvm::VPBasicBlock::executeRecipes(llvm::VPTransformState*, llvm::BasicBlock*) llvm-project/llvm/lib/Transforms/Vectorize/VPlan.cpp:583:3
#15 0x0000aaaab5f386d8 llvm::VPBasicBlock::execute(llvm::VPTransformState*) llvm-project/llvm/lib/Transforms/Vectorize/VPlan.cpp:563:3
#16 0x0000aaaab5f39db0 llvm::VPRegionBlock::execute(llvm::VPTransformState*) llvm-project/llvm/lib/Transforms/Vectorize/VPlan.cpp:790:5
#17 0x0000aaaab5f3b3b0 llvm::VPlan::execute(llvm::VPTransformState*) llvm-project/llvm/lib/Transforms/Vectorize/VPlan.cpp:995:3
#18 0x0000aaaab5eacaac llvm::LoopVectorizationPlanner::executePlan(llvm::ElementCount, unsigned int, llvm::VPlan&, llvm::InnerLoopVectorizer&, llvm::DominatorTree*, bool) llvm-project/llvm/lib/Transforms/
Vectorize/LoopVectorize.cpp:7303:13
#19 0x0000aaaab5eb8190 llvm::LoopVectorizePass::processLoop(llvm::Loop*) llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10225:9
#20 0x0000aaaab5ebb5ac llvm::LoopVectorizePass::runImpl(llvm::Function&) llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10278:30
#21 0x0000aaaab5ebb934 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) llvm-project/llvm/lib/Transforms/Vectorize/LoopVectorize.cpp:10316:32
#22 0x0000aaaab5d12f34 llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) llvm-project/ll
vm/include/llvm/IR/PassManagerInternal.h:91:5
```

I can't reproduce this with compiler explorer: https://godbolt.org/z/j9bc8K9We
Maybe that's related to their environment, it just fails with:

```
Killed - processing time exceeded
Program terminated with signal: SIGKILL
Compiler returned: 143
``` 

## Comments

### Comment 1 - sjoerdmeijer

FYI: @fhahn 

---

### Comment 2 - fhahn

Probably a duplicate of https://github.com/llvm/llvm-project/issues/157314

---

### Comment 3 - sjoerdmeijer

Ah thanks, sorry, I missed that one, overlooked it. But I agree, looks the same thing. 
I will close this, and will keep an eye on #157314.

---

