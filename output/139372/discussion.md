# ICE in FindDeclaringClass with alignas-annotated subobjects of the same type since clang 13.0

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/139372

## Body

Clang crashes with an internal compiler error when a struct contains multiple subobjects of the same type but each is annotated with a different alignas specification.

## Assertion

```
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::CXXRecordDecl; From = clang::DeclContext]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:7:25: current parser token 's2'
2.	<source>:2:12: parsing function body 'main'
3.	<source>:2:12: in compound statement ('{}')
4.	<source>:5:3: parsing struct/union/class body 'M'
 #0 0x0000000003f8ac78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8ac78)
 #1 0x0000000003f88904 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f88904)
 #2 0x0000000003ecd6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f3b3b442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f3b3b4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f3b3b442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f3b3b4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f3b3b42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f3b3b439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000067fbbb5 FindDeclaringClass(clang::NamedDecl*) SemaAccess.cpp:0:0
#10 0x00000000067feb1c (anonymous namespace)::AccessTarget::initialize() SemaAccess.cpp:0:0
#11 0x0000000006805cbf clang::Sema::CheckLookupAccess(clang::LookupResult const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6805cbf)
#12 0x000000000681bd52 clang::LookupResult::~LookupResult() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x681bd52)
#13 0x00000000069b3249 clang::Sema::ActOnTag(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::CXXScopeSpec&, clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributesView const&, clang::AccessSpecifier, clang::SourceLocation, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, bool&, clang::SourceLocation, bool, clang::ActionResult<clang::OpaquePtr<clang::QualType>, false>, bool, bool, clang::OffsetOfKind, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69b3249)
#14 0x0000000006681897 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6681897)
#15 0x0000000006655e60 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6655e60)
#16 0x000000000667b2e1 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667b2e1)
#17 0x000000000667d8e1 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667d8e1)
#18 0x000000000667f70f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667f70f)
#19 0x0000000006682620 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6682620)
#20 0x0000000006655e60 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6655e60)
#21 0x000000000665c47f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665c47f)
#22 0x000000000665c9b9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665c9b9)
#23 0x000000000670d7aa clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670d7aa)
#24 0x000000000670e49d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670e49d)
#25 0x00000000067161c3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67161c3)
#26 0x000000000671698a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671698a)
#27 0x000000000661dbb3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661dbb3)
#28 0x000000000665423d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665423d)
#29 0x00000000066116ee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66116ee)
#30 0x0000000006611ea9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6611ea9)
#31 0x00000000066197ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66197ca)
#32 0x000000000661a76d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661a76d)
#33 0x000000000660cb4a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660cb4a)
#34 0x000000000492f768 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492f768)
#35 0x0000000004c254a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c254a5)
#36 0x0000000004ba434e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba434e)
#37 0x0000000004d191b9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d191b9)
#38 0x0000000000da8bdf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8bdf)
#39 0x0000000000d9edaa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x0000000004996369 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000003ecdb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ecdb94)
#42 0x000000000499697f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x0000000004958d9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4958d9d)
#44 0x0000000004959e2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4959e2e)
#45 0x0000000004961c85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4961c85)
#46 0x0000000000da4b48 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4b48)
#47 0x0000000000c2a384 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2a384)
#48 0x00007f3b3b429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x00007f3b3b429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000d9e855 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e855)
```

## Program

```
#include <iostream>
int main() {
  struct alignas(16) S {
  } b;
  struct alignas(32) M {
    struct alignas(1) S s1;
    struct alignas(2) S s2;
  } c;
  return 0;
}
```

## Reproduce

Since clang 13 until trunk

https://gcc.godbolt.org/z/G86bhM3qK

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
Clang crashes with an internal compiler error when a struct contains multiple subobjects of the same type but each is annotated with a different alignas specification.

## Assertion

```
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::CXXRecordDecl; From = clang::DeclContext]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:7:25: current parser token 's2'
2.	&lt;source&gt;:2:12: parsing function body 'main'
3.	&lt;source&gt;:2:12: in compound statement ('{}')
4.	&lt;source&gt;:5:3: parsing struct/union/class body 'M'
 #<!-- -->0 0x0000000003f8ac78 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8ac78)
 #<!-- -->1 0x0000000003f88904 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f88904)
 #<!-- -->2 0x0000000003ecd6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f3b3b442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f3b3b4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007f3b3b442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007f3b3b4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007f3b3b42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007f3b3b439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000067fbbb5 FindDeclaringClass(clang::NamedDecl*) SemaAccess.cpp:0:0
#<!-- -->10 0x00000000067feb1c (anonymous namespace)::AccessTarget::initialize() SemaAccess.cpp:0:0
#<!-- -->11 0x0000000006805cbf clang::Sema::CheckLookupAccess(clang::LookupResult const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6805cbf)
#<!-- -->12 0x000000000681bd52 clang::LookupResult::~LookupResult() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x681bd52)
#<!-- -->13 0x00000000069b3249 clang::Sema::ActOnTag(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::IdentifierInfo*, clang::SourceLocation, clang::ParsedAttributesView const&amp;, clang::AccessSpecifier, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;, bool&amp;, clang::SourceLocation, bool, clang::ActionResult&lt;clang::OpaquePtr&lt;clang::QualType&gt;, false&gt;, bool, bool, clang::OffsetOfKind, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69b3249)
#<!-- -->14 0x0000000006681897 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6681897)
#<!-- -->15 0x0000000006655e60 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6655e60)
#<!-- -->16 0x000000000667b2e1 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667b2e1)
#<!-- -->17 0x000000000667d8e1 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667d8e1)
#<!-- -->18 0x000000000667f70f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667f70f)
#<!-- -->19 0x0000000006682620 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6682620)
#<!-- -->20 0x0000000006655e60 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6655e60)
#<!-- -->21 0x000000000665c47f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665c47f)
#<!-- -->22 0x000000000665c9b9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665c9b9)
#<!-- -->23 0x000000000670d7aa clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670d7aa)
#<!-- -->24 0x000000000670e49d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670e49d)
#<!-- -->25 0x00000000067161c3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67161c3)
#<!-- -->26 0x000000000671698a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671698a)
#<!-- -->27 0x000000000661dbb3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661dbb3)
#<!-- -->28 0x000000000665423d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665423d)
#<!-- -->29 0x00000000066116ee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66116ee)
#<!-- -->30 0x0000000006611ea9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6611ea9)
#<!-- -->31 0x00000000066197ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66197ca)
#<!-- -->32 0x000000000661a76d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661a76d)
#<!-- -->33 0x000000000660cb4a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660cb4a)
#<!-- -->34 0x000000000492f768 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492f768)
#<!-- -->35 0x0000000004c254a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c254a5)
#<!-- -->36 0x0000000004ba434e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba434e)
#<!-- -->37 0x0000000004d191b9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d191b9)
#<!-- -->38 0x0000000000da8bdf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8bdf)
#<!-- -->39 0x0000000000d9edaa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->40 0x0000000004996369 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x0000000003ecdb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ecdb94)
#<!-- -->42 0x000000000499697f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x0000000004958d9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4958d9d)
#<!-- -->44 0x0000000004959e2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4959e2e)
#<!-- -->45 0x0000000004961c85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4961c85)
#<!-- -->46 0x0000000000da4b48 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4b48)
#<!-- -->47 0x0000000000c2a384 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2a384)
#<!-- -->48 0x00007f3b3b429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x00007f3b3b429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000d9e855 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e855)
```

## Program

```
#include &lt;iostream&gt;
int main() {
  struct alignas(16) S {
  } b;
  struct alignas(32) M {
    struct alignas(1) S s1;
    struct alignas(2) S s2;
  } c;
  return 0;
}
```

## Reproduce

Since clang 13 until trunk

https://gcc.godbolt.org/z/G86bhM3qK
</details>


---

