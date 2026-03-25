# [Attributor] Assertion `(Offset == RangeTy::Unassigned) == (Size == RangeTy::Unassigned) && "Inconsistent state!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/165703
**Status:** Open
**Labels:** crash, llvm::Attributor

## Body

Reproducer:
https://godbolt.org/z/vbfnd94Ef

Backtrace:
```console
opt: /root/llvm-project/llvm/include/llvm/Transforms/IPO/Attributor.h:262: bool llvm::AA::RangeTy::isUnassigned() const: Assertion `(Offset == RangeTy::Unassigned) == (Size == RangeTy::Unassigned) && "Inconsistent state!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=attributor <source>
1.	Running pass "attributor" on module "<source>"
 #0 0x000000000598f068 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x598f068)
 #1 0x000000000598bf14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000078acbec42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000078acbec969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000078acbec42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000078acbec287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000078acbec2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000078acbec39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003145d03 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3145d03)
 #9 0x000000000316729d llvm::AAPointerInfo::Access::operator&=(llvm::AAPointerInfo::Access const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x316729d)
#10 0x000000000318411e llvm::AA::PointerInfo::State::addAccess(llvm::Attributor&, llvm::AAPointerInfo::RangeList const&, llvm::Instruction&, std::optional<llvm::Value*>, llvm::AAPointerInfo::AccessKind, llvm::Type*, llvm::Instruction*) (.constprop.0) AttributorAttributes.cpp:0:0
#11 0x0000000003184bf3 (anonymous namespace)::AAPointerInfoFloating::handleAccess(llvm::Attributor&, llvm::Instruction&, std::optional<llvm::Value*>, llvm::AAPointerInfo::AccessKind, llvm::SmallSet<long, 4u, std::less<long>>&, llvm::ChangeStatus&, llvm::Type&) AttributorAttributes.cpp:0:0
#12 0x000000000318517e (anonymous namespace)::AAPointerInfoFloating::updateImpl(llvm::Attributor&)::'lambda0'(llvm::Use const&, bool&)::operator()(llvm::Use const&, bool&) const::'lambda2'(llvm::Instruction&, llvm::Value*, llvm::Type&, llvm::ArrayRef<llvm::Value*>, llvm::AAPointerInfo::AccessKind)::operator()(llvm::Instruction&, llvm::Value*, llvm::Type&, llvm::ArrayRef<llvm::Value*>, llvm::AAPointerInfo::AccessKind) const AttributorAttributes.cpp:0:0
#13 0x00000000031c3209 (anonymous namespace)::AAPointerInfoFloating::updateImpl(llvm::Attributor&)::'lambda0'(llvm::Use const&, bool&)::operator()(llvm::Use const&, bool&) const AttributorAttributes.cpp:0:0
#14 0x000000000313f4e7 llvm::Attributor::checkForAllUses(llvm::function_ref<bool (llvm::Use const&, bool&)>, llvm::AbstractAttribute const&, llvm::Value const&, bool, llvm::DepClassTy, bool, llvm::function_ref<bool (llvm::Use const&, llvm::Use const&)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x313f4e7)
#15 0x000000000317fb6f (anonymous namespace)::AAPointerInfoFloating::updateImpl(llvm::Attributor&) AttributorAttributes.cpp:0:0
#16 0x000000000310c916 llvm::AbstractAttribute::update(llvm::Attributor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x310c916)
#17 0x0000000003121483 llvm::Attributor::updateAA(llvm::AbstractAttribute&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3121483)
#18 0x00000000011be203 llvm::AAPointerInfo const* llvm::Attributor::getOrCreateAAFor<llvm::AAPointerInfo>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x11be203)
#19 0x000000000312d010 bool llvm::function_ref<bool (llvm::Value&)>::callback_fn<bool getPotentialCopiesOfMemoryValue<false, llvm::StoreInst>(llvm::Attributor&, llvm::StoreInst&, llvm::SmallSetVector<llvm::Value*, 4u>&, llvm::SmallSetVector<llvm::Instruction*, 4u>*, llvm::AbstractAttribute const&, bool&, bool)::'lambda'(llvm::Value&)>(long, llvm::Value&) Attributor.cpp:0:0
#20 0x000000000315d481 (anonymous namespace)::AAUnderlyingObjectsImpl::forallUnderlyingObjects(llvm::function_ref<bool (llvm::Value&)>, llvm::AA::ValueScope) const AttributorAttributes.cpp:0:0
#21 0x000000000313e6da bool getPotentialCopiesOfMemoryValue<false, llvm::StoreInst>(llvm::Attributor&, llvm::StoreInst&, llvm::SmallSetVector<llvm::Value*, 4u>&, llvm::SmallSetVector<llvm::Instruction*, 4u>*, llvm::AbstractAttribute const&, bool&, bool) (.constprop.0) Attributor.cpp:0:0
#22 0x0000000003192d76 (anonymous namespace)::AAIsDeadFloating::isDeadStore(llvm::Attributor&, llvm::StoreInst&, llvm::SmallSetVector<llvm::Instruction*, 8u>*) AttributorAttributes.cpp:0:0
#23 0x00000000031a051a (anonymous namespace)::AAIsDeadFloating::updateImpl(llvm::Attributor&) AttributorAttributes.cpp:0:0
#24 0x000000000310c916 llvm::AbstractAttribute::update(llvm::Attributor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x310c916)
#25 0x00000000031212b0 llvm::Attributor::updateAA(llvm::AbstractAttribute&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31212b0)
#26 0x0000000003121a8b llvm::AAIsDead const* llvm::Attributor::getOrCreateAAFor<llvm::AAIsDead>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3121a8b)
#27 0x0000000003121ed3 llvm::Attributor::isAssumedDead(llvm::IRPosition const&, llvm::AbstractAttribute const*, llvm::AAIsDead const*, bool&, bool, llvm::DepClassTy) (.part.0) Attributor.cpp:0:0
#28 0x00000000031296b1 llvm::Attributor::checkForAllReadWriteInstructions(llvm::function_ref<bool (llvm::Instruction&)>, llvm::AbstractAttribute&, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31296b1)
#29 0x0000000003146dda (anonymous namespace)::AANoSyncImpl::updateImpl(llvm::Attributor&) AttributorAttributes.cpp:0:0
#30 0x000000000310c916 llvm::AbstractAttribute::update(llvm::Attributor&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x310c916)
#31 0x00000000031212b0 llvm::Attributor::updateAA(llvm::AbstractAttribute&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x31212b0)
#32 0x000000000312e7bb llvm::AANoSync const* llvm::Attributor::getOrCreateAAFor<llvm::AANoSync>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x312e7bb)
#33 0x0000000003138732 llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&) (.part.0) Attributor.cpp:0:0
#34 0x0000000003139711 llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x3139711)
#35 0x0000000003142d9b runAttributorOnFunctions(llvm::InformationCache&, llvm::SetVector<llvm::Function*, llvm::SmallVector<llvm::Function*, 0u>, llvm::DenseSet<llvm::Function*, llvm::DenseMapInfo<llvm::Function*, void>>, 0u>&, llvm::AnalysisGetter&, llvm::CallGraphUpdater&, bool, bool) (.part.0) Attributor.cpp:0:0
#36 0x000000000314395f llvm::AttributorPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x314395f)
#37 0x0000000002fb687e llvm::detail::PassModel<llvm::Module, llvm::AttributorPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x2fb687e)
#38 0x00000000057656c1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x57656c1)
#39 0x0000000000985b5a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x985b5a)
#40 0x0000000000979dd1 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x979dd1)
#41 0x000078acbec29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x000078acbec29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x00000000009711e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x9711e5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

