# Pack indexing expression is not being instantiated

**Author:** Eczbek
**URL:** https://github.com/llvm/llvm-project/issues/121242
**Status:** Closed
**Labels:** clang:frontend, confirmed
**Closed Date:** 2025-01-01T07:50:07Z

## Body

Compiler Explorer: https://godbolt.org/z/TG4hMMPv3

```cpp
template<int... x>
int y = x...[0];

int main() {
	y<0>;
}
```
```
<source>:2:5: error: cannot initialize a variable of type 'int' with an lvalue of type '<dependent type>'
    2 | int y = x...[0];
      |     ^   ~~~~~~~
<source>:5:2: warning: expression result unused [-Wunused-value]
    5 |         y<0>;
      |         ^~~~
1 warning and 1 error generated.
Compiler returned: 1
```


## Comments

### Comment 1 - Sirraide

CC @cor3ntin 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Eczbek)

<details>
Compiler Explorer: https://godbolt.org/z/TG4hMMPv3

```cpp
template&lt;int... x&gt;
int y = x...[0];

int main() {
	y&lt;0&gt;;
}
```
```
&lt;source&gt;:2:5: error: cannot initialize a variable of type 'int' with an lvalue of type '&lt;dependent type&gt;'
    2 | int y = x...[0];
      |     ^   ~~~~~~~
&lt;source&gt;:5:2: warning: expression result unused [-Wunused-value]
    5 |         y&lt;0&gt;;
      |         ^~~~
1 warning and 1 error generated.
Compiler returned: 1
```

</details>


---

### Comment 3 - zyn0217

It’s not that we're missing an instantiation for y<0>, but rather that we checked the initialization too early, right after the variable template was constructed. This is because the first PackIndexingExpr was assigned a ValueInstantiation dependency but retained a dependent type, which caused the logic to be skipped.

https://github.com/llvm/llvm-project/blob/48bf0a9457fd60d0872d9b9b4804a95c833a72e1/clang/lib/Sema/SemaInit.cpp#L6431-L6435

https://github.com/llvm/llvm-project/blob/48bf0a9457fd60d0872d9b9b4804a95c833a72e1/clang/lib/AST/ExprCXX.cpp#L1722-L1725

We should probably set the expr type to `PackIdExpr->getType()` on line 1725.

This also indicates that a workaround is to convert the NTTP to a dependent type, i.e. the following would bypass the initialization check that happens before we see `y<0>` and its instantiation

```cpp
template<auto... x>
int y = x...[0];

int main() {
	y<0>;
}
```

---

### Comment 4 - TilakChad

The changes suggested by @zyn0217 seems to be sufficient for resolving the issue. 
I'll create a PR with those changes and relevant test cases. 

---

