# mlir-opt crash: materializeTargetConversion assertion in XeGPU to XeVM conversion

**Author:** xvv4402
**URL:** https://github.com/llvm/llvm-project/issues/180716
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-14T06:05:54Z

## Body

**Bug Op**: scf.while (with vector block arguments)

**Bug Pass**: --convert-xegpu-to-xevm

**Detailed Description**: The mlir-opt tool crashes during the XeGPU to XeVM conversion process. The assertion failure occurs in DialectConversion.cpp when the TypeConverter fails to correctly materialize a target type for an scf.while loop's carry values. Specifically, when the loop uses vector<1x1xi1>, the conversion callback produces a result that does not match the expected type range, violating the internal consistency of the conversion framework.

```mlir
module {
  func.func @main() {
    %0 = vector.constant_mask [1, 1] : vector<2x2xi1>
    %1 = pdl.type : vector<2x2xi1>
    %2 = vector.extract_strided_slice %0 {offsets = [0, 0], sizes = [1, 1], strides = [1, 1]} : vector<2x2xi1> to vector<1x1xi1>
    %3 = scf.while (%arg0 = %2) : (vector<1x1xi1>) -> vector<1x1xi1> {
      %4 = arith.constant true
      scf.condition(%4) %arg0 : vector<1x1xi1>
    } do {
      ^bb0(%arg0: vector<1x1xi1>):
      scf.yield %arg0 : vector<1x1xi1>
    }
    return
  }
}
```
```
mlir-opt --convert-xegpu-to-xevm temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Transforms/Utils/DialectConversion.cpp:3756: llvm::SmallVector<mlir::Value> mlir::TypeConverter::materializeTargetConversion(mlir::OpBuilder&, mlir::Location, mlir::TypeRange, mlir::ValueRange, mlir::Type) const: Assertion `TypeRange(ValueRange(result)) == resultTypes && "callback produced incorrect number of values or values with " "incorrect types"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-xegpu-to-xevm temp.mlir
 #0 0x000055a18371ce32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000055a1837195af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000055a1837196fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007efde8445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007efde849eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007efde849eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007efde849eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007efde844527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007efde84288ff abort ./stdlib/abort.c:81:7
 #9 0x00007efde842881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007efde843b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000055a18aee1769 mlir::TypeConverter::materializeTargetConversion(mlir::OpBuilder&, mlir::Location, mlir::TypeRange, mlir::ValueRange, mlir::Type) const (llvmorg-22.1.0-install/bin/mlir-opt+0x9cf4769)
#12 0x000055a18af00afb mlir::OperationConverter::applyConversion(llvm::ArrayRef<mlir::Operation*>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d13afb)
#13 0x000055a18af012ab applyConversion(llvm::ArrayRef<mlir::Operation*>, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#14 0x000055a18af0144f mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&, mlir::FrozenRewritePatternSet const&, mlir::ConversionConfig) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d1444f)
#15 0x000055a18728711d (anonymous namespace)::ConvertXeGPUToXeVMPass::runOnOperation() XeGPUToXeVM.cpp:0:0
#16 0x000055a18afbc756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#17 0x000055a18afbca8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#18 0x000055a18afbd1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#19 0x000055a18afbe400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#20 0x000055a1837e3767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#21 0x000055a1837e44ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#22 0x000055a18b38c731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#23 0x000055a1837db8d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#24 0x000055a1837e4b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#25 0x000055a1837e4da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#26 0x000055a183616023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#27 0x00007efde842a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#28 0x00007efde842a28b call_init ./csu/../csu/libc-start.c:128:20
#29 0x00007efde842a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#30 0x000055a1836fb405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (xvv4402)

<details>
**Bug Op**: scf.while (with vector block arguments)

**Bug Pass**: --convert-xegpu-to-xevm

**Detailed Description**: The mlir-opt tool crashes during the XeGPU to XeVM conversion process. The assertion failure occurs in DialectConversion.cpp when the TypeConverter fails to correctly materialize a target type for an scf.while loop's carry values. Specifically, when the loop uses vector&lt;1x1xi1&gt;, the conversion callback produces a result that does not match the expected type range, violating the internal consistency of the conversion framework.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = vector.constant_mask [1, 1] : vector&lt;2x2xi1&gt;
    %1 = pdl.type : vector&lt;2x2xi1&gt;
    %2 = vector.extract_strided_slice %0 {offsets = [0, 0], sizes = [1, 1], strides = [1, 1]} : vector&lt;2x2xi1&gt; to vector&lt;1x1xi1&gt;
    %3 = scf.while (%arg0 = %2) : (vector&lt;1x1xi1&gt;) -&gt; vector&lt;1x1xi1&gt; {
      %4 = arith.constant true
      scf.condition(%4) %arg0 : vector&lt;1x1xi1&gt;
    } do {
      ^bb0(%arg0: vector&lt;1x1xi1&gt;):
      scf.yield %arg0 : vector&lt;1x1xi1&gt;
    }
    return
  }
}
```
```
mlir-opt --convert-xegpu-to-xevm temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/mlir/lib/Transforms/Utils/DialectConversion.cpp:3756: llvm::SmallVector&lt;mlir::Value&gt; mlir::TypeConverter::materializeTargetConversion(mlir::OpBuilder&amp;, mlir::Location, mlir::TypeRange, mlir::ValueRange, mlir::Type) const: Assertion `TypeRange(ValueRange(result)) == resultTypes &amp;&amp; "callback produced incorrect number of values or values with " "incorrect types"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --convert-xegpu-to-xevm temp.mlir
 #<!-- -->0 0x000055a18371ce32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000055a1837195af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000055a1837196fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007efde8445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007efde849eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007efde849eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007efde849eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007efde844527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007efde84288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007efde842881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007efde843b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000055a18aee1769 mlir::TypeConverter::materializeTargetConversion(mlir::OpBuilder&amp;, mlir::Location, mlir::TypeRange, mlir::ValueRange, mlir::Type) const (llvmorg-22.1.0-install/bin/mlir-opt+0x9cf4769)
#<!-- -->12 0x000055a18af00afb mlir::OperationConverter::applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d13afb)
#<!-- -->13 0x000055a18af012ab applyConversion(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig, (anonymous namespace)::OpConversionMode) DialectConversion.cpp:0:0
#<!-- -->14 0x000055a18af0144f mlir::applyPartialConversion(mlir::Operation*, mlir::ConversionTarget const&amp;, mlir::FrozenRewritePatternSet const&amp;, mlir::ConversionConfig) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d1444f)
#<!-- -->15 0x000055a18728711d (anonymous namespace)::ConvertXeGPUToXeVMPass::runOnOperation() XeGPUToXeVM.cpp:0:0
#<!-- -->16 0x000055a18afbc756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#<!-- -->17 0x000055a18afbca8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#<!-- -->18 0x000055a18afbd1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#<!-- -->19 0x000055a18afbe400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#<!-- -->20 0x000055a1837e3767 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->21 0x000055a1837e44ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->22 0x000055a18b38c731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->23 0x000055a1837db8d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->24 0x000055a1837e4b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->25 0x000055a1837e4da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->26 0x000055a183616023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->27 0x00007efde842a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->28 0x00007efde842a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->29 0x00007efde842a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->30 0x000055a1836fb405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

</details>


---

### Comment 2 - EugeneZelenko

@hstk30-hw: Please also set `Type` to `Bug` for crashes.

---

### Comment 3 - Ayush3941

hey @xvv4402 i tried to reproduce the crash but it seems to be working fine on current main maybe the bug is fixed or am i missing something ?

<img width="1366" height="768" alt="Image" src="https://github.com/user-attachments/assets/eafb895a-881b-47dc-a445-3a7a8980e10a" />

---

### Comment 4 - hstk30-hw

Maybe fixed by @CoTinker , check it. 
https://github.com/llvm/llvm-project/pull/180741
https://github.com/llvm/llvm-project/pull/180737

---

### Comment 5 - Men-cotton

I also confirmed the repro indeed doesn't crash on the current main branch.
I believe PR https://github.com/llvm/llvm-project/pull/178558 is the one that actually resolved this crash.

---

### Comment 6 - EugeneZelenko

@hstk30-hw: Should fix be backported to 22? Or MLIR users should rely on `main`?

---

### Comment 7 - hstk30-hw

Sry, I don't know the release calendar. 
I try to do some Issue Triage task for LLVM, not sure how to handle this case。
Could you please advise what the recommended approach is?

---

### Comment 8 - EugeneZelenko

@hstk30-hw: Frankly, I don't know. If such fuzzing reveals corner cases that unlikely to be found in real worlds, fuzzing should be run on current `main`. If not - will be good idea to backport to 22.

---

