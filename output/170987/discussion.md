# "counted_by" behavior is different in clang than gcc for a negative counted_by field

**Author:** vabridgers
**URL:** https://github.com/llvm/llvm-project/issues/170987
**Status:** Closed
**Labels:** clang:frontend, clang:codegen, clang:bounds-safety
**Closed Date:** 2025-12-10T23:18:11Z

## Body

I was exploring the similarities and differences between the clang and gcc implementations for "counted_by" attribute support and came across the following difference in behavior. 

In the specific example shown below gcc warns that index 2 is out of bounds for type int [*]. Clang does not detect this.

Is this intentional behavior or unexpected in clang? Looks like gcc intentionally covered this case since there is a specific test case for a negative counted_by field - see gcc/testsuite/gcc.dg/ubsan/flex-array-counted-by-bounds-3.c 

gcc ...

```
$ gcc -fsanitize=undefined counted-by-negative.c
$ ./a.out
counted-by-negative.c:13:8: runtime error: index 2 out of bounds for type 'int [*]'
```

clang ...

```
$ clang -fsanitize=undefined counted-by-negative.c
$ ./a.out
```


counted-by-negative.c case ...

```
#include <stdlib.h>

struct signed_struct {
  int b;
  int c[] __attribute__ ((counted_by (b)));
} *psigned_struct;

int main(int argc, char *argv[])
{
  struct signed_struct *ps = (struct signed_struct *) malloc(sizeof(struct signed_struct));
  ps->b = -3;
  ps->c[2] = 0;
  return 0;
}
```

## Comments

### Comment 1 - vabridgers

May I ask that @bwendling take a look at this and comment? Thank you! 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (vabridgers)

<details>
I was exploring the similarities and differences between the clang and gcc implementations for "counted_by" attribute support and came across the following difference in behavior. 

In the specific example shown below gcc warns that index 2 is out of bounds for type int [*]. Clang does not detect this.

Is this intentional behavior or unexpected in clang? Looks like gcc intentionally covered this case since there is a specific test case for a negative counted_by field - see gcc/testsuite/gcc.dg/ubsan/flex-array-counted-by-bounds-3.c 

gcc ...

```
$ gcc -fsanitize=undefined counted-by-negative.c
$ ./a.out
counted-by-negative.c:13:8: runtime error: index 2 out of bounds for type 'int [*]'
```

clang ...

```
$ clang -fsanitize=undefined counted-by-negative.c
$ ./a.out
```


counted-by-negative.c case ...

```
#include &lt;stdlib.h&gt;

struct signed_struct {
  int b;
  int c[] __attribute__ ((counted_by (b)));
} *psigned_struct;

int main(int argc, char *argv[])
{
  struct signed_struct *ps = (struct signed_struct *) malloc(sizeof(struct signed_struct));
  ps-&gt;b = -3;
  ps-&gt;c[2] = 0;
  return 0;
}
```
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (vabridgers)

<details>
I was exploring the similarities and differences between the clang and gcc implementations for "counted_by" attribute support and came across the following difference in behavior. 

In the specific example shown below gcc warns that index 2 is out of bounds for type int [*]. Clang does not detect this.

Is this intentional behavior or unexpected in clang? Looks like gcc intentionally covered this case since there is a specific test case for a negative counted_by field - see gcc/testsuite/gcc.dg/ubsan/flex-array-counted-by-bounds-3.c 

gcc ...

```
$ gcc -fsanitize=undefined counted-by-negative.c
$ ./a.out
counted-by-negative.c:13:8: runtime error: index 2 out of bounds for type 'int [*]'
```

clang ...

```
$ clang -fsanitize=undefined counted-by-negative.c
$ ./a.out
```


counted-by-negative.c case ...

```
#include &lt;stdlib.h&gt;

struct signed_struct {
  int b;
  int c[] __attribute__ ((counted_by (b)));
} *psigned_struct;

int main(int argc, char *argv[])
{
  struct signed_struct *ps = (struct signed_struct *) malloc(sizeof(struct signed_struct));
  ps-&gt;b = -3;
  ps-&gt;c[2] = 0;
  return 0;
}
```
</details>


---

### Comment 4 - bwendling

What command line are you using? I get this:

```
kiff ~/llvm/llvm-project (negative-counted-by) $ clang -O2 -Wall -fsanitize=array-bounds,object-size,local-bounds -fstrict-flex-arrays=3 ../counted-by-negative.c 
kiff ~/llvm/llvm-project (negative-counted-by) $ ./a.out
../counted-by-negative.c:12:3: runtime error: store to address 0x558824ee92ec with insufficient space for an object of type 'int'
0x558824ee92ec: note: pointer points here
  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  11 0d 02 00 00 00 00 00  00 00 00 00 00 00 00 00
              ^ 
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior ../counted-by-negative.c:12:3 
Illegal instruction        (core dumped) ./a.out
```


---

### Comment 5 - bwendling

Ah! Never mind. If I separate the allocation out it fails.

---

### Comment 6 - vabridgers

Thanks @bwendling for the prompt look and PR. Best!

---

