# [mlir-opt] Crash in shape.broadcast folding: llvm::cast<DenseIntElementsAttr> failure triggered by tensor.rank with ub.poison

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/179679
**Status:** Closed
**Labels:** crash, mlir:shape
**Closed Date:** 2026-03-05T12:23:54Z

## Body

The fold implementation of **shape.broadcast** performs an unchecked llvm::cast<DenseIntElementsAttr> on its operand attributes. When ub.poison is present, the attribute is a PoisonAttr, which causes the cast to fail.

```mlir
module {
  func.func @main() {
    %0 = shape.const_shape [1, 2, 3] : tensor<3xindex>
    %1 = ub.poison : tensor<3xindex>
    %2 = shape.broadcast %0, %1 : tensor<3xindex>, tensor<3xindex> -> tensor<3xindex>
    %3 = tensor.rank %2 : tensor<3xindex>
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: /llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::DenseIntElementsAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #0 0x000062a90a669e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000062a90a6665af llvm::sys::RunSignalHandlers() (/llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000062a90a6666fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007225ab245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007225ab29eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007225ab29eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007225ab29eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007225ab24527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007225ab2288ff abort ./stdlib/abort.c:81:7
 #9 0x00007225ab22881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007225ab23b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000062a90de20d0c mlir::shape::BroadcastOp::fold(mlir::shape::BroadcastOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>) (/llvmorg-22.1.0-install/bin/mlir-opt+0x5ce6d0c)
#12 0x000062a90de75816 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&>::CallImpl<mlir::Op<mlir::shape::BroadcastOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::Type>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::IsCommutative, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait>::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) const>(void*, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (/llvmorg-22.1.0-install/bin/mlir-opt+0x5d3b816)
#13 0x000062a90de82f9e mlir::RegisteredOperationName::Model<mlir::shape::BroadcastOp>::foldHook(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (/llvmorg-22.1.0-install/bin/mlir-opt+0x5d48f9e)
```

## Comments

### Comment 1 - EugeneZelenko

@Subway2023: If you plan to file more issues in future, it'll be good idea to request triage role ([examples](https://github.com/llvm/llvm-project/issues/?q=is%3Aissue%20label%3Ainfra%3Acommit-access-request%20Triage)), so you'll be able to label issues yourself, preferably at creation.


---

### Comment 2 - Subway2023

Thanks a lot for the suggestion

---

