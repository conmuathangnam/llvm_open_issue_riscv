# clang accepts a call to an ambiguous inherited constructor in some cases

**Author:** ec04fc15-fa35-46f2-80e1-5d271f2ef708
**URL:** https://github.com/llvm/llvm-project/issues/44357

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [45012](https://llvm.org/bz45012) |
| Version | trunk |
| OS | Linux |
| CC | @DougGregor,@zygoloid |

## Extended Description 
Testcase:
```cpp
struct A { A(int); };
struct B : virtual A { using A::A; };
struct C : A, B { using A::A; using B::B; };
C c(123);
```
Clang accepts and passes the `123` on to the constructors of both `A` base subobjects. This code is ill-formed under [class.inhctor.init]p2:

> If the constructor was inherited from multiple base class subobjects of type B, the program is ill-formed.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (ec04fc15-fa35-46f2-80e1-5d271f2ef708)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [45012](https://llvm.org/bz45012) |
| Version | trunk |
| OS | Linux |
| CC | @<!-- -->DougGregor,@<!-- -->zygoloid |

## Extended Description 
Testcase:
```cpp
struct A { A(int); };
struct B : virtual A { using A::A; };
struct C : A, B { using A::A; using B::B; };
C c(123);
```
Clang accepts and passes the `123` on to the constructors of both `A` base subobjects. This code is ill-formed under [class.inhctor.init]p2:

&gt; If the constructor was inherited from multiple base class subobjects of type B, the program is ill-formed.
</details>


---

### Comment 2 - shafik

clang catches it now but only makes it a warning, this ICEs gcc and EDG/MSVC reject: https://godbolt.org/z/M6oMooqcE

---

