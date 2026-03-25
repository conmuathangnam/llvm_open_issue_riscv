# [Attributor] Assertion `getBitWidth() == CR.getBitWidth() && "ConstantRange types don't agree!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165693
**Status:** Open
**Labels:** crash, llvm::Attributor

## Body

Reproducer:
https://godbolt.org/z/P9ab6jdG6

Backtrace:
```console
opt: /root/llvm-project/llvm/lib/IR/ConstantRange.cpp:699: llvm::ConstantRange llvm::ConstantRange::unionWith(const llvm::ConstantRange&, llvm::ConstantRange::PreferredRangeType) const: Assertion `getBitWidth() == CR.getBitWidth() && "ConstantRange types don't agree!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=attributor <source>
1.	Running pass "attributor" on module "<source>"
 #0 0x000000000598f068 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x598f068)
 #1 0x000000000598bf14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007674e0442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007674e04969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007674e0442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007674e04287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007674e042871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007674e0439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000055b85c6 llvm::ConstantRange::unionWith(llvm::ConstantRange const&, llvm::ConstantRange::PreferredRangeType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x55b85c6)
 #9 0x00000000011b90d5 llvm::ChangeStatus llvm::clampStateAndIndicateChange<llvm::IntegerRangeState>(llvm::IntegerRangeState&, llvm::IntegerRangeState const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x11b90d5)
#10 0x00000000031b039b bool llvm::function_ref<bool (llvm::ArrayRef<llvm::Function const*>)>::callback_fn<(anonymous namespace)::AACalleeToCallSite<llvm::AAValueConstantRange, (anonymous namespace)::AAValueConstantRangeImpl, llvm::IntegerRangeState, true, (llvm::Attribute::AttrKind)0>::updateImpl(llvm::Attributor&)::'lambda'(llvm::ArrayRef<llvm::Function const*>)>(long, llvm::ArrayRef<llvm::Function const*>) AttributorAttributes.cpp:0:0
#11 0x0000000003121553 llvm::Attributor::checkForAllCallees(llvm::function_ref<bool (llvm::ArrayRef<llvm::Function const*>)>, llvm::AbstractAttribute const&, llvm::CallBase const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3121553)
#12 0x000000000315c76e (anonymous namespace)::AACalleeToCallSite<llvm::AAValueConstantRange, (anonymous namespace)::AAValueConstantRangeImpl, llvm::IntegerRangeState, true, (llvm::Attribute::AttrKind)0>::updateImpl(llvm::Attributor&) AttributorAttributes.cpp:0:0
#13 0x000000000310c916 llvm::AbstractAttribute::update(llvm::Attributor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x310c916)
#14 0x00000000031212b0 llvm::Attributor::updateAA(llvm::AbstractAttribute&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31212b0)
#15 0x00000000031aec03 llvm::AAValueConstantRange const* llvm::Attributor::getOrCreateAAFor<llvm::AAValueConstantRange>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31aec03)
#16 0x00000000031ba49b (anonymous namespace)::AAPotentialValuesImpl::addValue(llvm::Attributor&, llvm::PotentialValuesState<std::pair<llvm::AA::ValueAndContext, llvm::AA::ValueScope>>&, llvm::Value&, llvm::Instruction const*, llvm::AA::ValueScope, llvm::Function*) const AttributorAttributes.cpp:0:0
#17 0x00000000031ba738 (anonymous namespace)::AAPotentialValuesImpl::recurseForValue(llvm::Attributor&, llvm::IRPosition const&, llvm::AA::ValueScope) AttributorAttributes.cpp:0:0
#18 0x00000000031cc7fe (anonymous namespace)::AAPotentialValuesFloating::updateImpl(llvm::Attributor&) AttributorAttributes.cpp:0:0
#19 0x000000000310c916 llvm::AbstractAttribute::update(llvm::Attributor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x310c916)
#20 0x00000000031212b0 llvm::Attributor::updateAA(llvm::AbstractAttribute&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31212b0)
#21 0x0000000003129c9b llvm::AAPotentialValues const* llvm::Attributor::getOrCreateAAFor<llvm::AAPotentialValues>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3129c9b)
#22 0x0000000003129f50 llvm::Attributor::getAssumedSimplifiedValues(llvm::IRPosition const&, llvm::AbstractAttribute const*, llvm::SmallVectorImpl<llvm::AA::ValueAndContext>&, llvm::AA::ValueScope, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3129f50)
#23 0x00000000031b8045 (anonymous namespace)::AAPotentialConstantValuesImpl::fillSetWithConstantValues(llvm::Attributor&, llvm::IRPosition const&, llvm::SmallSetVector<llvm::APInt, 8u>&, bool&, bool) AttributorAttributes.cpp:0:0
#24 0x00000000031b9707 (anonymous namespace)::AAPotentialConstantValuesFloating::updateWithInstruction(llvm::Attributor&, llvm::Instruction*) AttributorAttributes.cpp:0:0
#25 0x000000000310c916 llvm::AbstractAttribute::update(llvm::Attributor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x310c916)
#26 0x00000000031212b0 llvm::Attributor::updateAA(llvm::AbstractAttribute&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31212b0)
#27 0x00000000031b7f0b llvm::AAPotentialConstantValues const* llvm::Attributor::getOrCreateAAFor<llvm::AAPotentialConstantValues>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31b7f0b)
#28 0x00000000031ba43b (anonymous namespace)::AAPotentialValuesImpl::addValue(llvm::Attributor&, llvm::PotentialValuesState<std::pair<llvm::AA::ValueAndContext, llvm::AA::ValueScope>>&, llvm::Value&, llvm::Instruction const*, llvm::AA::ValueScope, llvm::Function*) const AttributorAttributes.cpp:0:0
#29 0x00000000031bb79d bool llvm::function_ref<bool (llvm::Instruction&)>::callback_fn<(anonymous namespace)::AAPotentialValuesReturned::updateImpl(llvm::Attributor&)::'lambda0'(llvm::Instruction&)>(long, llvm::Instruction&) AttributorAttributes.cpp:0:0
#30 0x0000000003122131 checkForAllInstructionsImpl(llvm::Attributor*, llvm::DenseMap<unsigned int, llvm::SmallVector<llvm::Instruction*, 8u>*, llvm::DenseMapInfo<unsigned int, void>, llvm::detail::DenseMapPair<unsigned int, llvm::SmallVector<llvm::Instruction*, 8u>*>>&, llvm::function_ref<bool (llvm::Instruction&)>, llvm::AbstractAttribute const*, llvm::AAIsDead const*, llvm::ArrayRef<unsigned int>, bool&, bool, bool) Attributor.cpp:0:0
#31 0x000000000312981a llvm::Attributor::checkForAllInstructions(llvm::function_ref<bool (llvm::Instruction&)>, llvm::AbstractAttribute const&, llvm::ArrayRef<unsigned int>, bool&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x312981a)
#32 0x00000000031bb267 (anonymous namespace)::AAPotentialValuesReturned::updateImpl(llvm::Attributor&) AttributorAttributes.cpp:0:0
#33 0x000000000310c916 llvm::AbstractAttribute::update(llvm::Attributor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x310c916)
#34 0x00000000031212b0 llvm::Attributor::updateAA(llvm::AbstractAttribute&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31212b0)
#35 0x0000000003129c9b llvm::AAPotentialValues const* llvm::Attributor::getOrCreateAAFor<llvm::AAPotentialValues>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3129c9b)
#36 0x0000000003129f50 llvm::Attributor::getAssumedSimplifiedValues(llvm::IRPosition const&, llvm::AbstractAttribute const*, llvm::SmallVectorImpl<llvm::AA::ValueAndContext>&, llvm::AA::ValueScope, bool&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3129f50)
#37 0x000000000312a728 llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x312a728)
#38 0x0000000003169bc6 (anonymous namespace)::AAReturnedFromReturnedValues<llvm::AANoUndef, (anonymous namespace)::AANoUndefImpl, llvm::BooleanState, false, (llvm::Attribute::AttrKind)41, true>::updateImpl(llvm::Attributor&) AttributorAttributes.cpp:0:0
#39 0x000000000310c916 llvm::AbstractAttribute::update(llvm::Attributor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x310c916)
#40 0x00000000031212b0 llvm::Attributor::updateAA(llvm::AbstractAttribute&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31212b0)
#41 0x00000000031c055b llvm::AANoUndef const* llvm::Attributor::getOrCreateAAFor<llvm::AANoUndef>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31c055b)
#42 0x00000000031c0689 bool llvm::AA::hasAssumedIRAttr<(llvm::Attribute::AttrKind)41, llvm::AbstractAttribute>(llvm::Attributor&, llvm::AbstractAttribute const*, llvm::IRPosition const&, llvm::DepClassTy, bool&, bool, llvm::AbstractAttribute const**) (.constprop.0) AttributorAttributes.cpp:0:0
#43 0x00000000031c08c7 (anonymous namespace)::AAUndefinedBehaviorImpl::updateImpl(llvm::Attributor&) AttributorAttributes.cpp:0:0
#44 0x000000000310c916 llvm::AbstractAttribute::update(llvm::Attributor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x310c916)
#45 0x00000000031212b0 llvm::Attributor::updateAA(llvm::AbstractAttribute&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31212b0)
#46 0x00000000031390e1 llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&) (.part.0) Attributor.cpp:0:0
#47 0x0000000003139711 llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3139711)
#48 0x0000000003142d9b runAttributorOnFunctions(llvm::InformationCache&, llvm::SetVector<llvm::Function*, llvm::SmallVector<llvm::Function*, 0u>, llvm::DenseSet<llvm::Function*, llvm::DenseMapInfo<llvm::Function*, void>>, 0u>&, llvm::AnalysisGetter&, llvm::CallGraphUpdater&, bool, bool) (.part.0) Attributor.cpp:0:0
#49 0x000000000314395f llvm::AttributorPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x314395f)
#50 0x0000000002fb687e llvm::detail::PassModel<llvm::Module, llvm::AttributorPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fb687e)
#51 0x00000000057656c1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57656c1)
#52 0x0000000000985b5a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985b5a)
#53 0x0000000000979dd1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979dd1)
#54 0x00007674e0429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x00007674e0429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x00000000009711e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9711e5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

