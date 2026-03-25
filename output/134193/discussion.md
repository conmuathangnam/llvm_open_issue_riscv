# Clang behaves different with other compilers for constraint nested lambdas

**Author:** zyn0217
**URL:** https://github.com/llvm/llvm-project/issues/134193

## Body

Both were discovered in #133719, but the underlying causes are different from the original issue so I filed a separate one to track these.

1. The following code causes clang to crash, dating back to Clang 16 and up to trunk:

```cpp
template <class T>
constexpr auto f{[] (auto arg) {
    return [a{arg}] {
        [] () requires (a == 1) {}();
    };
}};

int main() {
    f<int>(0);
}
```

https://compiler-explorer.com/z/Tq9xzrPns

2. Clang accepts the following code, while others don't (discovered by @shafik)

```cpp

void f() {
    auto f{ 
      [](auto arg) {
        return [a = arg]() 
          requires (a == 1)
         { return a;};
      }
    };

    f(10);
}

```

https://godbolt.org/z/ncYx8q9f6

This has started since Clang 16.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Younan Zhang (zyn0217)

<details>
Both were discovered in #<!-- -->133719, but the underlying causes are different from the original issue so I filed a separate one to track these.

1. The following code causes clang to crash, dating back to Clang 16 and up to trunk:

```cpp
template &lt;class T&gt;
constexpr auto f{[] (auto arg) {
    return [a{arg}] {
        [] () requires (a == 1) {}();
    };
}};

int main() {
    f&lt;int&gt;(0);
}
```

https://compiler-explorer.com/z/Tq9xzrPns

2. Clang accepts the following code, while others don't (discovered by @<!-- -->shafik)

```cpp

void f() {
    auto f{ 
      [](auto arg) {
        return [a = arg]() 
          requires (a == 1)
         { return a;};
      }
    };

    f(10);
}

```

https://godbolt.org/z/ncYx8q9f6

This has started since Clang 16.
</details>


---

### Comment 2 - frederick-vs-ja

> 2\. Clang accepts the following code, while others don't (discovered by @shafik)
> 
> ```cpp
> 
> void f() {
>     auto f{ 
>       [](auto arg) {
>         return [a = arg]() 
>           requires (a == 1)
>          { return a;};
>       }
>     };
> 
>     f(10);
> }
> 
> ```
> 
> https://godbolt.org/z/ncYx8q9f6

~The 2nd case is not a bug.~ It is ~an expected~ a possibly result of DR [P2036R3](https://wg21.link/p2036r3) + [P2579R0](https://wg21.link/P2579R0), which is currently only implemented by Clang.




---

### Comment 3 - frederick-vs-ja

Looks like that I was confused.

It seems that we should at least reject this
```C++
void f() {
  auto l2 = []<class = void>() {
    return [a = 10]() requires (a == 1) { return a; };
  }();
}
```

---

### Comment 4 - frederick-vs-ja

This one is perhaps valid, but seemingly causes infinite recursion in Clang ([Godbolt link](https://godbolt.org/z/bs5sj1nj8)).

```C++
struct ConvToOne {
  constexpr operator int() const { return 1; }
};

void g() {
  auto l2 = []<class = void>() {
    return [a = ConvToOne{}]() requires (a == 1) { return a; };
  }();
  l2();
}
```

---

### Comment 5 - shafik

CC @cor3ntin 

---

