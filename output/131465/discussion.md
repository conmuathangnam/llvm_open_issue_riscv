# [clang] Miscompilation at -O2/3

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/131465
**Status:** Closed
**Labels:** miscompilation, llvm:SCEV
**Closed Date:** 2025-03-17T05:59:18Z

## Body

This code prints 80 at `-O2/3` and 0 at `-O0/1`:

```c
int printf(const char *, ...);
int a, b;
void c(char d) { a = d; }
int main() {
  int e = 82, f = 20;
  for (; 85 + 20 + e - 187 + f; f = 65535 + 20 + e - 65637)
    if (b)
      e++;
  c(e >> 24);
  printf("%X\n", a);
}
```

Compiler Explorer: https://godbolt.org/z/6WP1aT6ba

Bisected to https://github.com/llvm/llvm-project/commit/04cd06906288dcb148de37d7c3e6c009c3e3b726, which was committed by @preames 

## Comments

### Comment 1 - dtcxzyw

Reduced reproducer: https://godbolt.org/z/bvhdjdMc7 https://alive2.llvm.org/ce/z/sSLNUX
```
; bin/opt -passes=loop-unroll test.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main(i1 %tobool2.not) #0 {
entry:
  %inc = zext i1 %tobool2.not to i32
  br label %for.body

for.body:                                         ; preds = %for.body, %entry
  %e.08 = phi i32 [ 2, %entry ], [ %spec.select, %for.body ]
  %spec.select = add nsw i32 %e.08, %inc
  %tobool.not = icmp eq i32 %spec.select, 2
  br i1 %tobool.not, label %for.end, label %for.body, !llvm.loop !0

for.end:                                          ; preds = %for.body
  ret i32 0
}

attributes #0 = { "target-cpu"="x86-64" }

!0 = distinct !{!0, !1}
!1 = !{!"llvm.loop.mustprogress"}

```

---

### Comment 2 - antoniofrighetto

@dtcxzyw May I ask whether llubi was used to get such a reduced reproducer? Having a hard time reducing this one, this time: am using [test.sh](https://github.com/dtcxzyw/llvm-ub-aware-interpreter?tab=readme-ov-file#automatic-ub-free-test-case-reduction-for-middle-end-miscompilation-bugs) with:
1) Custom pipelines & O2 in order to have some proper interestingness test;
2) `--ir-passes` with the default one, fine-tuning the pipeline by adjusting/removing passes.

However, by the time I obtain the reduced IR, it looks already invalid (i.e., unreachable). Any suggestions / idea what I may be missing?

---

### Comment 3 - dtcxzyw

1. Manually bisect
```
#!/usr/bin/bash

FILE=$1
BISECT=$2
../../LLVM/llvm-build/bin/clang -O3 -mllvm -inline-threshold=1000000 $FILE -mllvm -opt-bisect-limit=$BISECT -DNDEBUG -g0 -w -emit-llvm -S -I/home/dtcxzyw/WorkSpace/Projects/compilers/csmith/install/include -o test.ll -mllvm -print-changed -mllvm -print-module-scope 2>log
# ../../LLVM/llvm-build/bin/lli test.ll
./llubi --max-steps 1000000 test.ll
# ../../LLVM/llvm-build/bin/opt -O3 $FILE -o out.ll -S -opt-bisect-limit=$BISECT -print-changed -print-module-scope 2>log
# ./llubi out.ll
```
```
# Correct one
./bisect.sh test.c 175
# Miscompilation
./bisect.sh test.c 174
```
After the bisection, we got a single-pass reproducer:
```
; bin/opt -passes=loop-unroll test.ll -S
; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@a = dso_local local_unnamed_addr global i32 0, align 4
@b = dso_local local_unnamed_addr global i32 0, align 4
@.str = private unnamed_addr constant [4 x i8] c"%X\0A\00", align 1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(write, argmem: none, inaccessiblemem: none) uwtable
define dso_local void @c(i8 noundef signext %d) local_unnamed_addr #0 {
entry:
  %conv = sext i8 %d to i32
  store i32 %conv, ptr @a, align 4, !tbaa !5
  ret void
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #1 {
entry:
  %0 = load i32, ptr @b, align 4, !tbaa !5
  %tobool2.not = icmp ne i32 %0, 0
  %inc = zext i1 %tobool2.not to i32
  br label %for.body

for.body:                                         ; preds = %entry, %for.body
  %e.08 = phi i32 [ 82, %entry ], [ %spec.select, %for.body ]
  %spec.select = add nuw nsw i32 %e.08, %inc
  %sub = add nsw i32 %spec.select, -82
  %add1 = sub nsw i32 82, %spec.select
  %tobool.not = icmp eq i32 %sub, %add1
  br i1 %tobool.not, label %for.end, label %for.body, !llvm.loop !9

for.end:                                          ; preds = %for.body
  %shr = lshr i32 %spec.select, 24
  store i32 %shr, ptr @a, align 4, !tbaa !5
  %call = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef %shr)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(write, argmem: none, inaccessiblemem: none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 21.0.0git"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !10}
!10 = !{!"llvm.loop.mustprogress"}
```
2. reduce
```
#!/usr/bin/bash
# /home/dtcxzyw/WorkSpace/Projects/compilers/LLVM/llvm-build/bin/clang -Os -Xclang -disable-llvm-passes -emit-llvm -S test.c -o test.ll -I/home/dtcxzyw/WorkSpace/Projects/compilers/csmith/install/include -w
# llvm-reduce --test=ubi_reduce.sh --ir-passes="function(sroa,instcombine<no-verify-fixpoint>,gvn,simplifycfg,infer-address-spaces),inline" test.ll

a=$(/home/dtcxzyw/WorkSpace/Projects/compilers/llvm-ub-aware-interpreter/build/llubi --max-steps 1000000 --reduce-mode $1)
if [ $? -ne 0 ]; then
    exit 1
fi
/home/dtcxzyw/WorkSpace/Projects/compilers/LLVM/llvm-build/bin/opt -passes=loop-unroll $1 -o $1.tmp -S
if [ $? -ne 0 ]; then
    exit 1
fi
b=$(/home/dtcxzyw/WorkSpace/Projects/compilers/llvm-ub-aware-interpreter/build/llubi --max-steps 1000000 --reduce-mode $1.tmp)
if [ $? -ne 0 ]; then
    exit 0
    # exit 1
fi
if [[ "$a" == "$b" ]]; then
    exit 2
fi
# echo $a
# echo $b
exit 0
```
```
llvm-reduce --test=ubi_reduce.sh test.ll
```
Then we got the final reproducer.

In general, I will run `creduce w/ sanitizers` and the bisection first to avoid falling into some long-outstanding undef bugs.

BTW, an automatic reproducer reduction pipeline will be available soon :)


---

### Comment 4 - antoniofrighetto

@dtcxzyw Thanks a lot for clarifying that :)

---

