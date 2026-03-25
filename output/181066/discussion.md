# UNREACHABLE in LegalizeTypes.cpp

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/181066
**Status:** Closed
**Labels:** invalid, backend:Hexagon, crash
**Closed Date:** 2026-02-12T02:49:33Z

## Body

While building llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/usad-run.c

```
Unprocessed value in a map! SplitVectors
UNREACHABLE executed at /local/mnt/workspace/bcain-20260211_133912/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:183!
...
      Running pass 'Hexagon DAG->DAG Pattern Instruction Selection' on function '@bar'
```

**LLVM Ref:** `release/22.x` (`75fb34ab4f460ab01b6562520f10070226d41da7`)
**Crash Script:** `usad-run-64e7c6.sh`
**Crash Source:** `usad-run-64e7c6.c`

## Minimal Reproducer Command

```bash
/local/mnt/workspace/bcain-20260211_133912/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +hvx-length128b -vectorize-slp -x c /local/mnt/workspace/bcain-reduce-20260211_154112/case-usad-run-64e7c6/usad-run-64e7c6.c -o /dev/null
```

## Reduced Test Case

Reduced file: `reduced.ll` (1016 bytes)

```llvm
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define i32 @bar(<16 x i8> %0, <16 x i8> %1, <16 x i8> %2) {
entry:
  %3 = zext <16 x i8> %0 to <16 x i32>
  %4 = zext <16 x i8> %1 to <16 x i32>
  %5 = zext <16 x i8> %2 to <16 x i32>
  %6 = tail call i32 @llvm.vector.reduce.add.v16i32(<16 x i32> %5)
  %7 = tail call i32 @llvm.vector.reduce.add.v16i32(<16 x i32> %4)
  %8 = tail call i32 @llvm.vector.reduce.add.v16i32(<16 x i32> %3)
  %op.rdx.1 = or i32 %7, %8
  %op.rdx.3 = or i32 %6, %op.rdx.1
  ret i32 %op.rdx.3
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.add.v16i32(<16 x i32>) #0

; uselistorder directives
uselistorder ptr @llvm.vector.reduce.add.v16i32, { 2, 1, 0 }

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
While building llvm-test-suite/SingleSource/Regression/C/gcc-c-torture/execute/usad-run.c

```
Unprocessed value in a map! SplitVectors
UNREACHABLE executed at /local/mnt/workspace/bcain-20260211_133912/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp:183!
...
      Running pass 'Hexagon DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->bar'
```

**LLVM Ref:** `release/22.x` (`75fb34ab4f460ab01b6562520f10070226d41da7`)
**Crash Script:** `usad-run-64e7c6.sh`
**Crash Source:** `usad-run-64e7c6.c`

## Minimal Reproducer Command

```bash
/local/mnt/workspace/bcain-20260211_133912/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +hvx-length128b -vectorize-slp -x c /local/mnt/workspace/bcain-reduce-20260211_154112/case-usad-run-64e7c6/usad-run-64e7c6.c -o /dev/null
```

## Reduced Test Case

Reduced file: `reduced.ll` (1016 bytes)

```
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define i32 @<!-- -->bar(&lt;16 x i8&gt; %0, &lt;16 x i8&gt; %1, &lt;16 x i8&gt; %2) {
entry:
  %3 = zext &lt;16 x i8&gt; %0 to &lt;16 x i32&gt;
  %4 = zext &lt;16 x i8&gt; %1 to &lt;16 x i32&gt;
  %5 = zext &lt;16 x i8&gt; %2 to &lt;16 x i32&gt;
  %6 = tail call i32 @<!-- -->llvm.vector.reduce.add.v16i32(&lt;16 x i32&gt; %5)
  %7 = tail call i32 @<!-- -->llvm.vector.reduce.add.v16i32(&lt;16 x i32&gt; %4)
  %8 = tail call i32 @<!-- -->llvm.vector.reduce.add.v16i32(&lt;16 x i32&gt; %3)
  %op.rdx.1 = or i32 %7, %8
  %op.rdx.3 = or i32 %6, %op.rdx.1
  ret i32 %op.rdx.3
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @<!-- -->llvm.vector.reduce.add.v16i32(&lt;16 x i32&gt;) #<!-- -->0

; uselistorder directives
uselistorder ptr @<!-- -->llvm.vector.reduce.add.v16i32, { 2, 1, 0 }

attributes #<!-- -->0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

```

</details>


---

### Comment 2 - androm3da

Tsk - apologies! I used an automated test reducer and I had misplaced faith in it and this issue is not reproducible as reported.

---

