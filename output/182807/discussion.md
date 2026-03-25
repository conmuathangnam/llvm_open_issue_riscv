# [MLIR] mlir-runner crash in SyclRuntimeWrappers

**Author:** jiayiw2008
**URL:** https://github.com/llvm/llvm-project/issues/182807
**Status:** Open
**Labels:** mlir, crash

## Body

I found a crash bug in SyclRuntimeWrappers.cpp, see the following code example.

```llvm
module @add attributes {gpu.container_module} {
  memref.global "private" constant @__constant_2x2x2xf32_0 : memref<2x2x2xf32> = dense<[[[1.1, 2.2], [3.3, 4.4]], [[5.5, 6.6], [7.7, 8.8 ]]]>
  memref.global "private" constant @__constant_2x2x2xf32 : memref<2x2x2xf32> = dense<[[[1.2, 2.3], [4.5, 5.8]], [[7.2, 8.3], [10.5, 11.8]]]>
  func.func @main() {
    %0 = memref.get_global @__constant_2x2x2xf32 : memref<2x2x2xf32>
    %1 = memref.get_global @__constant_2x2x2xf32_0 : memref<2x2x2xf32>
    %2 = call @test(%0, %1) : (memref<2x2x2xf32>, memref<2x2x2xf32>) -> memref<2x2x2xf32>
    %cast = memref.cast %2 : memref<2x2x2xf32> to memref<*xf32>
    call @printMemrefF32(%cast) : (memref<*xf32>) -> ()
    return
  }
  func.func private @printMemrefF32(memref<*xf32>)
  func.func @test(%arg0: memref<2x2x2xf32>, %arg1: memref<2x2x2xf32>) -> memref<2x2x2xf32> {
    %c2 = arith.constant 2 : index
    %c1 = arith.constant 1 : index
    %mem = gpu.alloc host_shared () : memref<2x2x2xf32>
    memref.copy %arg1, %mem : memref<2x2x2xf32> to memref<2x2x2xf32>
    %memref_0 = gpu.alloc host_shared () : memref<2x2x2xf32>
    memref.copy %arg0, %memref_0 : memref<2x2x2xf32> to memref<2x2x2xf32>
    %memref_2 = gpu.alloc host_shared () : memref<2x2x2xf32>
    %2 = gpu.wait async
    %3 = gpu.launch_func async [%2] @test_kernel::@test_kernel blocks in (%c2, %c2, %c2) threads in (%c1, %c1, %c1) args(%memref_0 : memref<2x2x2xf32>, %mem : memref<2x2x2xf32>, %memref_2 : memref<2x2x2xf32>)
    gpu.wait [%3]
    %alloc = memref.alloc() : memref<2x2x2xf32>
    memref.copy %memref_2, %alloc : memref<2x2x2xf32> to memref<2x2x2xf32>
    %4 = gpu.wait async
    %5 = gpu.dealloc async [%4] %memref_2 : memref<2x2x2xf32>
    %6 = gpu.dealloc async [%5] %memref_0 : memref<2x2x2xf32>
    %7 = gpu.dealloc async [%6] %mem : memref<2x2x2xf32>
    gpu.wait [%7]
    return %alloc : memref<2x2x2xf32>
  }
  gpu.module @test_kernel attributes {spirv.target_env = #spirv.target_env<#spirv.vce<v1.0, [Addresses, Int64, Kernel], []>, api=OpenCL, #spirv.resource_limits<>>} {
    gpu.func @test_kernel(%arg0: memref<2x2x2xf32>, %arg1: memref<2x2x2xf32>, %arg2: memref<2x2x2xf32>) kernel attributes {gpu.known_block_size = array<i32: 1, 1, 1>, gpu.known_grid_size = array<i32: 2, 2, 2>, spirv.entry_point_abi = #spirv.entry_point_abi<>} {
      %0 = gpu.block_id  x
      %1 = gpu.block_id  y
      %2 = gpu.block_id  z
      %3 = memref.load %arg0[%0, %1, %2] : memref<2x2x2xf32>
      %4 = memref.load %arg1[%0, %1, %2] : memref<2x2x2xf32>
      %5 = arith.addf %3, %4 : f32
      memref.store %5, %arg2[%0, %1, %2] : memref<2x2x2xf32>
      gpu.return
    }
  }
}
```

First, I lowered the input MLIR program with the following command:
```bash
mlir-opt add32.mlir -pass-pipeline='builtin.module(spirv-attach-target{ver=v1.0 caps=Addresses,Int64,Kernel},convert-gpu-to-spirv{use-64bit-index=true},gpu.module(spirv.module(spirv-lower-abi-attrs,spirv-update-vce)),func.func(llvm-request-c-wrappers),convert-scf-to-cf,convert-cf-to-llvm,convert-arith-to-llvm,convert-math-to-llvm,convert-func-to-llvm,gpu-to-llvm{use-bare-pointers-for-kernels=true},gpu-module-to-binary,expand-strided-metadata,lower-affine,finalize-memref-to-llvm,reconcile-unrealized-casts)' -o lowered_add32.mlir
```

Then I ran it with the following command:
```bash
mlir-runner lowered_add32.mlir --shared-libs=/build_sycl/lib/libmlir_sycl_runtime.so --shared-libs=/build_all/lib/libmlir_runner_utils.so --entry-point-result=void
```

This produced the crash trace shown below.
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-runner lowered_add32.mlir --shared-libs=/build_sycl/lib/libmlir_sycl_runtime.so --shared-libs=/build_all/lib/libmlir_runner_utils.so --entry-point-result=void
 #0 0x00005e0ff7b87a8d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #1 0x00005e0ff7b87fbb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #2 0x00005e0ff7b85e54 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005e0ff7b88759 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #4 0x00007bfcae645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007bfcae69eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007bfcae69eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007bfcae69eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007bfcae64527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007bfcae6288ff abort ./stdlib/abort.c:81:7
#10 0x00007bfcaef62e7a mgpuModuleLoad.cold SyclRuntimeWrappers.cpp:0:0
#11 0x00007bfcaef5901c
#12 0x00007bfcaef595ed
#13 0x00005e0ff88828b0 (anonymous namespace)::GenericLLVMIRPlatformSupport::initialize(llvm::orc::JITDylib&) /llvm/lib/ExecutionEngine/Orc/LLJIT.cpp:237:28
#14 0x00005e0ff871b936 llvm::orc::LLJIT::initialize(llvm::orc::JITDylib&) /llvm/include/llvm/ExecutionEngine/Orc/LLJIT.h:203:16
#15 0x00005e0ff8717d25 mlir::ExecutionEngine::initialize() /mlir/lib/ExecutionEngine/ExecutionEngine.cpp:454:3
#16 0x00005e0ff86c613f compileAndExecute((anonymous namespace)::Options&, mlir::Operation*, llvm::StringRef, (anonymous namespace)::CompileAndExecuteConfig, void**, std::unique_ptr<llvm::TargetMachine, std::default_delete<llvm::TargetMachine>>) /mlir/lib/ExecutionEngine/JitRunner.cpp:207:23
#17 0x00005e0ff86c5cbf compileAndExecuteVoidFunction((anonymous namespace)::Options&, mlir::Operation*, llvm::StringRef, (anonymous namespace)::CompileAndExecuteConfig, std::unique_ptr<llvm::TargetMachine, std::default_delete<llvm::TargetMachine>>) /mlir/lib/ExecutionEngine/JitRunner.cpp:241:3
#18 0x00005e0ff86c44b7 mlir::JitRunnerMain(int, char**, mlir::DialectRegistry const&, mlir::JitRunnerConfig) /mlir/lib/ExecutionEngine/JitRunner.cpp:398:17
#19 0x00005e0ff771ef76 main /mlir/tools/mlir-runner/mlir-runner.cpp:93:3
#20 0x00007bfcae62a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#21 0x00007bfcae62a28b call_init ./csu/../csu/libc-start.c:128:20
#22 0x00007bfcae62a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#23 0x00005e0ff771eda5 _start (/mnt/raid/build_all/bin/mlir-runner+0x44eda5)
```

It seems that this bug is caused by the SYCL backend only supporting LevelZero. I think MLIR should terminate gracefully when only other OpenCL devices are available, instead of crashing.

https://github.com/llvm/llvm-project/blob/059accca5367283d842cfb2bbe03aa45af2fc543/mlir/lib/ExecutionEngine/SyclRuntimeWrappers.cpp#L67

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: PureLove77 (jiayiw2008)

<details>
I found a crash bug in SyclRuntimeWrappers.cpp, see the following code example.

```llvm
module @<!-- -->add attributes {gpu.container_module} {
  memref.global "private" constant @<!-- -->__constant_2x2x2xf32_0 : memref&lt;2x2x2xf32&gt; = dense&lt;[[[1.1, 2.2], [3.3, 4.4]], [[5.5, 6.6], [7.7, 8.8 ]]]&gt;
  memref.global "private" constant @<!-- -->__constant_2x2x2xf32 : memref&lt;2x2x2xf32&gt; = dense&lt;[[[1.2, 2.3], [4.5, 5.8]], [[7.2, 8.3], [10.5, 11.8]]]&gt;
  func.func @<!-- -->main() {
    %0 = memref.get_global @<!-- -->__constant_2x2x2xf32 : memref&lt;2x2x2xf32&gt;
    %1 = memref.get_global @<!-- -->__constant_2x2x2xf32_0 : memref&lt;2x2x2xf32&gt;
    %2 = call @<!-- -->test(%0, %1) : (memref&lt;2x2x2xf32&gt;, memref&lt;2x2x2xf32&gt;) -&gt; memref&lt;2x2x2xf32&gt;
    %cast = memref.cast %2 : memref&lt;2x2x2xf32&gt; to memref&lt;*xf32&gt;
    call @<!-- -->printMemrefF32(%cast) : (memref&lt;*xf32&gt;) -&gt; ()
    return
  }
  func.func private @<!-- -->printMemrefF32(memref&lt;*xf32&gt;)
  func.func @<!-- -->test(%arg0: memref&lt;2x2x2xf32&gt;, %arg1: memref&lt;2x2x2xf32&gt;) -&gt; memref&lt;2x2x2xf32&gt; {
    %c2 = arith.constant 2 : index
    %c1 = arith.constant 1 : index
    %mem = gpu.alloc host_shared () : memref&lt;2x2x2xf32&gt;
    memref.copy %arg1, %mem : memref&lt;2x2x2xf32&gt; to memref&lt;2x2x2xf32&gt;
    %memref_0 = gpu.alloc host_shared () : memref&lt;2x2x2xf32&gt;
    memref.copy %arg0, %memref_0 : memref&lt;2x2x2xf32&gt; to memref&lt;2x2x2xf32&gt;
    %memref_2 = gpu.alloc host_shared () : memref&lt;2x2x2xf32&gt;
    %2 = gpu.wait async
    %3 = gpu.launch_func async [%2] @<!-- -->test_kernel::@<!-- -->test_kernel blocks in (%c2, %c2, %c2) threads in (%c1, %c1, %c1) args(%memref_0 : memref&lt;2x2x2xf32&gt;, %mem : memref&lt;2x2x2xf32&gt;, %memref_2 : memref&lt;2x2x2xf32&gt;)
    gpu.wait [%3]
    %alloc = memref.alloc() : memref&lt;2x2x2xf32&gt;
    memref.copy %memref_2, %alloc : memref&lt;2x2x2xf32&gt; to memref&lt;2x2x2xf32&gt;
    %4 = gpu.wait async
    %5 = gpu.dealloc async [%4] %memref_2 : memref&lt;2x2x2xf32&gt;
    %6 = gpu.dealloc async [%5] %memref_0 : memref&lt;2x2x2xf32&gt;
    %7 = gpu.dealloc async [%6] %mem : memref&lt;2x2x2xf32&gt;
    gpu.wait [%7]
    return %alloc : memref&lt;2x2x2xf32&gt;
  }
  gpu.module @<!-- -->test_kernel attributes {spirv.target_env = #spirv.target_env&lt;#spirv.vce&lt;v1.0, [Addresses, Int64, Kernel], []&gt;, api=OpenCL, #spirv.resource_limits&lt;&gt;&gt;} {
    gpu.func @<!-- -->test_kernel(%arg0: memref&lt;2x2x2xf32&gt;, %arg1: memref&lt;2x2x2xf32&gt;, %arg2: memref&lt;2x2x2xf32&gt;) kernel attributes {gpu.known_block_size = array&lt;i32: 1, 1, 1&gt;, gpu.known_grid_size = array&lt;i32: 2, 2, 2&gt;, spirv.entry_point_abi = #spirv.entry_point_abi&lt;&gt;} {
      %0 = gpu.block_id  x
      %1 = gpu.block_id  y
      %2 = gpu.block_id  z
      %3 = memref.load %arg0[%0, %1, %2] : memref&lt;2x2x2xf32&gt;
      %4 = memref.load %arg1[%0, %1, %2] : memref&lt;2x2x2xf32&gt;
      %5 = arith.addf %3, %4 : f32
      memref.store %5, %arg2[%0, %1, %2] : memref&lt;2x2x2xf32&gt;
      gpu.return
    }
  }
}
```

First, I lowered the input MLIR program with the following command:
```bash
mlir-opt add32.mlir -pass-pipeline='builtin.module(spirv-attach-target{ver=v1.0 caps=Addresses,Int64,Kernel},convert-gpu-to-spirv{use-64bit-index=true},gpu.module(spirv.module(spirv-lower-abi-attrs,spirv-update-vce)),func.func(llvm-request-c-wrappers),convert-scf-to-cf,convert-cf-to-llvm,convert-arith-to-llvm,convert-math-to-llvm,convert-func-to-llvm,gpu-to-llvm{use-bare-pointers-for-kernels=true},gpu-module-to-binary,expand-strided-metadata,lower-affine,finalize-memref-to-llvm,reconcile-unrealized-casts)' -o lowered_add32.mlir
```

Then I ran it with the following command:
```bash
mlir-runner lowered_add32.mlir --shared-libs=/build_sycl/lib/libmlir_sycl_runtime.so --shared-libs=/build_all/lib/libmlir_runner_utils.so --entry-point-result=void
```

This produced the crash trace shown below.
```bash
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: mlir-runner lowered_add32.mlir --shared-libs=/build_sycl/lib/libmlir_sycl_runtime.so --shared-libs=/build_all/lib/libmlir_runner_utils.so --entry-point-result=void
 #<!-- -->0 0x00005e0ff7b87a8d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm/lib/Support/Unix/Signals.inc:880:11
 #<!-- -->1 0x00005e0ff7b87fbb PrintStackTraceSignalHandler(void*) /llvm/lib/Support/Unix/Signals.inc:962:1
 #<!-- -->2 0x00005e0ff7b85e54 llvm::sys::RunSignalHandlers() /llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005e0ff7b88759 SignalHandler(int, siginfo_t*, void*) /llvm/lib/Support/Unix/Signals.inc:448:38
 #<!-- -->4 0x00007bfcae645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007bfcae69eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007bfcae69eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007bfcae69eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007bfcae64527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007bfcae6288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007bfcaef62e7a mgpuModuleLoad.cold SyclRuntimeWrappers.cpp:0:0
#<!-- -->11 0x00007bfcaef5901c
#<!-- -->12 0x00007bfcaef595ed
#<!-- -->13 0x00005e0ff88828b0 (anonymous namespace)::GenericLLVMIRPlatformSupport::initialize(llvm::orc::JITDylib&amp;) /llvm/lib/ExecutionEngine/Orc/LLJIT.cpp:237:28
#<!-- -->14 0x00005e0ff871b936 llvm::orc::LLJIT::initialize(llvm::orc::JITDylib&amp;) /llvm/include/llvm/ExecutionEngine/Orc/LLJIT.h:203:16
#<!-- -->15 0x00005e0ff8717d25 mlir::ExecutionEngine::initialize() /mlir/lib/ExecutionEngine/ExecutionEngine.cpp:454:3
#<!-- -->16 0x00005e0ff86c613f compileAndExecute((anonymous namespace)::Options&amp;, mlir::Operation*, llvm::StringRef, (anonymous namespace)::CompileAndExecuteConfig, void**, std::unique_ptr&lt;llvm::TargetMachine, std::default_delete&lt;llvm::TargetMachine&gt;&gt;) /mlir/lib/ExecutionEngine/JitRunner.cpp:207:23
#<!-- -->17 0x00005e0ff86c5cbf compileAndExecuteVoidFunction((anonymous namespace)::Options&amp;, mlir::Operation*, llvm::StringRef, (anonymous namespace)::CompileAndExecuteConfig, std::unique_ptr&lt;llvm::TargetMachine, std::default_delete&lt;llvm::TargetMachine&gt;&gt;) /mlir/lib/ExecutionEngine/JitRunner.cpp:241:3
#<!-- -->18 0x00005e0ff86c44b7 mlir::JitRunnerMain(int, char**, mlir::DialectRegistry const&amp;, mlir::JitRunnerConfig) /mlir/lib/ExecutionEngine/JitRunner.cpp:398:17
#<!-- -->19 0x00005e0ff771ef76 main /mlir/tools/mlir-runner/mlir-runner.cpp:93:3
#<!-- -->20 0x00007bfcae62a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->21 0x00007bfcae62a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->22 0x00007bfcae62a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->23 0x00005e0ff771eda5 _start (/mnt/raid/build_all/bin/mlir-runner+0x44eda5)
```

It seems that this bug is caused by the SYCL backend only supporting LevelZero. I think MLIR should terminate gracefully when only other OpenCL devices are available, instead of crashing.

https://github.com/llvm/llvm-project/blob/059accca5367283d842cfb2bbe03aa45af2fc543/mlir/lib/ExecutionEngine/SyclRuntimeWrappers.cpp#L67
</details>


---

### Comment 2 - jiayiw2008

@EugeneZelenko Hi, the input program is not generated by fuzzer, and it originates from the test suite of MLIR, thanks!

---

