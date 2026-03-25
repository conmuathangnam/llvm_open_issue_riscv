# [clang] Assertion `DestTy->isFloatingType() && "unexpected type"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/182212

## Body

Reproducer:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00067.cpp
https://godbolt.org/z/YKqe7dMjK
```c
char *foo = (int (*[5])long){
#embed __FILE__
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:14774: bool {anonymous}::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(const clang::Expr*, llvm::ArrayRef<clang::Expr*>, const clang::Expr*, clang::QualType): Assertion `DestTy->isFloatingType() && "unexpected type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<source>:3:2: current parser token ';'
 #0 0x00000000043d5bb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43d5bb8)
 #1 0x00000000043d3014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43d3014)
 #2 0x0000000004313298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a02c0442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a02c04969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007a02c0442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007a02c04287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007a02c042871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007a02c0439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007ed8f08 (anonymous namespace)::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef<clang::Expr*>, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#10 0x0000000007ed63dd clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x0000000007ed6efd EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&, clang::APValue&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#12 0x0000000007ed7000 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#13 0x0000000007ed95bc (anonymous namespace)::LValueExprEvaluator::VisitCompoundLiteralExpr(clang::CompoundLiteralExpr const*) (.isra.0) ExprConstant.cpp:0:0
#14 0x0000000007ee49cf clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#15 0x0000000007ee693d EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#16 0x0000000007f2bcd7 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#17 0x0000000007eb8191 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#18 0x0000000007f2b658 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#19 0x0000000007eb8191 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#20 0x0000000007eb9c3b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#21 0x0000000007ebddd5 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#22 0x0000000007ebe91b EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#23 0x0000000007ec6a9e clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec6a9e)
#24 0x0000000007ec6b31 clang::Expr::isEvaluatable(clang::ASTContext const&, clang::Expr::SideEffectsKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec6b31)
#25 0x0000000007e77e0e clang::Expr::isConstantInitializer(clang::ASTContext&, bool, clang::Expr const**) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e77e0e)
#26 0x0000000006f29687 clang::Sema::CheckForConstantInitializer(clang::Expr*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f29687)
#27 0x0000000006f7bfaa clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f7bfaa)
#28 0x0000000006bc6b71 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc6b71)
#29 0x0000000006bd3dda clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bd3dda)
#30 0x0000000006b91cbb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b91cbb)
#31 0x0000000006b924af clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b924af)
#32 0x0000000006b99ad1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b99ad1)
#33 0x0000000006b9aa05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b9aa05)
#34 0x0000000006b9adf0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b9adf0)
#35 0x0000000006b7c0a3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b7c0a3)
#36 0x0000000004d51ef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d51ef8)
#37 0x00000000050508a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50508a5)
#38 0x0000000004fcfbde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fcfbde)
#39 0x000000000514bc4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x514bc4d)
#40 0x0000000000dea09e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdea09e)
#41 0x0000000000de0a6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#42 0x0000000000de0bed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#43 0x0000000004dc59d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#44 0x0000000004313734 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4313734)
#45 0x0000000004dc5fef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#46 0x0000000004d86972 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d86972)
#47 0x0000000004d8791e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d8791e)
#48 0x0000000004d8ebd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d8ebd5)
#49 0x0000000000de6411 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6411)
#50 0x0000000000ca0034 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca0034)
#51 0x00007a02c0429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#52 0x00007a02c0429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#53 0x0000000000de0505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde0505)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00067.cpp
https://godbolt.org/z/YKqe7dMjK
```c
char *foo = (int (*[5])long){
#embed __FILE__
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:14774: bool {anonymous}::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(const clang::Expr*, llvm::ArrayRef&lt;clang::Expr*&gt;, const clang::Expr*, clang::QualType): Assertion `DestTy-&gt;isFloatingType() &amp;&amp; "unexpected type"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c &lt;source&gt;
1.	&lt;source&gt;:3:2: current parser token ';'
 #<!-- -->0 0x00000000043d5bb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43d5bb8)
 #<!-- -->1 0x00000000043d3014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43d3014)
 #<!-- -->2 0x0000000004313298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a02c0442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007a02c04969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007a02c0442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007a02c04287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007a02c042871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007a02c0439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007ed8f08 (anonymous namespace)::ArrayExprEvaluator::VisitCXXParenListOrInitListExpr(clang::Expr const*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::Expr const*, clang::QualType) ExprConstant.cpp:0:0
#<!-- -->10 0x0000000007ed63dd clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::ArrayExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007ed6efd EvaluateArray(clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007ed7000 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) (.part.0) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007ed95bc (anonymous namespace)::LValueExprEvaluator::VisitCompoundLiteralExpr(clang::CompoundLiteralExpr const*) (.isra.0) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007ee49cf clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000007ee693d EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->16 0x0000000007f2bcd7 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->17 0x0000000007eb8191 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->18 0x0000000007f2b658 (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->19 0x0000000007eb8191 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->20 0x0000000007eb9c3b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->21 0x0000000007ebddd5 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->22 0x0000000007ebe91b EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->23 0x0000000007ec6a9e clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec6a9e)
#<!-- -->24 0x0000000007ec6b31 clang::Expr::isEvaluatable(clang::ASTContext const&amp;, clang::Expr::SideEffectsKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec6b31)
#<!-- -->25 0x0000000007e77e0e clang::Expr::isConstantInitializer(clang::ASTContext&amp;, bool, clang::Expr const**) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e77e0e)
#<!-- -->26 0x0000000006f29687 clang::Sema::CheckForConstantInitializer(clang::Expr*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f29687)
#<!-- -->27 0x0000000006f7bfaa clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f7bfaa)
#<!-- -->28 0x0000000006bc6b71 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc6b71)
#<!-- -->29 0x0000000006bd3dda clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bd3dda)
#<!-- -->30 0x0000000006b91cbb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b91cbb)
#<!-- -->31 0x0000000006b924af clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b924af)
#<!-- -->32 0x0000000006b99ad1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b99ad1)
#<!-- -->33 0x0000000006b9aa05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b9aa05)
#<!-- -->34 0x0000000006b9adf0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b9adf0)
#<!-- -->35 0x0000000006b7c0a3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b7c0a3)
#<!-- -->36 0x0000000004d51ef8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d51ef8)
#<!-- -->37 0x00000000050508a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50508a5)
#<!-- -->38 0x0000000004fcfbde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fcfbde)
#<!-- -->39 0x000000000514bc4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x514bc4d)
#<!-- -->40 0x0000000000dea09e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdea09e)
#<!-- -->41 0x0000000000de0a6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->42 0x0000000000de0bed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->43 0x0000000004dc59d9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->44 0x0000000004313734 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4313734)
#<!-- -->45 0x0000000004dc5fef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->46 0x0000000004d86972 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d86972)
#<!-- -->47 0x0000000004d8791e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d8791e)
#<!-- -->48 0x0000000004d8ebd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d8ebd5)
#<!-- -->49 0x0000000000de6411 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6411)
#<!-- -->50 0x0000000000ca0034 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca0034)
#<!-- -->51 0x00007a02c0429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->52 0x00007a02c0429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->53 0x0000000000de0505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde0505)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21:
https://godbolt.org/z/jE1q1hE9o

---

### Comment 3 - Aadarsh-Keshri

@Fznamznon I see the issue is self-assigned to you. But may I raise the PR? I've been working on this issue for a few hours and I think I've solved it.

---

### Comment 4 - Fznamznon

@Aadarsh-Keshri sure

---

### Comment 5 - Aadarsh-Keshri

I only have tests to add

---

### Comment 6 - Aadarsh-Keshri

@Fznamznon Thank you.

---

