# [IROutliner]  Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed

**Author:** svs-quic
**URL:** https://github.com/llvm/llvm-project/issues/178874

## Body

https://godbolt.org/z/rE63of7G8

The IR Outliner when run on 

```llvm
define ptr @outliner_crash(ptr %arrayidx9.i) {
entry:
  br label %do.body.i

do.body.i:                                        ; preds = %entry
  switch i8 0, label %do.end213.i [
    i8 5, label %do.end213.i
    i8 4, label %if.then202.i
  ]

if.then202.i:                                     ; preds = %do.body.i
  %0 = load i8, ptr null, align 1
  %cmp11.not.i288 = icmp eq i8 1, 0
  %arrayidx14.i290 = getelementptr [2 x [3 x i8]], ptr getelementptr inbounds nuw (i8, ptr null, i32 4), i32 0
  ret ptr %arrayidx14.i290

do.end213.i:                                      ; preds = %do.body.i, %do.body.i
  %1 = load i8, ptr %arrayidx9.i, align 1
  %cmp11.not.i = icmp eq i8 1, 0
  %arrayidx14.i265 = getelementptr [2 x [3 x i8]], ptr getelementptr inbounds nuw (i8, ptr null, i32 4), i32 0
  ret ptr null
}
```

as  `opt -S -passes=verify,iroutliner -ir-outlining-no-cost --debug-only=iroutliner`

crashes with the following assertion:

`opt: /root/llvm-project/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = llvm::Constant; From = llvm::Value]: Assertion isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.`

Looking at the outliner logs it looks like it is trying to replace the `ptr null` value in the `getelementptr` with a function argument `ptr %0` :

Replacing uses of constant ptr null in function

```llvm
 ; Function Attrs: minsize optsize
define internal void @outlined_ir_func_0(ptr %0, ptr %1, i32 %2) #1 {
newFuncRoot:
  br label %if.then202.i_to_outline

if.then202.i_to_outline:                          ; preds = %newFuncRoot
  %3 = load i8, ptr null, align 1
  %cmp11.not.i288 = icmp eq i8 1, 0
  %arrayidx14.i290 = getelementptr [2 x [3 x i8]], ptr getelementptr inbounds nuw (i8, ptr null, i32 4), i32 0
  br label %if.then202.i_after_outline.exitStub

if.then202.i_after_outline.exitStub:              ; preds = %if.then202.i_to_outline
  ret void

output_block_0_0:                                 ; No predecessors!
  store ptr %arrayidx14.i290, ptr %1, align 8
}

 with ptr %0
```

## Comments

### Comment 1 - spider-yamet

Hi @EugeneZelenko @svs-quic Hope you are doing great.
May I fix this bug? I am so interested, I just want to know if this issue is open for contribution.
Looking forward to your response.

Regards

---

### Comment 2 - svs-quic

> Hi [@EugeneZelenko](https://github.com/EugeneZelenko) [@svs-quic](https://github.com/svs-quic) Hope you are doing great. May I fix this bug? I am so interested, I just want to know if this issue is open for contribution. Looking forward to your response.
> 
> Regards

hi  @spider-yamet, we're taking a look at this downstream as well, but please feel free to assign it to yourself. 

---

### Comment 3 - lenary

So I think there are a few different confounding things going on here. I'm using a very slightly simpler example

<details><summary>My Code</summary>

I turned the switch back into a BR, and got rid of the spurious `icmp eq`s.

```llvm
define ptr @outliner_crash(ptr %arrayidx9.i) {
entry:
  %cmp = icmp eq i8 0, 4
  br i1 %cmp, label %if.then202.i, label %do.end213.i

if.then202.i:                                     ; preds = %do.body.i
  %0 = load i8, ptr null, align 1
  %arrayidx14.i290 = getelementptr [2 x [3 x i8]], ptr getelementptr inbounds nuw (i8, ptr null, i32 4), i32 0
  ret ptr %arrayidx14.i290

do.end213.i:                                      ; preds = %do.body.i, %do.body.i
  %1 = load i8, ptr %arrayidx9.i, align 1
  %arrayidx14.i265 = getelementptr [2 x [3 x i8]], ptr getelementptr inbounds nuw (i8, ptr null, i32 4), i32 0
  ret ptr null
}
```

</details>

`opt` does not crash if asserts are disabled. But that output is wrong:

```llvm
define ptr @outliner_crash(ptr %arrayidx9.i) {
entry:
  %arrayidx14.i290.loc = alloca ptr, align 8
  %cmp = icmp eq i8 0, 4
  br i1 %cmp, label %if.then202.i, label %do.end213.i

if.then202.i:
  call void @llvm.lifetime.start.p0(ptr %arrayidx14.i290.loc)
  call void @outlined_ir_func_0(ptr null, ptr %arrayidx14.i290.loc, i32 0)
  %arrayidx14.i290.reload = load ptr, ptr %arrayidx14.i290.loc, align 8
  call void @llvm.lifetime.end.p0(ptr %arrayidx14.i290.loc)
  ret ptr %arrayidx14.i290.reload

do.end213.i:
  call void @outlined_ir_func_0(ptr %arrayidx9.i, ptr null, i32 -1)
  ret ptr null
}

define internal void @outlined_ir_func_0(ptr %0, ptr %1, i32 %2) #1 {
newFuncRoot:
  br label %if.then202.i_to_outline

if.then202.i_to_outline:
  %3 = load i8, ptr %0, align 1
  %arrayidx14.i290 = getelementptr [2 x [3 x i8]], ptr getelementptr inbounds nuw (i8, ptr %0, i32 4), i32 0
  br label %if.then202.i_after_outline.exitStub

if.then202.i_after_outline.exitStub:
  switch i32 %2, label %final_block_0 [
    i32 0, label %output_block_0_0
  ]

output_block_0_0:
  store ptr %arrayidx14.i290, ptr %1, align 8
  br label %final_block_0

final_block_0:
  ret void
}
```

In the calculation of `%arrayidx14.i290`, the `ptr null` has been replaced with `ptr %0`. This is wrong for two reasons:
- This GEP has `ptr null` in both outlined regions
- We shouldn't have a value in a GEP Constant

LLVM 21 without assertions shows the same output: https://godbolt.org/z/8x111zbxd

LLVM 20 does not have the same bug: https://godbolt.org/z/fdTGrTEPY

The outlined function correctly only uses the `ptr %0` argument in the `load`, and not in the (constant) gep.

I think the problem could be a80a802d2f07 or beac727e4834. However, we cannot just revert these as they migrate away from APIs that are (iiuc) no longer available.

It's not clear to me that ValueMapper can map constants how the outliner needs to, anyway. We only want to map specific occurrences of any given constant, not all occurrences.


---

