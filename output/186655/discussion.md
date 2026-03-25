# [clang] crashes when using __type_pack_element<sizeof(T)> in a partial specialization

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/186655

## Body

Reproducer:
https://godbolt.org/z/5vKTKjYc6
```cpp
template <class {}, class C> struct S;
template <Foo T> struct S<T, __type_pack_element<sizeof(T)>> {};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = clang::Expr*; From = llvm::PointerUnion<clang::TemplateArgumentLocInfo::TemplateTemplateArgLocInfo*, clang::Expr*, clang::TypeSourceInfo*>]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:2:62: current parser token '{'
 #0 0x0000000004443e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443e38)
 #1 0x0000000004440c64 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4440c64)
 #2 0x0000000004441284 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4441284)
 #3 0x00000000043805a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x00007a05ca842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007a05ca8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007a05ca842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007a05ca8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007a05ca82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007a05ca839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x000000000848237f clang::TemplateArgumentLoc::getSourceRange() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848237f)
#11 0x0000000007629dca clang::TemplateArgumentLoc::getLocation() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7629dca)
#12 0x000000000765f188 clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x765f188)
#13 0x00000000077abe95 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000778afc5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000778f657 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000779749c (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000007798e87 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#18 0x00000000077993c7 clang::Sema::SubstTemplateArguments(llvm::ArrayRef<clang::TemplateArgumentLoc>, clang::MultiLevelTemplateArgumentList const&, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77993c7)
#19 0x00000000076be69b FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgumentLoc>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) SemaTemplateDeduction.cpp:0:0
#20 0x00000000076bfa78 FinishTemplateArgumentDeduction(clang::Sema&, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, bool) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#21 0x00000000076bfce8 void llvm::function_ref<void ()>::callback_fn<bool isAtLeastAsSpecializedAs<clang::ClassTemplatePartialSpecializationDecl>(clang::Sema&, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::TemplateDecl*, clang::sema::TemplateDeductionInfo&)::'lambda'()>(long) SemaTemplateDeduction.cpp:0:0
#22 0x000000000877c9c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x877c9c1)
#23 0x00000000076cbf6c bool isAtLeastAsSpecializedAs<clang::ClassTemplatePartialSpecializationDecl>(clang::Sema&, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::TemplateDecl*, clang::sema::TemplateDeductionInfo&) SemaTemplateDeduction.cpp:0:0
#24 0x00000000076ccc81 clang::ClassTemplatePartialSpecializationDecl* getMoreSpecialized<clang::ClassTemplatePartialSpecializationDecl, clang::ClassTemplateDecl>(clang::Sema&, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::ClassTemplateDecl*, clang::sema::TemplateDeductionInfo&) SemaTemplateDeduction.cpp:0:0
#25 0x00000000076cd0fa clang::Sema::isMoreSpecializedThanPrimary(clang::ClassTemplatePartialSpecializationDecl*, clang::sema::TemplateDeductionInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76cd0fa)
#26 0x00000000076410cd void checkTemplatePartialSpecialization<clang::ClassTemplatePartialSpecializationDecl>(clang::Sema&, clang::ClassTemplatePartialSpecializationDecl*) SemaTemplate.cpp:0:0
#27 0x00000000076878d8 clang::Sema::ActOnClassTemplateSpecialization(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&, clang::TemplateIdAnnotation&, clang::ParsedAttributesView const&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76878d8)
#28 0x0000000006c93f7d clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c93f7d)
#29 0x0000000006c684e6 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c684e6)
#30 0x0000000006d368d9 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d368d9)
#31 0x0000000006d43e49 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#32 0x0000000006d4425a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4425a)
#33 0x0000000006c6f290 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6f290)
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
#52 0x00007a05ca829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#53 0x00007a05ca829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#54 0x0000000000ddc395 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc395)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/5vKTKjYc6
```cpp
template &lt;class {}, class C&gt; struct S;
template &lt;Foo T&gt; struct S&lt;T, __type_pack_element&lt;sizeof(T)&gt;&gt; {};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = clang::Expr*; From = llvm::PointerUnion&lt;clang::TemplateArgumentLocInfo::TemplateTemplateArgLocInfo*, clang::Expr*, clang::TypeSourceInfo*&gt;]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:2:62: current parser token '{'
 #<!-- -->0 0x0000000004443e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4443e38)
 #<!-- -->1 0x0000000004440c64 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4440c64)
 #<!-- -->2 0x0000000004441284 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4441284)
 #<!-- -->3 0x00000000043805a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x00007a05ca842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007a05ca8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007a05ca842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007a05ca8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007a05ca82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007a05ca839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x000000000848237f clang::TemplateArgumentLoc::getSourceRange() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x848237f)
#<!-- -->11 0x0000000007629dca clang::TemplateArgumentLoc::getLocation() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7629dca)
#<!-- -->12 0x000000000765f188 clang::Sema::CheckTemplateIdType(clang::ElaboratedTypeKeyword, clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::Scope*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x765f188)
#<!-- -->13 0x00000000077abe95 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::QualType, clang::NamedDecl*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x000000000778afc5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x000000000778f657 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x000000000779749c (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x0000000007798e87 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x00000000077993c7 clang::Sema::SubstTemplateArguments(llvm::ArrayRef&lt;clang::TemplateArgumentLoc&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateArgumentListInfo&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77993c7)
#<!-- -->19 0x00000000076be69b FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgumentLoc&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) SemaTemplateDeduction.cpp:0:0
#<!-- -->20 0x00000000076bfa78 FinishTemplateArgumentDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::TemplateParameterList*, clang::TemplateDecl*, bool, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->21 0x00000000076bfce8 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;bool isAtLeastAsSpecializedAs&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(clang::Sema&amp;, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::TemplateDecl*, clang::sema::TemplateDeductionInfo&amp;)::'lambda'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->22 0x000000000877c9c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x877c9c1)
#<!-- -->23 0x00000000076cbf6c bool isAtLeastAsSpecializedAs&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(clang::Sema&amp;, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::TemplateDecl*, clang::sema::TemplateDeductionInfo&amp;) SemaTemplateDeduction.cpp:0:0
#<!-- -->24 0x00000000076ccc81 clang::ClassTemplatePartialSpecializationDecl* getMoreSpecialized&lt;clang::ClassTemplatePartialSpecializationDecl, clang::ClassTemplateDecl&gt;(clang::Sema&amp;, clang::QualType, clang::QualType, clang::ClassTemplatePartialSpecializationDecl*, clang::ClassTemplateDecl*, clang::sema::TemplateDeductionInfo&amp;) SemaTemplateDeduction.cpp:0:0
#<!-- -->25 0x00000000076cd0fa clang::Sema::isMoreSpecializedThanPrimary(clang::ClassTemplatePartialSpecializationDecl*, clang::sema::TemplateDeductionInfo&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76cd0fa)
#<!-- -->26 0x00000000076410cd void checkTemplatePartialSpecialization&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(clang::Sema&amp;, clang::ClassTemplatePartialSpecializationDecl*) SemaTemplate.cpp:0:0
#<!-- -->27 0x00000000076878d8 clang::Sema::ActOnClassTemplateSpecialization(clang::Scope*, unsigned int, clang::TagUseKind, clang::SourceLocation, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::TemplateIdAnnotation&amp;, clang::ParsedAttributesView const&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76878d8)
#<!-- -->28 0x0000000006c93f7d clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c93f7d)
#<!-- -->29 0x0000000006c684e6 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c684e6)
#<!-- -->30 0x0000000006d368d9 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d368d9)
#<!-- -->31 0x0000000006d43e49 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->32 0x0000000006d4425a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d4425a)
#<!-- -->33 0x0000000006c6f290 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6f290)
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
#<!-- -->52 0x00007a05ca829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->53 0x00007a05ca829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->54 0x0000000000ddc395 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc395)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21:
https://godbolt.org/z/48v3hqPTx

---

### Comment 3 - shafik

@mizvekov this bisects to 3954d258a5d20c418718bb2f655665e02e6a7475

---

