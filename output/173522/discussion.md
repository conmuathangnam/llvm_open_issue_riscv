# [flang] fatal internal error: CHECK(!parseState.anyErrorRecovery() || parseState.messages().AnyFatalError()) failed at llvm-project/flang/lib/Parser/parsing.cpp(289)

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173522
**Status:** Closed
**Labels:** flang:frontend, crash
**Closed Date:** 2025-12-31T20:43:29Z

## Body

Reproducer:
https://godbolt.org/z/r16zGrxos
```f90
namelist /abc/def
write (*,nml=123)
end
```

Backtrace:
```console
fatal internal error: CHECK(!parseState.anyErrorRecovery() || parseState.messages().AnyFatalError()) failed at /root/llvm-project/flang/lib/Parser/parsing.cpp(289)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #0 0x000000000454c958 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x454c958)
 #1 0x0000000004549b24 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000773abce42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000773abce969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000773abce42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000773abce287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x000000000676f6c9 (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x676f6c9)
 #7 0x00000000063e52d5 Fortran::parser::Parsing::Parse(llvm::raw_ostream&) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x63e52d5)
 #8 0x00000000045b3308 Fortran::frontend::FrontendAction::runParse(bool) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x45b3308)
 #9 0x000000000485a72d Fortran::frontend::CodeGenAction::beginSourceFileAction() (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x485a72d)
#10 0x00000000045afd25 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&, Fortran::frontend::FrontendInputFile const&) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x45afd25)
#11 0x000000000459d11f Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x459d11f)
#12 0x00000000045b9673 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x45b9673)
#13 0x00000000027f43c2 fc1_main(llvm::ArrayRef<char const*>, char const*) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x27f43c2)
#14 0x00000000026930a1 main (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x26930a1)
#15 0x0000773abce29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#16 0x0000773abce29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#17 0x00000000027f2e75 _start (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x27f2e75)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 575d200d64e973bf0c62a42f7ae35cc8b18579c2)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin
flang-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-22: note: diagnostic msg: /tmp/example-fe76fc
flang-22: note: diagnostic msg: /tmp/example-fe76fc.sh
flang-22: note: diagnostic msg: 

********************
Compiler returned: 254
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-flang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/r16zGrxos
```f90
namelist /abc/def
write (*,nml=123)
end
```

Backtrace:
```console
fatal internal error: CHECK(!parseState.anyErrorRecovery() || parseState.messages().AnyFatalError()) failed at /root/llvm-project/flang/lib/Parser/parsing.cpp(289)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang -fc1 -triple x86_64-unknown-linux-gnu -S -mrelocation-model pic -pic-level 2 -pic-is-pie -target-cpu x86-64 -mllvm -x86-asm-syntax=intel -resource-dir /cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/lib/clang/22 -mframe-pointer=all -o /app/output.s -x f95 /app/example.f90
 #<!-- -->0 0x000000000454c958 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x454c958)
 #<!-- -->1 0x0000000004549b24 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000773abce42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000773abce969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000773abce42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000773abce287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x000000000676f6c9 (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x676f6c9)
 #<!-- -->7 0x00000000063e52d5 Fortran::parser::Parsing::Parse(llvm::raw_ostream&amp;) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x63e52d5)
 #<!-- -->8 0x00000000045b3308 Fortran::frontend::FrontendAction::runParse(bool) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x45b3308)
 #<!-- -->9 0x000000000485a72d Fortran::frontend::CodeGenAction::beginSourceFileAction() (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x485a72d)
#<!-- -->10 0x00000000045afd25 Fortran::frontend::FrontendAction::beginSourceFile(Fortran::frontend::CompilerInstance&amp;, Fortran::frontend::FrontendInputFile const&amp;) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x45afd25)
#<!-- -->11 0x000000000459d11f Fortran::frontend::CompilerInstance::executeAction(Fortran::frontend::FrontendAction&amp;) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x459d11f)
#<!-- -->12 0x00000000045b9673 Fortran::frontend::executeCompilerInvocation(Fortran::frontend::CompilerInstance*) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x45b9673)
#<!-- -->13 0x00000000027f43c2 fc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*) (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x27f43c2)
#<!-- -->14 0x00000000026930a1 main (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x26930a1)
#<!-- -->15 0x0000773abce29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->16 0x0000773abce29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->17 0x00000000027f2e75 _start (/cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin/flang+0x27f2e75)
flang-22: error: unable to execute command: Aborted (core dumped)
flang-22: error: flang frontend command failed due to signal (use -v to see invocation)
flang version 22.0.0git (https://github.com/llvm/llvm-project.git 575d200d64e973bf0c62a42f7ae35cc8b18579c2)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /cefs/d6/d627a40be3938837fc12a226_clang-llvmflang-trunk-20251224/bin
flang-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
flang-22: note: diagnostic msg: /tmp/example-fe76fc
flang-22: note: diagnostic msg: /tmp/example-fe76fc.sh
flang-22: note: diagnostic msg: 

********************
Compiler returned: 254
```
</details>


---

