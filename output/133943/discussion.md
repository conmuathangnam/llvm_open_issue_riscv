# [RISC-V] Miscompile on rv64gcv with -O[23]

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/133943
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-04-15T14:24:24Z

## Body

Testcase:
```c
_Bool a;
long long q;
char f;
char g;
char k[18];
int l[18];
int main() {
  for (int i = 0; i < 18; ++i)
    k[i] = l[i] = 7;
  for (signed i = 0; i < g + 18; i += 4) {
    int o = (_Bool)(l[i] ? k[i] : f / l[i]);
    a = a < o ? a : o;
  }
  q = a;
  __builtin_printf("%llu\n", q);
}
```

Commands:
```
# riscv
$ ./bin/clang -march=rv64gcv_zvl256b -flto -O3 ./red.c -o user-config.out
$ QEMU_CPU=rv64,vlen=256,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 ./bin/qemu-riscv64 user-config.out 1
1

$ ./bin/clang -march=rv64gcv_zvl256b -flto -O2 ./red.c -o user-config.out
$ QEMU_CPU=rv64,vlen=256,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 ./bin/qemu-riscv64 user-config.out 1
1

$ ./bin/clang -march=rv64gcv_zvl256b -flto -O1 ./red.c -o user-config.out
$ QEMU_CPU=rv64,vlen=256,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 ./bin/qemu-riscv64 user-config.out 1
0

# x86
$ ./native.out
0
```

Bisected to f044564db1cbc588d0cad4f953d38f6c787dadd4 as the first bad commit

Found via fuzzer



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Edwin Lu (ewlu)

<details>
Testcase:
```c
_Bool a;
long long q;
char f;
char g;
char k[18];
int l[18];
int main() {
  for (int i = 0; i &lt; 18; ++i)
    k[i] = l[i] = 7;
  for (signed i = 0; i &lt; g + 18; i += 4) {
    int o = (_Bool)(l[i] ? k[i] : f / l[i]);
    a = a &lt; o ? a : o;
  }
  q = a;
  __builtin_printf("%llu\n", q);
}
```

Commands:
```
# riscv
$ ./bin/clang -march=rv64gcv_zvl256b -flto -O3 ./red.c -o user-config.out
$ QEMU_CPU=rv64,vlen=256,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 ./bin/qemu-riscv64 user-config.out 1
1

$ ./bin/clang -march=rv64gcv_zvl256b -flto -O2 ./red.c -o user-config.out
$ QEMU_CPU=rv64,vlen=256,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 ./bin/qemu-riscv64 user-config.out 1
1

$ ./bin/clang -march=rv64gcv_zvl256b -flto -O1 ./red.c -o user-config.out
$ QEMU_CPU=rv64,vlen=256,rvv_ta_all_1s=true,rvv_ma_all_1s=true,v=true,vext_spec=v1.0,zve32f=true,zve64f=true timeout --verbose -k 0.1 4 ./bin/qemu-riscv64 user-config.out 1
0

# x86
$ ./native.out
0
```

Bisected to f044564db1cbc588d0cad4f953d38f6c787dadd4 as the first bad commit

Found via fuzzer


</details>


---

### Comment 2 - dtcxzyw

Reproduced. This is a codegen bug.


---

### Comment 3 - dtcxzyw

Reproducer:
```
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

@l = global [18 x i32] zeroinitializer
@k = global [18 x i8] zeroinitializer
@g = global i8 0
@a = global i8 0
@.str = constant [6 x i8] c"%llu\0A\00"

define i32 @main() #0 {
entry:
  tail call void @llvm.memset.p0.i64(ptr @k, i8 1, i64 18, i1 false)
  store <16 x i32> splat (i32 1), ptr @l, align 64
  store i32 1, ptr getelementptr inbounds nuw (i8, ptr @l, i64 64), align 64
  %0 = load i8, ptr @g, align 1
  %1 = or i8 %0, 18
  %add = zext i8 %1 to i64
  %a.promoted = load i8, ptr @a, align 1
  br label %for.body8

for.cond.cleanup7:                                ; preds = %cond.end
  store i8 %storedv4344, ptr @a, align 1
  %loadedv32 = zext i1 %tobool28 to i64
  %call = tail call i32 (ptr, ...) @printf(ptr @.str, i64 %loadedv32)
  ret i32 0

for.body8:                                        ; preds = %cond.end, %entry
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %cond.end ]
  %storedv4344 = phi i8 [ %a.promoted, %entry ], [ %storedv, %cond.end ]
  %arrayidx10 = getelementptr [18 x i32], ptr @l, i64 0, i64 %indvars.iv
  %2 = load i32, ptr %arrayidx10, align 16
  %tobool.not = icmp eq i32 %2, 0
  br i1 %tobool.not, label %cond.end, label %cond.true

cond.true:                                        ; preds = %for.body8
  %arrayidx12 = getelementptr [18 x i8], ptr @k, i64 0, i64 %indvars.iv
  %3 = load i8, ptr %arrayidx12, align 1
  %4 = icmp ne i8 %3, 0
  %5 = zext i1 %4 to i32
  br label %cond.end

cond.end:                                         ; preds = %cond.true, %for.body8
  %cond = phi i32 [ %5, %cond.true ], [ poison, %for.body8 ]
  %loadedv = zext i8 %storedv4344 to i32
  %cond27 = tail call i32 @llvm.smin.i32(i32 %cond, i32 %loadedv)
  %tobool28 = icmp ne i32 %cond27, 0
  %storedv = zext i1 %tobool28 to i8
  %indvars.iv.next = add i64 %indvars.iv, 4
  %cmp5 = icmp ult i64 %indvars.iv.next, %add
  br i1 %cmp5, label %for.body8, label %for.cond.cleanup7
}

declare i32 @printf(ptr, ...)

attributes #0 = { "target-features"="+64bit,+a,+c,+d,+f,+m,+v" }

!llvm.module.flags = !{!0}

!0 = !{i32 1, !"target-abi", !"lp64d"}
```

---

### Comment 4 - dtcxzyw

Further reduced:
```
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

@l = global [18 x i32] zeroinitializer
@k = global [18 x i8] zeroinitializer
@g = global i8 0
@a = global i8 0

define i32 @main() #0 {
entry:
  store <18 x i8> splat (i8 1), ptr @k, align 1
  store <16 x i32> splat (i32 1), ptr @l, align 4
  %0 = load i8, ptr @g, align 1
  %1 = or i8 %0, 18
  %add = zext i8 %1 to i64
  %a.promoted = load i8, ptr @a, align 1
  br label %for.body8

for.cond.cleanup7:                                ; preds = %cond.end
  store i8 %storedv4344, ptr @a, align 1
  ret i32 %loadedv

for.body8:                                        ; preds = %cond.end, %entry
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %cond.end ]
  %storedv4344 = phi i8 [ %a.promoted, %entry ], [ %storedv, %cond.end ]
  %arrayidx10 = getelementptr [18 x i32], ptr @l, i64 0, i64 %indvars.iv
  %2 = load i32, ptr %arrayidx10, align 16
  %tobool.not = icmp eq i32 %2, 0
  br i1 %tobool.not, label %cond.end, label %cond.true

cond.true:                                        ; preds = %for.body8
  %arrayidx12 = getelementptr [18 x i8], ptr @k, i64 0, i64 %indvars.iv
  %3 = load i8, ptr %arrayidx12, align 1
  %4 = icmp ne i8 %3, 0
  %5 = zext i1 %4 to i32
  br label %cond.end

cond.end:                                         ; preds = %cond.true, %for.body8
  %cond = phi i32 [ %5, %cond.true ], [ poison, %for.body8 ]
  %loadedv = zext i8 %storedv4344 to i32
  %cond27 = tail call i32 @llvm.smin.i32(i32 %cond, i32 %loadedv)
  %tobool28 = icmp ne i32 %cond27, 0
  %storedv = zext i1 %tobool28 to i8
  %indvars.iv.next = add i64 %indvars.iv, 4
  %cmp5 = icmp ult i64 %indvars.iv.next, %add
  br i1 %cmp5, label %for.body8, label %for.cond.cleanup7
}

attributes #0 = { "target-features"="+64bit,+a,+c,+d,+f,+m,+v" }

!llvm.module.flags = !{!0}
!0 = !{i32 1, !"target-abi", !"lp64d"}
```

---

### Comment 5 - topperc

Is it SelectionDAG or another optimization?

---

### Comment 6 - wangpc-pp

> Is it SelectionDAG or another optimization?

I was going to take a look but I was occupied by other stuffs. One weirdness I have noticed is that we are using the <16 x i32> vector for the second store of splat 1. This doesn't match the source code I think. So I suggest to look into the LV or SLP, where the vectors are generated.

---

### Comment 7 - wangpc-pp

> > Is it SelectionDAG or another optimization?
> 
> I was going to take a look but I was occupied by other stuffs. One weirdness I have noticed is that we are using the <16 x i32> vector for the second store of splat 1. This doesn't match the source code I think. So I suggest to look into the LV or SLP, where the vectors are generated.

I think this weirdness is because of reduction, the vector part is OK I think. I don't have time to take another look, please feel free to continue the work.

---

### Comment 8 - mshockwave

I think this is caused by SLP vectorizer in the LTO pipeline (non-LTO configuration won't trigger this bug). This is the IR right before going into SLP:
``` llvm
; *** IR Dump Before SLPVectorizerPass on main ***
; Function Attrs: nofree nounwind uwtable vscale_range(4,1024)
define dso_local noundef signext i32 @main() local_unnamed_addr #0 {
entry:
  tail call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(18) @k, i8 7, i64 18, i1 false), !tbaa !11
  store <16 x i32> splat (i32 7), ptr @l, align 64, !tbaa !14
  store i32 7, ptr getelementptr inbounds nuw (i8, ptr @l, i64 64), align 64, !tbaa !14
  store i32 7, ptr getelementptr inbounds nuw (i8, ptr @l, i64 68), align 4, !tbaa !14
  %a.promoted = load i8, ptr @a, align 1, !tbaa !16
  %0 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 4), align 1
  %1 = icmp ne i8 %0, 0
  %2 = zext i1 %1 to i32
  %3 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 8), align 1
  %4 = icmp ne i8 %3, 0
  %5 = zext i1 %4 to i32
  %6 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 12), align 1
  %7 = icmp ne i8 %6, 0
  %8 = zext i1 %7 to i32
  %9 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 16), align 1
  %10 = icmp ne i8 %9, 0
  %11 = zext i1 %10 to i32
  %tobool28 = icmp ne i8 %a.promoted, 0
  %loadedv.1 = zext i1 %tobool28 to i32
  %cond27.1 = tail call i32 @llvm.smin.i32(i32 %2, i32 %loadedv.1)
  %cond27.2 = tail call i32 @llvm.smin.i32(i32 %5, i32 %cond27.1)
  %cond27.3 = tail call i32 @llvm.smin.i32(i32 %8, i32 %cond27.2)
  %cond27.4 = tail call i32 @llvm.smin.i32(i32 %11, i32 %cond27.3)
  %storedv.4 = trunc nuw nsw i32 %cond27.4 to i8
  store i8 %storedv.4, ptr @a, align 1, !tbaa !16
  %loadedv32 = zext nneg i32 %cond27.4 to i64
  %call = tail call signext i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %loadedv32) #4
  ret i32 0
}
```
The keys here are those `smin` lines:
``` llvm
  %0 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 4), align 1
  %1 = icmp ne i8 %0, 0
  %2 = zext i1 %1 to i32
  %3 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 8), align 1
  %4 = icmp ne i8 %3, 0
  %5 = zext i1 %4 to i32
  %6 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 12), align 1
  %7 = icmp ne i8 %6, 0
  %8 = zext i1 %7 to i32
  %9 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 16), align 1
  %10 = icmp ne i8 %9, 0
  %11 = zext i1 %10 to i32
  %tobool28 = icmp ne i8 %a.promoted, 0
  %loadedv.1 = zext i1 %tobool28 to i32
  %cond27.1 = tail call i32 @llvm.smin.i32(i32 %2, i32 %loadedv.1)
  %cond27.2 = tail call i32 @llvm.smin.i32(i32 %5, i32 %cond27.1)
  %cond27.3 = tail call i32 @llvm.smin.i32(i32 %8, i32 %cond27.2)
  %cond27.4 = tail call i32 @llvm.smin.i32(i32 %11, i32 %cond27.3)
```
They're effectively doing a reduction on a bunch of 0 & 1 _i32_ integers generated by the icmp, so if any of those comparisons is a zero, `%cond27.4` -- the final reduced value -- would be zero.

However, after SLP:
``` llvm
; *** IR Dump After SLPVectorizerPass on main ***
; Function Attrs: nofree nounwind uwtable vscale_range(4,1024)
define dso_local noundef signext i32 @main() local_unnamed_addr #0 {
entry:
  tail call void @llvm.memset.p0.i64(ptr noundef nonnull align 1 dereferenceable(18) @k, i8 7, i64 18, i1 false), !tbaa !11
  store <16 x i32> splat (i32 7), ptr @l, align 64, !tbaa !14
  store i32 7, ptr getelementptr inbounds nuw (i8, ptr @l, i64 64), align 64, !tbaa !14
  store i32 7, ptr getelementptr inbounds nuw (i8, ptr @l, i64 68), align 4, !tbaa !14
  %a.promoted = load i8, ptr @a, align 1, !tbaa !16
  %0 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 4), align 1
  %1 = load i8, ptr getelementptr inbounds nuw (i8, ptr @k, i64 16), align 1
  %2 = icmp ne i8 %1, 0
  %3 = zext i1 %2 to i32
  %4 = call <2 x i8> @llvm.experimental.vp.strided.load.v2i8.p0.i64(ptr align 1 getelementptr inbounds nuw (i8, ptr @k, i64 8), i64 4, <2 x i1> splat (i1 true), i32 2)
  %5 = insertelement <4 x i8> poison, i8 %0, i32 0
  %6 = insertelement <4 x i8> %5, i8 %a.promoted, i32 1
  %7 = call <4 x i8> @llvm.vector.insert.v4i8.v2i8(<4 x i8> %6, <2 x i8> %4, i64 2)
  %8 = icmp ne <4 x i8> %7, zeroinitializer
  %9 = call i1 @llvm.vector.reduce.smin.v4i1(<4 x i1> %8)
  %10 = zext i1 %9 to i32
  %11 = call i32 @llvm.smin.i32(i32 %10, i32 %3)
  %storedv.4 = trunc nuw nsw i32 %11 to i8
  store i8 %storedv.4, ptr @a, align 1, !tbaa !16
  %loadedv32 = zext nneg i32 %11 to i64
  %call = tail call signext i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i64 noundef %loadedv32) #6
  ret i32 0
}
```
The vectorized code of the aforementioned area looks like this now:
``` llvm
  %8 = icmp ne <4 x i8> %7, zeroinitializer
  %9 = call i1 @llvm.vector.reduce.smin.v4i1(<4 x i1> %8)
  %10 = zext i1 %9 to i32
  %11 = call i32 @llvm.smin.i32(i32 %10, i32 %3)
```
While it looks legit, noted that `smin` reduction on a bunch of _i1_ 0 & 1 will give you 1 if any of those values is a 1 (because the only bit in i1 is the signed bit. In fact, SelectionDAG will simply emit a `vectorreduce_or` if it sees a `vectorreduce_smin` with i1 vector), even if there is a zero. Which doesn't match the behavior of the scalar code.

As a PoC, I manually changed the vectorized code into
``` llvm
  %8 = icmp ne <4 x i8> %7, zeroinitializer
  %9 = call i1 @llvm.vector.reduce.umin.v4i1(<4 x i1> %8)
  %10 = zext i1 %9 to i32
  %11 = call i32 @llvm.smin.i32(i32 %10, i32 %3)
```
and it fixed the issue (this is just a PoC, I don't think using umin should be the solution, we should probably use `vector.reduce.and` instead)

I tried to fix this in SLP but it seem like we have to look into the candidate tree, which is something I'm not really confident to do without breaking everything else. @alexey-bataev could you help on this item?

---

### Comment 9 - alexey-bataev

Yep, looks like another issue for minbitwidth analysis, when unsigned input value is used in signed min

---

