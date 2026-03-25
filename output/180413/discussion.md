# mlir-opt crash: hasRank() assertion failure in tosa.max_pool2d shape inference

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180413
**Status:** Closed
**Labels:** crash, mlir:tosa
**Closed Date:** 2026-02-14T05:28:04Z

## Body

**Bug Op**: tosa.max_pool2d

**Bug Pass**: --tosa-infer-shapes

**Detailed Description**: The mlir-opt tool crashes during the --tosa-infer-shapes pass when encountering a tosa.max_pool2d operation with an unranked input tensor (e.g., tensor<*xi32>).

```mlir
module {
  spirv.EntryPoint "GLCompute" @main
  func.func @main() {
    %0 = "tosa.const"() <{values = dense<0> : tensor<1xi32>}> : () -> tensor<1xi32>
    %1 = tosa.bitwise_not %0 : (tensor<1xi32>) -> tensor<1xi32>
    %2 = tosa.logical_right_shift %1, %0 : (tensor<1xi32>, tensor<1xi32>) -> tensor<1xi32>
    %3 = tosa.reduce_product %2 {axis = 0 : i32} : (tensor<1xi32>) -> tensor<1xi32>
    %4 = tensor.cast %3 : tensor<1xi32> to tensor<*xi32>
    %5 = tosa.max_pool2d %4 {kernel = array<i64: 1, 1>, pad = array<i64: 0, 0, 0, 0>, stride = array<i64: 1, 1>} : (tensor<*xi32>) -> tensor<*xi32>
    return
  }
}
```
```
mlir-opt --tosa-infer-shapes temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Interfaces/InferTypeOpInterface.cpp:121: int64_t mlir::ShapeAdaptor::getDimSize(int) const: Assertion `hasRank()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --tosa-infer-shapes temp.mlir
 #0 0x0000628e3b20de32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000628e3b20a5af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000628e3b20a6fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007aab5dc45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007aab5dc9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007aab5dc9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007aab5dc9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007aab5dc4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007aab5dc288ff abort ./stdlib/abort.c:81:7
 #9 0x00007aab5dc2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007aab5dc3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000628e42c58621 mlir::ShapeAdaptor::getDimSize(int) const (llvmorg-22.1.0-install/bin/mlir-opt+0x9f7a621)
#12 0x0000628e40e4ae95 poolingInferReturnTypes(mlir::ShapeAdaptor, llvm::ArrayRef<long>, llvm::ArrayRef<long>, llvm::ArrayRef<long>, llvm::SmallVectorImpl<mlir::ShapedTypeComponents>&) TosaOps.cpp:0:0
#13 0x0000628e40e4b539 mlir::tosa::MaxPool2dOp::inferReturnTypeComponents(mlir::MLIRContext*, std::optional<mlir::Location>, mlir::tosa::MaxPool2dOpAdaptor, llvm::SmallVectorImpl<mlir::ShapedTypeComponents>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x816d539)
#14 0x0000628e40e4b835 mlir::detail::InferShapedTypeOpInterfaceInterfaceTraits::Model<mlir::tosa::MaxPool2dOp>::inferReturnTypeComponents(mlir::MLIRContext*, std::optional<mlir::Location>, mlir::ValueShapeRange, mlir::DictionaryAttr, mlir::OpaqueProperties, mlir::RegionRange, llvm::SmallVectorImpl<mlir::ShapedTypeComponents>&) (llvmorg-22.1.0-install/bin/mlir-opt+0x816d835)
#15 0x0000628e3eb5e3ab (anonymous namespace)::propagateShapesInRegion(mlir::Region&, (anonymous namespace)::TypeModificationState&) TosaInferShapes.cpp:0:0
```

## Comments

### Comment 1 - EugeneZelenko

@xvv4402: If you plan to file more issues in future, it'll be good idea to request triage role ([examples](https://github.com/llvm/llvm-project/issues/?q=is%3Aissue%20label%3Ainfra%3Acommit-access-request%20Triage)), so you'll be able to label issues yourself, preferably at creation.

---

### Comment 2 - lhutton1

A fix related to this issue was merged a couple of weeks ago: https://github.com/llvm/llvm-project/pull/177999

---

### Comment 3 - EugeneZelenko

@Men-cotton: Should such fixes be backported to 22? If not, will be good idea to ask all MLIR issues submitters to work with `main` only.

---

### Comment 4 - Men-cotton

@EugeneZelenko 
I just got commit access, so I can't make the call on backporting. However, working with main only makes sense to me. 

---

### Comment 5 - EugeneZelenko

@xvv4402: Please run fuzzing-like testing on current `main`.

---

