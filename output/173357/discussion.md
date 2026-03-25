# opt crashes in llvm/lib/IR/Value.cpp:102

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/173357
**Status:** Closed
**Labels:** loopoptim, crash
**Closed Date:** 2025-12-30T17:00:10Z

## Body

When running opt on the following IR, opt crashes. 
I am not sure whether this is a valid case. If using opt in this way is not appropriate, I am open to closing this issue.
godbolt: https://godbolt.org/z/WP538Wx4z

llvm version 50ae726bb3498

test.ll

```
define void @foo(ptr writeonly captures(none) initializes((0, 4)) %P) #0 {
bb:
  %p.promoted2 = load ptr, ptr %P, align 8
  br label %bb1

bb1:                                              ; preds = %bb1.backedge, %bb
  %p.ph = phi ptr [ %P, %bb ], [ %p.promoted2, %bb1.backedge ]
  br label %bb1.backedge

bb1.backedge:                                     ; preds = %bb1, %merge
  br label %bb1

merge:                                            ; No predecessors!
  store ptr %p.ph, ptr %P, align 8
  br label %bb1.backedge
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) }

```


commads
./bin/opt test.ll -passes=loop-deletion -S

stacktrace

```
While deleting: ptr %p.ph
Use still stuck around after Def is destroyed:  store ptr %p.ph, ptr %P, align 8
Uses remain when a value is destroyed!
UNREACHABLE executed at /root/llvm-project/llvm/lib/IR/Value.cpp:102!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=loop-deletion -S <source>
1.	Running pass "function(loop(loop-deletion))" on module "<source>"
2.	Running pass "loop(loop-deletion)" on function "foo"
 #0 0x0000000005a44d98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a44d98)
 #1 0x0000000005a41c44 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007dc51e242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007dc51e2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007dc51e242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007dc51e2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000000000597959a (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x597959a)
 #7 0x0000000005859a21 llvm::Value::~Value() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5859a21)
 #8 0x0000000005859bba llvm::Value::deleteValue() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5859bba)
 #9 0x0000000005655e5e llvm::BasicBlock::~BasicBlock() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5655e5e)
#10 0x00000000056560f5 llvm::BasicBlock::eraseFromParent() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x56560f5)
#11 0x0000000004c3d859 llvm::deleteDeadLoop(llvm::Loop*, llvm::DominatorTree*, llvm::ScalarEvolution*, llvm::LoopInfo*, llvm::MemorySSA*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4c3d859)
#12 0x00000000047321f7 llvm::LoopDeletionPass::run(llvm::Loop&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x47321f7)
#13 0x0000000003020c4e llvm::detail::PassModel<llvm::Loop, llvm::LoopDeletionPass, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>::run(llvm::Loop&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3020c4e)
#14 0x00000000047894d6 std::optional<llvm::PreservedAnalyses> llvm::PassManager<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>::runSinglePass<llvm::Loop, std::unique_ptr<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>, std::default_delete<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>>>>(llvm::Loop&, std::unique_ptr<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>, std::default_delete<llvm::detail::PassConcept<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>>>&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&, llvm::PassInstrumentation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x47894d6)
#15 0x00000000047896bf llvm::PassManager<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>::runWithoutLoopNestPasses(llvm::Loop&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x47896bf)
#16 0x000000000478ae24 llvm::PassManager<llvm::Loop, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&>::run(llvm::Loop&, llvm::AnalysisManager<llvm::Loop, llvm::LoopStandardAnalysisResults&>&, llvm::LoopStandardAnalysisResults&, llvm::LPMUpdater&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x478ae24)
#17 0x000000000478bdff llvm::FunctionToLoopPassAdaptor::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x478bdff)
#18 0x0000000000f1874e llvm::detail::PassModel<llvm::Function, llvm::FunctionToLoopPassAdaptor, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf1874e)
#19 0x0000000005821031 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5821031)
#20 0x0000000000f1952e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf1952e)
#21 0x000000000581f6fa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x581f6fa)
#22 0x000000000097d43e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97d43e)
#23 0x000000000581f0b1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x581f0b1)
#24 0x000000000098776a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98776a)
#25 0x000000000097b5e8 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97b5e8)
#26 0x00007dc51e229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#27 0x00007dc51e229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#28 0x0000000000972085 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x972085)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


