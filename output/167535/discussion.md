# [clangd] crash in VSCode on Windows

**Author:** smilediver
**URL:** https://github.com/llvm/llvm-project/issues/167535
**Status:** Closed
**Labels:** duplicate, clangd, clang:modules, crash
**Closed Date:** 2025-11-11T19:13:58Z

## Body

Clangd in VSCode on Windows crashes with a trivial C++ module file.

clangd version 21.1.5

Repro steps:

* Create an empty folder and the following files inside it:
* Create `main.cppm` file:
```cpp
module;

export module app;

extern "C++" int main() {
    return 0;
}
```
* Create `.vscode/settings.json` file:
```json
{
    "clangd.arguments": [
        "--experimental-modules-support",
        "--log=verbose",
    ],
}
```
* Open the folder as project in VSCode.
* Then open the `main.cppm` file, and clangd should crash at this point.

Output window log:
```
I[18:48:02.052] clangd version 21.1.5
I[18:48:02.053] Features: windows
I[18:48:02.053] PID: 25940
I[18:48:02.053] Working directory: d:\Projects\bugs\clangd-ccd_crash
I[18:48:02.053] argv[0]: c:\Program Files\LLVM\bin\clangd.exe
I[18:48:02.053] argv[1]: --experimental-modules-support
I[18:48:02.053] argv[2]: --log=verbose
V[18:48:02.057] User config file is C:\Users\smile\AppData\Local\clangd\config.yaml
I[18:48:02.057] Starting LSP over stdin/stdout
V[18:48:02.057] <<< {"id":0,"jsonrpc":"2.0","method":"initialize","params":{"capabilities":{"general":{"markdown":{"parser":"marked","version":"1.1.0"},"positionEncodings":["utf-16"],"regularExpressions":{"engine":"ECMAScript","version":"ES2020"},"staleRequestSupport":{"cancel":true,"retryOnContentModified":["textDocument/semanticTokens/full","textDocument/semanticTokens/range","textDocument/semanticTokens/full/delta"]}},"notebookDocument":{"synchronization":{"dynamicRegistration":true,"executionSummarySupport":true}},"textDocument":{"callHierarchy":{"dynamicRegistration":true},"codeAction":{"codeActionLiteralSupport":{"codeActionKind":{"valueSet":["","quickfix","refactor","refactor.extract","refactor.inline","refactor.rewrite","source","source.organizeImports"]}},"dataSupport":true,"disabledSupport":true,"dynamicRegistration":true,"honorsChangeAnnotations":true,"isPreferredSupport":true,"resolveSupport":{"properties":["edit"]}},"codeLens":{"dynamicRegistration":true},"colorProvider":{"dynamicRegistration":true},"completion":{"completionItem":{"commitCharactersSupport":true,"deprecatedSupport":true,"documentationFormat":["markdown","plaintext"],"insertReplaceSupport":true,"insertTextModeSupport":{"valueSet":[1,2]},"labelDetailsSupport":true,"preselectSupport":true,"resolveSupport":{"properties":["documentation","detail","additionalTextEdits"]},"snippetSupport":true,"tagSupport":{"valueSet":[1]}},"completionItemKind":{"valueSet":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25]},"completionList":{"itemDefaults":["commitCharacters","editRange","insertTextFormat","insertTextMode","data"]},"contextSupport":true,"dynamicRegistration":true,"editsNearCursor":true,"insertTextMode":2},"declaration":{"dynamicRegistration":true,"linkSupport":true},"definition":{"dynamicRegistration":true,"linkSupport":true},"diagnostic":{"dynamicRegistration":true,"relatedDocumentSupport":false},"documentHighlight":{"dynamicRegistration":true},"documentLink":{"dynamicRegistration":true,"tooltipSupport":true},"documentSymbol":{"dynamicRegistration":true,"hierarchicalDocumentSymbolSupport":true,"labelSupport":true,"symbolKind":{"valueSet":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26]},"tagSupport":{"valueSet":[1]}},"foldingRange":{"dynamicRegistration":true,"foldingRange":{"collapsedText":false},"foldingRangeKind":{"valueSet":["comment","imports","region"]},"lineFoldingOnly":true,"rangeLimit":5000},"formatting":{"dynamicRegistration":true},"hover":{"contentFormat":["markdown","plaintext"],"dynamicRegistration":true},"implementation":{"dynamicRegistration":true,"linkSupport":true},"inactiveRegionsCapabilities":{"inactiveRegions":true},"inlayHint":{"dynamicRegistration":true,"resolveSupport":{"properties":["tooltip","textEdits","label.tooltip","label.location","label.command"]}},"inlineValue":{"dynamicRegistration":true},"linkedEditingRange":{"dynamicRegistration":true},"onTypeFormatting":{"dynamicRegistration":true},"publishDiagnostics":{"codeDescriptionSupport":true,"dataSupport":true,"relatedInformation":true,"tagSupport":{"valueSet":[1,2]},"versionSupport":false},"rangeFormatting":{"dynamicRegistration":true,"rangesSupport":true},"references":{"dynamicRegistration":true},"rename":{"dynamicRegistration":true,"honorsChangeAnnotations":true,"prepareSupport":true,"prepareSupportDefaultBehavior":1},"selectionRange":{"dynamicRegistration":true},"semanticTokens":{"augmentsSyntaxTokens":true,"dynamicRegistration":true,"formats":["relative"],"multilineTokenSupport":false,"overlappingTokenSupport":false,"requests":{"full":{"delta":true},"range":true},"serverCancelSupport":true,"tokenModifiers":["declaration","definition","readonly","static","deprecated","abstract","async","modification","documentation","defaultLibrary"],"tokenTypes":["namespace","type","class","enum","interface","struct","typeParameter","parameter","variable","property","enumMember","event","function","method","macro","keyword","modifier","comment","string","number","regexp","operator","decorator"]},"signatureHelp":{"contextSupport":true,"dynamicRegistration":true,"signatureInformation":{"activeParameterSupport":true,"documentationFormat":["markdown","plaintext"],"parameterInformation":{"labelOffsetSupport":true}}},"synchronization":{"didSave":true,"dynamicRegistration":true,"willSave":true,"willSaveWaitUntil":true},"typeDefinition":{"dynamicRegistration":true,"linkSupport":true},"typeHierarchy":{"dynamicRegistration":true}},"window":{"showDocument":{"support":true},"showMessage":{"messageActionItem":{"additionalPropertiesSupport":true}},"workDoneProgress":true},"workspace":{"applyEdit":true,"codeLens":{"refreshSupport":true},"configuration":true,"diagnostics":{"refreshSupport":true},"didChangeConfiguration":{"dynamicRegistration":true},"didChangeWatchedFiles":{"dynamicRegistration":true,"relativePatternSupport":true},"executeCommand":{"dynamicRegistration":true},"fileOperations":{"didCreate":true,"didDelete":true,"didRename":true,"dynamicRegistration":true,"willCreate":true,"willDelete":true,"willRename":true},"foldingRange":{"refreshSupport":true},"inlayHint":{"refreshSupport":true},"inlineValue":{"refreshSupport":true},"semanticTokens":{"refreshSupport":true},"symbol":{"dynamicRegistration":true,"resolveSupport":{"properties":["location.range"]},"symbolKind":{"valueSet":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26]},"tagSupport":{"valueSet":[1]}},"workspaceEdit":{"changeAnnotationSupport":{"groupsOnLabel":true},"documentChanges":true,"failureHandling":"textOnlyTransactional","normalizesLineEndings":true,"resourceOperations":["create","rename","delete"]},"workspaceFolders":true}},"clientInfo":{"name":"Visual Studio Code","version":"1.105.1"},"initializationOptions":{"clangdFileStatus":true,"fallbackFlags":[]},"locale":"en","processId":60656,"rootPath":"d:\\Projects\\bugs\\clangd-ccd_crash","rootUri":"file:///d%3A/Projects/bugs/clangd-ccd_crash","trace":"off","workspaceFolders":[{"name":"clangd-ccd_crash","uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash"}]}}

I[18:48:02.057] <-- initialize(0)
I[18:48:02.059] --> reply:initialize(0) 1 ms
V[18:48:02.059] >>> {"id":0,"jsonrpc":"2.0","result":{"capabilities":{"astProvider":true,"callHierarchyProvider":true,"clangdInlayHintsProvider":true,"codeActionProvider":{"codeActionKinds":["quickfix","refactor","info"]},"compilationDatabase":{"automaticReload":true},"completionProvider":{"resolveProvider":false,"triggerCharacters":[".","<",">",":","\"","/","*"]},"declarationProvider":true,"definitionProvider":true,"documentFormattingProvider":true,"documentHighlightProvider":true,"documentLinkProvider":{"resolveProvider":false},"documentOnTypeFormattingProvider":{"firstTriggerCharacter":"\n","moreTriggerCharacter":[]},"documentRangeFormattingProvider":{"rangesSupport":true},"documentSymbolProvider":true,"executeCommandProvider":{"commands":["clangd.applyFix","clangd.applyRename","clangd.applyTweak"]},"foldingRangeProvider":true,"hoverProvider":true,"implementationProvider":true,"inactiveRegionsProvider":true,"inlayHintProvider":true,"memoryUsageProvider":true,"positionEncoding":"utf-16","referencesProvider":true,"renameProvider":{"prepareProvider":true},"selectionRangeProvider":true,"semanticTokensProvider":{"full":{"delta":true},"legend":{"tokenModifiers":["declaration","definition","deprecated","deduced","readonly","static","abstract","virtual","dependentName","defaultLibrary","usedAsMutableReference","usedAsMutablePointer","constructorOrDestructor","userDefined","functionScope","classScope","fileScope","globalScope"],"tokenTypes":["variable","variable","parameter","function","method","function","property","variable","class","interface","enum","enumMember","type","type","unknown","namespace","typeParameter","concept","type","macro","modifier","operator","bracket","label","comment"]},"range":false},"signatureHelpProvider":{"triggerCharacters":["(",")","{","}","<",">",","]},"standardTypeHierarchyProvider":true,"textDocumentSync":{"change":2,"openClose":true,"save":true},"typeDefinitionProvider":true,"typeHierarchyProvider":true,"workspaceSymbolProvider":true},"offsetEncoding":"utf-16","serverInfo":{"name":"clangd","version":"clangd version 21.1.5 windows x86_64-pc-windows-msvc"}}}

V[18:48:02.063] <<< {"jsonrpc":"2.0","method":"initialized","params":{}}

I[18:48:02.063] <-- initialized
V[18:48:02.065] <<< {"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"languageId":"cpp","text":"module;\r\n\r\nexport module app;\r\n\r\nextern \"C++\" int main() {\r\n    return 0;\r\n}\r\n","uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash/main.cppm","version":1}}}

I[18:48:02.065] <-- textDocument/didOpen
I[18:48:02.066] Failed to find compilation database for d:\Projects\bugs\clangd-ccd_crash\main.cppm
I[18:48:02.066] ASTWorker building file d:\Projects\bugs\clangd-ccd_crash\main.cppm version 1 with command clangd fallback
[d:\Projects\bugs\clangd-ccd_crash]
"C:\\Program Files\\LLVM\\bin\\clang" "-resource-dir=C:\\Program Files\\LLVM\\lib\\clang\\21" -- "d:\\Projects\\bugs\\clangd-ccd_crash\\main.cppm"
V[18:48:02.067] <<< {"id":1,"jsonrpc":"2.0","method":"textDocument/documentSymbol","params":{"textDocument":{"uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash/main.cppm"}}}

I[18:48:02.067] <-- textDocument/documentSymbol(1)
V[18:48:02.068] Driver produced command: cc1 -cc1 -triple x86_64-pc-windows-msvc19.44.35219 -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.cppm -mrelocation-model pic -pic-level 2 -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -fms-volatile -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic "-fdebug-compilation-dir=d:\\Projects\\bugs\\clangd-ccd_crash" "-fcoverage-compilation-dir=d:\\Projects\\bugs\\clangd-ccd_crash" -resource-dir "C:\\Program Files\\LLVM\\lib\\clang\\21" -internal-isystem "C:\\Program Files\\LLVM\\lib\\clang\\21\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.44.35207\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\VS\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.26100.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\cppwinrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.7.1\\include\\um" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.44.35207\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\VS\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.26100.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\cppwinrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.7.1\\include\\um" -fdeprecated-macro -ferror-limit 19 -fno-use-cxa-atexit -fms-extensions -fms-compatibility -fms-compatibility-version=19.44.35219 -std=c++14 -fskip-odr-check-in-gmf -fdelayed-template-parsing -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -x c++ "d:\\Projects\\bugs\\clangd-ccd_crash\\main.cppm"
I[18:48:02.068] --> textDocument/clangd.fileStatus
V[18:48:02.068] >>> {"jsonrpc":"2.0","method":"textDocument/clangd.fileStatus","params":{"state":"parsing includes, running Update","uri":"file:///d:/Projects/bugs/clangd-ccd_crash/main.cppm"}}

V[18:48:02.068] Building first preamble for d:\Projects\bugs\clangd-ccd_crash\main.cppm version 1
I[18:48:02.077] Built preamble of size 270900 for file d:\Projects\bugs\clangd-ccd_crash\main.cppm version 1 in 0.01 seconds
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Signalled while building preamble
  Filename: main.cppm
  Directory: d:\Projects\bugs\clangd-ccd_crash
  Command Line: C:\Program Files\LLVM\bin\clang -resource-dir=C:\Program Files\LLVM\lib\clang\21 -- d:\Projects\bugs\clangd-ccd_crash\main.cppm
  Version: 1
Exception Code: 0xC0000005
V[18:48:02.087] <<< {"id":2,"jsonrpc":"2.0","method":"textDocument/documentLink","params":{"textDocument":{"uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash/main.cppm"}}}

I[18:48:02.087] <-- textDocument/documentLink(2)
[Error - 6:48:02 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
[Error - 6:48:02 PM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 6:48:02 PM] Request textDocument/documentLink failed.
[object Object]
[Error - 6:48:02 PM] Server process exited with code 3221225477.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: None (smilediver)

<details>
Clangd in VSCode on Windows crashes with a trivial C++ module file.

clangd version 21.1.5

Repro steps:

* Create an empty folder and the following files inside it:
* Create `main.cppm` file:
```cpp
module;

export module app;

extern "C++" int main() {
    return 0;
}
```
* Create `.vscode/settings.json` file:
```json
{
    "clangd.arguments": [
        "--experimental-modules-support",
        "--log=verbose",
    ],
}
```
* Open the folder as project in VSCode.
* Then open the `main.cppm` file, and clangd should crash at this point.

Output window log:
```
I[18:48:02.052] clangd version 21.1.5
I[18:48:02.053] Features: windows
I[18:48:02.053] PID: 25940
I[18:48:02.053] Working directory: d:\Projects\bugs\clangd-ccd_crash
I[18:48:02.053] argv[0]: c:\Program Files\LLVM\bin\clangd.exe
I[18:48:02.053] argv[1]: --experimental-modules-support
I[18:48:02.053] argv[2]: --log=verbose
V[18:48:02.057] User config file is C:\Users\smile\AppData\Local\clangd\config.yaml
I[18:48:02.057] Starting LSP over stdin/stdout
V[18:48:02.057] &lt;&lt;&lt; {"id":0,"jsonrpc":"2.0","method":"initialize","params":{"capabilities":{"general":{"markdown":{"parser":"marked","version":"1.1.0"},"positionEncodings":["utf-16"],"regularExpressions":{"engine":"ECMAScript","version":"ES2020"},"staleRequestSupport":{"cancel":true,"retryOnContentModified":["textDocument/semanticTokens/full","textDocument/semanticTokens/range","textDocument/semanticTokens/full/delta"]}},"notebookDocument":{"synchronization":{"dynamicRegistration":true,"executionSummarySupport":true}},"textDocument":{"callHierarchy":{"dynamicRegistration":true},"codeAction":{"codeActionLiteralSupport":{"codeActionKind":{"valueSet":["","quickfix","refactor","refactor.extract","refactor.inline","refactor.rewrite","source","source.organizeImports"]}},"dataSupport":true,"disabledSupport":true,"dynamicRegistration":true,"honorsChangeAnnotations":true,"isPreferredSupport":true,"resolveSupport":{"properties":["edit"]}},"codeLens":{"dynamicRegistration":true},"colorProvider":{"dynamicRegistration":true},"completion":{"completionItem":{"commitCharactersSupport":true,"deprecatedSupport":true,"documentationFormat":["markdown","plaintext"],"insertReplaceSupport":true,"insertTextModeSupport":{"valueSet":[1,2]},"labelDetailsSupport":true,"preselectSupport":true,"resolveSupport":{"properties":["documentation","detail","additionalTextEdits"]},"snippetSupport":true,"tagSupport":{"valueSet":[1]}},"completionItemKind":{"valueSet":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25]},"completionList":{"itemDefaults":["commitCharacters","editRange","insertTextFormat","insertTextMode","data"]},"contextSupport":true,"dynamicRegistration":true,"editsNearCursor":true,"insertTextMode":2},"declaration":{"dynamicRegistration":true,"linkSupport":true},"definition":{"dynamicRegistration":true,"linkSupport":true},"diagnostic":{"dynamicRegistration":true,"relatedDocumentSupport":false},"documentHighlight":{"dynamicRegistration":true},"documentLink":{"dynamicRegistration":true,"tooltipSupport":true},"documentSymbol":{"dynamicRegistration":true,"hierarchicalDocumentSymbolSupport":true,"labelSupport":true,"symbolKind":{"valueSet":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26]},"tagSupport":{"valueSet":[1]}},"foldingRange":{"dynamicRegistration":true,"foldingRange":{"collapsedText":false},"foldingRangeKind":{"valueSet":["comment","imports","region"]},"lineFoldingOnly":true,"rangeLimit":5000},"formatting":{"dynamicRegistration":true},"hover":{"contentFormat":["markdown","plaintext"],"dynamicRegistration":true},"implementation":{"dynamicRegistration":true,"linkSupport":true},"inactiveRegionsCapabilities":{"inactiveRegions":true},"inlayHint":{"dynamicRegistration":true,"resolveSupport":{"properties":["tooltip","textEdits","label.tooltip","label.location","label.command"]}},"inlineValue":{"dynamicRegistration":true},"linkedEditingRange":{"dynamicRegistration":true},"onTypeFormatting":{"dynamicRegistration":true},"publishDiagnostics":{"codeDescriptionSupport":true,"dataSupport":true,"relatedInformation":true,"tagSupport":{"valueSet":[1,2]},"versionSupport":false},"rangeFormatting":{"dynamicRegistration":true,"rangesSupport":true},"references":{"dynamicRegistration":true},"rename":{"dynamicRegistration":true,"honorsChangeAnnotations":true,"prepareSupport":true,"prepareSupportDefaultBehavior":1},"selectionRange":{"dynamicRegistration":true},"semanticTokens":{"augmentsSyntaxTokens":true,"dynamicRegistration":true,"formats":["relative"],"multilineTokenSupport":false,"overlappingTokenSupport":false,"requests":{"full":{"delta":true},"range":true},"serverCancelSupport":true,"tokenModifiers":["declaration","definition","readonly","static","deprecated","abstract","async","modification","documentation","defaultLibrary"],"tokenTypes":["namespace","type","class","enum","interface","struct","typeParameter","parameter","variable","property","enumMember","event","function","method","macro","keyword","modifier","comment","string","number","regexp","operator","decorator"]},"signatureHelp":{"contextSupport":true,"dynamicRegistration":true,"signatureInformation":{"activeParameterSupport":true,"documentationFormat":["markdown","plaintext"],"parameterInformation":{"labelOffsetSupport":true}}},"synchronization":{"didSave":true,"dynamicRegistration":true,"willSave":true,"willSaveWaitUntil":true},"typeDefinition":{"dynamicRegistration":true,"linkSupport":true},"typeHierarchy":{"dynamicRegistration":true}},"window":{"showDocument":{"support":true},"showMessage":{"messageActionItem":{"additionalPropertiesSupport":true}},"workDoneProgress":true},"workspace":{"applyEdit":true,"codeLens":{"refreshSupport":true},"configuration":true,"diagnostics":{"refreshSupport":true},"didChangeConfiguration":{"dynamicRegistration":true},"didChangeWatchedFiles":{"dynamicRegistration":true,"relativePatternSupport":true},"executeCommand":{"dynamicRegistration":true},"fileOperations":{"didCreate":true,"didDelete":true,"didRename":true,"dynamicRegistration":true,"willCreate":true,"willDelete":true,"willRename":true},"foldingRange":{"refreshSupport":true},"inlayHint":{"refreshSupport":true},"inlineValue":{"refreshSupport":true},"semanticTokens":{"refreshSupport":true},"symbol":{"dynamicRegistration":true,"resolveSupport":{"properties":["location.range"]},"symbolKind":{"valueSet":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26]},"tagSupport":{"valueSet":[1]}},"workspaceEdit":{"changeAnnotationSupport":{"groupsOnLabel":true},"documentChanges":true,"failureHandling":"textOnlyTransactional","normalizesLineEndings":true,"resourceOperations":["create","rename","delete"]},"workspaceFolders":true}},"clientInfo":{"name":"Visual Studio Code","version":"1.105.1"},"initializationOptions":{"clangdFileStatus":true,"fallbackFlags":[]},"locale":"en","processId":60656,"rootPath":"d:\\Projects\\bugs\\clangd-ccd_crash","rootUri":"file:///d%3A/Projects/bugs/clangd-ccd_crash","trace":"off","workspaceFolders":[{"name":"clangd-ccd_crash","uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash"}]}}

I[18:48:02.057] &lt;-- initialize(0)
I[18:48:02.059] --&gt; reply:initialize(0) 1 ms
V[18:48:02.059] &gt;&gt;&gt; {"id":0,"jsonrpc":"2.0","result":{"capabilities":{"astProvider":true,"callHierarchyProvider":true,"clangdInlayHintsProvider":true,"codeActionProvider":{"codeActionKinds":["quickfix","refactor","info"]},"compilationDatabase":{"automaticReload":true},"completionProvider":{"resolveProvider":false,"triggerCharacters":[".","&lt;","&gt;",":","\"","/","*"]},"declarationProvider":true,"definitionProvider":true,"documentFormattingProvider":true,"documentHighlightProvider":true,"documentLinkProvider":{"resolveProvider":false},"documentOnTypeFormattingProvider":{"firstTriggerCharacter":"\n","moreTriggerCharacter":[]},"documentRangeFormattingProvider":{"rangesSupport":true},"documentSymbolProvider":true,"executeCommandProvider":{"commands":["clangd.applyFix","clangd.applyRename","clangd.applyTweak"]},"foldingRangeProvider":true,"hoverProvider":true,"implementationProvider":true,"inactiveRegionsProvider":true,"inlayHintProvider":true,"memoryUsageProvider":true,"positionEncoding":"utf-16","referencesProvider":true,"renameProvider":{"prepareProvider":true},"selectionRangeProvider":true,"semanticTokensProvider":{"full":{"delta":true},"legend":{"tokenModifiers":["declaration","definition","deprecated","deduced","readonly","static","abstract","virtual","dependentName","defaultLibrary","usedAsMutableReference","usedAsMutablePointer","constructorOrDestructor","userDefined","functionScope","classScope","fileScope","globalScope"],"tokenTypes":["variable","variable","parameter","function","method","function","property","variable","class","interface","enum","enumMember","type","type","unknown","namespace","typeParameter","concept","type","macro","modifier","operator","bracket","label","comment"]},"range":false},"signatureHelpProvider":{"triggerCharacters":["(",")","{","}","&lt;","&gt;",","]},"standardTypeHierarchyProvider":true,"textDocumentSync":{"change":2,"openClose":true,"save":true},"typeDefinitionProvider":true,"typeHierarchyProvider":true,"workspaceSymbolProvider":true},"offsetEncoding":"utf-16","serverInfo":{"name":"clangd","version":"clangd version 21.1.5 windows x86_64-pc-windows-msvc"}}}

V[18:48:02.063] &lt;&lt;&lt; {"jsonrpc":"2.0","method":"initialized","params":{}}

I[18:48:02.063] &lt;-- initialized
V[18:48:02.065] &lt;&lt;&lt; {"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"languageId":"cpp","text":"module;\r\n\r\nexport module app;\r\n\r\nextern \"C++\" int main() {\r\n    return 0;\r\n}\r\n","uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash/main.cppm","version":1}}}

I[18:48:02.065] &lt;-- textDocument/didOpen
I[18:48:02.066] Failed to find compilation database for d:\Projects\bugs\clangd-ccd_crash\main.cppm
I[18:48:02.066] ASTWorker building file d:\Projects\bugs\clangd-ccd_crash\main.cppm version 1 with command clangd fallback
[d:\Projects\bugs\clangd-ccd_crash]
"C:\\Program Files\\LLVM\\bin\\clang" "-resource-dir=C:\\Program Files\\LLVM\\lib\\clang\\21" -- "d:\\Projects\\bugs\\clangd-ccd_crash\\main.cppm"
V[18:48:02.067] &lt;&lt;&lt; {"id":1,"jsonrpc":"2.0","method":"textDocument/documentSymbol","params":{"textDocument":{"uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash/main.cppm"}}}

I[18:48:02.067] &lt;-- textDocument/documentSymbol(1)
V[18:48:02.068] Driver produced command: cc1 -cc1 -triple x86_64-pc-windows-msvc19.44.35219 -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.cppm -mrelocation-model pic -pic-level 2 -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -fms-volatile -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic "-fdebug-compilation-dir=d:\\Projects\\bugs\\clangd-ccd_crash" "-fcoverage-compilation-dir=d:\\Projects\\bugs\\clangd-ccd_crash" -resource-dir "C:\\Program Files\\LLVM\\lib\\clang\\21" -internal-isystem "C:\\Program Files\\LLVM\\lib\\clang\\21\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.44.35207\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\VS\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.26100.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\cppwinrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.7.1\\include\\um" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.44.35207\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\VS\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.26100.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\cppwinrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.7.1\\include\\um" -fdeprecated-macro -ferror-limit 19 -fno-use-cxa-atexit -fms-extensions -fms-compatibility -fms-compatibility-version=19.44.35219 -std=c++14 -fskip-odr-check-in-gmf -fdelayed-template-parsing -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -x c++ "d:\\Projects\\bugs\\clangd-ccd_crash\\main.cppm"
I[18:48:02.068] --&gt; textDocument/clangd.fileStatus
V[18:48:02.068] &gt;&gt;&gt; {"jsonrpc":"2.0","method":"textDocument/clangd.fileStatus","params":{"state":"parsing includes, running Update","uri":"file:///d:/Projects/bugs/clangd-ccd_crash/main.cppm"}}

V[18:48:02.068] Building first preamble for d:\Projects\bugs\clangd-ccd_crash\main.cppm version 1
I[18:48:02.077] Built preamble of size 270900 for file d:\Projects\bugs\clangd-ccd_crash\main.cppm version 1 in 0.01 seconds
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Signalled while building preamble
  Filename: main.cppm
  Directory: d:\Projects\bugs\clangd-ccd_crash
  Command Line: C:\Program Files\LLVM\bin\clang -resource-dir=C:\Program Files\LLVM\lib\clang\21 -- d:\Projects\bugs\clangd-ccd_crash\main.cppm
  Version: 1
Exception Code: 0xC0000005
V[18:48:02.087] &lt;&lt;&lt; {"id":2,"jsonrpc":"2.0","method":"textDocument/documentLink","params":{"textDocument":{"uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash/main.cppm"}}}

I[18:48:02.087] &lt;-- textDocument/documentLink(2)
[Error - 6:48:02 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
[Error - 6:48:02 PM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 6:48:02 PM] Request textDocument/documentLink failed.
[object Object]
[Error - 6:48:02 PM] Server process exited with code 3221225477.
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: None (smilediver)

<details>
Clangd in VSCode on Windows crashes with a trivial C++ module file.

clangd version 21.1.5

Repro steps:

* Create an empty folder and the following files inside it:
* Create `main.cppm` file:
```cpp
module;

export module app;

extern "C++" int main() {
    return 0;
}
```
* Create `.vscode/settings.json` file:
```json
{
    "clangd.arguments": [
        "--experimental-modules-support",
        "--log=verbose",
    ],
}
```
* Open the folder as project in VSCode.
* Then open the `main.cppm` file, and clangd should crash at this point.

Output window log:
```
I[18:48:02.052] clangd version 21.1.5
I[18:48:02.053] Features: windows
I[18:48:02.053] PID: 25940
I[18:48:02.053] Working directory: d:\Projects\bugs\clangd-ccd_crash
I[18:48:02.053] argv[0]: c:\Program Files\LLVM\bin\clangd.exe
I[18:48:02.053] argv[1]: --experimental-modules-support
I[18:48:02.053] argv[2]: --log=verbose
V[18:48:02.057] User config file is C:\Users\smile\AppData\Local\clangd\config.yaml
I[18:48:02.057] Starting LSP over stdin/stdout
V[18:48:02.057] &lt;&lt;&lt; {"id":0,"jsonrpc":"2.0","method":"initialize","params":{"capabilities":{"general":{"markdown":{"parser":"marked","version":"1.1.0"},"positionEncodings":["utf-16"],"regularExpressions":{"engine":"ECMAScript","version":"ES2020"},"staleRequestSupport":{"cancel":true,"retryOnContentModified":["textDocument/semanticTokens/full","textDocument/semanticTokens/range","textDocument/semanticTokens/full/delta"]}},"notebookDocument":{"synchronization":{"dynamicRegistration":true,"executionSummarySupport":true}},"textDocument":{"callHierarchy":{"dynamicRegistration":true},"codeAction":{"codeActionLiteralSupport":{"codeActionKind":{"valueSet":["","quickfix","refactor","refactor.extract","refactor.inline","refactor.rewrite","source","source.organizeImports"]}},"dataSupport":true,"disabledSupport":true,"dynamicRegistration":true,"honorsChangeAnnotations":true,"isPreferredSupport":true,"resolveSupport":{"properties":["edit"]}},"codeLens":{"dynamicRegistration":true},"colorProvider":{"dynamicRegistration":true},"completion":{"completionItem":{"commitCharactersSupport":true,"deprecatedSupport":true,"documentationFormat":["markdown","plaintext"],"insertReplaceSupport":true,"insertTextModeSupport":{"valueSet":[1,2]},"labelDetailsSupport":true,"preselectSupport":true,"resolveSupport":{"properties":["documentation","detail","additionalTextEdits"]},"snippetSupport":true,"tagSupport":{"valueSet":[1]}},"completionItemKind":{"valueSet":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25]},"completionList":{"itemDefaults":["commitCharacters","editRange","insertTextFormat","insertTextMode","data"]},"contextSupport":true,"dynamicRegistration":true,"editsNearCursor":true,"insertTextMode":2},"declaration":{"dynamicRegistration":true,"linkSupport":true},"definition":{"dynamicRegistration":true,"linkSupport":true},"diagnostic":{"dynamicRegistration":true,"relatedDocumentSupport":false},"documentHighlight":{"dynamicRegistration":true},"documentLink":{"dynamicRegistration":true,"tooltipSupport":true},"documentSymbol":{"dynamicRegistration":true,"hierarchicalDocumentSymbolSupport":true,"labelSupport":true,"symbolKind":{"valueSet":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26]},"tagSupport":{"valueSet":[1]}},"foldingRange":{"dynamicRegistration":true,"foldingRange":{"collapsedText":false},"foldingRangeKind":{"valueSet":["comment","imports","region"]},"lineFoldingOnly":true,"rangeLimit":5000},"formatting":{"dynamicRegistration":true},"hover":{"contentFormat":["markdown","plaintext"],"dynamicRegistration":true},"implementation":{"dynamicRegistration":true,"linkSupport":true},"inactiveRegionsCapabilities":{"inactiveRegions":true},"inlayHint":{"dynamicRegistration":true,"resolveSupport":{"properties":["tooltip","textEdits","label.tooltip","label.location","label.command"]}},"inlineValue":{"dynamicRegistration":true},"linkedEditingRange":{"dynamicRegistration":true},"onTypeFormatting":{"dynamicRegistration":true},"publishDiagnostics":{"codeDescriptionSupport":true,"dataSupport":true,"relatedInformation":true,"tagSupport":{"valueSet":[1,2]},"versionSupport":false},"rangeFormatting":{"dynamicRegistration":true,"rangesSupport":true},"references":{"dynamicRegistration":true},"rename":{"dynamicRegistration":true,"honorsChangeAnnotations":true,"prepareSupport":true,"prepareSupportDefaultBehavior":1},"selectionRange":{"dynamicRegistration":true},"semanticTokens":{"augmentsSyntaxTokens":true,"dynamicRegistration":true,"formats":["relative"],"multilineTokenSupport":false,"overlappingTokenSupport":false,"requests":{"full":{"delta":true},"range":true},"serverCancelSupport":true,"tokenModifiers":["declaration","definition","readonly","static","deprecated","abstract","async","modification","documentation","defaultLibrary"],"tokenTypes":["namespace","type","class","enum","interface","struct","typeParameter","parameter","variable","property","enumMember","event","function","method","macro","keyword","modifier","comment","string","number","regexp","operator","decorator"]},"signatureHelp":{"contextSupport":true,"dynamicRegistration":true,"signatureInformation":{"activeParameterSupport":true,"documentationFormat":["markdown","plaintext"],"parameterInformation":{"labelOffsetSupport":true}}},"synchronization":{"didSave":true,"dynamicRegistration":true,"willSave":true,"willSaveWaitUntil":true},"typeDefinition":{"dynamicRegistration":true,"linkSupport":true},"typeHierarchy":{"dynamicRegistration":true}},"window":{"showDocument":{"support":true},"showMessage":{"messageActionItem":{"additionalPropertiesSupport":true}},"workDoneProgress":true},"workspace":{"applyEdit":true,"codeLens":{"refreshSupport":true},"configuration":true,"diagnostics":{"refreshSupport":true},"didChangeConfiguration":{"dynamicRegistration":true},"didChangeWatchedFiles":{"dynamicRegistration":true,"relativePatternSupport":true},"executeCommand":{"dynamicRegistration":true},"fileOperations":{"didCreate":true,"didDelete":true,"didRename":true,"dynamicRegistration":true,"willCreate":true,"willDelete":true,"willRename":true},"foldingRange":{"refreshSupport":true},"inlayHint":{"refreshSupport":true},"inlineValue":{"refreshSupport":true},"semanticTokens":{"refreshSupport":true},"symbol":{"dynamicRegistration":true,"resolveSupport":{"properties":["location.range"]},"symbolKind":{"valueSet":[1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26]},"tagSupport":{"valueSet":[1]}},"workspaceEdit":{"changeAnnotationSupport":{"groupsOnLabel":true},"documentChanges":true,"failureHandling":"textOnlyTransactional","normalizesLineEndings":true,"resourceOperations":["create","rename","delete"]},"workspaceFolders":true}},"clientInfo":{"name":"Visual Studio Code","version":"1.105.1"},"initializationOptions":{"clangdFileStatus":true,"fallbackFlags":[]},"locale":"en","processId":60656,"rootPath":"d:\\Projects\\bugs\\clangd-ccd_crash","rootUri":"file:///d%3A/Projects/bugs/clangd-ccd_crash","trace":"off","workspaceFolders":[{"name":"clangd-ccd_crash","uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash"}]}}

I[18:48:02.057] &lt;-- initialize(0)
I[18:48:02.059] --&gt; reply:initialize(0) 1 ms
V[18:48:02.059] &gt;&gt;&gt; {"id":0,"jsonrpc":"2.0","result":{"capabilities":{"astProvider":true,"callHierarchyProvider":true,"clangdInlayHintsProvider":true,"codeActionProvider":{"codeActionKinds":["quickfix","refactor","info"]},"compilationDatabase":{"automaticReload":true},"completionProvider":{"resolveProvider":false,"triggerCharacters":[".","&lt;","&gt;",":","\"","/","*"]},"declarationProvider":true,"definitionProvider":true,"documentFormattingProvider":true,"documentHighlightProvider":true,"documentLinkProvider":{"resolveProvider":false},"documentOnTypeFormattingProvider":{"firstTriggerCharacter":"\n","moreTriggerCharacter":[]},"documentRangeFormattingProvider":{"rangesSupport":true},"documentSymbolProvider":true,"executeCommandProvider":{"commands":["clangd.applyFix","clangd.applyRename","clangd.applyTweak"]},"foldingRangeProvider":true,"hoverProvider":true,"implementationProvider":true,"inactiveRegionsProvider":true,"inlayHintProvider":true,"memoryUsageProvider":true,"positionEncoding":"utf-16","referencesProvider":true,"renameProvider":{"prepareProvider":true},"selectionRangeProvider":true,"semanticTokensProvider":{"full":{"delta":true},"legend":{"tokenModifiers":["declaration","definition","deprecated","deduced","readonly","static","abstract","virtual","dependentName","defaultLibrary","usedAsMutableReference","usedAsMutablePointer","constructorOrDestructor","userDefined","functionScope","classScope","fileScope","globalScope"],"tokenTypes":["variable","variable","parameter","function","method","function","property","variable","class","interface","enum","enumMember","type","type","unknown","namespace","typeParameter","concept","type","macro","modifier","operator","bracket","label","comment"]},"range":false},"signatureHelpProvider":{"triggerCharacters":["(",")","{","}","&lt;","&gt;",","]},"standardTypeHierarchyProvider":true,"textDocumentSync":{"change":2,"openClose":true,"save":true},"typeDefinitionProvider":true,"typeHierarchyProvider":true,"workspaceSymbolProvider":true},"offsetEncoding":"utf-16","serverInfo":{"name":"clangd","version":"clangd version 21.1.5 windows x86_64-pc-windows-msvc"}}}

V[18:48:02.063] &lt;&lt;&lt; {"jsonrpc":"2.0","method":"initialized","params":{}}

I[18:48:02.063] &lt;-- initialized
V[18:48:02.065] &lt;&lt;&lt; {"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"languageId":"cpp","text":"module;\r\n\r\nexport module app;\r\n\r\nextern \"C++\" int main() {\r\n    return 0;\r\n}\r\n","uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash/main.cppm","version":1}}}

I[18:48:02.065] &lt;-- textDocument/didOpen
I[18:48:02.066] Failed to find compilation database for d:\Projects\bugs\clangd-ccd_crash\main.cppm
I[18:48:02.066] ASTWorker building file d:\Projects\bugs\clangd-ccd_crash\main.cppm version 1 with command clangd fallback
[d:\Projects\bugs\clangd-ccd_crash]
"C:\\Program Files\\LLVM\\bin\\clang" "-resource-dir=C:\\Program Files\\LLVM\\lib\\clang\\21" -- "d:\\Projects\\bugs\\clangd-ccd_crash\\main.cppm"
V[18:48:02.067] &lt;&lt;&lt; {"id":1,"jsonrpc":"2.0","method":"textDocument/documentSymbol","params":{"textDocument":{"uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash/main.cppm"}}}

I[18:48:02.067] &lt;-- textDocument/documentSymbol(1)
V[18:48:02.068] Driver produced command: cc1 -cc1 -triple x86_64-pc-windows-msvc19.44.35219 -fsyntax-only -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name main.cppm -mrelocation-model pic -pic-level 2 -mframe-pointer=none -relaxed-aliasing -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -fms-volatile -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic "-fdebug-compilation-dir=d:\\Projects\\bugs\\clangd-ccd_crash" "-fcoverage-compilation-dir=d:\\Projects\\bugs\\clangd-ccd_crash" -resource-dir "C:\\Program Files\\LLVM\\lib\\clang\\21" -internal-isystem "C:\\Program Files\\LLVM\\lib\\clang\\21\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.44.35207\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\VS\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.26100.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\cppwinrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.7.1\\include\\um" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.44.35207\\include" -internal-isystem "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\VS\\include" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.26100.0\\ucrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\um" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\shared" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\winrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\10\\\\include\\10.0.26100.0\\\\cppwinrt" -internal-isystem "C:\\Program Files (x86)\\Windows Kits\\NETFXSDK\\4.7.1\\include\\um" -fdeprecated-macro -ferror-limit 19 -fno-use-cxa-atexit -fms-extensions -fms-compatibility -fms-compatibility-version=19.44.35219 -std=c++14 -fskip-odr-check-in-gmf -fdelayed-template-parsing -fcxx-exceptions -fexceptions -no-round-trip-args -faddrsig -x c++ "d:\\Projects\\bugs\\clangd-ccd_crash\\main.cppm"
I[18:48:02.068] --&gt; textDocument/clangd.fileStatus
V[18:48:02.068] &gt;&gt;&gt; {"jsonrpc":"2.0","method":"textDocument/clangd.fileStatus","params":{"state":"parsing includes, running Update","uri":"file:///d:/Projects/bugs/clangd-ccd_crash/main.cppm"}}

V[18:48:02.068] Building first preamble for d:\Projects\bugs\clangd-ccd_crash\main.cppm version 1
I[18:48:02.077] Built preamble of size 270900 for file d:\Projects\bugs\clangd-ccd_crash\main.cppm version 1 in 0.01 seconds
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Signalled while building preamble
  Filename: main.cppm
  Directory: d:\Projects\bugs\clangd-ccd_crash
  Command Line: C:\Program Files\LLVM\bin\clang -resource-dir=C:\Program Files\LLVM\lib\clang\21 -- d:\Projects\bugs\clangd-ccd_crash\main.cppm
  Version: 1
Exception Code: 0xC0000005
V[18:48:02.087] &lt;&lt;&lt; {"id":2,"jsonrpc":"2.0","method":"textDocument/documentLink","params":{"textDocument":{"uri":"file:///d%3A/Projects/bugs/clangd-ccd_crash/main.cppm"}}}

I[18:48:02.087] &lt;-- textDocument/documentLink(2)
[Error - 6:48:02 PM] The Clang Language Server server crashed 5 times in the last 3 minutes. The server will not be restarted. See the output for more information.
[Error - 6:48:02 PM] Request textDocument/documentSymbol failed.
[object Object]
[Error - 6:48:02 PM] Request textDocument/documentLink failed.
[object Object]
[Error - 6:48:02 PM] Server process exited with code 3221225477.
```
</details>


---

