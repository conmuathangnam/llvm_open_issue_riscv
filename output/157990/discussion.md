# ICE on requires clause in IILE in requires clause of member function in class template

**Author:** Eczbek
**URL:** https://github.com/llvm/llvm-project/issues/157990

## Body

https://godbolt.org/z/8hbjx1ePe

```cpp
template<int = 0>
struct S {
	void f(auto... a) requires(requires { [](auto) requires(!sizeof...(a)) {}(0); }) {}
};

int main() { S().f(); }
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:6:20: current parser token ')'
2.	<source>:6:12: parsing function body 'main'
3.	<source>:6:12: in compound statement ('{}')
 #0 0x0000000003c43ef8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c43ef8)
 #1 0x0000000003c418cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c418cc)
 #2 0x0000000003b917b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071789d442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006ffc9f9 clang::ASTContext::getSubstTemplateTypeParmType(clang::QualType, clang::Decl*, unsigned int, clang::UnsignedOrNone, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ffc9f9)
 #5 0x0000000006c7f50c (anonymous namespace)::TemplateInstantiator::BuildSubstTemplateTypeParmType(clang::TypeLocBuilder&, bool, bool, clang::Decl*, unsigned int, clang::UnsignedOrNone, clang::TemplateArgument, clang::SourceLocation) SemaTemplateInstantiate.cpp:0:0
 #6 0x0000000006cac7a1 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
 #7 0x0000000006c9ad94 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #8 0x0000000006c9c8b7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #9 0x0000000006ca004f clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ca004f)
#10 0x0000000006ca7dac clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ca7dac)
#11 0x0000000006cb9f14 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006c98cec clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000006c9a29d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000006cd6aea clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000006c92b08 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000006cb39d0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000006c921aa clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000006c90607 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000006c9257e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#20 0x0000000006c98765 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c98765)
#21 0x0000000006482235 calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#22 0x0000000006484275 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#23 0x0000000006487387 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (.constprop.0) SemaConcept.cpp:0:0
#24 0x00000000064883d8 clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64883d8)
#25 0x0000000006c36800 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c36800)
#26 0x0000000006c3780a void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#27 0x000000000795b581 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x795b581)
#28 0x0000000006c0478b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c0478b)
#29 0x0000000006a615cd AddMethodTemplateCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#30 0x0000000006a640c3 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a640c3)
#31 0x0000000006a64174 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a64174)
#32 0x0000000006a7a094 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a7a094)
#33 0x00000000066f2f7d clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66f2f7d)
#34 0x00000000066f7aac clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66f7aac)
#35 0x000000000620bbbe clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620bbbe)
#36 0x0000000006203afa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6203afa)
#37 0x0000000006205d9a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6205d9a)
#38 0x0000000006205f89 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6205f89)
#39 0x000000000620b089 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620b089)
#40 0x000000000629a977 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x629a977)
#41 0x0000000006291392 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6291392)
#42 0x0000000006291d89 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6291d89)
#43 0x000000000629b840 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x629b840)
#44 0x000000000629c27a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x629c27a)
#45 0x0000000006185e13 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6185e13)
#46 0x00000000061d27ef clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61d27ef)
#47 0x000000000617e8ff clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617e8ff)
#48 0x000000000617f6f7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617f6f7)
#49 0x000000000618834f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618834f)
#50 0x0000000006189b57 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6189b57)
#51 0x000000000616260a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x616260a)
#52 0x000000000450c4ad clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450c4ad)
#53 0x0000000004812ffa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4812ffa)
#54 0x000000000478d23b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x478d23b)
#55 0x0000000004907693 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4907693)
#56 0x0000000000dc34a5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc34a5)
#57 0x0000000000dbb44d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#58 0x0000000004579749 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#59 0x0000000003b91bd3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b91bd3)
#60 0x0000000004579969 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#61 0x000000000453cacd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x453cacd)
#62 0x000000000453da81 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x453da81)
#63 0x00000000045473dc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45473dc)
#64 0x0000000000dbfcb1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbfcb1)
#65 0x0000000000c6e4b4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc6e4b4)
#66 0x000071789d429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#67 0x000071789d429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#68 0x0000000000dbaee5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbaee5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - tbaederr

Assertion: https://godbolt.org/z/zz9nb3G54

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2708: clang::QualType {anonymous}::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool): Assertion `Arg.getKind() == TemplateArgument::Type && "Template argument kind mismatch"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:6:20: current parser token ')'
2.	<source>:6:12: parsing function body 'main'
3.	<source>:6:12: in compound statement ('{}')
 #0 0x0000000004081b98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4081b98)
 #1 0x000000000407efc4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x407efc4)
 #2 0x0000000003fc3468 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000749ecd442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000749ecd4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000749ecd442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000749ecd4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000749ecd42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000749ecd439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000723bd73 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#10 0x000000000722677d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#11 0x000000000722b2fa clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000072306ee clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007235142 clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7235142)
#14 0x0000000007248487 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000007225b47 clang::QualType clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000007227360 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000007263bc4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x000000000721bdad clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x00000000072424d6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000721b8f2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000007218e3b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#22 0x000000000721b316 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#23 0x000000000721bc5e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#24 0x0000000007225692 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7225692)
#25 0x0000000006a1286e calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#26 0x0000000006a14888 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Eczbek)

<details>
https://godbolt.org/z/8hbjx1ePe

```cpp
template&lt;int = 0&gt;
struct S {
	void f(auto... a) requires(requires { [](auto) requires(!sizeof...(a)) {}(0); }) {}
};

int main() { S().f(); }
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:6:20: current parser token ')'
2.	&lt;source&gt;:6:12: parsing function body 'main'
3.	&lt;source&gt;:6:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003c43ef8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c43ef8)
 #<!-- -->1 0x0000000003c418cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c418cc)
 #<!-- -->2 0x0000000003b917b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000071789d442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006ffc9f9 clang::ASTContext::getSubstTemplateTypeParmType(clang::QualType, clang::Decl*, unsigned int, clang::UnsignedOrNone, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ffc9f9)
 #<!-- -->5 0x0000000006c7f50c (anonymous namespace)::TemplateInstantiator::BuildSubstTemplateTypeParmType(clang::TypeLocBuilder&amp;, bool, bool, clang::Decl*, unsigned int, clang::UnsignedOrNone, clang::TemplateArgument, clang::SourceLocation) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->6 0x0000000006cac7a1 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->7 0x0000000006c9ad94 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->8 0x0000000006c9c8b7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->9 0x0000000006ca004f clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ca004f)
#<!-- -->10 0x0000000006ca7dac clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ca7dac)
#<!-- -->11 0x0000000006cb9f14 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef&lt;clang::ParmVarDecl*&gt;, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl&lt;clang::QualType&gt;&amp;, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;*, clang::Sema::ExtParameterInfoBuilder&amp;, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x0000000006c98cec clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionProtoType(clang::TypeLocBuilder&amp;, clang::FunctionProtoTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000006c9a29d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x0000000006cd6aea clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x0000000006c92b08 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000006cb39d0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x0000000006c921aa clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x0000000006c90607 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x0000000006c9257e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x0000000006c98765 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c98765)
#<!-- -->21 0x0000000006482235 calculateConstraintSatisfaction(clang::Sema&amp;, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->22 0x0000000006484275 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->23 0x0000000006487387 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (.constprop.0) SemaConcept.cpp:0:0
#<!-- -->24 0x00000000064883d8 clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64883d8)
#<!-- -->25 0x0000000006c36800 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c36800)
#<!-- -->26 0x0000000006c3780a void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->27 0x000000000795b581 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x795b581)
#<!-- -->28 0x0000000006c0478b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c0478b)
#<!-- -->29 0x0000000006a615cd AddMethodTemplateCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#<!-- -->30 0x0000000006a640c3 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a640c3)
#<!-- -->31 0x0000000006a64174 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a64174)
#<!-- -->32 0x0000000006a7a094 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a7a094)
#<!-- -->33 0x00000000066f2f7d clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66f2f7d)
#<!-- -->34 0x00000000066f7aac clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66f7aac)
#<!-- -->35 0x000000000620bbbe clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620bbbe)
#<!-- -->36 0x0000000006203afa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6203afa)
#<!-- -->37 0x0000000006205d9a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6205d9a)
#<!-- -->38 0x0000000006205f89 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6205f89)
#<!-- -->39 0x000000000620b089 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620b089)
#<!-- -->40 0x000000000629a977 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x629a977)
#<!-- -->41 0x0000000006291392 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6291392)
#<!-- -->42 0x0000000006291d89 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6291d89)
#<!-- -->43 0x000000000629b840 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x629b840)
#<!-- -->44 0x000000000629c27a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x629c27a)
#<!-- -->45 0x0000000006185e13 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6185e13)
#<!-- -->46 0x00000000061d27ef clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61d27ef)
#<!-- -->47 0x000000000617e8ff clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617e8ff)
#<!-- -->48 0x000000000617f6f7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x617f6f7)
#<!-- -->49 0x000000000618834f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618834f)
#<!-- -->50 0x0000000006189b57 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6189b57)
#<!-- -->51 0x000000000616260a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x616260a)
#<!-- -->52 0x000000000450c4ad clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450c4ad)
#<!-- -->53 0x0000000004812ffa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4812ffa)
#<!-- -->54 0x000000000478d23b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x478d23b)
#<!-- -->55 0x0000000004907693 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4907693)
#<!-- -->56 0x0000000000dc34a5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc34a5)
#<!-- -->57 0x0000000000dbb44d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->58 0x0000000004579749 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->59 0x0000000003b91bd3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b91bd3)
#<!-- -->60 0x0000000004579969 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->61 0x000000000453cacd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x453cacd)
#<!-- -->62 0x000000000453da81 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x453da81)
#<!-- -->63 0x00000000045473dc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45473dc)
#<!-- -->64 0x0000000000dbfcb1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbfcb1)
#<!-- -->65 0x0000000000c6e4b4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc6e4b4)
#<!-- -->66 0x000071789d429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->67 0x000071789d429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->68 0x0000000000dbaee5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbaee5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 3 - shafik

CC @cor3ntin 

---

### Comment 4 - jiixyj

Here is another reproducer for this assert, curiously only if `-std=c++26` is active. C++23 and C++20 compile fine.

https://godbolt.org/z/7E5drWses

It also seems to come from a `requires` clause in a IILE in a `requires` clause (`requires(Signature sig) { []<callable Tag>(Tag) {}(sig); };`).

It does not matter if it is `[]<callable Tag>(Tag)` or `[]<class Tag> requires callable<Tag> (Tag)`.

---

### Comment 5 - jiixyj

> https://godbolt.org/z/7E5drWses

`Arg.getKind()` is `TemplateArgument::Pack` in the assert if that helps.

---

