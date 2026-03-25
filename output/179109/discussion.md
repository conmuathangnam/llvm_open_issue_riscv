# clangd crashes when i try to find incomint/outcoming calls on non valid target

**Author:** NeKon69
**URL:** https://github.com/llvm/llvm-project/issues/179109
**Status:** Closed
**Labels:** clangd, crash
**Closed Date:** 2026-02-06T07:51:10Z

## Body

I sent "incoming calls" request just for fun and my clangd crashed, so i looked at the logs and was able to reproduce the bug using:

```bash
#!/bin/bash
INIT='{"jsonrpc":"2.0","id":0,"method":"initialize","params":{"processId":123,"rootUri":"file:///tmp","capabilities":{},"trace":"off"}}'
CRASH='{"jsonrpc":"2.0","id":1,"method":"callHierarchy/incomingCalls","params":[]}'

printf "Content-Length: ${#INIT}\r\n\r\n$INITContent-Length: ${#CRASH}\r\n\r\n$CRASH" | clangd
```

Result of this command is SEGFAULT, to be more precise - Address boundary error.

I tested it on both `21.1.6` version and on `23.0.0git (https://github.com/llvm/llvm-project e5d235831dfb07b6e767812af7f9d3daa5896f78)`. So it looks like bug was around for a while.

Here's the full trace from neovim:
<details>
  <summary>Details</summary>
I[16:30:12.593] clangd version 23.0.0git (https://github.com/llvm/llvm-project e5d235831dfb07b6e767812af7f9d3daa5896f78)
I[16:30:12.593] Features: linux+grpc
I[16:30:12.593] PID: 664540
I[16:30:12.593] Working directory: /home/progamers/CLionProjects/path_finder
I[16:30:12.593] argv[0]: clangd
I[16:30:12.593] Starting LSP over stdin/stdout
I[16:30:12.593] <-- initialize(0)
I[16:30:12.594] --> reply:initialize(0) 0 ms
Content-Length: 2067

{"id":0,"jsonrpc":"2.0","result":{"capabilities":{"astProvider":true,"callHierarchyProvider":true,"clangdInlayHintsProvider":true,"codeActionProvider":true,"compilationDatabase":{"automaticReload":true},"completionProvider":{"resolveProvider":false,"t
riggerCharacters":[".","<",">",":","\"","/","*"]},"declarationProvider":true,"definitionProvider":true,"documentFormattingProvider":true,"documentHighlightProvider":true,"documentLinkProvider":{"resolveProvider":false},"documentOnTypeFormattingProvide
r":{"firstTriggerCharacter":"\n","moreTriggerCharacter":[]},"documentRangeFormattingProvider":{"rangesSupport":true},"documentSymbolProvider":true,"executeCommandProvider":{"commands":["clangd.applyFix","clangd.applyRename","clangd.applyTweak"]},"fold
ingRangeProvider":true,"hoverProvider":true,"implementationProvider":true,"inactiveRegionsProvider":true,"inlayHintProvider":true,"memoryUsageProvider":true,"referencesProvider":true,"renameProvider":true,"selectionRangeProvider":true,"semanticTokensP
rovider":{"full":{"delta":true},"legend":{"tokenModifiers":["declaration","definition","deprecated","deduced","readonly","static","abstract","virtual","dependentName","defaultLibrary","usedAsMutableReference","usedAsMutablePointer","constructorOrDestr
uctor","userDefined","functionScope","classScope","fileScope","globalScope"],"tokenTypes":["variable","variable","parameter","function","method","function","property","variable","class","interface","enum","enumMember","type","type","unknown","namespac
e","typeParameter","concept","type","macro","modifier","operator","bracket","label","comment"]},"range":false},"signatureHelpProvider":{"triggerCharacters":["(",")","{","}","<",">",","]},"standardTypeHierarchyProvider":true,"textDocumentSync":{"change
":2,"openClose":true,"save":true},"typeDefinitionProvider":true,"typeHierarchyProvider":true,"workspaceSymbolProvider":true},"serverInfo":{"name":"clangd","version":"clangd version 23.0.0git (https://github.com/llvm/llvm-project e5d235831dfb07b6e76781
2af7f9d3daa5896f78) linux+grpc x86_64-unknown-linux-gnu"}}}I[16:30:12.594] <-- callHierarchy/incomingCalls(1)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: clangd
 #0 0x0000000000511167 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:842:13
 #1 0x000000000050ebb5 llvm::sys::RunSignalHandlers() /__w/clangd/clangd/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #2 0x00000000005120ca SignalHandler(int, siginfo_t*, void*) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:38
 #3 0x00007f9c522724d0 (/usr/lib/libc.so.6+0x3e4d0)
 #4 0x0000000002ae384e getNumBuckets /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/DenseMap.h:823:43
 #5 0x0000000002ae384e getNumBuckets /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/DenseMap.h:551:53
 #6 0x0000000002ae384e llvm::detail::DenseMapPair<llvm::json::ObjectKey, llvm::json::Value> const* llvm::DenseMapBase<llvm::DenseMap<llvm::json::ObjectKey, llvm::json::Value, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::
json::ObjectKey, llvm::json::Value>>, llvm::json::ObjectKey, llvm::json::Value, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::json::ObjectKey, llvm::json::Value>>::doFind<llvm::StringRef>(llvm::StringRef const&) const /__
w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/DenseMap.h:619:33
 #7 0x0000000002adfe79 find_as<llvm::StringRef> /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/DenseMap.h:0:0
 #8 0x0000000002adfe79 find /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/JSON.h:137:53
 #9 0x0000000002adfe79 llvm::json::Object::get(llvm::StringRef) const /__w/clangd/clangd/llvm-project/llvm/lib/Support/JSON.cpp:37:12
#10 0x00000000016a1e0e map<clang::clangd::CallHierarchyItem> /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/JSON.h:876:22
#11 0x00000000016a1e0e clang::clangd::fromJSON(llvm::json::Value const&, clang::clangd::CallHierarchyIncomingCallsParams&, llvm::json::Path) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/Protocol.cpp:1514:12
#12 0x000000000156bcf4 llvm::Expected<clang::clangd::CallHierarchyIncomingCallsParams> clang::clangd::LSPBinder::parse<clang::clangd::CallHierarchyIncomingCallsParams>(llvm::json::Value const&, llvm::StringRef, llvm::StringRef) /__w/clangd/clangd/llvm
-project/clang-tools-extra/clangd/LSPBinder.h:115:7
#13 0x000000000156b865 operator bool /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:578:13
#14 0x000000000156b865 void clang::clangd::LSPBinder::method<clang::clangd::CallHierarchyIncomingCallsParams, std::vector<clang::clangd::CallHierarchyIncomingCall, std::allocator<clang::clangd::CallHierarchyIncomingCall>>, clang::clangd::ClangdLSPServ
er>(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::CallHierarchyIncomingCallsParams const&, llvm::unique_function<void (llvm::Expected<std::vector<clang::clangd::CallHierarchyIncomingCall,
 std::allocator<clang::clangd::CallHierarchyIncomingCall>>>)>))::'lambda'(llvm::json::Value, llvm::unique_function<void (llvm::Expected<llvm::json::Value>)>)::operator()(llvm::json::Value, llvm::unique_function<void (llvm::Expected<llvm::json::Value>)
>) const /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/LSPBinder.h:139:10
#15 0x000000000156b7d9 asInt /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:41:5
#16 0x000000000156b7d9 operator long /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:45:48
#17 0x000000000156b7d9 getPointer /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:94:58
#18 0x000000000156b7d9 ~UniqueFunctionBase /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:274:32
#19 0x000000000156b7d9 void llvm::detail::UniqueFunctionBase<void, llvm::json::Value, llvm::unique_function<void (llvm::Expected<llvm::json::Value>)>>::CallImpl<void clang::clangd::LSPBinder::method<clang::clangd::CallHierarchyIncomingCallsParams, std
::vector<clang::clangd::CallHierarchyIncomingCall, std::allocator<clang::clangd::CallHierarchyIncomingCall>>, clang::clangd::ClangdLSPServer>(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd:
:CallHierarchyIncomingCallsParams const&, llvm::unique_function<void (llvm::Expected<std::vector<clang::clangd::CallHierarchyIncomingCall, std::allocator<clang::clangd::CallHierarchyIncomingCall>>>)>))::'lambda'(llvm::json::Value, llvm::unique_functio
n<void (llvm::Expected<llvm::json::Value>)>)>(void*, llvm::json::Value&, llvm::unique_function<void (llvm::Expected<llvm::json::Value>)>&) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:212:5
#20 0x0000000001542f84 asInt /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:41:5
#21 0x0000000001542f84 operator long /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:45:48
#22 0x0000000001542f84 getPointer /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:94:58
#23 0x0000000001542f84 ~UniqueFunctionBase /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:274:32
#24 0x0000000001542f84 clang::clangd::ClangdLSPServer::MessageHandler::onCall(llvm::StringRef, llvm::json::Value, llvm::json::Value) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/ClangdLSPServer.cpp:243:7
#25 0x00000000016851cd handleMessage /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/JSONTransport.cpp:0:20
#26 0x00000000016851cd clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/JSONTransport.cpp:119:16
#27 0x000000000153d241 operator bool /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:242:21
#28 0x000000000153d241 clang::clangd::ClangdLSPServer::run() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/ClangdLSPServer.cpp:1758:12
#29 0x00000000014bd20f clang::clangd::clangdMain(int, char**) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/tool/ClangdMain.cpp:1067:18
#30 0x00007f9c5225b635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#31 0x00007f9c5225b6e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#32 0x00007f9c5225b6e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#33 0x0000000000431c5e _start (/home/progamers/.local/share/nvim/mason/packages/clangd/clangd_snapshot_20260125/bin/clangd+0x431c5e)
Signalled while processing message:
{"jsonrpc":"2.0","id":1,"method":"callHierarchy/incomingCalls","params":[]}
fish: Job 1, 'printf "Content-Length: %d\r\n\…' terminated by signal SIGSEGV (Address boundary error)
</details>


I am using:
- ldd (GNU libc) 2.42
- Arch Linux 6.18.5-arch1-1

I hope this helps, and that the bug will be fixed, Thanks!

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (NeKon69)

<details>
I sent "incoming calls" request just for fun and my clangd crashed, so i looked at the logs and was able to reproduce the bug using:

```bash
#!/bin/bash
INIT='{"jsonrpc":"2.0","id":0,"method":"initialize","params":{"processId":123,"rootUri":"file:///tmp","capabilities":{},"trace":"off"}}'
CRASH='{"jsonrpc":"2.0","id":1,"method":"callHierarchy/incomingCalls","params":[]}'

printf "Content-Length: ${#INIT}\r\n\r\n$INITContent-Length: ${#CRASH}\r\n\r\n$CRASH" | clangd
```

Result of this command is SEGFAULT, to be more precise - Address boundary error.

I tested it on both `21.1.6` version and on `23.0.0git (https://github.com/llvm/llvm-project e5d235831dfb07b6e767812af7f9d3daa5896f78)`. So it looks like bug was around for a while.

Here's the full trace from neovim:
&lt;details&gt;
  &lt;summary&gt;Details&lt;/summary&gt;
I[16:30:12.593] clangd version 23.0.0git (https://github.com/llvm/llvm-project e5d235831dfb07b6e767812af7f9d3daa5896f78)
I[16:30:12.593] Features: linux+grpc
I[16:30:12.593] PID: 664540
I[16:30:12.593] Working directory: /home/progamers/CLionProjects/path_finder
I[16:30:12.593] argv[0]: clangd
I[16:30:12.593] Starting LSP over stdin/stdout
I[16:30:12.593] &lt;-- initialize(0)
I[16:30:12.594] --&gt; reply:initialize(0) 0 ms
Content-Length: 2067

{"id":0,"jsonrpc":"2.0","result":{"capabilities":{"astProvider":true,"callHierarchyProvider":true,"clangdInlayHintsProvider":true,"codeActionProvider":true,"compilationDatabase":{"automaticReload":true},"completionProvider":{"resolveProvider":false,"t
riggerCharacters":[".","&lt;","&gt;",":","\"","/","*"]},"declarationProvider":true,"definitionProvider":true,"documentFormattingProvider":true,"documentHighlightProvider":true,"documentLinkProvider":{"resolveProvider":false},"documentOnTypeFormattingProvide
r":{"firstTriggerCharacter":"\n","moreTriggerCharacter":[]},"documentRangeFormattingProvider":{"rangesSupport":true},"documentSymbolProvider":true,"executeCommandProvider":{"commands":["clangd.applyFix","clangd.applyRename","clangd.applyTweak"]},"fold
ingRangeProvider":true,"hoverProvider":true,"implementationProvider":true,"inactiveRegionsProvider":true,"inlayHintProvider":true,"memoryUsageProvider":true,"referencesProvider":true,"renameProvider":true,"selectionRangeProvider":true,"semanticTokensP
rovider":{"full":{"delta":true},"legend":{"tokenModifiers":["declaration","definition","deprecated","deduced","readonly","static","abstract","virtual","dependentName","defaultLibrary","usedAsMutableReference","usedAsMutablePointer","constructorOrDestr
uctor","userDefined","functionScope","classScope","fileScope","globalScope"],"tokenTypes":["variable","variable","parameter","function","method","function","property","variable","class","interface","enum","enumMember","type","type","unknown","namespac
e","typeParameter","concept","type","macro","modifier","operator","bracket","label","comment"]},"range":false},"signatureHelpProvider":{"triggerCharacters":["(",")","{","}","&lt;","&gt;",","]},"standardTypeHierarchyProvider":true,"textDocumentSync":{"change
":2,"openClose":true,"save":true},"typeDefinitionProvider":true,"typeHierarchyProvider":true,"workspaceSymbolProvider":true},"serverInfo":{"name":"clangd","version":"clangd version 23.0.0git (https://github.com/llvm/llvm-project e5d235831dfb07b6e76781
2af7f9d3daa5896f78) linux+grpc x86_64-unknown-linux-gnu"}}}I[16:30:12.594] &lt;-- callHierarchy/incomingCalls(1)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: clangd
 #<!-- -->0 0x0000000000511167 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:842:13
 #<!-- -->1 0x000000000050ebb5 llvm::sys::RunSignalHandlers() /__w/clangd/clangd/llvm-project/llvm/lib/Support/Signals.cpp:109:18
 #<!-- -->2 0x00000000005120ca SignalHandler(int, siginfo_t*, void*) /__w/clangd/clangd/llvm-project/llvm/lib/Support/Unix/Signals.inc:429:38
 #<!-- -->3 0x00007f9c522724d0 (/usr/lib/libc.so.6+0x3e4d0)
 #<!-- -->4 0x0000000002ae384e getNumBuckets /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/DenseMap.h:823:43
 #<!-- -->5 0x0000000002ae384e getNumBuckets /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/DenseMap.h:551:53
 #<!-- -->6 0x0000000002ae384e llvm::detail::DenseMapPair&lt;llvm::json::ObjectKey, llvm::json::Value&gt; const* llvm::DenseMapBase&lt;llvm::DenseMap&lt;llvm::json::ObjectKey, llvm::json::Value, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::
json::ObjectKey, llvm::json::Value&gt;&gt;, llvm::json::ObjectKey, llvm::json::Value, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::json::ObjectKey, llvm::json::Value&gt;&gt;::doFind&lt;llvm::StringRef&gt;(llvm::StringRef const&amp;) const /__
w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/DenseMap.h:619:33
 #<!-- -->7 0x0000000002adfe79 find_as&lt;llvm::StringRef&gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/DenseMap.h:0:0
 #<!-- -->8 0x0000000002adfe79 find /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/JSON.h:137:53
 #<!-- -->9 0x0000000002adfe79 llvm::json::Object::get(llvm::StringRef) const /__w/clangd/clangd/llvm-project/llvm/lib/Support/JSON.cpp:37:12
#<!-- -->10 0x00000000016a1e0e map&lt;clang::clangd::CallHierarchyItem&gt; /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/JSON.h:876:22
#<!-- -->11 0x00000000016a1e0e clang::clangd::fromJSON(llvm::json::Value const&amp;, clang::clangd::CallHierarchyIncomingCallsParams&amp;, llvm::json::Path) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/Protocol.cpp:1514:12
#<!-- -->12 0x000000000156bcf4 llvm::Expected&lt;clang::clangd::CallHierarchyIncomingCallsParams&gt; clang::clangd::LSPBinder::parse&lt;clang::clangd::CallHierarchyIncomingCallsParams&gt;(llvm::json::Value const&amp;, llvm::StringRef, llvm::StringRef) /__w/clangd/clangd/llvm
-project/clang-tools-extra/clangd/LSPBinder.h:115:7
#<!-- -->13 0x000000000156b865 operator bool /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:578:13
#<!-- -->14 0x000000000156b865 void clang::clangd::LSPBinder::method&lt;clang::clangd::CallHierarchyIncomingCallsParams, std::vector&lt;clang::clangd::CallHierarchyIncomingCall, std::allocator&lt;clang::clangd::CallHierarchyIncomingCall&gt;&gt;, clang::clangd::ClangdLSPServ
er&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::CallHierarchyIncomingCallsParams const&amp;, llvm::unique_function&lt;void (llvm::Expected&lt;std::vector&lt;clang::clangd::CallHierarchyIncomingCall,
 std::allocator&lt;clang::clangd::CallHierarchyIncomingCall&gt;&gt;&gt;)&gt;))::'lambda'(llvm::json::Value, llvm::unique_function&lt;void (llvm::Expected&lt;llvm::json::Value&gt;)&gt;)::operator()(llvm::json::Value, llvm::unique_function&lt;void (llvm::Expected&lt;llvm::json::Value&gt;)
&gt;) const /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/LSPBinder.h:139:10
#<!-- -->15 0x000000000156b7d9 asInt /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:41:5
#<!-- -->16 0x000000000156b7d9 operator long /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:45:48
#<!-- -->17 0x000000000156b7d9 getPointer /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:94:58
#<!-- -->18 0x000000000156b7d9 ~UniqueFunctionBase /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:274:32
#<!-- -->19 0x000000000156b7d9 void llvm::detail::UniqueFunctionBase&lt;void, llvm::json::Value, llvm::unique_function&lt;void (llvm::Expected&lt;llvm::json::Value&gt;)&gt;&gt;::CallImpl&lt;void clang::clangd::LSPBinder::method&lt;clang::clangd::CallHierarchyIncomingCallsParams, std
::vector&lt;clang::clangd::CallHierarchyIncomingCall, std::allocator&lt;clang::clangd::CallHierarchyIncomingCall&gt;&gt;, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd:
:CallHierarchyIncomingCallsParams const&amp;, llvm::unique_function&lt;void (llvm::Expected&lt;std::vector&lt;clang::clangd::CallHierarchyIncomingCall, std::allocator&lt;clang::clangd::CallHierarchyIncomingCall&gt;&gt;&gt;)&gt;))::'lambda'(llvm::json::Value, llvm::unique_functio
n&lt;void (llvm::Expected&lt;llvm::json::Value&gt;)&gt;)&gt;(void*, llvm::json::Value&amp;, llvm::unique_function&lt;void (llvm::Expected&lt;llvm::json::Value&gt;)&gt;&amp;) /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:212:5
#<!-- -->20 0x0000000001542f84 asInt /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:41:5
#<!-- -->21 0x0000000001542f84 operator long /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:45:48
#<!-- -->22 0x0000000001542f84 getPointer /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/PointerIntPair.h:94:58
#<!-- -->23 0x0000000001542f84 ~UniqueFunctionBase /__w/clangd/clangd/llvm-project/llvm/include/llvm/ADT/FunctionExtras.h:274:32
#<!-- -->24 0x0000000001542f84 clang::clangd::ClangdLSPServer::MessageHandler::onCall(llvm::StringRef, llvm::json::Value, llvm::json::Value) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/ClangdLSPServer.cpp:243:7
#<!-- -->25 0x00000000016851cd handleMessage /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/JSONTransport.cpp:0:20
#<!-- -->26 0x00000000016851cd clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&amp;) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/JSONTransport.cpp:119:16
#<!-- -->27 0x000000000153d241 operator bool /__w/clangd/clangd/llvm-project/llvm/include/llvm/Support/Error.h:242:21
#<!-- -->28 0x000000000153d241 clang::clangd::ClangdLSPServer::run() /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/ClangdLSPServer.cpp:1758:12
#<!-- -->29 0x00000000014bd20f clang::clangd::clangdMain(int, char**) /__w/clangd/clangd/llvm-project/clang-tools-extra/clangd/tool/ClangdMain.cpp:1067:18
#<!-- -->30 0x00007f9c5225b635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->31 0x00007f9c5225b6e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->32 0x00007f9c5225b6e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->33 0x0000000000431c5e _start (/home/progamers/.local/share/nvim/mason/packages/clangd/clangd_snapshot_20260125/bin/clangd+0x431c5e)
Signalled while processing message:
{"jsonrpc":"2.0","id":1,"method":"callHierarchy/incomingCalls","params":[]}
fish: Job 1, 'printf "Content-Length: %d\r\n\…' terminated by signal SIGSEGV (Address boundary error)
&lt;/details&gt;


I am using:
- ldd (GNU libc) 2.42
- Arch Linux 6.18.5-arch1-1

I hope this helps, and that the bug will be fixed, Thanks!
</details>


---

### Comment 2 - NeKon69

Can i try to fix this issue myself? This seems like a generally easy fix and i would love to contribute to something as big as LLVM!

---

### Comment 3 - HighCommander4

> Can i try to fix this issue myself?

Sure!

---

### Comment 4 - NeKon69

Ok so it looks like i found where the bug is happening. I don't know how to properly handle it though. Is there like a manual or something about that stuff?

---

### Comment 5 - HighCommander4

There is no manual.

Feel free to share your findings and I may be able to suggest a next step.

---

### Comment 6 - NeKon69

Sorry for long wait. Fix was actually simple, yesterday i did something that wasn't really intended because i didn't understand how codebase works :) anyways, now that i fixed it, before making a PR i think i should run tests for clangd and maybe write my own (as stated in contributing guide). do you happen to know how to run them or where they are stored?

---

### Comment 7 - NeKon69

Alr i figured out how to run tests and make my own. Probably will make a PR now

---

