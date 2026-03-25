# [MLIR] Invalid --mlir-debug-counter causes MLIR crash bug

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/180117
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-27T00:50:11Z

## Body

I encountered a crash triggered by invalid DebugCounter arguments. The code example below demonstrates the problem.

### Code Example
```llvm
module attributes {gpu.container_module} {
  gpu.module @kernels {
    func.func @gemv_kernel(%arg0: memref<?xf64, 3>, %arg1: memref<?xf64, 2>, %arg2: memref<?xf64>) attributes {gpu.kernel} {
      %c1 = arith.constant 1 : index
      %c2 = arith.constant 2 : index
      %c0 = arith.constant 0 : index
      %c1_i64 = arith.constant 1 : i64
      %c0_f64 = arith.constant 0.0 : f64
      
      %tidx = gpu.thread_id x
      %bidx = gpu.block_id x
      %bdim = gpu.block_dim x
      %gdim = gpu.grid_dim y
      
      %global_id = arith.addi %bidx, %tidx
      %scaled_id = arith.muli %global_id, %c2
      
      %dim0 = memref.dim %arg0, %c0 : memref<?xf64, 3>
      %dim1 = memref.dim %arg1, %c1 : memref<?xf64, 2>
      
      scf.while (%iter = %c0) : (index) -> (index) {
        %cond = arith.cmpi slt, %iter, %dim0 : index
        scf.condition(%cond) %iter : index
      } do {
      ^bb0(%iter: index):
        %val = memref.load %arg1[%iter, %c0] : memref<?xf64, 2>
        %sum = arith.addf %val, %c0_f64 : f64
        memref.store %sum, %arg0[%iter, %c0, %c1] : memref<?xf64, 3>
        %next = arith.addi %iter, %c1
        scf.yield %next : index
      }
      
      scf.for %i = %c0 to %scaled_id step %c2 {
        scf.if {
          %mod = arith.remsi %i, %c2 : index
          %cmp = arith.cmpi eq, %mod, %c0 : index
          scf.condition(%cmp)
        } {
          %temp = memref.alloca() : memref<10xf64>
          scf.parallel (%j = %c0 to %c2 step %c1) {
            %idx = arith.addi %i, %j
            %load = memref.load %arg2[%idx] : memref<?xf64>
            %mul = arith.mulf %load, %load : f64
            memref.store %mul, %temp[%j] : memref<10xf64>
          }
        }
      }
      gpu.return
    }
  }
  
  func.func @host_launch_gemv(%arg0: memref<?xf64, 3>, %arg1: memref<?xf64, 2>, %arg2: memref<?xf64>) {
    %c1 = arith.constant 1 : index
    %c64 = arith.constant 64 : index
    %c1024 = arith.constant 1024 : index
    
    %dim0 = memref.dim %arg0, %c0 : memref<?xf64, 3>
    %blocks = arith.ceildivui %dim0, %c64
    %grid_x = arith.select %blocks, %blocks, %c1 : index
    %grid_y = arith.constant 2 : index
    
    scf.execute_region {
      gpu.launch_func @kernels::@gemv_kernel
        blocks in (%grid_x, %grid_y, %c1)
        threads in (%c1024)
        args(%arg0 : memref<?xf64, 3>, %arg1 : memref<?xf64, 2>, %arg2 : memref<?xf64>)
      scf.yield
    }
    return
  }
}
```

I ran the command: `mlir-opt input.mlir --mlir-debug-counter=unique-tag-for-my-action-skip=-1\n`. Although this command is invalid, it causes a crash. Since MLIR handles other similar option cases correctly (e.g., `--test-greedy-patterns=max-iterations=1\n`), I believe this issue should be fixed.

### Stack Trace
```bash
error: expected DebugCounter counter value to be numeric, but got `-1n`
LLVM ERROR: Invalid DebugCounter command-line configuration
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir --mlir-debug-counter=unique-tag-for-my-action-skip=-1n
 #0 0x00005e961e5e287d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00005e961e5e2dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00005e961e5e0c64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005e961e5e3449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007b5d23045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007b5d2309eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76

#11 0x00005e961e5bb4b2 /llvm/lib/Support/ErrorHandling.cpp:96:3
#12 0x00005e961f679467 mlir::tracing::DebugCounter::applyCLOptions() /mlir/lib/Debug/DebugCounter.cpp:155:9
#13 0x00005e961f6792b2 mlir::tracing::DebugCounter::DebugCounter() /mlir/lib/Debug/DebugCounter.cpp:46:50
#14 0x00005e961f6752c5 mlir::tracing::InstallDebugHandler::Impl::Impl(mlir::MLIRContext&, mlir::tracing::DebugConfig const&) /mlir/lib/Debug/CLOptionsSetup.cpp:90:39
#15 0x00005e961f66f7ad std::__detail::_MakeUniq<mlir::tracing::InstallDebugHandler::Impl>::__single_object std::make_unique<mlir::tracing::InstallDebugHandler::Impl, mlir::MLIRContext&, mlir::tracing::DebugConfig const&>(mlir::MLIRContext&, mlir::tracing::DebugConfig const&) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:1070:34
#16 0x00005e961f66eca5 mlir::tracing::InstallDebugHandler::InstallDebugHandler(mlir::MLIRContext&, mlir::tracing::DebugConfig const&) /mlir/lib/Debug/CLOptionsSetup.cpp:152:54
 #2 0x00005e961e5e0c64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005e961e5e3449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007b5d23045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007b5d2309eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007b5d2309eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007b5d2309eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007b5d2304527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007b5d230288ff abort ./stdlib/abort.c:81:7
#10 0x00005e961e5bb635 llvm::report_fatal_error(llvm::Twine const&, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x00005e961e5bb4b2 /llvm/lib/Support/ErrorHandling.cpp:96:3
#12 0x00005e961f679467 mlir::tracing::DebugCounter::applyCLOptions() /mlir/lib/Debug/DebugCounter.cpp:155:9
#13 0x00005e961f6792b2 mlir::tracing::DebugCounter::DebugCounter() /mlir/lib/Debug/DebugCounter.cpp:46:50
#14 0x00005e961f6752c5 mlir::tracing::InstallDebugHandler::Impl::Impl(mlir::MLIRContext&, mlir::tracing::DebugConfig const&) /mlir/lib/Debug/CLOptionsSetup.cpp:90:39
#15 0x00005e961f66f7ad std::__detail::_MakeUniq<mlir::tracing::InstallDebugHandler::Impl>::__single_object std::make_unique<mlir::tracing::InstallDebugHandler::Impl, mlir::MLIRContext&, mlir::tracing::DebugConfig const&>(mlir::MLIRContext&, mlir::tracing::DebugConfig const&) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:1070:34
#16 0x00005e961f66eca5 mlir::tracing::InstallDebugHandler::InstallDebugHandler(mlir::MLIRContext&, mlir::tracing::DebugConfig const&) /mlir/lib/Debug/CLOptionsSetup.cpp:152:54
#17 0x00005e961e6ec415 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:668:8
#18 0x00005e961e6ec18c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#19 0x00005e961e6ec096 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#20 0x00005e962cfb8c5a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#21 0x00005e962cfb801a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#22 0x00005e961e6e7e16 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#23 0x00005e961e6e8215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#24 0x00005e961e6e83e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#25 0x00005e961e5b7485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#26 0x00007b5d2302a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#27 0x00007b5d2302a28b call_init ./csu/../csu/libc-start.c:128:20
#28 0x00007b5d2302a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#29 0x00005e961e5b72f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
I encountered a crash triggered by invalid DebugCounter arguments. The code example below demonstrates the problem.

### Code Example
```llvm
module attributes {gpu.container_module} {
  gpu.module @<!-- -->kernels {
    func.func @<!-- -->gemv_kernel(%arg0: memref&lt;?xf64, 3&gt;, %arg1: memref&lt;?xf64, 2&gt;, %arg2: memref&lt;?xf64&gt;) attributes {gpu.kernel} {
      %c1 = arith.constant 1 : index
      %c2 = arith.constant 2 : index
      %c0 = arith.constant 0 : index
      %c1_i64 = arith.constant 1 : i64
      %c0_f64 = arith.constant 0.0 : f64
      
      %tidx = gpu.thread_id x
      %bidx = gpu.block_id x
      %bdim = gpu.block_dim x
      %gdim = gpu.grid_dim y
      
      %global_id = arith.addi %bidx, %tidx
      %scaled_id = arith.muli %global_id, %c2
      
      %dim0 = memref.dim %arg0, %c0 : memref&lt;?xf64, 3&gt;
      %dim1 = memref.dim %arg1, %c1 : memref&lt;?xf64, 2&gt;
      
      scf.while (%iter = %c0) : (index) -&gt; (index) {
        %cond = arith.cmpi slt, %iter, %dim0 : index
        scf.condition(%cond) %iter : index
      } do {
      ^bb0(%iter: index):
        %val = memref.load %arg1[%iter, %c0] : memref&lt;?xf64, 2&gt;
        %sum = arith.addf %val, %c0_f64 : f64
        memref.store %sum, %arg0[%iter, %c0, %c1] : memref&lt;?xf64, 3&gt;
        %next = arith.addi %iter, %c1
        scf.yield %next : index
      }
      
      scf.for %i = %c0 to %scaled_id step %c2 {
        scf.if {
          %mod = arith.remsi %i, %c2 : index
          %cmp = arith.cmpi eq, %mod, %c0 : index
          scf.condition(%cmp)
        } {
          %temp = memref.alloca() : memref&lt;10xf64&gt;
          scf.parallel (%j = %c0 to %c2 step %c1) {
            %idx = arith.addi %i, %j
            %load = memref.load %arg2[%idx] : memref&lt;?xf64&gt;
            %mul = arith.mulf %load, %load : f64
            memref.store %mul, %temp[%j] : memref&lt;10xf64&gt;
          }
        }
      }
      gpu.return
    }
  }
  
  func.func @<!-- -->host_launch_gemv(%arg0: memref&lt;?xf64, 3&gt;, %arg1: memref&lt;?xf64, 2&gt;, %arg2: memref&lt;?xf64&gt;) {
    %c1 = arith.constant 1 : index
    %c64 = arith.constant 64 : index
    %c1024 = arith.constant 1024 : index
    
    %dim0 = memref.dim %arg0, %c0 : memref&lt;?xf64, 3&gt;
    %blocks = arith.ceildivui %dim0, %c64
    %grid_x = arith.select %blocks, %blocks, %c1 : index
    %grid_y = arith.constant 2 : index
    
    scf.execute_region {
      gpu.launch_func @<!-- -->kernels::@<!-- -->gemv_kernel
        blocks in (%grid_x, %grid_y, %c1)
        threads in (%c1024)
        args(%arg0 : memref&lt;?xf64, 3&gt;, %arg1 : memref&lt;?xf64, 2&gt;, %arg2 : memref&lt;?xf64&gt;)
      scf.yield
    }
    return
  }
}
```

I ran the command: `mlir-opt input.mlir --mlir-debug-counter=unique-tag-for-my-action-skip=-1`. Although this command is invalid, it causes a crash. Since MLIR handles other similar cases correctly, I believe this issue should be fixed.

### Stack Trace
```bash
error: expected DebugCounter counter value to be numeric, but got `-1n`
LLVM ERROR: Invalid DebugCounter command-line configuration
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir --mlir-debug-counter=unique-tag-for-my-action-skip=-1n
 #<!-- -->0 0x00005e961e5e287d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00005e961e5e2dab PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00005e961e5e0c64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005e961e5e3449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007b5d23045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007b5d2309eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76

#<!-- -->11 0x00005e961e5bb4b2 /llvm/lib/Support/ErrorHandling.cpp:96:3
#<!-- -->12 0x00005e961f679467 mlir::tracing::DebugCounter::applyCLOptions() /mlir/lib/Debug/DebugCounter.cpp:155:9
#<!-- -->13 0x00005e961f6792b2 mlir::tracing::DebugCounter::DebugCounter() /mlir/lib/Debug/DebugCounter.cpp:46:50
#<!-- -->14 0x00005e961f6752c5 mlir::tracing::InstallDebugHandler::Impl::Impl(mlir::MLIRContext&amp;, mlir::tracing::DebugConfig const&amp;) /mlir/lib/Debug/CLOptionsSetup.cpp:90:39
#<!-- -->15 0x00005e961f66f7ad std::__detail::_MakeUniq&lt;mlir::tracing::InstallDebugHandler::Impl&gt;::__single_object std::make_unique&lt;mlir::tracing::InstallDebugHandler::Impl, mlir::MLIRContext&amp;, mlir::tracing::DebugConfig const&amp;&gt;(mlir::MLIRContext&amp;, mlir::tracing::DebugConfig const&amp;) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:1070:34
#<!-- -->16 0x00005e961f66eca5 mlir::tracing::InstallDebugHandler::InstallDebugHandler(mlir::MLIRContext&amp;, mlir::tracing::DebugConfig const&amp;) /mlir/lib/Debug/CLOptionsSetup.cpp:152:54
 #<!-- -->2 0x00005e961e5e0c64 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005e961e5e3449 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007b5d23045330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007b5d2309eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007b5d2309eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007b5d2309eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007b5d2304527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007b5d230288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00005e961e5bb635 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x00005e961e5bb4b2 /llvm/lib/Support/ErrorHandling.cpp:96:3
#<!-- -->12 0x00005e961f679467 mlir::tracing::DebugCounter::applyCLOptions() /mlir/lib/Debug/DebugCounter.cpp:155:9
#<!-- -->13 0x00005e961f6792b2 mlir::tracing::DebugCounter::DebugCounter() /mlir/lib/Debug/DebugCounter.cpp:46:50
#<!-- -->14 0x00005e961f6752c5 mlir::tracing::InstallDebugHandler::Impl::Impl(mlir::MLIRContext&amp;, mlir::tracing::DebugConfig const&amp;) /mlir/lib/Debug/CLOptionsSetup.cpp:90:39
#<!-- -->15 0x00005e961f66f7ad std::__detail::_MakeUniq&lt;mlir::tracing::InstallDebugHandler::Impl&gt;::__single_object std::make_unique&lt;mlir::tracing::InstallDebugHandler::Impl, mlir::MLIRContext&amp;, mlir::tracing::DebugConfig const&amp;&gt;(mlir::MLIRContext&amp;, mlir::tracing::DebugConfig const&amp;) /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:1070:34
#<!-- -->16 0x00005e961f66eca5 mlir::tracing::InstallDebugHandler::InstallDebugHandler(mlir::MLIRContext&amp;, mlir::tracing::DebugConfig const&amp;) /mlir/lib/Debug/CLOptionsSetup.cpp:152:54
#<!-- -->17 0x00005e961e6ec415 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:668:8
#<!-- -->18 0x00005e961e6ec18c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->19 0x00005e961e6ec096 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->20 0x00005e962cfb8c5a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->21 0x00005e962cfb801a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->22 0x00005e961e6e7e16 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->23 0x00005e961e6e8215 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->24 0x00005e961e6e83e8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->25 0x00005e961e5b7485 main /mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->26 0x00007b5d2302a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->27 0x00007b5d2302a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->28 0x00007b5d2302a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->29 0x00005e961e5b72f5 _start (/mnt/raid/build_all/bin/mlir-opt+0x46a92f5)
Aborted
```
</details>


---

