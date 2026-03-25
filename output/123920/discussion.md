# [LoopInterchange] Interchange potentially breaks the program correctness

**Author:** kasuga-fj
**URL:** https://github.com/llvm/llvm-project/issues/123920
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-01-29T10:30:56Z

## Body

In the following program, LoopInterchange incorrectly interchanges the innermost two loops.

```c
#define N 4
int a[N*N][N*N][N*N];
void f() {
  for (int i = 0; i < N; i++)
    for (int j = 1; j < 2*N; j++)
      for (int k = 1; k < 2*N; k++)
        a[2*i][k+1][j-1] -= a[i+N-1][k][j];
}
```

See also: https://godbolt.org/z/rfev9drn7 

Note that the current LoopInterchange interchanges these loops twice, therefore the order returns to the original one. So this case works fine.

The problem here is that the LoopInterchange regards `Dependence::DVEntry::LE` as '<'. As a result, the direction vector becomes `[< < >]`. Swapping the innermost loops changes this to `[< > <]`, which passes the legality check. In fact, we must also consider the direction vector such as `[= < >]`.

## Comments

### Comment 1 - kasuga-fj

CC: @madhur13490 @sjoerdmeijer 

---

### Comment 2 - kasuga-fj

I think there are at least three ways to solve this issue.

1. Stop replacing the `DVEntry` with the character.
    - This may require some effort, such as rewriting most of the legality check.
1. Duplicate the direction vector when we encounter `DVEntry::LE` or `DVEntry::GE` to express both `<` (or `>`) and `=`.
    - In this case, the original direction vector is `[LE LT GT]` and `[< < >]` and `[= < >]` should be created from it.
    - In the worst case,  this will exponentially increase the size of the dependency matrix.
        - For example, if the original one is `[LE LE LE]`, create 2^3 = 8 rows need to be created.
1. ~Abandon the interchange if we find `DVEntry::LE` or `DVEntry::GE`.~
    - edited: It may be better to replace them with `*` than just to give up.

---

### Comment 3 - sjoerdmeijer

Hi @kasuga-fj , thanks for the report and letting us know. 

I can look at this / pick this up on Monday. But please go ahead of course if you want to continue looking at this.  

---

### Comment 4 - kasuga-fj

Hi @sjoerdmeijer, thanks for the confirmation. I'll wait for you because I'd like to know your opinion.

---

### Comment 5 - kasuga-fj

I did a quick check in llvm-test-suite and found only six loops that have `LE/GE` in their direction vector, e.g., https://github.com/llvm/llvm-test-suite/blob/8e0f3abe458bacfca38757e5d7c1125c8a8d930b/MultiSource/Benchmarks/ASCI_Purple/SMG2000/struct_stencil.c#L166 .
At the moment, I think replacing them with `*` is a good approach because it is easy and seems to have less impact on performance. If there are no particular objections, I would like to proceed this way.

---

### Comment 6 - sjoerdmeijer

> I did a quick check in llvm-test-suite and found only six loops that have `LE/GE` in their direction vector, e.g., https://github.com/llvm/llvm-test-suite/blob/8e0f3abe458bacfca38757e5d7c1125c8a8d930b/MultiSource/Benchmarks/ASCI_Purple/SMG2000/struct_stencil.c#L166 . At the moment, I think replacing them with `*` is a good approach because it is easy and seems to have less impact on performance. If there are no particular objections, I would like to proceed this way.

Sounds like a good approach to me for now: let's make interchange correct first.
And replacint it with '*' should be conservative and correct.

---

