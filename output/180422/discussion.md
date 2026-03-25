# [analyzer] Crash (0xC0000005) in evalCall when strcmp is overloaded with (const char*, char) | clang-tidy (2.1.8), with complete crash.cpp

**Author:** wobosoft
**URL:** https://github.com/llvm/llvm-project/issues/180422
**Status:** Open
**Labels:** clang:static analyzer, crash

## Body

### Command line used:
clang-tidy crash.cpp -checks='clang-analyzer-*' -- -std=c++26

### crash.cpp:
```c++
int strcmp(const char *s, char c); 

void trigger(const char* p) {
    if (strcmp(p, '(') == 0) { 
        // Crash happens here
    }
}
```

```
### Stack dump:
0.	Program arguments: D:\\msys64\\ucrt64\\bin\\clang-tidy.exe crash.cpp -checks=clang-analyzer-* -- -std=c++26
1.	<eof> parser at end of file
2.	While analyzing stack: 
	#0 Calling trigger(const char *)
3.	E:/vhdl/lib_tb_matfile/crash.cpp:5:9: Error evaluating statement
4.	E:/vhdl/lib_tb_matfile/crash.cpp:5:9: Error evaluating statement
Exception Code: 0xC0000005
0x00007FF84C945669, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x475669 byte(s), _Z9EmitToIntRN5clang7CodeGen15CodeGenFunctionEPN4llvm5ValueENS_8QualTypeEPNS3_11IntegerTypeE() + 0x459239 byte(s)
0x00007FF84DD41413, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1871413 byte(s), _ZN5clang4ento11SValBuilder9evalBinOpEN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEENS_18BinaryOperatorKindENS0_4SValES8_NS_8QualTypeE() + 0x293 byte(s)
0x00007FF84DD40EB7, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1870EB7 byte(s), _ZN5clang4ento11SValBuilder6evalEQEN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEENS0_20DefinedOrUnknownSValES7_() + 0xC7 byte(s)
0x00007FF84E69C07E, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x21CC07E byte(s), _ZNK12PNaClABIInfo9EmitVAArgERN5clang7CodeGen15CodeGenFunctionENS1_7AddressENS0_8QualTypeENS1_12AggValueSlotE() + 0x39F5E byte(s)
0x00007FF84E699081, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x21C9081 byte(s), _ZNK12PNaClABIInfo9EmitVAArgERN5clang7CodeGen15CodeGenFunctionENS1_7AddressENS0_8QualTypeENS1_12AggValueSlotE() + 0x36F61 byte(s)
0x00007FF84DDDD83A, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x190D83A byte(s), _ZN5clang4ento48shouldRegisterNullableReturnedFromNonnullCheckerERKNS0_14CheckerManagerE() + 0xF7A byte(s)
0x00007FF84DD56A43, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1886A43 byte(s), _ZN5clang4ento14CheckerManager22runCheckersForEvalCallERNS0_15ExplodedNodeSetERKS2_RKNS0_9CallEventERNS0_10ExprEngineERKNS0_15EvalCallOptionsE() + 0x1E3 byte(s)
0x00007FF84DD388F2, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x18688F2 byte(s), _ZN5clang4ento10ExprEngine8evalCallERNS0_15ExplodedNodeSetEPNS0_12ExplodedNodeERKNS0_9CallEventE() + 0x192 byte(s)
0x00007FF84DD15EC4, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1845EC4 byte(s), _ZN5clang4ento10ExprEngine13VisitCallExprEPKNS_8CallExprEPNS0_12ExplodedNodeERNS0_15ExplodedNodeSetE() + 0x1D4 byte(s)
0x00007FF84DD38011, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1868011 byte(s), _ZN5clang4ento10ExprEngine5VisitEPKNS_4StmtEPNS0_12ExplodedNodeERNS0_15ExplodedNodeSetE() + 0x2661 byte(s)
0x00007FF84DD15237, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1845237 byte(s), _ZN5clang4ento10ExprEngine11ProcessStmtEPKNS_4StmtEPNS0_12ExplodedNodeE() + 0x267 byte(s)
0x00007FF84DD13939, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1843939 byte(s), _ZN5clang4ento10CoreEngine16dispatchWorkItemEPNS0_12ExplodedNodeENS_12ProgramPointERKNS0_12WorkListUnitE() + 0x509 byte(s)
0x00007FF84ECFA24E, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x282A24E byte(s), _ZThn8_N5clang9ASTWriterD1Ev() + 0x43C7E byte(s)
0x00007FF84DD11F88, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1841F88 byte(s), _ZN5clang4ento10CoreEngine15ExecuteWorkListEPKNS_15LocationContextEjN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEE() + 0x378 byte(s)
0x00007FF84C8C0B06, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x3F0B06 byte(s), _Z9EmitToIntRN5clang7CodeGen15CodeGenFunctionEPN4llvm5ValueENS_8QualTypeEPNS3_11IntegerTypeE() + 0x3D46D6 byte(s)
0x00007FF84C8C1E71, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x3F1E71 byte(s), _Z9EmitToIntRN5clang7CodeGen15CodeGenFunctionEPN4llvm5ValueENS_8QualTypeEPNS3_11IntegerTypeE() + 0x3D5A41 byte(s)
0x00007FF84D51876C, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x104876C byte(s), _ZN5clang17MultiplexConsumer21HandleTranslationUnitERNS_10ASTContextE() + 0x2C byte(s)
0x00007FF84E4F2587, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x2022587 byte(s), _ZN5clang8ParseASTERNS_4SemaEbb() + 0x287 byte(s)
0x00007FF84D35727F, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0xE8727F byte(s), _ZN5clang14FrontendAction7ExecuteEv() + 0x2F byte(s)
0x00007FF84D4D35C4, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x10035C4 byte(s), _ZN5clang16CompilerInstance13ExecuteActionERNS_14FrontendActionE() + 0x434 byte(s)
0x00007FF84E4D94CF, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x20094CF byte(s), _ZN5clang7tooling21FrontendActionFactory13runInvocationESt10shared_ptrINS_18CompilerInvocationEEPNS_11FileManagerES2_INS_22PCHContainerOperationsEEPNS_18DiagnosticConsumerE() + 0x15F byte(s)
0x00007FF6380F19CC, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x7C19CC byte(s)
0x00007FF84E4CAF76, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1FFAF76 byte(s), _ZN5clang7tooling14ToolInvocation13runInvocationEPKcPNS_6driver11CompilationESt10shared_ptrINS_18CompilerInvocationEES7_INS_22PCHContainerOperationsEE() + 0x136 byte(s)
0x00007FF84E4CB5A8, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1FFB5A8 byte(s), _ZN5clang7tooling14ToolInvocation3runEv() + 0x568 byte(s)
0x00007FF84E4EA862, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x201A862 byte(s), _ZN5clang7tooling9ClangTool3runEPNS0_10ToolActionE() + 0x1772 byte(s)
0x00007FF637D5B77A, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x42B77A byte(s)
0x00007FF637D5E91E, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x42E91E byte(s)
0x00007FF6379310D9, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x10D9 byte(s)
0x00007FF637931456, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x1456 byte(s)
0x00007FF9076A7374, C:\WINDOWS\System32\KERNEL32.DLL(0x00007FF907690000) + 0x17374 byte(s), BaseThreadInitThunk() + 0x14 byte(s)
0x00007FF90925CC91, C:\WINDOWS\SYSTEM32\ntdll.dll(0x00007FF909210000) + 0x4CC91 byte(s), RtlUserThreadStart() + 0x21 byte(s)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-tidy

Author: None (wobosoft)

<details>
### Command line used:
clang-tidy crash.cpp -checks='clang-analyzer-*' -- -std=c++26

### crash.cpp:
int strcmp(const char *s, char c); 

void trigger(const char* p) {
    if (strcmp(p, '(') == 0) { 
        // Crash happens here
    }
}

### Stack dump:
0.	Program arguments: D:\\msys64\\ucrt64\\bin\\clang-tidy.exe crash.cpp -checks=clang-analyzer-* -- -std=c++26
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling trigger(const char *)
3.	E:/vhdl/lib_tb_matfile/crash.cpp:5:9: Error evaluating statement
4.	E:/vhdl/lib_tb_matfile/crash.cpp:5:9: Error evaluating statement
Exception Code: 0xC0000005
0x00007FF84C945669, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x475669 byte(s), _Z9EmitToIntRN5clang7CodeGen15CodeGenFunctionEPN4llvm5ValueENS_8QualTypeEPNS3_11IntegerTypeE() + 0x459239 byte(s)
0x00007FF84DD41413, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1871413 byte(s), _ZN5clang4ento11SValBuilder9evalBinOpEN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEENS_18BinaryOperatorKindENS0_4SValES8_NS_8QualTypeE() + 0x293 byte(s)
0x00007FF84DD40EB7, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1870EB7 byte(s), _ZN5clang4ento11SValBuilder6evalEQEN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEENS0_20DefinedOrUnknownSValES7_() + 0xC7 byte(s)
0x00007FF84E69C07E, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x21CC07E byte(s), _ZNK12PNaClABIInfo9EmitVAArgERN5clang7CodeGen15CodeGenFunctionENS1_7AddressENS0_8QualTypeENS1_12AggValueSlotE() + 0x39F5E byte(s)
0x00007FF84E699081, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x21C9081 byte(s), _ZNK12PNaClABIInfo9EmitVAArgERN5clang7CodeGen15CodeGenFunctionENS1_7AddressENS0_8QualTypeENS1_12AggValueSlotE() + 0x36F61 byte(s)
0x00007FF84DDDD83A, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x190D83A byte(s), _ZN5clang4ento48shouldRegisterNullableReturnedFromNonnullCheckerERKNS0_14CheckerManagerE() + 0xF7A byte(s)
0x00007FF84DD56A43, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1886A43 byte(s), _ZN5clang4ento14CheckerManager22runCheckersForEvalCallERNS0_15ExplodedNodeSetERKS2_RKNS0_9CallEventERNS0_10ExprEngineERKNS0_15EvalCallOptionsE() + 0x1E3 byte(s)
0x00007FF84DD388F2, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x18688F2 byte(s), _ZN5clang4ento10ExprEngine8evalCallERNS0_15ExplodedNodeSetEPNS0_12ExplodedNodeERKNS0_9CallEventE() + 0x192 byte(s)
0x00007FF84DD15EC4, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1845EC4 byte(s), _ZN5clang4ento10ExprEngine13VisitCallExprEPKNS_8CallExprEPNS0_12ExplodedNodeERNS0_15ExplodedNodeSetE() + 0x1D4 byte(s)
0x00007FF84DD38011, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1868011 byte(s), _ZN5clang4ento10ExprEngine5VisitEPKNS_4StmtEPNS0_12ExplodedNodeERNS0_15ExplodedNodeSetE() + 0x2661 byte(s)
0x00007FF84DD15237, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1845237 byte(s), _ZN5clang4ento10ExprEngine11ProcessStmtEPKNS_4StmtEPNS0_12ExplodedNodeE() + 0x267 byte(s)
0x00007FF84DD13939, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1843939 byte(s), _ZN5clang4ento10CoreEngine16dispatchWorkItemEPNS0_12ExplodedNodeENS_12ProgramPointERKNS0_12WorkListUnitE() + 0x509 byte(s)
0x00007FF84ECFA24E, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x282A24E byte(s), _ZThn8_N5clang9ASTWriterD1Ev() + 0x43C7E byte(s)
0x00007FF84DD11F88, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1841F88 byte(s), _ZN5clang4ento10CoreEngine15ExecuteWorkListEPKNS_15LocationContextEjN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEE() + 0x378 byte(s)
0x00007FF84C8C0B06, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x3F0B06 byte(s), _Z9EmitToIntRN5clang7CodeGen15CodeGenFunctionEPN4llvm5ValueENS_8QualTypeEPNS3_11IntegerTypeE() + 0x3D46D6 byte(s)
0x00007FF84C8C1E71, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x3F1E71 byte(s), _Z9EmitToIntRN5clang7CodeGen15CodeGenFunctionEPN4llvm5ValueENS_8QualTypeEPNS3_11IntegerTypeE() + 0x3D5A41 byte(s)
0x00007FF84D51876C, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x104876C byte(s), _ZN5clang17MultiplexConsumer21HandleTranslationUnitERNS_10ASTContextE() + 0x2C byte(s)
0x00007FF84E4F2587, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x2022587 byte(s), _ZN5clang8ParseASTERNS_4SemaEbb() + 0x287 byte(s)
0x00007FF84D35727F, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0xE8727F byte(s), _ZN5clang14FrontendAction7ExecuteEv() + 0x2F byte(s)
0x00007FF84D4D35C4, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x10035C4 byte(s), _ZN5clang16CompilerInstance13ExecuteActionERNS_14FrontendActionE() + 0x434 byte(s)
0x00007FF84E4D94CF, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x20094CF byte(s), _ZN5clang7tooling21FrontendActionFactory13runInvocationESt10shared_ptrINS_18CompilerInvocationEEPNS_11FileManagerES2_INS_22PCHContainerOperationsEEPNS_18DiagnosticConsumerE() + 0x15F byte(s)
0x00007FF6380F19CC, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x7C19CC byte(s)
0x00007FF84E4CAF76, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1FFAF76 byte(s), _ZN5clang7tooling14ToolInvocation13runInvocationEPKcPNS_6driver11CompilationESt10shared_ptrINS_18CompilerInvocationEES7_INS_22PCHContainerOperationsEE() + 0x136 byte(s)
0x00007FF84E4CB5A8, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1FFB5A8 byte(s), _ZN5clang7tooling14ToolInvocation3runEv() + 0x568 byte(s)
0x00007FF84E4EA862, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x201A862 byte(s), _ZN5clang7tooling9ClangTool3runEPNS0_10ToolActionE() + 0x1772 byte(s)
0x00007FF637D5B77A, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x42B77A byte(s)
0x00007FF637D5E91E, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x42E91E byte(s)
0x00007FF6379310D9, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x10D9 byte(s)
0x00007FF637931456, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x1456 byte(s)
0x00007FF9076A7374, C:\WINDOWS\System32\KERNEL32.DLL(0x00007FF907690000) + 0x17374 byte(s), BaseThreadInitThunk() + 0x14 byte(s)
0x00007FF90925CC91, C:\WINDOWS\SYSTEM32\ntdll.dll(0x00007FF909210000) + 0x4CC91 byte(s), RtlUserThreadStart() + 0x21 byte(s)



</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-static-analyzer

Author: None (wobosoft)

<details>
### Command line used:
clang-tidy crash.cpp -checks='clang-analyzer-*' -- -std=c++26

### crash.cpp:
int strcmp(const char *s, char c); 

void trigger(const char* p) {
    if (strcmp(p, '(') == 0) { 
        // Crash happens here
    }
}

### Stack dump:
0.	Program arguments: D:\\msys64\\ucrt64\\bin\\clang-tidy.exe crash.cpp -checks=clang-analyzer-* -- -std=c++26
1.	&lt;eof&gt; parser at end of file
2.	While analyzing stack: 
	#<!-- -->0 Calling trigger(const char *)
3.	E:/vhdl/lib_tb_matfile/crash.cpp:5:9: Error evaluating statement
4.	E:/vhdl/lib_tb_matfile/crash.cpp:5:9: Error evaluating statement
Exception Code: 0xC0000005
0x00007FF84C945669, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x475669 byte(s), _Z9EmitToIntRN5clang7CodeGen15CodeGenFunctionEPN4llvm5ValueENS_8QualTypeEPNS3_11IntegerTypeE() + 0x459239 byte(s)
0x00007FF84DD41413, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1871413 byte(s), _ZN5clang4ento11SValBuilder9evalBinOpEN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEENS_18BinaryOperatorKindENS0_4SValES8_NS_8QualTypeE() + 0x293 byte(s)
0x00007FF84DD40EB7, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1870EB7 byte(s), _ZN5clang4ento11SValBuilder6evalEQEN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEENS0_20DefinedOrUnknownSValES7_() + 0xC7 byte(s)
0x00007FF84E69C07E, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x21CC07E byte(s), _ZNK12PNaClABIInfo9EmitVAArgERN5clang7CodeGen15CodeGenFunctionENS1_7AddressENS0_8QualTypeENS1_12AggValueSlotE() + 0x39F5E byte(s)
0x00007FF84E699081, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x21C9081 byte(s), _ZNK12PNaClABIInfo9EmitVAArgERN5clang7CodeGen15CodeGenFunctionENS1_7AddressENS0_8QualTypeENS1_12AggValueSlotE() + 0x36F61 byte(s)
0x00007FF84DDDD83A, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x190D83A byte(s), _ZN5clang4ento48shouldRegisterNullableReturnedFromNonnullCheckerERKNS0_14CheckerManagerE() + 0xF7A byte(s)
0x00007FF84DD56A43, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1886A43 byte(s), _ZN5clang4ento14CheckerManager22runCheckersForEvalCallERNS0_15ExplodedNodeSetERKS2_RKNS0_9CallEventERNS0_10ExprEngineERKNS0_15EvalCallOptionsE() + 0x1E3 byte(s)
0x00007FF84DD388F2, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x18688F2 byte(s), _ZN5clang4ento10ExprEngine8evalCallERNS0_15ExplodedNodeSetEPNS0_12ExplodedNodeERKNS0_9CallEventE() + 0x192 byte(s)
0x00007FF84DD15EC4, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1845EC4 byte(s), _ZN5clang4ento10ExprEngine13VisitCallExprEPKNS_8CallExprEPNS0_12ExplodedNodeERNS0_15ExplodedNodeSetE() + 0x1D4 byte(s)
0x00007FF84DD38011, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1868011 byte(s), _ZN5clang4ento10ExprEngine5VisitEPKNS_4StmtEPNS0_12ExplodedNodeERNS0_15ExplodedNodeSetE() + 0x2661 byte(s)
0x00007FF84DD15237, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1845237 byte(s), _ZN5clang4ento10ExprEngine11ProcessStmtEPKNS_4StmtEPNS0_12ExplodedNodeE() + 0x267 byte(s)
0x00007FF84DD13939, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1843939 byte(s), _ZN5clang4ento10CoreEngine16dispatchWorkItemEPNS0_12ExplodedNodeENS_12ProgramPointERKNS0_12WorkListUnitE() + 0x509 byte(s)
0x00007FF84ECFA24E, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x282A24E byte(s), _ZThn8_N5clang9ASTWriterD1Ev() + 0x43C7E byte(s)
0x00007FF84DD11F88, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1841F88 byte(s), _ZN5clang4ento10CoreEngine15ExecuteWorkListEPKNS_15LocationContextEjN4llvm18IntrusiveRefCntPtrIKNS0_12ProgramStateEEE() + 0x378 byte(s)
0x00007FF84C8C0B06, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x3F0B06 byte(s), _Z9EmitToIntRN5clang7CodeGen15CodeGenFunctionEPN4llvm5ValueENS_8QualTypeEPNS3_11IntegerTypeE() + 0x3D46D6 byte(s)
0x00007FF84C8C1E71, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x3F1E71 byte(s), _Z9EmitToIntRN5clang7CodeGen15CodeGenFunctionEPN4llvm5ValueENS_8QualTypeEPNS3_11IntegerTypeE() + 0x3D5A41 byte(s)
0x00007FF84D51876C, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x104876C byte(s), _ZN5clang17MultiplexConsumer21HandleTranslationUnitERNS_10ASTContextE() + 0x2C byte(s)
0x00007FF84E4F2587, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x2022587 byte(s), _ZN5clang8ParseASTERNS_4SemaEbb() + 0x287 byte(s)
0x00007FF84D35727F, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0xE8727F byte(s), _ZN5clang14FrontendAction7ExecuteEv() + 0x2F byte(s)
0x00007FF84D4D35C4, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x10035C4 byte(s), _ZN5clang16CompilerInstance13ExecuteActionERNS_14FrontendActionE() + 0x434 byte(s)
0x00007FF84E4D94CF, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x20094CF byte(s), _ZN5clang7tooling21FrontendActionFactory13runInvocationESt10shared_ptrINS_18CompilerInvocationEEPNS_11FileManagerES2_INS_22PCHContainerOperationsEEPNS_18DiagnosticConsumerE() + 0x15F byte(s)
0x00007FF6380F19CC, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x7C19CC byte(s)
0x00007FF84E4CAF76, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1FFAF76 byte(s), _ZN5clang7tooling14ToolInvocation13runInvocationEPKcPNS_6driver11CompilationESt10shared_ptrINS_18CompilerInvocationEES7_INS_22PCHContainerOperationsEE() + 0x136 byte(s)
0x00007FF84E4CB5A8, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x1FFB5A8 byte(s), _ZN5clang7tooling14ToolInvocation3runEv() + 0x568 byte(s)
0x00007FF84E4EA862, D:\msys64\ucrt64\bin\libclang-cpp.dll(0x00007FF84C4D0000) + 0x201A862 byte(s), _ZN5clang7tooling9ClangTool3runEPNS0_10ToolActionE() + 0x1772 byte(s)
0x00007FF637D5B77A, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x42B77A byte(s)
0x00007FF637D5E91E, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x42E91E byte(s)
0x00007FF6379310D9, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x10D9 byte(s)
0x00007FF637931456, D:\msys64\ucrt64\bin\clang-tidy.exe(0x00007FF637930000) + 0x1456 byte(s)
0x00007FF9076A7374, C:\WINDOWS\System32\KERNEL32.DLL(0x00007FF907690000) + 0x17374 byte(s), BaseThreadInitThunk() + 0x14 byte(s)
0x00007FF90925CC91, C:\WINDOWS\SYSTEM32\ntdll.dll(0x00007FF909210000) + 0x4CC91 byte(s), RtlUserThreadStart() + 0x21 byte(s)



</details>


---

### Comment 3 - wobosoft

No, it is not that urgent.
Thanks for fixing!

Wobosoft

---

