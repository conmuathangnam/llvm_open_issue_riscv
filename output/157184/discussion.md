# Assertion `OldMaskParam && "no mask param to fold the vl param into"' failed

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/157184

## Body

Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <vscale x 8 x i16> @llvm.vp.merge.nxv8i16(<vscale x 8 x i1>, <vscale x 8 x i16>, <vscale x 8 x i16>, i32) #0

define <vscale x 8 x i16> @main() {
entry:
  %0 = tail call <vscale x 8 x i16> @llvm.vp.merge.nxv8i16(<vscale x 8 x i1> zeroinitializer, <vscale x 8 x i16> zeroinitializer, <vscale x 8 x i16> zeroinitializer, i32 0)
  ret <vscale x 8 x i16> %0
}

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(none) }
```

Commands/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced-oldmaskparam.ll
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/CodeGen/ExpandVectorPredication.cpp:531: std::pair<llvm::Value*, bool> {anonymous}::CachingVPExpander::foldEVLIntoMask(llvm::VPIntrinsic&): Assertion `OldMaskParam && "no mask param to fold the vl param into"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced-oldmaskparam.ll
1.      Running pass 'Pre-ISel Intrinsic Lowering' on module 'reduced-oldmaskparam.ll'.
 #0 0x00005636ee644fc2 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1ec6fc2)
 #1 0x00005636ee641cbf llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1ec3cbf)
 #2 0x00005636ee641e0c SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007e186d445330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007e186d49eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007e186d49eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007e186d49eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007e186d44527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007e186d4288ff abort ./stdlib/abort.c:81:7
 #9 0x00007e186d42881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x00007e186d43b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x00005636ed8eea25 (anonymous namespace)::CachingVPExpander::foldEVLIntoMask(llvm::VPIntrinsic&) (.isra.0) ExpandVectorPredication.cpp:0:0
#12 0x00005636ed8f07ab llvm::expandVectorPredicationIntrinsic(llvm::VPIntrinsic&, llvm::TargetTransformInfo const&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x11727ab)
#13 0x00005636ed63fb72 (anonymous namespace)::PreISelIntrinsicLowering::lowerIntrinsics(llvm::Module&) const (.constprop.0) PreISelIntrinsicLowering.cpp:0:0
#14 0x00005636ed64092d (anonymous namespace)::PreISelIntrinsicLoweringLegacyPass::runOnModule(llvm::Module&) PreISelIntrinsicLowering.cpp:0:0
#15 0x00005636edb0a382 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x138c382)
#16 0x00005636eccb690b compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#17 0x00005636ecbc5887 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x447887)
#18 0x00007e186d42a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#19 0x00007e186d42a28b call_init ./csu/../csu/libc-start.c:128:20
#20 0x00007e186d42a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#21 0x00005636eccab895 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x52d895)
Aborted (core dumped)
```

Godbolt: https://godbolt.org/z/qdvxaah89

Found via fuzzer?

Not sure if this is related to https://github.com/llvm/llvm-project/issues/157177 or if this is an interesting testcase to begin with. The fuzzer originally found that testcase, however, in the middle of reducing it with llvm-reduce, I ran into this one. Snippet from llvm-reduce:
```
*** Reducing Attributes (attributes)...
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/include/llvm/CodeGen/ValueTypes.h:312: llvm::MVT llvm::EVT::getSimpleVT() const: Assertion `isSimple() && "Expected a SimpleValueType!"' failed.
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/CodeGen/ExpandVectorPredication.cpp:531: std::pair<llvm::Value*, bool> {anonymous}::CachingVPExpander::foldEVLIntoMask(llvm::VPIntrinsic&): Assertion `OldMaskParam && "no mask param to fold the vl param into"' failed.
 **** SUCCESS | Saved new best reduction to reduced.ll
```
with interestingness test:
```
#!/bin/bash

/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc $1 2>&1 | grep "Assertion"
```

