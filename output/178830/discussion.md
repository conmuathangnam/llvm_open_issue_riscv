# [MLIR] GPU lowering pipeline causes a crash due to optional compilation target handling

**Author:** tisble
**URL:** https://github.com/llvm/llvm-project/issues/178830
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2026-02-28T23:08:27Z

## Body

Hi, I find a crash bug when lowering a gpu program. See the minimized code example below.

```llvm
module attributes {gpu.container_module} {
    gpu.module @kernels {
        gpu.func @func1() kernel {
            %0 = gpu.thread_id x
            %csti8 = arith.constant 2 : i8
            %cstf32 = arith.constant 3.0 : f32
            gpu.printf "%lld, %d, %f\n", %0, %csti8, %cstf32  : index, i8, f32
            gpu.return
        }
    }

    func.func @main() {
        %c2 = arith.constant 2 : index
        %c1 = arith.constant 1 : index
        gpu.launch_func @kernels::@func1
            blocks in (%c1, %c1, %c1)
            threads in (%c2, %c1, %c1)
        return
    }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-lower-to-nvvm-pipeline=cubin-format=%gpu_compilation_format
 #0 0x000056552d2c761d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000056552d2c7b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x000056552d2c5a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000056552d2c81e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007fb9c1e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007fb9c1e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007fb9c1e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007fb9c1e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007fb9c1e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007fb9c1e288ff abort ./stdlib/abort.c:81:7
#10 0x00007fb9c22df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#11 0x000056553029bd54 std::_Optional_base_impl<mlir::gpu::CompilationTarget, std::_Optional_base<mlir::gpu::CompilationTarget, true, true>>::_M_get() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:479:2
#12 0x000056553029b755 std::optional<mlir::gpu::CompilationTarget>::operator*() & /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:976:9
#13 0x0000565530299b97 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:68:43
#14 0x000056553ac8e864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#15 0x000056553ac8e805 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#16 0x000056552d2ed879 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#17 0x000056553ac91f2b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#18 0x000056553ac85cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#19 0x000056553ac863da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#20 0x000056553ac8bccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#21 0x000056553ac8b256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#22 0x000056552d3c849b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#23 0x000056552d3c7b71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#24 0x000056552d3c782c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#25 0x000056552d3c7736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#26 0x000056553b1d0cba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#27 0x000056553b1d007a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#28 0x000056552d3c3126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#29 0x000056552d3c3525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#30 0x000056552d3c36f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#31 0x000056552d29c935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#32 0x00007fb9c1e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#33 0x00007fb9c1e2a28b call_init ./csu/../csu/libc-start.c:128:20
#34 0x00007fb9c1e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#35 0x000056552d29c505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the command: `mlir-opt input.mlir -gpu-lower-to-nvvm-pipeline="cubin-format=%gpu_compilation_format"`

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (tisble)

<details>
Hi, I find a crash bug when lowering a gpu program. See the minimized code example below.

```llvm
module attributes {gpu.container_module} {
    gpu.module @<!-- -->kernels {
        gpu.func @<!-- -->func1() kernel {
            %0 = gpu.thread_id x
            %csti8 = arith.constant 2 : i8
            %cstf32 = arith.constant 3.0 : f32
            gpu.printf "%lld, %d, %f\n", %0, %csti8, %cstf32  : index, i8, f32
            gpu.return
        }
    }

    func.func @<!-- -->main() {
        %c2 = arith.constant 2 : index
        %c1 = arith.constant 1 : index
        gpu.launch_func @<!-- -->kernels::@<!-- -->func1
            blocks in (%c1, %c1, %c1)
            threads in (%c2, %c1, %c1)
        return
    }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-lower-to-nvvm-pipeline=cubin-format=%gpu_compilation_format
 #<!-- -->0 0x000056552d2c761d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000056552d2c7b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x000056552d2c5a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000056552d2c81e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007fb9c1e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007fb9c1e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fb9c1e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fb9c1e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fb9c1e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fb9c1e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007fb9c22df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#<!-- -->11 0x000056553029bd54 std::_Optional_base_impl&lt;mlir::gpu::CompilationTarget, std::_Optional_base&lt;mlir::gpu::CompilationTarget, true, true&gt;&gt;::_M_get() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:479:2
#<!-- -->12 0x000056553029b755 std::optional&lt;mlir::gpu::CompilationTarget&gt;::operator*() &amp; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:976:9
#<!-- -->13 0x0000565530299b97 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:68:43
#<!-- -->14 0x000056553ac8e864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->15 0x000056553ac8e805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->16 0x000056552d2ed879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->17 0x000056553ac91f2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->18 0x000056553ac85cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->19 0x000056553ac863da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->20 0x000056553ac8bccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->21 0x000056553ac8b256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->22 0x000056552d3c849b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->23 0x000056552d3c7b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->24 0x000056552d3c782c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->25 0x000056552d3c7736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->26 0x000056553b1d0cba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->27 0x000056553b1d007a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->28 0x000056552d3c3126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->29 0x000056552d3c3525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->30 0x000056552d3c36f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->31 0x000056552d29c935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->32 0x00007fb9c1e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x00007fb9c1e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x00007fb9c1e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->35 0x000056552d29c505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the command: `mlir-opt input.mlir -gpu-lower-to-nvvm-pipeline="cubin-format=%gpu_compilation_format"`
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (tisble)

<details>
Hi, I find a crash bug when lowering a gpu program. See the minimized code example below.

```llvm
module attributes {gpu.container_module} {
    gpu.module @<!-- -->kernels {
        gpu.func @<!-- -->func1() kernel {
            %0 = gpu.thread_id x
            %csti8 = arith.constant 2 : i8
            %cstf32 = arith.constant 3.0 : f32
            gpu.printf "%lld, %d, %f\n", %0, %csti8, %cstf32  : index, i8, f32
            gpu.return
        }
    }

    func.func @<!-- -->main() {
        %c2 = arith.constant 2 : index
        %c1 = arith.constant 1 : index
        gpu.launch_func @<!-- -->kernels::@<!-- -->func1
            blocks in (%c1, %c1, %c1)
            threads in (%c2, %c1, %c1)
        return
    }
}
```

### Stack Trace
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir -gpu-lower-to-nvvm-pipeline=cubin-format=%gpu_compilation_format
 #<!-- -->0 0x000056552d2c761d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000056552d2c7b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x000056552d2c5a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000056552d2c81e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007fb9c1e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007fb9c1e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007fb9c1e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007fb9c1e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007fb9c1e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007fb9c1e288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007fb9c22df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#<!-- -->11 0x000056553029bd54 std::_Optional_base_impl&lt;mlir::gpu::CompilationTarget, std::_Optional_base&lt;mlir::gpu::CompilationTarget, true, true&gt;&gt;::_M_get() /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:479:2
#<!-- -->12 0x000056553029b755 std::optional&lt;mlir::gpu::CompilationTarget&gt;::operator*() &amp; /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/optional:976:9
#<!-- -->13 0x0000565530299b97 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:68:43
#<!-- -->14 0x000056553ac8e864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->15 0x000056553ac8e805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->16 0x000056552d2ed879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->17 0x000056553ac91f2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->18 0x000056553ac85cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->19 0x000056553ac863da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->20 0x000056553ac8bccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->21 0x000056553ac8b256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->22 0x000056552d3c849b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->23 0x000056552d3c7b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->24 0x000056552d3c782c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->25 0x000056552d3c7736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->26 0x000056553b1d0cba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->27 0x000056553b1d007a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->28 0x000056552d3c3126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->29 0x000056552d3c3525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->30 0x000056552d3c36f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->31 0x000056552d29c935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->32 0x00007fb9c1e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->33 0x00007fb9c1e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->34 0x00007fb9c1e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->35 0x000056552d29c505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

I used the command: `mlir-opt input.mlir -gpu-lower-to-nvvm-pipeline="cubin-format=%gpu_compilation_format"`
</details>


---

### Comment 3 - joker-eph

Fixed at HEAD. It will error with "error: Invalid format specified." because the command line invocation is incorrect.

---

### Comment 4 - joker-eph

I also just improved the error message to:
> error: Invalid format specified: '%gpu_compilation_format' (expected one of: offloading, llvm, assembly, isa, binary, bin, fatbinary, fatbin)

---

