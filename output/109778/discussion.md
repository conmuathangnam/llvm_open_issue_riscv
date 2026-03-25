# clang crash assigning to a global named register variable when the matching -ffixed- option is not used

**Author:** DavidSpickett
**URL:** https://github.com/llvm/llvm-project/issues/109778

## Body

https://godbolt.org/z/fcMnqKGxM

```
#include <stdint.h>

register uint64_t t asm ("x21");

void fn() {
    t = 1;
}
```
Compile with:
```
$ ./bin/clang -target aarch64-linux-gnu /tmp/test.c -c -o /dev/null
```
Crashes:
```
fatal error: error in backend: Invalid register name "x21".
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ./bin/clang -target aarch64-linux-gnu /tmp/test.c -c -o /dev/null
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/tmp/test.c'.
4.      Running pass 'AArch64 Instruction Selection' on function '@fn'
 #0 0x0000ffffa92c8a14 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0x188a14)
 #1 0x0000ffffa92c6884 llvm::sys::RunSignalHandlers() (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0x186884)
 #2 0x0000ffffa9200ebc (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #3 0x0000ffffa9200e4c llvm::CrashRecoveryContext::HandleExit(int) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0xc0e4c)
 #4 0x0000ffffa92c2d44 llvm::sys::Process::Exit(int, bool) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0x182d44)
 #5 0x0000aaaab717e87c llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::~IntrusiveRefCntPtr() (./bin/clang+0x1287c)
 #6 0x0000ffffa9213a0c llvm::report_fatal_error(llvm::Twine const&, bool) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0xd3a0c)
 #7 0x0000ffffb3a876e8 llvm::AArch64TargetLowering::getRegisterByName(char const*, llvm::LLT, llvm::MachineFunction const&) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMAArch64CodeGen.so.20.0git+0x2476e8)
 #8 0x0000ffffa881473c llvm::SelectionDAGISel::Select_WRITE_REGISTER(llvm::SDNode*) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x2a473c)
 #9 0x0000ffffb3a21054 (anonymous namespace)::AArch64DAGToDAGISel::Select(llvm::SDNode*) AArch64ISelDAGToDAG.cpp:0:0
#10 0x0000ffffa8810458 llvm::SelectionDAGISel::DoInstructionSelection() (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x2a0458)
#11 0x0000ffffa880f340 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x29f340)
#12 0x0000ffffa880d270 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x29d270)
#13 0x0000ffffa880a76c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x29a76c)
#14 0x0000ffffa88081dc llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x2981dc)
#15 0x0000ffffac95d3f4 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCodeGen.so.20.0git+0x34d3f4)
#16 0x0000ffffa96c7330 llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCore.so.20.0git+0x297330)
#17 0x0000ffffa96cefe8 llvm::FPPassManager::runOnModule(llvm::Module&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCore.so.20.0git+0x29efe8)
#18 0x0000ffffa96c7d10 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCore.so.20.0git+0x297d10)
#19 0x0000ffffad0817d8 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangCodeGen.so.20.0git+0xf17d8)
#20 0x0000ffffad49f740 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangCodeGen.so.20.0git+0x50f740)
#21 0x0000ffffa5de9a18 clang::ParseAST(clang::Sema&, bool, bool) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libclangParse.so.20.0git+0x39a18)
#22 0x0000ffffabaddf08 clang::FrontendAction::Execute() (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangFrontend.so.20.0git+0x13df08)
#23 0x0000ffffaba5bd44 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangFrontend.so.20.0git+0xbbd44)
#24 0x0000ffffaef33cc4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangFrontendTool.so.20.0git+0x3cc4)
#25 0x0000aaaab717e3c4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (./bin/clang+0x123c4)
#26 0x0000aaaab717aee0 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x0000ffffab74d3cc void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#28 0x0000ffffa9200de0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0xc0de0)
#29 0x0000ffffab74c600 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0xcc600)
#30 0x0000ffffab710690 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0x90690)
#31 0x0000ffffab710878 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0x90878)
#32 0x0000ffffab729728 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0xa9728)
#33 0x0000aaaab717a30c clang_main(int, char**, llvm::ToolContext const&) (./bin/clang+0xe30c)
#34 0x0000aaaab7187c44 main (./bin/clang+0x1bc44)
#35 0x0000ffffa8cb73fc __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#36 0x0000ffffa8cb74cc call_init ./csu/../csu/libc-start.c:128:20
#37 0x0000ffffa8cb74cc __libc_start_main ./csu/../csu/libc-start.c:379:5
#38 0x0000aaaab7178bf0 _start (./bin/clang+0xcbf0)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git be0b1142df7733633354ef1f73d0379bcd2ccb54)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/david.spickett/build-llvm-aarch64/bin
Build config: +assertions
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-b8124c.c
clang: note: diagnostic msg: /tmp/test-b8124c.sh
clang: note: diagnostic msg:

********************
```
If I remove the assignment, the error does not happen.

## Comments

### Comment 1 - DavidSpickett

Also happens for ARM (32 bit Arm that is): https://godbolt.org/z/8f14YYK1c

```
#include <stdint.h>

register uint32_t t asm ("r10");

void fn() {
    t = 1;
}
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: David Spickett (DavidSpickett)

<details>
https://godbolt.org/z/fcMnqKGxM

```
#include &lt;stdint.h&gt;

register uint64_t t asm ("x21");

void fn() {
    t = 1;
}
```
Compile with:
```
$ ./bin/clang -target aarch64-linux-gnu /tmp/test.c -c -o /dev/null
```
Crashes:
```
fatal error: error in backend: Invalid register name "x21".
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ./bin/clang -target aarch64-linux-gnu /tmp/test.c -c -o /dev/null
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/tmp/test.c'.
4.      Running pass 'AArch64 Instruction Selection' on function '@<!-- -->fn'
 #<!-- -->0 0x0000ffffa92c8a14 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0x188a14)
 #<!-- -->1 0x0000ffffa92c6884 llvm::sys::RunSignalHandlers() (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0x186884)
 #<!-- -->2 0x0000ffffa9200ebc (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000ffffa9200e4c llvm::CrashRecoveryContext::HandleExit(int) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0xc0e4c)
 #<!-- -->4 0x0000ffffa92c2d44 llvm::sys::Process::Exit(int, bool) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0x182d44)
 #<!-- -->5 0x0000aaaab717e87c llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::~IntrusiveRefCntPtr() (./bin/clang+0x1287c)
 #<!-- -->6 0x0000ffffa9213a0c llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0xd3a0c)
 #<!-- -->7 0x0000ffffb3a876e8 llvm::AArch64TargetLowering::getRegisterByName(char const*, llvm::LLT, llvm::MachineFunction const&amp;) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMAArch64CodeGen.so.20.0git+0x2476e8)
 #<!-- -->8 0x0000ffffa881473c llvm::SelectionDAGISel::Select_WRITE_REGISTER(llvm::SDNode*) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x2a473c)
 #<!-- -->9 0x0000ffffb3a21054 (anonymous namespace)::AArch64DAGToDAGISel::Select(llvm::SDNode*) AArch64ISelDAGToDAG.cpp:0:0
#<!-- -->10 0x0000ffffa8810458 llvm::SelectionDAGISel::DoInstructionSelection() (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x2a0458)
#<!-- -->11 0x0000ffffa880f340 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x29f340)
#<!-- -->12 0x0000ffffa880d270 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x29d270)
#<!-- -->13 0x0000ffffa880a76c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x29a76c)
#<!-- -->14 0x0000ffffa88081dc llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x2981dc)
#<!-- -->15 0x0000ffffac95d3f4 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCodeGen.so.20.0git+0x34d3f4)
#<!-- -->16 0x0000ffffa96c7330 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCore.so.20.0git+0x297330)
#<!-- -->17 0x0000ffffa96cefe8 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCore.so.20.0git+0x29efe8)
#<!-- -->18 0x0000ffffa96c7d10 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCore.so.20.0git+0x297d10)
#<!-- -->19 0x0000ffffad0817d8 clang::EmitBackendOutput(clang::DiagnosticsEngine&amp;, clang::HeaderSearchOptions const&amp;, clang::CodeGenOptions const&amp;, clang::TargetOptions const&amp;, clang::LangOptions const&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangCodeGen.so.20.0git+0xf17d8)
#<!-- -->20 0x0000ffffad49f740 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangCodeGen.so.20.0git+0x50f740)
#<!-- -->21 0x0000ffffa5de9a18 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libclangParse.so.20.0git+0x39a18)
#<!-- -->22 0x0000ffffabaddf08 clang::FrontendAction::Execute() (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangFrontend.so.20.0git+0x13df08)
#<!-- -->23 0x0000ffffaba5bd44 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangFrontend.so.20.0git+0xbbd44)
#<!-- -->24 0x0000ffffaef33cc4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangFrontendTool.so.20.0git+0x3cc4)
#<!-- -->25 0x0000aaaab717e3c4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (./bin/clang+0x123c4)
#<!-- -->26 0x0000aaaab717aee0 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x0000ffffab74d3cc void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000ffffa9200de0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0xc0de0)
#<!-- -->29 0x0000ffffab74c600 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0xcc600)
#<!-- -->30 0x0000ffffab710690 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0x90690)
#<!-- -->31 0x0000ffffab710878 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0x90878)
#<!-- -->32 0x0000ffffab729728 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0xa9728)
#<!-- -->33 0x0000aaaab717a30c clang_main(int, char**, llvm::ToolContext const&amp;) (./bin/clang+0xe30c)
#<!-- -->34 0x0000aaaab7187c44 main (./bin/clang+0x1bc44)
#<!-- -->35 0x0000ffffa8cb73fc __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->36 0x0000ffffa8cb74cc call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->37 0x0000ffffa8cb74cc __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->38 0x0000aaaab7178bf0 _start (./bin/clang+0xcbf0)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git be0b1142df7733633354ef1f73d0379bcd2ccb54)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/david.spickett/build-llvm-aarch64/bin
Build config: +assertions
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-b8124c.c
clang: note: diagnostic msg: /tmp/test-b8124c.sh
clang: note: diagnostic msg:

********************
```
If I remove the assignment, the error does not happen.
</details>


---

### Comment 3 - shafik

The crash goes back to clang-10: https://godbolt.org/z/6EnhE1nMK

---

### Comment 4 - akshaykumars614

I would like to take this issue

---

### Comment 5 - DavidSpickett

The issue is more clear if you use the RISC-V backend:
```
#include <stdint.h>

register uint64_t t asm ("x21");

void fn() {
    t = 1;
}
```
```
fatal error: error in backend: Trying to obtain non-reserved register "x21".
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'RISC-V DAG->DAG Pattern Instruction Selection' on function '@fn'
 #0 0x0000000003988e88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3988e88)
 #1 0x0000000003986fd4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3986fd4)
 #2 0x00000000038d7ff3 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38d7ff3)
 #3 0x000000000397ea1e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x397ea1e)
 #4 0x0000000000cec1eb LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x00000000038e2063 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x38e2063)
 #6 0x000000000206abdf llvm::RISCVTargetLowering::getRegisterByName(char const*, llvm::LLT, llvm::MachineFunction const&) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x206abdf)
```
Adding the option `-ffixed-x21` will reserve the register and avoid the crash. GCC without that option uses the register anyway, though it might not be safe from being modified by regular code.

So the bug here is that we don't (possibly cannot) tell the user in a nice way that they did not reserve the register they are trying to use as a global named register variable.

---

### Comment 6 - c-rhodes

this was fixed for AArch64 in #117419 so the AArch64 label can be dropped. Issue remains on RISCV (and potentially other targets, not checked), so keeping open.

---

### Comment 7 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: David Spickett (DavidSpickett)

<details>
https://godbolt.org/z/fcMnqKGxM

```
#include &lt;stdint.h&gt;

register uint64_t t asm ("x21");

void fn() {
    t = 1;
}
```
Compile with:
```
$ ./bin/clang -target aarch64-linux-gnu /tmp/test.c -c -o /dev/null
```
Crashes:
```
fatal error: error in backend: Invalid register name "x21".
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: ./bin/clang -target aarch64-linux-gnu /tmp/test.c -c -o /dev/null
1.      &lt;eof&gt; parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module '/tmp/test.c'.
4.      Running pass 'AArch64 Instruction Selection' on function '@<!-- -->fn'
 #<!-- -->0 0x0000ffffa92c8a14 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0x188a14)
 #<!-- -->1 0x0000ffffa92c6884 llvm::sys::RunSignalHandlers() (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0x186884)
 #<!-- -->2 0x0000ffffa9200ebc (anonymous namespace)::CrashRecoveryContextImpl::HandleCrash(int, unsigned long) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000ffffa9200e4c llvm::CrashRecoveryContext::HandleExit(int) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0xc0e4c)
 #<!-- -->4 0x0000ffffa92c2d44 llvm::sys::Process::Exit(int, bool) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0x182d44)
 #<!-- -->5 0x0000aaaab717e87c llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::~IntrusiveRefCntPtr() (./bin/clang+0x1287c)
 #<!-- -->6 0x0000ffffa9213a0c llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0xd3a0c)
 #<!-- -->7 0x0000ffffb3a876e8 llvm::AArch64TargetLowering::getRegisterByName(char const*, llvm::LLT, llvm::MachineFunction const&amp;) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMAArch64CodeGen.so.20.0git+0x2476e8)
 #<!-- -->8 0x0000ffffa881473c llvm::SelectionDAGISel::Select_WRITE_REGISTER(llvm::SDNode*) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x2a473c)
 #<!-- -->9 0x0000ffffb3a21054 (anonymous namespace)::AArch64DAGToDAGISel::Select(llvm::SDNode*) AArch64ISelDAGToDAG.cpp:0:0
#<!-- -->10 0x0000ffffa8810458 llvm::SelectionDAGISel::DoInstructionSelection() (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x2a0458)
#<!-- -->11 0x0000ffffa880f340 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x29f340)
#<!-- -->12 0x0000ffffa880d270 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x29d270)
#<!-- -->13 0x0000ffffa880a76c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x29a76c)
#<!-- -->14 0x0000ffffa88081dc llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libLLVMSelectionDAG.so.20.0git+0x2981dc)
#<!-- -->15 0x0000ffffac95d3f4 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCodeGen.so.20.0git+0x34d3f4)
#<!-- -->16 0x0000ffffa96c7330 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCore.so.20.0git+0x297330)
#<!-- -->17 0x0000ffffa96cefe8 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCore.so.20.0git+0x29efe8)
#<!-- -->18 0x0000ffffa96c7d10 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMCore.so.20.0git+0x297d10)
#<!-- -->19 0x0000ffffad0817d8 clang::EmitBackendOutput(clang::DiagnosticsEngine&amp;, clang::HeaderSearchOptions const&amp;, clang::CodeGenOptions const&amp;, clang::TargetOptions const&amp;, clang::LangOptions const&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangCodeGen.so.20.0git+0xf17d8)
#<!-- -->20 0x0000ffffad49f740 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangCodeGen.so.20.0git+0x50f740)
#<!-- -->21 0x0000ffffa5de9a18 clang::ParseAST(clang::Sema&amp;, bool, bool) (/home/david.spickett/build-llvm-aarch64/bin/../lib/../lib/libclangParse.so.20.0git+0x39a18)
#<!-- -->22 0x0000ffffabaddf08 clang::FrontendAction::Execute() (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangFrontend.so.20.0git+0x13df08)
#<!-- -->23 0x0000ffffaba5bd44 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangFrontend.so.20.0git+0xbbd44)
#<!-- -->24 0x0000ffffaef33cc4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangFrontendTool.so.20.0git+0x3cc4)
#<!-- -->25 0x0000aaaab717e3c4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (./bin/clang+0x123c4)
#<!-- -->26 0x0000aaaab717aee0 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x0000ffffab74d3cc void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000ffffa9200de0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libLLVMSupport.so.20.0git+0xc0de0)
#<!-- -->29 0x0000ffffab74c600 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0xcc600)
#<!-- -->30 0x0000ffffab710690 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0x90690)
#<!-- -->31 0x0000ffffab710878 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0x90878)
#<!-- -->32 0x0000ffffab729728 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/home/david.spickett/build-llvm-aarch64/bin/../lib/libclangDriver.so.20.0git+0xa9728)
#<!-- -->33 0x0000aaaab717a30c clang_main(int, char**, llvm::ToolContext const&amp;) (./bin/clang+0xe30c)
#<!-- -->34 0x0000aaaab7187c44 main (./bin/clang+0x1bc44)
#<!-- -->35 0x0000ffffa8cb73fc __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->36 0x0000ffffa8cb74cc call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->37 0x0000ffffa8cb74cc __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->38 0x0000aaaab7178bf0 _start (./bin/clang+0xcbf0)
clang: error: clang frontend command failed with exit code 70 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git be0b1142df7733633354ef1f73d0379bcd2ccb54)
Target: aarch64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/david.spickett/build-llvm-aarch64/bin
Build config: +assertions
clang: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-b8124c.c
clang: note: diagnostic msg: /tmp/test-b8124c.sh
clang: note: diagnostic msg:

********************
```
If I remove the assignment, the error does not happen.
</details>


---

