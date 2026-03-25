# [MLIR] `-linalg-specialize-generic-ops` causes wrong-code for `linalg.matmul`

**Author:** BealZephyr
**URL:** https://github.com/llvm/llvm-project/issues/174517
**Status:** Closed
**Labels:** miscompilation, mlir:linalg
**Closed Date:** 2026-01-23T14:48:04Z

## Body

**test commit**: [5cfd02](https://github.com/llvm/llvm-project/commit/5cfd02f44a43a2e2a085a633b022a62f64ba2b93)

## Description:
I found a miscompilation issue in MLIR where adding -linalg-specialize-generic-ops to an otherwise identical lowering pipeline changes the numerical result of a linalg.matmul.


## test case
```mlir
#map = affine_map<(d0, d1, d2) -> (d0, d2)>
#map1 = affine_map<(d0, d1, d2) -> (d2, d1)>
#map2 = affine_map<(d0, d1, d2) -> (d0, d1)>
module {
  memref.global "private" constant @__constant_16x32xi32 : memref<16x32xi32> = dense<130> {alignment = 64 : i64}
  memref.global "private" constant @__constant_8x32xi64 : memref<8x32xi64> = dense<159> {alignment = 64 : i64}
  memref.global "private" constant @__constant_16x8xi16 : memref<16x8xi16> = dense<-50> {alignment = 64 : i64}
  func.func private @printMemrefI32(memref<*xi32>)
  func.func @main() {
    %0 = memref.get_global @__constant_16x8xi16 : memref<16x8xi16>
    %1 = memref.get_global @__constant_8x32xi64 : memref<8x32xi64>
    %2 = memref.get_global @__constant_16x32xi32 : memref<16x32xi32>
    %alloc = memref.alloc() {alignment = 64 : i64} : memref<16x32xi32>
    memref.copy %2, %alloc : memref<16x32xi32> to memref<16x32xi32>
    linalg.generic {indexing_maps = [#map, #map1, #map2], iterator_types = ["parallel", "parallel", "reduction"]} ins(%0, %1 : memref<16x8xi16>, memref<8x32xi64>) outs(%alloc : memref<16x32xi32>) {
    ^bb0(%in: i16, %in_0: i64, %out: i32):
      %3 = arith.extui %in : i16 to i32
      %4 = arith.trunci %in_0 : i64 to i32
      %5 = arith.muli %3, %4 : i32
      %6 = arith.addi %out, %5 : i32
      linalg.yield %6 : i32
    }
    %cast = memref.cast %alloc : memref<16x32xi32> to memref<*xi32>
    call @printMemrefI32(%cast) : (memref<*xi32>) -> ()
    return
  }
}
```

## Command:
### 1. without -linalg-specialize-generic-ops

```
mlir-opt test.mlir -convert-linalg-to-affine-loops -lower-affine -convert-scf-to-cf -convert-to-llvm |  mlir-runner -e main -entry-point-result=void -shared-libs=/home/workdir/llvm-project-latest/build/lib/libmlir_runner_utils.so
```

### output:
```
[83298322,   83298322,   83298322,   83298322,   83298322,   83298322,   83298322,   83298322,   ...]]
```
### 2. With -linalg-specialize-generic-ops:

```
mlir-opt test.mlir -linalg-specialize-generic-ops -convert-linalg-to-affine-loops -lower-affine -convert-scf-to-cf -convert-to-llvm |  mlir-runner -e main -entry-point-result=void -shared-libs=/home/workdir/llvm-project-latest/build/lib/libmlir_runner_utils.so
```
### output:
```
[-63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   ...]]
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: Tong (BealZephyr)

<details>
**test commit**: [5cfd02](https://github.com/llvm/llvm-project/commit/5cfd02f44a43a2e2a085a633b022a62f64ba2b93)

## Description:
I found a miscompilation issue in MLIR where adding -linalg-specialize-generic-ops to an otherwise identical lowering pipeline changes the numerical result of a linalg.matmul.


## test case
```
#map = affine_map&lt;(d0, d1, d2) -&gt; (d0, d2)&gt;
#map1 = affine_map&lt;(d0, d1, d2) -&gt; (d2, d1)&gt;
#map2 = affine_map&lt;(d0, d1, d2) -&gt; (d0, d1)&gt;
module {
  memref.global "private" constant @<!-- -->__constant_16x32xi32 : memref&lt;16x32xi32&gt; = dense&lt;130&gt; {alignment = 64 : i64}
  memref.global "private" constant @<!-- -->__constant_8x32xi64 : memref&lt;8x32xi64&gt; = dense&lt;159&gt; {alignment = 64 : i64}
  memref.global "private" constant @<!-- -->__constant_16x8xi16 : memref&lt;16x8xi16&gt; = dense&lt;-50&gt; {alignment = 64 : i64}
  func.func private @<!-- -->printMemrefI32(memref&lt;*xi32&gt;)
  func.func @<!-- -->main() {
    %0 = memref.get_global @<!-- -->__constant_16x8xi16 : memref&lt;16x8xi16&gt;
    %1 = memref.get_global @<!-- -->__constant_8x32xi64 : memref&lt;8x32xi64&gt;
    %2 = memref.get_global @<!-- -->__constant_16x32xi32 : memref&lt;16x32xi32&gt;
    %alloc = memref.alloc() {alignment = 64 : i64} : memref&lt;16x32xi32&gt;
    memref.copy %2, %alloc : memref&lt;16x32xi32&gt; to memref&lt;16x32xi32&gt;
    linalg.generic {indexing_maps = [#map, #map1, #map2], iterator_types = ["parallel", "parallel", "reduction"]} ins(%0, %1 : memref&lt;16x8xi16&gt;, memref&lt;8x32xi64&gt;) outs(%alloc : memref&lt;16x32xi32&gt;) {
    ^bb0(%in: i16, %in_0: i64, %out: i32):
      %3 = arith.extui %in : i16 to i32
      %4 = arith.trunci %in_0 : i64 to i32
      %5 = arith.muli %3, %4 : i32
      %6 = arith.addi %out, %5 : i32
      linalg.yield %6 : i32
    }
    %cast = memref.cast %alloc : memref&lt;16x32xi32&gt; to memref&lt;*xi32&gt;
    call @<!-- -->printMemrefI32(%cast) : (memref&lt;*xi32&gt;) -&gt; ()
    return
  }
}
```

## Command:
### 1. without -linalg-specialize-generic-ops

```
mlir-opt test.mlir -convert-linalg-to-affine-loops -lower-affine -convert-scf-to-cf -convert-to-llvm |  mlir-runner -e main -entry-point-result=void -shared-libs=/home/workdir/llvm-project-latest/build/lib/libmlir_runner_utils.so,/home/workdir/llvm-project-latest/build/lib/libmlir_c_runner_utils.so,/home/workdir/llvm-project-latest/build/lib/libmlir_async_runtime.so
```

### output:
```
[83298322,   83298322,   83298322,   83298322,   83298322,   83298322,   83298322,   83298322,   ...]]
```
### 2. With -linalg-specialize-generic-ops:

```
mlir-opt test.mlir -linalg-specialize-generic-ops -convert-linalg-to-affine-loops -lower-affine -convert-scf-to-cf -convert-to-llvm |  mlir-runner -e main -entry-point-result=void -shared-libs=/home/workdir/llvm-project-latest/build/lib/libmlir_runner_utils.so,/home/workdir/llvm-project-latest/build/lib/libmlir_c_runner_utils.so,/home/workdir/llvm-project-latest/build/lib/libmlir_async_runtime.so
```
### output:
```
[-63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   ...]]
```

</details>


---

### Comment 2 - meshtag

I think the reason for different values here is the presence of unsigned cast inside the `linalg.generic` loop nest. 

We have the following ops inside the original nest 
```
%3 = arith.extui %in : i16 to i32
%4 = arith.trunci %in_0 : i64 to i32
...
```

Notice the `arith.extui` which is doing an unsigned extension from i16 to i32. But the value it gets via `%in` is a negative number, so overflow should happen ideally and we should get the overflowed output from the CPU runner (which is true for the first case). However, `-linalg-specialize-generic-ops` is not considering the nature of this cast and emits the following matmul op for the loop nest (notice that it is not specifying the kind of cast to be emitted)
```
linalg.matmul ins(%0, %1 : memref<16x8xi16>, memref<8x32xi64>) outs(%alloc : memref<16x32xi32>)
```

The default behaviour of `linalg.matmul` is to have signed casts for operands with different element types (see [here](https://github.com/llvm/llvm-project/blob/67404745171125f1d68c629973b0e978986869f6/mlir/include/mlir/Dialect/Linalg/IR/LinalgStructuredOps.td#L731)). This results in no overflow due to casting and we get a negative output in the second case.


I think we can fix the `-linalg-specialize-generic-ops` pass to emit matmul ops with correct casts based on the input. 

---

### Comment 3 - meshtag

@CoTinker I didn't realise you assigned this issue to yourself recently and created a PR for it. Apologies.

---

### Comment 4 - CoTinker

It's okay, I assigned it to you.

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-mlir-linalg

Author: Tong (BealZephyr)

<details>
**test commit**: [5cfd02](https://github.com/llvm/llvm-project/commit/5cfd02f44a43a2e2a085a633b022a62f64ba2b93)

## Description:
I found a miscompilation issue in MLIR where adding -linalg-specialize-generic-ops to an otherwise identical lowering pipeline changes the numerical result of a linalg.matmul.


## test case
```mlir
#map = affine_map&lt;(d0, d1, d2) -&gt; (d0, d2)&gt;
#map1 = affine_map&lt;(d0, d1, d2) -&gt; (d2, d1)&gt;
#map2 = affine_map&lt;(d0, d1, d2) -&gt; (d0, d1)&gt;
module {
  memref.global "private" constant @<!-- -->__constant_16x32xi32 : memref&lt;16x32xi32&gt; = dense&lt;130&gt; {alignment = 64 : i64}
  memref.global "private" constant @<!-- -->__constant_8x32xi64 : memref&lt;8x32xi64&gt; = dense&lt;159&gt; {alignment = 64 : i64}
  memref.global "private" constant @<!-- -->__constant_16x8xi16 : memref&lt;16x8xi16&gt; = dense&lt;-50&gt; {alignment = 64 : i64}
  func.func private @<!-- -->printMemrefI32(memref&lt;*xi32&gt;)
  func.func @<!-- -->main() {
    %0 = memref.get_global @<!-- -->__constant_16x8xi16 : memref&lt;16x8xi16&gt;
    %1 = memref.get_global @<!-- -->__constant_8x32xi64 : memref&lt;8x32xi64&gt;
    %2 = memref.get_global @<!-- -->__constant_16x32xi32 : memref&lt;16x32xi32&gt;
    %alloc = memref.alloc() {alignment = 64 : i64} : memref&lt;16x32xi32&gt;
    memref.copy %2, %alloc : memref&lt;16x32xi32&gt; to memref&lt;16x32xi32&gt;
    linalg.generic {indexing_maps = [#map, #map1, #map2], iterator_types = ["parallel", "parallel", "reduction"]} ins(%0, %1 : memref&lt;16x8xi16&gt;, memref&lt;8x32xi64&gt;) outs(%alloc : memref&lt;16x32xi32&gt;) {
    ^bb0(%in: i16, %in_0: i64, %out: i32):
      %3 = arith.extui %in : i16 to i32
      %4 = arith.trunci %in_0 : i64 to i32
      %5 = arith.muli %3, %4 : i32
      %6 = arith.addi %out, %5 : i32
      linalg.yield %6 : i32
    }
    %cast = memref.cast %alloc : memref&lt;16x32xi32&gt; to memref&lt;*xi32&gt;
    call @<!-- -->printMemrefI32(%cast) : (memref&lt;*xi32&gt;) -&gt; ()
    return
  }
}
```

## Command:
### 1. without -linalg-specialize-generic-ops

```
mlir-opt test.mlir -convert-linalg-to-affine-loops -lower-affine -convert-scf-to-cf -convert-to-llvm |  mlir-runner -e main -entry-point-result=void -shared-libs=/home/workdir/llvm-project-latest/build/lib/libmlir_runner_utils.so
```

### output:
```
[83298322,   83298322,   83298322,   83298322,   83298322,   83298322,   83298322,   83298322,   ...]]
```
### 2. With -linalg-specialize-generic-ops:

```
mlir-opt test.mlir -linalg-specialize-generic-ops -convert-linalg-to-affine-loops -lower-affine -convert-scf-to-cf -convert-to-llvm |  mlir-runner -e main -entry-point-result=void -shared-libs=/home/workdir/llvm-project-latest/build/lib/libmlir_runner_utils.so
```
### output:
```
[-63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   -63470,   ...]]
```

</details>


---

