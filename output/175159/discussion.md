# [WebAssembly] Assertion `llvm::isUIntN(BitWidth, val) && "Value is not an N-bit unsigned value"' failed

**Author:** mr-c
**URL:** https://github.com/llvm/llvm-project/issues/175159
**Status:** Closed
**Labels:** crash, constant-folding
**Closed Date:** 2026-01-09T16:15:40Z

## Body

Reproduced using 804aa88317ddbdb6e713306d48dbee0443b252b1

git-bisecting from the emscripten side narrowed down the introduction of the crash to https://github.com/llvm/llvm-project/compare/55eaa6c27b36...88b77d5eaa66


[26060.zip](https://github.com/user-attachments/files/24526777/26060.zip)

Emscripten issue: https://github.com/emscripten-core/emscripten/issues/26060#issue-3795333593

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-webassembly

Author: Michael R. Crusoe (mr-c)

<details>
Reproduced using 804aa88317ddbdb6e713306d48dbee0443b252b1

git-bisecting from the emscripten side narrowed down the introduction of the crash to https://github.com/llvm/llvm-project/compare/55eaa6c27b36...88b77d5eaa66


[26060.zip](https://github.com/user-attachments/files/24526777/26060.zip)

Emscripten issue: https://github.com/emscripten-core/emscripten/issues/26060#issue-3795333593
</details>


---

### Comment 2 - mr-c

I've git-bisected this to a83c89495ba6fe0134dcaa02372c320cc7ff0dbf
Tagging @nikic 

---

### Comment 3 - nikic

Reduced:
```llvm
; RUN: opt -S -passes=instsimplify < %s
target triple = "wasm32-unknown-emscripten"

define <4 x i32> @test() {
  %dot = tail call <4 x i32> @llvm.wasm.dot(<8 x i16> <i16 19733, i16 18694, i16 -2158, i16 -12949, i16 -22664, i16 -3805, i16 30779, i16 1600>, <8 x i16> <i16 11918, i16 7268, i16 32199, i16 14806, i16 -14152, i16 -23986, i16 16941, i16 17135>)
  ret <4 x i32> %dot
}
```

---

### Comment 4 - mr-c

Thank you for the quick fix @nikic ! I can confirm it from my end using emscripten + https://github.com/llvm/llvm-project/commit/4996e3be341af782b856597023c83ee3ef9b5094 and the entire SIMDe test suite

---

