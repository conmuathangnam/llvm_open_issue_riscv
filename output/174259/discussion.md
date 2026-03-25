# [clang] Assertion `R.getAsSingle<TemplateDecl>() && "There should only be one declaration found."' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/174259

## Body

Reproducer:
https://godbolt.org/z/TGh3GGW3o
```cpp
template<class > foo ; foo(foo<>
```

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00027.cpp

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExpr.cpp:2910: clang::ExprResult clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*): Assertion `R.getAsSingle<TemplateDecl>() && "There should only be one declaration found."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
 #0 0x0000000004305e68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4305e68)
 #1 0x0000000004303294 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4303294)
 #2 0x0000000004247c98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007468b0a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007468b0a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007468b0a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007468b0a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007468b0a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007468b0a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006f94ca8 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f94ca8)
#10 0x0000000006ad68a6 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad68a6)
#11 0x0000000006ad6b70 clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad6b70)
#12 0x0000000006aba8aa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aba8aa)
#13 0x0000000006abc027 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abc027)
#14 0x0000000006abc0b9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abc0b9)
#15 0x0000000006abc41e clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abc41e)
#16 0x0000000006a7520a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7520a)
#17 0x0000000006a8298a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8298a)
#18 0x0000000006a4003b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4003b)
#19 0x0000000006a4082f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4082f)
#20 0x0000000006a4865a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4865a)
#21 0x0000000006a495f5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a495f5)
#22 0x0000000006a2a67a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2a67a)
#23 0x0000000004c670f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c670f8)
#24 0x0000000004f5d8c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5d8c5)
#25 0x0000000004edda7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edda7e)
#26 0x0000000005057c4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5057c4d)
#27 0x0000000000de6fec cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6fec)
#28 0x0000000000ddda3a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000dddbbd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x0000000004cd97c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000004248134 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4248134)
#32 0x0000000004cd9ddf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000004c9a8c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a8c2)
#34 0x0000000004c9b86e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9b86e)
#35 0x0000000004ca2cc5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2cc5)
#36 0x0000000000de33e1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde33e1)
#37 0x0000000000c8dd14 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8dd14)
#38 0x00007468b0a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007468b0a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000ddd4d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd4d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-21:
https://godbolt.org/z/rMhE3er15

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/TGh3GGW3o
```cpp
template&lt;class &gt; foo ; foo(foo&lt;&gt;
```

The above reproducer is available at:
https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00027.cpp

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExpr.cpp:2910: clang::ExprResult clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*): Assertion `R.getAsSingle&lt;TemplateDecl&gt;() &amp;&amp; "There should only be one declaration found."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000004305e68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4305e68)
 #<!-- -->1 0x0000000004303294 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4303294)
 #<!-- -->2 0x0000000004247c98 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007468b0a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007468b0a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007468b0a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007468b0a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007468b0a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007468b0a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f94ca8 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f94ca8)
#<!-- -->10 0x0000000006ad68a6 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad68a6)
#<!-- -->11 0x0000000006ad6b70 clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad6b70)
#<!-- -->12 0x0000000006aba8aa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aba8aa)
#<!-- -->13 0x0000000006abc027 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abc027)
#<!-- -->14 0x0000000006abc0b9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abc0b9)
#<!-- -->15 0x0000000006abc41e clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abc41e)
#<!-- -->16 0x0000000006a7520a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a7520a)
#<!-- -->17 0x0000000006a8298a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8298a)
#<!-- -->18 0x0000000006a4003b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4003b)
#<!-- -->19 0x0000000006a4082f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4082f)
#<!-- -->20 0x0000000006a4865a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4865a)
#<!-- -->21 0x0000000006a495f5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a495f5)
#<!-- -->22 0x0000000006a2a67a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2a67a)
#<!-- -->23 0x0000000004c670f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c670f8)
#<!-- -->24 0x0000000004f5d8c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5d8c5)
#<!-- -->25 0x0000000004edda7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4edda7e)
#<!-- -->26 0x0000000005057c4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5057c4d)
#<!-- -->27 0x0000000000de6fec cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde6fec)
#<!-- -->28 0x0000000000ddda3a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000dddbbd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004cd97c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000004248134 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4248134)
#<!-- -->32 0x0000000004cd9ddf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x0000000004c9a8c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9a8c2)
#<!-- -->34 0x0000000004c9b86e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9b86e)
#<!-- -->35 0x0000000004ca2cc5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2cc5)
#<!-- -->36 0x0000000000de33e1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde33e1)
#<!-- -->37 0x0000000000c8dd14 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8dd14)
#<!-- -->38 0x00007468b0a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007468b0a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000ddd4d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddd4d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-21:
https://godbolt.org/z/rMhE3er15
</details>


---

### Comment 2 - shafik

@AaronBallman this bisect to 9eef4d1c5fa6b1bcbbe675c14ca8301d5d346f7b 

---

