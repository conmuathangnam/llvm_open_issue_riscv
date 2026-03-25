# [InstSimplify] crash on no_fold_fcmp_denormal_double_ieee_dynamic_scalable_vector_splat in llvm/test/Transforms/SCCP/no-fold-fcmp-dynamic-denormal-mode-issue114947.ll

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/187928
**Status:** Open
**Labels:** crash, llvm:instcombine

## Body

https://godbolt.org/z/anfEbo5dG

Backtrace:
```console
Invalid floating type
UNREACHABLE executed at /root/llvm-project/llvm/lib/IR/Type.cpp:119!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=instsimplify <source>
1.	Running pass "function(instsimplify)" on module "<source>"
2.	Running pass "instsimplify" on function "no_fold_fcmp_denormal_double_ieee_dynamic_scalable_vector_splat"
 #0 0x0000000005ba0f88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5ba0f88)
 #1 0x0000000005b9ddb4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5b9ddb4)
 #2 0x0000000005b9df2a SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007e1ccbe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e1ccbe969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e1ccbe42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e1ccbe287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000005acfb8a (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5acfb8a)
 #8 0x00000000059283c1 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59283c1)
 #9 0x0000000004f1c497 llvm::FlushFPConstant(llvm::Constant*, llvm::Instruction const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f1c497)
#10 0x0000000004f1fa3a llvm::ConstantFoldCompareInstOperands(unsigned int, llvm::Constant*, llvm::Constant*, llvm::DataLayout const&, llvm::TargetLibraryInfo const*, llvm::Instruction const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x4f1fa3a)
#11 0x0000000005016463 simplifyFCmpInst(llvm::CmpPredicate, llvm::Value*, llvm::Value*, llvm::FastMathFlags, llvm::SimplifyQuery const&, unsigned int) InstructionSimplify.cpp:0:0
#12 0x0000000005026b8b simplifyInstructionWithOperands(llvm::Instruction*, llvm::ArrayRef<llvm::Value*>, llvm::SimplifyQuery const&, unsigned int) InstructionSimplify.cpp:0:0
#13 0x00000000050277fb llvm::simplifyInstruction(llvm::Instruction*, llvm::SimplifyQuery const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x50277fb)
#14 0x000000000480ddb1 runImpl(llvm::Function&, llvm::SimplifyQuery const&) (.constprop.0) InstSimplifyPass.cpp:0:0
#15 0x000000000480e658 llvm::InstSimplifyPass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x480e658)
#16 0x0000000003140e9e llvm::detail::PassModel<llvm::Function, llvm::InstSimplifyPass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3140e9e)
#17 0x0000000005907e41 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5907e41)
#18 0x0000000000f1e74e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xf1e74e)
#19 0x00000000059063fa llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x59063fa)
#20 0x000000000096281e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96281e)
#21 0x0000000005905db1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5905db1)
#22 0x000000000096c9da llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96c9da)
#23 0x0000000000960ab2 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x960ab2)
#24 0x00007e1ccbe29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x00007e1ccbe29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000957695 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x957695)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - nikic

Works with `-use-constant-fp-for-scalable-splat=0` cc @paulwalker-arm 

---

