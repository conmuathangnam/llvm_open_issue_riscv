# Clang Crash on Parsing Constructor with Lambda and Template Name Resolution since clang 11

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/139387

## Body

## Assertion

No asserion given

## Stack dump

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:11:1: current parser token '}'
2.	<source>:7:1: parsing struct/union/class body 'B1'
 #0 0x0000000003f8ac78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8ac78)
 #1 0x0000000003f88904 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f88904)
 #2 0x0000000003ecd6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000077b8db042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000007c611b8 clang::TemplateName::getKind() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c611b8)
 #5 0x00000000074f6be6 clang::ASTContext::getNameForTemplate(clang::TemplateName, clang::SourceLocation) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74f6be6)
 #6 0x0000000006985075 clang::Sema::GetNameFromUnqualifiedId(clang::UnqualifiedId const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6985075)
 #7 0x000000000698553e clang::Sema::GetNameForDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698553e)
 #8 0x0000000006a83f20 clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Expr*, clang::VirtSpecifiers const&, clang::InClassInitStyle) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a83f20)
 #9 0x000000000667bfc0 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667bfc0)
#10 0x000000000667d8e1 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667d8e1)
#11 0x000000000667f70f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667f70f)
#12 0x0000000006682620 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6682620)
#13 0x0000000006655e60 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6655e60)
#14 0x0000000006611444 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6611444)
#15 0x0000000006611ea9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6611ea9)
#16 0x00000000066197ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66197ca)
#17 0x000000000661a76d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661a76d)
#18 0x000000000660cb4a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660cb4a)
#19 0x000000000492f768 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492f768)
#20 0x0000000004c254a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c254a5)
#21 0x0000000004ba434e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba434e)
#22 0x0000000004d191b9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d191b9)
#23 0x0000000000da8bdf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8bdf)
#24 0x0000000000d9edaa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x0000000004996369 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x0000000003ecdb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ecdb94)
#27 0x000000000499697f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x0000000004958d9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4958d9d)
#29 0x0000000004959e2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4959e2e)
#30 0x0000000004961c85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4961c85)
#31 0x0000000000da4b48 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4b48)
#32 0x0000000000c2a384 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2a384)
#33 0x000077b8db029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x000077b8db029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x0000000000d9e855 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e855)
```

## Program

```
#include <iostream>
class A0
{};
class A1 : public A0
{
};
class B1 : public A1
{
public:
    B1(delegate<A0*(void*)> mem_alloc) : A1([mem_alloc](void **ptr, delegate<void(void*)> del) { *pt r = mem_alloc(del);})
}
```

## To reproduce

Since 17, until trunk

https://gcc.godbolt.org/z/obvhvnnfj

## Comments

### Comment 1 - frederick-vs-ja

It's curious that when `A1(/*...*/)` is moved to a new line, the crash disappears.

Also, the crash seems to starts from Clang 11 ([Godbolt link](https://gcc.godbolt.org/z/T899zT8GW)).

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Assertion

No asserion given

## Stack dump

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:11:1: current parser token '}'
2.	&lt;source&gt;:7:1: parsing struct/union/class body 'B1'
 #<!-- -->0 0x0000000003f8ac78 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f8ac78)
 #<!-- -->1 0x0000000003f88904 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f88904)
 #<!-- -->2 0x0000000003ecd6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000077b8db042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000007c611b8 clang::TemplateName::getKind() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c611b8)
 #<!-- -->5 0x00000000074f6be6 clang::ASTContext::getNameForTemplate(clang::TemplateName, clang::SourceLocation) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74f6be6)
 #<!-- -->6 0x0000000006985075 clang::Sema::GetNameFromUnqualifiedId(clang::UnqualifiedId const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6985075)
 #<!-- -->7 0x000000000698553e clang::Sema::GetNameForDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698553e)
 #<!-- -->8 0x0000000006a83f20 clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::Expr*, clang::VirtSpecifiers const&amp;, clang::InClassInitStyle) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a83f20)
 #<!-- -->9 0x000000000667bfc0 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667bfc0)
#<!-- -->10 0x000000000667d8e1 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667d8e1)
#<!-- -->11 0x000000000667f70f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667f70f)
#<!-- -->12 0x0000000006682620 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6682620)
#<!-- -->13 0x0000000006655e60 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6655e60)
#<!-- -->14 0x0000000006611444 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6611444)
#<!-- -->15 0x0000000006611ea9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6611ea9)
#<!-- -->16 0x00000000066197ca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66197ca)
#<!-- -->17 0x000000000661a76d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661a76d)
#<!-- -->18 0x000000000660cb4a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660cb4a)
#<!-- -->19 0x000000000492f768 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x492f768)
#<!-- -->20 0x0000000004c254a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c254a5)
#<!-- -->21 0x0000000004ba434e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ba434e)
#<!-- -->22 0x0000000004d191b9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d191b9)
#<!-- -->23 0x0000000000da8bdf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8bdf)
#<!-- -->24 0x0000000000d9edaa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->25 0x0000000004996369 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->26 0x0000000003ecdb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ecdb94)
#<!-- -->27 0x000000000499697f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->28 0x0000000004958d9d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4958d9d)
#<!-- -->29 0x0000000004959e2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4959e2e)
#<!-- -->30 0x0000000004961c85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4961c85)
#<!-- -->31 0x0000000000da4b48 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda4b48)
#<!-- -->32 0x0000000000c2a384 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2a384)
#<!-- -->33 0x000077b8db029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->34 0x000077b8db029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->35 0x0000000000d9e855 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e855)
```

## Program

```
#include &lt;iostream&gt;
class A0
{};
class A1 : public A0
{
};
class B1 : public A1
{
public:
    B1(delegate&lt;A0*(void*)&gt; mem_alloc) : A1([mem_alloc](void **ptr, delegate&lt;void(void*)&gt; del) { *pt r = mem_alloc(del);})
}
```

## To reproduce

Since 17, until trunk

https://gcc.godbolt.org/z/obvhvnnfj
</details>


---

