# Crash on code with StmtExpr and atomic char load in `Expr::EvaluateAsRValue`

**Author:** michael-jabbour-sonarsource
**URL:** https://github.com/llvm/llvm-project/issues/106576
**Status:** Closed
**Labels:** good first issue, clang:frontend, crash
**Closed Date:** 2025-02-18T08:52:33Z

## Body

The following example crashes `clang-16`, `clang-17` and `clang-18`:

```c
typedef _Atomic char atomic_char;

atomic_char counter;

char load_plus_one() {
  return ({counter;}) + 1;
}
```

See [on Compiler Explorer](https://godbolt.org/z/s6hxnY518).

The crash seems reproducible on release builds on Ubuntu 22 installed from apt.llvm.org, but I can't observe it on non-asserted builds on Compiler Explorer for some reason...

<details>
  <summary>Stack trace</summary>

```
$ clang-18 -fsyntax-only minimal.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang-18 -fsyntax-only minimal.c
1.	minimal.c:6:26: current parser token ';'
2.	minimal.c:5:22: parsing function body 'load_plus_one'
3.	minimal.c:5:22: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM-18.so.18.1   0x000079cec1794716 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 54
1  libLLVM-18.so.18.1   0x000079cec17926d0 llvm::sys::RunSignalHandlers() + 80
2  libLLVM-18.so.18.1   0x000079cec16e41f0
3  libc.so.6            0x000079cec0242520
4  libclang-cpp.so.18.1 0x000079cec90da059
5  libclang-cpp.so.18.1 0x000062b1379df1f8
clang-18: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang-18: note: diagnostic msg: 
```
I am able to get a better stack trace on clang-16:
```
$ clang-16 -fsyntax-only minimal.c
*** stack smashing detected ***: terminated
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang-16 -fsyntax-only minimal.c
1.	minimal.c:6:26: current parser token ';'
2.	minimal.c:5:22: parsing function body 'load_plus_one'
3.	minimal.c:5:22: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM-16.so.1    0x0000794646bbec16 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 54
1  libLLVM-16.so.1    0x0000794646bbcdb0 llvm::sys::RunSignalHandlers() + 80
2  libLLVM-16.so.1    0x0000794646afd600
3  libc.so.6          0x0000794645442520
4  libc.so.6          0x00007946454969fc pthread_kill + 300
5  libc.so.6          0x0000794645442476 raise + 22
6  libc.so.6          0x00007946454287f3 abort + 211
7  libc.so.6          0x0000794645489676
8  libc.so.6          0x000079464553659a __fortify_fail + 42
9  libc.so.6          0x0000794645536566
10 libclang-cpp.so.16 0x000079464e2f1cde
11 libclang-cpp.so.16 0x000079464e2daf1b
12 libclang-cpp.so.16 0x000079464e2f1307
13 libclang-cpp.so.16 0x000079464e2af018
14 libclang-cpp.so.16 0x000079464e2d67a2
15 libclang-cpp.so.16 0x000079464e2af018
16 libclang-cpp.so.16 0x000079464e2acaf0
17 libclang-cpp.so.16 0x000079464e2a8bca clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const + 602
18 libclang-cpp.so.16 0x000079464e7e5412
19 libclang-cpp.so.16 0x000079464e7e2255
20 libclang-cpp.so.16 0x000079464e7d6995
21 libclang-cpp.so.16 0x000079464e7d901f clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) + 207
22 libclang-cpp.so.16 0x000079464eb14604 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) + 324
23 libclang-cpp.so.16 0x000079464ecf0db5 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) + 2485
24 libclang-cpp.so.16 0x000079464ecf033e clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) + 94
25 libclang-cpp.so.16 0x000079464e0748b6 clang::Parser::ParseReturnStatement() + 326
26 libclang-cpp.so.16 0x000079464e06eee9 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) + 2649
27 libclang-cpp.so.16 0x000079464e06e25d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 349
28 libclang-cpp.so.16 0x000079464e0766f0 clang::Parser::ParseCompoundStatementBody(bool) + 1600
29 libclang-cpp.so.16 0x000079464e0777af clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) + 175
30 libclang-cpp.so.16 0x000079464e091910 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) + 3376
31 libclang-cpp.so.16 0x000079464dfe1832 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 4802
32 libclang-cpp.so.16 0x000079464e090a47 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 1031
33 libclang-cpp.so.16 0x000079464e090469 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 457
34 libclang-cpp.so.16 0x000079464e08f707 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 1943
35 libclang-cpp.so.16 0x000079464e08daae clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 1182
36 libclang-cpp.so.16 0x000079464dfcd75e clang::ParseAST(clang::Sema&, bool, bool) + 718
37 libclang-cpp.so.16 0x000079464fbe5565 clang::FrontendAction::Execute() + 85
38 libclang-cpp.so.16 0x000079464fb62754 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 724
39 libclang-cpp.so.16 0x000079464fc5dfe4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 660
40 clang-16           0x000057ac82bf7f4c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 2348
41 clang-16           0x000057ac82bf51f0
42 libclang-cpp.so.16 0x000079464f83e9a2
43 libLLVM-16.so.1    0x0000794646afd39c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 140
44 libclang-cpp.so.16 0x000079464f83e0c3 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 355
45 libclang-cpp.so.16 0x000079464f809922 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 866
46 libclang-cpp.so.16 0x000079464f809b7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 142
47 libclang-cpp.so.16 0x000079464f8242fd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 349
48 clang-16           0x000057ac82bf48b5 clang_main(int, char**) + 11541
49 libc.so.6          0x0000794645429d90
50 libc.so.6          0x0000794645429e40 __libc_start_main + 128
51 clang-16           0x000057ac82bf1855 _start + 37
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Ubuntu clang version 16.0.6 (++20231112100510+7cbf1a259152-1~exp1~20231112100554.106)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
```
</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Michael Jabbour (michael-jabbour-sonarsource)

<details>
The following example crashes `clang-16`, `clang-17` and `clang-18`:

```c
typedef _Atomic char atomic_char;

atomic_char counter;

char load_plus_one() {
  return ({counter;}) + 1;
}
```

See [on Compiler Explorer](https://godbolt.org/z/s6hxnY518).

The crash seems reproducible on release builds on Ubuntu 22 installed from apt.llvm.org, but I can't observe it on non-asserted builds on Compiler Explorer for some reason...

&lt;details&gt;
  &lt;summary&gt;Stack trace&lt;/summary&gt;

```
$ clang-18 -fsyntax-only minimal.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang-18 -fsyntax-only minimal.c
1.	minimal.c:6:26: current parser token ';'
2.	minimal.c:5:22: parsing function body 'load_plus_one'
3.	minimal.c:5:22: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM-18.so.18.1   0x000079cec1794716 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 54
1  libLLVM-18.so.18.1   0x000079cec17926d0 llvm::sys::RunSignalHandlers() + 80
2  libLLVM-18.so.18.1   0x000079cec16e41f0
3  libc.so.6            0x000079cec0242520
4  libclang-cpp.so.18.1 0x000079cec90da059
5  libclang-cpp.so.18.1 0x000062b1379df1f8
clang-18: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang-18: note: diagnostic msg: 
```
I am able to get a better stack trace on clang-16:
```
$ clang-16 -fsyntax-only minimal.c
*** stack smashing detected ***: terminated
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang-16 -fsyntax-only minimal.c
1.	minimal.c:6:26: current parser token ';'
2.	minimal.c:5:22: parsing function body 'load_plus_one'
3.	minimal.c:5:22: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM-16.so.1    0x0000794646bbec16 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 54
1  libLLVM-16.so.1    0x0000794646bbcdb0 llvm::sys::RunSignalHandlers() + 80
2  libLLVM-16.so.1    0x0000794646afd600
3  libc.so.6          0x0000794645442520
4  libc.so.6          0x00007946454969fc pthread_kill + 300
5  libc.so.6          0x0000794645442476 raise + 22
6  libc.so.6          0x00007946454287f3 abort + 211
7  libc.so.6          0x0000794645489676
8  libc.so.6          0x000079464553659a __fortify_fail + 42
9  libc.so.6          0x0000794645536566
10 libclang-cpp.so.16 0x000079464e2f1cde
11 libclang-cpp.so.16 0x000079464e2daf1b
12 libclang-cpp.so.16 0x000079464e2f1307
13 libclang-cpp.so.16 0x000079464e2af018
14 libclang-cpp.so.16 0x000079464e2d67a2
15 libclang-cpp.so.16 0x000079464e2af018
16 libclang-cpp.so.16 0x000079464e2acaf0
17 libclang-cpp.so.16 0x000079464e2a8bca clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const + 602
18 libclang-cpp.so.16 0x000079464e7e5412
19 libclang-cpp.so.16 0x000079464e7e2255
20 libclang-cpp.so.16 0x000079464e7d6995
21 libclang-cpp.so.16 0x000079464e7d901f clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) + 207
22 libclang-cpp.so.16 0x000079464eb14604 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) + 324
23 libclang-cpp.so.16 0x000079464ecf0db5 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) + 2485
24 libclang-cpp.so.16 0x000079464ecf033e clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) + 94
25 libclang-cpp.so.16 0x000079464e0748b6 clang::Parser::ParseReturnStatement() + 326
26 libclang-cpp.so.16 0x000079464e06eee9 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) + 2649
27 libclang-cpp.so.16 0x000079464e06e25d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 349
28 libclang-cpp.so.16 0x000079464e0766f0 clang::Parser::ParseCompoundStatementBody(bool) + 1600
29 libclang-cpp.so.16 0x000079464e0777af clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) + 175
30 libclang-cpp.so.16 0x000079464e091910 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) + 3376
31 libclang-cpp.so.16 0x000079464dfe1832 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 4802
32 libclang-cpp.so.16 0x000079464e090a47 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 1031
33 libclang-cpp.so.16 0x000079464e090469 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 457
34 libclang-cpp.so.16 0x000079464e08f707 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 1943
35 libclang-cpp.so.16 0x000079464e08daae clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1182
36 libclang-cpp.so.16 0x000079464dfcd75e clang::ParseAST(clang::Sema&amp;, bool, bool) + 718
37 libclang-cpp.so.16 0x000079464fbe5565 clang::FrontendAction::Execute() + 85
38 libclang-cpp.so.16 0x000079464fb62754 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 724
39 libclang-cpp.so.16 0x000079464fc5dfe4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 660
40 clang-16           0x000057ac82bf7f4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 2348
41 clang-16           0x000057ac82bf51f0
42 libclang-cpp.so.16 0x000079464f83e9a2
43 libLLVM-16.so.1    0x0000794646afd39c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 140
44 libclang-cpp.so.16 0x000079464f83e0c3 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 355
45 libclang-cpp.so.16 0x000079464f809922 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 866
46 libclang-cpp.so.16 0x000079464f809b7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 142
47 libclang-cpp.so.16 0x000079464f8242fd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 349
48 clang-16           0x000057ac82bf48b5 clang_main(int, char**) + 11541
49 libc.so.6          0x0000794645429d90
50 libc.so.6          0x0000794645429e40 __libc_start_main + 128
51 clang-16           0x000057ac82bf1855 _start + 37
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Ubuntu clang version 16.0.6 (++20231112100510+7cbf1a259152-1~exp1~20231112100554.106)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
```
&lt;/details&gt;
</details>


---

### Comment 2 - tbaederr

With debug symbols:
```
invalid cast kind for integral value
UNREACHABLE executed at /home/tbaeder/code/llvm-project/clang/lib/AST/ExprConstant.cpp:14482!

    #3: ErrorHandling.cpp:213  llvm::llvm_unreachable_internal(msg="invalid cast kind for integral value", file="/home/tbaeder/code/llvm-project/clang/lib/AST/ExprConstant.cpp", line=14482)
    #4: ExprConstant.cpp:14482  (anonymous namespace)::IntExprEvaluator::VisitCastExpr(this=0x00007fff6b7ba110, E=0x0000521000074eb8)
    #5: StmtNodes.inc:522  clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitImplicitCastExpr(this=0x00007fff6b7ba110, S=0x0000521000074eb8)
    #6: StmtNodes.inc:522  clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(this=0x00007fff6b7ba110, S=0x0000521000074eb8)
    #7: ExprConstant.cpp:8368  (anonymous namespace)::ExprEvaluatorBase<(anonymous namespace)::IntExprEvaluator>::VisitStmtExpr(this=0x00007fff6b7ba110, E=0x0000521000074ef8)
    #8: StmtNodes.inc:92  clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(this=0x00007fff6b7ba110, S=0x0000521000074ef8)
    #9: ExprConstant.cpp:14540  (anonymous namespace)::IntExprEvaluator::VisitCastExpr(this=0x00007fff6b7ba110, E=0x0000521000074f48)
    #10: StmtNodes.inc:522  clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitImplicitCastExpr(this=0x00007fff6b7ba110, S=0x0000521000074f48)
    #11: StmtNodes.inc:522  clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(this=0x00007fff6b7ba110, S=0x0000521000074f48)
    #12: ExprConstant.cpp:15934  Evaluate(Result=0x00007fff6b483d20, Info=0x00007fff6b7b9060, E=0x0000521000074f48)
    #13: ExprConstant.cpp:13485  (anonymous namespace)::DataRecursiveIntBinOpEvaluator::EvaluateExpr(this=0x00007fff6b8cc040, E=0x0000521000074f48, Result=0x00007fff6b483d20)
    #14: ExprConstant.cpp:13667  (anonymous namespace)::DataRecursiveIntBinOpEvaluator::process(this=0x00007fff6b8cc040, Result=0x00007fff6b483d20)
    #15: ExprConstant.cpp:13451  (anonymous namespace)::DataRecursiveIntBinOpEvaluator::Traverse(this=0x00007fff6b8cc040, E=0x0000521000074f68)
    #16: ExprConstant.cpp:14113  (anonymous namespace)::IntExprEvaluator::VisitBinaryOperator(this=0x00007fff6b7b9910, E=0x0000521000074f68)
    #17: StmtVisitor.h:129  clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::VisitBinAdd(this=0x00007fff6b7b9910, S=0x0000521000074f68)
    #18: StmtVisitor.h:55  clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(this=0x00007fff6b7b9910, S=0x0000521000074f68)
    #19: ExprConstant.cpp:15934  Evaluate(Result=0x00007fff6b68f460, Info=0x00007fff6b7b9060, E=0x0000521000074f68)
    #20: ExprConstant.cpp:16050  EvaluateAsRValue(Info=0x00007fff6b7b9060, E=0x0000521000074f68, Result=0x00007fff6b68f460)
    #21: ExprConstant.cpp:16121  EvaluateAsRValue(E=0x0000521000074f68, Result=0x00007fff6b68f450, Ctx=0x000052a000000200, Info=0x00007fff6b7b9060)
    #22: ExprConstant.cpp:16170  clang::Expr::EvaluateAsRValue(this=0x0000521000074f68, Result=0x00007fff6b68f450, Ctx=0x000052a000000200, InConstantContext=false) const
    #23: SemaChecking.cpp:9522  GetExprRange(C=0x000052a000000200, E=0x0000521000074f68, MaxWidth=32, InConstantContext=false, Approximate=true)
    #24: SemaChecking.cpp:9766  GetExprRange(C=0x000052a000000200, E=0x0000521000074f68, InConstantContext=false, Approximate=true)
    #25: SemaChecking.cpp:11131  clang::Sema::CheckImplicitConversion(this=0x0000529000005200, E=0x0000521000074f68, T=QualType @ 0x00007fff6b9df020, CC=(ID = 378), ICContext=0x0000000000000000, IsListInit=false)
    #26: SemaChecking.cpp:11408  AnalyzeImplicitConversions(S=0x0000529000005200, Item=AnalyzeImplicitConversionsWorkItem @ 0x00007fff6b7b8820, WorkList=0x00007fff6b5fec20)
    #27: SemaChecking.cpp:11505  AnalyzeImplicitConversions(S=0x0000529000005200, OrigE=0x0000521000074f90, CC=(ID = 378), IsListInit=false)
    #28: SemaChecking.cpp:11770  clang::Sema::CheckImplicitConversions(this=0x0000529000005200, E=0x0000521000074f90, CC=(ID = 378))
```
The expression in question:
```
ImplicitCastExpr 0x521000074eb8 'atomic_char':'_Atomic(char)' <NonAtomicToAtomic>
`-ImplicitCastExpr 0x521000074e98 'char' <AtomicToNonAtomic>
  `-ImplicitCastExpr 0x521000074e78 'atomic_char':'_Atomic(char)' <LValueToRValue>
    `-DeclRefExpr 0x521000074e50 'atomic_char':'_Atomic(char)' lvalue Var 0x521000074c80 'counter' 'atomic_char':'_Atomic(char)'

```


---

### Comment 3 - shafik

Looks like this crash existed since clang-3.3: https://godbolt.org/z/noqv1sbdv

---

### Comment 4 - tbaederr

This probably just needs to be handled the same as `CK_AtomicToNonAtomic`? Not sure why `AtomicToNonAtomic` would be possible for integral values but `NonAtomicToAtomic` wouldn't be.

---

### Comment 5 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor has already been assigned to this issue. If you believe that no one is actually working on it despite an assignment, ping the person. After one week without a response, the assignee may be changed.
1. In the comments of this issue, request for it to be assigned to you, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 6 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Michael Jabbour (michael-jabbour-sonarsource)

<details>
The following example crashes `clang-16`, `clang-17` and `clang-18`:

```c
typedef _Atomic char atomic_char;

atomic_char counter;

char load_plus_one() {
  return ({counter;}) + 1;
}
```

See [on Compiler Explorer](https://godbolt.org/z/s6hxnY518).

The crash seems reproducible on release builds on Ubuntu 22 installed from apt.llvm.org, but I can't observe it on non-asserted builds on Compiler Explorer for some reason...

&lt;details&gt;
  &lt;summary&gt;Stack trace&lt;/summary&gt;

```
$ clang-18 -fsyntax-only minimal.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang-18 -fsyntax-only minimal.c
1.	minimal.c:6:26: current parser token ';'
2.	minimal.c:5:22: parsing function body 'load_plus_one'
3.	minimal.c:5:22: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM-18.so.18.1   0x000079cec1794716 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 54
1  libLLVM-18.so.18.1   0x000079cec17926d0 llvm::sys::RunSignalHandlers() + 80
2  libLLVM-18.so.18.1   0x000079cec16e41f0
3  libc.so.6            0x000079cec0242520
4  libclang-cpp.so.18.1 0x000079cec90da059
5  libclang-cpp.so.18.1 0x000062b1379df1f8
clang-18: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang-18: note: diagnostic msg: 
```
I am able to get a better stack trace on clang-16:
```
$ clang-16 -fsyntax-only minimal.c
*** stack smashing detected ***: terminated
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang-16 -fsyntax-only minimal.c
1.	minimal.c:6:26: current parser token ';'
2.	minimal.c:5:22: parsing function body 'load_plus_one'
3.	minimal.c:5:22: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM-16.so.1    0x0000794646bbec16 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 54
1  libLLVM-16.so.1    0x0000794646bbcdb0 llvm::sys::RunSignalHandlers() + 80
2  libLLVM-16.so.1    0x0000794646afd600
3  libc.so.6          0x0000794645442520
4  libc.so.6          0x00007946454969fc pthread_kill + 300
5  libc.so.6          0x0000794645442476 raise + 22
6  libc.so.6          0x00007946454287f3 abort + 211
7  libc.so.6          0x0000794645489676
8  libc.so.6          0x000079464553659a __fortify_fail + 42
9  libc.so.6          0x0000794645536566
10 libclang-cpp.so.16 0x000079464e2f1cde
11 libclang-cpp.so.16 0x000079464e2daf1b
12 libclang-cpp.so.16 0x000079464e2f1307
13 libclang-cpp.so.16 0x000079464e2af018
14 libclang-cpp.so.16 0x000079464e2d67a2
15 libclang-cpp.so.16 0x000079464e2af018
16 libclang-cpp.so.16 0x000079464e2acaf0
17 libclang-cpp.so.16 0x000079464e2a8bca clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const + 602
18 libclang-cpp.so.16 0x000079464e7e5412
19 libclang-cpp.so.16 0x000079464e7e2255
20 libclang-cpp.so.16 0x000079464e7d6995
21 libclang-cpp.so.16 0x000079464e7d901f clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) + 207
22 libclang-cpp.so.16 0x000079464eb14604 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) + 324
23 libclang-cpp.so.16 0x000079464ecf0db5 clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) + 2485
24 libclang-cpp.so.16 0x000079464ecf033e clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) + 94
25 libclang-cpp.so.16 0x000079464e0748b6 clang::Parser::ParseReturnStatement() + 326
26 libclang-cpp.so.16 0x000079464e06eee9 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) + 2649
27 libclang-cpp.so.16 0x000079464e06e25d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) + 349
28 libclang-cpp.so.16 0x000079464e0766f0 clang::Parser::ParseCompoundStatementBody(bool) + 1600
29 libclang-cpp.so.16 0x000079464e0777af clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) + 175
30 libclang-cpp.so.16 0x000079464e091910 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) + 3376
31 libclang-cpp.so.16 0x000079464dfe1832 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 4802
32 libclang-cpp.so.16 0x000079464e090a47 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 1031
33 libclang-cpp.so.16 0x000079464e090469 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 457
34 libclang-cpp.so.16 0x000079464e08f707 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 1943
35 libclang-cpp.so.16 0x000079464e08daae clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1182
36 libclang-cpp.so.16 0x000079464dfcd75e clang::ParseAST(clang::Sema&amp;, bool, bool) + 718
37 libclang-cpp.so.16 0x000079464fbe5565 clang::FrontendAction::Execute() + 85
38 libclang-cpp.so.16 0x000079464fb62754 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 724
39 libclang-cpp.so.16 0x000079464fc5dfe4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 660
40 clang-16           0x000057ac82bf7f4c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 2348
41 clang-16           0x000057ac82bf51f0
42 libclang-cpp.so.16 0x000079464f83e9a2
43 libLLVM-16.so.1    0x0000794646afd39c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 140
44 libclang-cpp.so.16 0x000079464f83e0c3 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 355
45 libclang-cpp.so.16 0x000079464f809922 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 866
46 libclang-cpp.so.16 0x000079464f809b7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 142
47 libclang-cpp.so.16 0x000079464f8242fd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 349
48 clang-16           0x000057ac82bf48b5 clang_main(int, char**) + 11541
49 libc.so.6          0x0000794645429d90
50 libc.so.6          0x0000794645429e40 __libc_start_main + 128
51 clang-16           0x000057ac82bf1855 _start + 37
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Ubuntu clang version 16.0.6 (++20231112100510+7cbf1a259152-1~exp1~20231112100554.106)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg: 
```
&lt;/details&gt;
</details>


---

### Comment 7 - c8ef

Hi, I would like to work on this.

---

### Comment 8 - daniel-petrovic

@c8ef Are you still working on this issue ? Otherwise I would be glad to take it over .. Just give me an Info. Thanks

---

### Comment 9 - daniel-petrovic

As it looks like the current issue comes from the fact  that (having ToType = `char`) ` IsStandardConversion `from `atomic_char ` to  `char ` will return` true` or `false `depending on that if `From `expression is L-value or not, which is causing the implicit conversion to fail because no `InitializationSequence `in` Sema::PerformCopyInitialization `can be constructed:

```
 // SemaOverload.cpp
 static bool IsStandardConversion(Sema &S, Expr* From, QualType ToType,
                                   bool InOverloadResolution,
                                   StandardConversionSequence &SCS,
                                   bool CStyle,
                                   bool AllowObjCWritebackConversion) {
...
    // Lvalue-to-rvalue conversion (C++11 4.1):
    //   A glvalue (3.10) of a non-function, non-array type T can
    //   be converted to a prvalue.
    bool argIsLValue = From->isGLValue();
    if (argIsLValue && !FromType->canDecayToPointerType() &&
        S.Context.getCanonicalType(T: FromType) != S.Context.OverloadTy) {
      SCS.First = ICK_Lvalue_To_Rvalue;

      // C11 6.3.2.1p2:
      //   ... if the lvalue has atomic type, the value has the non-atomic version
      //   of the type of the lvalue ...
      if (const AtomicType *Atomic = FromType->getAs<AtomicType>())
        FromType = Atomic->getValueType();
..

```

So the issue seems not to be related to StmtExpr itself, but converting pr-value atomic expressions.

These 2 examples crash the same way: 

https://godbolt.org/z/61Tcncqdf
https://godbolt.org/z/dGaazj6rs

and these 2 exmples provide the same error:
https://godbolt.org/z/14dhfsn1M
https://godbolt.org/z/exKEfxhad


The fix seems to be either:

-  to consider conversions of non-L-value atomic expressions in  `IsStandardConversion `
- or to always convert r-value atomic expressions to non-atomic expression

Would appreciate some hints from someone more experienced.


---

