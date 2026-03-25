# Clang  crashes with ICE when concept constraint uses incomplete requires block with ellipsis since version 20 with -std=c++20

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/138820
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, concepts, confirmed, regression:20
**Closed Date:** 2025-05-07T16:39:42Z

## Body

Using a concept with a requires clause that includes an incomplete expression using ellipsis (...) results in an internal compiler error (ICE)

## Assertion 

No assertion arises

## Program

```
int a;
template<typename T>
concept atomicish = requires() {
  {
    a
   ...
  };
};
atomicish<int> f();
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 <source>
1.	<source>:9:16: current parser token 'f'
 #0 0x0000000003a59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a59568)
 #1 0x0000000003a576ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a576ac)
 #2 0x00000000039a69e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070092b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000742ef29 clang::Expr::IgnoreParenImpCasts() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x742ef29)
 #5 0x00000000065c1b26 clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
 #6 0x00000000065c484a CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #7 0x00000000065c4d97 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c4d97)
 #8 0x0000000006ced338 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (.constprop.0) SemaTemplate.cpp:0:0
 #9 0x0000000006ceeb06 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6ceeb06)
#10 0x00000000067f9e35 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x67f9e35)
#11 0x000000000636ab85 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x636ab85)
#12 0x000000000636ae10 clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x636ae10)
#13 0x000000000634ae63 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634ae63)
#14 0x000000000634bf45 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634bf45)
#15 0x000000000634c46a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634c46a)
#16 0x000000000634c639 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634c639)
#17 0x00000000063518d9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63518d9)
#18 0x00000000063625f0 clang::Parser::ParseRequiresExpression() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63625f0)
#19 0x000000000634b3ee clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634b3ee)
#20 0x0000000006359b86 clang::Parser::ParseConstraintExpression() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6359b86)
#21 0x00000000063ee95b clang::Parser::ParseConceptDefinition(clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63ee95b)
#22 0x00000000063f6cf1 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63f6cf1)
#23 0x00000000063f6e4f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63f6e4f)
#24 0x000000000631dd02 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x631dd02)
#25 0x00000000062d39fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d39fa)
#26 0x00000000062d5268 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d5268)
#27 0x00000000062c38ca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62c38ca)
#28 0x00000000043aa295 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43aa295)
#29 0x0000000004682ca1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x4682ca1)
#30 0x000000000460260b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x460260b)
#31 0x000000000476e173 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x476e173)
#32 0x0000000000d116c1 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd116c1)
#33 0x0000000000d09eed ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#34 0x00000000043f5b49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#35 0x00000000039a6e13 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x39a6e13)
#36 0x00000000043f5d69 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#37 0x00000000043b9ebd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43b9ebd)
#38 0x00000000043bae51 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43bae51)
#39 0x00000000043c51ec clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43c51ec)
#40 0x0000000000d0e391 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd0e391)
#41 0x0000000000bd3bc4 main (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xbd3bc4)
#42 0x000070092b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#43 0x000070092b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#44 0x0000000000d09985 _start (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd09985)
```

## To quickly reproduce

It only happens in version 20 and current with -std=c++20 flag 

https://gcc.godbolt.org/z/x47naqv91


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
Using a concept with a requires clause that includes an incomplete expression using ellipsis (...) results in an internal compiler error (ICE)

## Assertion 

No assertion arises

## Program

```
int a;
template&lt;typename T&gt;
concept atomicish = requires() {
  {
    a
   ...
  };
};
atomicish&lt;int&gt; f();
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:9:16: current parser token 'f'
 #<!-- -->0 0x0000000003a59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a59568)
 #<!-- -->1 0x0000000003a576ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a576ac)
 #<!-- -->2 0x00000000039a69e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070092b642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000000000742ef29 clang::Expr::IgnoreParenImpCasts() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x742ef29)
 #<!-- -->5 0x00000000065c1b26 clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator const&amp;) SemaConcept.cpp:0:0
 #<!-- -->6 0x00000000065c484a CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
 #<!-- -->7 0x00000000065c4d97 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x65c4d97)
 #<!-- -->8 0x0000000006ced338 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (.constprop.0) SemaTemplate.cpp:0:0
 #<!-- -->9 0x0000000006ceeb06 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6ceeb06)
#<!-- -->10 0x00000000067f9e35 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x67f9e35)
#<!-- -->11 0x000000000636ab85 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x636ab85)
#<!-- -->12 0x000000000636ae10 clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x636ae10)
#<!-- -->13 0x000000000634ae63 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634ae63)
#<!-- -->14 0x000000000634bf45 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634bf45)
#<!-- -->15 0x000000000634c46a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634c46a)
#<!-- -->16 0x000000000634c639 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634c639)
#<!-- -->17 0x00000000063518d9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63518d9)
#<!-- -->18 0x00000000063625f0 clang::Parser::ParseRequiresExpression() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63625f0)
#<!-- -->19 0x000000000634b3ee clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x634b3ee)
#<!-- -->20 0x0000000006359b86 clang::Parser::ParseConstraintExpression() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x6359b86)
#<!-- -->21 0x00000000063ee95b clang::Parser::ParseConceptDefinition(clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63ee95b)
#<!-- -->22 0x00000000063f6cf1 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63f6cf1)
#<!-- -->23 0x00000000063f6e4f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x63f6e4f)
#<!-- -->24 0x000000000631dd02 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x631dd02)
#<!-- -->25 0x00000000062d39fa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d39fa)
#<!-- -->26 0x00000000062d5268 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62d5268)
#<!-- -->27 0x00000000062c38ca clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x62c38ca)
#<!-- -->28 0x00000000043aa295 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43aa295)
#<!-- -->29 0x0000000004682ca1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x4682ca1)
#<!-- -->30 0x000000000460260b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x460260b)
#<!-- -->31 0x000000000476e173 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x476e173)
#<!-- -->32 0x0000000000d116c1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd116c1)
#<!-- -->33 0x0000000000d09eed ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->34 0x00000000043f5b49 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->35 0x00000000039a6e13 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x39a6e13)
#<!-- -->36 0x00000000043f5d69 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->37 0x00000000043b9ebd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43b9ebd)
#<!-- -->38 0x00000000043bae51 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43bae51)
#<!-- -->39 0x00000000043c51ec clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x43c51ec)
#<!-- -->40 0x0000000000d0e391 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd0e391)
#<!-- -->41 0x0000000000bd3bc4 main (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xbd3bc4)
#<!-- -->42 0x000070092b629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->43 0x000070092b629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->44 0x0000000000d09985 _start (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0xd09985)
```

## To quickly reproduce

It only happens in version 20 and current with -std=c++20 flag 

https://gcc.godbolt.org/z/x47naqv91

</details>


---

