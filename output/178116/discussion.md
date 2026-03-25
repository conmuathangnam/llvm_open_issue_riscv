# [MLIR] Crash Bug in MLIR Program caused by GPU Structure Issues

**Author:** tisble
**URL:** https://github.com/llvm/llvm-project/issues/178116
**Status:** Closed
**Labels:** crash, mlir:tosa
**Closed Date:** 2026-01-28T10:43:14Z

## Body

I discovered a crash bug while analyzing the following program using MLIR.

```llvm
module attributes {gpu.container_module} {
  func.func @main() -> () {
    %c2 = arith.constant 2 : index
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c3 = arith.constant 3 : index

    // Compose 2D memref then reshape to 1D view
    %A_host = memref.alloc(%c3) : memref<?xf32>
    %x_host = memref.alloc(%c2) : memref<?xf32>
    %C_host = memref.alloc(%c2, %c2) : memref<?x?xf32>

    // Initialize with 1D layout
    %c1_f32 = arith.constant 1.0 : f32
    %c2_f32 = arith.constant 2.0 : f32
    %c3_f32 = arith.constant 3.0 : f32
    %c4_f32 = arith.constant 4.0 : f32
    %c5_f32 = arith.constant 5.0 : f32
    
    // Store A elements
    memref.store %c1_f32, %A_host[%c0] : memref<?xf32>
    memref.store %c2_f32, %A_host[%c1] : memref<?xf32>
    memref.store %c3_f32, %A_host[%c2] : memref<?xf32>

    // Initialize x
    memref.store %c4_f32, %x_host[%c0] : memref<?xf32>
    memref.store %c5_f32, %x_host[%c1] : memref<?xf32>

    // Allocate device memory with host shared attribute nested
    %d_A = gpu.alloc host_shared (%c2, %c2) : memref<?x?xf32, 3>
    %d_x = gpu.alloc host_shared (%c2) : memref<?xf32, 3>
    %d_C = gpu.alloc host_shared (%c2, %c2) : memref<?x?xf32, 3>

    // Use nested regions and scf ops for selective copying
    %c2_i32 = arith.constant 2 : i32
    %c0_i32 = arith.constant 0 : i32
    %c1_i32 = arith.constant 1 : i32
    
    // Copy with loop
    %idx0 = arith.constant 0 : index
    %idx1 = arith.constant 1 : index
    %val0 = memref.load %A_host[%idx0] : memref<?xf32>
    %val1 = memref.load %A_host[%idx1] : memref<?xf32>
    memref.store %val0, %d_A[%c0, %c0] : memref<?x?xf32, 3>
    memref.store %val1, %d_A[%c0, %c1] : memref<?x?xf32, 3>
    
    // Copy rest using gpu.memcpy
    gpu.memcpy %d_x, %x_host : memref<?xf32, 3>, memref<?xf32>

    // Launch kernel with if condition inside
    gpu.launch blocks(%bx, %by, %bz) in (%grid_x = %c2, %grid_y = %c2, %grid_z = %c1)
                threads(%tx, %ty, %tz) in (%block_x = %c1, %block_y = %c1, %block_z = %c1) {
      %row = gpu.block_id x
      %col = gpu.block_id y
      %x_val = memref.load %d_x[%row] : memref<?xf32, 3>
      %a_val = memref.load %d_A[%row, %col] : memref<?x?xf32, 3>
      
      // Conditionally compute
      %pred = arith.cmpf ugt, %x_val, %c2_f32 : f32
      scf.if %pred {
        %result = arith.mulf %a_val, %x_val : f32
        memref.store %result, %d_C[%row, %col] : memref<?x?xf32, 3>
      } else {
        %neg = arith.negf %a_val : f32
        memref.store %neg, %d_C[%row, %col] : memref<?x?xf32, 3>
      }
      gpu.terminator
    }

    // Copy result back
    gpu.memcpy %C_host, %d_C : memref<?x?xf32>, memref<?x?xf32, 3>

    // Free device memory
    gpu.dealloc %d_A : memref<?x?xf32, 3>
    gpu.dealloc %d_x : memref<?xf32, 3>
    gpu.dealloc %d_C : memref<?x?xf32, 3>

    memref.dealloc %A_host : memref<?xf32>
    memref.dealloc %x_host : memref<?xf32>
    memref.dealloc %C_host : memref<?x?xf32>

    return
  }
}
```

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir --tosa-validate
 #0 0x000055ef322a161d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x000055ef322a1b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x000055ef3229fa1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x000055ef322a21e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f2e88c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f2e88c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f2e88c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f2e88c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f2e88c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f2e88c288ff abort ./stdlib/abort.c:81:7
#10 0x000055ef3227a845 llvm::report_fatal_error(llvm::Twine const&, bool) /llvm-project/llvm/lib/Support/ErrorHandling.cpp:137:5
#11 0x000055ef3d020ffa std::enable_if<!std::is_same<mlir::tosa::TargetEnvAttr::ImplType, mlir::AttributeStorage>::value, mlir::tosa::TargetEnvAttr>::type mlir::detail::AttributeUniquer::getWithTypeID<mlir::tosa::TargetEnvAttr, mlir::tosa::SpecificationVersion, mlir::tosa::Level, llvm::ArrayRef<mlir::tosa::Profile>, llvm::ArrayRef<mlir::tosa::Extension>>(mlir::MLIRContext*, mlir::TypeID, mlir::tosa::SpecificationVersion&&, mlir::tosa::Level&&, llvm::ArrayRef<mlir::tosa::Profile>&&, llvm::ArrayRef<mlir::tosa::Extension>&&) /llvm-project/mlir/include/mlir/IR/AttributeSupport.h:239:12
#12 0x000055ef3d020f0a mlir::tosa::TargetEnvAttr mlir::detail::AttributeUniquer::get<mlir::tosa::TargetEnvAttr, mlir::tosa::SpecificationVersion, mlir::tosa::Level, llvm::ArrayRef<mlir::tosa::Profile>, llvm::ArrayRef<mlir::tosa::Extension>>(mlir::MLIRContext*, mlir::tosa::SpecificationVersion&&, mlir::tosa::Level&&, llvm::ArrayRef<mlir::tosa::Profile>&&, llvm::ArrayRef<mlir::tosa::Extension>&&) /llvm-project/mlir/include/mlir/IR/AttributeSupport.h:220:12
#13 0x000055ef3cf02de8 mlir::tosa::TargetEnvAttr mlir::detail::StorageUserBase<mlir::tosa::TargetEnvAttr, mlir::Attribute, mlir::tosa::detail::TargetEnvAttrStorage, mlir::detail::AttributeUniquer>::get<mlir::tosa::SpecificationVersion, mlir::tosa::Level, llvm::ArrayRef<mlir::tosa::Profile>, llvm::ArrayRef<mlir::tosa::Extension>>(mlir::MLIRContext*, mlir::tosa::SpecificationVersion&&, mlir::tosa::Level&&, llvm::ArrayRef<mlir::tosa::Profile>&&, llvm::ArrayRef<mlir::tosa::Extension>&&) /llvm-project/mlir/include/mlir/IR/StorageUniquerSupport.h:181:12
#14 0x000055ef3cded0b6 mlir::tosa::TargetEnvAttr::get(mlir::MLIRContext*, mlir::tosa::SpecificationVersion, mlir::tosa::Level, llvm::ArrayRef<mlir::tosa::Profile>, llvm::ArrayRef<mlir::tosa::Extension>) /llvm-project/build_all/tools/mlir/include/mlir/Dialect/Tosa/IR/TosaAttributes.cpp.inc:940:10
#15 0x000055ef3d04a508 mlir::tosa::getDefaultTargetEnv(mlir::MLIRContext*) /llvm-project/mlir/lib/Dialect/Tosa/IR/TargetEnv.cpp:122:10
#16 0x000055ef3d04a55b mlir::tosa::lookupTargetEnvOrDefault(mlir::Operation*) /llvm-project/mlir/lib/Dialect/Tosa/IR/TargetEnv.cpp:130:10
#17 0x000055ef399f1544 (anonymous namespace)::TosaValidation::runOnOperation() /llvm-project/mlir/lib/Dialect/Tosa/Transforms/TosaValidation.cpp:1229:39
#18 0x000055ef3fc68864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#19 0x000055ef3fc68805 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#20 0x000055ef322c7879 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#21 0x000055ef3fc6bf2b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#22 0x000055ef3fc5fcfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#23 0x000055ef3fc603da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#24 0x000055ef3fc65ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#25 0x000055ef3fc65256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#26 0x000055ef323a249b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#27 0x000055ef323a1b71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#28 0x000055ef323a182c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#29 0x000055ef323a1736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#30 0x000055ef401aacba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#31 0x000055ef401aa07a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#32 0x000055ef3239d126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#33 0x000055ef3239d525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#34 0x000055ef3239d6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#35 0x000055ef32276935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#36 0x00007f2e88c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#37 0x00007f2e88c2a28b call_init ./csu/../csu/libc-start.c:128:20
#38 0x00007f2e88c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#39 0x000055ef32276505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (tisble)

<details>
I discovered a crash bug while analyzing the following program using MLIR.

```llvm
module attributes {gpu.container_module} {
  func.func @<!-- -->main() -&gt; () {
    %c2 = arith.constant 2 : index
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %c3 = arith.constant 3 : index

    // Compose 2D memref then reshape to 1D view
    %A_host = memref.alloc(%c3) : memref&lt;?xf32&gt;
    %x_host = memref.alloc(%c2) : memref&lt;?xf32&gt;
    %C_host = memref.alloc(%c2, %c2) : memref&lt;?x?xf32&gt;

    // Initialize with 1D layout
    %c1_f32 = arith.constant 1.0 : f32
    %c2_f32 = arith.constant 2.0 : f32
    %c3_f32 = arith.constant 3.0 : f32
    %c4_f32 = arith.constant 4.0 : f32
    %c5_f32 = arith.constant 5.0 : f32
    
    // Store A elements
    memref.store %c1_f32, %A_host[%c0] : memref&lt;?xf32&gt;
    memref.store %c2_f32, %A_host[%c1] : memref&lt;?xf32&gt;
    memref.store %c3_f32, %A_host[%c2] : memref&lt;?xf32&gt;

    // Initialize x
    memref.store %c4_f32, %x_host[%c0] : memref&lt;?xf32&gt;
    memref.store %c5_f32, %x_host[%c1] : memref&lt;?xf32&gt;

    // Allocate device memory with host shared attribute nested
    %d_A = gpu.alloc host_shared (%c2, %c2) : memref&lt;?x?xf32, 3&gt;
    %d_x = gpu.alloc host_shared (%c2) : memref&lt;?xf32, 3&gt;
    %d_C = gpu.alloc host_shared (%c2, %c2) : memref&lt;?x?xf32, 3&gt;

    // Use nested regions and scf ops for selective copying
    %c2_i32 = arith.constant 2 : i32
    %c0_i32 = arith.constant 0 : i32
    %c1_i32 = arith.constant 1 : i32
    
    // Copy with loop
    %idx0 = arith.constant 0 : index
    %idx1 = arith.constant 1 : index
    %val0 = memref.load %A_host[%idx0] : memref&lt;?xf32&gt;
    %val1 = memref.load %A_host[%idx1] : memref&lt;?xf32&gt;
    memref.store %val0, %d_A[%c0, %c0] : memref&lt;?x?xf32, 3&gt;
    memref.store %val1, %d_A[%c0, %c1] : memref&lt;?x?xf32, 3&gt;
    
    // Copy rest using gpu.memcpy
    gpu.memcpy %d_x, %x_host : memref&lt;?xf32, 3&gt;, memref&lt;?xf32&gt;

    // Launch kernel with if condition inside
    gpu.launch blocks(%bx, %by, %bz) in (%grid_x = %c2, %grid_y = %c2, %grid_z = %c1)
                threads(%tx, %ty, %tz) in (%block_x = %c1, %block_y = %c1, %block_z = %c1) {
      %row = gpu.block_id x
      %col = gpu.block_id y
      %x_val = memref.load %d_x[%row] : memref&lt;?xf32, 3&gt;
      %a_val = memref.load %d_A[%row, %col] : memref&lt;?x?xf32, 3&gt;
      
      // Conditionally compute
      %pred = arith.cmpf ugt, %x_val, %c2_f32 : f32
      scf.if %pred {
        %result = arith.mulf %a_val, %x_val : f32
        memref.store %result, %d_C[%row, %col] : memref&lt;?x?xf32, 3&gt;
      } else {
        %neg = arith.negf %a_val : f32
        memref.store %neg, %d_C[%row, %col] : memref&lt;?x?xf32, 3&gt;
      }
      gpu.terminator
    }

    // Copy result back
    gpu.memcpy %C_host, %d_C : memref&lt;?x?xf32&gt;, memref&lt;?x?xf32, 3&gt;

    // Free device memory
    gpu.dealloc %d_A : memref&lt;?x?xf32, 3&gt;
    gpu.dealloc %d_x : memref&lt;?xf32, 3&gt;
    gpu.dealloc %d_C : memref&lt;?x?xf32, 3&gt;

    memref.dealloc %A_host : memref&lt;?xf32&gt;
    memref.dealloc %x_host : memref&lt;?xf32&gt;
    memref.dealloc %C_host : memref&lt;?x?xf32&gt;

    return
  }
}
```

```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-opt input.mlir --tosa-validate
 #<!-- -->0 0x000055ef322a161d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x000055ef322a1b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x000055ef3229fa1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x000055ef322a21e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f2e88c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f2e88c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f2e88c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f2e88c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f2e88c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f2e88c288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x000055ef3227a845 llvm::report_fatal_error(llvm::Twine const&amp;, bool) /llvm-project/llvm/lib/Support/ErrorHandling.cpp:137:5
#<!-- -->11 0x000055ef3d020ffa std::enable_if&lt;!std::is_same&lt;mlir::tosa::TargetEnvAttr::ImplType, mlir::AttributeStorage&gt;::value, mlir::tosa::TargetEnvAttr&gt;::type mlir::detail::AttributeUniquer::getWithTypeID&lt;mlir::tosa::TargetEnvAttr, mlir::tosa::SpecificationVersion, mlir::tosa::Level, llvm::ArrayRef&lt;mlir::tosa::Profile&gt;, llvm::ArrayRef&lt;mlir::tosa::Extension&gt;&gt;(mlir::MLIRContext*, mlir::TypeID, mlir::tosa::SpecificationVersion&amp;&amp;, mlir::tosa::Level&amp;&amp;, llvm::ArrayRef&lt;mlir::tosa::Profile&gt;&amp;&amp;, llvm::ArrayRef&lt;mlir::tosa::Extension&gt;&amp;&amp;) /llvm-project/mlir/include/mlir/IR/AttributeSupport.h:239:12
#<!-- -->12 0x000055ef3d020f0a mlir::tosa::TargetEnvAttr mlir::detail::AttributeUniquer::get&lt;mlir::tosa::TargetEnvAttr, mlir::tosa::SpecificationVersion, mlir::tosa::Level, llvm::ArrayRef&lt;mlir::tosa::Profile&gt;, llvm::ArrayRef&lt;mlir::tosa::Extension&gt;&gt;(mlir::MLIRContext*, mlir::tosa::SpecificationVersion&amp;&amp;, mlir::tosa::Level&amp;&amp;, llvm::ArrayRef&lt;mlir::tosa::Profile&gt;&amp;&amp;, llvm::ArrayRef&lt;mlir::tosa::Extension&gt;&amp;&amp;) /llvm-project/mlir/include/mlir/IR/AttributeSupport.h:220:12
#<!-- -->13 0x000055ef3cf02de8 mlir::tosa::TargetEnvAttr mlir::detail::StorageUserBase&lt;mlir::tosa::TargetEnvAttr, mlir::Attribute, mlir::tosa::detail::TargetEnvAttrStorage, mlir::detail::AttributeUniquer&gt;::get&lt;mlir::tosa::SpecificationVersion, mlir::tosa::Level, llvm::ArrayRef&lt;mlir::tosa::Profile&gt;, llvm::ArrayRef&lt;mlir::tosa::Extension&gt;&gt;(mlir::MLIRContext*, mlir::tosa::SpecificationVersion&amp;&amp;, mlir::tosa::Level&amp;&amp;, llvm::ArrayRef&lt;mlir::tosa::Profile&gt;&amp;&amp;, llvm::ArrayRef&lt;mlir::tosa::Extension&gt;&amp;&amp;) /llvm-project/mlir/include/mlir/IR/StorageUniquerSupport.h:181:12
#<!-- -->14 0x000055ef3cded0b6 mlir::tosa::TargetEnvAttr::get(mlir::MLIRContext*, mlir::tosa::SpecificationVersion, mlir::tosa::Level, llvm::ArrayRef&lt;mlir::tosa::Profile&gt;, llvm::ArrayRef&lt;mlir::tosa::Extension&gt;) /llvm-project/build_all/tools/mlir/include/mlir/Dialect/Tosa/IR/TosaAttributes.cpp.inc:940:10
#<!-- -->15 0x000055ef3d04a508 mlir::tosa::getDefaultTargetEnv(mlir::MLIRContext*) /llvm-project/mlir/lib/Dialect/Tosa/IR/TargetEnv.cpp:122:10
#<!-- -->16 0x000055ef3d04a55b mlir::tosa::lookupTargetEnvOrDefault(mlir::Operation*) /llvm-project/mlir/lib/Dialect/Tosa/IR/TargetEnv.cpp:130:10
#<!-- -->17 0x000055ef399f1544 (anonymous namespace)::TosaValidation::runOnOperation() /llvm-project/mlir/lib/Dialect/Tosa/Transforms/TosaValidation.cpp:1229:39
#<!-- -->18 0x000055ef3fc68864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->19 0x000055ef3fc68805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->20 0x000055ef322c7879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->21 0x000055ef3fc6bf2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->22 0x000055ef3fc5fcfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->23 0x000055ef3fc603da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->24 0x000055ef3fc65ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->25 0x000055ef3fc65256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->26 0x000055ef323a249b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->27 0x000055ef323a1b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->28 0x000055ef323a182c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->29 0x000055ef323a1736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->30 0x000055ef401aacba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->31 0x000055ef401aa07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->32 0x000055ef3239d126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->33 0x000055ef3239d525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->34 0x000055ef3239d6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->35 0x000055ef32276935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->36 0x00007f2e88c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->37 0x00007f2e88c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->38 0x00007f2e88c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->39 0x000055ef32276505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```
</details>


---

### Comment 2 - woruyu

I would like to fix it!

---

### Comment 3 - lhutton1

Hi @tisble, which hash of LLVM are you using? I believe this was fixed recently by https://github.com/llvm/llvm-project/pull/173449. I tested your reproducer on `7a5e59e6` but wasn't able to reproduce

---

### Comment 4 - woruyu

I also test it in main branch(cf4ce383b3c7), which wasn't able to reproduce.

---

### Comment 5 - lhutton1

I'll close for now, but please reopen if the issue persists

---

### Comment 6 - tisble

@lhutton1 Hi, my version is 564c3de67d20d578d05678b49045378fdcf5ccaa

---

### Comment 7 - tisble

> Hi [@tisble](https://github.com/tisble), which hash of LLVM are you using? I believe this was fixed recently by [#173449](https://github.com/llvm/llvm-project/pull/173449). I tested your reproducer on `7a5e59e6` but wasn't able to reproduce

It seems that this is an old bug, I will try to confirm it in the latest branch.

---

### Comment 8 - tisble

This bug was tested in version b52591b329ba3a3085041b2452edcd3c27556ca8, and it appears to have been fixed in a later version.

---

