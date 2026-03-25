# [mlir][sparse] Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.

**Author:** secona
**URL:** https://github.com/llvm/llvm-project/issues/183776
**Status:** Closed
**Labels:** mlir:sparsetensor, crash
**Closed Date:** 2026-03-05T11:27:10Z

## Body

Commit: https://github.com/llvm/llvm-project/commit/2265d3240f23c1c6ff7b4d0fa2711a9ed36fdc8a

Command: `mlir-opt -sparse-tensor-codegen -sparse-assembler input.mlir`

```mlir
#CSR = #sparse_tensor.encoding<{ map = (d0, d1) -> (d0: dense, d1: compressed) }>

func.func @tensor_add(%arg0: tensor<32x32xf32, #CSR>, 
                          %arg1: tensor<32x32xf32, #CSR>) -> tensor<32x32xf32> {
  %cst = arith.constant 0.0 : f32
  %init = tensor.empty() : tensor<32x32xf32>
  %out = linalg.fill ins(%cst : f32) outs(%init : tensor<32x32xf32>) -> tensor<32x32xf32>

  %3 = linalg.add
      ins(%arg0, %arg1 : tensor<32x32xf32, #CSR>, tensor<32x32xf32, #CSR>)
      outs(%out : tensor<32x32xf32>) -> tensor<32x32xf32>

  return %3 : tensor<32x32xf32>
}
```

Stack dump:
```
mlir-opt: /home/guest/llvm-project/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From &) [To = mlir::RankedTensorType, From = mlir::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: bin/mlir-opt -sparse-tensor-codegen -sparse-assembler sparse_tensor_add.mlir
 #0 0x000055c56cf6a8ab llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (bin/mlir-opt+0x1d698ab)
 #1 0x000055c56cf673aa llvm::sys::RunSignalHandlers() (bin/mlir-opt+0x1d663aa)
 #2 0x000055c56cf6b9fa SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007fe5100419c0 __restore_rt (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x419c0)
 #4 0x00007fe51009caac __pthread_kill_implementation (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x9caac)
 #5 0x00007fe51004190e gsignal (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x4190e)
 #6 0x00007fe510028942 abort (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x28942)
 #7 0x00007fe51002885e _nl_load_domain.cold (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2885e)
 #8 0x00007fe5100396f7 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x396f7)
 #9 0x000055c56dc39db5 convTypes(bool&, mlir::TypeRange, llvm::SmallVectorImpl<mlir::Type>&, llvm::SmallVectorImpl<mlir::Type>*, bool) SparseAssembler.cpp:0:0
#10 0x000055c56dc39308 (anonymous namespace)::SparseFuncAssembler::matchAndRewrite(mlir::func::FuncOp, mlir::PatternRewriter&) const SparseAssembler.cpp:0:0
#11 0x000055c574ebbe4e void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_0>(long) PatternApplicator.cpp:0:0
#12 0x000055c574eb7fb3 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (bin/mlir-opt+0x9cb6fb3)
#13 0x000055c574e918cd (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#14 0x000055c574e8dd09 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) (bin/mlir-opt+0x9c8cd09)
#15 0x000055c56dbd8081 (anonymous namespace)::SparseAssembler::runOnOperation() SparseTensorPasses.cpp:0:0
#16 0x000055c574f20a83 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (bin/mlir-opt+0x9d1fa83)
#17 0x000055c574f21928 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (bin/mlir-opt+0x9d20928)
#18 0x000055c574f296ff mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (bin/mlir-opt+0x9d286ff)
#19 0x000055c574f28a18 mlir::PassManager::run(mlir::Operation*) (bin/mlir-opt+0x9d27a18)
#20 0x000055c56d02c23d performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#21 0x000055c56d02b9ff llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#22 0x000055c57529d6c4 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (bin/mlir-opt+0xa09c6c4)
#23 0x000055c56d0200bb mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (bin/mlir-opt+0x1e1f0bb)
#24 0x000055c56d02046b mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (bin/mlir-opt+0x1e1f46b)
#25 0x000055c56d020708 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (bin/mlir-opt+0x1e1f708)
#26 0x000055c56cf4e054 main (bin/mlir-opt+0x1d4d054)
#27 0x00007fe51002a4d8 __libc_start_call_main (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a4d8)
#28 0x00007fe51002a59b __libc_start_main@GLIBC_2.2.5 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a59b)
#29 0x000055c56cf4db25 _start (bin/mlir-opt+0x1d4cb25)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Vito Secona (secona)

<details>
Commit: https://github.com/llvm/llvm-project/commit/2265d3240f23c1c6ff7b4d0fa2711a9ed36fdc8a

Command: `mlir-opt -sparse-tensor-codegen -sparse-assembler input.mlir`

```mlir
#CSR = #sparse_tensor.encoding&lt;{ map = (d0, d1) -&gt; (d0: dense, d1: compressed) }&gt;

func.func @<!-- -->tensor_add(%arg0: tensor&lt;32x32xf32, #CSR&gt;, 
                          %arg1: tensor&lt;32x32xf32, #CSR&gt;) -&gt; tensor&lt;32x32xf32&gt; {
  %cst = arith.constant 0.0 : f32
  %init = tensor.empty() : tensor&lt;32x32xf32&gt;
  %out = linalg.fill ins(%cst : f32) outs(%init : tensor&lt;32x32xf32&gt;) -&gt; tensor&lt;32x32xf32&gt;

  %3 = linalg.add
      ins(%arg0, %arg1 : tensor&lt;32x32xf32, #CSR&gt;, tensor&lt;32x32xf32, #CSR&gt;)
      outs(%out : tensor&lt;32x32xf32&gt;) -&gt; tensor&lt;32x32xf32&gt;

  return %3 : tensor&lt;32x32xf32&gt;
}
```

Stack dump:
```
mlir-opt: /home/guest/llvm-project/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From &amp;) [To = mlir::RankedTensorType, From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: bin/mlir-opt -sparse-tensor-codegen -sparse-assembler sparse_tensor_add.mlir
 #<!-- -->0 0x000055c56cf6a8ab llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (bin/mlir-opt+0x1d698ab)
 #<!-- -->1 0x000055c56cf673aa llvm::sys::RunSignalHandlers() (bin/mlir-opt+0x1d663aa)
 #<!-- -->2 0x000055c56cf6b9fa SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007fe5100419c0 __restore_rt (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x419c0)
 #<!-- -->4 0x00007fe51009caac __pthread_kill_implementation (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x9caac)
 #<!-- -->5 0x00007fe51004190e gsignal (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x4190e)
 #<!-- -->6 0x00007fe510028942 abort (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x28942)
 #<!-- -->7 0x00007fe51002885e _nl_load_domain.cold (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2885e)
 #<!-- -->8 0x00007fe5100396f7 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x396f7)
 #<!-- -->9 0x000055c56dc39db5 convTypes(bool&amp;, mlir::TypeRange, llvm::SmallVectorImpl&lt;mlir::Type&gt;&amp;, llvm::SmallVectorImpl&lt;mlir::Type&gt;*, bool) SparseAssembler.cpp:0:0
#<!-- -->10 0x000055c56dc39308 (anonymous namespace)::SparseFuncAssembler::matchAndRewrite(mlir::func::FuncOp, mlir::PatternRewriter&amp;) const SparseAssembler.cpp:0:0
#<!-- -->11 0x000055c574ebbe4e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_0&gt;(long) PatternApplicator.cpp:0:0
#<!-- -->12 0x000055c574eb7fb3 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (bin/mlir-opt+0x9cb6fb3)
#<!-- -->13 0x000055c574e918cd (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#<!-- -->14 0x000055c574e8dd09 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) (bin/mlir-opt+0x9c8cd09)
#<!-- -->15 0x000055c56dbd8081 (anonymous namespace)::SparseAssembler::runOnOperation() SparseTensorPasses.cpp:0:0
#<!-- -->16 0x000055c574f20a83 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (bin/mlir-opt+0x9d1fa83)
#<!-- -->17 0x000055c574f21928 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (bin/mlir-opt+0x9d20928)
#<!-- -->18 0x000055c574f296ff mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (bin/mlir-opt+0x9d286ff)
#<!-- -->19 0x000055c574f28a18 mlir::PassManager::run(mlir::Operation*) (bin/mlir-opt+0x9d27a18)
#<!-- -->20 0x000055c56d02c23d performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->21 0x000055c56d02b9ff llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->22 0x000055c57529d6c4 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (bin/mlir-opt+0xa09c6c4)
#<!-- -->23 0x000055c56d0200bb mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (bin/mlir-opt+0x1e1f0bb)
#<!-- -->24 0x000055c56d02046b mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (bin/mlir-opt+0x1e1f46b)
#<!-- -->25 0x000055c56d020708 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (bin/mlir-opt+0x1e1f708)
#<!-- -->26 0x000055c56cf4e054 main (bin/mlir-opt+0x1d4d054)
#<!-- -->27 0x00007fe51002a4d8 __libc_start_call_main (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a4d8)
#<!-- -->28 0x00007fe51002a59b __libc_start_main@<!-- -->GLIBC_2.2.5 (/nix/store/xx7cm72qy2c0643cm1ipngd87aqwkcdp-glibc-2.40-66/lib/libc.so.6+0x2a59b)
#<!-- -->29 0x000055c56cf4db25 _start (bin/mlir-opt+0x1d4cb25)
```
</details>


---

