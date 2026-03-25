# ICE segfault on `static_assert` and instantiation constrained by concept with lambda NTTPs

**Author:** Eczbek
**URL:** https://github.com/llvm/llvm-project/issues/161467

## Body

https://godbolt.org/z/YEh68rqea

```cpp
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"



template<typename T>
using type = T;

template<typename T>
struct wrap_type {
	using type = T;
};

#define MAKE_VALUE(...) ([]<typename... $> { return __VA_ARGS__; })
#define MAKE_TYPE(...) MAKE_VALUE(wrap_type<__VA_ARGS__>())

// #define LIFT_VALUE(...) MAKE_VALUE(__VA_ARGS__<$...>)
#define LIFT_TYPE(...) MAKE_TYPE(__VA_ARGS__<$...>)

#define GET_VALUE(_name, ...) _name.template operator()<__VA_ARGS__>()
#define GET_TYPE(_name, ...) typename decltype(GET_VALUE(_name, __VA_ARGS__))::type



template<typename T, auto pred, auto proj, typename... Us>
concept make_concept = GET_VALUE(pred, GET_TYPE(proj, T), GET_TYPE(proj, Us)...);

constexpr auto is_same = MAKE_VALUE(requires (type<int(wrap_type<$...[0]>)>& f) {
	f(wrap_type<$...[1]>());
});

template<typename... Ts>
requires(sizeof...(Ts) == 1)
using add_cv = volatile const Ts...[0];



static_assert(make_concept<const int, is_same, LIFT_TYPE(add_cv), int>);

template<make_concept<is_same, LIFT_TYPE(add_cv), int>>
int x;

int main() {
	x<const int>;
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 <source>
1.	<source>:43:14: current parser token ';'
2.	<source>:42:12: parsing function body 'main'
3.	<source>:42:12: in compound statement ('{}')
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Eczbek)

<details>
https://godbolt.org/z/YEh68rqea

```cpp
#pragma clang diagnostic ignored "-Wdollar-in-identifier-extension"



template&lt;typename T&gt;
using type = T;

template&lt;typename T&gt;
struct wrap_type {
	using type = T;
};

#define MAKE_VALUE(...) ([]&lt;typename... $&gt; { return __VA_ARGS__; })
#define MAKE_TYPE(...) MAKE_VALUE(wrap_type&lt;__VA_ARGS__&gt;())

// #define LIFT_VALUE(...) MAKE_VALUE(__VA_ARGS__&lt;$...&gt;)
#define LIFT_TYPE(...) MAKE_TYPE(__VA_ARGS__&lt;$...&gt;)

#define GET_VALUE(_name, ...) _name.template operator()&lt;__VA_ARGS__&gt;()
#define GET_TYPE(_name, ...) typename decltype(GET_VALUE(_name, __VA_ARGS__))::type



template&lt;typename T, auto pred, auto proj, typename... Us&gt;
concept make_concept = GET_VALUE(pred, GET_TYPE(proj, T), GET_TYPE(proj, Us)...);

constexpr auto is_same = MAKE_VALUE(requires (type&lt;int(wrap_type&lt;$...[0]&gt;)&gt;&amp; f) {
	f(wrap_type&lt;$...[1]&gt;());
});

template&lt;typename... Ts&gt;
requires(sizeof...(Ts) == 1)
using add_cv = volatile const Ts...[0];



static_assert(make_concept&lt;const int, is_same, LIFT_TYPE(add_cv), int&gt;);

template&lt;make_concept&lt;is_same, LIFT_TYPE(add_cv), int&gt;&gt;
int x;

int main() {
	x&lt;const int&gt;;
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 &lt;source&gt;
1.	&lt;source&gt;:43:14: current parser token ';'
2.	&lt;source&gt;:42:12: parsing function body 'main'
3.	&lt;source&gt;:42:12: in compound statement ('{}')
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - term-est

We are triggering
```cpp

  /// Return the array of arguments in this template argument pack.
  ArrayRef<TemplateArgument> getPackAsArray() const {
    assert(getKind() == Pack);
    return {Args.Args, Args.NumArgs};
  }
```
from TemplateBase.h:453

reproduction example
```cpp
template<typename>
struct wrap_type {};

template<typename, auto>
concept make_concept = true;

template <make_concept<[]<typename... $> { return wrap_type<$...>(); }>>
int x{};

auto main() -> int
{
  x<int>;
}
```

---

### Comment 3 - shafik

Original code goes back to clang-12: https://godbolt.org/z/dq4WEcx69

Reduced must not be the same b/c it goes back to clang-10: https://godbolt.org/z/q7P1q88ad

---

### Comment 4 - Backl1ght

it looks like in reduced repro, the parameter pack in `wrap_type<$...>` is wrongly substituted by int in `x<int>` , which is a Type instead of a Pack, during pack expansion.

---

### Comment 5 - zyn0217

The reduced example https://github.com/llvm/llvm-project/issues/161467#issuecomment-3355265364 is no longer reproducible after https://github.com/llvm/llvm-project/commit/e9972debc98ce5d00db47409248bbcf06fafaf73


---

