# [clang-21] Crash after upgrading to clang 21 with C++20 modules and header units.

**Author:** lilyatplyable
**URL:** https://github.com/llvm/llvm-project/issues/165445
**Status:** Closed
**Labels:** clang:modules, crash, regression:21
**Closed Date:** 2025-11-04T01:54:57Z

## Body

Clang 21.1.3 (Also fails with downloaded binaries for clang 21.1.4)
Fedora Linux 43 (Workstation Edition)


badHeader.h:
```
#include <set>
```

badModule.cppm:
```
module;
#include <set>
export module badModule;

export void explode(){
        std::set<int> noooo;
}
```

Running:
```
clang++ -std=gnu++23 -fmodule-header=system badHeader.h -I .
clang++ -std=gnu++23 -fmodule-file=badHeader.pcm --precompile badModule.cppm -I .
```

Second command produces:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -std=gnu++23 -fmodule-file=badHeader.pcm --precompile badModule.cppm -I .
1.	<eof> parser at end of file
 #0 0x00007f6c268cb519 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib64/libLLVM.so.21.1+0x50cb519)
 #1 0x00007f6c268cb289 llvm::sys::CleanupOnSignal(unsigned long) (/lib64/libLLVM.so.21.1+0x50cb289)
 #2 0x00007f6c2683a7e5 (/lib64/libLLVM.so.21.1+0x503a7e5)
 #3 0x00007f6c2683aa36 (/lib64/libLLVM.so.21.1+0x503aa36)
 #4 0x00007f6c212262c0 __restore_rt (/lib64/libc.so.6+0x1a2c0)
 #5 0x00007f6c2d6c00a0 (/lib64/libclang-cpp.so.21.1+0x34c00a0)
 #6 0x00007f6c2d6a05dd clang::ASTWriter::GenerateNameLookupTable(clang::ASTContext&, clang::DeclContext const*, llvm::SmallVectorImpl<char>&, llvm::SmallVectorImpl<char>&, llvm::SmallVectorImpl<char>&) (/lib64/libclang-cpp.so.21.1+0x34a05dd)
 #7 0x00007f6c2d6a325c clang::ASTWriter::WriteDeclContextVisibleUpdate(clang::ASTContext&, clang::DeclContext const*) (/lib64/libclang-cpp.so.21.1+0x34a325c)
 #8 0x00007f6c2d6ad207 clang::ASTWriter::WriteDeclAndTypes(clang::ASTContext&) (/lib64/libclang-cpp.so.21.1+0x34ad207)
 #9 0x00007f6c2d6a94a3 clang::ASTWriter::WriteASTCore(clang::Sema*, llvm::StringRef, clang::Module*) (/lib64/libclang-cpp.so.21.1+0x34a94a3)
#10 0x00007f6c2d6a8a43 clang::ASTWriter::WriteAST(llvm::PointerUnion<clang::Sema*, clang::Preprocessor*>, llvm::StringRef, clang::Module*, llvm::StringRef, bool) (/lib64/libclang-cpp.so.21.1+0x34a8a43)
#11 0x00007f6c2d6df613 clang::PCHGenerator::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.21.1+0x34df613)
#12 0x00007f6c2d6df80a clang::CXX20ModulesGenerator::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.21.1+0x34df80a)
#13 0x00007f6c2d7f8c15 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&) (/lib64/libclang-cpp.so.21.1+0x35f8c15)
#14 0x00007f6c2b6250b2 clang::ParseAST(clang::Sema&, bool, bool) (/lib64/libclang-cpp.so.21.1+0x14250b2)
#15 0x00007f6c2bfc6f0f clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x1dc6f0f)
#16 0x00007f6c2bfc6a20 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib64/libclang-cpp.so.21.1+0x1dc6a20)
#17 0x00007f6c2bfc63c5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.21.1+0x1dc63c5)
#18 0x00005562d927d8d6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang-21+0x198d6)
#19 0x00005562d9274a4e (/usr/bin/clang-21+0x10a4e)
#20 0x00007f6c2d4f462d (/lib64/libclang-cpp.so.21.1+0x32f462d)
#21 0x00007f6c2683a780 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/lib64/libLLVM.so.21.1+0x503a780)
#22 0x00007f6c2c1d6345 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/lib64/libclang-cpp.so.21.1+0x1fd6345)
#23 0x00007f6c2c12249e clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/lib64/libclang-cpp.so.21.1+0x1f2249e)
#24 0x00007f6c2c122292 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/lib64/libclang-cpp.so.21.1+0x1f22292)
#25 0x00007f6c2c121c06 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/lib64/libclang-cpp.so.21.1+0x1f21c06)
#26 0x00005562d9276f04 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang-21+0x12f04)
#27 0x00005562d9275782 main (/usr/bin/clang-21+0x11782)
#28 0x00007f6c2120f5b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
#29 0x00007f6c2120f668 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
#30 0x00005562d927ff85 _start (/usr/bin/clang-21+0x1bf85)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.1.3 (Fedora 21.1.3-1.fc43)
Target: x86_64-redhat-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
Configuration file: /etc/clang/x86_64-redhat-linux-gnu-clang.cfg
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/badModule-c85b17.cppm
clang: note: diagnostic msg: /tmp/badModule-c85b17.sh
clang: note: diagnostic msg: 

********************
```

Produced files in `/tmp`:
[badModule-c85b17.zip](https://github.com/user-attachments/files/23306897/badModule-c85b17.zip)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Lily Penrose (lilyatplyable)

<details>
This is new since upgrading, was fine in clang-20.

[SeparatedSet-c2a664.sh](https://github.com/user-attachments/files/23194252/SeparatedSet-c2a664.sh)
[SeparatedSet-c2a664.cppm.zip](https://github.com/user-attachments/files/23194260/SeparatedSet-c2a664.cppm.zip)

Stack dump:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang-21 -cc1 -triple x86_64-redhat-linux-gnu -emit-module-interface -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name SeparatedSet.cppm -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/lily/mbox/q/cmake-build-debug -fcoverage-compilation-dir=/home/lily/mbox/q/cmake-build-debug -sys-header-deps -module-file-deps -D BOOST_ATOMIC_DYN_LINK -D BOOST_ATOMIC_NO_LIB -D BOOST_CHRONO_DYN_LINK -D BOOST_CHRONO_NO_LIB -D BOOST_FILESYSTEM_DYN_LINK -D BOOST_FILESYSTEM_NO_LIB -D BOOST_LOG_DYN_LINK -D BOOST_LOG_NO_LIB -D BOOST_LOG_SETUP_DYN_LINK -D BOOST_LOG_SETUP_NO_LIB -D BOOST_REGEX_DYN_LINK -D BOOST_REGEX_NO_LIB -D BOOST_SERIALIZATION_DYN_LINK -D BOOST_SERIALIZATION_NO_LIB -D BOOST_STACKTRACE_USE_ADDR2LINE -D BOOST_THREAD_DYN_LINK -D BOOST_THREAD_NO_LIB -D BOOST_UNIT_TEST_FRAMEWORK_DYN_LINK -D BOOST_UNIT_TEST_FRAMEWORK_NO_LIB -D CGAL_USE_CORE=1 -D CGAL_USE_GMPXX=1 -D OCC_CONVERT_SIGNALS -Wno-experimental-header-units -std=gnu++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fmodule-file=external-modules/std.pcm -fmodule-file=external-modules/boost.pcm -fmodule-file=external-modules/cgal.pcm -fmodule-file=external-modules/opencascade.pcm -fmodule-file=external-modules/libdxfrw.pcm -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ SeparatedSet-c2a664.cppm
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x00007ff1da6cb519 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib64/libLLVM.so.21.1+0x50cb519)
 #<!-- -->1 0x00007ff1da6cbf5d (/lib64/libLLVM.so.21.1+0x50cbf5d)
 #<!-- -->2 0x00007ff1d50262c0 __restore_rt (/lib64/libc.so.6+0x1a2c0)
 #<!-- -->3 0x00007ff1e14c00a0 (/lib64/libclang-cpp.so.21.1+0x34c00a0)
 #<!-- -->4 0x00007ff1e14a05dd clang::ASTWriter::GenerateNameLookupTable(clang::ASTContext&amp;, clang::DeclContext const*, llvm::SmallVectorImpl&lt;char&gt;&amp;, llvm::SmallVectorImpl&lt;char&gt;&amp;, llvm::SmallVectorImpl&lt;char&gt;&amp;) (/lib64/libclang-cpp.so.21.1+0x34a05dd)
 #<!-- -->5 0x00007ff1e14a325c clang::ASTWriter::WriteDeclContextVisibleUpdate(clang::ASTContext&amp;, clang::DeclContext const*) (/lib64/libclang-cpp.so.21.1+0x34a325c)
 #<!-- -->6 0x00007ff1e14ad207 clang::ASTWriter::WriteDeclAndTypes(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x34ad207)
 #<!-- -->7 0x00007ff1e14a94a3 clang::ASTWriter::WriteASTCore(clang::Sema*, llvm::StringRef, clang::Module*) (/lib64/libclang-cpp.so.21.1+0x34a94a3)
 #<!-- -->8 0x00007ff1e14a8a43 clang::ASTWriter::WriteAST(llvm::PointerUnion&lt;clang::Sema*, clang::Preprocessor*&gt;, llvm::StringRef, clang::Module*, llvm::StringRef, bool) (/lib64/libclang-cpp.so.21.1+0x34a8a43)
 #<!-- -->9 0x00007ff1e14df613 clang::PCHGenerator::HandleTranslationUnit(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x34df613)
#<!-- -->10 0x00007ff1e14df80a clang::CXX20ModulesGenerator::HandleTranslationUnit(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x34df80a)
#<!-- -->11 0x00007ff1e15f8c15 clang::MultiplexConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/lib64/libclang-cpp.so.21.1+0x35f8c15)
#<!-- -->12 0x00007ff1df4250b2 clang::ParseAST(clang::Sema&amp;, bool, bool) (/lib64/libclang-cpp.so.21.1+0x14250b2)
#<!-- -->13 0x00007ff1dfdc6f0f clang::FrontendAction::Execute() (/lib64/libclang-cpp.so.21.1+0x1dc6f0f)
#<!-- -->14 0x00007ff1dfdc6a20 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/lib64/libclang-cpp.so.21.1+0x1dc6a20)
#<!-- -->15 0x00007ff1dfdc63c5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib64/libclang-cpp.so.21.1+0x1dc63c5)
#<!-- -->16 0x0000563b84fe08d6 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang-21+0x198d6)
#<!-- -->17 0x0000563b84fd7a4e (/usr/bin/clang-21+0x10a4e)
#<!-- -->18 0x0000563b84fd9534 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang-21+0x12534)
#<!-- -->19 0x0000563b84fd8782 main (/usr/bin/clang-21+0x11782)
#<!-- -->20 0x00007ff1d500f5b5 __libc_start_call_main (/lib64/libc.so.6+0x35b5)
#<!-- -->21 0x00007ff1d500f668 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x3668)
#<!-- -->22 0x0000563b84fe2f85 _start (/usr/bin/clang-21+0x1bf85)
SeparatedSet-c2a664.sh: line 4: 28466 Segmentation fault         (core dumped) "/usr/bin/clang-21" "-cc1" "-triple" "x86_64-redhat-linux-gnu" "-emit-module-interface" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "SeparatedSet.cppm" "-mrelocation-model" "pic" "-pic-level" "2" "-fhalf-no-semantic-interposition" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/lily/mbox/q/cmake-build-debug" "-fcoverage-compilation-dir=/home/lily/mbox/q/cmake-build-debug" "-sys-header-deps" "-module-file-deps" "-D" "BOOST_ATOMIC_DYN_LINK" "-D" "BOOST_ATOMIC_NO_LIB" "-D" "BOOST_CHRONO_DYN_LINK" "-D" "BOOST_CHRONO_NO_LIB" "-D" "BOOST_FILESYSTEM_DYN_LINK" "-D" "BOOST_FILESYSTEM_NO_LIB" "-D" "BOOST_LOG_DYN_LINK" "-D" "BOOST_LOG_NO_LIB" "-D" "BOOST_LOG_SETUP_DYN_LINK" "-D" "BOOST_LOG_SETUP_NO_LIB" "-D" "BOOST_REGEX_DYN_LINK" "-D" "BOOST_REGEX_NO_LIB" "-D" "BOOST_SERIALIZATION_DYN_LINK" "-D" "BOOST_SERIALIZATION_NO_LIB" "-D" "BOOST_STACKTRACE_USE_ADDR2LINE" "-D" "BOOST_THREAD_DYN_LINK" "-D" "BOOST_THREAD_NO_LIB" "-D" "BOOST_UNIT_TEST_FRAMEWORK_DYN_LINK" "-D" "BOOST_UNIT_TEST_FRAMEWORK_NO_LIB" "-D" "CGAL_USE_CORE=1" "-D" "CGAL_USE_GMPXX=1" "-D" "OCC_CONVERT_SIGNALS" "-Wno-experimental-header-units" "-std=gnu++23" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fmodule-file=external-modules/std.pcm" "-fmodule-file=external-modules/boost.pcm" "-fmodule-file=external-modules/cgal.pcm" "-fmodule-file=external-modules/opencascade.pcm" "-fmodule-file=external-modules/libdxfrw.pcm" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "SeparatedSet-c2a664.cppm"
```
</details>


---

### Comment 2 - ChuanqiXu9

Would you like to reduce it further to make it as small as possible?

And it will be helpful to test it with trunk and assertion-enabled build.

---

### Comment 3 - lilyatplyable

Have updated to minimal version.

Note that it crashes both when using `#include <set>` in gmf even though the header isn't imported, and also `import "badHeader.h"`. If `-fmodule-file=badHeader.pcm` is removed from the second command it runs fine.

---

### Comment 4 - naveen-seth

Without stdlib:

```c++
//--- enum.h
enum { SomeName };
```

```c++
//--- header.h
#include "enum.h"
```

```c++
//--- A.cppm
module;
#include "enum.h"
export module ThanksForBugReport;
```

```sh
clang++ -std=c++20 -fmodule-header=user header.h -o header.pcm
clang++ -std=c++20 -fmodule-file=header.pcm --precompile A.cppm 
```

Also crashes with:

```c++
//--- A.cppm
export module ThanksForBugReport;
import "header.h";
```

---

### Comment 5 - lilyatplyable

Y'all are awesome <3 so impressive

---

