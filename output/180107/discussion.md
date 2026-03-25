# mlir-opt crash: xegpu.dpas conversion to xeVM fails with unsupported xevm::ElemType

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180107
**Status:** Closed
**Labels:** crash, mlir:xegpu
**Closed Date:** 2026-03-25T14:37:20Z

## Body

**MLIR Version**: 22.1.0

**Bug Op**: xegpu.dpas 

**Bug Pass**: --convert-xegpu-to-xevm

**Detailed Description**: The mlir-opt tool hits a llvm_unreachable during the conversion of xegpu.dpas to xeVM. The crash occurs in DpasToXeVMPattern when the operation uses i32 element types. The converter triggers "unsupported xevm::ElemType" and terminates, indicating that the lowering path for i32 DPAS operations is either unimplemented or missing type validation.

```mlir
module {
  func.func @main() {
    %0 = spirv.Constant dense<0> : vector<4xi32>
    %1 = vector.constant_mask [4] : vector<4xi1>
    %2 = llvm.add %0, %0 : vector<4xi32>
    %3 = spirv.SLessThanEqual %2, %0 : vector<4xi32>
    %4 = spirv.Not %2 : vector<4xi32>
    %5 = xegpu.dpas %2, %0 : vector<4xi32>, vector<4xi32> -> vector<4xi32>
    return
  }
}
```
```
mlir-opt --convert-xegpu-to-xevm temp.mlir
```
```
unsupported xevm::ElemType
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Conversion/XeGPUToXeVM/XeGPUToXeVM.cpp:969!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-xegpu-to-xevm temp.mlir
 #0 0x000059c2de758e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000059c2de7555af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000059c2de7556fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000072870f445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000072870f49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000072870f49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000072870f49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000072870f44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000072870f4288ff abort ./stdlib/abort.c:81:7
 #9 0x000059c2de73812e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#10 0x000059c2e22cb816 (anonymous namespace)::DpasToXeVMPattern::matchAndRewrite(mlir::xegpu::DpasOp, mlir::xegpu::DpasOpAdaptor, mlir::ConversionPatternRewriter&) const XeGPUToXeVM.cpp:0:0
#11 0x000059c2e22c70ec llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::OpConversionPattern<mlir::xegpu::DpasOp>, mlir::xegpu::DpasOp>(mlir::OpConversionPattern<mlir::xegpu::DpasOp> const&, mlir::xegpu::DpasOp, mlir::xegpu::DpasOp::GenericAdaptor<llvm::ArrayRef<mlir::ValueRange>>, mlir::ConversionPatternRewriter&) (llvmorg-22.1.0-install/bin/mlir-opt+0x609e0ec)
#12 0x000059c2e22c723e mlir::OpConversionPattern<mlir::xegpu::DpasOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionPatternRewriter&) const (llvmorg-22.1.0-install/bin/mlir-opt+0x609e23e)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**MLIR Version**: 22.1.0

**Bug Op**: xegpu.dpas 

**Bug Pass**: --convert-xegpu-to-xevm

**Detailed Description**: The mlir-opt tool hits a llvm_unreachable during the conversion of xegpu.dpas to xeVM. The crash occurs in DpasToXeVMPattern when the operation uses i32 element types. The converter triggers "unsupported xevm::ElemType" and terminates, indicating that the lowering path for i32 DPAS operations is either unimplemented or missing type validation.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = spirv.Constant dense&lt;0&gt; : vector&lt;4xi32&gt;
    %1 = vector.constant_mask [4] : vector&lt;4xi1&gt;
    %2 = llvm.add %0, %0 : vector&lt;4xi32&gt;
    %3 = spirv.SLessThanEqual %2, %0 : vector&lt;4xi32&gt;
    %4 = spirv.Not %2 : vector&lt;4xi32&gt;
    %5 = xegpu.dpas %2, %0 : vector&lt;4xi32&gt;, vector&lt;4xi32&gt; -&gt; vector&lt;4xi32&gt;
    return
  }
}
```
```
mlir-opt --convert-xegpu-to-xevm temp.mlir
```
```
unsupported xevm::ElemType
UNREACHABLE executed at llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Conversion/XeGPUToXeVM/XeGPUToXeVM.cpp:969!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-xegpu-to-xevm temp.mlir
 #<!-- -->0 0x000059c2de758e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000059c2de7555af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000059c2de7556fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000072870f445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000072870f49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000072870f49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000072870f49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000072870f44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000072870f4288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000059c2de73812e (llvmorg-22.1.0-install/bin/mlir-opt+0x250f12e)
#<!-- -->10 0x000059c2e22cb816 (anonymous namespace)::DpasToXeVMPattern::matchAndRewrite(mlir::xegpu::DpasOp, mlir::xegpu::DpasOpAdaptor, mlir::ConversionPatternRewriter&amp;) const XeGPUToXeVM.cpp:0:0
#<!-- -->11 0x000059c2e22c70ec llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::OpConversionPattern&lt;mlir::xegpu::DpasOp&gt;, mlir::xegpu::DpasOp&gt;(mlir::OpConversionPattern&lt;mlir::xegpu::DpasOp&gt; const&amp;, mlir::xegpu::DpasOp, mlir::xegpu::DpasOp::GenericAdaptor&lt;llvm::ArrayRef&lt;mlir::ValueRange&gt;&gt;, mlir::ConversionPatternRewriter&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x609e0ec)
#<!-- -->12 0x000059c2e22c723e mlir::OpConversionPattern&lt;mlir::xegpu::DpasOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionPatternRewriter&amp;) const (llvmorg-22.1.0-install/bin/mlir-opt+0x609e23e)
```
</details>


---

### Comment 2 - abhamra

I would like to be assigned to this issue, I'm able to reproduce the error. 

---

### Comment 3 - abhamra

Quick question; I noticed that the tablegen for XeVM's `ElemType` defines it as follows:
`def XeVM_ElemType : AnyTypeOf<[AnyI8, AnyI16, AnyI32, F32, TF32, F16, BF16]>;`
However, the function that hits the unreachable:
```cpp
  static unsigned getNumOperandsPerDword(xevm::ElemType pTy) {
    switch (pTy) {
    case xevm::ElemType::TF32:
      return 1;
    case xevm::ElemType::BF16:
    case xevm::ElemType::F16:
      return 2;
    case xevm::ElemType::U8:
    case xevm::ElemType::S8:
      return 4;
    default:
      llvm_unreachable("unsupported xevm::ElemType");
    }
  }
```
Doesn't handle the I32 case even though it's present in the ElemType enum. Perhaps this is a silly question, but is the preferred solution here to add a guard for ElemType and use `notifyMatchError/failure()` or simply support the remaining variants of the enum? It seems from the [xegpu dialect's description of `dpas`](https://mlir.llvm.org/docs/Dialects/XeGPU/#xegpudpas-xegpudpasop) that `dpas` should be able to support i32s.

---

### Comment 4 - abhamra

@Jianhui-Li pinging because I believe you're a maintainer for XeGPU, any answers to the above questions would be very helpful.

---

### Comment 5 - adam-smnk

FYI @silee2 

In hardware, DPAS doesn't support i32 inputs, it only accumulates into i32.
But yes, lowering isn't too graceful and op verifier should be revisited.

---

### Comment 6 - abhamra

> In hardware, DPAS doesn't support i32 inputs, it only accumulates into i32. But yes, lowering isn't too graceful and op verifier should be revisited.

Gotcha, is a sufficient fix just to emit errors when `DpasOp::verify` encounters `lhs` and `rhs` that are >= 32 bits? I'll work on this as my current assumption, just want to be extra sure.

---

### Comment 7 - adam-smnk

> Gotcha, is a sufficient fix just to emit errors when `DpasOp::verify` encounters `lhs` and `rhs` that are >= 32 bits? I'll work on this as my current assumption, just want to be extra sure.

This should be enough to make the op easier/safer to use.
Ideally, tablegen types for `dpas` should be adjusted that we get the type check and better documentation for free.



---

### Comment 8 - abhamra

@adam-smnk  I changed the `DpasOp` to use the following types instead:
```ods
def XeGPU_DpasOprElemType : AnyTypeOf<[AnyI1, I<4>, AnyI8, AnyI16,
                                       F4E2M1FN, F8E4M3FN, F8E5M2, F8E8M0FNU, F16, BF16]>;
def XeGPU_DpasResElemType : AnyTypeOf<[XeGPU_DpasOprElemType, AnyI32, TF32, F32]>;
```
Which generates a type error for the above case (good!), however when changing the testcase to use `i16`s, it still fails with unreachables in the XeGPU to XeVM conversion in `encodePrecision`:
```cpp
    auto encodePrecision = [&](Type type) -> xevm::ElemType {
      if (type == rewriter.getBF16Type())
        return xevm::ElemType::BF16;
      else if (type == rewriter.getF16Type())
        return xevm::ElemType::F16;
      else if (type == rewriter.getTF32Type())
        return xevm::ElemType::TF32;
      else if (type.isInteger(8)) {
        if (type.isUnsignedInteger())
          return xevm::ElemType::U8;
        return xevm::ElemType::S8;
      } else if (type == rewriter.getF32Type())
        return xevm::ElemType::F32;
      else if (type.isInteger(32))
        return xevm::ElemType::S32;
      llvm_unreachable("add more support for ElemType");
    };
```
Based on the [`XeVM::MMAOp`'s description](https://mlir.llvm.org/docs/Dialects/XeVMDialect/#xevmmma-xevmmmaop), it seems like it should support `a` and `b` inputs containing `i16`s, but both `encodePrecision` and eventually `getNumOperandsPerDword` would fail at unreachable with this input.

Should there be additional support for types in the lowering from Dpas Op to XeVM's MMA, or should I just `notifyMatchError` (preferable over the unreachable), since it's indeed not allowed by hardware? Thanks!

---

### Comment 9 - adam-smnk

XeVM definitions probably have the same weak type checking.
AFAIK, `i16` inputs are not supported (only 16-bit types are f16 and bf16) or not always. But my memory could be missing sth.
It'd have to be verified against hardware specs and/or @Jianhui-Li who has better knowledge about these details.

---

### Comment 10 - silee2

> FYI [@silee2](https://github.com/silee2)
> 
> In hardware, DPAS doesn't support i32 inputs, it only accumulates into i32. But yes, lowering isn't too graceful and op verifier should be revisited.

Agree. Op verifier should be revisited.



---

### Comment 11 - silee2

> XeVM definitions probably have the same weak type checking. AFAIK, `i16` inputs are not supported (only 16-bit types are f16 and bf16) or not always. But my memory could be missing sth. It'd have to be verified against hardware specs and/or [@Jianhui-Li](https://github.com/Jianhui-Li) who has better knowledge about these details.

At XeVM level - which is LLVM - wrapper types can be used instead of real types.
Like a combination of wrapper type + type enum.
XeVM ElemType is currently loosely defined as any type, wrapper or real type, that are used as operand or result element type.
I would prefer to improve op verifier instead of introducing a bunch of XeVM <SomeOp>ElemType in tablegen file per op.
As XeVM dialect maintainer, I'll do a full review of the current supported types for individual XeVM and harden op verifiers in the coming weeks. 



---

### Comment 12 - abhamra

@adam-smnk @silee2 sounds good. Should I still create a PR to handle this specific case based on our discussion? Depending on whether you think the type change is too heavy handed or not, my original plan was to add something like the following to the `DpasOp::verify()` function instead:
```cpp
  auto lhsElemTy = lhsType.getElementType();
  auto rhsElemTy = rhsType.getElementType();
  if (lhsElemTy.getIntOrFloatBitWidth() >= 32 ||
      rhsElemTy.getIntOrFloatBitWidth() >= 32) {
    return emitOpError(
        "Expecting lhs and rhs element types to be at most 32 bits.");
  }
```

Let me know what you think, I'd love to continue working on this.

---

### Comment 13 - silee2

> [@adam-smnk](https://github.com/adam-smnk) [@silee2](https://github.com/silee2) sounds good. Should I still create a PR to handle this specific case based on our discussion? Depending on whether you think the type change is too heavy handed or not, my original plan was to add something like the following to the `DpasOp::verify()` function instead:
> 
>   auto lhsElemTy = lhsType.getElementType();
>   auto rhsElemTy = rhsType.getElementType();
>   if (lhsElemTy.getIntOrFloatBitWidth() >= 32 ||
>       rhsElemTy.getIntOrFloatBitWidth() >= 32) {
>     return emitOpError(
>         "Expecting lhs and rhs element types to be at most 32 bits.");
>   }
> Let me know what you think, I'd love to continue working on this.

@abhamra First of all, thank you for your contribution.
XeVM is the lowest level dialect for Intel GPU, and a lot of the semantics directly depends on HW features. In an ideal world with proper documentation, contribution from outside would be easier but unfortunately, in reality, at this stage any update to XeVM dialect may still rely on access to non-public resources for future proofing the dialect.

---

### Comment 14 - silee2

@abhamra Updating just the `verify()` function doesn't seem like the proper solution for long term. Type definitions in `.td` file need some update as well. As I mentioned above, will review the dialect in coming weeks and then decide on how to address the issues.
Meanwhile, any bug reports or suggestions are welcome.
Thanks again for your contributions!


---

### Comment 15 - silee2

@abhamra My bad, you can ignore my previous comments, I thought your plan was about update to XeVM. For contributing to XeGPU, please check with @Jianhui-Li 

---

### Comment 16 - Jianhui-Li

@abhamra  Thanks for the discussion.  
We were thinking to validate the type support using uArch interface. See it here: 
https://github.com/llvm/llvm-project/blob/f409804208e93ccf0fd10cc805c55f799126f38e/mlir/include/mlir/Dialect/XeGPU/uArch/IntelGpuXe2.h#L336

The element type and verifiers of XeGPU OPs may seem a bit too loose, but that is not right place to fix the issue cleanly, as each uArch may have it own support matrix. 

Actually for the bug reported, potentially it can be fixed in the caller of getNumOperandsPerDword(), where it first check uArch to bail out if the type combination are not supported for Dpas op. 


---

### Comment 17 - abhamra

@Jianhui-Li sounds good! I think I have a PR ready based on the `getSupportedTypes` checks you suggested; it will likely need some work but hopefully it moves in the right direction. I will cc you on that PR.

In related work, are there any plans to remove the llvm_unreachable's from the `XeGPUToXeVM.cpp` file? 

---

### Comment 18 - abhamra

Also @silee2 thank you for the additional info/context surrounding Xe!

---

