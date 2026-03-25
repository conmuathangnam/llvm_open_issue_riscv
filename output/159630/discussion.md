# [Clang] Crash on invalidly named destructor definition

**Author:** MitalAshok
**URL:** https://github.com/llvm/llvm-project/issues/159630
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, regression:21
**Closed Date:** 2025-09-21T13:56:00Z

## Body

Reproducer (with `-fsyntax-only`) <https://godbolt.org/z/j57z3EbbW>:

```c++
struct X {
    template<typename T>
    struct typo_correction {
        ~typo_correction();
    };
};

template<typename T>
X::typo_correction<T>::typo_correctiom::~typo_correctiom() {}
```

Crashes:

```
example.cpp:9:24: error: no member named 'typo_correctiom' in 'X::typo_correction<T>'; did you mean 'typo_correction'?
    9 | X::typo_correction<T>::typo_correctiom::~typo_correctiom() {}
      | ~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~
      |                        typo_correction
example.cpp:3:12: note: 'typo_correction' declared here
    3 |     struct typo_correction {
      |            ^
example.cpp:9:42: error: no type named 'typo_correctiom' in 'X::typo_correction<T>'
    9 | X::typo_correction<T>::typo_correctiom::~typo_correctiom() {}
      | ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~
clang++: /llvm/llvm-project/clang/include/clang/AST/TypeBase.h:953: const ExtQualsTypeCommonBase *clang::QualType::getCommonPtr() const: Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ./build/clang++ -fsyntax-only example.cpp
1.      example.cpp:9:57: current parser token '('
 #0 0x000055a3d3da40ed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000055a3d3da465b PrintStackTraceSignalHandler(void*) /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:917:1
 #2 0x000055a3d3da2406 llvm::sys::RunSignalHandlers() /llvm/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000055a3d3da396e llvm::sys::CleanupOnSignal(unsigned long) /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:374:1
 #4 0x000055a3d3cc4f17 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /llvm/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:0:7
 #5 0x000055a3d3cc5305 CrashRecoverySignalHandler(int) /llvm/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
 #6 0x00007d98b4a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #7 0x00007d98b4a969fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
 #8 0x00007d98b4a969fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
 #9 0x00007d98b4a969fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
#10 0x00007d98b4a42476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
#11 0x00007d98b4a287f3 abort ./stdlib/./stdlib/abort.c:81:7
#12 0x00007d98b4a2871b _nl_load_domain ./intl/./intl/loadmsgcat.c:1177:9
#13 0x00007d98b4a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#14 0x000055a3d415e347 clang::QualType::getCommonPtr() const /llvm/llvm-project/clang/include/clang/AST/TypeBase.h:0:5
#15 0x000055a3d415e2d5 clang::QualType::getTypePtr() const /llvm/llvm-project/clang/include/clang/AST/TypeBase.h:8286:26
#16 0x000055a3d415de25 clang::QualType::operator->() const /llvm/llvm-project/clang/include/clang/AST/TypeBase.h:997:5
#17 0x000055a3d8b322cd clang::LocInfoType::LocInfoType(clang::QualType, clang::TypeSourceInfo*) /llvm/llvm-project/clang/include/clang/AST/LocInfoType.h:38:38
#18 0x000055a3d8b0e8b8 clang::Sema::CreateParsedType(clang::QualType, clang::TypeSourceInfo*) /llvm/llvm-project/clang/lib/Sema/SemaType.cpp:6381:3
#19 0x000055a3d82ee62e clang::Sema::getDestructorName(clang::IdentifierInfo const&, clang::SourceLocation, clang::Scope*, clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool) /llvm/llvm-project/clang/lib/Sema/SemaExprCXX.cpp:357:12
#20 0x000055a3d7aeb4b3 clang::Parser::ParseUnqualifiedId(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool, bool, bool, clang::SourceLocation*, clang::UnqualifiedId&) /llvm/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:2839:17
#21 0x000055a3d7b3db37 clang::Parser::ParseDirectDeclarator(clang::Declarator&) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:6641:43
#22 0x000055a3d7b3c7fe clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:9
#23 0x000055a3d7b444b7 clang::Parser::ParseDeclarator(clang::Declarator&)::$_0::operator()() const /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:6288:3
#24 0x000055a3d7b44475 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseDeclarator(clang::Declarator&)::$_0>(long) /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#25 0x000055a3d2888cc9 llvm::function_ref<void ()>::operator()() const /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#26 0x000055a3d4f5db1d clang::runWithSufficientStackSpace(llvm::function_ref<void ()>, llvm::function_ref<void ()>) /llvm/llvm-project/clang/include/clang/Basic/Stack.h:55:3
#27 0x000055a3d4f5d970 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /llvm/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
#28 0x000055a3d7c524a8 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /llvm/llvm-project/clang/lib/Sema/Sema.cpp:630:1
#29 0x000055a3d7b1da76 clang::Parser::ParseDeclarator(clang::Declarator&) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:6289:1
#30 0x000055a3d7b2bc67 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:2147:7
#31 0x000055a3d7b78cbe clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /llvm/llvm-project/clang/lib/Parse/ParseTemplate.cpp:228:10
#32 0x000055a3d7b77d32 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) /llvm/llvm-project/clang/lib/Parse/ParseTemplate.cpp:147:10
#33 0x000055a3d7b77710 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) /llvm/llvm-project/clang/lib/Parse/ParseTemplate.cpp:43:10
#34 0x000055a3d7b2adbe clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:1890:12
#35 0x000055a3d7a906bf clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /llvm/llvm-project/clang/lib/Parse/Parser.cpp:946:14
#36 0x000055a3d7a8e9bd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /llvm/llvm-project/clang/lib/Parse/Parser.cpp:745:12
#37 0x000055a3d7a7acdc clang::ParseAST(clang::Sema&, bool, bool) /llvm/llvm-project/clang/lib/Parse/ParseAST.cpp:170:16
#38 0x000055a3d5484cb3 clang::ASTFrontendAction::ExecuteAction() /llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1435:1
#39 0x000055a3d5484626 clang::FrontendAction::Execute() /llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:7
#40 0x000055a3d538c1ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /llvm/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:23
#41 0x000055a3d5690f6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /llvm/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:308:8
#42 0x000055a3d1c2d30b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /llvm/llvm-project/clang/tools/driver/cc1_main.cpp:300:13
#43 0x000055a3d1c20242 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /llvm/llvm-project/clang/tools/driver/driver.cpp:223:5
#44 0x000055a3d1c20d50 clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const /llvm/llvm-project/clang/tools/driver/driver.cpp:366:9
#45 0x000055a3d1c20d1d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#46 0x000055a3d51c8c01 llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#47 0x000055a3d51c55d8 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const::$_0::operator()() const /llvm/llvm-project/clang/lib/Driver/Job.cpp:436:34
#48 0x000055a3d51c55a5 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const::$_0>(long) /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#49 0x000055a3d2888cc9 llvm::function_ref<void ()>::operator()() const /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#50 0x000055a3d3cc4d0a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /llvm/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:3
#51 0x000055a3d51c4f3b clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const /llvm/llvm-project/clang/lib/Driver/Job.cpp:436:7
#52 0x000055a3d5156ba3 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /llvm/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#53 0x000055a3d5156db4 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&, bool) const /llvm/llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#54 0x000055a3d51725e2 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) /llvm/llvm-project/clang/lib/Driver/Driver.cpp:2244:7
#55 0x000055a3d1c1fd25 clang_main(int, char**, llvm::ToolContext const&) /llvm/llvm-project/clang/tools/driver/driver.cpp:404:9
#56 0x000055a3d1c54cb5 main /llvm/llvm-project/llvm/cmake-build-debug/tools/clang/tools/driver/clang-driver.cpp:17:3
#57 0x00007d98b4a29d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#58 0x00007d98b4a29e40 call_init ./csu/../csu/libc-start.c:128:20
#59 0x00007d98b4a29e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#60 0x000055a3d1c1ea65 _start (./build/clang+++0x5e4aa65)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /llvm/llvm-project/llvm/cmake-build-debug/bin
Build config: +unoptimized, +assertions
```

`git bisect` seems to show this starting with d1a80deae674300d1011ccb6d6ee7030eaf8e713 / #135914 , this didn't crash in clang 20

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mital Ashok (MitalAshok)

<details>
Reproducer (with `-fsyntax-only`) &lt;https://godbolt.org/z/j57z3EbbW&gt;:

```c++
struct X {
    template&lt;typename T&gt;
    struct typo_correction {
        ~typo_correction();
    };
};

template&lt;typename T&gt;
X::typo_correction&lt;T&gt;::typo_correctiom::~typo_correctiom() {}
```

Crashes:

```
example.cpp:9:24: error: no member named 'typo_correctiom' in 'X::typo_correction&lt;T&gt;'; did you mean 'typo_correction'?
    9 | X::typo_correction&lt;T&gt;::typo_correctiom::~typo_correctiom() {}
      | ~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~
      |                        typo_correction
example.cpp:3:12: note: 'typo_correction' declared here
    3 |     struct typo_correction {
      |            ^
example.cpp:9:42: error: no type named 'typo_correctiom' in 'X::typo_correction&lt;T&gt;'
    9 | X::typo_correction&lt;T&gt;::typo_correctiom::~typo_correctiom() {}
      | ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~
clang++: /llvm/llvm-project/clang/include/clang/AST/TypeBase.h:953: const ExtQualsTypeCommonBase *clang::QualType::getCommonPtr() const: Assertion `!isNull() &amp;&amp; "Cannot retrieve a NULL type pointer"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ./build/clang++ -fsyntax-only example.cpp
1.      example.cpp:9:57: current parser token '('
 #<!-- -->0 0x000055a3d3da40ed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000055a3d3da465b PrintStackTraceSignalHandler(void*) /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:917:1
 #<!-- -->2 0x000055a3d3da2406 llvm::sys::RunSignalHandlers() /llvm/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000055a3d3da396e llvm::sys::CleanupOnSignal(unsigned long) /llvm/llvm-project/llvm/lib/Support/Unix/Signals.inc:374:1
 #<!-- -->4 0x000055a3d3cc4f17 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /llvm/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:0:7
 #<!-- -->5 0x000055a3d3cc5305 CrashRecoverySignalHandler(int) /llvm/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
 #<!-- -->6 0x00007d98b4a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->7 0x00007d98b4a969fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
 #<!-- -->8 0x00007d98b4a969fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
 #<!-- -->9 0x00007d98b4a969fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
#<!-- -->10 0x00007d98b4a42476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
#<!-- -->11 0x00007d98b4a287f3 abort ./stdlib/./stdlib/abort.c:81:7
#<!-- -->12 0x00007d98b4a2871b _nl_load_domain ./intl/./intl/loadmsgcat.c:1177:9
#<!-- -->13 0x00007d98b4a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->14 0x000055a3d415e347 clang::QualType::getCommonPtr() const /llvm/llvm-project/clang/include/clang/AST/TypeBase.h:0:5
#<!-- -->15 0x000055a3d415e2d5 clang::QualType::getTypePtr() const /llvm/llvm-project/clang/include/clang/AST/TypeBase.h:8286:26
#<!-- -->16 0x000055a3d415de25 clang::QualType::operator-&gt;() const /llvm/llvm-project/clang/include/clang/AST/TypeBase.h:997:5
#<!-- -->17 0x000055a3d8b322cd clang::LocInfoType::LocInfoType(clang::QualType, clang::TypeSourceInfo*) /llvm/llvm-project/clang/include/clang/AST/LocInfoType.h:38:38
#<!-- -->18 0x000055a3d8b0e8b8 clang::Sema::CreateParsedType(clang::QualType, clang::TypeSourceInfo*) /llvm/llvm-project/clang/lib/Sema/SemaType.cpp:6381:3
#<!-- -->19 0x000055a3d82ee62e clang::Sema::getDestructorName(clang::IdentifierInfo const&amp;, clang::SourceLocation, clang::Scope*, clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool) /llvm/llvm-project/clang/lib/Sema/SemaExprCXX.cpp:357:12
#<!-- -->20 0x000055a3d7aeb4b3 clang::Parser::ParseUnqualifiedId(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool, bool, bool, clang::SourceLocation*, clang::UnqualifiedId&amp;) /llvm/llvm-project/clang/lib/Parse/ParseExprCXX.cpp:2839:17
#<!-- -->21 0x000055a3d7b3db37 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:6641:43
#<!-- -->22 0x000055a3d7b3c7fe clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:9
#<!-- -->23 0x000055a3d7b444b7 clang::Parser::ParseDeclarator(clang::Declarator&amp;)::$_0::operator()() const /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:6288:3
#<!-- -->24 0x000055a3d7b44475 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Parser::ParseDeclarator(clang::Declarator&amp;)::$_0&gt;(long) /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->25 0x000055a3d2888cc9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->26 0x000055a3d4f5db1d clang::runWithSufficientStackSpace(llvm::function_ref&lt;void ()&gt;, llvm::function_ref&lt;void ()&gt;) /llvm/llvm-project/clang/include/clang/Basic/Stack.h:55:3
#<!-- -->27 0x000055a3d4f5d970 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /llvm/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
#<!-- -->28 0x000055a3d7c524a8 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /llvm/llvm-project/clang/lib/Sema/Sema.cpp:630:1
#<!-- -->29 0x000055a3d7b1da76 clang::Parser::ParseDeclarator(clang::Declarator&amp;) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:6289:1
#<!-- -->30 0x000055a3d7b2bc67 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:2147:7
#<!-- -->31 0x000055a3d7b78cbe clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) /llvm/llvm-project/clang/lib/Parse/ParseTemplate.cpp:228:10
#<!-- -->32 0x000055a3d7b77d32 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) /llvm/llvm-project/clang/lib/Parse/ParseTemplate.cpp:147:10
#<!-- -->33 0x000055a3d7b77710 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) /llvm/llvm-project/clang/lib/Parse/ParseTemplate.cpp:43:10
#<!-- -->34 0x000055a3d7b2adbe clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /llvm/llvm-project/clang/lib/Parse/ParseDecl.cpp:1890:12
#<!-- -->35 0x000055a3d7a906bf clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /llvm/llvm-project/clang/lib/Parse/Parser.cpp:946:14
#<!-- -->36 0x000055a3d7a8e9bd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /llvm/llvm-project/clang/lib/Parse/Parser.cpp:745:12
#<!-- -->37 0x000055a3d7a7acdc clang::ParseAST(clang::Sema&amp;, bool, bool) /llvm/llvm-project/clang/lib/Parse/ParseAST.cpp:170:16
#<!-- -->38 0x000055a3d5484cb3 clang::ASTFrontendAction::ExecuteAction() /llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1435:1
#<!-- -->39 0x000055a3d5484626 clang::FrontendAction::Execute() /llvm/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:7
#<!-- -->40 0x000055a3d538c1ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /llvm/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:23
#<!-- -->41 0x000055a3d5690f6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /llvm/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:308:8
#<!-- -->42 0x000055a3d1c2d30b cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /llvm/llvm-project/clang/tools/driver/cc1_main.cpp:300:13
#<!-- -->43 0x000055a3d1c20242 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /llvm/llvm-project/clang/tools/driver/driver.cpp:223:5
#<!-- -->44 0x000055a3d1c20d50 clang_main(int, char**, llvm::ToolContext const&amp;)::$_0::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /llvm/llvm-project/clang/tools/driver/driver.cpp:366:9
#<!-- -->45 0x000055a3d1c20d1d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->46 0x000055a3d51c8c01 llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->47 0x000055a3d51c55d8 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;*, bool*) const::$_0::operator()() const /llvm/llvm-project/clang/lib/Driver/Job.cpp:436:34
#<!-- -->48 0x000055a3d51c55a5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;*, bool*) const::$_0&gt;(long) /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->49 0x000055a3d2888cc9 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->50 0x000055a3d3cc4d0a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /llvm/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:3
#<!-- -->51 0x000055a3d51c4f3b clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;*, bool*) const /llvm/llvm-project/clang/lib/Driver/Job.cpp:436:7
#<!-- -->52 0x000055a3d5156ba3 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /llvm/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
#<!-- -->53 0x000055a3d5156db4 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;, bool) const /llvm/llvm-project/clang/lib/Driver/Compilation.cpp:246:13
#<!-- -->54 0x000055a3d51725e2 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;) /llvm/llvm-project/clang/lib/Driver/Driver.cpp:2244:7
#<!-- -->55 0x000055a3d1c1fd25 clang_main(int, char**, llvm::ToolContext const&amp;) /llvm/llvm-project/clang/tools/driver/driver.cpp:404:9
#<!-- -->56 0x000055a3d1c54cb5 main /llvm/llvm-project/llvm/cmake-build-debug/tools/clang/tools/driver/clang-driver.cpp:17:3
#<!-- -->57 0x00007d98b4a29d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->58 0x00007d98b4a29e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->59 0x00007d98b4a29e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->60 0x000055a3d1c1ea65 _start (./build/clang+++0x5e4aa65)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /llvm/llvm-project/llvm/cmake-build-debug/bin
Build config: +unoptimized, +assertions
```

`git bisect` seems to show this starting with d1a80deae674300d1011ccb6d6ee7030eaf8e713 / #<!-- -->135914 , this didn't crash in clang 20
</details>


---

### Comment 2 - shafik

clang-21 regression, works in clang-20: https://godbolt.org/z/r5avoe7rq

---

### Comment 3 - shafik

@zyn0217 this bisects to: d1a80deae674300d1011ccb6d6ee7030eaf8e713

---

