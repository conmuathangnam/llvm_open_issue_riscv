# [RISC-V] LLVM ERROR: Invalid size request on a scalable vector

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/125306
**Status:** Closed
**Labels:** backend:RISC-V, release:backport, crash
**Closed Date:** 2025-02-03T21:49:09Z

## Body

Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define i32 @main(ptr %0) #0 {
entry:
  store <16 x i16> zeroinitializer, ptr null, align 2
  store <8 x i32> zeroinitializer, ptr %0, align 4
  store <4 x i32> zeroinitializer, ptr getelementptr inbounds nuw (i8, ptr null, i64 64), align 4
  store i32 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 80), align 4
  %1 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 29916), align 4
  %broadcast.splatinsert53 = insertelement <4 x i32> zeroinitializer, i32 %1, i64 0
  %2 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28484), align 4
  %broadcast.splatinsert55 = insertelement <4 x i32> zeroinitializer, i32 %2, i64 0
  %3 = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %broadcast.splatinsert53, <4 x i32> %broadcast.splatinsert55)
  %4 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28368), align 4
  %broadcast.splatinsert57 = insertelement <4 x i32> zeroinitializer, i32 %4, i64 0
  %5 = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %3, <4 x i32> %broadcast.splatinsert57)
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 3704), align 2
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 4630), align 2
  %6 = shufflevector <4 x i32> %5, <4 x i32> zeroinitializer, <2 x i32> <i32 0, i32 4>
  store <2 x i32> %6, ptr getelementptr inbounds nuw (i8, ptr null, i64 16), align 4
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 5556), align 2
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 6482), align 2
  store <2 x i32> zeroinitializer, ptr getelementptr inbounds nuw (i8, ptr null, i64 24), align 4
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 7408), align 2
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 8334), align 2
  store <2 x i32> zeroinitializer, ptr getelementptr inbounds nuw (i8, ptr null, i64 32), align 4
  store i16 0, ptr %0, align 2
  store <2 x i32> zeroinitializer, ptr getelementptr inbounds nuw (i8, ptr null, i64 40), align 4
  %7 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 29916), align 4
  %broadcast.splatinsert165 = insertelement <4 x i32> poison, i32 %7, i64 0
  %8 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28484), align 4
  %broadcast.splatinsert167 = insertelement <4 x i32> poison, i32 %8, i64 0
  %9 = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %broadcast.splatinsert165, <4 x i32> %broadcast.splatinsert167)
  %10 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28368), align 4
  %broadcast.splatinsert169 = insertelement <4 x i32> poison, i32 %10, i64 0
  %11 = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %9, <4 x i32> %broadcast.splatinsert169)
  store i16 0, ptr null, align 2
  %12 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 29916), align 4
  %broadcast.splatinsert179 = insertelement <4 x i32> poison, i32 %12, i64 0
  %13 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28484), align 4
  %broadcast.splatinsert181 = insertelement <4 x i32> poison, i32 %13, i64 0
  %14 = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %broadcast.splatinsert179, <4 x i32> %broadcast.splatinsert181)
  %15 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28368), align 4
  %broadcast.splatinsert183 = insertelement <4 x i32> poison, i32 %15, i64 0
  %16 = call <4 x i32> @llvm.smin.v4i32(<4 x i32> %14, <4 x i32> %broadcast.splatinsert183)
  %17 = shufflevector <4 x i32> %11, <4 x i32> %16, <2 x i32> <i32 0, i32 4>
  store <2 x i32> %17, ptr null, align 4
  ret i32 0
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare <4 x i32> @llvm.smin.v4i32(<4 x i32>, <4 x i32>) #1

; uselistorder directives
uselistorder ptr null, { 0, 1, 11, 6, 17, 16, 10, 15, 14, 9, 13, 12, 5, 4, 3, 8, 7, 2 }
uselistorder ptr @llvm.smin.v4i32, { 5, 4, 3, 2, 1, 0 }

attributes #0 = { "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,+v,+zaamo,+zalrsc,+zicsr,+zifencei,+zmmul,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvl128b,+zvl32b,+zvl64b,-b,-e,-experimental-sdext,-experimental-sdtrig,-experimental-smctr,-experimental-ssctr,-experimental-svukte,-experimental-xqcia,-experimental-xqciac,-experimental-xqcicli,-experimental-xqcicm,-experimental-xqcics,-experimental-xqcicsr,-experimental-xqciint,-experimental-xqcilo,-experimental-xqcilsm,-experimental-xqcisls,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-h,-sha,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smdbltrp,-smepmp,-smmpm,-smnpm,-smrnmi,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssdbltrp,-ssnpm,-sspm,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-supm,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-svvptc,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xmipscmove,-xmipslsp,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zabha,-zacas,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl4096b,-zvl512b,-zvl65536b,-zvl8192b" }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

Command/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
LLVM ERROR: Invalid size request on a scalable vector.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@main'
 #0 0x000061aae52a1672 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x2092672)
 #1 0x000061aae529e6bf llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x208f6bf)
 #2 0x000061aae529e7fc SignalHandler(int) Signals.cpp:0:0
 #3 0x00007b9270045320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #4 0x00007b927009eb1c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x00007b927009eb1c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x00007b927009eb1c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x00007b927004526e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007b92700288ff abort ./stdlib/abort.c:81:7
 #9 0x000061aae3860302 llvm::Regex::match(llvm::StringRef, llvm::SmallVectorImpl<llvm::StringRef>*, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*) const (.cold) Regex.cpp:0:0
#10 0x000061aae51ec13e (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1fdd13e)
#11 0x000061aae5241738 (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x2032738)
#12 0x000061aae3ac63ac llvm::EVT::getVectorNumElements() const (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x8b73ac)
#13 0x000061aae3b3e130 lowerVECTOR_SHUFFLE(llvm::SDValue, llvm::SelectionDAG&, llvm::RISCVSubtarget const&) RISCVISelLowering.cpp:0:0
#14 0x000061aae4edfda9 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) LegalizeDAG.cpp:0:0
#15 0x000061aae4ee2a80 llvm::SelectionDAG::Legalize() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1cd3a80)
#16 0x000061aae50177de llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e087de)
#17 0x000061aae501a849 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e0b849)
#18 0x000061aae501bf5a llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e0cf5a)
#19 0x000061aae50068d7 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1df78d7)
#20 0x000061aae40bcfab llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#21 0x000061aae4729181 llvm::FPPassManager::runOnFunction(llvm::Function&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151a181)
#22 0x000061aae47295d1 llvm::FPPassManager::runOnModule(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151a5d1)
#23 0x000061aae4729f64 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151af64)
#24 0x000061aae394e942 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#25 0x000061aae3867827 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x658827)
#26 0x00007b927002a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#27 0x00007b927002a28b call_init ./csu/../csu/libc-start.c:128:20
#28 0x00007b927002a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#29 0x000061aae3943cc5 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x734cc5)
Aborted (core dumped)
```

Godbolt: https://godbolt.org/z/1v7P6eddE

Found via fuzzer (C program before reduction)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Edwin Lu (ewlu)

<details>
Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

define i32 @<!-- -->main(ptr %0) #<!-- -->0 {
entry:
  store &lt;16 x i16&gt; zeroinitializer, ptr null, align 2
  store &lt;8 x i32&gt; zeroinitializer, ptr %0, align 4
  store &lt;4 x i32&gt; zeroinitializer, ptr getelementptr inbounds nuw (i8, ptr null, i64 64), align 4
  store i32 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 80), align 4
  %1 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 29916), align 4
  %broadcast.splatinsert53 = insertelement &lt;4 x i32&gt; zeroinitializer, i32 %1, i64 0
  %2 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28484), align 4
  %broadcast.splatinsert55 = insertelement &lt;4 x i32&gt; zeroinitializer, i32 %2, i64 0
  %3 = call &lt;4 x i32&gt; @<!-- -->llvm.smin.v4i32(&lt;4 x i32&gt; %broadcast.splatinsert53, &lt;4 x i32&gt; %broadcast.splatinsert55)
  %4 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28368), align 4
  %broadcast.splatinsert57 = insertelement &lt;4 x i32&gt; zeroinitializer, i32 %4, i64 0
  %5 = call &lt;4 x i32&gt; @<!-- -->llvm.smin.v4i32(&lt;4 x i32&gt; %3, &lt;4 x i32&gt; %broadcast.splatinsert57)
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 3704), align 2
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 4630), align 2
  %6 = shufflevector &lt;4 x i32&gt; %5, &lt;4 x i32&gt; zeroinitializer, &lt;2 x i32&gt; &lt;i32 0, i32 4&gt;
  store &lt;2 x i32&gt; %6, ptr getelementptr inbounds nuw (i8, ptr null, i64 16), align 4
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 5556), align 2
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 6482), align 2
  store &lt;2 x i32&gt; zeroinitializer, ptr getelementptr inbounds nuw (i8, ptr null, i64 24), align 4
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 7408), align 2
  store i16 0, ptr getelementptr inbounds nuw (i8, ptr null, i64 8334), align 2
  store &lt;2 x i32&gt; zeroinitializer, ptr getelementptr inbounds nuw (i8, ptr null, i64 32), align 4
  store i16 0, ptr %0, align 2
  store &lt;2 x i32&gt; zeroinitializer, ptr getelementptr inbounds nuw (i8, ptr null, i64 40), align 4
  %7 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 29916), align 4
  %broadcast.splatinsert165 = insertelement &lt;4 x i32&gt; poison, i32 %7, i64 0
  %8 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28484), align 4
  %broadcast.splatinsert167 = insertelement &lt;4 x i32&gt; poison, i32 %8, i64 0
  %9 = call &lt;4 x i32&gt; @<!-- -->llvm.smin.v4i32(&lt;4 x i32&gt; %broadcast.splatinsert165, &lt;4 x i32&gt; %broadcast.splatinsert167)
  %10 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28368), align 4
  %broadcast.splatinsert169 = insertelement &lt;4 x i32&gt; poison, i32 %10, i64 0
  %11 = call &lt;4 x i32&gt; @<!-- -->llvm.smin.v4i32(&lt;4 x i32&gt; %9, &lt;4 x i32&gt; %broadcast.splatinsert169)
  store i16 0, ptr null, align 2
  %12 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 29916), align 4
  %broadcast.splatinsert179 = insertelement &lt;4 x i32&gt; poison, i32 %12, i64 0
  %13 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28484), align 4
  %broadcast.splatinsert181 = insertelement &lt;4 x i32&gt; poison, i32 %13, i64 0
  %14 = call &lt;4 x i32&gt; @<!-- -->llvm.smin.v4i32(&lt;4 x i32&gt; %broadcast.splatinsert179, &lt;4 x i32&gt; %broadcast.splatinsert181)
  %15 = load i32, ptr getelementptr inbounds nuw (i8, ptr null, i64 28368), align 4
  %broadcast.splatinsert183 = insertelement &lt;4 x i32&gt; poison, i32 %15, i64 0
  %16 = call &lt;4 x i32&gt; @<!-- -->llvm.smin.v4i32(&lt;4 x i32&gt; %14, &lt;4 x i32&gt; %broadcast.splatinsert183)
  %17 = shufflevector &lt;4 x i32&gt; %11, &lt;4 x i32&gt; %16, &lt;2 x i32&gt; &lt;i32 0, i32 4&gt;
  store &lt;2 x i32&gt; %17, ptr null, align 4
  ret i32 0
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare &lt;4 x i32&gt; @<!-- -->llvm.smin.v4i32(&lt;4 x i32&gt;, &lt;4 x i32&gt;) #<!-- -->1

; uselistorder directives
uselistorder ptr null, { 0, 1, 11, 6, 17, 16, 10, 15, 14, 9, 13, 12, 5, 4, 3, 8, 7, 2 }
uselistorder ptr @<!-- -->llvm.smin.v4i32, { 5, 4, 3, 2, 1, 0 }

attributes #<!-- -->0 = { "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,+v,+zaamo,+zalrsc,+zicsr,+zifencei,+zmmul,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvl128b,+zvl32b,+zvl64b,-b,-e,-experimental-sdext,-experimental-sdtrig,-experimental-smctr,-experimental-ssctr,-experimental-svukte,-experimental-xqcia,-experimental-xqciac,-experimental-xqcicli,-experimental-xqcicm,-experimental-xqcics,-experimental-xqcicsr,-experimental-xqciint,-experimental-xqcilo,-experimental-xqcilsm,-experimental-xqcisls,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-h,-sha,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smdbltrp,-smepmp,-smmpm,-smnpm,-smrnmi,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssdbltrp,-ssnpm,-sspm,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-supm,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-svvptc,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xmipscmove,-xmipslsp,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zabha,-zacas,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl4096b,-zvl512b,-zvl65536b,-zvl8192b" }
attributes #<!-- -->1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
```

Command/backtrace:
```
$ /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
LLVM ERROR: Invalid size request on a scalable vector.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->main'
 #<!-- -->0 0x000061aae52a1672 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x2092672)
 #<!-- -->1 0x000061aae529e6bf llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x208f6bf)
 #<!-- -->2 0x000061aae529e7fc SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x00007b9270045320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #<!-- -->4 0x00007b927009eb1c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x00007b927009eb1c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x00007b927009eb1c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x00007b927004526e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x00007b92700288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x000061aae3860302 llvm::Regex::match(llvm::StringRef, llvm::SmallVectorImpl&lt;llvm::StringRef&gt;*, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*) const (.cold) Regex.cpp:0:0
#<!-- -->10 0x000061aae51ec13e (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1fdd13e)
#<!-- -->11 0x000061aae5241738 (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x2032738)
#<!-- -->12 0x000061aae3ac63ac llvm::EVT::getVectorNumElements() const (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x8b73ac)
#<!-- -->13 0x000061aae3b3e130 lowerVECTOR_SHUFFLE(llvm::SDValue, llvm::SelectionDAG&amp;, llvm::RISCVSubtarget const&amp;) RISCVISelLowering.cpp:0:0
#<!-- -->14 0x000061aae4edfda9 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) LegalizeDAG.cpp:0:0
#<!-- -->15 0x000061aae4ee2a80 llvm::SelectionDAG::Legalize() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1cd3a80)
#<!-- -->16 0x000061aae50177de llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e087de)
#<!-- -->17 0x000061aae501a849 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e0b849)
#<!-- -->18 0x000061aae501bf5a llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e0cf5a)
#<!-- -->19 0x000061aae50068d7 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1df78d7)
#<!-- -->20 0x000061aae40bcfab llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->21 0x000061aae4729181 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151a181)
#<!-- -->22 0x000061aae47295d1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151a5d1)
#<!-- -->23 0x000061aae4729f64 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151af64)
#<!-- -->24 0x000061aae394e942 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->25 0x000061aae3867827 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x658827)
#<!-- -->26 0x00007b927002a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->27 0x00007b927002a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->28 0x00007b927002a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->29 0x000061aae3943cc5 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x734cc5)
Aborted (core dumped)
```

Godbolt: https://godbolt.org/z/1v7P6eddE

Found via fuzzer (C program before reduction)
</details>


---

### Comment 2 - topperc

Candidate patch

```
diff --git a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
index 07e3390f3fbb..1dddb67f9ad8 100644
--- a/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
+++ b/llvm/lib/Target/RISCV/RISCVISelLowering.cpp
@@ -4510,7 +4510,8 @@ static SDValue getSingleShuffleSrc(MVT VT, MVT ContainerVT, SDValue V1,
 
   // Src needs to have twice the number of elements.
   unsigned NumElts = VT.getVectorNumElements();
-  if (Src.getValueType().getVectorNumElements() != (NumElts * 2))
+  if (Src.getValueType().isScalableVector() ||
+      Src.getValueType().getVectorNumElements() != (NumElts * 2))
     return SDValue();
 
   // The extracts must extract the two halves of the source.
```

---

### Comment 3 - topperc

/cherry-pick 7c5100d36d8027dd205d6ec410a63c3930a1d9c1

---

### Comment 4 - llvmbot

/pull-request llvm/llvm-project#125590

---

