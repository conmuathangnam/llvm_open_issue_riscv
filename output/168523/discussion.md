# [RISCV] Crash at -O2: Assertion `NewLen >= V.getBitWidth() && "Can't splat to smaller bit width!"' failed.

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/168523
**Status:** Closed
**Labels:** backend:RISC-V, llvm:globalisel, crash
**Closed Date:** 2025-11-19T08:12:55Z

## Body

This code crashed at `-O2 -fglobal-isel`:

```c
int *a;
int b() {
  int c = 1;
  int d = 1;
  if (a[1] < 0)
    c = 0;
  if (a[2] < 0)
    d = 0;
  return c + d;
}
```

Crash:

```sh
clang-22: /data/yunboni/utils/compilers/repos/llvm-project/llvm/lib/Support/APInt.cpp:652: static APInt llvm::APInt::getSplat(unsigned int, const APInt &): Assertion `NewLen >= V.getBitWidth() && "Can't splat to smaller bit width!"' failed.  
```

Backtrace:

```sh
Stack dump:                                                                                                                                                                                                 
0.      Program arguments: /data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22 -cc1 -triple riscv64-unknown-linux-gnu -O2 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-na
me case.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu generic-rv64 -target-fea
ture +i -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature +zicsr -target-feature +zifencei -target-feature +zmmul -target-fe
ature +zaamo -target-feature +zalrsc -target-feature +zca -target-feature +zcd -target-feature +zve32f -target-feature +zve32x -target-feature +zve64d -target-feature +zve64f -target-feature +zve64x -targ
et-feature +zvl128b -target-feature +zvl32b -target-feature +zvl64b -target-feature -b -target-feature -e -target-feature -h -target-feature -q -target-feature -sdext -target-feature -sdtrig -target-featu
re -sha -target-feature -shcounterenw -target-feature -shgatpa -target-feature -shlcofideleg -target-feature -shtvala -target-feature -shvsatpa -target-feature -shvstvala -target-feature -shvstvecd -targe
t-feature -smaia -target-feature -smcdeleg -target-feature -smcntrpmf -target-feature -smcsrind -target-feature -smctr -target-feature -smdbltrp -target-feature -smepmp -target-feature -smmpm -target-feat
ure -smnpm -target-feature -smrnmi -target-feature -smstateen -target-feature -ssaia -target-feature -ssccfg -target-feature -ssccptr -target-feature -sscofpmf -target-feature -sscounterenw -target-featur
e -sscsrind -target-feature -ssctr -target-feature -ssdbltrp -target-feature -ssnpm -target-feature -sspm -target-feature -ssqosid -target-feature -ssstateen -target-feature -ssstrict -target-feature -sst
c -target-feature -sstvala -target-feature -sstvecd -target-feature -ssu64xl -target-feature -supm -target-feature -svade -target-feature -svadu -target-feature -svbare -target-feature -svinval -target-fe
ature -svnapot -target-feature -svpbmt -target-feature -svvptc -target-feature -xandesbfhcvt -target-feature -xandesperf -target-feature -xandesvbfhcvt -target-feature -xandesvdot -target-feature -xandesv
packfph -target-feature -xandesvsinth -target-feature -xandesvsintload -target-feature -xcvalu -target-feature -xcvbi -target-feature -xcvbitmanip -target-feature -xcvelw -target-feature -xcvmac -target-f
eature -xcvmem -target-feature -xcvsimd -target-feature -xmipscbop -target-feature -xmipscmov -target-feature -xmipsexectl -target-feature -xmipslsp -target-feature -xsfcease -target-feature -xsfmm128t -t
arget-feature -xsfmm16t -target-feature -xsfmm32a16f -target-feature -xsfmm32a32f -target-feature -xsfmm32a8f -target-feature -xsfmm32a8i -target-feature -xsfmm32t -target-feature -xsfmm64a64f -target-fea
ture -xsfmm64t -target-feature -xsfmmbase -target-feature -xsfvcp -target-feature -xsfvfbfexp16e -target-feature -xsfvfexp16e -target-feature -xsfvfexp32e -target-feature -xsfvfexpa -target-feature -xsfvf
expa64e -target-feature -xsfvfnrclipxfqf -target-feature -xsfvfwmaccqqq -target-feature -xsfvqmaccdod -target-feature -xsfvqmaccqoq -target-feature -xsifivecdiscarddlone -target-feature -xsifivecflushdlon
e -target-feature -xsmtvdot -target-feature -xtheadba -target-feature -xtheadbb -target-feature -xtheadbs -target-feature -xtheadcmo -target-feature -xtheadcondmov -target-feature -xtheadfmemidx -target-f
eature -xtheadmac -target-feature -xtheadmemidx -target-feature -xtheadmempair -target-feature -xtheadsync -target-feature -xtheadvdot -target-feature -xventanacondops -target-feature -xwchc -target-featu
re -za128rs -target-feature -za64rs -target-feature -zabha -target-feature -zacas -target-feature -zama16b -target-feature -zawrs -target-feature -zba -target-feature -zbb -target-feature -zbc -target-fea
ture -zbkb -target-feature -zbkc -target-feature -zbkx -target-feature -zbs -target-feature -zcb -target-feature -zce -target-feature -zcf -target-feature -zclsd -target-feature -zcmop -target-feature -zc
mp -target-feature -zcmt -target-feature -zdinx -target-feature -zfa -target-feature -zfbfmin -target-feature -zfh -target-feature -zfhmin -target-feature -zfinx -target-feature -zhinx -target-feature -zh
inxmin -target-feature -zic64b -target-feature -zicbom -target-feature -zicbop -target-feature -zicboz -target-feature -ziccamoa -target-feature -ziccamoc -target-feature -ziccif -target-feature -zicclsm 
-target-feature -ziccrse -target-feature -zicntr -target-feature -zicond -target-feature -zihintntl -target-feature -zihintpause -target-feature -zihpm -target-feature -zilsd -target-feature -zimop -targe
t-feature -zk -target-feature -zkn -target-feature -zknd -target-feature -zkne -target-feature -zknh -target-feature -zkr -target-feature -zks -target-feature -zksed -target-feature -zksh -target-feature 
-zkt -target-feature -ztso -target-feature -zvbb -target-feature -zvbc -target-feature -zvfbfmin -target-feature -zvfbfwma -target-feature -zvfh -target-feature -zvfhmin -target-feature -zvkb -target-feat
ure -zvkg -target-feature -zvkn -target-feature -zvknc -target-feature -zvkned -target-feature -zvkng -target-feature -zvknha -target-feature -zvknhb -target-feature -zvks -target-feature -zvksc -target-f
eature -zvksed -target-feature -zvksg -target-feature -zvksh -target-feature -zvkt -target-feature -zvl1024b -target-feature -zvl16384b -target-feature -zvl2048b -target-feature -zvl256b -target-feature -
zvl32768b -target-feature -zvl4096b -target-feature -zvl512b -target-feature -zvl65536b -target-feature -zvl8192b -target-feature -experimental-p -target-feature -experimental-smpmpmt -target-feature -exp
erimental-svukte -target-feature -experimental-xqccmp -target-feature -experimental-xqcia -target-feature -experimental-xqciac -target-feature -experimental-xqcibi -target-feature -experimental-xqcibm -ta
rget-feature -experimental-xqcicli -target-feature -experimental-xqcicm -target-feature -experimental-xqcics -target-feature -experimental-xqcicsr -target-feature -experimental-xqciint -target-feature -ex
perimental-xqciio -target-feature -experimental-xqcilb -target-feature -experimental-xqcili -target-feature -experimental-xqcilia -target-feature -experimental-xqcilo -target-feature -experimental-xqcilsm
 -target-feature -experimental-xqcisim -target-feature -experimental-xqcisls -target-feature -experimental-xqcisync -target-feature -experimental-xrivosvisni -target-feature -experimental-xrivosvizip -tar
get-feature -experimental-xsfmclic -target-feature -experimental-xsfsclic -target-feature -experimental-zalasr -target-feature -experimental-zibi -target-feature -experimental-zicfilp -target-feature -experimental-zicfiss -target-feature -experimental-zvbc32e -target-feature -experimental-zvfbfa -target-feature -experimental-zvfofp8min -target-feature -experimental-zvkgs -target-feature -experimental-zvqdotq -target-feature +experimental -target-feature +relax -target-abi lp64d -debugger-tuning=gdb -fdebug-compilation-dir=/data/yunboni/projects/LegoFuzz/bugs/reported/case_kloPTwYu -fcoverage-compilation-dir=/data/yunboni/projects/LegoFuzz/bugs/reported/case_kloPTwYu -resource-dir /data/yunboni/utils/compilers/llvm_latest_rvv/lib/clang/22 -I /data/yunboni/utils/compilers/llvm_latest_rvv/lib/clang/22/include/riscv_vector.h -internal-isystem /data/yunboni/utils/compilers/llvm_latest_rvv/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc-cross/riscv64-linux-gnu/13/../../../../riscv64-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=204 -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -global-isel=1 -mllvm -global-isel-abort=2 -faddrsig -fdwarf2-cfi-asm -o /tmp/case-30ef90.o -x c case.c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'case.c'.
4.      Running pass 'Legalizer' on function '@b'
 #0 0x00007a4f72cdf0e7 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0xcdf0e7)
 #1 0x00007a4f72cdae22 llvm::sys::RunSignalHandlers() (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0xcdae22)
 #2 0x00007a4f72ce012d SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007a4f71845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x00007a4f7189eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #5 0x00007a4f7184527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #6 0x00007a4f718288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #7 0x00007a4f7182881b (/lib/x86_64-linux-gnu/libc.so.6+0x2881b)
 #8 0x00007a4f7183b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
 #9 0x00007a4f72aa547c (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0xaa547c)
#10 0x00007a4f7488ddc5 llvm::LegalizerHelper::lowerBitCount(llvm::MachineInstr&) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x288ddc5)
#11 0x00007a4f748423f9 llvm::Legalizer::legalizeMachineFunction(llvm::MachineFunction&, llvm::LegalizerInfo const&, llvm::ArrayRef<llvm::GISelChangeObserver*>, llvm::LostDebugLocObserver&, llvm::MachineIRBuilder&, llvm::GISelValueTracking*) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x28423f9)
#12 0x00007a4f748458e1 llvm::Legalizer::runOnMachineFunction(llvm::MachineFunction&) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x28458e1)
#13 0x00007a4f73857fe3 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x1857fe3)
#14 0x00007a4f73072a02 llvm::FPPassManager::runOnFunction(llvm::Function&) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x1072a02)
#15 0x00007a4f7307f231 llvm::FPPassManager::runOnModule(llvm::Module&) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x107f231)
#16 0x00007a4f7307380c llvm::legacy::PassManagerImpl::run(llvm::Module&) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x107380c)
#17 0x00007a4f8981d9cb clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x361d9cb)
#18 0x00007a4f89ec4884 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x3cc4884)
#19 0x00007a4f8717992c clang::ParseAST(clang::Sema&, bool, bool) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0xf7992c)
#20 0x00007a4f8afba6e3 clang::FrontendAction::Execute() (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x4dba6e3)
#21 0x00007a4f8ae7f06b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x4c7f06b)
#22 0x00007a4f8b0ee189 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x4eee189)
#23 0x000063838281cab1 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22+0x1cab1)
#24 0x0000638382812254 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#25 0x000063838281062f clang_main(int, char**, llvm::ToolContext const&) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22+0x1062f)
#26 0x0000638382838f15 main (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22+0x38f15)
#27 0x00007a4f7182a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#28 0x00007a4f7182a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#29 0x000063838280d265 _start (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22+0xd265)
```

Compiler version:

```sh
clang version 22.0.0git (https://github.com/llvm/llvm-project.git f15b756b56d0653181f062901916806bc5eba280)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/yunboni/utils/compilers/llvm_latest_rvv/bin
Build config: +assertions
```


## Comments

### Comment 1 - XChy

IR Reproducer: https://godbolt.org/z/x8q51MxTE

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Yunbo Ni (cardigan1008)

<details>
This code crashed at `-O2 -fglobal-isel`:

```c
int *a;
int b() {
  int c = 1;
  int d = 1;
  if (a[1] &lt; 0)
    c = 0;
  if (a[2] &lt; 0)
    d = 0;
  return c + d;
}
```

Crash:

```sh
clang-22: /data/yunboni/utils/compilers/repos/llvm-project/llvm/lib/Support/APInt.cpp:652: static APInt llvm::APInt::getSplat(unsigned int, const APInt &amp;): Assertion `NewLen &gt;= V.getBitWidth() &amp;&amp; "Can't splat to smaller bit width!"' failed.  
```

Backtrace:

```sh
Stack dump:                                                                                                                                                                                                 
0.      Program arguments: /data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22 -cc1 -triple riscv64-unknown-linux-gnu -O2 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-na
me case.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=none -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu generic-rv64 -target-fea
ture +i -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature +zicsr -target-feature +zifencei -target-feature +zmmul -target-fe
ature +zaamo -target-feature +zalrsc -target-feature +zca -target-feature +zcd -target-feature +zve32f -target-feature +zve32x -target-feature +zve64d -target-feature +zve64f -target-feature +zve64x -targ
et-feature +zvl128b -target-feature +zvl32b -target-feature +zvl64b -target-feature -b -target-feature -e -target-feature -h -target-feature -q -target-feature -sdext -target-feature -sdtrig -target-featu
re -sha -target-feature -shcounterenw -target-feature -shgatpa -target-feature -shlcofideleg -target-feature -shtvala -target-feature -shvsatpa -target-feature -shvstvala -target-feature -shvstvecd -targe
t-feature -smaia -target-feature -smcdeleg -target-feature -smcntrpmf -target-feature -smcsrind -target-feature -smctr -target-feature -smdbltrp -target-feature -smepmp -target-feature -smmpm -target-feat
ure -smnpm -target-feature -smrnmi -target-feature -smstateen -target-feature -ssaia -target-feature -ssccfg -target-feature -ssccptr -target-feature -sscofpmf -target-feature -sscounterenw -target-featur
e -sscsrind -target-feature -ssctr -target-feature -ssdbltrp -target-feature -ssnpm -target-feature -sspm -target-feature -ssqosid -target-feature -ssstateen -target-feature -ssstrict -target-feature -sst
c -target-feature -sstvala -target-feature -sstvecd -target-feature -ssu64xl -target-feature -supm -target-feature -svade -target-feature -svadu -target-feature -svbare -target-feature -svinval -target-fe
ature -svnapot -target-feature -svpbmt -target-feature -svvptc -target-feature -xandesbfhcvt -target-feature -xandesperf -target-feature -xandesvbfhcvt -target-feature -xandesvdot -target-feature -xandesv
packfph -target-feature -xandesvsinth -target-feature -xandesvsintload -target-feature -xcvalu -target-feature -xcvbi -target-feature -xcvbitmanip -target-feature -xcvelw -target-feature -xcvmac -target-f
eature -xcvmem -target-feature -xcvsimd -target-feature -xmipscbop -target-feature -xmipscmov -target-feature -xmipsexectl -target-feature -xmipslsp -target-feature -xsfcease -target-feature -xsfmm128t -t
arget-feature -xsfmm16t -target-feature -xsfmm32a16f -target-feature -xsfmm32a32f -target-feature -xsfmm32a8f -target-feature -xsfmm32a8i -target-feature -xsfmm32t -target-feature -xsfmm64a64f -target-fea
ture -xsfmm64t -target-feature -xsfmmbase -target-feature -xsfvcp -target-feature -xsfvfbfexp16e -target-feature -xsfvfexp16e -target-feature -xsfvfexp32e -target-feature -xsfvfexpa -target-feature -xsfvf
expa64e -target-feature -xsfvfnrclipxfqf -target-feature -xsfvfwmaccqqq -target-feature -xsfvqmaccdod -target-feature -xsfvqmaccqoq -target-feature -xsifivecdiscarddlone -target-feature -xsifivecflushdlon
e -target-feature -xsmtvdot -target-feature -xtheadba -target-feature -xtheadbb -target-feature -xtheadbs -target-feature -xtheadcmo -target-feature -xtheadcondmov -target-feature -xtheadfmemidx -target-f
eature -xtheadmac -target-feature -xtheadmemidx -target-feature -xtheadmempair -target-feature -xtheadsync -target-feature -xtheadvdot -target-feature -xventanacondops -target-feature -xwchc -target-featu
re -za128rs -target-feature -za64rs -target-feature -zabha -target-feature -zacas -target-feature -zama16b -target-feature -zawrs -target-feature -zba -target-feature -zbb -target-feature -zbc -target-fea
ture -zbkb -target-feature -zbkc -target-feature -zbkx -target-feature -zbs -target-feature -zcb -target-feature -zce -target-feature -zcf -target-feature -zclsd -target-feature -zcmop -target-feature -zc
mp -target-feature -zcmt -target-feature -zdinx -target-feature -zfa -target-feature -zfbfmin -target-feature -zfh -target-feature -zfhmin -target-feature -zfinx -target-feature -zhinx -target-feature -zh
inxmin -target-feature -zic64b -target-feature -zicbom -target-feature -zicbop -target-feature -zicboz -target-feature -ziccamoa -target-feature -ziccamoc -target-feature -ziccif -target-feature -zicclsm 
-target-feature -ziccrse -target-feature -zicntr -target-feature -zicond -target-feature -zihintntl -target-feature -zihintpause -target-feature -zihpm -target-feature -zilsd -target-feature -zimop -targe
t-feature -zk -target-feature -zkn -target-feature -zknd -target-feature -zkne -target-feature -zknh -target-feature -zkr -target-feature -zks -target-feature -zksed -target-feature -zksh -target-feature 
-zkt -target-feature -ztso -target-feature -zvbb -target-feature -zvbc -target-feature -zvfbfmin -target-feature -zvfbfwma -target-feature -zvfh -target-feature -zvfhmin -target-feature -zvkb -target-feat
ure -zvkg -target-feature -zvkn -target-feature -zvknc -target-feature -zvkned -target-feature -zvkng -target-feature -zvknha -target-feature -zvknhb -target-feature -zvks -target-feature -zvksc -target-f
eature -zvksed -target-feature -zvksg -target-feature -zvksh -target-feature -zvkt -target-feature -zvl1024b -target-feature -zvl16384b -target-feature -zvl2048b -target-feature -zvl256b -target-feature -
zvl32768b -target-feature -zvl4096b -target-feature -zvl512b -target-feature -zvl65536b -target-feature -zvl8192b -target-feature -experimental-p -target-feature -experimental-smpmpmt -target-feature -exp
erimental-svukte -target-feature -experimental-xqccmp -target-feature -experimental-xqcia -target-feature -experimental-xqciac -target-feature -experimental-xqcibi -target-feature -experimental-xqcibm -ta
rget-feature -experimental-xqcicli -target-feature -experimental-xqcicm -target-feature -experimental-xqcics -target-feature -experimental-xqcicsr -target-feature -experimental-xqciint -target-feature -ex
perimental-xqciio -target-feature -experimental-xqcilb -target-feature -experimental-xqcili -target-feature -experimental-xqcilia -target-feature -experimental-xqcilo -target-feature -experimental-xqcilsm
 -target-feature -experimental-xqcisim -target-feature -experimental-xqcisls -target-feature -experimental-xqcisync -target-feature -experimental-xrivosvisni -target-feature -experimental-xrivosvizip -tar
get-feature -experimental-xsfmclic -target-feature -experimental-xsfsclic -target-feature -experimental-zalasr -target-feature -experimental-zibi -target-feature -experimental-zicfilp -target-feature -experimental-zicfiss -target-feature -experimental-zvbc32e -target-feature -experimental-zvfbfa -target-feature -experimental-zvfofp8min -target-feature -experimental-zvkgs -target-feature -experimental-zvqdotq -target-feature +experimental -target-feature +relax -target-abi lp64d -debugger-tuning=gdb -fdebug-compilation-dir=/data/yunboni/projects/LegoFuzz/bugs/reported/case_kloPTwYu -fcoverage-compilation-dir=/data/yunboni/projects/LegoFuzz/bugs/reported/case_kloPTwYu -resource-dir /data/yunboni/utils/compilers/llvm_latest_rvv/lib/clang/22 -I /data/yunboni/utils/compilers/llvm_latest_rvv/lib/clang/22/include/riscv_vector.h -internal-isystem /data/yunboni/utils/compilers/llvm_latest_rvv/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc-cross/riscv64-linux-gnu/13/../../../../riscv64-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fmessage-length=204 -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -vectorize-loops -vectorize-slp -mllvm -global-isel=1 -mllvm -global-isel-abort=2 -faddrsig -fdwarf2-cfi-asm -o /tmp/case-30ef90.o -x c case.c
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'case.c'.
4.      Running pass 'Legalizer' on function '@<!-- -->b'
 #<!-- -->0 0x00007a4f72cdf0e7 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0xcdf0e7)
 #<!-- -->1 0x00007a4f72cdae22 llvm::sys::RunSignalHandlers() (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0xcdae22)
 #<!-- -->2 0x00007a4f72ce012d SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007a4f71845330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x00007a4f7189eb2c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x9eb2c)
 #<!-- -->5 0x00007a4f7184527e raise (/lib/x86_64-linux-gnu/libc.so.6+0x4527e)
 #<!-- -->6 0x00007a4f718288ff abort (/lib/x86_64-linux-gnu/libc.so.6+0x288ff)
 #<!-- -->7 0x00007a4f7182881b (/lib/x86_64-linux-gnu/libc.so.6+0x2881b)
 #<!-- -->8 0x00007a4f7183b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
 #<!-- -->9 0x00007a4f72aa547c (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0xaa547c)
#<!-- -->10 0x00007a4f7488ddc5 llvm::LegalizerHelper::lowerBitCount(llvm::MachineInstr&amp;) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x288ddc5)
#<!-- -->11 0x00007a4f748423f9 llvm::Legalizer::legalizeMachineFunction(llvm::MachineFunction&amp;, llvm::LegalizerInfo const&amp;, llvm::ArrayRef&lt;llvm::GISelChangeObserver*&gt;, llvm::LostDebugLocObserver&amp;, llvm::MachineIRBuilder&amp;, llvm::GISelValueTracking*) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x28423f9)
#<!-- -->12 0x00007a4f748458e1 llvm::Legalizer::runOnMachineFunction(llvm::MachineFunction&amp;) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x28458e1)
#<!-- -->13 0x00007a4f73857fe3 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x1857fe3)
#<!-- -->14 0x00007a4f73072a02 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x1072a02)
#<!-- -->15 0x00007a4f7307f231 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x107f231)
#<!-- -->16 0x00007a4f7307380c llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libLLVM.so.22.0git+0x107380c)
#<!-- -->17 0x00007a4f8981d9cb clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x361d9cb)
#<!-- -->18 0x00007a4f89ec4884 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x3cc4884)
#<!-- -->19 0x00007a4f8717992c clang::ParseAST(clang::Sema&amp;, bool, bool) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0xf7992c)
#<!-- -->20 0x00007a4f8afba6e3 clang::FrontendAction::Execute() (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x4dba6e3)
#<!-- -->21 0x00007a4f8ae7f06b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x4c7f06b)
#<!-- -->22 0x00007a4f8b0ee189 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/../lib/libclang-cpp.so.22.0git+0x4eee189)
#<!-- -->23 0x000063838281cab1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22+0x1cab1)
#<!-- -->24 0x0000638382812254 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->25 0x000063838281062f clang_main(int, char**, llvm::ToolContext const&amp;) (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22+0x1062f)
#<!-- -->26 0x0000638382838f15 main (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22+0x38f15)
#<!-- -->27 0x00007a4f7182a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->28 0x00007a4f7182a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->29 0x000063838280d265 _start (/data/yunboni/utils/compilers/llvm_latest_rvv/bin/clang-22+0xd265)
```

Compiler version:

```sh
clang version 22.0.0git (https://github.com/llvm/llvm-project.git f15b756b56d0653181f062901916806bc5eba280)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /data/yunboni/utils/compilers/llvm_latest_rvv/bin
Build config: +assertions
```

</details>


---

### Comment 3 - XChy

Reduced testcase:
```llvm
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-unknown"

define i2 @b() {
entry:
  %0 = tail call i2 @llvm.ctpop.i2(i2 0)
  ret i2 %0
}

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i2 @llvm.ctpop.i2(i2) #0

attributes #0 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
```

---

### Comment 4 - XChy

#168559 should fix the problem of ctpop.
But the legalization problem of bitcast in the original reproducer remains, possibly related to #168495:
```
LLVM ERROR: unable to legalize instruction: %8:_(s2) = G_BITCAST %7:_(<2 x s1>) (in function: b)
```

---

### Comment 5 - XChy

ISel uses the fallback path now. The C reproducer doesn't crash Clang anymore.

---

