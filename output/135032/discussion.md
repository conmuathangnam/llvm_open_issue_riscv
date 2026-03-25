# [clang] Crash instantiating static variable template specialization member of class template

**Author:** katzdm
**URL:** https://github.com/llvm/llvm-project/issues/135032
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-05-18T10:42:08Z

## Body

The following program crashes during codegen, and looks to have done so since Clang 9.

```cpp
template <typename>
struct TCls {
  template <typename T>
  static auto var = T::smem;
};

struct S { static constexpr int smem = 1; };

int main() {
  (void) TCls<int>::var<S>;
}

```

https://godbolt.org/z/x9eYMKfrv

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Daniel M. Katz (katzdm)

<details>
The following program crashes during codegen, and looks to have done so since Clang 9.

```cpp
template &lt;typename&gt;
struct TCls {
  template &lt;typename T&gt;
  static auto var = T::smem;
};

struct S { static constexpr int smem = 1; };

int main() {
  (void) TCls&lt;int&gt;::var&lt;S&gt;;
}

```

https://godbolt.org/z/x9eYMKfrv
</details>


---

### Comment 2 - shafik

edg rejects your original version: https://godbolt.org/z/3Y6eT17T7 b/c `var` is not constant.


Note edg accepts this version, clang still crashes https://godbolt.org/z/accxTdhE3

```cpp
template <typename T1>
struct TCls {
  template <typename T2>
  const static auto var = T1::smem;
};

struct S { static constexpr int smem = 1; };

int main() {
  (void) TCls<S>::var<S>;
}
```



---

### Comment 3 - dty2

Hi, I'd like to take ownership of this issue and work on a fix. I'll investigate the root cause and provide updates here as I progress. Thanks!

---

### Comment 4 - dty2

Sorry for the long wait. The pr is https://github.com/llvm/llvm-project/pull/138122

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Daniel M. Katz (katzdm)

<details>
The following program crashes during codegen, and looks to have done so since Clang 9.

```cpp
template &lt;typename&gt;
struct TCls {
  template &lt;typename T&gt;
  static auto var = T::smem;
};

struct S { static constexpr int smem = 1; };

int main() {
  (void) TCls&lt;int&gt;::var&lt;S&gt;;
}

```

https://godbolt.org/z/x9eYMKfrv
</details>


---

