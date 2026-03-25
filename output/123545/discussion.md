# __builtin_launder complains about type completeness

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/123545
**Status:** Closed
**Labels:** duplicate, c++17, clang:frontend, diverges-from:gcc, diverges-from:msvc, diverges-from:edg
**Closed Date:** 2025-01-20T06:23:53Z

## Body

`std::launder` does not require that the type pointed to is complete. See https://wg21.link/ptr.launder.

Clang refuses to compile the following:
```cpp
#include <new>
struct A;
A *h(A *p) { return std::launder(p); }
```
which becomes (with multiple C++ standard libraries):
```cpp
struct A;
A *h(A *p) { return __builtin_launder(p); }
```

Online compiler link: https://godbolt.org/z/8KG98zaxW

### SOURCE (`<stdin>`):
```cpp
struct A;
A *h(A *p) { return __builtin_launder(p); }
```

### COMPILER INVOCATION:
```
clang++ -fsyntax-only -std=c++17 -xc++ -
```

### ACTUAL COMPILER OUTPUT:
```
<stdin>:2:21: error: incomplete type 'A' where a complete type is required
    2 | A *h(A *p) { return __builtin_launder(p); }
      |                     ^
<stdin>:1:8: note: forward declaration of 'A'
    1 | struct A;
      |        ^
1 error generated.
```

### EXPECTED COMPILER OUTPUT:
(clean compile)

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 20.0.0git (https://github.com/llvm/llvm-project.git dd42651295d0e1566b34104b007ad19865c57fcc)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/14
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/14
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hubert Tong (hubert-reinterpretcast)

<details>
`std::launder` does not require that the type pointed to is complete. See https://wg21.link/ptr.launder.

Clang refuses to compile the following:
```cpp
#include &lt;new&gt;
struct A;
A *h(A *p) { return std::launder(p); }
```
which becomes (with multiple C++ standard libraries):
```cpp
struct A;
A *h(A *p) { return __builtin_launder(p); }
```

Online compiler link: https://godbolt.org/z/8KG98zaxW

### SOURCE (`&lt;stdin&gt;`):
```cpp
struct A;
A *h(A *p) { return __builtin_launder(p); }
```

### COMPILER INVOCATION:
```
clang++ -fsyntax-only -std=c++17 -xc++ -
```

### ACTUAL COMPILER OUTPUT:
```
&lt;stdin&gt;:2:21: error: incomplete type 'A' where a complete type is required
    2 | A *h(A *p) { return __builtin_launder(p); }
      |                     ^
&lt;stdin&gt;:1:8: note: forward declaration of 'A'
    1 | struct A;
      |        ^
1 error generated.
```

### EXPECTED COMPILER OUTPUT:
(clean compile)

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 20.0.0git (https://github.com/llvm/llvm-project.git dd42651295d0e1566b34104b007ad19865c57fcc)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/14
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/14
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-17

Author: Hubert Tong (hubert-reinterpretcast)

<details>
`std::launder` does not require that the type pointed to is complete. See https://wg21.link/ptr.launder.

Clang refuses to compile the following:
```cpp
#include &lt;new&gt;
struct A;
A *h(A *p) { return std::launder(p); }
```
which becomes (with multiple C++ standard libraries):
```cpp
struct A;
A *h(A *p) { return __builtin_launder(p); }
```

Online compiler link: https://godbolt.org/z/8KG98zaxW

### SOURCE (`&lt;stdin&gt;`):
```cpp
struct A;
A *h(A *p) { return __builtin_launder(p); }
```

### COMPILER INVOCATION:
```
clang++ -fsyntax-only -std=c++17 -xc++ -
```

### ACTUAL COMPILER OUTPUT:
```
&lt;stdin&gt;:2:21: error: incomplete type 'A' where a complete type is required
    2 | A *h(A *p) { return __builtin_launder(p); }
      |                     ^
&lt;stdin&gt;:1:8: note: forward declaration of 'A'
    1 | struct A;
      |        ^
1 error generated.
```

### EXPECTED COMPILER OUTPUT:
(clean compile)

### COMPILER VERSION INFO (`clang++ -v`):
```
clang version 20.0.0git (https://github.com/llvm/llvm-project.git dd42651295d0e1566b34104b007ad19865c57fcc)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/14
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/14
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```
</details>


---

### Comment 3 - frederick-vs-ja

It seems that [[res.on.functions]/2.5](https://eel.is/c++draft/res.on.functions#2.5) permits implementations to reject this because the deduced template argument is an incomplete type (although such UB should be IFNDR, see [P3534R0](https://wg21.link/p3534r0)).

---

### Comment 4 - cor3ntin

Some discussion here https://github.com/llvm/llvm-project/pull/91070

---

