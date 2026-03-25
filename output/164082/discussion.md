# crashes with assertion failure when running opt -passes=loop-fusion

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/164082
**Status:** Closed
**Labels:** crash, llvm:transforms
**Closed Date:** 2025-11-04T22:12:34Z

## Body

Reproducer:
https://godbolt.org/z/qzjrq4P3z

Backtrace:
```console
opt: /root/llvm-project/llvm/include/llvm/Support/GenericLoopInfo.h:135: bool llvm::LoopBase<N, M>::contains(const BlockT*) const [with BlockT = llvm::BasicBlock; LoopT = llvm::Loop]: Assertion `!isInvalid() && "Loop not in a valid state!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-fusion <source>
1.	Running pass "function(loop-fusion)" on module "<source>"
2.	Running pass "loop-fusion" on function "main"
 #0 0x0000000005954738 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5954738)
 #1 0x00000000059515e4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000071f9da242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000071f9da2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000071f9da242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000071f9da2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000071f9da22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000071f9da239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000004f4c6b3 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f4c6b3)
 #9 0x0000000004f6c4e9 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f6c4e9)
#10 0x0000000004f6becf llvm::ScalarEvolution::getLoopDisposition(llvm::SCEV const*, llvm::Loop const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f6becf)
#11 0x0000000004fbe4a4 llvm::ScalarEvolution::verify() const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4fbe4a4)
#12 0x0000000004673741 (anonymous namespace)::LoopFuser::performFusion((anonymous namespace)::FusionCandidate const&, (anonymous namespace)::FusionCandidate const&) LoopFuse.cpp:0:0
#13 0x00000000046789ae (anonymous namespace)::LoopFuser::fuseCandidates() LoopFuse.cpp:0:0
#14 0x000000000467a501 llvm::LoopFusePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x467a501)
#15 0x0000000002f8e9de llvm::detail::PassModel<llvm::Function, llvm::LoopFusePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2f8e9de)
#16 0x000000000572e961 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x572e961)
#17 0x0000000000ef661e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xef661e)
#18 0x000000000572ceca llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x572ceca)
#19 0x0000000000977e0e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x977e0e)
#20 0x000000000572c881 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x572c881)
#21 0x0000000000981f1a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x981f1a)
#22 0x000000000097619e optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97619e)
#23 0x000071f9da229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#24 0x000071f9da229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#25 0x000000000096d4f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96d4f5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Comes from llvm/test/CodeGen/PowerPC/ctrloop-sums.ll

---

### Comment 2 - dtcxzyw

Bisected to https://github.com/llvm/llvm-project/commit/d6072986cd40a3c009d2963a2b33501e047c7dc4
cc @1997alireza @CongzheUalberta

---

### Comment 3 - CongzheUalberta

Can you take a look? @1997alireza 

---

### Comment 4 - 1997alireza

> Can you take a look? [@1997alireza](https://github.com/1997alireza)

Yes, working on it.

---

### Comment 5 - 1997alireza

> > Can you take a look? [@1997alireza](https://github.com/1997alireza)
> 
> Yes, working on it.

Found the solution. Will post a patch to fix it asap.


---

### Comment 6 - CongzheUalberta

Should be fixed with #166233 so closing the issue now.
If something related happens down the road we can reopen it.

---

