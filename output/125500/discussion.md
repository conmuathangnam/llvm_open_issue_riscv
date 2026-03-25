# Crash during code completion in Sema::getMoreSpecializedTemplate: Assertion `TA2.getKind() == TemplateArgument::Pack` failed

**Author:** HenryAWE
**URL:** https://github.com/llvm/llvm-project/issues/125500
**Status:** Closed
**Labels:** clang:frontend, crash, regression:16
**Closed Date:** 2025-03-03T14:41:51Z

## Body

I'm using clangd with [this project](https://github.com/HenryAWE/asbind20), and it crash when trying to complete the second argument of [this line](https://github.com/HenryAWE/asbind20/blob/55c3c2284bb60efc57e783f359baf4f7e97a250a/test/test_bind/ref_class.cpp#L110). You can reproduce this error by deleting the second argument (now this line is `.method("void from_var_type(const ?&in)")`), then type a comma (`,`) and trigger the code completion (`.method("void from_var_type(const ?&in)", ^)`, ^ is the position of input cursor).

My clangd version is 19.1.2, installed by the VSCode extension.

Stacktrace
```
 #0 0x000000000055715b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
 #1 0x0000000000554fbe llvm::sys::RunSignalHandlers() /__w/clangd/clangd/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #2 0x0000000000557af6 SignalHandler(int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #3 0x00007f15707d0320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #4 0x000000000095e5d1 clang::TemplateArgument::isPackExpansion() const /__w/clangd/clangd/llvm-project/clang/lib/AST/TemplateBase.cpp:323:3
 #5 0x0000000000cec428 clang::Sema::getMoreSpecializedTemplate(clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation, clang::TemplatePartialOrderingContext, unsigned int, clang::QualType, clang::QualType, bool) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:5669:51
 #6 0x0000000000c18299 clang::isBetterOverloadCandidate(clang::Sema&, clang::OverloadCandidate const&, clang::OverloadCandidate const&, clang::SourceLocation, clang::OverloadCandidateSet::CandidateSetKind) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaOverload.cpp:10458:50
 #7 0x0000000000f89e25 __move_merge_adaptive_backward<clang::OverloadCandidate *, clang::OverloadCandidate *, clang::OverloadCandidate *, __gnu_cxx::__ops::_Iter_comp_iter<(lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)> > /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:0:0
 #8 0x0000000000f89e25 void std::__merge_adaptive<clang::OverloadCandidate*, long, clang::OverloadCandidate*, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_16>>(clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, long, long, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_16>) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:2431:4
 #9 0x0000000000f87ed7 void std::__stable_sort_adaptive<clang::OverloadCandidate*, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_16>>(clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_16>) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:2756:5
#10 0x0000000000f59d81 __destroy<clang::OverloadCandidate *> /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_construct.h:107:19
#11 0x0000000000f59d81 _Destroy<clang::OverloadCandidate *> /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_construct.h:136:7
#12 0x0000000000f59d81 ~_Temporary_buffer /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_tempbuf.h:167:2
#13 0x0000000000f59d81 __stable_sort<clang::OverloadCandidate *, __gnu_cxx::__ops::_Iter_comp_iter<(lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)> > /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:5010:5
#14 0x0000000000f59d81 stable_sort<clang::OverloadCandidate *, (lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)> /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:5077:7
#15 0x0000000000f59d81 stable_sort<clang::OverloadCandidateSet &, (lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)> /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2001:3
#16 0x0000000000f59d81 mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:3
#17 0x0000000000f59a80 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6327:45
#18 0x000000000453cd6f operator() /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:2198:29
#19 0x000000000453cd6f clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:2213:15
#20 0x000000000453f636 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:1945:9
#21 0x000000000453b581 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:712:20
#22 0x0000000004539d73 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:185:10
#23 0x0000000004539ca9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:134:10
#24 0x00000000045a6d45 isInvalid /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#25 0x00000000045a6d45 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:555:12
#26 0x00000000045a5196 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:292:14
#27 0x00000000045a49b8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:124:20
#28 0x00000000045adcb1 clang::Parser::ParseCompoundStatementBody(bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:1248:11
#29 0x00000000045af26d clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:2526:21
#30 0x00000000044e3431 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:0
#31 0x0000000004504bde clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:0
#32 0x00000000044e2237 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:1249:10
#33 0x00000000044e1bb1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:1271:12
#34 0x00000000044e0d23 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:1074:14
#35 0x00000000045216cb ~AttributePool /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:726:22
#36 0x00000000045216cb ~ParsedAttributes /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:958:7
#37 0x00000000045216cb clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:273:5
#38 0x00000000045210a8 Exit /__w/clangd/clangd/llvm-project/clang/include/clang/Parse/Parser.h:1196:11
#39 0x00000000045210a8 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:252:18
#40 0x00000000045023ee clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:0
#41 0x00000000044e003a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:3
#42 0x00000000044deaf9 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:763:12
#43 0x00000000044de2df clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:608:26
#44 0x00000000044da548 clang::ParseAST(clang::Sema&, bool, bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseAST.cpp:170:25
#45 0x000000000411b83b clang::FrontendAction::Execute() /__w/clangd/clangd/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1082:10
#46 0x00000000015f9ac5 getPtr /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:282:12
#47 0x00000000015f9ac5 operator bool /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:242:16
#48 0x00000000015f9ac5 clang::clangd::(anonymous namespace)::semaCodeComplete(std::unique_ptr<clang::CodeCompleteConsumer, std::default_delete<clang::CodeCompleteConsumer>>, clang::CodeCompleteOptions const&, clang::clangd::(anonymous namespace)::SemaCompleteInput const&, clang::clangd::IncludeStructure*) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.cpp:1427:19
#49 0x00000000015fc734 ~unique_ptr /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/unique_ptr.h:262:6
#50 0x00000000015fc734 run /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.cpp:1694:5
#51 0x00000000015fc734 clang::clangd::codeComplete(llvm::StringRef, clang::clangd::Position, clang::clangd::PreambleData const*, clang::clangd::ParseInputs const&, clang::clangd::CodeCompleteOptions, clang::clangd::SpeculativeFuzzyFind*) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.cpp:2245:32
#52 0x00000000015eb5dd ~_Function_base /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/std_function.h:275:11
#53 0x00000000015eb5dd ~CodeCompleteOptions /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.h:42:8
#54 0x00000000015eb5dd operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/ClangdServer.cpp:456:33
#55 0x00000000015eb5dd void llvm::detail::UniqueFunctionBase<void, llvm::Expected<clang::clangd::InputsAndPreamble>>::CallImpl<clang::clangd::ClangdServer::codeComplete(llvm::StringRef, clang::clangd::Position, clang::clangd::CodeCompleteOptions const&, llvm::unique_function<void (llvm::Expected<clang::clangd::CodeCompleteResult>)>)::$_0>(void*, llvm::Expected<clang::clangd::InputsAndPreamble>&) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:220:12
#56 0x00000000017998fb ~Expected /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:563:10
#57 0x00000000017998fb operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1807:5
#58 0x00000000017998fb void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::TUScheduler::runWithPreamble(llvm::StringRef, llvm::StringRef, clang::clangd::TUScheduler::PreambleConsistency, llvm::unique_function<void (llvm::Expected<clang::clangd::InputsAndPreamble>)>)::$_3>(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:220:12
#59 0x000000000190f7c1 operator= /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:48:5
#60 0x000000000190f7c1 PunnedPointer /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:37:60
#61 0x000000000190f7c1 PointerIntPair /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:86:13
#62 0x000000000190f7c1 UniqueFunctionBase /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:340:3
#63 0x000000000190f7c1 unique_function /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:369:3
#64 0x000000000190f7c1 operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:103:14
#65 0x000000000190f7c1 operator()<(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &> /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:43:11
#66 0x000000000190f7c1 __invoke_impl<void, (lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &> /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/invoke.h:60:14
#67 0x000000000190f7c1 __invoke<(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &> /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/invoke.h:95:14
#68 0x000000000190f7c1 __apply_impl<(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple<(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)> &, 0> /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/tuple:1662:14
#69 0x000000000190f7c1 apply<(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple<(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)> &> /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/tuple:1671:14
#70 0x000000000190f7c1 GenericThreadProxy<std::tuple<(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)> > /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:41:5
#71 0x000000000190f7c1 void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:55:5
#72 0x00007f1570827a94 start_thread ./nptl/pthread_create.c:447:8
#73 0x00007f15708b4c3c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during preamble action: CodeComplete
  Filename: /home/awe/devel/asbind20/test/test_bind/ref_class.cpp
  Directory: /home/awe/devel/asbind20/build
  Command Line: /usr/lib/llvm-18/bin/clang++-18 --driver-mode=g++ -I/home/awe/devel/asbind20/test/. -I/home/awe/devel/asbind20/include -I/home/awe/devel/asbind20/ext/utility/include -I/home/awe/devel/asbind20/ext/container/include -isystem /home/awe/devel/asbind20/build/vcpkg_installed/x64-linux/include -isystem /home/awe/devel/asbind20/build/_deps/googletest-src/googletest/include -isystem /home/awe/devel/asbind20/build/_deps/googletest-src/googletest -g -std=gnu++20 -Wall -Wextra -Wpedantic -Werror -o test/test_bind/CMakeFiles/test_bind.dir/ref_class.cpp.o -c -resource-dir=/home/awe/.vscode-server/data/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/19.1.2/clangd_19.1.2/lib/clang/19 -- /home/awe/devel/asbind20/test/test_bind/ref_class.cpp
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: HenryAWE (HenryAWE)

<details>
I'm using clangd with [this project](https://github.com/HenryAWE/asbind20), and it crash when trying to complete the second argument of [this line](https://github.com/HenryAWE/asbind20/blob/55c3c2284bb60efc57e783f359baf4f7e97a250a/test/test_bind/ref_class.cpp#L110). You can reproduce this error by deleting the second argument (now this line is `.method("void from_var_type(const ?&amp;in)")`), then type a comma (`,`) and trigger the code completion (`.method("void from_var_type(const ?&amp;in)", ^)`, ^ is the position of input cursor).

My clangd version is 19.1.2, installed by the VSCode extension.

Stacktrace
```
 #<!-- -->0 0x000000000055715b llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x0000000000554fbe llvm::sys::RunSignalHandlers() /__w/clangd/clangd/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x0000000000557af6 SignalHandler(int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->3 0x00007f15707d0320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #<!-- -->4 0x000000000095e5d1 clang::TemplateArgument::isPackExpansion() const /__w/clangd/clangd/llvm-project/clang/lib/AST/TemplateBase.cpp:323:3
 #<!-- -->5 0x0000000000cec428 clang::Sema::getMoreSpecializedTemplate(clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation, clang::TemplatePartialOrderingContext, unsigned int, clang::QualType, clang::QualType, bool) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:5669:51
 #<!-- -->6 0x0000000000c18299 clang::isBetterOverloadCandidate(clang::Sema&amp;, clang::OverloadCandidate const&amp;, clang::OverloadCandidate const&amp;, clang::SourceLocation, clang::OverloadCandidateSet::CandidateSetKind) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaOverload.cpp:10458:50
 #<!-- -->7 0x0000000000f89e25 __move_merge_adaptive_backward&lt;clang::OverloadCandidate *, clang::OverloadCandidate *, clang::OverloadCandidate *, __gnu_cxx::__ops::_Iter_comp_iter&lt;(lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)&gt; &gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:0:0
 #<!-- -->8 0x0000000000f89e25 void std::__merge_adaptive&lt;clang::OverloadCandidate*, long, clang::OverloadCandidate*, __gnu_cxx::__ops::_Iter_comp_iter&lt;mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long)::$_16&gt;&gt;(clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, long, long, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter&lt;mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long)::$_16&gt;) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:2431:4
 #<!-- -->9 0x0000000000f87ed7 void std::__stable_sort_adaptive&lt;clang::OverloadCandidate*, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter&lt;mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long)::$_16&gt;&gt;(clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter&lt;mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long)::$_16&gt;) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:2756:5
#<!-- -->10 0x0000000000f59d81 __destroy&lt;clang::OverloadCandidate *&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_construct.h:107:19
#<!-- -->11 0x0000000000f59d81 _Destroy&lt;clang::OverloadCandidate *&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_construct.h:136:7
#<!-- -->12 0x0000000000f59d81 ~_Temporary_buffer /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_tempbuf.h:167:2
#<!-- -->13 0x0000000000f59d81 __stable_sort&lt;clang::OverloadCandidate *, __gnu_cxx::__ops::_Iter_comp_iter&lt;(lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)&gt; &gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:5010:5
#<!-- -->14 0x0000000000f59d81 stable_sort&lt;clang::OverloadCandidate *, (lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:5077:7
#<!-- -->15 0x0000000000f59d81 stable_sort&lt;clang::OverloadCandidateSet &amp;, (lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)&gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2001:3
#<!-- -->16 0x0000000000f59d81 mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:3
#<!-- -->17 0x0000000000f59a80 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6327:45
#<!-- -->18 0x000000000453cd6f operator() /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:2198:29
#<!-- -->19 0x000000000453cd6f clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:2213:15
#<!-- -->20 0x000000000453f636 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:1945:9
#<!-- -->21 0x000000000453b581 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:712:20
#<!-- -->22 0x0000000004539d73 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:185:10
#<!-- -->23 0x0000000004539ca9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:134:10
#<!-- -->24 0x00000000045a6d45 isInvalid /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#<!-- -->25 0x00000000045a6d45 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:555:12
#<!-- -->26 0x00000000045a5196 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:292:14
#<!-- -->27 0x00000000045a49b8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:124:20
#<!-- -->28 0x00000000045adcb1 clang::Parser::ParseCompoundStatementBody(bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:1248:11
#<!-- -->29 0x00000000045af26d clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:2526:21
#<!-- -->30 0x00000000044e3431 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:0
#<!-- -->31 0x0000000004504bde clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:0
#<!-- -->32 0x00000000044e2237 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:1249:10
#<!-- -->33 0x00000000044e1bb1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:1271:12
#<!-- -->34 0x00000000044e0d23 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:1074:14
#<!-- -->35 0x00000000045216cb ~AttributePool /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:726:22
#<!-- -->36 0x00000000045216cb ~ParsedAttributes /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:958:7
#<!-- -->37 0x00000000045216cb clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:273:5
#<!-- -->38 0x00000000045210a8 Exit /__w/clangd/clangd/llvm-project/clang/include/clang/Parse/Parser.h:1196:11
#<!-- -->39 0x00000000045210a8 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:252:18
#<!-- -->40 0x00000000045023ee clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:0
#<!-- -->41 0x00000000044e003a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:3
#<!-- -->42 0x00000000044deaf9 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:763:12
#<!-- -->43 0x00000000044de2df clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:608:26
#<!-- -->44 0x00000000044da548 clang::ParseAST(clang::Sema&amp;, bool, bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseAST.cpp:170:25
#<!-- -->45 0x000000000411b83b clang::FrontendAction::Execute() /__w/clangd/clangd/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1082:10
#<!-- -->46 0x00000000015f9ac5 getPtr /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:282:12
#<!-- -->47 0x00000000015f9ac5 operator bool /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:242:16
#<!-- -->48 0x00000000015f9ac5 clang::clangd::(anonymous namespace)::semaCodeComplete(std::unique_ptr&lt;clang::CodeCompleteConsumer, std::default_delete&lt;clang::CodeCompleteConsumer&gt;&gt;, clang::CodeCompleteOptions const&amp;, clang::clangd::(anonymous namespace)::SemaCompleteInput const&amp;, clang::clangd::IncludeStructure*) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.cpp:1427:19
#<!-- -->49 0x00000000015fc734 ~unique_ptr /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/unique_ptr.h:262:6
#<!-- -->50 0x00000000015fc734 run /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.cpp:1694:5
#<!-- -->51 0x00000000015fc734 clang::clangd::codeComplete(llvm::StringRef, clang::clangd::Position, clang::clangd::PreambleData const*, clang::clangd::ParseInputs const&amp;, clang::clangd::CodeCompleteOptions, clang::clangd::SpeculativeFuzzyFind*) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.cpp:2245:32
#<!-- -->52 0x00000000015eb5dd ~_Function_base /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/std_function.h:275:11
#<!-- -->53 0x00000000015eb5dd ~CodeCompleteOptions /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.h:42:8
#<!-- -->54 0x00000000015eb5dd operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/ClangdServer.cpp:456:33
#<!-- -->55 0x00000000015eb5dd void llvm::detail::UniqueFunctionBase&lt;void, llvm::Expected&lt;clang::clangd::InputsAndPreamble&gt;&gt;::CallImpl&lt;clang::clangd::ClangdServer::codeComplete(llvm::StringRef, clang::clangd::Position, clang::clangd::CodeCompleteOptions const&amp;, llvm::unique_function&lt;void (llvm::Expected&lt;clang::clangd::CodeCompleteResult&gt;)&gt;)::$_0&gt;(void*, llvm::Expected&lt;clang::clangd::InputsAndPreamble&gt;&amp;) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:220:12
#<!-- -->56 0x00000000017998fb ~Expected /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:563:10
#<!-- -->57 0x00000000017998fb operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1807:5
#<!-- -->58 0x00000000017998fb void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::TUScheduler::runWithPreamble(llvm::StringRef, llvm::StringRef, clang::clangd::TUScheduler::PreambleConsistency, llvm::unique_function&lt;void (llvm::Expected&lt;clang::clangd::InputsAndPreamble&gt;)&gt;)::$_3&gt;(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:220:12
#<!-- -->59 0x000000000190f7c1 operator= /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:48:5
#<!-- -->60 0x000000000190f7c1 PunnedPointer /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:37:60
#<!-- -->61 0x000000000190f7c1 PointerIntPair /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:86:13
#<!-- -->62 0x000000000190f7c1 UniqueFunctionBase /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:340:3
#<!-- -->63 0x000000000190f7c1 unique_function /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:369:3
#<!-- -->64 0x000000000190f7c1 operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:103:14
#<!-- -->65 0x000000000190f7c1 operator()&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &amp;&gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:43:11
#<!-- -->66 0x000000000190f7c1 __invoke_impl&lt;void, (lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &amp;&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/invoke.h:60:14
#<!-- -->67 0x000000000190f7c1 __invoke&lt;(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &amp;&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/invoke.h:95:14
#<!-- -->68 0x000000000190f7c1 __apply_impl&lt;(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)&gt; &amp;, 0&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/tuple:1662:14
#<!-- -->69 0x000000000190f7c1 apply&lt;(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)&gt; &amp;&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/tuple:1671:14
#<!-- -->70 0x000000000190f7c1 GenericThreadProxy&lt;std::tuple&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)&gt; &gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:41:5
#<!-- -->71 0x000000000190f7c1 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:55:5
#<!-- -->72 0x00007f1570827a94 start_thread ./nptl/pthread_create.c:447:8
#<!-- -->73 0x00007f15708b4c3c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during preamble action: CodeComplete
  Filename: /home/awe/devel/asbind20/test/test_bind/ref_class.cpp
  Directory: /home/awe/devel/asbind20/build
  Command Line: /usr/lib/llvm-18/bin/clang++-18 --driver-mode=g++ -I/home/awe/devel/asbind20/test/. -I/home/awe/devel/asbind20/include -I/home/awe/devel/asbind20/ext/utility/include -I/home/awe/devel/asbind20/ext/container/include -isystem /home/awe/devel/asbind20/build/vcpkg_installed/x64-linux/include -isystem /home/awe/devel/asbind20/build/_deps/googletest-src/googletest/include -isystem /home/awe/devel/asbind20/build/_deps/googletest-src/googletest -g -std=gnu++20 -Wall -Wextra -Wpedantic -Werror -o test/test_bind/CMakeFiles/test_bind.dir/ref_class.cpp.o -c -resource-dir=/home/awe/.vscode-server/data/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/19.1.2/clangd_19.1.2/lib/clang/19 -- /home/awe/devel/asbind20/test/test_bind/ref_class.cpp
```
</details>


---

### Comment 2 - HighCommander4

> I'm using clangd with [this project](https://github.com/HenryAWE/asbind20)

I attempted to reproduce this, but I need some more detailed setup instructions to do so.

 1. Clone https://github.com/HenryAWE/asbind20
 2. Download and extract https://www.angelcode.com/angelscript/sdk/files/angelscript_2.36.1.zip
 2. ???
 3. In `asbind20`, run `cmake -GNinja -DCMAKE_BUILD_TYPE=Release -S . -B build`

Can you help me fill in the blank? How do I tell your project's build about the location of angelscript?

---

### Comment 3 - HenryAWE

> > I'm using clangd with [this project](https://github.com/HenryAWE/asbind20)
> 
> I attempted to reproduce this, but I need some more detailed setup instructions to do so.
> 
> 1. Clone https://github.com/HenryAWE/asbind20
> 2. Download and extract https://www.angelcode.com/angelscript/sdk/files/angelscript_2.36.1.zip
> 3. ???
> 4. In `asbind20`, run `cmake -GNinja -DCMAKE_BUILD_TYPE=Release -S . -B build`
> 
> Can you help me fill in the blank? How do I tell your project's build about the location of angelscript?

You can use [vcpkg](https://github.com/microsoft/vcpkg) for handling the dependency. [(Example in the script for GitHub Action)](https://github.com/HenryAWE/asbind20/blob/master/.github/workflows/build.yml)

```sh
cmake
          -S .
          -B build
          -G Ninja
          -DCMAKE_CXX_STANDARD=20
          -DCMAKE_TOOLCHAIN_FILE="C:/vcpkg/scripts/buildsystems/vcpkg.cmake" # The location of vcpkg's cmake toolchain
          -Dasbind_build_test=1
```
Besides, the crash was caused by test code,  remember to use `-Dasbind_build_test=1` to active the unit test.

---

### Comment 4 - HighCommander4

Thanks. I can reproduce the issue.

Here is a reduced testcase:

```c++
namespace N
{

template <auto> class A {};
template <int...> struct B {};
template <int> class C;

struct S
{
    template <typename D, int Z>
    void waldo(D, C<Z>);

    template <auto X, int... Is, int Z>
    void waldo(A<X>, B<Is...>, C<Z>);
};

}

void foo()
{
    N::S s;
    s.waldo();  // invoke completion inside parentheses
}
```



---

### Comment 5 - HighCommander4

Reproduction steps that don't involve clangd:

 1. Create `test.cpp` containing code from previous comment
 2. Run `clang -fsyntax-only -Xclang -code-completion-at=test.cpp:22:13 test.cpp`

Output:

```console
$ clang-19 -fsyntax-only -Xclang -code-completion-at=test.cpp:22:13 test.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang-19 -fsyntax-only -Xclang -code-completion-at=test.cpp:22:13 test.cpp
1.      <eof> parser at end of file
2.      test.cpp:20:1: parsing function body 'foo'
3.      test.cpp:20:1: in compound statement ('{}')
 #0 0x00007f6f3d8a8246 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb4246)
 #1 0x00007f6f3d8a5e30 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb1e30)
 #2 0x00007f6f3d8a7694 llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xeb3694)
 #3 0x00007f6f3d7ef810 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfb810)
 #4 0x00007f6f48919140 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x13140)
 #5 0x00007f6f45818e45 clang::TemplateArgument::isPackExpansion() const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1278e45)
 #6 0x00007f6f46260fdc clang::Sema::getMoreSpecializedTemplate(clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation, clang::TemplatePartialOrderingContext, unsigned int, clang::QualType, clang::QualType, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1cc0fdc)
 #7 0x00007f6f4614782e clang::isBetterOverloadCandidate(clang::Sema&, clang::OverloadCandidate const&, clang::OverloadCandidate const&, clang::SourceLocation, clang::OverloadCandidateSet::CandidateSetKind) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1ba782e)
 #8 0x00007f6f45c2e213 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x168e213)
 #9 0x00007f6f45c2c318 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x168c318)
#10 0x00007f6f45c028ee (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x16628ee)
#11 0x00007f6f45c02489 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x1662489)
#12 0x00007f6f45235c44 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc95c44)
#13 0x00007f6f45223e8b clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc83e8b)
#14 0x00007f6f452330fa clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc930fa)
#15 0x00007f6f45226dbd clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc86dbd)
#16 0x00007f6f4522a70f clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc8a70f)
#17 0x00007f6f45223ef9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc83ef9)
#18 0x00007f6f45223dc9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc83dc9)
#19 0x00007f6f4528c465 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcec465)
#20 0x00007f6f4528a780 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcea780)
#21 0x00007f6f4528a156 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcea156)
#22 0x00007f6f45292cb0 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcf2cb0)
#23 0x00007f6f45293de2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xcf3de2)
#24 0x00007f6f452add60 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0dd60)
#25 0x00007f6f451ef8fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc4f8fd)
#26 0x00007f6f452acd59 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0cd59)
#27 0x00007f6f452ac70b clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0c70b)
#28 0x00007f6f452ab917 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd0b917)
#29 0x00007f6f452a9df7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xd09df7)
#30 0x00007f6f451d7c5e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0xc37c5e)
#31 0x00007f6f47290095 clang::FrontendAction::Execute() (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2cf0095)
#32 0x00007f6f47200d14 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2c60d14)
#33 0x00007f6f4730d45e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x2d6d45e)
#34 0x0000556fc2184b62 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-19/bin/clang+0x11b62)
#35 0x0000556fc2181a65 (/usr/lib/llvm-19/bin/clang+0xea65)
#36 0x00007f6f46e98e49 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28f8e49)
#37 0x00007f6f3d7ef5a8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xdfb5a8)
#38 0x00007f6f46e987cd clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28f87cd)
#39 0x00007f6f46e5e225 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28be225)
#40 0x00007f6f46e5e48e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28be48e)
#41 0x00007f6f46e7b5ec clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x28db5ec)
#42 0x0000556fc21814c0 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-19/bin/clang+0xe4c0)
#43 0x0000556fc218ee16 main (/usr/lib/llvm-19/bin/clang+0x1be16)
#44 0x00007f6f3c516d7a __libc_start_main ./csu/../csu/libc-start.c:308:16
#45 0x0000556fc217f72a _start (/usr/lib/llvm-19/bin/clang+0xc72a)
clang-19: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 19.1.7 (++20250114103043+cd708029e0b2-1~exp1~20250114103143.80)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
```

---

### Comment 6 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: HenryAWE (HenryAWE)

<details>
I'm using clangd with [this project](https://github.com/HenryAWE/asbind20), and it crash when trying to complete the second argument of [this line](https://github.com/HenryAWE/asbind20/blob/55c3c2284bb60efc57e783f359baf4f7e97a250a/test/test_bind/ref_class.cpp#L110). You can reproduce this error by deleting the second argument (now this line is `.method("void from_var_type(const ?&amp;in)")`), then type a comma (`,`) and trigger the code completion (`.method("void from_var_type(const ?&amp;in)", ^)`, ^ is the position of input cursor).

My clangd version is 19.1.2, installed by the VSCode extension.

Stacktrace
```
 #<!-- -->0 0x000000000055715b llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x0000000000554fbe llvm::sys::RunSignalHandlers() /__w/clangd/clangd/llvm-project/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x0000000000557af6 SignalHandler(int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->3 0x00007f15707d0320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #<!-- -->4 0x000000000095e5d1 clang::TemplateArgument::isPackExpansion() const /__w/clangd/clangd/llvm-project/clang/lib/AST/TemplateBase.cpp:323:3
 #<!-- -->5 0x0000000000cec428 clang::Sema::getMoreSpecializedTemplate(clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation, clang::TemplatePartialOrderingContext, unsigned int, clang::QualType, clang::QualType, bool) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:5669:51
 #<!-- -->6 0x0000000000c18299 clang::isBetterOverloadCandidate(clang::Sema&amp;, clang::OverloadCandidate const&amp;, clang::OverloadCandidate const&amp;, clang::SourceLocation, clang::OverloadCandidateSet::CandidateSetKind) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaOverload.cpp:10458:50
 #<!-- -->7 0x0000000000f89e25 __move_merge_adaptive_backward&lt;clang::OverloadCandidate *, clang::OverloadCandidate *, clang::OverloadCandidate *, __gnu_cxx::__ops::_Iter_comp_iter&lt;(lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)&gt; &gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:0:0
 #<!-- -->8 0x0000000000f89e25 void std::__merge_adaptive&lt;clang::OverloadCandidate*, long, clang::OverloadCandidate*, __gnu_cxx::__ops::_Iter_comp_iter&lt;mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long)::$_16&gt;&gt;(clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, long, long, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter&lt;mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long)::$_16&gt;) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:2431:4
 #<!-- -->9 0x0000000000f87ed7 void std::__stable_sort_adaptive&lt;clang::OverloadCandidate*, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter&lt;mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long)::$_16&gt;&gt;(clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter&lt;mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long)::$_16&gt;) /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:2756:5
#<!-- -->10 0x0000000000f59d81 __destroy&lt;clang::OverloadCandidate *&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_construct.h:107:19
#<!-- -->11 0x0000000000f59d81 _Destroy&lt;clang::OverloadCandidate *&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_construct.h:136:7
#<!-- -->12 0x0000000000f59d81 ~_Temporary_buffer /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_tempbuf.h:167:2
#<!-- -->13 0x0000000000f59d81 __stable_sort&lt;clang::OverloadCandidate *, __gnu_cxx::__ops::_Iter_comp_iter&lt;(lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)&gt; &gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:5010:5
#<!-- -->14 0x0000000000f59d81 stable_sort&lt;clang::OverloadCandidate *, (lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/stl_algo.h:5077:7
#<!-- -->15 0x0000000000f59d81 stable_sort&lt;clang::OverloadCandidateSet &amp;, (lambda at /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:35)&gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/STLExtras.h:2001:3
#<!-- -->16 0x0000000000f59d81 mergeCandidatesWithResults(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::CodeCompleteConsumer::OverloadCandidate&gt;&amp;, clang::OverloadCandidateSet&amp;, clang::SourceLocation, unsigned long) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6103:3
#<!-- -->17 0x0000000000f59a80 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) /__w/clangd/clangd/llvm-project/clang/lib/Sema/SemaCodeComplete.cpp:6327:45
#<!-- -->18 0x000000000453cd6f operator() /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:2198:29
#<!-- -->19 0x000000000453cd6f clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:2213:15
#<!-- -->20 0x000000000453f636 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:1945:9
#<!-- -->21 0x000000000453b581 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:712:20
#<!-- -->22 0x0000000004539d73 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:185:10
#<!-- -->23 0x0000000004539ca9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseExpr.cpp:134:10
#<!-- -->24 0x00000000045a6d45 isInvalid /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/Ownership.h:199:41
#<!-- -->25 0x00000000045a6d45 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:555:12
#<!-- -->26 0x00000000045a5196 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:292:14
#<!-- -->27 0x00000000045a49b8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:124:20
#<!-- -->28 0x00000000045adcb1 clang::Parser::ParseCompoundStatementBody(bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:1248:11
#<!-- -->29 0x00000000045af26d clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseStmt.cpp:2526:21
#<!-- -->30 0x00000000044e3431 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:0
#<!-- -->31 0x0000000004504bde clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:0
#<!-- -->32 0x00000000044e2237 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:1249:10
#<!-- -->33 0x00000000044e1bb1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:1271:12
#<!-- -->34 0x00000000044e0d23 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:1074:14
#<!-- -->35 0x00000000045216cb ~AttributePool /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:726:22
#<!-- -->36 0x00000000045216cb ~ParsedAttributes /__w/clangd/clangd/llvm-project/clang/include/clang/Sema/ParsedAttr.h:958:7
#<!-- -->37 0x00000000045216cb clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:273:5
#<!-- -->38 0x00000000045210a8 Exit /__w/clangd/clangd/llvm-project/clang/include/clang/Parse/Parser.h:1196:11
#<!-- -->39 0x00000000045210a8 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:252:18
#<!-- -->40 0x00000000045023ee clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseDecl.cpp:0:0
#<!-- -->41 0x00000000044e003a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:0:3
#<!-- -->42 0x00000000044deaf9 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:763:12
#<!-- -->43 0x00000000044de2df clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /__w/clangd/clangd/llvm-project/clang/lib/Parse/Parser.cpp:608:26
#<!-- -->44 0x00000000044da548 clang::ParseAST(clang::Sema&amp;, bool, bool) /__w/clangd/clangd/llvm-project/clang/lib/Parse/ParseAST.cpp:170:25
#<!-- -->45 0x000000000411b83b clang::FrontendAction::Execute() /__w/clangd/clangd/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1082:10
#<!-- -->46 0x00000000015f9ac5 getPtr /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:282:12
#<!-- -->47 0x00000000015f9ac5 operator bool /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:242:16
#<!-- -->48 0x00000000015f9ac5 clang::clangd::(anonymous namespace)::semaCodeComplete(std::unique_ptr&lt;clang::CodeCompleteConsumer, std::default_delete&lt;clang::CodeCompleteConsumer&gt;&gt;, clang::CodeCompleteOptions const&amp;, clang::clangd::(anonymous namespace)::SemaCompleteInput const&amp;, clang::clangd::IncludeStructure*) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.cpp:1427:19
#<!-- -->49 0x00000000015fc734 ~unique_ptr /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/unique_ptr.h:262:6
#<!-- -->50 0x00000000015fc734 run /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.cpp:1694:5
#<!-- -->51 0x00000000015fc734 clang::clangd::codeComplete(llvm::StringRef, clang::clangd::Position, clang::clangd::PreambleData const*, clang::clangd::ParseInputs const&amp;, clang::clangd::CodeCompleteOptions, clang::clangd::SpeculativeFuzzyFind*) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.cpp:2245:32
#<!-- -->52 0x00000000015eb5dd ~_Function_base /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/std_function.h:275:11
#<!-- -->53 0x00000000015eb5dd ~CodeCompleteOptions /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/CodeComplete.h:42:8
#<!-- -->54 0x00000000015eb5dd operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/ClangdServer.cpp:456:33
#<!-- -->55 0x00000000015eb5dd void llvm::detail::UniqueFunctionBase&lt;void, llvm::Expected&lt;clang::clangd::InputsAndPreamble&gt;&gt;::CallImpl&lt;clang::clangd::ClangdServer::codeComplete(llvm::StringRef, clang::clangd::Position, clang::clangd::CodeCompleteOptions const&amp;, llvm::unique_function&lt;void (llvm::Expected&lt;clang::clangd::CodeCompleteResult&gt;)&gt;)::$_0&gt;(void*, llvm::Expected&lt;clang::clangd::InputsAndPreamble&gt;&amp;) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:220:12
#<!-- -->56 0x00000000017998fb ~Expected /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:563:10
#<!-- -->57 0x00000000017998fb operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/TUScheduler.cpp:1807:5
#<!-- -->58 0x00000000017998fb void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::TUScheduler::runWithPreamble(llvm::StringRef, llvm::StringRef, clang::clangd::TUScheduler::PreambleConsistency, llvm::unique_function&lt;void (llvm::Expected&lt;clang::clangd::InputsAndPreamble&gt;)&gt;)::$_3&gt;(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:220:12
#<!-- -->59 0x000000000190f7c1 operator= /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:48:5
#<!-- -->60 0x000000000190f7c1 PunnedPointer /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:37:60
#<!-- -->61 0x000000000190f7c1 PointerIntPair /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:86:13
#<!-- -->62 0x000000000190f7c1 UniqueFunctionBase /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:340:3
#<!-- -->63 0x000000000190f7c1 unique_function /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:369:3
#<!-- -->64 0x000000000190f7c1 operator() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:103:14
#<!-- -->65 0x000000000190f7c1 operator()&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &amp;&gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:43:11
#<!-- -->66 0x000000000190f7c1 __invoke_impl&lt;void, (lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &amp;&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/invoke.h:60:14
#<!-- -->67 0x000000000190f7c1 __invoke&lt;(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), (lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15) &amp;&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/bits/invoke.h:95:14
#<!-- -->68 0x000000000190f7c1 __apply_impl&lt;(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)&gt; &amp;, 0&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/tuple:1662:14
#<!-- -->69 0x000000000190f7c1 apply&lt;(lambda at /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:42:9), std::tuple&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)&gt; &amp;&gt; /usr/bin/../lib/gcc/x86_64-linux-gnu/7.5.0/../../../../include/c++/7.5.0/tuple:1671:14
#<!-- -->70 0x000000000190f7c1 GenericThreadProxy&lt;std::tuple&lt;(lambda at /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/support/Threading.cpp:98:15)&gt; &gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:41:5
#<!-- -->71 0x000000000190f7c1 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/thread.h:55:5
#<!-- -->72 0x00007f1570827a94 start_thread ./nptl/pthread_create.c:447:8
#<!-- -->73 0x00007f15708b4c3c clone3 ./misc/../sysdeps/unix/sysv/linux/x86_64/clone3.S:80:0
Signalled during preamble action: CodeComplete
  Filename: /home/awe/devel/asbind20/test/test_bind/ref_class.cpp
  Directory: /home/awe/devel/asbind20/build
  Command Line: /usr/lib/llvm-18/bin/clang++-18 --driver-mode=g++ -I/home/awe/devel/asbind20/test/. -I/home/awe/devel/asbind20/include -I/home/awe/devel/asbind20/ext/utility/include -I/home/awe/devel/asbind20/ext/container/include -isystem /home/awe/devel/asbind20/build/vcpkg_installed/x64-linux/include -isystem /home/awe/devel/asbind20/build/_deps/googletest-src/googletest/include -isystem /home/awe/devel/asbind20/build/_deps/googletest-src/googletest -g -std=gnu++20 -Wall -Wextra -Wpedantic -Werror -o test/test_bind/CMakeFiles/test_bind.dir/ref_class.cpp.o -c -resource-dir=/home/awe/.vscode-server/data/User/globalStorage/llvm-vs-code-extensions.vscode-clangd/install/19.1.2/clangd_19.1.2/lib/clang/19 -- /home/awe/devel/asbind20/test/test_bind/ref_class.cpp
```
</details>


---

### Comment 7 - HighCommander4

This is a regression in clang 18. Newer versions including trunk are affected as well.

---

### Comment 8 - HighCommander4

Slightly more informative backtrace from a debug build:

```console
$ clang -fsyntax-only -Xclang -code-completion-at=test.cpp:22:13 test.cpp
clang: clang/lib/Sema/SemaTemplateDeduction.cpp:5912: FunctionTemplateDecl *clang::Sema::getMoreSpecializedTemplate(FunctionTemplateDecl *, FunctionTemplateDecl *, SourceLocation, TemplatePartialOrderingContext, unsigned int, QualType, QualType, bool): Assertion `TA2.getKind() == TemplateArgument::Pack' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: build/bin/clang -fsyntax-only -Xclang -code-completion-at=test.cpp:22:13 test.cpp
1.      <eof> parser at end of file
2.      test.cpp:20:1: parsing function body 'foo'
3.      test.cpp:20:1: in compound statement ('{}')
 #0 0x00007f8de3986bed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm/lib/Support/Unix/Signals.inc:800:11
 #1 0x00007f8de39870db PrintStackTraceSignalHandler(void*) llvm/lib/Support/Unix/Signals.inc:876:1
 #2 0x00007f8de3985166 llvm::sys::RunSignalHandlers() llvm/lib/Support/Signals.cpp:105:5
 #3 0x00007f8de398647e llvm::sys::CleanupOnSignal(unsigned long) llvm/lib/Support/Unix/Signals.inc:372:1
 #4 0x00007f8de3823e77 (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) llvm/lib/Support/CrashRecoveryContext.cpp:0:7
 #5 0x00007f8de3824215 CrashRecoverySignalHandler(int) llvm/lib/Support/CrashRecoveryContext.cpp:391:1
 #6 0x00007f8deca24140 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x13140)
 #7 0x00007f8de30a2d51 raise ./signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #8 0x00007f8de308c537 abort ./stdlib/abort.c:81:7
 #9 0x00007f8de308c40f get_sysdep_segment_value ./intl/loadmsgcat.c:509:8
#10 0x00007f8de308c40f _nl_load_domain ./intl/loadmsgcat.c:970:34
#11 0x00007f8de309b6d2 (/lib/x86_64-linux-gnu/libc.so.6+0x316d2)
#12 0x00007f8ddc8863cd clang::Sema::getMoreSpecializedTemplate(clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation, clang::TemplatePartialOrderingContext, unsigned int, clang::QualType, clang::QualType, bool) clang/lib/Sema/SemaTemplateDeduction.cpp:5913:28
#13 0x00007f8ddc68e1c0 clang::isBetterOverloadCandidate(clang::Sema&, clang::OverloadCandidate const&, clang::OverloadCandidate const&, clang::SourceLocation, clang::OverloadCandidateSet::CandidateSetKind) clang/lib/Sema/SemaOverload.cpp:10597:31
#14 0x00007f8ddbc9bc68 mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0::operator()(clang::OverloadCandidate const&, clang::OverloadCandidate const&) const clang/lib/Sema/SemaCodeComplete.cpp:6233:5
#15 0x00007f8ddbc9bb05 bool __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>::operator()<clang::OverloadCandidate*, clang::OverloadCandidate*>(clang::OverloadCandidate*, clang::OverloadCandidate*) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/predefined_ops.h:156:11
#16 0x00007f8ddbc9c8f6 void std::__move_merge_adaptive<clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>>(clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/stl_algo.h:2326:8
#17 0x00007f8ddbc9c14a void std::__merge_adaptive<clang::OverloadCandidate*, long, clang::OverloadCandidate*, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>>(clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, long, long, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/stl_algo.h:2439:2
#18 0x00007f8ddbc9b5e8 void std::__stable_sort_adaptive<clang::OverloadCandidate*, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>>(clang::OverloadCandidate*, clang::OverloadCandidate*, clang::OverloadCandidate*, long, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/stl_algo.h:2768:5
#19 0x00007f8ddbc9b12f void std::__stable_sort<clang::OverloadCandidate*, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>>(clang::OverloadCandidate*, clang::OverloadCandidate*, __gnu_cxx::__ops::_Iter_comp_iter<mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/stl_algo.h:5039:5
#20 0x00007f8ddbc9b00b void std::stable_sort<clang::OverloadCandidate*, mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>(clang::OverloadCandidate*, clang::OverloadCandidate*, mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/stl_algo.h:5108:5
#21 0x00007f8ddbc9af6e void llvm::stable_sort<clang::OverloadCandidateSet&, mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0>(clang::OverloadCandidateSet&, mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long)::$_0) llvm/include/llvm/ADT/STLExtras.h:2044:1
#22 0x00007f8ddbc7d24c mergeCandidatesWithResults(clang::Sema&, llvm::SmallVectorImpl<clang::CodeCompleteConsumer::OverloadCandidate>&, clang::OverloadCandidateSet&, clang::SourceLocation, unsigned long) clang/lib/Sema/SemaCodeComplete.cpp:6238:39
#23 0x00007f8ddbc7ccf4 clang::SemaCodeCompletion::ProduceCallSignatureHelp(clang::Expr*, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation) clang/lib/Sema/SemaCodeComplete.cpp:6454:3
#24 0x00007f8ddcdd6cb9 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>)::$_1::operator()() const clang/lib/Parse/ParseExpr.cpp:2195:38
#25 0x00007f8ddcdd92f5 clang::QualType llvm::function_ref<clang::QualType ()>::callback_fn<clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>)::$_1>(long) llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#26 0x00007f8ddcddb069 llvm::function_ref<clang::QualType ()>::operator()() const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#27 0x00007f8ddcdd9429 clang::PreferredTypeBuilder::get(clang::SourceLocation) const clang/include/clang/Sema/Sema.h:333:14
#28 0x00007f8ddcdc6090 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) clang/lib/Parse/ParseExpr.cpp:175:38
#29 0x00007f8ddcdd6779 clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool, bool, bool*) clang/lib/Parse/ParseExpr.cpp:3684:14
#30 0x00007f8ddcdc926f clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) clang/lib/Parse/ParseExpr.cpp:2203:16
#31 0x00007f8ddcdceba1 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) clang/lib/Parse/ParseExpr.cpp:1944:9
#32 0x00007f8ddcdc7b9c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) clang/lib/Parse/ParseExpr.cpp:710:20
#33 0x00007f8ddcdc6131 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) clang/lib/Parse/ParseExpr.cpp:184:20
#34 0x00007f8ddcdc5fdf clang::Parser::ParseExpression(clang::Parser::TypeCastState) clang/lib/Parse/ParseExpr.cpp:135:18
#35 0x00007f8ddce71868 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) clang/lib/Parse/ParseStmt.cpp:564:19
#36 0x00007f8ddce6fbaa clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) clang/lib/Parse/ParseStmt.cpp:293:14
#37 0x00007f8ddce6f21a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) clang/lib/Parse/ParseStmt.cpp:125:20
#38 0x00007f8ddce788f9 clang::Parser::ParseCompoundStatementBody(bool) clang/lib/Parse/ParseStmt.cpp:1267:11
#39 0x00007f8ddce7a077 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) clang/lib/Parse/ParseStmt.cpp:2577:21
#40 0x00007f8ddcea4350 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) clang/lib/Parse/Parser.cpp:1520:3
#41 0x00007f8ddcd5b995 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) clang/lib/Parse/ParseDecl.cpp:2459:21
#42 0x00007f8ddcea310c clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1244:10
#43 0x00007f8ddcea25ee clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) clang/lib/Parse/Parser.cpp:1266:12
#44 0x00007f8ddcea1e66 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) clang/lib/Parse/Parser.cpp:1069:14
#45 0x00007f8ddce9fcb8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) clang/lib/Parse/Parser.cpp:758:12
#46 0x00007f8ddcd327cc clang::ParseAST(clang::Sema&, bool, bool) clang/lib/Parse/ParseAST.cpp:171:16
#47 0x00007f8de8762851 clang::ASTFrontendAction::ExecuteAction() clang/lib/Frontend/FrontendAction.cpp:1193:1
#48 0x00007f8de876226c clang::FrontendAction::Execute() clang/lib/Frontend/FrontendAction.cpp:1081:7
#49 0x00007f8de8656aa6 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) clang/lib/Frontend/CompilerInstance.cpp:1062:23
#50 0x00007f8dec9f75be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:296:8
#51 0x0000556b25d2083b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) clang/tools/driver/cc1_main.cpp:286:13
#52 0x0000556b25d13122 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) clang/tools/driver/driver.cpp:218:5
#53 0x0000556b25d13be0 clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const clang/tools/driver/driver.cpp:360:9
#54 0x0000556b25d13bad int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#55 0x00007f8de7fa0a21 llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#56 0x00007f8de7f9aaa8 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0::operator()() const clang/lib/Driver/Job.cpp:437:34
#57 0x00007f8de7f9aa75 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#58 0x00007f8de380fa49 llvm::function_ref<void ()>::operator()() const llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#59 0x00007f8de3823c6a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) llvm/lib/Support/CrashRecoveryContext.cpp:427:3
#60 0x00007f8de7f9a40b clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const clang/lib/Driver/Job.cpp:437:7
#61 0x00007f8de7f13e78 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const clang/lib/Driver/Compilation.cpp:196:15
#62 0x00007f8de7f14087 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const clang/lib/Driver/Compilation.cpp:251:13
#63 0x00007f8de7f36ff2 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) clang/lib/Driver/Driver.cpp:2124:7
#64 0x0000556b25d12beb clang_main(int, char**, llvm::ToolContext const&) clang/tools/driver/driver.cpp:398:9
#65 0x0000556b25d46a15 main build/tools/clang/tools/driver/clang-driver.cpp:17:3
#66 0x00007f8de308dd7a __libc_start_main ./csu/../csu/libc-start.c:308:16
#67 0x0000556b25d118fa _start (build/bin/clang+0x3c8fa)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

Note in particular the assertion failure: 

> clang/lib/Sema/SemaTemplateDeduction.cpp:5912: FunctionTemplateDecl *clang::Sema::getMoreSpecializedTemplate(FunctionTemplateDecl *, FunctionTemplateDecl *, SourceLocation, TemplatePartialOrderingContext, unsigned int, QualType, QualType, bool): Assertion `TA2.getKind() == TemplateArgument::Pack' failed.

---

### Comment 9 - zyn0217

Does it crash without -codecomplete?

---

### Comment 10 - HighCommander4

> Does it crash without -codecomplete?

It does not, at least not on the reduced testcase as written. But the crash is in parser code, and my understanding is that code completion is "just" a parse that's cut off at the cursor, so I suspect a test case that crashes during normal compilation can be constructed.

---

### Comment 11 - HighCommander4

> But the crash is in parser code

Never mind, there is `ProduceCallSignatureHelp` in the backtrace.

So the issue could be specific to code completion.

---

### Comment 12 - HenryAWE

> So the issue could be specific to code completion.

My project can be successfully compiled & run by Clang 18 on Ubuntu 2404 and Emscripten 4.0.1 (whose frontend is Clang 20). I'm sure that it only crashed for code completion.

---

### Comment 13 - HighCommander4

> This is a regression in clang 18

It looks like I was mistaken about this being new in clang 18. (I think I tried with an assertions-disabled build of clang 17 and got lucky in that it did not crash.)

It is still a regression, but from earlier, sometime between 2022-09-30 and 2023-03-01 (so clang 16 or clang 17 time frame). Haven't been able to narrow it down further so far.

---

### Comment 14 - HighCommander4

> It is still a regression, but from earlier, sometime between 2022-09-30 and 2023-03-01 (so clang 16 or clang 17 time frame). Haven't been able to narrow it down further so far.

The regressing commit is https://github.com/llvm/llvm-project/commit/1fb728e95c74bf7.

cc @erichkeane, @mizvekov (reviewers of the regressing patch -- I can't find a Github account for the patch author)

(This regression window was brought to you by [manyclangs](https://github.com/elfshaker/manyclangs).)

---

### Comment 15 - erichkeane

The original author is @yuanfang-chen who may still be active?  Not sure.  It'll probably take a while to page this one in mentally though.

---

### Comment 16 - erichkeane

I looked over this a while, and I really hope that @mizvekov stops by for this one, as I'm not nearly as knowledgeable about template deduction and potential speculative fixes.  The issue here is that we have:
```
    template <typename D, int Z>
    void waldo(D, C<Z>);
AND
    template <auto X, int... Is, int Z>
    void waldo(A<X>, B<Is...>, C<Z>);
```

BOTH being considered together.  It is a touch shocking that it makes it this far, but it manages to go to: https://github.com/llvm/llvm-project/commit/1fb728e95c74bf7#diff-e6172eaed3f75b260e9bd0611fe9d43f2cd6d7d5a9384b8e17b8ccf74ac8e75cR5239

This speculative fix attempts to choose the smaller pack, but to do so has to assume that both sides are trying to fill in a pack for the 2nd argument (`C<Z>` vs `B<Is...>`, note of course that those are reversed, `B<Is...>` is the type from `FD1`.

I would think that we would want to choose a winner (I also see the assert that the size of the arguments lists match, which also doesn't seem sound here, so perhaps we need to dive into those as well?)) in this case.  I would THINK the non-pack would be the 'winner' (or have it be ambiguous?) in any sort of speculative fix here?  I tried that out and the code completion obviously no longer crashes, but ends up just emitting a ton of options that seems like "anything can go here".

So @mizvekov : DO you have thoughts?  Could you put a patch together with them for us to play with?



---

### Comment 17 - HighCommander4

> the code completion obviously no longer crashes, but ends up just emitting a ton of options that seems like "anything can go here"

I think this part is fine; that seems to have been the behaviour before the crash as well. The context for the code completion is an argument expression in a function call, so any identifier visible in that scope, as well as various non-identifier tokens like `const_cast`, could be valid as the beginning of an argument expression there.

---

### Comment 18 - mizvekov

Will be fixed by https://github.com/llvm/llvm-project/pull/129436

---

