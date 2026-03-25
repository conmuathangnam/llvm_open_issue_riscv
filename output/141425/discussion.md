# [Clang] ICE when lambda is passed to alias template using CTAD

**Author:** MorganCaron
**URL:** https://github.com/llvm/llvm-project/issues/141425
**Status:** Closed
**Labels:** clang:frontend, regression, confirmed, crash-on-valid, lambda
**Closed Date:** 2025-05-28T15:33:44Z

## Body

This minimal example triggers an internal compiler error (ICE) on Clang.

```cpp
#include <tuple>
#include <functional>

template<class... Args>
struct StdTupleLike
{
    template<class... Args2>
    StdTupleLike(Args2...)
    {}
};

template<class... Args>
StdTupleLike(Args...) -> StdTupleLike<Args...>;



template<class>
struct StdFunctionLike;

template<class Ret, class... Args>
struct StdFunctionLike<Ret(Args...)>
{
    template<class U>
    StdFunctionLike(U u): func{u}
    {}

    void operator()(Args... args)
    {
        func(args...);
    }

    Ret (*func)(Args...);
};



template<class... Funcs>
using FunctionsTuple = StdTupleLike<Funcs...>; // or: using FunctionsTuple = std::tuple<Funcs...>;
using NoFunctions = FunctionsTuple<>;

template<class Functions = NoFunctions>
inline auto execute(const Functions& = {}) -> void
{}

#if 1
struct TestSuite
{
    inline explicit TestSuite(std::function<void(TestSuite &)> function) // ICE
    {
        function(*this);
    }
};
#elif 0
struct TestSuite
{
    inline explicit TestSuite(StdFunctionLike<void(TestSuite &)> function) // BUILD
    {
        function(*this);
    }
};
#else
template<class T>
struct TestSuite
{
    inline explicit TestSuite(T &&function) // BUILD
    {
        function(*this);
    }
};
#endif

auto _ = TestSuite{[](auto &) {
    auto _ = [] {
        auto print = []() -> void {};
        execute(FunctionsTuple{print});
    };
}};

int main()
{
    return 0;
}
```

Tested on:
```
clang version 21.0.0 (/opt/llvm-git/llvm-project fd8fb7148674456ce3fb338864c9aa9f576feb22)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm-git/bin
```

Replacing ``std::function<void(TestSuite &)>`` with a custom wrapper like ``StdFunctionLike<void(TestSuite &)>`` avoids the ICE.

Same goes for using a generic forwarding constructor ``template<class T> TestSuite(T&&)``.

This seems to be related to how auto lambdas and CTAD interact inside the TestSuite construction when ``std::function`` is involved.

This compiles and runs correctly on GCC and MSVC.

The issue is isolated here on Compiler Explorer: https://godbolt.org/z/Ko5rYcros

Could you confirm if this is a known issue, or if a fix is planned?


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Morgan (MorganCaron)

<details>
This minimal example triggers an internal compiler error (ICE) on Clang.

```cpp
#include &lt;tuple&gt;
#include &lt;functional&gt;

template&lt;class... Args&gt;
struct StdTupleLike
{
    template&lt;class... Args2&gt;
    StdTupleLike(Args2...)
    {}
};

template&lt;class... Args&gt;
StdTupleLike(Args...) -&gt; StdTupleLike&lt;Args...&gt;;



template&lt;class&gt;
struct StdFunctionLike;

template&lt;class Ret, class... Args&gt;
struct StdFunctionLike&lt;Ret(Args...)&gt;
{
    template&lt;class U&gt;
    StdFunctionLike(U u): func{u}
    {}

    void operator()(Args... args)
    {
        func(args...);
    }

    Ret (*func)(Args...);
};



template&lt;class... Funcs&gt;
using FunctionsTuple = StdTupleLike&lt;Funcs...&gt;; // or: using FunctionsTuple = std::tuple&lt;Funcs...&gt;;
using NoFunctions = FunctionsTuple&lt;&gt;;

template&lt;class Functions = NoFunctions&gt;
inline auto execute(const Functions&amp; = {}) -&gt; void
{}

#if 1
struct TestSuite
{
    inline explicit TestSuite(std::function&lt;void(TestSuite &amp;)&gt; function) // ICE
    {
        function(*this);
    }
};
#elif 0
struct TestSuite
{
    inline explicit TestSuite(StdFunctionLike&lt;void(TestSuite &amp;)&gt; function) // BUILD
    {
        function(*this);
    }
};
#else
template&lt;class T&gt;
struct TestSuite
{
    inline explicit TestSuite(T &amp;&amp;function) // BUILD
    {
        function(*this);
    }
};
#endif

auto _ = TestSuite{[](auto &amp;) {
    auto _ = [] {
        auto print = []() -&gt; void {};
        execute(FunctionsTuple{print});
    };
}};

int main()
{
    return 0;
}
```

Tested on:
```
clang version 21.0.0 (/opt/llvm-git/llvm-project fd8fb7148674456ce3fb338864c9aa9f576feb22)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /opt/llvm-git/bin
```

Replacing ``std::function&lt;void(TestSuite &amp;)&gt;`` with a custom wrapper like ``StdFunctionLike&lt;void(TestSuite &amp;)&gt;`` avoids the ICE.

Same goes for using a generic forwarding constructor ``template&lt;class T&gt; TestSuite(T&amp;&amp;)``.

This seems to be related to how auto lambdas and CTAD interact inside the TestSuite construction when ``std::function`` is involved.

This compiles and runs correctly on GCC and MSVC.

The issue is isolated here on Compiler Explorer: https://godbolt.org/z/Ko5rYcros

Could you confirm if this is a known issue, or if a fix is planned?

</details>


---

### Comment 2 - frederick-vs-ja

Reduced example:

([Godbolt link](https://godbolt.org/z/z5eEYo1hK), used to compile in Clang 20)
```C++
template<bool B>
struct BoolConstant {
    static constexpr bool value = B;
    using type = BoolConstant;
};

using TrueType = BoolConstant<true>;
using FalseType = BoolConstant<false>;

template<class, class>
struct IsNotSameImpl : TrueType {};
template<class T>
struct IsNotSameImpl<T, T> : FalseType {};

template<bool, class>
struct EnableIfImpl {};
template<class T>
struct EnableIfImpl<true, T> { using type = T; };

template<bool Cond, class T = void>
using EnableIf = EnableIfImpl<Cond, T>::type;

template<class...>
using ExpandToTrue = TrueType;

template<class... X>
ExpandToTrue<EnableIf<X::value>...> AndHelper(int); // not defined
template<class...>
FalseType AndHelper(...); // not defined

template<class... X>
using AndType = decltype(::AndHelper<X...>(0));

template<class F, class... Args>
auto TestInvoke(F&& f, Args&&... args)
    noexcept(noexcept(static_cast<F&&>(f)(static_cast<Args&&>(args)...))) ->
        decltype(static_cast<F&&>(f)(static_cast<Args&&>(args)...));

struct Nat {};

template<class F, class... Args>
struct InvocableImpl {
    template <class F2, class... Args2>
    static auto TryCall(int) -> decltype(
        ::TestInvoke(static_cast<F2&&(*)()>(nullptr)(), static_cast<Args2&&(*)()>(nullptr)()...)); // not defined
    template <class F2, class... Args2>
    static auto TryCall(...) -> Nat; // not defined

    static constexpr bool value = IsNotSameImpl<decltype(TryCall<F, Args...>(0)), Nat>::value;
};


template<class... Args>
struct TupleLike {
    template<class... Args2>
    TupleLike(Args2...) {}
};

template<class... Args>
TupleLike(Args...) -> TupleLike<Args...>;

template<class... Args>
using AliasedTupleLike = TupleLike<Args...>;


template<class>
struct FunctionLike;

template<class Ret, class... Args>
struct FunctionLike<Ret(Args...)> {
    template<class U, bool = AndType<TrueType, InvocableImpl<U&, Args...>>::value>
    FunctionLike(U) {}
};


struct TestSuite {
    explicit TestSuite(FunctionLike<void(TestSuite &)>) {}
};

TestSuite _{[](auto &) {
    (void)AliasedTupleLike{[]() -> void {}};
}};
```

Alias template argument deduction and lambda seem needed to trigger this bug.

---

### Comment 3 - MorganCaron

Thanks for the help to reduce the code.
I have continued your reduction:

[Godbolt link](https://godbolt.org/z/h96M8eMYv)

```cpp
template<class... Lambda>
struct Container
{
    Container(Lambda...) {}
};

template<class... T>
using Alias = Container<T...>;

template<class = void>
struct Invocable {
    using T = decltype([]() {
        (void)Alias([]() -> void {});
    }());
};

struct Type {
    using T = bool;
};

template<class...>
struct ExpandType {
    using T = bool;
};

template<class... X>
using Expand = ExpandType<typename X::T...>;

Expand<Type, Invocable<>> _{};
```

---

### Comment 4 - shafik

@zyn0217 git bisect points to 032ad590d6bac76965fac17b9f2ce41e076a5a0a

---

