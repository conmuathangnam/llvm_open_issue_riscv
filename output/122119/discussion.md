# [clang] Assertion `(isUnevaluated() || getCharByteWidth() == 1) && "This function is used in places that assume strings use char"' failed

**Author:** ym-xie
**URL:** https://github.com/llvm/llvm-project/issues/122119

## Body

code
```c
volatile float x = __builtin_nan("st    %1,%0"
                                 "Alpha Component"
                                 "%Lb"
                                 "aEgiH"
                                 u"foo");
```
start with clang-9.0.0(assertions)
https://godbolt.org/z/K4r5YEhYP
generated via fuzzing
No related issues were found by searching [issue](https://github.com/llvm/llvm-project/issues?q=sort%3Aupdated-desc%20is%3Aissue%20is%3Aopen%20assertion%20This%20function%20is%20used%20in%20places%20that%20assume%20strings%20use%20char)
<details><summary><strong>Backtrace</strong></summary>
<p>

```
clang: /root/llvm-project/clang/include/clang/AST/Expr.h:1856: llvm::StringRef clang::StringLiteral::getString() const: Assertion `(isUnevaluated() || getCharByteWidth() == 1) && "This function is used in places that assume strings use char"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2x -c <source>
1.	<source>:5:41: current parser token ';'
 #0 0x0000000003c8d758 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c8d758)
 #1 0x0000000003c8b464 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c8b464)
 #2 0x0000000003bd89a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000799ace842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000799ace8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000799ace842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000799ace8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000799ace82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000799ace839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000077a2ece (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x77a2ece)
#10 0x000000000780cb1c (anonymous namespace)::FloatExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#11 0x0000000007805313 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::FloatExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x00000000078073a1 EvaluateFloat(clang::Expr const*, llvm::APFloat&, (anonymous namespace)::EvalInfo&) ExprConstant.cpp:0:0
#13 0x00000000077ca3db Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#14 0x00000000077d2ddb EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#15 0x00000000077d5fa7 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x77d5fa7)
#16 0x0000000006895647 clang::Sema::CheckImplicitConversion(clang::Expr*, clang::QualType, clang::SourceLocation, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6895647)
#17 0x0000000006897325 AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#18 0x0000000006898fcd clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6898fcd)
#19 0x0000000006c3af1c clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c3af1c)
#20 0x00000000069cee65 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69cee65)
#21 0x00000000066645ea clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66645ea)
#22 0x0000000006673699 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6673699)
#23 0x000000000663322e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663322e)
#24 0x00000000066339e9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66339e9)
#25 0x000000000663b193 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663b193)
#26 0x000000000663c06d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663c06d)
#27 0x000000000663c510 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663c510)
#28 0x000000000662e493 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662e493)
#29 0x000000000461f2b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x461f2b8)
#30 0x00000000048dbab9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48dbab9)
#31 0x000000000485e18e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x485e18e)
#32 0x00000000049c906e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c906e)
#33 0x0000000000ceda5f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xceda5f)
#34 0x0000000000ce54fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x0000000004664e49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000003bd8e54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bd8e54)
#37 0x000000000466543f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x000000000462902d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x462902d)
#39 0x000000000462a0ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x462a0ae)
#40 0x0000000004631bd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4631bd5)
#41 0x0000000000cea8a3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcea8a3)
#42 0x0000000000bb6b84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbb6b84)
#43 0x0000799ace829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x0000799ace829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000ce4fa5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce4fa5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134

```

</p>
</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (wxie7)

<details>
code
```c
volatile float x = __builtin_nan("st    %1,%0"
                                 "Alpha Component"
                                 "%Lb"
                                 "aEgiH"
                                 u"foo");
```
just crash on assertions trunk
https://godbolt.org/z/4qsrvdqqh
generated via fuzzing
No related issues were found by searching [issue](https://github.com/llvm/llvm-project/issues?q=sort%3Aupdated-desc%20is%3Aissue%20is%3Aopen%20assertion%20This%20function%20is%20used%20in%20places%20that%20assume%20strings%20use%20char)
&lt;details&gt;&lt;summary&gt;&lt;strong&gt;Backtrace&lt;/strong&gt;&lt;/summary&gt;
&lt;p&gt;

```
clang: /root/llvm-project/clang/include/clang/AST/Expr.h:1856: llvm::StringRef clang::StringLiteral::getString() const: Assertion `(isUnevaluated() || getCharByteWidth() == 1) &amp;&amp; "This function is used in places that assume strings use char"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2x -c &lt;source&gt;
1.	&lt;source&gt;:5:41: current parser token ';'
 #<!-- -->0 0x0000000003c8d758 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c8d758)
 #<!-- -->1 0x0000000003c8b464 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c8b464)
 #<!-- -->2 0x0000000003bd89a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000799ace842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000799ace8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000799ace842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000799ace8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000799ace82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000799ace839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000077a2ece (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x77a2ece)
#<!-- -->10 0x000000000780cb1c (anonymous namespace)::FloatExprEvaluator::VisitCallExpr(clang::CallExpr const*) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007805313 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::FloatExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000078073a1 EvaluateFloat(clang::Expr const*, llvm::APFloat&amp;, (anonymous namespace)::EvalInfo&amp;) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000077ca3db Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00000000077d2ddb EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000077d5fa7 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x77d5fa7)
#<!-- -->16 0x0000000006895647 clang::Sema::CheckImplicitConversion(clang::Expr*, clang::QualType, clang::SourceLocation, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6895647)
#<!-- -->17 0x0000000006897325 AnalyzeImplicitConversions(clang::Sema&amp;, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#<!-- -->18 0x0000000006898fcd clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6898fcd)
#<!-- -->19 0x0000000006c3af1c clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c3af1c)
#<!-- -->20 0x00000000069cee65 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69cee65)
#<!-- -->21 0x00000000066645ea clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66645ea)
#<!-- -->22 0x0000000006673699 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6673699)
#<!-- -->23 0x000000000663322e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663322e)
#<!-- -->24 0x00000000066339e9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66339e9)
#<!-- -->25 0x000000000663b193 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663b193)
#<!-- -->26 0x000000000663c06d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663c06d)
#<!-- -->27 0x000000000663c510 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663c510)
#<!-- -->28 0x000000000662e493 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662e493)
#<!-- -->29 0x000000000461f2b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x461f2b8)
#<!-- -->30 0x00000000048dbab9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48dbab9)
#<!-- -->31 0x000000000485e18e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x485e18e)
#<!-- -->32 0x00000000049c906e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c906e)
#<!-- -->33 0x0000000000ceda5f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xceda5f)
#<!-- -->34 0x0000000000ce54fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004664e49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000003bd8e54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bd8e54)
#<!-- -->37 0x000000000466543f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x000000000462902d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x462902d)
#<!-- -->39 0x000000000462a0ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x462a0ae)
#<!-- -->40 0x0000000004631bd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4631bd5)
#<!-- -->41 0x0000000000cea8a3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcea8a3)
#<!-- -->42 0x0000000000bb6b84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbb6b84)
#<!-- -->43 0x0000799ace829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x0000799ace829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000ce4fa5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce4fa5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134

```

&lt;/p&gt;
&lt;/details&gt;
</details>


---

### Comment 2 - keinflue

Occurs since first support of `u` literal prefix in Clang 3.3.

Occurs only in C mode, not C++ mode.

Reduces to
```cpp
float x = __builtin_nan(u"ANY_STRING_HERE");
```
https://godbolt.org/z/h4hnabq56

Happens with other non-narrow string literals as well, e.g.
```cpp
float x = __builtin_nan(L"ANY_STRING_HERE");
```
since Clang 3.1.

---

