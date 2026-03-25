# mlir-opt crash in --convert-arith-to-llvm: assertion failure in DenseIntOrFPElementsAttr with dynamic-shaped tensor

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/178415
**Status:** Closed
**Labels:** crash, mlir:arith
**Closed Date:** 2026-01-28T15:25:53Z

## Body

mlir-opt crashes with an assertion failure when running --convert-arith-to-llvm on a valid MLIR module containing arithmetic operations over dynamically shaped tensors.

```
module {
  func.func @main() {
    %0 = pdl_interp.create_type i32
    %1 = "emitc.variable"() <{value = #emitc.opaque<"">}> : () -> !emitc.lvalue<i32>
    %2 = gpu.block_dim  x
    %3 = shape.const_shape [1, 2, 3] : tensor<3xindex>
    %4 = smt.exists {
      ^bb0(%arg0: !smt.int):
      %5 = smt.eq %arg0, %arg0 : !smt.int
      smt.yield %5 : !smt.bool
    }
    %6 = tensor.generate %2 {
      ^bb0(%arg0: index):
      %7 = arith.index_cast %arg0 : index to i32
      tensor.yield %7 : i32
    } : tensor<?xi32>
    %8 = arith.floordivsi %6, %6 : tensor<?xi32>
    return
  }
}
```
```
mlir-opt --convert-arith-to-llvm test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/IR/BuiltinAttributes.cpp:1353: static mlir::DenseElementsAttr mlir::DenseIntOrFPElementsAttr::getRaw(mlir::ShapedType, llvm::ArrayRef<char>): Assertion `type.hasStaticShape() && "type must have static shape"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-arith-to-llvm test.mlir
 #0 0x00005eeafc1b3e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005eeafc1b05af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005eeafc1b06fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000728781a45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000728781a9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000728781a9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000728781a9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000728781a4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000728781a288ff abort ./stdlib/abort.c:81:7
 #9 0x0000728781a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000728781a3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```

