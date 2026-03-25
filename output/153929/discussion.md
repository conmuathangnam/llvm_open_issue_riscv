# [clang] crash on function parameter with VLAs and _Countof

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/153929
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2025-08-16T10:31:49Z

## Body

Reproducer:
https://godbolt.org/z/cqxn3aP6W
```c
void foo (char (*a)[*][2], int (*x)[_Countof (*a)]);
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:51: current parser token ')'
 #0 0x0000000003cf8cd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cf8cd8)
 #1 0x0000000003cf66ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cf66ac)
 #2 0x0000000003c46938 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c38aee42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000074f1074 CheckICE(clang::Expr const*, clang::ASTContext const&) ExprConstant.cpp:0:0
 #5 0x00000000074f11e7 CheckICE(clang::Expr const*, clang::ASTContext const&) ExprConstant.cpp:0:0
 #6 0x00000000074f1b2b clang::Expr::isIntegerConstantExpr(clang::ASTContext const&) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x74f1b2b)
 #7 0x000000000683b9cb clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::AllowFoldKind) (/opt/compiler-explorer/clang-trunk/bin/clang+0x683b9cb)
 #8 0x0000000006f0532a clang::Sema::BuildArrayType(clang::QualType, clang::ArraySizeModifier, clang::Expr*, unsigned int, clang::SourceRange, clang::DeclarationName) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6f0532a)
 #9 0x0000000006f1ba8d GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#10 0x0000000006f22694 clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6f22694)
#11 0x0000000006687907 clang::Sema::ActOnParamDeclarator(clang::Scope*, clang::Declarator&, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6687907)
#12 0x00000000063248ee clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&, llvm::SmallVectorImpl<clang::DeclaratorChunk::ParamInfo>&, clang::SourceLocation&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x63248ee)
#13 0x00000000063267a3 clang::Parser::ParseFunctionDeclarator(clang::Declarator&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x63267a3)
#14 0x000000000632803f clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x632803f)
#15 0x0000000006320cfe clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6320cfe)
#16 0x0000000007ae1d31 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x7ae1d31)
#17 0x000000000631ca94 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x631ca94)
#18 0x00000000062c90df clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62c90df)
#19 0x00000000062c9ed7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62c9ed7)
#20 0x00000000062d2b1f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62d2b1f)
#21 0x00000000062d4317 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62d4317)
#22 0x00000000062d4860 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62d4860)
#23 0x00000000062c2933 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62c2933)
#24 0x00000000046405a5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x46405a5)
#25 0x000000000494c26a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x494c26a)
#26 0x00000000048c57cb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x48c57cb)
#27 0x0000000004a3cff3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4a3cff3)
#28 0x0000000000dc0df5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdc0df5)
#29 0x0000000000db8d9d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x00000000046b2839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003c46d53 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c46d53)
#32 0x00000000046b2a59 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000004675ded clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4675ded)
#34 0x0000000004676da1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4676da1)
#35 0x0000000004680bec clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4680bec)
#36 0x0000000000dbd601 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdbd601)
#37 0x0000000000c6c3f4 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc6c3f4)
#38 0x00007c38aee29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007c38aee29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000db8835 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdb8835)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/cqxn3aP6W
```c
void foo (char (*a)[*][2], int (*x)[_Countof (*a)]);
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:51: current parser token ')'
 #<!-- -->0 0x0000000003cf8cd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cf8cd8)
 #<!-- -->1 0x0000000003cf66ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3cf66ac)
 #<!-- -->2 0x0000000003c46938 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c38aee42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000074f1074 CheckICE(clang::Expr const*, clang::ASTContext const&amp;) ExprConstant.cpp:0:0
 #<!-- -->5 0x00000000074f11e7 CheckICE(clang::Expr const*, clang::ASTContext const&amp;) ExprConstant.cpp:0:0
 #<!-- -->6 0x00000000074f1b2b clang::Expr::isIntegerConstantExpr(clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x74f1b2b)
 #<!-- -->7 0x000000000683b9cb clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&amp;, clang::AllowFoldKind) (/opt/compiler-explorer/clang-trunk/bin/clang+0x683b9cb)
 #<!-- -->8 0x0000000006f0532a clang::Sema::BuildArrayType(clang::QualType, clang::ArraySizeModifier, clang::Expr*, unsigned int, clang::SourceRange, clang::DeclarationName) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6f0532a)
 #<!-- -->9 0x0000000006f1ba8d GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&amp;, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#<!-- -->10 0x0000000006f22694 clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6f22694)
#<!-- -->11 0x0000000006687907 clang::Sema::ActOnParamDeclarator(clang::Scope*, clang::Declarator&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6687907)
#<!-- -->12 0x00000000063248ee clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&amp;, llvm::SmallVectorImpl&lt;clang::DeclaratorChunk::ParamInfo&gt;&amp;, clang::SourceLocation&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x63248ee)
#<!-- -->13 0x00000000063267a3 clang::Parser::ParseFunctionDeclarator(clang::Declarator&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x63267a3)
#<!-- -->14 0x000000000632803f clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x632803f)
#<!-- -->15 0x0000000006320cfe clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/opt/compiler-explorer/clang-trunk/bin/clang+0x6320cfe)
#<!-- -->16 0x0000000007ae1d31 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x7ae1d31)
#<!-- -->17 0x000000000631ca94 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x631ca94)
#<!-- -->18 0x00000000062c90df clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62c90df)
#<!-- -->19 0x00000000062c9ed7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62c9ed7)
#<!-- -->20 0x00000000062d2b1f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62d2b1f)
#<!-- -->21 0x00000000062d4317 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62d4317)
#<!-- -->22 0x00000000062d4860 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62d4860)
#<!-- -->23 0x00000000062c2933 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x62c2933)
#<!-- -->24 0x00000000046405a5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x46405a5)
#<!-- -->25 0x000000000494c26a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x494c26a)
#<!-- -->26 0x00000000048c57cb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x48c57cb)
#<!-- -->27 0x0000000004a3cff3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4a3cff3)
#<!-- -->28 0x0000000000dc0df5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdc0df5)
#<!-- -->29 0x0000000000db8d9d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->30 0x00000000046b2839 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003c46d53 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c46d53)
#<!-- -->32 0x00000000046b2a59 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x0000000004675ded clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4675ded)
#<!-- -->34 0x0000000004676da1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x4676da1)
#<!-- -->35 0x0000000004680bec clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4680bec)
#<!-- -->36 0x0000000000dbd601 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xdbd601)
#<!-- -->37 0x0000000000c6c3f4 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc6c3f4)
#<!-- -->38 0x00007c38aee29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007c38aee29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000db8835 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdb8835)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - k-arrows

Looks like this is a regression in trunk:
https://godbolt.org/z/ch5afPv7G

---

### Comment 3 - Mr-Anyone

I think this is a duplicate of #152826 

Also, I don't think _Countof is supported in clang 20 judging from the error message. 

---

### Comment 4 - k-arrows

I will close this issue since it is a duplicate of another issue.

---

### Comment 5 - k-arrows

Here is the original reproducer:
https://github.com/gcc-mirror/gcc/blob/master/gcc/testsuite/gcc.dg/countof-vla.c

---

