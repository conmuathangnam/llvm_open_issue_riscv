# [clang] Crash for template inner class with default argument

**Author:** Rush10233
**URL:** https://github.com/llvm/llvm-project/issues/132251

## Body

The following code causes clang to crash:

```
  template<typename T>
  class B {
  public:
    template <typename=int>class Inner : public A { };

    static Inner i;
    static const unsigned value = sizeof(i) == 4;

    int f() { return B<int>::value; }

  }
```

[https://godbolt.org/z/7WrWrY85P](https://godbolt.org/z/7WrWrY85P)


Stack Dump:

```
<source>:4:49: error: expected class name
    4 |     template <typename=int>class Inner : public A { };
      |                                                 ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:9:30: current parser token 'value'
2.	<source>:2:3: parsing struct/union/class body 'B'
3.	<source>:9:13: parsing function body 'B::f'
4.	<source>:9:13: in compound statement ('{}')
5.	<source>:2:9: instantiating class definition 'B<int>'
 #0 0x0000000003b7abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b7abe8)
 #1 0x0000000003b78d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b78d2c)
 #2 0x0000000003ac85a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fcaeac42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006ffa170 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ffa170)
 #5 0x0000000006fe4bab clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fe4bab)
 #6 0x0000000006fe4bab clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fe4bab)
 #7 0x0000000006ffbb9b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ffbb9b)
 #8 0x0000000006ffc4e9 clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ffc4e9)
 #9 0x00000000072d8c53 HandleSizeof((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::CharUnits&, SizeOfType) ExprConstant.cpp:0:0
#10 0x0000000007349b1e clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x000000000730db6e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#12 0x0000000007379e90 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#13 0x0000000007349339 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#14 0x000000000730db6e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#15 0x00000000073161bf EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#16 0x000000000731a19a clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x731a19a)
#17 0x000000000632dfdc TryGetExprRange(clang::ASTContext&, clang::Expr const*, unsigned int, bool, bool) SemaChecking.cpp:0:0
#18 0x000000000636090d clang::Sema::CheckImplicitConversion(clang::Expr*, clang::QualType, clang::SourceLocation, bool*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x636090d)
#19 0x000000000636378c AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#20 0x000000000636636a clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x636636a)
#21 0x0000000006714a54 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6714a54)
#22 0x00000000064a1449 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64a1449)
#23 0x0000000006ced1b8 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ced1b8)
#24 0x0000000006d360b2 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d360b2)
#25 0x0000000006d428a3 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d428a3)
#26 0x0000000006caed85 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6caed85)
#27 0x0000000006ce5b77 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ce5b77)
#28 0x0000000006d6056e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#29 0x00000000078efdd1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78efdd1)
#30 0x0000000006d653f5 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d653f5)
#31 0x0000000006d65475 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d65475)
#32 0x00000000062fc715 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&, clang::DeclContext*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fc715)
#33 0x00000000064b7908 clang::Sema::getTypeName(clang::IdentifierInfo const&, clang::SourceLocation, clang::Scope*, clang::CXXScopeSpec*, bool, bool, clang::OpaquePtr<clang::QualType>, bool, bool, bool, clang::ImplicitTypenameContext, clang::IdentifierInfo**) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64b7908)
#34 0x00000000060d837d clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d837d)
#35 0x00000000060d86d4 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d86d4)
#36 0x000000000615d1ce clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x615d1ce)
#37 0x000000000615f1ba clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x615f1ba)
#38 0x000000000615f3a9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x615f3a9)
#39 0x0000000006164719 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6164719)
#40 0x00000000061e3990 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e3990)
#41 0x00000000061e948a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e948a)
#42 0x00000000061ea69e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ea69e)
#43 0x00000000061f30bd clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f30bd)
#44 0x00000000061f3afa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f3afa)
#45 0x00000000060fafaa clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60fafaa)
#46 0x00000000060fac7d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60fac7d)
#47 0x00000000061531af clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61531af)
#48 0x0000000006154f08 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6154f08)
#49 0x000000000611da69 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x611da69)
#50 0x00000000061fe54a clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61fe54a)
#51 0x000000000620ba9a clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620ba9a)
#52 0x000000000620be1f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620be1f)
#53 0x000000000612fd17 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x612fd17)
#54 0x00000000060e4f62 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e4f62)
#55 0x00000000060e67d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e67d8)
#56 0x00000000060e6d20 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e6d20)
#57 0x00000000060d4b23 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d4b23)
#58 0x00000000044f8d8d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44f8d8d)
#59 0x00000000047ddc31 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47ddc31)
#60 0x000000000475c1bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x475c1bb)
#61 0x00000000048cddb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48cddb3)
#62 0x0000000000d65be1 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd65be1)
#63 0x0000000000d5e3dd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#64 0x0000000004546089 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#65 0x0000000003ac89c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ac89c3)
#66 0x00000000045462a9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#67 0x00000000045095cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45095cd)
#68 0x000000000450a591 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450a591)
#69 0x000000000451492c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451492c)
#70 0x0000000000d629b1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd629b1)
#71 0x0000000000c23264 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc23264)
#72 0x00007fcaeac29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#73 0x00007fcaeac29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#74 0x0000000000d5de75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5de75)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Rush10233)

<details>
The following code causes clang to crash:

```
  template&lt;typename T&gt;
  class B {
  public:
    template &lt;typename=int&gt;class Inner : public A { };

    static Inner i;
    static const unsigned value = sizeof(i) == 4;

    int f() { return B&lt;int&gt;::value; }

  }
```

[https://godbolt.org/z/7WrWrY85P](https://godbolt.org/z/7WrWrY85P)


Stack Dump:

```
&lt;source&gt;:4:49: error: expected class name
    4 |     template &lt;typename=int&gt;class Inner : public A { };
      |                                                 ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:9:30: current parser token 'value'
2.	&lt;source&gt;:2:3: parsing struct/union/class body 'B'
3.	&lt;source&gt;:9:13: parsing function body 'B::f'
4.	&lt;source&gt;:9:13: in compound statement ('{}')
5.	&lt;source&gt;:2:9: instantiating class definition 'B&lt;int&gt;'
 #<!-- -->0 0x0000000003b7abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b7abe8)
 #<!-- -->1 0x0000000003b78d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b78d2c)
 #<!-- -->2 0x0000000003ac85a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fcaeac42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006ffa170 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ffa170)
 #<!-- -->5 0x0000000006fe4bab clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fe4bab)
 #<!-- -->6 0x0000000006fe4bab clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fe4bab)
 #<!-- -->7 0x0000000006ffbb9b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ffbb9b)
 #<!-- -->8 0x0000000006ffc4e9 clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ffc4e9)
 #<!-- -->9 0x00000000072d8c53 HandleSizeof((anonymous namespace)::EvalInfo&amp;, clang::SourceLocation, clang::QualType, clang::CharUnits&amp;, SizeOfType) ExprConstant.cpp:0:0
#<!-- -->10 0x0000000007349b1e clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->11 0x000000000730db6e Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007379e90 (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(clang::BinaryOperator const*) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007349339 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->14 0x000000000730db6e Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000073161bf EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->16 0x000000000731a19a clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x731a19a)
#<!-- -->17 0x000000000632dfdc TryGetExprRange(clang::ASTContext&amp;, clang::Expr const*, unsigned int, bool, bool) SemaChecking.cpp:0:0
#<!-- -->18 0x000000000636090d clang::Sema::CheckImplicitConversion(clang::Expr*, clang::QualType, clang::SourceLocation, bool*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x636090d)
#<!-- -->19 0x000000000636378c AnalyzeImplicitConversions(clang::Sema&amp;, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#<!-- -->20 0x000000000636636a clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x636636a)
#<!-- -->21 0x0000000006714a54 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6714a54)
#<!-- -->22 0x00000000064a1449 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64a1449)
#<!-- -->23 0x0000000006ced1b8 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ced1b8)
#<!-- -->24 0x0000000006d360b2 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d360b2)
#<!-- -->25 0x0000000006d428a3 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef&lt;clang::BindingDecl*&gt;*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d428a3)
#<!-- -->26 0x0000000006caed85 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6caed85)
#<!-- -->27 0x0000000006ce5b77 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6ce5b77)
#<!-- -->28 0x0000000006d6056e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->29 0x00000000078efdd1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x78efdd1)
#<!-- -->30 0x0000000006d653f5 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d653f5)
#<!-- -->31 0x0000000006d65475 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d65475)
#<!-- -->32 0x00000000062fc715 clang::Sema::RequireCompleteDeclContext(clang::CXXScopeSpec&amp;, clang::DeclContext*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fc715)
#<!-- -->33 0x00000000064b7908 clang::Sema::getTypeName(clang::IdentifierInfo const&amp;, clang::SourceLocation, clang::Scope*, clang::CXXScopeSpec*, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool, clang::ImplicitTypenameContext, clang::IdentifierInfo**) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64b7908)
#<!-- -->34 0x00000000060d837d clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&amp;, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d837d)
#<!-- -->35 0x00000000060d86d4 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d86d4)
#<!-- -->36 0x000000000615d1ce clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x615d1ce)
#<!-- -->37 0x000000000615f1ba clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x615f1ba)
#<!-- -->38 0x000000000615f3a9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x615f3a9)
#<!-- -->39 0x0000000006164719 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6164719)
#<!-- -->40 0x00000000061e3990 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e3990)
#<!-- -->41 0x00000000061e948a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e948a)
#<!-- -->42 0x00000000061ea69e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ea69e)
#<!-- -->43 0x00000000061f30bd clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f30bd)
#<!-- -->44 0x00000000061f3afa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f3afa)
#<!-- -->45 0x00000000060fafaa clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60fafaa)
#<!-- -->46 0x00000000060fac7d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60fac7d)
#<!-- -->47 0x00000000061531af clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61531af)
#<!-- -->48 0x0000000006154f08 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6154f08)
#<!-- -->49 0x000000000611da69 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x611da69)
#<!-- -->50 0x00000000061fe54a clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61fe54a)
#<!-- -->51 0x000000000620ba9a clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620ba9a)
#<!-- -->52 0x000000000620be1f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x620be1f)
#<!-- -->53 0x000000000612fd17 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x612fd17)
#<!-- -->54 0x00000000060e4f62 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e4f62)
#<!-- -->55 0x00000000060e67d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e67d8)
#<!-- -->56 0x00000000060e6d20 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e6d20)
#<!-- -->57 0x00000000060d4b23 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d4b23)
#<!-- -->58 0x00000000044f8d8d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44f8d8d)
#<!-- -->59 0x00000000047ddc31 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47ddc31)
#<!-- -->60 0x000000000475c1bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x475c1bb)
#<!-- -->61 0x00000000048cddb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48cddb3)
#<!-- -->62 0x0000000000d65be1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd65be1)
#<!-- -->63 0x0000000000d5e3dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->64 0x0000000004546089 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->65 0x0000000003ac89c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ac89c3)
#<!-- -->66 0x00000000045462a9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->67 0x00000000045095cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45095cd)
#<!-- -->68 0x000000000450a591 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450a591)
#<!-- -->69 0x000000000451492c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451492c)
#<!-- -->70 0x0000000000d629b1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd629b1)
#<!-- -->71 0x0000000000c23264 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc23264)
#<!-- -->72 0x00007fcaeac29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->73 0x00007fcaeac29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->74 0x0000000000d5de75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5de75)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - shafik

Goes back to clang-16: https://godbolt.org/z/q1vxvhzv7

Is this fuzzer generated?

Please, in future bug reports it is always helpful to know when the bug started to show up, assertions builds are preferred since the assertion usually points to the root issue better. 

---

### Comment 3 - Rush10233

> Goes back to clang-16: https://godbolt.org/z/q1vxvhzv7
> 
> Is this fuzzer generated?
> 
> Please, in future bug reports it is always helpful to know when the bug started to show up, assertions builds are preferred since the assertion usually points to the root issue better.

Okay, sincerely thanks for the advice and I'll try to attach them for the next issue submission.

---

### Comment 4 - shafik

This has to be in C++20 mode, this goes back to clang-5: https://godbolt.org/z/3Ma7qTr9j

---

