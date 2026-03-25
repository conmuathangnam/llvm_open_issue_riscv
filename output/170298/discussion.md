# [RISCV] Crash at -O2: SubRange for this mask not found

**Author:** cardigan1008
**URL:** https://github.com/llvm/llvm-project/issues/170298

## Body

This code crashed at `-O2`:

```c
static int a;
#include "riscv_vector.h"
int b(short c) {
  char e[] = {2000000000};
  char *f = e;
  short g = c;
  long h = 0;
  __rvv_bool4_t i;
  __rvv_uint16m4_t j = __riscv_vmv_s_x_u16m4(g, h);
  long k;
  __rvv_int8m2x3_t l ;
  __riscv_vsoxseg3ei16_v_i8m2x3_m(i, e, j, l, k);
}
int n(int o) {
  int p = a;
  a = o;
  o + p &&b(9);
}
int q() {
  {
    int p = a;
    a = 0;
    p &&b(9);
  }
}
int r();
void s(int t, int u, int v) {
  if (r())
    v = 1;
  v + t - u &&q();
}
void main() {
  s(59, 2, 47);
  n(7);
  r();
  {
    a&&b(9);
  {
      char d = 0;
      char e[] = {2, d, d, d, d, d, d, d, d, d};
      char *f = e;
      short g = 9;
      long h = 0;
      __rvv_bool4_t i;
      __rvv_uint16m4_t j = __riscv_vmv_s_x_u16m4(g, h);
      long k;
      __rvv_int8m2x3_t l = __riscv_vlseg3e8ff_v_i8m2x3(f, &k, h);
      __riscv_vsoxseg3ei16_v_i8m2x3_m(i, e, j, l, k);
    }
  }
}
```

Compiler Explorer: https://godbolt.org/z/n95EqK1YT

Crash:

```sh
SubRange for this mask not found
UNREACHABLE executed at /root/llvm-project/llvm/lib/CodeGen/SplitKit.cpp:402!
```

Backtrace:

```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --target=riscv64-unknown-linux-gnu -march=rv64gcv -mabi=lp64d -menable-experimental-extensions -O2 <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'Greedy Register Allocator' on function '@main'
 #0 0x000000000427b8f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x427b8f8)
 #1 0x0000000004278d24 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4278d24)
 #2 0x00000000041bca58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000719e72c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000719e72c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000719e72c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000719e72c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00000000041c872a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41c872a)
 #8 0x00000000037dc79c llvm::SplitEditor::extendPHIKillRanges() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37dc79c)
 #9 0x00000000037e71cb llvm::SplitEditor::finish(llvm::SmallVectorImpl<unsigned int>*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37e71cb)
#10 0x000000000371442b llvm::RAGreedy::tryBlockSplit(llvm::LiveInterval const&, llvm::AllocationOrder&, llvm::SmallVectorImpl<llvm::Register>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x371442b)
#11 0x0000000003720491 llvm::RAGreedy::trySplit(llvm::LiveInterval const&, llvm::AllocationOrder&, llvm::SmallVectorImpl<llvm::Register>&, llvm::SmallSet<llvm::Register, 16u, std::less<llvm::Register>> const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3720491)
#12 0x0000000003727449 llvm::RAGreedy::selectOrSplitImpl(llvm::LiveInterval const&, llvm::SmallVectorImpl<llvm::Register>&, llvm::SmallSet<llvm::Register, 16u, std::less<llvm::Register>>&, llvm::SmallVector<std::pair<llvm::LiveInterval const*, llvm::MCRegister>, 8u>&, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3727449)
#13 0x0000000003727cd7 llvm::RAGreedy::selectOrSplit(llvm::LiveInterval const&, llvm::SmallVectorImpl<llvm::Register>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3727cd7)
#14 0x00000000039a6aa0 llvm::RegAllocBase::allocatePhysRegs() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x39a6aa0)
#15 0x00000000037238ab llvm::RAGreedy::run(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37238ab)
#16 0x00000000037244fa (anonymous namespace)::RAGreedyLegacy::runOnMachineFunction(llvm::MachineFunction&) RegAllocGreedy.cpp:0:0
#17 0x000000000352f5d9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#18 0x0000000003b74126 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b74126)
#19 0x0000000003b744d1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b744d1)
#20 0x0000000003b74d3f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b74d3f)
#21 0x0000000004539e66 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4539e66)
#22 0x0000000004bcac77 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bcac77)
#23 0x0000000006962ddc clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6962ddc)
#24 0x0000000004bcb0d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bcb0d8)
#25 0x0000000004eb6945 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4eb6945)
#26 0x0000000004e36afe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4e36afe)
#27 0x0000000004faf97d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4faf97d)
#28 0x0000000000dc7740 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc7740)
#29 0x0000000000dbe1ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#30 0x0000000000dbe33d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#31 0x0000000004c338c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#32 0x00000000041bcef4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41bcef4)
#33 0x0000000004c33edf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#34 0x0000000004bf46d2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bf46d2)
#35 0x0000000004bf567e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bf567e)
#36 0x0000000004bfcab5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bfcab5)
#37 0x0000000000dc3b61 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc3b61)
#38 0x0000000000c72574 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc72574)
#39 0x0000719e72c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#40 0x0000719e72c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#41 0x0000000000dbdc55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdbdc55)
```


## Comments

### Comment 1 - cardigan1008

Slightly simplified version:

```c
static int a;
#include "riscv_vector.h"
long c;
int b(short p1) {
  char e[] = {};
  *e;
  short g = p1;
  long h = 0;
  __rvv_bool4_t i;
  __rvv_uint16m4_t j = __riscv_vmv_s_x_u16m4(g, h);
  __rvv_int8m2x3_t l;
  __riscv_vsoxseg3ei16_v_i8m2x3_m(i, e, j, l, c);
}
int m(int p1) {
  int n = a;
  a = p1;
  p1 + n &&b(9);
}
int o() {
  {
    int n = a;
    a = 0;
    n &&b(9);
  }
}
int p();
void q(int p1, int r, int s) {
  if (p())
    s = 1;
  s + p1 - r &&o();
}
void main() {
  q(9, 2, 7);
  m(7);
  p();
  {
    a &&b(9);
    {
      char d = 0;
      char e[] = {2, d, d, d, d, d, d, d, d, d};
      char *f = e;
      short g = 9;
      long h = 0, k;
      __rvv_bool4_t i;
      __rvv_uint16m4_t j = __riscv_vmv_s_x_u16m4(g, h);
      __rvv_int8m2x3_t l = __riscv_vlseg3e8ff_v_i8m2x3(f, &k, h);
      __riscv_vsoxseg3ei16_v_i8m2x3_m(i, e, j, l, k);
    }
  }
}

```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Yunbo Ni (cardigan1008)

<details>
This code crashed at `-O2`:

```c
static int a;
#include "riscv_vector.h"
int b(short c) {
  char e[] = {2000000000};
  char *f = e;
  short g = c;
  long h = 0;
  __rvv_bool4_t i;
  __rvv_uint16m4_t j = __riscv_vmv_s_x_u16m4(g, h);
  long k;
  __rvv_int8m2x3_t l ;
  __riscv_vsoxseg3ei16_v_i8m2x3_m(i, e, j, l, k);
}
int n(int o) {
  int p = a;
  a = o;
  o + p &amp;&amp;b(9);
}
int q() {
  {
    int p = a;
    a = 0;
    p &amp;&amp;b(9);
  }
}
int r();
void s(int t, int u, int v) {
  if (r())
    v = 1;
  v + t - u &amp;&amp;q();
}
void main() {
  s(59, 2, 47);
  n(7);
  r();
  {
    a&amp;&amp;b(9);
  {
      char d = 0;
      char e[] = {2, d, d, d, d, d, d, d, d, d};
      char *f = e;
      short g = 9;
      long h = 0;
      __rvv_bool4_t i;
      __rvv_uint16m4_t j = __riscv_vmv_s_x_u16m4(g, h);
      long k;
      __rvv_int8m2x3_t l = __riscv_vlseg3e8ff_v_i8m2x3(f, &amp;k, h);
      __riscv_vsoxseg3ei16_v_i8m2x3_m(i, e, j, l, k);
    }
  }
}
```

Compiler Explorer: https://godbolt.org/z/n95EqK1YT

Crash:

```sh
SubRange for this mask not found
UNREACHABLE executed at /root/llvm-project/llvm/lib/CodeGen/SplitKit.cpp:402!
```

Backtrace:

```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --target=riscv64-unknown-linux-gnu -march=rv64gcv -mabi=lp64d -menable-experimental-extensions -O2 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
4.	Running pass 'Greedy Register Allocator' on function '@<!-- -->main'
 #<!-- -->0 0x000000000427b8f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x427b8f8)
 #<!-- -->1 0x0000000004278d24 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4278d24)
 #<!-- -->2 0x00000000041bca58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000719e72c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000719e72c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000719e72c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000719e72c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00000000041c872a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41c872a)
 #<!-- -->8 0x00000000037dc79c llvm::SplitEditor::extendPHIKillRanges() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37dc79c)
 #<!-- -->9 0x00000000037e71cb llvm::SplitEditor::finish(llvm::SmallVectorImpl&lt;unsigned int&gt;*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37e71cb)
#<!-- -->10 0x000000000371442b llvm::RAGreedy::tryBlockSplit(llvm::LiveInterval const&amp;, llvm::AllocationOrder&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x371442b)
#<!-- -->11 0x0000000003720491 llvm::RAGreedy::trySplit(llvm::LiveInterval const&amp;, llvm::AllocationOrder&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;, llvm::SmallSet&lt;llvm::Register, 16u, std::less&lt;llvm::Register&gt;&gt; const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3720491)
#<!-- -->12 0x0000000003727449 llvm::RAGreedy::selectOrSplitImpl(llvm::LiveInterval const&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;, llvm::SmallSet&lt;llvm::Register, 16u, std::less&lt;llvm::Register&gt;&gt;&amp;, llvm::SmallVector&lt;std::pair&lt;llvm::LiveInterval const*, llvm::MCRegister&gt;, 8u&gt;&amp;, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3727449)
#<!-- -->13 0x0000000003727cd7 llvm::RAGreedy::selectOrSplit(llvm::LiveInterval const&amp;, llvm::SmallVectorImpl&lt;llvm::Register&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3727cd7)
#<!-- -->14 0x00000000039a6aa0 llvm::RegAllocBase::allocatePhysRegs() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x39a6aa0)
#<!-- -->15 0x00000000037238ab llvm::RAGreedy::run(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x37238ab)
#<!-- -->16 0x00000000037244fa (anonymous namespace)::RAGreedyLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) RegAllocGreedy.cpp:0:0
#<!-- -->17 0x000000000352f5d9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->18 0x0000000003b74126 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b74126)
#<!-- -->19 0x0000000003b744d1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b744d1)
#<!-- -->20 0x0000000003b74d3f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3b74d3f)
#<!-- -->21 0x0000000004539e66 clang::emitBackendOutput(clang::CompilerInstance&amp;, clang::CodeGenOptions&amp;, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;, std::unique_ptr&lt;llvm::raw_pwrite_stream, std::default_delete&lt;llvm::raw_pwrite_stream&gt;&gt;, clang::BackendConsumer*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4539e66)
#<!-- -->22 0x0000000004bcac77 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bcac77)
#<!-- -->23 0x0000000006962ddc clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6962ddc)
#<!-- -->24 0x0000000004bcb0d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bcb0d8)
#<!-- -->25 0x0000000004eb6945 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4eb6945)
#<!-- -->26 0x0000000004e36afe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4e36afe)
#<!-- -->27 0x0000000004faf97d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4faf97d)
#<!-- -->28 0x0000000000dc7740 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc7740)
#<!-- -->29 0x0000000000dbe1ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->30 0x0000000000dbe33d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->31 0x0000000004c338c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->32 0x00000000041bcef4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41bcef4)
#<!-- -->33 0x0000000004c33edf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->34 0x0000000004bf46d2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bf46d2)
#<!-- -->35 0x0000000004bf567e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bf567e)
#<!-- -->36 0x0000000004bfcab5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bfcab5)
#<!-- -->37 0x0000000000dc3b61 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdc3b61)
#<!-- -->38 0x0000000000c72574 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc72574)
#<!-- -->39 0x0000719e72c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->40 0x0000719e72c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->41 0x0000000000dbdc55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdbdc55)
```

</details>


---

### Comment 3 - XChy

Reduced ir reproducer:
```llvm
; RUN: llc < %s
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64-unknown-linux-musl"

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare <vscale x 16 x i16> @llvm.riscv.vmv.s.x.nxv16i16.i64(<vscale x 16 x i16>, i16, i64) #0

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(write)
declare void @llvm.riscv.vsoxseg3.mask.triscv.vector.tuple_nxv16i8_3t.p0.nxv16i16.nxv16i1.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 3), ptr captures(none), <vscale x 16 x i16>, <vscale x 16 x i1>, i64, i64 immarg) #1

define void @main(i1 %tobool.not) #2 {
entry:
  %0 = tail call <vscale x 16 x i16> @llvm.riscv.vmv.s.x.nxv16i16.i64(<vscale x 16 x i16> poison, i16 9, i64 0)
  call void @llvm.riscv.vsoxseg3.mask.triscv.vector.tuple_nxv16i8_3t.p0.nxv16i16.nxv16i1.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 3) zeroinitializer, ptr null, <vscale x 16 x i16> %0, <vscale x 16 x i1> zeroinitializer, i64 0, i64 3)
  %call = tail call i32 null()
  br i1 %tobool.not, label %n.exit.land.end_crit_edge, label %land.rhs

n.exit.land.end_crit_edge:                        ; preds = %entry
  %.pre = tail call <vscale x 16 x i16> @llvm.riscv.vmv.s.x.nxv16i16.i64(<vscale x 16 x i16> poison, i16 9, i64 0)
  br label %land.end

land.rhs:                                         ; preds = %entry
  %1 = tail call <vscale x 16 x i16> @llvm.riscv.vmv.s.x.nxv16i16.i64(<vscale x 16 x i16> poison, i16 9, i64 0)
  call void @llvm.riscv.vsoxseg3.mask.triscv.vector.tuple_nxv16i8_3t.p0.nxv16i16.nxv16i1.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 3) zeroinitializer, ptr null, <vscale x 16 x i16> %1, <vscale x 16 x i1> zeroinitializer, i64 0, i64 3)
  br label %land.end

land.end:                                         ; preds = %land.rhs, %n.exit.land.end_crit_edge
  %.pre-phi = phi <vscale x 16 x i16> [ %.pre, %n.exit.land.end_crit_edge ], [ %1, %land.rhs ]
  call void @llvm.memset.p0.i64(ptr poison, i8 0, i64 9, i1 false)
  call void @llvm.riscv.vsoxseg3.mask.triscv.vector.tuple_nxv16i8_3t.p0.nxv16i16.nxv16i1.i64(target("riscv.vector.tuple", <vscale x 16 x i8>, 3) zeroinitializer, ptr null, <vscale x 16 x i16> %.pre-phi, <vscale x 16 x i1> zeroinitializer, i64 0, i64 3)
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #3

attributes #0 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(write) }
attributes #2 = { "target-features"="+v" }
attributes #3 = { nocallback nofree nounwind willreturn memory(argmem: write) }
```

---

### Comment 4 - XChy

It seems specific to the greedy register allocator, since `-riscv-rvv-regalloc=basic/fast` works well.

---

### Comment 5 - XChy

I felt hard when debugging RegAllocGreedy and inspecting the annoying dump, and I will be busy these days. If anyone is interested, any insight is appreciated. Or you could just assign this issue yourself. CC @topperc @preames.

The MIR before regalloc:
```mir
0B	bb.0.entry:
	  successors: %bb.1(0x40000000), %bb.2(0x40000000); %bb.1(50.00%), %bb.2(50.00%)
	  liveins: $x10
16B	  %3:gpr = COPY $x10
24B	  %14:gprjalr = COPY $x0
32B	  %4:gpr = ANDI %3:gpr, 1
80B	  undef %34.sub_vrm2_0:vrn3m2 = PseudoVMV_V_I_M2 undef %34.sub_vrm2_0:vrn3m2(tied-def 0), 0, 0, 3, 0
176B	  %13:vr = PseudoVMCLR_M_B4 0, 0
184B	  %34.sub_vrm2_1:vrn3m2 = COPY %34.sub_vrm2_0:vrn3m2
192B	  undef %1.sub_vrm1_0:vrm4 = PseudoVMV_V_I_M1 undef %1.sub_vrm1_0:vrm4(tied-def 0), 9, 0, 4, 0
200B	  %34.sub_vrm2_2:vrn3m2 = COPY %34.sub_vrm2_0:vrn3m2
216B	  $v0 = COPY %13:vr
224B	  PseudoVSOXSEG3EI16_V_M4_M2_MASK %34:vrn3m2, %14:gprjalr, %1:vrm4, killed $v0, 0, 3 :: (store unknown-size, align 1)
240B	  ADJCALLSTACKDOWN 0, 0, implicit-def dead $x2, implicit $x2
256B	  PseudoCALLIndirect %14:gprjalr, <regmask $vlenb $x0 $x1 $x8 $x9 $x18 $x19 $x20 $x21 $x22 $x23 $x24 $x25 $x26 $x27 $f8_d $f9_d $f18_d $f19_d $f20_d $f21_d $f22_d $f23_d $f24_d $f25_d $f26_d $f27_d $f8_f $f9_f $f18_f $f19_f $f20_f $f21_f and 52 more...>, implicit-def dead $x1, implicit-def $x2, implicit-def dead $x10
272B	  ADJCALLSTACKUP 0, 0, implicit-def dead $x2, implicit $x2
288B	  BEQ %4:gpr, $x0, %bb.2
304B	  PseudoBR %bb.1

320B	bb.1.n.exit.land.end_crit_edge:
	; predecessors: %bb.0
	  successors: %bb.3(0x80000000); %bb.3(100.00%)

368B	  PseudoBR %bb.3

384B	bb.2.land.rhs:
	; predecessors: %bb.0
	  successors: %bb.3(0x80000000); %bb.3(100.00%)

448B	  %34.sub_vrm2_1:vrn3m2 = COPY %34.sub_vrm2_0:vrn3m2
496B	  %24:vr = PseudoVMCLR_M_B4 0, 0
504B	  %34.sub_vrm2_2:vrn3m2 = COPY %34.sub_vrm2_0:vrn3m2
528B	  $v0 = COPY %24:vr
544B	  PseudoVSOXSEG3EI16_V_M4_M2_MASK %34:vrn3m2, $x0, %1:vrm4, killed $v0, 0, 3 :: (store unknown-size, align 1)

576B	bb.3.land.end:
	; predecessors: %bb.2, %bb.1

608B	  ADJCALLSTACKDOWN 0, 0, implicit-def dead $x2, implicit $x2
656B	  $x12 = ADDI $x0, 9
664B	  $x11 = COPY $x0
672B	  PseudoCALL target-flags(riscv-call) &memset, <regmask $vlenb $x0 $x1 $x8 $x9 $x18 $x19 $x20 $x21 $x22 $x23 $x24 $x25 $x26 $x27 $f8_d $f9_d $f18_d $f19_d $f20_d $f21_d $f22_d $f23_d $f24_d $f25_d $f26_d $f27_d $f8_f $f9_f $f18_f $f19_f $f20_f $f21_f and 52 more...>, implicit-def dead $x1, implicit undef $x10, implicit $x11, implicit killed $x12, implicit-def $x2, implicit-def dead $x10
688B	  ADJCALLSTACKUP 0, 0, implicit-def dead $x2, implicit $x2
736B	  %34.sub_vrm2_1:vrn3m2 = COPY %34.sub_vrm2_0:vrn3m2
784B	  %38:vr = PseudoVMCLR_M_B4 0, 0
792B	  %34.sub_vrm2_2:vrn3m2 = COPY %34.sub_vrm2_0:vrn3m2
800B	  $v0 = COPY %38:vr
816B	  PseudoVSOXSEG3EI16_V_M4_M2_MASK %34:vrn3m2, $x0, %1:vrm4, killed $v0, 0, 3 :: (store unknown-size, align 1)
832B	  PseudoRET
```

Greedy cannot figure out the SubRange for a specific subreg of the mask in the last `PseudoVSOXSEG3EI16_V_M4_M2_MASK`.

---

