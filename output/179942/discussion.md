# mlir-opt crash: Assertion failure in getMixedValues during affine.linearize_index folding with ub.poison

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/179942
**Status:** Closed
**Labels:** mlir:affine, crash
**Closed Date:** 2026-02-27T13:51:47Z

## Body

**Bug Op**: affine.linearize_index 

**Bug Pass**: --canonicalize

**Commenting out does not trigger the error**: emitc.rem

Detailed Description: The affine.linearize_index folder crashes when encountering ub.poison as a basis operand. 
The crash occurs in mlir::getMixedValues because the folder fails to maintain consistency between dynamic operands and static metadata when poison values are present. This triggers an assertion failure regarding the expected count of dynamic values.

```mlir
module {
  func.func @main() {
    %0 = gpu.cluster_dim  x
    %1 = ub.poison : index
    %2 = index.or %0, %1
    %3 = affine.linearize_index [%2] by (%1) : index
    %4 = emitc.rem %3, %1 : (index, index) -> index
    return
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Utils/StaticValueUtils.cpp:214: llvm::SmallVector<mlir::OpFoldResult> mlir::getMixedValues(llvm::ArrayRef<long int>, ValueRange, MLIRContext*): Assertion `dynamicValues.size() == static_cast<size_t>(llvm::count_if( staticValues, ShapedType::isDynamic)) && "expected the rank of dynamic values to match the number of " "values known to be dynamic"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #0 0x00005c6422743e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005c64227405af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005c64227406fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000790748645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000079074869eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000079074869eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000079074869eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000079074864527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007907486288ff abort ./stdlib/abort.c:81:7
 #9 0x000079074862881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000079074863b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005c6429e812ee (llvmorg-22.1.0-install/bin/mlir-opt+0x9c6d2ee)
#12 0x00005c6429e81314 mlir::getMixedValues(llvm::ArrayRef<long>, mlir::ValueRange, mlir::Builder&) (llvmorg-22.1.0-install/bin/mlir-opt+0x9c6d314)
#13 0x00005c6429bc658e mlir::affine::AffineLinearizeIndexOp::getMixedBasis() (llvmorg-22.1.0-install/bin/mlir-opt+0x99b258e)
#14 0x00005c6429bf0536 mlir::affine::AffineLinearizeIndexOp::fold(mlir::affine::AffineLinearizeIndexOpGenericAdaptor<llvm::ArrayRef<mlir::Attribute>>) (llvmorg-22.1.0-install/bin/mlir-opt+0x99dc536)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir-affine

Author: None (compilerStudyer)

<details>
**Bug Op**: affine.linearize_index 

**Bug Pass**: --canonicalize

**Commenting out does not trigger the error**: emitc.rem

Detailed Description: The affine.linearize_index folder crashes when encountering ub.poison as a basis operand. 
The crash occurs in mlir::getMixedValues because the folder fails to maintain consistency between dynamic operands and static metadata when poison values are present. This triggers an assertion failure regarding the expected count of dynamic values.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = gpu.cluster_dim  x
    %1 = ub.poison : index
    %2 = index.or %0, %1
    %3 = affine.linearize_index [%2] by (%1) : index
    %4 = emitc.rem %3, %1 : (index, index) -&gt; index
    return
  }
}
```
```
mlir-opt --canonicalize temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Dialect/Utils/StaticValueUtils.cpp:214: llvm::SmallVector&lt;mlir::OpFoldResult&gt; mlir::getMixedValues(llvm::ArrayRef&lt;long int&gt;, ValueRange, MLIRContext*): Assertion `dynamicValues.size() == static_cast&lt;size_t&gt;(llvm::count_if( staticValues, ShapedType::isDynamic)) &amp;&amp; "expected the rank of dynamic values to match the number of " "values known to be dynamic"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --canonicalize temp.mlir
 #<!-- -->0 0x00005c6422743e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005c64227405af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005c64227406fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000790748645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000079074869eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000079074869eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000079074869eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000079074864527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007907486288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000079074862881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000079074863b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005c6429e812ee (llvmorg-22.1.0-install/bin/mlir-opt+0x9c6d2ee)
#<!-- -->12 0x00005c6429e81314 mlir::getMixedValues(llvm::ArrayRef&lt;long&gt;, mlir::ValueRange, mlir::Builder&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9c6d314)
#<!-- -->13 0x00005c6429bc658e mlir::affine::AffineLinearizeIndexOp::getMixedBasis() (llvmorg-22.1.0-install/bin/mlir-opt+0x99b258e)
#<!-- -->14 0x00005c6429bf0536 mlir::affine::AffineLinearizeIndexOp::fold(mlir::affine::AffineLinearizeIndexOpGenericAdaptor&lt;llvm::ArrayRef&lt;mlir::Attribute&gt;&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x99dc536)
```
</details>


---

### Comment 2 - joker-eph

Fixed in 98825908fc51f0226b38381520ba2a4f5f1f36e9

---

