# mlir-opt crash: Segmentation fault in arith.uitofp folding with `i0` (zero-bit) integer types

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/180463
**Status:** Closed
**Labels:** crash, mlir:arith, mlir:vector
**Closed Date:** 2026-03-04T11:34:20Z

## Body

Mlir-opt Version: 22.1.0-rc1

Bug Op: arith.uitofp

Bug Pass: --canonicalize

Detailed Description:The mlir-opt tool crashes during canonicalization when trying to constant-fold an arith.uitofp operation whose input is a vector of zero-bit integers (vector<nx i0>).The folding logic for arith.uitofp eventually calls llvm::APInt::tcMSB, which expects the integer to have at least one bit to determine the Most Significant Bit. When provided with a bit-width of $0$, the logic performs an out-of-bounds memory access or invalid arithmetic in the IEEEFloat conversion component, leading to a crash.

```mlir
module {
  func.func @main() {
    %1 = vector.constant_mask [1] : vector<1xi1>
    %2 = arith.trunci %1 : vector<1xi1> to vector<1xi0>
    %3 = arith.uitofp %2 : vector<1xi0> to vector<1xf32>
    %4 = vector.transpose %3, [0] : vector<1xf32> to vector<1xf32>
    return
  }
}
```
```
mlir-opt --canonicalize test.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --canonicalize test.mlir
 #0 0x00005e3b37149e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (mlir-opt+0x252fe32)
 #1 0x00005e3b371465af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #2 0x00005e3b371466fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007b8afea45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00005e3b371e723b llvm::APInt::tcMSB(unsigned long const*, unsigned int) (mlir-opt+0x25cd23b)
 #5 0x00005e3b371cbbd5 llvm::detail::IEEEFloat::convertFromUnsignedParts(unsigned long const*, unsigned int, llvm::RoundingMode) (mlir-opt+0x25b1bd5)
 #6 0x00005e3b371cbd60 llvm::detail::IEEEFloat::convertFromAPInt(llvm::APInt const&, bool, llvm::RoundingMode) (mlir-opt+0x25b1d60)
 #7 0x00005e3b3e8014be mlir::Attribute mlir::constFoldCastOp<mlir::IntegerAttr, mlir::FloatAttr, llvm::APInt, llvm::APFloat, mlir::ub::PoisonAttr, mlir::arith::UIToFPOp::fold(mlir::arith::UIToFPOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>)::'lambda'(llvm::APInt const&, bool&)>(llvm::ArrayRef<mlir::Attribute>, mlir::Type, mlir::arith::UIToFPOp::fold(mlir::arith::UIToFPOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>)::'lambda'(llvm::APInt const&, bool&)&&) ArithOps.cpp:0:0
 #8 0x00005e3b3e801713 mlir::arith::UIToFPOp::fold(mlir::arith::UIToFPOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>) (mlir-opt+0x9be7713)
 #9 0x00005e3b3e854f44 llvm::LogicalResult llvm::detail::UniqueFunctionBase<llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&>::CallImpl<mlir::Op<mlir::arith::UIToFPOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::Type>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::OneOperand, mlir::OpTrait::OpInvariants, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::SameOperandsAndResultShape, mlir::CastOpInterface::Trait, mlir::VectorUnrollOpInterface::Trait, mlir::OpTrait::Elementwise, mlir::OpTrait::Scalarizable, mlir::OpTrait::Vectorizable, mlir::OpTrait::Tensorizable>::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) const>(void*, mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (mlir-opt+0x9c3af44)
#10 0x00005e3b3e8582d1 mlir::RegisteredOperationName::Model<mlir::arith::UIToFPOp>::foldHook(mlir::Operation*, llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (mlir-opt+0x9c3e2d1)
#11 0x00005e3b3ed5f074 mlir::Operation::fold(llvm::ArrayRef<mlir::Attribute>, llvm::SmallVectorImpl<mlir::OpFoldResult>&) (mlir-opt+0xa145074)
#12 0x00005e3b3ed5f4e3 mlir::Operation::fold(llvm::SmallVectorImpl<mlir::OpFoldResult>&) (mlir-opt+0xa1454e3)
#13 0x00005e3b3e93b603 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#14 0x00005e3b3e940060 mlir::applyPatternsGreedily(mlir::Region&, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*) (mlir-opt+0x9d26060)
#15 0x00005e3b3e88bac3 (anonymous namespace)::Canonicalizer::runOnOperation() Canonicalizer.cpp:0:0
#16 0x00005e3b3e9e9756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf756)
#17 0x00005e3b3e9e9a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#18 0x00005e3b3e9ea1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (mlir-opt+0x9dd01e2)
#19 0x00005e3b3e9eb400 mlir::PassManager::run(mlir::Operation*) (mlir-opt+0x9dd1400)
#20 0x00005e3b37210767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#21 0x00005e3b372114ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#22 0x00005e3b3edb9731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (mlir-opt+0xa19f731)
#23 0x00005e3b372088d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#24 0x00005e3b37211b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (mlir-opt+0x25f7b70)
#25 0x00005e3b37211da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (mlir-opt+0x25f7da9)
#26 0x00005e3b37043023 main (mlir-opt+0x2429023)
#27 0x00007b8afea2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#28 0x00007b8afea2a28b call_init ./csu/../csu/libc-start.c:128:20
#29 0x00007b8afea2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#30 0x00005e3b37128405 _start (mlir-opt+0x250e405)
Segmentation fault (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (compilerStudyer)

<details>
Mlir-opt Version: 22.1.0-rc1

Bug Op: arith.uitofp

Bug Pass: --canonicalize

Detailed Description:The mlir-opt tool crashes during canonicalization when trying to constant-fold an arith.uitofp operation whose input is a vector of zero-bit integers (vector&lt;nx i0&gt;).The folding logic for arith.uitofp eventually calls llvm::APInt::tcMSB, which expects the integer to have at least one bit to determine the Most Significant Bit. When provided with a bit-width of $0$, the logic performs an out-of-bounds memory access or invalid arithmetic in the IEEEFloat conversion component, leading to a crash.

```mlir
module {
  func.func @<!-- -->main() {
    %1 = vector.constant_mask [1] : vector&lt;1xi1&gt;
    %2 = arith.trunci %1 : vector&lt;1xi1&gt; to vector&lt;1xi0&gt;
    %3 = arith.uitofp %2 : vector&lt;1xi0&gt; to vector&lt;1xf32&gt;
    %4 = vector.transpose %3, [0] : vector&lt;1xf32&gt; to vector&lt;1xf32&gt;
    return
  }
}
```
```
mlir-opt --canonicalize test.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt --canonicalize test.mlir
 #<!-- -->0 0x00005e3b37149e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (mlir-opt+0x252fe32)
 #<!-- -->1 0x00005e3b371465af llvm::sys::RunSignalHandlers() (mlir-opt+0x252c5af)
 #<!-- -->2 0x00005e3b371466fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007b8afea45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00005e3b371e723b llvm::APInt::tcMSB(unsigned long const*, unsigned int) (mlir-opt+0x25cd23b)
 #<!-- -->5 0x00005e3b371cbbd5 llvm::detail::IEEEFloat::convertFromUnsignedParts(unsigned long const*, unsigned int, llvm::RoundingMode) (mlir-opt+0x25b1bd5)
 #<!-- -->6 0x00005e3b371cbd60 llvm::detail::IEEEFloat::convertFromAPInt(llvm::APInt const&amp;, bool, llvm::RoundingMode) (mlir-opt+0x25b1d60)
 #<!-- -->7 0x00005e3b3e8014be mlir::Attribute mlir::constFoldCastOp&lt;mlir::IntegerAttr, mlir::FloatAttr, llvm::APInt, llvm::APFloat, mlir::ub::PoisonAttr, mlir::arith::UIToFPOp::fold(mlir::arith::UIToFPOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::Attribute&gt;&gt;)::'lambda'(llvm::APInt const&amp;, bool&amp;)&gt;(llvm::ArrayRef&lt;mlir::Attribute&gt;, mlir::Type, mlir::arith::UIToFPOp::fold(mlir::arith::UIToFPOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::Attribute&gt;&gt;)::'lambda'(llvm::APInt const&amp;, bool&amp;)&amp;&amp;) ArithOps.cpp:0:0
 #<!-- -->8 0x00005e3b3e801713 mlir::arith::UIToFPOp::fold(mlir::arith::UIToFPOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::Attribute&gt;&gt;) (mlir-opt+0x9be7713)
 #<!-- -->9 0x00005e3b3e854f44 llvm::LogicalResult llvm::detail::UniqueFunctionBase&lt;llvm::LogicalResult, mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;&gt;::CallImpl&lt;mlir::Op&lt;mlir::arith::UIToFPOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::Type&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::OneOperand, mlir::OpTrait::OpInvariants, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::SameOperandsAndResultShape, mlir::CastOpInterface::Trait, mlir::VectorUnrollOpInterface::Trait, mlir::OpTrait::Elementwise, mlir::OpTrait::Scalarizable, mlir::OpTrait::Vectorizable, mlir::OpTrait::Tensorizable&gt;::getFoldHookFn()::'lambda'(mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) const&gt;(void*, mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) (mlir-opt+0x9c3af44)
#<!-- -->10 0x00005e3b3e8582d1 mlir::RegisteredOperationName::Model&lt;mlir::arith::UIToFPOp&gt;::foldHook(mlir::Operation*, llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) (mlir-opt+0x9c3e2d1)
#<!-- -->11 0x00005e3b3ed5f074 mlir::Operation::fold(llvm::ArrayRef&lt;mlir::Attribute&gt;, llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) (mlir-opt+0xa145074)
#<!-- -->12 0x00005e3b3ed5f4e3 mlir::Operation::fold(llvm::SmallVectorImpl&lt;mlir::OpFoldResult&gt;&amp;) (mlir-opt+0xa1454e3)
#<!-- -->13 0x00005e3b3e93b603 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
#<!-- -->14 0x00005e3b3e940060 mlir::applyPatternsGreedily(mlir::Region&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*) (mlir-opt+0x9d26060)
#<!-- -->15 0x00005e3b3e88bac3 (anonymous namespace)::Canonicalizer::runOnOperation() Canonicalizer.cpp:0:0
#<!-- -->16 0x00005e3b3e9e9756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (mlir-opt+0x9dcf756)
#<!-- -->17 0x00005e3b3e9e9a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (mlir-opt+0x9dcfa8e)
#<!-- -->18 0x00005e3b3e9ea1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (mlir-opt+0x9dd01e2)
#<!-- -->19 0x00005e3b3e9eb400 mlir::PassManager::run(mlir::Operation*) (mlir-opt+0x9dd1400)
#<!-- -->20 0x00005e3b37210767 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->21 0x00005e3b372114ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->22 0x00005e3b3edb9731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (mlir-opt+0xa19f731)
#<!-- -->23 0x00005e3b372088d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->24 0x00005e3b37211b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (mlir-opt+0x25f7b70)
#<!-- -->25 0x00005e3b37211da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (mlir-opt+0x25f7da9)
#<!-- -->26 0x00005e3b37043023 main (mlir-opt+0x2429023)
#<!-- -->27 0x00007b8afea2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->28 0x00007b8afea2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->29 0x00007b8afea2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->30 0x00005e3b37128405 _start (mlir-opt+0x250e405)
Segmentation fault (core dumped)
```
</details>


---

### Comment 2 - PragmaTwice

> zero-bit integers

hmmm is this even valid in MLIR?

---

### Comment 3 - woruyu

I would like to fix this!

---

### Comment 4 - woruyu

> hmmm is this even valid in MLIR?

I find lots of testcase with `i0` in `mlir/test/Dialect/Arith/canonicalize.mlir`, it seems that mlir allow use i0 type.
```llvm
// CHECK-LABEL: @extui_i0
//       CHECK:   %[[ZERO:.*]] = arith.constant 0 : i16
//       CHECK:   return %[[ZERO]] : i16
func.func @extui_i0() -> i16 {
  %c0 = arith.constant 0 : i0
  %extui = arith.extui %c0 : i0 to i16
  return %extui : i16
}
``` 

---

### Comment 5 - PragmaTwice

> it seems that mlir allow use i0 type

I don't think its appearance in test cases implies that it is valid. It may simply be due to the lack of clear specifications.

I am in favor of prohibiting `i0` for the following reasons:
1. LLVM IR does not support `i0` (as specified in the [LLVM LangRef](https://llvm.org/docs/LangRef.html#integer-type)).
2. If you check [the documentation for MLIR `IntegerType`](https://mlir.llvm.org/docs/Dialects/Builtin/#integertype), you will find that `i0` is excluded from its syntax definition (`i [1-9][0-9]*`). 🤷 
3. Without a clear justification, I prefer to minimize extra states to avoid introducing corner cases that could lead to maintenance issues (such as this one).

I'm not an MLIR LangRef expert so cc @joker-eph @jpienaar @ftynse @kuhar 

---

### Comment 6 - zero9178

> > it seems that mlir allow use i0 type
> 
> I don't think its appearance in test cases implies that it is valid. It may simply be due to the lack of clear specifications.
> 
> I am in favor of prohibiting `i0` for the following reasons:
> 
> 1. LLVM IR does not support `i0` (as specified in the [LLVM LangRef](https://llvm.org/docs/LangRef.html#integer-type)).
> 2. If you check [the documentation for MLIR `IntegerType`](https://mlir.llvm.org/docs/Dialects/Builtin/#integertype), you will find that `i0` is excluded from its syntax definition (`i [1-9][0-9]*`). 🤷
> 3. Without a clear justification, I prefer to minimize extra states to avoid introducing corner cases that could lead to maintenance issues (such as this one).
> 
> I'm not an MLIR LangRef expert so cc [@joker-eph](https://github.com/joker-eph) [@jpienaar](https://github.com/jpienaar) [@ftynse](https://github.com/ftynse) [@kuhar](https://github.com/kuhar)

The justification for i0 is that it generalizes relatively for many optimizations that e.g. do bitwidth reduction. In particularly this is used in the hardware space e.g. in Circt. See e.g. https://github.com/llvm/llvm-project/pull/87193, https://github.com/llvm/llvm-project/pull/89665 and https://discourse.llvm.org/t/rfc-support-zero-width-integers-in-the-comb-dialect-circt/78492/11
Updating the langref would definitely make sense. CC @Moxinilian 

---

### Comment 7 - PragmaTwice

> The justification for i0 is that it generalizes relatively for many optimizations that e.g. do bitwidth reduction. In particularly this is used in the hardware space e.g. in Circt. See e.g. [#87193](https://github.com/llvm/llvm-project/pull/87193), [#89665](https://github.com/llvm/llvm-project/pull/89665) and https://discourse.llvm.org/t/rfc-support-zero-width-integers-in-the-comb-dialect-circt/78492/11 Updating the langref would definitely make sense. CC [@Moxinilian](https://github.com/Moxinilian)

Ahh sorry don't know this RFC. It looks reasonable and indeed docs updating will help community members to be aware and understand it. For example in LLVMIR lowering we may need type conversion if `i0` is used in high-level IR modules.

Maybe we can have an ODS constraint like `IntegerTypeNonZero` for many scenarios/dialects where `i0` is not so meaningful (for machine learning or programming languages)?



---

### Comment 8 - kuhar

I'd be in favor or restricting core dialects to non-zero bitwidth integers; I'm not aware of any usecases for general compute.

Bug reports like this one are valid and appreciated, but I'd assign them very low priority because of how contrived i0 is.

---

