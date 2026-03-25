# [Clang] Crash on CTAD for type alias template involving default template argument and constrained ctor template

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/122134
**Status:** Closed
**Labels:** c++20, clang:frontend, crash
**Closed Date:** 2025-01-16T08:37:59Z

## Body

Originally reported for MSVC [here](https://developercommunity.visualstudio.com/t/10584684).
```c++
template <class, class> concept Constraint = true;

template <class T, int> struct Struct {
	Struct(Constraint<T> auto) { }
};

template <int N = 0> using Test = Struct<int, N>;

int main() {
	Test{ 0 };
}
```

See https://compiler-explorer.com/z/7Tn3dGG3v.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:2713:
clang::QualType {anonymous}::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool):
Assertion `Arg.getKind() == TemplateArgument::Type && "Template argument kind mismatch"' failed.
```

<details>
<summary>Stack dump:</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:10:11: current parser token ';'
2.	<source>:9:12: parsing function body 'main'
3.	<source>:9:12: in compound statement ('{}')
 #0 0x0000000003c71738 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c71738)
 #1 0x0000000003c6f444 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c6f444)
 #2 0x0000000003bbc988 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076e1e1a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076e1e1a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076e1e1a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076e1e1a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076e1e1a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076e1e1a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007120f3b (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000007124804 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000007128d3a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000714590c (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007147385 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000071478ae clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000712cc1a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000007155382 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7155382)
#17 0x00000000068ef06f clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#18 0x00000000068f0d7e CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#19 0x00000000068f13dd clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f13dd)
#20 0x00000000068f330a clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f330a)
#21 0x000000000709c07c clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709c07c)
#22 0x000000000709d5aa void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#23 0x0000000007da9101 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7da9101)
#24 0x000000000707683d clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x707683d)
#25 0x0000000006ef038e clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ef038e)
#26 0x0000000006cfe748 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#27 0x0000000006cff63f clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cff63f)
#28 0x0000000006bf4665 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf4665)
#29 0x0000000006c5175d clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c5175d)
#30 0x00000000066a7ac6 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a7ac6)
#31 0x000000000668b6da clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b6da)
#32 0x000000000668c217 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668c217)
#33 0x000000000668c2a9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668c2a9)
#34 0x0000000006690ce9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6690ce9)
#35 0x0000000006714b49 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6714b49)
#36 0x000000000670aebe clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670aebe)
#37 0x000000000670be10 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670be10)
#38 0x000000000670ccd5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670ccd5)
#39 0x000000000670e4da clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670e4da)
#40 0x00000000066207a3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66207a3)
#41 0x000000000665535d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665535d)
#42 0x000000000661450e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661450e)
#43 0x0000000006614cc9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6614cc9)
#44 0x000000000661c473 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661c473)
#45 0x000000000661d34d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661d34d)
#46 0x000000000660f7aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660f7aa)
#47 0x0000000004602478 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4602478)
#48 0x00000000048be799 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48be799)
#49 0x0000000004840e2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4840e2e)
#50 0x00000000049abb8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49abb8e)
#51 0x0000000000ce93ef cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce93ef)
#52 0x0000000000ce0e8a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#53 0x0000000004647ae9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#54 0x0000000003bbce34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bbce34)
#55 0x00000000046480df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#56 0x000000000460c1ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x460c1ed)
#57 0x000000000460d26e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x460d26e)
#58 0x0000000004614d95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4614d95)
#59 0x0000000000ce6233 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce6233)
#60 0x0000000000bb2524 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbb2524)
#61 0x000076e1e1a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#62 0x000076e1e1a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#63 0x0000000000ce0935 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce0935)
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
Originally reported for MSVC [here](https://developercommunity.visualstudio.com/t/10584684).
```c++
template &lt;class, class&gt; concept Constraint = true;

template &lt;class T, int&gt; struct Struct {
	Struct(Constraint&lt;T&gt; auto) { }
};

template &lt;int N = 0&gt; using Test = Struct&lt;int, N&gt;;

int main() {
	Test{ 0 };
}
```

See https://compiler-explorer.com/z/7Tn3dGG3v.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:2713:
clang::QualType {anonymous}::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::TemplateTypeParmTypeLoc, bool):
Assertion `Arg.getKind() == TemplateArgument::Type &amp;&amp; "Template argument kind mismatch"' failed.
```

&lt;details&gt;
&lt;summary&gt;Stack dump:&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:10:11: current parser token ';'
2.	&lt;source&gt;:9:12: parsing function body 'main'
3.	&lt;source&gt;:9:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003c71738 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c71738)
 #<!-- -->1 0x0000000003c6f444 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c6f444)
 #<!-- -->2 0x0000000003bbc988 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000076e1e1a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000076e1e1a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000076e1e1a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000076e1e1a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076e1e1a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000076e1e1a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007120f3b (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x0000000007124804 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000007128d3a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x000000000714590c (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000007147385 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x00000000071478ae clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x000000000712cc1a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000007155382 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7155382)
#<!-- -->17 0x00000000068ef06f clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator const&amp;) SemaConcept.cpp:0:0
#<!-- -->18 0x00000000068f0d7e CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->19 0x00000000068f13dd clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f13dd)
#<!-- -->20 0x00000000068f330a clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f330a)
#<!-- -->21 0x000000000709c07c clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709c07c)
#<!-- -->22 0x000000000709d5aa void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->23 0x0000000007da9101 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7da9101)
#<!-- -->24 0x000000000707683d clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x707683d)
#<!-- -->25 0x0000000006ef038e clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ef038e)
#<!-- -->26 0x0000000006cfe748 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#<!-- -->27 0x0000000006cff63f clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cff63f)
#<!-- -->28 0x0000000006bf4665 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf4665)
#<!-- -->29 0x0000000006c5175d clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c5175d)
#<!-- -->30 0x00000000066a7ac6 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a7ac6)
#<!-- -->31 0x000000000668b6da clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b6da)
#<!-- -->32 0x000000000668c217 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668c217)
#<!-- -->33 0x000000000668c2a9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668c2a9)
#<!-- -->34 0x0000000006690ce9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6690ce9)
#<!-- -->35 0x0000000006714b49 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6714b49)
#<!-- -->36 0x000000000670aebe clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670aebe)
#<!-- -->37 0x000000000670be10 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670be10)
#<!-- -->38 0x000000000670ccd5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670ccd5)
#<!-- -->39 0x000000000670e4da clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670e4da)
#<!-- -->40 0x00000000066207a3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66207a3)
#<!-- -->41 0x000000000665535d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665535d)
#<!-- -->42 0x000000000661450e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661450e)
#<!-- -->43 0x0000000006614cc9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6614cc9)
#<!-- -->44 0x000000000661c473 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661c473)
#<!-- -->45 0x000000000661d34d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661d34d)
#<!-- -->46 0x000000000660f7aa clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660f7aa)
#<!-- -->47 0x0000000004602478 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4602478)
#<!-- -->48 0x00000000048be799 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48be799)
#<!-- -->49 0x0000000004840e2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4840e2e)
#<!-- -->50 0x00000000049abb8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49abb8e)
#<!-- -->51 0x0000000000ce93ef cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce93ef)
#<!-- -->52 0x0000000000ce0e8a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->53 0x0000000004647ae9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->54 0x0000000003bbce34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bbce34)
#<!-- -->55 0x00000000046480df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->56 0x000000000460c1ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x460c1ed)
#<!-- -->57 0x000000000460d26e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x460d26e)
#<!-- -->58 0x0000000004614d95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4614d95)
#<!-- -->59 0x0000000000ce6233 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce6233)
#<!-- -->60 0x0000000000bb2524 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbb2524)
#<!-- -->61 0x000076e1e1a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->62 0x000076e1e1a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->63 0x0000000000ce0935 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce0935)
```

&lt;/details&gt;
</details>


---

### Comment 2 - cor3ntin

@hokein 

---

### Comment 3 - shafik

Looks like this came in clang-19, which I am guessing was when this was implemented. 

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (MagentaTreehouse)

<details>
Originally reported for MSVC [here](https://developercommunity.visualstudio.com/t/10584684).
```c++
template &lt;class, class&gt; concept Constraint = true;

template &lt;class T, int&gt; struct Struct {
	Struct(Constraint&lt;T&gt; auto) { }
};

template &lt;int N = 0&gt; using Test = Struct&lt;int, N&gt;;

int main() {
	Test{ 0 };
}
```

See https://compiler-explorer.com/z/7Tn3dGG3v.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:2713:
clang::QualType {anonymous}::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::TemplateTypeParmTypeLoc, bool):
Assertion `Arg.getKind() == TemplateArgument::Type &amp;&amp; "Template argument kind mismatch"' failed.
```

&lt;details&gt;
&lt;summary&gt;Stack dump:&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:10:11: current parser token ';'
2.	&lt;source&gt;:9:12: parsing function body 'main'
3.	&lt;source&gt;:9:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003c71738 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c71738)
 #<!-- -->1 0x0000000003c6f444 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c6f444)
 #<!-- -->2 0x0000000003bbc988 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000076e1e1a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000076e1e1a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000076e1e1a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000076e1e1a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076e1e1a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000076e1e1a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007120f3b (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x0000000007124804 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000007128d3a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x000000000714590c (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000007147385 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x00000000071478ae clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x000000000712cc1a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000007155382 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7155382)
#<!-- -->17 0x00000000068ef06f clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator const&amp;) SemaConcept.cpp:0:0
#<!-- -->18 0x00000000068f0d7e CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->19 0x00000000068f13dd clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f13dd)
#<!-- -->20 0x00000000068f330a clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f330a)
#<!-- -->21 0x000000000709c07c clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x709c07c)
#<!-- -->22 0x000000000709d5aa void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->23 0x0000000007da9101 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7da9101)
#<!-- -->24 0x000000000707683d clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x707683d)
#<!-- -->25 0x0000000006ef038e clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ef038e)
#<!-- -->26 0x0000000006cfe748 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#<!-- -->27 0x0000000006cff63f clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cff63f)
#<!-- -->28 0x0000000006bf4665 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf4665)
#<!-- -->29 0x0000000006c5175d clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c5175d)
#<!-- -->30 0x00000000066a7ac6 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66a7ac6)
#<!-- -->31 0x000000000668b6da clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668b6da)
#<!-- -->32 0x000000000668c217 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668c217)
#<!-- -->33 0x000000000668c2a9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668c2a9)
#<!-- -->34 0x0000000006690ce9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6690ce9)
#<!-- -->35 0x0000000006714b49 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6714b49)
#<!-- -->36 0x000000000670aebe clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670aebe)
#<!-- -->37 0x000000000670be10 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670be10)
#<!-- -->38 0x000000000670ccd5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670ccd5)
#<!-- -->39 0x000000000670e4da clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670e4da)
#<!-- -->40 0x00000000066207a3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66207a3)
#<!-- -->41 0x000000000665535d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665535d)
#<!-- -->42 0x000000000661450e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661450e)
#<!-- -->43 0x0000000006614cc9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6614cc9)
#<!-- -->44 0x000000000661c473 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661c473)
#<!-- -->45 0x000000000661d34d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661d34d)
#<!-- -->46 0x000000000660f7aa clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x660f7aa)
#<!-- -->47 0x0000000004602478 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4602478)
#<!-- -->48 0x00000000048be799 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48be799)
#<!-- -->49 0x0000000004840e2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4840e2e)
#<!-- -->50 0x00000000049abb8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49abb8e)
#<!-- -->51 0x0000000000ce93ef cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce93ef)
#<!-- -->52 0x0000000000ce0e8a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->53 0x0000000004647ae9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->54 0x0000000003bbce34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bbce34)
#<!-- -->55 0x00000000046480df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->56 0x000000000460c1ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x460c1ed)
#<!-- -->57 0x000000000460d26e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x460d26e)
#<!-- -->58 0x0000000004614d95 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4614d95)
#<!-- -->59 0x0000000000ce6233 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce6233)
#<!-- -->60 0x0000000000bb2524 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbb2524)
#<!-- -->61 0x000076e1e1a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->62 0x000076e1e1a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->63 0x0000000000ce0935 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce0935)
```

&lt;/details&gt;
</details>


---

