# Compile-time pre-inc/post-inc do not honor wrapping types

**Author:** tbaederr
**URL:** https://github.com/llvm/llvm-project/issues/183760
**Status:** Closed
**Labels:** clang:frontend, constexpr
**Closed Date:** 2026-03-06T17:21:50Z

## Body

See https://godbolt.org/z/fnd7KG3vM

```c++
#define __wrap __attribute__((overflow_behavior(wrap)))
#define __no_trap __attribute__((overflow_behavior(trap)))

typedef int __ob_wrap wrap_int;
typedef int __ob_trap no_trap_int;

constexpr wrap_int add(wrap_int a, wrap_int b) {
  return a + b;
}

constexpr wrap_int preinc(wrap_int a) {
  ++a;
  return a;
}

constexpr wrap_int postinc(wrap_int a) {
  a++;
  return a;
}

constexpr wrap_int max = 2147483647;
constexpr wrap_int one = 1;
static_assert(add(max, one) == -2147483648, "constexpr wrapping failed");
static_assert(max+1 == -2147483648);
static_assert(preinc(max) == -2147483648);
static_assert(postinc(max) == -2147483648);
```

Since the first two static asserts work, I'd expect the last two to also work.

CC @JustinStitt 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timm Baeder (tbaederr)

<details>
See https://godbolt.org/z/fnd7KG3vM

```c++
#define __wrap __attribute__((overflow_behavior(wrap)))
#define __no_trap __attribute__((overflow_behavior(trap)))

typedef int __ob_wrap wrap_int;
typedef int __ob_trap no_trap_int;

constexpr wrap_int add(wrap_int a, wrap_int b) {
  return a + b;
}

constexpr wrap_int preinc(wrap_int a) {
  ++a;
  return a;
}

constexpr wrap_int postinc(wrap_int a) {
  a++;
  return a;
}

constexpr wrap_int max = 2147483647;
constexpr wrap_int one = 1;
static_assert(add(max, one) == -2147483648, "constexpr wrapping failed");
static_assert(max+1 == -2147483648);
static_assert(preinc(max) == -2147483648);
static_assert(postinc(max) == -2147483648);
```

Since the first two static asserts work, I'd expect the last two to also work.

CC @<!-- -->JustinStitt 
</details>


---

### Comment 2 - shafik

CC @AaronBallman 

---

### Comment 3 - JustinStitt

Looking into this :)

edit: I have a patch, just waiting for my tests to finish up before opening a PR.

---

### Comment 4 - JustinStitt

Hi,

PTAL https://github.com/llvm/llvm-project/pull/183826

---

