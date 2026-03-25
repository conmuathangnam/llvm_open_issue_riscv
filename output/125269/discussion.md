# [RISC-V]  Assertion `Idx2 != UINT_MAX && Values.contains(Idx2) && "Expected both indices to be extracted already."' failed

**Author:** ewlu
**URL:** https://github.com/llvm/llvm-project/issues/125269
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-02-05T12:13:34Z

## Body

Testcase:
```llvm ir
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-gnu"

; Function Attrs: vscale_range(4,4)
define i32 @main() #0 {
entry:
  %0 = shufflevector <32 x i64> zeroinitializer, <32 x i64> splat (i64 1), <32 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15, i32 16, i32 34, i32 35, i32 36, i32 37, i32 38, i32 39, i32 poison, i32 poison, i32 33, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison>
  %1 = icmp slt <32 x i64> zeroinitializer, %0
  %2 = bitcast <32 x i1> %1 to i32
  %3 = trunc i32 %2 to i8
  store i8 %3, ptr null, align 1
  ret i32 0
}

attributes #0 = { vscale_range(4,4) "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,+v,+zaamo,+zalrsc,+zicsr,+zifencei,+zmmul,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvl128b,+zvl256b,+zvl32b,+zvl64b,-b,-e,-experimental-sdext,-experimental-sdtrig,-experimental-smctr,-experimental-ssctr,-experimental-svukte,-experimental-xqcia,-experimental-xqciac,-experimental-xqcicli,-experimental-xqcicm,-experimental-xqcics,-experimental-xqcicsr,-experimental-xqciint,-experimental-xqcilo,-experimental-xqcilsm,-experimental-xqcisls,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-h,-sha,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smdbltrp,-smepmp,-smmpm,-smnpm,-smrnmi,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssdbltrp,-ssnpm,-sspm,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-supm,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-svvptc,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xmipscmove,-xmipslsp,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zabha,-zacas,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl16384b,-zvl2048b,-zvl32768b,-zvl4096b,-zvl512b,-zvl65536b,-zvl8192b" }
```
Command/backtrace
```
$  /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/Target/RISCV/RISCVISelLowering.cpp:5216: llvm::SDValue lowerShuffleViaVRegSplitting(llvm::ShuffleVectorSDNode*, llvm::SelectionDAG&, const llvm::RISCVSubtarget&): Assertion `Idx2 != UINT_MAX && Values.contains(Idx2) && "Expected both indices to be extracted already."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@main'
 #0 0x00005a29ac913672 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x2092672)
 #1 0x00005a29ac9106bf llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x208f6bf)
 #2 0x00005a29ac9107fc SignalHandler(int) Signals.cpp:0:0
 #3 0x0000729162a45320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #4 0x0000729162a9eb1c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000729162a9eb1c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x0000729162a9eb1c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x0000729162a4526e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000729162a288ff abort ./stdlib/abort.c:81:7
 #9 0x0000729162a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x0000729162a3b507 (/lib/x86_64-linux-gnu/libc.so.6+0x3b507)
#11 0x00005a29ab1b043d lowerVECTOR_SHUFFLE(llvm::SDValue, llvm::SelectionDAG&, llvm::RISCVSubtarget const&) RISCVISelLowering.cpp:0:0
#12 0x00005a29ac551da9 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) LegalizeDAG.cpp:0:0
#13 0x00005a29ac554a80 llvm::SelectionDAG::Legalize() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1cd3a80)
#14 0x00005a29ac6897de llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e087de)
#15 0x00005a29ac68c849 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e0b849)
#16 0x00005a29ac68df5a llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e0cf5a)
#17 0x00005a29ac6788d7 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1df78d7)
#18 0x00005a29ab72efab llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#19 0x00005a29abd9b181 llvm::FPPassManager::runOnFunction(llvm::Function&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151a181)
#20 0x00005a29abd9b5d1 llvm::FPPassManager::runOnModule(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151a5d1)
#21 0x00005a29abd9bf64 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151af64)
#22 0x00005a29aafc0942 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#23 0x00005a29aaed9827 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x658827)
#24 0x0000729162a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#25 0x0000729162a2a28b call_init ./csu/../csu/libc-start.c:128:20
#26 0x0000729162a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#27 0x00005a29aafb5cc5 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x734cc5)
Aborted (core dumped)
```
godbolt link: https://godbolt.org/z/bf4q3TTzW
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

; Function Attrs: vscale_range(4,4)
define i32 @<!-- -->main() #<!-- -->0 {
entry:
  %0 = shufflevector &lt;32 x i64&gt; zeroinitializer, &lt;32 x i64&gt; splat (i64 1), &lt;32 x i32&gt; &lt;i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15, i32 16, i32 34, i32 35, i32 36, i32 37, i32 38, i32 39, i32 poison, i32 poison, i32 33, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison, i32 poison&gt;
  %1 = icmp slt &lt;32 x i64&gt; zeroinitializer, %0
  %2 = bitcast &lt;32 x i1&gt; %1 to i32
  %3 = trunc i32 %2 to i8
  store i8 %3, ptr null, align 1
  ret i32 0
}

attributes #<!-- -->0 = { vscale_range(4,4) "target-features"="+64bit,+a,+c,+d,+f,+m,+relax,+v,+zaamo,+zalrsc,+zicsr,+zifencei,+zmmul,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvl128b,+zvl256b,+zvl32b,+zvl64b,-b,-e,-experimental-sdext,-experimental-sdtrig,-experimental-smctr,-experimental-ssctr,-experimental-svukte,-experimental-xqcia,-experimental-xqciac,-experimental-xqcicli,-experimental-xqcicm,-experimental-xqcics,-experimental-xqcicsr,-experimental-xqciint,-experimental-xqcilo,-experimental-xqcilsm,-experimental-xqcisls,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-h,-sha,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smdbltrp,-smepmp,-smmpm,-smnpm,-smrnmi,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssdbltrp,-ssnpm,-sspm,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-supm,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-svvptc,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xmipscmove,-xmipslsp,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zabha,-zacas,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl16384b,-zvl2048b,-zvl32768b,-zvl4096b,-zvl512b,-zvl65536b,-zvl8192b" }
```
Command/backtrace
```
$  /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
llc: /scratch/ewlu/daily-upstream-build/llvm/llvm/lib/Target/RISCV/RISCVISelLowering.cpp:5216: llvm::SDValue lowerShuffleViaVRegSplitting(llvm::ShuffleVectorSDNode*, llvm::SelectionDAG&amp;, const llvm::RISCVSubtarget&amp;): Assertion `Idx2 != UINT_MAX &amp;&amp; Values.contains(Idx2) &amp;&amp; "Expected both indices to be extracted already."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc reduced.ll
1.      Running pass 'Function Pass Manager' on module 'reduced.ll'.
2.      Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->main'
 #<!-- -->0 0x00005a29ac913672 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x2092672)
 #<!-- -->1 0x00005a29ac9106bf llvm::sys::RunSignalHandlers() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x208f6bf)
 #<!-- -->2 0x00005a29ac9107fc SignalHandler(int) Signals.cpp:0:0
 #<!-- -->3 0x0000729162a45320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #<!-- -->4 0x0000729162a9eb1c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000729162a9eb1c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->6 0x0000729162a9eb1c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->7 0x0000729162a4526e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000729162a288ff abort ./stdlib/abort.c:81:7
 #<!-- -->9 0x0000729162a2881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#<!-- -->10 0x0000729162a3b507 (/lib/x86_64-linux-gnu/libc.so.6+0x3b507)
#<!-- -->11 0x00005a29ab1b043d lowerVECTOR_SHUFFLE(llvm::SDValue, llvm::SelectionDAG&amp;, llvm::RISCVSubtarget const&amp;) RISCVISelLowering.cpp:0:0
#<!-- -->12 0x00005a29ac551da9 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) LegalizeDAG.cpp:0:0
#<!-- -->13 0x00005a29ac554a80 llvm::SelectionDAG::Legalize() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1cd3a80)
#<!-- -->14 0x00005a29ac6897de llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e087de)
#<!-- -->15 0x00005a29ac68c849 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e0b849)
#<!-- -->16 0x00005a29ac68df5a llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1e0cf5a)
#<!-- -->17 0x00005a29ac6788d7 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x1df78d7)
#<!-- -->18 0x00005a29ab72efab llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->19 0x00005a29abd9b181 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151a181)
#<!-- -->20 0x00005a29abd9b5d1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151a5d1)
#<!-- -->21 0x00005a29abd9bf64 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x151af64)
#<!-- -->22 0x00005a29aafc0942 compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->23 0x00005a29aaed9827 main (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x658827)
#<!-- -->24 0x0000729162a2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->25 0x0000729162a2a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->26 0x0000729162a2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->27 0x00005a29aafb5cc5 _start (/scratch/ewlu/daily-upstream-build/build-gcv/build-llvm-linux/bin/llc+0x734cc5)
Aborted (core dumped)
```
godbolt link: https://godbolt.org/z/bf4q3TTzW
Found via fuzzer (C program before reduction)


</details>


---

### Comment 2 - topperc

CC @preames @lukel97 

---

### Comment 3 - topperc

@alexey-bataev this might be yours it's in `lowerShuffleViaVRegSplitting` which uses `processShuffleMasks`

---

