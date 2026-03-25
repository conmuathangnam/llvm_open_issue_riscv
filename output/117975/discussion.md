# Clang-20 crash at clang::CallExpr::getUnusedResultAttr(clang::ASTContext const&) const

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/117975
**Status:** Closed
**Labels:** clang:frontend, regression, crash
**Closed Date:** 2025-01-06T12:25:41Z

## Body

### Description
I encountered a crash in Clang-20 when compiling the following code 
[clangcrash.zip](https://github.com/user-attachments/files/17944587/clangcrash.zip)


###  Command

The compilation command used was:
```sh
clang++  -fno-rtlib-add-rpath  -fno-ident -flto -O3 -Wall -Wextra -fno-strict-aliasing -fwrapv -g -fsanitize=address   clangcrash.c
```

### Error Message
```sh
In file included from core.c:11:
In file included from ./helpers.h:13:
./aArray.h:3255:34: error: use of undeclared identifier 'std'
 3255 |     char errLoc[], int8_t vec[], std::function<void(int8_t*)> f), {
      |                                  ^
./aArray.h:3262:35: error: use of undeclared identifier 'std'
 3262 |     char errLoc[], int16_t vec[], std::function<void(int16_t*)> f), {
      |                                   ^
./aArray.h:3269:35: error: use of undeclared identifier 'std'
 3269 |     char errLoc[], int32_t vec[], std::function<void(int32_t*)> f), {
      |                                   ^
./aArray.h:3276:35: error: use of undeclared identifier 'std'
 3276 |     char errLoc[], int64_t vec[], std::function<void(int64_t*)> f), {
      |                                   ^
./aArray.h:3291:46: error: use of undeclared identifier 'std'
 3291 |     char errLoc[], int8_t vec[], size_t pos, std::function<int(size_t)> f), {
      |                                              ^
./aArray.h:3305:47: error: use of undeclared identifier 'std'
 3305 |     char errLoc[], int16_t vec[], size_t pos, std::function<int(size_t)> f), {
      |                                               ^
./aArray.h:3319:47: error: use of undeclared identifier 'std'
 3319 |     char errLoc[], int32_t vec[], size_t pos, std::function<int(size_t)> f), {
      |                                               ^
./aArray.h:3333:47: error: use of undeclared identifier 'std'
 3333 |     char errLoc[], int64_t vec[], size_t pos, std::function<int(size_t)> f), {
      |                                               ^
./aArray.h:3356:34: error: use of undeclared identifier 'std'
 3356 |     char errLoc[], int8_t vec[], std::function<int(int8_t*)> f), {
      |                                  ^
./aArray.h:3366:35: error: use of undeclared identifier 'std'
 3366 |     char errLoc[], int16_t vec[], std::function<int(int16_t*)> f), {
      |                                   ^
./aArray.h:3376:35: error: use of undeclared identifier 'std'
 3376 |     char errLoc[], int32_t vec[], std::function<int(int32_t*)> f), {
      |                                   ^
./aArray.h:3386:35: error: use of undeclared identifier 'std'
 3386 |     char errLoc[], int64_t vec[], std::function<int(int64_t*)> f), {
      |                                   ^
./aArray.h:3733:5: error: use of undeclared identifier 'std'
 3733 |     std::function<int8_t(int8_t,int8_t)>f), {
      |     ^
./aArray.h:3741:5: error: use of undeclared identifier 'std'
 3741 |     std::function<int16_t(int16_t,int8_t)>f), {
      |     ^
./aArray.h:3749:5: error: use of undeclared identifier 'std'
 3749 |     std::function<int32_t(int32_t,int8_t)>f), {
      |     ^
./aArray.h:3757:5: error: use of undeclared identifier 'std'
 3757 |     std::function<int64_t(int64_t,int8_t)>f), {
      |     ^
./aArray.h:3765:5: error: use of undeclared identifier 'std'
 3765 |     std::function<int8_t(int8_t,int16_t)>f), {
      |     ^
./aArray.h:3773:5: error: use of undeclared identifier 'std'
 3773 |     std::function<int16_t(int16_t,int16_t)>f), {
      |     ^
./aArray.h:3781:5: error: use of undeclared identifier 'std'
 3781 |     std::function<int32_t(int32_t,int16_t)>f), {
      |     ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fno-rtlib-add-rpath -fno-ident -flto -O3 -Wall -Wextra -fno-strict-aliasing -fwrapv -g -fsanitize=address <source>
1.	core.c:154:60: current parser token '}'
2.	core.c:139:50: parsing function body 'id_print_name'
3.	core.c:139:50: in compound statement ('{}')
4.	core.c:144:17: in compound statement ('{}')
5.	core.c:154:10: in compound statement ('{}')
 #0 0x0000000003c15168 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c15168)
 #1 0x0000000003c12e6c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c12e6c)
 #2 0x0000000003b60558 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007cf1b7442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000764f77b clang::CallExpr::getUnusedResultAttr(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x764f77b)
 #5 0x0000000006e567be clang::Sema::DiagnoseUnusedExprResult(clang::Stmt const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e567be)
 #6 0x0000000006b3eecc clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3eecc)
 #7 0x0000000006e479ae clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e479ae)
 #8 0x0000000006649284 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6649284)
 #9 0x000000000663f39e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663f39e)
#10 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#11 0x00000000066411d5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66411d5)
#12 0x00000000066418c4 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66418c4)
#13 0x000000000663e367 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663e367)
#14 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#15 0x0000000006642b68 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6642b68)
#16 0x0000000006645e02 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6645e02)
#17 0x000000000663ebd8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663ebd8)
#18 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#19 0x00000000066411d5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66411d5)
#20 0x00000000066418c4 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66418c4)
#21 0x000000000663e367 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663e367)
#22 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#23 0x0000000006642b68 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6642b68)
#24 0x0000000006645cff clang::Parser::ParseIfStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6645cff)
#25 0x000000000663ebd8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663ebd8)
#26 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#27 0x00000000066411d5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66411d5)
#28 0x00000000066429da clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66429da)
#29 0x0000000006555fe3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6555fe3)
#30 0x000000000658a79d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658a79d)
#31 0x0000000006549d2e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6549d2e)
#32 0x000000000654a4ee clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x654a4ee)
#33 0x0000000006551c83 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6551c83)
#34 0x0000000006552b6d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552b6d)
#35 0x000000000654508a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x654508a)
#36 0x000000000459b908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x459b908)
#37 0x0000000004855fe9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4855fe9)
#38 0x00000000047d5efe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47d5efe)
#39 0x000000000494104e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x494104e)
#40 0x0000000000cae571 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcae571)
#41 0x0000000000ca60fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#42 0x00000000045deb19 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#43 0x0000000003b60a04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b60a04)
#44 0x00000000045df10f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#45 0x00000000045a53ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a53ed)
#46 0x00000000045a64dd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a64dd)
#47 0x00000000045ad895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45ad895)
#48 0x0000000000cab3a3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcab3a3)
#49 0x0000000000b7ee94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb7ee94)
#50 0x00007cf1b7429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#51 0x00007cf1b7429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#52 0x0000000000ca5bae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca5bae)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

The issue can also be reproduced on Compiler Explorer:https://godbolt.org/z/v4W5WcjKM
Please let me know if you need any more details.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
### Description
I encountered a crash in Clang-20 when compiling the following code 
[clangcrash.zip](https://github.com/user-attachments/files/17944587/clangcrash.zip)


###  Command

The compilation command used was:
```sh
clang++  -fno-rtlib-add-rpath  -fno-ident -flto -O3 -Wall -Wextra -fno-strict-aliasing -fwrapv -g -fsanitize=address   clangcrash.c
```

### Error Message
```sh
In file included from core.c:11:
In file included from ./helpers.h:13:
./aArray.h:3255:34: error: use of undeclared identifier 'std'
 3255 |     char errLoc[], int8_t vec[], std::function&lt;void(int8_t*)&gt; f), {
      |                                  ^
./aArray.h:3262:35: error: use of undeclared identifier 'std'
 3262 |     char errLoc[], int16_t vec[], std::function&lt;void(int16_t*)&gt; f), {
      |                                   ^
./aArray.h:3269:35: error: use of undeclared identifier 'std'
 3269 |     char errLoc[], int32_t vec[], std::function&lt;void(int32_t*)&gt; f), {
      |                                   ^
./aArray.h:3276:35: error: use of undeclared identifier 'std'
 3276 |     char errLoc[], int64_t vec[], std::function&lt;void(int64_t*)&gt; f), {
      |                                   ^
./aArray.h:3291:46: error: use of undeclared identifier 'std'
 3291 |     char errLoc[], int8_t vec[], size_t pos, std::function&lt;int(size_t)&gt; f), {
      |                                              ^
./aArray.h:3305:47: error: use of undeclared identifier 'std'
 3305 |     char errLoc[], int16_t vec[], size_t pos, std::function&lt;int(size_t)&gt; f), {
      |                                               ^
./aArray.h:3319:47: error: use of undeclared identifier 'std'
 3319 |     char errLoc[], int32_t vec[], size_t pos, std::function&lt;int(size_t)&gt; f), {
      |                                               ^
./aArray.h:3333:47: error: use of undeclared identifier 'std'
 3333 |     char errLoc[], int64_t vec[], size_t pos, std::function&lt;int(size_t)&gt; f), {
      |                                               ^
./aArray.h:3356:34: error: use of undeclared identifier 'std'
 3356 |     char errLoc[], int8_t vec[], std::function&lt;int(int8_t*)&gt; f), {
      |                                  ^
./aArray.h:3366:35: error: use of undeclared identifier 'std'
 3366 |     char errLoc[], int16_t vec[], std::function&lt;int(int16_t*)&gt; f), {
      |                                   ^
./aArray.h:3376:35: error: use of undeclared identifier 'std'
 3376 |     char errLoc[], int32_t vec[], std::function&lt;int(int32_t*)&gt; f), {
      |                                   ^
./aArray.h:3386:35: error: use of undeclared identifier 'std'
 3386 |     char errLoc[], int64_t vec[], std::function&lt;int(int64_t*)&gt; f), {
      |                                   ^
./aArray.h:3733:5: error: use of undeclared identifier 'std'
 3733 |     std::function&lt;int8_t(int8_t,int8_t)&gt;f), {
      |     ^
./aArray.h:3741:5: error: use of undeclared identifier 'std'
 3741 |     std::function&lt;int16_t(int16_t,int8_t)&gt;f), {
      |     ^
./aArray.h:3749:5: error: use of undeclared identifier 'std'
 3749 |     std::function&lt;int32_t(int32_t,int8_t)&gt;f), {
      |     ^
./aArray.h:3757:5: error: use of undeclared identifier 'std'
 3757 |     std::function&lt;int64_t(int64_t,int8_t)&gt;f), {
      |     ^
./aArray.h:3765:5: error: use of undeclared identifier 'std'
 3765 |     std::function&lt;int8_t(int8_t,int16_t)&gt;f), {
      |     ^
./aArray.h:3773:5: error: use of undeclared identifier 'std'
 3773 |     std::function&lt;int16_t(int16_t,int16_t)&gt;f), {
      |     ^
./aArray.h:3781:5: error: use of undeclared identifier 'std'
 3781 |     std::function&lt;int32_t(int32_t,int16_t)&gt;f), {
      |     ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fno-rtlib-add-rpath -fno-ident -flto -O3 -Wall -Wextra -fno-strict-aliasing -fwrapv -g -fsanitize=address &lt;source&gt;
1.	core.c:154:60: current parser token '}'
2.	core.c:139:50: parsing function body 'id_print_name'
3.	core.c:139:50: in compound statement ('{}')
4.	core.c:144:17: in compound statement ('{}')
5.	core.c:154:10: in compound statement ('{}')
 #<!-- -->0 0x0000000003c15168 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c15168)
 #<!-- -->1 0x0000000003c12e6c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c12e6c)
 #<!-- -->2 0x0000000003b60558 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007cf1b7442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000000000764f77b clang::CallExpr::getUnusedResultAttr(clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x764f77b)
 #<!-- -->5 0x0000000006e567be clang::Sema::DiagnoseUnusedExprResult(clang::Stmt const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e567be)
 #<!-- -->6 0x0000000006b3eecc clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3eecc)
 #<!-- -->7 0x0000000006e479ae clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e479ae)
 #<!-- -->8 0x0000000006649284 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6649284)
 #<!-- -->9 0x000000000663f39e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663f39e)
#<!-- -->10 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#<!-- -->11 0x00000000066411d5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66411d5)
#<!-- -->12 0x00000000066418c4 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66418c4)
#<!-- -->13 0x000000000663e367 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663e367)
#<!-- -->14 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#<!-- -->15 0x0000000006642b68 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6642b68)
#<!-- -->16 0x0000000006645e02 clang::Parser::ParseIfStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6645e02)
#<!-- -->17 0x000000000663ebd8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663ebd8)
#<!-- -->18 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#<!-- -->19 0x00000000066411d5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66411d5)
#<!-- -->20 0x00000000066418c4 clang::Parser::ParseCompoundStatement(bool, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66418c4)
#<!-- -->21 0x000000000663e367 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663e367)
#<!-- -->22 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#<!-- -->23 0x0000000006642b68 clang::Parser::ParseStatement(clang::SourceLocation*, clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6642b68)
#<!-- -->24 0x0000000006645cff clang::Parser::ParseIfStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6645cff)
#<!-- -->25 0x000000000663ebd8 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663ebd8)
#<!-- -->26 0x00000000066402f0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66402f0)
#<!-- -->27 0x00000000066411d5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66411d5)
#<!-- -->28 0x00000000066429da clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66429da)
#<!-- -->29 0x0000000006555fe3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6555fe3)
#<!-- -->30 0x000000000658a79d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658a79d)
#<!-- -->31 0x0000000006549d2e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6549d2e)
#<!-- -->32 0x000000000654a4ee clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x654a4ee)
#<!-- -->33 0x0000000006551c83 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6551c83)
#<!-- -->34 0x0000000006552b6d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552b6d)
#<!-- -->35 0x000000000654508a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x654508a)
#<!-- -->36 0x000000000459b908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x459b908)
#<!-- -->37 0x0000000004855fe9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4855fe9)
#<!-- -->38 0x00000000047d5efe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47d5efe)
#<!-- -->39 0x000000000494104e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x494104e)
#<!-- -->40 0x0000000000cae571 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcae571)
#<!-- -->41 0x0000000000ca60fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->42 0x00000000045deb19 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->43 0x0000000003b60a04 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b60a04)
#<!-- -->44 0x00000000045df10f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->45 0x00000000045a53ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a53ed)
#<!-- -->46 0x00000000045a64dd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a64dd)
#<!-- -->47 0x00000000045ad895 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45ad895)
#<!-- -->48 0x0000000000cab3a3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcab3a3)
#<!-- -->49 0x0000000000b7ee94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb7ee94)
#<!-- -->50 0x00007cf1b7429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->51 0x00007cf1b7429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->52 0x0000000000ca5bae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca5bae)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

The issue can also be reproduced on Compiler Explorer:https://godbolt.org/z/v4W5WcjKM
Please let me know if you need any more details.
</details>


---

### Comment 2 - MagentaTreehouse

Reduced:
```c++
int f() { return 0; }

void id_print_name() {
  (int)
    ((int(*)())f)();
}
```
Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:6:1: current parser token '}'
2.	<source>:3:22: parsing function body 'id_print_name'
3.	<source>:3:22: in compound statement ('{}')
 #0 0x0000000003c02b58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c02b58)
 #1 0x0000000003c0085c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c0085c)
 #2 0x0000000003b4df48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007eb3eb242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000076406db clang::CallExpr::getUnusedResultAttr(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76406db)
 #5 0x0000000006e46f2e clang::Sema::DiagnoseUnusedExprResult(clang::Stmt const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e46f2e)
 #6 0x0000000006b2f5ec clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b2f5ec)
 #7 0x0000000006e3811e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e3811e)
 #8 0x0000000006638524 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6638524)
 #9 0x000000000662e63e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e63e)
#10 0x000000000662f590 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662f590)
#11 0x0000000006630475 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6630475)
#12 0x0000000006631c7a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631c7a)
#13 0x0000000006545263 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6545263)
#14 0x0000000006579a3d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6579a3d)
#15 0x0000000006538fae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6538fae)
#16 0x000000000653976e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x653976e)
#17 0x0000000006540f03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6540f03)
#18 0x0000000006541ded clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6541ded)
#19 0x000000000653430a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x653430a)
#20 0x00000000045898b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45898b8)
#21 0x0000000004843f99 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4843f99)
#22 0x00000000047c3eae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c3eae)
#23 0x000000000492efae clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492efae)
#24 0x0000000000caec91 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcaec91)
#25 0x0000000000ca681a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x00000000045ccac9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x0000000003b4e3f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b4e3f4)
#28 0x00000000045cd0bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x000000000459339d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x459339d)
#30 0x000000000459448d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x459448d)
#31 0x000000000459b845 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x459b845)
#32 0x0000000000cabac3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcabac3)
#33 0x0000000000b7f544 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb7f544)
#34 0x00007eb3eb229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x00007eb3eb229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000ca62ce _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca62ce)
```

Note:

The crash does not happen:
- in clang 19.
- if either cast is removed.

See Compiler Explorer: https://godbolt.org/z/8xr7MKh3z


---

### Comment 3 - shafik

@Mick235711 git bisect points to this commit as the source of the regression: 51ad2901ca54a01a72db71622b116eb27ea6dcf2

---

