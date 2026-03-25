# Clang crashes during host-only CUDA compilation w/ precompiled headers.

**Author:** kadircet
**URL:** https://github.com/llvm/llvm-project/issues/106394
**Status:** Closed
**Labels:** clang:frontend, crash-on-valid
**Closed Date:** 2025-02-03T18:27:12Z

## Body

```
$ cat clang/test/Frontend/a.cpp
// RUN: %clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -Xclang -emit-pch -std=gnu++20 %s > %t.pch
// RUN: %clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -include-pch %t.pch -std=gnu++20 %s
#ifndef PREAMBLE
#define PREAMBLE
void *operator new(decltype(sizeof(int)));
#else
void foo() { delete (int *)0; };
#endif
```

```
./build/bin/llvm-lit -s -v -a clang/test/Frontend/a.cpp
llvm-lit: /usr/local/google/home/kadircet/repos/llvm/llvm/utils/lit/lit/llvm/config.py:509: note: using clang: /usr/local/google/home/kadircet/repos/llvm/build/bin/clang
FAIL: Clang :: Frontend/a.cpp (1 of 1)
******************** TEST 'Clang :: Frontend/a.cpp' FAILED ********************
Exit Code: 1

Command Output (stderr):
--
RUN: at line 1: /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -Xclang -emit-pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp > /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/test/Frontend/Output/a.cpp.tmp.pch
+ /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -Xclang -emit-pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp
RUN: at line 2: /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -include-pch /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/test/Frontend/Output/a.cpp.tmp.pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp
+ /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -include-pch /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/test/Frontend/Output/a.cpp.tmp.pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp
clang: /usr/local/google/home/kadircet/repos/llvm/clang/lib/Sema/SemaExprCXX.cpp:3281: FunctionDecl *clang::Sema::FindUsualDeallocationFunction(SourceLocation, bool, bool, DeclarationName): Assertion `Result.FD && "operator delete missing from global scope?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -include-pch /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/test/Frontend/Output/a.cpp.tmp.pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp
1.      /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp:7:29: current parser token ';'
2.      /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp:7:12: parsing function body 'foo'
3.      /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp:7:12: in compound statement ('{}')
 #0 0x0000561342261908 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/Unix/Signals.inc:723:13
 #1 0x000056134225f4de llvm::sys::RunSignalHandlers() /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/Signals.cpp:106:18
 #2 0x00005613421c8db6 HandleCrash /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #3 0x00005613421c8db6 CrashRecoverySignalHandler(int) /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #4 0x00007fc6fc2591a0 (/lib/x86_64-linux-gnu/libc.so.6+0x3d1a0)
 #5 0x00007fc6fc2a70ec __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007fc6fc259102 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #7 0x00007fc6fc2424f2 abort ./stdlib/abort.c:81:7
 #8 0x00007fc6fc242415 _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #9 0x00007fc6fc251d32 (/lib/x86_64-linux-gnu/libc.so.6+0x35d32)
#10 0x0000561344f71789 (/usr/local/google/home/kadircet/repos/llvm/build/bin/clang+0xac71789)
#11 0x0000561344f731a2 clang::Sema::ActOnCXXDelete(clang::SourceLocation, bool, bool, clang::Expr*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Sema/SemaExprCXX.cpp:3817:24
#12 0x00005613448bf732 clang::Parser::ParseCXXDeleteExpression(bool, clang::SourceLocation) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExprCXX.cpp:0:18
#13 0x00005613448a45ab clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExpr.cpp:1787:9
#14 0x000056134489e38c ParseCastExpression /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExpr.cpp:0:20
#15 0x000056134489e38c clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExpr.cpp:184:20
#16 0x000056134489e289 clang::Parser::ParseExpression(clang::Parser::TypeCastState) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExpr.cpp:136:10
#17 0x0000561344937e91 isInvalid /usr/local/google/home/kadircet/repos/llvm/clang/include/clang/Sema/Ownership.h:199:41
#18 0x0000561344937e91 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:564:12
#19 0x0000561344935bbc clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:0:14
#20 0x00005613449354b4 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:124:20
#21 0x000056134493fb41 clang::Parser::ParseCompoundStatementBody(bool) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:1257:11
#22 0x0000561344940bf2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:2535:21
#23 0x000056134487d0e9 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:0:0
#24 0x00005613448f4dcf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseDecl.cpp:2438:18
#25 0x000056134487bd09 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:0:10
#26 0x000056134487b522 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:1266:12
#27 0x000056134487a309 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:0:14
#28 0x0000561344878336 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:758:10
#29 0x0000561344877a5d clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:615:23
#30 0x000056134487376a clang::ParseAST(clang::Sema&, bool, bool) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseAST.cpp:170:5
#31 0x0000561342f1b21f clang::FrontendAction::Execute() /usr/local/google/home/kadircet/repos/llvm/clang/lib/Frontend/FrontendAction.cpp:1082:10
#32 0x0000561342e857ed getPtr /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/Support/Error.h:279:42
#33 0x0000561342e857ed operator bool /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/Support/Error.h:242:16
#34 0x0000561342e857ed clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Frontend/CompilerInstance.cpp:1061:23
#35 0x00005613430035de clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:25
#36 0x000056133fadbc6b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/local/google/home/kadircet/repos/llvm/clang/tools/driver/cc1_main.cpp:285:15
#37 0x000056133fad834d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /usr/local/google/home/kadircet/repos/llvm/clang/tools/driver/driver.cpp:215:12
#38 0x0000561342cbb6f9 operator() /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Job.cpp:440:30
#39 0x0000561342cbb6f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/ADT/STLFunctionalExtras.h:45:12
#40 0x00005613421c8af6 operator() /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#41 0x00005613421c8af6 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/CrashRecoveryContext.cpp:426:3
#42 0x0000561342cbade2 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Job.cpp:440:7
#43 0x0000561342c74ab7 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Compilation.cpp:199:15
#44 0x0000561342c74ff7 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Compilation.cpp:253:13
#45 0x0000561342c96869 empty /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/ADT/SmallVector.h:94:46
#46 0x0000561342c96869 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Driver.cpp:1946:23
#47 0x000056133fad784d clang_main(int, char**, llvm::ToolContext const&) /usr/local/google/home/kadircet/repos/llvm/clang/tools/driver/driver.cpp:391:21
#48 0x000056133fae8977 main /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/tools/driver/clang-driver.cpp:17:10
#49 0x00007fc6fc243b8a __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#50 0x00007fc6fc243c45 call_init ./csu/../csu/libc-start.c:128:20
#51 0x00007fc6fc243c45 __libc_start_main ./csu/../csu/libc-start.c:347:5
#52 0x000056133fad5ca1 _start (/usr/local/google/home/kadircet/repos/llvm/build/bin/clang+0x57d5ca1)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: kadir çetinkaya (kadircet)

<details>
```
$ cat clang/test/Frontend/a.cpp
// RUN: %clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -Xclang -emit-pch -std=gnu++20 %s &gt; %t.pch
// RUN: %clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -include-pch %t.pch -std=gnu++20 %s
#ifndef PREAMBLE
#define PREAMBLE
void *operator new(decltype(sizeof(int)));
#else
void foo() { delete (int *)0; };
#endif
```

```
./build/bin/llvm-lit -s -v -a clang/test/Frontend/a.cpp
llvm-lit: /usr/local/google/home/kadircet/repos/llvm/llvm/utils/lit/lit/llvm/config.py:509: note: using clang: /usr/local/google/home/kadircet/repos/llvm/build/bin/clang
FAIL: Clang :: Frontend/a.cpp (1 of 1)
******************** TEST 'Clang :: Frontend/a.cpp' FAILED ********************
Exit Code: 1

Command Output (stderr):
--
RUN: at line 1: /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -Xclang -emit-pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp &gt; /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/test/Frontend/Output/a.cpp.tmp.pch
+ /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -Xclang -emit-pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp
RUN: at line 2: /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -include-pch /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/test/Frontend/Output/a.cpp.tmp.pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp
+ /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -include-pch /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/test/Frontend/Output/a.cpp.tmp.pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp
clang: /usr/local/google/home/kadircet/repos/llvm/clang/lib/Sema/SemaExprCXX.cpp:3281: FunctionDecl *clang::Sema::FindUsualDeallocationFunction(SourceLocation, bool, bool, DeclarationName): Assertion `Result.FD &amp;&amp; "operator delete missing from global scope?"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/local/google/home/kadircet/repos/llvm/build/bin/clang -xcuda -nocudainc --cuda-host-only -fsyntax-only -include-pch /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/test/Frontend/Output/a.cpp.tmp.pch -std=gnu++20 /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp
1.      /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp:7:29: current parser token ';'
2.      /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp:7:12: parsing function body 'foo'
3.      /usr/local/google/home/kadircet/repos/llvm/clang/test/Frontend/a.cpp:7:12: in compound statement ('{}')
 #<!-- -->0 0x0000561342261908 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x000056134225f4de llvm::sys::RunSignalHandlers() /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x00005613421c8db6 HandleCrash /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->3 0x00005613421c8db6 CrashRecoverySignalHandler(int) /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/CrashRecoveryContext.cpp:390:51
 #<!-- -->4 0x00007fc6fc2591a0 (/lib/x86_64-linux-gnu/libc.so.6+0x3d1a0)
 #<!-- -->5 0x00007fc6fc2a70ec __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fc6fc259102 gsignal ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->7 0x00007fc6fc2424f2 abort ./stdlib/abort.c:81:7
 #<!-- -->8 0x00007fc6fc242415 _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #<!-- -->9 0x00007fc6fc251d32 (/lib/x86_64-linux-gnu/libc.so.6+0x35d32)
#<!-- -->10 0x0000561344f71789 (/usr/local/google/home/kadircet/repos/llvm/build/bin/clang+0xac71789)
#<!-- -->11 0x0000561344f731a2 clang::Sema::ActOnCXXDelete(clang::SourceLocation, bool, bool, clang::Expr*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Sema/SemaExprCXX.cpp:3817:24
#<!-- -->12 0x00005613448bf732 clang::Parser::ParseCXXDeleteExpression(bool, clang::SourceLocation) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExprCXX.cpp:0:18
#<!-- -->13 0x00005613448a45ab clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExpr.cpp:1787:9
#<!-- -->14 0x000056134489e38c ParseCastExpression /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExpr.cpp:0:20
#<!-- -->15 0x000056134489e38c clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExpr.cpp:184:20
#<!-- -->16 0x000056134489e289 clang::Parser::ParseExpression(clang::Parser::TypeCastState) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseExpr.cpp:136:10
#<!-- -->17 0x0000561344937e91 isInvalid /usr/local/google/home/kadircet/repos/llvm/clang/include/clang/Sema/Ownership.h:199:41
#<!-- -->18 0x0000561344937e91 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:564:12
#<!-- -->19 0x0000561344935bbc clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:0:14
#<!-- -->20 0x00005613449354b4 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:124:20
#<!-- -->21 0x000056134493fb41 clang::Parser::ParseCompoundStatementBody(bool) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:1257:11
#<!-- -->22 0x0000561344940bf2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseStmt.cpp:2535:21
#<!-- -->23 0x000056134487d0e9 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:0:0
#<!-- -->24 0x00005613448f4dcf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseDecl.cpp:2438:18
#<!-- -->25 0x000056134487bd09 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:0:10
#<!-- -->26 0x000056134487b522 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:1266:12
#<!-- -->27 0x000056134487a309 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:0:14
#<!-- -->28 0x0000561344878336 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:758:10
#<!-- -->29 0x0000561344877a5d clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/Parser.cpp:615:23
#<!-- -->30 0x000056134487376a clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Parse/ParseAST.cpp:170:5
#<!-- -->31 0x0000561342f1b21f clang::FrontendAction::Execute() /usr/local/google/home/kadircet/repos/llvm/clang/lib/Frontend/FrontendAction.cpp:1082:10
#<!-- -->32 0x0000561342e857ed getPtr /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/Support/Error.h:279:42
#<!-- -->33 0x0000561342e857ed operator bool /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/Support/Error.h:242:16
#<!-- -->34 0x0000561342e857ed clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Frontend/CompilerInstance.cpp:1061:23
#<!-- -->35 0x00005613430035de clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/local/google/home/kadircet/repos/llvm/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:280:25
#<!-- -->36 0x000056133fadbc6b cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/local/google/home/kadircet/repos/llvm/clang/tools/driver/cc1_main.cpp:285:15
#<!-- -->37 0x000056133fad834d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /usr/local/google/home/kadircet/repos/llvm/clang/tools/driver/driver.cpp:215:12
#<!-- -->38 0x0000561342cbb6f9 operator() /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Job.cpp:440:30
#<!-- -->39 0x0000561342cbb6f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/ADT/STLFunctionalExtras.h:45:12
#<!-- -->40 0x00005613421c8af6 operator() /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->41 0x00005613421c8af6 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /usr/local/google/home/kadircet/repos/llvm/llvm/lib/Support/CrashRecoveryContext.cpp:426:3
#<!-- -->42 0x0000561342cbade2 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Job.cpp:440:7
#<!-- -->43 0x0000561342c74ab7 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Compilation.cpp:199:15
#<!-- -->44 0x0000561342c74ff7 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Compilation.cpp:253:13
#<!-- -->45 0x0000561342c96869 empty /usr/local/google/home/kadircet/repos/llvm/llvm/include/llvm/ADT/SmallVector.h:94:46
#<!-- -->46 0x0000561342c96869 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /usr/local/google/home/kadircet/repos/llvm/clang/lib/Driver/Driver.cpp:1946:23
#<!-- -->47 0x000056133fad784d clang_main(int, char**, llvm::ToolContext const&amp;) /usr/local/google/home/kadircet/repos/llvm/clang/tools/driver/driver.cpp:391:21
#<!-- -->48 0x000056133fae8977 main /usr/local/google/home/kadircet/repos/llvm/build/tools/clang/tools/driver/clang-driver.cpp:17:10
#<!-- -->49 0x00007fc6fc243b8a __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->50 0x00007fc6fc243c45 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->51 0x00007fc6fc243c45 __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->52 0x000056133fad5ca1 _start (/usr/local/google/home/kadircet/repos/llvm/build/bin/clang+0x57d5ca1)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```
</details>


---

### Comment 2 - cor3ntin

@ChuanqiXu9 

---

### Comment 3 - ChuanqiXu9

I don't have an insight here.. I don't have experiences with CUDA nor PCH.  There are a lot of `if (CUDA)` conditions in the code bases. And from the crash trace, it looks like the deallocation function gets lost in the process of serialization with CUDA?

---

### Comment 4 - kadircet

Unfortunately not much insights from my side either, i tried to chase this a little bit, but the amount of `if (CUDA)` parts in the sema logic makes it really hard to reason about the flow.

---

### Comment 5 - Artem-B

How did we end up with this crash to start with? What do we want to have in the end?

PCH is not useful for CUDA as it requires all compilations to be done with the same flags, and CUDA compilation uses different flags for host and device sub-compilations. We'd need a set of PCH files, one per sub-compilation, and the driver currently does not support this.

Is there a sensible way to disable/ignore/error-out on PCH options during CUDA compilation?

---

### Comment 6 - kadircet

The real-life workflow involving this crash is clangd. It builds a PCH for all the `#include`s at the top of the file (called preamble) and keeps reusing that as the user edits the code in the main file. PCH and the main file (as can be seen in this example) are built with exact same flags in this use case, and for host-only.

So erroring out when we're about to perform multiple compilation jobs sounds sensible to me, but making it not crash when we have a single compilation job in host-only mode, reusing a PCH built with exact some configuration would be preferred for clangd users. Right now, this is rendering clangd useless (as it keeps crash looping) for all such files.

---

### Comment 7 - Artem-B

> Right now, this is rendering clangd useless

I suspect it will be nearly useless even without the crash. Even if it manages to produce some results, I would not trust them.

> clang -xcuda -nocudainc 

The problem is -- CUDA compilation inherently depends on CUDA SDK and its headers. Compilation w/o them will give you all sorts of weird results as it will completely break function overloads between host/device/global functions. Yes, it may work OK on toy examples and subset of the host-only code, but I suspect most of the real CUDA code would be producing way too much noise. Overloads will look like conflicting redeclarations/redefinitions, some GPU-side functions will be missing, a lot of commonly used types unavailable, C++ templated code will be instantiated in a wrong way, etc.

That said, I don't mind fixing the crash, there's clearly something odd happening there, just not sure that it will be of much help with your end use case.


---

### Comment 8 - kadircet

> I suspect it will be nearly useless even without the crash. Even if it manages to produce some results, I would not trust them.

Well I am not a CUDA developer myself, so I can't really say much about this. All I know is, some people were actually putting effort back in the day to make sure it was "useful". That being said, I am not sure to which extend they succeeded (or we did a good job of not regressing that).

> The problem is -- CUDA compilation inherently depends on CUDA SDK and its headers. Compilation w/o them will give you all sorts of weird results as it will completely break function overloads between host/device/global functions.

Just to be clear, this is a minimized repro from an actual invocation we have internally that actually sets cuda libraries properly. The repro here is triggering the crash through the same stack trace, without any external dependencies.

---

### Comment 9 - Artem-B

> Just to be clear, this is a minimized repro from an actual invocation we have internally that actually sets cuda libraries properly. 
> The repro here is triggering the crash through the same stack trace, without any external dependencies.

Got it. I'll see what I can do to fix it.

---

### Comment 10 - Artem-B

This is odd. Even though PCH generation and the crashing test both compile only host code, the overloads for the delete all set implicit `__device__` attribute and that's what's causing the failure.

```
(rr) p FoundDelete.dump()
lookup results for operator delete:
FunctionDecl 0x55e63ef73ee8 prev 0x55e63ef73ca0 <<invalid sloc>> <invalid sloc> imported implicit operator delete 'void (void *) noexcept'
|-ParmVarDecl 0x55e63ef74018 <<invalid sloc>> <invalid sloc> imported implicit 'void *'
|-VisibilityAttr 0x55e63ef73f90 <<invalid sloc>> Implicit Default
`-CUDADeviceAttr 0x55e63ef73fb8 <<invalid sloc>> Implicit
FunctionDecl 0x55e63ef74798 prev 0x55e63ef74528 <<invalid sloc>> <invalid sloc> imported implicit operator delete 'void (void *, unsigned long) noexcept'
|-ParmVarDecl 0x55e63ef748c8 <<invalid sloc>> <invalid sloc> imported implicit 'void *'
|-ParmVarDecl 0x55e63ef74938 <<invalid sloc>> <invalid sloc> imported implicit 'unsigned long'
|-VisibilityAttr 0x55e63ef74840 <<invalid sloc>> Implicit Default
`-CUDADeviceAttr 0x55e63ef74868 <<invalid sloc>> Implicit
FunctionDecl 0x55e63ef74308 prev 0x55e63ef74090 <<invalid sloc>> <invalid sloc> imported implicit operator delete 'void (void *, std::align_val_t) noexcept'
|-ParmVarDecl 0x55e63ef74438 <<invalid sloc>> <invalid sloc> imported implicit 'void *'
|-ParmVarDecl 0x55e63ef744a8 <<invalid sloc>> <invalid sloc> imported implicit 'std::align_val_t'
|-VisibilityAttr 0x55e63ef743b0 <<invalid sloc>> Implicit Default
`-CUDADeviceAttr 0x55e63ef743d8 <<invalid sloc>> Implicit
FunctionDecl 0x55e63ef74ca8 prev 0x55e63ef749b8 <<invalid sloc>> <invalid sloc> imported implicit operator delete 'void (void *, unsigned long, std::align_val_t) noexcept'
|-ParmVarDecl 0x55e63ef74dd8 <<invalid sloc>> <invalid sloc> imported implicit 'void *'
|-ParmVarDecl 0x55e63ef74e48 <<invalid sloc>> <invalid sloc> imported implicit 'unsigned long'
|-ParmVarDecl 0x55e63ef74ef8 <<invalid sloc>> <invalid sloc> imported implicit 'std::align_val_t'
|-VisibilityAttr 0x55e63ef74d50 <<invalid sloc>> Implicit Default
`-CUDADeviceAttr 0x55e63ef74d78 <<invalid sloc>> Implicit

```

---

### Comment 11 - Artem-B

Looks like the AST in the compiler run that created the .pch file contained both `__host__` and `__device__` overloads for the `operator delete`:
```
|-FunctionDecl 0x56355bb741f0 <<invalid sloc>> <invalid sloc> implicit operator delete 'void (void *) noexcept'
| |-ParmVarDecl 0x56355bb742f8 <<invalid sloc>> <invalid sloc> implicit 'void *'
| |-VisibilityAttr 0x56355bb74298 <<invalid sloc>> Implicit Default
| `-CUDAHostAttr 0x56355bb74180 <<invalid sloc>> Implicit
|-FunctionDecl 0x56355bb74398 <<invalid sloc>> <invalid sloc> implicit operator delete 'void (void *) noexcept'
| |-ParmVarDecl 0x56355bb744a0 <<invalid sloc>> <invalid sloc> implicit 'void *'
| |-VisibilityAttr 0x56355bb74440 <<invalid sloc>> Implicit Default
| `-CUDADeviceAttr 0x56355bb74368 <<invalid sloc>> Implicit

```

Now the question is -- where did we lose the `__host__` variant -- when we were writing the PCH file, or when we were reading it in.
Does anybody know how to dump what gets written into the PCH?

---

