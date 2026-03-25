# mlir-opt --convert-func-to-spirv crashes on tosa ops with vector and spirv type

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/177775
**Status:** Closed
**Labels:** mlir:spirv, crash
**Closed Date:** 2026-03-09T15:30:39Z

## Body

# Description
When running mlir-opt --convert-func-to-spirv on a MLIR module containing tosa operations that produce vectors and a spirv.Variable, the tool crashes with an assertion failure in Casting.h
## Reproduce
mlir-opt version: 21.1.8
location: If the **spirv.Variable** operation is commented out, the crash no longer occurs.

```mlir
module {
  func.func @main() {
    %0 = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
    %1 = vector.step : vector<1xindex>
    %2 = spirv.Variable : !spirv.ptr<vector<1xindex>, Function>
    return
  }
}
```
```
mlir-opt --convert-func-to-spirv test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(const From&) [with To = mlir::spirv::SPIRVType; From = mlir::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --convert-func-to-spirv /mnt/sdd1/sbw/mlir/data/genProgram_0123_night_filerted/genProgram_0123_night_valid/241115_ffc2_5.mlir
 #0 0x0000573e52f91612 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #1 0x0000573e52f8dd7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #2 0x0000573e52f8decc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000075fa50845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000075fa5089eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000075fa5089eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000075fa5089eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000075fa5084527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000075fa508288ff abort ./stdlib/abort.c:81:7
 #9 0x000075fa5082881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000075fa5083b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000573e559b7161 mlir::spirv::PointerType::getExtensions(llvm::SmallVectorImpl<llvm::ArrayRef<mlir::spirv::Extension>>&, std::optional<mlir::spirv::StorageClass>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x4ac7161)
#12 0x0000573e559b6fcd mlir::spirv::SPIRVType::getExtensions(llvm::SmallVectorImpl<llvm::ArrayRef<mlir::spirv::Extension>>&, std::optional<mlir::spirv::StorageClass>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x4ac6fcd)
#13 0x0000573e5828e989 mlir::SPIRVConversionTarget::isLegalOp(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x739e989)
#14 0x0000573e5828eba3 std::_Function_handler<std::optional<bool> (mlir::Operation*), mlir::SPIRVConversionTarget::get(mlir::spirv::TargetEnvAttr)::'lambda'(mlir::Operation*)>::_M_invoke(std::_Any_data const&, mlir::Operation*&&) SPIRVConversion.cpp:0:0
#15 0x0000573e567dc63f mlir::ConversionTarget::isLegal(mlir::Operation*) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58ec63f)
#16 0x0000573e567f5740 mlir::WalkResult mlir::detail::walk<mlir::ForwardDominanceIterator<false>>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Operation*)>, mlir::WalkOrder) (.constprop.0) DialectConversion.cpp:0:0
#17 0x0000573e567f5c90 mlir::WalkResult mlir::detail::walk<mlir::ForwardDominanceIterator<false>>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Operation*)>, mlir::WalkOrder) (.constprop.0) DialectConversion.cpp:0:0
#18 0x0000573e567f5c90 mlir::WalkResult mlir::detail::walk<mlir::ForwardDominanceIterator<false>>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Operation*)>, mlir::WalkOrder) (.constprop.0) DialectConversion.cpp:0:0
#19 0x0000573e567f64fe mlir::OperationConverter::convertOperations(llvm::ArrayRef<mlir::Operation*>) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x59064fe)
#20 0x0000573e567f7bba applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#21 0x0000573e567f7cf2 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5907cf2)
#22 0x0000573e561f756c (anonymous namespace)::ConvertFuncToSPIRVPass::runOnOperation() FuncToSPIRVPass.cpp:0:0
#23 0x0000573e5674607e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#24 0x0000573e567464c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#25 0x0000573e56747523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#26 0x0000573e567370a7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#27 0x0000573e567379c6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#28 0x0000573e56737b8c llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#29 0x0000573e5684d24e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#30 0x0000573e56730080 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#31 0x0000573e56737cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#32 0x0000573e567381ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#33 0x0000573e52ea9577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#34 0x000075fa5082a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#35 0x000075fa5082a28b call_init ./csu/../csu/libc-start.c:128:20
#36 0x000075fa5082a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#37 0x0000573e52f62665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
# Description
When running mlir-opt --convert-func-to-spirv on a MLIR module containing tosa operations that produce vectors and a spirv.Variable, the tool crashes with an assertion failure in Casting.h
## Reproduce
mlir-opt version: 21.1.8
location: If the **spirv.Variable** operation is commented out, the crash no longer occurs.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = "tosa.const"() &lt;{values = dense&lt;0&gt; : tensor&lt;1xi32&gt;}&gt; : () -&gt; tensor&lt;1xi32&gt;
    %1 = vector.step : vector&lt;1xindex&gt;
    %2 = spirv.Variable : !spirv.ptr&lt;vector&lt;1xindex&gt;, Function&gt;
    return
  }
}
```
```
mlir-opt --convert-func-to-spirv test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::spirv::SPIRVType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --convert-func-to-spirv /mnt/sdd1/sbw/mlir/data/genProgram_0123_night_filerted/genProgram_0123_night_valid/241115_ffc2_5.mlir
 #<!-- -->0 0x0000573e52f91612 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #<!-- -->1 0x0000573e52f8dd7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #<!-- -->2 0x0000573e52f8decc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000075fa50845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000075fa5089eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000075fa5089eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000075fa5089eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000075fa5084527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000075fa508288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000075fa5082881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000075fa5083b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000573e559b7161 mlir::spirv::PointerType::getExtensions(llvm::SmallVectorImpl&lt;llvm::ArrayRef&lt;mlir::spirv::Extension&gt;&gt;&amp;, std::optional&lt;mlir::spirv::StorageClass&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x4ac7161)
#<!-- -->12 0x0000573e559b6fcd mlir::spirv::SPIRVType::getExtensions(llvm::SmallVectorImpl&lt;llvm::ArrayRef&lt;mlir::spirv::Extension&gt;&gt;&amp;, std::optional&lt;mlir::spirv::StorageClass&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x4ac6fcd)
#<!-- -->13 0x0000573e5828e989 mlir::SPIRVConversionTarget::isLegalOp(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x739e989)
#<!-- -->14 0x0000573e5828eba3 std::_Function_handler&lt;std::optional&lt;bool&gt; (mlir::Operation*), mlir::SPIRVConversionTarget::get(mlir::spirv::TargetEnvAttr)::'lambda'(mlir::Operation*)&gt;::_M_invoke(std::_Any_data const&amp;, mlir::Operation*&amp;&amp;) SPIRVConversion.cpp:0:0
#<!-- -->15 0x0000573e567dc63f mlir::ConversionTarget::isLegal(mlir::Operation*) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58ec63f)
#<!-- -->16 0x0000573e567f5740 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardDominanceIterator&lt;false&gt;&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) DialectConversion.cpp:0:0
#<!-- -->17 0x0000573e567f5c90 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardDominanceIterator&lt;false&gt;&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) DialectConversion.cpp:0:0
#<!-- -->18 0x0000573e567f5c90 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardDominanceIterator&lt;false&gt;&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) DialectConversion.cpp:0:0
#<!-- -->19 0x0000573e567f64fe mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x59064fe)
#<!-- -->20 0x0000573e567f7bba applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->21 0x0000573e567f7cf2 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5907cf2)
#<!-- -->22 0x0000573e561f756c (anonymous namespace)::ConvertFuncToSPIRVPass::runOnOperation() FuncToSPIRVPass.cpp:0:0
#<!-- -->23 0x0000573e5674607e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#<!-- -->24 0x0000573e567464c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#<!-- -->25 0x0000573e56747523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#<!-- -->26 0x0000573e567370a7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->27 0x0000573e567379c6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#<!-- -->28 0x0000573e56737b8c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->29 0x0000573e5684d24e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#<!-- -->30 0x0000573e56730080 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#<!-- -->31 0x0000573e56737cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#<!-- -->32 0x0000573e567381ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#<!-- -->33 0x0000573e52ea9577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#<!-- -->34 0x000075fa5082a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->35 0x000075fa5082a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x000075fa5082a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->37 0x0000573e52f62665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```
</details>


---

### Comment 2 - gdehame

Not exactly a fix to the bug but I read the docs and sources of the SPIR-V dialect and vector of index typed elements isn't meant to be allowed:
- SPIR-V `ScalarType` doesn't include `index`, see https://github.com/llvm/llvm-project/blob/main/mlir/lib/Dialect/SPIRV/IR/SPIRVTypes.cpp#L531
- SPIR-V vector's elements are expected to be of type `ScalarType`, see https://github.com/llvm/llvm-project/blob/main/mlir/lib/Dialect/SPIRV/IR/SPIRVTypes.cpp#L191

Thus I added a check in the verifier to make sure the element type of a SPIR-V vector is a `ScalarType` in the follownig PR:
https://github.com/llvm/llvm-project/pull/177787

This "fixes" the bug as it shouldn't trigger anymore due to failing at MLIR verifier level, not crashing in the pass.

Important to mention: the size of vectors might need to be updated in the verifier but I didn't add it since I figured there was an inconsistency between the doc, the current verifier, the `CompositeType::isValid` method and I don't know which of those make sense since I am not very knowledgable with regard to SPIR-V

---

### Comment 3 - Men-cotton

No crash at HEAD. I believe #180430 resolves this.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-mlir-spirv

Author: None (Subway2023)

<details>
# Description
When running mlir-opt --convert-func-to-spirv on a MLIR module containing tosa operations that produce vectors and a spirv.Variable, the tool crashes with an assertion failure in Casting.h
## Reproduce
mlir-opt version: 21.1.8
location: If the **spirv.Variable** operation is commented out, the crash no longer occurs.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = "tosa.const"() &lt;{values = dense&lt;0&gt; : tensor&lt;1xi32&gt;}&gt; : () -&gt; tensor&lt;1xi32&gt;
    %1 = vector.step : vector&lt;1xindex&gt;
    %2 = spirv.Variable : !spirv.ptr&lt;vector&lt;1xindex&gt;, Function&gt;
    return
  }
}
```
```
mlir-opt --convert-func-to-spirv test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::spirv::SPIRVType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --convert-func-to-spirv /mnt/sdd1/sbw/mlir/data/genProgram_0123_night_filerted/genProgram_0123_night_valid/241115_ffc2_5.mlir
 #<!-- -->0 0x0000573e52f91612 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #<!-- -->1 0x0000573e52f8dd7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #<!-- -->2 0x0000573e52f8decc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000075fa50845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000075fa5089eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000075fa5089eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000075fa5089eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000075fa5084527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000075fa508288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000075fa5082881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000075fa5083b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000573e559b7161 mlir::spirv::PointerType::getExtensions(llvm::SmallVectorImpl&lt;llvm::ArrayRef&lt;mlir::spirv::Extension&gt;&gt;&amp;, std::optional&lt;mlir::spirv::StorageClass&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x4ac7161)
#<!-- -->12 0x0000573e559b6fcd mlir::spirv::SPIRVType::getExtensions(llvm::SmallVectorImpl&lt;llvm::ArrayRef&lt;mlir::spirv::Extension&gt;&gt;&amp;, std::optional&lt;mlir::spirv::StorageClass&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x4ac6fcd)
#<!-- -->13 0x0000573e5828e989 mlir::SPIRVConversionTarget::isLegalOp(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x739e989)
#<!-- -->14 0x0000573e5828eba3 std::_Function_handler&lt;std::optional&lt;bool&gt; (mlir::Operation*), mlir::SPIRVConversionTarget::get(mlir::spirv::TargetEnvAttr)::'lambda'(mlir::Operation*)&gt;::_M_invoke(std::_Any_data const&amp;, mlir::Operation*&amp;&amp;) SPIRVConversion.cpp:0:0
#<!-- -->15 0x0000573e567dc63f mlir::ConversionTarget::isLegal(mlir::Operation*) const (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58ec63f)
#<!-- -->16 0x0000573e567f5740 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardDominanceIterator&lt;false&gt;&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) DialectConversion.cpp:0:0
#<!-- -->17 0x0000573e567f5c90 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardDominanceIterator&lt;false&gt;&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) DialectConversion.cpp:0:0
#<!-- -->18 0x0000573e567f5c90 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardDominanceIterator&lt;false&gt;&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) DialectConversion.cpp:0:0
#<!-- -->19 0x0000573e567f64fe mlir::OperationConverter::convertOperations(llvm::ArrayRef&lt;mlir::Operation*&gt;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x59064fe)
#<!-- -->20 0x0000573e567f7bba applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->21 0x0000573e567f7cf2 mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5907cf2)
#<!-- -->22 0x0000573e561f756c (anonymous namespace)::ConvertFuncToSPIRVPass::runOnOperation() FuncToSPIRVPass.cpp:0:0
#<!-- -->23 0x0000573e5674607e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#<!-- -->24 0x0000573e567464c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#<!-- -->25 0x0000573e56747523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#<!-- -->26 0x0000573e567370a7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->27 0x0000573e567379c6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#<!-- -->28 0x0000573e56737b8c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->29 0x0000573e5684d24e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#<!-- -->30 0x0000573e56730080 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#<!-- -->31 0x0000573e56737cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#<!-- -->32 0x0000573e567381ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#<!-- -->33 0x0000573e52ea9577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#<!-- -->34 0x000075fa5082a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->35 0x000075fa5082a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x000075fa5082a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->37 0x0000573e52f62665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```
</details>


---

