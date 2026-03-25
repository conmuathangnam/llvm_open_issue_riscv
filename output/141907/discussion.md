# [RISCV] VSETVLI pass crashes due to "Cannot create empty or backwards segment" error

**Author:** BeMg
**URL:** https://github.com/llvm/llvm-project/issues/141907
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-06-06T15:34:28Z

## Body

The error message
```
llc: llvm-project/llvm/include/llvm/CodeGen/LiveInterval.h:172: llvm::LiveRange::Segment::Segment(SlotIndex, SlotIndex, VNInfo *): Assertion `S < E && "Cannot create empty or backwards segment"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llvm-project/build/bin/llc reduced.ll -print-after-all
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V Insert VSETVLI pass' on function '@silk_resampler_down2_3'
 #0 0x0000556207b594d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvm-project/build/bin/llc+0x21e24d8)
 #1 0x0000556207b5702e llvm::sys::RunSignalHandlers() (llvm-project/build/bin/llc+0x21e002e)
 #2 0x0000556207b59bf6 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007fce399d1cf0 __restore_rt (/lib64/libpthread.so.0+0x12cf0)
 #4 0x00007fce38451acf raise (/lib64/libc.so.6+0x4eacf)
 #5 0x00007fce38424ea5 abort (/lib64/libc.so.6+0x21ea5)
 #6 0x00007fce38424d79 _nl_load_domain.cold.0 (/lib64/libc.so.6+0x21d79)
 #7 0x00007fce3844a426 (/lib64/libc.so.6+0x47426)
 #8 0x00005562063189cf (llvm-project/build/bin/llc+0x9a19cf)
 #9 0x0000556206c7fb7b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (llvm-project/build/bin/llc+0x1308b7b)
#10 0x00005562071fd555 llvm::FPPassManager::runOnFunction(llvm::Function&) (llvm-project/build/bin/llc+0x1886555)
#11 0x0000556207205292 llvm::FPPassManager::runOnModule(llvm::Module&) (llvm-project/build/bin/llc+0x188e292)
#12 0x00005562071fdda1 llvm::legacy::PassManagerImpl::run(llvm::Module&) (llvm-project/build/bin/llc+0x1886da1)
#13 0x00005562061e4c85 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#14 0x00005562061e228d main (llvm-project/build/bin/llc+0x86b28d)
#15 0x00007fce3843dd85 __libc_start_main (/lib64/libc.so.6+0x3ad85)
#16 0x00005562061de4de _start (llvm-project/build/bin/llc+0x8674de)
Aborted
```


```
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define void @silk_resampler_down2_3(ptr %0) #0 {
entry:
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %entry
  %evl.based.iv = phi i64 [ 0, %entry ], [ %2, %vector.body ]
  %vector.recur = phi <vscale x 2 x i32> [ zeroinitializer, %entry ], [ %3, %vector.body ]
  %1 = call i32 @llvm.experimental.get.vector.length.i64(i64 %evl.based.iv, i32 1, i1 true)
  %2 = zext i32 %1 to i64
  %wide.masked.load = call <vscale x 6 x i32> @llvm.vp.load.nxv6i32.p0(ptr null, <vscale x 6 x i1> zeroinitializer, i32 0)
  %deinterleaved.results = call { <vscale x 2 x i32>, <vscale x 2 x i32>, <vscale x 2 x i32> } @llvm.vector.deinterleave3.nxv6i32(<vscale x 6 x i32> %wide.masked.load)
  %3 = extractvalue { <vscale x 2 x i32>, <vscale x 2 x i32>, <vscale x 2 x i32> } %deinterleaved.results, 1
  %vp.cast65 = call <vscale x 2 x i16> @llvm.vp.trunc.nxv2i16.nxv2i32(<vscale x 2 x i32> %vector.recur, <vscale x 2 x i1> zeroinitializer, i32 0)
  %interleaved.vec = call <vscale x 4 x i16> @llvm.vector.interleave2.nxv4i16(<vscale x 2 x i16> %vp.cast65, <vscale x 2 x i16> zeroinitializer)
  call void @llvm.vp.store.nxv4i16.p0(<vscale x 4 x i16> %interleaved.vec, ptr null, <vscale x 4 x i1> splat (i1 true), i32 0)
  %4 = icmp eq i32 %1, 0
  br i1 %4, label %while.body5, label %vector.body

while.body5:                                      ; preds = %while.body5, %vector.body
  %5 = bitcast <vscale x 2 x i32> %3 to <vscale x 4 x i16>
  %cond52 = extractelement <vscale x 4 x i16> %5, i64 0
  store i16 %cond52, ptr %0, align 2
  br label %while.body5
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @llvm.experimental.get.vector.length.i64(i64, i32 immarg, i1 immarg) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare <vscale x 6 x i32> @llvm.vp.load.nxv6i32.p0(ptr nocapture, <vscale x 6 x i1>, i32) #2

declare { <vscale x 2 x i32>, <vscale x 2 x i32>, <vscale x 2 x i32> } @llvm.vector.deinterleave3.nxv6i32(<vscale x 6 x i32>)

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <vscale x 2 x i16> @llvm.vp.trunc.nxv2i16.nxv2i32(<vscale x 2 x i32>, <vscale x 2 x i1>, i32) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <vscale x 4 x i16> @llvm.vector.interleave2.nxv4i16(<vscale x 2 x i16>, <vscale x 2 x i16>) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.vp.store.nxv4i16.p0(<vscale x 4 x i16>, ptr nocapture, <vscale x 4 x i1>, i32) #3

; uselistorder directives
uselistorder i64 0, { 0, 2, 1 }
uselistorder i1 true, { 1, 0 }

attributes #0 = { "target-cpu"="sifive-p670" "target-features"="+64bit,+a,+c,+d,+experimental,+f,+m,+relax,+unaligned-scalar-mem,+unaligned-vector-mem,+v,+xsifivecdiscarddlone,+xsifivecflushdlone,+za64rs,+zaamo,+zalrsc,+zba,+zbb,+zbs,+zca,+zcd,+zfhmin,+zic64b,+zicbom,+zicbop,+zicboz,+ziccamoa,+ziccif,+zicclsm,+ziccrse,+zicsr,+zifencei,+zihintntl,+zihintpause,+zihpm,+zmmul,+zvbb,+zvbc,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvkb,+zvkg,+zvkn,+zvknc,+zvkned,+zvkng,+zvknhb,+zvks,+zvksc,+zvksed,+zvksg,+zvksh,+zvkt,+zvl128b,+zvl32b,+zvl64b,-b,-e,-experimental-p,-experimental-smctr" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #2 = { nocallback nofree nosync nounwind willreturn memory(argmem: read) }
attributes #3 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }
```

I think is relate to 

https://github.com/llvm/llvm-project/blob/e2f86b5584959ec2b000d183841c8fb7c3402388/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp#L1708

The `beginIndex()` value could less than `MISlot`. 

~~This bug could be solved by replacing those statements with~~

```
LIS->removeInterval(DefReg);
LIS->createAndComputeVirtRegInterval(DefReg);
```

Update: remove and create will make other testcase failure.



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bug

Author: Piyou Chen (BeMg)

<details>
The error message
```
llc: llvm-project/llvm/include/llvm/CodeGen/LiveInterval.h:172: llvm::LiveRange::Segment::Segment(SlotIndex, SlotIndex, VNInfo *): Assertion `S &lt; E &amp;&amp; "Cannot create empty or backwards segment"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llvm-project/build/bin/llc reduced.ll -print-after-all
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V Insert VSETVLI pass' on function '@<!-- -->silk_resampler_down2_3'
 #<!-- -->0 0x0000556207b594d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvm-project/build/bin/llc+0x21e24d8)
 #<!-- -->1 0x0000556207b5702e llvm::sys::RunSignalHandlers() (llvm-project/build/bin/llc+0x21e002e)
 #<!-- -->2 0x0000556207b59bf6 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007fce399d1cf0 __restore_rt (/lib64/libpthread.so.0+0x12cf0)
 #<!-- -->4 0x00007fce38451acf raise (/lib64/libc.so.6+0x4eacf)
 #<!-- -->5 0x00007fce38424ea5 abort (/lib64/libc.so.6+0x21ea5)
 #<!-- -->6 0x00007fce38424d79 _nl_load_domain.cold.0 (/lib64/libc.so.6+0x21d79)
 #<!-- -->7 0x00007fce3844a426 (/lib64/libc.so.6+0x47426)
 #<!-- -->8 0x00005562063189cf (llvm-project/build/bin/llc+0x9a19cf)
 #<!-- -->9 0x0000556206c7fb7b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (llvm-project/build/bin/llc+0x1308b7b)
#<!-- -->10 0x00005562071fd555 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (llvm-project/build/bin/llc+0x1886555)
#<!-- -->11 0x0000556207205292 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (llvm-project/build/bin/llc+0x188e292)
#<!-- -->12 0x00005562071fdda1 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (llvm-project/build/bin/llc+0x1886da1)
#<!-- -->13 0x00005562061e4c85 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->14 0x00005562061e228d main (llvm-project/build/bin/llc+0x86b28d)
#<!-- -->15 0x00007fce3843dd85 __libc_start_main (/lib64/libc.so.6+0x3ad85)
#<!-- -->16 0x00005562061de4de _start (llvm-project/build/bin/llc+0x8674de)
Aborted
```


```
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define void @<!-- -->silk_resampler_down2_3(ptr %0) #<!-- -->0 {
entry:
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %entry
  %evl.based.iv = phi i64 [ 0, %entry ], [ %2, %vector.body ]
  %vector.recur = phi &lt;vscale x 2 x i32&gt; [ zeroinitializer, %entry ], [ %3, %vector.body ]
  %1 = call i32 @<!-- -->llvm.experimental.get.vector.length.i64(i64 %evl.based.iv, i32 1, i1 true)
  %2 = zext i32 %1 to i64
  %wide.masked.load = call &lt;vscale x 6 x i32&gt; @<!-- -->llvm.vp.load.nxv6i32.p0(ptr null, &lt;vscale x 6 x i1&gt; zeroinitializer, i32 0)
  %deinterleaved.results = call { &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt; } @<!-- -->llvm.vector.deinterleave3.nxv6i32(&lt;vscale x 6 x i32&gt; %wide.masked.load)
  %3 = extractvalue { &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt; } %deinterleaved.results, 1
  %vp.cast65 = call &lt;vscale x 2 x i16&gt; @<!-- -->llvm.vp.trunc.nxv2i16.nxv2i32(&lt;vscale x 2 x i32&gt; %vector.recur, &lt;vscale x 2 x i1&gt; zeroinitializer, i32 0)
  %interleaved.vec = call &lt;vscale x 4 x i16&gt; @<!-- -->llvm.vector.interleave2.nxv4i16(&lt;vscale x 2 x i16&gt; %vp.cast65, &lt;vscale x 2 x i16&gt; zeroinitializer)
  call void @<!-- -->llvm.vp.store.nxv4i16.p0(&lt;vscale x 4 x i16&gt; %interleaved.vec, ptr null, &lt;vscale x 4 x i1&gt; splat (i1 true), i32 0)
  %4 = icmp eq i32 %1, 0
  br i1 %4, label %while.body5, label %vector.body

while.body5:                                      ; preds = %while.body5, %vector.body
  %5 = bitcast &lt;vscale x 2 x i32&gt; %3 to &lt;vscale x 4 x i16&gt;
  %cond52 = extractelement &lt;vscale x 4 x i16&gt; %5, i64 0
  store i16 %cond52, ptr %0, align 2
  br label %while.body5
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @<!-- -->llvm.experimental.get.vector.length.i64(i64, i32 immarg, i1 immarg) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare &lt;vscale x 6 x i32&gt; @<!-- -->llvm.vp.load.nxv6i32.p0(ptr nocapture, &lt;vscale x 6 x i1&gt;, i32) #<!-- -->2

declare { &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt; } @<!-- -->llvm.vector.deinterleave3.nxv6i32(&lt;vscale x 6 x i32&gt;)

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare &lt;vscale x 2 x i16&gt; @<!-- -->llvm.vp.trunc.nxv2i16.nxv2i32(&lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i1&gt;, i32) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare &lt;vscale x 4 x i16&gt; @<!-- -->llvm.vector.interleave2.nxv4i16(&lt;vscale x 2 x i16&gt;, &lt;vscale x 2 x i16&gt;) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.vp.store.nxv4i16.p0(&lt;vscale x 4 x i16&gt;, ptr nocapture, &lt;vscale x 4 x i1&gt;, i32) #<!-- -->3

; uselistorder directives
uselistorder i64 0, { 0, 2, 1 }
uselistorder i1 true, { 1, 0 }

attributes #<!-- -->0 = { "target-cpu"="sifive-p670" "target-features"="+64bit,+a,+c,+d,+experimental,+f,+m,+relax,+unaligned-scalar-mem,+unaligned-vector-mem,+v,+xsifivecdiscarddlone,+xsifivecflushdlone,+za64rs,+zaamo,+zalrsc,+zba,+zbb,+zbs,+zca,+zcd,+zfhmin,+zic64b,+zicbom,+zicbop,+zicboz,+ziccamoa,+ziccif,+zicclsm,+ziccrse,+zicsr,+zifencei,+zihintntl,+zihintpause,+zihpm,+zmmul,+zvbb,+zvbc,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvkb,+zvkg,+zvkn,+zvknc,+zvkned,+zvkng,+zvknhb,+zvks,+zvksc,+zvksed,+zvksg,+zvksh,+zvkt,+zvl128b,+zvl32b,+zvl64b,-b,-e,-experimental-p,-experimental-smctr" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #<!-- -->2 = { nocallback nofree nosync nounwind willreturn memory(argmem: read) }
attributes #<!-- -->3 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }
```

I think is relate to 

https://github.com/llvm/llvm-project/blob/e2f86b5584959ec2b000d183841c8fb7c3402388/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp#L1708

The `beginIndex()` value could less than `MISlot`. 

This bug could be solved by replacing those statements with

```
LIS-&gt;removeInterval(DefReg);
LIS-&gt;createAndComputeVirtRegInterval(DefReg);
```


</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Piyou Chen (BeMg)

<details>
The error message
```
llc: llvm-project/llvm/include/llvm/CodeGen/LiveInterval.h:172: llvm::LiveRange::Segment::Segment(SlotIndex, SlotIndex, VNInfo *): Assertion `S &lt; E &amp;&amp; "Cannot create empty or backwards segment"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: llvm-project/build/bin/llc reduced.ll -print-after-all
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V Insert VSETVLI pass' on function '@<!-- -->silk_resampler_down2_3'
 #<!-- -->0 0x0000556207b594d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (llvm-project/build/bin/llc+0x21e24d8)
 #<!-- -->1 0x0000556207b5702e llvm::sys::RunSignalHandlers() (llvm-project/build/bin/llc+0x21e002e)
 #<!-- -->2 0x0000556207b59bf6 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007fce399d1cf0 __restore_rt (/lib64/libpthread.so.0+0x12cf0)
 #<!-- -->4 0x00007fce38451acf raise (/lib64/libc.so.6+0x4eacf)
 #<!-- -->5 0x00007fce38424ea5 abort (/lib64/libc.so.6+0x21ea5)
 #<!-- -->6 0x00007fce38424d79 _nl_load_domain.cold.0 (/lib64/libc.so.6+0x21d79)
 #<!-- -->7 0x00007fce3844a426 (/lib64/libc.so.6+0x47426)
 #<!-- -->8 0x00005562063189cf (llvm-project/build/bin/llc+0x9a19cf)
 #<!-- -->9 0x0000556206c7fb7b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (llvm-project/build/bin/llc+0x1308b7b)
#<!-- -->10 0x00005562071fd555 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (llvm-project/build/bin/llc+0x1886555)
#<!-- -->11 0x0000556207205292 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (llvm-project/build/bin/llc+0x188e292)
#<!-- -->12 0x00005562071fdda1 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (llvm-project/build/bin/llc+0x1886da1)
#<!-- -->13 0x00005562061e4c85 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->14 0x00005562061e228d main (llvm-project/build/bin/llc+0x86b28d)
#<!-- -->15 0x00007fce3843dd85 __libc_start_main (/lib64/libc.so.6+0x3ad85)
#<!-- -->16 0x00005562061de4de _start (llvm-project/build/bin/llc+0x8674de)
Aborted
```


```
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define void @<!-- -->silk_resampler_down2_3(ptr %0) #<!-- -->0 {
entry:
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %entry
  %evl.based.iv = phi i64 [ 0, %entry ], [ %2, %vector.body ]
  %vector.recur = phi &lt;vscale x 2 x i32&gt; [ zeroinitializer, %entry ], [ %3, %vector.body ]
  %1 = call i32 @<!-- -->llvm.experimental.get.vector.length.i64(i64 %evl.based.iv, i32 1, i1 true)
  %2 = zext i32 %1 to i64
  %wide.masked.load = call &lt;vscale x 6 x i32&gt; @<!-- -->llvm.vp.load.nxv6i32.p0(ptr null, &lt;vscale x 6 x i1&gt; zeroinitializer, i32 0)
  %deinterleaved.results = call { &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt; } @<!-- -->llvm.vector.deinterleave3.nxv6i32(&lt;vscale x 6 x i32&gt; %wide.masked.load)
  %3 = extractvalue { &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt; } %deinterleaved.results, 1
  %vp.cast65 = call &lt;vscale x 2 x i16&gt; @<!-- -->llvm.vp.trunc.nxv2i16.nxv2i32(&lt;vscale x 2 x i32&gt; %vector.recur, &lt;vscale x 2 x i1&gt; zeroinitializer, i32 0)
  %interleaved.vec = call &lt;vscale x 4 x i16&gt; @<!-- -->llvm.vector.interleave2.nxv4i16(&lt;vscale x 2 x i16&gt; %vp.cast65, &lt;vscale x 2 x i16&gt; zeroinitializer)
  call void @<!-- -->llvm.vp.store.nxv4i16.p0(&lt;vscale x 4 x i16&gt; %interleaved.vec, ptr null, &lt;vscale x 4 x i1&gt; splat (i1 true), i32 0)
  %4 = icmp eq i32 %1, 0
  br i1 %4, label %while.body5, label %vector.body

while.body5:                                      ; preds = %while.body5, %vector.body
  %5 = bitcast &lt;vscale x 2 x i32&gt; %3 to &lt;vscale x 4 x i16&gt;
  %cond52 = extractelement &lt;vscale x 4 x i16&gt; %5, i64 0
  store i16 %cond52, ptr %0, align 2
  br label %while.body5
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare i32 @<!-- -->llvm.experimental.get.vector.length.i64(i64, i32 immarg, i1 immarg) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: read)
declare &lt;vscale x 6 x i32&gt; @<!-- -->llvm.vp.load.nxv6i32.p0(ptr nocapture, &lt;vscale x 6 x i1&gt;, i32) #<!-- -->2

declare { &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i32&gt; } @<!-- -->llvm.vector.deinterleave3.nxv6i32(&lt;vscale x 6 x i32&gt;)

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare &lt;vscale x 2 x i16&gt; @<!-- -->llvm.vp.trunc.nxv2i16.nxv2i32(&lt;vscale x 2 x i32&gt;, &lt;vscale x 2 x i1&gt;, i32) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare &lt;vscale x 4 x i16&gt; @<!-- -->llvm.vector.interleave2.nxv4i16(&lt;vscale x 2 x i16&gt;, &lt;vscale x 2 x i16&gt;) #<!-- -->1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @<!-- -->llvm.vp.store.nxv4i16.p0(&lt;vscale x 4 x i16&gt;, ptr nocapture, &lt;vscale x 4 x i1&gt;, i32) #<!-- -->3

; uselistorder directives
uselistorder i64 0, { 0, 2, 1 }
uselistorder i1 true, { 1, 0 }

attributes #<!-- -->0 = { "target-cpu"="sifive-p670" "target-features"="+64bit,+a,+c,+d,+experimental,+f,+m,+relax,+unaligned-scalar-mem,+unaligned-vector-mem,+v,+xsifivecdiscarddlone,+xsifivecflushdlone,+za64rs,+zaamo,+zalrsc,+zba,+zbb,+zbs,+zca,+zcd,+zfhmin,+zic64b,+zicbom,+zicbop,+zicboz,+ziccamoa,+ziccif,+zicclsm,+ziccrse,+zicsr,+zifencei,+zihintntl,+zihintpause,+zihpm,+zmmul,+zvbb,+zvbc,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvkb,+zvkg,+zvkn,+zvknc,+zvkned,+zvkng,+zvknhb,+zvks,+zvksc,+zvksed,+zvksg,+zvksh,+zvkt,+zvl128b,+zvl32b,+zvl64b,-b,-e,-experimental-p,-experimental-smctr" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #<!-- -->2 = { nocallback nofree nosync nounwind willreturn memory(argmem: read) }
attributes #<!-- -->3 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }
```

I think is relate to 

https://github.com/llvm/llvm-project/blob/e2f86b5584959ec2b000d183841c8fb7c3402388/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp#L1708

The `beginIndex()` value could less than `MISlot`. 

This bug could be solved by replacing those statements with

```
LIS-&gt;removeInterval(DefReg);
LIS-&gt;createAndComputeVirtRegInterval(DefReg);
```


</details>


---

### Comment 3 - BeMg

cc @lukel97 

---

### Comment 4 - BeMg

Another possible fix.

```
diff --git a/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp b/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp
index 8fe8dfabee29..09d8514a0481 100644
--- a/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp
+++ b/llvm/lib/Target/RISCV/RISCVInsertVSETVLI.cpp
@@ -1704,8 +1704,13 @@ void RISCVInsertVSETVLI::coalesceVSETVLIs(MachineBasicBlock &MBB) const {
           if (DefReg.isVirtual() && LIS) {
             LiveInterval &DefLI = LIS->getInterval(DefReg);
             SlotIndex MISlot = LIS->getInstructionIndex(MI).getRegSlot();
-            VNInfo *DefVNI = DefLI.getVNInfoAt(DefLI.beginIndex());
-            LiveInterval::Segment S(MISlot, DefLI.beginIndex(), DefVNI);
+            SlotIndex EndSlot = LIS->getInstructionIndex(*NextMI).getBaseIndex();
+
+            SlotIndex SelectedSlot = std::max(DefLI.beginIndex(),EndSlot);
+
+            VNInfo *DefVNI = DefLI.getVNInfoAt(SelectedSlot);
+
+            LiveInterval::Segment S(MISlot, SelectedSlot, DefVNI);
             DefLI.addSegment(S);
             DefVNI->def = MISlot;
             // Mark DefLI as spillable if it was previously unspillable
```

---

### Comment 5 - lukel97

I think this due to both the AVL and defining VL being the same registers when coalescing:

```
    %23:gprnox0 = PseudoVSETIVLI 1, 192, implicit-def $vl, implicit-def $vtype
    renamable $v9 = COPY killed renamable $v8
    %23:gprnox0 = PseudoVSETVLI %23:gprnox0, 197, implicit-def $vl, implicit-def $vtype
```

We should probably move the AVL operand up and shrink its live range first, then handle DefReg.

The DefReg bit is also incorrectly assuming that DefLI only has one segment 😬 

I think I have a fix for this, will post a patch soon

---

