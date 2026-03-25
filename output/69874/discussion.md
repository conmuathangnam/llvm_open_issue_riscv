# clang: 18: Assertion `isLValue()' failed.

**Author:** wierton
**URL:** https://github.com/llvm/llvm-project/issues/69874
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-06-13T10:45:44Z

## Body

Compiler Explorer: https://gcc.godbolt.org/z/n87aPY7rP

When compile this program, clang crashes:
```
void f(void) {
  *a = (a_struct){0};
}
```

Interestingly, renaming `a_struct` in the program appears to prevent the crash. For instance, changing `a_struct` to another name, such as `T`, seems to eliminate the issue:
```
void f(void) {
  *a = (T){0};
}
```

The stack dump:
```
<source>:2:9: error: use of undeclared identifier 'a_struct'; did you mean 'struct'?
    2 |   *a = (a_struct){0};
      |         ^~~~~~~~
      |         struct
clang: /root/llvm-project/clang/lib/AST/ExprClassification.cpp:57: Cl clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const: Assertion `isLValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -S --gcc-toolchain=/opt/compiler-explorer/gcc-9.2.0 -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:2:18: current parser token '{'
2.	<source>:1:14: parsing function body 'f'
3.	<source>:1:14: in compound statement ('{}')
 #0 0x0000000003742c08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3742c08)
 #1 0x00000000037408cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37408cc)
 #2 0x0000000003689278 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f6722cd5420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f672279800b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f6722777859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f6722777729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f6722788fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000000006fc23f4 clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fc23f4)
 #9 0x0000000006fc25bd clang::Expr::isModifiableLvalue(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fc25bd)
#10 0x00000000063f50e9 CheckForModifiableLvalue(clang::Expr*, clang::SourceLocation, clang::Sema&) SemaExpr.cpp:0:0
#11 0x0000000006453557 clang::Sema::CheckAssignmentOperands(clang::Expr*, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::QualType, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6453557)
#12 0x0000000006454ba3 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6454ba3)
#13 0x0000000006456565 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6456565)
#14 0x00000000064566e3 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64566e3)
#15 0x0000000005f1b1fb clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f1b1fb)
#16 0x0000000005f1e759 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f1e759)
#17 0x0000000005f99397 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f99397)
#18 0x0000000005f8fa2d clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f8fa2d)
#19 0x0000000005f90868 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f90868)
#20 0x0000000005f917a9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f917a9)
#21 0x0000000005f930ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f930ca)
#22 0x0000000005ec0d71 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ec0d71)
#23 0x0000000005ee8288 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ee8288)
#24 0x0000000005eb51db clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb51db)
#25 0x0000000005eb590f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#26 0x0000000005ebd2b4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ebd2b4)
#27 0x0000000005ebdadd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ebdadd)
#28 0x0000000005ebdf20 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ebdf20)
#29 0x0000000005eb0b52 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb0b52)
#30 0x00000000049aa318 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49aa318)
#31 0x000000000420eee9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x420eee9)
#32 0x00000000041902de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41902de)
#33 0x00000000042edf3e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42edf3e)
#34 0x0000000000bf1286 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbf1286)
#35 0x0000000000be8b4a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x0000000003fec189 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000003689724 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3689724)
#38 0x0000000003fec77f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000003fb4625 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fb4625)
#40 0x0000000003fb508d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fb508d)
#41 0x0000000003fbcfb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fbcfb5)
#42 0x0000000000bee72c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbee72c)
#43 0x0000000000ae8071 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xae8071)
#44 0x00007f6722779083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#45 0x0000000000be862e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbe862e)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: wt.cc (wierton)

<details>
Compiler Explorer: https://gcc.godbolt.org/z/n87aPY7rP

When compile this program, clang crashes:
```
void f(void) {
  *a = (a_struct){0};
}
```

Interestingly, renaming `a_struct` in the program appears to prevent the crash. For instance, changing `a_struct` to another name, such as `T`, seems to eliminate the issue:
```
void f(void) {
  *a = (T){0};
}
```

The stack dump:
```
&lt;source&gt;:2:9: error: use of undeclared identifier 'a_struct'; did you mean 'struct'?
    2 |   *a = (a_struct){0};
      |         ^~~~~~~~
      |         struct
clang: /root/llvm-project/clang/lib/AST/ExprClassification.cpp:57: Cl clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const: Assertion `isLValue()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -S --gcc-toolchain=/opt/compiler-explorer/gcc-9.2.0 -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:2:18: current parser token '{'
2.	&lt;source&gt;:1:14: parsing function body 'f'
3.	&lt;source&gt;:1:14: in compound statement ('{}')
 #<!-- -->0 0x0000000003742c08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3742c08)
 #<!-- -->1 0x00000000037408cc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37408cc)
 #<!-- -->2 0x0000000003689278 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f6722cd5420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x00007f672279800b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #<!-- -->5 0x00007f6722777859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #<!-- -->6 0x00007f6722777729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #<!-- -->7 0x00007f6722788fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #<!-- -->8 0x0000000006fc23f4 clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fc23f4)
 #<!-- -->9 0x0000000006fc25bd clang::Expr::isModifiableLvalue(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fc25bd)
#<!-- -->10 0x00000000063f50e9 CheckForModifiableLvalue(clang::Expr*, clang::SourceLocation, clang::Sema&amp;) SemaExpr.cpp:0:0
#<!-- -->11 0x0000000006453557 clang::Sema::CheckAssignmentOperands(clang::Expr*, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::QualType, clang::BinaryOperatorKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6453557)
#<!-- -->12 0x0000000006454ba3 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6454ba3)
#<!-- -->13 0x0000000006456565 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6456565)
#<!-- -->14 0x00000000064566e3 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x64566e3)
#<!-- -->15 0x0000000005f1b1fb clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f1b1fb)
#<!-- -->16 0x0000000005f1e759 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f1e759)
#<!-- -->17 0x0000000005f99397 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f99397)
#<!-- -->18 0x0000000005f8fa2d clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f8fa2d)
#<!-- -->19 0x0000000005f90868 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f90868)
#<!-- -->20 0x0000000005f917a9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f917a9)
#<!-- -->21 0x0000000005f930ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5f930ca)
#<!-- -->22 0x0000000005ec0d71 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ec0d71)
#<!-- -->23 0x0000000005ee8288 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ee8288)
#<!-- -->24 0x0000000005eb51db clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb51db)
#<!-- -->25 0x0000000005eb590f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#<!-- -->26 0x0000000005ebd2b4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ebd2b4)
#<!-- -->27 0x0000000005ebdadd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ebdadd)
#<!-- -->28 0x0000000005ebdf20 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5ebdf20)
#<!-- -->29 0x0000000005eb0b52 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x5eb0b52)
#<!-- -->30 0x00000000049aa318 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49aa318)
#<!-- -->31 0x000000000420eee9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x420eee9)
#<!-- -->32 0x00000000041902de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41902de)
#<!-- -->33 0x00000000042edf3e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42edf3e)
#<!-- -->34 0x0000000000bf1286 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbf1286)
#<!-- -->35 0x0000000000be8b4a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000003fec189 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x0000000003689724 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3689724)
#<!-- -->38 0x0000000003fec77f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x0000000003fb4625 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fb4625)
#<!-- -->40 0x0000000003fb508d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fb508d)
#<!-- -->41 0x0000000003fbcfb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fbcfb5)
#<!-- -->42 0x0000000000bee72c clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbee72c)
#<!-- -->43 0x0000000000ae8071 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xae8071)
#<!-- -->44 0x00007f6722779083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->45 0x0000000000be862e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xbe862e)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```
</details>


---

### Comment 2 - shafik

Maybe duplicate: https://github.com/llvm/llvm-project/issues/28496

---

### Comment 3 - shafik

may also be related to: https://github.com/llvm/llvm-project/issues/69345

The failure path is very similar we are using `err_undeclared_var_use_suggest` and we are hitting delated type correction.

---

### Comment 4 - shafik

So we end up crashing here:

https://github.com/llvm/llvm-project/blob/e6b3a2753b173aa87bd9ac373a150106856e80e4/clang/lib/AST/ExprClassification.cpp#L56-L58

We have typo correction which gets correct and so the expression end up as a `UnresolvedLookupExpr`:

```console
UnresolvedLookupExpr 0x13e8b90e0 '<overloaded function type>' lvalue (no ADL) = 'a_struct' empty
```

Just a little before we call `ClassifyInternal(...)` which classifies these as lvalues and has a nice comment as follows: 

https://github.com/llvm/llvm-project/blob/e6b3a2753b173aa87bd9ac373a150106856e80e4/clang/lib/AST/ExprClassification.cpp#L134-L136

If I check the result of `expr getValueKind()` during debugging I get `VK_PRValue`. So perhaps we should modify `ClassifyInternal(...)` or perhaps modify the assert? 

CC @zygoloid 

PS see my comment here https://github.com/llvm/llvm-project/issues/62377#issuecomment-1523936922 where I found these asserts used to be a debugging tool and maybe were a mistake to uncomment the original #if block around them

---

### Comment 5 - shafik

Unfortunately modifying `ClassifyInternal(...)` to return `CL_PRValue` for `UnresolvedLookupExpr` breaks 36 tests for the opposite case in which we assert `isPRValue()`. So perhaps the `UnresolvedLookupExpr` is created incorrectly.

---

### Comment 6 - shafik

I was mistaken, I thought I was working on the RHS but actually I was working on the LHS and this has a `TypoExpr`:

```console
UnaryOperator 0x10180dcb0 '<dependent type>' contains-errors prefix '*' cannot overflow
`-ImplicitCastExpr 0x10180dc98 '<dependent type>' contains-errors <LValueToRValue>
  `-TypoExpr 0x10180dc80 '<dependent type>' contains-errors lvalue
```

So I am guessing we are not checking for errors correctly in `Parser::ParseRHSOfBinaryExpression()`.

---

### Comment 7 - shafik

In `Sema::CreateBuiltinUnaryOp(...)` when we make this call:

https://github.com/llvm/llvm-project/blob/6768a3d4318455dd61029ebbd3015b1748f1e603/clang/lib/Sema/SemaExpr.cpp#L16347-L16348

We don't set `VK` to `VK_LValue` b/c `Op->isTypeDependent()` b/c it contains an error.

Then when we create the `UnaryOperator` here:

https://github.com/llvm/llvm-project/blob/6768a3d4318455dd61029ebbd3015b1748f1e603/clang/lib/Sema/SemaExpr.cpp#L16519-L16521

It will be set to a PRValue. 

So the assumption in `ClassifyInternal(...)` is just not correct. 

I suppose a fix would be to add a check in `ClassifyInternal(...)` for the `case Expr::UnaryOperatorClass:` and check `isTypeDependent()` and then return `Cl::CL_PRValue` for that case. Verified this change does not break any tests but the diagnostics are awful. 

---

