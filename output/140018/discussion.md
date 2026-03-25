# Clang crashes with assertion in ResolveSingleFunctionTemplateSpecialization with -std=c++20 since clang 10

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/140018

## Body

It is similar to https://github.com/llvm/llvm-project/issues/138098 but new Stack dump is shown. It only happens with std=c++20 flag.


## Assertion

```
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionTemplateDecl; From = clang::NamedDecl]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 <source>
1.	<source>:16:49: current parser token ';'
2.	<source>:12:1: parsing struct/union/class body 'g_foo_f1<>'
3.	<source>:15:5: parsing function body 'g_foo_f1<>::foo'
4.	<source>:15:5: in compound statement ('{}')
 #0 0x0000000003f98e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98e38)
 #1 0x0000000003f96ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f96ac4)
 #2 0x0000000003edb6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007daaf5042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007daaf50969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007daaf5042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007daaf50287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007daaf502871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007daaf5039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006f6ade4 clang::Sema::ResolveSingleFunctionTemplateSpecialization(clang::OverloadExpr*, bool, clang::DeclAccessPair*, clang::TemplateSpecCandidateSet*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f6ade4)
#10 0x000000000714921f ResolveOverloadForDeduction(clang::Sema&, clang::TemplateParameterList*, clang::Expr*, clang::QualType, bool, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#11 0x00000000071499c9 AdjustFunctionParmAndArgTypesForDeduction(clang::Sema&, clang::TemplateParameterList*, unsigned int, clang::QualType&, clang::QualType&, clang::Expr::Classification, clang::Expr*, unsigned int&, clang::TemplateSpecCandidateSet*) (.isra.0) SemaTemplateDeduction.cpp:0:0
#12 0x0000000007149cb8 DeduceTemplateArgumentsFromCallArgument(clang::Sema&, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg>&, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#13 0x0000000007166f7c clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7166f7c)
#14 0x0000000006fd32dc clang::Sema::DeduceFunctionTypeFromReturnExpr(clang::FunctionDecl*, clang::SourceLocation, clang::Expr*, clang::AutoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd32dc)
#15 0x0000000006fd567d clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd567d)
#16 0x0000000006fd6377 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd6377)
#17 0x0000000006730417 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6730417)
#18 0x00000000067336bd clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67336bd)
#19 0x0000000006734d3d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6734d3d)
#20 0x000000000673ca63 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673ca63)
#21 0x000000000673d22a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673d22a)
#22 0x0000000006646140 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6646140)
#23 0x0000000006645e2d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6645e2d)
#24 0x0000000006699ef6 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6699ef6)
#25 0x000000000669cce0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669cce0)
#26 0x000000000666f790 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666f790)
#27 0x00000000067489fc clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67489fc)
#28 0x000000000675659f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#29 0x000000000675694a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x675694a)
#30 0x00000000066764a0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66764a0)
#31 0x0000000006632e9c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6632e9c)
#32 0x0000000006633d8d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6633d8d)
#33 0x000000000662615a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662615a)
#34 0x0000000004942fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4942fd8)
#35 0x0000000004c39725 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c39725)
#36 0x0000000004bb834e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb834e)
#37 0x0000000004d2d289 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2d289)
#38 0x0000000000daba2f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaba2f)
#39 0x0000000000da1bfa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x00000000049aa2c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000003edbb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edbb94)
#42 0x00000000049aa8df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x000000000496c6ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496c6ad)
#44 0x000000000496d73e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496d73e)
#45 0x0000000004975a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4975a25)
#46 0x0000000000da7998 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7998)
#47 0x0000000000c2d064 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2d064)
#48 0x00007daaf5029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x00007daaf5029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000da16a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda16a5)
```

## Program

```
#include <iostream>
using namespace std;
template <typename... Funcs>
struct g_foo_f1;
template <typename Func>
struct g_foo_f1<Func>
{
    static Func foo()
    {}
};
template <>
struct g_foo_f1<>
{
    static constexpr auto foo()
    {
        return foo<decltype(g_foo_f1<int>::foo)>;
    }
};
```

## To reproduce quickly:

https://godbolt.org/z/3Tsxa5cPT



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
It is similar to https://github.com/llvm/llvm-project/issues/138098 but new Stack dump is shown. It only happens with std=c++20 flag.


## Assertion

```
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionTemplateDecl; From = clang::NamedDecl]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:16:49: current parser token ';'
2.	&lt;source&gt;:12:1: parsing struct/union/class body 'g_foo_f1&lt;&gt;'
3.	&lt;source&gt;:15:5: parsing function body 'g_foo_f1&lt;&gt;::foo'
4.	&lt;source&gt;:15:5: in compound statement ('{}')
 #<!-- -->0 0x0000000003f98e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98e38)
 #<!-- -->1 0x0000000003f96ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f96ac4)
 #<!-- -->2 0x0000000003edb6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007daaf5042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007daaf50969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007daaf5042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007daaf50287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007daaf502871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007daaf5039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f6ade4 clang::Sema::ResolveSingleFunctionTemplateSpecialization(clang::OverloadExpr*, bool, clang::DeclAccessPair*, clang::TemplateSpecCandidateSet*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f6ade4)
#<!-- -->10 0x000000000714921f ResolveOverloadForDeduction(clang::Sema&amp;, clang::TemplateParameterList*, clang::Expr*, clang::QualType, bool, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#<!-- -->11 0x00000000071499c9 AdjustFunctionParmAndArgTypesForDeduction(clang::Sema&amp;, clang::TemplateParameterList*, unsigned int, clang::QualType&amp;, clang::QualType&amp;, clang::Expr::Classification, clang::Expr*, unsigned int&amp;, clang::TemplateSpecCandidateSet*) (.isra.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->12 0x0000000007149cb8 DeduceTemplateArgumentsFromCallArgument(clang::Sema&amp;, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt;&amp;, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#<!-- -->13 0x0000000007166f7c clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7166f7c)
#<!-- -->14 0x0000000006fd32dc clang::Sema::DeduceFunctionTypeFromReturnExpr(clang::FunctionDecl*, clang::SourceLocation, clang::Expr*, clang::AutoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd32dc)
#<!-- -->15 0x0000000006fd567d clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd567d)
#<!-- -->16 0x0000000006fd6377 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd6377)
#<!-- -->17 0x0000000006730417 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6730417)
#<!-- -->18 0x00000000067336bd clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67336bd)
#<!-- -->19 0x0000000006734d3d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6734d3d)
#<!-- -->20 0x000000000673ca63 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673ca63)
#<!-- -->21 0x000000000673d22a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673d22a)
#<!-- -->22 0x0000000006646140 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6646140)
#<!-- -->23 0x0000000006645e2d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6645e2d)
#<!-- -->24 0x0000000006699ef6 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6699ef6)
#<!-- -->25 0x000000000669cce0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669cce0)
#<!-- -->26 0x000000000666f790 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666f790)
#<!-- -->27 0x00000000067489fc clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67489fc)
#<!-- -->28 0x000000000675659f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->29 0x000000000675694a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x675694a)
#<!-- -->30 0x00000000066764a0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66764a0)
#<!-- -->31 0x0000000006632e9c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6632e9c)
#<!-- -->32 0x0000000006633d8d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6633d8d)
#<!-- -->33 0x000000000662615a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662615a)
#<!-- -->34 0x0000000004942fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4942fd8)
#<!-- -->35 0x0000000004c39725 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c39725)
#<!-- -->36 0x0000000004bb834e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb834e)
#<!-- -->37 0x0000000004d2d289 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2d289)
#<!-- -->38 0x0000000000daba2f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaba2f)
#<!-- -->39 0x0000000000da1bfa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->40 0x00000000049aa2c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x0000000003edbb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edbb94)
#<!-- -->42 0x00000000049aa8df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x000000000496c6ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496c6ad)
#<!-- -->44 0x000000000496d73e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496d73e)
#<!-- -->45 0x0000000004975a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4975a25)
#<!-- -->46 0x0000000000da7998 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7998)
#<!-- -->47 0x0000000000c2d064 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2d064)
#<!-- -->48 0x00007daaf5029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x00007daaf5029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000da16a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda16a5)
```

## Program

```
#include &lt;iostream&gt;
using namespace std;
template &lt;typename... Funcs&gt;
struct g_foo_f1;
template &lt;typename Func&gt;
struct g_foo_f1&lt;Func&gt;
{
    static Func foo()
    {}
};
template &lt;&gt;
struct g_foo_f1&lt;&gt;
{
    static constexpr auto foo()
    {
        return foo&lt;decltype(g_foo_f1&lt;int&gt;::foo)&gt;;
    }
};
```

## To reproduce quickly:

https://godbolt.org/z/3Tsxa5cPT


</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (mariete1223)

<details>
It is similar to https://github.com/llvm/llvm-project/issues/138098 but new Stack dump is shown. It only happens with std=c++20 flag.


## Assertion

```
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionTemplateDecl; From = clang::NamedDecl]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:16:49: current parser token ';'
2.	&lt;source&gt;:12:1: parsing struct/union/class body 'g_foo_f1&lt;&gt;'
3.	&lt;source&gt;:15:5: parsing function body 'g_foo_f1&lt;&gt;::foo'
4.	&lt;source&gt;:15:5: in compound statement ('{}')
 #<!-- -->0 0x0000000003f98e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98e38)
 #<!-- -->1 0x0000000003f96ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f96ac4)
 #<!-- -->2 0x0000000003edb6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007daaf5042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007daaf50969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007daaf5042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007daaf50287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007daaf502871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007daaf5039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006f6ade4 clang::Sema::ResolveSingleFunctionTemplateSpecialization(clang::OverloadExpr*, bool, clang::DeclAccessPair*, clang::TemplateSpecCandidateSet*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f6ade4)
#<!-- -->10 0x000000000714921f ResolveOverloadForDeduction(clang::Sema&amp;, clang::TemplateParameterList*, clang::Expr*, clang::QualType, bool, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#<!-- -->11 0x00000000071499c9 AdjustFunctionParmAndArgTypesForDeduction(clang::Sema&amp;, clang::TemplateParameterList*, unsigned int, clang::QualType&amp;, clang::QualType&amp;, clang::Expr::Classification, clang::Expr*, unsigned int&amp;, clang::TemplateSpecCandidateSet*) (.isra.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->12 0x0000000007149cb8 DeduceTemplateArgumentsFromCallArgument(clang::Sema&amp;, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt;&amp;, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#<!-- -->13 0x0000000007166f7c clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7166f7c)
#<!-- -->14 0x0000000006fd32dc clang::Sema::DeduceFunctionTypeFromReturnExpr(clang::FunctionDecl*, clang::SourceLocation, clang::Expr*, clang::AutoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd32dc)
#<!-- -->15 0x0000000006fd567d clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd567d)
#<!-- -->16 0x0000000006fd6377 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd6377)
#<!-- -->17 0x0000000006730417 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6730417)
#<!-- -->18 0x00000000067336bd clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67336bd)
#<!-- -->19 0x0000000006734d3d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6734d3d)
#<!-- -->20 0x000000000673ca63 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673ca63)
#<!-- -->21 0x000000000673d22a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673d22a)
#<!-- -->22 0x0000000006646140 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6646140)
#<!-- -->23 0x0000000006645e2d clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6645e2d)
#<!-- -->24 0x0000000006699ef6 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6699ef6)
#<!-- -->25 0x000000000669cce0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669cce0)
#<!-- -->26 0x000000000666f790 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666f790)
#<!-- -->27 0x00000000067489fc clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67489fc)
#<!-- -->28 0x000000000675659f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->29 0x000000000675694a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x675694a)
#<!-- -->30 0x00000000066764a0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66764a0)
#<!-- -->31 0x0000000006632e9c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6632e9c)
#<!-- -->32 0x0000000006633d8d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6633d8d)
#<!-- -->33 0x000000000662615a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662615a)
#<!-- -->34 0x0000000004942fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4942fd8)
#<!-- -->35 0x0000000004c39725 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c39725)
#<!-- -->36 0x0000000004bb834e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb834e)
#<!-- -->37 0x0000000004d2d289 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2d289)
#<!-- -->38 0x0000000000daba2f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaba2f)
#<!-- -->39 0x0000000000da1bfa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->40 0x00000000049aa2c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x0000000003edbb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edbb94)
#<!-- -->42 0x00000000049aa8df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x000000000496c6ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496c6ad)
#<!-- -->44 0x000000000496d73e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496d73e)
#<!-- -->45 0x0000000004975a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4975a25)
#<!-- -->46 0x0000000000da7998 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7998)
#<!-- -->47 0x0000000000c2d064 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2d064)
#<!-- -->48 0x00007daaf5029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x00007daaf5029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000da16a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda16a5)
```

## Program

```
#include &lt;iostream&gt;
using namespace std;
template &lt;typename... Funcs&gt;
struct g_foo_f1;
template &lt;typename Func&gt;
struct g_foo_f1&lt;Func&gt;
{
    static Func foo()
    {}
};
template &lt;&gt;
struct g_foo_f1&lt;&gt;
{
    static constexpr auto foo()
    {
        return foo&lt;decltype(g_foo_f1&lt;int&gt;::foo)&gt;;
    }
};
```

## To reproduce quickly:

https://godbolt.org/z/3Tsxa5cPT


</details>


---

