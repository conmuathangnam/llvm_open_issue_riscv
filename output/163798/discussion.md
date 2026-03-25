# [X86] Assertion failure in X86AsmBackend::applyFixup

**Author:** s-barannikov
**URL:** https://github.com/llvm/llvm-project/issues/163798
**Status:** Open
**Labels:** backend:X86, llvm:mc, crash

## Body

https://gcc.godbolt.org/z/c9Yb9d8sf
```
.data
label1:
.byte label2 - label1
.rept 255
  .byte 0
.endr
label2:
```
Compiled as:
```
llvm-mc -triple=i386 test.S -filetype=obj -o test.o
```
```
llvm-mc: /root/llvm-project/llvm/lib/Target/X86/MCTargetDesc/X86AsmBackend.cpp:714: virtual void {anonymous}::X86AsmBackend::applyFixup(const llvm::MCFragment&, const llvm::MCFixup&, const llvm::MCValue&, uint8_t*, uint64_t, bool): Assertion `(Size == 0 || isIntN(Size * 8 + 1, SignedValue)) && "Value does not fit in the Fixup field"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc -filetype=obj -o example.o <source>
 #0 0x0000000000d33a18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xd33a18)
 #1 0x0000000000d30b64 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000708f01442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000708f014969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000708f01442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000708f014287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000708f0142871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000708f01439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000009d1cac (anonymous namespace)::X86AsmBackend::applyFixup(llvm::MCFragment const&, llvm::MCFixup const&, llvm::MCValue const&, unsigned char*, unsigned long, bool) X86AsmBackend.cpp:0:0
 #9 0x0000000000bbd819 llvm::MCAssembler::evaluateFixup(llvm::MCFragment const&, llvm::MCFixup&, llvm::MCValue&, unsigned long&, bool, unsigned char*) const (.constprop.0) MCAssembler.cpp:0:0
#10 0x0000000000bc2a0f llvm::MCAssembler::layout() (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xbc2a0f)
#11 0x0000000000bc2e89 llvm::MCAssembler::Finish() (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xbc2e89)
#12 0x0000000000be7a4c llvm::MCELFStreamer::finishImpl() (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xbe7a4c)
#13 0x0000000000c074d7 llvm::MCStreamer::finish(llvm::SMLoc) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc074d7)
#14 0x0000000000c8fa62 (anonymous namespace)::AsmParser::Run(bool, bool) AsmParser.cpp:0:0
#15 0x000000000040d714 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x40d714)
#16 0x0000708f01429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#17 0x0000708f01429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#18 0x0000000000480115 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x480115)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Sergei Barannikov (s-barannikov)

<details>
https://gcc.godbolt.org/z/c9Yb9d8sf
```
.data
label1:
.byte label2 - label1
.rept 255
  .byte 0
.endr
label2:
```
Compiled as:
```
llvm-mc -triple=i386 test.S -filetype=obj -o test.o
```
```
llvm-mc: /root/llvm-project/llvm/lib/Target/X86/MCTargetDesc/X86AsmBackend.cpp:714: virtual void {anonymous}::X86AsmBackend::applyFixup(const llvm::MCFragment&amp;, const llvm::MCFixup&amp;, const llvm::MCValue&amp;, uint8_t*, uint64_t, bool): Assertion `(Size == 0 || isIntN(Size * 8 + 1, SignedValue)) &amp;&amp; "Value does not fit in the Fixup field"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc -filetype=obj -o example.o &lt;source&gt;
 #<!-- -->0 0x0000000000d33a18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xd33a18)
 #<!-- -->1 0x0000000000d30b64 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000708f01442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000708f014969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000708f01442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000708f014287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000708f0142871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000708f01439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x00000000009d1cac (anonymous namespace)::X86AsmBackend::applyFixup(llvm::MCFragment const&amp;, llvm::MCFixup const&amp;, llvm::MCValue const&amp;, unsigned char*, unsigned long, bool) X86AsmBackend.cpp:0:0
 #<!-- -->9 0x0000000000bbd819 llvm::MCAssembler::evaluateFixup(llvm::MCFragment const&amp;, llvm::MCFixup&amp;, llvm::MCValue&amp;, unsigned long&amp;, bool, unsigned char*) const (.constprop.0) MCAssembler.cpp:0:0
#<!-- -->10 0x0000000000bc2a0f llvm::MCAssembler::layout() (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xbc2a0f)
#<!-- -->11 0x0000000000bc2e89 llvm::MCAssembler::Finish() (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xbc2e89)
#<!-- -->12 0x0000000000be7a4c llvm::MCELFStreamer::finishImpl() (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xbe7a4c)
#<!-- -->13 0x0000000000c074d7 llvm::MCStreamer::finish(llvm::SMLoc) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc074d7)
#<!-- -->14 0x0000000000c8fa62 (anonymous namespace)::AsmParser::Run(bool, bool) AsmParser.cpp:0:0
#<!-- -->15 0x000000000040d714 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x40d714)
#<!-- -->16 0x0000708f01429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->17 0x0000708f01429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->18 0x0000000000480115 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x480115)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: Sergei Barannikov (s-barannikov)

<details>
https://gcc.godbolt.org/z/c9Yb9d8sf
```
.data
label1:
.byte label2 - label1
.rept 255
  .byte 0
.endr
label2:
```
Compiled as:
```
llvm-mc -triple=i386 test.S -filetype=obj -o test.o
```
```
llvm-mc: /root/llvm-project/llvm/lib/Target/X86/MCTargetDesc/X86AsmBackend.cpp:714: virtual void {anonymous}::X86AsmBackend::applyFixup(const llvm::MCFragment&amp;, const llvm::MCFixup&amp;, const llvm::MCValue&amp;, uint8_t*, uint64_t, bool): Assertion `(Size == 0 || isIntN(Size * 8 + 1, SignedValue)) &amp;&amp; "Value does not fit in the Fixup field"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc -filetype=obj -o example.o &lt;source&gt;
 #<!-- -->0 0x0000000000d33a18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xd33a18)
 #<!-- -->1 0x0000000000d30b64 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000708f01442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000708f014969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000708f01442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000708f014287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000708f0142871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000708f01439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x00000000009d1cac (anonymous namespace)::X86AsmBackend::applyFixup(llvm::MCFragment const&amp;, llvm::MCFixup const&amp;, llvm::MCValue const&amp;, unsigned char*, unsigned long, bool) X86AsmBackend.cpp:0:0
 #<!-- -->9 0x0000000000bbd819 llvm::MCAssembler::evaluateFixup(llvm::MCFragment const&amp;, llvm::MCFixup&amp;, llvm::MCValue&amp;, unsigned long&amp;, bool, unsigned char*) const (.constprop.0) MCAssembler.cpp:0:0
#<!-- -->10 0x0000000000bc2a0f llvm::MCAssembler::layout() (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xbc2a0f)
#<!-- -->11 0x0000000000bc2e89 llvm::MCAssembler::Finish() (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xbc2e89)
#<!-- -->12 0x0000000000be7a4c llvm::MCELFStreamer::finishImpl() (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xbe7a4c)
#<!-- -->13 0x0000000000c074d7 llvm::MCStreamer::finish(llvm::SMLoc) (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0xc074d7)
#<!-- -->14 0x0000000000c8fa62 (anonymous namespace)::AsmParser::Run(bool, bool) AsmParser.cpp:0:0
#<!-- -->15 0x000000000040d714 main (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x40d714)
#<!-- -->16 0x0000708f01429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->17 0x0000708f01429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->18 0x0000000000480115 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/llvm-mc+0x480115)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

</details>


---

