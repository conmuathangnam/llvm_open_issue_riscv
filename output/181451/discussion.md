# mlir-opt crash: llvm.blockaddress verifier fails with dyn_cast on non-existent function

**Author:** mlirBug
**URL:** https://github.com/llvm/llvm-project/issues/181451
**Status:** Closed
**Labels:** mlir:llvm, crash
**Closed Date:** 2026-02-18T19:19:14Z

## Body

Bug Op: llvm.blockaddress

Bug Pass: N/A (General Verification)

Detailed Description:
The mlir-opt tool crashes when verifying an llvm.blockaddress operation that points to a non-existent symbol (e.g., @builtin.integer).

The BlockAddressOp::getBlockTagOp() method attempts to resolve the function reference provided in the function attribute. When the symbol table lookup fails to find the function, the pointer returned is null. The code then immediately performs an llvm::dyn_cast<mlir::LLVM::LLVMFuncOp> on this null value, which triggers the assertion: Assertion detail::isPresent(Val) && "dyn_cast on a non-existent value".

```mlir
module {
  irdl.dialect @builtin {
    irdl.attribute @integer_attr {
      %0 = irdl.base "!builtin.integer"
      %1 = llvm.blockaddress <function = @builtin.integer, tag = <id = 1>> : !llvm.ptr
    }
  }
}
```
```
mlir-opt temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From*) [with To = mlir::LLVM::LLVMFuncOp; From = mlir::Operation]: Assertion `detail::isPresent(Val) && "dyn_cast on a non-existent value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
 #0 0x00006318a9499e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x00006318a94965af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x00006318a94966fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000780890e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000780890e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000780890e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000780890e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000780890e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000780890e288ff abort ./stdlib/abort.c:81:7
 #9 0x0000780890e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000780890e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00006318b01e1ba9 mlir::LLVM::BlockAddressOp::getBlockTagOp() (llvmorg-22.1.0-install/bin/mlir-opt+0x9277ba9)
#12 0x00006318b03fe6b4 mlir::Op<mlir::LLVM::BlockAddressOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult<mlir::LLVM::LLVMPointerType>::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::ConstantLike, mlir::SymbolUserOpInterface::Trait>::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x94946b4)
#13 0x00006318b0451717 mlir::RegisteredOperationName::Model<mlir::LLVM::BlockAddressOp>::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x94e7717)
#14 0x00006318b10ec76e (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&) (.isra.0) Verifier.cpp:0:0
#15 0x00006318b10ee818 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#16 0x00006318b10edac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) Verifier.cpp:0:0
#17 0x00006318b10ee799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#18 0x00006318b10edac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&) Verifier.cpp:0:0
#19 0x00006318b10ee799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&) Verifier.cpp:0:0
#20 0x00006318b10ef6c8 mlir::verify(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0xa1856c8)
#21 0x00006318af63040a mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c640a)
#22 0x00006318a955fdf7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#23 0x00006318a95614ea llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#24 0x00006318b1109731 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#25 0x00006318a95588d0 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (.part.0) MlirOptMain.cpp:0:0
#26 0x00006318a9561b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#27 0x00006318a9561da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#28 0x00006318a9393023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#29 0x0000780890e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#30 0x0000780890e2a28b call_init ./csu/../csu/libc-start.c:128:20
#31 0x0000780890e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#32 0x00006318a9478405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (mlirBug)

<details>
Bug Op: llvm.blockaddress

Bug Pass: N/A (General Verification)

Detailed Description:
The mlir-opt tool crashes when verifying an llvm.blockaddress operation that points to a non-existent symbol (e.g., @<!-- -->builtin.integer).

The BlockAddressOp::getBlockTagOp() method attempts to resolve the function reference provided in the function attribute. When the symbol table lookup fails to find the function, the pointer returned is null. The code then immediately performs an llvm::dyn_cast&lt;mlir::LLVM::LLVMFuncOp&gt; on this null value, which triggers the assertion: Assertion detail::isPresent(Val) &amp;&amp; "dyn_cast on a non-existent value".

```mlir
module {
  irdl.dialect @<!-- -->builtin {
    irdl.attribute @<!-- -->integer_attr {
      %0 = irdl.base "!builtin.integer"
      %1 = llvm.blockaddress &lt;function = @<!-- -->builtin.integer, tag = &lt;id = 1&gt;&gt; : !llvm.ptr
    }
  }
}
```
```
mlir-opt temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From*) [with To = mlir::LLVM::LLVMFuncOp; From = mlir::Operation]: Assertion `detail::isPresent(Val) &amp;&amp; "dyn_cast on a non-existent value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
 #<!-- -->0 0x00006318a9499e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00006318a94965af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00006318a94966fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000780890e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000780890e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000780890e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000780890e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000780890e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000780890e288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x0000780890e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x0000780890e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00006318b01e1ba9 mlir::LLVM::BlockAddressOp::getBlockTagOp() (llvmorg-22.1.0-install/bin/mlir-opt+0x9277ba9)
#<!-- -->12 0x00006318b03fe6b4 mlir::Op&lt;mlir::LLVM::BlockAddressOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::LLVM::LLVMPointerType&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::ConstantLike, mlir::SymbolUserOpInterface::Trait&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x94946b4)
#<!-- -->13 0x00006318b0451717 mlir::RegisteredOperationName::Model&lt;mlir::LLVM::BlockAddressOp&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x94e7717)
#<!-- -->14 0x00006318b10ec76e (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) (.isra.0) Verifier.cpp:0:0
#<!-- -->15 0x00006318b10ee818 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->16 0x00006318b10edac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->17 0x00006318b10ee799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->18 0x00006318b10edac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->19 0x00006318b10ee799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->20 0x00006318b10ef6c8 mlir::verify(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0xa1856c8)
#<!-- -->21 0x00006318af63040a mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c640a)
#<!-- -->22 0x00006318a955fdf7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->23 0x00006318a95614ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->24 0x00006318b1109731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->25 0x00006318a95588d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->26 0x00006318a9561b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->27 0x00006318a9561da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->28 0x00006318a9393023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->29 0x0000780890e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->30 0x0000780890e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->31 0x0000780890e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->32 0x00006318a9478405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-llvm

Author: None (mlirBug)

<details>
Bug Op: llvm.blockaddress

Bug Pass: N/A (General Verification)

Detailed Description:
The mlir-opt tool crashes when verifying an llvm.blockaddress operation that points to a non-existent symbol (e.g., @<!-- -->builtin.integer).

The BlockAddressOp::getBlockTagOp() method attempts to resolve the function reference provided in the function attribute. When the symbol table lookup fails to find the function, the pointer returned is null. The code then immediately performs an llvm::dyn_cast&lt;mlir::LLVM::LLVMFuncOp&gt; on this null value, which triggers the assertion: Assertion detail::isPresent(Val) &amp;&amp; "dyn_cast on a non-existent value".

```mlir
module {
  irdl.dialect @<!-- -->builtin {
    irdl.attribute @<!-- -->integer_attr {
      %0 = irdl.base "!builtin.integer"
      %1 = llvm.blockaddress &lt;function = @<!-- -->builtin.integer, tag = &lt;id = 1&gt;&gt; : !llvm.ptr
    }
  }
}
```
```
mlir-opt temp.mlir
```
```
mlir-opt: llvm-project-llvmorg-22.1.0-rc1/llvm/include/llvm/Support/Casting.h:656: decltype(auto) llvm::dyn_cast(From*) [with To = mlir::LLVM::LLVMFuncOp; From = mlir::Operation]: Assertion `detail::isPresent(Val) &amp;&amp; "dyn_cast on a non-existent value"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt temp.mlir
 #<!-- -->0 0x00006318a9499e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x00006318a94965af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x00006318a94966fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000780890e45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000780890e9eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000780890e9eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000780890e9eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000780890e4527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000780890e288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x0000780890e2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x0000780890e3b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00006318b01e1ba9 mlir::LLVM::BlockAddressOp::getBlockTagOp() (llvmorg-22.1.0-install/bin/mlir-opt+0x9277ba9)
#<!-- -->12 0x00006318b03fe6b4 mlir::Op&lt;mlir::LLVM::BlockAddressOp, mlir::OpTrait::ZeroRegions, mlir::OpTrait::OneResult, mlir::OpTrait::OneTypedResult&lt;mlir::LLVM::LLVMPointerType&gt;::Impl, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::ConditionallySpeculatable::Trait, mlir::OpTrait::AlwaysSpeculatableImplTrait, mlir::MemoryEffectOpInterface::Trait, mlir::OpTrait::ConstantLike, mlir::SymbolUserOpInterface::Trait&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x94946b4)
#<!-- -->13 0x00006318b0451717 mlir::RegisteredOperationName::Model&lt;mlir::LLVM::BlockAddressOp&gt;::verifyInvariants(mlir::Operation*) (llvmorg-22.1.0-install/bin/mlir-opt+0x94e7717)
#<!-- -->14 0x00006318b10ec76e (anonymous namespace)::OperationVerifier::verifyOnEntrance(mlir::Operation&amp;) (.isra.0) Verifier.cpp:0:0
#<!-- -->15 0x00006318b10ee818 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->16 0x00006318b10edac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->17 0x00006318b10ee799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->18 0x00006318b10edac9 (anonymous namespace)::OperationVerifier::verifyOnExit(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->19 0x00006318b10ee799 (anonymous namespace)::OperationVerifier::verifyOperation(mlir::Operation&amp;) Verifier.cpp:0:0
#<!-- -->20 0x00006318b10ef6c8 mlir::verify(mlir::Operation*, bool) (llvmorg-22.1.0-install/bin/mlir-opt+0xa1856c8)
#<!-- -->21 0x00006318af63040a mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) (llvmorg-22.1.0-install/bin/mlir-opt+0x86c640a)
#<!-- -->22 0x00006318a955fdf7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->23 0x00006318a95614ea llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->24 0x00006318b1109731 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (llvmorg-22.1.0-install/bin/mlir-opt+0xa19f731)
#<!-- -->25 0x00006318a95588d0 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (.part.0) MlirOptMain.cpp:0:0
#<!-- -->26 0x00006318a9561b70 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7b70)
#<!-- -->27 0x00006318a9561da9 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (llvmorg-22.1.0-install/bin/mlir-opt+0x25f7da9)
#<!-- -->28 0x00006318a9393023 main (llvmorg-22.1.0-install/bin/mlir-opt+0x2429023)
#<!-- -->29 0x0000780890e2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->30 0x0000780890e2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->31 0x0000780890e2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->32 0x00006318a9478405 _start (llvmorg-22.1.0-install/bin/mlir-opt+0x250e405)
Aborted (core dumped)
```
</details>


---

