# clang crash, possibly related to templating of std::variant, isHorizontalWhitespace LexTokenInternal Preprocessor::Lex

**Author:** sparr
**URL:** https://github.com/llvm/llvm-project/issues/153800

## Body

I had not modified the body of `JsonObject::get_member_opt` when I encountered this crash. The most recent change prior to this crash was implementing `template< typename T1, typename T2 > bool JsonValue::read( std::variant<T1, T2> &v, bool throw_on_error ) const`, although it is possible I made some other change since the previous successful compile.

```
Stack dump:
0.      Program arguments: /usr/lib/llvm18/bin/clang++ -Og -Xclang -fno-pch-timestamp -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wpedantic -Wunused-macros -Wredundant-decls -g -fsigned-char -pthread -pthread -std=c++17 -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsuggest-override -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Winvalid-pch -Werror -Isrc -isystem src/third-party -DZSTD_STATIC_LINKING_ONLY -DZSTD_DISABLE_ASM -DGIT_VERSION -DTILES -I/usr/include/SDL2 -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/SDL2 -DHWY_SHARED_DEFINE -DAVIF_DLL -I/usr/include/rav1e -I/usr/include/svt-av1 -DEB_DLL -I/usr/include/webp -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -I/usr/include/SDL2 -D_GNU_SOURCE=1 -D_REENTRANT -DSDL_SOUND -include-pch pch/main-pch.hpp.pch -fpch-preprocess -c -MMD -MP -fcolor-diagnostics -o obj/tiles/catacharset.o src/catacharset.cpp
1.      <unknown> parser at unknown location
2.      src/flexbuffer_json-inl.h:1061:1: parsing function body 'JsonObject::get_member_opt'
3.      src/flexbuffer_json-inl.h:1061:1: in compound statement ('{}')
 #0 0x00007fca349ca540 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/Unix/Signals.inc:723:22
 #1 0x00007fca349c79f5 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/Signals.cpp:105:20
 #2 0x00007fca349c79f5 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/Unix/Signals.inc:367:31
 #3 0x00007fca348ab2d9 HandleCrash /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #4 0x00007fca348ab2d9 CrashRecoverySignalHandler /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #5 0x00007fca33c4def0 (/usr/lib/libc.so.6+0x3def0)
 #6 0x00007fca3c92a14d clang::isHorizontalWhitespace(unsigned char) /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Basic/CharInfo.h:78:22
 #7 0x00007fca3c92a14d clang::Lexer::LexTokenInternal(clang::Token&, bool) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/Lexer.cpp:3668:29
 #8 0x00007fca3c9556cc clang::Preprocessor::Lex(clang::Token&) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/Preprocessor.cpp:869:10
 #9 0x00007fca3c935ff8 clang::Preprocessor::CachingLex(clang::Token&) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/PPCaching.cpp:65:3
#10 0x00007fca3c935ff8 clang::Preprocessor::CachingLex(clang::Token&) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/PPCaching.cpp:48:6
#11 0x00007fca3c935ff8 clang::Preprocessor::CLK_CachingLexer(clang::Preprocessor&, clang::Token&) /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Lex/Preprocessor.h:2920:17
#12 0x00007fca3c9556cc clang::Preprocessor::Lex(clang::Token&) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/Preprocessor.cpp:869:10
#13 0x00007fca3ca5dfd3 clang::Parser::TryConsumeToken(clang::tok::TokenKind) /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Parse/Parser.h:528:11
#14 0x00007fca3ca5dfd3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:519:23
#15 0x00007fca3ca5e2b8 llvm::SmallVectorBase<unsigned int>::empty() const /usr/lib/llvm18/include/llvm/ADT/SmallVector.h:94:46
#16 0x00007fca3ca5e2b8 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#17 0x00007fca3ca5e2b8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#18 0x00007fca3ca63b6a clang::Parser::ParseCompoundStatementBody(bool) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1236:38
#19 0x00007fca3ca65b34 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:2517:3
#20 0x00007fca3ca66c9f clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/Parser.cpp:1516:36
#21 0x00007fca3c9c4570 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseDecl.cpp:2272:50
#22 0x00007fca3ca50793 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/Parser.cpp:1266:42
#23 0x00007fca3ca69532 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/Parser.cpp:1070:50
#24 0x00007fca3ca6a14b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/Parser.cpp:760:36
#25 0x00007fca3c98cdf8 clang::ParseAST(clang::Sema&, bool, bool) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseAST.cpp:162:69
#26 0x00007fca3e463971 clang::FrontendAction::Execute() /usr/src/debug/clang18/clang-18.1.8.src/lib/Frontend/FrontendAction.cpp:1069:21
#27 0x00007fca3e404617 llvm::Error::getPtr() const /usr/lib/llvm18/include/llvm/Support/Error.h:279:0
#28 0x00007fca3e404617 llvm::Error::operator bool() /usr/lib/llvm18/include/llvm/Support/Error.h:239:0
#29 0x00007fca3e404617 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/debug/clang18/clang-18.1.8.src/lib/Frontend/CompilerInstance.cpp:1057:0
#30 0x00007fca3e4bc0fd std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#31 0x00007fca3e4bc0fd std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#32 0x00007fca3e4bc0fd std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#33 0x00007fca3e4bc0fd clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Frontend/CompilerInvocation.h:254:0
#34 0x00007fca3e4bc0fd clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Frontend/CompilerInstance.h:291:0
#35 0x00007fca3e4bc0fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang18/clang-18.1.8.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:273:0
#36 0x00005645bca18384 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/src/debug/clang18/clang-18.1.8.src/tools/driver/cc1_main.cpp:294:40
#37 0x00005645bca1c6ea ExecuteCC1Tool /usr/src/debug/clang18/clang-18.1.8.src/tools/driver/driver.cpp:365:20
#38 0x00007fca3e139485 operator() /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Job.cpp:440:32
#39 0x00007fca3e139485 callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::string*, bool*) const::<lambda()> > /usr/lib/llvm18/include/llvm/ADT/STLFunctionalExtras.h:45:52
#40 0x00007fca348ab72a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/CrashRecoveryContext.cpp:428:1
#41 0x00007fca3e13be16 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Job.cpp:444:10
#42 0x00007fca3e0f6d79 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Compilation.cpp:200:3
#43 0x00007fca3e0f717c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Compilation.cpp:253:5
#44 0x00007fca3e1157e4 llvm::SmallVectorBase<unsigned int>::empty() const /usr/lib/llvm18/include/llvm/ADT/SmallVector.h:94:0
#45 0x00007fca3e1157e4 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Driver.cpp:1921:0
#46 0x00005645bca1f045 llvm::SmallVectorBase<unsigned int>::size() const /usr/lib/llvm18/include/llvm/ADT/SmallVector.h:91:32
#47 0x00005645bca1f045 llvm::SmallVectorTemplateCommon<std::pair<int, clang::driver::Command const*>, void>::end() /usr/lib/llvm18/include/llvm/ADT/SmallVector.h:282:41
#48 0x00005645bca1f045 clang_main(int, char**, llvm::ToolContext const&) /usr/src/debug/clang18/clang-18.1.8.src/tools/driver/driver.cpp:542:26
#49 0x00005645bca0ffd5 main /usr/src/debug/clang18/clang-18.1.8.src/build/tools/driver/clang-driver.cpp:18:1
#50 0x00007fca33c376b5 (/usr/lib/libc.so.6+0x276b5)
#51 0x00007fca33c37769 __libc_start_main (/usr/lib/libc.so.6+0x27769)
#52 0x00005645bca10035 _start (/usr/lib/llvm18/bin/clang+++0xb035)
clang++: error: clang frontend command failed with exit code 135 (use -v to see invocation)
clang version 18.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm18/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/catacharset-773ed3.cpp
clang++: note: diagnostic msg: /tmp/catacharset-773ed3.sh
clang++: note: diagnostic msg: 

********************
```

[catacharset-773ed3.cpp.gz](https://github.com/user-attachments/files/21795273/catacharset-773ed3.cpp.gz)
[catacharset-773ed3.sh.gz](https://github.com/user-attachments/files/21795280/catacharset-773ed3.sh.gz)
Github rejected my attempts to upload the files uncompressed.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Clarence "Sparr" Risher (sparr)

<details>
I had not modified the body of `JsonObject::get_member_opt` when I encountered this crash. The most recent change prior to this crash was implementing `template&lt; typename T1, typename T2 &gt; bool JsonValue::read( std::variant&lt;T1, T2&gt; &amp;v, bool throw_on_error ) const`, although it is possible I made some other change since the previous successful compile.

```
Stack dump:
0.      Program arguments: /usr/lib/llvm18/bin/clang++ -Og -Xclang -fno-pch-timestamp -Wall -Wextra -Wformat-signedness -Wlogical-op -Wmissing-declarations -Wmissing-noreturn -Wpedantic -Wunused-macros -Wredundant-decls -g -fsigned-char -pthread -pthread -std=c++17 -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsuggest-override -Wzero-as-null-pointer-constant -Wno-unknown-warning-option -Wno-dangling-reference -Wno-c++20-compat -Winvalid-pch -Werror -Isrc -isystem src/third-party -DZSTD_STATIC_LINKING_ONLY -DZSTD_DISABLE_ASM -DGIT_VERSION -DTILES -I/usr/include/SDL2 -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/SDL2 -DHWY_SHARED_DEFINE -DAVIF_DLL -I/usr/include/rav1e -I/usr/include/svt-av1 -DEB_DLL -I/usr/include/webp -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/harfbuzz -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-6 -I/usr/include/SDL2 -D_GNU_SOURCE=1 -D_REENTRANT -DSDL_SOUND -include-pch pch/main-pch.hpp.pch -fpch-preprocess -c -MMD -MP -fcolor-diagnostics -o obj/tiles/catacharset.o src/catacharset.cpp
1.      &lt;unknown&gt; parser at unknown location
2.      src/flexbuffer_json-inl.h:1061:1: parsing function body 'JsonObject::get_member_opt'
3.      src/flexbuffer_json-inl.h:1061:1: in compound statement ('{}')
 #<!-- -->0 0x00007fca349ca540 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/Unix/Signals.inc:723:22
 #<!-- -->1 0x00007fca349c79f5 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/Signals.cpp:105:20
 #<!-- -->2 0x00007fca349c79f5 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/Unix/Signals.inc:367:31
 #<!-- -->3 0x00007fca348ab2d9 HandleCrash /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->4 0x00007fca348ab2d9 CrashRecoverySignalHandler /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #<!-- -->5 0x00007fca33c4def0 (/usr/lib/libc.so.6+0x3def0)
 #<!-- -->6 0x00007fca3c92a14d clang::isHorizontalWhitespace(unsigned char) /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Basic/CharInfo.h:78:22
 #<!-- -->7 0x00007fca3c92a14d clang::Lexer::LexTokenInternal(clang::Token&amp;, bool) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/Lexer.cpp:3668:29
 #<!-- -->8 0x00007fca3c9556cc clang::Preprocessor::Lex(clang::Token&amp;) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/Preprocessor.cpp:869:10
 #<!-- -->9 0x00007fca3c935ff8 clang::Preprocessor::CachingLex(clang::Token&amp;) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/PPCaching.cpp:65:3
#<!-- -->10 0x00007fca3c935ff8 clang::Preprocessor::CachingLex(clang::Token&amp;) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/PPCaching.cpp:48:6
#<!-- -->11 0x00007fca3c935ff8 clang::Preprocessor::CLK_CachingLexer(clang::Preprocessor&amp;, clang::Token&amp;) /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Lex/Preprocessor.h:2920:17
#<!-- -->12 0x00007fca3c9556cc clang::Preprocessor::Lex(clang::Token&amp;) /usr/src/debug/clang18/clang-18.1.8.src/lib/Lex/Preprocessor.cpp:869:10
#<!-- -->13 0x00007fca3ca5dfd3 clang::Parser::TryConsumeToken(clang::tok::TokenKind) /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Parse/Parser.h:528:11
#<!-- -->14 0x00007fca3ca5dfd3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:519:23
#<!-- -->15 0x00007fca3ca5e2b8 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /usr/lib/llvm18/include/llvm/ADT/SmallVector.h:94:46
#<!-- -->16 0x00007fca3ca5e2b8 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Parse/Parser.h:315:27
#<!-- -->17 0x00007fca3ca5e2b8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:120:26
#<!-- -->18 0x00007fca3ca63b6a clang::Parser::ParseCompoundStatementBody(bool) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:1236:38
#<!-- -->19 0x00007fca3ca65b34 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseStmt.cpp:2517:3
#<!-- -->20 0x00007fca3ca66c9f clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/Parser.cpp:1516:36
#<!-- -->21 0x00007fca3c9c4570 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseDecl.cpp:2272:50
#<!-- -->22 0x00007fca3ca50793 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/Parser.cpp:1266:42
#<!-- -->23 0x00007fca3ca69532 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/Parser.cpp:1070:50
#<!-- -->24 0x00007fca3ca6a14b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/Parser.cpp:760:36
#<!-- -->25 0x00007fca3c98cdf8 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang18/clang-18.1.8.src/lib/Parse/ParseAST.cpp:162:69
#<!-- -->26 0x00007fca3e463971 clang::FrontendAction::Execute() /usr/src/debug/clang18/clang-18.1.8.src/lib/Frontend/FrontendAction.cpp:1069:21
#<!-- -->27 0x00007fca3e404617 llvm::Error::getPtr() const /usr/lib/llvm18/include/llvm/Support/Error.h:279:0
#<!-- -->28 0x00007fca3e404617 llvm::Error::operator bool() /usr/lib/llvm18/include/llvm/Support/Error.h:239:0
#<!-- -->29 0x00007fca3e404617 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang18/clang-18.1.8.src/lib/Frontend/CompilerInstance.cpp:1057:0
#<!-- -->30 0x00007fca3e4bc0fd std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#<!-- -->31 0x00007fca3e4bc0fd std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#<!-- -->32 0x00007fca3e4bc0fd std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#<!-- -->33 0x00007fca3e4bc0fd clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Frontend/CompilerInvocation.h:254:0
#<!-- -->34 0x00007fca3e4bc0fd clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang18/clang-18.1.8.src/include/clang/Frontend/CompilerInstance.h:291:0
#<!-- -->35 0x00007fca3e4bc0fd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang18/clang-18.1.8.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:273:0
#<!-- -->36 0x00005645bca18384 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang18/clang-18.1.8.src/tools/driver/cc1_main.cpp:294:40
#<!-- -->37 0x00005645bca1c6ea ExecuteCC1Tool /usr/src/debug/clang18/clang-18.1.8.src/tools/driver/driver.cpp:365:20
#<!-- -->38 0x00007fca3e139485 operator() /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Job.cpp:440:32
#<!-- -->39 0x00007fca3e139485 callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::string*, bool*) const::&lt;lambda()&gt; &gt; /usr/lib/llvm18/include/llvm/ADT/STLFunctionalExtras.h:45:52
#<!-- -->40 0x00007fca348ab72a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /usr/src/debug/llvm18/llvm-18.1.8.src/lib/Support/CrashRecoveryContext.cpp:428:1
#<!-- -->41 0x00007fca3e13be16 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Job.cpp:444:10
#<!-- -->42 0x00007fca3e0f6d79 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Compilation.cpp:200:3
#<!-- -->43 0x00007fca3e0f717c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Compilation.cpp:253:5
#<!-- -->44 0x00007fca3e1157e4 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /usr/lib/llvm18/include/llvm/ADT/SmallVector.h:94:0
#<!-- -->45 0x00007fca3e1157e4 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /usr/src/debug/clang18/clang-18.1.8.src/lib/Driver/Driver.cpp:1921:0
#<!-- -->46 0x00005645bca1f045 llvm::SmallVectorBase&lt;unsigned int&gt;::size() const /usr/lib/llvm18/include/llvm/ADT/SmallVector.h:91:32
#<!-- -->47 0x00005645bca1f045 llvm::SmallVectorTemplateCommon&lt;std::pair&lt;int, clang::driver::Command const*&gt;, void&gt;::end() /usr/lib/llvm18/include/llvm/ADT/SmallVector.h:282:41
#<!-- -->48 0x00005645bca1f045 clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang18/clang-18.1.8.src/tools/driver/driver.cpp:542:26
#<!-- -->49 0x00005645bca0ffd5 main /usr/src/debug/clang18/clang-18.1.8.src/build/tools/driver/clang-driver.cpp:18:1
#<!-- -->50 0x00007fca33c376b5 (/usr/lib/libc.so.6+0x276b5)
#<!-- -->51 0x00007fca33c37769 __libc_start_main (/usr/lib/libc.so.6+0x27769)
#<!-- -->52 0x00005645bca10035 _start (/usr/lib/llvm18/bin/clang+++0xb035)
clang++: error: clang frontend command failed with exit code 135 (use -v to see invocation)
clang version 18.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm18/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: /tmp/catacharset-773ed3.cpp
clang++: note: diagnostic msg: /tmp/catacharset-773ed3.sh
clang++: note: diagnostic msg: 

********************
```

[catacharset-773ed3.cpp.gz](https://github.com/user-attachments/files/21795273/catacharset-773ed3.cpp.gz)
[catacharset-773ed3.sh.gz](https://github.com/user-attachments/files/21795280/catacharset-773ed3.sh.gz)
Github rejected my attempts to upload the files uncompressed.
</details>


---

