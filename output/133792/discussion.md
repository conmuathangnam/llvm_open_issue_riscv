# Assertion failure in `LocalInstantiationScope::findInstantiationOf`: `isa<LabelDecl>(D) && "declaration not instantiated in this scope"

**Author:** ahatanak
**URL:** https://github.com/llvm/llvm-project/issues/133792

## Body

$ cat test.cpp
```
template <typename Func>
auto foo(Func func, decltype(func()) (*bar)())
    -> decltype(func()) {
  return bar();
}

template <typename Func>
auto foo(Func func, decltype(func()) Value)
    -> decltype(func()) {
  return foo(func, [=] { return Value; });
}

void *foo(void *(*func)()) { return foo(func, nullptr); } 
```

$ clang++ -std=c++20 test.cpp
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -Wextra -Wpedantic <source>
1.	<source>:13:54: current parser token ')'
2.	<source>:13:28: parsing function body 'foo'
3.	<source>:13:28: in compound statement ('{}')
 #0 0x0000000003e64af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e64af8)
 #1 0x0000000003e627b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e627b4)
 #2 0x0000000003daec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079dc61842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000079dc618969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000079dc61842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000079dc618287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000079dc6182871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000079dc61839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007373d65 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7373d65)
#10 0x000000000744ff66 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x744ff66)
#11 0x000000000736b97f (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000073cbd20 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000073cc1fd (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000073a8589 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000073b9e76 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000073a84b6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x000000000739f7b9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#18 0x00000000073a451a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#19 0x00000000073a463c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#20 0x00000000073a5371 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x73a5371)
#21 0x00000000072ae881 CheckDeductionConsistency(clang::Sema&, clang::FunctionTemplateDecl*, int, clang::QualType, clang::QualType, llvm::ArrayRef<clang::TemplateArgument>, bool) SemaTemplateDeduction.cpp:0:0
#22 0x00000000072c0e95 clang::TemplateDeductionResult FinishTemplateArgumentDeduction<isAtLeastAsSpecializedAs(clang::Sema&, clang::SourceLocation, clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::TemplatePartialOrderingContext, llvm::ArrayRef<clang::QualType>, llvm::ArrayRef<clang::QualType>, bool)::'lambda'()::operator()() const::'lambda'(clang::Sema&, clang::FunctionTemplateDecl*, llvm::ArrayRef<clang::TemplateArgument>)>(clang::Sema&, clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, clang::sema::TemplateDeductionInfo&, isAtLeastAsSpecializedAs(clang::Sema&, clang::SourceLocation, clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::TemplatePartialOrderingContext, llvm::ArrayRef<clang::QualType>, llvm::ArrayRef<clang::QualType>, bool)::'lambda'()::operator()() const::'lambda'(clang::Sema&, clang::FunctionTemplateDecl*, llvm::ArrayRef<clang::TemplateArgument>)&&) SemaTemplateDeduction.cpp:0:0
#23 0x00000000072c1c80 void llvm::function_ref<void ()>::callback_fn<isAtLeastAsSpecializedAs(clang::Sema&, clang::SourceLocation, clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::TemplatePartialOrderingContext, llvm::ArrayRef<clang::QualType>, llvm::ArrayRef<clang::QualType>, bool)::'lambda'()>(long) SemaTemplateDeduction.cpp:0:0
#24 0x000000000804d981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x804d981)
#25 0x00000000072d354a isAtLeastAsSpecializedAs(clang::Sema&, clang::SourceLocation, clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::TemplatePartialOrderingContext, llvm::ArrayRef<clang::QualType>, llvm::ArrayRef<clang::QualType>, bool) SemaTemplateDeduction.cpp:0:0
#26 0x00000000072d42ca clang::Sema::getMoreSpecializedTemplate(clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation, clang::TemplatePartialOrderingContext, unsigned int, clang::QualType, clang::QualType, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x72d42ca)
#27 0x00000000071051cb clang::isBetterOverloadCandidate(clang::Sema&, clang::OverloadCandidate const&, clang::OverloadCandidate const&, clang::SourceLocation, clang::OverloadCandidateSet::CandidateSetKind) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x71051cb)
#28 0x000000000710d311 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x710d311)
#29 0x000000000713bdd7 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x713bdd7)
#30 0x0000000006d746c6 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d746c6)
#31 0x0000000006d7553c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d7553c)
#32 0x00000000068c1cb2 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c1cb2)
#33 0x00000000068ba491 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68ba491)
#34 0x00000000068bc5f7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc5f7)
#35 0x00000000068bc689 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc689)
#36 0x00000000068c11f9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c11f9)
#37 0x00000000069382c2 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x69382c2)
#38 0x000000000693b4c5 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693b4c5)
#39 0x000000000693cbf0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693cbf0)
#40 0x000000000693dab5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693dab5)
#41 0x000000000693f2ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693f2ba)
#42 0x0000000006850a53 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6850a53)
#43 0x00000000068856ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68856ad)
#44 0x00000000068447be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68447be)
#45 0x0000000006844f79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6844f79)
#46 0x000000000684c723 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684c723)
#47 0x000000000684d5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684d5fd)
#48 0x000000000683fa5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x683fa5a)
#49 0x00000000047f7e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x47f7e58)
#50 0x0000000004abdd25 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4abdd25)
#51 0x0000000004a407de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4a407de)
#52 0x0000000004babd8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4babd8e)
#53 0x0000000000cfe40f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfe40f)
#54 0x0000000000cf5e9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#55 0x000000000483e749 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#56 0x0000000003daf134 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3daf134)
#57 0x000000000483ed3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#58 0x0000000004801bad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4801bad)
#59 0x0000000004802c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4802c2e)
#60 0x000000000480a765 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x480a765)
#61 0x0000000000cfb213 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfb213)
#62 0x0000000000bc3694 main (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xbc3694)
#63 0x000079dc61829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#64 0x000079dc61829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#65 0x0000000000cf5945 _start (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcf5945)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

https://godbolt.org/z/56MnozeG6

## Comments

### Comment 1 - ahatanak

It looks like the following the assertion started failing after this commit: https://github.com/llvm/llvm-project/commit/aa7497a66c4272669fa63f7ec61a3f01aa9dabaf

@mizvekov

---

### Comment 2 - mizvekov

This could be incidental, clang is known to be bad at keeping track of the parent declaration of lambdas when they appear
in weird places, like function parameters and return type of templated functions.

Just looking at the stack trace, it looks like we think the lambda is at the wrong template depth, which looks like something https://github.com/llvm/llvm-project/pull/107942 would fix.



---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Akira Hatanaka (ahatanak)

<details>
$ cat test.cpp
```
template &lt;typename Func&gt;
auto foo(Func func, decltype(func()) (*bar)())
    -&gt; decltype(func()) {
  return bar();
}

template &lt;typename Func&gt;
auto foo(Func func, decltype(func()) Value)
    -&gt; decltype(func()) {
  return foo(func, [=] { return Value; });
}

void *foo(void *(*func)()) { return foo(func, nullptr); } 
```

$ clang++ -std=c++20 test.cpp
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -Wextra -Wpedantic &lt;source&gt;
1.	&lt;source&gt;:13:54: current parser token ')'
2.	&lt;source&gt;:13:28: parsing function body 'foo'
3.	&lt;source&gt;:13:28: in compound statement ('{}')
 #<!-- -->0 0x0000000003e64af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e64af8)
 #<!-- -->1 0x0000000003e627b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e627b4)
 #<!-- -->2 0x0000000003daec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079dc61842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000079dc618969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000079dc61842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000079dc618287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000079dc6182871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000079dc61839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007373d65 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7373d65)
#<!-- -->10 0x000000000744ff66 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x744ff66)
#<!-- -->11 0x000000000736b97f (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000073cbd20 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000073cc1fd (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x00000000073a8589 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x00000000073b9e76 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x00000000073a84b6 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x000000000739f7b9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x00000000073a451a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x00000000073a463c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x00000000073a5371 clang::Sema::SubstType(clang::QualType, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x73a5371)
#<!-- -->21 0x00000000072ae881 CheckDeductionConsistency(clang::Sema&amp;, clang::FunctionTemplateDecl*, int, clang::QualType, clang::QualType, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool) SemaTemplateDeduction.cpp:0:0
#<!-- -->22 0x00000000072c0e95 clang::TemplateDeductionResult FinishTemplateArgumentDeduction&lt;isAtLeastAsSpecializedAs(clang::Sema&amp;, clang::SourceLocation, clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::TemplatePartialOrderingContext, llvm::ArrayRef&lt;clang::QualType&gt;, llvm::ArrayRef&lt;clang::QualType&gt;, bool)::'lambda'()::operator()() const::'lambda'(clang::Sema&amp;, clang::FunctionTemplateDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;)&gt;(clang::Sema&amp;, clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, clang::sema::TemplateDeductionInfo&amp;, isAtLeastAsSpecializedAs(clang::Sema&amp;, clang::SourceLocation, clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::TemplatePartialOrderingContext, llvm::ArrayRef&lt;clang::QualType&gt;, llvm::ArrayRef&lt;clang::QualType&gt;, bool)::'lambda'()::operator()() const::'lambda'(clang::Sema&amp;, clang::FunctionTemplateDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;)&amp;&amp;) SemaTemplateDeduction.cpp:0:0
#<!-- -->23 0x00000000072c1c80 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;isAtLeastAsSpecializedAs(clang::Sema&amp;, clang::SourceLocation, clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::TemplatePartialOrderingContext, llvm::ArrayRef&lt;clang::QualType&gt;, llvm::ArrayRef&lt;clang::QualType&gt;, bool)::'lambda'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->24 0x000000000804d981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x804d981)
#<!-- -->25 0x00000000072d354a isAtLeastAsSpecializedAs(clang::Sema&amp;, clang::SourceLocation, clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::TemplatePartialOrderingContext, llvm::ArrayRef&lt;clang::QualType&gt;, llvm::ArrayRef&lt;clang::QualType&gt;, bool) SemaTemplateDeduction.cpp:0:0
#<!-- -->26 0x00000000072d42ca clang::Sema::getMoreSpecializedTemplate(clang::FunctionTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation, clang::TemplatePartialOrderingContext, unsigned int, clang::QualType, clang::QualType, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x72d42ca)
#<!-- -->27 0x00000000071051cb clang::isBetterOverloadCandidate(clang::Sema&amp;, clang::OverloadCandidate const&amp;, clang::OverloadCandidate const&amp;, clang::SourceLocation, clang::OverloadCandidateSet::CandidateSetKind) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x71051cb)
#<!-- -->28 0x000000000710d311 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x710d311)
#<!-- -->29 0x000000000713bdd7 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x713bdd7)
#<!-- -->30 0x0000000006d746c6 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d746c6)
#<!-- -->31 0x0000000006d7553c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d7553c)
#<!-- -->32 0x00000000068c1cb2 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c1cb2)
#<!-- -->33 0x00000000068ba491 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68ba491)
#<!-- -->34 0x00000000068bc5f7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc5f7)
#<!-- -->35 0x00000000068bc689 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc689)
#<!-- -->36 0x00000000068c11f9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c11f9)
#<!-- -->37 0x00000000069382c2 clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x69382c2)
#<!-- -->38 0x000000000693b4c5 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693b4c5)
#<!-- -->39 0x000000000693cbf0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693cbf0)
#<!-- -->40 0x000000000693dab5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693dab5)
#<!-- -->41 0x000000000693f2ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693f2ba)
#<!-- -->42 0x0000000006850a53 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6850a53)
#<!-- -->43 0x00000000068856ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68856ad)
#<!-- -->44 0x00000000068447be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68447be)
#<!-- -->45 0x0000000006844f79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6844f79)
#<!-- -->46 0x000000000684c723 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684c723)
#<!-- -->47 0x000000000684d5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684d5fd)
#<!-- -->48 0x000000000683fa5a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x683fa5a)
#<!-- -->49 0x00000000047f7e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x47f7e58)
#<!-- -->50 0x0000000004abdd25 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4abdd25)
#<!-- -->51 0x0000000004a407de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4a407de)
#<!-- -->52 0x0000000004babd8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4babd8e)
#<!-- -->53 0x0000000000cfe40f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfe40f)
#<!-- -->54 0x0000000000cf5e9a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->55 0x000000000483e749 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->56 0x0000000003daf134 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3daf134)
#<!-- -->57 0x000000000483ed3f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->58 0x0000000004801bad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4801bad)
#<!-- -->59 0x0000000004802c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4802c2e)
#<!-- -->60 0x000000000480a765 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x480a765)
#<!-- -->61 0x0000000000cfb213 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfb213)
#<!-- -->62 0x0000000000bc3694 main (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xbc3694)
#<!-- -->63 0x000079dc61829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->64 0x000079dc61829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->65 0x0000000000cf5945 _start (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcf5945)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

https://godbolt.org/z/56MnozeG6
</details>


---

### Comment 4 - ahatanak

clang is still crashing and the crash isn't caused by the lambda in the function. clang crashes compiling the following code too. 

```
template <typename Func>
void foo(Func func, decltype(func()) (*bar)()) {
}

template <typename Func1>
void foo(Func1 func1, decltype(func1()) Value) {
}

void test(void *(*func2)()) { foo2(func2, nullptr); }
```

When `isAtLeastAsSpecializedAs` is called, `FT1` is the first `foo` and `FT2` is the second `foo`.  It looks like `SubstType` is being called to substitute `decltype(func())` and the assertion fails when it's trying to find `func`.

---

### Comment 5 - ahatanak

@mizvekov any thoughts?

---

### Comment 6 - ahatanak

There's a typo in the code in https://github.com/llvm/llvm-project/issues/133792#issuecomment-2975100321. `test` should be calling `foo`, not `foo2`.

---

### Comment 7 - mizvekov

Yeah, I was wrong.

This looks like a simpler problem, we are not pushing the function parameters into the local instantiation scope when performing the substitutions for the partial ordering non-deduced consistency checks.

---

### Comment 8 - mizvekov

This needs to change the function template version of `getMoreSpecializedTemplate` and all of its helpers so they pass around and substitute the whole parameter's ParmVarDecls on the parameter side, instead of just their types. This will make sure they are available in the local instantiation scope for subsequent parameters to use. We can keep passing just the types for the argument side, since these are not substituted.

This needs to be done in declaration order, and all parameters need to be transformed, not just the ones which have a correspondence in the argument side, since they may still be referenced from the type of other parameters, even if they don't directly participate in deduction.

This needs to deal with incomplete substitutions and all of that, if substitution fails for a parameter which is not directly used in deduction, this should only fail subsequent substitutions which depend on it.

---

### Comment 9 - mizvekov

Assigning myself so I don't lose track of this bug and pick it up at some point, in case no one else does first.
I will likely not be working on it so soon, so feel free to pick it up.

---

