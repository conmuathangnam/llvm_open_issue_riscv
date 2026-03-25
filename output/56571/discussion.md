# Crash when compiling strong_ordering with int statics

**Author:** msharov
**URL:** https://github.com/llvm/llvm-project/issues/56571
**Status:** Closed
**Labels:** c++20, clang:frontend, confirmed, crash-on-valid
**Closed Date:** 2025-12-16T11:45:32Z

## Body

clang crashes when compiling a `strong_ordering` class implementation
that does not contain `less/equal/greater` members of type `strong_ordering`.
I was trying to create something that would not require out-of-class inline
static variables, for which I get a warning when compiling with musl.

```c++
namespace std {
class strong_ordering {
    int _v;
public:
    constexpr strong_ordering (int v) :_v(v) {}
    constexpr operator int (void) const { return _v; }
    constexpr bool operator== (int v) const { return _v == v; }
    constexpr bool operator< (int v) const { return _v < v; }
    constexpr bool operator<= (int v) const { return _v <= v; }
    constexpr bool operator> (int v) const { return _v > v; }
    constexpr bool operator>= (int v) const { return _v >= v; }
    constexpr bool operator== (const strong_ordering& v) const { return _v == v._v; }
    constexpr bool operator< (const strong_ordering& v) const { return _v < v._v; }
    constexpr bool operator<= (const strong_ordering& v) const { return _v <= v._v; }
    constexpr bool operator> (const strong_ordering& v) const { return _v > v._v; }
    constexpr bool operator>= (const strong_ordering& v) const { return _v >= v._v; }
    static constexpr const int less = -1;
    static constexpr const int equal = 0;
    static constexpr const int greater = 1;
};
}

struct A {
    int v;
    auto operator<=> (const A& a) const = default;
};
```
Compiling with `clang --std=c++20 -c` produces:
```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang --std=c++20 -c tes.cc
1.	tes.cc:26:2: current parser token ';'
2.	tes.cc:23:1: parsing struct/union/class body 'A'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
/usr/lib/libLLVM-14.so(+0xcd553a)[0x7fa0dc08c53a]
/usr/lib/libLLVM-14.so(_ZN4llvm3sys17RunSignalHandlersEv+0x2f)[0x7fa0dc089bcf]
/usr/lib/libLLVM-14.so(+0xbbfac9)[0x7fa0dbf76ac9]
/usr/lib/libc.so.6(+0x3e8e0)[0x7fa0dafb28e0]
/usr/lib/libclang-cpp.so.14(_ZNK5clang10RecordDecl11field_beginEv+0x8)[0x7fa0e2ea5088]
/usr/lib/libclang-cpp.so.14(_ZNK5clang22ComparisonCategoryInfo9ValueInfo16hasValidIntValueEv+0x5f)[0x7fa0e2ec98bf]
/usr/lib/libclang-cpp.so.14(_ZN5clang4Sema27CheckComparisonCategoryTypeENS_22ComparisonCategoryTypeENS_14SourceLocationENS0_23ComparisonCategoryUsageE+0x630)[0x7fa0e34d0620]
/usr/lib/libclang-cpp.so.14(_ZN5clang4Sema34CheckExplicitlyDefaultedComparisonEPNS_5ScopeEPNS_12FunctionDeclENS0_23DefaultedComparisonKindE+0xcd8)[0x7fa0e34e1b88]
/usr/lib/libclang-cpp.so.14(_ZN5clang4Sema32CheckExplicitlyDefaultedFunctionEPNS_5ScopeEPNS_12FunctionDeclE+0x123)[0x7fa0e34e4f93]
/usr/lib/libclang-cpp.so.14(+0x1059ed2)[0x7fa0e34c3ed2]
/usr/lib/libclang-cpp.so.14(_ZN5clang4Sema22CheckCompletedCXXClassEPNS_5ScopeEPNS_13CXXRecordDeclE+0xb37)[0x7fa0e34ece67]
/usr/lib/libclang-cpp.so.14(_ZN5clang6Parser27ParseCXXMemberSpecificationENS_14SourceLocationES1_RNS_25ParsedAttributesWithRangeEjPNS_4DeclE+0x4f5)[0x7fa0e2c84185]
/usr/lib/libclang-cpp.so.14(_ZN5clang6Parser19ParseClassSpecifierENS_3tok9TokenKindENS_14SourceLocationERNS_8DeclSpecERKNS0_18ParsedTemplateInfoENS_15AccessSpecifierEbNS0_15DeclSpecContextERNS_25ParsedAttributesWithRangeE+0x1254)[0x7fa0e2c85ae4]
/usr/lib/libclang-cpp.so.14(_ZN5clang6Parser26ParseDeclarationSpecifiersERNS_8DeclSpecERKNS0_18ParsedTemplateInfoENS_15AccessSpecifierENS0_15DeclSpecContextEPNS0_18LateParsedAttrListE+0x737)[0x7fa0e2c69627]
/usr/lib/libclang-cpp.so.14(_ZN5clang6Parser30ParseDeclOrFunctionDefInternalERNS_25ParsedAttributesWithRangeERNS_15ParsingDeclSpecENS_15AccessSpecifierE+0x83)[0x7fa0e2ce3b73]
/usr/lib/libclang-cpp.so.14(+0x87a438)[0x7fa0e2ce4438]
/usr/lib/libclang-cpp.so.14(_ZN5clang6Parser24ParseExternalDeclarationERNS_25ParsedAttributesWithRangeEPNS_15ParsingDeclSpecE+0xa0e)[0x7fa0e2cf968e]
/usr/lib/libclang-cpp.so.14(_ZN5clang6Parser17ParseTopLevelDeclERNS_9OpaquePtrINS_12DeclGroupRefEEEb+0x256)[0x7fa0e2cf9a16]
/usr/lib/libclang-cpp.so.14(_ZN5clang8ParseASTERNS_4SemaEbb+0x238)[0x7fa0e2c43e38]
/usr/lib/libclang-cpp.so.14(_ZN5clang14FrontendAction7ExecuteEv+0x189)[0x7fa0e421d929]
/usr/lib/libclang-cpp.so.14(_ZN5clang16CompilerInstance13ExecuteActionERNS_14FrontendActionE+0x187)[0x7fa0e41bcdf7]
/usr/lib/libclang-cpp.so.14(_ZN5clang25ExecuteCompilerInvocationEPNS_16CompilerInstanceE+0x1ba)[0x7fa0e427478a]
clang(_Z8cc1_mainN4llvm8ArrayRefIPKcEES2_Pv+0x1131)[0x55aeefe8a6f1]
clang(+0x19337)[0x55aeefe8c337]
/usr/lib/libclang-cpp.so.14(+0x1aaa699)[0x7fa0e3f14699]
/usr/lib/libLLVM-14.so(_ZN4llvm20CrashRecoveryContext9RunSafelyENS_12function_refIFvvEEE+0x27)[0x7fa0dbf76b97]
/usr/lib/libclang-cpp.so.14(+0x1ab01ff)[0x7fa0e3f1a1ff]
/usr/lib/libclang-cpp.so.14(_ZNK5clang6driver11Compilation14ExecuteCommandERKNS0_7CommandERPS3_+0x93)[0x7fa0e3f2f903]
/usr/lib/libclang-cpp.so.14(_ZNK5clang6driver11Compilation11ExecuteJobsERKNS0_7JobListERN4llvm15SmallVectorImplISt4pairIiPKNS0_7CommandEEEE+0xf9)[0x7fa0e3f2fcd9]
/usr/lib/libclang-cpp.so.14(_ZN5clang6driver6Driver18ExecuteCompilationERNS0_11CompilationERN4llvm15SmallVectorImplISt4pairIiPKNS0_7CommandEEEE+0x122)[0x7fa0e3f38012]
clang(main+0x172a)[0x55aeefe8146a]
/usr/lib/libc.so.6(+0x29290)[0x7fa0daf9d290]
/usr/lib/libc.so.6(__libc_start_main+0x8a)[0x7fa0daf9d34a]
clang(_start+0x25)[0x55aeefe82655]
clang-14: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 14.0.6
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang-14: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-14: note: diagnostic msg: /tmp/me/tes-8c0234.cpp
clang-14: note: diagnostic msg: /tmp/me/tes-8c0234.sh
clang-14: note: diagnostic msg: 

********************
```
File `/tmp/me/tes-8c0234.cpp` contains the above source with additional header:
```
# 1 "tes.cc"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 432 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "tes.cc" 2
```
File `/tmp/me/tes-8c0234.sh` contains:
```shell
# Crash reproducer for clang version 14.0.6
# Driver args: "--std=c++20" "-c" "tes.cc"
# Original command:  "/usr/bin/clang-14" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "--mrelax-relocations" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "tes.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/home/me/progs/tmp" "-resource-dir" "/usr/lib/clang/14.0.6" "-internal-isystem" "/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/12.1.0/../../../../include/c++/12.1.0" "-internal-isystem" "/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/12.1.0/../../../../include/c++/12.1.0/x86_64-pc-linux-gnu" "-internal-isystem" "/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/12.1.0/../../../../include/c++/12.1.0/backward" "-internal-isystem" "/usr/lib/clang/14.0.6/include" "-internal-isystem" "/usr/local/include" "-internal-isystem" "/usr/bin/../lib64/gcc/x86_64-pc-linux-gnu/12.1.0/../../../../x86_64-pc-linux-gnu/include" "-internal-externc-isystem" "/include" "-internal-externc-isystem" "/usr/include" "--std=c++20" "-fdeprecated-macro" "-fdebug-compilation-dir=/home/me/progs/tmp" "-ferror-limit" "19" "-stack-protector" "2" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "tes.o" "-x" "c++" "tes.cc"
 "/usr/bin/clang-14" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-mrelax-all" "--mrelax-relocations" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "tes.cc" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-mllvm" "-treat-scalable-fixed-error-as-warning" "-debugger-tuning=gdb" "-fcoverage-compilation-dir=/home/me/progs/tmp" "--std=c++20" "-fdeprecated-macro" "-fdebug-compilation-dir=/home/me/progs/tmp" "-ferror-limit" "19" "-stack-protector" "2" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fcxx-exceptions" "-fexceptions" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "c++" "tes-8c0234.cpp"
```

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-c-20

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - shafik

gobolt reproducing issue: https://godbolt.org/z/bfq7qfcee

---

