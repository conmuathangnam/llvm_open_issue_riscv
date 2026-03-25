# [crash] fatal error: too many errors emitted, stopping now

**Author:** olegrok
**URL:** https://github.com/llvm/llvm-project/issues/163978

## Body

```
[2/3] Building CXX object src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o
FAILED: src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o 
ccache /usr/bin/ccache /usr/bin/clang++-21 -DBOOST_ALL_NO_LIB -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -DBOOST_NO_CXX98_FUNCTION_BASE -I/home/oleg/Projects/indexer/build/src -I/home/oleg/Projects/indexer/src -I/home/oleg/Projects/indexer/src/univalue/include -I/home/oleg/Projects/indexer/src/minisketch/include -I/home/oleg/Projects/indexer/src/secp256k1/include -O2 -g -std=c++23 -fPIE -fdebug-prefix-map=/home/oleg/Projects/indexer/src=. -fmacro-prefix-map=/home/oleg/Projects/indexer/src=. -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -Wstack-protector -fstack-protector-all -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wgnu -Wformat -Wformat-security -Wvla -Wshadow-field -Wthread-safety -Wthread-safety-pointer -Wloop-analysis -Wredundant-decls -Wunused-member-function -Wdate-time -Wconditional-uninitialized -Woverloaded-virtual -Wsuggest-override -Wimplicit-fallthrough -Wunreachable-code -Wdocumentation -Wself-assign -Wundef -Wno-unused-parameter -MD -MT src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o -MF src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o.d -o src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o -c /home/oleg/Projects/indexer/src/test/metaindex_btc_tests.cpp 
<built-in>:1:2: warning: null character ignored [-Wnull-character]
    1 | # 1 "<built-in>" 3
      |  ^
<built-in>:67:18: error: source file is not valid UTF-8
   67 | #define __cpp_digit_separators 201309L
      |                  ^
<built-in>:67:19: error: source file is not valid UTF-8
   67 | #define __cpp_digit_separators 201309L
      |                   ^
<built-in>:67:17: error: unknown type name 'P'
   67 | #define __cpp_digit_separators 201309L
      |                 ^
<built-in>:67:20: error: expected unqualified-id
   67 | #define __cpp_digit_separators 201309L
      |                    ^
<built-in>:67:27: error: source file is not valid UTF-8
   67 | #define __cpp_digit_separators 201309L
      |                           ^
<built-in>:68:3: error: source file is not valid UTF-8
   68 | #define __cpp_init_captures 201803L
      |   ^
<built-in>:68:4: error: source file is not valid UTF-8
   68 | #define __cpp_init_captures 201803L
      |    ^
<built-in>:69:22: error: source file is not valid UTF-8
   69 | #define __cpp_generic_lambdas 201707L
      |                      ^
<built-in>:69:23: error: source file is not valid UTF-8
   69 | #define __cpp_generic_lambdas 201707L
      |                       ^
<built-in>:69:24: error: source file is not valid UTF-8
   69 | #define __cpp_generic_lambdas 201707L
      |                        ^
<built-in>:69:32: error: source file is not valid UTF-8
   69 | #define __cpp_generic_lambdas 201707L
      |                                ^
<built-in>:70:9: error: source file is not valid UTF-8
   70 | #define __cpp_decltype_auto 201304L
      |         ^
<built-in>:70:10: error: source file is not valid UTF-8
   70 | #define __cpp_decltype_auto 201304L
      |          ^
<built-in>:71:28: error: source file is not valid UTF-8
   71 | #define __cpp_return_type_deduction 201304L
      |                            ^
<built-in>:71:29: error: source file is not valid UTF-8
   71 | #define __cpp_return_type_deduction 201304L
      |                             ^
<built-in>:71:30: error: source file is not valid UTF-8
   71 | #define __cpp_return_type_deduction 201304L
      |                              ^
<built-in>:71:38: error: source file is not valid UTF-8
   71 | #define __cpp_return_type_deduction 201304L
      |                                      ^
<built-in>:72:9: error: source file is not valid UTF-8
   72 | #define __cpp_aggregate_nsdmi 201304L
      |         ^
<built-in>:72:10: error: source file is not valid UTF-8
   72 | #define __cpp_aggregate_nsdmi 201304L
      |          ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++-21 -O2 -g -std=c++23 -fPIE -fdebug-prefix-map=/home/oleg/Projects/indexer/src=. -fmacro-prefix-map=/home/oleg/Projects/indexer/src=. -Wstack-protector -fstack-protector-all -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wgnu -Wformat -Wformat-security -Wvla -Wshadow-field -Wthread-safety -Wthread-safety-pointer -Wloop-analysis -Wredundant-decls -Wunused-member-function -Wdate-time -Wconditional-uninitialized -Woverloaded-virtual -Wsuggest-override -Wimplicit-fallthrough -Wunreachable-code -Wdocumentation -Wself-assign -Wundef -Wno-unused-parameter -DBOOST_ALL_NO_LIB -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -DBOOST_NO_CXX98_FUNCTION_BASE -I/home/oleg/Projects/indexer/build/src -I/home/oleg/Projects/indexer/src -I/home/oleg/Projects/indexer/src/univalue/include -I/home/oleg/Projects/indexer/src/minisketch/include -I/home/oleg/Projects/indexer/src/secp256k1/include -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -c -MD -MT src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o -MF src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o.d -fcolor-diagnostics -o src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o /home/oleg/Projects/indexer/src/test/metaindex_btc_tests.cpp
1.	<unknown> parser at unknown location
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.21.0      0x000071592a3b88f6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 54
1  libLLVM.so.21.0      0x000071592a3b61a3 llvm::sys::RunSignalHandlers() + 131
2  libLLVM.so.21.0      0x000071592a2f6680
3  libc.so.6            0x0000715929042520
4  libclang-cpp.so.21.0 0x000071593290ad05 clang::Lexer::SkipWhitespace(clang::Token&, char const*, bool&) + 37
5  libclang-cpp.so.21.0 0x000071593290e46c clang::Lexer::LexTokenInternal(clang::Token&, bool) + 1692
6  libclang-cpp.so.21.0 0x0000715932974ddd clang::Preprocessor::Lex(clang::Token&) + 61
7  libclang-cpp.so.21.0 0x0000715932982c39
8  libclang-cpp.so.21.0 0x0000715932a3a6d1 clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) + 1041
9  libclang-cpp.so.21.0 0x0000715932a3a5be clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) + 766
10 libclang-cpp.so.21.0 0x0000715932a3a5be clang::Parser::SkipUntil(llvm::ArrayRef<clang::tok::TokenKind>, clang::Parser::SkipUntilFlags) + 766
11 libclang-cpp.so.21.0 0x0000715932992206 clang::Parser::SkipMalformedDecl() + 310
12 libclang-cpp.so.21.0 0x00007159329910a6 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 2502
13 libclang-cpp.so.21.0 0x0000715932a3e88f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 1055
14 libclang-cpp.so.21.0 0x0000715932a3e28e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 590
15 libclang-cpp.so.21.0 0x0000715932a3d591 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 2081
16 libclang-cpp.so.21.0 0x0000715932a3bdc7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 1287
17 libclang-cpp.so.21.0 0x000071593297cb4e clang::ParseAST(clang::Sema&, bool, bool) + 798
18 libclang-cpp.so.21.0 0x000071593459539b clang::FrontendAction::Execute() + 43
19 libclang-cpp.so.21.0 0x0000715934508cc4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 948
20 libclang-cpp.so.21.0 0x000071593461b49e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 702
21 clang++-21           0x00005b8c72b569ab cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 6459
22 clang++-21           0x00005b8c72b53595
23 libclang-cpp.so.21.0 0x00007159341d3079
24 libLLVM.so.21.0      0x000071592a2f6348 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 136
25 libclang-cpp.so.21.0 0x00007159341d29fd clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 365
26 libclang-cpp.so.21.0 0x0000715934195dae clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 494
27 libclang-cpp.so.21.0 0x0000715934195f6e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 142
28 libclang-cpp.so.21.0 0x00007159341b4bed clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 365
29 clang++-21           0x00005b8c72b5307a clang_main(int, char**, llvm::ToolContext const&) + 6410
30 clang++-21           0x00005b8c72b60b26 main + 102
31 libc.so.6            0x0000715929029d90
32 libc.so.6            0x0000715929029e40 __libc_start_main + 128
33 clang++-21           0x00005b8c72b512e5 _start + 37
clang++-21: error: clang frontend command failed with exit code 135 (use -v to see invocation)
Ubuntu clang version 21.0.0 (++20250708042308+dcf485609c5c-1~exp1~20250708162410.2527)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang++-21: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-21: note: diagnostic msg: /tmp/metaindex_btc_tests-d2f4de.cpp
clang++-21: note: diagnostic msg: /tmp/metaindex_btc_tests-d2f4de.sh
clang++-21: note: diagnostic msg:
```

[metaindex_btc_tests-d2f4de.sh](https://github.com/user-attachments/files/22974383/metaindex_btc_tests-d2f4de.sh)
[metaindex_btc_tests-d2f4de.cpp](https://github.com/user-attachments/files/22974382/metaindex_btc_tests-d2f4de.cpp)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Oleg Babin (olegrok)

<details>
```
[2/3] Building CXX object src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o
FAILED: src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o 
ccache /usr/bin/ccache /usr/bin/clang++-21 -DBOOST_ALL_NO_LIB -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -DBOOST_NO_CXX98_FUNCTION_BASE -I/home/oleg/Projects/indexer/build/src -I/home/oleg/Projects/indexer/src -I/home/oleg/Projects/indexer/src/univalue/include -I/home/oleg/Projects/indexer/src/minisketch/include -I/home/oleg/Projects/indexer/src/secp256k1/include -O2 -g -std=c++23 -fPIE -fdebug-prefix-map=/home/oleg/Projects/indexer/src=. -fmacro-prefix-map=/home/oleg/Projects/indexer/src=. -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -Wstack-protector -fstack-protector-all -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wgnu -Wformat -Wformat-security -Wvla -Wshadow-field -Wthread-safety -Wthread-safety-pointer -Wloop-analysis -Wredundant-decls -Wunused-member-function -Wdate-time -Wconditional-uninitialized -Woverloaded-virtual -Wsuggest-override -Wimplicit-fallthrough -Wunreachable-code -Wdocumentation -Wself-assign -Wundef -Wno-unused-parameter -MD -MT src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o -MF src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o.d -o src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o -c /home/oleg/Projects/indexer/src/test/metaindex_btc_tests.cpp 
&lt;built-in&gt;:1:2: warning: null character ignored [-Wnull-character]
    1 | # 1 "&lt;built-in&gt;" 3
      |  ^
&lt;built-in&gt;:67:18: error: source file is not valid UTF-8
   67 | #define __cpp_digit_separators 201309L
      |                  ^
&lt;built-in&gt;:67:19: error: source file is not valid UTF-8
   67 | #define __cpp_digit_separators 201309L
      |                   ^
&lt;built-in&gt;:67:17: error: unknown type name 'P'
   67 | #define __cpp_digit_separators 201309L
      |                 ^
&lt;built-in&gt;:67:20: error: expected unqualified-id
   67 | #define __cpp_digit_separators 201309L
      |                    ^
&lt;built-in&gt;:67:27: error: source file is not valid UTF-8
   67 | #define __cpp_digit_separators 201309L
      |                           ^
&lt;built-in&gt;:68:3: error: source file is not valid UTF-8
   68 | #define __cpp_init_captures 201803L
      |   ^
&lt;built-in&gt;:68:4: error: source file is not valid UTF-8
   68 | #define __cpp_init_captures 201803L
      |    ^
&lt;built-in&gt;:69:22: error: source file is not valid UTF-8
   69 | #define __cpp_generic_lambdas 201707L
      |                      ^
&lt;built-in&gt;:69:23: error: source file is not valid UTF-8
   69 | #define __cpp_generic_lambdas 201707L
      |                       ^
&lt;built-in&gt;:69:24: error: source file is not valid UTF-8
   69 | #define __cpp_generic_lambdas 201707L
      |                        ^
&lt;built-in&gt;:69:32: error: source file is not valid UTF-8
   69 | #define __cpp_generic_lambdas 201707L
      |                                ^
&lt;built-in&gt;:70:9: error: source file is not valid UTF-8
   70 | #define __cpp_decltype_auto 201304L
      |         ^
&lt;built-in&gt;:70:10: error: source file is not valid UTF-8
   70 | #define __cpp_decltype_auto 201304L
      |          ^
&lt;built-in&gt;:71:28: error: source file is not valid UTF-8
   71 | #define __cpp_return_type_deduction 201304L
      |                            ^
&lt;built-in&gt;:71:29: error: source file is not valid UTF-8
   71 | #define __cpp_return_type_deduction 201304L
      |                             ^
&lt;built-in&gt;:71:30: error: source file is not valid UTF-8
   71 | #define __cpp_return_type_deduction 201304L
      |                              ^
&lt;built-in&gt;:71:38: error: source file is not valid UTF-8
   71 | #define __cpp_return_type_deduction 201304L
      |                                      ^
&lt;built-in&gt;:72:9: error: source file is not valid UTF-8
   72 | #define __cpp_aggregate_nsdmi 201304L
      |         ^
&lt;built-in&gt;:72:10: error: source file is not valid UTF-8
   72 | #define __cpp_aggregate_nsdmi 201304L
      |          ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++-21 -O2 -g -std=c++23 -fPIE -fdebug-prefix-map=/home/oleg/Projects/indexer/src=. -fmacro-prefix-map=/home/oleg/Projects/indexer/src=. -Wstack-protector -fstack-protector-all -fcf-protection=full -fstack-clash-protection -Wall -Wextra -Wgnu -Wformat -Wformat-security -Wvla -Wshadow-field -Wthread-safety -Wthread-safety-pointer -Wloop-analysis -Wredundant-decls -Wunused-member-function -Wdate-time -Wconditional-uninitialized -Woverloaded-virtual -Wsuggest-override -Wimplicit-fallthrough -Wunreachable-code -Wdocumentation -Wself-assign -Wundef -Wno-unused-parameter -DBOOST_ALL_NO_LIB -DBOOST_MULTI_INDEX_DISABLE_SERIALIZATION -DBOOST_NO_CXX98_FUNCTION_BASE -I/home/oleg/Projects/indexer/build/src -I/home/oleg/Projects/indexer/src -I/home/oleg/Projects/indexer/src/univalue/include -I/home/oleg/Projects/indexer/src/minisketch/include -I/home/oleg/Projects/indexer/src/secp256k1/include -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -c -MD -MT src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o -MF src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o.d -fcolor-diagnostics -o src/test/CMakeFiles/test_bitcoin.dir/metaindex_btc_tests.cpp.o /home/oleg/Projects/indexer/src/test/metaindex_btc_tests.cpp
1.	&lt;unknown&gt; parser at unknown location
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.21.0      0x000071592a3b88f6 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 54
1  libLLVM.so.21.0      0x000071592a3b61a3 llvm::sys::RunSignalHandlers() + 131
2  libLLVM.so.21.0      0x000071592a2f6680
3  libc.so.6            0x0000715929042520
4  libclang-cpp.so.21.0 0x000071593290ad05 clang::Lexer::SkipWhitespace(clang::Token&amp;, char const*, bool&amp;) + 37
5  libclang-cpp.so.21.0 0x000071593290e46c clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) + 1692
6  libclang-cpp.so.21.0 0x0000715932974ddd clang::Preprocessor::Lex(clang::Token&amp;) + 61
7  libclang-cpp.so.21.0 0x0000715932982c39
8  libclang-cpp.so.21.0 0x0000715932a3a6d1 clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) + 1041
9  libclang-cpp.so.21.0 0x0000715932a3a5be clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) + 766
10 libclang-cpp.so.21.0 0x0000715932a3a5be clang::Parser::SkipUntil(llvm::ArrayRef&lt;clang::tok::TokenKind&gt;, clang::Parser::SkipUntilFlags) + 766
11 libclang-cpp.so.21.0 0x0000715932992206 clang::Parser::SkipMalformedDecl() + 310
12 libclang-cpp.so.21.0 0x00007159329910a6 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 2502
13 libclang-cpp.so.21.0 0x0000715932a3e88f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 1055
14 libclang-cpp.so.21.0 0x0000715932a3e28e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 590
15 libclang-cpp.so.21.0 0x0000715932a3d591 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 2081
16 libclang-cpp.so.21.0 0x0000715932a3bdc7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1287
17 libclang-cpp.so.21.0 0x000071593297cb4e clang::ParseAST(clang::Sema&amp;, bool, bool) + 798
18 libclang-cpp.so.21.0 0x000071593459539b clang::FrontendAction::Execute() + 43
19 libclang-cpp.so.21.0 0x0000715934508cc4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 948
20 libclang-cpp.so.21.0 0x000071593461b49e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 702
21 clang++-21           0x00005b8c72b569ab cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 6459
22 clang++-21           0x00005b8c72b53595
23 libclang-cpp.so.21.0 0x00007159341d3079
24 libLLVM.so.21.0      0x000071592a2f6348 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 136
25 libclang-cpp.so.21.0 0x00007159341d29fd clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 365
26 libclang-cpp.so.21.0 0x0000715934195dae clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 494
27 libclang-cpp.so.21.0 0x0000715934195f6e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 142
28 libclang-cpp.so.21.0 0x00007159341b4bed clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 365
29 clang++-21           0x00005b8c72b5307a clang_main(int, char**, llvm::ToolContext const&amp;) + 6410
30 clang++-21           0x00005b8c72b60b26 main + 102
31 libc.so.6            0x0000715929029d90
32 libc.so.6            0x0000715929029e40 __libc_start_main + 128
33 clang++-21           0x00005b8c72b512e5 _start + 37
clang++-21: error: clang frontend command failed with exit code 135 (use -v to see invocation)
Ubuntu clang version 21.0.0 (++20250708042308+dcf485609c5c-1~exp1~20250708162410.2527)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang++-21: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-21: note: diagnostic msg: /tmp/metaindex_btc_tests-d2f4de.cpp
clang++-21: note: diagnostic msg: /tmp/metaindex_btc_tests-d2f4de.sh
clang++-21: note: diagnostic msg:
```

[metaindex_btc_tests-d2f4de.sh](https://github.com/user-attachments/files/22974383/metaindex_btc_tests-d2f4de.sh)
[metaindex_btc_tests-d2f4de.cpp](https://github.com/user-attachments/files/22974382/metaindex_btc_tests-d2f4de.cpp)
</details>


---

