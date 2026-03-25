# [clang] crash while recovering from invalid template-id

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/183505
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, regression:21
**Closed Date:** 2026-02-27T20:27:28Z

## Body

Reproducer:
https://godbolt.org/z/EscE9TKqj
```cpp
int ab[7];

int[(int)(abc<>)];
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = clang::UnresolvedLookupExpr; From = clang::Expr]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:3:17: current parser token ']'
 #0 0x0000000004405028 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4405028)
 #1 0x0000000004402474 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4402474)
 #2 0x0000000004342448 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ff176642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ff1766969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ff176642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ff1766287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ff17662871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ff176639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000070d2383 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70d2383)
#10 0x000000000712c474 clang::Sema::CheckPlaceholderExpr(clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x712c474)
#11 0x0000000006dfd78e (anonymous namespace)::CastOperation::CheckCXXCStyleCast(bool, bool) SemaCast.cpp:0:0
#12 0x0000000006e00613 clang::Sema::BuildCStyleCastExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e00613)
#13 0x000000000715b04d clang::Sema::ActOnCastExpr(clang::Scope*, clang::SourceLocation, clang::Declarator&, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x715b04d)
#14 0x0000000006c5f274 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c5f274)
#15 0x0000000006c55200 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c55200)
#16 0x0000000006c6127e clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6127e)
#17 0x0000000006c6138a clang::Parser::ParseArrayBoundExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6138a)
#18 0x0000000006c1af59 clang::Parser::ParseBracketDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1af59)
#19 0x0000000006c2aa9b clang::Parser::ParseMisplacedBracketDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2aa9b)
#20 0x0000000006c2c0ca clang::Parser::ParseDecompositionDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2c0ca)
#21 0x0000000006c2d234 clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2d234)
#22 0x0000000006c1a26f clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1a26f)
#23 0x00000000086ffc71 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x86ffc71)
#24 0x0000000006c1cf8f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1cf8f)
#25 0x0000000006bdb44b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bdb44b)
#26 0x0000000006bdbc4f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bdbc4f)
#27 0x0000000006be3251 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be3251)
#28 0x0000000006be4185 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be4185)
#29 0x0000000006bc586a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc586a)
#30 0x0000000004edcac8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edcac8)
#31 0x00000000051e3185 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51e3185)
#32 0x00000000051615ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51615ce)
#33 0x00000000052def4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52def4d)
#34 0x0000000000defeee cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdefeee)
#35 0x0000000000de687a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#36 0x0000000000de69fd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#37 0x0000000004f51229 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x0000000004342884 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4342884)
#39 0x0000000004f5183f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x0000000004f11522 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f11522)
#41 0x0000000004f124ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f124ce)
#42 0x0000000004f1a215 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a215)
#43 0x0000000000dec269 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdec269)
#44 0x0000000000ca465a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca465a)
#45 0x00007ff176629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#46 0x00007ff176629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#47 0x0000000000de6315 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6315)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-21:
https://godbolt.org/z/8n5G3Wcvs

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/EscE9TKqj
```cpp
int ab[7];

int[(int)(abc&lt;&gt;)];
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = clang::UnresolvedLookupExpr; From = clang::Expr]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:3:17: current parser token ']'
 #<!-- -->0 0x0000000004405028 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4405028)
 #<!-- -->1 0x0000000004402474 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4402474)
 #<!-- -->2 0x0000000004342448 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ff176642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007ff1766969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007ff176642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007ff1766287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007ff17662871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007ff176639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000070d2383 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70d2383)
#<!-- -->10 0x000000000712c474 clang::Sema::CheckPlaceholderExpr(clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x712c474)
#<!-- -->11 0x0000000006dfd78e (anonymous namespace)::CastOperation::CheckCXXCStyleCast(bool, bool) SemaCast.cpp:0:0
#<!-- -->12 0x0000000006e00613 clang::Sema::BuildCStyleCastExpr(clang::SourceLocation, clang::TypeSourceInfo*, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e00613)
#<!-- -->13 0x000000000715b04d clang::Sema::ActOnCastExpr(clang::Scope*, clang::SourceLocation, clang::Declarator&amp;, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x715b04d)
#<!-- -->14 0x0000000006c5f274 clang::Parser::ParseParenExpression(clang::ParenParseOption&amp;, bool, clang::ParenExprKind, clang::TypoCorrectionTypeBehavior, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c5f274)
#<!-- -->15 0x0000000006c55200 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c55200)
#<!-- -->16 0x0000000006c6127e clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6127e)
#<!-- -->17 0x0000000006c6138a clang::Parser::ParseArrayBoundExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6138a)
#<!-- -->18 0x0000000006c1af59 clang::Parser::ParseBracketDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1af59)
#<!-- -->19 0x0000000006c2aa9b clang::Parser::ParseMisplacedBracketDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2aa9b)
#<!-- -->20 0x0000000006c2c0ca clang::Parser::ParseDecompositionDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2c0ca)
#<!-- -->21 0x0000000006c2d234 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2d234)
#<!-- -->22 0x0000000006c1a26f clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1a26f)
#<!-- -->23 0x00000000086ffc71 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x86ffc71)
#<!-- -->24 0x0000000006c1cf8f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1cf8f)
#<!-- -->25 0x0000000006bdb44b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bdb44b)
#<!-- -->26 0x0000000006bdbc4f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bdbc4f)
#<!-- -->27 0x0000000006be3251 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be3251)
#<!-- -->28 0x0000000006be4185 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be4185)
#<!-- -->29 0x0000000006bc586a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc586a)
#<!-- -->30 0x0000000004edcac8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edcac8)
#<!-- -->31 0x00000000051e3185 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51e3185)
#<!-- -->32 0x00000000051615ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51615ce)
#<!-- -->33 0x00000000052def4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x52def4d)
#<!-- -->34 0x0000000000defeee cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdefeee)
#<!-- -->35 0x0000000000de687a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->36 0x0000000000de69fd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->37 0x0000000004f51229 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->38 0x0000000004342884 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4342884)
#<!-- -->39 0x0000000004f5183f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->40 0x0000000004f11522 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f11522)
#<!-- -->41 0x0000000004f124ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f124ce)
#<!-- -->42 0x0000000004f1a215 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1a215)
#<!-- -->43 0x0000000000dec269 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdec269)
#<!-- -->44 0x0000000000ca465a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca465a)
#<!-- -->45 0x00007ff176629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->46 0x00007ff176629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->47 0x0000000000de6315 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6315)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - shafik

@AaronBallman another one linked to this PR: 9eef4d1c5fa6b1bcbbe675c14ca8301d5d346f7b

---

