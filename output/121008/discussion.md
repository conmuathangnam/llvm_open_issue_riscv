# Constraints of template template parameter not checked for non-dependent arguments despite [temp.names]/8

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/121008

## Body

https://wg21.link/temp.names#8 specifies that when

> all [_template-argument_](https://eel.is/c++draft/temp.names#nt:template-argument)s in the [_simple-template-id_](https://eel.is/c++draft/temp.names#nt:simple-template-id) are non-dependent ([[temp.dep.temp]](https://eel.is/c++draft/temp.dep.temp)), the associated constraints ([[temp.constr.decl]](https://eel.is/c++draft/temp.constr.decl)) of the constrained template shall be satisfied

where the "constrained template" may be a template template parameter.

Even when template is instantiated, GCC and Clang both fail to emit a diagnostic for a violation of this rule.
EDG and MSVC both diagnose without needing an instantiation.

See also: https://github.com/cplusplus/CWG/issues/658

Online compiler link: https://godbolt.org/z/fTzMnEPG8

### SOURCE (`<stdin>`)
```cpp
template <typename T>
concept C = false;

template <typename> struct Q;

template <template <C> class TT>
struct A {
  TT<int> *p; 
};

A<Q> a;
```

### COMPILER INVOCATION
```
clang++ -fsyntax-only -std=c++20 -Wall -Wextra -Werror -pedantic-errors -xc++ -
```

### ACTUAL COMPILER OUTPUT
(clean compile)

### EXPECTED COMPILER OUTPUT
(error)

### COMPILER VERSION INFO (`clang++ -v`)
```
clang version 20.0.0git (https://github.com/llvm/llvm-project.git b84218526d364cb9fda23bfca164d25a7b03a641)
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


@llvm/issue-subscribers-c-20

Author: Hubert Tong (hubert-reinterpretcast)

<details>
https://wg21.link/temp.names#<!-- -->8 specifies that when

&gt; all [_template-argument_](https://eel.is/c++draft/temp.names#nt:template-argument)s in the [_simple-template-id_](https://eel.is/c++draft/temp.names#nt:simple-template-id) are non-dependent ([[temp.dep.temp]](https://eel.is/c++draft/temp.dep.temp)), the associated constraints ([[temp.constr.decl]](https://eel.is/c++draft/temp.constr.decl)) of the constrained template shall be satisfied

where the "constrained template" may be a template template parameter.

Even when template is instantiated, GCC and Clang both fail to emit a diagnostic for a violation of this rule.
EDG and MSVC both diagnose without needing an instantiation.

See also: https://github.com/cplusplus/CWG/issues/658

Online compiler link: https://godbolt.org/z/fTzMnEPG8

### SOURCE (`&lt;stdin&gt;`)
```cpp
template &lt;typename T&gt;
concept C = false;

template &lt;typename&gt; struct Q;

template &lt;template &lt;C&gt; class TT&gt;
struct A {
  TT&lt;int&gt; *p; 
};

A&lt;Q&gt; a;
```

### COMPILER INVOCATION
```
clang++ -fsyntax-only -std=c++20 -Wall -Wextra -Werror -pedantic-errors -xc++ -
```

### ACTUAL COMPILER OUTPUT
(clean compile)

### EXPECTED COMPILER OUTPUT
(error)

### COMPILER VERSION INFO (`clang++ -v`)
```
clang version 20.0.0git (https://github.com/llvm/llvm-project.git b84218526d364cb9fda23bfca164d25a7b03a641)
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


@llvm/issue-subscribers-clang-frontend

Author: Hubert Tong (hubert-reinterpretcast)

<details>
https://wg21.link/temp.names#<!-- -->8 specifies that when

&gt; all [_template-argument_](https://eel.is/c++draft/temp.names#nt:template-argument)s in the [_simple-template-id_](https://eel.is/c++draft/temp.names#nt:simple-template-id) are non-dependent ([[temp.dep.temp]](https://eel.is/c++draft/temp.dep.temp)), the associated constraints ([[temp.constr.decl]](https://eel.is/c++draft/temp.constr.decl)) of the constrained template shall be satisfied

where the "constrained template" may be a template template parameter.

Even when template is instantiated, GCC and Clang both fail to emit a diagnostic for a violation of this rule.
EDG and MSVC both diagnose without needing an instantiation.

See also: https://github.com/cplusplus/CWG/issues/658

Online compiler link: https://godbolt.org/z/fTzMnEPG8

### SOURCE (`&lt;stdin&gt;`)
```cpp
template &lt;typename T&gt;
concept C = false;

template &lt;typename&gt; struct Q;

template &lt;template &lt;C&gt; class TT&gt;
struct A {
  TT&lt;int&gt; *p; 
};

A&lt;Q&gt; a;
```

### COMPILER INVOCATION
```
clang++ -fsyntax-only -std=c++20 -Wall -Wextra -Werror -pedantic-errors -xc++ -
```

### ACTUAL COMPILER OUTPUT
(clean compile)

### EXPECTED COMPILER OUTPUT
(error)

### COMPILER VERSION INFO (`clang++ -v`)
```
clang version 20.0.0git (https://github.com/llvm/llvm-project.git b84218526d364cb9fda23bfca164d25a7b03a641)
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

### Comment 3 - llvmbot


@llvm/issue-subscribers-bug

Author: Hubert Tong (hubert-reinterpretcast)

<details>
https://wg21.link/temp.names#<!-- -->8 specifies that when

&gt; all [_template-argument_](https://eel.is/c++draft/temp.names#nt:template-argument)s in the [_simple-template-id_](https://eel.is/c++draft/temp.names#nt:simple-template-id) are non-dependent ([[temp.dep.temp]](https://eel.is/c++draft/temp.dep.temp)), the associated constraints ([[temp.constr.decl]](https://eel.is/c++draft/temp.constr.decl)) of the constrained template shall be satisfied

where the "constrained template" may be a template template parameter.

Even when template is instantiated, GCC and Clang both fail to emit a diagnostic for a violation of this rule.
EDG and MSVC both diagnose without needing an instantiation.

See also: https://github.com/cplusplus/CWG/issues/658

Online compiler link: https://godbolt.org/z/fTzMnEPG8

### SOURCE (`&lt;stdin&gt;`)
```cpp
template &lt;typename T&gt;
concept C = false;

template &lt;typename&gt; struct Q;

template &lt;template &lt;C&gt; class TT&gt;
struct A {
  TT&lt;int&gt; *p; 
};

A&lt;Q&gt; a;
```

### COMPILER INVOCATION
```
clang++ -fsyntax-only -std=c++20 -Wall -Wextra -Werror -pedantic-errors -xc++ -
```

### ACTUAL COMPILER OUTPUT
(clean compile)

### EXPECTED COMPILER OUTPUT
(error)

### COMPILER VERSION INFO (`clang++ -v`)
```
clang version 20.0.0git (https://github.com/llvm/llvm-project.git b84218526d364cb9fda23bfca164d25a7b03a641)
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

