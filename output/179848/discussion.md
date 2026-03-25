# mlir-opt crash: shape.from_extents folder fails on ub.poison during --acc-legalize-serial

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/179848
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-28T15:52:48Z

## Body

**Bug Op**: shape.from_extents

**Bug Pass**: --acc-legalize-serial

**Commenting out does not trigger the error**: smt.solver

**Detailed Description**: The mlir-opt tool crashes during the folding of shape.from_extents when using ub.poison. This issue is similar to [177951](https://github.com/llvm/llvm-project/issues/177951), but presents a different assertion failure.

The crash occurs because FromExtentsOp::fold attempts to cast<IntegerAttr> on a poison attribute without verification. Removing smt.solver prevents the crash because the unused shape.from_extents is removed by Dead Code Elimination (DCE) before folding. The folder should be updated to handle non-integer attributes safely.

```mlir
module {
  func.func @main() {
    %0 = gpu.dynamic_shared_memory : memref<?xi8, #gpu.address_space<workgroup>>
    %1 = ub.poison : index
    %2 = shape.from_extents %1 : index
    smt.solver(%2) : (!shape.shape) -> () {
      ^bb0(%arg0: !shape.shape):
      smt.yield
    }
    return
  }
}
```
```
mlir-opt --acc-legalize-serial temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --acc-legalize-serial temp.mlir
 #0 0x000061b927ca0e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000061b927c9d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000061b927c9d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007b1968e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007b1968e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007b1968e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007b1968e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007b1968e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007b1968e288ff abort ./stdlib/abort.c:81:7
 #9 0x00007b1968e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007b1968e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
·······
#33 0x000061b927b9a023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#34 0x00007b1968e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#35 0x00007b1968e2a28b call_init ./csu/../csu/libc-start.c:128:20
#36 0x00007b1968e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#37 0x000061b927c7f405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

**Crash info of [177951](https://github.com/llvm/llvm-project/issues/177951)**:
```mlir
module {
  func.func @main() {
    %0 = ub.poison : index
    %1 = ml_program.token
    %2 = shape.from_extents %0 : index
    smt.solver(%1, %2) : (!ml_program.token, !shape.shape) -> () {
      ^bb0(%arg0: !ml_program.token, %arg1: !shape.shape):
      smt.yield
    }
    return
  }
}
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --buffer-deallocation-simplification temp.mlir
 #0 0x0000600582620e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000060058261d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000060058261d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000776cd8645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000776cd869eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000776cd869eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000776cd869eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000776cd864527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000776cd86288ff abort ./stdlib/abort.c:81:7
 #9 0x0000776cd862881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000776cd863b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000600585dd66a3 mlir::shape::FromExtentsOp::fold(mlir::shape::FromExtentsOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>) (llvmorg-22.1.0-install/bin/mlir-opt+0x5ce56a3)
#12 0x0000600585dd67a9 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&>::CallImpl<mlir::Op<mlir::shape::FromExtentsOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::shape::ShapeType>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::OpInvariants, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::InferTypeOpInterface::Trait>::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) const>(void*, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x5ce57a9)
······
#29 0x000060058251a023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#30 0x0000776cd862a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x0000776cd862a28b call_init ./csu/../csu/libc-start.c:128:20
#32 0x0000776cd862a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#33 0x00006005825ff405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**Bug Op**: shape.from_extents

**Bug Pass**: --acc-legalize-serial

**Commenting out does not trigger the error**: smt.solver

**Detailed Description**: The mlir-opt tool crashes during the folding of shape.from_extents when using ub.poison. This issue is similar to [177951](https://github.com/llvm/llvm-project/issues/177951), but presents a different assertion failure.

The crash occurs because FromExtentsOp::fold attempts to cast&lt;IntegerAttr&gt; on a poison attribute without verification. Removing smt.solver prevents the crash because the unused shape.from_extents is removed by Dead Code Elimination (DCE) before folding. The folder should be updated to handle non-integer attributes safely.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = gpu.dynamic_shared_memory : memref&lt;?xi8, #gpu.address_space&lt;workgroup&gt;&gt;
    %1 = ub.poison : index
    %2 = shape.from_extents %1 : index
    smt.solver(%2) : (!shape.shape) -&gt; () {
      ^bb0(%arg0: !shape.shape):
      smt.yield
    }
    return
  }
}
```
```
mlir-opt --acc-legalize-serial temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&amp;) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --acc-legalize-serial temp.mlir
 #<!-- -->0 0x000061b927ca0e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000061b927c9d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000061b927c9d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007b1968e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007b1968e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007b1968e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007b1968e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007b1968e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007b1968e288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007b1968e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007b1968e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
·······
#<!-- -->33 0x000061b927b9a023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->34 0x00007b1968e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->35 0x00007b1968e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x00007b1968e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->37 0x000061b927c7f405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

**Crash info of [177951](https://github.com/llvm/llvm-project/issues/177951)**:
```mlir
module {
  func.func @<!-- -->main() {
    %0 = ub.poison : index
    %1 = ml_program.token
    %2 = shape.from_extents %0 : index
    smt.solver(%1, %2) : (!ml_program.token, !shape.shape) -&gt; () {
      ^bb0(%arg0: !ml_program.token, %arg1: !shape.shape):
      smt.yield
    }
    return
  }
}
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&amp;) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --buffer-deallocation-simplification temp.mlir
 #<!-- -->0 0x0000600582620e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000060058261d5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000060058261d6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000776cd8645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000776cd869eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000776cd869eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000776cd869eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000776cd864527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000776cd86288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x0000776cd862881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x0000776cd863b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000600585dd66a3 mlir::shape::FromExtentsOp::fold(mlir::shape::FromExtentsOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::Attribute&gt;&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x5ce56a3)
#<!-- -->12 0x0000600585dd67a9 llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;&gt;::CallImpl&lt;mlir::Op&lt;mlir::shape::FromExtentsOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::shape::ShapeType&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::OpInvariants, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::InferTypeOpInterface::Trait&gt;::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) const&gt;(void*, mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x5ce57a9)
······
#<!-- -->29 0x000060058251a023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->30 0x0000776cd862a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->31 0x0000776cd862a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->32 0x0000776cd862a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->33 0x00006005825ff405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

### Comment 2 - SakshamSinghal20

I would like to work on this could you please assign this to me?

---

### Comment 3 - Men-cotton

No crash at HEAD. I believe https://github.com/llvm/llvm-project/pull/178844 resolved this.

---

