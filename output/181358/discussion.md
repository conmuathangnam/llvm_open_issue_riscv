# Clangd crashes when importing a non-existent c++20 module, if the import statement is the last statement in the file.

**Author:** neek78
**URL:** https://github.com/llvm/llvm-project/issues/181358
**Status:** Open
**Labels:** clangd, clang:modules, crash

## Body

Clangd crashes when importing a non-existent c++20 module, if that import statement is the last statement in the file.
This is with trunk (debug build) as of a few hours ago, tested on macOS. The same crash happens on linux, it also blows up in a release build too (though obviously not at an assert). 

It looks like the final statement is not being fully "pushed through" - hence the failure at this assert

Assertion failed: (Result.ExpandedTokens.back().kind() == tok::eof), function build, file Tokens.cpp, line 715.

I've traced it as best I can, it seems to enter this case in clang/lib/Parser/Parser.cpp:2464 
`
  if (PP.hadModuleLoaderFatalFailure()) {
`
But that case doesn't seem to effect anything, and the current token seems to just get lost. Alas, i'm somewhat at the end of my ability to trace it further. 

```
# | V[13:49:15.570] Building first preamble for /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp version 1
# | I[13:49:15.642] Built preamble of size 832408 for file /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp version 1 in 0.07 seconds
# | I[13:49:15.807] Built module A to /var/folders/4m/kw96hwgn60j2gryrqk71d8n40000gp/T/lit-tmp-483hpyp_/clangd/module_files/A.cppm12420089627662187155-5c-7a-06-2c-b0-98/A.pcm
# | E[13:49:15.807] Failed to build module NonExistent; due to Don't get the module unit for module NonExistent
# | I[13:49:15.807] Built prerequisite modules for file /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp in 0.17 seconds
# | V[13:49:15.810] indexed preamble AST for /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp version 1:
# |   symbol slab: 0 symbols, 120 bytes
# |   ref slab: 0 symbols, 0 refs, 128 bytes
# |   relations slab: 0 relations, 24 bytes
# | Assertion failed: (Result.ExpandedTokens.back().kind() == tok::eof), function build, file Tokens.cpp, line 715.
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# | Stack dump:
# | 0.	Program arguments: clangd -experimental-modules-support -lit-test
# |  #0 0x00000001011f0980 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063c980)
# |  #1 0x00000001011f0f38 PrintStackTraceSignalHandler(void*) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063cf38)
# |  #2 0x00000001011ee5dc llvm::sys::RunSignalHandlers() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063a5dc)
# |  #3 0x00000001011f2fb4 SignalHandler(int, __siginfo*, void*) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063efb4)
# |  #4 0x000000018155b744 (/usr/lib/system/libsystem_platform.dylib+0x1804e3744)
# |  #5 0x0000000181551888 (/usr/lib/system/libsystem_pthread.dylib+0x1804d9888)
# |  #6 0x0000000181456850 (/usr/lib/system/libsystem_c.dylib+0x1803de850)
# |  #7 0x0000000181455a84 (/usr/lib/system/libsystem_c.dylib+0x1803dda84)
# |  #8 0x00000001036a485c clang::syntax::TokenCollector::Builder::build() && (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102af085c)
# |  #9 0x00000001036a466c clang::syntax::TokenCollector::consume() && (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102af066c)
# | #10 0x0000000103d99288 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::__1::unique_ptr<clang::CompilerInvocation, std::__1::default_delete<clang::CompilerInvocation>>, llvm::ArrayRef<clang::clangd::Diag>, std::__1::shared_ptr<clang::clangd::PreambleData const>) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1031e5288)
# | #11 0x0000000103f63154 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::__1::unique_ptr<clang::CompilerInvocation, std::__1::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::__1::vector<clang::clangd::Diag, std::__1::allocator<clang::clangd::Diag>>) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033af154)
# | #12 0x0000000103f62d54 clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::__1::unique_ptr<clang::CompilerInvocation, std::__1::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::__1::shared_ptr<clang::clangd::PreambleData const>, std::__1::vector<clang::clangd::Diag, std::__1::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics)::$_0::operator()() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033aed54)
# | #13 0x0000000103f62bd4 void llvm::function_ref<void ()>::callback_fn<clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::__1::unique_ptr<clang::CompilerInvocation, std::__1::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::__1::shared_ptr<clang::clangd::PreambleData const>, std::__1::vector<clang::clangd::Diag, std::__1::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics)::$_0>(long) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033aebd4)
# | #14 0x000000010104e39c llvm::function_ref<void ()>::operator()() const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10049a39c)
# | #15 0x0000000103f58808 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033a4808)
# | #16 0x0000000103f6286c clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::__1::unique_ptr<clang::CompilerInvocation, std::__1::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::__1::shared_ptr<clang::clangd::PreambleData const>, std::__1::vector<clang::clangd::Diag, std::__1::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ae86c)
# | #17 0x0000000103f62700 clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request)::$_2::operator()() const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ae700)
# | #18 0x0000000103f62648 llvm::scope_exit<clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request)::$_2>::~scope_exit() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ae648)
# | #19 0x0000000103f60424 llvm::scope_exit<clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request)::$_2>::~scope_exit() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ac424)
# | #20 0x0000000103f5f6d4 clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ab6d4)
# | #21 0x0000000103f69b9c clang::clangd::(anonymous namespace)::PreambleThread::update(std::__1::unique_ptr<clang::CompilerInvocation, std::__1::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::__1::vector<clang::clangd::Diag, std::__1::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b5b9c)
# | #22 0x0000000103f69704 clang::clangd::(anonymous namespace)::ASTWorker::update(clang::clangd::ParseInputs, clang::clangd::WantDiagnostics, bool)::$_0::operator()() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b5704)
# | #23 0x0000000103f69150 void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::(anonymous namespace)::ASTWorker::update(clang::clangd::ParseInputs, clang::clangd::WantDiagnostics, bool)::$_0>(void*) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b5150)
# | #24 0x0000000103a40344 llvm::unique_function<void ()>::operator()() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102e8c344)
# | #25 0x0000000103f5e95c void llvm::function_ref<void ()>::callback_fn<llvm::unique_function<void ()>>(long) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033aa95c)
# | #26 0x000000010104e39c llvm::function_ref<void ()>::operator()() const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10049a39c)
# | #27 0x0000000103f58808 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033a4808)
# | #28 0x0000000103f680ac clang::clangd::(anonymous namespace)::ASTWorker::startTask(llvm::StringRef, llvm::unique_function<void ()>, std::__1::optional<clang::clangd::(anonymous namespace)::ASTWorker::UpdateType>, clang::clangd::TUScheduler::ASTActionInvalidation) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b40ac)
# | #29 0x0000000103f50d58 clang::clangd::(anonymous namespace)::ASTWorker::update(clang::clangd::ParseInputs, clang::clangd::WantDiagnostics, bool) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10339cd58)
# | #30 0x0000000103f50784 clang::clangd::TUScheduler::update(llvm::StringRef, clang::clangd::ParseInputs, clang::clangd::WantDiagnostics) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10339c784)
# | #31 0x00000001039c74bc clang::clangd::ClangdServer::addDocument(llvm::StringRef, llvm::StringRef, llvm::StringRef, clang::clangd::WantDiagnostics, bool) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102e134bc)
# | #32 0x00000001038e1168 clang::clangd::ClangdLSPServer::onDocumentDidOpen(clang::clangd::DidOpenTextDocumentParams const&) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d2d168)
# | #33 0x000000010399d1f0 void clang::clangd::LSPBinder::notification<clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer>(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&))::'lambda'(llvm::json::Value)::operator()(llvm::json::Value) const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102de91f0)
# | #34 0x000000010399d0bc void llvm::detail::UniqueFunctionBase<void, llvm::json::Value>::CallImpl<void clang::clangd::LSPBinder::notification<clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer>(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&))::'lambda'(llvm::json::Value)>(void*, llvm::json::Value&) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102de90bc)
# | #35 0x0000000103927014 llvm::unique_function<void (llvm::json::Value)>::operator()(llvm::json::Value) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d73014)
# | #36 0x0000000103925d58 clang::clangd::ClangdLSPServer::MessageHandler::onNotify(llvm::StringRef, llvm::json::Value) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d71d58)
# | #37 0x0000000103d55ce4 clang::clangd::(anonymous namespace)::JSONTransport::handleMessage(llvm::json::Value, clang::clangd::Transport::MessageHandler&) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1031a1ce4)
# | #38 0x0000000103d54314 clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1031a0314)
# | #39 0x00000001038eb460 clang::clangd::ClangdLSPServer::run() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d37460)
# | #40 0x00000001036bd938 clang::clangd::clangdMain(int, char**) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102b09938)
# | #41 0x0000000100fbf32c main (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10040b32c)
# | #42 0x0000000181189d54
# | Signalled during AST worker action: Build AST
# |   Filename: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Directory: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp
# |   Command Line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/%clang -fprebuilt-module-path=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp -std=c++20 -o /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/main.cpp.o -fmodule-file=A=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/A.pcm -resource-dir=/Volumes/devel/build/clang.trunk.debug/lib/clang/23 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -- /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Version: 1
# | Signalled during AST worker action: Update
# |   Filename: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Directory: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp
# |   Command Line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/%clang -fprebuilt-module-path=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp -std=c++20 -o /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/main.cpp.o -fmodule-file=A=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/A.pcm -resource-dir=/Volumes/devel/build/clang.trunk.debug/lib/clang/23 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -- /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Version: 1
# | Signalled while processing message:
# | {
# |   "jsonrpc": "2.0",
# |   "method": "textDocument/didOpen",
# |   "params": {
# |     "textDocument": {
# |       "uri": "file:///Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp",
# |       "languageId": "cpp",
# |       "version": 1,
# |       "text": "module;\nexport module Use;\nimport A;\nimport NonExistent;\n"
# |     }
# |   }
# | }
# |
# `-----------------------------
# error: command failed with exit status: -6
```



## Comments

### Comment 1 - neek78

Here is  a test case that reproduces the crash -

```
% cat clang-tools-extra/clangd/test/module-crash.test
#
## reproduce a crash in module processing - seems having a non-existent module imported
## as the last statement in a file causes clangd to crash
## modified from modules.test
#
# Windows have different escaping modes.
# FIXME: We should add one for windows.
# UNSUPPORTED: system-windows
#
# RUN: rm -fr %t
# RUN: mkdir -p %t
# RUN: split-file %s %t
#
# RUN: sed -e "s|DIR|%/t|g" %t/compile_commands.json.tmpl > %t/compile_commands.json.tmp
# RUN: sed -e "s|CLANG_CC|%clang|g" %t/compile_commands.json.tmp > %t/compile_commands.json
# RUN: sed -e "s|DIR|%/t|g" %t/definition.jsonrpc.tmpl > %t/definition.jsonrpc
#
# RUN: clangd -experimental-modules-support -lit-test < %t/definition.jsonrpc

#--- A.cppm
module;
export module A;

#--- Use.cpp
module;
export module Use;
import A;
import NonExistent;

#--- compile_commands.json.tmpl
[
    {
      "directory": "DIR",
      "command": "CLANG_CC -fprebuilt-module-path=DIR -std=c++20 -o DIR/main.cpp.o  DIR/Use.cpp  -fmodule-file=A=DIR/A.pcm",
      "file": "DIR/Use.cpp"
      "output": "DIR/main.cpp.o"
    },
    {
      "directory": "DIR",
      "command": "CLANG_CC -fprebuilt-module-path=DIR --std=c++20 DIR/A.cppm --precompile -o DIR/A.pcm",
      "file": "DIR/A.cppm"
      "output": "DIR/A.pcm"
    }
]

#--- definition.jsonrpc.tmpl
{
  "jsonrpc": "2.0",
  "id": 0,
  "method": "initialize",
  "params": {
    "processId": 123,
    "rootPath": "clangd",
    "capabilities": {
      "textDocument": {
        "completion": {
          "completionItem": {
            "snippetSupport": true
          }
        }
      }
    },
    "trace": "off"
  }
}
---
{
  "jsonrpc": "2.0",
  "method": "textDocument/didOpen",
  "params": {
    "textDocument": {
      "uri": "file://DIR/Use.cpp",
      "languageId": "cpp",
      "version": 1,
      "text": "module;\nexport module Use;\nimport A;\nimport NonExistent;\n"
    }
  }
}
---
{"jsonrpc":"2.0","id":2,"method":"shutdown"}
---
{"jsonrpc":"2.0","method":"exit"}

```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clangd

Author: Nick Begg (neek78)

<details>
Clangd crashes when importing a non-existent c++20 module, if that import statement is the last statement in the file.
This is with trunk (debug build) as of a few hours ago, tested on macOS. The same crash happens on linux, it also blows up in a release build too (though obviously not at an assert). 

It looks like the final statement is not being fully "pushed through" - hence the failure at this assert

Assertion failed: (Result.ExpandedTokens.back().kind() == tok::eof), function build, file Tokens.cpp, line 715.

I've traced it as best I can, it seems to enter this case in clang/lib/Parser/Parser.cpp:2464 
`
  if (PP.hadModuleLoaderFatalFailure()) {
`
But that case doesn't seem to effect anything, and the current token seems to just get lost. Alas, i'm somewhat at the end of my ability to trace it further. 

```
# | V[13:49:15.570] Building first preamble for /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp version 1
# | I[13:49:15.642] Built preamble of size 832408 for file /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp version 1 in 0.07 seconds
# | I[13:49:15.807] Built module A to /var/folders/4m/kw96hwgn60j2gryrqk71d8n40000gp/T/lit-tmp-483hpyp_/clangd/module_files/A.cppm12420089627662187155-5c-7a-06-2c-b0-98/A.pcm
# | E[13:49:15.807] Failed to build module NonExistent; due to Don't get the module unit for module NonExistent
# | I[13:49:15.807] Built prerequisite modules for file /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp in 0.17 seconds
# | V[13:49:15.810] indexed preamble AST for /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp version 1:
# |   symbol slab: 0 symbols, 120 bytes
# |   ref slab: 0 symbols, 0 refs, 128 bytes
# |   relations slab: 0 relations, 24 bytes
# | Assertion failed: (Result.ExpandedTokens.back().kind() == tok::eof), function build, file Tokens.cpp, line 715.
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# | Stack dump:
# | 0.	Program arguments: clangd -experimental-modules-support -lit-test
# |  #<!-- -->0 0x00000001011f0980 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063c980)
# |  #<!-- -->1 0x00000001011f0f38 PrintStackTraceSignalHandler(void*) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063cf38)
# |  #<!-- -->2 0x00000001011ee5dc llvm::sys::RunSignalHandlers() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063a5dc)
# |  #<!-- -->3 0x00000001011f2fb4 SignalHandler(int, __siginfo*, void*) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063efb4)
# |  #<!-- -->4 0x000000018155b744 (/usr/lib/system/libsystem_platform.dylib+0x1804e3744)
# |  #<!-- -->5 0x0000000181551888 (/usr/lib/system/libsystem_pthread.dylib+0x1804d9888)
# |  #<!-- -->6 0x0000000181456850 (/usr/lib/system/libsystem_c.dylib+0x1803de850)
# |  #<!-- -->7 0x0000000181455a84 (/usr/lib/system/libsystem_c.dylib+0x1803dda84)
# |  #<!-- -->8 0x00000001036a485c clang::syntax::TokenCollector::Builder::build() &amp;&amp; (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102af085c)
# |  #<!-- -->9 0x00000001036a466c clang::syntax::TokenCollector::consume() &amp;&amp; (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102af066c)
# | #<!-- -->10 0x0000000103d99288 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::__1::shared_ptr&lt;clang::clangd::PreambleData const&gt;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1031e5288)
# | #<!-- -->11 0x0000000103f63154 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033af154)
# | #<!-- -->12 0x0000000103f62d54 clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033aed54)
# | #<!-- -->13 0x0000000103f62bd4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0&gt;(long) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033aebd4)
# | #<!-- -->14 0x000000010104e39c llvm::function_ref&lt;void ()&gt;::operator()() const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10049a39c)
# | #<!-- -->15 0x0000000103f58808 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033a4808)
# | #<!-- -->16 0x0000000103f6286c clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ae86c)
# | #<!-- -->17 0x0000000103f62700 clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request)::$_2::operator()() const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ae700)
# | #<!-- -->18 0x0000000103f62648 llvm::scope_exit&lt;clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request)::$_2&gt;::~scope_exit() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ae648)
# | #<!-- -->19 0x0000000103f60424 llvm::scope_exit&lt;clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request)::$_2&gt;::~scope_exit() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ac424)
# | #<!-- -->20 0x0000000103f5f6d4 clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ab6d4)
# | #<!-- -->21 0x0000000103f69b9c clang::clangd::(anonymous namespace)::PreambleThread::update(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b5b9c)
# | #<!-- -->22 0x0000000103f69704 clang::clangd::(anonymous namespace)::ASTWorker::update(clang::clangd::ParseInputs, clang::clangd::WantDiagnostics, bool)::$_0::operator()() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b5704)
# | #<!-- -->23 0x0000000103f69150 void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::update(clang::clangd::ParseInputs, clang::clangd::WantDiagnostics, bool)::$_0&gt;(void*) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b5150)
# | #<!-- -->24 0x0000000103a40344 llvm::unique_function&lt;void ()&gt;::operator()() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102e8c344)
# | #<!-- -->25 0x0000000103f5e95c void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;llvm::unique_function&lt;void ()&gt;&gt;(long) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033aa95c)
# | #<!-- -->26 0x000000010104e39c llvm::function_ref&lt;void ()&gt;::operator()() const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10049a39c)
# | #<!-- -->27 0x0000000103f58808 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033a4808)
# | #<!-- -->28 0x0000000103f680ac clang::clangd::(anonymous namespace)::ASTWorker::startTask(llvm::StringRef, llvm::unique_function&lt;void ()&gt;, std::__1::optional&lt;clang::clangd::(anonymous namespace)::ASTWorker::UpdateType&gt;, clang::clangd::TUScheduler::ASTActionInvalidation) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b40ac)
# | #<!-- -->29 0x0000000103f50d58 clang::clangd::(anonymous namespace)::ASTWorker::update(clang::clangd::ParseInputs, clang::clangd::WantDiagnostics, bool) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10339cd58)
# | #<!-- -->30 0x0000000103f50784 clang::clangd::TUScheduler::update(llvm::StringRef, clang::clangd::ParseInputs, clang::clangd::WantDiagnostics) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10339c784)
# | #<!-- -->31 0x00000001039c74bc clang::clangd::ClangdServer::addDocument(llvm::StringRef, llvm::StringRef, llvm::StringRef, clang::clangd::WantDiagnostics, bool) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102e134bc)
# | #<!-- -->32 0x00000001038e1168 clang::clangd::ClangdLSPServer::onDocumentDidOpen(clang::clangd::DidOpenTextDocumentParams const&amp;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d2d168)
# | #<!-- -->33 0x000000010399d1f0 void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)::operator()(llvm::json::Value) const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102de91f0)
# | #<!-- -->34 0x000000010399d0bc void llvm::detail::UniqueFunctionBase&lt;void, llvm::json::Value&gt;::CallImpl&lt;void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)&gt;(void*, llvm::json::Value&amp;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102de90bc)
# | #<!-- -->35 0x0000000103927014 llvm::unique_function&lt;void (llvm::json::Value)&gt;::operator()(llvm::json::Value) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d73014)
# | #<!-- -->36 0x0000000103925d58 clang::clangd::ClangdLSPServer::MessageHandler::onNotify(llvm::StringRef, llvm::json::Value) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d71d58)
# | #<!-- -->37 0x0000000103d55ce4 clang::clangd::(anonymous namespace)::JSONTransport::handleMessage(llvm::json::Value, clang::clangd::Transport::MessageHandler&amp;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1031a1ce4)
# | #<!-- -->38 0x0000000103d54314 clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&amp;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1031a0314)
# | #<!-- -->39 0x00000001038eb460 clang::clangd::ClangdLSPServer::run() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d37460)
# | #<!-- -->40 0x00000001036bd938 clang::clangd::clangdMain(int, char**) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102b09938)
# | #<!-- -->41 0x0000000100fbf32c main (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10040b32c)
# | #<!-- -->42 0x0000000181189d54
# | Signalled during AST worker action: Build AST
# |   Filename: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Directory: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp
# |   Command Line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/%clang -fprebuilt-module-path=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp -std=c++20 -o /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/main.cpp.o -fmodule-file=A=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/A.pcm -resource-dir=/Volumes/devel/build/clang.trunk.debug/lib/clang/23 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -- /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Version: 1
# | Signalled during AST worker action: Update
# |   Filename: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Directory: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp
# |   Command Line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/%clang -fprebuilt-module-path=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp -std=c++20 -o /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/main.cpp.o -fmodule-file=A=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/A.pcm -resource-dir=/Volumes/devel/build/clang.trunk.debug/lib/clang/23 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -- /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Version: 1
# | Signalled while processing message:
# | {
# |   "jsonrpc": "2.0",
# |   "method": "textDocument/didOpen",
# |   "params": {
# |     "textDocument": {
# |       "uri": "file:///Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp",
# |       "languageId": "cpp",
# |       "version": 1,
# |       "text": "module;\nexport module Use;\nimport A;\nimport NonExistent;\n"
# |     }
# |   }
# | }
# |
# `-----------------------------
# error: command failed with exit status: -6
```


</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Nick Begg (neek78)

<details>
Clangd crashes when importing a non-existent c++20 module, if that import statement is the last statement in the file.
This is with trunk (debug build) as of a few hours ago, tested on macOS. The same crash happens on linux, it also blows up in a release build too (though obviously not at an assert). 

It looks like the final statement is not being fully "pushed through" - hence the failure at this assert

Assertion failed: (Result.ExpandedTokens.back().kind() == tok::eof), function build, file Tokens.cpp, line 715.

I've traced it as best I can, it seems to enter this case in clang/lib/Parser/Parser.cpp:2464 
`
  if (PP.hadModuleLoaderFatalFailure()) {
`
But that case doesn't seem to effect anything, and the current token seems to just get lost. Alas, i'm somewhat at the end of my ability to trace it further. 

```
# | V[13:49:15.570] Building first preamble for /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp version 1
# | I[13:49:15.642] Built preamble of size 832408 for file /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp version 1 in 0.07 seconds
# | I[13:49:15.807] Built module A to /var/folders/4m/kw96hwgn60j2gryrqk71d8n40000gp/T/lit-tmp-483hpyp_/clangd/module_files/A.cppm12420089627662187155-5c-7a-06-2c-b0-98/A.pcm
# | E[13:49:15.807] Failed to build module NonExistent; due to Don't get the module unit for module NonExistent
# | I[13:49:15.807] Built prerequisite modules for file /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp in 0.17 seconds
# | V[13:49:15.810] indexed preamble AST for /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp version 1:
# |   symbol slab: 0 symbols, 120 bytes
# |   ref slab: 0 symbols, 0 refs, 128 bytes
# |   relations slab: 0 relations, 24 bytes
# | Assertion failed: (Result.ExpandedTokens.back().kind() == tok::eof), function build, file Tokens.cpp, line 715.
# | PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
# | Stack dump:
# | 0.	Program arguments: clangd -experimental-modules-support -lit-test
# |  #<!-- -->0 0x00000001011f0980 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063c980)
# |  #<!-- -->1 0x00000001011f0f38 PrintStackTraceSignalHandler(void*) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063cf38)
# |  #<!-- -->2 0x00000001011ee5dc llvm::sys::RunSignalHandlers() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063a5dc)
# |  #<!-- -->3 0x00000001011f2fb4 SignalHandler(int, __siginfo*, void*) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10063efb4)
# |  #<!-- -->4 0x000000018155b744 (/usr/lib/system/libsystem_platform.dylib+0x1804e3744)
# |  #<!-- -->5 0x0000000181551888 (/usr/lib/system/libsystem_pthread.dylib+0x1804d9888)
# |  #<!-- -->6 0x0000000181456850 (/usr/lib/system/libsystem_c.dylib+0x1803de850)
# |  #<!-- -->7 0x0000000181455a84 (/usr/lib/system/libsystem_c.dylib+0x1803dda84)
# |  #<!-- -->8 0x00000001036a485c clang::syntax::TokenCollector::Builder::build() &amp;&amp; (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102af085c)
# |  #<!-- -->9 0x00000001036a466c clang::syntax::TokenCollector::consume() &amp;&amp; (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102af066c)
# | #<!-- -->10 0x0000000103d99288 clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::__1::shared_ptr&lt;clang::clangd::PreambleData const&gt;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1031e5288)
# | #<!-- -->11 0x0000000103f63154 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033af154)
# | #<!-- -->12 0x0000000103f62d54 clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033aed54)
# | #<!-- -->13 0x0000000103f62bd4 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0&gt;(long) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033aebd4)
# | #<!-- -->14 0x000000010104e39c llvm::function_ref&lt;void ()&gt;::operator()() const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10049a39c)
# | #<!-- -->15 0x0000000103f58808 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033a4808)
# | #<!-- -->16 0x0000000103f6286c clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ae86c)
# | #<!-- -->17 0x0000000103f62700 clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request)::$_2::operator()() const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ae700)
# | #<!-- -->18 0x0000000103f62648 llvm::scope_exit&lt;clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request)::$_2&gt;::~scope_exit() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ae648)
# | #<!-- -->19 0x0000000103f60424 llvm::scope_exit&lt;clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request)::$_2&gt;::~scope_exit() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ac424)
# | #<!-- -->20 0x0000000103f5f6d4 clang::clangd::(anonymous namespace)::PreambleThread::build(clang::clangd::(anonymous namespace)::PreambleThread::Request) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033ab6d4)
# | #<!-- -->21 0x0000000103f69b9c clang::clangd::(anonymous namespace)::PreambleThread::update(std::__1::unique_ptr&lt;clang::CompilerInvocation, std::__1::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::__1::vector&lt;clang::clangd::Diag, std::__1::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b5b9c)
# | #<!-- -->22 0x0000000103f69704 clang::clangd::(anonymous namespace)::ASTWorker::update(clang::clangd::ParseInputs, clang::clangd::WantDiagnostics, bool)::$_0::operator()() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b5704)
# | #<!-- -->23 0x0000000103f69150 void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::update(clang::clangd::ParseInputs, clang::clangd::WantDiagnostics, bool)::$_0&gt;(void*) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b5150)
# | #<!-- -->24 0x0000000103a40344 llvm::unique_function&lt;void ()&gt;::operator()() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102e8c344)
# | #<!-- -->25 0x0000000103f5e95c void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;llvm::unique_function&lt;void ()&gt;&gt;(long) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033aa95c)
# | #<!-- -->26 0x000000010104e39c llvm::function_ref&lt;void ()&gt;::operator()() const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10049a39c)
# | #<!-- -->27 0x0000000103f58808 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033a4808)
# | #<!-- -->28 0x0000000103f680ac clang::clangd::(anonymous namespace)::ASTWorker::startTask(llvm::StringRef, llvm::unique_function&lt;void ()&gt;, std::__1::optional&lt;clang::clangd::(anonymous namespace)::ASTWorker::UpdateType&gt;, clang::clangd::TUScheduler::ASTActionInvalidation) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1033b40ac)
# | #<!-- -->29 0x0000000103f50d58 clang::clangd::(anonymous namespace)::ASTWorker::update(clang::clangd::ParseInputs, clang::clangd::WantDiagnostics, bool) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10339cd58)
# | #<!-- -->30 0x0000000103f50784 clang::clangd::TUScheduler::update(llvm::StringRef, clang::clangd::ParseInputs, clang::clangd::WantDiagnostics) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10339c784)
# | #<!-- -->31 0x00000001039c74bc clang::clangd::ClangdServer::addDocument(llvm::StringRef, llvm::StringRef, llvm::StringRef, clang::clangd::WantDiagnostics, bool) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102e134bc)
# | #<!-- -->32 0x00000001038e1168 clang::clangd::ClangdLSPServer::onDocumentDidOpen(clang::clangd::DidOpenTextDocumentParams const&amp;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d2d168)
# | #<!-- -->33 0x000000010399d1f0 void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)::operator()(llvm::json::Value) const (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102de91f0)
# | #<!-- -->34 0x000000010399d0bc void llvm::detail::UniqueFunctionBase&lt;void, llvm::json::Value&gt;::CallImpl&lt;void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)&gt;(void*, llvm::json::Value&amp;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102de90bc)
# | #<!-- -->35 0x0000000103927014 llvm::unique_function&lt;void (llvm::json::Value)&gt;::operator()(llvm::json::Value) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d73014)
# | #<!-- -->36 0x0000000103925d58 clang::clangd::ClangdLSPServer::MessageHandler::onNotify(llvm::StringRef, llvm::json::Value) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d71d58)
# | #<!-- -->37 0x0000000103d55ce4 clang::clangd::(anonymous namespace)::JSONTransport::handleMessage(llvm::json::Value, clang::clangd::Transport::MessageHandler&amp;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1031a1ce4)
# | #<!-- -->38 0x0000000103d54314 clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&amp;) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x1031a0314)
# | #<!-- -->39 0x00000001038eb460 clang::clangd::ClangdLSPServer::run() (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102d37460)
# | #<!-- -->40 0x00000001036bd938 clang::clangd::clangdMain(int, char**) (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x102b09938)
# | #<!-- -->41 0x0000000100fbf32c main (/Volumes/devel/build/clang.trunk.debug/bin/clangd+0x10040b32c)
# | #<!-- -->42 0x0000000181189d54
# | Signalled during AST worker action: Build AST
# |   Filename: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Directory: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp
# |   Command Line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/%clang -fprebuilt-module-path=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp -std=c++20 -o /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/main.cpp.o -fmodule-file=A=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/A.pcm -resource-dir=/Volumes/devel/build/clang.trunk.debug/lib/clang/23 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -- /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Version: 1
# | Signalled during AST worker action: Update
# |   Filename: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Directory: /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp
# |   Command Line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/%clang -fprebuilt-module-path=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp -std=c++20 -o /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/main.cpp.o -fmodule-file=A=/Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/A.pcm -resource-dir=/Volumes/devel/build/clang.trunk.debug/lib/clang/23 -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -- /Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp
# |   Version: 1
# | Signalled while processing message:
# | {
# |   "jsonrpc": "2.0",
# |   "method": "textDocument/didOpen",
# |   "params": {
# |     "textDocument": {
# |       "uri": "file:///Volumes/devel/build/clang.trunk.debug/tools/clang/tools/extra/clangd/test/Output/module-crash.test.tmp/Use.cpp",
# |       "languageId": "cpp",
# |       "version": 1,
# |       "text": "module;\nexport module Use;\nimport A;\nimport NonExistent;\n"
# |     }
# |   }
# | }
# |
# `-----------------------------
# error: command failed with exit status: -6
```


</details>


---

### Comment 4 - ChuanqiXu9

Located https://github.com/llvm/llvm-project/pull/173130

---

### Comment 5 - yronglin

Fix PR: https://github.com/llvm/llvm-project/pull/187858

---

### Comment 6 - yronglin

This is a existing issue, not a regression, before https://github.com/llvm/llvm-project/pull/173130, the issue already exists. I try to use llvm-21.1.8 release, this issue can be reproduced. So I removed the regression label.

---

