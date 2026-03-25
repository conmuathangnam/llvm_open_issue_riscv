# Clang assertion failed in LocalInstantiationScope::findInstantiationOf when generic lambda default argument refers to init-capture

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173662

## Body

### Link for quick verification: https://godbolt.org/z/89n6dh564
```c
int main() {
    int y;
    auto inner_y = [z = 0](auto inner_y, double inner_val = z) {};
    inner_y(y);
    return 0;
}
```
Go back to `clang 18.1.0 (assertions)`.
The compiler crashes when instantiating a generic lambda where a default parameter expression refers to a variable introduced via init-capture. This violates the expected instantiation scope mapping.

## Stack dump
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4632: llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::ValueDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:14: current parser token ')'
2.	<source>:1:12: parsing function body 'main'
3.	<source>:1:12: in compound statement ('{}')
 #0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e1914642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e19146969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e1914642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e19146287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007e191462871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007e1914639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000754f7cd clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x754f7cd)
#10 0x0000000007636eb7 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7636eb7)
#11 0x0000000007576843 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000758bfe0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x000000000758c4eb (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000755c101 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000755c0a0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000007563769 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRecoveryExpr(clang::RecoveryExpr*) SemaTemplateInstantiate.cpp:0:0
#17 0x000000000755c3be clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#18 0x000000000757bd65 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000757e25f void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDefaultArgument(clang::SourceLocation, clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, bool)::'lambda'()>(long) SemaTemplateInstantiate.cpp:0:0
#20 0x00000000084f7351 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f7351)
#21 0x00000000075599f3 clang::Sema::SubstDefaultArgument(clang::SourceLocation, clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75599f3)
#22 0x00000000075e9102 clang::Sema::InstantiateDefaultArgument(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75e9102)
#23 0x0000000006f78acd clang::Sema::BuildCXXDefaultArgExpr(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f78acd)
#24 0x00000000072f1a8b PrepareArgumentsForCallToObjectOfClassType(clang::Sema&, llvm::SmallVectorImpl<clang::Expr*>&, clang::CXXMethodDecl*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) SemaOverload.cpp:0:0
#25 0x0000000007343d90 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7343d90)
#26 0x0000000006f9c2b4 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9c2b4)
#27 0x0000000006fa120c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa120c)
#28 0x0000000006a9f607 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9f607)
#29 0x0000000006a9851a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9851a)
#30 0x0000000006a9a777 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9a777)
#31 0x0000000006a9a809 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9a809)
#32 0x0000000006a9ede9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9ede9)
#33 0x0000000006b246a9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b246a9)
#34 0x0000000006b1bb54 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1bb54)
#35 0x0000000006b1c536 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1c536)
#36 0x0000000006b24e7e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b24e7e)
#37 0x0000000006b256ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b256ca)
#38 0x0000000006a2b62b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b62b)
#39 0x0000000006a61aed clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a61aed)
#40 0x0000000006a1e78b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e78b)
#41 0x0000000006a1ef7f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ef7f)
#42 0x0000000006a26daa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a26daa)
#43 0x0000000006a27d45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27d45)
#44 0x0000000006a281f0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a281f0)
#45 0x0000000006a08d93 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08d93)
#46 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#47 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#48 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#49 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#50 0x0000000000de5b4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#51 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#52 0x0000000000ddc71d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#53 0x0000000004cc0739 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#54 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#55 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#56 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#57 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#58 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#59 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#60 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#61 0x00007e1914629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#62 0x00007e1914629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#63 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/89n6dh564
```c
int main() {
    int y;
    auto inner_y = [z = 0](auto inner_y, double inner_val = z) {};
    inner_y(y);
    return 0;
}
```
Go back to `clang 18.1.0 (assertions)`.
The compiler crashes when instantiating a generic lambda where a default parameter expression refers to a variable introduced via init-capture. This violates the expected instantiation scope mapping.

## Stack dump
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:4632: llvm::PointerUnion&lt;clang::Decl*, llvm::SmallVector&lt;clang::ValueDecl*, 4&gt;*&gt;* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*): Assertion `isa&lt;LabelDecl&gt;(D) &amp;&amp; "declaration not instantiated in this scope"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:5:14: current parser token ')'
2.	&lt;source&gt;:1:12: parsing function body 'main'
3.	&lt;source&gt;:1:12: in compound statement ('{}')
 #<!-- -->0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #<!-- -->1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #<!-- -->2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e1914642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e19146969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e1914642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e19146287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007e191462871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007e1914639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000754f7cd clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x754f7cd)
#<!-- -->10 0x0000000007636eb7 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7636eb7)
#<!-- -->11 0x0000000007576843 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x000000000758bfe0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x000000000758c4eb (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x000000000755c101 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x000000000755c0a0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000007563769 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRecoveryExpr(clang::RecoveryExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x000000000755c3be clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x000000000757bd65 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x000000000757e25f void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDefaultArgument(clang::SourceLocation, clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool)::'lambda'()&gt;(long) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x00000000084f7351 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f7351)
#<!-- -->21 0x00000000075599f3 clang::Sema::SubstDefaultArgument(clang::SourceLocation, clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75599f3)
#<!-- -->22 0x00000000075e9102 clang::Sema::InstantiateDefaultArgument(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75e9102)
#<!-- -->23 0x0000000006f78acd clang::Sema::BuildCXXDefaultArgExpr(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f78acd)
#<!-- -->24 0x00000000072f1a8b PrepareArgumentsForCallToObjectOfClassType(clang::Sema&amp;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::CXXMethodDecl*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) SemaOverload.cpp:0:0
#<!-- -->25 0x0000000007343d90 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7343d90)
#<!-- -->26 0x0000000006f9c2b4 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9c2b4)
#<!-- -->27 0x0000000006fa120c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa120c)
#<!-- -->28 0x0000000006a9f607 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9f607)
#<!-- -->29 0x0000000006a9851a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9851a)
#<!-- -->30 0x0000000006a9a777 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9a777)
#<!-- -->31 0x0000000006a9a809 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9a809)
#<!-- -->32 0x0000000006a9ede9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9ede9)
#<!-- -->33 0x0000000006b246a9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b246a9)
#<!-- -->34 0x0000000006b1bb54 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1bb54)
#<!-- -->35 0x0000000006b1c536 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1c536)
#<!-- -->36 0x0000000006b24e7e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b24e7e)
#<!-- -->37 0x0000000006b256ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b256ca)
#<!-- -->38 0x0000000006a2b62b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b62b)
#<!-- -->39 0x0000000006a61aed clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a61aed)
#<!-- -->40 0x0000000006a1e78b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e78b)
#<!-- -->41 0x0000000006a1ef7f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ef7f)
#<!-- -->42 0x0000000006a26daa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a26daa)
#<!-- -->43 0x0000000006a27d45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27d45)
#<!-- -->44 0x0000000006a281f0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a281f0)
#<!-- -->45 0x0000000006a08d93 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08d93)
#<!-- -->46 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#<!-- -->47 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#<!-- -->48 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#<!-- -->49 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#<!-- -->50 0x0000000000de5b4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#<!-- -->51 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->52 0x0000000000ddc71d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->53 0x0000000004cc0739 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->54 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#<!-- -->55 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->56 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#<!-- -->57 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#<!-- -->58 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#<!-- -->59 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#<!-- -->60 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#<!-- -->61 0x00007e1914629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->62 0x00007e1914629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->63 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - AbyssStaror

Another testcase seemingly triggers the same ICE. 
Go back to clang 16 (assertions). [Link]( https://godbolt.org/z/fnhKd9Wqn)
```c
template <typename T>
void foo() {
    int i = 0;
    
    auto x = [i]() {        
        struct A { static constexpr int val = 42 + i; };  // Mix local types with captured state
        return A::val;
    };
}

int main() {
   foo<int>();
   return 0;
}
```

---

### Comment 3 - shafik

@kadircet this bisects to 373fcd5d73a3ed5bedff771bcf6a3aba981155cc

---

### Comment 4 - kadircet

I took a brief look here, it just looks like code path starting with `clang::Sema::BuildCXXDefaultArgExpr` doesn't really look at invalid decls/exprs. Before https://github.com/llvm/llvm-project/commit/373fcd5d73a3ed5bedff771bcf6a3aba981155cc this worked fine, as we weren't attaching any subexpressions under the opaquevalueexpr. but we now have a recovery-expr pointing at the malformed default-arg-expr, which triggers these assertions.

i think we should check for validness of parameters before trying to build default args, but I am not sure, and hit my time-box while investigating the issue (couldn't prioritize more as this is a crash-on-invalid and underlying change is 2+ years old). early exiting in `Sema::SubstDefaultArgument` (first place in the stack that analyzes the recovery expr) when the default arg contains errors fixes the crash (while preserving the diagnostic). put a draft PR in https://github.com/llvm/llvm-project/pull/175582, happy to land if that makes sense for you folks as well.

---

