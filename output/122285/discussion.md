# clang crashes on generating pre-compiled header chain with -std=c++20

**Author:** sushinskiy
**URL:** https://github.com/llvm/llvm-project/issues/122285

## Body

It is reproduced when I run chain of commands:

```
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cstdio" -o C:\project\pch-crash\cstdio.pch 
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cstdlib" -o C:\project\pch-crash\cstdlib.pch -include-pch C:\project\pch-crash\cstdio.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cstring" -o C:\project\pch-crash\cstring.pch -include-pch C:\project\pch-crash\cstdlib.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\string" -o C:\project\pch-crash\string.pch -include-pch C:\project\pch-crash\cstring.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector" -o C:\project\pch-crash\vector.pch -include-pch C:\project\pch-crash\string.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\limits" -o C:\project\pch-crash\limits.pch -include-pch C:\project\pch-crash\vector.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\iostream" -o C:\project\pch-crash\iostream.pch -include-pch C:\project\pch-crash\limits.pch
```

**Note**: If I use -std=c++17 instead of -std=c++20 there are no issues.

Stack dump:
0.      Program arguments: D:\\project\\llvm-project\\build64\\Debug\\bin\\clang++.exe -std=c++20 "-IC:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.38.33130\\include" -x c++-header "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.38.33130\\include\\iostream" -o C:\\project\\pch-crash\\iostream.pch -include-pch C:\\project\\pch-crash\\limits.pch
Exception Code: 0xC0000005
0x00007FF6D06079B4, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83379B4 byte(s), clang::ASTReader::getLocalModuleFile() + 0x104 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 9090 + 0x2E byte(s)
0x00007FF6D05C9712, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x82F9712 byte(s), clang::serialization::reader::ASTDeclContextNameLookupTrait::ReadFileRef() + 0x42 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 1213 + 0x0 byte(s)
0x00007FF6D08705FF, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x85A05FF byte(s), clang::serialization::MultiOnDiskHashTable<clang::serialization::reader::ASTDeclContextNameLookupTrait>::add() + 0xAF byte(s), D:\project\llvm-project\clang\lib\Serialization\MultiOnDiskHashTable.h, line 213 + 0x12 byte(s)
0x00007FF6D07EAC29, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x851AC29 byte(s), clang::ASTReader::loadDeclUpdateRecords() + 0xA59 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReaderDecl.cpp, line 4333 + 0x0 byte(s)
0x00007FF6D05E95A8, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83195A8 byte(s), clang::ASTReader::finishPendingActions() + 0xB98 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 9762 + 0x0 byte(s)
0x00007FF6D06006A7, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83306A7 byte(s), clang::ASTReader::FinishedDeserializing() + 0x97 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 10231 + 0x0 byte(s)
0x00007FF6D06B0B43, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83E0B43 byte(s), clang::ExternalASTSource::Deserializing::~Deserializing() + 0x23 byte(s), D:\project\llvm-project\clang\include\clang\AST\ExternalASTSource.h, line 87 + 0x0 byte(s)
0x00007FF6D05F02F6, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83202F6 byte(s), clang::ASTReader::ReadAST() + 0x1786 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 4670 + 0x25 byte(s)
0x00007FF6D01D26A6, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7F026A6 byte(s), clang::CompilerInstance::createPCHExternalASTSource() + 0x636 byte(s), D:\project\llvm-project\clang\lib\Frontend\CompilerInstance.cpp, line 659 + 0x109 byte(s)
0x00007FF6D01D2041, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7F02041 byte(s), clang::CompilerInstance::createPCHExternalASTSource() + 0x611 byte(s), D:\project\llvm-project\clang\lib\Frontend\CompilerInstance.cpp, line 615 + 0x5B7 byte(s)
0x00007FF6D020D87B, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7F3D87B byte(s), clang::FrontendAction::BeginSourceFile() + 0x37DB byte(s), D:\project\llvm-project\clang\lib\Frontend\FrontendAction.cpp, line 999 + 0x0 byte(s)
0x00007FF6D01CE4C2, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7EFE4C2 byte(s), clang::CompilerInstance::ExecuteAction() + 0x422 byte(s), D:\project\llvm-project\clang\lib\Frontend\CompilerInstance.cpp, line 1060 + 0x1D byte(s)
0x00007FF6D04B2320, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x81E2320 byte(s), clang::ExecuteCompilerInvocation() + 0x440 byte(s), D:\project\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp, line 280 + 0x1A byte(s)
0x00007FF6C88B1EED, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5E1EED byte(s), cc1_main() + 0x9BD byte(s), D:\project\llvm-project\clang\tools\driver\cc1_main.cpp, line 284 + 0x12 byte(s)
0x00007FF6C889C0BB, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5CC0BB byte(s), ExecuteCC1Tool() + 0x2DB byte(s), D:\project\llvm-project\clang\tools\driver\driver.cpp, line 215 + 0xA1 byte(s)
0x00007FF6C889E261, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5CE261 byte(s), `clang_main'::`43'::<lambda_1>::operator()() + 0x21 byte(s), D:\project\llvm-project\clang\tools\driver\driver.cpp, line 356 + 0x0 byte(s)
0x00007FF6C88A1EDE, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5D1EDE byte(s), llvm::function_ref<int __cdecl(llvm::SmallVectorImpl<char const *> &)>::callback_fn<`clang_main'::`43'::<lambda_1> >() + 0x1E byte(s), D:\project\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 47 + 0x0 byte(s)
0x00007FF6CFF25F8E, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7C55F8E byte(s), llvm::function_ref<int __cdecl(llvm::SmallVectorImpl<char const *> &)>::operator()() + 0x2E byte(s), D:\project\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 69 + 0x0 byte(s)
0x00007FF6CFF21CD1, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7C51CD1 byte(s), `clang::driver::CC1Command::Execute'::`7'::<lambda_1>::operator()() + 0x31 byte(s), D:\project\llvm-project\clang\lib\Driver\Job.cpp, line 440 + 0x31 byte(s)
0x00007FF6CFF25114, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7C55114 byte(s), llvm::function_ref<void __cdecl(void)>::callback_fn<`clang::driver::CC1Command::Execute'::`7'::<lambda_1> >() + 0x14 byte(s), D:\project\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 47 + 0x0 byte(s)
0x00007FF6CC7B15E4, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x44E15E4 byte(s), llvm::function_ref<void __cdecl(void)>::operator()() + 0x24 byte(s), D:\project\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 69 + 0x0 byte(s)
0x00007FF6CE6BE93B, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x63EE93B byte(s), llvm::CrashRecoveryContext::RunSafely() + 0xAB byte(s), D:\project\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp, line 235 + 0xA byte(s)
0x00007FF6CFF1FC74, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7C4FC74 byte(s), clang::driver::CC1Command::Execute() + 0x214 byte(s), D:\project\llvm-project\clang\lib\Driver\Job.cpp, line 440 + 0x72 byte(s)
0x00007FF6CFDD618E, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7B0618E byte(s), clang::driver::Compilation::ExecuteCommand() + 0x44E byte(s), D:\project\llvm-project\clang\lib\Driver\Compilation.cpp, line 199 + 0x9E byte(s)
0x00007FF6CFDD640A, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7B0640A byte(s), clang::driver::Compilation::ExecuteJobs() + 0xDA byte(s), D:\project\llvm-project\clang\lib\Driver\Compilation.cpp, line 253 + 0x20 byte(s)
0x00007FF6CFD6A64B, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7A9A64B byte(s), clang::driver::Driver::ExecuteCompilation() + 0x2BB byte(s), D:\project\llvm-project\clang\lib\Driver\Driver.cpp, line 1943 + 0x0 byte(s)
0x00007FF6C889DE49, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5CDE49 byte(s), clang_main() + 0x1989 byte(s), D:\project\llvm-project\clang\tools\driver\driver.cpp, line 391 + 0x25 byte(s)
0x00007FF6C88E77E5, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x6177E5 byte(s), main() + 0xA5 byte(s), D:\project\llvm-project\build64\tools\clang\tools\driver\clang-driver.cpp, line 17 + 0x4D byte(s)
0x00007FF6D8CAB2E9, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x109DB2E9 byte(s), invoke_main() + 0x39 byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl, line 79 + 0x0 byte(s)
0x00007FF6D8CAB1CE, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x109DB1CE byte(s), __scrt_common_main_seh() + 0x12E byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl, line 288 + 0x5 byte(s)
0x00007FF6D8CAB08E, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x109DB08E byte(s), __scrt_common_main() + 0xE byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl, line 331 + 0x0 byte(s)
0x00007FF6D8CAB37E, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x109DB37E byte(s), mainCRTStartup() + 0xE byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp, line 17 + 0x0 byte(s)
0x00007FF8D8A87374, C:\Windows\System32\KERNEL32.DLL(0x00007FF8D8A70000) + 0x17374 byte(s), BaseThreadInitThunk() + 0x14 byte(s)
0x00007FF8DA87CC91, C:\Windows\SYSTEM32\ntdll.dll(0x00007FF8DA830000) + 0x4CC91 byte(s), RtlUserThreadStart() + 0x21 byte(s)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.0-rc4 (https://github.com/llvm/llvm-project.git 0c641568515a797473394694f05937e1f1913d87)
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: D:\project\llvm-project\build64\Debug\bin
Build config: +unoptimized, +assertions
clang++: note: diagnostic msg:
********************

Also attached the following files:
iostream-d10074.hh
iostream-d10074.sh
in zip:
[clang-crash.zip](https://github.com/user-attachments/files/18363239/clang-crash.zip)






## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (sushinskiy)

<details>
It is reproduced when I run chain of commands:

```
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cstdio" -o C:\project\pch-crash\cstdio.pch 
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cstdlib" -o C:\project\pch-crash\cstdlib.pch -include-pch C:\project\pch-crash\cstdio.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cstring" -o C:\project\pch-crash\cstring.pch -include-pch C:\project\pch-crash\cstdlib.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\string" -o C:\project\pch-crash\string.pch -include-pch C:\project\pch-crash\cstring.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector" -o C:\project\pch-crash\vector.pch -include-pch C:\project\pch-crash\string.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\limits" -o C:\project\pch-crash\limits.pch -include-pch C:\project\pch-crash\vector.pch
D:\project\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\iostream" -o C:\project\pch-crash\iostream.pch -include-pch C:\project\pch-crash\limits.pch
```

**Note**: If I use -std=c++17 instead of -std=c++20 there are no issues.

Stack dump:
0.      Program arguments: D:\\project\\llvm-project\\build64\\Debug\\bin\\clang++.exe -std=c++20 "-IC:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.38.33130\\include" -x c++-header "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.38.33130\\include\\iostream" -o C:\\project\\pch-crash\\iostream.pch -include-pch C:\\project\\pch-crash\\limits.pch
Exception Code: 0xC0000005
0x00007FF6D06079B4, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83379B4 byte(s), clang::ASTReader::getLocalModuleFile() + 0x104 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 9090 + 0x2E byte(s)
0x00007FF6D05C9712, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x82F9712 byte(s), clang::serialization::reader::ASTDeclContextNameLookupTrait::ReadFileRef() + 0x42 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 1213 + 0x0 byte(s)
0x00007FF6D08705FF, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x85A05FF byte(s), clang::serialization::MultiOnDiskHashTable&lt;clang::serialization::reader::ASTDeclContextNameLookupTrait&gt;::add() + 0xAF byte(s), D:\project\llvm-project\clang\lib\Serialization\MultiOnDiskHashTable.h, line 213 + 0x12 byte(s)
0x00007FF6D07EAC29, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x851AC29 byte(s), clang::ASTReader::loadDeclUpdateRecords() + 0xA59 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReaderDecl.cpp, line 4333 + 0x0 byte(s)
0x00007FF6D05E95A8, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83195A8 byte(s), clang::ASTReader::finishPendingActions() + 0xB98 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 9762 + 0x0 byte(s)
0x00007FF6D06006A7, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83306A7 byte(s), clang::ASTReader::FinishedDeserializing() + 0x97 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 10231 + 0x0 byte(s)
0x00007FF6D06B0B43, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83E0B43 byte(s), clang::ExternalASTSource::Deserializing::~Deserializing() + 0x23 byte(s), D:\project\llvm-project\clang\include\clang\AST\ExternalASTSource.h, line 87 + 0x0 byte(s)
0x00007FF6D05F02F6, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x83202F6 byte(s), clang::ASTReader::ReadAST() + 0x1786 byte(s), D:\project\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 4670 + 0x25 byte(s)
0x00007FF6D01D26A6, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7F026A6 byte(s), clang::CompilerInstance::createPCHExternalASTSource() + 0x636 byte(s), D:\project\llvm-project\clang\lib\Frontend\CompilerInstance.cpp, line 659 + 0x109 byte(s)
0x00007FF6D01D2041, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7F02041 byte(s), clang::CompilerInstance::createPCHExternalASTSource() + 0x611 byte(s), D:\project\llvm-project\clang\lib\Frontend\CompilerInstance.cpp, line 615 + 0x5B7 byte(s)
0x00007FF6D020D87B, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7F3D87B byte(s), clang::FrontendAction::BeginSourceFile() + 0x37DB byte(s), D:\project\llvm-project\clang\lib\Frontend\FrontendAction.cpp, line 999 + 0x0 byte(s)
0x00007FF6D01CE4C2, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7EFE4C2 byte(s), clang::CompilerInstance::ExecuteAction() + 0x422 byte(s), D:\project\llvm-project\clang\lib\Frontend\CompilerInstance.cpp, line 1060 + 0x1D byte(s)
0x00007FF6D04B2320, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x81E2320 byte(s), clang::ExecuteCompilerInvocation() + 0x440 byte(s), D:\project\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp, line 280 + 0x1A byte(s)
0x00007FF6C88B1EED, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5E1EED byte(s), cc1_main() + 0x9BD byte(s), D:\project\llvm-project\clang\tools\driver\cc1_main.cpp, line 284 + 0x12 byte(s)
0x00007FF6C889C0BB, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5CC0BB byte(s), ExecuteCC1Tool() + 0x2DB byte(s), D:\project\llvm-project\clang\tools\driver\driver.cpp, line 215 + 0xA1 byte(s)
0x00007FF6C889E261, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5CE261 byte(s), `clang_main'::`43'::&lt;lambda_1&gt;::operator()() + 0x21 byte(s), D:\project\llvm-project\clang\tools\driver\driver.cpp, line 356 + 0x0 byte(s)
0x00007FF6C88A1EDE, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5D1EDE byte(s), llvm::function_ref&lt;int __cdecl(llvm::SmallVectorImpl&lt;char const *&gt; &amp;)&gt;::callback_fn&lt;`clang_main'::`43'::&lt;lambda_1&gt; &gt;() + 0x1E byte(s), D:\project\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 47 + 0x0 byte(s)
0x00007FF6CFF25F8E, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7C55F8E byte(s), llvm::function_ref&lt;int __cdecl(llvm::SmallVectorImpl&lt;char const *&gt; &amp;)&gt;::operator()() + 0x2E byte(s), D:\project\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 69 + 0x0 byte(s)
0x00007FF6CFF21CD1, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7C51CD1 byte(s), `clang::driver::CC1Command::Execute'::`7'::&lt;lambda_1&gt;::operator()() + 0x31 byte(s), D:\project\llvm-project\clang\lib\Driver\Job.cpp, line 440 + 0x31 byte(s)
0x00007FF6CFF25114, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7C55114 byte(s), llvm::function_ref&lt;void __cdecl(void)&gt;::callback_fn&lt;`clang::driver::CC1Command::Execute'::`7'::&lt;lambda_1&gt; &gt;() + 0x14 byte(s), D:\project\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 47 + 0x0 byte(s)
0x00007FF6CC7B15E4, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x44E15E4 byte(s), llvm::function_ref&lt;void __cdecl(void)&gt;::operator()() + 0x24 byte(s), D:\project\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 69 + 0x0 byte(s)
0x00007FF6CE6BE93B, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x63EE93B byte(s), llvm::CrashRecoveryContext::RunSafely() + 0xAB byte(s), D:\project\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp, line 235 + 0xA byte(s)
0x00007FF6CFF1FC74, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7C4FC74 byte(s), clang::driver::CC1Command::Execute() + 0x214 byte(s), D:\project\llvm-project\clang\lib\Driver\Job.cpp, line 440 + 0x72 byte(s)
0x00007FF6CFDD618E, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7B0618E byte(s), clang::driver::Compilation::ExecuteCommand() + 0x44E byte(s), D:\project\llvm-project\clang\lib\Driver\Compilation.cpp, line 199 + 0x9E byte(s)
0x00007FF6CFDD640A, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7B0640A byte(s), clang::driver::Compilation::ExecuteJobs() + 0xDA byte(s), D:\project\llvm-project\clang\lib\Driver\Compilation.cpp, line 253 + 0x20 byte(s)
0x00007FF6CFD6A64B, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x7A9A64B byte(s), clang::driver::Driver::ExecuteCompilation() + 0x2BB byte(s), D:\project\llvm-project\clang\lib\Driver\Driver.cpp, line 1943 + 0x0 byte(s)
0x00007FF6C889DE49, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x5CDE49 byte(s), clang_main() + 0x1989 byte(s), D:\project\llvm-project\clang\tools\driver\driver.cpp, line 391 + 0x25 byte(s)
0x00007FF6C88E77E5, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x6177E5 byte(s), main() + 0xA5 byte(s), D:\project\llvm-project\build64\tools\clang\tools\driver\clang-driver.cpp, line 17 + 0x4D byte(s)
0x00007FF6D8CAB2E9, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x109DB2E9 byte(s), invoke_main() + 0x39 byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl, line 79 + 0x0 byte(s)
0x00007FF6D8CAB1CE, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x109DB1CE byte(s), __scrt_common_main_seh() + 0x12E byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl, line 288 + 0x5 byte(s)
0x00007FF6D8CAB08E, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x109DB08E byte(s), __scrt_common_main() + 0xE byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl, line 331 + 0x0 byte(s)
0x00007FF6D8CAB37E, D:\project\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF6C82D0000) + 0x109DB37E byte(s), mainCRTStartup() + 0xE byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp, line 17 + 0x0 byte(s)
0x00007FF8D8A87374, C:\Windows\System32\KERNEL32.DLL(0x00007FF8D8A70000) + 0x17374 byte(s), BaseThreadInitThunk() + 0x14 byte(s)
0x00007FF8DA87CC91, C:\Windows\SYSTEM32\ntdll.dll(0x00007FF8DA830000) + 0x4CC91 byte(s), RtlUserThreadStart() + 0x21 byte(s)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 19.1.0-rc4 (https://github.com/llvm/llvm-project.git 0c641568515a797473394694f05937e1f1913d87)
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: D:\project\llvm-project\build64\Debug\bin
Build config: +unoptimized, +assertions
clang++: note: diagnostic msg:
********************

Also attached the following files:
iostream-d10074.hh
iostream-d10074.sh
in zip:
[clang-crash.zip](https://github.com/user-attachments/files/18363239/clang-crash.zip)





</details>


---

### Comment 2 - EugeneZelenko

Could you please try most recent 19 or `main` branch?

---

### Comment 3 - sushinskiy

> Could you please try most recent 19 or `main` branch?

The problem still remains on the 'main' branch
```

Stack dump:
0.      Program arguments: D:\\llvm-project\\build64\\Debug\\bin\\clang++.exe -std=c++20 "-IC:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.38.33130\\include" -x c++-header "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.38.33130\\include\\iostream" -o C:\\project\\pch-crash\\iostream.pch -include-pch C:\\project\\pch-crash\\limits.pch
Exception Code: 0xC0000005
0x00007FF66E584BB4, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x8A34BB4 byte(s), clang::ASTReader::getLocalModuleFile() + 0x104 byte(s), D:\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 9461 + 0x2E byte(s)
0x00007FF66E5430B2, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x89F30B2 byte(s), clang::serialization::reader::ASTDeclContextNameLookupTrait::ReadFileRef() + 0x42 byte(s), D:\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 1244 + 0x0 byte(s)
0x00007FF66E8014DF, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x8CB14DF byte(s), clang::serialization::MultiOnDiskHashTable<clang::serialization::reader::ASTDeclContextNameLookupTrait>::add() + 0xAF byte(s), D:\llvm-project\clang\lib\Serialization\MultiOnDiskHashTable.h, line 213 + 0x12 byte(s)
0x00007FF66E77C67F, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x8C2C67F byte(s), clang::ASTReader::loadDeclUpdateRecords() + 0x93F byte(s), D:\llvm-project\clang\lib\Serialization\ASTReaderDecl.cpp, line 4329 + 0x0 byte(s)
0x00007FF66E565408, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x8A15408 byte(s), clang::ASTReader::finishPendingActions() + 0xB98 byte(s), D:\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 10141 + 0x0 byte(s)
0x00007FF66E57D5B7, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x8A2D5B7 byte(s), clang::ASTReader::FinishedDeserializing() + 0x97 byte(s), D:\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 10636 + 0x0 byte(s)
0x00007FF66E638DB3, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x8AE8DB3 byte(s), clang::ExternalASTSource::Deserializing::~Deserializing() + 0x23 byte(s), D:\llvm-project\clang\include\clang\AST\ExternalASTSource.h, line 89 + 0x0 byte(s)
0x00007FF66E56CAD1, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x8A1CAD1 byte(s), clang::ASTReader::ReadAST() + 0x17D1 byte(s), D:\llvm-project\clang\lib\Serialization\ASTReader.cpp, line 4804 + 0x25 byte(s)
0x00007FF66E10D156, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x85BD156 byte(s), clang::CompilerInstance::createPCHExternalASTSource() + 0x636 byte(s), D:\llvm-project\clang\lib\Frontend\CompilerInstance.cpp, line 660 + 0x109 byte(s)
0x00007FF66E10CAF1, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x85BCAF1 byte(s), clang::CompilerInstance::createPCHExternalASTSource() + 0x611 byte(s), D:\llvm-project\clang\lib\Frontend\CompilerInstance.cpp, line 616 + 0x5B7 byte(s)
0x00007FF66E14862D, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x85F862D byte(s), clang::FrontendAction::BeginSourceFile() + 0x38FD byte(s), D:\llvm-project\clang\lib\Frontend\FrontendAction.cpp, line 998 + 0x0 byte(s)
0x00007FF66E108EA2, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x85B8EA2 byte(s), clang::CompilerInstance::ExecuteAction() + 0x422 byte(s), D:\llvm-project\clang\lib\Frontend\CompilerInstance.cpp, line 1061 + 0x1D byte(s)
0x00007FF66E40E020, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x88BE020 byte(s), clang::ExecuteCompilerInvocation() + 0x440 byte(s), D:\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp, line 296 + 0x1A byte(s)
0x00007FF66618D176, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x63D176 byte(s), cc1_main() + 0x9F6 byte(s), D:\llvm-project\clang\tools\driver\cc1_main.cpp, line 286 + 0x12 byte(s)
0x00007FF66617730B, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x62730B byte(s), ExecuteCC1Tool() + 0x2DB byte(s), D:\llvm-project\clang\tools\driver\driver.cpp, line 218 + 0xA1 byte(s)
0x00007FF666179511, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x629511 byte(s), `clang_main'::`2'::<lambda_1>::operator()() + 0x21 byte(s), D:\llvm-project\clang\tools\driver\driver.cpp, line 361 + 0x0 byte(s)
0x00007FF66617D19E, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x62D19E byte(s), llvm::function_ref<int __cdecl(llvm::SmallVectorImpl<char const *> &)>::callback_fn<`clang_main'::`2'::<lambda_1> >() + 0x1E byte(s), D:\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 48 + 0x0 byte(s)
0x00007FF66DE62EEE, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x8312EEE byte(s), llvm::function_ref<int __cdecl(llvm::SmallVectorImpl<char const *> &)>::operator()() + 0x2E byte(s), D:\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 70 + 0x0 byte(s)
0x00007FF66DE5EE81, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x830EE81 byte(s), `clang::driver::CC1Command::Execute'::`7'::<lambda_1>::operator()() + 0x31 byte(s), D:\llvm-project\clang\lib\Driver\Job.cpp, line 437 + 0x31 byte(s)
0x00007FF66DE62134, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x8312134 byte(s), llvm::function_ref<void __cdecl(void)>::callback_fn<`clang::driver::CC1Command::Execute'::`7'::<lambda_1> >() + 0x14 byte(s), D:\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 48 + 0x0 byte(s)
0x00007FF66A2F16F4, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x47A16F4 byte(s), llvm::function_ref<void __cdecl(void)>::operator()() + 0x24 byte(s), D:\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h, line 70 + 0x0 byte(s)
0x00007FF66C2F364B, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x67A364B byte(s), llvm::CrashRecoveryContext::RunSafely() + 0xAB byte(s), D:\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp, line 235 + 0xA byte(s)
0x00007FF66DE5CE24, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x830CE24 byte(s), clang::driver::CC1Command::Execute() + 0x214 byte(s), D:\llvm-project\clang\lib\Driver\Job.cpp, line 437 + 0x72 byte(s)
0x00007FF66DD0551E, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x81B551E byte(s), clang::driver::Compilation::ExecuteCommand() + 0x44E byte(s), D:\llvm-project\clang\lib\Driver\Compilation.cpp, line 196 + 0x9E byte(s)
0x00007FF66DD0579A, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x81B579A byte(s), clang::driver::Compilation::ExecuteJobs() + 0xDA byte(s), D:\llvm-project\clang\lib\Driver\Compilation.cpp, line 251 + 0x20 byte(s)
0x00007FF66DC9589D, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x814589D byte(s), clang::driver::Driver::ExecuteCompilation() + 0x2CD byte(s), D:\llvm-project\clang\lib\Driver\Driver.cpp, line 2124 + 0x0 byte(s)
0x00007FF6661790F5, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x6290F5 byte(s), clang_main() + 0x19E5 byte(s), D:\llvm-project\clang\tools\driver\driver.cpp, line 398 + 0x25 byte(s)
0x00007FF6661C0DC5, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0x670DC5 byte(s), main() + 0xA5 byte(s), D:\llvm-project\build64\tools\clang\tools\driver\clang-driver.cpp, line 17 + 0x4D byte(s)
0x00007FF675720B99, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0xFBD0B99 byte(s), invoke_main() + 0x39 byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl, line 79 + 0x0 byte(s)
0x00007FF675720A7E, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0xFBD0A7E byte(s), __scrt_common_main_seh() + 0x12E byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl, line 288 + 0x5 byte(s)
0x00007FF67572093E, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0xFBD093E byte(s), __scrt_common_main() + 0xE byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl, line 331 + 0x0 byte(s)
0x00007FF675720C2E, D:\llvm-project\build64\Debug\bin\clang++.exe(0x00007FF665B50000) + 0xFBD0C2E byte(s), mainCRTStartup() + 0xE byte(s), D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_main.cpp, line 17 + 0x0 byte(s)
0x00007FF8D8A87374, C:\Windows\System32\KERNEL32.DLL(0x00007FF8D8A70000) + 0x17374 byte(s), BaseThreadInitThunk() + 0x14 byte(s)
0x00007FF8DA87CC91, C:\Windows\SYSTEM32\ntdll.dll(0x00007FF8DA830000) + 0x4CC91 byte(s), RtlUserThreadStart() + 0x21 byte(s)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git 9d5299eb61a64cd4df5fefa0299b0cf8d917978f)
Target: x86_64-pc-windows-msvc
Thread model: posix
InstalledDir: D:\llvm-project\build64\Debug\bin
Build config: +unoptimized, +assertions
clang++: note: diagnostic msg:
********************


```
Preprocessed source(s) and associated run script(s):
[pp_and_script.zip](https://github.com/user-attachments/files/18376941/pp_and_script.zip)



---

### Comment 4 - sushinskiy

I also noticed that if I rotate files in the chain, the problem remains. However, the crash occurs on the other file, but always on the 7th one.
For example, the following list of commands leads to it also:

```
D:\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\limits" -o C:\project\pch-crash\limits.pch
D:\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\iostream" -o C:\project\pch-crash\iostream.pch -include-pch C:\project\pch-crash\limits.pch
D:\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cstdio" -o C:\project\pch-crash\cstdio.pch -include-pch C:\project\pch-crash\iostream.pch
D:\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cstdlib" -o C:\project\pch-crash\cstdlib.pch -include-pch C:\project\pch-crash\cstdio.pch
D:\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\cstring" -o C:\project\pch-crash\cstring.pch -include-pch C:\project\pch-crash\cstdlib.pch
D:\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\string" -o C:\project\pch-crash\string.pch -include-pch C:\project\pch-crash\cstring.pch
D:\llvm-project\build64\Debug\bin\clang++.exe -std=c++20 -I"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include" -x c++-header "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.38.33130\include\vector" -o C:\project\pch-crash\vector.pch -include-pch C:\project\pch-crash\string.pch
```

Moreover, I've tried to experiment with 'boost' library header files and got the same issue. The crash occurs on a 7th file as well. It seems, that regardless of file input content, there is a limitation on the number of files in a PCH-chain.

---

### Comment 5 - koplas

@sushinskiy Can you try the patch that is located here: https://github.com/llvm/llvm-project/issues/105994#issuecomment-2323585850?

---

