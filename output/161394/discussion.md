# mlir-opt tool crashing when using gpu.launch operation with workgroup memory attributions

**Author:** Edward-EJ
**URL:** https://github.com/llvm/llvm-project/issues/161394
**Status:** Closed
**Labels:** mlir:gpu, crash
**Closed Date:** 2025-09-30T17:49:24Z

## Body

**version: llvm21.1.0 release**

**reproduce with:** ./bin/mlir-opt  example.mlir

The following mlir-opt tool crashes with the following IR:

`builtin.module{
    
    func.func @main(){    

    %c64 = arith.constant 64 : index
    %c0 = arith.constant 0 : index
    %c5 = arith.constant 5 : index
    %c1 = arith.constant 1 : index

    gpu.launch
    blocks (%bx, %by, %bz) in (%sz_bx = %c5, %sz_by = %c1, %sz_bz = %c1)
    threads (%tx, %ty, %tz) in (%sz_tx = %c64, %sz_ty = %c1, %sz_tz = %c1)
    workgroup(%alloc: memref<7x8xf64, #gpu.address_space<workgroup>>)
    { 
      gpu.terminator
    }
    return
    }
}`

trace: 
```
0.      Program arguments: /home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt ../examples/CheckingIR.mlir --debug
 #0 0x000059cd0c409148 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x19be148)
 #1 0x000059cd0c406875 llvm::sys::RunSignalHandlers() (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x19bb875)
 #2 0x000059cd0c40a251 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000742758c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000059cd1019d053 findParent(mlir::Operation*, bool) AsmPrinter.cpp:0:0
 #5 0x000059cd1019b901 mlir::Value::print(llvm::raw_ostream&, mlir::OpPrintingFlags const&) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5750901)
 #6 0x000059cd1019b8a8 mlir::Value::print(llvm::raw_ostream&) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x57508a8)
 #7 0x000059cd0cd6b454 llvm::support::detail::stream_operator_format_adapter<mlir::BlockArgument&>::format(llvm::raw_ostream&, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x2320454)
 #8 0x000059cd0c3f3c44 llvm::formatv_object_base::format(llvm::raw_ostream&) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x19a8c44)
 #9 0x000059cd0c3f3b7f llvm::raw_ostream::operator<<(llvm::formatv_object_base const&) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x19a8b7f)
#10 0x000059cd0cca1793 printAttributions(mlir::OpAsmPrinter&, llvm::StringRef, llvm::ArrayRef<mlir::BlockArgument>) GPUDialect.cpp:0:0
#11 0x000059cd0cca10d8 mlir::gpu::LaunchOp::print(mlir::OpAsmPrinter&) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x22560d8)
#12 0x000059cd0cdad7be mlir::Op<mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands<6u>::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects>::getPrintAssemblyFn()::'lambda'(mlir::Operation*, mlir::OpAsmPrinter&, llvm::StringRef)::operator()(mlir::Operation*, mlir::OpAsmPrinter&, llvm::StringRef) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x23627be)
#13 0x000059cd0cdabe21 mlir::RegisteredOperationName::Model<mlir::gpu::LaunchOp>::printAssembly(mlir::Operation*, mlir::OpAsmPrinter&, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x2360e21)
#14 0x000059cd101a65cd (anonymous namespace)::DummyAliasOperationPrinter::print(mlir::Block*, bool, bool) AsmPrinter.cpp:0:0
#15 0x000059cd101a4290 (anonymous namespace)::DummyAliasOperationPrinter::printRegion(mlir::Region&, bool, bool, bool) AsmPrinter.cpp:0:0
#16 0x000059cd137925f9 mlir::function_interface_impl::printFunctionOp(mlir::OpAsmPrinter&, mlir::FunctionOpInterface, bool, llvm::StringRef, mlir::StringAttr, mlir::StringAttr) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x8d475f9)
#17 0x000059cd0cc5d9be mlir::func::FuncOp::print(mlir::OpAsmPrinter&) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x22129be)
#18 0x000059cd0cc7947e mlir::Op<mlir::func::FuncOp, mlir::OpTrait::OneRegion, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::SymbolOpInterface::Trait, mlir::CallableOpInterface::Trait, mlir::FunctionOpInterface::Trait, mlir::OpTrait::IsIsolatedFromAbove, mlir::OpAsmOpInterface::Trait>::getPrintAssemblyFn()::'lambda'(mlir::Operation*, mlir::OpAsmPrinter&, llvm::StringRef)::operator()(mlir::Operation*, mlir::OpAsmPrinter&, llvm::StringRef) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x222e47e)
#19 0x000059cd0cc75661 mlir::RegisteredOperationName::Model<mlir::func::FuncOp>::printAssembly(mlir::Operation*, mlir::OpAsmPrinter&, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x222a661)
#20 0x000059cd101a65cd (anonymous namespace)::DummyAliasOperationPrinter::print(mlir::Block*, bool, bool) AsmPrinter.cpp:0:0
#21 0x000059cd101a4290 (anonymous namespace)::DummyAliasOperationPrinter::printRegion(mlir::Region&, bool, bool, bool) AsmPrinter.cpp:0:0
#22 0x000059cd101fac80 mlir::ModuleOp::print(mlir::OpAsmPrinter&) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x57afc80)
#23 0x000059cd101fec9e mlir::Op<mlir::ModuleOp, mlir::OpTrait::OneRegion, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::NoRegionArguments, mlir::OpTrait::NoTerminator, mlir::OpTrait::SingleBlock, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::IsIsolatedFromAbove, mlir::OpTrait::SymbolTable, mlir::SymbolOpInterface::Trait, mlir::OpAsmOpInterface::Trait, mlir::RegionKindInterface::Trait, mlir::OpTrait::HasOnlyGraphRegion>::getPrintAssemblyFn()::'lambda'(mlir::Operation*, mlir::OpAsmPrinter&, llvm::StringRef)::operator()(mlir::Operation*, mlir::OpAsmPrinter&, llvm::StringRef) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x57b3c9e)
#24 0x000059cd101fc141 mlir::RegisteredOperationName::Model<mlir::ModuleOp>::printAssembly(mlir::Operation*, mlir::OpAsmPrinter&, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x57b1141)
#25 0x000059cd1019d236 mlir::detail::AsmStateImpl::initializeAliases(mlir::Operation*) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5752236)
#26 0x000059cd1019bf16 mlir::Operation::print(llvm::raw_ostream&, mlir::AsmState&) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5750f16)
#27 0x000059cd100ca42c performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#28 0x000059cd100c9bc0 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#29 0x000059cd10181165 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5736165)
#30 0x000059cd100c2cf5 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5677cf5)
#31 0x000059cd100c2fa8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5677fa8)
#32 0x000059cd100c31c2 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x56781c2)
#33 0x000059cd0c3e8cb7 main (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x199dcb7)
#34 0x0000742758c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#35 0x0000742758c2a28b call_init ./csu/../csu/libc-start.c:128:20
#36 0x0000742758c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#37 0x000059cd0c3e8755 _start (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x199d755)
Segmentation fault (core dumped)
```

Notably, if you remove the workgroup attribution on the gpu.launch operation, everything is fine. 

I apologise for the poor formatting here, this is the first issue I've submitted.





## Comments

### Comment 1 - joker-eph

The issue is:
```
  auto printBlockArg = [](BlockArgument v) {
    return llvm::formatv("{} : {}", v, v.getType());
  };
```

formatv is like a StringRef, it can't be returned. Sad that ASAN didn't catch this before!

(for the GitHub formatting, use triple backticks for verbatim sections)

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-mlir-gpu

Author: None (Edward-EJ)

<details>
**version: llvm21.1.0 release**

**reproduce with:** ./bin/mlir-opt  example.mlir

The following mlir-opt tool crashes with the following IR:

`builtin.module{
    
    func.func @<!-- -->main(){    

    %c64 = arith.constant 64 : index
    %c0 = arith.constant 0 : index
    %c5 = arith.constant 5 : index
    %c1 = arith.constant 1 : index

    gpu.launch
    blocks (%bx, %by, %bz) in (%sz_bx = %c5, %sz_by = %c1, %sz_bz = %c1)
    threads (%tx, %ty, %tz) in (%sz_tx = %c64, %sz_ty = %c1, %sz_tz = %c1)
    workgroup(%alloc: memref&lt;7x8xf64, #gpu.address_space&lt;workgroup&gt;&gt;)
    { 
      gpu.terminator
    }
    return
    }
}`

trace: 
```
0.      Program arguments: /home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt ../examples/CheckingIR.mlir --debug
 #<!-- -->0 0x000059cd0c409148 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x19be148)
 #<!-- -->1 0x000059cd0c406875 llvm::sys::RunSignalHandlers() (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x19bb875)
 #<!-- -->2 0x000059cd0c40a251 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000742758c45330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000059cd1019d053 findParent(mlir::Operation*, bool) AsmPrinter.cpp:0:0
 #<!-- -->5 0x000059cd1019b901 mlir::Value::print(llvm::raw_ostream&amp;, mlir::OpPrintingFlags const&amp;) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5750901)
 #<!-- -->6 0x000059cd1019b8a8 mlir::Value::print(llvm::raw_ostream&amp;) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x57508a8)
 #<!-- -->7 0x000059cd0cd6b454 llvm::support::detail::stream_operator_format_adapter&lt;mlir::BlockArgument&amp;&gt;::format(llvm::raw_ostream&amp;, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x2320454)
 #<!-- -->8 0x000059cd0c3f3c44 llvm::formatv_object_base::format(llvm::raw_ostream&amp;) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x19a8c44)
 #<!-- -->9 0x000059cd0c3f3b7f llvm::raw_ostream::operator&lt;&lt;(llvm::formatv_object_base const&amp;) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x19a8b7f)
#<!-- -->10 0x000059cd0cca1793 printAttributions(mlir::OpAsmPrinter&amp;, llvm::StringRef, llvm::ArrayRef&lt;mlir::BlockArgument&gt;) GPUDialect.cpp:0:0
#<!-- -->11 0x000059cd0cca10d8 mlir::gpu::LaunchOp::print(mlir::OpAsmPrinter&amp;) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x22560d8)
#<!-- -->12 0x000059cd0cdad7be mlir::Op&lt;mlir::gpu::LaunchOp, mlir::OpTrait::OneRegion, mlir::OpTrait::VariadicResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::AtLeastNOperands&lt;6u&gt;::Impl, mlir::OpTrait::AttrSizedOperandSegments, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::InferIntRangeInterface::Trait, mlir::gpu::AsyncOpInterface::Trait, mlir::OpTrait::HasRecursiveMemoryEffects&gt;::getPrintAssemblyFn()::'lambda'(mlir::Operation*, mlir::OpAsmPrinter&amp;, llvm::StringRef)::operator()(mlir::Operation*, mlir::OpAsmPrinter&amp;, llvm::StringRef) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x23627be)
#<!-- -->13 0x000059cd0cdabe21 mlir::RegisteredOperationName::Model&lt;mlir::gpu::LaunchOp&gt;::printAssembly(mlir::Operation*, mlir::OpAsmPrinter&amp;, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x2360e21)
#<!-- -->14 0x000059cd101a65cd (anonymous namespace)::DummyAliasOperationPrinter::print(mlir::Block*, bool, bool) AsmPrinter.cpp:0:0
#<!-- -->15 0x000059cd101a4290 (anonymous namespace)::DummyAliasOperationPrinter::printRegion(mlir::Region&amp;, bool, bool, bool) AsmPrinter.cpp:0:0
#<!-- -->16 0x000059cd137925f9 mlir::function_interface_impl::printFunctionOp(mlir::OpAsmPrinter&amp;, mlir::FunctionOpInterface, bool, llvm::StringRef, mlir::StringAttr, mlir::StringAttr) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x8d475f9)
#<!-- -->17 0x000059cd0cc5d9be mlir::func::FuncOp::print(mlir::OpAsmPrinter&amp;) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x22129be)
#<!-- -->18 0x000059cd0cc7947e mlir::Op&lt;mlir::func::FuncOp, mlir::OpTrait::OneRegion, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::AutomaticAllocationScope, mlir::SymbolOpInterface::Trait, mlir::CallableOpInterface::Trait, mlir::FunctionOpInterface::Trait, mlir::OpTrait::IsIsolatedFromAbove, mlir::OpAsmOpInterface::Trait&gt;::getPrintAssemblyFn()::'lambda'(mlir::Operation*, mlir::OpAsmPrinter&amp;, llvm::StringRef)::operator()(mlir::Operation*, mlir::OpAsmPrinter&amp;, llvm::StringRef) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x222e47e)
#<!-- -->19 0x000059cd0cc75661 mlir::RegisteredOperationName::Model&lt;mlir::func::FuncOp&gt;::printAssembly(mlir::Operation*, mlir::OpAsmPrinter&amp;, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x222a661)
#<!-- -->20 0x000059cd101a65cd (anonymous namespace)::DummyAliasOperationPrinter::print(mlir::Block*, bool, bool) AsmPrinter.cpp:0:0
#<!-- -->21 0x000059cd101a4290 (anonymous namespace)::DummyAliasOperationPrinter::printRegion(mlir::Region&amp;, bool, bool, bool) AsmPrinter.cpp:0:0
#<!-- -->22 0x000059cd101fac80 mlir::ModuleOp::print(mlir::OpAsmPrinter&amp;) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x57afc80)
#<!-- -->23 0x000059cd101fec9e mlir::Op&lt;mlir::ModuleOp, mlir::OpTrait::OneRegion, mlir::OpTrait::ZeroResults, mlir::OpTrait::ZeroSuccessors, mlir::OpTrait::ZeroOperands, mlir::OpTrait::NoRegionArguments, mlir::OpTrait::NoTerminator, mlir::OpTrait::SingleBlock, mlir::OpTrait::OpInvariants, mlir::BytecodeOpInterface::Trait, mlir::OpTrait::AffineScope, mlir::OpTrait::IsIsolatedFromAbove, mlir::OpTrait::SymbolTable, mlir::SymbolOpInterface::Trait, mlir::OpAsmOpInterface::Trait, mlir::RegionKindInterface::Trait, mlir::OpTrait::HasOnlyGraphRegion&gt;::getPrintAssemblyFn()::'lambda'(mlir::Operation*, mlir::OpAsmPrinter&amp;, llvm::StringRef)::operator()(mlir::Operation*, mlir::OpAsmPrinter&amp;, llvm::StringRef) const (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x57b3c9e)
#<!-- -->24 0x000059cd101fc141 mlir::RegisteredOperationName::Model&lt;mlir::ModuleOp&gt;::printAssembly(mlir::Operation*, mlir::OpAsmPrinter&amp;, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x57b1141)
#<!-- -->25 0x000059cd1019d236 mlir::detail::AsmStateImpl::initializeAliases(mlir::Operation*) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5752236)
#<!-- -->26 0x000059cd1019bf16 mlir::Operation::print(llvm::raw_ostream&amp;, mlir::AsmState&amp;) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5750f16)
#<!-- -->27 0x000059cd100ca42c performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->28 0x000059cd100c9bc0 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->29 0x000059cd10181165 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5736165)
#<!-- -->30 0x000059cd100c2cf5 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5677cf5)
#<!-- -->31 0x000059cd100c2fa8 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x5677fa8)
#<!-- -->32 0x000059cd100c31c2 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x56781c2)
#<!-- -->33 0x000059cd0c3e8cb7 main (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x199dcb7)
#<!-- -->34 0x0000742758c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->35 0x0000742758c2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->36 0x0000742758c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->37 0x000059cd0c3e8755 _start (/home/edward/projects/LLVMs/llvm-project-llvmorg-21.1.0/build/bin/mlir-opt+0x199d755)
Segmentation fault (core dumped)
```

Notably, if you remove the workgroup attribution on the gpu.launch operation, everything is fine. 

I apologise for the poor formatting here, this is the first issue I've submitted.




</details>


---

