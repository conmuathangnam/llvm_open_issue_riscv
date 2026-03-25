# ICE segfault on generic lambda with defaulted parameter in requires clause

**Author:** Eczbek
**URL:** https://github.com/llvm/llvm-project/issues/131620

## Body

https://godbolt.org/z/3Grx96fPf

```cpp
int x = 0;

int main() {
	[](auto x) requires requires { []<int = 0>(int = x) {}(); } {}(0);
}
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Eczbek)

<details>
https://godbolt.org/z/3Grx96fPf

```cpp
int x = 0;

int main() {
	[](auto x) requires requires { []&lt;int = 0&gt;(int = x) {}(); } {}(0);
}
```

</details>


---

### Comment 2 - shafik

Backtrace:

```console
Program terminated with signal: SIGSEGV
ASM generation compiler returned: 139
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20250317/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20250317/lib/clang/21 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../include/c++/15.0.1/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20250317/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/15.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-821625.o -x c++ <source>
1.	<source>:4:66: current parser token ')'
2.	<source>:3:12: parsing function body 'main'
3.	<source>:3:12: in compound statement ('{}')
4.	<source>:4:33: instantiating function definition 'main()::(anonymous class)::operator()(int)::(anonymous class)::operator()<0>'
5.	<source>:4:33: LLVM IR generation of declaration 'main()::(anonymous class)::operator()(int)::(anonymous class)::operator()'
6.	<source>:4:33: Mangling declaration 'main()::(anonymous class)::operator()(int)::(anonymous class)::operator()'
  #0 0x0000000003b6ed88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk-20250317/bin/clang-21+0x3b6ed88)
  #1 0x0000000003b6c944 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
  #2 0x00007971f6042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #3 0x000000000755b947 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #4 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #5 0x000000000755d95e (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #6 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #7 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
  #8 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
  #9 0x000000000755cfeb (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #10 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #11 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #12 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #13 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #14 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #15 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #16 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #17 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #18 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #19 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #20 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #21 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #22 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #23 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #24 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #25 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #26 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #27 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #28 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #29 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #30 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #31 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #32 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #33 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #34 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #35 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #36 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #37 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #38 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #39 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #40 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #41 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #42 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #43 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #44 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #45 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #46 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #47 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #48 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #49 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #50 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #51 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #52 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #53 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #54 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #55 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #56 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #57 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #58 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #59 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #60 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #61 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #62 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #63 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #64 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #65 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #66 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #67 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #68 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #69 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #70 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #71 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #72 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #73 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #74 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #75 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #76 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #77 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #78 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #79 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #80 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #81 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #82 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #83 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #84 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #85 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #86 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #87 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #88 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #89 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #90 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #91 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
 #92 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
 #93 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
 #94 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #95 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #96 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
 #97 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
 #98 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
 #99 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#100 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#101 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#102 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#103 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#104 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#105 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#106 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#107 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#108 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#109 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#110 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#111 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#112 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#113 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#114 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#115 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#116 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#117 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#118 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#119 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#120 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#121 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#122 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#123 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#124 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#125 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#126 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#127 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#128 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#129 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#130 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#131 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#132 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#133 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#134 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#135 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#136 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#137 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#138 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#139 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#140 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#141 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#142 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#143 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#144 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#145 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#146 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#147 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#148 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#149 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#150 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#151 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#152 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#153 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#154 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#155 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#156 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#157 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#158 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#159 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#160 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#161 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#162 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#163 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#164 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#165 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#166 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#167 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#168 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#169 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#170 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#171 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#172 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#173 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#174 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#175 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#176 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#177 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#178 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#179 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#180 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#181 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#182 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#183 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#184 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#185 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#186 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#187 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#188 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#189 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#190 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#191 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#192 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#193 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#194 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#195 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#196 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#197 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#198 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#199 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#200 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#201 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#202 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#203 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#204 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#205 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#206 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#207 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#208 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#209 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#210 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#211 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#212 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#213 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#214 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#215 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#216 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#217 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#218 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#219 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#220 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#221 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#222 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#223 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#224 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#225 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#226 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#227 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#228 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#229 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#230 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#231 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#232 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#233 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#234 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#235 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#236 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#237 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#238 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#239 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#240 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#241 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#242 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#243 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#244 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#245 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#246 0x000000000755861e (anonymous namespace)::CXXNameMangler::mangleRequirement(clang::SourceLocation, clang::concepts::Requirement const*) ItaniumMangle.cpp:0:0
#247 0x0000000007555635 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#248 0x000000000755760c (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) ItaniumMangle.cpp:0:0
#249 0x000000000755cffa (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#250 0x000000000755b4be (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#251 0x000000000755bb3e (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#252 0x000000000755c4f7 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#253 0x000000000754b972 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) ItaniumMangle.cpp:0:0
#254 0x0000000007553f7f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
#255 0x0000000007553dce (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) ItaniumMangle.cpp:0:0
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Execution build compiler returned: 254
```

---

### Comment 3 - shafik

Looks very similar to: https://github.com/llvm/llvm-project/issues/102169

I am tempted to say they are duplicates but the previous one seems to crash one version earlier.

---

