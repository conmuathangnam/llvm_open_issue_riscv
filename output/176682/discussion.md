# Miscompilation at -O3 with profiled data using PGO

**Author:** helloqirun
**URL:** https://github.com/llvm/llvm-project/issues/176682
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2026-02-12T09:58:31Z

## Body

Reproducible on a recent commit: af98aadc63637ac674d4

```
$ clang-trunk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git af98aadc63637ac674d403c77ccba42b05a3d11c)
```

I have reduced the testcase. The profiling data is produced by

`$ llvm-profdata merge prof.txt -o prof.profdata`

Correctly compiles without PGO.

```
$ clang-trunk -O3 -msse4.2 aa-831991.c ; ./a.out
1

```

Miscompiles with PGO enabled.
```
$ clang-trunk -O3 -msse4.2 -fprofile-instr-use=prof.profdata   aa-831991.c ; ./a.out
0
```

The testcase as well as the profiling data (in txt):

```c
$ cat aa-831991.c
int printf(const char *, ...);
short a, e;
int b = 1, aj;
char f;
unsigned c(char g) {
  b &= 0 >= g | 90;
  return 0;
}
short d(int g) { return a; }
static unsigned char func_28() {
  e = 0;
  for (; e != -22; --e)
    c(d((((e <= (0 > (((8 > e) > 3) > 5) || 1)) < 3) > 5 == f) >= e || 3) <
      e) &&
        7 && (5 || 5 > (7 || 1)) < 0;
  return 1;
}
int main() {
  for (; aj < 6; aj++)
    func_28();
  printf("%X\n", b);
}
```

```
$ cat prof.txt
a
1000537922509932317
67
2
3
2
9
3
9
9
2
1
9
3
8
1
4
0
8
7
8
7
9
1
0
0
8
0
0
7
8
6
3
1
7
1
2
6
4
5
3
5
9
5
9
5
7
1
5
2
4
0
7
6
0
1
7
7
2
4
6
8
1
0
8
4
3
5
0
2
:
70444406143875
57
1
4
9
5
7
1
1
3
4
2
5
0
4
1
7
1
7
2
9
4
8
9
3
9
6
6
6
9
5
1
0
4
6
2
7
3
1
7
2
5
9
9
5
8
4
4
5
1
0
5
5
5
3
0
4
1
4
aa-831991.c:func_28
413345032826402189
9
1
8
5
5
5
9
5
807
601
```



## Comments

### Comment 1 - boomanaiden154

```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@b = global i32 1
@a = global i16 0
@.str = constant [4 x i8] c"%X\0A\00"

define i32 @main() {
entry:
  %call = call i8 @func_28()
  %0 = load i32, ptr @b, align 4
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %0)
  ret i32 %call1
}

declare i32 @printf(ptr, ...)

define i8 @func_28() #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.body, %entry
  %0 = phi i16 [ 0, %entry ], [ %dec, %for.body ]
  %cmp.not = icmp eq i16 %0, -22
  br i1 %cmp.not, label %for.end, label %for.body

for.body:                                         ; preds = %for.cond
  %1 = load i16, ptr @a, align 2
  %cmp29 = icmp sge i16 %1, %0
  %2 = load i32, ptr @b, align 4
  %conv1.i = zext i1 %cmp29 to i32
  %or.i = or i32 %conv1.i, 90
  %and.i = and i32 %2, %or.i
  store i32 %and.i, ptr @b, align 4
  %dec = add i16 %0, -1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret i8 0
}

attributes #0 = { "target-features"="+sse4.1" }
```

Should be a more minimal reproducer. Given after the reduction all the PGO information gets dropped, I'm not sure this is actually PGO specific.

---

### Comment 2 - boomanaiden154

Seems to be a fairly recent regression this is fine in Clang 21.1.0 (https://godbolt.org/z/bvs3PTEWG).

---

### Comment 3 - boomanaiden154

Bisects to 782759b757f69c6be98bd8079bc57e8f49b7ec86. CC @arsenm 

---

### Comment 4 - boomanaiden154

There's a difference post SelectionDAG with that patch applied versus without it. Looks like we end up using the result of an additional and after a subtraction instead of just the subtraction in quite a few places.
```patch
--- /tmp/good.s 2026-01-19 06:05:35.166978781 +0000
+++ /tmp/bad.s  2026-01-19 06:04:54.046700892 +0000
@@ -1,66 +1,66 @@
 body:             |
   bb.0.entry:
     %0:gr64 = MOV64rm $rip, 1, $noreg, target-flags(x86-gotpcrel) @a, $noreg :: (load (s64) from got)
     %1:gr32 = MOVZX32rm16 killed %0, 1, $noreg, 0, $noreg :: (dereferenceable load (s16) from @a)
     %2:gr16 = COPY %1.sub_16bit
     %3:gr64 = MOV64rm $rip, 1, $noreg, target-flags(x86-gotpcrel) @b, $noreg :: (load (s64) from got)
     %4:vr128 = MOVDI2PDIrr %1
     %5:vr128 = PSHUFLWri killed %4, 0
     %6:vr128 = PSHUFDri %5, 68
     %7:vr128 = PCMPGTWrm %6, $rip, 1, $noreg, %const.0, $noreg :: (load (s128) from constant-pool)
     %8:vr128 = PCMPGTWrm %6, $rip, 1, $noreg, %const.1, $noreg :: (load (s128) from constant-pool)
     %9:vr128 = PACKSSWBrr %8, killed %7
     %10:vr128 = PCMPGTWrm %5, $rip, 1, $noreg, %const.2, $noreg :: (load (s128) from constant-pool)
     %11:vr128 = PACKSSWBrr %10, %10
     %12:vr128 = PANDrm %11, $rip, 1, $noreg, %const.3, $noreg :: (load (s128) from constant-pool)
     %13:vr128 = MOVAPSrm $rip, 1, $noreg, %const.4, $noreg :: (load (s128) from constant-pool)
     %14:vr128 = PSUBBrr %13, killed %9
-    %15:gr16 = SUB16ri %2, -20, implicit-def $eflags
-    %16:gr8 = SETCCr 13, implicit $eflags
-    %17:gr32 = MOVZX32rr8 killed %16
-    %18:gr32 = ADD32ri_DB %17, 90, implicit-def dead $eflags
-    %19:gr16 = SUB16ri %2, -21, implicit-def $eflags
-    %20:gr8 = SETCCr 13, implicit $eflags
-    %21:gr32 = MOVZX32rr8 killed %20
-    %22:gr32 = disjoint ADD32ri_DB %21, 90, implicit-def dead $eflags
-    %23:vr128 = PORrm %12, $rip, 1, $noreg, %const.5, $noreg :: (load (s128) from constant-pool)
-    %24:vr128 = PANDrr %14, killed %23
+    %15:vr128 = PORrm %12, $rip, 1, $noreg, %const.5, $noreg :: (load (s128) from constant-pool)
+    %16:gr16 = SUB16ri %2, -20, implicit-def $eflags
+    %17:gr8 = SETCCr 13, implicit $eflags
+    %18:gr32 = MOVZX32rr8 killed %17
+    %19:gr32 = ADD32ri_DB %18, 90, implicit-def dead $eflags
+    %20:gr16 = SUB16ri %2, -21, implicit-def $eflags
+    %21:gr8 = SETCCr 13, implicit $eflags
+    %22:gr32 = MOVZX32rr8 killed %21
+    %23:gr32 = disjoint ADD32ri_DB %22, 90, implicit-def dead $eflags
+    %24:vr128 = PANDrr %14, killed %15
     %25:gr32 = PEXTRBrri %24, 1
     %26:gr32 = MOVPDI2DIrr %24
     %27:gr32 = AND32rr %26, killed %25, implicit-def dead $eflags
     %28:gr32 = PEXTRBrri %24, 2
     %29:gr32 = AND32rr %27, killed %28, implicit-def dead $eflags
     %30:gr32 = PEXTRBrri %24, 3
     %31:gr32 = AND32rr %29, killed %30, implicit-def dead $eflags
-    %32:gr32 = PEXTRBrri %14, 4
+    %32:gr32 = PEXTRBrri %24, 4
     %33:gr32 = AND32rr %31, killed %32, implicit-def dead $eflags
-    %34:gr32 = PEXTRBrri %14, 5
+    %34:gr32 = PEXTRBrri %24, 5
     %35:gr32 = AND32rr %33, killed %34, implicit-def dead $eflags
-    %36:gr32 = PEXTRBrri %14, 6
+    %36:gr32 = PEXTRBrri %24, 6
     %37:gr32 = AND32rr %35, killed %36, implicit-def dead $eflags
-    %38:gr32 = PEXTRBrri %14, 7
+    %38:gr32 = PEXTRBrri %24, 7
     %39:gr32 = AND32rr %37, killed %38, implicit-def dead $eflags
-    %40:gr32 = PEXTRBrri %14, 8
+    %40:gr32 = PEXTRBrri %24, 8
     %41:gr32 = AND32rr %39, killed %40, implicit-def dead $eflags
-    %42:gr32 = PEXTRBrri %14, 9
+    %42:gr32 = PEXTRBrri %24, 9
     %43:gr32 = AND32rr %41, killed %42, implicit-def dead $eflags
-    %44:gr32 = PEXTRBrri %14, 10
+    %44:gr32 = PEXTRBrri %24, 10
     %45:gr32 = AND32rr %43, killed %44, implicit-def dead $eflags
-    %46:gr32 = PEXTRBrri %14, 11
+    %46:gr32 = PEXTRBrri %24, 11
     %47:gr32 = AND32rr %45, killed %46, implicit-def dead $eflags
-    %48:gr32 = PEXTRBrri %14, 12
+    %48:gr32 = PEXTRBrri %24, 12
     %49:gr32 = AND32rr %47, killed %48, implicit-def dead $eflags
-    %50:gr32 = PEXTRBrri %14, 13
+    %50:gr32 = PEXTRBrri %24, 13
     %51:gr32 = AND32rr %49, killed %50, implicit-def dead $eflags
-    %52:gr32 = PEXTRBrri %14, 14
+    %52:gr32 = PEXTRBrri %24, 14
     %53:gr32 = AND32rr %51, killed %52, implicit-def dead $eflags
-    %54:gr32 = PEXTRBrri %14, 15
+    %54:gr32 = PEXTRBrri %24, 15
     %55:gr32 = AND32rr %53, killed %54, implicit-def dead $eflags
-    %56:gr32 = AND32rr %18, killed %55, implicit-def dead $eflags
-    %57:gr32 = AND32rm %22, %3, 1, $noreg, 0, $noreg, implicit-def dead $eflags :: (dereferenceable load (s32) from @b)
+    %56:gr32 = AND32rr %19, killed %55, implicit-def dead $eflags
+    %57:gr32 = AND32rm %23, %3, 1, $noreg, 0, $noreg, implicit-def dead $eflags :: (dereferenceable load (s32) from @b)
     %58:gr32 = AND32rr %56, killed %57, implicit-def dead $eflags
     MOV32mr %3, 1, $noreg, 0, $noreg, killed %58 :: (store (s32) into @b)
     %59:gr32 = MOV32r0 implicit-def dead $eflags
     %60:gr8 = COPY %59.sub_8bit
     $al = COPY %60
     RET 0, $al
```

---

### Comment 5 - boomanaiden154

@RKSimon Are you able to take a look given this looks like a SelectionDAG issue?

---

### Comment 6 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Qirun Zhang (helloqirun)

<details>
Reproducible on a recent commit: af98aadc63637ac674d4

```
$ clang-trunk -v
clang version 22.0.0git (https://github.com/llvm/llvm-project.git af98aadc63637ac674d403c77ccba42b05a3d11c)
```

I have reduced the testcase. The profiling data is produced by

`$ llvm-profdata merge prof.txt -o prof.profdata`

Correctly compiles without PGO.

```
$ clang-trunk -O3 -msse4.2 aa-831991.c ; ./a.out
1

```

Miscompiles with PGO enabled.
```
$ clang-trunk -O3 -msse4.2 -fprofile-instr-use=prof.profdata   aa-831991.c ; ./a.out
0
```

The testcase as well as the profiling data (in txt):

```
$ cat aa-831991.c
int printf(const char *, ...);
short a, e;
int b = 1, aj;
char f;
unsigned c(char g) {
  b &amp;= 0 &gt;= g | 90;
  return 0;
}
short d(int g) { return a; }
static unsigned char func_28() {
  e = 0;
  for (; e != -22; --e)
    c(d((((e &lt;= (0 &gt; (((8 &gt; e) &gt; 3) &gt; 5) || 1)) &lt; 3) &gt; 5 == f) &gt;= e || 3) &lt;
      e) &amp;&amp;
        7 &amp;&amp; (5 || 5 &gt; (7 || 1)) &lt; 0;
  return 1;
}
int main() {
  for (; aj &lt; 6; aj++)
    func_28();
  printf("%X\n", b);
}
```

```
$ cat prof.txt
a
1000537922509932317
67
2
3
2
9
3
9
9
2
1
9
3
8
1
4
0
8
7
8
7
9
1
0
0
8
0
0
7
8
6
3
1
7
1
2
6
4
5
3
5
9
5
9
5
7
1
5
2
4
0
7
6
0
1
7
7
2
4
6
8
1
0
8
4
3
5
0
2
:
70444406143875
57
1
4
9
5
7
1
1
3
4
2
5
0
4
1
7
1
7
2
9
4
8
9
3
9
6
6
6
9
5
1
0
4
6
2
7
3
1
7
2
5
9
9
5
8
4
4
5
1
0
5
5
5
3
0
4
1
4
aa-831991.c:func_28
413345032826402189
9
1
8
5
5
5
9
5
807
601
```


</details>


---

