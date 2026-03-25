# EvaluateTemporary: Assertion `E->isPRValue() && E->getType()->isRecordType()' failed.

**Author:** zhouzhouyi-hub
**URL:** https://github.com/llvm/llvm-project/issues/53741

## Body

When compile following code, The Assertion in EvaluateTemporary will fail.
```c++
typedef bool Var;
bool foobool() {
        return (bool().Var::~Var(), false);
}
```

command line:
`clang++ -S  -emit-llvm -Xclang -disable-llvm-passes  crash.C`

my fix of it:
https://reviews.llvm.org/D119493


## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - JOE1994

Reproducible with clang 19.1.0 (assertions enabled) : https://godbolt.org/z/PvcoarP1d

---

### Comment 3 - shafik

So it looks like this is the only place we don't validate the assertions in `EvaluateTemporary()`:

https://github.com/llvm/llvm-project/blob/f7261e9bbe335a512179624e1419a27e5aa9ce75/clang/lib/AST/ExprConstant.cpp#L4894-L4895

So maybe we need to adjust the assert to be or `isLiteralType()` 

---

### Comment 4 - shafik

I just ran into this with myself: https://godbolt.org/z/3Gfr41qde

```cpp
constexpr int f() {
 using T=int;
 0 .~T();
 return 0; 
}

static_assert(f() == 0); 
```

---

