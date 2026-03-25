# [HLSL][Matrix] Matrix splats requiring implicit casting of initializer scalar data type encounters an assertion error

**Author:** Icohedron
**URL:** https://github.com/llvm/llvm-project/issues/175853
**Status:** Closed
**Labels:** clang:frontend, crash, HLSL
**Closed Date:** 2026-02-23T20:10:11Z

## Body

Matrix splats where the initializer scalar is of a data type differing from that of the matrix element type causes an assertion error to occur.

Examples: https://hlsl.godbolt.org/z/oodaTzWEa

Each of the following function declarations encounter the same assertion error when compiled.
```hlsl
export int4x4 fn0(bool b) {
    return b;
}

export int4x4 fn1(float f) {
    return f;
}

export float4x4 fn2(int i) {
    return i;
}

export float4x4 fn3(bool b) {
    return b;
}
```

Assertion error:
```
clang: /root/llvm-project/llvm/tools/clang/lib/AST/Type.cpp:2366: clang::Type::ScalarTypeKind clang::Type::getScalarTypeKind() const: Assertion `isScalarType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -T lib_6_3 <source>
1.	<source>:2:13: current parser token ';'
2.	<source>:1:27: parsing function body 'fn0'
3.	<source>:1:27: in compound statement ('{}')
 #0 0x00000000042fea18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42fea18)
 #1 0x00000000042fbe54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42fbe54)
 #2 0x000000000423e748 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000704af0642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000704af06969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000704af0642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000704af06287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000704af062871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000704af0639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000818de3e clang::Type::getScalarTypeKind() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x818de3e)
#10 0x0000000006f624b0 clang::Sema::PrepareScalarCast(clang::ActionResult<clang::Expr*, true>&, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6f624b0)
#11 0x0000000006f62dfc clang::Sema::prepareMatrixSplat(clang::QualType, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6f62dfc)
#12 0x000000000708127c clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::StandardConversionSequence const&, clang::AssignmentAction, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x708127c)
#13 0x000000000708264a clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::ImplicitConversionSequence const&, clang::AssignmentAction, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x708264a)
#14 0x00000000071127de clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x71127de)
#15 0x0000000007115692 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7115692)
#16 0x000000000739cba4 clang::Sema::PerformMoveOrCopyInitialization(clang::InitializedEntity const&, clang::Sema::NamedReturnInfo const&, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x739cba4)
#17 0x00000000073b2bd4 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x73b2bd4)
#18 0x00000000073b37c7 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x73b37c7)
#19 0x0000000006b486aa clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b486aa)
#20 0x0000000006b4caed clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b4caed)
#21 0x0000000006b4da06 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b4da06)
#22 0x0000000006b5634e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b5634e)
#23 0x0000000006b56b9a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b56b9a)
#24 0x0000000006a5cfeb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a5cfeb)
#25 0x0000000006a9345d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a9345d)
#26 0x0000000006a5014b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a5014b)
#27 0x0000000006a5093f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a5093f)
#28 0x0000000006a5876a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a5876a)
#29 0x0000000006ab45ac clang::Parser::ParseExportDeclaration() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ab45ac)
#30 0x0000000006a58c46 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a58c46)
#31 0x0000000006a59705 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a59705)
#32 0x0000000006a3a78a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a3a78a)
#33 0x0000000004c66488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c66488)
#34 0x0000000004f834fd clang::HLSLFrontendAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f834fd)
#35 0x0000000004f60135 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f60135)
#36 0x0000000004edff0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4edff0e)
#37 0x000000000505a7cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x505a7cd)
#38 0x0000000000dce92e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdce92e)
#39 0x0000000000dc52fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#40 0x0000000000dc547d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#41 0x0000000004cd96e9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x000000000423ebe4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x423ebe4)
#43 0x0000000004cd9cff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x0000000004c9a692 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9a692)
#45 0x0000000004c9b63e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9b63e)
#46 0x0000000004ca2a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca2a95)
#47 0x0000000000dcaca1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdcaca1)
#48 0x0000000000c88434 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc88434)
#49 0x0000704af0629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x0000704af0629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000dc4d95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc4d95)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Deric C. (Icohedron)

<details>
Matrix splats where the initializer scalar is of a data type differing from that of the matrix element type causes an assertion error to occur.

Examples: https://hlsl.godbolt.org/z/oodaTzWEa

Each of the following function declarations encounter the same assertion error when compiled.
```hlsl
export int4x4 fn0(bool b) {
    return b;
}

export int4x4 fn1(float f) {
    return f;
}

export float4x4 fn2(int i) {
    return i;
}

export float4x4 fn3(bool b) {
    return b;
}
```

Assertion error:
```
clang: /root/llvm-project/llvm/tools/clang/lib/AST/Type.cpp:2366: clang::Type::ScalarTypeKind clang::Type::getScalarTypeKind() const: Assertion `isScalarType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang --driver-mode=dxc -Zi -Qembed_debug -Fc /app/output.s -fcolor-diagnostics -fno-crash-diagnostics -T lib_6_3 &lt;source&gt;
1.	&lt;source&gt;:2:13: current parser token ';'
2.	&lt;source&gt;:1:27: parsing function body 'fn0'
3.	&lt;source&gt;:1:27: in compound statement ('{}')
 #<!-- -->0 0x00000000042fea18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42fea18)
 #<!-- -->1 0x00000000042fbe54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x42fbe54)
 #<!-- -->2 0x000000000423e748 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000704af0642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000704af06969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000704af0642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000704af06287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000704af062871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000704af0639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000818de3e clang::Type::getScalarTypeKind() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x818de3e)
#<!-- -->10 0x0000000006f624b0 clang::Sema::PrepareScalarCast(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6f624b0)
#<!-- -->11 0x0000000006f62dfc clang::Sema::prepareMatrixSplat(clang::QualType, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6f62dfc)
#<!-- -->12 0x000000000708127c clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::StandardConversionSequence const&amp;, clang::AssignmentAction, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x708127c)
#<!-- -->13 0x000000000708264a clang::Sema::PerformImplicitConversion(clang::Expr*, clang::QualType, clang::ImplicitConversionSequence const&amp;, clang::AssignmentAction, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x708264a)
#<!-- -->14 0x00000000071127de clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x71127de)
#<!-- -->15 0x0000000007115692 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&amp;, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7115692)
#<!-- -->16 0x000000000739cba4 clang::Sema::PerformMoveOrCopyInitialization(clang::InitializedEntity const&amp;, clang::Sema::NamedReturnInfo const&amp;, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x739cba4)
#<!-- -->17 0x00000000073b2bd4 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x73b2bd4)
#<!-- -->18 0x00000000073b37c7 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x73b37c7)
#<!-- -->19 0x0000000006b486aa clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b486aa)
#<!-- -->20 0x0000000006b4caed clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b4caed)
#<!-- -->21 0x0000000006b4da06 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b4da06)
#<!-- -->22 0x0000000006b5634e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b5634e)
#<!-- -->23 0x0000000006b56b9a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b56b9a)
#<!-- -->24 0x0000000006a5cfeb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a5cfeb)
#<!-- -->25 0x0000000006a9345d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a9345d)
#<!-- -->26 0x0000000006a5014b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a5014b)
#<!-- -->27 0x0000000006a5093f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a5093f)
#<!-- -->28 0x0000000006a5876a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a5876a)
#<!-- -->29 0x0000000006ab45ac clang::Parser::ParseExportDeclaration() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ab45ac)
#<!-- -->30 0x0000000006a58c46 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a58c46)
#<!-- -->31 0x0000000006a59705 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a59705)
#<!-- -->32 0x0000000006a3a78a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a3a78a)
#<!-- -->33 0x0000000004c66488 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c66488)
#<!-- -->34 0x0000000004f834fd clang::HLSLFrontendAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f834fd)
#<!-- -->35 0x0000000004f60135 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f60135)
#<!-- -->36 0x0000000004edff0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4edff0e)
#<!-- -->37 0x000000000505a7cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x505a7cd)
#<!-- -->38 0x0000000000dce92e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdce92e)
#<!-- -->39 0x0000000000dc52fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->40 0x0000000000dc547d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->41 0x0000000004cd96e9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x000000000423ebe4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x423ebe4)
#<!-- -->43 0x0000000004cd9cff clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x0000000004c9a692 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9a692)
#<!-- -->45 0x0000000004c9b63e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9b63e)
#<!-- -->46 0x0000000004ca2a95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca2a95)
#<!-- -->47 0x0000000000dcaca1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdcaca1)
#<!-- -->48 0x0000000000c88434 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc88434)
#<!-- -->49 0x0000704af0629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->50 0x0000704af0629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->51 0x0000000000dc4d95 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc4d95)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

