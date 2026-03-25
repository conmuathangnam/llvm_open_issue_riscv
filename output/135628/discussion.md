# clang++: clang/lib/AST/NestedNameSpecifier.cpp:307: void clang::NestedNameSpecifier::print(raw_ostream &, const PrintingPolicy &, bool) const: Assertion `!isa<ElaboratedType>(T) && "Elaborated type in nested-name-specifier"' failed.

**Author:** avikivity
**URL:** https://github.com/llvm/llvm-project/issues/135628

## Body

```
/home/avi/clang.bin/bin/clang++ -MD -MT build/release/gen/cql3/CqlParser.o -MF build/release/gen/cql3/CqlParser.o.d -std=gnu++23 -I/home/avi/scylla/seastar/include -I/home/avi/scylla/build/release/seastar/gen/include -Werror=unused-result -DSEASTAR_API_LEVEL=7 -DSEASTAR_SSTRING -DSEASTAR_LOGGER_COMPILE_TIME_FMT -DSEASTAR_SCHEDULING_GROUPS_COUNT=19 -DSEASTAR_LOGGER_TYPE_STDOUT -DBOOST_PROGRAM_OPTIONS_NO_LIB -DBOOST_PROGRAM_OPTIONS_DYN_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_DYN_LINK -DFMT_SHARED -I/usr/include/p11-kit-1 -ffile-prefix-map=/home/avi/scylla=. -DSEASTAR_NO_EXCEPTION_HACK -march=westmere -Wno-backend-plugin -flto=thin -ffat-lto-objects -fprofile-use=/home/avi/scylla/build/profile.profdata -mllvm -pgso=false -mllvm -enable-value-profiling=false -ffunction-sections -fdata-sections  -O3 -mllvm -inline-threshold=2500 -fno-slp-vectorize -DSCYLLA_BUILD_MODE=release -g -gz -Xclang -fexperimental-assignment-tracking=disabled -iquote. -iquote build/release/gen -std=gnu++23  -ffile-prefix-map=/home/avi/scylla=. -DSEASTAR_NO_EXCEPTION_HACK -march=westmere -DBOOST_ALL_DYN_LINK    -fvisibility=hidden -Ikmipc/kmipc-2.1.0t-rhel84_64/include -DHAVE_KMIP -isystem abseil -Wall -Werror -Wextra -Wimplicit-fallthrough -Wno-mismatched-tags -Wno-c++11-narrowing -Wno-overloaded-virtual -Wno-unused-parameter -Wno-unsupported-friend -Wno-missing-field-initializers -Wno-deprecated-copy -Wno-error=deprecated-declarations -DXXH_PRIVATE_API -DSEASTAR_TESTING_MAIN -Wno-parentheses-equality -c -o build/release/gen/cql3/CqlParser.o build/release/gen/cql3/CqlParser.cpp
clang++: /home/avi/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:307: void clang::NestedNameSpecifier::print(raw_ostream &, const PrintingPolicy &, bool) const: Assertion `!isa<ElaboratedType>(T) && "Elaborated type in nested-name-specifier"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/avi/clang.bin/bin/clang++ -MD -MT build/release/gen/cql3/CqlParser.o -MF build/release/gen/cql3/CqlParser.o.d -std=gnu++23 -I/home/avi/scylla/seastar/include -I/home/avi/scylla/build/release/seastar/gen/include -Werror=unused-result -DSEASTAR_API_LEVEL=7 -DSEASTAR_SSTRING -DSEASTAR_LOGGER_COMPILE_TIME_FMT -DSEASTAR_SCHEDULING_GROUPS_COUNT=19 -DSEASTAR_LOGGER_TYPE_STDOUT -DBOOST_PROGRAM_OPTIONS_NO_LIB -DBOOST_PROGRAM_OPTIONS_DYN_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_DYN_LINK -DFMT_SHARED -I/usr/include/p11-kit-1 -ffile-prefix-map=/home/avi/scylla=. -DSEASTAR_NO_EXCEPTION_HACK -march=westmere -Wno-backend-plugin -flto=thin -ffat-lto-objects -fprofile-use=/home/avi/scylla/build/profile.profdata -mllvm -pgso=false -mllvm -enable-value-profiling=false -ffunction-sections -fdata-sections -O3 -mllvm -inline-threshold=2500 -fno-slp-vectorize -DSCYLLA_BUILD_MODE=release -g -gz -Xclang -fexperimental-assignment-tracking=disabled -iquote. -iquote build/release/gen -std=gnu++23 -ffile-prefix-map=/home/avi/scylla=. -DSEASTAR_NO_EXCEPTION_HACK -march=westmere -DBOOST_ALL_DYN_LINK -fvisibility=hidden -Ikmipc/kmipc-2.1.0t-rhel84_64/include -DHAVE_KMIP -isystem abseil -Wall -Werror -Wextra -Wimplicit-fallthrough -Wno-mismatched-tags -Wno-c++11-narrowing -Wno-overloaded-virtual -Wno-unused-parameter -Wno-unsupported-friend -Wno-missing-field-initializers -Wno-deprecated-copy -Wno-error=deprecated-declarations -DXXH_PRIVATE_API -DSEASTAR_TESTING_MAIN -Wno-parentheses-equality -c -o build/release/gen/cql3/CqlParser.o build/release/gen/cql3/CqlParser.cpp
1.	build/release/gen/idl/query.dist.impl.hh:40:8: current parser token ')'
2.	build/release/gen/idl/query.dist.impl.hh:17:1: parsing namespace 'ser'
3.	build/release/gen/idl/query.dist.impl.hh:30:1: parsing struct/union/class body 'ser::qr_cell_view'
4.	build/release/gen/idl/query.dist.impl.hh:34:28: parsing function body 'ser::qr_cell_view::timestamp'
5.	build/release/gen/idl/query.dist.impl.hh:34:28: in compound statement ('{}')
  #0 0x0000000007e7b159 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/avi/llvm-project/llvm/lib/Support/Unix/Signals.inc:799:11
  #1 0x0000000007e7b738 PrintStackTraceSignalHandler(void*) /home/avi/llvm-project/llvm/lib/Support/Unix/Signals.inc:875:1
  #2 0x0000000007e7934c llvm::sys::RunSignalHandlers() /home/avi/llvm-project/llvm/lib/Support/Signals.cpp:105:5
  #3 0x0000000007e7a806 llvm::sys::CleanupOnSignal(unsigned long) /home/avi/llvm-project/llvm/lib/Support/Unix/Signals.inc:370:1
  #4 0x0000000007d5262f (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/avi/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:0:7
  #5 0x0000000007d52af1 CrashRecoverySignalHandler(int) /home/avi/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
  #6 0x00007fce0c226df0 __restore_rt (/lib64/libc.so.6+0x19df0)
  #7 0x00007fce0c28027c __pthread_kill_implementation (/lib64/libc.so.6+0x7327c)
  #8 0x00007fce0c226cbe gsignal (/lib64/libc.so.6+0x19cbe)
  #9 0x00007fce0c20e6d6 abort (/lib64/libc.so.6+0x16d6)
 #10 0x00007fce0c20e639 __assert_perror_fail (/lib64/libc.so.6+0x1639)
 #11 0x000000000fb55bee clang::NestedNameSpecifier::print(llvm::raw_ostream&, clang::PrintingPolicy const&, bool) const /home/avi/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:309:50
 #12 0x000000000fb5594f clang::NestedNameSpecifier::print(llvm::raw_ostream&, clang::PrintingPolicy const&, bool) const /home/avi/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:0:18
 #13 0x000000000e722e19 (anonymous namespace)::FailedBooleanConditionPrinterHelper::handledStmt(clang::Stmt*, llvm::raw_ostream&) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:3414:29
 #14 0x000000000fc65831 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:148:18
 #15 0x000000000fc754b9 (anonymous namespace)::StmtPrinter::PrintExpr(clang::Expr*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:136:9
 #16 0x000000000fc6d71a (anonymous namespace)::StmtPrinter::VisitImplicitCastExpr(clang::ImplicitCastExpr*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:1779:1
 #17 0x000000000fc66609 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::StmtPrinter, void>::Visit(clang::Stmt*) /home/avi/llvm-project/build/tools/clang/include/clang/AST/StmtNodes.inc:534:1
 #18 0x000000000fc65846 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:151:5
 #19 0x000000000fc754b9 (anonymous namespace)::StmtPrinter::PrintExpr(clang::Expr*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:136:9
 #20 0x000000000fc6843c (anonymous namespace)::StmtPrinter::VisitUnaryOperator(clang::UnaryOperator*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:1544:7
 #21 0x000000000fc67f9a clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::StmtPrinter, void>::VisitUnaryLNot(clang::UnaryOperator*) /home/avi/llvm-project/clang/include/clang/AST/StmtVisitor.h:167:31
 #22 0x000000000fc65fc3 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::StmtPrinter, void>::Visit(clang::Stmt*) /home/avi/llvm-project/clang/include/clang/AST/StmtVisitor.h:98:26
 #23 0x000000000fc65846 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:151:5
 #24 0x000000000fc656ff clang::Stmt::printPretty(llvm::raw_ostream&, clang::PrinterHelper*, clang::PrintingPolicy const&, unsigned int, llvm::StringRef, clang::ASTContext const*) const /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:2912:5
 #25 0x000000000e6f9959 clang::Sema::findFailedBooleanCondition[abi:cxx11](clang::Expr*) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:3472:17
 #26 0x000000000e6fa508 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:3576:13
 #27 0x000000000ea450d9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildTemplateSpecializationType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:17181:18
 #28 0x000000000ea43e18 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:7346:18
 #29 0x000000000ea51333 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:7092:23
 #30 0x000000000e98904e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) /home/avi/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:77:1
 #31 0x000000000ea4cabf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:7453:34
 #32 0x000000000e988766 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) /home/avi/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:49:1
 #33 0x000000000e98797c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:5139:34
 #34 0x000000000ea1e9e6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:4870:8
 #35 0x000000000e991d24 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1664:7
 #36 0x000000000e99194a clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&, clang::MultiLevelTemplateArgumentList const&, clang::TemplateArgumentLoc&, clang::SourceLocation, clang::DeclarationName const&) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4499:23
 #37 0x000000000e705fa0 SubstDefaultTemplateArgument(clang::Sema&, clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateTypeParmDecl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, clang::TemplateArgumentLoc&) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:5012:9
 #38 0x000000000e7058f9 clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, bool&) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:5141:9
 #39 0x000000000e83b7c0 clang::TemplateDeductionResult ConvertDeducedTemplateArguments<clang::FunctionTemplateDecl>(clang::Sema&, clang::FunctionTemplateDecl*, bool, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, clang::Sema::CheckTemplateArgumentInfo&, clang::LocalInstantiationScope*, unsigned int, bool*) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3161:14
 #40 0x000000000e83a725 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool ()>) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4043:12
 #41 0x000000000e8ddfb1 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::$_2::operator()() const /home/avi/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4776:14
 #42 0x000000000e8ddea2 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::$_2>(long) /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #43 0x0000000006404b86 llvm::function_ref<void ()>::operator()() const /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #44 0x000000000969edd0 clang::runWithSufficientStackSpace(llvm::function_ref<void ()>, llvm::function_ref<void ()>) /home/avi/llvm-project/clang/include/clang/Basic/Stack.h:52:3
 #45 0x000000000969eb65 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /home/avi/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
 #46 0x000000000d8b577d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /home/avi/llvm-project/clang/lib/Sema/Sema.cpp:567:1
 #47 0x000000000e83d2bc clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4784:10
 #48 0x000000000e578992 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:7852:31
 #49 0x000000000e58e72e AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:13677:5
 #50 0x000000000e58e37a clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:13726:40
 #51 0x000000000e58eb96 clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:14033:3
 #52 0x000000000e58f193 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:14243:7
 #53 0x000000000de24024 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) /home/avi/llvm-project/clang/lib/Sema/SemaExpr.cpp:6562:16
 #54 0x000000000de3eb4e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) /home/avi/llvm-project/clang/lib/Sema/SemaExpr.cpp:6442:7
 #55 0x000000000d6d535d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:2270:23
 #56 0x000000000d6daa20 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:1963:9
 #57 0x000000000d6d7c21 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:1248:18
 #58 0x000000000d6d374c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:729:20
 #59 0x000000000d6d1bd3 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:184:20
 #60 0x000000000d6d1a6c clang::Parser::ParseExpression(clang::Parser::TypeCastState) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:135:18
 #61 0x000000000d7885f0 clang::Parser::ParseReturnStatement() /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:2511:11
 #62 0x000000000d7812da clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:351:11
 #63 0x000000000d78062b clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:125:20
 #64 0x000000000d78a1d0 clang::Parser::ParseCompoundStatementBody(bool) /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:1267:11
 #65 0x000000000d78ba88 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:2577:21
 #66 0x000000000d77d809 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) /home/avi/llvm-project/clang/lib/Parse/ParseCXXInlineMethods.cpp:0:3
 #67 0x000000000d77d2a6 clang::Parser::LexedMethod::ParseLexedMethodDefs() /home/avi/llvm-project/clang/lib/Parse/ParseCXXInlineMethods.cpp:321:1
 #68 0x000000000d77c0d3 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) /home/avi/llvm-project/clang/lib/Parse/ParseCXXInlineMethods.cpp:591:33
 #69 0x000000000d717f2d clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) /home/avi/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3985:5
 #70 0x000000000d716175 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) /home/avi/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:2346:7
 #71 0x000000000d7590d0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) /home/avi/llvm-project/clang/lib/Parse/ParseDecl.cpp:4709:23
 #72 0x000000000d689d22 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*) /home/avi/llvm-project/clang/include/clang/Parse/Parser.h:2515:5
 #73 0x000000000d681049 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:1153:7
 #74 0x000000000d680b0f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:1266:12
 #75 0x000000000d6802ba clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:1069:14
 #76 0x000000000d70d1ff clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /home/avi/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:264:7
 #77 0x000000000d70c9b1 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /home/avi/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:244:18
 #78 0x000000000d7505d1 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /home/avi/llvm-project/clang/lib/Parse/ParseDecl.cpp:2077:12
 #79 0x000000000d67fd3c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:985:14
 #80 0x000000000d67df0b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:758:12
 #81 0x000000000d677927 clang::ParseAST(clang::Sema&, bool, bool) /home/avi/llvm-project/clang/lib/Parse/ParseAST.cpp:171:16
 #82 0x0000000009ac6e34 clang::ASTFrontendAction::ExecuteAction() /home/avi/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1188:1
 #83 0x0000000009286326 clang::CodeGenAction::ExecuteAction() /home/avi/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1102:5
 #84 0x0000000009ac6786 clang::FrontendAction::Execute() /home/avi/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:7
 #85 0x00000000099ad38e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/avi/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:23
 #86 0x0000000009d0971d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/avi/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:296:8
 #87 0x00000000055d6438 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/avi/llvm-project/clang/tools/driver/cc1_main.cpp:290:13
 #88 0x00000000055c1a53 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /home/avi/llvm-project/clang/tools/driver/driver.cpp:218:5
 #89 0x00000000055c26fd clang_main(int, char**, llvm::ToolContext const&)::$_0::operator()(llvm::SmallVectorImpl<char const*>&) const /home/avi/llvm-project/clang/tools/driver/driver.cpp:364:16
 #90 0x00000000055c26aa int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_0>(long, llvm::SmallVectorImpl<char const*>&) /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
 #91 0x00000000097d326e llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(llvm::SmallVectorImpl<char const*>&) const /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
 #92 0x00000000097ce7a5 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0::operator()() const /home/avi/llvm-project/clang/lib/Driver/Job.cpp:437:34
 #93 0x00000000097ce752 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #94 0x0000000006404b86 llvm::function_ref<void ()>::operator()() const /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #95 0x0000000007d523ec llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /home/avi/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:3
 #96 0x00000000097ce0c3 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /home/avi/llvm-project/clang/lib/Driver/Job.cpp:437:7
 #97 0x000000000973b65e clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /home/avi/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
 #98 0x000000000973b87c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /home/avi/llvm-project/clang/lib/Driver/Compilation.cpp:251:13
 #99 0x000000000975c941 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /home/avi/llvm-project/clang/lib/Driver/Driver.cpp:2190:7
#100 0x00000000055c151e clang_main(int, char**, llvm::ToolContext const&) /home/avi/llvm-project/clang/tools/driver/driver.cpp:402:9
#101 0x000000000561132a main /home/avi/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:17:10
#102 0x00007fce0c2105f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#103 0x00007fce0c2106a8 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#104 0x00000000055c01e5 _start (/home/avi/clang.bin/bin/clang+++0x55c01e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 20.1.3 (https://github.com/llvm/llvm-project.git e0db588f3db40b1486e215c5d0b72a8f9ea2b018)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/avi/clang.bin/bin
Build config: +unoptimized, +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/CqlParser-49b09c.cpp
clang++: note: diagnostic msg: /tmp/CqlParser-49b09c.sh
clang++: note: diagnostic msg: 

********************
```

<!-- Failed to upload "source-and-runscript.tgz" -->


## Comments

### Comment 1 - avikivity

Seen with current 20.x (e0db588f3db4)

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Avi Kivity (avikivity)

<details>
```
/home/avi/clang.bin/bin/clang++ -MD -MT build/release/gen/cql3/CqlParser.o -MF build/release/gen/cql3/CqlParser.o.d -std=gnu++23 -I/home/avi/scylla/seastar/include -I/home/avi/scylla/build/release/seastar/gen/include -Werror=unused-result -DSEASTAR_API_LEVEL=7 -DSEASTAR_SSTRING -DSEASTAR_LOGGER_COMPILE_TIME_FMT -DSEASTAR_SCHEDULING_GROUPS_COUNT=19 -DSEASTAR_LOGGER_TYPE_STDOUT -DBOOST_PROGRAM_OPTIONS_NO_LIB -DBOOST_PROGRAM_OPTIONS_DYN_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_DYN_LINK -DFMT_SHARED -I/usr/include/p11-kit-1 -ffile-prefix-map=/home/avi/scylla=. -DSEASTAR_NO_EXCEPTION_HACK -march=westmere -Wno-backend-plugin -flto=thin -ffat-lto-objects -fprofile-use=/home/avi/scylla/build/profile.profdata -mllvm -pgso=false -mllvm -enable-value-profiling=false -ffunction-sections -fdata-sections  -O3 -mllvm -inline-threshold=2500 -fno-slp-vectorize -DSCYLLA_BUILD_MODE=release -g -gz -Xclang -fexperimental-assignment-tracking=disabled -iquote. -iquote build/release/gen -std=gnu++23  -ffile-prefix-map=/home/avi/scylla=. -DSEASTAR_NO_EXCEPTION_HACK -march=westmere -DBOOST_ALL_DYN_LINK    -fvisibility=hidden -Ikmipc/kmipc-2.1.0t-rhel84_64/include -DHAVE_KMIP -isystem abseil -Wall -Werror -Wextra -Wimplicit-fallthrough -Wno-mismatched-tags -Wno-c++11-narrowing -Wno-overloaded-virtual -Wno-unused-parameter -Wno-unsupported-friend -Wno-missing-field-initializers -Wno-deprecated-copy -Wno-error=deprecated-declarations -DXXH_PRIVATE_API -DSEASTAR_TESTING_MAIN -Wno-parentheses-equality -c -o build/release/gen/cql3/CqlParser.o build/release/gen/cql3/CqlParser.cpp
clang++: /home/avi/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:307: void clang::NestedNameSpecifier::print(raw_ostream &amp;, const PrintingPolicy &amp;, bool) const: Assertion `!isa&lt;ElaboratedType&gt;(T) &amp;&amp; "Elaborated type in nested-name-specifier"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/avi/clang.bin/bin/clang++ -MD -MT build/release/gen/cql3/CqlParser.o -MF build/release/gen/cql3/CqlParser.o.d -std=gnu++23 -I/home/avi/scylla/seastar/include -I/home/avi/scylla/build/release/seastar/gen/include -Werror=unused-result -DSEASTAR_API_LEVEL=7 -DSEASTAR_SSTRING -DSEASTAR_LOGGER_COMPILE_TIME_FMT -DSEASTAR_SCHEDULING_GROUPS_COUNT=19 -DSEASTAR_LOGGER_TYPE_STDOUT -DBOOST_PROGRAM_OPTIONS_NO_LIB -DBOOST_PROGRAM_OPTIONS_DYN_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_DYN_LINK -DFMT_SHARED -I/usr/include/p11-kit-1 -ffile-prefix-map=/home/avi/scylla=. -DSEASTAR_NO_EXCEPTION_HACK -march=westmere -Wno-backend-plugin -flto=thin -ffat-lto-objects -fprofile-use=/home/avi/scylla/build/profile.profdata -mllvm -pgso=false -mllvm -enable-value-profiling=false -ffunction-sections -fdata-sections -O3 -mllvm -inline-threshold=2500 -fno-slp-vectorize -DSCYLLA_BUILD_MODE=release -g -gz -Xclang -fexperimental-assignment-tracking=disabled -iquote. -iquote build/release/gen -std=gnu++23 -ffile-prefix-map=/home/avi/scylla=. -DSEASTAR_NO_EXCEPTION_HACK -march=westmere -DBOOST_ALL_DYN_LINK -fvisibility=hidden -Ikmipc/kmipc-2.1.0t-rhel84_64/include -DHAVE_KMIP -isystem abseil -Wall -Werror -Wextra -Wimplicit-fallthrough -Wno-mismatched-tags -Wno-c++11-narrowing -Wno-overloaded-virtual -Wno-unused-parameter -Wno-unsupported-friend -Wno-missing-field-initializers -Wno-deprecated-copy -Wno-error=deprecated-declarations -DXXH_PRIVATE_API -DSEASTAR_TESTING_MAIN -Wno-parentheses-equality -c -o build/release/gen/cql3/CqlParser.o build/release/gen/cql3/CqlParser.cpp
1.	build/release/gen/idl/query.dist.impl.hh:40:8: current parser token ')'
2.	build/release/gen/idl/query.dist.impl.hh:17:1: parsing namespace 'ser'
3.	build/release/gen/idl/query.dist.impl.hh:30:1: parsing struct/union/class body 'ser::qr_cell_view'
4.	build/release/gen/idl/query.dist.impl.hh:34:28: parsing function body 'ser::qr_cell_view::timestamp'
5.	build/release/gen/idl/query.dist.impl.hh:34:28: in compound statement ('{}')
  #<!-- -->0 0x0000000007e7b159 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/avi/llvm-project/llvm/lib/Support/Unix/Signals.inc:799:11
  #<!-- -->1 0x0000000007e7b738 PrintStackTraceSignalHandler(void*) /home/avi/llvm-project/llvm/lib/Support/Unix/Signals.inc:875:1
  #<!-- -->2 0x0000000007e7934c llvm::sys::RunSignalHandlers() /home/avi/llvm-project/llvm/lib/Support/Signals.cpp:105:5
  #<!-- -->3 0x0000000007e7a806 llvm::sys::CleanupOnSignal(unsigned long) /home/avi/llvm-project/llvm/lib/Support/Unix/Signals.inc:370:1
  #<!-- -->4 0x0000000007d5262f (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) /home/avi/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:0:7
  #<!-- -->5 0x0000000007d52af1 CrashRecoverySignalHandler(int) /home/avi/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:391:1
  #<!-- -->6 0x00007fce0c226df0 __restore_rt (/lib64/libc.so.6+0x19df0)
  #<!-- -->7 0x00007fce0c28027c __pthread_kill_implementation (/lib64/libc.so.6+0x7327c)
  #<!-- -->8 0x00007fce0c226cbe gsignal (/lib64/libc.so.6+0x19cbe)
  #<!-- -->9 0x00007fce0c20e6d6 abort (/lib64/libc.so.6+0x16d6)
 #<!-- -->10 0x00007fce0c20e639 __assert_perror_fail (/lib64/libc.so.6+0x1639)
 #<!-- -->11 0x000000000fb55bee clang::NestedNameSpecifier::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, bool) const /home/avi/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:309:50
 #<!-- -->12 0x000000000fb5594f clang::NestedNameSpecifier::print(llvm::raw_ostream&amp;, clang::PrintingPolicy const&amp;, bool) const /home/avi/llvm-project/clang/lib/AST/NestedNameSpecifier.cpp:0:18
 #<!-- -->13 0x000000000e722e19 (anonymous namespace)::FailedBooleanConditionPrinterHelper::handledStmt(clang::Stmt*, llvm::raw_ostream&amp;) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:3414:29
 #<!-- -->14 0x000000000fc65831 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:148:18
 #<!-- -->15 0x000000000fc754b9 (anonymous namespace)::StmtPrinter::PrintExpr(clang::Expr*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:136:9
 #<!-- -->16 0x000000000fc6d71a (anonymous namespace)::StmtPrinter::VisitImplicitCastExpr(clang::ImplicitCastExpr*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:1779:1
 #<!-- -->17 0x000000000fc66609 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::StmtPrinter, void&gt;::Visit(clang::Stmt*) /home/avi/llvm-project/build/tools/clang/include/clang/AST/StmtNodes.inc:534:1
 #<!-- -->18 0x000000000fc65846 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:151:5
 #<!-- -->19 0x000000000fc754b9 (anonymous namespace)::StmtPrinter::PrintExpr(clang::Expr*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:136:9
 #<!-- -->20 0x000000000fc6843c (anonymous namespace)::StmtPrinter::VisitUnaryOperator(clang::UnaryOperator*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:1544:7
 #<!-- -->21 0x000000000fc67f9a clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::StmtPrinter, void&gt;::VisitUnaryLNot(clang::UnaryOperator*) /home/avi/llvm-project/clang/include/clang/AST/StmtVisitor.h:167:31
 #<!-- -->22 0x000000000fc65fc3 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::StmtPrinter, void&gt;::Visit(clang::Stmt*) /home/avi/llvm-project/clang/include/clang/AST/StmtVisitor.h:98:26
 #<!-- -->23 0x000000000fc65846 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:151:5
 #<!-- -->24 0x000000000fc656ff clang::Stmt::printPretty(llvm::raw_ostream&amp;, clang::PrinterHelper*, clang::PrintingPolicy const&amp;, unsigned int, llvm::StringRef, clang::ASTContext const*) const /home/avi/llvm-project/clang/lib/AST/StmtPrinter.cpp:2912:5
 #<!-- -->25 0x000000000e6f9959 clang::Sema::findFailedBooleanCondition[abi:cxx11](clang::Expr*) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:3472:17
 #<!-- -->26 0x000000000e6fa508 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:3576:13
 #<!-- -->27 0x000000000ea450d9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildTemplateSpecializationType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:17181:18
 #<!-- -->28 0x000000000ea43e18 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:7346:18
 #<!-- -->29 0x000000000ea51333 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:7092:23
 #<!-- -->30 0x000000000e98904e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) /home/avi/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:77:1
 #<!-- -->31 0x000000000ea4cabf clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformElaboratedType(clang::TypeLocBuilder&amp;, clang::ElaboratedTypeLoc) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:7453:34
 #<!-- -->32 0x000000000e988766 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) /home/avi/llvm-project/build/tools/clang/include/clang/AST/TypeNodes.inc:49:1
 #<!-- -->33 0x000000000e98797c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:5139:34
 #<!-- -->34 0x000000000ea1e9e6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) /home/avi/llvm-project/clang/lib/Sema/TreeTransform.h:4870:8
 #<!-- -->35 0x000000000e991d24 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1664:7
 #<!-- -->36 0x000000000e99194a clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateArgumentLoc&amp;, clang::SourceLocation, clang::DeclarationName const&amp;) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4499:23
 #<!-- -->37 0x000000000e705fa0 SubstDefaultTemplateArgument(clang::Sema&amp;, clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateTypeParmDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::TemplateArgumentLoc&amp;) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:5012:9
 #<!-- -->38 0x000000000e7058f9 clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool&amp;) /home/avi/llvm-project/clang/lib/Sema/SemaTemplate.cpp:5141:9
 #<!-- -->39 0x000000000e83b7c0 clang::TemplateDeductionResult ConvertDeducedTemplateArguments&lt;clang::FunctionTemplateDecl&gt;(clang::Sema&amp;, clang::FunctionTemplateDecl*, bool, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, clang::Sema::CheckTemplateArgumentInfo&amp;, clang::LocalInstantiationScope*, unsigned int, bool*) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:3161:14
 #<!-- -->40 0x000000000e83a725 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, llvm::function_ref&lt;bool ()&gt;) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4043:12
 #<!-- -->41 0x000000000e8ddfb1 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::$_2::operator()() const /home/avi/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4776:14
 #<!-- -->42 0x000000000e8ddea2 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::$_2&gt;(long) /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #<!-- -->43 0x0000000006404b86 llvm::function_ref&lt;void ()&gt;::operator()() const /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #<!-- -->44 0x000000000969edd0 clang::runWithSufficientStackSpace(llvm::function_ref&lt;void ()&gt;, llvm::function_ref&lt;void ()&gt;) /home/avi/llvm-project/clang/include/clang/Basic/Stack.h:52:3
 #<!-- -->45 0x000000000969eb65 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /home/avi/llvm-project/clang/lib/Basic/StackExhaustionHandler.cpp:21:1
 #<!-- -->46 0x000000000d8b577d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /home/avi/llvm-project/clang/lib/Sema/Sema.cpp:567:1
 #<!-- -->47 0x000000000e83d2bc clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) /home/avi/llvm-project/clang/lib/Sema/SemaTemplateDeduction.cpp:4784:10
 #<!-- -->48 0x000000000e578992 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:7852:31
 #<!-- -->49 0x000000000e58e72e AddOverloadedCallCandidate(clang::Sema&amp;, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:13677:5
 #<!-- -->50 0x000000000e58e37a clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:13726:40
 #<!-- -->51 0x000000000e58eb96 clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:14033:3
 #<!-- -->52 0x000000000e58f193 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) /home/avi/llvm-project/clang/lib/Sema/SemaOverload.cpp:14243:7
 #<!-- -->53 0x000000000de24024 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) /home/avi/llvm-project/clang/lib/Sema/SemaExpr.cpp:6562:16
 #<!-- -->54 0x000000000de3eb4e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) /home/avi/llvm-project/clang/lib/Sema/SemaExpr.cpp:6442:7
 #<!-- -->55 0x000000000d6d535d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:2270:23
 #<!-- -->56 0x000000000d6daa20 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:1963:9
 #<!-- -->57 0x000000000d6d7c21 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:1248:18
 #<!-- -->58 0x000000000d6d374c clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:729:20
 #<!-- -->59 0x000000000d6d1bd3 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:184:20
 #<!-- -->60 0x000000000d6d1a6c clang::Parser::ParseExpression(clang::Parser::TypeCastState) /home/avi/llvm-project/clang/lib/Parse/ParseExpr.cpp:135:18
 #<!-- -->61 0x000000000d7885f0 clang::Parser::ParseReturnStatement() /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:2511:11
 #<!-- -->62 0x000000000d7812da clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:351:11
 #<!-- -->63 0x000000000d78062b clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:125:20
 #<!-- -->64 0x000000000d78a1d0 clang::Parser::ParseCompoundStatementBody(bool) /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:1267:11
 #<!-- -->65 0x000000000d78ba88 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /home/avi/llvm-project/clang/lib/Parse/ParseStmt.cpp:2577:21
 #<!-- -->66 0x000000000d77d809 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) /home/avi/llvm-project/clang/lib/Parse/ParseCXXInlineMethods.cpp:0:3
 #<!-- -->67 0x000000000d77d2a6 clang::Parser::LexedMethod::ParseLexedMethodDefs() /home/avi/llvm-project/clang/lib/Parse/ParseCXXInlineMethods.cpp:321:1
 #<!-- -->68 0x000000000d77c0d3 clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) /home/avi/llvm-project/clang/lib/Parse/ParseCXXInlineMethods.cpp:591:33
 #<!-- -->69 0x000000000d717f2d clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) /home/avi/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:3985:5
 #<!-- -->70 0x000000000d716175 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) /home/avi/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:2346:7
 #<!-- -->71 0x000000000d7590d0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) /home/avi/llvm-project/clang/lib/Parse/ParseDecl.cpp:4709:23
 #<!-- -->72 0x000000000d689d22 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*) /home/avi/llvm-project/clang/include/clang/Parse/Parser.h:2515:5
 #<!-- -->73 0x000000000d681049 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:1153:7
 #<!-- -->74 0x000000000d680b0f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:1266:12
 #<!-- -->75 0x000000000d6802ba clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:1069:14
 #<!-- -->76 0x000000000d70d1ff clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /home/avi/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:264:7
 #<!-- -->77 0x000000000d70c9b1 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /home/avi/llvm-project/clang/lib/Parse/ParseDeclCXX.cpp:244:18
 #<!-- -->78 0x000000000d7505d1 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /home/avi/llvm-project/clang/lib/Parse/ParseDecl.cpp:2077:12
 #<!-- -->79 0x000000000d67fd3c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:985:14
 #<!-- -->80 0x000000000d67df0b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /home/avi/llvm-project/clang/lib/Parse/Parser.cpp:758:12
 #<!-- -->81 0x000000000d677927 clang::ParseAST(clang::Sema&amp;, bool, bool) /home/avi/llvm-project/clang/lib/Parse/ParseAST.cpp:171:16
 #<!-- -->82 0x0000000009ac6e34 clang::ASTFrontendAction::ExecuteAction() /home/avi/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1188:1
 #<!-- -->83 0x0000000009286326 clang::CodeGenAction::ExecuteAction() /home/avi/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1102:5
 #<!-- -->84 0x0000000009ac6786 clang::FrontendAction::Execute() /home/avi/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:7
 #<!-- -->85 0x00000000099ad38e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/avi/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:23
 #<!-- -->86 0x0000000009d0971d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/avi/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:296:8
 #<!-- -->87 0x00000000055d6438 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /home/avi/llvm-project/clang/tools/driver/cc1_main.cpp:290:13
 #<!-- -->88 0x00000000055c1a53 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /home/avi/llvm-project/clang/tools/driver/driver.cpp:218:5
 #<!-- -->89 0x00000000055c26fd clang_main(int, char**, llvm::ToolContext const&amp;)::$_0::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /home/avi/llvm-project/clang/tools/driver/driver.cpp:364:16
 #<!-- -->90 0x00000000055c26aa int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_0&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
 #<!-- -->91 0x00000000097d326e llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(llvm::SmallVectorImpl&lt;char const*&gt;&amp;) const /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
 #<!-- -->92 0x00000000097ce7a5 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0::operator()() const /home/avi/llvm-project/clang/lib/Driver/Job.cpp:437:34
 #<!-- -->93 0x00000000097ce752 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
 #<!-- -->94 0x0000000006404b86 llvm::function_ref&lt;void ()&gt;::operator()() const /home/avi/llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
 #<!-- -->95 0x0000000007d523ec llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /home/avi/llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:3
 #<!-- -->96 0x00000000097ce0c3 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /home/avi/llvm-project/clang/lib/Driver/Job.cpp:437:7
 #<!-- -->97 0x000000000973b65e clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /home/avi/llvm-project/clang/lib/Driver/Compilation.cpp:196:15
 #<!-- -->98 0x000000000973b87c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /home/avi/llvm-project/clang/lib/Driver/Compilation.cpp:251:13
 #<!-- -->99 0x000000000975c941 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /home/avi/llvm-project/clang/lib/Driver/Driver.cpp:2190:7
#<!-- -->100 0x00000000055c151e clang_main(int, char**, llvm::ToolContext const&amp;) /home/avi/llvm-project/clang/tools/driver/driver.cpp:402:9
#<!-- -->101 0x000000000561132a main /home/avi/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:17:10
#<!-- -->102 0x00007fce0c2105f5 __libc_start_call_main (/lib64/libc.so.6+0x35f5)
#<!-- -->103 0x00007fce0c2106a8 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x36a8)
#<!-- -->104 0x00000000055c01e5 _start (/home/avi/clang.bin/bin/clang+++0x55c01e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 20.1.3 (https://github.com/llvm/llvm-project.git e0db588f3db40b1486e215c5d0b72a8f9ea2b018)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/avi/clang.bin/bin
Build config: +unoptimized, +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/CqlParser-49b09c.cpp
clang++: note: diagnostic msg: /tmp/CqlParser-49b09c.sh
clang++: note: diagnostic msg: 

********************
```

&lt;!-- Failed to upload "source-and-runscript.tgz" --&gt;

</details>


---

### Comment 3 - avikivity

This is the code in question:

```c++
struct qr_cell_view {
    utils::input_stream v;


    auto timestamp() const {
      return seastar::with_serialized_stream(v, [this] (auto& v) -> decltype(deserialize(std::declval<utils::input_stream&>(), std::type_identity<std::optional<api::timestamp_type>>())) {
       std::ignore = this;
       auto in = v;
       ser::skip(in, std::type_identity<size_type>());
       return deserialize(in, std::type_identity<std::optional<api::timestamp_type>>());
      });
    }
```

I'd guess that decltype somehow returns an elaborated-type which then confuses the rest.

---

### Comment 4 - avikivity

This was a debug mode build (left over from a previous bug investigation), I'll check if it triggers in release too.

---

### Comment 5 - avikivity

It does not trigger in a release build.

---

### Comment 6 - shafik

Do you have a minimal reproducer that you can share? 

---

