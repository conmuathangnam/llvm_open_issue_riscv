# [AArch64] Assertion `EltCnt.isKnownEven() && "Cannot halve vector with odd number of elements."' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/166320
**Status:** Closed
**Labels:** crash, llvm::vectorcombine
**Closed Date:** 2025-11-06T12:45:43Z

## Body

Reproducer:
https://godbolt.org/z/a3WMoGhPd

Backtrace:
```console
opt: /root/llvm-project/llvm/include/llvm/IR/DerivedTypes.h:538: static llvm::VectorType* llvm::VectorType::getHalfElementsVectorType(llvm::VectorType*): Assertion `EltCnt.isKnownEven() && "Cannot halve vector with odd number of elements."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=vector-combine <source>
1.	Running pass "function(vector-combine)" on module "<source>"
2.	Running pass "vector-combine" on function "interleave2_same_const_splat_nxv4i16"
 #0 0x0000000005989ef8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5989ef8)
 #1 0x0000000005986da4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000078099b042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000078099b0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000078099b042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000078099b0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000078099b02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000078099b039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000000bd6b5a llvm::BasicTTIImplBase<llvm::AArch64TTIImpl>::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xbd6b5a)
 #9 0x0000000000bd6cd8 llvm::AArch64TTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xbd6cd8)
#10 0x0000000000bd6841 llvm::BasicTTIImplBase<llvm::AArch64TTIImpl>::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xbd6841)
#11 0x0000000000bd6cd8 llvm::AArch64TTIImpl::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xbd6cd8)
#12 0x00000000050299fe llvm::TargetTransformInfo::getCastInstrCost(unsigned int, llvm::Type*, llvm::Type*, llvm::TargetTransformInfo::CastContextHint, llvm::TargetTransformInfo::TargetCostKind, llvm::Instruction const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x50299fe)
#13 0x00000000037fb501 (anonymous namespace)::VectorCombine::foldInterleaveIntrinsics(llvm::Instruction&) VectorCombine.cpp:0:0
#14 0x000000000380fbd8 (anonymous namespace)::VectorCombine::run()::'lambda'(llvm::Instruction&)::operator()(llvm::Instruction&) const (.isra.0) VectorCombine.cpp:0:0
#15 0x0000000003811215 llvm::VectorCombinePass::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3811215)
#16 0x0000000002faaebe llvm::detail::PassModel<llvm::Function, llvm::VectorCombinePass, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2faaebe)
#17 0x0000000005763711 llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5763711)
#18 0x0000000000efdf2e llvm::detail::PassModel<llvm::Function, llvm::PassManager<llvm::Function, llvm::AnalysisManager<llvm::Function>>, llvm::AnalysisManager<llvm::Function>>::run(llvm::Function&, llvm::AnalysisManager<llvm::Function>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0xefdf2e)
#19 0x0000000005761d8a llvm::ModuleToFunctionPassAdaptor::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761d8a)
#20 0x000000000097972e llvm::detail::PassModel<llvm::Module, llvm::ModuleToFunctionPassAdaptor, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97972e)
#21 0x0000000005761741 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5761741)
#22 0x00000000009838ca llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9838ca)
#23 0x0000000000977b41 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x977b41)
#24 0x000078099b029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x000078099b029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x000000000096ef55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x96ef55)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - k-arrows

Comes from llvm/test/CodeGen/AArch64/sve-vector-interleave.ll

---

### Comment 2 - ahmedshakill

Looking into it trying to understand if I can take up this task. Will let you know. 

---

### Comment 3 - ahmedshakill

@k-arrows I looked into it. Could you please assign me this task. 

---

