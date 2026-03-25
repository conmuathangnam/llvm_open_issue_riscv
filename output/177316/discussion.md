# [MLIR] Crash during gpu kernel outlining in mlir-opt​

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/177316
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-04T06:44:05Z

## Body

I encountered a crash bug while lowering a gpu program. The issue can be reproduced with the following code example.

```mlir
func.func @floatToChar(%arg0: memref<?xf32>, %arg1: tensor<?xi8>, %arg2: i32, %arg3: i32, %arg4: i32) -> tensor<?xi8> {
  %c1 = arith.constant 1 : index
  %c0 = arith.constant 0 : index
  %c3 = arith.constant 3 : index
  %c2 = arith.constant 2 : index
  %0 = arith.index_cast %arg2 : i32 to index
  %1 = arith.index_cast %arg3 : i32 to index
  %2 = arith.index_cast %arg4 : i32 to index
  %3 = arith.muli %0, %1 : index
  %4 = gpu.thread_id x
  %5 = gpu.block_id x
  %6 = gpu.block_dim x
  %7 = arith.muli %5, %6 : index
  %8 = arith.addi %4, %7 : index
  %9 = arith.cmpi slt, %8, %3 : index
  %10 = scf.if %9 -> (tensor<?xi8>) {
    %result = linalg.generic {indexing_maps = [affine_map<(d0, d1) -> (d0, d1)>], iterator_types = ["parallel", "parallel"]}
        outs(%arg1 : tensor<?xi8>) {
    ^bb0(%in: f32, %out: i8):
      %11 = arith.fptoui %in : f32 to i8
      linalg.yield %11 : i8
    } -> tensor<?xi8>
    scf.yield %result : tensor<?xi8>
  } else {
    scf.yield %arg1 : tensor<?xi8>
  }
  gpu.terminator
  func.return %10 : tensor<?xi8>
}
```

The crash log is shown below.

```bash
mlir-opt: /llvm-project/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From &) [To = mlir::AffineConstantExpr, From = mlir::AffineExpr]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt -gpu-kernel-outlining input.mlir
 #0 0x0000556d078f761d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x0000556d078f7b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x0000556d078f5a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x0000556d078f81e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f9d64445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f9d6449eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f9d6449eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f9d6449eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f9d6444527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f9d644288ff abort ./stdlib/abort.c:81:7
#10 0x00007f9d6442881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007f9d6443b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x0000556d08e77324 decltype(auto) llvm::cast<mlir::AffineConstantExpr, mlir::AffineExpr>(mlir::AffineExpr const&) /llvm-project/llvm/include/llvm/Support/Casting.h:561:43
#13 0x0000556d155c205d mlir::AffineMap::compose(llvm::ArrayRef<long>) const /llvm-project/mlir/lib/IR/AffineMap.cpp:585:19
#14 0x0000556d122dfbcc mlir::detail::IndexingMapOpInterfaceTrait<mlir::linalg::GenericOp>::getStaticLoopRanges() /llvm-project/build_all/tools/mlir/include/mlir/Interfaces/IndexingMapOpInterface.h.inc:204:28
#15 0x0000556d12cd5fa9 mlir::detail::IndexingMapOpInterfaceInterfaceTraits::Model<mlir::linalg::GenericOp>::getStaticLoopRanges(mlir::detail::IndexingMapOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*) /llvm-project/build_all/tools/mlir/include/mlir/Interfaces/IndexingMapOpInterface.h.inc:235:56
#16 0x0000556d13584ed7 mlir::IndexingMapOpInterface::getStaticLoopRanges() /llvm-project/build_all/tools/mlir/include/mlir/Interfaces/IndexingMapOpInterface.cpp.inc:68:14
#17 0x0000556d13585151 mlir::IndexingMapOpInterface::verifyImpl() /llvm-project/mlir/lib/Interfaces/IndexingMapOpInterface.cpp:35:45
#18 0x0000556d128fd887 mlir::linalg::detail::verifyStructuredOpInterface(mlir::Operation*) /llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:1256:47
#19 0x0000556d12cd9555 mlir::linalg::detail::LinalgOpTrait<mlir::linalg::GenericOp>::verifyRegionTrait(mlir::Operation*) /llvm-project/build_all/tools/mlir/include/mlir/Dialect/Linalg/IR/LinalgInterfaces.h.inc:1192:14
#20 0x0000556d12cd9225 llvm::LogicalResult mlir::op_definition_impl::verifyRegionTrait<mlir::linalg::LinalgOp::Trait<mlir::linalg::GenericOp>>(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OpDefinition.h:1652:12
#21 0x0000556d12cd8ea6 llvm::LogicalResult mlir::op_definition_impl::verifyRegionTraits<mlir::OpTrait::OneRegion<mlir::linalg::GenericOp>, mlir::OpTrait::VariadicResults<mlir::linalg::GenericOp>, mlir::OpTrait::ZeroSuccessors<mlir::linalg::GenericOp>, mlir::OpTrait::VariadicOperands<mlir::linalg::GenericOp>, mlir::OpTrait::SingleBlock<mlir::linalg::GenericOp>, mlir::OpTrait::SingleBlockImplicitTerminator<mlir::linalg::YieldOp>::Impl<mlir::linalg::GenericOp>, mlir::OpTrait::AttrSizedOperandSegments<mlir::linalg::GenericOp>, mlir::OpTrait::OpInvariants<mlir::linalg::GenericOp>, mlir::BytecodeOpInterface::Trait<mlir::linalg::GenericOp>, mlir::MemoryEffectOpInterface::Trait<mlir::linalg::GenericOp>, mlir::ConditionallySpeculatable::Trait<mlir::linalg::GenericOp>, mlir::OpTrait::HasRecursiveMemoryEffects<mlir::linalg::GenericOp>, mlir::DestinationStyleOpInterface::Trait<mlir::linalg::GenericOp>, mlir::IndexingMapOpInterface::Trait<mlir::linalg::GenericOp>, mlir::linalg::LinalgOp::Trait<mlir::linalg::GenericOp>, mlir::ReifyRankedShapedTypeOpInterface::Trait<mlir::linalg::GenericOp>, mlir::OpAsmOpInterface::Trait<mlir::linalg::GenericOp>>(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OpDefinition.h:1661:29
#22 0x0000556d12cd8c25 mlir::Op<mlir::linalg::GenericOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::SingleBlock, mlir::OpTrait::SingleBlockImplicitTerminator<mlir::linalg::YieldOp>::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::MemoryEffectOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::HasRecursiveMemoryEffects, mlir::DestinationStyleOpInterface::Trait, mlir::IndexingMapOpInterface::Trait, mlir::linalg::LinalgOp::Trait, mlir::ReifyRankedShapedTypeOpInterface::Trait, mlir::OpAsmOpInterface::Trait>::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OpDefinition.h:2063:16
#23 0x0000556d07c60345 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*>::CallImpl<llvm::LogicalResult (* const)(mlir::Operation*)>(void*, mlir::Operation*) /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#24 0x0000556d07c60117 llvm::unique_function<llvm::LogicalResult (mlir::Operation*) const>::operator()(mlir::Operation*) const /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:391:12
#25 0x0000556d12cd52f6 mlir::RegisteredOperationName::Model<mlir::linalg::GenericOp>::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OperationSupport.h:558:14
#26 0x0000556d157d7c16 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /llvm-project/mlir/include/mlir/IR/OperationSupport.h:317:23
#27 0x0000556d157d4cbf (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /llvm-project/mlir/lib/IR/Verifier.cpp:236:48
#28 0x0000556d157d4a00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#29 0x0000556d157d4935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#30 0x0000556d157d41fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#31 0x0000556d157d4061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#32 0x0000556d157d5264 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0::operator()(mlir::Operation*) const /llvm-project/mlir/lib/IR/Verifier.cpp:225:16
#33 0x0000556d157d51e3 auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /llvm-project/mlir/include/mlir/IR/Threading.h:120:56
#34 0x0000556d157d502b llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0&&) /llvm-project/mlir/include/mlir/IR/Threading.h:46:18
#35 0x0000556d157d4f85 void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0&&) /llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#36 0x0000556d157d4f46 void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_0&&) /llvm-project/mlir/include/mlir/IR/Threading.h:133:1
#37 0x0000556d157d4c3c (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /llvm-project/mlir/lib/IR/Verifier.cpp:228:22
#38 0x0000556d157d4a00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#39 0x0000556d157d4935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#40 0x0000556d157d41fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#41 0x0000556d157d4061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#42 0x0000556d157d4012 mlir::verify(mlir::Operation*, bool) /llvm-project/mlir/lib/IR/Verifier.cpp:425:19
#43 0x0000556d12df93e7 (anonymous namespace)::OperationParser::finalize() /llvm-project/mlir/lib/AsmParser/Parser.cpp:931:55
#44 0x0000556d12df83c5 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /llvm-project/mlir/lib/AsmParser/Parser.cpp:2876:20
#45 0x0000556d12df820e mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /llvm-project/mlir/lib/AsmParser/Parser.cpp:2930:41
#46 0x0000556d12dc934f mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /llvm-project/mlir/lib/Parser/Parser.cpp:64:10
#47 0x0000556d07a11c0f mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /llvm-project/mlir/include/mlir/Parser/Parser.h:158:14
#48 0x0000556d07a11b18 mlir::OwningOpRef<mlir::ModuleOp> mlir::parseSourceFile<mlir::ModuleOp>(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&) /llvm-project/mlir/include/mlir/Parser/Parser.h:188:10
#49 0x0000556d07a10dbd mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#50 0x0000556d079f7d6a performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#51 0x0000556d079f7b71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#52 0x0000556d079f782c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#53 0x0000556d079f7736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#54 0x0000556d15800cba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#55 0x0000556d1580007a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#56 0x0000556d079f3126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#57 0x0000556d079f3525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#58 0x0000556d079f36f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#59 0x0000556d078cc935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#60 0x00007f9d6442a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#61 0x00007f9d6442a28b call_init ./csu/../csu/libc-start.c:128:20
#62 0x00007f9d6442a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#63 0x0000556d078cc505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the command:

```bash
mlir-opt -gpu-kernel-outlining input.mlir
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
I encountered a crash bug while lowering a gpu program. The issue can be reproduced with the following code example.

```mlir
func.func @<!-- -->floatToChar(%arg0: memref&lt;?xf32&gt;, %arg1: tensor&lt;?xi8&gt;, %arg2: i32, %arg3: i32, %arg4: i32) -&gt; tensor&lt;?xi8&gt; {
  %c1 = arith.constant 1 : index
  %c0 = arith.constant 0 : index
  %c3 = arith.constant 3 : index
  %c2 = arith.constant 2 : index
  %0 = arith.index_cast %arg2 : i32 to index
  %1 = arith.index_cast %arg3 : i32 to index
  %2 = arith.index_cast %arg4 : i32 to index
  %3 = arith.muli %0, %1 : index
  %4 = gpu.thread_id x
  %5 = gpu.block_id x
  %6 = gpu.block_dim x
  %7 = arith.muli %5, %6 : index
  %8 = arith.addi %4, %7 : index
  %9 = arith.cmpi slt, %8, %3 : index
  %10 = scf.if %9 -&gt; (tensor&lt;?xi8&gt;) {
    %result = linalg.generic {indexing_maps = [affine_map&lt;(d0, d1) -&gt; (d0, d1)&gt;], iterator_types = ["parallel", "parallel"]}
        outs(%arg1 : tensor&lt;?xi8&gt;) {
    ^bb0(%in: f32, %out: i8):
      %11 = arith.fptoui %in : f32 to i8
      linalg.yield %11 : i8
    } -&gt; tensor&lt;?xi8&gt;
    scf.yield %result : tensor&lt;?xi8&gt;
  } else {
    scf.yield %arg1 : tensor&lt;?xi8&gt;
  }
  gpu.terminator
  func.return %10 : tensor&lt;?xi8&gt;
}
```

The crash log is shown below.

```bash
mlir-opt: /llvm-project/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From &amp;) [To = mlir::AffineConstantExpr, From = mlir::AffineExpr]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt -gpu-kernel-outlining input.mlir
 #<!-- -->0 0x0000556d078f761d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000556d078f7b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000556d078f5a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x0000556d078f81e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f9d64445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f9d6449eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f9d6449eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f9d6449eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f9d6444527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f9d644288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007f9d6442881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007f9d6443b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x0000556d08e77324 decltype(auto) llvm::cast&lt;mlir::AffineConstantExpr, mlir::AffineExpr&gt;(mlir::AffineExpr const&amp;) /llvm-project/llvm/include/llvm/Support/Casting.h:561:43
#<!-- -->13 0x0000556d155c205d mlir::AffineMap::compose(llvm::ArrayRef&lt;long&gt;) const /llvm-project/mlir/lib/IR/AffineMap.cpp:585:19
#<!-- -->14 0x0000556d122dfbcc mlir::detail::IndexingMapOpInterfaceTrait&lt;mlir::linalg::GenericOp&gt;::getStaticLoopRanges() /llvm-project/build_all/tools/mlir/include/mlir/Interfaces/IndexingMapOpInterface.h.inc:204:28
#<!-- -->15 0x0000556d12cd5fa9 mlir::detail::IndexingMapOpInterfaceInterfaceTraits::Model&lt;mlir::linalg::GenericOp&gt;::getStaticLoopRanges(mlir::detail::IndexingMapOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*) /llvm-project/build_all/tools/mlir/include/mlir/Interfaces/IndexingMapOpInterface.h.inc:235:56
#<!-- -->16 0x0000556d13584ed7 mlir::IndexingMapOpInterface::getStaticLoopRanges() /llvm-project/build_all/tools/mlir/include/mlir/Interfaces/IndexingMapOpInterface.cpp.inc:68:14
#<!-- -->17 0x0000556d13585151 mlir::IndexingMapOpInterface::verifyImpl() /llvm-project/mlir/lib/Interfaces/IndexingMapOpInterface.cpp:35:45
#<!-- -->18 0x0000556d128fd887 mlir::linalg::detail::verifyStructuredOpInterface(mlir::Operation*) /llvm-project/mlir/lib/Dialect/Linalg/IR/LinalgInterfaces.cpp:1256:47
#<!-- -->19 0x0000556d12cd9555 mlir::linalg::detail::LinalgOpTrait&lt;mlir::linalg::GenericOp&gt;::verifyRegionTrait(mlir::Operation*) /llvm-project/build_all/tools/mlir/include/mlir/Dialect/Linalg/IR/LinalgInterfaces.h.inc:1192:14
#<!-- -->20 0x0000556d12cd9225 llvm::LogicalResult mlir::op_definition_impl::verifyRegionTrait&lt;mlir::linalg::LinalgOp::Trait&lt;mlir::linalg::GenericOp&gt;&gt;(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OpDefinition.h:1652:12
#<!-- -->21 0x0000556d12cd8ea6 llvm::LogicalResult mlir::op_definition_impl::verifyRegionTraits&lt;mlir::OpTrait::OneRegion&lt;mlir::linalg::GenericOp&gt;, mlir::OpTrait::VariadicResults&lt;mlir::linalg::GenericOp&gt;, mlir::OpTrait::ZeroSuccessors&lt;mlir::linalg::GenericOp&gt;, mlir::OpTrait::VariadicOperands&lt;mlir::linalg::GenericOp&gt;, mlir::OpTrait::SingleBlock&lt;mlir::linalg::GenericOp&gt;, mlir::OpTrait::SingleBlockImplicitTerminator&lt;mlir::linalg::YieldOp&gt;::Impl&lt;mlir::linalg::GenericOp&gt;, mlir::OpTrait::AttrSizedOperandSegments&lt;mlir::linalg::GenericOp&gt;, mlir::OpTrait::OpInvariants&lt;mlir::linalg::GenericOp&gt;, mlir::BytecodeOpInterface::Trait&lt;mlir::linalg::GenericOp&gt;, mlir::MemoryEffectOpInterface::Trait&lt;mlir::linalg::GenericOp&gt;, mlir::ConditionallySpeculatable::Trait&lt;mlir::linalg::GenericOp&gt;, mlir::OpTrait::HasRecursiveMemoryEffects&lt;mlir::linalg::GenericOp&gt;, mlir::DestinationStyleOpInterface::Trait&lt;mlir::linalg::GenericOp&gt;, mlir::IndexingMapOpInterface::Trait&lt;mlir::linalg::GenericOp&gt;, mlir::linalg::LinalgOp::Trait&lt;mlir::linalg::GenericOp&gt;, mlir::ReifyRankedShapedTypeOpInterface::Trait&lt;mlir::linalg::GenericOp&gt;, mlir::OpAsmOpInterface::Trait&lt;mlir::linalg::GenericOp&gt;&gt;(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OpDefinition.h:1661:29
#<!-- -->22 0x0000556d12cd8c25 mlir::Op&lt;mlir::linalg::GenericOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::VariadicOperands, mlir::OpTrait::SingleBlock, mlir::OpTrait::SingleBlockImplicitTerminator&lt;mlir::linalg::YieldOp&gt;::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::MemoryEffectOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::HasRecursiveMemoryEffects, mlir::DestinationStyleOpInterface::Trait, mlir::IndexingMapOpInterface::Trait, mlir::linalg::LinalgOp::Trait, mlir::ReifyRankedShapedTypeOpInterface::Trait, mlir::OpAsmOpInterface::Trait&gt;::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OpDefinition.h:2063:16
#<!-- -->23 0x0000556d07c60345 llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*&gt;::CallImpl&lt;llvm::LogicalResult (* const)(mlir::Operation*)&gt;(void*, mlir::Operation*) /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:213:12
#<!-- -->24 0x0000556d07c60117 llvm::unique_function&lt;llvm::LogicalResult (mlir::Operation*) const&gt;::operator()(mlir::Operation*) const /llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:391:12
#<!-- -->25 0x0000556d12cd52f6 mlir::RegisteredOperationName::Model&lt;mlir::linalg::GenericOp&gt;::verifyRegionInvariants(mlir::Operation*) /llvm-project/mlir/include/mlir/IR/OperationSupport.h:558:14
#<!-- -->26 0x0000556d157d7c16 mlir::OperationName::verifyRegionInvariants(mlir::Operation*) const /llvm-project/mlir/include/mlir/IR/OperationSupport.h:317:23
#<!-- -->27 0x0000556d157d4cbf (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:236:48
#<!-- -->28 0x0000556d157d4a00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->29 0x0000556d157d4935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->30 0x0000556d157d41fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->31 0x0000556d157d4061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->32 0x0000556d157d5264 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0::operator()(mlir::Operation*) const /llvm-project/mlir/lib/IR/Verifier.cpp:225:16
#<!-- -->33 0x0000556d157d51e3 auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /llvm-project/mlir/include/mlir/IR/Threading.h:120:56
#<!-- -->34 0x0000556d157d502b llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0&amp;&amp;) /llvm-project/mlir/include/mlir/IR/Threading.h:46:18
#<!-- -->35 0x0000556d157d4f85 void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0&amp;&amp;) /llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->36 0x0000556d157d4f46 void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_0&amp;&amp;) /llvm-project/mlir/include/mlir/IR/Threading.h:133:1
#<!-- -->37 0x0000556d157d4c3c (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:228:22
#<!-- -->38 0x0000556d157d4a00 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_0clIS2_EEDaPT_ /llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->39 0x0000556d157d4935 _ZZN12_GLOBAL__N_117OperationVerifier15verifyOperationERN4mlir9OperationEENK3$_1clIZNS0_15verifyOperationES3_E3$_0EEDaOT_N4llvm12PointerUnionIJPS2_PNS1_5BlockEEEE /llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->40 0x0000556d157d41fa (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->41 0x0000556d157d4061 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->42 0x0000556d157d4012 mlir::verify(mlir::Operation*, bool) /llvm-project/mlir/lib/IR/Verifier.cpp:425:19
#<!-- -->43 0x0000556d12df93e7 (anonymous namespace)::OperationParser::finalize() /llvm-project/mlir/lib/AsmParser/Parser.cpp:931:55
#<!-- -->44 0x0000556d12df83c5 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /llvm-project/mlir/lib/AsmParser/Parser.cpp:2876:20
#<!-- -->45 0x0000556d12df820e mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /llvm-project/mlir/lib/AsmParser/Parser.cpp:2930:41
#<!-- -->46 0x0000556d12dc934f mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /llvm-project/mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->47 0x0000556d07a11c0f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /llvm-project/mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->48 0x0000556d07a11b18 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /llvm-project/mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->49 0x0000556d07a10dbd mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->50 0x0000556d079f7d6a performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#<!-- -->51 0x0000556d079f7b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->52 0x0000556d079f782c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->53 0x0000556d079f7736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->54 0x0000556d15800cba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->55 0x0000556d1580007a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->56 0x0000556d079f3126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->57 0x0000556d079f3525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->58 0x0000556d079f36f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->59 0x0000556d078cc935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->60 0x00007f9d6442a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->61 0x00007f9d6442a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->62 0x00007f9d6442a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->63 0x0000556d078cc505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the command:

```bash
mlir-opt -gpu-kernel-outlining input.mlir
```
</details>


---

### Comment 2 - CoTinker

The mlir you provided is illegal and can't reproduce this issue.

---

### Comment 3 - jiayiw2008

@CoTinker It seems bug has been fixed. I can reproduce it in the version 564c3de67d20d578d05678b49045378fdcf5ccaa. The latest version has fixed the issue.

---

