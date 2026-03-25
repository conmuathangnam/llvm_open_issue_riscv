# mlir-opt --canonicalize crashes with assertion failure when folding tensor.from_elements containing ub.poison

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/178818
**Status:** Closed
**Labels:** obsolete, mlir:tensor, crash
**Closed Date:** 2026-02-27T01:37:26Z

## Body

mlir-opt version: -22.1.0-rc1

mlir-opt crashes when folding a tensor.from_elements operation containing a ub.poison operand, and the crash is triggered when its result is used by a **tensor.extract** operation. The assertion failure is: llvm::cast<mlir::IntegerAttr>() argument of incompatible type!. Notably, if the **tensor.extract** is commented out, the crash does not occur.
```mlir
module {
  func.func @main() {
    %0 = ub.poison : index
    %1 = amx.tile_zero : !amx.tile<16x16xbf16>
    %2 = tensor.from_elements %0, %0 : tensor<2xindex>
    %3 = tensor.extract %2[%0] : tensor<2xindex>
    return
  }
}
```
```
mlir-opt --canonicalize test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize test.mlir
 #0 0x0000559ff5927e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000559ff59245af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000559ff59246fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000767db3445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000767db349eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000767db349eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000767db349eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000767db344527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000767db34288ff abort ./stdlib/abort.c:81:7
 #9 0x0000767db342881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000767db343b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000559ffd4a2578 mlir::DenseElementsAttr::get(mlir::ShapedType, llvm::ArrayRef<mlir::Attribute>) (llvmorg-22.1.0-install/bin/mlir-opt+0xa0aa578)
#12 0x0000559ffcd6920e mlir::tensor::FromElementsOp::fold(mlir::tensor::FromElementsOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>) (llvmorg-22.1.0-install/bin/mlir-opt+0x997120e)
#13 0x0000559ffcd15ab2 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&>::CallImpl<mlir::Op<mlir::tensor::FromElementsOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::RankedTensorType>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::OpInvariants, mlir::OpAsmOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait>::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) const>(void*, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x991dab2)
```

## Comments

### Comment 1 - joker-eph

This seems fixed at HEAD. Please reopen if this still happens

---

