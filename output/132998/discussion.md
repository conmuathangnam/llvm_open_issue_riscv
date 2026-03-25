# ABI break for `_Atomic` in C++23 when using libstdc++

**Author:** zygoloid
**URL:** https://github.com/llvm/llvm-project/issues/132998
**Status:** Open
**Labels:** clang, ABI, c++23, libstdc++, diverges-from:gcc

## Body

In C++23 onwards, [`<stdatomic.h>` `#define`s `_Atomic(T)` as `std::atomic<T>`](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p0943r6.html). This results in an ABI break when using libstdc++. Example:

```c++
#include <stdatomic.h>
struct A { int m, n; char c; };
static_assert(sizeof(_Atomic(A)) == 16);
```

When using Clang with libstdc++, [the built-in `_Atomic(A)` has size 16 but `std::atomic<A>` has size 12](https://godbolt.org/z/Y9G7sYcsE) on for example x86_64-linux-gnu. That means that including the `<stdatomic.h>` header in C++23, or switching from C++20 to C++23 when already including `<stdatomic.h>`, changes the ABI for the type that is spelled `_Atomic(A)`.

In some sense this is the same issue as #26836, but we're now seeing different ABI behavior with two Clang builds depending on a `-std=` flag / a `#include`, which seems worse than before.

## Comments

### Comment 1 - zygoloid

CC @jwakely 

---

### Comment 2 - jwakely

I did tell SG1 that simply adding a header called `<stdatomic.h>` to C++ would not provide any kind of actual compatibility between `_Atomic` and `std::atomic`, other than at the most superficial source level

We currently do:

```c++
#ifdef __cpp_lib_stdatomic_h // C++ >= 23
# include <atomic> 
# define _Atomic(_Tp) ::std::atomic<_Tp>
...
#elif defined __clang__
# include_next <stdatomic.h>
#endif // __cpp_lib_stdatomic_h
```

Maybe what we want is:

```c++
#ifdef __clang__
# include_next <stdatomic.h>
#elifdef __cpp_lib_stdatomic_h // C++ >= 23
# include <atomic> 
# define _Atomic(_Tp) ::std::atomic<_Tp>
...
#endif // __cpp_lib_stdatomic_h
```

And then it's up to Clang to decide what `_Atomic` means, and whether that should be compatible with Clang's own `_Atomic` extension or with libstdc++'s `std::atomic`.

---

### Comment 3 - Endilll

CC @AaronBallman it seems that this bug report fell through the cracks

---

### Comment 4 - AaronBallman

We're going to have the ABI issue either way if we make a change, aren't we? If `_Atomic` in Clang becomes ABI compatible with libstdc++'s `std::atomic`, then we're not ABI compatible with the previous version of Clang and so we break those folks.

CC @ldionne 

---

### Comment 5 - zygoloid

We have always had an ABI divergence from GCC for `_Atomic`:

* The GCC ABI rule is used for GCC `_Atomic` and GCC + libstdc++ `std::atomic`.
* The Clang ABI rule is used for Clang `_Atomic` and Clang libc++ `std::atomic`.
* Clang + libstdc++ `std::atomic` uses a third rule (according to #26836) that attempts to mimic the GCC rule but misses in some cases.

That sucks, but at least for a single build configuration (compiler plus standard library) you get a single ABI. The problem is that this `<stdatomic.h>` change means even that's no longer the case: C++ code using `_Atomic` (eg, by including a C header) with Clang and libstdc++ (a fairly common configuration) now sometimes uses the GCC ABI rule instead, depending on whether or not `<stdatomic.h>` was included.

I think the best we can do, until the ABI folks actually fix their ABIs to define a rule for `_Atomic` / `std::atomic`, is to revert to the behavior before this break -- perhaps by making `<stdatomic.h>` not define `_Atomic` at all when using Clang with libstdc++.

---

### Comment 6 - AaronBallman

> I think the best we can do, until the ABI folks actually fix their ABIs to define a rule for _Atomic / std::atomic, is to revert to the behavior before this break -- perhaps by making <stdatomic.h> not define _Atomic at all when using Clang with libstdc++.

But I don't think this is a Clang behavior?
```
#if __STDC_HOSTED__ &&                                                         \
    __has_include_next(<stdatomic.h>) &&                                       \
    (!defined(_MSC_VER) || (defined(__cplusplus) && __cplusplus >= 202002L))
# include_next <stdatomic.h>
#else
...
```
We defer to the system stdatomic.h in C++20 and later, and the else branch never defines `_Atomic`. libc++ does define `_Atomic`, however: https://github.com/llvm/llvm-project/blob/0b5daeb2e55e21eb8af9ca504fca82b82a27e1aa/libcxx/include/stdatomic.h#L138 so am I correct in understanding that the user's #include finds Clang's stdatomic.h which then include_next into libstdc++ stdatomic,h, which then (currently) defines `_Atomic` in C++23 and later? If so, then I think the suggestion from @jwakely would end up with user including Clang's stdatomic.h which then include_next into libstdc++, which would then try to include_next and try to find the definitions elsewhere?

---

### Comment 7 - zygoloid

> so am I correct in understanding that the user's #include finds Clang's stdatomic.h which then include_next into libstdc++ stdatomic,h, which then (currently) defines `_Atomic` in C++23 and later? If so, then I think the suggestion from [@jwakely](https://github.com/jwakely) would end up with user including Clang's stdatomic.h which then include_next into libstdc++, which would then try to include_next and try to find the definitions elsewhere?

For whatever reason, the include order is:

- C++ standard library
- Compiler headers
- C standard library

So with @jwakely's suggested approach, libstdc++ would allow Clang to decide what to do, and we'd then include the C library's header if it exists (which, one would assume, doesn't define `_Atomic` to `std::atomic<T>` in C++ and instead provides operations on `_Atomic(T)`). That at least would undo the ABI break and give us more time to fix this properly.

---

