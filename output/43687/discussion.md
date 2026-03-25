# Wrong optimization: provenance affects comparison of saved bits of addresses of dead auto variables

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/43687

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [44342](https://llvm.org/bz44342) |
| Version | trunk |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @efriedma-quic,@aqjune,@LebedevRI,@nunoplopes |

## Extended Description 
It's known that the value of a pointer to an object becomes indeterminate after the object is dead (C11, 6.2.4p2). Whether its representation becomes indeterminate is up for debate but let's bypass the issue by saving the representation while the object is still alive. For example, we can cast it to an integer. And we'll get an ordinary integer, with a stable value etc., not affected by changes in the life of the original object. Right?

This seems to be broken for the equality operators when the operands are constructed from addresses of automatic variables and at least one of these variables is dead at the time of comparison.

----------------------------------------------------------------------
#include <stdio.h>

int main()
{
    unsigned long u, v;

    {
        int x[5];
        u = (unsigned long)x;
    }

    {
        int y[5];
        v = (unsigned long)y;
    }

    printf("u = %#lx\n", u);
    printf("v = %#lx\n", v);
    printf("diff = %#lx\n", u - v);
    printf("eq = %d\n", u == v);
}
----------------------------------------------------------------------
$ clang -std=c11 -Weverything -O3 test.c && ./a.out
u = 0x7ffd6e1f3de0
v = 0x7ffd6e1f3de0
diff = 0
eq = 0
----------------------------------------------------------------------
clang x86-64 version: clang version 10.0.0 (https://github.com/llvm/llvm-project.git 200cce345dcf114a1b1012bc9c68adef6c99a595)


If "diff" is 0 then "eq" should be 1.

gcc bug -- https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93010

## Comments

### Comment 1 - efriedma-quic


> The main question is, when comparing two integers for equality,
> and both integers are ptr2int to two distinct, non-aliasing memory regions,
> are we allowed to look past ptr2int and make use of aliasing info?

In general, non-aliasing doesn't mean non-equal.  For example, any pointer is noalias with a pointer to a constant global.  We have to construct a different proof to simplify value comparisons.

The comment to justify the current transform explains the relevant considerations, mostly... but it it isn't considering stack coloring as it currently exists in LLVM.  (https://github.com/llvm/llvm-project/blob/6080387f136af5a51bbc310abb08c7158d7cd9d2/llvm/lib/Analysis/InstructionSimplify.cpp#L2395)

Note that we do have some flexibility here under the as-if rule: if you remove the "diff" line from the original testcase, we can fold the comparison to a constant.

---

### Comment 2 - LebedevRI

The main question is, when comparing two integers for equality,
and both integers are ptr2int to two distinct, non-aliasing memory regions,
are we allowed to look past ptr2int and make use of aliasing info?

This isn't illegal as of right now:

----------------------------------------
define i32 @&#8203;_Z4testv() {
%0:
  %1 = alloca i64 4, align 4
  %2 = alloca i64 8, align 8
  %3 = alloca i64 8, align 8
  %4 = alloca i64 20, align 16
  %5 = alloca i64 20, align 16
  store i32 0, * %1, align 4
  %6 = gep inbounds * %4, 20 x i64 0, 4 x i64 0
  %7 = ptrtoint * %6 to i64
  store i64 %7, * %2, align 8
  %8 = gep inbounds * %5, 20 x i64 0, 4 x i64 0
  %9 = ptrtoint * %8 to i64
  store i64 %9, * %3, align 8
  %10 = load i64, * %2, align 8
  %11 = load i64, * %3, align 8
  %12 = icmp eq i64 %10, %11
  %13 = zext i1 %12 to i32
  ret i32 %13
}
=>
define i32 @&#8203;_Z4testv() {
%0:
  ret i32 0
}
Transformation seems to be correct!

Summary:
  1 correct transformations
  0 incorrect transformations
  0 errors

---

### Comment 3 - llvmbot

And the same with memcpy/memcmp:

----------------------------------------------------------------------
#include <string.h>
#include <stdio.h>

int main()
{
    unsigned char u[sizeof(int *)];
    {
        int x[5];
        int *p = x;
        memcpy(&u, &p, sizeof(int *));
    }

    unsigned char v[sizeof(int *)];
    {
        int y[5];
        int *q = y;
        memcpy(&v, &q, sizeof(int *));
    }

    printf("A: %d\n", memcmp(&u, &v, sizeof(int *)));
    printf("B: %d\n", memcmp(&u, &v, sizeof(int *)) == 0);
}
----------------------------------------------------------------------
$ clang -std=c11 -Weverything -O3 test.c && ./a.out
A: 0
B: 0
----------------------------------------------------------------------
clang x86-64 version: clang version 10.0.0 (https://github.com/llvm/llvm-project.git 200cce345dcf114a1b1012bc9c68adef6c99a595)

---

### Comment 4 - RalfJung

Is this the same issue as https://github.com/llvm/llvm-project/issues/132085? It sure looks similar on the surface.

---

### Comment 5 - RalfJung

Or maybe https://github.com/llvm/llvm-project/issues/45725 is the more correct issue?

---

