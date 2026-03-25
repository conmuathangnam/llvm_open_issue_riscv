# [Regression] Clang accepts a syntactically invalid use of uniform_int_distribution<>(1, );

**Author:** AAAA-I
**URL:** https://github.com/llvm/llvm-project/issues/136254
**Status:** Closed
**Labels:** clang:frontend, regression:20
**Closed Date:** 2025-04-18T08:27:29Z

## Body

Clang-trunk accepts the following code without producing any syntax error.
https://godbolt.org/z/dnW5oWY84
```cpp
#include <bits/stdc++.h>
using namespace std;
int a() { 
    uniform_int_distribution<>(1, ); 
    return 0;
}
```
Expected (Clang 19):
```
<source>:4:35: error: expected expression
    4 |     uniform_int_distribution<>(1, ); 
      |                                   ^
1 error generated.
ASM generation compiler returned: 1
```

bisect to https://github.com/llvm/llvm-project/commit/925e1956cd5039fa2489b802d25555e247c34175

## Comments

### Comment 1 - zyn0217

Reduced to https://godbolt.org/z/EGveT313K

```cpp
template <class T>
struct uniform_int_distribution {
  uniform_int_distribution(T);
};

void call() { 
  uniform_int_distribution<int>(1, ); 
}
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (GINN-Imp)

<details>
Clang-trunk accepts the following code without producing any syntax error.
https://godbolt.org/z/dnW5oWY84
```cpp
#include &lt;bits/stdc++.h&gt;
using namespace std;
int a() { 
    uniform_int_distribution&lt;&gt;(1, ); 
    return 0;
}
```
Expected (Clang 19):
```
&lt;source&gt;:4:35: error: expected expression
    4 |     uniform_int_distribution&lt;&gt;(1, ); 
      |                                   ^
1 error generated.
ASM generation compiler returned: 1
```

bisect to https://github.com/llvm/llvm-project/commit/925e1956cd5039fa2489b802d25555e247c34175
</details>


---

### Comment 3 - zyn0217

This is very unfortunate because every users of `ParseExpressionList` have to handle the trailing-commas now!

https://godbolt.org/z/h9zq1rh6x

```cpp
void call() {
  [a(42, )]() {} ();

  int *b = new int(42, );

  struct S {
    int c;

    S() : c(42, ) {}
  };

  int d(42, );
}
```

---

