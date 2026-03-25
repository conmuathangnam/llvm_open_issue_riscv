# Clang assertion failure in CheckNonNullExpr

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/138371
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-07-15T17:06:46Z

## Body

https://gcc.godbolt.org/z/5b63E1sjs
```c++
namespace absl {
template <typename T>
struct StatusOr {
  const T operator*() const;
};
}  // namespace absl
#define ASSIGN_OR_RETURN(...)                                                \
  STATUS_MACROS_IMPL_GET_VARIADIC_((return_keyword, __VA_ARGS__,             \
                                    STATUS_MACROS_IMPL_ASSIGN_OR_RETURN_3_,  \
                                    STATUS_MACROS_IMPL_ASSIGN_OR_RETURN_2_)) \
  (return_keyword, __VA_ARGS__)
#define STATUS_MACROS_IMPL_GET_VARIADIC_HELPER_(_1, _2, _3, _4, NAME, ...) NAME
#define STATUS_MACROS_IMPL_GET_VARIADIC_(args) \
  STATUS_MACROS_IMPL_GET_VARIADIC_HELPER_ args
#define STATUS_MACROS_IMPL_ASSIGN_OR_RETURN_2_(return_keyword, lhs, rexpr) \
  auto statusor = rexpr;                                                   \
  STATUS_MACROS_IMPL_UNPARENTHESIZE_IF_PARENTHESIZED(lhs) = *statusor
#define STATUS_MACROS_IMPL_UNPARENTHESIZE_IF_PARENTHESIZED(...) __VA_ARGS__

namespace x {
struct Loc {
  int operator()(const char* _Nonnull f = __builtin_FILE()) const;
};
void f() {
  [](auto...) {
    ASSIGN_OR_RETURN(int x, [&] {
      Loc()();
      return absl::StatusOr<int>{};
    }());
  }();
}
}  // namespace x
```

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Basic/SourceLocation.h:328: const char* clang::PresumedLoc::getFilename() const: Assertion `isValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:30:5: current parser token ')'
2.	<source>:20:1: parsing namespace 'x'
3.	<source>:24:10: parsing function body 'x::f'
4.	<source>:24:10: in compound statement ('{}')
5.	<source>:25:3: instantiating function definition 'x::f()::(anonymous class)::operator()<>'
 #0 0x0000000003f6f428 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6f428)
 #1 0x0000000003f6d0b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6d0b4)
 #2 0x0000000003eb2098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007328b1042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007328b10969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007328b1042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007328b10287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007328b102871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007328b1039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000782f221 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x782f221)
#10 0x000000000787b148 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x000000000787aad7 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x000000000787b92b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#13 0x0000000007863c45 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#14 0x000000000786ba1b EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#15 0x000000000786ec19 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x786ec19)
#16 0x000000000786f055 clang::Expr::EvaluateAsBooleanCondition(bool&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x786f055)
#17 0x0000000006819e5b CheckNonNullExpr(clang::Sema&, clang::Expr const*) (.isra.0) SemaChecking.cpp:0:0
#18 0x000000000681a270 CheckNonNullArgument(clang::Sema&, clang::Expr const*, clang::SourceLocation) SemaChecking.cpp:0:0
#19 0x00000000068647e2 clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, bool, clang::SourceLocation, clang::SourceRange, clang::VariadicCallType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68647e2)
#20 0x0000000006865780 clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6865780)
#21 0x0000000006f3bcfa clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3bcfa)
#22 0x0000000006b32f7e clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b32f7e)
#23 0x0000000006b3387e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3387e)
#24 0x00000000071b0f9c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXOperatorCallExpr(clang::CXXOperatorCallExpr*) SemaTemplateInstantiate.cpp:0:0
#25 0x00000000071846d4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#26 0x000000000718480c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#27 0x00000000071c4f47 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#28 0x00000000071c57ce clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#29 0x00000000071836fe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#30 0x00000000071842e2 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#31 0x000000000718486f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#32 0x0000000007191186 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#33 0x00000000071846e8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#34 0x00000000071865c5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#35 0x0000000007186bdb clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7186bdb)
#36 0x00000000071db816 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71db816)
#37 0x0000000007229cb0 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7229cb0)
#38 0x0000000007235267 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7235267)
#39 0x000000000723a9b4 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#40 0x0000000007ec5ca1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec5ca1)
#41 0x00000000071d969a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71d969a)
#42 0x000000000717db7d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#43 0x00000000071c57ce clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#44 0x00000000071ceb74 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71ceb74)
#45 0x000000000722dfd4 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722dfd4)
#46 0x0000000007ec5ca1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec5ca1)
#47 0x00000000070bd68a clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70bd68a)
#48 0x0000000006aea563 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aea563)
#49 0x0000000006ef97d7 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#50 0x0000000006f3b9a6 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3b9a6)
#51 0x0000000006b32f7e clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b32f7e)
#52 0x0000000006b3387e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3387e)
#53 0x00000000066662fd clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66662fd)
#54 0x000000000665ed0a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665ed0a)
#55 0x0000000006660ea7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6660ea7)
#56 0x0000000006660f39 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6660f39)
#57 0x0000000006665919 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6665919)
#58 0x00000000066e97b9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e97b9)
#59 0x00000000066e1272 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1272)
#60 0x00000000066e219d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e219d)
#61 0x00000000066e9eb3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e9eb3)
#62 0x00000000066ea67a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea67a)
#63 0x00000000065f2bf3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f2bf3)
#64 0x000000000662926d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662926d)
#65 0x00000000065e672e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e672e)
#66 0x00000000065e6ee9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e6ee9)
#67 0x00000000065ee80a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee80a)
#68 0x000000000664c154 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664c154)
#69 0x000000000664ce16 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664ce16)
#70 0x0000000006631bf0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631bf0)
#71 0x00000000065ee8bc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee8bc)
#72 0x00000000065ef7ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ef7ad)
#73 0x00000000065e1b8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e1b8a)
#74 0x0000000004910eb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4910eb8)
#75 0x0000000004c03675 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c03675)
#76 0x0000000004b82cfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b82cfe)
#77 0x0000000004cf74f9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf74f9)
#78 0x0000000000da554f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda554f)
#79 0x0000000000d9b71a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#80 0x0000000004977769 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#81 0x0000000003eb2534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb2534)
#82 0x0000000004977d7f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#83 0x000000000493a1cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493a1cd)
#84 0x000000000493b25e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493b25e)
#85 0x00000000049430b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49430b5)
#86 0x0000000000da14b8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda14b8)
#87 0x0000000000c26ce4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc26ce4)
#88 0x00007328b1029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#89 0x00007328b1029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#90 0x0000000000d9b1c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b1c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - alexfh

Sam, I saw your relatively recent changes in `CheckNonNullExpr`, which might be related. Could you take a look at this?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexander Kornienko (alexfh)

<details>
https://gcc.godbolt.org/z/5b63E1sjs
```
namespace absl {
template &lt;typename T&gt;
struct StatusOr {
  const T operator*() const;
};
}  // namespace absl
#define ASSIGN_OR_RETURN(...)                                                \
  STATUS_MACROS_IMPL_GET_VARIADIC_((return_keyword, __VA_ARGS__,             \
                                    STATUS_MACROS_IMPL_ASSIGN_OR_RETURN_3_,  \
                                    STATUS_MACROS_IMPL_ASSIGN_OR_RETURN_2_)) \
  (return_keyword, __VA_ARGS__)
#define STATUS_MACROS_IMPL_GET_VARIADIC_HELPER_(_1, _2, _3, _4, NAME, ...) NAME
#define STATUS_MACROS_IMPL_GET_VARIADIC_(args) \
  STATUS_MACROS_IMPL_GET_VARIADIC_HELPER_ args
#define STATUS_MACROS_IMPL_ASSIGN_OR_RETURN_2_(return_keyword, lhs, rexpr) \
  auto statusor = rexpr;                                                   \
  STATUS_MACROS_IMPL_UNPARENTHESIZE_IF_PARENTHESIZED(lhs) = *statusor
#define STATUS_MACROS_IMPL_UNPARENTHESIZE_IF_PARENTHESIZED(...) __VA_ARGS__

namespace x {
struct Loc {
  int operator()(const char* _Nonnull f = __builtin_FILE()) const;
};
void f() {
  [](auto...) {
    ASSIGN_OR_RETURN(int x, [&amp;] {
      Loc()();
      return absl::StatusOr&lt;int&gt;{};
    }());
  }();
}
}  // namespace x
```

```
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Basic/SourceLocation.h:328: const char* clang::PresumedLoc::getFilename() const: Assertion `isValid()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:30:5: current parser token ')'
2.	&lt;source&gt;:20:1: parsing namespace 'x'
3.	&lt;source&gt;:24:10: parsing function body 'x::f'
4.	&lt;source&gt;:24:10: in compound statement ('{}')
5.	&lt;source&gt;:25:3: instantiating function definition 'x::f()::(anonymous class)::operator()&lt;&gt;'
 #<!-- -->0 0x0000000003f6f428 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6f428)
 #<!-- -->1 0x0000000003f6d0b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6d0b4)
 #<!-- -->2 0x0000000003eb2098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007328b1042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007328b10969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007328b1042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007328b10287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007328b102871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007328b1039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000782f221 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x782f221)
#<!-- -->10 0x000000000787b148 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->11 0x000000000787aad7 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->12 0x000000000787b92b EvaluatePointer(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007863c45 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x000000000786ba1b EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->15 0x000000000786ec19 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x786ec19)
#<!-- -->16 0x000000000786f055 clang::Expr::EvaluateAsBooleanCondition(bool&amp;, clang::ASTContext const&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x786f055)
#<!-- -->17 0x0000000006819e5b CheckNonNullExpr(clang::Sema&amp;, clang::Expr const*) (.isra.0) SemaChecking.cpp:0:0
#<!-- -->18 0x000000000681a270 CheckNonNullArgument(clang::Sema&amp;, clang::Expr const*, clang::SourceLocation) SemaChecking.cpp:0:0
#<!-- -->19 0x00000000068647e2 clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::SourceLocation, clang::SourceRange, clang::VariadicCallType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68647e2)
#<!-- -->20 0x0000000006865780 clang::Sema::CheckFunctionCall(clang::FunctionDecl*, clang::CallExpr*, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6865780)
#<!-- -->21 0x0000000006f3bcfa clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3bcfa)
#<!-- -->22 0x0000000006b32f7e clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b32f7e)
#<!-- -->23 0x0000000006b3387e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3387e)
#<!-- -->24 0x00000000071b0f9c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXOperatorCallExpr(clang::CXXOperatorCallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x00000000071846d4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x000000000718480c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->27 0x00000000071c4f47 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->28 0x00000000071c57ce clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->29 0x00000000071836fe clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->30 0x00000000071842e2 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->31 0x000000000718486f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->32 0x0000000007191186 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->33 0x00000000071846e8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->34 0x00000000071865c5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->35 0x0000000007186bdb clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7186bdb)
#<!-- -->36 0x00000000071db816 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71db816)
#<!-- -->37 0x0000000007229cb0 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7229cb0)
#<!-- -->38 0x0000000007235267 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef&lt;clang::BindingDecl*&gt;*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7235267)
#<!-- -->39 0x000000000723a9b4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->40 0x0000000007ec5ca1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec5ca1)
#<!-- -->41 0x00000000071d969a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71d969a)
#<!-- -->42 0x000000000717db7d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->43 0x00000000071c57ce clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->44 0x00000000071ceb74 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71ceb74)
#<!-- -->45 0x000000000722dfd4 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x722dfd4)
#<!-- -->46 0x0000000007ec5ca1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec5ca1)
#<!-- -->47 0x00000000070bd68a clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70bd68a)
#<!-- -->48 0x0000000006aea563 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aea563)
#<!-- -->49 0x0000000006ef97d7 CreateFunctionRefExpr(clang::Sema&amp;, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&amp;) SemaOverload.cpp:0:0
#<!-- -->50 0x0000000006f3b9a6 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3b9a6)
#<!-- -->51 0x0000000006b32f7e clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b32f7e)
#<!-- -->52 0x0000000006b3387e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3387e)
#<!-- -->53 0x00000000066662fd clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66662fd)
#<!-- -->54 0x000000000665ed0a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665ed0a)
#<!-- -->55 0x0000000006660ea7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6660ea7)
#<!-- -->56 0x0000000006660f39 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6660f39)
#<!-- -->57 0x0000000006665919 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6665919)
#<!-- -->58 0x00000000066e97b9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e97b9)
#<!-- -->59 0x00000000066e1272 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1272)
#<!-- -->60 0x00000000066e219d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e219d)
#<!-- -->61 0x00000000066e9eb3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e9eb3)
#<!-- -->62 0x00000000066ea67a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea67a)
#<!-- -->63 0x00000000065f2bf3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f2bf3)
#<!-- -->64 0x000000000662926d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662926d)
#<!-- -->65 0x00000000065e672e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e672e)
#<!-- -->66 0x00000000065e6ee9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e6ee9)
#<!-- -->67 0x00000000065ee80a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee80a)
#<!-- -->68 0x000000000664c154 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664c154)
#<!-- -->69 0x000000000664ce16 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664ce16)
#<!-- -->70 0x0000000006631bf0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631bf0)
#<!-- -->71 0x00000000065ee8bc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ee8bc)
#<!-- -->72 0x00000000065ef7ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65ef7ad)
#<!-- -->73 0x00000000065e1b8a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e1b8a)
#<!-- -->74 0x0000000004910eb8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4910eb8)
#<!-- -->75 0x0000000004c03675 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c03675)
#<!-- -->76 0x0000000004b82cfe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b82cfe)
#<!-- -->77 0x0000000004cf74f9 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf74f9)
#<!-- -->78 0x0000000000da554f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda554f)
#<!-- -->79 0x0000000000d9b71a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->80 0x0000000004977769 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->81 0x0000000003eb2534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eb2534)
#<!-- -->82 0x0000000004977d7f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->83 0x000000000493a1cd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493a1cd)
#<!-- -->84 0x000000000493b25e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493b25e)
#<!-- -->85 0x00000000049430b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49430b5)
#<!-- -->86 0x0000000000da14b8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda14b8)
#<!-- -->87 0x0000000000c26ce4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc26ce4)
#<!-- -->88 0x00007328b1029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->89 0x00007328b1029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->90 0x0000000000d9b1c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b1c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - AaronBallman

The issue is here: https://github.com/llvm/llvm-project/blob/c632ac3506731e394dd7e3ac3f3320f5442250b4/clang/lib/Sema/TreeTransform.h#L14025

The problem is that the end location is within a macro expansion, so `getLocForEndOfToken()` receives a valid source location but returns an invalid one: https://github.com/llvm/llvm-project/blob/c632ac3506731e394dd7e3ac3f3320f5442250b4/clang/lib/Lex/Lexer.cpp#L857

This can be hacked around via something like:
```
    // FIXME: Poor location information
    SourceLocation Loc = static_cast<Expr *>(Object.get())->getEndLoc();
    SourceLocation FakeLParenLoc = SemaRef.getLocForEndOfToken(Loc);
    if (FakeLParenLoc.isInvalid() && Loc.isValid())
      FakeLParenLoc = Loc;
```
but a better fix would be to track the lparen location on `CallExpr` instead of just the rparen location. CUDA call expressions have a similar hack.

---

### Comment 4 - alexfh

Assigning to @AaronBallman who seems to have the most context here at the moment. 

---

### Comment 5 - AaronBallman

This looks to be a regression in Clang 21, so I'll try to take a look.

---

### Comment 6 - AaronBallman

Oops, I take that back, not a regression, it requires an asserts build. I can see the assertion as far back as Clang 9

---

### Comment 7 - AaronBallman

A smaller reproducer is:
```
#define ASSIGN_OR_RETURN(...)  (__VA_ARGS__)

struct Loc {
  int operator()(const char* _Nonnull f = __builtin_FILE()) const;
};

template <typename Ty>
void f() {
  ASSIGN_OR_RETURN(Loc()());
}

void test() {
  f<int>();
}
```

---

