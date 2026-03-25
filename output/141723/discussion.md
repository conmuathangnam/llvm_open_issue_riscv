# Regression for RISC-V caused by b754e40 "MC: Remove redundant relocations for label differences"

**Author:** asb
**URL:** https://github.com/llvm/llvm-project/issues/141723
**Status:** Closed
**Labels:** backend:RISC-V, regression, llvm:mc
**Closed Date:** 2025-05-29T17:28:19Z

## Body

After b754e40 commit, the `XRay-riscv64-linux :: TestCases/Posix/coverage-sample.cpp` test case starts to fail:

```
/home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/stage2/./bin/clang  --driver-mode=g++ -fxray-instrument   -Wthread-safety -Wthread-safety-reference -Wthread-safety-beta    -std=c++11 /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/llvm/compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp -o /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/stage2/runtimes/runtimes-bins/compiler-rt/test/xray/RISCV64LinuxConfig/TestCases/Posix/Output/coverage-sample.cpp.tmp # RUN: at line 3
+ /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/stage2/./bin/clang --driver-mode=g++ -fxray-instrument -Wthread-safety -Wthread-safety-reference -Wthread-safety-beta -std=c++11 /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/llvm/compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp -o /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/stage2/runtimes/runtimes-bins/compiler-rt/test/xray/RISCV64LinuxConfig/TestCases/Posix/Output/coverage-sample.cpp.tmp
/usr/bin/ld: .eh_frame_hdr refers to overlapping FDEs
/usr/bin/ld: final link failed: bad value
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```

The following recipe is sufficient to reproduce (and a variant of it was used to do the bisect and confirm the problematic commit):

(Note that the failure message pasted above is from a "native" two-stage build. The commands below produce the same issue via cross compilation on an x86 host)

```sh
cmake -G Ninja \
  -DLLVM_TARGETS_TO_BUILD="X86;RISCV" \
  -DLLVM_ENABLE_PROJECTS='clang;lld' \
  -DLLVM_ENABLE_RUNTIMES=compiler-rt \
  -DLLVM_CCACHE_BUILD=ON \
  -DLLVM_USE_RELATIVE_PATHS_IN_FILES=ON \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DLLVM_ENABLE_LLD=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DLLVM_RUNTIME_TARGETS=riscv64-linux-gnu \
  -DRUNTIMES_riscv64-linux-gnu_CMAKE_SYSTEM_NAME=Linux \
  -DRUNTIMES_riscv64-linux-gnu_CMAKE_SYSROOT=$HOME/rvsysroot \
  -S llvm \
  -B build/rvregression
cmake --build build/rvregression --target runtimes
./build/rvregression/bin/clang --target=riscv64-linux-gnu --sysroot=$HOME/rvsysroot --driver-mode=g++ -fxray-instrument -std=c++11 compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp

```
The riscv64-linux-gnu-ld on my system is 2.43.1.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Alex Bradbury (asb)

<details>
After b754e40 commit, the `XRay-riscv64-linux :: TestCases/Posix/coverage-sample.cpp` test case starts to fail:

```
/home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/stage2/./bin/clang  --driver-mode=g++ -fxray-instrument   -Wthread-safety -Wthread-safety-reference -Wthread-safety-beta    -std=c++11 /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/llvm/compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp -o /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/stage2/runtimes/runtimes-bins/compiler-rt/test/xray/RISCV64LinuxConfig/TestCases/Posix/Output/coverage-sample.cpp.tmp # RUN: at line 3
+ /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/stage2/./bin/clang --driver-mode=g++ -fxray-instrument -Wthread-safety -Wthread-safety-reference -Wthread-safety-beta -std=c++11 /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/llvm/compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp -o /home/buildbot-worker/bbroot/clang-riscv-rva23-2stage/stage2/runtimes/runtimes-bins/compiler-rt/test/xray/RISCV64LinuxConfig/TestCases/Posix/Output/coverage-sample.cpp.tmp
/usr/bin/ld: .eh_frame_hdr refers to overlapping FDEs
/usr/bin/ld: final link failed: bad value
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```

The following recipe is sufficient to reproduce (and a variant of it was used to do the bisect and confirm the problematic commit):

```sh
cmake -G Ninja \
  -DLLVM_TARGETS_TO_BUILD="X86;RISCV" \
  -DLLVM_ENABLE_PROJECTS='clang;lld' \
  -DLLVM_ENABLE_RUNTIMES=compiler-rt \
  -DLLVM_CCACHE_BUILD=ON \
  -DLLVM_USE_RELATIVE_PATHS_IN_FILES=ON \
  -DCMAKE_C_COMPILER=clang \
  -DCMAKE_CXX_COMPILER=clang++ \
  -DLLVM_ENABLE_LLD=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DLLVM_RUNTIME_TARGETS=riscv64-linux-gnu \
  -DRUNTIMES_riscv64-linux-gnu_CMAKE_SYSTEM_NAME=Linux \
  -DRUNTIMES_riscv64-linux-gnu_CMAKE_SYSROOT=$HOME/rvsysroot \
  -S llvm \
  -B build/rvregression
cmake --build build/rvregression --target runtimes
./build/rvregression/bin/clang --target=riscv64-linux-gnu --sysroot=$HOME/rvsysroot --driver-mode=g++ -fxray-instrument -std=c++11 compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp

```
The riscv64-linux-gnu-ld on my system is 2.43.1.
</details>


---

### Comment 2 - MaskRay

The regression might start with bb03cdcb441fd68da9d1ebb7d5f39f73667cd39c ("RISCV: Remove shouldForceRelocation and unneeded relocations"). 

Is the issue resolved after ab0931b6389838cb5d7d11914063a1ddd84102f0 (MC: Set MCSection::LinkerRelaxable for linker-relaxable MCAlignFragment)?

---

### Comment 3 - MaskRay

BTW: How can I build a sysroot and make a cross-compilation build (you have some notes that I haven't tried yet... https://muxup.com/2024q4/rootless-cross-architecture-debootstrap)

I've tried this
```
git clone https://github.com/riscv-collab/riscv-gnu-toolchain
cd riscv-gnu-toolchain
./configure --prefix=$HOME/opt/riscv/
```

```
 cmake -G Ninja \
          -DLLVM_TARGETS_TO_BUILD="X86;RISCV" \
          -DLLVM_ENABLE_PROJECTS='clang;lld' \
          -DLLVM_ENABLE_RUNTIMES=compiler-rt \
          -DLLVM_USE_RELATIVE_PATHS_IN_FILES=ON \
          -DCMAKE_C_COMPILER=clang \
          -DCMAKE_CXX_COMPILER=clang++ \
          -DLLVM_ENABLE_LLD=ON \
          -DCMAKE_BUILD_TYPE=Release \
          -DLLVM_ENABLE_ASSERTIONS=ON \
          -DLLVM_RUNTIME_TARGETS=riscv64-linux-gnu \
          -DRUNTIMES_riscv64-linux-gnu_CMAKE_SYSTEM_NAME=Linux \
          -DRUNTIMES_riscv64-linux-gnu_CMAKE_SYSROOT=$HOME/opt/riscv/sysroot \   # is this a sysroot?
          -S llvm \
          -B /t/rv
```

Then I get many failures
```
FAILED: lib/Support/CMakeFiles/LLVMSupport.dir/CodeGenCoverage.cpp.o
/usr/local/bin/clang++ -DGTEST_HAS_RTTI=0 -D_DEBUG -D_GLIBCXX_ASSERTIONS -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/t/rv/lib/Support -I/home/ray/llvm/llvm/lib/Support -I/t/rv/include -I/home/ray/llvm/llvm/include -fPIC -fno-semantic-interpo
sition -fvisibility-inlines-hidden -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -pedantic -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno
-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wmisleading-indentation -Wctad-maybe-unsupported -fdiagnostics-color -ffunction-sections -fdata-sections -ffile-prefix-map=/t/rv=../../home/ray/llvm -ffile-prefix-map=/home/ray/llvm/=
-no-canonical-prefixes  -O3 -DNDEBUG -std=c++17 -UNDEBUG  -fno-exceptions -funwind-tables -fno-rtti -MD -MT lib/Support/CMakeFiles/LLVMSupport.dir/CodeGenCoverage.cpp.o -MF lib/Support/CMakeFiles/LLVMSupport.dir/CodeGenCoverage.cpp.o.d -o lib/Support/CMakeFiles/LLVMSupport.dir/CodeGe
nCoverage.cpp.o -c /home/ray/llvm/llvm/lib/Support/CodeGenCoverage.cpp
In file included from /home/ray/llvm/llvm/lib/Support/CodeGenCoverage.cpp:12:
In file included from /home/ray/llvm/llvm/include/llvm/Support/CodeGenCoverage.h:14:
In file included from /home/ray/llvm/llvm/include/llvm/ADT/BitVector.h:17:
In file included from /home/ray/llvm/llvm/include/llvm/ADT/ArrayRef.h:12:
In file included from /home/ray/llvm/llvm/include/llvm/ADT/Hashing.h:47:
In file included from /home/ray/llvm/llvm/include/llvm/ADT/ADL.h:13:
In file included from /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/iterator:65:
In file included from /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/stream_iterator.h:35:
In file included from /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/iosfwd:42:
In file included from /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/postypes.h:40:
In file included from /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/cwchar:44:
/usr/include/wchar.h:35:10: fatal error: 'stddef.h' file not found
   35 | #include <stddef.h>
      |          ^~~~~~~~~~
```

---

### Comment 4 - asb

Thanks for taking a look! I can confirm that ab0931b6389838cb5d7d11914063a1ddd84102f0 fixed this (I can't reproduce on HEAD, and reverting that commit reintroduces the issue).

The following is kind of moot now, but for the record:

Sorry for not giving more information about the sysroot etc, I wasn't sure if you had an existing setup. My sysroot was just a debian one, and the binutils ld that gets invoked is the one provided by the Arch `riscv64-linux-gnu-binutils` package. In terms of producing your own sysroot for cross compilation, the linked debootstrap wrapper should hopefully work. Though if you're happy just using `sudo` I recently rewrote LLVM's cross-compile docs and added guidance on producing a sysroot and fixing symlinks [here](https://llvm.org/docs/HowToCrossCompileLLVM.html#setting-up-a-sysroot).

---

