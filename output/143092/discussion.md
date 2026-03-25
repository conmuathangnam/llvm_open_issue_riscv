# Clang-21 Crash: non-canonical or dependent type in IR-generation UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/CodeGenFunction.cpp:265!

**Author:** hyf751175784
**URL:** https://github.com/llvm/llvm-project/issues/143092
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-invalid
**Closed Date:** 2025-06-06T11:04:41Z

## Body

### Description
I encountered a crash in Clang-21 when compiling the following code with compilation options:
```cpp
template<typename T> void foo(T) {}

template void foo(auto);

void bar()
{
  foo(0);
}


```
Options:
```sh
-std=c++20 -Winit-self -Wswitch-enum
```

### Clang Version

```sh
$ clang -v
Ubuntu clang version 21.0.0 (++20250501082025+9b1051281e43-1~exp1~20250501082214.882)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@m64
Selected multilib: .;@m64
```

### Command
The compilation command used was:
```sh
clang bug.cpp -std=c++20 -Winit-self -Wswitch-enum  -c -o bug.o
```

### Error Message
```sh
non-canonical or dependent type in IR-generation
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/CodeGenFunction.cpp:265!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -Winit-self -Wswitch-enum <source>
1.	<source>:3:24: current parser token ';'
2.	<source>:1:27: instantiating function definition 'foo<type-parameter-0-0>'
3.	<source>:1:27: LLVM IR generation of declaration 'foo'
4.	<source>:1:27: Generating code for declaration 'foo'
 #0 0x0000000003fdf498 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdf498)
 #1 0x0000000003fdd124 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdd124)
 #2 0x0000000003f21e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c78b2a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007c78b2a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007c78b2a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007c78b2a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003f2d5ca (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f2d5ca)
 #8 0x000000000452ee72 clang::CodeGen::CodeGenFunction::getEvaluationKind(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x452ee72)
 #9 0x000000000481ffcf clang::CodeGen::isAggregateTypeForABI(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481ffcf)
#10 0x00000000048066c4 (anonymous namespace)::X86_64ABIInfo::getIndirectResult(clang::QualType, unsigned int) const X86.cpp:0:0
#11 0x000000000480ecd8 (anonymous namespace)::X86_64ABIInfo::classifyArgumentType(clang::QualType, unsigned int, unsigned int&, unsigned int&, bool, bool) const X86.cpp:0:0
#12 0x000000000480f744 (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&) const X86.cpp:0:0
#13 0x0000000004890e11 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, clang::CodeGen::FnInfoOpts, llvm::ArrayRef<clang::CanQual<clang::Type>>, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4890e11)
#14 0x00000000048932ce arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&, bool, llvm::SmallVectorImpl<clang::CanQual<clang::Type>>&, clang::CanQual<clang::FunctionProtoType>) CGCall.cpp:0:0
#15 0x0000000004896e9e clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4896e9e)
#16 0x00000000045adb45 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45adb45)
#17 0x00000000045a8c84 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a8c84)
#18 0x00000000045a9333 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a9333)
#19 0x00000000045b3e53 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#20 0x0000000004998b76 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#21 0x0000000004988654 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4988654)
#22 0x00000000072f201a clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f201a)
#23 0x0000000007101451 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7101451)
#24 0x00000000066c32bd clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c32bd)
#25 0x00000000066d2fa9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d2fa9)
#26 0x00000000067b4058 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67b4058)
#27 0x00000000067b4ba6 clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67b4ba6)
#28 0x00000000067c2018 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67c2018)
#29 0x00000000066dc2e0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dc2e0)
#30 0x00000000066991dc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66991dc)
#31 0x000000000669a0cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669a0cd)
#32 0x000000000668c49a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668c49a)
#33 0x0000000004994d08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4994d08)
#34 0x0000000004c8ede5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8ede5)
#35 0x0000000004c0c0ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0c0ee)
#36 0x0000000004d81149 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d81149)
#37 0x0000000000dbc0cf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbc0cf)
#38 0x0000000000db2cda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#39 0x00000000049fbd59 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x0000000003f22324 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f22324)
#41 0x00000000049fc36f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x00000000049bdebd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bdebd)
#43 0x00000000049bef4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bef4e)
#44 0x00000000049c7045 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c7045)
#45 0x0000000000db85df clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb85df)
#46 0x0000000000c3ba44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc3ba44)
#47 0x00007c78b2a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x00007c78b2a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000db2785 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2785)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

The issue can also be reproduced on Compiler Explorer:https://godbolt.org/z/jxfGe8e5r
Please let me know if you need any more details.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Huang Yifan (hyf751175784)

<details>
### Description
I encountered a crash in Clang-21 when compiling the following code with compilation options:
```cpp
template&lt;typename T&gt; void foo(T) {}

template void foo(auto);

void bar()
{
  foo(0);
}


```
Options:
```sh
-std=c++20 -Winit-self -Wswitch-enum
```

### Clang Version

```sh
$ clang -v
Ubuntu clang version 21.0.0 (++20250501082025+9b1051281e43-1~exp1~20250501082214.882)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

### Command
The compilation command used was:
```sh
clang bug.cpp -std=c++20 -Winit-self -Wswitch-enum  -c -o bug.o
```

### Error Message
```sh
non-canonical or dependent type in IR-generation
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/CodeGenFunction.cpp:265!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -Winit-self -Wswitch-enum &lt;source&gt;
1.	&lt;source&gt;:3:24: current parser token ';'
2.	&lt;source&gt;:1:27: instantiating function definition 'foo&lt;type-parameter-0-0&gt;'
3.	&lt;source&gt;:1:27: LLVM IR generation of declaration 'foo'
4.	&lt;source&gt;:1:27: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003fdf498 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdf498)
 #<!-- -->1 0x0000000003fdd124 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdd124)
 #<!-- -->2 0x0000000003f21e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c78b2a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007c78b2a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007c78b2a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007c78b2a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003f2d5ca (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f2d5ca)
 #<!-- -->8 0x000000000452ee72 clang::CodeGen::CodeGenFunction::getEvaluationKind(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x452ee72)
 #<!-- -->9 0x000000000481ffcf clang::CodeGen::isAggregateTypeForABI(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481ffcf)
#<!-- -->10 0x00000000048066c4 (anonymous namespace)::X86_64ABIInfo::getIndirectResult(clang::QualType, unsigned int) const X86.cpp:0:0
#<!-- -->11 0x000000000480ecd8 (anonymous namespace)::X86_64ABIInfo::classifyArgumentType(clang::QualType, unsigned int, unsigned int&amp;, unsigned int&amp;, bool, bool) const X86.cpp:0:0
#<!-- -->12 0x000000000480f744 (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&amp;) const X86.cpp:0:0
#<!-- -->13 0x0000000004890e11 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4890e11)
#<!-- -->14 0x00000000048932ce arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt;&gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->15 0x0000000004896e9e clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4896e9e)
#<!-- -->16 0x00000000045adb45 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45adb45)
#<!-- -->17 0x00000000045a8c84 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a8c84)
#<!-- -->18 0x00000000045a9333 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a9333)
#<!-- -->19 0x00000000045b3e53 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->20 0x0000000004998b76 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->21 0x0000000004988654 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4988654)
#<!-- -->22 0x00000000072f201a clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f201a)
#<!-- -->23 0x0000000007101451 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7101451)
#<!-- -->24 0x00000000066c32bd clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c32bd)
#<!-- -->25 0x00000000066d2fa9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d2fa9)
#<!-- -->26 0x00000000067b4058 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67b4058)
#<!-- -->27 0x00000000067b4ba6 clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67b4ba6)
#<!-- -->28 0x00000000067c2018 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67c2018)
#<!-- -->29 0x00000000066dc2e0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dc2e0)
#<!-- -->30 0x00000000066991dc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66991dc)
#<!-- -->31 0x000000000669a0cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669a0cd)
#<!-- -->32 0x000000000668c49a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668c49a)
#<!-- -->33 0x0000000004994d08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4994d08)
#<!-- -->34 0x0000000004c8ede5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8ede5)
#<!-- -->35 0x0000000004c0c0ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0c0ee)
#<!-- -->36 0x0000000004d81149 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d81149)
#<!-- -->37 0x0000000000dbc0cf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbc0cf)
#<!-- -->38 0x0000000000db2cda ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x00000000049fbd59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x0000000003f22324 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f22324)
#<!-- -->41 0x00000000049fc36f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x00000000049bdebd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bdebd)
#<!-- -->43 0x00000000049bef4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bef4e)
#<!-- -->44 0x00000000049c7045 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c7045)
#<!-- -->45 0x0000000000db85df clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb85df)
#<!-- -->46 0x0000000000c3ba44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc3ba44)
#<!-- -->47 0x00007c78b2a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007c78b2a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000db2785 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2785)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

The issue can also be reproduced on Compiler Explorer:https://godbolt.org/z/jxfGe8e5r
Please let me know if you need any more details.
</details>


---

### Comment 2 - zwuis

It looks like a duplicate of: #121009.

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Huang Yifan (hyf751175784)

<details>
### Description
I encountered a crash in Clang-21 when compiling the following code with compilation options:
```cpp
template&lt;typename T&gt; void foo(T) {}

template void foo(auto);

void bar()
{
  foo(0);
}


```
Options:
```sh
-std=c++20 -Winit-self -Wswitch-enum
```

### Clang Version

```sh
$ clang -v
Ubuntu clang version 21.0.0 (++20250501082025+9b1051281e43-1~exp1~20250501082214.882)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/13
Candidate multilib: .;@<!-- -->m64
Selected multilib: .;@<!-- -->m64
```

### Command
The compilation command used was:
```sh
clang bug.cpp -std=c++20 -Winit-self -Wswitch-enum  -c -o bug.o
```

### Error Message
```sh
non-canonical or dependent type in IR-generation
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/CodeGen/CodeGenFunction.cpp:265!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -Winit-self -Wswitch-enum &lt;source&gt;
1.	&lt;source&gt;:3:24: current parser token ';'
2.	&lt;source&gt;:1:27: instantiating function definition 'foo&lt;type-parameter-0-0&gt;'
3.	&lt;source&gt;:1:27: LLVM IR generation of declaration 'foo'
4.	&lt;source&gt;:1:27: Generating code for declaration 'foo'
 #<!-- -->0 0x0000000003fdf498 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdf498)
 #<!-- -->1 0x0000000003fdd124 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fdd124)
 #<!-- -->2 0x0000000003f21e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c78b2a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007c78b2a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007c78b2a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007c78b2a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003f2d5ca (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f2d5ca)
 #<!-- -->8 0x000000000452ee72 clang::CodeGen::CodeGenFunction::getEvaluationKind(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x452ee72)
 #<!-- -->9 0x000000000481ffcf clang::CodeGen::isAggregateTypeForABI(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x481ffcf)
#<!-- -->10 0x00000000048066c4 (anonymous namespace)::X86_64ABIInfo::getIndirectResult(clang::QualType, unsigned int) const X86.cpp:0:0
#<!-- -->11 0x000000000480ecd8 (anonymous namespace)::X86_64ABIInfo::classifyArgumentType(clang::QualType, unsigned int, unsigned int&amp;, unsigned int&amp;, bool, bool) const X86.cpp:0:0
#<!-- -->12 0x000000000480f744 (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&amp;) const X86.cpp:0:0
#<!-- -->13 0x0000000004890e11 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4890e11)
#<!-- -->14 0x00000000048932ce arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt;&gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->15 0x0000000004896e9e clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4896e9e)
#<!-- -->16 0x00000000045adb45 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45adb45)
#<!-- -->17 0x00000000045a8c84 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a8c84)
#<!-- -->18 0x00000000045a9333 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45a9333)
#<!-- -->19 0x00000000045b3e53 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->20 0x0000000004998b76 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->21 0x0000000004988654 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4988654)
#<!-- -->22 0x00000000072f201a clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f201a)
#<!-- -->23 0x0000000007101451 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7101451)
#<!-- -->24 0x00000000066c32bd clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c32bd)
#<!-- -->25 0x00000000066d2fa9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66d2fa9)
#<!-- -->26 0x00000000067b4058 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67b4058)
#<!-- -->27 0x00000000067b4ba6 clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67b4ba6)
#<!-- -->28 0x00000000067c2018 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67c2018)
#<!-- -->29 0x00000000066dc2e0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dc2e0)
#<!-- -->30 0x00000000066991dc clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66991dc)
#<!-- -->31 0x000000000669a0cd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x669a0cd)
#<!-- -->32 0x000000000668c49a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668c49a)
#<!-- -->33 0x0000000004994d08 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4994d08)
#<!-- -->34 0x0000000004c8ede5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8ede5)
#<!-- -->35 0x0000000004c0c0ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0c0ee)
#<!-- -->36 0x0000000004d81149 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d81149)
#<!-- -->37 0x0000000000dbc0cf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbc0cf)
#<!-- -->38 0x0000000000db2cda ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->39 0x00000000049fbd59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->40 0x0000000003f22324 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f22324)
#<!-- -->41 0x00000000049fc36f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->42 0x00000000049bdebd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bdebd)
#<!-- -->43 0x00000000049bef4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bef4e)
#<!-- -->44 0x00000000049c7045 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c7045)
#<!-- -->45 0x0000000000db85df clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb85df)
#<!-- -->46 0x0000000000c3ba44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc3ba44)
#<!-- -->47 0x00007c78b2a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->48 0x00007c78b2a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->49 0x0000000000db2785 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb2785)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

The issue can also be reproduced on Compiler Explorer:https://godbolt.org/z/jxfGe8e5r
Please let me know if you need any more details.
</details>


---

