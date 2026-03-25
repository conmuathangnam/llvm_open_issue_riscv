# [InstCombine] Assertion failed in visitBitCastBitwiseLogic due to type mismatch during constant folding

**Author:** varev-dev
**URL:** https://github.com/llvm/llvm-project/issues/179626
**Status:** Open
**Labels:** crash, llvm:instcombine

## Body

https://godbolt.org/z/q6TEvboYM
opt crashes with an assertion failure in InstCombine when processing a bitwise operation involving a bitcast of "exotic" vector type (e.g., casting <3 x i32> to <4 x i24>).
```c++
opt -passes=instcombine repro.ll
```
```llvm
define void @crash() {
  %LGV = load <3 x i32>, ptr addrspace(1) null, align 16
  %B1 = xor <3 x i32> %LGV, splat (i32 1)
  %1 = bitcast <3 x i32> %B1 to i96
  %2 = trunc i96 %1 to i24
  %3 = bitcast i24 %2 to <3 x i8>
  %B4 = lshr <3 x i8> zeroinitializer, %3
  store <3 x i8> %B4, ptr addrspace(1) null, align 4
  ret void
}
```

The crash occurs because the Constant Folding mechanism seems to incorrectly handle bitcasts for these vector types, returning a constant with a mismatching type (wrong element count/total bit width) compared to the requested destination type.

This triggers a type mismatch assertion when InstCombine attmepts to create the new binary operator.

**Crash Log**
>opt: .../llvm-project/llvm/lib/IR/Instructions.cpp:2707: static llvm::BinaryOperator* llvm::BinaryOperator::Create(llvm::Instruction::BinaryOps, llvm::Value*, llvm::Value*, const llvm::Twine&, llvm::InsertPosition): Assertion `S1->getType() == S2->getType() && "Cannot create binary operator with two operands of differing type!"' failed.


## Comments

### Comment 1 - varev-dev

**Root Cause**
Debugging revealed that `foldBitCastBitwiseLogic` in `InstCombineCast.cpp` attempts to optimize the bitcast by moving it past the bitwise operation. It calls `Builder.CreateBitCast(C, DestTy)` on the constant operand.

However, for the case of casting `<3 x i32>` to `<4 x i24>` (96 bits -> 96 bits), the Constant Folder returns a constant of type `<3 x i24>` (72 bits), incorrectly preserving the element count of the source instead of respecting the `DestTy` layout.

All of this results in `BinaryOperator::Create` receiving operands of differing types (`<4 x i24>` vs `<3 x i24>`), triggering an assertion.

**Proposed Fix**
Type guard in `visitBitCastBitwiseLogic`. It will ensure that if the constant folding returns a type that does not match the expected destination type, the transformation is aborted to prevent the crash.

---

