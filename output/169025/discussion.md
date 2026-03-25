# [AArch64][SVE] crashes when applying ternary operator to svbool_t and GNU vector type

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/169025
**Status:** Closed
**Labels:** backend:AArch64, clang:frontend, SVE, crash
**Closed Date:** 2025-11-26T10:24:57Z

## Body

Reproducer:
https://godbolt.org/z/6eoxdzzMW
```cpp
#include <arm_sve.h>

typedef uint8_t gnu_uint8_t __attribute__((vector_size(32)));

void foo(svbool_t sve_b1, gnu_uint8_t gnu_u1) { sve_b1 ? gnu_u1 : gnu_u1; }
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -mcpu=grace <source>
1.	<source>:5:73: current parser token ';'
2.	<source>:5:47: parsing function body 'foo'
3.	<source>:5:47: in compound statement ('{}')
 #0 0x0000000003c97ca8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c97ca8)
 #1 0x0000000003c9567c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9567c)
 #2 0x0000000003bdb588 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ab339e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006330541 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector<clang::CXXBaseSpecifier*, 4u> const*, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6330541)
 #5 0x00000000067ab42d clang::Sema::CheckSizelessVectorConditionalTypes(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67ab42d)
 #6 0x00000000067ce7d9 clang::Sema::CXXCheckConditionalOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ExprValueKind&, clang::ExprObjectKind&, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67ce7d9)
 #7 0x0000000006715c8b clang::Sema::CheckConditionalOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ExprValueKind&, clang::ExprObjectKind&, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6715c8b)
 #8 0x00000000067171ca clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67171ca)
 #9 0x000000000621a84f clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621a84f)
#10 0x000000000621e289 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621e289)
#11 0x00000000062aeeb7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62aeeb7)
#12 0x00000000062a5ca9 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a5ca9)
#13 0x00000000062a6686 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a6686)
#14 0x00000000062afb79 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62afb79)
#15 0x00000000062b061a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b061a)
#16 0x000000000619a34b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619a34b)
#17 0x00000000061e6c5f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e6c5f)
#18 0x000000000619378f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619378f)
#19 0x0000000006193ec7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6193ec7)
#20 0x000000000619c89f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619c89f)
#21 0x000000000619e0a7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619e0a7)
#22 0x0000000006176f7a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6176f7a)
#23 0x000000000455f625 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455f625)
#24 0x00000000048625aa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48625aa)
#25 0x00000000047e13bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47e13bb)
#26 0x00000000049585ab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49585ab)
#27 0x0000000000dcc1e5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcc1e5)
#28 0x0000000000dc409b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000dc413d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x00000000045cc819 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003bdb9a3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdb9a3)
#32 0x00000000045cca39 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x000000000458f472 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x458f472)
#34 0x0000000004590351 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4590351)
#35 0x00000000045990cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45990cc)
#36 0x0000000000dc8b69 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc8b69)
#37 0x0000000000c73b24 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc73b24)
#38 0x00007ab339e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007ab339e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000dc3b35 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc3b35)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/6eoxdzzMW
```cpp
#include &lt;arm_sve.h&gt;

typedef uint8_t gnu_uint8_t __attribute__((vector_size(32)));

void foo(svbool_t sve_b1, gnu_uint8_t gnu_u1) { sve_b1 ? gnu_u1 : gnu_u1; }
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -mcpu=grace &lt;source&gt;
1.	&lt;source&gt;:5:73: current parser token ';'
2.	&lt;source&gt;:5:47: parsing function body 'foo'
3.	&lt;source&gt;:5:47: in compound statement ('{}')
 #<!-- -->0 0x0000000003c97ca8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c97ca8)
 #<!-- -->1 0x0000000003c9567c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9567c)
 #<!-- -->2 0x0000000003bdb588 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ab339e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006330541 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt; const*, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6330541)
 #<!-- -->5 0x00000000067ab42d clang::Sema::CheckSizelessVectorConditionalTypes(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67ab42d)
 #<!-- -->6 0x00000000067ce7d9 clang::Sema::CXXCheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67ce7d9)
 #<!-- -->7 0x0000000006715c8b clang::Sema::CheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6715c8b)
 #<!-- -->8 0x00000000067171ca clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67171ca)
 #<!-- -->9 0x000000000621a84f clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621a84f)
#<!-- -->10 0x000000000621e289 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621e289)
#<!-- -->11 0x00000000062aeeb7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62aeeb7)
#<!-- -->12 0x00000000062a5ca9 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a5ca9)
#<!-- -->13 0x00000000062a6686 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a6686)
#<!-- -->14 0x00000000062afb79 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62afb79)
#<!-- -->15 0x00000000062b061a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b061a)
#<!-- -->16 0x000000000619a34b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619a34b)
#<!-- -->17 0x00000000061e6c5f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e6c5f)
#<!-- -->18 0x000000000619378f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619378f)
#<!-- -->19 0x0000000006193ec7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6193ec7)
#<!-- -->20 0x000000000619c89f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619c89f)
#<!-- -->21 0x000000000619e0a7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619e0a7)
#<!-- -->22 0x0000000006176f7a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6176f7a)
#<!-- -->23 0x000000000455f625 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455f625)
#<!-- -->24 0x00000000048625aa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48625aa)
#<!-- -->25 0x00000000047e13bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47e13bb)
#<!-- -->26 0x00000000049585ab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49585ab)
#<!-- -->27 0x0000000000dcc1e5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcc1e5)
#<!-- -->28 0x0000000000dc409b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000dc413d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x00000000045cc819 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003bdb9a3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdb9a3)
#<!-- -->32 0x00000000045cca39 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x000000000458f472 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x458f472)
#<!-- -->34 0x0000000004590351 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4590351)
#<!-- -->35 0x00000000045990cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45990cc)
#<!-- -->36 0x0000000000dc8b69 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc8b69)
#<!-- -->37 0x0000000000c73b24 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc73b24)
#<!-- -->38 0x00007ab339e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007ab339e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000dc3b35 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc3b35)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/6eoxdzzMW
```cpp
#include &lt;arm_sve.h&gt;

typedef uint8_t gnu_uint8_t __attribute__((vector_size(32)));

void foo(svbool_t sve_b1, gnu_uint8_t gnu_u1) { sve_b1 ? gnu_u1 : gnu_u1; }
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -mcpu=grace &lt;source&gt;
1.	&lt;source&gt;:5:73: current parser token ';'
2.	&lt;source&gt;:5:47: parsing function body 'foo'
3.	&lt;source&gt;:5:47: in compound statement ('{}')
 #<!-- -->0 0x0000000003c97ca8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c97ca8)
 #<!-- -->1 0x0000000003c9567c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9567c)
 #<!-- -->2 0x0000000003bdb588 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ab339e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006330541 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt; const*, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6330541)
 #<!-- -->5 0x00000000067ab42d clang::Sema::CheckSizelessVectorConditionalTypes(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67ab42d)
 #<!-- -->6 0x00000000067ce7d9 clang::Sema::CXXCheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67ce7d9)
 #<!-- -->7 0x0000000006715c8b clang::Sema::CheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6715c8b)
 #<!-- -->8 0x00000000067171ca clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67171ca)
 #<!-- -->9 0x000000000621a84f clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621a84f)
#<!-- -->10 0x000000000621e289 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621e289)
#<!-- -->11 0x00000000062aeeb7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62aeeb7)
#<!-- -->12 0x00000000062a5ca9 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a5ca9)
#<!-- -->13 0x00000000062a6686 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a6686)
#<!-- -->14 0x00000000062afb79 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62afb79)
#<!-- -->15 0x00000000062b061a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b061a)
#<!-- -->16 0x000000000619a34b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619a34b)
#<!-- -->17 0x00000000061e6c5f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e6c5f)
#<!-- -->18 0x000000000619378f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619378f)
#<!-- -->19 0x0000000006193ec7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6193ec7)
#<!-- -->20 0x000000000619c89f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619c89f)
#<!-- -->21 0x000000000619e0a7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619e0a7)
#<!-- -->22 0x0000000006176f7a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6176f7a)
#<!-- -->23 0x000000000455f625 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455f625)
#<!-- -->24 0x00000000048625aa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48625aa)
#<!-- -->25 0x00000000047e13bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47e13bb)
#<!-- -->26 0x00000000049585ab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49585ab)
#<!-- -->27 0x0000000000dcc1e5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcc1e5)
#<!-- -->28 0x0000000000dc409b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000dc413d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x00000000045cc819 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003bdb9a3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdb9a3)
#<!-- -->32 0x00000000045cca39 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x000000000458f472 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x458f472)
#<!-- -->34 0x0000000004590351 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4590351)
#<!-- -->35 0x00000000045990cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45990cc)
#<!-- -->36 0x0000000000dc8b69 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc8b69)
#<!-- -->37 0x0000000000c73b24 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc73b24)
#<!-- -->38 0x00007ab339e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007ab339e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000dc3b35 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc3b35)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 3 - k-arrows

Goes back to clang-15.0.0 (with `-march=armv8.2-a+sve`):
https://godbolt.org/z/7Ef8jdo6z

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/6eoxdzzMW
```cpp
#include &lt;arm_sve.h&gt;

typedef uint8_t gnu_uint8_t __attribute__((vector_size(32)));

void foo(svbool_t sve_b1, gnu_uint8_t gnu_u1) { sve_b1 ? gnu_u1 : gnu_u1; }
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -mcpu=grace &lt;source&gt;
1.	&lt;source&gt;:5:73: current parser token ';'
2.	&lt;source&gt;:5:47: parsing function body 'foo'
3.	&lt;source&gt;:5:47: in compound statement ('{}')
 #<!-- -->0 0x0000000003c97ca8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c97ca8)
 #<!-- -->1 0x0000000003c9567c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9567c)
 #<!-- -->2 0x0000000003bdb588 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ab339e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006330541 clang::Sema::ImpCastExprToType(clang::Expr*, clang::QualType, clang::CastKind, clang::ExprValueKind, llvm::SmallVector&lt;clang::CXXBaseSpecifier*, 4u&gt; const*, clang::CheckedConversionKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6330541)
 #<!-- -->5 0x00000000067ab42d clang::Sema::CheckSizelessVectorConditionalTypes(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67ab42d)
 #<!-- -->6 0x00000000067ce7d9 clang::Sema::CXXCheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67ce7d9)
 #<!-- -->7 0x0000000006715c8b clang::Sema::CheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6715c8b)
 #<!-- -->8 0x00000000067171ca clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67171ca)
 #<!-- -->9 0x000000000621a84f clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621a84f)
#<!-- -->10 0x000000000621e289 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x621e289)
#<!-- -->11 0x00000000062aeeb7 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62aeeb7)
#<!-- -->12 0x00000000062a5ca9 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a5ca9)
#<!-- -->13 0x00000000062a6686 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a6686)
#<!-- -->14 0x00000000062afb79 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62afb79)
#<!-- -->15 0x00000000062b061a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b061a)
#<!-- -->16 0x000000000619a34b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619a34b)
#<!-- -->17 0x00000000061e6c5f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e6c5f)
#<!-- -->18 0x000000000619378f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619378f)
#<!-- -->19 0x0000000006193ec7 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6193ec7)
#<!-- -->20 0x000000000619c89f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619c89f)
#<!-- -->21 0x000000000619e0a7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x619e0a7)
#<!-- -->22 0x0000000006176f7a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6176f7a)
#<!-- -->23 0x000000000455f625 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455f625)
#<!-- -->24 0x00000000048625aa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48625aa)
#<!-- -->25 0x00000000047e13bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47e13bb)
#<!-- -->26 0x00000000049585ab clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49585ab)
#<!-- -->27 0x0000000000dcc1e5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcc1e5)
#<!-- -->28 0x0000000000dc409b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000dc413d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x00000000045cc819 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003bdb9a3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdb9a3)
#<!-- -->32 0x00000000045cca39 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x000000000458f472 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x458f472)
#<!-- -->34 0x0000000004590351 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4590351)
#<!-- -->35 0x00000000045990cc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45990cc)
#<!-- -->36 0x0000000000dc8b69 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc8b69)
#<!-- -->37 0x0000000000c73b24 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc73b24)
#<!-- -->38 0x00007ab339e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007ab339e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000dc3b35 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc3b35)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

