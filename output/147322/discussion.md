# Commit "MCAssembler: Simplify fixup handling" (922dde3c64) regresses RISC-V XRay tests

**Author:** asb
**URL:** https://github.com/llvm/llvm-project/issues/147322
**Status:** Closed
**Labels:** backend:RISC-V, xray, regression, llvm:mc
**Closed Date:** 2025-07-10T14:31:36Z

## Body

922dde3c6403c7ef61f3c21af485a7feeb931a09 broke XRay-riscv64-linux::coverage-sample.cpp (and I assume XRay-riscv64-linux::func-id-utils.cpp as well, which broke in the same batch of commits).


This is the bisect script which shows how to reproduce from an x86 host. Apologies for the hacky addition of QEMU_LD_PREFIX:

```
cat - <<'EOF' > build_and_test.sh
#!/bin/sh

export QEMU_LD_PREFIX=$HOME/rvsysroot # HACK as there's a python3 CMake issue with the runtimes otherwise
rm -rf build/rvregression
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
cmake --build build/rvregression --target runtimes || exit 125
./build/rvregression/bin/clang --target=riscv64-linux-gnu --sysroot=$HOME/rvsysroot --driver-mode=g++ -fxray-instrument -std=c++11 compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp || exit 1
./a.out || exit 1
EOF
chmod +x build_and_test.sh

BAD=dd2891535d4b6d80f92d9a69e9db3b787203b9d0
GOOD=a9ee1797b716fa61f495a2400f95da4594a8ae80
git bisect start $BAD $GOOD
git bisect run ./build_and_test.sh
```

Prior to the patch, the output is:
```
first pass.
patched: 1
patched: 2
patched: 3
second pass.
patched: 1
patched: 2
patched: 3
missing 1.
patched: 2
patched: 3
```

After the patch, the output is:
```
==3238438==Failed patching any sled for function '3'.==3238438==Failed patching any sled for function '2'.==3238438==Failed patching any sled for function '1'.==3238438==Failed patching any sled for function '1'.==3238438==Failed patching any sled for function '2'.==3238438==Failed patching any sled for function '3'.first pass.
patched: 1
patched: 2
patched: 3
==3238438==Failed patching any sled for function '1'.a.out.bad: compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp:64: int main(int, char **): Assertion `__xray_patch_function(id) == XRayPatchingStatus::SUCCESS' failed.
Aborted (core dumped)
```

The patch doesn't revert cleanly currently. I'm wondering @MaskRay if the likely issue is obvious to you.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Alex Bradbury (asb)

<details>
922dde3c6403c7ef61f3c21af485a7feeb931a09 broke XRay-riscv64-linux::coverage-sample.cpp (and I assume XRay-riscv64-linux::func-id-utils.cpp as well, which broke in the same batch of commits).


This is the bisect script which shows how to reproduce from an x86 host. Apologies for the hacky addition of QEMU_LD_PREFIX:

```
cat - &lt;&lt;'EOF' &gt; build_and_test.sh
#!/bin/sh

export QEMU_LD_PREFIX=$HOME/rvsysroot # Needed as there's a python3 issue with the runtimes otherwise?
rm -rf build/rvregression
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
cmake --build build/rvregression --target runtimes || exit 125
./build/rvregression/bin/clang --target=riscv64-linux-gnu --sysroot=$HOME/rvsysroot --driver-mode=g++ -fxray-instrument -std=c++11 compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp || exit 1
./a.out || exit 1
EOF
chmod +x build_and_test.sh

BAD=dd2891535d4b6d80f92d9a69e9db3b787203b9d0
GOOD=a9ee1797b716fa61f495a2400f95da4594a8ae80
git bisect start $BAD $GOOD
git bisect run ./build_and_test.sh
```

Prior to the patch, the output is:
```
first pass.
patched: 1
patched: 2
patched: 3
second pass.
patched: 1
patched: 2
patched: 3
missing 1.
patched: 2
patched: 3
```

After the patch, the output is:
```
==3238438==Failed patching any sled for function '3'.==3238438==Failed patching any sled for function '2'.==3238438==Failed patching any sled for function '1'.==3238438==Failed patching any sled for function '1'.==3238438==Failed patching any sled for function '2'.==3238438==Failed patching any sled for function '3'.first pass.
patched: 1
patched: 2
patched: 3
==3238438==Failed patching any sled for function '1'.a.out.bad: compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp:64: int main(int, char **): Assertion `__xray_patch_function(id) == XRayPatchingStatus::SUCCESS' failed.
Aborted (core dumped)
```

The patch doesn't revert cleanly currently. I'm wondering @<!-- -->MaskRay if the likely issue is obvious to you.
</details>


---

### Comment 2 - MaskRay

Thanks for the detailed reproduce!

I have created a riscv64 sysroot with
```
sudo debootstrap --arch=riscv64 --variant=minbase --include=build-essential,symlinks unstable ~/sysroot/riscv64
```

After 922dde3c6403c7ef61f3c21af485a7feeb931a09, we call `shouldInsertFixupForCodeAlign` `if (AF->hasEmitNops())` instead of `if (Sec.useCodeAlign() && AF.hasEmitNops())`.
The change is intended, as `hasEmitNops` is intended to only be used in a code section.

The `xray_fn_idx` section does not contain code, but the xray instrumentation incorrectly uses `emitCodeAlignment` at llvm/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:4635


---

### Comment 3 - MaskRay

Fixed the xray instrumentation in commit 68494ae072676726722d87af6d9fd9e7c8b808bb . I cannot verify as
`/t/rv/bin/clang++ --target=riscv64-linux-gnu --sysroot=$HOME/sysroot/riscv64 -fxray-instrument -std=c++11 compiler-rt/test/xray/TestCases/Posix/coverage-sample.cpp   -o /t/new --ld-path=$HOME/sysroot/riscv64/usr/bin/ld; /t/new` passed even without the patch..

---

### Comment 4 - asb

Thank you! I can confirm the bot is now green and both failing tests are now fixed. Marking closed.

---

### Comment 5 - MaskRay

> Thank you! I can confirm the bot is now green and both failing tests are now fixed. Marking closed.

No problem! If you happen to have extra cycles, I'd appreciate it if you could help me understand what went wrong. The redundant R_RISCV_ALIGN relocations in the non-code section xray_fn_idx should be benign for both GNU ld and LLD, unless there's something I'm missing...

---

