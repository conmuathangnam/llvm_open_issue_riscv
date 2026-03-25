# clang-22: `Sema::getCurLambda(bool)`: Assertion `!CodeSynthesisContexts.empty()' failed.

**Author:** vogelsgesang
**URL:** https://github.com/llvm/llvm-project/issues/176045
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash-on-valid, regression:22
**Closed Date:** 2026-01-20T01:56:06Z

## Body

Try compiling the following source code with `-std=c++20` (https://godbolt.org/z/oqoPa5bWa)

```
template <int NumArgs> struct MessageFormat {
  template <int N> consteval MessageFormat(const char (&)[N]) {}
};
template <typename... Ts> void format(MessageFormat<sizeof...(Ts)>, Ts ...args);

auto message = [] {
   format("");
   format("");
};
```

clang-21 accepts the code (https://godbolt.org/z/edjj5rEz5).
clang-trunk (as of b8d5ef9d2e197234da197986b606b6753e363f24) crashes.
I assume that clang-22 also has the same crash given that the 22 branch was cut around the same commit.

Stack trace:

```
clang: /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/Sema.cpp:2574: clang::sema::LambdaScopeInfo* clang::Sema::getCurLambda(bool): Assertion `!CodeSynthesisContexts.empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -std=c++23 -c minimal_repro_preproc.cpp
1.      minimal_repro_preproc.cpp:9:2: current parser token ';'
2.      minimal_repro_preproc.cpp:6:16: lambda expression parsing
 #0 0x00006307b0e44f62 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Unix/Signals.inc:846:3
 #1 0x00006307b0e41ebf llvm::sys::RunSignalHandlers() /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #2 0x00006307b0d9d860 CrashRecoverySignalHandler(int) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #3 0x00007619ff045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007619ff09eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007619ff09eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007619ff09eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007619ff04527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007619ff0288ff abort ./stdlib/abort.c:81:7
 #9 0x00007619ff02881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007619ff03b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00006307b32bc5f9 (/home/avogelsgesang/Documents/llvm-project/build/bin/clang-22+0x456a5f9)
#12 0x00006307b37137bb clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaExprCXX.cpp:7738:21
#13 0x00006307b39b0bb2 BuildConvertedConstantExpression(clang::Sema&, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&) (.constprop.0) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaOverload.cpp:6443:33
#14 0x00006307b39b130f clang::Sema::BuildConvertedConstantExpression(clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/AST/APValue.h:442:5
#15 0x00006307b3a848e1 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&, clang::TemplateArgument&, bool, clang::Sema::CheckTemplateArgumentKind) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaTemplate.cpp:7309:51
#16 0x00006307b3a92af1 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&, clang::Sema::CheckTemplateArgumentKind) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaTemplate.cpp:5569:45
#17 0x00006307b3a977ce clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::TemplateParameterList*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaTemplate.cpp:5974:9
#18 0x00006307b3aa6b1a clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::Scope*, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaTemplate.cpp:3782:3
#19 0x00006307b3695c6c clang::TreeTransform<RemoveNestedImmediateInvocation(clang::Sema&, clang::Sema::ExpressionEvaluationContextRecord&, std::reverse_iterator<llvm::PointerIntPair<clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>, llvm::PointerIntPairInfo<clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>>>*>)::ComplexRemove>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/AST/TypeBase.h:1005:28
#20 0x00006307b368a33b clang::TreeTransform<RemoveNestedImmediateInvocation(clang::Sema&, clang::Sema::ExpressionEvaluationContextRecord&, std::reverse_iterator<llvm::PointerIntPair<clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>, llvm::PointerIntPairInfo<clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>>>*>)::ComplexRemove>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) /home/avogelsgesang/Documents/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:79:1
#21 0x00006307b368b7d2 clang::TreeTransform<RemoveNestedImmediateInvocation(clang::Sema&, clang::Sema::ExpressionEvaluationContextRecord&, std::reverse_iterator<llvm::PointerIntPair<clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>, llvm::PointerIntPairInfo<clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>>>*>)::ComplexRemove>::TransformType(clang::TypeSourceInfo*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/TreeTransform.h:5427:47
#22 0x00006307b368b978 clang::TreeTransform<RemoveNestedImmediateInvocation(clang::Sema&, clang::Sema::ExpressionEvaluationContextRecord&, std::reverse_iterator<llvm::PointerIntPair<clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>, llvm::PointerIntPairInfo<clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>>>*>)::ComplexRemove>::TransformType(clang::QualType) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/TreeTransform.h:5408:3
#23 0x00006307b368c88e clang::TreeTransform<RemoveNestedImmediateInvocation(clang::Sema&, clang::Sema::ExpressionEvaluationContextRecord&, std::reverse_iterator<llvm::PointerIntPair<clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>, llvm::PointerIntPairInfo<clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits<clang::ConstantExpr*>>>*>)::ComplexRemove>::TransformCXXConstructExpr(clang::CXXConstructExpr*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/TreeTransform.h:15452:42
#24 0x00006307b36497a5 clang::Sema::PopExpressionEvaluationContext() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Sema/Ownership.h:202:47
#25 0x00006307b37b163d clang::Sema::BuildLambdaExpr(clang::SourceLocation, clang::SourceLocation) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaLambda.cpp:2167:43
#26 0x00006307b31f63c6 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:1490:35
#27 0x00006307b31f7133 clang::Parser::ParseLambdaExpression() /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:693:46
#28 0x00006307b31db895 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExpr.cpp:1562:34
#29 0x00006307b31dc4e9 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExpr.cpp:570:3
#30 0x00006307b31dc588 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExpr.cpp:93:1
#31 0x00006307b3200dc4 clang::Parser::ParseInitializer(clang::Decl*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseInit.cpp:606:1
#32 0x00006307b3198169 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:2620:32
#33 0x00006307b31a97a2 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:2357:3
#34 0x00006307b316a369 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:1181:73
#35 0x00006307b316abdf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:1203:42
#36 0x00006307b31715d1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:1031:50
#37 0x00006307b3173227 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:744:36
#38 0x00006307b31568ea clang::ParseAST(clang::Sema&, bool, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseAST.cpp:169:69
#39 0x00006307b19cc487 clang::FrontendAction::Execute() /home/avogelsgesang/Documents/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1316:38
#40 0x00006307b195fece clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#41 0x00006307b1aae535 std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/13/bits/shared_ptr_base.h:1666:16
#42 0x00006307b1aae535 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/13/bits/shared_ptr_base.h:1363:69
#43 0x00006307b1aae535 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/13/bits/shared_ptr_base.h:1349:2
#44 0x00006307b1aae535 clang::CompilerInvocation::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#45 0x00006307b1aae535 clang::CompilerInstance::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:301:39
#46 0x00006307b1aae535 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/avogelsgesang/Documents/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:311:29
#47 0x00006307afa0f580 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/cc1_main.cpp:304:40
#48 0x00006307afa06a29 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:225:20
#49 0x00006307afa06c15 std::__atomic_base<int>::fetch_sub(int, std::memory_order) /usr/include/c++/13/bits/atomic_base.h:645:34
#50 0x00006307afa06c15 llvm::ThreadSafeRefCountedBase<llvm::vfs::FileSystem>::Release() const /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:134:41
#51 0x00006307afa06c15 llvm::IntrusiveRefCntPtrInfo<llvm::vfs::FileSystem>::release(llvm::vfs::FileSystem*) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:164:45
#52 0x00006307afa06c15 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::release() /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:233:41
#53 0x00006307afa06c15 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::~IntrusiveRefCntPtr() /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#54 0x00006307afa06c15 operator() /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:369:26
#55 0x00006307afa06c15 int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:52
#56 0x00006307b1777d8d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) /home/avogelsgesang/Documents/llvm-project/clang/lib/Driver/Job.cpp:442:32
#57 0x00006307b0d9d9ee llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:10
#58 0x00006307b17787bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) /home/avogelsgesang/Documents/llvm-project/clang/lib/Driver/Job.cpp:446:10
#59 0x00006307b173af97 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /home/avogelsgesang/Documents/llvm-project/clang/lib/Driver/Compilation.cpp:197:3
#60 0x00006307b173b386 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /home/avogelsgesang/Documents/llvm-project/clang/lib/Driver/Compilation.cpp:246:5
#61 0x00006307b1746774 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:83:46
#62 0x00006307afa0a136 llvm::SmallVectorBase<unsigned int>::size() const /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:80:32
#63 0x00006307afa0a136 llvm::SmallVectorTemplateCommon<std::pair<int, clang::driver::Command const*>, void>::end() /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:273:41
#64 0x00006307afa0a136 clang_main(int, char**, llvm::ToolContext const&) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:409:26
#65 0x00006307af90717a main /home/avogelsgesang/Documents/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:18:1
#66 0x00007619ff02a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#67 0x00007619ff02a28b call_init ./csu/../csu/libc-start.c:128:20
#68 0x00007619ff02a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#69 0x00006307afa05d85 _start (/home/avogelsgesang/Documents/llvm-project/build/bin/clang-22+0xcb3d85)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/avogelsgesang/Documents/llvm-project/build/bin
Build config: +assertions
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Adrian Vogelsgesang (vogelsgesang)

<details>
Try compiling the following source code with `-std=c++20` (https://godbolt.org/z/oqoPa5bWa)

```
template &lt;int NumArgs&gt; struct MessageFormat {
  template &lt;int N&gt; consteval MessageFormat(const char (&amp;)[N]) {}
};
template &lt;typename... Ts&gt; void format(MessageFormat&lt;sizeof...(Ts)&gt;, Ts ...args);

auto message = [] {
   format("");
   format("");
};
```

clang-21 accepts the code (https://godbolt.org/z/edjj5rEz5).
clang-trunk (as of b8d5ef9d2e197234da197986b606b6753e363f24) crashes.
I assume that clang-22 also has the same crash given that the 22 branch was cut around the same commit.

Stack trace:

```
clang: /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/Sema.cpp:2574: clang::sema::LambdaScopeInfo* clang::Sema::getCurLambda(bool): Assertion `!CodeSynthesisContexts.empty()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -std=c++23 -c minimal_repro_preproc.cpp
1.      minimal_repro_preproc.cpp:9:2: current parser token ';'
2.      minimal_repro_preproc.cpp:6:16: lambda expression parsing
 #<!-- -->0 0x00006307b0e44f62 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Unix/Signals.inc:846:3
 #<!-- -->1 0x00006307b0e41ebf llvm::sys::RunSignalHandlers() /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #<!-- -->2 0x00006307b0d9d860 CrashRecoverySignalHandler(int) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->3 0x00007619ff045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007619ff09eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007619ff09eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007619ff09eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007619ff04527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007619ff0288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007619ff02881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007619ff03b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00006307b32bc5f9 (/home/avogelsgesang/Documents/llvm-project/build/bin/clang-22+0x456a5f9)
#<!-- -->12 0x00006307b37137bb clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaExprCXX.cpp:7738:21
#<!-- -->13 0x00006307b39b0bb2 BuildConvertedConstantExpression(clang::Sema&amp;, clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*, clang::APValue&amp;) (.constprop.0) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaOverload.cpp:6443:33
#<!-- -->14 0x00006307b39b130f clang::Sema::BuildConvertedConstantExpression(clang::Expr*, clang::QualType, clang::CCEKind, clang::NamedDecl*) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/AST/APValue.h:442:5
#<!-- -->15 0x00006307b3a848e1 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&amp;, clang::TemplateArgument&amp;, bool, clang::Sema::CheckTemplateArgumentKind) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaTemplate.cpp:7309:51
#<!-- -->16 0x00006307b3a92af1 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&amp;, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&amp;, clang::Sema::CheckTemplateArgumentKind) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaTemplate.cpp:5569:45
#<!-- -->17 0x00006307b3a977ce clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::TemplateParameterList*, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::DefaultArguments const&amp;, bool, clang::Sema::CheckTemplateArgumentInfo&amp;, bool, bool*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaTemplate.cpp:5974:9
#<!-- -->18 0x00006307b3aa6b1a clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::Scope*, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaTemplate.cpp:3782:3
#<!-- -->19 0x00006307b3695c6c clang::TreeTransform&lt;RemoveNestedImmediateInvocation(clang::Sema&amp;, clang::Sema::ExpressionEvaluationContextRecord&amp;, std::reverse_iterator&lt;llvm::PointerIntPair&lt;clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;, llvm::PointerIntPairInfo&lt;clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;&gt;&gt;*&gt;)::ComplexRemove&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) /home/avogelsgesang/Documents/llvm-project/clang/include/clang/AST/TypeBase.h:1005:28
#<!-- -->20 0x00006307b368a33b clang::TreeTransform&lt;RemoveNestedImmediateInvocation(clang::Sema&amp;, clang::Sema::ExpressionEvaluationContextRecord&amp;, std::reverse_iterator&lt;llvm::PointerIntPair&lt;clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;, llvm::PointerIntPairInfo&lt;clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;&gt;&gt;*&gt;)::ComplexRemove&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) /home/avogelsgesang/Documents/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:79:1
#<!-- -->21 0x00006307b368b7d2 clang::TreeTransform&lt;RemoveNestedImmediateInvocation(clang::Sema&amp;, clang::Sema::ExpressionEvaluationContextRecord&amp;, std::reverse_iterator&lt;llvm::PointerIntPair&lt;clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;, llvm::PointerIntPairInfo&lt;clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;&gt;&gt;*&gt;)::ComplexRemove&gt;::TransformType(clang::TypeSourceInfo*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/TreeTransform.h:5427:47
#<!-- -->22 0x00006307b368b978 clang::TreeTransform&lt;RemoveNestedImmediateInvocation(clang::Sema&amp;, clang::Sema::ExpressionEvaluationContextRecord&amp;, std::reverse_iterator&lt;llvm::PointerIntPair&lt;clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;, llvm::PointerIntPairInfo&lt;clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;&gt;&gt;*&gt;)::ComplexRemove&gt;::TransformType(clang::QualType) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/TreeTransform.h:5408:3
#<!-- -->23 0x00006307b368c88e clang::TreeTransform&lt;RemoveNestedImmediateInvocation(clang::Sema&amp;, clang::Sema::ExpressionEvaluationContextRecord&amp;, std::reverse_iterator&lt;llvm::PointerIntPair&lt;clang::ConstantExpr*, 1u, unsigned int, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;, llvm::PointerIntPairInfo&lt;clang::ConstantExpr*, 1u, llvm::PointerLikeTypeTraits&lt;clang::ConstantExpr*&gt;&gt;&gt;*&gt;)::ComplexRemove&gt;::TransformCXXConstructExpr(clang::CXXConstructExpr*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/TreeTransform.h:15452:42
#<!-- -->24 0x00006307b36497a5 clang::Sema::PopExpressionEvaluationContext() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Sema/Ownership.h:202:47
#<!-- -->25 0x00006307b37b163d clang::Sema::BuildLambdaExpr(clang::SourceLocation, clang::SourceLocation) /home/avogelsgesang/Documents/llvm-project/clang/lib/Sema/SemaLambda.cpp:2167:43
#<!-- -->26 0x00006307b31f63c6 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:1490:35
#<!-- -->27 0x00006307b31f7133 clang::Parser::ParseLambdaExpression() /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:693:46
#<!-- -->28 0x00006307b31db895 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExpr.cpp:1562:34
#<!-- -->29 0x00006307b31dc4e9 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExpr.cpp:570:3
#<!-- -->30 0x00006307b31dc588 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseExpr.cpp:93:1
#<!-- -->31 0x00006307b3200dc4 clang::Parser::ParseInitializer(clang::Decl*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseInit.cpp:606:1
#<!-- -->32 0x00006307b3198169 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:2620:32
#<!-- -->33 0x00006307b31a97a2 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseDecl.cpp:2357:3
#<!-- -->34 0x00006307b316a369 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:1181:73
#<!-- -->35 0x00006307b316abdf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:1203:42
#<!-- -->36 0x00006307b31715d1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:1031:50
#<!-- -->37 0x00006307b3173227 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/Parser.cpp:744:36
#<!-- -->38 0x00006307b31568ea clang::ParseAST(clang::Sema&amp;, bool, bool) /home/avogelsgesang/Documents/llvm-project/clang/lib/Parse/ParseAST.cpp:169:69
#<!-- -->39 0x00006307b19cc487 clang::FrontendAction::Execute() /home/avogelsgesang/Documents/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1316:38
#<!-- -->40 0x00006307b195fece clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#<!-- -->41 0x00006307b1aae535 std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/13/bits/shared_ptr_base.h:1666:16
#<!-- -->42 0x00006307b1aae535 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/13/bits/shared_ptr_base.h:1363:69
#<!-- -->43 0x00006307b1aae535 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/13/bits/shared_ptr_base.h:1349:2
#<!-- -->44 0x00006307b1aae535 clang::CompilerInvocation::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#<!-- -->45 0x00006307b1aae535 clang::CompilerInstance::getFrontendOpts() /home/avogelsgesang/Documents/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:301:39
#<!-- -->46 0x00006307b1aae535 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/avogelsgesang/Documents/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:311:29
#<!-- -->47 0x00006307afa0f580 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/cc1_main.cpp:304:40
#<!-- -->48 0x00006307afa06a29 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:225:20
#<!-- -->49 0x00006307afa06c15 std::__atomic_base&lt;int&gt;::fetch_sub(int, std::memory_order) /usr/include/c++/13/bits/atomic_base.h:645:34
#<!-- -->50 0x00006307afa06c15 llvm::ThreadSafeRefCountedBase&lt;llvm::vfs::FileSystem&gt;::Release() const /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:134:41
#<!-- -->51 0x00006307afa06c15 llvm::IntrusiveRefCntPtrInfo&lt;llvm::vfs::FileSystem&gt;::release(llvm::vfs::FileSystem*) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:164:45
#<!-- -->52 0x00006307afa06c15 llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::release() /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:233:41
#<!-- -->53 0x00006307afa06c15 llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::~IntrusiveRefCntPtr() /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#<!-- -->54 0x00006307afa06c15 operator() /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:369:26
#<!-- -->55 0x00006307afa06c15 int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:52
#<!-- -->56 0x00006307b1777d8d void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) /home/avogelsgesang/Documents/llvm-project/clang/lib/Driver/Job.cpp:442:32
#<!-- -->57 0x00006307b0d9d9ee llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /home/avogelsgesang/Documents/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:10
#<!-- -->58 0x00006307b17787bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) /home/avogelsgesang/Documents/llvm-project/clang/lib/Driver/Job.cpp:446:10
#<!-- -->59 0x00006307b173af97 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /home/avogelsgesang/Documents/llvm-project/clang/lib/Driver/Compilation.cpp:197:3
#<!-- -->60 0x00006307b173b386 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /home/avogelsgesang/Documents/llvm-project/clang/lib/Driver/Compilation.cpp:246:5
#<!-- -->61 0x00006307b1746774 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:83:46
#<!-- -->62 0x00006307afa0a136 llvm::SmallVectorBase&lt;unsigned int&gt;::size() const /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:80:32
#<!-- -->63 0x00006307afa0a136 llvm::SmallVectorTemplateCommon&lt;std::pair&lt;int, clang::driver::Command const*&gt;, void&gt;::end() /home/avogelsgesang/Documents/llvm-project/llvm/include/llvm/ADT/SmallVector.h:273:41
#<!-- -->64 0x00006307afa0a136 clang_main(int, char**, llvm::ToolContext const&amp;) /home/avogelsgesang/Documents/llvm-project/clang/tools/driver/driver.cpp:409:26
#<!-- -->65 0x00006307af90717a main /home/avogelsgesang/Documents/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:18:1
#<!-- -->66 0x00007619ff02a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->67 0x00007619ff02a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->68 0x00007619ff02a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->69 0x00006307afa05d85 _start (/home/avogelsgesang/Documents/llvm-project/build/bin/clang-22+0xcb3d85)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/avogelsgesang/Documents/llvm-project/build/bin
Build config: +assertions
```
</details>


---

### Comment 2 - shafik

@EugeneZelenko there is a "Regressions" we use for frontend bugs, please add to that when you label a regression, thank you!

---

### Comment 3 - EugeneZelenko

> [@EugeneZelenko](https://github.com/EugeneZelenko) there is a "Regressions" we use for frontend bugs, please add to that when you label a regression, thank you!

Sure, I'll try to not forget to do so!

---

### Comment 4 - shafik

@zyn0217 this bisects to 5f9630b388527d7a9d268001dcb7b2f0707e060a

---

