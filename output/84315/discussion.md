# Assigning to `__real` or `__imag` of an uninitialized local complex variable at compile time asserts

**Author:** tbaederr
**URL:** https://github.com/llvm/llvm-project/issues/84315

## Body

This code:

```c++
constexpr bool b() {
  _Complex float A;// = {3.0, 3.0};
  _Complex float B = {4.0, 2.0};

  __real(A) = 4.0;
  __imag(A) = 2.0;

  return A == B;
}
static_assert(b());
```
https://godbolt.org/z/v59jxqMhE

asserts:
```
clang++: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:3828: typename SubobjectHandler::result_type findSubobject({anonymous}::EvalInfo&, const clang::Expr*, const {anonymous}::CompleteObject&, const {anonymous}::SubobjectDesignator&, SubobjectHandler&) [with SubobjectHandler = {anonymous}::ModifySubobjectHandler; typename SubobjectHandler::result_type = bool]: Assertion `O->isComplexFloat()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:11:1: current parser token 'static_assert'
2.	<source>:2:20: parsing function body 'b'
 #0 0x00000000038e30c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x38e30c8)
 #1 0x00000000038e0dac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x38e0dac)
 #2 0x0000000003829088 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fded1642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fded16969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007fded1642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007fded16287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007fded162871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007fded1639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000073782cc handleAssignment((anonymous namespace)::EvalInfo&, clang::Expr const*, (anonymous namespace)::LValue const&, clang::QualType, clang::APValue&) ExprConstant.cpp:0:0
#10 0x00000000073a92c2 clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x00000000073a99cd EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#12 0x0000000007380b8e Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#13 0x00000000073818af EvaluateIgnoredValue((anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#14 0x00000000073a166c EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#15 0x00000000073a18a8 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#16 0x00000000073a6d6b HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&, clang::APValue&, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#17 0x00000000073ed8b6 clang::Expr::isPotentialConstantExpr(clang::FunctionDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73ed8b6)
#18 0x000000000658203f clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658203f)
#19 0x00000000064d205b clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d205b)
#20 0x000000000624a08f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x624a08f)
#21 0x0000000006165f51 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6165f51)
[...]
```

The problem goes away when `A` is initialized before the call to `__imag` or `__real`..

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timm Baeder (tbaederr)

<details>
This code:

```c++
constexpr bool b() {
  _Complex float A;// = {3.0, 3.0};
  _Complex float B = {4.0, 2.0};

  __real(A) = 4.0;
  __imag(A) = 2.0;

  return A == B;
}
static_assert(b());
```
https://godbolt.org/z/v59jxqMhE

asserts:
```
clang++: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:3828: typename SubobjectHandler::result_type findSubobject({anonymous}::EvalInfo&amp;, const clang::Expr*, const {anonymous}::CompleteObject&amp;, const {anonymous}::SubobjectDesignator&amp;, SubobjectHandler&amp;) [with SubobjectHandler = {anonymous}::ModifySubobjectHandler; typename SubobjectHandler::result_type = bool]: Assertion `O-&gt;isComplexFloat()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:11:1: current parser token 'static_assert'
2.	&lt;source&gt;:2:20: parsing function body 'b'
 #<!-- -->0 0x00000000038e30c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x38e30c8)
 #<!-- -->1 0x00000000038e0dac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x38e0dac)
 #<!-- -->2 0x0000000003829088 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007fded1642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007fded16969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007fded1642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007fded16287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007fded162871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007fded1639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000073782cc handleAssignment((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::QualType, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->10 0x00000000073a92c2 clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->11 0x00000000073a99cd EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007380b8e Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000073818af EvaluateIgnoredValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00000000073a166c EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000073a18a8 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->16 0x00000000073a6d6b HandleFunctionCall(clang::SourceLocation, clang::FunctionDecl const*, (anonymous namespace)::LValue const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, (anonymous namespace)::CallRef, clang::Stmt const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;, (anonymous namespace)::LValue const*) ExprConstant.cpp:0:0
#<!-- -->17 0x00000000073ed8b6 clang::Expr::isPotentialConstantExpr(clang::FunctionDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73ed8b6)
#<!-- -->18 0x000000000658203f clang::Sema::CheckConstexprFunctionDefinition(clang::FunctionDecl const*, clang::Sema::CheckConstexprKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x658203f)
#<!-- -->19 0x00000000064d205b clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d205b)
#<!-- -->20 0x000000000624a08f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x624a08f)
#<!-- -->21 0x0000000006165f51 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6165f51)
[...]
```

The problem goes away when `A` is initialized before the call to `__imag` or `__real`..
</details>


---

### Comment 2 - AaronBallman

Slightly reduced example:
```
constexpr void b() {
  _Complex float A;// = {3.0, 3.0};
  __real(A) = 4.0;
}
```

---

### Comment 3 - AaronBallman

The issue is that the code here https://github.com/llvm/llvm-project/blob/e0d49066c1acfa4ae0f0e8ea49b0f0b6bb3f1a25/clang/lib/AST/ExprConstant.cpp#L3824 is expecting either a complex of a float type or a complex of an int type but what it has is an indeterminate value (allowed by https://github.com/llvm/llvm-project/blob/e0d49066c1acfa4ae0f0e8ea49b0f0b6bb3f1a25/clang/lib/AST/ExprConstant.cpp#L3701) which is neither. The assertion is not incorrect, the underlying `APValue` is not initialized properly and thus can't be used for assignment.

---

### Comment 4 - tbaederr

Also happens with vectors now: https://github.com/llvm/llvm-project/issues/140348

---

### Comment 5 - ziqingluo-90

Do you think the example itself is well-defined according to the standard? Or a vector/complex object must be initialized by an initializer explicitly in a `constexpr` function?

---

### Comment 6 - AaronBallman

> Do you think the example itself is well-defined according to the standard? Or a vector/complex object must be initialized by an initializer explicitly in a `constexpr` function?

This involves compiler extensions, so the standard doesn't have much to say on it. But there's no reason to require an initializer for complex or vector types when we don't require it for scalars: https://godbolt.org/z/7788TdxEW

---

### Comment 7 - ziqingluo-90

To work around it locally, I lazy-initialize vectors to `{Indeterminate, Indeterminate,  ...}` before 
https://github.com/llvm/llvm-project/blob/a2b8c84d553bd8e9a21f3c6a78ed5ef844e08f61/clang/lib/AST/ExprConstant.cpp#L3907-L3910
with
```
      // Lazy initialize O:
      if (O->isIndeterminate()) {
        llvm::SmallVector<APValue> VecOfIndeterminate{NumElements};

        std::fill(VecOfIndeterminate.begin(), VecOfIndeterminate.end(),
                    APValue::IndeterminateValue());
        *O = APValue(VecOfIndeterminate.data(), VecOfIndeterminate.size());
      }
```

---

### Comment 8 - ziqingluo-90

Hi, I wanted to check if there's been any progress on this issue?

---

### Comment 9 - tbaederr

Nobody has been working on this as far as I know.

---

