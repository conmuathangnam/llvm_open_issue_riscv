# [VE][TableGen] Corrupted LaneBitmask representing regLiveUnits for few registerClasses

**Author:** vg0204
**URL:** https://github.com/llvm/llvm-project/issues/138552
**Status:** Closed
**Labels:** tablegen, llvm:codegen, backend:VE
**Closed Date:** 2025-05-14T14:02:49Z

## Body

In VE codegen, the laneBitmask generated corresponding to regUnits (for MCRegister) via MCRegUnitMaskIterator is always corrupted (value being 0x0 irrespective of regunits) for registerClasses dealing with scalar i32 & f32 dataTypes. 

Its happening specifically for `SX#index` registers (64-bit) which are constituted from subregs `SW#index`(32-bit) and `SF#index` (32-bit) respectively with respective subregIndex denoting position as `sub_i32` and `sub_i32`. Now, as in Target.td, the rules for subreg for any registers suggests that these subregs cannot be overlapping(or aliases) with each other. For example, in X86, EAX's Subregs list contains only [AX], not [AX, AH, AL]. 

But, as its defined, `SF#index` is marked as an alias of `SW#index`, which makes them overlapping with each other OR technically the same register, and then also used as subregs for `SX#index` vioalating the tableGen rule, which causing the generation of exactly single regunit(with 2 roots register) of form `SW#index~SF#index` with consequently corrupted LaneBitmask value, instead of 2 regunits eventually reperesnting upper and lower halves of `SX#index`, with right laneBitmask

This is acting as a blocker for #129847, where I observed significant downfall in VE's LIT test due to corrupted LaneBitmask, causing incorrect livein addition at ISEL-phase.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Vikash Gupta (vg0204)

<details>

</details>


---

### Comment 2 - vg0204

CC : @jayfoad @arsenm @Pierre-vh. Can you have a look at it!

---

### Comment 3 - jayfoad

VE uses "ad hoc aliasing", i.e. using the `Aliases` field in TableGen to force LLVM to think that two registers alias, even though the subreg info makes it look like they are disjoint. In TableGen this is modelled by creating a fake regunit shared by the two registers, but because it is not associated with any subreg info, it does not have a well defined lanemask. In practice if you query the lanemask for the fake regunit you get zero. This is probably what you're seeing.

See also #78942 and linked PRs.

cc @kaz7

---

### Comment 4 - vg0204

> VE uses "ad hoc aliasing", i.e. using the `Aliases` field in TableGen to force LLVM to think that two registers alias, even though the subreg info makes it look like they are disjoint. In TableGen this is modelled by creating a fake regunit shared by the two registers, but because it is not associated with any subreg info, it does not have a well defined lanemask. In practice if you query the lanemask for the fake regunit you get zero. This is probably what you're seeing.
> 
> See also #78942 and linked PRs.
> 
> cc @kaz7

Yes, I am seeing it as zero. Also, Is it right to call it fake regUnit, rather than a regunit with 2 root registers Or is it the saying the same thing. Also, if the 2 registers are one in the same, how does it make sense to represent both upper 32bit and lower 32bit part of the sx registers from hardware's perspective, I am quite lost at that part in its visualization.

---

### Comment 5 - vg0204

> See also #78942 and linked PRs.

Seen the PR, and I too tried your initial experimental patch that solved the issue, but as it's further mentioned it's required to model register usage for VE arch, you suggested a partial revert.

Technically, it makes sense. Just I know this case is impossible to occur, but what if all regunits for any register in some other target is dead? This partial revert type patch will overlook that scenario saying all regunits are alive!

---

### Comment 6 - arsenm

Should the VE case be using RegisterTuples? Last I looked at this we have 2 incompatible ways of expressing subregisters 

---

### Comment 7 - vg0204

> Should the VE case be using RegisterTuples? Last I looked at this we have 2 incompatible ways of expressing subregisters 

VE really not using RegisterTuple, but it's registers such as SX0 are modelled out of subregs, in this scenario specifically those subregs (supposed to be disjoint) are aliases of each other.

Can you put more light on suggested incompatible ways!

---

### Comment 8 - jayfoad

> Also, Is it right to call it fake regUnit, rather than a regunit with 2 root registers Or is it the saying the same thing.

It is probably saying the same thing. I am not too familiar with root registers.

> Should the VE case be using RegisterTuples? Last I looked at this we have 2 incompatible ways of expressing subregisters

I thought RegisterTuples was just some convenience stuff built on top of the core SubRegs machinery.

---

### Comment 9 - jayfoad

See also [this comment](https://github.com/llvm/llvm-project/pull/79831#issuecomment-1943746260):

> By the way I have yet another idea for fixing the reported problem. Currently if a leaf register has an ad hoc alias, tablegen will give it a regunit to represent the alias but it will not give it its own "leaf" regunit. My idea is that we could change that, so that leaf registers always get a unique leaf regunit, in addition to any regunits for ad hoc aliasing. This is something that I need anyway for another project: converting MRI / TRI to track reserved regunits instead of reserved registers.

---

### Comment 10 - vg0204

> See also [this comment](https://github.com/llvm/llvm-project/pull/79831#issuecomment-1943746260):
> 
> > By the way I have yet another idea for fixing the reported problem. Currently if a leaf register has an ad hoc alias, tablegen will give it a regunit to represent the alias but it will not give it its own "leaf" regunit. My idea is that we could change that, so that leaf registers always get a unique leaf regunit, in addition to any regunits for ad hoc aliasing. This is something that I need anyway for another project: converting MRI / TRI to track reserved regunits instead of reserved registers.

I agree with it, in that way its laneBitmask would be generated appropriately. Even your need for it,  is identical to my ongoing PR ( mentioned here as well ) about converting MBB LiveIn tracking as regunits rather than registers. So can you look into [this comment](https://github.com/llvm/llvm-project/pull/129847#issuecomment-2846948448), telling is it right way to go with.

CC : @arsenm 

---

