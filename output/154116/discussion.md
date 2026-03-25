# separate-const-offset-from-gep miscompile related to "trunc nuw"

**Author:** mikaelholmen
**URL:** https://github.com/llvm/llvm-project/issues/154116
**Status:** Closed
**Labels:** miscompilation, llvm:transforms
**Closed Date:** 2025-08-22T08:29:38Z

## Body

llvm commit: 673750feea

Reproduce with:
```
opt -passes=separate-const-offset-from-gep bbi-109669_sep.ll -S -o - -mtriple=hexagon
```
The input program does
```
%0 = phi i32 [ -20, %entry ]
%1 = add i32 %0, 20
%2 = trunc nuw i32 %1 to i16
```
where the "trunc nuw" is ok since %1 is 0. No truncated bits are set.

However, in the result we get:
```
%0 = phi i32 [ -20, %entry ]
[...]
%1 = trunc nuw i32 %0 to i16
```
so here the input to "trunc nuw" is negative and the result is poison.
Langref https://llvm.org/docs/LangRef.html#trunc-to-instruction says
"If the nuw keyword is present, and any of the truncated bits are non-zero, the result is a [poison value](https://llvm.org/docs/LangRef.html#poisonvalues)."

Then %1 is further used to compute the return value from the function so the whole thing results in poison which it did not in the input.

(I stumbled upon this as a miscompile after
 https://github.com/llvm/llvm-project/pull/152990
which I first incorrectly blamed.)

[bbi-109669_sep.ll.gz](https://github.com/user-attachments/files/21836248/bbi-109669_sep.ll.gz)

## Comments

### Comment 1 - bjope

I don't think I'm an expert at SeparateConstOffsetFromGEP, but some ideas after looking at the source code:
* One idea is to let `ConstantOffsetExtractor::applyExts `drop poison generating flags from any TruncInst when cloning the chain. Afaict distributing trunc over add/sub (and disjoint or) should be fine if we drop the flags.
* Another idea is to track if we have truncate with nuw/nsw in `ConstantOffsetExtractor::find` and `ConstantOffsetExtractor::CanTraceInto` to skip rewrites in scenarios when we can't keep `trunc nuw`.

But maybe there are other solutions?

---

### Comment 2 - nikic

cc @ritter-x2a @jrbyrnes @krzysz00 

> One idea is to let `ConstantOffsetExtractor::applyExts `drop poison generating flags from any TruncInst when cloning the chain. Afaict distributing trunc over add/sub (and disjoint or) should be fine if we drop the flags.

That sounds reasonable to me.

---

### Comment 3 - krzysz00

Yeah, dropping the `nsw` or `nuw` is probablyt he safe thing

(That or doing some analysis on the operands to the add to see if they're also small enough)

---

### Comment 4 - ritter-x2a

I think there is more broken around distributing truncs in separate-const-offset-from-gep. For example, I got this miscompilation while checking if the fix makes sense: https://alive2.llvm.org/ce/z/hLPfcw

It seems odd that `NonNegative` in `ConstantOffsetExtractor::find` [isn't cleared for trunc](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L606-L609) but [it is for zext](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L617-L620) "because zext(a) >= 0 does not imply a >= 0" -- that doesn't hold for trunc either.
The meaning of `NonNegative` also seems odd: [the doxygen comment](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L235-L239) says
> NonNegative -- Whether V is guaranteed to be non-negative. For example, an index of an inbounds GEP is guaranteed to be non-negative. Levaraging this, we can better split inbounds GEPs."

I don't think that's right, inbounds GEP indices can be negative.

---

### Comment 5 - bjope

> It seems odd that `NonNegative` in `ConstantOffsetExtractor::find` [isn't cleared for trunc](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L606-L609) but [it is for zext](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L617-L620) "because zext(a) >= 0 does not imply a >= 0" -- that doesn't hold for trunc either. 
 
I've been suspecting that there could be more problems in that area. Wondering if perhaps `NonNegative` should be cleared for trunc. And I'm not quite sure it is correct to just pass on `SignExtended` and `ZeroExtended` either for trunc.

I'm not sure if there is any comprehensive tests involving scenarios with combinarions of sext/zext/trunc/add/sub/or/xor. The support for xor is for example not using CanTraceInto, so unclear to me how that works together with `NonNegative`, `SignExtended` and `ZeroExtended`.

Still think dropping the nuw/nsw flags (https://github.com/llvm/llvm-project/pull/154582) would solve the problem given the IR reproducer in this issue. But agree that more problems might be lurking.

---

### Comment 6 - mikaelholmen

> I think there is more broken around distributing truncs in separate-const-offset-from-gep. For example, I got this miscompilation while checking if the fix makes sense: https://alive2.llvm.org/ce/z/hLPfcw
> 
> It seems odd that `NonNegative` in `ConstantOffsetExtractor::find` [isn't cleared for trunc](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L606-L609) but [it is for zext](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L617-L620) "because zext(a) >= 0 does not imply a >= 0" -- that doesn't hold for trunc either. The meaning of `NonNegative` also seems odd: [the doxygen comment](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L235-L239) says
> 
> > NonNegative -- Whether V is guaranteed to be non-negative. For example, an index of an inbounds GEP is guaranteed to be non-negative. Levaraging this, we can better split inbounds GEPs."
> 
> I don't think that's right, inbounds GEP indices can be negative.

@ritter-x2a : 
Will you write a new issue about the miscompile you found or how do we make sure that is not just forgotten?

---

### Comment 7 - ritter-x2a

> > I think there is more broken around distributing truncs in separate-const-offset-from-gep. For example, I got this miscompilation while checking if the fix makes sense: https://alive2.llvm.org/ce/z/hLPfcw
> > It seems odd that `NonNegative` in `ConstantOffsetExtractor::find` [isn't cleared for trunc](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L606-L609) but [it is for zext](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L617-L620) "because zext(a) >= 0 does not imply a >= 0" -- that doesn't hold for trunc either. The meaning of `NonNegative` also seems odd: [the doxygen comment](https://github.com/llvm/llvm-project/blob/f306e0aeb2c72e040c59e160e88af3bf76457693/llvm/lib/Transforms/Scalar/SeparateConstOffsetFromGEP.cpp#L235-L239) says
> > > NonNegative -- Whether V is guaranteed to be non-negative. For example, an index of an inbounds GEP is guaranteed to be non-negative. Levaraging this, we can better split inbounds GEPs."
> > 
> > 
> > I don't think that's right, inbounds GEP indices can be negative.
> 
> [@ritter-x2a](https://github.com/ritter-x2a) : Will you write a new issue about the miscompile you found or how do we make sure that is not just forgotten?

Here it is: #154936
I might be able to look into this myself in a few weeks, but right now I don't have the capacity.

---

