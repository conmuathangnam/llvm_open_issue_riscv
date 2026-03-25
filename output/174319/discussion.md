# mlir-opt after transform-interpreter have problem about scf.forall to scf.for

**Author:** Confen
**URL:** https://github.com/llvm/llvm-project/issues/174319
**Status:** Closed
**Labels:** crash, mlir:scf
**Closed Date:** 2026-01-19T23:48:58Z

## Body

### Description

`mlir-opt` crashes with a segmentation fault while running a transform dialect interpreter pipeline.
The crash happens during verification of `tensor.extract_slice` (`ExtractSliceOp::verifyInvariantsImpl`).

This appears to be triggered by the transform sequence (or by invalid IR produced by the transform), but the crash is a hard segfault instead of a clean verifier failure.

---

### Steps to Reproduce

Run:

```bash
mlir-opt transform_interpreter.mlir --scf-forall-to-for

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #0 0x00005e8bf5801632 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/llvm-21/bin/mlir-opt+0x1d3c632)
 #1 0x00005e8bf57fe312 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000714642c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #3 0x00005e8bf7c3dae5 mlir::tensor::__mlir_ods_local_type_constraint_TensorOps4(mlir::Operation*, mlir::Type, llvm::StringRef, unsigned int) TensorOps.cpp:0:0
 #4 0x00005e8bf7c4061b mlir::tensor::ExtractSliceOp::verifyInvariantsImpl() (/opt/llvm-21/bin/mlir-opt+0x417b61b)
 #5 0x00005e8bf7be4648 mlir::Op<mlir::tensor::ExtractSliceOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::RankedTensorType>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands<1u>::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpAsmOpInterface::Trait, mlir::ReifyRankedShapedTypeOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OffsetSizeAndStrideOpInterface::Trait>::verifyInvariants(mlir::Operation*) (/opt/llvm-21/bin/mlir-opt+0x411f648)
 #6 0x00005e8bf7be9be7 mlir::RegisteredOperationName::Model<mlir::tensor::ExtractSliceOp>::verifyInvariants(mlir::Operation*) (/opt/llvm-21/bin/mlir-opt+0x4124be7)
 #7 0x00005e8bf88fa43c (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
 #8 0x00005e8bf88fb662 std::_Function_handler<void (), llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::'lambda'(mlir::Operation*)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::'lambda'(mlir::Operation*)&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::'lambda'(mlir::Operation*)&&)::'lambda'()>::_M_invoke(std::_Any_data const&) Verifier.cpp:0:0
 #9 0x00005e8bf86d5841 std::_Function_handler<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> (), std::__future_base::_Task_setter<std::unique_ptr<std::__future_base::_Result<void>, std::__future_base::_Result_base::_Deleter>, std::thread::_Invoker<std::tuple<std::function<void ()>>>, void>>::_M_invoke(std::_Any_data const&) (/opt/llvm-21/bin/mlir-opt+0x4c10841)
#10 0x00005e8bf86d66ad std::__future_base::_State_baseV2::_M_do_set(std::function<std::unique_ptr<std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter> ()>*, bool*) (/opt/llvm-21/bin/mlir-opt+0x4c116ad)
#11 0x0000714642ca1ed3 __pthread_once_slow ./nptl/pthread_once.c:118:7
#12 0x00005e8bf86d649e std::__future_base::_Deferred_state<std::thread::_Invoker<std::tuple<std::function<void ()>>>, void>::_M_complete_async() (/opt/llvm-21/bin/mlir-opt+0x4c1149e)
#13 0x00005e8bf86d61c3 std::_Function_handler<void (), std::shared_future<void> llvm::ThreadPoolInterface::asyncImpl<void>(std::function<void ()>, llvm::ThreadPoolTaskGroup*)::'lambda'()>::_M_invoke(std::_Any_data const&) (/opt/llvm-21/bin/mlir-opt+0x4c111c3)
#14 0x00005e8bfb40abd5 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) (/opt/llvm-21/bin/mlir-opt+0x7945bd5)
#15 0x00005e8bfb40b5c1 void* llvm::thread::ThreadProxy<std::tuple<llvm::StdThreadPool::grow(int)::'lambda'()>>(void*) ThreadPool.cpp:0:0
#16 0x0000714642c9caa4 start_thread ./nptl/pthread_create.c:447:8
#17 0x0000714642d29c6c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Segmentation fault (core dumped)



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Confen)

<details>
### Description

`mlir-opt` crashes with a segmentation fault while running a transform dialect interpreter pipeline.
The crash happens during verification of `tensor.extract_slice` (`ExtractSliceOp::verifyInvariantsImpl`).

This appears to be triggered by the transform sequence (or by invalid IR produced by the transform), but the crash is a hard segfault instead of a clean verifier failure.

---

### Steps to Reproduce

Run:

```bash
mlir-opt transform_interpreter.mlir --scf-forall-to-for

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #<!-- -->0 0x00005e8bf5801632 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/llvm-21/bin/mlir-opt+0x1d3c632)
 #<!-- -->1 0x00005e8bf57fe312 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000714642c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->3 0x00005e8bf7c3dae5 mlir::tensor::__mlir_ods_local_type_constraint_TensorOps4(mlir::Operation*, mlir::Type, llvm::StringRef, unsigned int) TensorOps.cpp:0:0
 #<!-- -->4 0x00005e8bf7c4061b mlir::tensor::ExtractSliceOp::verifyInvariantsImpl() (/opt/llvm-21/bin/mlir-opt+0x417b61b)
 #<!-- -->5 0x00005e8bf7be4648 mlir::Op&lt;mlir::tensor::ExtractSliceOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::RankedTensorType&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;1u&gt;::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpAsmOpInterface::Trait, mlir::ReifyRankedShapedTypeOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OffsetSizeAndStrideOpInterface::Trait&gt;::verifyInvariants(mlir::Operation*) (/opt/llvm-21/bin/mlir-opt+0x411f648)
 #<!-- -->6 0x00005e8bf7be9be7 mlir::RegisteredOperationName::Model&lt;mlir::tensor::ExtractSliceOp&gt;::verifyInvariants(mlir::Operation*) (/opt/llvm-21/bin/mlir-opt+0x4124be7)
 #<!-- -->7 0x00005e8bf88fa43c (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
 #<!-- -->8 0x00005e8bf88fb662 std::_Function_handler&lt;void (), llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::'lambda'(mlir::Operation*)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::'lambda'(mlir::Operation*)&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::'lambda'(mlir::Operation*)&amp;&amp;)::'lambda'()&gt;::_M_invoke(std::_Any_data const&amp;) Verifier.cpp:0:0
 #<!-- -->9 0x00005e8bf86d5841 std::_Function_handler&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; (), std::__future_base::_Task_setter&lt;std::unique_ptr&lt;std::__future_base::_Result&lt;void&gt;, std::__future_base::_Result_base::_Deleter&gt;, std::thread::_Invoker&lt;std::tuple&lt;std::function&lt;void ()&gt;&gt;&gt;, void&gt;&gt;::_M_invoke(std::_Any_data const&amp;) (/opt/llvm-21/bin/mlir-opt+0x4c10841)
#<!-- -->10 0x00005e8bf86d66ad std::__future_base::_State_baseV2::_M_do_set(std::function&lt;std::unique_ptr&lt;std::__future_base::_Result_base, std::__future_base::_Result_base::_Deleter&gt; ()&gt;*, bool*) (/opt/llvm-21/bin/mlir-opt+0x4c116ad)
#<!-- -->11 0x0000714642ca1ed3 __pthread_once_slow ./nptl/pthread_once.c:118:7
#<!-- -->12 0x00005e8bf86d649e std::__future_base::_Deferred_state&lt;std::thread::_Invoker&lt;std::tuple&lt;std::function&lt;void ()&gt;&gt;&gt;, void&gt;::_M_complete_async() (/opt/llvm-21/bin/mlir-opt+0x4c1149e)
#<!-- -->13 0x00005e8bf86d61c3 std::_Function_handler&lt;void (), std::shared_future&lt;void&gt; llvm::ThreadPoolInterface::asyncImpl&lt;void&gt;(std::function&lt;void ()&gt;, llvm::ThreadPoolTaskGroup*)::'lambda'()&gt;::_M_invoke(std::_Any_data const&amp;) (/opt/llvm-21/bin/mlir-opt+0x4c111c3)
#<!-- -->14 0x00005e8bfb40abd5 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) (/opt/llvm-21/bin/mlir-opt+0x7945bd5)
#<!-- -->15 0x00005e8bfb40b5c1 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;llvm::StdThreadPool::grow(int)::'lambda'()&gt;&gt;(void*) ThreadPool.cpp:0:0
#<!-- -->16 0x0000714642c9caa4 start_thread ./nptl/pthread_create.c:447:8
#<!-- -->17 0x0000714642d29c6c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Segmentation fault (core dumped)


</details>


---

### Comment 2 - CoTinker

We can't reproduce without transform_interpreter.mlir

---

### Comment 3 - Confen

> 没有transform_interpreter.mlir，我们无法繁殖
```
#map = affine_map<(d0) -> (d0 * 4)>
#map1 = affine_map<(d0) -> (d0 * 32)>
#map2 = affine_map<(d0, d1) -> (d0, d1)>
#map3 = affine_map<(d0, d1) -> ()>
module attributes {transform.with_named_sequence} {
  func.func @fc_relu(%arg0: tensor<512x512xf32>, %arg1: tensor<512x512xf32>, %arg2: tensor<512x512xf32>, %arg3: tensor<512x512xf32>) -> tensor<512x512xf32> {
    %0 = scf.forall (%arg4, %arg5) in (128, 16) shared_outs(%arg6 = %arg3) -> (tensor<512x512xf32>) {
      %3 = affine.apply #map(%arg4)
      %4 = affine.apply #map1(%arg5)
      %extracted_slice = tensor.extract_slice %arg0[%3, 0] [4, 512] [1, 1] : tensor<512x512xf32> to tensor<4x512xf32>
      %extracted_slice_0 = tensor.extract_slice %arg1[0, %4] [512, 32] [1, 1] : tensor<512x512xf32> to tensor<512x32xf32>
      %extracted_slice_1 = tensor.extract_slice %arg6[%3, %4] [4, 32] [1, 1] : tensor<512x512xf32> to tensor<4x32xf32>
      %5 = linalg.matmul ins(%extracted_slice, %extracted_slice_0 : tensor<4x512xf32>, tensor<512x32xf32>) outs(%extracted_slice_1 : tensor<4x32xf32>) -> tensor<4x32xf32>
      scf.forall.in_parallel {
        tensor.parallel_insert_slice %5 into %arg6[%3, %4] [4, 32] [1, 1] : tensor<4x32xf32> into tensor<512x512xf32>
      }
    }
    %1 = linalg.elementwise kind=#linalg.elementwise_kind<add> ins(%0, %arg2 : tensor<512x512xf32>, tensor<512x512xf32>) outs(%arg3 : tensor<512x512xf32>) -> tensor<512x512xf32>
    %cst = arith.constant 0.000000e+00 : f32
    %2 = linalg.elementwise kind=#linalg.elementwise_kind<max_signed> indexing_maps = [#map2, #map3, #map2] ins(%1, %cst : tensor<512x512xf32>, f32) outs(%arg3 : tensor<512x512xf32>) -> tensor<512x512xf32>
    return %2 : tensor<512x512xf32>
  }
  transform.named_sequence @__transform_main(%arg0: !transform.any_op, %arg1: !transform.op<"linalg.matmul">, %arg2: !transform.op<"linalg.elementwise">) {
    %tiled_op, %forall_op = transform.structured.tile_using_forall %arg1 tile_sizes [4, 32] : (!transform.op<"linalg.matmul">) -> (!transform.any_op, !transform.any_op)
    transform.yield 
  }
}
```


---

### Comment 4 - meshtag

I think this is not a problem with `tensor.extract_slice` (no invalid IR in the picture) but is instead due to the `scf.forall` to `scf.for` conversion. There doesn't seem to be any handling of original op results in `-scf-forall-to-for` pass, so it should cleanly bail out in such cases without crashing helper utilities ahead. Can raise a PR to verify when I get the chance.

---

