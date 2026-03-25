# [Clang][Regression:17] Crash: segfault or stack overflow on lambda in template argument in lambda parameter type

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/132076

## Body

The following C++20 code causes a crash since Clang 17:
```c++
template <class>
struct A {};

template <class T>
constexpr auto f(T) {
    return [] (A<decltype([] (T) {})>) {};
}

int main() {
    f(0)({});
}
```

See https://compiler-explorer.com/z/rTT78G6a7.

Note:
* Stack dumps are different on Ubuntu and Windows, but it looks like an infinite recursion on both platforms.
* I haven't found another similar issue that would crash on Windows like this yet. So, I believe this one is unique. 

---

<details>

<summary>On Ubuntu</summary>

Stack dump obtained with local build:
```console
0.      Program arguments: /home/treehouse/build/llvm/Clang_19-Debug/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/treehouse/project/examples -fcoverage-compilation-dir=/home/treehouse/project/examples -resource-dir /home/treehouse/build/llvm/Clang_19-Debug/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/x86_64-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /home/treehouse/build/llvm/Clang_19-Debug/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-489a9a.o -x c++ test.cpp
1.      test.cpp:10:8: current parser token ')'
2.      test.cpp:9:12: parsing function body 'main'
3.      test.cpp:9:12: in compound statement ('{}')
4.      test.cpp:5:16: instantiating function definition 'f<int>'
  #0 0x0000559431030dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/treehouse/source/llvm-project/llvm/lib/Support/Unix/Signals.inc:800:11
  #1 0x000055943103127b PrintStackTraceSignalHandler(void*) /home/treehouse/source/llvm-project/llvm/lib/Support/Unix/Signals.inc:876:1
  #2 0x000055943102f41f llvm::sys::RunSignalHandlers() /home/treehouse/source/llvm-project/llvm/lib/Support/Signals.cpp:105:5
  #3 0x000055943103193e SignalHandler(int) /home/treehouse/source/llvm-project/llvm/lib/Support/Unix/Signals.inc:417:1
  #4 0x00007fd3e1033330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
  #5 0x0000559436f78a02 (anonymous namespace)::CXXNameMangler::mangleType(clang::BuiltinType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3107:15
  #6 0x0000559436f779d1 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:37:1
  #7 0x0000559436f8b50a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6238:5
  #8 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
  #9 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #10 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #11 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #12 0x0000559436f765e3 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:856:5
 #13 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #14 0x0000559436f771e2 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1079:5
 #15 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #16 0x0000559436f765e3 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:856:5
 #17 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #18 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #19 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #20 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #21 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #22 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #23 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #24 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #25 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #26 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #27 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #28 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #29 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #30 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #31 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #32 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #33 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
 #34 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
 #35 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #36 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #37 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #38 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #39 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #40 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #41 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #42 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #43 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #44 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #45 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #46 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #47 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #48 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #49 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #50 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #51 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
 #52 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
 #53 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #54 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #55 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #56 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #57 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #58 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #59 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #60 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #61 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #62 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #63 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #64 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #65 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #66 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #67 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #68 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #69 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
 #70 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
 #71 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #72 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #73 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #74 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #75 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #76 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #77 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #78 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #79 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #80 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #81 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #82 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #83 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #84 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #85 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #86 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #87 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
 #88 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
 #89 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #90 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #91 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #92 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #93 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #94 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #95 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #96 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #97 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #98 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #99 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
#100 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
#101 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
#102 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
#103 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
#104 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
#105 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
#106 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
#107 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
#108 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
#109 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
#110 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
#111 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
#112 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
#113 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
#114 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
#115 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
#116 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
#117 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
#118 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
#119 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
#120 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
#121 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
#122 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
#123 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
#124 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
#125 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
#126 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
#127 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
```
(truncated)

</details>

---

<details>

<summary>On Windows</summary>

Using a debugger shows a stack overflow:
```
Exception thrown at 0x00007FF72F42CD13 in clang.exe: 0xC00000FD: Stack overflow (parameters: 0x0000000000000001, 0x00000072ACC03FF8).
```

Stack dump (obtained by debugger):
```console
clang.exe!llvm::DenseMapInfo<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,void>::getEmptyKey() Line 225 (d:\source\llvm-project\llvm\include\llvm\ADT\PointerIntPair.h:225)
clang.exe!llvm::DenseMapBase<llvm::SmallDenseMap<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo,8,llvm::DenseMapInfo<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,void>,llvm::detail::DenseMapPair<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo>>,llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo,llvm::DenseMapInfo<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,void>,llvm::detail::DenseMapPair<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo>>::getEmptyKey() Line 471 (d:\source\llvm-project\llvm\include\llvm\ADT\DenseMap.h:471)
clang.exe!llvm::DenseMapBase<llvm::SmallDenseMap<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo,8,llvm::DenseMapInfo<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,void>,llvm::detail::DenseMapPair<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo>>,llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo,llvm::DenseMapInfo<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,void>,llvm::detail::DenseMapPair<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo>>::initEmpty() Line 390 (d:\source\llvm-project\llvm\include\llvm\ADT\DenseMap.h:390)
clang.exe!llvm::SmallDenseMap<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo,8,llvm::DenseMapInfo<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,void>,llvm::detail::DenseMapPair<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo>>::init(unsigned int InitBuckets) Line 1041 (d:\source\llvm-project\llvm\include\llvm\ADT\DenseMap.h:1041)
clang.exe!llvm::SmallDenseMap<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo,8,llvm::DenseMapInfo<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,void>,llvm::detail::DenseMapPair<llvm::PointerIntPair<const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>,llvm::PointerIntPairInfo<const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits<const clang::NamedDecl *>>>,clang::LinkageInfo>>::SmallDenseMap(unsigned int NumInitBuckets) Line 911 (d:\source\llvm-project\llvm\include\llvm\ADT\DenseMap.h:911)
clang.exe!clang::LinkageComputer::LinkageComputer() Line 77 (d:\source\llvm-project\clang\lib\AST\Linkage.h:77)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type & T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef<clang::TemplateArgument> Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList & TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo & LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type & T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef<clang::TemplateArgument> Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList & TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo & LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type & T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef<clang::TemplateArgument> Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList & TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo & LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type & T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef<clang::TemplateArgument> Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList & TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo & LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type & T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef<clang::TemplateArgument> Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList & TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo & LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type & T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef<clang::TemplateArgument> Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList & TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo & LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type & T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef<clang::TemplateArgument> Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList & TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo & LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache<(anonymous namespace)::Private>::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
```
(truncated)

</details>


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
The following C++20 code causes a crash since Clang 17:
```c++
template &lt;class&gt;
struct A {};

template &lt;class T&gt;
constexpr auto f(T) {
    return [] (A&lt;decltype([] (T) {})&gt;) {};
}

int main() {
    f(0)({});
}
```

See https://compiler-explorer.com/z/rTT78G6a7.

Note:
* Stack dumps are different on Ubuntu and Windows, but it looks like an infinite recursion on both platforms.
* I haven't found another similar issue that would crash on Windows like this yet. So, I believe this one is unique. 

---

&lt;details&gt;

&lt;summary&gt;On Ubuntu&lt;/summary&gt;

Stack dump obtained with local build:
```console
0.      Program arguments: /home/treehouse/build/llvm/Clang_19-Debug/bin/clang-20 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name test.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/treehouse/project/examples -fcoverage-compilation-dir=/home/treehouse/project/examples -resource-dir /home/treehouse/build/llvm/Clang_19-Debug/lib/clang/20 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/x86_64-linux-gnu/c++/14 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../include/c++/14/backward -internal-isystem /home/treehouse/build/llvm/Clang_19-Debug/lib/clang/20/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/14/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/test-489a9a.o -x c++ test.cpp
1.      test.cpp:10:8: current parser token ')'
2.      test.cpp:9:12: parsing function body 'main'
3.      test.cpp:9:12: in compound statement ('{}')
4.      test.cpp:5:16: instantiating function definition 'f&lt;int&gt;'
  #<!-- -->0 0x0000559431030dbd llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /home/treehouse/source/llvm-project/llvm/lib/Support/Unix/Signals.inc:800:11
  #<!-- -->1 0x000055943103127b PrintStackTraceSignalHandler(void*) /home/treehouse/source/llvm-project/llvm/lib/Support/Unix/Signals.inc:876:1
  #<!-- -->2 0x000055943102f41f llvm::sys::RunSignalHandlers() /home/treehouse/source/llvm-project/llvm/lib/Support/Signals.cpp:105:5
  #<!-- -->3 0x000055943103193e SignalHandler(int) /home/treehouse/source/llvm-project/llvm/lib/Support/Unix/Signals.inc:417:1
  #<!-- -->4 0x00007fd3e1033330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
  #<!-- -->5 0x0000559436f78a02 (anonymous namespace)::CXXNameMangler::mangleType(clang::BuiltinType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3107:15
  #<!-- -->6 0x0000559436f779d1 (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:37:1
  #<!-- -->7 0x0000559436f8b50a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6238:5
  #<!-- -->8 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
  #<!-- -->9 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #<!-- -->10 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #<!-- -->11 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->12 0x0000559436f765e3 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:856:5
 #<!-- -->13 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #<!-- -->14 0x0000559436f771e2 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1079:5
 #<!-- -->15 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->16 0x0000559436f765e3 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:856:5
 #<!-- -->17 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #<!-- -->18 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #<!-- -->19 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->20 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #<!-- -->21 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #<!-- -->22 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #<!-- -->23 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&amp;, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #<!-- -->24 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #<!-- -->25 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #<!-- -->26 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #<!-- -->27 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #<!-- -->28 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->29 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #<!-- -->30 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #<!-- -->31 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #<!-- -->32 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #<!-- -->33 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
 #<!-- -->34 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
 #<!-- -->35 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #<!-- -->36 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #<!-- -->37 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->38 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #<!-- -->39 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #<!-- -->40 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #<!-- -->41 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&amp;, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #<!-- -->42 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #<!-- -->43 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #<!-- -->44 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #<!-- -->45 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #<!-- -->46 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->47 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #<!-- -->48 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #<!-- -->49 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #<!-- -->50 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #<!-- -->51 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
 #<!-- -->52 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
 #<!-- -->53 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #<!-- -->54 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #<!-- -->55 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->56 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #<!-- -->57 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #<!-- -->58 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #<!-- -->59 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&amp;, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #<!-- -->60 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #<!-- -->61 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #<!-- -->62 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #<!-- -->63 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #<!-- -->64 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->65 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #<!-- -->66 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #<!-- -->67 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #<!-- -->68 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #<!-- -->69 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
 #<!-- -->70 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
 #<!-- -->71 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #<!-- -->72 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #<!-- -->73 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->74 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #<!-- -->75 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #<!-- -->76 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #<!-- -->77 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&amp;, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #<!-- -->78 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #<!-- -->79 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #<!-- -->80 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #<!-- -->81 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
 #<!-- -->82 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->83 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #<!-- -->84 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #<!-- -->85 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #<!-- -->86 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
 #<!-- -->87 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
 #<!-- -->88 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
 #<!-- -->89 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
 #<!-- -->90 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
 #<!-- -->91 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
 #<!-- -->92 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
 #<!-- -->93 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
 #<!-- -->94 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
 #<!-- -->95 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&amp;, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
 #<!-- -->96 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
 #<!-- -->97 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
 #<!-- -->98 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
 #<!-- -->99 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
#<!-- -->100 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
#<!-- -->101 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
#<!-- -->102 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
#<!-- -->103 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
#<!-- -->104 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
#<!-- -->105 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
#<!-- -->106 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
#<!-- -->107 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
#<!-- -->108 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
#<!-- -->109 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
#<!-- -->110 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
#<!-- -->111 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
#<!-- -->112 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
#<!-- -->113 0x0000559436f88239 (anonymous namespace)::CXXNameMangler::mangleValueInTemplateArg(clang::QualType, clang::APValue const&amp;, bool, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6538:19
#<!-- -->114 0x0000559436f8b65a (anonymous namespace)::CXXNameMangler::mangleTemplateArg(clang::TemplateArgument, bool) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6265:7
#<!-- -->115 0x0000559436f8d0c7 (anonymous namespace)::CXXNameMangler::mangleTemplateArg((anonymous namespace)::CXXNameMangler::TemplateArgManglingInfo&amp;, unsigned int, clang::TemplateArgument) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6222:1
#<!-- -->116 0x0000559436f8f5e7 (anonymous namespace)::CXXNameMangler::mangleTemplateArgs(clang::TemplateName, clang::TemplateArgumentList const&amp;) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:6186:47
#<!-- -->117 0x0000559436f773c9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1102:7
#<!-- -->118 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
#<!-- -->119 0x0000559436f91b3d (anonymous namespace)::CXXNameMangler::mangleType(clang::TagType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3779:1
#<!-- -->120 0x0000559436f82cad (anonymous namespace)::CXXNameMangler::mangleType(clang::RecordType const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3776:1
#<!-- -->121 0x0000559436f77caa (anonymous namespace)::CXXNameMangler::mangleType(clang::QualType) /home/treehouse/build/llvm/Clang_19-Debug/tools/clang/include/clang/AST/TypeNodes.inc:76:1
#<!-- -->122 0x0000559436f8fee9 (anonymous namespace)::CXXNameMangler::mangleBareFunctionType(clang::FunctionProtoType const*, bool, clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:3737:11
#<!-- -->123 0x0000559436f76fc0 (anonymous namespace)::CXXNameMangler::mangleFunctionEncodingBareType(clang::FunctionDecl const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:960:1
#<!-- -->124 0x0000559436f76615 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:858:5
#<!-- -->125 0x0000559436f92eba (anonymous namespace)::CXXNameMangler::mangleLocalName(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:0:7
#<!-- -->126 0x0000559436f772b9 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1093:5
#<!-- -->127 0x0000559436f76ba1 (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) /home/treehouse/source/llvm-project/clang/lib/AST/ItaniumMangle.cpp:1045:1
```
(truncated)

&lt;/details&gt;

---

&lt;details&gt;

&lt;summary&gt;On Windows&lt;/summary&gt;

Using a debugger shows a stack overflow:
```
Exception thrown at 0x00007FF72F42CD13 in clang.exe: 0xC00000FD: Stack overflow (parameters: 0x0000000000000001, 0x00000072ACC03FF8).
```

Stack dump (obtained by debugger):
```console
clang.exe!llvm::DenseMapInfo&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,void&gt;::getEmptyKey() Line 225 (d:\source\llvm-project\llvm\include\llvm\ADT\PointerIntPair.h:225)
clang.exe!llvm::DenseMapBase&lt;llvm::SmallDenseMap&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo,8,llvm::DenseMapInfo&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,void&gt;,llvm::detail::DenseMapPair&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo&gt;&gt;,llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo,llvm::DenseMapInfo&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,void&gt;,llvm::detail::DenseMapPair&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo&gt;&gt;::getEmptyKey() Line 471 (d:\source\llvm-project\llvm\include\llvm\ADT\DenseMap.h:471)
clang.exe!llvm::DenseMapBase&lt;llvm::SmallDenseMap&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo,8,llvm::DenseMapInfo&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,void&gt;,llvm::detail::DenseMapPair&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo&gt;&gt;,llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo,llvm::DenseMapInfo&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,void&gt;,llvm::detail::DenseMapPair&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo&gt;&gt;::initEmpty() Line 390 (d:\source\llvm-project\llvm\include\llvm\ADT\DenseMap.h:390)
clang.exe!llvm::SmallDenseMap&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo,8,llvm::DenseMapInfo&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,void&gt;,llvm::detail::DenseMapPair&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo&gt;&gt;::init(unsigned int InitBuckets) Line 1041 (d:\source\llvm-project\llvm\include\llvm\ADT\DenseMap.h:1041)
clang.exe!llvm::SmallDenseMap&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo,8,llvm::DenseMapInfo&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,void&gt;,llvm::detail::DenseMapPair&lt;llvm::PointerIntPair&lt;const clang::NamedDecl *,3,unsigned int,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;,llvm::PointerIntPairInfo&lt;const clang::NamedDecl *,3,llvm::PointerLikeTypeTraits&lt;const clang::NamedDecl *&gt;&gt;&gt;,clang::LinkageInfo&gt;&gt;::SmallDenseMap(unsigned int NumInitBuckets) Line 911 (d:\source\llvm-project\llvm\include\llvm\ADT\DenseMap.h:911)
clang.exe!clang::LinkageComputer::LinkageComputer() Line 77 (d:\source\llvm-project\clang\lib\AST\Linkage.h:77)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type &amp; T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef&lt;clang::TemplateArgument&gt; Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList &amp; TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo &amp; LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type &amp; T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef&lt;clang::TemplateArgument&gt; Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList &amp; TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo &amp; LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type &amp; T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef&lt;clang::TemplateArgument&gt; Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList &amp; TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo &amp; LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type &amp; T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef&lt;clang::TemplateArgument&gt; Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList &amp; TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo &amp; LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type &amp; T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef&lt;clang::TemplateArgument&gt; Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList &amp; TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo &amp; LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type &amp; T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef&lt;clang::TemplateArgument&gt; Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList &amp; TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo &amp; LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::LinkageComputer::getLVForClosure(const clang::DeclContext * DC, clang::Decl * ContextDecl, clang::LVComputationKind computation) Line 1342 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1342)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1517 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1517)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForType(const clang::Type &amp; T, clang::LVComputationKind computation) Line 243 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:243)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(llvm::ArrayRef&lt;clang::TemplateArgument&gt; Args, clang::LVComputationKind computation) Line 332 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:332)
clang.exe!clang::LinkageComputer::getLVForTemplateArgumentList(const clang::TemplateArgumentList &amp; TArgs, clang::LVComputationKind computation) Line 371 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:371)
clang.exe!clang::LinkageComputer::mergeTemplateLV(clang::LinkageInfo &amp; LV, const clang::ClassTemplateSpecializationDecl * spec, clang::LVComputationKind computation) Line 491 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:491)
clang.exe!clang::LinkageComputer::getLVForNamespaceScopeDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 864 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:864)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1537 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1537)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
clang.exe!clang::NamedDecl::getLinkageInternal() Line 1180 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1180)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4605 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4605)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(const clang::Type * T) Line 4529 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4529)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::get(clang::QualType T) Line 4524 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4524)
clang.exe!computeCachedProperties(const clang::Type * T) Line 4645 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4645)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4550 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4550)
clang.exe!clang::TypePropertyCache&lt;(anonymous namespace)::Private&gt;::ensure(const clang::Type * T) Line 4542 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4542)
clang.exe!clang::Type::getLinkage() Line 4669 (d:\source\llvm-project\clang\lib\AST\Type.cpp:4669)
clang.exe!clang::LinkageComputer::getLVForClassMember(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 979 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:979)
clang.exe!clang::LinkageComputer::computeLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation, bool IgnoreVarTypeLinkage) Line 1547 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1547)
clang.exe!clang::LinkageComputer::getLVForDecl(const clang::NamedDecl * D, clang::LVComputationKind computation) Line 1582 (d:\source\llvm-project\clang\lib\AST\Decl.cpp:1582)
```
(truncated)

&lt;/details&gt;

</details>


---

### Comment 2 - shafik

Maybe related: https://github.com/llvm/llvm-project/issues/88592

---

