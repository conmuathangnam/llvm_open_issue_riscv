# mlir-opt crash in WhileConditionTruth pattern during --remove-dead-values

**Author:** compilerStudyer
**URL:** https://github.com/llvm/llvm-project/issues/179944
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2026-02-16T19:46:38Z

## Body

**Bug Op**: scf.while 

**Bug Pass**: --remove-dead-values

**Commenting out does not trigger the crash**: smt.solver

**Detailed Description**: The pass crashes when rewriting nested scf.while loops that return vector types. The issue is located in the WhileConditionTruth pattern. The crash is only reproducible when the loop results are "live" (e.g., used by an smt.solver op); otherwise, DCE removes the loops and avoids the buggy code path.

```mlir
module {
  func.func @main() {
    %0 = gpu.dynamic_shared_memory : memref<?xi8, #gpu.address_space<workgroup>>
    %1 = vector.step : vector<1xindex>
    %2 = scf.while (%arg0 = %1) : (vector<1xindex>) -> vector<1xindex> {
      %cond = arith.constant true
      scf.condition(%cond) %arg0 : vector<1xindex>
    } do {
      ^bb0(%arg0: vector<1xindex>):
      scf.yield %arg0 : vector<1xindex>
    }
    %3 = scf.while (%arg0 = %2) : (vector<1xindex>) -> vector<1xindex> {
      %cond = arith.constant true
      scf.condition(%cond) %arg0 : vector<1xindex>
    } do {
      ^bb0(%arg0: vector<1xindex>):
      scf.yield %arg0 : vector<1xindex>
    }
    smt.solver(%3) : (vector<1xindex>) -> () {
      ^bb0(%arg0: vector<1xindex>):
      smt.yield
    }
    return
  }
}
```
```
mlir-opt --remove-dead-values temp.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --remove-dead-values temp.mlir
 #0 0x000055bea1e34e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #1 0x000055bea1e315af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #2 0x000055bea1e316fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x0000751474845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000055bea91b9b73 (anonymous namespace)::WhileConditionTruth::matchAndRewrite(mlir::scf::WhileOp, mlir::PatternRewriter&) const SCF.cpp:0:0
 #5 0x000055bea965fe03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
 #6 0x000055bea96613af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&, llvm::function_ref<bool (mlir::Pattern const&)>, llvm::function_ref<void (mlir::Pattern const&)>, llvm::function_ref<llvm::LogicalResult (mlir::Pattern const&)>) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
 #7 0x000055bea96263c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
 #8 0x000055bea962911e mlir::applyOpPatternsGreedily(llvm::ArrayRef<mlir::Operation*>, mlir::FrozenRewritePatternSet const&, mlir::GreedyRewriteConfig, bool*, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d2411e)
 #9 0x000055bea95ad690 (anonymous namespace)::RemoveDeadValues::runOnOperation() RemoveDeadValues.cpp:0:0
#10 0x000055bea96d4756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#11 0x000055bea96d4a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (compilerStudyer)

<details>
**Bug Op**: scf.while 

**Bug Pass**: --remove-dead-values

**Commenting out does not trigger the crash**: smt.solver

**Detailed Description**: The pass crashes when rewriting nested scf.while loops that return vector types. The issue is located in the WhileConditionTruth pattern. The crash is only reproducible when the loop results are "live" (e.g., used by an smt.solver op); otherwise, DCE removes the loops and avoids the buggy code path.

```mlir
module {
  func.func @<!-- -->main() {
    %0 = gpu.dynamic_shared_memory : memref&lt;?xi8, #gpu.address_space&lt;workgroup&gt;&gt;
    %1 = vector.step : vector&lt;1xindex&gt;
    %2 = scf.while (%arg0 = %1) : (vector&lt;1xindex&gt;) -&gt; vector&lt;1xindex&gt; {
      %cond = arith.constant true
      scf.condition(%cond) %arg0 : vector&lt;1xindex&gt;
    } do {
      ^bb0(%arg0: vector&lt;1xindex&gt;):
      scf.yield %arg0 : vector&lt;1xindex&gt;
    }
    %3 = scf.while (%arg0 = %2) : (vector&lt;1xindex&gt;) -&gt; vector&lt;1xindex&gt; {
      %cond = arith.constant true
      scf.condition(%cond) %arg0 : vector&lt;1xindex&gt;
    } do {
      ^bb0(%arg0: vector&lt;1xindex&gt;):
      scf.yield %arg0 : vector&lt;1xindex&gt;
    }
    smt.solver(%3) : (vector&lt;1xindex&gt;) -&gt; () {
      ^bb0(%arg0: vector&lt;1xindex&gt;):
      smt.yield
    }
    return
  }
}
```
```
mlir-opt --remove-dead-values temp.mlir
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llvmorg-22.1.0-install/bin/mlir-opt --remove-dead-values temp.mlir
 #<!-- -->0 0x000055bea1e34e32 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvmorg-22.1.0-install/bin/mlir-opt+0x252fe32)
 #<!-- -->1 0x000055bea1e315af llvm::sys::RunSignalHandlers() (llvmorg-22.1.0-install/bin/mlir-opt+0x252c5af)
 #<!-- -->2 0x000055bea1e316fc SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x0000751474845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000055bea91b9b73 (anonymous namespace)::WhileConditionTruth::matchAndRewrite(mlir::scf::WhileOp, mlir::PatternRewriter&amp;) const SCF.cpp:0:0
 #<!-- -->5 0x000055bea965fe03 mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;)::'lambda'()::operator()() const PatternApplicator.cpp:0:0
 #<!-- -->6 0x000055bea96613af mlir::PatternApplicator::matchAndRewrite(mlir::Operation*, mlir::PatternRewriter&amp;, llvm::function_ref&lt;bool (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;void (mlir::Pattern const&amp;)&gt;, llvm::function_ref&lt;llvm::LogicalResult (mlir::Pattern const&amp;)&gt;) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d5c3af)
 #<!-- -->7 0x000055bea96263c0 (anonymous namespace)::GreedyPatternRewriteDriver::processWorklist() GreedyPatternRewriteDriver.cpp:0:0
 #<!-- -->8 0x000055bea962911e mlir::applyOpPatternsGreedily(llvm::ArrayRef&lt;mlir::Operation*&gt;, mlir::FrozenRewritePatternSet const&amp;, mlir::GreedyRewriteConfig, bool*, bool*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9d2411e)
 #<!-- -->9 0x000055bea95ad690 (anonymous namespace)::RemoveDeadValues::runOnOperation() RemoveDeadValues.cpp:0:0
#<!-- -->10 0x000055bea96d4756 mlir::detail::OpToOpPassAdaptor::run(mlir::Pass*, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcf756)
#<!-- -->11 0x000055bea96d4a8e mlir::detail::OpToOpPassAdaptor::runPipeline(mlir::OpPassManager&amp;, mlir::Operation*, mlir::AnalysisManager, bool, unsigned int, mlir::PassInstrumentor*, mlir::PassInstrumentation::PipelineParentInfo const*) (llvmorg-22.1.0-install/bin/mlir-opt+0x9dcfa8e)
```
</details>


---

### Comment 2 - meshtag

The crash is coming from canonicalization due to a dangling reference being created in `scf.condition` after the removal of dead values in the pass. Attaching the intermediate IR before canonicalization here for reference: 
```
"builtin.module"() ({
  "func.func"() <{function_type = () -> (), sym_name = "main"}> ({
    %0 = "ub.poison"() <{value = #ub.poison}> : () -> vector<1xindex>
    %1 = "scf.while"(%0) ({
    ^bb0(%arg2: vector<1xindex>):
      %3 = "ub.poison"() <{value = #ub.poison}> : () -> vector<1xindex>
      "scf.condition"(<<NULL VALUE>>, %3) : (<<NULL TYPE>>, vector<1xindex>) -> ()
    }, {
    ^bb0(%arg1: vector<1xindex>):
      %2 = "ub.poison"() <{value = #ub.poison}> : () -> vector<1xindex>
      "scf.yield"(%2) : (vector<1xindex>) -> ()
    }) : (vector<1xindex>) -> vector<1xindex>
    "smt.solver"(%1) ({
    ^bb0(%arg0: vector<1xindex>):
      "smt.yield"() : () -> ()
    }) : (vector<1xindex>) -> ()
    "func.return"() : () -> ()
  }) : () -> ()
}) : () -> ()
```
The helper method to get condition in the pattern tries to cast the NULL value which results in the crash.

One solution is to not go through the trouble of canonicalization if we have dangling reference in the IR (since it is illegal anyway).

---

