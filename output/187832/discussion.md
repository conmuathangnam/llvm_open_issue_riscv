# [LoongArch] Crash in LoongArchTargetLowering::LowerCallTo during ThinLTO linking of ldc compiler

**Author:** wszqkzqk
**URL:** https://github.com/llvm/llvm-project/issues/187832
**Status:** Closed
**Labels:** crash, LTO, backend:loongarch, needs-reduction, llvm:SelectionDAG
**Closed Date:** 2026-03-25T15:26:10Z

## Body

## Description

LLD/LLVM 22.1.1 crashes with segmentation fault during ThinLTO linking when building LDC (D Language Compiler) on LoongArch64. The crash occurs in the `LoongArch DAG->DAG Pattern Instruction Selection` pass, specifically in `llvm::TargetLowering::LowerCallTo`.

### Environment

- **LLVM Version**: 22.1.1
- **Linker**: LLD 22.1.1
- **Target Architecture**: LoongArch64 (loongarch64-unknown-linux-gnu)
- **Operating System**: Arch Linux for Loong64
- **C++ Compiler**: GCC 15.2.1
- **Linker**: LLD 22.1.1

### Steps to Reproduce

* Set compile flags
```bash
CFLAGS="-mabi=lp64d -march=loongarch64 -mlsx -O2 -pipe -fno-plt -fexceptions -mcmodel=medium \
        -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security \
        -fstack-clash-protection \
        -fno-omit-frame-pointer"
CXXFLAGS="$CFLAGS -Wp,-D_GLIBCXX_ASSERTIONS"
LDFLAGS="-Wl,-O1 -Wl,--sort-common -Wl,--as-needed -Wl,-z,relro -Wl,-z,now"
```
* Clone LDC repository: `https://github.com/ldc-developers/ldc#commit=3d5bafdf0e6f0d4619f04baad80d2b8afb6ba8ad`
* Configure with CMake:
```bash
cmake -GNinja \
  -DCMAKE_INSTALL_PREFIX=/usr \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_SKIP_RPATH=ON \
  -DINCLUDE_INSTALL_DIR=/usr/include/dlang/ldc \
  -DBUILD_SHARED_LIBS=BOTH \
  -DBUILD_LTO_LIBS=ON \
  -DLDC_WITH_LLD=OFF \
  -DD_COMPILER_FLAGS="-link-defaultlib-shared=false -linker=lld --flto=thin" \
  -DADDITIONAL_DEFAULT_LDC_SWITCHES="\"-link-defaultlib-shared\"," \
  ..
```
* Build with Ninja: `ninja`
* Crash occurs during linking of `timetrace2txt` executable

### Stack Dump

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0. Running pass 'Function Pass Manager' on module 'obj/timetrace2txt.o'.
1. Running pass 'LoongArch DAG->DAG Pattern Instruction Selection' on function '@_D3std6format8internal6floats__T10printFloatTSQBsQBr__T7sformatTaTxdZQoFNkMAaMAxaxdZ4SinkTdTaZQCkFNaNfKQCgxdSQEdQEc4spec__T10FormatSpecTaZQpZv'
```

**Full Backtrace**:
```
0 libLLVM.so.22.1 0x00007fffe909ec64 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 68
1 libLLVM.so.22.1 0x00007fffe909b8a4
2 linux-vdso.so.1 0x00007ffffcd5d188 __vdso_rt_sigreturn + 0
3 libLLVM.so.22.1 0x00007fffed4db950
4 libLLVM.so.22.1 0x00007fffed4f37f0
5 libLLVM.so.22.1 0x00007fffe9dc6cdc llvm::TargetLowering::LowerCallTo(llvm::TargetLowering::CallLoweringInfo&) const + 9428
6 libLLVM.so.22.1 0x00007fffe9dd4e1c llvm::SelectionDAGBuilder::lowerInvokable(llvm::TargetLowering::CallLoweringInfo&, llvm::BasicBlock const*) + 244
7 libLLVM.so.22.1 0x00007fffe9deaa4c llvm::SelectionDAGBuilder::LowerCallTo(llvm::CallBase const&, llvm::SDValue, bool, bool, llvm::BasicBlock const*, llvm::TargetLowering::PtrAuthInfo const*) + 3620
8 libLLVM.so.22.1 0x00007fffe9e1a004 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) + 700
9 libLLVM.so.22.1 0x00007fffe9eb1438 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, false, false, void, true, llvm::BasicBlock>, false, true>, bool&) + 340
10 libLLVM.so.22.1 0x00007fffe9eb24dc llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) + 4176
11 libLLVM.so.22.1 0x00007fffe9eb4330 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) + 228
12 libLLVM.so.22.1 0x00007fffe9ea2ca0 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) + 448
13 libLLVM.so.22.1 0x00007fffe9728e3c llvm::MachineFunctionPass::runOnFunction(llvm::Function&) + 436
14 libLLVM.so.22.1 0x00007fffe92c7b48 llvm::FPPassManager::runOnFunction(llvm::Function&) + 1508
15 libLLVM.so.22.1 0x00007fffe92c7d34 llvm::FPPassManager::runOnModule(llvm::Module&) + 80
16 libLLVM.so.22.1 0x00007fffe92c73f0 llvm::legacy::PassManagerImpl::run(llvm::Module&) + 2020
17 libLLVM.so.22.1 0x00007fffeb6eb4e8
18 libLLVM.so.22.1 0x00007fffeb6ec5fc
19 libLLVM.so.22.1 0x00007fffeb6ed134 llvm::lto::thinBackend(llvm::lto::Config const&, unsigned int, std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, llvm::Module&, llvm::ModuleSummaryIndex const&, llvm::FunctionImporter::ImportMapTy const&, llvm::DenseMap<unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo<unsigned long, void>, llvm::detail::DenseMapPair<unsigned long, llvm::GlobalValueSummary*>> const&, llvm::MapVector<llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int>>, llvm::SmallVector<std::pair<llvm::StringRef, llvm::BitcodeModule>, 0u>>*, bool, std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, std::vector<unsigned char, std::allocator<unsigned char>> const&) + 2380
20 libLLVM.so.22.1 0x00007fffeb6c1abc
21 libLLVM.so.22.1 0x00007fffeb6dbd78
22 libLLVM.so.22.1 0x00007fffeb6cac74
23 libLLVM.so.22.1 0x00007fffe8f08a04
24 libLLVM.so.22.1 0x00007fffe8f08868
25 libc.so.6 0x00007fffe7faa4bc
26 libc.so.6 0x00007fffe7faa5a8 pthread_once + 68
27 libLLVM.so.22.1 0x00007fffe8f08584
28 libLLVM.so.22.1 0x00007fffe8f082dc
29 libLLVM.so.22.1 0x00007fffe8fff788 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) + 508
30 libLLVM.so.22.1 0x00007fffe9000580
31 libc.so.6 0x00007fffe7fa33a4
32 libc.so.6 0x00007fffe8024e18
collect2: fatal error: ld terminated with signal 11 [Segmentation fault], core dumped
```

### Workarounds

Currently, the build can complete by using LLD/LLVM 21 instead of LLD22/LLVM 22.


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-loongarch

Author: wszqkzqk (wszqkzqk)

<details>
## Description

LLD/LLVM 22.1.1 crashes with segmentation fault during ThinLTO linking when building LDC (D Language Compiler) on LoongArch64. The crash occurs in the `LoongArch DAG-&gt;DAG Pattern Instruction Selection` pass, specifically in `llvm::TargetLowering::LowerCallTo`.

### Environment

- **LLVM Version**: 22.1.1
- **Linker**: LLD 22.1.1
- **Target Architecture**: LoongArch64 (loongarch64-unknown-linux-gnu)
- **Operating System**: Arch Linux for Loong64
- **C++ Compiler**: GCC 15.2.1
- **Linker**: LLD 22.1.1

### Stack Dump

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0. Running pass 'Function Pass Manager' on module 'obj/timetrace2txt.o'.
1. Running pass 'LoongArch DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_D3std6format8internal6floats__T10printFloatTSQBsQBr__T7sformatTaTxdZQoFNkMAaMAxaxdZ4SinkTdTaZQCkFNaNfKQCgxdSQEdQEc4spec__T10FormatSpecTaZQpZv'
```

**Full Backtrace**:
```
0 libLLVM.so.22.1 0x00007fffe909ec64 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 68
1 libLLVM.so.22.1 0x00007fffe909b8a4
2 linux-vdso.so.1 0x00007ffffcd5d188 __vdso_rt_sigreturn + 0
3 libLLVM.so.22.1 0x00007fffed4db950
4 libLLVM.so.22.1 0x00007fffed4f37f0
5 libLLVM.so.22.1 0x00007fffe9dc6cdc llvm::TargetLowering::LowerCallTo(llvm::TargetLowering::CallLoweringInfo&amp;) const + 9428
6 libLLVM.so.22.1 0x00007fffe9dd4e1c llvm::SelectionDAGBuilder::lowerInvokable(llvm::TargetLowering::CallLoweringInfo&amp;, llvm::BasicBlock const*) + 244
7 libLLVM.so.22.1 0x00007fffe9deaa4c llvm::SelectionDAGBuilder::LowerCallTo(llvm::CallBase const&amp;, llvm::SDValue, bool, bool, llvm::BasicBlock const*, llvm::TargetLowering::PtrAuthInfo const*) + 3620
8 libLLVM.so.22.1 0x00007fffe9e1a004 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) + 700
9 libLLVM.so.22.1 0x00007fffe9eb1438 llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, false, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) + 340
10 libLLVM.so.22.1 0x00007fffe9eb24dc llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) + 4176
11 libLLVM.so.22.1 0x00007fffe9eb4330 llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) + 228
12 libLLVM.so.22.1 0x00007fffe9ea2ca0 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) + 448
13 libLLVM.so.22.1 0x00007fffe9728e3c llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) + 436
14 libLLVM.so.22.1 0x00007fffe92c7b48 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) + 1508
15 libLLVM.so.22.1 0x00007fffe92c7d34 llvm::FPPassManager::runOnModule(llvm::Module&amp;) + 80
16 libLLVM.so.22.1 0x00007fffe92c73f0 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) + 2020
17 libLLVM.so.22.1 0x00007fffeb6eb4e8
18 libLLVM.so.22.1 0x00007fffeb6ec5fc
19 libLLVM.so.22.1 0x00007fffeb6ed134 llvm::lto::thinBackend(llvm::lto::Config const&amp;, unsigned int, std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, llvm::Module&amp;, llvm::ModuleSummaryIndex const&amp;, llvm::FunctionImporter::ImportMapTy const&amp;, llvm::DenseMap&lt;unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo&lt;unsigned long, void&gt;, llvm::detail::DenseMapPair&lt;unsigned long, llvm::GlobalValueSummary*&gt;&gt; const&amp;, llvm::MapVector&lt;llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt;&gt;, llvm::SmallVector&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt;, 0u&gt;&gt;*, bool, std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt;&gt;&gt; (unsigned int, llvm::Twine const&amp;)&gt;, std::vector&lt;unsigned char, std::allocator&lt;unsigned char&gt;&gt; const&amp;) + 2380
20 libLLVM.so.22.1 0x00007fffeb6c1abc
21 libLLVM.so.22.1 0x00007fffeb6dbd78
22 libLLVM.so.22.1 0x00007fffeb6cac74
23 libLLVM.so.22.1 0x00007fffe8f08a04
24 libLLVM.so.22.1 0x00007fffe8f08868
25 libc.so.6 0x00007fffe7faa4bc
26 libc.so.6 0x00007fffe7faa5a8 pthread_once + 68
27 libLLVM.so.22.1 0x00007fffe8f08584
28 libLLVM.so.22.1 0x00007fffe8f082dc
29 libLLVM.so.22.1 0x00007fffe8fff788 llvm::StdThreadPool::processTasks(llvm::ThreadPoolTaskGroup*) + 508
30 libLLVM.so.22.1 0x00007fffe9000580
31 libc.so.6 0x00007fffe7fa33a4
32 libc.so.6 0x00007fffe8024e18
collect2: fatal error: ld terminated with signal 11 [Segmentation fault], core dumped
```

### Workarounds

Currently, the build can complete by using LLD/LLVM 21 instead of LLD22/LLVM 22.

</details>


---

### Comment 2 - EugeneZelenko

@wszqkzqk: Could you please provide reproducer?

---

### Comment 3 - wszqkzqk

> [@wszqkzqk](https://github.com/wszqkzqk): Could you please provide reproducer?

OK.

* Clone LDC repository: `https://github.com/ldc-developers/ldc#commit=3d5bafdf0e6f0d4619f04baad80d2b8afb6ba8ad`
* Configure with CMake:
```bash
cmake -GNinja \
  -DCMAKE_INSTALL_PREFIX=/usr \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_SKIP_RPATH=ON \
  -DINCLUDE_INSTALL_DIR=/usr/include/dlang/ldc \
  -DBUILD_SHARED_LIBS=BOTH \
  -DBUILD_LTO_LIBS=ON \
  -DLDC_WITH_LLD=OFF \
  -DD_COMPILER_FLAGS="-link-defaultlib-shared=false -linker=lld --flto=thin" \
  -DADDITIONAL_DEFAULT_LDC_SWITCHES="\"-link-defaultlib-shared\"," \
  ..
```
* Build with Ninja: `ninja`
* Crash occurs during linking of `timetrace2txt` executable

```bash
CFLAGS="-mabi=lp64d -march=loongarch64 -mlsx -O2 -pipe -fno-plt -fexceptions -mcmodel=medium \
        -Wp,-D_FORTIFY_SOURCE=3 -Wformat -Werror=format-security \
        -fstack-clash-protection \
        -fno-omit-frame-pointer"
CXXFLAGS="$CFLAGS -Wp,-D_GLIBCXX_ASSERTIONS"
LDFLAGS="-Wl,-O1 -Wl,--sort-common -Wl,--as-needed -Wl,-z,relro -Wl,-z,now"
```

---

