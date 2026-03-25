# Clang considers destructor in noexcept expression that should call destroying delete

**Author:** AlisdairM
**URL:** https://github.com/llvm/llvm-project/issues/118660
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2024-12-05T19:26:34Z

## Body

When a destroying delete function is called, the pointed-to object's destructor is not run --- it is the responsibility of the destroying delete function to run any destructors.  However, when querying the noexcept operator for such a delete expression, Clang is considering the exception specification of the destructor, even though at runtime it clearly performs the right code.

```
#include <cstdio>
#include <new>

struct Explicit {
    ~Explicit() noexcept(false) { puts("should not be called");}

    void operator delete(Explicit*, std::destroying_delete_t) noexcept {
        std::puts("destroyed explicit");
    }
};

// #define  SHOW_BUGS
#if defined(SHOW_BUGS)

Explicit * qn = nullptr;
static_assert( noexcept(delete(qn)));

#endif

int main() {
    Explicit *q = new Explicit();
    delete q;
}
```

Here us a Compiler Explorer link to a more complete test, that passed on EDG even with the SHOW_BUGS macro defined:
   https://godbolt.org/z/YaaeT8W17


## Comments

### Comment 1 - AaronBallman

https://eel.is/c++draft/expr.unary.noexcept#2 says that the only thing that matters is if the destructor is potentially throwing.
https://eel.is/c++draft/except.spec#5 defines what it means to be potentially throwing.
https://eel.is/c++draft/except.spec#5.2 says that the implicitly called `operator delete` is what matters, and 
https://eel.is/c++draft/except.spec#1 says that it's not potentially throwing.

So by my reading of the standard, this is a Clang bug not because of the destroying delete but because we're failing to consider the overloaded operator delete when trying to determine if the `noexcept` operand is potentially throwing. I think the issue is somewhere in the vicinity of: https://github.com/llvm/llvm-project/blob/095c3c9d6ec349815563d47f951d4590b3d18333/clang/lib/Sema/SemaExceptionSpec.cpp#L1201

---

### Comment 2 - AlisdairM

I read it slightly differently, as nothing in https://eel.is/c++draft/expr.unary.noexcept#2  mentions destructors --- but I wonder if we are saying the same thing in different ways.

What matters is whether the expression is potentially throwing --- and for that we need to know what the expression does.  For most calls to delete, there are two parts: first run the destructor of the target object, and then call the delete operator to reclaim memory.  In such cases, whether the destructor is potentially throwing is relevant.  However, in the event that overload resolution finds a destroying delete function, then the destructor is *not* called, and only the exception specification of the destroying delete function is relevant.

---

### Comment 3 - AaronBallman

> I read it slightly differently, as nothing in https://eel.is/c++draft/expr.unary.noexcept#2 mentions destructors --- but I wonder if we are saying the same thing in different ways.
> 
> What matters is whether the expression is potentially throwing --- and for that we need to know what the expression does. For most calls to delete, there are two parts: first run the destructor of the target object, and then call the delete operator to reclaim memory. In such cases, whether the destructor is potentially throwing is relevant. However, in the event that overload resolution finds a destroying delete function, then the destructor is _not_ called, and only the exception specification of the destroying delete function is relevant.

Yup, I think we're saying the same thing in slightly different ways. Basically, when resolving whether it can throw, we normally consider the destructor of what's thrown in all circumstances when we should be ignoring the destructor for a destroying delete.

---

### Comment 4 - efriedma-quic

While we're in the area, maybe also worth addressing the following:

```
namespace std { struct destroying_delete_t {}; }
struct S { S(); ~S() = delete; void operator delete(S*, std::destroying_delete_t); };
void f(S* s) { delete s; }
```

---

### Comment 5 - AaronBallman

> While we're in the area, maybe also worth addressing the following:
> 
> ```
> namespace std { struct destroying_delete_t {}; }
> struct S { S(); ~S() = delete; void operator delete(S*, std::destroying_delete_t); };
> void f(S* s) { delete s; }
> ```

I posted https://github.com/llvm/llvm-project/pull/118800 to handle that, good catch!

---

### Comment 6 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alisdair Meredith (AlisdairM)

<details>
When a destroying delete function is called, the pointed-to object's destructor is not run --- it is the responsibility of the destroying delete function to run any destructors.  However, when querying the noexcept operator for such a delete expression, Clang is considering the exception specification of the destructor, even though at runtime it clearly performs the right code.

```
#include &lt;cstdio&gt;
#include &lt;new&gt;

struct Explicit {
    ~Explicit() noexcept(false) { puts("should not be called");}

    void operator delete(Explicit*, std::destroying_delete_t) noexcept {
        std::puts("destroyed explicit");
    }
};

// #define  SHOW_BUGS
#if defined(SHOW_BUGS)

Explicit * qn = nullptr;
static_assert( noexcept(delete(qn)));

#endif

int main() {
    Explicit *q = new Explicit();
    delete q;
}
```

Here us a Compiler Explorer link to a more complete test, that passed on EDG even with the SHOW_BUGS macro defined:
   https://godbolt.org/z/YaaeT8W17

</details>


---

