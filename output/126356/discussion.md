# Internal compiler error in clang::Lexer::LexTokenInternal

**Author:** moxian
**URL:** https://github.com/llvm/llvm-project/issues/126356

## Body

```
$ ninja -C build -k 0
ninja: Entering directory `build'
[1/570] Building CXX object src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o
FAILED: src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o
/usr/bin/c++ -DBACKTRACE -DCMAKE -DGIT_VERSION -DLOCALIZE -DSDL_SOUND -DTILES -DUSE_HOME_DIR -I/home/username/github/Cataclysm-DDA/src -isystem /home/username/github/Cataclysm-DDA/src/third-party -isystem /usr/include/SDL2 -Werror -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wsuggest-override -Wunused-macros -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Wno-unknown-warning-option -Wredundant-decls  -fsigned-char -g1 -Og -g2 -std=c++17 -pthread -MD -MT src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o -MF src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o.d -o src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o -c /home/username/github/Cataclysm-DDA/src/advanced_inv_area.cpp
In file included from /home/username/github/Cataclysm-DDA/src/advanced_inv_area.cpp:29:
/home/username/github/Cataclysm-DDA/src/vehicle.h:1152:9: error: unknown type name 'vehicle_part_with_fakes_range'
 1152 |         vehicle_part_with_fakes_range get_all_parts_with_fakes( bool with_inactive = false ) const;
      |         ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/bin/c++ -DBACKTRACE -DCMAKE -DGIT_VERSION -DLOCALIZE -DSDL_SOUND -DTILES -DUSE_HOME_DIR -I/home/username/github/Cataclysm-DDA/src -isystem /home/username/github/Cataclysm-DDA/src/third-party -isystem /usr/include/SDL2 -Werror -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wsuggest-override -Wunused-macros -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Wno-unknown-warning-option -Wredundant-decls -fsigned-char -g1 -Og -g2 -std=c++17 -pthread -MD -MT src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o -MF src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o.d -o src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o -c /home/username/github/Cataclysm-DDA/src/advanced_inv_area.cpp
1.      <unknown> parser at unknown location
2.      /home/username/github/Cataclysm-DDA/src/vehicle.h:809:1: parsing struct/union/class body 'vehicle'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.18.1      0x00007f93e23fc3bf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 63
1  libLLVM.so.18.1      0x00007f93e23fa4f9 llvm::sys::RunSignalHandlers() + 89
2  libLLVM.so.18.1      0x00007f93e2346227
3  libc.so.6            0x00007f93e1202330
4  libclang-cpp.so.18.1 0x00007f93e9786c32 clang::Lexer::LexTokenInternal(clang::Token&, bool) + 114
5  libclang-cpp.so.18.1 0x00007f93e97f05ed clang::Preprocessor::Lex(clang::Token&) + 45
6  libclang-cpp.so.18.1 0x00007f93e97afafc clang::Preprocessor::CachingLex(clang::Token&) + 188
7  libclang-cpp.so.18.1 0x00007f93e97afe7d
8  libclang-cpp.so.18.1 0x00007f93e97f05ed clang::Preprocessor::Lex(clang::Token&) + 45
9  libclang-cpp.so.18.1 0x00007f93e97fe29d
10 libclang-cpp.so.18.1 0x00007f93e98bd652 clang::Parser::ExpectAndConsume(clang::tok::TokenKind, unsigned int, llvm::StringRef) + 1666
11 libclang-cpp.so.18.1 0x00007f93e98372a5 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject*) + 7429
12 libclang-cpp.so.18.1 0x00007f93e98385a8 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) + 888
13 libclang-cpp.so.18.1 0x00007f93e9832cb2 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) + 3714
14 libclang-cpp.so.18.1 0x00007f93e9830284 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) + 6772
15 libclang-cpp.so.18.1 0x00007f93e9811e1d clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) + 4317
16 libclang-cpp.so.18.1 0x00007f93e98c3d0d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 205
17 libclang-cpp.so.18.1 0x00007f93e98c3a39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 489
18 libclang-cpp.so.18.1 0x00007f93e98c2c65 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 2197
19 libclang-cpp.so.18.1 0x00007f93e98c0f7b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 1499
20 libclang-cpp.so.18.1 0x00007f93e97f847e clang::ParseAST(clang::Sema&, bool, bool) + 766
21 libclang-cpp.so.18.1 0x00007f93eb66762c clang::FrontendAction::Execute() + 92
22 libclang-cpp.so.18.1 0x00007f93eb5e40b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 708
23 libclang-cpp.so.18.1 0x00007f93eb6e363d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 765
24 c++                  0x000055fc462ee42e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 3694
25 c++                  0x000055fc462eb894
26 libclang-cpp.so.18.1 0x00007f93eb294972
27 libLLVM.so.18.1      0x00007f93e2345f77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 151
28 libclang-cpp.so.18.1 0x00007f93eb294237 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 407
29 libclang-cpp.so.18.1 0x00007f93eb25c518 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 888
30 libclang-cpp.so.18.1 0x00007f93eb25c77f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 159
31 libclang-cpp.so.18.1 0x00007f93eb278c20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 352
32 c++                  0x000055fc462eb1ec clang_main(int, char**, llvm::ToolContext const&) + 11180
33 c++                  0x000055fc462f8383 main + 131
34 libc.so.6            0x00007f93e11e71ca
35 libc.so.6            0x00007f93e11e728b __libc_start_main + 139
36 c++                  0x000055fc462e8255 _start + 37
c++: error: clang frontend command failed with exit code 135 (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
c++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
c++: note: diagnostic msg: /tmp/advanced_inv_area-a55444.cpp
c++: note: diagnostic msg: /tmp/advanced_inv_area-a55444.sh
c++: note: diagnostic msg:

********************
[2/570] Building CXX object src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o
FAILED: src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o
/usr/bin/c++ -DBACKTRACE -DCMAKE -DGIT_VERSION -DLOCALIZE -DSDL_SOUND -DTILES -DUSE_HOME_DIR -I/home/username/github/Cataclysm-DDA/src -isystem /home/username/github/Cataclysm-DDA/src/third-party -isystem /usr/include/SDL2 -Werror -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wsuggest-override -Wunused-macros -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Wno-unknown-warning-option -Wredundant-decls  -fsigned-char -g1 -Og -g2 -std=c++17 -pthread -MD -MT src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o -MF src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o.d -o src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o -c /home/username/github/Cataclysm-DDA/src/action.cpp
In file included from /home/username/github/Cataclysm-DDA/src/action.cpp:42:
/home/username/github/Cataclysm-DDA/src/vehicle.h:1152:9: error: unknown type name 'vehicle_part_with_fakes_range'
 1152 |         vehicle_part_with_fakes_range get_all_parts_with_fakes( bool with_inactive = false ) const;
      |         ^
1 error generated.
[2/570] Building CXX object src/CMakeFiles/cataclysm-tiles-common.dir/ammo.cpp.o^C
ninja: build stopped: interrupted by user.
```

/tmp/advanced_inv_area-a55444.cpp and /tmp/advanced_inv_area-a55444.sh: [advanced_inv_area-a55444.zip](https://github.com/user-attachments/files/18717496/advanced_inv_area-a55444.zip)

This is inside fresh-ish WSL Ubuntu 24.04.1 LTS with clang++-18

Full repo state at the time of the crash: https://github.com/moxian/Cataclysm-DDA/tree/clang-crash
The crash did not reoccur when I stopped the build and tried anew, so I'm not sure I can provide any extra info


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (moxian)

<details>
```
$ ninja -C build -k 0
ninja: Entering directory `build'
[1/570] Building CXX object src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o
FAILED: src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o
/usr/bin/c++ -DBACKTRACE -DCMAKE -DGIT_VERSION -DLOCALIZE -DSDL_SOUND -DTILES -DUSE_HOME_DIR -I/home/username/github/Cataclysm-DDA/src -isystem /home/username/github/Cataclysm-DDA/src/third-party -isystem /usr/include/SDL2 -Werror -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wsuggest-override -Wunused-macros -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Wno-unknown-warning-option -Wredundant-decls  -fsigned-char -g1 -Og -g2 -std=c++17 -pthread -MD -MT src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o -MF src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o.d -o src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o -c /home/username/github/Cataclysm-DDA/src/advanced_inv_area.cpp
In file included from /home/username/github/Cataclysm-DDA/src/advanced_inv_area.cpp:29:
/home/username/github/Cataclysm-DDA/src/vehicle.h:1152:9: error: unknown type name 'vehicle_part_with_fakes_range'
 1152 |         vehicle_part_with_fakes_range get_all_parts_with_fakes( bool with_inactive = false ) const;
      |         ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/bin/c++ -DBACKTRACE -DCMAKE -DGIT_VERSION -DLOCALIZE -DSDL_SOUND -DTILES -DUSE_HOME_DIR -I/home/username/github/Cataclysm-DDA/src -isystem /home/username/github/Cataclysm-DDA/src/third-party -isystem /usr/include/SDL2 -Werror -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wsuggest-override -Wunused-macros -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Wno-unknown-warning-option -Wredundant-decls -fsigned-char -g1 -Og -g2 -std=c++17 -pthread -MD -MT src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o -MF src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o.d -o src/CMakeFiles/cataclysm-tiles-common.dir/advanced_inv_area.cpp.o -c /home/username/github/Cataclysm-DDA/src/advanced_inv_area.cpp
1.      &lt;unknown&gt; parser at unknown location
2.      /home/username/github/Cataclysm-DDA/src/vehicle.h:809:1: parsing struct/union/class body 'vehicle'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.18.1      0x00007f93e23fc3bf llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 63
1  libLLVM.so.18.1      0x00007f93e23fa4f9 llvm::sys::RunSignalHandlers() + 89
2  libLLVM.so.18.1      0x00007f93e2346227
3  libc.so.6            0x00007f93e1202330
4  libclang-cpp.so.18.1 0x00007f93e9786c32 clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) + 114
5  libclang-cpp.so.18.1 0x00007f93e97f05ed clang::Preprocessor::Lex(clang::Token&amp;) + 45
6  libclang-cpp.so.18.1 0x00007f93e97afafc clang::Preprocessor::CachingLex(clang::Token&amp;) + 188
7  libclang-cpp.so.18.1 0x00007f93e97afe7d
8  libclang-cpp.so.18.1 0x00007f93e97f05ed clang::Preprocessor::Lex(clang::Token&amp;) + 45
9  libclang-cpp.so.18.1 0x00007f93e97fe29d
10 libclang-cpp.so.18.1 0x00007f93e98bd652 clang::Parser::ExpectAndConsume(clang::tok::TokenKind, unsigned int, llvm::StringRef) + 1666
11 libclang-cpp.so.18.1 0x00007f93e98372a5 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::ParsingDeclRAIIObject*) + 7429
12 libclang-cpp.so.18.1 0x00007f93e98385a8 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) + 888
13 libclang-cpp.so.18.1 0x00007f93e9832cb2 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) + 3714
14 libclang-cpp.so.18.1 0x00007f93e9830284 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) + 6772
15 libclang-cpp.so.18.1 0x00007f93e9811e1d clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) + 4317
16 libclang-cpp.so.18.1 0x00007f93e98c3d0d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 205
17 libclang-cpp.so.18.1 0x00007f93e98c3a39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 489
18 libclang-cpp.so.18.1 0x00007f93e98c2c65 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 2197
19 libclang-cpp.so.18.1 0x00007f93e98c0f7b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1499
20 libclang-cpp.so.18.1 0x00007f93e97f847e clang::ParseAST(clang::Sema&amp;, bool, bool) + 766
21 libclang-cpp.so.18.1 0x00007f93eb66762c clang::FrontendAction::Execute() + 92
22 libclang-cpp.so.18.1 0x00007f93eb5e40b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 708
23 libclang-cpp.so.18.1 0x00007f93eb6e363d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 765
24 c++                  0x000055fc462ee42e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 3694
25 c++                  0x000055fc462eb894
26 libclang-cpp.so.18.1 0x00007f93eb294972
27 libLLVM.so.18.1      0x00007f93e2345f77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 151
28 libclang-cpp.so.18.1 0x00007f93eb294237 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 407
29 libclang-cpp.so.18.1 0x00007f93eb25c518 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 888
30 libclang-cpp.so.18.1 0x00007f93eb25c77f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 159
31 libclang-cpp.so.18.1 0x00007f93eb278c20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 352
32 c++                  0x000055fc462eb1ec clang_main(int, char**, llvm::ToolContext const&amp;) + 11180
33 c++                  0x000055fc462f8383 main + 131
34 libc.so.6            0x00007f93e11e71ca
35 libc.so.6            0x00007f93e11e728b __libc_start_main + 139
36 c++                  0x000055fc462e8255 _start + 37
c++: error: clang frontend command failed with exit code 135 (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
c++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
c++: note: diagnostic msg: /tmp/advanced_inv_area-a55444.cpp
c++: note: diagnostic msg: /tmp/advanced_inv_area-a55444.sh
c++: note: diagnostic msg:

********************
[2/570] Building CXX object src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o
FAILED: src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o
/usr/bin/c++ -DBACKTRACE -DCMAKE -DGIT_VERSION -DLOCALIZE -DSDL_SOUND -DTILES -DUSE_HOME_DIR -I/home/username/github/Cataclysm-DDA/src -isystem /home/username/github/Cataclysm-DDA/src/third-party -isystem /usr/include/SDL2 -Werror -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wsuggest-override -Wunused-macros -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Wno-unknown-warning-option -Wredundant-decls  -fsigned-char -g1 -Og -g2 -std=c++17 -pthread -MD -MT src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o -MF src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o.d -o src/CMakeFiles/cataclysm-tiles-common.dir/action.cpp.o -c /home/username/github/Cataclysm-DDA/src/action.cpp
In file included from /home/username/github/Cataclysm-DDA/src/action.cpp:42:
/home/username/github/Cataclysm-DDA/src/vehicle.h:1152:9: error: unknown type name 'vehicle_part_with_fakes_range'
 1152 |         vehicle_part_with_fakes_range get_all_parts_with_fakes( bool with_inactive = false ) const;
      |         ^
1 error generated.
[2/570] Building CXX object src/CMakeFiles/cataclysm-tiles-common.dir/ammo.cpp.o^C
ninja: build stopped: interrupted by user.
```

/tmp/advanced_inv_area-a55444.cpp and /tmp/advanced_inv_area-a55444.sh: [advanced_inv_area-a55444.zip](https://github.com/user-attachments/files/18717496/advanced_inv_area-a55444.zip)

This is inside fresh-ish WSL Ubuntu 24.04.1 LTS with clang++-18

Full repo state at the time of the crash: https://github.com/moxian/Cataclysm-DDA/tree/clang-crash
The crash did not reoccur when I stopped the build and tried anew, so I'm not sure I can provide any extra info

</details>


---

### Comment 2 - EugeneZelenko

Could you please try 19, 20 or `main` branch?

---

### Comment 3 - moxian

As I mentioned, the error didn't reoccur even with the same clang-18 when I tried reproducing it, so I don't expect other versions to crash either.

The non-reproducibility does make me suspect that this is not a real bug but perhaps some weird  localized bit flip that randomly happened in my system, but the error message asked me to file an issue so I'm filling an issue.

Edit:
Tried once again just in case. Did not repro in either 17, 18 nor 19

---

### Comment 4 - moxian

I should probably note that the code lives in WSL but I was working on it in Visual Studio Code in Windows, and there is a slight chance that a file that was being compiled changed during compilation, and in perhaps somewhat unorthodox ways due to Windows<->WSL filesystem syncing not being a fully "native" thing.
But I considered this possibility at the time, and I was reasonably confident back then that I was not actually chaning any files in the editor since the compilation started, only just browsing them.

---

### Comment 5 - moxian

Hello, i've encountered this again, and this time I definitely _was_ saving  the source file right as it was being compiled.

<details>
<summary>terminal output / stack trace</summary>

```
$ ninja -C build
ninja: Entering directory `build'
[67/598] Building CXX object src/CMakeFiles/cataclysm-tiles-common.dir/condition.cpp.o
FAILED: src/CMakeFiles/cataclysm-tiles-common.dir/condition.cpp.o
/usr/bin/c++ -DBACKTRACE -DCMAKE -DGIT_VERSION -DLOCALIZE -DSDL_SOUND -DTILES -DUSE_HOME_DIR -I/home/username/github/Cataclysm-DDA/src -isystem /home/username/github/Cataclysm-DDA/src/third-party -isystem /usr/include/SDL2 -Werror -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wsuggest-override -Wunused-macros -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Wno-unknown-warning-option -Wredundant-decls  -fsigned-char -g1 -Og -g2 -std=c++17 -pthread -MD -MT src/CMakeFiles/cataclysm-tiles-common.dir/condition.cpp.o -MF src/CMakeFiles/cataclysm-tiles-common.dir/condition.cpp.o.d -o src/CMakeFiles/cataclysm-tiles-common.dir/condition.cpp.o -c /home/username/github/Cataclysm-DDA/src/condition.cpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /usr/bin/c++ -DBACKTRACE -DCMAKE -DGIT_VERSION -DLOCALIZE -DSDL_SOUND -DTILES -DUSE_HOME_DIR -I/home/username/github/Cataclysm-DDA/src -isystem /home/username/github/Cataclysm-DDA/src/third-party -isystem /usr/include/SDL2 -Werror -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wpedantic -Wsuggest-override -Wunused-macros -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Wno-unknown-warning-option -Wredundant-decls -fsigned-char -g1 -Og -g2 -std=c++17 -pthread -MD -MT src/CMakeFiles/cataclysm-tiles-common.dir/condition.cpp.o -MF src/CMakeFiles/cataclysm-tiles-common.dir/condition.cpp.o.d -o src/CMakeFiles/cataclysm-tiles-common.dir/condition.cpp.o -c /home/username/github/Cataclysm-DDA/src/condition.cpp
1.      <unknown> parser at unknown location
2.      /home/username/github/Cataclysm-DDA/src/condition.cpp:2827:1: parsing function body 'dialogue_data::complex_conds'
3.      /home/username/github/Cataclysm-DDA/src/condition.cpp:2827:1: in compound statement ('{}')
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.18.1      0x00007fd49722f3bf llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 63
1  libLLVM.so.18.1      0x00007fd49722d4f9 llvm::sys::RunSignalHandlers() + 89
2  libLLVM.so.18.1      0x00007fd497179227
3  libc.so.6            0x00007fd496035330
4  libclang-cpp.so.18.1 0x00007fd49e5b5d3d clang::Lexer::SkipWhitespace(clang::Token&, char const*, bool&) + 45
5  libclang-cpp.so.18.1 0x00007fd49e5ba2f7 clang::Lexer::LexTokenInternal(clang::Token&, bool) + 1847
6  libclang-cpp.so.18.1 0x00007fd49e6235ed clang::Preprocessor::Lex(clang::Token&) + 45
7  libclang-cpp.so.18.1 0x00007fd49e641938
8  libclang-cpp.so.18.1 0x00007fd49e634938
9  libclang-cpp.so.18.1 0x00007fd49e6dbe6e clang::Parser::ParseCompoundStatementBody(bool) + 2590
10 libclang-cpp.so.18.1 0x00007fd49e6dcb77 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) + 183
11 libclang-cpp.so.18.1 0x00007fd49e6f80c7 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) + 3751
12 libclang-cpp.so.18.1 0x00007fd49e6412a1 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 4689
13 libclang-cpp.so.18.1 0x00007fd49e6f707f clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 1087
14 libclang-cpp.so.18.1 0x00007fd49e6f6a39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 489
15 libclang-cpp.so.18.1 0x00007fd49e6f5c65 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 2197
16 libclang-cpp.so.18.1 0x00007fd49e6f3f7b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 1499
17 libclang-cpp.so.18.1 0x00007fd49e62b47e clang::ParseAST(clang::Sema&, bool, bool) + 766
18 libclang-cpp.so.18.1 0x00007fd4a049a62c clang::FrontendAction::Execute() + 92
19 libclang-cpp.so.18.1 0x00007fd4a04170b4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 708
20 libclang-cpp.so.18.1 0x00007fd4a051663d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 765
21 c++                  0x000055fe5956c42e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 3694
22 c++                  0x000055fe59569894
23 libclang-cpp.so.18.1 0x00007fd4a00c7972
24 libLLVM.so.18.1      0x00007fd497178f77 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 151
25 libclang-cpp.so.18.1 0x00007fd4a00c7237 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 407
26 libclang-cpp.so.18.1 0x00007fd4a008f518 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 888
27 libclang-cpp.so.18.1 0x00007fd4a008f77f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 159
28 libclang-cpp.so.18.1 0x00007fd4a00abc20 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 352
29 c++                  0x000055fe595691ec clang_main(int, char**, llvm::ToolContext const&) + 11180
30 c++                  0x000055fe59576383 main + 131
31 libc.so.6            0x00007fd49601a1ca
32 libc.so.6            0x00007fd49601a28b __libc_start_main + 139
33 c++                  0x000055fe59566255 _start + 37
c++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.3 (1ubuntu1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
c++: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
c++: note: diagnostic msg: /tmp/condition-f963a4.cpp
c++: note: diagnostic msg: /tmp/condition-f963a4.sh
c++: note: diagnostic msg:

********************
[72/598] Building CXX object src/CMakeFiles/cataclysm-tiles-common.dir/consumption.cpp.o
ninja: build stopped: subcommand failed.
```

[condition-f963a4.zip](https://github.com/user-attachments/files/18726982/condition-f963a4.zip)


</details>


I'm not exactly sure how VSCode/WSL save file interaction works, but given the symptoms (crash in lexer, unability to easily reproduce, being caught during the save operation) it probably has something to do with this.

---

### Comment 6 - zyn0217

This is a known issue that was first reported in https://github.com/llvm/llvm-project/issues/12110

---

