# clang frontend in clang 20 uses >60GB of RAM (and crashes with OOM) to compile a small template meta-program that compiles fine in clang 18 and 19

**Author:** dwith-ts
**URL:** https://github.com/llvm/llvm-project/issues/154820
**Status:** Closed
**Labels:** clang:frontend, crash-on-valid, regression:20
**Closed Date:** 2026-01-20T17:58:17Z

## Body

Hi, 

I have a condensed template meta-program below which causes clang 20.1.0 to use a large amount of memory (verified that it uses more than 60GB) and then it crashes with out-of-memory on my machine.

The program compiles without problems with clang 18 and clang 19. I used C++17 option for all compilers.

Using the instructions from https://llvm.org/docs/HowToSubmitABug.html, this seems to be a frontend bug.

For a quick try, here is the godbolt link: https://godbolt.org/z/67vejdxMe

Code example:
```
#include <tuple>
#include <cstdint>

template<typename...> struct TypeList{};

template<typename T1, typename T2> struct Pair {
    using FirstType = T1;
    using SecondType = T2;
};

template <typename T1, typename T2>
T2 remove_from_start;

template <typename T1, typename T2>
using RemoveFromStart = decltype(remove_from_start<T1, T2>);

template <typename T, typename... Rest1, typename... Rest2>
RemoveFromStart<TypeList<Rest1...>, TypeList<Rest2...>>
    remove_from_start<TypeList<T, Rest1...>,
                      TypeList<T, Rest2...>>;

template <typename T>
constexpr std::int32_t size_of = -1;

template <template <class...> class L, typename... Ts>
constexpr std::int32_t size_of<L<Ts...>> = sizeof...(Ts);


template <bool>
struct CheckingArgNr;

template <typename AllRows_T, typename AllCols_T, typename... Others_T>
using CheckConstructorArgs = typename CheckingArgNr<(
    sizeof...(Others_T) > 0)>::template Result<1, AllRows_T, AllCols_T, TypeList<>, AllCols_T, Others_T...>::FirstType;


template <typename RowsT, typename AllRowsT>
using IsStartOf = std::bool_constant<size_of<AllRowsT> - size_of<RemoveFromStart<RowsT, AllRowsT>> == size_of<RowsT>>;

template <bool>
struct RemoveFromStartOfOr;

template <>
struct RemoveFromStartOfOr<true> {
  template <typename StartList_T, typename FullList_T, typename>
  using type = RemoveFromStart<StartList_T, FullList_T>;
};
template <>
struct RemoveFromStartOfOr<false> {
  template <typename, typename, typename Fallback_T>
  using type = Fallback_T;
};

template <>
struct CheckingArgNr<true> {
  template <typename Pair_T>
  using RowList = typename Pair_T::FirstType;
  template <typename Pair_T>
  using ColList = typename Pair_T::SecondType;

  template <std::int32_t counter_v,
            typename RemainingRows_T,
            typename AllCols_T,
            typename CurrentRows_T,
            typename RemainingCols_T,
            typename CurrentPair_T,
            typename... Others_T>
  using NextArgumentResult = typename CheckingArgNr<(sizeof...(Others_T) > 0)>::template Result<
      counter_v + 1,
      typename RemoveFromStartOfOr<  //
          std::is_same<ColList<CurrentPair_T>,
                 RemainingCols_T>::value          
          >::template type<RowList<CurrentPair_T>,
                           RemainingRows_T,       
                           RemainingRows_T>,      
      AllCols_T,
      std::conditional_t<std::is_same<ColList<CurrentPair_T>, RemainingCols_T>::value, TypeList<>, RowList<CurrentPair_T>>,
      typename RemoveFromStartOfOr<not std::is_same<ColList<CurrentPair_T>, RemainingCols_T>::value>::
          template type<ColList<CurrentPair_T>, RemainingCols_T, AllCols_T>,
      Others_T...>;

  template <typename RemainingRows_T,
            typename AllCols_T,
            typename CurrentRows_T,
            typename RemainingCols_T,
            typename CurrentPair_T>
  static constexpr bool current_argument_check =
      IsStartOf<ColList<CurrentPair_T>, RemainingCols_T>::value &&
      (((size_of<CurrentRows_T> == 0) && IsStartOf<RowList<CurrentPair_T>, RemainingRows_T>::value &&
        std::is_same<RemainingCols_T, AllCols_T>::value) ||
       std::is_same<RowList<CurrentPair_T>, CurrentRows_T>::value);  

  template <std::int32_t counter_v,
            typename RemainingRows_T,
            typename AllCols_T,
            typename CurrentRows_T,
            typename RemainingCols_T,
            typename CurrentPair_T,
            typename... Others_T>
  using Result = std::conditional_t<
      current_argument_check<RemainingRows_T, AllCols_T, CurrentRows_T, RemainingCols_T, CurrentPair_T>,
      NextArgumentResult<counter_v,
                         RemainingRows_T,
                         AllCols_T,
                         CurrentRows_T,
                         RemainingCols_T,
                         CurrentPair_T,
                         Others_T...>,
      Pair<std::false_type, std::integral_constant<std::int32_t, counter_v>>>;           
};

template <>
struct CheckingArgNr<false> {
  template <std::int32_t counter_v,
            typename RemainingRows_T,
            typename AllCols_T,
            typename CurrentRows_T,
            typename RemainingCols_T>
  using Result = Pair<
      std::bool_constant<(size_of<RemainingRows_T> == 0)>,
      std::integral_constant<std::int32_t, (size_of<RemainingRows_T> == 0) ? -1 : counter_v>>;
};

struct A; struct B; struct C; struct D; struct E; struct F; struct G; 
struct H; struct I; struct J;

int main()  {
  using RowList1 = TypeList<A, B, H, I>;
  using ColList1 = TypeList<A, B, C, D, F, G>;

  // this already crashes / uses a huge amount of memory with clang 20
  using Result = CheckConstructorArgs<RowList1, ColList1, 
        Pair<A, A>, Pair<A, B>, Pair<A, C>, Pair<A, D>,
        Pair<A, F>, Pair<A, G>,
        Pair<B, A>, Pair<B, B>, Pair<B, C>, Pair<B, D>,
        Pair<B, F>, Pair<B, G>,
        Pair<H, A>, Pair<H, B>, Pair<H, C>,
        Pair<H, D>,
        Pair<H, F>, Pair<H, G>,
        Pair<I, A>, Pair<I, B>, Pair<I, C>,
        Pair<I, D>,
        Pair<I, F>, Pair<I, G>
  >;

//   // even larger example
//   using RowList2 = TypeList<A, B, H, I>;
//   using ColList2 = TypeList<A, B, C, D, E, F, G, J>;
//   using Result = CheckConstructorArgs<RowList2, ColList2, 
//         Pair<A, A>, Pair<A, B>, Pair<A, C>, Pair<A, D>, Pair<A, E>, Pair<A, F>, Pair<A, G>, Pair<A, J>,  //  
//         Pair<B, A>, Pair<B, B>, Pair<B, C>, Pair<B, D>, Pair<B, E>, Pair<B, F>, Pair<B, G>, Pair<B, J>,  //  
//         Pair<H, A>, Pair<H, B>, Pair<H, C>, Pair<H, D>, Pair<H, E>, Pair<H, F>, Pair<H, G>, Pair<H, J>,  //  
//         Pair<I, A>, Pair<I, B>, Pair<I, C>, Pair<I, D>, Pair<I, E>, Pair<I, F>, Pair<I, G>, Pair<I, J>
//   >;
}
```

## Comments

### Comment 1 - dwith-ts

FYI: clang (trunc) can compile the program when I try it on godbolt.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (dwith-ts)

<details>
Hi, 

I have a condensed template meta-program below which causes clang 20.1.0 to use a large amount of memory (verified that it uses more than 60GB) and then it crashes with out-of-memory on my machine.

The program compiles without problems with clang 18 and clang 19. I used C++17 option for all compilers.

Using the instructions from https://llvm.org/docs/HowToSubmitABug.html, this seems to be a frontend bug.

For a quick try, here is the godbolt link: https://godbolt.org/z/67vejdxMe

Code example:
```
#include &lt;tuple&gt;
#include &lt;cstdint&gt;

template&lt;typename...&gt; struct TypeList{};

template&lt;typename T1, typename T2&gt; struct Pair {
    using FirstType = T1;
    using SecondType = T2;
};

template &lt;typename T1, typename T2&gt;
T2 remove_from_start;

template &lt;typename T1, typename T2&gt;
using RemoveFromStart = decltype(remove_from_start&lt;T1, T2&gt;);

template &lt;typename T, typename... Rest1, typename... Rest2&gt;
RemoveFromStart&lt;TypeList&lt;Rest1...&gt;, TypeList&lt;Rest2...&gt;&gt;
    remove_from_start&lt;TypeList&lt;T, Rest1...&gt;,
                      TypeList&lt;T, Rest2...&gt;&gt;;

template &lt;typename T&gt;
constexpr std::int32_t size_of = -1;

template &lt;template &lt;class...&gt; class L, typename... Ts&gt;
constexpr std::int32_t size_of&lt;L&lt;Ts...&gt;&gt; = sizeof...(Ts);


template &lt;bool&gt;
struct CheckingArgNr;

template &lt;typename AllRows_T, typename AllCols_T, typename... Others_T&gt;
using CheckConstructorArgs = typename CheckingArgNr&lt;(
    sizeof...(Others_T) &gt; 0)&gt;::template Result&lt;1, AllRows_T, AllCols_T, TypeList&lt;&gt;, AllCols_T, Others_T...&gt;::FirstType;


template &lt;typename RowsT, typename AllRowsT&gt;
using IsStartOf = std::bool_constant&lt;size_of&lt;AllRowsT&gt; - size_of&lt;RemoveFromStart&lt;RowsT, AllRowsT&gt;&gt; == size_of&lt;RowsT&gt;&gt;;

template &lt;bool&gt;
struct RemoveFromStartOfOr;

template &lt;&gt;
struct RemoveFromStartOfOr&lt;true&gt; {
  template &lt;typename StartList_T, typename FullList_T, typename&gt;
  using type = RemoveFromStart&lt;StartList_T, FullList_T&gt;;
};
template &lt;&gt;
struct RemoveFromStartOfOr&lt;false&gt; {
  template &lt;typename, typename, typename Fallback_T&gt;
  using type = Fallback_T;
};

template &lt;&gt;
struct CheckingArgNr&lt;true&gt; {
  template &lt;typename Pair_T&gt;
  using RowList = typename Pair_T::FirstType;
  template &lt;typename Pair_T&gt;
  using ColList = typename Pair_T::SecondType;

  template &lt;std::int32_t counter_v,
            typename RemainingRows_T,
            typename AllCols_T,
            typename CurrentRows_T,
            typename RemainingCols_T,
            typename CurrentPair_T,
            typename... Others_T&gt;
  using NextArgumentResult = typename CheckingArgNr&lt;(sizeof...(Others_T) &gt; 0)&gt;::template Result&lt;
      counter_v + 1,
      typename RemoveFromStartOfOr&lt;  //
          std::is_same&lt;ColList&lt;CurrentPair_T&gt;,
                 RemainingCols_T&gt;::value          
          &gt;::template type&lt;RowList&lt;CurrentPair_T&gt;,
                           RemainingRows_T,       
                           RemainingRows_T&gt;,      
      AllCols_T,
      std::conditional_t&lt;std::is_same&lt;ColList&lt;CurrentPair_T&gt;, RemainingCols_T&gt;::value, TypeList&lt;&gt;, RowList&lt;CurrentPair_T&gt;&gt;,
      typename RemoveFromStartOfOr&lt;not std::is_same&lt;ColList&lt;CurrentPair_T&gt;, RemainingCols_T&gt;::value&gt;::
          template type&lt;ColList&lt;CurrentPair_T&gt;, RemainingCols_T, AllCols_T&gt;,
      Others_T...&gt;;

  template &lt;typename RemainingRows_T,
            typename AllCols_T,
            typename CurrentRows_T,
            typename RemainingCols_T,
            typename CurrentPair_T&gt;
  static constexpr bool current_argument_check =
      IsStartOf&lt;ColList&lt;CurrentPair_T&gt;, RemainingCols_T&gt;::value &amp;&amp;
      (((size_of&lt;CurrentRows_T&gt; == 0) &amp;&amp; IsStartOf&lt;RowList&lt;CurrentPair_T&gt;, RemainingRows_T&gt;::value &amp;&amp;
        std::is_same&lt;RemainingCols_T, AllCols_T&gt;::value) ||
       std::is_same&lt;RowList&lt;CurrentPair_T&gt;, CurrentRows_T&gt;::value);  

  template &lt;std::int32_t counter_v,
            typename RemainingRows_T,
            typename AllCols_T,
            typename CurrentRows_T,
            typename RemainingCols_T,
            typename CurrentPair_T,
            typename... Others_T&gt;
  using Result = std::conditional_t&lt;
      current_argument_check&lt;RemainingRows_T, AllCols_T, CurrentRows_T, RemainingCols_T, CurrentPair_T&gt;,
      NextArgumentResult&lt;counter_v,
                         RemainingRows_T,
                         AllCols_T,
                         CurrentRows_T,
                         RemainingCols_T,
                         CurrentPair_T,
                         Others_T...&gt;,
      Pair&lt;std::false_type, std::integral_constant&lt;std::int32_t, counter_v&gt;&gt;&gt;;           
};

template &lt;&gt;
struct CheckingArgNr&lt;false&gt; {
  template &lt;std::int32_t counter_v,
            typename RemainingRows_T,
            typename AllCols_T,
            typename CurrentRows_T,
            typename RemainingCols_T&gt;
  using Result = Pair&lt;
      std::bool_constant&lt;(size_of&lt;RemainingRows_T&gt; == 0)&gt;,
      std::integral_constant&lt;std::int32_t, (size_of&lt;RemainingRows_T&gt; == 0) ? -1 : counter_v&gt;&gt;;
};

struct A; struct B; struct C; struct D; struct E; struct F; struct G; 
struct H; struct I; struct J;

int main()  {
  using RowList1 = TypeList&lt;A, B, H, I&gt;;
  using ColList1 = TypeList&lt;A, B, C, D, F, G&gt;;

  // this already crashes / uses a huge amount of memory with clang 20
  using Result = CheckConstructorArgs&lt;RowList1, ColList1, 
        Pair&lt;A, A&gt;, Pair&lt;A, B&gt;, Pair&lt;A, C&gt;, Pair&lt;A, D&gt;,
        Pair&lt;A, F&gt;, Pair&lt;A, G&gt;,
        Pair&lt;B, A&gt;, Pair&lt;B, B&gt;, Pair&lt;B, C&gt;, Pair&lt;B, D&gt;,
        Pair&lt;B, F&gt;, Pair&lt;B, G&gt;,
        Pair&lt;H, A&gt;, Pair&lt;H, B&gt;, Pair&lt;H, C&gt;,
        Pair&lt;H, D&gt;,
        Pair&lt;H, F&gt;, Pair&lt;H, G&gt;,
        Pair&lt;I, A&gt;, Pair&lt;I, B&gt;, Pair&lt;I, C&gt;,
        Pair&lt;I, D&gt;,
        Pair&lt;I, F&gt;, Pair&lt;I, G&gt;
  &gt;;

//   // even larger example
//   using RowList2 = TypeList&lt;A, B, H, I&gt;;
//   using ColList2 = TypeList&lt;A, B, C, D, E, F, G, J&gt;;
//   using Result = CheckConstructorArgs&lt;RowList2, ColList2, 
//         Pair&lt;A, A&gt;, Pair&lt;A, B&gt;, Pair&lt;A, C&gt;, Pair&lt;A, D&gt;, Pair&lt;A, E&gt;, Pair&lt;A, F&gt;, Pair&lt;A, G&gt;, Pair&lt;A, J&gt;,  //  
//         Pair&lt;B, A&gt;, Pair&lt;B, B&gt;, Pair&lt;B, C&gt;, Pair&lt;B, D&gt;, Pair&lt;B, E&gt;, Pair&lt;B, F&gt;, Pair&lt;B, G&gt;, Pair&lt;B, J&gt;,  //  
//         Pair&lt;H, A&gt;, Pair&lt;H, B&gt;, Pair&lt;H, C&gt;, Pair&lt;H, D&gt;, Pair&lt;H, E&gt;, Pair&lt;H, F&gt;, Pair&lt;H, G&gt;, Pair&lt;H, J&gt;,  //  
//         Pair&lt;I, A&gt;, Pair&lt;I, B&gt;, Pair&lt;I, C&gt;, Pair&lt;I, D&gt;, Pair&lt;I, E&gt;, Pair&lt;I, F&gt;, Pair&lt;I, G&gt;, Pair&lt;I, J&gt;
//   &gt;;
}
```
</details>


---

### Comment 3 - shafik

It looks fixed in trunk, I don't have a good idea of what the problem was or what fixed it though.

CC @erichkeane 

---

### Comment 4 - keinflue

Since commit f302f35526 this is able to compile in ~200MB. Before that commit it required ~35GB.

I don't know what exactly in the commit causes the difference, but this issue seems resolved to me.

---

