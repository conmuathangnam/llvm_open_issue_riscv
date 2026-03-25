# [Clang] frontend crashes with: bool clang::CastExpr::CastConsistency() const: Assertion `!getType()->isBooleanType() && "unheralded conversion to bool"' failed.

**Author:** zitongzhoueric
**URL:** https://github.com/llvm/llvm-project/issues/130446
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-invalid
**Closed Date:** 2025-03-09T05:40:41Z

## Body

To reproduce: https://godbolt.org/z/Khe7cj3Ee
Clang frontend crashes when trying to do int to bool conversion on undefined identifier:

```c
#include <stdio.h>

typedef _Bool bool ; 
const bool false = 0 ; 

int main(void) {
    // bool value = false; // uncomment fix the problem
    switch (value) {
        case 1:
            printf("Is 1\n");
            break;
        default:
            printf("I don't know\n");
    }
}
```
This is related to https://github.com/llvm/llvm-project/issues/108326, which was fixed by https://github.com/llvm/llvm-project/pull/108657. 
But the problem seems to persist when the value to be converted is undefined. Adding back the definition fixes the problem.

Stack Dump:
```
clang: /root/llvm-project/clang/lib/AST/Expr.cpp:1949: bool clang::CastExpr::CastConsistency() const: Assertion `!getType()->isBooleanType() && "unheralded conversion to bool"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:9:15: current parser token ':'
2.	<source>:6:16: parsing function body 'main'
3.	<source>:6:16: in compound statement ('{}')
4.	<source>:8:20: in compound statement ('{}')
 #0 0x0000000003e6b938 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e6b938)
 #1 0x0000000003e695f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e695f4)
 #2 0x0000000003db5f08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070b139642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070b1396969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070b139642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070b1396287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070b13962871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070b139639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000765dba0 clang::CastExpr::CastConsistency() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x765dba0)
#10 0x000000000765eb1d clang::ImplicitCastExpr::Create(clang::ASTContext const&, clang::QualType, clang::CastKind, clang::Expr*, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u> const*, clang::ExprValueKind, clang::FPOptionsOverride) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x765eb1d)
#11 0x00000000065bd8b1 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u> const*, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65bd8b1)
#12 0x0000000006d820af clang::Sema::ActOnCaseExpr(clang::SourceLocation, clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6d820af)
#13 0x00000000064aca72 clang::Parser::ParseCaseExpression(clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64aca72)
#14 0x0000000006527bd3 clang::Parser::ParseCaseStatement(clang::Parser::ParsedStmtContext, bool, clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6527bd3)
#15 0x000000000651f253 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x651f253)
#16 0x00000000065208e0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65208e0)
#17 0x00000000065217d1 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65217d1)
#18 0x0000000006521ea4 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6521ea4)
#19 0x000000000651e957 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x651e957)
#20 0x00000000065208e0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65208e0)
#21 0x0000000006523148 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6523148)
#22 0x0000000006525a7d clang::Parser::ParseSwitchStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6525a7d)
#23 0x000000000651f4a7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x651f4a7)
#24 0x00000000065208e0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65208e0)
#25 0x00000000065217d1 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65217d1)
#26 0x0000000006522fba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6522fba)
#27 0x00000000064347e3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64347e3)
#28 0x000000000646959d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x646959d)
#29 0x000000000642851e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x642851e)
#30 0x0000000006428cd9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6428cd9)
#31 0x00000000064304b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64304b3)
#32 0x000000000643138d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x643138d)
#33 0x000000000642381a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x642381a)
#34 0x000000000482f028 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x482f028)
#35 0x0000000004af7965 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4af7965)
#36 0x0000000004a7b13e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a7b13e)
#37 0x0000000004be662e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4be662e)
#38 0x0000000000d522e7 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd522e7)
#39 0x0000000000d490aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x0000000004877429 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000003db63b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3db63b4)
#42 0x0000000004877a1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x000000000483a1fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x483a1fd)
#44 0x000000000483b27e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x483b27e)
#45 0x0000000004843195 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4843195)
#46 0x0000000000d4e583 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd4e583)
#47 0x0000000000c16164 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc16164)
#48 0x000070b139629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x000070b139629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000d48b55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd48b55)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eric Zitong Zhou (zitongzhoueric)

<details>
To reproduce: https://godbolt.org/z/Khe7cj3Ee
Clang frontend crashes when trying to do int to bool conversion on undefined identifier:

```c
#include &lt;stdio.h&gt;

typedef _Bool bool ; 
const bool false = 0 ; 

int main(void) {
    // bool value = false; // uncomment fix the problem
    switch (value) {
        case 1:
            printf("Is 1\n");
            break;
        default:
            printf("I don't know\n");
    }
}
```
This is related to https://github.com/llvm/llvm-project/issues/108326, which was fixed by https://github.com/llvm/llvm-project/pull/108657. 
But the problem seems to persist when the value to be converted is undefined. Adding back the definition fixes the problem.

Stack Dump:
```
clang: /root/llvm-project/clang/lib/AST/Expr.cpp:1949: bool clang::CastExpr::CastConsistency() const: Assertion `!getType()-&gt;isBooleanType() &amp;&amp; "unheralded conversion to bool"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:9:15: current parser token ':'
2.	&lt;source&gt;:6:16: parsing function body 'main'
3.	&lt;source&gt;:6:16: in compound statement ('{}')
4.	&lt;source&gt;:8:20: in compound statement ('{}')
 #<!-- -->0 0x0000000003e6b938 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e6b938)
 #<!-- -->1 0x0000000003e695f4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3e695f4)
 #<!-- -->2 0x0000000003db5f08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070b139642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070b1396969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070b139642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070b1396287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070b13962871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070b139639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000765dba0 clang::CastExpr::CastConsistency() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x765dba0)
#<!-- -->10 0x000000000765eb1d clang::ImplicitCastExpr::Create(clang::ASTContext const&amp;, clang::QualType, clang::CastKind, clang::Expr*, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt; const*, clang::ExprValueKind, clang::FPOptionsOverride) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x765eb1d)
#<!-- -->11 0x00000000065bd8b1 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt; const*, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65bd8b1)
#<!-- -->12 0x0000000006d820af clang::Sema::ActOnCaseExpr(clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6d820af)
#<!-- -->13 0x00000000064aca72 clang::Parser::ParseCaseExpression(clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64aca72)
#<!-- -->14 0x0000000006527bd3 clang::Parser::ParseCaseStatement(clang::Parser::ParsedStmtContext, bool, clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6527bd3)
#<!-- -->15 0x000000000651f253 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x651f253)
#<!-- -->16 0x00000000065208e0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65208e0)
#<!-- -->17 0x00000000065217d1 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65217d1)
#<!-- -->18 0x0000000006521ea4 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6521ea4)
#<!-- -->19 0x000000000651e957 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x651e957)
#<!-- -->20 0x00000000065208e0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65208e0)
#<!-- -->21 0x0000000006523148 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6523148)
#<!-- -->22 0x0000000006525a7d clang::Parser::ParseSwitchStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6525a7d)
#<!-- -->23 0x000000000651f4a7 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x651f4a7)
#<!-- -->24 0x00000000065208e0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65208e0)
#<!-- -->25 0x00000000065217d1 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65217d1)
#<!-- -->26 0x0000000006522fba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6522fba)
#<!-- -->27 0x00000000064347e3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64347e3)
#<!-- -->28 0x000000000646959d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x646959d)
#<!-- -->29 0x000000000642851e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x642851e)
#<!-- -->30 0x0000000006428cd9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6428cd9)
#<!-- -->31 0x00000000064304b3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64304b3)
#<!-- -->32 0x000000000643138d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x643138d)
#<!-- -->33 0x000000000642381a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x642381a)
#<!-- -->34 0x000000000482f028 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x482f028)
#<!-- -->35 0x0000000004af7965 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4af7965)
#<!-- -->36 0x0000000004a7b13e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4a7b13e)
#<!-- -->37 0x0000000004be662e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4be662e)
#<!-- -->38 0x0000000000d522e7 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd522e7)
#<!-- -->39 0x0000000000d490aa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->40 0x0000000004877429 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x0000000003db63b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3db63b4)
#<!-- -->42 0x0000000004877a1f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x000000000483a1fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x483a1fd)
#<!-- -->44 0x000000000483b27e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x483b27e)
#<!-- -->45 0x0000000004843195 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4843195)
#<!-- -->46 0x0000000000d4e583 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd4e583)
#<!-- -->47 0x0000000000c16164 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc16164)
#<!-- -->48 0x000070b139629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x000070b139629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000d48b55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd48b55)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

Looks like this goes back to clang-3.6: https://godbolt.org/z/1xn8e7395

---

### Comment 3 - shafik

Couple of comments, it is always good to identify how far back a bug goes, so we can know if it is a regression or not.

For C where we don't have assertion builds going back you can use C++ w/ `-x c` and this works for everything I have seen.

Also please, make sure you try and find duplicates and if you find it just append your example to the duplicate. 

---

