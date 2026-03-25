# segmentation fault with clang-22 (-O3 -Wno-return-type -Wno-implicit-int)

**Author:** ayseirmak
**URL:** https://github.com/llvm/llvm-project/issues/184947
**Status:** Open
**Labels:** llvm:SCEV, confirmed, crash

## Body

**Environment**
- Clang-Version: LLVM version 22.1.1
- Commit Hash: git [e693f0fe6390d491a7c9195c2a77d52581bd3f24](https://github.com/llvm/llvm-project/commit/e693f0fe6390d491a7c9195c2a77d52581bd3f24)
- Platform: Linux 5.15.0-168-generic 

when compiling the following code with this flag combination:
```C
// clang-22 -O3 -Wno-return-type -Wno-implicit-int input.c 

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
user42@node0:~$ clang-build/llvm-22-build/bin/clang-22 -O3 -Wno-return-type -Wno-implicit-int crash.c 
crash.c:3:11: warning: expression result unused [-Wunused-value]
    3 |   for (;; a) {
      |           ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /users/user42/clang-build/llvm-22-build/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O3 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name crash.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/users/user42 -fcoverage-compilation-dir=/users/user42 -resource-dir /users/user42/clang-build/llvm-22-build/lib/clang/22 -internal-isystem /users/user42/clang-build/llvm-22-build/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wno-return-type -Wno-implicit-int -ferror-limit 19 -fmessage-length=275 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -o /tmp/crash-2ed190.o -x c crash.c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'crash.c'.
4.      Running pass 'Loop Pass Manager' on function '@a'
5.      Running pass 'Loop Strength Reduction' on basic block '%263'
  #0 0x000056225998de50 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x2060e50)
  #1 0x000056225998b04d SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
  #2 0x00007f27e1ed6520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #3 0x0000562258b0172e llvm::ScalarEvolution::findExistingSCEVInCache(llvm::SCEVTypes, llvm::ArrayRef<llvm::SCEV const*>) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x11d472e)
  #4 0x0000562258b3e543 llvm::ScalarEvolution::getAddExpr(llvm::SmallVectorImpl<llvm::SCEV const*>&, llvm::SCEV::NoWrapFlags, unsigned int) (.part.0) ScalarEvolution.cpp:0:0
  #5 0x0000562258b40e1c llvm::ScalarEvolution::getAddExpr(llvm::SCEV const*, llvm::SCEV const*, llvm::SCEV::NoWrapFlags, unsigned int) (.constprop.0) ScalarEvolution.cpp:0:0
  #6 0x0000562258b40342 llvm::ScalarEvolution::getAddExpr(llvm::SmallVectorImpl<llvm::SCEV const*>&, llvm::SCEV::NoWrapFlags, unsigned int) (.part.0) ScalarEvolution.cpp:0:0
  #7 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
  #8 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
  #9 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #10 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #11 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #12 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #13 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #14 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #15 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #16 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #17 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #18 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #19 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #20 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #21 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #22 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #23 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #24 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #25 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #26 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #27 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #28 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #29 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #30 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #31 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #32 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #33 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #34 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #35 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #36 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #37 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #38 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #39 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #40 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #41 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #42 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #43 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #44 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #45 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #46 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #47 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #48 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #49 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #50 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #51 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #52 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #53 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #54 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #55 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #56 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #57 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #58 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #59 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #60 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #61 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #62 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #63 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #64 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #65 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #66 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #67 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #68 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #69 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #70 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #71 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #72 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #73 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #74 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #75 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #76 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #77 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #78 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #79 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #80 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #81 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #82 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #83 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #84 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #85 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #86 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #87 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #88 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #89 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #90 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #91 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #92 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #93 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #94 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #95 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #96 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #97 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
 #98 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
 #99 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#100 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#101 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#102 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#103 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#104 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#105 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#106 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#107 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#108 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#109 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#110 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#111 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#112 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#113 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#114 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#115 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#116 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#117 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#118 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#119 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#120 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#121 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#122 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#123 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#124 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#125 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#126 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#127 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#128 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#129 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#130 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#131 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#132 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#133 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#134 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#135 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#136 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#137 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#138 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#139 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#140 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#141 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#142 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#143 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#144 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#145 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#146 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#147 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#148 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#149 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#150 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#151 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#152 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#153 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#154 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#155 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#156 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#157 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#158 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#159 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#160 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#161 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#162 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#163 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#164 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#165 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#166 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#167 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#168 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#169 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#170 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#171 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#172 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#173 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#174 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#175 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#176 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#177 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#178 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#179 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#180 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#181 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#182 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#183 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#184 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#185 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#186 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#187 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#188 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#189 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#190 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#191 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#192 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#193 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#194 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#195 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#196 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#197 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#198 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#199 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#200 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#201 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#202 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#203 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#204 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#205 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#206 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#207 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#208 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#209 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#210 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#211 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#212 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#213 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#214 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#215 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#216 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#217 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#218 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#219 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#220 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#221 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#222 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#223 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#224 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#225 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#226 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#227 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#228 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#229 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#230 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#231 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#232 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#233 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#234 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#235 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#236 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#237 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#238 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#239 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#240 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#241 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#242 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#243 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#244 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#245 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#246 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#247 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#248 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#249 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#250 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#251 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#252 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#253 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
#254 0x0000562258b37a67 llvm::ScalarEvolution::getZeroExtendExpr(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x120aa67)
#255 0x0000562258b36bd8 llvm::ScalarEvolution::getZeroExtendExprImpl(llvm::SCEV const*, llvm::Type*, unsigned int) (/users/user42/clang-build/llvm-22-build/bin/clang-22+0x1209bd8)
clang-22: error: unable to execute command: Segmentation fault
clang-22: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.1.1 (https://github.com/llvm/llvm-project.git e693f0fe6390d491a7c9195c2a77d52581bd3f24)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /users/user42/clang-build/llvm-22-build/bin
clang-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-22: note: diagnostic msg: /tmp/crash-255585.c
clang-22: note: diagnostic msg: /tmp/crash-255585.sh
clang-22: note: diagnostic msg: 

********************
```
</details>

Below are the crash files:
```sh 
> cat /tmp/crash-255585.c

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
> cat /tmp/crash-255585.sh

# Crash reproducer for clang version 22.1.1 (https://github.com/llvm/llvm-project.git e693f0fe6390d491a7c9195c2a77d52581bd3f24)
# Driver args: "-O3" "-Wno-return-type" "-Wno-implicit-int" "crash.c"
# Original command:  "/users/user42/clang-build/llvm-22-build/bin/clang-22" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-O3" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "crash.c" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=none" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/users/user42" "-fcoverage-compilation-dir=/users/user42" "-resource-dir" "/users/user42/clang-build/llvm-22-build/lib/clang/22" "-internal-isystem" "/users/user42/clang-build/llvm-22-build/lib/clang/22/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/lib/gcc/x86_64-linux-gnu/11/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-Wno-return-type" "-Wno-implicit-int" "-ferror-limit" "19" "-fmessage-length=275" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-faddrsig" "-fdwarf2-cfi-asm" "-o" "/tmp/crash-2ed190.o" "-x" "c" "crash.c"
 "/users/user42/clang-build/llvm-22-build/bin/clang-22" "-cc1" "-triple" "x86_64-unknown-linux-gnu" "-O3" "-emit-obj" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "crash.c" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=none" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/users/user42" "-fcoverage-compilation-dir=/users/user42" "-Wno-return-type" "-Wno-implicit-int" "-ferror-limit" "19" "-fmessage-length=275" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcolor-diagnostics" "-vectorize-loops" "-vectorize-slp" "-faddrsig" "-fdwarf2-cfi-asm" "-x" "c" "crash-255585.c"
```
This segfault persists across versions 22 and 23 (23.0.0git [63db92d6d25bab26f28d8d263c70f47f06a380da](https://github.com/llvm/llvm-project/commit/63db92d6d25bab26f28d8d263c70f47f06a380da)). 
But not present on 20.
llvm::ScalarEvolution appears to be faulty, causing segmentation fault

