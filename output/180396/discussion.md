# [AArch64][Backend] Stack Overflow in AArch64 Backend on QNX

**Author:** yizhew-kk
**URL:** https://github.com/llvm/llvm-project/issues/180396
**Status:** Open
**Labels:** backend:AArch64, crash

## Body

Hi, I hit a `llc` crashes with SIGSEGV on QNX/aarch64 backend. Root cause: PR #172942 changed `AArch64Subtarget` from heap to stack allocation, causing stack overflow on platforms with smaller default stack sizes. (Reverting PR #172942's changes to `AArch64AsmPrinter.cpp` resolves the crash.)

## Reproducer

**test.ll**:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"

@g1 = internal global { i64, i64, i64, ptr, i64, ptr, i64, i64, ptr } undef
@g2 = internal constant [4 x i8] c"foo\00"
@g3 = internal constant [1 x { ptr, i64, i64, ptr, ptr, ptr, ptr, ptr }] [{ ptr, i64, i64, ptr, ptr, ptr, ptr, ptr } { ptr @g2, i64 0, i64 0, ptr null, ptr null, ptr @func1, ptr null, ptr null }]
@g4 = constant { i64, i64, i64, ptr, i64, ptr, i64, i64, ptr } { i64 2, i64 0, i64 0, ptr null, i64 64, ptr @g3, i64 1, i64 1, ptr @func2 }

define internal i32 @func1(ptr %0, ptr %1, ptr %2, ptr %3, ptr %4) {
  br i1 false, label %6, label %7
6:
  br label %7
7:
  %8 = phi i64 [ 8, %6 ], [ 0, %5 ]
  %9 = add i64 %8, 8
  %10 = urem i64 %9, 8
  %11 = icmp ugt i64 %10, 0
  br i1 %11, label %12, label %15
12:
  %13 = add i64 %9, 8
  %14 = sub i64 %13, %10
  br label %15
15:
  %16 = phi i64 [ %14, %12 ], [ %9, %7 ]
  %17 = add i64 %16, 8
  %18 = urem i64 %17, 4
  %19 = icmp ugt i64 %18, 0
  br i1 %19, label %20, label %21
20:
  br label %21
21:
  ret i32 0
}

define i32 @func2(ptr %0, ptr %1, i32 %2, ptr %3) {
  %5 = alloca i32, align 4
  %6 = icmp eq i32 %2, 0
  br i1 %6, label %7, label %8
7:
  ret i32 1
8:
  %9 = getelementptr { i64, ptr, ptr, ptr, ptr, ptr, ptr }, ptr %0, i32 0, i32 6
  %10 = load ptr, ptr %9, align 8
  %11 = call i32 %10(ptr %1, i32 16, ptr %5)
  %12 = load { i64, i64, i64, ptr, i64, ptr, i64, i64, ptr }, ptr @g4, align 8
  store { i64, i64, i64, ptr, i64, ptr, i64, i64, ptr } %12, ptr @g1, align 8
  store i64 0, ptr getelementptr inbounds nuw (i8, ptr @g1, i64 16), align 8
  store ptr @g1, ptr %3, align 8
  ret i32 0
}

!llvm.module.flags = !{!0}
!0 = !{i32 2, !"Debug Info Version", i32 3}
```

```bash
# On QNX/aarch64:
./llc test.ll -filetype=obj -o test.o
# SIGSEGV
```

## Crash Analysis

```
(gdb) bt
#0  AArch64AsmPrinter::emitHwasanMemaccessSymbols(llvm::Module&)
#1  AArch64AsmPrinter::emitEndOfAsmFile(llvm::Module&)
#2  llvm::AsmPrinter::doFinalization(llvm::Module&)

(gdb) x/i $pc
=> stp x29, x30, [sp, #64]   ; crash at function prologue

(gdb) x/gx $sp
Cannot access memory at address 0x...   ; stack overflow
```

My internal fork tracks upstream main (around mid-Dec 2025). If this issue has been already fixed please point me the fix commit. Thanks!


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Yizhe Wang (yizhew-kk)

<details>
Hi, I hit a `llc` crashes with SIGSEGV on QNX/aarch64 backend. Root cause: PR #<!-- -->172942 changed `AArch64Subtarget` from heap to stack allocation, causing stack overflow on platforms with smaller default stack sizes. (Reverting PR #<!-- -->172942's changes to `AArch64AsmPrinter.cpp` resolves the crash.)

## Reproducer

**test.ll**:
```llvm
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"

@<!-- -->g1 = internal global { i64, i64, i64, ptr, i64, ptr, i64, i64, ptr } undef
@<!-- -->g2 = internal constant [4 x i8] c"foo\00"
@<!-- -->g3 = internal constant [1 x { ptr, i64, i64, ptr, ptr, ptr, ptr, ptr }] [{ ptr, i64, i64, ptr, ptr, ptr, ptr, ptr } { ptr @<!-- -->g2, i64 0, i64 0, ptr null, ptr null, ptr @<!-- -->func1, ptr null, ptr null }]
@<!-- -->g4 = constant { i64, i64, i64, ptr, i64, ptr, i64, i64, ptr } { i64 2, i64 0, i64 0, ptr null, i64 64, ptr @<!-- -->g3, i64 1, i64 1, ptr @<!-- -->func2 }

define internal i32 @<!-- -->func1(ptr %0, ptr %1, ptr %2, ptr %3, ptr %4) {
  br i1 false, label %6, label %7
6:
  br label %7
7:
  %8 = phi i64 [ 8, %6 ], [ 0, %5 ]
  %9 = add i64 %8, 8
  %10 = urem i64 %9, 8
  %11 = icmp ugt i64 %10, 0
  br i1 %11, label %12, label %15
12:
  %13 = add i64 %9, 8
  %14 = sub i64 %13, %10
  br label %15
15:
  %16 = phi i64 [ %14, %12 ], [ %9, %7 ]
  %17 = add i64 %16, 8
  %18 = urem i64 %17, 4
  %19 = icmp ugt i64 %18, 0
  br i1 %19, label %20, label %21
20:
  br label %21
21:
  ret i32 0
}

define i32 @<!-- -->func2(ptr %0, ptr %1, i32 %2, ptr %3) {
  %5 = alloca i32, align 4
  %6 = icmp eq i32 %2, 0
  br i1 %6, label %7, label %8
7:
  ret i32 1
8:
  %9 = getelementptr { i64, ptr, ptr, ptr, ptr, ptr, ptr }, ptr %0, i32 0, i32 6
  %10 = load ptr, ptr %9, align 8
  %11 = call i32 %10(ptr %1, i32 16, ptr %5)
  %12 = load { i64, i64, i64, ptr, i64, ptr, i64, i64, ptr }, ptr @<!-- -->g4, align 8
  store { i64, i64, i64, ptr, i64, ptr, i64, i64, ptr } %12, ptr @<!-- -->g1, align 8
  store i64 0, ptr getelementptr inbounds nuw (i8, ptr @<!-- -->g1, i64 16), align 8
  store ptr @<!-- -->g1, ptr %3, align 8
  ret i32 0
}

!llvm.module.flags = !{!0}
!0 = !{i32 2, !"Debug Info Version", i32 3}
```

```bash
# On QNX/aarch64:
./llc test.ll -filetype=obj -o test.o
# SIGSEGV
```

## Crash Analysis

```
(gdb) bt
#<!-- -->0  AArch64AsmPrinter::emitHwasanMemaccessSymbols(llvm::Module&amp;)
#<!-- -->1  AArch64AsmPrinter::emitEndOfAsmFile(llvm::Module&amp;)
#<!-- -->2  llvm::AsmPrinter::doFinalization(llvm::Module&amp;)

(gdb) x/i $pc
=&gt; stp x29, x30, [sp, #<!-- -->64]   ; crash at function prologue

(gdb) x/gx $sp
Cannot access memory at address 0x...   ; stack overflow
```

My internal fork tracks upstream main (around mid-Dec 2025). If this issue has been already fixed please point me the fix commit. Thanks!

</details>


---

### Comment 2 - efriedma-quic

Should be fine to change it to `std::make_unique<AArch64Subtarget>` or something like that.

---

### Comment 3 - felixaszx

QNX default stack is around 128kb under ARM, 512kb under x86. If you did run it under QNX8 compiled by Blackberry themselves, then the stack size will be adjusted to 8mb accordingly. 
Not sure how is this handled tho. Have you tried setting the default stack size for newly created thread on QNX?

---

