# [clang] Crash compiling Jank master

**Author:** tsliwkan
**URL:** https://github.com/llvm/llvm-project/issues/167362

## Body

Crash while compiling jank (https://github.com/jank-lang/jank) latest 

```[2944/4906] Building CXX object tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o
FAILED: [code=1] tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o 
/usr/bin/clang++ -DCLANG_EXPORTS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/tools/clang/lib/Sema -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/lib/Sema -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/tools/clang/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/llvm/include -fPIC -fexceptions -frtti -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -fno-common -Woverloaded-virtual -Wno-nested-anon-types -O3 -DNDEBUG -std=c++17 -MD -MT tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o -MF tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o.d -o tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o -c /home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/lib/Sema/SemaExprMember.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -DCLANG_EXPORTS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/tools/clang/lib/Sema -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/lib/Sema -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/tools/clang/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/llvm/include -fPIC -fexceptions -frtti -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -fno-common -Woverloaded-virtual -Wno-nested-anon-types -O3 -DNDEBUG -std=c++17 -MD -MT tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o -MF tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o.d -o tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o -c /home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/lib/Sema/SemaExprMember.cpp
1.	<eof> parser at end of file
2.	/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/llvm/include/llvm/ADT/TinyPtrVector.h:135:8: instantiating function definition 'llvm::TinyPtrVector<clang::LambdaCapture *>::empty'
 #0 0x00007f5bf790832d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Unix/Signals.inc:834:22
 #1 0x00007f5bf7905e07 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Signals.cpp:104:20
 #2 0x00007f5bf7905e07 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Unix/Signals.inc:373:31
 #3 0x00007f5bf77da7f9 HandleCrash /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #4 0x00007f5bf77da7f9 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #5 0x00007f5bf683e540 (/usr/lib/libc.so.6+0x3e540)
 #6 0x00007f5c013ca8ac clang::Stmt::getStmtClass() const /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/Stmt.h:1484:44
 #7 0x00007f5c013ca8ac clang::ImplicitCastExpr::classof(clang::Stmt const*) /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/Expr.h:3833:27
 #8 0x00007f5c013ca8ac llvm::isa_impl<clang::ImplicitCastExpr, clang::Expr, void>::doit(clang::Expr const&) /usr/include/llvm/Support/Casting.h:64:64
 #9 0x00007f5c013ca8ac llvm::isa_impl_cl<clang::ImplicitCastExpr, clang::Expr const*>::doit(clang::Expr const*) /usr/include/llvm/Support/Casting.h:110:36
#10 0x00007f5c013ca8ac llvm::isa_impl_wrap<clang::ImplicitCastExpr, clang::Expr const*, clang::Expr const*>::doit(clang::Expr const* const&) /usr/include/llvm/Support/Casting.h:137:41
#11 0x00007f5c013ca8ac llvm::isa_impl_wrap<clang::ImplicitCastExpr, clang::Expr* const, clang::Expr const*>::doit(clang::Expr* const&) /usr/include/llvm/Support/Casting.h:129:13
#12 0x00007f5c013ca8ac llvm::CastIsPossible<clang::ImplicitCastExpr, clang::Expr*, void>::isPossible(clang::Expr* const&) /usr/include/llvm/Support/Casting.h:257:62
#13 0x00007f5c013ca8ac llvm::CastInfo<clang::ImplicitCastExpr, clang::Expr*, void>::doCastIfPossible(clang::Expr* const&) /usr/include/llvm/Support/Casting.h:493:26
#14 0x00007f5c013ca8ac decltype(auto) llvm::dyn_cast<clang::ImplicitCastExpr, clang::Expr>(clang::Expr*) /usr/include/llvm/Support/Casting.h:663:48
#15 0x00007f5c013ca8ac clang::IgnoreImplicitCastsSingleStep(clang::Expr*) /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/IgnoreExpr.h:49:45
#16 0x00007f5c013ca8ac clang::Expr* clang::detail::IgnoreExprNodesImpl<clang::Expr* (&)(clang::Expr*)>(clang::Expr*, clang::Expr* (&)(clang::Expr*)) /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/IgnoreExpr.h:26:29
#17 0x00007f5c013ca8ac clang::Expr* clang::IgnoreExprNodes<clang::Expr* (&)(clang::Expr*)>(clang::Expr*, clang::Expr* (&)(clang::Expr*)) /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/IgnoreExpr.h:38:36
#18 0x00007f5c013ca8ac clang::Expr::IgnoreImpCasts() /usr/src/debug/clang/clang-21.1.5.src/lib/AST/Expr.cpp:3071:25
#19 0x00007f5c013ca8ac clang::Expr::getReferencedDeclOfCallee() /usr/src/debug/clang/clang-21.1.5.src/lib/AST/Expr.cpp:1550:29
#20 0x00007f5c013f0241 auto llvm::dyn_cast_if_present<clang::FunctionDecl, clang::Decl const>(clang::Decl const*) /usr/include/llvm/Support/Casting.h:751:3
#21 0x00007f5c013f0241 auto llvm::dyn_cast_or_null<clang::FunctionDecl, clang::Decl const>(clang::Decl const*) /usr/include/llvm/Support/Casting.h:768:32
#22 0x00007f5c013f0241 clang::CallExpr::getDirectCallee() const /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/Expr.h:3062:42
#23 0x00007f5c013f0241 clang::CallExpr::getBuiltinCallee() const /usr/src/debug/clang/clang-21.1.5.src/lib/AST/Expr.cpp:1592:38
#24 0x00007f5c0143cc93 IsConstantEvaluatedBuiltinCall /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:8061:12
#25 0x00007f5c0143cc93 VisitCallExpr /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:12933:38
#26 0x00007f5c0144ede9 Evaluate /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16641:46
#27 0x00007f5c0144f7ee EvaluateAsRValue /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16763:3
#28 0x00007f5c01450673 EvaluateAsRValue /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16846:26
#29 0x00007f5c01450673 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16895:28
#30 0x00007f5c014507eb clang::Expr::EvaluateAsBooleanCondition(bool&, clang::ASTContext const&, bool) const (.constprop.0) /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16904:26
#31 0x00007f5c029fc5cb TryResult /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:408:27
#32 0x00007f5c029fc5cb evaluateAsBooleanConditionNoCache /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:1436:14
#33 0x00007f5c02a240e2 VisitIfStmt /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:3292:56
#34 0x00007f5c02a23b1d VisitCompoundStmt /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:2939:9
#35 0x00007f5c02a28092 addStmt /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:790:17
#36 0x00007f5c02a28092 buildCFG /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:1674:24
#37 0x00007f5c02a28092 clang::CFG::buildCFG(clang::Decl const*, clang::Stmt*, clang::ASTContext*, clang::CFG::BuildOptions const&) /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:5348:39
#38 0x00007f5c029f34be std::__uniq_ptr_impl<clang::CFG, std::default_delete<clang::CFG>>::release() /usr/include/c++/15.2.1/bits/unique_ptr.h:211:10
#39 0x00007f5c029f34be std::__uniq_ptr_impl<clang::CFG, std::default_delete<clang::CFG>>::operator=(std::__uniq_ptr_impl<clang::CFG, std::default_delete<clang::CFG>>&&) /usr/include/c++/15.2.1/bits/unique_ptr.h:185:7
#40 0x00007f5c029f34be std::__uniq_ptr_data<clang::CFG, std::default_delete<clang::CFG>, true, true>::operator=(std::__uniq_ptr_data<clang::CFG, std::default_delete<clang::CFG>, true, true>&&) /usr/include/c++/15.2.1/bits/unique_ptr.h:237:24
#41 0x00007f5c029f34be std::unique_ptr<clang::CFG, std::default_delete<clang::CFG>>::operator=(std::unique_ptr<clang::CFG, std::default_delete<clang::CFG>>&&) /usr/include/c++/15.2.1/bits/unique_ptr.h:409:19
#42 0x00007f5c029f34be clang::AnalysisDeclContext::getCFG() /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/AnalysisDeclContext.cpp:216:75
#43 0x00007f5c01afb7f1 CheckFallThrough /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/AnalysisBasedWarnings.cpp:569:23
#44 0x00007f5c01b06afd CheckFallThroughForBody /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/AnalysisBasedWarnings.cpp:830:49
#45 0x00007f5c01b06afd clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/AnalysisBasedWarnings.cpp:2954:28
#46 0x00007f5c01b93978 clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl const*, clang::QualType) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/Sema.cpp:2457:35
#47 0x00007f5c01d82e39 std::unique_ptr<clang::sema::FunctionScopeInfo, clang::Sema::PoppedFunctionScopeDeleter>::~unique_ptr() /usr/include/c++/15.2.1/bits/unique_ptr.h:398:12
#48 0x00007f5c01d82e39 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/SemaDecl.cpp:16687:23
#49 0x00007f5c023ed127 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:5856:32
#50 0x00007f5c023fa352 clang::FunctionDecl::isDefined() const /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/Decl.h:2257:21
#51 0x00007f5c023fa352 clang::Sema::PerformPendingInstantiations(bool, bool) (.constprop.0) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:7105:32
#52 0x00007f5c01b9b1a4 llvm::TimeTraceScope::~TimeTraceScope() /usr/include/llvm/Support/TimeProfiler.h:200:5
#53 0x00007f5c01b9b1a4 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/Sema.cpp:1223:3
#54 0x00007f5c01b9b501 clang::Sema::ActOnEndOfTranslationUnit() /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/Sema.cpp:1252:9
#55 0x00007f5c01083ad7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/src/debug/clang/clang-21.1.5.src/lib/Parse/Parser.cpp:714:12
#56 0x00007f5c00f71589 clang::ParseAST(clang::Sema&, bool, bool) /usr/src/debug/clang/clang-21.1.5.src/lib/Parse/ParseAST.cpp:169:69
#57 0x00007f5c0304e8dc clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.5.src/lib/Frontend/CompilerInstance.cpp:86:58
#58 0x00007f5c0304e8dc clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.5.src/lib/Frontend/FrontendAction.cpp:1225:38
#59 0x00007f5c02fafd4f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
#60 0x00007f5c02fafd4f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
#61 0x00007f5c02fafd4f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/debug/clang/clang-21.1.5.src/lib/Frontend/CompilerInstance.cpp:1055:42
#62 0x00007f5c030abb2c std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1673:16
#63 0x00007f5c030abb2c std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1370:69
#64 0x00007f5c030abb2c std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1358:40
#65 0x00007f5c030abb2c clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-21.1.5.src/include/clang/Frontend/CompilerInvocation.h:259:48
#66 0x00007f5c030abb2c clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-21.1.5.src/include/clang/Frontend/CompilerInstance.h:304:39
#67 0x00007f5c030abb2c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-21.1.5.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:300:29
#68 0x000055be2c00ce9a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/src/debug/clang/clang-21.1.5.src/tools/driver/cc1_main.cpp:297:40
#69 0x000055be2c010b61 ExecuteCC1Tool /usr/src/debug/clang/clang-21.1.5.src/tools/driver/driver.cpp:223:20
#70 0x00007f5c02bf30b5 operator() /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Job.cpp:436:32
#71 0x00007f5c02bf30b5 callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::string*, bool*) const::<lambda()> > /usr/include/llvm/ADT/STLFunctionalExtras.h:46:52
#72 0x00007f5bf77dac38 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/CrashRecoveryContext.cpp:428:1
#73 0x00007f5c02bfec75 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Job.cpp:440:10
#74 0x00007f5c02c120cb clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Compilation.cpp:196:22
#75 0x00007f5c02c124cc clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Compilation.cpp:251:5
#76 0x00007f5c02c24ca5 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:82:46
#77 0x00007f5c02c24ca5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Driver.cpp:2246:28
#78 0x000055be2c012b85 llvm::SmallVectorBase<unsigned int>::size() const /usr/include/llvm/ADT/SmallVector.h:79:32
#79 0x000055be2c012b85 llvm::SmallVectorTemplateCommon<std::pair<int, clang::driver::Command const*>, void>::end() /usr/include/llvm/ADT/SmallVector.h:270:41
#80 0x000055be2c012b85 clang_main(int, char**, llvm::ToolContext const&) /usr/src/debug/clang/clang-21.1.5.src/tools/driver/driver.cpp:408:26
#81 0x000055be2c0042b0 main /usr/src/debug/clang/clang-21.1.5.src/build/tools/driver/clang-driver.cpp:18:1
#82 0x00007f5bf6827675 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#83 0x00007f5bf6827729 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#84 0x00007f5bf6827729 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#85 0x000055be2c004315 _start (/usr/bin/clang+++0xb315)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.1.5
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/SemaExprMember-5b97cc.cpp
clang++: note: diagnostic msg: /tmp/SemaExprMember-5b97cc.sh
clang++: note: diagnostic msg: 

********************
[2975/4906] Building CXX object tools/clang/lib/ASTMatchers/Dynamic/CMakeFiles/obj.clangDynamicASTMatchers.dir/Registry.cpp.o
ninja: build stopped: subcommand failed.
==> ERROR: A failure occurred in build().
    Aborting...
```

The run script:
[SemaExprMember-5b97cc.sh.txt](https://github.com/user-attachments/files/23459975/SemaExprMember-5b97cc.sh.txt)

Github is repeatedly failing to upload the pre-processed source: maybe the file size is too large (35MB)? I've made it public in google drive at https://drive.google.com/file/d/1dZZVt5UwgDVKculnx0O0RZKbRBe2dyxu/view?usp=drive_link. If you would prefer some other method to get this file please let me know.




## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (tsliwkan)

<details>
Crash while compiling jank (https://github.com/jank-lang/jank) latest 

```[2944/4906] Building CXX object tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o
FAILED: [code=1] tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o 
/usr/bin/clang++ -DCLANG_EXPORTS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/tools/clang/lib/Sema -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/lib/Sema -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/tools/clang/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/llvm/include -fPIC -fexceptions -frtti -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -fno-common -Woverloaded-virtual -Wno-nested-anon-types -O3 -DNDEBUG -std=c++17 -MD -MT tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o -MF tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o.d -o tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o -c /home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/lib/Sema/SemaExprMember.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -DCLANG_EXPORTS -D_GLIBCXX_USE_CXX11_ABI=1 -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/tools/clang/lib/Sema -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/lib/Sema -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/tools/clang/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm-build/include -I/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/llvm/include -fPIC -fexceptions -frtti -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -fno-common -Woverloaded-virtual -Wno-nested-anon-types -O3 -DNDEBUG -std=c++17 -MD -MT tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o -MF tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o.d -o tools/clang/lib/Sema/CMakeFiles/obj.clangSema.dir/SemaExprMember.cpp.o -c /home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/clang/lib/Sema/SemaExprMember.cpp
1.	&lt;eof&gt; parser at end of file
2.	/home/tsliwkan/opt/jank-git/src/jank/compiler+runtime/build/llvm/llvm/include/llvm/ADT/TinyPtrVector.h:135:8: instantiating function definition 'llvm::TinyPtrVector&lt;clang::LambdaCapture *&gt;::empty'
 #<!-- -->0 0x00007f5bf790832d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Unix/Signals.inc:834:22
 #<!-- -->1 0x00007f5bf7905e07 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Signals.cpp:104:20
 #<!-- -->2 0x00007f5bf7905e07 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/Unix/Signals.inc:373:31
 #<!-- -->3 0x00007f5bf77da7f9 HandleCrash /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->4 0x00007f5bf77da7f9 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #<!-- -->5 0x00007f5bf683e540 (/usr/lib/libc.so.6+0x3e540)
 #<!-- -->6 0x00007f5c013ca8ac clang::Stmt::getStmtClass() const /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/Stmt.h:1484:44
 #<!-- -->7 0x00007f5c013ca8ac clang::ImplicitCastExpr::classof(clang::Stmt const*) /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/Expr.h:3833:27
 #<!-- -->8 0x00007f5c013ca8ac llvm::isa_impl&lt;clang::ImplicitCastExpr, clang::Expr, void&gt;::doit(clang::Expr const&amp;) /usr/include/llvm/Support/Casting.h:64:64
 #<!-- -->9 0x00007f5c013ca8ac llvm::isa_impl_cl&lt;clang::ImplicitCastExpr, clang::Expr const*&gt;::doit(clang::Expr const*) /usr/include/llvm/Support/Casting.h:110:36
#<!-- -->10 0x00007f5c013ca8ac llvm::isa_impl_wrap&lt;clang::ImplicitCastExpr, clang::Expr const*, clang::Expr const*&gt;::doit(clang::Expr const* const&amp;) /usr/include/llvm/Support/Casting.h:137:41
#<!-- -->11 0x00007f5c013ca8ac llvm::isa_impl_wrap&lt;clang::ImplicitCastExpr, clang::Expr* const, clang::Expr const*&gt;::doit(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:129:13
#<!-- -->12 0x00007f5c013ca8ac llvm::CastIsPossible&lt;clang::ImplicitCastExpr, clang::Expr*, void&gt;::isPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:257:62
#<!-- -->13 0x00007f5c013ca8ac llvm::CastInfo&lt;clang::ImplicitCastExpr, clang::Expr*, void&gt;::doCastIfPossible(clang::Expr* const&amp;) /usr/include/llvm/Support/Casting.h:493:26
#<!-- -->14 0x00007f5c013ca8ac decltype(auto) llvm::dyn_cast&lt;clang::ImplicitCastExpr, clang::Expr&gt;(clang::Expr*) /usr/include/llvm/Support/Casting.h:663:48
#<!-- -->15 0x00007f5c013ca8ac clang::IgnoreImplicitCastsSingleStep(clang::Expr*) /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/IgnoreExpr.h:49:45
#<!-- -->16 0x00007f5c013ca8ac clang::Expr* clang::detail::IgnoreExprNodesImpl&lt;clang::Expr* (&amp;)(clang::Expr*)&gt;(clang::Expr*, clang::Expr* (&amp;)(clang::Expr*)) /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/IgnoreExpr.h:26:29
#<!-- -->17 0x00007f5c013ca8ac clang::Expr* clang::IgnoreExprNodes&lt;clang::Expr* (&amp;)(clang::Expr*)&gt;(clang::Expr*, clang::Expr* (&amp;)(clang::Expr*)) /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/IgnoreExpr.h:38:36
#<!-- -->18 0x00007f5c013ca8ac clang::Expr::IgnoreImpCasts() /usr/src/debug/clang/clang-21.1.5.src/lib/AST/Expr.cpp:3071:25
#<!-- -->19 0x00007f5c013ca8ac clang::Expr::getReferencedDeclOfCallee() /usr/src/debug/clang/clang-21.1.5.src/lib/AST/Expr.cpp:1550:29
#<!-- -->20 0x00007f5c013f0241 auto llvm::dyn_cast_if_present&lt;clang::FunctionDecl, clang::Decl const&gt;(clang::Decl const*) /usr/include/llvm/Support/Casting.h:751:3
#<!-- -->21 0x00007f5c013f0241 auto llvm::dyn_cast_or_null&lt;clang::FunctionDecl, clang::Decl const&gt;(clang::Decl const*) /usr/include/llvm/Support/Casting.h:768:32
#<!-- -->22 0x00007f5c013f0241 clang::CallExpr::getDirectCallee() const /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/Expr.h:3062:42
#<!-- -->23 0x00007f5c013f0241 clang::CallExpr::getBuiltinCallee() const /usr/src/debug/clang/clang-21.1.5.src/lib/AST/Expr.cpp:1592:38
#<!-- -->24 0x00007f5c0143cc93 IsConstantEvaluatedBuiltinCall /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:8061:12
#<!-- -->25 0x00007f5c0143cc93 VisitCallExpr /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:12933:38
#<!-- -->26 0x00007f5c0144ede9 Evaluate /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16641:46
#<!-- -->27 0x00007f5c0144f7ee EvaluateAsRValue /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16763:3
#<!-- -->28 0x00007f5c01450673 EvaluateAsRValue /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16846:26
#<!-- -->29 0x00007f5c01450673 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16895:28
#<!-- -->30 0x00007f5c014507eb clang::Expr::EvaluateAsBooleanCondition(bool&amp;, clang::ASTContext const&amp;, bool) const (.constprop.0) /usr/src/debug/clang/clang-21.1.5.src/lib/AST/ExprConstant.cpp:16904:26
#<!-- -->31 0x00007f5c029fc5cb TryResult /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:408:27
#<!-- -->32 0x00007f5c029fc5cb evaluateAsBooleanConditionNoCache /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:1436:14
#<!-- -->33 0x00007f5c02a240e2 VisitIfStmt /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:3292:56
#<!-- -->34 0x00007f5c02a23b1d VisitCompoundStmt /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:2939:9
#<!-- -->35 0x00007f5c02a28092 addStmt /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:790:17
#<!-- -->36 0x00007f5c02a28092 buildCFG /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:1674:24
#<!-- -->37 0x00007f5c02a28092 clang::CFG::buildCFG(clang::Decl const*, clang::Stmt*, clang::ASTContext*, clang::CFG::BuildOptions const&amp;) /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/CFG.cpp:5348:39
#<!-- -->38 0x00007f5c029f34be std::__uniq_ptr_impl&lt;clang::CFG, std::default_delete&lt;clang::CFG&gt;&gt;::release() /usr/include/c++/15.2.1/bits/unique_ptr.h:211:10
#<!-- -->39 0x00007f5c029f34be std::__uniq_ptr_impl&lt;clang::CFG, std::default_delete&lt;clang::CFG&gt;&gt;::operator=(std::__uniq_ptr_impl&lt;clang::CFG, std::default_delete&lt;clang::CFG&gt;&gt;&amp;&amp;) /usr/include/c++/15.2.1/bits/unique_ptr.h:185:7
#<!-- -->40 0x00007f5c029f34be std::__uniq_ptr_data&lt;clang::CFG, std::default_delete&lt;clang::CFG&gt;, true, true&gt;::operator=(std::__uniq_ptr_data&lt;clang::CFG, std::default_delete&lt;clang::CFG&gt;, true, true&gt;&amp;&amp;) /usr/include/c++/15.2.1/bits/unique_ptr.h:237:24
#<!-- -->41 0x00007f5c029f34be std::unique_ptr&lt;clang::CFG, std::default_delete&lt;clang::CFG&gt;&gt;::operator=(std::unique_ptr&lt;clang::CFG, std::default_delete&lt;clang::CFG&gt;&gt;&amp;&amp;) /usr/include/c++/15.2.1/bits/unique_ptr.h:409:19
#<!-- -->42 0x00007f5c029f34be clang::AnalysisDeclContext::getCFG() /usr/src/debug/clang/clang-21.1.5.src/lib/Analysis/AnalysisDeclContext.cpp:216:75
#<!-- -->43 0x00007f5c01afb7f1 CheckFallThrough /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/AnalysisBasedWarnings.cpp:569:23
#<!-- -->44 0x00007f5c01b06afd CheckFallThroughForBody /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/AnalysisBasedWarnings.cpp:830:49
#<!-- -->45 0x00007f5c01b06afd clang::sema::AnalysisBasedWarnings::IssueWarnings(clang::sema::AnalysisBasedWarnings::Policy, clang::sema::FunctionScopeInfo*, clang::Decl const*, clang::QualType) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/AnalysisBasedWarnings.cpp:2954:28
#<!-- -->46 0x00007f5c01b93978 clang::Sema::PopFunctionScopeInfo(clang::sema::AnalysisBasedWarnings::Policy const*, clang::Decl const*, clang::QualType) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/Sema.cpp:2457:35
#<!-- -->47 0x00007f5c01d82e39 std::unique_ptr&lt;clang::sema::FunctionScopeInfo, clang::Sema::PoppedFunctionScopeDeleter&gt;::~unique_ptr() /usr/include/c++/15.2.1/bits/unique_ptr.h:398:12
#<!-- -->48 0x00007f5c01d82e39 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/SemaDecl.cpp:16687:23
#<!-- -->49 0x00007f5c023ed127 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:5856:32
#<!-- -->50 0x00007f5c023fa352 clang::FunctionDecl::isDefined() const /usr/src/debug/clang/clang-21.1.5.src/include/clang/AST/Decl.h:2257:21
#<!-- -->51 0x00007f5c023fa352 clang::Sema::PerformPendingInstantiations(bool, bool) (.constprop.0) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:7105:32
#<!-- -->52 0x00007f5c01b9b1a4 llvm::TimeTraceScope::~TimeTraceScope() /usr/include/llvm/Support/TimeProfiler.h:200:5
#<!-- -->53 0x00007f5c01b9b1a4 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/Sema.cpp:1223:3
#<!-- -->54 0x00007f5c01b9b501 clang::Sema::ActOnEndOfTranslationUnit() /usr/src/debug/clang/clang-21.1.5.src/lib/Sema/Sema.cpp:1252:9
#<!-- -->55 0x00007f5c01083ad7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/debug/clang/clang-21.1.5.src/lib/Parse/Parser.cpp:714:12
#<!-- -->56 0x00007f5c00f71589 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-21.1.5.src/lib/Parse/ParseAST.cpp:169:69
#<!-- -->57 0x00007f5c0304e8dc clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.5.src/lib/Frontend/CompilerInstance.cpp:86:58
#<!-- -->58 0x00007f5c0304e8dc clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.5.src/lib/Frontend/FrontendAction.cpp:1225:38
#<!-- -->59 0x00007f5c02fafd4f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
#<!-- -->60 0x00007f5c02fafd4f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
#<!-- -->61 0x00007f5c02fafd4f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-21.1.5.src/lib/Frontend/CompilerInstance.cpp:1055:42
#<!-- -->62 0x00007f5c030abb2c std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1673:16
#<!-- -->63 0x00007f5c030abb2c std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1370:69
#<!-- -->64 0x00007f5c030abb2c std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1358:40
#<!-- -->65 0x00007f5c030abb2c clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-21.1.5.src/include/clang/Frontend/CompilerInvocation.h:259:48
#<!-- -->66 0x00007f5c030abb2c clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-21.1.5.src/include/clang/Frontend/CompilerInstance.h:304:39
#<!-- -->67 0x00007f5c030abb2c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-21.1.5.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:300:29
#<!-- -->68 0x000055be2c00ce9a cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang/clang-21.1.5.src/tools/driver/cc1_main.cpp:297:40
#<!-- -->69 0x000055be2c010b61 ExecuteCC1Tool /usr/src/debug/clang/clang-21.1.5.src/tools/driver/driver.cpp:223:20
#<!-- -->70 0x00007f5c02bf30b5 operator() /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Job.cpp:436:32
#<!-- -->71 0x00007f5c02bf30b5 callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::string*, bool*) const::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:46:52
#<!-- -->72 0x00007f5bf77dac38 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /usr/src/debug/llvm/llvm-21.1.5.src/lib/Support/CrashRecoveryContext.cpp:428:1
#<!-- -->73 0x00007f5c02bfec75 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Job.cpp:440:10
#<!-- -->74 0x00007f5c02c120cb clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Compilation.cpp:196:22
#<!-- -->75 0x00007f5c02c124cc clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Compilation.cpp:251:5
#<!-- -->76 0x00007f5c02c24ca5 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /usr/include/llvm/ADT/SmallVector.h:82:46
#<!-- -->77 0x00007f5c02c24ca5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /usr/src/debug/clang/clang-21.1.5.src/lib/Driver/Driver.cpp:2246:28
#<!-- -->78 0x000055be2c012b85 llvm::SmallVectorBase&lt;unsigned int&gt;::size() const /usr/include/llvm/ADT/SmallVector.h:79:32
#<!-- -->79 0x000055be2c012b85 llvm::SmallVectorTemplateCommon&lt;std::pair&lt;int, clang::driver::Command const*&gt;, void&gt;::end() /usr/include/llvm/ADT/SmallVector.h:270:41
#<!-- -->80 0x000055be2c012b85 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang/clang-21.1.5.src/tools/driver/driver.cpp:408:26
#<!-- -->81 0x000055be2c0042b0 main /usr/src/debug/clang/clang-21.1.5.src/build/tools/driver/clang-driver.cpp:18:1
#<!-- -->82 0x00007f5bf6827675 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->83 0x00007f5bf6827729 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->84 0x00007f5bf6827729 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->85 0x000055be2c004315 _start (/usr/bin/clang+++0xb315)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.1.5
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/SemaExprMember-5b97cc.cpp
clang++: note: diagnostic msg: /tmp/SemaExprMember-5b97cc.sh
clang++: note: diagnostic msg: 

********************
[2975/4906] Building CXX object tools/clang/lib/ASTMatchers/Dynamic/CMakeFiles/obj.clangDynamicASTMatchers.dir/Registry.cpp.o
ninja: build stopped: subcommand failed.
==&gt; ERROR: A failure occurred in build().
    Aborting...
```

The run script:
[SemaExprMember-5b97cc.sh.txt](https://github.com/user-attachments/files/23459975/SemaExprMember-5b97cc.sh.txt)

Github is repeatedly failing to upload the pre-processed source: maybe the file size is too large (35MB)? I've made it public in google drive at https://drive.google.com/file/d/1dZZVt5UwgDVKculnx0O0RZKbRBe2dyxu/view?usp=drive_link. If you would prefer some other method to get this file please let me know.



</details>


---

### Comment 2 - Endilll

Re-uploading preprocessed source: [SemaExprMember-5b97cc.zip](https://github.com/user-attachments/files/23463896/SemaExprMember-5b97cc.zip)

---

