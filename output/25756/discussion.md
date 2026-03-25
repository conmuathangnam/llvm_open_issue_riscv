# call of overloaded `function(<brace-enclosed initializer list>)` is ambiguous

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/25756

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [25382](https://llvm.org/bz25382) |
| Version | 3.5 |
| OS | Windows NT |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor |

## Extended Description 
Source:
```cpp
// a.C

#include <initializer_list>
int f(const std::initializer_list<int>&) {return 1;}
int f(const std::initializer_list<const int>&) {return 2;}
int x = f({37});
```
Found in Clang 3.5 and it's not fixed in Clang 3.7 yet.
```
clang++ -std=c++11 -c a.C
```

With g++ 4.8.3:
```console
> g++ -std=c++11 -c a.C
lib_over07.C:4:15: error: call of overloaded "f(<brace-enclosed initializer list>)" is ambiguous
int x = f({37});
               ˆ
```
But with Clang, it doesn't report an error and choose the non const function to call.
It's possible to write a test case where this results in doing something wrong functionally as well, if Clang does not stop user from doing the wrong thing.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [25382](https://llvm.org/bz25382) |
| Version | 3.5 |
| OS | Windows NT |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->DougGregor |

## Extended Description 
Source:
```cpp
// a.C

#include &lt;initializer_list&gt;
int f(const std::initializer_list&lt;int&gt;&amp;) {return 1;}
int f(const std::initializer_list&lt;const int&gt;&amp;) {return 2;}
int x = f({37});
```
Found in Clang 3.5 and it's not fixed in Clang 3.7 yet.
```
clang++ -std=c++11 -c a.C
```

With g++ 4.8.3:
```console
&gt; g++ -std=c++11 -c a.C
lib_over07.C:4:15: error: call of overloaded "f(&lt;brace-enclosed initializer list&gt;)" is ambiguous
int x = f({37});
               ˆ
```
But with Clang, it doesn't report an error and choose the non const function to call.
It's possible to write a test case where this results in doing something wrong functionally as well, if Clang does not stop user from doing the wrong thing.
</details>


---

