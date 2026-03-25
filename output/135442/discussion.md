# Clang fails to consider default arguments reachable at the instantiation context

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/135442

## Body

When compiling the following, Clang does not consider the default arguments provided by the declaration marked `(A)`: https://godbolt.org/z/8Gxb3fvPz

The Clang behaviour is inconsistent with CWG's interpretation (during the 2025-04-11 teleconference) of https://wg21.link/dcl.fct.default#9.sentence-9:

> When an overload set contains a declaration of a function that inhabits a scope _S_, any default argument associated with any reachable declaration that inhabits _S_ is available to the call.

That is, although no argument-dependent lookup takes place for the case in question, the default arguments in play are extended to ones provided by redeclarations of the functions that were candidates in the template definition context.

### Source (`<stdin>`)
```cpp
constexpr int f(int *, int) { return 42; }
constexpr int f(void *) { return 13; }

template <typename T>
constexpr int g() {
  return f(T{});
}

constexpr int f(int *, int = 0);  // (A)

static_assert(g<int *>() == 42, "");
```

### Compiler invocation
```
clang++ -fsyntax-only -Wall -Wextra -pedantic -std=c++11 -xc++ -
```

### Actual output
```
<stdin>:11:15: error: static assertion failed due to requirement 'g<int *>() == 42':
   11 | static_assert(g<int *>() == 42, "");
      |               ^~~~~~~~~~~~~~~~
<stdin>:11:26: note: expression evaluates to '13 == 42'
   11 | static_assert(g<int *>() == 42, "");
      |               ~~~~~~~~~~~^~~~~
1 error generated.
```

### Expected output
(Clean compile)

### Compiler version info (`clang++ -v`)
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 9466cbdf29306b2595303dac5b62b54cf71841b4)
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
When compiling the following, Clang does not consider the default arguments provided by the declaration marked `(A)`: https://godbolt.org/z/8Gxb3fvPz

The Clang behaviour is inconsistent with CWG's interpretation (during the 2025-04-11 teleconference) of https://wg21.link/dcl.fct.default#<!-- -->9.sentence-9:

&gt; When an overload set contains a declaration of a function that inhabits a scope _S_, any default argument associated with any reachable declaration that inhabits _S_ is available to the call.

That is, although no argument-dependent lookup takes place for the case in question, the default arguments in play are extended to ones provided by redeclarations of the functions that were candidates in the template definition context.

### Source (`&lt;stdin&gt;`)
```cpp
constexpr int f(int *, int) { return 42; }
constexpr int f(void *) { return 13; }

template &lt;typename T&gt;
constexpr int g() {
  return f(T{});
}

constexpr int f(int *, int = 0);  // (A)

static_assert(g&lt;int *&gt;() == 42, "");
```

### Compiler invocation
```
clang++ -fsyntax-only -Wall -Wextra -pedantic -std=c++11 -xc++ -
```

### Actual output
```
&lt;stdin&gt;:11:15: error: static assertion failed due to requirement 'g&lt;int *&gt;() == 42':
   11 | static_assert(g&lt;int *&gt;() == 42, "");
      |               ^~~~~~~~~~~~~~~~
&lt;stdin&gt;:11:26: note: expression evaluates to '13 == 42'
   11 | static_assert(g&lt;int *&gt;() == 42, "");
      |               ~~~~~~~~~~~^~~~~
1 error generated.
```

### Expected output
(Clean compile)

### Compiler version info (`clang++ -v`)
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 9466cbdf29306b2595303dac5b62b54cf71841b4)
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


@llvm/issue-subscribers-c-1

Author: Hubert Tong (hubert-reinterpretcast)

<details>
When compiling the following, Clang does not consider the default arguments provided by the declaration marked `(A)`: https://godbolt.org/z/8Gxb3fvPz

The Clang behaviour is inconsistent with CWG's interpretation (during the 2025-04-11 teleconference) of https://wg21.link/dcl.fct.default#<!-- -->9.sentence-9:

&gt; When an overload set contains a declaration of a function that inhabits a scope _S_, any default argument associated with any reachable declaration that inhabits _S_ is available to the call.

That is, although no argument-dependent lookup takes place for the case in question, the default arguments in play are extended to ones provided by redeclarations of the functions that were candidates in the template definition context.

### Source (`&lt;stdin&gt;`)
```cpp
constexpr int f(int *, int) { return 42; }
constexpr int f(void *) { return 13; }

template &lt;typename T&gt;
constexpr int g() {
  return f(T{});
}

constexpr int f(int *, int = 0);  // (A)

static_assert(g&lt;int *&gt;() == 42, "");
```

### Compiler invocation
```
clang++ -fsyntax-only -Wall -Wextra -pedantic -std=c++11 -xc++ -
```

### Actual output
```
&lt;stdin&gt;:11:15: error: static assertion failed due to requirement 'g&lt;int *&gt;() == 42':
   11 | static_assert(g&lt;int *&gt;() == 42, "");
      |               ^~~~~~~~~~~~~~~~
&lt;stdin&gt;:11:26: note: expression evaluates to '13 == 42'
   11 | static_assert(g&lt;int *&gt;() == 42, "");
      |               ~~~~~~~~~~~^~~~~
1 error generated.
```

### Expected output
(Clean compile)

### Compiler version info (`clang++ -v`)
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 9466cbdf29306b2595303dac5b62b54cf71841b4)
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

