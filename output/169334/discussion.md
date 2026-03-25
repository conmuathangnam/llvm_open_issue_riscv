# [LV] Constant::getSplatValue: Assertion `this->getType()->isVectorTy() && "Only valid for vectors!"' failed.

**Author:** david-arm
**URL:** https://github.com/llvm/llvm-project/issues/169334
**Status:** Closed
**Labels:** vectorizers, crash
**Closed Date:** 2025-12-01T08:12:42Z

## Body

While trying to rewrite the test 'both' in test/Transforms/LoopVectorize/iv_outside_user.ll to look like this:

```
define ptr @both(ptr %p, i32 %k)  {
entry:
  %base = getelementptr inbounds i32, ptr %p, i64 1
  br label %for.body

for.body:
  %inc.phi = phi i32 [ 0, %entry ], [ %inc, %for.body ]
  %inc.lag1 = phi ptr [ %base, %entry ], [ %tmp, %for.body]
  %inc.lag2 = phi ptr [ %base, %entry ], [ %inc.lag1, %for.body]
  %tmp = getelementptr inbounds i32, ptr %inc.lag1, i64 1
  %inc = add nsw i32 %inc.phi, 1
  %cmp = icmp eq i32 %inc, %k
  br i1 %cmp, label %for.end, label %for.body

for.end:
  ret ptr %inc.lag1
}
```

I encountered this assert and backtrace:

```
llvm::Constant* llvm::Constant::getSplatValue(bool) const: Assertion `this->getType()->isVectorTy() && "Only valid for vectors!"' failed.
...
 #8 0x0000aaaab9e2fba4 llvm::Constant::getSplatValue(bool) const (./bin/opt+0x4c2fba4)
 #9 0x0000aaaab9dfb844 llvm::ConstantFoldBinaryInstruction(unsigned int, llvm::Constant*, llvm::Constant*) (./bin/opt+0x4bfb844)
#10 0x0000aaaab8235dd0 llvm::IRBuilderBase::CreateBinOpFMF(llvm::Instruction::BinaryOps, llvm::Value*, llvm::Value*, llvm::FMFSource, llvm::Twine const&, llvm::MDNode*) (.constprop.0) VPlanRecipes.cpp:0:0
#11 0x0000aaaab8248338 llvm::VPInstruction::generate(llvm::VPTransformState&) (./bin/opt+0x3048338)
#12 0x0000aaaab824a5c0 llvm::VPInstruction::execute(llvm::VPTransformState&) (./bin/opt+0x304a5c0)
#13 0x0000aaaab81f7988 llvm::VPBasicBlock::executeRecipes(llvm::VPTransformState*, llvm::BasicBlock*) (./bin/opt+0x2ff7988)
#14 0x0000aaaab820422c llvm::VPBasicBlock::execute(llvm::VPTransformState*) (./bin/opt+0x300422c)
```

when using the command:

```
opt -S -passes=loop-vectorize -force-vector-interleave=2 -force-vector-width=1 < iv_outside_user.ll
```

The vplan for VF=1 looks like this:

```
vector.body:
  EMIT-SCALAR vp<%index> = phi [ ir<0>, vector.ph ], [ vp<%index.next>, vector.body ]
  EMIT-SCALAR vp<%pointer.phi> = phi [ ir<%base>, vector.ph ], [ vp<%ptr.ind>, vector.body ]
  EMIT vp<%7> = step-vector i64
  EMIT vp<%8> = mul vp<%7>, ir<4>
  EMIT vp<%vector.gep> = wide-ptradd vp<%pointer.phi>, vp<%8>
  EMIT vp<%step.add> = wide-ptradd vp<%vector.gep>, vp<%6>
  EMIT vp<%index.next> = add nuw vp<%index>, ir<2>
  EMIT vp<%ptr.ind> = ptradd vp<%pointer.phi>, ir<8>
  EMIT branch-on-count vp<%index.next>, vp<%n.vec>
Successor(s): middle.block, vector.body
```

The problem happens when generating code for the mul VPInstruction, since we are attempting to multiply `<1 x i64> zeroinitializer` by `i64 4`. It looks like this is the problematic code:

```
  if (Instruction::isBinaryOp(getOpcode())) {
    bool OnlyFirstLaneUsed = vputils::onlyFirstLaneUsed(this);
    Value *A = State.get(getOperand(0), OnlyFirstLaneUsed);
    Value *B = State.get(getOperand(1), OnlyFirstLaneUsed);
```

Presumably OnlyFirstLaneUsed should be true here?


## Comments

### Comment 1 - david-arm

cc @fhahn @lukel97

---

### Comment 2 - lukel97

I guess this must have snuck in recently, I can't seem to reproduce it on 86a82f27ee8cb7477c25561bc5697a6a6a8ff21d but I can on ad0acf4af001a3781b41b572788adcd7d652d18a

---

### Comment 3 - david-arm

I think I may have a fix for this, but be good to verify this first:

```
@@ -1360,7 +1360,7 @@ bool VPInstruction::usesFirstLaneOnly(const VPValue *Op) const {
     return Op == getOperand(0) || vputils::onlyFirstLaneUsed(this);
   case VPInstruction::WidePtrAdd:
     // WidePtrAdd supports scalar and vector base addresses.
-    return false;
+    return vputils::onlyFirstLaneUsed(this);;
   case VPInstruction::ComputeAnyOfResult:
   case VPInstruction::ComputeFindIVResult:
     return Op == getOperand(1);
```

---

### Comment 4 - david-arm

I created this issue in case there is already a fix upstream that I don't know about!

---

