# Exception code 0xC0000005, clang frontend command failed due to signal

**Author:** retifrav
**URL:** https://github.com/llvm/llvm-project/issues/153182

## Body

I got this crash on building a project (*SPIRV-Cross library*) with Emscripten (*v4.0.12*), and the output said to report it here, so here goes.

Steps to reproduce:

``` sh
$ git clone --branch vulkan-sdk-1.4.321.0 --single-branch git@github.com:KhronosGroup/SPIRV-Cross.git
$ cd ./SPIRV-Cross
$ git rev-parse HEAD
d8e3e2b141b8c8a167b2e3984736a6baacff316c

$ mkdir build && cd $_
$ source /e/tools/emsdk-llvm/emsdk_env.sh
$ emcc --version
emcc (Emscripten gcc/clang-like replacement + linker emulating GNU ld) 4.0.12 (6f625bd62ee64f4f8cd11476774741b961e4a2d5)

$ cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="../install" \
    -DCMAKE_TOOLCHAIN_FILE="$EMSDK/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" \
    -DSPIRV_CROSS_ENABLE_CPP=0 \
    -DSPIRV_CROSS_ENABLE_REFLECT=0 \
    -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS=1 \
    -DSPIRV_CROSS_ENABLE_TESTS=0 \
    -DSPIRV_CROSS_CLI=0 \
    ..
$ cmake --build . --target install
```

The build output and crash backtrace:

``` sh
[10/16] Building CXX object CMakeFiles/spirv-cross-glsl.dir/spirv_glsl.cpp.o
FAILED: CMakeFiles/spirv-cross-glsl.dir/spirv_glsl.cpp.o
E:\tools\emsdk-llvm\upstream\emscripten\em++.bat -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS -ID:/programs/_src/SPIRV-Cross -O3 -DNDEBUG -std=c++11 -fansi-escape-codes -fcolor-diagnostics -fno-exceptions -Wall -Wextra -Wshadow -Wno-deprecated-declarations -MD -MT CMakeFiles/spirv-cross-glsl.dir/spirv_glsl.cpp.o -MF CMakeFiles\spirv-cross-glsl.dir\spirv_glsl.cpp.o.d -o CMakeFiles/spirv-cross-glsl.dir/spirv_glsl.cpp.o -c D:/programs/_src/SPIRV-Cross/spirv_glsl.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: E:/tools/emsdk-llvm/upstream/bin\\clang++.exe -target wasm32-unknown-emscripten -fignore-exceptions -mllvm -combiner-global-alias-analysis=false -mllvm -enable-emscripten-sjlj -mllvm -disable-lsr --sysroot=E:\\tools\\emsdk-llvm\\upstream\\emscripten\\cache\\sysroot -DEMSCRIPTEN -Xclang -iwithsysroot/include\\fakesdl -Xclang -iwithsysroot/include\\compat -DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS -ID:/programs/_src/SPIRV-Cross -O3 -DNDEBUG -std=c++11 -fansi-escape-codes -fcolor-diagnostics -fno-exceptions -Wall -Wextra -Wshadow -Wno-deprecated-declarations -MD -MT CMakeFiles/spirv-cross-glsl.dir/spirv_glsl.cpp.o -MF CMakeFiles\\spirv-cross-glsl.dir\\spirv_glsl.cpp.o.d -oCMakeFiles/spirv-cross-glsl.dir/spirv_glsl.cpp.o -c D:/programs/_src/SPIRV-Cross/spirv_glsl.cpp
1.      <eof> parser at end of file
2.      Optimizer
3.      Running pass "require<globals-aa>,function(invalidate<aa>),require<profile-summary>,cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide)),function(invalidate<should-not-run-function-passes>),cgscc(devirt<4>())" on module "D:/programs/_src/SPIRV-Cross/spirv_glsl.cpp"
4.      Running pass "cgscc(devirt<4>(inline,function-attrs<skip-non-recursive-function-attrs>,argpromotion,openmp-opt-cgscc,function<eager-inv;no-rerun>(sroa<modify-cfg>,early-cse<memssa>,speculative-execution<only-if-divergent-target>,jump-threading,correlated-propagation,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,aggressive-instcombine,libcalls-shrinkwrap,tailcallelim,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,reassociate,constraint-elimination,loop-mssa(loop-instsimplify,loop-simplifycfg,licm<no-allowspeculation>,loop-rotate<header-duplication;no-prepare-for-lto>,licm<allowspeculation>,simple-loop-unswitch<nontrivial;trivial>),simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;no-hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;no-sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>,loop(loop-idiom,indvars,extra-simple-loop-unswitch-passes,loop-deletion,loop-unroll-full),sroa<modify-cfg>,vector-combine,mldst-motion<no-split-footer-bb>,gvn<>,sccp,bdce,instcombine<max-iterations=1;no-verify-fixpoint>,jump-threading,correlated-propagation,adce,memcpyopt,dse,move-auto-init,loop-mssa(licm<allowspeculation>),coro-elide,simplifycfg<bonus-inst-threshold=1;no-forward-switch-cond;switch-range-to-icmp;no-switch-to-lookup;keep-loops;hoist-common-insts;no-hoist-loads-stores-with-cond-faulting;sink-common-insts;speculate-blocks;simplify-cond-branch;no-speculate-unpredictables>,instcombine<max-iterations=1;no-verify-fixpoint>),function-attrs,function(require<should-not-run-function-passes>),coro-split,coro-annotation-elide))" on module "D:/programs/_src/SPIRV-Cross/spirv_glsl.cpp"
5.      Running pass "instcombine<max-iterations=1;no-verify-fixpoint>" on function "_ZN11spirv_cross12CompilerGLSL22constant_op_expressionERKNS_14SPIRConstantOpE"
Exception Code: 0xC0000005
 #0 0x00007ff6223038f6 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x8638f6)
 #1 0x00007ff621cd6e2a (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x236e2a)
 #2 0x00007ff621cdf6db (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x23f6db)
 #3 0x00007ff6227e1729 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0xd41729)
 #4 0x00007ff621cef30a (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x24f30a)
 #5 0x00007ff621cf771e (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x25771e)
 #6 0x00007ff621cf7005 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x257005)
 #7 0x00007ff622bc4f21 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x1124f21)
 #8 0x00007ff62240993b (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x96993b)
 #9 0x00007ff6221ae891 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x70e891)
#10 0x00007ff6233d3591 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x1933591)
#11 0x00007ff6221ae44b (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x70e44b)
#12 0x00007ff6233cfc2e (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x192fc2e)
#13 0x00007ff623e3241b (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x239241b)
#14 0x00007ff6233d1f54 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x1931f54)
#15 0x00007ff623edfa7b (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x243fa7b)
#16 0x00007ff6233d17de (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x19317de)
#17 0x00007ff6221ae2b1 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x70e2b1)
#18 0x00007ff622408847 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x968847)
#19 0x00007ff6251f9a74 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x3759a74)
#20 0x00007ff623e351e1 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x23951e1)
#21 0x00007ff622408847 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x968847)
#22 0x00007ff622bbfcf8 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x111fcf8)
#23 0x00007ff622bb76d3 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x11176d3)
#24 0x00007ff622f5ca4b (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x14bca4b)
#25 0x00007ff6247ad527 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x2d0d527)
#26 0x00007ff622e89522 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x13e9522)
#27 0x00007ff621fa94bd (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x5094bd)
#28 0x00007ff622057816 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x5b7816)
#29 0x00007ff621aa8a6c (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x8a6c)
#30 0x00007ff621aa3fc5 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x3fc5)
#31 0x00007ff622be289d (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x114289d)
#32 0x00007ff622be1fc9 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x1141fc9)
#33 0x00007ff621f8ee47 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x4eee47)
#34 0x00007ff621f8f0cd (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x4ef0cd)
#35 0x00007ff621f730b5 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x4d30b5)
#36 0x00007ff621aa3732 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x3732)
#37 0x00007ff621ab9d27 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x19d27)
#38 0x00007ff625f27d58 (E:\tools\emsdk-llvm\upstream\bin\clang++.exe+0x4487d58)
#39 0x00007ffe6ca17374 (C:\Windows\System32\KERNEL32.DLL+0x17374)
#40 0x00007ffe6cf9cc91 (C:\Windows\SYSTEM32\ntdll.dll+0x4cc91)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https:/github.com/llvm/llvm-project 7f93487862d98bf1c168babba87daf6224d8a46f)
Target: wasm32-unknown-emscripten
Thread model: posix
InstalledDir: E:\tools\emsdk-llvm\upstream\bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:\Users\USERNAME\AppData\Local\Temp\spirv_glsl-159775.cpp
clang++: note: diagnostic msg: C:\Users\USERNAME\AppData\Local\Temp\spirv_glsl-159775.sh
clang++: note: diagnostic msg:

********************
[11/16] Building CXX object CMakeFiles/spirv-cross-msl.dir/spirv_msl.cpp.o
ninja: build stopped: subcommand failed.
```

The `spirv_glsl-159775*` files are attached here: [spirv_glsl-159775.zip](https://github.com/user-attachments/files/21736390/spirv_glsl-159775.zip)

Some additional notes:

- I could reproduce it on ARM-based Mac OS 15.6 and also x64-based Windows 10 and 11 (*the output above is from Windows 10, commands executed in Git BASH environment*)
- building Debug configuration (*`-DCMAKE_BUILD_TYPE=Debug`*) does not crash, only Release does
- the crash does not reproduce with Emscripten v4.0.11, both Debug and Release configurations build fine, so this seems to be specific to Emscripten v4.0.12

