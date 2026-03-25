# Bogus warning: 'memset' will always overflow; destination buffer has size 1, but size argument is 2 [-Wfortify-source]

**Author:** stbergmann
**URL:** https://github.com/llvm/llvm-project/issues/124513
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-01-27T07:17:51Z

## Body

On recent LLVM trunk (towards LLVM 20), bisecting shows that <https://github.com/llvm/llvm-project/commit/0a9c08c59ba61e727e9dee6d71883d9106963442> "[Clang] Implement P2280R4 Using unknown pointers and references in constant expressions (https://github.com/llvm/llvm-project/pull/95474)" started to cause
```
$ cat test.cc
#include <cstring>
struct S { char m1, m2; };
void f(S & x) { std::memset(&x, 0, sizeof (S)); }
```
```
$ clang++ -std=c++23 -fsyntax-only test.cc
test.cc:3:17: warning: 'memset' will always overflow; destination buffer has size 1, but size argument is 2 [-Wfortify-source]
    3 | void f(S & x) { std::memset(&x, 0, sizeof (S)); }
      |                 ^
1 warning generated.
```

## Comments

### Comment 1 - stbergmann

ah, this has apparently already been fixed with <https://github.com/llvm/llvm-project/commit/c9637afec7ed72904c74c2fc71e990d378f3d7a6> "[Clang] Fix createConstexprUnknownAPValues to use zero offset when ceating APValue (https://github.com/llvm/llvm-project/pull/124478)"

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Stephan Bergmann (stbergmann)

<details>
On recent LLVM trunk (towards LLVM 20), bisecting shows that &lt;https://github.com/llvm/llvm-project/commit/0a9c08c59ba61e727e9dee6d71883d9106963442&gt; "[Clang] Implement P2280R4 Using unknown pointers and references in constant expressions (https://github.com/llvm/llvm-project/pull/95474)" started to cause
```
$ cat test.cc
#include &lt;cstring&gt;
struct S { char m1, m2; };
void f(S &amp; x) { std::memset(&amp;x, 0, sizeof (S)); }
```
```
$ clang++ -std=c++23 -fsyntax-only test.cc
test.cc:3:17: warning: 'memset' will always overflow; destination buffer has size 1, but size argument is 2 [-Wfortify-source]
    3 | void f(S &amp; x) { std::memset(&amp;x, 0, sizeof (S)); }
      |                 ^
1 warning generated.
```
</details>


---

### Comment 3 - shafik

Thank you for the bug report, I was pondering filing one just for tracking purposes :-)

---

