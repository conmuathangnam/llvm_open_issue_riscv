# [clangd] crash when using #embed

**Author:** Nemi26
**URL:** https://github.com/llvm/llvm-project/issues/107869
**Status:** Closed
**Labels:** clang:frontend, crash, embed
**Closed Date:** 2025-04-17T13:15:09Z

## Body

```c
static constexpr unsigned char data[] = {
#embed "data"
};
```
I use sublime text with the LSP plugin and clangd. hovering my mouse over `data` causes it to crash

## Comments

### Comment 1 - pinskia

Dup of https://github.com/llvm/llvm-project/issues/107724 ? At least related to.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clangd

Author: Streetware (StreetwareGames)

<details>
```c
static constexpr unsigned char data[] = {
#embed "data"
};
```
I use sublime text with the LSP plugin and clangd. hovering my mouse over `data` causes it to crash
</details>


---

### Comment 3 - HighCommander4

What clangd version are you using?

---

### Comment 4 - circl-lastname

Also have this problem:
```
[15:49:26 ￼ LSP Server Log] c@/home/maks
LLVM ERROR: Not implemented
[15:49:26 ￼ LSP Server Log] c@/home/maks
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0 libLLVM.so.19.1 0x00007f371d0b72c6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 54
1 libLLVM.so.19.1 0x00007f371d0b4f70 llvm::sys::RunSignalHandlers() + 80
2 libLLVM.so.19.1 0x00007f371d0b798b
3 libc.so.6 0x00007f371b45b050
4 libc.so.6 0x00007f371b4a9ebc
5 libc.so.6 0x00007f371b45afb2 gsignal + 18
6 libc.so.6 0x00007f371b445472 abort + 211
7 libLLVM.so.19.1 0x00007f371d00bb24 llvm::report_fatal_error(llvm::Twine const&, bool) + 436
8 libLLVM.so.19.1 0x00007f371d00b966
9 libclang-cpp.so.19.1 0x00007f372505b3fb
10 libclang-cpp.so.19.1 0x00007f37250600e5
11 libclang-cpp.so.19.1 0x00007f3725059f71 clang::Stmt::printPretty(llvm::raw_ostream&, clang::PrinterHelper*, clang::PrintingPolicy const&, unsigned int, llvm::StringRef, clang::ASTContext const*) const + 241
12 libclang-cpp.so.19.1 0x00007f3724d21c6e
13 libclang-cpp.so.19.1 0x00007f3724d1adb2 clang::Decl::print(llvm::raw_ostream&, clang::PrintingPolicy const&, unsigned int, bool) const + 82
14 clangd 0x0000556d373c81e6
15 clangd 0x0000556d373c5a8b
16 clangd 0x0000556d372f8453
17 clangd 0x0000556d374ce779
18 clangd 0x0000556d374c625c
19 clangd 0x0000556d374c5dcf
20 clangd 0x0000556d3762d66a
21 libc.so.6 0x00007f371b4a81c4
22 libc.so.6 0x00007f371b52885c
Signalled during AST worker action: Hover
Filename: main.c
Directory: /home/maks/Source/MBLF/Source
Command Line: /usr/bin/clang -xc -std=c23 -resource-dir=/usr/lib/llvm-19/lib/clang/19 -- /home/maks/Source/MBLF/Source/main.c
Version: 2
```

---

### Comment 5 - HighCommander4

@circl-lastname can you share a complete code example that triggers the crash and [clangd logs](https://clangd.llvm.org/troubleshooting.html#gathering-logs) please?

---

### Comment 6 - circl-lastname

@HighCommander4
The file `example.txt`:
```
Hello World! 
```
The file `example.c`:
```c
#include <stdio.h>

char hello_message[] = {
  #embed "example.txt"
};

int main(int argc, char** argv) {
  fwrite(hello_message, sizeof(hello_message), 1, stdout);
}
```
Compiled with:
```
maks@maks-ryzen:~/Source/example$ clang-19 -std=c23 example.c -o example
maks@maks-ryzen:~/Source/example$ ./example 
Hello World! 
```
`clangd` version:
```
maks@maks-ryzen:~$ clangd --version
Debian clangd version 19.1.5 (++20241119043506+51dee6b64fda-1~exp1~20241119163522.67)
Features: linux+grpc
Platform: x86_64-pc-linux-gnu
maks@maks-ryzen:~$ 
```
Full logs when trying to hover hello_message:
```
I[15:16:48.481] PID: 17276
I[15:16:48.481] Working directory: /home/maks
I[15:16:48.481] argv[0]: /usr/bin/clangd
I[15:16:48.481] argv[1]: -log=verbose
I[15:16:48.481] argv[2]: --background-index
I[15:16:48.481] argv[3]: --limit-results=500
I[15:16:48.481] argv[4]: --completion-style=bundled
V[15:16:48.481] User config file is /home/maks/.config/clangd/config.yaml
I[15:16:48.481] Starting LSP over stdin/stdout
V[15:16:48.481] 
I[15:16:48.481] 
I[15:16:48.482] --> reply:initialize(1) 0 ms
V[15:16:48.482] >>> {"id":1,"jsonrpc":"2.0","result":{"capabilities":{"astProvider":true,"callHierarchyProvider":true,"clangdInlayHintsProvider":true,"codeActionProvider":{"codeActionKinds":["quickfix","refactor","info"]},"compilationDatabase":{"automaticReload":true},"completionProvider":{"resolveProvider":false,"triggerCharacters":[".","",":","\"","/","*"]},"declarationProvider":true,"definitionProvider":true,"documentFormattingProvider":true,"documentHighlightProvider":true,"documentLinkProvider":{"resolveProvider":false},"documentOnTypeFormattingProvider":{"firstTriggerCharacter":"\n","moreTriggerCharacter":[]},"documentRangeFormattingProvider":true,"documentSymbolProvider":true,"executeCommandProvider":{"commands":["clangd.applyFix","clangd.applyRename","clangd.applyTweak"]},"foldingRangeProvider":true,"hoverProvider":true,"implementationProvider":true,"inactiveRegionsProvider":true,"inlayHintProvider":true,"memoryUsageProvider":true,"referencesProvider":true,"renameProvider":true,"selectionRangeProvider":true,"semanticTokensProvider":{"full":{"delta":true},"legend":{"tokenModifiers":["declaration","definition","deprecated","deduced","readonly","static","abstract","virtual","dependentName","defaultLibrary","usedAsMutableReference","usedAsMutablePointer","constructorOrDestructor","userDefined","functionScope","classScope","fileScope","globalScope"],"tokenTypes":["variable","variable","parameter","function","method","function","property","variable","class","interface","enum","enumMember","type","type","unknown","namespace","typeParameter","concept","type","macro","modifier","operator","bracket","label","comment"]},"range":false},"signatureHelpProvider":{"triggerCharacters":["(",")","{","}","",","]},"standardTypeHierarchyProvider":true,"textDocumentSync":{"change":2,"openClose":true,"save":true},"typeDefinitionProvider":true,"typeHierarchyProvider":true,"workspaceSymbolProvider":true},"serverInfo":{"name":"clangd","version":"Debian clangd version 19.1.5 (++20241119043506+51dee6b64fda-1~exp1~20241119163522.67) linux+grpc x86_64-pc-linux-gnu"}}}
V[15:16:48.489] 
I[15:16:48.489] 
V[15:16:48.489] \n\nchar hello_message[] = {\n #embed \"example.txt\"\n};\n\nint main(int argc, char** argv) {\n fwrite(hello_message, sizeof(hello_message), 1, stdout);\n}\n","uri":"file:///home/maks/Source/example/example.c","version":0}}}
I[15:16:48.489] 
V[15:16:48.489] 
I[15:16:48.489] 
V[15:16:48.490] config note at /home/maks/Source/example/.clangd:1:0: Parsing config fragment
V[15:16:48.490] config note at /home/maks/Source/example/.clangd:1:0: Parsed 1 fragments from file
V[15:16:48.490] Config fragment: compiling /home/maks/Source/example/.clangd:1 -> 0x00007F7F8C002800 (trusted=false)
I[15:16:48.490] --> textDocument/publishDiagnostics
V[15:16:48.490] >>> {"jsonrpc":"2.0","method":"textDocument/publishDiagnostics","params":{"diagnostics":[],"uri":"file:///home/maks/Source/example/.clangd"}}
I[15:16:48.490] Failed to find compilation database for /home/maks/Source/example/example.c
I[15:16:48.490] ASTWorker building file /home/maks/Source/example/example.c version 0 with command clangd fallback
[/home/maks/Source/example]
/usr/bin/clang -xc -std=c23 -resource-dir=/usr/lib/llvm-19/lib/clang/19 -- /home/maks/Source/example/example.c
V[15:16:48.491] Driver produced command: cc1 -cc1 -triple x86_64-pc-linux-gnu -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/maks/Source/example -fcoverage-compilation-dir=/home/maks/Source/example -resource-dir /usr/lib/llvm-19/lib/clang/19 -internal-isystem /usr/lib/llvm-19/lib/clang/19/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c23 -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -no-round-trip-args -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -x c /home/maks/Source/example/example.c
V[15:16:48.491] Building first preamble for /home/maks/Source/example/example.c version 0
I[15:16:48.501] Built preamble of size 308024 for file /home/maks/Source/example/example.c version 0 in 0.01 seconds
I[15:16:48.501] Indexing c23 standard library in the context of /home/maks/Source/example/example.c
V[15:16:48.505] indexed preamble AST for /home/maks/Source/example/example.c version 0:
symbol slab: 294 symbols, 84712 bytes
ref slab: 0 symbols, 0 refs, 128 bytes
relations slab: 0 relations, 24 bytes
V[15:16:48.506] Build dynamic index for header symbols with estimated memory usage of 504896 bytes
V[15:16:48.507] indexed file AST for /home/maks/Source/example/example.c version 0:
symbol slab: 2 symbols, 4680 bytes
ref slab: 5 symbols, 7 refs, 4416 bytes
relations slab: 0 relations, 24 bytes
V[15:16:48.507] Build dynamic index for main-file symbols with estimated memory usage of 12344 bytes
I[15:16:48.507] --> textDocument/publishDiagnostics
V[15:16:48.507] >>> {"jsonrpc":"2.0","method":"textDocument/publishDiagnostics","params":{"diagnostics":[],"uri":"file:///home/maks/Source/example/example.c","version":0}}
V[15:16:48.508] ASTWorker running DocumentSymbols on version 0 of /home/maks/Source/example/example.c
I[15:16:48.508] --> reply:textDocument/documentSymbol(2) 18 ms
V[15:16:48.508] >>> {"id":2,"jsonrpc":"2.0","result":[{"detail":"char[14]","kind":13,"name":"hello_message","range":{"end":{"character":1,"line":4},"start":{"character":0,"line":2}},"selectionRange":{"end":{"character":18,"line":2},"start":{"character":5,"line":2}}},{"detail":"int (int, char **)","kind":12,"name":"main","range":{"end":{"character":1,"line":8},"start":{"character":0,"line":6}},"selectionRange":{"end":{"character":8,"line":6},"start":{"character":4,"line":6}}}]}
V[15:16:48.530] 
I[15:16:48.530] 
V[15:16:48.530] ASTWorker running SemanticHighlights on version 0 of /home/maks/Source/example/example.c
I[15:16:48.530] --> reply:textDocument/semanticTokens/full(3) 0 ms
V[15:16:48.531] >>> {"id":3,"jsonrpc":"2.0","result":{"data":[2,5,13,0,131075,4,4,4,3,131075,0,9,4,2,16387,0,13,4,2,16387,1,2,6,3,131584,0,7,13,0,131072,0,22,13,0,131072,0,19,6,19,131072],"resultId":"1"}}
I[15:16:48.547] Indexed c23 standard library: 2025 symbols, 30 filtered
V[15:16:48.551] Build dynamic index for header symbols with estimated memory usage of 2166844 bytes
V[15:16:52.410] 
I[15:16:52.410] 
V[15:16:52.410] ASTWorker running Hover on version 0 of /home/maks/Source/example/example.c
LLVM ERROR: Not implemented
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0 libLLVM.so.19.1 0x00007f7faf4b72c6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 54
1 libLLVM.so.19.1 0x00007f7faf4b4f70 llvm::sys::RunSignalHandlers() + 80
2 libLLVM.so.19.1 0x00007f7faf4b798b
3 libc.so.6 0x00007f7fad85b050
4 libc.so.6 0x00007f7fad8a9ebc
5 libc.so.6 0x00007f7fad85afb2 gsignal + 18
6 libc.so.6 0x00007f7fad845472 abort + 211
7 libLLVM.so.19.1 0x00007f7faf40bb24 llvm::report_fatal_error(llvm::Twine const&, bool) + 436
8 libLLVM.so.19.1 0x00007f7faf40b966
9 libclang-cpp.so.19.1 0x00007f7fb745b3fb
10 libclang-cpp.so.19.1 0x00007f7fb74600e5
11 libclang-cpp.so.19.1 0x00007f7fb7459f71 clang::Stmt::printPretty(llvm::raw_ostream&, clang::PrinterHelper*, clang::PrintingPolicy const&, unsigned int, llvm::StringRef, clang::ASTContext const*) const + 241
12 libclang-cpp.so.19.1 0x00007f7fb7121c6e
13 libclang-cpp.so.19.1 0x00007f7fb711adb2 clang::Decl::print(llvm::raw_ostream&, clang::PrintingPolicy const&, unsigned int, bool) const + 82
14 clangd 0x000055ede394c1e6
15 clangd 0x000055ede3949a8b
16 clangd 0x000055ede387c453
17 clangd 0x000055ede3a52779
18 clangd 0x000055ede3a4a25c
19 clangd 0x000055ede3a49dcf
20 clangd 0x000055ede3bb166a
21 libc.so.6 0x00007f7fad8a81c4
22 libc.so.6 0x00007f7fad92885c
Signalled during AST worker action: Hover
Filename: example.c
Directory: /home/maks/Source/example
Command Line: /usr/bin/clang -xc -std=c23 -resource-dir=/usr/lib/llvm-19/lib/clang/19 -- /home/maks/Source/example/example.c
Version: 0
[15:16:52 ￼ LSP Client Warning] Server terminated unexpectedly ... Restarting [/usr/bin/clangd -log=verbose --background-index --limit-results=500 --completion-style=bundled] [homepage: https://clang.llvm.org/extra/clangd/]
```

---

### Comment 7 - circl-lastname

I can try the llvm-symbolizer if it helps

---

### Comment 8 - HighCommander4

Thanks, I can reproduce the issue.

Here is a stack trace from a debug build:

```
LLVM ERROR: Not implemented
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
 #0 0x00007f43a9133b7d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm/lib/Support/Unix/Signals.inc:723:11
 #1 0x00007f43a913406b PrintStackTraceSignalHandler(void*) llvm/lib/Support/Unix/Signals.inc:798:1
 #2 0x00007f43a91320f6 llvm::sys::RunSignalHandlers() llvm/lib/Support/Signals.cpp:105:5
 #3 0x00007f43a91347f5 SignalHandler(int) llvm/lib/Support/Unix/Signals.inc:413:1
 #4 0x00007f43afced140 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x13140)
 #5 0x00007f43a8865d51 raise ./signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #6 0x00007f43a884f537 abort ./stdlib/abort.c:81:7
 #7 0x00007f43a9000984 llvm::report_fatal_error(llvm::Twine const&, bool) llvm/lib/Support/ErrorHandling.cpp:126:5
 #8 0x00007f43a90007f2 llvm/lib/Support/ErrorHandling.cpp:84:3
 #9 0x00007f43ab517d21 clang/lib/AST/StmtPrinter.cpp:1196:3
#10 0x00007f43ab5129d4 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::StmtPrinter, void>::Visit(clang::Stmt*) build/tools/clang/include/clang/AST/StmtNodes.inc:438:1
#11 0x00007f43ab511e47 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) clang/lib/AST/StmtPrinter.cpp:151:5
#12 0x00007f43ab51efe0 (anonymous namespace)::StmtPrinter::PrintExpr(clang::Expr*) clang/lib/AST/StmtPrinter.cpp:136:9
#13 0x00007f43ab51892d (anonymous namespace)::StmtPrinter::VisitImplicitCastExpr(clang::ImplicitCastExpr*) clang/lib/AST/StmtPrinter.cpp:1693:1
#14 0x00007f43ab512abe clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::StmtPrinter, void>::Visit(clang::Stmt*) build/tools/clang/include/clang/AST/StmtNodes.inc:528:1
#15 0x00007f43ab511e47 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) clang/lib/AST/StmtPrinter.cpp:151:5
#16 0x00007f43ab51efe0 (anonymous namespace)::StmtPrinter::PrintExpr(clang::Expr*) clang/lib/AST/StmtPrinter.cpp:136:9
#17 0x00007f43ab5173b2 (anonymous namespace)::StmtPrinter::VisitInitListExpr(clang::InitListExpr*) clang/lib/AST/StmtPrinter.cpp:1775:7
#18 0x00007f43ab5128ea clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::StmtPrinter, void>::Visit(clang::Stmt*) build/tools/clang/include/clang/AST/StmtNodes.inc:352:1
#19 0x00007f43ab511e47 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) clang/lib/AST/StmtPrinter.cpp:151:5
#20 0x00007f43ab517330 (anonymous namespace)::StmtPrinter::VisitInitListExpr(clang::InitListExpr*) clang/lib/AST/StmtPrinter.cpp:1768:5
#21 0x00007f43ab5128ea clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::StmtPrinter, void>::Visit(clang::Stmt*) build/tools/clang/include/clang/AST/StmtNodes.inc:352:1
#22 0x00007f43ab511e47 (anonymous namespace)::StmtPrinter::Visit(clang::Stmt*) clang/lib/AST/StmtPrinter.cpp:151:5
#23 0x00007f43ab511d3b clang::Stmt::printPretty(llvm::raw_ostream&, clang::PrinterHelper*, clang::PrintingPolicy const&, unsigned int, llvm::StringRef, clang::ASTContext const*) const clang/lib/AST/StmtPrinter.cpp:2824:1
#24 0x00007f43aaef18f0 (anonymous namespace)::DeclPrinter::VisitVarDecl(clang::VarDecl*) clang/lib/AST/DeclPrinter.cpp:998:12
#25 0x00007f43aaeebe3b clang::declvisitor::Base<std::add_pointer, (anonymous namespace)::DeclPrinter, void>::Visit(clang::Decl*) build/tools/clang/include/clang/AST/DeclNodes.inc:278:1
#26 0x00007f43aaeebaea clang::Decl::print(llvm::raw_ostream&, clang::PrintingPolicy const&, unsigned int, bool) const clang/lib/AST/DeclPrinter.cpp:140:1
#27 0x00007f43af467825 clang::clangd::(anonymous namespace)::printDefinition[abi:cxx11](clang::Decl const*, clang::PrintingPolicy, clang::syntax::TokenBuffer const&) clang-tools-extra/clangd/Hover.cpp:153:3
#28 0x00007f43af4616f8 clang::clangd::(anonymous namespace)::getHoverContents(clang::NamedDecl const*, clang::PrintingPolicy const&, clang::clangd::SymbolIndex const*, clang::syntax::TokenBuffer const&) clang-tools-extra/clangd/Hover.cpp:674:19
#29 0x00007f43af45f884 clang::clangd::getHover(clang::clangd::ParsedAST&, clang::clangd::Position, clang::format::FormatStyle const&, clang::clangd::SymbolIndex const*) clang-tools-extra/clangd/Hover.cpp:1366:12
#30 0x00007f43af1a04fa clang::clangd::ClangdServer::findHover(llvm::StringRef, clang::clangd::Position, llvm::unique_function<void (llvm::Expected<std::optional<clang::clangd::HoverInfo>>)>)::$_0::operator()(llvm::Expected<clang::clangd::InputsAndAST>) clang-tools-extra/clangd/ClangdServer.cpp:837:8
#31 0x00007f43af1a030a void llvm::detail::UniqueFunctionBase<void, llvm::Expected<clang::clangd::InputsAndAST>>::CallImpl<clang::clangd::ClangdServer::findHover(llvm::StringRef, clang::clangd::Position, llvm::unique_function<void (llvm::Expected<std::optional<clang::clangd::HoverInfo>>)>)::$_0>(void*, llvm::Expected<clang::clangd::InputsAndAST>&) llvm/include/llvm/ADT/FunctionExtras.h:222:5
#32 0x00007f43af7462d7 llvm::unique_function<void (llvm::Expected<clang::clangd::InputsAndAST>)>::operator()(llvm::Expected<clang::clangd::InputsAndAST>) llvm/include/llvm/ADT/FunctionExtras.h:387:5
#33 0x00007f43af744324 clang::clangd::(anonymous namespace)::ASTWorker::runWithAST(llvm::StringRef, llvm::unique_function<void (llvm::Expected<clang::clangd::InputsAndAST>)>, clang::clangd::TUScheduler::ASTActionInvalidation)::$_0::operator()() clang-tools-extra/clangd/TUScheduler.cpp:1009:5
#34 0x00007f43af743e9d void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::(anonymous namespace)::ASTWorker::runWithAST(llvm::StringRef, llvm::unique_function<void (llvm::Expected<clang::clangd::InputsAndAST>)>, clang::clangd::TUScheduler::ASTActionInvalidation)::$_0>(void*) llvm/include/llvm/ADT/FunctionExtras.h:222:5
#35 0x00007f43af218daf llvm::unique_function<void ()>::operator()() llvm/include/llvm/ADT/FunctionExtras.h:387:5
#36 0x00007f43af749345 void llvm::function_ref<void ()>::callback_fn<llvm::unique_function<void ()>>(long) llvm/include/llvm/ADT/STLFunctionalExtras.h:45:5
#37 0x00007f43af749329 llvm::function_ref<void ()>::operator()() const llvm/include/llvm/ADT/STLFunctionalExtras.h:68:5
#38 0x00007f43af73aa54 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) clang-tools-extra/clangd/TUScheduler.cpp:1325:1
#39 0x00007f43af73a0f7 clang::clangd::(anonymous namespace)::ASTWorker::run() clang-tools-extra/clangd/TUScheduler.cpp:1459:5
#40 0x00007f43af73999d clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0::operator()() const clang-tools-extra/clangd/TUScheduler.cpp:822:49
#41 0x00007f43af73991d void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0>(void*) llvm/include/llvm/ADT/FunctionExtras.h:222:5
#42 0x00007f43aa05096f llvm::unique_function<void ()>::operator()() llvm/include/llvm/ADT/FunctionExtras.h:387:5
#43 0x00007f43aa052f53 clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1::operator()() clang-tools-extra/clangd/support/Threading.cpp:103:14
#44 0x00007f43aa052f19 auto void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)::operator()<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) const llvm/include/llvm/Support/thread.h:44:9
#45 0x00007f43aa052eed auto std::__invoke_impl<void, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(std::__invoke_other, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)&&, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/invoke.h:60:7
#46 0x00007f43aa052e9d std::__invoke_result<auto, auto...>::type std::__invoke<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/bits/invoke.h:95:7
#47 0x00007f43aa052e6d decltype(auto) std::__apply_impl<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, 0ul>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, std::integer_sequence<unsigned long, 0ul>) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/tuple:1723:7
#48 0x00007f43aa052dcd decltype(auto) std::apply<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&) /usr/bin/../lib/gcc/x86_64-linux-gnu/10/../../../../include/c++/10/tuple:1734:7
#49 0x00007f43aa052d9e void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) llvm/include/llvm/Support/thread.h:46:3
#50 0x00007f43aa052a65 void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) llvm/include/llvm/Support/thread.h:56:5
#51 0x00007f43afce1ea7 start_thread ./nptl/pthread_create.c:478:7
#52 0x00007f43a8928acf __clone ./misc/../sysdeps/unix/sysv/linux/x86_64/clone.S:97:0
```

The crash is here:

https://github.com/llvm/llvm-project/blob/3fb0bea859efaf401ad0ce420d7b75e3ff1c4746/clang/lib/AST/StmtPrinter.cpp#L1204-L1206

---

### Comment 9 - circl-lastname

I might think about how #embed is supposed to be printed then. From what I understand EmbedExpr can be a partial reference to a file, the `if_empty` fallback, and probably must account for the prefix, suffix.

---

### Comment 10 - Fznamznon

@HighCommander4 , could you please also help to understand if [#135957](https://github.com/llvm/llvm-project/pull/135957) fixes this issue?

---

### Comment 11 - HighCommander4

> [@HighCommander4](https://github.com/HighCommander4) , could you please also help to understand if [#135957](https://github.com/llvm/llvm-project/pull/135957) fixes this issue?

Yes, it does.

---

### Comment 12 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Streetware (StreetwareGames)

<details>
```c
static constexpr unsigned char data[] = {
#embed "data"
};
```
I use sublime text with the LSP plugin and clangd. hovering my mouse over `data` causes it to crash
</details>


---

