# [clang] `__builtin_assume_aligned()` with constexpr argument crashes clang

**Author:** Artem-B
**URL:** https://github.com/llvm/llvm-project/issues/161272
**Status:** Closed
**Labels:** clang:frontend, crash, constexpr
**Closed Date:** 2025-10-01T14:24:50Z

## Body

Reproducer: https://godbolt.org/z/d6eeM34rd

```c++
float *words_;

constexpr int x(int a, int b) { return a+b; }

void *crash_me(int index) {
    return __builtin_assume_aligned(words_ + index, x(2,2));
}
```

Partial stack trace:
```
#4  0x00007fd5be232420 in __assert_fail_base (fmt=<optimized out>, assertion=<optimized out>, file=<optimized out>, line=578, function=<optimized out>) at ./assert/assert.c:118
#5  0x00007fd5c7fd0914 in llvm::cast<llvm::ConstantInt, llvm::Value> (Val=0x556d3e9886d0) at /usr/local/google/home/tra/work/llvm/repo/llvm/include/llvm/Support/Casting.h:578
#6  0x00007fd5c801592c in clang::CodeGen::CodeGenFunction::EmitBuiltinExpr (this=0x7ffe61027380, GD=..., BuiltinID=202, E=0x556d3e964d48, ReturnValue=...) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGBuiltin.cpp:3543
#7  0x00007fd5c83429ac in clang::CodeGen::CodeGenFunction::EmitCallExpr (this=0x7ffe61027380, E=0x556d3e964d48, ReturnValue=..., CallOrInvoke=0x7ffe61025a40) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExpr.cpp:5962
#8  0x00007fd5c83f4de1 in (anonymous namespace)::ScalarExprEmitter::VisitCallExpr (this=0x7ffe61025c60, E=0x556d3e964d48) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExprScalar.cpp:631
#9  0x00007fd5c83eb9e1 in clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit (this=0x7ffe61025c60, S=0x556d3e964d48) at tools/clang/include/clang/AST/StmtNodes.inc:608
#10 0x00007fd5c83e1225 in (anonymous namespace)::ScalarExprEmitter::Visit (this=0x7ffe61025c60, E=0x556d3e964d48) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExprScalar.cpp:452
#11 0x00007fd5c83e1062 in clang::CodeGen::CodeGenFunction::EmitScalarExpr (this=0x7ffe61027380, E=0x556d3e964d48, IgnoreResultAssign=false) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExprScalar.cpp:5873
#12 0x00007fd5c86494b5 in clang::CodeGen::CodeGenFunction::EmitReturnStmt (this=0x7ffe61027380, S=...) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGStmt.cpp:1692
```

Root cause is the implicit assumption in CGBuiltin.cpp https://github.com/llvm/llvm-project/blob/b629981a6daf397375ff1fcadad286883dcaf0ea/clang/lib/CodeGen/CGBuiltin.cpp#L3540-L3543

Instead of the hard cast of the emitted argument, the code should've evaluated the argument value and used it instead. Sema already verifies that we can evaluate the argument value.



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Artem Belevich (Artem-B)

<details>
Reproducer: https://godbolt.org/z/d6eeM34rd

```
float *words_;

constexpr int x(int a, int b) { return a+b; }

void *crash_me(int index) {
    return __builtin_assume_aligned(words_ + index, x(2,2));
}
```

Partial stack trace:
```
#<!-- -->4  0x00007fd5be232420 in __assert_fail_base (fmt=&lt;optimized out&gt;, assertion=&lt;optimized out&gt;, file=&lt;optimized out&gt;, line=578, function=&lt;optimized out&gt;) at ./assert/assert.c:118
#<!-- -->5  0x00007fd5c7fd0914 in llvm::cast&lt;llvm::ConstantInt, llvm::Value&gt; (Val=0x556d3e9886d0) at /usr/local/google/home/tra/work/llvm/repo/llvm/include/llvm/Support/Casting.h:578
#<!-- -->6  0x00007fd5c801592c in clang::CodeGen::CodeGenFunction::EmitBuiltinExpr (this=0x7ffe61027380, GD=..., BuiltinID=202, E=0x556d3e964d48, ReturnValue=...) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGBuiltin.cpp:3543
#<!-- -->7  0x00007fd5c83429ac in clang::CodeGen::CodeGenFunction::EmitCallExpr (this=0x7ffe61027380, E=0x556d3e964d48, ReturnValue=..., CallOrInvoke=0x7ffe61025a40) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExpr.cpp:5962
#<!-- -->8  0x00007fd5c83f4de1 in (anonymous namespace)::ScalarExprEmitter::VisitCallExpr (this=0x7ffe61025c60, E=0x556d3e964d48) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExprScalar.cpp:631
#<!-- -->9  0x00007fd5c83eb9e1 in clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit (this=0x7ffe61025c60, S=0x556d3e964d48) at tools/clang/include/clang/AST/StmtNodes.inc:608
#<!-- -->10 0x00007fd5c83e1225 in (anonymous namespace)::ScalarExprEmitter::Visit (this=0x7ffe61025c60, E=0x556d3e964d48) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExprScalar.cpp:452
#<!-- -->11 0x00007fd5c83e1062 in clang::CodeGen::CodeGenFunction::EmitScalarExpr (this=0x7ffe61027380, E=0x556d3e964d48, IgnoreResultAssign=false) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExprScalar.cpp:5873
#<!-- -->12 0x00007fd5c86494b5 in clang::CodeGen::CodeGenFunction::EmitReturnStmt (this=0x7ffe61027380, S=...) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGStmt.cpp:1692
```

Root cause is the implicit assumption in CGBuiltin.cpp https://github.com/llvm/llvm-project/blob/b629981a6daf397375ff1fcadad286883dcaf0ea/clang/lib/CodeGen/CGBuiltin.cpp#L3540-L3543

Instead of the hard cast of the emitted argument, the code should've evaluated the argument value and used it instead. Sema already verifies that we can evaluate the argument value.


</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Artem Belevich (Artem-B)

<details>
Reproducer: https://godbolt.org/z/d6eeM34rd

```c++
float *words_;

constexpr int x(int a, int b) { return a+b; }

void *crash_me(int index) {
    return __builtin_assume_aligned(words_ + index, x(2,2));
}
```

Partial stack trace:
```
#<!-- -->4  0x00007fd5be232420 in __assert_fail_base (fmt=&lt;optimized out&gt;, assertion=&lt;optimized out&gt;, file=&lt;optimized out&gt;, line=578, function=&lt;optimized out&gt;) at ./assert/assert.c:118
#<!-- -->5  0x00007fd5c7fd0914 in llvm::cast&lt;llvm::ConstantInt, llvm::Value&gt; (Val=0x556d3e9886d0) at /usr/local/google/home/tra/work/llvm/repo/llvm/include/llvm/Support/Casting.h:578
#<!-- -->6  0x00007fd5c801592c in clang::CodeGen::CodeGenFunction::EmitBuiltinExpr (this=0x7ffe61027380, GD=..., BuiltinID=202, E=0x556d3e964d48, ReturnValue=...) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGBuiltin.cpp:3543
#<!-- -->7  0x00007fd5c83429ac in clang::CodeGen::CodeGenFunction::EmitCallExpr (this=0x7ffe61027380, E=0x556d3e964d48, ReturnValue=..., CallOrInvoke=0x7ffe61025a40) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExpr.cpp:5962
#<!-- -->8  0x00007fd5c83f4de1 in (anonymous namespace)::ScalarExprEmitter::VisitCallExpr (this=0x7ffe61025c60, E=0x556d3e964d48) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExprScalar.cpp:631
#<!-- -->9  0x00007fd5c83eb9e1 in clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit (this=0x7ffe61025c60, S=0x556d3e964d48) at tools/clang/include/clang/AST/StmtNodes.inc:608
#<!-- -->10 0x00007fd5c83e1225 in (anonymous namespace)::ScalarExprEmitter::Visit (this=0x7ffe61025c60, E=0x556d3e964d48) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExprScalar.cpp:452
#<!-- -->11 0x00007fd5c83e1062 in clang::CodeGen::CodeGenFunction::EmitScalarExpr (this=0x7ffe61027380, E=0x556d3e964d48, IgnoreResultAssign=false) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGExprScalar.cpp:5873
#<!-- -->12 0x00007fd5c86494b5 in clang::CodeGen::CodeGenFunction::EmitReturnStmt (this=0x7ffe61027380, S=...) at /usr/local/google/home/tra/work/llvm/repo/clang/lib/CodeGen/CGStmt.cpp:1692
```

Root cause is the implicit assumption in CGBuiltin.cpp https://github.com/llvm/llvm-project/blob/b629981a6daf397375ff1fcadad286883dcaf0ea/clang/lib/CodeGen/CGBuiltin.cpp#L3540-L3543

Instead of the hard cast of the emitted argument, the code should've evaluated the argument value and used it instead. Sema already verifies that we can evaluate the argument value.


</details>


---

