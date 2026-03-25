# [LV] wrf in SPEC2017 emits "Verification Error" on Grace when using llvmorg-20.1.0-rc1

**Author:** yus3710-fj
**URL:** https://github.com/llvm/llvm-project/issues/126836
**Status:** Closed
**Labels:** regression, miscompilation, vectorizers
**Closed Date:** 2025-04-16T14:02:35Z

## Body

* CPU2017.*.log
```
runcpu v6612 started at 2025-02-12 10:12:40 on "***"
runcpu is: /path/to/cpu2017-1.1.0/bin/harness/runcpu
runcpu: runcpu -I --action run -T base -c /path/to/config.cfg -i ref -n 1 --threads=1 --output_root=/path/to/output -o asc 521
toolset: linux-aarch64
suite: CPU2017 1.1.9
 :
Benchmarks selected: 521.wrf_r
Compiling Binaries
------------------------------------------------------------------------
 :
O: OPTIMIZE="-O2 -mcpu=neoverse-v2"
 :
flang version 20.1.0-rc1 (https://github.com/llvm/llvm-project.git af7f483a9d801252247b6c72e3763c1f55c5a506)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /path/to/bin
clang version 20.1.0-rc1 (https://github.com/llvm/llvm-project.git af7f483a9d801252247b6c72e3763c1f55c5a506)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /path/to/bin
 :
Setting Up Run Directories
  Setting up 521.wrf_r refrate (ref) base mytest-64 (1 copy): run_base_refrate_mytest-64.0000
-----------------------------------
Running Benchmarks
-----------------------------------
 :
Error with '/path/to/cpu2017-1.1.0/bin/specinvoke -d /path/to/output/benchspec/CPU/521.wrf_r/run/run_base_refrate_mytest-64.0000 -f compare.cmd -E -e compare.err -o compare.stdout'; no non-empty output files exist
  Command returned exit code 1
*** Miscompare of diffwrf_output_01.txt; for details see
    /path/to/output/benchspec/CPU/521.wrf_r/run/run_base_refrate_mytest-64.0000/diffwrf_output_01.txt.mis
0015:           W    PASSED
                W    121182    1   0.7117559909E-01   0.6272285812E-01   0   0.3873E-01   0.1085E+01
                  ^
0105:       RAINC    PASSED
            RAINC      1132    2   0.5266002302E+01   0.7384289461E+01   0   0.2787E+01   0.4166E+00
                  ^
0107:      RAINNC    PASSED
           RAINNC      4114    1   0.4236885721E+01   0.3284801810E+01   0   0.1728E+01   0.9835E+00
                  ^
0244:       RAINC    PASSED
            RAINC      1359    2   0.9852032964E+01   0.1287943984E+02   0   0.4379E+01   0.4672E+00
                  ^
0246:      RAINNC    PASSED
           RAINNC      4118    1   0.1017513524E+02   0.8983057159E+01   0   0.2827E+01   0.6375E+00
                  ^
0383:       RAINC    PASSED
            RAINC      1510    2   0.1283821850E+02   0.1616687672E+02   0   0.4926E+01   0.4845E+00
                  ^
Benchmark Times:
  Run Start:    2025-02-12 10:12:46 (1739322766)
  Rate Start:   2025-02-12 10:12:46 (1739322766.13335)
  Rate End:     2025-02-12 10:15:34 (1739322934.19951)
  Run Stop:     2025-02-12 10:15:34 (1739322934)
  Run Elapsed:  00:02:48 (168)
  Run Reported: 00:02:48 (168 66154003 168.066154)
 Error 521.wrf_r base refrate ratio=13.33, runtime=168.066154, copies=1, threads=1, power=0.00W, temp=0.00 degC, humidity=0.00%, errorcode=VE
Error: 1x521.wrf_r
 :
```
The verification is passed when the option `-mcpu=neoverse-v2` is removed.
(The option `-mcpu=neoverse-v2+nosve` doesn't work for this.)

## Comments

### Comment 1 - yus3710-fj

Roughly, the regression occured between f5a35a31bfe6 and 69d3ba3db922.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Yusuke MINATO (yus3710-fj)

<details>
* CPU2017.*.log
```
runcpu v6612 started at 2025-02-12 10:12:40 on "***"
runcpu is: /path/to/cpu2017-1.1.0/bin/harness/runcpu
runcpu: runcpu -I --action run -T base -c /path/to/config.cfg -i ref -n 1 --threads=1 --output_root=/path/to/output -o asc 521
toolset: linux-aarch64
suite: CPU2017 1.1.9
 :
Benchmarks selected: 521.wrf_r
Compiling Binaries
------------------------------------------------------------------------
 :
O: OPTIMIZE="-O2 -mcpu=neoverse-v2"
 :
flang version 20.1.0-rc1 (https://github.com/llvm/llvm-project.git af7f483a9d801252247b6c72e3763c1f55c5a506)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /path/to/bin
clang version 20.1.0-rc1 (https://github.com/llvm/llvm-project.git af7f483a9d801252247b6c72e3763c1f55c5a506)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /path/to/bin
 :
Setting Up Run Directories
  Setting up 521.wrf_r refrate (ref) base mytest-64 (1 copy): run_base_refrate_mytest-64.0000
-----------------------------------
Running Benchmarks
-----------------------------------
 :
Error with '/path/to/cpu2017-1.1.0/bin/specinvoke -d /path/to/output/benchspec/CPU/521.wrf_r/run/run_base_refrate_mytest-64.0000 -f compare.cmd -E -e compare.err -o compare.stdout'; no non-empty output files exist
  Command returned exit code 1
*** Miscompare of diffwrf_output_01.txt; for details see
    /path/to/output/benchspec/CPU/521.wrf_r/run/run_base_refrate_mytest-64.0000/diffwrf_output_01.txt.mis
0015:           W    PASSED
                W    121182    1   0.7117559909E-01   0.6272285812E-01   0   0.3873E-01   0.1085E+01
                  ^
0105:       RAINC    PASSED
            RAINC      1132    2   0.5266002302E+01   0.7384289461E+01   0   0.2787E+01   0.4166E+00
                  ^
0107:      RAINNC    PASSED
           RAINNC      4114    1   0.4236885721E+01   0.3284801810E+01   0   0.1728E+01   0.9835E+00
                  ^
0244:       RAINC    PASSED
            RAINC      1359    2   0.9852032964E+01   0.1287943984E+02   0   0.4379E+01   0.4672E+00
                  ^
0246:      RAINNC    PASSED
           RAINNC      4118    1   0.1017513524E+02   0.8983057159E+01   0   0.2827E+01   0.6375E+00
                  ^
0383:       RAINC    PASSED
            RAINC      1510    2   0.1283821850E+02   0.1616687672E+02   0   0.4926E+01   0.4845E+00
                  ^
Benchmark Times:
  Run Start:    2025-02-12 10:12:46 (1739322766)
  Rate Start:   2025-02-12 10:12:46 (1739322766.13335)
  Rate End:     2025-02-12 10:15:34 (1739322934.19951)
  Run Stop:     2025-02-12 10:15:34 (1739322934)
  Run Elapsed:  00:02:48 (168)
  Run Reported: 00:02:48 (168 66154003 168.066154)
 Error 521.wrf_r base refrate ratio=13.33, runtime=168.066154, copies=1, threads=1, power=0.00W, temp=0.00 degC, humidity=0.00%, errorcode=VE
Error: 1x521.wrf_r
 :
```
The verification is passed when the option `-mcpu=neoverse-v2` is removed.
</details>


---

### Comment 3 - yus3710-fj

The verification is also passed when the option `-mllvm -vectorize-loops=false` is specified.
FYI: Flang doesn't support the option `-fno-vectorize`.

---

### Comment 4 - fhahn

@yus3710-fj would you be able to isolate the loop/function that's causing the issue and share the input IR?

---

### Comment 5 - kiranchandramohan

> The verification is also passed when the option `-mllvm -vectorize-loops=false` is specified. FYI: Flang doesn't support the option `-fno-vectorize`.

There is a patch up for review for `-fno-vectorize` https://github.com/llvm/llvm-project/pull/119718.

Does it work correctly for `-O3` or `-Ofast`?

---

### Comment 6 - kawashima-fj

@yus3710-fj is a little busy. I (his colleague) will try identifying the cause commit and the affected file.

---

### Comment 7 - yus3710-fj

> Does it work correctly for `-O3` or `-Ofast`?

`-O3` and `-Ofast` also cause the error. I've not checked whether disabling vectorization works for them.

---

### Comment 8 - kawashima-fj

`git bisect` identified the commit 3397950f2d21 (@Mel-Chen) as the trigger of the regression. I don't know whether this commit is a root cause or just a trigger.

I'll identify the affected source file/loop.

---

### Comment 9 - kawashima-fj

The affected source file is `module_cu_kfeta.F90` (`module_cu_kfeta.fppized.f90`).

I built wrf_r with 3397950f2d21 and 41a94de75caa (one ahead of 3397950f2d21). 3397950f2d21 has the verification error and 41a94de75caa has not the verification error. When I linked `module_cu_kfeta.fppized.o` of 41a94de75caa and all other object files of 3397950f2d21, the verification error disappeared. When I linked `module_cu_kfeta.fppized.o` of 3397950f2d21 and all other object files of 41a94de75caa, the verification error appeared.


---

### Comment 10 - yus3710-fj

@fhahn I found that the first loop in `KF_eta_PARA` @ `module_cu_kfeta.F90` (`module_cu_kfeta.fppized.f90`) causes the error.

---

### Comment 11 - kawashima-fj

I don't know whether I can put a snippet of the SPEC CPU source code and its IR. [WRF code in GitHub](https://github.com/wrf-model/WRF/) has [the same loop](https://github.com/wrf-model/WRF/blob/master/phys/module_cu_kfeta.F#L761-L788) (the first loop in `KF_eta_PARA` @ `module_cu_kfeta.F90` (`module_cu_kfeta.fppized.f90`)). I compiled this soruce code using 41a94de75caacb979070ec7a010dfe3c4e9f116f and 3397950f2d21426c7520d114a12588128906a897, and compared the output IRs. I saw exactly the same difference of output IRs as that of SPEC CPU code. So you can investigate without SPEC CPU 2017.

You can compile the affected source file using the following procedures. `module_wrf_error.F` is needed to obtain the Fortran module (`module_wrf_error.mod`), which is used by `module_cu_kfeta.F`.

```shell
flang --target=aarch64-unknown-linux-gnu -ffree-form -c frame/module_wrf_error.F
flang --target=aarch64-unknown-linux-gnu -ffree-form -S -emit-llvm -I. -O2 -mcpu=neoverse-v2 phys/module_cu_kfeta.F
```

If you compare the output IR files, you'll see the following difference.

```diff
--- module_cu_kfeta.41a94de75caa.ll     2025-02-19 20:40:51.617355000 +0900
+++ module_cu_kfeta.3397950f2d21.ll     2025-02-19 20:41:25.116384000 +0900
@@ -3665,14 +3665,8 @@
 vec.epilog.ph:                                    ; preds = %vec.epilog.iter.check, %vector.main.loop.iter.check
   %vec.epilog.resume.val = phi i64 [ %n.vec, %vec.epilog.iter.check ], [ 0, %vector.main.loop.iter.check ]
   %bc.resume.val = phi i64 [ %230, %vec.epilog.iter.check ], [ 1, %vector.main.loop.iter.check ]
-  %bc.merge.rdx = phi i32 [ %302, %vec.epilog.iter.check ], [ undef, %vector.main.loop.iter.check ]
-  %bc.merge.rdx3729 = phi i32 [ %303, %vec.epilog.iter.check ], [ undef, %vector.main.loop.iter.check ]
   %n.vec3731 = and i64 %107, 2147483646
   %305 = or i64 %107, 1
-  %.splatinsert = insertelement <2 x i32> poison, i32 %bc.merge.rdx, i64 0
-  %.splat = shufflevector <2 x i32> %.splatinsert, <2 x i32> poison, <2 x i32> zeroinitializer
-  %.splatinsert3735 = insertelement <2 x i32> poison, i32 %bc.merge.rdx3729, i64 0
-  %.splat3736 = shufflevector <2 x i32> %.splatinsert3735, <2 x i32> poison, <2 x i32> zeroinitializer
   %306 = trunc nsw i64 %bc.resume.val to i32
   %.splatinsert3737 = insertelement <2 x i32> poison, i32 %306, i64 0
   %.splat3738 = shufflevector <2 x i32> %.splatinsert3737, <2 x i32> poison, <2 x i32> zeroinitializer
@@ -3695,8 +3689,8 @@
 
 vec.epilog.vector.body:                           ; preds = %vec.epilog.vector.body, %vec.epilog.ph
   %index3732 = phi i64 [ %vec.epilog.resume.val, %vec.epilog.ph ], [ %index.next3760, %vec.epilog.vector.body ]
-  %vec.phi3733 = phi <2 x i32> [ %.splat, %vec.epilog.ph ], [ %339, %vec.epilog.vector.body ]
-  %vec.phi3734 = phi <2 x i32> [ %.splat3736, %vec.epilog.ph ], [ %341, %vec.epilog.vector.body ]
+  %vec.phi3733 = phi <2 x i32> [ splat (i32 -2147483648), %vec.epilog.ph ], [ %339, %vec.epilog.vector.body ]
+  %vec.phi3734 = phi <2 x i32> [ splat (i32 -2147483648), %vec.epilog.ph ], [ %341, %vec.epilog.vector.body ]
   %vec.ind3739 = phi <2 x i32> [ %induction, %vec.epilog.ph ], [ %vec.ind.next3740, %vec.epilog.vector.body ]
   %offset.idx = or disjoint i64 %index3732, 1
   %307 = sub nsw i64 %offset.idx, %105
```

I confirmed using `-mllvm -print-before-all -mllvm -print-after-all`  that this difference first comes from LoopVectorizePass.

IR before the LoopVectorizePass is [before-loop-vectorize.txt](https://github.com/user-attachments/files/18867935/before-loop-vectorize.txt).

@fhahn @Mel-Chen Could you take a look? I'm not so familiar with LoopVectorizePass.

---

### Comment 12 - kiranchandramohan

@fhahn @Mel-Chen Are you working on this?

---

### Comment 13 - kawashima-fj

@Mel-Chen @fhahn I found the cause and made a small reproducer. I think this is a bug of 3397950f2d21426c7520d114a12588128906a897 (#120395).

Reproducer:

```c
#include <stdio.h>

__attribute__((noinline))
int foo(int n, float *a, float cmp) {
    int idx;  // uninitialized
    for (int i = 0; i < n; i++)
        if (a[i] > cmp)  // take only when i == 10
            idx = i;
    return idx;
}

int main() {
    float a[20] = {};
    a[10] = 3.0;
    int idx = foo(20, a, 2.0);
    printf("%d\n", idx);  // should print "10"
    return 0;
}
```

Run (on AArch64 Neoverse V2):

```console
$ clang -O0 -mcpu=neoverse-v2 test.c
$ ./a.out
10
$ clang -O2 -mcpu=neoverse-v2 test.c
$ ./a.out
-2147483648
```

In this case, the loop is vectorized with a main (VF=4, UF=4) loop and a epilogue (VF=4, UF=1) loop by the LoopVectorizePass. Just after the LoopVectorizePass, the IR of the preheader of the epilogue loop is the following. Comments after `;;` are added by me.

```llvm
vec.epilog.ph:                                    ; preds = %vec.epilog.iter.check, %vector.main.loop.iter.check
  %bc.resume.val = phi i64 [ %n.vec, %vec.epilog.iter.check ], [ 0, %vector.main.loop.iter.check ]
  ;; `idx` just before the epilogue loop; `%rdx.select` if the main loop is executed (`n >= 16`), otherwise `undef`
  %bc.merge.rdx = phi i32 [ %rdx.select, %vec.epilog.iter.check ], [ undef, %vector.main.loop.iter.check ]
  ;; `icmp` with `undef`; can be folded to `undef`
  %16 = icmp eq i32 %bc.merge.rdx, undef
  ;; `idx`; folded to `-2147483648` because `%16` can be `undef`
  %17 = select i1 %16, i32 -2147483648, i32 %bc.merge.rdx
  %n.mod.vf19 = urem i64 %wide.trip.count, 4
  %n.vec20 = sub i64 %wide.trip.count, %n.mod.vf19
  %.splatinsert = insertelement <4 x i32> poison, i32 %17, i64 0
  %.splat = shufflevector <4 x i32> %.splatinsert, <4 x i32> poison, <4 x i32> zeroinitializer
  %18 = trunc i64 %bc.resume.val to i32
  %.splatinsert23 = insertelement <4 x i32> poison, i32 %18, i64 0
  %.splat24 = shufflevector <4 x i32> %.splatinsert23, <4 x i32> poison, <4 x i32> zeroinitializer
  %induction = add <4 x i32> %.splat24, <i32 0, i32 1, i32 2, i32 3>
  %broadcast.splatinsert28 = insertelement <4 x float> poison, float %cmp, i64 0
  %broadcast.splat29 = shufflevector <4 x float> %broadcast.splatinsert28, <4 x float> poison, <4 x i32> zeroinitializer
  br label %vec.epilog.vector.body
```

If `idx` is updated in the main loop but not updated in the epilogue loop, the final `idx` becomes the sentinel value -2147483648. `%16` and `%17` were introduced by 3397950f2d21426c7520d114a12588128906a897 (#120395). I think this is the cause of this problem.

This bug still exists in both `main` and `release/20.x` branches. This bug probably affects other targets. `-mcpu=neoverse-v2` is required to select VF and UF. We want to fix this bug before the 20.1.0 release.


---

### Comment 14 - davemgreen

@fhahn @Mel-Chen FYI. Sometimes these issues do not send out notifications like they should.

---

### Comment 15 - kawashima-fj

A little additional information:

InstCombinePass run after LoopVectorizePass changes

```llvm
vec.epilog.vector.body:                           ; preds = %vec.epilog.vector.body, %vec.epilog.ph
  %index21 = phi i64 [ %bc.resume.val, %vec.epilog.ph ], [ %index.next30, %vec.epilog.vector.body ]
  %vec.phi22 = phi <4 x i32> [ %.splat, %vec.epilog.ph ], [ %23, %vec.epilog.vector.body ]
  %vec.ind25 = phi <4 x i32> [ %induction, %vec.epilog.ph ], [ %vec.ind.next26, %vec.epilog.vector.body ]
  ...
```

to


```llvm
vec.epilog.vector.body:                           ; preds = %vec.epilog.vector.body, %vec.epilog.ph
  %index21 = phi i64 [ %bc.resume.val, %vec.epilog.ph ], [ %index.next30, %vec.epilog.vector.body ]
  %vec.phi22 = phi <4 x i32> [ splat (i32 -2147483648), %vec.epilog.ph ], [ %17, %vec.epilog.vector.body ]
  %vec.ind25 = phi <4 x i32> [ %induction, %vec.epilog.ph ], [ %vec.ind.next26, %vec.epilog.vector.body ]
  ...
```

on the basis of

```llvm
vec.epilog.ph:                                    ; preds = %vec.epilog.iter.check, %vector.main.loop.iter.check
  ...
  %bc.merge.rdx = phi i32 [ %rdx.select, %vec.epilog.iter.check ], [ undef, %vector.main.loop.iter.check ]
  %16 = icmp eq i32 %bc.merge.rdx, undef
  %17 = select i1 %16, i32 -2147483648, i32 %bc.merge.rdx
  ...
```

If I replace `undef` with 0 in the `%16` above, I get the correct result.


---

### Comment 16 - fhahn

I'm taking a look, it seems like there are multiple places where we aren't undef/poison safe.

---

### Comment 17 - vzakhari

There is the same issue in NWChem app.  I have a reproducer generated from Fortran code, but it is essentially the same as in https://github.com/llvm/llvm-project/issues/126836#issuecomment-2687002109: a comparison with `undef` is generated by vectorizer, and then it is folded into `true` by `InstCombine`.



---

### Comment 18 - Mel-Chen

I apologize for seeing this message so late. I just saw this issue in the Vectorizer Improvements meeting notes. 

> I'm taking a look, it seems like there are multiple places where we aren't undef/poison safe.

@fhahn Have you started working on it? If not, I can take this issue.


---

### Comment 19 - fhahn

It looks like even without epilogue vec, the generated code isn't poison-safe: https://alive2.llvm.org/ce/z/4TiMZh. I'll prepare a patch for that to start with.

---

### Comment 20 - kkwli

We also encounter an issue in the `kf_eta_para` procedure. `opt-bisect-limit` also points to `LoopVectorizePass`. However, the behavior is seg fault on AIX and Linux/PowerPC.

```fortran
  IF(P0(K).GE.0.5*P0(1))L5=K
...
  WSPD(L5)=SQRT(U0(L5)*U0(L5)+V0(L5)*V0(L5))
```
Somehow `L5` acquires a negative (uninitialized?) value.

---

### Comment 21 - Mel-Chen

> It looks like even without epilogue vec, the generated code isn't poison-safe: https://alive2.llvm.org/ce/z/4TiMZh. I'll prepare a patch for that to start with.

@fhahn 
I found that the IR generated by trunk opt can pass the Alive2 test. After manually filtering, I discovered that adding the following declaration allows it to pass: https://alive2.llvm.org/ce/z/HpwW4o
```
declare i8 @llvm.vector.reduce.smax.v16i8(<16 x i8>) #0

attributes #0 = { memory(none) }
```
However, I'm still unclear about the effect of `memory(none)`.
That said, I'm also very concerned about the condition producing poison, as it would make AnyOf reduction poison-unsafe as well. https://alive2.llvm.org/ce/z/Gx5_iy

> We also encounter an issue in the `kf_eta_para` procedure. `opt-bisect-limit` also points to `LoopVectorizePass`. However, the behavior is seg fault on AIX and Linux/PowerPC.
> 
>   IF(P0(K).GE.0.5*P0(1))L5=K
> ...
>   WSPD(L5)=SQRT(U0(L5)*U0(L5)+V0(L5)*V0(L5))
> Somehow `L5` acquires a negative (uninitialized?) value.

@kkwli I’d like to know if all the current issues occur when epilogue vectorization is enabled. If they all stem from the start value being undef, then reverting to our initial fix for the resume value should be able to resolve this problem?https://github.com/llvm/llvm-project/pull/120395/commits/840b2950632baf26b2276dbafc3448da444504a3

---

### Comment 22 - kkwli

@Mel-Chen Did you mean to apply the change in https://github.com/llvm/llvm-project/commit/840b2950632baf26b2276dbafc3448da444504a3? I did it on top of 20.1.0, the seg fault is still there.

---

### Comment 23 - llvmbot


@llvm/issue-subscribers-bug

Author: Yusuke MINATO (yus3710-fj)

<details>
* CPU2017.*.log
```
runcpu v6612 started at 2025-02-12 10:12:40 on "***"
runcpu is: /path/to/cpu2017-1.1.0/bin/harness/runcpu
runcpu: runcpu -I --action run -T base -c /path/to/config.cfg -i ref -n 1 --threads=1 --output_root=/path/to/output -o asc 521
toolset: linux-aarch64
suite: CPU2017 1.1.9
 :
Benchmarks selected: 521.wrf_r
Compiling Binaries
------------------------------------------------------------------------
 :
O: OPTIMIZE="-O2 -mcpu=neoverse-v2"
 :
flang version 20.1.0-rc1 (https://github.com/llvm/llvm-project.git af7f483a9d801252247b6c72e3763c1f55c5a506)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /path/to/bin
clang version 20.1.0-rc1 (https://github.com/llvm/llvm-project.git af7f483a9d801252247b6c72e3763c1f55c5a506)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /path/to/bin
 :
Setting Up Run Directories
  Setting up 521.wrf_r refrate (ref) base mytest-64 (1 copy): run_base_refrate_mytest-64.0000
-----------------------------------
Running Benchmarks
-----------------------------------
 :
Error with '/path/to/cpu2017-1.1.0/bin/specinvoke -d /path/to/output/benchspec/CPU/521.wrf_r/run/run_base_refrate_mytest-64.0000 -f compare.cmd -E -e compare.err -o compare.stdout'; no non-empty output files exist
  Command returned exit code 1
*** Miscompare of diffwrf_output_01.txt; for details see
    /path/to/output/benchspec/CPU/521.wrf_r/run/run_base_refrate_mytest-64.0000/diffwrf_output_01.txt.mis
0015:           W    PASSED
                W    121182    1   0.7117559909E-01   0.6272285812E-01   0   0.3873E-01   0.1085E+01
                  ^
0105:       RAINC    PASSED
            RAINC      1132    2   0.5266002302E+01   0.7384289461E+01   0   0.2787E+01   0.4166E+00
                  ^
0107:      RAINNC    PASSED
           RAINNC      4114    1   0.4236885721E+01   0.3284801810E+01   0   0.1728E+01   0.9835E+00
                  ^
0244:       RAINC    PASSED
            RAINC      1359    2   0.9852032964E+01   0.1287943984E+02   0   0.4379E+01   0.4672E+00
                  ^
0246:      RAINNC    PASSED
           RAINNC      4118    1   0.1017513524E+02   0.8983057159E+01   0   0.2827E+01   0.6375E+00
                  ^
0383:       RAINC    PASSED
            RAINC      1510    2   0.1283821850E+02   0.1616687672E+02   0   0.4926E+01   0.4845E+00
                  ^
Benchmark Times:
  Run Start:    2025-02-12 10:12:46 (1739322766)
  Rate Start:   2025-02-12 10:12:46 (1739322766.13335)
  Rate End:     2025-02-12 10:15:34 (1739322934.19951)
  Run Stop:     2025-02-12 10:15:34 (1739322934)
  Run Elapsed:  00:02:48 (168)
  Run Reported: 00:02:48 (168 66154003 168.066154)
 Error 521.wrf_r base refrate ratio=13.33, runtime=168.066154, copies=1, threads=1, power=0.00W, temp=0.00 degC, humidity=0.00%, errorcode=VE
Error: 1x521.wrf_r
 :
```
The verification is passed when the option `-mcpu=neoverse-v2` is removed.
(The option `-mcpu=neoverse-v2+nosve` doesn't work for this.)
</details>


---

### Comment 24 - fhahn

I think we need to freeze the start value and use the frozen value consistently if the start value may be undef: https://github.com/llvm/llvm-project/pull/132691

---

### Comment 25 - kkwli

I verify that #132691 fixes the problem we encountered on AIX and Linux on PPC. Thanks.

---

### Comment 26 - vzakhari

Sorry for the ignorant question in advance.  How does this work with FP values?  If the `undef` value happens to be `NaN`, then do we care that the comparison will return false (regardless of whether the initial value was updated or not by the vector loop)?

---

### Comment 27 - david-arm

Hi @fhahn, is this actually fixed yet? I thought that https://github.com/llvm/llvm-project/pull/132691 was the fix, but that hasn't landed yet.

---

### Comment 28 - fhahn

Github was over-eager with closing, https://github.com/llvm/llvm-project/pull/132691 depends on 2 other PRs which need to be merged first, one of them mentioned that this will enable fixing https://github.com/llvm/llvm-project/issues/126836

---

### Comment 29 - david-arm

> Github was over-eager with closing, [#132691](https://github.com/llvm/llvm-project/pull/132691) depends on 2 other PRs which need to be merged first, one of them mentioned that this will enable fixing [#126836](https://github.com/llvm/llvm-project/issues/126836)

Yeah I thought that might be what happened. It's a bit annoying! Perhaps in future we need to write out the defect in letters to avoid github's over-eagerness, i.e. "Fixes one two six eight three six". :)

---

### Comment 30 - DavidSpickett

I think this got closed accidentally again, is that right @smithp35 ?
(not even sure how it was able to do that)


---

