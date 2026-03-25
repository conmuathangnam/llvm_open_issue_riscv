# [clang] Assertion `Ty->isArrayType()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/180893
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid
**Closed Date:** 2026-02-13T13:38:37Z

## Body

Reproducer:
https://godbolt.org/z/s5z7b9bzY
```c
int foo[_Countof(void)];
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:18450: bool {anonymous}::IntExprEvaluator::VisitUnaryExprOrTypeTraitExpr(const clang::UnaryExprOrTypeTraitExpr*): Assertion `Ty->isArrayType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c <source>
1.	<source>:1:24: current parser token ';'
 #0 0x00000000043bf928 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43bf928)
 #1 0x00000000043bcd84 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43bcd84)
 #2 0x00000000042fd0c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007dc4b8e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007dc4b8e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007dc4b8e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007dc4b8e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007dc4b8e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007dc4b8e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007ea3b9c (anonymous namespace)::IntExprEvaluator::VisitUnaryExprOrTypeTraitExpr(clang::UnaryExprOrTypeTraitExpr const*) ExprConstant.cpp:0:0
#10 0x0000000007e8feb0 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x0000000007e9869e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#12 0x0000000007e98f9b EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#13 0x0000000007e9d2aa clang::Expr::EvaluateKnownConstIntCheckOverflow(clang::ASTContext const&, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e9d2aa)
#14 0x00000000070d5b47 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70d5b47)
#15 0x00000000077c84a7 clang::Sema::BuildArrayType(clang::QualType, clang::ArraySizeModifier, clang::Expr*, unsigned int, clang::SourceRange, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77c84a7)
#16 0x00000000077e53c2 GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#17 0x00000000077eb0ef clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77eb0ef)
#18 0x0000000006f6f5cc clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f6f5cc)
#19 0x0000000006f70910 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f70910)
#20 0x0000000006ba3cc3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ba3cc3)
#21 0x0000000006bb1aea clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bb1aea)
#22 0x0000000006b6f11b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6f11b)
#23 0x0000000006b6f90f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6f90f)
#24 0x0000000006b77911 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b77911)
#25 0x0000000006b78825 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b78825)
#26 0x0000000006b78c10 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b78c10)
#27 0x0000000006b59503 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b59503)
#28 0x0000000004d37868 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d37868)
#29 0x0000000005035335 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5035335)
#30 0x0000000004fb46ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fb46ae)
#31 0x00000000051309dd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51309dd)
#32 0x0000000000de849e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde849e)
#33 0x0000000000ddee6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x0000000000ddefed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#35 0x0000000004dab2f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x00000000042fd564 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fd564)
#37 0x0000000004dab90f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x0000000004d6c322 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6c322)
#39 0x0000000004d6d2ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6d2ce)
#40 0x0000000004d74585 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d74585)
#41 0x0000000000de4811 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4811)
#42 0x0000000000c9ecf4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9ecf4)
#43 0x00007dc4b8e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x00007dc4b8e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000dde905 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde905)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/s5z7b9bzY
```c
int foo[_Countof(void)];
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:18450: bool {anonymous}::IntExprEvaluator::VisitUnaryExprOrTypeTraitExpr(const clang::UnaryExprOrTypeTraitExpr*): Assertion `Ty-&gt;isArrayType()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c &lt;source&gt;
1.	&lt;source&gt;:1:24: current parser token ';'
 #<!-- -->0 0x00000000043bf928 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43bf928)
 #<!-- -->1 0x00000000043bcd84 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43bcd84)
 #<!-- -->2 0x00000000042fd0c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007dc4b8e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007dc4b8e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007dc4b8e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007dc4b8e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007dc4b8e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007dc4b8e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007ea3b9c (anonymous namespace)::IntExprEvaluator::VisitUnaryExprOrTypeTraitExpr(clang::UnaryExprOrTypeTraitExpr const*) ExprConstant.cpp:0:0
#<!-- -->10 0x0000000007e8feb0 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007e9869e Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007e98f9b EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007e9d2aa clang::Expr::EvaluateKnownConstIntCheckOverflow(clang::ASTContext const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e9d2aa)
#<!-- -->14 0x00000000070d5b47 clang::Sema::VerifyIntegerConstantExpression(clang::Expr*, llvm::APSInt*, clang::Sema::VerifyICEDiagnoser&amp;, clang::AllowFoldKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70d5b47)
#<!-- -->15 0x00000000077c84a7 clang::Sema::BuildArrayType(clang::QualType, clang::ArraySizeModifier, clang::Expr*, unsigned int, clang::SourceRange, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77c84a7)
#<!-- -->16 0x00000000077e53c2 GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&amp;, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#<!-- -->17 0x00000000077eb0ef clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77eb0ef)
#<!-- -->18 0x0000000006f6f5cc clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f6f5cc)
#<!-- -->19 0x0000000006f70910 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f70910)
#<!-- -->20 0x0000000006ba3cc3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ba3cc3)
#<!-- -->21 0x0000000006bb1aea clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bb1aea)
#<!-- -->22 0x0000000006b6f11b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6f11b)
#<!-- -->23 0x0000000006b6f90f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b6f90f)
#<!-- -->24 0x0000000006b77911 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b77911)
#<!-- -->25 0x0000000006b78825 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b78825)
#<!-- -->26 0x0000000006b78c10 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b78c10)
#<!-- -->27 0x0000000006b59503 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b59503)
#<!-- -->28 0x0000000004d37868 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d37868)
#<!-- -->29 0x0000000005035335 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5035335)
#<!-- -->30 0x0000000004fb46ae clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fb46ae)
#<!-- -->31 0x00000000051309dd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x51309dd)
#<!-- -->32 0x0000000000de849e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde849e)
#<!-- -->33 0x0000000000ddee6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->34 0x0000000000ddefed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004dab2f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x00000000042fd564 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fd564)
#<!-- -->37 0x0000000004dab90f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x0000000004d6c322 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6c322)
#<!-- -->39 0x0000000004d6d2ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d6d2ce)
#<!-- -->40 0x0000000004d74585 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d74585)
#<!-- -->41 0x0000000000de4811 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4811)
#<!-- -->42 0x0000000000c9ecf4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc9ecf4)
#<!-- -->43 0x00007dc4b8e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x00007dc4b8e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000dde905 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdde905)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21:
https://godbolt.org/z/ovbvqYY9E

---

### Comment 3 - Aadarsh-Keshri

I’m interested in working on this as my first contribution!
I’ll start looking into the crash and keep updating my progress here.

---

### Comment 4 - Aadarsh-Keshri

I was able to reproduce this issue.
I would appreciate any guidance on the right direction to proceed.

---

### Comment 5 - csxplorer

[For GCC compatibility](https://godbolt.org/z/ro63TenEj), the compiler should raise a `err_countof_arg_not_array_type`.

In a simple sample:
```cpp
int x = _Countof(void);
```
In [`clang/lib/Sema/SemaExpr.cpp`](https://github.com/llvm/llvm-project/blob/99c9e5ebd6c7c5564190133b3171b53f17b7f3f8/clang/lib/Sema/SemaExpr.cpp), the function `Sema::CheckUnaryExprOrTypeTraitOperand` is expected to return at Line 4653, but actually returns at Line 4637.

```cpp
  // Explicitly list some types as extensions.
  if (!CheckExtensionTraitOperandType(*this, ExprType, OpLoc, ExprRange,
                                      ExprKind))
    return false;  // Line 4637, where the function actually returns


  if (RequireCompleteSizedType(
          OpLoc, ExprType, diag::err_sizeof_alignof_incomplete_or_sizeless_type,
          KWName, ExprRange))
    return true;


  if (ExprType->isFunctionType()) {
    Diag(OpLoc, diag::err_sizeof_alignof_function_type) << KWName << ExprRange;
    return true;
  }


  if (ExprKind == UETT_CountOf) {
    // The type has to be an array type. We already checked for incomplete
    // types above.
    if (!ExprType->isArrayType()) {
      Diag(OpLoc, diag::err_countof_arg_not_array_type) << ExprType;
      return true;  // Line 4653, expected to return
    }
  }

```

---

### Comment 6 - Aadarsh-Keshri

@csxplorer Thank you for pointing out that part of the code. I'll also keep GCC compatibility in mind.

---

### Comment 7 - csxplorer

@Aadarsh-Keshri Maybe changing line 4239 of `clang/lib/Sema/SemaExpr.cpp` can solve the problem:
```cpp
-  if (T->isVoidType()) {
+  if (T->isVoidType() && TraitKind != UETT_CountOf) {
```

Though this will raise `err_sizeof_alignof_incomplete_or_sizeless_type` but not `err_countof_arg_not_array_type`. I also tried other ways, but they seems to be complicated.

However, I'm not sure if this fix will cause unexpected problems.

---

### Comment 8 - shafik

This is not a regression is goes back to the orignal change: 00c43ae23524d72707701620da89ad248393a8e4 

CC @AaronBallman 

---

### Comment 9 - AaronBallman

Yeah, this one is my fault, sorry!

I think the issue is in `CheckExtensionTraitOperandType` for the void type handling. @Aadarsh-Keshri I would combine the existing check for extended sizeof/alignof to be more like:
```
if (sizeof || alignof || preferred alignof) {
  if (is function type)
    return !diag(extension);
  else if (is void type)
    return !diag(opencl extension or C extension)
}
```

---

### Comment 10 - Aadarsh-Keshri

@AaronBallman @shafik  Thank you for your suggestions. Could you please review my pull request?

CC:  @csxplorer @k-arrows 

---

