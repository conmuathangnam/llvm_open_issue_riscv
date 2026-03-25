# mlir-opt crash: Assertion failed in llvm::cast during --canonicalize involving shape.broadcast with ub.poison

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/179845
**Status:** Closed
**Labels:** crash, mlir:shape
**Closed Date:** 2026-03-08T10:09:16Z

## Body

The mlir-opt tool crashes with an assertion failure when running the **--canonicalize** pass on MLIR code where a **shape.broadcast** operation consumes a ub.poison value. 
The crash occurs because the folder for **shape.broadcast** attempts to llvm::cast a **ub.poison** attribute (or a null attribute) to a mlir::DenseIntElementsAttr without verifying the attribute type.
```mlir
module {
  func.func @main() {
    %0 = irdl.c_pred "::llvm::isa<::mlir::IntegerAttr>($_self)"
    %1 = shape.const_shape [1, 2, 3] : tensor<3xindex>
    smt.solver(%1) : (tensor<3xindex>) -> () {
      ^bb0(%arg0: tensor<3xindex>):
      smt.yield
    }
    %2 = ub.poison : tensor<3xindex>
    %3 = shape.broadcast %2, %2 : tensor<3xindex>, tensor<3xindex> -> tensor<3xindex>
    %4 = shape.broadcast %3, %2 : tensor<3xindex>, tensor<3xindex> -> tensor<3xindex>
    return
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::DenseIntElementsAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #0 0x00005abb4a911e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005abb4a90e5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005abb4a90e6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000706e23445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000706e2349eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000706e2349eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000706e2349eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000706e2344527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000706e234288ff abort ./stdlib/abort.c:81:7
 #9 0x0000706e2342881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000706e2343b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005abb4e0c8c68 mlir::shape::BroadcastOp::fold(mlir::shape::BroadcastOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>) (llvmorg-22.1.0-install/bin/mlir-opt+0x5ce6c68)
#12 0x00005abb4e11d816 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&>::CallImpl<mlir::Op<mlir::shape::BroadcastOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::Type>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::IsCommutative, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait>::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) const>(void*, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x5d3b816)
#13 0x00005abb4e12af9e mlir::RegisteredOperationName::Model<mlir::shape::BroadcastOp>::foldHook(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x5d48f9e)
#14 0x000
```

## Comments

### Comment 1 - SakshamSinghal20

I would like to work on this could you please assign this to me?

---

### Comment 2 - Men-cotton

@SakshamSinghal20 
Assigned. Please note that assignment is not required to work on an issue and submit a PR. Just submitting a PR is sufficient.

---

### Comment 3 - SakshamSinghal20

okay got it sir, I will raise the PR soon.

---

### Comment 4 - Men-cotton

I confirmed that this crash is fixed by https://github.com/llvm/llvm-project/pull/183931.

---

