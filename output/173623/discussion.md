# Clang Assertion `isLValue()' failed in Expr::ClassifyImpl when using imag on a non-complex type in inline asm

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173623

## Body

### Link for quick verification: https://godbolt.org/z/YdeosbG91
```c
void process_struct() {
    float r = 3.14f;
    __asm__("" : "=rm"(__imag__ r));
}
```
Go back to `clang 3.6 (assertions)`. Clang trunk and Clang trunk assertion have the different stack dump.
May relate to https://github.com/llvm/llvm-project/issues/62377.

## Stack dump
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprClassification.cpp:57: Cl clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const: Assertion `isLValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:4:36: current parser token ';'
2.	<source>:1:23: parsing function body 'process_struct'
3.	<source>:1:23: in compound statement ('{}')
 #0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000073f764e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000073f764e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000073f764e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000073f764e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000073f764e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000073f764e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007cd1ed6 clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cd1ed6)
#10 0x0000000007cd20ad clang::Expr::isModifiableLvalue(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cd20ad)
#11 0x0000000007397d68 clang::Sema::ActOnGCCAsmStmt(clang::SourceLocation, bool, bool, unsigned int, unsigned int, clang::IdentifierInfo**, llvm::MutableArrayRef<clang::Expr*>, llvm::MutableArrayRef<clang::Expr*>, clang::Expr*, llvm::MutableArrayRef<clang::Expr*>, unsigned int, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7397d68)
#12 0x0000000006b2e8b7 clang::Parser::ParseAsmStatement(bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b2e8b7)
#13 0x0000000006b1b857 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1b857)
#14 0x0000000006b1c536 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1c536)
#15 0x0000000006b24e7e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b24e7e)
#16 0x0000000006b256ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b256ca)
#17 0x0000000006a2b62b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b62b)
#18 0x0000000006a61aed clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a61aed)
#19 0x0000000006a1e78b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e78b)
#20 0x0000000006a1ef7f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ef7f)
#21 0x0000000006a26daa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a26daa)
#22 0x0000000006a27d45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27d45)
#23 0x0000000006a281f0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a281f0)
#24 0x0000000006a08d93 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08d93)
#25 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#26 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#27 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#28 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#29 0x0000000000de5b4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#30 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#31 0x0000000000ddc71d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#32 0x0000000004cc0739 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#34 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#36 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#37 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#38 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#39 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#40 0x000073f764e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x000073f764e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/YdeosbG91
```c
void process_struct() {
    float r = 3.14f;
    __asm__("" : "=rm"(__imag__ r));
}
```
Go back to `clang 3.6 (assertions)`. Clang trunk and Clang trunk assertion have the different stack dump.
May relate to https://github.com/llvm/llvm-project/issues/62377.

## Stack dump
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprClassification.cpp:57: Cl clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const: Assertion `isLValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:4:36: current parser token ';'
2.	&lt;source&gt;:1:23: parsing function body 'process_struct'
3.	&lt;source&gt;:1:23: in compound statement ('{}')
 #<!-- -->0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #<!-- -->1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #<!-- -->2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000073f764e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000073f764e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000073f764e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000073f764e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000073f764e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000073f764e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007cd1ed6 clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cd1ed6)
#<!-- -->10 0x0000000007cd20ad clang::Expr::isModifiableLvalue(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cd20ad)
#<!-- -->11 0x0000000007397d68 clang::Sema::ActOnGCCAsmStmt(clang::SourceLocation, bool, bool, unsigned int, unsigned int, clang::IdentifierInfo**, llvm::MutableArrayRef&lt;clang::Expr*&gt;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::Expr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, unsigned int, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7397d68)
#<!-- -->12 0x0000000006b2e8b7 clang::Parser::ParseAsmStatement(bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b2e8b7)
#<!-- -->13 0x0000000006b1b857 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1b857)
#<!-- -->14 0x0000000006b1c536 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1c536)
#<!-- -->15 0x0000000006b24e7e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b24e7e)
#<!-- -->16 0x0000000006b256ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b256ca)
#<!-- -->17 0x0000000006a2b62b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b62b)
#<!-- -->18 0x0000000006a61aed clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a61aed)
#<!-- -->19 0x0000000006a1e78b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e78b)
#<!-- -->20 0x0000000006a1ef7f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ef7f)
#<!-- -->21 0x0000000006a26daa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a26daa)
#<!-- -->22 0x0000000006a27d45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27d45)
#<!-- -->23 0x0000000006a281f0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a281f0)
#<!-- -->24 0x0000000006a08d93 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08d93)
#<!-- -->25 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#<!-- -->26 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#<!-- -->27 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#<!-- -->28 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#<!-- -->29 0x0000000000de5b4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#<!-- -->30 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->31 0x0000000000ddc71d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->32 0x0000000004cc0739 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#<!-- -->34 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#<!-- -->36 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#<!-- -->37 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#<!-- -->38 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#<!-- -->39 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#<!-- -->40 0x000073f764e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x000073f764e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/YdeosbG91
```c
void process_struct() {
    float r = 3.14f;
    __asm__("" : "=rm"(__imag__ r));
}
```
Go back to `clang 3.6 (assertions)`. Clang trunk and Clang trunk assertion have the different stack dump.
May relate to https://github.com/llvm/llvm-project/issues/62377.

## Stack dump
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprClassification.cpp:57: Cl clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const: Assertion `isLValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:4:36: current parser token ';'
2.	&lt;source&gt;:1:23: parsing function body 'process_struct'
3.	&lt;source&gt;:1:23: in compound statement ('{}')
 #<!-- -->0 0x00000000042ecab8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ecab8)
 #<!-- -->1 0x00000000042e9ee4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e9ee4)
 #<!-- -->2 0x000000000422ec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000073f764e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000073f764e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000073f764e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000073f764e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000073f764e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000073f764e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007cd1ed6 clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cd1ed6)
#<!-- -->10 0x0000000007cd20ad clang::Expr::isModifiableLvalue(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7cd20ad)
#<!-- -->11 0x0000000007397d68 clang::Sema::ActOnGCCAsmStmt(clang::SourceLocation, bool, bool, unsigned int, unsigned int, clang::IdentifierInfo**, llvm::MutableArrayRef&lt;clang::Expr*&gt;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::Expr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, unsigned int, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7397d68)
#<!-- -->12 0x0000000006b2e8b7 clang::Parser::ParseAsmStatement(bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b2e8b7)
#<!-- -->13 0x0000000006b1b857 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1b857)
#<!-- -->14 0x0000000006b1c536 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1c536)
#<!-- -->15 0x0000000006b24e7e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b24e7e)
#<!-- -->16 0x0000000006b256ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b256ca)
#<!-- -->17 0x0000000006a2b62b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2b62b)
#<!-- -->18 0x0000000006a61aed clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a61aed)
#<!-- -->19 0x0000000006a1e78b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e78b)
#<!-- -->20 0x0000000006a1ef7f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1ef7f)
#<!-- -->21 0x0000000006a26daa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a26daa)
#<!-- -->22 0x0000000006a27d45 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a27d45)
#<!-- -->23 0x0000000006a281f0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a281f0)
#<!-- -->24 0x0000000006a08d93 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a08d93)
#<!-- -->25 0x0000000004c4dde8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4dde8)
#<!-- -->26 0x0000000004f446e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f446e5)
#<!-- -->27 0x0000000004ec497e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec497e)
#<!-- -->28 0x000000000503ea6d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503ea6d)
#<!-- -->29 0x0000000000de5b4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b4c)
#<!-- -->30 0x0000000000ddc59a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->31 0x0000000000ddc71d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->32 0x0000000004cc0739 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x000000000422f124 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422f124)
#<!-- -->34 0x0000000004cc0d4f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x0000000004c81832 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c81832)
#<!-- -->36 0x0000000004c827de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c827de)
#<!-- -->37 0x0000000004c89c35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c89c35)
#<!-- -->38 0x0000000000de1f41 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f41)
#<!-- -->39 0x0000000000c8cf94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf94)
#<!-- -->40 0x000073f764e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x000073f764e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000ddc035 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc035)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

</details>


---

### Comment 3 - shafik

gcc just diagnoses, we should do the same.

@erichkeane  @AaronBallman I think `ActOnGCCAsmStmt` needs to check `isLvalue` before attempting to check `isModifiableLvalue` which seems to assume it is an lvalue but not sure. If so maybe this is a good first issue?

---

