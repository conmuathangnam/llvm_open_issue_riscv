# [rejects valid] CTAD failing for alias template

**Author:** ericniebler
**URL:** https://github.com/llvm/llvm-project/issues/129077
**Status:** Closed
**Labels:** clang:frontend, rejects-valid
**Closed Date:** 2025-03-22T14:56:00Z

## Body

i believe (and a few folks in CWG agree) that the following code should compile. GCC trunk accepts it.

```c++
using size_t = decltype(sizeof(0));

struct index_type
{
  size_t value{~0ull};
  index_type() = default;
  constexpr index_type(size_t i) noexcept : value(i) {}
};

template <index_type... Extents>
struct extents
{
  constexpr extents(decltype(Extents)...) noexcept {}
};

template <class... Extents>
extents(Extents...) -> extents<(requires { Extents::value; } ? Extents{} : ~0ull)...>;

template <index_type... Index>
using index = extents<Index...>;

int main()
{
  extents i{0,0};
  auto j = extents<64,{}>({}, 42);

  index k{0,0};
  auto l = index<64,{}>({}, 42);
}
```

<details>

<summary>Diagnostic:</summary>

```
<source>:27:9: error: no viable constructor or deduction guide for deduction of template arguments of 'index'
   27 |   index k{0,0};
      |         ^
<source>:20:1: note: candidate template ignored: substitution failure: deduced incomplete pack <(no value), (no value)> for template parameter 'Extents'
   10 | template <index_type... Extents>
      |                         ~~~~~~~
   11 | struct extents
   12 | {
   13 |   constexpr extents(decltype(Extents)...) noexcept {}
   14 | };
   15 | 
   16 | template <class... Extents>
   17 | extents(Extents...) -> extents<(requires { Extents::value; } ? Extents{} : ~0ull)...>;
   18 | 
   19 | template <index_type... Index>
   20 | using index = extents<Index...>;
      | ^
<source>:20:1: note: implicit deduction guide declared as 'template <index_type ...Extents> requires __is_deducible(index, extents<Extents...>) index(decltype(Extents) ...) -> extents<Extents...>'
<source>:20:1: note: candidate function template not viable: requires 1 argument, but 2 were provided
   11 | struct extents
      |        ~~~~~~~
   12 | {
   13 |   constexpr extents(decltype(Extents)...) noexcept {}
   14 | };
   15 | 
   16 | template <class... Extents>
   17 | extents(Extents...) -> extents<(requires { Extents::value; } ? Extents{} : ~0ull)...>;
   18 | 
   19 | template <index_type... Index>
   20 | using index = extents<Index...>;
      | ^
<source>:20:1: note: implicit deduction guide declared as 'template <index_type ...Extents> requires __is_deducible(index, extents<Extents...>) index(extents<Extents...>) -> extents<Extents...>'
<source>:20:1: note: candidate template ignored: constraints not satisfied
<source>:20:1: note: cannot deduce template arguments for 'index' from 'extents<(requires { Extents::value; } ? Extents{} : ~0ULL)...>'
<source>:20:1: note: implicit deduction guide declared as 'template <> requires __is_deducible(index, extents<(requires { Extents::value; } ? Extents{} : ~0ULL)...>) index(Extents ...) -> extents<(requires { Extents::value; } ? Extents{} : ~0ULL)...>'
```

</details>

See https://godbolt.org/z/qxsTWGsdx

Christof Meerwald (@cmeerw) of CWG explains it thusly:

> You try to deduce `extents<(requires { Extents::value; } ? Extents{} : ~0ull)...>` from `extents<Index...>` (but can't deduce anything). As you didn't deduce anything, substituting into the original deduction guide doesn't change anything. So your `f'` is the same as `f` (and as you haven't deduced anything, you only use the template parameters from the original deduction guide, but none from the alias template). After doing deduction for `f'`, the only thing left to do is the deducible check (which also succeeds).

this issue seems distinct from #111216.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eric Niebler (ericniebler)

<details>
i believe (and a few folks in CWG agree) that the following code should compile. GCC trunk accepts it.

```c++
using size_t = decltype(sizeof(0));

struct index_type
{
  size_t value{~0ull};
  index_type() = default;
  constexpr index_type(size_t i) noexcept : value(i) {}
};

template &lt;index_type... Extents&gt;
struct extents
{
  constexpr extents(decltype(Extents)...) noexcept {}
};

template &lt;class... Extents&gt;
extents(Extents...) -&gt; extents&lt;(requires { Extents::value; } ? Extents{} : ~0ull)...&gt;;

template &lt;index_type... Index&gt;
using index = extents&lt;Index...&gt;;

int main()
{
  extents i{0,0};
  auto j = extents&lt;64,{}&gt;({}, 42);

  index k{0,0};
  auto l = index&lt;64,{}&gt;({}, 42);
}
```

&lt;details&gt;

&lt;summary&gt;Diagnostic:&lt;/summary&gt;

```
&lt;source&gt;:27:9: error: no viable constructor or deduction guide for deduction of template arguments of 'index'
   27 |   index k{0,0};
      |         ^
&lt;source&gt;:20:1: note: candidate template ignored: substitution failure: deduced incomplete pack &lt;(no value), (no value)&gt; for template parameter 'Extents'
   10 | template &lt;index_type... Extents&gt;
      |                         ~~~~~~~
   11 | struct extents
   12 | {
   13 |   constexpr extents(decltype(Extents)...) noexcept {}
   14 | };
   15 | 
   16 | template &lt;class... Extents&gt;
   17 | extents(Extents...) -&gt; extents&lt;(requires { Extents::value; } ? Extents{} : ~0ull)...&gt;;
   18 | 
   19 | template &lt;index_type... Index&gt;
   20 | using index = extents&lt;Index...&gt;;
      | ^
&lt;source&gt;:20:1: note: implicit deduction guide declared as 'template &lt;index_type ...Extents&gt; requires __is_deducible(index, extents&lt;Extents...&gt;) index(decltype(Extents) ...) -&gt; extents&lt;Extents...&gt;'
&lt;source&gt;:20:1: note: candidate function template not viable: requires 1 argument, but 2 were provided
   11 | struct extents
      |        ~~~~~~~
   12 | {
   13 |   constexpr extents(decltype(Extents)...) noexcept {}
   14 | };
   15 | 
   16 | template &lt;class... Extents&gt;
   17 | extents(Extents...) -&gt; extents&lt;(requires { Extents::value; } ? Extents{} : ~0ull)...&gt;;
   18 | 
   19 | template &lt;index_type... Index&gt;
   20 | using index = extents&lt;Index...&gt;;
      | ^
&lt;source&gt;:20:1: note: implicit deduction guide declared as 'template &lt;index_type ...Extents&gt; requires __is_deducible(index, extents&lt;Extents...&gt;) index(extents&lt;Extents...&gt;) -&gt; extents&lt;Extents...&gt;'
&lt;source&gt;:20:1: note: candidate template ignored: constraints not satisfied
&lt;source&gt;:20:1: note: cannot deduce template arguments for 'index' from 'extents&lt;(requires { Extents::value; } ? Extents{} : ~0ULL)...&gt;'
&lt;source&gt;:20:1: note: implicit deduction guide declared as 'template &lt;&gt; requires __is_deducible(index, extents&lt;(requires { Extents::value; } ? Extents{} : ~0ULL)...&gt;) index(Extents ...) -&gt; extents&lt;(requires { Extents::value; } ? Extents{} : ~0ULL)...&gt;'
```

&lt;/details&gt;

See https://godbolt.org/z/qxsTWGsdx

Christof Meerwald (@<!-- -->cmeerw) of CWG explains it thusly:

&gt; You try to deduce `extents&lt;(requires { Extents::value; } ? Extents{} : ~0ull)...&gt;` from `extents&lt;Index...&gt;` (but can't deduce anything). As you didn't deduce anything, substituting into the original deduction guide doesn't change anything. So your `f'` is the same as `f` (and as you haven't deduced anything, you only use the template parameters from the original deduction guide, but none from the alias template). After doing deduction for `f'`, the only thing left to do is the deducible check (which also succeeds).

this issue seems distinct from #<!-- -->111216.

</details>


---

### Comment 2 - shafik

CC @hokein 

---

