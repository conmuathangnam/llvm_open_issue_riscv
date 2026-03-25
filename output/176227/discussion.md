# [RISCV] Crash in Machine Instruction Scheduler for invalid global physical register w/ a fixed register

**Author:** ilovepi
**URL:** https://github.com/llvm/llvm-project/issues/176227

## Body

We're seeing some compiler crashes in codegen for RISC-V in Fuchsia builds. This is a new build configuration in our CI, so its unclear when this may have been introduced. I have a suspicion that this is related to the recent changes to the default scheduling model, but I haven't started bisecting yet.

Error from CI:
```
FAILED: [code=1] kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o 
../../prebuilt/third_party/clang/custom/bin/clang++ -MD -MF kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o.d -o kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS=1 -DSMP_MAX_CPUS=16 -D_KERNEL -DLK -DENABLE_PANIC_SHELL -DZIRCON_TOOLCHAIN -DLK_DEBUGLEVEL=2 -DDEBUG_PRINT_LEVEL=2 -DVM_TRACING_LEVEL=0 -DFUTEX_BLOCK_TRACING_ENABLED=false -DLOCK_NAME_TRACING_ENABLED=false -DEXPERIMENTAL_THREAD_SAMPLER_ENABLED=false -DEXPERIMENTAL_KTRACE_STREAMING_ENABLED=true -DEXPERIMENTAL_FORCE_NEW_WAKEUP_ACCOUNTING=false -DCHANNEL_MESSAGE_BODY_TRACING_ENABLED=false -DJTRACE_TARGET_BUFFER_SIZE=0 -DSCHEDULER_TRACING_LEVEL=0 -DSCHEDULER_QUEUE_TRACING_ENABLED=false -DSCHEDULER_EXTRA_INVARIANT_VALIDATION=false -DSCHEDULER_LOCK_SPIN_TRACING_COMPRESSED=false -DSCHEDULER_LOCK_SPIN_TRACING_ENABLED=false -DWAIT_QUEUE_DEPTH_TRACING_ENABLED=false -DKERNEL_IMAGE_MAX_SIZE=14680064 -DWITH_FRAME_POINTERS=1 -DZX_ASSERT_LEVEL=2 -DMIN_CRASHLOG_SIZE=2048 -DPERSISTENT_RAM_ALLOCATION_GRANULARITY=128 -DKERNEL_MEMORY_PROFILER=false -DLIB_ARCH_PAGING_CONFIGURATION=\"sv39\" -I../../zircon/kernel/include -I../../zircon/kernel/lib/libc/include -I../../zircon/kernel/lib/ktl/include -I../../sdk/lib/fit/include -I../../zircon/kernel/lib/heap/include -I../../zircon/kernel/lib/lockup_detector/include -I../../zircon/system/ulib/lazy_init/include -I../../zircon/system/ulib/lockdep/include -I../../zircon/system/ulib/ffl/include -I../../zircon/kernel/lib/kconcurrent/include -I../../zircon/system/ulib/concurrent/include -I../../zircon/kernel/vm/include -I../../zircon/kernel/vm/phys/include -I../../zircon/kernel/lib/user_copy/include -I../../zircon/kernel/lib/acpi_lite/include -I../../zircon/kernel/lib/boot-options/include -I../../zircon/kernel/lib/devicetree/include -I../../zircon/system/ulib/uart/include -I../../zircon/system/ulib/hwreg/include -I../../zircon/kernel/phys/lib/memalloc/include -I../../zircon/system/ulib/zircon-internal/include -I../../zircon/kernel/lib/spsc_buffer/include -I../../src/performance/lib/fxt/include -I../../zircon/kernel/lib/special-sections/include -I../../zircon/system/ulib/fbl/include -I../../zircon/kernel/lib/fbl/include -I../../zircon/system/ulib/mmio-ptr/include -I../../zircon/kernel/lib/id_allocator/include -I../../zircon/system/ulib/zx/include -I../../sdk/lib/stdcompat/include -I../../zircon/kernel/lib/backtrace/include -I../../zircon/kernel/lib/version/include -I../../zircon/kernel/lib/wake-vector/include -I../../zircon/kernel/lib/sched/include -I../../zircon/kernel/lib/arch/riscv64/include -I../../zircon/kernel/lib/arch/include -I../../sdk/lib/zbi-format/include -I../../zircon/kernel/lib/power-management/include -I../../zircon/kernel/lib/stall/include -I../../zircon/system/public -I../../sdk/lib/zircon-assert -I../../zircon/kernel/arch/riscv64/include -I../../zircon/kernel/dev/iommu/stub/include -I../../zircon/kernel/dev/pdev/power/include -I../../zircon/kernel/dev/pdev/timer/include -Igen/zircon/kernel/lib/arch/gen-arm64-feature-asm.include -Igen/zircon/kernel/lib/arch/gen-arm64-system-asm.include -Igen/zircon/kernel/lib/arch/gen-riscv64-system-asm.include -Igen/zircon/kernel/lib/arch/gen-x86-msr-asm.include -Igen/zircon/kernel/lib/arch/gen-x86-cpuid-asm.include -I../../src/zircon/lib/zircon/include -Ifidling/gen/zircon/vdso/zx/zither/legacy_syscall_cdecl -I../../zircon/kernel/lib/code-patching/include -Igen/zircon/kernel/lib/code-patching/case-id-asm.include -I../../src/lib/zbitl/include -I../../zircon/third_party/ulib/cksum/include -I../../zircon/kernel/lib/efi/include -I../../zircon/kernel/arch/riscv64/code-patches/include -I../../zircon/kernel/lib/console/include -I../../zircon/kernel/lib/counters/include -I../../zircon/kernel/lib/crashlog/include -I../../zircon/kernel/lib/init/include -I../../zircon/kernel/lib/page/include -I../../zircon/kernel/lib/syscalls/include -Ifidling/gen/zircon/vdso/zx/zither/kernel -I../../zircon/kernel/lib/thread-stack/include -I../../zircon/kernel/phys/include -I../../zircon/kernel/arch/riscv64/phys/include -I../../zircon/kernel/lib/unittest/include -I../../zircon/kernel/object/include -I../../zircon/kernel/lib/crypto/include -I../../zircon/third_party/lib/jitterentropy/include -I../../zircon/kernel/lib/page_cache/include -I../../zircon/kernel/lib/instrumentation/include -I../../zircon/kernel/lib/object_cache/include -I../../zircon/kernel/dev/pcie/include -I../../zircon/kernel/dev/interrupt/include -I../../zircon/system/ulib/region-alloc/include -I../../zircon/kernel/lib/debuglog/include -I../../zircon/system/ulib/affine/include -I../../src/lib/elfldltl/include -I../../zircon/system/ulib/bitmap/include -I../../zircon/system/ulib/pretty/include -idirafter ../../zircon/kernel/lib/libc/include-after -idirafter ../../zircon/kernel/lib/ktl/include-after -Xclang -debug-info-kind=constructor -g3 -grecord-gcc-switches -gdwarf-4 -gz=zstd -fdata-sections -ffunction-sections -O2 -Wall -Wextra -Wconversion -Wextra-semi -Wimplicit-fallthrough -Wnewline-eof -Wstrict-prototypes -Wwrite-strings -Wno-sign-conversion -Wno-unused-parameter -Wnonportable-system-include-path -Wno-missing-field-initializers -Wno-extra-qualification -Wno-cast-function-type-mismatch -Wno-unknown-warning-option -Wno-missing-template-arg-list-after-template-kw -Wno-deprecated-pragma -Wno-nontrivial-memaccess -ftrivial-auto-var-init=pattern -ffile-compilation-dir=. -no-canonical-prefixes -fvisibility=hidden -Wthread-safety -Wno-unknown-warning-option -Wno-thread-safety-reference-return -Werror -Wa,--fatal-warnings -fno-common -fsized-deallocation --target=riscv64-fuchsia -msmall-data-limit=0 -march=rv64gcv_zihintpause_zba_zbb_zbs_zicbom_zicbop_zicboz_zfhmin_zkt -mabi=lp64d -fcolor-diagnostics -fcrash-diagnostics-dir=clang-crashreports -fcrash-diagnostics=all -gen-reproducer=error -fstack-size-section -ffuchsia-api-level=4293918720 -fpie -ffreestanding -include ../../zircon/kernel/include/hidden.h -fno-unwind-tables -Wformat=2 -Wmissing-declarations -Wvla -Wshadow -march=rv64imac_zihintpause_zba_zbb_zbs_zicbom_zicbop_zicboz_zfhmin_zkt_zicsr_zifencei -mabi=lp64 -ffixed-x27 -fno-sanitize=safe-stack -fsanitize=shadow-call-stack -fno-finite-loops -fdata-sections -fno-omit-frame-pointer -momit-leaf-frame-pointer -fno-omit-frame-pointer -momit-leaf-frame-pointer -idirafter ../../zircon/kernel/lib/ktl/include-after -idirafter ../../zircon/kernel/lib/libc/include-after -idirafter ../../zircon/kernel/lib/ktl/include-after -Wno-deprecated-this-capture -std=c++20 -fno-exceptions -fno-rtti -Xclang -fexperimental-omit-vtable-rtti -fvisibility-inlines-hidden -ftemplate-backtrace-limit=0 -faligned-new=8 -fno-exceptions -c ../../zircon/kernel/arch/riscv64/mp.cc
The register $x27 needs to be live in to %bb.2, but is missing from the live-in list.
fatal error: error in backend: Invalid global physical register
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: ../../prebuilt/third_party/clang/custom/bin/clang++ -MD -MF kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o.d -o kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS=1 -DSMP_MAX_CPUS=16 -D_KERNEL -DLK -DENABLE_PANIC_SHELL -DZIRCON_TOOLCHAIN -DLK_DEBUGLEVEL=2 -DDEBUG_PRINT_LEVEL=2 -DVM_TRACING_LEVEL=0 -DFUTEX_BLOCK_TRACING_ENABLED=false -DLOCK_NAME_TRACING_ENABLED=false -DEXPERIMENTAL_THREAD_SAMPLER_ENABLED=false -DEXPERIMENTAL_KTRACE_STREAMING_ENABLED=true -DEXPERIMENTAL_FORCE_NEW_WAKEUP_ACCOUNTING=false -DCHANNEL_MESSAGE_BODY_TRACING_ENABLED=false -DJTRACE_TARGET_BUFFER_SIZE=0 -DSCHEDULER_TRACING_LEVEL=0 -DSCHEDULER_QUEUE_TRACING_ENABLED=false -DSCHEDULER_EXTRA_INVARIANT_VALIDATION=false -DSCHEDULER_LOCK_SPIN_TRACING_COMPRESSED=false -DSCHEDULER_LOCK_SPIN_TRACING_ENABLED=false -DWAIT_QUEUE_DEPTH_TRACING_ENABLED=false -DKERNEL_IMAGE_MAX_SIZE=14680064 -DWITH_FRAME_POINTERS=1 -DZX_ASSERT_LEVEL=2 -DMIN_CRASHLOG_SIZE=2048 -DPERSISTENT_RAM_ALLOCATION_GRANULARITY=128 -DKERNEL_MEMORY_PROFILER=false -DLIB_ARCH_PAGING_CONFIGURATION=\"sv39\" -I../../zircon/kernel/include -I../../zircon/kernel/lib/libc/include -I../../zircon/kernel/lib/ktl/include -I../../sdk/lib/fit/include -I../../zircon/kernel/lib/heap/include -I../../zircon/kernel/lib/lockup_detector/include -I../../zircon/system/ulib/lazy_init/include -I../../zircon/system/ulib/lockdep/include -I../../zircon/system/ulib/ffl/include -I../../zircon/kernel/lib/kconcurrent/include -I../../zircon/system/ulib/concurrent/include -I../../zircon/kernel/vm/include -I../../zircon/kernel/vm/phys/include -I../../zircon/kernel/lib/user_copy/include -I../../zircon/kernel/lib/acpi_lite/include -I../../zircon/kernel/lib/boot-options/include -I../../zircon/kernel/lib/devicetree/include -I../../zircon/system/ulib/uart/include -I../../zircon/system/ulib/hwreg/include -I../../zircon/kernel/phys/lib/memalloc/include -I../../zircon/system/ulib/zircon-internal/include -I../../zircon/kernel/lib/spsc_buffer/include -I../../src/performance/lib/fxt/include -I../../zircon/kernel/lib/special-sections/include -I../../zircon/system/ulib/fbl/include -I../../zircon/kernel/lib/fbl/include -I../../zircon/system/ulib/mmio-ptr/include -I../../zircon/kernel/lib/id_allocator/include -I../../zircon/system/ulib/zx/include -I../../sdk/lib/stdcompat/include -I../../zircon/kernel/lib/backtrace/include -I../../zircon/kernel/lib/version/include -I../../zircon/kernel/lib/wake-vector/include -I../../zircon/kernel/lib/sched/include -I../../zircon/kernel/lib/arch/riscv64/include -I../../zircon/kernel/lib/arch/include -I../../sdk/lib/zbi-format/include -I../../zircon/kernel/lib/power-management/include -I../../zircon/kernel/lib/stall/include -I../../zircon/system/public -I../../sdk/lib/zircon-assert -I../../zircon/kernel/arch/riscv64/include -I../../zircon/kernel/dev/iommu/stub/include -I../../zircon/kernel/dev/pdev/power/include -I../../zircon/kernel/dev/pdev/timer/include -Igen/zircon/kernel/lib/arch/gen-arm64-feature-asm.include -Igen/zircon/kernel/lib/arch/gen-arm64-system-asm.include -Igen/zircon/kernel/lib/arch/gen-riscv64-system-asm.include -Igen/zircon/kernel/lib/arch/gen-x86-msr-asm.include -Igen/zircon/kernel/lib/arch/gen-x86-cpuid-asm.include -I../../src/zircon/lib/zircon/include -Ifidling/gen/zircon/vdso/zx/zither/legacy_syscall_cdecl -I../../zircon/kernel/lib/code-patching/include -Igen/zircon/kernel/lib/code-patching/case-id-asm.include -I../../src/lib/zbitl/include -I../../zircon/third_party/ulib/cksum/include -I../../zircon/kernel/lib/efi/include -I../../zircon/kernel/arch/riscv64/code-patches/include -I../../zircon/kernel/lib/console/include -I../../zircon/kernel/lib/counters/include -I../../zircon/kernel/lib/crashlog/include -I../../zircon/kernel/lib/init/include -I../../zircon/kernel/lib/page/include -I../../zircon/kernel/lib/syscalls/include -Ifidling/gen/zircon/vdso/zx/zither/kernel -I../../zircon/kernel/lib/thread-stack/include -I../../zircon/kernel/phys/include -I../../zircon/kernel/arch/riscv64/phys/include -I../../zircon/kernel/lib/unittest/include -I../../zircon/kernel/object/include -I../../zircon/kernel/lib/crypto/include -I../../zircon/third_party/lib/jitterentropy/include -I../../zircon/kernel/lib/page_cache/include -I../../zircon/kernel/lib/instrumentation/include -I../../zircon/kernel/lib/object_cache/include -I../../zircon/kernel/dev/pcie/include -I../../zircon/kernel/dev/interrupt/include -I../../zircon/system/ulib/region-alloc/include -I../../zircon/kernel/lib/debuglog/include -I../../zircon/system/ulib/affine/include -I../../src/lib/elfldltl/include -I../../zircon/system/ulib/bitmap/include -I../../zircon/system/ulib/pretty/include -idirafter ../../zircon/kernel/lib/libc/include-after -idirafter ../../zircon/kernel/lib/ktl/include-after -Xclang -debug-info-kind=constructor -g3 -grecord-gcc-switches -gdwarf-4 -gz=zstd -fdata-sections -ffunction-sections -O2 -Wall -Wextra -Wconversion -Wextra-semi -Wimplicit-fallthrough -Wnewline-eof -Wstrict-prototypes -Wwrite-strings -Wno-sign-conversion -Wno-unused-parameter -Wnonportable-system-include-path -Wno-missing-field-initializers -Wno-extra-qualification -Wno-cast-function-type-mismatch -Wno-unknown-warning-option -Wno-missing-template-arg-list-after-template-kw -Wno-deprecated-pragma -Wno-nontrivial-memaccess -ftrivial-auto-var-init=pattern -ffile-compilation-dir=. -no-canonical-prefixes -fvisibility=hidden -Wthread-safety -Wno-unknown-warning-option -Wno-thread-safety-reference-return -Werror -Wa,--fatal-warnings -fno-common -fsized-deallocation --target=riscv64-fuchsia -msmall-data-limit=0 -march=rv64gcv_zihintpause_zba_zbb_zbs_zicbom_zicbop_zicboz_zfhmin_zkt -mabi=lp64d -fcolor-diagnostics -fcrash-diagnostics-dir=clang-crashreports -fcrash-diagnostics=all -gen-reproducer=error -fstack-size-section -ffuchsia-api-level=4293918720 -fpie -ffreestanding -include ../../zircon/kernel/include/hidden.h -fno-unwind-tables -Wformat=2 -Wmissing-declarations -Wvla -Wshadow -march=rv64imac_zihintpause_zba_zbb_zbs_zicbom_zicbop_zicboz_zfhmin_zkt_zicsr_zifencei -mabi=lp64 -ffixed-x27 -fno-sanitize=safe-stack -fsanitize=shadow-call-stack -fno-finite-loops -fdata-sections -fno-omit-frame-pointer -momit-leaf-frame-pointer -fno-omit-frame-pointer -momit-leaf-frame-pointer -idirafter ../../zircon/kernel/lib/ktl/include-after -idirafter ../../zircon/kernel/lib/libc/include-after -idirafter ../../zircon/kernel/lib/ktl/include-after -Wno-deprecated-this-capture -std=c++20 -fno-exceptions -fno-rtti -Xclang -fexperimental-omit-vtable-rtti -fvisibility-inlines-hidden -ftemplate-backtrace-limit=0 -faligned-new=8 -fno-exceptions -c ../../zircon/kernel/arch/riscv64/mp.cc
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '../../zircon/kernel/arch/riscv64/mp.cc'.
4.	Running pass 'Machine Instruction Scheduler' on function '@_Z26riscv64_software_exceptionv'
#0 0x00005592afee87f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (../../prebuilt/third_party/clang/custom/bin/clang+++0x97c77f8)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Fuchsia clang version 23.0.0git (https://llvm.googlesource.com/llvm-project 444adbe53472582f3f077fa1a16e52df97caddb8)
Target: riscv64-unknown-fuchsia
Thread model: posix
InstalledDir: ../../prebuilt/third_party/clang/custom/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: clang-crashreports/mp-6c8da4.cpp
clang++: note: diagnostic msg: clang-crashreports/mp-6c8da4.sh
clang++: note: diagnostic msg: 

********************
```

Note, that in Fuchsia's kernel `X27` is reserved. This code in question is some early startup code that uses a custom ABI, but still has  `-ffixed-x27` as part of its compiler options, which is visible in the IR attributes in the reduced IR.

A reproducer can be found here: https://storage.googleapis.com/fuchsia-artifacts/builds/8692593409294242177/build-debug/clang-crashreports/mp-6c8da4.tar

I haven't reduced the source yet, but got this from llvm-reduce:
```llvm
; RUN: llc <%s -o /dev/null

target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-fuchsia"

define i64 @_Z26riscv64_software_exceptionv(i1 %cmp) #0 {
entry:
  br i1 %cmp, label %do.end, label %if.else

if.else:                                          ; preds = %entry
  unreachable

do.end:                                           ; preds = %entry
  store i64 0, ptr null, align 8
  %0 = tail call i64 @llvm.read_register.i64(metadata !0)
  ret i64 %0
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(read)
declare i64 @llvm.read_register.i64(metadata) #1

attributes #0 = { "target-features"="+reserve-x27" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(read) }

!0 = !{!"s11"}


```

Output before crash:

```
# *** IR Dump Before Machine Instruction Scheduler (machine-scheduler) ***:
# Machine code for function _Z26riscv64_software_exceptionv: NoPHIs, TracksLiveness, TiedOpsRewritten
Function Live Ins: $x10 in %0

0B      bb.0.entry:
          successors: %bb.2(0x80000000), %bb.1(0x00000000); %bb.2(100.00%), %bb.1(0.00%)
          liveins: $x10
16B       %0:gpr = COPY $x10
32B       %1:gpr = ANDI %0:gpr, 1
48B       BNE %1:gpr, $x0, %bb.2
64B       PseudoBR %bb.1

80B     bb.1.if.else:
        ; predecessors: %bb.0


96B     bb.2.do.end:
        ; predecessors: %bb.0

128B      SD $x0, $x0, 0 :: (store (s64) into `ptr null`)
144B      %3:gpr = COPY $x27
160B      $x10 = COPY %3:gpr
176B      PseudoRET implicit $x10

# End machine code for function _Z26riscv64_software_exceptionv.

The register $x27 needs to be live in to %bb.2, but is missing from the live-in list.
LLVM ERROR: Invalid global physical register
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc reduced.ll -print-before-all
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'Machine Instruction Scheduler' on function '@_Z26riscv64_software_exceptionv'
#0 0x000055b3ec6fd308 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/local/google/home/paulkirth/llvm-fork/llvm/lib/Support/Unix/Signals.inc:842:13
[1]    2527824 IOT instruction (core dumped)  llc reduced.ll -print-before-all
```

Failing bot (note that the blame is irrelevant, since this was present on this bots very first run):  https://ci.chromium.org/ui/p/fuchsia/builders/global.ci/clang_toolchain.ci.minimal.riscv64-debug/b8693393982444601825/overview



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Paul Kirth (ilovepi)

<details>
We're seeing some compiler crashes in codegen for RISC-V in Fuchsia builds. This is a new build configuration in our CI, so its unclear when this may have been introduced. I have a suspicion that this is related to the recent changes to the default scheduling model, but I haven't started bisecting yet.

Error from CI:
```
FAILED: [code=1] kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o 
../../prebuilt/third_party/clang/custom/bin/clang++ -MD -MF kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o.d -o kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS=1 -DSMP_MAX_CPUS=16 -D_KERNEL -DLK -DENABLE_PANIC_SHELL -DZIRCON_TOOLCHAIN -DLK_DEBUGLEVEL=2 -DDEBUG_PRINT_LEVEL=2 -DVM_TRACING_LEVEL=0 -DFUTEX_BLOCK_TRACING_ENABLED=false -DLOCK_NAME_TRACING_ENABLED=false -DEXPERIMENTAL_THREAD_SAMPLER_ENABLED=false -DEXPERIMENTAL_KTRACE_STREAMING_ENABLED=true -DEXPERIMENTAL_FORCE_NEW_WAKEUP_ACCOUNTING=false -DCHANNEL_MESSAGE_BODY_TRACING_ENABLED=false -DJTRACE_TARGET_BUFFER_SIZE=0 -DSCHEDULER_TRACING_LEVEL=0 -DSCHEDULER_QUEUE_TRACING_ENABLED=false -DSCHEDULER_EXTRA_INVARIANT_VALIDATION=false -DSCHEDULER_LOCK_SPIN_TRACING_COMPRESSED=false -DSCHEDULER_LOCK_SPIN_TRACING_ENABLED=false -DWAIT_QUEUE_DEPTH_TRACING_ENABLED=false -DKERNEL_IMAGE_MAX_SIZE=14680064 -DWITH_FRAME_POINTERS=1 -DZX_ASSERT_LEVEL=2 -DMIN_CRASHLOG_SIZE=2048 -DPERSISTENT_RAM_ALLOCATION_GRANULARITY=128 -DKERNEL_MEMORY_PROFILER=false -DLIB_ARCH_PAGING_CONFIGURATION=\"sv39\" -I../../zircon/kernel/include -I../../zircon/kernel/lib/libc/include -I../../zircon/kernel/lib/ktl/include -I../../sdk/lib/fit/include -I../../zircon/kernel/lib/heap/include -I../../zircon/kernel/lib/lockup_detector/include -I../../zircon/system/ulib/lazy_init/include -I../../zircon/system/ulib/lockdep/include -I../../zircon/system/ulib/ffl/include -I../../zircon/kernel/lib/kconcurrent/include -I../../zircon/system/ulib/concurrent/include -I../../zircon/kernel/vm/include -I../../zircon/kernel/vm/phys/include -I../../zircon/kernel/lib/user_copy/include -I../../zircon/kernel/lib/acpi_lite/include -I../../zircon/kernel/lib/boot-options/include -I../../zircon/kernel/lib/devicetree/include -I../../zircon/system/ulib/uart/include -I../../zircon/system/ulib/hwreg/include -I../../zircon/kernel/phys/lib/memalloc/include -I../../zircon/system/ulib/zircon-internal/include -I../../zircon/kernel/lib/spsc_buffer/include -I../../src/performance/lib/fxt/include -I../../zircon/kernel/lib/special-sections/include -I../../zircon/system/ulib/fbl/include -I../../zircon/kernel/lib/fbl/include -I../../zircon/system/ulib/mmio-ptr/include -I../../zircon/kernel/lib/id_allocator/include -I../../zircon/system/ulib/zx/include -I../../sdk/lib/stdcompat/include -I../../zircon/kernel/lib/backtrace/include -I../../zircon/kernel/lib/version/include -I../../zircon/kernel/lib/wake-vector/include -I../../zircon/kernel/lib/sched/include -I../../zircon/kernel/lib/arch/riscv64/include -I../../zircon/kernel/lib/arch/include -I../../sdk/lib/zbi-format/include -I../../zircon/kernel/lib/power-management/include -I../../zircon/kernel/lib/stall/include -I../../zircon/system/public -I../../sdk/lib/zircon-assert -I../../zircon/kernel/arch/riscv64/include -I../../zircon/kernel/dev/iommu/stub/include -I../../zircon/kernel/dev/pdev/power/include -I../../zircon/kernel/dev/pdev/timer/include -Igen/zircon/kernel/lib/arch/gen-arm64-feature-asm.include -Igen/zircon/kernel/lib/arch/gen-arm64-system-asm.include -Igen/zircon/kernel/lib/arch/gen-riscv64-system-asm.include -Igen/zircon/kernel/lib/arch/gen-x86-msr-asm.include -Igen/zircon/kernel/lib/arch/gen-x86-cpuid-asm.include -I../../src/zircon/lib/zircon/include -Ifidling/gen/zircon/vdso/zx/zither/legacy_syscall_cdecl -I../../zircon/kernel/lib/code-patching/include -Igen/zircon/kernel/lib/code-patching/case-id-asm.include -I../../src/lib/zbitl/include -I../../zircon/third_party/ulib/cksum/include -I../../zircon/kernel/lib/efi/include -I../../zircon/kernel/arch/riscv64/code-patches/include -I../../zircon/kernel/lib/console/include -I../../zircon/kernel/lib/counters/include -I../../zircon/kernel/lib/crashlog/include -I../../zircon/kernel/lib/init/include -I../../zircon/kernel/lib/page/include -I../../zircon/kernel/lib/syscalls/include -Ifidling/gen/zircon/vdso/zx/zither/kernel -I../../zircon/kernel/lib/thread-stack/include -I../../zircon/kernel/phys/include -I../../zircon/kernel/arch/riscv64/phys/include -I../../zircon/kernel/lib/unittest/include -I../../zircon/kernel/object/include -I../../zircon/kernel/lib/crypto/include -I../../zircon/third_party/lib/jitterentropy/include -I../../zircon/kernel/lib/page_cache/include -I../../zircon/kernel/lib/instrumentation/include -I../../zircon/kernel/lib/object_cache/include -I../../zircon/kernel/dev/pcie/include -I../../zircon/kernel/dev/interrupt/include -I../../zircon/system/ulib/region-alloc/include -I../../zircon/kernel/lib/debuglog/include -I../../zircon/system/ulib/affine/include -I../../src/lib/elfldltl/include -I../../zircon/system/ulib/bitmap/include -I../../zircon/system/ulib/pretty/include -idirafter ../../zircon/kernel/lib/libc/include-after -idirafter ../../zircon/kernel/lib/ktl/include-after -Xclang -debug-info-kind=constructor -g3 -grecord-gcc-switches -gdwarf-4 -gz=zstd -fdata-sections -ffunction-sections -O2 -Wall -Wextra -Wconversion -Wextra-semi -Wimplicit-fallthrough -Wnewline-eof -Wstrict-prototypes -Wwrite-strings -Wno-sign-conversion -Wno-unused-parameter -Wnonportable-system-include-path -Wno-missing-field-initializers -Wno-extra-qualification -Wno-cast-function-type-mismatch -Wno-unknown-warning-option -Wno-missing-template-arg-list-after-template-kw -Wno-deprecated-pragma -Wno-nontrivial-memaccess -ftrivial-auto-var-init=pattern -ffile-compilation-dir=. -no-canonical-prefixes -fvisibility=hidden -Wthread-safety -Wno-unknown-warning-option -Wno-thread-safety-reference-return -Werror -Wa,--fatal-warnings -fno-common -fsized-deallocation --target=riscv64-fuchsia -msmall-data-limit=0 -march=rv64gcv_zihintpause_zba_zbb_zbs_zicbom_zicbop_zicboz_zfhmin_zkt -mabi=lp64d -fcolor-diagnostics -fcrash-diagnostics-dir=clang-crashreports -fcrash-diagnostics=all -gen-reproducer=error -fstack-size-section -ffuchsia-api-level=4293918720 -fpie -ffreestanding -include ../../zircon/kernel/include/hidden.h -fno-unwind-tables -Wformat=2 -Wmissing-declarations -Wvla -Wshadow -march=rv64imac_zihintpause_zba_zbb_zbs_zicbom_zicbop_zicboz_zfhmin_zkt_zicsr_zifencei -mabi=lp64 -ffixed-x27 -fno-sanitize=safe-stack -fsanitize=shadow-call-stack -fno-finite-loops -fdata-sections -fno-omit-frame-pointer -momit-leaf-frame-pointer -fno-omit-frame-pointer -momit-leaf-frame-pointer -idirafter ../../zircon/kernel/lib/ktl/include-after -idirafter ../../zircon/kernel/lib/libc/include-after -idirafter ../../zircon/kernel/lib/ktl/include-after -Wno-deprecated-this-capture -std=c++20 -fno-exceptions -fno-rtti -Xclang -fexperimental-omit-vtable-rtti -fvisibility-inlines-hidden -ftemplate-backtrace-limit=0 -faligned-new=8 -fno-exceptions -c ../../zircon/kernel/arch/riscv64/mp.cc
The register $x27 needs to be live in to %bb.2, but is missing from the live-in list.
fatal error: error in backend: Invalid global physical register
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: ../../prebuilt/third_party/clang/custom/bin/clang++ -MD -MF kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o.d -o kernel_riscv64/obj/zircon/kernel/arch/riscv64/riscv64.mp.cc.o -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D_LIBCPP_ENABLE_THREAD_SAFETY_ANNOTATIONS=1 -DSMP_MAX_CPUS=16 -D_KERNEL -DLK -DENABLE_PANIC_SHELL -DZIRCON_TOOLCHAIN -DLK_DEBUGLEVEL=2 -DDEBUG_PRINT_LEVEL=2 -DVM_TRACING_LEVEL=0 -DFUTEX_BLOCK_TRACING_ENABLED=false -DLOCK_NAME_TRACING_ENABLED=false -DEXPERIMENTAL_THREAD_SAMPLER_ENABLED=false -DEXPERIMENTAL_KTRACE_STREAMING_ENABLED=true -DEXPERIMENTAL_FORCE_NEW_WAKEUP_ACCOUNTING=false -DCHANNEL_MESSAGE_BODY_TRACING_ENABLED=false -DJTRACE_TARGET_BUFFER_SIZE=0 -DSCHEDULER_TRACING_LEVEL=0 -DSCHEDULER_QUEUE_TRACING_ENABLED=false -DSCHEDULER_EXTRA_INVARIANT_VALIDATION=false -DSCHEDULER_LOCK_SPIN_TRACING_COMPRESSED=false -DSCHEDULER_LOCK_SPIN_TRACING_ENABLED=false -DWAIT_QUEUE_DEPTH_TRACING_ENABLED=false -DKERNEL_IMAGE_MAX_SIZE=14680064 -DWITH_FRAME_POINTERS=1 -DZX_ASSERT_LEVEL=2 -DMIN_CRASHLOG_SIZE=2048 -DPERSISTENT_RAM_ALLOCATION_GRANULARITY=128 -DKERNEL_MEMORY_PROFILER=false -DLIB_ARCH_PAGING_CONFIGURATION=\"sv39\" -I../../zircon/kernel/include -I../../zircon/kernel/lib/libc/include -I../../zircon/kernel/lib/ktl/include -I../../sdk/lib/fit/include -I../../zircon/kernel/lib/heap/include -I../../zircon/kernel/lib/lockup_detector/include -I../../zircon/system/ulib/lazy_init/include -I../../zircon/system/ulib/lockdep/include -I../../zircon/system/ulib/ffl/include -I../../zircon/kernel/lib/kconcurrent/include -I../../zircon/system/ulib/concurrent/include -I../../zircon/kernel/vm/include -I../../zircon/kernel/vm/phys/include -I../../zircon/kernel/lib/user_copy/include -I../../zircon/kernel/lib/acpi_lite/include -I../../zircon/kernel/lib/boot-options/include -I../../zircon/kernel/lib/devicetree/include -I../../zircon/system/ulib/uart/include -I../../zircon/system/ulib/hwreg/include -I../../zircon/kernel/phys/lib/memalloc/include -I../../zircon/system/ulib/zircon-internal/include -I../../zircon/kernel/lib/spsc_buffer/include -I../../src/performance/lib/fxt/include -I../../zircon/kernel/lib/special-sections/include -I../../zircon/system/ulib/fbl/include -I../../zircon/kernel/lib/fbl/include -I../../zircon/system/ulib/mmio-ptr/include -I../../zircon/kernel/lib/id_allocator/include -I../../zircon/system/ulib/zx/include -I../../sdk/lib/stdcompat/include -I../../zircon/kernel/lib/backtrace/include -I../../zircon/kernel/lib/version/include -I../../zircon/kernel/lib/wake-vector/include -I../../zircon/kernel/lib/sched/include -I../../zircon/kernel/lib/arch/riscv64/include -I../../zircon/kernel/lib/arch/include -I../../sdk/lib/zbi-format/include -I../../zircon/kernel/lib/power-management/include -I../../zircon/kernel/lib/stall/include -I../../zircon/system/public -I../../sdk/lib/zircon-assert -I../../zircon/kernel/arch/riscv64/include -I../../zircon/kernel/dev/iommu/stub/include -I../../zircon/kernel/dev/pdev/power/include -I../../zircon/kernel/dev/pdev/timer/include -Igen/zircon/kernel/lib/arch/gen-arm64-feature-asm.include -Igen/zircon/kernel/lib/arch/gen-arm64-system-asm.include -Igen/zircon/kernel/lib/arch/gen-riscv64-system-asm.include -Igen/zircon/kernel/lib/arch/gen-x86-msr-asm.include -Igen/zircon/kernel/lib/arch/gen-x86-cpuid-asm.include -I../../src/zircon/lib/zircon/include -Ifidling/gen/zircon/vdso/zx/zither/legacy_syscall_cdecl -I../../zircon/kernel/lib/code-patching/include -Igen/zircon/kernel/lib/code-patching/case-id-asm.include -I../../src/lib/zbitl/include -I../../zircon/third_party/ulib/cksum/include -I../../zircon/kernel/lib/efi/include -I../../zircon/kernel/arch/riscv64/code-patches/include -I../../zircon/kernel/lib/console/include -I../../zircon/kernel/lib/counters/include -I../../zircon/kernel/lib/crashlog/include -I../../zircon/kernel/lib/init/include -I../../zircon/kernel/lib/page/include -I../../zircon/kernel/lib/syscalls/include -Ifidling/gen/zircon/vdso/zx/zither/kernel -I../../zircon/kernel/lib/thread-stack/include -I../../zircon/kernel/phys/include -I../../zircon/kernel/arch/riscv64/phys/include -I../../zircon/kernel/lib/unittest/include -I../../zircon/kernel/object/include -I../../zircon/kernel/lib/crypto/include -I../../zircon/third_party/lib/jitterentropy/include -I../../zircon/kernel/lib/page_cache/include -I../../zircon/kernel/lib/instrumentation/include -I../../zircon/kernel/lib/object_cache/include -I../../zircon/kernel/dev/pcie/include -I../../zircon/kernel/dev/interrupt/include -I../../zircon/system/ulib/region-alloc/include -I../../zircon/kernel/lib/debuglog/include -I../../zircon/system/ulib/affine/include -I../../src/lib/elfldltl/include -I../../zircon/system/ulib/bitmap/include -I../../zircon/system/ulib/pretty/include -idirafter ../../zircon/kernel/lib/libc/include-after -idirafter ../../zircon/kernel/lib/ktl/include-after -Xclang -debug-info-kind=constructor -g3 -grecord-gcc-switches -gdwarf-4 -gz=zstd -fdata-sections -ffunction-sections -O2 -Wall -Wextra -Wconversion -Wextra-semi -Wimplicit-fallthrough -Wnewline-eof -Wstrict-prototypes -Wwrite-strings -Wno-sign-conversion -Wno-unused-parameter -Wnonportable-system-include-path -Wno-missing-field-initializers -Wno-extra-qualification -Wno-cast-function-type-mismatch -Wno-unknown-warning-option -Wno-missing-template-arg-list-after-template-kw -Wno-deprecated-pragma -Wno-nontrivial-memaccess -ftrivial-auto-var-init=pattern -ffile-compilation-dir=. -no-canonical-prefixes -fvisibility=hidden -Wthread-safety -Wno-unknown-warning-option -Wno-thread-safety-reference-return -Werror -Wa,--fatal-warnings -fno-common -fsized-deallocation --target=riscv64-fuchsia -msmall-data-limit=0 -march=rv64gcv_zihintpause_zba_zbb_zbs_zicbom_zicbop_zicboz_zfhmin_zkt -mabi=lp64d -fcolor-diagnostics -fcrash-diagnostics-dir=clang-crashreports -fcrash-diagnostics=all -gen-reproducer=error -fstack-size-section -ffuchsia-api-level=4293918720 -fpie -ffreestanding -include ../../zircon/kernel/include/hidden.h -fno-unwind-tables -Wformat=2 -Wmissing-declarations -Wvla -Wshadow -march=rv64imac_zihintpause_zba_zbb_zbs_zicbom_zicbop_zicboz_zfhmin_zkt_zicsr_zifencei -mabi=lp64 -ffixed-x27 -fno-sanitize=safe-stack -fsanitize=shadow-call-stack -fno-finite-loops -fdata-sections -fno-omit-frame-pointer -momit-leaf-frame-pointer -fno-omit-frame-pointer -momit-leaf-frame-pointer -idirafter ../../zircon/kernel/lib/ktl/include-after -idirafter ../../zircon/kernel/lib/libc/include-after -idirafter ../../zircon/kernel/lib/ktl/include-after -Wno-deprecated-this-capture -std=c++20 -fno-exceptions -fno-rtti -Xclang -fexperimental-omit-vtable-rtti -fvisibility-inlines-hidden -ftemplate-backtrace-limit=0 -faligned-new=8 -fno-exceptions -c ../../zircon/kernel/arch/riscv64/mp.cc
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '../../zircon/kernel/arch/riscv64/mp.cc'.
4.	Running pass 'Machine Instruction Scheduler' on function '@<!-- -->_Z26riscv64_software_exceptionv'
#<!-- -->0 0x00005592afee87f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (../../prebuilt/third_party/clang/custom/bin/clang+++0x97c77f8)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Fuchsia clang version 23.0.0git (https://llvm.googlesource.com/llvm-project 444adbe53472582f3f077fa1a16e52df97caddb8)
Target: riscv64-unknown-fuchsia
Thread model: posix
InstalledDir: ../../prebuilt/third_party/clang/custom/bin
Build config: +assertions
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: clang-crashreports/mp-6c8da4.cpp
clang++: note: diagnostic msg: clang-crashreports/mp-6c8da4.sh
clang++: note: diagnostic msg: 

********************
```

Ntote, that on Fuchsia `X27/gp` is reserved for platform use. This code in question is some early startup code that uses a custom ABI, but still has  `-ffixed-x27` as part of its compiler options, which is visible in the IR attributes in the reduced IR.

A reproducer can be found here: https://storage.googleapis.com/fuchsia-artifacts/builds/8692593409294242177/build-debug/clang-crashreports/mp-6c8da4.tar

I haven't reduced the source yet, but got this from llvm-reduce:
```llvm
; RUN: llc &lt;%s -o /dev/null

target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-fuchsia"

define i64 @<!-- -->_Z26riscv64_software_exceptionv(i1 %cmp) #<!-- -->0 {
entry:
  br i1 %cmp, label %do.end, label %if.else

if.else:                                          ; preds = %entry
  unreachable

do.end:                                           ; preds = %entry
  store i64 0, ptr null, align 8
  %0 = tail call i64 @<!-- -->llvm.read_register.i64(metadata !0)
  ret i64 %0
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(read)
declare i64 @<!-- -->llvm.read_register.i64(metadata) #<!-- -->1

attributes #<!-- -->0 = { "target-features"="+reserve-x27" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind willreturn memory(read) }

!0 = !{!"s11"}


```

Output before crash:

```
# *** IR Dump Before Machine Instruction Scheduler (machine-scheduler) ***:
# Machine code for function _Z26riscv64_software_exceptionv: NoPHIs, TracksLiveness, TiedOpsRewritten
Function Live Ins: $x10 in %0

0B      bb.0.entry:
          successors: %bb.2(0x80000000), %bb.1(0x00000000); %bb.2(100.00%), %bb.1(0.00%)
          liveins: $x10
16B       %0:gpr = COPY $x10
32B       %1:gpr = ANDI %0:gpr, 1
48B       BNE %1:gpr, $x0, %bb.2
64B       PseudoBR %bb.1

80B     bb.1.if.else:
        ; predecessors: %bb.0


96B     bb.2.do.end:
        ; predecessors: %bb.0

128B      SD $x0, $x0, 0 :: (store (s64) into `ptr null`)
144B      %3:gpr = COPY $x27
160B      $x10 = COPY %3:gpr
176B      PseudoRET implicit $x10

# End machine code for function _Z26riscv64_software_exceptionv.

The register $x27 needs to be live in to %bb.2, but is missing from the live-in list.
LLVM ERROR: Invalid global physical register
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: llc reduced.ll -print-before-all
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'Machine Instruction Scheduler' on function '@<!-- -->_Z26riscv64_software_exceptionv'
#<!-- -->0 0x000055b3ec6fd308 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/local/google/home/paulkirth/llvm-fork/llvm/lib/Support/Unix/Signals.inc:842:13
[1]    2527824 IOT instruction (core dumped)  llc reduced.ll -print-before-all
```

Failing bot (note that the blame is irrelevant, since this was present on this bots very first run):  https://ci.chromium.org/ui/p/fuchsia/builders/global.ci/clang_toolchain.ci.minimal.riscv64-debug/b8693393982444601825/overview


</details>


---

### Comment 2 - ilovepi

@petrhosek actually started bisecting this a bit ago, and traced it to https://github.com/llvm/llvm-project/pull/115445. It isn't clear to me if this patch introduced this issue, or just uncovered an existing issue w/ codegen.

CC: @mshockwave @wangpc-pp 

---

### Comment 3 - ilovepi

It also seems like this reproduces w/ other registers that get marked fixed, so it isn't specific to a particular register.

---

### Comment 4 - ilovepi

Reduced source file test-case from clang:

--- mp-6c8da4.reduced.cpp ---
```cpp
enum { __mo_acq_rel, __mo_seq_cst };
enum memory_order { seq_cst = __mo_seq_cst };
struct atomic {
  void exchange(memory_order = seq_cst);
};
struct {
  atomic ipi_data;
} register *riscv64_percpu_ptr __asm__("s11");
int arch_curr_cpu_num();
void riscv64_software_exception() {
  if (arch_curr_cpu_num())
    riscv64_percpu_ptr->ipi_data.exchange();
}
```

Reducing the clang crash command...
```console
 clang -cc1 -triple riscv64-unknown-fuchsia -O2 -emit-obj -target-feature -d -target-feature -e -target-feature -h -target-feature -q -target-feature -sdext -target-feature -sdtrig -target-feature -sha -target-feature -shcounterenw -target-feature -shgatpa -target-feature -shlcofideleg -target-feature -shtvala -target-feature -shvsatpa -target-feature -shvstvala -target-feature -shvstvecd -target-feature -smaia -target-feature -smcdeleg -target-feature -smcntrpmf -target-feature -smcsrind -target-feature -smctr -target-feature -smdbltrp -target-feature -smepmp -target-feature -smmpm -target-feature -smnpm -target-feature -smrnmi -target-feature -smstateen -target-feature -ssaia -target-feature -ssccfg -target-feature -ssccptr -target-feature -sscofpmf -target-feature -sscounterenw -target-feature -sscsrind -target-feature -ssctr -target-feature -ssdbltrp -target-feature -ssnpm -target-feature -sspm -target-feature -ssqosid -target-feature -ssstateen -target-feature -ssstrict -target-feature -sstc -target-feature -sstvala -target-feature -sstvecd -target-feature -ssu64xl -target-feature -supm -target-feature -svade -target-feature -svadu -target-feature -svbare -target-feature -svinval -target-feature -svnapot -target-feature -svpbmt -target-feature -xwchc -target-feature -za128rs -target-feature -za64rs -target-feature -zabha -target-feature -zacas -target-feature -zama16b -target-feature -zawrs -target-feature -zbc -target-feature -zbkb -target-feature -zbkc -target-feature -zbkx -target-feature -zcb -target-feature -zcd -target-feature -zce -target-feature -zcf -target-feature -zclsd -target-feature -zcmop -target-feature -zcmp -target-feature -zcmt -target-feature -zdinx -target-feature -zfa -target-feature -zfbfmin -target-feature -zfh -target-feature -zfinx -target-feature -zhinx -target-feature -zhinxmin -target-feature -zic64b -target-feature -ziccamoa -target-feature -ziccamoc -target-feature -ziccif -target-feature -zicclsm -target-feature -ziccrse -target-feature -zicntr -target-feature -zicond -target-feature -zihintntl -target-feature -zihpm -target-feature -zilsd -target-feature -zimop -target-feature -zk -target-feature -zkn -target-feature -zknd -target-feature -zkne -target-feature -zknh -target-feature -zkr -target-feature -zks -target-feature -zksed -target-feature -zksh -target-feature -ztso -target-feature -zvbb -target-feature -zvbc -target-feature -zve32f -target-feature -zve32x -target-feature -zve64d -target-feature -zve64f -target-feature -zve64x -target-feature -zvfbfmin -target-feature -zvfbfwma -target-feature -zvfh -target-feature -zvfhmin -target-feature -zvkb -target-feature -zvkg -target-feature -zvkn -target-feature -zvknc -target-feature -zvkned -target-feature -zvkng -target-feature -zvknha -target-feature -zvknhb -target-feature -zvks -target-feature -zvksc -target-feature -zvksed -target-feature -zvksg -target-feature -zvksh -target-feature -zvkt -target-feature -zvl1024b -target-feature -zvl128b -target-feature -zvl16384b -target-feature -zvl2048b -target-feature -zvl256b -target-feature -zvl32768b -target-feature -zvl32b -target-feature -zvl4096b -target-feature -zvl512b -target-feature -zvl64b -target-feature -zvl65536b -target-feature -zvl8192b -target-feature -experimental-p -target-feature -experimental-smpmpmt -target-feature -experimental-svukte -target-feature -experimental-xrivosvisni -target-feature -experimental-xrivosvizip -target-feature -experimental-xsfmclic -target-feature -experimental-xsfsclic -target-feature -experimental-zalasr -target-feature -experimental-zibi -target-feature -experimental-zicfilp -target-feature -experimental-zicfiss -target-feature -experimental-zvbc32e -target-feature -experimental-zvfbfa -target-feature -experimental-zvfofp8min -target-feature -experimental-zvkgs -target-feature -experimental-zvqdotq -target-feature +reserve-x27 -fno-rtti mp-6c8da4.reduced.cpp
```


---

### Comment 5 - topperc

The stack trace shows that is coming from trying to schedule a COPY from x27. I think the liveness check should be supressed for reserved registers. That's what the MachineVerifier does.

CC: @arsenm 

---

### Comment 6 - ilovepi

Thanks @topperc, looking a bit more It seems that this block should have its check updated, https://github.com/llvm/llvm-project/blob/a304968d5e2e7a6895ce9cc53cc4d254199ecf9a/llvm/lib/CodeGen/LiveRangeCalc.cpp#L218-L230 to ignore if `isReserved() == true`. Or do you think its better to change `isLivein()` to do that checking?

I guess another option would be to always add reserved regs to the `LiveIns` vector, but that looks like it would be a bit invasive, and we'd need to rework several APIs to account for the special logic for little benefit.

---

### Comment 7 - arsenm

I'm not sure reserved registers have consistently ignored liveness. I think it's only "sometimes"

---

### Comment 8 - ilovepi

> I'm not sure reserved registers have consistently ignored liveness. I think it's only "sometimes"

That's .. surprising. In every place I've seen it used,  reserving the register generally means its got a use the toolchain can't see or reason about. How does that work? Do we have any docs on what constitutes "sometimes"?

For a concrete case in wide usage, the `gp` reg on RISC-V  is the platform reg and is either the Global pointer for small data, or used for platform reasons, like shadow call stack, but its always reserved and readable (AFAICT). 

---

### Comment 9 - arsenm

If you query the liveness on a regunit of a reserved register, it does compute and work. I believe we do make use of that in at least a few AMDGPU passes. For the AMDGPU use case, it's special management of a compiler manage register. We're not worried about some external platform intervening, but all generic LLVM code 

---

