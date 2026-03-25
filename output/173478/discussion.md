# [AArch64] Assertion `VT.isVector() == N1.getValueType().isVector() && "TRUNCATE result type type should be vector iff the operand " "type is vector!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173478
**Status:** Open
**Labels:** backend:AArch64, crash

## Body

Reproducer:
https://godbolt.org/z/7xxPd1xjY

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:6673: llvm::SDValue llvm::SelectionDAG::getNode(unsigned int, const llvm::SDLoc&, llvm::EVT, llvm::SDValue, llvm::SDNodeFlags): Assertion `VT.isVector() == N1.getValueType().isVector() && "TRUNCATE result type type should be vector iff the operand " "type is vector!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@f_si_qi_0'
 #0 0x000000000423f888 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423f888)
 #1 0x000000000423c734 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000709ca7642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000709ca76969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000709ca7642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000709ca76287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000709ca762871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000709ca7639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003f9779d llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&, llvm::EVT, llvm::SDValue, llvm::SDNodeFlags) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f9779d)
 #9 0x0000000003ee83e5 getCopyFromParts(llvm::SelectionDAG&, llvm::SDLoc const&, llvm::SDValue const*, unsigned int, llvm::MVT, llvm::EVT, llvm::Value const*, llvm::SDValue, std::optional<unsigned int>, std::optional<llvm::ISD::NodeType>) (.isra.0) SelectionDAGBuilder.cpp:0:0
#10 0x0000000003eea7c7 llvm::RegsForValue::getCopyFromRegs(llvm::SelectionDAG&, llvm::FunctionLoweringInfo&, llvm::SDLoc const&, llvm::SDValue&, llvm::SDValue*, llvm::Value const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eea7c7)
#11 0x0000000003f12f03 llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f12f03)
#12 0x0000000003f308af llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f308af)
#13 0x0000000003f44c32 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f44c32)
#14 0x0000000003fd774d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, llvm::ilist_iterator_w_bits<llvm::ilist_detail::node_options<llvm::Instruction, true, false, void, true, llvm::BasicBlock>, false, true>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd774d)
#15 0x0000000003fd81b8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd81b8)
#16 0x0000000003fd9cec llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd9cec)
#17 0x0000000003fc557f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fc557f)
#18 0x0000000003117f29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#19 0x000000000376a706 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376a706)
#20 0x000000000376aab1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376aab1)
#21 0x000000000376b31f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376b31f)
#22 0x000000000090253d compileModule(char**, llvm::SmallVectorImpl<llvm::PassPlugin>&, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#23 0x00000000007cbc1c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7cbc1c)
#24 0x0000709ca7629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x0000709ca7629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x00000000008f7215 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f7215)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
typedef unsigned char v16qi __attribute__((vector_size(16)));

struct si_qi_1 { unsigned char c[2]; unsigned short x; };

#define TEST(NAME, STYPE, VTYPE, LHS, RHS)      \
  void                                          \
  NAME (VTYPE x)                                \
  {                                             \
    register struct STYPE y asm ("v1");         \
    asm volatile ("" : "=w" (y));               \
    LHS = RHS;                                  \
    asm volatile ("" :: "w" (y));               \
  }

TEST (f_si_qi_0, si_qi_1, v16qi, y.c[0], x[0])
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/7xxPd1xjY

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:6673: llvm::SDValue llvm::SelectionDAG::getNode(unsigned int, const llvm::SDLoc&amp;, llvm::EVT, llvm::SDValue, llvm::SDNodeFlags): Assertion `VT.isVector() == N1.getValueType().isVector() &amp;&amp; "TRUNCATE result type type should be vector iff the operand " "type is vector!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->f_si_qi_0'
 #<!-- -->0 0x000000000423f888 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x423f888)
 #<!-- -->1 0x000000000423c734 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000709ca7642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000709ca76969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000709ca7642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000709ca76287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000709ca762871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000709ca7639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000003f9779d llvm::SelectionDAG::getNode(unsigned int, llvm::SDLoc const&amp;, llvm::EVT, llvm::SDValue, llvm::SDNodeFlags) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f9779d)
 #<!-- -->9 0x0000000003ee83e5 getCopyFromParts(llvm::SelectionDAG&amp;, llvm::SDLoc const&amp;, llvm::SDValue const*, unsigned int, llvm::MVT, llvm::EVT, llvm::Value const*, llvm::SDValue, std::optional&lt;unsigned int&gt;, std::optional&lt;llvm::ISD::NodeType&gt;) (.isra.0) SelectionDAGBuilder.cpp:0:0
#<!-- -->10 0x0000000003eea7c7 llvm::RegsForValue::getCopyFromRegs(llvm::SelectionDAG&amp;, llvm::FunctionLoweringInfo&amp;, llvm::SDLoc const&amp;, llvm::SDValue&amp;, llvm::SDValue*, llvm::Value const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3eea7c7)
#<!-- -->11 0x0000000003f12f03 llvm::SelectionDAGBuilder::visitInlineAsm(llvm::CallBase const&amp;, llvm::BasicBlock const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f12f03)
#<!-- -->12 0x0000000003f308af llvm::SelectionDAGBuilder::visitCall(llvm::CallInst const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f308af)
#<!-- -->13 0x0000000003f44c32 llvm::SelectionDAGBuilder::visit(llvm::Instruction const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3f44c32)
#<!-- -->14 0x0000000003fd774d llvm::SelectionDAGISel::SelectBasicBlock(llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, llvm::ilist_iterator_w_bits&lt;llvm::ilist_detail::node_options&lt;llvm::Instruction, true, false, void, true, llvm::BasicBlock&gt;, false, true&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd774d)
#<!-- -->15 0x0000000003fd81b8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd81b8)
#<!-- -->16 0x0000000003fd9cec llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fd9cec)
#<!-- -->17 0x0000000003fc557f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fc557f)
#<!-- -->18 0x0000000003117f29 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->19 0x000000000376a706 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376a706)
#<!-- -->20 0x000000000376aab1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376aab1)
#<!-- -->21 0x000000000376b31f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x376b31f)
#<!-- -->22 0x000000000090253d compileModule(char**, llvm::SmallVectorImpl&lt;llvm::PassPlugin&gt;&amp;, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->23 0x00000000007cbc1c main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7cbc1c)
#<!-- -->24 0x0000709ca7629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x0000709ca7629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x00000000008f7215 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f7215)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
typedef unsigned char v16qi __attribute__((vector_size(16)));

struct si_qi_1 { unsigned char c[2]; unsigned short x; };

#define TEST(NAME, STYPE, VTYPE, LHS, RHS)      \
  void                                          \
  NAME (VTYPE x)                                \
  {                                             \
    register struct STYPE y asm ("v1");         \
    asm volatile ("" : "=w" (y));               \
    LHS = RHS;                                  \
    asm volatile ("" :: "w" (y));               \
  }

TEST (f_si_qi_0, si_qi_1, v16qi, y.c[0], x[0])
```
</details>


---

