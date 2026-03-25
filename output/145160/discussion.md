# [MicrosoftCXXNameMangler] Crash on const struct

**Author:** kasper93
**URL:** https://github.com/llvm/llvm-project/issues/145160

## Body

Hi

I noticed that ffmpeg cannot be built with asan on Windows, turns out there is `typedef const struct ...` in one of the files, which crashes the mangler when used in function. Would be nice to not crash.

I tested with v19.1.5 and HEAD, the following report is from be7e4113c84317feda055cac68a855429e46e381.

### Command
`clang --target=x86_64-pc-windows-msvc -fsanitize=function -c r.c`

### Source code 
``` c
typedef const struct {
    int i;
} a;
void b(a c) {}
```

### Result

```
Stack dump:
0.      Program arguments: E:\\dev\\llvm-project\\build\\bin\\clang.exe --target=x86_64-pc-windows-msvc -fsanitize=function -c r.c
1.      <eof> parser at end of file
2.      r.c:4:6: LLVM IR generation of declaration 'b'
3.      r.c:4:6: Generating code for declaration 'b'
Exception Code: 0xC0000005
 #0 0x00007ff7a0407757 clang::ASTContext::getDeclaratorForUnnamedTagDecl(class clang::TagDecl const *) E:\dev\llvm-project\clang\lib\AST\ASTContext.cpp:13439:0
 #1 0x00007ff7a08f40ba `anonymous namespace'::MicrosoftCXXNameMangler::mangleUnqualifiedName E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:1287:0
 #2 0x00007ff7a08e43bf `anonymous namespace'::MicrosoftCXXNameMangler::mangleName E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:973:0
 #3 0x00007ff7a08f11c4 `anonymous namespace'::MicrosoftCXXNameMangler::mangleType E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:3260:0
 #4 0x00007ff7a08f2b61 `anonymous namespace'::MicrosoftCXXNameMangler::mangleType E:\dev\llvm-project\build\tools\clang\include\clang\AST\TypeNodes.inc:77:0
 #5 0x00007ff7a08e1eb8 `anonymous namespace'::MicrosoftCXXNameMangler::mangleFunctionArgumentType E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:2417:0
 #6 0x00007ff7a08e2f67 `anonymous namespace'::MicrosoftCXXNameMangler::mangleFunctionType E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:3050:0
 #7 0x00007ff7a08f2867 `anonymous namespace'::MicrosoftCXXNameMangler::mangleType E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:2605:0
 #8 0x00007ff7a08e1850 `anonymous namespace'::MicrosoftMangleContextImpl::mangleCanonicalTypeName E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:4150:0
 #9 0x00007ff79dcc2174 std::basic_string<char,std::char_traits<char>,std::allocator<char> >::data C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.43.34808\include\xstring:2332:0
#10 0x00007ff79dcc2174 llvm::StringRef::{ctor} E:\dev\llvm-project\llvm\include\llvm\ADT\StringRef.h:108:0
#11 0x00007ff79dcc2174 clang::CodeGen::CodeGenFunction::getUBSanFunctionTypeHash(class clang::QualType) const E:\dev\llvm-project\clang\lib\CodeGen\CodeGenFunction.cpp:623:0
#12 0x00007ff79dcbc22f clang::CodeGen::CodeGenFunction::StartFunction(class clang::GlobalDecl, class clang::QualType, class llvm::Function *, class clang::CodeGen::CGFunctionInfo const &, class clang::CodeGen::FunctionArgList const &, class clang::SourceLocation, class clang::SourceLocation) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenFunction.cpp:1048:0
#13 0x00007ff79dcb735e clang::CodeGen::CodeGenFunction::GenerateCode(class clang::GlobalDecl, class llvm::Function *, class clang::CodeGen::CGFunctionInfo const &) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenFunction.cpp:1559:0
#14 0x00007ff79db6ecf3 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(class clang::GlobalDecl, class llvm::GlobalValue *) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenModule.cpp:6210:0
#15 0x00007ff79db6ea9d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(class clang::GlobalDecl, class llvm::GlobalValue *) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenModule.cpp:4314:0
#16 0x00007ff79db6dec6 clang::CodeGen::CodeGenModule::EmitGlobal(class clang::GlobalDecl) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenModule.cpp:4029:0
#17 0x00007ff79db71ded clang::CodeGen::CodeGenModule::EmitTopLevelDecl(class clang::Decl *) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenModule.cpp:7114:0
#18 0x00007ff79e551b4c `anonymous namespace'::CodeGeneratorImpl::HandleTopLevelDecl E:\dev\llvm-project\clang\lib\CodeGen\ModuleBuilder.cpp:190:0
#19 0x00007ff79e522251 clang::BackendConsumer::HandleTopLevelDecl(class clang::DeclGroupRef) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:172:0
#20 0x00007ff79f74b306 clang::ParseAST(class clang::Sema &, bool, bool) E:\dev\llvm-project\clang\lib\Parse\ParseAST.cpp:174:0
#21 0x00007ff79e827494 clang::ASTFrontendAction::ExecuteAction(void) E:\dev\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1339:0
#22 0x00007ff79e5214ca clang::CodeGenAction::ExecuteAction(void) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:1112:0
#23 0x00007ff79e8272a4 clang::FrontendAction::Execute(void) E:\dev\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1225:0
#24 0x00007ff79e809064 llvm::Error::operator bool E:\dev\llvm-project\llvm\include\llvm\Support\Error.h:242:0
#25 0x00007ff79e809064 clang::CompilerInstance::ExecuteAction(class clang::FrontendAction &) E:\dev\llvm-project\clang\lib\Frontend\CompilerInstance.cpp:1055:0
#26 0x00007ff79e91f099 clang::ExecuteCompilerInvocation(class clang::CompilerInstance *) E:\dev\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp:300:0
#27 0x00007ff79ab6d201 cc1_main(class llvm::ArrayRef<char const *>, char const *, void *) E:\dev\llvm-project\clang\tools\driver\cc1_main.cpp:298:0
#28 0x00007ff79ab6571a ExecuteCC1Tool E:\dev\llvm-project\clang\tools\driver\driver.cpp:223:0
#29 0x00007ff79e68ca71 clang::driver::CC1Command::Execute::__l7::<lambda_1>::operator() E:\dev\llvm-project\clang\lib\Driver\Job.cpp:436:0
#30 0x00007ff79e68ca71 llvm::function_ref<void __cdecl(void)>::callback_fn<`clang::driver::CC1Command::Execute'::`7'::<lambda_1> > E:\dev\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:46:0
#31 0x00007ff79d7e45ca llvm::CrashRecoveryContext::RunSafely(class llvm::function_ref<(void)>) E:\dev\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp:235:0
#32 0x00007ff79e68d315 clang::driver::CC1Command::Execute(class llvm::ArrayRef<class std::optional<class llvm::StringRef>>, class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char>> *, bool *) const E:\dev\llvm-project\clang\lib\Driver\Job.cpp:436:0
#33 0x00007ff79e5a47cb std::_Func_class<void,clang::driver::Command const &,int>::_Empty C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.43.34808\include\functional:939:0
#34 0x00007ff79e5a47cb std::function<void __cdecl(clang::driver::Command const &,int)>::operator bool C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.43.34808\include\functional:1192:0
#35 0x00007ff79e5a47cb clang::driver::Compilation::ExecuteCommand(class clang::driver::Command const &, class clang::driver::Command const *&, bool) const E:\dev\llvm-project\clang\lib\Driver\Compilation.cpp:197:0
#36 0x00007ff79e5a4a8b clang::driver::Compilation::ExecuteJobs(class clang::driver::JobList const &, class llvm::SmallVectorImpl<struct std::pair<int, class clang::driver::Command const *>> &, bool) const E:\dev\llvm-project\clang\lib\Driver\Compilation.cpp:251:0
#37 0x00007ff79e57d519 llvm::SmallVectorBase<unsigned int>::empty E:\dev\llvm-project\llvm\include\llvm\ADT\SmallVector.h:82:0
#38 0x00007ff79e57d519 clang::driver::Driver::ExecuteCompilation(class clang::driver::Compilation &, class llvm::SmallVectorImpl<struct std::pair<int, class clang::driver::Command const *>> &) E:\dev\llvm-project\clang\lib\Driver\Driver.cpp:2241:0
#39 0x00007ff79ab67921 clang_main(int, char **, struct llvm::ToolContext const &) E:\dev\llvm-project\clang\tools\driver\driver.cpp:408:0
#40 0x00007ff79ab7a044 main E:\dev\llvm-project\build\tools\clang\tools\driver\clang-driver.cpp:17:0
#41 0x00007ff7a11ac92c invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78:0
#42 0x00007ff7a11ac92c __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
#43 0x00007ffa695be8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#44 0x00007ffa6b0bc34c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x3c34c)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git (be7e4113c84317feda055cac68a855429e46e381)
Target: x86_64-pc-windows-msvc
```

Thanks,
Kacper

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Kacper Michajłow (kasper93)

<details>
Hi

I noticed that ffmpeg cannot be built with asan on Windows, turns out there is `typedef const struct ...` in one of the files, which crashes the mangler when used in function. Would be nice to not crash.

I tested with v19.1.5 and HEAD, the following report is from be7e4113c84317feda055cac68a855429e46e381.

### Command
`clang --target=x86_64-pc-windows-msvc -fsanitize=function -c r.c`

### Source code 
``` c
typedef const struct {
    int i;
} a;
void b(a c) {}
```

### Result

```
Stack dump:
0.      Program arguments: E:\\dev\\llvm-project\\build\\bin\\clang.exe --target=x86_64-pc-windows-msvc -fsanitize=function -c r.c
1.      &lt;eof&gt; parser at end of file
2.      r.c:4:6: LLVM IR generation of declaration 'b'
3.      r.c:4:6: Generating code for declaration 'b'
Exception Code: 0xC0000005
 #<!-- -->0 0x00007ff7a0407757 clang::ASTContext::getDeclaratorForUnnamedTagDecl(class clang::TagDecl const *) E:\dev\llvm-project\clang\lib\AST\ASTContext.cpp:13439:0
 #<!-- -->1 0x00007ff7a08f40ba `anonymous namespace'::MicrosoftCXXNameMangler::mangleUnqualifiedName E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:1287:0
 #<!-- -->2 0x00007ff7a08e43bf `anonymous namespace'::MicrosoftCXXNameMangler::mangleName E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:973:0
 #<!-- -->3 0x00007ff7a08f11c4 `anonymous namespace'::MicrosoftCXXNameMangler::mangleType E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:3260:0
 #<!-- -->4 0x00007ff7a08f2b61 `anonymous namespace'::MicrosoftCXXNameMangler::mangleType E:\dev\llvm-project\build\tools\clang\include\clang\AST\TypeNodes.inc:77:0
 #<!-- -->5 0x00007ff7a08e1eb8 `anonymous namespace'::MicrosoftCXXNameMangler::mangleFunctionArgumentType E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:2417:0
 #<!-- -->6 0x00007ff7a08e2f67 `anonymous namespace'::MicrosoftCXXNameMangler::mangleFunctionType E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:3050:0
 #<!-- -->7 0x00007ff7a08f2867 `anonymous namespace'::MicrosoftCXXNameMangler::mangleType E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:2605:0
 #<!-- -->8 0x00007ff7a08e1850 `anonymous namespace'::MicrosoftMangleContextImpl::mangleCanonicalTypeName E:\dev\llvm-project\clang\lib\AST\MicrosoftMangle.cpp:4150:0
 #<!-- -->9 0x00007ff79dcc2174 std::basic_string&lt;char,std::char_traits&lt;char&gt;,std::allocator&lt;char&gt; &gt;::data C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.43.34808\include\xstring:2332:0
#<!-- -->10 0x00007ff79dcc2174 llvm::StringRef::{ctor} E:\dev\llvm-project\llvm\include\llvm\ADT\StringRef.h:108:0
#<!-- -->11 0x00007ff79dcc2174 clang::CodeGen::CodeGenFunction::getUBSanFunctionTypeHash(class clang::QualType) const E:\dev\llvm-project\clang\lib\CodeGen\CodeGenFunction.cpp:623:0
#<!-- -->12 0x00007ff79dcbc22f clang::CodeGen::CodeGenFunction::StartFunction(class clang::GlobalDecl, class clang::QualType, class llvm::Function *, class clang::CodeGen::CGFunctionInfo const &amp;, class clang::CodeGen::FunctionArgList const &amp;, class clang::SourceLocation, class clang::SourceLocation) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenFunction.cpp:1048:0
#<!-- -->13 0x00007ff79dcb735e clang::CodeGen::CodeGenFunction::GenerateCode(class clang::GlobalDecl, class llvm::Function *, class clang::CodeGen::CGFunctionInfo const &amp;) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenFunction.cpp:1559:0
#<!-- -->14 0x00007ff79db6ecf3 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(class clang::GlobalDecl, class llvm::GlobalValue *) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenModule.cpp:6210:0
#<!-- -->15 0x00007ff79db6ea9d clang::CodeGen::CodeGenModule::EmitGlobalDefinition(class clang::GlobalDecl, class llvm::GlobalValue *) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenModule.cpp:4314:0
#<!-- -->16 0x00007ff79db6dec6 clang::CodeGen::CodeGenModule::EmitGlobal(class clang::GlobalDecl) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenModule.cpp:4029:0
#<!-- -->17 0x00007ff79db71ded clang::CodeGen::CodeGenModule::EmitTopLevelDecl(class clang::Decl *) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenModule.cpp:7114:0
#<!-- -->18 0x00007ff79e551b4c `anonymous namespace'::CodeGeneratorImpl::HandleTopLevelDecl E:\dev\llvm-project\clang\lib\CodeGen\ModuleBuilder.cpp:190:0
#<!-- -->19 0x00007ff79e522251 clang::BackendConsumer::HandleTopLevelDecl(class clang::DeclGroupRef) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:172:0
#<!-- -->20 0x00007ff79f74b306 clang::ParseAST(class clang::Sema &amp;, bool, bool) E:\dev\llvm-project\clang\lib\Parse\ParseAST.cpp:174:0
#<!-- -->21 0x00007ff79e827494 clang::ASTFrontendAction::ExecuteAction(void) E:\dev\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1339:0
#<!-- -->22 0x00007ff79e5214ca clang::CodeGenAction::ExecuteAction(void) E:\dev\llvm-project\clang\lib\CodeGen\CodeGenAction.cpp:1112:0
#<!-- -->23 0x00007ff79e8272a4 clang::FrontendAction::Execute(void) E:\dev\llvm-project\clang\lib\Frontend\FrontendAction.cpp:1225:0
#<!-- -->24 0x00007ff79e809064 llvm::Error::operator bool E:\dev\llvm-project\llvm\include\llvm\Support\Error.h:242:0
#<!-- -->25 0x00007ff79e809064 clang::CompilerInstance::ExecuteAction(class clang::FrontendAction &amp;) E:\dev\llvm-project\clang\lib\Frontend\CompilerInstance.cpp:1055:0
#<!-- -->26 0x00007ff79e91f099 clang::ExecuteCompilerInvocation(class clang::CompilerInstance *) E:\dev\llvm-project\clang\lib\FrontendTool\ExecuteCompilerInvocation.cpp:300:0
#<!-- -->27 0x00007ff79ab6d201 cc1_main(class llvm::ArrayRef&lt;char const *&gt;, char const *, void *) E:\dev\llvm-project\clang\tools\driver\cc1_main.cpp:298:0
#<!-- -->28 0x00007ff79ab6571a ExecuteCC1Tool E:\dev\llvm-project\clang\tools\driver\driver.cpp:223:0
#<!-- -->29 0x00007ff79e68ca71 clang::driver::CC1Command::Execute::__l7::&lt;lambda_1&gt;::operator() E:\dev\llvm-project\clang\lib\Driver\Job.cpp:436:0
#<!-- -->30 0x00007ff79e68ca71 llvm::function_ref&lt;void __cdecl(void)&gt;::callback_fn&lt;`clang::driver::CC1Command::Execute'::`7'::&lt;lambda_1&gt; &gt; E:\dev\llvm-project\llvm\include\llvm\ADT\STLFunctionalExtras.h:46:0
#<!-- -->31 0x00007ff79d7e45ca llvm::CrashRecoveryContext::RunSafely(class llvm::function_ref&lt;(void)&gt;) E:\dev\llvm-project\llvm\lib\Support\CrashRecoveryContext.cpp:235:0
#<!-- -->32 0x00007ff79e68d315 clang::driver::CC1Command::Execute(class llvm::ArrayRef&lt;class std::optional&lt;class llvm::StringRef&gt;&gt;, class std::basic_string&lt;char, struct std::char_traits&lt;char&gt;, class std::allocator&lt;char&gt;&gt; *, bool *) const E:\dev\llvm-project\clang\lib\Driver\Job.cpp:436:0
#<!-- -->33 0x00007ff79e5a47cb std::_Func_class&lt;void,clang::driver::Command const &amp;,int&gt;::_Empty C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.43.34808\include\functional:939:0
#<!-- -->34 0x00007ff79e5a47cb std::function&lt;void __cdecl(clang::driver::Command const &amp;,int)&gt;::operator bool C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.43.34808\include\functional:1192:0
#<!-- -->35 0x00007ff79e5a47cb clang::driver::Compilation::ExecuteCommand(class clang::driver::Command const &amp;, class clang::driver::Command const *&amp;, bool) const E:\dev\llvm-project\clang\lib\Driver\Compilation.cpp:197:0
#<!-- -->36 0x00007ff79e5a4a8b clang::driver::Compilation::ExecuteJobs(class clang::driver::JobList const &amp;, class llvm::SmallVectorImpl&lt;struct std::pair&lt;int, class clang::driver::Command const *&gt;&gt; &amp;, bool) const E:\dev\llvm-project\clang\lib\Driver\Compilation.cpp:251:0
#<!-- -->37 0x00007ff79e57d519 llvm::SmallVectorBase&lt;unsigned int&gt;::empty E:\dev\llvm-project\llvm\include\llvm\ADT\SmallVector.h:82:0
#<!-- -->38 0x00007ff79e57d519 clang::driver::Driver::ExecuteCompilation(class clang::driver::Compilation &amp;, class llvm::SmallVectorImpl&lt;struct std::pair&lt;int, class clang::driver::Command const *&gt;&gt; &amp;) E:\dev\llvm-project\clang\lib\Driver\Driver.cpp:2241:0
#<!-- -->39 0x00007ff79ab67921 clang_main(int, char **, struct llvm::ToolContext const &amp;) E:\dev\llvm-project\clang\tools\driver\driver.cpp:408:0
#<!-- -->40 0x00007ff79ab7a044 main E:\dev\llvm-project\build\tools\clang\tools\driver\clang-driver.cpp:17:0
#<!-- -->41 0x00007ff7a11ac92c invoke_main D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:78:0
#<!-- -->42 0x00007ff7a11ac92c __scrt_common_main_seh D:\a\_work\1\s\src\vctools\crt\vcstartup\src\startup\exe_common.inl:288:0
#<!-- -->43 0x00007ffa695be8d7 (C:\WINDOWS\System32\KERNEL32.DLL+0x2e8d7)
#<!-- -->44 0x00007ffa6b0bc34c (C:\WINDOWS\SYSTEM32\ntdll.dll+0x3c34c)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 21.0.0git (be7e4113c84317feda055cac68a855429e46e381)
Target: x86_64-pc-windows-msvc
```

Thanks,
Kacper
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 20 or `main` branch?

---

### Comment 3 - kasper93

> Could you please try 20 or `main` branch?

> I tested with v19.1.5 and HEAD, the following report is from https://github.com/llvm/llvm-project/commit/be7e4113c84317feda055cac68a855429e46e381.
> clang version 21.0.0git (be7e4113c84317feda055cac68a855429e46e381)

As you can imagine it also happens with 20.1.7.

EDIT: It's easily reproducible on https://godbolt.org/z/azcTz8Ecd too

---

### Comment 4 - shafik

Looks like this goes back to clang-17: https://godbolt.org/z/zsrnME9bq

---

### Comment 5 - kasper93

> Looks like this goes back to clang-17: https://godbolt.org/z/zsrnME9bq

For some definition of an regression, because before 17 there was no support for `-fsanitize=function`, so it just didn't trigger this mangler code. 

---

### Comment 6 - shafik

> > Looks like this goes back to clang-17: https://godbolt.org/z/zsrnME9bq
> 
> For some definition of an regression, because before 17 there was no support for `-fsanitize=function`, so it just didn't trigger this mangler code.

I missed the diagnostic there.

---

