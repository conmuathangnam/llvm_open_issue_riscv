# [clang] SARIF diagnostics cause crash

**Author:** cjdb
**URL:** https://github.com/llvm/llvm-project/issues/58430
**Status:** Closed
**Labels:** clang:frontend, clang:diagnostics, incomplete
**Closed Date:** 2025-02-07T14:45:50Z

## Body

```cpp
#include <concepts>

int f(int);
double f(double);

long g(std::signed_integral auto);
unsigned long g(std::unsigned_integral auto);

struct S {};

int main()
{
  f(0L);
  g(S{});
}
```
I'll provide more context as I investigate. The purpose of this bug is to create a work item for myself for now.

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-bug

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Christopher Di Bella (cjdb)

<details>
```cpp
#include &lt;concepts&gt;

int f(int);
double f(double);

long g(std::signed_integral auto);
unsigned long g(std::unsigned_integral auto);

struct S {};

int main()
{
  f(0L);
  g(S{});
}
```
I'll provide more context as I investigate. The purpose of this bug is to create a work item for myself for now.
</details>


---

### Comment 4 - cor3ntin

@cjdb You have more info?

---

### Comment 5 - AaronBallman

Closing as we've not gotten a response in a year. Feel free to reopen if the issue persists.

---

