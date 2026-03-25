# Crash when running pass pipeline with custom LowerSimpleVectorStrideReadPass (WSL)

**Author:** Confen
**URL:** https://github.com/llvm/llvm-project/issues/168179
**Status:** Closed
**Labels:** worksforme, mlir, crash
**Closed Date:** 2025-12-17T01:21:06Z

## Body

## Summary
Running a pass pipeline containing a custom lowering pass 'lower-simple-vector-stride-read' crashes on a minimal input using 'simple_vector.stride_read' on WSL. The tool is a custom mlir-opt-like driver 'simple-arith-opt2'.

## Reproducer
- Input file (/mnt/d/LLVMproject/simple_compiler/IR2/test/test_vector.mlir):
```mlir
module {
  func.func @test(%v: vector<8xf32>) -> vector<4xf32> {
    %r = simple_vector.stride_read %v offsets = [0], sizes = [4], strides = [2]
         : vector<8xf32> -> vector<4xf32>
    return %r : vector<4xf32>
  }
}
```

- Command:
```bash
/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2 /mnt/d/LLVMproject/simple_compiler/IR2/test/test_vector.mlir -pass-pipeline="builtin.module(func.func(lower-simple-vector-stride-read,canonicalize,cse))"
```

## Expected
Pipeline completes without a crash, producing valid IR.

## Actual
Full output (symbolized if LLVM_SYMBOLIZER_PATH is available):
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2 /mnt/d/LLVMproject/simple_compiler/IR2/test/test_vector.mlir -pass-pipeline=builtin.module(func.func(lower-simple-vector-stride-read,canonicalize,cse))
 #0 0x00006399bbde9e12 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0xcde12)
 #1 0x00006399bbde6c6f llvm::sys::RunSignalHandlers() (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0xcac6f)
 #2 0x00006399bbde6db5 SignalHandler(int) Signals.cpp:0:0
 #3 0x0000765a2a045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000765a2a09eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000765a2a09eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000765a2a09eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000765a2a04527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000765a2a0288ff abort ./stdlib/abort.c:81:7
 #9 0x00006399bbd9ba99 (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x7fa99)
#10 0x00006399bbe56c3a mlir::vector::ExtractOp mlir::OpBuilder::create<mlir::vector::ExtractOp, mlir::detail::TypedValue<mlir::VectorType>, mlir::ValueRange>(mlir::Location, mlir::detail::TypedValue<mlir::VectorType>&&, mlir::ValueRange&&) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x13ac3a)
#11 0x00006399bbe511b3 (anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation()::'lambda'(mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::ValueRange)::operator()(mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::ValueRange) const SimpleVectorDialect.cpp:0:0
#12 0x00006399bbe52021 void llvm::function_ref<void (mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::ValueRange)>::callback_fn<(anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation()::'lambda'(mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::ValueRange)>(long, mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::ValueRange) SimpleVectorDialect.cpp:0:0
#13 0x00006399bbf1563c mlir::scf::ForOp::build(mlir::OpBuilder&, mlir::OperationState&, mlir::Value, mlir::Value, mlir::Value, mlir::ValueRange, llvm::function_ref<void (mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::ValueRange)>) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x1f963c)
#14 0x00006399bbe51ada mlir::scf::ForOp mlir::OpBuilder::create<mlir::scf::ForOp, mlir::Value&, mlir::Value&, mlir::Value&, mlir::ValueRange, (anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation()::'lambda'(mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::ValueRange)>(mlir::Location, mlir::Value&, mlir::Value&, mlir::Value&, mlir::ValueRange&&, (anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation()::'lambda'(mlir::OpBuilder&, mlir::Location, mlir::Value, mlir::ValueRange)&&) SimpleVectorDialect.cpp:0:0
#15 0x00006399bbe518a6 (anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation() SimpleVectorDialect.cpp:0:0
#16 0x00006399bc278dbe mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55cdbe)
#17 0x00006399bc2793f0 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55d3f0)
#18 0x00006399bc279813 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::'lambda'(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&)::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&) const Pass.cpp:0:0
#19 0x00006399bc278425 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55c425)
#20 0x00006399bc278b72 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55cb72)
#21 0x00006399bc2793f0 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55d3f0)
#22 0x00006399bc27a3bd mlir::PassManager::run(mlir::Operation*) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55e3bd)
#23 0x00006399bbe667b7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#24 0x00006399bbe67833 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPool*) MlirOptMain.cpp:0:0
#25 0x00006399bbe6797c mlir::LogicalResult llvm::function_ref<mlir::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#26 0x00006399bc796ca7 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<mlir::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, bool, bool) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0xa7aca7)
#27 0x00006399bbe61262 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x145262)
#28 0x00006399bbe67cba mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x14bcba)
#29 0x00006399bbdb44f4 main (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x984f4)
#30 0x0000765a2a02a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x0000765a2a02a28b call_init ./csu/../csu/libc-start.c:128:20
#32 0x0000765a2a02a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#33 0x00006399bbdb43a5 _start (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x983a5)
```

## Environment
- OS (uname -a): Linux asen 6.6.87.2-microsoft-standard-WSL2 #1 SMP PREEMPT_DYNAMIC Thu Jun  5 18:30:46 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
- /etc/os-release:
```
PRETTY_NAME="Ubuntu 24.04.1 LTS"
NAME="Ubuntu"
VERSION_ID="24.04"
VERSION="24.04.1 LTS (Noble Numbat)"
VERSION_CODENAME=noble
ID=ubuntu
ID_LIKE=debian
HOME_URL="https://www.ubuntu.com/"
SUPPORT_URL="https://help.ubuntu.com/"
BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"
PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
UBUNTU_CODENAME=noble
LOGO=ubuntu-logo
```
- simple-arith-opt2 --version:
```
LLVM (http://llvm.org/):
  LLVM version 17.0.6
  Optimized build with assertions.

```
- llvm-config --version: llvm-config not found
- Compiler (clang): Ubuntu clang version 18.1.3 (1ubuntu1)
- Compiler (gcc): gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
- CMake: cmake version 3.28.3

- CMake cache highlights (from /mnt/d/LLVMproject/simple_compiler/IR2/build/CMakeCache.txt):
```
CMAKE_BUILD_TYPE:STRING=
CMAKE_CXX_COMPILER:FILEPATH=/usr/bin/c++
CMAKE_C_COMPILER:FILEPATH=/usr/bin/cc
LLVM_DIR:PATH=/home/asen/llvm-project/build/lib/cmake/llvm
MLIR_DIR:PATH=/home/asen/llvm-project/build/lib/cmake/mlir
```

## Notes
- Custom dialect: simple_vector
- Custom pass: lower-simple-vector-stride-read (depends on arith, vector, scf)
- Pipeline: builtin.module(func.func(lower-simple-vector-stride-read,canonicalize,cse))

If this is not actionable upstream, please advise best practices to validate pass output or diagnose crashes after canonicalize/cse.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Confen)

<details>
## Summary
Running a pass pipeline containing a custom lowering pass 'lower-simple-vector-stride-read' crashes on a minimal input using 'simple_vector.stride_read' on WSL. The tool is a custom mlir-opt-like driver 'simple-arith-opt2'.

## Reproducer
- Input file (/mnt/d/LLVMproject/simple_compiler/IR2/test/test_vector.mlir):
```mlir
module {
  func.func @<!-- -->test(%v: vector&lt;8xf32&gt;) -&gt; vector&lt;4xf32&gt; {
    %r = simple_vector.stride_read %v offsets = [0], sizes = [4], strides = [2]
         : vector&lt;8xf32&gt; -&gt; vector&lt;4xf32&gt;
    return %r : vector&lt;4xf32&gt;
  }
}
```

- Command:
```bash
/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2 /mnt/d/LLVMproject/simple_compiler/IR2/test/test_vector.mlir -pass-pipeline="builtin.module(func.func(lower-simple-vector-stride-read,canonicalize,cse))"
```

## Expected
Pipeline completes without a crash, producing valid IR.

## Actual
Full output (symbolized if LLVM_SYMBOLIZER_PATH is available):
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2 /mnt/d/LLVMproject/simple_compiler/IR2/test/test_vector.mlir -pass-pipeline=builtin.module(func.func(lower-simple-vector-stride-read,canonicalize,cse))
 #<!-- -->0 0x00006399bbde9e12 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0xcde12)
 #<!-- -->1 0x00006399bbde6c6f llvm::sys::RunSignalHandlers() (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0xcac6f)
 #<!-- -->2 0x00006399bbde6db5 SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x0000765a2a045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000765a2a09eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000765a2a09eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000765a2a09eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000765a2a04527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000765a2a0288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00006399bbd9ba99 (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x7fa99)
#<!-- -->10 0x00006399bbe56c3a mlir::vector::ExtractOp mlir::OpBuilder::create&lt;mlir::vector::ExtractOp, mlir::detail::TypedValue&lt;mlir::VectorType&gt;, mlir::ValueRange&gt;(mlir::Location, mlir::detail::TypedValue&lt;mlir::VectorType&gt;&amp;&amp;, mlir::ValueRange&amp;&amp;) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x13ac3a)
#<!-- -->11 0x00006399bbe511b3 (anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation()::'lambda'(mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::ValueRange)::operator()(mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::ValueRange) const SimpleVectorDialect.cpp:0:0
#<!-- -->12 0x00006399bbe52021 void llvm::function_ref&lt;void (mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::ValueRange)&gt;::callback_fn&lt;(anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation()::'lambda'(mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::ValueRange)&gt;(long, mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::ValueRange) SimpleVectorDialect.cpp:0:0
#<!-- -->13 0x00006399bbf1563c mlir::scf::ForOp::build(mlir::OpBuilder&amp;, mlir::OperationState&amp;, mlir::Value, mlir::Value, mlir::Value, mlir::ValueRange, llvm::function_ref&lt;void (mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::ValueRange)&gt;) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x1f963c)
#<!-- -->14 0x00006399bbe51ada mlir::scf::ForOp mlir::OpBuilder::create&lt;mlir::scf::ForOp, mlir::Value&amp;, mlir::Value&amp;, mlir::Value&amp;, mlir::ValueRange, (anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation()::'lambda'(mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::ValueRange)&gt;(mlir::Location, mlir::Value&amp;, mlir::Value&amp;, mlir::Value&amp;, mlir::ValueRange&amp;&amp;, (anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation()::'lambda'(mlir::OpBuilder&amp;, mlir::Location, mlir::Value, mlir::ValueRange)&amp;&amp;) SimpleVectorDialect.cpp:0:0
#<!-- -->15 0x00006399bbe518a6 (anonymous namespace)::LowerSimpleVectorStrideReadPass::runOnOperation() SimpleVectorDialect.cpp:0:0
#<!-- -->16 0x00006399bc278dbe mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55cdbe)
#<!-- -->17 0x00006399bc2793f0 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55d3f0)
#<!-- -->18 0x00006399bc279813 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::'lambda'(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;)::operator()(mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool)::OpPMInfo&amp;) const Pass.cpp:0:0
#<!-- -->19 0x00006399bc278425 mlir::detail::OpToOpPassAdaptor::runOnOperationAsyncImpl(bool) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55c425)
#<!-- -->20 0x00006399bc278b72 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55cb72)
#<!-- -->21 0x00006399bc2793f0 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55d3f0)
#<!-- -->22 0x00006399bc27a3bd mlir::PassManager::run(mlir::Operation*) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x55e3bd)
#<!-- -->23 0x00006399bbe667b7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->24 0x00006399bbe67833 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPool*) MlirOptMain.cpp:0:0
#<!-- -->25 0x00006399bbe6797c mlir::LogicalResult llvm::function_ref&lt;mlir::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->26 0x00006399bc796ca7 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;mlir::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, bool, bool) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0xa7aca7)
#<!-- -->27 0x00006399bbe61262 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x145262)
#<!-- -->28 0x00006399bbe67cba mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x14bcba)
#<!-- -->29 0x00006399bbdb44f4 main (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x984f4)
#<!-- -->30 0x0000765a2a02a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->31 0x0000765a2a02a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->32 0x0000765a2a02a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->33 0x00006399bbdb43a5 _start (/mnt/d/LLVMproject/simple_compiler/IR2/build/simple-arith-opt2+0x983a5)
```

## Environment
- OS (uname -a): Linux asen 6.6.87.2-microsoft-standard-WSL2 #<!-- -->1 SMP PREEMPT_DYNAMIC Thu Jun  5 18:30:46 UTC 2025 x86_64 x86_64 x86_64 GNU/Linux
- /etc/os-release:
```
PRETTY_NAME="Ubuntu 24.04.1 LTS"
NAME="Ubuntu"
VERSION_ID="24.04"
VERSION="24.04.1 LTS (Noble Numbat)"
VERSION_CODENAME=noble
ID=ubuntu
ID_LIKE=debian
HOME_URL="https://www.ubuntu.com/"
SUPPORT_URL="https://help.ubuntu.com/"
BUG_REPORT_URL="https://bugs.launchpad.net/ubuntu/"
PRIVACY_POLICY_URL="https://www.ubuntu.com/legal/terms-and-policies/privacy-policy"
UBUNTU_CODENAME=noble
LOGO=ubuntu-logo
```
- simple-arith-opt2 --version:
```
LLVM (http://llvm.org/):
  LLVM version 17.0.6
  Optimized build with assertions.

```
- llvm-config --version: llvm-config not found
- Compiler (clang): Ubuntu clang version 18.1.3 (1ubuntu1)
- Compiler (gcc): gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
- CMake: cmake version 3.28.3

- CMake cache highlights (from /mnt/d/LLVMproject/simple_compiler/IR2/build/CMakeCache.txt):
```
CMAKE_BUILD_TYPE:STRING=
CMAKE_CXX_COMPILER:FILEPATH=/usr/bin/c++
CMAKE_C_COMPILER:FILEPATH=/usr/bin/cc
LLVM_DIR:PATH=/home/asen/llvm-project/build/lib/cmake/llvm
MLIR_DIR:PATH=/home/asen/llvm-project/build/lib/cmake/mlir
```

## Notes
- Custom dialect: simple_vector
- Custom pass: lower-simple-vector-stride-read (depends on arith, vector, scf)
- Pipeline: builtin.module(func.func(lower-simple-vector-stride-read,canonicalize,cse))

If this is not actionable upstream, please advise best practices to validate pass output or diagnose crashes after canonicalize/cse.

</details>


---

### Comment 2 - swote-git

This is YOUR custom code crashing. Upstream MLIR has no `simple_vector` dialect, no `lower-simple-vector-stride-read` pass, and no `simple-arith-opt2` tool.
Without the actual implementation code, this is just a stack trace of your own bug. 

Required for this to be actionable:
- Full source of `SimpleVectorDialect.cpp`
- Operation definition of `simple_vector.stride_read`  
- Build instructions for your custom tool

Once the code is provided, we can determine if this is a bug in your pass implementation or an actual issue in MLIR's vector/scf dialects.

---

### Comment 3 - CoTinker

can't reproduce.

---

