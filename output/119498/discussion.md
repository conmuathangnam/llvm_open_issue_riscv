# Ice Issue in LLVM-Trunk with _Complex Type Handling

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/119498

## Body

This crash can be observed in both LLVM 18 and LLVM trunk versions. [https://godbolt.org/z/j1Pedxvrq](https://godbolt.org/z/j1Pedxvrq)
# Command
`clang++ bug.cpp`
# Code
```cpp
_Complex float foo()
{
  float f;
  __real__ f = 0;
  __imag__ f = 0;
  return f;
}
_Complex float bar()
{
  float f;
  __real__ f = 0;
  __imag__ f = 0;
  return f;
}
_Complex float baz()
{
  float f;
  __real__ f = 0;
  __imag__ } 


typedef _Complex float C;
C foo()
{
  C f;
  __real__ f = 0;
  __imag__ f = 0;
  return f;
}
```

# Version
```sh
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

# Error Output
```sh
Stack dump:
0.	Program arguments: /usr/lib/llvm-18/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name uninit-12-O0_2.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/masked_data/gcc-11.1.0/gcc/testsuite/gcc.dg -fcoverage-compilation-dir=/home/masked_data/gcc-11.1.0/gcc/testsuite/gcc.dg -resource-dir /usr/lib/llvm-18/lib/clang/18 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/llvm-18/lib/clang/18/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O0 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -dwarf-debug-flags "/usr/lib/llvm-18/bin/clang --driver-mode=g++ uninit-12-O0_2.c -O0 -dumpdir a-" -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/uninit-12-O0_2-196ef2.o -x c++ uninit-12-O0_2.c
1.	uninit-12-O0_2.c:17:1: current parser token '_Complex'
2.	uninit-12-O0_2.c:10:16: LLVM IR generation of declaration 'foo'
3.	uninit-12-O0_2.c:10:16: Generating code for declaration 'foo'
 #0 0x00007feba0fdfb46 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd93b46)
 #1 0x00007feba0fddaf0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd91af0)
 #2 0x00007feba0fe0210 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94210)
 #3 0x00007feb9fe2f090 (/lib/x86_64-linux-gnu/libc.so.6+0x43090)
 #4 0x00007feba96301ea clang::CodeGen::CodeGenFunction::emitAddrOfImagComponent(clang::CodeGen::Address, clang::QualType) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1de61ea)
 #5 0x00007feba96068a7 clang::CodeGen::CodeGenFunction::EmitUnaryOpLValue(clang::UnaryOperator const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1dbc8a7)
 #6 0x00007feba9604018 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1dba018)
 #7 0x00007feba9601ebb clang::CodeGen::CodeGenFunction::EmitCheckedLValue(clang::Expr const*, clang::CodeGen::CodeGenFunction::TypeCheckKind) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1db7ebb)
 #8 0x00007feba9654295 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1e0a295)
 #9 0x00007feba9648bad clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1dfebad)
#10 0x00007feba95f6ce9 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1dacce9)
#11 0x00007feba974d70b clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f0370b)
#12 0x00007feba9758f7c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f0ef7c)
#13 0x00007feba97b1b8f clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f67b8f)
#14 0x00007feba97b2657 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f68657)
#15 0x00007feba97d33c5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f893c5)
#16 0x00007feba97ccceb clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f82ceb)
#17 0x00007feba97d0a33 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f86a33)
#18 0x00007feba97cba02 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f81a02)
#19 0x00007feba986a09c (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x202009c)
#20 0x00007feba97a2b7e clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f58b7e)
#21 0x00007feba83cfc9a clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb85c9a)
#22 0x00007febaa207415 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29bd415)
#23 0x00007febaa181674 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2937674)
#24 0x00007febaa28169e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a3769e)
#25 0x000055b1c8e83c62 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-18/bin/clang+0x12c62)
#26 0x000055b1c8e81095 (/usr/lib/llvm-18/bin/clang+0x10095)
#27 0x000055b1c8e80256 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-18/bin/clang+0xf256)
#28 0x000055b1c8e8daf6 main (/usr/lib/llvm-18/bin/clang+0x1caf6)
#29 0x00007feb9fe10083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#30 0x000055b1c8e7db0e _start (/usr/lib/llvm-18/bin/clang+0xcb0e)
clang++-18: error: unable to execute command: Segmentation fault (core dumped)
clang++-18: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++-18: warning: treating 'c' input as 'c++' when in C++ mode, this behavior is deprecated [-Wdeprecated]
clang++-18: note: diagnostic msg: 
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
This crash can be observed in both LLVM 18 and LLVM trunk versions. [https://godbolt.org/z/j1Pedxvrq](https://godbolt.org/z/j1Pedxvrq)
# Command
`clang++ bug.cpp`
# Code
```cpp
_Complex float foo()
{
  float f;
  __real__ f = 0;
  __imag__ f = 0;
  return f;
}
_Complex float bar()
{
  float f;
  __real__ f = 0;
  __imag__ f = 0;
  return f;
}
_Complex float baz()
{
  float f;
  __real__ f = 0;
  __imag__ } 


typedef _Complex float C;
C foo()
{
  C f;
  __real__ f = 0;
  __imag__ f = 0;
  return f;
}
```

# Version
```sh
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

# Error Output
```sh
Stack dump:
0.	Program arguments: /usr/lib/llvm-18/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name uninit-12-O0_2.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/masked_data/gcc-11.1.0/gcc/testsuite/gcc.dg -fcoverage-compilation-dir=/home/masked_data/gcc-11.1.0/gcc/testsuite/gcc.dg -resource-dir /usr/lib/llvm-18/lib/clang/18 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/llvm-18/lib/clang/18/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -O0 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -dwarf-debug-flags "/usr/lib/llvm-18/bin/clang --driver-mode=g++ uninit-12-O0_2.c -O0 -dumpdir a-" -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/uninit-12-O0_2-196ef2.o -x c++ uninit-12-O0_2.c
1.	uninit-12-O0_2.c:17:1: current parser token '_Complex'
2.	uninit-12-O0_2.c:10:16: LLVM IR generation of declaration 'foo'
3.	uninit-12-O0_2.c:10:16: Generating code for declaration 'foo'
 #<!-- -->0 0x00007feba0fdfb46 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd93b46)
 #<!-- -->1 0x00007feba0fddaf0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd91af0)
 #<!-- -->2 0x00007feba0fe0210 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94210)
 #<!-- -->3 0x00007feb9fe2f090 (/lib/x86_64-linux-gnu/libc.so.6+0x43090)
 #<!-- -->4 0x00007feba96301ea clang::CodeGen::CodeGenFunction::emitAddrOfImagComponent(clang::CodeGen::Address, clang::QualType) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1de61ea)
 #<!-- -->5 0x00007feba96068a7 clang::CodeGen::CodeGenFunction::EmitUnaryOpLValue(clang::UnaryOperator const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1dbc8a7)
 #<!-- -->6 0x00007feba9604018 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1dba018)
 #<!-- -->7 0x00007feba9601ebb clang::CodeGen::CodeGenFunction::EmitCheckedLValue(clang::Expr const*, clang::CodeGen::CodeGenFunction::TypeCheckKind) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1db7ebb)
 #<!-- -->8 0x00007feba9654295 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1e0a295)
 #<!-- -->9 0x00007feba9648bad clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1dfebad)
#<!-- -->10 0x00007feba95f6ce9 clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1dacce9)
#<!-- -->11 0x00007feba974d70b clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f0370b)
#<!-- -->12 0x00007feba9758f7c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f0ef7c)
#<!-- -->13 0x00007feba97b1b8f clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f67b8f)
#<!-- -->14 0x00007feba97b2657 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f68657)
#<!-- -->15 0x00007feba97d33c5 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f893c5)
#<!-- -->16 0x00007feba97ccceb clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f82ceb)
#<!-- -->17 0x00007feba97d0a33 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f86a33)
#<!-- -->18 0x00007feba97cba02 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f81a02)
#<!-- -->19 0x00007feba986a09c (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x202009c)
#<!-- -->20 0x00007feba97a2b7e clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1f58b7e)
#<!-- -->21 0x00007feba83cfc9a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb85c9a)
#<!-- -->22 0x00007febaa207415 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29bd415)
#<!-- -->23 0x00007febaa181674 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2937674)
#<!-- -->24 0x00007febaa28169e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a3769e)
#<!-- -->25 0x000055b1c8e83c62 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-18/bin/clang+0x12c62)
#<!-- -->26 0x000055b1c8e81095 (/usr/lib/llvm-18/bin/clang+0x10095)
#<!-- -->27 0x000055b1c8e80256 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-18/bin/clang+0xf256)
#<!-- -->28 0x000055b1c8e8daf6 main (/usr/lib/llvm-18/bin/clang+0x1caf6)
#<!-- -->29 0x00007feb9fe10083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->30 0x000055b1c8e7db0e _start (/usr/lib/llvm-18/bin/clang+0xcb0e)
clang++-18: error: unable to execute command: Segmentation fault (core dumped)
clang++-18: error: clang frontend command failed due to signal (use -v to see invocation)
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++-18: warning: treating 'c' input as 'c++' when in C++ mode, this behavior is deprecated [-Wdeprecated]
clang++-18: note: diagnostic msg: 
```

</details>


---

### Comment 2 - EugeneZelenko

Assertion build crashes in frontend: https://godbolt.org/z/eTKrxcrzo

---

### Comment 3 - shafik

Goes back to clang-3.1: https://godbolt.org/z/qz4orbdb5

---

