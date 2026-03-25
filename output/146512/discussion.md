# [clang] ICE with large constexpr array of std::variant triggers LLVM ERROR: out of memory

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/146512
**Status:** Closed
**Labels:** duplicate, clang:frontend, confirmed, crash, constexpr
**Closed Date:** 2025-07-02T21:00:12Z

## Body

When defining a very large array of std::variant elements as a constexpr local variable, Clang triggers an internal failure:

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<eof> parser at end of file
2.	<source>:2:12: parsing function body 'main'
 #0 0x0000000003f78848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f78848)
 #1 0x0000000003f764d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f764d4)
 #2 0x0000000003ebb048 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079f5a0c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000079f5a0c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000079f5a0c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000079f5a0c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003ec685b (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ec685b)
 #8 0x0000000003ec6875 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ec6875)
 #9 0x000079f5a10ae9a3 operator new(unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0xae9a3)
#10 0x000000000748b1dc clang::APValue::MakeArray(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x748b1dc)
#11 0x00000000078a4205 (anonymous namespace)::ArrayExprEvaluator::VisitCXXConstructExpr(clang::CXXConstructExpr const*, (anonymous namespace)::LValue const&, clang::APValue*, clang::QualType) ExprConstant.cpp:0:0
#12 0x00000000078a69fd clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#13 0x00000000078a869d EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#14 0x000000000786a280 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#15 0x00000000078ad3c1 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78ad3c1)
#16 0x00000000076cad42 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76cad42)
#17 0x00000000076caeba clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76caeba)
#18 0x00000000069d1249 clang::Sema::DiagnoseUnusedDecl(clang::NamedDecl const*, llvm::function_ref<void (clang::SourceLocation, clang::PartialDiagnostic)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d1249)
#19 0x00000000069d2d0d clang::Sema::ActOnPopScope(clang::SourceLocation, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d2d0d)
#20 0x0000000006625815 clang::Parser::ExitScope() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6625815)
#21 0x000000000673b351 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673b351)
#22 0x0000000006633bd3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6633bd3)
#23 0x000000000666987d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666987d)
#24 0x00000000066276be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66276be)
#25 0x0000000006627e69 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6627e69)
#26 0x000000000662f7aa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662f7aa)
#27 0x000000000663077d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663077d)
#28 0x0000000006622aea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6622aea)
#29 0x000000000492ed48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492ed48)
#30 0x0000000004c28c65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c28c65)
#31 0x0000000004ba5ffe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba5ffe)
#32 0x0000000004d1b441 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d1b441)
#33 0x0000000000d8e81f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8e81f)
#34 0x0000000000d853fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x0000000004995b49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003ebb4e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebb4e4)
#37 0x000000000499615f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x0000000004957f8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4957f8d)
#39 0x000000000495901e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495901e)
#40 0x00000000049614d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49614d5)
#41 0x0000000000d8accf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8accf)
#42 0x0000000000c41924 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc41924)
#43 0x000079f5a0c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x000079f5a0c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000d84ea5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd84ea5)
```

## Program

```
#include <variant>
int main() {
    constexpr int size = 1000000000;
    std::variant<int, int, double, char> numbers[size];
    return 0;
}
```

To quickly reproduce:

https://godbolt.org/z/M1MnrGWsh

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
When defining a very large array of std::variant elements as a constexpr local variable, Clang triggers an internal failure:

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:2:12: parsing function body 'main'
 #<!-- -->0 0x0000000003f78848 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f78848)
 #<!-- -->1 0x0000000003f764d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f764d4)
 #<!-- -->2 0x0000000003ebb048 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079f5a0c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000079f5a0c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000079f5a0c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000079f5a0c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003ec685b (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ec685b)
 #<!-- -->8 0x0000000003ec6875 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ec6875)
 #<!-- -->9 0x000079f5a10ae9a3 operator new(unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0xae9a3)
#<!-- -->10 0x000000000748b1dc clang::APValue::MakeArray(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x748b1dc)
#<!-- -->11 0x00000000078a4205 (anonymous namespace)::ArrayExprEvaluator::VisitCXXConstructExpr(clang::CXXConstructExpr const*, (anonymous namespace)::LValue const&amp;, clang::APValue*, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000078a69fd clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000078a869d EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->14 0x000000000786a280 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000078ad3c1 clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78ad3c1)
#<!-- -->16 0x00000000076cad42 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76cad42)
#<!-- -->17 0x00000000076caeba clang::VarDecl::evaluateValue() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76caeba)
#<!-- -->18 0x00000000069d1249 clang::Sema::DiagnoseUnusedDecl(clang::NamedDecl const*, llvm::function_ref&lt;void (clang::SourceLocation, clang::PartialDiagnostic)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d1249)
#<!-- -->19 0x00000000069d2d0d clang::Sema::ActOnPopScope(clang::SourceLocation, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d2d0d)
#<!-- -->20 0x0000000006625815 clang::Parser::ExitScope() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6625815)
#<!-- -->21 0x000000000673b351 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673b351)
#<!-- -->22 0x0000000006633bd3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6633bd3)
#<!-- -->23 0x000000000666987d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666987d)
#<!-- -->24 0x00000000066276be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66276be)
#<!-- -->25 0x0000000006627e69 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6627e69)
#<!-- -->26 0x000000000662f7aa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662f7aa)
#<!-- -->27 0x000000000663077d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663077d)
#<!-- -->28 0x0000000006622aea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6622aea)
#<!-- -->29 0x000000000492ed48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492ed48)
#<!-- -->30 0x0000000004c28c65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c28c65)
#<!-- -->31 0x0000000004ba5ffe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba5ffe)
#<!-- -->32 0x0000000004d1b441 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d1b441)
#<!-- -->33 0x0000000000d8e81f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8e81f)
#<!-- -->34 0x0000000000d853fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004995b49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003ebb4e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebb4e4)
#<!-- -->37 0x000000000499615f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x0000000004957f8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4957f8d)
#<!-- -->39 0x000000000495901e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x495901e)
#<!-- -->40 0x00000000049614d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49614d5)
#<!-- -->41 0x0000000000d8accf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8accf)
#<!-- -->42 0x0000000000c41924 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc41924)
#<!-- -->43 0x000079f5a0c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x000079f5a0c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000d84ea5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd84ea5)
```

## Program

```
#include &lt;variant&gt;
int main() {
    constexpr int size = 1000000000;
    std::variant&lt;int, int, double, char&gt; numbers[size];
    return 0;
}
```

To quickly reproduce:

https://godbolt.org/z/M1MnrGWsh
</details>


---

### Comment 2 - shafik

We can definitely do better here: https://godbolt.org/z/hjsKso3rj

edg also fails but gcc accepts and MSVC diagnoses b/c the array is too large.

---

