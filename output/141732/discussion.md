# [clang] Failed to compile simple and legal code while gcc is ok

**Author:** yaoxinliu
**URL:** https://github.com/llvm/llvm-project/issues/141732
**Status:** Closed
**Labels:** clang:frontend, concepts, lambda, diverges-from:gcc, regression:16
**Closed Date:** 2025-11-25T14:06:05Z

## Body

See: [https://godbolt.org/z/GqbzYfa78](https://godbolt.org/z/GqbzYfa78)
Related Issue: [https://github.com/llvm/llvm-project/issues/141693](https://github.com/llvm/llvm-project/issues/141693)

```
template<typename T>
concept IsOK =
    []<typename U> consteval -> bool { return requires { requires sizeof(U) > 1; }; }.template operator()<T>();

static_assert(IsOK<int>);

int main() {
}
```
**gcc is ok, but clang gives errors as follows:**

```
<source>:3:67: error: substitution into constraint expression resulted in a non-constant expression
    3 |     []<typename U> consteval -> bool { return requires { requires sizeof(U) > 1; }; }.template operator()<T>();
      |                                                                   ^~~~~~~~~~~~~
<source>:3:67: note: while checking the satisfaction of nested requirement requested here
    3 |     []<typename U> consteval -> bool { return requires { requires sizeof(U) > 1; }; }.template operator()<T>();
      |                                                                   ^~~~~~~~~~~~~
<source>:3:67: note: in instantiation of requirement here
    3 |     []<typename U> consteval -> bool { return requires { requires sizeof(U) > 1; }; }.template operator()<T>();
      |                                                                   ^~~~~~~~~~~~~
<source>:3:67: note: while checking the satisfaction of nested requirement requested here
    3 |     []<typename U> consteval -> bool { return requires { requires sizeof(U) > 1; }; }.template operator()<T>();
      |                                                                   ^~~~~~~~~~~~~
<source>:3:38: note: while substituting into a lambda expression here
    3 |     []<typename U> consteval -> bool { return requires { requires sizeof(U) > 1; }; }.template operator()<T>();
      |                                      ^
<source>:3:5: note: while substituting template arguments into constraint expression here
    3 |     []<typename U> consteval -> bool { return requires { requires sizeof(U) > 1; }; }.template operator()<T>();
      |     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
<source>:5:15: note: while checking the satisfaction of concept 'IsOK<int>' requested here
    5 | static_assert(IsOK<int>);
      |               ^~~~~~~~~
<source>:3:67: note: subexpression not valid in a constant expression
    3 |     []<typename U> consteval -> bool { return requires { requires sizeof(U) > 1; }; }.template operator()<T>();
      |                                                                   ^
<source>:5:15: error: static assertion failed
    5 | static_assert(IsOK<int>);
      |               ^~~~~~~~~
<source>:5:15: note: because 'int' does not satisfy 'IsOK'
<source>:3:5: note: because '[]<typename U>() -> bool {
    return requires { requires <<error-expression>>; };
}.template operator()<int>()' evaluated to false
    3 |     []<typename U> consteval -> bool { return requires { requires sizeof(U) > 1; }; }.template operator()<T>();
      |     ^
2 errors generated.
Compiler returned: 1

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (yaoxinliu)

<details>
See: [https://godbolt.org/z/GqbzYfa78](https://godbolt.org/z/GqbzYfa78)

```
template&lt;typename T&gt;
concept IsOK =
    []&lt;typename U&gt; consteval -&gt; bool { return requires { requires sizeof(U) &gt; 1; }; }.template operator()&lt;T&gt;();

static_assert(IsOK&lt;int&gt;);

int main() {
}
```
**gcc is ok, but clang gives errors as follows:**

```
&lt;source&gt;:3:67: error: substitution into constraint expression resulted in a non-constant expression
    3 |     []&lt;typename U&gt; consteval -&gt; bool { return requires { requires sizeof(U) &gt; 1; }; }.template operator()&lt;T&gt;();
      |                                                                   ^~~~~~~~~~~~~
&lt;source&gt;:3:67: note: while checking the satisfaction of nested requirement requested here
    3 |     []&lt;typename U&gt; consteval -&gt; bool { return requires { requires sizeof(U) &gt; 1; }; }.template operator()&lt;T&gt;();
      |                                                                   ^~~~~~~~~~~~~
&lt;source&gt;:3:67: note: in instantiation of requirement here
    3 |     []&lt;typename U&gt; consteval -&gt; bool { return requires { requires sizeof(U) &gt; 1; }; }.template operator()&lt;T&gt;();
      |                                                                   ^~~~~~~~~~~~~
&lt;source&gt;:3:67: note: while checking the satisfaction of nested requirement requested here
    3 |     []&lt;typename U&gt; consteval -&gt; bool { return requires { requires sizeof(U) &gt; 1; }; }.template operator()&lt;T&gt;();
      |                                                                   ^~~~~~~~~~~~~
&lt;source&gt;:3:38: note: while substituting into a lambda expression here
    3 |     []&lt;typename U&gt; consteval -&gt; bool { return requires { requires sizeof(U) &gt; 1; }; }.template operator()&lt;T&gt;();
      |                                      ^
&lt;source&gt;:3:5: note: while substituting template arguments into constraint expression here
    3 |     []&lt;typename U&gt; consteval -&gt; bool { return requires { requires sizeof(U) &gt; 1; }; }.template operator()&lt;T&gt;();
      |     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
&lt;source&gt;:5:15: note: while checking the satisfaction of concept 'IsOK&lt;int&gt;' requested here
    5 | static_assert(IsOK&lt;int&gt;);
      |               ^~~~~~~~~
&lt;source&gt;:3:67: note: subexpression not valid in a constant expression
    3 |     []&lt;typename U&gt; consteval -&gt; bool { return requires { requires sizeof(U) &gt; 1; }; }.template operator()&lt;T&gt;();
      |                                                                   ^
&lt;source&gt;:5:15: error: static assertion failed
    5 | static_assert(IsOK&lt;int&gt;);
      |               ^~~~~~~~~
&lt;source&gt;:5:15: note: because 'int' does not satisfy 'IsOK'
&lt;source&gt;:3:5: note: because '[]&lt;typename U&gt;() -&gt; bool {
    return requires { requires &lt;&lt;error-expression&gt;&gt;; };
}.template operator()&lt;int&gt;()' evaluated to false
    3 |     []&lt;typename U&gt; consteval -&gt; bool { return requires { requires sizeof(U) &gt; 1; }; }.template operator()&lt;T&gt;();
      |     ^
2 errors generated.
Compiler returned: 1

```
</details>


---

### Comment 2 - shafik

This crashes on assertion build, this is bug: https://godbolt.org/z/zKdascM9z

Looks like the crash goes back to clang-16.

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:16983:
bool clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, const clang::ASTContext&, clang::Expr::ConstantExprKind) const:
Assertion `!isValueDependent() && "Expression evaluator can't be called on a dependent expression."' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<source>:5:24: current parser token ')'
 #0 0x0000000003fbf178 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fbf178)
 #1 0x0000000003fbce04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fbce04)
 #2 0x0000000003f01a38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ffb82c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ffb82c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ffb82c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ffb82c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ffb82c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ffb82c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000796d6ba clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x796d6ba)
#10 0x0000000006962619 calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#11 0x0000000006963d38 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#12 0x0000000006964212 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6964212)
#13 0x0000000007250d39 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000072522d6 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000072191a9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000721b045 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#17 0x000000000721b3ec clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#18 0x000000000725980e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000007217f56 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#20 0x0000000007218a62 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000007218fe1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#22 0x000000000724741c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXDependentScopeMemberExpr(clang::CXXDependentScopeMemberExpr*) SemaTemplateInstantiate.cpp:0:0
#23 0x0000000007218d8f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#24 0x0000000007225966 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#25 0x0000000007218e5a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#26 0x0000000007228b52 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7228b52)
#27 0x0000000006961d3f calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#28 0x0000000006963d38 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#29 0x0000000006964355 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6964355)
#30 0x00000000070c03ab clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (.constprop.0) SemaTemplate.cpp:0:0
#31 0x00000000070c242d clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70c242d)
#32 0x0000000006b99288 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b99288)
#33 0x00000000067002e8 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67002e8)
#34 0x000000000670060a clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x670060a)
#35 0x00000000066e2e92 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e2e92)
#36 0x00000000066e264e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e264e)
#37 0x00000000066f054e clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f054e)
#38 0x00000000066c50db clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c50db)
#39 0x00000000066b3ae6 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b3ae6)
#40 0x000000000667056c clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667056c)
#41 0x000000000667145d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667145d)
#42 0x000000000666382a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666382a)
#43 0x0000000004973df8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4973df8)
#44 0x0000000004c6c5e5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6c5e5)
#45 0x0000000004beaa2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4beaa2e)
#46 0x0000000004d5ec49 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d5ec49)
#47 0x0000000000db329f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb329f)
#48 0x0000000000da9eba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#49 0x00000000049db229 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#50 0x0000000003f01ed4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f01ed4)
#51 0x00000000049db83f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#52 0x000000000499d60d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499d60d)
#53 0x000000000499e69e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499e69e)
#54 0x00000000049a6985 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a6985)
#55 0x0000000000daf7af clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf7af)
#56 0x0000000000c34b94 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc34b94)
#57 0x00007ffb82c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#58 0x00007ffb82c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#59 0x0000000000da9965 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda9965)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - shafik

@usx95 git bisect links this regression to b3ce87285186ba190103a90b0b49da2e45fb7d1f

CC @zyn0217 @erichkeane 

---

### Comment 4 - cor3ntin

FYI, I'm refactoring that area of code, I'll look into this next week

---

### Comment 5 - shafik

ping @cor3ntin 

---

### Comment 6 - Taw3e8

@cor3ntin this seems to work now on trunk

---

### Comment 7 - cor3ntin

Fixed in trunk

---

