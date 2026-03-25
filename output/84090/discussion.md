# failed to perform tail call elimination on a call site marked musttail

**Author:** hiraditya
**URL:** https://github.com/llvm/llvm-project/issues/84090

## Body

```
Stack dump:
0.	Running pass 'Function Pass Manager' on module 'disk/android_trees/main/out/soong/.intermediates/external/skia/libskia/android_riscv64_static_afdo-libhwui/c7b2e1d59c67dbe9379df594344f61da/libskia.a(skcms_TransformBaseline.o at 2848574)'.
1.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@_ZN13skcms_private8baselineL12Exec_load_a8ENS0_9StageListEPPKvPKcPcDv4_fS8_S8_S8_i.__uniq.225338500882782122409073156489154930671'
 #0 0x00005573cecfc352 (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2c15352)
 #1 0x00005573cecfb3f0 (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2c143f0)
 #2 0x00005573cecfc75a (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2c1575a)
 #3 0x00007f0338257510 (/lib/x86_64-linux-gnu/libc.so.6+0x3c510)
 #4 0x00007f03382a50fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007f0338257472 raise ./signal/../sysdeps/posix/raise.c:27:6
 #6 0x00007f03382414b2 abort ./stdlib/abort.c:81:7
 #7 0x00005573ce169c6d (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2082c6d)
 #8 0x00005573cdaf7995 (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1a10995)
 #9 0x00005573cf049f19 (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2f62f19)
#10 0x00005573cde97e1d (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1db0e1d)
#11 0x00005573cde7076d (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1d8976d)
#12 0x00005573cde6fe59 (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1d88e59)
#13 0x00005573cde57b23 (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1d70b23)
#14 0x00005573cde542d4 (/usr/local/foobar/g/master-plus-llvm/prebuilts/clang/host/li
```

Because of this skia had to disable `musttail` for RISC-V https://skia.googlesource.com/skcms/+/529510fe003a3ccbf2d0d76939bf49f88b825177%5E%21/src/skcms_internals.h

Unofortunately I dont have a small repro testcase.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: AdityaK (hiraditya)

<details>

```
Stack dump:
0.	Running pass 'Function Pass Manager' on module 'disk/android_trees/main/out/soong/.intermediates/external/skia/libskia/android_riscv64_static_afdo-libhwui/c7b2e1d59c67dbe9379df594344f61da/libskia.a(skcms_TransformBaseline.o at 2848574)'.
1.	Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_ZN13skcms_private8baselineL12Exec_load_a8ENS0_9StageListEPPKvPKcPcDv4_fS8_S8_S8_i.__uniq.225338500882782122409073156489154930671'
 #<!-- -->0 0x00005573cecfc352 (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2c15352)
 #<!-- -->1 0x00005573cecfb3f0 (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2c143f0)
 #<!-- -->2 0x00005573cecfc75a (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2c1575a)
 #<!-- -->3 0x00007f0338257510 (/lib/x86_64-linux-gnu/libc.so.6+0x3c510)
 #<!-- -->4 0x00007f03382a50fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007f0338257472 raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->6 0x00007f03382414b2 abort ./stdlib/abort.c:81:7
 #<!-- -->7 0x00005573ce169c6d (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2082c6d)
 #<!-- -->8 0x00005573cdaf7995 (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1a10995)
 #<!-- -->9 0x00005573cf049f19 (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x2f62f19)
#<!-- -->10 0x00005573cde97e1d (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1db0e1d)
#<!-- -->11 0x00005573cde7076d (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1d8976d)
#<!-- -->12 0x00005573cde6fe59 (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1d88e59)
#<!-- -->13 0x00005573cde57b23 (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/linux-x86/clang-r498229b/bin/ld.lld+0x1d70b23)
#<!-- -->14 0x00005573cde542d4 (/usr/local/google/home/appujee/g/master-plus-llvm/prebuilts/clang/host/li
```

Because of this skia had to disable `musttail` for RISC-V https://skia.googlesource.com/skcms/+/529510fe003a3ccbf2d0d76939bf49f88b825177%5E%21/src/skcms_internals.h

Unofortunately I dont have a small repro testcase.
</details>


---

### Comment 2 - jrtc27

RISCV's tail call eligibility check is a bit simplistic and bails if it sees arguments passed on the stack. To fully support musttail (and optimise more cases) it needs to also handle when the arguments passed on the stack fit in our frame.

---

### Comment 3 - jrtc27

https://godbolt.org/z/xcvGboreY

---

### Comment 4 - hiraditya

> https://godbolt.org/z/xcvGboreY

Thanks for sharing the testcase. copy-pasting here for posterity

```c
void foo(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);
void bar(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) {
    [[clang::musttail]] return foo(a0, a1, a2, a3, a4, a5, a6, a7, a8);
}
```

$ rv64gc-clang -O2
```
0.	Program arguments: /opt/compiler-explorer/clang-17.0.1/bin/clang -g -o /app/output.s -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics -O2 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@bar'
 #0 0x0000000003349f18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-17.0.1/bin/clang+0x3349f18)
 #1 0x0000000003348044 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-17.0.1/bin/clang+0x3348044)
 #2 0x000000000329c553 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-17.0.1/bin/clang+0x329c553)
 #3 0x000000000334007e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang+0x334007e)
 #4 0x0000000000bd3b76 (/opt/compiler-explorer/clang-17.0.1/bin/clang+0xbd3b76)
 #5 0x00000000032a5583 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-17.0.1/bin/clang+0x32a5583)
 #6 0x00000000032a56e8 (/opt/compiler-explorer/clang-17.0.1/bin/clang+0x32a56e8)
 #7 0x0000000001d03bda llvm::RISCVTargetLowering::LowerCall(llvm::TargetLowering::CallLoweringInfo&, llvm::SmallVectorImpl<llvm::SDValue>&) const (/opt/compiler-explorer/clang-17.0.1/bin/clang+0x1d03bda)
```

---

