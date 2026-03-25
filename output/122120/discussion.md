# [clang] Assertion `Initializer && "Initializer must be non-null"' failed

**Author:** ym-xie
**URL:** https://github.com/llvm/llvm-project/issues/122120

## Body

code:
```c
void *f() {
  int sp_r1 __asm__("ebx")((char *)sp_r1, 0);
}
```
just crash on assertion trunk
https://godbolt.org/z/YTcbYxd87
generated via fuzzing.
No related issues were found by searching [issue](https://github.com/llvm/llvm-project/issues?q=sort%3Aupdated-desc%20is%3Aissue%20is%3Aopen%20assertion%20Initializer%20must%20be%20non-null)

<details><summary><strong>Backtrace</strong></summary>
<p>

```
clang: /root/llvm-project/clang/lib/Sema/SemaInit.cpp:6593: void clang::InitializationSequence::InitializeFrom(clang::Sema&, const clang::InitializedEntity&, const clang::InitializationKind&, clang::MultiExprArg, bool, bool): Assertion `Initializer && "Initializer must be non-null"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2x -c <source>
1.	<source>:2:45: current parser token ';'
2.	<source>:1:11: parsing function body 'f'
3.	<source>:1:11: in compound statement ('{}')
 #0 0x0000000003c8d758 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c8d758)
 #1 0x0000000003c8b464 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c8b464)
 #2 0x0000000003bd89a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000775878642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007758786969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000775878642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007758786287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000077587862871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000775878639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006d0bff4 clang::InitializationSequence::InitializeFrom(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6d0bff4)
#10 0x00000000069cf647 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69cf647)
#11 0x0000000006664529 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6664529)
#12 0x0000000006673699 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6673699)
#13 0x000000000667bfb7 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667bfb7)
#14 0x000000000667c479 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667c479)
#15 0x0000000006729e02 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6729e02)
#16 0x000000000672ab30 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672ab30)
#17 0x000000000672b9f5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672b9f5)
#18 0x000000000672d1fa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672d1fa)
#19 0x000000000663f4c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663f4c3)
#20 0x000000000667407d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667407d)
#21 0x000000000663322e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663322e)
#22 0x00000000066339e9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66339e9)
#23 0x000000000663b193 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663b193)
#24 0x000000000663c06d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663c06d)
#25 0x000000000663c510 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663c510)
#26 0x000000000662e493 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662e493)
#27 0x000000000461f2b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x461f2b8)
#28 0x00000000048dbab9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48dbab9)
#29 0x000000000485e18e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x485e18e)
#30 0x00000000049c906e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c906e)
#31 0x0000000000ceda5f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xceda5f)
#32 0x0000000000ce54fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x0000000004664e49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x0000000003bd8e54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bd8e54)
#35 0x000000000466543f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x000000000462902d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x462902d)
#37 0x000000000462a0ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x462a0ae)
#38 0x0000000004631bd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4631bd5)
#39 0x0000000000cea8a3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcea8a3)
#40 0x0000000000bb6b84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbb6b84)
#41 0x0000775878629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x0000775878629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000ce4fa5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce4fa5)
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
code:
```c
void *f() {
  int sp_r1 __asm__("ebx")((char *)sp_r1, 0);
}
```
just crash on assertion trunk
https://godbolt.org/z/YTcbYxd87
generated via fuzzing.
No related issues were found by searching [issue](https://github.com/llvm/llvm-project/issues?q=sort%3Aupdated-desc%20is%3Aissue%20is%3Aopen%20assertion%20Initializer%20must%20be%20non-null)

&lt;details&gt;&lt;summary&gt;&lt;strong&gt;Backtrace&lt;/strong&gt;&lt;/summary&gt;
&lt;p&gt;

```
clang: /root/llvm-project/clang/lib/Sema/SemaInit.cpp:6593: void clang::InitializationSequence::InitializeFrom(clang::Sema&amp;, const clang::InitializedEntity&amp;, const clang::InitializationKind&amp;, clang::MultiExprArg, bool, bool): Assertion `Initializer &amp;&amp; "Initializer must be non-null"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2x -c &lt;source&gt;
1.	&lt;source&gt;:2:45: current parser token ';'
2.	&lt;source&gt;:1:11: parsing function body 'f'
3.	&lt;source&gt;:1:11: in compound statement ('{}')
 #<!-- -->0 0x0000000003c8d758 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c8d758)
 #<!-- -->1 0x0000000003c8b464 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3c8b464)
 #<!-- -->2 0x0000000003bd89a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000775878642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007758786969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000775878642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007758786287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000077587862871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000775878639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006d0bff4 clang::InitializationSequence::InitializeFrom(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6d0bff4)
#<!-- -->10 0x00000000069cf647 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69cf647)
#<!-- -->11 0x0000000006664529 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6664529)
#<!-- -->12 0x0000000006673699 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6673699)
#<!-- -->13 0x000000000667bfb7 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667bfb7)
#<!-- -->14 0x000000000667c479 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667c479)
#<!-- -->15 0x0000000006729e02 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6729e02)
#<!-- -->16 0x000000000672ab30 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672ab30)
#<!-- -->17 0x000000000672b9f5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672b9f5)
#<!-- -->18 0x000000000672d1fa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672d1fa)
#<!-- -->19 0x000000000663f4c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663f4c3)
#<!-- -->20 0x000000000667407d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667407d)
#<!-- -->21 0x000000000663322e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663322e)
#<!-- -->22 0x00000000066339e9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66339e9)
#<!-- -->23 0x000000000663b193 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663b193)
#<!-- -->24 0x000000000663c06d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663c06d)
#<!-- -->25 0x000000000663c510 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663c510)
#<!-- -->26 0x000000000662e493 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662e493)
#<!-- -->27 0x000000000461f2b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x461f2b8)
#<!-- -->28 0x00000000048dbab9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48dbab9)
#<!-- -->29 0x000000000485e18e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x485e18e)
#<!-- -->30 0x00000000049c906e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c906e)
#<!-- -->31 0x0000000000ceda5f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xceda5f)
#<!-- -->32 0x0000000000ce54fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x0000000004664e49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x0000000003bd8e54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3bd8e54)
#<!-- -->35 0x000000000466543f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x000000000462902d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x462902d)
#<!-- -->37 0x000000000462a0ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x462a0ae)
#<!-- -->38 0x0000000004631bd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4631bd5)
#<!-- -->39 0x0000000000cea8a3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xcea8a3)
#<!-- -->40 0x0000000000bb6b84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbb6b84)
#<!-- -->41 0x0000775878629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x0000775878629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000ce4fa5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xce4fa5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134

```

&lt;/p&gt;
&lt;/details&gt;
</details>


---

### Comment 2 - shafik

goes back to clang-2.7: https://godbolt.org/z/xx9nT5sYe

---

