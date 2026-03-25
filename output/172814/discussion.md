# clang assertion failure in getStackIndexOfNearestEnclosingCaptureCapableLambda with recursive lambda capture in default argument

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/172814
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, regression:19
**Closed Date:** 2026-01-07T09:38:23Z

## Body

### Link for quick verification: https://godbolt.org/z/sPjKKcWvK
```c
auto f() {
    int a = 0;
    return [](auto w=[&]{a += w();}); 
};
```
Go back to `clang 19.1.0 (assertions)`

## Stack dump
```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaLambda.cpp:193: clang::UnsignedOrNone clang::getStackIndexOfNearestEnclosingCaptureCapableLambda(llvm::ArrayRef<const clang::sema::FunctionScopeInfo*>, clang::ValueDecl*, clang::Sema&): Assertion `((IndexOfCaptureReadyLambda != (FunctionScopes.size() - 1)) || S.getCurGenericLambda()) && "The capture ready lambda for a potential capture can only be the " "current lambda if it is a generic lambda"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:3:33: current parser token '}'
2.	<source>:1:10: parsing function body 'f'
3.	<source>:1:10: in compound statement ('{}')
4.	<source>:3:12: lambda expression parsing
5.	<source>:3:22: lambda expression parsing
6.	<source>:3:25: in compound statement ('{}')
 #0 0x000000000429e098 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429e098)
 #1 0x000000000429af44 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429af44)
 #2 0x000000000429b4c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429b4c4)
 #3 0x00000000041e0208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x0000740202642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007402026969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x0000740202642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007402026287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x000074020262871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x0000740202639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x0000000007065a1f (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7065a1f)
#11 0x0000000006fafdba void llvm::function_ref<void (clang::ValueDecl*, clang::Expr*)>::callback_fn<CheckIfAnyEnclosingLambdasMustCaptureAnyPotentialCaptures(clang::Expr*, clang::sema::LambdaScopeInfo*, clang::Sema&)::'lambda'(clang::ValueDecl*, clang::Expr*)>(long, clang::ValueDecl*, clang::Expr*) SemaExprCXX.cpp:0:0
#12 0x00000000076771fa clang::sema::LambdaScopeInfo::visitPotentialCaptures(llvm::function_ref<void (clang::ValueDecl*, clang::Expr*)>) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76771fa)
#13 0x0000000006fbd1ae clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fbd1ae)
#14 0x00000000072da11e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72da11e)
#15 0x0000000006a9c644 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9c644)
#16 0x0000000006a94d75 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a94d75)
#17 0x0000000006a95739 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95739)
#18 0x0000000006a9cd26 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9cd26)
#19 0x0000000006a2fd42 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2fd42)
#20 0x0000000006a30e3b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a30e3b)
#21 0x0000000006a11de5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a11de5)
#22 0x0000000006a12807 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12807)
#23 0x0000000006a12899 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12899)
#24 0x00000000069e8025 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&, llvm::SmallVectorImpl<clang::DeclaratorChunk::ParamInfo>&, clang::SourceLocation&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e8025)
#25 0x0000000006a306c6 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a306c6)
#26 0x0000000006a30e3b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a30e3b)
#27 0x0000000006a11de5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a11de5)
#28 0x0000000006a12807 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12807)
#29 0x0000000006a12899 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12899)
#30 0x0000000006a16e79 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a16e79)
#31 0x0000000006a8f055 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8f055)
#32 0x0000000006a947ab clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a947ab)
#33 0x0000000006a95739 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95739)
#34 0x0000000006a9cd26 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9cd26)
#35 0x0000000006a9d53a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9d53a)
#36 0x000000000699ebdb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699ebdb)
#37 0x00000000069dec8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69dec8d)
#38 0x000000000699714b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699714b)
#39 0x000000000699793f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699793f)
#40 0x00000000069a2bd3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a2bd3)
#41 0x00000000069a3ae5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3ae5)
#42 0x00000000069a3f90 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3f90)
#43 0x0000000006981733 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6981733)
#44 0x0000000004bf5da8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf5da8)
#45 0x0000000004eea105 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eea105)
#46 0x0000000004e6a3be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6a3be)
#47 0x0000000004fe41ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fe41ed)
#48 0x0000000000dd9ef2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9ef2)
#49 0x0000000000dd091a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#50 0x0000000000dd0a9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#51 0x0000000004c67a89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#52 0x00000000041e06a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e06a4)
#53 0x0000000004c680b6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#54 0x0000000004c29732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c29732)
#55 0x0000000004c2a6de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2a6de)
#56 0x0000000004c319b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c319b5)
#57 0x0000000000dd62c1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd62c1)
#58 0x0000000000c813d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc813d4)
#59 0x0000740202629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#60 0x0000740202629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#61 0x0000000000dd03b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd03b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/sPjKKcWvK
```c
auto f() {
    int a = 0;
    return [](auto w=[&amp;]{a += w();}); 
};
```
Go back to `clang 19.1.0 (assertions)`

## Stack dump
```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaLambda.cpp:193: clang::UnsignedOrNone clang::getStackIndexOfNearestEnclosingCaptureCapableLambda(llvm::ArrayRef&lt;const clang::sema::FunctionScopeInfo*&gt;, clang::ValueDecl*, clang::Sema&amp;): Assertion `((IndexOfCaptureReadyLambda != (FunctionScopes.size() - 1)) || S.getCurGenericLambda()) &amp;&amp; "The capture ready lambda for a potential capture can only be the " "current lambda if it is a generic lambda"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:3:33: current parser token '}'
2.	&lt;source&gt;:1:10: parsing function body 'f'
3.	&lt;source&gt;:1:10: in compound statement ('{}')
4.	&lt;source&gt;:3:12: lambda expression parsing
5.	&lt;source&gt;:3:22: lambda expression parsing
6.	&lt;source&gt;:3:25: in compound statement ('{}')
 #<!-- -->0 0x000000000429e098 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429e098)
 #<!-- -->1 0x000000000429af44 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429af44)
 #<!-- -->2 0x000000000429b4c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429b4c4)
 #<!-- -->3 0x00000000041e0208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x0000740202642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007402026969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x0000740202642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007402026287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x000074020262871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x0000740202639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x0000000007065a1f (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7065a1f)
#<!-- -->11 0x0000000006fafdba void llvm::function_ref&lt;void (clang::ValueDecl*, clang::Expr*)&gt;::callback_fn&lt;CheckIfAnyEnclosingLambdasMustCaptureAnyPotentialCaptures(clang::Expr*, clang::sema::LambdaScopeInfo*, clang::Sema&amp;)::'lambda'(clang::ValueDecl*, clang::Expr*)&gt;(long, clang::ValueDecl*, clang::Expr*) SemaExprCXX.cpp:0:0
#<!-- -->12 0x00000000076771fa clang::sema::LambdaScopeInfo::visitPotentialCaptures(llvm::function_ref&lt;void (clang::ValueDecl*, clang::Expr*)&gt;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76771fa)
#<!-- -->13 0x0000000006fbd1ae clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fbd1ae)
#<!-- -->14 0x00000000072da11e clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72da11e)
#<!-- -->15 0x0000000006a9c644 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9c644)
#<!-- -->16 0x0000000006a94d75 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a94d75)
#<!-- -->17 0x0000000006a95739 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95739)
#<!-- -->18 0x0000000006a9cd26 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9cd26)
#<!-- -->19 0x0000000006a2fd42 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2fd42)
#<!-- -->20 0x0000000006a30e3b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a30e3b)
#<!-- -->21 0x0000000006a11de5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a11de5)
#<!-- -->22 0x0000000006a12807 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12807)
#<!-- -->23 0x0000000006a12899 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12899)
#<!-- -->24 0x00000000069e8025 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&amp;, llvm::SmallVectorImpl&lt;clang::DeclaratorChunk::ParamInfo&gt;&amp;, clang::SourceLocation&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e8025)
#<!-- -->25 0x0000000006a306c6 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a306c6)
#<!-- -->26 0x0000000006a30e3b clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a30e3b)
#<!-- -->27 0x0000000006a11de5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a11de5)
#<!-- -->28 0x0000000006a12807 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12807)
#<!-- -->29 0x0000000006a12899 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12899)
#<!-- -->30 0x0000000006a16e79 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a16e79)
#<!-- -->31 0x0000000006a8f055 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8f055)
#<!-- -->32 0x0000000006a947ab clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a947ab)
#<!-- -->33 0x0000000006a95739 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95739)
#<!-- -->34 0x0000000006a9cd26 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9cd26)
#<!-- -->35 0x0000000006a9d53a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9d53a)
#<!-- -->36 0x000000000699ebdb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699ebdb)
#<!-- -->37 0x00000000069dec8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69dec8d)
#<!-- -->38 0x000000000699714b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699714b)
#<!-- -->39 0x000000000699793f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699793f)
#<!-- -->40 0x00000000069a2bd3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a2bd3)
#<!-- -->41 0x00000000069a3ae5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3ae5)
#<!-- -->42 0x00000000069a3f90 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3f90)
#<!-- -->43 0x0000000006981733 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6981733)
#<!-- -->44 0x0000000004bf5da8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf5da8)
#<!-- -->45 0x0000000004eea105 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eea105)
#<!-- -->46 0x0000000004e6a3be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6a3be)
#<!-- -->47 0x0000000004fe41ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fe41ed)
#<!-- -->48 0x0000000000dd9ef2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9ef2)
#<!-- -->49 0x0000000000dd091a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->50 0x0000000000dd0a9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->51 0x0000000004c67a89 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->52 0x00000000041e06a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e06a4)
#<!-- -->53 0x0000000004c680b6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->54 0x0000000004c29732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c29732)
#<!-- -->55 0x0000000004c2a6de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2a6de)
#<!-- -->56 0x0000000004c319b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c319b5)
#<!-- -->57 0x0000000000dd62c1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd62c1)
#<!-- -->58 0x0000000000c813d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc813d4)
#<!-- -->59 0x0000740202629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->60 0x0000740202629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->61 0x0000000000dd03b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd03b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

