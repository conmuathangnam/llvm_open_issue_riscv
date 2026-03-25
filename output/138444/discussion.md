# Crash in Clang when using sizeof(int[vec.size()]) with invalid std::vector initialization since version 19.0

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/138444
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-05-05T17:14:32Z

## Body

## Summary 
Clang crashes with an internal compiler error when evaluating sizeof(int[vec.size()]) where vec is incorrectly initialized from an int. 

## Assertion
No assertion appears

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:7:1: current parser token '}'
2.	<source>:4:12: parsing function body 'main'
3.	<source>:4:12: in compound statement ('{}')
 #0 0x0000000003f710c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f710c8)
 #1 0x0000000003f6ed54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6ed54)
 #2 0x0000000003eb3d38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072de4b242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000076cb014 clang::computeDependence(clang::UnaryExprOrTypeTraitExpr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76cb014)
 #5 0x0000000006b3bcf9 clang::Sema::CreateUnaryExprOrTypeTraitExpr(clang::TypeSourceInfo*, clang::SourceLocation, clang::UnaryExprOrTypeTrait, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3bcf9)
 #6 0x0000000006b6ec5e clang::Sema::ActOnUnaryExprOrTypeTraitExpr(clang::SourceLocation, clang::UnaryExprOrTypeTrait, bool, void*, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6ec5e)
 #7 0x000000000666eefd clang::Parser::ParseUnaryExprOrTypeTraitExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666eefd)
 #8 0x000000000666240a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666240a)
 #9 0x00000000066642d7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66642d7)
#10 0x0000000006664369 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6664369)
#11 0x0000000006668d49 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6668d49)
#12 0x00000000066ecc39 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ecc39)
#13 0x00000000066e46f2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e46f2)
#14 0x00000000066e561d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e561d)
#15 0x00000000066ed333 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ed333)
#16 0x00000000066edafa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66edafa)
#17 0x00000000065f6003 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f6003)
#18 0x000000000662c6ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662c6ad)
#19 0x00000000065e9b3e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e9b3e)
#20 0x00000000065ea2f9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ea2f9)
#21 0x00000000065f1c1a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f1c1a)
#22 0x00000000065f2bbd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f2bbd)
#23 0x00000000065e4f9a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e4f9a)
#24 0x00000000049130c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49130c8)
#25 0x0000000004c06635 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c06635)
#26 0x0000000004b85cae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b85cae)
#27 0x0000000004cfa479 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cfa479)
#28 0x0000000000da5c6f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda5c6f)
#29 0x0000000000d9be3a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x0000000004979979 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003eb41d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb41d4)
#32 0x0000000004979f8f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x000000000493c3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493c3dd)
#34 0x000000000493d46e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493d46e)
#35 0x00000000049452c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49452c5)
#36 0x0000000000da1bd8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda1bd8)
#37 0x0000000000c273a4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc273a4)
#38 0x000072de4b229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x000072de4b229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
```

## Program

```
#include <vector>
#include <variant>
using namespace std;
int main() {
  vector<int> vec1 = 2;
  sizeof(int[vec1.size()])
}
```

## To quickly reproduce

It happens in versions 19, 20 and current.

https://gcc.godbolt.org/z/Krh4bPqTY

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary 
Clang crashes with an internal compiler error when evaluating sizeof(int[vec.size()]) where vec is incorrectly initialized from an int. 

## Assertion
No assertion appears

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:7:1: current parser token '}'
2.	&lt;source&gt;:4:12: parsing function body 'main'
3.	&lt;source&gt;:4:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003f710c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f710c8)
 #<!-- -->1 0x0000000003f6ed54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6ed54)
 #<!-- -->2 0x0000000003eb3d38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072de4b242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000076cb014 clang::computeDependence(clang::UnaryExprOrTypeTraitExpr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76cb014)
 #<!-- -->5 0x0000000006b3bcf9 clang::Sema::CreateUnaryExprOrTypeTraitExpr(clang::TypeSourceInfo*, clang::SourceLocation, clang::UnaryExprOrTypeTrait, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3bcf9)
 #<!-- -->6 0x0000000006b6ec5e clang::Sema::ActOnUnaryExprOrTypeTraitExpr(clang::SourceLocation, clang::UnaryExprOrTypeTrait, bool, void*, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6ec5e)
 #<!-- -->7 0x000000000666eefd clang::Parser::ParseUnaryExprOrTypeTraitExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666eefd)
 #<!-- -->8 0x000000000666240a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666240a)
 #<!-- -->9 0x00000000066642d7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66642d7)
#<!-- -->10 0x0000000006664369 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6664369)
#<!-- -->11 0x0000000006668d49 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6668d49)
#<!-- -->12 0x00000000066ecc39 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ecc39)
#<!-- -->13 0x00000000066e46f2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e46f2)
#<!-- -->14 0x00000000066e561d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e561d)
#<!-- -->15 0x00000000066ed333 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ed333)
#<!-- -->16 0x00000000066edafa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66edafa)
#<!-- -->17 0x00000000065f6003 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f6003)
#<!-- -->18 0x000000000662c6ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662c6ad)
#<!-- -->19 0x00000000065e9b3e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e9b3e)
#<!-- -->20 0x00000000065ea2f9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ea2f9)
#<!-- -->21 0x00000000065f1c1a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f1c1a)
#<!-- -->22 0x00000000065f2bbd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f2bbd)
#<!-- -->23 0x00000000065e4f9a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e4f9a)
#<!-- -->24 0x00000000049130c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49130c8)
#<!-- -->25 0x0000000004c06635 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c06635)
#<!-- -->26 0x0000000004b85cae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b85cae)
#<!-- -->27 0x0000000004cfa479 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cfa479)
#<!-- -->28 0x0000000000da5c6f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda5c6f)
#<!-- -->29 0x0000000000d9be3a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004979979 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003eb41d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb41d4)
#<!-- -->32 0x0000000004979f8f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x000000000493c3dd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493c3dd)
#<!-- -->34 0x000000000493d46e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493d46e)
#<!-- -->35 0x00000000049452c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49452c5)
#<!-- -->36 0x0000000000da1bd8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda1bd8)
#<!-- -->37 0x0000000000c273a4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc273a4)
#<!-- -->38 0x000072de4b229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x000072de4b229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
```

## Program

```
#include &lt;vector&gt;
#include &lt;variant&gt;
using namespace std;
int main() {
  vector&lt;int&gt; vec1 = 2;
  sizeof(int[vec1.size()])
}
```

## To quickly reproduce

It happens in versions 19, 20 and current.

https://gcc.godbolt.org/z/Krh4bPqTY
</details>


---

### Comment 2 - frederick-vs-ja

Looks likes that the crash happens when trying to constant-folding the VLA size.

---

### Comment 3 - AaronBallman

Reduced test case:
```
struct S {
  S(const char *);

  int size() const;
};

int main() {
  S vec1 = 2;
  sizeof(int[vec1.size()])
}
```
https://gcc.godbolt.org/z/1hxaKaxj8

---

