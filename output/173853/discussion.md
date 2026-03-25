# mlir-opt crashes in getLayoutAttr with affine.for iter_args and tensor.extract

**Author:** Subway2023
**URL:** https://github.com/llvm/llvm-project/issues/173853
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2025-12-31T08:30:32Z

## Body

## Description
The --remove-dead-values pass crashes when processing an scf.execute_region whose region contains explicit control flow via cf.br.

## Reproduce
mlir-opt version: 21.1.8
location: If the `cf.br ^bb1` and `^bb1: ` operation is commented out, the crash no longer occurs.
```mlir
module {
  func.func @main() {
    %0 = vector.constant_mask [1] : vector<1xi1>
    scf.execute_region {
      scf.yield
    }
    scf.execute_region {
      cf.br ^bb1
      ^bb1:  // pred: ^bb0
      scf.yield
    }
    scf.execute_region {
      scf.execute_region {
        scf.yield
      } {set = true}
      scf.yield
    }
    vector.print %0 : vector<1xi1>
    %c1_i32 = arith.constant 1 : i32
    return
  }
}
```
```
mlir-opt --remove-dead-values test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = mlir::RegionBranchTerminatorOpInterface; From = mlir::Operation]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --remove-dead-values /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250639_98f9_5.mlir
 #0 0x00005e7e3ed81612 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #1 0x00005e7e3ed7dd7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #2 0x00005e7e3ed7decc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000758eb5645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x0000758eb569eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000758eb569eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000758eb569eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000758eb564527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000758eb56288ff abort ./stdlib/abort.c:81:7
 #9 0x0000758eb562881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000758eb563b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005e7e42590b1b (anonymous namespace)::processRegionBranchOp(mlir::RegionBranchOpInterface, mlir::dataflow::RunLivenessAnalysis&, llvm::DenseSet<mlir::Value, llvm::DenseMapInfo<mlir::Value, void>>&, (anonymous namespace)::RDVFinalCleanupList&) RemoveDeadValues.cpp:0:0
#12 0x00005e7e42590bc8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.0) RemoveDeadValues.cpp:0:0
#13 0x00005e7e42590bc8 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) (.constprop.0) RemoveDeadValues.cpp:0:0
#14 0x00005e7e425910c0 (anonymous namespace)::RemoveDeadValues::runOnOperation() RemoveDeadValues.cpp:0:0
#15 0x00005e7e4253607e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#16 0x00005e7e425364c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#17 0x00005e7e42537523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#18 0x00005e7e425270a7 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) MlirOptMain.cpp:0:0
#19 0x00005e7e425279c6 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#20 0x00005e7e42527b8c llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::'lambda'(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&) MlirOptMain.cpp:0:0
#21 0x00005e7e4263d24e mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#22 0x00005e7e42520080 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#23 0x00005e7e42527cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#24 0x00005e7e425281ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#25 0x00005e7e3ec99577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#26 0x0000758eb562a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#27 0x0000758eb562a28b call_init ./csu/../csu/libc-start.c:128:20
#28 0x0000758eb562a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#29 0x00005e7e3ed52665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (Subway2023)

<details>
## Description
The --remove-dead-values pass crashes when processing an scf.execute_region whose region contains explicit control flow via cf.br.

## Reproduce
mlir-opt version: 21.1.8
location: If the `cf.br ^bb1` and `^bb1: ` operation is commented out, the crash no longer occurs.
```mlir
module {
  func.func @<!-- -->main() {
    %0 = vector.constant_mask [1] : vector&lt;1xi1&gt;
    scf.execute_region {
      scf.yield
    }
    scf.execute_region {
      cf.br ^bb1
      ^bb1:  // pred: ^bb0
      scf.yield
    }
    scf.execute_region {
      scf.execute_region {
        scf.yield
      } {set = true}
      scf.yield
    }
    vector.print %0 : vector&lt;1xi1&gt;
    %c1_i32 = arith.constant 1 : i32
    return
  }
}
```
```
mlir-opt --remove-dead-values test.mlir
```
```
mlir-opt: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = mlir::RegionBranchTerminatorOpInterface; From = mlir::Operation]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt --remove-dead-values /mnt/sdd1/sbw/mlir/data/genProgram_1224_filtered/genProgram_1224_valid/250639_98f9_5.mlir
 #<!-- -->0 0x00005e7e3ed81612 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x20a1612)
 #<!-- -->1 0x00005e7e3ed7dd7f llvm::sys::RunSignalHandlers() (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x209dd7f)
 #<!-- -->2 0x00005e7e3ed7decc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000758eb5645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x0000758eb569eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000758eb569eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000758eb569eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000758eb564527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000758eb56288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x0000758eb562881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x0000758eb563b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#<!-- -->11 0x00005e7e42590b1b (anonymous namespace)::processRegionBranchOp(mlir::RegionBranchOpInterface, mlir::dataflow::RunLivenessAnalysis&amp;, llvm::DenseSet&lt;mlir::Value, llvm::DenseMapInfo&lt;mlir::Value, void&gt;&gt;&amp;, (anonymous namespace)::RDVFinalCleanupList&amp;) RemoveDeadValues.cpp:0:0
#<!-- -->12 0x00005e7e42590bc8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) RemoveDeadValues.cpp:0:0
#<!-- -->13 0x00005e7e42590bc8 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) (.constprop.0) RemoveDeadValues.cpp:0:0
#<!-- -->14 0x00005e7e425910c0 (anonymous namespace)::RemoveDeadValues::runOnOperation() RemoveDeadValues.cpp:0:0
#<!-- -->15 0x00005e7e4253607e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x585607e)
#<!-- -->16 0x00005e7e425364c1 mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58564c1)
#<!-- -->17 0x00005e7e42537523 mlir::PassManager::run(mlir::Operation*) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5857523)
#<!-- -->18 0x00005e7e425270a7 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) MlirOptMain.cpp:0:0
#<!-- -->19 0x00005e7e425279c6 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, llvm::ThreadPoolInterface*) MlirOptMain.cpp:0:0
#<!-- -->20 0x00005e7e42527b8c llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::'lambda'(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;) MlirOptMain.cpp:0:0
#<!-- -->21 0x00005e7e4263d24e mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x595d24e)
#<!-- -->22 0x00005e7e42520080 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5840080)
#<!-- -->23 0x00005e7e42527cf0 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x5847cf0)
#<!-- -->24 0x00005e7e425281ef mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x58481ef)
#<!-- -->25 0x00005e7e3ec99577 main (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x1fb9577)
#<!-- -->26 0x0000758eb562a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->27 0x0000758eb562a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->28 0x0000758eb562a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->29 0x00005e7e3ed52665 _start (/mnt/sdd1/sbw/mlir/tool/llvm-project-llvmorg-21.1.8/build/bin/mlir-opt+0x2072665)
Aborted (core dumped)
```
</details>


---

### Comment 2 - sweiglbosker

Can't reproduce, I think this has been fixed

---

### Comment 3 - CoTinker

Fixed

---

