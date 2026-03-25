# [clang] Assertion `(T->hasSignedIntegerRepresentation() || T->isSignedFixedPointType()) && "Unexpected signed integer or fixed point type"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/174331
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, regression:22
**Closed Date:** 2026-01-27T07:50:33Z

## Body

Reproducer:
https://godbolt.org/z/TPh3vjqqv
```cpp
void foo(_Bool *zn) { printf("%zn", zn); }
```

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00051.cpp

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:12294: clang::QualType clang::ASTContext::getCorrespondingSignedType(clang::QualType) const: Assertion `(T->hasSignedIntegerRepresentation() || T->isSignedFixedPointType()) && "Unexpected signed integer or fixed point type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<source>:1:39: current parser token ')'
2.	<source>:1:21: parsing function body 'foo'
3.	<source>:1:21: in compound statement ('{}')
 #0 0x0000000004308698 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4308698)
 #1 0x0000000004305ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4305ac4)
 #2 0x000000000424a568 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a2c84842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a2c848969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007a2c84842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007a2c848287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007a2c8482871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007a2c84839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007994116 clang::ASTContext::getCorrespondingSignedType(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7994116)
#10 0x0000000007dcad06 matchesSizeTPtrdiffT(clang::ASTContext&, clang::QualType, clang::QualType) FormatString.cpp:0:0
#11 0x0000000006ca2d11 (anonymous namespace)::CheckPrintfHandler::HandlePrintfSpecifier(clang::analyze_printf::PrintfSpecifier const&, char const*, unsigned int, clang::TargetInfo const&) SemaChecking.cpp:0:0
#12 0x00000000080b5031 clang::analyze_format_string::ParsePrintfString(clang::analyze_format_string::FormatStringHandler&, char const*, char const*, clang::LangOptions const&, clang::TargetInfo const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80b5031)
#13 0x0000000006cb6ce9 checkFormatStringExpr(clang::Sema&, clang::StringLiteral const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::FormatStringType, clang::VariadicCallType, bool, llvm::SmallBitVector&, (anonymous namespace)::UncoveredArgHandler&, llvm::APSInt, std::optional<unsigned int>*, bool) SemaChecking.cpp:0:0
#14 0x0000000006cb7081 clang::Sema::CheckFormatArguments(llvm::ArrayRef<clang::Expr const*>, clang::Sema::FormatArgumentPassingKind, clang::StringLiteral*, unsigned int, unsigned int, clang::FormatStringType, clang::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb7081)
#15 0x0000000006cb8477 clang::Sema::CheckFormatArguments(clang::FormatAttr const*, llvm::ArrayRef<clang::Expr const*>, bool, clang::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb8477)
#16 0x0000000006cbd9fb clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, bool, clang::SourceLocation, clang::SourceRange, clang::VariadicCallType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbd9fb)
#17 0x0000000006cbfc7c clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbfc7c)
#18 0x0000000006fc6bcb clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc6bcb)
#19 0x0000000006fc1aea clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc1aea)
#20 0x0000000006fc7a4c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc7a4c)
#21 0x0000000006ac5d27 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac5d27)
#22 0x0000000006abec3a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abec3a)
#23 0x0000000006ac0e97 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac0e97)
#24 0x0000000006ac0f29 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac0f29)
#25 0x0000000006ac5509 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac5509)
#26 0x0000000006b4adc9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4adc9)
#27 0x0000000006b42274 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b42274)
#28 0x0000000006b42c56 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b42c56)
#29 0x0000000006b4b59e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4b59e)
#30 0x0000000006b4bdea clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4bdea)
#31 0x0000000006a51d4b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a51d4b)
#32 0x0000000006a8820d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8820d)
#33 0x0000000006a44eab clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a44eab)
#34 0x0000000006a4569f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4569f)
#35 0x0000000006a4d4ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4d4ca)
#36 0x0000000006a4e465 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4e465)
#37 0x0000000006a4e910 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4e910)
#38 0x0000000006a2f4b3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2f4b3)
#39 0x0000000004c69a48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c69a48)
#40 0x0000000004f60215 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f60215)
#41 0x0000000004ee03ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee03ce)
#42 0x000000000505a59d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505a59d)
#43 0x0000000000de704c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde704c)
#44 0x0000000000ddda9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#45 0x0000000000dddc1d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#46 0x0000000004cdc119 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#47 0x000000000424aa04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424aa04)
#48 0x0000000004cdc72f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#49 0x0000000004c9d212 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9d212)
#50 0x0000000004c9e1be clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9e1be)
#51 0x0000000004ca5615 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca5615)
#52 0x0000000000de3441 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde3441)
#53 0x0000000000c8dd74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8dd74)
#54 0x00007a2c84829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x00007a2c84829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x0000000000ddd535 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd535)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

This seems a regression on trunk:
https://godbolt.org/z/eY8Yvr5T4

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/TPh3vjqqv
```cpp
void foo(_Bool *zn) { printf("%zn", zn); }
```

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00051.cpp

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:12294: clang::QualType clang::ASTContext::getCorrespondingSignedType(clang::QualType) const: Assertion `(T-&gt;hasSignedIntegerRepresentation() || T-&gt;isSignedFixedPointType()) &amp;&amp; "Unexpected signed integer or fixed point type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c &lt;source&gt;
1.	&lt;source&gt;:1:39: current parser token ')'
2.	&lt;source&gt;:1:21: parsing function body 'foo'
3.	&lt;source&gt;:1:21: in compound statement ('{}')
 #<!-- -->0 0x0000000004308698 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4308698)
 #<!-- -->1 0x0000000004305ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4305ac4)
 #<!-- -->2 0x000000000424a568 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a2c84842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007a2c848969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007a2c84842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007a2c848287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007a2c8482871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007a2c84839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007994116 clang::ASTContext::getCorrespondingSignedType(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7994116)
#<!-- -->10 0x0000000007dcad06 matchesSizeTPtrdiffT(clang::ASTContext&amp;, clang::QualType, clang::QualType) FormatString.cpp:0:0
#<!-- -->11 0x0000000006ca2d11 (anonymous namespace)::CheckPrintfHandler::HandlePrintfSpecifier(clang::analyze_printf::PrintfSpecifier const&amp;, char const*, unsigned int, clang::TargetInfo const&amp;) SemaChecking.cpp:0:0
#<!-- -->12 0x00000000080b5031 clang::analyze_format_string::ParsePrintfString(clang::analyze_format_string::FormatStringHandler&amp;, char const*, char const*, clang::LangOptions const&amp;, clang::TargetInfo const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80b5031)
#<!-- -->13 0x0000000006cb6ce9 checkFormatStringExpr(clang::Sema&amp;, clang::StringLiteral const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::FormatStringType, clang::VariadicCallType, bool, llvm::SmallBitVector&amp;, (anonymous namespace)::UncoveredArgHandler&amp;, llvm::APSInt, std::optional&lt;unsigned int&gt;*, bool) SemaChecking.cpp:0:0
#<!-- -->14 0x0000000006cb7081 clang::Sema::CheckFormatArguments(llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::Sema::FormatArgumentPassingKind, clang::StringLiteral*, unsigned int, unsigned int, clang::FormatStringType, clang::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb7081)
#<!-- -->15 0x0000000006cb8477 clang::Sema::CheckFormatArguments(clang::FormatAttr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb8477)
#<!-- -->16 0x0000000006cbd9fb clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::SourceLocation, clang::SourceRange, clang::VariadicCallType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbd9fb)
#<!-- -->17 0x0000000006cbfc7c clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbfc7c)
#<!-- -->18 0x0000000006fc6bcb clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc6bcb)
#<!-- -->19 0x0000000006fc1aea clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc1aea)
#<!-- -->20 0x0000000006fc7a4c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc7a4c)
#<!-- -->21 0x0000000006ac5d27 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac5d27)
#<!-- -->22 0x0000000006abec3a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abec3a)
#<!-- -->23 0x0000000006ac0e97 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac0e97)
#<!-- -->24 0x0000000006ac0f29 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac0f29)
#<!-- -->25 0x0000000006ac5509 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ac5509)
#<!-- -->26 0x0000000006b4adc9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4adc9)
#<!-- -->27 0x0000000006b42274 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b42274)
#<!-- -->28 0x0000000006b42c56 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b42c56)
#<!-- -->29 0x0000000006b4b59e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4b59e)
#<!-- -->30 0x0000000006b4bdea clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4bdea)
#<!-- -->31 0x0000000006a51d4b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a51d4b)
#<!-- -->32 0x0000000006a8820d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8820d)
#<!-- -->33 0x0000000006a44eab clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a44eab)
#<!-- -->34 0x0000000006a4569f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4569f)
#<!-- -->35 0x0000000006a4d4ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4d4ca)
#<!-- -->36 0x0000000006a4e465 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4e465)
#<!-- -->37 0x0000000006a4e910 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4e910)
#<!-- -->38 0x0000000006a2f4b3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2f4b3)
#<!-- -->39 0x0000000004c69a48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c69a48)
#<!-- -->40 0x0000000004f60215 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f60215)
#<!-- -->41 0x0000000004ee03ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee03ce)
#<!-- -->42 0x000000000505a59d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505a59d)
#<!-- -->43 0x0000000000de704c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde704c)
#<!-- -->44 0x0000000000ddda9a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->45 0x0000000000dddc1d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->46 0x0000000004cdc119 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->47 0x000000000424aa04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424aa04)
#<!-- -->48 0x0000000004cdc72f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->49 0x0000000004c9d212 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9d212)
#<!-- -->50 0x0000000004c9e1be clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9e1be)
#<!-- -->51 0x0000000004ca5615 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca5615)
#<!-- -->52 0x0000000000de3441 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde3441)
#<!-- -->53 0x0000000000c8dd74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8dd74)
#<!-- -->54 0x00007a2c84829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->55 0x00007a2c84829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->56 0x0000000000ddd535 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd535)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

This seems a regression on trunk:
https://godbolt.org/z/eY8Yvr5T4
</details>


---

### Comment 2 - HamzaHassanain

I can investigate this!

---

### Comment 3 - shafik

CC @vvuksanovic maybe 1df7b512bde79

---

### Comment 4 - vvuksanovic

Bisected to 7c402b8. CC @YexuanXiao 

`getCorrespondingSignedType` gets called for the `_Bool` type which is not handled. This was the case even before that commit, so it might be a preexisting issue.

---

### Comment 5 - YexuanXiao

I still remember these code snippets, so I'll take a look. 😝

---

### Comment 6 - YexuanXiao

@vvuksanovic What should be the correct behavior of `getCorrespondingSignedType` for `_Bool`? I am indeed surprised that it rejects `_Bool`.

---

### Comment 7 - vvuksanovic

> What should be the correct behavior of getCorrespondingSignedType for _Bool?

According to C99 6.2.5p6, there isn't one. Looking at other uses of `getCorrespondingSignedType` they avoid calling it if the type is a bool. You should probably add a check for that in `matchesSizeTPtrdiffT`.

It might be a good idea to also check that in the assertion at the start of `getCorrespondingSignedType` to make it clear it is not supported. Instead it fails the assertion in the default switch case.

---

### Comment 8 - YexuanXiao

While writing new tests, I discovered a new bug: if the format specifier expects `short*` but `_Bool*` is actually passed, no warning is generated. https://godbolt.org/z/a3z1zsMas I'll try to fix it along with that.

It belongs to -Wformat-type-confusion. I don’t understand the purpose of this classification, and for now it seems best not to bother with it.

---

