# clang frontend crash

**Author:** adrianimboden
**URL:** https://github.com/llvm/llvm-project/issues/58172
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-06-13T10:45:42Z

## Body

version: 15.0.1

[source_and_script.tar.gz](https://github.com/llvm/llvm-project/files/9719102/source_and_script.tar.gz)

stacktrace:
```
 #0 0x000055ec4db8f733 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/bin/clang-15+0x34f1733)
 #1 0x000055ec4db8d0ee llvm::sys::RunSignalHandlers() (/usr/bin/clang-15+0x34ef0ee)
 #2 0x000055ec4db8fe7f SignalHandler(int) Signals.cpp:0:0
 #3 0x00007f9ca4b8d420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x000055ec507baf39 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/usr/bin/clang-15+0x611cf39)
 #5 0x000055ec50a2097e clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/usr/bin/clang-15+0x638297e)
 #6 0x000055ec501efeae clang::Parser::ParseReturnStatement() (/usr/bin/clang-15+0x5b51eae)
 #7 0x000055ec501e80db clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/bin/clang-15+0x5b4a0db)
 #8 0x000055ec501e75bf clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/bin/clang-15+0x5b495bf)
 #9 0x000055ec501f2491 clang::Parser::ParseCompoundStatementBody(bool) (/usr/bin/clang-15+0x5b54491)
#10 0x000055ec501e7f09 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/bin/clang-15+0x5b49f09)
#11 0x000055ec501e75bf clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/bin/clang-15+0x5b495bf)
#12 0x000055ec501ec9ae clang::Parser::ParseIfStatement(clang::SourceLocation*) (/usr/bin/clang-15+0x5b4e9ae)
#13 0x000055ec501e80cb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/bin/clang-15+0x5b4a0cb)
#14 0x000055ec501e75bf clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/bin/clang-15+0x5b495bf)
#15 0x000055ec501f2491 clang::Parser::ParseCompoundStatementBody(bool) (/usr/bin/clang-15+0x5b54491)
#16 0x000055ec501f3d40 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/bin/clang-15+0x5b55d40)
#17 0x000055ec501c953b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/bin/clang-15+0x5b2b53b)
#18 0x000055ec5021ef37 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/bin/clang-15+0x5b80f37)
#19 0x000055ec501c7c6d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/bin/clang-15+0x5b29c6d)
#20 0x000055ec501c774e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/bin/clang-15+0x5b2974e)
#21 0x000055ec501c673b clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/bin/clang-15+0x5b2873b)
#22 0x000055ec501c4310 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/bin/clang-15+0x5b26310)
#23 0x000055ec501befde clang::ParseAST(clang::Sema&, bool, bool) (/usr/bin/clang-15+0x5b20fde)
#24 0x000055ec4e8504e2 clang::FrontendAction::Execute() (/usr/bin/clang-15+0x41b24e2)
#25 0x000055ec4e7af9c2 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/bin/clang-15+0x41119c2)
#26 0x000055ec4e92bed3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/bin/clang-15+0x428ded3)
#27 0x000055ec4c568e33 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang-15+0x1ecae33)
#28 0x000055ec4c566dd7 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#29 0x000055ec4c5669bc clang_main(int, char**) (/usr/bin/clang-15+0x1ec89bc)
#30 0x00007f9ca46dd083 __libc_start_main /build/glibc-SzIz7B/glibc-2.31/csu/../csu/libc-start.c:342:3
#31 0x000055ec4c562eee _start (/usr/bin/clang-15+0x1ec4eee)
```


## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - k-mana

I created a minimal repro code.
```
// clang -std=c++20 -fsyntax-only
#include <coroutine>

template<typename T>
struct task {
  struct promise_type {
    auto get_return_object() { return task(); }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
    void return_value(T) {}
  };
};

template<typename T, typename... Args>
struct std::coroutine_traits<task<T>, Args...> {
  using promise_type = typename task<T>::promise_type;
};

template<typename Fn>
int f2(int, Fn&&)
{
  return 0;
}

int f1()
{
  return f2(v1, []() -> task<int> {
    co_return v2;
  });
}
```
```
$ clang++-15 -std=c++20 -fsyntax-only -c repro.cpp
repro.cpp:29:15: error: use of undeclared identifier 'v2'
    co_return v2;
              ^
repro.cpp:28:13: error: use of undeclared identifier 'v1'
  return f2(v1, []() -> task<int> {
            ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang++-15 -std=c++20 -fsyntax-only -c repro.cpp
1.      repro.cpp:30:5: current parser token ';'
2.      repro.cpp:27:1: parsing function body 'f1'
3.      repro.cpp:27:1: in compound statement ('{}')
 #0 0x00007f4b1eaea3b1 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xf043b1)
 #1 0x00007f4b1eae80fe llvm::sys::RunSignalHandlers() (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xf020fe)
 #2 0x00007f4b1eae9771 llvm::sys::CleanupOnSignal(unsigned long) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xf03771)
 #3 0x00007f4b1ea0d67f (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe2767f)
 #4 0x00007f4b1d6c8520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007f4b26113089 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x153b089)
 #6 0x00007f4b2631c25a clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x174425a)
 #7 0x00007f4b2570d3cb clang::Parser::ParseReturnStatement() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb353cb)
 #8 0x00007f4b257071cd clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb2f1cd)
 #9 0x00007f4b257065af clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb2e5af)
#10 0x00007f4b2570f280 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb37280)
#11 0x00007f4b257102b5 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb382b5)
#12 0x00007f4b2572a592 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb52592)
#13 0x00007f4b2567ed58 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xaa6d58)
#14 0x00007f4b257295ba clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb515ba)
#15 0x00007f4b25729017 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb51017)
#16 0x00007f4b2572833a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb5033a)
#17 0x00007f4b25726603 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xb4e603)
#18 0x00007f4b25664e2e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0xa8ce2e)
#19 0x00007f4b2728cea7 clang::FrontendAction::Execute() (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x26b4ea7)
#20 0x00007f4b271fffb6 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x2627fb6)
#21 0x00007f4b2730ae4a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x2732e4a)
#22 0x000055fd56f5c9e0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-15/bin/clang+0x139e0)
#23 0x000055fd56f5abab (/usr/lib/llvm-15/bin/clang+0x11bab)
#24 0x00007f4b26e63bd2 (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x228bbd2)
#25 0x00007f4b1ea0d3de llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-15/bin/../lib/libLLVM-15.so.1+0xe273de)
#26 0x00007f4b26e63698 clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x228b698)
#27 0x00007f4b26e25a8a clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x224da8a)
#28 0x00007f4b26e25cde clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&, bool) const (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x224dcde)
#29 0x00007f4b26e42b1f clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/usr/lib/llvm-15/bin/../lib/libclang-cpp.so.15+0x226ab1f)
#30 0x000055fd56f5a311 clang_main(int, char**) (/usr/lib/llvm-15/bin/clang+0x11311)
#31 0x00007f4b1d6afd90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#32 0x00007f4b1d6afe40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#33 0x000055fd56f573b5 _start (/usr/lib/llvm-15/bin/clang+0xe3b5)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 15.0.6
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/repro-973b75.cpp
clang: note: diagnostic msg: /tmp/repro-973b75.sh
clang: note: diagnostic msg:

********************
```

---

### Comment 3 - shafik

Confirmed: https://godbolt.org/z/3afdzfn5s

I get a slightly different backtrace on trunk assert build: 

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:30:5: current parser token ';'
2.	<source>:27:1: parsing function body 'f1'
3.	<source>:27:1: in compound statement ('{}')
 #0 0x000055f3e47e7f1f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x370ef1f)
 #1 0x000055f3e47e5f4c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x370cf4c)
 #2 0x000055f3e4737b58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f13b0273420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x000055f3e6e5523e clang::Sema::Diag(clang::SourceLocation, unsigned int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d7c23e)
 #5 0x000055f3e71c52d1 emitEmptyLookupTypoDiagnostic(clang::TypoCorrection const&, clang::Sema&, clang::CXXScopeSpec const&, clang::DeclarationName, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, unsigned int, unsigned int) (.isra.0) SemaExpr.cpp:0:0
 #6 0x000055f3e71c626d std::_Function_handler<void (clang::TypoCorrection const&), clang::Sema::DiagnoseEmptyLookup(clang::Scope*, clang::CXXScopeSpec&, clang::LookupResult&, clang::CorrectionCandidateCallback&, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::TypoExpr**)::'lambda'(clang::TypoCorrection const&)>::_M_invoke(std::_Any_data const&, clang::TypoCorrection const&) SemaExpr.cpp:0:0
 #7 0x000055f3e7365416 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x628c416)
 #8 0x000055f3e76033c5 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x652a3c5)
 #9 0x000055f3e6dd7108 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5cfe108)
#10 0x000055f3e6ddac75 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d01c75)
#11 0x000055f3e6ddbff3 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d02ff3)
#12 0x000055f3e6ddceba clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d03eba)
#13 0x000055f3e6dde272 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5d05272)
#14 0x000055f3e6cf9e64 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5c20e64)
#15 0x000055f3e6d2b9bf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5c529bf)
#16 0x000055f3e6cf42d7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5c1b2d7)
#17 0x000055f3e6cf542f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#18 0x000055f3e6cfcec4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5c23ec4)
#19 0x000055f3e6cfe47d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5c2547d)
#20 0x000055f3e6ced7ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5c147ba)
#21 0x000055f3e594dcd5 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4874cd5)
#22 0x000055f3e5239df1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4160df1)
#23 0x000055f3e51bc703 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x40e3703)
#24 0x000055f3e531a1d3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x42411d3)
#25 0x000055f3e20d837c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xfff37c)
#26 0x000055f3e20d3b3f ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x000055f3e5018f7d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x000055f3e4737fb7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x365efb7)
#29 0x000055f3e50191bc clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x000055f3e4fe059c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f0759c)
#31 0x000055f3e4fe0f8d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f07f8d)
#32 0x000055f3e4feb5bc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f125bc)
#33 0x000055f3e20d60ef clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xffd0ef)
#34 0x000055f3e1fd6cc5 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xefdcc5)
#35 0x00007f13afd21083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#36 0x000055f3e20cfd1e _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xff6d1e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

---

