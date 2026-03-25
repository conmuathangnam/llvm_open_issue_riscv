# [clang] Crash when specifying default values of template parameters of lambdas in require clauses of lambdas.

**Author:** Timothy-Liuxf
**URL:** https://github.com/llvm/llvm-project/issues/132925

## Body

```cpp
void func() {
    [](auto x) requires requires {
        []<int = 0>() {}();
    } {}(0);
}
```

The above code compiled with `clang++-19 -std=c++23` (or `clang++-20`, `-std=c++20`) will cause Clang to crash.

## Error Message

The error messages are as follows (Too long, folded in `<details>`):

<details>

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/timothy/tmp -fcoverage-compilation-dir=/home/timothy/tmp -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/x86_64-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-01e6cd.o -x c++ bug.cpp
1.      bug.cpp:4:11: current parser token ')'
2.      bug.cpp:1:13: parsing function body 'func'
3.      bug.cpp:1:13: in compound statement ('{}')
4.      bug.cpp:3:9: instantiating function definition 'func()::(anonymous class)::operator()(int)::(anonymous class)::operator()<0>'
5.      bug.cpp:3:9: LLVM IR generation of declaration 'func()::(anonymous class)::operator()(int)::(anonymous class)::operator()'
6.      bug.cpp:3:9: Mangling declaration 'func()::(anonymous class)::operator()(int)::(anonymous class)::operator()'
  #0 0x00007fb9149350af llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebd0af)
  #1 0x00007fb914932db9 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebadb9)
  #2 0x00007fb914935800 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebd800)
  #3 0x00007fb913505330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
  #4 0x00007fb91c7c4a47 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5a47)
  #5 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
  #6 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
  #7 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
  #8 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
  #9 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #10 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #11 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #12 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #13 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #14 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #15 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #16 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #17 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #18 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #19 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #20 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #21 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #22 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #23 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #24 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #25 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #26 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #27 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #28 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #29 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #30 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #31 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #32 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #33 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #34 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #35 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #36 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #37 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #38 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #39 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #40 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #41 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #42 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #43 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #44 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #45 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #46 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #47 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #48 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #49 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #50 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #51 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #52 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #53 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #54 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #55 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #56 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #57 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #58 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #59 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #60 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #61 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #62 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #63 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #64 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #65 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #66 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #67 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #68 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #69 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #70 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #71 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #72 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #73 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #74 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #75 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #76 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #77 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #78 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #79 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #80 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #81 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #82 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #83 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #84 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #85 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #86 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #87 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #88 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #89 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #90 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #91 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #92 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #93 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #94 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #95 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #96 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #97 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #98 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #99 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#100 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#101 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#102 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#103 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#104 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#105 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#106 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#107 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#108 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#109 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#110 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#111 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#112 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#113 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#114 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#115 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#116 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#117 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#118 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#119 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#120 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#121 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#122 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#123 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#124 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#125 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#126 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#127 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#128 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#129 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#130 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#131 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#132 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#133 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#134 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#135 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#136 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#137 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#138 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#139 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#140 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#141 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#142 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#143 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#144 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#145 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#146 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#147 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#148 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#149 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#150 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#151 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#152 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#153 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#154 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#155 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#156 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#157 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#158 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#159 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#160 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#161 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#162 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#163 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#164 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#165 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#166 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#167 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#168 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#169 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#170 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#171 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#172 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#173 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#174 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#175 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#176 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#177 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#178 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#179 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#180 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#181 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#182 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#183 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#184 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#185 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#186 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#187 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#188 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#189 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#190 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#191 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#192 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#193 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#194 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#195 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#196 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#197 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#198 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#199 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#200 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#201 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#202 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#203 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#204 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#205 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#206 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#207 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#208 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#209 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#210 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#211 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#212 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#213 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#214 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#215 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#216 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#217 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#218 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#219 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#220 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#221 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#222 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#223 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#224 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#225 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#226 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#227 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#228 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#229 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#230 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#231 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#232 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#233 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#234 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#235 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#236 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#237 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#238 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#239 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#240 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#241 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#242 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#243 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#244 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#245 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#246 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#247 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#248 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#249 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#250 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#251 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#252 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#253 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#254 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#255 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
clang++-19: error: unable to execute command: Segmentation fault (core dumped)
clang++-19: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 19.1.1 (1ubuntu1~24.04.2)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++-19: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-19: note: diagnostic msg: /tmp/bug-b3fbac.cpp
clang++-19: note: diagnostic msg: /tmp/bug-b3fbac.sh
clang++-19: note: diagnostic msg:

********************
```

</details>

---

## Attached files

**bug-b3fbac:** [bug-b3fbac.tar.gz](https://github.com/user-attachments/files/19447810/bug-b3fbac.tar.gz)

---



## Comments

### Comment 1 - EugeneZelenko

Could you please try 20 or `main` branch? https://godbolt.org should be helpful.


---

### Comment 2 - Timothy-Liuxf

> Could you please try 20 or `main` branch? https://godbolt.org should be helpful.

I have already tried godbolt before. In godbolt, Clang 20 and Clang trunk will be killed by signal `SIGSEGV`: https://godbolt.org/z/WhE8cWczo

which is the same as Clang 19.

---

### Comment 3 - Timothy-Liuxf

I will try Clang 20 later on my local machine after installing.

---

### Comment 4 - Timothy-Liuxf

@EugeneZelenko 

I compiled the code with Clang 20.1.1: [Release llvmorg-20.1.1](https://github.com/llvm/llvm-project/releases/tag/llvmorg-20.1.1)

---

Attached files:
[bug-df4d07.tar.gz](https://github.com/user-attachments/files/19450330/bug-df4d07.tar.gz)

---

The logs are as follows:

<details>

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/timothy/code/LLVM-20.1.1-Linux-X64/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/timothy/code/LLVM-20.1.1-Linux-X64/bin -fcoverage-compilation-dir=/home/timothy/code/LLVM-20.1.1-Linux-X64/bin -resource-dir /home/timothy/code/LLVM-20.1.1-Linux-X64/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/x86_64-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /home/timothy/code/LLVM-20.1.1-Linux-X64/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-42d3c3.o -x c++ /home/timothy/tmp/bug.cpp
1.      /home/timothy/tmp/bug.cpp:4:11: current parser token ')'
2.      /home/timothy/tmp/bug.cpp:1:13: parsing function body 'func'
3.      /home/timothy/tmp/bug.cpp:1:13: in compound statement ('{}')
4.      /home/timothy/tmp/bug.cpp:3:9: instantiating function definition 'func()::(anonymous class)::operator()(int)::(anonymous class)::operator()<0>'
5.      /home/timothy/tmp/bug.cpp:3:9: LLVM IR generation of declaration 'func()::(anonymous class)::operator()(int)::(anonymous class)::operator()'
6.      /home/timothy/tmp/bug.cpp:3:9: Mangling declaration 'func()::(anonymous class)::operator()(int)::(anonymous class)::operator()'
  #0 0x000055bbd6a17757 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/timothy/code/LLVM-20.1.1-Linux-X64/bin/clang-20+0x8017757)
  #1 0x000055bbd6a17b8f SignalHandler(int) Signals.cpp:0:0
  #2 0x00007fa047a13330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
  #3 0x000055bbd98ad9ae (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
  #4 0x000055bbd98aaf79 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #5 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #6 0x000055bbd98ab0bb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #7 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #8 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
  #9 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #10 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #11 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #12 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #13 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #14 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #15 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #16 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #17 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #18 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #19 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #20 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #21 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #22 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #23 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #24 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #25 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #26 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #27 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #28 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #29 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #30 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #31 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #32 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #33 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #34 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #35 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #36 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #37 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #38 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #39 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #40 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #41 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #42 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #43 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #44 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #45 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #46 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #47 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #48 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #49 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #50 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #51 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #52 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #53 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #54 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #55 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #56 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #57 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #58 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #59 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #60 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #61 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #62 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #63 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #64 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #65 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #66 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #67 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #68 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #69 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #70 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #71 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #72 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #73 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #74 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #75 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #76 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #77 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #78 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #79 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #80 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #81 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #82 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #83 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #84 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #85 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #86 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #87 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #88 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #89 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #90 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #91 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #92 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #93 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
 #94 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #95 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #96 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #97 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #98 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #99 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#100 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#101 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#102 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#103 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#104 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#105 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#106 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#107 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#108 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#109 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#110 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#111 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#112 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#113 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#114 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#115 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#116 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#117 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#118 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#119 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#120 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#121 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#122 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#123 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#124 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#125 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#126 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#127 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#128 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#129 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#130 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#131 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#132 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#133 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#134 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#135 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#136 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#137 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#138 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#139 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#140 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#141 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#142 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#143 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#144 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#145 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#146 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#147 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#148 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#149 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#150 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#151 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#152 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#153 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#154 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#155 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#156 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#157 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#158 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#159 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#160 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#161 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#162 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#163 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#164 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#165 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#166 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#167 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#168 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#169 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#170 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#171 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#172 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#173 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#174 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#175 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#176 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#177 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#178 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#179 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#180 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#181 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#182 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#183 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#184 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#185 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#186 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#187 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#188 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#189 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#190 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#191 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#192 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#193 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#194 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#195 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#196 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#197 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#198 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#199 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#200 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#201 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#202 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#203 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#204 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#205 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#206 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#207 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#208 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#209 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#210 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#211 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#212 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#213 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#214 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#215 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#216 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#217 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#218 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#219 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#220 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#221 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#222 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#223 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#224 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#225 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#226 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#227 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#228 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#229 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#230 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#231 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#232 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#233 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#234 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#235 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#236 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#237 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#238 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#239 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#240 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#241 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#242 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#243 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#244 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#245 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#246 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#247 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#248 0x000055bbd98b5c7a (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#249 0x000055bbda4b0cf4 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#250 0x000055bbda4aeaa3 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#251 0x000055bbd89c8618 (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#252 0x000055bbda4b0de2 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#253 0x000055bbdaab56c2 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) (.cold.0) ItaniumMangle.cpp:0:0
#254 0x000055bbd98ab0cb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#255 0x000055bbd989d0ee (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.1 (https://github.com/llvm/llvm-project 424c2d9b7e4de40d0804dd374721e6411c27d1d1)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/timothy/code/LLVM-20.1.1-Linux-X64/bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/bug-df4d07.cpp
clang++: note: diagnostic msg: /tmp/bug-df4d07.sh
clang++: note: diagnostic msg:

********************
```
</details>


---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timothy Liu (Timothy-Liuxf)

<details>
```cpp
void func() {
    [](auto x) requires requires {
        []&lt;int = 0&gt;() {}();
    } {}(0);
}
```

The above code compiled with `clang++-19 -std=c++23` (or `clang++-20`, `-std=c++20`) will cause Clang to crash.

## Error Message

The error messages are as follows (Too long, folded in `&lt;details&gt;`):

&lt;details&gt;

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm-19/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/timothy/tmp -fcoverage-compilation-dir=/home/timothy/tmp -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/x86_64-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-01e6cd.o -x c++ bug.cpp
1.      bug.cpp:4:11: current parser token ')'
2.      bug.cpp:1:13: parsing function body 'func'
3.      bug.cpp:1:13: in compound statement ('{}')
4.      bug.cpp:3:9: instantiating function definition 'func()::(anonymous class)::operator()(int)::(anonymous class)::operator()&lt;0&gt;'
5.      bug.cpp:3:9: LLVM IR generation of declaration 'func()::(anonymous class)::operator()(int)::(anonymous class)::operator()'
6.      bug.cpp:3:9: Mangling declaration 'func()::(anonymous class)::operator()(int)::(anonymous class)::operator()'
  #<!-- -->0 0x00007fb9149350af llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebd0af)
  #<!-- -->1 0x00007fb914932db9 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebadb9)
  #<!-- -->2 0x00007fb914935800 (/usr/lib/llvm-19/bin/../lib/libLLVM.so.19.1+0xebd800)
  #<!-- -->3 0x00007fb913505330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
  #<!-- -->4 0x00007fb91c7c4a47 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5a47)
  #<!-- -->5 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
  #<!-- -->6 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
  #<!-- -->7 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
  #<!-- -->8 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
  #<!-- -->9 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->10 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->11 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->12 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->13 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->14 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->15 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->16 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->17 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->18 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->19 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->20 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->21 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->22 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->23 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->24 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->25 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->26 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->27 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->28 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->29 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->30 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->31 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->32 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->33 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->34 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->35 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->36 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->37 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->38 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->39 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->40 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->41 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->42 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->43 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->44 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->45 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->46 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->47 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->48 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->49 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->50 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->51 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->52 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->53 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->54 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->55 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->56 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->57 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->58 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->59 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->60 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->61 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->62 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->63 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->64 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->65 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->66 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->67 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->68 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->69 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->70 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->71 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->72 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->73 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->74 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->75 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->76 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->77 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->78 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->79 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->80 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->81 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->82 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->83 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->84 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->85 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->86 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->87 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->88 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->89 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->90 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->91 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
 #<!-- -->92 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
 #<!-- -->93 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
 #<!-- -->94 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
 #<!-- -->95 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
 #<!-- -->96 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
 #<!-- -->97 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
 #<!-- -->98 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
 #<!-- -->99 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->100 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->101 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->102 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->103 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->104 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->105 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->106 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->107 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->108 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->109 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->110 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->111 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->112 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->113 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->114 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->115 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->116 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->117 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->118 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->119 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->120 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->121 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->122 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->123 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->124 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->125 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->126 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->127 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->128 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->129 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->130 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->131 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->132 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->133 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->134 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->135 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->136 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->137 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->138 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->139 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->140 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->141 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->142 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->143 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->144 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->145 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->146 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->147 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->148 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->149 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->150 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->151 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->152 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->153 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->154 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->155 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->156 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->157 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->158 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->159 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->160 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->161 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->162 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->163 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->164 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->165 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->166 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->167 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->168 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->169 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->170 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->171 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->172 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->173 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->174 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->175 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->176 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->177 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->178 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->179 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->180 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->181 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->182 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->183 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->184 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->185 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->186 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->187 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->188 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->189 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->190 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->191 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->192 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->193 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->194 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->195 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->196 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->197 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->198 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->199 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->200 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->201 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->202 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->203 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->204 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->205 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->206 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->207 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->208 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->209 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->210 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->211 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->212 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->213 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->214 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->215 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->216 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->217 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->218 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->219 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->220 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->221 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->222 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->223 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->224 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->225 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->226 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->227 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->228 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->229 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->230 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->231 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->232 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->233 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->234 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->235 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->236 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->237 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->238 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->239 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->240 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->241 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->242 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->243 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->244 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->245 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->246 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->247 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
#<!-- -->248 0x00007fb91c7ce966 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bf966)
#<!-- -->249 0x00007fb91c7d28aa (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c38aa)
#<!-- -->250 0x00007fb91c7cea1d (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bfa1d)
#<!-- -->251 0x00007fb91c7d848b (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11c948b)
#<!-- -->252 0x00007fb91c7c4e1a (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b5e1a)
#<!-- -->253 0x00007fb91c7da0d0 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11cb0d0)
#<!-- -->254 0x00007fb91c7c6420 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11b7420)
#<!-- -->255 0x00007fb91c7cdd00 (/usr/lib/llvm-19/bin/../lib/libclang-cpp.so.19.1+0x11bed00)
clang++-19: error: unable to execute command: Segmentation fault (core dumped)
clang++-19: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 19.1.1 (1ubuntu1~24.04.2)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-19/bin
clang++-19: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-19: note: diagnostic msg: /tmp/bug-b3fbac.cpp
clang++-19: note: diagnostic msg: /tmp/bug-b3fbac.sh
clang++-19: note: diagnostic msg:

********************
```

&lt;/details&gt;

---

## Attached files

**bug-b3fbac:** [bug-b3fbac.tar.gz](https://github.com/user-attachments/files/19447810/bug-b3fbac.tar.gz)

---


</details>


---

### Comment 6 - shafik

Looks like we get a backtrace on clang-17: https://godbolt.org/z/YKqbM3YnP but it is different than the one above. Not sure if this is two different bugs or not.

---

### Comment 7 - shafik

@zygoloid git bisect points to 4b163e343cfa54c8d55c9da73c70d58f55ea9df2 as the cause for this regression, which fits w/ the original backtrace which points to mangling. 

---

### Comment 8 - MagentaTreehouse

Looks like #131620.

---

