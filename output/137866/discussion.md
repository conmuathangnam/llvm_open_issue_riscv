# Clang crash ( Stack Dump + SIGSEGV) when initializing struct with too few arguments (constexpr constructor)

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/137866

## Body

## Summary

Clang crashes with SIGSEGV in gcc bolt and stack dump in local when compiling code that incorrectly initializes a struct with fewer arguments than required for a constexpr constructor.
Instead of reporting a clean diagnostic, the compiler crashes during parsing or constant evaluation.

## Stack dump

```
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O1 -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-2dedc7.o -x c prueba.cpp
1.      prueba.cpp:9:22: current parser token '='
  #0 0x0000557026f4318f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/local/bin/clang-21+0x3c4a18f)
  #1 0x0000557026f40bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
  #2 0x00007fb475beb420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
  #3 0x000055702a734e88 clang::QualType::getSplitDesugaredType(clang::QualType) (/usr/local/bin/clang-21+0x743be88)
  #4 0x000055702a0383cf clang::ASTContext::getBaseElementType(clang::QualType) const (/usr/local/bin/clang-21+0x6d3f3cf)
  #5 0x00005570294dd39a CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
  #6 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
  #7 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
  #8 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
  #9 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #10 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #11 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #12 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #13 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #14 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #15 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #16 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #17 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #18 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #19 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #20 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #21 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #22 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #23 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #24 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #25 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #26 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #27 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #28 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #29 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #30 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #31 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #32 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #33 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #34 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #35 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #36 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #37 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #38 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #39 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #40 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #41 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #42 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #43 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #44 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #45 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #46 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #47 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #48 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #49 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #50 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #51 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #52 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #53 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #54 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #55 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #56 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #57 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #58 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #59 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #60 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #61 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #62 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #63 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #64 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #65 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #66 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #67 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #68 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #69 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #70 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #71 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #72 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #73 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #74 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #75 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #76 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #77 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #78 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #79 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #80 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #81 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #82 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #83 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #84 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #85 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #86 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #87 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #88 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #89 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #90 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #91 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #92 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #93 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #94 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #95 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #96 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #97 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #98 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #99 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#100 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#101 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#102 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#103 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#104 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#105 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#106 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#107 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#108 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#109 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#110 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#111 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#112 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#113 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#114 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#115 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#116 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#117 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#118 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#119 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#120 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#121 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#122 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#123 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#124 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#125 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#126 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#127 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#128 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#129 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#130 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#131 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#132 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#133 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#134 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#135 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#136 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#137 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#138 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#139 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#140 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#141 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#142 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#143 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#144 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#145 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#146 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#147 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#148 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#149 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#150 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#151 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#152 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#153 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#154 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#155 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#156 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#157 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#158 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#159 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#160 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#161 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#162 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#163 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#164 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#165 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#166 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#167 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#168 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#169 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#170 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#171 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#172 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#173 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#174 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#175 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#176 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#177 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#178 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#179 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#180 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#181 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#182 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#183 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#184 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#185 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#186 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#187 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#188 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#189 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#190 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#191 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#192 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#193 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#194 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#195 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#196 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#197 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#198 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#199 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#200 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#201 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#202 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#203 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#204 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#205 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#206 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#207 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#208 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#209 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#210 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#211 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#212 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#213 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#214 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#215 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#216 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#217 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#218 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#219 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#220 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#221 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#222 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#223 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#224 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#225 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#226 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#227 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#228 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#229 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#230 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#231 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#232 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#233 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#234 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#235 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#236 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#237 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#238 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#239 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#240 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#241 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#242 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#243 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#244 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#245 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#246 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#247 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#248 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#249 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#250 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#251 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#252 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#253 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#254 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#255 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
```

## Program

```
struct Vec {
    float x;
    float y;
    Vec(float x, float y)
        : x(x)
       , y(y) {}
};

constexpr Vec unit_z = Vec{0};
```

## To quickly reproduce

(Here SIGSEV is obtained not stack dump, as happen in local)
https://gcc.godbolt.org/z/Mz6M56or4



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary

Clang crashes with SIGSEGV in gcc bolt and stack dump in local when compiling code that incorrectly initializes a struct with fewer arguments than required for a constexpr constructor.
Instead of reporting a clean diagnostic, the compiler crashes during parsing or constant evaluation.

## Stack dump

```
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O1 -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-2dedc7.o -x c prueba.cpp
1.      prueba.cpp:9:22: current parser token '='
  #<!-- -->0 0x0000557026f4318f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/bin/clang-21+0x3c4a18f)
  #<!-- -->1 0x0000557026f40bd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
  #<!-- -->2 0x00007fb475beb420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
  #<!-- -->3 0x000055702a734e88 clang::QualType::getSplitDesugaredType(clang::QualType) (/usr/local/bin/clang-21+0x743be88)
  #<!-- -->4 0x000055702a0383cf clang::ASTContext::getBaseElementType(clang::QualType) const (/usr/local/bin/clang-21+0x6d3f3cf)
  #<!-- -->5 0x00005570294dd39a CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
  #<!-- -->6 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
  #<!-- -->7 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
  #<!-- -->8 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
  #<!-- -->9 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->10 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->11 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->12 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->13 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->14 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->15 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->16 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->17 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->18 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->19 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->20 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->21 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->22 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->23 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->24 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->25 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->26 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->27 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->28 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->29 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->30 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->31 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->32 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->33 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->34 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->35 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->36 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->37 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->38 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->39 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->40 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->41 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->42 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->43 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->44 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->45 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->46 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->47 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->48 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->49 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->50 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->51 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->52 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->53 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->54 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->55 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->56 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->57 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->58 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->59 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->60 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->61 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->62 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->63 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->64 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->65 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->66 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->67 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->68 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->69 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->70 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->71 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->72 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->73 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->74 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->75 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->76 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->77 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->78 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->79 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->80 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->81 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->82 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->83 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->84 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->85 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->86 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->87 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->88 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->89 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->90 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->91 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->92 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->93 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->94 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->95 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->96 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->97 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->98 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
 #<!-- -->99 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->100 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->101 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->102 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->103 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->104 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->105 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->106 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->107 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->108 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->109 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->110 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->111 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->112 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->113 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->114 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->115 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->116 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->117 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->118 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->119 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->120 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->121 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->122 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->123 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->124 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->125 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->126 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->127 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->128 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->129 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->130 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->131 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->132 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->133 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->134 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->135 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->136 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->137 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->138 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->139 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->140 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->141 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->142 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->143 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->144 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->145 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->146 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->147 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->148 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->149 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->150 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->151 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->152 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->153 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->154 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->155 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->156 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->157 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->158 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->159 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->160 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->161 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->162 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->163 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->164 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->165 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->166 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->167 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->168 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->169 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->170 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->171 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->172 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->173 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->174 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->175 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->176 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->177 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->178 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->179 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->180 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->181 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->182 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->183 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->184 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->185 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->186 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->187 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->188 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->189 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->190 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->191 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->192 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->193 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->194 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->195 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->196 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->197 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->198 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->199 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->200 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->201 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->202 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->203 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->204 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->205 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->206 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->207 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->208 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->209 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->210 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->211 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->212 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->213 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->214 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->215 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->216 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->217 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->218 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->219 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->220 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->221 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->222 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->223 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->224 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->225 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->226 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->227 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->228 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->229 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->230 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->231 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->232 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->233 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->234 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->235 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->236 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->237 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->238 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->239 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->240 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->241 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->242 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->243 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->244 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->245 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->246 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->247 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->248 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->249 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->250 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->251 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->252 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->253 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->254 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
#<!-- -->255 0x00005570294dd52f CheckC23ConstexprVarType(clang::Sema&amp;, clang::SourceLocation, clang::QualType) SemaDecl.cpp:0:0
```

## Program

```
struct Vec {
    float x;
    float y;
    Vec(float x, float y)
        : x(x)
       , y(y) {}
};

constexpr Vec unit_z = Vec{0};
```

## To quickly reproduce

(Here SIGSEV is obtained not stack dump, as happen in local)
https://gcc.godbolt.org/z/Mz6M56or4


</details>


---

### Comment 2 - shafik

Is this fuzzer generate?

CC @Fznamznon 

---

### Comment 3 - mariete1223

Yes

---

### Comment 4 - Fznamznon

Because the "constructor" in C gets parsed as a field of the outer type itself, we enter an endless recursion when recursively checking type's fields. 

---

