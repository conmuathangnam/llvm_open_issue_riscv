# <eof> Parser at end of file, segmentation fault

**Author:** sigurd4
**URL:** https://github.com/llvm/llvm-project/issues/123770

## Body

Compiler crash. Full logs are added in attachment. I've only added the first crash below.

# System info

OS: Arch Linux x86_64
Kernel: 6.12.9-arch1-1
Toolchain:
- llvm-minimal-git 20.0.0_r524521.4b73f6a54884-1
- llvm-libs-minimal-git 20.0.0_r524521.4b73f6a54884-1
- clang-libs-minimal-git 20.0.0_r524521.4b73f6a54884-1
- clang-minimal-git 20.0.0_r524521.4b73f6a54884-1
- libc++-modules 19.1.6-1
- libc++abi-modules 19.1.6-1
- lld 19.1.6-1

# Crash logs

<details>

```
[main] Building folder: /home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug main
[build] Starting build
[proc] Executing command: /usr/bin/cmake --build /home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug --config Debug --target main --
[build] [1/2   0% :: 0.003] Re-checking globbed directories...
[build] [1/24   4% :: 0.540] Scanning '/home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug/_deps/std-src/std.cppm' for CXX dependencies
[build] [2/24   8% :: 0.561] Generating CXX dyndep file _deps/std-build/CMakeFiles/std.dir/CXX.dd
[build] [4/24  12% :: 0.588] Generating CXX dyndep file CMakeFiles/main.dir/CXX.dd
[build] [13/24  16% :: 1.688] Building CXX object CMakeFiles/main.dir/src/units/si/si_area.cppm.o
[build] FAILED: CMakeFiles/main.dir/src/units/si/si_area.cppm.o CMakeFiles/main.dir/units-si_area.pcm 
[build] /usr/bin/clang++   -stdlib=libc++ -g -std=gnu++23 -isystem/usr/include/c++/v1 "-isystem/usr/include/c++/v1/*" -Wall -Wextra -pedantic -Wpedantic -Wsign-conversion -Wconversion -Werror=switch -fdiagnostics-color=always -Wdouble-promotion -Wnull-dereference -Winfinite-recursion -Wmisleading-indentation -g -Werror=return-local-addr -Wno-c++23-extensions -Xclang -emit-module-interface -fmodules -stdlib=libc++ -fbuiltin-module-map -fimplicit-module-maps -fprebuilt-module-path=/home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug/modules -fretain-comments-from-system-headers -Wstrict-overflow -MD -MT CMakeFiles/main.dir/src/units/si/si_area.cppm.o -MF CMakeFiles/main.dir/src/units/si/si_area.cppm.o.d @CMakeFiles/main.dir/src/units/si/si_area.cppm.o.modmap -o CMakeFiles/main.dir/src/units/si/si_area.cppm.o -c '/home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/src/units/si/si_area.cppm'
[build] PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang-20 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name si_area.cppm -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/sigurd/Code/c++/programmering2/2-4_h\303\270yde_p\303\245_tree/build/debug -fcoverage-compilation-dir=/home/sigurd/Code/c++/programmering2/2-4_h\303\270yde_p\303\245_tree/build/debug -resource-dir /usr/lib/clang/20 -Wall -Wextra -Wpedantic -Wsign-conversion -Wconversion -Werror=switch -Wdouble-promotion -Wnull-dereference -Winfinite-recursion -Wmisleading-indentation -Werror=return-local-addr -Wno-c++23-extensions -Wstrict-overflow -pedantic -std=gnu++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fmodules -fimplicit-module-maps -fmodules-cache-path=/home/sigurd/.cache/clang/ModuleCache -fmodule-map-file=/usr/lib/clang/20/include/module.modulemap -fprebuilt-module-path=/home/sigurd/Code/c++/programmering2/2-4_h\303\270yde_p\303\245_tree/build/debug/modules -fmodules-validate-system-headers -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fretain-comments-from-system-headers -emit-module-interface -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o CMakeFiles/main.dir/src/units/si/si_area.cppm.o -x pcm CMakeFiles/main.dir/units-si_area.pcm
[build] 1.	<eof> parser at end of file
[build]  #0 0x0000772922d78ea3 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/bin/../lib/libLLVM.so.20.0+0x2778ea3)
[build]  #1 0x0000772922d76a1f llvm::sys::RunSignalHandlers() (/usr/bin/../lib/libLLVM.so.20.0+0x2776a1f)
[build]  #2 0x0000772922d796b6 (/usr/bin/../lib/libLLVM.so.20.0+0x27796b6)
[build]  #3 0x000077292044c1d0 (/usr/bin/../lib/libc.so.6+0x3d1d0)
[build]  #4 0x0000772928f50bf9 clang::ASTWriter::WriteSubmodules(clang::Module*, clang::ASTContext*) (/usr/bin/../lib/libclang-cpp.so.20.0+0x3150bf9)
[build]  #5 0x0000772928f67d63 clang::ASTWriter::WriteASTCore(clang::Sema*, llvm::StringRef, clang::Module*) (/usr/bin/../lib/libclang-cpp.so.20.0+0x3167d63)
[build]  #6 0x0000772928f6640e clang::ASTWriter::WriteAST(llvm::PointerUnion<clang::Sema*, clang::Preprocessor*>, llvm::StringRef, clang::Module*, llvm::StringRef, bool) (/usr/bin/../lib/libclang-cpp.so.20.0+0x316640e)
[build]  #7 0x0000772928fbd7cf clang::PCHGenerator::HandleTranslationUnit(clang::ASTContext&) (/usr/bin/../lib/libclang-cpp.so.20.0+0x31bd7cf)
[build]  #8 0x0000772928fbd9d1 clang::CXX20ModulesGenerator::HandleTranslationUnit(clang::ASTContext&) (/usr/bin/../lib/libclang-cpp.so.20.0+0x31bd9d1)
[build]  #9 0x00007729291425ac clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/bin/../lib/libclang-cpp.so.20.0+0x33425ac)
[build] #10 0x0000772926e87e88 clang::ParseAST(clang::Sema&, bool, bool) (/usr/bin/../lib/libclang-cpp.so.20.0+0x1087e88)
[build] #11 0x0000772929101f65 clang::FrontendAction::Execute() (/usr/bin/../lib/libclang-cpp.so.20.0+0x3301f65)
[build] #12 0x000077292907c395 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/bin/../lib/libclang-cpp.so.20.0+0x327c395)
[build] #13 0x0000772929183e4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/bin/../lib/libclang-cpp.so.20.0+0x3383e4d)
[build] #14 0x0000595babd1b4ba cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang-20+0x104ba)
[build] #15 0x0000595babd184a1 (/usr/bin/clang-20+0xd4a1)
[build] #16 0x0000595babd1745c clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang-20+0xc45c)
[build] #17 0x0000595babd254da main (/usr/bin/clang-20+0x1a4da)
[build] #18 0x0000772920434e08 (/usr/bin/../lib/libc.so.6+0x25e08)
[build] #19 0x0000772920434ecc __libc_start_main (/usr/bin/../lib/libc.so.6+0x25ecc)
[build] #20 0x0000595babd15e75 _start (/usr/bin/clang-20+0xae75)
[build] clang++: error: unable to execute command: Segmentation fault (core dumped)
[build] clang++: error: clang frontend command failed due to signal (use -v to see invocation)
[build] clang version 20.0.0 (/home/sigurd/.cache/yay/llvm-minimal-git/llvm-project 4b73f6a54884b6a34fbff16b5e24b7a2e480ebcb)
[build] Target: x86_64-pc-linux-gnu
[build] Thread model: posix
[build] InstalledDir: /usr/bin
[build] clang++: note: diagnostic msg: 
[build] ********************
[build] 
[build] PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
[build] Preprocessed source(s) and associated run script(s) are located at:
[build] clang++: note: diagnostic msg: /tmp/si_area-71eeab.cppm
[build] clang++: note: diagnostic msg: /tmp/si_area-71eeab.cache
[build] clang++: note: diagnostic msg: /tmp/si_area-71eeab.sh
[build] clang++: note: diagnostic msg: 
[build] 
[build] ********************
[build] [13/24  20% :: 2.513] Building CXX object CMakeFiles/main.dir/src/consts/numeric.cppm.o
```
<details/>

[log.txt](https://github.com/user-attachments/files/18493342/log.txt)

[crash.zip](https://github.com/user-attachments/files/18493433/crash.zip)

## Comments

### Comment 1 - EugeneZelenko

Could you please provide reproducer? https://godbolt.org should be helpful.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: sigurd4 (sigurd4)

<details>
Compiler crash. Full logs are added in attachment. I've only added the first crash below.

# System info

OS: Arch Linux x86_64
Kernel: 6.12.9-arch1-1
Toolchain:
- llvm-minimal-git 20.0.0_r524521.4b73f6a54884-1
- llvm-libs-minimal-git 20.0.0_r524521.4b73f6a54884-1
- clang-libs-minimal-git 20.0.0_r524521.4b73f6a54884-1
- clang-minimal-git 20.0.0_r524521.4b73f6a54884-1
- libc++-modules 19.1.6-1
- libc++abi-modules 19.1.6-1
- lld 19.1.6-1

# Crash logs

&lt;details&gt;

```
[main] Building folder: /home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug main
[build] Starting build
[proc] Executing command: /usr/bin/cmake --build /home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug --config Debug --target main --
[build] [1/2   0% :: 0.003] Re-checking globbed directories...
[build] [1/24   4% :: 0.540] Scanning '/home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug/_deps/std-src/std.cppm' for CXX dependencies
[build] [2/24   8% :: 0.561] Generating CXX dyndep file _deps/std-build/CMakeFiles/std.dir/CXX.dd
[build] [4/24  12% :: 0.588] Generating CXX dyndep file CMakeFiles/main.dir/CXX.dd
[build] [13/24  16% :: 1.688] Building CXX object CMakeFiles/main.dir/src/units/si/si_area.cppm.o
[build] FAILED: CMakeFiles/main.dir/src/units/si/si_area.cppm.o CMakeFiles/main.dir/units-si_area.pcm 
[build] /usr/bin/clang++   -stdlib=libc++ -g -std=gnu++23 -isystem/usr/include/c++/v1 "-isystem/usr/include/c++/v1/*" -Wall -Wextra -pedantic -Wpedantic -Wsign-conversion -Wconversion -Werror=switch -fdiagnostics-color=always -Wdouble-promotion -Wnull-dereference -Winfinite-recursion -Wmisleading-indentation -g -Werror=return-local-addr -Wno-c++23-extensions -Xclang -emit-module-interface -fmodules -stdlib=libc++ -fbuiltin-module-map -fimplicit-module-maps -fprebuilt-module-path=/home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug/modules -fretain-comments-from-system-headers -Wstrict-overflow -MD -MT CMakeFiles/main.dir/src/units/si/si_area.cppm.o -MF CMakeFiles/main.dir/src/units/si/si_area.cppm.o.d @<!-- -->CMakeFiles/main.dir/src/units/si/si_area.cppm.o.modmap -o CMakeFiles/main.dir/src/units/si/si_area.cppm.o -c '/home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/src/units/si/si_area.cppm'
[build] PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang-20 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name si_area.cppm -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/sigurd/Code/c++/programmering2/2-4_h\303\270yde_p\303\245_tree/build/debug -fcoverage-compilation-dir=/home/sigurd/Code/c++/programmering2/2-4_h\303\270yde_p\303\245_tree/build/debug -resource-dir /usr/lib/clang/20 -Wall -Wextra -Wpedantic -Wsign-conversion -Wconversion -Werror=switch -Wdouble-promotion -Wnull-dereference -Winfinite-recursion -Wmisleading-indentation -Werror=return-local-addr -Wno-c++23-extensions -Wstrict-overflow -pedantic -std=gnu++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fmodules -fimplicit-module-maps -fmodules-cache-path=/home/sigurd/.cache/clang/ModuleCache -fmodule-map-file=/usr/lib/clang/20/include/module.modulemap -fprebuilt-module-path=/home/sigurd/Code/c++/programmering2/2-4_h\303\270yde_p\303\245_tree/build/debug/modules -fmodules-validate-system-headers -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fretain-comments-from-system-headers -emit-module-interface -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o CMakeFiles/main.dir/src/units/si/si_area.cppm.o -x pcm CMakeFiles/main.dir/units-si_area.pcm
[build] 1.	&lt;eof&gt; parser at end of file
[build]  #<!-- -->0 0x0000772922d78ea3 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/bin/../lib/libLLVM.so.20.0+0x2778ea3)
[build]  #<!-- -->1 0x0000772922d76a1f llvm::sys::RunSignalHandlers() (/usr/bin/../lib/libLLVM.so.20.0+0x2776a1f)
[build]  #<!-- -->2 0x0000772922d796b6 (/usr/bin/../lib/libLLVM.so.20.0+0x27796b6)
[build]  #<!-- -->3 0x000077292044c1d0 (/usr/bin/../lib/libc.so.6+0x3d1d0)
[build]  #<!-- -->4 0x0000772928f50bf9 clang::ASTWriter::WriteSubmodules(clang::Module*, clang::ASTContext*) (/usr/bin/../lib/libclang-cpp.so.20.0+0x3150bf9)
[build]  #<!-- -->5 0x0000772928f67d63 clang::ASTWriter::WriteASTCore(clang::Sema*, llvm::StringRef, clang::Module*) (/usr/bin/../lib/libclang-cpp.so.20.0+0x3167d63)
[build]  #<!-- -->6 0x0000772928f6640e clang::ASTWriter::WriteAST(llvm::PointerUnion&lt;clang::Sema*, clang::Preprocessor*&gt;, llvm::StringRef, clang::Module*, llvm::StringRef, bool) (/usr/bin/../lib/libclang-cpp.so.20.0+0x316640e)
[build]  #<!-- -->7 0x0000772928fbd7cf clang::PCHGenerator::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/bin/../lib/libclang-cpp.so.20.0+0x31bd7cf)
[build]  #<!-- -->8 0x0000772928fbd9d1 clang::CXX20ModulesGenerator::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/bin/../lib/libclang-cpp.so.20.0+0x31bd9d1)
[build]  #<!-- -->9 0x00007729291425ac clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/bin/../lib/libclang-cpp.so.20.0+0x33425ac)
[build] #<!-- -->10 0x0000772926e87e88 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/bin/../lib/libclang-cpp.so.20.0+0x1087e88)
[build] #<!-- -->11 0x0000772929101f65 clang::FrontendAction::Execute() (/usr/bin/../lib/libclang-cpp.so.20.0+0x3301f65)
[build] #<!-- -->12 0x000077292907c395 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/bin/../lib/libclang-cpp.so.20.0+0x327c395)
[build] #<!-- -->13 0x0000772929183e4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/bin/../lib/libclang-cpp.so.20.0+0x3383e4d)
[build] #<!-- -->14 0x0000595babd1b4ba cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang-20+0x104ba)
[build] #<!-- -->15 0x0000595babd184a1 (/usr/bin/clang-20+0xd4a1)
[build] #<!-- -->16 0x0000595babd1745c clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang-20+0xc45c)
[build] #<!-- -->17 0x0000595babd254da main (/usr/bin/clang-20+0x1a4da)
[build] #<!-- -->18 0x0000772920434e08 (/usr/bin/../lib/libc.so.6+0x25e08)
[build] #<!-- -->19 0x0000772920434ecc __libc_start_main (/usr/bin/../lib/libc.so.6+0x25ecc)
[build] #<!-- -->20 0x0000595babd15e75 _start (/usr/bin/clang-20+0xae75)
[build] clang++: error: unable to execute command: Segmentation fault (core dumped)
[build] clang++: error: clang frontend command failed due to signal (use -v to see invocation)
[build] clang version 20.0.0 (/home/sigurd/.cache/yay/llvm-minimal-git/llvm-project 4b73f6a54884b6a34fbff16b5e24b7a2e480ebcb)
[build] Target: x86_64-pc-linux-gnu
[build] Thread model: posix
[build] InstalledDir: /usr/bin
[build] clang++: note: diagnostic msg: 
[build] ********************
[build] 
[build] PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
[build] Preprocessed source(s) and associated run script(s) are located at:
[build] clang++: note: diagnostic msg: /tmp/si_area-71eeab.cppm
[build] clang++: note: diagnostic msg: /tmp/si_area-71eeab.cache
[build] clang++: note: diagnostic msg: /tmp/si_area-71eeab.sh
[build] clang++: note: diagnostic msg: 
[build] 
[build] ********************
[build] [13/24  20% :: 2.513] Building CXX object CMakeFiles/main.dir/src/consts/numeric.cppm.o
```
&lt;details/&gt;

[log.txt](https://github.com/user-attachments/files/18493342/log.txt)

[crash.zip](https://github.com/user-attachments/files/18493433/crash.zip)
</details>


---

### Comment 3 - sigurd4

The problem is, i have a bunch of files, and i'm not sure exaclty where in my project the problem occurs. I could upload the entire project on github if that helps. I have uploaded the files that the crash message asked about in the zip attachment.

---

### Comment 4 - sigurd4

I've tried to reproduce it in godbolt, but so far to no avail

---

### Comment 5 - sigurd4

I'm probably using modules wrong. The compiler error occurred when i split up my module code into module interface (.cppm), header (.hpp), and implementation (.cpp). This is probably not how you're supposed to do it.

I'll try to find a minimum reproducable example.

---

### Comment 6 - sigurd4

Removed all of that, and i now suspect it has nothing to do with my code, but the compiler flags i'm giving to clang

---

### Comment 7 - sigurd4

Removing the flags -Xclang -emit-module-interface fixed it

---

### Comment 8 - shafik

CC @ChuanqiXu9 

---

### Comment 9 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: sigurd4 (sigurd4)

<details>
Compiler crash. Full logs are added in attachment. I've only added the first crash below.

# System info

OS: Arch Linux x86_64
Kernel: 6.12.9-arch1-1
Toolchain:
- llvm-minimal-git 20.0.0_r524521.4b73f6a54884-1
- llvm-libs-minimal-git 20.0.0_r524521.4b73f6a54884-1
- clang-libs-minimal-git 20.0.0_r524521.4b73f6a54884-1
- clang-minimal-git 20.0.0_r524521.4b73f6a54884-1
- libc++-modules 19.1.6-1
- libc++abi-modules 19.1.6-1
- lld 19.1.6-1

# Crash logs

&lt;details&gt;

```
[main] Building folder: /home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug main
[build] Starting build
[proc] Executing command: /usr/bin/cmake --build /home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug --config Debug --target main --
[build] [1/2   0% :: 0.003] Re-checking globbed directories...
[build] [1/24   4% :: 0.540] Scanning '/home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug/_deps/std-src/std.cppm' for CXX dependencies
[build] [2/24   8% :: 0.561] Generating CXX dyndep file _deps/std-build/CMakeFiles/std.dir/CXX.dd
[build] [4/24  12% :: 0.588] Generating CXX dyndep file CMakeFiles/main.dir/CXX.dd
[build] [13/24  16% :: 1.688] Building CXX object CMakeFiles/main.dir/src/units/si/si_area.cppm.o
[build] FAILED: CMakeFiles/main.dir/src/units/si/si_area.cppm.o CMakeFiles/main.dir/units-si_area.pcm 
[build] /usr/bin/clang++   -stdlib=libc++ -g -std=gnu++23 -isystem/usr/include/c++/v1 "-isystem/usr/include/c++/v1/*" -Wall -Wextra -pedantic -Wpedantic -Wsign-conversion -Wconversion -Werror=switch -fdiagnostics-color=always -Wdouble-promotion -Wnull-dereference -Winfinite-recursion -Wmisleading-indentation -g -Werror=return-local-addr -Wno-c++23-extensions -Xclang -emit-module-interface -fmodules -stdlib=libc++ -fbuiltin-module-map -fimplicit-module-maps -fprebuilt-module-path=/home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/build/debug/modules -fretain-comments-from-system-headers -Wstrict-overflow -MD -MT CMakeFiles/main.dir/src/units/si/si_area.cppm.o -MF CMakeFiles/main.dir/src/units/si/si_area.cppm.o.d @<!-- -->CMakeFiles/main.dir/src/units/si/si_area.cppm.o.modmap -o CMakeFiles/main.dir/src/units/si/si_area.cppm.o -c '/home/sigurd/Code/c++/programmering2/2-4_høyde_på_tree/src/units/si/si_area.cppm'
[build] PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
[build] Stack dump:
[build] 0.	Program arguments: /usr/bin/clang-20 -cc1 -triple x86_64-pc-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name si_area.cppm -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/sigurd/Code/c++/programmering2/2-4_h\303\270yde_p\303\245_tree/build/debug -fcoverage-compilation-dir=/home/sigurd/Code/c++/programmering2/2-4_h\303\270yde_p\303\245_tree/build/debug -resource-dir /usr/lib/clang/20 -Wall -Wextra -Wpedantic -Wsign-conversion -Wconversion -Werror=switch -Wdouble-promotion -Wnull-dereference -Winfinite-recursion -Wmisleading-indentation -Werror=return-local-addr -Wno-c++23-extensions -Wstrict-overflow -pedantic -std=gnu++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fmodules -fimplicit-module-maps -fmodules-cache-path=/home/sigurd/.cache/clang/ModuleCache -fmodule-map-file=/usr/lib/clang/20/include/module.modulemap -fprebuilt-module-path=/home/sigurd/Code/c++/programmering2/2-4_h\303\270yde_p\303\245_tree/build/debug/modules -fmodules-validate-system-headers -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -fretain-comments-from-system-headers -emit-module-interface -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o CMakeFiles/main.dir/src/units/si/si_area.cppm.o -x pcm CMakeFiles/main.dir/units-si_area.pcm
[build] 1.	&lt;eof&gt; parser at end of file
[build]  #<!-- -->0 0x0000772922d78ea3 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/bin/../lib/libLLVM.so.20.0+0x2778ea3)
[build]  #<!-- -->1 0x0000772922d76a1f llvm::sys::RunSignalHandlers() (/usr/bin/../lib/libLLVM.so.20.0+0x2776a1f)
[build]  #<!-- -->2 0x0000772922d796b6 (/usr/bin/../lib/libLLVM.so.20.0+0x27796b6)
[build]  #<!-- -->3 0x000077292044c1d0 (/usr/bin/../lib/libc.so.6+0x3d1d0)
[build]  #<!-- -->4 0x0000772928f50bf9 clang::ASTWriter::WriteSubmodules(clang::Module*, clang::ASTContext*) (/usr/bin/../lib/libclang-cpp.so.20.0+0x3150bf9)
[build]  #<!-- -->5 0x0000772928f67d63 clang::ASTWriter::WriteASTCore(clang::Sema*, llvm::StringRef, clang::Module*) (/usr/bin/../lib/libclang-cpp.so.20.0+0x3167d63)
[build]  #<!-- -->6 0x0000772928f6640e clang::ASTWriter::WriteAST(llvm::PointerUnion&lt;clang::Sema*, clang::Preprocessor*&gt;, llvm::StringRef, clang::Module*, llvm::StringRef, bool) (/usr/bin/../lib/libclang-cpp.so.20.0+0x316640e)
[build]  #<!-- -->7 0x0000772928fbd7cf clang::PCHGenerator::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/bin/../lib/libclang-cpp.so.20.0+0x31bd7cf)
[build]  #<!-- -->8 0x0000772928fbd9d1 clang::CXX20ModulesGenerator::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/bin/../lib/libclang-cpp.so.20.0+0x31bd9d1)
[build]  #<!-- -->9 0x00007729291425ac clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/usr/bin/../lib/libclang-cpp.so.20.0+0x33425ac)
[build] #<!-- -->10 0x0000772926e87e88 clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/bin/../lib/libclang-cpp.so.20.0+0x1087e88)
[build] #<!-- -->11 0x0000772929101f65 clang::FrontendAction::Execute() (/usr/bin/../lib/libclang-cpp.so.20.0+0x3301f65)
[build] #<!-- -->12 0x000077292907c395 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/bin/../lib/libclang-cpp.so.20.0+0x327c395)
[build] #<!-- -->13 0x0000772929183e4d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/bin/../lib/libclang-cpp.so.20.0+0x3383e4d)
[build] #<!-- -->14 0x0000595babd1b4ba cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang-20+0x104ba)
[build] #<!-- -->15 0x0000595babd184a1 (/usr/bin/clang-20+0xd4a1)
[build] #<!-- -->16 0x0000595babd1745c clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang-20+0xc45c)
[build] #<!-- -->17 0x0000595babd254da main (/usr/bin/clang-20+0x1a4da)
[build] #<!-- -->18 0x0000772920434e08 (/usr/bin/../lib/libc.so.6+0x25e08)
[build] #<!-- -->19 0x0000772920434ecc __libc_start_main (/usr/bin/../lib/libc.so.6+0x25ecc)
[build] #<!-- -->20 0x0000595babd15e75 _start (/usr/bin/clang-20+0xae75)
[build] clang++: error: unable to execute command: Segmentation fault (core dumped)
[build] clang++: error: clang frontend command failed due to signal (use -v to see invocation)
[build] clang version 20.0.0 (/home/sigurd/.cache/yay/llvm-minimal-git/llvm-project 4b73f6a54884b6a34fbff16b5e24b7a2e480ebcb)
[build] Target: x86_64-pc-linux-gnu
[build] Thread model: posix
[build] InstalledDir: /usr/bin
[build] clang++: note: diagnostic msg: 
[build] ********************
[build] 
[build] PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
[build] Preprocessed source(s) and associated run script(s) are located at:
[build] clang++: note: diagnostic msg: /tmp/si_area-71eeab.cppm
[build] clang++: note: diagnostic msg: /tmp/si_area-71eeab.cache
[build] clang++: note: diagnostic msg: /tmp/si_area-71eeab.sh
[build] clang++: note: diagnostic msg: 
[build] 
[build] ********************
[build] [13/24  20% :: 2.513] Building CXX object CMakeFiles/main.dir/src/consts/numeric.cppm.o
```
&lt;details/&gt;

[log.txt](https://github.com/user-attachments/files/18493342/log.txt)

[crash.zip](https://github.com/user-attachments/files/18493433/crash.zip)
</details>


---

### Comment 10 - ChuanqiXu9

While all compilers crash are compiler bugs, it is really not recommended to use `-Xclang` options generally. Are you developing a build system? If not, I'll recommend you to use build systems which supports modules already.

---

### Comment 11 - ChuanqiXu9

> The problem is, i have a bunch of files, and i'm not sure exaclty where in my project the problem occurs. I could upload the entire project on github if that helps. I have uploaded the files that the crash message asked about in the zip attachment.

Maybe it helps to look at the code.

---

### Comment 12 - sigurd4

I'm not developing a build system. I just had problems with clangd, and tried adding a bunch of different flags to make it work, some of which i probably shouldn't have added.

---

### Comment 13 - sigurd4

Just let me know, and i'll upload the entire code-base to github. I managed to get the same error even with only a handful of the files, to the point where i just assumed it wasn't my code that triggered the crash, but if it helps, i'll upload it.

---

### Comment 14 - ChuanqiXu9

> I'm not developing a build system. I just had problems with clangd, and tried adding a bunch of different flags to make it work, some of which i probably shouldn't have added.

Then it might be clangd's problem. Let's file an issue for that with the reduced one.

---

### Comment 15 - sigurd4

Managed to make clangd work as well by also omitting the "-fmodules" flag. Modules still work for some reason. I guess the flag is no longer needed.

---

### Comment 16 - sigurd4

I've found that if i don't use std::print or std::println, the error also goes away

---

