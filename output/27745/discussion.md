# Out of memory in the backend

**Author:** b1622bb8-0285-400e-ba44-ba0e5027e546
**URL:** https://github.com/llvm/llvm-project/issues/27745

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [27371](https://llvm.org/bz27371) |
| Version | trunk |
| OS | Windows NT |
| Attachments | [Script file](https://user-images.githubusercontent.com/92601339/143753376-836031ad-e032-4907-a3cc-78239587fc65.gz) |
| CC | @DougGregor,@TNorthover |

## Extended Description 
Here is the bug report:

This application has requested the Runtime to terminate it in an unusual way.
Please contact the application's support team for more information.
Assertion failed!

Program: C:\Programming\VersionControl\bninja_installed_clang\bin\clang++.exe
File: E:/Programming/VersionControl/llvm/lib/Support/SmallVector.cpp, Line 36
```
Expression: NewElts && "Out of memory"
#0 0x016ff859 HandleAbort (C:\Programming\VersionControl\bninja_installed_clang\bin\clang++.exe+0x12ff859)
#1 0x74b1636c abort (C:\WINDOWS\SYSTEM32\msvcrt.dll+0x8636c)
#2 0x02d655a8 _wassert (C:\Programming\VersionControl\bninja_installed_clang\bin\clang++.exe+0x29655a8)
#3 0x0062005c _ZNK4llvm11SIInstrInfo18buildExtractSubRegENS_26MachineInstrBundleIteratorINS_12MachineInstrEEERNS_19MachineRegisterInfoERNS_14MachineOperandEPKNS_19TargetRegisterClassEjSA_ (C:\Programming\VersionControl\bninja_installed_clang\bin\clang++.exe+0x22005c)
#4 0x0069006e _ZNK4llvm20AMDGPUTargetLowering14LowerUDIVREM64ENS_7SDValueERNS_12SelectionDAGERNS_15SmallVectorImplIS1_EE (C:\Programming\VersionControl\bninja_installed_clang\bin\clang++.exe+0x29006e)
#5 0x000fff25
#6 0x003e8000
#7 0x38244489
#8 0xfffffffffea1840f
#9 0x44c7ffff
#10 0x010c0824 _ZN12_GLOBAL__N_19SSAIfConv12canConvertIfEPN4llvm17MachineBasicBlockE.constprop.221 (C:\Programming\VersionControl\bninja_installed_clang\bin\clang++.exe+0xcc0824)
#11 0x44c70000
#12 0xffffffffdad00424
#13 0x04c70347
#14 0x47db4824
#15 0x5522e803
#16 0xffffffff80e90274
#17 0xffffffffc7fffffe
#18 0x2e082444
#19 0xffffffffc7000000
#20 0xffffffffd0042444
#21 0xffffffffc70347dd
#22 0xffffffffde142404
#23 0x01e80347 _ZN12_GLOBAL__N_111CrossDSOCFI13buildCFICheckEv.isra.97.constprop.98 (C:\Programming\VersionControl\bninja_installed_clang\bin\clang++.exe+0x1a80347)
#24 0xffffffffe9027455
#25 0xffffffffffffff0a
#26 0x082444c7
clang++.exe: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 3.9.0 (trunk 264728)
Target: i686-pc-windows-gnu
Thread model: posix
InstalledDir: C:/Programming/VersionControl/bninja_installed_clang/bin
clang++.exe: note: diagnostic msg: PLEASE submit a bug report to http://llvm.org/bugs/ and include the crash backtrace, preprocessed source, and associated run script.
clang++.exe: note: diagnostic msg: 
********************
```

## Comments

### Comment 1 - TNorthover

Hang on, no. That backtrace is pretty nonsensical. I get preprocessor errors. Back to Clang.

---

### Comment 2 - TNorthover

The backtrace indicates MachineInstrs are in play so this is a backend issue.

---

### Comment 3 - b1622bb8-0285-400e-ba44-ba0e5027e546

The link to the preprocessed output is on DropBox and is https://www.dropbox.com/s/spz8bo1pfr5vfpi/test_library_name-cb80ca.cpp?dl=0.

---

### Comment 4 - b1622bb8-0285-400e-ba44-ba0e5027e546

Preprocessed output attachment is 8 MB and is too large to add as an attachment. Furthermore pastebin etc. has limits below 8 MB. I can send the preprocessed output as a zip file to whomever will handle the bug report.

---

### Comment 5 - Endilll

Reproduced with Clang 5 with assertions enabled:
```
clang: /home/tonik/llvm-project/llvm/tools/clang/lib/Basic/SourceManager.cpp:592:
clang::SourceLocation clang::SourceManager::createExpansionLocImpl(const clang::SrcMgr::ExpansionInfo&, unsigned int, int, unsigned int):
Assertion `NextLocalOffset + TokLength + 1 > NextLocalOffset && NextLocalOffset + TokLength + 1 <= CurrentLoadedOffset && "Ran out of source locations!"' failed.

#0 0x00000000021ad3da llvm::sys::PrintStackTrace(llvm::raw_ostream&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x21ad3da)
#1 0x00000000021aaf76 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x21aaf76)
#2 0x00000000021ab315 SignalHandler(int) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x21ab315)
#3 0x00007f0200042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
#4 0x00007f0200096a7c pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x96a7c)
#5 0x00007f0200042476 raise (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
#6 0x00007f02000287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
#7 0x00007f020002871b _dl_exception_create (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
#8 0x00007f0200039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#9 0x0000000002338ee5 clang::SourceManager::createExpansionLocImpl(clang::SrcMgr::ExpansionInfo const&, unsigned int, int, unsigned int) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2338ee5)
#10 0x0000000002339005 clang::SourceManager::createMacroArgExpansionLoc(clang::SourceLocation, clang::SourceLocation, unsigned int) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2339005)
#11 0x0000000003ced4de clang::TokenLexer::updateLocForMacroArgTokens(clang::SourceLocation, clang::Token*, clang::Token*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ced4de)
#12 0x0000000003cee54f clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee54f)
#13 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#14 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#15 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#16 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#17 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#18 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#19 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#20 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#21 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#22 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#23 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#24 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#25 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#26 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#27 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#28 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#29 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#30 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#31 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#32 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#33 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#34 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#35 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#36 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#37 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#38 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#39 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#40 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#41 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#42 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#43 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#44 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#45 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#46 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#47 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#48 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#49 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#50 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#51 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#52 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#53 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#54 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#55 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#56 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#57 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#58 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#59 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#60 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#61 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#62 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#63 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#64 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#65 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#66 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#67 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#68 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#69 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#70 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#71 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#72 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#73 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#74 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#75 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#76 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#77 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#78 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#79 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#80 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#81 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#82 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#83 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#84 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#85 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#86 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#87 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#88 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#89 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#90 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#91 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#92 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#93 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#94 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#95 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#96 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#97 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#98 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#99 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#100 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#101 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#102 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#103 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#104 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#105 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#106 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#107 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#108 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#109 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#110 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#111 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#112 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#113 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#114 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#115 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#116 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#117 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#118 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#119 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#120 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#121 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#122 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#123 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#124 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#125 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#126 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#127 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#128 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#129 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#130 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#131 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#132 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#133 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#134 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#135 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#136 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#137 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#138 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#139 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#140 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#141 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#142 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#143 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#144 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#145 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#146 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#147 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#148 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#149 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#150 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#151 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#152 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#153 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#154 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#155 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#156 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#157 0x0000000003ce867f clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce867f)
#158 0x0000000003cf0bea clang::MacroArgs::getPreExpArgument(unsigned int, clang::MacroInfo const*, clang::Preprocessor&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cf0bea)
#159 0x0000000003cee4fb clang::TokenLexer::ExpandFunctionArguments() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee4fb)
#160 0x0000000003cee90f clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cee90f)
#161 0x0000000003cb8abb clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3cb8abb)
#162 0x0000000003ccc918 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ccc918)
#163 0x0000000003ce55ca clang::Preprocessor::HandleIdentifier(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce55ca)
#164 0x0000000003c716fc clang::Lexer::LexIdentifier(clang::Token&, char const*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3c716fc)
#165 0x0000000003c73153 clang::Lexer::LexTokenInternal(clang::Token&, bool) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3c73153)
#166 0x0000000003c75996 clang::Lexer::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3c75996)
#167 0x0000000003ce86bf clang::Preprocessor::Lex(clang::Token&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x3ce86bf)
#168 0x0000000002eed60b clang::Parser::ExpectAndConsumeSemi(unsigned int) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2eed60b)
#169 0x0000000002f73dfd clang::Parser::ParseExprStatement() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2f73dfd)
#170 0x0000000002f70f7c clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::AllowedConstructsKind, clang::SourceLocation*, clang::Parser::ParsedAttributesWithRange&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2f70f7c)
#171 0x0000000002f71254 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::AllowedConstructsKind, clang::SourceLocation*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2f71254)
#172 0x0000000002f75800 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2f75800)
#173 0x0000000002f77dcb clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2f77dcb)
#174 0x0000000002eef64f clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2eef64f)
#175 0x0000000002f109c5 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, unsigned int, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2f109c5)
#176 0x0000000002eebcc1 clang::Parser::ParseDeclOrFunctionDefInternal(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2eebcc1)
#177 0x0000000002eec2f1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*, clang::AccessSpecifier) [clone .part.166] (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2eec2f1)
#178 0x0000000002ef200a clang::Parser::ParseExternalDeclaration(clang::Parser::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2ef200a)
#179 0x0000000002ef2ed8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2ef2ed8)
#180 0x0000000002ee7612 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2ee7612)
#181 0x00000000027501e6 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x27501e6)
#182 0x0000000002719bde clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x2719bde)
#183 0x00000000027ec85a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x27ec85a)
#184 0x0000000000a59238 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0xa59238)
#185 0x00000000009eafca main (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0x9eafca)
#186 0x00007f0200029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#187 0x00007f0200029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#188 0x0000000000a554f9 _start (/opt/compiler-explorer/clang-5.0.0-assert/bin/clang+0xa554f9)
Stack dump:
0.      Program arguments: /opt/compiler-explorer/clang-5.0.0-assert/bin/clang -cc1 -triple i686-pc-windows-gnu -D __MINGW_FORCE_SYS_INTRINS -D BOOST_ALL_NO_LIB=1 -std=c++11 -x c++ test_library_name-cb80ca.cpp 
1.      test_library_name.cpp:138:3: current parser token 'BOOST_TEST'
2.      test_library_name.cpp:15:3: parsing function body 'main'
3.      test_library_name.cpp:15:3: in compound statement ('{}')
Aborted (core dumped)
```

---

### Comment 6 - Endilll

Apparently reproducer is close enough to make compiler consume all available memory, which makes automatic reduction via C-Reduce problematic.

---

### Comment 7 - Endilll

Possibly related: #29894

---

### Comment 8 - Endilll

After days of reduction, C-Reduced produced a reasonably-sized reduction:
```
#define BOOST_PP_NODE_192() BOOST_PP_NODE_160
#define BOOST_PP_NODE_160() BOOST_PP_NODE_176
#define BOOST_PP_NODE_176() BOOST_PP_NODE_184
#define BOOST_PP_NODE_184() BOOST_PP_NODE_187
#define BOOST_PP_NODE_187() 188
#define BOOST_PP_AUTO_REC(pred, n) BOOST_PP_NODE_192()()()()()
#define BOOST_PP_WHILE_188(p, o, s) BOOST_PP_WHILE_189(, o, o(, s))
#define BOOST_PP_WHILE_189(p, o, s) BOOST_PP_WHILE_190(, o, o(, s))
#define BOOST_PP_WHILE_190(p, o, s) o(, s)
#define BOOST_PP_IIF(bit, t, f) bit f(bit f
#define BOOST_VMD_DETAIL_SEQUENCE_OP(d, state)                                   (state(state\state\state\state\state(state) state state))
#define BOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE(vseq, elem, output, from)   (BOOST_PP_CAT(                                                                     BOOST_PP_WHILE_,                                                               BOOST_PP_AUTO_REC(, )(, BOOST_VMD_DETAIL_SEQUENCE_OP, vseq\from)))
#define BOOST_VMD_DETAIL_SEQUENCE_ELEM_NM(elem, vseq, nm)                        (((BOOST_VMD_DETAIL_SEQUENCE_ELEM_PROCESS_TUPLE(vseq, , , nm))))
#define BOOST_VMD_GET_TYPE(...) BOOST_VMD_DETAIL_SEQUENCE_ELEM_NM(                                           , BOOST_VMD_DETAIL_SEQUENCE_ELEM_GET_VSEQ(__VA_ARGS__),                        __VA_ARGS__(BOOST_PP_VARIADIC_ELEM(__VA_ARGS__)                                                BOOST_VMD_RETURN_AFTERBOOST_VMD_RETURN_TYPE_TUPLE))
BOOST_PP_IIF(, , (BOOST_PP_IIF(BOOST_VMD_GET_TYPE(BOOST_VMD_GET_TYPE()), , ))         
#define BOOST_PP_CAT(a, b) BOOST_PP_CAT_I(a, b)
#define BOOST_PP_CAT_I(a, b) a##b
)
```
I tried to remove seemingly unimportant whitespaces, but turn out it is sensitive to whitespaces as well.

---

### Comment 9 - Endilll

Still crashing as of post-16 trunk. No Compiler explorer link this time, because it takes too much time and memory to crash:
```
clang: /home/user/endill/llvm-project/clang/lib/Basic/SourceManager.cpp:678:
SourceLocation clang::SourceManager::createExpansionLocImpl(const ExpansionInfo &, unsigned int, int, SourceLocation::UIntTy):
Assertion `NextLocalOffset + Length + 1 > NextLocalOffset && NextLocalOffset + Length + 1 <= CurrentLoadedOffset && "Ran out of source locations!"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /home/user/endill/llvm-project/build-linux/bin/clang -cc1 -std=c++11 -x c++ test_library_name-cb80ca.cpp
1.	test_library_name-cb80ca.cpp:15:1: current parser token 'BOOST_PP_IIF'
 #0 0x0000556956b9f181 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /home/user/endill/llvm-project/llvm/lib/Support/Unix/Signals.inc:602:11
 #1 0x0000556956b9f62b PrintStackTraceSignalHandler(void*) /home/user/endill/llvm-project/llvm/lib/Support/Unix/Signals.inc:675:1
 #2 0x0000556956b9d896 llvm::sys::RunSignalHandlers() /home/user/endill/llvm-project/llvm/lib/Support/Signals.cpp:104:5
 #3 0x0000556956b9fdc5 SignalHandler(int) /home/user/endill/llvm-project/llvm/lib/Support/Unix/Signals.inc:413:1
 #4 0x00007f9a68436510 (/lib/x86_64-linux-gnu/libc.so.6+0x3c510)
 #5 0x00007f9a684840fc __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #6 0x00007f9a68436472 raise ./signal/../sysdeps/posix/raise.c:27:6
 #7 0x00007f9a684204b2 abort ./stdlib/abort.c:81:7
 #8 0x00007f9a684203d5 _nl_load_domain ./intl/loadmsgcat.c:1177:9
 #9 0x00007f9a6842f3a2 (/lib/x86_64-linux-gnu/libc.so.6+0x353a2)
#10 0x0000556956de714b clang::SourceManager::createExpansionLocImpl(clang::SrcMgr::ExpansionInfo const&, unsigned int, int, unsigned int) /home/user/endill/llvm-project/clang/lib/Basic/SourceManager.cpp:0:3
#11 0x0000556956de6e82 clang::SourceManager::createMacroArgExpansionLoc(clang::SourceLocation, clang::SourceLocation, unsigned int) /home/user/endill/llvm-project/clang/lib/Basic/SourceManager.cpp:638:10
#12 0x000055695db4ccfc updateConsecutiveMacroArgTokens(clang::SourceManager&, clang::SourceLocation, clang::Token*&, clang::Token*) /home/user/endill/llvm-project/clang/lib/Lex/TokenLexer.cpp:1043:10
#13 0x000055695db4c320 clang::TokenLexer::updateLocForMacroArgTokens(clang::SourceLocation, clang::Token*, clang::Token*) /home/user/endill/llvm-project/clang/lib/Lex/TokenLexer.cpp:1076:3
#14 0x000055695db4a358 clang::TokenLexer::ExpandFunctionArguments() /home/user/endill/llvm-project/clang/lib/Lex/TokenLexer.cpp:490:20
#15 0x000055695db493d2 clang::TokenLexer::Init(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) /home/user/endill/llvm-project/clang/lib/Lex/TokenLexer.cpp:0:5
#16 0x000055695db0cc06 clang::TokenLexer::TokenLexer(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*, clang::Preprocessor&) /home/user/endill/llvm-project/clang/include/clang/Lex/TokenLexer.h:112:3
#17 0x000055695db09cc3 std::__detail::_MakeUniq<clang::TokenLexer>::__single_object std::make_unique<clang::TokenLexer, clang::Token&, clang::SourceLocation&, clang::MacroInfo*&, clang::MacroArgs*&, clang::Preprocessor&>(clang::Token&, clang::SourceLocation&, clang::MacroInfo*&, clang::MacroArgs*&, clang::Preprocessor&) /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/bits/unique_ptr.h:1070:34
#18 0x000055695db05e7a clang::Preprocessor::EnterMacro(clang::Token&, clang::SourceLocation, clang::MacroInfo*, clang::MacroArgs*) /home/user/endill/llvm-project/clang/lib/Lex/PPLexerChange.cpp:155:14
#19 0x000055695db10e27 clang::Preprocessor::HandleMacroExpandedIdentifier(clang::Token&, clang::MacroDefinition const&) /home/user/endill/llvm-project/clang/lib/Lex/PPMacroExpansion.cpp:633:3
#20 0x000055695db4008b clang::Preprocessor::HandleIdentifier(clang::Token&) /home/user/endill/llvm-project/clang/lib/Lex/Preprocessor.cpp:825:11
#21 0x000055695daa2bf5 clang::Lexer::LexIdentifierContinue(clang::Token&, char const*) /home/user/endill/llvm-project/clang/lib/Lex/Lexer.cpp:1900:5
#22 0x000055695daa912a clang::Lexer::LexTokenInternal(clang::Token&, bool) /home/user/endill/llvm-project/clang/lib/Lex/Lexer.cpp:3875:5
#23 0x000055695daa63ec clang::Lexer::Lex(clang::Token&) /home/user/endill/llvm-project/clang/lib/Lex/Lexer.cpp:3578:8
#24 0x000055695db403ef clang::Preprocessor::Lex(clang::Token&) /home/user/endill/llvm-project/clang/lib/Lex/Preprocessor.cpp:886:21
#25 0x000055695b13f1ee clang::Parser::ConsumeToken() /home/user/endill/llvm-project/clang/include/clang/Parse/Parser.h:504:8
#26 0x000055695b136254 clang::Parser::Initialize() /home/user/endill/llvm-project/clang/lib/Parse/Parser.cpp:568:3
#27 0x000055695b1319e5 clang::ParseAST(clang::Sema&, bool, bool) /home/user/endill/llvm-project/clang/lib/Parse/ParseAST.cpp:157:28
#28 0x0000556957f1a8c6 clang::ASTFrontendAction::ExecuteAction() /home/user/endill/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1170:1
#29 0x0000556957f1a2ec clang::FrontendAction::Execute() /home/user/endill/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1062:7
#30 0x0000556957e485a8 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /home/user/endill/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1049:23
#31 0x00005569580e4e16 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /home/user/endill/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:272:8
#32 0x0000556954e201e3 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /home/user/endill/llvm-project/clang/tools/driver/cc1_main.cpp:249:13
#33 0x0000556954e10972 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /home/user/endill/llvm-project/clang/tools/driver/driver.cpp:366:5
#34 0x0000556954e0f6b4 clang_main(int, char**, llvm::ToolContext const&) /home/user/endill/llvm-project/clang/tools/driver/driver.cpp:407:5
#35 0x0000556954e429ed main /home/user/endill/llvm-project/build-linux/tools/clang/tools/driver/clang-driver.cpp:15:3
#36 0x00007f9a684216ca __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#37 0x00007f9a68421785 call_init ./csu/../csu/libc-start.c:128:20
#38 0x00007f9a68421785 __libc_start_main ./csu/../csu/libc-start.c:347:5
#39 0x0000556954e0f0e1 _start (/home/user/endill/llvm-project/build-linux/bin/clang+0x515d0e1)
```

---

### Comment 10 - Endilll

> The link to the preprocessed output is on DropBox and is https://www.dropbox.com/s/spz8bo1pfr5vfpi/test_library_name-cb80ca.cpp?dl=0.

Attaching the original reproducer directly for archival purposes: [test_library_name-cb80ca.zip](https://github.com/user-attachments/files/21306812/test_library_name-cb80ca.1.zip)




---

