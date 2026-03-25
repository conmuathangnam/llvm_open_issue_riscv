# mlir-opt --convert-scf-to-openmp crashes on scf.parallel reduction with vector element type

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/173860
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-01-13T10:06:30Z

## Body

## Description
mlir-opt --convert-scf-to-openmp crashes when lowering an scf.parallel that performs a reduction on a vector element type (vector<2xi1>). 
## Reproduce
mlir-opt version: 21.1.8
location: If the `scf.parallel` operation is commented out, the crash no longer occurs.
```mlir
module {
  func.func @main() {
    %0 = vector.constant_mask [1] : vector<2xi1>
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c2 = arith.constant 2 : index
    %result = scf.parallel (%arg0) = (%c0) to (%c2) step (%c1) init(%0) -> vector<2xi1> {
      %val = vector.constant_mask [1] : vector<2xi1>
      scf.reduce (%val : vector<2xi1>) {
        ^bb0(%lhs : vector<2xi1>, %rhs: vector<2xi1>):
        %res = arith.ori %lhs, %rhs : vector<2xi1>
        scf.reduce.return %res : vector<2xi1>
      }
    }
    return
  }
}
```
```
mlir-opt --convert-scf-to-openmp test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/mlir/lib/IR/Types.cpp:123: unsigned int mlir::Type::getIntOrFloatBitWidth() const: Assertion `isIntOrFloat() && "only integers and floats have a bitwidth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --convert-scf-to-openmp /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250327_ff00_5.mlir
 #0 0x0000556b73ffa612 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #1 0x0000556b73ff6d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #2 0x0000556b73ff6ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000070c3fb645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000070c3fb69eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000070c3fb69eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000070c3fb69eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000070c3fb64527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000070c3fb6288ff abort ./stdlib/abort.c:81:7
 #9 0x000070c3fb62881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000070c3fb63b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000556b779f9b76 mlir::Type::getIntOrFloatBitWidth() const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5aa0b76)
#12 0x0000556b7790615b mlir::Builder::getIntegerAttr(mlir::Type, long) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x59ad15b)
#13 0x0000556b7754cba9 declareReduction(mlir::PatternRewriter&, mlir::scf::ReduceOp, long) SCFToOpenMP.cpp:0:0
#14 0x0000556b7754db9a (anonymous namespace)::ParallelOpLowering::matchAndRewrite(mlir::scf::ParallelOp, mlir::PatternRewriter&) const SCFToOpenMP.cpp:0:0
#15 0x0000556b7a91c08a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x89c308a)
#16 0x0000556b7785a6fe (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&) DialectConversion.cpp:0:0
#17 0x0000556b7785acae mlir::OperationConverter::convert(mlir::ConversionPatternRewriter&, mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5901cae)
#18 0x0000556b7785f57c mlir::OperationConverter::convertOperations(llvm::ArrayRef<mlir::Operation*>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x590657c)
#19 0x0000556b77860bba applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#20 0x0000556b77860cf2 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5907cf2)
#21 0x0000556b7754733c (anonymous namespace)::applyPatterns(mlir::ModuleOp, unsigned int) SCFToOpenMP.cpp:0:0
#22 0x0000556b775484ea (anonymous namespace)::SCFToOpenMPPass::runOnOperation() SCFToOpenMP.cpp:0:0
#23 0x0000556b777af07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#24 0x0000556b777af4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#25 0x0000556b777b0523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#26 0x0000556b777a00a7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#27 0x0000556b777a09c6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#28 0x0000556b777a0b8c llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#29 0x0000556b778b624e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#30 0x0000556b77799080 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#31 0x0000556b777a0cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#32 0x0000556b777a11ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#33 0x0000556b73f12577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#34 0x000070c3fb62a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#35 0x000070c3fb62a28b call_init ./csu/../csu/libc-start.c:128:20
#36 0x000070c3fb62a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#37 0x0000556b73fcb665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
## Description
mlir-opt --convert-scf-to-openmp crashes when lowering an scf.parallel that performs a reduction on a vector element type (vector&lt;2xi1&gt;). 
## Reproduce
mlir-opt version: 21.1.8
location: If the `scf.parallel` operation is commented out, the crash no longer occurs.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = vector.constant_mask [1] : vector&lt;2xi1&gt;
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c2 = arith.constant 2 : index
    %result = scf.parallel (%arg0) = (%c0) to (%c2) step (%c1) init(%0) -&gt; vector&lt;2xi1&gt; {
      %val = vector.constant_mask [1] : vector&lt;2xi1&gt;
      scf.reduce (%val : vector&lt;2xi1&gt;) {
        ^bb0(%lhs : vector&lt;2xi1&gt;, %rhs: vector&lt;2xi1&gt;):
        %res = arith.ori %lhs, %rhs : vector&lt;2xi1&gt;
        scf.reduce.return %res : vector&lt;2xi1&gt;
      }
    }
    return
  }
}
```
```
mlir-opt --convert-scf-to-openmp test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/mlir/lib/IR/Types.cpp:123: unsigned int mlir::Type::getIntOrFloatBitWidth() const: Assertion `isIntOrFloat() &amp;&amp; "only integers and floats have a bitwidth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --convert-scf-to-openmp /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250327_ff00_5.mlir
 #<!-- -->0 0x0000556b73ffa612 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #<!-- -->1 0x0000556b73ff6d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #<!-- -->2 0x0000556b73ff6ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000070c3fb645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000070c3fb69eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000070c3fb69eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000070c3fb69eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000070c3fb64527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000070c3fb6288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000070c3fb62881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000070c3fb63b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000556b779f9b76 mlir::Type::getIntOrFloatBitWidth() const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5aa0b76)
#<!-- -->12 0x0000556b7790615b mlir::Builder::getIntegerAttr(mlir::Type, long) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x59ad15b)
#<!-- -->13 0x0000556b7754cba9 declareReduction(mlir::PatternRewriter&amp;, mlir::scf::ReduceOp, long) SCFToOpenMP.cpp:0:0
#<!-- -->14 0x0000556b7754db9a (anonymous namespace)::ParallelOpLowering::matchAndRewrite(mlir::scf::ParallelOp, mlir::PatternRewriter&amp;) const SCFToOpenMP.cpp:0:0
#<!-- -->15 0x0000556b7a91c08a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x89c308a)
#<!-- -->16 0x0000556b7785a6fe (anonymous namespace)::OperationLegalizer::legalize(mlir::Operation*, mlir::ConversionPatternRewriter&amp;) DialectConversion.cpp:0:0
#<!-- -->17 0x0000556b7785acae mlir::OperationConverter::convert(mlir::ConversionPatternRewriter&amp;, mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5901cae)
#<!-- -->18 0x0000556b7785f57c mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x590657c)
#<!-- -->19 0x0000556b77860bba applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->20 0x0000556b77860cf2 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5907cf2)
#<!-- -->21 0x0000556b7754733c (anonymous namespace)::applyPatterns(mlir::ModuleOp, unsigned int) SCFToOpenMP.cpp:0:0
#<!-- -->22 0x0000556b775484ea (anonymous namespace)::SCFToOpenMPPass::runOnOperation() SCFToOpenMP.cpp:0:0
#<!-- -->23 0x0000556b777af07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#<!-- -->24 0x0000556b777af4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#<!-- -->25 0x0000556b777b0523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#<!-- -->26 0x0000556b777a00a7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->27 0x0000556b777a09c6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#<!-- -->28 0x0000556b777a0b8c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->29 0x0000556b778b624e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#<!-- -->30 0x0000556b77799080 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#<!-- -->31 0x0000556b777a0cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#<!-- -->32 0x0000556b777a11ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#<!-- -->33 0x0000556b73f12577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#<!-- -->34 0x000070c3fb62a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->35 0x000070c3fb62a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x000070c3fb62a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->37 0x0000556b73fcb665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```
</details>


---

