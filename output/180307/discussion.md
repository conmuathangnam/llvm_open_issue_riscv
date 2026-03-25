# [clang] Assertion `Converted.size() == 2 && "__type_pack_element should be given an index and a parameter pack"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/180307
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, regression:16
**Closed Date:** 2026-03-01T21:38:53Z

## Body

Reproducer:
https://godbolt.org/z/j9TzssW9G
```cpp
namespace std {
typedef decltype(sizeof(0)) size_t;
}

template <std::size_t... Seq> void f(__type_pack_element<Seq...>) {}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplate.cpp:3465: clang::QualType checkBuiltinTemplateIdType(clang::Sema&, clang::ElaboratedTypeKeyword, clang::BuiltinTemplateDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::SourceLocation, clang::TemplateArgumentListInfo&): Assertion `Converted.size() == 2 && "__type_pack_element should be given an index and a parameter pack"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:38: at annotation token
 #0 0x000000000435ba08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x435ba08)
 #1 0x0000000004358e64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4358e64)
 #2 0x00000000042991d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070445d242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070445d2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070445d242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070445d2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070445d22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070445d239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000751f857 clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x751f857)
#10 0x00000000075207e1 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75207e1)
#11 0x0000000006c0c84c clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&, clang::ImplicitTypenameContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0c84c)
#12 0x0000000006af93f0 clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6af93f0)
#13 0x0000000006afc529 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afc529)
#14 0x0000000006c1e9da clang::Parser::isCXXDeclarationSpecifier(clang::ImplicitTypenameContext, clang::Parser::TPResult, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1e9da)
#15 0x0000000006c21315 clang::Parser::TryParseParameterDeclarationClause(bool*, bool, clang::ImplicitTypenameContext) (.part.0) ParseTentative.cpp:0:0
#16 0x0000000006c21912 clang::Parser::isCXXFunctionDeclarator(bool*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c21912)
#17 0x0000000006b4c9b1 clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4c9b1)
#18 0x0000000006b39eaf clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b39eaf)
#19 0x000000000860a5f1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x860a5f1)
#20 0x0000000006b3cb5f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3cb5f)
#21 0x0000000006c0dea8 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0dea8)
#22 0x0000000006c1b399 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#23 0x0000000006c1b7aa clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1b7aa)
#24 0x0000000006b46650 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b46650)
#25 0x0000000006b030f6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b030f6)
#26 0x0000000006b03fe5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b03fe5)
#27 0x0000000006ae4cfa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae4cfa)
#28 0x0000000004cd2d18 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cd2d18)
#29 0x0000000004fcfef5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fcfef5)
#30 0x0000000004f4f72e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f4f72e)
#31 0x00000000050cb2cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50cb2cd)
#32 0x0000000000de429e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde429e)
#33 0x0000000000ddac6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x0000000000ddaded int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#35 0x0000000004d46929 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000004299674 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4299674)
#37 0x0000000004d46f3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x0000000004d07732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d07732)
#39 0x0000000004d086de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d086de)
#40 0x0000000004d0fb25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d0fb25)
#41 0x0000000000de0611 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde0611)
#42 0x0000000000c9b9c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9b9c4)
#43 0x000070445d229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x000070445d229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000dda705 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdda705)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/j9TzssW9G
```cpp
namespace std {
typedef decltype(sizeof(0)) size_t;
}

template &lt;std::size_t... Seq&gt; void f(__type_pack_element&lt;Seq...&gt;) {}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplate.cpp:3465: clang::QualType checkBuiltinTemplateIdType(clang::Sema&amp;, clang::ElaboratedTypeKeyword, clang::BuiltinTemplateDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;): Assertion `Converted.size() == 2 &amp;&amp; "__type_pack_element should be given an index and a parameter pack"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:5:38: at annotation token
 #<!-- -->0 0x000000000435ba08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x435ba08)
 #<!-- -->1 0x0000000004358e64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4358e64)
 #<!-- -->2 0x00000000042991d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070445d242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070445d2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070445d242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070445d2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070445d22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070445d239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000751f857 clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x751f857)
#<!-- -->10 0x00000000075207e1 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::OpaquePtr&lt;clang::TemplateName&gt;, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::ParsedTemplateArgument&gt;, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75207e1)
#<!-- -->11 0x0000000006c0c84c clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&amp;, clang::ImplicitTypenameContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0c84c)
#<!-- -->12 0x0000000006af93f0 clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&amp;, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6af93f0)
#<!-- -->13 0x0000000006afc529 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afc529)
#<!-- -->14 0x0000000006c1e9da clang::Parser::isCXXDeclarationSpecifier(clang::ImplicitTypenameContext, clang::Parser::TPResult, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1e9da)
#<!-- -->15 0x0000000006c21315 clang::Parser::TryParseParameterDeclarationClause(bool*, bool, clang::ImplicitTypenameContext) (.part.0) ParseTentative.cpp:0:0
#<!-- -->16 0x0000000006c21912 clang::Parser::isCXXFunctionDeclarator(bool*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c21912)
#<!-- -->17 0x0000000006b4c9b1 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b4c9b1)
#<!-- -->18 0x0000000006b39eaf clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b39eaf)
#<!-- -->19 0x000000000860a5f1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x860a5f1)
#<!-- -->20 0x0000000006b3cb5f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3cb5f)
#<!-- -->21 0x0000000006c0dea8 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0dea8)
#<!-- -->22 0x0000000006c1b399 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->23 0x0000000006c1b7aa clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1b7aa)
#<!-- -->24 0x0000000006b46650 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b46650)
#<!-- -->25 0x0000000006b030f6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b030f6)
#<!-- -->26 0x0000000006b03fe5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b03fe5)
#<!-- -->27 0x0000000006ae4cfa clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ae4cfa)
#<!-- -->28 0x0000000004cd2d18 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cd2d18)
#<!-- -->29 0x0000000004fcfef5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fcfef5)
#<!-- -->30 0x0000000004f4f72e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f4f72e)
#<!-- -->31 0x00000000050cb2cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50cb2cd)
#<!-- -->32 0x0000000000de429e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde429e)
#<!-- -->33 0x0000000000ddac6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->34 0x0000000000ddaded int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004d46929 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000004299674 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4299674)
#<!-- -->37 0x0000000004d46f3f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x0000000004d07732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d07732)
#<!-- -->39 0x0000000004d086de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d086de)
#<!-- -->40 0x0000000004d0fb25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d0fb25)
#<!-- -->41 0x0000000000de0611 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde0611)
#<!-- -->42 0x0000000000c9b9c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9b9c4)
#<!-- -->43 0x000070445d229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x000070445d229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000dda705 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdda705)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-16:
https://godbolt.org/z/zhno8xjeP

---

### Comment 3 - shafik

@mizvekov this bisects to f4ea3bd4b2086e6de10131b197aaf7d066a24df8

---

