# [Clang] Accepts invalid constructor declaration with an asterisk

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/121706
**Status:** Closed
**Labels:** clang:frontend, accepts-invalid, confirmed
**Closed Date:** 2025-01-16T21:00:43Z

## Body

```c++
struct A {
    *A();
};
```
See Compiler Explorer: https://compiler-explorer.com/z/ccf6rqenE

## Comments

### Comment 1 - pinskia

Just FYI, GCC has a similar bug and was filed as https://gcc.gnu.org/bugzilla/show_bug.cgi?id=118306 (there was also a recent regression with an ICE when defaulting the constructor which the reporter here reported to GCC as https://gcc.gnu.org/bugzilla/show_bug.cgi?id=118304 ).

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
```c++
struct A {
    *A();
};
```
See Compiler Explorer: https://compiler-explorer.com/z/ccf6rqenE
</details>


---

### Comment 3 - shafik

Fun to note it accepts any number of asterisks: https://compiler-explorer.com/z/naaP75err

```cpp
struct A {
    ******************A();
};
```

we can bring the whole gang along:  https://compiler-explorer.com/z/TY8aq8G9W

```cpp
struct A {
    ****A();
    ***~A();
    **A(const A&);
    *A(A&&);
};
```

goes back to clang-2.6: https://compiler-explorer.com/z/zWv4b45s7

---

### Comment 4 - MagentaTreehouse

To generalize further, Clang seems to allow any of the [_ptr-operator_](https://timsong-cpp.github.io/cppwp/dcl.decl.general#nt:ptr-operator)s in the _ptr-declarator_ for [constructors](https://timsong-cpp.github.io/cppwp/class.ctor#general-1) and [destructors](https://timsong-cpp.github.io/cppwp/class.dtor#1).

Consider also these accepts-invalid cases: https://compiler-explorer.com/z/qdbdsjbWh

```c++
struct A {
    *&A();
};

struct B {
    *&&B();
};

struct C {
    *const C();
};

struct D {
    *const *D();
};

struct E {
    *E::*E();
};

struct F {
    *F::*const F();
};
```

Only to fail when there is a `&` in the front:

```c++
struct G {
    &G();
};
```

Output:
```console
<source>:2:5: error: cannot form a reference to 'void'
    2 |     &G();
      |     ^
```

---

