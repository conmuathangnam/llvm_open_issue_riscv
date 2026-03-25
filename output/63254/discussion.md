# Clang frontend C++ crash when using declaration refers to its own class c++20 and above

**Author:** brutalsavage
**URL:** https://github.com/llvm/llvm-project/issues/63254
**Status:** Closed
**Labels:** c++20, clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-07-05T06:30:29Z

## Body

To quickly reproduce: https://gcc.godbolt.org/z/Y9r3sYhYc (assertion-trunk)

```cpp
#include <variant>
using namespace std;
struct test: variant <int> {
    using test::variant; // using refers to its own class, removing this line leads to correct compilation.
    void mutated_v1(variant <int> v);
};
int main() {}
```

Compiling the above code crashes clang `clang++ -x c++ -std=c++20`, crashes locally using clang-17.0 (a10019a), also on trunk with assertion (see godbolt link)

Note `--std=c++17` does not crash.

## Comments

### Comment 1 - brutalsavage

Assertion: 
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplate.cpp:291: 
clang::TemplateNameKind clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, const clang::UnqualifiedId&, clang::ParsedType, bool, clang::Sema::TemplateTy&, bool&, bool): 
Assertion `!FoundUsingShadow || FoundUsingShadow->getTargetDecl() == TD' failed.
```

Backtrace: 
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 <source>
1.	<source>:5:21: current parser token 'variant'
2.	<source>:3:1: parsing struct/union/class body 'test'
 #0 0x0000561cee124d0f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c20d0f)
 #1 0x0000561cee122a7c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c1ea7c)
 #2 0x0000561cee06cdc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fc8b6baa420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fc8b667700b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fc8b6656859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007fc8b6656729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007fc8b6667fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000561cf123e93f (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d3a93f)
 #9 0x0000561cf08f1ca4 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo**, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63edca4)
#10 0x0000561cf0870bc3 clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x636cbc3)
#11 0x0000561cf08a619c clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63a219c)
#12 0x0000561cf08ad064 clang::Parser::ParseParameterDeclarationClause(clang::DeclaratorContext, clang::ParsedAttributes&, llvm::SmallVectorImpl<clang::DeclaratorChunk::ParamInfo>&, clang::SourceLocation&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63a9064)
#13 0x0000561cf08ae3d8 clang::Parser::ParseFunctionDeclarator(clang::Declarator&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63aa3d8)
#14 0x0000561cf08b21ff clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63ae1ff)
#15 0x0000561cf08a0f78 clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x639cf78)
#16 0x0000561cf09c6765 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c2765)
#17 0x0000561cf0891753 clang::Parser::ParseDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x638d753)
#18 0x0000561cf08c57b5 clang::Parser::ParseCXXMemberDeclaratorBeforeInitializer(clang::Declarator&, clang::VirtSpecifiers&, clang::ActionResult<clang::Expr*, true>&, clang::Parser::LateParsedAttrList&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c17b5)
#19 0x0000561cf08c719d clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c319d)
#20 0x0000561cf08c97e5 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c57e5)
#21 0x0000561cf08cb1fd clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c71fd)
#22 0x0000561cf08cd3e8 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c93e8)
#23 0x0000561cf08a60cd clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63a20cd)
#24 0x0000561cf08711bb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x636d1bb)
#25 0x0000561cf0871aef clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (.part.0) Parser.cpp:0:0
#26 0x0000561cf0878491 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6374491)
#27 0x0000561cf0878e06 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6374e06)
#28 0x0000561cf086cbca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6368bca)
#29 0x0000561cef3751c8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e711c8)
#30 0x0000561ceebc7929 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46c3929)
#31 0x0000561ceeb4c826 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4648826)
#32 0x0000561ceeca9bb6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47a5bb6)
#33 0x0000561ceb5d748d cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10d348d)
#34 0x0000561ceb5d318a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x0000561cee9b535d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000561cee06d2d0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b692d0)
#37 0x0000561cee9b597f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x0000561cee97d15c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x447915c)
#39 0x0000561cee97dbed clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4479bed)
#40 0x0000561cee985ccd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4481ccd)
#41 0x0000561ceb5d56ea clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10d16ea)
#42 0x0000561ceb4dc5e5 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xfd85e5)
#43 0x00007fc8b6658083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#44 0x0000561ceb5cdf6e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10c9f6e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
ASM generation compiler returned: 134
```

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - Endilll

Confirmed on Clang 19.0 (trunk) in C++20 mode: https://gcc.godbolt.org/z/4e963j38n
Reduced by me and C-Reduce:
```cpp
template <typename...>
struct variant {};

struct test : variant<> {
  using test::variant;
  variant<> v;
};
```
Clang 19 crash:
```
<source>:5:9: error: using declaration refers to its own class
    5 |   using test::variant;
      |         ^~~~~~

clang++: /root/llvm-project/clang/lib/Sema/SemaTemplate.cpp:294:
clang::TemplateNameKind clang::Sema::isTemplateName(clang::Scope*, clang::CXXScopeSpec&, bool, const clang::UnqualifiedId&, clang::ParsedType, bool, clang::Sema::TemplateTy&, bool&, bool):
Assertion `!FoundUsingShadow || FoundUsingShadow->getTargetDecl() == TD' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++20 <source>
1.	<source>:6:3: current parser token 'variant'
2.	<source>:4:1: parsing struct/union/class body 'test'
 #0 0x0000000003979978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3979978)
 #1 0x000000000397765c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x397765c)
 #2 0x00000000038c9018 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007cc705842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007cc7058969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007cc705842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007cc7058287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007cc70582871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007cc705839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006ced72d (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ced72d)
#10 0x000000000631ab31 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x631ab31)
#11 0x0000000006289343 clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6289343)
#12 0x00000000062f2d7e clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62f2d7e)
#13 0x00000000062f5265 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62f5265)
#14 0x00000000062f6e37 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62f6e37)
#15 0x00000000062f9050 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62f9050)
#16 0x00000000062cfd95 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62cfd95)
#17 0x0000000006289907 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6289907)
#18 0x000000000628a303 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x628a303)
#19 0x0000000006290907 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6290907)
#20 0x00000000062917bf clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62917bf)
#21 0x000000000628527a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x628527a)
#22 0x0000000004246e08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4246e08)
#23 0x00000000044c8199 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44c8199)
#24 0x000000000445025e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x445025e)
#25 0x00000000045aef3e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45aef3e)
#26 0x0000000000c4fdec cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4fdec)
#27 0x0000000000c4907a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#28 0x000000000428a649 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x00000000038c94c4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x38c94c4)
#30 0x000000000428ac3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x0000000004250fd5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4250fd5)
#32 0x0000000004251a3d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4251a3d)
#33 0x00000000042597c5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42597c5)
#34 0x0000000000c4d285 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4d285)
#35 0x0000000000b2f354 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb2f354)
#36 0x00007cc705829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x00007cc705829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000c48b6e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc48b6e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-c-20

Author: Steven Xia (brutalsavage)

<details>
To quickly reproduce: https://gcc.godbolt.org/z/Y9r3sYhYc (assertion-trunk)

```cpp
#include &lt;variant&gt;
using namespace std;
struct test: variant &lt;int&gt; {
    using test::variant; // using refers to its own class, removing this line leads to correct compilation.
    void mutated_v1(variant &lt;int&gt; v);
};
int main() {}
```

Compiling the above code crashes clang `clang++ -x c++ -std=c++20`, crashes locally using clang-17.0 (a10019a), also on trunk with assertion (see godbolt link)

Note `--std=c++17` does not crash.
</details>


---

