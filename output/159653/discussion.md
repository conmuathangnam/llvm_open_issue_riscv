# [MLIR] Shape dialect null pointer crashes when verifying shape library attributes

**Author:** sairam2661
**URL:** https://github.com/llvm/llvm-project/issues/159653
**Status:** Closed
**Labels:** crash, mlir:shape
**Closed Date:** 2026-03-04T14:37:05Z

## Body

Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The shape dialect crashes when verifying operations with the `shape.lib` attributes that reference some non-existing library (possibly due to a null pointer check?).

**Steps to reproduce**
- MLIR program (test1.mlir)
```mlir
"builtin.module"() ({
  "func.func"() <{function_type = (tensor<10xf32>) -> tensor<10xf32>, sym_name = "test_func"}> ({
  	^bb0(%arg0: tensor<10xf32>):
    "func.return"(%arg0) : (tensor<10xf32>) -> ()
	}) {shape.function = @shape_lib::@test_shape} : () -> ()
  
  "shape.function_library"() <{sym_name = "shape_lib"}> ({
    "shape.func"() <{
      function_type = (!shape.value_shape) -> !shape.shape,
      sym_name = "test_shape"
    }> ({
    ^bb0(%arg0: !shape.value_shape):
      %0 = "shape.shape_of"(%arg0) : (!shape.value_shape) -> !shape.shape
      "shape.return"(%0) : (!shape.shape) -> ()
    }) : () -> ()
  }) : () -> ()
}) {shape.lib = [@shape_lib]} : () -> ()
```
- Minimized MLIR program (test2.mlir)
```
"builtin.module"() ({
}) {shape.lib = [@missing]} : () -> ()
```
**Command**
```
mlir-opt test1.mlir
mlir-opt test2.mlir
```

**Output**
- test1.mlir,
```
/workdir/llvm-project/build/tools/mlir/include/mlir/IR/BuiltinAttributes.cpp.inc:259:21: runtime error: member access within null pointer of type 'mlir::detail::StorageUserBase<mlir::DictionaryAttr, mlir::Attribute, mlir::detail::DictionaryAttrStorage, mlir::detail::AttributeUniquer>::ImplType' (aka 'mlir::detail::DictionaryAttrStorage')
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior /workdir/llvm-project/build/tools/mlir/include/mlir/IR/BuiltinAttributes.cpp.inc:259:21 in 
```
- test2.mlir,
```
mlir-opt: /workdir/llvm-project/llvm/include/llvm/Support/Casting.h:662: decltype(auto) llvm::dyn_cast(From *) [To = mlir::shape::FunctionLibraryOp, From = mlir::Operation]: Assertion `detail::isPresent(Val) && "dyn_cast on a non-existent value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt -split-input-file test2.mlir
 #0 0x00005603cf596a1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #1 0x00005603cf81bcfc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #2 0x00005603cf81617f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #3 0x00005603cf81e316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f479e927520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007f479e97b9fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007f479e927476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007f479e90d7f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007f479e90d71b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007f479e91ee96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x00005603d612da06 decltype(auto) llvm::dyn_cast<mlir::shape::FunctionLibraryOp, mlir::Operation>(mlir::Operation*) /workdir/llvm-project/llvm/include/llvm/Support/Casting.h:662:3
#11 0x00005603d612da06 mlir::shape::ShapeDialect::verifyOperationAttribute(mlir::Operation*, mlir::NamedAttribute) /workdir/llvm-project/mlir/lib/Dialect/Shape/IR/Shape.cpp:196:27
#12 0x00005603d9ca3d28 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#13 0x00005603d9ca3d28 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#14 0x00005603d9ca3d28 (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:167:11
#15 0x00005603d9ca3d28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
#16 0x00005603d9ca3d28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_2&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#17 0x00005603d9ca3d28 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
#18 0x00005603d9ca3d28 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#19 0x00005603d9ca924d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#20 0x00005603d9ca924d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#21 0x00005603d9ca924d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#22 0x00005603d9ca924d auto void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)::operator()<mlir::Operation*&>(mlir::Operation**&&) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#23 0x00005603d9ca4941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#24 0x00005603d9ca4941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#25 0x00005603d9ca4941 llvm::LogicalResult mlir::failableParallelForEach<mlir::Operation**, void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&)::'lambda'(mlir::Operation**&&)>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#26 0x00005603d9ca4941 void mlir::parallelForEach<mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#27 0x00005603d9ca4941 void mlir::parallelForEach<llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3>(mlir::MLIRContext*, llvm::SmallVector<mlir::Operation*, 6u>&, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&)::$_3&&) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#28 0x00005603d9ca4941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#29 0x00005603d9ca4941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0::operator()<mlir::Operation>(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#30 0x00005603d9ca4941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_1::operator()<(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0>((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&)::$_0&&, llvm::PointerUnion<mlir::Operation*, mlir::Block*>) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#31 0x00005603d9ca4941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#32 0x00005603d9ca4941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#33 0x00005603d9ca29f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#34 0x00005603e1394d69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#35 0x00005603e1394d69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#36 0x00005603e1394d69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#37 0x00005603e1394d69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#38 0x00005603e1394d69 mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#39 0x00005603d96fd1de mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#40 0x00005603d96fa425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#41 0x00005603d96fa425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#42 0x00005603d96fa425 mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#43 0x00005603d96f9cd8 std::enable_if<__and_<std::__not_<std::__is_tuple_like<mlir::ModuleOp> >, std::is_move_constructible<mlir::ModuleOp>, std::is_move_assignable<mlir::ModuleOp> >::value, void>::type std::swap<mlir::ModuleOp>(mlir::ModuleOp&, mlir::ModuleOp&) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#44 0x00005603d96f9cd8 mlir::OwningOpRef<mlir::ModuleOp>::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#45 0x00005603d96f9cd8 mlir::OwningOpRef<mlir::ModuleOp>::operator mlir::OwningOpRef<mlir::Operation*>() && /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#46 0x00005603d96f9cd8 mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#47 0x00005603d96f86ff performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#48 0x00005603d96f7fe4 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#49 0x00005603d96f7fe4 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#50 0x00005603d96f7fe4 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_3>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#51 0x00005603d9930eac std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#52 0x00005603d9930eac llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#53 0x00005603d99314f7 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#54 0x00005603d99314f7 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#55 0x00005603d99314f7 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:9
#56 0x00005603d99307cb void llvm::interleave<llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&)::'lambda'(), void>(llvm::SmallVector<llvm::StringRef, 8u>, llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:0:3
#57 0x00005603d99307cb void llvm::interleave<llvm::SmallVector<llvm::StringRef, 8u>, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef>(llvm::SmallVector<llvm::StringRef, 8u> const&, llvm::raw_ostream&, mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2238:3
#58 0x00005603d99307cb mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:89:3
#59 0x00005603d96e6b1d mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer> >, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#60 0x00005603d96e7405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#61 0x00005603d96e7405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#62 0x00005603d96e7405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#63 0x00005603d96e7b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#64 0x00005603cf61b987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#65 0x00005603cf61b987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#66 0x00005603cf61b987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#67 0x00007f479e90ed90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#68 0x00007f479e90ee40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#69 0x00005603cf55aee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (sairam2661)

<details>
Test commit: [5a86dc996c26299de63effc927075dcbfb924167](https://github.com/llvm/llvm-project/pull/157520/commits/5a86dc996c26299de63effc927075dcbfb924167)

**Description**:
The shape dialect crashes when verifying operations with the `shape.lib` attributes that reference some non-existing library (possibly due to a null pointer check?).

**Steps to reproduce**
- MLIR program (test1.mlir)
```
"builtin.module"() ({
  "func.func"() &lt;{function_type = (tensor&lt;10xf32&gt;) -&gt; tensor&lt;10xf32&gt;, sym_name = "test_func"}&gt; ({
  	^bb0(%arg0: tensor&lt;10xf32&gt;):
    "func.return"(%arg0) : (tensor&lt;10xf32&gt;) -&gt; ()
	}) {shape.function = @<!-- -->shape_lib::@<!-- -->test_shape} : () -&gt; ()
  
  "shape.function_library"() &lt;{sym_name = "shape_lib"}&gt; ({
    "shape.func"() &lt;{
      function_type = (!shape.value_shape) -&gt; !shape.shape,
      sym_name = "test_shape"
    }&gt; ({
    ^bb0(%arg0: !shape.value_shape):
      %0 = "shape.shape_of"(%arg0) : (!shape.value_shape) -&gt; !shape.shape
      "shape.return"(%0) : (!shape.shape) -&gt; ()
    }) : () -&gt; ()
  }) : () -&gt; ()
}) {shape.lib = [@<!-- -->shape_lib]} : () -&gt; ()
```
- Minimized MLIR program (test2.mlir)
```
"builtin.module"() ({
}) {shape.lib = [@<!-- -->missing]} : () -&gt; ()
```
**Command**
```
mlir-opt test1.mlir
mlir-opt test2.mlir
```

**Output**
- test1.mlir,
```
/workdir/llvm-project/build/tools/mlir/include/mlir/IR/BuiltinAttributes.cpp.inc:259:21: runtime error: member access within null pointer of type 'mlir::detail::StorageUserBase&lt;mlir::DictionaryAttr, mlir::Attribute, mlir::detail::DictionaryAttrStorage, mlir::detail::AttributeUniquer&gt;::ImplType' (aka 'mlir::detail::DictionaryAttrStorage')
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior /workdir/llvm-project/build/tools/mlir/include/mlir/IR/BuiltinAttributes.cpp.inc:259:21 in 
```
- test2.mlir,
```
mlir-opt: /workdir/llvm-project/llvm/include/llvm/Support/Casting.h:662: decltype(auto) llvm::dyn_cast(From *) [To = mlir::shape::FunctionLibraryOp, From = mlir::Operation]: Assertion `detail::isPresent(Val) &amp;&amp; "dyn_cast on a non-existent value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /workdir/llvm-project/build/bin/mlir-opt -split-input-file test2.mlir
 #<!-- -->0 0x00005603cf596a1b backtrace (/workdir/llvm-project/build/bin/mlir-opt+0xf5f1a1b)
 #<!-- -->1 0x00005603cf81bcfc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:13
 #<!-- -->2 0x00005603cf81617f llvm::sys::RunSignalHandlers() /workdir/llvm-project/llvm/lib/Support/Signals.cpp:105:18
 #<!-- -->3 0x00005603cf81e316 SignalHandler(int, siginfo_t*, void*) /workdir/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f479e927520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007f479e97b9fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007f479e927476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007f479e90d7f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007f479e90d71b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007f479e91ee96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x00005603d612da06 decltype(auto) llvm::dyn_cast&lt;mlir::shape::FunctionLibraryOp, mlir::Operation&gt;(mlir::Operation*) /workdir/llvm-project/llvm/include/llvm/Support/Casting.h:662:3
#<!-- -->11 0x00005603d612da06 mlir::shape::ShapeDialect::verifyOperationAttribute(mlir::Operation*, mlir::NamedAttribute) /workdir/llvm-project/mlir/lib/Dialect/Shape/IR/Shape.cpp:196:27
#<!-- -->12 0x00005603d9ca3d28 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->13 0x00005603d9ca3d28 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->14 0x00005603d9ca3d28 (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:167:11
#<!-- -->15 0x00005603d9ca3d28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:296:45
#<!-- -->16 0x00005603d9ca3d28 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_2&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->17 0x00005603d9ca3d28 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:295:16
#<!-- -->18 0x00005603d9ca3d28 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->19 0x00005603d9ca924d llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->20 0x00005603d9ca924d llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->21 0x00005603d9ca924d (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3::operator()(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:225:9
#<!-- -->22 0x00005603d9ca924d auto void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)::operator()&lt;mlir::Operation*&amp;&gt;(mlir::Operation**&amp;&amp;) const /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:120:12
#<!-- -->23 0x00005603d9ca4941 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->24 0x00005603d9ca4941 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->25 0x00005603d9ca4941 llvm::LogicalResult mlir::failableParallelForEach&lt;mlir::Operation**, void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;)::'lambda'(mlir::Operation**&amp;&amp;)&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:46:11
#<!-- -->26 0x00005603d9ca4941 void mlir::parallelForEach&lt;mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, mlir::Operation**, mlir::Operation**, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:119:9
#<!-- -->27 0x00005603d9ca4941 void mlir::parallelForEach&lt;llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&gt;(mlir::MLIRContext*, llvm::SmallVector&lt;mlir::Operation*, 6u&gt;&amp;, (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;)::$_3&amp;&amp;) /workdir/llvm-project/mlir/include/mlir/IR/Threading.h:131:3
#<!-- -->28 0x00005603d9ca4941 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:224:3
#<!-- -->29 0x00005603d9ca4941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0::operator()&lt;mlir::Operation&gt;(mlir::Operation*) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:53
#<!-- -->30 0x00005603d9ca4941 auto (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_1::operator()&lt;(anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&gt;((anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;)::$_0&amp;&amp;, llvm::PointerUnion&lt;mlir::Operation*, mlir::Block*&gt;) const /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:276:16
#<!-- -->31 0x00005603d9ca4941 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:287:15
#<!-- -->32 0x00005603d9ca4941 (anonymous namespace)::OperationVerifier::verifyOpAndDominance(mlir::Operation&amp;) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:81:14
#<!-- -->33 0x00005603d9ca29f3 mlir::verify(mlir::Operation*, bool) /workdir/llvm-project/mlir/lib/IR/Verifier.cpp:426:1
#<!-- -->34 0x00005603e1394d69 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->35 0x00005603e1394d69 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->36 0x00005603e1394d69 (anonymous namespace)::OperationParser::finalize() /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:931:48
#<!-- -->37 0x00005603e1394d69 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2840:20
#<!-- -->38 0x00005603e1394d69 mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workdir/llvm-project/mlir/lib/AsmParser/Parser.cpp:2894:41
#<!-- -->39 0x00005603d96fd1de mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workdir/llvm-project/mlir/lib/Parser/Parser.cpp:47:1
#<!-- -->40 0x00005603d96fa425 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->41 0x00005603d96fa425 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->42 0x00005603d96fa425 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workdir/llvm-project/mlir/include/mlir/Parser/Parser.h:159:7
#<!-- -->43 0x00005603d96f9cd8 std::enable_if&lt;__and_&lt;std::__not_&lt;std::__is_tuple_like&lt;mlir::ModuleOp&gt; &gt;, std::is_move_constructible&lt;mlir::ModuleOp&gt;, std::is_move_assignable&lt;mlir::ModuleOp&gt; &gt;::value, void&gt;::type std::swap&lt;mlir::ModuleOp&gt;(mlir::ModuleOp&amp;, mlir::ModuleOp&amp;) /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/move.h:205:11
#<!-- -->44 0x00005603d96f9cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::release() /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:69:5
#<!-- -->45 0x00005603d96f9cd8 mlir::OwningOpRef&lt;mlir::ModuleOp&gt;::operator mlir::OwningOpRef&lt;mlir::Operation*&gt;() &amp;&amp; /workdir/llvm-project/mlir/include/mlir/IR/OwningOpRef.h:64:51
#<!-- -->46 0x00005603d96f9cd8 mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workdir/llvm-project/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->47 0x00005603d96f86ff performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:0:33
#<!-- -->48 0x00005603d96f7fe4 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:551:12
#<!-- -->49 0x00005603d96f7fe4 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:635:12
#<!-- -->50 0x00005603d96f7fe4 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_3&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->51 0x00005603d9930eac std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;::~unique_ptr() /usr/bin/../lib/gcc/x86_64-linux-gnu/11/../../../../include/c++/11/bits/unique_ptr.h:360:6
#<!-- -->52 0x00005603d9930eac llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;) const /workdir/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->53 0x00005603d99314f7 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->54 0x00005603d99314f7 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->55 0x00005603d99314f7 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0::operator()(llvm::StringRef) const /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:86:9
#<!-- -->56 0x00005603d99307cb void llvm::interleave&lt;llvm::StringRef const*, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;)::'lambda'(), void&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:0:3
#<!-- -->57 0x00005603d99307cb void llvm::interleave&lt;llvm::SmallVector&lt;llvm::StringRef, 8u&gt;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::raw_ostream, llvm::StringRef&gt;(llvm::SmallVector&lt;llvm::StringRef, 8u&gt; const&amp;, llvm::raw_ostream&amp;, mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef)::$_0, llvm::StringRef const&amp;) /workdir/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2238:3
#<!-- -->58 0x00005603d99307cb mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workdir/llvm-project/mlir/lib/Support/ToolUtilities.cpp:89:3
#<!-- -->59 0x00005603d96e6b1d mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt; &gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:638:10
#<!-- -->60 0x00005603d96e7405 llvm::LogicalResult::failed() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:43:43
#<!-- -->61 0x00005603d96e7405 llvm::failed(llvm::LogicalResult) /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:71:58
#<!-- -->62 0x00005603d96e7405 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:679:7
#<!-- -->63 0x00005603d96e7b53 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workdir/llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:695:10
#<!-- -->64 0x00005603cf61b987 llvm::LogicalResult::succeeded() const /workdir/llvm-project/llvm/include/llvm/Support/LogicalResult.h:40:45
#<!-- -->65 0x00005603cf61b987 mlir::asMainReturnCode(llvm::LogicalResult) /workdir/llvm-project/mlir/include/mlir/Tools/mlir-opt/MlirOptMain.h:355:12
#<!-- -->66 0x00005603cf61b987 main /workdir/llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:339:10
#<!-- -->67 0x00007f479e90ed90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->68 0x00007f479e90ee40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->69 0x00005603cf55aee5 _start (/workdir/llvm-project/build/bin/mlir-opt+0xf5b5ee5)
Aborted (core dumped)
```
</details>


---

