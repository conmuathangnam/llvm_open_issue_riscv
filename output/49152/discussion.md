# Incorrect partial specialization selected for zero-length-array type

**Author:** Matthewacon
**URL:** https://github.com/llvm/llvm-project/issues/49152

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [49808](https://llvm.org/bz49808) |
| Version | trunk |
| OS | Linux |
| CC | @DougGregor,@zygoloid |

## Extended Description 
Affected versions:
 - trunk
 - 11.0.1
 - 11.0.0
 - 10.0.1
 - 10.0.0
 - 9.0.1
 - 9.0.0
 - 8.0.1
 - 8.0.0
 - 7.1.0
 - 7.0.1
 - 7.0.0
 - 6.0.1
 - 6.0.0
 - 5.0.1
 - 5.0.0
 - 4.0.1
 - 4.0.0

Driver cmdline:
Note: For clang 4.0.1 and 4.0.0, '-std=c++1z' was substituted for '-std=c++17'
 clang++ -std=c++17

Code:
```cpp
 #include<type_traits>

 tempalte<typename>
 struct A : std::false_type {};

 template<typename T, auto N>
 struct A<T[N]> : std::true_type {};

 static_assert(A<int[0]>::value);
```
Expected behaviour:
The static assertion passes; the `A<T[N]>` partial specialization should've been selected.

Actual behaviour:
The static assertion fails; the base template is selected

Notes:
Even when adding the following partial specialization, the base template is still, incorrectly, selected:
```cpp
 template<typename T>
 struct A<T[0]> : std::true_type {};
```

## Comments

### Comment 1 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Thanks for the detailed and well-researched report.

Zero-length array types are invalid in standard C++. Clang accepts such types as an extension, but if such a type is formed in a SFINAE context, Clang considers that to be a substitution failure.

This is intentional; such behavior is necessary for standard conformance. We can't allow formation of zero-length array types as an extension during deduction because it would change the meaning of valid programs. For example:

```cpp
template<int N> void f(int[N]); // #1
template<int N> void f(...); // #2
void g(int *p) { f<0>(p); }
```

... is required to call `#2` in a conforming C++ compiler, because the call to `#1` would involve forming a zero-length array type.

The situation here is a little more indirect -- the invalid `int[0]` type is formed after deducing `T` = `int` and `N` = `0`, when we substitute them back into the `T[N]` -- but is fundamentally happening for the same reason.

So, while I'm sympathetic to your situation, I don't think we can reasonably extend our zero-length array extension to cover the `T[N]` case where `N` becomes `0` after deduction and substitution. GCC, ICC, and MSVC (which also permit zero-length arrays as an extension in other contexts) exhibit the same behavior as Clang in this situation.


> Even when adding the following partial specialization, the base template is still,
> incorrectly, selected:
>  template<typename T>
>  struct A<T[0]> : std::true_type {};

This case is more interesting: we could make this work without a conformance problem, because the `[0]` is present in the original template and isn't created by deduction and substitution. Other compilers do accept this variant of the testcase, which seems like sufficient motivation to go the extra mile here.

Confirming on the basis that we should address the case where the array bound is a non-dependent zero.

---

### Comment 2 - frederick-vs-ja

See also #54705.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Matthewacon (Matthewacon)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [49808](https://llvm.org/bz49808) |
| Version | trunk |
| OS | Linux |
| CC | @<!-- -->DougGregor,@<!-- -->zygoloid |

## Extended Description 
Affected versions:
 - trunk
 - 11.0.1
 - 11.0.0
 - 10.0.1
 - 10.0.0
 - 9.0.1
 - 9.0.0
 - 8.0.1
 - 8.0.0
 - 7.1.0
 - 7.0.1
 - 7.0.0
 - 6.0.1
 - 6.0.0
 - 5.0.1
 - 5.0.0
 - 4.0.1
 - 4.0.0

Driver cmdline:
Note: For clang 4.0.1 and 4.0.0, '-std=c++1z' was substituted for '-std=c++17'
 clang++ -std=c++17

Code:
```cpp
 #include&lt;type_traits&gt;

 tempalte&lt;typename&gt;
 struct A : std::false_type {};

 template&lt;typename T, auto N&gt;
 struct A&lt;T[N]&gt; : std::true_type {};

 static_assert(A&lt;int[0]&gt;::value);
```
Expected behaviour:
The static assertion passes; the `A&lt;T[N]&gt;` partial specialization should've been selected.

Actual behaviour:
The static assertion fails; the base template is selected

Notes:
Even when adding the following partial specialization, the base template is still, incorrectly, selected:
```cpp
 template&lt;typename T&gt;
 struct A&lt;T[0]&gt; : std::true_type {};
```
</details>


---

