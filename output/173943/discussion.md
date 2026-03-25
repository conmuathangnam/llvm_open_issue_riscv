# [clang] crash in CheckArityMismatch since clang-19

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173943
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, regression:19
**Closed Date:** 2026-01-06T19:30:44Z

## Body

Reproducer:
https://godbolt.org/z/rr94r1K8a
```cpp
constexpr void f(this auto& self)

void g() {
  f();
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:12129: bool CheckArityMismatch(clang::Sema&, clang::OverloadCandidate*, unsigned int, bool): Assertion `(Cand->FailureKind == ovl_fail_too_many_arguments) || (Cand->FailureKind == ovl_fail_bad_deduction && Cand->DeductionFailure.getResult() == TemplateDeductionResult::TooManyArguments)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++03 <source>
1.	<source>:4:5: current parser token ')'
2.	<source>:3:10: parsing function body 'g'
3.	<source>:3:10: in compound statement ('{}')
 #0 0x0000000004302f28 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4302f28)
 #1 0x0000000004300354 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4300354)
 #2 0x0000000004244d58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007efbff642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007efbff6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007efbff642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007efbff6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007efbff62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007efbff639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000730c349 CheckArityMismatch(clang::Sema&, clang::OverloadCandidate*, unsigned int, bool) (.constprop.0) SemaOverload.cpp:0:0
#10 0x0000000007353553 clang::OverloadCandidateSet::NoteCandidates(clang::Sema&, llvm::ArrayRef<clang::Expr*>, llvm::ArrayRef<clang::OverloadCandidate*>, llvm::StringRef, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7353553)
#11 0x0000000007354801 clang::OverloadCandidateSet::NoteCandidates(std::pair<clang::SourceLocation, clang::PartialDiagnostic>, clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, llvm::StringRef, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7354801)
#12 0x00000000073590ca FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#13 0x000000000735a230 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x735a230)
#14 0x0000000006fb890a clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb890a)
#15 0x0000000006fbde1c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fbde1c)
#16 0x0000000006abc1a7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abc1a7)
#17 0x0000000006ab50ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab50ba)
#18 0x0000000006ab7317 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab7317)
#19 0x0000000006ab73a9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab73a9)
#20 0x0000000006abb989 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abb989)
#21 0x0000000006b41249 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b41249)
#22 0x0000000006b386f4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b386f4)
#23 0x0000000006b390d6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b390d6)
#24 0x0000000006b41a1e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b41a1e)
#25 0x0000000006b4226a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4226a)
#26 0x0000000006a481cb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a481cb)
#27 0x0000000006a7e68d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e68d)
#28 0x0000000006a3b32b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3b32b)
#29 0x0000000006a3bb1f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3bb1f)
#30 0x0000000006a4394a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4394a)
#31 0x0000000006a448e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a448e5)
#32 0x0000000006a2596a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2596a)
#33 0x0000000004c64298 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64298)
#34 0x0000000004f5a915 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5a915)
#35 0x0000000004edabae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edabae)
#36 0x0000000005054c9d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5054c9d)
#37 0x0000000000de672c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde672c)
#38 0x0000000000ddd17a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#39 0x0000000000ddd2fd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#40 0x0000000004cd6969 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x00000000042451f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42451f4)
#42 0x0000000004cd6f7f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x0000000004c97a62 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c97a62)
#44 0x0000000004c98a0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c98a0e)
#45 0x0000000004c9fe65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9fe65)
#46 0x0000000000de2b21 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2b21)
#47 0x0000000000c8d454 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d454)
#48 0x00007efbff629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x00007efbff629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000ddcc15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddcc15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-19:
https://godbolt.org/z/EzfKzsPj1

Note: using `-std=c++03`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/rr94r1K8a
```cpp
constexpr void f(this auto&amp; self)

void g() {
  f();
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:12129: bool CheckArityMismatch(clang::Sema&amp;, clang::OverloadCandidate*, unsigned int, bool): Assertion `(Cand-&gt;FailureKind == ovl_fail_too_many_arguments) || (Cand-&gt;FailureKind == ovl_fail_bad_deduction &amp;&amp; Cand-&gt;DeductionFailure.getResult() == TemplateDeductionResult::TooManyArguments)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++03 &lt;source&gt;
1.	&lt;source&gt;:4:5: current parser token ')'
2.	&lt;source&gt;:3:10: parsing function body 'g'
3.	&lt;source&gt;:3:10: in compound statement ('{}')
 #<!-- -->0 0x0000000004302f28 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4302f28)
 #<!-- -->1 0x0000000004300354 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4300354)
 #<!-- -->2 0x0000000004244d58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007efbff642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007efbff6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007efbff642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007efbff6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007efbff62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007efbff639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000730c349 CheckArityMismatch(clang::Sema&amp;, clang::OverloadCandidate*, unsigned int, bool) (.constprop.0) SemaOverload.cpp:0:0
#<!-- -->10 0x0000000007353553 clang::OverloadCandidateSet::NoteCandidates(clang::Sema&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::ArrayRef&lt;clang::OverloadCandidate*&gt;, llvm::StringRef, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7353553)
#<!-- -->11 0x0000000007354801 clang::OverloadCandidateSet::NoteCandidates(std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;, clang::Sema&amp;, clang::OverloadCandidateDisplayKind, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::StringRef, clang::SourceLocation, llvm::function_ref&lt;bool (clang::OverloadCandidate&amp;)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7354801)
#<!-- -->12 0x00000000073590ca FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#<!-- -->13 0x000000000735a230 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x735a230)
#<!-- -->14 0x0000000006fb890a clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fb890a)
#<!-- -->15 0x0000000006fbde1c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fbde1c)
#<!-- -->16 0x0000000006abc1a7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abc1a7)
#<!-- -->17 0x0000000006ab50ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab50ba)
#<!-- -->18 0x0000000006ab7317 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab7317)
#<!-- -->19 0x0000000006ab73a9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab73a9)
#<!-- -->20 0x0000000006abb989 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abb989)
#<!-- -->21 0x0000000006b41249 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b41249)
#<!-- -->22 0x0000000006b386f4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b386f4)
#<!-- -->23 0x0000000006b390d6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b390d6)
#<!-- -->24 0x0000000006b41a1e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b41a1e)
#<!-- -->25 0x0000000006b4226a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4226a)
#<!-- -->26 0x0000000006a481cb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a481cb)
#<!-- -->27 0x0000000006a7e68d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7e68d)
#<!-- -->28 0x0000000006a3b32b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3b32b)
#<!-- -->29 0x0000000006a3bb1f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3bb1f)
#<!-- -->30 0x0000000006a4394a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4394a)
#<!-- -->31 0x0000000006a448e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a448e5)
#<!-- -->32 0x0000000006a2596a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2596a)
#<!-- -->33 0x0000000004c64298 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64298)
#<!-- -->34 0x0000000004f5a915 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5a915)
#<!-- -->35 0x0000000004edabae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edabae)
#<!-- -->36 0x0000000005054c9d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5054c9d)
#<!-- -->37 0x0000000000de672c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde672c)
#<!-- -->38 0x0000000000ddd17a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->39 0x0000000000ddd2fd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->40 0x0000000004cd6969 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x00000000042451f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42451f4)
#<!-- -->42 0x0000000004cd6f7f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x0000000004c97a62 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c97a62)
#<!-- -->44 0x0000000004c98a0e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c98a0e)
#<!-- -->45 0x0000000004c9fe65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9fe65)
#<!-- -->46 0x0000000000de2b21 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2b21)
#<!-- -->47 0x0000000000c8d454 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d454)
#<!-- -->48 0x00007efbff629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x00007efbff629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000ddcc15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddcc15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-19:
https://godbolt.org/z/EzfKzsPj1

Note: using `-std=c++03`
</details>


---

### Comment 2 - akparmar004

Could i be assigned to this issue

---

### Comment 3 - shafik

> Could i be assigned to this issue

We don't assign frontend issues until you have a PR and we confirm that it looks like it is heading in the right direction.

---

### Comment 4 - akparmar004

got it..

---

### Comment 5 - hax0kartik

Reduced testcase:

```C++
a void f(this int){
   f();
}
```

This is actually the same bug as https://github.com/llvm/llvm-project/issues/113185. However, the fix provided in commit https://github.com/llvm/llvm-project/pull/148807 only works when the return type is valid. An easy fix IMO, is to simply move the [relevant parts](https://github.com/llvm/llvm-project/blob/dff081c26f11e1679411e7c0b4012e6a740b6cc3/clang/lib/Sema/SemaType.cpp#L4840-L4895) of code before the `if (!D.isInvalidType())` check.


---

