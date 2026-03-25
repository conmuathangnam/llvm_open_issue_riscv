# [clang] Sporadic crashes in parser: "null character ignored"

**Author:** chaoticgd
**URL:** https://github.com/llvm/llvm-project/issues/128418
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2025-02-24T14:04:51Z

## Body

I've run into ~~two~~ three different instances of a sporadic crash in the clang frontend within the last couple days.

- ~~Both times it crashed on the last function in the translation unit.~~ The third time it didn't.
- It seems to dump out a lot of nonsensical `warning: null character ignored` warnings before crashing.
- I've tried reproducing it with the scripts that it dumped out, but no luck so far.

### Clang version

#### First and second crashes

```
clang-18:
  Installed: 1:18.1.8~++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144
  Candidate: 1:18.1.8~++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144
  Version table:
 *** 1:18.1.8~++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144 500
        500 http://apt.llvm.org/jammy llvm-toolchain-jammy-18/main amd64 Packages
        100 /var/lib/dpkg/status
```

#### Third crash

`LLVM-20.1.0-rc2-Linux-X64.tar.xz` from https://github.com/llvm/llvm-project/releases/tag/llvmorg-20.1.0-rc2

### OS

Kubuntu 22.04

### Code I was working on
https://github.com/PCSX2/pcsx2/pull/12241
https://github.com/chaoticgd/pcsx2/commit/80f461687ceb75ce0f6fb13d470cd3f08f1d398c (exact state of the code at the time of the second crash)

### Logs, scripts and preprocessed code

[clangcrash.zip](https://github.com/user-attachments/files/18930649/clangcrash.zip)
[clangcrash3.zip](https://github.com/user-attachments/files/18933354/clangcrash3.zip)

### Stack traces

#### First crash
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++-18 -DC4_NO_DEBUG_BREAK -DCPUINFO_SUPPORTED_PLATFORM=1 -DENABLE_OPENGL -DENABLE_VULKAN -DHAVE_DECL_BASENAME=1 -DHAVE_LIMITS_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DKDDW_FRONTEND_QT -DKDDW_FRONTEND_QTWIDGETS -DPCSX2_DEVBUILD -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG -DQT_NO_EXCEPTIONS -DQT_WIDGETS_LIB -DSOUNDTOUCH_FLOAT_SAMPLES -DST_NO_EXCEPTION_HANDLING -DWAYLAND_API -DX11_API -DXBYAK_NO_EXCEPTION -DZYCORE_STATIC_DEFINE -DZYDIS_STATIC_DEFINE -D_DEVEL -D_M_X86=1 -D__POSIX__ -I/home/thomas/projects/pcsx2/build/pcsx2-qt/pcsx2-qt_autogen/include -I/home/thomas/projects/pcsx2/deps/include/QtGui/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtGui/6.8.2/QtGui -I/home/thomas/projects/pcsx2/deps/include/QtCore/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtCore/6.8.2/QtCore -I/home/thomas/projects/pcsx2/deps/include/QtDBus/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtDBus/6.8.2/QtDBus -I/home/thomas/projects/pcsx2/build/common/include -I/home/thomas/projects/pcsx2/pcsx2 -I/home/thomas/projects/pcsx2/pcsx2-qt -I/home/thomas/projects/pcsx2/3rdparty/xbyak -I/home/thomas/projects/pcsx2/build/pcsx2 -I/home/thomas/projects/pcsx2/3rdparty/glad/include -I/home/thomas/projects/pcsx2/3rdparty/vulkan/include -I/home/thomas/projects/pcsx2/3rdparty/zydis/dependencies/zycore/include -I/home/thomas/projects/pcsx2/3rdparty/zydis/include -I/home/thomas/projects/pcsx2/common/../3rdparty/include -I/home/thomas/projects/pcsx2/common/.. -I/home/thomas/projects/pcsx2/3rdparty/fast_float/include -I/home/thomas/projects/pcsx2/3rdparty/imgui/include -I/home/thomas/projects/pcsx2/3rdparty/rapidyaml/include -I/home/thomas/projects/pcsx2/3rdparty/libchdr/include -I/home/thomas/projects/pcsx2/3rdparty/rcheevos/include -I/home/thomas/projects/pcsx2/3rdparty/discord-rpc/include -I/home/thomas/projects/pcsx2/3rdparty/rapidjson/include -I/home/thomas/projects/pcsx2/3rdparty/simpleini/include -I/home/thomas/projects/pcsx2/3rdparty/freesurround/include -I/home/thomas/projects/pcsx2/3rdparty/soundtouch/soundtouch -I/home/thomas/projects/pcsx2/3rdparty/lzma/include -I/home/thomas/projects/pcsx2/3rdparty/demangler/include -I/home/thomas/projects/pcsx2/3rdparty/ccc/src -I/home/thomas/projects/pcsx2/3rdparty/fmt/include -I/home/thomas/projects/pcsx2/3rdparty/libzip/lib -I/home/thomas/projects/pcsx2/build/3rdparty/libzip -I/home/thomas/projects/pcsx2/3rdparty/cpuinfo/include -I/home/thomas/projects/pcsx2/3rdparty/cubeb/include -isystem /home/thomas/projects/pcsx2/deps/include -isystem /usr/include/freetype2 -isystem /home/thomas/projects/pcsx2/deps/include/SDL2 -isystem /home/thomas/projects/pcsx2/deps/include/QtCore -isystem /home/thomas/projects/pcsx2/deps/mkspecs/linux-g++ -isystem /home/thomas/projects/pcsx2/deps/include/QtGui -isystem /home/thomas/projects/pcsx2/deps/include/QtWidgets -isystem /home/thomas/projects/pcsx2/deps/include/kddockwidgets-qt6 -DFMT_USE_EXCEPTIONS=0 -DFMT_USE_RTTI=0 -O2 -g -DNDEBUG -march=native -pipe -fvisibility=hidden -pthread -fno-exceptions -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers -ffp-contract=fast -fno-strict-aliasing -Wstrict-aliasing -Wno-parentheses -Wno-missing-braces -Wno-unknown-pragmas -fPIC -std=gnu++20 -Winvalid-pch -Xclang -include-pch -Xclang /home/thomas/projects/pcsx2/build/pcsx2-qt/CMakeFiles/pcsx2-qt.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/thomas/projects/pcsx2/build/pcsx2-qt/CMakeFiles/pcsx2-qt.dir/cmake_pch.hxx -MD -MT pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Memory/MemorySearchWidget.cpp.o -MF pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Memory/MemorySearchWidget.cpp.o.d -o pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Memory/MemorySearchWidget.cpp.o -c /home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Memory/MemorySearchWidget.cpp
1.	<unknown> parser at unknown location
2.	/home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Memory/MemorySearchWidget.cpp:729:1: parsing function body 'MemorySearchWidget::getValidSearchComparisonsForState'
3.	/home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Memory/MemorySearchWidget.cpp:729:1: in compound statement ('{}')
 #0 0x00007fbced8ed716 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94716)
 #1 0x00007fbced8eb6d0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd926d0)
 #2 0x00007fbced83d1f0 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xce41f0)
 #3 0x00007fbcec746520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007fbcf4cd36d5 clang::Lexer::SkipWhitespace(clang::Token&, char const*, bool&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb0e6d5)
 #5 0x00007fbcf4cd7b0e clang::Lexer::LexTokenInternal(clang::Token&, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb12b0e)
 #6 0x00007fbcf4d3d86d clang::Preprocessor::Lex(clang::Token&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb7886d)
 #7 0x00007fbcf4d5b12f (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9612f)
 #8 0x00007fbcf4d4e0f1 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb890f1)
 #9 0x00007fbcf4df4666 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc2f666)
#10 0x00007fbcf4df535f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc3035f)
#11 0x00007fbcf4e10346 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4b346)
#12 0x00007fbcf4d5aab8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb95ab8)
#13 0x00007fbcf4e0f3aa clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4a3aa)
#14 0x00007fbcf4e0ed73 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc49d73)
#15 0x00007fbcf4e0dfd6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc48fd6)
#16 0x00007fbcf4e0c40a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4740a)
#17 0x00007fbcf4d4506e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb8006e)
#18 0x00007fbcf6b75d25 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29b0d25)
#19 0x00007fbcf6aef2f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x292a2f4)
#20 0x00007fbcf6bf04ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a2b4ce)
#21 0x0000560cfe51fd55 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang++-18+0x12d55)
#22 0x0000560cfe51d155 (/usr/bin/clang++-18+0x10155)
#23 0x00007fbcf67a7659 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25e2659)
#24 0x00007fbced83cf8c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xce3f8c)
#25 0x00007fbcf67a6fee clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25e1fee)
#26 0x00007fbcf676f561 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25aa561)
#27 0x00007fbcf676f7ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25aa7ae)
#28 0x00007fbcf678bdbd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25c6dbd)
#29 0x0000560cfe51cac4 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang++-18+0xfac4)
#30 0x0000560cfe529bf6 main (/usr/bin/clang++-18+0x1cbf6)
#31 0x00007fbcec72dd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#32 0x00007fbcec72de40 call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007fbcec72de40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#34 0x0000560cfe519bc5 _start (/usr/bin/clang++-18+0xcbc5)
clang++-18: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++-18: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-18: note: diagnostic msg: /tmp/MemorySearchWidget-6c2701.cpp
clang++-18: note: diagnostic msg: /tmp/MemorySearchWidget-6c2701.sh
clang++-18: note: diagnostic msg: 

********************
[23/24] Building CXX object pcsx2-qt/CMakeFiles/pcsx2-qt.dir/pcsx2-qt_autogen/mocs_compilation.cpp.o
ninja: build stopped: subcommand failed.
```

#### Second crash
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++-18 -DC4_NO_DEBUG_BREAK -DCPUINFO_SUPPORTED_PLATFORM=1 -DENABLE_OPENGL -DENABLE_VULKAN -DHAVE_DECL_BASENAME=1 -DHAVE_LIMITS_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DKDDW_FRONTEND_QT -DKDDW_FRONTEND_QTWIDGETS -DPCSX2_DEVBUILD -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG -DQT_NO_EXCEPTIONS -DQT_WIDGETS_LIB -DSOUNDTOUCH_FLOAT_SAMPLES -DST_NO_EXCEPTION_HANDLING -DWAYLAND_API -DX11_API -DXBYAK_NO_EXCEPTION -DZYCORE_STATIC_DEFINE -DZYDIS_STATIC_DEFINE -D_DEVEL -D_M_X86=1 -D__POSIX__ -I/home/thomas/projects/pcsx2/build/pcsx2-qt/pcsx2-qt_autogen/include -I/home/thomas/projects/pcsx2/deps/include/QtGui/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtGui/6.8.2/QtGui -I/home/thomas/projects/pcsx2/deps/include/QtCore/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtCore/6.8.2/QtCore -I/home/thomas/projects/pcsx2/deps/include/QtDBus/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtDBus/6.8.2/QtDBus -I/home/thomas/projects/pcsx2/build/common/include -I/home/thomas/projects/pcsx2/pcsx2 -I/home/thomas/projects/pcsx2/pcsx2-qt -I/home/thomas/projects/pcsx2/3rdparty/xbyak -I/home/thomas/projects/pcsx2/build/pcsx2 -I/home/thomas/projects/pcsx2/3rdparty/glad/include -I/home/thomas/projects/pcsx2/3rdparty/vulkan/include -I/home/thomas/projects/pcsx2/3rdparty/zydis/dependencies/zycore/include -I/home/thomas/projects/pcsx2/3rdparty/zydis/include -I/home/thomas/projects/pcsx2/common/../3rdparty/include -I/home/thomas/projects/pcsx2/common/.. -I/home/thomas/projects/pcsx2/3rdparty/fast_float/include -I/home/thomas/projects/pcsx2/3rdparty/imgui/include -I/home/thomas/projects/pcsx2/3rdparty/rapidyaml/include -I/home/thomas/projects/pcsx2/3rdparty/libchdr/include -I/home/thomas/projects/pcsx2/3rdparty/rcheevos/include -I/home/thomas/projects/pcsx2/3rdparty/discord-rpc/include -I/home/thomas/projects/pcsx2/3rdparty/rapidjson/include -I/home/thomas/projects/pcsx2/3rdparty/simpleini/include -I/home/thomas/projects/pcsx2/3rdparty/freesurround/include -I/home/thomas/projects/pcsx2/3rdparty/soundtouch/soundtouch -I/home/thomas/projects/pcsx2/3rdparty/lzma/include -I/home/thomas/projects/pcsx2/3rdparty/demangler/include -I/home/thomas/projects/pcsx2/3rdparty/ccc/src -I/home/thomas/projects/pcsx2/3rdparty/fmt/include -I/home/thomas/projects/pcsx2/3rdparty/libzip/lib -I/home/thomas/projects/pcsx2/build/3rdparty/libzip -I/home/thomas/projects/pcsx2/3rdparty/cpuinfo/include -I/home/thomas/projects/pcsx2/3rdparty/cubeb/include -isystem /home/thomas/projects/pcsx2/deps/include -isystem /usr/include/freetype2 -isystem /home/thomas/projects/pcsx2/deps/include/SDL2 -isystem /home/thomas/projects/pcsx2/deps/include/QtCore -isystem /home/thomas/projects/pcsx2/deps/mkspecs/linux-g++ -isystem /home/thomas/projects/pcsx2/deps/include/QtGui -isystem /home/thomas/projects/pcsx2/deps/include/QtWidgets -isystem /home/thomas/projects/pcsx2/deps/include/kddockwidgets-qt6 -DFMT_USE_EXCEPTIONS=0 -DFMT_USE_RTTI=0 -O2 -g -DNDEBUG -march=native -pipe -fvisibility=hidden -pthread -fno-exceptions -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers -ffp-contract=fast -fno-strict-aliasing -Wstrict-aliasing -Wno-parentheses -Wno-missing-braces -Wno-unknown-pragmas -fPIC -std=gnu++20 -Winvalid-pch -Xclang -include-pch -Xclang /home/thomas/projects/pcsx2/build/pcsx2-qt/CMakeFiles/pcsx2-qt.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/thomas/projects/pcsx2/build/pcsx2-qt/CMakeFiles/pcsx2-qt.dir/cmake_pch.hxx -MD -MT pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Docking/DockLayout.cpp.o -MF pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Docking/DockLayout.cpp.o.d -o pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Docking/DockLayout.cpp.o -c /home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Docking/DockLayout.cpp
1.	<unknown> parser at unknown location
2.	/home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Docking/DockLayout.cpp:756:1: parsing function body 'DockLayout::generateNewUniqueName'
3.	/home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Docking/DockLayout.cpp:756:1: in compound statement ('{}')
 #0 0x00007f7ccc187716 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94716)
 #1 0x00007f7ccc1856d0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd926d0)
 #2 0x00007f7ccc0d71f0 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xce41f0)
 #3 0x00007f7ccafe0520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f7cd356d6d5 clang::Lexer::SkipWhitespace(clang::Token&, char const*, bool&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb0e6d5)
 #5 0x00007f7cd3571b0e clang::Lexer::LexTokenInternal(clang::Token&, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb12b0e)
 #6 0x00007f7cd35d786d clang::Preprocessor::Lex(clang::Token&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb7886d)
 #7 0x00007f7cd35f512f (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9612f)
 #8 0x00007f7cd35e80f1 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb890f1)
 #9 0x00007f7cd368e666 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc2f666)
#10 0x00007f7cd368f35f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc3035f)
#11 0x00007f7cd36aa346 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4b346)
#12 0x00007f7cd35f4ab8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb95ab8)
#13 0x00007f7cd36a93aa clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4a3aa)
#14 0x00007f7cd36a8d73 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc49d73)
#15 0x00007f7cd36a7fd6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc48fd6)
#16 0x00007f7cd36a640a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4740a)
#17 0x00007f7cd35df06e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb8006e)
#18 0x00007f7cd540fd25 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29b0d25)
#19 0x00007f7cd53892f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x292a2f4)
#20 0x00007f7cd548a4ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a2b4ce)
#21 0x000055e4b538bd55 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/bin/clang++-18+0x12d55)
#22 0x000055e4b5389155 (/usr/bin/clang++-18+0x10155)
#23 0x00007f7cd5041659 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25e2659)
#24 0x00007f7ccc0d6f8c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xce3f8c)
#25 0x00007f7cd5040fee clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25e1fee)
#26 0x00007f7cd5009561 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25aa561)
#27 0x00007f7cd50097ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25aa7ae)
#28 0x00007f7cd5025dbd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25c6dbd)
#29 0x000055e4b5388ac4 clang_main(int, char**, llvm::ToolContext const&) (/usr/bin/clang++-18+0xfac4)
#30 0x000055e4b5395bf6 main (/usr/bin/clang++-18+0x1cbf6)
#31 0x00007f7ccafc7d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#32 0x00007f7ccafc7e40 call_init ./csu/../csu/libc-start.c:128:20
#33 0x00007f7ccafc7e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#34 0x000055e4b5385bc5 _start (/usr/bin/clang++-18+0xcbc5)
clang++-18: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++-18: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-18: note: diagnostic msg: /tmp/DockLayout-99f52e.cpp
clang++-18: note: diagnostic msg: /tmp/DockLayout-99f52e.sh
clang++-18: note: diagnostic msg: 

********************
[33/36] Building CXX object pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Memory/MemorySearchWidget.cpp.o
ninja: build stopped: subcommand failed.
```

#### Third crash

See clangcrash3.zip.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: chaoticgd (chaoticgd)

<details>
I've run into two different instances of a sporadic crash in the clang frontend within the last couple days.

- Both times it crashed on the last function in the translation unit.
- It seems to dump out a lot of nonsensical `warning: null character ignored` warnings before crashing.
- I've tried reproducing it with the scripts that it dumped out, but no luck so far.

### Clang version
```
clang-18:
  Installed: 1:18.1.8~++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144
  Candidate: 1:18.1.8~++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144
  Version table:
 *** 1:18.1.8~++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144 500
        500 http://apt.llvm.org/jammy llvm-toolchain-jammy-18/main amd64 Packages
        100 /var/lib/dpkg/status
```

### OS

Kubuntu 22.04

### Code I was working on
https://github.com/PCSX2/pcsx2/pull/12241
https://github.com/chaoticgd/pcsx2/commit/80f461687ceb75ce0f6fb13d470cd3f08f1d398c (exact state of the code at the time of the second crash)

### Logs, scripts and preprocessed code

[clangcrash.zip](https://github.com/user-attachments/files/18930649/clangcrash.zip)

### Stack traces

#### First crash
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++-18 -DC4_NO_DEBUG_BREAK -DCPUINFO_SUPPORTED_PLATFORM=1 -DENABLE_OPENGL -DENABLE_VULKAN -DHAVE_DECL_BASENAME=1 -DHAVE_LIMITS_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DKDDW_FRONTEND_QT -DKDDW_FRONTEND_QTWIDGETS -DPCSX2_DEVBUILD -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG -DQT_NO_EXCEPTIONS -DQT_WIDGETS_LIB -DSOUNDTOUCH_FLOAT_SAMPLES -DST_NO_EXCEPTION_HANDLING -DWAYLAND_API -DX11_API -DXBYAK_NO_EXCEPTION -DZYCORE_STATIC_DEFINE -DZYDIS_STATIC_DEFINE -D_DEVEL -D_M_X86=1 -D__POSIX__ -I/home/thomas/projects/pcsx2/build/pcsx2-qt/pcsx2-qt_autogen/include -I/home/thomas/projects/pcsx2/deps/include/QtGui/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtGui/6.8.2/QtGui -I/home/thomas/projects/pcsx2/deps/include/QtCore/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtCore/6.8.2/QtCore -I/home/thomas/projects/pcsx2/deps/include/QtDBus/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtDBus/6.8.2/QtDBus -I/home/thomas/projects/pcsx2/build/common/include -I/home/thomas/projects/pcsx2/pcsx2 -I/home/thomas/projects/pcsx2/pcsx2-qt -I/home/thomas/projects/pcsx2/3rdparty/xbyak -I/home/thomas/projects/pcsx2/build/pcsx2 -I/home/thomas/projects/pcsx2/3rdparty/glad/include -I/home/thomas/projects/pcsx2/3rdparty/vulkan/include -I/home/thomas/projects/pcsx2/3rdparty/zydis/dependencies/zycore/include -I/home/thomas/projects/pcsx2/3rdparty/zydis/include -I/home/thomas/projects/pcsx2/common/../3rdparty/include -I/home/thomas/projects/pcsx2/common/.. -I/home/thomas/projects/pcsx2/3rdparty/fast_float/include -I/home/thomas/projects/pcsx2/3rdparty/imgui/include -I/home/thomas/projects/pcsx2/3rdparty/rapidyaml/include -I/home/thomas/projects/pcsx2/3rdparty/libchdr/include -I/home/thomas/projects/pcsx2/3rdparty/rcheevos/include -I/home/thomas/projects/pcsx2/3rdparty/discord-rpc/include -I/home/thomas/projects/pcsx2/3rdparty/rapidjson/include -I/home/thomas/projects/pcsx2/3rdparty/simpleini/include -I/home/thomas/projects/pcsx2/3rdparty/freesurround/include -I/home/thomas/projects/pcsx2/3rdparty/soundtouch/soundtouch -I/home/thomas/projects/pcsx2/3rdparty/lzma/include -I/home/thomas/projects/pcsx2/3rdparty/demangler/include -I/home/thomas/projects/pcsx2/3rdparty/ccc/src -I/home/thomas/projects/pcsx2/3rdparty/fmt/include -I/home/thomas/projects/pcsx2/3rdparty/libzip/lib -I/home/thomas/projects/pcsx2/build/3rdparty/libzip -I/home/thomas/projects/pcsx2/3rdparty/cpuinfo/include -I/home/thomas/projects/pcsx2/3rdparty/cubeb/include -isystem /home/thomas/projects/pcsx2/deps/include -isystem /usr/include/freetype2 -isystem /home/thomas/projects/pcsx2/deps/include/SDL2 -isystem /home/thomas/projects/pcsx2/deps/include/QtCore -isystem /home/thomas/projects/pcsx2/deps/mkspecs/linux-g++ -isystem /home/thomas/projects/pcsx2/deps/include/QtGui -isystem /home/thomas/projects/pcsx2/deps/include/QtWidgets -isystem /home/thomas/projects/pcsx2/deps/include/kddockwidgets-qt6 -DFMT_USE_EXCEPTIONS=0 -DFMT_USE_RTTI=0 -O2 -g -DNDEBUG -march=native -pipe -fvisibility=hidden -pthread -fno-exceptions -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers -ffp-contract=fast -fno-strict-aliasing -Wstrict-aliasing -Wno-parentheses -Wno-missing-braces -Wno-unknown-pragmas -fPIC -std=gnu++20 -Winvalid-pch -Xclang -include-pch -Xclang /home/thomas/projects/pcsx2/build/pcsx2-qt/CMakeFiles/pcsx2-qt.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/thomas/projects/pcsx2/build/pcsx2-qt/CMakeFiles/pcsx2-qt.dir/cmake_pch.hxx -MD -MT pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Memory/MemorySearchWidget.cpp.o -MF pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Memory/MemorySearchWidget.cpp.o.d -o pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Memory/MemorySearchWidget.cpp.o -c /home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Memory/MemorySearchWidget.cpp
1.	&lt;unknown&gt; parser at unknown location
2.	/home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Memory/MemorySearchWidget.cpp:729:1: parsing function body 'MemorySearchWidget::getValidSearchComparisonsForState'
3.	/home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Memory/MemorySearchWidget.cpp:729:1: in compound statement ('{}')
 #<!-- -->0 0x00007fbced8ed716 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94716)
 #<!-- -->1 0x00007fbced8eb6d0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd926d0)
 #<!-- -->2 0x00007fbced83d1f0 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xce41f0)
 #<!-- -->3 0x00007fbcec746520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007fbcf4cd36d5 clang::Lexer::SkipWhitespace(clang::Token&amp;, char const*, bool&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb0e6d5)
 #<!-- -->5 0x00007fbcf4cd7b0e clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb12b0e)
 #<!-- -->6 0x00007fbcf4d3d86d clang::Preprocessor::Lex(clang::Token&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb7886d)
 #<!-- -->7 0x00007fbcf4d5b12f (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9612f)
 #<!-- -->8 0x00007fbcf4d4e0f1 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb890f1)
 #<!-- -->9 0x00007fbcf4df4666 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc2f666)
#<!-- -->10 0x00007fbcf4df535f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc3035f)
#<!-- -->11 0x00007fbcf4e10346 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4b346)
#<!-- -->12 0x00007fbcf4d5aab8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb95ab8)
#<!-- -->13 0x00007fbcf4e0f3aa clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4a3aa)
#<!-- -->14 0x00007fbcf4e0ed73 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc49d73)
#<!-- -->15 0x00007fbcf4e0dfd6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc48fd6)
#<!-- -->16 0x00007fbcf4e0c40a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4740a)
#<!-- -->17 0x00007fbcf4d4506e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb8006e)
#<!-- -->18 0x00007fbcf6b75d25 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29b0d25)
#<!-- -->19 0x00007fbcf6aef2f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x292a2f4)
#<!-- -->20 0x00007fbcf6bf04ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a2b4ce)
#<!-- -->21 0x0000560cfe51fd55 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang++-18+0x12d55)
#<!-- -->22 0x0000560cfe51d155 (/usr/bin/clang++-18+0x10155)
#<!-- -->23 0x00007fbcf67a7659 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25e2659)
#<!-- -->24 0x00007fbced83cf8c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xce3f8c)
#<!-- -->25 0x00007fbcf67a6fee clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;*, bool*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25e1fee)
#<!-- -->26 0x00007fbcf676f561 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25aa561)
#<!-- -->27 0x00007fbcf676f7ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25aa7ae)
#<!-- -->28 0x00007fbcf678bdbd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25c6dbd)
#<!-- -->29 0x0000560cfe51cac4 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang++-18+0xfac4)
#<!-- -->30 0x0000560cfe529bf6 main (/usr/bin/clang++-18+0x1cbf6)
#<!-- -->31 0x00007fbcec72dd90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->32 0x00007fbcec72de40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->33 0x00007fbcec72de40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->34 0x0000560cfe519bc5 _start (/usr/bin/clang++-18+0xcbc5)
clang++-18: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++-18: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-18: note: diagnostic msg: /tmp/MemorySearchWidget-6c2701.cpp
clang++-18: note: diagnostic msg: /tmp/MemorySearchWidget-6c2701.sh
clang++-18: note: diagnostic msg: 

********************
[23/24] Building CXX object pcsx2-qt/CMakeFiles/pcsx2-qt.dir/pcsx2-qt_autogen/mocs_compilation.cpp.o
ninja: build stopped: subcommand failed.
```

#### Second crash
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /usr/bin/clang++-18 -DC4_NO_DEBUG_BREAK -DCPUINFO_SUPPORTED_PLATFORM=1 -DENABLE_OPENGL -DENABLE_VULKAN -DHAVE_DECL_BASENAME=1 -DHAVE_LIMITS_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DKDDW_FRONTEND_QT -DKDDW_FRONTEND_QTWIDGETS -DPCSX2_DEVBUILD -DQT_CORE_LIB -DQT_GUI_LIB -DQT_NO_DEBUG -DQT_NO_EXCEPTIONS -DQT_WIDGETS_LIB -DSOUNDTOUCH_FLOAT_SAMPLES -DST_NO_EXCEPTION_HANDLING -DWAYLAND_API -DX11_API -DXBYAK_NO_EXCEPTION -DZYCORE_STATIC_DEFINE -DZYDIS_STATIC_DEFINE -D_DEVEL -D_M_X86=1 -D__POSIX__ -I/home/thomas/projects/pcsx2/build/pcsx2-qt/pcsx2-qt_autogen/include -I/home/thomas/projects/pcsx2/deps/include/QtGui/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtGui/6.8.2/QtGui -I/home/thomas/projects/pcsx2/deps/include/QtCore/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtCore/6.8.2/QtCore -I/home/thomas/projects/pcsx2/deps/include/QtDBus/6.8.2 -I/home/thomas/projects/pcsx2/deps/include/QtDBus/6.8.2/QtDBus -I/home/thomas/projects/pcsx2/build/common/include -I/home/thomas/projects/pcsx2/pcsx2 -I/home/thomas/projects/pcsx2/pcsx2-qt -I/home/thomas/projects/pcsx2/3rdparty/xbyak -I/home/thomas/projects/pcsx2/build/pcsx2 -I/home/thomas/projects/pcsx2/3rdparty/glad/include -I/home/thomas/projects/pcsx2/3rdparty/vulkan/include -I/home/thomas/projects/pcsx2/3rdparty/zydis/dependencies/zycore/include -I/home/thomas/projects/pcsx2/3rdparty/zydis/include -I/home/thomas/projects/pcsx2/common/../3rdparty/include -I/home/thomas/projects/pcsx2/common/.. -I/home/thomas/projects/pcsx2/3rdparty/fast_float/include -I/home/thomas/projects/pcsx2/3rdparty/imgui/include -I/home/thomas/projects/pcsx2/3rdparty/rapidyaml/include -I/home/thomas/projects/pcsx2/3rdparty/libchdr/include -I/home/thomas/projects/pcsx2/3rdparty/rcheevos/include -I/home/thomas/projects/pcsx2/3rdparty/discord-rpc/include -I/home/thomas/projects/pcsx2/3rdparty/rapidjson/include -I/home/thomas/projects/pcsx2/3rdparty/simpleini/include -I/home/thomas/projects/pcsx2/3rdparty/freesurround/include -I/home/thomas/projects/pcsx2/3rdparty/soundtouch/soundtouch -I/home/thomas/projects/pcsx2/3rdparty/lzma/include -I/home/thomas/projects/pcsx2/3rdparty/demangler/include -I/home/thomas/projects/pcsx2/3rdparty/ccc/src -I/home/thomas/projects/pcsx2/3rdparty/fmt/include -I/home/thomas/projects/pcsx2/3rdparty/libzip/lib -I/home/thomas/projects/pcsx2/build/3rdparty/libzip -I/home/thomas/projects/pcsx2/3rdparty/cpuinfo/include -I/home/thomas/projects/pcsx2/3rdparty/cubeb/include -isystem /home/thomas/projects/pcsx2/deps/include -isystem /usr/include/freetype2 -isystem /home/thomas/projects/pcsx2/deps/include/SDL2 -isystem /home/thomas/projects/pcsx2/deps/include/QtCore -isystem /home/thomas/projects/pcsx2/deps/mkspecs/linux-g++ -isystem /home/thomas/projects/pcsx2/deps/include/QtGui -isystem /home/thomas/projects/pcsx2/deps/include/QtWidgets -isystem /home/thomas/projects/pcsx2/deps/include/kddockwidgets-qt6 -DFMT_USE_EXCEPTIONS=0 -DFMT_USE_RTTI=0 -O2 -g -DNDEBUG -march=native -pipe -fvisibility=hidden -pthread -fno-exceptions -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers -ffp-contract=fast -fno-strict-aliasing -Wstrict-aliasing -Wno-parentheses -Wno-missing-braces -Wno-unknown-pragmas -fPIC -std=gnu++20 -Winvalid-pch -Xclang -include-pch -Xclang /home/thomas/projects/pcsx2/build/pcsx2-qt/CMakeFiles/pcsx2-qt.dir/cmake_pch.hxx.pch -Xclang -include -Xclang /home/thomas/projects/pcsx2/build/pcsx2-qt/CMakeFiles/pcsx2-qt.dir/cmake_pch.hxx -MD -MT pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Docking/DockLayout.cpp.o -MF pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Docking/DockLayout.cpp.o.d -o pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Docking/DockLayout.cpp.o -c /home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Docking/DockLayout.cpp
1.	&lt;unknown&gt; parser at unknown location
2.	/home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Docking/DockLayout.cpp:756:1: parsing function body 'DockLayout::generateNewUniqueName'
3.	/home/thomas/projects/pcsx2/pcsx2-qt/Debugger/Docking/DockLayout.cpp:756:1: in compound statement ('{}')
 #<!-- -->0 0x00007f7ccc187716 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94716)
 #<!-- -->1 0x00007f7ccc1856d0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd926d0)
 #<!-- -->2 0x00007f7ccc0d71f0 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xce41f0)
 #<!-- -->3 0x00007f7ccafe0520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f7cd356d6d5 clang::Lexer::SkipWhitespace(clang::Token&amp;, char const*, bool&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb0e6d5)
 #<!-- -->5 0x00007f7cd3571b0e clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb12b0e)
 #<!-- -->6 0x00007f7cd35d786d clang::Preprocessor::Lex(clang::Token&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb7886d)
 #<!-- -->7 0x00007f7cd35f512f (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9612f)
 #<!-- -->8 0x00007f7cd35e80f1 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb890f1)
 #<!-- -->9 0x00007f7cd368e666 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc2f666)
#<!-- -->10 0x00007f7cd368f35f clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc3035f)
#<!-- -->11 0x00007f7cd36aa346 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4b346)
#<!-- -->12 0x00007f7cd35f4ab8 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb95ab8)
#<!-- -->13 0x00007f7cd36a93aa clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4a3aa)
#<!-- -->14 0x00007f7cd36a8d73 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc49d73)
#<!-- -->15 0x00007f7cd36a7fd6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc48fd6)
#<!-- -->16 0x00007f7cd36a640a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4740a)
#<!-- -->17 0x00007f7cd35df06e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb8006e)
#<!-- -->18 0x00007f7cd540fd25 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29b0d25)
#<!-- -->19 0x00007f7cd53892f4 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x292a2f4)
#<!-- -->20 0x00007f7cd548a4ce clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a2b4ce)
#<!-- -->21 0x000055e4b538bd55 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/bin/clang++-18+0x12d55)
#<!-- -->22 0x000055e4b5389155 (/usr/bin/clang++-18+0x10155)
#<!-- -->23 0x00007f7cd5041659 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25e2659)
#<!-- -->24 0x00007f7ccc0d6f8c llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xce3f8c)
#<!-- -->25 0x00007f7cd5040fee clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt; &gt;*, bool*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25e1fee)
#<!-- -->26 0x00007f7cd5009561 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25aa561)
#<!-- -->27 0x00007f7cd50097ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25aa7ae)
#<!-- -->28 0x00007f7cd5025dbd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt; &gt;&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x25c6dbd)
#<!-- -->29 0x000055e4b5388ac4 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/bin/clang++-18+0xfac4)
#<!-- -->30 0x000055e4b5395bf6 main (/usr/bin/clang++-18+0x1cbf6)
#<!-- -->31 0x00007f7ccafc7d90 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#<!-- -->32 0x00007f7ccafc7e40 call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->33 0x00007f7ccafc7e40 __libc_start_main ./csu/../csu/libc-start.c:379:5
#<!-- -->34 0x000055e4b5385bc5 _start (/usr/bin/clang++-18+0xcbc5)
clang++-18: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Ubuntu clang version 18.1.8 (++20240731024944+3b5b5c1ec4a3-1~exp1~20240731145000.144)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++-18: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-18: note: diagnostic msg: /tmp/DockLayout-99f52e.cpp
clang++-18: note: diagnostic msg: /tmp/DockLayout-99f52e.sh
clang++-18: note: diagnostic msg: 

********************
[33/36] Building CXX object pcsx2-qt/CMakeFiles/pcsx2-qt.dir/Debugger/Memory/MemorySearchWidget.cpp.o
ninja: build stopped: subcommand failed.
```
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 20 release candidate or `main` branch?

---

### Comment 3 - chaoticgd

I've got the release candidate setup. I'll have to report back if the problem reoccurs within the next week or so.

---

### Comment 4 - chaoticgd

I've had a crash on the RC build.

---

### Comment 5 - shafik

Possible related issues: https://github.com/llvm/llvm-project/issues/90993 and https://github.com/llvm/llvm-project/issues/64576

---

### Comment 6 - chaoticgd

It looks like it's a duplicate of #12110/#64576 then. It's just crashing when a source file is modified mid-compilation. As it's already tracked I'll just close this one.

---

