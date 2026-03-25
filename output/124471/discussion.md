# clang++ 19.1.7 segfault in isPackExpansion()

**Author:** jehelset
**URL:** https://github.com/llvm/llvm-project/issues/124471

## Body

Attached auto-generated reproducer, stack-dump in eval-fc3c48.dump.

[eval-fc3c48.tar.gz](https://github.com/user-attachments/files/18550662/eval-fc3c48.tar.gz)

Top of the backtrace:

```console
  #0 0x000076452301a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
  #1 0x00007645230176bd llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
  #2 0x00007645230176bd SignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:403:31
  #3 0x000076452224c1d0 (/usr/lib/libc.so.6+0x3d1d0)
  #4 0x000076452dd102bb clang::TemplateArgument::isPackExpansion() const /usr/src/debug/clang/clang-19.1.7.src/lib/AST/TemplateBase.cpp:323:0
  #5 0x000076452dd102bb getPackSubstitutedTemplateArgument(clang::Sema&, clang::TemplateArgument) (.isra.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1805:0
  #6 0x000076452c66edbf TransformTemplateTypeParmType /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2515:0
  #7 0x000076452c6a30fa TransformReferenceType /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:5371:0
  #8 0x000076452c676f13 clang::Sema::SubstType(clang::TypeLoc, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2833:0
  #9 0x000076452c677146 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, std::optional<unsigned int>, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:3080:0
 #10 0x000076452c677db7 TransformFunctionTypeParam /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2423:0
 #11 0x000076452c677db7 TransformFunctionTypeParams /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:6011:0
 #12 0x000076452db287eb clang::QualType clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*)::'lambda'(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::operator()(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc) const::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*)::'lambda'(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::operator()(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc) const::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:6217:0
 #13 0x000076452c69ba9a llvm::SmallVectorTemplateCommon<clang::QualType, void>::isSmall() const /usr/include/llvm/ADT/SmallVector.h:157:0
 #14 0x000076452c69ba9a llvm::SmallVectorImpl<clang::QualType>::~SmallVectorImpl() /usr/include/llvm/ADT/SmallVector.h:617:0
 #15 0x000076452c69ba9a llvm::SmallVector<clang::QualType, 4u>::~SmallVector() /usr/include/llvm/ADT/SmallVector.h:1217:0
 #16 0x000076452c69ba9a operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:14573:0
 #17 0x000076452c69ba9a TransformLambdaExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:14587:0
 #18 0x000076452c67b24a TransformLambdaExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1662:0
 #19 0x000076452c67b24a TransformExpr /usr/src/debug/clang/clang-19.1.7.src/build/include/clang/AST/StmtNodes.inc:340:0
 #20 0x000076452c682ea0 TransformExprs /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4346:0
```

## Comments

### Comment 1 - Endilll

Confirmed on Clang 21.0 (trunk): https://godbolt.org/z/P8WhGEWjb
This one is fun: up until Clang 17 we used to reject this like everyone else (https://godbolt.org/z/s9j7bfYEP), then in Clang 18 we started accepting it, then since 19 we crash.
Reduced by C-Reduce:
```cpp
auto argids(auto s, auto t)
  requires requires { [](auto...) {}; }
{
  argids(0, 0);
}
```
Clang 21.0 crash:
```
clang++: /root/llvm-project/clang/include/clang/AST/TemplateBase.h:445:
llvm::ArrayRef<clang::TemplateArgument> clang::TemplateArgument::getPackAsArray() const:
Assertion `getKind() == Pack' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:4:14: current parser token ')'
2.	<source>:3:1: parsing function body 'argids'
3.	<source>:3:1: in compound statement ('{}')
 #0 0x0000000003e836b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e836b8)
 #1 0x0000000003e81344 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e81344)
 #2 0x0000000003dc61d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fc646642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fc6466969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007fc646642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007fc6466287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007fc64662871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007fc646639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007107471 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, clang::MultiLevelTemplateArgumentList const&, bool&, bool&, std::optional<unsigned int>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7107471)
#10 0x0000000007030c32 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000007031b66 clang::QualType clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000007032bdd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007039fbe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000703bcd6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000007048145 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000703b9d6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000007064402 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7064402)
#18 0x0000000006787d2c clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#19 0x0000000006789a7e CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#20 0x000000000678a0dd clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x678a0dd)
#21 0x000000000678b128 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x678b128)
#22 0x0000000006fabe35 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#23 0x0000000006fac277 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#24 0x0000000007d762e1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d762e1)
#25 0x0000000006f84705 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f84705)
#26 0x0000000006dc2f6c clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#27 0x0000000006dc373b AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) SemaOverload.cpp:0:0
#28 0x0000000006dc3a51 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dc3a51)
#29 0x0000000006dc3d4e clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dc3d4e)
#30 0x0000000006dd057d clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd057d)
#31 0x00000000069e23a6 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e23a6)
#32 0x00000000069e321c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e321c)
#33 0x0000000006520912 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6520912)
#34 0x0000000006519101 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6519101)
#35 0x000000000651b267 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651b267)
#36 0x000000000651b2f9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651b2f9)
#37 0x000000000651fe59 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651fe59)
#38 0x00000000065a4099 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65a4099)
#39 0x000000000659a2b4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659a2b4)
#40 0x000000000659b240 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659b240)
#41 0x000000000659c141 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659c141)
#42 0x000000000659d8da clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659d8da)
#43 0x00000000064ae7a3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ae7a3)
#44 0x00000000064e36dd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e36dd)
#45 0x00000000064a25fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a25fe)
#46 0x00000000064a2db9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64a2db9)
#47 0x00000000064aa61a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64aa61a)
#48 0x00000000064ab58d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ab58d)
#49 0x00000000064aba40 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64aba40)
#50 0x000000000649dad3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x649dad3)
#51 0x000000000483eeb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483eeb8)
#52 0x0000000004b0e995 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b0e995)
#53 0x0000000004a90ade clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a90ade)
#54 0x0000000004bfe6be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfe6be)
#55 0x0000000000d4f70f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4f70f)
#56 0x0000000000d46eca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#57 0x0000000004887419 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#58 0x0000000003dc6674 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3dc6674)
#59 0x0000000004887a2f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#60 0x000000000484a49d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484a49d)
#61 0x000000000484b52e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484b52e)
#62 0x0000000004852c05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4852c05)
#63 0x0000000000d4c508 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4c508)
#64 0x0000000000c13de4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc13de4)
#65 0x00007fc646629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#66 0x00007fc646629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#67 0x0000000000d46975 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd46975)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (jehelset)

<details>
Attached auto-generated reproducer, stack-dump in eval-fc3c48.dump.

[eval-fc3c48.tar.gz](https://github.com/user-attachments/files/18550662/eval-fc3c48.tar.gz)

Top of the backtrace:

```console
  #<!-- -->0 0x000076452301a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
  #<!-- -->1 0x00007645230176bd llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
  #<!-- -->2 0x00007645230176bd SignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:403:31
  #<!-- -->3 0x000076452224c1d0 (/usr/lib/libc.so.6+0x3d1d0)
  #<!-- -->4 0x000076452dd102bb clang::TemplateArgument::isPackExpansion() const /usr/src/debug/clang/clang-19.1.7.src/lib/AST/TemplateBase.cpp:323:0
  #<!-- -->5 0x000076452dd102bb getPackSubstitutedTemplateArgument(clang::Sema&amp;, clang::TemplateArgument) (.isra.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1805:0
  #<!-- -->6 0x000076452c66edbf TransformTemplateTypeParmType /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2515:0
  #<!-- -->7 0x000076452c6a30fa TransformReferenceType /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:5371:0
  #<!-- -->8 0x000076452c676f13 clang::Sema::SubstType(clang::TypeLoc, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2833:0
  #<!-- -->9 0x000076452c677146 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, int, std::optional&lt;unsigned int&gt;, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:3080:0
 #<!-- -->10 0x000076452c677db7 TransformFunctionTypeParam /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2423:0
 #<!-- -->11 0x000076452c677db7 TransformFunctionTypeParams /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:6011:0
 #<!-- -->12 0x000076452db287eb clang::QualType clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionProtoType&lt;clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*)::'lambda'(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc)::operator()(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc) const::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&amp;, bool&amp;)&gt;(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*)::'lambda'(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc)::operator()(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc) const::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&amp;, bool&amp;)) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:6217:0
 #<!-- -->13 0x000076452c69ba9a llvm::SmallVectorTemplateCommon&lt;clang::QualType, void&gt;::isSmall() const /usr/include/llvm/ADT/SmallVector.h:157:0
 #<!-- -->14 0x000076452c69ba9a llvm::SmallVectorImpl&lt;clang::QualType&gt;::~SmallVectorImpl() /usr/include/llvm/ADT/SmallVector.h:617:0
 #<!-- -->15 0x000076452c69ba9a llvm::SmallVector&lt;clang::QualType, 4u&gt;::~SmallVector() /usr/include/llvm/ADT/SmallVector.h:1217:0
 #<!-- -->16 0x000076452c69ba9a operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:14573:0
 #<!-- -->17 0x000076452c69ba9a TransformLambdaExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:14587:0
 #<!-- -->18 0x000076452c67b24a TransformLambdaExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1662:0
 #<!-- -->19 0x000076452c67b24a TransformExpr /usr/src/debug/clang/clang-19.1.7.src/build/include/clang/AST/StmtNodes.inc:340:0
 #<!-- -->20 0x000076452c682ea0 TransformExprs /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4346:0
```
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (jehelset)

<details>
Attached auto-generated reproducer, stack-dump in eval-fc3c48.dump.

[eval-fc3c48.tar.gz](https://github.com/user-attachments/files/18550662/eval-fc3c48.tar.gz)

Top of the backtrace:

```console
  #<!-- -->0 0x000076452301a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
  #<!-- -->1 0x00007645230176bd llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
  #<!-- -->2 0x00007645230176bd SignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:403:31
  #<!-- -->3 0x000076452224c1d0 (/usr/lib/libc.so.6+0x3d1d0)
  #<!-- -->4 0x000076452dd102bb clang::TemplateArgument::isPackExpansion() const /usr/src/debug/clang/clang-19.1.7.src/lib/AST/TemplateBase.cpp:323:0
  #<!-- -->5 0x000076452dd102bb getPackSubstitutedTemplateArgument(clang::Sema&amp;, clang::TemplateArgument) (.isra.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1805:0
  #<!-- -->6 0x000076452c66edbf TransformTemplateTypeParmType /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2515:0
  #<!-- -->7 0x000076452c6a30fa TransformReferenceType /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:5371:0
  #<!-- -->8 0x000076452c676f13 clang::Sema::SubstType(clang::TypeLoc, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2833:0
  #<!-- -->9 0x000076452c677146 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, int, std::optional&lt;unsigned int&gt;, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:3080:0
 #<!-- -->10 0x000076452c677db7 TransformFunctionTypeParam /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2423:0
 #<!-- -->11 0x000076452c677db7 TransformFunctionTypeParams /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:6011:0
 #<!-- -->12 0x000076452db287eb clang::QualType clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionProtoType&lt;clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*)::'lambda'(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc)::operator()(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc) const::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&amp;, bool&amp;)&gt;(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*)::'lambda'(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc)::operator()(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc) const::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&amp;, bool&amp;)) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:6217:0
 #<!-- -->13 0x000076452c69ba9a llvm::SmallVectorTemplateCommon&lt;clang::QualType, void&gt;::isSmall() const /usr/include/llvm/ADT/SmallVector.h:157:0
 #<!-- -->14 0x000076452c69ba9a llvm::SmallVectorImpl&lt;clang::QualType&gt;::~SmallVectorImpl() /usr/include/llvm/ADT/SmallVector.h:617:0
 #<!-- -->15 0x000076452c69ba9a llvm::SmallVector&lt;clang::QualType, 4u&gt;::~SmallVector() /usr/include/llvm/ADT/SmallVector.h:1217:0
 #<!-- -->16 0x000076452c69ba9a operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:14573:0
 #<!-- -->17 0x000076452c69ba9a TransformLambdaExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:14587:0
 #<!-- -->18 0x000076452c67b24a TransformLambdaExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1662:0
 #<!-- -->19 0x000076452c67b24a TransformExpr /usr/src/debug/clang/clang-19.1.7.src/build/include/clang/AST/StmtNodes.inc:340:0
 #<!-- -->20 0x000076452c682ea0 TransformExprs /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4346:0
```
</details>


---

### Comment 4 - shafik

@zyn0217 git bisect points to 3d361b225fe89ce1d8c93639f27d689082bd8dad as the cause for this regression

---

