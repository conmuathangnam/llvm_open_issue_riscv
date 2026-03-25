# Crash in `clang::CodeGen::CodeGenModule::EmitTopLevelDecl(Decl*)` when using constexpr generated `std::string` in toplevel `asm` statement

**Author:** apache-hb
**URL:** https://github.com/llvm/llvm-project/issues/137459
**Status:** Closed
**Labels:** clang:frontend, clang:codegen, crash
**Closed Date:** 2025-06-08T07:16:58Z

## Body

Full stacktrace, only works in c++26 mode with a very large `-fconstexpr-step=` limit.
```
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20250426/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20250426/lib/clang/21 -isystem /opt/compiler-explorer/libs/fmt/trunk/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20250426/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++26 -fdeprecated-macro -fconstexpr-steps=99999999 -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-456c4d.o -x c++ <source>
1.	<eof> parser at end of file
2.	<source>:28:1: LLVM IR generation of declaration
 #0 0x0000000003bd83d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x3bd83d8)
 #1 0x0000000003bd5f94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000074c8b2242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000074c8b23a0d30 (/lib/x86_64-linux-gnu/libc.so.6+0x1a0d30)
 #4 0x000074c8b274c104 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_mutate(unsigned long, unsigned long, char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x14c104)
 #5 0x000074c8b274db50 std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_append(char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x14db50)
 #6 0x000000000415ad50 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
 #7 0x000000000451650e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
 #8 0x0000000004500389 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x4500389)
 #9 0x0000000006147584 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x6147584)
#10 0x0000000004513b2d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x4513b2d)
#11 0x000000000482266a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x482266a)
#12 0x000000000479eaab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x479eaab)
#13 0x0000000004912773 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x4912773)
#14 0x0000000000db4311 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xdb4311)
#15 0x0000000000dac4bd ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#16 0x0000000000daf310 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xdaf310)
#17 0x0000000000c325a4 main (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xc325a4)
#18 0x000074c8b2229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#19 0x000074c8b2229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#20 0x0000000000dabf55 _start (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xdabf55)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```
[Godbolt reproducer](https://godbolt.org/z/rcahb4W6W)

Source

```cpp
#include <string>
#include <format>
#include <fmt/format.h>
#include <fmt/compile.h>

consteval std::string GenerateIsrTable() {
  std::string isrs;
  isrs.reserve(0x10000);
  for (size_t i = 32; i < 34; i++) {
    fmt::format_to(
      std::back_inserter(isrs),
      FMT_COMPILE(
      ".align 16\n"
      "AsmIsr{0}:\n"
      "  pushq {0}\n"
      "  pushq {0}\n"
      "  jmp AsmIsrEntry\n")
      , i);
  }
  return isrs;
}

asm(
    "AsmIsrEntry:\n"
    "nop\n"
);

asm((GenerateIsrTable()));
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Elliot (apache-hb)

<details>
Full stacktrace, only works in c++26 mode with a very large `-fconstexpr-step=` limit.
```
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20250426/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20250426/lib/clang/21 -isystem /opt/compiler-explorer/libs/fmt/trunk/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20250426/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++26 -fdeprecated-macro -fconstexpr-steps=99999999 -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-456c4d.o -x c++ &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:28:1: LLVM IR generation of declaration
 #<!-- -->0 0x0000000003bd83d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x3bd83d8)
 #<!-- -->1 0x0000000003bd5f94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000074c8b2242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000074c8b23a0d30 (/lib/x86_64-linux-gnu/libc.so.6+0x1a0d30)
 #<!-- -->4 0x000074c8b274c104 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;::_M_mutate(unsigned long, unsigned long, char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x14c104)
 #<!-- -->5 0x000074c8b274db50 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;::_M_append(char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x14db50)
 #<!-- -->6 0x000000000415ad50 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
 #<!-- -->7 0x000000000451650e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
 #<!-- -->8 0x0000000004500389 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x4500389)
 #<!-- -->9 0x0000000006147584 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x6147584)
#<!-- -->10 0x0000000004513b2d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x4513b2d)
#<!-- -->11 0x000000000482266a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x482266a)
#<!-- -->12 0x000000000479eaab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x479eaab)
#<!-- -->13 0x0000000004912773 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x4912773)
#<!-- -->14 0x0000000000db4311 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xdb4311)
#<!-- -->15 0x0000000000dac4bd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->16 0x0000000000daf310 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xdaf310)
#<!-- -->17 0x0000000000c325a4 main (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xc325a4)
#<!-- -->18 0x000074c8b2229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->19 0x000074c8b2229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->20 0x0000000000dabf55 _start (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xdabf55)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```
[Godbolt reproducer](https://godbolt.org/z/rcahb4W6W)
</details>


---

### Comment 2 - shafik

Backtraces from assertion builds are almost always more useful, since seeing what invariants we are breaking tells us a lot more: https://godbolt.org/z/eecWsvbj4

Assertion:

```console
clang-21: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578:
decltype(auto) llvm::cast(From*) [with To = clang::StringLiteral; From = clang::Expr]:
Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-trunk-20250426/lib/clang/21 -isystem /opt/compiler-explorer/libs/fmt/trunk/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /opt/compiler-explorer/clang-assertions-trunk-20250426/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++26 -fdeprecated-macro -fconstexpr-steps=99999999 -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-5ceab4.o -x c++ <source>
1.	<eof> parser at end of file
 #0 0x0000000003ef8fe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0x3ef8fe8)
 #1 0x0000000003ef6974 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007b78cae42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007b78cae969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007b78cae42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007b78cae287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007b78cae2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007b78cae39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x00000000068b7d0b (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0x68b7d0b)
 #9 0x0000000006566c32 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0x6566c32)
#10 0x00000000065675cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0x65675cd)
#11 0x00000000065599aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0x65599aa)
#12 0x0000000004898328 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0x4898328)
#13 0x0000000004b8b815 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0x4b8b815)
#14 0x0000000004b0a53e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0x4b0a53e)
#15 0x0000000004c7ae4e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0x4c7ae4e)
#16 0x0000000000da069f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0xda069f)
#17 0x0000000000d976aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#18 0x0000000000d9bf66 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0xd9bf66)
#19 0x0000000000c248e4 main (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0xc248e4)
#20 0x00007b78cae29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#21 0x00007b78cae29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#22 0x0000000000d97155 _start (/opt/compiler-explorer/clang-assertions-trunk-20250426/bin/clang-21+0xd97155)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```

It looks like this is rejected in 20, so I am guessing a recent PR introduced this bug when adding support for this feature: https://godbolt.org/z/Yjezv5vbK

---

### Comment 3 - zwuis

It seems that the Clang's constexpr string asm extension implementation doesn't support file scope asm. CC @cor3ntin 

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Elliot (apache-hb)

<details>
Full stacktrace, only works in c++26 mode with a very large `-fconstexpr-step=` limit.
```
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20250426/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20250426/lib/clang/21 -isystem /opt/compiler-explorer/libs/fmt/trunk/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20250426/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++26 -fdeprecated-macro -fconstexpr-steps=99999999 -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-456c4d.o -x c++ &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:28:1: LLVM IR generation of declaration
 #<!-- -->0 0x0000000003bd83d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x3bd83d8)
 #<!-- -->1 0x0000000003bd5f94 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000074c8b2242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000074c8b23a0d30 (/lib/x86_64-linux-gnu/libc.so.6+0x1a0d30)
 #<!-- -->4 0x000074c8b274c104 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;::_M_mutate(unsigned long, unsigned long, char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x14c104)
 #<!-- -->5 0x000074c8b274db50 std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;::_M_append(char const*, unsigned long) (/lib/x86_64-linux-gnu/libstdc++.so.6+0x14db50)
 #<!-- -->6 0x000000000415ad50 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
 #<!-- -->7 0x000000000451650e (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
 #<!-- -->8 0x0000000004500389 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x4500389)
 #<!-- -->9 0x0000000006147584 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x6147584)
#<!-- -->10 0x0000000004513b2d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x4513b2d)
#<!-- -->11 0x000000000482266a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x482266a)
#<!-- -->12 0x000000000479eaab clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x479eaab)
#<!-- -->13 0x0000000004912773 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0x4912773)
#<!-- -->14 0x0000000000db4311 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xdb4311)
#<!-- -->15 0x0000000000dac4bd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->16 0x0000000000daf310 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xdaf310)
#<!-- -->17 0x0000000000c325a4 main (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xc325a4)
#<!-- -->18 0x000074c8b2229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->19 0x000074c8b2229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->20 0x0000000000dabf55 _start (/opt/compiler-explorer/clang-trunk-20250426/bin/clang-21+0xdabf55)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```
[Godbolt reproducer](https://godbolt.org/z/rcahb4W6W)

Source

```cpp
#include &lt;string&gt;
#include &lt;format&gt;
#include &lt;fmt/format.h&gt;
#include &lt;fmt/compile.h&gt;

consteval std::string GenerateIsrTable() {
  std::string isrs;
  isrs.reserve(0x10000);
  for (size_t i = 32; i &lt; 34; i++) {
    fmt::format_to(
      std::back_inserter(isrs),
      FMT_COMPILE(
      ".align 16\n"
      "AsmIsr{0}:\n"
      "  pushq {0}\n"
      "  pushq {0}\n"
      "  jmp AsmIsrEntry\n")
      , i);
  }
  return isrs;
}

asm(
    "AsmIsrEntry:\n"
    "nop\n"
);

asm((GenerateIsrTable()));
```
</details>


---

