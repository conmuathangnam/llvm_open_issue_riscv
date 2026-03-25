# Incomplete program triggers infinite loop in clang::Sema

**Author:** karineek
**URL:** https://github.com/llvm/llvm-project/issues/55312

## Body

This code hangs clang-14 with -O3:
```cpp
struct a {
  signed ov6;
  signed pv6;
  signed qv6;
  signed rv6;
  struct {
    float jv6;
  } sv6;
  int tv6;
} b;
a c() {
  b.mv6.av6 == c.cv6 == c.mv6.d
```
We are doing a study about compiler fuzzers. We got these from one of the tools (not ours). 
I wonder if such a bug is interesting even if the program is chopped in the middle?

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - AaronBallman

`TransformTypos::RecursiveTransformLoop()` is where we get stuck. Within 
```
  bool finished() {
    return CorrectionResults.empty() &&
           CurrentTCIndex >= ValidatedCorrections.size();
  }
```
`CorrectionResults` is empty, `CurrentTCIndex` is `1`, and `ValidatedCorrections` has seven elements, so this returns false and we never finish.

CC @hokein

---

