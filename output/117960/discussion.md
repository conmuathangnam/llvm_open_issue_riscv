# Crashes When Compiling Template Enum with Function and Static Assertions

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/117960
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash
**Closed Date:** 2025-01-10T15:31:59Z

## Body

This crash can be observed in both LLVM 18 and LLVM trunk versions.[https://godbolt.org/z/v57annjGn](https://godbolt.org/z/v57annjGn)
# Command
`clang++ bug.cpp`

# Code
```cpp
template < typename T > struct A
{
  enum E : T;
  E h ();
};

A < int > a;
A < int >::E b = a.h () + 1;

int i = b.e ();
static_assert (i == 1, "i");
A::E e = b.e ();
static_assert (e, "");

A::E e = A::h () + 1;

template < typename T > enum A < T >::E : T { e };  

template < typename T > typename A < T >::E A < T >::h ()
{
  return e;  
}
```
# Version
clang-18
```sh
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```
# Error Output
```sh
Stack dump:
0.      Program arguments: /usr/lib/llvm-18/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/bugs -fcoverage-compilation-dir=/home/bugs -resource-dir /usr/lib/llvm-18/lib/clang/18 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/llvm-18/lib/clang/18/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -dwarf-debug-flags "/usr/lib/llvm-18/bin/clang --driver-mode=g++ bug.cpp -dumpdir a-" -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-2ab286.o -x c++ bug.cpp
1.      bug.cpp:8:28: current parser token ';'
 #0 0x00007fb2523ecb46 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd93b46)
 #1 0x00007fb2523eaaf0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd91af0)
 #2 0x00007fb2523ed210 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94210)
 #3 0x00007fb25123c090 (/lib/x86_64-linux-gnu/libc.so.6+0x43090)
 #4 0x00007fb2598f2639 clang::ASTContext::getIntegerRank(clang::Type const*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc9b639)
 #5 0x00007fb2598f30c5 clang::ASTContext::getIntegerTypeOrder(clang::QualType, clang::QualType) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc9c0c5)
 #6 0x00007fb25a2fcdf3 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a5df3)
 #7 0x00007fb25a2fc132 clang::Sema::UsualArithmeticConversions(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::Sema::ArithConvKind) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a5132)
 #8 0x00007fb25a32095b clang::Sema::CheckAdditionOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::BinaryOperatorKind, clang::QualType*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16c995b)
 #9 0x00007fb25a30de92 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16b6e92)
#10 0x00007fb25a5f3d74 clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x199cd74)
#11 0x00007fb25a32cf2c (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16d5f2c)
#12 0x00007fb25a32cb10 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16d5b10)
#13 0x00007fb25a2fba9f clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a4a9f)
#14 0x00007fb2598250a4 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbce0a4)
#15 0x00007fb259823e8f clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbcce8f)
#16 0x00007fb2597f44fe clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9d4fe)
#17 0x00007fb2597f1884 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9a884)
#18 0x00007fb2598a6e7a clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4fe7a)
#19 0x00007fb2598a6843 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4f843)
#20 0x00007fb2598a5aa6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4eaa6)
#21 0x00007fb2598a3eda clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4ceda)
#22 0x00007fb2597dcc7e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb85c7e)
#23 0x00007fb25b614415 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29bd415)
#24 0x00007fb25b58e674 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2937674)
#25 0x00007fb25b68e69e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a3769e)
#26 0x000055ecd5475c62 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-18/bin/clang+0x12c62)
#27 0x000055ecd5473095 (/usr/lib/llvm-18/bin/clang+0x10095)
#28 0x000055ecd5472256 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-18/bin/clang+0xf256)
#29 0x000055ecd547faf6 main (/usr/lib/llvm-18/bin/clang+0x1caf6)
#30 0x00007fb25121d083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#31 0x000055ecd546fb0e _start (/usr/lib/llvm-18/bin/clang+0xcb0e)
```
clang-truck
```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:8:28: current parser token ';'
#0 0x00000000039bee18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x39bee18)
#1 0x00000000039bcf64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x39bcf64)
#2 0x000000000390d898 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#3 0x0000791d8a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#4 0x0000000007025f38 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7025f38)
#5 0x00007ffcdfa753a0 
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
This crash can be observed in both LLVM 18 and LLVM trunk versions.[https://godbolt.org/z/v57annjGn](https://godbolt.org/z/v57annjGn)
# Command
`clang++ bug.cpp`

# Code
```cpp
template &lt; typename T &gt; struct A
{
  enum E : T;
  E h ();
};

A &lt; int &gt; a;
A &lt; int &gt;::E b = a.h () + 1;

int i = b.e ();
static_assert (i == 1, "i");
A::E e = b.e ();
static_assert (e, "");

A::E e = A::h () + 1;

template &lt; typename T &gt; enum A &lt; T &gt;::E : T { e };  

template &lt; typename T &gt; typename A &lt; T &gt;::E A &lt; T &gt;::h ()
{
  return e;  
}
```
# Version
clang-18
```sh
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```
# Error Output
```sh
Stack dump:
0.      Program arguments: /usr/lib/llvm-18/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/bugs -fcoverage-compilation-dir=/home/bugs -resource-dir /usr/lib/llvm-18/lib/clang/18 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/llvm-18/lib/clang/18/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -dwarf-debug-flags "/usr/lib/llvm-18/bin/clang --driver-mode=g++ bug.cpp -dumpdir a-" -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-2ab286.o -x c++ bug.cpp
1.      bug.cpp:8:28: current parser token ';'
 #<!-- -->0 0x00007fb2523ecb46 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd93b46)
 #<!-- -->1 0x00007fb2523eaaf0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd91af0)
 #<!-- -->2 0x00007fb2523ed210 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94210)
 #<!-- -->3 0x00007fb25123c090 (/lib/x86_64-linux-gnu/libc.so.6+0x43090)
 #<!-- -->4 0x00007fb2598f2639 clang::ASTContext::getIntegerRank(clang::Type const*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc9b639)
 #<!-- -->5 0x00007fb2598f30c5 clang::ASTContext::getIntegerTypeOrder(clang::QualType, clang::QualType) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc9c0c5)
 #<!-- -->6 0x00007fb25a2fcdf3 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a5df3)
 #<!-- -->7 0x00007fb25a2fc132 clang::Sema::UsualArithmeticConversions(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::Sema::ArithConvKind) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a5132)
 #<!-- -->8 0x00007fb25a32095b clang::Sema::CheckAdditionOperands(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::SourceLocation, clang::BinaryOperatorKind, clang::QualType*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16c995b)
 #<!-- -->9 0x00007fb25a30de92 clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16b6e92)
#<!-- -->10 0x00007fb25a5f3d74 clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&amp;, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x199cd74)
#<!-- -->11 0x00007fb25a32cf2c (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16d5f2c)
#<!-- -->12 0x00007fb25a32cb10 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16d5b10)
#<!-- -->13 0x00007fb25a2fba9f clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a4a9f)
#<!-- -->14 0x00007fb2598250a4 clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbce0a4)
#<!-- -->15 0x00007fb259823e8f clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbcce8f)
#<!-- -->16 0x00007fb2597f44fe clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9d4fe)
#<!-- -->17 0x00007fb2597f1884 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9a884)
#<!-- -->18 0x00007fb2598a6e7a clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4fe7a)
#<!-- -->19 0x00007fb2598a6843 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4f843)
#<!-- -->20 0x00007fb2598a5aa6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4eaa6)
#<!-- -->21 0x00007fb2598a3eda clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4ceda)
#<!-- -->22 0x00007fb2597dcc7e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb85c7e)
#<!-- -->23 0x00007fb25b614415 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29bd415)
#<!-- -->24 0x00007fb25b58e674 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2937674)
#<!-- -->25 0x00007fb25b68e69e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a3769e)
#<!-- -->26 0x000055ecd5475c62 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-18/bin/clang+0x12c62)
#<!-- -->27 0x000055ecd5473095 (/usr/lib/llvm-18/bin/clang+0x10095)
#<!-- -->28 0x000055ecd5472256 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-18/bin/clang+0xf256)
#<!-- -->29 0x000055ecd547faf6 main (/usr/lib/llvm-18/bin/clang+0x1caf6)
#<!-- -->30 0x00007fb25121d083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->31 0x000055ecd546fb0e _start (/usr/lib/llvm-18/bin/clang+0xcb0e)
```
clang-truck
```sh
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:8:28: current parser token ';'
#<!-- -->0 0x00000000039bee18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x39bee18)
#<!-- -->1 0x00000000039bcf64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x39bcf64)
#<!-- -->2 0x000000000390d898 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
#<!-- -->3 0x0000791d8a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#<!-- -->4 0x0000000007025f38 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7025f38)
#<!-- -->5 0x00007ffcdfa753a0 
```
</details>


---

### Comment 2 - MagentaTreehouse

Reduced to valid code:
```c++
template <typename T> struct A
{
  enum E : T;
  E h();
};

int b = A<int>{}.h() + 0;
```

Assertion:
```
clang++: /root/llvm-project/clang/lib/AST/ASTContext.cpp:7674: unsigned int clang::ASTContext::getIntegerRank(const clang::Type*) const: Assertion `T->isCanonicalUnqualified() && "T should be canonicalized"' failed.
```

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:7:25: current parser token ';'
 #0 0x0000000003bf5af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf5af8)
 #1 0x0000000003bf37fc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf37fc)
 #2 0x0000000003b40ee8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000732fabe42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000732fabe969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000732fabe42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000732fabe287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000732fabe2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000732fabe39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000738fa59 clang::ASTContext::getIntegerRank(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x738fa59)
#10 0x000000000738fbb6 clang::ASTContext::getIntegerTypeOrder(clang::QualType, clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x738fbb6)
#11 0x00000000069b2ca5 clang::QualType handleIntegerConversion<&(anonymous namespace)::doIntegralCast(clang::Sema&, clang::Expr*, clang::QualType), &(anonymous namespace)::doIntegralCast(clang::Sema&, clang::Expr*, clang::QualType)>(clang::Sema&, clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::QualType, clang::QualType, bool) SemaExpr.cpp:0:0
#12 0x0000000006a3072b clang::Sema::UsualArithmeticConversions(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::Sema::ArithConvKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3072b)
#13 0x0000000006a3426c clang::Sema::CheckAdditionOperands(clang::ActionResult<clang::Expr*, true>&, clang::ActionResult<clang::Expr*, true>&, clang::SourceLocation, clang::BinaryOperatorKind, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3426c)
#14 0x0000000006a3de8d clang::Sema::CreateBuiltinBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3de8d)
#15 0x0000000006df1b7e clang::Sema::CreateOverloadedBinOp(clang::SourceLocation, clang::BinaryOperatorKind, clang::UnresolvedSetImpl const&, clang::Expr*, clang::Expr*, bool, bool, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df1b7e)
#16 0x00000000069e8602 BuildOverloadedBinOp(clang::Sema&, clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) SemaExpr.cpp:0:0
#17 0x0000000006a3f187 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3f187)
#18 0x0000000006a3f8c4 clang::Sema::ActOnBinOp(clang::Scope*, clang::SourceLocation, clang::tok::TokenKind, clang::Expr*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3f8c4)
#19 0x000000000659a17d clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659a17d)
#20 0x0000000006552448 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552448)
#21 0x0000000006561519 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6561519)
#22 0x000000000652146e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652146e)
#23 0x0000000006521c2e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6521c2e)
#24 0x00000000065293c3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65293c3)
#25 0x000000000652a2ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652a2ad)
#26 0x000000000651c7ca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651c7ca)
#27 0x0000000004578f98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4578f98)
#28 0x0000000004833eb9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4833eb9)
#29 0x00000000047b3dce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47b3dce)
#30 0x000000000491edde clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x491edde)
#31 0x0000000000caf871 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcaf871)
#32 0x0000000000ca73fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x00000000045bc1f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x0000000003b41394 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b41394)
#35 0x00000000045bc7ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000004582a7d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4582a7d)
#37 0x0000000004583b6d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4583b6d)
#38 0x000000000458af25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458af25)
#39 0x0000000000cac6a3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcac6a3)
#40 0x0000000000b7fe54 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb7fe54)
#41 0x0000732fabe29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x0000732fabe29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000ca6eae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca6eae)
```

See Compiler Explorer: https://godbolt.org/z/hr7zc8bhT

---

### Comment 3 - shafik

This goes back to clang-7: https://godbolt.org/z/j5TM95vcY

---

### Comment 4 - shafik

So I think here:

https://github.com/llvm/llvm-project/blob/2af2634c64b1c6f34c4905b24863b930effe3b9c/clang/lib/AST/ASTContext.cpp#L7869-L7870

we call `getIntegerTypeForEnum` which is here:

https://github.com/llvm/llvm-project/blob/2af2634c64b1c6f34c4905b24863b930effe3b9c/clang/lib/AST/ASTContext.cpp#L7851-L7856

I think it should be using

`getIntegerType()->getUnqualifiedDesugaredType()`

because what we get back w/ `getIntegerType().getTypePtr()` in this case is

```console
SubstTemplateTypeParmType 0x108150f80 'int' sugar typename depth 0 index 0 T
|-ClassTemplateSpecialization 0x108150bc0 'A'
`-BuiltinType 0x108054d10 'int'
```

which does not satify `isCanonicalUnqualified`

CC @erichkeane if you agree, i think this is a good first issue

---

### Comment 5 - thebrandre

It turns out, `EnumDecl::PromotionType` for the instantiated `EnumDecl` is set only in `Sema::ActOnEnumBody`, which, as the name suggests, is not called in case of an *opaque-enum-declaration* [[dcl.enum]](https://eel.is/c++draft/dcl.enum).
By contrast, the member `EnumDecl::IntegerType` is set correctly on instantiation.

This causes inconsistent behavior in even simpler cases where the *enum-base* does not depend on the template parameter:
```cpp
enum E : unsigned;

struct S {
    enum E : unsigned;
};

template <typename T>
struct TS {
    enum E : unsigned;
};

#include <concepts>
#include <type_traits>

static_assert(std::same_as<unsigned, decltype(E{} + 0)>);
static_assert(std::same_as<unsigned, decltype(S::E{} + 0)>);
static_assert(std::same_as<unsigned, decltype(TS<float>::E{} + 0)>); // fails in Clang but is accepted by msvc and GCC

static_assert(std::same_as<unsigned, std::underlying_type_t<E>>);
static_assert(std::same_as<unsigned, std::underlying_type_t<S::E>>);
static_assert(std::same_as<unsigned, std::underlying_type_t<TS<float>::E>>); // works also in Clang!

```
See Compiler Explorer: [https://godbolt.org/z/7q6dWzxjh](https://godbolt.org/z/7q6dWzxjh).

Note that for the previously discussed program, which causes a crash in Clang,
```cpp
template <typename T>
struct A {
    enum E : T ;
};

int b = A<int>::E{} + 0; // assertion triggered in handleIntegerConversion
```
...  if `EnumDecl::PromotionType` is set, `handleIntegerConversion` in `SemaExpr.cpp` will not be called because `Sema::UsualUnaryConversions` already produces an `int`.
I am not sure yet if `handleIntegerConversion` needs to be fixed regardless.

I am working on a fix right now but please be aware that this would be in fact a "first issue" for me. Thanks @erichkeane for the encouragement!
Considering the implications and the fact that it will probably still take some time until I figure out how to write good regression tests, please just let me know if you would prefer to leave this to a more experienced developer.


---

