# Clang fails to destroy temporaries bound to reference members by default member initializers at end of full-expression

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/85601

## Body

Clang appears to defer the destruction of temporaries bound to reference members by a default member initializer through aggregate initialization until the end of the block that the aggregate initialization appears in.

In the following program, `f()` and `g()` should have the same return value: The destructor for the `A` object in the evaluation of `g()` should occur at the end of the full-expression where the `B` temporary is created.

Online compiler link: https://godbolt.org/z/o9Ez3d7Ex

### SOURCE (`<stdin>`):
```cpp
struct A {
  int &x;
  constexpr ~A() { --x; }
};
struct B {
  int &x;
  const A &a = A{x};
};
constexpr int f() {
  int x = 1;
  { B{x}; }
  return x;
}
constexpr int g() {
  int x = 1;
  B{x};
  return x;
}
static_assert(f() == g());
```

### COMPILER INVOCATION:
```
clang++ -fsyntax-only -std=c++20 -w -xc++ -
```

### EXPECTED OUTPUT:
Clean compile

### ACTUAL OUTPUT:
```
<stdin>:19:15: error: static assertion failed due to requirement 'f() == g()'
   19 | static_assert(f() == g());
      |               ^~~~~~~~~~
<stdin>:19:19: note: expression evaluates to '0 == 1'
   19 | static_assert(f() == g());
      |               ~~~~^~~~~~
1 error generated.
```

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 19.0.0git (https://github.com/llvm/llvm-project.git c0d03d28c58f3016eb25ce8d1160e11183d01735)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Candidate multilib: .;@m64
Selected multilib: .;@m64
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-14

Author: Hubert Tong (hubert-reinterpretcast)

<details>
Clang appears to defer the destruction of temporaries bound to reference members by a default member initializer through aggregate initialization until the end of the block that the aggregate initialization appears in.

In the following program, `f()` and `g()` should have the same return value: The destructor for the `A` object in the evaluation of `g()` should occur at the end of the full-expression where the `B` temporary is created.

Online compiler link: https://godbolt.org/z/o9Ez3d7Ex

### SOURCE (`&lt;stdin&gt;`):
```cpp
struct A {
  int &amp;x;
  constexpr ~A() { --x; }
};
struct B {
  int &amp;x;
  const A &amp;a = A{x};
};
constexpr int f() {
  int x = 1;
  { B{x}; }
  return x;
}
constexpr int g() {
  int x = 1;
  B{x};
  return x;
}
static_assert(f() == g());
```

### COMPILER INVOCATION:
```
clang++ -fsyntax-only -std=c++20 -w -xc++ -
```

### EXPECTED OUTPUT:
Clean compile

### ACTUAL OUTPUT:
```
&lt;stdin&gt;:19:15: error: static assertion failed due to requirement 'f() == g()'
   19 | static_assert(f() == g());
      |               ^~~~~~~~~~
&lt;stdin&gt;:19:19: note: expression evaluates to '0 == 1'
   19 | static_assert(f() == g());
      |               ~~~~^~~~~~
1 error generated.
```

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 19.0.0git (https://github.com/llvm/llvm-project.git c0d03d28c58f3016eb25ce8d1160e11183d01735)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hubert Tong (hubert-reinterpretcast)

<details>
Clang appears to defer the destruction of temporaries bound to reference members by a default member initializer through aggregate initialization until the end of the block that the aggregate initialization appears in.

In the following program, `f()` and `g()` should have the same return value: The destructor for the `A` object in the evaluation of `g()` should occur at the end of the full-expression where the `B` temporary is created.

Online compiler link: https://godbolt.org/z/o9Ez3d7Ex

### SOURCE (`&lt;stdin&gt;`):
```cpp
struct A {
  int &amp;x;
  constexpr ~A() { --x; }
};
struct B {
  int &amp;x;
  const A &amp;a = A{x};
};
constexpr int f() {
  int x = 1;
  { B{x}; }
  return x;
}
constexpr int g() {
  int x = 1;
  B{x};
  return x;
}
static_assert(f() == g());
```

### COMPILER INVOCATION:
```
clang++ -fsyntax-only -std=c++20 -w -xc++ -
```

### EXPECTED OUTPUT:
Clean compile

### ACTUAL OUTPUT:
```
&lt;stdin&gt;:19:15: error: static assertion failed due to requirement 'f() == g()'
   19 | static_assert(f() == g());
      |               ^~~~~~~~~~
&lt;stdin&gt;:19:19: note: expression evaluates to '0 == 1'
   19 | static_assert(f() == g());
      |               ~~~~^~~~~~
1 error generated.
```

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 19.0.0git (https://github.com/llvm/llvm-project.git c0d03d28c58f3016eb25ce8d1160e11183d01735)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

</details>


---

### Comment 3 - efriedma-quic

The fundamental issue here is that there are two different ways an in-class initializer can be used: in the constructor, and in aggregate initialization.  And the rules for each of them are very different.  In the constructor, each member initializer is a standalone full-expression.  During aggregate initialization, the initializer is part of whatever full-expression contains the aggregate initialization.

The immediate cause of the current issue is that Sema::BuildCXXDefaultInitExpr calls ConvertMemberDefaultInitExpression(), which marks the temporary as lifetime-extended, but there's a bunch of other related stuff going wrong based on the assumption that we're dealing with a full-expression.

Variations on the theme.

```
struct A {
  int &x;
  constexpr ~A() { x = 0; }
};
struct AA {
  int &x;
  constexpr ~AA() { x = -1; }
};
struct B {
  int &x;
  const A &a = A{x};
}; 
struct BB {
  int &x;
  const AA &a = AA{x};
};
constexpr int g1() {
  int x = 1;
  B{x};
  return x;
}
constexpr int g2() {
  int x = 1;
  B{x}, BB{x};
  return x;
}
static_assert(g1() == 0);
static_assert(g2() == 0);

template<typename T> int noInstantiate() {
  static_assert(false);
}
struct NoInstantiate {
  int x = noInstantiate<int>();
};
int x = sizeof(NoInstantiate{});

struct ConstEvalCtor {
  int a;
  static consteval int z(int&x) {return --x;}
  int b = z(a);
};
struct ConstEvalCtorMember {
  const ConstEvalCtor &y = ConstEvalCtor{1};
};
constexpr int x = ConstEvalCtorMember{}.y.a;
```

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-c-20

Author: Hubert Tong (hubert-reinterpretcast)

<details>
Clang appears to defer the destruction of temporaries bound to reference members by a default member initializer through aggregate initialization until the end of the block that the aggregate initialization appears in.

In the following program, `f()` and `g()` should have the same return value: The destructor for the `A` object in the evaluation of `g()` should occur at the end of the full-expression where the `B` temporary is created.

Online compiler link: https://godbolt.org/z/o9Ez3d7Ex

### SOURCE (`&lt;stdin&gt;`):
```cpp
struct A {
  int &amp;x;
  constexpr ~A() { --x; }
};
struct B {
  int &amp;x;
  const A &amp;a = A{x};
};
constexpr int f() {
  int x = 1;
  { B{x}; }
  return x;
}
constexpr int g() {
  int x = 1;
  B{x};
  return x;
}
static_assert(f() == g());
```

### COMPILER INVOCATION:
```
clang++ -fsyntax-only -std=c++20 -w -xc++ -
```

### EXPECTED OUTPUT:
Clean compile

### ACTUAL OUTPUT:
```
&lt;stdin&gt;:19:15: error: static assertion failed due to requirement 'f() == g()'
   19 | static_assert(f() == g());
      |               ^~~~~~~~~~
&lt;stdin&gt;:19:19: note: expression evaluates to '0 == 1'
   19 | static_assert(f() == g());
      |               ~~~~^~~~~~
1 error generated.
```

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 19.0.0git (https://github.com/llvm/llvm-project.git c0d03d28c58f3016eb25ce8d1160e11183d01735)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/10
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

</details>


---

### Comment 5 - hubert-reinterpretcast

Compared against additional compilers: https://godbolt.org/z/xd6r4znaP

---

