# [clang++] Crash using C++ modules

**Author:** olizit
**URL:** https://github.com/llvm/llvm-project/issues/160844
**Status:** Open
**Labels:** clang:modules, platform:windows, crash

## Body

Crash when using pointer to member function

[Model-0f3e90.cppm.log](https://github.com/user-attachments/files/22558108/Model-0f3e90.cppm.log)
[Model-0f3e90.sh.log](https://github.com/user-attachments/files/22558122/Model-0f3e90.sh.log)
```
[build] 1.	<eof> parser at end of file
[build] 2.	C:\Dev\DemoApp\src\events\Property.cppm:6:7: LLVM IR generation of declaration 'Property'
[build] Exception Code: 0xC0000005
[build]  #0 0x00007ff69f768f17 (C:\Program Files\LLVM\bin\clang++.exe+0x17e8f17)
[build]  #1 0x00007ff69e16a953 (C:\Program Files\LLVM\bin\clang++.exe+0x1ea953)
[build]  #2 0x00007ff69f7634d4 (C:\Program Files\LLVM\bin\clang++.exe+0x17e34d4)
[build]  #3 0x00007ff69f75a89a (C:\Program Files\LLVM\bin\clang++.exe+0x17da89a)
[build]  #4 0x00007ff69f764f8d (C:\Program Files\LLVM\bin\clang++.exe+0x17e4f8d)
[build]  #5 0x00007ff69f98b58e (C:\Program Files\LLVM\bin\clang++.exe+0x1a0b58e)
[build]  #6 0x00007ff69e9927d5 (C:\Program Files\LLVM\bin\clang++.exe+0xa127d5)
[build]  #7 0x00007ff6a069c2ec (C:\Program Files\LLVM\bin\clang++.exe+0x271c2ec)
[build]  #8 0x00007ff6a06a6d4b (C:\Program Files\LLVM\bin\clang++.exe+0x2726d4b)
[build]  #9 0x00007ff6a06a31c8 (C:\Program Files\LLVM\bin\clang++.exe+0x27231c8)
[build] #10 0x00007ff6a06a2b32 (C:\Program Files\LLVM\bin\clang++.exe+0x2722b32)
[build] #11 0x00007ff6a06a2aa9 (C:\Program Files\LLVM\bin\clang++.exe+0x2722aa9)
[build] #12 0x00007ff6a06a0ad5 (C:\Program Files\LLVM\bin\clang++.exe+0x2720ad5)
[build] #13 0x00007ff69f61d328 (C:\Program Files\LLVM\bin\clang++.exe+0x169d328)
[build] #14 0x00007ff69ff204bc (C:\Program Files\LLVM\bin\clang++.exe+0x1fa04bc)
[build] #15 0x00007ff69ff12d3f (C:\Program Files\LLVM\bin\clang++.exe+0x1f92d3f)
[build] #16 0x00007ff6a15f0e90 (C:\Program Files\LLVM\bin\clang++.exe+0x3670e90)
[build] #17 0x00007ff6a08ae4e1 (C:\Program Files\LLVM\bin\clang++.exe+0x292e4e1)
[build] #18 0x00007ff69fea6e84 (C:\Program Files\LLVM\bin\clang++.exe+0x1f26e84)
[build] #19 0x00007ff69e5a9453 (C:\Program Files\LLVM\bin\clang++.exe+0x629453)
[build] #20 0x00007ff69e5a909d (C:\Program Files\LLVM\bin\clang++.exe+0x62909d)
[build] #21 0x00007ff69e5a5d56 (C:\Program Files\LLVM\bin\clang++.exe+0x625d56)
[build] #22 0x00007ff69e5a28b8 (C:\Program Files\LLVM\bin\clang++.exe+0x6228b8)
[build] #23 0x00007ff69e5a114e (C:\Program Files\LLVM\bin\clang++.exe+0x62114e)
[build] #24 0x00007ff69e44c50a (C:\Program Files\LLVM\bin\clang++.exe+0x4cc50a)
[build] #25 0x00007ff69e448dc3 (C:\Program Files\LLVM\bin\clang++.exe+0x4c8dc3)
[build] #26 0x00007ff69ff6c890 (C:\Program Files\LLVM\bin\clang++.exe+0x1fec890)
[build] #27 0x00007ffff094e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
[build] #28 0x00007ffff2b3bf2c (C:\WINDOWS\SYSTEM32\ntdll.dll+0xbbf2c)
[build] clang++: error: clang frontend command failed due to signal (use -v to see invocation)
[build] clang version 21.1.2
[build] Target: x86_64-pc-windows-msvc
[build] Thread model: posix
[build] InstalledDir: C:\Program Files\LLVM\bin
[build] clang++: note: diagnostic msg: 
[build] ********************
[build] 
[build] PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
[build] Preprocessed source(s) and associated run script(s) are located at:
[build] clang++: note: diagnostic msg: C:\Users\OLIVIE~1.PEL\AppData\Local\Temp\Model-0f3e90.cppm
[build] clang++: note: diagnostic msg: C:\Users\OLIVIE~1.PEL\AppData\Local\Temp\Model-0f3e90.sh
[build] clang++: note: diagnostic msg: 
[build] 
[build] ********************
```

Step to reproduce with reduce sample:
1. Define first module:
```
// In module1.cppm file
export module module1;

export class IFunc
{
public:
  virtual ~IFunc() = default;

public:
  virtual void firstFunc(double val) = 0;
};
```
2. Define second module:
```
// In module2.cppm file
export module module2;

import module1;

export void Consume(IFunc * object)
{
  double d{2.1};
  auto func = &IFunc::firstFunc;
  (object->*func)(d);
}
```
Arguments and result backtrace:
```
Stack dump:
0.      Program arguments: "C:\\Program Files\\LLVM\\bin\\clang++.exe" -cc1 -triple x86_64-pc-windows-msvc19.39.33521 -O0 -emit-obj -mincremental-linker-compatible -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name module2.cppm -mrelocation-model pic -pic-level 2 -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -fms-volatile -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -gno-column-info -gcodeview -debug-info-kind=constructor -fdebug-compilation-dir=C:\\Dev\\Build\\DemoApp\\Debug -object-file-name=C:\\Dev\\Build\\DemoApp\\Debug\\CMakeFiles\\DemoAppExe.dir\\example\\module2.cppm.obj -fcoverage-compilation-dir=C:\\Dev\\Build\\DemoApp\\Debug -resource-dir "C:\\Program Files\\LLVM\\lib\\clang\\21" -std=c++23 -fdeprecated-macro -ferror-limit 19 -fno-use-cxa-atexit -fms-extensions -fms-compatibility -fms-compatibility-version=19.39.33521 -fno-implicit-modules -fmodule-file=module1=CMakeFiles/DemoAppExe.dir/module1.pcm -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions --dependent-lib=msvcrtd -gcodeview -faddrsig -o CMakeFiles/DemoAppExe.dir/example/module2.cppm.obj -x pcm CMakeFiles/DemoAppExe.dir/module2.pcm
1.      <eof> parser at end of file
2.      C:\Dev\DemoApp\example\module2.cppm:5:13: LLVM IR generation of declaration 'Consume'
3.      C:\Dev\DemoApp\example\module2.cppm:5:13: Generating code for declaration 'Consume'
Exception Code: 0xC0000005
 #0 0x00007ff6916d8f17 (C:\Program Files\LLVM\bin\clang++.exe+0x17e8f17)
 #1 0x00007ff68ff545df (C:\Program Files\LLVM\bin\clang++.exe+0x645df)
 #2 0x00007ff691941d41 (C:\Program Files\LLVM\bin\clang++.exe+0x1a51d41)
 #3 0x00007ff68ff01856 (C:\Program Files\LLVM\bin\clang++.exe+0x11856)
 #4 0x00007ff691109fb6 (C:\Program Files\LLVM\bin\clang++.exe+0x1219fb6)
 #5 0x00007ff68ff00c6d (C:\Program Files\LLVM\bin\clang++.exe+0x10c6d)
 #6 0x00007ff68fefeefb (C:\Program Files\LLVM\bin\clang++.exe+0xeefb)
 #7 0x00007ff691102109 (C:\Program Files\LLVM\bin\clang++.exe+0x1212109)
 #8 0x00007ff6910ffbee (C:\Program Files\LLVM\bin\clang++.exe+0x120fbee)
 #9 0x00007ff6910ff73e (C:\Program Files\LLVM\bin\clang++.exe+0x120f73e)
#10 0x00007ff6910fdd55 (C:\Program Files\LLVM\bin\clang++.exe+0x120dd55)
#11 0x00007ff6910fb78e (C:\Program Files\LLVM\bin\clang++.exe+0x120b78e)
#12 0x00007ff6910fad0d (C:\Program Files\LLVM\bin\clang++.exe+0x120ad0d)
#13 0x00007ff69158f9d7 (C:\Program Files\LLVM\bin\clang++.exe+0x169f9d7)
#14 0x00007ff69158c5b2 (C:\Program Files\LLVM\bin\clang++.exe+0x169c5b2)
#15 0x00007ff691e904bc (C:\Program Files\LLVM\bin\clang++.exe+0x1fa04bc)
#16 0x00007ff691e82d3f (C:\Program Files\LLVM\bin\clang++.exe+0x1f92d3f)
#17 0x00007ff693560e90 (C:\Program Files\LLVM\bin\clang++.exe+0x3670e90)
#18 0x00007ff69281e4e1 (C:\Program Files\LLVM\bin\clang++.exe+0x292e4e1)
#19 0x00007ff691e16e84 (C:\Program Files\LLVM\bin\clang++.exe+0x1f26e84)
#20 0x00007ff690519453 (C:\Program Files\LLVM\bin\clang++.exe+0x629453)
#21 0x00007ff69051909d (C:\Program Files\LLVM\bin\clang++.exe+0x62909d)
#22 0x00007ff690515d56 (C:\Program Files\LLVM\bin\clang++.exe+0x625d56)
#23 0x00007ff6905128b8 (C:\Program Files\LLVM\bin\clang++.exe+0x6228b8)
#24 0x00007ff69051114e (C:\Program Files\LLVM\bin\clang++.exe+0x62114e)
#25 0x00007ff6903bc50a (C:\Program Files\LLVM\bin\clang++.exe+0x4cc50a)
#26 0x00007ff6903b8dc3 (C:\Program Files\LLVM\bin\clang++.exe+0x4c8dc3)
#27 0x00007ff691edc890 (C:\Program Files\LLVM\bin\clang++.exe+0x1fec890)
#28 0x00007ffff094e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#29 0x00007ffff2b3bf2c (C:\WINDOWS\SYSTEM32\ntdll.dll+0xbbf2c)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.2
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: C:\Program Files\LLVM\bin
clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: C:\Users\OLIVIE~1.PEL\AppData\Local\Temp\module2-d17ad9.cppm
clang++: note: diagnostic msg: C:\Users\OLIVIE~1.PEL\AppData\Local\Temp\module2-d17ad9.sh
clang++: note: diagnostic msg:

********************

```
Log files:
[module2-93f56c.cppm.log](https://github.com/user-attachments/files/22591901/module2-93f56c.cppm.log)
[module2-93f56c.sh.log](https://github.com/user-attachments/files/22591904/module2-93f56c.sh.log)

Regression status:
LLVM v19.1.1: failed
LLVM v18.1.8: failed

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Olivier (olizit)

<details>
[Model-0f3e90.cppm.log](https://github.com/user-attachments/files/22558108/Model-0f3e90.cppm.log)

[Model-0f3e90.sh.log](https://github.com/user-attachments/files/22558122/Model-0f3e90.sh.log)

Crash when using pointer to member function
```
[build] 1.	&lt;eof&gt; parser at end of file
[build] 2.	C:\Dev\DemoApp\src\events\Property.cppm:6:7: LLVM IR generation of declaration 'Property'
[build] Exception Code: 0xC0000005
[build]  #<!-- -->0 0x00007ff69f768f17 (C:\Program Files\LLVM\bin\clang++.exe+0x17e8f17)
[build]  #<!-- -->1 0x00007ff69e16a953 (C:\Program Files\LLVM\bin\clang++.exe+0x1ea953)
[build]  #<!-- -->2 0x00007ff69f7634d4 (C:\Program Files\LLVM\bin\clang++.exe+0x17e34d4)
[build]  #<!-- -->3 0x00007ff69f75a89a (C:\Program Files\LLVM\bin\clang++.exe+0x17da89a)
[build]  #<!-- -->4 0x00007ff69f764f8d (C:\Program Files\LLVM\bin\clang++.exe+0x17e4f8d)
[build]  #<!-- -->5 0x00007ff69f98b58e (C:\Program Files\LLVM\bin\clang++.exe+0x1a0b58e)
[build]  #<!-- -->6 0x00007ff69e9927d5 (C:\Program Files\LLVM\bin\clang++.exe+0xa127d5)
[build]  #<!-- -->7 0x00007ff6a069c2ec (C:\Program Files\LLVM\bin\clang++.exe+0x271c2ec)
[build]  #<!-- -->8 0x00007ff6a06a6d4b (C:\Program Files\LLVM\bin\clang++.exe+0x2726d4b)
[build]  #<!-- -->9 0x00007ff6a06a31c8 (C:\Program Files\LLVM\bin\clang++.exe+0x27231c8)
[build] #<!-- -->10 0x00007ff6a06a2b32 (C:\Program Files\LLVM\bin\clang++.exe+0x2722b32)
[build] #<!-- -->11 0x00007ff6a06a2aa9 (C:\Program Files\LLVM\bin\clang++.exe+0x2722aa9)
[build] #<!-- -->12 0x00007ff6a06a0ad5 (C:\Program Files\LLVM\bin\clang++.exe+0x2720ad5)
[build] #<!-- -->13 0x00007ff69f61d328 (C:\Program Files\LLVM\bin\clang++.exe+0x169d328)
[build] #<!-- -->14 0x00007ff69ff204bc (C:\Program Files\LLVM\bin\clang++.exe+0x1fa04bc)
[build] #<!-- -->15 0x00007ff69ff12d3f (C:\Program Files\LLVM\bin\clang++.exe+0x1f92d3f)
[build] #<!-- -->16 0x00007ff6a15f0e90 (C:\Program Files\LLVM\bin\clang++.exe+0x3670e90)
[build] #<!-- -->17 0x00007ff6a08ae4e1 (C:\Program Files\LLVM\bin\clang++.exe+0x292e4e1)
[build] #<!-- -->18 0x00007ff69fea6e84 (C:\Program Files\LLVM\bin\clang++.exe+0x1f26e84)
[build] #<!-- -->19 0x00007ff69e5a9453 (C:\Program Files\LLVM\bin\clang++.exe+0x629453)
[build] #<!-- -->20 0x00007ff69e5a909d (C:\Program Files\LLVM\bin\clang++.exe+0x62909d)
[build] #<!-- -->21 0x00007ff69e5a5d56 (C:\Program Files\LLVM\bin\clang++.exe+0x625d56)
[build] #<!-- -->22 0x00007ff69e5a28b8 (C:\Program Files\LLVM\bin\clang++.exe+0x6228b8)
[build] #<!-- -->23 0x00007ff69e5a114e (C:\Program Files\LLVM\bin\clang++.exe+0x62114e)
[build] #<!-- -->24 0x00007ff69e44c50a (C:\Program Files\LLVM\bin\clang++.exe+0x4cc50a)
[build] #<!-- -->25 0x00007ff69e448dc3 (C:\Program Files\LLVM\bin\clang++.exe+0x4c8dc3)
[build] #<!-- -->26 0x00007ff69ff6c890 (C:\Program Files\LLVM\bin\clang++.exe+0x1fec890)
[build] #<!-- -->27 0x00007ffff094e8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
[build] #<!-- -->28 0x00007ffff2b3bf2c (C:\WINDOWS\SYSTEM32\ntdll.dll+0xbbf2c)
[build] clang++: error: clang frontend command failed due to signal (use -v to see invocation)
[build] clang version 21.1.2
[build] Target: x86_64-pc-windows-msvc
[build] Thread model: posix
[build] InstalledDir: C:\Program Files\LLVM\bin
[build] clang++: note: diagnostic msg: 
[build] ********************
[build] 
[build] PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
[build] Preprocessed source(s) and associated run script(s) are located at:
[build] clang++: note: diagnostic msg: C:\Users\OLIVIE~1.PEL\AppData\Local\Temp\Model-0f3e90.cppm
[build] clang++: note: diagnostic msg: C:\Users\OLIVIE~1.PEL\AppData\Local\Temp\Model-0f3e90.sh
[build] clang++: note: diagnostic msg: 
[build] 
[build] ********************
```
</details>


---

### Comment 2 - ChuanqiXu9

It is better to asserted clang to give more information. And it will be helpful to test with trunk and previous version. If it doesn't occur with previous version, this is a regression. We can find the commit introduces the regression.

Finally, it will be much better to provide a reduced example.

---

### Comment 3 - olizit

Ticket modified with a reduce example with sample code and log files.

---

### Comment 4 - ChuanqiXu9

Can you test with trunk and enable assertions? And it will be helpful to get rid of headers like `<functional>`

---

### Comment 5 - olizit

Ticket modified without headers and add regression status

---

### Comment 6 - ChuanqiXu9

> Ticket modified without headers and add regression status

This is not valid:

```
// In module2.cppm file
export module module2;

import module1;

export void Consume(IFunc & func)
{
  double d{2.1};
  auto func = &IFunc::firstFunc;
  (object->*func)(d);
}
```

`func` redefined and object not defined.

---

### Comment 7 - olizit

Oupps sorry wrong copy paste. It's fixed now

---

