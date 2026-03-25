# [mlir] SEGV when converting arith op to llvm

**Author:** YuanchengJiang
**URL:** https://github.com/llvm/llvm-project/issues/172258
**Status:** Closed
**Labels:** mlir, crash
**Closed Date:** 2025-12-16T02:43:01Z

## Body

```mlir
#CSR = #sparse_tensor.encoding<{map = (d0, d1) -> (d0 : dense, d1 : compressed)}>

module {
  func.func @crash_target(%arg0: tensor<10x20xf16, #CSR>) {
    // The pass attempts to convert this arith op and potentially reconciles 
    // the function signature, crashing on the unhandled sparse tensor type.
    %0 = arith.constant 0.0 : f64
    return
  }
}
```

reproduce: `mlir-opt -pass-pipeline="builtin.module(func.func(convert-arith-to-llvm))" ./test.mlir`

```
Segmentation fault (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Yuancheng Jiang (YuanchengJiang)

<details>
```mlir
#CSR = #sparse_tensor.encoding&lt;{map = (d0, d1) -&gt; (d0 : dense, d1 : compressed)}&gt;

module {
  func.func @<!-- -->crash_target(%arg0: tensor&lt;10x20xf16, #CSR&gt;) {
    // The pass attempts to convert this arith op and potentially reconciles 
    // the function signature, crashing on the unhandled sparse tensor type.
    %0 = arith.constant 0.0 : f64
    return
  }
}
```

reproduce: `mlir-opt -pass-pipeline="builtin.module(func.func(convert-arith-to-llvm))" ./test.mlir`

```
Segmentation fault (core dumped)
```
</details>


---

### Comment 2 - linuxlonelyeagle

The latest MLIR has fixed it, and I haven't encountered this bug on my machine.

---

### Comment 3 - CoTinker

Fixed in latest version.

---

