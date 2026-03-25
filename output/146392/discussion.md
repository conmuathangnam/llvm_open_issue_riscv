# [clang] Assertion `!isResultDependent() && "Generic selection is result-dependent but getResultIndex called!"' failed

**Author:** dmantipov
**URL:** https://github.com/llvm/llvm-project/issues/146392

## Body

Triggered by clang-tidy (HEAD at 629126ed44bd3ce5b6f476459c805be4e4e0c2ca), crash [test5.log](https://github.com/user-attachments/files/20984723/test5.log) and C reproducer [test5.txt](https://github.com/user-attachments/files/20984722/test5.txt).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Dmitry Antipov (dmantipov)

<details>
Triggered by clang-tidy (HEAD at 629126ed44bd3ce5b6f476459c805be4e4e0c2ca), crash [test5.log](https://github.com/user-attachments/files/20984723/test5.log) and C reproducer [test5.txt](https://github.com/user-attachments/files/20984722/test5.txt).
</details>


---

### Comment 2 - AaronBallman

Here's a smaller reproducer:
```
void foo(void) {}

void bar(void) {
  struct invalid *i;
  sizeof(
      ({_Generic(i->whatever,
                 unsigned : foo)(0);
        0;
      })
    );
}
```
https://godbolt.org/z/v796rnY8E

Things which seem to matter:
* the use of _Generic needs to be within a GNU expression statement
* the GNU expression statement needs to be within an operator which triggers a side effect diagnostic, like sizeof
* The controlling operand of _Generic needs to be an invalid expression
* One of the associations of the _Generic needs to be a function designator
* The result of the _Generic operator needs to be used for a function call expression

(Note: I've not spent any time debugging, so it's possible not all of this matters.)


---

### Comment 3 - shafik

Looks like it goes back to clang-12: https://godbolt.org/z/YvqK8bM3z

---

