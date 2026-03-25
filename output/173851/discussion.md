# mlir-opt crashes on vector<index> due to invalid getIntOrFloatBitWidth call

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/173851
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-01-20T21:12:35Z

## Body

## Description
The --convert-vector-to-xegpu pass crashes when lowering vector operations whose element type is index. The pass assumes all vector element types are integers or floats and does not guard against index element types before querying bitwidth.

If the `vector.store %0, %alloc[%c0, %c0] : memref<16x16xindex>, vector<16xindex>` operation is commented out, the crash no longer occurs.

## Reproduce
mlir-opt version: 21.1.8
```mlir
module {
  func.func @main() {
    %0 = vector.step : vector<16xindex>
    %1 = vector.constant_mask [16] : vector<16xi1>
    %alloc = memref.alloc() : memref<16x16xindex>
    %c0 = arith.constant 0 : index
    vector.store %0, %alloc[%c0, %c0] : memref<16x16xindex>, vector<16xindex>
    vector.maskedstore %alloc[%c0, %c0], %1, %0 : memref<16x16xindex>, vector<16xi1>, vector<16xindex>
    return
  }
}
````
```
mlir-opt --convert-vector-to-xegpu crash.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/mlir/lib/IR/Types.cpp:123: unsigned int mlir::Type::getIntOrFloatBitWidth() const: Assertion `isIntOrFloat() && "only integers and floats have a bitwidth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --convert-vector-to-xegpu /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250410_96d2_5.mlir
 #0 0x0000563f29d98612 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #1 0x0000563f29d94d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #2 0x0000563f29d94ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000072dfe0a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000072dfe0a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000072dfe0a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000072dfe0a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000072dfe0a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000072dfe0a288ff abort ./stdlib/abort.c:81:7
 #9 0x000072dfe0a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000072dfe0a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000563f2d797b76 mlir::Type::getIntOrFloatBitWidth() const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5aa0b76)
#12 0x0000563f2ce00610 mlir::xegpu::TensorDescType::verify(llvm::function_ref<mlir::InFlightDiagnostic ()>, llvm::ArrayRef<long>, mlir::Type, mlir::Attribute, mlir::Attribute) (.constprop.0) XeGPUDialect.cpp:0:0
#13 0x0000563f2ce03fe4 mlir::xegpu::TensorDescType::get(llvm::ArrayRef<long>, mlir::Type, int, bool, mlir::xegpu::MemorySpace, mlir::Attribute) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x510cfe4)
#14 0x0000563f2d486a15 (anonymous namespace)::StoreLowering::matchAndRewrite(mlir::vector::StoreOp, mlir::PatternRewriter&) const VectorToXeGPU.cpp:0:0
#15 0x0000563f306ba08a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x89c308a)
#16 0x0000563f2d60a537 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#17 0x0000563f2d60d0fe mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x59160fe)
#18 0x0000563f2d4836f9 (anonymous namespace)::ConvertVectorToXeGPUPass::runOnOperation() VectorToXeGPU.cpp:0:0
#19 0x0000563f2d54d07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#20 0x0000563f2d54d4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#21 0x0000563f2d54e523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#22 0x0000563f2d53e0a7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#23 0x0000563f2d53e9c6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#24 0x0000563f2d53eb8c llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#25 0x0000563f2d65424e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#26 0x0000563f2d537080 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#27 0x0000563f2d53ecf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#28 0x0000563f2d53f1ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#29 0x0000563f29cb0577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#30 0x000072dfe0a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x000072dfe0a2a28b call_init ./csu/../csu/libc-start.c:128:20
#32 0x000072dfe0a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#33 0x0000563f29d69665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
## Description
The --convert-vector-to-xegpu pass crashes when lowering vector operations whose element type is index. The pass assumes all vector element types are integers or floats and does not guard against index element types before querying bitwidth.

If the `vector.store %0, %alloc[%c0, %c0] : memref&lt;16x16xindex&gt;, vector&lt;16xindex&gt;` operation is commented out, the crash no longer occurs.

## Reproduce
mlir-opt version: 21.1.8
```mlir
module {
  func.func @<!-- -->main() {
    %0 = vector.step : vector&lt;16xindex&gt;
    %1 = vector.constant_mask [16] : vector&lt;16xi1&gt;
    %alloc = memref.alloc() : memref&lt;16x16xindex&gt;
    %c0 = arith.constant 0 : index
    vector.store %0, %alloc[%c0, %c0] : memref&lt;16x16xindex&gt;, vector&lt;16xindex&gt;
    vector.maskedstore %alloc[%c0, %c0], %1, %0 : memref&lt;16x16xindex&gt;, vector&lt;16xi1&gt;, vector&lt;16xindex&gt;
    return
  }
}
````
```
mlir-opt --convert-vector-to-xegpu crash.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/mlir/lib/IR/Types.cpp:123: unsigned int mlir::Type::getIntOrFloatBitWidth() const: Assertion `isIntOrFloat() &amp;&amp; "only integers and floats have a bitwidth"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --convert-vector-to-xegpu /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250410_96d2_5.mlir
 #<!-- -->0 0x0000563f29d98612 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #<!-- -->1 0x0000563f29d94d7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #<!-- -->2 0x0000563f29d94ecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000072dfe0a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000072dfe0a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000072dfe0a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000072dfe0a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000072dfe0a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000072dfe0a288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000072dfe0a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000072dfe0a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000563f2d797b76 mlir::Type::getIntOrFloatBitWidth() const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5aa0b76)
#<!-- -->12 0x0000563f2ce00610 mlir::xegpu::TensorDescType::verify(llvm::function_ref&lt;mlir::InFlightDiagnostic ()&gt;, llvm::ArrayRef&lt;long&gt;, mlir::Type, mlir::Attribute, mlir::Attribute) (.constprop.0) XeGPUDialect.cpp:0:0
#<!-- -->13 0x0000563f2ce03fe4 mlir::xegpu::TensorDescType::get(llvm::ArrayRef&lt;long&gt;, mlir::Type, int, bool, mlir::xegpu::MemorySpace, mlir::Attribute) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x510cfe4)
#<!-- -->14 0x0000563f2d486a15 (anonymous namespace)::StoreLowering::matchAndRewrite(mlir::vector::StoreOp, mlir::PatternRewriter&amp;) const VectorToXeGPU.cpp:0:0
#<!-- -->15 0x0000563f306ba08a mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x89c308a)
#<!-- -->16 0x0000563f2d60a537 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#<!-- -->17 0x0000563f2d60d0fe mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x59160fe)
#<!-- -->18 0x0000563f2d4836f9 (anonymous namespace)::ConvertVectorToXeGPUPass::runOnOperation() VectorToXeGPU.cpp:0:0
#<!-- -->19 0x0000563f2d54d07e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#<!-- -->20 0x0000563f2d54d4c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#<!-- -->21 0x0000563f2d54e523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#<!-- -->22 0x0000563f2d53e0a7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->23 0x0000563f2d53e9c6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#<!-- -->24 0x0000563f2d53eb8c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->25 0x0000563f2d65424e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#<!-- -->26 0x0000563f2d537080 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#<!-- -->27 0x0000563f2d53ecf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#<!-- -->28 0x0000563f2d53f1ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#<!-- -->29 0x0000563f29cb0577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#<!-- -->30 0x000072dfe0a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->31 0x000072dfe0a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->32 0x000072dfe0a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->33 0x0000563f29d69665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```

</details>


---

