# clang-cl crash [Exception Code: 0xC0000005]

**Author:** Rinn
**URL:** https://github.com/llvm/llvm-project/issues/167433
**Status:** Closed
**Labels:** clang:frontend, crash, regression:21
**Closed Date:** 2026-03-20T23:30:56Z

## Body

Getting a crash with clang-cl with versions 21.1.0 to 22.1.1 (most recent tested), does not occur with 20.1.8

[FortActivityBrowserListView-33d686.zip](https://github.com/user-attachments/files/23467229/FortActivityBrowserListView-33d686.zip)

```
>E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe -v @FortActivityBrowserListView.cpp.i.rsp
clang version 21.1.5
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin
 (in-process)
 "E:\\AutoSDK\\HostWin64\\Win64\\LLVM\\21.1.5\\bin\\clang-cl.exe" -cc1 -triple x86_64-pc-windows-msvc19.44.0 -O3 -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name FortActivityBrowserListView.cpp.i -mrelocation-model pic -pic-level 2 -fno-delete-null-pointer-checks -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -complex-range=full -mconstructor-aliases -fms-volatile -funwind-tables=2 -target-cpu x86-64 -target-feature +sse4.2 -target-feature +waitpkg -mllvm -x86-asm-syntax=intel -tune-cpu generic -D_MT -D_DLL --dependent-lib=msvcrt --dependent-lib=oldnames -fno-rtti-data -stack-protector 2 -fdiagnostics-format msvc -gno-column-info -gcodeview -debug-info-kind=unused-types "-fdebug-compilation-dir=D:\\tmp" "-object-file-name=D:\\tmp\\FortActivityBrowserListView.cpp.obj" -v -ffunction-sections -fdata-sections "-fcoverage-compilation-dir=D:\\tmp" -resource-dir E:/AutoSDK/HostWin64/Win64/LLVM/21.1.5/lib/clang/21 -D SAL_NO_ATTRIBUTE_DECLARATIONS=1 -D _CRT_SECURE_NO_WARNINGS -D _DISABLE_EXTENDED_ALIGNED_STORAGE -D _HAS_EXCEPTIONS=0 -D PLATFORM_EXCEPTIONS_DISABLED=1 -internal-isystem "E:/AutoSDK/HostWin64/Win64/LLVM/21.1.5/lib/clang/21\\include" -internal-isystem "undefined\\include" -internal-isystem "undefined\\atlmfc\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\cppwinrt" -Werror -WCL4 -Werror -Werror -Wshadow -Wno-dllexport-explicit-instantiation-decl -Wno-undefined-var-template -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-unused-function -Wno-unused-lambda-capture -Wno-unused-local-typedef -Wno-unused-private-field -Wno-unused-variable -Wno-unknown-pragmas -Wno-tautological-compare -Wno-switch -Wno-invalid-offsetof -Wno-inconsistent-missing-override -Wno-gnu-string-literal-operator-template -Wno-missing-template-arg-list-after-template-kw -Wno-cast-function-type-mismatch -Wno-extra-qualification -Wno-nontrivial-memaccess -Wno-nan-infinity-disabled -Wno-deprecated-copy -Wno-bitwise-instead-of-logical -Wno-ordered-compare-function-pointers -Wno-deprecated-volatile -Wno-deprecated-anon-enum-enum-conversion -Wno-ambiguous-reversed-operator -Wno-enum-float-conversion -Wno-enum-enum-conversion -Wundef -Wno-float-conversion -Wno-implicit-float-conversion -Wno-implicit-int-conversion -Wno-c++11-narrowing -Wno-invalid-unevaluated-string -Wdeprecated-declarations -Wno-error=deprecated-declarations -Wno-deprecated-copy-with-user-provided-copy -Wno-shorten-64-to-32 -Wno-unused-value -Wunused-result -Wno-switch-enum -Wno-dangling -Wno-null-pointer-subtraction -Wno-bitfield-enum-conversion -Wno-constant-logical-operand -Wno-null-pointer-arithmetic -Wno-invalid-token-paste -Wno-nonportable-include-path -Wno-missing-field-initializers -Wno-sign-compare -Wno-expansion-to-defined -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-return-type -Wno-uninitialized -Wno-ignored-attributes -Wno-microsoft -Wno-msvc-include -Wno-pragma-pack -Wno-deprecated-declarations -Wno-inline-new-delete -Wno-undefined-bool-conversion -Wno-deprecated-writable-strings -Wno-deprecated-register -Wno-logical-op-parentheses -Wno-null-arithmetic -Wno-return-type-c-linkage -Wno-implicit-exception-spec-mismatch -fdeprecated-macro -ferror-limit 19 -fmessage-length=280 -fno-temp-file -fno-use-cxa-atexit -fms-extensions -fms-compatibility -fms-compatibility-version=19.44 -std=c++20 -finline-functions -fno-implicit-modules -fskip-odr-check-in-gmf -fpack-struct=8 -fcolor-diagnostics -fdiagnostics-absolute-paths -vectorize-loops -vectorize-slp -x c++ -gcodeview-ghash -faddrsig -o FortActivityBrowserListView.cpp.obj -x c++ FortActivityBrowserListView.cpp.i
clang -cc1 version 21.1.5 based upon LLVM 21.1.5 default target x86_64-pc-windows-msvc
ignoring nonexistent directory "undefined\include"
ignoring nonexistent directory "undefined\atlmfc\include"
#include "..." search starts here:
#include <...> search starts here:
 E:/AutoSDK/HostWin64/Win64/LLVM/21.1.5/lib/clang/21\include
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\winrt
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\cppwinrt
End of search list.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: E:\\AutoSDK\\HostWin64\\Win64\\LLVM\\21.1.5\\bin\\clang-cl.exe -v @FortActivityBrowserListView.cpp.i.rsp
1.      <eof> parser at end of file
2.      Per-file LLVM IR generation
3.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:316:45: Generating code for declaration 'SBrowserListView<TSharedRef<FFortActivityCategory, ESPMode::ThreadSafe>>::ReGenerateItems'
4.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:321:3: LLVM IR generation of compound statement ('{}')
5.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:322:4: LLVM IR generation of compound statement ('{}')
6.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:322:366: LLVM IR generation of compound statement ('{}')
Exception Code: 0xC0000005
 #0 0x00007ff67cd344cd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x16844cd)
 #1 0x00007ff67cad1fa5 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1421fa5)
 #2 0x00007ff67cad1cf8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1421cf8)
 #3 0x00007ff67d11c9c4 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1a6c9c4)
 #4 0x00007ff67c8ca8a2 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121a8a2)
 #5 0x00007ff67b6c0c6d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x10c6d)
 #6 0x00007ff67b6beefb (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0xeefb)
 #7 0x00007ff67c8c28c9 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x12128c9)
 #8 0x00007ff67c8c03ae (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x12103ae)
 #9 0x00007ff67c8c031b (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121031b)
#10 0x00007ff67b6d59f6 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x259f6)
#11 0x00007ff67c8c0497 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1210497)
#12 0x00007ff67c8c031b (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121031b)
#13 0x00007ff67c8c031b (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121031b)
#14 0x00007ff67b6c26ba (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x126ba)
#15 0x00007ff67c8c03d8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x12103d8)
#16 0x00007ff67c8bfefe (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120fefe)
#17 0x00007ff67c8be515 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120e515)
#18 0x00007ff67c8bbf4e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120bf4e)
#19 0x00007ff67c8bb373 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120b373)
#20 0x00007ff67c8baedd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120aedd)
#21 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#22 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#23 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#24 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#25 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#26 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#27 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#28 0x00007ff67bf1432e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x86432e)
#29 0x00007ff67bf1427d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x86427d)
#30 0x00007ff67d0808cd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19d08cd)
#31 0x00007ff67d5e9e85 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1f39e85)
#32 0x00007ff67bcda493 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a493)
#33 0x00007ff67bcda0dd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a0dd)
#34 0x00007ff67bcd6d96 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x626d96)
#35 0x00007ff67bcd38f8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x6238f8)
#36 0x00007ff67bcd218e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62218e)
#37 0x00007ff67c5bee5d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0xf0ee5d)
#38 0x00007ff67be62685 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2685)
#39 0x00007ff67be62478 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2478)
#40 0x00007ff67bb87825 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d7825)
#41 0x00007ff67bb8716e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d716e)
#42 0x00007ff67bb86d4d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d6d4d)
#43 0x00007ff67bb7da86 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cda86)
#44 0x00007ff67bb7b023 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cb023)
#45 0x00007ff67d69d0a0 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1fed0a0)
#46 0x00007ff98981e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#47 0x00007ff98a26c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.5
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin
clang-cl: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-cl: note: diagnostic msg: C:\Users\JOE~1.KIR\AppData\Local\Temp\FortActivityBrowserListView-33d686.cpp
clang-cl: note: diagnostic msg: C:\Users\JOE~1.KIR\AppData\Local\Temp\FortActivityBrowserListView-33d686.sh
clang-cl: note: diagnostic msg:

********************
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Joe Kirchoff (Rinn)

<details>
Getting a crash with clang-cl with versions 21.1.0 to 21.1.5, does not occur with 20.1.8

[FortActivityBrowserListView-33d686.zip](https://github.com/user-attachments/files/23467229/FortActivityBrowserListView-33d686.zip)

```
&gt;E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe -v @<!-- -->FortActivityBrowserListView.cpp.i.rsp
clang version 21.1.5
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin
 (in-process)
 "E:\\AutoSDK\\HostWin64\\Win64\\LLVM\\21.1.5\\bin\\clang-cl.exe" -cc1 -triple x86_64-pc-windows-msvc19.44.0 -O3 -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name FortActivityBrowserListView.cpp.i -mrelocation-model pic -pic-level 2 -fno-delete-null-pointer-checks -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -complex-range=full -mconstructor-aliases -fms-volatile -funwind-tables=2 -target-cpu x86-64 -target-feature +sse4.2 -target-feature +waitpkg -mllvm -x86-asm-syntax=intel -tune-cpu generic -D_MT -D_DLL --dependent-lib=msvcrt --dependent-lib=oldnames -fno-rtti-data -stack-protector 2 -fdiagnostics-format msvc -gno-column-info -gcodeview -debug-info-kind=unused-types "-fdebug-compilation-dir=D:\\tmp" "-object-file-name=D:\\tmp\\FortActivityBrowserListView.cpp.obj" -v -ffunction-sections -fdata-sections "-fcoverage-compilation-dir=D:\\tmp" -resource-dir E:/AutoSDK/HostWin64/Win64/LLVM/21.1.5/lib/clang/21 -D SAL_NO_ATTRIBUTE_DECLARATIONS=1 -D _CRT_SECURE_NO_WARNINGS -D _DISABLE_EXTENDED_ALIGNED_STORAGE -D _HAS_EXCEPTIONS=0 -D PLATFORM_EXCEPTIONS_DISABLED=1 -internal-isystem "E:/AutoSDK/HostWin64/Win64/LLVM/21.1.5/lib/clang/21\\include" -internal-isystem "undefined\\include" -internal-isystem "undefined\\atlmfc\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\cppwinrt" -Werror -WCL4 -Werror -Werror -Wshadow -Wno-dllexport-explicit-instantiation-decl -Wno-undefined-var-template -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-unused-function -Wno-unused-lambda-capture -Wno-unused-local-typedef -Wno-unused-private-field -Wno-unused-variable -Wno-unknown-pragmas -Wno-tautological-compare -Wno-switch -Wno-invalid-offsetof -Wno-inconsistent-missing-override -Wno-gnu-string-literal-operator-template -Wno-missing-template-arg-list-after-template-kw -Wno-cast-function-type-mismatch -Wno-extra-qualification -Wno-nontrivial-memaccess -Wno-nan-infinity-disabled -Wno-deprecated-copy -Wno-bitwise-instead-of-logical -Wno-ordered-compare-function-pointers -Wno-deprecated-volatile -Wno-deprecated-anon-enum-enum-conversion -Wno-ambiguous-reversed-operator -Wno-enum-float-conversion -Wno-enum-enum-conversion -Wundef -Wno-float-conversion -Wno-implicit-float-conversion -Wno-implicit-int-conversion -Wno-c++11-narrowing -Wno-invalid-unevaluated-string -Wdeprecated-declarations -Wno-error=deprecated-declarations -Wno-deprecated-copy-with-user-provided-copy -Wno-shorten-64-to-32 -Wno-unused-value -Wunused-result -Wno-switch-enum -Wno-dangling -Wno-null-pointer-subtraction -Wno-bitfield-enum-conversion -Wno-constant-logical-operand -Wno-null-pointer-arithmetic -Wno-invalid-token-paste -Wno-nonportable-include-path -Wno-missing-field-initializers -Wno-sign-compare -Wno-expansion-to-defined -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-return-type -Wno-uninitialized -Wno-ignored-attributes -Wno-microsoft -Wno-msvc-include -Wno-pragma-pack -Wno-deprecated-declarations -Wno-inline-new-delete -Wno-undefined-bool-conversion -Wno-deprecated-writable-strings -Wno-deprecated-register -Wno-logical-op-parentheses -Wno-null-arithmetic -Wno-return-type-c-linkage -Wno-implicit-exception-spec-mismatch -fdeprecated-macro -ferror-limit 19 -fmessage-length=280 -fno-temp-file -fno-use-cxa-atexit -fms-extensions -fms-compatibility -fms-compatibility-version=19.44 -std=c++20 -finline-functions -fno-implicit-modules -fskip-odr-check-in-gmf -fpack-struct=8 -fcolor-diagnostics -fdiagnostics-absolute-paths -vectorize-loops -vectorize-slp -x c++ -gcodeview-ghash -faddrsig -o FortActivityBrowserListView.cpp.obj -x c++ FortActivityBrowserListView.cpp.i
clang -cc1 version 21.1.5 based upon LLVM 21.1.5 default target x86_64-pc-windows-msvc
ignoring nonexistent directory "undefined\include"
ignoring nonexistent directory "undefined\atlmfc\include"
#include "..." search starts here:
#include &lt;...&gt; search starts here:
 E:/AutoSDK/HostWin64/Win64/LLVM/21.1.5/lib/clang/21\include
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\winrt
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\cppwinrt
End of search list.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: E:\\AutoSDK\\HostWin64\\Win64\\LLVM\\21.1.5\\bin\\clang-cl.exe -v @<!-- -->FortActivityBrowserListView.cpp.i.rsp
1.      &lt;eof&gt; parser at end of file
2.      Per-file LLVM IR generation
3.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:316:45: Generating code for declaration 'SBrowserListView&lt;TSharedRef&lt;FFortActivityCategory, ESPMode::ThreadSafe&gt;&gt;::ReGenerateItems'
4.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:321:3: LLVM IR generation of compound statement ('{}')
5.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:322:4: LLVM IR generation of compound statement ('{}')
6.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:322:366: LLVM IR generation of compound statement ('{}')
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ff67cd344cd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x16844cd)
 #<!-- -->1 0x00007ff67cad1fa5 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1421fa5)
 #<!-- -->2 0x00007ff67cad1cf8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1421cf8)
 #<!-- -->3 0x00007ff67d11c9c4 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1a6c9c4)
 #<!-- -->4 0x00007ff67c8ca8a2 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121a8a2)
 #<!-- -->5 0x00007ff67b6c0c6d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x10c6d)
 #<!-- -->6 0x00007ff67b6beefb (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0xeefb)
 #<!-- -->7 0x00007ff67c8c28c9 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x12128c9)
 #<!-- -->8 0x00007ff67c8c03ae (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x12103ae)
 #<!-- -->9 0x00007ff67c8c031b (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121031b)
#<!-- -->10 0x00007ff67b6d59f6 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x259f6)
#<!-- -->11 0x00007ff67c8c0497 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1210497)
#<!-- -->12 0x00007ff67c8c031b (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121031b)
#<!-- -->13 0x00007ff67c8c031b (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121031b)
#<!-- -->14 0x00007ff67b6c26ba (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x126ba)
#<!-- -->15 0x00007ff67c8c03d8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x12103d8)
#<!-- -->16 0x00007ff67c8bfefe (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120fefe)
#<!-- -->17 0x00007ff67c8be515 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120e515)
#<!-- -->18 0x00007ff67c8bbf4e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120bf4e)
#<!-- -->19 0x00007ff67c8bb373 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120b373)
#<!-- -->20 0x00007ff67c8baedd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120aedd)
#<!-- -->21 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->22 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->23 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->24 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->25 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->26 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->27 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->28 0x00007ff67bf1432e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x86432e)
#<!-- -->29 0x00007ff67bf1427d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x86427d)
#<!-- -->30 0x00007ff67d0808cd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19d08cd)
#<!-- -->31 0x00007ff67d5e9e85 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1f39e85)
#<!-- -->32 0x00007ff67bcda493 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a493)
#<!-- -->33 0x00007ff67bcda0dd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a0dd)
#<!-- -->34 0x00007ff67bcd6d96 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x626d96)
#<!-- -->35 0x00007ff67bcd38f8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x6238f8)
#<!-- -->36 0x00007ff67bcd218e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62218e)
#<!-- -->37 0x00007ff67c5bee5d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0xf0ee5d)
#<!-- -->38 0x00007ff67be62685 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2685)
#<!-- -->39 0x00007ff67be62478 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2478)
#<!-- -->40 0x00007ff67bb87825 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d7825)
#<!-- -->41 0x00007ff67bb8716e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d716e)
#<!-- -->42 0x00007ff67bb86d4d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d6d4d)
#<!-- -->43 0x00007ff67bb7da86 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cda86)
#<!-- -->44 0x00007ff67bb7b023 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cb023)
#<!-- -->45 0x00007ff67d69d0a0 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1fed0a0)
#<!-- -->46 0x00007ff98981e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#<!-- -->47 0x00007ff98a26c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.5
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin
clang-cl: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-cl: note: diagnostic msg: C:\Users\JOE~1.KIR\AppData\Local\Temp\FortActivityBrowserListView-33d686.cpp
clang-cl: note: diagnostic msg: C:\Users\JOE~1.KIR\AppData\Local\Temp\FortActivityBrowserListView-33d686.sh
clang-cl: note: diagnostic msg:

********************
```
</details>


---

### Comment 2 - Rinn

I also got another crash with the same exception code, unsure if it's related or not as the stack dump looks different. If not I can make a new issue, but same as before this crash does not occur with 20.1.8 but does with 21.1.0 and later.

[FieldOfViewNetObjectPrioritizer-8aa4e8.zip](https://github.com/user-attachments/files/23511878/FieldOfViewNetObjectPrioritizer-8aa4e8.zip)

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0. Program arguments: E:\\AutoSDK\\HostWin64\\Win64\\LLVM\\21.1.5\\bin\\clang-cl.exe @../Intermediate/Build/XB1/x64/UnrealGameNU/Development/IrisCore/FieldOfViewNetObjectPrioritizer.cpp.obj.rsp
1. <eof> parser at end of file
2. Code generation
3. Running pass 'Function Pass Manager' on module 'Runtime/Net/Iris/Private/Iris/ReplicationSystem/Prioritization/FieldOfViewNetObjectPrioritizer.cpp'.
4. Running pass 'X86 DAG->DAG Instruction Selection' on function '@"?PrioritizeBatch@UFieldOfViewNetObjectPrioritizer@@IEAAXAEAUFBatchParams@1@@Z"'
Exception Code: 0xC0000005
 #0 0x00007ff6f64211fe (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x15311fe)
 #1 0x00007ff6f6420db8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1530db8)
 #2 0x00007ff6f689389c (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19a389c)
 #3 0x00007ff6f688e332 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x199e332)
 #4 0x00007ff6f69c67ef (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1ad67ef)
 #5 0x00007ff6f69c2dcc (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1ad2dcc)
 #6 0x00007ff6f693a8a9 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1a4a8a9)
 #7 0x00007ff6f51c1de9 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x2d1de9)
 #8 0x00007ff6f54288d4 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x5388d4)
 #9 0x00007ff6f68a536e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19b536e)
#10 0x00007ff6f68c61d0 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19d61d0)
#11 0x00007ff6f5186e0a (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x296e0a)
#12 0x00007ff6f51868de (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x2968de)
#13 0x00007ff6f518041e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x29041e)
#14 0x00007ff6f68c0dda (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19d0dda)
#15 0x00007ff6f6e29e85 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1f39e85)
#16 0x00007ff6f551a493 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a493)
#17 0x00007ff6f551a0dd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a0dd)
#18 0x00007ff6f5516d96 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x626d96)
#19 0x00007ff6f55138f8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x6238f8)
#20 0x00007ff6f551218e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62218e)
#21 0x00007ff6f5dfee5d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0xf0ee5d)
#22 0x00007ff6f56a2685 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2685)
#23 0x00007ff6f56a2478 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2478)
#24 0x00007ff6f53c7825 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d7825)
#25 0x00007ff6f53c716e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d716e)
#26 0x00007ff6f53c6d4d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d6d4d)
#27 0x00007ff6f53bda86 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cda86)
#28 0x00007ff6f53bb023 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cb023)
#29 0x00007ff6f6edd0a0 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1fed0a0)
#30 0x00007ffc7f4ee8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#31 0x00007ffc8000c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.5
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin
clang-cl: note: diagnostic msg:
********************
PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-cl: note: diagnostic msg: C:\Users\joe.kirchoff\AppData\Local\Temp\UnrealBuildTool\3f3aa673\FieldOfViewNetObjectPrioritizer-8aa4e8.cpp
clang-cl: note: diagnostic msg: C:\Users\joe.kirchoff\AppData\Local\Temp\UnrealBuildTool\3f3aa673\FieldOfViewNetObjectPrioritizer-8aa4e8.sh
clang-cl: note: diagnostic msg:
********************
```

---

### Comment 3 - EugeneZelenko

It's different problem, see lines below `Stack dump:`.

---

### Comment 4 - Rinn

I spent some time getting a build of cvise working and believe I have a reduced repro for this

```c++
namespace UE::Core::Private {
struct FResult {};
} // namespace UE::Core::Private
enum ESPMode { ThreadSafe };
template <int, typename ObjectType> void NewIntrusiveReferenceController() {
  new ObjectType;
}
template <typename InObjectType, ESPMode InMode = ThreadSafe>
void MakeShared() {
  NewIntrusiveReferenceController<InMode, InObjectType>;
}
struct source_location {
  static source_location current(const char * = __builtin_FUNCSIG());
};
namespace UE {
struct FSourceLocation {
  static FSourceLocation Current(source_location = source_location::current());
};
} // namespace UE
int MakeRequiredArgs, MakeTDecl_OnLine;
template <typename> class SBrowserListView;
template <typename RequiredArgsPayloadType> struct TSlateDecl {
  TSlateDecl(char *, char *, int, RequiredArgsPayloadType) {
    MakeShared<SBrowserListView<int>>;
  }
};
char MakeTDecl_InFile;
struct STableViewBase {
  virtual int ReGenerateItems(const int &);
};
template <template <typename> class> void ConstructListView() {
  char InType;
  TSlateDecl(&InType, &MakeTDecl_InFile, MakeTDecl_OnLine, MakeRequiredArgs);
}
namespace Fort {
struct FSourceLocation {
  static void Current(UE::FSourceLocation = UE::FSourceLocation::Current());
};
} // namespace Fort
template <typename> struct SBrowserListView : STableViewBase {
  int ReGenerateItems(const int &) {
    {
      namespace UCFS = UE::Core::Private;
      using UCFS_FChecker = decltype(Fort::FSourceLocation::Current());
      UCFS::FResult UCFS_Result = UCFS_FChecker::Check0;
    }
  }
};
void ListItemsRebuildListWidget() { ConstructListView<SBrowserListView>; }
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.                                                                                                                                     
Stack dump:                                                                                                                                                                                                                                                                                     
0.      Program arguments: clang-cl.exe -cc1 -triple x86_64-pc-windows-msvc19.44.0 -O3 -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name FortActivityBrowserListView.cpp.i -mrelocation-model pic -pic-level 2 -fno-delete-null-pointer-checks -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -complex-range=full -mconstructor-aliases -fms-volatile -funwind-tables=2 -target-cpu x86-64 -target-feature +sse4.2 -target-feature +waitpkg -mllvm -x86-asm-syntax=intel -tune-cpu generic -D_MT -D_DLL --dependent-lib=msvcrt --dependent-lib=oldnames -fno-rtti-data -stack-protector 2 -fdiagnostics-format msvc -gno-column-info -gcodeview -debug-info-kind=unused-types -ffunction-sections -fdata-sections -fdeprecated-macro -ferror-limit 19 -fmessage-length=280 -fno-temp-file -fno-use-cxa-atexit -fms-extensions -fms-compatibility -fms-compatibility-version=19.44 -std=c++20 -finline-functions -fno-implicit-modules -fskip-odr-check-in-gmf -fpack-struct=8 -fcolor-diagnostics -fdiagnostics-absolute-paths -vectorize-loops -vectorize-slp -gcodeview-ghash -faddrsig -x c++ -w -Wfatal-errors -Wno-mismatched-tags reduced.cpp                                                                                                                                                                                                                                        
1.      <eof> parser at end of file                                                                                                                                                                                                                                                             
2.      Per-file LLVM IR generation                                                                                                                                                                                                                                                             
3.      reduced.cpp:41:7: Generating code for declaration 'SBrowserListView<int>::ReGenerateItems'                                                                                                                                                                                              
4.      reduced.cpp:42:5: LLVM IR generation of compound statement ('{}')                                                                                                                                                                                                                       
Exception Code: 0xC0000005
 #0 0x00007ff6ab054cea (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x1684cea)
 #1 0x00007ff6aadf1745 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x1421745)
 #2 0x00007ff6aadf1498 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x1421498)
 #3 0x00007ff6ab43d224 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x1a6d224)
 #4 0x00007ff6aabea052 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x121a052)
 #5 0x00007ff6a99e0c6d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x10c6d)
 #6 0x00007ff6a99deefb (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0xeefb)
 #7 0x00007ff6aabe2079 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x1212079)
 #8 0x00007ff6aabdfb5e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120fb5e)
 #9 0x00007ff6aabdfacb (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120facb)
#10 0x00007ff6aabdf6ae (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120f6ae)
#11 0x00007ff6aabddcc5 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120dcc5)
#12 0x00007ff6aabdb6fe (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120b6fe)
#13 0x00007ff6aabdab23 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120ab23)
#14 0x00007ff6aabda68d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120a68d)
#15 0x00007ff6aabda6b5 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120a6b5)
#16 0x00007ff6aabda6b5 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120a6b5)
#17 0x00007ff6aabda6b5 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120a6b5)
#18 0x00007ff6aabda6b5 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120a6b5)
#19 0x00007ff6aabda6b5 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x120a6b5)
#20 0x00007ff6aa23364e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x86364e)
#21 0x00007ff6aa23359d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x86359d)
#22 0x00007ff6ab3a110d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x19d110d)
#23 0x00007ff6ab8f6a55 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x1f26a55)
#24 0x00007ff6a9ff8ce3 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x628ce3)
#25 0x00007ff6a9ff892d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x62892d)
#26 0x00007ff6a9ff55e6 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x6255e6)
#27 0x00007ff6a9ff2148 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x622148)
#28 0x00007ff6a9ff09de (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x6209de)
#29 0x00007ff6a9e9bb5a (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x4cbb5a)
#30 0x00007ff6a9e98413 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x4c8413)
#31 0x00007ff6ab9bc470 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.8\bin\clang-cl.exe+0x1fec470)
#32 0x00007ffafee3e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#33 0x00007ffb006ac53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
```

---

### Comment 5 - EugeneZelenko

Could you please try 22 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

---

### Comment 6 - Rinn

The crash still occurs with 22.1.1 I just checked.

---

### Comment 7 - Rinn

I was able to reduce the repro even more and I made a PR to fix this #187755 

---

### Comment 8 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Joe Kirchoff (Rinn)

<details>
Getting a crash with clang-cl with versions 21.1.0 to 22.1.1 (most recent tested), does not occur with 20.1.8

[FortActivityBrowserListView-33d686.zip](https://github.com/user-attachments/files/23467229/FortActivityBrowserListView-33d686.zip)

```
&gt;E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe -v @<!-- -->FortActivityBrowserListView.cpp.i.rsp
clang version 21.1.5
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin
 (in-process)
 "E:\\AutoSDK\\HostWin64\\Win64\\LLVM\\21.1.5\\bin\\clang-cl.exe" -cc1 -triple x86_64-pc-windows-msvc19.44.0 -O3 -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name FortActivityBrowserListView.cpp.i -mrelocation-model pic -pic-level 2 -fno-delete-null-pointer-checks -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -complex-range=full -mconstructor-aliases -fms-volatile -funwind-tables=2 -target-cpu x86-64 -target-feature +sse4.2 -target-feature +waitpkg -mllvm -x86-asm-syntax=intel -tune-cpu generic -D_MT -D_DLL --dependent-lib=msvcrt --dependent-lib=oldnames -fno-rtti-data -stack-protector 2 -fdiagnostics-format msvc -gno-column-info -gcodeview -debug-info-kind=unused-types "-fdebug-compilation-dir=D:\\tmp" "-object-file-name=D:\\tmp\\FortActivityBrowserListView.cpp.obj" -v -ffunction-sections -fdata-sections "-fcoverage-compilation-dir=D:\\tmp" -resource-dir E:/AutoSDK/HostWin64/Win64/LLVM/21.1.5/lib/clang/21 -D SAL_NO_ATTRIBUTE_DECLARATIONS=1 -D _CRT_SECURE_NO_WARNINGS -D _DISABLE_EXTENDED_ALIGNED_STORAGE -D _HAS_EXCEPTIONS=0 -D PLATFORM_EXCEPTIONS_DISABLED=1 -internal-isystem "E:/AutoSDK/HostWin64/Win64/LLVM/21.1.5/lib/clang/21\\include" -internal-isystem "undefined\\include" -internal-isystem "undefined\\atlmfc\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.26100.0\\cppwinrt" -Werror -WCL4 -Werror -Werror -Wshadow -Wno-dllexport-explicit-instantiation-decl -Wno-undefined-var-template -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-unused-function -Wno-unused-lambda-capture -Wno-unused-local-typedef -Wno-unused-private-field -Wno-unused-variable -Wno-unknown-pragmas -Wno-tautological-compare -Wno-switch -Wno-invalid-offsetof -Wno-inconsistent-missing-override -Wno-gnu-string-literal-operator-template -Wno-missing-template-arg-list-after-template-kw -Wno-cast-function-type-mismatch -Wno-extra-qualification -Wno-nontrivial-memaccess -Wno-nan-infinity-disabled -Wno-deprecated-copy -Wno-bitwise-instead-of-logical -Wno-ordered-compare-function-pointers -Wno-deprecated-volatile -Wno-deprecated-anon-enum-enum-conversion -Wno-ambiguous-reversed-operator -Wno-enum-float-conversion -Wno-enum-enum-conversion -Wundef -Wno-float-conversion -Wno-implicit-float-conversion -Wno-implicit-int-conversion -Wno-c++11-narrowing -Wno-invalid-unevaluated-string -Wdeprecated-declarations -Wno-error=deprecated-declarations -Wno-deprecated-copy-with-user-provided-copy -Wno-shorten-64-to-32 -Wno-unused-value -Wunused-result -Wno-switch-enum -Wno-dangling -Wno-null-pointer-subtraction -Wno-bitfield-enum-conversion -Wno-constant-logical-operand -Wno-null-pointer-arithmetic -Wno-invalid-token-paste -Wno-nonportable-include-path -Wno-missing-field-initializers -Wno-sign-compare -Wno-expansion-to-defined -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-return-type -Wno-uninitialized -Wno-ignored-attributes -Wno-microsoft -Wno-msvc-include -Wno-pragma-pack -Wno-deprecated-declarations -Wno-inline-new-delete -Wno-undefined-bool-conversion -Wno-deprecated-writable-strings -Wno-deprecated-register -Wno-logical-op-parentheses -Wno-null-arithmetic -Wno-return-type-c-linkage -Wno-implicit-exception-spec-mismatch -fdeprecated-macro -ferror-limit 19 -fmessage-length=280 -fno-temp-file -fno-use-cxa-atexit -fms-extensions -fms-compatibility -fms-compatibility-version=19.44 -std=c++20 -finline-functions -fno-implicit-modules -fskip-odr-check-in-gmf -fpack-struct=8 -fcolor-diagnostics -fdiagnostics-absolute-paths -vectorize-loops -vectorize-slp -x c++ -gcodeview-ghash -faddrsig -o FortActivityBrowserListView.cpp.obj -x c++ FortActivityBrowserListView.cpp.i
clang -cc1 version 21.1.5 based upon LLVM 21.1.5 default target x86_64-pc-windows-msvc
ignoring nonexistent directory "undefined\include"
ignoring nonexistent directory "undefined\atlmfc\include"
#include "..." search starts here:
#include &lt;...&gt; search starts here:
 E:/AutoSDK/HostWin64/Win64/LLVM/21.1.5/lib/clang/21\include
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\ucrt
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\shared
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\um
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\winrt
 C:\Program Files (x86)\Windows Kits\10\Include\10.0.26100.0\cppwinrt
End of search list.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: E:\\AutoSDK\\HostWin64\\Win64\\LLVM\\21.1.5\\bin\\clang-cl.exe -v @<!-- -->FortActivityBrowserListView.cpp.i.rsp
1.      &lt;eof&gt; parser at end of file
2.      Per-file LLVM IR generation
3.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:316:45: Generating code for declaration 'SBrowserListView&lt;TSharedRef&lt;FFortActivityCategory, ESPMode::ThreadSafe&gt;&gt;::ReGenerateItems'
4.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:321:3: LLVM IR generation of compound statement ('{}')
5.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:322:4: LLVM IR generation of compound statement ('{}')
6.      D:/FN/FortniteGame/Plugins/GameFeatures/DiscoveryBrowser/Source/DiscoveryBrowserUI/Public\ActivityBrowser/FortActivityBrowserListView.h:322:366: LLVM IR generation of compound statement ('{}')
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ff67cd344cd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x16844cd)
 #<!-- -->1 0x00007ff67cad1fa5 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1421fa5)
 #<!-- -->2 0x00007ff67cad1cf8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1421cf8)
 #<!-- -->3 0x00007ff67d11c9c4 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1a6c9c4)
 #<!-- -->4 0x00007ff67c8ca8a2 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121a8a2)
 #<!-- -->5 0x00007ff67b6c0c6d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x10c6d)
 #<!-- -->6 0x00007ff67b6beefb (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0xeefb)
 #<!-- -->7 0x00007ff67c8c28c9 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x12128c9)
 #<!-- -->8 0x00007ff67c8c03ae (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x12103ae)
 #<!-- -->9 0x00007ff67c8c031b (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121031b)
#<!-- -->10 0x00007ff67b6d59f6 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x259f6)
#<!-- -->11 0x00007ff67c8c0497 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1210497)
#<!-- -->12 0x00007ff67c8c031b (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121031b)
#<!-- -->13 0x00007ff67c8c031b (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x121031b)
#<!-- -->14 0x00007ff67b6c26ba (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x126ba)
#<!-- -->15 0x00007ff67c8c03d8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x12103d8)
#<!-- -->16 0x00007ff67c8bfefe (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120fefe)
#<!-- -->17 0x00007ff67c8be515 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120e515)
#<!-- -->18 0x00007ff67c8bbf4e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120bf4e)
#<!-- -->19 0x00007ff67c8bb373 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120b373)
#<!-- -->20 0x00007ff67c8baedd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120aedd)
#<!-- -->21 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->22 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->23 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->24 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->25 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->26 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->27 0x00007ff67c8baf05 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x120af05)
#<!-- -->28 0x00007ff67bf1432e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x86432e)
#<!-- -->29 0x00007ff67bf1427d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x86427d)
#<!-- -->30 0x00007ff67d0808cd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x19d08cd)
#<!-- -->31 0x00007ff67d5e9e85 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1f39e85)
#<!-- -->32 0x00007ff67bcda493 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a493)
#<!-- -->33 0x00007ff67bcda0dd (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62a0dd)
#<!-- -->34 0x00007ff67bcd6d96 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x626d96)
#<!-- -->35 0x00007ff67bcd38f8 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x6238f8)
#<!-- -->36 0x00007ff67bcd218e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x62218e)
#<!-- -->37 0x00007ff67c5bee5d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0xf0ee5d)
#<!-- -->38 0x00007ff67be62685 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2685)
#<!-- -->39 0x00007ff67be62478 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x7b2478)
#<!-- -->40 0x00007ff67bb87825 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d7825)
#<!-- -->41 0x00007ff67bb8716e (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d716e)
#<!-- -->42 0x00007ff67bb86d4d (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4d6d4d)
#<!-- -->43 0x00007ff67bb7da86 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cda86)
#<!-- -->44 0x00007ff67bb7b023 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x4cb023)
#<!-- -->45 0x00007ff67d69d0a0 (E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin\clang-cl.exe+0x1fed0a0)
#<!-- -->46 0x00007ff98981e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#<!-- -->47 0x00007ff98a26c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang-cl: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.5
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: E:\AutoSDK\HostWin64\Win64\LLVM\21.1.5\bin
clang-cl: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-cl: note: diagnostic msg: C:\Users\JOE~1.KIR\AppData\Local\Temp\FortActivityBrowserListView-33d686.cpp
clang-cl: note: diagnostic msg: C:\Users\JOE~1.KIR\AppData\Local\Temp\FortActivityBrowserListView-33d686.sh
clang-cl: note: diagnostic msg:

********************
```
</details>


---

