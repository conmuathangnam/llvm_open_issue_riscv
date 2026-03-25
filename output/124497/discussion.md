# [Clang] Wrong constraint evaluation for lambda in unevaluated context

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/124497

## Body

The following should be valid C++20 code:
```c++
template <class T>
using F = decltype([] (auto a) requires (sizeof(T) > sizeof a) {});

int main() {
    F<int> f;
    f(char{});
}
```

Clang rejects with:
```console
<source>:6:5: error: no matching function for call to object of type 'F<int>' (aka '(lambda at <source>:2:20)')
    6 |     f(char{});
      |     ^
<source>:2:20: note: candidate template ignored: constraints not satisfied [with a:auto = char]
    2 | using F = decltype([] (auto a) requires (sizeof(T) > sizeof a) {});
      |                    ^
<source>:2:42: note: because 'sizeof(char) > sizeof a' (1 > 1) evaluated to false
    2 | using F = decltype([] (auto a) requires (sizeof(T) > sizeof a) {});
      |                                          ^
```

Somehow it got `char` substituted into `T` in `sizeof(T)`?

See https://compiler-explorer.com/z/YvnzozWvx.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
The following should be valid C++20 code:
```c++
template &lt;class T&gt;
using F = decltype([] (auto a) requires (sizeof(T) &gt; sizeof a) {});

int main() {
    F&lt;int&gt; f;
    f(char{});
}
```

Clang rejects with:
```console
&lt;source&gt;:6:5: error: no matching function for call to object of type 'F&lt;int&gt;' (aka '(lambda at &lt;source&gt;:2:20)')
    6 |     f(char{});
      |     ^
&lt;source&gt;:2:20: note: candidate template ignored: constraints not satisfied [with a:auto = char]
    2 | using F = decltype([] (auto a) requires (sizeof(T) &gt; sizeof a) {});
      |                    ^
&lt;source&gt;:2:42: note: because 'sizeof(char) &gt; sizeof a' (1 &gt; 1) evaluated to false
    2 | using F = decltype([] (auto a) requires (sizeof(T) &gt; sizeof a) {});
      |                                          ^
```

Somehow it got `char` substituted into `T` in `sizeof(T)`?

See https://compiler-explorer.com/z/YvnzozWvx.
</details>


---

### Comment 2 - zyn0217

@mizvekov 

---

### Comment 3 - shafik

This looks like a clang-16 regression: https://compiler-explorer.com/z/bsq9cT134

@zyn0217 do you think you know which PR regressed this?

@MagentaTreehouse in general it is super helpful to see how far back an issue goes. This is super helpful especially if it is a recent regression.

---

