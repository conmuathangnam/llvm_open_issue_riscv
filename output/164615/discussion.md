# Support for DR2936: Local classes of templated functions should be part of the current instantiation

**Author:** ghost
**URL:** https://github.com/llvm/llvm-project/issues/164615

## Body

Test code ([CE](https://godbolt.org/z/7r76s9h55)):

```cpp
template <int> void f() {
  // - EDG: OK
  // - GCC/Clang/MSVC: error
  []<int> {}.operator()<0>();
}
```

According to [[temp.dep.type]/10](https://eel.is/c++draft/temp.dep.type#10), the closure type above is not dependent. Even if it happens to be dependent, [[temp.dep.type]/1.5](https://eel.is/c++draft/temp.dep.type#1.5) (added by [CWG2936](https://cplusplus.github.io/CWG/issues/2936.html)) specifies that a local class in a templated function is part of the current instantiation (though I'm not sure whether this applies to closure types):

> - in the definition of a templated function, the name of a local class ([class.local]).

So, I don't think the `template` disambiguator should be required here, as it only adds syntactic noise.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Vincent X (ckwastra)

<details>
Test code ([CE](https://godbolt.org/z/7r76s9h55)):

```cpp
template &lt;int&gt; void f() {
  // - EDG: OK
  // - GCC/Clang/MSVC: error
  []&lt;int&gt; {}.operator()&lt;0&gt;();
}
```

According to [[temp.dep.type]/10](https://eel.is/c++draft/temp.dep.type#<!-- -->10), the closure type above is not dependent. Even if it happens to be dependent, [[temp.dep.type]/1.5](https://eel.is/c++draft/temp.dep.type#<!-- -->1.5) (added by [CWG2936](https://cplusplus.github.io/CWG/issues/2936.html)) specifies that a local class in a templated function is part of the current instantiation (though I'm not sure whether this applies to closure types):

&gt; - in the definition of a templated function, the name of a local class ([class.local]).

So, I don't think the `template` disambiguator should be required here, as it only adds syntactic noise.
</details>


---

### Comment 2 - shafik

Re titling this to make it more generic and adding additional cases that should be support: https://godbolt.org/z/1EWT9vE7Y

```cpp
template<class T>
  void f()
  {
    struct Y {
      struct A{};
      using type = int;
    };

    Y::A a;
    Y::A{};
    Y::A();
    
    int x;

    x = Y::A{};
    x = Y::A();

    Y::type y;  
    Y::type{};
    Y::type();

    x = Y::type{};
    x = Y::type();
  }
```

---

### Comment 3 - shafik

CC @Endilll 

---

