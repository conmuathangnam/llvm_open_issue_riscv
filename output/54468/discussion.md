# clang-10 regression: instantiation of constexpr template callable in C++17 mode despite trivial default init

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/54468

## Body

Templated functions that can instantiate such that the specialization satisfies the requirements for a constexpr function are well-formed template definitions. Instantiations that do not satisfy the requirements for a constexpr function are not considered ill-formed for that reason, are constexpr ([DR 1358](https://wg21.link/cwg1358)), but are not callable during constant expression evaluation ([CWG 1872](https://wg21.link/cwg1872)).

Altogether, this means that the following is a well-formed C++17 program that Clang 9 compiled but Clang 10 does not. GCC still compiles it fine (https://godbolt.org/z/YMdq7es7K).

**Source (`<stdin>`):**
```cpp
template <typename T>
struct A {
  constexpr int f() {
    T t;  // cannot be trivial default initialization until P1331
          // (not DR out of Cologne 2019)
    t = T();
    return 42 + t;
  }
};

template <const int &N>
short g(int, char (*)[N] = 0);

template <const int &N>
long g(void *);

const int x = A<int>().f();

extern decltype(g<x>(0)) q;
long q;
```

**Compiler invocation:**
```
clang++ -cc1 -fsyntax-only -std=c++17 -xc++ -
```

**Actual output:**
```
<stdin>:20:6: error: redefinition of 'q' with a different type: 'long' vs 'decltype(g<x>(0))' (aka 'short')
long q;
     ^
<stdin>:19:26: note: previous declaration is here
extern decltype(g<x>(0)) q;
                         ^
1 error generated.
```

**Expected output:**
Successful compile.

**Compiler version info (`clang++ -v`):**
```
clang version 15.0.0 (https://github.com/llvm/llvm-project.git 02db3cfe7d695da8a3119e0305c21a27af6f605c)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/9
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - EugeneZelenko

Clang 10 is very old version. Does same problem exist in 13, 14 and `main`?

---

### Comment 3 - hubert-reinterpretcast

> Clang 10 is very old version. Does same problem exist in 13, 14 and `main`?

That information was already provided in the description:
```
clang version 15.0.0 (https://github.com/llvm/llvm-project.git 02db3cfe7d695da8a3119e0305c21a27af6f605c)
```

---

### Comment 4 - frederick-vs-ja

Still reproducible in Clang 20 ([Godbolt link](https://godbolt.org/z/K38d31Y8q)). It seems that Clang backported P1331R2 to C++14/17 modes.

IIUC, the example showed that P1331R2 can change behavior of a well-defined program, so it's more arguable to backport it to old modes. At least we should say something in https://clang.llvm.org/docs/LanguageExtensions.html.

---

