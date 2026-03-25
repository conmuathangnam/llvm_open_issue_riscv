# Clang assertion failed: Access != AS_none && "Access specifier is AS_none inside a record decl" when using address-of-label in nested struct array bound

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173835

## Body

### Link for quick verification: https://godbolt.org/z/PMvv5eWhT
```c
int main() {
    int a;
    struct {
        int j[a ? (&&m) : (&&m)]; 
    } s;
}
```
Go back to `clang 2.9.0 (asertions)`.
The following code triggers an ICE in Clang. It appears that when Clang's Sema attempts to resolve a label address (`&&label`) within the array bound of a nested struct member, the access specifier check fails because the context is incorrectly identified or not properly initialized for this specific GCC extension.

## Stack dump:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/DeclBase.cpp:1138: bool clang::Decl::AccessDeclContextCheck() const: Assertion `Access != AS_none && "Access specifier is AS_none inside a record decl"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:30: current parser token 'm'
2.	<source>:1:12: parsing function body 'main'
3.	<source>:1:12: in compound statement ('{}')
4.	<source>:3:5: parsing struct/union/class body '(unnamed struct at <source>:3:5)'
 #0 0x0000000004301458 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4301458)
 #1 0x00000000042fe884 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fe884)
 #2 0x0000000004243628 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070ea2ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070ea2ea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070ea2ea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070ea2ea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070ea2ea2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070ea2ea39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007ba4bb8 clang::Decl::AccessDeclContextCheck() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ba4bb8)
#10 0x0000000007139a21 LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#11 0x000000000713a361 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x713a361)
#12 0x00000000071294f5 clang::Sema::CppLookupName(clang::LookupResult&, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71294f5)
#13 0x000000000712a2b5 clang::Sema::LookupName(clang::LookupResult&, clang::Scope*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x712a2b5)
#14 0x000000000713c9a4 clang::Sema::LookupExistingLabel(clang::IdentifierInfo*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x713c9a4)
#15 0x000000000713cbb5 clang::Sema::LookupOrCreateLabel(clang::IdentifierInfo*, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x713cbb5)
#16 0x0000000006aaf067 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aaf067)
#17 0x0000000006aafd37 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aafd37)
#18 0x0000000006aafdc9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aafdc9)
#19 0x0000000006ab43a9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab43a9)
#20 0x0000000006ab8184 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab8184)
#21 0x0000000006aae9c8 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aae9c8)
#22 0x0000000006aafd37 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aafd37)
#23 0x0000000006aafdc9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aafdc9)
#24 0x0000000006ab166c clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab166c)
#25 0x0000000006abad0a clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abad0a)
#26 0x0000000006abadfa clang::Parser::ParseArrayBoundExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abadfa)
#27 0x0000000006a73f19 clang::Parser::ParseBracketDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a73f19)
#28 0x0000000006a85f13 clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a85f13)
#29 0x0000000006a7329f clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7329f)
#30 0x000000000850d501 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x850d501)
#31 0x0000000006a61caf clang::Parser::ParseDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a61caf)
#32 0x0000000006a95d75 clang::Parser::ParseCXXMemberDeclaratorBeforeInitializer(clang::Declarator&, clang::VirtSpecifiers&, clang::ActionResult<clang::Expr*, true>&, clang::Parser::LateParsedAttrList&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95d75)
#33 0x0000000006a9f832 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9f832)
#34 0x0000000006aa14ff clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa14ff)
#35 0x0000000006aa32bc clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa32bc)
#36 0x0000000006aa5ea0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa5ea0)
#37 0x0000000006a78cce clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a78cce)
#38 0x0000000006a7f337 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7f337)
#39 0x0000000006a7f87c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7f87c)
#40 0x0000000006b31264 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b31264)
#41 0x0000000006b31af6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b31af6)
#42 0x0000000006b3a43e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3a43e)
#43 0x0000000006b3ac8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3ac8a)
#44 0x0000000006a40beb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a40beb)
#45 0x0000000006a770ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a770ad)
#46 0x0000000006a33d4b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a33d4b)
#47 0x0000000006a3453f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3453f)
#48 0x0000000006a3c36a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3c36a)
#49 0x0000000006a3d305 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3d305)
#50 0x0000000006a3d7b0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3d7b0)
#51 0x0000000006a1e353 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e353)
#52 0x0000000004c62788 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c62788)
#53 0x0000000004f58e05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f58e05)
#54 0x0000000004ed909e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed909e)
#55 0x000000000505318d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505318d)
#56 0x0000000000de63ec cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde63ec)
#57 0x0000000000ddce3a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#58 0x0000000000ddcfbd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#59 0x0000000004cd4e59 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#60 0x0000000004243ac4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4243ac4)
#61 0x0000000004cd546f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#62 0x0000000004c95f52 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c95f52)
#63 0x0000000004c96efe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c96efe)
#64 0x0000000004c9e355 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9e355)
#65 0x0000000000de27e1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde27e1)
#66 0x0000000000c8d114 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d114)
#67 0x000070ea2ea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#68 0x000070ea2ea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#69 0x0000000000ddc8d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc8d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/PMvv5eWhT
```c
int main() {
    int a;
    struct {
        int j[a ? (&amp;&amp;m) : (&amp;&amp;m)]; 
    } s;
}
```
Go back to `clang 2.9.0 (asertions)`.
The following code triggers an ICE in Clang. It appears that when Clang's Sema attempts to resolve a label address (`&amp;&amp;label`) within the array bound of a nested struct member, the access specifier check fails because the context is incorrectly identified or not properly initialized for this specific GCC extension.

## Stack dump:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/DeclBase.cpp:1138: bool clang::Decl::AccessDeclContextCheck() const: Assertion `Access != AS_none &amp;&amp; "Access specifier is AS_none inside a record decl"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:5:30: current parser token 'm'
2.	&lt;source&gt;:1:12: parsing function body 'main'
3.	&lt;source&gt;:1:12: in compound statement ('{}')
4.	&lt;source&gt;:3:5: parsing struct/union/class body '(unnamed struct at &lt;source&gt;:3:5)'
 #<!-- -->0 0x0000000004301458 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4301458)
 #<!-- -->1 0x00000000042fe884 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fe884)
 #<!-- -->2 0x0000000004243628 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070ea2ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070ea2ea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070ea2ea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070ea2ea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070ea2ea2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070ea2ea39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007ba4bb8 clang::Decl::AccessDeclContextCheck() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ba4bb8)
#<!-- -->10 0x0000000007139a21 LookupDirect(clang::Sema&amp;, clang::LookupResult&amp;, clang::DeclContext const*) SemaLookup.cpp:0:0
#<!-- -->11 0x000000000713a361 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x713a361)
#<!-- -->12 0x00000000071294f5 clang::Sema::CppLookupName(clang::LookupResult&amp;, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71294f5)
#<!-- -->13 0x000000000712a2b5 clang::Sema::LookupName(clang::LookupResult&amp;, clang::Scope*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x712a2b5)
#<!-- -->14 0x000000000713c9a4 clang::Sema::LookupExistingLabel(clang::IdentifierInfo*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x713c9a4)
#<!-- -->15 0x000000000713cbb5 clang::Sema::LookupOrCreateLabel(clang::IdentifierInfo*, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x713cbb5)
#<!-- -->16 0x0000000006aaf067 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aaf067)
#<!-- -->17 0x0000000006aafd37 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aafd37)
#<!-- -->18 0x0000000006aafdc9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aafdc9)
#<!-- -->19 0x0000000006ab43a9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab43a9)
#<!-- -->20 0x0000000006ab8184 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab8184)
#<!-- -->21 0x0000000006aae9c8 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aae9c8)
#<!-- -->22 0x0000000006aafd37 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aafd37)
#<!-- -->23 0x0000000006aafdc9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aafdc9)
#<!-- -->24 0x0000000006ab166c clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab166c)
#<!-- -->25 0x0000000006abad0a clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abad0a)
#<!-- -->26 0x0000000006abadfa clang::Parser::ParseArrayBoundExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abadfa)
#<!-- -->27 0x0000000006a73f19 clang::Parser::ParseBracketDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a73f19)
#<!-- -->28 0x0000000006a85f13 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a85f13)
#<!-- -->29 0x0000000006a7329f clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7329f)
#<!-- -->30 0x000000000850d501 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x850d501)
#<!-- -->31 0x0000000006a61caf clang::Parser::ParseDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a61caf)
#<!-- -->32 0x0000000006a95d75 clang::Parser::ParseCXXMemberDeclaratorBeforeInitializer(clang::Declarator&amp;, clang::VirtSpecifiers&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::Parser::LateParsedAttrList&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95d75)
#<!-- -->33 0x0000000006a9f832 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9f832)
#<!-- -->34 0x0000000006aa14ff clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa14ff)
#<!-- -->35 0x0000000006aa32bc clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa32bc)
#<!-- -->36 0x0000000006aa5ea0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa5ea0)
#<!-- -->37 0x0000000006a78cce clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a78cce)
#<!-- -->38 0x0000000006a7f337 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7f337)
#<!-- -->39 0x0000000006a7f87c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7f87c)
#<!-- -->40 0x0000000006b31264 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b31264)
#<!-- -->41 0x0000000006b31af6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b31af6)
#<!-- -->42 0x0000000006b3a43e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3a43e)
#<!-- -->43 0x0000000006b3ac8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3ac8a)
#<!-- -->44 0x0000000006a40beb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a40beb)
#<!-- -->45 0x0000000006a770ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a770ad)
#<!-- -->46 0x0000000006a33d4b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a33d4b)
#<!-- -->47 0x0000000006a3453f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3453f)
#<!-- -->48 0x0000000006a3c36a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3c36a)
#<!-- -->49 0x0000000006a3d305 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3d305)
#<!-- -->50 0x0000000006a3d7b0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3d7b0)
#<!-- -->51 0x0000000006a1e353 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e353)
#<!-- -->52 0x0000000004c62788 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c62788)
#<!-- -->53 0x0000000004f58e05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f58e05)
#<!-- -->54 0x0000000004ed909e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed909e)
#<!-- -->55 0x000000000505318d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505318d)
#<!-- -->56 0x0000000000de63ec cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde63ec)
#<!-- -->57 0x0000000000ddce3a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->58 0x0000000000ddcfbd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->59 0x0000000004cd4e59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->60 0x0000000004243ac4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4243ac4)
#<!-- -->61 0x0000000004cd546f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->62 0x0000000004c95f52 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c95f52)
#<!-- -->63 0x0000000004c96efe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c96efe)
#<!-- -->64 0x0000000004c9e355 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9e355)
#<!-- -->65 0x0000000000de27e1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde27e1)
#<!-- -->66 0x0000000000c8d114 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d114)
#<!-- -->67 0x000070ea2ea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->68 0x000070ea2ea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->69 0x0000000000ddc8d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc8d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

