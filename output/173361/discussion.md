# [clang]Clang assertion when generating implicit copy assignment with an ill-formed user-declared `operator=`

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/173361

## Body

The following code has been causing Clang to crash(assertion) since version 11.0.0 and still does in the current trunk (x86‑64).

```cpp
struct A { Err& operator=(const A&); };
struct B { A f; };

void test() { B b1, b2; b1=b2; }
```

Please check: https://godbolt.org/z/4sYdes9Ya

Clang (assertions build) crashes during semantic analysis of copy assignment for a class that contains a member whose class declares an ill-formed copy assignment operator. The compiler emits a diagnostic for the malformed `operator=`, then later aborts while determining/generating the implicit copy assignment for the containing class. This should be handled as a normal error path without an assertion failure.

**Output:**  
```
<source>:1:12: error: unknown type name 'Err'
    1 | struct A { Err& operator=(const A&); };
      |            ^
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/DeclCXX.h:917: void clang::CXXRecordDecl::setImplicitCopyAssignmentIsDeleted(): Assertion `(data().DefaultedCopyAssignmentIsDeleted || needsOverloadResolutionForCopyAssignment()) && "copy assignment should not be deleted"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:4:30: current parser token ';'
2.	<source>:4:13: parsing function body 'test'
3.	<source>:4:13: in compound statement ('{}')
 #0 0x00000000042eab68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eab68)
 #1 0x00000000042e7f94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e7f94)
 #2 0x000000000422cd78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000075414ec42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000075414ec969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000075414ec42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000075414ec287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000075414ec2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000075414ec39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006ec830d clang::Sema::DeclareImplicitCopyAssignment(clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ec830d)
#10 0x0000000007102c25 DeclareImplicitMemberFunctionsWithName(clang::Sema&, clang::DeclarationName, clang::SourceLocation, clang::DeclContext const*) SemaLookup.cpp:0:0
#11 0x00000000071202ce LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#12 0x00000000071205e1 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71205e1)
#13 0x000000000731d928 clang::Sema::AddMemberOperatorCandidates(clang::OverloadedOperatorKind, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x731d928)
#14 0x000000000732fb44 clang::Sema::LookupOverloadedBinOp(clang::OverloadCandidateSet&, clang::OverloadedOperatorKind, clang::UnresolvedSetImpl const&, llvm::ArrayRef<clang::Expr*>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x732fb44)
#15 0x000000000733a629 clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x733a629)
#16 0x0000000006f3dde2 BuildOverloadedBinOp(clang::Sema&, clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) SemaExpr.cpp:0:0
#17 0x0000000006f94307 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f94307)
#18 0x0000000006f94886 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f94886)
#19 0x0000000006a974d0 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a974d0)
#20 0x0000000006a9a969 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9a969)
#21 0x0000000006b20299 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b20299)
#22 0x0000000006b17744 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b17744)
#23 0x0000000006b18126 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b18126)
#24 0x0000000006b20a6e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b20a6e)
#25 0x0000000006b212ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b212ba)
#26 0x0000000006a271ab clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a271ab)
#27 0x0000000006a5d66d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5d66d)
#28 0x0000000006a1a30b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1a30b)
#29 0x0000000006a1aaff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1aaff)
#30 0x0000000006a2292a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2292a)
#31 0x0000000006a238c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a238c5)
#32 0x0000000006a0494a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0494a)
#33 0x0000000004c4b068 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4b068)
#34 0x0000000004f41965 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41965)
#35 0x0000000004ec1bfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec1bfe)
#36 0x000000000503bced clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503bced)
#37 0x0000000000de5afc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5afc)
#38 0x0000000000ddc54a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#39 0x0000000000ddc6cd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#40 0x0000000004cbd9b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x000000000422d214 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422d214)
#42 0x0000000004cbdfcf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x0000000004c7eab2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7eab2)
#44 0x0000000004c7fa5e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7fa5e)
#45 0x0000000004c86eb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c86eb5)
#46 0x0000000000de1ef1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1ef1)
#47 0x0000000000c8cf44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf44)
#48 0x000075414ec29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x000075414ec29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000ddbfe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddbfe5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following code has been causing Clang to crash(assertion) since version 11.0.0 and still does in the current trunk (x86‑64).

```cpp
struct A { Err&amp; operator=(const A&amp;); };
struct B { A f; };

void test() { B b1, b2; b1=b2; }
```

Please check: https://godbolt.org/z/4sYdes9Ya

Clang (assertions build) crashes during semantic analysis of copy assignment for a class that contains a member whose class declares an ill-formed copy assignment operator. The compiler emits a diagnostic for the malformed `operator=`, then later aborts while determining/generating the implicit copy assignment for the containing class. This should be handled as a normal error path without an assertion failure.

**Output:**  
```
&lt;source&gt;:1:12: error: unknown type name 'Err'
    1 | struct A { Err&amp; operator=(const A&amp;); };
      |            ^
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/DeclCXX.h:917: void clang::CXXRecordDecl::setImplicitCopyAssignmentIsDeleted(): Assertion `(data().DefaultedCopyAssignmentIsDeleted || needsOverloadResolutionForCopyAssignment()) &amp;&amp; "copy assignment should not be deleted"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:4:30: current parser token ';'
2.	&lt;source&gt;:4:13: parsing function body 'test'
3.	&lt;source&gt;:4:13: in compound statement ('{}')
 #<!-- -->0 0x00000000042eab68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eab68)
 #<!-- -->1 0x00000000042e7f94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e7f94)
 #<!-- -->2 0x000000000422cd78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000075414ec42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000075414ec969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000075414ec42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000075414ec287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000075414ec2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000075414ec39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006ec830d clang::Sema::DeclareImplicitCopyAssignment(clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ec830d)
#<!-- -->10 0x0000000007102c25 DeclareImplicitMemberFunctionsWithName(clang::Sema&amp;, clang::DeclarationName, clang::SourceLocation, clang::DeclContext const*) SemaLookup.cpp:0:0
#<!-- -->11 0x00000000071202ce LookupDirect(clang::Sema&amp;, clang::LookupResult&amp;, clang::DeclContext const*) SemaLookup.cpp:0:0
#<!-- -->12 0x00000000071205e1 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71205e1)
#<!-- -->13 0x000000000731d928 clang::Sema::AddMemberOperatorCandidates(clang::OverloadedOperatorKind, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x731d928)
#<!-- -->14 0x000000000732fb44 clang::Sema::LookupOverloadedBinOp(clang::OverloadCandidateSet&amp;, clang::OverloadedOperatorKind, clang::UnresolvedSetImpl const&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x732fb44)
#<!-- -->15 0x000000000733a629 clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&amp;, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x733a629)
#<!-- -->16 0x0000000006f3dde2 BuildOverloadedBinOp(clang::Sema&amp;, clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) SemaExpr.cpp:0:0
#<!-- -->17 0x0000000006f94307 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f94307)
#<!-- -->18 0x0000000006f94886 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f94886)
#<!-- -->19 0x0000000006a974d0 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a974d0)
#<!-- -->20 0x0000000006a9a969 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9a969)
#<!-- -->21 0x0000000006b20299 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b20299)
#<!-- -->22 0x0000000006b17744 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b17744)
#<!-- -->23 0x0000000006b18126 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b18126)
#<!-- -->24 0x0000000006b20a6e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b20a6e)
#<!-- -->25 0x0000000006b212ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b212ba)
#<!-- -->26 0x0000000006a271ab clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a271ab)
#<!-- -->27 0x0000000006a5d66d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5d66d)
#<!-- -->28 0x0000000006a1a30b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1a30b)
#<!-- -->29 0x0000000006a1aaff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1aaff)
#<!-- -->30 0x0000000006a2292a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2292a)
#<!-- -->31 0x0000000006a238c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a238c5)
#<!-- -->32 0x0000000006a0494a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0494a)
#<!-- -->33 0x0000000004c4b068 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4b068)
#<!-- -->34 0x0000000004f41965 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41965)
#<!-- -->35 0x0000000004ec1bfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec1bfe)
#<!-- -->36 0x000000000503bced clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503bced)
#<!-- -->37 0x0000000000de5afc cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5afc)
#<!-- -->38 0x0000000000ddc54a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->39 0x0000000000ddc6cd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->40 0x0000000004cbd9b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x000000000422d214 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422d214)
#<!-- -->42 0x0000000004cbdfcf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x0000000004c7eab2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7eab2)
#<!-- -->44 0x0000000004c7fa5e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7fa5e)
#<!-- -->45 0x0000000004c86eb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c86eb5)
#<!-- -->46 0x0000000000de1ef1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1ef1)
#<!-- -->47 0x0000000000c8cf44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf44)
#<!-- -->48 0x000075414ec29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x000075414ec29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000ddbfe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddbfe5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

