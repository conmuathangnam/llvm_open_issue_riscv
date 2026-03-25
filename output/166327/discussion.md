# crash in CheckDesignatedInitializer since clang 3.7

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/166327

## Body

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaInit.cpp:3029: bool {anonymous}::InitListChecker::CheckDesignatedInitializer(const clang::InitializedEntity&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::RecordDecl::field_iterator*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool): Assertion `StructuredList->getNumInits() == 1 && "A union should never have more than one initializer!"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 <source>
1.	<source>:15:77: current parser token ';'
2.	<source>:1:16: parsing function body 'main'
3.	<source>:1:16: in compound statement ('{}')
 #0 0x0000000004238d78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4238d78)
 #1 0x00000000042361a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42361a4)
 #2 0x000000000417a418 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b615b442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b615b4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b615b442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b615b4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b615b42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b615b439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000700203f (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) SemaInit.cpp:0:0
#10 0x0000000007000841 (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) SemaInit.cpp:0:0
#11 0x0000000007000841 (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>*, llvm::APSInt*, unsigned int&, clang::InitListExpr*, unsigned int&, bool, bool) SemaInit.cpp:0:0
#12 0x00000000070027d1 (anonymous namespace)::InitListChecker::CheckStructUnionTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType, llvm::iterator_range<clang::CXXBaseSpecifier const*>, clang::DeclContext::specific_decl_iterator<clang::FieldDecl>, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) SemaInit.cpp:0:0
#13 0x0000000006ffb8fa (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) (.constprop.0) SemaInit.cpp:0:0
#14 0x0000000006fea8ea (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#15 0x0000000006feb347 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
#16 0x0000000006ff49e2 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ff49e2)
#17 0x0000000006d25b4c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d25b4c)
#18 0x000000000697145a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697145a)
#19 0x000000000697e77a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697e77a)
#20 0x00000000069874af clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69874af)
#21 0x000000000698797c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698797c)
#22 0x0000000006a38a5a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a38a5a)
#23 0x0000000006a392b9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a392b9)
#24 0x0000000006a41a46 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a41a46)
#25 0x0000000006a4225a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4225a)
#26 0x0000000006948dbb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6948dbb)
#27 0x000000000697f18d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697f18d)
#28 0x000000000693c56e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693c56e)
#29 0x000000000693cd0f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693cd0f)
#30 0x00000000069445c3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69445c3)
#31 0x00000000069454d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69454d5)
#32 0x0000000006945980 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6945980)
#33 0x0000000006926b23 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6926b23)
#34 0x0000000004b84408 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84408)
#35 0x0000000004e72255 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72255)
#36 0x0000000004df319e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df319e)
#37 0x0000000004f6a9cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6a9cd)
#38 0x0000000000dcd1b0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcd1b0)
#39 0x0000000000dc3d2a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#40 0x0000000000dc3ead int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#41 0x0000000004bedcb9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x000000000417a8b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a8b4)
#43 0x0000000004bee2cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x0000000004baf022 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf022)
#45 0x0000000004baffce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baffce)
#46 0x0000000004bb7725 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb7725)
#47 0x0000000000dc96d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc96d1)
#48 0x0000000000c79094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79094)
#49 0x00007b615b429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x00007b615b429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000dc37c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc37c5)
```

## To quickly reproduce

https://godbolt.org/z/n9Ee8WW3j

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaInit.cpp:3029: bool {anonymous}::InitListChecker::CheckDesignatedInitializer(const clang::InitializedEntity&amp;, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&amp;, clang::RecordDecl::field_iterator*, llvm::APSInt*, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool, bool): Assertion `StructuredList-&gt;getNumInits() == 1 &amp;&amp; "A union should never have more than one initializer!"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 &lt;source&gt;
1.	&lt;source&gt;:15:77: current parser token ';'
2.	&lt;source&gt;:1:16: parsing function body 'main'
3.	&lt;source&gt;:1:16: in compound statement ('{}')
 #<!-- -->0 0x0000000004238d78 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4238d78)
 #<!-- -->1 0x00000000042361a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42361a4)
 #<!-- -->2 0x000000000417a418 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b615b442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b615b4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b615b442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b615b4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b615b42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b615b439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000700203f (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&amp;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;*, llvm::APSInt*, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool, bool) SemaInit.cpp:0:0
#<!-- -->10 0x0000000007000841 (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&amp;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;*, llvm::APSInt*, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool, bool) SemaInit.cpp:0:0
#<!-- -->11 0x0000000007000841 (anonymous namespace)::InitListChecker::CheckDesignatedInitializer(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::DesignatedInitExpr*, unsigned int, clang::QualType&amp;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;*, llvm::APSInt*, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool, bool) SemaInit.cpp:0:0
#<!-- -->12 0x00000000070027d1 (anonymous namespace)::InitListChecker::CheckStructUnionTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType, llvm::iterator_range&lt;clang::CXXBaseSpecifier const*&gt;, clang::DeclContext::specific_decl_iterator&lt;clang::FieldDecl&gt;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) SemaInit.cpp:0:0
#<!-- -->13 0x0000000006ffb8fa (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->14 0x0000000006fea8ea (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#<!-- -->15 0x0000000006feb347 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) SemaInit.cpp:0:0
#<!-- -->16 0x0000000006ff49e2 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ff49e2)
#<!-- -->17 0x0000000006d25b4c clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d25b4c)
#<!-- -->18 0x000000000697145a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697145a)
#<!-- -->19 0x000000000697e77a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697e77a)
#<!-- -->20 0x00000000069874af clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69874af)
#<!-- -->21 0x000000000698797c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698797c)
#<!-- -->22 0x0000000006a38a5a clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a38a5a)
#<!-- -->23 0x0000000006a392b9 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a392b9)
#<!-- -->24 0x0000000006a41a46 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a41a46)
#<!-- -->25 0x0000000006a4225a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4225a)
#<!-- -->26 0x0000000006948dbb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6948dbb)
#<!-- -->27 0x000000000697f18d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x697f18d)
#<!-- -->28 0x000000000693c56e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693c56e)
#<!-- -->29 0x000000000693cd0f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x693cd0f)
#<!-- -->30 0x00000000069445c3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69445c3)
#<!-- -->31 0x00000000069454d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69454d5)
#<!-- -->32 0x0000000006945980 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6945980)
#<!-- -->33 0x0000000006926b23 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6926b23)
#<!-- -->34 0x0000000004b84408 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84408)
#<!-- -->35 0x0000000004e72255 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e72255)
#<!-- -->36 0x0000000004df319e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df319e)
#<!-- -->37 0x0000000004f6a9cd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6a9cd)
#<!-- -->38 0x0000000000dcd1b0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcd1b0)
#<!-- -->39 0x0000000000dc3d2a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->40 0x0000000000dc3ead int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->41 0x0000000004bedcb9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x000000000417a8b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417a8b4)
#<!-- -->43 0x0000000004bee2cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x0000000004baf022 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baf022)
#<!-- -->45 0x0000000004baffce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4baffce)
#<!-- -->46 0x0000000004bb7725 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb7725)
#<!-- -->47 0x0000000000dc96d1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc96d1)
#<!-- -->48 0x0000000000c79094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc79094)
#<!-- -->49 0x00007b615b429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->50 0x00007b615b429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->51 0x0000000000dc37c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc37c5)
```

## To quickly reproduce

https://godbolt.org/z/n9Ee8WW3j
</details>


---

### Comment 2 - frederick-vs-ja

I guess this can be categorized as https://github.com/llvm/llvm-project/labels/crash-on-valid as we seem to accept C-style designators in C++ modes as an extension.

In C23 mode Clang correctly accepts it, [Godbolt link](https://godbolt.org/z/8PYebqszx).

---

### Comment 3 - naveen-seth

I managed to get this to behave correctly, but my solution is kinda hacky and I am not able to find a fix to the real underlying issue, which is why I am posting it here in case it helps someone with more knowledge find a better fix.

```diff
diff --git a/clang/lib/Sema/SemaInit.cpp b/clang/lib/Sema/SemaInit.cpp
index 073010d16b42..43587197689d 100644
--- a/clang/lib/Sema/SemaInit.cpp
+++ b/clang/lib/Sema/SemaInit.cpp
@@ -3024,14 +3024,16 @@ InitListChecker::CheckDesignatedInitializer(const InitializedEntity &Entity,
     if (RD->isUnion()) {
       FieldIndex = 0;
       if (StructuredList) {
         FieldDecl *CurrentField = StructuredList->getInitializedFieldInUnion();
         if (CurrentField && !declaresSameEntity(CurrentField, *Field)) {
-          assert(StructuredList->getNumInits() == 1
-                 && "A union should never have more than one initializer!");
+          const auto NumInits = StructuredList->getNumInits();
 
-          Expr *ExistingInit = StructuredList->getInit(0);
+          assert(StructuredList->getNumInits() <= 1 &&
+                 "A union should never have more than one initializer!");
+
+          Expr *ExistingInit = NumInits ? StructuredList->getInit(0) : nullptr;
           if (ExistingInit) {
             // We're about to throw away an initializer, emit warning.
             diagnoseInitOverride(
                 ExistingInit, SourceRange(D->getBeginLoc(), DIE->getEndLoc()),
                 /*UnionOverride=*/true,
```

The crash occurs when handling the designator `.p.extra.label = 'p'` (I believe). At that point, `StructuredList->getInitializedFieldInUnion()` returns a non-null pointer even though `StructuredList->getNumInits()` is 0.

However, when using C23 mode, `StructuredList->getInitializedFieldInUnion()` returns a null pointer, so we never enter the branch containing the assertion.

That `StructuredList->getInitializedFieldInUnion()` returns a non-null pointer while `StructuredList->getNumInits()` is `0` doesn't seem like a valid state to me, and the proper fix would probably be make the C++ behavior match the C behavior in this case.

If this is an acceptable fix, please let me know and I will make a PR. Otherwise, I hope this helps!

---

