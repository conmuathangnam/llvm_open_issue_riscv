# clang-21 regression: "enable_if attribute expression never produces a constant expression" when compiling LLVM in C++23

**Author:** vogelsgesang
**URL:** https://github.com/llvm/llvm-project/issues/150131
**Status:** Closed
**Labels:** duplicate, clang:frontend, c++23, regression:21
**Closed Date:** 2025-07-23T01:34:08Z

## Body

The command 

```
echo '#include "llvm/ADT/StringExtras.h"' | clang++-21 -x c++ - -std=c++23
```

fails with the compilation error

```
In file included from <stdin>:1:
In file included from ./llvm/include/llvm/ADT/StringExtras.h:19:
In file included from ./llvm/include/llvm/ADT/SmallString.h:18:
./llvm/include/llvm/ADT/StringRef.h:871:22: error: 'enable_if' attribute
      expression never produces a constant expression
  871 |         __attribute((enable_if(__builtin_strlen(Str) == N - 1,
      |                      ^
./llvm/include/llvm/ADT/StringExtras.h:150:34: note: in instantiation of
      function template specialization 'llvm::StringLiteral::StringLiteral<33UL>' requested here
  150 |   static constexpr StringLiteral Punctuations =
      |                                  ^
./llvm/include/llvm/ADT/StringRef.h:871:49: note: read of variable 'Str'
      whose value is not known
  871 |         __attribute((enable_if(__builtin_strlen(Str) == N - 1,
      |                                                 ^
./llvm/include/llvm/ADT/StringRef.h:867:42: note: declared here
  867 |     constexpr StringLiteral(const char (&Str)[N])
      |                                          ^
1 error generated.
```

Both clang-21 and the used LLVM source were taken from the 21.1.0-rc1 tag.

The same command works against clang++ version 20, as such this is a regression compared to clang-20.
(The issue also only appears for C++23, using C++20 on clang-21 works)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Adrian Vogelsgesang (vogelsgesang)

<details>
The command 

```
echo '#include "llvm/ADT/StringExtras.h"' | clang++-21 -x c++ - -std=c++23
```

fails with the compilation error

```
In file included from &lt;stdin&gt;:1:
In file included from ./llvm/include/llvm/ADT/StringExtras.h:19:
In file included from ./llvm/include/llvm/ADT/SmallString.h:18:
./llvm/include/llvm/ADT/StringRef.h:871:22: error: 'enable_if' attribute
      expression never produces a constant expression
  871 |         __attribute((enable_if(__builtin_strlen(Str) == N - 1,
      |                      ^
./llvm/include/llvm/ADT/StringExtras.h:150:34: note: in instantiation of
      function template specialization 'llvm::StringLiteral::StringLiteral&lt;33UL&gt;' requested here
  150 |   static constexpr StringLiteral Punctuations =
      |                                  ^
./llvm/include/llvm/ADT/StringRef.h:871:49: note: read of variable 'Str'
      whose value is not known
  871 |         __attribute((enable_if(__builtin_strlen(Str) == N - 1,
      |                                                 ^
./llvm/include/llvm/ADT/StringRef.h:867:42: note: declared here
  867 |     constexpr StringLiteral(const char (&amp;Str)[N])
      |                                          ^
1 error generated.
```

Both clang-21 and the used LLVM source were taken from the 21.1.0-rc1 tag.

The same command works against clang++ version 20, as such this is a regression compared to clang-20.
(The issue goes away when using C++20 instead of C++23)
</details>


---

