# mlir-opt crash: affine.linearize_index folder fails on ub.poison due to invalid IntegerAttr cast

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180106
**Status:** Closed
**Labels:** mlir:affine, crash
**Closed Date:** 2026-02-28T11:15:30Z

## Body

**MLIR Version**: 22.1.0

**Bug Op**: affine.linearize_index

**Bug Pass**: --canonicalize

**Detailed Description**: The mlir-opt tool crashes during the folding of affine.linearize_index when an operand is ub.poison. The folder attempts to llvm::cast<IntegerAttr> a PoisonAttr, triggering an assertion failure. Removing the smt.solver block prevents the crash because DCE deletes the unused operation before folding occurs.

```mlir
module {
  func.func @main() {
    %0 = index.constant 0
    %1 = ub.poison : index
    %2 = affine.linearize_index [%0, %1] by (1) : index
    smt.solver(%2) : (index) -> () {
      ^bb0(%arg0: index):
      smt.yield
    }
    return
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #0 0x00006092cdc68e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00006092cdc655af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00006092cdc656fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000070c32ea45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000070c32ea9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000070c32ea9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000070c32ea9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000070c32ea4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000070c32ea288ff abort ./stdlib/abort.c:81:7
 #9 0x000070c32ea2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000070c32ea3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00006092d50d98f2 (llvmorg-22.1.0-install/bin/mlir-opt+0x99a08f2)
#12 0x00006092d5115819 mlir::affine::AffineLinearizeIndexOp::fold(mlir::affine::AffineLinearizeIndexOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>) (llvmorg-22.1.0-install/bin/mlir-opt+0x99dc819)
#13 0x00006092d514093f llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&>::CallImpl<mlir::Op<mlir::affine::AffineLinearizeIndexOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::IndexType>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::InferTypeOpInterface::Trait>::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) const>(void*, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x9a0793f)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir-affine

Author: None (xvv4402)

<details>
**MLIR Version**: 22.1.0

**Bug Op**: affine.linearize_index

**Bug Pass**: --canonicalize

**Detailed Description**: The mlir-opt tool crashes during the folding of affine.linearize_index when an operand is ub.poison. The folder attempts to llvm::cast&lt;IntegerAttr&gt; a PoisonAttr, triggering an assertion failure. Removing the smt.solver block prevents the crash because DCE deletes the unused operation before folding occurs.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = index.constant 0
    %1 = ub.poison : index
    %2 = affine.linearize_index [%0, %1] by (1) : index
    smt.solver(%2) : (index) -&gt; () {
      ^bb0(%arg0: index):
      smt.yield
    }
    return
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #<!-- -->0 0x00006092cdc68e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00006092cdc655af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00006092cdc656fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000070c32ea45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000070c32ea9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000070c32ea9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000070c32ea9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000070c32ea4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000070c32ea288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000070c32ea2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000070c32ea3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00006092d50d98f2 (llvmorg-22.1.0-install/bin/mlir-opt+0x99a08f2)
#<!-- -->12 0x00006092d5115819 mlir::affine::AffineLinearizeIndexOp::fold(mlir::affine::AffineLinearizeIndexOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::Attribute&gt;&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x99dc819)
#<!-- -->13 0x00006092d514093f llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;&gt;::CallImpl&lt;mlir::Op&lt;mlir::affine::AffineLinearizeIndexOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::IndexType&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::InferTypeOpInterface::Trait&gt;::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) const&gt;(void*, mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9a0793f)
```
</details>


---

### Comment 2 - JueonPark

I made a fix PR regarding this:
https://github.com/llvm/llvm-project/pull/181144

---

