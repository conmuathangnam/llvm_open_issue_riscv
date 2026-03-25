# [clang] Assertion failure from member function of variadic class template constrained on class template parameters

**Author:** davidstone
**URL:** https://github.com/llvm/llvm-project/issues/162125
**Status:** Closed
**Labels:** clang:frontend, regression, confirmed, crash
**Closed Date:** 2025-10-07T11:10:16Z

## Body

The following valid translation unit

```c++
template<typename, int size>
concept true_int = (size, true);

template<typename, typename... Ts>
concept true_types = true_int<void, sizeof...(Ts)>;

template<typename... Ts>
struct s {
	template<typename T> requires true_types<T, Ts...>
	auto f(T) -> void {
	}
};

template auto s<void>::f(int) -> void;
```

causes clang to crash with

```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Sema/Template.h:156: const clang::TemplateArgument& clang::MultiLevelTemplateArgumentList::operator()(unsigned int, unsigned int) const: Assertion `Index < TemplateArgumentLists[getNumLevels() - Depth - 1].Args.size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:14:38: current parser token ';'
 #0 0x00000000041ca628 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ca628)
 #1 0x00000000041c7a54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c7a54)
 #2 0x000000000410bce8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000709188842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007091888969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000709188842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007091888287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070918882871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000709188839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006bf2bf3 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseType(clang::QualType, bool) SemaConcept.cpp:0:0
#10 0x0000000006c052fe clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseTemplateTypeParmDecl(clang::TemplateTypeParmDecl*) SemaConcept.cpp:0:0
#11 0x0000000006be9525 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::dataTraverseNode(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) SemaConcept.cpp:0:0
#12 0x0000000006bf0709 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) (.constprop.0) SemaConcept.cpp:0:0
#13 0x0000000006bf156b clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseSubstNonTypeTemplateParmExpr(clang::SubstNonTypeTemplateParmExpr*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) SemaConcept.cpp:0:0
#14 0x0000000006be963b clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::dataTraverseNode(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) SemaConcept.cpp:0:0
#15 0x0000000006bf0709 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl<llvm::PointerIntPair<clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits<clang::Stmt*>, llvm::PointerIntPairInfo<clang::Stmt*, 1u, llvm::PointerLikeTypeTraits<clang::Stmt*>>>>*) (.constprop.0) SemaConcept.cpp:0:0
#16 0x0000000006bf30b1 clang::RecursiveASTVisitor<(anonymous namespace)::HashParameterMapping>::TraverseTemplateArgument(clang::TemplateArgument const&) SemaConcept.cpp:0:0
#17 0x0000000006bf3387 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&, clang::MultiLevelTemplateArgumentList const&) SemaConcept.cpp:0:0
#18 0x0000000006bf6f18 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&, clang::MultiLevelTemplateArgumentList const&) SemaConcept.cpp:0:0
#19 0x0000000006bf6f18 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&, clang::MultiLevelTemplateArgumentList const&) SemaConcept.cpp:0:0
#20 0x0000000006bf9615 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::Expr**, clang::ConceptReference const*) SemaConcept.cpp:0:0
#21 0x0000000006bf9dad clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion<clang::NamedDecl const*, clang::concepts::NestedRequirement const*>, llvm::ArrayRef<clang::AssociatedConstraint>, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&, clang::ConceptReference const*, clang::Expr**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf9dad)
#22 0x0000000006bfaeea clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bfaeea)
#23 0x0000000007373598 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7373598)
#24 0x0000000007374a6c void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool)::'lambda0'()>(long) SemaTemplateDeduction.cpp:0:0
#25 0x000000000817f1f1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x817f1f1)
#26 0x0000000007335a8c clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7335a8c)
#27 0x00000000072c4102 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72c4102)
#28 0x00000000068e259d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e259d)
#29 0x00000000068efc29 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68efc29)
#30 0x00000000069bf9b8 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69bf9b8)
#31 0x00000000069c050c clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c050c)
#32 0x00000000069cd078 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69cd078)
#33 0x00000000068f8f30 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f8f30)
#34 0x00000000068b5627 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68b5627)
#35 0x00000000068b64f5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68b64f5)
#36 0x0000000006897b5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897b5a)
#37 0x0000000004b08ee8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b08ee8)
#38 0x0000000004df44e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df44e5)
#39 0x0000000004d754be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d754be)
#40 0x0000000004eeb75d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eeb75d)
#41 0x0000000000dbbd40 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbbd40)
#42 0x0000000000db28aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#43 0x0000000000db2a2d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#44 0x0000000004b72069 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#45 0x000000000410c184 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x410c184)
#46 0x0000000004b7267f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#47 0x0000000004b339b2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b339b2)
#48 0x0000000004b3495e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3495e)
#49 0x0000000004b3c095 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3c095)
#50 0x0000000000db8269 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8269)
#51 0x0000000000c69a74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc69a74)
#52 0x0000709188829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#53 0x0000709188829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#54 0x0000000000db2345 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2345)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

@cor3ntin @zyn0217 

See it live: https://godbolt.org/z/Kj9cd3xPE

## Comments

### Comment 1 - shafik

Looks like a trunk regression: https://godbolt.org/z/8b6fE46f6

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: David Stone (davidstone)

<details>
The following valid translation unit

```c++
template&lt;typename, int size&gt;
concept true_int = (size, true);

template&lt;typename, typename... Ts&gt;
concept true_types = true_int&lt;void, sizeof...(Ts)&gt;;

template&lt;typename... Ts&gt;
struct s {
	template&lt;typename T&gt; requires true_types&lt;T, Ts...&gt;
	auto f(T) -&gt; void {
	}
};

template auto s&lt;void&gt;::f(int) -&gt; void;
```

causes clang to crash with

```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Sema/Template.h:156: const clang::TemplateArgument&amp; clang::MultiLevelTemplateArgumentList::operator()(unsigned int, unsigned int) const: Assertion `Index &lt; TemplateArgumentLists[getNumLevels() - Depth - 1].Args.size()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:14:38: current parser token ';'
 #<!-- -->0 0x00000000041ca628 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ca628)
 #<!-- -->1 0x00000000041c7a54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41c7a54)
 #<!-- -->2 0x000000000410bce8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000709188842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007091888969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000709188842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007091888287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070918882871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000709188839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006bf2bf3 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseType(clang::QualType, bool) SemaConcept.cpp:0:0
#<!-- -->10 0x0000000006c052fe clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseTemplateTypeParmDecl(clang::TemplateTypeParmDecl*) SemaConcept.cpp:0:0
#<!-- -->11 0x0000000006be9525 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::dataTraverseNode(clang::Stmt*, llvm::SmallVectorImpl&lt;llvm::PointerIntPair&lt;clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;, llvm::PointerIntPairInfo&lt;clang::Stmt*, 1u, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;&gt;&gt;&gt;*) SemaConcept.cpp:0:0
#<!-- -->12 0x0000000006bf0709 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl&lt;llvm::PointerIntPair&lt;clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;, llvm::PointerIntPairInfo&lt;clang::Stmt*, 1u, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;&gt;&gt;&gt;*) (.constprop.0) SemaConcept.cpp:0:0
#<!-- -->13 0x0000000006bf156b clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseSubstNonTypeTemplateParmExpr(clang::SubstNonTypeTemplateParmExpr*, llvm::SmallVectorImpl&lt;llvm::PointerIntPair&lt;clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;, llvm::PointerIntPairInfo&lt;clang::Stmt*, 1u, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;&gt;&gt;&gt;*) SemaConcept.cpp:0:0
#<!-- -->14 0x0000000006be963b clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::dataTraverseNode(clang::Stmt*, llvm::SmallVectorImpl&lt;llvm::PointerIntPair&lt;clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;, llvm::PointerIntPairInfo&lt;clang::Stmt*, 1u, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;&gt;&gt;&gt;*) SemaConcept.cpp:0:0
#<!-- -->15 0x0000000006bf0709 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseStmt(clang::Stmt*, llvm::SmallVectorImpl&lt;llvm::PointerIntPair&lt;clang::Stmt*, 1u, bool, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;, llvm::PointerIntPairInfo&lt;clang::Stmt*, 1u, llvm::PointerLikeTypeTraits&lt;clang::Stmt*&gt;&gt;&gt;&gt;*) (.constprop.0) SemaConcept.cpp:0:0
#<!-- -->16 0x0000000006bf30b1 clang::RecursiveASTVisitor&lt;(anonymous namespace)::HashParameterMapping&gt;::TraverseTemplateArgument(clang::TemplateArgument const&amp;) SemaConcept.cpp:0:0
#<!-- -->17 0x0000000006bf3387 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::AtomicConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) SemaConcept.cpp:0:0
#<!-- -->18 0x0000000006bf6f18 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) SemaConcept.cpp:0:0
#<!-- -->19 0x0000000006bf6f18 (anonymous namespace)::ConstraintSatisfactionChecker::Evaluate(clang::ConceptIdConstraint const&amp;, clang::MultiLevelTemplateArgumentList const&amp;) SemaConcept.cpp:0:0
#<!-- -->20 0x0000000006bf9615 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::Expr**, clang::ConceptReference const*) SemaConcept.cpp:0:0
#<!-- -->21 0x0000000006bf9dad clang::Sema::CheckConstraintSatisfaction(llvm::PointerUnion&lt;clang::NamedDecl const*, clang::concepts::NestedRequirement const*&gt;, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;, clang::ConceptReference const*, clang::Expr**) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf9dad)
#<!-- -->22 0x0000000006bfaeea clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bfaeea)
#<!-- -->23 0x0000000007373598 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7373598)
#<!-- -->24 0x0000000007374a6c void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool)::'lambda0'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->25 0x000000000817f1f1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x817f1f1)
#<!-- -->26 0x0000000007335a8c clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7335a8c)
#<!-- -->27 0x00000000072c4102 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72c4102)
#<!-- -->28 0x00000000068e259d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e259d)
#<!-- -->29 0x00000000068efc29 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68efc29)
#<!-- -->30 0x00000000069bf9b8 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69bf9b8)
#<!-- -->31 0x00000000069c050c clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69c050c)
#<!-- -->32 0x00000000069cd078 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69cd078)
#<!-- -->33 0x00000000068f8f30 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f8f30)
#<!-- -->34 0x00000000068b5627 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68b5627)
#<!-- -->35 0x00000000068b64f5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68b64f5)
#<!-- -->36 0x0000000006897b5a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6897b5a)
#<!-- -->37 0x0000000004b08ee8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b08ee8)
#<!-- -->38 0x0000000004df44e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df44e5)
#<!-- -->39 0x0000000004d754be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d754be)
#<!-- -->40 0x0000000004eeb75d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eeb75d)
#<!-- -->41 0x0000000000dbbd40 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbbd40)
#<!-- -->42 0x0000000000db28aa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->43 0x0000000000db2a2d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->44 0x0000000004b72069 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->45 0x000000000410c184 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x410c184)
#<!-- -->46 0x0000000004b7267f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->47 0x0000000004b339b2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b339b2)
#<!-- -->48 0x0000000004b3495e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3495e)
#<!-- -->49 0x0000000004b3c095 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3c095)
#<!-- -->50 0x0000000000db8269 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb8269)
#<!-- -->51 0x0000000000c69a74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc69a74)
#<!-- -->52 0x0000709188829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->53 0x0000709188829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->54 0x0000000000db2345 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2345)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

@<!-- -->cor3ntin @<!-- -->zyn0217 

See it live: https://godbolt.org/z/Kj9cd3xPE
</details>


---

### Comment 3 - davidstone

Yes, it was introduced by 9583b399d85cacdfa0a41f798ab44abaa3981bbf, I meant to include that in my initial report.

---

