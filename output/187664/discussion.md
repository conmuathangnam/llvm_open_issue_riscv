# [clang]Assertion failure in TypeLoc (ElaboratedTypeLoc/TypedefTypeLoc) on invalid default template type parameter combining two type specifiers

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/187664
**Status:** Open
**Labels:** clang:frontend, crash

## Body

## The following invalid code causes **Clang** to crash since 16.0.0:
when two **type specifiers** are combined in a default template argument:

```cpp
class A {
public:
    class B {};
};

using X = A::B;

class C {
    template <typename T = X class A::B> void f();
};
```
**https://godbolt.org/z/Mh8j156E7**

## **OUTPUT:**
```
<source>:9:30: error: cannot combine with previous 'type-name' declaration specifier
    9 |     template <typename T = X class A::B> void f();
      |                              ^
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/TypeLoc.h:746: void clang::ElaboratedNameTypeLoc<TL, T>::set(clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::SourceLocation) [with TL = clang::TypedefTypeLoc; T = clang::TypedefType]: Assertion `QualifierLoc.getNestedNameSpecifier() == this->getTypePtr()->getQualifier()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:9:40: current parser token '>'
2.	<source>:8:1: parsing struct/union/class body 'C'
 #0 0x000000000445abc8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x445abc8)
 #1 0x00000000044579f4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44579f4)
 #2 0x0000000004458014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4458014)
 #3 0x00000000043973b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x000079b404842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x000079b4048969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x000079b404842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x000079b4048287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x000079b40482871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x000079b404839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x000000000791dffb (anonymous namespace)::TypeSpecLocFiller::VisitTypedefTypeLoc(clang::TypedefTypeLoc) (.isra.0) SemaType.cpp:0:0
#11 0x0000000007943685 clang::TypeLocVisitor<(anonymous namespace)::TypeSpecLocFiller, void>::Visit(clang::UnqualTypeLoc) SemaType.cpp:0:0
#12 0x000000000794ba3b GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#13 0x000000000794c85f clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x794c85f)
#14 0x000000000794cb9e clang::Sema::ActOnTypeName(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x794cb9e)
#15 0x0000000006d0a1c3 clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d0a1c3)
#16 0x0000000006dd0535 clang::Parser::ParseTypeParameter(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd0535)
#17 0x0000000006dd76b1 clang::Parser::ParseTemplateParameter(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd76b1)
#18 0x0000000006dd824e clang::Parser::ParseTemplateParameterList(unsigned int, llvm::SmallVectorImpl<clang::NamedDecl*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd824e)
#19 0x0000000006dd8a2d clang::Parser::ParseTemplateParameters(clang::Parser::MultiParseScope&, unsigned int, llvm::SmallVectorImpl<clang::NamedDecl*>&, clang::SourceLocation&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd8a2d)
#20 0x0000000006ddb2d3 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#21 0x0000000006d26225 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d26225)
#22 0x0000000006d282af clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d282af)
#23 0x0000000006d29f8f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d29f8f)
#24 0x0000000006d2c873 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d2c873)
#25 0x0000000006cff986 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cff986)
#26 0x0000000006cbae17 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbae17)
#27 0x0000000006cbb89f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbb89f)
#28 0x0000000006cc2ec1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc2ec1)
#29 0x0000000006cc3df5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc3df5)
#30 0x0000000006ca54ba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca54ba)
#31 0x0000000004f439a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f439a8)
#32 0x000000000525e6a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x525e6a5)
#33 0x00000000051dc19e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51dc19e)
#34 0x000000000535a68d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x535a68d)
#35 0x0000000000df5092 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf5092)
#36 0x0000000000deb9ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#37 0x0000000000debb4d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#38 0x0000000004facb69 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x00000000043977f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43977f4)
#40 0x0000000004fad196 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x0000000004f6c272 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6c272)
#42 0x0000000004f6d21e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6d21e)
#43 0x0000000004f74dd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f74dd5)
#44 0x0000000000df13f4 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf13f4)
#45 0x0000000000ca8f0a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca8f0a)
#46 0x000079b404829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x000079b404829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000deb465 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdeb465)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

The crash also occurs with:

```cpp
class A {
public:
    class B {};
};

typedef A::B X;

class C {
    template <typename T = X class A::B> void f();
};
```

and:

```cpp
class A {
public:
    class B {};
};

using X = char;

class C {
    template <typename T = X class A::B> void f();
};
```

But if you remove the _**type alias**_, the same pattern no longer crashes:

```cpp
class A {
public:
    class B {};
};

class C {
    template <typename T = A::B class A::B> void f();
};
```

This code does not crash either:
```cpp
class A {
public:
    class B {};
};

class C {
    template <typename T = A::B class A> void f();
};
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
## The following invalid code causes **Clang** to crash since 16.0.0:
when two **type specifiers** are combined in a default template argument:

```cpp
class A {
public:
    class B {};
};

using X = A::B;

class C {
    template &lt;typename T = X class A::B&gt; void f();
};
```
**https://godbolt.org/z/Mh8j156E7**

## **OUTPUT:**
```
&lt;source&gt;:9:30: error: cannot combine with previous 'type-name' declaration specifier
    9 |     template &lt;typename T = X class A::B&gt; void f();
      |                              ^
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/TypeLoc.h:746: void clang::ElaboratedNameTypeLoc&lt;TL, T&gt;::set(clang::SourceLocation, clang::NestedNameSpecifierLoc, clang::SourceLocation) [with TL = clang::TypedefTypeLoc; T = clang::TypedefType]: Assertion `QualifierLoc.getNestedNameSpecifier() == this-&gt;getTypePtr()-&gt;getQualifier()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:9:40: current parser token '&gt;'
2.	&lt;source&gt;:8:1: parsing struct/union/class body 'C'
 #<!-- -->0 0x000000000445abc8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x445abc8)
 #<!-- -->1 0x00000000044579f4 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44579f4)
 #<!-- -->2 0x0000000004458014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4458014)
 #<!-- -->3 0x00000000043973b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x000079b404842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x000079b4048969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x000079b404842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x000079b4048287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x000079b40482871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x000079b404839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x000000000791dffb (anonymous namespace)::TypeSpecLocFiller::VisitTypedefTypeLoc(clang::TypedefTypeLoc) (.isra.0) SemaType.cpp:0:0
#<!-- -->11 0x0000000007943685 clang::TypeLocVisitor&lt;(anonymous namespace)::TypeSpecLocFiller, void&gt;::Visit(clang::UnqualTypeLoc) SemaType.cpp:0:0
#<!-- -->12 0x000000000794ba3b GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&amp;, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#<!-- -->13 0x000000000794c85f clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x794c85f)
#<!-- -->14 0x000000000794cb9e clang::Sema::ActOnTypeName(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x794cb9e)
#<!-- -->15 0x0000000006d0a1c3 clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d0a1c3)
#<!-- -->16 0x0000000006dd0535 clang::Parser::ParseTypeParameter(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd0535)
#<!-- -->17 0x0000000006dd76b1 clang::Parser::ParseTemplateParameter(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd76b1)
#<!-- -->18 0x0000000006dd824e clang::Parser::ParseTemplateParameterList(unsigned int, llvm::SmallVectorImpl&lt;clang::NamedDecl*&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd824e)
#<!-- -->19 0x0000000006dd8a2d clang::Parser::ParseTemplateParameters(clang::Parser::MultiParseScope&amp;, unsigned int, llvm::SmallVectorImpl&lt;clang::NamedDecl*&gt;&amp;, clang::SourceLocation&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dd8a2d)
#<!-- -->20 0x0000000006ddb2d3 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->21 0x0000000006d26225 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d26225)
#<!-- -->22 0x0000000006d282af clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d282af)
#<!-- -->23 0x0000000006d29f8f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d29f8f)
#<!-- -->24 0x0000000006d2c873 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d2c873)
#<!-- -->25 0x0000000006cff986 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cff986)
#<!-- -->26 0x0000000006cbae17 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbae17)
#<!-- -->27 0x0000000006cbb89f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cbb89f)
#<!-- -->28 0x0000000006cc2ec1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc2ec1)
#<!-- -->29 0x0000000006cc3df5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc3df5)
#<!-- -->30 0x0000000006ca54ba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca54ba)
#<!-- -->31 0x0000000004f439a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f439a8)
#<!-- -->32 0x000000000525e6a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x525e6a5)
#<!-- -->33 0x00000000051dc19e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51dc19e)
#<!-- -->34 0x000000000535a68d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x535a68d)
#<!-- -->35 0x0000000000df5092 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf5092)
#<!-- -->36 0x0000000000deb9ca ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->37 0x0000000000debb4d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->38 0x0000000004facb69 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->39 0x00000000043977f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43977f4)
#<!-- -->40 0x0000000004fad196 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->41 0x0000000004f6c272 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6c272)
#<!-- -->42 0x0000000004f6d21e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6d21e)
#<!-- -->43 0x0000000004f74dd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f74dd5)
#<!-- -->44 0x0000000000df13f4 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdf13f4)
#<!-- -->45 0x0000000000ca8f0a main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca8f0a)
#<!-- -->46 0x000079b404829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->47 0x000079b404829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->48 0x0000000000deb465 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdeb465)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

The crash also occurs with:

```cpp
class A {
public:
    class B {};
};

typedef A::B X;

class C {
    template &lt;typename T = X class A::B&gt; void f();
};
```

and:

```cpp
class A {
public:
    class B {};
};

using X = char;

class C {
    template &lt;typename T = X class A::B&gt; void f();
};
```

But if you remove the _**type alias**_, the same pattern no longer crashes:

```cpp
class A {
public:
    class B {};
};

class C {
    template &lt;typename T = A::B class A::B&gt; void f();
};
```

This code does not crash either:
```cpp
class A {
public:
    class B {};
};

class C {
    template &lt;typename T = A::B class A&gt; void f();
};
```
</details>


---

### Comment 2 - TPPPP72

Well, this crash was caused by AST repeatedly processing tokens that already had an associated TST. I will submit a pull request to fix this issue.
https://github.com/llvm/llvm-project/blob/24546d96445a6d4d7f6209795975faa7e3310356/clang/lib/Parse/ParseDecl.cpp#L4453-L4459
We need to add an extra check between lines 4458 and 4459.

---

### Comment 3 - Attacker23

> Well, this crash was caused by AST repeatedly processing tokens that already had an associated TST. I will submit a pull request to fix this issue.
> https://github.com/llvm/llvm-project/blob/24546d96445a6d4d7f6209795975faa7e3310356/clang/lib/Parse/ParseDecl.cpp#L4453-L4459
> We need to add an extra check between lines 4458 and 4459.

Thank you, Misaka🥰🥰🥰

---

### Comment 4 - TPPPP72

I believe my current solution has some issues, causing other regression tests to fail. Therefore, I'm temporarily closing the pull request until I can come up with a patch that doesn't produce side effects.

---

