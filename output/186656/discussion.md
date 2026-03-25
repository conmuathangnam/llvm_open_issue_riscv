# [clang] Assertion `PArg->isInvalidDecl() && "Unexpected NonDeducedMismatch"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/186656

## Body

This seems a recent regression. (reproducible with trunk, but non-reproducible with clang-22)

Reproducer:
https://godbolt.org/z/YMPYPTEx3
```cpp
template <template <int> typename> struct S;
template <int, int = (foo<void, void>())> struct T;
template <typename...> struct U;

using V = U<S<T>>;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateDeduction.cpp:6616: bool clang::Sema::isTemplateTemplateParameterAtLeastAsSpecializedAs(clang::TemplateParameterList*, clang::TemplateDecl*, clang::TemplateDecl*, const clang::DefaultArguments&, clang::SourceLocation, bool, bool*): Assertion `PArg->isInvalidDecl() && "Unexpected NonDeducedMismatch"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:5:13: at annotation token
 #0 0x0000000004443e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443e38)
 #1 0x0000000004440c64 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4440c64)
 #2 0x0000000004441284 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4441284)
 #3 0x00000000043805a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x00007c2913042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007c29130969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007c2913042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007c29130287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007c291302871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007c2913039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x00000000076cb6e2 clang::Sema::isTemplateTemplateParameterAtLeastAsSpecializedAs(clang::TemplateParameterList*, clang::TemplateDecl*, clang::TemplateDecl*, clang::DefaultArguments const&, clang::SourceLocation, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76cb6e2)
#11 0x0000000007641fb6 clang::Sema::CheckTemplateTemplateArgument(clang::TemplateTemplateParmDecl*, clang::TemplateParameterList*, clang::TemplateArgumentLoc&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7641fb6)
#12 0x000000000764bed1 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x764bed1)
#13 0x00000000076541d4 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::TemplateParameterList*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76541d4)
#14 0x000000000765d887 clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x765d887)
#15 0x0000000007660dd1 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7660dd1)
#16 0x0000000006d352c2 clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&, clang::ImplicitTypenameContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d352c2)
#17 0x0000000006c22710 clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c22710)
#18 0x0000000006c25859 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c25859)
#19 0x0000000006d47384 clang::Parser::isCXXDeclarationSpecifier(clang::ImplicitTypenameContext, clang::Parser::TPResult, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d47384)
#20 0x0000000006d4b2c7 clang::Parser::isCXXTypeId(clang::TentativeCXXTypeIdContext, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4b2c7)
#21 0x0000000006d35ef1 clang::Parser::ParseTemplateArgument() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d35ef1)
#22 0x0000000006d3ee15 clang::Parser::ParseTemplateArgumentList(llvm::SmallVector<clang::ParsedTemplateArgument, 16u>&, clang::OpaquePtr<clang::TemplateName>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d3ee15)
#23 0x0000000006d3f277 clang::Parser::ParseTemplateIdAfterTemplateName(bool, clang::SourceLocation&, llvm::SmallVector<clang::ParsedTemplateArgument, 16u>&, clang::SourceLocation&, clang::OpaquePtr<clang::TemplateName>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d3f277)
#24 0x0000000006d3f447 clang::Parser::AnnotateTemplateIdToken(clang::OpaquePtr<clang::TemplateName>, clang::TemplateNameKind, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d3f447)
#25 0x0000000006cb63b2 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&, clang::OpaquePtr<clang::QualType>, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb63b2)
#26 0x0000000006c2338c clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2338c)
#27 0x0000000006c6a4e0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6a4e0)
#28 0x0000000006c6b945 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6b945)
#29 0x0000000006c727a5 clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c727a5)
#30 0x0000000006c81114 clang::Parser::ParseAliasDeclarationAfterDeclarator(clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::Parser::UsingDeclarator&, clang::SourceLocation&, clang::AccessSpecifier, clang::ParsedAttributes&, clang::Decl**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c81114)
#31 0x0000000006c897ba clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c897ba)
#32 0x0000000006c8a365 clang::Parser::ParseUsingDirectiveOrDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c8a365)
#33 0x0000000006c6f313 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6f313)
#34 0x0000000006c2ba46 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2ba46)
#35 0x0000000006c2c955 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2c955)
#36 0x0000000006c0e01a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0e01a)
#37 0x0000000004f1e888 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1e888)
#38 0x0000000005228445 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5228445)
#39 0x00000000051a6b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51a6b8e)
#40 0x0000000005324dbd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5324dbd)
#41 0x0000000000de5fc2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5fc2)
#42 0x0000000000ddc8fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#43 0x0000000000ddca7d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#44 0x0000000004f94a49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#45 0x00000000043809e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43809e4)
#46 0x0000000004f95076 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#47 0x0000000004f54222 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f54222)
#48 0x0000000004f551ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f551ce)
#49 0x0000000004f5cf15 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5cf15)
#50 0x0000000000de2324 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2324)
#51 0x0000000000c996ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc996ba)
#52 0x00007c2913029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#53 0x00007c2913029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#54 0x0000000000ddc395 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc395)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
This seems a recent regression. (reproducible with trunk, but non-reproducible with clang-22)

Reproducer:
https://godbolt.org/z/YMPYPTEx3
```cpp
template &lt;template &lt;int&gt; typename&gt; struct S;
template &lt;int, int = (foo&lt;void, void&gt;())&gt; struct T;
template &lt;typename...&gt; struct U;

using V = U&lt;S&lt;T&gt;&gt;;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateDeduction.cpp:6616: bool clang::Sema::isTemplateTemplateParameterAtLeastAsSpecializedAs(clang::TemplateParameterList*, clang::TemplateDecl*, clang::TemplateDecl*, const clang::DefaultArguments&amp;, clang::SourceLocation, bool, bool*): Assertion `PArg-&gt;isInvalidDecl() &amp;&amp; "Unexpected NonDeducedMismatch"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:5:13: at annotation token
 #<!-- -->0 0x0000000004443e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443e38)
 #<!-- -->1 0x0000000004440c64 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4440c64)
 #<!-- -->2 0x0000000004441284 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4441284)
 #<!-- -->3 0x00000000043805a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x00007c2913042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007c29130969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007c2913042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007c29130287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007c291302871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007c2913039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x00000000076cb6e2 clang::Sema::isTemplateTemplateParameterAtLeastAsSpecializedAs(clang::TemplateParameterList*, clang::TemplateDecl*, clang::TemplateDecl*, clang::DefaultArguments const&amp;, clang::SourceLocation, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76cb6e2)
#<!-- -->11 0x0000000007641fb6 clang::Sema::CheckTemplateTemplateArgument(clang::TemplateTemplateParmDecl*, clang::TemplateParameterList*, clang::TemplateArgumentLoc&amp;, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7641fb6)
#<!-- -->12 0x000000000764bed1 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&amp;, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&amp;, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x764bed1)
#<!-- -->13 0x00000000076541d4 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::TemplateParameterList*, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::DefaultArguments const&amp;, bool, clang::Sema::CheckTemplateArgumentInfo&amp;, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76541d4)
#<!-- -->14 0x000000000765d887 clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x765d887)
#<!-- -->15 0x0000000007660dd1 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::ElaboratedTypeKeyword, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::OpaquePtr&lt;clang::TemplateName&gt;, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::ParsedTemplateArgument&gt;, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7660dd1)
#<!-- -->16 0x0000000006d352c2 clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&amp;, clang::ImplicitTypenameContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d352c2)
#<!-- -->17 0x0000000006c22710 clang::Parser::TryAnnotateTypeOrScopeTokenAfterScopeSpec(clang::CXXScopeSpec&amp;, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c22710)
#<!-- -->18 0x0000000006c25859 clang::Parser::TryAnnotateTypeOrScopeToken(clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c25859)
#<!-- -->19 0x0000000006d47384 clang::Parser::isCXXDeclarationSpecifier(clang::ImplicitTypenameContext, clang::Parser::TPResult, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d47384)
#<!-- -->20 0x0000000006d4b2c7 clang::Parser::isCXXTypeId(clang::TentativeCXXTypeIdContext, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4b2c7)
#<!-- -->21 0x0000000006d35ef1 clang::Parser::ParseTemplateArgument() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d35ef1)
#<!-- -->22 0x0000000006d3ee15 clang::Parser::ParseTemplateArgumentList(llvm::SmallVector&lt;clang::ParsedTemplateArgument, 16u&gt;&amp;, clang::OpaquePtr&lt;clang::TemplateName&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d3ee15)
#<!-- -->23 0x0000000006d3f277 clang::Parser::ParseTemplateIdAfterTemplateName(bool, clang::SourceLocation&amp;, llvm::SmallVector&lt;clang::ParsedTemplateArgument, 16u&gt;&amp;, clang::SourceLocation&amp;, clang::OpaquePtr&lt;clang::TemplateName&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d3f277)
#<!-- -->24 0x0000000006d3f447 clang::Parser::AnnotateTemplateIdToken(clang::OpaquePtr&lt;clang::TemplateName&gt;, clang::TemplateNameKind, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d3f447)
#<!-- -->25 0x0000000006cb63b2 clang::Parser::ParseOptionalCXXScopeSpecifier(clang::CXXScopeSpec&amp;, clang::OpaquePtr&lt;clang::QualType&gt;, bool, bool, bool*, bool, clang::IdentifierInfo const**, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cb63b2)
#<!-- -->26 0x0000000006c2338c clang::Parser::TryAnnotateCXXScopeToken(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2338c)
#<!-- -->27 0x0000000006c6a4e0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6a4e0)
#<!-- -->28 0x0000000006c6b945 clang::Parser::ParseSpecifierQualifierList(clang::DeclSpec&amp;, clang::ImplicitTypenameContext, clang::AccessSpecifier, clang::Parser::DeclSpecContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6b945)
#<!-- -->29 0x0000000006c727a5 clang::Parser::ParseTypeName(clang::SourceRange*, clang::DeclaratorContext, clang::AccessSpecifier, clang::Decl**, clang::ParsedAttributes*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c727a5)
#<!-- -->30 0x0000000006c81114 clang::Parser::ParseAliasDeclarationAfterDeclarator(clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation, clang::Parser::UsingDeclarator&amp;, clang::SourceLocation&amp;, clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Decl**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c81114)
#<!-- -->31 0x0000000006c897ba clang::Parser::ParseUsingDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c897ba)
#<!-- -->32 0x0000000006c8a365 clang::Parser::ParseUsingDirectiveOrDeclaration(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c8a365)
#<!-- -->33 0x0000000006c6f313 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6f313)
#<!-- -->34 0x0000000006c2ba46 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2ba46)
#<!-- -->35 0x0000000006c2c955 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c2c955)
#<!-- -->36 0x0000000006c0e01a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0e01a)
#<!-- -->37 0x0000000004f1e888 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f1e888)
#<!-- -->38 0x0000000005228445 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5228445)
#<!-- -->39 0x00000000051a6b8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51a6b8e)
#<!-- -->40 0x0000000005324dbd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5324dbd)
#<!-- -->41 0x0000000000de5fc2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5fc2)
#<!-- -->42 0x0000000000ddc8fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->43 0x0000000000ddca7d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->44 0x0000000004f94a49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->45 0x00000000043809e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43809e4)
#<!-- -->46 0x0000000004f95076 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->47 0x0000000004f54222 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f54222)
#<!-- -->48 0x0000000004f551ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f551ce)
#<!-- -->49 0x0000000004f5cf15 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f5cf15)
#<!-- -->50 0x0000000000de2324 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde2324)
#<!-- -->51 0x0000000000c996ba main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc996ba)
#<!-- -->52 0x00007c2913029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->53 0x00007c2913029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->54 0x0000000000ddc395 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc395)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

@Serosh-commits this bisects to 9472490520b11b855991f13e5617381df1be23fd

CC @mizvekov @erichkeane who reviewed it and had some concerns w/ it

---

