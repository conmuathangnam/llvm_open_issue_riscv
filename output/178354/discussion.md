# [MLIR] NVPTX LowerAlloca Failure during GPU Program Lowering

**Author:** stoneman258
**URL:** https://github.com/llvm/llvm-project/issues/178354
**Status:** Open
**Labels:** mlir:gpu, crash

## Body

While performing MLIR lowering using mlir-opt, a crash bug occurs in the NVPTX lowering pass.

### Input Program
```llvm
module {
  func.func @host_mat_trans_smem_swizzle_kernel_launcher(%arg0: memref<?xi32, 1>, %arg1: index, %arg2: index, %arg3: memref<?xi32, 1>) {
    %c32 = arith.constant 32 : index
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %grid_x = arith.divui %arg2, %c32 : index
    %grid_y = arith.divui %arg1, %c32 : index
    %unroll_factor = arith.constant 4 : index
    scf.for %i = %c0 to %grid_x step %c1 {
      scf.for %j = %c0 to %grid_y step %c1 {
        gpu.launch blocks(%bx, %by, %bz) in (%sz_bx = %unroll_factor, %sz_by = %unroll_factor, %sz_bz = %c1)
                 threads(%tx, %ty, %tz) in (%sz_tx = %c32, %sz_ty = %c32, %sz_tz = %c1) {
          %block_offset_x = arith.muli %i, %unroll_factor : index
          %block_offset_y = arith.muli %j, %unroll_factor : index
          %global_bx = arith.addi %bx, %block_offset_x : index
          %global_by = arith.addi %by, %block_offset_y : index
          %row_base = arith.muli %global_by, %c32 : index
          %row = arith.addi %row_base, %ty : index
          %col_base = arith.muli %global_bx, %c32 : index
          %col = arith.addi %col_base, %tx : index
          %s_data = memref.alloca() : memref<32x32xi32, 3>
          %cond = arith.cmpi slt, %row, %arg1 : index
          scf.if %cond {
            %cond_inner = arith.cmpi slt, %col, %arg2 : index
            scf.if %cond_inner {
              %idx = arith.muli %row, %arg2 : index
              %idx1 = arith.addi %idx, %col : index
              %val = memref.load %arg0[%idx1] : memref<?xi32, 1>
              %neg = arith.subi %c32, %tx : index
              %swizzle_idx = arith.xori %neg, %ty : index
              memref.store %val, %s_data[%tx, %swizzle_idx] : memref<32x32xi32, 3>
            }
            gpu.barrier
            %n_col_base = arith.muli %global_by, %c32 : index
            %n_col = arith.addi %n_col_base, %tx : index
            %n_row_base = arith.muli %global_bx, %c32 : index
            %n_row = arith.addi %n_row_base, %ty : index
            %cond2 = arith.cmpi slt, %n_row, %arg2 : index
            scf.if %cond2 {
              %cond3 = arith.cmpi slt, %n_col, %arg1 : index
              scf.if %cond3 {
                %swizzle_idx2 = arith.xori %ty, %tx : index
                %val2 = memref.load %s_data[%ty, %swizzle_idx2] : memref<32x32xi32, 3>
                %idx2 = arith.muli %n_row, %arg1 : index
                %idx3 = arith.addi %idx2, %n_col : index
                memref.store %val2, %arg3[%idx3] : memref<?xi32, 1>
              }
            }
          }
          gpu.terminator
        }
      }
    }
    return
  }
}
```

### Stack Trace
```bash
mlir-opt: /llvm-project/llvm/lib/Target/NVPTX/NVPTXLowerAlloca.cpp:73: virtual bool (anonymous namespace)::NVPTXLowerAlloca::runOnFunction(Function &): Assertion `(AllocAddrSpace == ADDRESS_SPACE_GENERIC || AllocAddrSpace == ADDRESS_SPACE_LOCAL) && "AllocaInst can only be in Generic or Local address space for " "NVPTX."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_90 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
1.      Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.      Running pass 'convert address space of alloca'ed memory to local' on function '@host_mat_trans_smem_swizzle_kernel_launcher_kernel'
 #0 0x0000560014dd261d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #1 0x0000560014dd2b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #2 0x0000560014dd0a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x0000560014dd31e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #4 0x00007f4404c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007f4404c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f4404c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007f4404c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007f4404c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007f4404c288ff abort ./stdlib/abort.c:81:7
#10 0x00007f4404c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#11 0x00007f4404c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#12 0x00005600180f7969 (anonymous namespace)::NVPTXLowerAlloca::runOnFunction(llvm::Function&) /llvm-project/llvm/lib/Target/NVPTX/NVPTXLowerAlloca.cpp:75:40
#13 0x0000560021d1927f llvm::FPPassManager::runOnFunction(llvm::Function&) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#14 0x0000560021d1d90b llvm::FPPassManager::runOnModule(llvm::Module&) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#15 0x0000560021d19aa9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#16 0x0000560021d1966f llvm::legacy::PassManagerImpl::run(llvm::Module&) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:16
#17 0x0000560021d1dbb1 llvm::legacy::PassManager::run(llvm::Module&) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#18 0x000056001a4359c6 mlir::LLVM::ModuleToObject::translateToISA[abi:cxx11](llvm::Module&, llvm::TargetMachine&) /llvm-project/mlir/lib/Target/LLVM/ModuleToObject.cpp:223:3
#19 0x0000560017ff664c (anonymous namespace)::NVPTXSerializer::moduleToObject(llvm::Module&) /llvm-project/mlir/lib/Target/LLVM/NVVM/Target.cpp:702:23
#20 0x000056001a435eb0 mlir::LLVM::ModuleToObject::run() /llvm-project/mlir/lib/Target/LLVM/ModuleToObject.cpp:283:10
#21 0x0000560017ff5f0d (anonymous namespace)::NVVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) const /llvm-project/mlir/lib/Target/LLVM/NVVM/Target.cpp:751:39
#22 0x0000560017ff5d50 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel<(anonymous namespace)::NVVMTargetAttrImpl>::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&) /llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:230:51
#23 0x0000560020fc5fb4 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&) const /llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#24 0x0000560017da3e9c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:92:10
#25 0x0000560017da3c0d mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&) /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:125:20
#26 0x0000560017da4d51 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:69:14
#27 0x0000560022799864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#28 0x0000560022799805 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#29 0x0000560014df8879 llvm::function_ref<void ()>::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#30 0x000056002279cf2b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#31 0x0000560022790cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#32 0x00005600227913da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#33 0x0000560022796ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#34 0x0000560022796256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#35 0x0000560014ed349b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#36 0x0000560014ed2b71 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#37 0x0000560014ed282c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#38 0x0000560014ed2736 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#39 0x0000560022cdbcba llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#40 0x0000560022cdb07a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#41 0x0000560014ece126 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#42 0x0000560014ece525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#43 0x0000560014ece6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#44 0x0000560014da7935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#45 0x00007f4404c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#46 0x00007f4404c2a28b call_init ./csu/../csu/libc-start.c:128:20
#47 0x00007f4404c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#48 0x0000560014da7505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Used Command
```bash
mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_90 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (stoneman258)

<details>
While performing MLIR lowering using mlir-opt, a crash bug occurs in the NVPTX lowering pass.

### Input Program
```llvm
module {
  func.func @<!-- -->host_mat_trans_smem_swizzle_kernel_launcher(%arg0: memref&lt;?xi32, 1&gt;, %arg1: index, %arg2: index, %arg3: memref&lt;?xi32, 1&gt;) {
    %c32 = arith.constant 32 : index
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %grid_x = arith.divui %arg2, %c32 : index
    %grid_y = arith.divui %arg1, %c32 : index
    %unroll_factor = arith.constant 4 : index
    scf.for %i = %c0 to %grid_x step %c1 {
      scf.for %j = %c0 to %grid_y step %c1 {
        gpu.launch blocks(%bx, %by, %bz) in (%sz_bx = %unroll_factor, %sz_by = %unroll_factor, %sz_bz = %c1)
                 threads(%tx, %ty, %tz) in (%sz_tx = %c32, %sz_ty = %c32, %sz_tz = %c1) {
          %block_offset_x = arith.muli %i, %unroll_factor : index
          %block_offset_y = arith.muli %j, %unroll_factor : index
          %global_bx = arith.addi %bx, %block_offset_x : index
          %global_by = arith.addi %by, %block_offset_y : index
          %row_base = arith.muli %global_by, %c32 : index
          %row = arith.addi %row_base, %ty : index
          %col_base = arith.muli %global_bx, %c32 : index
          %col = arith.addi %col_base, %tx : index
          %s_data = memref.alloca() : memref&lt;32x32xi32, 3&gt;
          %cond = arith.cmpi slt, %row, %arg1 : index
          scf.if %cond {
            %cond_inner = arith.cmpi slt, %col, %arg2 : index
            scf.if %cond_inner {
              %idx = arith.muli %row, %arg2 : index
              %idx1 = arith.addi %idx, %col : index
              %val = memref.load %arg0[%idx1] : memref&lt;?xi32, 1&gt;
              %neg = arith.subi %c32, %tx : index
              %swizzle_idx = arith.xori %neg, %ty : index
              memref.store %val, %s_data[%tx, %swizzle_idx] : memref&lt;32x32xi32, 3&gt;
            }
            gpu.barrier
            %n_col_base = arith.muli %global_by, %c32 : index
            %n_col = arith.addi %n_col_base, %tx : index
            %n_row_base = arith.muli %global_bx, %c32 : index
            %n_row = arith.addi %n_row_base, %ty : index
            %cond2 = arith.cmpi slt, %n_row, %arg2 : index
            scf.if %cond2 {
              %cond3 = arith.cmpi slt, %n_col, %arg1 : index
              scf.if %cond3 {
                %swizzle_idx2 = arith.xori %ty, %tx : index
                %val2 = memref.load %s_data[%ty, %swizzle_idx2] : memref&lt;32x32xi32, 3&gt;
                %idx2 = arith.muli %n_row, %arg1 : index
                %idx3 = arith.addi %idx2, %n_col : index
                memref.store %val2, %arg3[%idx3] : memref&lt;?xi32, 1&gt;
              }
            }
          }
          gpu.terminator
        }
      }
    }
    return
  }
}
```

### Stack Trace
```bash
mlir-opt: /llvm-project/llvm/lib/Target/NVPTX/NVPTXLowerAlloca.cpp:73: virtual bool (anonymous namespace)::NVPTXLowerAlloca::runOnFunction(Function &amp;): Assertion `(AllocAddrSpace == ADDRESS_SPACE_GENERIC || AllocAddrSpace == ADDRESS_SPACE_LOCAL) &amp;&amp; "AllocaInst can only be in Generic or Local address space for " "NVPTX."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_90 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
1.      Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.      Running pass 'convert address space of alloca'ed memory to local' on function '@<!-- -->host_mat_trans_smem_swizzle_kernel_launcher_kernel'
 #<!-- -->0 0x0000560014dd261d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000560014dd2b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000560014dd0a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x0000560014dd31e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f4404c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f4404c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f4404c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f4404c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f4404c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f4404c288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007f4404c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007f4404c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005600180f7969 (anonymous namespace)::NVPTXLowerAlloca::runOnFunction(llvm::Function&amp;) /llvm-project/llvm/lib/Target/NVPTX/NVPTXLowerAlloca.cpp:75:40
#<!-- -->13 0x0000560021d1927f llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->14 0x0000560021d1d90b llvm::FPPassManager::runOnModule(llvm::Module&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->15 0x0000560021d19aa9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->16 0x0000560021d1966f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->17 0x0000560021d1dbb1 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->18 0x000056001a4359c6 mlir::LLVM::ModuleToObject::translateToISA[abi:cxx11](llvm::Module&amp;, llvm::TargetMachine&amp;) /llvm-project/mlir/lib/Target/LLVM/ModuleToObject.cpp:223:3
#<!-- -->19 0x0000560017ff664c (anonymous namespace)::NVPTXSerializer::moduleToObject(llvm::Module&amp;) /llvm-project/mlir/lib/Target/LLVM/NVVM/Target.cpp:702:23
#<!-- -->20 0x000056001a435eb0 mlir::LLVM::ModuleToObject::run() /llvm-project/mlir/lib/Target/LLVM/ModuleToObject.cpp:283:10
#<!-- -->21 0x0000560017ff5f0d (anonymous namespace)::NVVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /llvm-project/mlir/lib/Target/LLVM/NVVM/Target.cpp:751:39
#<!-- -->22 0x0000560017ff5d50 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::NVVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:230:51
#<!-- -->23 0x0000560020fc5fb4 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->24 0x0000560017da3e9c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:92:10
#<!-- -->25 0x0000560017da3c0d mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:125:20
#<!-- -->26 0x0000560017da4d51 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:69:14
#<!-- -->27 0x0000560022799864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->28 0x0000560022799805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->29 0x0000560014df8879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->30 0x000056002279cf2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->31 0x0000560022790cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->32 0x00005600227913da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->33 0x0000560022796ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->34 0x0000560022796256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->35 0x0000560014ed349b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->36 0x0000560014ed2b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->37 0x0000560014ed282c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->38 0x0000560014ed2736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->39 0x0000560022cdbcba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->40 0x0000560022cdb07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->41 0x0000560014ece126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->42 0x0000560014ece525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->43 0x0000560014ece6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->44 0x0000560014da7935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->45 0x00007f4404c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->46 0x00007f4404c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->47 0x00007f4404c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->48 0x0000560014da7505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Used Command
```bash
mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_90 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (stoneman258)

<details>
While performing MLIR lowering using mlir-opt, a crash bug occurs in the NVPTX lowering pass.

### Input Program
```llvm
module {
  func.func @<!-- -->host_mat_trans_smem_swizzle_kernel_launcher(%arg0: memref&lt;?xi32, 1&gt;, %arg1: index, %arg2: index, %arg3: memref&lt;?xi32, 1&gt;) {
    %c32 = arith.constant 32 : index
    %c0 = arith.constant 0 : index
    %c1 = arith.constant 1 : index
    %grid_x = arith.divui %arg2, %c32 : index
    %grid_y = arith.divui %arg1, %c32 : index
    %unroll_factor = arith.constant 4 : index
    scf.for %i = %c0 to %grid_x step %c1 {
      scf.for %j = %c0 to %grid_y step %c1 {
        gpu.launch blocks(%bx, %by, %bz) in (%sz_bx = %unroll_factor, %sz_by = %unroll_factor, %sz_bz = %c1)
                 threads(%tx, %ty, %tz) in (%sz_tx = %c32, %sz_ty = %c32, %sz_tz = %c1) {
          %block_offset_x = arith.muli %i, %unroll_factor : index
          %block_offset_y = arith.muli %j, %unroll_factor : index
          %global_bx = arith.addi %bx, %block_offset_x : index
          %global_by = arith.addi %by, %block_offset_y : index
          %row_base = arith.muli %global_by, %c32 : index
          %row = arith.addi %row_base, %ty : index
          %col_base = arith.muli %global_bx, %c32 : index
          %col = arith.addi %col_base, %tx : index
          %s_data = memref.alloca() : memref&lt;32x32xi32, 3&gt;
          %cond = arith.cmpi slt, %row, %arg1 : index
          scf.if %cond {
            %cond_inner = arith.cmpi slt, %col, %arg2 : index
            scf.if %cond_inner {
              %idx = arith.muli %row, %arg2 : index
              %idx1 = arith.addi %idx, %col : index
              %val = memref.load %arg0[%idx1] : memref&lt;?xi32, 1&gt;
              %neg = arith.subi %c32, %tx : index
              %swizzle_idx = arith.xori %neg, %ty : index
              memref.store %val, %s_data[%tx, %swizzle_idx] : memref&lt;32x32xi32, 3&gt;
            }
            gpu.barrier
            %n_col_base = arith.muli %global_by, %c32 : index
            %n_col = arith.addi %n_col_base, %tx : index
            %n_row_base = arith.muli %global_bx, %c32 : index
            %n_row = arith.addi %n_row_base, %ty : index
            %cond2 = arith.cmpi slt, %n_row, %arg2 : index
            scf.if %cond2 {
              %cond3 = arith.cmpi slt, %n_col, %arg1 : index
              scf.if %cond3 {
                %swizzle_idx2 = arith.xori %ty, %tx : index
                %val2 = memref.load %s_data[%ty, %swizzle_idx2] : memref&lt;32x32xi32, 3&gt;
                %idx2 = arith.muli %n_row, %arg1 : index
                %idx3 = arith.addi %idx2, %n_col : index
                memref.store %val2, %arg3[%idx3] : memref&lt;?xi32, 1&gt;
              }
            }
          }
          gpu.terminator
        }
      }
    }
    return
  }
}
```

### Stack Trace
```bash
mlir-opt: /llvm-project/llvm/lib/Target/NVPTX/NVPTXLowerAlloca.cpp:73: virtual bool (anonymous namespace)::NVPTXLowerAlloca::runOnFunction(Function &amp;): Assertion `(AllocAddrSpace == ADDRESS_SPACE_GENERIC || AllocAddrSpace == ADDRESS_SPACE_LOCAL) &amp;&amp; "AllocaInst can only be in Generic or Local address space for " "NVPTX."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_90 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
1.      Running pass 'Function Pass Manager' on module 'LLVMDialectModule'.
2.      Running pass 'convert address space of alloca'ed memory to local' on function '@<!-- -->host_mat_trans_smem_swizzle_kernel_launcher_kernel'
 #<!-- -->0 0x0000560014dd261d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /llvm-project/llvm/lib/Support/Unix/Signals.inc:834:11
 #<!-- -->1 0x0000560014dd2b4b PrintStackTraceSignalHandler(void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:916:1
 #<!-- -->2 0x0000560014dd0a1f llvm::sys::RunSignalHandlers() /llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #<!-- -->3 0x0000560014dd31e9 SignalHandler(int, siginfo_t*, void*) /llvm-project/llvm/lib/Support/Unix/Signals.inc:426:38
 #<!-- -->4 0x00007f4404c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007f4404c9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007f4404c9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007f4404c9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007f4404c4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007f4404c288ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007f4404c2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->11 0x00007f4404c3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->12 0x00005600180f7969 (anonymous namespace)::NVPTXLowerAlloca::runOnFunction(llvm::Function&amp;) /llvm-project/llvm/lib/Target/NVPTX/NVPTXLowerAlloca.cpp:75:40
#<!-- -->13 0x0000560021d1927f llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1398:23
#<!-- -->14 0x0000560021d1d90b llvm::FPPassManager::runOnModule(llvm::Module&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1444:16
#<!-- -->15 0x0000560021d19aa9 (anonymous namespace)::MPPassManager::runOnModule(llvm::Module&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1513:23
#<!-- -->16 0x0000560021d1966f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:16
#<!-- -->17 0x0000560021d1dbb1 llvm::legacy::PassManager::run(llvm::Module&amp;) /llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1640:3
#<!-- -->18 0x000056001a4359c6 mlir::LLVM::ModuleToObject::translateToISA[abi:cxx11](llvm::Module&amp;, llvm::TargetMachine&amp;) /llvm-project/mlir/lib/Target/LLVM/ModuleToObject.cpp:223:3
#<!-- -->19 0x0000560017ff664c (anonymous namespace)::NVPTXSerializer::moduleToObject(llvm::Module&amp;) /llvm-project/mlir/lib/Target/LLVM/NVVM/Target.cpp:702:23
#<!-- -->20 0x000056001a435eb0 mlir::LLVM::ModuleToObject::run() /llvm-project/mlir/lib/Target/LLVM/ModuleToObject.cpp:283:10
#<!-- -->21 0x0000560017ff5f0d (anonymous namespace)::NVVMTargetAttrImpl::serializeToObject(mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /llvm-project/mlir/lib/Target/LLVM/NVVM/Target.cpp:751:39
#<!-- -->22 0x0000560017ff5d50 mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::FallbackModel&lt;(anonymous namespace)::NVVMTargetAttrImpl&gt;::serializeToObject(mlir::gpu::detail::TargetAttrInterfaceInterfaceTraits::Concept const*, mlir::Attribute, mlir::Operation*, mlir::gpu::TargetOptions const&amp;) /llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.h.inc:230:51
#<!-- -->23 0x0000560020fc5fb4 mlir::gpu::TargetAttrInterface::serializeToObject(mlir::Operation*, mlir::gpu::TargetOptions const&amp;) const /llvm-project/build_all/tools/mlir/include/mlir/Dialect/GPU/IR/CompilationAttrInterfaces.cpp.inc:19:14
#<!-- -->24 0x0000560017da3e9c (anonymous namespace)::moduleSerializer(mlir::gpu::GPUModuleOp, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:92:10
#<!-- -->25 0x0000560017da3c0d mlir::gpu::transformGpuModulesToBinaries(mlir::Operation*, mlir::gpu::OffloadingLLVMTranslationAttrInterface, mlir::gpu::TargetOptions const&amp;) /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:125:20
#<!-- -->26 0x0000560017da4d51 (anonymous namespace)::GpuModuleToBinaryPass::runOnOperation() /llvm-project/mlir/lib/Dialect/GPU/Transforms/ModuleToBinary.cpp:69:14
#<!-- -->27 0x0000560022799864 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /llvm-project/mlir/lib/Pass/Pass.cpp:0:17
#<!-- -->28 0x0000560022799805 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->29 0x0000560014df8879 llvm::function_ref&lt;void ()&gt;::operator()() const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->30 0x000056002279cf2b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /llvm-project/mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->31 0x0000560022790cfa mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /llvm-project/mlir/lib/Pass/Pass.cpp:615:17
#<!-- -->32 0x00005600227913da mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) /llvm-project/mlir/lib/Pass/Pass.cpp:682:16
#<!-- -->33 0x0000560022796ccd mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /llvm-project/mlir/lib/Pass/Pass.cpp:1117:10
#<!-- -->34 0x0000560022796256 mlir::PassManager::run(mlir::Operation*) /llvm-project/mlir/lib/Pass/Pass.cpp:1091:60
#<!-- -->35 0x0000560014ed349b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->36 0x0000560014ed2b71 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->37 0x0000560014ed282c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:762:12
#<!-- -->38 0x0000560014ed2736 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->39 0x0000560022cdbcba llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->40 0x0000560022cdb07a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /llvm-project/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->41 0x0000560014ece126 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:767:26
#<!-- -->42 0x0000560014ece525 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:813:14
#<!-- -->43 0x0000560014ece6f8 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /llvm-project/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:829:10
#<!-- -->44 0x0000560014da7935 main /llvm-project/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->45 0x00007f4404c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->46 0x00007f4404c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->47 0x00007f4404c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->48 0x0000560014da7505 _start (/mnt/raid/llvm-project/build_all/bin/mlir-opt+0x435c505)
Aborted
```

### Used Command
```bash
mlir-opt input.mlir -gpu-kernel-outlining --convert-scf-to-cf --affine-loop-coalescing --convert-gpu-to-nvvm -reconcile-unrealized-casts --nvvm-attach-target=chip=sm_90 --gpu-module-to-binary -gpu-to-llvm --convert-cf-to-llvm --convert-index-to-llvm --convert-arith-to-llvm --convert-math-to-llvm --convert-func-to-llvm --finalize-memref-to-llvm -convert-nvvm-to-llvm -reconcile-unrealized-casts
```
</details>


---

### Comment 3 - woruyu

I would like to fix it!

---

### Comment 4 - woruyu

@stoneman258 , The problem is `nvptx-lower-alloca` pass just support lowering allocaInst of AddressSpace GENERIC or LOCAL, but `%s_data = memref.alloca() : memref<32x32xi32, 3>` is in shared addressspace. where is you source mlir from?

---

### Comment 5 - stoneman258

@woruyu Hi, it is from an official NVIDIA benchmark.

---

### Comment 6 - woruyu

> [@woruyu](https://github.com/woruyu) Hi, it is from an official NVIDIA benchmark.

Could you provide more details information.  What is an official NVIDIA benchmark's url? And how is this mlir code generated? Friendly ping @weiweichen 

---

### Comment 7 - stoneman258

Hi @woruyu , the dataset is quite large and private. So I’m sorry that I can’t share it with you at the moment. Generally, we mainly implemented these MLIR programs by drawing on some CUDA programs as references.

---

### Comment 8 - woruyu

> Hi [@woruyu](https://github.com/woruyu) , the dataset is quite large and private. So I’m sorry that I can’t share it with you at the moment. Generally, we mainly implemented these MLIR programs by drawing on some CUDA programs as references.

Thank you for the clarification. Without access to the dataset, I’m afraid I won’t be able to investigate this issue any further. I can only point out the cause as described by the bug reporter, but I’m unable to suggest a definitive solution without additional information.


---

