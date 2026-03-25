# [IROutliner] Assertion `OrigLoopID && OrigLoopID->getOperand(0).get() == OrigLoopID && "Loop ID should refer to itself"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/168265
**Status:** Open
**Labels:** crash

## Body

Reproducer:
https://godbolt.org/z/49n3KPWx1

Backtrace:
```console
opt: /root/llvm-project/llvm/lib/IR/DebugInfo.cpp:411: llvm::MDNode* updateLoopMetadataDebugLocationsImpl(llvm::MDNode*, llvm::function_ref<llvm::Metadata*(llvm::Metadata*)>): Assertion `OrigLoopID && OrigLoopID->getOperand(0).get() == OrigLoopID && "Loop ID should refer to itself"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=iroutliner -ir-outlining-no-cost <source>
1.	Running pass "iroutliner" on module "<source>"
 #0 0x00000000059a4608 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59a4608)
 #1 0x00000000059a14b4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000799e83242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000799e832969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000799e83242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000799e832287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000799e8322871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000799e83239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000005642fbe (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5642fbe)
 #9 0x0000000005642ff1 llvm::updateLoopMetadataDebugLocations(llvm::Instruction&, llvm::function_ref<llvm::Metadata* (llvm::Metadata*)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5642ff1)
#10 0x0000000004ac3661 fixupDebugInfoPostExtraction(llvm::Function&, llvm::Function&, llvm::CallInst&, llvm::SetVector<llvm::Value*, llvm::SmallVector<llvm::Value*, 0u>, llvm::DenseSet<llvm::Value*, llvm::DenseMapInfo<llvm::Value*, void>>, 0u> const&, llvm::ArrayRef<llvm::Value*>) CodeExtractor.cpp:0:0
#11 0x0000000004acbed1 llvm::CodeExtractor::extractCodeRegion(llvm::CodeExtractorAnalysisCache const&, llvm::SetVector<llvm::Value*, llvm::SmallVector<llvm::Value*, 0u>, llvm::DenseSet<llvm::Value*, llvm::DenseMapInfo<llvm::Value*, void>>, 0u>&, llvm::SetVector<llvm::Value*, llvm::SmallVector<llvm::Value*, 0u>, llvm::DenseSet<llvm::Value*, llvm::DenseMapInfo<llvm::Value*, void>>, 0u>&) (.part.0) CodeExtractor.cpp:0:0
#12 0x000000000326a9c3 llvm::IROutliner::extractSection(llvm::OutlinableRegion&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x326a9c3)
#13 0x000000000327c850 llvm::IROutliner::doOutline(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x327c850)
#14 0x000000000327dc17 llvm::IROutlinerPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x327dc17)
#15 0x0000000002fc63ae llvm::detail::PassModel<llvm::Module, llvm::IROutlinerPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fc63ae)
#16 0x0000000005782091 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5782091)
#17 0x0000000000984dba llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::RTLIB::RuntimeLibcallsInfo&, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x984dba)
#18 0x0000000000978cfd optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x978cfd)
#19 0x0000799e83229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x0000799e83229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x000000000096ff45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96ff45)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

This comes from llvm/test/Transforms/IndVarSimplify/X86/indvar-debug-value.ll

---

