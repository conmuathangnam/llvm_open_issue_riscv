# [hexagon] loop-3c fails with -Oz

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/133241
**Status:** Closed
**Labels:** backend:Hexagon, miscompilation, release:backport
**Closed Date:** 2025-04-14T18:49:08Z

## Body

This program (reduced slightly from llvm-test-suite's `SingleSource/Regression/C/gcc-c-torture/execute/loop-3c.c`) fails with -Oz but passes with -O0, -O2, -Os.

```
$ /local/mnt/workspace/install/clang+llvm-19.1.5-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/hexagon-unknown-linux-musl-clang -Oz -o loop3c -static ~/tmp/loop3c.c -Wno-deprecated-non-prototype -Wno-implicit-int -Wno-implicit-function-declaration
/usr2/bcain/tmp/loop3c.c:17:1: warning: non-void function does not return a value [-Wreturn-type]
   17 | }
      | ^
1 warning generated.
$ /local/mnt/workspace/install/clang+llvm-19.1.5-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/qemu-hexagon ./loop3c 
Aborted (core dumped)

```

 
```
#include <limits.h>

void * a[255];

f (m)
{
  int i;
  int sh = 0x100;
  i = m;
  do
    { 
      a[sh >>= 1] = ((unsigned)i << 3)  + (char*)a;
      i += 4;
    }
  while (i < INT_MAX/2 + 1 + 4 * 4);
}

main ()
{
  a[0x10] = 0;
  a[0x08] = 0;
  f (INT_MAX/2 + INT_MAX/4 + 2);
  if (a[0x10] || a[0x08])
    abort ();
}

```

* [loop-3c_Oz.ll.txt](https://github.com/user-attachments/files/19486209/loop-3c_Oz.ll.txt)
* [loop-3c_Os.ll.txt](https://github.com/user-attachments/files/19486210/loop-3c_Os.ll.txt)
* [loop-3c_O2.ll.txt](https://github.com/user-attachments/files/19486211/loop-3c_O2.ll.txt)



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
This program (reduced slightly from llvm-test-suite's `SingleSource/Regression/C/gcc-c-torture/execute/loop-3c.c`) fails with -Oz but passes with -O0, -O2, -Os.
 
```
#include &lt;limits.h&gt;

void * a[255];

f (m)
{
  int i;
  int sh = 0x100;
  i = m;
  do
    { 
      a[sh &gt;&gt;= 1] = ((unsigned)i &lt;&lt; 3)  + (char*)a;
      i += 4;
    }
  while (i &lt; INT_MAX/2 + 1 + 4 * 4);
}

main ()
{
  a[0x10] = 0;
  a[0x08] = 0;
  f (INT_MAX/2 + INT_MAX/4 + 2);
  if (a[0x10] || a[0x08])
    abort ();
}

```
</details>


---

### Comment 2 - androm3da

cc @aankit-quic 

---

### Comment 3 - aankit-ca

Looking into this

---

### Comment 4 - androm3da

/cherry-pick da8ce56c53fe6e34809ba0b310fa90257e230a89

---

### Comment 5 - llvmbot

/pull-request llvm/llvm-project#135657

---

