# [clang] Incorrect type error when using a variable member template of class template

**Author:** wreien
**URL:** https://github.com/llvm/llvm-project/issues/125181

## Body

Consider the following valid C++14 snippet (https://godbolt.org/z/jYGfzTf7P):
```cpp
template <typename>
struct S {
  template <typename> static const auto x = 1;
};

int r = S<int>::x<int>;
```
Clang trunk gives the following error:
```
<source>:6:5: error: cannot initialize a variable of type 'int' with an lvalue of type 'auto'
    6 | int r = S<int>::x<int>;
      |     ^   ~~~~~~~~~~~~~~
```
Removing either template resolves the error.  Marking `x` as `constexpr` also fixes in this particular case.

This error appears to have started with Clang 9.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Nathaniel Shead (wreien)

<details>
Consider the following valid C++14 snippet (https://godbolt.org/z/jYGfzTf7P):
```cpp
template &lt;typename&gt;
struct S {
  template &lt;typename&gt; static const auto x = 1;
};

int r = S&lt;int&gt;::x&lt;int&gt;;
```
Clang trunk gives the following error:
```
&lt;source&gt;:6:5: error: cannot initialize a variable of type 'int' with an lvalue of type 'auto'
    6 | int r = S&lt;int&gt;::x&lt;int&gt;;
      |     ^   ~~~~~~~~~~~~~~
```
Removing either template resolves the error.  Marking `x` as `constexpr` also fixes in this particular case.

This error appears to have started with Clang 9.
</details>


---

### Comment 2 - shafik

This straight up just looks like a bug that started around clang-8: https://godbolt.org/z/7adcd4Gxq

---

### Comment 3 - cor3ntin

@mizvekov 

---

