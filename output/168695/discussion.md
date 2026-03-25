# [MLIR] [Vector] Error when canonicalizing `vector.scatter` with tensor output

**Author:** sakupan102
**URL:** https://github.com/llvm/llvm-project/issues/168695
**Status:** Closed
**Labels:** mlir:vectorops, crash
**Closed Date:** 2025-12-12T12:24:38Z

## Body

I tried to compile the code below with `mlir-opt sample.mlir --pass-pipeline="builtin.module(func.func(canonicalize))"`
```mlir
func.func @scatter_tensor(%base: tensor<16xf32>, %mask: vector<16xi1>, %value: vector<16xf32>) -> tensor<16xf32> {
  %c0 = arith.constant 0 : index
  %index = vector.step : vector<16xindex>
  %0 = vector.scatter %base[%c0] [%index], %mask, %value
      : tensor<16xf32>, vector<16xindex>, vector<16xi1>, vector<16xf32> -> tensor<16xf32>
  return %0 : tensor<16xf32>
}
```

output
```
Assertion failed: (op->getNumResults() == newOp->getNumResults() && "ops have different number of results"), function replaceOp, file PatternMatch.cpp, line 144.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
```

## Version 
915e9adbe5d1c577a21ac8b495b7c54c465460fd

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Ryutaro Okada (sakupan102)

<details>
I tried to compile the code below with `mlir-opt sample.mlir --pass-pipeline="builtin.module(func.func(canonicalize))"`
```mlir
func.func @<!-- -->scatter_tensor(%base: tensor&lt;16xf32&gt;, %mask: vector&lt;16xi1&gt;, %value: vector&lt;16xf32&gt;) -&gt; tensor&lt;16xf32&gt; {
  %c0 = arith.constant 0 : index
  %index = vector.step : vector&lt;16xindex&gt;
  %0 = vector.scatter %base[%c0] [%index], %mask, %value
      : tensor&lt;16xf32&gt;, vector&lt;16xindex&gt;, vector&lt;16xi1&gt;, vector&lt;16xf32&gt; -&gt; tensor&lt;16xf32&gt;
  return %0 : tensor&lt;16xf32&gt;
}
```

output
```
Assertion failed: (op-&gt;getNumResults() == newOp-&gt;getNumResults() &amp;&amp; "ops have different number of results"), function replaceOp, file PatternMatch.cpp, line 144.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
```

## Version 
915e9adbe5d1c577a21ac8b495b7c54c465460fd
</details>


---

### Comment 2 - sakupan102

This error is due to https://github.com/llvm/llvm-project/commit/7e7ea9c5357efcdf9ba6bd7ea3669e607a9af400 (which I authored).
I'll work on this issue.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-mlir-vectorops

Author: Ryutaro Okada (sakupan102)

<details>
I tried to compile the code below with `mlir-opt sample.mlir --pass-pipeline="builtin.module(func.func(canonicalize))"`
```mlir
func.func @<!-- -->scatter_tensor(%base: tensor&lt;16xf32&gt;, %mask: vector&lt;16xi1&gt;, %value: vector&lt;16xf32&gt;) -&gt; tensor&lt;16xf32&gt; {
  %c0 = arith.constant 0 : index
  %index = vector.step : vector&lt;16xindex&gt;
  %0 = vector.scatter %base[%c0] [%index], %mask, %value
      : tensor&lt;16xf32&gt;, vector&lt;16xindex&gt;, vector&lt;16xi1&gt;, vector&lt;16xf32&gt; -&gt; tensor&lt;16xf32&gt;
  return %0 : tensor&lt;16xf32&gt;
}
```

output
```
Assertion failed: (op-&gt;getNumResults() == newOp-&gt;getNumResults() &amp;&amp; "ops have different number of results"), function replaceOp, file PatternMatch.cpp, line 144.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
```

## Version 
915e9adbe5d1c577a21ac8b495b7c54c465460fd
</details>


---

