# Clang fails to redo overload resolution in `typeid` operand after lambda _id-expression_ transformation

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/137270

## Body

The transformation in https://wg21.link/expr.prim.lambda.capture#11 can change the `const` qualification of a glvalue expression in a non-mutable lambda and whether or not a `typeid` has an unevaluated operand or not.

Clang fails to redo overload resolution after the `const` qualification of an argument changes because of the rewrite.

Unlike https://github.com/llvm/llvm-project/issues/22978, the type of the operand is _correctly_ `const` in the end. The error here is in the choice of `f`.

https://godbolt.org/z/qzs6bnfMo

### Source (`<stdin>`)
```cpp
#include <typeinfo>
struct A {
  virtual void f();
};
int &f(const int &);
A &f(int &);
constexpr auto q(int &&x) {
  return [=] { return &typeid(f(x)); }();  // x has type `int`, causing odr-use and triggering rewrite;
                                           // after rewrite x has type `const int` and the result is `typeid(int)`.
}
static_assert(q(0) == &typeid(int));
```

### Compiler invocation command
```
clang++ -fsyntax-only -Wall -Wextra -pedantic -std=c++2c -stdlib=libc++ -xc++ -
```

### Actual compiler output
```
<stdin>:8:33: error: binding reference of type 'int' to value of type 'const int' drops 'const' qualifier
    8 |   return [=] { return &typeid(f(x)); }();  // x has type `int`, causing odr-use and triggering rewrite;
      |                                 ^
<stdin>:6:11: note: passing argument to parameter here
    6 | A &f(int &);
      |           ^
<stdin>:11:20: error: invalid operands to binary expression ('void' and 'const std::type_info *')
   11 | static_assert(q(0) == &typeid(int));
      |               ~~~~ ^  ~~~~~~~~~~~~
2 errors generated.
```

### Expected compiler output
(clean compile)

### Compiler version info (`clang++ -v`)
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 6ba704a7f4e72ba088fa19c7622e42b9c460da47)
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
The transformation in https://wg21.link/expr.prim.lambda.capture#<!-- -->11 can change the `const` qualification of a glvalue expression in a non-mutable lambda and whether or not a `typeid` has an unevaluated operand or not.

Clang fails to redo overload resolution after the `const` qualification of an argument changes because of the rewrite.

Unlike https://github.com/llvm/llvm-project/issues/22978, the type of the operand is _correctly_ `const` in the end. The error here is in the choice of `f`.

https://godbolt.org/z/qzs6bnfMo

### Source (`&lt;stdin&gt;`)
```cpp
#include &lt;typeinfo&gt;
struct A {
  virtual void f();
};
int &amp;f(const int &amp;);
A &amp;f(int &amp;);
constexpr auto q(int &amp;&amp;x) {
  return [=] { return &amp;typeid(f(x)); }();  // x has type `int`, causing odr-use and triggering rewrite;
                                           // after rewrite x has type `const int` and the result is `typeid(int)`.
}
static_assert(q(0) == &amp;typeid(int));
```

### Compiler invocation command
```
clang++ -fsyntax-only -Wall -Wextra -pedantic -std=c++2c -stdlib=libc++ -xc++ -
```

### Actual compiler output
```
&lt;stdin&gt;:8:33: error: binding reference of type 'int' to value of type 'const int' drops 'const' qualifier
    8 |   return [=] { return &amp;typeid(f(x)); }();  // x has type `int`, causing odr-use and triggering rewrite;
      |                                 ^
&lt;stdin&gt;:6:11: note: passing argument to parameter here
    6 | A &amp;f(int &amp;);
      |           ^
&lt;stdin&gt;:11:20: error: invalid operands to binary expression ('void' and 'const std::type_info *')
   11 | static_assert(q(0) == &amp;typeid(int));
      |               ~~~~ ^  ~~~~~~~~~~~~
2 errors generated.
```

### Expected compiler output
(clean compile)

### Compiler version info (`clang++ -v`)
```
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 6ba704a7f4e72ba088fa19c7622e42b9c460da47)
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

