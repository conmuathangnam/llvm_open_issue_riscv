# [MLIR] GPU dialect heap buffer overflow with negative workgroup_attributions

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/159674
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2026-01-13T21:19:14Z

## Body

Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The GPU dialect crashes with a heap buffer overflow when the`gpu.func` operations specify negative `workgroup_attributions` values. 

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "gpu.module"() <{sym_name = "memref_conversions"}> ({
    "gpu.func"() <{function_type = (memref<8xf32>) -> ()}> ({
    ^bb0(%arg0: memref<8xf32>):
      "gpu.return"() : () -> ()
    }) {gpu.kernel, sym_name = "kern", workgroup_attributions = -1 : i64} : () -> ()
  }) : () -> ()
}) : () -> ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
=================================================================
==2064283==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000006978 at pc 0x55d735b2929b bp 0x7ffcbba93050 sp 0x7ffcbba93048
READ of size 8 at 0x602000006978 thread T0
    #0 0x55d735b2929a in verifyAttributions(mlir::Operation*, llvm::ArrayRef<mlir::BlockArgument>, mlir::gpu::AddressSpace) /workdir/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:552:16
    #1 0x55d735b3c08b in mlir::gpu::GPUFuncOp::verifyBody() /workdir/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:1824:14
    #2 0x55d735dd0a87 in llvm::LogicalResult mlir::function_interface_impl::verifyTrait<mlir::gpu::GPUFuncOp>(mlir::gpu::GPUFuncOp) /workdir/llvm-project/mlir/include/mlir/Interfaces/FunctionInterfaces.h:230:13
    #3 0x55d735dcf21f in llvm::LogicalResult mlir::op_definition_impl::verifyTraits<mlir::OpTrait::OneRegion<mlir::gpu::GPUFuncOp>, mlir::OpTrait::ZeroResults<mlir::gpu::GPUFuncOp>, mlir::OpTrait::ZeroSuccessors<mlir::gpu::GPUFuncOp>, mlir::OpTrait::ZeroOperands<mlir::gpu::GPUFuncOp>, mlir::OpTrait::HasParent<mlir::gpu::GPUModuleOp>::Impl<mlir::gpu::GPUFuncOp>, mlir::OpTrait::OpInvariants<mlir::gpu::GPUFuncOp>, mlir::BytecodeOpInterface::Trait<mlir::gpu::GPUFuncOp>, mlir::OpTrait::AutomaticAllocationScope<mlir::gpu::GPUFuncOp>, mlir::SymbolOpInterface::Trait<mlir::gpu::GPUFuncOp>, mlir::CallableOpInterface::Trait<mlir::gpu::GPUFuncOp>, mlir::FunctionOpInterface::Trait<mlir::gpu::GPUFuncOp>, mlir::OpTrait::IsIsolatedFromAbove<mlir::gpu::GPUFuncOp>, mlir::OpTrait::AffineScope<mlir::gpu::GPUFuncOp> >(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:1645:29
    #4 0x55d735dcf05d in mlir::Op<mlir::gpu::GPUFuncOp, mlir::OpTrait::OneRegion, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::HasParent<mlir::gpu::GPUModuleOp>::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AutomaticAllocationScope, mlir::SymbolOpInterface::Trait, mlir::CallableOpInterface::Trait, mlir::FunctionOpInterface::Trait, mlir::OpTrait::IsIsolatedFromAbove, mlir::OpTrait::AffineScope>::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2052:16
    #5 0x55d735dcafc0 in mlir::RegisteredOperationName::Model<mlir::gpu::GPUFuncOp>::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:555:14
    #6 0x55d73ea0fe8c in mlir::OperationName::verifyInvariants(mlir::Operation*) const /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:314:23
    #7 0x55d73ea0fe8c in (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:175:48
    #8 0x55d73ea0fe8c in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
    #9 0x55d73ea0fe8c in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #10 0x55d73ea0fe8c in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
    #11 0x55d73ea0fe8c in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #12 0x55d73ea1524c in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:16
    #13 0x55d73ea1524c in auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
    #14 0x55d73ea10940 in llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:18
    #15 0x55d73ea10940 in void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
    #16 0x55d73ea10940 in void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
    #17 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
    #18 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
    #19 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #20 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
    #21 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #22 0x55d73ea1524c in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:16
    #23 0x55d73ea1524c in auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
    #24 0x55d73ea10940 in llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:18
    #25 0x55d73ea10940 in void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
    #26 0x55d73ea10940 in void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
    #27 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
    #28 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
    #29 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #30 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
    #31 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #32 0x55d73ea1524c in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:16
    #33 0x55d73ea1524c in auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
    #34 0x55d73ea10940 in llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:18
    #35 0x55d73ea10940 in void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
    #36 0x55d73ea10940 in void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
    #37 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
    #38 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
    #39 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #40 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
    #41 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #42 0x55d73ea0e9f2 in mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:425:19
    #43 0x55d746100d68 in (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:55
    #44 0x55d746100d68 in (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
    #45 0x55d746100d68 in mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
    #46 0x55d73e4691dd in mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:46:10
    #47 0x55d73e466424 in mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:14
    #48 0x55d73e465cd7 in mlir::OwningOpRef<mlir::ModuleOp> mlir::parseSourceFile<mlir::ModuleOp>(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:189:10
    #49 0x55d73e465cd7 in mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
    #50 0x55d73e4646fe in performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:459:33
    #51 0x55d73e463fe3 in processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
    #52 0x55d73e463fe3 in mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
    #53 0x55d73e463fe3 in llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #54 0x55d73e69ceab in llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #55 0x55d73e69d4f6 in mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:16
    #56 0x55d73e69c7ca in void llvm::interleave<llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&)::'lambda'(), void>(llvm::SmallVector<llvm::StringRef, 8u>, llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2216:3
    #57 0x55d73e69c7ca in void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2238:3
    #58 0x55d73e69c7ca in mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:89:3
    #59 0x55d73e452b1c in mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
    #60 0x55d73e453404 in mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:14
    #61 0x55d73e453b52 in mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
    #62 0x55d734387986 in main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:33
    #63 0x7fdda04ffd8f  (/lib/x86_64-linux-gnu/libc.so.6+0x29d8f) (BuildId: d5197096f709801829b118af1b7cf6631efa2dcd)
    #64 0x7fdda04ffe3f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e3f) (BuildId: d5197096f709801829b118af1b7cf6631efa2dcd)
    #65 0x55d7342c6ee4 in _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee4) (BuildId: e970ca4352d984cb)

0x602000006978 is located 0 bytes to the right of 8-byte region [0x602000006970,0x602000006978)
allocated by thread T0 here:
    #0 0x55d734384b7d in operator new(unsigned long) (/workdir/llvm-project/build/bin/mlir-opt+0xf673b7d) (BuildId: e970ca4352d984cb)
    #1 0x55d73e755bb1 in void std::vector<mlir::BlockArgument, std::allocator<mlir::BlockArgument> >::_M_realloc_insert<mlir::BlockArgument const&>(__gnu_cxx::__normal_iterator<mlir::BlockArgument*, std::vector<mlir::BlockArgument, std::allocator<mlir::BlockArgument> > >, mlir::BlockArgument const&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/vector.tcc:440:33
    #2 0x55d73e7515a5 in mlir::Block::addArgument(mlir::Type, mlir::Location) /workdir/llvm-project/mlir/lib/IR/Block.cpp:155:13
    #3 0x55d74612632c in (anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13::operator()() const::'lambda'(mlir::OpAsmParser::UnresolvedOperand, mlir::Type)::operator()(mlir::OpAsmParser::UnresolvedOperand, mlir::Type) const /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2460:26
    #4 0x55d74612632c in llvm::ParseResult llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser::UnresolvedOperand, mlir::Type)>::callback_fn<(anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13::operator()() const::'lambda'(mlir::OpAsmParser::UnresolvedOperand, mlir::Type)>(long, mlir::OpAsmParser::UnresolvedOperand, mlir::Type) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #5 0x55d74612632c in llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser::UnresolvedOperand, mlir::Type)>::operator()(mlir::OpAsmParser::UnresolvedOperand, mlir::Type) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #6 0x55d74612632c in (anonymous namespace)::OperationParser::parseSSADefOrUseAndType(llvm::function_ref<llvm::ParseResult (mlir::OpAsmParser::UnresolvedOperand, mlir::Type)>) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1140:10
    #7 0x55d74612632c in (anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13::operator()() const /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2443:12
    #8 0x55d74612632c in llvm::ParseResult llvm::function_ref<llvm::ParseResult ()>::callback_fn<(anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13>(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #9 0x55d7460f9d3c in llvm::function_ref<llvm::ParseResult ()>::operator()() const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #10 0x55d7460f9d3c in mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref<llvm::ParseResult ()>, llvm::StringRef) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:138:7
    #11 0x55d746124246 in (anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2442:10
    #12 0x55d746124246 in (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2383:9
    #13 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
    #14 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
    #15 0x55d74611b67a in (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&, std::optional<llvm::ArrayRef<mlir::OpAsmParser::UnresolvedOperand> >, std::optional<llvm::ArrayRef<mlir::Block*> >, std::optional<llvm::MutableArrayRef<std::unique_ptr<mlir::Region, std::default_delete<mlir::Region> > > >, std::optional<llvm::ArrayRef<mlir::NamedAttribute> >, std::optional<mlir::Attribute>, std::optional<mlir::FunctionType>) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
    #16 0x55d74610b05d in (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
    #17 0x55d746102c82 in (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
    #18 0x55d746125a7a in (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2404:9
    #19 0x55d746123e26 in (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2334:12
    #20 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
    #21 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
    #22 0x55d74611b67a in (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&, std::optional<llvm::ArrayRef<mlir::OpAsmParser::UnresolvedOperand> >, std::optional<llvm::ArrayRef<mlir::Block*> >, std::optional<llvm::MutableArrayRef<std::unique_ptr<mlir::Region, std::default_delete<mlir::Region> > > >, std::optional<llvm::ArrayRef<mlir::NamedAttribute> >, std::optional<mlir::Attribute>, std::optional<mlir::FunctionType>) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
    #23 0x55d74610b05d in (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
    #24 0x55d746102c82 in (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
    #25 0x55d746125a7a in (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2404:9
    #26 0x55d746123e26 in (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2334:12
    #27 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&, llvm::SMLoc, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
    #28 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegion(mlir::Region&, llvm::ArrayRef<mlir::OpAsmParser::Argument>, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
    #29 0x55d74611b67a in (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&, std::optional<llvm::ArrayRef<mlir::OpAsmParser::UnresolvedOperand> >, std::optional<llvm::ArrayRef<mlir::Block*> >, std::optional<llvm::MutableArrayRef<std::unique_ptr<mlir::Region, std::default_delete<mlir::Region> > > >, std::optional<llvm::ArrayRef<mlir::NamedAttribute> >, std::optional<mlir::Attribute>, std::optional<mlir::FunctionType>) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
    #30 0x55d74610b05d in (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
    #31 0x55d746102c82 in (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
    #32 0x55d74610006b in (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2834:20
    #33 0x55d74610006b in mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
    #34 0x55d73e4691dd in mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:46:10
    #35 0x55d73e466424 in mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:14
    #36 0x55d73e465cd7 in mlir::OwningOpRef<mlir::ModuleOp> mlir::parseSourceFile<mlir::ModuleOp>(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:189:10
    #37 0x55d73e465cd7 in mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
    #38 0x55d73e4646fe in performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:459:33
    #39 0x55d73e463fe3 in processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
    #40 0x55d73e463fe3 in mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
    #41 0x55d73e463fe3 in llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #42 0x55d73e69ceab in llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #43 0x55d73e69d4f6 in mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:16

SUMMARY: AddressSanitizer: heap-buffer-overflow /workdir/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:552:16 in verifyAttributions(mlir::Operation*, llvm::ArrayRef<mlir::BlockArgument>, mlir::gpu::AddressSpace)
Shadow bytes around the buggy address:
  0x0c047fff8cd0: fa fa 00 fa fa fa 00 00 fa fa 00 00 fa fa 00 fa
  0x0c047fff8ce0: fa fa 00 fa fa fa 00 00 fa fa 00 00 fa fa 00 00
  0x0c047fff8cf0: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
  0x0c047fff8d00: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
  0x0c047fff8d10: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
=>0x0c047fff8d20: fa fa 00 00 fa fa 00 fa fa fa 00 00 fa fa 00[fa]
  0x0c047fff8d30: fa fa fd fd fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d40: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d50: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d60: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d70: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==2064283==ABORTING
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (sairam2661)

<details>
Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The GPU dialect crashes with a heap buffer overflow when the`gpu.func` operations specify negative `workgroup_attributions` values. 

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "gpu.module"() &lt;{sym_name = "memref_conversions"}&gt; ({
    "gpu.func"() &lt;{function_type = (memref&lt;8xf32&gt;) -&gt; ()}&gt; ({
    ^bb0(%arg0: memref&lt;8xf32&gt;):
      "gpu.return"() : () -&gt; ()
    }) {gpu.kernel, sym_name = "kern", workgroup_attributions = -1 : i64} : () -&gt; ()
  }) : () -&gt; ()
}) : () -&gt; ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
=================================================================
==2064283==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000006978 at pc 0x55d735b2929b bp 0x7ffcbba93050 sp 0x7ffcbba93048
READ of size 8 at 0x602000006978 thread T0
    #<!-- -->0 0x55d735b2929a in verifyAttributions(mlir::Operation*, llvm::ArrayRef&lt;mlir::BlockArgument&gt;, mlir::gpu::AddressSpace) /workdir/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:552:16
    #<!-- -->1 0x55d735b3c08b in mlir::gpu::GPUFuncOp::verifyBody() /workdir/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:1824:14
    #<!-- -->2 0x55d735dd0a87 in llvm::LogicalResult mlir::function_interface_impl::verifyTrait&lt;mlir::gpu::GPUFuncOp&gt;(mlir::gpu::GPUFuncOp) /workdir/llvm-project/mlir/include/mlir/Interfaces/FunctionInterfaces.h:230:13
    #<!-- -->3 0x55d735dcf21f in llvm::LogicalResult mlir::op_definition_impl::verifyTraits&lt;mlir::OpTrait::OneRegion&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::ZeroResults&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::ZeroSuccessors&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::ZeroOperands&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::HasParent&lt;mlir::gpu::GPUModuleOp&gt;::Impl&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::OpInvariants&lt;mlir::gpu::GPUFuncOp&gt;, mlir::BytecodeOpInterface::Trait&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::AutomaticAllocationScope&lt;mlir::gpu::GPUFuncOp&gt;, mlir::SymbolOpInterface::Trait&lt;mlir::gpu::GPUFuncOp&gt;, mlir::CallableOpInterface::Trait&lt;mlir::gpu::GPUFuncOp&gt;, mlir::FunctionOpInterface::Trait&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::IsIsolatedFromAbove&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::AffineScope&lt;mlir::gpu::GPUFuncOp&gt; &gt;(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:1645:29
    #<!-- -->4 0x55d735dcf05d in mlir::Op&lt;mlir::gpu::GPUFuncOp, mlir::OpTrait::OneRegion, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::HasParent&lt;mlir::gpu::GPUModuleOp&gt;::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AutomaticAllocationScope, mlir::SymbolOpInterface::Trait, mlir::CallableOpInterface::Trait, mlir::FunctionOpInterface::Trait, mlir::OpTrait::IsIsolatedFromAbove, mlir::OpTrait::AffineScope&gt;::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2052:16
    #<!-- -->5 0x55d735dcafc0 in mlir::RegisteredOperationName::Model&lt;mlir::gpu::GPUFuncOp&gt;::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:555:14
    #<!-- -->6 0x55d73ea0fe8c in mlir::OperationName::verifyInvariants(mlir::Operation*) const /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:314:23
    #<!-- -->7 0x55d73ea0fe8c in (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:175:48
    #<!-- -->8 0x55d73ea0fe8c in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
    #<!-- -->9 0x55d73ea0fe8c in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #<!-- -->10 0x55d73ea0fe8c in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
    #<!-- -->11 0x55d73ea0fe8c in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #<!-- -->12 0x55d73ea1524c in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:16
    #<!-- -->13 0x55d73ea1524c in auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
    #<!-- -->14 0x55d73ea10940 in llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:18
    #<!-- -->15 0x55d73ea10940 in void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
    #<!-- -->16 0x55d73ea10940 in void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
    #<!-- -->17 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
    #<!-- -->18 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
    #<!-- -->19 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #<!-- -->20 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
    #<!-- -->21 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #<!-- -->22 0x55d73ea1524c in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:16
    #<!-- -->23 0x55d73ea1524c in auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
    #<!-- -->24 0x55d73ea10940 in llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:18
    #<!-- -->25 0x55d73ea10940 in void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
    #<!-- -->26 0x55d73ea10940 in void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
    #<!-- -->27 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
    #<!-- -->28 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
    #<!-- -->29 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #<!-- -->30 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
    #<!-- -->31 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #<!-- -->32 0x55d73ea1524c in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:16
    #<!-- -->33 0x55d73ea1524c in auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
    #<!-- -->34 0x55d73ea10940 in llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:18
    #<!-- -->35 0x55d73ea10940 in void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
    #<!-- -->36 0x55d73ea10940 in void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
    #<!-- -->37 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
    #<!-- -->38 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
    #<!-- -->39 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #<!-- -->40 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
    #<!-- -->41 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #<!-- -->42 0x55d73ea0e9f2 in mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:425:19
    #<!-- -->43 0x55d746100d68 in (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:55
    #<!-- -->44 0x55d746100d68 in (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
    #<!-- -->45 0x55d746100d68 in mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
    #<!-- -->46 0x55d73e4691dd in mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:46:10
    #<!-- -->47 0x55d73e466424 in mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:14
    #<!-- -->48 0x55d73e465cd7 in mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:189:10
    #<!-- -->49 0x55d73e465cd7 in mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
    #<!-- -->50 0x55d73e4646fe in performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:459:33
    #<!-- -->51 0x55d73e463fe3 in processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
    #<!-- -->52 0x55d73e463fe3 in mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
    #<!-- -->53 0x55d73e463fe3 in llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #<!-- -->54 0x55d73e69ceab in llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->55 0x55d73e69d4f6 in mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:16
    #<!-- -->56 0x55d73e69c7ca in void llvm::interleave&lt;llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;)::'lambda'(), void&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2216:3
    #<!-- -->57 0x55d73e69c7ca in void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2238:3
    #<!-- -->58 0x55d73e69c7ca in mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:89:3
    #<!-- -->59 0x55d73e452b1c in mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
    #<!-- -->60 0x55d73e453404 in mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:14
    #<!-- -->61 0x55d73e453b52 in mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
    #<!-- -->62 0x55d734387986 in main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:33
    #<!-- -->63 0x7fdda04ffd8f  (/lib/x86_64-linux-gnu/libc.so.6+0x29d8f) (BuildId: d5197096f709801829b118af1b7cf6631efa2dcd)
    #<!-- -->64 0x7fdda04ffe3f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e3f) (BuildId: d5197096f709801829b118af1b7cf6631efa2dcd)
    #<!-- -->65 0x55d7342c6ee4 in _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee4) (BuildId: e970ca4352d984cb)

0x602000006978 is located 0 bytes to the right of 8-byte region [0x602000006970,0x602000006978)
allocated by thread T0 here:
    #<!-- -->0 0x55d734384b7d in operator new(unsigned long) (/workdir/llvm-project/build/bin/mlir-opt+0xf673b7d) (BuildId: e970ca4352d984cb)
    #<!-- -->1 0x55d73e755bb1 in void std::vector&lt;mlir::BlockArgument, std::allocator&lt;mlir::BlockArgument&gt; &gt;::_M_realloc_insert&lt;mlir::BlockArgument const&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::BlockArgument*, std::vector&lt;mlir::BlockArgument, std::allocator&lt;mlir::BlockArgument&gt; &gt; &gt;, mlir::BlockArgument const&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/vector.tcc:440:33
    #<!-- -->2 0x55d73e7515a5 in mlir::Block::addArgument(mlir::Type, mlir::Location) /workdir/llvm-project/mlir/lib/IR/Block.cpp:155:13
    #<!-- -->3 0x55d74612632c in (anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13::operator()() const::'lambda'(mlir::OpAsmParser::UnresolvedOperand, mlir::Type)::operator()(mlir::OpAsmParser::UnresolvedOperand, mlir::Type) const /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2460:26
    #<!-- -->4 0x55d74612632c in llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser::UnresolvedOperand, mlir::Type)&gt;::callback_fn&lt;(anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13::operator()() const::'lambda'(mlir::OpAsmParser::UnresolvedOperand, mlir::Type)&gt;(long, mlir::OpAsmParser::UnresolvedOperand, mlir::Type) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #<!-- -->5 0x55d74612632c in llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser::UnresolvedOperand, mlir::Type)&gt;::operator()(mlir::OpAsmParser::UnresolvedOperand, mlir::Type) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->6 0x55d74612632c in (anonymous namespace)::OperationParser::parseSSADefOrUseAndType(llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser::UnresolvedOperand, mlir::Type)&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1140:10
    #<!-- -->7 0x55d74612632c in (anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13::operator()() const /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2443:12
    #<!-- -->8 0x55d74612632c in llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult ()&gt;::callback_fn&lt;(anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13&gt;(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #<!-- -->9 0x55d7460f9d3c in llvm::function_ref&lt;llvm::ParseResult ()&gt;::operator()() const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->10 0x55d7460f9d3c in mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref&lt;llvm::ParseResult ()&gt;, llvm::StringRef) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:138:7
    #<!-- -->11 0x55d746124246 in (anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2442:10
    #<!-- -->12 0x55d746124246 in (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2383:9
    #<!-- -->13 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
    #<!-- -->14 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
    #<!-- -->15 0x55d74611b67a in (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&amp;, std::optional&lt;llvm::ArrayRef&lt;mlir::OpAsmParser::UnresolvedOperand&gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::Block*&gt; &gt;, std::optional&lt;llvm::MutableArrayRef&lt;std::unique_ptr&lt;mlir::Region, std::default_delete&lt;mlir::Region&gt; &gt; &gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::NamedAttribute&gt; &gt;, std::optional&lt;mlir::Attribute&gt;, std::optional&lt;mlir::FunctionType&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
    #<!-- -->16 0x55d74610b05d in (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
    #<!-- -->17 0x55d746102c82 in (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
    #<!-- -->18 0x55d746125a7a in (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2404:9
    #<!-- -->19 0x55d746123e26 in (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2334:12
    #<!-- -->20 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
    #<!-- -->21 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
    #<!-- -->22 0x55d74611b67a in (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&amp;, std::optional&lt;llvm::ArrayRef&lt;mlir::OpAsmParser::UnresolvedOperand&gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::Block*&gt; &gt;, std::optional&lt;llvm::MutableArrayRef&lt;std::unique_ptr&lt;mlir::Region, std::default_delete&lt;mlir::Region&gt; &gt; &gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::NamedAttribute&gt; &gt;, std::optional&lt;mlir::Attribute&gt;, std::optional&lt;mlir::FunctionType&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
    #<!-- -->23 0x55d74610b05d in (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
    #<!-- -->24 0x55d746102c82 in (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
    #<!-- -->25 0x55d746125a7a in (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2404:9
    #<!-- -->26 0x55d746123e26 in (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2334:12
    #<!-- -->27 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
    #<!-- -->28 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
    #<!-- -->29 0x55d74611b67a in (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&amp;, std::optional&lt;llvm::ArrayRef&lt;mlir::OpAsmParser::UnresolvedOperand&gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::Block*&gt; &gt;, std::optional&lt;llvm::MutableArrayRef&lt;std::unique_ptr&lt;mlir::Region, std::default_delete&lt;mlir::Region&gt; &gt; &gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::NamedAttribute&gt; &gt;, std::optional&lt;mlir::Attribute&gt;, std::optional&lt;mlir::FunctionType&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
    #<!-- -->30 0x55d74610b05d in (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
    #<!-- -->31 0x55d746102c82 in (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
    #<!-- -->32 0x55d74610006b in (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2834:20
    #<!-- -->33 0x55d74610006b in mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
    #<!-- -->34 0x55d73e4691dd in mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:46:10
    #<!-- -->35 0x55d73e466424 in mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:14
    #<!-- -->36 0x55d73e465cd7 in mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:189:10
    #<!-- -->37 0x55d73e465cd7 in mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
    #<!-- -->38 0x55d73e4646fe in performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:459:33
    #<!-- -->39 0x55d73e463fe3 in processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
    #<!-- -->40 0x55d73e463fe3 in mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
    #<!-- -->41 0x55d73e463fe3 in llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #<!-- -->42 0x55d73e69ceab in llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->43 0x55d73e69d4f6 in mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:16

SUMMARY: AddressSanitizer: heap-buffer-overflow /workdir/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:552:16 in verifyAttributions(mlir::Operation*, llvm::ArrayRef&lt;mlir::BlockArgument&gt;, mlir::gpu::AddressSpace)
Shadow bytes around the buggy address:
  0x0c047fff8cd0: fa fa 00 fa fa fa 00 00 fa fa 00 00 fa fa 00 fa
  0x0c047fff8ce0: fa fa 00 fa fa fa 00 00 fa fa 00 00 fa fa 00 00
  0x0c047fff8cf0: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
  0x0c047fff8d00: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
  0x0c047fff8d10: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
=&gt;0x0c047fff8d20: fa fa 00 00 fa fa 00 fa fa fa 00 00 fa fa 00[fa]
  0x0c047fff8d30: fa fa fd fd fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d40: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d50: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d60: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d70: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==2064283==ABORTING
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (sairam2661)

<details>
Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The GPU dialect crashes with a heap buffer overflow when the`gpu.func` operations specify negative `workgroup_attributions` values. 

**Steps to reproduce**
- Minimal MLIR program (test.mlir)
```
"builtin.module"() ({
  "gpu.module"() &lt;{sym_name = "memref_conversions"}&gt; ({
    "gpu.func"() &lt;{function_type = (memref&lt;8xf32&gt;) -&gt; ()}&gt; ({
    ^bb0(%arg0: memref&lt;8xf32&gt;):
      "gpu.return"() : () -&gt; ()
    }) {gpu.kernel, sym_name = "kern", workgroup_attributions = -1 : i64} : () -&gt; ()
  }) : () -&gt; ()
}) : () -&gt; ()
```

**Command**
```
mlir-opt test.mlir
```

**Output**
```
=================================================================
==2064283==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x602000006978 at pc 0x55d735b2929b bp 0x7ffcbba93050 sp 0x7ffcbba93048
READ of size 8 at 0x602000006978 thread T0
    #<!-- -->0 0x55d735b2929a in verifyAttributions(mlir::Operation*, llvm::ArrayRef&lt;mlir::BlockArgument&gt;, mlir::gpu::AddressSpace) /workdir/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:552:16
    #<!-- -->1 0x55d735b3c08b in mlir::gpu::GPUFuncOp::verifyBody() /workdir/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:1824:14
    #<!-- -->2 0x55d735dd0a87 in llvm::LogicalResult mlir::function_interface_impl::verifyTrait&lt;mlir::gpu::GPUFuncOp&gt;(mlir::gpu::GPUFuncOp) /workdir/llvm-project/mlir/include/mlir/Interfaces/FunctionInterfaces.h:230:13
    #<!-- -->3 0x55d735dcf21f in llvm::LogicalResult mlir::op_definition_impl::verifyTraits&lt;mlir::OpTrait::OneRegion&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::ZeroResults&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::ZeroSuccessors&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::ZeroOperands&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::HasParent&lt;mlir::gpu::GPUModuleOp&gt;::Impl&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::OpInvariants&lt;mlir::gpu::GPUFuncOp&gt;, mlir::BytecodeOpInterface::Trait&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::AutomaticAllocationScope&lt;mlir::gpu::GPUFuncOp&gt;, mlir::SymbolOpInterface::Trait&lt;mlir::gpu::GPUFuncOp&gt;, mlir::CallableOpInterface::Trait&lt;mlir::gpu::GPUFuncOp&gt;, mlir::FunctionOpInterface::Trait&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::IsIsolatedFromAbove&lt;mlir::gpu::GPUFuncOp&gt;, mlir::OpTrait::AffineScope&lt;mlir::gpu::GPUFuncOp&gt; &gt;(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:1645:29
    #<!-- -->4 0x55d735dcf05d in mlir::Op&lt;mlir::gpu::GPUFuncOp, mlir::OpTrait::OneRegion, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::HasParent&lt;mlir::gpu::GPUModuleOp&gt;::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AutomaticAllocationScope, mlir::SymbolOpInterface::Trait, mlir::CallableOpInterface::Trait, mlir::FunctionOpInterface::Trait, mlir::OpTrait::IsIsolatedFromAbove, mlir::OpTrait::AffineScope&gt;::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OpDefinition.h:2052:16
    #<!-- -->5 0x55d735dcafc0 in mlir::RegisteredOperationName::Model&lt;mlir::gpu::GPUFuncOp&gt;::verifyInvariants(mlir::Operation*) /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:555:14
    #<!-- -->6 0x55d73ea0fe8c in mlir::OperationName::verifyInvariants(mlir::Operation*) const /workdir/llvm-project/mlir/include/mlir/IR/OperationSupport.h:314:23
    #<!-- -->7 0x55d73ea0fe8c in (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:175:48
    #<!-- -->8 0x55d73ea0fe8c in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
    #<!-- -->9 0x55d73ea0fe8c in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #<!-- -->10 0x55d73ea0fe8c in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
    #<!-- -->11 0x55d73ea0fe8c in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #<!-- -->12 0x55d73ea1524c in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:16
    #<!-- -->13 0x55d73ea1524c in auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
    #<!-- -->14 0x55d73ea10940 in llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:18
    #<!-- -->15 0x55d73ea10940 in void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
    #<!-- -->16 0x55d73ea10940 in void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
    #<!-- -->17 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
    #<!-- -->18 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
    #<!-- -->19 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #<!-- -->20 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
    #<!-- -->21 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #<!-- -->22 0x55d73ea1524c in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:16
    #<!-- -->23 0x55d73ea1524c in auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
    #<!-- -->24 0x55d73ea10940 in llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:18
    #<!-- -->25 0x55d73ea10940 in void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
    #<!-- -->26 0x55d73ea10940 in void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
    #<!-- -->27 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
    #<!-- -->28 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
    #<!-- -->29 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #<!-- -->30 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
    #<!-- -->31 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #<!-- -->32 0x55d73ea1524c in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:16
    #<!-- -->33 0x55d73ea1524c in auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
    #<!-- -->34 0x55d73ea10940 in llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:18
    #<!-- -->35 0x55d73ea10940 in void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
    #<!-- -->36 0x55d73ea10940 in void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
    #<!-- -->37 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
    #<!-- -->38 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
    #<!-- -->39 0x55d73ea10940 in auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
    #<!-- -->40 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
    #<!-- -->41 0x55d73ea10940 in (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
    #<!-- -->42 0x55d73ea0e9f2 in mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:425:19
    #<!-- -->43 0x55d746100d68 in (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:55
    #<!-- -->44 0x55d746100d68 in (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
    #<!-- -->45 0x55d746100d68 in mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
    #<!-- -->46 0x55d73e4691dd in mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:46:10
    #<!-- -->47 0x55d73e466424 in mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:14
    #<!-- -->48 0x55d73e465cd7 in mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:189:10
    #<!-- -->49 0x55d73e465cd7 in mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
    #<!-- -->50 0x55d73e4646fe in performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:459:33
    #<!-- -->51 0x55d73e463fe3 in processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
    #<!-- -->52 0x55d73e463fe3 in mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
    #<!-- -->53 0x55d73e463fe3 in llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #<!-- -->54 0x55d73e69ceab in llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->55 0x55d73e69d4f6 in mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:16
    #<!-- -->56 0x55d73e69c7ca in void llvm::interleave&lt;llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;)::'lambda'(), void&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2216:3
    #<!-- -->57 0x55d73e69c7ca in void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2238:3
    #<!-- -->58 0x55d73e69c7ca in mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:89:3
    #<!-- -->59 0x55d73e452b1c in mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
    #<!-- -->60 0x55d73e453404 in mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:14
    #<!-- -->61 0x55d73e453b52 in mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
    #<!-- -->62 0x55d734387986 in main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:33
    #<!-- -->63 0x7fdda04ffd8f  (/lib/x86_64-linux-gnu/libc.so.6+0x29d8f) (BuildId: d5197096f709801829b118af1b7cf6631efa2dcd)
    #<!-- -->64 0x7fdda04ffe3f in __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e3f) (BuildId: d5197096f709801829b118af1b7cf6631efa2dcd)
    #<!-- -->65 0x55d7342c6ee4 in _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee4) (BuildId: e970ca4352d984cb)

0x602000006978 is located 0 bytes to the right of 8-byte region [0x602000006970,0x602000006978)
allocated by thread T0 here:
    #<!-- -->0 0x55d734384b7d in operator new(unsigned long) (/workdir/llvm-project/build/bin/mlir-opt+0xf673b7d) (BuildId: e970ca4352d984cb)
    #<!-- -->1 0x55d73e755bb1 in void std::vector&lt;mlir::BlockArgument, std::allocator&lt;mlir::BlockArgument&gt; &gt;::_M_realloc_insert&lt;mlir::BlockArgument const&amp;&gt;(__gnu_cxx::__normal_iterator&lt;mlir::BlockArgument*, std::vector&lt;mlir::BlockArgument, std::allocator&lt;mlir::BlockArgument&gt; &gt; &gt;, mlir::BlockArgument const&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/vector.tcc:440:33
    #<!-- -->2 0x55d73e7515a5 in mlir::Block::addArgument(mlir::Type, mlir::Location) /workdir/llvm-project/mlir/lib/IR/Block.cpp:155:13
    #<!-- -->3 0x55d74612632c in (anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13::operator()() const::'lambda'(mlir::OpAsmParser::UnresolvedOperand, mlir::Type)::operator()(mlir::OpAsmParser::UnresolvedOperand, mlir::Type) const /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2460:26
    #<!-- -->4 0x55d74612632c in llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser::UnresolvedOperand, mlir::Type)&gt;::callback_fn&lt;(anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13::operator()() const::'lambda'(mlir::OpAsmParser::UnresolvedOperand, mlir::Type)&gt;(long, mlir::OpAsmParser::UnresolvedOperand, mlir::Type) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #<!-- -->5 0x55d74612632c in llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser::UnresolvedOperand, mlir::Type)&gt;::operator()(mlir::OpAsmParser::UnresolvedOperand, mlir::Type) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->6 0x55d74612632c in (anonymous namespace)::OperationParser::parseSSADefOrUseAndType(llvm::function_ref&lt;llvm::ParseResult (mlir::OpAsmParser::UnresolvedOperand, mlir::Type)&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1140:10
    #<!-- -->7 0x55d74612632c in (anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13::operator()() const /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2443:12
    #<!-- -->8 0x55d74612632c in llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult ()&gt;::callback_fn&lt;(anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*)::$_13&gt;(long) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #<!-- -->9 0x55d7460f9d3c in llvm::function_ref&lt;llvm::ParseResult ()&gt;::operator()() const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->10 0x55d7460f9d3c in mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref&lt;llvm::ParseResult ()&gt;, llvm::StringRef) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:138:7
    #<!-- -->11 0x55d746124246 in (anonymous namespace)::OperationParser::parseOptionalBlockArgList(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2442:10
    #<!-- -->12 0x55d746124246 in (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2383:9
    #<!-- -->13 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
    #<!-- -->14 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
    #<!-- -->15 0x55d74611b67a in (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&amp;, std::optional&lt;llvm::ArrayRef&lt;mlir::OpAsmParser::UnresolvedOperand&gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::Block*&gt; &gt;, std::optional&lt;llvm::MutableArrayRef&lt;std::unique_ptr&lt;mlir::Region, std::default_delete&lt;mlir::Region&gt; &gt; &gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::NamedAttribute&gt; &gt;, std::optional&lt;mlir::Attribute&gt;, std::optional&lt;mlir::FunctionType&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
    #<!-- -->16 0x55d74610b05d in (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
    #<!-- -->17 0x55d746102c82 in (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
    #<!-- -->18 0x55d746125a7a in (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2404:9
    #<!-- -->19 0x55d746123e26 in (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2334:12
    #<!-- -->20 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
    #<!-- -->21 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
    #<!-- -->22 0x55d74611b67a in (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&amp;, std::optional&lt;llvm::ArrayRef&lt;mlir::OpAsmParser::UnresolvedOperand&gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::Block*&gt; &gt;, std::optional&lt;llvm::MutableArrayRef&lt;std::unique_ptr&lt;mlir::Region, std::default_delete&lt;mlir::Region&gt; &gt; &gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::NamedAttribute&gt; &gt;, std::optional&lt;mlir::Attribute&gt;, std::optional&lt;mlir::FunctionType&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
    #<!-- -->23 0x55d74610b05d in (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
    #<!-- -->24 0x55d746102c82 in (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
    #<!-- -->25 0x55d746125a7a in (anonymous namespace)::OperationParser::parseBlockBody(mlir::Block*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2404:9
    #<!-- -->26 0x55d746123e26 in (anonymous namespace)::OperationParser::parseBlock(mlir::Block*&amp;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2334:12
    #<!-- -->27 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegionBody(mlir::Region&amp;, llvm::SMLoc, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2292:7
    #<!-- -->28 0x55d74611d54c in (anonymous namespace)::OperationParser::parseRegion(mlir::Region&amp;, llvm::ArrayRef&lt;mlir::OpAsmParser::Argument&gt;, bool) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2221:7
    #<!-- -->29 0x55d74611b67a in (anonymous namespace)::OperationParser::parseGenericOperationAfterOpName(mlir::OperationState&amp;, std::optional&lt;llvm::ArrayRef&lt;mlir::OpAsmParser::UnresolvedOperand&gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::Block*&gt; &gt;, std::optional&lt;llvm::MutableArrayRef&lt;std::unique_ptr&lt;mlir::Region, std::default_delete&lt;mlir::Region&gt; &gt; &gt; &gt;, std::optional&lt;llvm::ArrayRef&lt;mlir::NamedAttribute&gt; &gt;, std::optional&lt;mlir::Attribute&gt;, std::optional&lt;mlir::FunctionType&gt;) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1430:13
    #<!-- -->30 0x55d74610b05d in (anonymous namespace)::OperationParser::parseGenericOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1527:7
    #<!-- -->31 0x55d746102c82 in (anonymous namespace)::OperationParser::parseOperation() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:1270:10
    #<!-- -->32 0x55d74610006b in (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2834:20
    #<!-- -->33 0x55d74610006b in mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
    #<!-- -->34 0x55d73e4691dd in mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:46:10
    #<!-- -->35 0x55d73e466424 in mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:14
    #<!-- -->36 0x55d73e465cd7 in mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:189:10
    #<!-- -->37 0x55d73e465cd7 in mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
    #<!-- -->38 0x55d73e4646fe in performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:459:33
    #<!-- -->39 0x55d73e463fe3 in processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
    #<!-- -->40 0x55d73e463fe3 in mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
    #<!-- -->41 0x55d73e463fe3 in llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
    #<!-- -->42 0x55d73e69ceab in llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->43 0x55d73e69d4f6 in mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:16

SUMMARY: AddressSanitizer: heap-buffer-overflow /workdir/llvm-project/mlir/lib/Dialect/GPU/IR/GPUDialect.cpp:552:16 in verifyAttributions(mlir::Operation*, llvm::ArrayRef&lt;mlir::BlockArgument&gt;, mlir::gpu::AddressSpace)
Shadow bytes around the buggy address:
  0x0c047fff8cd0: fa fa 00 fa fa fa 00 00 fa fa 00 00 fa fa 00 fa
  0x0c047fff8ce0: fa fa 00 fa fa fa 00 00 fa fa 00 00 fa fa 00 00
  0x0c047fff8cf0: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
  0x0c047fff8d00: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
  0x0c047fff8d10: fa fa 00 00 fa fa 00 00 fa fa 00 00 fa fa 00 00
=&gt;0x0c047fff8d20: fa fa 00 00 fa fa 00 fa fa fa 00 00 fa fa 00[fa]
  0x0c047fff8d30: fa fa fd fd fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d40: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d50: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d60: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c047fff8d70: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==2064283==ABORTING
```

</details>


---

### Comment 3 - meshtag

That attribute tells you about the number of buffers located in the workgroup memory. It doesn't make sense for it to be negative. Nevertheless, I think we should emit a clean error (with a helpful message) for this and not crash. 

I am guessing a similar problem would also exist for the `LaunchOp` in gpu dialect since it uses a similar attribute.

---

