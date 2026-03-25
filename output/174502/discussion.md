# [MLIR] DCE crashes on CIR

**Author:** badumbatish
**URL:** https://github.com/llvm/llvm-project/issues/174502
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-23T09:36:24Z

## Body

Hello, i tried to run the remove-dead-values pass on the following test case (ClangIR dialect) and it's crashing. Below is the reproducer


Test file 
```c
int switch_all_return(int x) {
  switch (x) {
  case 0:
    return 0;
  case 1:
    return 1;
  default:
    return -1;
  }
}
```

Git Branch: main
Git Commit: fb8bbd4ed89e
Git Repo: git@github.com:badumbatish/llvm-project.git
Command: ./build_debug/bin/clang -fclangir scratch/switch_all_return.c -emit-cir -o - | ./build_debug/bin/cir-opt --remove-dead-values
Exit Code: 134
Duration: 3.2s

```
cir-opt: ../llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From *) [To = mlir::RegionBranchTerminatorOpInterface, From = mlir::Operation]: Assertion `isa<To>(Val) && "cast<Ty>() ar
gument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build_debug/bin/cir-opt --remove-dead-values
 #0 0x00005561c52a5d8d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00005561c52a62bb PrintStackTraceSignalHandler(void*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00005561c52a4174 llvm::sys::RunSignalHandlers() /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/lib/Support/Signals.cpp:108:5
 #3 0x00005561c52a68e9 SignalHandler(int, siginfo_t*, void*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007fc951a3e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #5 0x00007fc951a9890c (/usr/lib/libc.so.6+0x9890c)
 #6 0x00007fc951a3e3a0 raise (/usr/lib/libc.so.6+0x3e3a0)
 #7 0x00007fc951a2557a abort (/usr/lib/libc.so.6+0x2557a)
 #8 0x00007fc951a254e3 __assert_perror_fail (/usr/lib/libc.so.6+0x254e3)
 #9 0x00005561c1ac2f94 decltype(auto) llvm::cast<mlir::RegionBranchTerminatorOpInterface, mlir::Operation>(mlir::Operation*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/include/ll
vm/Support/Casting.h:573:39
#10 0x00005561c2d9427b (anonymous namespace)::processRegionBranchOp(mlir::RegionBranchOpInterface, mlir::dataflow::RunLivenessAnalysis&, llvm::DenseSet<mlir::Value, llvm::DenseMapInfo<mlir::Value, voi
d>>&, (anonymous namespace)::RDVFinalCleanupList&)::$_5::operator()(llvm::DenseMap<mlir::Operation*, llvm::BitVector, llvm::DenseMapInfo<mlir::Operation*, void>, llvm::detail::DenseMapPair<mlir::Opera
tion*, llvm::BitVector>>&) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Transforms/RemoveDeadValues.cpp:480:29
#11 0x00005561c2d8fb91 (anonymous namespace)::processRegionBranchOp(mlir::RegionBranchOpInterface, mlir::dataflow::RunLivenessAnalysis&, llvm::DenseSet<mlir::Value, llvm::DenseMapInfo<mlir::Value, voi
d>>&, (anonymous namespace)::RDVFinalCleanupList&) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Transforms/RemoveDeadValues.cpp:665:3
#12 0x00005561c2d8e9d0 (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0::operator()(mlir::Operation*) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Transform
s/RemoveDeadValues.cpp:941:5
#13 0x00005561c2d8e8fd void llvm::function_ref<void (mlir::Operation*)>::callback_fn<(anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0>(long, mlir::Operation*) /home/jjasmine/Developer/ig
alia/llvm-project/build_debug/../llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#14 0x00005561c1bb8d61 llvm::function_ref<void (mlir::Operation*)>::operator()(mlir::Operation*) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/include/llvm/ADT/STLFunctionalEx
tras.h:69:5
#15 0x00005561c1bb8d2d void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:153:1
#16 0x00005561c1bb8ce9 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:145:27
#17 0x00005561c1bb8ce9 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:145:27
#18 0x00005561c1bb8ce9 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:145:27
#19 0x00005561c1bb8ce9 void mlir::detail::walk<mlir::ForwardIterator>(mlir::Operation*, llvm::function_ref<void (mlir::Operation*)>, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:145:27
#20 0x00005561c2d8e8a2 std::enable_if<llvm::is_one_of<mlir::Operation*, mlir::Operation*, mlir::Region*, mlir::Block*>::value, void>::type mlir::detail::walk<(mlir::WalkOrder)1, mlir::ForwardIterator,
 (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0, mlir::Operation*, void>(mlir::Operation*, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&&) /home/jjasmine/Developer/ig
alia/llvm-project/build_debug/../mlir/include/mlir/IR/Visitors.h:278:3
#21 0x00005561c2d8c68d std::enable_if<llvm::function_traits<std::decay<(anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0>::type>::num_args == 1, void>::type mlir::Operation::walk<(mlir::W
alkOrder)1, mlir::ForwardIterator, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0, void>((anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&&) /home/jjasmine/Developer/igal
ia/llvm-project/build_debug/../mlir/include/mlir/IR/Operation.h:798:5
#22 0x00005561c2d8c4b7 (anonymous namespace)::RemoveDeadValues::runOnOperation() /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Transforms/RemoveDeadValues.cpp:954:3
#23 0x00005561c2cd5514 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/jjasmine/Developer/igalia/llvm-proj
ect/build_debug/../mlir/lib/Pass/Pass.cpp:0:19
#24 0x00005561c2cd54b5 void llvm::function_ref<void ()>::callback_fn<mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3>(long) /home/jj
asmine/Developer/igalia/llvm-project/build_debug/../llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#25 0x00005561c29cbed9 llvm::function_ref<void ()>::operator()() const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#26 0x00005561c2cd8e0b void mlir::MLIRContext::executeAction<mlir::PassExecutionAction, mlir::Pass&>(llvm::function_ref<void ()>, llvm::ArrayRef<mlir::IRUnit>, mlir::Pass&) /home/jjasmine/Developer/ig
alia/llvm-project/build_debug/../mlir/include/mlir/IR/MLIRContext.h:291:3
#27 0x00005561c2ccd24e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/l
ib/Pass/Pass.cpp:619:17
#28 0x00005561c2ccd9aa mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentatio
n::PipelineParentInfo const*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Pass/Pass.cpp:688:16
#29 0x00005561c2cd32ad mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Pass/Pass.cpp:1123:10
#30 0x00005561c2cd2836 mlir::PassManager::run(mlir::Operation*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Pass/Pass.cpp:1097:60
#31 0x00005561c2c1210b performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /home/jjasmine/Developer/igalia/llvm-project/buil
d_debug/../mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#32 0x00005561c2c117e1 processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::Dial
ectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#33 0x00005561c2c1149c mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0:
:operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../m
lir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#34 0x00005561c2c113a6 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw
_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_
0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llv
m/include/llvm/ADT/STLFunctionalExtras.h:46:12
#35 0x00005561c4ce527a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operato
r()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../ll
vm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#36 0x00005561c4ce463a mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuf
fer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /home/jjasmine/Developer/igalia/llvm-project/bu
ild_debug/../mlir/lib/Support/ToolUtilities.cpp:30:12
#37 0x00005561c2c0ce36 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /home
/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#38 0x00005561c2c0d235 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Tools/mlir-opt/Mlir
OptMain.cpp:822:14
#39 0x00005561c2c0d408 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:1
0
#40 0x00005561c16e4e56 main /home/jjasmine/Developer/igalia/llvm-project/build_debug/../clang/tools/cir-opt/cir-opt.cpp:67:33
#41 0x00007fc951a27635 (/usr/lib/libc.so.6+0x27635)
#42 0x00007fc951a276e9 __libc_start_main (/usr/lib/libc.so.6+0x276e9)
#43 0x00005561c16e4bb5 _start (./build_debug/bin/cir-opt+0x237bbb5)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Jasmine Tang (badumbatish)

<details>
Hello, i tried to run the remove-dead-values pass on the following test case and it's crashing.


Test file 
```c
int switch_all_return(int x) {
  switch (x) {
  case 0:
    return 0;
  case 1:
    return 1;
  default:
    return -1;
  }
}
```

Git Branch: main
Git Commit: fb8bbd4ed89e
Git Repo: git@<!-- -->github.com:badumbatish/llvm-project.git
Command: ./build_debug/bin/clang -fclangir scratch/switch_all_return.c -emit-cir -o - | ./build_debug/bin/cir-opt --remove-dead-values
Exit Code: 134
Duration: 3.2s

```
cir-opt: ../llvm/include/llvm/Support/Casting.h:572: decltype(auto) llvm::cast(From *) [To = mlir::RegionBranchTerminatorOpInterface, From = mlir::Operation]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() ar
gument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ./build_debug/bin/cir-opt --remove-dead-values
 #<!-- -->0 0x00005561c52a5d8d llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00005561c52a62bb PrintStackTraceSignalHandler(void*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00005561c52a4174 llvm::sys::RunSignalHandlers() /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00005561c52a68e9 SignalHandler(int, siginfo_t*, void*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007fc951a3e4d0 (/usr/lib/libc.so.6+0x3e4d0)
 #<!-- -->5 0x00007fc951a9890c (/usr/lib/libc.so.6+0x9890c)
 #<!-- -->6 0x00007fc951a3e3a0 raise (/usr/lib/libc.so.6+0x3e3a0)
 #<!-- -->7 0x00007fc951a2557a abort (/usr/lib/libc.so.6+0x2557a)
 #<!-- -->8 0x00007fc951a254e3 __assert_perror_fail (/usr/lib/libc.so.6+0x254e3)
 #<!-- -->9 0x00005561c1ac2f94 decltype(auto) llvm::cast&lt;mlir::RegionBranchTerminatorOpInterface, mlir::Operation&gt;(mlir::Operation*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/include/ll
vm/Support/Casting.h:573:39
#<!-- -->10 0x00005561c2d9427b (anonymous namespace)::processRegionBranchOp(mlir::RegionBranchOpInterface, mlir::dataflow::RunLivenessAnalysis&amp;, llvm::DenseSet&lt;mlir::Value, llvm::DenseMapInfo&lt;mlir::Value, voi
d&gt;&gt;&amp;, (anonymous namespace)::RDVFinalCleanupList&amp;)::$_5::operator()(llvm::DenseMap&lt;mlir::Operation*, llvm::BitVector, llvm::DenseMapInfo&lt;mlir::Operation*, void&gt;, llvm::detail::DenseMapPair&lt;mlir::Opera
tion*, llvm::BitVector&gt;&gt;&amp;) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Transforms/RemoveDeadValues.cpp:480:29
#<!-- -->11 0x00005561c2d8fb91 (anonymous namespace)::processRegionBranchOp(mlir::RegionBranchOpInterface, mlir::dataflow::RunLivenessAnalysis&amp;, llvm::DenseSet&lt;mlir::Value, llvm::DenseMapInfo&lt;mlir::Value, voi
d&gt;&gt;&amp;, (anonymous namespace)::RDVFinalCleanupList&amp;) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Transforms/RemoveDeadValues.cpp:665:3
#<!-- -->12 0x00005561c2d8e9d0 (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0::operator()(mlir::Operation*) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Transform
s/RemoveDeadValues.cpp:941:5
#<!-- -->13 0x00005561c2d8e8fd void llvm::function_ref&lt;void (mlir::Operation*)&gt;::callback_fn&lt;(anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&gt;(long, mlir::Operation*) /home/jjasmine/Developer/ig
alia/llvm-project/build_debug/../llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->14 0x00005561c1bb8d61 llvm::function_ref&lt;void (mlir::Operation*)&gt;::operator()(mlir::Operation*) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/include/llvm/ADT/STLFunctionalEx
tras.h:69:5
#<!-- -->15 0x00005561c1bb8d2d void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:153:1
#<!-- -->16 0x00005561c1bb8ce9 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:145:27
#<!-- -->17 0x00005561c1bb8ce9 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:145:27
#<!-- -->18 0x00005561c1bb8ce9 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:145:27
#<!-- -->19 0x00005561c1bb8ce9 void mlir::detail::walk&lt;mlir::ForwardIterator&gt;(mlir::Operation*, llvm::function_ref&lt;void (mlir::Operation*)&gt;, mlir::WalkOrder) /home/jjasmine/Developer/igalia/llvm-project/build
_debug/../mlir/include/mlir/IR/Visitors.h:145:27
#<!-- -->20 0x00005561c2d8e8a2 std::enable_if&lt;llvm::is_one_of&lt;mlir::Operation*, mlir::Operation*, mlir::Region*, mlir::Block*&gt;::value, void&gt;::type mlir::detail::walk&lt;(mlir::WalkOrder)1, mlir::ForwardIterator,
 (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0, mlir::Operation*, void&gt;(mlir::Operation*, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&amp;&amp;) /home/jjasmine/Developer/ig
alia/llvm-project/build_debug/../mlir/include/mlir/IR/Visitors.h:278:3
#<!-- -->21 0x00005561c2d8c68d std::enable_if&lt;llvm::function_traits&lt;std::decay&lt;(anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&gt;::type&gt;::num_args == 1, void&gt;::type mlir::Operation::walk&lt;(mlir::W
alkOrder)1, mlir::ForwardIterator, (anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0, void&gt;((anonymous namespace)::RemoveDeadValues::runOnOperation()::$_0&amp;&amp;) /home/jjasmine/Developer/igal
ia/llvm-project/build_debug/../mlir/include/mlir/IR/Operation.h:798:5
#<!-- -->22 0x00005561c2d8c4b7 (anonymous namespace)::RemoveDeadValues::runOnOperation() /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Transforms/RemoveDeadValues.cpp:954:3
#<!-- -->23 0x00005561c2cd5514 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3::operator()() const /home/jjasmine/Developer/igalia/llvm-proj
ect/build_debug/../mlir/lib/Pass/Pass.cpp:0:19
#<!-- -->24 0x00005561c2cd54b5 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int)::$_3&gt;(long) /home/jj
asmine/Developer/igalia/llvm-project/build_debug/../llvm/include/llvm/ADT/STLFunctionalExtras.h:46:5
#<!-- -->25 0x00005561c29cbed9 llvm::function_ref&lt;void ()&gt;::operator()() const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llvm/include/llvm/ADT/STLFunctionalExtras.h:69:5
#<!-- -->26 0x00005561c2cd8e0b void mlir::MLIRContext::executeAction&lt;mlir::PassExecutionAction, mlir::Pass&amp;&gt;(llvm::function_ref&lt;void ()&gt;, llvm::ArrayRef&lt;mlir::IRUnit&gt;, mlir::Pass&amp;) /home/jjasmine/Developer/ig
alia/llvm-project/build_debug/../mlir/include/mlir/IR/MLIRContext.h:291:3
#<!-- -->27 0x00005561c2ccd24e mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/l
ib/Pass/Pass.cpp:619:17
#<!-- -->28 0x00005561c2ccd9aa mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentatio
n::PipelineParentInfo const*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Pass/Pass.cpp:688:16
#<!-- -->29 0x00005561c2cd32ad mlir::PassManager::runPasses(mlir::Operation*, mlir::AnalysisManager) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Pass/Pass.cpp:1123:10
#<!-- -->30 0x00005561c2cd2836 mlir::PassManager::run(mlir::Operation*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Pass/Pass.cpp:1097:60
#<!-- -->31 0x00005561c2c1210b performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /home/jjasmine/Developer/igalia/llvm-project/buil
d_debug/../mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:591:17
#<!-- -->32 0x00005561c2c117e1 processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::Dial
ectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->33 0x00005561c2c1149c mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0:
:operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../m
lir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->34 0x00005561c2c113a6 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw
_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_
0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../llv
m/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->35 0x00005561c4ce527a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operato
r()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /home/jjasmine/Developer/igalia/llvm-project/build_debug/../ll
vm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->36 0x00005561c4ce463a mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuf
fer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /home/jjasmine/Developer/igalia/llvm-project/bu
ild_debug/../mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->37 0x00005561c2c0ce36 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /home
/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->38 0x00005561c2c0d235 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Tools/mlir-opt/Mlir
OptMain.cpp:822:14
#<!-- -->39 0x00005561c2c0d408 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /home/jjasmine/Developer/igalia/llvm-project/build_debug/../mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:1
0
#<!-- -->40 0x00005561c16e4e56 main /home/jjasmine/Developer/igalia/llvm-project/build_debug/../clang/tools/cir-opt/cir-opt.cpp:67:33
#<!-- -->41 0x00007fc951a27635 (/usr/lib/libc.so.6+0x27635)
#<!-- -->42 0x00007fc951a276e9 __libc_start_main (/usr/lib/libc.so.6+0x276e9)
#<!-- -->43 0x00005561c16e4bb5 _start (./build_debug/bin/cir-opt+0x237bbb5)
```
</details>


---

### Comment 2 - Men-cotton

https://github.com/llvm/llvm-project/pull/173505 resolved this crash.

---

