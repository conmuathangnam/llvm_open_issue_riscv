# [clang] Regression in lambda return type inference

**Author:** ojhunt
**URL:** https://github.com/llvm/llvm-project/issues/153884
**Status:** Closed
**Labels:** c++, clang:frontend, rejects-valid, confirmed, lambda, regression:21
**Closed Date:** 2025-08-18T09:07:28Z

## Body

I've found a regression in the return type inference in lambda expressions that seems fairly severe

```cpp
int v;
bool foo() {
  auto init = [](auto) { return true; };
  if constexpr (0)
    return init(v);
  return false;
}
```

This compiles in clang20 and gcc, but not in main.

I believe it to be broken in clang21 as well, but I'm not sure if I was building the correct branch so will need to re-test.

Pre-emptively tagging as a 21 regression as I am reasonably certain that it does repro and want people to see this in case they have any immediate ideas of what might be the cause before I start looking at this myself.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-1

Author: Oliver Hunt (ojhunt)

<details>
I've found a regression in the return type inference in lambda expressions that seems fairly severe

```cpp
int v;
bool foo() {
  auto init = [](auto) { return true; };
  if constexpr (0)
    return init(v);
  return false;
}
```

This compiles in clang20 and gcc, but not in main.

I believe it to be broken in clang21 as well, but I'm not sure if I was building the correct branch so will need to re-test.

Pre-emptively tagging as a 21 regression as I am reasonably certain that it does repro and want people to see this in case they have any immediate ideas of what might be the cause before I start looking at this myself.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Oliver Hunt (ojhunt)

<details>
I've found a regression in the return type inference in lambda expressions that seems fairly severe

```cpp
int v;
bool foo() {
  auto init = [](auto) { return true; };
  if constexpr (0)
    return init(v);
  return false;
}
```

This compiles in clang20 and gcc, but not in main.

I believe it to be broken in clang21 as well, but I'm not sure if I was building the correct branch so will need to re-test.

Pre-emptively tagging as a 21 regression as I am reasonably certain that it does repro and want people to see this in case they have any immediate ideas of what might be the cause before I start looking at this myself.
</details>


---

### Comment 3 - ojhunt

@cor3ntin looks like 5c1db3823e0d7ac820c938ca81adbffda3d98219 is responsible

---

### Comment 4 - ojhunt

Ok, this is is specific to lambdas:

```cpp

auto global_f(auto d)  {
    return true;
}
bool foo1() {
  if constexpr (0)
    return global_f(1);
  return false;
}

bool foo2() {
    auto local_f = [](auto) { return true; };
    if constexpr (0)
        return local_f(1);
    return false;
}

```

The lambda version trips this but the "equivalent" auto function does not. I assume that we're doing something akin to "this is discarded so we should not instantiate it", but that then leads to us not getting the return type - it's not quite that trivial though because we do instantiate enough of it to test for errors.


---

### Comment 5 - ojhunt

Somewhat expectedly the first instantiation determines the deduced return type, implying this specifically a change in the way deduction is being performed during instantiation in a discarded context:


```cpp
bool f1() {
  auto l = [](auto) { return true; };
  if constexpr (1)
    return l(1); // Succeeds
  return false;
}

bool f2() {
  auto l = [](auto) { return true; };
  if constexpr (1) l(1);
  if constexpr (0)
    return l(1); // Succeeds
  return false;
}

bool f3() {
  auto l = [](auto) { return true; };
  if constexpr (0) l(1);
  if constexpr (1)
    return l(1); // Fails
  return false;
}
```

e.g the first instantiation deduces a void result type (somehow), and that is the return type in the subsequent uses as it has already been instantiated.

---

