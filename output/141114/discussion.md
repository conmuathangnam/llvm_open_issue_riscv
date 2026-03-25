# Unknown references and pointers related bad codegen

**Author:** shafik
**URL:** https://github.com/llvm/llvm-project/issues/141114
**Status:** Closed
**Labels:** clang:frontend, miscompilation, needs-reduction
**Closed Date:** 2025-06-03T16:51:38Z

## Body

This semi-reduced code from: https://github.com/llvm/llvm-project/pull/95474#issuecomment-2884165093

```cpp
struct MyStruct {
    double m0{-9999.0};
    int m1{-12345};
};

constexpr MyStruct default_val;

auto compute_area(double l, const MyStruct &val = default_val) -> double {
    if (val.m1 == 1)
      return 2.0;
    return 0;
}


#include <cassert>


auto main() -> int {
    MyStruct in_val{.m0 = 2.0, .m1 = 1};
    double val = compute_area(1.0, in_val);
    assert(val == 2.0);
}
```

crashes at runtime, it looks like due to `VD->getAnyInitializer(VD)` pulling in the default value.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Shafik Yaghmour (shafik)

<details>
This semi-reduced code from: https://github.com/llvm/llvm-project/pull/95474#issuecomment-2884165093

```cpp
struct MyStruct {
    double m0{-9999.0};
    int m1{-12345};
};

constexpr MyStruct default_val;

auto compute_area(double l, const MyStruct &amp;val = default_val) -&gt; double {
    if (val.m1 == 1)
      return 2.0;
    return 0;
}


#include &lt;cassert&gt;


auto main() -&gt; int {
    MyStruct in_val{.m0 = 2.0, .m1 = 1};
    double val = compute_area(1.0, in_val);
    assert(val == 2.0);
}
```

crashes at runtime, it looks like due to `VD-&gt;getAnyInitializer(VD)` pulling in the default value.
</details>


---

### Comment 2 - shafik

C++20 results Vs C++23 results:

https://godbolt.org/z/v1EcsxhEa

---

### Comment 3 - shafik

Further reduced: https://godbolt.org/z/hTMa3EG73

```cpp
struct MyStruct {
    int m1{-12345};
};

constexpr MyStruct default_val;

auto compute_area(double l, const MyStruct &val = default_val) -> int {
    if (val.m1 == -12345)
      return 2;
    return 0;
}


int main() {
    return compute_area(1.0, {.m1 = 1}) == 2;
}
```

---

### Comment 4 - frederick-vs-ja

Further reduced. https://godbolt.org/z/67j641dKW

```C++
constexpr int arbitrary = -12345;

bool check_value(const int &n = arbitrary) {
    if (n == -12345)
        return true;
    return false;
}

int main() {
    return check_value(0);
}
```

The class is unnecessary. It seems that the bug is triggered whenever the default argument for a reference parameter is a constexpr variable.

---

### Comment 5 - shafik

IR seems to show it just default to returning `2` for the original case: https://godbolt.org/z/vzvTvde7j

```llvm
%struct.MyStruct = type { i32 }

define dso_local noundef i32 @compute_area(double, MyStruct const&)(double noundef %l, ptr noundef nonnull align 4 dereferenceable(4) %val) {
entry:
  %l.addr = alloca double, align 8
  %val.addr = alloca ptr, align 8
  store double %l, ptr %l.addr, align 8
  store ptr %val, ptr %val.addr, align 8
  ret i32 2
}
```

@frederick-vs-ja case also does the same thing: https://godbolt.org/z/vad417G8b

I am debugging and the difference may be here: 

https://github.com/llvm/llvm-project/blob/720014f70841f0284d21ef8100c406d6c864ac9c/clang/lib/CodeGen/CGStmt.cpp#L902

in C++20 this is `false` but it is `true` in C++23 for this code.

---

### Comment 6 - shafik

@efriedma-quic This kind of feels like a unknown reference leaking into codegen but maybe this is not the same as the earlier issue that was fixed the other day here: https://github.com/llvm/llvm-project/pull/128409

The contexpr-unknown `VarDecl` does not leak directly into codegen like in that case it is only during checking if it foldable. Maybe we need a way to tell evaluation we are in codegen?

If you can see a more clever way, let me know and I can post a fix. 

---

### Comment 7 - efriedma-quic

I don't think it has anything to do with codegen; it's just a straight bug in constant evaluation.  Consider:

```
constexpr int arbitrary = -12345;
void f(const int &n = arbitrary) {
  static_assert(n==arbitrary);
}
```

I think we just need to fix constant evaluation so it never looks at the initializers of default arguments.  (A constexpr function can have default arguments, but in that case there's a CXXDefaultArgExpr.)

---

### Comment 8 - zygoloid

I wonder if we should consider making `VarDecl::getInit` and similar functions not find default arguments? The root cause here seems to be that this is a confusing and error-prone interface. I'm sure this isn't the first time I've seen bugs due to `getInit` finding things that aren't a normal variable initializer. (Larger change, but if we were doing things over again I'd probably not make `ParmVarDecl` derive from `VarDecl` but instead make both derive from some common base class...)

---

### Comment 9 - shafik

> I wonder if we should consider making `VarDecl::getInit` and similar functions not find default arguments? The root cause here seems to be that this is a confusing and error-prone interface. I'm sure this isn't the first time I've seen bugs due to `getInit` finding things that aren't a normal variable initializer. (Larger change, but if we were doing things over again I'd probably not make `ParmVarDecl` derive from `VarDecl` but instead make both derive from some common base class...)

So I can make a first approach of rejecting default arguments during constant eval and then work on the larger change as a follow-up. Can you outline more details about how you would approach it?

---

