# [Clang] [CodeGen] Wrong code generation for `extern reference`

**Author:** 16bit-ykiko
**URL:** https://github.com/llvm/llvm-project/issues/127475
**Status:** Closed
**Labels:** clang:frontend, clang:codegen, regression, miscompilation
**Closed Date:** 2025-03-05T06:01:26Z

## Body

Minimal Reproducible Example:

```cpp
/// src.cpp
#include <vector>

struct S {
    std::vector<int> c;
};

static S ls = {{1, 2, 3}};

S& s = ls;

/// main.cpp
#include <cstdio>
#include <vector>

struct S {
    std::vector<int> c;
};

extern S& s;

int main() {
    for (size_t i = 0; i < s.c.size(); i++) {
        printf("aaa\n");
    }

    for (auto& i : s.c) {
        printf("bbb\n");
    }
    return 0;
}
```

https://godbolt.org/z/W1YoYsMYh

## Comments

### Comment 1 - dtcxzyw

Reduced: https://godbolt.org/z/TMMhcEo7s

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: ykiko (16bit-ykiko)

<details>
Minimal Reproducible Example:

```cpp
/// src.cpp
#include &lt;vector&gt;

struct S {
    std::vector&lt;int&gt; c;
};

static S ls = {{1, 2, 3}};

S&amp; s = ls;

/// main.cpp
#include &lt;cstdio&gt;
#include &lt;vector&gt;

struct S {
    std::vector&lt;int&gt; c;
};

extern S&amp; s;

int main() {
    for (size_t i = 0; i &lt; s.c.size(); i++) {
        printf("aaa\n");
    }

    for (auto&amp; i : s.c) {
        printf("bbb\n");
    }
    return 0;
}
```

https://godbolt.org/z/W1YoYsMYh
</details>


---

### Comment 3 - sakria9

I use git bisect and find commit 0a9c08c , which emits

```llvm
  store ptr getelementptr (i8, ptr @s, i64 1), ptr %__range1, align 8
  %3 = load ptr, ptr %__range1, align 8
  %call1 = call ptr @_ZNSt6vectorIiSaIiEE5beginEv(ptr noundef nonnull align 8 dereferenceable(24) %3) #1
```

then commit is c9637af (which try to fix 0a9c08c #95474 ) , which emits:

```llvm
  store ptr @s, ptr %__range1, align 8
  %3 = load ptr, ptr %__range1, align 8
  %call1 = call ptr @_ZNSt6vectorIiSaIiEE5beginEv(ptr noundef nonnull align 8 dereferenceable(24) %3) #1
```

---

### Comment 4 - dtcxzyw

Related code:
https://github.com/llvm/llvm-project/blob/fb29f19fdb0b2b3c8c87cc767482d941818e92a8/clang/lib/CodeGen/CGDecl.cpp#L1988-L1992

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: ykiko (16bit-ykiko)

<details>
Minimal Reproducible Example:

```cpp
/// src.cpp
#include &lt;vector&gt;

struct S {
    std::vector&lt;int&gt; c;
};

static S ls = {{1, 2, 3}};

S&amp; s = ls;

/// main.cpp
#include &lt;cstdio&gt;
#include &lt;vector&gt;

struct S {
    std::vector&lt;int&gt; c;
};

extern S&amp; s;

int main() {
    for (size_t i = 0; i &lt; s.c.size(); i++) {
        printf("aaa\n");
    }

    for (auto&amp; i : s.c) {
        printf("bbb\n");
    }
    return 0;
}
```

https://godbolt.org/z/W1YoYsMYh
</details>


---

