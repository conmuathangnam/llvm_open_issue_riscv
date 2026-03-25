# [Hexagon] `Assertion 'StageScheduled != -1 && "Expecting scheduled instruction."' failed.` when building `APFloat.cpp`

**Author:** alexrp
**URL:** https://github.com/llvm/llvm-project/issues/161050
**Status:** Closed
**Labels:** llvm:codegen, release:backport, crash-on-valid
**Closed Date:** 2025-12-04T20:18:42Z

## Body

Repro files:

* https://files.alexrp.com/APFloat-f0c57c.cpp
* https://files.alexrp.com/APFloat-f0c57c.sh

```
clang: /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:647: void llvm::ModuloScheduleExpander::generatePhis(llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, ValueMapTy*, ValueMapTy*, InstrMapTy&, unsigned int, unsigned int, bool): Assertion `StageScheduled != -1 && "Expecting scheduled instruction."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple hexagon-unknown-linux5.10.0-musl -O2 -emit-obj -disable-free -clear-ast-before-backend -main-file-name APFloat.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=1 -target-cpu hexagonv68 -target-feature -long-calls -mqdsp6-compat -Wreturn-type -mllvm -hexagon-small-data-threshold=0 -fshort-enums -mllvm -machine-sink-split=0 -debugger-tuning=gdb -fdebug-compilation-dir=/home/alexrp/Source/ziglang/zig-bootstrap/out/build-llvm-hexagon-linux-musl-baseline -ffunction-sections -fdata-sections -fcoverage-compilation-dir=/home/alexrp/Source/ziglang/zig-bootstrap/out/build-llvm-hexagon-linux-musl-baseline -nostdsysteminc -nobuiltininc -sys-header-deps -D NDEBUG -D _LIBCPP_ABI_VERSION=1 -D _LIBCPP_ABI_NAMESPACE=__1 -D _LIBCPP_HAS_THREADS -D _LIBCPP_HAS_MONOTONIC_CLOCK -D _LIBCPP_HAS_TERMINAL -D _LIBCPP_HAS_MUSL_LIBC -D _LIBCXXABI_DISABLE_VISIBILITY_ANNOTATIONS -D _LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D _LIBCPP_HAS_NO_VENDOR_AVAILABILITY_ANNOTATIONS -D _LIBCPP_HAS_FILESYSTEM -D _LIBCPP_HAS_RANDOM_DEVICE -D _LIBCPP_HAS_LOCALIZATION -D _LIBCPP_HAS_UNICODE -D _LIBCPP_HAS_WIDE_CHARACTERS -D _LIBCPP_HAS_NO_STD_MODULES -D _LIBCPP_HAS_TIME_ZONE_DATABASE -D _LIBCPP_PSTL_BACKEND_SERIAL -D _LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_NONE -D _LIBCPP_ENABLE_CXX17_REMOVED_UNEXPECTED_FUNCTIONS -D EXPERIMENTAL_KEY_INSTRUCTIONS -D GTEST_HAS_RTTI=0 -D _FILE_OFFSET_BITS=64 -D _LARGEFILE_SOURCE -D __STDC_CONSTANT_MACROS -D __STDC_FORMAT_MACROS -D __STDC_LIMIT_MACROS -D NDEBUG -Werror=date-time -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wmisleading-indentation -Wctad-maybe-unsupported -pedantic -std=c++17 -fdeprecated-macro -ferror-limit 19 -fvisibility-inlines-hidden -fno-rtti -fshort-enums -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -fno-spell-checking -vectorize-loops -vectorize-slp -target-cpu hexagonv68 -target-feature -audio -target-feature +cabac -target-feature +compound -target-feature +duplex -target-feature -hvx -target-feature -hvx-ieee-fp -target-feature -hvx-length128b -target-feature -hvx-length64b -target-feature -hvx-qfloat -target-feature -hvxv60 -target-feature -hvxv62 -target-feature -hvxv65 -target-feature -hvxv66 -target-feature -hvxv67 -target-feature -hvxv68 -target-feature -hvxv69 -target-feature -hvxv71 -target-feature -hvxv73 -target-feature -hvxv75 -target-feature -hvxv79 -target-feature -long-calls -target-feature +mem_noshuf -target-feature +memops -target-feature -noreturn-stack-elim -target-feature +nvj -target-feature +nvs -target-feature +packets -target-feature -prev65 -target-feature -reserved-r19 -target-feature -small-data -target-feature -tinycore -target-feature -unsafe-fp -target-feature +v5 -target-feature +v55 -target-feature +v60 -target-feature +v62 -target-feature +v65 -target-feature +v66 -target-feature +v67 -target-feature +v68 -target-feature -v69 -target-feature -v71 -target-feature -v73 -target-feature -v75 -target-feature -v79 -target-feature -zreg -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ APFloat-f0c57c.cpp
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'APFloat-f0c57c.cpp'.
4.      Running pass 'Modulo Software Pipelining' on function '@_ZNK4llvm6detail9IEEEFloat24convertNormalToHexStringEPcjbNS_12RoundingModeE'
 #0 0x0000743a46f91af0 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/alexrp/Source/llvm-project/llvm/lib/Support/Unix/Signals.inc:838:3
 #1 0x0000743a46f8e9b4 llvm::sys::RunSignalHandlers() /home/alexrp/Source/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #2 0x0000743a46f8f1e9 SignalHandler(int, siginfo_t*, void*) /home/alexrp/Source/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #3 0x0000743a458458d0 (/lib/x86_64-linux-gnu/libc.so.6+0x458d0)
 #4 0x0000743a458a49bc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x0000743a458a49bc __pthread_kill_internal ./nptl/pthread_kill.c:89:10
 #6 0x0000743a458a49bc pthread_kill ./nptl/pthread_kill.c:100:10
 #7 0x0000743a4584579e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x0000743a458288cd internal_signal_block_all ./stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
 #9 0x0000743a458288cd __abort_lock_wrlock ./stdlib/abort.c:58:3
#10 0x0000743a458288cd abort ./stdlib/abort.c:81:3
#11 0x0000743a45828830 __assert_perror_fail ./assert/assert-perr.c:31:1
#12 0x0000743a4690f60c llvm::MachineOperand::isDef() const /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:647:7
#13 0x0000743a4690f60c llvm::MachineRegisterInfo::defusechain_iterator<true, false, false, true, false>::defusechain_iterator(llvm::MachineOperand*) /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:1064:38
#14 0x0000743a4690f60c llvm::MachineRegisterInfo::use_begin(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:468:52
#15 0x0000743a4690f60c llvm::MachineRegisterInfo::use_operands(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:473:22
#16 0x0000743a4690f60c hasUseAfterLoop /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:361:51
#17 0x0000743a4690f60c llvm::ModuloScheduleExpander::generatePhis(llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::DenseMap<llvm::Register, llvm::Register, llvm::DenseMapInfo<llvm::Register, void>, llvm::detail::DenseMapPair<llvm::Register, llvm::Register>>*, llvm::DenseMap<llvm::Register, llvm::Register, llvm::DenseMapInfo<llvm::Register, void>, llvm::detail::DenseMapPair<llvm::Register, llvm::Register>>*, llvm::DenseMap<llvm::MachineInstr*, llvm::MachineInstr*, llvm::DenseMapInfo<llvm::MachineInstr*, void>, llvm::detail::DenseMapPair<llvm::MachineInstr*, llvm::MachineInstr*>>&, unsigned int, unsigned int, bool) (.cold) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:654:26
#18 0x0000743a4762c2fa llvm::ModuloScheduleExpander::generatePipelinedLoop() /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:164:15
#19 0x0000743a4756867d llvm::SwingSchedulerDAG::schedule() /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:864:16
#20 0x0000743a4756a9b8 llvm::MachinePipeliner::swingModuloScheduler(llvm::MachineLoop&) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:610:17
#21 0x0000743a4756af2b llvm::MachinePipeliner::scheduleLoop(llvm::MachineLoop&) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:428:25
#22 0x0000743a4756ae3e llvm::MachinePipeliner::scheduleLoop(llvm::MachineLoop&) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:399:13
#23 0x0000743a4756b200 llvm::MachinePipeliner::runOnMachineFunction(llvm::MachineFunction&) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:386:25
#24 0x0000743a4756b200 llvm::MachinePipeliner::runOnMachineFunction(llvm::MachineFunction&) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:358:6
#25 0x0000743a474d947b llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#26 0x0000743a4714cd34 llvm::FPPassManager::runOnFunction(llvm::Function&) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1408:7
#27 0x0000743a4714cf45 llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<true, void>, true>::getNext() const /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:42:38
#28 0x0000743a4714cf45 llvm::ilist_node_impl<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>>::getNext() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_node.h:119:66
#29 0x0000743a4714cf45 llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::Function, true, false, void, false, void>, false, false>::operator++() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#30 0x0000743a4714cf45 llvm::FPPassManager::runOnModule(llvm::Module&) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1443:22
#31 0x0000743a4714d75b runOnModule /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1520:7
#32 0x0000743a4714d75b llvm::legacy::PassManagerImpl::run(llvm::Module&) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:55
#33 0x0000743a5300f41e RunCodegenPipeline /home/alexrp/Source/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1261:9
#34 0x0000743a5300f41e emitAssembly /home/alexrp/Source/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1284:21
#35 0x0000743a5300f41e clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /home/alexrp/Source/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1449:25
#36 0x0000743a53399f6b llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::release() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#37 0x0000743a53399f6b llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::~IntrusiveRefCntPtr() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#38 0x0000743a53399f6b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /home/alexrp/Source/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:316:20
#39 0x0000743a51b4c32c clang::ParseAST(clang::Sema&, bool, bool) /home/alexrp/Source/llvm-project/clang/lib/Parse/ParseAST.cpp:183:34
#40 0x0000743a53cbdd25 clang::FrontendAction::Execute() /home/alexrp/Source/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1225:38
#41 0x0000743a53c4bf32 llvm::Error::getPtr() const /home/alexrp/Source/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#42 0x0000743a53c4bf32 llvm::Error::operator bool() /home/alexrp/Source/llvm-project/llvm/include/llvm/Support/Error.h:241:22
#43 0x0000743a53c4bf32 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/alexrp/Source/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1055:42
#44 0x0000743a53d4edfb std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/14/bits/shared_ptr_base.h:1667:16
#45 0x0000743a53d4edfb std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/14/bits/shared_ptr_base.h:1364:69
#46 0x0000743a53d4edfb std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/14/bits/shared_ptr_base.h:1350:2
#47 0x0000743a53d4edfb clang::CompilerInvocation::getFrontendOpts() /home/alexrp/Source/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:259:48
#48 0x0000743a53d4edfb clang::CompilerInstance::getFrontendOpts() /home/alexrp/Source/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:304:39
#49 0x0000743a53d4edfb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/alexrp/Source/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:300:29
#50 0x00005da7d48360eb cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/alexrp/Source/llvm-project/clang/tools/driver/cc1_main.cpp:297:40
#51 0x00005da7d482d5d8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /home/alexrp/Source/llvm-project/clang/tools/driver/driver.cpp:223:20
#52 0x00005da7d4830388 clang_main(int, char**, llvm::ToolContext const&) /home/alexrp/Source/llvm-project/clang/tools/driver/driver.cpp:264:26
#53 0x00005da7d482c888 main /home/alexrp/Source/llvm-project/build-21/tools/clang/tools/driver/clang-driver.cpp:18:1
#54 0x0000743a4582a578 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#55 0x0000743a4582a63b call_init ./csu/../csu/libc-start.c:128:20
#56 0x0000743a4582a63b __libc_start_main ./csu/../csu/libc-start.c:347:5
#57 0x00005da7d482c8e5 _start (/opt/llvm-21/bin/clang-21+0xf8e5)
Aborted (core dumped)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Alex Rønne Petersen (alexrp)

<details>
Repro files:

* https://files.alexrp.com/APFloat-f0c57c.cpp
* https://files.alexrp.com/APFloat-f0c57c.sh

```
clang: /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:647: void llvm::ModuloScheduleExpander::generatePhis(llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, ValueMapTy*, ValueMapTy*, InstrMapTy&amp;, unsigned int, unsigned int, bool): Assertion `StageScheduled != -1 &amp;&amp; "Expecting scheduled instruction."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang -cc1 -triple hexagon-unknown-linux5.10.0-musl -O2 -emit-obj -disable-free -clear-ast-before-backend -main-file-name APFloat.cpp -mrelocation-model pic -pic-level 2 -fhalf-no-semantic-interposition -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=1 -target-cpu hexagonv68 -target-feature -long-calls -mqdsp6-compat -Wreturn-type -mllvm -hexagon-small-data-threshold=0 -fshort-enums -mllvm -machine-sink-split=0 -debugger-tuning=gdb -fdebug-compilation-dir=/home/alexrp/Source/ziglang/zig-bootstrap/out/build-llvm-hexagon-linux-musl-baseline -ffunction-sections -fdata-sections -fcoverage-compilation-dir=/home/alexrp/Source/ziglang/zig-bootstrap/out/build-llvm-hexagon-linux-musl-baseline -nostdsysteminc -nobuiltininc -sys-header-deps -D NDEBUG -D _LIBCPP_ABI_VERSION=1 -D _LIBCPP_ABI_NAMESPACE=__1 -D _LIBCPP_HAS_THREADS -D _LIBCPP_HAS_MONOTONIC_CLOCK -D _LIBCPP_HAS_TERMINAL -D _LIBCPP_HAS_MUSL_LIBC -D _LIBCXXABI_DISABLE_VISIBILITY_ANNOTATIONS -D _LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS -D _LIBCPP_HAS_NO_VENDOR_AVAILABILITY_ANNOTATIONS -D _LIBCPP_HAS_FILESYSTEM -D _LIBCPP_HAS_RANDOM_DEVICE -D _LIBCPP_HAS_LOCALIZATION -D _LIBCPP_HAS_UNICODE -D _LIBCPP_HAS_WIDE_CHARACTERS -D _LIBCPP_HAS_NO_STD_MODULES -D _LIBCPP_HAS_TIME_ZONE_DATABASE -D _LIBCPP_PSTL_BACKEND_SERIAL -D _LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_NONE -D _LIBCPP_ENABLE_CXX17_REMOVED_UNEXPECTED_FUNCTIONS -D EXPERIMENTAL_KEY_INSTRUCTIONS -D GTEST_HAS_RTTI=0 -D _FILE_OFFSET_BITS=64 -D _LARGEFILE_SOURCE -D __STDC_CONSTANT_MACROS -D __STDC_FORMAT_MACROS -D __STDC_LIMIT_MACROS -D NDEBUG -Werror=date-time -Werror=date-time -Werror=unguarded-availability-new -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wmissing-field-initializers -Wno-long-long -Wc++98-compat-extra-semi -Wimplicit-fallthrough -Wcovered-switch-default -Wno-noexcept-type -Wnon-virtual-dtor -Wdelete-non-virtual-dtor -Wsuggest-override -Wstring-conversion -Wmisleading-indentation -Wctad-maybe-unsupported -pedantic -std=c++17 -fdeprecated-macro -ferror-limit 19 -fvisibility-inlines-hidden -fno-rtti -fshort-enums -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -fno-spell-checking -vectorize-loops -vectorize-slp -target-cpu hexagonv68 -target-feature -audio -target-feature +cabac -target-feature +compound -target-feature +duplex -target-feature -hvx -target-feature -hvx-ieee-fp -target-feature -hvx-length128b -target-feature -hvx-length64b -target-feature -hvx-qfloat -target-feature -hvxv60 -target-feature -hvxv62 -target-feature -hvxv65 -target-feature -hvxv66 -target-feature -hvxv67 -target-feature -hvxv68 -target-feature -hvxv69 -target-feature -hvxv71 -target-feature -hvxv73 -target-feature -hvxv75 -target-feature -hvxv79 -target-feature -long-calls -target-feature +mem_noshuf -target-feature +memops -target-feature -noreturn-stack-elim -target-feature +nvj -target-feature +nvs -target-feature +packets -target-feature -prev65 -target-feature -reserved-r19 -target-feature -small-data -target-feature -tinycore -target-feature -unsafe-fp -target-feature +v5 -target-feature +v55 -target-feature +v60 -target-feature +v62 -target-feature +v65 -target-feature +v66 -target-feature +v67 -target-feature +v68 -target-feature -v69 -target-feature -v71 -target-feature -v73 -target-feature -v75 -target-feature -v79 -target-feature -zreg -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c++ APFloat-f0c57c.cpp
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'APFloat-f0c57c.cpp'.
4.      Running pass 'Modulo Software Pipelining' on function '@<!-- -->_ZNK4llvm6detail9IEEEFloat24convertNormalToHexStringEPcjbNS_12RoundingModeE'
 #<!-- -->0 0x0000743a46f91af0 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/alexrp/Source/llvm-project/llvm/lib/Support/Unix/Signals.inc:838:3
 #<!-- -->1 0x0000743a46f8e9b4 llvm::sys::RunSignalHandlers() /home/alexrp/Source/llvm-project/llvm/lib/Support/Signals.cpp:104:20
 #<!-- -->2 0x0000743a46f8f1e9 SignalHandler(int, siginfo_t*, void*) /home/alexrp/Source/llvm-project/llvm/lib/Support/Unix/Signals.inc:426:14
 #<!-- -->3 0x0000743a458458d0 (/lib/x86_64-linux-gnu/libc.so.6+0x458d0)
 #<!-- -->4 0x0000743a458a49bc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->5 0x0000743a458a49bc __pthread_kill_internal ./nptl/pthread_kill.c:89:10
 #<!-- -->6 0x0000743a458a49bc pthread_kill ./nptl/pthread_kill.c:100:10
 #<!-- -->7 0x0000743a4584579e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->8 0x0000743a458288cd internal_signal_block_all ./stdlib/../sysdeps/unix/sysv/linux/internal-signals.h:81:3
 #<!-- -->9 0x0000743a458288cd __abort_lock_wrlock ./stdlib/abort.c:58:3
#<!-- -->10 0x0000743a458288cd abort ./stdlib/abort.c:81:3
#<!-- -->11 0x0000743a45828830 __assert_perror_fail ./assert/assert-perr.c:31:1
#<!-- -->12 0x0000743a4690f60c llvm::MachineOperand::isDef() const /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:647:7
#<!-- -->13 0x0000743a4690f60c llvm::MachineRegisterInfo::defusechain_iterator&lt;true, false, false, true, false&gt;::defusechain_iterator(llvm::MachineOperand*) /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:1064:38
#<!-- -->14 0x0000743a4690f60c llvm::MachineRegisterInfo::use_begin(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:468:52
#<!-- -->15 0x0000743a4690f60c llvm::MachineRegisterInfo::use_operands(llvm::Register) const /home/alexrp/Source/llvm-project/llvm/include/llvm/CodeGen/MachineRegisterInfo.h:473:22
#<!-- -->16 0x0000743a4690f60c hasUseAfterLoop /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:361:51
#<!-- -->17 0x0000743a4690f60c llvm::ModuloScheduleExpander::generatePhis(llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::MachineBasicBlock*, llvm::DenseMap&lt;llvm::Register, llvm::Register, llvm::DenseMapInfo&lt;llvm::Register, void&gt;, llvm::detail::DenseMapPair&lt;llvm::Register, llvm::Register&gt;&gt;*, llvm::DenseMap&lt;llvm::Register, llvm::Register, llvm::DenseMapInfo&lt;llvm::Register, void&gt;, llvm::detail::DenseMapPair&lt;llvm::Register, llvm::Register&gt;&gt;*, llvm::DenseMap&lt;llvm::MachineInstr*, llvm::MachineInstr*, llvm::DenseMapInfo&lt;llvm::MachineInstr*, void&gt;, llvm::detail::DenseMapPair&lt;llvm::MachineInstr*, llvm::MachineInstr*&gt;&gt;&amp;, unsigned int, unsigned int, bool) (.cold) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:654:26
#<!-- -->18 0x0000743a4762c2fa llvm::ModuloScheduleExpander::generatePipelinedLoop() /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:164:15
#<!-- -->19 0x0000743a4756867d llvm::SwingSchedulerDAG::schedule() /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:864:16
#<!-- -->20 0x0000743a4756a9b8 llvm::MachinePipeliner::swingModuloScheduler(llvm::MachineLoop&amp;) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:610:17
#<!-- -->21 0x0000743a4756af2b llvm::MachinePipeliner::scheduleLoop(llvm::MachineLoop&amp;) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:428:25
#<!-- -->22 0x0000743a4756ae3e llvm::MachinePipeliner::scheduleLoop(llvm::MachineLoop&amp;) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:399:13
#<!-- -->23 0x0000743a4756b200 llvm::MachinePipeliner::runOnMachineFunction(llvm::MachineFunction&amp;) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:386:25
#<!-- -->24 0x0000743a4756b200 llvm::MachinePipeliner::runOnMachineFunction(llvm::MachineFunction&amp;) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachinePipeliner.cpp:358:6
#<!-- -->25 0x0000743a474d947b llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) /home/alexrp/Source/llvm-project/llvm/lib/CodeGen/MachineFunctionPass.cpp:108:30
#<!-- -->26 0x0000743a4714cd34 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1408:7
#<!-- -->27 0x0000743a4714cf45 llvm::ilist_detail::node_base_prevnext&lt;llvm::ilist_node_base&lt;true, void&gt;, true&gt;::getNext() const /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_node_base.h:42:38
#<!-- -->28 0x0000743a4714cf45 llvm::ilist_node_impl&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;&gt;::getNext() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_node.h:119:66
#<!-- -->29 0x0000743a4714cf45 llvm::ilist_iterator&lt;llvm::ilist_detail::node_options&lt;llvm::Function, true, false, void, false, void&gt;, false, false&gt;::operator++() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/ilist_iterator.h:187:25
#<!-- -->30 0x0000743a4714cf45 llvm::FPPassManager::runOnModule(llvm::Module&amp;) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1443:22
#<!-- -->31 0x0000743a4714d75b runOnModule /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:1520:7
#<!-- -->32 0x0000743a4714d75b llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) /home/alexrp/Source/llvm-project/llvm/lib/IR/LegacyPassManager.cpp:531:55
#<!-- -->33 0x0000743a5300f41e RunCodegenPipeline /home/alexrp/Source/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1261:9
#<!-- -->34 0x0000743a5300f41e emitAssembly /home/alexrp/Source/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1284:21
#<!-- -->35 0x0000743a5300f41e clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) /home/alexrp/Source/llvm-project/clang/lib/CodeGen/BackendUtil.cpp:1449:25
#<!-- -->36 0x0000743a53399f6b llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::release() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#<!-- -->37 0x0000743a53399f6b llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::~IntrusiveRefCntPtr() /home/alexrp/Source/llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#<!-- -->38 0x0000743a53399f6b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) /home/alexrp/Source/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:316:20
#<!-- -->39 0x0000743a51b4c32c clang::ParseAST(clang::Sema&amp;, bool, bool) /home/alexrp/Source/llvm-project/clang/lib/Parse/ParseAST.cpp:183:34
#<!-- -->40 0x0000743a53cbdd25 clang::FrontendAction::Execute() /home/alexrp/Source/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1225:38
#<!-- -->41 0x0000743a53c4bf32 llvm::Error::getPtr() const /home/alexrp/Source/llvm-project/llvm/include/llvm/Support/Error.h:278:51
#<!-- -->42 0x0000743a53c4bf32 llvm::Error::operator bool() /home/alexrp/Source/llvm-project/llvm/include/llvm/Support/Error.h:241:22
#<!-- -->43 0x0000743a53c4bf32 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /home/alexrp/Source/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1055:42
#<!-- -->44 0x0000743a53d4edfb std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/14/bits/shared_ptr_base.h:1667:16
#<!-- -->45 0x0000743a53d4edfb std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/14/bits/shared_ptr_base.h:1364:69
#<!-- -->46 0x0000743a53d4edfb std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/14/bits/shared_ptr_base.h:1350:2
#<!-- -->47 0x0000743a53d4edfb clang::CompilerInvocation::getFrontendOpts() /home/alexrp/Source/llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:259:48
#<!-- -->48 0x0000743a53d4edfb clang::CompilerInstance::getFrontendOpts() /home/alexrp/Source/llvm-project/clang/include/clang/Frontend/CompilerInstance.h:304:39
#<!-- -->49 0x0000743a53d4edfb clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/alexrp/Source/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:300:29
#<!-- -->50 0x00005da7d48360eb cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /home/alexrp/Source/llvm-project/clang/tools/driver/cc1_main.cpp:297:40
#<!-- -->51 0x00005da7d482d5d8 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /home/alexrp/Source/llvm-project/clang/tools/driver/driver.cpp:223:20
#<!-- -->52 0x00005da7d4830388 clang_main(int, char**, llvm::ToolContext const&amp;) /home/alexrp/Source/llvm-project/clang/tools/driver/driver.cpp:264:26
#<!-- -->53 0x00005da7d482c888 main /home/alexrp/Source/llvm-project/build-21/tools/clang/tools/driver/clang-driver.cpp:18:1
#<!-- -->54 0x0000743a4582a578 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->55 0x0000743a4582a63b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->56 0x0000743a4582a63b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->57 0x00005da7d482c8e5 _start (/opt/llvm-21/bin/clang-21+0xf8e5)
Aborted (core dumped)
```
</details>


---

### Comment 2 - androm3da

Reduced test case, @aankit-ca 

```
$ cat reduced.ll 
target datalayout = "e-m:e-p:32:32:32-a:0-n16:32-i64:64:64-i32:32:32-i16:16:16-i1:8:8-f32:32:32-f64:64:64-v32:32:32-v64:64:64-v512:512:512-v1024:1024:1024-v2048:2048:2048"
target triple = "hexagon-unknown-linux-musl"

define ptr @_ZNK9IEEEFloat24convertNormalToHexStringEv(i64 %shr.7) {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.body4, %entry
  %0 = phi ptr [ null, %entry ], [ inttoptr (i32 -1 to ptr), %while.body4 ]
  br label %while.body4

while.body4:                                      ; preds = %while.body4, %while.cond
  %dec11 = phi i32 [ 31, %while.cond ], [ %dec.7, %while.body4 ]
  %part.110 = phi i64 [ %shr.7, %while.cond ], [ %shr.72, %while.body4 ]
  %dec.6 = add i32 %dec11, 1
  %1 = trunc i64 %part.110 to i32
  %idxprom.7 = and i32 %1, 1
  %arrayidx.7 = getelementptr [18 x i8], ptr null, i32 0, i32 %idxprom.7
  %2 = load i8, ptr %arrayidx.7, align 1
  %arrayidx5.7 = getelementptr i8, ptr %0, i32 %dec.6
  store i8 %2, ptr %arrayidx5.7, align 1
  %shr.72 = lshr i64 %part.110, 32
  %dec.7 = add i32 %dec11, -8
  %tobool3.not.7 = icmp eq i32 %dec.6, 0
  br i1 %tobool3.not.7, label %while.cond, label %while.body4
}

$ ./obj_llvm/bin/llc -mcpu=hexagonv68 --relocation-model=pic -mtriple=hexagon-unknown-linux-musl -O2 reduced.ll
llc: /home/brian/src/toolchain_for_hexagon/llvm-project/llvm/lib/CodeGen/ModuloSchedule.cpp:647: void llvm::ModuloScheduleExpander::generatePhis(llvm::MachineBasicBlock *, llvm::MachineBasicBlock *, llvm::MachineBasicBlock *, llvm::MachineBasicBlock *, llvm::ModuloScheduleExpander::ValueMapTy *, llvm::ModuloScheduleExpander::ValueMapTy *, llvm::ModuloScheduleExpander::InstrMapTy &, unsigned int, unsigned int, bool): Assertion `StageScheduled != -1 && "Expecting scheduled instruction."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.

```

---

### Comment 3 - iajbar

Thanks Brian!


---

### Comment 4 - androm3da

@alexrp are there any more assertions lurking behind this one?  Can the zig bootstrap do the equivalent of `--keep-going`?

---

### Comment 5 - alexrp

It should be able to, yes. I'll try later today and report back.

---

### Comment 6 - alexrp

One other crash:

* https://files.alexrp.com/AArch64MCCodeEmitter-02318c.cpp
* https://files.alexrp.com/AArch64MCCodeEmitter-02318c.sh

Unfortunately I forgot to actually enable assertions again... will post a stack trace shortly.

---

### Comment 7 - alexrp

Here's the output I see: https://gist.github.com/alexrp/d2578de4cbc7c0129f15e7b7959272ee

---

### Comment 8 - androm3da

> Here's the output I see: https://gist.github.com/alexrp/d2578de4cbc7c0129f15e7b7959272ee

Thanks!  I'm reducing this one now - it's taking quite some time.  I'll open a new issue to track this once it's done.

---

### Comment 9 - androm3da

> > Here's the output I see: https://gist.github.com/alexrp/d2578de4cbc7c0129f15e7b7959272ee
> 
> Thanks! I'm reducing this one now - it's taking quite some time. I'll open a new issue to track this once it's done.

Opened as #163774

---

### Comment 10 - quic-asaravan

The assertion hits in the generatePhis() function (Modulo Schedule). The dependencies between phis are incorrectly updated in  SwingSchedulerDAG::updatePhiDependences(). [135925](https://github.com/llvm/llvm-project/pull/135925) fixes this. 

---

### Comment 11 - dyung

Hi, we are getting close to the end of the LLVM 21.x release cycle, so if we would like to submit a fix for this issue, it would need to be soon.

---

### Comment 12 - iajbar

The fix is merged https://github.com/llvm/llvm-project/pull/135925

---

### Comment 13 - androm3da

If it's not too late to catch the last 21.x release let's please cherry pick the fix?

---

### Comment 14 - alexrp

/cherry-pick 78ee4a5

---

### Comment 15 - llvmbot

/pull-request llvm/llvm-project#170749

---

