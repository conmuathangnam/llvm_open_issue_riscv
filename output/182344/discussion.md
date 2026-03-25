# [Clang] Regression causing ambiguous call error related to alias template (derived from issue in Unreal Engine)

**Author:** nga888
**URL:** https://github.com/llvm/llvm-project/issues/182344
**Status:** Closed
**Labels:** c++20, clang:frontend, regression:22
**Closed Date:** 2026-02-25T00:34:36Z

## Body

The following code no longer compiles successfully with Clang 22:
``` C++
#include <type_traits>

template <typename T>
  requires(!std::is_array_v<T>)
void f() {}

template <typename T>
  requires(std::is_bounded_array_v<T>)
void f() = delete;

struct Bar {};

template <typename T> using Foo = Bar;

template <typename T> void use() { f<Foo<T>>(); }
```
and produces the following error:
``` bash
$ clang++ -std=c++20 -c test-case.cpp
test-case.cpp:15:36: error: call to 'f' is ambiguous
   15 | template <typename T> void use() { f<Foo<T>>(); }
      |                                    ^~~~~~~~~
test-case.cpp:5:6: note: candidate function [with T = Bar]
    5 | void f() {}
      |      ^
test-case.cpp:9:6: note: candidate function [with T = Bar] has been explicitly deleted
    9 | void f() = delete;
      |      ^
1 error generated.
```
This issue was discovered whilst attempting to compile Unreal Engine 5.6 with Clang 22.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-20

Author: Andrew Ng (nga888)

<details>
The following code no longer compiles successfully with Clang 22:
``` C++
#include &lt;type_traits&gt;

template &lt;typename T&gt;
  requires(!std::is_array_v&lt;T&gt;)
void f() {}

template &lt;typename T&gt;
  requires(std::is_bounded_array_v&lt;T&gt;)
void f() = delete;

struct Bar {};

template &lt;typename T&gt; using Foo = Bar;

template &lt;typename T&gt; void use() { f&lt;Foo&lt;T&gt;&gt;(); }
```
and produces the following error:
``` bash
$ clang++ -std=c++20 -c test-case.cpp
test-case.cpp:15:36: error: call to 'f' is ambiguous
   15 | template &lt;typename T&gt; void use() { f&lt;Foo&lt;T&gt;&gt;(); }
      |                                    ^~~~~~~~~
test-case.cpp:5:6: note: candidate function [with T = Bar]
    5 | void f() {}
      |      ^
test-case.cpp:9:6: note: candidate function [with T = Bar] has been explicitly deleted
    9 | void f() = delete;
      |      ^
1 error generated.
```
This issue was discovered whilst attempting to compile Unreal Engine 5.6 with Clang 22.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Andrew Ng (nga888)

<details>
The following code no longer compiles successfully with Clang 22:
``` C++
#include &lt;type_traits&gt;

template &lt;typename T&gt;
  requires(!std::is_array_v&lt;T&gt;)
void f() {}

template &lt;typename T&gt;
  requires(std::is_bounded_array_v&lt;T&gt;)
void f() = delete;

struct Bar {};

template &lt;typename T&gt; using Foo = Bar;

template &lt;typename T&gt; void use() { f&lt;Foo&lt;T&gt;&gt;(); }
```
and produces the following error:
``` bash
$ clang++ -std=c++20 -c test-case.cpp
test-case.cpp:15:36: error: call to 'f' is ambiguous
   15 | template &lt;typename T&gt; void use() { f&lt;Foo&lt;T&gt;&gt;(); }
      |                                    ^~~~~~~~~
test-case.cpp:5:6: note: candidate function [with T = Bar]
    5 | void f() {}
      |      ^
test-case.cpp:9:6: note: candidate function [with T = Bar] has been explicitly deleted
    9 | void f() = delete;
      |      ^
1 error generated.
```
This issue was discovered whilst attempting to compile Unreal Engine 5.6 with Clang 22.
</details>


---

