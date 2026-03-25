# tuple variant crashing iterating over vector with std::apply (clang 17.0.1 - clang 19.1.0)

**Author:** wh1t3lord
**URL:** https://github.com/llvm/llvm-project/issues/126246
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-02-08T03:53:22Z

## Body

## Problem 

Here's a function that fills a std::vector<std::variant<Ts...>> from a std::vector<std::string_view>. The function template takes a std::tuple<Ts...> as a type parameter and uses std::apply to iterate over the tuple's types. It attempts to convert each string in the input vector to the corresponding type or keeps it as a string if conversion is not possible.

## Code & Proof

```cpp
#include <iostream>
#include <vector>
#include <variant>
#include <tuple>
#include <string_view>
#include <cstdlib> // for std::atoi, std::atof
#include <type_traits>

template <typename... Ts>
std::vector<std::variant<Ts...>> fill_vector(const std::vector<std::string_view>& input) {
    using VariantType = std::variant<Ts...>;
    std::vector<VariantType> result;
    result.reserve(input.size());

    for (const auto& str : input) {
        bool converted = false;

        // Create a temporary tuple to pass to std::apply
        std::tuple<Ts...> tempTuple;

        std::apply([&](auto&&... args) {
            // Fold expression to try each type
            (([&] {
                using T = std::decay_t<decltype(args)>;
                if (!converted) {
                    if constexpr (std::is_integral_v<T>) {
                        try {
                            result.emplace_back(static_cast<T>(std::atoi(str.data())));
                            converted = true;
                        } catch (...) {}
                    } else if constexpr (std::is_floating_point_v<T>) {
                        try {
                            result.emplace_back(static_cast<T>(std::atof(str.data())));
                            converted = true;
                        } catch (...) {}
                    }
                }
            }()), ...);
        }, tempTuple);

        if (!converted) {
            result.emplace_back(std::string(str)); // Fallback to string
        }
    }

    return result;
}

int main() {
    std::vector<std::string_view> input = {"123", "45.67", "hello", "789"};

    auto output = fill_vector<int, double, std::string>(input);

    for (const auto& v : output) {
        std::visit([](const auto& value) { std::cout << value << " "; }, v);
    }

    return 0;
}

```

Proof №1: https://godbolt.org/z/TPv4vcGch (clang 17.0.1)

Proof №2: https://godbolt.org/z/178zP4x9v (clang 19.1.0) 

So both crashes.

### Working sample (Expected behaviour)

Unexpected but it compiles on gcc. 

Link: https://godbolt.org/z/W8jqWqzE3 (gcc 12.1)

Link: https://godbolt.org/z/n9GWG4EM8 (gcc 14.2)

## Crash backtrace

Proceed to godbolt site with attached link in previous section.

## Comments

### Comment 1 - zwuis

> Proof №2: https://godbolt.org/z/178zP4x9v (clang 19.1.0) 

Doesn't reproduce on Clang thunk. Please wait for Clang 20 release.

> Unexpected but it compiles on gcc. 

You can visit [cppreference](https://en.cppreference.com/w/cpp/string/byte/atoi) to see how `std::atoi` works.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (wh1t3lord)

<details>
## Problem 

Here's a function that fills a std::vector&lt;std::variant&lt;Ts...&gt;&gt; from a std::vector&lt;std::string_view&gt;. The function template takes a std::tuple&lt;Ts...&gt; as a type parameter and uses std::apply to iterate over the tuple's types. It attempts to convert each string in the input vector to the corresponding type or keeps it as a string if conversion is not possible.

## Code &amp; Proof

```cpp
#include &lt;iostream&gt;
#include &lt;vector&gt;
#include &lt;variant&gt;
#include &lt;tuple&gt;
#include &lt;string_view&gt;
#include &lt;cstdlib&gt; // for std::atoi, std::atof
#include &lt;type_traits&gt;

template &lt;typename... Ts&gt;
std::vector&lt;std::variant&lt;Ts...&gt;&gt; fill_vector(const std::vector&lt;std::string_view&gt;&amp; input) {
    using VariantType = std::variant&lt;Ts...&gt;;
    std::vector&lt;VariantType&gt; result;
    result.reserve(input.size());

    for (const auto&amp; str : input) {
        bool converted = false;

        // Create a temporary tuple to pass to std::apply
        std::tuple&lt;Ts...&gt; tempTuple;

        std::apply([&amp;](auto&amp;&amp;... args) {
            // Fold expression to try each type
            (([&amp;] {
                using T = std::decay_t&lt;decltype(args)&gt;;
                if (!converted) {
                    if constexpr (std::is_integral_v&lt;T&gt;) {
                        try {
                            result.emplace_back(static_cast&lt;T&gt;(std::atoi(str.data())));
                            converted = true;
                        } catch (...) {}
                    } else if constexpr (std::is_floating_point_v&lt;T&gt;) {
                        try {
                            result.emplace_back(static_cast&lt;T&gt;(std::atof(str.data())));
                            converted = true;
                        } catch (...) {}
                    }
                }
            }()), ...);
        }, tempTuple);

        if (!converted) {
            result.emplace_back(std::string(str)); // Fallback to string
        }
    }

    return result;
}

int main() {
    std::vector&lt;std::string_view&gt; input = {"123", "45.67", "hello", "789"};

    auto output = fill_vector&lt;int, double, std::string&gt;(input);

    for (const auto&amp; v : output) {
        std::visit([](const auto&amp; value) { std::cout &lt;&lt; value &lt;&lt; " "; }, v);
    }

    return 0;
}

```

Proof №1: https://godbolt.org/z/TPv4vcGch (clang 17.0.1)

Proof №2: https://godbolt.org/z/178zP4x9v (clang 19.1.0) 

So both crashes.

### Working sample (Expected behaviour)

Unexpected but it compiles on gcc. 

Link: https://godbolt.org/z/W8jqWqzE3 (gcc 12.1)

Link: https://godbolt.org/z/n9GWG4EM8 (gcc 14.2)

## Crash backtrace

Proceed to godbolt site with attached link in previous section.
</details>


---

### Comment 3 - shafik

This looks fixed on trunk, maybe this is related to https://github.com/llvm/llvm-project/issues/122417 

CC @zyn0217 

---

### Comment 4 - zyn0217

As a workaround you can inline the type-alias into the template arugments: https://godbolt.org/z/P57q6h7os

---

