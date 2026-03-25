# clang crash on x86_64-linux-gnu: Assertion `Value > 0 && "Value must not be 0"' failed

**Author:** bi6c
**URL:** https://github.com/llvm/llvm-project/issues/120082

## Body

Compiler Explorer: https://godbolt.org/z/9KqosxEdc
```console
<source>:3:47: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const volatile void *' [-Wint-conversion]
    3 | _Static_assert (__atomic_always_lock_free (0, 1), "");
      |                                               ^
clang: /root/llvm-project/llvm/include/llvm/Support/Alignment.h:77: llvm::Align::Align(uint64_t): Assertion `Value > 0 && "Value must not be 0"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
 #0 0x0000000003c56df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c56df8)
 #1 0x0000000003c54b04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c54b04)
 #2 0x0000000003ba2068 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e8f0de42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e8f0de969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e8f0de42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e8f0de287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007e8f0de2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007e8f0de39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000077a80cd (anonymous namespace)::IntExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) ExprConstant.cpp:0:0
#10 0x00000000077a9322 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#11 0x000000000774c78e clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x000000000773bc16 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#13 0x00000000077444eb EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#14 0x0000000007747358 CheckEvalInICE(clang::Expr const*, clang::ASTContext const&) ExprConstant.cpp:0:0
#15 0x0000000007748a2b CheckICE(clang::Expr const*, clang::ASTContext const&) ExprConstant.cpp:0:0
#16 0x00000000077492df clang::Expr::isIntegerConstantExpr(clang::ASTContext const&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x77492df)
#17 0x0000000006adcbee clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::Sema::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6adcbee)
#18 0x0000000006add712 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, unsigned int, clang::Sema::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6add712)
#19 0x00000000069fca24 clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69fca24)
#20 0x0000000006629d24 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6629d24)
#21 0x0000000006618009 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6618009)
#22 0x00000000065d6e47 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d6e47)
#23 0x00000000065d7ccd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d7ccd)
#24 0x00000000065ca20a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ca20a)
#25 0x00000000045e0668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45e0668)
#26 0x0000000004899a89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4899a89)
#27 0x000000000481c48e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481c48e)
#28 0x00000000049862ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49862ce)
#29 0x0000000000ce1c7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce1c7f)
#30 0x0000000000cd971a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#31 0x0000000004624ad9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#32 0x0000000003ba2514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ba2514)
#33 0x00000000046250cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#34 0x00000000045ea3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45ea3dd)
#35 0x00000000045eb4cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45eb4cd)
#36 0x00000000045f2a45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45f2a45)
#37 0x0000000000cdeac3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcdeac3)
#38 0x0000000000bac364 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbac364)
#39 0x00007e8f0de29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#40 0x00007e8f0de29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#41 0x0000000000cd91c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcd91c5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
int *p;
_Static_assert (__atomic_always_lock_free (1, p), "");
_Static_assert (__atomic_always_lock_free (0, 1), "");
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (bi6c)

<details>
Compiler Explorer: https://godbolt.org/z/9KqosxEdc
```console
&lt;source&gt;:3:47: error: incompatible integer to pointer conversion passing 'int' to parameter of type 'const volatile void *' [-Wint-conversion]
    3 | _Static_assert (__atomic_always_lock_free (0, 1), "");
      |                                               ^
clang: /root/llvm-project/llvm/include/llvm/Support/Alignment.h:77: llvm::Align::Align(uint64_t): Assertion `Value &gt; 0 &amp;&amp; "Value must not be 0"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -fno-verbose-asm -c --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000003c56df8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c56df8)
 #<!-- -->1 0x0000000003c54b04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c54b04)
 #<!-- -->2 0x0000000003ba2068 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e8f0de42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e8f0de969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e8f0de42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e8f0de287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007e8f0de2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007e8f0de39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000077a80cd (anonymous namespace)::IntExprEvaluator::VisitBuiltinCallExpr(clang::CallExpr const*, unsigned int) ExprConstant.cpp:0:0
#<!-- -->10 0x00000000077a9322 (anonymous namespace)::IntExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->11 0x000000000774c78e clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->12 0x000000000773bc16 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000077444eb EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007747358 CheckEvalInICE(clang::Expr const*, clang::ASTContext const&amp;) ExprConstant.cpp:0:0
#<!-- -->15 0x0000000007748a2b CheckICE(clang::Expr const*, clang::ASTContext const&amp;) ExprConstant.cpp:0:0
#<!-- -->16 0x00000000077492df clang::Expr::isIntegerConstantExpr(clang::ASTContext const&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x77492df)
#<!-- -->17 0x0000000006adcbee clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&amp;, clang::Sema::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6adcbee)
#<!-- -->18 0x0000000006add712 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, unsigned int, clang::Sema::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6add712)
#<!-- -->19 0x00000000069fca24 clang::Sema::BuildStaticAssertDeclaration(clang::SourceLocation, clang::Expr*, clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69fca24)
#<!-- -->20 0x0000000006629d24 clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6629d24)
#<!-- -->21 0x0000000006618009 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6618009)
#<!-- -->22 0x00000000065d6e47 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d6e47)
#<!-- -->23 0x00000000065d7ccd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65d7ccd)
#<!-- -->24 0x00000000065ca20a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x65ca20a)
#<!-- -->25 0x00000000045e0668 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45e0668)
#<!-- -->26 0x0000000004899a89 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4899a89)
#<!-- -->27 0x000000000481c48e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481c48e)
#<!-- -->28 0x00000000049862ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49862ce)
#<!-- -->29 0x0000000000ce1c7f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce1c7f)
#<!-- -->30 0x0000000000cd971a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->31 0x0000000004624ad9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->32 0x0000000003ba2514 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ba2514)
#<!-- -->33 0x00000000046250cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->34 0x00000000045ea3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45ea3dd)
#<!-- -->35 0x00000000045eb4cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45eb4cd)
#<!-- -->36 0x00000000045f2a45 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x45f2a45)
#<!-- -->37 0x0000000000cdeac3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcdeac3)
#<!-- -->38 0x0000000000bac364 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbac364)
#<!-- -->39 0x00007e8f0de29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->40 0x00007e8f0de29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->41 0x0000000000cd91c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcd91c5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
Code:
```c
int *p;
_Static_assert (__atomic_always_lock_free (1, p), "");
_Static_assert (__atomic_always_lock_free (0, 1), "");
```
</details>


---

### Comment 2 - shafik

Goes back to clang-3.1: https://godbolt.org/z/KKjYvGea8

---

