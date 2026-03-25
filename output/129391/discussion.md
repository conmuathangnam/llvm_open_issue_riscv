# compare-fp-4.c fails with -Os

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/129391
**Status:** Closed
**Labels:** backend:Hexagon, miscompilation, release:backport
**Closed Date:** 2025-03-14T04:09:21Z

## Body

`compare-fp-4.c` from [llvm-test-suite](https://github.com/androm3da/llvm-test-suite/blob/b73ae5c1e7e72a7a616cd6f789e13d29e848a5fd/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c) fails with -Os (but passes with -O2).

Test case reduced to 
```
$ cat ../llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c
extern void abort (void);

#define TEST(c) if ((c) != ok) abort ();
#define ORD(a, b) (((a) < (b)) || (a) >= (b))
#define UNEQ(a, b) (!LTGT ((a), (b)))
#define UNLT(a, b) (((a) < (b)) || __builtin_isunordered ((a), (b)))
#define UNGT(a, b) (((a) > (b)) || __builtin_isunordered ((a), (b)))
#define LTGT(a, b) (((a) < (b)) || (a) > (b))

float pinf;
float ninf;
float NaN;

int iltgt (float x, float y, int ok) {
  TEST (!UNEQ (x, y));
  TEST (LTGT (x, y));
  TEST (ORD (x, y) && (UNLT (x, y) || UNGT (x,y)));
}

int main () {
  pinf = __builtin_inf ();
  ninf = -__builtin_inf ();
  NaN = __builtin_nan ("");

  iltgt (ninf, pinf, 1);
  iltgt (NaN, NaN, 0);

  return 0;
}

```

```
$ ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/hexagon-unknown-linux-musl-clang -o fp4 ../llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c -Wno-implicit-function-declaration  -Os -DNDEBUG
$ ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/qemu-hexagon -L ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/target/hexagon-unknown-linux-musl/usr/ ./fp4 ; echo $?
Aborted (core dumped)
134
$ /opt/clang+llvm-18.1.0-rc1-cross-hexagon-unknown-linux-musl/x86_64-linux-musl/bin/hexagon-unknown-linux-musl-clang -o fp4 ../llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c -Wno-implicit-function-declaration  -O2 -DNDEBUG
../llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c:28:1: warning: non-void function does not return a value in all control paths [-Wreturn-type]
   28 | }
      | ^
1 warning generated.

$ ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/qemu-hexagon -L ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/target/hexagon-unknown-linux-musl/usr/ ./fp4 ; echo $?
0

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
`compare-fp-4.c` from [llvm-test-suite](https://github.com/androm3da/llvm-test-suite/blob/b73ae5c1e7e72a7a616cd6f789e13d29e848a5fd/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c) fails with -Os (but passes with -O2).

Test case reduced to 
```
$ cat ../llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c
extern void abort (void);

#define TEST(c) if ((c) != ok) abort ();
#define ORD(a, b) (((a) &lt; (b)) || (a) &gt;= (b))
#define UNEQ(a, b) (!LTGT ((a), (b)))
#define UNLT(a, b) (((a) &lt; (b)) || __builtin_isunordered ((a), (b)))
#define UNGT(a, b) (((a) &gt; (b)) || __builtin_isunordered ((a), (b)))
#define LTGT(a, b) (((a) &lt; (b)) || (a) &gt; (b))

float pinf;
float ninf;
float NaN;

int iltgt (float x, float y, int ok) {
  TEST (!UNEQ (x, y));
  TEST (LTGT (x, y));
  TEST (ORD (x, y) &amp;&amp; (UNLT (x, y) || UNGT (x,y)));
}

int main () {
  pinf = __builtin_inf ();
  ninf = -__builtin_inf ();
  NaN = __builtin_nan ("");

  iltgt (ninf, pinf, 1);
  iltgt (NaN, NaN, 0);

  return 0;
}

```

```
$ ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/hexagon-unknown-linux-musl-clang -o fp4 ../llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c -Wno-implicit-function-declaration  -Os -DNDEBUG
$ ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/qemu-hexagon -L ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/target/hexagon-unknown-linux-musl/usr/ ./fp4 ; echo $?
Aborted (core dumped)
134
$ /opt/clang+llvm-18.1.0-rc1-cross-hexagon-unknown-linux-musl/x86_64-linux-musl/bin/hexagon-unknown-linux-musl-clang -o fp4 ../llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c -Wno-implicit-function-declaration  -O2 -DNDEBUG
../llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/ieee/compare-fp-4.c:28:1: warning: non-void function does not return a value in all control paths [-Wreturn-type]
   28 | }
      | ^
1 warning generated.

$ ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/qemu-hexagon -L ../clang+llvm-21.0.0-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/target/hexagon-unknown-linux-musl/usr/ ./fp4 ; echo $?
0

```
</details>


---

### Comment 2 - androm3da

After patching above to unconditionally `return -1;` from `iltgt()`:

```
$ /local/mnt/workspace/install/clang+llvm-19.1.5-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/hexagon-unknown-linux-musl-clang -o fp4_O2.bc -c ./compare-fp-4.c -O2  -emit-llvm 
$ /local/mnt/workspace/install/clang+llvm-19.1.5-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/llvm-dis fp4_O2.bc 
$ /local/mnt/workspace/install/clang+llvm-19.1.5-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/hexagon-unknown-linux-musl-clang -o fp4_Os.bc -c ./compare-fp-4.c -Os -static -emit-llvm 
$ /local/mnt/workspace/install/clang+llvm-19.1.5-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/llvm-dis fp4_Os.bc 

```

[fp4_O2.ll.txt](https://github.com/user-attachments/files/19117999/fp4_O2.ll.txt)
[fp4_Os.ll.txt](https://github.com/user-attachments/files/19118000/fp4_Os.ll.txt)

---

### Comment 3 - androm3da

/cherry-pick d642eec78fc94ef3c5266dc0b10b8c51ea046e7a

---

### Comment 4 - llvmbot

/pull-request llvm/llvm-project#131270

---

