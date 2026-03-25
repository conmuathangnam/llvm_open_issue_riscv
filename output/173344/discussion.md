# Clang frontend failure: "Missing argument pack" in TransformTemplateTypeParmType during template instantiation

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173344
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-12-29T06:09:25Z

## Body

### Link for quick verification: https://godbolt.org/z/E4be19Mo7
```c
auto l = []<typename T, typename... Ts>(T* p, Ts**) -> auto&& {
    return p;
}(new int[5], new int*[3]); 
```
Go back to `clang 9.0.0 (assertions)` 
Share the same assertion failure as issue https://github.com/llvm/llvm-project/issues/70522 but with different stack dump. Their source code are similar.

## Stack dump:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2450: clang::QualType {anonymous}::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool): Assertion `Arg.getKind() == TemplateArgument::Pack && "Missing argument pack"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:4:26: current parser token ')'
 #0 0x00000000042eab68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eab68)
 #1 0x00000000042e7f94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e7f94)
 #2 0x000000000422cd78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079f0dbe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000079f0dbe969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000079f0dbe42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000079f0dbe287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000079f0dbe2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000079f0dbe39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000075766d9 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000007562844 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000007566351 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformPointerType(clang::TypeLocBuilder&, clang::PointerTypeLoc) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000007563513 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007566351 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformPointerType(clang::TypeLocBuilder&, clang::PointerTypeLoc) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000007563513 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#15 0x0000000007566c57 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000756b3f1 clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#17 0x000000000757119a clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x757119a)
#18 0x0000000007571cc6 (anonymous namespace)::TemplateInstantiator::TransformFunctionTypeParam(clang::ParmVarDecl*, int, clang::UnsignedOrNone, bool) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000757f627 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000758d7e6 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x758d7e6)
#21 0x00000000075e33bf clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75e33bf)
#22 0x0000000007646d9a clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7646d9a)
#23 0x0000000007648e24 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#24 0x00000000084f33d1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f33d1)
#25 0x00000000075e317a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75e317a)
#26 0x00000000074ed538 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74ed538)
#27 0x00000000074ee7da void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#28 0x00000000084f33d1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f33d1)
#29 0x00000000074cd9e0 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74cd9e0)
#30 0x000000000731cbfd AddMethodTemplateCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#31 0x000000000731f933 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x731f933)
#32 0x000000000731fb63 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x731fb63)
#33 0x000000000733ed5f clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x733ed5f)
#34 0x0000000006f97d64 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f97d64)
#35 0x0000000006f9ccbc clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9ccbc)
#36 0x0000000006a9b187 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9b187)
#37 0x0000000006a9409a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9409a)
#38 0x0000000006a962f7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a962f7)
#39 0x0000000006a96389 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a96389)
#40 0x0000000006abe870 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abe870)
#41 0x0000000006a4f88f clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4f88f)
#42 0x0000000006a5cc5a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5cc5a)
#43 0x0000000006a1a30b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1a30b)
#44 0x0000000006a1aaff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1aaff)
#45 0x0000000006a2292a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2292a)
#46 0x0000000006a238c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a238c5)
#47 0x0000000006a23d70 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a23d70)
#48 0x0000000006a04913 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a04913)
#49 0x0000000004c4b068 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4b068)
#50 0x0000000004f41965 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41965)
#51 0x0000000004ec1bfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec1bfe)
#52 0x000000000503bced clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503bced)
#53 0x0000000000de5afc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5afc)
#54 0x0000000000ddc54a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#55 0x0000000000ddc6cd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#56 0x0000000004cbd9b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#57 0x000000000422d214 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422d214)
#58 0x0000000004cbdfcf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#59 0x0000000004c7eab2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7eab2)
#60 0x0000000004c7fa5e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7fa5e)
#61 0x0000000004c86eb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c86eb5)
#62 0x0000000000de1ef1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1ef1)
#63 0x0000000000c8cf44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf44)
#64 0x000079f0dbe29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#65 0x000079f0dbe29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#66 0x0000000000ddbfe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddbfe5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/E4be19Mo7
```c
auto l = []&lt;typename T, typename... Ts&gt;(T* p, Ts**) -&gt; auto&amp;&amp; {
    return p;
}(new int[5], new int*[3]); 
```
Go back to `clang 9.0.0 (assertions)` 
Share the same assertion failure as issue https://github.com/llvm/llvm-project/issues/70522 but with different stack dump. Their source code are similar.

## Stack dump:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2450: clang::QualType {anonymous}::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::TemplateTypeParmTypeLoc, bool): Assertion `Arg.getKind() == TemplateArgument::Pack &amp;&amp; "Missing argument pack"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:4:26: current parser token ')'
 #<!-- -->0 0x00000000042eab68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eab68)
 #<!-- -->1 0x00000000042e7f94 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42e7f94)
 #<!-- -->2 0x000000000422cd78 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079f0dbe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000079f0dbe969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000079f0dbe42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000079f0dbe287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000079f0dbe2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000079f0dbe39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000075766d9 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x0000000007562844 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000007566351 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformPointerType(clang::TypeLocBuilder&amp;, clang::PointerTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x0000000007563513 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000007566351 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformPointerType(clang::TypeLocBuilder&amp;, clang::PointerTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x0000000007563513 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x0000000007566c57 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x000000000756b3f1 clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x000000000757119a clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x757119a)
#<!-- -->18 0x0000000007571cc6 (anonymous namespace)::TemplateInstantiator::TransformFunctionTypeParam(clang::ParmVarDecl*, int, clang::UnsignedOrNone, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x000000000757f627 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef&lt;clang::ParmVarDecl*&gt;, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl&lt;clang::QualType&gt;&amp;, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;*, clang::Sema::ExtParameterInfoBuilder&amp;, unsigned int*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x000000000758d7e6 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x758d7e6)
#<!-- -->21 0x00000000075e33bf clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75e33bf)
#<!-- -->22 0x0000000007646d9a clang::TemplateDeclInstantiator::VisitCXXMethodDecl(clang::CXXMethodDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7646d9a)
#<!-- -->23 0x0000000007648e24 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->24 0x00000000084f33d1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f33d1)
#<!-- -->25 0x00000000075e317a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75e317a)
#<!-- -->26 0x00000000074ed538 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74ed538)
#<!-- -->27 0x00000000074ee7da void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->28 0x00000000084f33d1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x84f33d1)
#<!-- -->29 0x00000000074cd9e0 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74cd9e0)
#<!-- -->30 0x000000000731cbfd AddMethodTemplateCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#<!-- -->31 0x000000000731f933 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x731f933)
#<!-- -->32 0x000000000731fb63 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x731fb63)
#<!-- -->33 0x000000000733ed5f clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x733ed5f)
#<!-- -->34 0x0000000006f97d64 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f97d64)
#<!-- -->35 0x0000000006f9ccbc clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9ccbc)
#<!-- -->36 0x0000000006a9b187 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9b187)
#<!-- -->37 0x0000000006a9409a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9409a)
#<!-- -->38 0x0000000006a962f7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a962f7)
#<!-- -->39 0x0000000006a96389 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a96389)
#<!-- -->40 0x0000000006abe870 clang::Parser::ParseInitializer(clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6abe870)
#<!-- -->41 0x0000000006a4f88f clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4f88f)
#<!-- -->42 0x0000000006a5cc5a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5cc5a)
#<!-- -->43 0x0000000006a1a30b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1a30b)
#<!-- -->44 0x0000000006a1aaff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1aaff)
#<!-- -->45 0x0000000006a2292a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2292a)
#<!-- -->46 0x0000000006a238c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a238c5)
#<!-- -->47 0x0000000006a23d70 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a23d70)
#<!-- -->48 0x0000000006a04913 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a04913)
#<!-- -->49 0x0000000004c4b068 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4b068)
#<!-- -->50 0x0000000004f41965 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f41965)
#<!-- -->51 0x0000000004ec1bfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec1bfe)
#<!-- -->52 0x000000000503bced clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503bced)
#<!-- -->53 0x0000000000de5afc cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5afc)
#<!-- -->54 0x0000000000ddc54a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->55 0x0000000000ddc6cd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->56 0x0000000004cbd9b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->57 0x000000000422d214 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422d214)
#<!-- -->58 0x0000000004cbdfcf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->59 0x0000000004c7eab2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7eab2)
#<!-- -->60 0x0000000004c7fa5e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c7fa5e)
#<!-- -->61 0x0000000004c86eb5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c86eb5)
#<!-- -->62 0x0000000000de1ef1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1ef1)
#<!-- -->63 0x0000000000c8cf44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cf44)
#<!-- -->64 0x000079f0dbe29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->65 0x000079f0dbe29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->66 0x0000000000ddbfe5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddbfe5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

