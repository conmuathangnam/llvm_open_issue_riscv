# [clang] Crash with official test `Parser/cxx0x-attributes.cpp` and default template argument

**Author:** Rush10233
**URL:** https://github.com/llvm/llvm-project/issues/132256
**Status:** Closed
**Labels:** c++11, clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-04-01T10:44:11Z

## Body

Extended from [https://github.com/llvm/llvm-project/blob/main/clang/test/Parser/cxx0x-attributes.cpp](https://github.com/llvm/llvm-project/blob/main/clang/test/Parser/cxx0x-attributes.cpp).

The crash came when I was trying to add some useless template arguments with default values to the code.

Please see [https://godbolt.org/z/hYPEKhzMc](https://godbolt.org/z/hYPEKhzMc)


Stack Dump:

```
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:312:41: current parser token '{'
2.	<source>:291:13: parsing function body 'baz'
3.	<source>:291:13: in compound statement ('{}')
 #0 0x0000000003b7abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b7abe8)
 #1 0x0000000003b78d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b78d2c)
 #2 0x0000000003ac85a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a5387e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006fc7404 clang::ASTContext::getElaboratedType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier*, clang::QualType, clang::TagDecl*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fc7404)
 #5 0x00000000065084a6 clang::Sema::BuildStdInitializerList(clang::QualType, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65084a6)
 #6 0x0000000006bfef42 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6bfef42)
 #7 0x0000000006a61abf FinishForRangeVarDecl(clang::Sema&, clang::VarDecl*, clang::Expr*, clang::SourceLocation, int) SemaStmt.cpp:0:0
 #8 0x0000000006a77caa clang::Sema::ActOnCXXForRangeStmt(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Stmt*, clang::Stmt*, clang::SourceLocation, clang::Expr*, clang::SourceLocation, clang::Sema::BuildForRangeKind, llvm::ArrayRef<clang::MaterializeTemporaryExpr*>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a77caa)
 #9 0x00000000061ed100 clang::Parser::ParseForStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ed100)
#10 0x00000000061e94c4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e94c4)
#11 0x00000000061ea69e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ea69e)
#12 0x00000000061f30bd clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f30bd)
#13 0x00000000061f3afa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f3afa)
#14 0x00000000060e1ee6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e1ee6)
#15 0x000000000612cfdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x612cfdf)
#16 0x00000000060da427 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60da427)
#17 0x00000000060db23e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60db23e)
#18 0x00000000060e4f3a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e4f3a)
#19 0x00000000060e67d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e67d8)
#20 0x00000000060d4b5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d4b5a)
#21 0x00000000044f8d8d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44f8d8d)
#22 0x00000000047ddc31 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47ddc31)
#23 0x000000000475c1bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x475c1bb)
#24 0x00000000048cddb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48cddb3)
#25 0x0000000000d65be1 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd65be1)
#26 0x0000000000d5e3dd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x0000000004546089 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003ac89c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ac89c3)
#29 0x00000000045462a9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x00000000045095cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45095cd)
#31 0x000000000450a591 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450a591)
#32 0x000000000451492c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451492c)
#33 0x0000000000d629b1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd629b1)
#34 0x0000000000c23264 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc23264)
#35 0x00007a5387e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x00007a5387e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000d5de75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5de75)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Rush10233)

<details>
Extended from [https://github.com/llvm/llvm-project/blob/main/clang/test/Parser/cxx0x-attributes.cpp](https://github.com/llvm/llvm-project/blob/main/clang/test/Parser/cxx0x-attributes.cpp).

The crash came when I was trying to add some useless template arguments with default values to the code.

Please see [https://godbolt.org/z/hYPEKhzMc](https://godbolt.org/z/hYPEKhzMc)


Stack Dump:

```
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:312:41: current parser token '{'
2.	&lt;source&gt;:291:13: parsing function body 'baz'
3.	&lt;source&gt;:291:13: in compound statement ('{}')
 #<!-- -->0 0x0000000003b7abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b7abe8)
 #<!-- -->1 0x0000000003b78d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b78d2c)
 #<!-- -->2 0x0000000003ac85a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a5387e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006fc7404 clang::ASTContext::getElaboratedType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier*, clang::QualType, clang::TagDecl*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fc7404)
 #<!-- -->5 0x00000000065084a6 clang::Sema::BuildStdInitializerList(clang::QualType, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65084a6)
 #<!-- -->6 0x0000000006bfef42 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6bfef42)
 #<!-- -->7 0x0000000006a61abf FinishForRangeVarDecl(clang::Sema&amp;, clang::VarDecl*, clang::Expr*, clang::SourceLocation, int) SemaStmt.cpp:0:0
 #<!-- -->8 0x0000000006a77caa clang::Sema::ActOnCXXForRangeStmt(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Stmt*, clang::Stmt*, clang::SourceLocation, clang::Expr*, clang::SourceLocation, clang::Sema::BuildForRangeKind, llvm::ArrayRef&lt;clang::MaterializeTemporaryExpr*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a77caa)
 #<!-- -->9 0x00000000061ed100 clang::Parser::ParseForStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ed100)
#<!-- -->10 0x00000000061e94c4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e94c4)
#<!-- -->11 0x00000000061ea69e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ea69e)
#<!-- -->12 0x00000000061f30bd clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f30bd)
#<!-- -->13 0x00000000061f3afa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f3afa)
#<!-- -->14 0x00000000060e1ee6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e1ee6)
#<!-- -->15 0x000000000612cfdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x612cfdf)
#<!-- -->16 0x00000000060da427 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60da427)
#<!-- -->17 0x00000000060db23e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60db23e)
#<!-- -->18 0x00000000060e4f3a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e4f3a)
#<!-- -->19 0x00000000060e67d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e67d8)
#<!-- -->20 0x00000000060d4b5a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d4b5a)
#<!-- -->21 0x00000000044f8d8d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44f8d8d)
#<!-- -->22 0x00000000047ddc31 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47ddc31)
#<!-- -->23 0x000000000475c1bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x475c1bb)
#<!-- -->24 0x00000000048cddb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48cddb3)
#<!-- -->25 0x0000000000d65be1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd65be1)
#<!-- -->26 0x0000000000d5e3dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004546089 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003ac89c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ac89c3)
#<!-- -->29 0x00000000045462a9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x00000000045095cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45095cd)
#<!-- -->31 0x000000000450a591 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450a591)
#<!-- -->32 0x000000000451492c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451492c)
#<!-- -->33 0x0000000000d629b1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd629b1)
#<!-- -->34 0x0000000000c23264 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc23264)
#<!-- -->35 0x00007a5387e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007a5387e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000d5de75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5de75)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 2 - shafik

Goes back to clang-7: https://godbolt.org/z/jccd7T5Gv

---

### Comment 3 - zyn0217

> Extended from https://github.com/llvm/llvm-project/blob/main/clang/test/Parser/cxx0x-attributes.cpp.
> 
> The crash came when I was trying to add some useless template arguments with default values to the code.
> 

Are you running a fuzzer on existing tests? It would be better to highlight that in the issue body if this comes from a fuzzer, thanks.

---

### Comment 4 - Endilll

Seems to be related to error recovery and attributes on the first statement of `for`. Note `-ferror-limit=1` in the reduced example.
The crash seems to go all the way back to Clang 3.7.
Reduced by me (no C-Reduce this time 😄): https://godbolt.org/z/hr9G98zsh
```cpp
namespace std {
template <class, int = 0>
class initializer_list {};
} // namespace std

using a::b;

void baz () {
  for ([[]];;) {}
  for (int n : { 0 }) {}
}
```
Clang 21.0 crash:
```
<source>:6:7: error: use of undeclared identifier 'a'
    6 | using a::b;
      |       ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]

clang++: /root/llvm-project/clang/include/clang/AST/Type.h:945:
const clang::ExtQualsTypeCommonBase* clang::QualType::getCommonPtr() const:
Assertion `!isNull() && "Cannot retrieve a NULL type pointer"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ferror-limit=1 <source>
1.	<source>:10:23: current parser token '{'
2.	<source>:8:13: parsing function body 'baz'
3.	<source>:8:13: in compound statement ('{}')
 #0 0x0000000003e8f628 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e8f628)
 #1 0x0000000003e8d2b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e8d2b4)
 #2 0x0000000003dd2148 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c4487e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007c4487e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007c4487e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007c4487e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007c4487e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007c4487e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000474a55a clang::QualType::isCanonical() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x474a55a)
#10 0x00000000073a529f clang::ASTContext::getElaboratedType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier*, clang::QualType, clang::TagDecl*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73a529f)
#11 0x00000000068e2b0d clang::Sema::BuildStdInitializerList(clang::QualType, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e2b0d)
#12 0x0000000006fbcc2a clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fbcc2a)
#13 0x0000000006e23fd7 FinishForRangeVarDecl(clang::Sema&, clang::VarDecl*, clang::Expr*, clang::SourceLocation, int) SemaStmt.cpp:0:0
#14 0x0000000006e30eaf clang::Sema::ActOnCXXForRangeStmt(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Stmt*, clang::Stmt*, clang::SourceLocation, clang::Expr*, clang::SourceLocation, clang::Sema::BuildForRangeKind, llvm::ArrayRef<clang::MaterializeTemporaryExpr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e30eaf)
#15 0x00000000065ba1d5 clang::Parser::ParseForStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ba1d5)
#16 0x00000000065b6caf clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b6caf)
#17 0x00000000065b831d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b831d)
#18 0x00000000065bfdd3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bfdd3)
#19 0x00000000065c059a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65c059a)
#20 0x00000000064cad03 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64cad03)
#21 0x00000000064ffc3d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ffc3d)
#22 0x00000000064be95e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64be95e)
#23 0x00000000064bf119 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64bf119)
#24 0x00000000064c69e3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c69e3)
#25 0x00000000064c78bd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c78bd)
#26 0x00000000064b9e4a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64b9e4a)
#27 0x000000000484c098 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484c098)
#28 0x0000000004b1c685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1c685)
#29 0x0000000004a9e54e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a9e54e)
#30 0x0000000004c0c87e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0c87e)
#31 0x0000000000d5057f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5057f)
#32 0x0000000000d47d3a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x00000000048945f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x0000000003dd25e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3dd25e4)
#35 0x0000000004894c0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x000000000485767d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485767d)
#37 0x000000000485870e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485870e)
#38 0x000000000485fde5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485fde5)
#39 0x0000000000d4d378 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4d378)
#40 0x0000000000c140b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc140b4)
#41 0x00007c4487e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00007c4487e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000d477e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd477e5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-c-11

Author: None (Rush10233)

<details>
Extended from [https://github.com/llvm/llvm-project/blob/main/clang/test/Parser/cxx0x-attributes.cpp](https://github.com/llvm/llvm-project/blob/main/clang/test/Parser/cxx0x-attributes.cpp).

The crash came when I was trying to add some useless template arguments with default values to the code.

Please see [https://godbolt.org/z/hYPEKhzMc](https://godbolt.org/z/hYPEKhzMc)


Stack Dump:

```
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:312:41: current parser token '{'
2.	&lt;source&gt;:291:13: parsing function body 'baz'
3.	&lt;source&gt;:291:13: in compound statement ('{}')
 #<!-- -->0 0x0000000003b7abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b7abe8)
 #<!-- -->1 0x0000000003b78d2c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b78d2c)
 #<!-- -->2 0x0000000003ac85a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a5387e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006fc7404 clang::ASTContext::getElaboratedType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier*, clang::QualType, clang::TagDecl*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fc7404)
 #<!-- -->5 0x00000000065084a6 clang::Sema::BuildStdInitializerList(clang::QualType, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x65084a6)
 #<!-- -->6 0x0000000006bfef42 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6bfef42)
 #<!-- -->7 0x0000000006a61abf FinishForRangeVarDecl(clang::Sema&amp;, clang::VarDecl*, clang::Expr*, clang::SourceLocation, int) SemaStmt.cpp:0:0
 #<!-- -->8 0x0000000006a77caa clang::Sema::ActOnCXXForRangeStmt(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Stmt*, clang::Stmt*, clang::SourceLocation, clang::Expr*, clang::SourceLocation, clang::Sema::BuildForRangeKind, llvm::ArrayRef&lt;clang::MaterializeTemporaryExpr*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a77caa)
 #<!-- -->9 0x00000000061ed100 clang::Parser::ParseForStatement(clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ed100)
#<!-- -->10 0x00000000061e94c4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61e94c4)
#<!-- -->11 0x00000000061ea69e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61ea69e)
#<!-- -->12 0x00000000061f30bd clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f30bd)
#<!-- -->13 0x00000000061f3afa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f3afa)
#<!-- -->14 0x00000000060e1ee6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e1ee6)
#<!-- -->15 0x000000000612cfdf clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x612cfdf)
#<!-- -->16 0x00000000060da427 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60da427)
#<!-- -->17 0x00000000060db23e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60db23e)
#<!-- -->18 0x00000000060e4f3a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e4f3a)
#<!-- -->19 0x00000000060e67d8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60e67d8)
#<!-- -->20 0x00000000060d4b5a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60d4b5a)
#<!-- -->21 0x00000000044f8d8d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44f8d8d)
#<!-- -->22 0x00000000047ddc31 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47ddc31)
#<!-- -->23 0x000000000475c1bb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x475c1bb)
#<!-- -->24 0x00000000048cddb3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48cddb3)
#<!-- -->25 0x0000000000d65be1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd65be1)
#<!-- -->26 0x0000000000d5e3dd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000004546089 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003ac89c3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ac89c3)
#<!-- -->29 0x00000000045462a9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x00000000045095cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45095cd)
#<!-- -->31 0x000000000450a591 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450a591)
#<!-- -->32 0x000000000451492c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451492c)
#<!-- -->33 0x0000000000d629b1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd629b1)
#<!-- -->34 0x0000000000c23264 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc23264)
#<!-- -->35 0x00007a5387e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x00007a5387e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000d5de75 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5de75)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

