# Incorrect using of `final` triggers a clang frontend crash

**Author:** Rush10233
**URL:** https://github.com/llvm/llvm-project/issues/120939

## Body

Consider the following invalid code:

```c++
template<class T, class = decltype(T())>
char f(int);

struct A { 
    virtual ~A() = 0 final;
};

auto a = f<A>(0);
```

The `final` specifier here erroneously appears after the pure virtual destructor declaration, which causes a clang frontend command failure with the following output:

```c++
<source>:5:13: error: initializer on function does not look like a pure-specifier
    5 |     virtual ~A() = 0 final;
      |             ^      ~
<source>:5:21: error: expected ';' at end of declaration list
    5 |     virtual ~A() = 0 final;
      |                     ^
      |                     ;
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:8:16: current parser token ')'
 #0 0x0000000003a2b6e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a2b6e8)
 #1 0x0000000003a29834 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a29834)
 #2 0x0000000003979e38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a51ac842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000679b663 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x679b663)
 #5 0x0000000006840a81 clang::Sema::ActOnDecltypeExpression(clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6840a81)
 #6 0x0000000006d9414b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #7 0x0000000006d9764b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #8 0x0000000006dad5c1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #9 0x0000000006dadf42 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&, clang::MultiLevelTemplateArgumentList const&, clang::TemplateArgumentLoc&, clang::SourceLocation, clang::DeclarationName const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6dadf42)
#10 0x0000000006c0cf7e clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, bool&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c0cf7e)
#11 0x0000000006cf764b clang::TemplateDeductionResult ConvertDeducedTemplateArguments<clang::FunctionTemplateDecl>(clang::Sema&, clang::FunctionTemplateDecl*, bool, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::TemplateArgument>&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::LocalInstantiationScope*, unsigned int, bool*) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#12 0x0000000006d16ffe clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#13 0x0000000006d1800a void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#14 0x0000000007947f91 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7947f91)
#15 0x0000000006d0d6e7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d0d6e7)
#16 0x0000000006b67d66 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#17 0x0000000006b69ffc clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b69ffc)
#18 0x0000000006b6a2ae clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b6a2ae)
#19 0x0000000006b6c12e clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b6c12e)
#20 0x00000000067bb5b8 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67bb5b8)
#21 0x00000000067bfc0c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67bfc0c)
#22 0x00000000062ef1c7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ef1c7)
#23 0x00000000062e6f21 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e6f21)
#24 0x00000000062e8c35 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e8c35)
#25 0x00000000062e915a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e915a)
#26 0x00000000062e9329 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e9329)
#27 0x00000000062a1bb8 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a1bb8)
#28 0x00000000062b780c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b780c)
#29 0x00000000062662c7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62662c7)
#30 0x00000000062670de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62670de)
#31 0x00000000062709c2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62709c2)
#32 0x0000000006272258 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6272258)
#33 0x000000000626092a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626092a)
#34 0x0000000004367615 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4367615)
#35 0x0000000004633561 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4633561)
#36 0x00000000045b34ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b34ab)
#37 0x000000000471d593 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x471d593)
#38 0x0000000000d012ca cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd012ca)
#39 0x0000000000cf9b1d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x00000000043b0589 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x000000000397a263 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x397a263)
#42 0x00000000043b07a9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x000000000437722d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x437722d)
#44 0x000000000437822d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x437822d)
#45 0x0000000004381ecc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4381ecc)
#46 0x0000000000cfdf71 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xcfdf71)
#47 0x0000000000bc5e54 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xbc5e54)
#48 0x00007a51ac829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x00007a51ac829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000cf95b5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xcf95b5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
Please check [https://godbolt.org/z/53j77M1EG](https://godbolt.org/z/53j77M1EG)



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Rush10233)

<details>
Consider the following invalid code:

```c++
template&lt;class T, class = decltype(T())&gt;
char f(int);

struct A { 
    virtual ~A() = 0 final;
};

auto a = f&lt;A&gt;(0);
```

The `final` specifier here erroneously appears after the pure virtual destructor declaration, which causes a clang frontend command failure with the following output:

```c++
&lt;source&gt;:5:13: error: initializer on function does not look like a pure-specifier
    5 |     virtual ~A() = 0 final;
      |             ^      ~
&lt;source&gt;:5:21: error: expected ';' at end of declaration list
    5 |     virtual ~A() = 0 final;
      |                     ^
      |                     ;
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:8:16: current parser token ')'
 #<!-- -->0 0x0000000003a2b6e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a2b6e8)
 #<!-- -->1 0x0000000003a29834 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a29834)
 #<!-- -->2 0x0000000003979e38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a51ac842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000000000679b663 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x679b663)
 #<!-- -->5 0x0000000006840a81 clang::Sema::ActOnDecltypeExpression(clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6840a81)
 #<!-- -->6 0x0000000006d9414b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->7 0x0000000006d9764b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->8 0x0000000006dad5c1 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->9 0x0000000006dadf42 clang::Sema::SubstTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateArgumentLoc&amp;, clang::SourceLocation, clang::DeclarationName const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6dadf42)
#<!-- -->10 0x0000000006c0cf7e clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c0cf7e)
#<!-- -->11 0x0000000006cf764b clang::TemplateDeductionResult ConvertDeducedTemplateArguments&lt;clang::FunctionTemplateDecl&gt;(clang::Sema&amp;, clang::FunctionTemplateDecl*, bool, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::LocalInstantiationScope*, unsigned int, bool*) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->12 0x0000000006d16ffe clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->13 0x0000000006d1800a void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->14 0x0000000007947f91 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7947f91)
#<!-- -->15 0x0000000006d0d6e7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d0d6e7)
#<!-- -->16 0x0000000006b67d66 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#<!-- -->17 0x0000000006b69ffc clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b69ffc)
#<!-- -->18 0x0000000006b6a2ae clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b6a2ae)
#<!-- -->19 0x0000000006b6c12e clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b6c12e)
#<!-- -->20 0x00000000067bb5b8 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67bb5b8)
#<!-- -->21 0x00000000067bfc0c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67bfc0c)
#<!-- -->22 0x00000000062ef1c7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ef1c7)
#<!-- -->23 0x00000000062e6f21 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e6f21)
#<!-- -->24 0x00000000062e8c35 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e8c35)
#<!-- -->25 0x00000000062e915a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e915a)
#<!-- -->26 0x00000000062e9329 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e9329)
#<!-- -->27 0x00000000062a1bb8 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62a1bb8)
#<!-- -->28 0x00000000062b780c clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b780c)
#<!-- -->29 0x00000000062662c7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62662c7)
#<!-- -->30 0x00000000062670de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62670de)
#<!-- -->31 0x00000000062709c2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62709c2)
#<!-- -->32 0x0000000006272258 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6272258)
#<!-- -->33 0x000000000626092a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626092a)
#<!-- -->34 0x0000000004367615 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4367615)
#<!-- -->35 0x0000000004633561 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4633561)
#<!-- -->36 0x00000000045b34ab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b34ab)
#<!-- -->37 0x000000000471d593 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x471d593)
#<!-- -->38 0x0000000000d012ca cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd012ca)
#<!-- -->39 0x0000000000cf9b1d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->40 0x00000000043b0589 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x000000000397a263 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x397a263)
#<!-- -->42 0x00000000043b07a9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x000000000437722d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x437722d)
#<!-- -->44 0x000000000437822d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x437822d)
#<!-- -->45 0x0000000004381ecc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4381ecc)
#<!-- -->46 0x0000000000cfdf71 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xcfdf71)
#<!-- -->47 0x0000000000bc5e54 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xbc5e54)
#<!-- -->48 0x00007a51ac829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x00007a51ac829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000cf95b5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xcf95b5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
Please check [https://godbolt.org/z/53j77M1EG](https://godbolt.org/z/53j77M1EG)


</details>


---

### Comment 2 - DimitryAndric

This is apparently a long-existing issue, first introduced in `llvmorg-15-init-13886-g`21eb1af469c ("[Concepts] Implement overload resolution for destructors (P0848)") by @royjacobson (cc @erichkeane). This resulted in a different assertion:

```text
pr120939.cpp:5:13: error: initializer on function does not look like a pure-specifier
    virtual ~A() = 0 final;
            ^      ~
pr120939.cpp:5:21: error: expected ';' at end of declaration list
    virtual ~A() = 0 final;
                    ^
                    ;
Assertion failed: (Val && "isa<> used on a null pointer"), function doit, file /home/dim/src/llvm/llvm-project/llvm/include/llvm/Support/Casting.h, line 109.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang -cc1 -triple x86_64 -S pr120939.cpp
1.      pr120939.cpp:8:16: current parser token ')'
 #0 0x0000000002461f3b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x2461f3b)
 #1 0x000000000245fe99 llvm::sys::RunSignalHandlers() (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x245fe99)
 #2 0x00000000024626ae SignalHandler(int) Signals.cpp:0:0
 #3 0x0000000827d874af handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #4 0x0000000827d86a6b thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:245:1
 #5 0x0000000827c702d3 ([vdso]+0x2d3)
 #6 0x000000082dfe9a0a __sys_thr_kill /usr/obj/usr/src/lib/libc/thr_kill.S:4:0
 #7 0x000000082df631e4 _raise /usr/src/lib/libc/gen/raise.c:0:10
 #8 0x000000082e013bc9 abort /usr/src/lib/libc/stdlib/abort.c:71:17
 #9 0x000000082df46551 (/lib/libc.so.7+0x93551)
#10 0x0000000003d71460 clang::Sema::LookupDestructor(clang::CXXRecordDecl*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3d71460)
#11 0x0000000003c8bc87 clang::Sema::ActOnDecltypeExpression(clang::Expr*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3c8bc87)
#12 0x000000000405d6f2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDecltypeType(clang::TypeLocBuilder&, clang::DecltypeTypeLoc) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000040372e1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000004036d9f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000040369ca clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x40369ca)
#16 0x0000000003f0dbb1 SubstDefaultTemplateArgument(clang::Sema&, clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateTypeParmDecl*, llvm::SmallVectorImpl<clang::TemplateArgument>&) SemaTemplate.cpp:0:0
#17 0x0000000003f0d54f clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::SmallVectorImpl<clang::TemplateArgument>&, bool&) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3f0d54f)
#18 0x0000000003fcc20c clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3fcc20c)
#19 0x0000000004030076 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::$_2>(long) SemaTemplateDeduction.cpp:0:0
#20 0x00000000038d571f clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x38d571f)
#21 0x0000000003fce002 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3fce002)
#22 0x0000000003e79cde clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3e79cde)
#23 0x0000000003e8ad29 AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) SemaOverload.cpp:0:0
#24 0x0000000003e8aba3 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3e8aba3)
#25 0x0000000003e8b07a clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3e8b07a)
#26 0x0000000003e8b417 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3e8b417)
#27 0x0000000003b7f281 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3b7f281)
#28 0x0000000003b98ec7 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x3b98ec7)
#29 0x000000000380cfb7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x380cfb7)
#30 0x000000000380dfcc clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x380dfcc)
#31 0x00000000038107c8 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x38107c8)
#32 0x000000000380a014 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x380a014)
#33 0x00000000037d81a4 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x37d81a4)
#34 0x00000000037d5924 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x37d5924)
#35 0x00000000037b9171 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x37b9171)
#36 0x00000000037b8b13 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x37b8b13)
#37 0x00000000037b7a53 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x37b7a53)
#38 0x00000000037b5593 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x37b5593)
#39 0x00000000037afa9e clang::ParseAST(clang::Sema&, bool, bool) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x37afa9e)
#40 0x0000000002e23cd3 clang::FrontendAction::Execute() (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x2e23cd3)
#41 0x0000000002db0b3d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x2db0b3d)
#42 0x0000000002ee60d7 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x2ee60d7)
#43 0x0000000000956339 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/dim/ins/llvmorg-16-init-06721-g477e8e10f08/bin/clang+0x956339)
#44 0x0000000000953e68 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
```

The assertion changed with `llvmorg-16-init-06722-g`adab08ecf2b ("[clang][Sema] Fix crash on invalid base destructor") by @kadircet (cc @hokein), to the following:

```text
Assertion failed: (Func && "No function?"), function MarkFunctionReferenced, file /home/dim/src/llvm/llvm-project/clang/lib/Sema/SemaExpr.cpp, line 17949.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang -cc1 -triple x86_64 -S pr120939.cpp
1.      pr120939.cpp:8:16: current parser token ')'
 #0 0x0000000002461f3b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x2461f3b)
 #1 0x000000000245fe99 llvm::sys::RunSignalHandlers() (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x245fe99)
 #2 0x00000000024626ae SignalHandler(int) Signals.cpp:0:0
 #3 0x00000008281204af handle_signal /usr/src/lib/libthr/thread/thr_sig.c:0:3
 #4 0x000000082811fa6b thr_sighandler /usr/src/lib/libthr/thread/thr_sig.c:245:1
 #5 0x000000082746f2d3 ([vdso]+0x2d3)
 #6 0x000000082f59ca0a __sys_thr_kill /usr/obj/usr/src/lib/libc/thr_kill.S:4:0
 #7 0x000000082f5161e4 _raise /usr/src/lib/libc/gen/raise.c:0:10
 #8 0x000000082f5c6bc9 abort /usr/src/lib/libc/stdlib/abort.c:71:17
 #9 0x000000082f4f9551 (/lib/libc.so.7+0x93551)
#10 0x0000000003bbdadd clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3bbdadd)
#11 0x0000000003c8bc48 clang::Sema::ActOnDecltypeExpression(clang::Expr*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3c8bc48)
#12 0x000000000405d672 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDecltypeType(clang::TypeLocBuilder&, clang::DecltypeTypeLoc) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000004037261 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000004036d1f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000403694a clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x403694a)
#16 0x0000000003f0db31 SubstDefaultTemplateArgument(clang::Sema&, clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateTypeParmDecl*, llvm::SmallVectorImpl<clang::TemplateArgument>&) SemaTemplate.cpp:0:0
#17 0x0000000003f0d4cf clang::Sema::SubstDefaultTemplateArgumentIfAvailable(clang::TemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::Decl*, llvm::SmallVectorImpl<clang::TemplateArgument>&, bool&) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3f0d4cf)
#18 0x0000000003fcc18c clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3fcc18c)
#19 0x000000000402fff6 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::$_2>(long) SemaTemplateDeduction.cpp:0:0
#20 0x00000000038d571f clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x38d571f)
#21 0x0000000003fcdf82 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3fcdf82)
#22 0x0000000003e79c5e clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3e79c5e)
#23 0x0000000003e8aca9 AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) SemaOverload.cpp:0:0
#24 0x0000000003e8ab23 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3e8ab23)
#25 0x0000000003e8affa clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3e8affa)
#26 0x0000000003e8b397 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3e8b397)
#27 0x0000000003b7f221 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3b7f221)
#28 0x0000000003b98e67 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x3b98e67)
#29 0x000000000380cfb7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x380cfb7)
#30 0x000000000380dfcc clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x380dfcc)
#31 0x00000000038107c8 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x38107c8)
#32 0x000000000380a014 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x380a014)
#33 0x00000000037d81a4 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x37d81a4)
#34 0x00000000037d5924 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x37d5924)
#35 0x00000000037b9171 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x37b9171)
#36 0x00000000037b8b13 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x37b8b13)
#37 0x00000000037b7a53 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x37b7a53)
#38 0x00000000037b5593 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x37b5593)
#39 0x00000000037afa9e clang::ParseAST(clang::Sema&, bool, bool) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x37afa9e)
#40 0x0000000002e23cd3 clang::FrontendAction::Execute() (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x2e23cd3)
#41 0x0000000002db0b3d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x2db0b3d)
#42 0x0000000002ee60d7 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x2ee60d7)
#43 0x0000000000956339 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/dim/ins/llvmorg-16-init-06722-gadab08ecf2b/bin/clang+0x956339)
#44 0x0000000000953e68 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
```

It still reproduces with main as of today.


---

### Comment 3 - erichkeane

So @royjacobson did a bunch of work in the area that I think needs to be considered, so I hope he can take a look at this.  But the basics of it are that the `final` causes the destructor to be 'invalid', and then when we go to look up the destructor, it is `isIneligibleOrNotSelected` in DeclCXX's `getDestructor`.  The result is that the `LookupDestructor` in `ActOnDecltypeExpression` (when trying to set the cleanups for delayed decltype binds) gets a `nullptr` Destructor (see ~7938), as the destructor is 'invalid' thanks to the `final` not being in the right place.

I think we have a few approaches:
1- Not mark the destructor 'invalid'.  I think this ends up still being somewhat fragile, as we can end up with no destructor in other cases I think, thanks to roy's work for ineligible destructors.  This also ends up being problematic if we ever try to continue with that destructor with no real knowledge of how much of it is 'good'.

2- Have the `LookupDestructor` figure out how to pick up the invalid but not ineligible destructor.  I think this is a bad idea as well, as I don't have a good way of figuring out whether this is intentional.  

3- Do a better job setting `needsImplicitDestructor`.  I think this means we would need to revert the `DeclaredSpecialMembers` on the `CXXRecordDecl` if the destructor comes back invalid.  I think this is reasonable, but I don't have a good feel of the impact.  This would result in `LookupDestructor` giving an implicit destructor, which is perhaps ok and the best way 'forward'?

4- Just make `ActonDecltypeExpression` tolerant of the `Destructor` being nullptr, and skip the cleanup.  This seems reasonable, but I somewhat fear this is a touch of 'wack-a-mole'.  

I lean towards 3 with 4 being plausible as well.  I think the deciding factor is if we can come up with an example where a non-invalid case can cause this.  That is, can we end up in a situation where the only written destructors are 'ineligible or not selected', and thus this gets a nullptr anyway?  If we can, we should do 4 for that anyway.

So in the end, I would want feedback from @royjacobson 

---

