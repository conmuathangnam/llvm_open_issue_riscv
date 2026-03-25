# [LV] operands with different vscale types

**Author:** sjoerdmeijer
**URL:** https://github.com/llvm/llvm-project/issues/162530
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-11-06T21:45:59Z

## Body

Compiling this reproducer:

```
short a;
extern short b[];
void c(bool d[][1][1]) {
#pragma clang loop vectorize_predicate(enable)
  for (short e = 0; e < 858; e += 3) {
    b[e] = d[0644][e][1];
    for (char f = 0; f < 21; f++)
      a += d[0644][e][f];
  }
}
```

with `-O2 -mcpu=grace` results in:

```
 #4 0x000000000518a9aa llvm::SelectionDAG::isConstantIntBuildVectorOrConstantInt(llvm::SDValue, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518a9aa)
 #5 0x000000000518ab8b llvm::SelectionDAG::canonicalizeCommutativeBinop(unsigned int, llvm::SDValue&, llvm::SDValue&) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x518ab8b)
 #6 0x00000000051ad64d llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&, llvm::EVT, llvm::SDValue, llvm::SDValue, llvm::SDNodeFlags) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51ad64d)
 #7 0x000000000533de2e llvm::DAGTypeLegalizer::PromoteIntRes_SimpleIntBinOp(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x533de2e)
 #8 0x000000000535821b llvm::DAGTypeLegalizer::PromoteIntegerResult(llvm::SDNode*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x535821b)
 #9 0x00000000052771da llvm::DAGTypeLegalizer::run() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x52771da)
#10 0x0000000005277821 llvm::SelectionDAG::LegalizeTypes() 
```

See also: https://godbolt.org/z/vz87Koh4q

I've tried creating an IR reproducer, but ran into some troubles that might be hint what is going on: `%20` is a`8 x i16`, which is used in an add in `%26` and mixed with a `4 x i16` operand:

```
 ...
  %20 = zext nneg <vscale x 8 x i8> %19 to <vscale x 8 x i16>
  %21 = getelementptr inbounds nuw i16, ptr @b, <vscale x 8 x i64> %15
  tail call void @llvm.masked.scatter.nxv8i16.nxv8p0(<vscale x 8 x i16> %20, <vscale x 8 x ptr> %21, i32 2, <vscale x 8 x i1> %14)
  %22 = tail call <vscale x 8 x i8> @llvm.masked.gather.nxv8i8.nxv8p0(<vscale x 8 x ptr> %17, i32 1, <vscale x 8 x i1> %14, <vscale x 8 x i8> poison)
  %23 = zext nneg <vscale x 8 x i8> %22 to <vscale x 8 x i16>
  %24 = select <vscale x 8 x i1> %14, <vscale x 8 x i16> %23, <vscale x 8 x i16> zeroinitializer
  %25 = tail call <vscale x 4 x i16> @llvm.vector.partial.reduce.add.nxv4i16.nxv8i16(<vscale x 4 x i16> %16, <vscale x 8 x i16> %24)
  %26 = add <vscale x 4 x i16> %25, <vscale x 8 x i16> %20
...
```



## Comments

### Comment 1 - sjoerdmeijer

CC: @paulwalker-arm , @david-arm , @sdesmalen-arm , @fhahn 

---

### Comment 2 - topperc

An assertions build fails earlier in IRBuilder https://godbolt.org/z/zadvTnne8

---

### Comment 3 - sdesmalen-arm

@SamTebbs33 it seems the result from the partial reduction here is a `<vscale x 4 x i16>`, whereas the use (an `add`) of this value assumes a `<vscale x 8 x i16>`. Can you investigate?

---

### Comment 4 - fhahn

IR reproducer: https://llvm.godbolt.org/z/Mz5xe8vK1

---

### Comment 5 - fhahn

@sjoerdmeijer you might want to use `-Xclang -llvm-verify-each` if you are not building with assertions. This makes it a bit easier to pin-point which pass creates invalid IR.

---

