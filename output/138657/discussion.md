# Clang crash (ICE) on partial specialization with template<int N, int... Args> for template<auto V> since clang 18

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/138657
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-05-06T20:51:06Z

## Body

Clang crashes with an internal compiler error (ICE) when attempting to partially specialize a class template template<auto V> using a function-style syntax like template<int N, int... Args> class meta<N(Args...)>. The syntax appears invalid but instead of producing a diagnostic, Clang segfaults inside CallExpr::getCallReturnType. This occurs at compile time, even without instantiation.

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Type.h:8839: const T* clang::Type::castAs() const [with T = clang::FunctionType]: Assertion `isa<T>(CanonicalType)' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<eof> parser at end of file
 #0 0x0000000003f70b58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f70b58)
 #1 0x0000000003f6e7e4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6e7e4)
 #2 0x0000000003eb36a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fa969642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fa9696969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007fa969642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007fa9696287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007fa96962871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007fa969639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000782b06f clang::CallExpr::getCallReturnType(clang::ASTContext const&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x782b06f)
#10 0x000000000783a92b ClassifyInternal(clang::ASTContext&, clang::Expr const*) ExprClassification.cpp:0:0
#11 0x000000000783bb44 clang::Expr::ClassifyImpl(clang::ASTContext&, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x783bb44)
#12 0x000000000711017b clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x711017b)
#13 0x0000000007026319 clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&, clang::TemplateArgument&, bool, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7026319)
#14 0x000000000702b501 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x702b501)
#15 0x000000000702c761 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x702c761)
#16 0x0000000007037335 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7037335)
#17 0x0000000007039062 clang::Sema::ActOnTagTemplateIdType(clang::TagUseKind, clang::TypeSpecifierType, clang::SourceLocation, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7039062)
#18 0x0000000006655613 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6655613)
#19 0x000000000662ad10 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662ad10)
#20 0x00000000066f5cfc clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f5cfc)
#21 0x000000000670389f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#22 0x0000000006703c4a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6703c4a)
#23 0x0000000006631a10 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631a10)
#24 0x00000000065ee70c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee70c)
#25 0x00000000065ef5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ef5fd)
#26 0x00000000065e19da clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e19da)
#27 0x0000000004912908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4912908)
#28 0x0000000004c05bb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c05bb5)
#29 0x0000000004b851be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b851be)
#30 0x0000000004cf9a09 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf9a09)
#31 0x0000000000da625f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda625f)
#32 0x0000000000d9c42a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x00000000049791b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x0000000003eb3b44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb3b44)
#35 0x00000000049797cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x000000000493bc1d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493bc1d)
#37 0x000000000493ccae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493ccae)
#38 0x0000000004944b05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4944b05)
#39 0x0000000000da21c8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda21c8)
#40 0x0000000000c27864 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc27864)
#41 0x00007fa969629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00007fa969629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000d9bed5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9bed5)
```

## Program

```
#include <iostream>
using namespace std;
template <auto V>
class meta {};
template<int N, int... Args>
class meta<N(Args...)> {}
```

## To reproduce

It happens in version 18, 19, 20 and current 

All versions compiled: https://gcc.godbolt.org/z/aa3PxYePK

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
Clang crashes with an internal compiler error (ICE) when attempting to partially specialize a class template template&lt;auto V&gt; using a function-style syntax like template&lt;int N, int... Args&gt; class meta&lt;N(Args...)&gt;. The syntax appears invalid but instead of producing a diagnostic, Clang segfaults inside CallExpr::getCallReturnType. This occurs at compile time, even without instantiation.

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/Type.h:8839: const T* clang::Type::castAs() const [with T = clang::FunctionType]: Assertion `isa&lt;T&gt;(CanonicalType)' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000003f70b58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f70b58)
 #<!-- -->1 0x0000000003f6e7e4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6e7e4)
 #<!-- -->2 0x0000000003eb36a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fa969642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007fa9696969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007fa969642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007fa9696287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007fa96962871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007fa969639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000782b06f clang::CallExpr::getCallReturnType(clang::ASTContext const&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x782b06f)
#<!-- -->10 0x000000000783a92b ClassifyInternal(clang::ASTContext&amp;, clang::Expr const*) ExprClassification.cpp:0:0
#<!-- -->11 0x000000000783bb44 clang::Expr::ClassifyImpl(clang::ASTContext&amp;, clang::SourceLocation*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x783bb44)
#<!-- -->12 0x000000000711017b clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x711017b)
#<!-- -->13 0x0000000007026319 clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&amp;, clang::TemplateArgument&amp;, bool, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7026319)
#<!-- -->14 0x000000000702b501 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&amp;, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&amp;, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x702b501)
#<!-- -->15 0x000000000702c761 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::DefaultArguments const&amp;, bool, clang::Sema::CheckTemplateArgumentInfo&amp;, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x702c761)
#<!-- -->16 0x0000000007037335 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7037335)
#<!-- -->17 0x0000000007039062 clang::Sema::ActOnTagTemplateIdType(clang::TagUseKind, clang::TypeSpecifierType, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::OpaquePtr&lt;clang::TemplateName&gt;, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::ParsedTemplateArgument&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7039062)
#<!-- -->18 0x0000000006655613 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6655613)
#<!-- -->19 0x000000000662ad10 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662ad10)
#<!-- -->20 0x00000000066f5cfc clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f5cfc)
#<!-- -->21 0x000000000670389f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->22 0x0000000006703c4a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6703c4a)
#<!-- -->23 0x0000000006631a10 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631a10)
#<!-- -->24 0x00000000065ee70c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee70c)
#<!-- -->25 0x00000000065ef5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ef5fd)
#<!-- -->26 0x00000000065e19da clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e19da)
#<!-- -->27 0x0000000004912908 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4912908)
#<!-- -->28 0x0000000004c05bb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c05bb5)
#<!-- -->29 0x0000000004b851be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b851be)
#<!-- -->30 0x0000000004cf9a09 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf9a09)
#<!-- -->31 0x0000000000da625f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda625f)
#<!-- -->32 0x0000000000d9c42a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x00000000049791b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x0000000003eb3b44 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb3b44)
#<!-- -->35 0x00000000049797cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x000000000493bc1d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493bc1d)
#<!-- -->37 0x000000000493ccae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493ccae)
#<!-- -->38 0x0000000004944b05 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4944b05)
#<!-- -->39 0x0000000000da21c8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda21c8)
#<!-- -->40 0x0000000000c27864 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc27864)
#<!-- -->41 0x00007fa969629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x00007fa969629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000d9bed5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9bed5)
```

## Program

```
#include &lt;iostream&gt;
using namespace std;
template &lt;auto V&gt;
class meta {};
template&lt;int N, int... Args&gt;
class meta&lt;N(Args...)&gt; {}
```

## To reproduce

It happens in version 18, 19, 20 and current 

All versions compiled: https://gcc.godbolt.org/z/aa3PxYePK
</details>


---

### Comment 2 - EugeneZelenko

@mariete1223: If you plan to file more bug reports, it'll be good idea to request triage access, so you could label issues properly yourself. See [examples](https://github.com/llvm/llvm-project/issues?q=is%3Aissue%20state%3Aclosed%20label%3Ainfra%3Acommit-access-request%20triage).

---

### Comment 3 - mariete1223

I´ll do that then, thank you for the information!

---

