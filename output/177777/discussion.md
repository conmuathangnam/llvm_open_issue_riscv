# mlir-opt --xegpu-propagate-layout crashes on llvm.ptr cast to LLVMFunctionType

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/177777
**Status:** Closed
**Labels:** crash, mlir:xegpu
**Closed Date:** 2026-03-03T11:14:56Z

## Body

# Description
Running mlir-opt --xegpu-propagate-layout on a MLIR module containing llvm.ptr to function pointer produces an assertion failure in Casting.h

# Reproduce
mlir-opt version: 21.1.8
location: If the **llvm.func @foo()** is commented out, the crash no longer occurs.
```mlir
module {
  llvm.func @foo()
  func.func @main() {
    %0 = llvm.dso_local_equivalent @foo : !llvm.ptr
    return
  }
}
```
```
mlir-opt --xegpu-propagate-layout test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(const From&) [with To = mlir::LLVM::LLVMFunctionType; From = mlir::Type]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --xegpu-propagate-layout /mnt/sdd1/sbw/mlir/data/genProgram_0123_night_filerted/genProgram_0123_night_valid/241251_714e_0.mlir
 #0 0x00005cc323c00612 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #1 0x00005cc323bfcd7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #2 0x00005cc323bfcecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000750854445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000075085449eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000075085449eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000075085449eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000075085444527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007508544288ff abort ./stdlib/abort.c:81:7
 #9 0x000075085442881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000075085443b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005cc324ff9abd (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x349aabd)
#12 0x00005cc324ff9d71 mlir::detail::CallableOpInterfaceInterfaceTraits::Model<mlir::LLVM::LLVMFuncOp>::getArgumentTypes(mlir::detail::CallableOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x349ad71)
#13 0x00005cc32a61be22 mlir::function_interface_impl::setFunctionType(mlir::FunctionOpInterface, mlir::Type) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x8abce22)
#14 0x00005cc326ce6a20 (anonymous namespace)::XeGPUPropagateLayoutPass::runOnOperation()::'lambda'(mlir::Block*)::operator()(mlir::Block*) const XeGPUPropagateLayout.cpp:0:0
#15 0x00005cc326ce828b mlir::WalkResult mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<mlir::WalkResult (mlir::Block*)>, mlir::WalkOrder) (.constprop.0) XeGPUPropagateLayout.cpp:0:0
#16 0x00005cc326ce98ef (anonymous namespace)::XeGPUPropagateLayoutPass::runOnOperation() XeGPUPropagateLayout.cpp:0:0
#17 0x00005cc3273b507e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#18 0x00005cc3273b54c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#19 0x00005cc3273b6523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#20 0x00005cc3273a60a7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#21 0x00005cc3273a69c6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#22 0x00005cc3273a6b8c llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#23 0x00005cc3274bc24e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#24 0x00005cc32739f080 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#25 0x00005cc3273a6cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#26 0x00005cc3273a71ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#27 0x00005cc323b18577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#28 0x000075085442a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#29 0x000075085442a28b call_init ./csu/../csu/libc-start.c:128:20
#30 0x000075085442a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#31 0x00005cc323bd1665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
# Description
Running mlir-opt --xegpu-propagate-layout on a MLIR module containing llvm.ptr to function pointer produces an assertion failure in Casting.h

# Reproduce
mlir-opt version: 21.1.8
location: If the **llvm.func @<!-- -->foo()** is commented out, the crash no longer occurs.
```mlir
module {
  llvm.func @<!-- -->foo()
  func.func @<!-- -->main() {
    %0 = llvm.dso_local_equivalent @<!-- -->foo : !llvm.ptr
    return
  }
}
```
```
mlir-opt --xegpu-propagate-layout test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/llvm/include/llvm/Support/Casting.h:566: decltype(auto) llvm::cast(const From&amp;) [with To = mlir::LLVM::LLVMFunctionType; From = mlir::Type]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --xegpu-propagate-layout /mnt/sdd1/sbw/mlir/data/genProgram_0123_night_filerted/genProgram_0123_night_valid/241251_714e_0.mlir
 #<!-- -->0 0x00005cc323c00612 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #<!-- -->1 0x00005cc323bfcd7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #<!-- -->2 0x00005cc323bfcecc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000750854445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000075085449eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x000075085449eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x000075085449eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x000075085444527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007508544288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000075085442881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x000075085443b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005cc324ff9abd (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x349aabd)
#<!-- -->12 0x00005cc324ff9d71 mlir::detail::CallableOpInterfaceInterfaceTraits::Model&lt;mlir::LLVM::LLVMFuncOp&gt;::getArgumentTypes(mlir::detail::CallableOpInterfaceInterfaceTraits::Concept const*, mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x349ad71)
#<!-- -->13 0x00005cc32a61be22 mlir::function_interface_impl::setFunctionType(mlir::FunctionOpInterface, mlir::Type) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x8abce22)
#<!-- -->14 0x00005cc326ce6a20 (anonymous namespace)::XeGPUPropagateLayoutPass::runOnOperation()::'lambda'(mlir::Block*)::operator()(mlir::Block*) const XeGPUPropagateLayout.cpp:0:0
#<!-- -->15 0x00005cc326ce828b mlir::WalkResult mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;mlir::WalkResult (mlir::Block*)&gt;, mlir::WalkOrder) (.constprop.0) XeGPUPropagateLayout.cpp:0:0
#<!-- -->16 0x00005cc326ce98ef (anonymous namespace)::XeGPUPropagateLayoutPass::runOnOperation() XeGPUPropagateLayout.cpp:0:0
#<!-- -->17 0x00005cc3273b507e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#<!-- -->18 0x00005cc3273b54c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#<!-- -->19 0x00005cc3273b6523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#<!-- -->20 0x00005cc3273a60a7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->21 0x00005cc3273a69c6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#<!-- -->22 0x00005cc3273a6b8c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->23 0x00005cc3274bc24e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#<!-- -->24 0x00005cc32739f080 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#<!-- -->25 0x00005cc3273a6cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#<!-- -->26 0x00005cc3273a71ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#<!-- -->27 0x00005cc323b18577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#<!-- -->28 0x000075085442a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->29 0x000075085442a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->30 0x000075085442a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->31 0x00005cc323bd1665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.


---

### Comment 3 - joker-eph

Fixed at HEAD

---

### Comment 4 - joker-eph

Actually, fixing in #183899 I think

---

