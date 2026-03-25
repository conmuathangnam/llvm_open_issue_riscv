# [MLIR][MemRef] GenericAtomicRMW crashes when converting to LLVM when result op is defined outside the region

**Author:** ashermancinelli
**URL:** https://github.com/llvm/llvm-project/issues/172184
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2025-12-15T01:17:29Z

## Body

I was looking for an atomic exchange op, and before I realized I should be using `arith::AtomicRMWKind::assign`, I tried this which segfaults because of this part:
```c++
    // MemRefToLLVM.cpp
    // Clone the GenericAtomicRMWOp region and extract the result.
    auto loopArgument = loopBlock->getArgument(0);
    IRMapping mapping;
    mapping.map(atomicOp.getCurrentValue(), loopArgument);
    Block &entryBlock = atomicOp.body().front();
    for (auto &nestedOp : entryBlock.without_terminator()) {
      Operation *clone = rewriter.clone(nestedOp, mapping);
      mapping.map(nestedOp.getResults(), clone->getResults());
    }
    Value result = mapping.lookup(entryBlock.getTerminator()->getOperand(0));
```

```
module {
  func.func @test_atomic_exch(%arg0: memref<?xi32>, %idx: index, %value: i32) {
    %1 = memref.generic_atomic_rmw %arg0[%idx] : memref<?xi32> {
    ^bb0(%arg3: i32):
      memref.atomic_yield %value : i32
    }
    func.return
  }
}
```
```
$ mlir-opt t.mlir --convert-to-llvm
mlir-opt: /__w/eudsl/eudsl/third_party/llvm-project/mlir/include/mlir/IR/IRMapping.h:74: 
auto mlir::IRMapping::lookup(T) const [T = mlir::Value]: 
Assertion `result && "expected 'from' to be contained within the map"' failed.
  PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
  Stack dump:
  0.      Program arguments: mlir-opt t.mlir --convert-to-llvm --mlir-print-ir-after-all
   #0 0x00005c50ae55f238 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (mlir-opt+0x1c48238)
   #1 0x00005c50ae55c905 llvm::sys::RunSignalHandlers() (mlir-opt+0x1c45905)
   #2 0x00005c50ae560356 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
   #3 0x00007ff4dfe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
   #4 0x00007ff4dfe969fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
   #5 0x00007ff4dfe969fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
   #6 0x00007ff4dfe969fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
   #7 0x00007ff4dfe42476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
   #8 0x00007ff4dfe287f3 abort ./stdlib/./stdlib/abort.c:81:7
   #9 0x00007ff4dfe2871b _nl_load_domain ./intl/./intl/loadmsgcat.c:1177:9
  #10 0x00007ff4dfe39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #11 0x00005c50ae6b561b auto mlir::IRMapping::lookup<mlir::Value>(mlir::Value) const (mlir-opt+0x1d9e61b)
  #12 0x00005c50b11457df (anonymous namespace)::GenericAtomicRMWOpLowering::matchAndRewrite(mlir::memref::GenericAtomicRMWOp, mlir::memref::GenericAtomicRMWOpAdaptor, mlir::C
onversionPatternRewriter&) const MemRefToLLVM.cpp:0:0
  #13 0x00005c50b1145f72 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1<mlir::ConvertOpToLLVMPattern<mlir::memref::GenericAtomicRMWOp>, mlir::memref::GenericAtom
icRMWOp>(mlir::ConvertOpToLLVMPattern<mlir::memref::GenericAtomicRMWOp> const&, mlir::memref::GenericAtomicRMWOp, mlir::memref::GenericAtomicRMWOp::GenericAdaptor<llvm::Array
Ref<mlir::ValueRange> >, mlir::ConversionPatternRewriter&) (mlir-opt+0x482ef72)
  #14 0x00005c50b11452ca mlir::ConvertOpToLLVMPattern<mlir::memref::GenericAtomicRMWOp>::matchAndRewrite(mlir::Operation*, llvm::ArrayRef<mlir::ValueRange>, mlir::ConversionP
atternRewriter&) const (mlir-opt+0x482e2ca)
  #15 0x00005c50b41744e6 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&) const (mlir-opt+0x785d4e6)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Asher Mancinelli (ashermancinelli)

<details>
```
module {
  func.func @<!-- -->test_atomic_exch(%arg0: memref&lt;?xi32&gt;, %idx: index, %value: i32) {
    %1 = memref.generic_atomic_rmw %arg0[%idx] : memref&lt;?xi32&gt; {
    ^bb0(%arg3: i32):
      memref.atomic_yield %value : i32
    }
    func.return
  }
}
```
```
$ mlir-opt t.mlir --convert-to-llvm
mlir-opt: /__w/eudsl/eudsl/third_party/llvm-project/mlir/include/mlir/IR/IRMapping.h:74: 
auto mlir::IRMapping::lookup(T) const [T = mlir::Value]: 
Assertion `result &amp;&amp; "expected 'from' to be contained within the map"' failed.
  PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
  Stack dump:
  0.      Program arguments: mlir-opt t.mlir --convert-to-llvm --mlir-print-ir-after-all
   #<!-- -->0 0x00005c50ae55f238 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (mlir-opt+0x1c48238)
   #<!-- -->1 0x00005c50ae55c905 llvm::sys::RunSignalHandlers() (mlir-opt+0x1c45905)
   #<!-- -->2 0x00005c50ae560356 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
   #<!-- -->3 0x00007ff4dfe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
   #<!-- -->4 0x00007ff4dfe969fc __pthread_kill_implementation ./nptl/./nptl/pthread_kill.c:44:76
   #<!-- -->5 0x00007ff4dfe969fc __pthread_kill_internal ./nptl/./nptl/pthread_kill.c:78:10
   #<!-- -->6 0x00007ff4dfe969fc pthread_kill ./nptl/./nptl/pthread_kill.c:89:10
   #<!-- -->7 0x00007ff4dfe42476 gsignal ./signal/../sysdeps/posix/raise.c:27:6
   #<!-- -->8 0x00007ff4dfe287f3 abort ./stdlib/./stdlib/abort.c:81:7
   #<!-- -->9 0x00007ff4dfe2871b _nl_load_domain ./intl/./intl/loadmsgcat.c:1177:9
  #<!-- -->10 0x00007ff4dfe39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #<!-- -->11 0x00005c50ae6b561b auto mlir::IRMapping::lookup&lt;mlir::Value&gt;(mlir::Value) const (mlir-opt+0x1d9e61b)
  #<!-- -->12 0x00005c50b11457df (anonymous namespace)::GenericAtomicRMWOpLowering::matchAndRewrite(mlir::memref::GenericAtomicRMWOp, mlir::memref::GenericAtomicRMWOpAdaptor, mlir::C
onversionPatternRewriter&amp;) const MemRefToLLVM.cpp:0:0
  #<!-- -->13 0x00005c50b1145f72 llvm::LogicalResult mlir::ConversionPattern::dispatchTo1To1&lt;mlir::ConvertOpToLLVMPattern&lt;mlir::memref::GenericAtomicRMWOp&gt;, mlir::memref::GenericAtom
icRMWOp&gt;(mlir::ConvertOpToLLVMPattern&lt;mlir::memref::GenericAtomicRMWOp&gt; const&amp;, mlir::memref::GenericAtomicRMWOp, mlir::memref::GenericAtomicRMWOp::GenericAdaptor&lt;llvm::Array
Ref&lt;mlir::ValueRange&gt; &gt;, mlir::ConversionPatternRewriter&amp;) (mlir-opt+0x482ef72)
  #<!-- -->14 0x00005c50b11452ca mlir::ConvertOpToLLVMPattern&lt;mlir::memref::GenericAtomicRMWOp&gt;::matchAndRewrite(mlir::Operation*, llvm::ArrayRef&lt;mlir::ValueRange&gt;, mlir::ConversionP
atternRewriter&amp;) const (mlir-opt+0x482e2ca)
  #<!-- -->15 0x00005c50b41744e6 mlir::ConversionPattern::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;) const (mlir-opt+0x785d4e6)
```
</details>


---

### Comment 2 - ashermancinelli

Maybe a check to ensure the result is defined in the region with an error message would be appropriate here.

---

