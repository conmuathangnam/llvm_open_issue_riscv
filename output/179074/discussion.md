# Loop vectorizer crash in LLVM 22

**Author:** Noratrieb
**URL:** https://github.com/llvm/llvm-project/issues/179074
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2026-02-05T07:50:58Z

## Body

This code crashes in the loop vectorizer (`opt -passes=loop-vectorize`).

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @reproducer() {
entry:
  br label %head

head:                                             ; preds = %tail, %entry
  %phi = phi ptr [ null, %entry ], [ %getelementptr, %tail ]
  %getelementptr = getelementptr i8, ptr %phi, i64 12
  br i1 false, label %tail, label %forward

forward:                                          ; preds = %head
  switch i32 0, label %tail [
    i32 0, label %tail
  ]

tail:                                             ; preds = %forward, %forward, %head
  %phi5 = phi i32 [ 1, %forward ], [ 1, %forward ], [ 0, %head ]
  %icmp = icmp eq ptr %getelementptr, null
  br i1 %icmp, label %exit, label %head

exit:                                             ; preds = %tail
  ret void
}
```

```
Stack dump:
0.      Program arguments: /home/nora/projects/rust/build/host/ci-llvm/bin/opt meta.ll -passes=loop-vectorize -disable-output
1.      Running pass "function(loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>)" on module "meta.ll"
2.      Running pass "loop-vectorize<no-interleave-forced-only;no-vectorize-forced-only;>" on function "reproducer"
 #0 0x00007f91e56c2008 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x58c2008)
 #1 0x00007f91e56bebe5 llvm::sys::RunSignalHandlers() (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x58bebe5)
 #2 0x00007f91e56c2746 SignalHandler(int, siginfo_t*, void*) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x58c2746)
 #3 0x00007f91dfa42790 __restore_rt (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x42790)
 #4 0x00007f91e7879f77 llvm::VPlanTransforms::introduceMasksAndLinearize(llvm::VPlan&, bool) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x7a79f77)
 #5 0x00007f91e7662efb llvm::LoopVectorizationPlanner::tryToBuildVPlanWithVPRecipes(std::unique_ptr<llvm::VPlan, std::default_delete<llvm::VPlan>>, llvm::VFRange&, llvm::LoopVersioning*) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x7862efb)
 #6 0x00007f91e7654643 llvm::LoopVectorizationPlanner::buildVPlansWithVPRecipes(llvm::ElementCount, llvm::ElementCount) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x7854643)
 #7 0x00007f91e7653ab6 llvm::LoopVectorizationPlanner::plan(llvm::ElementCount, unsigned int) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x7853ab6)
 #8 0x00007f91e766beac llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x786beac)
 #9 0x00007f91e7678f8b llvm::LoopVectorizePass::runImpl(llvm::Function&) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x7878f8b)
#10 0x00007f91e76798c2 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x78798c2)
#11 0x00007f91e592f22f llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x5b2f22f)
#12 0x00007f91e5934490 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x5b34490)
#13 0x00007f91e592de99 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/home/nora/projects/rust/build/x86_64-unknown-linux-gnu/ci-llvm/bin/../lib/libLLVM.so.22.1-rust-1.95.0-nightly+0x5b2de99)
#14 0x000055bb94c79523 llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/home/nora/projects/rust/build/host/ci-llvm/bin/opt+0x2a523)
#15 0x000055bb94c6b6ca optMain (/home/nora/projects/rust/build/host/ci-llvm/bin/opt+0x1c6ca)
#16 0x00007f91dfa2b285 __libc_start_call_main (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x2b285)
#17 0x00007f91dfa2b338 __libc_start_main@GLIBC_2.2.5 (/nix/store/wb6rhpznjfczwlwx23zmdrrw74bayxw4-glibc-2.42-47/lib/libc.so.6+0x2b338)
#18 0x000055bb94c67e49 _start (/home/nora/projects/rust/build/host/ci-llvm/bin/opt+0x18e49)
```


## Comments

### Comment 1 - Noratrieb

I've been trying to bisect it and it looks like this is not actually an LLVM 22 regression for the minimal reproducer, I've been able to reproduce it in LLVM 21 and LLVM 20. It looks like change in LLVM 22 that triggered the Rust reproducer (https://github.com/rust-lang/rust/issues/151926) was just perturbing something to reveal this existing bug.

---

### Comment 2 - Noratrieb

296781524902c4eb06efc5bb054cac58b973d839 looks relevant but it didn't cause this issue, it merely turned an assertion failure into the segfault (when I revert it it triggers the `Distinct incoming values with one having a full mask` assertion)

---

