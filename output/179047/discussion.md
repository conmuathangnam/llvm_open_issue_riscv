# mlir-opt crash: Assertion failure isa<To>(Val) in GenericAtomicRMWOp::parse when operand is a tensor instead of memref

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/179047
**Status:** Open
**Labels:** mlir, crash

## Body

mlir-opt version: llvmorg-22.1.0
A crash occurs in **mlir-opt** during parsing of the **memref.generic_atomic_rmw** operation. The parser attempts to llvm::cast<MemRefType> the input operand's type without prior validation. If the operand is a tensor (e.g., produced by tosa.variable_read), the cast fails and triggers an assertion.
```mlir
module {
  func.func @main() {
    tosa.variable @var_x : tensor<10xf32>
    %0 = emitc.literal "M_PI" : f32
    %1 = arith.fptosi %0 : f32 to i32
    %2 = ptr.type_offset f32 : index
    %3 = vector.vscale
    %4 = index.shru %2, %3
    %5 = tosa.variable_read @var_x : tensor<10xf32>
    memref.generic_atomic_rmw %5[%4] : tensor<10xf32> {
      ^bb0(%arg0: f32):
        %6 = arith.sitofp %1 : i32 to f32
        memref.atomic_yield %6 : f32
    }
    return
  }
}
```
```
mlir-opt test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::MemRefType; From = mlir::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt test.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'memref.generic_atomic_rmw'
 #0 0x0000631425209e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00006314252065af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00006314252066fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000073ba22445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000073ba2249eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000073ba2249eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000073ba2249eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000073ba2244527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000073ba224288ff abort ./stdlib/abort.c:81:7
 #9 0x000073ba2242881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000073ba2243b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
mlir-opt version: llvmorg-22.1.0
A crash occurs in **mlir-opt** during parsing of the **memref.generic_atomic_rmw** operation. The parser attempts to llvm::cast&lt;MemRefType&gt; the input operand's type without prior validation. If the operand is a tensor (e.g., produced by tosa.variable_read), the cast fails and triggers an assertion.
```
module {
  func.func @<!-- -->main() {
    tosa.variable @<!-- -->var_x : tensor&lt;10xf32&gt;
    %0 = emitc.literal "M_PI" : f32
    %1 = arith.fptosi %0 : f32 to i32
    %2 = ptr.type_offset f32 : index
    %3 = vector.vscale
    %4 = index.shru %2, %3
    %5 = tosa.variable_read @<!-- -->var_x : tensor&lt;10xf32&gt;
    memref.generic_atomic_rmw %5[%4] : tensor&lt;10xf32&gt; {
      ^bb0(%arg0: f32):
        %6 = arith.sitofp %1 : i32 to f32
        memref.atomic_yield %6 : f32
    }
    return
  }
}
```
```
mlir-opt test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&amp;) [with To = mlir::MemRefType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt test.mlir
1.      MLIR Parser: custom op parser 'builtin.module'
2.      MLIR Parser: custom op parser 'func.func'
3.      MLIR Parser: custom op parser 'memref.generic_atomic_rmw'
 #<!-- -->0 0x0000631425209e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00006314252065af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00006314252066fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000073ba22445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000073ba2249eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000073ba2249eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000073ba2249eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000073ba2244527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000073ba224288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000073ba2242881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000073ba2243b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
```
</details>


---

### Comment 2 - 0xzre

hi, i'd take on this one

---

### Comment 3 - sweiglbosker

@xvv4402 if these are fuzzer generated, can you mention?

---

