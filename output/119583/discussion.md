# [Clang] Crash: SIGSEGV emitted when variable declared in lambda init-capture is used in parameter type

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/119583
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2025-02-10T07:53:28Z

## Body

```c++
template <class T>
void f(T t) {
    [x = t] (decltype(x)) {};
}
```

Crash since clang 17:
```console
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

Compiler Explorer: https://godbolt.org/z/6EWaoa79s

Maybe related to the implementation of this DR, but I am not sure:

* DR11: Change scope of lambda trailing-return-type ([P2036R3](https://wg21.link/P2036R3), [P2579R0](https://wg21.link/P2579R0))

## Comments

### Comment 1 - shafik

@cor3ntin git bisect points to this commit which caused the regression here: 93d7002dc4644b0a6f15a998dff0d55c72012e87

---

### Comment 2 - MagentaTreehouse

Tested with local debug build and got stacktrace:
```console
0.      Program arguments: /home/treehouse/build/llvm-project-main/Clang_19-Debug/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir test- -disable-free -clear-ast-before-backend -main-file-name test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math 
-mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/mnt/d/CS/CppExamples -fcoverage-compilation-dir=/mnt/d/CS/CppExamples -resource-dir /home/treehouse/build/llvm-project-main/Clang_19-Debug/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /home/treehouse/build/llvm-project-main/Clang_19-Debug/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-3d82ba.o -x c++ test.cpp
1.      test.cpp:3:27: current parser token '{'
2.      test.cpp:2:13: parsing function body 'f'
3.      test.cpp:2:13: in compound statement ('{}')
4.      test.cpp:3:5: lambda expression parsing
  #0 0x00007f290749817d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/treehouse/source/llvm-project-main/llvm/lib/Support/Unix/Signals.inc:723:11
  #1 0x00007f290749863b PrintStackTraceSignalHandler(void*) /home/treehouse/source/llvm-project-main/llvm/lib/Support/Unix/Signals.inc:798:1
  #2 0x00007f29074967df llvm::sys::RunSignalHandlers() /home/treehouse/source/llvm-project-main/llvm/lib/Support/Signals.cpp:105:5
  #3 0x00007f2907498cfe SignalHandler(int) /home/treehouse/source/llvm-project-main/llvm/lib/Support/Unix/Signals.inc:413:1
  #4 0x00007f2905c64320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
  #5 0x00007f2910cb35e7 (anonymous namespace)::CXXNameMangler::mangleType(clang::BuiltinType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3065:0
  #6 0x00007f2910cb25c1 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:37:1
  #7 0x00007f2910cb1895 (anonymous namespace)::CXXNameMangler::makeFunctionReturnTypeTags(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:7150:23
  #8 0x00007f2910cb117c (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:846:25
  #9 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
 #10 0x00007f2910cb1dd2 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1079:5
 #11 0x00007f2910cb1791 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #12 0x00007f2910cb11d3 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:856:5
 #13 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
 #14 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #15 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
 #16 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
 #17 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
 #18 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
 #19 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
 #20 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
 #21 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #22 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
 #23 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
 #24 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
 #25 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #26 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
 #27 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
 #28 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
 #29 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
 #30 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
 #31 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
 #32 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #33 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
 #34 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
 #35 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
 #36 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #37 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
 #38 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
 #39 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
 #40 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
 #41 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
 #42 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
 #43 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #44 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
 #45 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
 #46 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
 #47 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #48 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
 #49 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
 #50 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
 #51 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
 #52 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
 #53 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
 #54 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #55 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
 #56 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
 #57 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
 #58 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #59 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
 #60 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
 #61 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
 #62 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
 #63 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
 #64 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
 #65 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #66 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
 #67 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
 #68 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
 #69 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #70 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
 #71 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
 #72 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
 #73 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
 #74 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
 #75 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
 #76 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #77 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
 #78 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
 #79 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
 #80 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #81 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
 #82 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
 #83 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
 #84 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
 #85 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
 #86 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
 #87 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #88 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
 #89 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
 #90 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
 #91 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #92 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
 #93 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
 #94 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
 #95 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
 #96 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
 #97 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
 #98 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #99 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#100 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#101 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#102 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#103 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#104 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#105 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#106 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#107 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#108 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#109 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#110 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#111 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#112 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#113 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#114 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#115 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#116 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#117 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#118 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#119 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#120 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#121 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#122 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#123 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#124 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#125 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#126 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#127 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#128 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#129 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#130 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#131 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#132 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#133 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#134 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#135 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#136 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#137 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#138 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#139 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#140 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#141 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#142 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#143 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#144 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#145 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#146 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#147 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#148 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#149 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#150 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#151 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#152 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#153 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#154 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#155 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#156 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#157 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#158 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#159 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#160 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#161 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#162 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#163 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#164 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#165 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#166 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#167 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#168 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#169 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#170 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#171 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#172 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#173 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#174 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#175 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#176 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#177 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#178 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#179 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#180 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#181 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#182 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#183 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#184 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#185 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#186 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#187 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#188 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#189 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#190 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#191 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#192 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#193 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#194 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#195 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#196 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#197 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#198 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#199 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#200 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#201 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#202 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#203 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#204 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#205 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#206 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#207 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#208 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#209 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#210 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#211 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#212 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#213 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#214 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#215 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#216 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#217 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#218 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#219 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#220 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#221 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#222 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#223 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#224 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#225 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#226 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#227 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#228 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#229 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#230 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#231 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#232 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#233 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#234 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#235 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#236 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5   
#237 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7        
#238 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#239 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#240 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#241 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#242 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#243 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#244 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
#245 0x00007f2910cb1ea9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1093:5
#246 0x00007f2910cb15ab (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:1020:7
#247 0x00007f2910cb0ef8 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:827:5
#248 0x00007f2910cc5c8f (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool)::$_2::operator()(clang::NamedDecl const*) const /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4832:7
#249 0x00007f2910cc1928 (anonymous namespace)::CXXNameMangler::mangleExpression(clang::Expr const*, unsigned int, bool) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:5634:5
#250 0x00007f2910cbc2f3 (anonymous namespace)::CXXNameMangler::mangleType(clang::DecltypeType const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:4499:3
#251 0x00007f2910cb25eb (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm-project-main/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:39:1
#252 0x00007f2910ccaad9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:3737:11
#253 0x00007f2910cb1bb0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:960:1
#254 0x00007f2910cb1205 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:858:5
#255 0x00007f2910ccdaaa (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project-main/clang/lib/AST/ItaniumMangle.cpp:0:7
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git 537e0e1ff639ed4f8fa4dadbc84f4a6a12e1d20a)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/treehouse/build/llvm-project-main/Clang_19-Debug/bin
Build config: +unoptimized, +assertions
```

Looks like a duplicate of #86240

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
```c++
template &lt;class T&gt;
void f(T t) {
    [x = t] (decltype(x)) {};
}
```

Crash since clang 17:
```console
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

Compiler Explorer: https://godbolt.org/z/6EWaoa79s

Maybe related to the implementation of this DR, but I am not sure:

* DR11: Change scope of lambda trailing-return-type ([P2036R3](https://wg21.link/P2036R3), [P2579R0](https://wg21.link/P2579R0))
</details>


---

