# Clang argument deduction failure within nested generic lambda

**Author:** sneed-deens
**URL:** https://github.com/llvm/llvm-project/issues/154065

## Body

Latest GCC and MSVC able to compile, latest Clang don't ([Godbolt](https://godbolt.org/z/9YbMbez67)).
Compiled with: `-std=c++20 -Wall -Wextra -Wpedantic`.

`x86-64 Clang 14.0.0` also able to compile (any version above don't).

Factoring out part with problematic deduction also makes it compile ([Godbolt](https://godbolt.org/z/Wv5zEncT3)).

```c++
using Index = int;
using Alignment = int;
struct Key {
  Index old;
  Alignment a;
};

template<Key k, typename T> struct Elem {};
template<typename ...Elems> struct Map : Elems... {};

template<typename ...Ts> struct Tuple {};

// removing 't' parameter makes it compile,
// so sort_tuple should be template to trigger error
constexpr auto sort_tuple(auto t)
{
  (void)t;
  using M = ::Map<
    ::Elem<::Key{0, 1}, char>
  >;

  // this lambda should be generic to trigger error
  using Sorted = decltype([]<auto ...Is>(
  ){
    return ::Tuple<
      decltype([]<::Key k, typename T>(
        [[maybe_unused]] ::Elem<k, T>&& deduced
      ){
        return T{};
      }.template operator()<::Key{0, 1}>(M{}))
    >{};
  }.template operator()<1,2,3>());

  return Sorted{};
};

using T1 = Tuple<char, short, int, double>;
using Sorted = decltype(sort_tuple(T1{}));

int main() {}
```
Error from Compiler Explorer:
```
<source>:30:18: error: no matching member function for call to 'operator()'
   26 |       decltype([]<::Key k, typename T>(
      |                ~~~~~~~~~~~~~~~~~~~~~~~~
   27 |         [[maybe_unused]] ::Elem<k, T>&& deduced
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   28 |       ){
      |       ~~
   29 |         return T{};
      |         ~~~~~~~~~~~
   30 |       }.template operator()<::Key{0, 1}>(M{}))
      |       ~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~
<source>:24:4: note: while substituting into a lambda expression here
   24 |   ){
      |    ^
<source>:38:25: note: in instantiation of function template specialization 'sort_tuple<Tuple<char, short, int, double>>' requested here
   38 | using Sorted = decltype(sort_tuple(T1{}));
      |                         ^
<source>:26:16: note: candidate template ignored: deduced type '::Elem<k, T>' of 1st parameter does not match adjusted type 'M' (aka 'Map< ::Elem< ::Key{0, 1}, char>>') of argument [with k = Key{0, 1}, T = char]
   26 |       decltype([]<::Key k, typename T>(
      |                ^
1 error generated.
Compiler returned: 1
```
This minimized example made from code that sorts tuple's types, where
GCC also unable to compile ([Godbolt](https://godbolt.org/z/YPh1aMnco), if interested).


## Comments

### Comment 1 - frederick-vs-ja

Slightly reduced ([Godbolt link](https://godbolt.org/z/7b97jd5M6)):
```C++
template<int, class> struct Base {};

// needs to be a function template
template<class = void>
void fun()
{
  // operator() needs to be a function template
  []<class = void>(){
    struct LocalDerived : Base<0, char> {};
    using X [[maybe_unused]] = decltype([]<int N, class T>(Base<N, T>&&)
      {}.template operator()<0>(LocalDerived{}));
  }();
};

template void fun<>();
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (sneed-deens)

<details>
Latest GCC and MSVC able to compile, latest Clang don't ([Godbolt](https://godbolt.org/z/9YbMbez67)).
Compiled with: `-std=c++20 -Wall -Wextra -Wpedantic`.

`x86-64 Clang 14.0.0` also able to compile (any version above don't).

Factoring out part with problematic deduction also makes it compile ([Godbolt](https://godbolt.org/z/Wv5zEncT3)).

```c++
using Index = int;
using Alignment = int;
struct Key {
  Index old;
  Alignment a;
};

template&lt;Key k, typename T&gt; struct Elem {};
template&lt;typename ...Elems&gt; struct Map : Elems... {};

template&lt;typename ...Ts&gt; struct Tuple {};

// removing 't' parameter makes it compile,
// so sort_tuple should be template to trigger error
constexpr auto sort_tuple(auto t)
{
  (void)t;
  using M = ::Map&lt;
    ::Elem&lt;::Key{0, 1}, char&gt;
  &gt;;

  // this lambda should be generic to trigger error
  using Sorted = decltype([]&lt;auto ...Is&gt;(
  ){
    return ::Tuple&lt;
      decltype([]&lt;::Key k, typename T&gt;(
        [[maybe_unused]] ::Elem&lt;k, T&gt;&amp;&amp; deduced
      ){
        return T{};
      }.template operator()&lt;::Key{0, 1}&gt;(M{}))
    &gt;{};
  }.template operator()&lt;1,2,3&gt;());

  return Sorted{};
};

using T1 = Tuple&lt;char, short, int, double&gt;;
using Sorted = decltype(sort_tuple(T1{}));

int main() {}
```
Error from Compiler Explorer:
```
&lt;source&gt;:30:18: error: no matching member function for call to 'operator()'
   26 |       decltype([]&lt;::Key k, typename T&gt;(
      |                ~~~~~~~~~~~~~~~~~~~~~~~~
   27 |         [[maybe_unused]] ::Elem&lt;k, T&gt;&amp;&amp; deduced
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   28 |       ){
      |       ~~
   29 |         return T{};
      |         ~~~~~~~~~~~
   30 |       }.template operator()&lt;::Key{0, 1}&gt;(M{}))
      |       ~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~
&lt;source&gt;:24:4: note: while substituting into a lambda expression here
   24 |   ){
      |    ^
&lt;source&gt;:38:25: note: in instantiation of function template specialization 'sort_tuple&lt;Tuple&lt;char, short, int, double&gt;&gt;' requested here
   38 | using Sorted = decltype(sort_tuple(T1{}));
      |                         ^
&lt;source&gt;:26:16: note: candidate template ignored: deduced type '::Elem&lt;k, T&gt;' of 1st parameter does not match adjusted type 'M' (aka 'Map&lt; ::Elem&lt; ::Key{0, 1}, char&gt;&gt;') of argument [with k = Key{0, 1}, T = char]
   26 |       decltype([]&lt;::Key k, typename T&gt;(
      |                ^
1 error generated.
Compiler returned: 1
```
This minimized example made from code that sorts tuple's types, where
GCC also unable to compile ([Godbolt](https://godbolt.org/z/YPh1aMnco), if interested).

</details>


---

