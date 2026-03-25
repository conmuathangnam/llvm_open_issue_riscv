# Clang pretends to supports indexing a pack of template-names

**Author:** cor3ntin
**URL:** https://github.com/llvm/llvm-project/issues/135550

## Body

```cpp
template <
    template <typename> typename... TT>
struct S {
    using First = TT...[0];
};

template <typename T>
struct U;
using A = S<U>::First;
A a;
```

https://compiler-explorer.com/z/b663d38hs


This code is not valid C++26 as pack indexing is only supported for types and expressions.
Clang fails to reject it, which subsequently leads to a crash at codegen.


Note that I'm writing the paper to make this well-formed, and I wonder if we shouldn't preemptively accept that as an extension or do nothing until the next WG21 meeting.

At the same time, adding a diagnostic to reject it should be easy.







## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: cor3ntin (cor3ntin)

<details>
```cpp
template &lt;
    template &lt;typename&gt; typename... TT&gt;
struct S {
    using First = TT...[0];
};

template &lt;typename T&gt;
struct U;
using A = S&lt;U&gt;::First;
A a;
```

https://compiler-explorer.com/z/b663d38hs


This code is not valid C++26 as pack indexing is only supported for types and expressions.
Clang fails to reject it, which subsequently leads to a crash at codegen.


Note that I'm writing the paper to make this well-formed, and I wonder if we shouldn't preemptively accept that as an extension or do nothing until the next WG21 meeting.

At the same time, adding a diagnostic to reject it should be easy.






</details>


---

