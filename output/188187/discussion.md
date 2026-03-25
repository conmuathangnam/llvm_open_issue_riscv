# [MLIR][tosa] Almost all tosa ops would crash when folding different input and result type (Unranked Tensor Type) 

**Author:** hockyy
**URL:** https://github.com/llvm/llvm-project/issues/188187
**Status:** Open
**Labels:** crash, mlir:tosa

## Body

I was curious that #187974 happened, because I saw so many tosaOp allowing Unranked tensor type and has no verifier. I found out that some of the fold also have the same crashing problem over these checks. These would crash some TOSA folds ....

```bash
mlir-opt tc.mlir -canonicalize
```

```mlir
// CHECK-LABEL: func @try_fold_equal_with_unranked_tensor_constants
func.func @try_fold_equal_with_unranked_tensor_constants() {
  // CHECK: tosa.equal
  // CHECK-NEXT: return
  %lhs = arith.constant dense<1> : tensor<1xi32>
  %rhs = arith.constant dense<2> : tensor<1xi32>
  %0 = tosa.equal %lhs, %rhs : (tensor<1xi32>, tensor<1xi32>) -> tensor<*xi1>
  return
}

// -----

// CHECK-LABEL: func @try_fold_greater_with_unranked_tensor_constants
func.func @try_fold_greater_with_unranked_tensor_constants() {
  // CHECK: tosa.greater
  // CHECK-NEXT: return
  %lhs = arith.constant dense<1> : tensor<1xi32>
  %rhs = arith.constant dense<2> : tensor<1xi32>
  %0 = tosa.greater %lhs, %rhs : (tensor<1xi32>, tensor<1xi32>) -> tensor<*xi1>
  return
}

// -----

// CHECK-LABEL: func @try_fold_greater_equal_with_unranked_tensor_constants
func.func @try_fold_greater_equal_with_unranked_tensor_constants() {
  // CHECK: tosa.greater_equal
  // CHECK-NEXT: return
  %lhs = arith.constant dense<1> : tensor<1xi32>
  %rhs = arith.constant dense<2> : tensor<1xi32>
  %0 = tosa.greater_equal %lhs, %rhs : (tensor<1xi32>, tensor<1xi32>) -> tensor<*xi1>
  return
}

// -----
func.func @dont_fold_resize_unranked(%arg0 : tensor<*xf32>) -> tensor<*xf32> {
  // CHECK: tosa.resize
  %scale = tosa.const_shape { values = dense<[2, 2, 1, 1]> : tensor<4xindex> } : () -> !tosa.shape<4>
  %offset = tosa.const_shape { values = dense<0> : tensor<2xindex> } : () -> !tosa.shape<2>
  %border = tosa.const_shape { values = dense<0> : tensor<2xindex> } : () -> !tosa.shape<2>
  %resize = tosa.resize %arg0, %scale, %offset, %border {mode = NEAREST_NEIGHBOR} : (tensor<*xf32>, !tosa.shape<4>, !tosa.shape<2>, !tosa.shape<2>) -> tensor<*xf32>
  return %resize : tensor<*xf32>
}
```

Assisted-by: Cursor - GPT 5.3 - Codex

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Hocky Yudhiono (hockyy)

<details>
I was curious that #<!-- -->187974 happened, because I saw so many tosaOp allowing Unranked tensor type and has no verifier. I found out that some of the fold also have the same crashing problem over these checks. These would crash

```bash
mlir-opt tc.mlir -canonicalize
```

```
// CHECK-LABEL: func @<!-- -->try_fold_equal_with_unranked_tensor_constants
func.func @<!-- -->try_fold_equal_with_unranked_tensor_constants() {
  // CHECK: tosa.equal
  // CHECK-NEXT: return
  %lhs = arith.constant dense&lt;1&gt; : tensor&lt;1xi32&gt;
  %rhs = arith.constant dense&lt;2&gt; : tensor&lt;1xi32&gt;
  %0 = tosa.equal %lhs, %rhs : (tensor&lt;1xi32&gt;, tensor&lt;1xi32&gt;) -&gt; tensor&lt;*xi1&gt;
  return
}

// -----

// CHECK-LABEL: func @<!-- -->try_fold_greater_with_unranked_tensor_constants
func.func @<!-- -->try_fold_greater_with_unranked_tensor_constants() {
  // CHECK: tosa.greater
  // CHECK-NEXT: return
  %lhs = arith.constant dense&lt;1&gt; : tensor&lt;1xi32&gt;
  %rhs = arith.constant dense&lt;2&gt; : tensor&lt;1xi32&gt;
  %0 = tosa.greater %lhs, %rhs : (tensor&lt;1xi32&gt;, tensor&lt;1xi32&gt;) -&gt; tensor&lt;*xi1&gt;
  return
}

// -----

// CHECK-LABEL: func @<!-- -->try_fold_greater_equal_with_unranked_tensor_constants
func.func @<!-- -->try_fold_greater_equal_with_unranked_tensor_constants() {
  // CHECK: tosa.greater_equal
  // CHECK-NEXT: return
  %lhs = arith.constant dense&lt;1&gt; : tensor&lt;1xi32&gt;
  %rhs = arith.constant dense&lt;2&gt; : tensor&lt;1xi32&gt;
  %0 = tosa.greater_equal %lhs, %rhs : (tensor&lt;1xi32&gt;, tensor&lt;1xi32&gt;) -&gt; tensor&lt;*xi1&gt;
  return
}

// -----
func.func @<!-- -->dont_fold_resize_unranked(%arg0 : tensor&lt;*xf32&gt;) -&gt; tensor&lt;*xf32&gt; {
  // CHECK: tosa.resize
  %scale = tosa.const_shape { values = dense&lt;[2, 2, 1, 1]&gt; : tensor&lt;4xindex&gt; } : () -&gt; !tosa.shape&lt;4&gt;
  %offset = tosa.const_shape { values = dense&lt;0&gt; : tensor&lt;2xindex&gt; } : () -&gt; !tosa.shape&lt;2&gt;
  %border = tosa.const_shape { values = dense&lt;0&gt; : tensor&lt;2xindex&gt; } : () -&gt; !tosa.shape&lt;2&gt;
  %resize = tosa.resize %arg0, %scale, %offset, %border {mode = NEAREST_NEIGHBOR} : (tensor&lt;*xf32&gt;, !tosa.shape&lt;4&gt;, !tosa.shape&lt;2&gt;, !tosa.shape&lt;2&gt;) -&gt; tensor&lt;*xf32&gt;
  return %resize : tensor&lt;*xf32&gt;
}
```

Assisted-by: Cursor - GPT 5.3 - Codex
</details>


---

