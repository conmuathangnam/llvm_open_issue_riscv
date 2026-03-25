# [Clang] Missing default template argument in synthesized guides during CTAD for alias template

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/164264

## Body

```c++
template <class T, auto = 0>
struct A {
    A(T) {}
};

template <class T, class U>
using Foo = A<T, U{}>;

template <class T, class U = int>
using Bar = Foo<T, U>;

Bar a{0};
```
Output:
```console
<source>:12:5: error: no viable constructor or deduction guide for deduction of template arguments of 'Bar'
   12 | Bar a{0};
      |     ^
<source>:10:1: note: candidate template ignored: couldn't infer template argument 'U'
   10 | using Bar = Foo<T, U>;
      | ^
<source>:10:1: note: implicit deduction guide declared as 'template <class T, class U> requires __is_deducible(Foo, A<T, U{}>) && __is_deducible(Bar, A<T, U{}>) Bar(T) -> A<T, U{}>'
<source>:10:1: note: candidate template ignored: could not match 'A<T, U{}>' against 'int'
<source>:10:1: note: implicit deduction guide declared as 'template <class T, class U> requires __is_deducible(Foo, A<T, U{}>) && __is_deducible(Bar, A<T, U{}>) Bar(A<T, U{}>) -> A<T, U{}>'
```
See https://compiler-explorer.com/z/vcjc1qEfv.
See also #133132.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
```c++
template &lt;class T, auto = 0&gt;
struct A {
    A(T) {}
};

template &lt;class T, class U&gt;
using Foo = A&lt;T, U{}&gt;;

template &lt;class T, class U = int&gt;
using Bar = Foo&lt;T, U&gt;;

Bar a{0};
```
Output:
```console
&lt;source&gt;:12:5: error: no viable constructor or deduction guide for deduction of template arguments of 'Bar'
   12 | Bar a{0};
      |     ^
&lt;source&gt;:10:1: note: candidate template ignored: couldn't infer template argument 'U'
   10 | using Bar = Foo&lt;T, U&gt;;
      | ^
&lt;source&gt;:10:1: note: implicit deduction guide declared as 'template &lt;class T, class U&gt; requires __is_deducible(Foo, A&lt;T, U{}&gt;) &amp;&amp; __is_deducible(Bar, A&lt;T, U{}&gt;) Bar(T) -&gt; A&lt;T, U{}&gt;'
&lt;source&gt;:10:1: note: candidate template ignored: could not match 'A&lt;T, U{}&gt;' against 'int'
&lt;source&gt;:10:1: note: implicit deduction guide declared as 'template &lt;class T, class U&gt; requires __is_deducible(Foo, A&lt;T, U{}&gt;) &amp;&amp; __is_deducible(Bar, A&lt;T, U{}&gt;) Bar(A&lt;T, U{}&gt;) -&gt; A&lt;T, U{}&gt;'
```
See https://compiler-explorer.com/z/vcjc1qEfv.
See also #<!-- -->133132.
</details>


---

