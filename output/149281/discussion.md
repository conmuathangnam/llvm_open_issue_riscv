# [clang]Assertion `(getExceptionSpecType() == EST_DependentNoexcept) == epi.ExceptionSpec.NoexceptExpr->isValueDependent()` failed

**Author:** Rush10233
**URL:** https://github.com/llvm/llvm-project/issues/149281

## Body

Simplified Test:

```c++
template<typename T>
concept C = true;

template<C ...T> void i() {
    consume([]() noexcept(sizeof(T) == 4) {} ...);
  }

```

Starts to crash since clang-14: https://godbolt.org/z/dnqf4s63a

Stack Dump:

```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/Type.cpp:3730: clang::FunctionProtoType::FunctionProtoType(clang::QualType, llvm::ArrayRef<clang::QualType>, clang::QualType, const clang::FunctionProtoType::ExtProtoInfo&): Assertion `(getExceptionSpecType() == EST_DependentNoexcept) == epi.ExceptionSpec.NoexceptExpr->isValueDependent()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:43: current parser token '{'
2.	<source>:4:27: parsing function body 'i'
3.	<source>:4:27: in compound statement ('{}')
4.	<source>:5:13: lambda expression parsing
 #0 0x0000000003fb2168 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fb2168)
 #1 0x0000000003faf594 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3faf594)
 #2 0x0000000003ef3d68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076f724442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076f7244969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076f724442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076f7244287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076f72442871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076f724439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007ca8231 clang::FunctionProtoType::FunctionProtoType(clang::QualType, llvm::ArrayRef<clang::QualType>, clang::QualType, clang::FunctionProtoType::ExtProtoInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ca8231)
#10 0x000000000751cf68 clang::ASTContext::getFunctionTypeInternal(clang::QualType, llvm::ArrayRef<clang::QualType>, clang::FunctionProtoType::ExtProtoInfo const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x751cf68)
#11 0x00000000072897f4 GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#12 0x000000000728ddcf clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x728ddcf)
#13 0x0000000006d142b0 clang::Sema::ActOnStartOfLambdaDefinition(clang::LambdaIntroducer&, clang::Declarator&, clang::DeclSpec const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d142b0)
#14 0x00000000066fa5d8 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66fa5d8)
#15 0x00000000066fb9ab clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66fb9ab)
#16 0x00000000066dc335 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dc335)
#17 0x00000000066dccf7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dccf7)
#18 0x00000000066dcd89 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dcd89)
#19 0x00000000066dd0ee clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dd0ee)
#20 0x00000000066e1eed clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1eed)
#21 0x00000000066dab5a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dab5a)
#22 0x00000000066dccf7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dccf7)
#23 0x00000000066dcd89 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dcd89)
#24 0x00000000066e1439 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1439)
#25 0x0000000006775f49 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6775f49)
#26 0x000000000676d6cc clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x676d6cc)
#27 0x000000000676e509 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x676e509)
#28 0x000000000677663b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677663b)
#29 0x0000000006776e8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6776e8a)
#30 0x000000000666e843 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666e843)
#31 0x00000000066a463d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a463d)
#32 0x0000000006782658 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6782658)
#33 0x000000000679028f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#34 0x000000000679063a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x679063a)
#35 0x00000000066acf60 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66acf60)
#36 0x0000000006669b5c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6669b5c)
#37 0x000000000666aa45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666aa45)
#38 0x000000000665c81a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665c81a)
#39 0x0000000004967388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4967388)
#40 0x0000000004c5ad75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c5ad75)
#41 0x0000000004bd782e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd782e)
#42 0x0000000004d4e821 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d4e821)
#43 0x0000000000d933bf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd933bf)
#44 0x0000000000d89f9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#45 0x00000000049ce879 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#46 0x0000000003ef4204 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef4204)
#47 0x00000000049cee8f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#48 0x0000000004990d9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4990d9d)
#49 0x0000000004991e2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4991e2e)
#50 0x000000000499aa75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499aa75)
#51 0x0000000000d8f86f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8f86f)
#52 0x0000000000c44414 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc44414)
#53 0x000076f724429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#54 0x000076f724429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#55 0x0000000000d89a45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd89a45)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

Generated by fuzzer.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Rush10233)

<details>
Simplified Test:

```c++
template&lt;typename T&gt;
concept C = true;

template&lt;C ...T&gt; void i() {
    consume([]() noexcept(sizeof(T) == 4) {} ...);
  }

```

Starts to crash since clang-14: https://godbolt.org/z/dnqf4s63a

Stack Dump:

```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/Type.cpp:3730: clang::FunctionProtoType::FunctionProtoType(clang::QualType, llvm::ArrayRef&lt;clang::QualType&gt;, clang::QualType, const clang::FunctionProtoType::ExtProtoInfo&amp;): Assertion `(getExceptionSpecType() == EST_DependentNoexcept) == epi.ExceptionSpec.NoexceptExpr-&gt;isValueDependent()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:5:43: current parser token '{'
2.	&lt;source&gt;:4:27: parsing function body 'i'
3.	&lt;source&gt;:4:27: in compound statement ('{}')
4.	&lt;source&gt;:5:13: lambda expression parsing
 #<!-- -->0 0x0000000003fb2168 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fb2168)
 #<!-- -->1 0x0000000003faf594 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3faf594)
 #<!-- -->2 0x0000000003ef3d68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000076f724442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000076f7244969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000076f724442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000076f7244287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076f72442871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000076f724439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007ca8231 clang::FunctionProtoType::FunctionProtoType(clang::QualType, llvm::ArrayRef&lt;clang::QualType&gt;, clang::QualType, clang::FunctionProtoType::ExtProtoInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ca8231)
#<!-- -->10 0x000000000751cf68 clang::ASTContext::getFunctionTypeInternal(clang::QualType, llvm::ArrayRef&lt;clang::QualType&gt;, clang::FunctionProtoType::ExtProtoInfo const&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x751cf68)
#<!-- -->11 0x00000000072897f4 GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&amp;, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#<!-- -->12 0x000000000728ddcf clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x728ddcf)
#<!-- -->13 0x0000000006d142b0 clang::Sema::ActOnStartOfLambdaDefinition(clang::LambdaIntroducer&amp;, clang::Declarator&amp;, clang::DeclSpec const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d142b0)
#<!-- -->14 0x00000000066fa5d8 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66fa5d8)
#<!-- -->15 0x00000000066fb9ab clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66fb9ab)
#<!-- -->16 0x00000000066dc335 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dc335)
#<!-- -->17 0x00000000066dccf7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dccf7)
#<!-- -->18 0x00000000066dcd89 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dcd89)
#<!-- -->19 0x00000000066dd0ee clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dd0ee)
#<!-- -->20 0x00000000066e1eed clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1eed)
#<!-- -->21 0x00000000066dab5a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dab5a)
#<!-- -->22 0x00000000066dccf7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dccf7)
#<!-- -->23 0x00000000066dcd89 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dcd89)
#<!-- -->24 0x00000000066e1439 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1439)
#<!-- -->25 0x0000000006775f49 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6775f49)
#<!-- -->26 0x000000000676d6cc clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x676d6cc)
#<!-- -->27 0x000000000676e509 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x676e509)
#<!-- -->28 0x000000000677663b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x677663b)
#<!-- -->29 0x0000000006776e8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6776e8a)
#<!-- -->30 0x000000000666e843 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666e843)
#<!-- -->31 0x00000000066a463d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a463d)
#<!-- -->32 0x0000000006782658 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6782658)
#<!-- -->33 0x000000000679028f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->34 0x000000000679063a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x679063a)
#<!-- -->35 0x00000000066acf60 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66acf60)
#<!-- -->36 0x0000000006669b5c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6669b5c)
#<!-- -->37 0x000000000666aa45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666aa45)
#<!-- -->38 0x000000000665c81a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665c81a)
#<!-- -->39 0x0000000004967388 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4967388)
#<!-- -->40 0x0000000004c5ad75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c5ad75)
#<!-- -->41 0x0000000004bd782e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bd782e)
#<!-- -->42 0x0000000004d4e821 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d4e821)
#<!-- -->43 0x0000000000d933bf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd933bf)
#<!-- -->44 0x0000000000d89f9a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->45 0x00000000049ce879 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->46 0x0000000003ef4204 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef4204)
#<!-- -->47 0x00000000049cee8f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->48 0x0000000004990d9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4990d9d)
#<!-- -->49 0x0000000004991e2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4991e2e)
#<!-- -->50 0x000000000499aa75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499aa75)
#<!-- -->51 0x0000000000d8f86f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8f86f)
#<!-- -->52 0x0000000000c44414 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc44414)
#<!-- -->53 0x000076f724429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->54 0x000076f724429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->55 0x0000000000d89a45 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd89a45)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

Generated by fuzzer.
</details>


---

