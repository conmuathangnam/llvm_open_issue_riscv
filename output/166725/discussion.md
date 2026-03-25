# [LoopVectorize][OptRemarks] Crash when combining loop vectorization with optimization remark output

**Author:** bozicrHT
**URL:** https://github.com/llvm/llvm-project/issues/166725
**Status:** Closed
**Labels:** duplicate, vectorizers, crash
**Closed Date:** 2026-02-09T17:54:18Z

## Body

**Description:**
Clang/opt crashes when applying the loop vectorization pass together with optimization remark output (`-opt-record-file` or `-pass-remarks-output`). The issue reproduces at all optimization levels (`-O1`, `-O2`, `-O3`, `-Os`, `-Oz`).

Reproducer:
The following reduced C code (generated via creduce) triggers the crash:
```c
typedef struct {
  int a;
} b;
void c() {
  void *d[] = {&&e, &&f};
  b var;
  b *g = &var;
  goto * 0;
e:
  while (1) {
    g--;
    if (g->a)
      break;
  }
f:;
}
```

**Steps to reproduce:**
Using clang:
```sh
clang -cc1 -O1 -vectorize-loops -opt-record-file tmp.yaml -emit-llvm test.c
```

Using opt directly:
```sh
clang -cc1 -disable-llvm-passes -O1 -emit-llvm test.c -o test.ll
opt -passes='default<O1>,loop-vectorize' -pass-remarks-output=rem.yaml -S test.ll
```

**Expected Behavior:**
Clang/opt should not crash; it should either safely skip vectorization or emit a valid diagnostic.

**Crash log:**
```
clang: /nobackup/rabozic/llvm-upstream/llvm/include/llvm/IR/Instructions.h:2817: llvm::Value* llvm::PHINode::getIncomingValueForBlock(const llvm::BasicBlock*) const: Assertion `Idx >= 0 && "Invalid basic block argument!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -O1 -vectorize-loops -opt-record-file tmp.yaml -emit-llvm test.c
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "function<eager-inv>(drop-unnecessary-assumes,float2int,lower-constant-intrinsics,loop(loop-rotate<header-duplication;no-prepare-for-lto>,loop-deletion),loop-distribute,inject-tli-mappings,loop-vectorize<interleave-forced-only;no-vectorize-forced-only;>,infer-alignment,loop-load-elim,instcombine<max-iterations=1;no-verify-fixpoint>,simplifycfg<bonus-inst-threshold=1;forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;switch-to-lookup;no-keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,vector-combine,instcombine<max-iterations=1;no-verify-fixpoint>,loop-unroll<O1>,transform-warning,sroa<preserve-cfg>,infer-alignment,instcombine<max-iterations=1;no-verify-fixpoint>,loop-mssa(licm<allowspeculation>),alignment-from-assumptions,loop-sink,instsimplify,div-rem-pairs,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;switch-to-arithmetic;no-switch-to-lookup;keep-loops;no-hoist-common-insts;hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;speculate-unpredictables>)" on module "test.c"
4.      Running pass "loop-vectorize<interleave-forced-only;no-vectorize-forced-only;>" on function "c"
 #0 0x0000000002105958 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x2105958)
 #1 0x00000000021027ac SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fc356a6ed20 __restore_rt (/lib64/libpthread.so.0+0x12d20)
 #3 0x00007fc3554dd52f raise (/lib64/libc.so.6+0x4e52f)
 #4 0x00007fc3554b0e65 abort (/lib64/libc.so.6+0x21e65)
 #5 0x00007fc3554b0d39 _nl_load_domain.cold.0 (/lib64/libc.so.6+0x21d39)
 #6 0x00007fc3554d5e86 (/lib64/libc.so.6+0x46e86)
 #7 0x0000000001220374 llvm::PHINode::getIncomingValueForBlock(llvm::BasicBlock const*) const (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x1220374)
 #8 0x000000000133d2b0 llvm::RecurrenceDescriptor::AddReductionVar(llvm::PHINode*, llvm::RecurKind, llvm::Loop*, llvm::FastMathFlags, llvm::RecurrenceDescriptor&, llvm::DemandedBits*, llvm::AssumptionCache*, llvm::DominatorTree*, llvm::ScalarEvolution*) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x133d2b0)
 #9 0x000000000133f628 llvm::RecurrenceDescriptor::isReductionPHI(llvm::PHINode*, llvm::Loop*, llvm::RecurrenceDescriptor&, llvm::DemandedBits*, llvm::AssumptionCache*, llvm::DominatorTree*, llvm::ScalarEvolution*) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x133f628)
#10 0x0000000003bb2af0 llvm::LoopVectorizationLegality::canVectorizeInstr(llvm::Instruction&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x3bb2af0)
#11 0x0000000003bb4156 llvm::LoopVectorizationLegality::canVectorizeInstrs() (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x3bb4156)
#12 0x0000000003bb6036 llvm::LoopVectorizationLegality::canVectorize(bool) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x3bb6036)
#13 0x00000000039d1503 llvm::LoopVectorizePass::processLoop(llvm::Loop*) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x39d1503)
#14 0x00000000039d6236 llvm::LoopVectorizePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x39d6236)
#15 0x0000000002fd9efd llvm::detail::PassModel<llvm::Function, llvm::LoopVectorizePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x2fd9efd)
#16 0x0000000001bd6e99 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x1bd6e99)
#17 0x0000000000d192fd llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0xd192fd)
#18 0x0000000001bd7803 llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x1bd7803)
#19 0x0000000000d19d7d llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0xd19d7d)
#20 0x0000000001bd8fe9 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x1bd8fe9)
#21 0x0000000002378fa7 (anonymous namespace)::EmitAssemblyHelper::RunOptimizationPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&, clang::BackendConsumer*) BackendUtil.cpp:0:0
#22 0x000000000237bee2 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x237bee2)
#23 0x000000000296e84f clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x296e84f)
#24 0x000000000424bdfc clang::ParseAST(clang::Sema&, bool, bool) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x424bdfc)
#25 0x000000000296f0b8 clang::CodeGenAction::ExecuteAction() (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x296f0b8)
#26 0x0000000002c2c7f3 clang::FrontendAction::Execute() (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x2c2c7f3)
#27 0x0000000002bbb949 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x2bbb949)
#28 0x0000000002d19e9d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0x2d19e9d)
#29 0x0000000000c97af6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0xc97af6)
#30 0x0000000000c8e0e1 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#31 0x0000000000c91902 clang_main(int, char**, llvm::ToolContext const&) (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0xc91902)
#32 0x0000000000bcaa03 main (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0xbcaa03)
#33 0x00007fc3554c97e5 __libc_start_main (/lib64/libc.so.6+0x3a7e5)
#34 0x0000000000c8d7ee _start (/nobackup/rabozic/llvm-upstream/install/bin/clang-22+0xc8d7ee)
Aborted (core dumped)
```

**Environment:**

- LLVM/Clang version: 22.0.0git (earliest version tested is 14, it also crashes)
- Commit hash: 9016c60c685e8f651fa9f9250703afe5f2c02565
- Build config: Assertions enabled
- Thread model: posix
- OS: Linux x86_64




## Comments

### Comment 1 - ParkHanbum

It looks similar to issue https://github.com/llvm/llvm-project/issues/165377


---

### Comment 2 - bozicrHT

Thank for looking into this!
I also found that adding an early bailout when the loop preheader is null prevents the crash, as in your solution.

---

### Comment 3 - fhahn

I think this was fixed by https://github.com/llvm/llvm-project/pull/166310. pelase re-open if it still reproduces on current `main`

---

### Comment 4 - bozicrHT

> I think this was fixed by [#166310](https://github.com/llvm/llvm-project/pull/166310). pelase re-open if it still reproduces on current `main`

It's fixed, thanks!

---

