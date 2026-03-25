# Backend crash due to SmallVector reaching max capacity.

**Author:** ilovepi
**URL:** https://github.com/llvm/llvm-project/issues/165755
**Status:** Closed
**Labels:** backend:X86, crash
**Closed Date:** 2025-10-31T11:32:02Z

## Body

We found this when qualifying a new toolchain. The revision range is between 8b93f27cf7e6e53636db870873b53269efa3cca4 and 242ebcf13e037df08725150ad39ae156bb0ab1b0. 

I was wondering if anyone else has run into this? I've been at the dev meeting this week, and we weren't paying all that much attention to our CI while that was going on.

Error output:
```
[98810/154458](639) CXX x64-coverage/obj/src/lib/elfldltl/test/elfldltl-unittests.symbol-tests.cc.o
FAILED: [code=1] x64-coverage/obj/src/lib/elfldltl/test/elfldltl-unittests.symbol-tests.cc.o 
../../build/rbe/reclient_cxx.sh --working-subdir=out/not-default --exec_strategy=remote_local_fallback --preserve_unchanged_output_mtime --  ../../prebuilt/third_party/clang/linux-x64/bin/clang++ -MD -MF x64-coverage/obj/src/lib/elfldltl/test/elfldltl-unittests.symbol-tests.cc.o.d -DTOOLCHAIN_VERSION=i2065Fhwg9LXYjwVynnninFWj46HGvsLAgGZHfBE8dMC -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D_LIBCPP_REMOVE_TRANSITIVE_INCLUDES -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS=1 -DZX_ASSERT_LEVEL=2 -D_LIBCPP_ENABLE_REMOVED_ALLOCATOR_CONST -I../.. -Ix64-coverage/gen -I../../src/lib/elfldltl/include -I../../sdk/lib/fit/include -I../../sdk/lib/stdcompat/include -I../../zircon/system/ulib/fbl/include -I../../zircon/system/ulib/zx/include -I../../src/zircon/lib/zircon/include -Ifidling/gen/zircon/vdso/zx/zither/legacy_syscall_cdecl -I../../src/lib/elfldltl/testing/include -I../../third_party/googletest/src/googlemock/include -I../../third_party/googletest/src/googletest/include -I../../sdk -Ix64-coverage/gen/sdk -I../../sdk/lib/fit-promise/include -I../../sdk/lib/syslog/cpp/include -I../../sdk/lib/async-loop/include -I../../sdk/lib/async/include -Ifidling/gen/sdk/fidl/fuchsia.logger/fuchsia.logger/cpp -Ifidling/gen/sdk/fidl/fuchsia.diagnostics/fuchsia.diagnostics/cpp -Ifidling/gen/sdk/fidl/fuchsia.diagnostics.types/fuchsia.diagnostics.types/cpp -I../../sdk/lib/fidl/cpp/include -I../../sdk/lib/fidl/cpp/wire/include -I../../sdk/lib/fidl_base/include -I../../zircon/system/ulib/sync/include -I../../sdk/lib/fidl/include -Ifidling/gen/sdk/fidl/fuchsia.mem/fuchsia.mem/cpp -I../../src/lib/symbolizer-markup/include -I../../src/lib/trivial-allocator/include -I../../sdk/lib/fdio/include -fcolor-diagnostics -fcrash-diagnostics-dir=clang-crashreports -fcrash-diagnostics=all -gen-reproducer=error -fstack-size-section -fstack-clash-protection -ffp-contract=off --sysroot=x64-coverage/gen/zircon/public/sysroot --target=x86_64-unknown-fuchsia -ffuchsia-api-level=4293918720 -march=x86-64-v2 -mtune=generic -mbranches-within-32B-boundaries -ffile-compilation-dir=. -no-canonical-prefixes -fno-omit-frame-pointer -momit-leaf-frame-pointer -fdata-sections -ffunction-sections -O1 -Xclang -debug-info-kind=constructor -g3 -grecord-gcc-switches -gdwarf-5 -gz=zstd -Wall -Wextra -Wconversion -Wextra-semi -Wimplicit-fallthrough -Wnewline-eof -Wstrict-prototypes -Wwrite-strings -Wno-sign-conversion -Wno-unused-parameter -Wnonportable-system-include-path -Wno-missing-field-initializers -Wno-extra-qualification -Wno-cast-function-type-mismatch -Wno-unknown-warning-option -Wno-missing-template-arg-list-after-template-kw -Wno-deprecated-pragma -Wno-nontrivial-memaccess -fvisibility=hidden -Werror -Wa,--fatal-warnings -ftrivial-auto-var-init=pattern -Wthread-safety -Wno-unknown-warning-option -Wno-thread-safety-reference-return -mllvm -profile-correlate=binary -mllvm -enable-single-byte-coverage=true -mllvm -system-headers-coverage -fprofile-instr-generate -fcoverage-mapping -fprofile-list=x64-coverage/profile-source-files.list -Wno-undef -Wno-character-conversion -fvisibility-inlines-hidden -std=c++20 -Wno-deprecated-this-capture -fno-exceptions -fno-rtti -ftemplate-backtrace-limit=0 -c ../../src/lib/elfldltl/test/symbol-tests.cc -o x64-coverage/obj/src/lib/elfldltl/test/elfldltl-unittests.symbol-tests.cc.o
fatal error: error in backend: SmallVector unable to grow. Requested capacity (4294967296) is larger than maximum value for size type (4294967295)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Fuchsia clang version 22.0.0git (https://llvm.googlesource.com/llvm-project 242ebcf13e037df08725150ad39ae156bb0ab1b0)
Target: x86_64-unknown-fuchsia
Thread model: posix
InstalledDir: ../../prebuilt/third_party/clang/linux-x64/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: clang-crashreports/symbol-tests-e49965.cpp
clang++: note: diagnostic msg: clang-crashreports/symbol-tests-e49965.sh
clang++: note: diagnostic msg: 

********************
```

bot: https://ci.chromium.org/ui/p/fuchsia/builders/try/core.x64-coverage-qemu_kvm-build_only/b8699572448760009713/overview


I'm trying to reduce the following reproducer:

[symbol-fuzzer-c2c81e.cpp](https://github.com/user-attachments/files/23242153/symbol-fuzzer-c2c81e.cpp)
[symbol-fuzzer-c2c81e.sh](https://github.com/user-attachments/files/23242154/symbol-fuzzer-c2c81e.sh)

## Comments

### Comment 1 - ilovepi

We seem to be tripping over this assert now that I have an asserts build available: https://github.com/llvm/llvm-project/blob/bb8261b7f3863350848751de9d5f3f52cf692962/llvm/lib/CodeGen/SelectionDAG/LegalizeTypes.cpp#L489

Seems like something is going wrong during instruction selection. 

My creduce is still running. I had some trouble reproducing w/ llc, unfortunately, so I'm unable to use llvm-reduce, ATM.

---

### Comment 2 - ilovepi

```llvm
; RUN: llc %s

; ModuleID = 'reduced.bc'
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-fuchsia"

define i32 @foo(ptr %0) {
  %2 = load i64, ptr %0, align 8
  store i8 0, ptr %0, align 1
  %3 = load i32, ptr %0, align 8
  %4 = and i32 %3, 32
  %5 = zext i32 %4 to i64
  %6 = lshr i64 %2, %5
  %7 = trunc i64 %6 to i32
  ret i32 %7
}
```

---

### Comment 3 - ilovepi

bisected to e588c7fa713d8bdd5c424831ca42136b560ff66b

@RKSimon could you take a look, since you're the author of #165266 ?

---

### Comment 4 - RKSimon

Looks like some nasty chain inter dependencies :/ 

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Paul Kirth (ilovepi)

<details>
We found this when qualifying a new toolchain. The revision range is between 8b93f27cf7e6e53636db870873b53269efa3cca4 and 242ebcf13e037df08725150ad39ae156bb0ab1b0. 

I was wondering if anyone else has run into this? I've been at the dev meeting this week, and we weren't paying all that much attention to our CI while that was going on.

Error output:
```
[98810/154458](639) CXX x64-coverage/obj/src/lib/elfldltl/test/elfldltl-unittests.symbol-tests.cc.o
FAILED: [code=1] x64-coverage/obj/src/lib/elfldltl/test/elfldltl-unittests.symbol-tests.cc.o 
../../build/rbe/reclient_cxx.sh --working-subdir=out/not-default --exec_strategy=remote_local_fallback --preserve_unchanged_output_mtime --  ../../prebuilt/third_party/clang/linux-x64/bin/clang++ -MD -MF x64-coverage/obj/src/lib/elfldltl/test/elfldltl-unittests.symbol-tests.cc.o.d -DTOOLCHAIN_VERSION=i2065Fhwg9LXYjwVynnninFWj46HGvsLAgGZHfBE8dMC -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D_LIBCPP_REMOVE_TRANSITIVE_INCLUDES -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS=1 -DZX_ASSERT_LEVEL=2 -D_LIBCPP_ENABLE_REMOVED_ALLOCATOR_CONST -I../.. -Ix64-coverage/gen -I../../src/lib/elfldltl/include -I../../sdk/lib/fit/include -I../../sdk/lib/stdcompat/include -I../../zircon/system/ulib/fbl/include -I../../zircon/system/ulib/zx/include -I../../src/zircon/lib/zircon/include -Ifidling/gen/zircon/vdso/zx/zither/legacy_syscall_cdecl -I../../src/lib/elfldltl/testing/include -I../../third_party/googletest/src/googlemock/include -I../../third_party/googletest/src/googletest/include -I../../sdk -Ix64-coverage/gen/sdk -I../../sdk/lib/fit-promise/include -I../../sdk/lib/syslog/cpp/include -I../../sdk/lib/async-loop/include -I../../sdk/lib/async/include -Ifidling/gen/sdk/fidl/fuchsia.logger/fuchsia.logger/cpp -Ifidling/gen/sdk/fidl/fuchsia.diagnostics/fuchsia.diagnostics/cpp -Ifidling/gen/sdk/fidl/fuchsia.diagnostics.types/fuchsia.diagnostics.types/cpp -I../../sdk/lib/fidl/cpp/include -I../../sdk/lib/fidl/cpp/wire/include -I../../sdk/lib/fidl_base/include -I../../zircon/system/ulib/sync/include -I../../sdk/lib/fidl/include -Ifidling/gen/sdk/fidl/fuchsia.mem/fuchsia.mem/cpp -I../../src/lib/symbolizer-markup/include -I../../src/lib/trivial-allocator/include -I../../sdk/lib/fdio/include -fcolor-diagnostics -fcrash-diagnostics-dir=clang-crashreports -fcrash-diagnostics=all -gen-reproducer=error -fstack-size-section -fstack-clash-protection -ffp-contract=off --sysroot=x64-coverage/gen/zircon/public/sysroot --target=x86_64-unknown-fuchsia -ffuchsia-api-level=4293918720 -march=x86-64-v2 -mtune=generic -mbranches-within-32B-boundaries -ffile-compilation-dir=. -no-canonical-prefixes -fno-omit-frame-pointer -momit-leaf-frame-pointer -fdata-sections -ffunction-sections -O1 -Xclang -debug-info-kind=constructor -g3 -grecord-gcc-switches -gdwarf-5 -gz=zstd -Wall -Wextra -Wconversion -Wextra-semi -Wimplicit-fallthrough -Wnewline-eof -Wstrict-prototypes -Wwrite-strings -Wno-sign-conversion -Wno-unused-parameter -Wnonportable-system-include-path -Wno-missing-field-initializers -Wno-extra-qualification -Wno-cast-function-type-mismatch -Wno-unknown-warning-option -Wno-missing-template-arg-list-after-template-kw -Wno-deprecated-pragma -Wno-nontrivial-memaccess -fvisibility=hidden -Werror -Wa,--fatal-warnings -ftrivial-auto-var-init=pattern -Wthread-safety -Wno-unknown-warning-option -Wno-thread-safety-reference-return -mllvm -profile-correlate=binary -mllvm -enable-single-byte-coverage=true -mllvm -system-headers-coverage -fprofile-instr-generate -fcoverage-mapping -fprofile-list=x64-coverage/profile-source-files.list -Wno-undef -Wno-character-conversion -fvisibility-inlines-hidden -std=c++20 -Wno-deprecated-this-capture -fno-exceptions -fno-rtti -ftemplate-backtrace-limit=0 -c ../../src/lib/elfldltl/test/symbol-tests.cc -o x64-coverage/obj/src/lib/elfldltl/test/elfldltl-unittests.symbol-tests.cc.o
fatal error: error in backend: SmallVector unable to grow. Requested capacity (4294967296) is larger than maximum value for size type (4294967295)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Fuchsia clang version 22.0.0git (https://llvm.googlesource.com/llvm-project 242ebcf13e037df08725150ad39ae156bb0ab1b0)
Target: x86_64-unknown-fuchsia
Thread model: posix
InstalledDir: ../../prebuilt/third_party/clang/linux-x64/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: clang-crashreports/symbol-tests-e49965.cpp
clang++: note: diagnostic msg: clang-crashreports/symbol-tests-e49965.sh
clang++: note: diagnostic msg: 

********************
```

bot: https://ci.chromium.org/ui/p/fuchsia/builders/try/core.x64-coverage-qemu_kvm-build_only/b8699572448760009713/overview


I'm trying to reduce the following reproducer:

[symbol-fuzzer-c2c81e.cpp](https://github.com/user-attachments/files/23242153/symbol-fuzzer-c2c81e.cpp)
[symbol-fuzzer-c2c81e.sh](https://github.com/user-attachments/files/23242154/symbol-fuzzer-c2c81e.sh)
</details>


---

