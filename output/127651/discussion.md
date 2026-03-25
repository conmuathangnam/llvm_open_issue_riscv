# [AVR] clang LTO causes Arduino examples to break

**Author:** beakthoven
**URL:** https://github.com/llvm/llvm-project/issues/127651
**Status:** Open
**Labels:** backend:AVR, miscompilation, LTO

## Body

When compiling the Blink example from the Arduino IDE for AVR using Clang with LTO enabled, the program fails to function correctly. The LED blinks once and then remains stuck in the ON position indefinitely. Disabling LTO for `wiring.c` from Arduino AVR core resolves the issue.

LLVM version: 20.1.0-rc2

### Blink example code:
```cpp
#include <Arduino.h>

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(1000); 
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); 
}
```

### Compiler flags used (other than -flto -fno-fat-lto-objects):
```bash
--target=avr -c -g -Os -Wall -Wextra --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu11 -ffunction-sections -fdata-sections -MMD -mmcu=atmega2560 -DF_CPU=16000000L
```
### Expected Behavior
The LED should continue blinking at a 1-second interval indefinitely.

### Attachments
LLVM IR files for wiring.c with LTO turned on and off.
[wiring.c-lto.ll.txt](https://github.com/user-attachments/files/18847521/wiring.c-lto.ll.txt)
[wiring.c-nonlto.ll.txt](https://github.com/user-attachments/files/18847522/wiring.c-nonlto.ll.txt)

`wiring.c`: https://github.com/ClangBuiltArduino/core_arduino_avr/blob/8b327d7bede1c1245db99daeba4e168c92c11194/cores/arduino/wiring.c

## Comments

### Comment 1 - foxtran

Does GCC LTO have the same effect? Just to be sure that is not a problem with C code.

---

### Comment 2 - beakthoven

> Does GCC LTO have the same effect? Just to be sure that is not a problem with C code.

No, AVR GCC works perfectly fine with the same code with and without LTO.

---

### Comment 3 - benshi001

I am a little confused, I have tried your attached two `.ll.txt` files, with 
```bash
wujie@WUJIE-16:~/Downloads$ llc wiring.c-lto.ll.txt -mcpu=atmega2560 -o wiring.c-lto.s.txt
wujie@WUJIE-16:~/Downloads$ llc wiring.c-nonlto.ll.txt -mcpu=atmega2560 wiring.c-nonlto.s.txt
```

The generated assembly source files only have difference in debug information, but not functional code.

[wiring.c-lto.s.txt](https://github.com/user-attachments/files/19267729/wiring.c-lto.s.txt)

[wiring.c-nonlto.s.txt](https://github.com/user-attachments/files/19267728/wiring.c-nonlto.s.txt)

---

### Comment 4 - benshi001

I also tried to compile `wiring.c` without debug information,

```bash
clang --target=avr  -S  -Os -Wall -Wextra  -std=gnu11 -ffunction-sections -fdata-sections -MMD -mmcu=atmega2560 -DF_CPU=16000000L ./core_arduino_avr/cores/arduino/wiring.c  -I core_arduino_avr/variants/mega/ -o wiring.nolto.s

clang --target=avr  -S  -Os -Wall -Wextra  -std=gnu11 -ffunction-sections -fdata-sections -MMD -mmcu=atmega2560 -DF_CPU=16000000L ./core_arduino_avr/cores/arduino/wiring.c -flto -I core_arduino_avr/variants/mega/ -o wiring.lto.ll
clang --target=avr wiring.lto.ll -S -mmcu=atmega2560 -o wiring.lto.s
```

The generated two `.s` files are also same in functional code.

<!-- Failed to upload "wiring.lto.s.txt" -->

<!-- Failed to upload "wiring.nolto.s.txt" -->

so I guess, there is something wrong in the core libraries, other than `wire.c`.

---

### Comment 5 - benshi001

BTW: how did build arduino core library by clang? I used the attached script but failed.

[arduino.sh.txt](https://github.com/user-attachments/files/19268276/arduino.sh.txt)

The failure log is
```bash
In file included from /usr/share/arduino/hardware/arduino/avr/cores/arduino/CDC.cpp:20:
/usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/wdt.h:464:12: error: value '64' out of range for constraint 'I'
  464 |                                 : "I" (_SFR_IO_ADDR(_WD_CONTROL_REG)),
      |                                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/sfr_defs.h:183:27: note: expanded from macro '_SFR_IO_ADDR'
  183 | #define _SFR_IO_ADDR(sfr) (_SFR_MEM_ADDR(sfr) - __SFR_OFFSET)
      |                           ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from /usr/share/arduino/hardware/arduino/avr/cores/arduino/CDC.cpp:20:
/usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/wdt.h:507:23: error: value '64' out of range for constraint 'I'
  507 |                                 : [WDTREG]  "I"  (_SFR_IO_ADDR(_WD_CONTROL_REG)),
      |                                                   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/sfr_defs.h:183:27: note: expanded from macro '_SFR_IO_ADDR'
  183 | #define _SFR_IO_ADDR(sfr) (_SFR_MEM_ADDR(sfr) - __SFR_OFFSET)
      |                           ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
2 errors generated.
clang++: /home/wujie/benshi/llvm-project/clang/include/clang/CodeGen/CGFunctionInfo.h:185: static clang::CodeGen::ABIArgInfo clang::CodeGen::ABIArgInfo::getExtend(clang::QualType, llvm::Type*): Assertion `Ty->isIntegralOrEnumerationType() && "Unexpected QualType"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++ --target=avr -c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=10819 -DARDUINO_AVR_NANO -DARDUINO_ARCH_AVR -I/usr/share/arduino/hardware/arduino/avr/cores/arduino -I/usr/share/arduino/hardware/arduino/avr/variants/eightanaloginputs /usr/share/arduino/hardware/arduino/avr/cores/arduino/new.cpp -o /tmp/arduino_build_960004/core/new.cpp.o
1.	/usr/share/arduino/hardware/arduino/avr/cores/arduino/new.cpp:68:1: current parser token 'void'
2.	/usr/share/arduino/hardware/arduino/avr/cores/arduino/new.cpp:59:8: LLVM IR generation of declaration 'operator new'
3.	/usr/share/arduino/hardware/arduino/avr/cores/arduino/new.cpp:59:8: Generating code for declaration 'operator new'
 #0 0x00005aa720523900 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x27fd900)
 #1 0x00005aa7205201ff llvm::sys::RunSignalHandlers() (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x27fa1ff)
 #2 0x00005aa7203fbb38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d6b34e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d6b34e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d6b34e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d6b34e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d6b34e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d6b34e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00005aa720c5f084 (anonymous namespace)::AVRABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&) const AVR.cpp:0:0
#10 0x00005aa720ec80fa clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, clang::CodeGen::FnInfoOpts, llvm::ArrayRef<clang::CanQual<clang::Type>>, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x31a20fa)
#11 0x00005aa720ecc97b arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&, bool, llvm::SmallVectorImpl<clang::CanQual<clang::Type>>&, clang::CanQual<clang::FunctionProtoType>) CGCall.cpp:0:0
#12 0x00005aa720ecec25 clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::FunctionDecl const*) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x31a8c25)
#13 0x00005aa720b87325 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x2e61325)
#14 0x00005aa720b82884 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x2e5c884)
#15 0x00005aa720b82efb clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x2e5cefb)
#16 0x00005aa720b8bbd7 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#17 0x00005aa720fe0cd6 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#18 0x00005aa720fce981 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x32a8981)
#19 0x00005aa723021e94 clang::ParseAST(clang::Sema&, bool, bool) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x52fbe94)
#20 0x00005aa721376bd9 clang::FrontendAction::Execute() (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x3650bd9)
#21 0x00005aa7212d151e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x35ab51e)
#22 0x00005aa721496dc8 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x3770dc8)
#23 0x00005aa71ea333bb cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0xd0d3bb)
#24 0x00005aa71ea24e9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x00005aa72104211d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x00005aa7203fbfe0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x26d5fe0)
#27 0x00005aa72104476e clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x00005aa720fed8c1 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x32c78c1)
#29 0x00005aa720fee9e1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x32c89e1)
#30 0x00005aa720ffeeb4 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0x32d8eb4)
#31 0x00005aa71ea2b13e clang_main(int, char**, llvm::ToolContext const&) (/home/wujie/benshi/llvm-project/build/bin/clang-21+0xd0513e)
#32 0x00005aa71e95bb0b main (/home/wujie/benshi/llvm-project/build/bin/clang-21+0xc35b0b)
#33 0x00007d6b34e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#34 0x00007d6b34e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#35 0x00005aa71ea245c5 _start (/home/wujie/benshi/llvm-project/build/bin/clang-21+0xcfe5c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 21.0.0git (git@github.com:llvm/llvm-project.git 254951749febef42620242fd22dfba62f624f79f)
Target: avr
Thread model: posix
InstalledDir: /home/wujie/benshi/llvm-project/build/bin
Build config: +assertions, +expensive-checks
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/new-fd3c2f.cpp
clang++: note: diagnostic msg: /tmp/new-fd3c2f.sh
clang++: note: diagnostic msg: 

********************
/usr/lib/gcc/avr/5.4.0/../../../avr/bin/ar: /tmp/arduino_build_960004/core/CDC.cpp.o: 没有那个文件或目录
/usr/lib/gcc/avr/5.4.0/../../../avr/bin/ar: /tmp/arduino_build_960004/core/new.cpp.o: 没有那个文件或目录
```

---

### Comment 6 - benshi001

I think I should fix the above failure first, it indeed is clang's ICE, as the first step to let clang support arduino.

---

### Comment 7 - beakthoven

> I am a little confused, I have tried your attached two `.ll.txt` files, with
> 
> wujie@WUJIE-16:~/Downloads$ llc wiring.c-lto.ll.txt -mcpu=atmega2560 -o wiring.c-lto.s.txt
> wujie@WUJIE-16:~/Downloads$ llc wiring.c-nonlto.ll.txt -mcpu=atmega2560 wiring.c-nonlto.s.txt
> The generated assembly source files only have difference in debug information, but not functional code.
> 
> [wiring.c-lto.s.txt](https://github.com/user-attachments/files/19267729/wiring.c-lto.s.txt)
> 
> [wiring.c-nonlto.s.txt](https://github.com/user-attachments/files/19267728/wiring.c-nonlto.s.txt)

Then i think LTO optimizations made during link time are then likely the cause of the issue.  I'll try to upload LTO intermediate files shortly in order to debug this.

---

### Comment 8 - beakthoven

> In file included from /usr/share/arduino/hardware/arduino/avr/cores/arduino/CDC.cpp:20:
> /usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/wdt.h:464:12: error: value '64' out of range for constraint 'I'
>   464 |                                 : "I" (_SFR_IO_ADDR(_WD_CONTROL_REG)),
>       |                                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> /usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/sfr_defs.h:183:27: note: expanded from macro '_SFR_IO_ADDR'
>   183 | #define _SFR_IO_ADDR(sfr) (_SFR_MEM_ADDR(sfr) - __SFR_OFFSET)
>       |                           ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> In file included from /usr/share/arduino/hardware/arduino/avr/cores/arduino/CDC.cpp:20:
> /usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/wdt.h:507:23: error: value '64' out of range for constraint 'I'
>   507 |                                 : [WDTREG]  "I"  (_SFR_IO_ADDR(_WD_CONTROL_REG)),
>       |                                                   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> /usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/sfr_defs.h:183:27: note: expanded from macro '_SFR_IO_ADDR'
>   183 | #define _SFR_IO_ADDR(sfr) (_SFR_MEM_ADDR(sfr) - __SFR_OFFSET)
>       |                           ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> 2 errors generated.

The constraint error you have encountered is because of a code in AVR libc thats written in a way that makes use of gcc specific behavior.
avr-libc issue: https://github.com/avrdudes/avr-libc/issues/998
llvm issue: https://github.com/llvm/llvm-project/issues/51513

For time being i have applied the [following patch](https://github.com/ClangBuiltArduino/avr-libc/commit/b0ecbda693844f45214792cf9a04308b0a08efaf) in my fork of avr-libc to mitigate the constraint issue since there is no official fix from avr-libc.

However about to the clang crash, I haven't encountered it during my builds so i dont know what we're looking here.

> BTW: how did build arduino core library by clang?

Well i wrote this simple [bash script](https://github.com/ClangBuiltArduino/build-scripts/blob/main/build-mega.sh) to use during initial stage. Now i have completely moved to arduino's build system, the required changes for this were made to the build files in the core lib repo as of commit https://github.com/ClangBuiltArduino/core_arduino_avr/commit/96cc1dbd93c963f399443be82ad50cc4e6dd4347. This should be compatible with arduino-cli as well.

---

### Comment 9 - benshi001

> > I am a little confused, I have tried your attached two `.ll.txt` files, with
> > wujie@WUJIE-16:~/Downloads$ llc wiring.c-lto.ll.txt -mcpu=atmega2560 -o wiring.c-lto.s.txt
> > wujie@WUJIE-16:~/Downloads$ llc wiring.c-nonlto.ll.txt -mcpu=atmega2560 wiring.c-nonlto.s.txt
> > The generated assembly source files only have difference in debug information, but not functional code.
> > [wiring.c-lto.s.txt](https://github.com/user-attachments/files/19267729/wiring.c-lto.s.txt)
> > [wiring.c-nonlto.s.txt](https://github.com/user-attachments/files/19267728/wiring.c-nonlto.s.txt)
> 
> Then i think LTO optimizations made during link time are then likely the cause of the issue. I'll try to upload LTO intermediate files shortly in order to debug this.

It would be better if you can give the final linked file in the form of

1. LTO linked in IR
2. LTO linked in assembly
3. not LTO linked in IR
4. not LTO linked in assembly 

---

### Comment 10 - benshi001

> > In file included from /usr/share/arduino/hardware/arduino/avr/cores/arduino/CDC.cpp:20:
> > /usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/wdt.h:464:12: error: value '64' out of range for constraint 'I'
> > 464 |                                 : "I" (_SFR_IO_ADDR(_WD_CONTROL_REG)),
> > |                                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> > /usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/sfr_defs.h:183:27: note: expanded from macro '_SFR_IO_ADDR'
> > 183 | #define _SFR_IO_ADDR(sfr) (_SFR_MEM_ADDR(sfr) - __SFR_OFFSET)
> > |                           ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> > In file included from /usr/share/arduino/hardware/arduino/avr/cores/arduino/CDC.cpp:20:
> > /usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/wdt.h:507:23: error: value '64' out of range for constraint 'I'
> > 507 |                                 : [WDTREG]  "I"  (_SFR_IO_ADDR(_WD_CONTROL_REG)),
> > |                                                   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> > /usr/lib/gcc/avr/5.4.0/../../../avr/include/avr/sfr_defs.h:183:27: note: expanded from macro '_SFR_IO_ADDR'
> > 183 | #define _SFR_IO_ADDR(sfr) (_SFR_MEM_ADDR(sfr) - __SFR_OFFSET)
> > |                           ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
> > 2 errors generated.
> 
> The constraint error you have encountered is because of a code in AVR libc thats written in a way that makes use of gcc specific behavior. avr-libc issue: [avrdudes/avr-libc#998](https://github.com/avrdudes/avr-libc/issues/998) llvm issue: [#51513](https://github.com/llvm/llvm-project/issues/51513)
> 
> For time being i have applied the [following patch](https://github.com/ClangBuiltArduino/avr-libc/commit/b0ecbda693844f45214792cf9a04308b0a08efaf) in my fork of avr-libc to mitigate the constraint issue since there is no official fix from avr-libc.
> 
> However about to the clang crash, I haven't encountered it during my builds so i dont know what we're looking here.
> 
> > BTW: how did build arduino core library by clang?
> 
> Well i wrote this simple [bash script](https://github.com/ClangBuiltArduino/build-scripts/blob/main/build-mega.sh) to use during initial stage. Now i have completely moved to arduino's build system, the required changes for this were made to the build files in the core lib repo as of commit [ClangBuiltArduino/core_arduino_avr@96cc1db](https://github.com/ClangBuiltArduino/core_arduino_avr/commit/96cc1dbd93c963f399443be82ad50cc4e6dd4347). This should be compatible with arduino-cli as well.

Thanks for your clarification, I would try

1. apply your patch to dismiss the first error
2. fix the second error
3. reproduce the bug you encountered

---

### Comment 11 - benshi001

I have fixed the first crash, as patch https://github.com/llvm/llvm-project/pull/131976,

but I encountered a second crash, as reported in https://github.com/llvm/llvm-project/issues/132203

---

### Comment 12 - benshi001

@dakkshesh07 , may I ask

1. Is there a instruction about how to build your `ClangBuiltArduino` project? Or I simply follow the arduino CLI way  ？
2. which version of  clang /llvm are you using? Is yours the newest main branch ? 

I still failed build the simplest blink program with the above two crashes.

---

### Comment 13 - beakthoven

> 1. Is there a instruction about how to build your `ClangBuiltArduino` project? Or I simply follow the arduino CLI way  ？

You can directly add the custom board package json and then install the custom board support to use it.

```shell
arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/ClangBuiltArduino/BoardManagerFiles/refs/heads/main/package_clangbuiltarduino_index.json
arduino-cli core update-index
arduino-cli core install ClangBuiltArduino:avr
arduino-cli compile -b ClangBuiltArduino:avr:mega $dir
```
> 3. which version of  clang /llvm are you using? Is yours the newest main branch ?

I haven't updated the json since 20.1.0-rc2 build, I have a 20.1.0 build ready to be used. I will update the json and try once.
I will also update toolchain scripts to add the ability to build off of main branch instead of release tarballs so that we can test bleeding edge builds.

Btw can you take a look at my llvm build configuration, maybe i disabled some crucial feature so I'm not getting those crashes?
https://github.com/ClangBuiltArduino/tc-build/blob/main/llvm/build-llvm-stage2.sh#L63-L223

> I still failed build the simplest blink program with the above two crashes.

I will try to use the latest 20.1.0 build or main branch build to see if i can hit those crashes.




---

### Comment 14 - beakthoven

I have updated the toolchain to 20.1.0 as of commit:
https://github.com/ClangBuiltArduino/BoardManagerFiles/commit/9584288805fe065b8fcb8779feec24d7ce2ab40d

```shell
$ arduino-cli compile --clean -b ClangBuiltArduino:avr:mega Blink

Sketch uses 1418 bytes (0%) of program storage space. Maximum is 253952 bytes.
Global variables use 9 bytes (0%) of dynamic memory, leaving 8183 bytes for local variables. Maximum is 8192 bytes.

Used platform         Version Path
ClangBuiltArduino:avr 1.0.1   /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1
```

---

### Comment 15 - beakthoven

> I have fixed the first crash, as patch [#131976](https://github.com/llvm/llvm-project/pull/131976),

I'll try building off of main branch once this patch is merged.

---

### Comment 16 - benshi001

I have fixed the 2nd crash as https://github.com/llvm/llvm-project/pull/132398, but a third error (a link failure) rised. I never thought building the simplest blink with clang is such that hard.


```bash
/data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(HardwareSerial.cpp.o): In function `serialEventRun()':
/data/home/bennshi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/HardwareSerial.cpp:(.text._Z14serialEventRunv+0xa): undefined reference to `serialEvent()'
/data/home/bennshi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/HardwareSerial.cpp:(.text._Z14serialEventRunv+0xc): undefined reference to `serialEvent()'
/data/home/bennshi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/HardwareSerial.cpp:67: undefined reference to `serialEvent()'
/data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(HardwareSerial.cpp.o):(.rodata._ZTI14HardwareSerial+0x0): undefined reference to `vtable for __cxxabiv1::__si_class_type_info'
/data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(HardwareSerial.cpp.o):(.rodata._ZTI6Stream[_ZTI6Stream]+0x0): undefined reference to `vtable for __cxxabiv1::__si_class_type_info'
/data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(Print.cpp.o):(.rodata._ZTI5Print+0x0): undefined reference to `vtable for __cxxabiv1::__class_type_info'
clang: error: avr-ld command failed with exit code 1 (use -v to see invocation)
```



---

### Comment 17 - beakthoven

> /data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(HardwareSerial.cpp.o): In function `serialEventRun()':
> /data/home/bennshi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/HardwareSerial.cpp:(.text._Z14serialEventRunv+0xa): undefined reference to `serialEvent()'
> /data/home/bennshi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/HardwareSerial.cpp:(.text._Z14serialEventRunv+0xc): undefined reference to `serialEvent()'
> /data/home/bennshi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/HardwareSerial.cpp:67: undefined reference to `serialEvent()'

These errors should be fixed using the following commits:
https://github.com/ClangBuiltArduino/core_arduino_avr/commit/698ad5d0f7b6f5147d01fd4c0a352d21bc6d5899
https://github.com/ClangBuiltArduino/core_arduino_avr/commit/9602bbb2523eedbe1e8c7459659e3eb00781b000

> /data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(HardwareSerial.cpp.o):(.rodata._ZTI14HardwareSerial+0x0): undefined reference to `vtable for __cxxabiv1::__si_class_type_info'
> /data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(HardwareSerial.cpp.o):(.rodata._ZTI6Stream[_ZTI6Stream]+0x0): undefined reference to `vtable for __cxxabiv1::__si_class_type_info'
> /data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(Print.cpp.o):(.rodata._ZTI5Print+0x0): undefined reference to `vtable for __cxxabiv1::__class_type_info'
> clang: error: avr-ld command failed with exit code 1 (use -v to see invocation)

This error should be fixed by using the `-fno-rtti` compiler flag

> I have fixed the 2nd crash as [#132398](https://github.com/llvm/llvm-project/pull/132398), but a third error (a link failure) rised. I never thought building the simplest blink with clang is such that hard.

It indeed is, i suggest you to use my fork of the [core lib](https://github.com/ClangBuiltArduino/core_arduino_avr) instead which already contains all required fixes.

---

### Comment 18 - beakthoven

I just used a llvm built off of main branch at commit c51d396f4dec65bac24e00ace3f5ce5582d2c699 with https://github.com/llvm/llvm-project/pull/132398 applied. The compiler was able to compile the blink sketch with no issues even with expensive checks enabled but the program breakage still exists, with LTO enabled globally the LED blinks once and then gets stuck in ON state forever. Please let me know if you need any other information.

<details>
<summary> Build logs </summary>

```shell
$ arduino-cli compile -v --clean -b ClangBuiltArduino:avr:mega Blink

FQBN: ClangBuiltArduino:avr:mega
Using board 'mega' from platform in folder: /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1
Using core 'arduino' from platform in folder: /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1

Detecting libraries used...
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -flto -w -x c++ -E -CC -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/sketch/Blink.ino.cpp -o /dev/null
Generating function prototypes...
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -flto -w -x c++ -E -CC -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/sketch/Blink.ino.cpp -o /tmp/3623586140/sketch_merged.cpp
/home/dakkshesh/.arduino15/packages/builtin/tools/ctags/5.8-arduino11/ctags -u --language-force=c++ -f - --c++-kinds=svpf --fields=KSTtzns --line-directives /tmp/3623586140/sketch_merged.cpp
Compiling sketch...
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/sketch/Blink.ino.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/sketch/Blink.ino.cpp.o
Compiling libraries...
Compiling core...
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/HardwareSerial2.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial2.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/CDC.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/CDC.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/HardwareSerial0.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial0.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/HardwareSerial3.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial3.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/PluggableUSB.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/PluggableUSB.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/HardwareSerial1.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial1.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/HardwareSerial.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/IPAddress.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/IPAddress.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/Print.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/Print.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/Stream.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/Stream.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/Tone.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/Tone.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/USBCore.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/USBCore.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/WMath.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/WMath.cpp.o
/home/dakkshesh/install/bin/clang --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/WInterrupts.c -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/WInterrupts.c.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/WString.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/WString.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/abi.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/abi.cpp.o
/home/dakkshesh/install/bin/clang --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/hooks.c -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/hooks.c.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/main.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/main.cpp.o
/home/dakkshesh/install/bin/clang++ --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fno-rtti -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/new.cpp -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/new.cpp.o
/home/dakkshesh/install/bin/clang --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/wiring.c -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring.c.o
/home/dakkshesh/install/bin/clang --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/wiring_analog.c -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_analog.c.o
/home/dakkshesh/install/bin/clang --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/wiring_digital.c -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_digital.c.o
/home/dakkshesh/install/bin/clang --target=avr --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -c -g -x assembler-with-cpp -flto -MMD -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/wiring_pulse.S -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_pulse.S.o
/home/dakkshesh/install/bin/clang --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/wiring_pulse.c -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_pulse.c.o
/home/dakkshesh/install/bin/clang --target=avr -c -g -Os -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10607 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino -I/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/variants/mega /home/dakkshesh/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1/cores/arduino/wiring_shift.c -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_shift.c.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/CDC.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial0.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial1.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial2.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/HardwareSerial3.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/IPAddress.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/PluggableUSB.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/Print.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/Stream.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/Tone.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/USBCore.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/WInterrupts.c.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/WMath.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/WString.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/abi.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/hooks.c.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/main.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/new.cpp.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring.c.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_analog.c.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_digital.c.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_pulse.S.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_pulse.c.o
/home/dakkshesh/install/bin/llvm-ar rcs /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/wiring_shift.c.o
Archiving built core (caching) in: /home/dakkshesh/.cache/arduino/cores/ClangBuiltArduino_avr_mega_116424f3ccbe01a19dd3deb05bebea6b/core.a
Linking everything together...
/home/dakkshesh/install/bin/clang++ --target=avr -w --sysroot=/home/dakkshesh/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-sysroot/12022025 -Os -g -flto -fuse-linker-plugin -ffunction-sections -fdata-sections -Wl,--gc-sections -mmcu=atmega2560 -o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/Blink.ino.elf /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/sketch/Blink.ino.cpp.o /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/core/core.a -L/home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7 -lm
/home/dakkshesh/install/bin/llvm-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/Blink.ino.elf /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/Blink.ino.eep
/home/dakkshesh/install/bin/llvm-objcopy -O ihex -R .eeprom /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/Blink.ino.elf /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/Blink.ino.hex

/home/dakkshesh/install/bin/llvm-size -A /home/dakkshesh/.cache/arduino/sketches/0A3424900B1B32407700B6A1409F8FE7/Blink.ino.elf
Sketch uses 1328 bytes (0%) of program storage space. Maximum is 253952 bytes.
Global variables use 9 bytes (0%) of dynamic memory, leaving 8183 bytes for local variables. Maximum is 8192 bytes.
```
</details>


Looking at the LED behavior i think the issue could be isolated to the interrupts or the delay timer? Im not sure though.

---

### Comment 19 - benshi001

> > /data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(HardwareSerial.cpp.o): In function `serialEventRun()': /data/home/bennshi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/HardwareSerial.cpp:(.text._Z14serialEventRunv+0xa): undefined reference to `serialEvent()'
> > /data/home/bennshi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/HardwareSerial.cpp:(.text._Z14serialEventRunv+0xc): undefined reference to `serialEvent()' /data/home/bennshi/.arduino15/packages/arduino/hardware/avr/1.8.6/cores/arduino/HardwareSerial.cpp:67: undefined reference to `serialEvent()'
> 
> These errors should be fixed using the following commits: [ClangBuiltArduino/core_arduino_avr@698ad5d](https://github.com/ClangBuiltArduino/core_arduino_avr/commit/698ad5d0f7b6f5147d01fd4c0a352d21bc6d5899) [ClangBuiltArduino/core_arduino_avr@9602bbb](https://github.com/ClangBuiltArduino/core_arduino_avr/commit/9602bbb2523eedbe1e8c7459659e3eb00781b000)
> 
> > /data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(HardwareSerial.cpp.o):(.rodata._ZTI14HardwareSerial+0x0): undefined reference to `vtable for __cxxabiv1::__si_class_type_info' /data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(HardwareSerial.cpp.o):(.rodata._ZTI6Stream[_ZTI6Stream]+0x0): undefined reference to `vtable for __cxxabiv1::__si_class_type_info'
> > /data/home/bennshi/.cache/arduino/sketches/CEE06FE2D07E31C0BC8B101357BC67E2/core/core.a(Print.cpp.o):(.rodata._ZTI5Print+0x0): undefined reference to `vtable for __cxxabiv1::__class_type_info'
> > clang: error: avr-ld command failed with exit code 1 (use -v to see invocation)
> 
> This error should be fixed by using the `-fno-rtti` compiler flag
> 
> > I have fixed the 2nd crash as [#132398](https://github.com/llvm/llvm-project/pull/132398), but a third error (a link failure) rised. I never thought building the simplest blink with clang is such that hard.
> 
> It indeed is, i suggest you to use my fork of the [core lib](https://github.com/ClangBuiltArduino/core_arduino_avr) instead which already contains all required fixes.

thanks. I will try your fork.

---

### Comment 20 - benshi001

> /home/dakkshesh/install/bin/llvm-ar rcs

BTW, I saw `/home/dakkshesh/install/bin/llvm-ar` is used during your build process, I guess it may be 
 the reason why LTO failed? because `clang -flto` emits LLVM byte code instead of relocatable ELF (which `avr-gcc` emits), so you have to use `llvm-link` instead of `llvm-ar` ? 

---

### Comment 21 - beakthoven

> > /home/dakkshesh/install/bin/llvm-ar rcs
> 
> BTW, I saw `/home/dakkshesh/install/bin/llvm-ar` is used during your build process, I guess it may be the reason why LTO failed? because `clang -flto` emits LLVM byte code instead of relocatable ELF (which `avr-gcc` emits), so you have to use `llvm-link` instead of `llvm-ar` ?

I believe llvm-ar can handle LLVM bitcode? According to the [docs](https://github.com/llvm/llvm-project/blob/441c9a9273c5d040f218986bf301c8a5c4d4c011/llvm/docs/CommandGuide/llvm-ar.rst)

> The :program:`llvm-ar` command is similar to the common Unix utility, :program:`ar`. It archives several files, such as objects and LLVM bitcode files into a single archive library that can be linked into a program.

---

### Comment 22 - benshi001

> > > /home/dakkshesh/install/bin/llvm-ar rcs
> > 
> > 
> > BTW, I saw `/home/dakkshesh/install/bin/llvm-ar` is used during your build process, I guess it may be the reason why LTO failed? because `clang -flto` emits LLVM byte code instead of relocatable ELF (which `avr-gcc` emits), so you have to use `llvm-link` instead of `llvm-ar` ?
> 
> I believe llvm-ar can handle LLVM bitcode? According to the [docs](https://github.com/llvm/llvm-project/blob/441c9a9273c5d040f218986bf301c8a5c4d4c011/llvm/docs/CommandGuide/llvm-ar.rst)
> 
> > The :program:`llvm-ar` command is similar to the common Unix utility, :program:`ar`. It archives several files, such as objects and LLVM bitcode files into a single archive library that can be linked into a program.

I see. Your are correct 👍

---

### Comment 23 - beakthoven

Here are the bitcode and IR LTO intermediate files for the Blink sketch, these should help us analyze what LTO is doing.

[Blink-intermediate.zip](https://github.com/user-attachments/files/19407335/Blink-intermediate.zip)

---

### Comment 24 - benshi001

> > 1. Is there a instruction about how to build your `ClangBuiltArduino` project? Or I simply follow the arduino CLI way  ？
> 
> You can directly add the custom board package json and then install the custom board support to use it.
> 
> arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/ClangBuiltArduino/BoardManagerFiles/refs/heads/main/package_clangbuiltarduino_index.json
> arduino-cli core update-index
> arduino-cli core install ClangBuiltArduino:avr
> arduino-cli compile -b ClangBuiltArduino:avr:mega $dir
> > 3. which version of  clang /llvm are you using? Is yours the newest main branch ?
> 
> I haven't updated the json since 20.1.0-rc2 build, I have a 20.1.0 build ready to be used. I will update the json and try once. I will also update toolchain scripts to add the ability to build off of main branch instead of release tarballs so that we can test bleeding edge builds.
> 
> Btw can you take a look at my llvm build configuration, maybe i disabled some crucial feature so I'm not getting those crashes? https://github.com/ClangBuiltArduino/tc-build/blob/main/llvm/build-llvm-stage2.sh#L63-L223
> 
> > I still failed build the simplest blink program with the above two crashes.
> 
> I will try to use the latest 20.1.0 build or main branch build to see if i can hit those crashes.

Unfortunately I got the following error with your tools.
```[bennshi@VM-78-142-tencentos ~]$ ./bin/arduino-cli compile -b ClangBuiltArduino:avr:mega ~/MyFirstSketch/
/data/home/bennshi/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-bfd/2.44-14022025/glibc/bin/avr-ld: /data/home/bennshi/.arduino15/packages/ClangBuiltArduino/tools/cba-llvm/20.1.0-06032025/bin/../lib/LLVMgold.so: error loading plugin: /lib64/libm.so.6: version `GLIBC_2.29' not found (required by /data/home/bennshi/.arduino15/packages/ClangBuiltArduino/tools/cba-llvm/20.1.0-06032025/bin/../lib/LLVMgold.so)
clang++: error: avr-ld command failed with exit code 1 (use -v to see invocation)
Error: exit status 1

Used platform         Version Path
ClangBuiltArduino:avr 1.0.1   /data/home/bennshi/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1
Error during build: exit status 1
```

My CentOS-7 is too old.

---

### Comment 25 - beakthoven

> > > 1. Is there a instruction about how to build your `ClangBuiltArduino` project? Or I simply follow the arduino CLI way  ？
> > 
> > 
> > You can directly add the custom board package json and then install the custom board support to use it.
> > arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/ClangBuiltArduino/BoardManagerFiles/refs/heads/main/package_clangbuiltarduino_index.json
> > arduino-cli core update-index
> > arduino-cli core install ClangBuiltArduino:avr
> > arduino-cli compile -b ClangBuiltArduino:avr:mega $dir
> > > 3. which version of  clang /llvm are you using? Is yours the newest main branch ?
> > 
> > 
> > I haven't updated the json since 20.1.0-rc2 build, I have a 20.1.0 build ready to be used. I will update the json and try once. I will also update toolchain scripts to add the ability to build off of main branch instead of release tarballs so that we can test bleeding edge builds.
> > Btw can you take a look at my llvm build configuration, maybe i disabled some crucial feature so I'm not getting those crashes? https://github.com/ClangBuiltArduino/tc-build/blob/main/llvm/build-llvm-stage2.sh#L63-L223
> > > I still failed build the simplest blink program with the above two crashes.
> > 
> > 
> > I will try to use the latest 20.1.0 build or main branch build to see if i can hit those crashes.
> 
> Unfortunately I got the following error with your tools.
> 
> ```
> /data/home/bennshi/.arduino15/packages/ClangBuiltArduino/tools/cba-avr-bfd/2.44-14022025/glibc/bin/avr-ld: /data/home/bennshi/.arduino15/packages/ClangBuiltArduino/tools/cba-llvm/20.1.0-06032025/bin/../lib/LLVMgold.so: error loading plugin: /lib64/libm.so.6: version `GLIBC_2.29' not found (required by /data/home/bennshi/.arduino15/packages/ClangBuiltArduino/tools/cba-llvm/20.1.0-06032025/bin/../lib/LLVMgold.so)
> clang++: error: avr-ld command failed with exit code 1 (use -v to see invocation)
> Error: exit status 1
> 
> Used platform         Version Path
> ClangBuiltArduino:avr 1.0.1   /data/home/bennshi/.arduino15/packages/ClangBuiltArduino/hardware/avr/1.0.1
> Error during build: exit status 1
> ```
> 
> My CentOS-7 is too old.

Ah thats a bummer, i compiled them against debian bookworm libs just to be sure i get good compatibility but i think thats not enough.
The main toolchain is statically linked against musl libc so I dont think you would face any issues with it, however the bfd linker `avr-ld` and the `LLVMgold.so` are dynamically linked to support loading plugins so these must be compatible with your old libc.

I could try building the dynamically linked programs against older libs to have more compatibility. Or you could edit the platform.txt in the core lib dir to point to your tools instead so that you end up compiling my fork of the core lib with fixes but use your tools to compile it. 

---

### Comment 26 - beakthoven

This mess could be properly mitigated later once LLD supports AVR linkerscript syntax.

---

### Comment 27 - benshi001

could  you  please  try to add option `-mno-relax` to clang in your config file? I find another bug triggered by this option, to avoid this bug   `-mno-relax` is needed when calling `avr-ld`.

I am not sure if this option affect LTO result, maybe you can have a try, and let me known your result.

---

### Comment 28 - benshi001

I have created another issue https://github.com/llvm/llvm-project/issues/133579 for that.

---

### Comment 29 - benshi001

@beakthoven I really appreciate you, since I never thought llvm-avr can be such that fragile !!!

---

### Comment 30 - beakthoven

> could you please try to add option `-mno-relax` to clang in your config file? I find another bug triggered by this option, to avoid this bug `-mno-relax` is needed when calling `avr-ld`.
> 
> I am not sure if this option affect LTO result, maybe you can have a try, and let me known your result.

Well it actually did fix the issue with LTO enabled! I tried it on both 20.1.1 release build as well as on a bleeding edge build with recent bug fixes, It seems to work fine on both builds when `-mno-relax` is passed during the final linking phase.

I also tried adding back the serial functions to see if #128536 got resolved as well, but that issue still persists. Regardless, this is a significant step forward. I'll keep an eye on your follow-up issue and see if I can help debug anything.

> [@beakthoven](https://github.com/beakthoven) I really appreciate you, since I never thought llvm-avr can be such that fragile !!!

Yeah! This project has definitely been full of surprises, but at least we’re making progress. Appreciate your effort in digging into these issues!

---

