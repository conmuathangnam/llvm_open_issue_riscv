# [SLPVectorizer] Miscompilation at O3

**Author:** dtcxzyw
**URL:** https://github.com/llvm/llvm-project/issues/129057
**Status:** Closed
**Labels:** miscompilation, llvm:SLPVectorizer
**Closed Date:** 2025-02-27T16:41:54Z

## Body

Reproducer:
```
; bin/opt -passes=slp-vectorizer reduced.ll -S
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@f = global i32 zeroinitializer
@.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
entry:
  store i32 152, ptr @f, align 4
  %agg.tmp.sroa.0.0.copyload.i = load i32, ptr @f, align 4
  %add.i.i = shl i32 %agg.tmp.sroa.0.0.copyload.i, 24
  %sext.i = add i32 %add.i.i, 83886080
  %conv2.i = ashr i32 %sext.i, 24
  %and.i = and i32 %conv2.i, 66440127
  %cmp3.i.i = icmp ugt i32 %and.i, 33554431
  %shl.i.i = select i1 %cmp3.i.i, i32 0, i32 6
  %cond.i.i = shl i32 %and.i, %shl.i.i
  %0 = trunc i32 %cond.i.i to i8
  %sext.1.i = add i32 0, 83886080
  %conv2.1.i = ashr i32 %sext.1.i, 24
  %and.1.i = and i32 %conv2.1.i, 1
  %cmp3.i.1.i = icmp ugt i32 0, 0
  %shl.i.1.i = select i1 %cmp3.i.1.i, i32 0, i32 0
  %cond.i.1.i = shl i32 %and.1.i, %shl.i.1.i
  %1 = trunc i32 %cond.i.1.i to i8
  %conv17.1.i = and i8 %0, %1
  %sext.2.i = add i32 0, 83886080
  %conv2.2.i = ashr i32 %sext.2.i, 24
  %and.2.i = and i32 %conv2.2.i, 1
  %shl.i.2.i = select i1 false, i32 0, i32 0
  %cond.i.2.i = shl i32 %and.2.i, %shl.i.2.i
  %2 = trunc i32 %cond.i.2.i to i8
  %conv17.2.i = and i8 %conv17.1.i, %2
  %sext.3.i = add i32 0, 83886080
  %conv2.3.i = ashr i32 %sext.3.i, 24
  %and.3.i = and i32 %conv2.3.i, 1
  %shl.i.3.i = select i1 false, i32 0, i32 0
  %cond.i.3.i = shl i32 %and.3.i, %shl.i.3.i
  %3 = trunc i32 %cond.i.3.i to i8
  %conv17.3.i = and i8 %conv17.2.i, %3
  %sext.4.i = add i32 0, 83886080
  %conv2.4.i = ashr i32 %sext.4.i, 24
  %and.4.i = and i32 %conv2.4.i, 1
  %shl.i.4.i = select i1 false, i32 0, i32 0
  %cond.i.4.i = shl i32 %and.4.i, %shl.i.4.i
  %4 = trunc i32 %cond.i.4.i to i8
  %conv17.4.i = and i8 %conv17.3.i, %4
  %sext.5.i = add i32 0, 83886080
  %conv2.5.i = ashr i32 %sext.5.i, 24
  %and.5.i = and i32 %conv2.5.i, 1
  %shl.i.5.i = select i1 false, i32 0, i32 0
  %cond.i.5.i = shl i32 %and.5.i, %shl.i.5.i
  %5 = trunc i32 %cond.i.5.i to i8
  %conv17.5.i = and i8 %conv17.4.i, %5
  %sext.6.i = add i32 0, 83886080
  %conv2.6.i = ashr i32 %sext.6.i, 24
  %and.6.i = and i32 %conv2.6.i, 1
  %shl.i.6.i = select i1 false, i32 0, i32 0
  %cond.i.6.i = shl i32 %and.6.i, %shl.i.6.i
  %6 = trunc i32 %cond.i.6.i to i8
  %conv17.6.i = and i8 %conv17.5.i, %6
  %sext.7.i = add i32 0, 83886080
  %conv2.7.i = ashr i32 %sext.7.i, 24
  %and.7.i = and i32 %conv2.7.i, 1
  %shl.i.7.i = select i1 false, i32 0, i32 0
  %cond.i.7.i = shl i32 %and.7.i, %shl.i.7.i
  %7 = trunc i32 %cond.i.7.i to i8
  %conv17.7.i = and i8 %conv17.6.i, %7
  %conv = zext i8 %conv17.7.i to i32
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %conv)
  ret i32 0
}

declare i32 @printf(ptr, ...)
```
```
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@f = global i32 0
@.str = constant [4 x i8] c"%d\0A\00"

define i32 @main() {
entry:
  store i32 152, ptr @f, align 4
  %agg.tmp.sroa.0.0.copyload.i = load i32, ptr @f, align 4
  %add.i.i = shl i32 %agg.tmp.sroa.0.0.copyload.i, 24
  %0 = insertelement <8 x i32> <i32 poison, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080>, i32 %add.i.i, i32 0
  %1 = add <8 x i32> <i32 83886080, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0>, %0
  %2 = ashr <8 x i32> %1, splat (i32 24)
  %3 = trunc <8 x i32> %2 to <8 x i8>
  %4 = and <8 x i8> %3, <i8 -65, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1>
  %5 = zext <8 x i8> %4 to <8 x i32>
  %6 = shufflevector <8 x i32> %5, <8 x i32> poison, <2 x i32> <i32 0, i32 poison>
  %7 = shufflevector <2 x i32> %6, <2 x i32> <i32 poison, i32 0>, <2 x i32> <i32 0, i32 3>
  %8 = icmp ugt <2 x i32> %7, <i32 33554431, i32 0>
  %9 = call <8 x i1> @llvm.vector.insert.v8i1.v2i1(<8 x i1> <i1 poison, i1 poison, i1 false, i1 false, i1 false, i1 false, i1 false, i1 false>, <2 x i1> %8, i64 0)
  %10 = select <8 x i1> %9, <8 x i8> zeroinitializer, <8 x i8> <i8 6, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0>
  %11 = shl <8 x i8> %4, %10
  %12 = call i8 @llvm.vector.reduce.and.v8i8(<8 x i8> %11)
  %conv = zext i8 %12 to i32
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %conv)
  ret i32 0
}

declare i32 @printf(ptr, ...)

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <8 x i1> @llvm.vector.insert.v8i1.v2i1(<8 x i1>, <2 x i1>, i64 immarg) #0

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i8 @llvm.vector.reduce.and.v8i8(<8 x i8>) #0

attributes #0 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```
llubi output:
Before SLP:
```
Entering function main
  store i32 152, ptr @f, align 4
  %agg.tmp.sroa.0.0.copyload.i = load i32, ptr @f, align 4 -> i32 152
  %add.i.i = shl i32 %agg.tmp.sroa.0.0.copyload.i, 24 -> i32 -1744830464
  %sext.i = add i32 %add.i.i, 83886080 -> i32 -1660944384
  %conv2.i = ashr i32 %sext.i, 24 -> i32 -99
  %and.i = and i32 %conv2.i, 66440127 -> i32 66440093
  %cmp3.i.i = icmp ugt i32 %and.i, 33554431 -> T
  %shl.i.i = select i1 %cmp3.i.i, i32 0, i32 6 -> i32 0
  %cond.i.i = shl i32 %and.i, %shl.i.i -> i32 66440093
  %0 = trunc i32 %cond.i.i to i8 -> i8 -99
  %sext.1.i = add i32 0, 83886080 -> i32 83886080
  %conv2.1.i = ashr i32 %sext.1.i, 24 -> i32 5
  %and.1.i = and i32 %conv2.1.i, 1 -> i32 1
  %cmp3.i.1.i = icmp ugt i32 0, 0 -> F
  %shl.i.1.i = select i1 %cmp3.i.1.i, i32 0, i32 0 -> i32 0
  %cond.i.1.i = shl i32 %and.1.i, %shl.i.1.i -> i32 1
  %1 = trunc i32 %cond.i.1.i to i8 -> i8 1
  %conv17.1.i = and i8 %0, %1 -> i8 1
  %sext.2.i = add i32 0, 83886080 -> i32 83886080
  %conv2.2.i = ashr i32 %sext.2.i, 24 -> i32 5
  %and.2.i = and i32 %conv2.2.i, 1 -> i32 1
  %shl.i.2.i = select i1 false, i32 0, i32 0 -> i32 0
  %cond.i.2.i = shl i32 %and.2.i, %shl.i.2.i -> i32 1
  %2 = trunc i32 %cond.i.2.i to i8 -> i8 1
  %conv17.2.i = and i8 %conv17.1.i, %2 -> i8 1
  %sext.3.i = add i32 0, 83886080 -> i32 83886080
  %conv2.3.i = ashr i32 %sext.3.i, 24 -> i32 5
  %and.3.i = and i32 %conv2.3.i, 1 -> i32 1
  %shl.i.3.i = select i1 false, i32 0, i32 0 -> i32 0
  %cond.i.3.i = shl i32 %and.3.i, %shl.i.3.i -> i32 1
  %3 = trunc i32 %cond.i.3.i to i8 -> i8 1
  %conv17.3.i = and i8 %conv17.2.i, %3 -> i8 1
  %sext.4.i = add i32 0, 83886080 -> i32 83886080
  %conv2.4.i = ashr i32 %sext.4.i, 24 -> i32 5
  %and.4.i = and i32 %conv2.4.i, 1 -> i32 1
  %shl.i.4.i = select i1 false, i32 0, i32 0 -> i32 0
  %cond.i.4.i = shl i32 %and.4.i, %shl.i.4.i -> i32 1
  %4 = trunc i32 %cond.i.4.i to i8 -> i8 1
  %conv17.4.i = and i8 %conv17.3.i, %4 -> i8 1
  %sext.5.i = add i32 0, 83886080 -> i32 83886080
  %conv2.5.i = ashr i32 %sext.5.i, 24 -> i32 5
  %and.5.i = and i32 %conv2.5.i, 1 -> i32 1
  %shl.i.5.i = select i1 false, i32 0, i32 0 -> i32 0
  %cond.i.5.i = shl i32 %and.5.i, %shl.i.5.i -> i32 1
  %5 = trunc i32 %cond.i.5.i to i8 -> i8 1
  %conv17.5.i = and i8 %conv17.4.i, %5 -> i8 1
  %sext.6.i = add i32 0, 83886080 -> i32 83886080
  %conv2.6.i = ashr i32 %sext.6.i, 24 -> i32 5
  %and.6.i = and i32 %conv2.6.i, 1 -> i32 1
  %shl.i.6.i = select i1 false, i32 0, i32 0 -> i32 0
  %cond.i.6.i = shl i32 %and.6.i, %shl.i.6.i -> i32 1
  %6 = trunc i32 %cond.i.6.i to i8 -> i8 1
  %conv17.6.i = and i8 %conv17.5.i, %6 -> i8 1
  %sext.7.i = add i32 0, 83886080 -> i32 83886080
  %conv2.7.i = ashr i32 %sext.7.i, 24 -> i32 5
  %and.7.i = and i32 %conv2.7.i, 1 -> i32 1
  %shl.i.7.i = select i1 false, i32 0, i32 0 -> i32 0
  %cond.i.7.i = shl i32 %and.7.i, %shl.i.7.i -> i32 1
  %7 = trunc i32 %cond.i.7.i to i8 -> i8 1
  %conv17.7.i = and i8 %conv17.6.i, %7 -> i8 1
  %conv = zext i8 %conv17.7.i to i32 -> i32 1
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %conv)
    Printf: 1
   -> i32 2
  ret i32 0
Exiting function main
```
After SLP:
```
Entering function main
  store i32 152, ptr @f, align 4
  %agg.tmp.sroa.0.0.copyload.i = load i32, ptr @f, align 4 -> i32 152
  %add.i.i = shl i32 %agg.tmp.sroa.0.0.copyload.i, 24 -> i32 -1744830464
  %0 = insertelement <8 x i32> <i32 poison, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080>, i32 %add.i.i, i32 0 -> { i32 -1744830464, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080 }
  %1 = add <8 x i32> <i32 83886080, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0, i32 0>, %0 -> { i32 -1660944384, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080, i32 83886080 }
  %2 = ashr <8 x i32> %1, splat (i32 24) -> { i32 -99, i32 5, i32 5, i32 5, i32 5, i32 5, i32 5, i32 5 }
  %3 = trunc <8 x i32> %2 to <8 x i8> -> { i8 -99, i8 5, i8 5, i8 5, i8 5, i8 5, i8 5, i8 5 }
  %4 = and <8 x i8> %3, <i8 -65, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1> -> { i8 -99, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1 }
  %5 = zext <8 x i8> %4 to <8 x i32> -> { i32 157, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1, i32 1 }
  %6 = shufflevector <8 x i32> %5, <8 x i32> poison, <2 x i32> <i32 0, i32 poison> -> { i32 157, poison }
  %7 = shufflevector <2 x i32> %6, <2 x i32> <i32 poison, i32 0>, <2 x i32> <i32 0, i32 3> -> { i32 157, i32 0 }
  %8 = icmp ugt <2 x i32> %7, <i32 33554431, i32 0> -> { F, F }
  %9 = call <8 x i1> @llvm.vector.insert.v8i1.v2i1(<8 x i1> <i1 poison, i1 poison, i1 false, i1 false, i1 false, i1 false, i1 false, i1 false>, <2 x i1> %8, i64 0) -> { F, F, F, F, F, F, F, F }
  %10 = select <8 x i1> %9, <8 x i8> zeroinitializer, <8 x i8> <i8 6, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0> -> { i8 6, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0, i8 0 }
  %11 = shl <8 x i8> %4, %10 -> { i8 64, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1, i8 1 }
  %12 = call i8 @llvm.vector.reduce.and.v8i8(<8 x i8> %11) -> i8 0
  %conv = zext i8 %12 to i32 -> i32 0
  %call1 = call i32 (ptr, ...) @printf(ptr @.str, i32 %conv)
    Printf: 0
   -> i32 2
  ret i32 0
Exiting function main
```
llvm version: a88f4f1962b47aa8db49b8687a7f8b9097a3d13b

C reproducer:
```
#include "csmith.h"
struct d {
  uint8_t a;
  uint16_t b
} f;
static uint64_t c[] = {66440127};
uint64_t e;
uint8_t h = 5;
uint32_t g(uint8_t, struct d);
int32_t j() {
  int16_t i = 10;
  struct d k = {152};
  for (e = 0; e < 22; ++e) {
    int32_t l[0];
    struct d *m = &f;
    h &= safe_lshift_func_int32_t_s_u(
        safe_add_func_int8_t_s_s(g(0, *m = k), 5) & c[0],
        safe_add_func_int16_t_s_s(6, (k.a > k.b) < l));
  }
  return i;
}
uint32_t g(uint8_t, struct d n) { return n.a; }
int main() {
  j();
  printf("%d\n", h);
}
```
```
> gcc -w -I/usr/include/csmith -fsanitize=address,undefined test.c && ./a.out
5
> bin/clang -w -I/usr/include/csmith -O3 test.c && ./a.out
0
```


