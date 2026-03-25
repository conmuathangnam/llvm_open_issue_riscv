# [mlir-opt] Parser/Verifier crash in linalg.batch_matmul

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/179677
**Status:** Closed
**Labels:** mlir:linalg, crash
**Closed Date:** 2026-03-02T14:18:31Z

## Body

mlir-opt crashes during the parsing/verification stage of a **linalg.batch_matmul** operation. 
The crash is triggered when the operation uses hardware-specific types like !amx.tile as operands. The Linalg regionBuilder (which automatically generates the scalar implementation for named ops) hits an llvm_unreachable because it only supports standard numeric (integer or float) types.
```mlir
module {
  func.func @main() {
    %0 = spirv.GroupNonUniformElect <Workgroup> : i1
    %1 = amx.tile_zero : !amx.tile<16x16xbf16>
    %2 = tensor.from_elements %0 : tensor<i1>
    %3 = tosa.reciprocal %2 : (tensor<i1>) -> tensor<i1>
    %4 = shape.const_shape [16, 16] : !shape.shape
    %5 = linalg.batch_matmul ins(%1, %1 : !amx.tile<16x16xbf16>, !amx.tile<16x16xbf16>) outs(%1 : !amx.tile<16x16xbf16>) -> !amx.tile<16x16xbf16>
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
unsupported non numeric type
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Linalg/IR/LinalgOps.cpp:514!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'linalg.batch_matmul'
 #0 0x00005bbc24c1be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005bbc24c185af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005bbc24c186fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000726cc0045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000726cc009eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000726cc009eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000726cc009eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000726cc004527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000726cc00288ff abort ./stdlib/abort.c:81:7
 #9 0x00005bbc24bfb12e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#10 0x00005bbc2abbee74 (anonymous namespace)::RegionBuilderHelper::buildBinaryFn(mlir::linalg::BinaryFn, mlir::Value, mlir::Value, llvm::function_ref<mlir::InFlightDiagnostic ()>) LinalgOps.cpp:0:0
#11 0x00005bbc2abc7e8e mlir::linalg::BatchMatmulOp::regionBuilder(mlir::ImplicitLocOpBuilder&, mlir::Block&, llvm::ArrayRef<mlir::NamedAttribute>, llvm::function_ref<mlir::InFlightDiagnostic ()>) (llvmorg-22.1.0-install/bin/mlir-opt+0x84dbe8e)
#12 0x00005bbc2aac195e void llvm::function_ref<void (mlir::ImplicitLocOpBuilder&, mlir::Block&, llvm::ArrayRef<mlir::NamedAttribute>, llvm::function_ref<mlir::InFlightDiagnostic ()>)>::callback_fn<std::function<void (mlir::ImplicitLocOpBuilder&, mlir::Block&, llvm::ArrayRef<mlir::NamedAttribute>, llvm::function_ref<mlir::InFlightDiagnostic ()>)>>(long, mlir::ImplicitLocOpBuilder&, mlir::Block&, llvm::ArrayRef<mlir::NamedAttribute>, llvm::function_ref<mlir::InFlightDiagnostic ()>) (llvmorg-22.1.0-install/bin/mlir-opt+0x83d595e)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir-linalg

Author: None (Subway2023)

<details>
mlir-opt crashes during the parsing/verification stage of a **linalg.batch_matmul** operation. 
The crash is triggered when the operation uses hardware-specific types like !amx.tile as operands. The Linalg regionBuilder (which automatically generates the scalar implementation for named ops) hits an llvm_unreachable because it only supports standard numeric (integer or float) types.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = spirv.GroupNonUniformElect &lt;Workgroup&gt; : i1
    %1 = amx.tile_zero : !amx.tile&lt;16x16xbf16&gt;
    %2 = tensor.from_elements %0 : tensor&lt;i1&gt;
    %3 = tosa.reciprocal %2 : (tensor&lt;i1&gt;) -&gt; tensor&lt;i1&gt;
    %4 = shape.const_shape [16, 16] : !shape.shape
    %5 = linalg.batch_matmul ins(%1, %1 : !amx.tile&lt;16x16xbf16&gt;, !amx.tile&lt;16x16xbf16&gt;) outs(%1 : !amx.tile&lt;16x16xbf16&gt;) -&gt; !amx.tile&lt;16x16xbf16&gt;
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
unsupported non numeric type
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Linalg/IR/LinalgOps.cpp:514!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'linalg.batch_matmul'
 #<!-- -->0 0x00005bbc24c1be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005bbc24c185af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005bbc24c186fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000726cc0045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000726cc009eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000726cc009eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000726cc009eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000726cc004527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000726cc00288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00005bbc24bfb12e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#<!-- -->10 0x00005bbc2abbee74 (anonymous namespace)::RegionBuilderHelper::buildBinaryFn(mlir::linalg::BinaryFn, mlir::Value, mlir::Value, llvm::function_ref&lt;mlir::InFlightDiagnostic ()&gt;) LinalgOps.cpp:0:0
#<!-- -->11 0x00005bbc2abc7e8e mlir::linalg::BatchMatmulOp::regionBuilder(mlir::ImplicitLocOpBuilder&amp;, mlir::Block&amp;, llvm::ArrayRef&lt;mlir::NamedAttribute&gt;, llvm::function_ref&lt;mlir::InFlightDiagnostic ()&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x84dbe8e)
#<!-- -->12 0x00005bbc2aac195e void llvm::function_ref&lt;void (mlir::ImplicitLocOpBuilder&amp;, mlir::Block&amp;, llvm::ArrayRef&lt;mlir::NamedAttribute&gt;, llvm::function_ref&lt;mlir::InFlightDiagnostic ()&gt;)&gt;::callback_fn&lt;std::function&lt;void (mlir::ImplicitLocOpBuilder&amp;, mlir::Block&amp;, llvm::ArrayRef&lt;mlir::NamedAttribute&gt;, llvm::function_ref&lt;mlir::InFlightDiagnostic ()&gt;)&gt;&gt;(long, mlir::ImplicitLocOpBuilder&amp;, mlir::Block&amp;, llvm::ArrayRef&lt;mlir::NamedAttribute&gt;, llvm::function_ref&lt;mlir::InFlightDiagnostic ()&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x83d595e)
```
</details>


---

### Comment 2 - rengolin

See #179056 for similar issues. Please whoever works on this, bring a holistic solution, not a quick fix.

---

### Comment 3 - shubhamnarlawar

> @llvm/issue-subscribers-mlir-linalg
> 
> Author: None (Subway2023)
> 
> Details

Can you assign this issue to me?

---

### Comment 4 - joker-eph

Seems like this is fixed at HEAD, maybe by #181616 ? Feel free to reopen if it reproduces for you.

---

### Comment 5 - shubhamnarlawar

> Seems like this is fixed at HEAD, maybe by [#181616](https://github.com/llvm/llvm-project/pull/181616) ? Feel free to reopen if it reproduces for you.

Yes, this is merged!


---

