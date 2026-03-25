# [clang] expected non-matching UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaChecking.cpp:8812!

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/161075
**Status:** Closed
**Labels:** clang:frontend, regression, confirmed, crash
**Closed Date:** 2025-10-07T05:07:07Z

## Body

Reproducer:
https://godbolt.org/z/Yasd9chf1
```cpp
template <typename... Args>
void format(const char *fmt, Args &&...args)
    __attribute__((format(printf, 1, 2)));

void do_format() {
  bool b = false;
  format("%hhi %hhu %hi %hu %i %u", b, b, b, b, b, b);
}
```

Backtrace:
```console
expected non-matching
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaChecking.cpp:8812!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Weverything <source>
1.	<source>:7:53: current parser token ')'
2.	<source>:5:18: parsing function body 'do_format'
3.	<source>:5:18: in compound statement ('{}')
 #0 0x00000000041b0358 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0358)
 #1 0x00000000041ad784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad784)
 #2 0x00000000040f1ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007dac1ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007dac1ea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007dac1ea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007dac1ea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000040fd18a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40fd18a)
 #8 0x0000000006af0676 (anonymous namespace)::CheckPrintfHandler::HandlePrintfSpecifier(clang::analyze_printf::PrintfSpecifier const&, char const*, unsigned int, clang::TargetInfo const&) SemaChecking.cpp:0:0
 #9 0x0000000007df9e51 clang::analyze_format_string::ParsePrintfString(clang::analyze_format_string::FormatStringHandler&, char const*, char const*, clang::LangOptions const&, clang::TargetInfo const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df9e51)
#10 0x0000000006affb6a checkFormatStringExpr(clang::Sema&, clang::StringLiteral const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::FormatStringType, clang::VariadicCallType, bool, llvm::SmallBitVector&, (anonymous namespace)::UncoveredArgHandler&, llvm::APSInt, bool) SemaChecking.cpp:0:0
#11 0x0000000006affee3 clang::Sema::CheckFormatArguments(llvm::ArrayRef<clang::Expr const*>, clang::Sema::FormatArgumentPassingKind, clang::StringLiteral const*, unsigned int, unsigned int, clang::FormatStringType, clang::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6affee3)
#12 0x0000000006b00477 clang::Sema::CheckFormatArguments(clang::FormatAttr const*, llvm::ArrayRef<clang::Expr const*>, bool, clang::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b00477)
#13 0x0000000006b00eab clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, bool, clang::SourceLocation, clang::SourceRange, clang::VariadicCallType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b00eab)
#14 0x0000000006b0314c clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b0314c)
#15 0x0000000006ddd0cc clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ddd0cc)
#16 0x00000000071650a6 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#17 0x0000000007165d30 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7165d30)
#18 0x0000000006dd8aea clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd8aea)
#19 0x0000000006dddf6c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dddf6c)
#20 0x000000000690f927 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690f927)
#21 0x00000000069085fa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69085fa)
#22 0x000000000690a857 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a857)
#23 0x000000000690a8e9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a8e9)
#24 0x000000000690f119 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690f119)
#25 0x0000000006993819 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6993819)
#26 0x000000000698aba2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698aba2)
#27 0x000000000698ba19 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698ba19)
#28 0x0000000006993f2e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6993f2e)
#29 0x000000000699474a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699474a)
#30 0x000000000689b13b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x689b13b)
#31 0x00000000068d15cd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d15cd)
#32 0x000000000688e50e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688e50e)
#33 0x000000000688ecaf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688ecaf)
#34 0x0000000006896563 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6896563)
#35 0x0000000006897475 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897475)
#36 0x000000000687842a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687842a)
#37 0x0000000004aecd68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd68)
#38 0x0000000004dd8b75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b75)
#39 0x0000000004d54a0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d54a0e)
#40 0x0000000004ecf74d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf74d)
#41 0x0000000000db8ce0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8ce0)
#42 0x0000000000daf84a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#43 0x0000000000daf9cd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#44 0x0000000004b55ea9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#45 0x00000000040f1f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f54)
#46 0x0000000004b564bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#47 0x0000000004b17822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17822)
#48 0x0000000004b187ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187ce)
#49 0x0000000004b1ff05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#50 0x0000000000db5209 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5209)
#51 0x0000000000c66b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b74)
#52 0x00007dac1ea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#53 0x00007dac1ea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#54 0x0000000000daf2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/Yasd9chf1
```cpp
template &lt;typename... Args&gt;
void format(const char *fmt, Args &amp;&amp;...args)
    __attribute__((format(printf, 1, 2)));

void do_format() {
  bool b = false;
  format("%hhi %hhu %hi %hu %i %u", b, b, b, b, b, b);
}
```

Backtrace:
```console
expected non-matching
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaChecking.cpp:8812!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Weverything &lt;source&gt;
1.	&lt;source&gt;:7:53: current parser token ')'
2.	&lt;source&gt;:5:18: parsing function body 'do_format'
3.	&lt;source&gt;:5:18: in compound statement ('{}')
 #<!-- -->0 0x00000000041b0358 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b0358)
 #<!-- -->1 0x00000000041ad784 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ad784)
 #<!-- -->2 0x00000000040f1ab8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007dac1ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007dac1ea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007dac1ea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007dac1ea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00000000040fd18a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40fd18a)
 #<!-- -->8 0x0000000006af0676 (anonymous namespace)::CheckPrintfHandler::HandlePrintfSpecifier(clang::analyze_printf::PrintfSpecifier const&amp;, char const*, unsigned int, clang::TargetInfo const&amp;) SemaChecking.cpp:0:0
 #<!-- -->9 0x0000000007df9e51 clang::analyze_format_string::ParsePrintfString(clang::analyze_format_string::FormatStringHandler&amp;, char const*, char const*, clang::LangOptions const&amp;, clang::TargetInfo const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7df9e51)
#<!-- -->10 0x0000000006affb6a checkFormatStringExpr(clang::Sema&amp;, clang::StringLiteral const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::Sema::FormatArgumentPassingKind, unsigned int, unsigned int, clang::FormatStringType, clang::VariadicCallType, bool, llvm::SmallBitVector&amp;, (anonymous namespace)::UncoveredArgHandler&amp;, llvm::APSInt, bool) SemaChecking.cpp:0:0
#<!-- -->11 0x0000000006affee3 clang::Sema::CheckFormatArguments(llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::Sema::FormatArgumentPassingKind, clang::StringLiteral const*, unsigned int, unsigned int, clang::FormatStringType, clang::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6affee3)
#<!-- -->12 0x0000000006b00477 clang::Sema::CheckFormatArguments(clang::FormatAttr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::VariadicCallType, clang::SourceLocation, clang::SourceRange, llvm::SmallBitVector&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b00477)
#<!-- -->13 0x0000000006b00eab clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::SourceLocation, clang::SourceRange, clang::VariadicCallType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b00eab)
#<!-- -->14 0x0000000006b0314c clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b0314c)
#<!-- -->15 0x0000000006ddd0cc clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ddd0cc)
#<!-- -->16 0x00000000071650a6 FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#<!-- -->17 0x0000000007165d30 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7165d30)
#<!-- -->18 0x0000000006dd8aea clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd8aea)
#<!-- -->19 0x0000000006dddf6c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dddf6c)
#<!-- -->20 0x000000000690f927 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690f927)
#<!-- -->21 0x00000000069085fa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69085fa)
#<!-- -->22 0x000000000690a857 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a857)
#<!-- -->23 0x000000000690a8e9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a8e9)
#<!-- -->24 0x000000000690f119 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690f119)
#<!-- -->25 0x0000000006993819 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6993819)
#<!-- -->26 0x000000000698aba2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698aba2)
#<!-- -->27 0x000000000698ba19 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698ba19)
#<!-- -->28 0x0000000006993f2e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6993f2e)
#<!-- -->29 0x000000000699474a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699474a)
#<!-- -->30 0x000000000689b13b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x689b13b)
#<!-- -->31 0x00000000068d15cd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d15cd)
#<!-- -->32 0x000000000688e50e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688e50e)
#<!-- -->33 0x000000000688ecaf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688ecaf)
#<!-- -->34 0x0000000006896563 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6896563)
#<!-- -->35 0x0000000006897475 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897475)
#<!-- -->36 0x000000000687842a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687842a)
#<!-- -->37 0x0000000004aecd68 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aecd68)
#<!-- -->38 0x0000000004dd8b75 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd8b75)
#<!-- -->39 0x0000000004d54a0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d54a0e)
#<!-- -->40 0x0000000004ecf74d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecf74d)
#<!-- -->41 0x0000000000db8ce0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8ce0)
#<!-- -->42 0x0000000000daf84a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->43 0x0000000000daf9cd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->44 0x0000000004b55ea9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->45 0x00000000040f1f54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f1f54)
#<!-- -->46 0x0000000004b564bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->47 0x0000000004b17822 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b17822)
#<!-- -->48 0x0000000004b187ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b187ce)
#<!-- -->49 0x0000000004b1ff05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#<!-- -->50 0x0000000000db5209 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5209)
#<!-- -->51 0x0000000000c66b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc66b74)
#<!-- -->52 0x00007dac1ea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->53 0x00007dac1ea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->54 0x0000000000daf2e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf2e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Note: using `-Weverything`

---

### Comment 3 - k-arrows

This is trunk only:
https://godbolt.org/z/WTWE8qz1K

---

### Comment 4 - shafik

@DeanSturtevant1 this bisects to 4cdc3388b3964f72026091b17cd76e90d10144b6

and it tracks since this does indeed crash w/ `-Wformat-signedness`: https://godbolt.org/z/bW5bj43Tf

---

