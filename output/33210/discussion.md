# `_Atomic _Bool` decrement gives infinite loop

**Author:** mibintc
**URL:** https://github.com/llvm/llvm-project/issues/33210
**Status:** Closed
**Labels:** c11, clang:codegen, bugzilla, miscompilation, confirmed
**Closed Date:** 2026-02-06T09:41:59Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [33863](https://llvm.org/bz33863) |
| Version | 4.0 |
| OS | MacOS X |
| CC | @DougGregor |

## Extended Description 
```console
$ clang -v
clang version 4.0.1 (tags/RELEASE_401/final)
Target: x86_64-apple-darwin14.4.0
Thread model: posix
InstalledDir: /site/spt/usr8/mblower/bin/clang+llvm-4.0.1-x86_64-apple-macosx10.9.0/bin
sptem26:_test mblower

$ cat tm.c
```
```cpp
void
test_minus (void)
{
 _Atomic _Bool a = 0;
 _Bool b = 1;
 _Atomic _Bool c = 1;
 a -= b; // Test completes OK if this line is removed.
 a -= c;
}

int main(void)
{
test_minus ();
}
```
I adapted this testcase from gcc testsuite. Execution of the 2nd decrement doesn't complete. It works OK without the first decrement.
--Melanie Blower (I work for Intel on the Intel c++ compiler)

## Comments

### Comment 1 - llvmbot

A slightly simplified repro looks like:
```cpp
  int main(void)
  {
   _Atomic _Bool a = 0;
   a -= 1;
   a -= 1;
  }
```
Compiling on trunk (llvm/trunk 308849, x86_64-apple-darwin16.6.0)
with `-O3` gives:
```asm
        movb    $0, -1(%rbp)
        movb    -1(%rbp), %al
        .p2align        4, 0x90
LBB0_1:
        andb    $1, %al
        movl    %eax, %ecx
        decb    %cl
        lock    cmpxchgb %cl, -1(%rbp)
        jne     LBB0_1
        movb    -1(%rbp), %al
        .p2align        4, 0x90
LBB0_3:
        andb    $1, %al
        movl    %eax, %ecx
        decb    %cl
        lock    cmpxchgb        %cl, -1(%rbp)
        jne     LBB0_3
```
The *first* decrement ends up assigning `0xFF` to `-1(%rbp)`.
Then when computing the second decrement we get:
```asm
  movb    -1(%rbp), %al
  # %al == 0xFF

  andb    $1, %al
  # %al == 0x1

  movl    %eax, %ecx
  # %ecx == 0x1

  decb    %cl
  # %cl == 0
```
At this point the follow compare-exchange happens:
```asm
  lock cmpxchgb %cl, -1(%rbp)
```
However, `%al == 0x1` and `-1(%rbp) == 0xFF` and the comparison fails.
The same sequence happens over and over.  Hence the infinite loop.

I'm not sure how best to fix this.

---

### Comment 2 - neldredge

Still present in clang 14.  The underlying issue seems to be that arithmetic on `atomic_bool` actually performs the arithmetic without coercing to 0 or 1.  A simple test case is (`clang -O3` on x86-64):
```cpp
#include <stdatomic.h>
#include <stdbool.h>
#include <assert.h>

atomic_bool b;

int main(void) {
    b += 2;
    assert(b);
}
```
The assertion should pass because the value assigned to `b` is either 2 or 3, both of which are truthy, so the value actually stored should be 1.  But the emitted code actually adds 2 and stores the value 2, which isn't valid for a `bool`.  The test in `assert(b)` then tests the low bit, which is unset, and so the assertion fails.

All the other compound assignment operators seem to have the same problem.  They're also optimized poorly, producing a `cmpxchg` loop where a simple `seq_cst` store (`xchg`) or bit flip (`lock xor [b], 1`) would suffice.

---

### Comment 3 - AaronBallman

Issue still reproduces as of Clang 20: https://godbolt.org/z/9ePh61bzK

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-c11

Author: Melanie Blower (mibintc)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [33863](https://llvm.org/bz33863) |
| Version | 4.0 |
| OS | MacOS X |
| CC | @<!-- -->DougGregor |

## Extended Description 
```console
$ clang -v
clang version 4.0.1 (tags/RELEASE_401/final)
Target: x86_64-apple-darwin14.4.0
Thread model: posix
InstalledDir: /site/spt/usr8/mblower/bin/clang+llvm-4.0.1-x86_64-apple-macosx10.9.0/bin
sptem26:_test mblower

$ cat tm.c
```
```cpp
void
test_minus (void)
{
 _Atomic _Bool a = 0;
 _Bool b = 1;
 _Atomic _Bool c = 1;
 a -= b; // Test completes OK if this line is removed.
 a -= c;
}

int main(void)
{
test_minus ();
}
```
I adapted this testcase from gcc testsuite. Execution of the 2nd decrement doesn't complete. It works OK without the first decrement.
--Melanie Blower (I work for Intel on the Intel c++ compiler)
</details>


---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Melanie Blower (mibintc)

<details>
|  |  |
| --- | --- |
| Bugzilla Link | [33863](https://llvm.org/bz33863) |
| Version | 4.0 |
| OS | MacOS X |
| CC | @<!-- -->DougGregor |

## Extended Description 
```console
$ clang -v
clang version 4.0.1 (tags/RELEASE_401/final)
Target: x86_64-apple-darwin14.4.0
Thread model: posix
InstalledDir: /site/spt/usr8/mblower/bin/clang+llvm-4.0.1-x86_64-apple-macosx10.9.0/bin
sptem26:_test mblower

$ cat tm.c
```
```cpp
void
test_minus (void)
{
 _Atomic _Bool a = 0;
 _Bool b = 1;
 _Atomic _Bool c = 1;
 a -= b; // Test completes OK if this line is removed.
 a -= c;
}

int main(void)
{
test_minus ();
}
```
I adapted this testcase from gcc testsuite. Execution of the 2nd decrement doesn't complete. It works OK without the first decrement.
--Melanie Blower (I work for Intel on the Intel c++ compiler)
</details>


---

