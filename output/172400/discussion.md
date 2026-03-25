# [AArch64] crashes when converting long double to __bf16 on AArch64

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/172400
**Status:** Open
**Labels:** backend:AArch64, crash

## Body

Reproducer:
https://godbolt.org/z/no5fv4zK9

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeDAG.cpp:985: void {anonymous}::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*): Assertion `(TLI.getTypeAction(*DAG.getContext(), Op.getValueType()) == TargetLowering::TypeLegal || Op.getOpcode() == ISD::TargetConstant || Op.getOpcode() == ISD::Register) && "Unexpected illegal type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'AArch64 Instruction Selection' on function '@tf_to_bf'
 #0 0x000000000421fb68 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x421fb68)
 #1 0x000000000421ca14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007714a4042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007714a40969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007714a4042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007714a40287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007714a402871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007714a4039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000003e93a17 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
 #9 0x0000000003e95e1e llvm::SelectionDAG::Legalize() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e95e1e)
#10 0x0000000003fb6262 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fb6262)
#11 0x0000000003fb95b8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fb95b8)
#12 0x0000000003fbb03c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fbb03c)
#13 0x0000000003fa683f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fa683f)
#14 0x00000000030f5fd9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#15 0x0000000003750ab6 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3750ab6)
#16 0x0000000003750e61 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3750e61)
#17 0x00000000037516cf llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x37516cf)
#18 0x00000000008fe384 compileModule(char**, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#19 0x00000000007c88f9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c88f9)
#20 0x00007714a4029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x00007714a4029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x00000000008f3585 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f3585)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
extern   __bf16    bf;
extern long double tf;

void tf_to_bf () { bf = tf; }
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/no5fv4zK9

Backtrace:
```console
llc: /root/llvm-project/llvm/lib/CodeGen/SelectionDAG/LegalizeDAG.cpp:985: void {anonymous}::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*): Assertion `(TLI.getTypeAction(*DAG.getContext(), Op.getValueType()) == TargetLowering::TypeLegal || Op.getOpcode() == ISD::TargetConstant || Op.getOpcode() == ISD::Register) &amp;&amp; "Unexpected illegal type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'AArch64 Instruction Selection' on function '@<!-- -->tf_to_bf'
 #<!-- -->0 0x000000000421fb68 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x421fb68)
 #<!-- -->1 0x000000000421ca14 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007714a4042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007714a40969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007714a4042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007714a40287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007714a402871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007714a4039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000003e93a17 (anonymous namespace)::SelectionDAGLegalize::LegalizeOp(llvm::SDNode*) (.part.0) LegalizeDAG.cpp:0:0
 #<!-- -->9 0x0000000003e95e1e llvm::SelectionDAG::Legalize() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3e95e1e)
#<!-- -->10 0x0000000003fb6262 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fb6262)
#<!-- -->11 0x0000000003fb95b8 llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fb95b8)
#<!-- -->12 0x0000000003fbb03c llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fbb03c)
#<!-- -->13 0x0000000003fa683f llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3fa683f)
#<!-- -->14 0x00000000030f5fd9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->15 0x0000000003750ab6 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3750ab6)
#<!-- -->16 0x0000000003750e61 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3750e61)
#<!-- -->17 0x00000000037516cf llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x37516cf)
#<!-- -->18 0x00000000008fe384 compileModule(char**, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->19 0x00000000007c88f9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c88f9)
#<!-- -->20 0x00007714a4029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->21 0x00007714a4029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->22 0x00000000008f3585 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f3585)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

This comes from the following C reproducer:
```c
extern   __bf16    bf;
extern long double tf;

void tf_to_bf () { bf = tf; }
```
</details>


---

### Comment 2 - SabYic

Is this a good first issue for beginner to start LLVM development?

---

### Comment 3 - cofibrant

> Is this a good first issue for beginner to start LLVM development?

Go for it! Keep the issue updated if you get stuck

---

### Comment 4 - SabYic

since I am a begginer ,it may take a month for me to tackle with this issue. I am trying

---

### Comment 5 - c-rhodes

> since I am a begginer ,it may take a month for me to tackle with this issue. I am trying

we can go from fp128 -> half: https://godbolt.org/z/jah6Tx8vn

it emits a libcall to `__trunctfhf2` which is implemented in compiler-rt:
https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/builtins/trunctfhf2.c

the LLVM part was implemented in https://reviews.llvm.org/D91731 and compiler-rt in https://reviews.llvm.org/D91732.

there's an equivalent routine for bfloat, perhaps we can emit a call to this:
https://github.com/llvm/llvm-project/blob/main/compiler-rt/lib/builtins/trunctfbf2.c

It's currently only supported on x86, but I suspect that should be replaced with `COMPILER_RT_HAS_BFLOAT16`.

---

### Comment 6 - rishabhmadan19

@k-arrows 
I added the fix. Please compile with my changes.

---

### Comment 7 - k-arrows

> [@k-arrows](https://github.com/k-arrows) I added the fix. Please compile with my changes.

Thanks for working on this and sharing the fix.
I’m not a developer, so it might be best to submit a PR and let the appropriate reviewers review it.

---

