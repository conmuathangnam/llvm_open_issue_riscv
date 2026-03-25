# [Clang] Accepts invalid alias template with the same name as its template parameter

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/123423
**Status:** Closed
**Labels:** clang:frontend, confirmed, diverges-from:gcc, diverges-from:msvc, diverges-from:edg
**Closed Date:** 2025-01-25T07:01:42Z

## Body

```c++
template <class T>
using T = int;
```

This is rejected by GCC, MSVC and EDG.

See https://compiler-explorer.com/z/6a5jWGn4s.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
```c++
template &lt;class T&gt;
using T = int;
```

This is rejected by GCC, MSVC and EDG.

See https://compiler-explorer.com/z/6a5jWGn4s.
</details>


---

### Comment 2 - zygoloid

Confirmed, violates [this rule](https://eel.is/c++draft/temp#local-6.sentence-1).

---

### Comment 3 - BaLiKfromUA

Hi folks!

I am interested in solving this issue, but I haven't contributed anything to the upstream Clang project yet and I see that this issue is not marked as a "good first issue".

However, I do have some basic experience with building, testing, and changing some experimental fork of Clang.

Is it fine for me to pick up this task or do you have other processes and expectations in Clang project?

P.S. Sorry for noob question :) 

---

### Comment 4 - cor3ntin

> Is it fine for me to pick up this task?

Of course!
 Here is some documentation https://llvm.org/docs/Contributing.html

In addition of the code change you should provide
 - A sufficiently detailed description/commit message
 - Tests (in `clang/tests`)
 - A release note in `clang/docs/ReleaseNotes.rst` under "bug fixes to c++ support"

I hope this help

---

### Comment 5 - BaLiKfromUA

Thank you!

I will look on this issue

---

