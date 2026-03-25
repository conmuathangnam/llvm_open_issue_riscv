# mlir-opt crash: processFuncOp assertion failure in --remove-dead-values when a function is used by spirv.EntryPoint

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180416
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-27T15:44:10Z

## Body

**Bug Pass**: --remove-dead-values

**Bug Op**: spirv.EntryPoint

**Internal Component**: RemoveDeadValues.cpp

**Detailed Description**: The mlir-opt tool crashes during the --remove-dead-values pass. The issue lies in the processFuncOp function, which assumes that any operation referencing a function must implement CallOpInterface

```mlir
module {
  spirv.EntryPoint "GLCompute" @main_func
  llvm.func @main_func() attributes {sym_visibility = "private"} {
    llvm.return
  }
}
```
```
mlir-opt --remove-dead-values temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Transforms/RemoveDeadValues.cpp:305: void {anonymous}::processFuncOp(mlir::FunctionOpInterface, mlir::Operation*, mlir::dataflow::RunLivenessAnalysis&, mlir::DenseSet<mlir::Value>&, RDVFinalCleanupList&): Assertion `isa<CallOpInterface>(callOp) && "expected a call-like user"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --remove-dead-values temp.mlir
 #0 0x000064c8ea7aae32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000064c8ea7a75af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000064c8ea7a76fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f9fde645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007f9fde69eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f9fde69eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f9fde69eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f9fde64527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f9fde6288ff abort ./stdlib/abort.c:81:7
 #9 0x00007f9fde62881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007f9fde63b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000064c8f1f22f0e (anonymous namespace)::processFuncOp(mlir::FunctionOpInterface, mlir::Operation*, mlir::dataflow::RunLivenessAnalysis&, llvm::DenseSet<mlir::Value, llvm::DenseMapInfo<mlir::Value, void>>&, (anonymous namespace)::RDVFinalCleanupList&) RemoveDeadValues.cpp:0:0
#12 0x000064c8f1f22ff8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.1) RemoveDeadValues.cpp:0:0
#13 0x000064c8f1f233e8 (anonymous namespace)::RemoveDeadValues::runOnOperation() RemoveDeadValues.cpp:0:0
#14 0x000064c8f204a756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**Bug Pass**: --remove-dead-values

**Bug Op**: spirv.EntryPoint

**Internal Component**: RemoveDeadValues.cpp

**Detailed Description**: The mlir-opt tool crashes during the --remove-dead-values pass. The issue lies in the processFuncOp function, which assumes that any operation referencing a function must implement CallOpInterface

```mlir
module {
  spirv.EntryPoint "GLCompute" @<!-- -->main_func
  llvm.func @<!-- -->main_func() attributes {sym_visibility = "private"} {
    llvm.return
  }
}
```
```
mlir-opt --remove-dead-values temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Transforms/RemoveDeadValues.cpp:305: void {anonymous}::processFuncOp(mlir::FunctionOpInterface, mlir::Operation*, mlir::dataflow::RunLivenessAnalysis&amp;, mlir::DenseSet&lt;mlir::Value&gt;&amp;, RDVFinalCleanupList&amp;): Assertion `isa&lt;CallOpInterface&gt;(callOp) &amp;&amp; "expected a call-like user"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --remove-dead-values temp.mlir
 #<!-- -->0 0x000064c8ea7aae32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000064c8ea7a75af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000064c8ea7a76fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f9fde645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f9fde69eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f9fde69eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f9fde69eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f9fde64527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f9fde6288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f9fde62881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007f9fde63b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000064c8f1f22f0e (anonymous namespace)::processFuncOp(mlir::FunctionOpInterface, mlir::Operation*, mlir::dataflow::RunLivenessAnalysis&amp;, llvm::DenseSet&lt;mlir::Value, llvm::DenseMapInfo&lt;mlir::Value, void&gt;&gt;&amp;, (anonymous namespace)::RDVFinalCleanupList&amp;) RemoveDeadValues.cpp:0:0
#<!-- -->12 0x000064c8f1f22ff8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.1) RemoveDeadValues.cpp:0:0
#<!-- -->13 0x000064c8f1f233e8 (anonymous namespace)::RemoveDeadValues::runOnOperation() RemoveDeadValues.cpp:0:0
#<!-- -->14 0x000064c8f204a756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
```
</details>


---

### Comment 2 - JueonPark

I made a fix PR regarding this:
https://github.com/llvm/llvm-project/pull/181155

---

