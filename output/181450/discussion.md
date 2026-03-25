# mlir-opt crash: WarpExecuteOnLane0Op assertion failure due to incorrect terminator type

**Author:** mlirBug
**URL:** https://github.com/llvm/llvm-project/issues/181450
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2026-02-28T17:37:37Z

## Body

Bug Op: gpu.warp_execute_on_lane_0

Bug Pass: N/A (Crash occurs during IR Verification)

Detailed Description:
The mlir-opt tool crashes during verification of the gpu.warp_execute_on_lane_0 operation. This operation is defined with the SingleBlockImplicitTerminator<mlir::gpu::YieldOp> trait, meaning it strictly requires a gpu.yield as its terminator.

```mlir
module {
  func.func @main() {
    %0 = ptr.type_offset i32 : index
    %1 = gpu.lane_id
    %2 = emitc.rem %1, %0 : (index, index) -> index
    %3 = mpi.comm_world : !mpi.comm
    %4 = shape.mul %2, %0 : index, index -> index
    %5 = vector.create_mask %4 : vector<4xi1>
    %6 = gpu.warp_execute_on_lane_0(%4)[32] -> vector<4xi1> {
      affine.yield %5 : vector<4xi1>
    }
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = mlir::gpu::YieldOp; From = mlir::Operation]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
 #0 0x00005d942001ae32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00005d94200175af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00005d94200176fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007885bc845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007885bc89eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007885bc89eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007885bc89eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007885bc84527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007885bc8288ff abort ./stdlib/abort.c:81:7
 #9 0x00007885bc82881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007885bc83b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005d9426a02a2f mlir::gpu::WarpExecuteOnLane0Op::getTerminator() (llvmorg-22.1.0-install/bin/mlir-opt+0x8f17a2f)
#12 0x00005d9426a999a8 mlir::gpu::WarpExecuteOnLane0Op::verify() (llvmorg-22.1.0-install/bin/mlir-opt+0x8fae9a8)
#13 0x00005d9426af7fe8 mlir::Op<mlir::gpu::WarpExecuteOnLane0Op, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands<1u>::Impl, mlir::OpTrait::SingleBlock, mlir::OpTrait::SingleBlockImplicitTerminator<mlir::gpu::YieldOp>::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::RegionBranchOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects>::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x900cfe8)
#14 0x00005d9426aed9c7 mlir::RegisteredOperationName::Model<mlir::gpu::WarpExecuteOnLane0Op>::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x90029c7)
#15 0x00005d9427c6d76e (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&) (.isra.0) Verifier.cpp:0:0
#16 0x00005d9427c6f818 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#17 0x00005d9427c6eac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) Verifier.cpp:0:0
#18 0x00005d9427c6f799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#19 0x00005d9427c6eac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) Verifier.cpp:0:0
#20 0x00005d9427c6f799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#21 0x00005d9427c706c8 mlir::verify(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0xa1856c8)
#22 0x00005d94261b140a mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c640a)
#23 0x00005d94200e0df7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#24 0x00005d94200e24ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#25 0x00005d9427c8a731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#26 0x00005d94200d98d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#27 0x00005d94200e2b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#28 0x00005d94200e2da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#29 0x00005d941ff14023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#30 0x00007885bc82a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x00007885bc82a28b call_init ./csu/../csu/libc-start.c:128:20
#32 0x00007885bc82a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#33 0x00005d941fff9405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (mlirBug)

<details>
Bug Op: gpu.warp_execute_on_lane_0

Bug Pass: N/A (Crash occurs during IR Verification)

Detailed Description:
The mlir-opt tool crashes during verification of the gpu.warp_execute_on_lane_0 operation. This operation is defined with the SingleBlockImplicitTerminator&lt;mlir::gpu::YieldOp&gt; trait, meaning it strictly requires a gpu.yield as its terminator.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = ptr.type_offset i32 : index
    %1 = gpu.lane_id
    %2 = emitc.rem %1, %0 : (index, index) -&gt; index
    %3 = mpi.comm_world : !mpi.comm
    %4 = shape.mul %2, %0 : index, index -&gt; index
    %5 = vector.create_mask %4 : vector&lt;4xi1&gt;
    %6 = gpu.warp_execute_on_lane_0(%4)[32] -&gt; vector&lt;4xi1&gt; {
      affine.yield %5 : vector&lt;4xi1&gt;
    }
    return
  }
}
```
```
mlir-opt temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From*) [with To = mlir::gpu::YieldOp; From = mlir::Operation]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
 #<!-- -->0 0x00005d942001ae32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00005d94200175af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00005d94200176fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007885bc845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007885bc89eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007885bc89eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007885bc89eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007885bc84527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007885bc8288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x00007885bc82881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x00007885bc83b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005d9426a02a2f mlir::gpu::WarpExecuteOnLane0Op::getTerminator() (llvmorg-22.1.0-install/bin/mlir-opt+0x8f17a2f)
#<!-- -->12 0x00005d9426a999a8 mlir::gpu::WarpExecuteOnLane0Op::verify() (llvmorg-22.1.0-install/bin/mlir-opt+0x8fae9a8)
#<!-- -->13 0x00005d9426af7fe8 mlir::Op&lt;mlir::gpu::WarpExecuteOnLane0Op, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;1u&gt;::Impl, mlir::OpTrait::SingleBlock, mlir::OpTrait::SingleBlockImplicitTerminator&lt;mlir::gpu::YieldOp&gt;::Impl, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::RegionBranchOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x900cfe8)
#<!-- -->14 0x00005d9426aed9c7 mlir::RegisteredOperationName::Model&lt;mlir::gpu::WarpExecuteOnLane0Op&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x90029c7)
#<!-- -->15 0x00005d9427c6d76e (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) (.isra.0) Verifier.cpp:0:0
#<!-- -->16 0x00005d9427c6f818 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->17 0x00005d9427c6eac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->18 0x00005d9427c6f799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->19 0x00005d9427c6eac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->20 0x00005d9427c6f799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->21 0x00005d9427c706c8 mlir::verify(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0xa1856c8)
#<!-- -->22 0x00005d94261b140a mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c640a)
#<!-- -->23 0x00005d94200e0df7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->24 0x00005d94200e24ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->25 0x00005d9427c8a731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->26 0x00005d94200d98d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->27 0x00005d94200e2b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->28 0x00005d94200e2da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->29 0x00005d941ff14023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->30 0x00007885bc82a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->31 0x00007885bc82a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->32 0x00007885bc82a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->33 0x00005d941fff9405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

