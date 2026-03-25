# [clang] Crash in constexpr evaluator when initializing complex value

**Author:** tbaederr
**URL:** https://github.com/llvm/llvm-project/issues/140348
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash, constexpr
**Closed Date:** 2025-05-17T04:49:24Z

## Body

See:
```c++
typedef __attribute__((__ext_vector_type__(2))) float float2;

struct vec2 {
    _Complex  float v;
public:
    constexpr vec2(float x, float y) {
      __real(v) = x;
    }
};

constexpr vec2 S = vec2(1.0, 1.0);
```
https://godbolt.org/z/Grds7av5Y

The [original report](https://github.com/llvm/llvm-project/pull/101126#issuecomment-2887852032) used vectors, but it also happens with the not-so-recent `_Complex`.

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:4071: typename SubobjectHandler::result_type findSubobject({anonymous}::EvalInfo&, const clang::Expr*, const {anonymous}::CompleteObject&, const {anonymous}::SubobjectDesignator&, SubobjectHandler&) [with SubobjectHandler = {anonymous}::ModifySubobjectHandler; typename SubobjectHandler::result_type = bool]: Assertion `O->isComplexFloat()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:12:34: current parser token ';'
 #0 0x0000000003f99d48 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f99d48)
 #1 0x0000000003f979d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f979d4)
 #2 0x0000000003edc608 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070c8a7842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070c8a78969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070c8a7842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070c8a78287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070c8a782871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070c8a7839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000078c3013 handleAssignment((anonymous namespace)::EvalInfo&, clang::Expr const*, (anonymous namespace)::LValue const&, clang::QualType, clang::APValue&) ExprConstant.cpp:0:0
#10 0x00000000078f7ead clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#11 0x00000000078f888d EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&, (anonymous namespace)::EvalInfo&, bool) ExprConstant.cpp:0:0
#12 0x00000000078cc266 Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#13 0x00000000078ccc14 EvaluateIgnoredValue((anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#14 0x00000000078efdfc EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#15 0x00000000078f0228 EvaluateStmt((anonymous namespace)::StmtResult&, (anonymous namespace)::EvalInfo&, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#16 0x0000000007921593 HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&, (anonymous namespace)::CallRef, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&, clang::APValue&) ExprConstant.cpp:0:0
#17 0x000000000792195f HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&, llvm::ArrayRef<clang::Expr const*>, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&, clang::APValue&) ExprConstant.cpp:0:0

```


## Comments

### Comment 1 - tbaederr

Meh, this is a duplicate of https://github.com/llvm/llvm-project/issues/84315, which _I_ filed.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timm Baeder (tbaederr)

<details>
See:
```c++
typedef __attribute__((__ext_vector_type__(2))) float float2;

struct vec2 {
    _Complex  float v;
public:
    constexpr vec2(float x, float y) {
      __real(v) = x;
    }
};

constexpr vec2 S = vec2(1.0, 1.0);
```
https://godbolt.org/z/Grds7av5Y

The [original report](https://github.com/llvm/llvm-project/pull/101126#issuecomment-2887852032) used vectors, but it also happens with the not-so-recent `_Complex`.

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ExprConstant.cpp:4071: typename SubobjectHandler::result_type findSubobject({anonymous}::EvalInfo&amp;, const clang::Expr*, const {anonymous}::CompleteObject&amp;, const {anonymous}::SubobjectDesignator&amp;, SubobjectHandler&amp;) [with SubobjectHandler = {anonymous}::ModifySubobjectHandler; typename SubobjectHandler::result_type = bool]: Assertion `O-&gt;isComplexFloat()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:12:34: current parser token ';'
 #<!-- -->0 0x0000000003f99d48 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f99d48)
 #<!-- -->1 0x0000000003f979d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f979d4)
 #<!-- -->2 0x0000000003edc608 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070c8a7842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070c8a78969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070c8a7842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070c8a78287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070c8a782871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070c8a7839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000078c3013 handleAssignment((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, (anonymous namespace)::LValue const&amp;, clang::QualType, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->10 0x00000000078f7ead clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::LValueExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->11 0x00000000078f888d EvaluateLValue(clang::Expr const*, (anonymous namespace)::LValue&amp;, (anonymous namespace)::EvalInfo&amp;, bool) ExprConstant.cpp:0:0
#<!-- -->12 0x00000000078cc266 Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->13 0x00000000078ccc14 EvaluateIgnoredValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->14 0x00000000078efdfc EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->15 0x00000000078f0228 EvaluateStmt((anonymous namespace)::StmtResult&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Stmt const*, clang::SwitchCase const*) (.part.0) ExprConstant.cpp:0:0
#<!-- -->16 0x0000000007921593 HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&amp;, (anonymous namespace)::CallRef, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->17 0x000000000792195f HandleConstructorCall(clang::Expr const*, (anonymous namespace)::LValue const&amp;, llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::CXXConstructorDecl const*, (anonymous namespace)::EvalInfo&amp;, clang::APValue&amp;) ExprConstant.cpp:0:0

```

</details>


---

