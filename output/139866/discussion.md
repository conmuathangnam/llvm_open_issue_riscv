# AArch64 ISel miscompilation from weird Rust MIR

**Author:** saethlin
**URL:** https://github.com/llvm/llvm-project/issues/139866
**Status:** Closed
**Labels:** backend:AArch64, miscompilation, rust, llvm:SelectionDAG
**Closed Date:** 2025-05-20T15:28:21Z

## Body

The reproducer for this is a bit large, I've (mostly) reduced an output from https://github.com/cbeuw/rustlantis, so what I have is Rust MIR, but I'm linking a godbolt that has the LLVM IR: https://godbolt.org/z/sP7qG363b (Reduced a bit more than the previous but now requires the rustc flag `-Zmir-opt-level=0` https://godbolt.org/z/1xWhzfMMq)

The structure of this program is that it hashes some locals, and prints a hash at the end. At some point in the optimization pipeline, the hash changes.

We (me and @nikic, who walked me through some opt-bisect use) are pretty confident this is an Aarch64 ISel issue because this produces the good hash:
```
rustc +nightly main.rs -Copt-level=2 -Cllvm-args=-opt-bisect-limit=3642 && ./main
```
And this produces the bad hash:
```
rustc +nightly main.rs -Copt-level=2 -Cllvm-args=-opt-bisect-limit=3647 && ./main
```
And the passes in that bisect range are:
```
BISECT: running pass (3643) AArch64 Instruction Selection on function (_ZN4main4main17he1f737bca831c768E)
BISECT: running pass (3644) SME Peephole Optimization pass on function (_ZN4main4main17he1f737bca831c768E)
BISECT: running pass (3645) Early Tail Duplication on function (_ZN4main4main17he1f737bca831c768E)
BISECT: running pass (3646) Optimize machine instruction PHIs on function (_ZN4main4main17he1f737bca831c768E)
BISECT: running pass (3647) Merge disjoint stack slots on function (_ZN4main4main17he1f737bca831c768E)
```

We've already checked that
```
rustc +nightly main.rs -Copt-level=2 -Cllvm-args=-no-stack-coloring=1 && ./main
```
produces the bad hash, so it doesn't seem like this is related to stack coloring.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Ben Kimock (saethlin)

<details>
The reproducer for this is a bit large, I've (mostly) reduced an output from https://github.com/cbeuw/rustlantis, so what I have is Rust MIR, but I'm linking a godbolt that has the LLVM IR: https://godbolt.org/z/sP7qG363b

The structure of this program is that it hashes some locals, and prints a hash at the end. At some point in the optimization pipeline, the hash changes.

We (me and @<!-- -->nikic, who walked me through some opt-bisect use) are pretty confident this is an Aarch64 ISel issue because this produces the good hash:
```
rustc +nightly main.rs -Copt-level=2 -Cllvm-args=-opt-bisect-limit=3642 &amp;&amp; ./main
```
And this produces the bad hash:
```
rustc +nightly main.rs -Copt-level=2 -Cllvm-args=-opt-bisect-limit=3647 &amp;&amp; ./main
```
And the passes in that bisect range are:
```
BISECT: running pass (3643) AArch64 Instruction Selection on function (_ZN4main4main17he1f737bca831c768E)
BISECT: running pass (3644) SME Peephole Optimization pass on function (_ZN4main4main17he1f737bca831c768E)
BISECT: running pass (3645) Early Tail Duplication on function (_ZN4main4main17he1f737bca831c768E)
BISECT: running pass (3646) Optimize machine instruction PHIs on function (_ZN4main4main17he1f737bca831c768E)
BISECT: running pass (3647) Merge disjoint stack slots on function (_ZN4main4main17he1f737bca831c768E)
```

We've already checked that
```
rustc +nightly main.rs -Copt-level=2 -Cllvm-args=-no-stack-coloring=1 &amp;&amp; ./main
```
produces the bad hash, so it doesn't seem like this is related to stack coloring.
</details>


---

### Comment 2 - antoniofrighetto

Likely some AArch64 combining pattern as I couldn't reproduce this with `-Cllvm-args='-global-isel=1'`.

---

### Comment 3 - nikic

I'm seeing this in the DAGCombine log:
```
Successfully custom legalized node
 ... replacing: t247: i64 = fshl Constant:i64<0>, Constant:i64<1>, Constant:i64<0>
with:      t250: i64 = fshr Constant:i64<0>, Constant:i64<1>, Constant:i64<64>
```
This is incorrect: https://alive2.llvm.org/ce/z/nMAGVv

---

### Comment 4 - nikic

Problematic transform: https://github.com/llvm/llvm-project/blob/c4f7ab1d2e27d7d742d89606eb40ffd547b448b1/llvm/lib/Target/AArch64/AArch64ISelLowering.cpp#L7268-L7273

This is hard to reproduce because a funnel shift by constant zero would usually get folded away by generic DAGCombine: https://github.com/llvm/llvm-project/blob/c4f7ab1d2e27d7d742d89606eb40ffd547b448b1/llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp#L11044-L11049

---

### Comment 5 - nikic

Reduction based on just the pattern in the combine log:
```llvm
define fastcc void @_ZN4test1d17h62d07d387379f03cE(ptr %_1, ptr %0) {
start:                    
  %_3.i = alloca [16 x i8], align 8
  %_2.i = alloca [24 x i8], align 8
  %_1.i = alloca [16 x i8], align 16
  %_12 = alloca [80 x i8], align 16
  %1 = getelementptr i8, ptr %_12, i64 48
  store ptr null, ptr %_1, align 8
  call void @llvm.lifetime.end.p0(i64 0, ptr %_3.i)
  call void @llvm.lifetime.end.p0(i64 0, ptr %_2.i)
  call void @llvm.lifetime.end.p0(i64 0, ptr %_1.i)
  %2 = getelementptr i8, ptr %_12, i64 16
  %3 = getelementptr i8, ptr %_12, i64 24
  store ptr null, ptr %_1, align 8
  %4 = getelementptr i8, ptr %_12, i64 32
  %5 = getelementptr i8, ptr %_12, i64 64
  store i64 0, ptr %5, align 16
  store ptr null, ptr %_1, align 16
  store i16 0, ptr %_1, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr null, ptr null, i64 6, i1 false) 
  call void @llvm.memcpy.p0.p0.i64(ptr %2, ptr %1, i64 24, i1 false)
  %6 = load i16, ptr %3, align 8
  %7 = load i64, ptr %4, align 16
  %8 = zext i64 %7 to i128
  %_6 = shl i128 1, %8
  store i16 0, ptr %3, align 8
  store i16 %6, ptr %0, align 8
  store i128 %_6, ptr %_1, align 16
  ret void
}
```
Unfortunately this is very fragile, e.g. dropping the null-to-null memcpy fixes the issue...

---

### Comment 6 - davemgreen

Sounds similar to the issue from https://github.com/llvm/llvm-project/commit/ef40ae4f4e273828e81de08a8b5ab71ac3f59c79. These zero-shift funnel shifts almost never come up as they are almost always canonicalized away beforehand, so are often missed.

---

### Comment 7 - paulwalker-arm

https://github.com/llvm/llvm-project/pull/140058 is a candidate fix.

---

### Comment 8 - paulwalker-arm

@saethlin - I've landed a fix. Can you verify it is sufficient to solve your original issue?

---

### Comment 9 - saethlin

Yup, I have verified that the Rust reproducer above no longer miscompiles.

---

