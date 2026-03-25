# [clang] crashes when redeclaring a struct with [[gnu::unused]] attribute

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/153916

## Body

Reproducer:
https://godbolt.org/z/M8nT1v9PG
```c
struct [[gnu::unused]] foo { } x;

void bar()
{
  extern struct foo { } x;
}
```

Backtrace:
```console
clang: /root/llvm-project/llvm/tools/clang/lib/AST/ASTStructuralEquivalence.cpp:2576: clang::DiagnosticBuilder clang::StructuralEquivalenceContext::Diag2(clang::SourceLocation, unsigned int): Assertion `Complain && "Not allowed to complain"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c23 <source>
1.	<source>:5:26: current parser token ';'
2.	<source>:4:1: parsing function body 'bar'
3.	<source>:4:1: in compound statement ('{}')
 #0 0x00000000040213f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x40213f8)
 #1 0x000000000401e824 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x401e824)
 #2 0x0000000003f63058 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000073ffcbe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000073ffcbe969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000073ffcbe42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000073ffcbe287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000073ffcbe2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000073ffcbe39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000076acb10 CheckStructurallyEquivalentAttributes(clang::StructuralEquivalenceContext&, clang::Decl const*, clang::Decl const*, clang::Decl const*) ASTStructuralEquivalence.cpp:0:0
#10 0x00000000076bd890 IsStructurallyEquivalent(clang::StructuralEquivalenceContext&, clang::RecordDecl*, clang::RecordDecl*) ASTStructuralEquivalence.cpp:0:0
#11 0x00000000076bea0d clang::StructuralEquivalenceContext::CheckKindSpecificEquivalence(clang::Decl*, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76bea0d)
#12 0x00000000076bf553 clang::StructuralEquivalenceContext::Finish() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76bf553)
#13 0x00000000076bf8b8 clang::StructuralEquivalenceContext::IsEquivalent(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76bf8b8)
#14 0x0000000007576f2c clang::ASTContext::mergeTagDefinitions(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7576f2c)
#15 0x00000000075c7271 clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x75c7271)
#16 0x0000000006a7e052 clang::Sema::MergeVarDeclTypes(clang::VarDecl*, clang::VarDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a7e052)
#17 0x0000000006a97658 clang::Sema::MergeVarDecl(clang::VarDecl*, clang::LookupResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a97658)
#18 0x0000000006a98a8a clang::Sema::CheckVariableDeclaration(clang::VarDecl*, clang::LookupResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a98a8a)
#19 0x0000000006a9c154 clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, llvm::ArrayRef<clang::BindingDecl*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a9c154)
#20 0x0000000006abc7af clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6abc7af)
#21 0x0000000006abd0f0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6abd0f0)
#22 0x00000000067098e3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67098e3)
#23 0x0000000006718d19 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6718d19)
#24 0x00000000067219cf clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67219cf)
#25 0x0000000006721e9c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6721e9c)
#26 0x00000000067e3af2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67e3af2)
#27 0x00000000067e47c9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67e47c9)
#28 0x00000000067ec92b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67ec92b)
#29 0x00000000067ed13a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67ed13a)
#30 0x00000000066e334b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66e334b)
#31 0x000000000671971d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x671971d)
#32 0x00000000066d676e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66d676e)
#33 0x00000000066d6f0f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66d6f0f)
#34 0x00000000066de783 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66de783)
#35 0x00000000066df695 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66df695)
#36 0x00000000066d179a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66d179a)
#37 0x00000000049c32f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c32f8)
#38 0x0000000004cb7355 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cb7355)
#39 0x0000000004c335ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c335ae)
#40 0x0000000004da8971 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4da8971)
#41 0x0000000000daff4f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdaff4f)
#42 0x0000000000da6baa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#43 0x0000000004a2ad09 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#44 0x0000000003f634f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f634f4)
#45 0x0000000004a2b31f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#46 0x00000000049ed58d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49ed58d)
#47 0x00000000049ee61e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49ee61e)
#48 0x00000000049f60b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49f60b5)
#49 0x0000000000dac405 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdac405)
#50 0x0000000000c60394 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc60394)
#51 0x000073ffcbe29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#52 0x000073ffcbe29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#53 0x0000000000da6655 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda6655)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/M8nT1v9PG
```c
struct [[gnu::unused]] foo { } x;

void bar()
{
  extern struct foo { } x;
}
```

Backtrace:
```console
clang: /root/llvm-project/llvm/tools/clang/lib/AST/ASTStructuralEquivalence.cpp:2576: clang::DiagnosticBuilder clang::StructuralEquivalenceContext::Diag2(clang::SourceLocation, unsigned int): Assertion `Complain &amp;&amp; "Not allowed to complain"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c23 &lt;source&gt;
1.	&lt;source&gt;:5:26: current parser token ';'
2.	&lt;source&gt;:4:1: parsing function body 'bar'
3.	&lt;source&gt;:4:1: in compound statement ('{}')
 #<!-- -->0 0x00000000040213f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x40213f8)
 #<!-- -->1 0x000000000401e824 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x401e824)
 #<!-- -->2 0x0000000003f63058 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000073ffcbe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000073ffcbe969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000073ffcbe42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000073ffcbe287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000073ffcbe2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000073ffcbe39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000076acb10 CheckStructurallyEquivalentAttributes(clang::StructuralEquivalenceContext&amp;, clang::Decl const*, clang::Decl const*, clang::Decl const*) ASTStructuralEquivalence.cpp:0:0
#<!-- -->10 0x00000000076bd890 IsStructurallyEquivalent(clang::StructuralEquivalenceContext&amp;, clang::RecordDecl*, clang::RecordDecl*) ASTStructuralEquivalence.cpp:0:0
#<!-- -->11 0x00000000076bea0d clang::StructuralEquivalenceContext::CheckKindSpecificEquivalence(clang::Decl*, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76bea0d)
#<!-- -->12 0x00000000076bf553 clang::StructuralEquivalenceContext::Finish() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76bf553)
#<!-- -->13 0x00000000076bf8b8 clang::StructuralEquivalenceContext::IsEquivalent(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x76bf8b8)
#<!-- -->14 0x0000000007576f2c clang::ASTContext::mergeTagDefinitions(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7576f2c)
#<!-- -->15 0x00000000075c7271 clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x75c7271)
#<!-- -->16 0x0000000006a7e052 clang::Sema::MergeVarDeclTypes(clang::VarDecl*, clang::VarDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a7e052)
#<!-- -->17 0x0000000006a97658 clang::Sema::MergeVarDecl(clang::VarDecl*, clang::LookupResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a97658)
#<!-- -->18 0x0000000006a98a8a clang::Sema::CheckVariableDeclaration(clang::VarDecl*, clang::LookupResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a98a8a)
#<!-- -->19 0x0000000006a9c154 clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;, llvm::ArrayRef&lt;clang::BindingDecl*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a9c154)
#<!-- -->20 0x0000000006abc7af clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6abc7af)
#<!-- -->21 0x0000000006abd0f0 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6abd0f0)
#<!-- -->22 0x00000000067098e3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67098e3)
#<!-- -->23 0x0000000006718d19 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6718d19)
#<!-- -->24 0x00000000067219cf clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67219cf)
#<!-- -->25 0x0000000006721e9c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6721e9c)
#<!-- -->26 0x00000000067e3af2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67e3af2)
#<!-- -->27 0x00000000067e47c9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67e47c9)
#<!-- -->28 0x00000000067ec92b clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67ec92b)
#<!-- -->29 0x00000000067ed13a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67ed13a)
#<!-- -->30 0x00000000066e334b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66e334b)
#<!-- -->31 0x000000000671971d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x671971d)
#<!-- -->32 0x00000000066d676e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66d676e)
#<!-- -->33 0x00000000066d6f0f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66d6f0f)
#<!-- -->34 0x00000000066de783 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66de783)
#<!-- -->35 0x00000000066df695 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66df695)
#<!-- -->36 0x00000000066d179a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66d179a)
#<!-- -->37 0x00000000049c32f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49c32f8)
#<!-- -->38 0x0000000004cb7355 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4cb7355)
#<!-- -->39 0x0000000004c335ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c335ae)
#<!-- -->40 0x0000000004da8971 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4da8971)
#<!-- -->41 0x0000000000daff4f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdaff4f)
#<!-- -->42 0x0000000000da6baa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->43 0x0000000004a2ad09 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->44 0x0000000003f634f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f634f4)
#<!-- -->45 0x0000000004a2b31f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->46 0x00000000049ed58d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49ed58d)
#<!-- -->47 0x00000000049ee61e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49ee61e)
#<!-- -->48 0x00000000049f60b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49f60b5)
#<!-- -->49 0x0000000000dac405 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdac405)
#<!-- -->50 0x0000000000c60394 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc60394)
#<!-- -->51 0x000073ffcbe29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->52 0x000073ffcbe29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->53 0x0000000000da6655 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda6655)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

You can check how far back a C assertion goes by using C++ w/ `-x c`: https://godbolt.org/z/6hW3645rE

Looks like this is in trunk

---

### Comment 3 - AaronBallman

There are two issues, both related to `CheckStructurallyEquivalentAttributes()`.

1) https://github.com/llvm/llvm-project/blob/98e8f01d183177a4f54187c23183da50a7cf6daf/clang/lib/AST/ASTStructuralEquivalence.cpp#L473 -- we need to check for `Context.Complain` as part of that predicate so we only emit the diagnostic when doing AST merging.
2) This exposes a different issue; when AST merging, we inherit attributes in a way that causes crashes when emitting the diagnostics. e.g.,
```
// struct3.c
struct AttrTest2 { int i; } x18;
struct [[gnu::unused]] AttrTest1 { int i; } x17;

// struct4.c
struct [[gnu::unused]] AttrTest2 { int i; } x18;
struct AttrTest1 { int i; } x17;

// test2.c
// RUN: %clang_cc1 -emit-pch -std=c23 -o %t.1.ast %S/Inputs/struct3.c
// RUN: %clang_cc1 -emit-pch -std=c23 -o %t.2.ast %S/Inputs/struct4.c
// RUN: %clang_cc1 -ast-merge %t.1.ast -ast-merge %t.2.ast -fsyntax-only -std=c23 %s 2>&1 | FileCheck %s
```
emits:
```
# .---command stdout------------
# | In file included from F:\source\llvm-project\clang\test\ASTMerge\struct\test2.c:1:
# | F:\source\llvm-project\clang\test\ASTMerge\struct\Inputs\struct3.c:2:8: error: type 'struct AttrTest2' has an attribute which currently causes the types to be treated as though they are incompatible [-Wodr]
# |     2 | struct AttrTest2 { int i; } x18;
# |       |        ^
# | F:\source\llvm-project\clang\test\ASTMerge\struct\Inputs\struct4.c:2:10: note: attribute 'gnu::unused' here
# |     2 | struct [[gnu::unused]] AttrTest2 { int i; } x18;
# |       |          ^
# | In file included from F:\source\llvm-project\clang\test\ASTMerge\struct\test2.c:1:
# | F:\source\llvm-project\clang\test\ASTMerge\struct\Inputs\struct3.c:2:8: error: type 'struct AttrTest2' has an attribute which currently causes the types to be treated as though they are incompatible [-Wodr]
# |     2 | struct AttrTest2 { int i; } x18;
# |       |        ^
# | F:\source\llvm-project\clang\test\ASTMerge\struct\Inputs\struct4.c:2:10: note: attribute 'gnu::unused' here
# |     2 | struct [[gnu::unused]] AttrTest2 { int i; } x18;
# |       |          ^
# | F:\source\llvm-project\clang\test\ASTMerge\struct\Inputs\struct4.c:2:10: note: attribute 'gnu::unused' here
# |     2 | struct [[gnu::unused]] AttrTest2 { int i; } x18;
# |       |          ^
# | F:\source\llvm-project\clang\test\ASTMerge\struct\Inputs\struct3.c:3:24: error: type 'struct AttrTest1' has an attribute which currently causes the types to be treated as though they are incompatible [-Wodr]
# |     3 | struct [[gnu::unused]] AttrTest1 { int i; } x17;
# |       |                        ^
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
# | Stack dump:
# | 0.  Program arguments: f:\\source\\llvm-project\\llvm\\out\\build\\x64-debug\\bin\\clang.exe -cc1 -internal-isystem F:\\source\\llvm-project\\llvm\\out\\build\\x64-Debug\\lib\\clang\\22\\include -nostdsysteminc -ast-merge F:\\source\\llvm-project\\llvm\\out\\build\\x64-Debug\\tools\\clang\\test\\ASTMerge\\struct\\Output\\test2.c.tmp.1.ast -ast-merge F:\\source\\llvm-project\\llvm\\out\\build\\x64-Debug\\tools\\clang\\test\\ASTMerge\\struct\\Output\\test2.c.tmp.2.ast -fsyntax-only -std=c23 F:\\source\\llvm-project\\clang\\test\\ASTMerge\\struct\\test2.c
# | Exception Code: 0xC0000005
# |  #0 0x00007ff7d988ae2b llvm::StringMapEntryBase::getKeyLength(void) const F:\source\llvm-project\llvm\include\llvm\ADT\StringMapEntry.h:31:0
# |  #1 0x00007ff7da2d4d88 clang::IdentifierInfo::getLength(void) const F:\source\llvm-project\clang\include\clang\Basic\IdentifierTable.h:247:0
# |  #2 0x00007ff7da2d4da9 clang::IdentifierInfo::getName(void) const F:\source\llvm-project\clang\include\clang\Basic\IdentifierTable.h:251:0
# |  #3 0x00007ff7ddd2cc2c normalizeAttrScopeName F:\source\llvm-project\clang\lib\Basic\Attributes.cpp:121:0
# |  #4 0x00007ff7ddcfac77 clang::AttributeCommonInfo::calculateAttributeSpellingListIndex(void) const F:\source\llvm-project\clang\lib\Basic\Attributes.cpp:241:0
# |  #5 0x00007ff7dba0bccd clang::AttributeCommonInfo::getAttributeSpellingListIndex(void) const F:\source\llvm-project\clang\include\clang\Basic\AttributeCommonInfo.h:261:0
# |  #6 0x00007ff7ddc70d34 clang::UnusedAttr::getSpelling(void) const F:\source\llvm-project\llvm\out\build\x64-Debug\tools\clang\include\clang\AST\AttrImpl.inc:46697:0
# |  #7 0x00007ff7ddb9cd33 clang::Attr::getSpelling(void) const F:\source\llvm-project\llvm\out\build\x64-Debug\tools\clang\include\clang\AST\AttrImpl.inc:50400:0
# |  #8 0x00007ff7df7d7447 clang::FormatASTNodeDiagnosticArgument(enum clang::DiagnosticsEngine::ArgumentKind, __int64, class llvm::StringRef, class llvm::StringRef, class llvm::ArrayRef<struct std::pair<enum clang::DiagnosticsEngine::ArgumentKind, __int64>>, class llvm::SmallVectorImpl<char> &, void *, class llvm::ArrayRef<__int64>) F:\source\llvm-project\clang\lib\AST\ASTDiagnostic.cpp:507:0
# |  #9 0x00007ff7da2d2a24 clang::DiagnosticsEngine::ConvertArgToString(enum clang::DiagnosticsEngine::ArgumentKind, __int64, class llvm::StringRef, class llvm::StringRef, class llvm::ArrayRef<struct std::pair<enum clang::DiagnosticsEngine::ArgumentKind, __int64>>, class llvm::SmallVectorImpl<char> &, class llvm::ArrayRef<__int64>) const F:\source\llvm-project\clang\include\clang\Basic\Diagnostic.h:914:0
# | #10 0x00007ff7da2c90f2 clang::Diagnostic::FormatDiagnostic(char const *, char const *, class llvm::SmallVectorImpl<char> &) const F:\source\llvm-project\clang\lib\Basic\Diagnostic.cpp:1352:0
# | #11 0x00007ff7da2c7be9 clang::Diagnostic::FormatDiagnostic(class llvm::SmallVectorImpl<char> &) const F:\source\llvm-project\clang\lib\Basic\Diagnostic.cpp:1083:0
# | #12 0x00007ff7da61a98a clang::TextDiagnosticPrinter::HandleDiagnostic(enum clang::DiagnosticsEngine::Level, class clang::Diagnostic const &) F:\source\llvm-project\clang\lib\Frontend\TextDiagnosticPrinter.cpp:119:0
# | #13 0x00007ff7da2c73d7 clang::DiagnosticsEngine::Report(enum clang::DiagnosticsEngine::Level, class clang::Diagnostic const &) F:\source\llvm-project\clang\lib\Basic\Diagnostic.cpp:667:0
# | #14 0x00007ff7da2c7303 clang::DiagnosticsEngine::ProcessDiag(class clang::DiagnosticBuilder const &) F:\source\llvm-project\clang\lib\Basic\Diagnostic.cpp:747:0
# | #15 0x00007ff7da2c7591 clang::DiagnosticsEngine::EmitDiagnostic(class clang::DiagnosticBuilder const &, bool) F:\source\llvm-project\clang\lib\Basic\Diagnostic.cpp:769:0
# | #16 0x00007ff7d988bfc6 clang::DiagnosticBuilder::Emit(void) F:\source\llvm-project\clang\include\clang\Basic\Diagnostic.h:1290:0
# | #17 0x00007ff7d988bff4 clang::DiagnosticBuilder::~DiagnosticBuilder(void) F:\source\llvm-project\clang\include\clang\Basic\Diagnostic.h:1325:0
# | #18 0x00007ff7ddd92485 CheckStructurallyEquivalentAttributes F:\source\llvm-project\clang\lib\AST\ASTStructuralEquivalence.cpp:483:0
# | #19 0x00007ff7ddd94c1d IsStructurallyEquivalent F:\source\llvm-project\clang\lib\AST\ASTStructuralEquivalence.cpp:1817:0
# | #20 0x00007ff7ddd83941 clang::StructuralEquivalenceContext::CheckKindSpecificEquivalence(class clang::Decl *, class clang::Decl *) F:\source\llvm-project\llvm\out\build\x64-Debug\tools\clang\include\clang\AST\DeclNodes.inc:422:0
# | #21 0x00007ff7ddd831a9 clang::StructuralEquivalenceContext::Finish(void) F:\source\llvm-project\clang\lib\AST\ASTStructuralEquivalence.cpp:2749:0
# | #22 0x00007ff7ddd82a65 clang::StructuralEquivalenceContext::IsEquivalent(class clang::Decl *, class clang::Decl *) F:\source\llvm-project\clang\lib\AST\ASTStructuralEquivalence.cpp:2685:0
# | #23 0x00007ff7dfe571f0 clang::ASTNodeImporter::IsStructuralMatch(class clang::Decl *, class clang::Decl *, bool, bool) F:\source\llvm-project\clang\lib\AST\ASTImporter.cpp:2707:0
# | #24 0x00007ff7dfe5bbc1 clang::ASTNodeImporter::VisitRecordDecl(class clang::RecordDecl *) F:\source\llvm-project\clang\lib\AST\ASTImporter.cpp:3361:0
# | #25 0x00007ff7dfea3631 clang::declvisitor::Base<struct std::add_pointer, class clang::ASTNodeImporter, class llvm::Expected<class clang::Decl *>>::Visit(class clang::Decl *) F:\source\llvm-project\llvm\out\build\x64-Debug\tools\clang\include\clang\AST\DeclNodes.inc:422:0
# | #26 0x00007ff7dfe36aa7 clang::ASTImporter::ImportImpl(class clang::Decl *) F:\source\llvm-project\clang\lib\AST\ASTImporter.cpp:9381:0
# | #27 0x00007ff7dfe386d9 clang::ASTImporter::Import(class clang::Decl *) F:\source\llvm-project\clang\lib\AST\ASTImporter.cpp:9749:0
# | #28 0x00007ff7dc8417eb clang::ASTMergeAction::ExecuteAction(void) F:\source\llvm-project\clang\lib\Frontend\ASTMerge.cpp:67:0
# | #29 0x00007ff7dc331d04 clang::FrontendAction::Execute(void) F:\source\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1308:0
# | #30 0x00007ff7da4acf8e clang::CompilerInstance::ExecuteAction(class clang::FrontendAction &) F:\source\llvm-project\clang\lib\Frontend\CompilerInstance.cpp:1066:0
# | #31 0x00007ff7da633885 clang::ExecuteCompilerInvocation(class clang::CompilerInstance *) F:\source\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp:299:0
# | #32 0x00007ff7d9897baa cc1_main(class llvm::ArrayRef<char const *>, char const *, void *) F:\source\llvm-project\clang\tools\driver\cc1_main.cpp:297:0
# | #33 0x00007ff7d988663e ExecuteCC1Tool F:\source\llvm-project\clang\tools\driver\driver.cpp:223:0
# | #34 0x00007ff7d9886e55 clang_main(int, char **, struct llvm::ToolContext const &) F:\source\llvm-project\clang\tools\driver\driver.cpp:264:0
# | #35 0x00007ff7d98c3ae6 main F:\source\llvm-project\llvm\out\build\x64-Debug\tools\clang\tools\driver\clang-driver.cpp:17:0
# | #36 0x00007ff7e4167729 invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:79:0
# | #37 0x00007ff7e4167862 __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
# | #38 0x00007ff7e41678ee __scrt_common_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:331:0
# | #39 0x00007ff7e416790e mainCRTStartup D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp:17:0
# | #40 0x00007ffd9d6b259d (C:\WINDOWS\System32\KERNEL32.DLL+0x1259d)
# | #41 0x00007ffd9e16af78 (C:\WINDOWS\SYSTEM32\ntdll.dll+0x5af78)
# `-----------------------------
# error: command failed with exit status: 0xc0000005
# executed command: 'f:\source\llvm-project\llvm\out\build\x64-debug\bin\filecheck.exe' 'F:\source\llvm-project\clang\test\ASTMerge\struct\test2.c'
# .---command stderr------------
# | error: no check strings found with prefix 'CHECK:'
# `-----------------------------
# error: command failed with exit status: 2

--

********************
Testing:  0.. 10.. 20.. 30.. 40.. 50.. 60.. 70.. 80.. 90..
********************
Failed Tests (1):
  Clang :: ASTMerge/struct/test2.c
```


---

### Comment 4 - AaronBallman

I think we have a bigger issue with serialization in general. We have this hack to use the correct `ASTContext` rather than what you get from `setAttrs`:

https://github.com/llvm/llvm-project/blob/c67d27dad02ab7debfff6c7f7fc3ea8abf064b6a/clang/lib/Serialization/ASTReaderDecl.cpp#L623

but... `addAttr`, `getAttrs`, et al.... all have the same property: they use `Decl::getASTContext()`, same as `setAttrs()` does. It looks like we need to add a lot more `impl` interfaces to correctly handle attributes. CC @Bigcheese @ChuanqiXu9 for input

---

### Comment 5 - shafik

CC @balazske @steakhal 

---

### Comment 6 - shafik

We got a different version of this here but looks like the same root cause: https://github.com/llvm/llvm-project/issues/172821

CC @Bigcheese @ChuanqiXu9 

---

### Comment 7 - ChuanqiXu9

Sorry for missing it,

> but... addAttr, getAttrs, et al.... all have the same property: they use Decl::getASTContext(), same as setAttrs() does. It looks like we need to add a lot more impl interfaces to correctly handle attributes. CC @Bigcheese @ChuanqiXu9 for input

Yeah, I think so. The reason for why we have it is, during deserialization, the parent of a decl may not be ready and getASTContext may call `getParent()`. I think adding more impl interfaces may be an easy solution.

A final solution may be adding a safe `getASTContext` interface, but it may ask for more spaces.

And finally, I just tried replace the `setAttrsImpl` with `setAttrs` in ASTReaderDecl and run check-clang, but everything works...

maybe @zygoloid has some insights?

---

