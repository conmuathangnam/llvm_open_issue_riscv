# Clang crashes when specializing and using a structured binding for std::expected in a C++ module

**Author:** tomboehmer
**URL:** https://github.com/llvm/llvm-project/issues/174439
**Status:** Closed
**Labels:** clang:modules, crash
**Closed Date:** 2026-01-09T01:51:06Z

## Body

This code crashes clang:

```c++
export module error;

import std;

using std::size_t;

template<typename T, typename E>
struct std::tuple_size<std::expected<T, E>> {
    static constexpr size_t value = 2;
};

template<size_t I, typename ...Ts>
struct std::tuple_element<I, std::expected<Ts...>> {
    using type = Ts...[I];
};

template<size_t I, typename E>
auto unpack_std_expected(E&&) -> std::tuple_element<I, std::remove_cvref_t<E>>::type {
    return {};
}

namespace std {
    template<size_t I, typename T, typename E>
    auto get(std::expected<T, E> const& e) { return unpack_std_expected<I>(e); }

    template<size_t I, typename T, typename E>
    auto get(std::expected<T, E>&& e) { return unpack_std_expected<I>(std::move(e)); }
};

enum class ParseError {
    Ok,
};

struct ParseResult{};

auto parse() -> std::expected<ParseResult, ParseError> {
    return {};
}

auto use_parse() -> std::expected<ParseResult, ParseError> {
    //return ::parse(); // This works.

    auto [ parsed, error ] = ::parse();
    if(error != ParseError::Ok) return std::unexpected(error);

    return parsed;
}

```

Output:
```text
$ cmake --build --preset debug-clang
[9/10] Building CXX object CMakeFiles/lib.dir/error.cppm.o
FAILED: [code=1] CMakeFiles/lib.dir/error.cppm.o CMakeFiles/lib.dir/error.pcm 
/usr/bin/clang++   -Wall -Wextra -Wpedantic -Wmost -fdiagnostics-color=always -g -std=gnu++26 -MD -MT CMakeFiles/lib.dir/error.cppm.o -MF CMakeFiles/lib.dir/error.cppm.o.d @CMakeFiles/lib.dir/error.cppm.o.modmap -o CMakeFiles/lib.dir/error.cppm.o -c /home/dep/workspace/bug_asn1/error.cppm
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -cc1 -triple x86_64-pc-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name error.cppm -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang -fcoverage-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang -resource-dir /usr/lib/clang/21 -Wall -Wextra -Wpedantic -Wmost -std=gnu++26 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fno-implicit-modules -fmodule-file=std=CMakeFiles/__cmake_cxx26.dir/std.pcm -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o CMakeFiles/lib.dir/error.cppm.o -x pcm CMakeFiles/lib.dir/error.pcm
1.	<eof> parser at end of file
2.	/home/dep/workspace/bug_asn1/error.cppm:40:6: LLVM IR generation of declaration 'use_parse'
3.	/home/dep/workspace/bug_asn1/error.cppm:40:6: Generating code for declaration 'use_parse'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.21.1      0x000075964bb081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 61
1  libLLVM.so.21.1      0x000075964bb052c7
2  libc.so.6            0x000075964aa3e4d0
3  libclang-cpp.so.21.1 0x000075965673bb0d clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) + 477
4  libclang-cpp.so.21.1 0x000075965673e855 clang::CodeGen::CGDebugInfo::CreateType(clang::TypedefType const*, llvm::DIFile*) + 53
5  libclang-cpp.so.21.1 0x000075965673b8df clang::CodeGen::CGDebugInfo::CreateTypeNode(clang::QualType, llvm::DIFile*) + 319
6  libclang-cpp.so.21.1 0x000075965673bd77 clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) + 1095
7  libclang-cpp.so.21.1 0x000075965673e4b5 clang::CodeGen::CGDebugInfo::CreatePointerLikeType(llvm::dwarf::Tag, clang::Type const*, clang::QualType, llvm::DIFile*) + 821
8  libclang-cpp.so.21.1 0x000075965673b88a clang::CodeGen::CGDebugInfo::CreateTypeNode(clang::QualType, llvm::DIFile*) + 234
9  libclang-cpp.so.21.1 0x000075965673bd77 clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) + 1095
10 libclang-cpp.so.21.1 0x000075965675806b clang::CodeGen::CGDebugInfo::EmitDeclare(clang::VarDecl const*, llvm::Value*, std::optional<unsigned int>, clang::CodeGen::CGBuilderTy&, bool) + 1707
11 libclang-cpp.so.21.1 0x000075965679a06e clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&) + 1326
12 libclang-cpp.so.21.1 0x000075965679b05e clang::CodeGen::CodeGenFunction::EmitAutoVarDecl(clang::VarDecl const&) + 46
13 libclang-cpp.so.21.1 0x000075965679b3b2
14 libclang-cpp.so.21.1 0x0000759656969d74 clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) + 148
15 libclang-cpp.so.21.1 0x000075965697a33d clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) + 765
16 libclang-cpp.so.21.1 0x00007596569743dc clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) + 156
17 libclang-cpp.so.21.1 0x00007596569fc1b3 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) + 99
18 libclang-cpp.so.21.1 0x0000759656a2b2b3 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) + 2419
19 libclang-cpp.so.21.1 0x0000759656a318bf clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 415
20 libclang-cpp.so.21.1 0x0000759656a2d724 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 564
21 libclang-cpp.so.21.1 0x0000759656a2e2b1 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) + 2833
22 libclang-cpp.so.21.1 0x0000759656a63359
23 libclang-cpp.so.21.1 0x0000759656ad0ef1
24 libclang-cpp.so.21.1 0x00007596569e883c clang::BackendConsumer::HandleInterestingDecl(clang::DeclGroupRef) + 204
25 libclang-cpp.so.21.1 0x00007596570aab8e
26 libclang-cpp.so.21.1 0x0000759656ffac48 non-virtual thunk to clang::ASTReader::StartTranslationUnit(clang::ASTConsumer*) + 56
27 libclang-cpp.so.21.1 0x00007596551714d6 clang::ParseAST(clang::Sema&, bool, bool) + 470
28 libclang-cpp.so.21.1 0x000075965724e8dc clang::FrontendAction::Execute() + 44
29 libclang-cpp.so.21.1 0x00007596571afd4f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 495
30 libclang-cpp.so.21.1 0x00007596572abb2c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 588
31 clang++              0x00005d504a830e9a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 11418
32 clang++              0x00005d504a834b61
33 clang++              0x00005d504a836604 clang_main(int, char**, llvm::ToolContext const&) + 6404
34 clang++              0x00005d504a8282b0 main + 128
35 libc.so.6            0x000075964aa27635
36 libc.so.6            0x000075964aa276e9 __libc_start_main + 137
37 clang++              0x00005d504a828315 _start + 37
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.6
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/error-ad79bd.cppm
clang++: note: diagnostic msg: /tmp/error-ad79bd.sh
clang++: note: diagnostic msg: 

********************
ninja: build stopped: subcommand failed.
```

/tmp/error-ad79bd.cppm:
```c++
# 1 "/home/dep/workspace/bug_asn1/error.cppm"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 499 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/home/dep/workspace/bug_asn1/error.cppm" 2
export module error;

import std;

using std::size_t;

template<typename T, typename E>
struct std::tuple_size<std::expected<T, E>> {
    static constexpr size_t value = 2;
};

template<size_t I, typename ...Ts>
struct std::tuple_element<I, std::expected<Ts...>> {
    using type = Ts...[I];
};

template<size_t I, typename E>
auto unpack_std_expected(E&&) -> std::tuple_element<I, std::remove_cvref_t<E>>::type {
    return {};
}

namespace std {
    template<size_t I, typename T, typename E>
    auto get(std::expected<T, E> const& e) { return unpack_std_expected<I>(e); }

    template<size_t I, typename T, typename E>
    auto get(std::expected<T, E>&& e) { return unpack_std_expected<I>(std::move(e)); }
};

enum class ParseError {
    Ok,
};

struct ParseResult{};

auto parse() -> std::expected<ParseResult, ParseError> {
    return {};
}

auto use_parse() -> std::expected<ParseResult, ParseError> {


    auto [ parsed, error ] = ::parse();
    if(error != ParseError::Ok) return std::unexpected(error);

    return parsed;
}
```

/tmp/error-ad79bd.sh:
```
# Crash reproducer for clang version 21.1.6
# Driver args: "--driver-mode=g++" "-Wall" "-Wextra" "-Wpedantic" "-Wmost" "-fdiagnostics-color=always" "-g" "-std=gnu++26" "-MD" "-MT" "CMakeFiles/lib.dir/error.cppm.o" "-MF" "CMakeFiles/lib.dir/error.cppm.o.d" "-x" "c++-module" "-fmodule-output=CMakeFiles/lib.dir/error.pcm" "-fmodule-file=std=CMakeFiles/__cmake_cxx26.dir/std.pcm" "-o" "CMakeFiles/lib.dir/error.cppm.o" "-c" "/home/dep/workspace/bug_asn1/error.cppm"
# Original command:  "/usr/bin/clang++" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "error.cppm" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debug-info-kind=constructor" "-dwarf-version=5" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang" "-fcoverage-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang" "-resource-dir" "/usr/lib/clang/21" "-Wall" "-Wextra" "-Wpedantic" "-Wmost" "-std=gnu++26" "-fdeprecated-macro" "-ferror-limit" "19" "-stack-protector" "2" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fmodule-file=std=CMakeFiles/__cmake_cxx26.dir/std.pcm" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "CMakeFiles/lib.dir/error.cppm.o" "-x" "pcm" "CMakeFiles/lib.dir/error.pcm"
 "/usr/bin/clang++" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "error.cppm" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debug-info-kind=constructor" "-dwarf-version=5" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang" "-fcoverage-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang" "-Wall" "-Wextra" "-Wpedantic" "-Wmost" "-std=gnu++26" "-fdeprecated-macro" "-ferror-limit" "19" "-stack-protector" "2" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fmodule-file=std=CMakeFiles/__cmake_cxx26.dir/std.pcm" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "pcm" "error-ad79bd.cppm"
```

CMakeLists.txt
```CMake
cmake_minimum_required(VERSION 4.0.0)

project(test-test LANGUAGES CXX)

add_library(lib)
target_sources(lib
	PUBLIC FILE_SET CXX_MODULES FILES
		${CMAKE_CURRENT_SOURCE_DIR}/error.cppm
)
target_compile_features(lib PUBLIC cxx_std_26)
```

CMakePresets.json
```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "base",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/${presetName}",
      "cacheVariables": {
        "CMAKE_CXX_MODULE_STD": "1",
        "CMAKE_EXPERIMENTAL_CXX_IMPORT_STD": "d0edc3af-4c50-42ea-a356-e2862fe7a444"
      }
    },
    {
      "name": "base-clang",
      "inherits": ["base"],
      "binaryDir": "${sourceDir}/build/${presetName}",
      "cacheVariables": {
        "CMAKE_CXX_COMPILER": "clang++",
        "CMAKE_CXX_FLAGS": "-Wall -Wextra -Wpedantic -Wmost -fdiagnostics-color=always"
      }
    },
    {
      "name": "debug-clang",
      "inherits": ["base-clang"],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "debug-clang",
      "configurePreset": "debug-clang"
    }
  ]
}
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: None (tomboehmer)

<details>
This code crashes clang:

```c++
export module error;

import std;

using std::size_t;

template&lt;typename T, typename E&gt;
struct std::tuple_size&lt;std::expected&lt;T, E&gt;&gt; {
    static constexpr size_t value = 2;
};

template&lt;size_t I, typename ...Ts&gt;
struct std::tuple_element&lt;I, std::expected&lt;Ts...&gt;&gt; {
    using type = Ts...[I];
};

template&lt;size_t I, typename E&gt;
auto unpack_std_expected(E&amp;&amp;) -&gt; std::tuple_element&lt;I, std::remove_cvref_t&lt;E&gt;&gt;::type {
    return {};
}

namespace std {
    template&lt;size_t I, typename T, typename E&gt;
    auto get(std::expected&lt;T, E&gt; const&amp; e) { return unpack_std_expected&lt;I&gt;(e); }

    template&lt;size_t I, typename T, typename E&gt;
    auto get(std::expected&lt;T, E&gt;&amp;&amp; e) { return unpack_std_expected&lt;I&gt;(std::move(e)); }
};

enum class ParseError {
    Ok,
};

struct ParseResult{};

auto parse() -&gt; std::expected&lt;ParseResult, ParseError&gt; {
    return {};
}

auto use_parse() -&gt; std::expected&lt;ParseResult, ParseError&gt; {
    //return ::parse(); // This works.

    auto [ parsed, error ] = ::parse();
    if(error != ParseError::Ok) return std::unexpected(error);

    return parsed;
}

```

Output:
```text
$ cmake --build --preset debug-clang
[9/10] Building CXX object CMakeFiles/lib.dir/error.cppm.o
FAILED: [code=1] CMakeFiles/lib.dir/error.cppm.o CMakeFiles/lib.dir/error.pcm 
/usr/bin/clang++   -Wall -Wextra -Wpedantic -Wmost -fdiagnostics-color=always -g -std=gnu++26 -MD -MT CMakeFiles/lib.dir/error.cppm.o -MF CMakeFiles/lib.dir/error.cppm.o.d @<!-- -->CMakeFiles/lib.dir/error.cppm.o.modmap -o CMakeFiles/lib.dir/error.cppm.o -c /home/dep/workspace/bug_asn1/error.cppm
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++ -cc1 -triple x86_64-pc-linux-gnu -emit-obj -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name error.cppm -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang -fcoverage-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang -resource-dir /usr/lib/clang/21 -Wall -Wextra -Wpedantic -Wmost -std=gnu++26 -fdeprecated-macro -ferror-limit 19 -stack-protector 2 -fgnuc-version=4.2.1 -fno-implicit-modules -fmodule-file=std=CMakeFiles/__cmake_cxx26.dir/std.pcm -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o CMakeFiles/lib.dir/error.cppm.o -x pcm CMakeFiles/lib.dir/error.pcm
1.	&lt;eof&gt; parser at end of file
2.	/home/dep/workspace/bug_asn1/error.cppm:40:6: LLVM IR generation of declaration 'use_parse'
3.	/home/dep/workspace/bug_asn1/error.cppm:40:6: Generating code for declaration 'use_parse'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.21.1      0x000075964bb081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 61
1  libLLVM.so.21.1      0x000075964bb052c7
2  libc.so.6            0x000075964aa3e4d0
3  libclang-cpp.so.21.1 0x000075965673bb0d clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) + 477
4  libclang-cpp.so.21.1 0x000075965673e855 clang::CodeGen::CGDebugInfo::CreateType(clang::TypedefType const*, llvm::DIFile*) + 53
5  libclang-cpp.so.21.1 0x000075965673b8df clang::CodeGen::CGDebugInfo::CreateTypeNode(clang::QualType, llvm::DIFile*) + 319
6  libclang-cpp.so.21.1 0x000075965673bd77 clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) + 1095
7  libclang-cpp.so.21.1 0x000075965673e4b5 clang::CodeGen::CGDebugInfo::CreatePointerLikeType(llvm::dwarf::Tag, clang::Type const*, clang::QualType, llvm::DIFile*) + 821
8  libclang-cpp.so.21.1 0x000075965673b88a clang::CodeGen::CGDebugInfo::CreateTypeNode(clang::QualType, llvm::DIFile*) + 234
9  libclang-cpp.so.21.1 0x000075965673bd77 clang::CodeGen::CGDebugInfo::getOrCreateType(clang::QualType, llvm::DIFile*) + 1095
10 libclang-cpp.so.21.1 0x000075965675806b clang::CodeGen::CGDebugInfo::EmitDeclare(clang::VarDecl const*, llvm::Value*, std::optional&lt;unsigned int&gt;, clang::CodeGen::CGBuilderTy&amp;, bool) + 1707
11 libclang-cpp.so.21.1 0x000075965679a06e clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&amp;) + 1326
12 libclang-cpp.so.21.1 0x000075965679b05e clang::CodeGen::CodeGenFunction::EmitAutoVarDecl(clang::VarDecl const&amp;) + 46
13 libclang-cpp.so.21.1 0x000075965679b3b2
14 libclang-cpp.so.21.1 0x0000759656969d74 clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) + 148
15 libclang-cpp.so.21.1 0x000075965697a33d clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) + 765
16 libclang-cpp.so.21.1 0x00007596569743dc clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) + 156
17 libclang-cpp.so.21.1 0x00007596569fc1b3 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) + 99
18 libclang-cpp.so.21.1 0x0000759656a2b2b3 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) + 2419
19 libclang-cpp.so.21.1 0x0000759656a318bf clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 415
20 libclang-cpp.so.21.1 0x0000759656a2d724 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) + 564
21 libclang-cpp.so.21.1 0x0000759656a2e2b1 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) + 2833
22 libclang-cpp.so.21.1 0x0000759656a63359
23 libclang-cpp.so.21.1 0x0000759656ad0ef1
24 libclang-cpp.so.21.1 0x00007596569e883c clang::BackendConsumer::HandleInterestingDecl(clang::DeclGroupRef) + 204
25 libclang-cpp.so.21.1 0x00007596570aab8e
26 libclang-cpp.so.21.1 0x0000759656ffac48 non-virtual thunk to clang::ASTReader::StartTranslationUnit(clang::ASTConsumer*) + 56
27 libclang-cpp.so.21.1 0x00007596551714d6 clang::ParseAST(clang::Sema&amp;, bool, bool) + 470
28 libclang-cpp.so.21.1 0x000075965724e8dc clang::FrontendAction::Execute() + 44
29 libclang-cpp.so.21.1 0x00007596571afd4f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 495
30 libclang-cpp.so.21.1 0x00007596572abb2c clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 588
31 clang++              0x00005d504a830e9a cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 11418
32 clang++              0x00005d504a834b61
33 clang++              0x00005d504a836604 clang_main(int, char**, llvm::ToolContext const&amp;) + 6404
34 clang++              0x00005d504a8282b0 main + 128
35 libc.so.6            0x000075964aa27635
36 libc.so.6            0x000075964aa276e9 __libc_start_main + 137
37 clang++              0x00005d504a828315 _start + 37
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.1.6
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/error-ad79bd.cppm
clang++: note: diagnostic msg: /tmp/error-ad79bd.sh
clang++: note: diagnostic msg: 

********************
ninja: build stopped: subcommand failed.
```

/tmp/error-ad79bd.cppm:
```c++
# 1 "/home/dep/workspace/bug_asn1/error.cppm"
# 1 "&lt;built-in&gt;" 1
# 1 "&lt;built-in&gt;" 3
# 499 "&lt;built-in&gt;" 3
# 1 "&lt;command line&gt;" 1
# 1 "&lt;built-in&gt;" 2
# 1 "/home/dep/workspace/bug_asn1/error.cppm" 2
export module error;

import std;

using std::size_t;

template&lt;typename T, typename E&gt;
struct std::tuple_size&lt;std::expected&lt;T, E&gt;&gt; {
    static constexpr size_t value = 2;
};

template&lt;size_t I, typename ...Ts&gt;
struct std::tuple_element&lt;I, std::expected&lt;Ts...&gt;&gt; {
    using type = Ts...[I];
};

template&lt;size_t I, typename E&gt;
auto unpack_std_expected(E&amp;&amp;) -&gt; std::tuple_element&lt;I, std::remove_cvref_t&lt;E&gt;&gt;::type {
    return {};
}

namespace std {
    template&lt;size_t I, typename T, typename E&gt;
    auto get(std::expected&lt;T, E&gt; const&amp; e) { return unpack_std_expected&lt;I&gt;(e); }

    template&lt;size_t I, typename T, typename E&gt;
    auto get(std::expected&lt;T, E&gt;&amp;&amp; e) { return unpack_std_expected&lt;I&gt;(std::move(e)); }
};

enum class ParseError {
    Ok,
};

struct ParseResult{};

auto parse() -&gt; std::expected&lt;ParseResult, ParseError&gt; {
    return {};
}

auto use_parse() -&gt; std::expected&lt;ParseResult, ParseError&gt; {


    auto [ parsed, error ] = ::parse();
    if(error != ParseError::Ok) return std::unexpected(error);

    return parsed;
}
```

/tmp/error-ad79bd.sh:
```
# Crash reproducer for clang version 21.1.6
# Driver args: "--driver-mode=g++" "-Wall" "-Wextra" "-Wpedantic" "-Wmost" "-fdiagnostics-color=always" "-g" "-std=gnu++26" "-MD" "-MT" "CMakeFiles/lib.dir/error.cppm.o" "-MF" "CMakeFiles/lib.dir/error.cppm.o.d" "-x" "c++-module" "-fmodule-output=CMakeFiles/lib.dir/error.pcm" "-fmodule-file=std=CMakeFiles/__cmake_cxx26.dir/std.pcm" "-o" "CMakeFiles/lib.dir/error.cppm.o" "-c" "/home/dep/workspace/bug_asn1/error.cppm"
# Original command:  "/usr/bin/clang++" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "error.cppm" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debug-info-kind=constructor" "-dwarf-version=5" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang" "-fcoverage-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang" "-resource-dir" "/usr/lib/clang/21" "-Wall" "-Wextra" "-Wpedantic" "-Wmost" "-std=gnu++26" "-fdeprecated-macro" "-ferror-limit" "19" "-stack-protector" "2" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fmodule-file=std=CMakeFiles/__cmake_cxx26.dir/std.pcm" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-o" "CMakeFiles/lib.dir/error.cppm.o" "-x" "pcm" "CMakeFiles/lib.dir/error.pcm"
 "/usr/bin/clang++" "-cc1" "-triple" "x86_64-pc-linux-gnu" "-emit-obj" "-disable-free" "-clear-ast-before-backend" "-disable-llvm-verifier" "-discard-value-names" "-main-file-name" "error.cppm" "-mrelocation-model" "pic" "-pic-level" "2" "-pic-is-pie" "-mframe-pointer=all" "-fmath-errno" "-ffp-contract=on" "-fno-rounding-math" "-mconstructor-aliases" "-funwind-tables=2" "-target-cpu" "x86-64" "-tune-cpu" "generic" "-debug-info-kind=constructor" "-dwarf-version=5" "-debugger-tuning=gdb" "-fdebug-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang" "-fcoverage-compilation-dir=/home/dep/workspace/bug_asn1/build/debug-clang" "-Wall" "-Wextra" "-Wpedantic" "-Wmost" "-std=gnu++26" "-fdeprecated-macro" "-ferror-limit" "19" "-stack-protector" "2" "-fgnuc-version=4.2.1" "-fno-implicit-modules" "-fmodule-file=std=CMakeFiles/__cmake_cxx26.dir/std.pcm" "-fskip-odr-check-in-gmf" "-fcxx-exceptions" "-fexceptions" "-fcolor-diagnostics" "-faddrsig" "-D__GCC_HAVE_DWARF2_CFI_ASM=1" "-x" "pcm" "error-ad79bd.cppm"
```

CMakeLists.txt
```CMake
cmake_minimum_required(VERSION 4.0.0)

project(test-test LANGUAGES CXX)

add_library(lib)
target_sources(lib
	PUBLIC FILE_SET CXX_MODULES FILES
		${CMAKE_CURRENT_SOURCE_DIR}/error.cppm
)
target_compile_features(lib PUBLIC cxx_std_26)
```

CMakePresets.json
```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "base",
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/${presetName}",
      "cacheVariables": {
        "CMAKE_CXX_MODULE_STD": "1",
        "CMAKE_EXPERIMENTAL_CXX_IMPORT_STD": "d0edc3af-4c50-42ea-a356-e2862fe7a444"
      }
    },
    {
      "name": "base-clang",
      "inherits": ["base"],
      "binaryDir": "${sourceDir}/build/${presetName}",
      "cacheVariables": {
        "CMAKE_CXX_COMPILER": "clang++",
        "CMAKE_CXX_FLAGS": "-Wall -Wextra -Wpedantic -Wmost -fdiagnostics-color=always"
      }
    },
    {
      "name": "debug-clang",
      "inherits": ["base-clang"],
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "debug-clang",
      "configurePreset": "debug-clang"
    }
  ]
}
```
</details>


---

### Comment 2 - ChuanqiXu9

Failed to reproduce with both libstdc++ and libc++.

What's your version of std module and what's your environment.

Or maybe you can try with clang trunk.

---

### Comment 3 - tomboehmer

I have tried the example in the following environments. I also could not reproduce the crash with clang trunk. But 21.1.x crashes reliably for me. The error message above is from Archlinux.

- Archlinx
  - clang: 21.1.6 (official repo)
  - libc++: 21.1.6 (official repo)
  - libstdc++: 15.2.1 (official repo)
  - cmake: 4.2.1 (official repo)
  - problem: yes, crash (both stdlibs)

- alpine:edge docker image under debian 13 host
  - clang: 21.1.8 (official repo)
  - libc++: 21.1.8 (official repo)
  - libstdc++: 15.2.0 (official repo)
  - cmake: 4.2.1 (official repo)
  - problem: yes, crash (both stdlibs)

- Debian 13
  - clang: 19.1.7 (official repo)
  - libc++: 19.1.7 (official repo)
  - cmake: 4.2.1 (prebuilt from github)
  - problem: different problem: got stuck linking lib__cmake_cxx26.a

- Debian 13
  - clang: 21.1.4 (built with spack)
  - libc++: 21.1.4 (built with spack)
  - cmake: 4.2.1 (prebuilt from github)
  - problem: yes, crash

- Debian 13
  - clang: trunk (22.0.0, built with spack)
  - libc++: trunk (22.0.0, built with spack)
  - cmake: 4.2.1 (prebuilt from github)
  - problem: no

This is the Dockerfile for the alpine:edge case:
```Dockerfile
FROM alpine:edge

USER root

RUN apk add --no-cache g++ cmake clang21 clang21-extra-tools libc++-dev ninja-build ninja-is-really-ninja bash llvm-libunwind-dev llvm-runtimes
RUN ln -sf /usr/lib/llvm-21/bin/clang-scan-deps /usr/local/bin/clang-scan-deps && \
    ln -sf /usr/bin/clang-21 /usr/bin/clang && \
    ln -sf /usr/bin/clang++-21 /usr/bin/clang++

```


---

### Comment 4 - ChuanqiXu9

Generally we won't fix bug which can't be reproduced in trunk. Especially the lifetime of clang-21 is going to end. I'd like to close this . Feel free to ask to reopen this if you have different opinions.

---

