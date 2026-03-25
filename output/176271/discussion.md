# [clang][bytecode] SIGSEGV where shift count is negative

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/176271
**Status:** Closed
**Labels:** crash, regression:19, clang:bytecode
**Closed Date:** 2026-01-20T05:24:42Z

## Body

Reproducer:
https://godbolt.org/z/8z4jqvoYf
```cpp
void foo() {
  int i;
  i = 0 << (1 << 31);
}
```

fine with clang-18:
https://godbolt.org/z/6x6vYY1GG


## Comments

### Comment 1 - k-arrows

In my environment, running `clang++ -c -fexperimental-new-constant-interpreter foo.cpp` results in a segmentation fault (core dumped), while running `clang++ -fexperimental-new-constant-interpreter foo.cpp` results in a crash. A portion of the backtrace is shown below.
```console
  #4 0x00007f031ac43ae3 clang::interp::State::CCEDiag(clang::interp::SourceInfo const&, unsigned int, unsigned int)
  #5 0x00007f031ab7bf1c bool clang::interp::DoShift<clang::interp::Integral<32u, true>, clang::interp::Integral<32u, true>, (clang::interp::ShiftDir)0>(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Integral<32u, true>&, clang::interp::Integral<32u, true>&, clang::interp::Integral<32u, true>*)
  #6 0x00007f031ab7c197 bool clang::interp::DoShift<clang::interp::Integral<32u, true>, clang::interp::Integral<32u, true>, (clang::interp::ShiftDir)1>(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Integral<32u, true>&, clang::interp::Integral<32u, true>&, clang::interp::Integral<32u, true>*)
  #7 0x00007f031ab7bfd7 bool clang::interp::DoShift<clang::interp::Integral<32u, true>, clang::interp::Integral<32u, true>, (clang::interp::ShiftDir)0>(clang::interp::InterpState&, clang::interp::CodePtr, clang::interp::Integral<32u, true>&, clang::interp::Integral<32u, true>&, clang::interp::Integral<32u, true>*)
```

---

### Comment 2 - hax0kartik

This issue is an edgecase for the codepath added in this commit: https://github.com/llvm/llvm-project/commit/de029943cc5ad0028f16e6ecaffa03e32ffd1a6f. 

The negation(`RHS = -RHS`) performed in this new codepath is sufficient for most cases, however MIN signed values will need to be taken care of separately.

Minimal reproducible: https://godbolt.org/z/Mard8xYzs

Please assign this issue to me.

---

