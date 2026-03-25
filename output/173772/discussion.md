# Clang assertion failed in SmallVectorTemplateCommon when lambda with capture-default in default argument captures function parameter

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173772

## Body

### Link for quick verification: https://godbolt.org/z/7z8v3sGTq
```c
class Node {
    static void doSomething(
        int z,
        auto g = [&](int i){ z }
    ) {}
};
```
Go back to `clang 17.0.1 (assertions)`.

- Clang crashes when a lambda expression appearing in a default argument of a class member function uses a capture-default and references a function parameter.

- Although the code is ill-formed. Clang correctly emits diagnostics but then proceeds into lambda capture analysis and crashes in `Sema::tryCaptureVariable` due to the absence of a function scope.

- The ICE only occurs when the lambda body actually references an entity that requires capture (e.g. the function parameter). Removing the reference avoids the crash.
## Stack dump
```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:295: T& llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::operator[](llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type) [with T = clang::sema::FunctionScopeInfo*; <template-parameter-1-2> = void; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::reference = clang::sema::FunctionScopeInfo*&; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type = long unsigned int]: Assertion `idx < size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:4:32: current parser token '}'
2.	<source>:1:1: parsing struct/union/class body 'Node'
3.	<source>:4:18: lambda expression parsing
4.	<source>:4:28: in compound statement ('{}')
 #0 0x00000000042ef648 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ef648)
 #1 0x00000000042eca74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eca74)
 #2 0x0000000004231818 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078be1c042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078be1c0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078be1c042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078be1c0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078be1c02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000078be1c039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006f6c45e clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f6c45e)
#10 0x0000000006f6c949 clang::Sema::getCapturedDeclRefType(clang::ValueDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f6c949)
#11 0x0000000006f74bdc clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f74bdc)
#12 0x0000000006f74f66 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::LookupResult&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f74f66)
#13 0x0000000006da3989 clang::Sema::ActOnNameClassifiedAsNonType(clang::Scope*, clang::CXXScopeSpec const&, clang::NamedDecl*, clang::SourceLocation, clang::Token const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6da3989)
#14 0x0000000006ab85ae clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab85ae)
#15 0x0000000006a9c0d2 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9c0d2)
#16 0x0000000006a9dcc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dcc7)
#17 0x0000000006a9dd59 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dd59)
#18 0x0000000006aa2339 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa2339)
#19 0x0000000006b27bf9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b27bf9)
#20 0x0000000006b1f0a4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1f0a4)
#21 0x0000000006b1fa86 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1fa86)
#22 0x0000000006b283ce clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b283ce)
#23 0x0000000006abbba2 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abbba2)
#24 0x0000000006abcccb clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abcccb)
#25 0x0000000006a9d2a5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9d2a5)
#26 0x0000000006a9dcc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dcc7)
#27 0x0000000006a9dd59 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dd59)
#28 0x0000000006a482b0 clang::Parser::ParseLexedMethodDeclaration(clang::Parser::LateParsedMethodDeclaration&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a482b0)
#29 0x0000000006a3d24d clang::Parser::ParseLexedMethodDeclarations(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3d24d)
#30 0x0000000006a913a9 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a913a9)
#31 0x0000000006a93e30 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a93e30)
#32 0x0000000006a66c5e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a66c5e)
#33 0x0000000006a21a57 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a21a57)
#34 0x0000000006a224cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a224cf)
#35 0x0000000006a2a2fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2a2fa)
#36 0x0000000006a2b295 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b295)
#37 0x0000000006a2b740 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b740)
#38 0x0000000006a0c2e3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0c2e3)
#39 0x0000000004c50978 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c50978)
#40 0x0000000004f46ff5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f46ff5)
#41 0x0000000004ec728e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec728e)
#42 0x000000000504137d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x504137d)
#43 0x0000000000de5c4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5c4c)
#44 0x0000000000ddc69a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#45 0x0000000000ddc81d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#46 0x0000000004cc3049 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#47 0x0000000004231cb4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231cb4)
#48 0x0000000004cc365f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#49 0x0000000004c84142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c84142)
#50 0x0000000004c850ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c850ee)
#51 0x0000000004c8c545 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8c545)
#52 0x0000000000de2041 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2041)
#53 0x0000000000c8d094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d094)
#54 0x000078be1c029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x000078be1c029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x0000000000ddc135 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc135)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/7z8v3sGTq
```c
class Node {
    static void doSomething(
        int z,
        auto g = [&amp;](int i){ z }
    ) {}
};
```
Go back to `clang 17.0.1 (assertions)`.

- Clang crashes when a lambda expression appearing in a default argument of a class member function uses a capture-default and references a function parameter.

- Although the code is ill-formed. Clang correctly emits diagnostics but then proceeds into lambda capture analysis and crashes in `Sema::tryCaptureVariable` due to the absence of a function scope.

- The ICE only occurs when the lambda body actually references an entity that requires capture (e.g. the function parameter). Removing the reference avoids the crash.
## Stack dump
```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:295: T&amp; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::operator[](llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type) [with T = clang::sema::FunctionScopeInfo*; &lt;template-parameter-1-2&gt; = void; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::reference = clang::sema::FunctionScopeInfo*&amp;; llvm::SmallVectorTemplateCommon&lt;T, &lt;template-parameter-1-2&gt; &gt;::size_type = long unsigned int]: Assertion `idx &lt; size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:4:32: current parser token '}'
2.	&lt;source&gt;:1:1: parsing struct/union/class body 'Node'
3.	&lt;source&gt;:4:18: lambda expression parsing
4.	&lt;source&gt;:4:28: in compound statement ('{}')
 #<!-- -->0 0x00000000042ef648 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ef648)
 #<!-- -->1 0x00000000042eca74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eca74)
 #<!-- -->2 0x0000000004231818 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078be1c042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000078be1c0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000078be1c042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000078be1c0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000078be1c02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000078be1c039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f6c45e clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&amp;, clang::QualType&amp;, unsigned int const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f6c45e)
#<!-- -->10 0x0000000006f6c949 clang::Sema::getCapturedDeclRefType(clang::ValueDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f6c949)
#<!-- -->11 0x0000000006f74bdc clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f74bdc)
#<!-- -->12 0x0000000006f74f66 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&amp;, clang::LookupResult&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f74f66)
#<!-- -->13 0x0000000006da3989 clang::Sema::ActOnNameClassifiedAsNonType(clang::Scope*, clang::CXXScopeSpec const&amp;, clang::NamedDecl*, clang::SourceLocation, clang::Token const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6da3989)
#<!-- -->14 0x0000000006ab85ae clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab85ae)
#<!-- -->15 0x0000000006a9c0d2 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9c0d2)
#<!-- -->16 0x0000000006a9dcc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dcc7)
#<!-- -->17 0x0000000006a9dd59 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dd59)
#<!-- -->18 0x0000000006aa2339 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa2339)
#<!-- -->19 0x0000000006b27bf9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b27bf9)
#<!-- -->20 0x0000000006b1f0a4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1f0a4)
#<!-- -->21 0x0000000006b1fa86 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1fa86)
#<!-- -->22 0x0000000006b283ce clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b283ce)
#<!-- -->23 0x0000000006abbba2 clang::Parser::ParseLambdaExpressionAfterIntroducer(clang::LambdaIntroducer&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abbba2)
#<!-- -->24 0x0000000006abcccb clang::Parser::ParseLambdaExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abcccb)
#<!-- -->25 0x0000000006a9d2a5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9d2a5)
#<!-- -->26 0x0000000006a9dcc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dcc7)
#<!-- -->27 0x0000000006a9dd59 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dd59)
#<!-- -->28 0x0000000006a482b0 clang::Parser::ParseLexedMethodDeclaration(clang::Parser::LateParsedMethodDeclaration&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a482b0)
#<!-- -->29 0x0000000006a3d24d clang::Parser::ParseLexedMethodDeclarations(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3d24d)
#<!-- -->30 0x0000000006a913a9 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a913a9)
#<!-- -->31 0x0000000006a93e30 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a93e30)
#<!-- -->32 0x0000000006a66c5e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a66c5e)
#<!-- -->33 0x0000000006a21a57 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a21a57)
#<!-- -->34 0x0000000006a224cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a224cf)
#<!-- -->35 0x0000000006a2a2fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2a2fa)
#<!-- -->36 0x0000000006a2b295 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b295)
#<!-- -->37 0x0000000006a2b740 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b740)
#<!-- -->38 0x0000000006a0c2e3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0c2e3)
#<!-- -->39 0x0000000004c50978 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c50978)
#<!-- -->40 0x0000000004f46ff5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f46ff5)
#<!-- -->41 0x0000000004ec728e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec728e)
#<!-- -->42 0x000000000504137d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x504137d)
#<!-- -->43 0x0000000000de5c4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5c4c)
#<!-- -->44 0x0000000000ddc69a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->45 0x0000000000ddc81d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->46 0x0000000004cc3049 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->47 0x0000000004231cb4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231cb4)
#<!-- -->48 0x0000000004cc365f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->49 0x0000000004c84142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c84142)
#<!-- -->50 0x0000000004c850ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c850ee)
#<!-- -->51 0x0000000004c8c545 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8c545)
#<!-- -->52 0x0000000000de2041 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2041)
#<!-- -->53 0x0000000000c8d094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d094)
#<!-- -->54 0x000078be1c029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->55 0x000078be1c029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->56 0x0000000000ddc135 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc135)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

We have had a lot of `tryCaptureVariable` crashes, most are fixed now.

This one also goes back to clang-17: https://github.com/llvm/llvm-project/issues/115280

---

