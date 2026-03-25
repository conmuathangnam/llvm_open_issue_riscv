# clang fails to find operator `->`

**Author:** ahatanak
**URL:** https://github.com/llvm/llvm-project/issues/104268

## Body

```cpp
struct D {
  template <typename T> void m() {}
};

template <typename T> struct S {
  void init() { (*this)->template m<T>(); };

  D *operator->();
};
```

clang rejects the code with the following error message:
```
<source>:6:35: error: no member named 'm' in 'S<T>'
    6 |   void init() { (*this)->template m<T>(); };
```
It looks like clang's behavior changed after [1595988ee6f9732e7ea79928af8a470ad5ef7dbe](https://github.com/llvm/llvm-project/pull/92283).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Akira Hatanaka (ahatanak)

<details>
```
struct D {
  template &lt;typename T&gt; void m() {}
};

template &lt;typename T&gt; struct S {
  void init() { (*this)-&gt;template m&lt;T&gt;(); };

  D *operator-&gt;();
};
```

clang rejects the code with the following error message:

&lt;source&gt;:6:35: error: no member named 'm' in 'S&lt;T&gt;'
    6 |   void init() { (*this)-&gt;template m&lt;T&gt;(); };

It looks like clang's behavior changed after 1595988ee6f9732e7ea79928af8a470ad5ef7dbe.
</details>


---

### Comment 2 - ahatanak

@sdkrystian is this expected?

---

### Comment 3 - sdkrystian

@ahatanak This is rather interesting... we accept this:
```cpp

struct D {
  template <typename T = 0> void m() {}
};

template <typename T> struct S {
  void init() { (*this)->m(); };

  D *operator->();
};
```
... but we reject your provided example. I have some ideas as to what is going wrong... I'll investigate tomorrow. 

---

### Comment 4 - h-vetinari

This should probably be reopened, given that https://github.com/llvm/llvm-project/commit/3681d8552fb9e6cb15e9d45849ff2e34a25c518e reverted https://github.com/llvm/llvm-project/pull/104458

---

### Comment 5 - porglezomp

I see the new PR is here: https://github.com/llvm/llvm-project/pull/109422

---

### Comment 6 - cor3ntin

@sdkrystian this is still broken https://godbolt.org/z/z4hGqTxjE

---

### Comment 7 - porglezomp

It looks like the follow-up PR was reverted again.

---

