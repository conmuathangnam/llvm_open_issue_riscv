# [MLIR] Assertion "incorrect fold result type" in tosa.transpose folder when folding identity permutation on ranked input with unranked result type

**Author:** YuanchengJiang
**URL:** https://github.com/llvm/llvm-project/issues/187974
**Status:** Open
**Labels:** mlir, crash

## Body

**Description**

`mlir-opt --sccp` crashes with an assertion failure in `checkFoldResultTypes` when processing a `tosa.transpose` op whose input is a ranked tensor, whose permutation is the identity, and whose declared result type is unranked (`tensor<*xi32>`). The `tosa.transpose` folder recognizes the identity permutation and folds the op by returning the input value directly. However, the input has type `tensor<3x2xi32>` while the declared result type is `tensor<*xi32>`. The fold result type does not match the op's declared result type, triggering the assertion.

**Reproducer**

```mlir
module {
  func.func @f(%arg0: tensor<3x2xi32>) -> tensor<*xi32> {
    %0 = tosa.transpose %arg0 {perms = array<i32: 0, 1>}: (tensor<3x2xi32>) -> tensor<*xi32>
    return %0 : tensor<*xi32>
  }
}
```

**Command**

```
mlir-opt --sccp reproduce.mlir
```

**Expected behavior**

`--sccp` either processes the file successfully or emits a diagnostic. The folder should not return a value whose type differs from the op's declared result type, or should bail out of folding when the types do not match.

**Actual behavior**

```
reproduce.mlir:3:10: error: 'tosa.transpose' op folder produced a value of incorrect type:
  'tensor<3x2xi32>', expected: 'tensor<*xi32>'
mlir-opt: mlir/lib/IR/Operation.cpp:621:
  void checkFoldResultTypes(Operation*, SmallVectorImpl<OpFoldResult>&):
  Assertion `false && "incorrect fold result type"' failed.
```

**Call chain**

```
SCCP::runOnOperation
  → DataFlowSolver::initializeAndRun
    → SparseConstantPropagation::visitOperation
      → Operation::fold
        → tosa.transpose folder  ← returns input directly (type mismatch)
          → checkFoldResultTypes ← assertion: fold result type != declared result type
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Yuancheng Jiang (YuanchengJiang)

<details>
**Description**

`mlir-opt --sccp` crashes with an assertion failure in `checkFoldResultTypes` when processing a `tosa.transpose` op whose input is a ranked tensor, whose permutation is the identity, and whose declared result type is unranked (`tensor&lt;*xi32&gt;`). The `tosa.transpose` folder recognizes the identity permutation and folds the op by returning the input value directly. However, the input has type `tensor&lt;3x2xi32&gt;` while the declared result type is `tensor&lt;*xi32&gt;`. The fold result type does not match the op's declared result type, triggering the assertion.

**Reproducer**

```mlir
module {
  func.func @<!-- -->f(%arg0: tensor&lt;3x2xi32&gt;) -&gt; tensor&lt;*xi32&gt; {
    %0 = tosa.transpose %arg0 {perms = array&lt;i32: 0, 1&gt;}: (tensor&lt;3x2xi32&gt;) -&gt; tensor&lt;*xi32&gt;
    return %0 : tensor&lt;*xi32&gt;
  }
}
```

**Command**

```
mlir-opt --sccp reproduce.mlir
```

**Expected behavior**

`--sccp` either processes the file successfully or emits a diagnostic. The folder should not return a value whose type differs from the op's declared result type, or should bail out of folding when the types do not match.

**Actual behavior**

```
reproduce.mlir:3:10: error: 'tosa.transpose' op folder produced a value of incorrect type:
  'tensor&lt;3x2xi32&gt;', expected: 'tensor&lt;*xi32&gt;'
mlir-opt: mlir/lib/IR/Operation.cpp:621:
  void checkFoldResultTypes(Operation*, SmallVectorImpl&lt;OpFoldResult&gt;&amp;):
  Assertion `false &amp;&amp; "incorrect fold result type"' failed.
```

**Call chain**

```
SCCP::runOnOperation
  → DataFlowSolver::initializeAndRun
    → SparseConstantPropagation::visitOperation
      → Operation::fold
        → tosa.transpose folder  ← returns input directly (type mismatch)
          → checkFoldResultTypes ← assertion: fold result type != declared result type
```
</details>


---

### Comment 2 - hockyy

will take a look^

---

### Comment 3 - hockyy

umm.. don't need to be sccp tho. Canonicalize will crash it.

---

### Comment 4 - hockyy

digged more into this issue #188187, found out almost all tosa op would crash ....

---

