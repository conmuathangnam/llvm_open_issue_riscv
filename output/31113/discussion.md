# incorrectly permitting definition of class template via injected-class-name

**Author:** ec04fc15-fa35-46f2-80e1-5d271f2ef708
**URL:** https://github.com/llvm/llvm-project/issues/31113

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [31765](https://llvm.org/bz31765) |
| Version | trunk |
| OS | Linux |
| CC | @DougGregor |

## Extended Description 
Clang accepts this:
```cpp
  template<typename T> struct A {};
  template<typename T> struct A<T>::A {};
```
It's not obvious whether this is a valid use of the injected-class-name (the standard doesn't say the lookup in a class-head ignores functions, but it must do), but this should produce a redefinition error if nothing else.

## Comments

### Comment 1 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

mentioned in issue llvm/llvm-bugzilla-archive#38286

---

### Comment 2 - ec04fc15-fa35-46f2-80e1-5d271f2ef708

Clang accepts the non-template case too:
```cpp
class s {};
class s::s {};
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (ec04fc15-fa35-46f2-80e1-5d271f2ef708)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [31765](https://llvm.org/bz31765) |
| Version | trunk |
| OS | Linux |
| CC | @<!-- -->DougGregor |

## Extended Description 
Clang accepts this:
```cpp
  template&lt;typename T&gt; struct A {};
  template&lt;typename T&gt; struct A&lt;T&gt;::A {};
```
It's not obvious whether this is a valid use of the injected-class-name (the standard doesn't say the lookup in a class-head ignores functions, but it must do), but this should produce a redefinition error if nothing else.
</details>


---

### Comment 4 - shafik

I believe [class.qual p1](https://eel.is/c++draft/class.qual#1) gives us what we need to say this is ill-formed.

No one but clang accepts this: https://godbolt.org/z/hjME4TGeP

---

