# [AArch64][SVE] crashes when applying ternary operator to SVE and GNU vector types

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/170279
**Status:** Closed
**Labels:** backend:AArch64, clang:frontend, SVE, crash
**Closed Date:** 2025-12-05T10:16:34Z

## Body

Reproducer:
https://godbolt.org/z/Ynx33EEq6
```cpp
#include <arm_sve.h>

typedef uint8_t gnu_uint8_t __attribute__((vector_size(32)));

void foo(svuint8_t sve_u1, gnu_uint8_t gnu_u1, unsigned char uc) {
  gnu_u1 ? uc : sve_u1;
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -mcpu=grace <source>
1.	<source>:6:23: current parser token ';'
2.	<source>:5:66: parsing function body 'foo'
3.	<source>:5:66: in compound statement ('{}')
 #0 0x0000000003cdab88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cdab88)
 #1 0x0000000003cd855c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd855c)
 #2 0x0000000003c1e468 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079f371c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000675c225 clang::Sema::CheckVectorOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x675c225)
 #5 0x00000000067fce6d clang::Sema::CheckVectorConditionalTypes(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67fce6d)
 #6 0x000000000681ed4f clang::Sema::CXXCheckConditionalOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ExprValueKind&, clang::ExprObjectKind&, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x681ed4f)
 #7 0x000000000676661b clang::Sema::CheckConditionalOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::ExprValueKind&, clang::ExprObjectKind&, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676661b)
 #8 0x0000000006767b5a clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6767b5a)
 #9 0x000000000626951f clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626951f)
#10 0x000000000626cf59 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626cf59)
#11 0x00000000062fdc27 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fdc27)
#12 0x00000000062f4a19 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f4a19)
#13 0x00000000062f53f6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f53f6)
#14 0x00000000062fe8e9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fe8e9)
#15 0x00000000062ff38a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ff38a)
#16 0x00000000061e901b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e901b)
#17 0x000000000623592f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623592f)
#18 0x00000000061e245f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e245f)
#19 0x00000000061e2b97 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e2b97)
#20 0x00000000061eb56f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61eb56f)
#21 0x00000000061ecd77 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ecd77)
#22 0x00000000061c5c4a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c5c4a)
#23 0x0000000004599785 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4599785)
#24 0x000000000489ae2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x489ae2a)
#25 0x0000000004819d0b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4819d0b)
#26 0x000000000498f96b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498f96b)
#27 0x0000000000dcda95 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcda95)
#28 0x0000000000dc594b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000dc59ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x0000000004606ab9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003c1e883 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1e883)
#32 0x0000000004606cd9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x00000000045c9662 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c9662)
#34 0x00000000045ca541 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45ca541)
#35 0x00000000045d312c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d312c)
#36 0x0000000000dca419 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca419)
#37 0x0000000000c74c74 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74c74)
#38 0x000079f371c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x000079f371c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000dc53e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc53e5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/Ynx33EEq6
```cpp
#include &lt;arm_sve.h&gt;

typedef uint8_t gnu_uint8_t __attribute__((vector_size(32)));

void foo(svuint8_t sve_u1, gnu_uint8_t gnu_u1, unsigned char uc) {
  gnu_u1 ? uc : sve_u1;
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -mcpu=grace &lt;source&gt;
1.	&lt;source&gt;:6:23: current parser token ';'
2.	&lt;source&gt;:5:66: parsing function body 'foo'
3.	&lt;source&gt;:5:66: in compound statement ('{}')
 #<!-- -->0 0x0000000003cdab88 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cdab88)
 #<!-- -->1 0x0000000003cd855c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd855c)
 #<!-- -->2 0x0000000003c1e468 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079f371c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000000000675c225 clang::Sema::CheckVectorOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x675c225)
 #<!-- -->5 0x00000000067fce6d clang::Sema::CheckVectorConditionalTypes(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67fce6d)
 #<!-- -->6 0x000000000681ed4f clang::Sema::CXXCheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x681ed4f)
 #<!-- -->7 0x000000000676661b clang::Sema::CheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676661b)
 #<!-- -->8 0x0000000006767b5a clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6767b5a)
 #<!-- -->9 0x000000000626951f clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626951f)
#<!-- -->10 0x000000000626cf59 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626cf59)
#<!-- -->11 0x00000000062fdc27 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fdc27)
#<!-- -->12 0x00000000062f4a19 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f4a19)
#<!-- -->13 0x00000000062f53f6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f53f6)
#<!-- -->14 0x00000000062fe8e9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fe8e9)
#<!-- -->15 0x00000000062ff38a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ff38a)
#<!-- -->16 0x00000000061e901b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e901b)
#<!-- -->17 0x000000000623592f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623592f)
#<!-- -->18 0x00000000061e245f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e245f)
#<!-- -->19 0x00000000061e2b97 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e2b97)
#<!-- -->20 0x00000000061eb56f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61eb56f)
#<!-- -->21 0x00000000061ecd77 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ecd77)
#<!-- -->22 0x00000000061c5c4a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c5c4a)
#<!-- -->23 0x0000000004599785 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4599785)
#<!-- -->24 0x000000000489ae2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x489ae2a)
#<!-- -->25 0x0000000004819d0b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4819d0b)
#<!-- -->26 0x000000000498f96b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498f96b)
#<!-- -->27 0x0000000000dcda95 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcda95)
#<!-- -->28 0x0000000000dc594b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000dc59ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004606ab9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003c1e883 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1e883)
#<!-- -->32 0x0000000004606cd9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x00000000045c9662 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c9662)
#<!-- -->34 0x00000000045ca541 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45ca541)
#<!-- -->35 0x00000000045d312c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d312c)
#<!-- -->36 0x0000000000dca419 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca419)
#<!-- -->37 0x0000000000c74c74 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74c74)
#<!-- -->38 0x000079f371c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x000079f371c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000dc53e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc53e5)
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
https://godbolt.org/z/Ynx33EEq6
```cpp
#include &lt;arm_sve.h&gt;

typedef uint8_t gnu_uint8_t __attribute__((vector_size(32)));

void foo(svuint8_t sve_u1, gnu_uint8_t gnu_u1, unsigned char uc) {
  gnu_u1 ? uc : sve_u1;
}
```

Backtrace:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -fno-verbose-asm -S -target aarch64-linux-gnu --gcc-toolchain=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/arm64/gcc-15.2.0/aarch64-unknown-linux-gnu/aarch64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -mcpu=grace &lt;source&gt;
1.	&lt;source&gt;:6:23: current parser token ';'
2.	&lt;source&gt;:5:66: parsing function body 'foo'
3.	&lt;source&gt;:5:66: in compound statement ('{}')
 #<!-- -->0 0x0000000003cdab88 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cdab88)
 #<!-- -->1 0x0000000003cd855c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cd855c)
 #<!-- -->2 0x0000000003c1e468 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079f371c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000000000675c225 clang::Sema::CheckVectorOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x675c225)
 #<!-- -->5 0x00000000067fce6d clang::Sema::CheckVectorConditionalTypes(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67fce6d)
 #<!-- -->6 0x000000000681ed4f clang::Sema::CXXCheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x681ed4f)
 #<!-- -->7 0x000000000676661b clang::Sema::CheckConditionalOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ExprValueKind&amp;, clang::ExprObjectKind&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x676661b)
 #<!-- -->8 0x0000000006767b5a clang::Sema::ActOnConditionalOp(clang::SourceLocation, clang::SourceLocation, clang::Expr*, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6767b5a)
 #<!-- -->9 0x000000000626951f clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626951f)
#<!-- -->10 0x000000000626cf59 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626cf59)
#<!-- -->11 0x00000000062fdc27 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fdc27)
#<!-- -->12 0x00000000062f4a19 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f4a19)
#<!-- -->13 0x00000000062f53f6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f53f6)
#<!-- -->14 0x00000000062fe8e9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62fe8e9)
#<!-- -->15 0x00000000062ff38a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ff38a)
#<!-- -->16 0x00000000061e901b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e901b)
#<!-- -->17 0x000000000623592f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x623592f)
#<!-- -->18 0x00000000061e245f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e245f)
#<!-- -->19 0x00000000061e2b97 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e2b97)
#<!-- -->20 0x00000000061eb56f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61eb56f)
#<!-- -->21 0x00000000061ecd77 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ecd77)
#<!-- -->22 0x00000000061c5c4a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61c5c4a)
#<!-- -->23 0x0000000004599785 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4599785)
#<!-- -->24 0x000000000489ae2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x489ae2a)
#<!-- -->25 0x0000000004819d0b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4819d0b)
#<!-- -->26 0x000000000498f96b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x498f96b)
#<!-- -->27 0x0000000000dcda95 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdcda95)
#<!-- -->28 0x0000000000dc594b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000dc59ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004606ab9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003c1e883 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c1e883)
#<!-- -->32 0x0000000004606cd9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x00000000045c9662 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45c9662)
#<!-- -->34 0x00000000045ca541 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45ca541)
#<!-- -->35 0x00000000045d312c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45d312c)
#<!-- -->36 0x0000000000dca419 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca419)
#<!-- -->37 0x0000000000c74c74 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74c74)
#<!-- -->38 0x000079f371c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x000079f371c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000dc53e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc53e5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

