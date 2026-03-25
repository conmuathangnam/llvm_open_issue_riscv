# [clang][OpenMP] Crashes with OpenMP `parallel` and `lastprivate` inside template member function

**Author:** Rush10233
**URL:** https://github.com/llvm/llvm-project/issues/144302

## Body

Goes back to clang-3.9: https://godbolt.org/z/vad3Eddc3

Simplified test:

```c++
struct B
{
  int i;
  template<typename T>
void foo();
};

template<typename T>
void B::foo()
{
#pragma omp parallel
  {
  #pragma omp sections lastprivate (i)
    {
      i = 0;
    }
  }
}
```
$clang++ -fopenmp -fsyntax-only test.cpp

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<source>:17:3: current parser token '}'
2.	<source>:10:1: parsing function body 'B::foo'
3.	<source>:10:1: in compound statement ('{}')
4.	<source>:12:3: in compound statement ('{}')
 #0 0x0000000003c31b88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c31b88)
 #1 0x0000000003c2fccc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c2fccc)
 #2 0x0000000003b80358 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071a30ac42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000069ec7ec clang::SemaOpenMP::EndOpenMPDSABlock(clang::Stmt*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x69ec7ec)
 #5 0x00000000062d1691 clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62d1691)
 #6 0x00000000062df7c7 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62df7c7)
 #7 0x0000000006302bd4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6302bd4)
 #8 0x00000000063047ce clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63047ce)
 #9 0x000000000630d24d clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630d24d)
#10 0x000000000630dbc4 void llvm::function_ref<void ()>::callback_fn<clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()>(long) ParseStmt.cpp:0:0
#11 0x00000000079bc501 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x79bc501)
#12 0x00000000062fbe1e clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fbe1e)
#13 0x0000000006302b47 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6302b47)
#14 0x00000000063047ce clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63047ce)
#15 0x0000000006305e68 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6305e68)
#16 0x00000000062d1bc2 clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62d1bc2)
#17 0x00000000062df7c7 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62df7c7)
#18 0x0000000006302bd4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6302bd4)
#19 0x00000000063047ce clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63047ce)
#20 0x000000000630d24d clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630d24d)
#21 0x000000000630dc8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630dc8a)
#22 0x00000000061e7c06 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e7c06)
#23 0x000000000623424f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623424f)
#24 0x0000000006318870 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6318870)
#25 0x0000000006325dfa clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6325dfa)
#26 0x000000000632617f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x632617f)
#27 0x0000000006237197 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6237197)
#28 0x00000000061ea961 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ea961)
#29 0x00000000061ec188 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ec188)
#30 0x00000000061da59a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61da59a)
#31 0x000000000457ae7d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x457ae7d)
#32 0x000000000488ca8a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x488ca8a)
#33 0x00000000048082db clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48082db)
#34 0x0000000004981033 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4981033)
#35 0x0000000000d95745 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd95745)
#36 0x0000000000d8d58d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#37 0x00000000045ec9b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x0000000003b80773 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b80773)
#39 0x00000000045ecbd9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x00000000045af77d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45af77d)
#41 0x00000000045b0741 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b0741)
#42 0x00000000045bb06c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45bb06c)
#43 0x0000000000d91ed1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd91ed1)
#44 0x0000000000c4c4f4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4c4f4)
#45 0x000071a30ac29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#46 0x000071a30ac29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#47 0x0000000000d8d025 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd8d025)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Generated by fuzzer.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Rush10233)

<details>
Goes back to clang-3.9: https://godbolt.org/z/vad3Eddc3

Simplified test:

```c++
struct B
{
  int i;
  template&lt;typename T&gt;
void foo();
};

template&lt;typename T&gt;
void B::foo()
{
#pragma omp parallel
  {
  #pragma omp sections lastprivate (i)
    {
      i = 0;
    }
  }
}
```
$clang++ -fopenmp -fsyntax-only test.cpp

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp &lt;source&gt;
1.	&lt;source&gt;:17:3: current parser token '}'
2.	&lt;source&gt;:10:1: parsing function body 'B::foo'
3.	&lt;source&gt;:10:1: in compound statement ('{}')
4.	&lt;source&gt;:12:3: in compound statement ('{}')
 #<!-- -->0 0x0000000003c31b88 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c31b88)
 #<!-- -->1 0x0000000003c2fccc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c2fccc)
 #<!-- -->2 0x0000000003b80358 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000071a30ac42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000069ec7ec clang::SemaOpenMP::EndOpenMPDSABlock(clang::Stmt*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x69ec7ec)
 #<!-- -->5 0x00000000062d1691 clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62d1691)
 #<!-- -->6 0x00000000062df7c7 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62df7c7)
 #<!-- -->7 0x0000000006302bd4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6302bd4)
 #<!-- -->8 0x00000000063047ce clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63047ce)
 #<!-- -->9 0x000000000630d24d clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630d24d)
#<!-- -->10 0x000000000630dbc4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Parser::ParseCompoundStatement(bool, unsigned int)::'lambda'()&gt;(long) ParseStmt.cpp:0:0
#<!-- -->11 0x00000000079bc501 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x79bc501)
#<!-- -->12 0x00000000062fbe1e clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fbe1e)
#<!-- -->13 0x0000000006302b47 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6302b47)
#<!-- -->14 0x00000000063047ce clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63047ce)
#<!-- -->15 0x0000000006305e68 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6305e68)
#<!-- -->16 0x00000000062d1bc2 clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62d1bc2)
#<!-- -->17 0x00000000062df7c7 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62df7c7)
#<!-- -->18 0x0000000006302bd4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6302bd4)
#<!-- -->19 0x00000000063047ce clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x63047ce)
#<!-- -->20 0x000000000630d24d clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630d24d)
#<!-- -->21 0x000000000630dc8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x630dc8a)
#<!-- -->22 0x00000000061e7c06 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e7c06)
#<!-- -->23 0x000000000623424f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623424f)
#<!-- -->24 0x0000000006318870 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6318870)
#<!-- -->25 0x0000000006325dfa clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6325dfa)
#<!-- -->26 0x000000000632617f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x632617f)
#<!-- -->27 0x0000000006237197 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6237197)
#<!-- -->28 0x00000000061ea961 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ea961)
#<!-- -->29 0x00000000061ec188 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ec188)
#<!-- -->30 0x00000000061da59a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61da59a)
#<!-- -->31 0x000000000457ae7d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x457ae7d)
#<!-- -->32 0x000000000488ca8a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x488ca8a)
#<!-- -->33 0x00000000048082db clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48082db)
#<!-- -->34 0x0000000004981033 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4981033)
#<!-- -->35 0x0000000000d95745 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd95745)
#<!-- -->36 0x0000000000d8d58d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->37 0x00000000045ec9b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->38 0x0000000003b80773 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b80773)
#<!-- -->39 0x00000000045ecbd9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->40 0x00000000045af77d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45af77d)
#<!-- -->41 0x00000000045b0741 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b0741)
#<!-- -->42 0x00000000045bb06c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45bb06c)
#<!-- -->43 0x0000000000d91ed1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd91ed1)
#<!-- -->44 0x0000000000c4c4f4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4c4f4)
#<!-- -->45 0x000071a30ac29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->46 0x000071a30ac29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->47 0x0000000000d8d025 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd8d025)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Generated by fuzzer.
</details>


---

