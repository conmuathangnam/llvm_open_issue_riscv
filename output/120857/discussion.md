# [Clang] parser error

**Author:** TrofiMichael
**URL:** https://github.com/llvm/llvm-project/issues/120857

## Body


I'm trying to compile the following code:
```
class BigInteger {
  friend bool operator==(const BigInteger&, const BigInteger&);
  friend class Rational;
};

bool operator==(const BigInteger&, const BigInteger&) = default;

int main() {}
```
https://godbolt.org/z/sKhKrv7WG

With clang++ -std=c++20

The compiler gives a parser error. 
The next three files contain a compilation log, a cpp file and a sh file.

 
- [compilation_log.txt](https://github.com/user-attachments/files/18219989/compilation_log.txt)
- [clang_issue_cpp.txt](https://github.com/user-attachments/files/18220002/clang_issue_cpp.txt)
- [clang_issue_sh.txt](https://github.com/user-attachments/files/18220004/clang_issue_sh.txt)

All the following versions of clang fail to compile this:
- clang 20.0.0git
- x86-64 clang from 19.1.0 to 15.0.0



## Comments

### Comment 1 - GrumpyPigSkin

The result of `getFriendDecl()` could return null as it is the result of a dynamic cast. Would a simple `nullptr` check here be enough? Or is it the result of some deeper issue where `getFriendDecl` should never return null?


```cpp
    if (llvm::none_of(RD->friends(), [&](const FriendDecl *F) {
          const auto Ffd = F->getFriendDecl();
          if (Ffd) {
            return FD->getCanonicalDecl() == Ffd->getCanonicalDecl();
          }
          return false;
        }))
```

No compiler errors after the nullptr check is added.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mikhail Trofimov (TrofiMichael)

<details>

I'm trying to compile the following code:
```
class BigInteger {
  friend bool operator==(const BigInteger&amp;, const BigInteger&amp;);
  friend class Rational;
};

bool operator==(const BigInteger&amp;, const BigInteger&amp;) = default;

int main() {}
```
With clang++ -std=c++20

The compiler gives a parser error. 
The next three files contain a compilation log, a cpp file and a sh file.

 
- [compilation_log.txt](https://github.com/user-attachments/files/18219989/compilation_log.txt)
- [clang_issue_cpp.txt](https://github.com/user-attachments/files/18220002/clang_issue_cpp.txt)
- [clang_issue_sh.txt](https://github.com/user-attachments/files/18220004/clang_issue_sh.txt)

All the following versions of clang fail to compile this:
- clang 20.0.0git
- x86-64 clang from 19.1.0 to 15.0.0


</details>


---

### Comment 3 - EugeneZelenko

@TrofiMichael: You could create reproducers like this: https://godbolt.org/z/sKhKrv7WG

---

### Comment 4 - shafik

> The result of `getFriendDecl()` could return null as it is the result of a dynamic cast. Would a simple `nullptr` check here be enough? Or is it the result of some deeper issue where `getFriendDecl` should never return null?
> 
>     if (llvm::none_of(RD->friends(), [&](const FriendDecl *F) {
>           const auto Ffd = F->getFriendDecl();
>           if (Ffd) {
>             return FD->getCanonicalDecl() == Ffd->getCanonicalDecl();
>           }
>           return false;
>         }))
> No compiler errors after the nullptr check is added.

That looks reasonable based on other used of `getFriendDecl()` but I have not dug into it too deeply.

---

### Comment 5 - GrumpyPigSkin

@shafik Okay, I'll make the change and make a PR. Will this need a test case to go along with it?

---

