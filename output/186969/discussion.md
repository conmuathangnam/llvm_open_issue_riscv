# RISC-V ISel hangs with `+unaligned-scalar-mem` on memcmp pattern

**Author:** TurkeyMan
**URL:** https://github.com/llvm/llvm-project/issues/186969

## Body

llc hangs in the RISC-V DAG→DAG Pattern Instruction Selection pass (riscv-isel) when compiling a simple function containing stores into an alloca buffer followed by memcmp, when +unaligned-scalar-mem is enabled. Without the feature flag, compilation completes instantly.

### Reproducer:

Version: LLVM 21.1.8

```ll
; repro.ll
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown-elf"

@.str.45 = constant [4 x i8] c" 16\00"
@.str.47 = constant [4 x i8] c"-0E\00"

declare i32 @memcmp(ptr, ptr, i64)

define i1 @test() {
domemcmp23:
  %buffer = alloca [64 x i8], align 1
  store i8 0, ptr null, align 4294967296
  %0 = getelementptr i8, ptr %buffer, i64 1
  store i8 49, ptr %0, align 1
  %1 = getelementptr i8, ptr %buffer, i64 2
  store i8 32, ptr %buffer, align 1
  store i8 54, ptr %1, align 1
  %2 = call i32 @memcmp(ptr %buffer, ptr @.str.45, i64 3)
  %3 = icmp eq i32 %2, 0
  br i1 %3, label %assertPassed44, label %assertFailed45

assertPassed44:
  %4 = call i32 @memcmp(ptr null, ptr @.str.47, i64 3)
  %5 = icmp eq i32 %4, 0
  ret i1 %5

assertFailed45:
  unreachable
}
```

### Commands:


#### Hangs:
```
llc -mtriple=riscv64-unknown-elf -mattr=+m,+a,+f,+d,+c,+unaligned-scalar-mem -O1 -o /dev/null repro.ll
```

#### Works (no unaligned-scalar-mem):
```
llc -mtriple=riscv64-unknown-elf -mattr=+m,+a,+f,+d,+c -O1 -o /dev/null repro.ll
```

#### Works (O0):
```
llc -mtriple=riscv64-unknown-elf -mattr=+m,+a,+f,+d,+c,+unaligned-scalar-mem -O0 -o /dev/null repro.ll
```

Bisection:

Hang is in the riscv-isel pass (confirmed via --print-before-all)
-O0 works, -O1 hangs
Removing +unaligned-scalar-mem avoids the hang at any optimization level
The pattern involves memcmp with a known-length (3) against stores into a stack alloca — likely the memcmp expansion interacts badly with unaligned load merging

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Manu Evans (TurkeyMan)

<details>
llc hangs in the RISC-V DAG→DAG Pattern Instruction Selection pass (riscv-isel) when compiling a simple function containing stores into an alloca buffer followed by memcmp, when +unaligned-scalar-mem is enabled. Without the feature flag, compilation completes instantly.

### Reproducer:

Version: LLVM 21.1.8

```ll
; repro.ll
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown-elf"

@.str.45 = constant [4 x i8] c" 16\00"
@.str.47 = constant [4 x i8] c"-0E\00"

declare i32 @<!-- -->memcmp(ptr, ptr, i64)

define i1 @<!-- -->test() {
domemcmp23:
  %buffer = alloca [64 x i8], align 1
  store i8 0, ptr null, align 4294967296
  %0 = getelementptr i8, ptr %buffer, i64 1
  store i8 49, ptr %0, align 1
  %1 = getelementptr i8, ptr %buffer, i64 2
  store i8 32, ptr %buffer, align 1
  store i8 54, ptr %1, align 1
  %2 = call i32 @<!-- -->memcmp(ptr %buffer, ptr @.str.45, i64 3)
  %3 = icmp eq i32 %2, 0
  br i1 %3, label %assertPassed44, label %assertFailed45

assertPassed44:
  %4 = call i32 @<!-- -->memcmp(ptr null, ptr @.str.47, i64 3)
  %5 = icmp eq i32 %4, 0
  ret i1 %5

assertFailed45:
  unreachable
}
```

### Commands:


#### Hangs:
```
llc -mtriple=riscv64-unknown-elf -mattr=+m,+a,+f,+d,+c,+unaligned-scalar-mem -O1 -o /dev/null repro.ll
```

#### Works (no unaligned-scalar-mem):
```
llc -mtriple=riscv64-unknown-elf -mattr=+m,+a,+f,+d,+c -O1 -o /dev/null repro.ll
```

#### Works (O0):
```
llc -mtriple=riscv64-unknown-elf -mattr=+m,+a,+f,+d,+c,+unaligned-scalar-mem -O0 -o /dev/null repro.ll
```

Bisection:

Hang is in the riscv-isel pass (confirmed via --print-before-all)
-O0 works, -O1 hangs
Removing +unaligned-scalar-mem avoids the hang at any optimization level
The pattern involves memcmp with a known-length (3) against stores into a stack alloca — likely the memcmp expansion interacts badly with unaligned load merging
</details>


---

