# [mlir] null pointer SEGV in mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp

**Author:** YuanchengJiang
**URL:** https://github.com/llvm/llvm-project/issues/162772
**Status:** Closed
**Labels:** mlir:linalg, crash
**Closed Date:** 2025-10-15T03:06:03Z

## Body

PoC:
```
#trait = {
    indexing_maps = [
      affine_map<(m, n, k) -> (m, k)>,
      affine_map<(m, n, k) -> (k, n)>,
      affine_map<(m, n, k) -> (m, n)>
    ],
    iterator_types = ["parallel", "parallel", "reduction"] 
  }
  func.func @B_hoist_linalg_ops_div_by_zero(%a : tensor<128x128xi32>, 
                              %b : tensor<128x128xi32>, 
                              %c: tensor<128x128xi32>,
                              %lb : index,
                              %ub : index,
                              %step : index,
                              %output : tensor<?x128xi32>) -> tensor<?x128xi32> {
    %final = 
    scf.for %i = %lb to %ub step %step iter_args(%acc = %output) 
                                              -> tensor<?x128xi32> {
      %compute = linalg.generic #trait
                 ins(%a, %b : tensor<128x128xi32>, tensor<128x128xi32>) 
                 outs(%c : tensor<128x128xi32>) {
      ^bb0(%in : i32, %in2 : i32, %in3 : i32):
        linalg.yield %in3 : i32
      } -> tensor<128x128xi32>
      %newacc = tensor.insert_slice %compute into 
                                    %output[%i, 0][128, 128][1, 1] 
                                    : tensor<128x128xi32> into tensor<?x128xi32>
      scf.yield %newacc : tensor<?x128xi32>
    }
    func.return %final : tensor<?x128xi32>
  }
```

reproduce: `mlir-opt ./test.mlir -split-input-file --linalg-specialize-generic-ops`

UBSan:
```
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:318:20 in 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: llvm-mlir-build/bin/mlir-opt /tmp/mlirfuzz_run_ee5lfpd3/fused.mlir -split-input-file --linalg-specialize-generic-ops
 #0 0x00005c4ba94af69b backtrace (llvm-mlir-build/bin/mlir-opt+0x83bd69b)
 #1 0x00005c4ba957cad7 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:838:8
 #2 0x00005c4ba9576dbf llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #3 0x00005c4ba957fe16 SignalHandler(int, siginfo_t*, void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x000072e2df10c520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x000072e2df1609fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x000072e2df1609fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x000072e2df1609fc pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x000072e2df10c476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x000072e2df0f27f3 abort ./stdlib/abort.c:81:7
#10 0x00005c4ba951ad07 (llvm-mlir-build/bin/mlir-opt+0x8428d07)
#11 0x00005c4ba9518ba1 (llvm-mlir-build/bin/mlir-opt+0x8426ba1)
#12 0x00005c4ba952c8ec __ubsan::ScopedReport::~ScopedReport() (llvm-mlir-build/bin/mlir-opt+0x843a8ec)
#13 0x00005c4ba952d45c handleTypeMismatchImpl(__ubsan::TypeMismatchData*, unsigned long, __ubsan::ReportOptions) ubsan_handlers.cpp.o:0:0
#14 0x00005c4ba952d4ee __ubsan_handle_type_mismatch_v1_abort (llvm-mlir-build/bin/mlir-opt+0x843b4ee)
#15 0x00005c4bb7a63529 mlir::linalg::detail::isContractionBody(mlir::Block&, llvm::function_ref<bool (mlir::Operation*, mlir::Operation*)>, llvm::raw_ostream&) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:336:40
#16 0x00005c4bb7a65eb2 bool isContractionBody<mlir::arith::MulFOp, mlir::arith::AddFOp, mlir::arith::MulIOp, mlir::arith::AddIOp, mlir::complex::MulOp, mlir::complex::AddOp, mlir::arith::AndIOp, mlir::arith::OrIOp>(mlir::Block&) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:380:3
#17 0x00005c4bb7a65eb2 mlir::linalg::detail::isContractionInterfaceImpl(mlir::Operation*, mlir::linalg::ContractionDimensions*) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:527:8
#18 0x00005c4bb7a662e7 mlir::linalg::isaContractionOpInterface(mlir::linalg::LinalgOp) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:569:64
#19 0x00005c4bb6b172be mlir::linalg::specializeGenericOp(mlir::RewriterBase&, mlir::linalg::GenericOp) llvm-project/mlir/lib/Dialect/Linalg/Transforms/Specialize.cpp:316:7
#20 0x00005c4bb6b1de92 std::_Optional_base_impl<mlir::linalg::LinalgOp, std::_Optional_base<mlir::linalg::LinalgOp, true, true>>::_M_is_engaged() const /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/optional:433:58
#21 0x00005c4bb6b1de92 std::optional<mlir::linalg::LinalgOp>::has_value() const /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/optional:945:22
#22 0x00005c4bb6b1de92 llvm::FailureOr<mlir::linalg::LinalgOp>::operator llvm::LogicalResult() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:93:51
#23 0x00005c4bb6b1de92 llvm::FailureOr<mlir::linalg::GenericOp>::FailureOr<mlir::linalg::LinalgOp, (void*)0>(llvm::FailureOr<mlir::linalg::LinalgOp> const&) llvm-project/llvm/include/llvm/Support/LogicalResult.h:90:33
#24 0x00005c4bb6b1de92 mlir::linalg::LinalgSpecializationPattern::returningMatchAndRewrite(mlir::linalg::GenericOp, mlir::PatternRewriter&) const llvm-project/mlir/include/mlir/Dialect/Linalg/Transforms/Transforms.h:1616:12
#25 0x00005c4bb6b1de92 mlir::linalg::LinalgSpecializationPattern::matchAndRewrite(mlir::linalg::GenericOp, mlir::PatternRewriter&) const llvm-project/mlir/include/mlir/Dialect/Linalg/Transforms/Transforms.h:1621:12
#26 0x00005c4bbbddda9e mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_6::operator()() const llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:223:13
#27 0x00005c4bbbddda9e void llvm::function_ref<void ()>::callback_fn<mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::$_6>(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#28 0x00005c4bbbdd307e mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#29 0x00005c4bbbd6c5b8 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() llvm-project/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:620:17
#30 0x00005c4bbbd6a537 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_14::operator()() const llvm-project/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:890:11
#31 0x00005c4bbbd6a537 void llvm::function_ref<void ()>::callback_fn<(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &&::$_14>(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#32 0x00005c4bbbd64892 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) && llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#33 0x00005c4bbbd64892 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) llvm-project/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:935:47
#34 0x00005c4ba97d5a73 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) llvm-project/mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#35 0x00005c4bb6b1d39d (anonymous namespace)::LinalgSpecializeGenericOpsPass::runOnOperation() llvm-project/mlir/lib/Dialect/Linalg/Transforms/Specialize.cpp:338:14
#36 0x00005c4bbbee5f44 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_44::operator()() const llvm-project/mlir/lib/Pass/Pass.cpp:610:22
#37 0x00005c4bbbee5f44 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_44>(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#38 0x00005c4bbbecf11a mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) llvm-project/mlir/lib/Pass/Pass.cpp:603:3
#39 0x00005c4bbbed10ec llvm::LogicalResult::failed() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#40 0x00005c4bbbed10ec llvm::failed(llvm::LogicalResult) llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#41 0x00005c4bbbed10ec mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) llvm-project/mlir/lib/Pass/Pass.cpp:682:9
#42 0x00005c4bbbedd238 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#43 0x00005c4bbbedbdb4 mlir::PassManager::run(mlir::Operation*) llvm-project/mlir/lib/Pass/Pass.cpp:0:60
#44 0x00005c4ba9737604 llvm::LogicalResult::failed() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#45 0x00005c4ba9737604 llvm::failed(llvm::LogicalResult) llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#46 0x00005c4ba9737604 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:568:7
#47 0x00005c4ba9736517 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:644:12
#48 0x00005c4ba9736517 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:733:12
#49 0x00005c4ba9736517 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#50 0x00005c4bbc6faa64 std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#51 0x00005c4bbc6faa64 llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#52 0x00005c4bbc6fb526 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const llvm-project/mlir/lib/Support/ToolUtilities.cpp:94:13
#53 0x00005c4bbc6fa447 void llvm::interleave<llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&)::'lambda'(), void>(llvm::SmallVector<llvm::StringRef, 8u>, llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) llvm-project/llvm/include/llvm/ADT/STLExtras.h:2201:24
#54 0x00005c4bbc6fa447 void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&) llvm-project/llvm/include/llvm/ADT/STLExtras.h:2221:3
#55 0x00005c4bbc6fa447 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) llvm-project/mlir/lib/Support/ToolUtilities.cpp:97:3
#56 0x00005c4ba971b432 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:26
#57 0x00005c4ba971be6e llvm::LogicalResult::failed() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#58 0x00005c4ba971be6e llvm::failed(llvm::LogicalResult) llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#59 0x00005c4ba971be6e mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:7
#60 0x00005c4ba971c5c3 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:800:10
#61 0x00005c4ba95343cf llvm::LogicalResult::succeeded() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#62 0x00005c4ba95343cf mlir::asMainReturnCode(llvm::LogicalResult) llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:399:12
#63 0x00005c4ba95343cf main llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:343:10
#64 0x000072e2df0f3d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#65 0x000072e2df0f3e40 call_init ./csu/../csu/libc-start.c:128:20
#66 0x000072e2df0f3e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#67 0x00005c4ba9473b65 _start (llvm-mlir-build/bin/mlir-opt+0x8381b65)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Yuancheng Jiang (YuanchengJiang)

<details>
PoC:
```
#trait = {
    indexing_maps = [
      affine_map&lt;(m, n, k) -&gt; (m, k)&gt;,
      affine_map&lt;(m, n, k) -&gt; (k, n)&gt;,
      affine_map&lt;(m, n, k) -&gt; (m, n)&gt;
    ],
    iterator_types = ["parallel", "parallel", "reduction"] 
  }
  func.func @<!-- -->B_hoist_linalg_ops_div_by_zero(%a : tensor&lt;128x128xi32&gt;, 
                              %b : tensor&lt;128x128xi32&gt;, 
                              %c: tensor&lt;128x128xi32&gt;,
                              %lb : index,
                              %ub : index,
                              %step : index,
                              %output : tensor&lt;?x128xi32&gt;) -&gt; tensor&lt;?x128xi32&gt; {
    %final = 
    scf.for %i = %lb to %ub step %step iter_args(%acc = %output) 
                                              -&gt; tensor&lt;?x128xi32&gt; {
      %compute = linalg.generic #trait
                 ins(%a, %b : tensor&lt;128x128xi32&gt;, tensor&lt;128x128xi32&gt;) 
                 outs(%c : tensor&lt;128x128xi32&gt;) {
      ^bb0(%in : i32, %in2 : i32, %in3 : i32):
        linalg.yield %in3 : i32
      } -&gt; tensor&lt;128x128xi32&gt;
      %newacc = tensor.insert_slice %compute into 
                                    %output[%i, 0][128, 128][1, 1] 
                                    : tensor&lt;128x128xi32&gt; into tensor&lt;?x128xi32&gt;
      scf.yield %newacc : tensor&lt;?x128xi32&gt;
    }
    func.return %final : tensor&lt;?x128xi32&gt;
  }
```

reproduce: `mlir-opt ./test.mlir -split-input-file --linalg-specialize-generic-ops`

UBSan:
```
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:318:20 in 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: llvm-mlir-build/bin/mlir-opt /tmp/mlirfuzz_run_ee5lfpd3/fused.mlir -split-input-file --linalg-specialize-generic-ops
 #<!-- -->0 0x00005c4ba94af69b backtrace (llvm-mlir-build/bin/mlir-opt+0x83bd69b)
 #<!-- -->1 0x00005c4ba957cad7 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:838:8
 #<!-- -->2 0x00005c4ba9576dbf llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->3 0x00005c4ba957fe16 SignalHandler(int, siginfo_t*, void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x000072e2df10c520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x000072e2df1609fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000072e2df1609fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000072e2df1609fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000072e2df10c476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000072e2df0f27f3 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00005c4ba951ad07 (llvm-mlir-build/bin/mlir-opt+0x8428d07)
#<!-- -->11 0x00005c4ba9518ba1 (llvm-mlir-build/bin/mlir-opt+0x8426ba1)
#<!-- -->12 0x00005c4ba952c8ec __ubsan::ScopedReport::~ScopedReport() (llvm-mlir-build/bin/mlir-opt+0x843a8ec)
#<!-- -->13 0x00005c4ba952d45c handleTypeMismatchImpl(__ubsan::TypeMismatchData*, unsigned long, __ubsan::ReportOptions) ubsan_handlers.cpp.o:0:0
#<!-- -->14 0x00005c4ba952d4ee __ubsan_handle_type_mismatch_v1_abort (llvm-mlir-build/bin/mlir-opt+0x843b4ee)
#<!-- -->15 0x00005c4bb7a63529 mlir::linalg::detail::isContractionBody(mlir::Block&amp;, llvm::function_ref&lt;bool (mlir::Operation*, mlir::Operation*)&gt;, llvm::raw_ostream&amp;) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:336:40
#<!-- -->16 0x00005c4bb7a65eb2 bool isContractionBody&lt;mlir::arith::MulFOp, mlir::arith::AddFOp, mlir::arith::MulIOp, mlir::arith::AddIOp, mlir::complex::MulOp, mlir::complex::AddOp, mlir::arith::AndIOp, mlir::arith::OrIOp&gt;(mlir::Block&amp;) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:380:3
#<!-- -->17 0x00005c4bb7a65eb2 mlir::linalg::detail::isContractionInterfaceImpl(mlir::Operation*, mlir::linalg::ContractionDimensions*) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:527:8
#<!-- -->18 0x00005c4bb7a662e7 mlir::linalg::isaContractionOpInterface(mlir::linalg::LinalgOp) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:569:64
#<!-- -->19 0x00005c4bb6b172be mlir::linalg::specializeGenericOp(mlir::RewriterBase&amp;, mlir::linalg::GenericOp) llvm-project/mlir/lib/Dialect/Linalg/Transforms/Specialize.cpp:316:7
#<!-- -->20 0x00005c4bb6b1de92 std::_Optional_base_impl&lt;mlir::linalg::LinalgOp, std::_Optional_base&lt;mlir::linalg::LinalgOp, true, true&gt;&gt;::_M_is_engaged() const /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/optional:433:58
#<!-- -->21 0x00005c4bb6b1de92 std::optional&lt;mlir::linalg::LinalgOp&gt;::has_value() const /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/optional:945:22
#<!-- -->22 0x00005c4bb6b1de92 llvm::FailureOr&lt;mlir::linalg::LinalgOp&gt;::operator llvm::LogicalResult() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:93:51
#<!-- -->23 0x00005c4bb6b1de92 llvm::FailureOr&lt;mlir::linalg::GenericOp&gt;::FailureOr&lt;mlir::linalg::LinalgOp, (void*)0&gt;(llvm::FailureOr&lt;mlir::linalg::LinalgOp&gt; const&amp;) llvm-project/llvm/include/llvm/Support/LogicalResult.h:90:33
#<!-- -->24 0x00005c4bb6b1de92 mlir::linalg::LinalgSpecializationPattern::returningMatchAndRewrite(mlir::linalg::GenericOp, mlir::PatternRewriter&amp;) const llvm-project/mlir/include/mlir/Dialect/Linalg/Transforms/Transforms.h:1616:12
#<!-- -->25 0x00005c4bb6b1de92 mlir::linalg::LinalgSpecializationPattern::matchAndRewrite(mlir::linalg::GenericOp, mlir::PatternRewriter&amp;) const llvm-project/mlir/include/mlir/Dialect/Linalg/Transforms/Transforms.h:1621:12
#<!-- -->26 0x00005c4bbbddda9e mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_6::operator()() const llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:223:13
#<!-- -->27 0x00005c4bbbddda9e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_6&gt;(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->28 0x00005c4bbbdd307e mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->29 0x00005c4bbbd6c5b8 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() llvm-project/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:620:17
#<!-- -->30 0x00005c4bbbd6a537 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_14::operator()() const llvm-project/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:890:11
#<!-- -->31 0x00005c4bbbd6a537 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_14&gt;(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->32 0x00005c4bbbd64892 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->33 0x00005c4bbbd64892 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) llvm-project/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:935:47
#<!-- -->34 0x00005c4ba97d5a73 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) llvm-project/mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#<!-- -->35 0x00005c4bb6b1d39d (anonymous namespace)::LinalgSpecializeGenericOpsPass::runOnOperation() llvm-project/mlir/lib/Dialect/Linalg/Transforms/Specialize.cpp:338:14
#<!-- -->36 0x00005c4bbbee5f44 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_44::operator()() const llvm-project/mlir/lib/Pass/Pass.cpp:610:22
#<!-- -->37 0x00005c4bbbee5f44 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_44&gt;(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->38 0x00005c4bbbecf11a mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) llvm-project/mlir/lib/Pass/Pass.cpp:603:3
#<!-- -->39 0x00005c4bbbed10ec llvm::LogicalResult::failed() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->40 0x00005c4bbbed10ec llvm::failed(llvm::LogicalResult) llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->41 0x00005c4bbbed10ec mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) llvm-project/mlir/lib/Pass/Pass.cpp:682:9
#<!-- -->42 0x00005c4bbbedd238 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->43 0x00005c4bbbedbdb4 mlir::PassManager::run(mlir::Operation*) llvm-project/mlir/lib/Pass/Pass.cpp:0:60
#<!-- -->44 0x00005c4ba9737604 llvm::LogicalResult::failed() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->45 0x00005c4ba9737604 llvm::failed(llvm::LogicalResult) llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->46 0x00005c4ba9737604 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:568:7
#<!-- -->47 0x00005c4ba9736517 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:644:12
#<!-- -->48 0x00005c4ba9736517 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:733:12
#<!-- -->49 0x00005c4ba9736517 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->50 0x00005c4bbc6faa64 std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->51 0x00005c4bbc6faa64 llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->52 0x00005c4bbc6fb526 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const llvm-project/mlir/lib/Support/ToolUtilities.cpp:94:13
#<!-- -->53 0x00005c4bbc6fa447 void llvm::interleave&lt;llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;)::'lambda'(), void&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) llvm-project/llvm/include/llvm/ADT/STLExtras.h:2201:24
#<!-- -->54 0x00005c4bbc6fa447 void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;) llvm-project/llvm/include/llvm/ADT/STLExtras.h:2221:3
#<!-- -->55 0x00005c4bbc6fa447 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) llvm-project/mlir/lib/Support/ToolUtilities.cpp:97:3
#<!-- -->56 0x00005c4ba971b432 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:26
#<!-- -->57 0x00005c4ba971be6e llvm::LogicalResult::failed() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->58 0x00005c4ba971be6e llvm::failed(llvm::LogicalResult) llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->59 0x00005c4ba971be6e mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:7
#<!-- -->60 0x00005c4ba971c5c3 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:800:10
#<!-- -->61 0x00005c4ba95343cf llvm::LogicalResult::succeeded() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->62 0x00005c4ba95343cf mlir::asMainReturnCode(llvm::LogicalResult) llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:399:12
#<!-- -->63 0x00005c4ba95343cf main llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:343:10
#<!-- -->64 0x000072e2df0f3d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->65 0x000072e2df0f3e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->66 0x000072e2df0f3e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->67 0x00005c4ba9473b65 _start (llvm-mlir-build/bin/mlir-opt+0x8381b65)
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-linalg

Author: Yuancheng Jiang (YuanchengJiang)

<details>
PoC:
```
#trait = {
    indexing_maps = [
      affine_map&lt;(m, n, k) -&gt; (m, k)&gt;,
      affine_map&lt;(m, n, k) -&gt; (k, n)&gt;,
      affine_map&lt;(m, n, k) -&gt; (m, n)&gt;
    ],
    iterator_types = ["parallel", "parallel", "reduction"] 
  }
  func.func @<!-- -->B_hoist_linalg_ops_div_by_zero(%a : tensor&lt;128x128xi32&gt;, 
                              %b : tensor&lt;128x128xi32&gt;, 
                              %c: tensor&lt;128x128xi32&gt;,
                              %lb : index,
                              %ub : index,
                              %step : index,
                              %output : tensor&lt;?x128xi32&gt;) -&gt; tensor&lt;?x128xi32&gt; {
    %final = 
    scf.for %i = %lb to %ub step %step iter_args(%acc = %output) 
                                              -&gt; tensor&lt;?x128xi32&gt; {
      %compute = linalg.generic #trait
                 ins(%a, %b : tensor&lt;128x128xi32&gt;, tensor&lt;128x128xi32&gt;) 
                 outs(%c : tensor&lt;128x128xi32&gt;) {
      ^bb0(%in : i32, %in2 : i32, %in3 : i32):
        linalg.yield %in3 : i32
      } -&gt; tensor&lt;128x128xi32&gt;
      %newacc = tensor.insert_slice %compute into 
                                    %output[%i, 0][128, 128][1, 1] 
                                    : tensor&lt;128x128xi32&gt; into tensor&lt;?x128xi32&gt;
      scf.yield %newacc : tensor&lt;?x128xi32&gt;
    }
    func.return %final : tensor&lt;?x128xi32&gt;
  }
```

reproduce: `mlir-opt ./test.mlir -split-input-file --linalg-specialize-generic-ops`

UBSan:
```
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:318:20 in 
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: llvm-mlir-build/bin/mlir-opt /tmp/mlirfuzz_run_ee5lfpd3/fused.mlir -split-input-file --linalg-specialize-generic-ops
 #<!-- -->0 0x00005c4ba94af69b backtrace (llvm-mlir-build/bin/mlir-opt+0x83bd69b)
 #<!-- -->1 0x00005c4ba957cad7 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:838:8
 #<!-- -->2 0x00005c4ba9576dbf llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->3 0x00005c4ba957fe16 SignalHandler(int, siginfo_t*, void*) llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x000072e2df10c520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x000072e2df1609fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x000072e2df1609fc __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x000072e2df1609fc pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x000072e2df10c476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x000072e2df0f27f3 abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00005c4ba951ad07 (llvm-mlir-build/bin/mlir-opt+0x8428d07)
#<!-- -->11 0x00005c4ba9518ba1 (llvm-mlir-build/bin/mlir-opt+0x8426ba1)
#<!-- -->12 0x00005c4ba952c8ec __ubsan::ScopedReport::~ScopedReport() (llvm-mlir-build/bin/mlir-opt+0x843a8ec)
#<!-- -->13 0x00005c4ba952d45c handleTypeMismatchImpl(__ubsan::TypeMismatchData*, unsigned long, __ubsan::ReportOptions) ubsan_handlers.cpp.o:0:0
#<!-- -->14 0x00005c4ba952d4ee __ubsan_handle_type_mismatch_v1_abort (llvm-mlir-build/bin/mlir-opt+0x843b4ee)
#<!-- -->15 0x00005c4bb7a63529 mlir::linalg::detail::isContractionBody(mlir::Block&amp;, llvm::function_ref&lt;bool (mlir::Operation*, mlir::Operation*)&gt;, llvm::raw_ostream&amp;) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:336:40
#<!-- -->16 0x00005c4bb7a65eb2 bool isContractionBody&lt;mlir::arith::MulFOp, mlir::arith::AddFOp, mlir::arith::MulIOp, mlir::arith::AddIOp, mlir::complex::MulOp, mlir::complex::AddOp, mlir::arith::AndIOp, mlir::arith::OrIOp&gt;(mlir::Block&amp;) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:380:3
#<!-- -->17 0x00005c4bb7a65eb2 mlir::linalg::detail::isContractionInterfaceImpl(mlir::Operation*, mlir::linalg::ContractionDimensions*) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:527:8
#<!-- -->18 0x00005c4bb7a662e7 mlir::linalg::isaContractionOpInterface(mlir::linalg::LinalgOp) llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:569:64
#<!-- -->19 0x00005c4bb6b172be mlir::linalg::specializeGenericOp(mlir::RewriterBase&amp;, mlir::linalg::GenericOp) llvm-project/mlir/lib/Dialect/Linalg/Transforms/Specialize.cpp:316:7
#<!-- -->20 0x00005c4bb6b1de92 std::_Optional_base_impl&lt;mlir::linalg::LinalgOp, std::_Optional_base&lt;mlir::linalg::LinalgOp, true, true&gt;&gt;::_M_is_engaged() const /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/optional:433:58
#<!-- -->21 0x00005c4bb6b1de92 std::optional&lt;mlir::linalg::LinalgOp&gt;::has_value() const /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/optional:945:22
#<!-- -->22 0x00005c4bb6b1de92 llvm::FailureOr&lt;mlir::linalg::LinalgOp&gt;::operator llvm::LogicalResult() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:93:51
#<!-- -->23 0x00005c4bb6b1de92 llvm::FailureOr&lt;mlir::linalg::GenericOp&gt;::FailureOr&lt;mlir::linalg::LinalgOp, (void*)0&gt;(llvm::FailureOr&lt;mlir::linalg::LinalgOp&gt; const&amp;) llvm-project/llvm/include/llvm/Support/LogicalResult.h:90:33
#<!-- -->24 0x00005c4bb6b1de92 mlir::linalg::LinalgSpecializationPattern::returningMatchAndRewrite(mlir::linalg::GenericOp, mlir::PatternRewriter&amp;) const llvm-project/mlir/include/mlir/Dialect/Linalg/Transforms/Transforms.h:1616:12
#<!-- -->25 0x00005c4bb6b1de92 mlir::linalg::LinalgSpecializationPattern::matchAndRewrite(mlir::linalg::GenericOp, mlir::PatternRewriter&amp;) const llvm-project/mlir/include/mlir/Dialect/Linalg/Transforms/Transforms.h:1621:12
#<!-- -->26 0x00005c4bbbddda9e mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_6::operator()() const llvm-project/mlir/lib/Rewrite/PatternApplicator.cpp:223:13
#<!-- -->27 0x00005c4bbbddda9e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::$_6&gt;(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->28 0x00005c4bbbdd307e mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->29 0x00005c4bbbd6c5b8 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() llvm-project/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:620:17
#<!-- -->30 0x00005c4bbbd6a537 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_14::operator()() const llvm-project/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:890:11
#<!-- -->31 0x00005c4bbbd6a537 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;(anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp;::$_14&gt;(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->32 0x00005c4bbbd64892 (anonymous namespace)::RegionPatternRewriteDriver::simplify(bool*) &amp;&amp; llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->33 0x00005c4bbbd64892 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) llvm-project/mlir/lib/Transforms/Utils/GreedyPatternRewriteDriver.cpp:935:47
#<!-- -->34 0x00005c4ba97d5a73 mlir::applyPatternsGreedily(mlir::Operation*, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) llvm-project/mlir/include/mlir/Transforms/GreedyPatternRewriteDriver.h:224:15
#<!-- -->35 0x00005c4bb6b1d39d (anonymous namespace)::LinalgSpecializeGenericOpsPass::runOnOperation() llvm-project/mlir/lib/Dialect/Linalg/Transforms/Specialize.cpp:338:14
#<!-- -->36 0x00005c4bbbee5f44 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_44::operator()() const llvm-project/mlir/lib/Pass/Pass.cpp:610:22
#<!-- -->37 0x00005c4bbbee5f44 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_44&gt;(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->38 0x00005c4bbbecf11a mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) llvm-project/mlir/lib/Pass/Pass.cpp:603:3
#<!-- -->39 0x00005c4bbbed10ec llvm::LogicalResult::failed() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->40 0x00005c4bbbed10ec llvm::failed(llvm::LogicalResult) llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->41 0x00005c4bbbed10ec mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) llvm-project/mlir/lib/Pass/Pass.cpp:682:9
#<!-- -->42 0x00005c4bbbedd238 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->43 0x00005c4bbbedbdb4 mlir::PassManager::run(mlir::Operation*) llvm-project/mlir/lib/Pass/Pass.cpp:0:60
#<!-- -->44 0x00005c4ba9737604 llvm::LogicalResult::failed() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->45 0x00005c4ba9737604 llvm::failed(llvm::LogicalResult) llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->46 0x00005c4ba9737604 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:568:7
#<!-- -->47 0x00005c4ba9736517 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:644:12
#<!-- -->48 0x00005c4ba9736517 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:733:12
#<!-- -->49 0x00005c4ba9736517 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->50 0x00005c4bbc6faa64 std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->51 0x00005c4bbc6faa64 llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->52 0x00005c4bbc6fb526 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const llvm-project/mlir/lib/Support/ToolUtilities.cpp:94:13
#<!-- -->53 0x00005c4bbc6fa447 void llvm::interleave&lt;llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;)::'lambda'(), void&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) llvm-project/llvm/include/llvm/ADT/STLExtras.h:2201:24
#<!-- -->54 0x00005c4bbc6fa447 void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;) llvm-project/llvm/include/llvm/ADT/STLExtras.h:2221:3
#<!-- -->55 0x00005c4bbc6fa447 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) llvm-project/mlir/lib/Support/ToolUtilities.cpp:97:3
#<!-- -->56 0x00005c4ba971b432 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:26
#<!-- -->57 0x00005c4ba971be6e llvm::LogicalResult::failed() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->58 0x00005c4ba971be6e llvm::failed(llvm::LogicalResult) llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->59 0x00005c4ba971be6e mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:784:7
#<!-- -->60 0x00005c4ba971c5c3 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:800:10
#<!-- -->61 0x00005c4ba95343cf llvm::LogicalResult::succeeded() const llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->62 0x00005c4ba95343cf mlir::asMainReturnCode(llvm::LogicalResult) llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:399:12
#<!-- -->63 0x00005c4ba95343cf main llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:343:10
#<!-- -->64 0x000072e2df0f3d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->65 0x000072e2df0f3e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->66 0x000072e2df0f3e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->67 0x00005c4ba9473b65 _start (llvm-mlir-build/bin/mlir-opt+0x8381b65)
```
</details>


---

