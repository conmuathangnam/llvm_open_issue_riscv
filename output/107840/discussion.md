# Clang clash with the Assertion `idx < size()' failed.

**Author:** Strke
**URL:** https://github.com/llvm/llvm-project/issues/107840
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-06-13T10:45:42Z

## Body

Reproducer: https://gcc.godbolt.org/z/dnoKqaofo 
```
#include <iostream>
#include <algorithm>
#include <array>

auto getCache = getPool([&](bool *const result, tms& ts) {
  std::thread([=] {
    tms time = std::chrono::system_clock::now();
    std::this_thread::sleep_for(ts);
    if (time >= ts) {
        *result = true;

    } else {
        *result = false;
    }
  }).detach();
});

```
and get the result
```
clang++: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:291: T& llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::operator[](llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type) [with T = clang::sema::FunctionScopeInfo*; <template-parameter-1-2> = void; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::reference = clang::sema::FunctionScopeInfo*&; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type = long unsigned int]: Assertion `idx < size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 <source>
1.	<source>:16:1: current parser token '}'
2.	<source>:5:25: lambda expression parsing
3.	<source>:5:58: in compound statement ('{}')
 #0 0x0000000003b258d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b258d8)
 #1 0x0000000003b2359c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b2359c)
 #2 0x0000000003a6c198 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b4972c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b4972c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b4972c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b4972c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b4972c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b4972c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006b5985e clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5985e)
#10 0x0000000006b59e48 MarkVarDeclODRUsed(clang::ValueDecl*, clang::SourceLocation, clang::Sema&, unsigned int const*) SemaExpr.cpp:0:0
#11 0x0000000006b5b302 DoMarkVarDeclReferenced(clang::Sema&, clang::SourceLocation, clang::VarDecl*, clang::Expr*, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
#12 0x0000000006b87b3e MarkExprReferenced(clang::Sema&, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap<clang::VarDecl const*, int, llvm::DenseMapInfo<clang::VarDecl const*, void>, llvm::detail::DenseMapPair<clang::VarDecl const*, int>>&) SemaExpr.cpp:0:0
#13 0x0000000006cdce49 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformDeclRefExpr(clang::DeclRefExpr*) SemaExprCXX.cpp:0:0
#14 0x0000000006cf1d18 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformBinaryOperator(clang::BinaryOperator*) SemaExprCXX.cpp:0:0
#15 0x0000000006cd6530 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformCondition(clang::SourceLocation, clang::VarDecl*, clang::Expr*, clang::Sema::ConditionKind) SemaExprCXX.cpp:0:0
#16 0x0000000006cf6046 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformIfStmt(clang::IfStmt*) SemaExprCXX.cpp:0:0
#17 0x0000000006cf6524 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaExprCXX.cpp:0:0
#18 0x0000000006cd54cf clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformLambdaExpr(clang::LambdaExpr*) SemaExprCXX.cpp:0:0
#19 0x0000000006cde858 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaExprCXX.cpp:0:0
#20 0x0000000006cde546 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaExprCXX.cpp:0:0
#21 0x0000000006ceec51 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformCallExpr(clang::CallExpr*) SemaExprCXX.cpp:0:0
#22 0x0000000006cf13c3 clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr*) SemaExprCXX.cpp:0:0
#23 0x0000000006ceebde clang::TreeTransform<clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>)::TyposReplace>::TransformCallExpr(clang::CallExpr*) SemaExprCXX.cpp:0:0
#24 0x0000000006ccb03d clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccb03d)
#25 0x0000000006ccb3bd clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccb3bd)
#26 0x000000000703eade clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x703eade)
#27 0x00000000066b3e24 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3e24)
#28 0x00000000066aa870 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66aa870)
#29 0x00000000066ab7a0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ab7a0)
#30 0x00000000066ac679 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ac679)
#31 0x000000000664c5d2 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664c5d2)
#32 0x000000000664d6ab clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664d6ab)
#33 0x000000000662d6d5 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662d6d5)
#34 0x000000000662e08a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e08a)
#35 0x000000000662e229 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e229)
#36 0x000000000662e58e clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e58e)
#37 0x0000000006632c1a clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6632c1a)
#38 0x000000000662bf31 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662bf31)
#39 0x000000000662e08a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e08a)
#40 0x000000000662e229 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e229)
#41 0x00000000065e79d6 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e79d6)
#42 0x00000000065f696b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f696b)
#43 0x00000000065b59be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b59be)
#44 0x00000000065b616e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b616e)
#45 0x00000000065bc6f7 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bc6f7)
#46 0x00000000065bd5cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bd5cd)
#47 0x00000000065b0bba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b0bba)
#48 0x0000000004470d68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4470d68)
#49 0x0000000004702ca9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4702ca9)
#50 0x000000000468655e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x468655e)
#51 0x00000000047ec0ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47ec0ce)
#52 0x0000000000cb996f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcb996f)
#53 0x0000000000cb2bba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#54 0x00000000044b4189 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#55 0x0000000003a6c644 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a6c644)
#56 0x00000000044b477f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#57 0x000000000447a6c5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447a6c5)
#58 0x000000000447b12d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447b12d)
#59 0x0000000004482d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4482d75)
#60 0x0000000000cb6d65 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcb6d65)
#61 0x0000000000b89614 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb89614)
#62 0x00007b4972c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#63 0x00007b4972c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#64 0x0000000000cb266e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcb266e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Linrz (Strke)

<details>
Reproducer: https://gcc.godbolt.org/z/dnoKqaofo 
```
#include &lt;iostream&gt;
#include &lt;algorithm&gt;
#include &lt;array&gt;

auto getCache = getPool([&amp;](bool *const result, tms&amp; ts) {
  std::thread([=] {
    tms time = std::chrono::system_clock::now();
    std::this_thread::sleep_for(ts);
    if (time &gt;= ts) {
        *result = true;

    } else {
        *result = false;
    }
  }).detach();
});

```
and get the result
```
clang++: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:291: T&amp; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::operator[](llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type) [with T = clang::sema::FunctionScopeInfo*; &lt;template-parameter-1-2&gt; = void; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::reference = clang::sema::FunctionScopeInfo*&amp;; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type = long unsigned int]: Assertion `idx &lt; size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 &lt;source&gt;
1.	&lt;source&gt;:16:1: current parser token '}'
2.	&lt;source&gt;:5:25: lambda expression parsing
3.	&lt;source&gt;:5:58: in compound statement ('{}')
 #<!-- -->0 0x0000000003b258d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b258d8)
 #<!-- -->1 0x0000000003b2359c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b2359c)
 #<!-- -->2 0x0000000003a6c198 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b4972c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b4972c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b4972c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b4972c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b4972c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b4972c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006b5985e clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&amp;, clang::QualType&amp;, unsigned int const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5985e)
#<!-- -->10 0x0000000006b59e48 MarkVarDeclODRUsed(clang::ValueDecl*, clang::SourceLocation, clang::Sema&amp;, unsigned int const*) SemaExpr.cpp:0:0
#<!-- -->11 0x0000000006b5b302 DoMarkVarDeclReferenced(clang::Sema&amp;, clang::SourceLocation, clang::VarDecl*, clang::Expr*, llvm::DenseMap&lt;clang::VarDecl const*, int, llvm::DenseMapInfo&lt;clang::VarDecl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::VarDecl const*, int&gt;&gt;&amp;) SemaExpr.cpp:0:0
#<!-- -->12 0x0000000006b87b3e MarkExprReferenced(clang::Sema&amp;, clang::SourceLocation, clang::Decl*, clang::Expr*, bool, llvm::DenseMap&lt;clang::VarDecl const*, int, llvm::DenseMapInfo&lt;clang::VarDecl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::VarDecl const*, int&gt;&gt;&amp;) SemaExpr.cpp:0:0
#<!-- -->13 0x0000000006cdce49 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformDeclRefExpr(clang::DeclRefExpr*) SemaExprCXX.cpp:0:0
#<!-- -->14 0x0000000006cf1d18 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformBinaryOperator(clang::BinaryOperator*) SemaExprCXX.cpp:0:0
#<!-- -->15 0x0000000006cd6530 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformCondition(clang::SourceLocation, clang::VarDecl*, clang::Expr*, clang::Sema::ConditionKind) SemaExprCXX.cpp:0:0
#<!-- -->16 0x0000000006cf6046 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformIfStmt(clang::IfStmt*) SemaExprCXX.cpp:0:0
#<!-- -->17 0x0000000006cf6524 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaExprCXX.cpp:0:0
#<!-- -->18 0x0000000006cd54cf clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaExprCXX.cpp:0:0
#<!-- -->19 0x0000000006cde858 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaExprCXX.cpp:0:0
#<!-- -->20 0x0000000006cde546 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaExprCXX.cpp:0:0
#<!-- -->21 0x0000000006ceec51 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformCallExpr(clang::CallExpr*) SemaExprCXX.cpp:0:0
#<!-- -->22 0x0000000006cf13c3 clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr*) SemaExprCXX.cpp:0:0
#<!-- -->23 0x0000000006ceebde clang::TreeTransform&lt;clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;)::TyposReplace&gt;::TransformCallExpr(clang::CallExpr*) SemaExprCXX.cpp:0:0
#<!-- -->24 0x0000000006ccb03d clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccb03d)
#<!-- -->25 0x0000000006ccb3bd clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccb3bd)
#<!-- -->26 0x000000000703eade clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x703eade)
#<!-- -->27 0x00000000066b3e24 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3e24)
#<!-- -->28 0x00000000066aa870 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66aa870)
#<!-- -->29 0x00000000066ab7a0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ab7a0)
#<!-- -->30 0x00000000066ac679 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ac679)
#<!-- -->31 0x000000000664c5d2 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664c5d2)
#<!-- -->32 0x000000000664d6ab clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664d6ab)
#<!-- -->33 0x000000000662d6d5 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662d6d5)
#<!-- -->34 0x000000000662e08a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e08a)
#<!-- -->35 0x000000000662e229 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e229)
#<!-- -->36 0x000000000662e58e clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e58e)
#<!-- -->37 0x0000000006632c1a clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6632c1a)
#<!-- -->38 0x000000000662bf31 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662bf31)
#<!-- -->39 0x000000000662e08a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e08a)
#<!-- -->40 0x000000000662e229 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e229)
#<!-- -->41 0x00000000065e79d6 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e79d6)
#<!-- -->42 0x00000000065f696b clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f696b)
#<!-- -->43 0x00000000065b59be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b59be)
#<!-- -->44 0x00000000065b616e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b616e)
#<!-- -->45 0x00000000065bc6f7 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bc6f7)
#<!-- -->46 0x00000000065bd5cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bd5cd)
#<!-- -->47 0x00000000065b0bba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b0bba)
#<!-- -->48 0x0000000004470d68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4470d68)
#<!-- -->49 0x0000000004702ca9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4702ca9)
#<!-- -->50 0x000000000468655e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x468655e)
#<!-- -->51 0x00000000047ec0ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47ec0ce)
#<!-- -->52 0x0000000000cb996f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcb996f)
#<!-- -->53 0x0000000000cb2bba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->54 0x00000000044b4189 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->55 0x0000000003a6c644 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a6c644)
#<!-- -->56 0x00000000044b477f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->57 0x000000000447a6c5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447a6c5)
#<!-- -->58 0x000000000447b12d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447b12d)
#<!-- -->59 0x0000000004482d75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4482d75)
#<!-- -->60 0x0000000000cb6d65 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcb6d65)
#<!-- -->61 0x0000000000b89614 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb89614)
#<!-- -->62 0x00007b4972c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->63 0x00007b4972c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->64 0x0000000000cb266e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcb266e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

confirmed

---

### Comment 3 - MagentaTreehouse

Reduced:
```c++
struct tm {};

auto getCache = [&] {
  ::foo([=] {
    tms time;
    (void)time;
  });
};
```
See Compiler Explorer: https://gcc.godbolt.org/z/ebre7G7q5

---

