# Clang crashes when capturing the same variable by copy and reference in a lambda since clang 18

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/138824
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-05-08T03:22:13Z

## Body

Defining a lambda that attempts to capture the same local variable both by copy and by reference (e.g., [a, &a]) results in an Internal Compiler Error (ICE) in Clang trunk with assertions enabled.

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:16699: bool EvaluateInPlace(clang::APValue&, {anonymous}::EvalInfo&, const {anonymous}::LValue&, const clang::Expr*, bool): Assertion `!E->isValueDependent()' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:7:2: current parser token ';'
2.	<source>:3:1: parsing function body 'main'
3.	<source>:3:1: in compound statement ('{}')
 #0 0x0000000003f76d78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f76d78)
 #1 0x0000000003f74a04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f74a04)
 #2 0x0000000003eb98c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ce594a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ce594a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ce594a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ce594a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ce594a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ce594a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007892fe7 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#10 0x00000000078d04df clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x00000000078d1208 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#12 0x0000000007892f51 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) ExprConstant.cpp:0:0
#13 0x00000000078d6860 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78d6860)
#14 0x00000000076f3f42 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76f3f42)
#15 0x00000000076f42f1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76f42f1)
#16 0x0000000006998b8b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6998b8b)
#17 0x000000000699fd02 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699fd02)
#18 0x0000000006620d3a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6620d3a)
#19 0x00000000066305b9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66305b9)
#20 0x0000000006639277 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6639277)
#21 0x0000000006639739 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6639739)
#22 0x00000000066e9d62 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e9d62)
#23 0x00000000066eaa6d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66eaa6d)
#24 0x00000000066f2783 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f2783)
#25 0x00000000066f2f4a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f2f4a)
#26 0x00000000065fa913 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fa913)
#27 0x0000000006630fbd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6630fbd)
#28 0x00000000065ee44e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee44e)
#29 0x00000000065eec09 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65eec09)
#30 0x00000000065f652a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f652a)
#31 0x00000000065f74cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f74cd)
#32 0x00000000065e98aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e98aa)
#33 0x0000000004918f78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4918f78)
#34 0x0000000004c0c235 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0c235)
#35 0x0000000004b8b83e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b8b83e)
#36 0x0000000004d000e9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d000e9)
#37 0x0000000000da766f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda766f)
#38 0x0000000000d9d83a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#39 0x000000000497f839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x0000000003eb9d64 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb9d64)
#41 0x000000000497fe4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x000000000494229d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x494229d)
#43 0x000000000494332e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x494332e)
#44 0x000000000494b185 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x494b185)
#45 0x0000000000da35d8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda35d8)
#46 0x0000000000c28a64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc28a64)
#47 0x00007ce594a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x00007ce594a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000d9d2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d2e5)
```

## Program

```
#include <iostream>
int main()
{
int a = 1000;
static auto f =
[a, &a] {
};
}
```

## To quickly reproduce

Happens since version 18

https://gcc.godbolt.org/z/WKehz5bda



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
Defining a lambda that attempts to capture the same local variable both by copy and by reference (e.g., [a, &amp;a]) results in an Internal Compiler Error (ICE) in Clang trunk with assertions enabled.

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:16699: bool EvaluateInPlace(clang::APValue&amp;, {anonymous}::EvalInfo&amp;, const {anonymous}::LValue&amp;, const clang::Expr*, bool): Assertion `!E-&gt;isValueDependent()' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:7:2: current parser token ';'
2.	&lt;source&gt;:3:1: parsing function body 'main'
3.	&lt;source&gt;:3:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003f76d78 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f76d78)
 #<!-- -->1 0x0000000003f74a04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f74a04)
 #<!-- -->2 0x0000000003eb98c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ce594a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007ce594a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007ce594a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007ce594a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007ce594a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007ce594a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007892fe7 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->10 0x00000000078d04df clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::RecordExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->11 0x00000000078d1208 EvaluateRecord(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007892f51 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000078d6860 clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78d6860)
#<!-- -->14 0x00000000076f3f42 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76f3f42)
#<!-- -->15 0x00000000076f42f1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76f42f1)
#<!-- -->16 0x0000000006998b8b clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6998b8b)
#<!-- -->17 0x000000000699fd02 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699fd02)
#<!-- -->18 0x0000000006620d3a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6620d3a)
#<!-- -->19 0x00000000066305b9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66305b9)
#<!-- -->20 0x0000000006639277 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6639277)
#<!-- -->21 0x0000000006639739 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6639739)
#<!-- -->22 0x00000000066e9d62 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e9d62)
#<!-- -->23 0x00000000066eaa6d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66eaa6d)
#<!-- -->24 0x00000000066f2783 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f2783)
#<!-- -->25 0x00000000066f2f4a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f2f4a)
#<!-- -->26 0x00000000065fa913 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fa913)
#<!-- -->27 0x0000000006630fbd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6630fbd)
#<!-- -->28 0x00000000065ee44e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee44e)
#<!-- -->29 0x00000000065eec09 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65eec09)
#<!-- -->30 0x00000000065f652a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f652a)
#<!-- -->31 0x00000000065f74cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f74cd)
#<!-- -->32 0x00000000065e98aa clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e98aa)
#<!-- -->33 0x0000000004918f78 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4918f78)
#<!-- -->34 0x0000000004c0c235 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0c235)
#<!-- -->35 0x0000000004b8b83e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b8b83e)
#<!-- -->36 0x0000000004d000e9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d000e9)
#<!-- -->37 0x0000000000da766f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda766f)
#<!-- -->38 0x0000000000d9d83a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x000000000497f839 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x0000000003eb9d64 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb9d64)
#<!-- -->41 0x000000000497fe4f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x000000000494229d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x494229d)
#<!-- -->43 0x000000000494332e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x494332e)
#<!-- -->44 0x000000000494b185 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x494b185)
#<!-- -->45 0x0000000000da35d8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda35d8)
#<!-- -->46 0x0000000000c28a64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc28a64)
#<!-- -->47 0x00007ce594a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007ce594a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000d9d2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d2e5)
```

## Program

```
#include &lt;iostream&gt;
int main()
{
int a = 1000;
static auto f =
[a, &amp;a] {
};
}
```

## To quickly reproduce

Happens since version 18

https://gcc.godbolt.org/z/WKehz5bda


</details>


---

