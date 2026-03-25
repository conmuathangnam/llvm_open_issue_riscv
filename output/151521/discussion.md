# Clang front-end crash on compiling lambda

**Author:** olivermadge
**URL:** https://github.com/llvm/llvm-project/issues/151521
**Status:** Closed
**Labels:** clang:frontend, obsolete, crash, lambda
**Closed Date:** 2025-07-31T14:37:14Z

## Body

The following code causes clang-19 to crash with the error `clang++-19: error: clang frontend command failed with exit code 139 (use -v to see invocation)`:

```
#include <utility>
#include <cstring>
#include <print>

template <typename TypeTuple>
auto count_types() {
    return []<size_t... Idx>(std::index_sequence<Idx...>) {
        return ([](std::tuple_element_t<Idx, TypeTuple> base) {return base;}(1) + ...);
    }(std::make_index_sequence<std::tuple_size_v<TypeTuple>>{});
}

int main() {
    std::println("{}", count_types<std::tuple<int>>());
    std::println("{}", count_types<std::tuple<int, char>>());
}

```

Interestingly, if I introduce an additional intermediate lambda to extract Idx, it compiles without error:
```
#include <utility>
#include <cstring>
#include <print>

template <typename TypeTuple>
auto count_types() {
    return []<size_t... Idx>(std::index_sequence<Idx...>) {
        return ([]<size_t Index>(std::integral_constant<size_t, Index>) {
            return [](std::tuple_element_t<Index, TypeTuple> base) {return base;}(1);
        }(std::integral_constant<size_t, Idx>()) + ...);
    }(std::make_index_sequence<std::tuple_size_v<TypeTuple>>{});
}

int main() {
    std::println("{}", count_types<std::tuple<int>>());
    std::println("{}", count_types<std::tuple<int, char>>());
}

```

Clang-19 from Arch Linux (version 19.1.7 on x86-64) invoked as `clang++-19 -emit-llvm -Xclang -disable-llvm-passes --std=c++20 clang_bug.cpp -c -o clang`

The same code compiles and runs with the expected result on clang++-20, g++-14 and g++-15

[clang_bug-86f8f9.cpp.log](https://github.com/user-attachments/files/21531707/clang_bug-86f8f9.cpp.log)
[clang_bug-86f8f9.sh.log](https://github.com/user-attachments/files/21531708/clang_bug-86f8f9.sh.log)

## Comments

### Comment 1 - AaronBallman

The issue is already fixed in Clang 20.x: https://godbolt.org/z/P7PTeb48s Closing because we're no longer backporting changes to Clang 19.x (we're already in the process of working on the 21.x release).

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (olivermadge)

<details>
The following code causes clang-19 to crash with the error `clang++-19: error: clang frontend command failed with exit code 139 (use -v to see invocation)`:

```
#include &lt;utility&gt;
#include &lt;cstring&gt;
#include &lt;print&gt;

template &lt;typename TypeTuple&gt;
auto count_types() {
    return []&lt;size_t... Idx&gt;(std::index_sequence&lt;Idx...&gt;) {
        return ([](std::tuple_element_t&lt;Idx, TypeTuple&gt; base) {return base;}(1) + ...);
    }(std::make_index_sequence&lt;std::tuple_size_v&lt;TypeTuple&gt;&gt;{});
}

int main() {
    std::println("{}", count_types&lt;std::tuple&lt;int&gt;&gt;());
    std::println("{}", count_types&lt;std::tuple&lt;int, char&gt;&gt;());
}

```

Interestingly, if I introduce an additional intermediate lambda to extract Idx, it compiles without error:
```
#include &lt;utility&gt;
#include &lt;cstring&gt;
#include &lt;print&gt;

template &lt;typename TypeTuple&gt;
auto count_types() {
    return []&lt;size_t... Idx&gt;(std::index_sequence&lt;Idx...&gt;) {
        return ([]&lt;size_t Index&gt;(std::integral_constant&lt;size_t, Index&gt;) {
            return [](std::tuple_element_t&lt;Index, TypeTuple&gt; base) {return base;}(1);
        }(std::integral_constant&lt;size_t, Idx&gt;()) + ...);
    }(std::make_index_sequence&lt;std::tuple_size_v&lt;TypeTuple&gt;&gt;{});
}

int main() {
    std::println("{}", count_types&lt;std::tuple&lt;int&gt;&gt;());
    std::println("{}", count_types&lt;std::tuple&lt;int, char&gt;&gt;());
}

```

Clang-19 from Arch Linux (version 19.1.7 on x86-64) invoked as `clang++-19 -emit-llvm -Xclang -disable-llvm-passes --std=c++20 clang_bug.cpp -c -o clang`

The same code compiles and runs with the expected result on clang++-20, g++-14 and g++-15

[clang_bug-86f8f9.cpp.log](https://github.com/user-attachments/files/21531707/clang_bug-86f8f9.cpp.log)
[clang_bug-86f8f9.sh.log](https://github.com/user-attachments/files/21531708/clang_bug-86f8f9.sh.log)
</details>


---

