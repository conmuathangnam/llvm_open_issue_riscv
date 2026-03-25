# [clang][OpenMP] Assertion `!isTokenSpecial() && "Should consume special tokens with Consume*Token"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/158481

## Body

Reproducer:
https://godbolt.org/z/8b4917Te5
```c
void foo(int **x) {
#pragma omp target map(iterator(i = 0 : 42), iterator(j = 0 : 42), to : x[i][j])
  ;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Parse/Parser.h:263: clang::SourceLocation clang::Parser::ConsumeToken(): Assertion `!isTokenSpecial() && "Should consume special tokens with Consume*Token"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fopenmp <source>
1.	<source>:2:54: current parser token '('
2.	<source>:1:19: parsing function body 'foo'
3.	<source>:1:19: in compound statement ('{}')
 #0 0x0000000004177998 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4177998)
 #1 0x0000000004174dc4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4174dc4)
 #2 0x00000000040b9368 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00006ffd48e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00006ffd48e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00006ffd48e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00006ffd48e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00006ffd48e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00006ffd48e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006911f3e clang::Parser::parseMapTypeModifiers(clang::SemaOpenMP::OpenMPVarListDataTy&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6911f3e)
#10 0x0000000006914d0d clang::Parser::ParseOpenMPVarList(llvm::omp::Directive, llvm::omp::Clause, llvm::SmallVectorImpl<clang::Expr*>&, clang::SemaOpenMP::OpenMPVarListDataTy&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6914d0d)
#11 0x000000000691676d clang::Parser::ParseOpenMPVarListClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691676d)
#12 0x0000000006916c71 clang::Parser::ParseOpenMPClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6916c71)
#13 0x000000000691859b clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691859b)
#14 0x0000000006925d24 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6925d24)
#15 0x0000000006944a84 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6944a84)
#16 0x0000000006946739 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6946739)
#17 0x000000000694ec4e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694ec4e)
#18 0x000000000694f46a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694f46a)
#19 0x000000000685656b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x685656b)
#20 0x000000000688c9cd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688c9cd)
#21 0x000000000684933e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684933e)
#22 0x0000000006849adf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6849adf)
#23 0x000000000685191a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x685191a)
#24 0x00000000068528b5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68528b5)
#25 0x0000000006852d70 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6852d70)
#26 0x00000000068332f3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68332f3)
#27 0x0000000004ab1438 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ab1438)
#28 0x0000000004dab495 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dab495)
#29 0x0000000004d2682e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2682e)
#30 0x0000000004ea0281 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea0281)
#31 0x0000000000db5abf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5abf)
#32 0x0000000000dac72a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x0000000004b1a3c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x00000000040b9804 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40b9804)
#35 0x0000000004b1a9df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000004adb5bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4adb5bd)
#37 0x0000000004adc64e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4adc64e)
#38 0x0000000004ae45e5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae45e5)
#39 0x0000000000db1f45 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb1f45)
#40 0x0000000000c63e84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63e84)
#41 0x00006ffd48e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00006ffd48e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000dac1d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac1d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-8.0.0:
https://godbolt.org/z/4f7vM4c11

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/8b4917Te5
```c
void foo(int **x) {
#pragma omp target map(iterator(i = 0 : 42), iterator(j = 0 : 42), to : x[i][j])
  ;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Parse/Parser.h:263: clang::SourceLocation clang::Parser::ConsumeToken(): Assertion `!isTokenSpecial() &amp;&amp; "Should consume special tokens with Consume*Token"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -fopenmp &lt;source&gt;
1.	&lt;source&gt;:2:54: current parser token '('
2.	&lt;source&gt;:1:19: parsing function body 'foo'
3.	&lt;source&gt;:1:19: in compound statement ('{}')
 #<!-- -->0 0x0000000004177998 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4177998)
 #<!-- -->1 0x0000000004174dc4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4174dc4)
 #<!-- -->2 0x00000000040b9368 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00006ffd48e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00006ffd48e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00006ffd48e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00006ffd48e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00006ffd48e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00006ffd48e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006911f3e clang::Parser::parseMapTypeModifiers(clang::SemaOpenMP::OpenMPVarListDataTy&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6911f3e)
#<!-- -->10 0x0000000006914d0d clang::Parser::ParseOpenMPVarList(llvm::omp::Directive, llvm::omp::Clause, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::SemaOpenMP::OpenMPVarListDataTy&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6914d0d)
#<!-- -->11 0x000000000691676d clang::Parser::ParseOpenMPVarListClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691676d)
#<!-- -->12 0x0000000006916c71 clang::Parser::ParseOpenMPClause(llvm::omp::Directive, llvm::omp::Clause, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6916c71)
#<!-- -->13 0x000000000691859b clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691859b)
#<!-- -->14 0x0000000006925d24 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6925d24)
#<!-- -->15 0x0000000006944a84 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6944a84)
#<!-- -->16 0x0000000006946739 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6946739)
#<!-- -->17 0x000000000694ec4e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694ec4e)
#<!-- -->18 0x000000000694f46a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x694f46a)
#<!-- -->19 0x000000000685656b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x685656b)
#<!-- -->20 0x000000000688c9cd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x688c9cd)
#<!-- -->21 0x000000000684933e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684933e)
#<!-- -->22 0x0000000006849adf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6849adf)
#<!-- -->23 0x000000000685191a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x685191a)
#<!-- -->24 0x00000000068528b5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68528b5)
#<!-- -->25 0x0000000006852d70 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6852d70)
#<!-- -->26 0x00000000068332f3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68332f3)
#<!-- -->27 0x0000000004ab1438 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ab1438)
#<!-- -->28 0x0000000004dab495 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dab495)
#<!-- -->29 0x0000000004d2682e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2682e)
#<!-- -->30 0x0000000004ea0281 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea0281)
#<!-- -->31 0x0000000000db5abf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb5abf)
#<!-- -->32 0x0000000000dac72a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x0000000004b1a3c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x00000000040b9804 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40b9804)
#<!-- -->35 0x0000000004b1a9df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x0000000004adb5bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4adb5bd)
#<!-- -->37 0x0000000004adc64e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4adc64e)
#<!-- -->38 0x0000000004ae45e5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae45e5)
#<!-- -->39 0x0000000000db1f45 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb1f45)
#<!-- -->40 0x0000000000c63e84 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc63e84)
#<!-- -->41 0x00006ffd48e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x00006ffd48e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000dac1d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac1d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - shafik

We probably should be doing more checking or maybe `ConsumeToken` is not the correct action:

https://github.com/llvm/llvm-project/blob/4957c473bc73a3a0bf1037adec770547c4a0aa02/clang/lib/Parse/ParseOpenMP.cpp#L4144-L4149

---

### Comment 4 - KrxGu

> We probably should be doing more checking or maybe `ConsumeToken` is not the correct action:
> 
> [llvm-project/clang/lib/Parse/ParseOpenMP.cpp](https://github.com/llvm/llvm-project/blob/4957c473bc73a3a0bf1037adec770547c4a0aa02/clang/lib/Parse/ParseOpenMP.cpp#L4144-L4149)
> 
> Lines 4144 to 4149 in [4957c47](/llvm/llvm-project/commit/4957c473bc73a3a0bf1037adec770547c4a0aa02)
> 
>  Diag(Tok, diag::err_omp_unknown_map_type_modifier) 
>      << (getLangOpts().OpenMP >= 51 ? (getLangOpts().OpenMP >= 52 ? 2 : 1) 
>                                     : 0) 
>      << getLangOpts().OpenMPExtensions 
>      << (getLangOpts().OpenMP >= 60 ? 1 : 0); 
>  ConsumeToken();

i too suspect the same on a broad overview i think instead of `ConsumeToken()` it should have been `ConsumeAnyToken()` . I am reproducing the error locally and will also create a test file and will create a PR soon

---

