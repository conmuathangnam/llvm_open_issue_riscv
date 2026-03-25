# Default non-type template parameters with immediately invoked lambdas fail with deduction guides

**Author:** friedkeenan
**URL:** https://github.com/llvm/llvm-project/issues/51315

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [51973](https://llvm.org/bz51973) |
| Version | trunk |
| OS | Linux |
| CC | @DougGregor,@friedkeenan,@zygoloid |

## Extended Description 
With the following code, clang claims the defaulted non-type template parameter is not a constant expression, despite it being so (with GCC handling it fine as well):
```cpp
#include <concepts>

template<
    typename T,
    auto V = []() {
        if constexpr (std::same_as<T, int>) {
            return 1;
        } else {
            return 2;
        }
    }()
>
struct Test {
    constexpr Test(T) { }
};

template<typename T>
Test(T) -> Test<T>;

void test() {
    Test<int, 1>(1);
}
```
Godbolt link: https://godbolt.org/z/jT3zPx9fx

As you can see, even if you specify the template parameters when constructing, it still complains, and if you duplicate the immediately invoked lambda in the deduction guide and remove the explicated template arguments for the constructor, clang actually says that it can't compile that expression yet. However, if you specify the template arguments, it consumes everything fine.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (friedkeenan)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [51973](https://llvm.org/bz51973) |
| Version | trunk |
| OS | Linux |
| CC | @<!-- -->DougGregor,@<!-- -->friedkeenan,@<!-- -->zygoloid |

## Extended Description 
With the following code, clang claims the defaulted non-type template parameter is not a constant expression, despite it being so (with GCC handling it fine as well):
```cpp
#include &lt;concepts&gt;

template&lt;
    typename T,
    auto V = []() {
        if constexpr (std::same_as&lt;T, int&gt;) {
            return 1;
        } else {
            return 2;
        }
    }()
&gt;
struct Test {
    constexpr Test(T) { }
};

template&lt;typename T&gt;
Test(T) -&gt; Test&lt;T&gt;;

void test() {
    Test&lt;int, 1&gt;(1);
}
```
Godbolt link: https://godbolt.org/z/jT3zPx9fx

As you can see, even if you specify the template parameters when constructing, it still complains, and if you duplicate the immediately invoked lambda in the deduction guide and remove the explicated template arguments for the constructor, clang actually says that it can't compile that expression yet. However, if you specify the template arguments, it consumes everything fine.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (friedkeenan)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [51973](https://llvm.org/bz51973) |
| Version | trunk |
| OS | Linux |
| CC | @<!-- -->DougGregor,@<!-- -->friedkeenan,@<!-- -->zygoloid |

## Extended Description 
With the following code, clang claims the defaulted non-type template parameter is not a constant expression, despite it being so (with GCC handling it fine as well):
```cpp
#include &lt;concepts&gt;

template&lt;
    typename T,
    auto V = []() {
        if constexpr (std::same_as&lt;T, int&gt;) {
            return 1;
        } else {
            return 2;
        }
    }()
&gt;
struct Test {
    constexpr Test(T) { }
};

template&lt;typename T&gt;
Test(T) -&gt; Test&lt;T&gt;;

void test() {
    Test&lt;int, 1&gt;(1);
}
```
Godbolt link: https://godbolt.org/z/jT3zPx9fx

As you can see, even if you specify the template parameters when constructing, it still complains, and if you duplicate the immediately invoked lambda in the deduction guide and remove the explicated template arguments for the constructor, clang actually says that it can't compile that expression yet. However, if you specify the template arguments, it consumes everything fine.
</details>


---

### Comment 3 - shafik

Still does not work: https://godbolt.org/z/85Mj487hv

CC @cor3ntin 

---

### Comment 4 - zyn0217

The dependency of lambda doesn't seem right

https://godbolt.org/z/6KdT4Tj7Y

we crash w/ an assertion enabled

---

