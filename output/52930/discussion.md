# Reading uninitialized memory must be poison, not undef

**Author:** nunoplopes
**URL:** https://github.com/llvm/llvm-project/issues/52930
**Status:** Open
**Labels:** clang:codegen, llvm:core, miscompilation

## Body

Currently we consider loads of uninitialized memory to be undef. This is both sub-optimal and a source of miscompilations.
For example, instsimplify & newgvn convert `phi(X, undef) -> X`. This is wrong as `X` might be poison. We had the same issue with `select` but it has already been fixed.
Fixing this bug is related with uninit memory because sroa/mem2reg create phis with undef. And we need to be able to fold those away.

We have a patch for langref, sroa, mem2reg here: https://reviews.llvm.org/D104648
I have patches locally for InstSimplify & NewGVN to fix the phi issue mentioned above.

Changing the semantics of load of uninit memory breaks compatibility. Although most users won't notice, we would need to interface with the major frontends.
For clang, we would need to fix codegen of bit-fields as at the least the first field store loads uninit memory before masking it. The easy way is to just freeze the loaded value. Spill-over of poison from different fields is not an issue as if one field becomes poison is because some UB already happened in the program so we can spill over poison to neighboring fields.
Bitfields aren't very common, so this shouldn't be a major issue.

On LLVM's side, I believe the only issue is load widening. This must be either be removed at IR level, or we need to use vectors (or the byte type) for the widened type.

In summary, todo:
- [ ] Change clang's codegen of bit-field loads to do a freeze of the loaded value
- [ ] Investigate if we need new instcombine patterns to remove freezes from bitfields
- [ ] Announce breaking changes to IR semantics on the mailing list for the following LLVM version
- [ ] Change semantics: commit LangRef patch
- [ ] Add auto-upgrade for old IR: load -> load + freeze
- [ ] Change load widening to use vectors
- [ ] Commit SROA + mem2reg patches
- [ ] Commit InstSimplify patch
- [x] Commit NewGVN patch
- [ ] Revert https://github.com/llvm/llvm-project/commit/1881711fbb7b0cd1b8d492b3ca4b70ce75824030
- [ ] Celebrate? One step closer to removing undef 🥂

cc @aqjune @regehr @nikic @rotateright @LebedevRI @alinas

## Comments

### Comment 1 - nikic

Generally sounds good, but I believe we also need to auto-upgrade existing bitcode by replacing all loads with freeze(load), which will significantly pessimize the ability to optimize old bitcode.

---

### Comment 2 - nunoplopes

> Generally sounds good, but I believe we also need to auto-upgrade existing bitcode by replacing all loads with freeze(load), which will significantly pessimize the ability to optimize old bitcode.

Right, forgot that. Added to the todo list, thanks!

---

### Comment 3 - aqjune

Sounds like a great plan! 
AFAIR load widening already exists in SelDag and/or MIR, and alias analysis should still work well without load widening, so it won't greatly affect performance (my hope).
The old GVN has the load widening; should we remove it?

---

### Comment 4 - nunoplopes

NewGVN patch fixed in https://github.com/llvm/llvm-project/commit/f1c18acb07aa40f42b87b70462a6d1ab77a4825c
Started with this one as is low risk for perf regressions as it's not enabled in the default pipeline.

---

### Comment 5 - nunoplopes

See also: https://reviews.llvm.org/D89050
Introduced !noundef on loads. Is that sufficient? Is that a good stepping stone on the way to change semantics?

(just for documentation so we don't forget)

---

### Comment 6 - aqjune

Clang frontend must be able to use the !noundef to annotate lvalues that must be initialized.
This will reduce the cost of using freeze, and be helpful for poison-to-undef switch because loading undef anyway becomes UB.

---

### Comment 7 - RalfJung

> On LLVM's side, I believe the only issue is load widening. This must be either be removed at IR level, or we need to use vectors (or the byte type) for the widened type.

Or `poison` might be changed to work per-bit, more like `undef` does, which would make load widening legal?

---

### Comment 8 - nunoplopes

> > On LLVM's side, I believe the only issue is load widening. This must be either be removed at IR level, or we need to use vectors (or the byte type) for the widened type.
> 
> Or `poison` might be changed to work per-bit, more like `undef` does, which would make load widening legal?

That's a huge can of worms. It's hard to define the semantics of arithmetic operations with bitwise poison semantics. Several people have tried in the past and failed.
So I have to consider bitwise poison to be impossible until there's a concrete proposal.

---

### Comment 9 - regehr

> That's a huge can of worms. It's hard to define the semantics of arithmetic operations with bitwise poison semantics. Several people have tried in the past and failed. So I have to consider bitwise poison to be impossible until there's a concrete proposal.

yeah I don't think we've ever seen a proposal along these lines that resulted in rules that were comprehensible in practice to the target audience, people trying to reason about optimizations. my take on what I saw was that it was so complicated that one would simply need to blindly trust alive or whatever other automated checking tool people write

---

### Comment 10 - RalfJung

> That's a huge can of worms. It's hard to define the semantics of arithmetic operations with bitwise poison semantics. Several people have tried in the past and failed.

Are these previously considered proposals and their issues summarized somewhere?

> So I have to consider bitwise poison to be impossible until there's a concrete proposal.

FWIW https://discourse.llvm.org/t/a-memory-model-for-llvm-ir-supporting-limited-type-punning/61948 proposes bitwise poison where for arithmetic, if *any* input bit is poison, the full output is poison.

---

### Comment 11 - SiliconA-Z

Is this fixed yet? As in can we change the optimizations that mention this issue to use poison rather than undef?

---

