# Segfault when lower COPY intrinsic RISCV

**Author:** bababuck
**URL:** https://github.com/llvm/llvm-project/issues/129929
**Status:** Closed
**Labels:** backend:RISC-V, crash
**Closed Date:** 2025-03-06T01:09:28Z

## Body

Built info:
Hash: `edc02351dd`

Command:
```
$LLVM_DIR/bin/llc o3.ll -march=riscv64 -mattr=+v -O3  -o output.s
```

Trace:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /work/rbuchner/llvm/build//bin/llc o3.ll -march=riscv64 -mattr=+v -O3 -o output.s
1.      Running pass 'Function Pass Manager' on module 'o3.ll'.
2.      Running pass 'Post-RA pseudo instruction expansion pass' on function '@strlen'
 #0 0x0000000001ce5388 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/work/rbuchner/llvm/build//bin/llc+0x1ce5388)
 #1 0x0000000001ce2bac SignalHandler(int) Signals.cpp:0:0
 #2 0x00007fde7f664d10 __restore_rt (/lib64/libpthread.so.0+0x12d10)
 #3 0x0000000000861bc7 llvm::RISCVInstrInfo::copyPhysRegVector(llvm::MachineBasicBlock&, llvm::MachineInstrBundleIterator<llvm::MachineInstr, f
alse>, llvm::DebugLoc const&, llvm::MCRegister, llvm::MCRegister, bool, llvm::TargetRegisterClass const*) const (/work/rbuchner/llvm/build//bin
/llc+0x861bc7)
 #4 0x0000000000862e57 llvm::RISCVInstrInfo::copyPhysReg(llvm::MachineBasicBlock&, llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>,
 llvm::DebugLoc const&, llvm::MCRegister, llvm::MCRegister, bool, bool, bool) const (/work/rbuchner/llvm/build//bin/llc+0x862e57)
 #5 0x00000000010afbf4 llvm::TargetInstrInfo::lowerCopy(llvm::MachineInstr*, llvm::TargetRegisterInfo const*) const (/work/rbuchner/llvm/build/
/bin/llc+0x10afbf4)
 #6 0x0000000000d21618 (anonymous namespace)::ExpandPostRA::runOnMachineFunction(llvm::MachineFunction&) ExpandPostRAPseudos.cpp:0:0
 #7 0x0000000000e4ea73 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.69) MachineFunctionPass.cpp:0:0
 #8 0x000000000136d581 llvm::FPPassManager::runOnFunction(llvm::Function&) (/work/rbuchner/llvm/build//bin/llc+0x136d581)
 #9 0x000000000136d701 llvm::FPPassManager::runOnModule(llvm::Module&) (/work/rbuchner/llvm/build//bin/llc+0x136d701)
#10 0x000000000136e144 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/work/rbuchner/llvm/build//bin/llc+0x136e144)
#11 0x000000000082ceae compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#12 0x0000000000774c06 main (/work/rbuchner/llvm/build//bin/llc+0x774c06)
#13 0x00007fde7e0bf7e5 __libc_start_main (/lib64/libc.so.6+0x3a7e5)
#14 0x0000000000824ade _start (/work/rbuchner/llvm/build//bin/llc+0x824ade)
```

Input file:
```
; ModuleID = '../o3.ll'
source_filename = "loop.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: read) vscale_range(2,1024)
define dso_local noundef signext i64 @strlen(ptr noundef %0) local_unnamed_addr #0 {
  br label %2

2:                                                ; preds = %1
  %3 = phi ptr [ %0, %1 ]
  %4 = call { <4 x i64>, i64 } @llvm.riscv.vleff.v4i64.i64(<4 x i64> undef, ptr %3, i64 4)
  %5 = extractvalue { <4 x i64>, i64 } %4, 0
  %6 = extractvalue { <4 x i64>, i64 } %4, 1
  %7 = icmp eq <4 x i64> %5, splat (i64 1)
  store <4 x i1> %7, ptr %3, align 1, !tbaa !8
  ret i64 %6
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare { <4 x i64>, i64 } @llvm.riscv.vleff.v4i64.i64(<4 x i64>, ptr nocapture, i64) #3

attributes #0 = { nofree norecurse nosync nounwind memory(argmem: read) vscale_range(2,1024) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv64" "target-features"="+64bit,+a,+d,+f,+m,+relax,+v,+zaamo,+zalrsc,+zicsr,+zifencei,+zmmul,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvl128b,+zvl32b,+zvl64b,-b,-c,-e,-experimental-sdext,-experimental-sdtrig,-experimental-smctr,-experimental-ssctr,-experimental-svukte,-experimental-xqcia,-experimental-xqciac,-experimental-xqcicli,-experimental-xqcicm,-experimental-xqcics,-experimental-xqcicsr,-experimental-xqciint,-experimental-xqcilsm,-experimental-xqcisls,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-h,-sha,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smdbltrp,-smepmp,-smmpm,-smnpm,-smrnmi,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssdbltrp,-ssnpm,-sspm,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-supm,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-svvptc,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zabha,-zacas,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl4096b,-zvl512b,-zvl65536b,-zvl8192b" }

!llvm.module.flags = !{!0, !1, !2, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64d"}
!2 = !{i32 6, !"riscv-isa", !3}
!3 = !{!"rv64i2p1_m2p0_a2p1_f2p2_d2p2_v1p0_zicsr2p0_zifencei2p0_zmmul1p0_zaamo1p0_zalrsc1p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"}
!4 = !{i32 8, !"SmallDataLimit", i32 0}
!5 = !{!"clang version 20.0.0git (https://github.com/llvm/llvm-project.git edc02351dd11cc4a39b7c541b26b71c6f36c8e55)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = !{!9, !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
```

More debug info:
Looks like the the instruction being lowered is: `renamable $v8m2 = COPY killed renamable $v8m4, implicit $vtype`

The segfault arise because of the following code, where `TRI->getCommonMinimalPhysRegClass(SrcReg, DstReg);` returns a nullptr because there is no minimal class for `V8M2` and `V8M4`:
```cpp
  const TargetRegisterClass *RegClass =
      TRI->getCommonMinimalPhysRegClass(SrcReg, DstReg);
  if (RISCVRegisterInfo::isRVVRegClass(RegClass)) {
    copyPhysRegVector(MBB, MBBI, DL, DstReg, SrcReg, KillSrc, RegClass);
    return;
```

## Comments

### Comment 1 - bababuck

@wangpc-pp looks like this is your code, not sure if the issue is with the IR, this code, or the code generating the copy instruction. If this a compiler issue, I would be interested in making the fix for this, I would just need a little guidance if you're open to that!

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Ryan Buchner (bababuck)

<details>
Built info:
Hash: `edc02351dd`

Command:
```
$LLVM_DIR/bin/llc o3.ll -march=riscv64 -mattr=+v -O3  -o output.s
```

Trace:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /work/rbuchner/llvm/build//bin/llc o3.ll -march=riscv64 -mattr=+v -O3 -o output.s
1.      Running pass 'Function Pass Manager' on module 'o3.ll'.
2.      Running pass 'Post-RA pseudo instruction expansion pass' on function '@<!-- -->strlen'
 #<!-- -->0 0x0000000001ce5388 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/work/rbuchner/llvm/build//bin/llc+0x1ce5388)
 #<!-- -->1 0x0000000001ce2bac SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007fde7f664d10 __restore_rt (/lib64/libpthread.so.0+0x12d10)
 #<!-- -->3 0x0000000000861bc7 llvm::RISCVInstrInfo::copyPhysRegVector(llvm::MachineBasicBlock&amp;, llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, f
alse&gt;, llvm::DebugLoc const&amp;, llvm::MCRegister, llvm::MCRegister, bool, llvm::TargetRegisterClass const*) const (/work/rbuchner/llvm/build//bin
/llc+0x861bc7)
 #<!-- -->4 0x0000000000862e57 llvm::RISCVInstrInfo::copyPhysReg(llvm::MachineBasicBlock&amp;, llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;,
 llvm::DebugLoc const&amp;, llvm::MCRegister, llvm::MCRegister, bool, bool, bool) const (/work/rbuchner/llvm/build//bin/llc+0x862e57)
 #<!-- -->5 0x00000000010afbf4 llvm::TargetInstrInfo::lowerCopy(llvm::MachineInstr*, llvm::TargetRegisterInfo const*) const (/work/rbuchner/llvm/build/
/bin/llc+0x10afbf4)
 #<!-- -->6 0x0000000000d21618 (anonymous namespace)::ExpandPostRA::runOnMachineFunction(llvm::MachineFunction&amp;) ExpandPostRAPseudos.cpp:0:0
 #<!-- -->7 0x0000000000e4ea73 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.69) MachineFunctionPass.cpp:0:0
 #<!-- -->8 0x000000000136d581 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/work/rbuchner/llvm/build//bin/llc+0x136d581)
 #<!-- -->9 0x000000000136d701 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/work/rbuchner/llvm/build//bin/llc+0x136d701)
#<!-- -->10 0x000000000136e144 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/work/rbuchner/llvm/build//bin/llc+0x136e144)
#<!-- -->11 0x000000000082ceae compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->12 0x0000000000774c06 main (/work/rbuchner/llvm/build//bin/llc+0x774c06)
#<!-- -->13 0x00007fde7e0bf7e5 __libc_start_main (/lib64/libc.so.6+0x3a7e5)
#<!-- -->14 0x0000000000824ade _start (/work/rbuchner/llvm/build//bin/llc+0x824ade)
```

Input file:
```
; ModuleID = '../o3.ll'
source_filename = "loop.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: read) vscale_range(2,1024)
define dso_local noundef signext i64 @<!-- -->strlen(ptr noundef %0) local_unnamed_addr #<!-- -->0 {
  br label %2

2:                                                ; preds = %1
  %3 = phi ptr [ %0, %1 ]
  %4 = call { &lt;4 x i64&gt;, i64 } @<!-- -->llvm.riscv.vleff.v4i64.i64(&lt;4 x i64&gt; undef, ptr %3, i64 4)
  %5 = extractvalue { &lt;4 x i64&gt;, i64 } %4, 0
  %6 = extractvalue { &lt;4 x i64&gt;, i64 } %4, 1
  %7 = icmp eq &lt;4 x i64&gt; %5, splat (i64 1)
  store &lt;4 x i1&gt; %7, ptr %3, align 1, !tbaa !8
  ret i64 %6
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare { &lt;4 x i64&gt;, i64 } @<!-- -->llvm.riscv.vleff.v4i64.i64(&lt;4 x i64&gt;, ptr nocapture, i64) #<!-- -->3

attributes #<!-- -->0 = { nofree norecurse nosync nounwind memory(argmem: read) vscale_range(2,1024) "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic-rv64" "target-features"="+64bit,+a,+d,+f,+m,+relax,+v,+zaamo,+zalrsc,+zicsr,+zifencei,+zmmul,+zve32f,+zve32x,+zve64d,+zve64f,+zve64x,+zvl128b,+zvl32b,+zvl64b,-b,-c,-e,-experimental-sdext,-experimental-sdtrig,-experimental-smctr,-experimental-ssctr,-experimental-svukte,-experimental-xqcia,-experimental-xqciac,-experimental-xqcicli,-experimental-xqcicm,-experimental-xqcics,-experimental-xqcicsr,-experimental-xqciint,-experimental-xqcilsm,-experimental-xqcisls,-experimental-zalasr,-experimental-zicfilp,-experimental-zicfiss,-experimental-zvbc32e,-experimental-zvkgs,-h,-sha,-shcounterenw,-shgatpa,-shtvala,-shvsatpa,-shvstvala,-shvstvecd,-smaia,-smcdeleg,-smcsrind,-smdbltrp,-smepmp,-smmpm,-smnpm,-smrnmi,-smstateen,-ssaia,-ssccfg,-ssccptr,-sscofpmf,-sscounterenw,-sscsrind,-ssdbltrp,-ssnpm,-sspm,-ssqosid,-ssstateen,-ssstrict,-sstc,-sstvala,-sstvecd,-ssu64xl,-supm,-svade,-svadu,-svbare,-svinval,-svnapot,-svpbmt,-svvptc,-xcvalu,-xcvbi,-xcvbitmanip,-xcvelw,-xcvmac,-xcvmem,-xcvsimd,-xsfcease,-xsfvcp,-xsfvfnrclipxfqf,-xsfvfwmaccqqq,-xsfvqmaccdod,-xsfvqmaccqoq,-xsifivecdiscarddlone,-xsifivecflushdlone,-xtheadba,-xtheadbb,-xtheadbs,-xtheadcmo,-xtheadcondmov,-xtheadfmemidx,-xtheadmac,-xtheadmemidx,-xtheadmempair,-xtheadsync,-xtheadvdot,-xventanacondops,-xwchc,-za128rs,-za64rs,-zabha,-zacas,-zama16b,-zawrs,-zba,-zbb,-zbc,-zbkb,-zbkc,-zbkx,-zbs,-zca,-zcb,-zcd,-zce,-zcf,-zcmop,-zcmp,-zcmt,-zdinx,-zfa,-zfbfmin,-zfh,-zfhmin,-zfinx,-zhinx,-zhinxmin,-zic64b,-zicbom,-zicbop,-zicboz,-ziccamoa,-ziccif,-zicclsm,-ziccrse,-zicntr,-zicond,-zihintntl,-zihintpause,-zihpm,-zimop,-zk,-zkn,-zknd,-zkne,-zknh,-zkr,-zks,-zksed,-zksh,-zkt,-ztso,-zvbb,-zvbc,-zvfbfmin,-zvfbfwma,-zvfh,-zvfhmin,-zvkb,-zvkg,-zvkn,-zvknc,-zvkned,-zvkng,-zvknha,-zvknhb,-zvks,-zvksc,-zvksed,-zvksg,-zvksh,-zvkt,-zvl1024b,-zvl16384b,-zvl2048b,-zvl256b,-zvl32768b,-zvl4096b,-zvl512b,-zvl65536b,-zvl8192b" }

!llvm.module.flags = !{!0, !1, !2, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64d"}
!2 = !{i32 6, !"riscv-isa", !3}
!3 = !{!"rv64i2p1_m2p0_a2p1_f2p2_d2p2_v1p0_zicsr2p0_zifencei2p0_zmmul1p0_zaamo1p0_zalrsc1p0_zve32f1p0_zve32x1p0_zve64d1p0_zve64f1p0_zve64x1p0_zvl128b1p0_zvl32b1p0_zvl64b1p0"}
!4 = !{i32 8, !"SmallDataLimit", i32 0}
!5 = !{!"clang version 20.0.0git (https://github.com/llvm/llvm-project.git edc02351dd11cc4a39b7c541b26b71c6f36c8e55)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = !{!9, !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
```

More debug info:
Looks like the the instruction being lowered is: `renamable $v8m2 = COPY killed renamable $v8m4, implicit $vtype`

The segfault arise because of the following code, where `TRI-&gt;getCommonMinimalPhysRegClass(SrcReg, DstReg);` returns a nullptr because there is no minimal class for `V8M2` and `V8M4`:
```cpp
  const TargetRegisterClass *RegClass =
      TRI-&gt;getCommonMinimalPhysRegClass(SrcReg, DstReg);
  if (RISCVRegisterInfo::isRVVRegClass(RegClass)) {
    copyPhysRegVector(MBB, MBBI, DL, DstReg, SrcReg, KillSrc, RegClass);
    return;
```
</details>


---

### Comment 3 - topperc

The llvm.riscv.vleff.v4i64.i64 intrinsic should only be used with scalable vector types. It's part of our C intrinsic interface where clang enforces the types that can be used with it. It's not a generic intrinsic.

A build with assertions enabled fails earlier `llc: llvm/lib/Target/RISCV/RISCVISelLowering.cpp:2405: static RISCVVType::VLMUL llvm::RISCVTargetLowering::getLMUL(MVT): Assertion `VT.isScalableVector() && "Expecting a scalable vector type"' failed.`

We have proposed a generic intrinsic for the autovectorizer here https://github.com/llvm/llvm-project/pull/128593

---

### Comment 4 - bababuck

@topperc thank you for the help.

I'm interested in contributing to the Risc-V backend, feel free to tag me if you ever have any tasks that you don't want to deal with!

---

### Comment 5 - wangpc-pp

Thanks for reporting! I think this is an improper way to use the intrinsic just like what @topperc has said so I will close this issue. 🤝

---

