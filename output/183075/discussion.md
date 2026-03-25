# [clang] UNREACHABLE executed in llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/183075
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, regression:21
**Closed Date:** 2026-02-25T16:44:38Z

## Body

Reproducer:
https://godbolt.org/z/M6ozrTM48
```cpp
int bar;

int foo()
{
  (baz<> + baz<>);
}
```

Backtrace:
```console
cannot canonicalize unresolved template
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:7351!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:17: current parser token ')'
2.	<source>:4:1: parsing function body 'foo'
3.	<source>:4:1: in compound statement ('{}')
 #0 0x00000000043fa218 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43fa218)
 #1 0x00000000043f7664 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43f7664)
 #2 0x00000000043377b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071aeb8642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000071aeb86969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000071aeb8642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000071aeb86287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000043421ba (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43421ba)
 #8 0x0000000007b791de clang::ASTContext::getCanonicalTemplateName(clang::TemplateName, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b791de)
 #9 0x0000000007b79337 getCommonTemplateName(clang::ASTContext const&, clang::TemplateName, clang::TemplateName, bool) ASTContext.cpp:0:0
#10 0x0000000007b7e3c9 clang::ASTContext::getCommonSugaredType(clang::QualType, clang::QualType, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b7e3c9)
#11 0x0000000007134a79 clang::Sema::UsualArithmeticConversions(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::ArithConvKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7134a79)
#12 0x000000000713927a clang::Sema::CheckAdditionOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::BinaryOperatorKind, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x713927a)
#13 0x00000000071432e5 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71432e5)
#14 0x000000000714489a clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x714489a)
#15 0x0000000007144dd6 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7144dd6)
#16 0x0000000006c43224 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c43224)
#17 0x0000000006c469b9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c469b9)
#18 0x0000000006c4a774 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c4a774)
#19 0x0000000006c40e50 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c40e50)
#20 0x0000000006c42037 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c42037)
#21 0x0000000006c420c9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c420c9)
#22 0x0000000006c469b9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c469b9)
#23 0x0000000006cccc59 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cccc59)
#24 0x0000000006cc40f4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc40f4)
#25 0x0000000006cc4ad6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc4ad6)
#26 0x0000000006ccd436 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccd436)
#27 0x0000000006ccdc7a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccdc7a)
#28 0x0000000006bd36a3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bd36a3)
#29 0x0000000006c09d3d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c09d3d)
#30 0x0000000006bc709b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc709b)
#31 0x0000000006bc789f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc789f)
#32 0x0000000006bceea1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bceea1)
#33 0x0000000006bcfdd5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcfdd5)
#34 0x0000000006bb14ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bb14ba)
#35 0x0000000004ed1ac8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed1ac8)
#36 0x00000000051d2175 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d2175)
#37 0x00000000051506ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51506ae)
#38 0x00000000052cde8d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52cde8d)
#39 0x0000000000deea2e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdeea2e)
#40 0x0000000000de53ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#41 0x0000000000de553d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#42 0x0000000004f454c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#43 0x0000000004337bf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4337bf4)
#44 0x0000000004f45adf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#45 0x0000000004f06542 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f06542)
#46 0x0000000004f074ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f074ee)
#47 0x0000000004f0e865 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f0e865)
#48 0x0000000000deada9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdeada9)
#49 0x0000000000ca3aba main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca3aba)
#50 0x000071aeb8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#51 0x000071aeb8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#52 0x0000000000de4e55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4e55)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/M6ozrTM48
```cpp
int bar;

int foo()
{
  (baz&lt;&gt; + baz&lt;&gt;);
}
```

Backtrace:
```console
cannot canonicalize unresolved template
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:7351!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:5:17: current parser token ')'
2.	&lt;source&gt;:4:1: parsing function body 'foo'
3.	&lt;source&gt;:4:1: in compound statement ('{}')
 #<!-- -->0 0x00000000043fa218 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43fa218)
 #<!-- -->1 0x00000000043f7664 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43f7664)
 #<!-- -->2 0x00000000043377b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000071aeb8642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000071aeb86969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000071aeb8642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000071aeb86287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00000000043421ba (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43421ba)
 #<!-- -->8 0x0000000007b791de clang::ASTContext::getCanonicalTemplateName(clang::TemplateName, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b791de)
 #<!-- -->9 0x0000000007b79337 getCommonTemplateName(clang::ASTContext const&amp;, clang::TemplateName, clang::TemplateName, bool) ASTContext.cpp:0:0
#<!-- -->10 0x0000000007b7e3c9 clang::ASTContext::getCommonSugaredType(clang::QualType, clang::QualType, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7b7e3c9)
#<!-- -->11 0x0000000007134a79 clang::Sema::UsualArithmeticConversions(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::ArithConvKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7134a79)
#<!-- -->12 0x000000000713927a clang::Sema::CheckAdditionOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::BinaryOperatorKind, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x713927a)
#<!-- -->13 0x00000000071432e5 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71432e5)
#<!-- -->14 0x000000000714489a clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x714489a)
#<!-- -->15 0x0000000007144dd6 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7144dd6)
#<!-- -->16 0x0000000006c43224 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c43224)
#<!-- -->17 0x0000000006c469b9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c469b9)
#<!-- -->18 0x0000000006c4a774 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c4a774)
#<!-- -->19 0x0000000006c40e50 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c40e50)
#<!-- -->20 0x0000000006c42037 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c42037)
#<!-- -->21 0x0000000006c420c9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c420c9)
#<!-- -->22 0x0000000006c469b9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c469b9)
#<!-- -->23 0x0000000006cccc59 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cccc59)
#<!-- -->24 0x0000000006cc40f4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc40f4)
#<!-- -->25 0x0000000006cc4ad6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc4ad6)
#<!-- -->26 0x0000000006ccd436 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccd436)
#<!-- -->27 0x0000000006ccdc7a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ccdc7a)
#<!-- -->28 0x0000000006bd36a3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bd36a3)
#<!-- -->29 0x0000000006c09d3d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c09d3d)
#<!-- -->30 0x0000000006bc709b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc709b)
#<!-- -->31 0x0000000006bc789f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc789f)
#<!-- -->32 0x0000000006bceea1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bceea1)
#<!-- -->33 0x0000000006bcfdd5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcfdd5)
#<!-- -->34 0x0000000006bb14ba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bb14ba)
#<!-- -->35 0x0000000004ed1ac8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed1ac8)
#<!-- -->36 0x00000000051d2175 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51d2175)
#<!-- -->37 0x00000000051506ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51506ae)
#<!-- -->38 0x00000000052cde8d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52cde8d)
#<!-- -->39 0x0000000000deea2e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdeea2e)
#<!-- -->40 0x0000000000de53ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->41 0x0000000000de553d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->42 0x0000000004f454c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->43 0x0000000004337bf4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4337bf4)
#<!-- -->44 0x0000000004f45adf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->45 0x0000000004f06542 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f06542)
#<!-- -->46 0x0000000004f074ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f074ee)
#<!-- -->47 0x0000000004f0e865 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f0e865)
#<!-- -->48 0x0000000000deada9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdeada9)
#<!-- -->49 0x0000000000ca3aba main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca3aba)
#<!-- -->50 0x000071aeb8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->51 0x000071aeb8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->52 0x0000000000de4e55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4e55)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21:
https://godbolt.org/z/qxn4vhPx1

---

### Comment 3 - shafik

CC @mizvekov 

---

