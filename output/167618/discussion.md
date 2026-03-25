# [RISCV][GISel] fatal error: error in backend: Cannot implicitly convert a scalable size to a fixed-width size

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/167618
**Status:** Closed
**Labels:** backend:RISC-V, llvm:globalisel, crash
**Closed Date:** 2025-11-13T05:53:12Z

## Body

 Code from https://github.com/llvm/llvm-project/issues/166613 caused fatal error with `-fglobal-isel`:

```c
#include "riscv_vector.h"
char b;
int i;
int a();
#define c() 0
void h() {
  {
    long d = 1, g;
    int e = a(), f = __builtin_rvv_vsetvli(e + d, 3, 0);
    __riscv_vle8ff_v_i8m1(&b, &g, f);
  }
  {
    a();
    int f = __builtin_rvv_vsetvli(1, 3, 0);
    long g;
    __riscv_vle8ff_v_i8m1(&b, &g, f);
  }
  i ? a() : c();
}
```

Compiler explorer: https://godbolt.org/z/7o3o3831T

Error:

```sh
fatal error: error in backend: Cannot implicitly convert a scalable size to a fixed-width size in `TypeSize::operator ScalarTy()`
```

Backtrace:

```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics --target=riscv64-unknown-linux-gnu -march=rv64gcv -mabi=lp64d -menable-experimental-extensions -O2 -fglobal-isel <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'IRTranslator' on function '@h'
 #0 0x0000000003d25608 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d25608)
 #1 0x0000000003d22fdc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d22fdc)
 #2 0x0000000003c69283 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c69283)
 #3 0x0000000003d1a81e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d1a81e)
 #4 0x0000000000de0f8b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003c734e3 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c734e3)
 #6 0x0000000003c73679 (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c73679)
 #7 0x0000000002f31373 (/opt/compiler-explorer/clang-trunk/bin/clang+0x2f31373)
 #8 0x0000000004fae46e llvm::IRTranslator::getOrCreateVRegs(llvm::Value const&) (.part.0) IRTranslator.cpp:0:0
 #9 0x0000000004faeee1 llvm::IRTranslator::getOrCreateVRegs(llvm::Value const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4faeee1)
#10 0x0000000004fbea3d llvm::IRTranslator::translateIntrinsic(llvm::CallBase const&, unsigned int, llvm::MachineIRBuilder&, llvm::TargetLoweringBase::IntrinsicInfo const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4fbea3d)
#11 0x0000000004fc45da llvm::IRTranslator::translateCall(llvm::User const&, llvm::MachineIRBuilder&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4fc45da)
#12 0x0000000004fc7445 llvm::IRTranslator::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4fc7445)
#13 0x0000000003101b0d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3101b0d)
#14 0x0000000003675ee2 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3675ee2)
#15 0x0000000003676171 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3676171)
#16 0x0000000003677a53 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3677a53)
#17 0x0000000003fc2b12 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fc2b12)
#18 0x00000000045fb6db clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fb6db)
#19 0x000000000627cd9c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x627cd9c)
#20 0x00000000045fc235 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fc235)
#21 0x00000000048fb3fa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x48fb3fa)
#22 0x000000000487aa7b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x487aa7b)
#23 0x00000000049f2b0b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x49f2b0b)
#24 0x0000000000de2fe5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xde2fe5)
#25 0x0000000000ddaf4b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#26 0x0000000000ddafed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#27 0x000000000466a4c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#28 0x0000000003c69183 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c69183)
#29 0x000000000466a6e9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#30 0x000000000462d2e2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x462d2e2)
#31 0x000000000462e1c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x462e1c1)
#32 0x000000000463706c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+0x463706c)
#33 0x0000000000ddfa19 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+0xddfa19)
#34 0x0000000000c89a24 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc89a24)
#35 0x000079e395c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x000079e395c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000dda9e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdda9e5)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Yunbo Ni (cardigan1008)

<details>
 Code from https://github.com/llvm/llvm-project/issues/166613 caused fatal error with `-fglobal-isel`:

```c
#include "riscv_vector.h"
char b;
int i;
int a();
#define c() 0
void h() {
  {
    long d = 1, g;
    int e = a(), f = __builtin_rvv_vsetvli(e + d, 3, 0);
    __riscv_vle8ff_v_i8m1(&amp;b, &amp;g, f);
  }
  {
    a();
    int f = __builtin_rvv_vsetvli(1, 3, 0);
    long g;
    __riscv_vle8ff_v_i8m1(&amp;b, &amp;g, f);
  }
  i ? a() : c();
}
```

Compiler explorer: https://godbolt.org/z/7o3o3831T

Error:

```sh
fatal error: error in backend: Cannot implicitly convert a scalable size to a fixed-width size in `TypeSize::operator ScalarTy()`
```

Backtrace:

```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang -g -o /app/output.s -fno-verbose-asm -S -target riscv64-unknown-linux-gnu -march=rv64gc -mabi=lp64d --gcc-toolchain=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu --sysroot=/opt/compiler-explorer/riscv64/gcc-8.2.0/riscv64-unknown-linux-gnu/riscv64-unknown-linux-gnu/sysroot -fcolor-diagnostics -fno-crash-diagnostics --target=riscv64-unknown-linux-gnu -march=rv64gcv -mabi=lp64d -menable-experimental-extensions -O2 -fglobal-isel &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'IRTranslator' on function '@<!-- -->h'
 #<!-- -->0 0x0000000003d25608 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d25608)
 #<!-- -->1 0x0000000003d22fdc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d22fdc)
 #<!-- -->2 0x0000000003c69283 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c69283)
 #<!-- -->3 0x0000000003d1a81e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3d1a81e)
 #<!-- -->4 0x0000000000de0f8b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #<!-- -->5 0x0000000003c734e3 llvm::report_fatal_error(llvm::Twine const&amp;, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c734e3)
 #<!-- -->6 0x0000000003c73679 (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c73679)
 #<!-- -->7 0x0000000002f31373 (/opt/compiler-explorer/clang-trunk/bin/clang+0x2f31373)
 #<!-- -->8 0x0000000004fae46e llvm::IRTranslator::getOrCreateVRegs(llvm::Value const&amp;) (.part.0) IRTranslator.cpp:0:0
 #<!-- -->9 0x0000000004faeee1 llvm::IRTranslator::getOrCreateVRegs(llvm::Value const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4faeee1)
#<!-- -->10 0x0000000004fbea3d llvm::IRTranslator::translateIntrinsic(llvm::CallBase const&amp;, unsigned int, llvm::MachineIRBuilder&amp;, llvm::TargetLoweringBase::IntrinsicInfo const*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4fbea3d)
#<!-- -->11 0x0000000004fc45da llvm::IRTranslator::translateCall(llvm::User const&amp;, llvm::MachineIRBuilder&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4fc45da)
#<!-- -->12 0x0000000004fc7445 llvm::IRTranslator::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x4fc7445)
#<!-- -->13 0x0000000003101b0d llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3101b0d)
#<!-- -->14 0x0000000003675ee2 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3675ee2)
#<!-- -->15 0x0000000003676171 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3676171)
#<!-- -->16 0x0000000003677a53 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3677a53)
#<!-- -->17 0x0000000003fc2b12 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3fc2b12)
#<!-- -->18 0x00000000045fb6db clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fb6db)
#<!-- -->19 0x000000000627cd9c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+0x627cd9c)
#<!-- -->20 0x00000000045fc235 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+0x45fc235)
#<!-- -->21 0x00000000048fb3fa clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+0x48fb3fa)
#<!-- -->22 0x000000000487aa7b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x487aa7b)
#<!-- -->23 0x00000000049f2b0b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+0x49f2b0b)
#<!-- -->24 0x0000000000de2fe5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+0xde2fe5)
#<!-- -->25 0x0000000000ddaf4b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->26 0x0000000000ddafed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->27 0x000000000466a4c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->28 0x0000000003c69183 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x3c69183)
#<!-- -->29 0x000000000466a6e9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->30 0x000000000462d2e2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x462d2e2)
#<!-- -->31 0x000000000462e1c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+0x462e1c1)
#<!-- -->32 0x000000000463706c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0x463706c)
#<!-- -->33 0x0000000000ddfa19 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+0xddfa19)
#<!-- -->34 0x0000000000c89a24 main (/opt/compiler-explorer/clang-trunk/bin/clang+0xc89a24)
#<!-- -->35 0x000079e395c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->36 0x000079e395c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->37 0x0000000000dda9e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+0xdda9e5)
```
</details>


---

