# [SDAG] miscompile of abs(add(v, 1)) when v is known non-negative

**Author:** aleks-tmb
**URL:** https://github.com/llvm/llvm-project/issues/185467
**Status:** Open
**Labels:** miscompilation, llvm:SelectionDAG

## Body

During downstream testing we found a miscompile with the following reduced test case:
```llvm
@x = global i32 1, align 4

declare i32 @llvm.abs.i32(i32, i1 immarg)

define i32 @main() {
  %v    = load i32, ptr @x, align 4, !range !0
  %v1   = add i32 %v, 1
  %absx = call i32 @llvm.abs.i32(i32 %v1, i1 false)
  ret i32 %absx
}
!0 = !{i32 0, i32 2147483647}
```
This function should return 2. However, when compiled with llc (https://godbolt.org/z/o5hWfKWbx), the generated code returns -2:
```asm
main:                                   # @main
        mov     rax, qword ptr [rip + x@GOTPCREL]
        mov     ecx, dword ptr [rax]
        lea     edx, [rcx + 1]
        mov     eax, -1
        sub     eax, ecx
        cmovb   eax, edx
        ret
x:
        .long   1                               # 0x1
```
Running the generated code produces -2, while the expected result is 2.

We bisected this issue and identified the following commit as the likely culprit: https://github.com/llvm/llvm-project/commit/fbda30607c5176f9e54ac86e75435cc57f322674

It seems the emitted `cmovbl` (CF-based) is problematic here, since it never triggers because CF is always 0 after `subl %ecx, 0xFFFFFFFF` - no unsigned borrow is possible. The result stays at -V instead of V.

Before the patch (https://godbolt.org/z/K8K1vsvah), llc emits `cmovsl` (SF-based) instead. SF is set correctly by `negl %eax`, so the move triggers and returns the right value.

## Comments

### Comment 1 - RKSimon

CC @DaKnig 

---

### Comment 2 - DaKnig

on it.

---

### Comment 3 - DaKnig

It seems that the patch fails its job (for the given input): https://godbolt.org/z/6oePPdx5x

An exert:
```
Initial selection DAG: %bb.0 'main:'
SelectionDAG has 12 nodes:
  t0: ch,glue = EntryToken
  t2: i64 = Constant<0>
        t4: i32,ch = load<(dereferenceable load (s32) from @x, !range !0)> t0, GlobalAddress:i64<ptr @x> 0, undef:i64
      t6: i32 = add t4, Constant:i32<1>
    t7: i32 = abs t6
  t10: ch,glue = CopyToReg t0, Register:i32 $eax, t7
  t11: ch = X86ISD::RET_GLUE t10, TargetConstant:i32<0>, Register:i32 $eax, t10:1

...

Combining: t7: i32 = abs t6
Creating constant: t12: i32 = Constant<0>
Creating constant: t13: i32 = Constant<-1>
Creating new node: t14: i32 = abdu t4, Constant:i32<-1>
 ... into: t14: i32 = abdu t4, Constant:i32<-1>

```

It seems to me that, for the given loaded value 1 at least, this transformed code produces the wrong answer. Some constant checks should be in place, for the abdu case. Will work on it.

<details>
  <summary>Later, less relevant transformations not directly caused by the patch</summary>
  
  ```
Legalizing: t14: i32 = abdu t4, Constant:i32<-1>
Trying custom legalization
Creating new node: t15: i32 = freeze t4
Creating new node: t16: i32,i32 = X86ISD::SUB t15, Constant:i32<-1>
Creating new node: t17: i32,i32 = X86ISD::SUB Constant:i32<-1>, t15
Creating constant: t18: i8 = TargetConstant<2>
Creating new node: t19: i32 = X86ISD::CMOV t17, t16, TargetConstant:i8<2>, t17:1
Successfully custom legalized node
 ... replacing: t14: i32 = abdu t4, Constant:i32<-1>
     with:      t19: i32 = X86ISD::CMOV t17, t16, TargetConstant:i8<2>, t17:1

...

Optimized legalized selection DAG: %bb.0 'main:'
SelectionDAG has 17 nodes:
  t0: ch,glue = EntryToken
      t28: i32 = add t15, Constant:i32<1>
    t19: i32 = X86ISD::CMOV t17, t28, TargetConstant:i8<2>, t17:1
  t10: ch,glue = CopyToReg t0, Register:i32 $eax, t19
        t21: i64 = X86ISD::WrapperRIP TargetGlobalAddress:i64<ptr @x> 0 [TF=5]
      t22: i64,ch = load<(load (s64) from got)> t0, t21, undef:i64
    t4: i32,ch = load<(dereferenceable load (s32) from @x, !range !0)> t0, t22, undef:i64
  t15: i32 = freeze t4
  t17: i32,i32 = X86ISD::SUB Constant:i32<-1>, t15
  t11: ch = X86ISD::RET_GLUE t10, TargetConstant:i32<0>, Register:i32 $eax, t10:1

  ```
  
</details>

---

### Comment 4 - RKSimon

@DaKnig I'm reverting the patch at #186068 - you will need to add a regression test prior to an updated patch for review

---

### Comment 5 - DaKnig

https://github.com/llvm/llvm-project/pull/186659 <- fix + regression test

---

