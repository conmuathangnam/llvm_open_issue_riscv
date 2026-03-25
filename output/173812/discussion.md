# [clang] llvm_unreachable("No way to get here unless both had cpu_dispatch")

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173812

## Body

Reproducer:
https://godbolt.org/z/E3eGecv68
```c
int __attribute__((cpu_specific(INVALID))) foo(void){
  return 1;}
int __attribute__((cpu_specific(pentium_iii))) foo(struct non_fam_struct *ptr, int size){
  return 2;}
int __attribute__((cpu_specific(pentium_4))) foo(void){
  return 3;}

int bar(void) {
  return foo();
}
```

Backtrace:
```console
No way to get here unless both had cpu_dispatch
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:10676!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<source>:9:14: current parser token ')'
2.	<source>:8:15: parsing function body 'bar'
3.	<source>:8:15: in compound statement ('{}')
 #0 0x00000000042ef648 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ef648)
 #1 0x00000000042eca74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eca74)
 #2 0x0000000004231818 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000075032f442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000075032f4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000075032f442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000075032f4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000000000423c44a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423c44a)
 #8 0x0000000007304593 clang::isBetterOverloadCandidate(clang::Sema&, clang::OverloadCandidate const&, clang::OverloadCandidate const&, clang::SourceLocation, clang::OverloadCandidateSet::CandidateSetKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7304593)
 #9 0x000000000730cb7c clang::OverloadCandidateSet::BestViableFunctionImpl(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x730cb7c)
#10 0x0000000007327884 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7327884)
#11 0x0000000007340a5b clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7340a5b)
#12 0x0000000006f9f2ba clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9f2ba)
#13 0x0000000006fa47cc clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa47cc)
#14 0x0000000006aa2b57 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa2b57)
#15 0x0000000006a9ba6a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9ba6a)
#16 0x0000000006a9dcc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dcc7)
#17 0x0000000006a9dd59 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dd59)
#18 0x0000000006aa2339 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa2339)
#19 0x0000000006b1a6f5 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1a6f5)
#20 0x0000000006b1eb6d clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1eb6d)
#21 0x0000000006b1fa86 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1fa86)
#22 0x0000000006b283ce clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b283ce)
#23 0x0000000006b28c1a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b28c1a)
#24 0x0000000006a2eb7b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2eb7b)
#25 0x0000000006a6503d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6503d)
#26 0x0000000006a21cdb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a21cdb)
#27 0x0000000006a224cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a224cf)
#28 0x0000000006a2a2fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2a2fa)
#29 0x0000000006a2b295 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b295)
#30 0x0000000006a0c31a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0c31a)
#31 0x0000000004c50978 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c50978)
#32 0x0000000004f46ff5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f46ff5)
#33 0x0000000004ec728e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec728e)
#34 0x000000000504137d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x504137d)
#35 0x0000000000de5c4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5c4c)
#36 0x0000000000ddc69a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#37 0x0000000000ddc81d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#38 0x0000000004cc3049 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x0000000004231cb4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231cb4)
#40 0x0000000004cc365f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x0000000004c84142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c84142)
#42 0x0000000004c850ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c850ee)
#43 0x0000000004c8c545 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8c545)
#44 0x0000000000de2041 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2041)
#45 0x0000000000c8d094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d094)
#46 0x000075032f429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x000075032f429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000ddc135 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc135)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/E3eGecv68
```c
int __attribute__((cpu_specific(INVALID))) foo(void){
  return 1;}
int __attribute__((cpu_specific(pentium_iii))) foo(struct non_fam_struct *ptr, int size){
  return 2;}
int __attribute__((cpu_specific(pentium_4))) foo(void){
  return 3;}

int bar(void) {
  return foo();
}
```

Backtrace:
```console
No way to get here unless both had cpu_dispatch
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:10676!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c &lt;source&gt;
1.	&lt;source&gt;:9:14: current parser token ')'
2.	&lt;source&gt;:8:15: parsing function body 'bar'
3.	&lt;source&gt;:8:15: in compound statement ('{}')
 #<!-- -->0 0x00000000042ef648 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ef648)
 #<!-- -->1 0x00000000042eca74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eca74)
 #<!-- -->2 0x0000000004231818 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000075032f442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000075032f4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000075032f442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000075032f4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000000000423c44a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423c44a)
 #<!-- -->8 0x0000000007304593 clang::isBetterOverloadCandidate(clang::Sema&amp;, clang::OverloadCandidate const&amp;, clang::OverloadCandidate const&amp;, clang::SourceLocation, clang::OverloadCandidateSet::CandidateSetKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7304593)
 #<!-- -->9 0x000000000730cb7c clang::OverloadCandidateSet::BestViableFunctionImpl(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x730cb7c)
#<!-- -->10 0x0000000007327884 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7327884)
#<!-- -->11 0x0000000007340a5b clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7340a5b)
#<!-- -->12 0x0000000006f9f2ba clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9f2ba)
#<!-- -->13 0x0000000006fa47cc clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa47cc)
#<!-- -->14 0x0000000006aa2b57 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa2b57)
#<!-- -->15 0x0000000006a9ba6a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9ba6a)
#<!-- -->16 0x0000000006a9dcc7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dcc7)
#<!-- -->17 0x0000000006a9dd59 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9dd59)
#<!-- -->18 0x0000000006aa2339 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa2339)
#<!-- -->19 0x0000000006b1a6f5 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1a6f5)
#<!-- -->20 0x0000000006b1eb6d clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1eb6d)
#<!-- -->21 0x0000000006b1fa86 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1fa86)
#<!-- -->22 0x0000000006b283ce clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b283ce)
#<!-- -->23 0x0000000006b28c1a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b28c1a)
#<!-- -->24 0x0000000006a2eb7b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2eb7b)
#<!-- -->25 0x0000000006a6503d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6503d)
#<!-- -->26 0x0000000006a21cdb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a21cdb)
#<!-- -->27 0x0000000006a224cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a224cf)
#<!-- -->28 0x0000000006a2a2fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2a2fa)
#<!-- -->29 0x0000000006a2b295 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b295)
#<!-- -->30 0x0000000006a0c31a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0c31a)
#<!-- -->31 0x0000000004c50978 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c50978)
#<!-- -->32 0x0000000004f46ff5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f46ff5)
#<!-- -->33 0x0000000004ec728e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec728e)
#<!-- -->34 0x000000000504137d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x504137d)
#<!-- -->35 0x0000000000de5c4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5c4c)
#<!-- -->36 0x0000000000ddc69a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->37 0x0000000000ddc81d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000004cc3049 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->39 0x0000000004231cb4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4231cb4)
#<!-- -->40 0x0000000004cc365f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->41 0x0000000004c84142 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c84142)
#<!-- -->42 0x0000000004c850ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c850ee)
#<!-- -->43 0x0000000004c8c545 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8c545)
#<!-- -->44 0x0000000000de2041 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2041)
#<!-- -->45 0x0000000000c8d094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d094)
#<!-- -->46 0x000075032f429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->47 0x000075032f429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->48 0x0000000000ddc135 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc135)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-8.0.0:
https://godbolt.org/z/ajMWjTYd1

---

### Comment 3 - shafik

Some of these recent issue are a little off the beaten track, so maybe not bringing as much value as others. 

---

### Comment 4 - k-arrows

> Some of these recent issue are a little off the beaten track, so maybe not bringing as much value as others.

Agreed — for the LLVM community, detecting regressions is likely more valuable.

By the way, the reproducer here was written based on the following program:
https://github.com/llvm/llvm-project/blob/main/clang/test/Sema/attr-cpuspecific.c

---

