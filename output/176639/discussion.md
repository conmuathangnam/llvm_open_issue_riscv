# [clang] still crashes in CheckArityMismatch since clang-19

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/176639
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, regression:19
**Closed Date:** 2026-01-31T16:45:31Z

## Body

Related issue (already fixed): https://github.com/llvm/llvm-project/issues/173943

Reproducer:
https://godbolt.org/z/YxGaG49E8
```cpp
struct S {
  void operator()(this S =)
};

void foo() {
  S s{};
  s(0);
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:12125: bool CheckArityMismatch(clang::Sema&, clang::OverloadCandidate*, unsigned int, bool): Assertion `(Cand->FailureKind == ovl_fail_too_few_arguments) || (Cand->FailureKind == ovl_fail_bad_deduction && Cand->DeductionFailure.getResult() == TemplateDeductionResult::TooFewArguments)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:7:6: current parser token ')'
2.	<source>:5:12: parsing function body 'foo'
3.	<source>:5:12: in compound statement ('{}')
 #0 0x000000000430d598 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430d598)
 #1 0x000000000430a9f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430a9f4)
 #2 0x000000000424ccc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fe796c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fe796c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007fe796c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007fe796c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007fe796c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007fe796c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000733a0c8 CheckArityMismatch(clang::Sema&, clang::OverloadCandidate*, unsigned int, bool) (.constprop.0) SemaOverload.cpp:0:0
#10 0x00000000073812d3 clang::OverloadCandidateSet::NoteCandidates(clang::Sema&, llvm::ArrayRef<clang::Expr*>, llvm::ArrayRef<clang::OverloadCandidate*>, llvm::StringRef, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73812d3)
#11 0x0000000007382581 clang::OverloadCandidateSet::NoteCandidates(std::pair<clang::SourceLocation, clang::PartialDiagnostic>, clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, llvm::StringRef, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7382581)
#12 0x000000000738e37b clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x738e37b)
#13 0x0000000006fe6154 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fe6154)
#14 0x0000000006feb0ac clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6feb0ac)
#15 0x0000000006ae9a27 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae9a27)
#16 0x0000000006ae293a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae293a)
#17 0x0000000006ae4b97 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae4b97)
#18 0x0000000006ae4c29 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae4c29)
#19 0x0000000006ae9209 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae9209)
#20 0x0000000006b6ed59 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6ed59)
#21 0x0000000006b66204 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b66204)
#22 0x0000000006b66be6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b66be6)
#23 0x0000000006b6f52e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6f52e)
#24 0x0000000006b6fd7a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6fd7a)
#25 0x0000000006a75f4b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a75f4b)
#26 0x0000000006aac3bd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aac3bd)
#27 0x0000000006a690ab clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a690ab)
#28 0x0000000006a6989f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6989f)
#29 0x0000000006a716ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a716ca)
#30 0x0000000006a72665 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a72665)
#31 0x0000000006a536ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a536ea)
#32 0x0000000004c75558 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c75558)
#33 0x0000000004f6f965 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6f965)
#34 0x0000000004eef33e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eef33e)
#35 0x000000000506a5dd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506a5dd)
#36 0x0000000000ddd1de cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd1de)
#37 0x0000000000dd3baa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#38 0x0000000000dd3d2d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#39 0x0000000004ce8a99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x000000000424d164 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424d164)
#41 0x0000000004ce90af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x0000000004ca9a82 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9a82)
#43 0x0000000004caaa2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4caaa2e)
#44 0x0000000004cb1e75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb1e75)
#45 0x0000000000dd9551 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9551)
#46 0x0000000000c96bf4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc96bf4)
#47 0x00007fe796c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x00007fe796c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000dd3645 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd3645)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-19:
https://godbolt.org/z/MGzzh4qqE

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Related issue (already fixed): https://github.com/llvm/llvm-project/issues/173943

Reproducer:
https://godbolt.org/z/YxGaG49E8
```cpp
struct S {
  void operator()(this S =)
};

void foo() {
  S s{};
  s(0);
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:12125: bool CheckArityMismatch(clang::Sema&amp;, clang::OverloadCandidate*, unsigned int, bool): Assertion `(Cand-&gt;FailureKind == ovl_fail_too_few_arguments) || (Cand-&gt;FailureKind == ovl_fail_bad_deduction &amp;&amp; Cand-&gt;DeductionFailure.getResult() == TemplateDeductionResult::TooFewArguments)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:7:6: current parser token ')'
2.	&lt;source&gt;:5:12: parsing function body 'foo'
3.	&lt;source&gt;:5:12: in compound statement ('{}')
 #<!-- -->0 0x000000000430d598 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430d598)
 #<!-- -->1 0x000000000430a9f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430a9f4)
 #<!-- -->2 0x000000000424ccc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fe796c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007fe796c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007fe796c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007fe796c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007fe796c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007fe796c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000733a0c8 CheckArityMismatch(clang::Sema&amp;, clang::OverloadCandidate*, unsigned int, bool) (.constprop.0) SemaOverload.cpp:0:0
#<!-- -->10 0x00000000073812d3 clang::OverloadCandidateSet::NoteCandidates(clang::Sema&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::ArrayRef&lt;clang::OverloadCandidate*&gt;, llvm::StringRef, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73812d3)
#<!-- -->11 0x0000000007382581 clang::OverloadCandidateSet::NoteCandidates(std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;, clang::Sema&amp;, clang::OverloadCandidateDisplayKind, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::StringRef, clang::SourceLocation, llvm::function_ref&lt;bool (clang::OverloadCandidate&amp;)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7382581)
#<!-- -->12 0x000000000738e37b clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x738e37b)
#<!-- -->13 0x0000000006fe6154 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fe6154)
#<!-- -->14 0x0000000006feb0ac clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6feb0ac)
#<!-- -->15 0x0000000006ae9a27 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae9a27)
#<!-- -->16 0x0000000006ae293a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae293a)
#<!-- -->17 0x0000000006ae4b97 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae4b97)
#<!-- -->18 0x0000000006ae4c29 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae4c29)
#<!-- -->19 0x0000000006ae9209 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae9209)
#<!-- -->20 0x0000000006b6ed59 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6ed59)
#<!-- -->21 0x0000000006b66204 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b66204)
#<!-- -->22 0x0000000006b66be6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b66be6)
#<!-- -->23 0x0000000006b6f52e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6f52e)
#<!-- -->24 0x0000000006b6fd7a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6fd7a)
#<!-- -->25 0x0000000006a75f4b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a75f4b)
#<!-- -->26 0x0000000006aac3bd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aac3bd)
#<!-- -->27 0x0000000006a690ab clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a690ab)
#<!-- -->28 0x0000000006a6989f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6989f)
#<!-- -->29 0x0000000006a716ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a716ca)
#<!-- -->30 0x0000000006a72665 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a72665)
#<!-- -->31 0x0000000006a536ea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a536ea)
#<!-- -->32 0x0000000004c75558 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c75558)
#<!-- -->33 0x0000000004f6f965 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6f965)
#<!-- -->34 0x0000000004eef33e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eef33e)
#<!-- -->35 0x000000000506a5dd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506a5dd)
#<!-- -->36 0x0000000000ddd1de cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd1de)
#<!-- -->37 0x0000000000dd3baa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->38 0x0000000000dd3d2d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->39 0x0000000004ce8a99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x000000000424d164 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424d164)
#<!-- -->41 0x0000000004ce90af clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x0000000004ca9a82 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9a82)
#<!-- -->43 0x0000000004caaa2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4caaa2e)
#<!-- -->44 0x0000000004cb1e75 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb1e75)
#<!-- -->45 0x0000000000dd9551 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9551)
#<!-- -->46 0x0000000000c96bf4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc96bf4)
#<!-- -->47 0x00007fe796c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007fe796c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000dd3645 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd3645)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-19:
https://godbolt.org/z/MGzzh4qqE
</details>


---

### Comment 2 - hax0kartik

Same crash log as https://github.com/llvm/llvm-project/issues/173943 but different reason. Adding `
D.SetInvalidType()
` in https://github.com/llvm/llvm-project/blob/main/clang/lib/Sema/SemaDeclCXX.cpp#L11599-L11603 does seem to fix the issue. Can you assign this to me?

---

### Comment 3 - shafik

> Same crash log as [#173943](https://github.com/llvm/llvm-project/issues/173943) but different reason. Adding `D.SetInvalidType()` in https://github.com/llvm/llvm-project/blob/main/clang/lib/Sema/SemaDeclCXX.cpp#L11599-L11603 does seem to fix the issue. Can you assign this to me?

@cor3ntin Looks like the code in question came in via af4751738db89 does the change proposed above make sense to you?

---

