# ICE in clang since 3.5 to trunk in c++14 mode (and up)

**Author:** hanickadot
**URL:** https://github.com/llvm/llvm-project/issues/75847

## Body

https://compiler-explorer.com/z/vfvqd941z

minimized reproducer:
```c++
template <typename T> struct array {
  T data[1]{};
};

template <typename Target> struct helper {
  template <unsigned> static constexpr auto pointer = &Target::select;
  constexpr int populate() {
    array<decltype(pointer<0>)>{};
    return 0;
  }
};

struct jump_table {
  void select() {}

  static constexpr auto pointers = helper<jump_table>().populate();
};
```

```text
clang++: /root/llvm-project/clang/lib/AST/ASTContext.cpp:2295: clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const: Assertion `!A->getDeducedType().isNull() && "cannot request the size of an undeduced or dependent auto type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++1y <source>
1.	<source>:16:67: current parser token ';'
2.	<source>:13:1: parsing struct/union/class body 'jump_table'
 #0 0x00000000037cc328 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x37cc328)
 #1 0x00000000037ca00c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x37ca00c)
 #2 0x0000000003711e68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f6af1e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f6af1e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f6af1e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f6af1e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f6af1e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f6af1e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006f00e35 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f00e35)
#10 0x0000000006ee9bb7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ee9bb7)
#11 0x0000000006f00ca3 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f00ca3)
#12 0x0000000006ee9bb7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ee9bb7)
#13 0x0000000006f01e5b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f01e5b)
#14 0x0000000006f01db3 clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f01db3)
#15 0x0000000006f01f73 clang::ASTContext::getTypeInfoInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f01f73)
#16 0x00000000073ab485 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(clang::FieldDecl const*, bool) RecordLayoutBuilder.cpp:0:0
#17 0x00000000073acd0f (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutFields(clang::RecordDecl const*) RecordLayoutBuilder.cpp:0:0
#18 0x00000000073a85d6 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73a85d6)
#19 0x000000000717e41e (anonymous namespace)::RecordExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef<clang::Expr*>) ExprConstant.cpp:0:0
#20 0x00000000071b26ec clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#21 0x00000000071b579b (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::RecordExprEvaluator>::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#22 0x00000000071b5b37 (anonymous namespace)::RecordExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#23 0x00000000071b1bec clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#24 0x00000000071b3468 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#25 0x000000000715e467 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#26 0x000000000715e78f EvaluateIgnoredValue((anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#27 0x0000000007181c05 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#28 0x00000000071811d7 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#29 0x000000000718624b HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#30 0x00000000071c5cc2 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#31 0x000000000716efb9 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#32 0x000000000716ea27 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#33 0x000000000715de9e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#34 0x000000000717c6e2 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#35 0x00000000071b8158 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71b8158)
#36 0x000000000708d762 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x708d762)
#37 0x000000000708da9c clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x708da9c)
#38 0x000000000631957d clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x631957d)
#39 0x0000000006327ef2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6327ef2)
#40 0x000000000601fbb4 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x601fbb4)
#41 0x0000000006020a85 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6020a85)
#42 0x00000000060223c6 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x60223c6)
#43 0x0000000006024526 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6024526)
#44 0x0000000005ffda08 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ffda08)
#45 0x0000000005fc4c54 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fc4c54)
#46 0x0000000005fc55e1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fc55e1)
#47 0x0000000005fcc896 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fcc896)
#48 0x0000000005fcd73d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fcd73d)
#49 0x0000000005fc072a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fc072a)
#50 0x0000000004046dd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4046dd8)
#51 0x00000000042b2a09 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42b2a09)
#52 0x0000000004231a9e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231a9e)
#53 0x0000000004391fbe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4391fbe)
#54 0x0000000000c04756 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc04756)
#55 0x0000000000bfc01a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#56 0x0000000004087dc9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#57 0x0000000003712314 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3712314)
#58 0x00000000040883bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#59 0x0000000004050665 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4050665)
#60 0x00000000040510cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40510cd)
#61 0x0000000004058fc5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4058fc5)
#62 0x0000000000c01bdc clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc01bdc)
#63 0x0000000000afac21 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xafac21)
#64 0x00007f6af1e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#65 0x00007f6af1e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#66 0x0000000000bfbafe _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbfbafe)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hana Dusíková (hanickadot)

<details>
https://compiler-explorer.com/z/vfvqd941z

minimized reproducer:
```c++
template &lt;typename T&gt; struct array {
  T data[1]{};
};

template &lt;typename Target&gt; struct helper {
  template &lt;unsigned&gt; static constexpr auto pointer = &amp;Target::select;
  constexpr int populate() {
    array&lt;decltype(pointer&lt;0&gt;)&gt;{};
    return 0;
  }
};

struct jump_table {
  void select() {}

  static constexpr auto pointers = helper&lt;jump_table&gt;().populate();
};
```

```text
clang++: /root/llvm-project/clang/lib/AST/ASTContext.cpp:2295: clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const: Assertion `!A-&gt;getDeducedType().isNull() &amp;&amp; "cannot request the size of an undeduced or dependent auto type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++1y &lt;source&gt;
1.	&lt;source&gt;:16:67: current parser token ';'
2.	&lt;source&gt;:13:1: parsing struct/union/class body 'jump_table'
 #<!-- -->0 0x00000000037cc328 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x37cc328)
 #<!-- -->1 0x00000000037ca00c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x37ca00c)
 #<!-- -->2 0x0000000003711e68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f6af1e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f6af1e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007f6af1e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007f6af1e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007f6af1e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007f6af1e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f00e35 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f00e35)
#<!-- -->10 0x0000000006ee9bb7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ee9bb7)
#<!-- -->11 0x0000000006f00ca3 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f00ca3)
#<!-- -->12 0x0000000006ee9bb7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ee9bb7)
#<!-- -->13 0x0000000006f01e5b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f01e5b)
#<!-- -->14 0x0000000006f01db3 clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f01db3)
#<!-- -->15 0x0000000006f01f73 clang::ASTContext::getTypeInfoInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f01f73)
#<!-- -->16 0x00000000073ab485 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(clang::FieldDecl const*, bool) RecordLayoutBuilder.cpp:0:0
#<!-- -->17 0x00000000073acd0f (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutFields(clang::RecordDecl const*) RecordLayoutBuilder.cpp:0:0
#<!-- -->18 0x00000000073a85d6 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73a85d6)
#<!-- -->19 0x000000000717e41e (anonymous namespace)::RecordExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef&lt;clang::Expr*&gt;) ExprConstant.cpp:0:0
#<!-- -->20 0x00000000071b26ec clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->21 0x00000000071b579b (anonymous namespace)::ExprEvaluatorBase&lt;(anonymous namespace)::RecordExprEvaluator&gt;::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->22 0x00000000071b5b37 (anonymous namespace)::RecordExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->23 0x00000000071b1bec clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->24 0x00000000071b3468 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->25 0x000000000715e467 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->26 0x000000000715e78f EvaluateIgnoredValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->27 0x0000000007181c05 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->28 0x00000000071811d7 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->29 0x000000000718624b HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#<!-- -->30 0x00000000071c5cc2 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->31 0x000000000716efb9 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->32 0x000000000716ea27 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->33 0x000000000715de9e Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->34 0x000000000717c6e2 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->35 0x00000000071b8158 clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71b8158)
#<!-- -->36 0x000000000708d762 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x708d762)
#<!-- -->37 0x000000000708da9c clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x708da9c)
#<!-- -->38 0x000000000631957d clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x631957d)
#<!-- -->39 0x0000000006327ef2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6327ef2)
#<!-- -->40 0x000000000601fbb4 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x601fbb4)
#<!-- -->41 0x0000000006020a85 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6020a85)
#<!-- -->42 0x00000000060223c6 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x60223c6)
#<!-- -->43 0x0000000006024526 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6024526)
#<!-- -->44 0x0000000005ffda08 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5ffda08)
#<!-- -->45 0x0000000005fc4c54 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fc4c54)
#<!-- -->46 0x0000000005fc55e1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fc55e1)
#<!-- -->47 0x0000000005fcc896 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fcc896)
#<!-- -->48 0x0000000005fcd73d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fcd73d)
#<!-- -->49 0x0000000005fc072a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5fc072a)
#<!-- -->50 0x0000000004046dd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4046dd8)
#<!-- -->51 0x00000000042b2a09 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42b2a09)
#<!-- -->52 0x0000000004231a9e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231a9e)
#<!-- -->53 0x0000000004391fbe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4391fbe)
#<!-- -->54 0x0000000000c04756 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc04756)
#<!-- -->55 0x0000000000bfc01a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->56 0x0000000004087dc9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->57 0x0000000003712314 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3712314)
#<!-- -->58 0x00000000040883bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->59 0x0000000004050665 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4050665)
#<!-- -->60 0x00000000040510cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40510cd)
#<!-- -->61 0x0000000004058fc5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4058fc5)
#<!-- -->62 0x0000000000c01bdc clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc01bdc)
#<!-- -->63 0x0000000000afac21 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xafac21)
#<!-- -->64 0x00007f6af1e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->65 0x00007f6af1e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->66 0x0000000000bfbafe _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbfbafe)
```
</details>


---

### Comment 2 - shafik

Confirmed.

CC @erichkeane @cor3ntin @Fznamznon 

---

### Comment 3 - cor3ntin

a slight simplification leads to reject valid

```cpp
template <typename Target> struct helper {
  template <unsigned> static constexpr auto pointer = &Target::select;
  constexpr int populate() {
    decltype(pointer<0>){};
    return 0;
  }
};

struct jump_table {
  void select() {}
};

static auto pointers = helper<jump_table>().populate();
```

We do try to initialize `pointers`  (and constant evaluate `populate`) before the variable template is instantiated.
It's like `decltype(pointer<0>)` does not constitute a point of instantiation. So it's type is never deduced and remains `auto` (which will obviously crash in constant evaluation)

If you use pointer<0> in an evaluated context, it is correctly instantiated 
https://compiler-explorer.com/z/T45bj3cos




---

### Comment 4 - MagentaTreehouse

Simpler test case:
```c++
template <typename> struct helper {
  template <unsigned> static constexpr auto i = 0;
};

constexpr auto i = sizeof helper<void>::i<0>;
```

Assertion:
```console
clang++: /root/llvm-project/clang/lib/AST/ASTContext.cpp:2415:
clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const:
Assertion `!A->getDeducedType().isNull() && "cannot request the size of an undeduced or dependent auto type"' failed.
```

Stack dump:
```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:45: current parser token ';'
 #0 0x0000000003c56d58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c56d58)
 #1 0x0000000003c54a64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c54a64)
 #2 0x0000000003ba1fc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007df6db642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007df6db6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007df6db642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007df6db6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007df6db62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007df6db639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000745474d clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x745474d)
#10 0x000000000743d907 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x743d907)
#11 0x000000000745566b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x745566b)
#12 0x00000000074560fb clang::ASTContext::getTypeSizeInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74560fb)
#13 0x000000000770d408 HandleSizeof((anonymous namespace)::EvalInfo&, clang::SourceLocation, clang::QualType, clang::CharUnits&, SizeOfType) ExprConstant.cpp:0:0
#14 0x000000000771e4da (anonymous namespace)::IntExprEvaluator::VisitUnaryExprOrTypeTraitExpr(clang::UnaryExprOrTypeTraitExpr const*) ExprConstant.cpp:0:0
#15 0x000000000774c084 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#16 0x000000000773a5c6 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#17 0x0000000007756e56 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#18 0x00000000077989e6 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77989e6)
#19 0x0000000007609332 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7609332)
#20 0x00000000076096b1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76096b1)
#21 0x000000000695ef9e clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x695ef9e)
#22 0x00000000069658ce clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69658ce)
#23 0x00000000065fea0a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fea0a)
#24 0x000000000660daa9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660daa9)
#25 0x00000000065cd87e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65cd87e)
#26 0x00000000065ce039 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ce039)
#27 0x00000000065d57b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d57b3)
#28 0x00000000065d667d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d667d)
#29 0x00000000065c8bba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65c8bba)
#30 0x00000000045e05c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45e05c8)
#31 0x0000000004899a19 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4899a19)
#32 0x000000000481c41e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481c41e)
#33 0x000000000498625e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x498625e)
#34 0x0000000000ce1caf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce1caf)
#35 0x0000000000cd974a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x0000000004624a39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000003ba2474 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ba2474)
#38 0x000000000462502f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x00000000045ea33d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45ea33d)
#40 0x00000000045eb42d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45eb42d)
#41 0x00000000045f29a5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45f29a5)
#42 0x0000000000cdeaf3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcdeaf3)
#43 0x0000000000bac394 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbac394)
#44 0x00007df6db629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x00007df6db629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000cd91f5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcd91f5)
```

Compiler Explorer: https://compiler-explorer.com/z/cGf99cMx5

---

