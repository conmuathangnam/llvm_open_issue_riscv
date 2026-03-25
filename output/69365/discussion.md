# clang++ crash: Assertion `(!Is64Bit || isInt<32>((long long)FIOffset + Imm)) && "Requesting 64-bit offset in 32-bit immediate!"' failed

**Author:** YangChenyuan
**URL:** https://github.com/llvm/llvm-project/issues/69365
**Status:** Closed
**Labels:** backend:X86, llvm:codegen, crash
**Closed Date:** 2025-08-11T15:53:46Z

## Body

When compiling the following program with `clang++ test.cpp -o exe`

```cpp
#include <iostream>

void may_throw() {
  while(true) {
    throw std::out_of_range("test");
  }
}

int main() { 
  try {
    may_throw();
    volatile int a[1000000000] = {};
    return 1/ (a[500000000] & 1);
  } catch (...) {
    return -1;
  }
}
```

I encountered the following crash

```
clang-18: /llvm-project/llvm/lib/Target/X86/X86RegisterInfo.cpp:900: virtual bool llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger*) const: Assertion `(!Is64Bit || isInt<32>((long long)FIOffset + Imm)) && "Requesting 64-bit offset in 32-bit immediate!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Chenyuan Yang (YangChenyuan)

<details>
When compiling the following program with `clang++ test.cpp -o exe`

```cpp
#include &lt;iostream&gt;

void may_throw() {
  while(true) {
    throw std::out_of_range("test");
  }
}

int main() { 
  try {
    may_throw();
    volatile int a[1000000000] = {};
    return 1/ (a[500000000] &amp; 1);
  } catch (...) {
    return -1;
  }
}
```

I encountered the following crash

```
clang-18: /llvm-project/llvm/lib/Target/X86/X86RegisterInfo.cpp:900: virtual bool llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger*) const: Assertion `(!Is64Bit || isInt&lt;32&gt;((long long)FIOffset + Imm)) &amp;&amp; "Requesting 64-bit offset in 32-bit immediate!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
```
</details>


---

### Comment 2 - shafik

Confirmed: https://godbolt.org/z/b4zqsrfxP

Assertion:

```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/BitVector.h:365:
llvm::BitVector& llvm::BitVector::set(unsigned int, unsigned int):
Assertion `I <= E && "Attempted to set backwards range!"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -O3 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Assignment Tracking Analysis' on function '@main'
 #0 0x000000000372dc98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x372dc98)
 #1 0x000000000372b95c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x372b95c)
 #2 0x0000000003674298 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f6b7e8c6420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f6b7e38900b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f6b7e368859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f6b7e368729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f6b7e379fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000000002f36da5 removeRedundantDbgLocsUsingBackwardScan(llvm::BasicBlock const*, FunctionVarLocsBuilder&) AssignmentTrackingAnalysis.cpp:0:0
 #9 0x0000000002f49dd9 llvm::AssignmentTrackingAnalysis::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x2f49dd9)
#10 0x00000000030b1439 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x30b1439)
#11 0x00000000030b1671 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x30b1671)
#12 0x00000000030b1e92 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x30b1e92)
#13 0x00000000039a2462 (anonymous namespace)::EmitAssemblyHelper::EmitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>) BackendUtil.cpp:0:0
#14 0x00000000039a2909 clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39a2909)
#15 0x000000000499ab1f clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499ab1f)
#16 0x0000000005e9ffe9 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5e9ffe9)
#17 0x0000000004999d08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4999d08)
#18 0x00000000041fd819 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41fd819)
#19 0x000000000417ecbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x417ecbe)
#20 0x00000000042dc99e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42dc99e)
#21 0x0000000000befa36 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbefa36)
#22 0x0000000000be72fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#23 0x0000000003fdaea9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#24 0x0000000003674744 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3674744)
#25 0x0000000003fdb49f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#26 0x0000000003fa3665 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa3665)
#27 0x0000000003fa40cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fa40cd)
#28 0x0000000003fabff5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fabff5)
#29 0x0000000000becedc clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbecedc)
#30 0x0000000000ae7071 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xae7071)
#31 0x00007f6b7e36a083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#32 0x0000000000be6dde _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbe6dde)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Chenyuan Yang (YangChenyuan)

<details>
When compiling the following program with `clang++ test.cpp -o exe`

```cpp
#include &lt;iostream&gt;

void may_throw() {
  while(true) {
    throw std::out_of_range("test");
  }
}

int main() { 
  try {
    may_throw();
    volatile int a[1000000000] = {};
    return 1/ (a[500000000] &amp; 1);
  } catch (...) {
    return -1;
  }
}
```

I encountered the following crash

```
clang-18: /llvm-project/llvm/lib/Target/X86/X86RegisterInfo.cpp:900: virtual bool llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger*) const: Assertion `(!Is64Bit || isInt&lt;32&gt;((long long)FIOffset + Imm)) &amp;&amp; "Requesting 64-bit offset in 32-bit immediate!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
```
</details>


---

