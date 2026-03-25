# C++ Member pointer with module causing clang crash

**Author:** Yuria-Shikibe
**URL:** https://github.com/llvm/llvm-project/issues/167638
**Status:** Open
**Labels:** help wanted, clang:modules, platform:windows, crash

## Body

these code can reproduce the issue, using clang 21.1.5


module1
``` c++
export module test;

export
struct foo{
	int v;
	int get() const noexcept{
		return v;
	}
};
```

module2
``` c++
export module test_2;

import test;
import std;

export
void bar(){
	std::vector<foo> foos;
	[[maybe_unused]] auto itr = std::ranges::find(foos, 0, &foo::get);
}

```

main.cpp
```  c++
import test_2;
import std;

int main() {
    bar();
}
```

Move the function `bar` into module `test` and `import test;` then the issue CANNOT be reproduced.

Both member pointer and member function pointer seem cause this issue.

My console output, conataining compile option
```
[  1%]: <mo_yanxi> generating.module.deps src\main.cpp
d:\lib\llvm\bin\clang-scan-deps.exe --format=p1689 -- D:\lib\LLVM\bin\clang++ -x c++ -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -c src\main.cpp -o build\.objs\mo_yanxi\windows\x64\debug\src\main.cpp.obj
[  1%]: <mo_yanxi> generating.module.deps src\test.ixx
d:\lib\llvm\bin\clang-scan-deps.exe --format=p1689 -- D:\lib\LLVM\bin\clang++ -x c++ -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -c src\test.ixx -o build\.objs\mo_yanxi\windows\x64\debug\src\test.ixx.obj
[ 25%]: <mo_yanxi> compiling.module.bmi.debug test
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -x c++-module --precompile -o build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm src\test.ixx
[ 34%]: <mo_yanxi> compiling.module.bmi.debug test_2
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -fmodule-file=std=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\af132e7790dc3ea1\std.pcm -fmodule-file=test=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm -x c++-module --precompile -o build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\effb05a9a0b27bb6\test_2.pcm src\test2.ixx
[ 43%]: compiling.debug src\test.ixx
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -o build\.objs\mo_yanxi\windows\x64\debug\src\test.ixx.obj build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm
[ 51%]: compiling.debug src\test2.ixx
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -fmodule-file=std=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\af132e7790dc3ea1\std.pcm -fmodule-file=test=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm -o build\.objs\mo_yanxi\windows\x64\debug\src\test2.ixx.obj build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\effb05a9a0b27bb6\test_2.pcm
[ 69%]: compiling.debug src\main.cpp
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -fmodule-file=std=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\af132e7790dc3ea1\std.pcm -fmodule-file=test_2=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\effb05a9a0b27bb6\test_2.pcm -fmodule-file=test=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm -o build\.objs\mo_yanxi\windows\x64\debug\src\main.cpp.obj src\main.cpp
error: PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: D:\\lib\\LLVM\\bin\\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -fmodule-file=std=build\\.gens\\mo_yanxi\\windows\\x64\\debug\\rules\\bmi\\cache\\interfaces\\af132e7790dc3ea1\\std.pcm -fmodule-file=test=build\\.gens\\mo_yanxi\\windows\\x64\\debug\\rules\\bmi\\cache\\interfaces\\5df994ea6185f58f\\test.pcm -fdiagnostics-color=always -o build\\.objs\\mo_yanxi\\windows\\x64\\debug\\src\\test2.ixx.obj build\\.gens\\mo_yanxi\\windows\\x64\\debug\\rules\\bmi\\cache\\interfaces\\effb05a9a0b27bb6\\test_2.pcm
1.      <eof> parser at end of file
2.      D:\projects\untitled\src\test2.ixx:7:6: LLVM IR generation of declaration 'bar'
3.      D:\projects\untitled\src\test2.ixx:7:6: Generating code for declaration 'bar'
Exception Code: 0xC0000005
 #0 0x00007ff766c084a7 (D:\lib\LLVM\bin\clang++.exe+0x17e84a7)
 #1 0x00007ff7654845df (D:\lib\LLVM\bin\clang++.exe+0x645df)
 #2 0x00007ff766e71201 (D:\lib\LLVM\bin\clang++.exe+0x1a51201)
 #3 0x00007ff766e8c5e4 (D:\lib\LLVM\bin\clang++.exe+0x1a6c5e4)
 #4 0x00007ff766e8ae89 (D:\lib\LLVM\bin\clang++.exe+0x1a6ae89)
 #5 0x00007ff766e86c01 (D:\lib\LLVM\bin\clang++.exe+0x1a66c01)
 #6 0x00007ff766e90bc9 (D:\lib\LLVM\bin\clang++.exe+0x1a70bc9)
 #7 0x00007ff766e7a1ac (D:\lib\LLVM\bin\clang++.exe+0x1a5a1ac)
 #8 0x00007ff7656f593e (D:\lib\LLVM\bin\clang++.exe+0x2d593e)
 #9 0x00007ff7656f564d (D:\lib\LLVM\bin\clang++.exe+0x2d564d)
#10 0x00007ff766e8cd3d (D:\lib\LLVM\bin\clang++.exe+0x1a6cd3d)
#11 0x00007ff766e8ced1 (D:\lib\LLVM\bin\clang++.exe+0x1a6ced1)
#12 0x00007ff766e8c965 (D:\lib\LLVM\bin\clang++.exe+0x1a6c965)
#13 0x00007ff76663a8a2 (D:\lib\LLVM\bin\clang++.exe+0x121a8a2)
#14 0x00007ff765430c6d (D:\lib\LLVM\bin\clang++.exe+0x10c6d)
#15 0x00007ff76542eefb (D:\lib\LLVM\bin\clang++.exe+0xeefb)
#16 0x00007ff7666328c9 (D:\lib\LLVM\bin\clang++.exe+0x12128c9)
#17 0x00007ff7666303ae (D:\lib\LLVM\bin\clang++.exe+0x12103ae)
#18 0x00007ff76662fefe (D:\lib\LLVM\bin\clang++.exe+0x120fefe)
#19 0x00007ff76662e515 (D:\lib\LLVM\bin\clang++.exe+0x120e515)
#20 0x00007ff76662bf4e (D:\lib\LLVM\bin\clang++.exe+0x120bf4e)
#21 0x00007ff76662b4cd (D:\lib\LLVM\bin\clang++.exe+0x120b4cd)
#22 0x00007ff766abef37 (D:\lib\LLVM\bin\clang++.exe+0x169ef37)
#23 0x00007ff766abbb12 (D:\lib\LLVM\bin\clang++.exe+0x169bb12)
#24 0x00007ff7673c0cbc (D:\lib\LLVM\bin\clang++.exe+0x1fa0cbc)
#25 0x00007ff7673b353f (D:\lib\LLVM\bin\clang++.exe+0x1f9353f)
#26 0x00007ff768a8fa80 (D:\lib\LLVM\bin\clang++.exe+0x366fa80)
#27 0x00007ff767d4d181 (D:\lib\LLVM\bin\clang++.exe+0x292d181)
#28 0x00007ff767359fa4 (D:\lib\LLVM\bin\clang++.exe+0x1f39fa4)
#29 0x00007ff765a4a493 (D:\lib\LLVM\bin\clang++.exe+0x62a493)
#30 0x00007ff765a4a0dd (D:\lib\LLVM\bin\clang++.exe+0x62a0dd)
#31 0x00007ff765a46d96 (D:\lib\LLVM\bin\clang++.exe+0x626d96)
#32 0x00007ff765a438f8 (D:\lib\LLVM\bin\clang++.exe+0x6238f8)
#33 0x00007ff765a4218e (D:\lib\LLVM\bin\clang++.exe+0x62218e)
#34 0x00007ff76632ee5d (D:\lib\LLVM\bin\clang++.exe+0xf0ee5d)
#35 0x00007ff765bd2685 (D:\lib\LLVM\bin\clang++.exe+0x7b2685)
#36 0x00007ff765bd2478 (D:\lib\LLVM\bin\clang++.exe+0x7b2478)
#37 0x00007ff7658f7825 (D:\lib\LLVM\bin\clang++.exe+0x4d7825)
#38 0x00007ff7658f716e (D:\lib\LLVM\bin\clang++.exe+0x4d716e)
#39 0x00007ff7658f6d4d (D:\lib\LLVM\bin\clang++.exe+0x4d6d4d)
#40 0x00007ff7658eda86 (D:\lib\LLVM\bin\clang++.exe+0x4cda86)
#41 0x00007ff7658eb023 (D:\lib\LLVM\bin\clang++.exe+0x4cb023)
#42 0x00007ff76740d0a0 (D:\lib\LLVM\bin\clang++.exe+0x1fed0a0)
#43 0x00007fff470ae8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#44 0x00007fff490cc53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.5
Target: x86_64-unknown-windows-msvc
Thread model: posix
InstalledDir: D:\lib\LLVM\bin
clang++: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: MoYanxi (Yuria-Shikibe)

<details>
these code can reproduce the issue, using clang 21.1.5


module1
``` c++
export module test;

export
struct foo{
	int v;
	int get() const noexcept{
		return v;
	}
};
```

module2
``` c++
export module test_2;

import test;
import std;

export
void bar(){
	std::vector&lt;foo&gt; foos;
	[[maybe_unused]] auto itr = std::ranges::find(foos, 0, &amp;foo::get);
}

```

main.cpp
```  c++
import test_2;
import std;

int main() {
    bar();
}
```

Move the function `bar` into module `test` and `import test;` then the issue CANNOT be reproduced.

Both member pointer and member function pointer seem cause this issue.

My console output, conataining compile option
```
[  1%]: &lt;mo_yanxi&gt; generating.module.deps src\main.cpp
d:\lib\llvm\bin\clang-scan-deps.exe --format=p1689 -- D:\lib\LLVM\bin\clang++ -x c++ -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -c src\main.cpp -o build\.objs\mo_yanxi\windows\x64\debug\src\main.cpp.obj
[  1%]: &lt;mo_yanxi&gt; generating.module.deps src\test.ixx
d:\lib\llvm\bin\clang-scan-deps.exe --format=p1689 -- D:\lib\LLVM\bin\clang++ -x c++ -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -c src\test.ixx -o build\.objs\mo_yanxi\windows\x64\debug\src\test.ixx.obj
[ 25%]: &lt;mo_yanxi&gt; compiling.module.bmi.debug test
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -x c++-module --precompile -o build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm src\test.ixx
[ 34%]: &lt;mo_yanxi&gt; compiling.module.bmi.debug test_2
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -fmodule-file=std=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\af132e7790dc3ea1\std.pcm -fmodule-file=test=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm -x c++-module --precompile -o build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\effb05a9a0b27bb6\test_2.pcm src\test2.ixx
[ 43%]: compiling.debug src\test.ixx
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -o build\.objs\mo_yanxi\windows\x64\debug\src\test.ixx.obj build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm
[ 51%]: compiling.debug src\test2.ixx
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -fmodule-file=std=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\af132e7790dc3ea1\std.pcm -fmodule-file=test=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm -o build\.objs\mo_yanxi\windows\x64\debug\src\test2.ixx.obj build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\effb05a9a0b27bb6\test_2.pcm
[ 69%]: compiling.debug src\main.cpp
D:\lib\LLVM\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -fmodule-file=std=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\af132e7790dc3ea1\std.pcm -fmodule-file=test_2=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\effb05a9a0b27bb6\test_2.pcm -fmodule-file=test=build\.gens\mo_yanxi\windows\x64\debug\rules\bmi\cache\interfaces\5df994ea6185f58f\test.pcm -o build\.objs\mo_yanxi\windows\x64\debug\src\main.cpp.obj src\main.cpp
error: PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: D:\\lib\\LLVM\\bin\\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -g -Wall -O0 -std=c++26 -fexceptions -fcxx-exceptions -finput-charset=UTF-8 -fexec-charset=UTF-8 -fmodule-file=std=build\\.gens\\mo_yanxi\\windows\\x64\\debug\\rules\\bmi\\cache\\interfaces\\af132e7790dc3ea1\\std.pcm -fmodule-file=test=build\\.gens\\mo_yanxi\\windows\\x64\\debug\\rules\\bmi\\cache\\interfaces\\5df994ea6185f58f\\test.pcm -fdiagnostics-color=always -o build\\.objs\\mo_yanxi\\windows\\x64\\debug\\src\\test2.ixx.obj build\\.gens\\mo_yanxi\\windows\\x64\\debug\\rules\\bmi\\cache\\interfaces\\effb05a9a0b27bb6\\test_2.pcm
1.      &lt;eof&gt; parser at end of file
2.      D:\projects\untitled\src\test2.ixx:7:6: LLVM IR generation of declaration 'bar'
3.      D:\projects\untitled\src\test2.ixx:7:6: Generating code for declaration 'bar'
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ff766c084a7 (D:\lib\LLVM\bin\clang++.exe+0x17e84a7)
 #<!-- -->1 0x00007ff7654845df (D:\lib\LLVM\bin\clang++.exe+0x645df)
 #<!-- -->2 0x00007ff766e71201 (D:\lib\LLVM\bin\clang++.exe+0x1a51201)
 #<!-- -->3 0x00007ff766e8c5e4 (D:\lib\LLVM\bin\clang++.exe+0x1a6c5e4)
 #<!-- -->4 0x00007ff766e8ae89 (D:\lib\LLVM\bin\clang++.exe+0x1a6ae89)
 #<!-- -->5 0x00007ff766e86c01 (D:\lib\LLVM\bin\clang++.exe+0x1a66c01)
 #<!-- -->6 0x00007ff766e90bc9 (D:\lib\LLVM\bin\clang++.exe+0x1a70bc9)
 #<!-- -->7 0x00007ff766e7a1ac (D:\lib\LLVM\bin\clang++.exe+0x1a5a1ac)
 #<!-- -->8 0x00007ff7656f593e (D:\lib\LLVM\bin\clang++.exe+0x2d593e)
 #<!-- -->9 0x00007ff7656f564d (D:\lib\LLVM\bin\clang++.exe+0x2d564d)
#<!-- -->10 0x00007ff766e8cd3d (D:\lib\LLVM\bin\clang++.exe+0x1a6cd3d)
#<!-- -->11 0x00007ff766e8ced1 (D:\lib\LLVM\bin\clang++.exe+0x1a6ced1)
#<!-- -->12 0x00007ff766e8c965 (D:\lib\LLVM\bin\clang++.exe+0x1a6c965)
#<!-- -->13 0x00007ff76663a8a2 (D:\lib\LLVM\bin\clang++.exe+0x121a8a2)
#<!-- -->14 0x00007ff765430c6d (D:\lib\LLVM\bin\clang++.exe+0x10c6d)
#<!-- -->15 0x00007ff76542eefb (D:\lib\LLVM\bin\clang++.exe+0xeefb)
#<!-- -->16 0x00007ff7666328c9 (D:\lib\LLVM\bin\clang++.exe+0x12128c9)
#<!-- -->17 0x00007ff7666303ae (D:\lib\LLVM\bin\clang++.exe+0x12103ae)
#<!-- -->18 0x00007ff76662fefe (D:\lib\LLVM\bin\clang++.exe+0x120fefe)
#<!-- -->19 0x00007ff76662e515 (D:\lib\LLVM\bin\clang++.exe+0x120e515)
#<!-- -->20 0x00007ff76662bf4e (D:\lib\LLVM\bin\clang++.exe+0x120bf4e)
#<!-- -->21 0x00007ff76662b4cd (D:\lib\LLVM\bin\clang++.exe+0x120b4cd)
#<!-- -->22 0x00007ff766abef37 (D:\lib\LLVM\bin\clang++.exe+0x169ef37)
#<!-- -->23 0x00007ff766abbb12 (D:\lib\LLVM\bin\clang++.exe+0x169bb12)
#<!-- -->24 0x00007ff7673c0cbc (D:\lib\LLVM\bin\clang++.exe+0x1fa0cbc)
#<!-- -->25 0x00007ff7673b353f (D:\lib\LLVM\bin\clang++.exe+0x1f9353f)
#<!-- -->26 0x00007ff768a8fa80 (D:\lib\LLVM\bin\clang++.exe+0x366fa80)
#<!-- -->27 0x00007ff767d4d181 (D:\lib\LLVM\bin\clang++.exe+0x292d181)
#<!-- -->28 0x00007ff767359fa4 (D:\lib\LLVM\bin\clang++.exe+0x1f39fa4)
#<!-- -->29 0x00007ff765a4a493 (D:\lib\LLVM\bin\clang++.exe+0x62a493)
#<!-- -->30 0x00007ff765a4a0dd (D:\lib\LLVM\bin\clang++.exe+0x62a0dd)
#<!-- -->31 0x00007ff765a46d96 (D:\lib\LLVM\bin\clang++.exe+0x626d96)
#<!-- -->32 0x00007ff765a438f8 (D:\lib\LLVM\bin\clang++.exe+0x6238f8)
#<!-- -->33 0x00007ff765a4218e (D:\lib\LLVM\bin\clang++.exe+0x62218e)
#<!-- -->34 0x00007ff76632ee5d (D:\lib\LLVM\bin\clang++.exe+0xf0ee5d)
#<!-- -->35 0x00007ff765bd2685 (D:\lib\LLVM\bin\clang++.exe+0x7b2685)
#<!-- -->36 0x00007ff765bd2478 (D:\lib\LLVM\bin\clang++.exe+0x7b2478)
#<!-- -->37 0x00007ff7658f7825 (D:\lib\LLVM\bin\clang++.exe+0x4d7825)
#<!-- -->38 0x00007ff7658f716e (D:\lib\LLVM\bin\clang++.exe+0x4d716e)
#<!-- -->39 0x00007ff7658f6d4d (D:\lib\LLVM\bin\clang++.exe+0x4d6d4d)
#<!-- -->40 0x00007ff7658eda86 (D:\lib\LLVM\bin\clang++.exe+0x4cda86)
#<!-- -->41 0x00007ff7658eb023 (D:\lib\LLVM\bin\clang++.exe+0x4cb023)
#<!-- -->42 0x00007ff76740d0a0 (D:\lib\LLVM\bin\clang++.exe+0x1fed0a0)
#<!-- -->43 0x00007fff470ae8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#<!-- -->44 0x00007fff490cc53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.5
Target: x86_64-unknown-windows-msvc
Thread model: posix
InstalledDir: D:\lib\LLVM\bin
clang++: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```


</details>


---

### Comment 2 - ChuanqiXu9

It would be best to provide a reproducer withoutstd module and std headers.

Or you can try to use assertion-built trunk clang to give it a try.

---

### Comment 3 - Yuria-Shikibe

> It would be best to provide a reproducer withoutstd module and std headers.
> 
> Or you can try to use assertion-built trunk clang to give it a try.

```
export module test;

export
struct foo{
	int v;
	int get() const noexcept{
		return v;
	}
};
```

```
export module test_2;

export import test;

export
auto bar(foo f){
	return (f.*&foo::get)();
}

```

```
import test_2;

int main() {
    foo f{1};
    bar(f);
}
```

this code sample without std module produce similar crash

---

### Comment 4 - ChuanqiXu9

I can't reproduce this in trunk. Please try it with trunk.

---

### Comment 5 - Yuria-Shikibe

> I can't reproduce this in trunk. Please try it with trunk.


<img width="2401" height="1346" alt="Image" src="https://github.com/user-attachments/assets/009efdd4-df81-4b4f-8bc1-a8693bdb57ba" />
The trunk and 21.1 20.1 and 21.1.5 on other host compiled the code. 

I tried reinstall llvm 21.1.5 still doesn't help. Is this issue have something to do with compile options or environment settings?

---

### Comment 6 - PerviousFurther

Recently encounting with same problem since project mirgation from header to module.
Seems the error will occurred when the situation have following factors:
 - some struct from *other* module is taken member (or member function) pointer.
 - maybe only when platform is windows.

test code:
```cpp
// test.cppm
export module test;
export struct foo
{
  int v;
  int get();
};

// test_1.cppm
export module test_2;
export import test;
void call()
{
  int (foo::*mem)() = &foo::get; // use auto make no effort.
  // int foo::*mem = &foo::v; // this will produce the same problem.
}
```

log:
```ps
error: clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.1.8
Target: x86_64-unknown-windows-msvc
Thread model: posix
InstalledDir: D:\Visual_Studio\VC\Tools\Llvm\x64\bin
clang++: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
  > in build\.gens\test\windows\x64\release\rules\bmi\cache\interfaces\effb05a9a0b27bb6\test_2.pcm
```

---

### Comment 7 - ChuanqiXu9

Seems to be a windows specific issue. I don't have a window env and I don't use windows. So I'd like to mark this one as help needed.

---

### Comment 8 - ChuanqiXu9

BTW, even if I used MSVC's ABI, I can't still reproduce this on Windows:

```
// RUN: rm -rf %t
// RUN: mkdir -p %t
// RUN: split-file %s %t
// RUN: %clang_cc1 -std=c++20 %t/test.cppm -triple x86_64-windows-msvc -emit-reduced-module-interface -o %t/test.pcm
// RUN: %clang_cc1 -std=c++20 %t/test_1.cppm -triple x86_64-windows-msvc -fprebuilt-module-path=%t -fsyntax-only -verify

//--- test.cppm
export module test;
export struct foo
{
  int v;
  int get();
};

//--- test_1.cppm
// expected-no-diagnostics
export module test_2;
export import test;
void call()
{
  int (foo::*mem)() = &foo::get; // use auto make no effort.
  // int foo::*mem = &foo::v; // this will produce the same problem.
}
```

---

### Comment 9 - Yuria-Shikibe

> BTW, even if I used MSVC's ABI, I can't still reproduce this on Windows:
> ...

Log from clang 23.0.0 with assertion, using sample from https://github.com/llvm/llvm-project/issues/167638#issuecomment-3520652716 :
G:\projects\llvm-project\build_assertions\bin\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -fms-runtime-lib=dll -fvisibility=hidden -fvisibility-inlines-hidden -O3 -std=c++26 -fexceptions -fcxx-exceptions -DNDEBUG -Wno-include-angled-in-module-purview -Wno-reserved-module-identifier -Wno-deprecated-declarations -o build\.objs\test\windows\x64\release\1bcdc514c6ebc3da38992483d19188c3\std.ixx.obj build\.gens\test\windows\x64\release\rules\bmi\cache\interfaces\7ae89070f949f44e\std.pcm
error: Assertion failed: IA && "Expected MSInheritanceAttr on the CXXRecordDecl!", file G:\projects\llvm-project\clang\lib\AST\MicrosoftCXXABI.cpp, line 234
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: G:\\projects\\llvm-project\\build_assertions\\bin\\clang++ -c -Qunused-arguments -m64 --target=x86_64-windows-msvc -fms-runtime-lib=dll -fvisibility=hidden -fvisibility-inlines-hidden -O3 -std=c++26 -fexceptions -fcxx-exceptions -DNDEBUG -fmodule-file=test=build\\.gens\\test\\windows\\x64\\release\\rules\\bmi\\cache\\interfaces\\33a9fcd79e11e85c\\test.pcm -fdiagnostics-color=always -o build\\.objs\\test\\windows\\x64\\release\\src\\test\\m2.ixx.obj build\\.gens\\test\\windows\\x64\\release\\rules\\bmi\\cache\\interfaces\\9bfafc8b394a7dc5\\test_2.pcm
1.      <eof> parser at end of file
2.      D:\projects\mo_yanxi_vulkan_wrapper\src\test\m2.ixx:6:6: LLVM IR generation of declaration 'bar'
3.      D:\projects\mo_yanxi_vulkan_wrapper\src\test\m2.ixx:6:6: Generating code for declaration 'bar'
Exception Code: 0x80000003
 #0 0x00007ff74a2b33d5 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x12133d5)
 #1 0x00007fff89281989 (C:\WINDOWS\System32\ucrtbase.dll+0xc1989)
 #2 0x00007fff89264ab1 (C:\WINDOWS\System32\ucrtbase.dll+0xa4ab1)
 #3 0x00007fff89282986 (C:\WINDOWS\System32\ucrtbase.dll+0xc2986)
 #4 0x00007fff89282b61 (C:\WINDOWS\System32\ucrtbase.dll+0xc2b61)
 #5 0x00007ff74c7789bd (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x36d89bd)
 #6 0x00007ff74a71f02e (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x167f02e)
 #7 0x00007ff74a61b5a0 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x157b5a0)
 #8 0x00007ff74a9203fb (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x18803fb)
 #9 0x00007ff74a91ba4b (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x187ba4b)
#10 0x00007ff74a992b1e (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x18f2b1e)
#11 0x00007ff74a990a25 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x18f0a25)
#12 0x00007ff74a6e671d (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x164671d)
#13 0x00007ff74a925be0 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1885be0)
#14 0x00007ff74a9212eb (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x18812eb)
#15 0x00007ff74a91ba4b (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x187ba4b)
#16 0x00007ff74a6056db (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x15656db)
#17 0x00007ff74a606292 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1566292)
#18 0x00007ff74a602697 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1562697)
#19 0x00007ff74a6cfb29 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x162fb29)
#20 0x00007ff74a5987a8 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x14f87a8)
#21 0x00007ff74a5983c9 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x14f83c9)
#22 0x00007ff74a597d54 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x14f7d54)
#23 0x00007ff74a59b4d2 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x14fb4d2)
#24 0x00007ff74ab845bd (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1ae45bd)
#25 0x00007ff74ab517cf (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1ab17cf)
#26 0x00007ff74afb4374 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1f14374)
#27 0x00007ff74af21142 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1e81142)
#28 0x00007ff74ba1fbc8 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x297fbc8)
#29 0x00007ff74ade9288 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1d49288)
#30 0x00007ff74ab50ac7 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1ab0ac7)
#31 0x00007ff74ade9086 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1d49086)
#32 0x00007ff74adcf85a (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1d2f85a)
#33 0x00007ff74ae97349 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1df7349)
#34 0x00007ff74914bcdb (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0xabcdb)
#35 0x00007ff7491457fc (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0xa57fc)
#36 0x00007ff74914447d (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0xa447d)
#37 0x00007ff74ac89be1 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1be9be1)
#38 0x00007ff74a2a4efa (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1204efa)
#39 0x00007ff74ac8a229 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1bea229)
#40 0x00007ff74abc67f0 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1b267f0)
#41 0x00007ff74abc6a35 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1b26a35)
#42 0x00007ff74aba7c13 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x1b07c13)
#43 0x00007ff749147988 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0xa7988)
#44 0x00007ff749156554 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0xb6554)
#45 0x00007ff74d03a8a8 (G:\projects\llvm-project\build_assertions\bin\clang++.exe+0x3f9a8a8)
#46 0x00007fff8a87e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#47 0x00007fff8bc2c53c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x8c53c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 23.0.0git
Target: x86_64-unknown-windows-msvc
Thread model: posix
InstalledDir: G:\projects\llvm-project\build_assertions\bin

---

### Comment 10 - ChuanqiXu9

This seems relates to https://github.com/llvm/llvm-project/issues/174067

---

