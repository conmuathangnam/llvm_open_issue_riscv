# Clang 18+ frontend crash on including a header file

**Author:** scixor
**URL:** https://github.com/llvm/llvm-project/issues/85148
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2026-03-14T14:12:19Z

## Body

This issue is related to https://github.com/tcbrindle/flux/issues/174.
Initially tested on : 
```
Ubuntu clang version 19.0.0 (++20240222031214+307409a8872f-1~exp1~20240222151237.1514)
Target: x86_64-pc-linux-gnu
Thread model: posix
```
Clang 19 backtrace can be accessed [here](https://gist.github.com/Sid911/a76b694a6c69656a5fb9957a2dec2551#file-flux_clang19_stacktrace-txt) with compiler explorer link to reproduce on clang 18 [here](https://flux.godbolt.org/z/8e87j7oM6) 

The flux maintainer did patch a workaround in the related PR to the issue by bypassing using the iterator altogether and using internal `FLUX_FOR` macro but I doubt this compiler behavior is acceptable. I also have experienced this before in last 3 weeks while including some other header library unfortunately my memory is vague about what I was doing so I won't be able to reproduce it. 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Siddharth Sinha (Sid911)

<details>
This issue is related to https://github.com/tcbrindle/flux/issues/174.
Initially tested on : 
```
Ubuntu clang version 19.0.0 (++20240222031214+307409a8872f-1~exp1~20240222151237.1514)
Target: x86_64-pc-linux-gnu
Thread model: posix
```
Clang 19 backtrace can be accessed [here](https://gist.github.com/Sid911/a76b694a6c69656a5fb9957a2dec2551#file-flux_clang19_stacktrace-txt) with compiler explorer link to reproduce on clang 18 [here](https://flux.godbolt.org/z/8e87j7oM6) 

The flux maintainer did patch a workaround in the related PR to the issue by bypassing using the iterator altogether and using internal `FLUX_FOR` macro but I doubt this compiler behavior is acceptable. I also have experienced this before in last 3 weeks while including some other header library unfortunately my memory is vague about what I was doing so I won't be able to reproduce it. 
</details>


---

### Comment 2 - shafik

Maybe related: https://github.com/llvm/llvm-project/issues/70601



---

### Comment 3 - tcbrindle

Another reproducer from the same library: https://flux.godbolt.org/z/P1PGPesoj   (via https://github.com/tcbrindle/flux/issues/179)

Interestingly, if we insert an explicit call to the `flux::end` function before the range-for loop, then compilation succeeds: https://flux.godbolt.org/z/zeK7x1K7W

Clang 16 and 17 compile the code correctly, so this looks to be a regression in 18.1

---

### Comment 4 - tbaederr

With assertions: https://flux.godbolt.org/z/66Es3chYT

Looks like it's a `SourceLocExpr` where the `PresumedLoc` we compute in `EvaluateInContext` is just invalid.
Not sure if this should just fail or if this should be caught earlier.

---

### Comment 5 - Endilll

One reduction of this seems to be fixed on post-18 trunk (https://godbolt.org/z/h3a8EPcq7):
```cpp
template <typename>
concept sequence = true;

template <sequence S>
struct sequence_iterator {
  template <sequence SS>
  friend struct sequence_iterator;

  constexpr sequence_iterator(S) {}
};

constexpr sequence_iterator<int> end() {
  return sequence_iterator{0};
}
```

---

### Comment 6 - zyn0217

> One reduction of this seems to be fixed on post-18 trunk (https://godbolt.org/z/h3a8EPcq7):
> 
> ```cpp
> 
> template <typename>
> 
> concept sequence = true;
> 
> 
> 
> template <sequence S>
> 
> struct sequence_iterator {
> 
>   template <sequence SS>
> 
>   friend struct sequence_iterator;
> 
> 
> 
>   constexpr sequence_iterator(S) {}
> 
> };
> 
> 
> 
> constexpr sequence_iterator<int> end() {
> 
>   return sequence_iterator{0};
> 
> }
> 
> ```

Yeah, the pattern looks like https://github.com/llvm/llvm-project/issues/86769 to me.

---

### Comment 7 - MagentaTreehouse

Reduced:
```c++
#include <source_location>

struct assert_fn {
    inline constexpr void operator()(
                              std::source_location = std::source_location::current()) const
    {}
};
inline constexpr auto assert_ = ::assert_fn{};
#define FLUX_DEBUG_ASSERT (::assert_());

constexpr auto inc(auto const&)
{
    FLUX_DEBUG_ASSERT;
}

template auto inc(int const&);

int main() {}
```

See https://compiler-explorer.com/z/rM633jfc8. (`include` directive expanded)

---

### Comment 8 - scixor

Came back to this after a year, this has been fixed already on trunk. Not sure what attributed to the fix.

---

