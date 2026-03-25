# PowerPC regressions introduce by 3469db82b5c821c94b58c0b81f03bbef51efa30b

**Author:** tstellar
**URL:** https://github.com/llvm/llvm-project/issues/141642
**Status:** Closed
**Labels:** backend:PowerPC, regression, miscompilation
**Closed Date:** 2025-06-23T17:22:34Z

## Body

I'm seeing a test failure in our Fedora builds of the [highway]( https://github.com/google/highway) package on ppc64le, and I've bisected the failures down to 3469db82b5c821c94b58c0b81f03bbef51efa30b (#108430) 

How to reproduce in the `fedora:rawhide` container for ppc64le:

```
$ curl -O -L https://download.copr.fedorainfracloud.org/results/tstellar/clang-fedora-centos-testing/fedora-rawhide-ppc64le/09017423-highway/highway-1.2.0-5.fc43.src.rpm
$ dnf builddep highway-1.2.0-5.fc43.src.rpm
$ dnf install rpm-build
$ rpmbuild -D "%_distro_extra_ldflags -fuse-ld=lld" -rb highway-1.2.0-5.fc43.src.rpm
```

cc @alexey-bataev 

## Comments

### Comment 1 - alexey-bataev

No idea what to do with this repro

---

### Comment 2 - tstellar

@alexey-bataev I can try to reduce it further. 

---

### Comment 3 - alexey-bataev

This will help, thanks!

---

### Comment 4 - tstellar

@alexey-bataev The bug is only reproducible with LTO, so here are the outputs from lld -save-temps:

* [bit_pack_test.cc.o.3.import.ll.txt](https://github.com/user-attachments/files/20510271/bit_pack_test.cc.o.3.import.ll.txt) is the LLVM IR before optimization.
* [bit_pack_test.cc.o.4.opt.good.ll.txt](https://github.com/user-attachments/files/20510273/bit_pack_test.cc.o.4.opt.good.ll.txt) is the output after optimization prior to 3469db82b5c821c94b58c0b81f03bbef51efa30b
* [bit_pack_test.cc.o.4.opt.bad.ll.txt](https://github.com/user-attachments/files/20510272/bit_pack_test.cc.o.4.opt.bad.ll.txt) is the output after optimization with 3469db82b5c821c94b58c0b81f03bbef51efa30b.

It looks like block 908 in @_ZN3hwy6N_PPC913TestAllPack16Ev is where the good and bad outputs begin to diverge.  Let me know if this is helpful or if you have tips for reducing further.

---

### Comment 5 - alexey-bataev

Do I understand correctly that it is in release 20? If so, can you try trunk? I don't see anything wrong in the provided code, looks correct. Maybe the codegen issue?

---

### Comment 6 - tstellar

@alexey-bataev I can reproduce on trunk too.  I'll take a look at the codegen.

---

### Comment 7 - tstellar

Here is the side-by-side disassembly with the good code on the left and bad code on the right.  The relevant block starts at line 3715.  

[asm_sdiff_good_bad.txt](https://github.com/user-attachments/files/20534258/asm_sdiff_good_bad.txt)

---

### Comment 8 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Tom Stellard (tstellar)

<details>
I'm seeing a test failure in our Fedora builds of the [highway]( https://github.com/google/highway) package on ppc64le, and I've bisected the failures down to 3469db82b5c821c94b58c0b81f03bbef51efa30b (#<!-- -->108430) 

How to reproduce in the `fedora:rawhide` container for ppc64le:

```
$ curl -O -L https://download.copr.fedorainfracloud.org/results/tstellar/clang-fedora-centos-testing/fedora-rawhide-ppc64le/09017423-highway/highway-1.2.0-5.fc43.src.rpm
$ dnf builddep highway-1.2.0-5.fc43.src.rpm
$ dnf install rpm-build
$ rpmbuild -D "%_distro_extra_ldflags -fuse-ld=lld" -rb highway-1.2.0-5.fc43.src.rpm
```

cc @<!-- -->alexey-bataev 
</details>


---

### Comment 9 - alexey-bataev

Need a PowerPC expert here to check the codegen. I do not see anything unusual in SLP result, everything looks correct (in the suspicious code in block 908). The SLP trees are pretty small (6 nodes only, 4 x i32 store rooted, no reordering, single gather node with partial matching for vector node without the reordering), nothing unusual.
Another problem might be related to aliasing, but not sure about it.
Also, would be good to double check that this code causes issues.

---

### Comment 10 - gilamn5tr

We will find someone to take a look.

---

### Comment 11 - tstellar

I was able to reproduce the issue without LTO and it seems like the root cause may be a different commit.  I'm going to try to bisect again with LTO disabled and see what I can get.

For now though, here is a container image that can be used to reproduce the issue: ghcr.io/tstellar/issue141642:latest

Below is the Container file used to generate it:

```
FROM fedora:rawhide

RUN dnf install -y cmake git ninja-build clang

RUN git clone https://github.com/llvm/llvm-project

WORKDIR ./llvm-project

RUN git checkout 3469db82b5c821c94b58c0b81f03bbef51efa30b

RUN cmake -G Ninja -B build -S llvm -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=Native -DLLVM_ENABLE_PROJECTS="clang;lld" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

RUN ninja -C build install-clang install-clang-resource-headers install-llvm-ar install-llvm-ranlib install-lld

RUN dnf builddep -y highway

RUN dnf install -y redhat-rpm-config

RUN git clone https://github.com/tstellar/highway

WORKDIR ./highway

RUN git checkout llvm-141642


ENV CFLAGS='-O2 -flto=thin  -fexceptions -grecord-gcc-switches -pipe -Wall -Werror=format-security -Wp,-U_FORTIFY_SOURCE,-D_FORTIFY_SOURCE=3 -Wp,-D_GLIBCXX_ASSERTIONS --config=/usr/lib/rpm/redhat/redhat-hardened-clang.cfg -fstack-protector-strong   -m64 -mcpu=power8 -mtune=power8 -fasynchronous-unwind-tables -fstack-clash-protection '
ENV CXXFLAGS='-O2 -flto=thin  -fexceptions -grecord-gcc-switches -pipe -Wall -Werror=format-security -Wp,-U_FORTIFY_SOURCE,-D_FORTIFY_SOURCE=3 -Wp,-D_GLIBCXX_ASSERTIONS --config=/usr/lib/rpm/redhat/redhat-hardened-clang.cfg -fstack-protector-strong   -m64 -mcpu=power8 -mtune=power8 -fasynchronous-unwind-tables -fstack-clash-protection '
ENV LDFLAGS='-Wl,-z,relro -Wl,--as-needed  -Wl,-z,pack-relative-relocs -Wl,-z,now --config=/usr/lib/rpm/redhat/redhat-hardened-clang-ld.cfg  -flto=thin  -Wl,--build-id=sha1 -fuse-ld=lld '
ENV CC=clang
ENV CXX=clang++

RUN /usr/bin/cmake -G Ninja -S . -B build -DCMAKE_C_FLAGS_RELEASE:STRING=-DNDEBUG -DCMAKE_CXX_FLAGS_RELEASE:STRING=-DNDEBUG -DCMAKE_Fortran_FLAGS_RELEASE:STRING=-DNDEBUG -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCMAKE_INSTALL_DO_STRIP:BOOL=OFF -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_INSTALL_FULL_SBINDIR:PATH=/usr/bin -DCMAKE_INSTALL_SBINDIR:PATH=bin -DINCLUDE_INSTALL_DIR:PATH=/usr/include -DLIB_INSTALL_DIR:PATH=/usr/lib64 -DSYSCONF_INSTALL_DIR:PATH=/etc -DSHARE_INSTALL_PREFIX:PATH=/usr/share -DLIB_SUFFIX=64 -DBUILD_SHARED_LIBS:BOOL=ON -DHWY_SYSTEM_GTEST:BOOL=ON -DHWY_CMAKE_RVV:BOOL=OFF -DCMAKE_BUILD_TYPE=Release

RUN ninja -v -C build/ bit_pack_test

RUN ./build/tests/bit_pack_test || true``
```

---

### Comment 12 - w2yehia

@tstellar since you mention you'll bisect, i'll wait for the results then investigate on our end.
Yesterday I was able to reproduce the problem using the original instructions:
```
$ /usr/bin/clang++ -DGTEST_LINKED_AS_SHARED_LIBRARY=1 -DHWY_SHARED_DEFINE -DTOOLCHAIN_MISS_ASM_HWCAP_H -I/root/rpmbuild/BUILD/highway-1.2.0-build/highway-1.2.0 -O2 -flto=thin -ffat-lto-objects -fexceptions -g -grecord-gcc-switches -pipe -Wall -Werror=format-security -Wp,-U_FORTIFY_SOURCE,-D_FORTIFY_SOURCE=3 -Wp,-D_GLIBCXX_ASSERTIONS --config=/usr/lib/rpm/redhat/redhat-hardened-clang.cfg -fstack-protector-strong   -m64 -mcpu=power8 -mtune=power8 -fasynchronous-unwind-tables -fstack-clash-protection -O2 -g -DNDEBUG -std=c++17 -fPIE -fvisibility=hidden -fvisibility-inlines-hidden -Wno-builtin-macro-redefined -D__DATE__=\"redacted\" -D__TIMESTAMP__=\"redacted\" -D__TIME__=\"redacted\" -fmerge-all-constants -Wall -Wextra -Wconversion -Wsign-conversion -Wvla -Wnon-virtual-dtor -Wcast-align -Wfloat-overflow-conversion -Wfloat-zero-conversion -Wfor-loop-analysis -Wgnu-redeclared-enum -Winfinite-recursion -Wself-assign -Wstring-conversion -Wtautological-overlap-compare -Wthread-safety-analysis -Wundefined-func-template -fno-cxx-exceptions -fno-slp-vectorize -fno-vectorize -fdiagnostics-show-option -fcolor-diagnostics -Wc++2a-extensions -fmath-errno -fno-exceptions -Wno-psabi -DHWY_IS_TEST=1 -MD -MT CMakeFiles/bit_pack_test.dir/hwy/contrib/bit_pack/bit_pack_test.cc.o -MF CMakeFiles/bit_pack_test.dir/hwy/contrib/bit_pack/bit_pack_test.cc.o.d -o CMakeFiles/bit_pack_test.dir/hwy/contrib/bit_pack/bit_pack_test.cc.o -c /root/rpmbuild/BUILD/highway-1.2.0-build/highway-1.2.0/hwy/contrib/bit_pack/bit_pack_test.cc \
  && /usr/bin/clang++ -O2 -fexceptions -g -grecord-gcc-switches -pipe -Wall -Werror=format-security -Wp,-U_FORTIFY_SOURCE,-D_FORTIFY_SOURCE=3 -Wp,-D_GLIBCXX_ASSERTIONS --config=/usr/lib/rpm/redhat/redhat-hardened-clang.cfg -fstack-protector-strong   -m64 -mcpu=power8 -mtune=power8 -fasynchronous-unwind-tables -fstack-clash-protection -O2 -g -DNDEBUG -Wl,-z,relro -Wl,--as-needed  -Wl,-z,pack-relative-relocs -Wl,-z,now --config=/usr/lib/rpm/redhat/redhat-hardened-clang-ld.cfg  -Wl,--build-id=sha1  -fuse-ld=lld -fPIE -pie -Xlinker --dependency-file=CMakeFiles/bit_pack_test.dir/link.d CMakeFiles/bit_pack_test.dir/hwy/contrib/bit_pack/bit_pack_test.cc.o -Wl,-rpath,/root/rpmbuild/BUILD/highway-1.2.0-build/highway-1.2.0/redhat-linux-build libhwy_test.so.1.2.0 libhwy_contrib.so.1.2.0 /usr/lib64/libgtest_main.so.1.15.2 libhwy.so.1.2.0 /usr/lib64/libgtest.so.1.15.2 \
   -o tests/bit_pack_test.lto -flto=thin -ffat-lto-objects \
  && /usr/bin/clang++ -O2 -fexceptions -g -grecord-gcc-switches -pipe -Wall -Werror=format-security -Wp,-U_FORTIFY_SOURCE,-D_FORTIFY_SOURCE=3 -Wp,-D_GLIBCXX_ASSERTIONS --config=/usr/lib/rpm/redhat/redhat-hardened-clang.cfg -fstack-protector-strong   -m64 -mcpu=power8 -mtune=power8 -fasynchronous-unwind-tables -fstack-clash-protection -O2 -g -DNDEBUG -Wl,-z,relro -Wl,--as-needed  -Wl,-z,pack-relative-relocs -Wl,-z,now --config=/usr/lib/rpm/redhat/redhat-hardened-clang-ld.cfg  -Wl,--build-id=sha1  -fuse-ld=lld -fPIE -pie -Xlinker --dependency-file=CMakeFiles/bit_pack_test.dir/link.d CMakeFiles/bit_pack_test.dir/hwy/contrib/bit_pack/bit_pack_test.cc.o -Wl,-rpath,/root/rpmbuild/BUILD/highway-1.2.0-build/highway-1.2.0/redhat-linux-build libhwy_test.so.1.2.0 libhwy_contrib.so.1.2.0 /usr/lib64/libgtest_main.so.1.15.2 libhwy.so.1.2.0 /usr/lib64/libgtest.so.1.15.2 \
  -o tests/bit_pack_test.nolto
$
$ tests/bit_pack_test.nolto
Running main() from /builddir/build/BUILD/gtest-1.15.2-build/googletest-1.15.2/googletest/src/gtest_main.cc
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from BitPackTestGroup/BitPackTest
[ RUN      ] BitPackTestGroup/BitPackTest.TestAllPack16/PPC9
[       OK ] BitPackTestGroup/BitPackTest.TestAllPack16/PPC9 (0 ms)
[ RUN      ] BitPackTestGroup/BitPackTest.TestAllPack16/PPC8
[       OK ] BitPackTestGroup/BitPackTest.TestAllPack16/PPC8 (0 ms)
[ RUN      ] BitPackTestGroup/BitPackTest.TestAllPack16/EMU128
[       OK ] BitPackTestGroup/BitPackTest.TestAllPack16/EMU128 (0 ms)
[----------] 3 tests from BitPackTestGroup/BitPackTest (0 ms total)

[----------] Global test environment tear-down
[==========] 3 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 3 tests.

$ tests/bit_pack_test.lto
Running main() from /builddir/build/BUILD/gtest-1.15.2-build/googletest-1.15.2/googletest/src/gtest_main.cc
[==========] Running 3 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 3 tests from BitPackTestGroup/BitPackTest
[ RUN      ] BitPackTestGroup/BitPackTest.TestAllPack16/PPC9
[       OK ] BitPackTestGroup/BitPackTest.TestAllPack16/PPC9 (0 ms)
[ RUN      ] BitPackTestGroup/BitPackTest.TestAllPack16/PPC8
[       OK ] BitPackTestGroup/BitPackTest.TestAllPack16/PPC8 (0 ms)
[ RUN      ] BitPackTestGroup/BitPackTest.TestAllPack16/EMU128
Abort at bit_pack_test.cc:68: 3 bits: pos 12 of 512, expected 1 actual 0

Aborted (core dumped)
```

---

### Comment 13 - tstellar

I was mistaken, I can't actually reproduce without lto, so 3469db82b5c821c94b58c0b81f03bbef51efa30b is indeed the commit where this test first started failing.

---

### Comment 14 - w2yehia

I was able to reproduce without LTO and narrow down to the SLPVectorizerPass with the following options:
```
fail: -mllvm -slp-max-vf=4 -mllvm -slp-threshold=11 -mllvm -opt-bisect-limit=$BL  # didn't put the actual BL here because i've simplified the test
pass: -mllvm -slp-max-vf=4 -mllvm -slp-threshold=12 -mllvm -opt-bisect-limit=$BL
```

The good (non-vectorized) and bad (vectorized) IR look semantically equivalent, so likely it's a codegen issue. Still investigating..

---

### Comment 15 - tstellar

@w2yehia Can you post the input IR that you used for reproducing?

---

### Comment 16 - w2yehia

[pass.func.ll.txt](https://github.com/user-attachments/files/20781961/pass.func.ll.txt)
[fail.func.ll.txt](https://github.com/user-attachments/files/20781960/fail.func.ll.txt)
Sure, here's the pass/fail IRs for function `_ZNK3hwy8N_EMU1286Pack16ILm3EE6UnpackINS0_4SimdItLm2ELi0EEEEEvT_PKtPt`

---

### Comment 17 - w2yehia

I narrowed down the issue to a wrong mask constant, and the issue seems to be during IR to Machine IR lowering.
Specifically, I think this IR
```
  %3 = shufflevector <2 x i32> %2, <2 x i32> poison, <4 x i32> <i32 0, i32 1, i32 poison, i32 0>
  %4 = insertelement <4 x i32> %3, i32 %0, i32 2
```
is generating the following constant mask
```
const.0: <i8 31, i8 30, i8 29, i8 28, i8 27, i8 26, i8 25, i8 24,   i8 7, i8 6, i8 5, i8 4,    i8 19, i8 18, i8 17, i8 16>
```
but should be
```
fixed:   <i8 31, i8 30, i8 29, i8 28, i8 27, i8 26, i8 25, i8 24,   i8 7, i8 6, i8 5, i8 4,    i8 23, i8 22, i8 21, i8 20>
```

---

