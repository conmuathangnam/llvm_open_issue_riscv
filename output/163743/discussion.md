# [MLIR][SCF] Wrong unrolling of for loops with UnsignedCmp

**Author:** 7FM
**URL:** https://github.com/llvm/llvm-project/issues/163743
**Status:** Open
**Labels:** miscompilation, mlir:scf

## Body

I noticed that the unrolling tools of SCF / `constantTripCount()` produce invalid results in case of unsigned `scf.for` operations with minimal bitwidth to represent the bounds. So far, I have identified three different problems.
All examples are run with `mlir-opt -test-loop-unrolling='unroll-factor=2'`

1. `constantTripCount()` returns 0 instead of 1
<details>

Initial loop:
```mlir
func.func @unroll_twos_complement_min_bit_width_bound() -> (i32, i32) {
  %0 = arith.constant 7 : i32
  %lb = arith.constant 0 : i2
  %ub = arith.constant 2 : i2
  %step = arith.constant 1 : i2
  %result:2 = scf.for unsigned %i0 = %lb to %ub step %step iter_args(%arg0 = %0, %arg1 = %0) -> (i32, i32) : i2{
    %add = arith.addi %arg0, %arg1 : i32
    %mul = arith.muli %arg0, %arg1 : i32
    scf.yield %add, %mul : i32, i32
  }
  return %result#0, %result#1 : i32, i32
}
```
Intermediate result while unrolling: 
```mlir
%0:2 = scf.for unsigned %arg0 = %c0_i2 to %c-2_i2 step %c-2_i2_0 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -> (i32, i32)  : i2 {...}
```

-> `constantTripCount()` returns 0: https://github.com/llvm/llvm-project/blob/c3366256fa42ae8a0d326d4b5344308a2d98281b/mlir/lib/Dialect/Utils/StaticValueUtils.cpp#L318-L319

Expected result:
```mlir
  func.func @unroll_twos_complement_min_bit_width_bound() -> (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %c0_i2 = arith.constant 0 : i2
    %c-2_i2 = arith.constant -2 : i2
    %c1_i2 = arith.constant 1 : i2
    %c-2_i2_0 = arith.constant -2 : i2
    %0 = arith.addi %c7_i32, %c7_i32 : i32
    %1 = arith.muli %c7_i32, %c7_i32 : i32
    %2 = arith.addi %0, %1 : i32
    %3 = arith.muli %0, %1 : i32
    return %2, %3 : i32, i32
  }
```
Actual result:
```mlir
  func.func @unroll_twos_complement_min_bit_width_bound() -> (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %c0_i2 = arith.constant 0 : i2
    %c-2_i2 = arith.constant -2 : i2
    %c1_i2 = arith.constant 1 : i2
    %c-2_i2_0 = arith.constant -2 : i2
    return %c7_i32, %c7_i32 : i32, i32
  }
```
</details>

2. Trip count interpreted as a negative number. Potential fix: `tripCount->ZExtValue()` instead of `tripCount->SExtValue()`
<details>

Initial loop:
```mlir
func.func @unroll_pow2_trip_count() -> (i32, i32) {
  %0 = arith.constant 7 : i32
  %lb = arith.constant 0 : i3
  %ub = arith.constant 4 : i3
  %step = arith.constant 1 : i3
  %result:2 = scf.for unsigned %i0 = %lb to %ub step %step iter_args(%arg0 = %0, %arg1 = %0) -> (i32, i32) : i3{
    %add = arith.addi %arg0, %arg1 : i32
    %mul = arith.muli %arg0, %arg1 : i32
    scf.yield %add, %mul : i32, i32
  }
  return %result#0, %result#1 : i32, i32
}
```
Intermediate result while unrolling: 
```mlir
%0:2 = scf.for unsigned %arg0 = %c0_i3 to %c-4_i3 step %c2_i3 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -> (i32, i32)  : i3 {...}
```

-> `tripCount->getSExtValue() = -2`: https://github.com/llvm/llvm-project/blob/33503d016e6219e9afc9092e85ff7ffb3b1414aa/mlir/lib/Dialect/SCF/IR/SCF.cpp#L438-L439

Expected result:
```mlir
  func.func @unroll_pow2_trip_count() -> (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %c0_i3 = arith.constant 0 : i3
    %c-4_i3 = arith.constant -4 : i3
    %c1_i3 = arith.constant 1 : i3
    %c2_i3 = arith.constant 2 : i3
    %0:2 = scf.for unsigned %arg0 = %c0_i3 to %c-4_i3 step %c2_i3 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -> (i32, i32)  : i3 {
      %1 = arith.addi %arg1, %arg2 : i32
      %2 = arith.muli %arg1, %arg2 : i32
      %3 = arith.addi %1, %2 : i32
      %4 = arith.muli %1, %2 : i32
      scf.yield %3, %4 : i32, i32
    }
    return %0#0, %0#1 : i32, i32
  }
```
Actual result:
```mlir
  func.func @unroll_pow2_trip_count() -> (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %c0_i3 = arith.constant 0 : i3
    %c-4_i3 = arith.constant -4 : i3
    %c1_i3 = arith.constant 1 : i3
    %c2_i3 = arith.constant 2 : i3
    %0 = arith.addi %c7_i32, %c7_i32 : i32
    %1 = arith.muli %c7_i32, %c7_i32 : i32
    %2 = arith.addi %0, %1 : i32
    %3 = arith.muli %0, %1 : i32
    return %2, %3 : i32, i32
  }
```
</details>

3. Invalid transformation of loop with a tripcount of 1
<details>
For this example, I am uncertain if I am just using the API incorrectly, since the loop has a trip count of 1, but I request to unroll it using a factor of 2. In my opinion, I would expect the loop to be either untouched or completely unrolled.

Initial loop:
```mlir
func.func @unroll_trip_count_1_min_bit_width() -> (i32, i32) {
  %0 = arith.constant 7 : i32
  %lb = arith.constant 0 : i1
  %ub = arith.constant 1 : i1
  %step = arith.constant 1 : i1
  %result:2 = scf.for unsigned %i0 = %lb to %ub step %step iter_args(%arg0 = %0, %arg1 = %0) -> (i32, i32) : i1{
    %add = arith.addi %arg0, %arg1 : i32
    %mul = arith.muli %arg0, %arg1 : i32
    scf.yield %add, %mul : i32, i32
  }
  return %result#0, %result#1 : i32, i32
}
```
Intermediate result while unrolling: 
```mlir
%0:2 = scf.for unsigned %arg0 = %false to %true step %false_1 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -> (i32, i32)  : i1 {...}
```
I this case, the intermediate result seems to be wrong, I would still expect a step size of 1.

-> `constantTripCount()` returns 0: https://github.com/llvm/llvm-project/blob/c3366256fa42ae8a0d326d4b5344308a2d98281b/mlir/lib/Dialect/Utils/StaticValueUtils.cpp#L316-L317

Expected result:
```mlir
  func.func @unroll_trip_count_1_min_bit_width() -> (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %false = arith.constant false
    %true = arith.constant true
    %true_0 = arith.constant true
    %add = arith.addi %c7_i32, %c7_i32 : i32
    %mul = arith.muli %c7_i32, %c7_i32 : i32
    return %add, %mul : i32, i32
  }
```
Actual result:
```mlir
  func.func @unroll_trip_count_1_min_bit_width() -> (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %false = arith.constant false
    %true = arith.constant true
    %true_0 = arith.constant true
    %false_1 = arith.constant false
    return %c7_i32, %c7_i32 : i32, i32
  }
```
</details>

There are probably more bugs of a similar nature.

For the ones I have found, please find the LIT tests with my expected outputs attached.

[loop-unroll.mlir.txt](https://github.com/user-attachments/files/22947344/loop-unroll.mlir.txt)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: None (7FM)

<details>
I noticed that the unrolling tools of SCF / `constantTripCount()` produce invalid results in case of unsigned `scf.for` operations with minimal bitwidth to represent the bounds. So far, I have identified three different problems.
All examples are run with `mlir-opt -test-loop-unrolling='unroll-factor=2'`

1. `constantTripCount()` returns 0 instead of 1
&lt;details&gt;

Initial loop:
```mlir
func.func @<!-- -->unroll_twos_complement_min_bit_width_bound() -&gt; (i32, i32) {
  %0 = arith.constant 7 : i32
  %lb = arith.constant 0 : i2
  %ub = arith.constant 2 : i2
  %step = arith.constant 1 : i2
  %result:2 = scf.for unsigned %i0 = %lb to %ub step %step iter_args(%arg0 = %0, %arg1 = %0) -&gt; (i32, i32) : i2{
    %add = arith.addi %arg0, %arg1 : i32
    %mul = arith.muli %arg0, %arg1 : i32
    scf.yield %add, %mul : i32, i32
  }
  return %result#<!-- -->0, %result#<!-- -->1 : i32, i32
}
```
Intermediate result while unrolling: 
```mlir
%0:2 = scf.for unsigned %arg0 = %c0_i2 to %c-2_i2 step %c-2_i2_0 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -&gt; (i32, i32)  : i2 {...}
```

-&gt; `constantTripCount()` returns 0: https://github.com/llvm/llvm-project/blob/c3366256fa42ae8a0d326d4b5344308a2d98281b/mlir/lib/Dialect/Utils/StaticValueUtils.cpp#L318-L319

Expected result:
```mlir
  func.func @<!-- -->unroll_twos_complement_min_bit_width_bound() -&gt; (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %c0_i2 = arith.constant 0 : i2
    %c-2_i2 = arith.constant -2 : i2
    %c1_i2 = arith.constant 1 : i2
    %c-2_i2_0 = arith.constant -2 : i2
    %0 = arith.addi %c7_i32, %c7_i32 : i32
    %1 = arith.muli %c7_i32, %c7_i32 : i32
    %2 = arith.addi %0, %1 : i32
    %3 = arith.muli %0, %1 : i32
    return %2, %3 : i32, i32
  }
```
Actual result:
```mlir
  func.func @<!-- -->unroll_twos_complement_min_bit_width_bound() -&gt; (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %c0_i2 = arith.constant 0 : i2
    %c-2_i2 = arith.constant -2 : i2
    %c1_i2 = arith.constant 1 : i2
    %c-2_i2_0 = arith.constant -2 : i2
    return %c7_i32, %c7_i32 : i32, i32
  }
```
&lt;/details&gt;

2. Trip count interpreted as a negative number. Potential fix: `tripCount-&gt;ZExtValue()` instead of `tripCount-&gt;SExtValue()`
&lt;details&gt;

Initial loop:
```mlir
func.func @<!-- -->unroll_pow2_trip_count() -&gt; (i32, i32) {
  %0 = arith.constant 7 : i32
  %lb = arith.constant 0 : i3
  %ub = arith.constant 4 : i3
  %step = arith.constant 1 : i3
  %result:2 = scf.for unsigned %i0 = %lb to %ub step %step iter_args(%arg0 = %0, %arg1 = %0) -&gt; (i32, i32) : i3{
    %add = arith.addi %arg0, %arg1 : i32
    %mul = arith.muli %arg0, %arg1 : i32
    scf.yield %add, %mul : i32, i32
  }
  return %result#<!-- -->0, %result#<!-- -->1 : i32, i32
}
```
Intermediate result while unrolling: 
```mlir
%0:2 = scf.for unsigned %arg0 = %c0_i3 to %c-4_i3 step %c2_i3 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -&gt; (i32, i32)  : i3 {...}
```
I this case, the intermediate result seems to be wrong, I would still expect a step size of 1.

-&gt; `tripCount-&gt;getSExtValue() = -2`: https://github.com/llvm/llvm-project/blob/33503d016e6219e9afc9092e85ff7ffb3b1414aa/mlir/lib/Dialect/SCF/IR/SCF.cpp#L438-L439

Expected result:
```mlir
  func.func @<!-- -->unroll_pow2_trip_count() -&gt; (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %c0_i3 = arith.constant 0 : i3
    %c-4_i3 = arith.constant -4 : i3
    %c1_i3 = arith.constant 1 : i3
    %c2_i3 = arith.constant 2 : i3
    %0:2 = scf.for unsigned %arg0 = %c0_i3 to %c-4_i3 step %c2_i3 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -&gt; (i32, i32)  : i3 {
      %1 = arith.addi %arg1, %arg2 : i32
      %2 = arith.muli %arg1, %arg2 : i32
      %3 = arith.addi %1, %2 : i32
      %4 = arith.muli %1, %2 : i32
      scf.yield %3, %4 : i32, i32
    }
    return %0#<!-- -->0, %0#<!-- -->1 : i32, i32
  }
```
Actual result:
```mlir
  func.func @<!-- -->unroll_pow2_trip_count() -&gt; (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %c0_i3 = arith.constant 0 : i3
    %c-4_i3 = arith.constant -4 : i3
    %c1_i3 = arith.constant 1 : i3
    %c2_i3 = arith.constant 2 : i3
    %0 = arith.addi %c7_i32, %c7_i32 : i32
    %1 = arith.muli %c7_i32, %c7_i32 : i32
    %2 = arith.addi %0, %1 : i32
    %3 = arith.muli %0, %1 : i32
    return %2, %3 : i32, i32
  }
```
&lt;/details&gt;

3. Invalid transformation of loop with a tripcount of 1
&lt;details&gt;
For this example, I am uncertain if I am just using the API incorrectly, since the loop has a trip count of 1, but I request to unroll it using a factor of 2. In my opinion, I would expect the loop to be either untouched or completely unrolled.

Initial loop:
```mlir
func.func @<!-- -->unroll_trip_count_1_min_bit_width() -&gt; (i32, i32) {
  %0 = arith.constant 7 : i32
  %lb = arith.constant 0 : i1
  %ub = arith.constant 1 : i1
  %step = arith.constant 1 : i1
  %result:2 = scf.for unsigned %i0 = %lb to %ub step %step iter_args(%arg0 = %0, %arg1 = %0) -&gt; (i32, i32) : i1{
    %add = arith.addi %arg0, %arg1 : i32
    %mul = arith.muli %arg0, %arg1 : i32
    scf.yield %add, %mul : i32, i32
  }
  return %result#<!-- -->0, %result#<!-- -->1 : i32, i32
}
```
Intermediate result while unrolling: 
```mlir
%0:2 = scf.for unsigned %arg0 = %false to %true step %false_1 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -&gt; (i32, i32)  : i1 {...}
```
-&gt; `constantTripCount()` returns 0: https://github.com/llvm/llvm-project/blob/c3366256fa42ae8a0d326d4b5344308a2d98281b/mlir/lib/Dialect/Utils/StaticValueUtils.cpp#L316-L317

Expected result:
```mlir
  func.func @<!-- -->unroll_trip_count_1_min_bit_width() -&gt; (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %false = arith.constant false
    %true = arith.constant true
    %true_0 = arith.constant true
    %add = arith.addi %c7_i32, %c7_i32 : i32
    %mul = arith.muli %c7_i32, %c7_i32 : i32
    return %add, %mul : i32, i32
  }
```
Actual result:
```mlir
  func.func @<!-- -->unroll_trip_count_1_min_bit_width() -&gt; (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %false = arith.constant false
    %true = arith.constant true
    %true_0 = arith.constant true
    %false_1 = arith.constant false
    return %c7_i32, %c7_i32 : i32, i32
  }
```
&lt;/details&gt;

There are probably more bugs of a similar nature.

For the ones I have found, please find the LIT tests with my expected outputs attached.

[loop-unroll.mlir.txt](https://github.com/user-attachments/files/22947344/loop-unroll.mlir.txt)
</details>


---

### Comment 2 - veera-sivarajan

I think this has been fixed by https://github.com/llvm/llvm-project/pull/175301

---

### Comment 3 - 7FM

> I think this has been fixed by [#175301](https://github.com/llvm/llvm-project/pull/175301)

Unfortunately, I can still reproduce
1.
```
[StaticValueUtils.cpp:374 1] constantTripCount: isUnsigned, ub-lb: 2(2b)
[StaticValueUtils.cpp:390 1] constantTripCount found: -2
[SCF.cpp:529 1] promoteIfSingleIteration tripCount is 0 for loop %0:2 = scf.for unsigned %arg0 = %c0_i2 to %c-2_i2 step %c-2_i2_0 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -> (i32, i32)  : i2 {...}
[pass-manager Pass.cpp:1108 2] PassManager run completed with result: success
module {
  func.func @unroll_twos_complement_min_bit_width_bound() -> (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %c0_i2 = arith.constant 0 : i2
    %c-2_i2 = arith.constant -2 : i2
    %c1_i2 = arith.constant 1 : i2
    %c-2_i2_0 = arith.constant -2 : i2
    return %c7_i32, %c7_i32 : i32, i32
  }
}
```
and
3. is still broken:
```
[SCF.cpp:529 1] promoteIfSingleIteration tripCount is 0 for loop %0:2 = scf.for unsigned %arg0 = %false to %true step %false_1 iter_args(%arg1 = %c7_i32, %arg2 = %c7_i32) -> (i32, i32)  : i1 {...}
[pass-manager Pass.cpp:1108 2] PassManager run completed with result: success
module {
  func.func @unroll_trip_count_1_min_bit_width() -> (i32, i32) {
    %c7_i32 = arith.constant 7 : i32
    %false = arith.constant false
    %true = arith.constant true
    %true_0 = arith.constant true
    %false_1 = arith.constant false
    return %c7_i32, %c7_i32 : i32, i32
  }
}
```

However,  2. is no longer reproducible and was fixed

---

### Comment 4 - veera-sivarajan

1 seems to be fixed by https://github.com/llvm/llvm-project/pull/178782: https://godbolt.org/z/bx5TfWh98

---

