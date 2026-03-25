# LLVM ERROR: out of memory

**Author:** abhilashnv
**URL:** https://github.com/llvm/llvm-project/issues/181266
**Status:** Open
**Labels:** mlir, crash

## Body

Link to CE: https://godbolt.org/z/n4G9Krfdh
Summary: mlir-opt crashes with LLVM ERROR: out of memory when parsing a dense_resource dialect resource hex blob whose first 4 bytes decode to alignment=0.
The parser should reject alignment=0 with a diagnostic instead of crashing.
Reproducer:

```mlir
// RUN: mlir-opt %s

module {
  func.func @test_double_resource() -> tensor<2xf64> {
    %0 = arith.constant dense_resource<double_data> : tensor<2xf64>
    return %0 : tensor<2xf64>
  }
}

{-#
  dialect_resources: {
    builtin: {
      double_data: "0x000000000000F03F0000000000000040"
    }
  }
#-}
```

Error:

```
>$ mlir-opt reduced.mlir
LLVM ERROR: out of memory
Buffer allocation failed
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt reduced.mlir
 #0 0x00007665ce4c9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #1 0x00007665ce4c99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #2 0x00007665ce4c770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #3 0x00007665ce4ca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #4 0x00007665cdacc330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #5 0x00007665cdb25b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007665cdb25b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #7 0x00007665cdb25b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #8 0x00007665cdacc27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #9 0x00007665cdaaf8ff abort ./stdlib/abort.c:81:7
#10 0x00007665ce35536e /workspace/oss-llvm/llvm_src/llvm/lib/Support/ErrorHandling.cpp:207:3
#11 0x00007665ce39a224 llvm::allocate_buffer(unsigned long, unsigned long) /workspace/oss-llvm/llvm_src/llvm/lib/Support/MemAlloc.cpp:24:10
#12 0x00007665ce971413 mlir::HeapAsmResourceBlob::allocate(unsigned long, unsigned long, bool) /workspace/oss-llvm/llvm_src/mlir/include/mlir/IR/AsmState.h:197:32
#13 0x00007665ce9713d1 mlir::AsmParsedResourceEntry::parseAsBlob() const::'lambda'(unsigned long, unsigned long)::operator()(unsigned long, unsigned long) const /workspace/oss-llvm/llvm_src/mlir/include/mlir/IR/AsmState.h:327:14
#14 0x00007665ce971390 mlir::AsmResourceBlob llvm::function_ref<mlir::AsmResourceBlob (unsigned long, unsigned long)>::callback_fn<mlir::AsmParsedResourceEntry::parseAsBlob() const::'lambda'(unsigned long, unsigned long)>(long, unsigned long, unsigned long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#15 0x00007665d72b7e44 llvm::function_ref<mlir::AsmResourceBlob (unsigned long, unsigned long)>::operator()(unsigned long, unsigned long) const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#16 0x00007665d729aaf0 (anonymous namespace)::ParsedResourceEntry::parseAsBlob(llvm::function_ref<mlir::AsmResourceBlob (unsigned long, unsigned long)>) const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2718:5
#17 0x00007665ce96d16d mlir::AsmParsedResourceEntry::parseAsBlob() const /workspace/oss-llvm/llvm_src/mlir/include/mlir/IR/AsmState.h:326:12
#18 0x00007665cea9d1a8 (anonymous namespace)::BuiltinOpAsmDialectInterface::parseResource(mlir::AsmParsedResourceEntry&) const /workspace/oss-llvm/llvm_src/mlir/lib/IR/BuiltinDialect.cpp:85:16
#19 0x00007665d729a14c (anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0::operator()(llvm::StringRef, llvm::SMLoc) const::'lambda'()::operator()() const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2863:23
#20 0x00007665d7299f45 llvm::ParseResult llvm::function_ref<llvm::ParseResult ()>::callback_fn<(anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0::operator()(llvm::StringRef, llvm::SMLoc) const::'lambda'()>(long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#21 0x00007665d729b3b9 llvm::function_ref<llvm::ParseResult ()>::operator()() const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#22 0x00007665d728666c mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref<llvm::ParseResult ()>, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:138:7
#23 0x00007665d7263150 mlir::detail::Parser::parseCommaSeparatedList(llvm::function_ref<llvm::ParseResult ()>) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.h:59:12
#24 0x00007665d7286aa5 mlir::detail::Parser::parseCommaSeparatedListUntil(mlir::Token::Kind, llvm::function_ref<llvm::ParseResult ()>, bool) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:184:7
#25 0x00007665d7299eeb (anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0::operator()(llvm::StringRef, llvm::SMLoc) const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2852:12
#26 0x00007665d7299cb5 llvm::ParseResult llvm::function_ref<llvm::ParseResult (llvm::StringRef, llvm::SMLoc)>::callback_fn<(anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0>(long, llvm::StringRef, llvm::SMLoc) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#27 0x00007665d72b6b99 llvm::function_ref<llvm::ParseResult (llvm::StringRef, llvm::SMLoc)>::operator()(llvm::StringRef, llvm::SMLoc) const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#28 0x00007665d7299c5b (anonymous namespace)::TopLevelOperationParser::parseResourceFileMetadata(llvm::function_ref<llvm::ParseResult (llvm::StringRef, llvm::SMLoc)>)::$_0::operator()() const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2835:12
#29 0x00007665d7299a75 llvm::ParseResult llvm::function_ref<llvm::ParseResult ()>::callback_fn<(anonymous namespace)::TopLevelOperationParser::parseResourceFileMetadata(llvm::function_ref<llvm::ParseResult (llvm::StringRef, llvm::SMLoc)>)::$_0>(long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#30 0x00007665d729b3b9 llvm::function_ref<llvm::ParseResult ()>::operator()() const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#31 0x00007665d728666c mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref<llvm::ParseResult ()>, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:138:7
#32 0x00007665d7263150 mlir::detail::Parser::parseCommaSeparatedList(llvm::function_ref<llvm::ParseResult ()>) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.h:59:12
#33 0x00007665d7286aa5 mlir::detail::Parser::parseCommaSeparatedListUntil(mlir::Token::Kind, llvm::function_ref<llvm::ParseResult ()>, bool) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:184:7
#34 0x00007665d72999e9 (anonymous namespace)::TopLevelOperationParser::parseResourceFileMetadata(llvm::function_ref<llvm::ParseResult (llvm::StringRef, llvm::SMLoc)>) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2825:10
#35 0x00007665d72998cd (anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata() /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2840:10
#36 0x00007665d7299765 (anonymous namespace)::TopLevelOperationParser::parseFileMetadataDictionary()::$_0::operator()() const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2812:18
#37 0x00007665d72995c5 llvm::ParseResult llvm::function_ref<llvm::ParseResult ()>::callback_fn<(anonymous namespace)::TopLevelOperationParser::parseFileMetadataDictionary()::$_0>(long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#38 0x00007665d729b3b9 llvm::function_ref<llvm::ParseResult ()>::operator()() const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#39 0x00007665d728666c mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref<llvm::ParseResult ()>, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:138:7
#40 0x00007665d7263150 mlir::detail::Parser::parseCommaSeparatedList(llvm::function_ref<llvm::ParseResult ()>) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.h:59:12
#41 0x00007665d7286aa5 mlir::detail::Parser::parseCommaSeparatedListUntil(mlir::Token::Kind, llvm::function_ref<llvm::ParseResult ()>, bool) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:184:7
#42 0x00007665d728a666 (anonymous namespace)::TopLevelOperationParser::parseFileMetadataDictionary() /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2799:10
#43 0x00007665d7288f08 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2946:11
#44 0x00007665d7288b1e mlir::parseAsmSourceFile(llvm::SourceMgr const&, mlir::Block*, mlir::ParserConfig const&, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2968:41
#45 0x00007665dd34cdc9 mlir::parseSourceFile(std::shared_ptr<llvm::SourceMgr> const&, mlir::Block*, mlir::ParserConfig const&, mlir::LocationAttr*) /workspace/oss-llvm/llvm_src/mlir/lib/Parser/Parser.cpp:64:10
#46 0x00007665ed02798f mlir::OwningOpRef<mlir::ModuleOp> mlir::detail::parseSourceFile<mlir::ModuleOp, std::shared_ptr<llvm::SourceMgr> const&>(mlir::ParserConfig const&, std::shared_ptr<llvm::SourceMgr> const&) /workspace/oss-llvm/llvm_src/mlir/include/mlir/Parser/Parser.h:158:14
#47 0x00007665ed027898 mlir::OwningOpRef<mlir::ModuleOp> mlir::parseSourceFile<mlir::ModuleOp>(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&) /workspace/oss-llvm/llvm_src/mlir/include/mlir/Parser/Parser.h:188:10
#48 0x00007665ed0268a1 mlir::parseSourceFileForTool(std::shared_ptr<llvm::SourceMgr> const&, mlir::ParserConfig const&, bool) /workspace/oss-llvm/llvm_src/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#49 0x00007665ed003cb1 performActions(llvm::raw_ostream&, std::shared_ptr<llvm::SourceMgr> const&, mlir::MLIRContext*, mlir::MlirOptMainConfig const&) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#50 0x00007665ed003aab processBuffer(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&, mlir::DialectRegistry&, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#51 0x00007665ed003749 mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef, llvm::raw_ostream&) const /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#52 0x00007665ed003646 llvm::LogicalResult llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::callback_fn<mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&)::$_0>(long, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#53 0x00007665ce5f672a llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>::operator()(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&) const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#54 0x00007665ce5f5a81 mlir::splitAndProcessBuffer(std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::function_ref<llvm::LogicalResult (std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, llvm::MemoryBufferRef const&, llvm::raw_ostream&)>, llvm::raw_ostream&, llvm::StringRef, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/Support/ToolUtilities.cpp:30:12
#55 0x00007665ecfff32b mlir::MlirOptMain(llvm::raw_ostream&, std::unique_ptr<llvm::MemoryBuffer, std::default_delete<llvm::MemoryBuffer>>, mlir::DialectRegistry&, mlir::MlirOptMainConfig const&) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#56 0x00007665ecfff759 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#57 0x00007665ecfff928 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#58 0x000057a387410255 main /workspace/oss-llvm/llvm_src/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#59 0x00007665cdab11ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#60 0x00007665cdab128b call_init ./csu/../csu/libc-start.c:128:20
#61 0x00007665cdab128b __libc_start_main ./csu/../csu/libc-start.c:347:5
#62 0x000057a3874100c5 _start (/workspace/oss-llvm/bin/bin/mlir-opt+0xa0c5)
Aborted (core dumped) 
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360
CC: @alidavanian @Vsevolod-Livinskij as collaborators.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-mlir

Author: None (abhilashnv)

<details>
Link to CE: https://godbolt.org/z/n4G9Krfdh
Summary: mlir-opt crashes with LLVM ERROR: out of memory when parsing a dense_resource dialect resource hex blob whose first 4 bytes decode to alignment=0.
The parser should reject alignment=0 with a diagnostic instead of crashing.
Reproducer:

// RUN: mlir-opt %s

module {
  func.func @<!-- -->test_double_resource() -&gt; tensor&lt;2xf64&gt; {
    %0 = arith.constant dense_resource&lt;double_data&gt; : tensor&lt;2xf64&gt;
    return %0 : tensor&lt;2xf64&gt;
  }
}

{-#
  dialect_resources: {
    builtin: {
      double_data: "0x000000000000F03F0000000000000040"
    }
  }
#-}


Error:

&gt;$ mlir-opt reduced.mlir
LLVM ERROR: out of memory
Buffer allocation failed
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt reduced.mlir
 #<!-- -->0 0x00007665ce4c9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00007665ce4c99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00007665ce4c770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00007665ce4ca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007665cdacc330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007665cdb25b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007665cdb25b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007665cdb25b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007665cdacc27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007665cdaaf8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007665ce35536e /workspace/oss-llvm/llvm_src/llvm/lib/Support/ErrorHandling.cpp:207:3
#<!-- -->11 0x00007665ce39a224 llvm::allocate_buffer(unsigned long, unsigned long) /workspace/oss-llvm/llvm_src/llvm/lib/Support/MemAlloc.cpp:24:10
#<!-- -->12 0x00007665ce971413 mlir::HeapAsmResourceBlob::allocate(unsigned long, unsigned long, bool) /workspace/oss-llvm/llvm_src/mlir/include/mlir/IR/AsmState.h:197:32
#<!-- -->13 0x00007665ce9713d1 mlir::AsmParsedResourceEntry::parseAsBlob() const::'lambda'(unsigned long, unsigned long)::operator()(unsigned long, unsigned long) const /workspace/oss-llvm/llvm_src/mlir/include/mlir/IR/AsmState.h:327:14
#<!-- -->14 0x00007665ce971390 mlir::AsmResourceBlob llvm::function_ref&lt;mlir::AsmResourceBlob (unsigned long, unsigned long)&gt;::callback_fn&lt;mlir::AsmParsedResourceEntry::parseAsBlob() const::'lambda'(unsigned long, unsigned long)&gt;(long, unsigned long, unsigned long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->15 0x00007665d72b7e44 llvm::function_ref&lt;mlir::AsmResourceBlob (unsigned long, unsigned long)&gt;::operator()(unsigned long, unsigned long) const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->16 0x00007665d729aaf0 (anonymous namespace)::ParsedResourceEntry::parseAsBlob(llvm::function_ref&lt;mlir::AsmResourceBlob (unsigned long, unsigned long)&gt;) const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2718:5
#<!-- -->17 0x00007665ce96d16d mlir::AsmParsedResourceEntry::parseAsBlob() const /workspace/oss-llvm/llvm_src/mlir/include/mlir/IR/AsmState.h:326:12
#<!-- -->18 0x00007665cea9d1a8 (anonymous namespace)::BuiltinOpAsmDialectInterface::parseResource(mlir::AsmParsedResourceEntry&amp;) const /workspace/oss-llvm/llvm_src/mlir/lib/IR/BuiltinDialect.cpp:85:16
#<!-- -->19 0x00007665d729a14c (anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0::operator()(llvm::StringRef, llvm::SMLoc) const::'lambda'()::operator()() const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2863:23
#<!-- -->20 0x00007665d7299f45 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult ()&gt;::callback_fn&lt;(anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0::operator()(llvm::StringRef, llvm::SMLoc) const::'lambda'()&gt;(long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->21 0x00007665d729b3b9 llvm::function_ref&lt;llvm::ParseResult ()&gt;::operator()() const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->22 0x00007665d728666c mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref&lt;llvm::ParseResult ()&gt;, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:138:7
#<!-- -->23 0x00007665d7263150 mlir::detail::Parser::parseCommaSeparatedList(llvm::function_ref&lt;llvm::ParseResult ()&gt;) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.h:59:12
#<!-- -->24 0x00007665d7286aa5 mlir::detail::Parser::parseCommaSeparatedListUntil(mlir::Token::Kind, llvm::function_ref&lt;llvm::ParseResult ()&gt;, bool) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:184:7
#<!-- -->25 0x00007665d7299eeb (anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0::operator()(llvm::StringRef, llvm::SMLoc) const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2852:12
#<!-- -->26 0x00007665d7299cb5 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;::callback_fn&lt;(anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0&gt;(long, llvm::StringRef, llvm::SMLoc) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->27 0x00007665d72b6b99 llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;::operator()(llvm::StringRef, llvm::SMLoc) const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->28 0x00007665d7299c5b (anonymous namespace)::TopLevelOperationParser::parseResourceFileMetadata(llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;)::$_0::operator()() const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2835:12
#<!-- -->29 0x00007665d7299a75 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult ()&gt;::callback_fn&lt;(anonymous namespace)::TopLevelOperationParser::parseResourceFileMetadata(llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;)::$_0&gt;(long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->30 0x00007665d729b3b9 llvm::function_ref&lt;llvm::ParseResult ()&gt;::operator()() const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->31 0x00007665d728666c mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref&lt;llvm::ParseResult ()&gt;, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:138:7
#<!-- -->32 0x00007665d7263150 mlir::detail::Parser::parseCommaSeparatedList(llvm::function_ref&lt;llvm::ParseResult ()&gt;) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.h:59:12
#<!-- -->33 0x00007665d7286aa5 mlir::detail::Parser::parseCommaSeparatedListUntil(mlir::Token::Kind, llvm::function_ref&lt;llvm::ParseResult ()&gt;, bool) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:184:7
#<!-- -->34 0x00007665d72999e9 (anonymous namespace)::TopLevelOperationParser::parseResourceFileMetadata(llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2825:10
#<!-- -->35 0x00007665d72998cd (anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata() /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2840:10
#<!-- -->36 0x00007665d7299765 (anonymous namespace)::TopLevelOperationParser::parseFileMetadataDictionary()::$_0::operator()() const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2812:18
#<!-- -->37 0x00007665d72995c5 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult ()&gt;::callback_fn&lt;(anonymous namespace)::TopLevelOperationParser::parseFileMetadataDictionary()::$_0&gt;(long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->38 0x00007665d729b3b9 llvm::function_ref&lt;llvm::ParseResult ()&gt;::operator()() const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->39 0x00007665d728666c mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref&lt;llvm::ParseResult ()&gt;, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:138:7
#<!-- -->40 0x00007665d7263150 mlir::detail::Parser::parseCommaSeparatedList(llvm::function_ref&lt;llvm::ParseResult ()&gt;) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.h:59:12
#<!-- -->41 0x00007665d7286aa5 mlir::detail::Parser::parseCommaSeparatedListUntil(mlir::Token::Kind, llvm::function_ref&lt;llvm::ParseResult ()&gt;, bool) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:184:7
#<!-- -->42 0x00007665d728a666 (anonymous namespace)::TopLevelOperationParser::parseFileMetadataDictionary() /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2799:10
#<!-- -->43 0x00007665d7288f08 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2946:11
#<!-- -->44 0x00007665d7288b1e mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2968:41
#<!-- -->45 0x00007665dd34cdc9 mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workspace/oss-llvm/llvm_src/mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->46 0x00007665ed02798f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workspace/oss-llvm/llvm_src/mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->47 0x00007665ed027898 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /workspace/oss-llvm/llvm_src/mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->48 0x00007665ed0268a1 mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workspace/oss-llvm/llvm_src/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->49 0x00007665ed003cb1 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#<!-- -->50 0x00007665ed003aab processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->51 0x00007665ed003749 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->52 0x00007665ed003646 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->53 0x00007665ce5f672a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->54 0x00007665ce5f5a81 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->55 0x00007665ecfff32b mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->56 0x00007665ecfff759 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->57 0x00007665ecfff928 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->58 0x000057a387410255 main /workspace/oss-llvm/llvm_src/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->59 0x00007665cdab11ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->60 0x00007665cdab128b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->61 0x00007665cdab128b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->62 0x000057a3874100c5 _start (/workspace/oss-llvm/bin/bin/mlir-opt+0xa0c5)
Aborted (core dumped) 


LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360
CC: @<!-- -->alidavanian @<!-- -->Vsevolod-Livinskij as collaborators.
</details>


---

### Comment 2 - EugeneZelenko

@abhilashnv: Please use text blocks in issue descriptions. See current formatting as example.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: None (abhilashnv)

<details>
Link to CE: https://godbolt.org/z/n4G9Krfdh
Summary: mlir-opt crashes with LLVM ERROR: out of memory when parsing a dense_resource dialect resource hex blob whose first 4 bytes decode to alignment=0.
The parser should reject alignment=0 with a diagnostic instead of crashing.
Reproducer:

```mlir
// RUN: mlir-opt %s

module {
  func.func @<!-- -->test_double_resource() -&gt; tensor&lt;2xf64&gt; {
    %0 = arith.constant dense_resource&lt;double_data&gt; : tensor&lt;2xf64&gt;
    return %0 : tensor&lt;2xf64&gt;
  }
}

{-#
  dialect_resources: {
    builtin: {
      double_data: "0x000000000000F03F0000000000000040"
    }
  }
#-}
```

Error:

```
&gt;$ mlir-opt reduced.mlir
LLVM ERROR: out of memory
Buffer allocation failed
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: mlir-opt reduced.mlir
 #<!-- -->0 0x00007665ce4c9481 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:842:11
 #<!-- -->1 0x00007665ce4c99fb PrintStackTraceSignalHandler(void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:924:1
 #<!-- -->2 0x00007665ce4c770b llvm::sys::RunSignalHandlers() /workspace/oss-llvm/llvm_src/llvm/lib/Support/Signals.cpp:108:5
 #<!-- -->3 0x00007665ce4ca17d SignalHandler(int, siginfo_t*, void*) /workspace/oss-llvm/llvm_src/llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->4 0x00007665cdacc330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #<!-- -->5 0x00007665cdb25b2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #<!-- -->6 0x00007665cdb25b2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #<!-- -->7 0x00007665cdb25b2c pthread_kill ./nptl/pthread_kill.c:89:10
 #<!-- -->8 0x00007665cdacc27e raise ./signal/../sysdeps/posix/raise.c:27:6
 #<!-- -->9 0x00007665cdaaf8ff abort ./stdlib/abort.c:81:7
#<!-- -->10 0x00007665ce35536e /workspace/oss-llvm/llvm_src/llvm/lib/Support/ErrorHandling.cpp:207:3
#<!-- -->11 0x00007665ce39a224 llvm::allocate_buffer(unsigned long, unsigned long) /workspace/oss-llvm/llvm_src/llvm/lib/Support/MemAlloc.cpp:24:10
#<!-- -->12 0x00007665ce971413 mlir::HeapAsmResourceBlob::allocate(unsigned long, unsigned long, bool) /workspace/oss-llvm/llvm_src/mlir/include/mlir/IR/AsmState.h:197:32
#<!-- -->13 0x00007665ce9713d1 mlir::AsmParsedResourceEntry::parseAsBlob() const::'lambda'(unsigned long, unsigned long)::operator()(unsigned long, unsigned long) const /workspace/oss-llvm/llvm_src/mlir/include/mlir/IR/AsmState.h:327:14
#<!-- -->14 0x00007665ce971390 mlir::AsmResourceBlob llvm::function_ref&lt;mlir::AsmResourceBlob (unsigned long, unsigned long)&gt;::callback_fn&lt;mlir::AsmParsedResourceEntry::parseAsBlob() const::'lambda'(unsigned long, unsigned long)&gt;(long, unsigned long, unsigned long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->15 0x00007665d72b7e44 llvm::function_ref&lt;mlir::AsmResourceBlob (unsigned long, unsigned long)&gt;::operator()(unsigned long, unsigned long) const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->16 0x00007665d729aaf0 (anonymous namespace)::ParsedResourceEntry::parseAsBlob(llvm::function_ref&lt;mlir::AsmResourceBlob (unsigned long, unsigned long)&gt;) const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2718:5
#<!-- -->17 0x00007665ce96d16d mlir::AsmParsedResourceEntry::parseAsBlob() const /workspace/oss-llvm/llvm_src/mlir/include/mlir/IR/AsmState.h:326:12
#<!-- -->18 0x00007665cea9d1a8 (anonymous namespace)::BuiltinOpAsmDialectInterface::parseResource(mlir::AsmParsedResourceEntry&amp;) const /workspace/oss-llvm/llvm_src/mlir/lib/IR/BuiltinDialect.cpp:85:16
#<!-- -->19 0x00007665d729a14c (anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0::operator()(llvm::StringRef, llvm::SMLoc) const::'lambda'()::operator()() const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2863:23
#<!-- -->20 0x00007665d7299f45 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult ()&gt;::callback_fn&lt;(anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0::operator()(llvm::StringRef, llvm::SMLoc) const::'lambda'()&gt;(long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->21 0x00007665d729b3b9 llvm::function_ref&lt;llvm::ParseResult ()&gt;::operator()() const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->22 0x00007665d728666c mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref&lt;llvm::ParseResult ()&gt;, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:138:7
#<!-- -->23 0x00007665d7263150 mlir::detail::Parser::parseCommaSeparatedList(llvm::function_ref&lt;llvm::ParseResult ()&gt;) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.h:59:12
#<!-- -->24 0x00007665d7286aa5 mlir::detail::Parser::parseCommaSeparatedListUntil(mlir::Token::Kind, llvm::function_ref&lt;llvm::ParseResult ()&gt;, bool) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:184:7
#<!-- -->25 0x00007665d7299eeb (anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0::operator()(llvm::StringRef, llvm::SMLoc) const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2852:12
#<!-- -->26 0x00007665d7299cb5 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;::callback_fn&lt;(anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata()::$_0&gt;(long, llvm::StringRef, llvm::SMLoc) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->27 0x00007665d72b6b99 llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;::operator()(llvm::StringRef, llvm::SMLoc) const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->28 0x00007665d7299c5b (anonymous namespace)::TopLevelOperationParser::parseResourceFileMetadata(llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;)::$_0::operator()() const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2835:12
#<!-- -->29 0x00007665d7299a75 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult ()&gt;::callback_fn&lt;(anonymous namespace)::TopLevelOperationParser::parseResourceFileMetadata(llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;)::$_0&gt;(long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->30 0x00007665d729b3b9 llvm::function_ref&lt;llvm::ParseResult ()&gt;::operator()() const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->31 0x00007665d728666c mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref&lt;llvm::ParseResult ()&gt;, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:138:7
#<!-- -->32 0x00007665d7263150 mlir::detail::Parser::parseCommaSeparatedList(llvm::function_ref&lt;llvm::ParseResult ()&gt;) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.h:59:12
#<!-- -->33 0x00007665d7286aa5 mlir::detail::Parser::parseCommaSeparatedListUntil(mlir::Token::Kind, llvm::function_ref&lt;llvm::ParseResult ()&gt;, bool) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:184:7
#<!-- -->34 0x00007665d72999e9 (anonymous namespace)::TopLevelOperationParser::parseResourceFileMetadata(llvm::function_ref&lt;llvm::ParseResult (llvm::StringRef, llvm::SMLoc)&gt;) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2825:10
#<!-- -->35 0x00007665d72998cd (anonymous namespace)::TopLevelOperationParser::parseDialectResourceFileMetadata() /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2840:10
#<!-- -->36 0x00007665d7299765 (anonymous namespace)::TopLevelOperationParser::parseFileMetadataDictionary()::$_0::operator()() const /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2812:18
#<!-- -->37 0x00007665d72995c5 llvm::ParseResult llvm::function_ref&lt;llvm::ParseResult ()&gt;::callback_fn&lt;(anonymous namespace)::TopLevelOperationParser::parseFileMetadataDictionary()::$_0&gt;(long) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->38 0x00007665d729b3b9 llvm::function_ref&lt;llvm::ParseResult ()&gt;::operator()() const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->39 0x00007665d728666c mlir::detail::Parser::parseCommaSeparatedList(mlir::AsmParser::Delimiter, llvm::function_ref&lt;llvm::ParseResult ()&gt;, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:138:7
#<!-- -->40 0x00007665d7263150 mlir::detail::Parser::parseCommaSeparatedList(llvm::function_ref&lt;llvm::ParseResult ()&gt;) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.h:59:12
#<!-- -->41 0x00007665d7286aa5 mlir::detail::Parser::parseCommaSeparatedListUntil(mlir::Token::Kind, llvm::function_ref&lt;llvm::ParseResult ()&gt;, bool) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:184:7
#<!-- -->42 0x00007665d728a666 (anonymous namespace)::TopLevelOperationParser::parseFileMetadataDictionary() /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2799:10
#<!-- -->43 0x00007665d7288f08 (anonymous namespace)::TopLevelOperationParser::parse(mlir::Block*, mlir::Location) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2946:11
#<!-- -->44 0x00007665d7288b1e mlir::parseAsmSourceFile(llvm::SourceMgr const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::AsmParserState*, mlir::AsmParserCodeCompleteContext*) /workspace/oss-llvm/llvm_src/mlir/lib/AsmParser/Parser.cpp:2968:41
#<!-- -->45 0x00007665dd34cdc9 mlir::parseSourceFile(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::Block*, mlir::ParserConfig const&amp;, mlir::LocationAttr*) /workspace/oss-llvm/llvm_src/mlir/lib/Parser/Parser.cpp:64:10
#<!-- -->46 0x00007665ed02798f mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::detail::parseSourceFile&lt;mlir::ModuleOp, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;&gt;(mlir::ParserConfig const&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;) /workspace/oss-llvm/llvm_src/mlir/include/mlir/Parser/Parser.h:158:14
#<!-- -->47 0x00007665ed027898 mlir::OwningOpRef&lt;mlir::ModuleOp&gt; mlir::parseSourceFile&lt;mlir::ModuleOp&gt;(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;) /workspace/oss-llvm/llvm_src/mlir/include/mlir/Parser/Parser.h:188:10
#<!-- -->48 0x00007665ed0268a1 mlir::parseSourceFileForTool(std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::ParserConfig const&amp;, bool) /workspace/oss-llvm/llvm_src/mlir/include/mlir/Tools/ParseUtilities.h:31:12
#<!-- -->49 0x00007665ed003cb1 performActions(llvm::raw_ostream&amp;, std::shared_ptr&lt;llvm::SourceMgr&gt; const&amp;, mlir::MLIRContext*, mlir::MlirOptMainConfig const&amp;) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:523:16
#<!-- -->50 0x00007665ed003aab processBuffer(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, mlir::MlirOptMainConfig const&amp;, mlir::DialectRegistry&amp;, mlir::SourceMgrDiagnosticVerifierHandler*, llvm::ThreadPoolInterface*) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:673:12
#<!-- -->51 0x00007665ed003749 mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef, llvm::raw_ostream&amp;) const /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:771:12
#<!-- -->52 0x00007665ed003646 llvm::LogicalResult llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::callback_fn&lt;mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;)::$_0&gt;(long, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:12
#<!-- -->53 0x00007665ce5f672a llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;::operator()(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;) const /workspace/oss-llvm/llvm_src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
#<!-- -->54 0x00007665ce5f5a81 mlir::splitAndProcessBuffer(std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::function_ref&lt;llvm::LogicalResult (std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, llvm::MemoryBufferRef const&amp;, llvm::raw_ostream&amp;)&gt;, llvm::raw_ostream&amp;, llvm::StringRef, llvm::StringRef) /workspace/oss-llvm/llvm_src/mlir/lib/Support/ToolUtilities.cpp:30:12
#<!-- -->55 0x00007665ecfff32b mlir::MlirOptMain(llvm::raw_ostream&amp;, std::unique_ptr&lt;llvm::MemoryBuffer, std::default_delete&lt;llvm::MemoryBuffer&gt;&gt;, mlir::DialectRegistry&amp;, mlir::MlirOptMainConfig const&amp;) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:776:26
#<!-- -->56 0x00007665ecfff759 mlir::MlirOptMain(int, char**, llvm::StringRef, llvm::StringRef, mlir::DialectRegistry&amp;) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:822:14
#<!-- -->57 0x00007665ecfff928 mlir::MlirOptMain(int, char**, llvm::StringRef, mlir::DialectRegistry&amp;) /workspace/oss-llvm/llvm_src/mlir/lib/Tools/mlir-opt/MlirOptMain.cpp:838:10
#<!-- -->58 0x000057a387410255 main /workspace/oss-llvm/llvm_src/mlir/tools/mlir-opt/mlir-opt.cpp:347:33
#<!-- -->59 0x00007665cdab11ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->60 0x00007665cdab128b call_init ./csu/../csu/libc-start.c:128:20
#<!-- -->61 0x00007665cdab128b __libc_start_main ./csu/../csu/libc-start.c:347:5
#<!-- -->62 0x000057a3874100c5 _start (/workspace/oss-llvm/bin/bin/mlir-opt+0xa0c5)
Aborted (core dumped) 
```

LLVM commit: 6716acd588462c6b33afa1633c4a5ddbcec59360
CC: @<!-- -->alidavanian @<!-- -->Vsevolod-Livinskij as collaborators.
</details>


---

