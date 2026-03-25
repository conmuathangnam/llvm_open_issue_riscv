# Assertion `!isa<ElaboratedType>(T) && "Elaborated type in nested-name-specifier"'

**Author:** dianqk
**URL:** https://github.com/llvm/llvm-project/issues/92757
**Status:** Closed
**Labels:** c++, clang:frontend, confirmed, crash-on-valid
**Closed Date:** 2025-08-09T08:06:55Z

## Body

I tried the following code with clang (assertions):

<details><summary>Details</summary>
<p>

```c++
namespace std {
typedef long unsigned int size_t;
typedef decltype(nullptr) nullptr_t;

template <typename _Tp> class reference_wrapper;
template <typename _Tp, _Tp __v> struct integral_constant {
  static constexpr _Tp value = __v;
  using value_type = _Tp;
  using type = integral_constant<_Tp, __v>;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};
template <bool __v> using __bool_constant = integral_constant<bool, __v>;
using true_type = __bool_constant<true>;
using false_type = __bool_constant<false>;
template <bool, typename _Tp = void> struct enable_if {};
template <typename _Tp> struct enable_if<true, _Tp> {
  using type = _Tp;
};
template <typename _Tp, typename _Up = _Tp &&> _Up __declval(int);
template <typename _Tp> _Tp __declval(long);
template <typename _Tp> auto declval() noexcept -> decltype(__declval<_Tp>(0));
template <bool _Cond, typename _Tp = void>
using enable_if_t = typename enable_if<_Cond, _Tp>::type;
} // namespace std

template <class T> struct type {};
template <typename Iterator> class fragmented_memory_input_stream {};
template <typename Iterator> class memory_input_stream {
public:
  using fragmented_type = fragmented_memory_input_stream<Iterator>;
  fragmented_type _fragmented;

public:
  template <typename StreamVisitor>
  decltype(auto) with_stream(StreamVisitor &&visitor) const {
    return visitor(_fragmented);
  }
  using has_with_stream = std::true_type;
};
template <typename Stream, typename StreamVisitor,
          typename = std::enable_if_t<Stream::has_with_stream::value>>
decltype(auto) with_serialized_stream(Stream &stream, StreamVisitor &&visitor) {
  return stream.with_stream(visitor);
}
template <typename Stream, typename StreamVisitor,
          typename = std::enable_if_t<!Stream::has_with_stream::value>,
          typename = void>
decltype(auto) with_serialized_stream(Stream &stream, StreamVisitor &&visitor);
class fragment_iterator {};
template <typename T> struct serializer;
template <typename T, typename Input> T deserialize(Input &in, type<T> t);
using input_stream = memory_input_stream<fragment_iterator>;
struct mutation_partition_view {};
struct canonical_mutation_view {
  input_stream v;
  auto partition() const {
    return with_serialized_stream(
        v,
        [this](
            auto &v) -> decltype(deserialize(std::declval<input_stream &>(),
                                             type<mutation_partition_view>())) {
          auto in = v;
          return deserialize(in, type<mutation_partition_view>());
        });
  }
};
```

</p>
</details> 

I got the following assertion:

```
1.	schema_tables.cpp:65:10: current parser token ')'
2.	schema_tables.cpp:55:1: parsing struct/union/class body 'canonical_mutation_view'
3.	schema_tables.cpp:57:26: parsing function body 'canonical_mutation_view::partition'
4.	schema_tables.cpp:57:26: in compound statement ('{}')
...
#10 0x00007f5fa1721560 llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>>::updatePointer(long, void*) /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:202:5
#11 0x00007f5fa1721560 llvm::PointerIntPair<void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>, llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>>>::setPointerAndInt(void*, int) & /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:111:29
#12 0x00007f5fa1721560 llvm::PointerIntPair<void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>, llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>>>::PointerIntPair(void*, int) /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:89:5
#13 0x00007f5fa1721560 llvm::pointer_union_detail::PointerUnionMembers<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>, llvm::PointerIntPair<void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>, llvm::PointerIntPairInfo<void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits<clang::Type const*, clang::ExtQuals const*>>>, 0, clang::Type const*, clang::ExtQuals const*>::PointerUnionMembers(clang::Type const*) /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:77:16
```

This code is from https://github.com/llvm/llvm-project/issues/91312#issuecomment-2116404306.

The output from `creduce`:

```c++
namespace std {
      template <typename _Tp, _Tp __v> struct integral_constant {
       static constexpr _Tp value = __v;
     };
      template <bool __v> using __bool_constant = integral_constant<bool, __v>;
      using true_type = __bool_constant<true>;
      template <bool, typename _Tp = void> struct enable_if {
    };
      template <typename _Tp> _Tp __declval(long);
      template <typename _Tp> auto declval() noexcept -> decltype(__declval<_Tp>(0));
      template <bool _Cond, typename _Tp = void> using enable_if_t = typename enable_if<_Cond, _Tp>::type;
        namespace seastar {
      template <class T> struct type {
   };
      template <typename Iterator> class memory_input_stream {
    public:   using fragmented_type = fragmented_memory_input_stream<Iterator>;
      using has_with_stream = std::true_type;
    };
      template <typename Stream, typename StreamVisitor,           typename = std::enable_if_t<Stream::has_with_stream::value>> decltype(auto) with_serialized_stream(Stream &stream, StreamVisitor &&visitor) {
    }
        class fragment_iterator {
      namespace ser {
    template <typename T, typename Input> T deserialize(Input &in, seastar::type<T> t);
      using input_stream = seastar::memory_input_stream<fragment_iterator>;
     struct mutation_partition_view {
     input_stream v;
     auto partition() const {
      return seastar::with_serialized_stream(         v,         [this](auto &v) -> decltype(ser::deserialize(                             std::declval<input_stream &>(),                             seastar::type<mutation_partition_view>())) {
         }
 );
```

godbolt 1: https://llvm.godbolt.org/z/6sd8he1Me
godbolt 2: https://llvm.godbolt.org/z/jqfnKz47d

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-1

Author: DianQK (DianQK)

<details>
I tried the following code with clang (assertions):

&lt;details&gt;&lt;summary&gt;Details&lt;/summary&gt;
&lt;p&gt;

```c++
namespace std {
typedef long unsigned int size_t;
typedef decltype(nullptr) nullptr_t;

template &lt;typename _Tp&gt; class reference_wrapper;
template &lt;typename _Tp, _Tp __v&gt; struct integral_constant {
  static constexpr _Tp value = __v;
  using value_type = _Tp;
  using type = integral_constant&lt;_Tp, __v&gt;;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};
template &lt;bool __v&gt; using __bool_constant = integral_constant&lt;bool, __v&gt;;
using true_type = __bool_constant&lt;true&gt;;
using false_type = __bool_constant&lt;false&gt;;
template &lt;bool, typename _Tp = void&gt; struct enable_if {};
template &lt;typename _Tp&gt; struct enable_if&lt;true, _Tp&gt; {
  using type = _Tp;
};
template &lt;typename _Tp, typename _Up = _Tp &amp;&amp;&gt; _Up __declval(int);
template &lt;typename _Tp&gt; _Tp __declval(long);
template &lt;typename _Tp&gt; auto declval() noexcept -&gt; decltype(__declval&lt;_Tp&gt;(0));
template &lt;bool _Cond, typename _Tp = void&gt;
using enable_if_t = typename enable_if&lt;_Cond, _Tp&gt;::type;
} // namespace std

template &lt;class T&gt; struct type {};
template &lt;typename Iterator&gt; class fragmented_memory_input_stream {};
template &lt;typename Iterator&gt; class memory_input_stream {
public:
  using fragmented_type = fragmented_memory_input_stream&lt;Iterator&gt;;
  fragmented_type _fragmented;

public:
  template &lt;typename StreamVisitor&gt;
  decltype(auto) with_stream(StreamVisitor &amp;&amp;visitor) const {
    return visitor(_fragmented);
  }
  using has_with_stream = std::true_type;
};
template &lt;typename Stream, typename StreamVisitor,
          typename = std::enable_if_t&lt;Stream::has_with_stream::value&gt;&gt;
decltype(auto) with_serialized_stream(Stream &amp;stream, StreamVisitor &amp;&amp;visitor) {
  return stream.with_stream(visitor);
}
template &lt;typename Stream, typename StreamVisitor,
          typename = std::enable_if_t&lt;!Stream::has_with_stream::value&gt;,
          typename = void&gt;
decltype(auto) with_serialized_stream(Stream &amp;stream, StreamVisitor &amp;&amp;visitor);
class fragment_iterator {};
template &lt;typename T&gt; struct serializer;
template &lt;typename T, typename Input&gt; T deserialize(Input &amp;in, type&lt;T&gt; t);
using input_stream = memory_input_stream&lt;fragment_iterator&gt;;
struct mutation_partition_view {};
struct canonical_mutation_view {
  input_stream v;
  auto partition() const {
    return with_serialized_stream(
        v,
        [this](
            auto &amp;v) -&gt; decltype(deserialize(std::declval&lt;input_stream &amp;&gt;(),
                                             type&lt;mutation_partition_view&gt;())) {
          auto in = v;
          return deserialize(in, type&lt;mutation_partition_view&gt;());
        });
  }
};
```

&lt;/p&gt;
&lt;/details&gt; 

I got the following assertion:

```
1.	schema_tables.cpp:65:10: current parser token ')'
2.	schema_tables.cpp:55:1: parsing struct/union/class body 'canonical_mutation_view'
3.	schema_tables.cpp:57:26: parsing function body 'canonical_mutation_view::partition'
4.	schema_tables.cpp:57:26: in compound statement ('{}')
...
#<!-- -->10 0x00007f5fa1721560 llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;::updatePointer(long, void*) /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:202:5
#<!-- -->11 0x00007f5fa1721560 llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;&gt;::setPointerAndInt(void*, int) &amp; /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:111:29
#<!-- -->12 0x00007f5fa1721560 llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;&gt;::PointerIntPair(void*, int) /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:89:5
#<!-- -->13 0x00007f5fa1721560 llvm::pointer_union_detail::PointerUnionMembers&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;, llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;&gt;, 0, clang::Type const*, clang::ExtQuals const*&gt;::PointerUnionMembers(clang::Type const*) /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:77:16
```

This code is from https://github.com/llvm/llvm-project/issues/91312#issuecomment-2116404306.

The output from `creduce`:

```c++
namespace std {
      template &lt;typename _Tp, _Tp __v&gt; struct integral_constant {
       static constexpr _Tp value = __v;
     };
      template &lt;bool __v&gt; using __bool_constant = integral_constant&lt;bool, __v&gt;;
      using true_type = __bool_constant&lt;true&gt;;
      template &lt;bool, typename _Tp = void&gt; struct enable_if {
    };
      template &lt;typename _Tp&gt; _Tp __declval(long);
      template &lt;typename _Tp&gt; auto declval() noexcept -&gt; decltype(__declval&lt;_Tp&gt;(0));
      template &lt;bool _Cond, typename _Tp = void&gt; using enable_if_t = typename enable_if&lt;_Cond, _Tp&gt;::type;
        namespace seastar {
      template &lt;class T&gt; struct type {
   };
      template &lt;typename Iterator&gt; class memory_input_stream {
    public:   using fragmented_type = fragmented_memory_input_stream&lt;Iterator&gt;;
      using has_with_stream = std::true_type;
    };
      template &lt;typename Stream, typename StreamVisitor,           typename = std::enable_if_t&lt;Stream::has_with_stream::value&gt;&gt; decltype(auto) with_serialized_stream(Stream &amp;stream, StreamVisitor &amp;&amp;visitor) {
    }
        class fragment_iterator {
      namespace ser {
    template &lt;typename T, typename Input&gt; T deserialize(Input &amp;in, seastar::type&lt;T&gt; t);
      using input_stream = seastar::memory_input_stream&lt;fragment_iterator&gt;;
     struct mutation_partition_view {
     input_stream v;
     auto partition() const {
      return seastar::with_serialized_stream(         v,         [this](auto &amp;v) -&gt; decltype(ser::deserialize(                             std::declval&lt;input_stream &amp;&gt;(),                             seastar::type&lt;mutation_partition_view&gt;())) {
         }
 );
```

godbolt 1: https://llvm.godbolt.org/z/6sd8he1Me
godbolt 2: https://llvm.godbolt.org/z/jqfnKz47d
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: DianQK (DianQK)

<details>
I tried the following code with clang (assertions):

&lt;details&gt;&lt;summary&gt;Details&lt;/summary&gt;
&lt;p&gt;

```c++
namespace std {
typedef long unsigned int size_t;
typedef decltype(nullptr) nullptr_t;

template &lt;typename _Tp&gt; class reference_wrapper;
template &lt;typename _Tp, _Tp __v&gt; struct integral_constant {
  static constexpr _Tp value = __v;
  using value_type = _Tp;
  using type = integral_constant&lt;_Tp, __v&gt;;
  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }
};
template &lt;bool __v&gt; using __bool_constant = integral_constant&lt;bool, __v&gt;;
using true_type = __bool_constant&lt;true&gt;;
using false_type = __bool_constant&lt;false&gt;;
template &lt;bool, typename _Tp = void&gt; struct enable_if {};
template &lt;typename _Tp&gt; struct enable_if&lt;true, _Tp&gt; {
  using type = _Tp;
};
template &lt;typename _Tp, typename _Up = _Tp &amp;&amp;&gt; _Up __declval(int);
template &lt;typename _Tp&gt; _Tp __declval(long);
template &lt;typename _Tp&gt; auto declval() noexcept -&gt; decltype(__declval&lt;_Tp&gt;(0));
template &lt;bool _Cond, typename _Tp = void&gt;
using enable_if_t = typename enable_if&lt;_Cond, _Tp&gt;::type;
} // namespace std

template &lt;class T&gt; struct type {};
template &lt;typename Iterator&gt; class fragmented_memory_input_stream {};
template &lt;typename Iterator&gt; class memory_input_stream {
public:
  using fragmented_type = fragmented_memory_input_stream&lt;Iterator&gt;;
  fragmented_type _fragmented;

public:
  template &lt;typename StreamVisitor&gt;
  decltype(auto) with_stream(StreamVisitor &amp;&amp;visitor) const {
    return visitor(_fragmented);
  }
  using has_with_stream = std::true_type;
};
template &lt;typename Stream, typename StreamVisitor,
          typename = std::enable_if_t&lt;Stream::has_with_stream::value&gt;&gt;
decltype(auto) with_serialized_stream(Stream &amp;stream, StreamVisitor &amp;&amp;visitor) {
  return stream.with_stream(visitor);
}
template &lt;typename Stream, typename StreamVisitor,
          typename = std::enable_if_t&lt;!Stream::has_with_stream::value&gt;,
          typename = void&gt;
decltype(auto) with_serialized_stream(Stream &amp;stream, StreamVisitor &amp;&amp;visitor);
class fragment_iterator {};
template &lt;typename T&gt; struct serializer;
template &lt;typename T, typename Input&gt; T deserialize(Input &amp;in, type&lt;T&gt; t);
using input_stream = memory_input_stream&lt;fragment_iterator&gt;;
struct mutation_partition_view {};
struct canonical_mutation_view {
  input_stream v;
  auto partition() const {
    return with_serialized_stream(
        v,
        [this](
            auto &amp;v) -&gt; decltype(deserialize(std::declval&lt;input_stream &amp;&gt;(),
                                             type&lt;mutation_partition_view&gt;())) {
          auto in = v;
          return deserialize(in, type&lt;mutation_partition_view&gt;());
        });
  }
};
```

&lt;/p&gt;
&lt;/details&gt; 

I got the following assertion:

```
1.	schema_tables.cpp:65:10: current parser token ')'
2.	schema_tables.cpp:55:1: parsing struct/union/class body 'canonical_mutation_view'
3.	schema_tables.cpp:57:26: parsing function body 'canonical_mutation_view::partition'
4.	schema_tables.cpp:57:26: in compound statement ('{}')
...
#<!-- -->10 0x00007f5fa1721560 llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;::updatePointer(long, void*) /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:202:5
#<!-- -->11 0x00007f5fa1721560 llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;&gt;::setPointerAndInt(void*, int) &amp; /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:111:29
#<!-- -->12 0x00007f5fa1721560 llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;&gt;::PointerIntPair(void*, int) /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:89:5
#<!-- -->13 0x00007f5fa1721560 llvm::pointer_union_detail::PointerUnionMembers&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;, llvm::PointerIntPair&lt;void*, 1u, int, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;, llvm::PointerIntPairInfo&lt;void*, 1u, llvm::pointer_union_detail::PointerUnionUIntTraits&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;&gt;, 0, clang::Type const*, clang::ExtQuals const*&gt;::PointerUnionMembers(clang::Type const*) /home/dianqk/llvm/llvm-project/llvm/include/llvm/ADT/PointerUnion.h:77:16
```

This code is from https://github.com/llvm/llvm-project/issues/91312#issuecomment-2116404306.

The output from `creduce`:

```c++
namespace std {
      template &lt;typename _Tp, _Tp __v&gt; struct integral_constant {
       static constexpr _Tp value = __v;
     };
      template &lt;bool __v&gt; using __bool_constant = integral_constant&lt;bool, __v&gt;;
      using true_type = __bool_constant&lt;true&gt;;
      template &lt;bool, typename _Tp = void&gt; struct enable_if {
    };
      template &lt;typename _Tp&gt; _Tp __declval(long);
      template &lt;typename _Tp&gt; auto declval() noexcept -&gt; decltype(__declval&lt;_Tp&gt;(0));
      template &lt;bool _Cond, typename _Tp = void&gt; using enable_if_t = typename enable_if&lt;_Cond, _Tp&gt;::type;
        namespace seastar {
      template &lt;class T&gt; struct type {
   };
      template &lt;typename Iterator&gt; class memory_input_stream {
    public:   using fragmented_type = fragmented_memory_input_stream&lt;Iterator&gt;;
      using has_with_stream = std::true_type;
    };
      template &lt;typename Stream, typename StreamVisitor,           typename = std::enable_if_t&lt;Stream::has_with_stream::value&gt;&gt; decltype(auto) with_serialized_stream(Stream &amp;stream, StreamVisitor &amp;&amp;visitor) {
    }
        class fragment_iterator {
      namespace ser {
    template &lt;typename T, typename Input&gt; T deserialize(Input &amp;in, seastar::type&lt;T&gt; t);
      using input_stream = seastar::memory_input_stream&lt;fragment_iterator&gt;;
     struct mutation_partition_view {
     input_stream v;
     auto partition() const {
      return seastar::with_serialized_stream(         v,         [this](auto &amp;v) -&gt; decltype(ser::deserialize(                             std::declval&lt;input_stream &amp;&gt;(),                             seastar::type&lt;mutation_partition_view&gt;())) {
         }
 );
```

godbolt 1: https://llvm.godbolt.org/z/6sd8he1Me
godbolt 2: https://llvm.godbolt.org/z/jqfnKz47d
</details>


---

### Comment 3 - dianqk

It looks similar to #43179 and #68670.

---

### Comment 4 - shafik

Confirmed

---

### Comment 5 - mizvekov

While the crash had stopped happening because the particular assertion this was tripping has been removed in https://github.com/llvm/llvm-project/pull/133610, https://github.com/llvm/llvm-project/pull/147835 will fix this for the mere fact that ElaboratedType nodes will cease to exist.

---

