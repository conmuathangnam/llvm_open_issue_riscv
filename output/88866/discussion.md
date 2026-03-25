# [clang] Crash on unexpanded pack in concept-constrained NTTP

**Author:** mjerabek
**URL:** https://github.com/llvm/llvm-project/issues/88866
**Status:** Closed
**Labels:** c++20, clang:frontend, concepts, confirmed, crash
**Closed Date:** 2025-01-03T11:10:45Z

## Body

### Reproducer

```c++
template <typename...Ts> struct index_by;

template <typename T, typename Indices>
concept InitFunc = true;

// with constrained non-type template parameter
// clang crashes
// gcc is still fine
template <typename Indices, InitFunc<Indices> auto init>
struct LazyLitMatrix;

template <
    typename...Indices,
    InitFunc<index_by<Indices>> auto init /* This parameter pack should be expanded */
>
struct LazyLitMatrix<index_by<Indices...>, init> {
};

LazyLitMatrix<
    index_by<int, char>,
    42
> inst;
```

When using the same for a type argument, clang correctly errors out with `type constraint contains unexpanded parameter pack 'Indices'`. After fixing that, even the original case with NTTP compiles correctly. See on compiler explorer: https://godbolt.org/z/4PocYYqWx

### Stack trace

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:22:7: current parser token ';'
 #0 0x00000000036b0788 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36b0788)
 #1 0x00000000036ae8d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36ae8d4)
 #2 0x00000000035f6e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fdd8e442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006fcbc61 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #5 0x0000000006fba9de Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
 #6 0x0000000006fd75c6 EvaluateInPlace(clang::APValue&, (anonymous namespace)::EvalInfo&, (anonymous namespace)::LValue const&, clang::Expr const*, bool) (.constprop.0) ExprConstant.cpp:0:0
 #7 0x0000000006fe590a clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fe590a)
 #8 0x00000000060b4c3e clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::'lambda'(clang::Expr const*)>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::'lambda'(clang::Expr const*)&&) SemaConcept.cpp:0:0
 #9 0x00000000060b5548 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#10 0x00000000060b5ae8 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60b5ae8)
#11 0x00000000068f8894 _Z31FinishTemplateArgumentDeductionIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_bN4llvm8ArrayRefINS0_16TemplateArgumentEEERNSC_15SmallVectorImplINS0_23DeducedTemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#12 0x00000000068f8cff _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
#13 0x0000000005f43f71 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5f43f71)
#14 0x000000000690202e _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#15 0x0000000006949012 getPatternForClassTemplateSpecialization(clang::Sema&, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind) (.isra.0) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000006986f69 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6986f69)
#17 0x0000000006a0842f void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#18 0x0000000005f43f71 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5f43f71)
#19 0x0000000006a0d7f5 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a0d7f5)
#20 0x0000000006a0d875 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a0d875)
#21 0x0000000006a0d950 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a0d950)
#22 0x0000000006168a91 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6168a91)
#23 0x0000000005e0e9dc clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5e0e9dc)
#24 0x0000000005e25f48 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5e25f48)
#25 0x0000000005dda64d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5dda64d)
#26 0x0000000005ddb223 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5ddb223)
#27 0x0000000005de38d2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5de38d2)
#28 0x0000000005de5447 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5de5447)
#29 0x0000000005dd443a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5dd443a)
#30 0x0000000003f18955 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f18955)
#31 0x00000000041af831 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41af831)
#32 0x000000000413506b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x413506b)
#33 0x0000000004292e13 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4292e13)
#34 0x0000000000c50e2c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc50e2c)
#35 0x0000000000c4addd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x0000000003f61839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x00000000035f72b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35f72b3)
#38 0x0000000003f61a59 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000003f27fa7 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f27fa7)
#40 0x0000000003f2895d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f2895d)
#41 0x0000000003f3277c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f3277c)
#42 0x0000000000c4e291 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4e291)
#43 0x0000000000b31424 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xb31424)
#44 0x00007fdd8e429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x00007fdd8e429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000c4a93e _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4a93e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - tbaederr

With assertions: https://godbolt.org/z/x6deonqqf

Assertion:

```console
clang++: /root/llvm-project/clang/lib/AST/ASTContext.cpp:5906:
clang::QualType clang::ASTContext::getAutoType(clang::QualType, clang::AutoTypeKeyword, bool, bool, clang::ConceptDecl*, llvm::ArrayRef<clang::TemplateArgument>) const:
Assertion `(!IsPack || IsDependent) && "only use IsPack for a dependent pack"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:35:1: current parser token '>'
 #0 0x000000000393d208 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x393d208)
 #1 0x000000000393aeec llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x393aeec)
 #2 0x0000000003880fb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f2c1aa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f2c1aa969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f2c1aa42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f2c1aa287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f2c1aa2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f2c1aa39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000071b4015 clang::ASTContext::getAutoType(clang::QualType, clang::AutoTypeKeyword, bool, bool, clang::ConceptDecl*, llvm::ArrayRef<clang::TemplateArgument>) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71b4015)
#10 0x00000000071b40b5 clang::ASTContext::getUnconstrainedType(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71b40b5)
#11 0x000000000771126d (anonymous namespace)::StmtProfilerWithPointers::VisitDecl(clang::Decl const*) StmtProfile.cpp:0:0
#12 0x000000000770f440 (anonymous namespace)::StmtProfiler::VisitDeclRefExpr(clang::DeclRefExpr const*) StmtProfile.cpp:0:0
#13 0x000000000770a7e4 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::StmtProfiler, void>::Visit(clang::Stmt const*) StmtProfile.cpp:0:0
#14 0x0000000007710f60 clang::Stmt::Profile(llvm::FoldingSetNodeID&, clang::ASTContext const&, bool, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7710f60)
#15 0x00000000071b7769 clang::ASTContext::getDecltypeType(clang::Expr*, clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71b7769)
#16 0x0000000006e55bc3 clang::Sema::BuildDecltypeType(clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e55bc3)
#17 0x0000000006c194e2 clang::Sema::AttachTypeConstraint(clang::AutoTypeLoc, clang::NonTypeTemplateParmDecl*, clang::NonTypeTemplateParmDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c194e2)
#18 0x0000000006c19c68 clang::Sema::ActOnNonTypeTemplateParameter(clang::Scope*, clang::Declarator&, unsigned int, unsigned int, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c19c68)
#19 0x00000000062fad8a clang::Parser::ParseNonTypeTemplateParameter(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62fad8a)
#20 0x00000000062fb77b clang::Parser::ParseTemplateParameter(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62fb77b)
#21 0x00000000062fc58e clang::Parser::ParseTemplateParameterList(unsigned int, llvm::SmallVectorImpl<clang::NamedDecl*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62fc58e)
#22 0x00000000062fccbf clang::Parser::ParseTemplateParameters(clang::Parser::MultiParseScope&, unsigned int, llvm::SmallVectorImpl<clang::NamedDecl*>&, clang::SourceLocation&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62fccbf)
#23 0x00000000062ffa71 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#24 0x00000000062ffe97 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x62ffe97)
#25 0x000000000623b860 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x623b860)
#26 0x00000000061f848e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61f848e)
#27 0x00000000061f930d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61f930d)
#28 0x00000000061ebf5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x61ebf5a)
#29 0x00000000041edcb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41edcb8)

```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Martin Jeřábek (mjerabek)

<details>
### Reproducer

```c++
template &lt;typename...Ts&gt; struct index_by;

template &lt;typename T, typename Indices&gt;
concept InitFunc = true;

// with constrained non-type template parameter
// clang crashes
// gcc is still fine
template &lt;typename Indices, InitFunc&lt;Indices&gt; auto init&gt;
struct LazyLitMatrix;

template &lt;
    typename...Indices,
    InitFunc&lt;index_by&lt;Indices&gt;&gt; auto init /* This parameter pack should be expanded */
&gt;
struct LazyLitMatrix&lt;index_by&lt;Indices...&gt;, init&gt; {
};

LazyLitMatrix&lt;
    index_by&lt;int, char&gt;,
    42
&gt; inst;
```

When using the same for a type argument, clang correctly errors out with `type constraint contains unexpanded parameter pack 'Indices'`. After fixing that, even the original case with NTTP compiles correctly. See on compiler explorer: https://godbolt.org/z/4PocYYqWx

### Stack trace

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:22:7: current parser token ';'
 #<!-- -->0 0x00000000036b0788 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36b0788)
 #<!-- -->1 0x00000000036ae8d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36ae8d4)
 #<!-- -->2 0x00000000035f6e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fdd8e442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006fcbc61 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #<!-- -->5 0x0000000006fba9de Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
 #<!-- -->6 0x0000000006fd75c6 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) (.constprop.0) ExprConstant.cpp:0:0
 #<!-- -->7 0x0000000006fe590a clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fe590a)
 #<!-- -->8 0x00000000060b4c3e clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::'lambda'(clang::Expr const*)&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::'lambda'(clang::Expr const*)&amp;&amp;) SemaConcept.cpp:0:0
 #<!-- -->9 0x00000000060b5548 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->10 0x00000000060b5ae8 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60b5ae8)
#<!-- -->11 0x00000000068f8894 _Z31FinishTemplateArgumentDeductionIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_bN4llvm8ArrayRefINS0_16TemplateArgumentEEERNSC_15SmallVectorImplINS0_23DeducedTemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#<!-- -->12 0x00000000068f8cff _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
#<!-- -->13 0x0000000005f43f71 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5f43f71)
#<!-- -->14 0x000000000690202e _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#<!-- -->15 0x0000000006949012 getPatternForClassTemplateSpecialization(clang::Sema&amp;, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind) (.isra.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000006986f69 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6986f69)
#<!-- -->17 0x0000000006a0842f void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->18 0x0000000005f43f71 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5f43f71)
#<!-- -->19 0x0000000006a0d7f5 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a0d7f5)
#<!-- -->20 0x0000000006a0d875 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a0d875)
#<!-- -->21 0x0000000006a0d950 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a0d950)
#<!-- -->22 0x0000000006168a91 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6168a91)
#<!-- -->23 0x0000000005e0e9dc clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5e0e9dc)
#<!-- -->24 0x0000000005e25f48 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5e25f48)
#<!-- -->25 0x0000000005dda64d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5dda64d)
#<!-- -->26 0x0000000005ddb223 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5ddb223)
#<!-- -->27 0x0000000005de38d2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5de38d2)
#<!-- -->28 0x0000000005de5447 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5de5447)
#<!-- -->29 0x0000000005dd443a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5dd443a)
#<!-- -->30 0x0000000003f18955 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f18955)
#<!-- -->31 0x00000000041af831 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41af831)
#<!-- -->32 0x000000000413506b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x413506b)
#<!-- -->33 0x0000000004292e13 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4292e13)
#<!-- -->34 0x0000000000c50e2c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc50e2c)
#<!-- -->35 0x0000000000c4addd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000003f61839 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x00000000035f72b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35f72b3)
#<!-- -->38 0x0000000003f61a59 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x0000000003f27fa7 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f27fa7)
#<!-- -->40 0x0000000003f2895d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f2895d)
#<!-- -->41 0x0000000003f3277c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f3277c)
#<!-- -->42 0x0000000000c4e291 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4e291)
#<!-- -->43 0x0000000000b31424 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xb31424)
#<!-- -->44 0x00007fdd8e429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x00007fdd8e429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000c4a93e _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4a93e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-c-20

Author: Martin Jeřábek (mjerabek)

<details>
### Reproducer

```c++
template &lt;typename...Ts&gt; struct index_by;

template &lt;typename T, typename Indices&gt;
concept InitFunc = true;

// with constrained non-type template parameter
// clang crashes
// gcc is still fine
template &lt;typename Indices, InitFunc&lt;Indices&gt; auto init&gt;
struct LazyLitMatrix;

template &lt;
    typename...Indices,
    InitFunc&lt;index_by&lt;Indices&gt;&gt; auto init /* This parameter pack should be expanded */
&gt;
struct LazyLitMatrix&lt;index_by&lt;Indices...&gt;, init&gt; {
};

LazyLitMatrix&lt;
    index_by&lt;int, char&gt;,
    42
&gt; inst;
```

When using the same for a type argument, clang correctly errors out with `type constraint contains unexpanded parameter pack 'Indices'`. After fixing that, even the original case with NTTP compiles correctly. See on compiler explorer: https://godbolt.org/z/4PocYYqWx

### Stack trace

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:22:7: current parser token ';'
 #<!-- -->0 0x00000000036b0788 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36b0788)
 #<!-- -->1 0x00000000036ae8d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36ae8d4)
 #<!-- -->2 0x00000000035f6e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fdd8e442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006fcbc61 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
 #<!-- -->5 0x0000000006fba9de Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
 #<!-- -->6 0x0000000006fd75c6 EvaluateInPlace(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, (anonymous namespace)::LValue const&amp;, clang::Expr const*, bool) (.constprop.0) ExprConstant.cpp:0:0
 #<!-- -->7 0x0000000006fe590a clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6fe590a)
 #<!-- -->8 0x00000000060b4c3e clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::'lambda'(clang::Expr const*)&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::'lambda'(clang::Expr const*)&amp;&amp;) SemaConcept.cpp:0:0
 #<!-- -->9 0x00000000060b5548 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->10 0x00000000060b5ae8 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60b5ae8)
#<!-- -->11 0x00000000068f8894 _Z31FinishTemplateArgumentDeductionIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_bN4llvm8ArrayRefINS0_16TemplateArgumentEEERNSC_15SmallVectorImplINS0_23DeducedTemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#<!-- -->12 0x00000000068f8cff _ZN4llvm12function_refIFvvEE11callback_fnIZ23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS5_23TemplateDeductionResultEE4typeERNS5_4SemaEPS9_NS_8ArrayRefINS5_16TemplateArgumentEEERNS5_4sema21TemplateDeductionInfoEEUlvE_EEvl SemaTemplateDeduction.cpp:0:0
#<!-- -->13 0x0000000005f43f71 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5f43f71)
#<!-- -->14 0x000000000690202e _Z23DeduceTemplateArgumentsIN5clang38ClassTemplatePartialSpecializationDeclEENSt9enable_ifIXsr23IsPartialSpecializationIT_E5valueENS0_23TemplateDeductionResultEE4typeERNS0_4SemaEPS4_N4llvm8ArrayRefINS0_16TemplateArgumentEEERNS0_4sema21TemplateDeductionInfoE SemaTemplateDeduction.cpp:0:0
#<!-- -->15 0x0000000006949012 getPatternForClassTemplateSpecialization(clang::Sema&amp;, clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind) (.isra.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000006986f69 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6986f69)
#<!-- -->17 0x0000000006a0842f void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->18 0x0000000005f43f71 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5f43f71)
#<!-- -->19 0x0000000006a0d7f5 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a0d7f5)
#<!-- -->20 0x0000000006a0d875 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a0d875)
#<!-- -->21 0x0000000006a0d950 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a0d950)
#<!-- -->22 0x0000000006168a91 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6168a91)
#<!-- -->23 0x0000000005e0e9dc clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5e0e9dc)
#<!-- -->24 0x0000000005e25f48 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5e25f48)
#<!-- -->25 0x0000000005dda64d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5dda64d)
#<!-- -->26 0x0000000005ddb223 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5ddb223)
#<!-- -->27 0x0000000005de38d2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5de38d2)
#<!-- -->28 0x0000000005de5447 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5de5447)
#<!-- -->29 0x0000000005dd443a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x5dd443a)
#<!-- -->30 0x0000000003f18955 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f18955)
#<!-- -->31 0x00000000041af831 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x41af831)
#<!-- -->32 0x000000000413506b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x413506b)
#<!-- -->33 0x0000000004292e13 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4292e13)
#<!-- -->34 0x0000000000c50e2c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc50e2c)
#<!-- -->35 0x0000000000c4addd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000003f61839 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x00000000035f72b3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35f72b3)
#<!-- -->38 0x0000000003f61a59 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x0000000003f27fa7 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f27fa7)
#<!-- -->40 0x0000000003f2895d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f2895d)
#<!-- -->41 0x0000000003f3277c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f3277c)
#<!-- -->42 0x0000000000c4e291 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4e291)
#<!-- -->43 0x0000000000b31424 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xb31424)
#<!-- -->44 0x00007fdd8e429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x00007fdd8e429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000c4a93e _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc4a93e)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```
</details>


---

### Comment 4 - zyn0217

I suppose we can add the following diagnostics to `AttachTypeConstraint` around line 1295:
```cpp
  SmallVector<UnexpandedParameterPack> UnexpandedPacks;
  collectUnexpandedParameterPacks(TL, UnexpandedPacks);
  if (DiagnoseUnexpandedParameterPacks(
          Ref->getBeginLoc(),
          UnexpandedParameterPackContext::UPPC_TypeConstraint, UnexpandedPacks))
    return true;
```
with this, clang will reject the reproducer with:
```bash
/tmp/issue-88866.cpp:15:38: error: type constraint contains unexpanded parameter pack 'Indices'
   15 |     InitFunc<index_by<Indices>> auto init /* This parameter pack should be expanded */
      |                       ~~~~~~~        ^
/tmp/issue-88866.cpp:23:3: error: implicit instantiation of undefined template 'LazyLitMatrix<index_by<int, char>, 42>'
   23 | > inst;
      |   ^
/tmp/issue-88866.cpp:11:8: note: template is declared here
   11 | struct LazyLitMatrix;
      |        ^
2 errors generated.
```

---

### Comment 5 - jcsxky

Would it be
```cpp
template <
    typename...Indices,
    InitFunc<index_by<Indices...>> auto init /* This parameter pack should be expanded */
>
```
? Update as above makes clang accept this code. But gcc accepts both. So I think we need `crash-on-invalid` label.

---

### Comment 6 - cor3ntin

@mizvekov 

---

