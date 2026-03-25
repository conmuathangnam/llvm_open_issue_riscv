# [Attributor] Assertion failure with closed world assumption

**Author:** thomasfaingnaert
**URL:** https://github.com/llvm/llvm-project/issues/165683
**Status:** Open
**Labels:** crash, llvm::Attributor

## Body

Given the following `llvm-reduce`'ed LLVM IR input file:

```llvm
; ModuleID = '<stdin>'
source_filename = "<stdin>"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@global = constant { [8 x ptr] } { [8 x ptr] [ptr null, ptr null, ptr null, ptr null, ptr null, ptr @zot, ptr null, ptr null] }

define ptr @ham(ptr %arg) {
bb:
  %load = load ptr, ptr %arg, align 8
  %call = call ptr %load(ptr null, i64 0)
  ret ptr %call
}

declare void @zot()
```

and running the Attributor with `opt -S -passes=attributor -attributor-assume-closed-world input.ll`, I get the following assertion failure and backtrace:

```
opt: /llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = PointerType; From = Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: opt -S -passes=attributor -attributor-assume-closed-world reduced-renamed.ll
1.	Running pass "attributor" on module "reduced-renamed.ll"
 #0 0x000060458ba1ab96 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:834:22
 #1 0x000060458ba1b009 PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x000060458ba18181 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:104:20
 #3 0x000060458ba1a3d3 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:426:14
 #4 0x00007a2e2a867330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007a2e2a8c0b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007a2e2a8c0b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007a2e2a8c0b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007a2e2a86727e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007a2e2a84a8ff abort ./stdlib/abort.c:81:7
#10 0x00007a2e2a84a81b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007a2e2a85d517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00006045871e8b19 decltype(auto) llvm::cast<llvm::PointerType, llvm::Type>(llvm::Type*) /llvm/include/llvm/Support/Casting.h:573:38
#13 0x00006045871e8b4c llvm::Type::getPointerAddressSpace() const /llvm/include/llvm/IR/DerivedTypes.h:774:61
#14 0x0000604587dae5d3 llvm::AANonNull::isImpliedByIR(llvm::Attributor&, llvm::IRPosition const&, llvm::Attribute::AttrKind, bool) /llvm/lib/Transforms/IPO/AttributorAttributes.cpp:2463:28
#15 0x0000604587d68a2f bool llvm::AA::hasAssumedIRAttr<(llvm::Attribute::AttrKind)44, llvm::AbstractAttribute>(llvm::Attributor&, llvm::AbstractAttribute const*, llvm::IRPosition const&, llvm::DepClassTy, bool&, bool, llvm::AbstractAttribute const**) /llvm/include/llvm/Transforms/IPO/Attributor.h:6649:5
#16 0x0000604587e04773 (anonymous namespace)::AACalleeToCallSite<llvm::AANonNull, (anonymous namespace)::AANonNullImpl, llvm::BooleanState, false, (llvm::Attribute::AttrKind)44>::updateImpl(llvm::Attributor&)::'lambda'(llvm::ArrayRef<llvm::Function const*>)::operator()(llvm::ArrayRef<llvm::Function const*>) const /llvm/lib/Transforms/IPO/AttributorAttributes.cpp:596:53
#17 0x0000604587e0c175 bool llvm::function_ref<bool (llvm::ArrayRef<llvm::Function const*>)>::callback_fn<(anonymous namespace)::AACalleeToCallSite<llvm::AANonNull, (anonymous namespace)::AANonNullImpl, llvm::BooleanState, false, (llvm::Attribute::AttrKind)44>::updateImpl(llvm::Attributor&)::'lambda'(llvm::ArrayRef<llvm::Function const*>)>(long, llvm::ArrayRef<llvm::Function const*>) /llvm/include/llvm/ADT/STLFunctionalExtras.h:48:3
#18 0x0000604587d540dc llvm::function_ref<bool (llvm::ArrayRef<llvm::Function const*>)>::operator()(llvm::ArrayRef<llvm::Function const*>) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:70:3
#19 0x0000604587d31e13 llvm::Attributor::checkForAllCallees(llvm::function_ref<bool (llvm::ArrayRef<llvm::Function const*>)>, llvm::AbstractAttribute const&, llvm::CallBase const&) /llvm/lib/Transforms/IPO/Attributor.cpp:1761:36
#20 0x0000604587e049b6 (anonymous namespace)::AACalleeToCallSite<llvm::AANonNull, (anonymous namespace)::AANonNullImpl, llvm::BooleanState, false, (llvm::Attribute::AttrKind)44>::updateImpl(llvm::Attributor&) /llvm/lib/Transforms/IPO/AttributorAttributes.cpp:612:30
#21 0x0000604587d2eb50 llvm::AbstractAttribute::update(llvm::Attributor&) /llvm/lib/Transforms/IPO/Attributor.cpp:1068:26
#22 0x0000604587d37c09 llvm::Attributor::updateAA(llvm::AbstractAttribute&) /llvm/lib/Transforms/IPO/Attributor.cpp:2696:19
#23 0x0000604587d71f82 llvm::AANonNull const* llvm::Attributor::getOrCreateAAFor<llvm::AANonNull>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) /llvm/include/llvm/Transforms/IPO/Attributor.h:1649:13
#24 0x0000604587d5d54c llvm::AANonNull const* llvm::Attributor::getAAFor<llvm::AANonNull>(llvm::AbstractAttribute const&, llvm::IRPosition const&, llvm::DepClassTy) /llvm/include/llvm/Transforms/IPO/Attributor.h:1579:36
#25 0x0000604587d68a6b bool llvm::AA::hasAssumedIRAttr<(llvm::Attribute::AttrKind)44, llvm::AbstractAttribute>(llvm::Attributor&, llvm::AbstractAttribute const*, llvm::IRPosition const&, llvm::DepClassTy, bool&, bool, llvm::AbstractAttribute const**) /llvm/include/llvm/Transforms/IPO/Attributor.h:6649:5
#26 0x0000604587e0b670 void clampReturnedValueStates<llvm::AANonNull, llvm::BooleanState, (llvm::Attribute::AttrKind)44, false>(llvm::Attributor&, llvm::AANonNull const&, llvm::BooleanState&, llvm::CallBase const*)::'lambda'(llvm::Value&)::operator()(llvm::Value&) const /llvm/lib/Transforms/IPO/AttributorAttributes.cpp:382:51
#27 0x0000604587e0c7c3 bool llvm::function_ref<bool (llvm::Value&)>::callback_fn<void clampReturnedValueStates<llvm::AANonNull, llvm::BooleanState, (llvm::Attribute::AttrKind)44, false>(llvm::Attributor&, llvm::AANonNull const&, llvm::BooleanState&, llvm::CallBase const*)::'lambda'(llvm::Value&)>(long, llvm::Value&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:48:3
#28 0x0000604587d548cf llvm::function_ref<bool (llvm::Value&)>::operator()(llvm::Value&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:70:3
#29 0x0000604587d337e3 llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)::operator()(llvm::AA::ValueAndContext const&) const /llvm/lib/Transforms/IPO/Attributor.cpp:2019:3
#30 0x0000604587d44f5b bool __gnu_cxx::__ops::_Iter_negate<llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)>::operator()<llvm::AA::ValueAndContext*>(llvm::AA::ValueAndContext*) /usr/include/c++/13/bits/predefined_ops.h:395:31
#31 0x0000604587d44746 llvm::AA::ValueAndContext* std::__find_if<llvm::AA::ValueAndContext*, __gnu_cxx::__ops::_Iter_negate<llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)>>(llvm::AA::ValueAndContext*, llvm::AA::ValueAndContext*, __gnu_cxx::__ops::_Iter_negate<llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)>, std::random_access_iterator_tag) /usr/include/c++/13/bits/stl_algobase.h:2102:4
#32 0x0000604587d43f7f llvm::AA::ValueAndContext* std::__find_if_not<llvm::AA::ValueAndContext*, __gnu_cxx::__ops::_Iter_pred<llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)>>(llvm::AA::ValueAndContext*, llvm::AA::ValueAndContext*, __gnu_cxx::__ops::_Iter_pred<llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)>) /usr/include/c++/13/bits/stl_algo.h:115:5
#33 0x0000604587d43a89 llvm::AA::ValueAndContext* std::find_if_not<llvm::AA::ValueAndContext*, llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)>(llvm::AA::ValueAndContext*, llvm::AA::ValueAndContext*, llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)) /usr/include/c++/13/bits/stl_algo.h:521:5
#34 0x0000604587d43176 bool std::all_of<llvm::AA::ValueAndContext*, llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)>(llvm::AA::ValueAndContext*, llvm::AA::ValueAndContext*, llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)) /usr/include/c++/13/bits/stl_algo.h:459:64
#35 0x0000604587d42248 bool llvm::all_of<llvm::SmallVector<llvm::AA::ValueAndContext, 3u>&, llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)>(llvm::SmallVector<llvm::AA::ValueAndContext, 3u>&, llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool)::'lambda'(llvm::AA::ValueAndContext const&)) /llvm/include/llvm/ADT/STLExtras.h:1727:1
#36 0x0000604587d3390e llvm::Attributor::checkForAllReturnedValues(llvm::function_ref<bool (llvm::Value&)>, llvm::AbstractAttribute const&, llvm::AA::ValueScope, bool) /llvm/lib/Transforms/IPO/Attributor.cpp:2017:22
#37 0x0000604587e0baa0 void clampReturnedValueStates<llvm::AANonNull, llvm::BooleanState, (llvm::Attribute::AttrKind)44, false>(llvm::Attributor&, llvm::AANonNull const&, llvm::BooleanState&, llvm::CallBase const*) /llvm/lib/Transforms/IPO/AttributorAttributes.cpp:401:35
#38 0x0000604587e04b21 (anonymous namespace)::AAReturnedFromReturnedValues<llvm::AANonNull, llvm::AANonNull, llvm::BooleanState, false, (llvm::Attribute::AttrKind)44, false>::updateImpl(llvm::Attributor&) /llvm/lib/Transforms/IPO/AttributorAttributes.cpp:429:65
#39 0x0000604587d2eb50 llvm::AbstractAttribute::update(llvm::Attributor&) /llvm/lib/Transforms/IPO/Attributor.cpp:1068:26
#40 0x0000604587d37c09 llvm::Attributor::updateAA(llvm::AbstractAttribute&) /llvm/lib/Transforms/IPO/Attributor.cpp:2696:19
#41 0x0000604587d71f82 llvm::AANonNull const* llvm::Attributor::getOrCreateAAFor<llvm::AANonNull>(llvm::IRPosition, llvm::AbstractAttribute const*, llvm::DepClassTy, bool, bool) /llvm/include/llvm/Transforms/IPO/Attributor.h:1649:13
#42 0x0000604587d68b16 llvm::AANonNull const* llvm::Attributor::getOrCreateAAFor<llvm::AANonNull>(llvm::IRPosition const&) /llvm/include/llvm/Transforms/IPO/Attributor.h:1660:36
#43 0x0000604587d587b3 void llvm::Attributor::checkAndQueryIRAttr<(llvm::Attribute::AttrKind)44, llvm::AANonNull>(llvm::IRPosition const&, llvm::AttributeSet, bool) /llvm/lib/Transforms/IPO/Attributor.cpp:3340:1
#44 0x0000604587d3c35b llvm::Attributor::identifyDefaultAbstractAttributes(llvm::Function&) /llvm/lib/Transforms/IPO/Attributor.cpp:3450:59
#45 0x0000604587d3deb6 runAttributorOnFunctions(llvm::InformationCache&, llvm::SetVector<llvm::Function*, llvm::SmallVector<llvm::Function*, 0u>, llvm::DenseSet<llvm::Function*, llvm::DenseMapInfo<llvm::Function*, void>>, 0u>&, llvm::AnalysisGetter&, llvm::CallGraphUpdater&, bool, bool) /llvm/lib/Transforms/IPO/Attributor.cpp:3898:40
#46 0x0000604587d3ee28 llvm::AttributorPass::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /llvm/lib/Transforms/IPO/Attributor.cpp:4031:3
#47 0x0000604587b9bbd9 llvm::detail::PassModel<llvm::Module, llvm::AttributorPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /llvm/include/llvm/IR/PassManagerInternal.h:92:3
#48 0x000060458b70daad llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) /llvm/include/llvm/IR/PassManagerImpl.h:80:18
#49 0x00006045871326eb llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) /llvm/tools/opt/NewPMDriver.cpp:561:10
#50 0x0000604587103def optMain /llvm/tools/opt/optdriver.cpp:745:27
#51 0x0000604587101241 main /llvm/tools/opt/opt.cpp:27:64
#52 0x00007a2e2a84c1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#53 0x00007a2e2a84c28b call_init ./csu/../csu/libc-start.c:128:20
#54 0x00007a2e2a84c28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#55 0x0000604587101125 _start (/install/bin/opt+0x81d125)
```

Without the closed world assumption, the assertion does not occur.

Tested on LLVM 287ca7b243facc7185bbd9dfdaa5a6a012819e1b on a Debug+Assertions build.


