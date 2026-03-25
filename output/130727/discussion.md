# Clang ICE on specialized template taking a member function pointer

**Author:** tambry
**URL:** https://github.com/llvm/llvm-project/issues/130727
**Status:** Closed
**Labels:** clang:frontend, regression, crash-on-invalid, confirmed
**Closed Date:** 2025-05-27T08:16:47Z

## Body

The following crashes on trunk, but not on 20 or earlier.
```
template<typename T, T>
struct proxy;

template<typename T, typename R, typename... Args, R(T::*mf)()>
struct proxy<R(T::*)(Args...), mf>;
```

I was just trying to write some code to get the return of a member function. 🥲

[Godbolt](https://godbo.lt/z/8qcq1vshn)  
[GCC bug](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=90170)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Raul Tambre (tambry)

<details>
The following crashes on trunk, but not on 20 or earlier.
```
template&lt;typename T, T&gt;
struct proxy;

template&lt;typename T, typename R, typename... Args, R(T::*mf)()&gt;
struct proxy&lt;R(T::*)(Args...), mf&gt;;
```

[Godbolt](https://godbo.lt/z/8qcq1vshn)
</details>


---

### Comment 2 - zyn0217

@mizvekov 

---

### Comment 3 - mizvekov

There is a direct issue here that's pretty easy to fix, unfortunately this then hits on some pretty complicated unresolved issues regarding partial ordering of templates with placeholder types.

---

### Comment 4 - tambry

Seems to have been fixed. Can't be bothered to bisect for the commit.

---

