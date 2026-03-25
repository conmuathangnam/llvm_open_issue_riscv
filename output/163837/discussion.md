# Lambda evaluated for constant template parameter of function inside templated generic lambda incorrectly complains about local variables needing captures

**Author:** MitalAshok
**URL:** https://github.com/llvm/llvm-project/issues/163837

## Body

This is a reduced example <https://godbolt.org/z/WzshaKvxW>:

```c++
// -std=c++20 -fsyntax-only
template<int N>
void f();

template<class>
struct X {
    using type = decltype([](auto) {
        f<[]{
            int result = 0;
            return result;
        }()>();
    }(0));
};

X<void> l;
```

```
<source>:9:20: error: variable 'result' cannot be implicitly captured in a lambda with no capture-default specified
    9 |             return result;
      |                    ^
<source>:7:13: note: while substituting into a lambda expression here
    7 |         f<[]{
      |             ^
<source>:6:27: note: in instantiation of function template specialization 'X<void>::(anonymous class)::operator()<int>' requested here
    6 |     using type = decltype([](auto) {
      |                           ^
<source>:14:9: note: in instantiation of template class 'X<void>' requested here
   14 | X<void> l;
      |         ^
<source>:8:17: note: 'result' declared here
    8 |             int result = 0;
      |                 ^
<source>:7:11: note: lambda expression begins here
    7 |         f<[]{
      |           ^
<source>:7:12: note: capture 'result' by value
    7 |         f<[]{
      |            ^
      |            result
<source>:7:12: note: capture 'result' by reference
    7 |         f<[]{
      |            ^
      |            &result
<source>:7:12: note: default capture by value
    7 |         f<[]{
      |            ^
      |            =
<source>:7:12: note: default capture by reference
    7 |         f<[]{
      |            ^
      |            &
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mital Ashok (MitalAshok)

<details>
This is a reduced example &lt;https://godbolt.org/z/WzshaKvxW&gt;:

```c++
// -std=c++20 -fsyntax-only
template&lt;int N&gt;
void f();

template&lt;class&gt;
struct X {
    using type = decltype([](auto) {
        f&lt;[]{
            int result = 0;
            return result;
        }()&gt;();
    }(0));
};

X&lt;void&gt; l;
```

```
&lt;source&gt;:9:20: error: variable 'result' cannot be implicitly captured in a lambda with no capture-default specified
    9 |             return result;
      |                    ^
&lt;source&gt;:7:13: note: while substituting into a lambda expression here
    7 |         f&lt;[]{
      |             ^
&lt;source&gt;:6:27: note: in instantiation of function template specialization 'X&lt;void&gt;::(anonymous class)::operator()&lt;int&gt;' requested here
    6 |     using type = decltype([](auto) {
      |                           ^
&lt;source&gt;:14:9: note: in instantiation of template class 'X&lt;void&gt;' requested here
   14 | X&lt;void&gt; l;
      |         ^
&lt;source&gt;:8:17: note: 'result' declared here
    8 |             int result = 0;
      |                 ^
&lt;source&gt;:7:11: note: lambda expression begins here
    7 |         f&lt;[]{
      |           ^
&lt;source&gt;:7:12: note: capture 'result' by value
    7 |         f&lt;[]{
      |            ^
      |            result
&lt;source&gt;:7:12: note: capture 'result' by reference
    7 |         f&lt;[]{
      |            ^
      |            &amp;result
&lt;source&gt;:7:12: note: default capture by value
    7 |         f&lt;[]{
      |            ^
      |            =
&lt;source&gt;:7:12: note: default capture by reference
    7 |         f&lt;[]{
      |            ^
      |            &amp;
```

</details>


---

### Comment 2 - shafik

CC @cor3ntin 

---

### Comment 3 - Backl1ght

I think we should return true here https://github.com/llvm/llvm-project/blob/035f81138888f30aab67ee12ce09982cd03370cb/clang/lib/Sema/SemaExpr.cpp#L19399 , but we won't currently due to mismatch of `VarDC` and `DC`. It looks like `DC` is instantiated from `VarDC`.

---

