# [Mips] Cttz table lookup lowering causes "cannot select" error on mips16

**Author:** topperc
**URL:** https://github.com/llvm/llvm-project/issues/159625
**Status:** Open
**Labels:** backend:MIPS, crash

## Body

This test case generates an error with `llc -mtriple=mips -mattr=+mips16`

```
define i32 @foo(i32 %x) {
  %a = call i32 @llvm.cttz.i32(i32 %x, i1 true)
  ret i32 %a
}
```

```
LLVM ERROR: Cannot select: 0x214bf9c0: i32 = MipsISD::Lo TargetConstantPool:i32<[32 x i8] c"\00\01\1C\02\1D\0E\18\03\1E\16\14\0F\19\11\04\08\1F\1B\0D\17\15\13\10\07\1A\0C\12\06\0B\05\0A\09"> 0 [TF=5]
In function: foo
```

I think it didn't expect a constant pool to be created for the lookup table.

godbolt: https://godbolt.org/z/hx73s7TGG

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Craig Topper (topperc)

<details>
This test case generates an error with `llc -mtriple=mips -mattr=+mips16`

```
define i32 @<!-- -->foo(i32 %x) {
  %a = call i32 @<!-- -->llvm.cttz.i32(i32 %x, i1 true)
  ret i32 %a
}
```

```
LLVM ERROR: Cannot select: 0x214bf9c0: i32 = MipsISD::Lo TargetConstantPool:i32&lt;[32 x i8] c"\00\01\1C\02\1D\0E\18\03\1E\16\14\0F\19\11\04\08\1F\1B\0D\17\15\13\10\07\1A\0C\12\06\0B\05\0A\09"&gt; 0 [TF=5]
In function: foo
```

I think it didn't expect a constant pool to be created for the lookup table.
</details>


---

### Comment 2 - topperc

Adding some of the reviewers from the patch that added the table look up https://reviews.llvm.org/D128911  @efriedma-quic @davemgreen @s-barannikov 

---

### Comment 3 - s-barannikov

FWIW I think the table lookup should be removed. There issue with it is that the constant created by the expansion is unique within the function, not globally. It would be unified by the linker if it were placed in a mergeable section, but the constant is too large and it ends up in .rodata. Those who want to use table expansion should implement a libcall.


---

### Comment 4 - s-barannikov

As a side note, mips16 is broken and has [poor test coverage](https://github.com/llvm/llvm-project/tree/252c42354eca54274ed7b10c32c73c6937478e8b/llvm/test/MC/Mips/mips16).


---

### Comment 5 - efriedma-quic

> It would be unified by the linker if it were placed in a mergeable section, but the constant is too large and it ends up in .rodata. 

On ELF targets, 32-byte constants should end up in .rodata.cst32, which is mergeable.  (Well, except on 32-bit Arm; there's a lot of weird history there because before Thumb2 there were no good instructions for constant materialization.)  We don't have currently support .rodata.cst64, but I think that's just a missing bit in the compiler, not an inherent ELF limitation.

---

### Comment 6 - s-barannikov

> On ELF targets, 32-byte constants should end up in .rodata.cst32, which is mergeable. 

Hmm, you are right. I spoke from experience, but I neglected that my experience is based on a target that has 4x larger bytes...


---

### Comment 7 - topperc

> Those who want to use table expansion should implement a libcall.

If I understand correctly, AggressiveInstCombine unconditionally turns a user written table lookup into a cttz intrinsic and we need this table expansion to get back to roughly what the user wrote if the instruction isn't supported.

---

### Comment 8 - s-barannikov

It certainly folds ctpop, and I heard about plans of folding cttz, but didn't know it was implemented already.
Either way, the unconditional folding has a potential to break Linux build (if SelectionDAG expands the operation into a libcall), as was pointed out in comments to #101786 and earlier #99752, both reverted.
I hope I can find time to finish the work (unlikely before the winter).

---

### Comment 9 - topperc

> It certainly folds ctpop, and I heard about plans of folding cttz, but didn't know it was implemented already. Either way, the unconditional folding has a potential to break Linux build (if SelectionDAG expands the operation into a libcall), as was pointed out in comments to [#101786](https://github.com/llvm/llvm-project/pull/101786) and earlier [#99752](https://github.com/llvm/llvm-project/pull/99752), both reverted. I hope I can find time to finish the work (unlikely before the winter).

Here's the original review https://reviews.llvm.org/D113291 The table lookup appear in 531.deepsjeng_r in spec2017.

---

### Comment 10 - s-barannikov

I confused cttz with ctlz folding proposal
https://discourse.llvm.org/t/fold-counting-leading-zeros-logic-with-llvm-ctlz-intrinsic/86336


---

### Comment 11 - topperc

> I confused cttz with ctlz folding proposal https://discourse.llvm.org/t/fold-counting-leading-zeros-logic-with-llvm-ctlz-intrinsic/86336

I stumbled onto the mips16 crash by trying to implement table based ctlz expansion in the backend. There is a ctlz mips16 test but not cttz.

---

