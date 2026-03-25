# Crash with "Should not have a NULL template argument"

**Author:** halbi2
**URL:** https://github.com/llvm/llvm-project/issues/174667
**Status:** Closed
**Labels:** clang:frontend, regression, concepts, confirmed, crash
**Closed Date:** 2026-01-09T08:34:00Z

## Body

https://godbolt.org/z/vcej6vaK8

```
template<class T, class>
concept C = requires{ requires T(); };

template<C<void> T> int f();
int main() { f<int>(); }
```

Crashes on Clang trunk but not in Clang 21.0.

```
Should not have a NULL template argument
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/TemplateBase.cpp:265!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:5:21: current parser token ')'
2.	<source>:5:12: parsing function body 'main'
3.	<source>:5:12: in compound statement ('{}')
 #0 0x0000000004311b48 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4311b48)
 #1 0x000000000430ef74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430ef74)
 #2 0x0000000004252588 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079befe042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000079befe0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000079befe042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000079befe0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000000000425d0fa (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x425d0fa)
 #8 0x0000000008136a8b clang::TemplateArgument::getDependence() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8136a8b)
 #9 0x0000000008136bd9 clang::TemplateArgument::isInstantiationDependent() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8136bd9)
#10 0x0000000006d9f650 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::Expr**, clang::ConceptReference const*) SemaConcept.cpp:0:0
#11 0x0000000006d9fddf clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::ConceptReference const*, clang::Expr**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9fddf)
#12 0x00000000075be3c8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000075940a2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000075cdec7 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75cdec7)
#15 0x0000000006d9af14 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&, clang::MultiLevelTemplateArgumentList const&) SemaConcept.cpp:0:0
#16 0x0000000006d9d153 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&, clang::MultiLevelTemplateArgumentList const&) SemaConcept.cpp:0:0
#17 0x0000000006d9f7e5 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::Expr**, clang::ConceptReference const*) SemaConcept.cpp:0:0
#18 0x0000000006d9fffd clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::ConceptReference const*, clang::Expr**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9fffd)
#19 0x0000000006da117a clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6da117a)
#20 0x00000000075296f8 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75296f8)
#21 0x000000000752ab6a void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#22 0x00000000085332c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85332c1)
#23 0x0000000007509d70 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7509d70)
#24 0x000000000735b2bb AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#25 0x000000000735d958 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#26 0x00000000073623ba clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73623ba)
#27 0x00000000073627de clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73627de)
#28 0x0000000007375110 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7375110)
#29 0x0000000006fd387a clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd387a)
#30 0x0000000006fd8d8c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd8d8c)
#31 0x0000000006ad69a7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad69a7)
#32 0x0000000006acf8ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6acf8ba)
#33 0x0000000006ad1b17 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad1b17)
#34 0x0000000006ad1ba9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad1ba9)
#35 0x0000000006ad6189 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad6189)
#36 0x0000000006b5ba49 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5ba49)
#37 0x0000000006b52ef4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b52ef4)
#38 0x0000000006b538d6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b538d6)
#39 0x0000000006b5c21e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5c21e)
#40 0x0000000006b5ca6a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5ca6a)
#41 0x0000000006a629cb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a629cb)
#42 0x0000000006a98e8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a98e8d)
#43 0x0000000006a55b2b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a55b2b)
#44 0x0000000006a5631f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5631f)
#45 0x0000000006a5e14a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5e14a)
#46 0x0000000006a5f0e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5f0e5)
#47 0x0000000006a4016a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4016a)
#48 0x0000000004c751e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c751e8)
#49 0x0000000004f6d605 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6d605)
#50 0x0000000004eed6de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eed6de)
#51 0x00000000050679ad clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50679ad)
#52 0x0000000000de7b64 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde7b64)
#53 0x0000000000dde5da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#54 0x0000000000dde75d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#55 0x0000000004ce8159 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#56 0x0000000004252a24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4252a24)
#57 0x0000000004ce876f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#58 0x0000000004ca9152 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9152)
#59 0x0000000004caa0fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4caa0fe)
#60 0x0000000004cb1555 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb1555)
#61 0x0000000000de3f81 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde3f81)
#62 0x0000000000c8e4a4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8e4a4)
#63 0x000079befe029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#64 0x000079befe029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#65 0x0000000000dde075 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde075)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (halbi2)

<details>
https://godbolt.org/z/vcej6vaK8

```
template&lt;class T, class&gt;
concept C = requires{ requires T(); };

template&lt;C&lt;void&gt; T&gt; int f();
int main() { f&lt;int&gt;(); }
```

Crashes on Clang trunk but not in Clang 21.0.

```
Should not have a NULL template argument
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/AST/TemplateBase.cpp:265!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:5:21: current parser token ')'
2.	&lt;source&gt;:5:12: parsing function body 'main'
3.	&lt;source&gt;:5:12: in compound statement ('{}')
 #<!-- -->0 0x0000000004311b48 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4311b48)
 #<!-- -->1 0x000000000430ef74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430ef74)
 #<!-- -->2 0x0000000004252588 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079befe042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000079befe0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000079befe042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000079befe0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000000000425d0fa (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x425d0fa)
 #<!-- -->8 0x0000000008136a8b clang::TemplateArgument::getDependence() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8136a8b)
 #<!-- -->9 0x0000000008136bd9 clang::TemplateArgument::isInstantiationDependent() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8136bd9)
#<!-- -->10 0x0000000006d9f650 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::Expr**, clang::ConceptReference const*) SemaConcept.cpp:0:0
#<!-- -->11 0x0000000006d9fddf clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::ConceptReference const*, clang::Expr**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9fddf)
#<!-- -->12 0x00000000075be3c8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000075940a2 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x00000000075cdec7 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75cdec7)
#<!-- -->15 0x0000000006d9af14 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) SemaConcept.cpp:0:0
#<!-- -->16 0x0000000006d9d153 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) SemaConcept.cpp:0:0
#<!-- -->17 0x0000000006d9f7e5 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::Expr**, clang::ConceptReference const*) SemaConcept.cpp:0:0
#<!-- -->18 0x0000000006d9fffd clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::ConceptReference const*, clang::Expr**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d9fffd)
#<!-- -->19 0x0000000006da117a clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6da117a)
#<!-- -->20 0x00000000075296f8 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75296f8)
#<!-- -->21 0x000000000752ab6a void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->22 0x00000000085332c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x85332c1)
#<!-- -->23 0x0000000007509d70 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7509d70)
#<!-- -->24 0x000000000735b2bb AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#<!-- -->25 0x000000000735d958 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#<!-- -->26 0x00000000073623ba clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73623ba)
#<!-- -->27 0x00000000073627de clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73627de)
#<!-- -->28 0x0000000007375110 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7375110)
#<!-- -->29 0x0000000006fd387a clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd387a)
#<!-- -->30 0x0000000006fd8d8c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd8d8c)
#<!-- -->31 0x0000000006ad69a7 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad69a7)
#<!-- -->32 0x0000000006acf8ba clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6acf8ba)
#<!-- -->33 0x0000000006ad1b17 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad1b17)
#<!-- -->34 0x0000000006ad1ba9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad1ba9)
#<!-- -->35 0x0000000006ad6189 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ad6189)
#<!-- -->36 0x0000000006b5ba49 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5ba49)
#<!-- -->37 0x0000000006b52ef4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b52ef4)
#<!-- -->38 0x0000000006b538d6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b538d6)
#<!-- -->39 0x0000000006b5c21e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5c21e)
#<!-- -->40 0x0000000006b5ca6a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5ca6a)
#<!-- -->41 0x0000000006a629cb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a629cb)
#<!-- -->42 0x0000000006a98e8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a98e8d)
#<!-- -->43 0x0000000006a55b2b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a55b2b)
#<!-- -->44 0x0000000006a5631f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5631f)
#<!-- -->45 0x0000000006a5e14a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5e14a)
#<!-- -->46 0x0000000006a5f0e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5f0e5)
#<!-- -->47 0x0000000006a4016a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a4016a)
#<!-- -->48 0x0000000004c751e8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c751e8)
#<!-- -->49 0x0000000004f6d605 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6d605)
#<!-- -->50 0x0000000004eed6de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eed6de)
#<!-- -->51 0x00000000050679ad clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x50679ad)
#<!-- -->52 0x0000000000de7b64 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde7b64)
#<!-- -->53 0x0000000000dde5da ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->54 0x0000000000dde75d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->55 0x0000000004ce8159 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->56 0x0000000004252a24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4252a24)
#<!-- -->57 0x0000000004ce876f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->58 0x0000000004ca9152 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9152)
#<!-- -->59 0x0000000004caa0fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4caa0fe)
#<!-- -->60 0x0000000004cb1555 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb1555)
#<!-- -->61 0x0000000000de3f81 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde3f81)
#<!-- -->62 0x0000000000c8e4a4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8e4a4)
#<!-- -->63 0x000079befe029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->64 0x000079befe029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->65 0x0000000000dde075 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde075)
```
</details>


---

