# Crash when lowering memcpy for RISC-V with long vectors

**Author:** al42and
**URL:** https://github.com/llvm/llvm-project/issues/139075
**Status:** Closed
**Labels:** backend:RISC-V, crash-on-valid
**Closed Date:** 2025-05-08T23:27:37Z

## Body

When cross-compiling the following code for `-march=rv64gcv -mrvv-vector-bits=16384`, I get an ICE

```c
void* memcpy();
struct s
{
    char g[2048];
};
void a(struct s *dst, const int *src)
{
    memcpy(dst, src, sizeof(*dst));
}
```

Compiler Explorer: https://godbolt.org/z/s3dKf9sGP

```console
$ clang --version
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 9732427f377ec076601d4d6ede697f39a4409ee4)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/aland/llvm-project/build/install/bin

$ clang --target=riscv64-linux-gnu -march=rv64gcv -mrvv-vector-bits=16384 -O0 test.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/aland/llvm-project/build/install/bin/clang-21 -cc1 -triple riscv64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu generic-rv64 -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature +zicsr -target-feature +zifencei -target-feature +zmmul -target-feature +zaamo -target-feature +zalrsc -target-feature +zca -target-feature +zcd -target-feature +zve32f -target-feature +zve32x -target-feature +zve64d -target-feature +zve64f -target-feature +zve64x -target-feature +zvl128b -target-feature +zvl32b -target-feature +zvl64b -target-feature -b -target-feature -e -target-feature -h -target-feature -sdext -target-feature -sdtrig -target-feature -sha -target-feature -shcounterenw -target-feature -shgatpa -target-feature -shtvala -target-feature -shvsatpa -target-feature -shvstvala -target-feature -shvstvecd -target-feature -smaia -target-feature -smcdeleg -target-feature -smcntrpmf -target-feature -smcsrind -target-feature -smdbltrp -target-feature -smepmp -target-feature -smmpm -target-feature -smnpm -target-feature -smrnmi -target-feature -smstateen -target-feature -ssaia -target-feature -ssccfg -target-feature -ssccptr -target-feature -sscofpmf -target-feature -sscounterenw -target-feature -sscsrind -target-feature -ssdbltrp -target-feature -ssnpm -target-feature -sspm -target-feature -ssqosid -target-feature -ssstateen -target-feature -ssstrict -target-feature -sstc -target-feature -sstvala -target-feature -sstvecd -target-feature -ssu64xl -target-feature -supm -target-feature -svade -target-feature -svadu -target-feature -svbare -target-feature -svinval -target-feature -svnapot -target-feature -svpbmt -target-feature -svvptc -target-feature -xandesperf -target-feature -xcvalu -target-feature -xcvbi -target-feature -xcvbitmanip -target-feature -xcvelw -target-feature -xcvmac -target-feature -xcvmem -target-feature -xcvsimd -target-feature -xmipscmov -target-feature -xmipslsp -target-feature -xsfcease -target-feature -xsfvcp -target-feature -xsfvfnrclipxfqf -target-feature -xsfvfwmaccqqq -target-feature -xsfvqmaccdod -target-feature -xsfvqmaccqoq -target-feature -xsifivecdiscarddlone -target-feature -xsifivecflushdlone -target-feature -xtheadba -target-feature -xtheadbb -target-feature -xtheadbs -target-feature -xtheadcmo -target-feature -xtheadcondmov -target-feature -xtheadfmemidx -target-feature -xtheadmac -target-feature -xtheadmemidx -target-feature -xtheadmempair -target-feature -xtheadsync -target-feature -xtheadvdot -target-feature -xventanacondops -target-feature -xwchc -target-feature -za128rs -target-feature -za64rs -target-feature -zabha -target-feature -zacas -target-feature -zama16b -target-feature -zawrs -target-feature -zba -target-feature -zbb -target-feature -zbc -target-feature -zbkb -target-feature -zbkc -target-feature -zbkx -target-feature -zbs -target-feature -zcb -target-feature -zce -target-feature -zcf -target-feature -zclsd -target-feature -zcmop -target-feature -zcmp -target-feature -zcmt -target-feature -zdinx -target-feature -zfa -target-feature -zfbfmin -target-feature -zfh -target-feature -zfhmin -target-feature -zfinx -target-feature -zhinx -target-feature -zhinxmin -target-feature -zic64b -target-feature -zicbom -target-feature -zicbop -target-feature -zicboz -target-feature -ziccamoa -target-feature -ziccamoc -target-feature -ziccif -target-feature -zicclsm -target-feature -ziccrse -target-feature -zicntr -target-feature -zicond -target-feature -zihintntl -target-feature -zihintpause -target-feature -zihpm -target-feature -zilsd -target-feature -zimop -target-feature -zk -target-feature -zkn -target-feature -zknd -target-feature -zkne -target-feature -zknh -target-feature -zkr -target-feature -zks -target-feature -zksed -target-feature -zksh -target-feature -zkt -target-feature -ztso -target-feature -zvbb -target-feature -zvbc -target-feature -zvfbfmin -target-feature -zvfbfwma -target-feature -zvfh -target-feature -zvfhmin -target-feature -zvkb -target-feature -zvkg -target-feature -zvkn -target-feature -zvknc -target-feature -zvkned -target-feature -zvkng -target-feature -zvknha -target-feature -zvknhb -target-feature -zvks -target-feature -zvksc -target-feature -zvksed -target-feature -zvksg -target-feature -zvksh -target-feature -zvkt -target-feature -zvl1024b -target-feature -zvl16384b -target-feature -zvl2048b -target-feature -zvl256b -target-feature -zvl32768b -target-feature -zvl4096b -target-feature -zvl512b -target-feature -zvl65536b -target-feature -zvl8192b -target-feature -experimental-p -target-feature -experimental-smctr -target-feature -experimental-ssctr -target-feature -experimental-svukte -target-feature -experimental-xqccmp -target-feature -experimental-xqcia -target-feature -experimental-xqciac -target-feature -experimental-xqcibi -target-feature -experimental-xqcibm -target-feature -experimental-xqcicli -target-feature -experimental-xqcicm -target-feature -experimental-xqcics -target-feature -experimental-xqcicsr -target-feature -experimental-xqciint -target-feature -experimental-xqciio -target-feature -experimental-xqcilb -target-feature -experimental-xqcili -target-feature -experimental-xqcilia -target-feature -experimental-xqcilo -target-feature -experimental-xqcilsm -target-feature -experimental-xqcisim -target-feature -experimental-xqcisls -target-feature -experimental-xqcisync -target-feature -experimental-xrivosvisni -target-feature -experimental-xrivosvizip -target-feature -experimental-xsfmclic -target-feature -experimental-xsfsclic -target-feature -experimental-zalasr -target-feature -experimental-zicfilp -target-feature -experimental-zicfiss -target-feature -experimental-zvbc32e -target-feature -experimental-zvkgs -target-feature -experimental-zvqdotq -target-feature +relax -target-abi lp64d -mvscale-max=256 -mvscale-min=256 -debugger-tuning=gdb -fdebug-compilation-dir=/home/aland/gromacs/build/clang-riscv -fcoverage-compilation-dir=/home/aland/gromacs/build/clang-riscv -resource-dir /home/aland/llvm-project/build/install/lib/clang/21 -internal-isystem /home/aland/llvm-project/build/install/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc-cross/riscv64-linux-gnu/12/../../../../riscv64-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -O0 -ferror-limit 19 -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-35c079.o -x c test.c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'test.c'.
4.      Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@a'
 #0 0x0000718b71dae14f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/aland/llvm-project/llvm/lib/Support/Unix/Signals.inc:808:3
 #1 0x0000718b71dac054 llvm::sys::RunSignalHandlers() /home/aland/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #2 0x0000718b71dac3a9 SignalHandler(int, siginfo_t*, void*) /home/aland/llvm-project/llvm/lib/Support/Unix/Signals.inc:418:13
 #3 0x0000718b71442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000718b74fbd778 llvm::CastInfo<llvm::IntegerType, llvm::Type*, void>::doCastIfPossible(llvm::Type* const&) /home/aland/llvm-project/llvm/include/llvm/Support/Casting.h:493:5
 #5 0x0000718b74fbd778 decltype(auto) llvm::dyn_cast<llvm::IntegerType, llvm::Type>(llvm::Type*) /home/aland/llvm-project/llvm/include/llvm/Support/Casting.h:663:48
 #6 0x0000718b74fbd778 llvm::EVT::getExtendedSizeInBits() const /home/aland/llvm-project/llvm/lib/CodeGen/ValueTypes.cpp:154:47
 #7 0x0000718b713601a8 llvm::EVT::getSizeInBits() const /home/aland/llvm-project/llvm/include/llvm/CodeGen/ValueTypes.h:371:35
 #8 0x0000718b713601a8 llvm::TargetLowering::findOptimalMemOpLowering(std::vector<llvm::EVT, std::allocator<llvm::EVT>>&, unsigned int, llvm::MemOp const&, unsigned int, unsigned int, llvm::AttributeList const&) const /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/TargetLowering.cpp:242:39
 #9 0x0000718b712c8b80 getMemcpyLoadsAndStores(llvm::SelectionDAG&, llvm::SDLoc const&, llvm::SDValue, llvm::SDValue, llvm::SDValue, unsigned long, llvm::Align, bool, bool, llvm::MachinePointerInfo, llvm::MachinePointerInfo, llvm::AAMDNodes const&, llvm::BatchAAResults*) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:8244:3
#10 0x0000718b712ca165 llvm::SelectionDAG::getMemcpy(llvm::SDValue, llvm::SDLoc const&, llvm::SDValue, llvm::SDValue, llvm::SDValue, llvm::Align, bool, bool, llvm::CallInst const*, std::optional<bool>, llvm::MachinePointerInfo, llvm::MachinePointerInfo, llvm::AAMDNodes const&, llvm::BatchAAResults*) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:8674:5
#11 0x0000718b712754de llvm::SelectionDAGBuilder::updateDAGForMaybeTailCall(llvm::SDValue) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:11994:3
#12 0x0000718b712754de llvm::SelectionDAGBuilder::visitIntrinsicCall(llvm::CallInst const&, unsigned int) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:6485:30
#13 0x0000718b71281099 llvm::Value::getValueID() const /home/aland/llvm-project/llvm/include/llvm/IR/Value.h:531:12
#14 0x0000718b71281099 llvm::Instruction::getOpcode() const /home/aland/llvm-project/llvm/include/llvm/IR/Instruction.h:310:49
#15 0x0000718b71281099 llvm::Instruction::isTerminator() const /home/aland/llvm-project/llvm/include/llvm/IR/Instruction.h:313:50
#16 0x0000718b71281099 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:1341:22
#17 0x0000718b712f1e14 llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<false, llvm::BasicBlock>, false>::getNext() const /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:28:38
#18 0x0000718b712f1e14 llvm::ilist_node_impl<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>>::getNext() const /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_node.h:127:66
#19 0x0000718b712f1e14 llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>::operator++() /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:345:25
#20 0x0000718b712f1e14 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, bool&) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:862:79
#21 0x0000718b712f2afe llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1906:33
#22 0x0000718b712f42d3 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:615:22
#23 0x0000718b712ec69b llvm::OptLevelChanger::~OptLevelChanger() /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:268:11
#24 0x0000718b712ec69b llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:375:1
#25 0x0000718b712ec69b llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:346:6
#26 0x0000718b74d28902 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) /home/aland/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#27 0x0000718b72214fd5 llvm::FPPassManager::runOnFunction(llvm::Function&) /home/aland/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1416:7
#28 0x0000718b7221520b llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<false, void>, false>::getNext() const /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:28:38
#29 0x0000718b7221520b llvm::ilist_node_impl<llvm::ilist_detail::node_options<llvm::Function, false, false, void, false, void>>::getNext() /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_node.h:119:66
#30 0x0000718b7221520b llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Function, false, false, void, false, void>, false, false>::operator++() /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#31 0x0000718b7221520b llvm::FPPassManager::runOnModule(llvm::Module&) /home/aland/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1451:22
#32 0x0000718b72215b7c runOnModule /home/aland/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1528:7
#33 0x0000718b72215b7c llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/aland/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:539:55
#34 0x0000718b75330e5e RunCodegenPipeline /home/aland/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1263:9
#35 0x0000718b75330e5e emitAssembly /home/aland/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1286:21
#36 0x0000718b75330e5e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/aland/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1451:25
#37 0x0000718b7566e817 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::release() /home/aland/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#38 0x0000718b7566e817 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::~IntrusiveRefCntPtr() /home/aland/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#39 0x0000718b7566e817 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /home/aland/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:316:20
#40 0x0000718b701409fc clang::ParseAST(clang::Sema&, bool, bool) /home/aland/llvm-project/clang/lib/Parse/ParseAST.cpp:184:34
#41 0x0000718b7412d5be clang::FrontendAction::Execute() /home/aland/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1229:38
#42 0x0000718b740b2a6b llvm::Error::getPtr() const /home/aland/llvm-project/llvm/include/llvm/Support/Error.h:281:12
#43 0x0000718b740b2a6b llvm::Error::operator bool() /home/aland/llvm-project/llvm/include/llvm/Support/Error.h:241:22
#44 0x0000718b740b2a6b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/aland/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:42
#45 0x0000718b78a352e8 std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/11/bits/shared_ptr_base.h:1296:16
#46 0x0000718b78a352e8 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/11/bits/shared_ptr_base.h:993:69
#47 0x0000718b78a352e8 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/11/bits/shared_ptr_base.h:980:16
#48 0x0000718b78a352e8 clang::CompilerInvocation::getFrontendOpts() /home/aland/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:259:48
#49 0x0000718b78a352e8 clang::CompilerInstance::getFrontendOpts() /home/aland/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:304:39
#50 0x0000718b78a352e8 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/aland/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:301:29
#51 0x00005b35205c9acf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/aland/llvm-project/clang/tools/driver/cc1_main.cpp:297:40
#52 0x00005b35205c2779 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /home/aland/llvm-project/clang/tools/driver/driver.cpp:223:20
#53 0x00005b35205c505d clang_main(int, char**, llvm::ToolContext const&) /home/aland/llvm-project/clang/tools/driver/driver.cpp:264:26
#54 0x00005b35205c1d8b main /home/aland/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:18:1
#55 0x0000718b71429d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#56 0x0000718b71429e40 call_init ./csu/../csu/libc-start.c:128:20
#57 0x0000718b71429e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#58 0x00005b35205c1de5 _start (/home/aland/llvm-project/build/install/bin/clang-21+0xcde5)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 9732427f377ec076601d4d6ede697f39a4409ee4)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/aland/llvm-project/build/install/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-3d5f2f.c
clang: note: diagnostic msg: /tmp/test-3d5f2f.sh
clang: note: diagnostic msg: 

********************
```

[test-3d5f2f.c.gz](https://github.com/user-attachments/files/20103243/test-3d5f2f.c.gz)
[test-3d5f2f.sh.gz](https://github.com/user-attachments/files/20103242/test-3d5f2f.sh.gz)

Looks like `getOptimalMemOpType` returns `{V = {SimpleTy = llvm::MVT::INVALID_SIMPLE_VALUE_TYPE}, LLVMTy = 0x0}`, and it goes downhill from there.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Andrey Alekseenko (al42and)

<details>
When cross-compiling the following code for `-march=rv64gcv -mrvv-vector-bits=16384`, I get an ICE

```c
void* memcpy();
struct s
{
    char g[2048];
};
void a(struct s *dst, const int *src)
{
    memcpy(dst, src, sizeof(*dst));
}
```

Compiler Explorer: https://godbolt.org/z/s3dKf9sGP

```console
$ clang --version
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 9732427f377ec076601d4d6ede697f39a4409ee4)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/aland/llvm-project/build/install/bin

$ clang --target=riscv64-linux-gnu -march=rv64gcv -mrvv-vector-bits=16384 -O0 test.c
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /home/aland/llvm-project/build/install/bin/clang-21 -cc1 -triple riscv64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name test.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu generic-rv64 -target-feature +m -target-feature +a -target-feature +f -target-feature +d -target-feature +c -target-feature +v -target-feature +zicsr -target-feature +zifencei -target-feature +zmmul -target-feature +zaamo -target-feature +zalrsc -target-feature +zca -target-feature +zcd -target-feature +zve32f -target-feature +zve32x -target-feature +zve64d -target-feature +zve64f -target-feature +zve64x -target-feature +zvl128b -target-feature +zvl32b -target-feature +zvl64b -target-feature -b -target-feature -e -target-feature -h -target-feature -sdext -target-feature -sdtrig -target-feature -sha -target-feature -shcounterenw -target-feature -shgatpa -target-feature -shtvala -target-feature -shvsatpa -target-feature -shvstvala -target-feature -shvstvecd -target-feature -smaia -target-feature -smcdeleg -target-feature -smcntrpmf -target-feature -smcsrind -target-feature -smdbltrp -target-feature -smepmp -target-feature -smmpm -target-feature -smnpm -target-feature -smrnmi -target-feature -smstateen -target-feature -ssaia -target-feature -ssccfg -target-feature -ssccptr -target-feature -sscofpmf -target-feature -sscounterenw -target-feature -sscsrind -target-feature -ssdbltrp -target-feature -ssnpm -target-feature -sspm -target-feature -ssqosid -target-feature -ssstateen -target-feature -ssstrict -target-feature -sstc -target-feature -sstvala -target-feature -sstvecd -target-feature -ssu64xl -target-feature -supm -target-feature -svade -target-feature -svadu -target-feature -svbare -target-feature -svinval -target-feature -svnapot -target-feature -svpbmt -target-feature -svvptc -target-feature -xandesperf -target-feature -xcvalu -target-feature -xcvbi -target-feature -xcvbitmanip -target-feature -xcvelw -target-feature -xcvmac -target-feature -xcvmem -target-feature -xcvsimd -target-feature -xmipscmov -target-feature -xmipslsp -target-feature -xsfcease -target-feature -xsfvcp -target-feature -xsfvfnrclipxfqf -target-feature -xsfvfwmaccqqq -target-feature -xsfvqmaccdod -target-feature -xsfvqmaccqoq -target-feature -xsifivecdiscarddlone -target-feature -xsifivecflushdlone -target-feature -xtheadba -target-feature -xtheadbb -target-feature -xtheadbs -target-feature -xtheadcmo -target-feature -xtheadcondmov -target-feature -xtheadfmemidx -target-feature -xtheadmac -target-feature -xtheadmemidx -target-feature -xtheadmempair -target-feature -xtheadsync -target-feature -xtheadvdot -target-feature -xventanacondops -target-feature -xwchc -target-feature -za128rs -target-feature -za64rs -target-feature -zabha -target-feature -zacas -target-feature -zama16b -target-feature -zawrs -target-feature -zba -target-feature -zbb -target-feature -zbc -target-feature -zbkb -target-feature -zbkc -target-feature -zbkx -target-feature -zbs -target-feature -zcb -target-feature -zce -target-feature -zcf -target-feature -zclsd -target-feature -zcmop -target-feature -zcmp -target-feature -zcmt -target-feature -zdinx -target-feature -zfa -target-feature -zfbfmin -target-feature -zfh -target-feature -zfhmin -target-feature -zfinx -target-feature -zhinx -target-feature -zhinxmin -target-feature -zic64b -target-feature -zicbom -target-feature -zicbop -target-feature -zicboz -target-feature -ziccamoa -target-feature -ziccamoc -target-feature -ziccif -target-feature -zicclsm -target-feature -ziccrse -target-feature -zicntr -target-feature -zicond -target-feature -zihintntl -target-feature -zihintpause -target-feature -zihpm -target-feature -zilsd -target-feature -zimop -target-feature -zk -target-feature -zkn -target-feature -zknd -target-feature -zkne -target-feature -zknh -target-feature -zkr -target-feature -zks -target-feature -zksed -target-feature -zksh -target-feature -zkt -target-feature -ztso -target-feature -zvbb -target-feature -zvbc -target-feature -zvfbfmin -target-feature -zvfbfwma -target-feature -zvfh -target-feature -zvfhmin -target-feature -zvkb -target-feature -zvkg -target-feature -zvkn -target-feature -zvknc -target-feature -zvkned -target-feature -zvkng -target-feature -zvknha -target-feature -zvknhb -target-feature -zvks -target-feature -zvksc -target-feature -zvksed -target-feature -zvksg -target-feature -zvksh -target-feature -zvkt -target-feature -zvl1024b -target-feature -zvl16384b -target-feature -zvl2048b -target-feature -zvl256b -target-feature -zvl32768b -target-feature -zvl4096b -target-feature -zvl512b -target-feature -zvl65536b -target-feature -zvl8192b -target-feature -experimental-p -target-feature -experimental-smctr -target-feature -experimental-ssctr -target-feature -experimental-svukte -target-feature -experimental-xqccmp -target-feature -experimental-xqcia -target-feature -experimental-xqciac -target-feature -experimental-xqcibi -target-feature -experimental-xqcibm -target-feature -experimental-xqcicli -target-feature -experimental-xqcicm -target-feature -experimental-xqcics -target-feature -experimental-xqcicsr -target-feature -experimental-xqciint -target-feature -experimental-xqciio -target-feature -experimental-xqcilb -target-feature -experimental-xqcili -target-feature -experimental-xqcilia -target-feature -experimental-xqcilo -target-feature -experimental-xqcilsm -target-feature -experimental-xqcisim -target-feature -experimental-xqcisls -target-feature -experimental-xqcisync -target-feature -experimental-xrivosvisni -target-feature -experimental-xrivosvizip -target-feature -experimental-xsfmclic -target-feature -experimental-xsfsclic -target-feature -experimental-zalasr -target-feature -experimental-zicfilp -target-feature -experimental-zicfiss -target-feature -experimental-zvbc32e -target-feature -experimental-zvkgs -target-feature -experimental-zvqdotq -target-feature +relax -target-abi lp64d -mvscale-max=256 -mvscale-min=256 -debugger-tuning=gdb -fdebug-compilation-dir=/home/aland/gromacs/build/clang-riscv -fcoverage-compilation-dir=/home/aland/gromacs/build/clang-riscv -resource-dir /home/aland/llvm-project/build/install/lib/clang/21 -internal-isystem /home/aland/llvm-project/build/install/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc-cross/riscv64-linux-gnu/12/../../../../riscv64-linux-gnu/include -internal-externc-isystem /include -internal-externc-isystem /usr/include -O0 -ferror-limit 19 -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-35c079.o -x c test.c
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'test.c'.
4.      Running pass 'RISC-V DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->a'
 #<!-- -->0 0x0000718b71dae14f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/aland/llvm-project/llvm/lib/Support/Unix/Signals.inc:808:3
 #<!-- -->1 0x0000718b71dac054 llvm::sys::RunSignalHandlers() /home/aland/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #<!-- -->2 0x0000718b71dac3a9 SignalHandler(int, siginfo_t*, void*) /home/aland/llvm-project/llvm/lib/Support/Unix/Signals.inc:418:13
 #<!-- -->3 0x0000718b71442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000718b74fbd778 llvm::CastInfo&lt;llvm::IntegerType, llvm::Type*, void&gt;::doCastIfPossible(llvm::Type* const&amp;) /home/aland/llvm-project/llvm/include/llvm/Support/Casting.h:493:5
 #<!-- -->5 0x0000718b74fbd778 decltype(auto) llvm::dyn_cast&lt;llvm::IntegerType, llvm::Type&gt;(llvm::Type*) /home/aland/llvm-project/llvm/include/llvm/Support/Casting.h:663:48
 #<!-- -->6 0x0000718b74fbd778 llvm::EVT::getExtendedSizeInBits() const /home/aland/llvm-project/llvm/lib/CodeGen/ValueTypes.cpp:154:47
 #<!-- -->7 0x0000718b713601a8 llvm::EVT::getSizeInBits() const /home/aland/llvm-project/llvm/include/llvm/CodeGen/ValueTypes.h:371:35
 #<!-- -->8 0x0000718b713601a8 llvm::TargetLowering::findOptimalMemOpLowering(std::vector&lt;llvm::EVT, std::allocator&lt;llvm::EVT&gt;&gt;&amp;, unsigned int, llvm::MemOp const&amp;, unsigned int, unsigned int, llvm::AttributeList const&amp;) const /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/TargetLowering.cpp:242:39
 #<!-- -->9 0x0000718b712c8b80 getMemcpyLoadsAndStores(llvm::SelectionDAG&amp;, llvm::SDLoc const&amp;, llvm::SDValue, llvm::SDValue, llvm::SDValue, unsigned long, llvm::Align, bool, bool, llvm::MachinePointerInfo, llvm::MachinePointerInfo, llvm::AAMDNodes const&amp;, llvm::BatchAAResults*) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:8244:3
#<!-- -->10 0x0000718b712ca165 llvm::SelectionDAG::getMemcpy(llvm::SDValue, llvm::SDLoc const&amp;, llvm::SDValue, llvm::SDValue, llvm::SDValue, llvm::Align, bool, bool, llvm::CallInst const*, std::optional&lt;bool&gt;, llvm::MachinePointerInfo, llvm::MachinePointerInfo, llvm::AAMDNodes const&amp;, llvm::BatchAAResults*) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:8674:5
#<!-- -->11 0x0000718b712754de llvm::SelectionDAGBuilder::updateDAGForMaybeTailCall(llvm::SDValue) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:11994:3
#<!-- -->12 0x0000718b712754de llvm::SelectionDAGBuilder::visitIntrinsicCall(llvm::CallInst const&amp;, unsigned int) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:6485:30
#<!-- -->13 0x0000718b71281099 llvm::Value::getValueID() const /home/aland/llvm-project/llvm/include/llvm/IR/Value.h:531:12
#<!-- -->14 0x0000718b71281099 llvm::Instruction::getOpcode() const /home/aland/llvm-project/llvm/include/llvm/IR/Instruction.h:310:49
#<!-- -->15 0x0000718b71281099 llvm::Instruction::isTerminator() const /home/aland/llvm-project/llvm/include/llvm/IR/Instruction.h:313:50
#<!-- -->16 0x0000718b71281099 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:1341:22
#<!-- -->17 0x0000718b712f1e14 llvm::ilist_detail::node_base_prevnext&lt;llvm::ilist_node_base&lt;false, llvm::BasicBlock&gt;, false&gt;::getNext() const /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:28:38
#<!-- -->18 0x0000718b712f1e14 llvm::ilist_node_impl&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;&gt;::getNext() const /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_node.h:127:66
#<!-- -->19 0x0000718b712f1e14 llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;::operator++() /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:345:25
#<!-- -->20 0x0000718b712f1e14 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:862:79
#<!-- -->21 0x0000718b712f2afe llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:1906:33
#<!-- -->22 0x0000718b712f42d3 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:615:22
#<!-- -->23 0x0000718b712ec69b llvm::OptLevelChanger::~OptLevelChanger() /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:268:11
#<!-- -->24 0x0000718b712ec69b llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:375:1
#<!-- -->25 0x0000718b712ec69b llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) /home/aland/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAGISel.cpp:346:6
#<!-- -->26 0x0000718b74d28902 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) /home/aland/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#<!-- -->27 0x0000718b72214fd5 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/aland/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1416:7
#<!-- -->28 0x0000718b7221520b llvm::ilist_detail::node_base_prevnext&lt;llvm::ilist_node_base&lt;false, void&gt;, false&gt;::getNext() const /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:28:38
#<!-- -->29 0x0000718b7221520b llvm::ilist_node_impl&lt;llvm::ilist_detail::node_options&lt;llvm::Function, false, false, void, false, void&gt;&gt;::getNext() /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_node.h:119:66
#<!-- -->30 0x0000718b7221520b llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::Function, false, false, void, false, void&gt;, false, false&gt;::operator++() /home/aland/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#<!-- -->31 0x0000718b7221520b llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/aland/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1451:22
#<!-- -->32 0x0000718b72215b7c runOnModule /home/aland/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1528:7
#<!-- -->33 0x0000718b72215b7c llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/aland/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:539:55
#<!-- -->34 0x0000718b75330e5e RunCodegenPipeline /home/aland/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1263:9
#<!-- -->35 0x0000718b75330e5e emitAssembly /home/aland/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1286:21
#<!-- -->36 0x0000718b75330e5e clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) /home/aland/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1451:25
#<!-- -->37 0x0000718b7566e817 llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::release() /home/aland/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#<!-- -->38 0x0000718b7566e817 llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::~IntrusiveRefCntPtr() /home/aland/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#<!-- -->39 0x0000718b7566e817 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) /home/aland/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:316:20
#<!-- -->40 0x0000718b701409fc clang::ParseAST(clang::Sema&amp;, bool, bool) /home/aland/llvm-project/clang/lib/Parse/ParseAST.cpp:184:34
#<!-- -->41 0x0000718b7412d5be clang::FrontendAction::Execute() /home/aland/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1229:38
#<!-- -->42 0x0000718b740b2a6b llvm::Error::getPtr() const /home/aland/llvm-project/llvm/include/llvm/Support/Error.h:281:12
#<!-- -->43 0x0000718b740b2a6b llvm::Error::operator bool() /home/aland/llvm-project/llvm/include/llvm/Support/Error.h:241:22
#<!-- -->44 0x0000718b740b2a6b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/aland/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:42
#<!-- -->45 0x0000718b78a352e8 std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/11/bits/shared_ptr_base.h:1296:16
#<!-- -->46 0x0000718b78a352e8 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/11/bits/shared_ptr_base.h:993:69
#<!-- -->47 0x0000718b78a352e8 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/11/bits/shared_ptr_base.h:980:16
#<!-- -->48 0x0000718b78a352e8 clang::CompilerInvocation::getFrontendOpts() /home/aland/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:259:48
#<!-- -->49 0x0000718b78a352e8 clang::CompilerInstance::getFrontendOpts() /home/aland/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:304:39
#<!-- -->50 0x0000718b78a352e8 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/aland/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:301:29
#<!-- -->51 0x00005b35205c9acf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /home/aland/llvm-project/clang/tools/driver/cc1_main.cpp:297:40
#<!-- -->52 0x00005b35205c2779 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /home/aland/llvm-project/clang/tools/driver/driver.cpp:223:20
#<!-- -->53 0x00005b35205c505d clang_main(int, char**, llvm::ToolContext const&amp;) /home/aland/llvm-project/clang/tools/driver/driver.cpp:264:26
#<!-- -->54 0x00005b35205c1d8b main /home/aland/llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:18:1
#<!-- -->55 0x0000718b71429d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->56 0x0000718b71429e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->57 0x0000718b71429e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->58 0x00005b35205c1de5 _start (/home/aland/llvm-project/build/install/bin/clang-21+0xcde5)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git (https://github.com/llvm/llvm-project.git 9732427f377ec076601d4d6ede697f39a4409ee4)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/aland/llvm-project/build/install/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-3d5f2f.c
clang: note: diagnostic msg: /tmp/test-3d5f2f.sh
clang: note: diagnostic msg: 

********************
```

[test-3d5f2f.c.gz](https://github.com/user-attachments/files/20103243/test-3d5f2f.c.gz)
[test-3d5f2f.sh.gz](https://github.com/user-attachments/files/20103242/test-3d5f2f.sh.gz)

Looks like `getOptimalMemOpType` returns `{V = {SimpleTy = llvm::MVT::INVALID_SIMPLE_VALUE_TYPE}, LLVMTy = 0x0}`, and it goes downhill from there.
</details>


---

### Comment 2 - al42and

Thank you for the quick fix, @topperc 

---

