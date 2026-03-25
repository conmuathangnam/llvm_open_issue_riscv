# [clang] Assertion `!Qualifier' still fails

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/181470
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed, regression:22
**Closed Date:** 2026-02-18T22:07:53Z

## Body

Related to: https://github.com/llvm/llvm-project/issues/167120

Reproducer: https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00120.cpp
https://godbolt.org/z/395xvbava
```cpp
#include <foo>

namespace N {}
bar;

template <class T> N::T::bar;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:5174: clang::QualType clang::ASTContext::getTypeDeclType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier, const clang::TypeDecl*) const: Assertion `!Qualifier' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:6:26: current parser token 'bar'
 #0 0x00000000043cc858 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43cc858)
 #1 0x00000000043c9cb4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43c9cb4)
 #2 0x0000000004309f38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007438bf642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007438bf6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007438bf642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007438bf6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007438bf62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007438bf639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007af11ed clang::ASTContext::getTypeDeclType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier, clang::TypeDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7af11ed)
#10 0x0000000006d87300 ExtendNestedNameSpecifier(clang::Sema&, clang::CXXScopeSpec&, clang::NamedDecl const*, clang::SourceLocation, clang::SourceLocation) (.isra.0) SemaCXXScopeSpec.cpp:0:0
#11 0x0000000006d90207 clang::Sema::BuildCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&, bool, clang::CXXScopeSpec&, clang::NamedDecl*, bool, bool*, bool) (.part.0) SemaCXXScopeSpec.cpp:0:0
#12 0x0000000006d91ab3 clang::Sema::ActOnCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&, bool, clang::CXXScopeSpec&, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d91ab3)
#13 0x0000000006c121ef clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c121ef)
#14 0x0000000006b7e93c clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b7e93c)
#15 0x0000000006bc581f clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc581f)
#16 0x0000000006c917c1 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c917c1)
#17 0x0000000006c9ed19 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#18 0x0000000006c9f12a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9f12a)
#19 0x0000000006bca600 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bca600)
#20 0x0000000006b86fe6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b86fe6)
#21 0x0000000006b87ef5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b87ef5)
#22 0x0000000006b695ca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b695ca)
#23 0x0000000004d43068 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d43068)
#24 0x00000000050408c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50408c5)
#25 0x0000000004fbfd5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fbfd5e)
#26 0x000000000513bfcd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x513bfcd)
#27 0x0000000000de90ae cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde90ae)
#28 0x0000000000ddfa7a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#29 0x0000000000ddfbfd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#30 0x0000000004db6b39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x000000000430a3d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430a3d4)
#32 0x0000000004db714f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x0000000004d77ad2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d77ad2)
#34 0x0000000004d78a7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d78a7e)
#35 0x0000000004d7fd35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7fd35)
#36 0x0000000000de5421 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5421)
#37 0x0000000000c9f384 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9f384)
#38 0x00007438bf629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007438bf629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000ddf515 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddf515)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Related to: https://github.com/llvm/llvm-project/issues/167120

Reproducer: https://github.com/k-arrows/clang-assert-tests/blob/main/cases/00120.cpp
https://godbolt.org/z/395xvbava
```cpp
#include &lt;foo&gt;

namespace N {}
bar;

template &lt;class T&gt; N::T::bar;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:5174: clang::QualType clang::ASTContext::getTypeDeclType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier, const clang::TypeDecl*) const: Assertion `!Qualifier' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:6:26: current parser token 'bar'
 #<!-- -->0 0x00000000043cc858 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43cc858)
 #<!-- -->1 0x00000000043c9cb4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43c9cb4)
 #<!-- -->2 0x0000000004309f38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007438bf642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007438bf6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007438bf642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007438bf6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007438bf62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007438bf639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007af11ed clang::ASTContext::getTypeDeclType(clang::ElaboratedTypeKeyword, clang::NestedNameSpecifier, clang::TypeDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7af11ed)
#<!-- -->10 0x0000000006d87300 ExtendNestedNameSpecifier(clang::Sema&amp;, clang::CXXScopeSpec&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::SourceLocation) (.isra.0) SemaCXXScopeSpec.cpp:0:0
#<!-- -->11 0x0000000006d90207 clang::Sema::BuildCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&amp;, bool, clang::CXXScopeSpec&amp;, clang::NamedDecl*, bool, bool*, bool) (.part.0) SemaCXXScopeSpec.cpp:0:0
#<!-- -->12 0x0000000006d91ab3 clang::Sema::ActOnCXXNestedNameSpecifier(clang::Scope*, clang::Sema::NestedNameSpecInfo&amp;, bool, clang::CXXScopeSpec&amp;, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d91ab3)
#<!-- -->13 0x0000000006c121ef clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c121ef)
#<!-- -->14 0x0000000006b7e93c clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b7e93c)
#<!-- -->15 0x0000000006bc581f clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bc581f)
#<!-- -->16 0x0000000006c917c1 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c917c1)
#<!-- -->17 0x0000000006c9ed19 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->18 0x0000000006c9f12a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c9f12a)
#<!-- -->19 0x0000000006bca600 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bca600)
#<!-- -->20 0x0000000006b86fe6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b86fe6)
#<!-- -->21 0x0000000006b87ef5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b87ef5)
#<!-- -->22 0x0000000006b695ca clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b695ca)
#<!-- -->23 0x0000000004d43068 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d43068)
#<!-- -->24 0x00000000050408c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50408c5)
#<!-- -->25 0x0000000004fbfd5e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fbfd5e)
#<!-- -->26 0x000000000513bfcd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x513bfcd)
#<!-- -->27 0x0000000000de90ae cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde90ae)
#<!-- -->28 0x0000000000ddfa7a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->29 0x0000000000ddfbfd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004db6b39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x000000000430a3d4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430a3d4)
#<!-- -->32 0x0000000004db714f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x0000000004d77ad2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d77ad2)
#<!-- -->34 0x0000000004d78a7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d78a7e)
#<!-- -->35 0x0000000004d7fd35 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d7fd35)
#<!-- -->36 0x0000000000de5421 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5421)
#<!-- -->37 0x0000000000c9f384 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9f384)
#<!-- -->38 0x00007438bf629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007438bf629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000ddf515 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddf515)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

</details>


---

### Comment 2 - k-arrows

does not crash with clang-21:
https://godbolt.org/z/81an9zohr

---

### Comment 3 - shafik

@mizvekov  this bisects to 91cdd35008e9ab32dffb7e401cdd7313b3461892

---

