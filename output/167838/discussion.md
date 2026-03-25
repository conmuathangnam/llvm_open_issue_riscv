# [AArch64] Assertion `E <= size() && "Attempted to reset out-of-bounds range!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/167838
**Status:** Open
**Labels:** backend:AArch64, crash

## Body

Reproduer:
https://godbolt.org/z/4Y7czPrx4

Backtrace:
```console
llc: /root/llvm-project/llvm/include/llvm/ADT/BitVector.h:424: llvm::BitVector& llvm::BitVector::reset(unsigned int, unsigned int): Assertion `E <= size() && "Attempted to reset out-of-bounds range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel <source>
1.	Running pass 'Function Pass Manager' on module '<source>'.
2.	Running pass 'Prologue/Epilogue Insertion & Frame Finalization' on function '@warn_vla'
 #0 0x00000000041c2638 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41c2638)
 #1 0x00000000041bf4e4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000075e072042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000075e0720969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x000075e072042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x000075e0720287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000075e07202871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x000075e072039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x000000000326282b llvm::BitVector::reset(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x326282b)
 #9 0x0000000003269c07 (anonymous namespace)::PEIImpl::calculateFrameObjectOffsets(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#10 0x000000000326b22c (anonymous namespace)::PEIImpl::run(llvm::MachineFunction&) (.constprop.0) PrologEpilogInserter.cpp:0:0
#11 0x000000000326dc3a (anonymous namespace)::PEILegacy::runOnMachineFunction(llvm::MachineFunction&) PrologEpilogInserter.cpp:0:0
#12 0x00000000030a1219 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (.part.0) MachineFunctionPass.cpp:0:0
#13 0x00000000036f0a06 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f0a06)
#14 0x00000000036f0db1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f0db1)
#15 0x00000000036f161f llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f161f)
#16 0x00000000008fca42 compileModule(char**, llvm::LLVMContext&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>&) llc.cpp:0:0
#17 0x00000000007c84f9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c84f9)
#18 0x000075e072029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#19 0x000075e072029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#20 0x00000000008f1275 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f1275)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: None (k-arrows)

<details>
Reproduer:
https://godbolt.org/z/4Y7czPrx4

Backtrace:
```console
llc: /root/llvm-project/llvm/include/llvm/ADT/BitVector.h:424: llvm::BitVector&amp; llvm::BitVector::reset(unsigned int, unsigned int): Assertion `E &lt;= size() &amp;&amp; "Attempted to reset out-of-bounds range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llc -o /app/output.s -x86-asm-syntax=intel &lt;source&gt;
1.	Running pass 'Function Pass Manager' on module '&lt;source&gt;'.
2.	Running pass 'Prologue/Epilogue Insertion &amp; Frame Finalization' on function '@<!-- -->warn_vla'
 #<!-- -->0 0x00000000041c2638 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x41c2638)
 #<!-- -->1 0x00000000041bf4e4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000075e072042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000075e0720969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x000075e072042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x000075e0720287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000075e07202871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x000075e072039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x000000000326282b llvm::BitVector::reset(unsigned int, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x326282b)
 #<!-- -->9 0x0000000003269c07 (anonymous namespace)::PEIImpl::calculateFrameObjectOffsets(llvm::MachineFunction&amp;) PrologEpilogInserter.cpp:0:0
#<!-- -->10 0x000000000326b22c (anonymous namespace)::PEIImpl::run(llvm::MachineFunction&amp;) (.constprop.0) PrologEpilogInserter.cpp:0:0
#<!-- -->11 0x000000000326dc3a (anonymous namespace)::PEILegacy::runOnMachineFunction(llvm::MachineFunction&amp;) PrologEpilogInserter.cpp:0:0
#<!-- -->12 0x00000000030a1219 llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (.part.0) MachineFunctionPass.cpp:0:0
#<!-- -->13 0x00000000036f0a06 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f0a06)
#<!-- -->14 0x00000000036f0db1 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f0db1)
#<!-- -->15 0x00000000036f161f llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x36f161f)
#<!-- -->16 0x00000000008fca42 compileModule(char**, llvm::LLVMContext&amp;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;&amp;) llc.cpp:0:0
#<!-- -->17 0x00000000007c84f9 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x7c84f9)
#<!-- -->18 0x000075e072029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->19 0x000075e072029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->20 0x00000000008f1275 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llc+0x8f1275)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```
</details>


---

### Comment 2 - k-arrows

Comes from the following program:
```c
void f (void *, ...);

void warn_vla (__PTRDIFF_TYPE__ n)
{
  int a[n];
  f (a);

  if (n <= __PTRDIFF_MAX__)
    n = __PTRDIFF_MAX__;

  int b[n];
  f (a, b);
}
```

This crashes with `clang -c -O1 bug.c`.

---

### Comment 3 - davemgreen

How would you expect it to generate an array of size 9223372036854775807?

LLVM should presumably give a better error message, but I'm not sure what the limit would be.

---

### Comment 4 - efriedma-quic

You could end up with a large allocation like this by accident in code that's dynamically unreachable due to inlining or jump threading.

I think in the x86 backend, we added code so that impossible allocations are lowered to an explicit trap instruction.

---

