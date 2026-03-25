# [MLIR]    affine-loop-coalescing  crash 

**Author:** RJerrica
**URL:** https://github.com/llvm/llvm-project/issues/169483
**Status:** Closed
**Labels:** mlir:affine, crash
**Closed Date:** 2025-12-01T02:00:56Z

## Body

Hi, I encountered a crash bug while lowering the following program with MLIR.

### Code example
```llvm
module {
  func.func @memory_op() {
    %c0 = arith.constant 0 : index
    %c17 = arith.constant 17 : index
    %c79 = arith.constant 79 : index
    %c64 = arith.constant 64 : index
    %gpu_alloc = gpu.alloc(%c79) : memref<?xi64>
    
    affine.for %i = 0 to 79 {
        %result = affine.for %j = 0 to 64 iter_args(%iter_arg = %gpu_alloc) -> (memref<?xi64>) {
            %index = arith.remui %j, %c17 : index
            %j_i64 = arith.index_cast %j : index to i64
            memref.store %j_i64, %iter_arg[%index] : memref<?xi64>
            %dim_size = memref.dim %iter_arg, %c0 : memref<?xi64>
            affine.yield %iter_arg : memref<?xi64>
        }
    }
    
    return
  }
}
```

### Crash Log
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /home/username/projects/llvm-project/build_nvptx/bin/mlir-opt test.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
 #0 0x0000560068e40a52 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x2010a52)
 #1 0x0000560068e3d732 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fe667a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #3 0x000056006f1a9d65 __mlir_ods_local_type_constraint_MemRefOps1(mlir::Operation*, mlir::Type, llvm::StringRef, unsigned int) MemRefOps.cpp:0:0
 #4 0x000056006f1aaecd mlir::memref::StoreOp::verifyInvariantsImpl() (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x837aecd)
 #5 0x000056006f12cd9d mlir::Op<mlir::memref::StoreOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands<2u>::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::MemRefsNormalizable, mlir::MemorySpaceCastConsumerOpInterface::Trait, mlir::PromotableMemOpInterface::Trait, mlir::DestructurableAccessorOpInterface::Trait, mlir::MemoryEffectOpInterface::Trait>::verifyInvariants(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x82fcd9d)
 #6 0x000056006f143277 mlir::RegisteredOperationName::Model<mlir::memref::StoreOp>::verifyInvariants(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8313277)
 #7 0x000056006f79081c (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
 #8 0x000056006f792608 mlir::verify(mlir::Operation*, bool) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8962608)
 #9 0x000056006f455805 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8625805)
#10 0x000056006f4559c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#11 0x000056006f454a70 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8624a70)
#12 0x000056006f45532e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862532e)
#13 0x000056006f4559c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#14 0x000056006f45681b mlir::PassManager::run(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862681b)
#15 0x0000560068eed5ff performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#16 0x0000560068eeebee llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#17 0x000056006f7add08 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x897dd08)
#18 0x0000560068ee9bc7 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#19 0x0000560068eef288 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf288)
#20 0x0000560068eef4ff mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf4ff)
#21 0x0000560068d3677f main (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1f0677f)
#22 0x00007fe667a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#23 0x00007fe667a2a28b call_init ./csu/../csu/libc-start.c:128:20
#24 0x00007fe667a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#25 0x0000560068e1cf35 _start (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1fecf35)
Segmentation fault
```

### Used Command
```bash
/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt test.mlir \
  -gpu-kernel-outlining \
  --convert-scf-to-cf \
  --affine-loop-coalescing \
  --convert-gpu-to-nvvm \
  -reconcile-unrealized-casts \
  --nvvm-attach-target=chip=sm_75 \
  --gpu-module-to-binary \
  -gpu-to-llvm \
  --convert-cf-to-llvm \
  --convert-index-to-llvm \
  --convert-arith-to-llvm \
  --convert-math-to-llvm \
  --convert-func-to-llvm \
  --finalize-memref-to-llvm \
  -convert-nvvm-to-llvm \
  -reconcile-unrealized-casts
```

### Version
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (RJerrica)

<details>
Hi, I encountered a crash bug while lowering the following program with MLIR.

### Code example
```llvm
module {
  func.func @<!-- -->memory_op() {
    %c0 = arith.constant 0 : index
    %c17 = arith.constant 17 : index
    %c79 = arith.constant 79 : index
    %c64 = arith.constant 64 : index
    %gpu_alloc = gpu.alloc(%c79) : memref&lt;?xi64&gt;
    
    affine.for %i = 0 to 79 {
        %result = affine.for %j = 0 to 64 iter_args(%iter_arg = %gpu_alloc) -&gt; (memref&lt;?xi64&gt;) {
            %index = arith.remui %j, %c17 : index
            %j_i64 = arith.index_cast %j : index to i64
            memref.store %j_i64, %iter_arg[%index] : memref&lt;?xi64&gt;
            %dim_size = memref.dim %iter_arg, %c0 : memref&lt;?xi64&gt;
            affine.yield %iter_arg : memref&lt;?xi64&gt;
        }
    }
    
    return
  }
}
```

### Crash Log
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /home/username/projects/llvm-project/build_nvptx/bin/mlir-opt test.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
 #<!-- -->0 0x0000560068e40a52 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x2010a52)
 #<!-- -->1 0x0000560068e3d732 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007fe667a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->3 0x000056006f1a9d65 __mlir_ods_local_type_constraint_MemRefOps1(mlir::Operation*, mlir::Type, llvm::StringRef, unsigned int) MemRefOps.cpp:0:0
 #<!-- -->4 0x000056006f1aaecd mlir::memref::StoreOp::verifyInvariantsImpl() (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x837aecd)
 #<!-- -->5 0x000056006f12cd9d mlir::Op&lt;mlir::memref::StoreOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;2u&gt;::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::MemRefsNormalizable, mlir::MemorySpaceCastConsumerOpInterface::Trait, mlir::PromotableMemOpInterface::Trait, mlir::DestructurableAccessorOpInterface::Trait, mlir::MemoryEffectOpInterface::Trait&gt;::verifyInvariants(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x82fcd9d)
 #<!-- -->6 0x000056006f143277 mlir::RegisteredOperationName::Model&lt;mlir::memref::StoreOp&gt;::verifyInvariants(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8313277)
 #<!-- -->7 0x000056006f79081c (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
 #<!-- -->8 0x000056006f792608 mlir::verify(mlir::Operation*, bool) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8962608)
 #<!-- -->9 0x000056006f455805 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8625805)
#<!-- -->10 0x000056006f4559c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#<!-- -->11 0x000056006f454a70 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8624a70)
#<!-- -->12 0x000056006f45532e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862532e)
#<!-- -->13 0x000056006f4559c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#<!-- -->14 0x000056006f45681b mlir::PassManager::run(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862681b)
#<!-- -->15 0x0000560068eed5ff performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->16 0x0000560068eeebee llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->17 0x000056006f7add08 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x897dd08)
#<!-- -->18 0x0000560068ee9bc7 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->19 0x0000560068eef288 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf288)
#<!-- -->20 0x0000560068eef4ff mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf4ff)
#<!-- -->21 0x0000560068d3677f main (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1f0677f)
#<!-- -->22 0x00007fe667a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->23 0x00007fe667a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->24 0x00007fe667a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->25 0x0000560068e1cf35 _start (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1fecf35)
Segmentation fault
```

### Used Command
```bash
/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt test.mlir \
  -gpu-kernel-outlining \
  --convert-scf-to-cf \
  --affine-loop-coalescing \
  --convert-gpu-to-nvvm \
  -reconcile-unrealized-casts \
  --nvvm-attach-target=chip=sm_75 \
  --gpu-module-to-binary \
  -gpu-to-llvm \
  --convert-cf-to-llvm \
  --convert-index-to-llvm \
  --convert-arith-to-llvm \
  --convert-math-to-llvm \
  --convert-func-to-llvm \
  --finalize-memref-to-llvm \
  -convert-nvvm-to-llvm \
  -reconcile-unrealized-casts
```

### Version
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```
</details>


---

### Comment 2 - joker-eph

It crashes for me in `affine-loop-coalescing`, so a different backtrace than yours.

The repro is simply `mlir-opt    --affine-loop-coalescing  ` 


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-mlir-affine

Author: None (RJerrica)

<details>
Hi, I encountered a crash bug while lowering the following program with MLIR.

### Code example
```llvm
module {
  func.func @<!-- -->memory_op() {
    %c0 = arith.constant 0 : index
    %c17 = arith.constant 17 : index
    %c79 = arith.constant 79 : index
    %c64 = arith.constant 64 : index
    %gpu_alloc = gpu.alloc(%c79) : memref&lt;?xi64&gt;
    
    affine.for %i = 0 to 79 {
        %result = affine.for %j = 0 to 64 iter_args(%iter_arg = %gpu_alloc) -&gt; (memref&lt;?xi64&gt;) {
            %index = arith.remui %j, %c17 : index
            %j_i64 = arith.index_cast %j : index to i64
            memref.store %j_i64, %iter_arg[%index] : memref&lt;?xi64&gt;
            %dim_size = memref.dim %iter_arg, %c0 : memref&lt;?xi64&gt;
            affine.yield %iter_arg : memref&lt;?xi64&gt;
        }
    }
    
    return
  }
}
```

### Crash Log
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: /home/username/projects/llvm-project/build_nvptx/bin/mlir-opt test.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_75 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
 #<!-- -->0 0x0000560068e40a52 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x2010a52)
 #<!-- -->1 0x0000560068e3d732 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007fe667a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->3 0x000056006f1a9d65 __mlir_ods_local_type_constraint_MemRefOps1(mlir::Operation*, mlir::Type, llvm::StringRef, unsigned int) MemRefOps.cpp:0:0
 #<!-- -->4 0x000056006f1aaecd mlir::memref::StoreOp::verifyInvariantsImpl() (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x837aecd)
 #<!-- -->5 0x000056006f12cd9d mlir::Op&lt;mlir::memref::StoreOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;2u&gt;::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::MemRefsNormalizable, mlir::MemorySpaceCastConsumerOpInterface::Trait, mlir::PromotableMemOpInterface::Trait, mlir::DestructurableAccessorOpInterface::Trait, mlir::MemoryEffectOpInterface::Trait&gt;::verifyInvariants(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x82fcd9d)
 #<!-- -->6 0x000056006f143277 mlir::RegisteredOperationName::Model&lt;mlir::memref::StoreOp&gt;::verifyInvariants(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8313277)
 #<!-- -->7 0x000056006f79081c (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
 #<!-- -->8 0x000056006f792608 mlir::verify(mlir::Operation*, bool) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8962608)
 #<!-- -->9 0x000056006f455805 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8625805)
#<!-- -->10 0x000056006f4559c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#<!-- -->11 0x000056006f454a70 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x8624a70)
#<!-- -->12 0x000056006f45532e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862532e)
#<!-- -->13 0x000056006f4559c9 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x86259c9)
#<!-- -->14 0x000056006f45681b mlir::PassManager::run(mlir::Operation*) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x862681b)
#<!-- -->15 0x0000560068eed5ff performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->16 0x0000560068eeebee llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->17 0x000056006f7add08 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x897dd08)
#<!-- -->18 0x0000560068ee9bc7 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->19 0x0000560068eef288 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf288)
#<!-- -->20 0x0000560068eef4ff mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x20bf4ff)
#<!-- -->21 0x0000560068d3677f main (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1f0677f)
#<!-- -->22 0x00007fe667a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->23 0x00007fe667a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->24 0x00007fe667a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->25 0x0000560068e1cf35 _start (/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt+0x1fecf35)
Segmentation fault
```

### Used Command
```bash
/home/username/projects/llvm-project/build_nvptx/bin/mlir-opt test.mlir \
  -gpu-kernel-outlining \
  --convert-scf-to-cf \
  --affine-loop-coalescing \
  --convert-gpu-to-nvvm \
  -reconcile-unrealized-casts \
  --nvvm-attach-target=chip=sm_75 \
  --gpu-module-to-binary \
  -gpu-to-llvm \
  --convert-cf-to-llvm \
  --convert-index-to-llvm \
  --convert-arith-to-llvm \
  --convert-math-to-llvm \
  --convert-func-to-llvm \
  --finalize-memref-to-llvm \
  -convert-nvvm-to-llvm \
  -reconcile-unrealized-casts
```

### Version
```bash
mlir-opt --version
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```
</details>


---

