# out of bound access on a `-verify`  test line ending with a splice before EOF

**Author:** cor3ntin
**URL:** https://github.com/llvm/llvm-project/issues/141221
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-05-29T20:26:26Z

## Body

I discovered that while writing tests

```cpp
// expected-error@-1 {{}} \
```
compiled with -verify causes an assertion failure

```
clang++: /root/llvm-project/llvm/include/llvm/ADT/StringRef.h:242: char llvm::StringRef::operator[](size_t) const: Assertion `Index < size() && "Invalid index!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Xclang -verify <source>
1.	<unknown> parser at unknown location
 #0 0x0000000003fac638 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fac638)
 #1 0x0000000003faa2c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3faa2c4)
 #2 0x0000000003eeeef8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007ff77a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007ff77a8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007ff77a842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007ff77a8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007ff77a82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007ff77a839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000004cc0e62 clang::VerifyDiagnosticConsumer::HandleComment(clang::Preprocessor&, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cc0e62)
#10 0x0000000007ede28f clang::Preprocessor::HandleComment(clang::Token&, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ede28f)
#11 0x0000000007e32c2b clang::Lexer::SkipLineComment(clang::Token&, char const*, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e32c2b)
#12 0x0000000007e3c415 clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e3c415)
#13 0x0000000007e3dbef clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e3dbef)
#14 0x0000000007edd9ec clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7edd9ec)
#15 0x000000000664c972 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664c972)
#16 0x000000000664a578 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664a578)
```


https://compiler-explorer.com/z/fxh34shnd



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: cor3ntin (cor3ntin)

<details>
I discovered that while writing tests

```cpp
// expected-error@-1 {{}} \
```
compiled with -verify causes an assertion failure

```
clang++: /root/llvm-project/llvm/include/llvm/ADT/StringRef.h:242: char llvm::StringRef::operator[](size_t) const: Assertion `Index &lt; size() &amp;&amp; "Invalid index!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -Xclang -verify &lt;source&gt;
1.	&lt;unknown&gt; parser at unknown location
 #<!-- -->0 0x0000000003fac638 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fac638)
 #<!-- -->1 0x0000000003faa2c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3faa2c4)
 #<!-- -->2 0x0000000003eeeef8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007ff77a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007ff77a8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007ff77a842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007ff77a8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007ff77a82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007ff77a839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000004cc0e62 clang::VerifyDiagnosticConsumer::HandleComment(clang::Preprocessor&amp;, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cc0e62)
#<!-- -->10 0x0000000007ede28f clang::Preprocessor::HandleComment(clang::Token&amp;, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ede28f)
#<!-- -->11 0x0000000007e32c2b clang::Lexer::SkipLineComment(clang::Token&amp;, char const*, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e32c2b)
#<!-- -->12 0x0000000007e3c415 clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e3c415)
#<!-- -->13 0x0000000007e3dbef clang::Lexer::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e3dbef)
#<!-- -->14 0x0000000007edd9ec clang::Preprocessor::Lex(clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7edd9ec)
#<!-- -->15 0x000000000664c972 clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664c972)
#<!-- -->16 0x000000000664a578 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x664a578)
```


https://compiler-explorer.com/z/fxh34shnd


</details>


---

