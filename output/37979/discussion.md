# Clang refuses to compile code that other compilers do due to `__restrict`

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/37979

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [38631](https://llvm.org/bz38631) |
| Version | 6.0 |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor,@hfinkel,@dobbelaj-snps |

## Extended Description 
I wasn't sure to call this a "bug" because `__restrict` keyword is supposedly a compiler extension, so portability is not guaranteed.

However Clang is the only compiler (vs MSVC and GCC) to refuse to build this simple snippet of code and seems to me this behavior is not really intended/expected from Clang:
```cpp
struct Obj
{
    float * __restrict myPtr[2];
};

int main()
{
    Obj a, b;

    a = b;

    return 0;
}
```
The error is:
```
main.cpp:742:8: error: cannot initialize a parameter of type 'void *' with an rvalue of type 'float *__restrict (*)[2]'
struct Obj
       ^~~
../../src/main.cpp:751:7: note: in implicit copy assignment operator for 'Obj' first required here
    a = b;
```
Removing the `__restrict` keyword fixes the problem. Also another way to fix the problem is to split `myPtr[2]` into two variables, e.g. `myPtr0` and `myPtr1`

Using `__restrict__` instead of `__restrict` yields the same outcome.

Invocation is:
```
clang++-6.0 -Wall -Wextra -g -DDEBUG -MD -MT CMakeFiles/Playground.dir/src/main.cpp.o -MF CMakeFiles/Playground.dir/src/main.cpp.o.d -o CMakeFiles/Playground.dir/src/main.cpp.o -c ../../src/main.cpp

clang++-6.0 --version
clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

## Comments

### Comment 1 - frederick-vs-ja

Currently, this causes crash ([Godbolt link](https://godbolt.org/z/xYMz5WT5M)).

Interestingly, Clang correctly accepts a similar program in C modes now ([Godbolt link](https://godbolt.org/z/TE7qPW5j8)).

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (llvmbot)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [38631](https://llvm.org/bz38631) |
| Version | 6.0 |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @<!-- -->DougGregor,@<!-- -->hfinkel,@<!-- -->dobbelaj-snps |

## Extended Description 
I wasn't sure to call this a "bug" because `__restrict` keyword is supposedly a compiler extension, so portability is not guaranteed.

However Clang is the only compiler (vs MSVC and GCC) to refuse to build this simple snippet of code and seems to me this behavior is not really intended/expected from Clang:
```cpp
struct Obj
{
    float * __restrict myPtr[2];
};

int main()
{
    Obj a, b;

    a = b;

    return 0;
}
```
The error is:
```
main.cpp:742:8: error: cannot initialize a parameter of type 'void *' with an rvalue of type 'float *__restrict (*)[2]'
struct Obj
       ^~~
../../src/main.cpp:751:7: note: in implicit copy assignment operator for 'Obj' first required here
    a = b;
```
Removing the `__restrict` keyword fixes the problem. Also another way to fix the problem is to split `myPtr[2]` into two variables, e.g. `myPtr0` and `myPtr1`

Using `__restrict__` instead of `__restrict` yields the same outcome.

Invocation is:
```
clang++-6.0 -Wall -Wextra -g -DDEBUG -MD -MT CMakeFiles/Playground.dir/src/main.cpp.o -MF CMakeFiles/Playground.dir/src/main.cpp.o.d -o CMakeFiles/Playground.dir/src/main.cpp.o -c ../../src/main.cpp

clang++-6.0 --version
clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```
</details>


---

