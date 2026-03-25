# mlir-opt crash: bufferizesToMemoryRead not implemented in sparse_tensor.new during --eliminate-empty-tensors

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180309
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-28T11:35:27Z

## Body

**Bug Op**: sparse_tensor.new 

**Bug Pass**: --eliminate-empty-tensors

**Detailed Description**: The mlir-opt tool hits an UNREACHABLE assertion during the --eliminate-empty-tensors pass. The crash occurs because the sparse_tensor.new operation lacks an implementation for the bufferizesToMemoryRead method of the BufferizableOpInterface.

```mlir
module {
  func.func @main() {
    %0 = gpu.cluster_id  x
    %1 = shape.const_witness true
    affine.if affine_set<(d0) : (d0 >= 0)>(%0) {
    }
    %2 = sparse_tensor.new %1 : !shape.witness to tensor<?xindex, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>>
    %3 = shape.assuming %1 -> tensor<?xindex, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>> {
      shape.assuming_yield %2 : tensor<?xindex, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>>
    }
    %4 = sparse_tensor.new %3 : tensor<?xindex, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>> to tensor<?xindex, #sparse_tensor.encoding<{ map = (d0) -> (d0 : compressed) }>>
    return
  }
}
```
```
mlir-opt --eliminate-empty-tensors temp.mlir
```
```
bufferizesToMemoryRead not implemented
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/build/tools/mlir/include/mlir/Dialect/Bufferization/IR/BufferizableOpInterface.h.inc:1166!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --eliminate-empty-tensors temp.mlir
 #0 0x000064f5ccf65e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000064f5ccf625af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000064f5ccf626fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007c6a8ae45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007c6a8ae9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007c6a8ae9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007c6a8ae9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007c6a8ae4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007c6a8ae288ff abort ./stdlib/abort.c:81:7
 #9 0x000064f5ccf4512e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#10 0x000064f5cdb9d262 (llvmorg-22.1.0-install/bin/mlir-opt+0x3167262)
#11 0x000064f5d32b6b94 mlir::bufferization::OneShotAnalysisState::applyOnAliases(mlir::Value, llvm::function_ref<void (mlir::Value)>) const (.constprop.3) OneShotAnalysis.cpp:0:0
#12 0x000064f5d32b6f85 wouldCreateReadAfterWriteInterference(mlir::OpOperand&, mlir::DominanceInfo const&, mlir::bufferization::OneShotAnalysisState&, bool) OneShotAnalysis.cpp:0:0
#13 0x000064f5d32b72d7 checkPreBufferizationAssumptions(mlir::Operation*, mlir::DominanceInfo const&, mlir::bufferization::OneShotAnalysisState&)::'lambda0'(mlir::bufferization::BufferizableOpInterface)::operator()(mlir::bufferization::BufferizableOpInterface) const (.isra.0) OneShotAnalysis.cpp:0:0
#14 0x000064f5d32b7511 mlir::WalkResult mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Operation*)>, mlir::WalkOrder) (.constprop.1) OneShotAnalysis.cpp:0:0
#15 0x000064f5d32b74d0 mlir::WalkResult mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Operation*)>, mlir::WalkOrder) (.constprop.1) OneShotAnalysis.cpp:0:0
#16 0x000064f5d32b74d0 mlir::WalkResult mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Operation*)>, mlir::WalkOrder) (.constprop.1) OneShotAnalysis.cpp:0:0
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**Bug Op**: sparse_tensor.new 

**Bug Pass**: --eliminate-empty-tensors

**Detailed Description**: The mlir-opt tool hits an UNREACHABLE assertion during the --eliminate-empty-tensors pass. The crash occurs because the sparse_tensor.new operation lacks an implementation for the bufferizesToMemoryRead method of the BufferizableOpInterface.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = gpu.cluster_id  x
    %1 = shape.const_witness true
    affine.if affine_set&lt;(d0) : (d0 &gt;= 0)&gt;(%0) {
    }
    %2 = sparse_tensor.new %1 : !shape.witness to tensor&lt;?xindex, #sparse_tensor.encoding&lt;{ map = (d0) -&gt; (d0 : compressed) }&gt;&gt;
    %3 = shape.assuming %1 -&gt; tensor&lt;?xindex, #sparse_tensor.encoding&lt;{ map = (d0) -&gt; (d0 : compressed) }&gt;&gt; {
      shape.assuming_yield %2 : tensor&lt;?xindex, #sparse_tensor.encoding&lt;{ map = (d0) -&gt; (d0 : compressed) }&gt;&gt;
    }
    %4 = sparse_tensor.new %3 : tensor&lt;?xindex, #sparse_tensor.encoding&lt;{ map = (d0) -&gt; (d0 : compressed) }&gt;&gt; to tensor&lt;?xindex, #sparse_tensor.encoding&lt;{ map = (d0) -&gt; (d0 : compressed) }&gt;&gt;
    return
  }
}
```
```
mlir-opt --eliminate-empty-tensors temp.mlir
```
```
bufferizesToMemoryRead not implemented
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/build/tools/mlir/include/mlir/Dialect/Bufferization/IR/BufferizableOpInterface.h.inc:1166!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --eliminate-empty-tensors temp.mlir
 #<!-- -->0 0x000064f5ccf65e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000064f5ccf625af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000064f5ccf626fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007c6a8ae45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007c6a8ae9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007c6a8ae9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007c6a8ae9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007c6a8ae4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007c6a8ae288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000064f5ccf4512e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#<!-- -->10 0x000064f5cdb9d262 (llvmorg-22.1.0-install/bin/mlir-opt+0x3167262)
#<!-- -->11 0x000064f5d32b6b94 mlir::bufferization::OneShotAnalysisState::applyOnAliases(mlir::Value, llvm::function_ref&lt;void (mlir::Value)&gt;) const (.constprop.3) OneShotAnalysis.cpp:0:0
#<!-- -->12 0x000064f5d32b6f85 wouldCreateReadAfterWriteInterference(mlir::OpOperand&amp;, mlir::DominanceInfo const&amp;, mlir::bufferization::OneShotAnalysisState&amp;, bool) OneShotAnalysis.cpp:0:0
#<!-- -->13 0x000064f5d32b72d7 checkPreBufferizationAssumptions(mlir::Operation*, mlir::DominanceInfo const&amp;, mlir::bufferization::OneShotAnalysisState&amp;)::'lambda0'(mlir::bufferization::BufferizableOpInterface)::operator()(mlir::bufferization::BufferizableOpInterface) const (.isra.0) OneShotAnalysis.cpp:0:0
#<!-- -->14 0x000064f5d32b7511 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.1) OneShotAnalysis.cpp:0:0
#<!-- -->15 0x000064f5d32b74d0 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.1) OneShotAnalysis.cpp:0:0
#<!-- -->16 0x000064f5d32b74d0 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.1) OneShotAnalysis.cpp:0:0
```
</details>


---

### Comment 2 - JueonPark

I think this is fixed from https://github.com/llvm/llvm-project/pull/178423

---

