# [clang] Assertion `From->getType()->isIntegerType()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/164976

## Body

Reproducer:
https://godbolt.org/z/WTbnq6jdr
```cpp
#define CONV(TYPE, NAME)                                                       \
  _Complex int NAME##_to_CI(TYPE a) { return a; }

CONV(short _Fract, sf);
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExprCXX.cpp:5111: clang::ExprResult clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, const clang::StandardConversionSequence&, clang::AssignmentAction, clang::CheckedConversionKind): Assertion `From->getType()->isIntegerType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ffixed-point <source>
1.	<source>:4:1 <Spelling=<source>:2:47>: current parser token ';'
2.	<source>:4:1 <Spelling=<source>:2:37>: parsing function body 'sf_to_CI'
3.	<source>:4:1 <Spelling=<source>:2:37>: in compound statement ('{}')
 #0 0x0000000004214d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4214d18)
 #1 0x0000000004212144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4212144)
 #2 0x0000000004156348 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078bc42e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078bc42e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078bc42e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078bc42e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078bc42e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000078bc42e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006f42367 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f42367)
#10 0x0000000006f4279a clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::ImplicitConversionSequence const&, clang::AssignmentAction, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f4279a)
#11 0x0000000006fcf6f6 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fcf6f6)
#12 0x0000000006fd2342 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd2342)
#13 0x00000000072506b4 clang::Sema::PerformMoveOrCopyInitialization(clang::InitializedEntity const&, clang::Sema::NamedReturnInfo const&, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72506b4)
#14 0x0000000007268354 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7268354)
#15 0x0000000007268f28 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7268f28)
#16 0x0000000006a0e66a clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0e66a)
#17 0x0000000006a1299a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1299a)
#18 0x0000000006a139c9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a139c9)
#19 0x0000000006a1c156 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c156)
#20 0x0000000006a1c96a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c96a)
#21 0x000000000692343b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692343b)
#22 0x00000000069598ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69598ad)
#23 0x0000000006916bee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6916bee)
#24 0x000000000691738f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691738f)
#25 0x000000000691ec43 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691ec43)
#26 0x000000000691fb55 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691fb55)
#27 0x0000000006920000 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6920000)
#28 0x00000000069011a3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69011a3)
#29 0x0000000004b5d4e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5d4e8)
#30 0x0000000004e4a685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4a685)
#31 0x0000000004dcb73e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dcb73e)
#32 0x0000000004f41c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41c2d)
#33 0x0000000000dce380 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce380)
#34 0x0000000000dc4eea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000dc506d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x0000000004bc6a79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x00000000041567e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41567e4)
#38 0x0000000004bc708f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000004b87fd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b87fd2)
#40 0x0000000004b88f7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b88f7e)
#41 0x0000000004b90695 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b90695)
#42 0x0000000000dca8a9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca8a9)
#43 0x0000000000c79c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79c64)
#44 0x000078bc42e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x000078bc42e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000dc4985 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4985)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/WTbnq6jdr
```cpp
#define CONV(TYPE, NAME)                                                       \
  _Complex int NAME##_to_CI(TYPE a) { return a; }

CONV(short _Fract, sf);
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExprCXX.cpp:5111: clang::ExprResult clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, const clang::StandardConversionSequence&amp;, clang::AssignmentAction, clang::CheckedConversionKind): Assertion `From-&gt;getType()-&gt;isIntegerType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ffixed-point &lt;source&gt;
1.	&lt;source&gt;:4:1 &lt;Spelling=&lt;source&gt;:2:47&gt;: current parser token ';'
2.	&lt;source&gt;:4:1 &lt;Spelling=&lt;source&gt;:2:37&gt;: parsing function body 'sf_to_CI'
3.	&lt;source&gt;:4:1 &lt;Spelling=&lt;source&gt;:2:37&gt;: in compound statement ('{}')
 #<!-- -->0 0x0000000004214d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4214d18)
 #<!-- -->1 0x0000000004212144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4212144)
 #<!-- -->2 0x0000000004156348 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078bc42e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000078bc42e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000078bc42e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000078bc42e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000078bc42e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000078bc42e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f42367 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f42367)
#<!-- -->10 0x0000000006f4279a clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::ImplicitConversionSequence const&amp;, clang::AssignmentAction, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f4279a)
#<!-- -->11 0x0000000006fcf6f6 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fcf6f6)
#<!-- -->12 0x0000000006fd2342 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&amp;, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd2342)
#<!-- -->13 0x00000000072506b4 clang::Sema::PerformMoveOrCopyInitialization(clang::InitializedEntity const&amp;, clang::Sema::NamedReturnInfo const&amp;, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72506b4)
#<!-- -->14 0x0000000007268354 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7268354)
#<!-- -->15 0x0000000007268f28 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7268f28)
#<!-- -->16 0x0000000006a0e66a clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0e66a)
#<!-- -->17 0x0000000006a1299a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1299a)
#<!-- -->18 0x0000000006a139c9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a139c9)
#<!-- -->19 0x0000000006a1c156 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c156)
#<!-- -->20 0x0000000006a1c96a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c96a)
#<!-- -->21 0x000000000692343b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692343b)
#<!-- -->22 0x00000000069598ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69598ad)
#<!-- -->23 0x0000000006916bee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6916bee)
#<!-- -->24 0x000000000691738f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691738f)
#<!-- -->25 0x000000000691ec43 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691ec43)
#<!-- -->26 0x000000000691fb55 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691fb55)
#<!-- -->27 0x0000000006920000 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6920000)
#<!-- -->28 0x00000000069011a3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69011a3)
#<!-- -->29 0x0000000004b5d4e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5d4e8)
#<!-- -->30 0x0000000004e4a685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e4a685)
#<!-- -->31 0x0000000004dcb73e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dcb73e)
#<!-- -->32 0x0000000004f41c2d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41c2d)
#<!-- -->33 0x0000000000dce380 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdce380)
#<!-- -->34 0x0000000000dc4eea ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->35 0x0000000000dc506d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000004bc6a79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x00000000041567e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41567e4)
#<!-- -->38 0x0000000004bc708f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x0000000004b87fd2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b87fd2)
#<!-- -->40 0x0000000004b88f7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b88f7e)
#<!-- -->41 0x0000000004b90695 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b90695)
#<!-- -->42 0x0000000000dca8a9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdca8a9)
#<!-- -->43 0x0000000000c79c64 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79c64)
#<!-- -->44 0x000078bc42e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x000078bc42e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000dc4985 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4985)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Note: using `-ffixed-point`

---

### Comment 3 - k-arrows

Goes back to clang-18.1.0:
https://godbolt.org/z/ajhd4fzj5

---

