# requires-clauses think void values can be passed to variadic functions

**Author:** Alcaro
**URL:** https://github.com/llvm/llvm-project/issues/135694

## Body

```c++
#include <type_traits>
void a(...);
template<typename T>
bool b()
{
    return requires { a(a(1)); };
}
bool c() { return b<void>(); }
```
Expected: Either false, because `a(1)` is void and cannot be passed to `a()`, or a hard error, because there is no set of template arguments for which that sub-expression is legal.

Actual: True.

https://godbolt.org/z/db83c35fq

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Alcaro)

<details>
```c++
#include &lt;type_traits&gt;
void a(...);
template&lt;typename T&gt;
bool b()
{
    return requires { a(a(1)); };
}
bool c() { return b&lt;void&gt;(); }
```
Expected: Either false, because `a(1)` is void and cannot be passed to `a()`, or a hard error, because there is no set of template arguments for which that sub-expression is legal.

Actual: True.

https://godbolt.org/z/db83c35fq
</details>


---

### Comment 2 - zygoloid

Confirmed. For what it's worth, [[expr.prim.req.general]/5](https://eel.is/c++draft/expr.prim.req#general-5.sentence-6) says:

> If the substitution of template arguments into a requirement would always result in a substitution failure, the program is ill-formed; no diagnostic required.

so accepting this and returning `true` is strictly-speaking conforming. But nonetheless Clang shouldn't do that, and here's a reduction of the same issue that's not IFNDR:

```c++
void a(...);
template<typename T> void b() {
  decltype(a(void())) *p;
}
void c() { b<void>(); }
```

Clang accepts this too. It looks like the error in question isn't generated in dependent contexts, and if the expression ends up not needing to be instantiated (because it's not dependent and is used in a context that doesn't need to rebuild it, such as the operand of `decltype` or a `requires` expression), that means we never produce the error at all.

It looks like the problem is that `Sema::checkCall` bails out when checking in a dependent context. But that's wrong: if it's checking something non-dependent, there's no guarantee it'll get checked again at any later point, because template instantiation sometimes skips non-dependent subexpressions.

---

