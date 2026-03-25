# Wrong optimizations for pointers: `p == q ? p : q` -> `q`

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/43719
**Status:** Open
**Labels:** bugzilla, miscompilation, confirmed, llvm:analysis

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [44374](https://llvm.org/bz44374) |
| Version | trunk |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |
| CC | @kamleshbhalui,@rnk |

## Extended Description 
Similar to bug 44313.

The optimizer sometimes changes `p == q ? p : q` to `q`. This is wrong when the actual provenance of `p` differs from that of `q`.
There are two forms -- with the actual conditional operator and with the `if` statement.

The ideal example would be constructed with the help of restricted pointers but it's run into a theoretical problem -- see the first testcase in bug 44373.
My other examples require two conditionals to eliminate the possibility of UB. Comparison of integers should give stable results, hopefully that would be enough to demonstrate the problem.

gcc bug -- https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93052.

Example with the conditional operator and with dead malloc (the wrong optimization seems to be applied in Early CSE):

----------------------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

__attribute__((noipa,optnone)) // imagine it in a separate TU
static void *opaque(void *p) { return p; }

int main()
{
    int *q = malloc(sizeof(int));
    opaque(q);
    uintptr_t iq = (uintptr_t)(void *)q;
    free(q);

    int *p = malloc(sizeof(int));
    opaque(p);
    uintptr_t ip = (uintptr_t)(void *)p;

    uintptr_t ir = ip == iq ? ip : iq;
    if (ip == iq) {
        *p = 1;
        *(int *)(void *)ir = 2;
        printf("result: %d\n", *p);
    }
}
----------------------------------------------------------------------
$ clang -std=c11 -Weverything -Wno-unknown-attributes test.c && ./a.out
result: 2
$ clang -std=c11 -Weverything -Wno-unknown-attributes -O3 test.c && ./a.out
result: 1
----------------------------------------------------------------------
clang x86-64 version: clang version 10.0.0 (https://github.com/llvm/llvm-project.git fccac1ec16951e9a9811abf19e2c18be147854fc)
----------------------------------------------------------------------

The idea of problems arising from `p == q ? p : q` is from Chung-Kil Hur via https://gcc.gnu.org/bugzilla/show_bug.cgi?id=65752#c15.

## Comments

### Comment 1 - llvmbot


> Mostly we discussed the issue verbally, but I think Piotr covered the
> problem it in his CppCon talk about the devirtualization feature:
> https://www.youtube.com/watch?v=lQAxldEGOys

Thanks, that provided the necessary context for bug 34548, comment 49. After looking a bit deeper into it I filed bug 44472.

> I think the idea of pointer provenance is very similar to the idea of trying
> to assign dynamic C++ types to blobs of memory.

Yeah, in some sense. E.g., when you optimize an isolated function, both are unknown but propagate through assignments and such (until control is passed to another function).

---

### Comment 2 - rnk


> 
> > This type of issue has long been known, 
> 
> Do you have any links to corresponding discussions, bug reports etc.?

Mostly we discussed the issue verbally, but I think Piotr covered the problem it in his CppCon talk about the devirtualization feature:
https://www.youtube.com/watch?v=lQAxldEGOys

I think the idea of pointer provenance is very similar to the idea of trying to assign dynamic C++ types to blobs of memory.

---

### Comment 3 - llvmbot


> This type of issue has long been known, 

Do you have any links to corresponding discussions, bug reports etc.?

> and is not likely to get traction in the LLVM issue tracker.

I don't expect too much from these bug reports but they cover two specific issues and provide simple and diverse testcases for them (especially bug 44313). Hopefully they could help inform future discussions of bigger issues.

---

### Comment 4 - rnk

This type of issue has long been known, and is not likely to get traction in the LLVM issue tracker. The first time I remember meaningfully discussing this type of issue was around Piotr Padlewski's internship in 2014 or 2015.

In order to resolve this and the larger issue class, a real discussion on llvm-dev is needed. Historically, LLVM has done these value replacement optimizations, but it has not done pointer provenance optimizations, so fixing this will be a change in direction.

---

### Comment 5 - llvmbot

Example with a past-the-end pointer (Early CSE w/ MemorySSA):

----------------------------------------------------------------------
#include <stdio.h>

__attribute__((noipa,optnone)) // imagine it in a separate TU
static void *opaque(void *p) { return p; }

static int been_there = 0;

static int *f(int *p, int *q)
{
    if (p == q) {
        been_there = 1;
        return p;
    } else {
        been_there = 0;
        return q;
    }
}

int main()
{
    int x[5];
    int y[1];
    
    int *p = x;
    int *q = y + 1;
    opaque(q);

    int *p1 = opaque(p); // prevents early optimization of x==y+1
    int *r = f(p1, q);
    
    if (been_there) {
        *p = 1;
        *r = 2;
        printf("result: %d\n", *p);
    }
}
----------------------------------------------------------------------
$ clang -std=c11 -Weverything -Wno-unknown-attributes test.c && ./a.out
result: 2
$ clang -std=c11 -Weverything -Wno-unknown-attributes -O3 test.c && ./a.out
result: 1
----------------------------------------------------------------------
clang x86-64 version: clang version 10.0.0 (https://github.com/llvm/llvm-project.git fccac1ec16951e9a9811abf19e2c18be147854fc)
----------------------------------------------------------------------

The testcase in https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93052#c2 seems to be optimized in the same way by clang, so not pasting it here.

---

### Comment 6 - agostonrobert

Had a closer look on the first reproducer, changed it a bit and could reproduce the behavior, but I am not sure if the compiler would be wrong here. At least not with my slightly modified case:
```c
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

__attribute__((noinline,optnone)) // imagine it in a separate TU
static void *opaque(void *p) { return p; }

int main() {
    int *q = malloc(sizeof(int));
    opaque(q);
    uintptr_t iq = (uintptr_t)(void *)q;
    free(q);
    // iq points to a freed address

    int *p = malloc(sizeof(int));
    opaque(p);
    uintptr_t ip = (uintptr_t)(void *)p;
    // ip points to the just allocated address.
    // Whether it has the same value as iq, depends on the
    // implementation of iq (i.e. if it reused the same address
    // after free(q).

    uintptr_t ir = ip == iq ? ip : iq;

    assert(ip == iq && "expected malloc reusing the address");

    if (ip == iq) {
        // If we get here, then we can also be sure that ir points to
        // the same address as p. Therefore the order of the next two
        // stores and one load is important.
        // The "problem" is that the compiler will perform the load from
        // p, as it thinks it still holds the value 1 and the store into
        // ir wasn't aliasing p - which is wrong.
        *p = 1;
        *(int *)(void *)ir = 2;
        assert(*p == 2); // fails here
    }
}
```

I am using picolibc, where `malloc` has the attribute `noalias`, so I assume the compiler will generate the call to `printf` (or `assert` in my changed example) without reloading `*p` as a sideeffect of the `noalias` attribute.

The IR generated by my compiler (based on LLVM-19) looks like:
```
; Function Attrs: noreturn nounwind
define dso_local noundef i32 @main() local_unnamed_addr #0 {
entry:
  %call = tail call noalias dereferenceable_or_null(4) ptr @malloc(i32 noundef 4) #5
  tail call fastcc void @opaque(ptr noundef %call) #6
  tail call void @free(ptr noundef %call) #7
  %call2 = tail call noalias dereferenceable_or_null(4) ptr @malloc(i32 noundef 4) #5
  tail call fastcc void @opaque(ptr noundef %call2) #6
  %cmp = icmp eq ptr %call2, %call
  br i1 %cmp, label %if.then, label %cond.false6

cond.false6:                                      ; preds = %entry
  tail call void @__assert_func(ptr noundef nonnull @.str.1, i32 noundef 29, ptr noundef nonnull @__func__.main, ptr no
undef nonnull @.str.2) #8
  unreachable

if.then:                                          ; preds = %entry
  store i32 1, ptr %call2, align 4, !tbaa !9
  store i32 2, ptr %call, align 4, !tbaa !9
  tail call void @__assert_func(ptr noundef nonnull @.str.1, i32 noundef 40, ptr noundef nonnull @__func__.main, ptr no
undef nonnull @.str.3) #8
  unreachable
}
```
Am I missing something?

---

### Comment 7 - agostonrobert

I think the other example is similar: `p` was set to point to `x`, and no other pointer was set to point to `x` in a "defined behavior" manner. Having `q` _actually_ pointing to `x` by using UB doesn't change the fact that following strict aliasing rules, only `x` and `p` shall know the values stored in `x`. Therefore clobbering the value by `*r = 2;` works only because of the UB, and I think the compiler is correct to assume that after storing `*p = 1;`, it doesn't have to load `*p` to pass the value for the `printf` call.

---

### Comment 8 - dtcxzyw

Should be fixed by https://github.com/llvm/llvm-project/commit/1af627b592dd15bbe58136f902ced46251fc344d.


---

### Comment 9 - dtcxzyw

> *(int *)(void *)ir = 2;

Converting pointers into integers will lose the provenance information. I think `inttoptr(ptrtoint(x)) -> x` is still problematic.
cc @nikic 


---

### Comment 10 - nikic

Yes, that's tracked at https://github.com/llvm/llvm-project/issues/33896.



---

