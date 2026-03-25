# [Clang] [SPIRV] `clang/include/clang/Driver/InputInfo.h:84: const char* clang::driver::InputInfo::getFilename() const: Assertion isFilename() && "Invalid accessor."' failed.` failure when building openmp for `spirv64-intel` with `-Wl,` flags

**Author:** mgorny
**URL:** https://github.com/llvm/llvm-project/issues/186598
**Status:** Closed
**Labels:** clang:driver, crash, SPIR-V
**Closed Date:** 2026-03-17T20:13:04Z

## Body

I'm trying to build the OpenMP offloading runtime for `spirv64-intel` target. I've noticed that when linker flags contain `-Wl,-O1`, clang driver crashes on an assertion:

```
clang++: /var/tmp/portage/llvm-core/clang-23.0.0.9999/work/clang/include/clang/Driver/InputInfo.h:84: const char* clang::driver::InputInfo::getFilename() const: Assertion `isFilename() && "Invalid accessor."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm/23/bin/clang++ -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -Xclang -fno-pch-timestamp -Wall -fcolor-diagnostics -Wcast-qual -Wformat-pedantic -Wimplicit-fallthrough -Wsign-compare -Wno-extra -Wno-pedantic -fno-semantic-interposition -fdata-sections -Wl,-O1 -nostdlib -emit-llvm --target=spirv64-intel openmp/device/CMakeFiles/libompdevice.dir/src/Allocator.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Configuration.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Debug.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Kernel.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/LibC.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Mapping.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Misc.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Parallelism.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Profiling.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Reduction.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/State.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Synchronization.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Tasking.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/DeviceUtils.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Workshare.cpp.o -o lib/spirv64-intel/libomptarget-spirv.bc
1.      Compilation construction
2.      Building compilation jobs
3.      Building compilation jobs
 #0 0x00007fde0574b410 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm/23/bin/../lib64/libLLVM.so.23.0git1f9b32ba+0x114b410)
 #1 0x00007fde05747f74 llvm::sys::RunSignalHandlers() (/usr/lib/llvm/23/bin/../lib64/libLLVM.so.23.0git1f9b32ba+0x1147f74)
 #2 0x00007fde0574815b (/usr/lib/llvm/23/bin/../lib64/libLLVM.so.23.0git1f9b32ba+0x114815b)
 #3 0x00007fde05748224 (/usr/lib/llvm/23/bin/../lib64/libLLVM.so.23.0git1f9b32ba+0x1148224)
 #4 0x00007fde04063be0 (/usr/lib64/libc.so.6+0x3dbe0)
 #5 0x00007fde040bd88c (/usr/lib64/libc.so.6+0x9788c)
 #6 0x00007fde04063aa6 raise (/usr/lib64/libc.so.6+0x3daa6)
 #7 0x00007fde0404b30b abort (/usr/lib64/libc.so.6+0x2530b)
 #8 0x00007fde0404b275 __assert_perror_fail (/usr/lib64/libc.so.6+0x25275)
 #9 0x00007fde11196e88 (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x996e88)
#10 0x00007fde11196f22 (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x996f22)
#11 0x00007fde136b6497 clang::driver::Driver::BuildJobsForActionNoCache(clang::driver::Compilation&, clang::driver::Action const*, clang::driver::ToolChain const*, llvm::StringRef, bool, bool, char const*, std::map<std::pair<clang::driver::Action const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::SmallVector<clang::driver::InputInfo, 4u>, std::less<std::pair<clang::driver::Action const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>>, std::allocator<std::pair<std::pair<clang::driver::Action const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> const, llvm::SmallVector<clang::driver::InputInfo, 4u>>>>&, clang::driver::Action::OffloadKind) const (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x2eb6497)
#12 0x00007fde136b46ce clang::driver::Driver::BuildJobsForAction(clang::driver::Compilation&, clang::driver::Action const*, clang::driver::ToolChain const*, llvm::StringRef, bool, bool, char const*, std::map<std::pair<clang::driver::Action const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>, llvm::SmallVector<clang::driver::InputInfo, 4u>, std::less<std::pair<clang::driver::Action const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>>, std::allocator<std::pair<std::pair<clang::driver::Action const*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> const, llvm::SmallVector<clang::driver::InputInfo, 4u>>>>&, clang::driver::Action::OffloadKind) const (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x2eb46ce)
#13 0x00007fde136b7e3a clang::driver::Driver::BuildJobs(clang::driver::Compilation&) const (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x2eb7e3a)
#14 0x00007fde136cb4ee clang::driver::Driver::BuildCompilation(llvm::ArrayRef<char const*>) (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x2ecb4ee)
#15 0x000056245d1d08f4 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm/23/bin/clang+++0x168f4)
#16 0x000056245d1ccbec main (/usr/lib/llvm/23/bin/clang+++0x12bec)
#17 0x00007fde0404d3fb (/usr/lib64/libc.so.6+0x273fb)
#18 0x00007fde0404d4ab __libc_start_main (/usr/lib64/libc.so.6+0x274ab)
#19 0x000056245d1ccc55 _start (/usr/lib/llvm/23/bin/clang+++0x12c55)
ninja: build stopped: subcommand failed.
```

To reproduce:

```
CC=clang CXX=clang++ LDFLAGS=-Wl,-O1 cmake -DLLVM_DEFAULT_TARGET_TRIPLE=spirv64-intel -DLLVM_ENABLE_RUNTIMES=openmp -G Ninja ${llvm_project}/runtimes
ninja -j1 -v
```

Confirmed on e865931b5f33bbab0b196489b00ee9df099402c2.

## Comments

### Comment 1 - mgorny

CC @jhuber6 perhaps?

---

### Comment 2 - jhuber6

The `spirv-intel` target is probably in @sarnex's domain. As far as I am aware, this uses `llvm-lto` or similar as its linker. There's probably a lot that needs to be improved here, it's very experimental at the moment.

---

### Comment 3 - mgorny

Actually, it seems to be triggered by any `-Wl,` flag.

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-driver

Author: Michał Górny (mgorny)

<details>
I'm trying to build the OpenMP offloading runtime for `spirv64-intel` target. I've noticed that when linker flags contain `-Wl,-O1`, clang driver crashes on an assertion:

```
clang++: /var/tmp/portage/llvm-core/clang-23.0.0.9999/work/clang/include/clang/Driver/InputInfo.h:84: const char* clang::driver::InputInfo::getFilename() const: Assertion `isFilename() &amp;&amp; "Invalid accessor."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/lib/llvm/23/bin/clang++ -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wno-pass-failed -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -Xclang -fno-pch-timestamp -Wall -fcolor-diagnostics -Wcast-qual -Wformat-pedantic -Wimplicit-fallthrough -Wsign-compare -Wno-extra -Wno-pedantic -fno-semantic-interposition -fdata-sections -Wl,-O1 -nostdlib -emit-llvm --target=spirv64-intel openmp/device/CMakeFiles/libompdevice.dir/src/Allocator.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Configuration.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Debug.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Kernel.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/LibC.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Mapping.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Misc.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Parallelism.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Profiling.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Reduction.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/State.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Synchronization.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Tasking.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/DeviceUtils.cpp.o openmp/device/CMakeFiles/libompdevice.dir/src/Workshare.cpp.o -o lib/spirv64-intel/libomptarget-spirv.bc
1.      Compilation construction
2.      Building compilation jobs
3.      Building compilation jobs
 #<!-- -->0 0x00007fde0574b410 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm/23/bin/../lib64/libLLVM.so.23.0git1f9b32ba+0x114b410)
 #<!-- -->1 0x00007fde05747f74 llvm::sys::RunSignalHandlers() (/usr/lib/llvm/23/bin/../lib64/libLLVM.so.23.0git1f9b32ba+0x1147f74)
 #<!-- -->2 0x00007fde0574815b (/usr/lib/llvm/23/bin/../lib64/libLLVM.so.23.0git1f9b32ba+0x114815b)
 #<!-- -->3 0x00007fde05748224 (/usr/lib/llvm/23/bin/../lib64/libLLVM.so.23.0git1f9b32ba+0x1148224)
 #<!-- -->4 0x00007fde04063be0 (/usr/lib64/libc.so.6+0x3dbe0)
 #<!-- -->5 0x00007fde040bd88c (/usr/lib64/libc.so.6+0x9788c)
 #<!-- -->6 0x00007fde04063aa6 raise (/usr/lib64/libc.so.6+0x3daa6)
 #<!-- -->7 0x00007fde0404b30b abort (/usr/lib64/libc.so.6+0x2530b)
 #<!-- -->8 0x00007fde0404b275 __assert_perror_fail (/usr/lib64/libc.so.6+0x25275)
 #<!-- -->9 0x00007fde11196e88 (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x996e88)
#<!-- -->10 0x00007fde11196f22 (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x996f22)
#<!-- -->11 0x00007fde136b6497 clang::driver::Driver::BuildJobsForActionNoCache(clang::driver::Compilation&amp;, clang::driver::Action const*, clang::driver::ToolChain const*, llvm::StringRef, bool, bool, char const*, std::map&lt;std::pair&lt;clang::driver::Action const*, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::SmallVector&lt;clang::driver::InputInfo, 4u&gt;, std::less&lt;std::pair&lt;clang::driver::Action const*, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;&gt;, std::allocator&lt;std::pair&lt;std::pair&lt;clang::driver::Action const*, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt; const, llvm::SmallVector&lt;clang::driver::InputInfo, 4u&gt;&gt;&gt;&gt;&amp;, clang::driver::Action::OffloadKind) const (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x2eb6497)
#<!-- -->12 0x00007fde136b46ce clang::driver::Driver::BuildJobsForAction(clang::driver::Compilation&amp;, clang::driver::Action const*, clang::driver::ToolChain const*, llvm::StringRef, bool, bool, char const*, std::map&lt;std::pair&lt;clang::driver::Action const*, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;, llvm::SmallVector&lt;clang::driver::InputInfo, 4u&gt;, std::less&lt;std::pair&lt;clang::driver::Action const*, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt;&gt;, std::allocator&lt;std::pair&lt;std::pair&lt;clang::driver::Action const*, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&gt; const, llvm::SmallVector&lt;clang::driver::InputInfo, 4u&gt;&gt;&gt;&gt;&amp;, clang::driver::Action::OffloadKind) const (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x2eb46ce)
#<!-- -->13 0x00007fde136b7e3a clang::driver::Driver::BuildJobs(clang::driver::Compilation&amp;) const (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x2eb7e3a)
#<!-- -->14 0x00007fde136cb4ee clang::driver::Driver::BuildCompilation(llvm::ArrayRef&lt;char const*&gt;) (/usr/lib/llvm/23/bin/../lib64/libclang-cpp.so.23.0git1f9b32ba+0x2ecb4ee)
#<!-- -->15 0x000056245d1d08f4 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm/23/bin/clang+++0x168f4)
#<!-- -->16 0x000056245d1ccbec main (/usr/lib/llvm/23/bin/clang+++0x12bec)
#<!-- -->17 0x00007fde0404d3fb (/usr/lib64/libc.so.6+0x273fb)
#<!-- -->18 0x00007fde0404d4ab __libc_start_main (/usr/lib64/libc.so.6+0x274ab)
#<!-- -->19 0x000056245d1ccc55 _start (/usr/lib/llvm/23/bin/clang+++0x12c55)
ninja: build stopped: subcommand failed.
```

To reproduce:

```
CC=clang CXX=clang++ LDFLAGS=-Wl,-O1 cmake -DLLVM_DEFAULT_TARGET_TRIPLE=spirv64-intel -DLLVM_ENABLE_RUNTIMES=openmp -G Ninja ${llvm_project}/runtimes
ninja -j1 -v
```

Confirmed on e865931b5f33bbab0b196489b00ee9df099402c2.
</details>


---

### Comment 5 - sarnex

Yep, I'll take a look, thanks. Using `spirv64-intel` with the OpenMP Device libraries is definitely experimental and even if it builds it's unlikely to work, but we are working to improve it. I'll fix this.

---

### Comment 6 - mgorny

Thanks! Honestly, I was just asked to package offloading runtimes for Gentoo. I have no clue if they work at all (could also be broken because of packaging error) :-). `spirv64-intel` is a 23.x-only thing, so not a big worry right now.

---

### Comment 7 - sarnex

Cool, feel free to ping me in #gentoo-dev if you hit any more nonsense

---

