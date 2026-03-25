# [clang] ICE and corrupted values of non-type template parameters that include padding

**Author:** tttapa
**URL:** https://github.com/llvm/llvm-project/issues/146272

## Body

Accessing members of a non-type template parameter of struct type sometimes results in incorrect values or internal compiler errors if the struct has padding.  
In particular, reading members after the padding returns the bits of other members. Accessing the last member seems to raise an out-of-bounds error in the compiler.

Reproducible on Clang 18, 19, 20 and trunk (3cc78a8067788e9aad2bee8ffa95c03ea2f17b04), both on Compiler Explorer and locally (using the releases from https://apt.llvm.org).

https://godbolt.org/z/Y8rf46Gvv

```cpp
// clang++ -std=c++20
#include <fmt/format.h>

struct Config {
    unsigned char a = 0xAA;
    unsigned int  b = 0xBBBBBBBB;
    unsigned char c = 0xCC;
    unsigned char d = 0xDD;
    unsigned char e = 0xEE;
};

template <Config Conf>
void foo() {
    fmt::print("Conf.a = {:02X}  ", Conf.a);
    fmt::print("Conf.b = {:08X}  ", Conf.b);
    fmt::print("Conf.c = {:02X}  ", Conf.c);
    fmt::print("Conf.d = {:02X}  ", Conf.d);
    #ifndef __clang__ // fatal error: error in backend: Invalid size request on a scalable vector.
    fmt::print("Conf.e = {:02X}  ", Conf.e);
    #endif
    fmt::println("");
    auto &conf = Conf;
    fmt::print("conf.a = {:02X}  ", conf.a);
    fmt::print("conf.b = {:08X}  ", conf.b);
    fmt::print("conf.c = {:02X}  ", conf.c);
    fmt::print("conf.d = {:02X}  ", conf.d);
    fmt::print("conf.e = {:02X}  ", conf.e);
    fmt::println("");
}

int main() { foo<{}>(); }
```
```
Conf.a = AA  Conf.b = 00EEDDCC  Conf.c = DD  Conf.d = EE  
conf.a = AA  conf.b = BBBBBBBB  conf.c = CC  conf.d = DD  conf.e = EE  
```
Note how `Conf.b` contains the bits of `Conf.{c,d,e}`, and the values of `Conf.{c,d}` are also offset by one byte. Trying to print `Conf.e` crashes the compiler.

The behavior seems to be sensitive to minor changes to the code. For example, storing `Conf` in a constexpr variable before printing does not trigger the bug. In the code base where I originally encountered the issue, passing `Conf.b` as a template argument to another function `template <int I> void bar();` would actually call `bar<0x00EEDDCC>()`, but I'm having trouble reproducing this in a standalone program.

## Comments

### Comment 1 - frederick-vs-ja

Reduced example ([Godbolt link](https://godbolt.org/z/6qWxz6bMd)):
```C++
struct Config {
    unsigned char a = 0xAA;
    unsigned int  b = 0xBBBBBBBB;
    unsigned char c = 0xCC;
    unsigned char d = 0xDD;
    unsigned char e = 0xEE;
};

template <Config Conf>
void foo() {
    [](auto&&){}(Conf.e);
}

int main() { foo<{}>(); }
```

[P3106R1](https://wg21.link/p3106r1) seems related.

A modified example (using `foo<Config{}>()`) seems to start crashing since Clang 12 ([demo](https://godbolt.org/z/33xcYK66E)), so this is perhaps not regression.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Pieter P (tttapa)

<details>
Accessing members of a non-type template parameter of struct type sometimes results in incorrect values or internal compiler errors if the struct has padding.  
In particular, reading members after the padding returns the bits of other members. Accessing the last member seems to raise an out-of-bounds error in the compiler.

Reproducible on Clang 18, 19, 20 and trunk (3cc78a8067788e9aad2bee8ffa95c03ea2f17b04), both on Compiler Explorer and locally (using the releases from https://apt.llvm.org).

https://godbolt.org/z/Y8rf46Gvv

```cpp
// clang++ -std=c++20
#include &lt;fmt/format.h&gt;

struct Config {
    unsigned char a = 0xAA;
    unsigned int  b = 0xBBBBBBBB;
    unsigned char c = 0xCC;
    unsigned char d = 0xDD;
    unsigned char e = 0xEE;
};

template &lt;Config Conf&gt;
void foo() {
    fmt::print("Conf.a = {:02X}  ", Conf.a);
    fmt::print("Conf.b = {:08X}  ", Conf.b);
    fmt::print("Conf.c = {:02X}  ", Conf.c);
    fmt::print("Conf.d = {:02X}  ", Conf.d);
    #ifndef __clang__ // fatal error: error in backend: Invalid size request on a scalable vector.
    fmt::print("Conf.e = {:02X}  ", Conf.e);
    #endif
    fmt::println("");
    auto &amp;conf = Conf;
    fmt::print("conf.a = {:02X}  ", conf.a);
    fmt::print("conf.b = {:08X}  ", conf.b);
    fmt::print("conf.c = {:02X}  ", conf.c);
    fmt::print("conf.d = {:02X}  ", conf.d);
    fmt::print("conf.e = {:02X}  ", conf.e);
    fmt::println("");
}

int main() { foo&lt;{}&gt;(); }
```
```
Conf.a = AA  Conf.b = 00EEDDCC  Conf.c = DD  Conf.d = EE  
conf.a = AA  conf.b = BBBBBBBB  conf.c = CC  conf.d = DD  conf.e = EE  
```
Note how `Conf.b` contains the bits of `Conf.{c,d,e}`, and the values of `Conf.{c,d}` are also offset by one byte. Trying to print `Conf.e` crashes the compiler.

The behavior seems to be sensitive to minor changes to the code. For example, storing `Conf` in a constexpr variable before printing does not trigger the bug. In the code base where I originally encountered the issue, passing `Conf.b` as a template argument to another function `template &lt;int I&gt; void bar();` would actually call `bar&lt;0x00EEDDCC&gt;()`, but I'm having trouble reproducing this in a standalone program.
</details>


---

