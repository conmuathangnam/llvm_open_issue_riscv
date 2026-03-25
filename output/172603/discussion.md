# [MLIR] A crash bug during dialect lowering

**Author:** RJerrica
**URL:** https://github.com/llvm/llvm-project/issues/172603
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2025-12-18T12:58:31Z

## Body

Hi, I found a crash bug during using MLIR to lower programs. See the minimized code exampel below:

File Name: **input.mlir**
```llvm
module attributes {gpu.container_module} {
  func.func @f(%arg0: tensor<4x4xf32>) {
    %c128_i64 = llvm.mlir.constant(128 : i64) : i64
    %c1_i64   = llvm.mlir.constant(1 : i64)   : i64
    %c128     = builtin.unrealized_conversion_cast %c128_i64 : i64 to index
    %c1       = builtin.unrealized_conversion_cast %c1_i64   : i64 to index

    %wait_token   = gpu.wait async
    %launch_token = gpu.launch_func async [%wait_token] @k::@k
      blocks in (%c1, %c1, %c1)
      threads in (%c1, %c1, %c1)
      args(%c128 : index)
    return
  }

  gpu.module @k {
    gpu.func @k(%arg0: index) kernel attributes {known_block_size = array<i32: 1, 1, 1>} {
      gpu.return
    }
  }
}
```

I used the command `mlir-opt input.mlir -test-symbol-uses -convert-gpu-to-nvvm -convert-scf-to-cf` to compile it, and got the following crash log:

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /mnt/raid/home/username/bin/mlir/bin/mlir-opt input.mlir -test-symbol-uses -convert-gpu-to-nvvm -convert-scf-to-cf
 #0 0x0000560da26e8508 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b3f508)
 #1 0x0000560da26e5c35 llvm::sys::RunSignalHandlers() (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b3cc35)
 #2 0x0000560da26e9611 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f4d92045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007f4d9209eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f4d9209eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007f4d9209eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007f4d9204527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007f4d920288ff abort ./stdlib/abort.c:81:7
 #9 0x00007f4d924df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#10 0x0000560da2eae289 mlir::WalkResult llvm::function_ref<mlir::WalkResult (mlir::Operation*)>::callback_fn<(anonymous namespace)::SymbolUsesPass::runOnOperation()::'lambda'(mlir::Operation*)>(long, mlir::Operation*) TestSymbolUses.cpp:0:0
#11 0x0000560da27a18d7 mlir::WalkResult mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Operation*)>, mlir::WalkOrder) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bf88d7)
#12 0x0000560da2ead2aa (anonymous namespace)::SymbolUsesPass::runOnOperation() TestSymbolUses.cpp:0:0
#13 0x0000560da9957bc1 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8daebc1)
#14 0x0000560da9958c07 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8dafc07)
#15 0x0000560da99601a3 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8db71a3)
#16 0x0000560da995f7b2 mlir::PassManager::run(mlir::Operation*) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8db67b2)
#17 0x0000560da278d603 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#18 0x0000560da278c864 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#19 0x0000560da9c94928 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x90eb928)
#20 0x0000560da2781dac mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bd8dac)
#21 0x0000560da27820e6 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bd90e6)
#22 0x0000560da27822f2 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bd92f2)
#23 0x0000560da26d024f main (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b2724f)
#24 0x00007f4d9202a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#25 0x00007f4d9202a28b call_init ./csu/../csu/libc-start.c:128:20
#26 0x00007f4d9202a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#27 0x0000560da26cfd75 _start (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b26d75)
Aborted
```
My MLIR version is:
```bash
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (RJerrica)

<details>
Hi, I found a crash bug during using MLIR to lower programs. See the minimized code exampel below:

File Name: **input.mlir**
```llvm
module attributes {gpu.container_module} {
  func.func @<!-- -->f(%arg0: tensor&lt;4x4xf32&gt;) {
    %c128_i64 = llvm.mlir.constant(128 : i64) : i64
    %c1_i64   = llvm.mlir.constant(1 : i64)   : i64
    %c128     = builtin.unrealized_conversion_cast %c128_i64 : i64 to index
    %c1       = builtin.unrealized_conversion_cast %c1_i64   : i64 to index

    %wait_token   = gpu.wait async
    %launch_token = gpu.launch_func async [%wait_token] @<!-- -->k::@<!-- -->k
      blocks in (%c1, %c1, %c1)
      threads in (%c1, %c1, %c1)
      args(%c128 : index)
    return
  }

  gpu.module @<!-- -->k {
    gpu.func @<!-- -->k(%arg0: index) kernel attributes {known_block_size = array&lt;i32: 1, 1, 1&gt;} {
      gpu.return
    }
  }
}
```

I used the command `mlir-opt input.mlir -test-symbol-uses -convert-gpu-to-nvvm -convert-scf-to-cf` to compile it, and got the following crash log:

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /mnt/raid/home/username/bin/mlir/bin/mlir-opt input.mlir -test-symbol-uses -convert-gpu-to-nvvm -convert-scf-to-cf
 #<!-- -->0 0x0000560da26e8508 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b3f508)
 #<!-- -->1 0x0000560da26e5c35 llvm::sys::RunSignalHandlers() (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b3cc35)
 #<!-- -->2 0x0000560da26e9611 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f4d92045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007f4d9209eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f4d9209eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007f4d9209eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007f4d9204527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007f4d920288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007f4d924df90d (/lib/x86_64-linux-gnu/libstdc++.so.6+0xdf90d)
#<!-- -->10 0x0000560da2eae289 mlir::WalkResult llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;::callback_fn&lt;(anonymous namespace)::SymbolUsesPass::runOnOperation()::'lambda'(mlir::Operation*)&gt;(long, mlir::Operation*) TestSymbolUses.cpp:0:0
#<!-- -->11 0x0000560da27a18d7 mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Operation*)&gt;, mlir::WalkOrder) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bf88d7)
#<!-- -->12 0x0000560da2ead2aa (anonymous namespace)::SymbolUsesPass::runOnOperation() TestSymbolUses.cpp:0:0
#<!-- -->13 0x0000560da9957bc1 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8daebc1)
#<!-- -->14 0x0000560da9958c07 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8dafc07)
#<!-- -->15 0x0000560da99601a3 mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8db71a3)
#<!-- -->16 0x0000560da995f7b2 mlir::PassManager::run(mlir::Operation*) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x8db67b2)
#<!-- -->17 0x0000560da278d603 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->18 0x0000560da278c864 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->19 0x0000560da9c94928 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x90eb928)
#<!-- -->20 0x0000560da2781dac mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bd8dac)
#<!-- -->21 0x0000560da27820e6 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bd90e6)
#<!-- -->22 0x0000560da27822f2 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1bd92f2)
#<!-- -->23 0x0000560da26d024f main (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b2724f)
#<!-- -->24 0x00007f4d9202a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->25 0x00007f4d9202a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->26 0x00007f4d9202a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->27 0x0000560da26cfd75 _start (/mnt/raid/home/username/bin/mlir/bin/mlir-opt+0x1b26d75)
Aborted
```
My MLIR version is:
```bash
LLVM (http://llvm.org/):
  LLVM version 22.0.0git
  Optimized build with assertions.
```

</details>


---

### Comment 2 - CoTinker

It crashes in `-test-symbol-uses`.

---

