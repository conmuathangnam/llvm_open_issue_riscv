# [RISCV] Cannot scavenge register without an emergency spill slot

**Author:** BeMg
**URL:** https://github.com/llvm/llvm-project/issues/165232
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-10-31T17:05:15Z

## Body

During the Prologue/Epilogue Insertion & Frame Finalization, the RISC-V backend will raise 

```
LLVM ERROR: Error while trying to spill X13 from class GPR: Cannot scavenge register without an emergency spill slot!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mattr=+v -O3 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@main'
 #0 0x0000000003c16c78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3c16c78)
 #1 0x0000000003c13e44 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fa35c842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007fa35c8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007fa35c842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007fa35c8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000007be6f0 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x0000000002e8bc43 llvm::RegScavenger::spill(llvm::Register, llvm::TargetRegisterClass const&, int, llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2e8bc43)
 #8 0x0000000002e8bf98 llvm::RegScavenger::scavengeRegisterBackwards(llvm::TargetRegisterClass const&, llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, bool, int, bool) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2e8bf98)
 #9 0x0000000002e8c488 scavengeVReg(llvm::MachineRegisterInfo&, llvm::RegScavenger&, llvm::Register, bool) RegisterScavenging.cpp:0:0
#10 0x0000000002e8c7fa scavengeFrameVirtualRegsInBlock(llvm::MachineRegisterInfo&, llvm::RegScavenger&, llvm::MachineBasicBlock&) RegisterScavenging.cpp:0:0
#11 0x0000000002e8c928 llvm::scavengeFrameVirtualRegs(llvm::MachineFunction&, llvm::RegScavenger&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2e8c928)
#12 0x0000000002e0355f (anonymous namespace)::PEIImpl::run(llvm::MachineFunction&) (.constprop.0) PrologEpilogInserter.cpp:0:0
#13 0x0000000002e04890 (anonymous namespace)::PEILegacy::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#14 0x0000000002c72e1d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c72e1d)
#15 0x00000000031ea752 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31ea752)
#16 0x00000000031ea9e1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31ea9e1)
#17 0x00000000031ec2c3 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31ec2c3)
#18 0x00000000007d5f5a main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7d5f5a)
#19 0x00007fa35c829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#20 0x00007fa35c829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#21 0x0000000000903a65 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x903a65)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

in the following LLVM IR.

https://godbolt.org/z/5e7son9dT

bisect to 17a98f85c2f3b1906b5b7368cfe1d6927b1f388f, may be the reason (choose larger LMUL load/store needs more GPR to compute the ShiftAmount?) 

```
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define i1 @main(ptr %var_117, ptr %arrayinit.element3045, ptr %arrayinit.element3047, ptr %arrayinit.element3049, ptr %arrayinit.element3051, ptr %arrayinit.element3053, ptr %arrayinit.element3055, ptr %arrayinit.element3057, ptr %arrayinit.element3059, ptr %arrayinit.element3061, ptr %arrayinit.element3063, ptr %arrayinit.element3065, ptr %arrayinit.element3067, i64 %var_94_i.07698, target("riscv.vector.tuple", <vscale x 16 x i8>, 2) %0, target("riscv.vector.tuple", <vscale x 16 x i8>, 4) %1) {
entry:
  store double 0.000000e+00, ptr %var_117, align 8
  store double 1.000000e+00, ptr %arrayinit.element3061, align 8
  br label %for.body

for.body:                                         ; preds = %for.body, %entry
  %2 = call <vscale x 2 x float> @llvm.riscv.vle.nxv2f32.p0.i64(<vscale x 2 x float> zeroinitializer, ptr %arrayinit.element3059, i64 0)
  %3 = call <vscale x 2 x float> @llvm.riscv.vle.nxv2f32.p0.i64(<vscale x 2 x float> zeroinitializer, ptr %arrayinit.element3067, i64 0)
  %4 = call <vscale x 2 x float> @llvm.riscv.vle.nxv2f32.p0.i64(<vscale x 2 x float> zeroinitializer, ptr %arrayinit.element3065, i64 0)
  %5 = call <vscale x 2 x float> @llvm.riscv.vle.nxv2f32.p0.i64(<vscale x 2 x float> zeroinitializer, ptr %arrayinit.element3063, i64 0)
  %6 = call <vscale x 2 x float> @llvm.riscv.vle.nxv2f32.p0.i64(<vscale x 2 x float> zeroinitializer, ptr %arrayinit.element3055, i64 0)
  %7 = call <vscale x 2 x float> @llvm.riscv.vle.nxv2f32.p0.i64(<vscale x 2 x float> zeroinitializer, ptr %arrayinit.element3057, i64 0)
  %8 = call <vscale x 2 x float> @llvm.riscv.vle.nxv2f32.p0.i64(<vscale x 2 x float> zeroinitializer, ptr %arrayinit.element3053, i64 0)
  %9 = call <vscale x 2 x double> @llvm.riscv.vle.nxv2f64.p0.i64(<vscale x 2 x double> zeroinitializer, ptr %arrayinit.element3051, i64 0)
  %10 = tail call <vscale x 2 x i32> @llvm.riscv.vle.nxv2i32.p0.i64(<vscale x 2 x i32> zeroinitializer, ptr %arrayinit.element3047, i64 0)
  %11 = tail call <vscale x 2 x i32> @llvm.riscv.vle.nxv2i32.p0.i64(<vscale x 2 x i32> zeroinitializer, ptr %arrayinit.element3049, i64 0)
  call void @llvm.riscv.vsseg4.triscv.vector.tuple_nxv8i8_4t.p0.i64(target("riscv.vector.tuple", <vscale x 8 x i8>, 4) zeroinitializer, ptr null, i64 0, i64 5)
  %12 = tail call target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) zeroinitializer, <vscale x 2 x float> %8, i32 0)
  %13 = tail call target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) %12, <vscale x 2 x float> %7, i32 2)
  %14 = tail call target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) %13, <vscale x 2 x float> %6, i32 0)
  %15 = tail call target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) %14, <vscale x 2 x float> %5, i32 0)
  %16 = tail call target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) %15, <vscale x 2 x float> %4, i32 0)
  %17 = tail call target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) %16, <vscale x 2 x float> %3, i32 0)
  %18 = tail call target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) %17, <vscale x 2 x float> %2, i32 0)
  call void @llvm.riscv.vsseg8.triscv.vector.tuple_nxv8i8_8t.p0.i64(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) %18, ptr %arrayinit.element3045, i64 0, i64 5)
  %19 = tail call <vscale x 2 x i1> @llvm.riscv.vmfgt.mask.nxv2f64.nxv2f64.i64(<vscale x 2 x i1> zeroinitializer, <vscale x 2 x double> zeroinitializer, <vscale x 2 x double> %9, <vscale x 2 x i1> zeroinitializer, i64 0)
  %20 = tail call <vscale x 2 x i32> @llvm.riscv.vssub.mask.nxv2i32.nxv2i32.i64(<vscale x 2 x i32> %11, <vscale x 2 x i32> zeroinitializer, <vscale x 2 x i32> %10, <vscale x 2 x i1> %19, i64 0, i64 0)
  call void @llvm.riscv.vsseg2.triscv.vector.tuple_nxv16i8_2t.p0.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 2) %0, ptr null, i64 %var_94_i.07698, i64 6)
  call void @llvm.riscv.vsseg4.mask.triscv.vector.tuple_nxv16i8_4t.p0.nxv2i1.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 4) zeroinitializer, ptr null, <vscale x 2 x i1> zeroinitializer, i64 0, i64 6)
  %21 = tail call target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2i32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) poison, <vscale x 2 x i32> %20, i32 0)
  call void @llvm.riscv.vsseg8.triscv.vector.tuple_nxv8i8_8t.p0.i64(target("riscv.vector.tuple", <vscale x 8 x i8>, 8) %21, ptr %var_117, i64 0, i64 5)
  call void @llvm.riscv.vsseg4.triscv.vector.tuple_nxv16i8_4t.p0.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 4) %1, ptr null, i64 0, i64 6)
  br label %for.body
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare <vscale x 2 x float> @llvm.riscv.vle.nxv2f32.p0.i64(<vscale x 2 x float>, ptr captures(none), i64) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare <vscale x 2 x double> @llvm.riscv.vle.nxv2f64.p0.i64(<vscale x 2 x double>, ptr captures(none), i64) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare <vscale x 2 x i32> @llvm.riscv.vle.nxv2i32.p0.i64(<vscale x 2 x i32>, ptr captures(none), i64) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.riscv.vsseg4.triscv.vector.tuple_nxv8i8_4t.p0.i64(target("riscv.vector.tuple", <vscale x 8 x i8>, 4), ptr captures(none), i64, i64 immarg) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8), <vscale x 2 x float>, i32 immarg) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.riscv.vsseg8.triscv.vector.tuple_nxv8i8_8t.p0.i64(target("riscv.vector.tuple", <vscale x 8 x i8>, 8), ptr captures(none), i64, i64 immarg) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <vscale x 2 x i1> @llvm.riscv.vmfgt.mask.nxv2f64.nxv2f64.i64(<vscale x 2 x i1>, <vscale x 2 x double>, <vscale x 2 x double>, <vscale x 2 x i1>, i64) #3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <vscale x 2 x i32> @llvm.riscv.vssub.mask.nxv2i32.nxv2i32.i64(<vscale x 2 x i32>, <vscale x 2 x i32>, <vscale x 2 x i32>, <vscale x 2 x i1>, i64, i64 immarg) #3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.riscv.vsseg2.triscv.vector.tuple_nxv16i8_2t.p0.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 2), ptr captures(none), i64, i64 immarg) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.riscv.vsseg4.mask.triscv.vector.tuple_nxv16i8_4t.p0.nxv2i1.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 4), ptr captures(none), <vscale x 2 x i1>, i64, i64 immarg) #1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare target("riscv.vector.tuple", <vscale x 8 x i8>, 8) @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2i32(target("riscv.vector.tuple", <vscale x 8 x i8>, 8), <vscale x 2 x i32>, i32 immarg) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.riscv.vsseg4.triscv.vector.tuple_nxv16i8_4t.p0.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 4), ptr captures(none), i64, i64 immarg) #1

; uselistorder directives
uselistorder ptr @llvm.riscv.vle.nxv2f32.p0.i64, { 6, 5, 4, 3, 2, 1, 0 }
uselistorder ptr @llvm.riscv.vle.nxv2i32.p0.i64, { 1, 0 }
uselistorder ptr @llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32, { 6, 5, 4, 3, 2, 1, 0 }
uselistorder ptr @llvm.riscv.vsseg8.triscv.vector.tuple_nxv8i8_8t.p0.i64, { 1, 0 }

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(argmem: read) }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }
attributes #2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(none) }
```

## Comments

### Comment 1 - BeMg

cc @wangpc-pp @lukel97 @topperc 

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Piyou Chen (BeMg)

<details>
During the Prologue/Epilogue Insertion &amp; Frame Finalization, the RISC-V backend will raise 

```
LLVM ERROR: Error while trying to spill X13 from class GPR: Cannot scavenge register without an emergency spill slot!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mattr=+v -O3 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'Prologue/Epilogue Insertion &amp; Frame Finalization' on function '@<!-- -->main'
 #<!-- -->0 0x0000000003c16c78 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/llc+0x3c16c78)
 #<!-- -->1 0x0000000003c13e44 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007fa35c842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007fa35c8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007fa35c842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007fa35c8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00000000007be6f0 llvm::json::operator==(llvm::json::Value const&amp;, llvm::json::Value const&amp;) (.cold) JSON.cpp:0:0
 #<!-- -->7 0x0000000002e8bc43 llvm::RegScavenger::spill(llvm::Register, llvm::TargetRegisterClass const&amp;, int, llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;, llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2e8bc43)
 #<!-- -->8 0x0000000002e8bf98 llvm::RegScavenger::scavengeRegisterBackwards(llvm::TargetRegisterClass const&amp;, llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;, bool, int, bool) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2e8bf98)
 #<!-- -->9 0x0000000002e8c488 scavengeVReg(llvm::MachineRegisterInfo&amp;, llvm::RegScavenger&amp;, llvm::Register, bool) RegisterScavenging.cpp:0:0
#<!-- -->10 0x0000000002e8c7fa scavengeFrameVirtualRegsInBlock(llvm::MachineRegisterInfo&amp;, llvm::RegScavenger&amp;, llvm::MachineBasicBlock&amp;) RegisterScavenging.cpp:0:0
#<!-- -->11 0x0000000002e8c928 llvm::scavengeFrameVirtualRegs(llvm::MachineFunction&amp;, llvm::RegScavenger&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2e8c928)
#<!-- -->12 0x0000000002e0355f (anonymous namespace)::PEIImpl::run(llvm::MachineFunction&amp;) (.constprop.0) PrologEpilogInserter.cpp:0:0
#<!-- -->13 0x0000000002e04890 (anonymous namespace)::PEILegacy::runOnMachineFunction(llvm::MachineFunction&amp;) PrologEpilogInserter.cpp:0:0
#<!-- -->14 0x0000000002c72e1d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x2c72e1d)
#<!-- -->15 0x00000000031ea752 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31ea752)
#<!-- -->16 0x00000000031ea9e1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31ea9e1)
#<!-- -->17 0x00000000031ec2c3 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/llc+0x31ec2c3)
#<!-- -->18 0x00000000007d5f5a main (/opt/compiler-explorer/clang-trunk/bin/llc+0x7d5f5a)
#<!-- -->19 0x00007fa35c829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->20 0x00007fa35c829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->21 0x0000000000903a65 _start (/opt/compiler-explorer/clang-trunk/bin/llc+0x903a65)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

in the following LLVM IR.

https://godbolt.org/z/5e7son9dT

bisect to 17a98f85c2f3b1906b5b7368cfe1d6927b1f388f, may be the reason (choose larger LMUL load/store needs more GPR to compute the ShiftAmount?) 

```
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define i1 @<!-- -->main(ptr %var_117, ptr %arrayinit.element3045, ptr %arrayinit.element3047, ptr %arrayinit.element3049, ptr %arrayinit.element3051, ptr %arrayinit.element3053, ptr %arrayinit.element3055, ptr %arrayinit.element3057, ptr %arrayinit.element3059, ptr %arrayinit.element3061, ptr %arrayinit.element3063, ptr %arrayinit.element3065, ptr %arrayinit.element3067, i64 %var_94_i.07698, target("riscv.vector.tuple", &lt;vscale x 16 x i8&gt;, 2) %0, target("riscv.vector.tuple", &lt;vscale x 16 x i8&gt;, 4) %1) {
entry:
  store double 0.000000e+00, ptr %var_117, align 8
  store double 1.000000e+00, ptr %arrayinit.element3061, align 8
  br label %for.body

for.body:                                         ; preds = %for.body, %entry
  %2 = call &lt;vscale x 2 x float&gt; @<!-- -->llvm.riscv.vle.nxv2f32.p0.i64(&lt;vscale x 2 x float&gt; zeroinitializer, ptr %arrayinit.element3059, i64 0)
  %3 = call &lt;vscale x 2 x float&gt; @<!-- -->llvm.riscv.vle.nxv2f32.p0.i64(&lt;vscale x 2 x float&gt; zeroinitializer, ptr %arrayinit.element3067, i64 0)
  %4 = call &lt;vscale x 2 x float&gt; @<!-- -->llvm.riscv.vle.nxv2f32.p0.i64(&lt;vscale x 2 x float&gt; zeroinitializer, ptr %arrayinit.element3065, i64 0)
  %5 = call &lt;vscale x 2 x float&gt; @<!-- -->llvm.riscv.vle.nxv2f32.p0.i64(&lt;vscale x 2 x float&gt; zeroinitializer, ptr %arrayinit.element3063, i64 0)
  %6 = call &lt;vscale x 2 x float&gt; @<!-- -->llvm.riscv.vle.nxv2f32.p0.i64(&lt;vscale x 2 x float&gt; zeroinitializer, ptr %arrayinit.element3055, i64 0)
  %7 = call &lt;vscale x 2 x float&gt; @<!-- -->llvm.riscv.vle.nxv2f32.p0.i64(&lt;vscale x 2 x float&gt; zeroinitializer, ptr %arrayinit.element3057, i64 0)
  %8 = call &lt;vscale x 2 x float&gt; @<!-- -->llvm.riscv.vle.nxv2f32.p0.i64(&lt;vscale x 2 x float&gt; zeroinitializer, ptr %arrayinit.element3053, i64 0)
  %9 = call &lt;vscale x 2 x double&gt; @<!-- -->llvm.riscv.vle.nxv2f64.p0.i64(&lt;vscale x 2 x double&gt; zeroinitializer, ptr %arrayinit.element3051, i64 0)
  %10 = tail call &lt;vscale x 2 x i32&gt; @<!-- -->llvm.riscv.vle.nxv2i32.p0.i64(&lt;vscale x 2 x i32&gt; zeroinitializer, ptr %arrayinit.element3047, i64 0)
  %11 = tail call &lt;vscale x 2 x i32&gt; @<!-- -->llvm.riscv.vle.nxv2i32.p0.i64(&lt;vscale x 2 x i32&gt; zeroinitializer, ptr %arrayinit.element3049, i64 0)
  call void @<!-- -->llvm.riscv.vsseg4.triscv.vector.tuple_nxv8i8_4t.p0.i64(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 4) zeroinitializer, ptr null, i64 0, i64 5)
  %12 = tail call target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) zeroinitializer, &lt;vscale x 2 x float&gt; %8, i32 0)
  %13 = tail call target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) %12, &lt;vscale x 2 x float&gt; %7, i32 2)
  %14 = tail call target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) %13, &lt;vscale x 2 x float&gt; %6, i32 0)
  %15 = tail call target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) %14, &lt;vscale x 2 x float&gt; %5, i32 0)
  %16 = tail call target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) %15, &lt;vscale x 2 x float&gt; %4, i32 0)
  %17 = tail call target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) %16, &lt;vscale x 2 x float&gt; %3, i32 0)
  %18 = tail call target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) %17, &lt;vscale x 2 x float&gt; %2, i32 0)
  call void @<!-- -->llvm.riscv.vsseg8.triscv.vector.tuple_nxv8i8_8t.p0.i64(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) %18, ptr %arrayinit.element3045, i64 0, i64 5)
  %19 = tail call &lt;vscale x 2 x i1&gt; @<!-- -->llvm.riscv.vmfgt.mask.nxv2f64.nxv2f64.i64(&lt;vscale x 2 x i1&gt; zeroinitializer, &lt;vscale x 2 x double&gt; zeroinitializer, &lt;vscale x 2 x double&gt; %9, &lt;vscale x 2 x i1&gt; zeroinitializer, i64 0)
  %20 = tail call &lt;vscale x 2 x i32&gt; @<!-- -->llvm.riscv.vssub.mask.nxv2i32.nxv2i32.i64(&lt;vscale x 2 x i32&gt; %11, &lt;vscale x 2 x i32&gt; zeroinitializer, &lt;vscale x 2 x i32&gt; %10, &lt;vscale x 2 x i1&gt; %19, i64 0, i64 0)
  call void @<!-- -->llvm.riscv.vsseg2.triscv.vector.tuple_nxv16i8_2t.p0.i64(target("riscv.vector.tuple", &lt;vscale x 16 x i8&gt;, 2) %0, ptr null, i64 %var_94_i.07698, i64 6)
  call void @<!-- -->llvm.riscv.vsseg4.mask.triscv.vector.tuple_nxv16i8_4t.p0.nxv2i1.i64(target("riscv.vector.tuple", &lt;vscale x 16 x i8&gt;, 4) zeroinitializer, ptr null, &lt;vscale x 2 x i1&gt; zeroinitializer, i64 0, i64 6)
  %21 = tail call target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2i32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) poison, &lt;vscale x 2 x i32&gt; %20, i32 0)
  call void @<!-- -->llvm.riscv.vsseg8.triscv.vector.tuple_nxv8i8_8t.p0.i64(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) %21, ptr %var_117, i64 0, i64 5)
  call void @<!-- -->llvm.riscv.vsseg4.triscv.vector.tuple_nxv16i8_4t.p0.i64(target("riscv.vector.tuple", &lt;vscale x 16 x i8&gt;, 4) %1, ptr null, i64 0, i64 6)
  br label %for.body
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare &lt;vscale x 2 x float&gt; @<!-- -->llvm.riscv.vle.nxv2f32.p0.i64(&lt;vscale x 2 x float&gt;, ptr captures(none), i64) #<!-- -->0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare &lt;vscale x 2 x double&gt; @<!-- -->llvm.riscv.vle.nxv2f64.p0.i64(&lt;vscale x 2 x double&gt;, ptr captures(none), i64) #<!-- -->0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare &lt;vscale x 2 x i32&gt; @<!-- -->llvm.riscv.vle.nxv2i32.p0.i64(&lt;vscale x 2 x i32&gt;, ptr captures(none), i64) #<!-- -->0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.riscv.vsseg4.triscv.vector.tuple_nxv8i8_4t.p0.i64(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 4), ptr captures(none), i64, i64 immarg) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8), &lt;vscale x 2 x float&gt;, i32 immarg) #<!-- -->2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.riscv.vsseg8.triscv.vector.tuple_nxv8i8_8t.p0.i64(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8), ptr captures(none), i64, i64 immarg) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare &lt;vscale x 2 x i1&gt; @<!-- -->llvm.riscv.vmfgt.mask.nxv2f64.nxv2f64.i64(&lt;vscale x 2 x i1&gt;, &lt;vscale x 2 x double&gt;, &lt;vscale x 2 x double&gt;, &lt;vscale x 2 x i1&gt;, i64) #<!-- -->3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare &lt;vscale x 2 x i32&gt; @<!-- -->llvm.riscv.vssub.mask.nxv2i32.nxv2i32.i64(&lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i1&gt;, i64, i64 immarg) #<!-- -->3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.riscv.vsseg2.triscv.vector.tuple_nxv16i8_2t.p0.i64(target("riscv.vector.tuple", &lt;vscale x 16 x i8&gt;, 2), ptr captures(none), i64, i64 immarg) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.riscv.vsseg4.mask.triscv.vector.tuple_nxv16i8_4t.p0.nxv2i1.i64(target("riscv.vector.tuple", &lt;vscale x 16 x i8&gt;, 4), ptr captures(none), &lt;vscale x 2 x i1&gt;, i64, i64 immarg) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8) @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2i32(target("riscv.vector.tuple", &lt;vscale x 8 x i8&gt;, 8), &lt;vscale x 2 x i32&gt;, i32 immarg) #<!-- -->2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.riscv.vsseg4.triscv.vector.tuple_nxv16i8_4t.p0.i64(target("riscv.vector.tuple", &lt;vscale x 16 x i8&gt;, 4), ptr captures(none), i64, i64 immarg) #<!-- -->1

; uselistorder directives
uselistorder ptr @<!-- -->llvm.riscv.vle.nxv2f32.p0.i64, { 6, 5, 4, 3, 2, 1, 0 }
uselistorder ptr @<!-- -->llvm.riscv.vle.nxv2i32.p0.i64, { 1, 0 }
uselistorder ptr @<!-- -->llvm.riscv.tuple.insert.triscv.vector.tuple_nxv8i8_8t.nxv2f32, { 6, 5, 4, 3, 2, 1, 0 }
uselistorder ptr @<!-- -->llvm.riscv.vsseg8.triscv.vector.tuple_nxv8i8_8t.p0.i64, { 1, 0 }

attributes #<!-- -->0 = { nocallback nofree nosync nounwind willreturn memory(argmem: read) }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }
attributes #<!-- -->2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #<!-- -->3 = { nocallback nofree nosync nounwind willreturn memory(none) }
```
</details>


---

### Comment 3 - topperc

Looks like the code in RISCVInstrInfo::mulImm for power of 2 +/- 1 requires an additional register we aren't accounting for in `getScavSlotsNumForRVV` in RISCVFrameLowering.cpp. We need to change `ScavSlotsNumRVVSpillScalableObject` to 3.

---

### Comment 4 - topperc

> Looks like the code in RISCVInstrInfo::mulImm for power of 2 +/- 1 requires an additional register we aren't accounting for in `getScavSlotsNumForRVV` in RISCVFrameLowering.cpp. We need to change `ScavSlotsNumRVVSpillScalableObject` to 3.

I assumed that's where it was coming from based on the SLLI+ADD but that isn't it. It's in the tuple register splitting code in storeRegToStackSlot

Here is the failing sequence.

```
  %6:gpr = ADDI $x2, 16                                                          
  VS2R_V $v22m2, %6:gpr, implicit $v22m2_v24m2_v26m2_v28m2 :: (store (<vscale x 1 x s128>) into %stack.3, align 8)
  %8:gpr = PseudoReadVLENB                                                       
  %9:gpr = SLLI %8:gpr, 1                                                        
  %7:gpr = ADD killed %6:gpr, killed %9:gpr                                      
  VS4R_V $v24m4, %7:gpr, implicit $v22m2_v24m2_v26m2_v28m2 :: (store (<vscale x 1 x s256>) into %stack.3, align 8)
  %10:gpr = SLLI killed %8:gpr, 2                                                
  %7:gpr = ADD killed %7:gpr, killed %10:gpr                                     
  VS2R_V $v28m2, killed %7:gpr, implicit $v22m2_v24m2_v26m2_v28m2 :: (store (<vscale x 1 x s128>) into %stack.3, align 8)
```

%9 is the register failing to allocate
%6 is the register used by frame lowering it is live until the ADD instruction.
%8 is used for twice because we need VLENB in two places.
Both %6 and %8 are live when we need to allocate %9 but we only have 2 emergency spill slots.

We could save a register by using SLLI %9:%gpr, 1 for the second SLLI. Then we wouldn't need %8 to be live.

---

