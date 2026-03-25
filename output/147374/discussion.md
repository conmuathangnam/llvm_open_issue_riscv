# [clang] Wrong rewritten `operator==` selected

**Author:** baxter-huntington
**URL:** https://github.com/llvm/llvm-project/issues/147374
**Status:** Closed
**Labels:** clang:frontend, confirmed, regression:20
**Closed Date:** 2025-07-18T15:05:20Z

## Body

The following minimal examples are used as a reference: 

**Example A** (https://godbolt.org/z/5Mvbj7ePd):
```cpp
#include <string>
#include <iostream>

class Bar {
public:
    explicit Bar(const std::string aValue) : mValue{aValue} {};

    bool operator==(const std::string aValue) const { 
        std::cout << "bool operator==(const std::string aValue) const\n";
        return aValue == mValue; 
    };

    friend bool operator==(const std::string aLhs, Bar aRhs) { return aRhs == aLhs; }
    
private:
    std::string mValue;
};

int main() {
    auto other1 = Bar("Bar");
    [[ maybe_unused ]] auto equal = other1 == "Bar";
}
```

**Example B** (https://godbolt.org/z/aM5f8odqa):

```cpp
#include <string>
#include <iostream>

template<typename T>
class Foo {
public:
    explicit Foo(const T aValue) : mValue{aValue} {};

    bool operator==(const T aValue) const { 
        std::cout << "bool operator==(const T aValue) const\n";
        return aValue == mValue; 
    };

    friend bool operator==(const T aLhs, Foo aRhs) { return aRhs == aLhs; }
    
private:
    T mValue;
};

int main() {
    auto thing1 = Foo<std::string>("Foo");
    [[maybe_unused]] auto equal = thing1 == "Foo";
}
```

In Clang 21 (and the most recent trunk at the time of writing), **Example A** results in the following warning being emitted:
```
<source>:13:62: warning: all paths through this function will call itself [-Winfinite-recursion]
   13 |     friend bool operator==(const std::string aLhs, Bar aRhs) { return aRhs == aLhs; }
      |      
```

In older versions (e.g., Clang 20.1.0), no warning is emitted. `const`-qualifying `aRhs` resolves the warning in Clang 21. Is this a false positive? I'm not sure I understand what Clang deems to be infinitely recursive here.

Notably, when using a class template as in **Example B**, no warning is emitted. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Baxter Huntington (baxter-huntington)

<details>
The following minimal examples are used as a reference: 

**Example A** (https://godbolt.org/z/5Mvbj7ePd):
```cpp
#include &lt;string&gt;
#include &lt;iostream&gt;

class Bar {
public:
    explicit Bar(const std::string aValue) : mValue{aValue} {};

    bool operator==(const std::string aValue) const { 
        std::cout &lt;&lt; "bool operator==(const std::string aValue) const\n";
        return aValue == mValue; 
    };

    friend bool operator==(const std::string aLhs, Bar aRhs) { return aRhs == aLhs; }
    
private:
    std::string mValue;
};

int main() {
    auto other1 = Bar("Bar");
    [[ maybe_unused ]] auto equal = other1 == "Bar";
}
```

**Example B** (https://godbolt.org/z/aM5f8odqa):

```cpp
#include &lt;string&gt;
#include &lt;iostream&gt;

template&lt;typename T&gt;
class Foo {
public:
    explicit Foo(const T aValue) : mValue{aValue} {};

    bool operator==(const T aValue) const { 
        std::cout &lt;&lt; "bool operator==(const T aValue) const\n";
        return aValue == mValue; 
    };

    friend bool operator==(const T aLhs, Foo aRhs) { return aRhs == aLhs; }
    
private:
    T mValue;
};

int main() {
    auto thing1 = Foo&lt;std::string&gt;("Foo");
    [[maybe_unused]] auto equal = thing1 == "Foo";
}
```

In Clang 21 (and the most recent trunk at the time of writing), **Example A** results in the following warning being emitted:
```
&lt;source&gt;:13:62: warning: all paths through this function will call itself [-Winfinite-recursion]
   13 |     friend bool operator==(const std::string aLhs, Bar aRhs) { return aRhs == aLhs; }
      |      
```

In older versions (e.g., Clang 20.1.0), no warning is emitted. `const`-qualifying `aRhs` resolves the warning in Clang 21. Is this a false positive? I'm not sure I understand what Clang deems to be infinitely recursive here.

Notably, when using a class template as in **Example B**, no warning is emitted. 
</details>


---

### Comment 2 - cor3ntin

The underlying issue is that given

``` 
bool operator==(const std::string aValue) const ;   // # 1
friend bool operator==(const std::string aLhs, Bar aRhs) { // #2
    return aRhs == aLhs; // #3
}
```

We pick a rewritten #2 in `#3` even if it is not a better candidate (afaict, there is a worse conversion sequence for `Bar`)


---

### Comment 3 - zygoloid

This doesn't seem to be related to rewrites at all. We [seem to think](https://godbolt.org/z/G1b57PW78) the non-member candidate *is* a better candidate:

```c++
#include <string>

using String = std::string;

struct Bar {
  void operator+(String aValue) const;
  friend void operator+(Bar aRhs, String aLhs);
};

int main() {
  Bar a;
  String b = "";
  a + b;
}
```

It looks like we conclude that the friend is better because the member would require a qualification conversion on `*this` -- if the `const` is removed from the member overload, then we report ambiguity. The really weird thing is that this only seems to happen when `String` is `std::string` -- I've tried other class types there and Clang does report the ambiguity in trunk.

---

### Comment 4 - zygoloid

Aha, [reduced testcase](https://godbolt.org/z/E3PqoWbG1):
```c++
struct String {};
template<typename T> void operator+(T, String&&) = delete;

struct Bar {
  void operator+(String) const;
  friend void operator+(Bar, String);
};

int main() {
  Bar a;
  String b;
  a + b;
}
```
Clang 20 rejects, Clang trunk accepts. Somehow the presence of the templated `operator+` causes the friend `operator+` to be preferred over the const-qualified member `operator+` (even if the templated one isn't even viable).

---

### Comment 5 - shafik

@cor3ntin git bisect points to 8c5a307bd8d406e6167a5cd3ce3c74e2e3bfb2a6 ... does that make sense to you

---

### Comment 6 - cor3ntin

Thanks, I'll try to look this week!

---

### Comment 7 - cor3ntin

@zygoloid We are consistent with GCC, although GCC does warn on it https://godbolt.org/z/crxc3x7dn 
But removing the template candidate certainly should not impact the outcome

---

