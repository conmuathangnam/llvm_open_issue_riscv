# Cannot build blank WebGL project in any version of Unity

**Author:** shys
**URL:** https://github.com/llvm/llvm-project/issues/121807

## Body

I cannot build to WebGL in any version of Unity I've tried. I've reinstalled unity and javascript fresh. I've deleted the library folder. I've deleted the project settings folder. I've done these things while running as administrator. The project I'm using is a blank with a cube.  I'm in Windows 11.

Reproduce by build and run.

Error:

```
Building Library\Bee\artifacts\WebGL\GameAssembly\release_WebGL_wasm\3mszqx8gckyp.o failed with output:
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: "C:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/PlaybackEngines/WebGLSupport/BuildTools/Emscripten/llvm\\clang++.exe" -target wasm32-unknown-emscripten -DEMSCRIPTEN -D__EMSCRIPTEN_major__=3 -D__EMSCRIPTEN_minor__=1 -D__EMSCRIPTEN_tiny__=8 -mllvm -combiner-global-alias-analysis=false -mllvm -enable-emscripten-cxx-exceptions -mllvm -enable-emscripten-sjlj -mllvm -disable-lsr -Werror=implicit-function-declaration -Xclang -iwithsysroot/include/SDL "--sysroot=C:\\Program Files\\Unity\\Hub\\Editor\\2022.3.55f1\\Editor\\Data\\PlaybackEngines\\WebGLSupport\\BuildTools\\Emscripten\\emscripten\\cache\\sysroot" -Xclang -iwithsysroot/include\\compat -D__webgl__ -Wno-c++11-extensions -Wno-nonportable-include-path -ffunction-sections -fno-unwind-tables -fomit-frame-pointer -fno-threadsafe-statics -std=c++11 -Wno-#warnings -Wswitch -Wno-trigraphs -Wno-tautological-compare -Wno-invalid-offsetof -Wno-implicitly-unsigned-literal -Wno-integer-overflow -Wno-shift-negative-value -Wno-unknown-attributes -Wno-implicit-function-declaration -Wno-null-conversion -Wno-missing-declarations -Wno-unused-value -Wno-pragma-once-outside-header -fvisibility=hidden -fexceptions -fno-rtti -O3 -fno-strict-overflow -ffunction-sections -fdata-sections -fmessage-length=0 -pipe -D_GLIBCXX_USE_CXX11_ABI=0 -DBASELIB_INLINE_NAMESPACE=il2cpp_baselib -DIL2CPP_MONO_DEBUGGER_DISABLED -DRUNTIME_IL2CPP -DHAVE_BDWGC_GC -DNDEBUG -I. -IF:/Projects/Purrsonalities/Unity/Blank/Library/Bee/artifacts/WebGL/il2cppOutput/cpp "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp/pch" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/external/baselib/Include" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp/os/ClassLibraryPAL/brotli/include" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/external/baselib/Platforms/WebGL/Include" -fcolor-diagnostics -fdiagnostics-absolute-paths -c -xc++ F:/Projects/Purrsonalities/Unity/Blank/Library/Bee/artifacts/WebGL/il2cppOutput/cpp/System.Xml__16.cpp -o Library/Bee/artifacts/WebGL/GameAssembly/release_WebGL_wasm/3mszqx8gckyp.o
1.	<eof> parser at end of file
2.	Optimizer
 #0 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x9b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x10cc40
 #1 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x177d79 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x4a5c35
 #2 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x4a1f71 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x4a224b
 #3 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x4a7cac C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1afff90
 #4 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x87a7fa C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0xca033e
 #5 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1a85652 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x87cd93
 #6 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1092ee2 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x87d434
 #7 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x10930d2 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x87d0d3
 #8 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x108c1a2 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x108824a
 #9 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1089e01 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x2f9382e
#10 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x2108a64 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1623857
#11 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x2f92628 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1623332
#12 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x15edf45 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x168a288
#13 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x73533 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x6e467
#14 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x15064f7 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0xdc4e1d
#15 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1506ad0 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1481fbf
#16 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x148230d C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x14691be
#17 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x708bb C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x2d53484
#18 0x00007ff7e866b23f (C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe+0x9b23f)
#19 0x00007ff7e86dcc40 (C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe+0x10cc40)
0x00007FF7E866B23F (0x0000023AE65B8890 0x00007FF7E93BC235 0x0000023AE71B7850 0x0000023AE76DB100)
0x00007FF7E86DCC40 (0x0000004708789400 0x0000023AE65B9D48 0x0000004708789998 0x00000047087892D0)
0x00007FF7E8747D79 (0x0000000000000000 0x0000023AE75A6BE0 0x00000047087898D0 0x0000023AE75A6BE0)
0x00007FF7E8A75C35 (0x0000023AE5C2D000 0x0000023AE65B9CD0 0x0000023AE65B9CD0 0x0000023AE65B7188)
0x00007FF7E8A71F71 (0x0000023AE65B9CD0 0x0000004708789BF9 0x0000023AE65B7188 0x0000004708789CC0)
0x00007FF7E8A7224B (0x0000023AE76E8001 0x0000023AE5616350 0x0000023AE65B7180 0x0000023AE76B8588)
0x00007FF7E8A77CAC (0x000000470878A958 0x0000004708789CE0 0x000000470878AC38 0x0000000000000000)
0x00007FF7EA0CFF90 (0x0000023AE65B9CD0 0x000000470878A958 0x000000470878A958 0x000000470878A6A0)
0x00007FF7E8E4A7FA (0x0B0C010C0C0C0301 0x0000023AE5609EE8 0x0000023AE5C2CEA0 0x00007FF7EC0DE4C0)
0x00007FF7E927033E (0x0000000000000012 0x000000470878A010 0x000000470878A040 0x0000000000000012)
0x00007FF7EA055652 (0x0000004708789FD0 0x000000470878A958 0x0000023AE76B8588 0x0000000000000000)
0x00007FF7E8E4CD93 (0x0000023AE6F99FE8 0x000000470878A250 0x000000470878A250 0x000000470878A670)
0x00007FF7E9662EE2 (0x000000470878A210 0x0000023AE76B85C0 0x0000023AE4E31448 0x0000023AE456C0E8)
0x00007FF7E8E4D434 (0x0000023AE4E31448 0x000000470878A460 0x0000004700000000 0x0000000000000011)
0x00007FF7E96630D2 (0x000000470878A400 0x000000470878A958 0x0000023AE456C0B0 0x0000000000000000)
0x00007FF7E8E4D0D3 (0x000000470878A570 0x000000470878A8A0 0x0000023AE7C9C000 0x000000470878A6C0)
0x00007FF7E965C1A2 (0x0000000000000000 0x000000470878C060 0x0000000000000005 0x0000000000000000)
0x00007FF7E965824A (0x0000000000000000 0x0000000000000000 0x00000000000003FF 0x0000000000040FA0)
0x00007FF7E9659E01 (0x0000023AE4559500 0x0000023AE456C0E8 0x0000023AE456C0E8 0x0000000000000000)
0x00007FF7EB56382E (0x0000000000000000 0x0000023AE4559500 0x0000023AE4506940 0x0000000000000000)
0x00007FF7EA6D8A64 (0x0000023AE4503F00 0x0000000000000000 0x0000023AE4503F00 0x0000000000000000)
0x00007FF7E9BF3857 (0x0000023AE4511370 0x0000000000000000 0x00000000000003FF 0x00000000000011F5)
0x00007FF7EB562628 (0x000000470878D7B0 0x00007FF7E9BC84B4 0x0000613EED6BE8AA 0x00007FF7EB32314F)
0x00007FF7E9BF3332 (0x0000000000000000 0x000000470878D728 0x0000000400000000 0x0000023AE4511370)
0x00007FF7E9BBDF45 (0x0000023AE44C8060 0x0000000000000038 0x0000000000000000 0x0000023AE4503F00)
0x00007FF7E9C5A288 (0x0000000000000000 0x000000470878D960 0x0000023AE449AC50 0x0000000000000000)
0x00007FF7E8643533 (0x0000000000000000 0x000000470878E0D9 0x000000470878E1E0 0x0000000000000000)
0x00007FF7E863E467 (0x0000023AE44CC050 0x000000470878E1F8 0x000000470878E1E0 0x0000000000000000)
0x00007FF7E9AD64F7 (0x0000023AE44CC050 0x000000470878E1E0 0x0000023AE4508099 0x0000023AE4504530)
0x00007FF7E9394E1D (0x000000470878FD80 0x0000000000000000 0x0000023AE4480000 0x0000023AE4506720)
0x00007FF7E9AD6AD0 (0x0000023AE44CB350 0x000000470878E739 0x0000023AE4504340 0x0000000000000000)
0x00007FF7E9A51FBF (0x0000023AE4500C80 0x0000023AE4500C80 0x000000000000003E 0x0000023AE4501988)
0x00007FF7E9A5230D (0x000000470878E8B9 0x0000000000000000 0x0000023AE4501990 0x0000023AE4501990)
0x00007FF7E9A391BE (0x0000000000000000 0x0000000000000001 0x0000000000000001 0x0000000000000000)
0x00007FF7E86408BB (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00007FF7EB323484 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00007FFC9DF2E8D7 (0x0000000000000000 0x0000000000000000 0x000004F0FFFFFB30 0x000004D0FFFFFB30), BaseThreadInitThunk() + 0x17 bytes(s)
0x00007FFC9F27FBCC (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000), RtlUserThreadStart() + 0x2C bytes(s)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 14.0.1 (https://github-com-cache.bf.unity3d.com/Unity-Technologies/llvm-project c62053979489ccb002efe411c3af059addcb5d7d)
Target: wasm32-unknown-emscripten
Thread model: posix
InstalledDir: C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:\Users\Shylo\AppData\Local\Temp\System-6eb765.cpp
clang++: note: diagnostic msg: C:\Users\Shylo\AppData\Local\Temp\System-6eb765.sh
clang++: note: diagnostic msg: 
```
[System-6eb765.zip](https://github.com/user-attachments/files/18322652/System-6eb765.zip)

```
********************
emcc: error: '"C:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/PlaybackEngines/WebGLSupport/BuildTools/Emscripten/llvm\clang++.exe" -target wasm32-unknown-emscripten -DEMSCRIPTEN -D__EMSCRIPTEN_major__=3 -D__EMSCRIPTEN_minor__=1 -D__EMSCRIPTEN_tiny__=8 -mllvm -combiner-global-alias-analysis=false -mllvm -enable-emscripten-cxx-exceptions -mllvm -enable-emscripten-sjlj -mllvm -disable-lsr -Werror=implicit-function-declaration -Xclang -iwithsysroot/include/SDL "--sysroot=C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\emscripten\cache\sysroot" -Xclang -iwithsysroot/include\compat -D__webgl__ -Wno-c++11-extensions -Wno-nonportable-include-path -ffunction-sections -fno-unwind-tables -fomit-frame-pointer -fno-threadsafe-statics -std=c++11 -Wno-#warnings -Wswitch -Wno-trigraphs -Wno-tautological-compare -Wno-invalid-offsetof -Wno-implicitly-unsigned-literal -Wno-integer-overflow -Wno-shift-negative-value -Wno-unknown-attributes -Wno-implicit-function-declaration -Wno-null-conversion -Wno-missing-declarations -Wno-unused-value -Wno-pragma-once-outside-header -fvisibility=hidden -fexceptions -fno-rtti -O3 -fno-strict-overflow -ffunction-sections -fdata-sections -fmessage-length=0 -pipe -D_GLIBCXX_USE_CXX11_ABI=0 -DBASELIB_INLINE_NAMESPACE=il2cpp_baselib -DIL2CPP_MONO_DEBUGGER_DISABLED -DRUNTIME_IL2CPP -DHAVE_BDWGC_GC -DNDEBUG -I. -IF:/Projects/Purrsonalities/Unity/Blank/Library/Bee/artifacts/WebGL/il2cppOutput/cpp "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp/pch" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/external/baselib/Include" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp/os/ClassLibraryPAL/brotli/include" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/external/baselib/Platforms/WebGL/Include" -fcolor-diagnostics -fdiagnostics-absolute-paths -c -xc++ F:/Projects/Purrsonalities/Unity/Blank/Library/Bee/artifacts/WebGL/il2cppOutput/cpp/System.Xml__16.cpp -o Library/Bee/artifacts/WebGL/GameAssembly/release_WebGL_wasm/3mszqx8gckyp.o' failed (returned 1)

UnityEngine.GUIUtility:ProcessEvent (int,intptr,bool&)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Shylo Silvenar (shys)

<details>
I cannot build to WebGL in any version of Unity I've tried. I've reinstalled unity and javascript fresh. I've deleted the library folder. I've deleted the project settings folder. I've done these things while running as administrator. The project I'm using is a blank with a cube.  I'm in Windows 11.

Reproduce by build and run.

Error:

```
Building Library\Bee\artifacts\WebGL\GameAssembly\release_WebGL_wasm\3mszqx8gckyp.o failed with output:
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: "C:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/PlaybackEngines/WebGLSupport/BuildTools/Emscripten/llvm\\clang++.exe" -target wasm32-unknown-emscripten -DEMSCRIPTEN -D__EMSCRIPTEN_major__=3 -D__EMSCRIPTEN_minor__=1 -D__EMSCRIPTEN_tiny__=8 -mllvm -combiner-global-alias-analysis=false -mllvm -enable-emscripten-cxx-exceptions -mllvm -enable-emscripten-sjlj -mllvm -disable-lsr -Werror=implicit-function-declaration -Xclang -iwithsysroot/include/SDL "--sysroot=C:\\Program Files\\Unity\\Hub\\Editor\\2022.3.55f1\\Editor\\Data\\PlaybackEngines\\WebGLSupport\\BuildTools\\Emscripten\\emscripten\\cache\\sysroot" -Xclang -iwithsysroot/include\\compat -D__webgl__ -Wno-c++11-extensions -Wno-nonportable-include-path -ffunction-sections -fno-unwind-tables -fomit-frame-pointer -fno-threadsafe-statics -std=c++11 -Wno-#warnings -Wswitch -Wno-trigraphs -Wno-tautological-compare -Wno-invalid-offsetof -Wno-implicitly-unsigned-literal -Wno-integer-overflow -Wno-shift-negative-value -Wno-unknown-attributes -Wno-implicit-function-declaration -Wno-null-conversion -Wno-missing-declarations -Wno-unused-value -Wno-pragma-once-outside-header -fvisibility=hidden -fexceptions -fno-rtti -O3 -fno-strict-overflow -ffunction-sections -fdata-sections -fmessage-length=0 -pipe -D_GLIBCXX_USE_CXX11_ABI=0 -DBASELIB_INLINE_NAMESPACE=il2cpp_baselib -DIL2CPP_MONO_DEBUGGER_DISABLED -DRUNTIME_IL2CPP -DHAVE_BDWGC_GC -DNDEBUG -I. -IF:/Projects/Purrsonalities/Unity/Blank/Library/Bee/artifacts/WebGL/il2cppOutput/cpp "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp/pch" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/external/baselib/Include" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp/os/ClassLibraryPAL/brotli/include" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/external/baselib/Platforms/WebGL/Include" -fcolor-diagnostics -fdiagnostics-absolute-paths -c -xc++ F:/Projects/Purrsonalities/Unity/Blank/Library/Bee/artifacts/WebGL/il2cppOutput/cpp/System.Xml__16.cpp -o Library/Bee/artifacts/WebGL/GameAssembly/release_WebGL_wasm/3mszqx8gckyp.o
1.	&lt;eof&gt; parser at end of file
2.	Optimizer
 #<!-- -->0 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x9b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x10cc40
 #<!-- -->1 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x177d79 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x4a5c35
 #<!-- -->2 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x4a1f71 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x4a224b
 #<!-- -->3 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x4a7cac C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1afff90
 #<!-- -->4 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x87a7fa C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0xca033e
 #<!-- -->5 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1a85652 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x87cd93
 #<!-- -->6 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1092ee2 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x87d434
 #<!-- -->7 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x10930d2 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x87d0d3
 #<!-- -->8 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x108c1a2 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x108824a
 #<!-- -->9 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1089e01 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x2f9382e
#<!-- -->10 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x2108a64 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1623857
#<!-- -->11 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x2f92628 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1623332
#<!-- -->12 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x15edf45 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x168a288
#<!-- -->13 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x73533 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x6e467
#<!-- -->14 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x15064f7 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0xdc4e1d
#<!-- -->15 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1506ad0 C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x1481fbf
#<!-- -->16 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x148230d C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x14691be
#<!-- -->17 0x00007ff7e866b23f C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x708bb C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe 0x2d53484
#<!-- -->18 0x00007ff7e866b23f (C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe+0x9b23f)
#<!-- -->19 0x00007ff7e86dcc40 (C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm\clang++.exe+0x10cc40)
0x00007FF7E866B23F (0x0000023AE65B8890 0x00007FF7E93BC235 0x0000023AE71B7850 0x0000023AE76DB100)
0x00007FF7E86DCC40 (0x0000004708789400 0x0000023AE65B9D48 0x0000004708789998 0x00000047087892D0)
0x00007FF7E8747D79 (0x0000000000000000 0x0000023AE75A6BE0 0x00000047087898D0 0x0000023AE75A6BE0)
0x00007FF7E8A75C35 (0x0000023AE5C2D000 0x0000023AE65B9CD0 0x0000023AE65B9CD0 0x0000023AE65B7188)
0x00007FF7E8A71F71 (0x0000023AE65B9CD0 0x0000004708789BF9 0x0000023AE65B7188 0x0000004708789CC0)
0x00007FF7E8A7224B (0x0000023AE76E8001 0x0000023AE5616350 0x0000023AE65B7180 0x0000023AE76B8588)
0x00007FF7E8A77CAC (0x000000470878A958 0x0000004708789CE0 0x000000470878AC38 0x0000000000000000)
0x00007FF7EA0CFF90 (0x0000023AE65B9CD0 0x000000470878A958 0x000000470878A958 0x000000470878A6A0)
0x00007FF7E8E4A7FA (0x0B0C010C0C0C0301 0x0000023AE5609EE8 0x0000023AE5C2CEA0 0x00007FF7EC0DE4C0)
0x00007FF7E927033E (0x0000000000000012 0x000000470878A010 0x000000470878A040 0x0000000000000012)
0x00007FF7EA055652 (0x0000004708789FD0 0x000000470878A958 0x0000023AE76B8588 0x0000000000000000)
0x00007FF7E8E4CD93 (0x0000023AE6F99FE8 0x000000470878A250 0x000000470878A250 0x000000470878A670)
0x00007FF7E9662EE2 (0x000000470878A210 0x0000023AE76B85C0 0x0000023AE4E31448 0x0000023AE456C0E8)
0x00007FF7E8E4D434 (0x0000023AE4E31448 0x000000470878A460 0x0000004700000000 0x0000000000000011)
0x00007FF7E96630D2 (0x000000470878A400 0x000000470878A958 0x0000023AE456C0B0 0x0000000000000000)
0x00007FF7E8E4D0D3 (0x000000470878A570 0x000000470878A8A0 0x0000023AE7C9C000 0x000000470878A6C0)
0x00007FF7E965C1A2 (0x0000000000000000 0x000000470878C060 0x0000000000000005 0x0000000000000000)
0x00007FF7E965824A (0x0000000000000000 0x0000000000000000 0x00000000000003FF 0x0000000000040FA0)
0x00007FF7E9659E01 (0x0000023AE4559500 0x0000023AE456C0E8 0x0000023AE456C0E8 0x0000000000000000)
0x00007FF7EB56382E (0x0000000000000000 0x0000023AE4559500 0x0000023AE4506940 0x0000000000000000)
0x00007FF7EA6D8A64 (0x0000023AE4503F00 0x0000000000000000 0x0000023AE4503F00 0x0000000000000000)
0x00007FF7E9BF3857 (0x0000023AE4511370 0x0000000000000000 0x00000000000003FF 0x00000000000011F5)
0x00007FF7EB562628 (0x000000470878D7B0 0x00007FF7E9BC84B4 0x0000613EED6BE8AA 0x00007FF7EB32314F)
0x00007FF7E9BF3332 (0x0000000000000000 0x000000470878D728 0x0000000400000000 0x0000023AE4511370)
0x00007FF7E9BBDF45 (0x0000023AE44C8060 0x0000000000000038 0x0000000000000000 0x0000023AE4503F00)
0x00007FF7E9C5A288 (0x0000000000000000 0x000000470878D960 0x0000023AE449AC50 0x0000000000000000)
0x00007FF7E8643533 (0x0000000000000000 0x000000470878E0D9 0x000000470878E1E0 0x0000000000000000)
0x00007FF7E863E467 (0x0000023AE44CC050 0x000000470878E1F8 0x000000470878E1E0 0x0000000000000000)
0x00007FF7E9AD64F7 (0x0000023AE44CC050 0x000000470878E1E0 0x0000023AE4508099 0x0000023AE4504530)
0x00007FF7E9394E1D (0x000000470878FD80 0x0000000000000000 0x0000023AE4480000 0x0000023AE4506720)
0x00007FF7E9AD6AD0 (0x0000023AE44CB350 0x000000470878E739 0x0000023AE4504340 0x0000000000000000)
0x00007FF7E9A51FBF (0x0000023AE4500C80 0x0000023AE4500C80 0x000000000000003E 0x0000023AE4501988)
0x00007FF7E9A5230D (0x000000470878E8B9 0x0000000000000000 0x0000023AE4501990 0x0000023AE4501990)
0x00007FF7E9A391BE (0x0000000000000000 0x0000000000000001 0x0000000000000001 0x0000000000000000)
0x00007FF7E86408BB (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00007FF7EB323484 (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000)
0x00007FFC9DF2E8D7 (0x0000000000000000 0x0000000000000000 0x000004F0FFFFFB30 0x000004D0FFFFFB30), BaseThreadInitThunk() + 0x17 bytes(s)
0x00007FFC9F27FBCC (0x0000000000000000 0x0000000000000000 0x0000000000000000 0x0000000000000000), RtlUserThreadStart() + 0x2C bytes(s)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 14.0.1 (https://github-com-cache.bf.unity3d.com/Unity-Technologies/llvm-project c62053979489ccb002efe411c3af059addcb5d7d)
Target: wasm32-unknown-emscripten
Thread model: posix
InstalledDir: C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\llvm
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:\Users\Shylo\AppData\Local\Temp\System-6eb765.cpp
clang++: note: diagnostic msg: C:\Users\Shylo\AppData\Local\Temp\System-6eb765.sh
clang++: note: diagnostic msg: 
```
[System-6eb765.zip](https://github.com/user-attachments/files/18322652/System-6eb765.zip)

```
********************
emcc: error: '"C:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/PlaybackEngines/WebGLSupport/BuildTools/Emscripten/llvm\clang++.exe" -target wasm32-unknown-emscripten -DEMSCRIPTEN -D__EMSCRIPTEN_major__=3 -D__EMSCRIPTEN_minor__=1 -D__EMSCRIPTEN_tiny__=8 -mllvm -combiner-global-alias-analysis=false -mllvm -enable-emscripten-cxx-exceptions -mllvm -enable-emscripten-sjlj -mllvm -disable-lsr -Werror=implicit-function-declaration -Xclang -iwithsysroot/include/SDL "--sysroot=C:\Program Files\Unity\Hub\Editor\2022.3.55f1\Editor\Data\PlaybackEngines\WebGLSupport\BuildTools\Emscripten\emscripten\cache\sysroot" -Xclang -iwithsysroot/include\compat -D__webgl__ -Wno-c++11-extensions -Wno-nonportable-include-path -ffunction-sections -fno-unwind-tables -fomit-frame-pointer -fno-threadsafe-statics -std=c++11 -Wno-#warnings -Wswitch -Wno-trigraphs -Wno-tautological-compare -Wno-invalid-offsetof -Wno-implicitly-unsigned-literal -Wno-integer-overflow -Wno-shift-negative-value -Wno-unknown-attributes -Wno-implicit-function-declaration -Wno-null-conversion -Wno-missing-declarations -Wno-unused-value -Wno-pragma-once-outside-header -fvisibility=hidden -fexceptions -fno-rtti -O3 -fno-strict-overflow -ffunction-sections -fdata-sections -fmessage-length=0 -pipe -D_GLIBCXX_USE_CXX11_ABI=0 -DBASELIB_INLINE_NAMESPACE=il2cpp_baselib -DIL2CPP_MONO_DEBUGGER_DISABLED -DRUNTIME_IL2CPP -DHAVE_BDWGC_GC -DNDEBUG -I. -IF:/Projects/Purrsonalities/Unity/Blank/Library/Bee/artifacts/WebGL/il2cppOutput/cpp "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp/pch" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/external/baselib/Include" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/libil2cpp/os/ClassLibraryPAL/brotli/include" "-IC:/Program Files/Unity/Hub/Editor/2022.3.55f1/Editor/Data/il2cpp/external/baselib/Platforms/WebGL/Include" -fcolor-diagnostics -fdiagnostics-absolute-paths -c -xc++ F:/Projects/Purrsonalities/Unity/Blank/Library/Bee/artifacts/WebGL/il2cppOutput/cpp/System.Xml__16.cpp -o Library/Bee/artifacts/WebGL/GameAssembly/release_WebGL_wasm/3mszqx8gckyp.o' failed (returned 1)

UnityEngine.GUIUtility:ProcessEvent (int,intptr,bool&amp;)
```
</details>


---

### Comment 2 - frederick-vs-ja

Is such crash still reproducible with newer versions (19, 20) of Clang?

---

### Comment 3 - frederick-vs-ja

Looks similar to #75906, possibly a duplicate.

---

### Comment 4 - shys

> @llvm/issue-subscribers-clang-frontend
> 
> Author: Shylo Silvenar (shys)

I updated my bios after reading #75906 and it fixed the problem. Thank you.

---

