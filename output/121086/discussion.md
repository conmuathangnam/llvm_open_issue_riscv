# Failed to compile matrix product like code using llvm 19.x for riscv vector extension.

**Author:** fanghuaqi
**URL:** https://github.com/llvm/llvm-project/issues/121086
**Status:** Closed
**Labels:** backend:RISC-V, release:backport, crash
**Closed Date:** 2024-12-27T02:58:09Z

## Body

Hi there,

I tried to compile source code as below, using clang with options: `-march=rv32imafc_zve32f_zvl128b -mabi=ilp32f -Ofast`

~~~c

float *matA, *matB, *matC;
int nthreads = 1;

typedef struct{
   int id;
   int rowsA;
   int colsA;
   int colsB;
} tArgs;

void CalculaProdutoMatriz(void *arg) {
   int i, j, k;
   tArgs *args = (tArgs*) arg;
   int rowsA = args->rowsA;
   int colsA = args->colsA;
   int colsB = args->colsB;

   for(i = args->id; i < rowsA; i += nthreads) {
      for(j = 0; j < colsB; j++) {
         matC[i * colsB + j] = 0;
         for(k = 0; k < colsA; k++) {
               matC[i * colsB + j] += matA[i * colsA + k] * matB[k * colsB + j];
         }
      }
   }
}
~~~

It works in llvm 18.x and llvm truck(20.x), but failed in llvm 19.x error message like this

```
3.      Running pass 'Function Pass Manager' on module 'main.c'.
4.      Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@main'
```

Thanks

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Huaqi Fang (fanghuaqi)

<details>
Hi there,

I tried to compile source code as below, using clang with options: `-march=rv32imafc_zve32f_zvl128b -mabi=ilp32f -Ofast`

~~~c

float *matA, *matB, *matC;
int nthreads = 1;

typedef struct{
   int id;
   int rowsA;
   int colsA;
   int colsB;
} tArgs;

void CalculaProdutoMatriz(void *arg) {
   int i, j, k;
   tArgs *args = (tArgs*) arg;
   int rowsA = args-&gt;rowsA;
   int colsA = args-&gt;colsA;
   int colsB = args-&gt;colsB;

   for(i = args-&gt;id; i &lt; rowsA; i += nthreads) {
      for(j = 0; j &lt; colsB; j++) {
         matC[i * colsB + j] = 0;
         for(k = 0; k &lt; colsA; k++) {
               matC[i * colsB + j] += matA[i * colsA + k] * matB[k * colsB + j];
         }
      }
   }
}
~~~

It works in llvm 18.x and llvm truck(20.x), but failed in llvm 19.x error message like this

```
3.      Running pass 'Function Pass Manager' on module 'main.c'.
4.      Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->main'
```

Thanks
</details>


---

### Comment 2 - topperc

The error says the `main` function was being compiled, but your source does not have a `main`. Is this the complete source?

---

### Comment 3 - fanghuaqi

I didn't paste the complete source code, I compiled it in my SDK environment, I create compiler explorer link here https://godbolt.org/z/Ec3o8vex1

It contains a main function. I just want to test the auto vectorization for riscv vector in llvm clang.

---

### Comment 4 - wangpc-pp

Can you be more specific about the baseline of 19.x? I can't reproduce the error with:
```bash
# clang -march=rv32imafc_zve32f_zvl128b -mabi=ilp32f -Ofast reduced.c -S
clang: warning: argument '-Ofast' is deprecated; use '-O3 -ffast-math' for the same behavior, or '-O3' to enable only conforming optimizations [-Wdeprecated-ofast]

# clang -v
clang version 19.1.6 (e21dc4bd5474d04b8e62d7331362edcc5648d7e5)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data00/wpc/software/llvm-dev/bin
Build config: +unoptimized, +assertions
```

----
OK, I can reproduce it with `main` function compiled. The error is:
```
Legalizing node: t54: i32,i32 = RISCVISD::SplitF64 t48
Analyzing result type: i32
Legal result type
Analyzing result type: i32
Legal result type
Analyzing operand: t48: f64,ch = load<(load (s32) from %ir.scevgep177, !tbaa !18), anyext from f32> t0, t7, undef:i32
Soften float operand 0: t54: i32,i32 = RISCVISD::SplitF64 t48
SoftenFloatOperand Op #0: t54: i32,i32 = RISCVISD::SplitF64 t48

fatal error: error in backend: Do not know how to soften this operator's operand!
```
The printed latest DAG is:
```
Optimized lowered selection DAG: %bb.35 'main:for.body78.us'
SelectionDAG has 41 nodes:
  t0: ch,glue = EntryToken
  t4: i32,ch = load<(dereferenceable load (s32) from @matC, !tbaa !32)> t0, GlobalAddress:i32<ptr @matC> 0, undef:i32
  t6: i32,ch = CopyFromReg t0, Register:i32 %90
  t13: i64 = bitcast t48
      t12: ch = TokenFactor t4:1, t48:1
    t18: ch,glue = callseq_start t12, TargetConstant:i32<0>, TargetConstant:i32<0>
  t20: ch,glue = CopyToReg t18, Register:i32 $x10, GlobalAddress:i32<ptr @.str.1> 0
    t16: i32 = extract_element t13, Constant:i32<0>
  t22: ch,glue = CopyToReg t20, Register:i32 $x12, t16, t20:1
    t15: i32 = extract_element t13, Constant:i32<1>
  t24: ch,glue = CopyToReg t22, Register:i32 $x13, t15, t22:1
  t27: ch,glue = RISCVISD::CALL t24, TargetGlobalAddress:i32<ptr @printf> 0 [TF=1], Register:i32 $x10, Register:i32 $x12, Register:i32 $x13, RegisterMask:Untyped, t24:1
  t28: ch,glue = callseq_end t27, TargetConstant:i32<0>, TargetConstant:i32<0>, t27:1
  t31: i32 = add t6, Constant:i32<4>
    t7: i32 = add t4, t6
  t48: f64,ch = load<(load (s32) from %ir.scevgep177, !tbaa !18), anyext from f32> t0, t7, undef:i32
        t33: ch = CopyToReg t0, Register:i32 %91, t31
        t29: i32,ch,glue = CopyFromReg t28, Register:i32 $x10, t28:1
      t40: ch = TokenFactor t33, t29:1
        t35: i32,ch = CopyFromReg t0, Register:i32 %89
      t46: i1 = setcc t31, t35, setne:ch
    t47: ch = brcond t40, t46, BasicBlock:ch<for.body78.us 0x55d485ddc040>
  t44: ch = br t47, BasicBlock:ch<for.cond74.for.cond.cleanup77_crit_edge.us 0x55d485ddc1a8>
```
The LLVM IR of `'main:for.body78.us'` is:
```
for.body78.us:                                    ; preds = %for.cond74.preheader.us, %for.body78.us
  %lsr.iv175 = phi i32 [ %lsr.iv173, %for.cond74.preheader.us ], [ %lsr.iv.next176, %for.body78.us ]
  %60 = load ptr, ptr @matC, align 4, !tbaa !32
  %scevgep177 = getelementptr i8, ptr %60, i32 %lsr.iv175
  %61 = load float, ptr %scevgep177, align 4, !tbaa !18
  %conv82.us = fpext float %61 to double
  %call83.us = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.1, double noundef nofpclass(nan inf) %conv82.us)
  %lsr.iv.next176 = add i32 %lsr.iv175, 4
  %lsr_fold_term_cond.replaced_term_cond = icmp eq i32 %lsr.iv.next176, %59
  br i1 %lsr_fold_term_cond.replaced_term_cond, label %for.cond74.for.cond.cleanup77_crit_edge.us, label %for.body78.us, !llvm.loop !45
```
I may take a detailed look later.

---

### Comment 5 - fanghuaqi

Thanks @topperc @wangpc-pp , yes the error is `fatal error: error in backend: Do not know how to soften this operator's operand!`



---

### Comment 6 - fanghuaqi

When I changed the `-march=rv32imafc_zve32f_zvl128b -mabi=ilp32f` to `-march=rv32imafdc_zve32f_zvl128b -mabi=ilp32d` then the compile error disappeared.

---

### Comment 7 - wangpc-pp

Reduced test:
```
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-unknown-linux-gnu"

define i32 @main(float %0) {
entry:
  %conv82.us = fpext float %0 to double
  %call83.us = tail call i32 (ptr, ...) @printf(ptr null, double %conv82.us)
  ret i32 0
}

declare i32 @printf(ptr, ...)
```
I think https://github.com/llvm/llvm-project/pull/116159 can fix this error.
We may backport it to 19.x, but it may be too late. @topperc WDYT?

---

### Comment 8 - fanghuaqi

Hi @wangpc-pp ， this #116159 can fix this issue locally in my llvm compiler cherry picked this PR.

---

### Comment 9 - topperc

/cherry-pick 0019565e9322350145c2b3bbc06a3a042f3a8ee1

---

### Comment 10 - llvmbot

/pull-request llvm/llvm-project#121175

---

### Comment 11 - topperc

/pull-request #121501

---

