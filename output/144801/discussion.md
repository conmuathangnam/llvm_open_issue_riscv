# [bug report] Clang 18.1.3 segmentation fault while parsing an erroneous class template specializaiton

**Author:** xealits
**URL:** https://github.com/llvm/llvm-project/issues/144801
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-06-18T23:02:03Z

## Body

Clang 18.1.3 crashed on a random test program and suggested to submit this bug report. I was just trying out a couple specializations of a variadic template with non-typename parameters in C++14. In the process, I tried an erroneous specialization like `template<char& a, char& b> struct S<> : public S<a, b> {...};` and got the following.

The log:
```
$ clang++ -std=c++14 -Wall -Wconversion specialise_variadic_templates.cpp
specialise_variadic_templates.cpp:32:8: error: partial specialization of 'Port8bit' does not use any of its template parameters
   32 | struct Port8bit<> : Port8bit<p_dir, p_out> {
      |        ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/lib/llvm-18/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name specialise_variadic_templates.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/ubuntu/tests/cpp -fcoverage-compilation-dir=/home/ubuntu/tests/cpp -resource-dir /usr/lib/llvm-18/lib/clang/18 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/llvm-18/lib/clang/18/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wconversion -std=c++14 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/specialise_variadic_templates-b4a691.o -x c++ specialise_variadic_templates.cpp
1.	specialise_variadic_templates.cpp:33:18: current parser token '{'
2.	specialise_variadic_templates.cpp:32:1: parsing struct/union/class body 'Port8bit<>'



 #0 0x000072d562ba63bf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
 #1 0x000072d562ba44f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
 #2 0x000072d562ba6b00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
 #3 0x000072d561645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000072d56aea7a8a clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) build-llvm/tools/clang/stage2-bins/clang/lib/Sema/SemaInit.cpp:8618:23
 #5 0x000072d56ac2272f BuildImplicitBaseInitializer build-llvm/tools/clang/stage2-bins/clang/lib/Sema/SemaDeclCXX.cpp:4946:24
 #6 0x000072d56ac21f9f clang::Sema::SetCtorInitializers(clang::CXXConstructorDecl*, bool, llvm::ArrayRef<clang::CXXCtorInitializer*>) build-llvm/tools/clang/stage2-bins/clang/lib/Sema/SemaDeclCXX.cpp:0:11
 #7 0x000072d56ac2549d clang::Sema::ActOnDefaultCtorInitializers(clang::Decl*) build-llvm/tools/clang/stage2-bins/clang/lib/Sema/SemaDeclCXX.cpp:5994:5
 #8 0x000072d56a19c1d0 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseCXXInlineMethods.cpp:599:33
 #9 0x000072d56a19af1a clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseCXXInlineMethods.cpp:530:33
#10 0x000072d56a1d1eac clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseDeclCXX.cpp:3694:21
#11 0x000072d56a1cf284 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseDeclCXX.cpp:0:7
#12 0x000072d56a1b0e1d empty build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/SmallVector.h:94:46
#13 0x000072d56a1b0e1d empty build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/ParsedAttr.h:820:40
#14 0x000072d56a1b0e1d clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseDecl.cpp:4403:23
#15 0x000072d56a2506ec is build-llvm/tools/clang/stage2-bins/clang/include/clang/Lex/Token.h:98:49
#16 0x000072d56a2506ec clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseTemplate.cpp:241:11
#17 0x000072d56a24fa56 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseTemplate.cpp:0:0
#18 0x000072d56a24f3d5 ObjCDeclContextSwitch build-llvm/tools/clang/stage2-bins/clang/include/clang/Parse/Parser.h:1059:11
#19 0x000072d56a24f3d5 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseTemplate.cpp:42:25
#20 0x000072d56a1ab2cd clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseDecl.cpp:1960:10
#21 0x000072d56a26148f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/Parser.cpp:0:0
#22 0x000072d56a25ff7b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/Parser.cpp:760:10
#23 0x000072d56a19747e clang::ParseAST(clang::Sema&, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:162:5
#24 0x000072d56c00662c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
#25 0x000072d56bf830b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
#26 0x000072d56bf830b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
#27 0x000072d56bf830b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
#28 0x000072d56c08263d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) build-llvm/tools/clang/stage2-bins/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:272:25
#29 0x000060a51888442e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) build-llvm/tools/clang/stage2-bins/clang/tools/driver/cc1_main.cpp:294:15
#30 0x000060a518881894 ExecuteCC1Tool build-llvm/tools/clang/stage2-bins/clang/tools/driver/driver.cpp:365:12
#31 0x000060a518880a26 clang_main(int, char**, llvm::ToolContext const&) build-llvm/tools/clang/stage2-bins/clang/tools/driver/driver.cpp:405:12
#32 0x000060a51888e383 main build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/tools/driver/clang-driver.cpp:17:10
#33 0x000072d56162a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#34 0x000072d56162a28b call_init ./csu/../csu/libc-start.c:128:20
#35 0x000072d56162a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#36 0x000060a51887e255 _start (/usr/lib/llvm-18/bin/clang+0xe255)

clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/specialise_variadic_templates-dd3a0e.cpp
clang++: note: diagnostic msg: /tmp/specialise_variadic_templates-dd3a0e.sh
clang++: note: diagnostic msg: 

********************
```

`specialise_variadic_templates-dd3a0e.sh` file:
```
$ cat bug_report_clang/specialise_variadic_templates-dd3a0e.sh 
# Crash reproducer for Ubuntu clang version 18.1.3 (1ubuntu1)
# Driver args: "--driver-mode=g++" "-std=c++14" "-Wall" "-Wconversion" "specialise_variadic_templates.cpp"
# Original command:  "/usr/lib/llvm-18/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "specialise_variadic_templates.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/ubuntu/tests/cpp" "-fcoverage-compilation-dir=/home/ubuntu/tests/cpp" "-resource-dir" "/usr/lib/llvm-18/lib/clang/18" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward" "-internal-isystem" "/usr/lib/llvm-18/lib/clang/18/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-Wall" "-Wconversion" "-std=c++14" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/specialise_variadic_templates-b4a691.o" "-x" "c++" "specialise_variadic_templates.cpp"
 "/usr/lib/llvm-18/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "specialise_variadic_templates.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/ubuntu/tests/cpp" "-fcoverage-compilation-dir=/home/ubuntu/tests/cpp" "-Wall" "-Wconversion" "-std=c++14" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "specialise_variadic_templates-dd3a0e.cpp"
```

The `specialise_variadic_templates-dd3a0e.cpp` file is 81K lines long and github does not let me attach it. I will try to attach it in a comment. The code part is simple there:
```
/* many where were 2 headers:
#include <iostream>
#include <vector>
*/

// I want a template - i.e. the same name - for 2 sets of parameters: N and K
// not just the parameters themselves are different,
// but their number is different
// and I would like the more-parameters template to inherit from the smaller one

// https://stackoverflow.com/questions/73888241/template-specialization-for-variadic-template
template <class...>
struct is_first_a_vector_impl : std::false_type {
  is_first_a_vector_impl(void) {std::cout << "variadic template\n";}
};

template <class T, class... Ts>
struct is_first_a_vector_impl<std::vector<T>, Ts...> : std::true_type {
  is_first_a_vector_impl(void) {std::cout << "template with a vector\n";}
};

template<volatile char& ...>
struct Port8bit; // purposefully undefined

// partial specialisation:
// https://stackoverflow.com/questions/35283056/too-many-template-parameters-in-template-redeclaration
template<volatile char& p_dir, volatile char& p_out>
struct Port8bit<p_dir, p_out> {
  Port8bit(void) { std::cout << "basic port control\n"; }
  void print(void) { std::cout << "hey!\n"; }
};

template<volatile char& p_dir, volatile char& p_out, volatile char& p_int>
struct Port8bit<> : Port8bit<p_dir, p_out> {
  Port8bit(void) { std::cout << "port with interrupt control\n"; }
  void more(void) { std::cout << "more "; print(); }
};


volatile char dir, out, interrupt;

int main() {
  is_first_a_vector_impl<std::vector<double>> foo;
  if (foo) {
    std::cout << "foo is true\n";
  } else {
    std::cout << "foo is false\n";
  }

  Port8bit<dir, out> basic;
  Port8bit<dir, out, interrupt> interrupt_port;

  basic.print();
  interrupt_port.print();
  interrupt_port.more();

  return 0;
}
```

## Comments

### Comment 1 - xealits

The file with the preprocessed source `specialise_variadic_templates-dd3a0e.cpp` is in [this gist](https://gist.github.com/xealits/6b6250f003d56069275bdb5fb79a55ea).

---

### Comment 2 - EugeneZelenko

Could you please try 20 or `main` branch? 

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alex Toldaiev (xealits)

<details>
Clang 18.1.3 crashed on a random test program and suggested to submit this bug report. I was just trying out a couple specializations of a variadic template with non-typename parameters in C++14. In the process, I tried an erroneous specialization like `template&lt;char&amp; a, char&amp; b&gt; struct S&lt;&gt; : public S&lt;a, b&gt; {...};` and got the following.

The log:
```
$ clang++ -std=c++14 -Wall -Wconversion specialise_variadic_templates.cpp
specialise_variadic_templates.cpp:32:8: error: partial specialization of 'Port8bit' does not use any of its template parameters
   32 | struct Port8bit&lt;&gt; : Port8bit&lt;p_dir, p_out&gt; {
      |        ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/lib/llvm-18/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name specialise_variadic_templates.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/ubuntu/tests/cpp -fcoverage-compilation-dir=/home/ubuntu/tests/cpp -resource-dir /usr/lib/llvm-18/lib/clang/18 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/llvm-18/lib/clang/18/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -Wall -Wconversion -std=c++14 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/specialise_variadic_templates-b4a691.o -x c++ specialise_variadic_templates.cpp
1.	specialise_variadic_templates.cpp:33:18: current parser token '{'
2.	specialise_variadic_templates.cpp:32:1: parsing struct/union/class body 'Port8bit&lt;&gt;'



 #<!-- -->0 0x000072d562ba63bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:723:13
 #<!-- -->1 0x000072d562ba44f9 llvm::sys::RunSignalHandlers() build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Signals.cpp:106:18
 #<!-- -->2 0x000072d562ba6b00 SignalHandler build-llvm/tools/clang/stage2-bins/llvm/lib/Support/Unix/Signals.inc:413:1
 #<!-- -->3 0x000072d561645330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->4 0x000072d56aea7a8a clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) build-llvm/tools/clang/stage2-bins/clang/lib/Sema/SemaInit.cpp:8618:23
 #<!-- -->5 0x000072d56ac2272f BuildImplicitBaseInitializer build-llvm/tools/clang/stage2-bins/clang/lib/Sema/SemaDeclCXX.cpp:4946:24
 #<!-- -->6 0x000072d56ac21f9f clang::Sema::SetCtorInitializers(clang::CXXConstructorDecl*, bool, llvm::ArrayRef&lt;clang::CXXCtorInitializer*&gt;) build-llvm/tools/clang/stage2-bins/clang/lib/Sema/SemaDeclCXX.cpp:0:11
 #<!-- -->7 0x000072d56ac2549d clang::Sema::ActOnDefaultCtorInitializers(clang::Decl*) build-llvm/tools/clang/stage2-bins/clang/lib/Sema/SemaDeclCXX.cpp:5994:5
 #<!-- -->8 0x000072d56a19c1d0 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseCXXInlineMethods.cpp:599:33
 #<!-- -->9 0x000072d56a19af1a clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseCXXInlineMethods.cpp:530:33
#<!-- -->10 0x000072d56a1d1eac clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseDeclCXX.cpp:3694:21
#<!-- -->11 0x000072d56a1cf284 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseDeclCXX.cpp:0:7
#<!-- -->12 0x000072d56a1b0e1d empty build-llvm/tools/clang/stage2-bins/llvm/include/llvm/ADT/SmallVector.h:94:46
#<!-- -->13 0x000072d56a1b0e1d empty build-llvm/tools/clang/stage2-bins/clang/include/clang/Sema/ParsedAttr.h:820:40
#<!-- -->14 0x000072d56a1b0e1d clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseDecl.cpp:4403:23
#<!-- -->15 0x000072d56a2506ec is build-llvm/tools/clang/stage2-bins/clang/include/clang/Lex/Token.h:98:49
#<!-- -->16 0x000072d56a2506ec clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseTemplate.cpp:241:11
#<!-- -->17 0x000072d56a24fa56 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseTemplate.cpp:0:0
#<!-- -->18 0x000072d56a24f3d5 ObjCDeclContextSwitch build-llvm/tools/clang/stage2-bins/clang/include/clang/Parse/Parser.h:1059:11
#<!-- -->19 0x000072d56a24f3d5 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseTemplate.cpp:42:25
#<!-- -->20 0x000072d56a1ab2cd clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseDecl.cpp:1960:10
#<!-- -->21 0x000072d56a26148f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/Parser.cpp:0:0
#<!-- -->22 0x000072d56a25ff7b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/Parser.cpp:760:10
#<!-- -->23 0x000072d56a19747e clang::ParseAST(clang::Sema&amp;, bool, bool) build-llvm/tools/clang/stage2-bins/clang/lib/Parse/ParseAST.cpp:162:5
#<!-- -->24 0x000072d56c00662c clang::FrontendAction::Execute() build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/FrontendAction.cpp:1073:10
#<!-- -->25 0x000072d56bf830b4 getPtr build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:279:12
#<!-- -->26 0x000072d56bf830b4 operator bool build-llvm/tools/clang/stage2-bins/llvm/include/llvm/Support/Error.h:239:16
#<!-- -->27 0x000072d56bf830b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) build-llvm/tools/clang/stage2-bins/clang/lib/Frontend/CompilerInstance.cpp:1057:23
#<!-- -->28 0x000072d56c08263d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) build-llvm/tools/clang/stage2-bins/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:272:25
#<!-- -->29 0x000060a51888442e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) build-llvm/tools/clang/stage2-bins/clang/tools/driver/cc1_main.cpp:294:15
#<!-- -->30 0x000060a518881894 ExecuteCC1Tool build-llvm/tools/clang/stage2-bins/clang/tools/driver/driver.cpp:365:12
#<!-- -->31 0x000060a518880a26 clang_main(int, char**, llvm::ToolContext const&amp;) build-llvm/tools/clang/stage2-bins/clang/tools/driver/driver.cpp:405:12
#<!-- -->32 0x000060a51888e383 main build-llvm/tools/clang/stage2-bins/build-llvm/tools/clang/stage2-bins/tools/clang/tools/driver/clang-driver.cpp:17:10
#<!-- -->33 0x000072d56162a1ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->34 0x000072d56162a28b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->35 0x000072d56162a28b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->36 0x000060a51887e255 _start (/usr/lib/llvm-18/bin/clang+0xe255)

clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/specialise_variadic_templates-dd3a0e.cpp
clang++: note: diagnostic msg: /tmp/specialise_variadic_templates-dd3a0e.sh
clang++: note: diagnostic msg: 

********************
```

`specialise_variadic_templates-dd3a0e.sh` file:
```
$ cat bug_report_clang/specialise_variadic_templates-dd3a0e.sh 
# Crash reproducer for Ubuntu clang version 18.1.3 (1ubuntu1)
# Driver args: "--driver-mode=g++" "-std=c++14" "-Wall" "-Wconversion" "specialise_variadic_templates.cpp"
# Original command:  "/usr/lib/llvm-18/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "specialise_variadic_templates.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/ubuntu/tests/cpp" "-fcoverage-compilation-dir=/home/ubuntu/tests/cpp" "-resource-dir" "/usr/lib/llvm-18/lib/clang/18" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward" "-internal-isystem" "/usr/lib/llvm-18/lib/clang/18/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include" "-internal-externc-isystem" "/usr/include/x86_64-linux-gnu" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "-Wall" "-Wconversion" "-std=c++14" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "/tmp/specialise_variadic_templates-b4a691.o" "-x" "c++" "specialise_variadic_templates.cpp"
 "/usr/lib/llvm-18/bin/clang" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "-dumpdir" "a-" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "specialise_variadic_templates.cpp" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/ubuntu/tests/cpp" "-fcoverage-compilation-dir=/home/ubuntu/tests/cpp" "-Wall" "-Wconversion" "-std=c++14" "-fdeprecated-macro" "-ferror-limit" "19" "-fgnuc-version=4.2.1" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "specialise_variadic_templates-dd3a0e.cpp"
```

The `specialise_variadic_templates-dd3a0e.cpp` file is 81K lines long and github does not let me attach it. I will try to attach it in a comment. The code part is simple there:
```
/* many where were 2 headers:
#include &lt;iostream&gt;
#include &lt;vector&gt;
*/

// I want a template - i.e. the same name - for 2 sets of parameters: N and K
// not just the parameters themselves are different,
// but their number is different
// and I would like the more-parameters template to inherit from the smaller one

// https://stackoverflow.com/questions/73888241/template-specialization-for-variadic-template
template &lt;class...&gt;
struct is_first_a_vector_impl : std::false_type {
  is_first_a_vector_impl(void) {std::cout &lt;&lt; "variadic template\n";}
};

template &lt;class T, class... Ts&gt;
struct is_first_a_vector_impl&lt;std::vector&lt;T&gt;, Ts...&gt; : std::true_type {
  is_first_a_vector_impl(void) {std::cout &lt;&lt; "template with a vector\n";}
};

template&lt;volatile char&amp; ...&gt;
struct Port8bit; // purposefully undefined

// partial specialisation:
// https://stackoverflow.com/questions/35283056/too-many-template-parameters-in-template-redeclaration
template&lt;volatile char&amp; p_dir, volatile char&amp; p_out&gt;
struct Port8bit&lt;p_dir, p_out&gt; {
  Port8bit(void) { std::cout &lt;&lt; "basic port control\n"; }
  void print(void) { std::cout &lt;&lt; "hey!\n"; }
};

template&lt;volatile char&amp; p_dir, volatile char&amp; p_out, volatile char&amp; p_int&gt;
struct Port8bit&lt;&gt; : Port8bit&lt;p_dir, p_out&gt; {
  Port8bit(void) { std::cout &lt;&lt; "port with interrupt control\n"; }
  void more(void) { std::cout &lt;&lt; "more "; print(); }
};


volatile char dir, out, interrupt;

int main() {
  is_first_a_vector_impl&lt;std::vector&lt;double&gt;&gt; foo;
  if (foo) {
    std::cout &lt;&lt; "foo is true\n";
  } else {
    std::cout &lt;&lt; "foo is false\n";
  }

  Port8bit&lt;dir, out&gt; basic;
  Port8bit&lt;dir, out, interrupt&gt; interrupt_port;

  basic.print();
  interrupt_port.print();
  interrupt_port.more();

  return 0;
}
```
</details>


---

### Comment 4 - xealits

> Could you please try 20 or `main` branch?

yes, it is reproducible and it fails with `-std=c++20` too.

I just tried with this program (slightly modified after finishing my tests):
```
clang++ -std=c++20 -Wall -Wconversion specialise_variadic_templates_bugreport.cpp
```

```
// specialise_variadic_templates_bugreport.cpp
#include <iostream>
#include <vector>

// I want a template - i.e. the same name - for 2 sets of parameters: N and K
// not just the parameters themselves are different,
// but their number is different
// and I would like the more-parameters template to inherit from the smaller one

// https://stackoverflow.com/questions/73888241/template-specialization-for-variadic-template
template <class...>
struct is_first_a_vector_impl : std::false_type {
  is_first_a_vector_impl(void) {std::cout << "variadic template\n";}
};

template <class T, class... Ts>
struct is_first_a_vector_impl<std::vector<T>, Ts...> : std::true_type {
  is_first_a_vector_impl(void) {std::cout << "template with a vector\n";}
};

template<volatile char& ...>
struct Port8bit; // purposefully undefined

// partial specialisation:
// https://stackoverflow.com/questions/35283056/too-many-template-parameters-in-template-redeclaration
template<volatile char& p_dir, volatile char& p_out>
struct Port8bit<p_dir, p_out> {
  Port8bit(void) { std::cout << "basic port control\n"; }
  static void print(void) { std::cout << "hey!\n"; }
};

template<volatile char& p_dir, volatile char& p_out, volatile char& p_int>
// error: partial specialization of 'Port8bit' does not use any of its template parameters
struct Port8bit<> : Port8bit<p_dir, p_out>
//struct Port8bit<p_dir, p_out, p_int> : public Port8bit<p_dir, p_out>
{
  Port8bit(void) { std::cout << "port with interrupt control\n"; }
  //using Port8bit<p_dir, p_out>::print; // needed in C++14 and C++20
                                       // to be able to call from the class code
                                       // but the users can call it without using
  static void more(void) { std::cout << "more "; /*print();*/ }
};


volatile char dir, out, interrupt;

int main() {
  is_first_a_vector_impl<std::vector<double>> foo;
  if (foo) {
    std::cout << "foo is true\n";
  } else {
    std::cout << "foo is false\n";
  }

  Port8bit<dir, out> basic;
  Port8bit<dir, out, interrupt> interrupt_port;

  basic.print();
  interrupt_port.print();
  interrupt_port.more();

  return 0;
}
```

---

### Comment 5 - EugeneZelenko

Sorry, if I was not clear, I meant Clang version 20. Only most recent release is maintained.

---

### Comment 6 - xealits

Oh, I just realised you mean clang 20! Sorry, let me see.

It is fine on 19:
```
$ clang++-19 -std=c++14 -Wall -Wconversion specialise_variadic_templates_bugreport.cpp 
specialise_variadic_templates_bugreport.cpp:34:8: error: partial specialization of 'Port8bit' does not use any of its template parameters
   34 | struct Port8bit<> : Port8bit<p_dir, p_out>
      |        ^
specialise_variadic_templates_bugreport.cpp:56:33: error: implicit instantiation of undefined template 'Port8bit<dir, out, interrupt>'
   56 |   Port8bit<dir, out, interrupt> interrupt_port;
      |                                 ^
specialise_variadic_templates_bugreport.cpp:22:8: note: template is declared here
   22 | struct Port8bit; // purposefully undefined
      |        ^
2 errors generated.
```

I cannot quickly install 20.

---

### Comment 7 - shafik

This does not crash on trunk: https://godbolt.org/z/PsPTYdzEY

looks like it was fixed in clang-19. I am going to close since this looks fixed.

Might be a dup of this one: https://github.com/llvm/llvm-project/issues/67914 but the backtrace is not identical but this was also fixed in clang-19.

---

