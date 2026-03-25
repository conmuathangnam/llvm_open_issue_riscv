# [DA] Crash in ScalarEvolution::getTruncateOrZeroExtend

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/159979
**Status:** Closed
**Labels:** crash, llvm:analysis
**Closed Date:** 2025-10-14T08:16:29Z

## Body

This input:

```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

define void @_Z1dbibPA2_i(i64 %conv, ptr %a) {
entry:
  %sub = add i64 %conv, 1
  br label %for.cond

for.cond:                                         ; preds = %for.cond, %entry
  %i.0 = phi i64 [ %add26, %for.cond ], [ 0, %entry ]
  %arrayidx12 = getelementptr i32, ptr %a, i64 %i.0
  %.pre.pre.pre = load i32, ptr %arrayidx12, align 4
  %add26 = add nsw i64 %sub, %i.0
  br label %for.cond
}
```

Compiled with: 

```
opt -disable-output "-passes=print<da>" -aa-pipeline=basic-aa
```

Results in

```
 UNREACHABLE executed at llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:412!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: bin/opt -disable-output -passes=print<da> -aa-pipeline=basic-aa reduced.ll
1.      Running pass "function(print<da>)" on module "reduced.ll"
2.      Running pass "print<da>" on function "_Z1dbibPA2_i"
 #0 0x0000aaaaaecdcf4c llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x0000aaaaaecdd46c PrintStackTraceSignalHandler(void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:917:1
 #2 0x0000aaaaaecdb504 llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x0000aaaaaecddc34 SignalHandler(int, siginfo_t*, void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x0000fffff7fb09d0 (linux-vdso.so.1+0x9d0)
 #5 0x0000fffff7a1f200 __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x0000fffff79da67c gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #7 0x0000fffff79c7130 abort ./stdlib/abort.c:81:7
 #8 0x0000aaaaaecd8f84 llvm::install_out_of_memory_new_handler() llvm-project/llvm/lib/Support/ErrorHandling.cpp:225:0
 #9 0x0000aaaaaf627654 llvm::SCEV::getType() const llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:414:3
#10 0x0000aaaaaf62904c llvm::ScalarEvolution::getTruncateOrZeroExtend(llvm::SCEV const*, llvm::Type*, unsigned int) llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:4756:9
#11 0x0000aaaaaf628794 BinomialCoefficient(llvm::SCEV const*, unsigned int, llvm::ScalarEvolution&, llvm::Type*) llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:872:5
#12 0x0000aaaaaf628698 llvm::SCEVAddRecExpr::evaluateAtIteration(llvm::ArrayRef<llvm::SCEV const*>, llvm::SCEV const*, llvm::ScalarEvolution&) llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:994:25
#13 0x0000aaaaaf6285b4 llvm::SCEVAddRecExpr::evaluateAtIteration(llvm::SCEV const*, llvm::ScalarEvolution&) const llvm-project/llvm/lib/Analysis/ScalarEvolution.cpp:982:3
#14 0x0000aaaab039b164 llvm::DependenceInfo::isKnownLessThan(llvm::SCEV const*, llvm::SCEV const*) const llvm-project/llvm/lib/Analysis/DependenceAnalysis.cpp:1189:33
#15 0x0000aaaab03a7ce0 llvm::DependenceInfo::tryDelinearizeParametricSize(llvm::Instruction*, llvm::Instruction*, llvm::SCEV const*, llvm::SCEV const*, llvm::SmallVectorImpl<llvm::SCEV const*>&, llvm::SmallVectorImpl<llvm::SCEV const*>&) llvm-project/llvm/lib/Analysis/DependenceAnalysis.cpp:3639:18
#16 0x0000aaaab03a7050 llvm::DependenceInfo::tryDelinearize(llvm::Instruction*, llvm::Instruction*, llvm::SmallVectorImpl<llvm::DependenceInfo::Subscript>&) llvm-project/llvm/lib/Analysis/DependenceAnalysis.cpp:3457:7
#17 0x0000aaaab03a8dac llvm::DependenceInfo::depends(llvm::Instruction*, llvm::Instruction*, bool) llvm-project/llvm/lib/Analysis/DependenceAnalysis.cpp:3819:9
#18 0x0000aaaab039773c dumpExampleDependence(llvm::raw_ostream&, llvm::DependenceInfo*, llvm::ScalarEvolution&, bool) llvm-project/llvm/lib/Analysis/DependenceAnalysis.cpp:195:28
```


## Comments

### Comment 1 - sjoerdmeijer

CC: @nikic , @kasuga-fj 

---

### Comment 2 - kasuga-fj

Seems the issue in DA. I think removing the following part and insert appropriate early exit could fix the problem. 

https://github.com/llvm/llvm-project/blob/094d313fe4f949351670449172a43a345694ede0/llvm/lib/Analysis/DependenceAnalysis.cpp#L1174-L1181

(Calling `getTruncateOrZeroExtend` here seems incorrect to begin with, as it may alter the value...)

---

### Comment 3 - sjoerdmeijer

Thanks for the analysis. I was going to ask if you would like to pick this up, or if I shall do that. But given your RFC about NewDA, my guess is that I can have a look, is that right? :-) 

---

### Comment 4 - kasuga-fj

Yes, at least for a while, I'm planning to step back from DA development. Feel free to send me review requests, though.

---

### Comment 5 - sjoerdmeijer

Ok, thanks, will do.

---

### Comment 6 - sjoerdmeijer

The C reproducer in the original description, isn't triggering the issue anymore, something is behaving differently, but this IR reproducer is triggering the same issue: https://godbolt.org/z/4abqrW1Wz

I will fix the description.

---

