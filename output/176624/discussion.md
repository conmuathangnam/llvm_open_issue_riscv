# fmaxnm/fminnm convert SNAN to QNAN on aarch64

**Author:** youknowone
**URL:** https://github.com/llvm/llvm-project/issues/176624
**Status:** Closed
**Labels:** miscompilation, llvm:SelectionDAG
**Closed Date:** 2026-03-05T08:47:03Z

## Body

I will try my best to describe what's this issue.

I only could reproduce this problem with Rust.
The original report to rust project: https://github.com/rust-lang/rust/issues/151286

I tested it with llvm-project:main and found same issue remains.

I wrote a patch fix this problem #176625

----

This is what I am guessing.

On AArch64 in release builds, this code returns wrong result (only in complex code):

```rust
let snan: f64 = f64::from_bits(0x7FF0_0000_0000_0001);
let max = if snan > 0.0 { snan } else { 0.0 };
// Expected: 0.0 (because fcmp ogt returns false for SNAN)
// Got: QNAN 0x7FF8000000000001
```

Only happens at opt-level >= 1. Debug builds work fine.


LLVM generates this IR: https://godbolt.org/z/dnjYPrsKa
```llvm
%cmp = fcmp ogt double %snan, 0.0
%max = select i1 %cmp, double %snan, double 0.0
```

DAGCombiner combines it to:
```llvm
%max = fmaxnum double %snan, 0.0
```

AArch64 backend selects:
```asm
fmaxnm d0, d1, d2
```

The `fmaxnm` instruction converts SNAN to QNAN. But `fcmp ogt` returns false for SNAN, so the original code should return `0.0`.



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Jeong, YunWon (youknowone)

<details>
I will try my best to describe what's this issue. But not good at what to tell right now. 

I only could reproduce this problem with Rust.
The original report to rust project: https://github.com/rust-lang/rust/issues/151286

I tested it with llvm-project:main and found same issue remains.

I investigated this issue more and have a working patch. PR soon.
</details>


---

### Comment 2 - davemgreen

This (the DAG combine) probably requires nnan if we care about snan enough to lose performance over it.

GCC defaults to -fno-signalling-nans, which a concept even more strict than trapping-math. I think we should be doing the same, but even it does not fold this without -ffinite-math-only (and -fno-signed-zeros) https://godbolt.org/z/xP11dhcMq.

---

### Comment 3 - nikic

It looks like DAGCombine already handles this correctly: https://github.com/llvm/llvm-project/blob/f4de9b89e9489deda24b6143c574ee8688d4a734/llvm/lib/CodeGen/SelectionDAG/DAGCombiner.cpp#L12018-L12021

I think this comes from the incorrect logic in SDAGBuilder. There is a fix for that at https://github.com/llvm/llvm-project/pull/93575. cc @dtcxzyw @arsenm 

---

### Comment 4 - RalfJung

Interestingly, on LLVM 20, this code also miscompiles:
```rust
fn minimum_num(x: f32, y: f32) -> f32 {
    if x.is_nan() || y >= x {
        y
    } else {
        // Either y < x or y is a NaN.
        x
    }
}

const SNAN: f32 = f32::from_bits(f32::NAN.to_bits() - 1);

fn main() {
    dbg!(minimum_num(-9.0, std::hint::black_box(SNAN)));
}
```
That got fixed for this simple case, but complex versions of the code seem to still run into the same miscompilation, likely the one from this issue. Was there an attempt to fix the miscompile but it was incomplete?

---

