# [clang++] Bugs on overloading operator== and operator!=

**Author:** ConnectionFailedd
**URL:** https://github.com/llvm/llvm-project/issues/138404

## Body

I'm writing a generative meta-programming library. Below is the code that triggers the bugs.

``` C++
// test.cpp
#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

class CppExpression;

template<typename T1, typename T2>
concept same_after_decay = std::same_as<std::decay_t<T1>, std::decay_t<T2>>;
template<typename T>
concept is_cpp_expression = same_after_decay<T, CppExpression>;
template<typename T>
concept convertible_to_cpp_expression = is_cpp_expression<T>
                                        // below are for generating literals
                                        || same_after_decay<T, int> || same_after_decay<T, unsigned int> || same_after_decay<T, long> || same_after_decay<T, unsigned long> || same_after_decay<T, long long> || same_after_decay<T, unsigned long long>
                                        || same_after_decay<T, float> || same_after_decay<T, double> || same_after_decay<T, long double>
                                        || same_after_decay<T, char> || same_after_decay<T, char *> || same_after_decay<T, const char *> || same_after_decay<T, std::string> || same_after_decay<T, std::string_view>
                                        || same_after_decay<T, bool> || same_after_decay<T, std::nullptr_t>;

class CppExpression {
public:
    enum class Precedence {
        LOWEST = 0,
        COMMA = 1,
        ASSIGN = 2,
        LOGICAL_OR = 3,
        LOGICAL_AND = 4,
        BITWISE_OR = 5,
        BITWISE_XOR = 6,
        BITWISE_AND = 7,
        EQUALITY = 8,
        COMPARISON = 9,
        SHIFT = 10,
        SUM = 11,
        PRODUCT = 12,
        PREFIX = 13,
        SUFFIX = 14,
        HIGHEST = 15
    };

private:
    std::string expression_;
    Precedence precedence_;

    CppExpression(std::string expression, Precedence precedence = Precedence::LOWEST) : expression_(std::move(expression)), precedence_(precedence) {}
    CppExpression(const CppExpression & other) : expression_(other.expression_), precedence_(other.precedence_) {}
    CppExpression(CppExpression && other) noexcept : expression_(std::move(other.expression_)), precedence_(other.precedence_) {}

public:
    static CppExpression create_expression(const std::string & expression, Precedence precedence = Precedence::LOWEST) { return CppExpression(expression, precedence); }
    static CppExpression create_termial(const std::string & expression) { return CppExpression(expression, Precedence::HIGHEST); }
    template<convertible_to_cpp_expression T>
    static CppExpression create_literal(T && value) {
        if constexpr(same_after_decay<T, int>) { return CppExpression(std::to_string(value), Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, unsigned int>) { return CppExpression(std::to_string(value) + 'u', Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, long>) { return CppExpression(std::to_string(value) + 'l', Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, unsigned long>) { return CppExpression(std::to_string(value) + "ul", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, long long>) { return CppExpression(std::to_string(value) + "ll", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, unsigned long long>) { return CppExpression(std::to_string(value) + "ull", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, float>) { return CppExpression(std::to_string(value) + 'f', Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, double>) { return CppExpression(std::to_string(value), Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, long double>) { return CppExpression(std::to_string(value) + 'l', Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, char>) { return CppExpression("'" + std::string(1, value) + "'", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, char *>) { return CppExpression("\"" + std::string(value) + "\"", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, const char *>) { return CppExpression("\"" + std::string(value) + "\"", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, std::string>) { return CppExpression("\"" + value + "\"", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, std::string_view>) { return CppExpression("\"" + std::string(value) + "\"", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, bool>) { return CppExpression(value ? "true" : "false", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, std::nullptr_t>) { return CppExpression("nullptr", Precedence::HIGHEST); }
        else if constexpr(same_after_decay<T, CppExpression>) { return CppExpression(value.expression(), value.precedence()); }
        else { static_assert(false, "Invalid C++ literal type"); }
    }

    const std::string & expression() const { return expression_; }
    Precedence precedence() const { return precedence_; }
};

template<convertible_to_cpp_expression T1, convertible_to_cpp_expression T2>
requires is_cpp_expression<T1> || is_cpp_expression<T2>
CppExpression operator==(T1 && lhs, T2 && rhs) {
    return CppExpression::create_expression(CppExpression::create_literal(lhs).expression() + " == " + CppExpression::create_literal(rhs).expression(), CppExpression::Precedence::EQUALITY);
}
template<convertible_to_cpp_expression T1, convertible_to_cpp_expression T2>
requires is_cpp_expression<T1> || is_cpp_expression<T2>
CppExpression operator!=(T1 && lhs, T2 && rhs) {
    return CppExpression::create_expression(CppExpression::create_literal(lhs).expression() + " != " + CppExpression::create_literal(rhs).expression(), CppExpression::Precedence::EQUALITY);
}
template<convertible_to_cpp_expression T1, convertible_to_cpp_expression T2>
requires is_cpp_expression<T1> || is_cpp_expression<T2>
CppExpression operator>(T1 && lhs, T2 && rhs) {
    return CppExpression::create_expression(CppExpression::create_literal(lhs).expression() + " > " + CppExpression::create_literal(rhs).expression(), CppExpression::Precedence::EQUALITY);
}

int main() {
    auto var_a = CppExpression::create_termial("a");
    std::cout << (var_a == 1).expression() << std::endl;
    std::cout << (var_a != 1).expression() << std::endl;
    std::cout << (var_a > 1).expression() << std::endl;
}
```

**Compiling this code with clang++ takes extremely long time:**

``` shell
$ # clang++ 20.1.2 | Apple M3 | MacOS 15.4.1
$ time clang++ -std=c++23 test.cpp
clang++ -std=c++23 test.cpp  132.19s user 0.29s system 99% cpu 2:12.60 total
```

On another machine:

``` shell
$ # clang++ 18.1.3 | Intel Core i9 14900K | Ubuntu 24.04
$ time clang++ -std=c++23 test.cpp
clang++ -std=c++23 test.cpp  184.87s user 0.07s system 99% cpu 3:04.95 total
```

While g++ performs like:

``` shell
$ # g++ 13.3.0 | Intel Core i9 14900K | Ubuntu 24.04
$ time g++ -std=c++23 test.cpp
g++ -std=c++23 test.cpp  0.15s user 0.04s system 95% cpu 0.202 total
```

And according to my observation:
- Call to `operator!=` is the reason of long compile time here. Removing line `std::cout << (var_a != 1).expression() << std::endl;`, clang++ can compile fastly.
- But if removing the definition of `operator!=`, then call to `operator==` will cause a similarly long compile time.
- Except these two operator, any other binary operator has the same effect.
- The underlying reason may be the concept `convertible_to_cpp_expression`. If we reduce the options of literals, e.g. remove all options after `long double`, the compile time will be short.

Anyway, I think it is a bug because g++ can easily compile such file. However, due to my limited capabilities, I can only hope that you will be able to address it.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: __NYA__ (ConnectionFailedd)

<details>
I'm writing a generative meta-programming library. Below is the code that triggers the bugs.

``` C++
// test.cpp
#include &lt;concepts&gt;
#include &lt;iostream&gt;
#include &lt;string&gt;
#include &lt;type_traits&gt;
#include &lt;utility&gt;

class CppExpression;

template&lt;typename T1, typename T2&gt;
concept same_after_decay = std::same_as&lt;std::decay_t&lt;T1&gt;, std::decay_t&lt;T2&gt;&gt;;
template&lt;typename T&gt;
concept is_cpp_expression = same_after_decay&lt;T, CppExpression&gt;;
template&lt;typename T&gt;
concept convertible_to_cpp_expression = is_cpp_expression&lt;T&gt;
                                        // below are for generating literals
                                        || same_after_decay&lt;T, int&gt; || same_after_decay&lt;T, unsigned int&gt; || same_after_decay&lt;T, long&gt; || same_after_decay&lt;T, unsigned long&gt; || same_after_decay&lt;T, long long&gt; || same_after_decay&lt;T, unsigned long long&gt;
                                        || same_after_decay&lt;T, float&gt; || same_after_decay&lt;T, double&gt; || same_after_decay&lt;T, long double&gt;
                                        || same_after_decay&lt;T, char&gt; || same_after_decay&lt;T, char *&gt; || same_after_decay&lt;T, const char *&gt; || same_after_decay&lt;T, std::string&gt; || same_after_decay&lt;T, std::string_view&gt;
                                        || same_after_decay&lt;T, bool&gt; || same_after_decay&lt;T, std::nullptr_t&gt;;

class CppExpression {
public:
    enum class Precedence {
        LOWEST = 0,
        COMMA = 1,
        ASSIGN = 2,
        LOGICAL_OR = 3,
        LOGICAL_AND = 4,
        BITWISE_OR = 5,
        BITWISE_XOR = 6,
        BITWISE_AND = 7,
        EQUALITY = 8,
        COMPARISON = 9,
        SHIFT = 10,
        SUM = 11,
        PRODUCT = 12,
        PREFIX = 13,
        SUFFIX = 14,
        HIGHEST = 15
    };

private:
    std::string expression_;
    Precedence precedence_;

    CppExpression(std::string expression, Precedence precedence = Precedence::LOWEST) : expression_(std::move(expression)), precedence_(precedence) {}
    CppExpression(const CppExpression &amp; other) : expression_(other.expression_), precedence_(other.precedence_) {}
    CppExpression(CppExpression &amp;&amp; other) noexcept : expression_(std::move(other.expression_)), precedence_(other.precedence_) {}

public:
    static CppExpression create_expression(const std::string &amp; expression, Precedence precedence = Precedence::LOWEST) { return CppExpression(expression, precedence); }
    static CppExpression create_termial(const std::string &amp; expression) { return CppExpression(expression, Precedence::HIGHEST); }
    template&lt;convertible_to_cpp_expression T&gt;
    static CppExpression create_literal(T &amp;&amp; value) {
        if constexpr(same_after_decay&lt;T, int&gt;) { return CppExpression(std::to_string(value), Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, unsigned int&gt;) { return CppExpression(std::to_string(value) + 'u', Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, long&gt;) { return CppExpression(std::to_string(value) + 'l', Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, unsigned long&gt;) { return CppExpression(std::to_string(value) + "ul", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, long long&gt;) { return CppExpression(std::to_string(value) + "ll", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, unsigned long long&gt;) { return CppExpression(std::to_string(value) + "ull", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, float&gt;) { return CppExpression(std::to_string(value) + 'f', Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, double&gt;) { return CppExpression(std::to_string(value), Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, long double&gt;) { return CppExpression(std::to_string(value) + 'l', Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, char&gt;) { return CppExpression("'" + std::string(1, value) + "'", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, char *&gt;) { return CppExpression("\"" + std::string(value) + "\"", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, const char *&gt;) { return CppExpression("\"" + std::string(value) + "\"", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, std::string&gt;) { return CppExpression("\"" + value + "\"", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, std::string_view&gt;) { return CppExpression("\"" + std::string(value) + "\"", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, bool&gt;) { return CppExpression(value ? "true" : "false", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, std::nullptr_t&gt;) { return CppExpression("nullptr", Precedence::HIGHEST); }
        else if constexpr(same_after_decay&lt;T, CppExpression&gt;) { return CppExpression(value.expression(), value.precedence()); }
        else { static_assert(false, "Invalid C++ literal type"); }
    }

    const std::string &amp; expression() const { return expression_; }
    Precedence precedence() const { return precedence_; }
};

template&lt;convertible_to_cpp_expression T1, convertible_to_cpp_expression T2&gt;
requires is_cpp_expression&lt;T1&gt; || is_cpp_expression&lt;T2&gt;
CppExpression operator==(T1 &amp;&amp; lhs, T2 &amp;&amp; rhs) {
    return CppExpression::create_expression(CppExpression::create_literal(lhs).expression() + " == " + CppExpression::create_literal(rhs).expression(), CppExpression::Precedence::EQUALITY);
}
template&lt;convertible_to_cpp_expression T1, convertible_to_cpp_expression T2&gt;
requires is_cpp_expression&lt;T1&gt; || is_cpp_expression&lt;T2&gt;
CppExpression operator!=(T1 &amp;&amp; lhs, T2 &amp;&amp; rhs) {
    return CppExpression::create_expression(CppExpression::create_literal(lhs).expression() + " != " + CppExpression::create_literal(rhs).expression(), CppExpression::Precedence::EQUALITY);
}
template&lt;convertible_to_cpp_expression T1, convertible_to_cpp_expression T2&gt;
requires is_cpp_expression&lt;T1&gt; || is_cpp_expression&lt;T2&gt;
CppExpression operator&gt;(T1 &amp;&amp; lhs, T2 &amp;&amp; rhs) {
    return CppExpression::create_expression(CppExpression::create_literal(lhs).expression() + " &gt; " + CppExpression::create_literal(rhs).expression(), CppExpression::Precedence::EQUALITY);
}

int main() {
    auto var_a = CppExpression::create_termial("a");
    std::cout &lt;&lt; (var_a == 1).expression() &lt;&lt; std::endl;
    std::cout &lt;&lt; (var_a != 1).expression() &lt;&lt; std::endl;
    std::cout &lt;&lt; (var_a &gt; 1).expression() &lt;&lt; std::endl;
}
```

**Compiling this code with clang++ takes extremely long time:**

``` shell
$ # clang++ 20.1.2 | Apple M3 | MacOS 15.4.1
$ time clang++ -std=c++23 test.cpp
clang++ -std=c++23 test.cpp  132.19s user 0.29s system 99% cpu 2:12.60 total
```

On another machine:

``` shell
$ # clang++ 18.1.3 | Intel Core i9 14900K | Ubuntu 24.04
$ time clang++ -std=c++23 test.cpp
clang++ -std=c++23 test.cpp  184.87s user 0.07s system 99% cpu 3:04.95 total
```

While g++ performs like:

``` shell
$ # g++ 13.3.0 | Intel Core i9 14900K | Ubuntu 24.04
$ time g++ -std=c++23 test.cpp
g++ -std=c++23 test.cpp  0.15s user 0.04s system 95% cpu 0.202 total
```

And according to my observation:
- Call to `operator!=` is the reason of long compile time here. Removing line `std::cout &lt;&lt; (var_a != 1).expression() &lt;&lt; std::endl;`, clang++ can compile fastly.
- But if removing the definition of `operator!=`, then call to `operator==` will cause a similarly long compile time.
- Except these two operator, any other binary operator has the same effect.
- The underlying reason may be the concept `convertible_to_cpp_expression`. If we reduce the options of literals, e.g. remove all options after `long double`, the compile time will be short.

Anyway, I think it is a bug because g++ can easily compile such file. However, due to my limited capabilities, I can only hope that you will be able to address it.
</details>


---

### Comment 2 - zwuis

It seems to be fixed by #132849. Please wait for Clang 21 release.

---

### Comment 3 - cor3ntin

This still crashes https://compiler-explorer.com/z/MKbf7xW84

---

