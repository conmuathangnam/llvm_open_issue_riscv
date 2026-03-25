# SCX_CAKE build failure on clang-20

**Author:** thomas-zimmerman
**URL:** https://github.com/llvm/llvm-project/issues/178915
**Status:** Closed
**Labels:** backend:BPF, crash, llvm:SelectionDAG
**Closed Date:** 2026-02-03T15:53:10Z

## Body

I get this build failure on Pop!OS 24.04 of the csx_cake BPF scheduler and the compiler asked that I report it.

https://github.com/RitzDaCat/scx_cake
Nightly checkout hash: 213dc99

```
Caused by:
  process didn't exit successfully: `/home/qubes/src/scx_cake/target/release/build/scx_cake-e6cbd6ff0bc7e155/build-script-build` (exit status: 101)
  --- stdout
  scx_utils:clang=ClangInfo { clang: "clang", ver: "20.1.2", arch: "x86_64" } ["-g", "-O2", "-Wall", "-Wno-compare-distinct-pointer-types", "-D__TARGET_ARCH_x86", "-mcpu=v3", "-mlittle-endian", "-idirafter", "/usr/lib/llvm-20/lib/clang/20/include", "-idirafter", "/usr/local/include", "-idirafter", "/usr/include/x86_64-linux-gnu", "-idirafter", "/usr/include", "-O3", "-mcpu=v4", "-fno-stack-protector", "-fno-asynchronous-unwind-tables", "-fno-jump-tables", "-fno-strict-aliasing", "-I/home/qubes/src/scx_cake/target/release/build/scx_cake-75f9d1c7cb8fef40/out/scx_utils-bpf_h/arch/x86", "-I/home/qubes/src/scx_cake/target/release/build/scx_cake-75f9d1c7cb8fef40/out/scx_utils-bpf_h", "-I/home/qubes/src/scx_cake/target/release/build/scx_cake-75f9d1c7cb8fef40/out/scx_utils-bpf_h/bpf-compat", "-D__SCX_TARGET_ARCH_x86"]

  --- stderr

  thread 'main' (417999) panicked at build.rs:12:10:
  called `Result::unwrap()` on an `Err` value: failed to build `src/bpf/cake.bpf.c`

  Caused by:
      0: failed to compile `src/bpf/cake.bpf.c`
      1: failed to compile /tmp/.tmpP1LbWd/cake.bpf.c from src/bpf/cake.bpf.c
      2: command `clang -g -O2 -Wall -Wno-compare-distinct-pointer-types -D__TARGET_ARCH_x86 -mcpu=v3 -mlittle-endian -idirafter /usr/lib/llvm-20/lib/clang/20/include -idirafter /usr/local/include -idirafter /usr/include/x86_64-linux-gnu -idirafter /usr/include -O3 -mcpu=v4 -fno-stack-protector -fno-asynchronous-unwind-tables -fno-jump-tables -fno-strict-aliasing -I/home/qubes/src/scx_cake/target/release/build/scx_cake-75f9d1c7cb8fef40/out/scx_utils-bpf_h/arch/x86 -I/home/qubes/src/scx_cake/target/release/build/scx_cake-75f9d1c7cb8fef40/out/scx_utils-bpf_h -I/home/qubes/src/scx_cake/target/release/build/scx_cake-75f9d1c7cb8fef40/out/scx_utils-bpf_h/bpf-compat -D__SCX_TARGET_ARCH_x86 -I /tmp/.tmpLk6GZY/bpf/src -fno-stack-protector -g -O2 -target bpf -c src/bpf/cake.bpf.c -o /tmp/.tmpP1LbWd/cake.bpf.c` failed (exit status: 1)
      3: fatal error: error in backend: Cannot select: 0x61f8a17c7900: i32,ch = AtomicLoad<(load monotonic (s32) from %ir.46, align 8)> 0x61f8a007cd20, 0x61f8a17c82a0, src/bpf/cake.bpf.c:605:15 @[ src/bpf/cake.bpf.c:557:5 ]
           0x61f8a17c82a0: i64 = add nuw 0x61f8a17c7c10, Constant:i64<8>, src/bpf/cake.bpf.c:605:15 @[ src/bpf/cake.bpf.c:557:5 ]
             0x61f8a17c7c10: i64,ch = CopyFromReg 0x61f8a007cd20, Register:i64 %7, src/bpf/cake.bpf.c:605:15 @[ src/bpf/cake.bpf.c:557:5 ]
               0x61f8a17c75f0: i64 = Register %7
             0x61f8a17c79e0: i64 = Constant<8>
         In function: cake_select_cpu
         PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
         Stack dump:
         0.	Program arguments: clang -g -O2 -Wall -Wno-compare-distinct-pointer-types -D__TARGET_ARCH_x86 -mcpu=v3 -mlittle-endian -idirafter /usr/lib/llvm-20/lib/clang/20/include -idirafter /usr/local/include -idirafter /usr/include/x86_64-linux-gnu -idirafter /usr/include -O3 -mcpu=v4 -fno-stack-protector -fno-asynchronous-unwind-tables -fno-jump-tables -fno-strict-aliasing -I/home/qubes/src/scx_cake/target/release/build/scx_cake-75f9d1c7cb8fef40/out/scx_utils-bpf_h/arch/x86 -I/home/qubes/src/scx_cake/target/release/build/scx_cake-75f9d1c7cb8fef40/out/scx_utils-bpf_h -I/home/qubes/src/scx_cake/target/release/build/scx_cake-75f9d1c7cb8fef40/out/scx_utils-bpf_h/bpf-compat -D__SCX_TARGET_ARCH_x86 -I /tmp/.tmpLk6GZY/bpf/src -fno-stack-protector -g -O2 -target bpf -c src/bpf/cake.bpf.c -o /tmp/.tmpP1LbWd/cake.bpf.c
         1.	<eof> parser at end of file
         2.	Code generation
         3.	Running pass 'Function Pass Manager' on module 'src/bpf/cake.bpf.c'.
         4.	Running pass 'BPF DAG->DAG Pattern Instruction Selection' on function '@cake_select_cpu'
          #0 0x000079780442bc7f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x102bc7f)
          #1 0x0000797804429989 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1029989)
          #2 0x000079780436eac3 (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0xf6eac3)
          #3 0x000079780436ea72 (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0xf6ea72)
          #4 0x0000797804425f30 llvm::sys::Process::Exit(int, bool) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1025f30)
          #5 0x000061f89c8beb9b (/usr/lib/llvm-20/bin/clang+0x13b9b)
          #6 0x000079780437e4b4 llvm::report_fatal_error(llvm::Twine const&, bool) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0xf7e4b4)
          #7 0x0000797804d33e69 (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1933e69)
          #8 0x0000797804d3331d llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x193331d)
          #9 0x0000797806d70383 (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x3970383)
         #10 0x0000797804d29dcc llvm::SelectionDAGISel::DoInstructionSelection() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1929dcc)
         #11 0x0000797804d29493 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1929493)
         #12 0x0000797804d28cbc llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1928cbc)
         #13 0x0000797804d26afa llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1926afa)
         #14 0x0000797804d24e41 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1924e41)
         #15 0x0000797804843c23 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1443c23)
         #16 0x000079780458869f llvm::FPPassManager::runOnFunction(llvm::Function&) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x118869f)
         #17 0x000079780458e614 llvm::FPPassManager::runOnModule(llvm::Module&) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x118e614)
         #18 0x0000797804588ea7 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0x1188ea7)
         #19 0x000079780dc80b72 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x1e80b72)
         #20 0x000079780e047383 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2247383)
         #21 0x000079780cb6e7a9 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0xd6e7a9)
         #22 0x000079780ead14a2 clang::FrontendAction::Execute() (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2cd14a2)
         #23 0x000079780ea49e44 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2c49e44)
         #24 0x000079780eb54e7c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x2d54e7c)
         #25 0x000061f89c8be74f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-20/bin/clang+0x1374f)
         #26 0x000061f89c8bb5d4 (/usr/lib/llvm-20/bin/clang+0x105d4)
         #27 0x000079780e6de052 (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x28de052)
         #28 0x000079780436ea47 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-20/bin/../lib/libLLVM.so.20.1+0xf6ea47)
         #29 0x000079780e6dd8a6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x28dd8a6)
         #30 0x000079780e6a5962 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x28a5962)
         #31 0x000079780e6a5b4f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x28a5b4f)
         #32 0x000079780e6c12d6 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-20/bin/../lib/libclang-cpp.so.20.1+0x28c12d6)
         #33 0x000061f89c8bb09a clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-20/bin/clang+0x1009a)
         #34 0x000061f89c8c7f13 main (/usr/lib/llvm-20/bin/clang+0x1cf13)
         #35 0x0000797802c2a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
         #36 0x0000797802c2a28b call_init ./csu/../csu/libc-start.c:128:20
         #37 0x0000797802c2a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
         #38 0x000061f89c8b9435 _start (/usr/lib/llvm-20/bin/clang+0xe435)
         clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
         Ubuntu clang version 20.1.2 (0ubuntu1~24.04.2)
         Target: bpf
         Thread model: posix
         InstalledDir: /usr/lib/llvm-20/bin
         clang: note: diagnostic msg:
         ********************

         PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
         Preprocessed source(s) and associated run script(s) are located at:
         clang: note: diagnostic msg: /tmp/cake-10bbc3.c
         clang: note: diagnostic msg: /tmp/cake-10bbc3.sh
         clang: note: diagnostic msg:

         ********************
```

[cake-10bbc3.c](https://github.com/user-attachments/files/24970242/cake-10bbc3.c)
[cake-10bbc3.sh](https://github.com/user-attachments/files/24970241/cake-10bbc3.sh)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 22 release candidate or `main` branch? Only most recent release is maintained.


---

### Comment 2 - thomas-zimmerman

Checked out 22 release candidate and built; with that clang version in my path first, this code built without issue.

---

