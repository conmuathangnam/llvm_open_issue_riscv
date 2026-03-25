# [CRASH] C23 #embed directive causes crash

**Author:** 8ss-boop
**URL:** https://github.com/llvm/llvm-project/issues/152829
**Status:** Closed
**Labels:** c23, clang:frontend, crash, embed
**Closed Date:** 2025-08-11T14:08:11Z

## Body

Compiler Explorer: https://godbolt.org/z/P1bxdqvPG

The crash triggering program (compile with `clang -O0 -std=c2x`):
```
#embed __FILE__ limit(1) prefix(])
```

Crash output:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /root/software/llvm-releases/llvm-0804/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O0 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name 1.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -fcoverage-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -resource-dir /root/software/llvm-releases/llvm-0804/lib/clang/22 -internal-isystem /root/software/llvm-releases/llvm-0804/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fmessage-length=180 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/1-7cdc51.o -x c 1.c
1.      1.c:1:2: current parser token 'embed'
 #0 0x00007f61012a5d01 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/root/software/llvm-releases/llvm-0804/bin/../lib/libLLVMSupport.so.22.0git+0x1f3d01)
 #1 0x00007f61012a2d27 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f6100b4d520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007f60feccfdc8 clang::Preprocessor::LexEmbedParameters(clang::Token&, bool)::'lambda'(llvm::SmallVectorImpl<clang::Token>&)::operator()(llvm::SmallVectorImpl<clang::Token>&) const (.constprop.0) PPDirectives.cpp:0:0
 #4 0x00007f60fecd4c73 clang::Preprocessor::LexEmbedParameters(clang::Token&, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0xa9c73)
 #5 0x00007f60fecd6722 clang::Preprocessor::HandleEmbedDirective(clang::SourceLocation, clang::Token&, clang::FileEntry const*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0xab722)
 #6 0x00007f60fece7a45 clang::Preprocessor::HandleDirective(clang::Token&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0xbca45)
 #7 0x00007f60fec77a76 clang::Lexer::LexTokenInternal(clang::Token&, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0x4ca76)
 #8 0x00007f60fed34f6c clang::Preprocessor::Lex(clang::Token&) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0x109f6c)
 #9 0x00007f60fe55ffc9 clang::ParseAST(clang::Sema&, bool, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x3ffc9)
#10 0x00007f61035b4236 clang::FrontendAction::Execute() (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x124236)
#11 0x00007f610352aa9a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x9aa9a)
#12 0x00007f61052aa7a2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontendTool.so.22.0git+0x57a2)
#13 0x0000000000414d9b cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x414d9b)
#14 0x000000000040c3c4 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#15 0x00000000004101e1 clang_main(int, char**, llvm::ToolContext const&) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x4101e1)
#16 0x000000000040bdbe main (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40bdbe)
#17 0x00007f6100b34d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#18 0x00007f6100b34e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#19 0x000000000040be05 _start (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40be05)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 28ed57eda80dc5e3b7dd01d15100c126fa4e50f6)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /root/software/llvm-releases/llvm-0804/bin
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: note: diagnostic msg: Error generating preprocessed source(s).
```

llvm does not provide automatically generated scripts and files to reproduce this bug

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (8ss-boop)

<details>
Compiler Explorer: https://godbolt.org/z/P1bxdqvPG

The crash triggering program (compile with `clang -O0 -std=c2x`):
```
#embed __FILE__ limit(1) prefix(])
```

Crash output:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /root/software/llvm-releases/llvm-0804/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O0 -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name 1.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -fcoverage-compilation-dir=/root/LLMFUZZ/LLVM-CRASH -resource-dir /root/software/llvm-releases/llvm-0804/lib/clang/22 -internal-isystem /root/software/llvm-releases/llvm-0804/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fmessage-length=180 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/1-7cdc51.o -x c 1.c
1.      1.c:1:2: current parser token 'embed'
 #<!-- -->0 0x00007f61012a5d01 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/root/software/llvm-releases/llvm-0804/bin/../lib/libLLVMSupport.so.22.0git+0x1f3d01)
 #<!-- -->1 0x00007f61012a2d27 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f6100b4d520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007f60feccfdc8 clang::Preprocessor::LexEmbedParameters(clang::Token&amp;, bool)::'lambda'(llvm::SmallVectorImpl&lt;clang::Token&gt;&amp;)::operator()(llvm::SmallVectorImpl&lt;clang::Token&gt;&amp;) const (.constprop.0) PPDirectives.cpp:0:0
 #<!-- -->4 0x00007f60fecd4c73 clang::Preprocessor::LexEmbedParameters(clang::Token&amp;, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0xa9c73)
 #<!-- -->5 0x00007f60fecd6722 clang::Preprocessor::HandleEmbedDirective(clang::SourceLocation, clang::Token&amp;, clang::FileEntry const*) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0xab722)
 #<!-- -->6 0x00007f60fece7a45 clang::Preprocessor::HandleDirective(clang::Token&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0xbca45)
 #<!-- -->7 0x00007f60fec77a76 clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0x4ca76)
 #<!-- -->8 0x00007f60fed34f6c clang::Preprocessor::Lex(clang::Token&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangLex.so.22.0git+0x109f6c)
 #<!-- -->9 0x00007f60fe55ffc9 clang::ParseAST(clang::Sema&amp;, bool, bool) (/root/software/llvm-releases/llvm-0804/bin/../lib/../lib/libclangParse.so.22.0git+0x3ffc9)
#<!-- -->10 0x00007f61035b4236 clang::FrontendAction::Execute() (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x124236)
#<!-- -->11 0x00007f610352aa9a clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontend.so.22.0git+0x9aa9a)
#<!-- -->12 0x00007f61052aa7a2 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/root/software/llvm-releases/llvm-0804/bin/../lib/libclangFrontendTool.so.22.0git+0x57a2)
#<!-- -->13 0x0000000000414d9b cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x414d9b)
#<!-- -->14 0x000000000040c3c4 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->15 0x00000000004101e1 clang_main(int, char**, llvm::ToolContext const&amp;) (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x4101e1)
#<!-- -->16 0x000000000040bdbe main (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40bdbe)
#<!-- -->17 0x00007f6100b34d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->18 0x00007f6100b34e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->19 0x000000000040be05 _start (/root/software/llvm-releases/llvm-0804/bin/clang-22+0x40be05)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 28ed57eda80dc5e3b7dd01d15100c126fa4e50f6)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /root/software/llvm-releases/llvm-0804/bin
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: note: diagnostic msg: Error generating preprocessed source(s).
```

llvm does not provide automatically generated scripts and files to reproduce this bug
</details>


---

