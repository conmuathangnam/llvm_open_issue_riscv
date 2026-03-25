# [FastISel]  "Requesting 64-bit offset in 32-bit immediate! failed."

**Author:** DigOrDog
**URL:** https://github.com/llvm/llvm-project/issues/113856
**Status:** Closed
**Labels:** backend:X86, llvm:codegen, crash
**Closed Date:** 2025-08-11T15:53:47Z

## Body

# Description
The following code crashes X86 backend with "Requesting 64-bit offset in 32-bit immediate! failed."

# Minimal Reproduction
https://godbolt.org/z/KET7GezGa

## Code
```
; llc  -mtriple=x86_64 -O=0
@G = global i8 0

define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %G = getelementptr i8, ptr %1, i32 -2147483648
  store i32 0, ptr %G, align 4
  ret i32 0
}

```

## Stack Trace
```
llc: /root/llvm-project/llvm/lib/Target/X86/X86RegisterInfo.cpp:962: virtual bool llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger*) const: Assertion `(!Is64Bit || isInt<32>((long long)FIOffset + Imm)) && "Requesting 64-bit offset in 32-bit immediate!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=x86_64 -O=0 <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@main'
 #0 0x0000000003ba5b58 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ba5b58)
 #1 0x0000000003ba354c SignalHandler(int) Signals.cpp:0:0
 #2 0x00007c1c21642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007c1c216969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007c1c21642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007c1c216287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007c1c2162871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007c1c21639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000001e7ecb2 llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineInstrBundleIterator<llvm::MachineInstr, false>, int, unsigned int, llvm::RegScavenger*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1e7ecb2)
 #9 0x0000000002cca2b0 (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#10 0x0000000002b23cc9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#11 0x000000000311e950 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x311e950)
#12 0x000000000311ed01 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x311ed01)
#13 0x000000000311f5b7 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x311f5b7)
#14 0x000000000086ad7c compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#15 0x0000000000756966 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x756966)
#16 0x00007c1c21629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#17 0x00007c1c21629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#18 0x0000000000861a8e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x861a8e)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: None (DigOrDog)

<details>
# Description
The following code crashes X86 backend with "Requesting 64-bit offset in 32-bit immediate! failed."

# Minimal Reproduction
https://godbolt.org/z/KET7GezGa

## Code
```
; llc  -mtriple=x86_64 -O=0
@<!-- -->G = global i8 0

define dso_local i32 @<!-- -->main() #<!-- -->0 {
  %1 = alloca i32, align 4
  %G = getelementptr i8, ptr %1, i32 -2147483648
  store i32 0, ptr %G, align 4
  ret i32 0
}

```

## Stack Trace
```
llc: /root/llvm-project/llvm/lib/Target/X86/X86RegisterInfo.cpp:962: virtual bool llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineBasicBlock::iterator, int, unsigned int, llvm::RegScavenger*) const: Assertion `(!Is64Bit || isInt&lt;32&gt;((long long)FIOffset + Imm)) &amp;&amp; "Requesting 64-bit offset in 32-bit immediate!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel -mtriple=x86_64 -O=0 &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'Prologue/Epilogue Insertion &amp; Frame Finalization' on function '@<!-- -->main'
 #<!-- -->0 0x0000000003ba5b58 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x3ba5b58)
 #<!-- -->1 0x0000000003ba354c SignalHandler(int) Signals.cpp:0:0
 #<!-- -->2 0x00007c1c21642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007c1c216969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007c1c21642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007c1c216287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007c1c2162871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007c1c21639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000001e7ecb2 llvm::X86RegisterInfo::eliminateFrameIndex(llvm::MachineInstrBundleIterator&lt;llvm::MachineInstr, false&gt;, int, unsigned int, llvm::RegScavenger*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x1e7ecb2)
 #<!-- -->9 0x0000000002cca2b0 (anonymous namespace)::PEI::runOnMachineFunction(llvm::MachineFunction&amp;) PrologEpilogInserter.cpp:0:0
#<!-- -->10 0x0000000002b23cc9 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->11 0x000000000311e950 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x311e950)
#<!-- -->12 0x000000000311ed01 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x311ed01)
#<!-- -->13 0x000000000311f5b7 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x311f5b7)
#<!-- -->14 0x000000000086ad7c compileModule(char**, llvm::LLVMContext&amp;) llc.cpp:0:0
#<!-- -->15 0x0000000000756966 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x756966)
#<!-- -->16 0x00007c1c21629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->17 0x00007c1c21629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->18 0x0000000000861a8e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x861a8e)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

</details>


---

