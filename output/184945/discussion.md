# segmentation fault with clang-22 (-Ofast -Wno-return-type -Wno-implicit-int)

**Author:** ayseirmak
**URL:** https://github.com/llvm/llvm-project/issues/184945
**Status:** Open
**Labels:** llvm:SCEV, crash

## Body

**Environment**
- Clang-Version: LLVM version 22.1.1
- Commit Hash: git [e693f0fe6390d491a7c9195c2a77d52581bd3f24](https://github.com/llvm/llvm-project/commit/e693f0fe6390d491a7c9195c2a77d52581bd3f24)
- Platform: Linux 5.15.0-168-generic 

when compiling the following code with this flag combination:
```C
// clang-22 -Ofast -Wno-return-type -Wno-implicit-int input.c 

a() {
  int b = 0, c = 0;
  for (;; a) {
    c++;
    if (c <= 14)
      continue;
    b++;
    if (b <= 45)
      continue;
    return;
  }
}

```
<details>
<summary>Below is the stack trace we get</summary>

```sh
user42@node0:~$ clang-build/llvm-22-build/bin/clang-22 -Ofast -Wno-return-type -Wno-implicit-int crash.c 
clang-22: warning: argument '-Ofast' is deprecated; use '-O3 -ffast-math' for the same behavior, or '-O3' to enable only conforming optimizations [-Wdeprecated-ofast]
crash.c:3:11: warning: expression result unused [-Wunused-value]
    3 |   for (;; a) {
      |           ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /users/user42/clang-build/llvm-22-build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -Ofast -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name crash.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -menable-no-infs -menable-no-nans -fapprox-func -funsafe-math-optimizations -fno-signed-zeros -mreassociate -freciprocal-math -ffp-contract=fast -fno-rounding-math -ffast-math -ffinite-math-only -complex-range=basic -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/users/user42 -fcoverage-compilation-dir=/users/user42 -resource-dir /users/user42/clang-build/llvm-22-build/lib/clang/22 -internal-isystem /users/user42/clang-build/llvm-22-build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wno-return-type -Wno-implicit-int -ferror-limit 19 -fmessage-length=190 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -o /tmp/crash-4036bf.o -x c crash.c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'crash.c'.
4.      Running pass 'Loop Pass Manager' on function '@a'
5.      Running pass 'Loop Strength Reduction' on basic block '%263'
  #0 0x000055f119eace50 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x2060e50)
  #1 0x000055f119eaa04d SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
  #2 0x00007fc8b3004520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #3 0x000055f119df9bc3 llvm::FoldingSetBase::FindNodeOrInsertPos(llvm::FoldingSetNodeID const&, void*&, llvm::FoldingSetBase::FoldingSetInfo const&) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1fadbc3)
  #4 0x000055f1190207dd llvm::ScalarEvolution::findExistingSCEVInCache(llvm::SCEVTypes, llvm::ArrayRef<llvm::SCEV const*>) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x11d47dd)
  #5 0x000055f11905d543 llvm::ScalarEvolution::getAddExpr(llvm::SmallVectorImpl<llvm::SCEV const*>&, llvm::SCEV::NoWrapFlags, unsigned int) (.part.0) ScalarEvolution.cpp:0:0
  #6 0x000055f11905fe1c llvm::ScalarEvolution::getAddExpr(llvm::SCEV const*, llvm::SCEV const*, llvm::SCEV::NoWrapFlags, unsigned int) (.constprop.0) ScalarEvolution.cpp:0:0
  #7 0x000055f11905f342 llvm::ScalarEvolution::getAddExpr(llvm::SmallVectorImpl<llvm::SCEV const*>&, llvm::SCEV::NoWrapFlags, unsigned int) (.part.0) ScalarEvolution.cpp:0:0
  #8 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
  #9 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #10 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #11 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #12 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #13 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #14 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #15 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #16 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #17 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #18 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #19 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #20 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #21 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #22 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #23 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #24 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #25 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #26 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #27 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #28 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #29 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #30 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #31 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #32 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #33 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #34 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #35 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #36 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #37 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #38 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #39 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #40 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #41 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #42 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #43 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #44 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #45 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #46 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #47 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #48 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #49 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #50 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #51 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #52 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #53 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #54 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #55 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #56 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #57 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #58 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #59 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #60 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #61 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #62 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #63 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #64 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #65 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #66 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #67 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #68 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #69 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #70 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #71 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #72 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #73 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #74 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #75 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #76 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #77 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #78 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #79 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #80 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #81 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #82 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #83 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #84 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #85 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #86 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #87 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #88 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #89 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #90 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #91 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #92 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #93 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #94 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #95 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #96 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #97 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #98 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #99 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#100 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#101 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#102 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#103 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#104 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#105 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#106 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#107 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#108 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#109 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#110 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#111 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#112 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#113 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#114 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#115 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#116 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#117 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#118 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#119 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#120 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#121 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#122 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#123 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#124 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#125 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#126 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#127 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#128 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#129 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#130 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#131 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#132 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#133 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#134 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#135 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#136 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#137 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#138 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#139 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#140 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#141 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#142 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#143 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#144 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#145 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#146 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#147 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#148 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#149 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#150 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#151 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#152 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#153 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#154 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#155 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#156 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#157 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#158 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#159 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#160 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#161 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#162 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#163 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#164 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#165 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#166 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#167 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#168 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#169 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#170 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#171 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#172 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#173 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#174 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#175 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#176 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#177 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#178 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#179 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#180 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#181 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#182 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#183 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#184 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#185 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#186 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#187 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#188 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#189 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#190 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#191 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#192 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#193 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#194 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#195 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#196 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#197 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#198 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#199 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#200 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#201 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#202 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#203 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#204 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#205 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#206 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#207 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#208 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#209 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#210 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#211 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#212 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#213 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#214 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#215 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#216 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#217 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#218 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#219 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#220 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#221 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#222 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#223 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#224 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#225 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#226 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#227 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#228 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#229 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#230 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#231 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#232 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#233 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#234 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#235 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#236 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#237 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#238 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#239 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#240 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#241 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#242 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#243 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#244 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#245 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#246 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#247 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#248 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#249 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#250 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#251 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#252 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#253 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#254 0x000055f119055bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#255 0x000055f119056a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
clang-22: error: unable to execute command: Segmentation fault
clang-22: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.1.1 (https://github.com/llvm/llvm-project.git e693f0fe6390d491a7c9195c2a77d52581bd3f24)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /users/user42/clang-build/llvm-22-build/bin
clang-22: warning: argument '-Ofast' is deprecated; use '-O3 -ffast-math' for the same behavior, or '-O3' to enable only conforming optimizations [-Wdeprecated-ofast]
clang-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-22: note: diagnostic msg: /tmp/crash-5f967b.c
clang-22: note: diagnostic msg: /tmp/crash-5f967b.sh
clang-22: note: diagnostic msg: 

********************
```
</details>

Below are the crash files:
```sh 
> cat /tmp/crash-5f967b.c

# 1 "<built-in>"
# 1 "crash.c"
a() {
  int b = 0, c = 0;
  for (;; a) {
    c++;
    if (c <= 14)
      continue;
    b++;
    if (b <= 45)
      continue;
    return;
  }
}
```

```sh
> cat /tmp/crash-179d51.sh

# Crash reproducer for clang version 22.1.1 (https://github.com/llvm/llvm-project.git e693f0fe6390d491a7c9195c2a77d52581bd3f24)
# Driver args: "-Ofast" "-Wno-return-type" "-Wno-implicit-int" "crash.c"
# Original command:  "/users/user42/clang-build/llvm-22-build/bin/clang-22" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-Ofast" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "crash.c" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=none" "-menable-no-infs" "-menable-no-nans" "-fapprox-func" "-funsafe-math-optimizations" "-fno-signed-zeros" "-mreassociate" "-freciprocal-math" "-ffp-contract=fast" "-fno-rounding-math" "-ffast-math" "-ffinite-math-only" "-complex-range=basic" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/users/user42" "-fcoverage-compilation-dir=/users/user42" "-resource-dir" "/users/user42/clang-build/llvm-22-build/lib/clang/22" "-internal-isystem" "/users/user42/clang-build/llvm-22-build/lib/clang/22/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-Wno-return-type" "-Wno-implicit-int" "-ferror-limit" "19" "-fmessage-length=190" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-faddrsig" "-fdwarf2-cfi-asm" "-o" "/tmp/crash-4036bf.o" "-x" "c" "crash.c"
 "/users/user42/clang-build/llvm-22-build/bin/clang-22" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-Ofast" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "crash.c" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=none" "-menable-no-infs" "-menable-no-nans" "-fapprox-func" "-funsafe-math-optimizations" "-fno-signed-zeros" "-mreassociate" "-freciprocal-math" "-ffp-contract=fast" "-fno-rounding-math" "-ffast-math" "-ffinite-math-only" "-complex-range=basic" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/users/user42" "-fcoverage-compilation-dir=/users/user42" "-Wno-return-type" "-Wno-implicit-int" "-ferror-limit" "19" "-fmessage-length=190" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-faddrsig" "-fdwarf2-cfi-asm" "-x" "c" "crash-5f967b.c"
```
This segfault persists across versions 22 and 23 (23.0.0git [63db92d6d25bab26f28d8d263c70f47f06a380da](https://github.com/llvm/llvm-project/commit/63db92d6d25bab26f28d8d263c70f47f06a380da)). 
But not present on 20.
llvm::ScalarEvolution appears to be faulty, causing segmentation fault

