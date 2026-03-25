# Invalid error when matching constrained out-of-line definition

**Author:** ilya-biryukov
**URL:** https://github.com/llvm/llvm-project/issues/133413
**Status:** Closed
**Labels:** duplicate, clang:frontend, concepts, regression:18
**Closed Date:** 2025-03-28T12:34:41Z

## Body

This code compiles with GCC and fails with Clang, see https://godbolt.org/z/rz877Tde9
```cpp
template <class FooT, class ArgT>
concept CanDo = requires (ArgT arg){
  FooT::Do(arg);
};

struct Arg {

};

template <class T>
class Outer {
 public:
  template <class FooT>
    requires CanDo<FooT, Arg>
  class Inner;
};

template <class T>
template <class FooT>
    requires CanDo<FooT, Arg>
class Outer<T>::Inner {

};
```



## Comments

### Comment 1 - frederick-vs-ja

I think this is duplicate of #102320.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ilya Biryukov (ilya-biryukov)

<details>
This code compiles with GCC and fails with Clang, see https://godbolt.org/z/rz877Tde9
```cpp
template &lt;class FooT, class ArgT&gt;
concept CanDo = requires (ArgT arg){
  FooT::Do(arg);
};

struct Arg {

};

template &lt;class T&gt;
class Outer {
 public:
  template &lt;class FooT&gt;
    requires CanDo&lt;FooT, Arg&gt;
  class Inner;
};

template &lt;class T&gt;
template &lt;class FooT&gt;
    requires CanDo&lt;FooT, Arg&gt;
class Outer&lt;T&gt;::Inner {

};
```


</details>


---

### Comment 3 - zyn0217

Started to fail since 18


https://godbolt.org/z/zW5Wx3j3G

---

