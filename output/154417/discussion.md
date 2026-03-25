# clang crash: UNREACHABLE executed at SemaDeclCXX.cpp:238

**Author:** DKLoehr
**URL:** https://github.com/llvm/llvm-project/issues/154417

## Body

Found in the wild while messing around. Unfortunately, the original source code didn't compile, so I'm not able to provide a nice tidy repro. The attached file was minimized by cvise, but is still nontrivial. I've tried making it smaller, with minimal success.

Clang build mode was `RelWithDebInfo`, with assertions enabled.

Expected result: compile failure without crashing.

Repro files don't want to upload for some reason, so the minimized example is below. I ran with `clang++ -Wno-everything -std=c++20`.

```
template < int > struct integral_constant;

template < class _Tp >
struct is_move_constructible : integral_constant< __is_constructible(_Tp,  _Tp ) >;

template < template < class >
class _BaseType, class _Tp, _Tp _SequenceSize >
using __make_integer_sequence_impl = __make_integer_seq< _BaseType, _Tp, _SequenceSize >;
    
template < class , _Tp... >
struct __integer_sequence;

template < int _SequenceSize >
using __make_index_sequence =
  __make_integer_sequence_impl< __integer_sequence, int, _SequenceSize >;

template < int >
struct __all;

template < class >
struct __make_tuple_types { using type = int >}

template < class, class... _Tp >
struct __tuple_impl :  _Tp ... {
  template < class _Tuple >
  __tuple_impl(_Tuple &&) noexcept(__all< is_nothrow_constructible<
            typename __make_tuple_types< _Tuple >::type >::
                    type >::value__tuple_leaf0);
};

template < class... _Tp >
struct tuple {
  __tuple_impl< __make_index_sequence< 0 >, _Tp... > __base_;
}

template < class _Tp >
using __optional_sfinae_ctor_base_t = is_move_constructible< _Tp >;

template < class _Tp >
struct optional : __optional_sfinae_assign_base_t< _Tp > template <typename  >

struct TypedStatus { 
  template < typename O >
  struct Or {
    optional< tuple< O > > value_;
  };
};

struct ResolvedSourceString;

struct SubstitutingParser { using ParseInto = ResolvedSourceString; };

template < typename T >
struct Quoted { TypedStatus<int>::Or< T::ParseInto > Parse; }

struct ResolvedSourceString  {
  ResolvedSourceString(ResolvedSourceString &) ;
  struct RawStr : SubstitutingParser Parse() { auto Quoted< RawStr >:: }
}
```

## Comments

### Comment 1 - EugeneZelenko

See https://godbolt.org/z/b8hfo6d1j

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Devon Loehr (DKLoehr)

<details>
Found in the wild while messing around. Unfortunately, the original source code didn't compile, so I'm not able to provide a nice tidy repro. The attached file was minimized by cvise, but is still nontrivial. I've tried making it smaller, with minimal success.

Clang build mode was `RelWithDebInfo`, with assertions enabled.

Expected result: compile failure without crashing.

Repro files don't want to upload for some reason, so the minimized example is below. I ran with `clang++ -Wno-everything -std=c++20`.

```
template &lt; int &gt; struct integral_constant;

template &lt; class _Tp &gt;
struct is_move_constructible : integral_constant&lt; __is_constructible(_Tp,  _Tp ) &gt;;

template &lt; template &lt; class &gt;
class _BaseType, class _Tp, _Tp _SequenceSize &gt;
using __make_integer_sequence_impl = __make_integer_seq&lt; _BaseType, _Tp, _SequenceSize &gt;;
    
template &lt; class , _Tp... &gt;
struct __integer_sequence;

template &lt; int _SequenceSize &gt;
using __make_index_sequence =
  __make_integer_sequence_impl&lt; __integer_sequence, int, _SequenceSize &gt;;

template &lt; int &gt;
struct __all;

template &lt; class &gt;
struct __make_tuple_types { using type = int &gt;}

template &lt; class, class... _Tp &gt;
struct __tuple_impl :  _Tp ... {
  template &lt; class _Tuple &gt;
  __tuple_impl(_Tuple &amp;&amp;) noexcept(__all&lt; is_nothrow_constructible&lt;
            typename __make_tuple_types&lt; _Tuple &gt;::type &gt;::
                    type &gt;::value__tuple_leaf0);
};

template &lt; class... _Tp &gt;
struct tuple {
  __tuple_impl&lt; __make_index_sequence&lt; 0 &gt;, _Tp... &gt; __base_;
}

template &lt; class _Tp &gt;
using __optional_sfinae_ctor_base_t = is_move_constructible&lt; _Tp &gt;;

template &lt; class _Tp &gt;
struct optional : __optional_sfinae_assign_base_t&lt; _Tp &gt; template &lt;typename  &gt;

struct TypedStatus { 
  template &lt; typename O &gt;
  struct Or {
    optional&lt; tuple&lt; O &gt; &gt; value_;
  };
};

struct ResolvedSourceString;

struct SubstitutingParser { using ParseInto = ResolvedSourceString; };

template &lt; typename T &gt;
struct Quoted { TypedStatus&lt;int&gt;::Or&lt; T::ParseInto &gt; Parse; }

struct ResolvedSourceString  {
  ResolvedSourceString(ResolvedSourceString &amp;) ;
  struct RawStr : SubstitutingParser Parse() { auto Quoted&lt; RawStr &gt;:: }
}
```
</details>


---

### Comment 3 - efriedma-quic

Reduced a bit:

```c++
template < int > struct I;
template < class _Tp >
struct J :  _Tp  {
  template < class T >
  J(T &&) noexcept( I<
            __is_constructible(_Tp,  _Tp )  >::i );
};
template < class T >
struct K {
  J<T> __base_;
};
struct S  {
  S(S &) ;
};
I<__is_constructible(K< S >,  K< S > )> *x;
```

---

### Comment 4 - shafik

@sdkrystian this bisects to fd87d765c0455265aea4595a3741a96b4c078fbc

CC @erichkeane 

---

