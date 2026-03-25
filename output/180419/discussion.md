# mlir-opt crash: SPIRVConversionTarget fails with invalid cast on non-SPIRV types

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180419
**Status:** Closed
**Labels:** mlir:spirv, crash
**Closed Date:** 2026-02-09T12:53:38Z

## Body

Bug Op: spirv.Variable (Triggering legalization check)

Bug Pass: --convert-cf-to-spirv

Detailed Description: The mlir-opt tool crashes during SPIR-V legalization. When the conversion target checks the legality of an operation (likely the spirv.Variable or surrounding context), it invokes SPIRVType::getExtensions. This utility internally performs an llvm::cast<mlir::spirv::SPIRVType> on all types involved in the operation.

```mlir
module {
  llvm.func @main() {
    %0 = llvm.dso_local_equivalent @main : !llvm.ptr
    %1 = smt.forall ["x"] {
      ^bb0(%arg0: !smt.int):
      %c1 = smt.int.constant 1
      %2 = smt.int.add %arg0, %c1
      %3 = smt.eq %2, %arg0 : !smt.int
      smt.yield %3 : !smt.bool
    }
    %4 = vector.constant_mask [1] : vector<1xi1>
    %5 = spirv.Variable : !spirv.ptr<vector<1xi1>, Function>
    llvm.return
  }
}
```
```
mlir-opt --convert-cf-to-spirv temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&) [with To = mlir::spirv::SPIRVType; From = mlir::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-cf-to-spirv temp.mlir
 #0 0x0000600594e68e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x0000600594e655af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x0000600594e656fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000079decc045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000079decc09eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000079decc09eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000079decc09eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000079decc04527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000079decc0288ff abort ./stdlib/abort.c:81:7
 #9 0x000079decc02881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000079decc03b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x0000600598fab848 (llvmorg-22.1.0-install/bin/mlir-opt+0x6672848)
#12 0x0000600598fad111 (anonymous namespace)::TypeExtensionVisitor::add(mlir::spirv::SPIRVType) SPIRVTypes.cpp:0:0
#13 0x0000600598fad34a mlir::spirv::SPIRVType::getExtensions(llvm::SmallVectorImpl<llvm::ArrayRef<mlir::spirv::Extension>>&, std::optional<mlir::spirv::StorageClass>) (llvmorg-22.1.0-install/bin/mlir-opt+0x667434a)
#14 0x0000600598b5bc41 mlir::SPIRVConversionTarget::isLegalOp(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x6222c41)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir-spirv

Author: None (xvv4402)

<details>
Bug Op: spirv.Variable (Triggering legalization check)

Bug Pass: --convert-cf-to-spirv

Detailed Description: The mlir-opt tool crashes during SPIR-V legalization. When the conversion target checks the legality of an operation (likely the spirv.Variable or surrounding context), it invokes SPIRVType::getExtensions. This utility internally performs an llvm::cast&lt;mlir::spirv::SPIRVType&gt; on all types involved in the operation.

```mlir
module {
  llvm.func @<!-- -->main() {
    %0 = llvm.dso_local_equivalent @<!-- -->main : !llvm.ptr
    %1 = smt.forall ["x"] {
      ^bb0(%arg0: !smt.int):
      %c1 = smt.int.constant 1
      %2 = smt.int.add %arg0, %c1
      %3 = smt.eq %2, %arg0 : !smt.int
      smt.yield %3 : !smt.bool
    }
    %4 = vector.constant_mask [1] : vector&lt;1xi1&gt;
    %5 = spirv.Variable : !spirv.ptr&lt;vector&lt;1xi1&gt;, Function&gt;
    llvm.return
  }
}
```
```
mlir-opt --convert-cf-to-spirv temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(From&amp;) [with To = mlir::spirv::SPIRVType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-cf-to-spirv temp.mlir
 #<!-- -->0 0x0000600594e68e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x0000600594e655af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x0000600594e656fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000079decc045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000079decc09eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000079decc09eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000079decc09eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000079decc04527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000079decc0288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000079decc02881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000079decc03b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x0000600598fab848 (llvmorg-22.1.0-install/bin/mlir-opt+0x6672848)
#<!-- -->12 0x0000600598fad111 (anonymous namespace)::TypeExtensionVisitor::add(mlir::spirv::SPIRVType) SPIRVTypes.cpp:0:0
#<!-- -->13 0x0000600598fad34a mlir::spirv::SPIRVType::getExtensions(llvm::SmallVectorImpl&lt;llvm::ArrayRef&lt;mlir::spirv::Extension&gt;&gt;&amp;, std::optional&lt;mlir::spirv::StorageClass&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x667434a)
#<!-- -->14 0x0000600598b5bc41 mlir::SPIRVConversionTarget::isLegalOp(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x6222c41)
```
</details>


---

### Comment 2 - kuhar

Thanks. I confirmed this crashes. Minimized repro:
```mlir
func.func @main() {
  %1 = spirv.Variable : !spirv.ptr<vector<1xi1>, Function>
  llvm.return
}
```

@xvv4402 In the future bug reports, could you please minimize your reproducers and include full stack traces? This would make it easier for maintainers to triage.

---

