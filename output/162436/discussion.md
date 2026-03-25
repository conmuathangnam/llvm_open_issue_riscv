# [AArch64] Crash trying to scalarize llvm.vector.partial.reduce.add.v2i128.v4i128

**Author:** fhahn
**URL:** https://github.com/llvm/llvm-project/issues/162436
**Status:** Open
**Labels:** backend:AArch64, crash, crash-on-valid

## Body

```
target triple = "arm64-apple-macosx"

define <2 x i128> @crash(<2 x i128> %a, <4 x i128> %b) {
  %r = call <2 x i128> @llvm.vector.partial.reduce.add.v2i128.v4i128(<2 x i128> %a, <4 x i128> %b)
  ret <2 x i128> %r
}
```

Running through `llc` results in 
```
ScalarizeVectorResult #0: t79: v1i128 = partial_reduce_umla t72, t75, t77

LLVM ERROR: Do not know how to scalarize the result of this operator!
```

https://llvm.godbolt.org/z/71nWK3qeT

## Comments

### Comment 1 - fhahn

cc @sdesmalen-arm @SamTebbs33 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Florian Hahn (fhahn)

<details>
```
target triple = "arm64-apple-macosx"

define &lt;2 x i128&gt; @<!-- -->crash(&lt;2 x i128&gt; %a, &lt;4 x i128&gt; %b) {
  %r = call &lt;2 x i128&gt; @<!-- -->llvm.vector.partial.reduce.add.v2i128.v4i128(&lt;2 x i128&gt; %a, &lt;4 x i128&gt; %b)
  ret &lt;2 x i128&gt; %r
}
```

Running through `llc` results in 
```
ScalarizeVectorResult #<!-- -->0: t79: v1i128 = partial_reduce_umla t72, t75, t77

LLVM ERROR: Do not know how to scalarize the result of this operator!
```

https://llvm.godbolt.org/z/71nWK3qeT
</details>


---

### Comment 3 - sdesmalen-arm

@fhahn is this just to record that we don't support scalarization for partial.reduce.add? Or did this appear anywhere from source code through vectorization? Because I recently addressed that issue for AArch64 in #162066.
Scalarization of these nodes is not yet implemented, nor is widening, although the latter is work in progress in #161834.

---

### Comment 4 - fhahn

> @fhahn is this just to record that we don't support scalarization for partial.reduce.add? 

Yes, ideally the backend should be able to lower any valid LLVM IR.

---

### Comment 5 - david-arm

I think this is probably true for a lot of IR, for example masked loads of predicates probably don't work, etc. There is an also an argument that the middle-end should never be generating IR that the backend says is illegal too, right? It's much more of a problem if we said something is legal, but then cannot lower it.

---

### Comment 6 - fhahn

> I think this is probably true for a lot of IR, for example masked loads of predicates probably don't work, etc. There is an also an argument that the middle-end should never be generating IR that the backend says is illegal too, right? It's much more of a problem if we said something is legal, but then cannot lower it.

I think in general we have (or at least aim to have) generic codegen to handle valid intrinsics, if possible. Masked loads/stores get expanded to a lot of code on plain NEON, but don't crash: https://llvm.godbolt.org/z/5Tdcz3zrr.

Doing something similar for partial reduction should also work.

---

