# [clang[OpenACC] crash with -fopenacc option: void *a = ^ { static int b };

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/154008
**Status:** Closed
**Labels:** clang:frontend, regression, crash, clang:openacc
**Closed Date:** 2025-08-18T14:37:48Z

## Body

The title says everything:
https://godbolt.org/z/bGqK59En5

Backtrace:
```console
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionDecl; From = clang::DeclContext]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenacc <source>
1.	<source>:1:28: current parser token '}'
2.	<source>:1:11: block literal parsing
3.	<source>:1:13: in compound statement ('{}')
 #0 0x0000000004028478 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4028478)
 #1 0x00000000040258a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x40258a4)
 #2 0x0000000003f6a0d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076a0a7c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076a0a7c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076a0a7c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076a0a7c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076a0a7c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076a0a7c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006e24b16 clang::SemaOpenACC::ActOnVariableDeclarator(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6e24b16)
#10 0x0000000006aa520c clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, llvm::ArrayRef<clang::BindingDecl*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6aa520c)
#11 0x0000000006ac5f8f clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ac5f8f)
#12 0x0000000006ac68d0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ac68d0)
#13 0x0000000006712be3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6712be3)
#14 0x0000000006722099 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6722099)
#15 0x000000000672ad5f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672ad5f)
#16 0x000000000672b22c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672b22c)
#17 0x00000000067ecaa2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67ecaa2)
#18 0x00000000067ed779 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67ed779)
#19 0x00000000067f58cb clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67f58cb)
#20 0x0000000006755eea clang::Parser::ParseBlockLiteralExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6755eea)
#21 0x0000000006759f6a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6759f6a)
#22 0x000000000675b697 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x675b697)
#23 0x000000000675b729 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x675b729)
#24 0x0000000006713638 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6713638)
#25 0x0000000006722099 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6722099)
#26 0x00000000066df3fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66df3fe)
#27 0x00000000066dfb9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66dfb9f)
#28 0x00000000066e79fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66e79fa)
#29 0x00000000066e8995 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66e8995)
#30 0x00000000066e8e50 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66e8e50)
#31 0x00000000066da4c3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66da4c3)
#32 0x00000000049ca7e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49ca7e8)
#33 0x0000000004cbedb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cbedb5)
#34 0x0000000004c3af5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c3af5e)
#35 0x0000000004db06d1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4db06d1)
#36 0x0000000000db082f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdb082f)
#37 0x0000000000da74da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#38 0x0000000004a321d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x0000000003f6a574 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f6a574)
#40 0x0000000004a327ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x00000000049f4a8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49f4a8d)
#42 0x00000000049f5b1e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49f5b1e)
#43 0x00000000049fd565 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49fd565)
#44 0x0000000000dacce5 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdacce5)
#45 0x0000000000c60ed4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc60ed4)
#46 0x000076a0a7c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x000076a0a7c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000da6f85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda6f85)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
The title says everything:
https://godbolt.org/z/bGqK59En5

Backtrace:
```console
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionDecl; From = clang::DeclContext]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenacc &lt;source&gt;
1.	&lt;source&gt;:1:28: current parser token '}'
2.	&lt;source&gt;:1:11: block literal parsing
3.	&lt;source&gt;:1:13: in compound statement ('{}')
 #<!-- -->0 0x0000000004028478 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4028478)
 #<!-- -->1 0x00000000040258a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x40258a4)
 #<!-- -->2 0x0000000003f6a0d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000076a0a7c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000076a0a7c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000076a0a7c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000076a0a7c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076a0a7c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000076a0a7c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006e24b16 clang::SemaOpenACC::ActOnVariableDeclarator(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6e24b16)
#<!-- -->10 0x0000000006aa520c clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;, llvm::ArrayRef&lt;clang::BindingDecl*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6aa520c)
#<!-- -->11 0x0000000006ac5f8f clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ac5f8f)
#<!-- -->12 0x0000000006ac68d0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ac68d0)
#<!-- -->13 0x0000000006712be3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6712be3)
#<!-- -->14 0x0000000006722099 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6722099)
#<!-- -->15 0x000000000672ad5f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672ad5f)
#<!-- -->16 0x000000000672b22c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672b22c)
#<!-- -->17 0x00000000067ecaa2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67ecaa2)
#<!-- -->18 0x00000000067ed779 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67ed779)
#<!-- -->19 0x00000000067f58cb clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67f58cb)
#<!-- -->20 0x0000000006755eea clang::Parser::ParseBlockLiteralExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6755eea)
#<!-- -->21 0x0000000006759f6a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6759f6a)
#<!-- -->22 0x000000000675b697 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x675b697)
#<!-- -->23 0x000000000675b729 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x675b729)
#<!-- -->24 0x0000000006713638 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6713638)
#<!-- -->25 0x0000000006722099 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6722099)
#<!-- -->26 0x00000000066df3fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66df3fe)
#<!-- -->27 0x00000000066dfb9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66dfb9f)
#<!-- -->28 0x00000000066e79fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66e79fa)
#<!-- -->29 0x00000000066e8995 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66e8995)
#<!-- -->30 0x00000000066e8e50 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66e8e50)
#<!-- -->31 0x00000000066da4c3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66da4c3)
#<!-- -->32 0x00000000049ca7e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49ca7e8)
#<!-- -->33 0x0000000004cbedb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cbedb5)
#<!-- -->34 0x0000000004c3af5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c3af5e)
#<!-- -->35 0x0000000004db06d1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4db06d1)
#<!-- -->36 0x0000000000db082f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdb082f)
#<!-- -->37 0x0000000000da74da ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000004a321d9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->39 0x0000000003f6a574 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f6a574)
#<!-- -->40 0x0000000004a327ef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->41 0x00000000049f4a8d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49f4a8d)
#<!-- -->42 0x00000000049f5b1e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49f5b1e)
#<!-- -->43 0x00000000049fd565 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49fd565)
#<!-- -->44 0x0000000000dacce5 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdacce5)
#<!-- -->45 0x0000000000c60ed4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc60ed4)
#<!-- -->46 0x000076a0a7c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->47 0x000076a0a7c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->48 0x0000000000da6f85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda6f85)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Crash is trunk only:
https://godbolt.org/z/8ze3193Mr

---

