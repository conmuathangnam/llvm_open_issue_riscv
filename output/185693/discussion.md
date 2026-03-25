# Candidate function is _explicitly_ deleted, not implicitly

**Author:** kparzysz
**URL:** https://github.com/llvm/llvm-project/issues/185693
**Status:** Closed
**Labels:** c++11, clang:frontend, clang:diagnostics, confirmed
**Closed Date:** 2026-03-16T22:31:32Z

## Body

The diagnostic message says that the candidate function was implicitly deleted while the deletion was most definitely explicit:

Testcase:
```c++
#include <iostream>

template <typename T>
void fred(const T &x) {
  std::cout << x << "\n";
}

template <>
void fred(const double &) = delete;

int main() {
  fred(8.0);
}
```

`clang++ tdel1.cc`

```
tdel1.cc:13:3: error: call to deleted function 'fred'
   13 |   fred(8.0);
      |   ^~~~
tdel1.cc:9:6: note: candidate function [with T = double] has been implicitly
      deleted
    9 | void fred(const double &) = delete;
      |      ^
1 error generated.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Krzysztof Parzyszek (kparzysz)

<details>
The diagnostic message says that the candidate function was implicitly deleted while the deletion was most definitely explicit:

Testcase:
```
#include &lt;iostream&gt;

template &lt;typename T&gt;
void fred(const T &amp;x) {
  std::cout &lt;&lt; x &lt;&lt; "\n";
}

template &lt;&gt;
void fred(const double &amp;) = delete;


int main() {
  fred(8.0);
}
```

`clang++ tdel1.cc`

```
tdel1.cc:13:3: error: call to deleted function 'fred'
   13 |   fred(8.0);
      |   ^~~~
tdel1.cc:9:6: note: candidate function [with T = double] has been implicitly
      deleted
    9 | void fred(const double &amp;) = delete;
      |      ^
1 error generated.
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-11

Author: Krzysztof Parzyszek (kparzysz)

<details>
The diagnostic message says that the candidate function was implicitly deleted while the deletion was most definitely explicit:

Testcase:
```c++
#include &lt;iostream&gt;

template &lt;typename T&gt;
void fred(const T &amp;x) {
  std::cout &lt;&lt; x &lt;&lt; "\n";
}

template &lt;&gt;
void fred(const double &amp;) = delete;

int main() {
  fred(8.0);
}
```

`clang++ tdel1.cc`

```
tdel1.cc:13:3: error: call to deleted function 'fred'
   13 |   fred(8.0);
      |   ^~~~
tdel1.cc:9:6: note: candidate function [with T = double] has been implicitly
      deleted
    9 | void fred(const double &amp;) = delete;
      |      ^
1 error generated.
```
</details>


---

### Comment 3 - abhijeetsharma200

Hey, just created a PR. Please take a look! Thanks for any advice

---

