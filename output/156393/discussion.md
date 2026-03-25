# [RISC-V, clang++ 20.1.8 / 21.1.1] error in backend: Incomplete scavenging after 2nd pass

**Author:** Tang-Haojin
**URL:** https://github.com/llvm/llvm-project/issues/156393

## Body

Hi, I am trying to compile a cpp project using riscv64-unknown-elf-clang++ from [riscv-gnu-toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain/releases/download/2025.08.29/riscv64-elf-ubuntu-22.04-llvm-nightly-2025.08.29-nightly.tar.xz), but I am encountering an error. The backtrace is as follows. If there is anything else I can do, please let me know. Thank you!

FYI, This error does not occur with `-O0` and occurs with `-O1`, `-O2` or `-O3`.

On llvm 20.1.8:
```
fatal error: error in backend: Incomplete scavenging after 2nd pass
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang++ -o build/applications/SimTop260.o -c -mcmodel=medany -march=rv64imac -mabi=lp64 -flax-vector-conversions -Wno-cpp -fno-common -ffunction-sections -fdata-sections -fdiagnostics-color=always -Xclang -fexperimental-max-bitint-width=20000 -fbracket-depth=2048 -Wno-parentheses-equality -O1 -mcmodel=medany -march=rv64imac -mabi=lp64 -flax-vector-conversions -Wno-cpp -fno-common -ffunction-sections -fdata-sections -fdiagnostics-color=always -Xclang -fexperimental-max-bitint-width=20000 -fbracket-depth=2048 -Wno-parentheses-equality -O1 -D__RTTHREAD__ -DRT_USING_NEWLIBC -DRT_USING_LIBC -D_POSIX_C_SOURCE=1 -I. -Iapplications -Iapplications/test -Iapplications/test/test_vector -Idriver -I/nfs/home/tanghaojin/rt-thread-2/include -I/nfs/home/tanghaojin/rt-thread-2/libcpu/risc-v/common64 -I/nfs/home/tanghaojin/rt-thread-2/libcpu/risc-v/virt64 -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/lwip-2.0.3/src/include -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/lwip-2.0.3/src/include/ipv4 -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/lwip-2.0.3/src/include/netif -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/port -I/nfs/home/tanghaojin/rt-thread-2/components/net/netdev/include -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include/socket -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/impl -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include/dfs_net -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include/socket/sys_socket -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/resource -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/utest -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/bitmap -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/ref -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/avl -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/uthash -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/hashmap -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/include -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/filesystems/romfs -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/filesystems/elmfat -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/filesystems/devfs -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/phy -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/include -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/virtio -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/smp_call -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/ktime -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/ktime/inc -I/nfs/home/tanghaojin/rt-thread-2/components/libc/cplusplus -I/nfs/home/tanghaojin/rt-thread-2/components/libc/cplusplus/os -I/nfs/home/tanghaojin/rt-thread-2/components/libc/compilers/common/include -I/nfs/home/tanghaojin/rt-thread-2/components/libc/compilers/newlib -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/ipc -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/mman -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/poll -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/termios -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/aio -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/epoll -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/eventfd -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/stdio -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/delay -I/nfs/home/tanghaojin/rt-thread-2/components/mm -I/nfs/home/tanghaojin/rt-thread-2/components/finsh applications/SimTop260.cpp
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'applications/SimTop260.cpp'.
4.      Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@_ZN7SSimTop10subStep258Ev'
 #0 0x000055cec9913650 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x1e8d650)
 #1 0x000055cec9911534 llvm::sys::CleanupOnSignal(unsigned long) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x1e8b534)
 #2 0x000055cec98633c7 llvm::CrashRecoveryContext::HandleExit(int) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x1ddd3c7)
 #3 0x000055cec9908832 llvm::sys::Process::Exit(int, bool) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x1e82832)
 #4 0x000055cec8862bcf LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x000055cec986bb50 llvm::report_fatal_error(llvm::Twine const&, bool) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x1de5b50)
 #6 0x000055cec986bcde (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x1de5cde)
 #7 0x000055cec8f1c117 llvm::scavengeFrameVirtualRegs(llvm::MachineFunction&, llvm::RegScavenger&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x1496117)
 #8 0x000055cec91a5e52 (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
 #9 0x000055cec8de5dc9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#10 0x000055cec93a1f72 llvm::FPPassManager::runOnFunction(llvm::Function&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x191bf72)
#11 0x000055cec93a2134 llvm::FPPassManager::runOnModule(llvm::Module&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x191c134)
#12 0x000055cec93a2b84 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x191cb84)
#13 0x000055cec9b9c088 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x2116088)
#14 0x000055ceca221310 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x279b310)
#15 0x000055cecbdf340c clang::ParseAST(clang::Sema&, bool, bool) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x436d40c)
#16 0x000055ceca519eb8 clang::FrontendAction::Execute() (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x2a93eb8)
#17 0x000055ceca48d7eb clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x2a077eb)
#18 0x000055ceca5ee4a3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x2b684a3)
#19 0x000055cec88652c8 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0xddf2c8)
#20 0x000055cec885d052 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#21 0x000055ceca26f68d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#22 0x000055cec98632b7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x1ddd2b7)
#23 0x000055ceca26fa27 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#24 0x000055ceca232601 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x27ac601)
#25 0x000055ceca233629 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x27ad629)
#26 0x000055ceca246e3f clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0x27c0e3f)
#27 0x000055cec88616b1 clang_main(int, char**, llvm::ToolContext const&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0xddb6b1)
#28 0x000055cec87832fb main (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0xcfd2fb)
#29 0x00007ff00ad0bd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#30 0x00007ff00ad0be40 call_init ./csu/../csu/libc-start.c:128:20
#31 0x00007ff00ad0be40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#32 0x000055cec885cab5 _start (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin/riscv64-unknown-elf-clang+++0xdd6ab5)
riscv64-unknown-elf-clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 20.1.8 (https://github.com/llvm/llvm-project.git 87f0227cb60147a26a1eeb4fb06e3b505e9c7261)
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/bin
riscv64-unknown-elf-clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
riscv64-unknown-elf-clang++: note: diagnostic msg: /tmp/SimTop260-930fd0.cpp
riscv64-unknown-elf-clang++: note: diagnostic msg: /tmp/SimTop260-930fd0.sh
riscv64-unknown-elf-clang++: note: diagnostic msg:

********************
```

Preprocessed source: [SimTop260-930fd0.cpp.gz](https://github.com/user-attachments/files/22085940/SimTop260-930fd0.cpp.gz)
Run script: [SimTop260-930fd0.sh](https://github.com/user-attachments/files/22085936/SimTop260-930fd0.sh)

On llvm 21.1.1:
```
fatal error: error in backend: Incomplete scavenging after 2nd pass
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang++ -o build/applications/SimTop17.o -c -mcmodel=medany -march=rv64imac -mabi=lp64 -flax-vector-conversions -Wno-cpp -fno-common -ffunction-sections -fdata-sections -fdiagnostics-color=always -Xclang -fexperimental-max-bitint-width=20000 -fbracket-depth=2048 -Wno-parentheses-equality -O1 -mcmodel=medany -march=rv64imac -mabi=lp64 -flax-vector-conversions -Wno-cpp -fno-common -ffunction-sections -fdata-sections -fdiagnostics-color=always -Xclang -fexperimental-max-bitint-width=20000 -fbracket-depth=2048 -Wno-parentheses-equality -O1 -D__RTTHREAD__ -DRT_USING_NEWLIBC -DRT_USING_LIBC -D_POSIX_C_SOURCE=1 -I. -Iapplications -Iapplications/test -Iapplications/test/test_vector -Idriver -I/nfs/home/tanghaojin/rt-thread-2/include -I/nfs/home/tanghaojin/rt-thread-2/libcpu/risc-v/common64 -I/nfs/home/tanghaojin/rt-thread-2/libcpu/risc-v/virt64 -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/lwip-2.0.3/src/include -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/lwip-2.0.3/src/include/ipv4 -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/lwip-2.0.3/src/include/netif -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/port -I/nfs/home/tanghaojin/rt-thread-2/components/net/netdev/include -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include/socket -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/impl -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include/dfs_net -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include/socket/sys_socket -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/resource -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/utest -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/bitmap -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/ref -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/avl -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/uthash -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/hashmap -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/include -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/filesystems/romfs -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/filesystems/elmfat -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/filesystems/devfs -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/phy -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/include -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/virtio -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/smp_call -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/ktime -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/ktime/inc -I/nfs/home/tanghaojin/rt-thread-2/components/libc/cplusplus -I/nfs/home/tanghaojin/rt-thread-2/components/libc/cplusplus/os -I/nfs/home/tanghaojin/rt-thread-2/components/libc/compilers/common/include -I/nfs/home/tanghaojin/rt-thread-2/components/libc/compilers/newlib -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/ipc -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/mman -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/poll -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/termios -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/aio -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/epoll -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/eventfd -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/stdio -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/delay -I/nfs/home/tanghaojin/rt-thread-2/components/mm -I/nfs/home/tanghaojin/rt-thread-2/components/finsh applications/SimTop17.cpp -I/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/riscv64-unknown-elf/include/c++/15.1.0 -I/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/riscv64-unknown-elf/include/c++/15.1.0/riscv64-unknown-elf -I/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/riscv64-unknown-elf/include/c++/15.1.0/backward
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'applications/SimTop17.cpp'.
4.      Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@_ZN7SSimTop9subStep15Ev'
 #0 0x000056242c803a00 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1d08a00)
 #1 0x000056242c8015b4 llvm::sys::CleanupOnSignal(unsigned long) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1d065b4)
 #2 0x000056242c754ae7 llvm::CrashRecoveryContext::HandleExit(int) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1c59ae7)
 #3 0x000056242c7f8e22 llvm::sys::Process::Exit(int, bool) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1cfde22)
 #4 0x000056242b6e4daf LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x000056242c75cd40 llvm::report_fatal_error(llvm::Twine const&, bool) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1c61d40)
 #6 0x000056242c75cece (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1c61ece)
 #7 0x000056242bdde0a7 llvm::scavengeFrameVirtualRegs(llvm::MachineFunction&, llvm::RegScavenger&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x12e30a7)
 #8 0x000056242c07d085 (anonymous namespace)::PEIImpl::run(llvm::MachineFunction&) (.constprop.0) PrologEpilogInserter.cpp:0:0
 #9 0x000056242c07e467 (anonymous namespace)::PEILegacy::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#10 0x000056242bca11cd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x11a61cd)
#11 0x000056242c2898b2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x178e8b2)
#12 0x000056242c289a74 llvm::FPPassManager::runOnModule(llvm::Module&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x178ea74)
#13 0x000056242c28a4a4 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x178f4a4)
#14 0x000056242ca8f872 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) BackendUtil.cpp:0:0
#15 0x000056242ca90343 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1f95343)
#16 0x000056242d10e567 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x2613567)
#17 0x000056242eb3d7dc clang::ParseAST(clang::Sema&, bool, bool) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x40427dc)
#18 0x000056242d45148e clang::FrontendAction::Execute() (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x295648e)
#19 0x000056242d3c211b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x28c711b)
#20 0x000056242d52df00 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x2a32f00)
#21 0x000056242b6e6f2a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0xbebf2a)
#22 0x000056242b6de7a2 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#23 0x000056242d18387d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x000056242c7549d7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1c599d7)
#25 0x000056242d183c17 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x000056242d145f61 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x264af61)
#27 0x000056242d146fd9 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x264bfd9)
#28 0x000056242d15bbef clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x2660bef)
#29 0x000056242b6e32b5 clang_main(int, char**, llvm::ToolContext const&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0xbe82b5)
#30 0x000056242b5f75bb main (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0xafc5bb)
#31 0x00007f7f1d41fd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#32 0x00007f7f1d41fe40 call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007f7f1d41fe40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#34 0x000056242b6de205 _start (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0xbe3205)
riscv64-unknown-elf-clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 21.1.1 (https://github.com/llvm/llvm-project.git 5a86dc996c26299de63effc927075dcbfb924167)
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin
riscv64-unknown-elf-clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
riscv64-unknown-elf-clang++: note: diagnostic msg: /tmp/SimTop17-fbf525.cpp
riscv64-unknown-elf-clang++: note: diagnostic msg: /tmp/SimTop17-fbf525.sh
riscv64-unknown-elf-clang++: note: diagnostic msg:

********************
```

Preprocessed source: [SimTop17-fbf525.cpp.gz](https://github.com/user-attachments/files/22527310/SimTop17-fbf525.cpp.gz)
Run script: [SimTop17-fbf525.sh](https://github.com/user-attachments/files/22527313/SimTop17-fbf525.sh)

## Comments

### Comment 1 - Tang-Haojin

I tried reducing the source file to make it easier to pinpoint the issue, but even removing less than 1% of the code can prevent the problem from being reproduced.

---

### Comment 2 - ukalappa-mips

@Tang-Haojin  ,you still see this issue ? ...if so please try passing " -fno-omit-frame-pointer" and compile ?

---

### Comment 3 - EugeneZelenko

Could you please try 21 or `main` branch?

---

### Comment 4 - Tang-Haojin

> [@Tang-Haojin](https://github.com/Tang-Haojin) ,you still see this issue ? ...if so please try passing " -fno-omit-frame-pointer" and compile ?

I added "-fno-omit-frame-pointer" to [SimTop260-930fd0.sh](https://github.com/user-attachments/files/22085936/SimTop260-930fd0.sh) but it reported an unknown argument.

<img width="431" height="42" alt="Image" src="https://github.com/user-attachments/assets/11f3f5d6-bf2e-42a9-9b29-d28629272a2d" />

---

### Comment 5 - Tang-Haojin

> Could you please try 21 or `main` branch?

https://github.com/riscv-collab/riscv-gnu-toolchain have bumped llvm 21.1.1 recently and I tried the latest nightly build https://github.com/riscv-collab/riscv-gnu-toolchain/releases/download/2025.09.20/riscv64-elf-ubuntu-22.04-llvm-nightly-2025.09.20-nightly.tar.xz.

The file originally attached for llvm 20.1.8 can pass now, but another file in my project seems failed for the similar reason.

```
fatal error: error in backend: Incomplete scavenging after 2nd pass
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang++ -o build/applications/SimTop17.o -c -mcmodel=medany -march=rv64imac -mabi=lp64 -flax-vector-conversions -Wno-cpp -fno-common -ffunction-sections -fdata-sections -fdiagnostics-color=always -Xclang -fexperimental-max-bitint-width=20000 -fbracket-depth=2048 -Wno-parentheses-equality -O1 -mcmodel=medany -march=rv64imac -mabi=lp64 -flax-vector-conversions -Wno-cpp -fno-common -ffunction-sections -fdata-sections -fdiagnostics-color=always -Xclang -fexperimental-max-bitint-width=20000 -fbracket-depth=2048 -Wno-parentheses-equality -O1 -D__RTTHREAD__ -DRT_USING_NEWLIBC -DRT_USING_LIBC -D_POSIX_C_SOURCE=1 -I. -Iapplications -Iapplications/test -Iapplications/test/test_vector -Idriver -I/nfs/home/tanghaojin/rt-thread-2/include -I/nfs/home/tanghaojin/rt-thread-2/libcpu/risc-v/common64 -I/nfs/home/tanghaojin/rt-thread-2/libcpu/risc-v/virt64 -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/lwip-2.0.3/src/include -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/lwip-2.0.3/src/include/ipv4 -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/lwip-2.0.3/src/include/netif -I/nfs/home/tanghaojin/rt-thread-2/components/net/lwip/port -I/nfs/home/tanghaojin/rt-thread-2/components/net/netdev/include -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include/socket -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/impl -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include/dfs_net -I/nfs/home/tanghaojin/rt-thread-2/components/net/sal/include/socket/sys_socket -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/resource -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/utest -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/bitmap -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/ref -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/avl -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/uthash -I/nfs/home/tanghaojin/rt-thread-2/components/utilities/libadt/hashmap -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/include -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/filesystems/romfs -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/filesystems/elmfat -I/nfs/home/tanghaojin/rt-thread-2/components/dfs/dfs_v2/filesystems/devfs -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/phy -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/include -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/virtio -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/smp_call -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/ktime -I/nfs/home/tanghaojin/rt-thread-2/components/drivers/ktime/inc -I/nfs/home/tanghaojin/rt-thread-2/components/libc/cplusplus -I/nfs/home/tanghaojin/rt-thread-2/components/libc/cplusplus/os -I/nfs/home/tanghaojin/rt-thread-2/components/libc/compilers/common/include -I/nfs/home/tanghaojin/rt-thread-2/components/libc/compilers/newlib -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/ipc -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/mman -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/poll -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/termios -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/aio -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/epoll -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/eventfd -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/io/stdio -I/nfs/home/tanghaojin/rt-thread-2/components/libc/posix/delay -I/nfs/home/tanghaojin/rt-thread-2/components/mm -I/nfs/home/tanghaojin/rt-thread-2/components/finsh applications/SimTop17.cpp -I/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/riscv64-unknown-elf/include/c++/15.1.0 -I/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/riscv64-unknown-elf/include/c++/15.1.0/riscv64-unknown-elf -I/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm20/riscv64-unknown-elf/include/c++/15.1.0/backward
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'applications/SimTop17.cpp'.
4.      Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@_ZN7SSimTop9subStep15Ev'
 #0 0x000056242c803a00 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1d08a00)
 #1 0x000056242c8015b4 llvm::sys::CleanupOnSignal(unsigned long) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1d065b4)
 #2 0x000056242c754ae7 llvm::CrashRecoveryContext::HandleExit(int) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1c59ae7)
 #3 0x000056242c7f8e22 llvm::sys::Process::Exit(int, bool) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1cfde22)
 #4 0x000056242b6e4daf LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x000056242c75cd40 llvm::report_fatal_error(llvm::Twine const&, bool) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1c61d40)
 #6 0x000056242c75cece (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1c61ece)
 #7 0x000056242bdde0a7 llvm::scavengeFrameVirtualRegs(llvm::MachineFunction&, llvm::RegScavenger&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x12e30a7)
 #8 0x000056242c07d085 (anonymous namespace)::PEIImpl::run(llvm::MachineFunction&) (.constprop.0) PrologEpilogInserter.cpp:0:0
 #9 0x000056242c07e467 (anonymous namespace)::PEILegacy::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#10 0x000056242bca11cd llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x11a61cd)
#11 0x000056242c2898b2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x178e8b2)
#12 0x000056242c289a74 llvm::FPPassManager::runOnModule(llvm::Module&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x178ea74)
#13 0x000056242c28a4a4 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x178f4a4)
#14 0x000056242ca8f872 (anonymous namespace)::EmitAssemblyHelper::RunCodegenPipeline(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>&, std::unique_ptr<llvm::ToolOutputFile, std::default_delete<llvm::ToolOutputFile>>&) BackendUtil.cpp:0:0
#15 0x000056242ca90343 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1f95343)
#16 0x000056242d10e567 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x2613567)
#17 0x000056242eb3d7dc clang::ParseAST(clang::Sema&, bool, bool) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x40427dc)
#18 0x000056242d45148e clang::FrontendAction::Execute() (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x295648e)
#19 0x000056242d3c211b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x28c711b)
#20 0x000056242d52df00 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x2a32f00)
#21 0x000056242b6e6f2a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0xbebf2a)
#22 0x000056242b6de7a2 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#23 0x000056242d18387d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x000056242c7549d7 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x1c599d7)
#25 0x000056242d183c17 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x000056242d145f61 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x264af61)
#27 0x000056242d146fd9 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x264bfd9)
#28 0x000056242d15bbef clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0x2660bef)
#29 0x000056242b6e32b5 clang_main(int, char**, llvm::ToolContext const&) (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0xbe82b5)
#30 0x000056242b5f75bb main (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0xafc5bb)
#31 0x00007f7f1d41fd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#32 0x00007f7f1d41fe40 call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007f7f1d41fe40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#34 0x000056242b6de205 _start (/nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin/riscv64-unknown-elf-clang+++0xbe3205)
riscv64-unknown-elf-clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 21.1.1 (https://github.com/llvm/llvm-project.git 5a86dc996c26299de63effc927075dcbfb924167)
Target: riscv64-unknown-unknown-elf
Thread model: posix
InstalledDir: /nfs/home/tanghaojin/riscv-llvm-newlib/riscv-llvm21/bin
riscv64-unknown-elf-clang++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
riscv64-unknown-elf-clang++: note: diagnostic msg: /tmp/SimTop17-fbf525.cpp
riscv64-unknown-elf-clang++: note: diagnostic msg: /tmp/SimTop17-fbf525.sh
riscv64-unknown-elf-clang++: note: diagnostic msg:

********************
```

[SimTop17-fbf525.cpp.gz](https://github.com/user-attachments/files/22527310/SimTop17-fbf525.cpp.gz)
[SimTop17-fbf525.sh](https://github.com/user-attachments/files/22527313/SimTop17-fbf525.sh)

---

### Comment 6 - ukalappa-mips

> > [@Tang-Haojin](https://github.com/Tang-Haojin) ,you still see this issue ? ...if so please try passing " -fno-omit-frame-pointer" and compile ?
> 
> I added "-fno-omit-frame-pointer" to [SimTop260-930fd0.sh](https://github.com/user-attachments/files/22085936/SimTop260-930fd0.sh) but it reported an unknown argument.
> 
> <img alt="Image" width="431" height="42" src="https://private-user-images.githubusercontent.com/46089480/493659848-11f3f5d6-bf2e-42a9-9b29-d28629272a2d.png?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NTg3NzI4OTUsIm5iZiI6MTc1ODc3MjU5NSwicGF0aCI6Ii80NjA4OTQ4MC80OTM2NTk4NDgtMTFmM2Y1ZDYtYmYyZS00MmE5LTliMjktZDI4NjI5MjcyYTJkLnBuZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNTA5MjUlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjUwOTI1VDAzNTYzNVomWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTFmNjQ1NDhhNGU5ZjUzZDQ2YWI0Y2VlMWVjMWUwZWJjMjZiMDU3NWRlMTdhZWRhZmEyYmNiZDU4MmFlZWI4NjcmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0In0.42PogJJxfnaKm33zqLjaPetbW417msP4eWUjdR24wMM">

same works for us ...like 
$clang -fno-omit-frame-pointer test.c -c
$clang --version 
clang version 21.1.1 (https://github.com/llvm/llvm-project.git 5a86dc996c26299de63effc927075dcbfb924167)

https://godbolt.org/z/5TdTY6rqd

---

### Comment 7 - Tang-Haojin

> > > [@Tang-Haojin](https://github.com/Tang-Haojin) ,you still see this issue ? ...if so please try passing " -fno-omit-frame-pointer" and compile ?
> > 
> > 
> > I added "-fno-omit-frame-pointer" to [SimTop260-930fd0.sh](https://github.com/user-attachments/files/22085936/SimTop260-930fd0.sh) but it reported an unknown argument.
> > <img alt="Image" width="431" height="42" src="https://private-user-images.githubusercontent.com/46089480/493659848-11f3f5d6-bf2e-42a9-9b29-d28629272a2d.png?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NTg3NzI4OTUsIm5iZiI6MTc1ODc3MjU5NSwicGF0aCI6Ii80NjA4OTQ4MC80OTM2NTk4NDgtMTFmM2Y1ZDYtYmYyZS00MmE5LTliMjktZDI4NjI5MjcyYTJkLnBuZz9YLUFtei1BbGdvcml0aG09QVdTNC1ITUFDLVNIQTI1NiZYLUFtei1DcmVkZW50aWFsPUFLSUFWQ09EWUxTQTUzUFFLNFpBJTJGMjAyNTA5MjUlMkZ1cy1lYXN0LTElMkZzMyUyRmF3czRfcmVxdWVzdCZYLUFtei1EYXRlPTIwMjUwOTI1VDAzNTYzNVomWC1BbXotRXhwaXJlcz0zMDAmWC1BbXotU2lnbmF0dXJlPTFmNjQ1NDhhNGU5ZjUzZDQ2YWI0Y2VlMWVjMWUwZWJjMjZiMDU3NWRlMTdhZWRhZmEyYmNiZDU4MmFlZWI4NjcmWC1BbXotU2lnbmVkSGVhZGVycz1ob3N0In0.42PogJJxfnaKm33zqLjaPetbW417msP4eWUjdR24wMM">
> 
> same works for us ...like $clang -fno-omit-frame-pointer test.c -c $clang --version clang version 21.1.1 (https://github.com/llvm/llvm-project.git [5a86dc9](https://github.com/llvm/llvm-project/commit/5a86dc996c26299de63effc927075dcbfb924167))
> 
> https://godbolt.org/z/5TdTY6rqd

Hi, I tried again to directly add "-fno-omit-frame-pointer" to my project configuration and this time the "error in backend: Incomplete scavenging after 2nd pass" issue did not occur. Thank you for your help!

---

### Comment 8 - ukalappa-mips

@Tang-Haojin  ...But this issue need to be fixed ....so please don't close this issue ....

---

