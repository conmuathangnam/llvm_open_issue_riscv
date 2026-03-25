# bad compilation with default arguments in clang 20

**Author:** rockwotj
**URL:** https://github.com/llvm/llvm-project/issues/141858
**Status:** Closed
**Labels:** clang:frontend, miscompilation, regression:20
**Closed Date:** 2025-06-03T16:51:39Z

## Body

https://godbolt.org/z/5s4GazKEP

```
#include <print>
#include <numeric>
#include <vector>
#include <string>

struct foo {std::string bar;};

const std::vector<foo> foos = {{"1"}, {"2"}, {"3"}};

size_t count(const std::vector<foo>& input = foos) {
  size_t c = 0;
  for (const foo& f : input) {
    c += f.bar.size();
  }
  return c;
}

int main() {
    std::print("answer 1: {}\n", count());
    std::print("answer 2: {}\n", count({{"1"}, {"2"}}));
    std::print("answer 3: {}\n", count());
}
```

When compiled in clang 20 we get 3 printed 3 times, while in clang 19 we get 3, 2, 3 printed. I suspect some kind of over-aggressive optimization is assuming the input to this function is always `foos`.

## Comments

### Comment 1 - efriedma-quic

Duplicate of #141114?

---

### Comment 2 - rockwotj

Looks very likely yes

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Tyler Rockwood (rockwotj)

<details>
https://godbolt.org/z/5s4GazKEP

```
#include &lt;print&gt;
#include &lt;numeric&gt;
#include &lt;vector&gt;
#include &lt;string&gt;

struct foo {std::string bar;};

const std::vector&lt;foo&gt; foos = {{"1"}, {"2"}, {"3"}};

size_t count(const std::vector&lt;foo&gt;&amp; input = foos) {
  size_t c = 0;
  for (const foo&amp; f : input) {
    c += f.bar.size();
  }
  return c;
}

int main() {
    std::print("answer 1: {}\n", count());
    std::print("answer 2: {}\n", count({{"1"}, {"2"}}));
    std::print("answer 3: {}\n", count());
}
```

When compiled in clang 20 we get 3 printed 3 times, while in clang 19 we get 3, 2, 3 printed. I suspect some kind of over-aggressive optimization is assuming the input to this function is always `foos`.
</details>


---

### Comment 4 - AaronBallman

CC @shafik 

---

