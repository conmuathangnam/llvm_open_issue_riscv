# [clang] NRVO used in C mode

**Author:** TedLyngmo
**URL:** https://github.com/llvm/llvm-project/issues/100902

## Body

This C program exits with `1` even though it should exit with `0`. To the best of my knowledge, NRVO is not allowed in C:
```c
typedef struct {
    int i, j;
    double a, b;
} S;

int result;

S test(S* q) {
    S s = {0, 0, 0, 0};
    result = &s == q;
    return s;
}

int main(void)
{
  S t = test(&t);
  return result;
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c

Author: Ted Lyngmo (TedLyngmo)

<details>
This C program exits with `1` even though it should exit with `0`. To the best of my knowledge, NRVO is not allowed in C:
```c
typedef struct {
    int i, j;
    double a, b;
} S;

int result;

S test(S* q) {
    S s = {0, 0, 0, 0};
    result = &amp;s == q;
    return s;
}

int main(void)
{
  S t = test(&amp;t);
  return result;
}
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ted Lyngmo (TedLyngmo)

<details>
This C program exits with `1` even though it should exit with `0`. To the best of my knowledge, NRVO is not allowed in C:
```c
typedef struct {
    int i, j;
    double a, b;
} S;

int result;

S test(S* q) {
    S s = {0, 0, 0, 0};
    result = &amp;s == q;
    return s;
}

int main(void)
{
  S t = test(&amp;t);
  return result;
}
```
</details>


---

### Comment 3 - AaronBallman

Agreed that this is not a valid optimization in C; Clang is marking the local variable as being available for NRVO: https://godbolt.org/z/PhdKhqPPc
```
|-FunctionDecl <line:8:1, line:12:1> line:8:3 used test 'S (S *)'
| |-ParmVarDecl <col:8, col:11> col:11 used q 'S *'
| `-CompoundStmt <col:14, line:12:1>
|   |-DeclStmt <line:9:5, col:23>
|   | `-VarDecl <col:5, col:22> col:7 used s 'S' nrvo cinit
```

---

### Comment 4 - pinskia

Note GCC has the opposite issue dealing with C++ where it optimizes away the comparison too early even with language mandatory NVRO, see https://cplusplus.github.io/CWG/issues/2868.html and https://gcc.gnu.org/bugzilla/show_bug.cgi?id=84414 . Note the optimizing away the comparison is how GCC handles this case here even though it does NVRO; though the NVRO is done iff the struct return has no address taken on it (after many optmizations).

Here is a replacement function where GCC does not do NVRO and the comparison is still kept around (but still does `return slot optimization`[RSO]) while clang does the NVRO [incorrectly]:
```
S test(S* q) __attribute__((noinline));
S test(S* q) {
    S s = {0, 0, 0, 0};
    void *a = q;
    void *a1 = &s;
    __asm__("":"+r"(a));
    __asm__("":"+r"(a1));
    result = a1 == a;
    return s;
}
```



---

