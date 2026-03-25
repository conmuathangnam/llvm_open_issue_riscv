# mlir-opt crash: IntegerAttr cast failure in coalesceLoops during loop coalescing

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/181590
**Status:** Closed
**Labels:** crash, mlir:scf
**Closed Date:** 2026-02-19T18:43:36Z

## Body

Bug Pass: --affine-loop-coalescing (calls SCF coalescing utilities)

Bug Op: scf.for

Description:
The mlir-opt tool crashes when attempting to coalesce nested scf.for loops. The failure occurs in getValueOrCreateConstantIntOp when the transformation assumes loop parameters (bounds or steps) can be folded into constant integers. If these parameters are dynamic or fail to fold into an IntegerAttr, the code performs an unsafe llvm::cast, triggering an assertion failure.

```mlir
module {
  func.func @main() {
    %0 = gpu.subgroup_id : index
    %c0 = arith.constant 0 : index
    %1 = scf.for %i = %c0 to %c0 step %c0 iter_args(%arg0 = %c0) -> (index) {
      %idx = scf.for %j = %c0 to %c0 step %c0 iter_args(%arg1 = %arg0) -> (index) {
        scf.yield %j : index
      }
      scf.yield %idx : index
    }
    return
  }
}
```
```
mlir-opt --affine-loop-coalescing test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --affine-loop-coalescing test.mlir
 #0 0x000055b74e45be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000055b74e4585af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000055b74e4586fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x000073eb24e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000073eb24e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000073eb24e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000073eb24e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000073eb24e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x000073eb24e288ff abort ./stdlib/abort.c:81:7
 #9 0x000073eb24e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000073eb24e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000055b7559f6e37 mlir::getValueOrCreateConstantIntOp(mlir::OpBuilder&, mlir::Location, mlir::OpFoldResult) (llvmorg-22.1.0-install/bin/mlir-opt+0x9acae37)
#12 0x000055b7546dfde8 mlir::coalesceLoops(mlir::RewriterBase&, llvm::MutableArrayRef<mlir::scf::ForOp>) (llvmorg-22.1.0-install/bin/mlir-opt+0x87b3de8)
#13 0x000055b7546e2037 mlir::coalescePerfectlyNestedSCFForLoops(mlir::scf::ForOp) (llvmorg-22.1.0-install/bin/mlir-opt+0x87b6037)
#14 0x000055b754692d2a void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.0) LoopCoalescing.cpp:0:0
#15 0x000055b754692d98 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.0) LoopCoalescing.cpp:0:0
#16 0x000055b754692f36 (anonymous namespace)::LoopCoalescingPass::runOnOperation() LoopCoalescing.cpp:0:0
#17 0x000055b755cfb756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#18 0x000055b755cfba8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#19 0x000055b755cfd673 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::'lambda'(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&)::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&) const Pass.cpp:0:0
#20 0x000055b755cfa72d mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dce72d)
#21 0x000055b755cfb5f0 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf5f0)
#22 0x000055b755cfba8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#23 0x000055b755cfc1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#24 0x000055b755cfd400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#25 0x000055b74e522767 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#26 0x000055b74e5234ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#27 0x000055b7560cb731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#28 0x000055b74e51a8d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#29 0x000055b74e523b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#30 0x000055b74e523da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#31 0x000055b74e355023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#32 0x000073eb24e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#33 0x000073eb24e2a28b call_init ./csu/../csu/libc-start.c:128:20
#34 0x000073eb24e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#35 0x000055b74e43a405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
Bug Pass: --affine-loop-coalescing (calls SCF coalescing utilities)

Bug Op: scf.for

Description:
The mlir-opt tool crashes when attempting to coalesce nested scf.for loops. The failure occurs in getValueOrCreateConstantIntOp when the transformation assumes loop parameters (bounds or steps) can be folded into constant integers. If these parameters are dynamic or fail to fold into an IntegerAttr, the code performs an unsafe llvm::cast, triggering an assertion failure.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = gpu.subgroup_id : index
    %c0 = arith.constant 0 : index
    %1 = scf.for %i = %c0 to %c0 step %c0 iter_args(%arg0 = %c0) -&gt; (index) {
      %idx = scf.for %j = %c0 to %c0 step %c0 iter_args(%arg1 = %arg0) -&gt; (index) {
        scf.yield %j : index
      }
      scf.yield %idx : index
    }
    return
  }
}
```
```
mlir-opt --affine-loop-coalescing test.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:560: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::IntegerAttr; From = mlir::Attribute]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --affine-loop-coalescing test.mlir
 #<!-- -->0 0x000055b74e45be32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000055b74e4585af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000055b74e4586fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x000073eb24e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000073eb24e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000073eb24e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000073eb24e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000073eb24e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x000073eb24e288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000073eb24e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000073eb24e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x000055b7559f6e37 mlir::getValueOrCreateConstantIntOp(mlir::OpBuilder&amp;, mlir::Location, mlir::OpFoldResult) (llvmorg-22.1.0-install/bin/mlir-opt+0x9acae37)
#<!-- -->12 0x000055b7546dfde8 mlir::coalesceLoops(mlir::RewriterBase&amp;, llvm::MutableArrayRef&lt;mlir::scf::ForOp&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x87b3de8)
#<!-- -->13 0x000055b7546e2037 mlir::coalescePerfectlyNestedSCFForLoops(mlir::scf::ForOp) (llvmorg-22.1.0-install/bin/mlir-opt+0x87b6037)
#<!-- -->14 0x000055b754692d2a void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) LoopCoalescing.cpp:0:0
#<!-- -->15 0x000055b754692d98 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) LoopCoalescing.cpp:0:0
#<!-- -->16 0x000055b754692f36 (anonymous namespace)::LoopCoalescingPass::runOnOperation() LoopCoalescing.cpp:0:0
#<!-- -->17 0x000055b755cfb756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#<!-- -->18 0x000055b755cfba8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#<!-- -->19 0x000055b755cfd673 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::'lambda'(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;)::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const Pass.cpp:0:0
#<!-- -->20 0x000055b755cfa72d mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dce72d)
#<!-- -->21 0x000055b755cfb5f0 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf5f0)
#<!-- -->22 0x000055b755cfba8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
#<!-- -->23 0x000055b755cfc1e2 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd01e2)
#<!-- -->24 0x000055b755cfd400 mlir::PassManager::run(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dd1400)
#<!-- -->25 0x000055b74e522767 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->26 0x000055b74e5234ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->27 0x000055b7560cb731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->28 0x000055b74e51a8d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->29 0x000055b74e523b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->30 0x000055b74e523da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->31 0x000055b74e355023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->32 0x000073eb24e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x000073eb24e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x000073eb24e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->35 0x000055b74e43a405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

