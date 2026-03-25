# clang::Expr::IgnoreParenImpCasts() Clang ICE since version 20 with c++20

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/149986
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, concepts, lambda, regression:20
**Closed Date:** 2025-07-23T11:25:34Z

## Body

## Assertion

No assertion given

## Stack dump

```
Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 <source>
1.	<source>:9:54: current parser token ';'
 #0 0x0000000003fc4898 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc4898)
 #1 0x0000000003fc1cc4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc1cc4)
 #2 0x0000000003f06498 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000736e53842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000785c1d4 clang::Expr::IgnoreParenImpCasts() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x785c1d4)
 #5 0x000000000696ac3b calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #6 0x000000000696d659 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #7 0x000000000696dc85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x696dc85)
 #8 0x0000000007047ee7 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (.constprop.0) SemaTemplate.cpp:0:0
 #9 0x0000000007049f7d clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7049f7d)
#10 0x0000000006ba3c37 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ba3c37)
#11 0x0000000006706bf6 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6706bf6)
#12 0x0000000006706e9a clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6706e9a)
#13 0x00000000066eaf0a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66eaf0a)
#14 0x00000000066ea6ce clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea6ce)
#15 0x00000000066ec5f7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ec5f7)
#16 0x00000000066ed796 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ed796)
#17 0x00000000066f76fb clang::Parser::ParseConstraintExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f76fb)
#18 0x000000000679869c clang::Parser::ParseConceptDefinition(clang::Parser::ParsedTemplateInfo const&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x679869c)
#19 0x000000000679f982 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#20 0x000000000679fcea clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x679fcea)
#21 0x00000000066bc820 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66bc820)
#22 0x000000000667942c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667942c)
#23 0x000000000667a315 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667a315)
#24 0x000000000666c07a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666c07a)
#25 0x0000000004972ad8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4972ad8)
#26 0x0000000004c64eb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64eb5)
#27 0x0000000004be1c7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be1c7e)
#28 0x0000000004d58991 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d58991)
#29 0x0000000000d9f10f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f10f)
#30 0x0000000000d95d6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#31 0x00000000049d94a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#32 0x0000000003f06934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f06934)
#33 0x00000000049d9abf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#34 0x000000000499c6bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499c6bd)
#35 0x000000000499d74e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499d74e)
#36 0x00000000049a57b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a57b5)
#37 0x0000000000d9b5bf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b5bf)
#38 0x0000000000c4fa44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4fa44)
#39 0x0000736e53829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#40 0x0000736e53829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#41 0x0000000000d95815 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd95815)
```

## Program

```
template <typename T> concept PerfectSquare = [](){}
([](auto) { return true; }) < PerfectSquare <class T>;
```

To quickly reproduce:

https://godbolt.org/z/955M65Pq6


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Assertion

No assertion given

## Stack dump

```
Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:9:54: current parser token ';'
 #<!-- -->0 0x0000000003fc4898 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc4898)
 #<!-- -->1 0x0000000003fc1cc4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc1cc4)
 #<!-- -->2 0x0000000003f06498 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000736e53842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000000000785c1d4 clang::Expr::IgnoreParenImpCasts() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x785c1d4)
 #<!-- -->5 0x000000000696ac3b calculateConstraintSatisfaction(clang::Sema&amp;, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
 #<!-- -->6 0x000000000696d659 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
 #<!-- -->7 0x000000000696dc85 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x696dc85)
 #<!-- -->8 0x0000000007047ee7 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (.constprop.0) SemaTemplate.cpp:0:0
 #<!-- -->9 0x0000000007049f7d clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7049f7d)
#<!-- -->10 0x0000000006ba3c37 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ba3c37)
#<!-- -->11 0x0000000006706bf6 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6706bf6)
#<!-- -->12 0x0000000006706e9a clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6706e9a)
#<!-- -->13 0x00000000066eaf0a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66eaf0a)
#<!-- -->14 0x00000000066ea6ce clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea6ce)
#<!-- -->15 0x00000000066ec5f7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ec5f7)
#<!-- -->16 0x00000000066ed796 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ed796)
#<!-- -->17 0x00000000066f76fb clang::Parser::ParseConstraintExpression() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f76fb)
#<!-- -->18 0x000000000679869c clang::Parser::ParseConceptDefinition(clang::Parser::ParsedTemplateInfo const&amp;, clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x679869c)
#<!-- -->19 0x000000000679f982 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->20 0x000000000679fcea clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x679fcea)
#<!-- -->21 0x00000000066bc820 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66bc820)
#<!-- -->22 0x000000000667942c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667942c)
#<!-- -->23 0x000000000667a315 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667a315)
#<!-- -->24 0x000000000666c07a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666c07a)
#<!-- -->25 0x0000000004972ad8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4972ad8)
#<!-- -->26 0x0000000004c64eb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64eb5)
#<!-- -->27 0x0000000004be1c7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be1c7e)
#<!-- -->28 0x0000000004d58991 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d58991)
#<!-- -->29 0x0000000000d9f10f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f10f)
#<!-- -->30 0x0000000000d95d6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->31 0x00000000049d94a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->32 0x0000000003f06934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f06934)
#<!-- -->33 0x00000000049d9abf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->34 0x000000000499c6bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499c6bd)
#<!-- -->35 0x000000000499d74e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499d74e)
#<!-- -->36 0x00000000049a57b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a57b5)
#<!-- -->37 0x0000000000d9b5bf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b5bf)
#<!-- -->38 0x0000000000c4fa44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4fa44)
#<!-- -->39 0x0000736e53829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->40 0x0000736e53829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->41 0x0000000000d95815 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd95815)
```

## Program

```
template &lt;typename T&gt; concept PerfectSquare = [](){}
([](auto) { return true; }) &lt; PerfectSquare &lt;class T&gt;;
```

To quickly reproduce:

https://godbolt.org/z/955M65Pq6

</details>


---

### Comment 2 - shafik

@cor3ntin this bisects to 0dedd6fe1409ab02945bb5f903a23335aa79b590

---

