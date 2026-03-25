# [llvm][RVV] Crash during compilation

**Author:** ChipKerchner
**URL:** https://github.com/llvm/llvm-project/issues/171873
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2026-01-05T21:50:16Z

## Body

```
Stack dump:
0.      Program arguments: /proj_sw/user_dev/ckerchner/tmp/tt-riscv-toolchain-20250709/bin/clang-21 -cc1 -triple riscv64-unknown-linux-gnu -O3 -emit-obj -dumpdir testEigen- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name testEigen.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu generic-rv64 -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature +zicsr -target-feature +zifencei -target-feature +zmmul -target-feature +zaamo -target-feature +zalrsc -target-feature +zfbfmin -target-feature +zfh -target-feature +zfhmin -target-feature +zca -target-feature +zcd -target-feature +zve32f -target-feature +zve32x -target-feature +zve64d -target-feature +zve64f -target-feature +zve64x -target-feature +zvfbfmin -target-feature +zvfbfwma -target-feature +zvfh -target-feature +zvfhmin -target-feature +zvl128b -target-feature +zvl256b -target-feature +zvl32b -target-feature +zvl64b -target-feature -b -target-feature -e -target-feature -h -target-feature -q -target-feature -sdext -target-feature -sdtrig -target-feature -sha -target-feature -shcounterenw -target-feature -shgatpa -target-feature -shlcofideleg -target-feature -shtvala -target-feature -shvsatpa -target-feature -shvstvala -target-feature -shvstvecd -target-feature -smaia -target-feature -smcdeleg -target-feature -smcntrpmf -target-feature -smcsrind -target-feature -smdbltrp -target-feature -smepmp -target-feature -smmpm -target-feature -smnpm -target-feature -smrnmi -target-feature -smstateen -target-feature -ssaia -target-feature -ssccfg -target-feature -ssccptr -target-feature -sscofpmf -target-feature -sscounterenw -target-feature -sscsrind -target-feature -ssdbltrp -target-feature -ssnpm -target-feature -sspm -target-feature -ssqosid -target-feature -ssstateen -target-feature -ssstrict -target-feature -sstc -target-feature -sstvala -target-feature -sstvecd -target-feature -ssu64xl -target-feature -supm -target-feature -svade -target-feature -svadu -target-feature -svbare -target-feature -svinval -target-feature -svnapot -target-feature -svpbmt -target-feature -svvptc -target-feature -xandesperf -target-feature -xandesvdot -target-feature -xandesvpackfph -target-feature -xcvalu -target-feature -xcvbi -target-feature -xcvbitmanip -target-feature -xcvelw -target-feature -xcvmac -target-feature -xcvmem -target-feature -xcvsimd -target-feature -xmipscmov -target-feature -xmipslsp -target-feature -xsfcease -target-feature -xsfmm128t -target-feature -xsfmm16t -target-feature -xsfmm32a16f -target-feature -xsfmm32a32f -target-feature -xsfmm32a8f -target-feature -xsfmm32a8i -target-feature -xsfmm32t -target-feature -xsfmm64a64f -target-feature -xsfmm64t -target-feature -xsfmmbase -target-feature -xsfvcp -target-feature -xsfvfnrclipxfqf -target-feature -xsfvfwmaccqqq -target-feature -xsfvqmaccdod -target-feature -xsfvqmaccqoq -target-feature -xsifivecdiscarddlone -target-feature -xsifivecflushdlone -target-feature -xtheadba -target-feature -xtheadbb -target-feature -xtheadbs -target-feature -xtheadcmo -target-feature -xtheadcondmov -target-feature -xtheadfmemidx -target-feature -xtheadmac -target-feature -xtheadmemidx -target-feature -xtheadmempair -target-feature -xtheadsync -target-feature -xtheadvdot -target-feature -xttvqdotq -target-feature -xventanacondops -target-feature -xwchc -target-feature -za128rs -target-feature -za64rs -target-feature -zabha -target-feature -zacas -target-feature -zama16b -target-feature -zawrs -target-feature -zba -target-feature -zbb -target-feature -zbc -target-feature -zbkb -target-feature -zbkc -target-feature -zbkx -target-feature -zbs -target-feature -zcb -target-feature -zce -target-feature -zcf -target-feature -zclsd -target-feature -zcmop -target-feature -zcmp -target-feature -zcmt -target-feature -zdinx -target-feature -zfa -target-feature -zfinx -target-feature -zhinx -target-feature -zhinxmin -target-feature -zic64b -target-feature -zicbom -target-feature -zicbop -target-feature -zicboz -target-feature -ziccamoa -target-feature -ziccamoc -target-feature -ziccif -target-feature -zicclsm -target-feature -ziccrse -target-feature -zicntr -target-feature -zicond -target-feature -zihintntl -target-feature -zihintpause -target-feature -zihpm -target-feature -zilsd -target-feature -zimop -target-feature -zk -target-feature -zkn -target-feature -zknd -target-feature -zkne -target-feature -zknh -target-feature -zkr -target-feature -zks -target-feature -zksed -target-feature -zksh -target-feature -zkt -target-feature -ztso -target-feature -zvbb -target-feature -zvbc -target-feature -zvkb -target-feature -zvkg -target-feature -zvkn -target-feature -zvknc -target-feature -zvkned -target-feature -zvkng -target-feature -zvknha -target-feature -zvknhb -target-feature -zvks -target-feature -zvksc -target-feature -zvksed -target-feature -zvksg -target-feature -zvksh -target-feature -zvkt -target-feature -zvl1024b -target-feature -zvl16384b -target-feature -zvl2048b -target-feature -zvl32768b -target-feature -zvl4096b -target-feature -zvl512b -target-feature -zvl65536b -target-feature -zvl8192b -target-feature -experimental-p -target-feature -experimental-smctr -target-feature -experimental-ssctr -target-feature -experimental-svukte -target-feature -experimental-xqccmp -target-feature -experimental-xqcia -target-feature -experimental-xqciac -target-feature -experimental-xqcibi -target-feature -experimental-xqcibm -target-feature -experimental-xqcicli -target-feature -experimental-xqcicm -target-feature -experimental-xqcics -target-feature -experimental-xqcicsr -target-feature -experimental-xqciint -target-feature -experimental-xqciio -target-feature -experimental-xqcilb -target-feature -experimental-xqcili -target-feature -experimental-xqcilia -target-feature -experimental-xqcilo -target-feature -experimental-xqcilsm -target-feature -experimental-xqcisim -target-feature -experimental-xqcisls -target-feature -experimental-xqcisync -target-feature -experimental-xrivosv@@@
                                                                                                                                                 1,1           Top
1.      ./Eigen/src/Core/arch/Default/GenericPacketMathFunctions.h:23:1: current parser token 'namespace'
2.      ./Eigen/src/Core/arch/RVV10/PacketMathBF16.h:16:11: LLVM IR generation of declaration 'Eigen'
3.      ./Eigen/src/Core/arch/RVV10/PacketMathBF16.h:143:31: Mangling declaration 'Eigen::internal::Bf16ToF32'
riscv64-unknown-linux-gnu-clang++: error: unable to execute command: Segmentation fault (core dumped)
riscv64-unknown-linux-gnu-clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://yyz-gitlab.local.tenstorrent.com/riscv-sw/llvm-project.git)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /proj_sw/user_dev/ckerchner/tmp/tt-riscv-toolchain-20250709/bin
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg: /tmp/testEigen-8f76fd.cpp
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg: /tmp/testEigen-8f76fd.sh
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg:

********************
```

## Comments

### Comment 1 - ChipKerchner

Same program works fine with gcc16

---

### Comment 2 - ChipKerchner

[testEigen-8f76fd.cpp](https://github.com/user-attachments/files/24109044/testEigen-8f76fd.cpp)

---

### Comment 3 - ChipKerchner

[testEigen-8f76fd.sh](https://github.com/user-attachments/files/24109061/testEigen-8f76fd.sh)

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Chip Kerchner (ChipKerchner)

<details>
```
Stack dump:
0.      Program arguments: /proj_sw/user_dev/ckerchner/tmp/tt-riscv-toolchain-20250709/bin/clang-21 -cc1 -triple riscv64-unknown-linux-gnu -O3 -emit-obj -dumpdir testEigen- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name testEigen.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu generic-rv64 -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature +zicsr -target-feature +zifencei -target-feature +zmmul -target-feature +zaamo -target-feature +zalrsc -target-feature +zfbfmin -target-feature +zfh -target-feature +zfhmin -target-feature +zca -target-feature +zcd -target-feature +zve32f -target-feature +zve32x -target-feature +zve64d -target-feature +zve64f -target-feature +zve64x -target-feature +zvfbfmin -target-feature +zvfbfwma -target-feature +zvfh -target-feature +zvfhmin -target-feature +zvl128b -target-feature +zvl256b -target-feature +zvl32b -target-feature +zvl64b -target-feature -b -target-feature -e -target-feature -h -target-feature -q -target-feature -sdext -target-feature -sdtrig -target-feature -sha -target-feature -shcounterenw -target-feature -shgatpa -target-feature -shlcofideleg -target-feature -shtvala -target-feature -shvsatpa -target-feature -shvstvala -target-feature -shvstvecd -target-feature -smaia -target-feature -smcdeleg -target-feature -smcntrpmf -target-feature -smcsrind -target-feature -smdbltrp -target-feature -smepmp -target-feature -smmpm -target-feature -smnpm -target-feature -smrnmi -target-feature -smstateen -target-feature -ssaia -target-feature -ssccfg -target-feature -ssccptr -target-feature -sscofpmf -target-feature -sscounterenw -target-feature -sscsrind -target-feature -ssdbltrp -target-feature -ssnpm -target-feature -sspm -target-feature -ssqosid -target-feature -ssstateen -target-feature -ssstrict -target-feature -sstc -target-feature -sstvala -target-feature -sstvecd -target-feature -ssu64xl -target-feature -supm -target-feature -svade -target-feature -svadu -target-feature -svbare -target-feature -svinval -target-feature -svnapot -target-feature -svpbmt -target-feature -svvptc -target-feature -xandesperf -target-feature -xandesvdot -target-feature -xandesvpackfph -target-feature -xcvalu -target-feature -xcvbi -target-feature -xcvbitmanip -target-feature -xcvelw -target-feature -xcvmac -target-feature -xcvmem -target-feature -xcvsimd -target-feature -xmipscmov -target-feature -xmipslsp -target-feature -xsfcease -target-feature -xsfmm128t -target-feature -xsfmm16t -target-feature -xsfmm32a16f -target-feature -xsfmm32a32f -target-feature -xsfmm32a8f -target-feature -xsfmm32a8i -target-feature -xsfmm32t -target-feature -xsfmm64a64f -target-feature -xsfmm64t -target-feature -xsfmmbase -target-feature -xsfvcp -target-feature -xsfvfnrclipxfqf -target-feature -xsfvfwmaccqqq -target-feature -xsfvqmaccdod -target-feature -xsfvqmaccqoq -target-feature -xsifivecdiscarddlone -target-feature -xsifivecflushdlone -target-feature -xtheadba -target-feature -xtheadbb -target-feature -xtheadbs -target-feature -xtheadcmo -target-feature -xtheadcondmov -target-feature -xtheadfmemidx -target-feature -xtheadmac -target-feature -xtheadmemidx -target-feature -xtheadmempair -target-feature -xtheadsync -target-feature -xtheadvdot -target-feature -xttvqdotq -target-feature -xventanacondops -target-feature -xwchc -target-feature -za128rs -target-feature -za64rs -target-feature -zabha -target-feature -zacas -target-feature -zama16b -target-feature -zawrs -target-feature -zba -target-feature -zbb -target-feature -zbc -target-feature -zbkb -target-feature -zbkc -target-feature -zbkx -target-feature -zbs -target-feature -zcb -target-feature -zce -target-feature -zcf -target-feature -zclsd -target-feature -zcmop -target-feature -zcmp -target-feature -zcmt -target-feature -zdinx -target-feature -zfa -target-feature -zfinx -target-feature -zhinx -target-feature -zhinxmin -target-feature -zic64b -target-feature -zicbom -target-feature -zicbop -target-feature -zicboz -target-feature -ziccamoa -target-feature -ziccamoc -target-feature -ziccif -target-feature -zicclsm -target-feature -ziccrse -target-feature -zicntr -target-feature -zicond -target-feature -zihintntl -target-feature -zihintpause -target-feature -zihpm -target-feature -zilsd -target-feature -zimop -target-feature -zk -target-feature -zkn -target-feature -zknd -target-feature -zkne -target-feature -zknh -target-feature -zkr -target-feature -zks -target-feature -zksed -target-feature -zksh -target-feature -zkt -target-feature -ztso -target-feature -zvbb -target-feature -zvbc -target-feature -zvkb -target-feature -zvkg -target-feature -zvkn -target-feature -zvknc -target-feature -zvkned -target-feature -zvkng -target-feature -zvknha -target-feature -zvknhb -target-feature -zvks -target-feature -zvksc -target-feature -zvksed -target-feature -zvksg -target-feature -zvksh -target-feature -zvkt -target-feature -zvl1024b -target-feature -zvl16384b -target-feature -zvl2048b -target-feature -zvl32768b -target-feature -zvl4096b -target-feature -zvl512b -target-feature -zvl65536b -target-feature -zvl8192b -target-feature -experimental-p -target-feature -experimental-smctr -target-feature -experimental-ssctr -target-feature -experimental-svukte -target-feature -experimental-xqccmp -target-feature -experimental-xqcia -target-feature -experimental-xqciac -target-feature -experimental-xqcibi -target-feature -experimental-xqcibm -target-feature -experimental-xqcicli -target-feature -experimental-xqcicm -target-feature -experimental-xqcics -target-feature -experimental-xqcicsr -target-feature -experimental-xqciint -target-feature -experimental-xqciio -target-feature -experimental-xqcilb -target-feature -experimental-xqcili -target-feature -experimental-xqcilia -target-feature -experimental-xqcilo -target-feature -experimental-xqcilsm -target-feature -experimental-xqcisim -target-feature -experimental-xqcisls -target-feature -experimental-xqcisync -target-feature -experimental-xrivosv@@@
                                                                                                                                                 1,1           Top
1.      ./Eigen/src/Core/arch/Default/GenericPacketMathFunctions.h:23:1: current parser token 'namespace'
2.      ./Eigen/src/Core/arch/RVV10/PacketMathBF16.h:16:11: LLVM IR generation of declaration 'Eigen'
3.      ./Eigen/src/Core/arch/RVV10/PacketMathBF16.h:143:31: Mangling declaration 'Eigen::internal::Bf16ToF32'
riscv64-unknown-linux-gnu-clang++: error: unable to execute command: Segmentation fault (core dumped)
riscv64-unknown-linux-gnu-clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://yyz-gitlab.local.tenstorrent.com/riscv-sw/llvm-project.git)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /proj_sw/user_dev/ckerchner/tmp/tt-riscv-toolchain-20250709/bin
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg: /tmp/testEigen-8f76fd.cpp
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg: /tmp/testEigen-8f76fd.sh
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg:

********************
```
</details>


---

### Comment 5 - topperc

We aren't handle BF16 in CXXNameMangler::mangleRISCVFixedRVVVectorType

---

### Comment 6 - ChipKerchner

Any update on this?

---

### Comment 7 - topperc

> Any update on this?

It was fixed by #172095 but it looks like I forgot to tag this issue.

---

### Comment 8 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Chip Kerchner (ChipKerchner)

<details>
```
Stack dump:
0.      Program arguments: /proj_sw/user_dev/ckerchner/tmp/tt-riscv-toolchain-20250709/bin/clang-21 -cc1 -triple riscv64-unknown-linux-gnu -O3 -emit-obj -dumpdir testEigen- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name testEigen.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu generic-rv64 -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature +zicsr -target-feature +zifencei -target-feature +zmmul -target-feature +zaamo -target-feature +zalrsc -target-feature +zfbfmin -target-feature +zfh -target-feature +zfhmin -target-feature +zca -target-feature +zcd -target-feature +zve32f -target-feature +zve32x -target-feature +zve64d -target-feature +zve64f -target-feature +zve64x -target-feature +zvfbfmin -target-feature +zvfbfwma -target-feature +zvfh -target-feature +zvfhmin -target-feature +zvl128b -target-feature +zvl256b -target-feature +zvl32b -target-feature +zvl64b -target-feature -b -target-feature -e -target-feature -h -target-feature -q -target-feature -sdext -target-feature -sdtrig -target-feature -sha -target-feature -shcounterenw -target-feature -shgatpa -target-feature -shlcofideleg -target-feature -shtvala -target-feature -shvsatpa -target-feature -shvstvala -target-feature -shvstvecd -target-feature -smaia -target-feature -smcdeleg -target-feature -smcntrpmf -target-feature -smcsrind -target-feature -smdbltrp -target-feature -smepmp -target-feature -smmpm -target-feature -smnpm -target-feature -smrnmi -target-feature -smstateen -target-feature -ssaia -target-feature -ssccfg -target-feature -ssccptr -target-feature -sscofpmf -target-feature -sscounterenw -target-feature -sscsrind -target-feature -ssdbltrp -target-feature -ssnpm -target-feature -sspm -target-feature -ssqosid -target-feature -ssstateen -target-feature -ssstrict -target-feature -sstc -target-feature -sstvala -target-feature -sstvecd -target-feature -ssu64xl -target-feature -supm -target-feature -svade -target-feature -svadu -target-feature -svbare -target-feature -svinval -target-feature -svnapot -target-feature -svpbmt -target-feature -svvptc -target-feature -xandesperf -target-feature -xandesvdot -target-feature -xandesvpackfph -target-feature -xcvalu -target-feature -xcvbi -target-feature -xcvbitmanip -target-feature -xcvelw -target-feature -xcvmac -target-feature -xcvmem -target-feature -xcvsimd -target-feature -xmipscmov -target-feature -xmipslsp -target-feature -xsfcease -target-feature -xsfmm128t -target-feature -xsfmm16t -target-feature -xsfmm32a16f -target-feature -xsfmm32a32f -target-feature -xsfmm32a8f -target-feature -xsfmm32a8i -target-feature -xsfmm32t -target-feature -xsfmm64a64f -target-feature -xsfmm64t -target-feature -xsfmmbase -target-feature -xsfvcp -target-feature -xsfvfnrclipxfqf -target-feature -xsfvfwmaccqqq -target-feature -xsfvqmaccdod -target-feature -xsfvqmaccqoq -target-feature -xsifivecdiscarddlone -target-feature -xsifivecflushdlone -target-feature -xtheadba -target-feature -xtheadbb -target-feature -xtheadbs -target-feature -xtheadcmo -target-feature -xtheadcondmov -target-feature -xtheadfmemidx -target-feature -xtheadmac -target-feature -xtheadmemidx -target-feature -xtheadmempair -target-feature -xtheadsync -target-feature -xtheadvdot -target-feature -xttvqdotq -target-feature -xventanacondops -target-feature -xwchc -target-feature -za128rs -target-feature -za64rs -target-feature -zabha -target-feature -zacas -target-feature -zama16b -target-feature -zawrs -target-feature -zba -target-feature -zbb -target-feature -zbc -target-feature -zbkb -target-feature -zbkc -target-feature -zbkx -target-feature -zbs -target-feature -zcb -target-feature -zce -target-feature -zcf -target-feature -zclsd -target-feature -zcmop -target-feature -zcmp -target-feature -zcmt -target-feature -zdinx -target-feature -zfa -target-feature -zfinx -target-feature -zhinx -target-feature -zhinxmin -target-feature -zic64b -target-feature -zicbom -target-feature -zicbop -target-feature -zicboz -target-feature -ziccamoa -target-feature -ziccamoc -target-feature -ziccif -target-feature -zicclsm -target-feature -ziccrse -target-feature -zicntr -target-feature -zicond -target-feature -zihintntl -target-feature -zihintpause -target-feature -zihpm -target-feature -zilsd -target-feature -zimop -target-feature -zk -target-feature -zkn -target-feature -zknd -target-feature -zkne -target-feature -zknh -target-feature -zkr -target-feature -zks -target-feature -zksed -target-feature -zksh -target-feature -zkt -target-feature -ztso -target-feature -zvbb -target-feature -zvbc -target-feature -zvkb -target-feature -zvkg -target-feature -zvkn -target-feature -zvknc -target-feature -zvkned -target-feature -zvkng -target-feature -zvknha -target-feature -zvknhb -target-feature -zvks -target-feature -zvksc -target-feature -zvksed -target-feature -zvksg -target-feature -zvksh -target-feature -zvkt -target-feature -zvl1024b -target-feature -zvl16384b -target-feature -zvl2048b -target-feature -zvl32768b -target-feature -zvl4096b -target-feature -zvl512b -target-feature -zvl65536b -target-feature -zvl8192b -target-feature -experimental-p -target-feature -experimental-smctr -target-feature -experimental-ssctr -target-feature -experimental-svukte -target-feature -experimental-xqccmp -target-feature -experimental-xqcia -target-feature -experimental-xqciac -target-feature -experimental-xqcibi -target-feature -experimental-xqcibm -target-feature -experimental-xqcicli -target-feature -experimental-xqcicm -target-feature -experimental-xqcics -target-feature -experimental-xqcicsr -target-feature -experimental-xqciint -target-feature -experimental-xqciio -target-feature -experimental-xqcilb -target-feature -experimental-xqcili -target-feature -experimental-xqcilia -target-feature -experimental-xqcilo -target-feature -experimental-xqcilsm -target-feature -experimental-xqcisim -target-feature -experimental-xqcisls -target-feature -experimental-xqcisync -target-feature -experimental-xrivosv@@@
                                                                                                                                                 1,1           Top
1.      ./Eigen/src/Core/arch/Default/GenericPacketMathFunctions.h:23:1: current parser token 'namespace'
2.      ./Eigen/src/Core/arch/RVV10/PacketMathBF16.h:16:11: LLVM IR generation of declaration 'Eigen'
3.      ./Eigen/src/Core/arch/RVV10/PacketMathBF16.h:143:31: Mangling declaration 'Eigen::internal::Bf16ToF32'
riscv64-unknown-linux-gnu-clang++: error: unable to execute command: Segmentation fault (core dumped)
riscv64-unknown-linux-gnu-clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://yyz-gitlab.local.tenstorrent.com/riscv-sw/llvm-project.git)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /proj_sw/user_dev/ckerchner/tmp/tt-riscv-toolchain-20250709/bin
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg: /tmp/testEigen-8f76fd.cpp
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg: /tmp/testEigen-8f76fd.sh
riscv64-unknown-linux-gnu-clang++: note: diagnostic msg:

********************
```
</details>


---

### Comment 9 - ChipKerchner

Thanks!

---

