# ICE on invalid explicit instantiation as abbreviated function template: non-canonical or dependent type in IR-generation

**Author:** hubert-reinterpretcast
**URL:** https://github.com/llvm/llvm-project/issues/121009

## Body

The following source attempts to explicitly instantiate something that is not completely-specialized.
Clang ICEs.

With assertions enabled, we see "non-canonical or dependent type in IR-generation".

Online compiler link: https://godbolt.org/z/WvKobK4P7

### SOURCE (`<stdin>`)
```cpp
template <typename T>
void f(auto) {}

template void f<int>(auto);
```

### COMPILER INVOCATION
```
clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
```

### ACTUAL COMPILER OUTPUT (without assertions enabled)
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/wandbox/clang-head/bin/clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
1.      <stdin>:4:27: current parser token ';'
2.      <stdin>:2:6: instantiating function definition 'f<int, type-parameter-0-0>'
3.      <stdin>:2:6: LLVM IR generation of declaration 'f'
4.      <stdin>:2:6: Generating code for declaration 'f'
 #0 0x000056a045979a98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/wandbox/clang-head/bin/clang+++0x301da98)
 #1 0x000056a04597788e llvm::sys::RunSignalHandlers() (/opt/wandbox/clang-head/bin/clang+++0x301b88e)
 #2 0x000056a0458f75f9 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d1275445320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #4 0x000056a045c33a68 clang::CodeGen::CodeGenFunction::EmitReturnBlock() (/opt/wandbox/clang-head/bin/clang+++0x32d7a68)
 #5 0x000056a045d32f3c clang::CodeGen::isAggregateTypeForABI(clang::QualType) (/opt/wandbox/clang-head/bin/clang+++0x33d6f3c)
 #6 0x000056a045d27c9e (anonymous namespace)::X86_64ABIInfo::getIndirectResult(clang::QualType, unsigned int) const X86.cpp:0:0
 #7 0x000056a045d2533d (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&) const X86.cpp:0:0
 #8 0x000056a045e7aab1 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual<clang::Type>, clang::CodeGen::FnInfoOpts, llvm::ArrayRef<clang::CanQual<clang::Type>>, clang::FunctionType::ExtInfo, llvm::ArrayRef<clang::FunctionType::ExtParameterInfo>, clang::CodeGen::RequiredArgs) (/opt/wandbox/clang-head/bin/clang+++0x351eab1)
 #9 0x000056a045e7ae8e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&, bool, llvm::SmallVectorImpl<clang::CanQual<clang::Type>>&, clang::CanQual<clang::FunctionProtoType>) CGCall.cpp:0:0
#10 0x000056a045e7c3aa clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::FunctionDecl const*) (/opt/wandbox/clang-head/bin/clang+++0x35203aa)
#11 0x000056a045c5d188 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x3301188)
#12 0x000056a045c565fa clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x32fa5fa)
#13 0x000056a045c5a48b clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/wandbox/clang-head/bin/clang+++0x32fe48b)
#14 0x000056a045c55600 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/opt/wandbox/clang-head/bin/clang+++0x32f9600)
#15 0x000056a04601a5fc (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#16 0x000056a046013b7d clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/wandbox/clang-head/bin/clang+++0x36b7b7d)
#17 0x000056a047df028f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x549428f)
#18 0x000056a047ca8ee0 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&) (/opt/wandbox/clang-head/bin/clang+++0x534cee0)
#19 0x000056a04767dc34 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d21c34)
#20 0x000056a04767bfb1 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d1ffb1)
#21 0x000056a04770a4ee clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dae4ee)
#22 0x000056a0477092d3 clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dad2d3)
#23 0x000056a0477091d6 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/wandbox/clang-head/bin/clang+++0x4dad1d6)
#24 0x000056a04767ab95 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/wandbox/clang-head/bin/clang+++0x4d1eb95)
#25 0x000056a04765c84e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/wandbox/clang-head/bin/clang+++0x4d0084e)
#26 0x000056a04765b876 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/wandbox/clang-head/bin/clang+++0x4cff876)
#27 0x000056a0476580de clang::ParseAST(clang::Sema&, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x4cfc0de)
#28 0x000056a046287e66 clang::FrontendAction::Execute() (/opt/wandbox/clang-head/bin/clang+++0x392be66)
#29 0x000056a046201234 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/wandbox/clang-head/bin/clang+++0x38a5234)
#30 0x000056a04634ea6e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/wandbox/clang-head/bin/clang+++0x39f2a6e)
#31 0x000056a0438ddca0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/wandbox/clang-head/bin/clang+++0xf81ca0)
#32 0x000056a0438dac61 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x000056a04605a649 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) Job.cpp:0:0
#34 0x000056a0458f73dc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/wandbox/clang-head/bin/clang+++0x2f9b3dc)
#35 0x000056a04605a086 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/opt/wandbox/clang-head/bin/clang+++0x36fe086)
#36 0x000056a04601eb07 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2b07)
#37 0x000056a04601ed6e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2d6e)
#38 0x000056a04603a6e0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/wandbox/clang-head/bin/clang+++0x36de6e0)
#39 0x000056a0438da40f clang_main(int, char**, llvm::ToolContext const&) (/opt/wandbox/clang-head/bin/clang+++0xf7e40f)
#40 0x000056a0438e7f37 main (/opt/wandbox/clang-head/bin/clang+++0xf8bf37)
#41 0x00007d127542a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#42 0x00007d127542a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#43 0x000056a0438d8a05 _start (/opt/wandbox/clang-head/bin/clang+++0xf7ca05)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git b84218526d364cb9fda23bfca164d25a7b03a641)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
clang++: note: diagnostic msg: Error generating preprocessed source(s) - ignoring input from stdin.
clang++: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

### EXPECTED COMPILER OUTPUT
(non-ICE error)


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-20

Author: Hubert Tong (hubert-reinterpretcast)

<details>
The following source attempts to explicitly instantiate something that is not completely-specialized.
Clang ICEs.

With assertions enabled, we see "non-canonical or dependent type in IR-generation".

Online compiler link: https://godbolt.org/z/WvKobK4P7

### SOURCE (`&lt;stdin&gt;`)
```cpp
template &lt;typename T&gt;
void f(auto) {}

template void f&lt;int&gt;(auto);
```

### COMPILER INVOCATION
```
clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
```

### ACTUAL COMPILER OUTPUT (without assertions enabled)
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/wandbox/clang-head/bin/clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
1.      &lt;stdin&gt;:4:27: current parser token ';'
2.      &lt;stdin&gt;:2:6: instantiating function definition 'f&lt;int, type-parameter-0-0&gt;'
3.      &lt;stdin&gt;:2:6: LLVM IR generation of declaration 'f'
4.      &lt;stdin&gt;:2:6: Generating code for declaration 'f'
 #<!-- -->0 0x000056a045979a98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/wandbox/clang-head/bin/clang+++0x301da98)
 #<!-- -->1 0x000056a04597788e llvm::sys::RunSignalHandlers() (/opt/wandbox/clang-head/bin/clang+++0x301b88e)
 #<!-- -->2 0x000056a0458f75f9 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d1275445320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #<!-- -->4 0x000056a045c33a68 clang::CodeGen::CodeGenFunction::EmitReturnBlock() (/opt/wandbox/clang-head/bin/clang+++0x32d7a68)
 #<!-- -->5 0x000056a045d32f3c clang::CodeGen::isAggregateTypeForABI(clang::QualType) (/opt/wandbox/clang-head/bin/clang+++0x33d6f3c)
 #<!-- -->6 0x000056a045d27c9e (anonymous namespace)::X86_64ABIInfo::getIndirectResult(clang::QualType, unsigned int) const X86.cpp:0:0
 #<!-- -->7 0x000056a045d2533d (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&amp;) const X86.cpp:0:0
 #<!-- -->8 0x000056a045e7aab1 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/wandbox/clang-head/bin/clang+++0x351eab1)
 #<!-- -->9 0x000056a045e7ae8e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt;&gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->10 0x000056a045e7c3aa clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::FunctionDecl const*) (/opt/wandbox/clang-head/bin/clang+++0x35203aa)
#<!-- -->11 0x000056a045c5d188 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x3301188)
#<!-- -->12 0x000056a045c565fa clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x32fa5fa)
#<!-- -->13 0x000056a045c5a48b clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/wandbox/clang-head/bin/clang+++0x32fe48b)
#<!-- -->14 0x000056a045c55600 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/opt/wandbox/clang-head/bin/clang+++0x32f9600)
#<!-- -->15 0x000056a04601a5fc (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->16 0x000056a046013b7d clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/wandbox/clang-head/bin/clang+++0x36b7b7d)
#<!-- -->17 0x000056a047df028f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x549428f)
#<!-- -->18 0x000056a047ca8ee0 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&amp;) (/opt/wandbox/clang-head/bin/clang+++0x534cee0)
#<!-- -->19 0x000056a04767dc34 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d21c34)
#<!-- -->20 0x000056a04767bfb1 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d1ffb1)
#<!-- -->21 0x000056a04770a4ee clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dae4ee)
#<!-- -->22 0x000056a0477092d3 clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dad2d3)
#<!-- -->23 0x000056a0477091d6 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/wandbox/clang-head/bin/clang+++0x4dad1d6)
#<!-- -->24 0x000056a04767ab95 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/wandbox/clang-head/bin/clang+++0x4d1eb95)
#<!-- -->25 0x000056a04765c84e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/wandbox/clang-head/bin/clang+++0x4d0084e)
#<!-- -->26 0x000056a04765b876 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/wandbox/clang-head/bin/clang+++0x4cff876)
#<!-- -->27 0x000056a0476580de clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x4cfc0de)
#<!-- -->28 0x000056a046287e66 clang::FrontendAction::Execute() (/opt/wandbox/clang-head/bin/clang+++0x392be66)
#<!-- -->29 0x000056a046201234 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/wandbox/clang-head/bin/clang+++0x38a5234)
#<!-- -->30 0x000056a04634ea6e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/wandbox/clang-head/bin/clang+++0x39f2a6e)
#<!-- -->31 0x000056a0438ddca0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/wandbox/clang-head/bin/clang+++0xf81ca0)
#<!-- -->32 0x000056a0438dac61 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x000056a04605a649 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->34 0x000056a0458f73dc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/wandbox/clang-head/bin/clang+++0x2f9b3dc)
#<!-- -->35 0x000056a04605a086 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/opt/wandbox/clang-head/bin/clang+++0x36fe086)
#<!-- -->36 0x000056a04601eb07 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2b07)
#<!-- -->37 0x000056a04601ed6e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2d6e)
#<!-- -->38 0x000056a04603a6e0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/wandbox/clang-head/bin/clang+++0x36de6e0)
#<!-- -->39 0x000056a0438da40f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/wandbox/clang-head/bin/clang+++0xf7e40f)
#<!-- -->40 0x000056a0438e7f37 main (/opt/wandbox/clang-head/bin/clang+++0xf8bf37)
#<!-- -->41 0x00007d127542a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->42 0x00007d127542a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->43 0x000056a0438d8a05 _start (/opt/wandbox/clang-head/bin/clang+++0xf7ca05)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git b84218526d364cb9fda23bfca164d25a7b03a641)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
clang++: note: diagnostic msg: Error generating preprocessed source(s) - ignoring input from stdin.
clang++: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

### EXPECTED COMPILER OUTPUT
(non-ICE error)

</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hubert Tong (hubert-reinterpretcast)

<details>
The following source attempts to explicitly instantiate something that is not completely-specialized.
Clang ICEs.

With assertions enabled, we see "non-canonical or dependent type in IR-generation".

Online compiler link: https://godbolt.org/z/WvKobK4P7

### SOURCE (`&lt;stdin&gt;`)
```cpp
template &lt;typename T&gt;
void f(auto) {}

template void f&lt;int&gt;(auto);
```

### COMPILER INVOCATION
```
clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
```

### ACTUAL COMPILER OUTPUT (without assertions enabled)
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/wandbox/clang-head/bin/clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
1.      &lt;stdin&gt;:4:27: current parser token ';'
2.      &lt;stdin&gt;:2:6: instantiating function definition 'f&lt;int, type-parameter-0-0&gt;'
3.      &lt;stdin&gt;:2:6: LLVM IR generation of declaration 'f'
4.      &lt;stdin&gt;:2:6: Generating code for declaration 'f'
 #<!-- -->0 0x000056a045979a98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/wandbox/clang-head/bin/clang+++0x301da98)
 #<!-- -->1 0x000056a04597788e llvm::sys::RunSignalHandlers() (/opt/wandbox/clang-head/bin/clang+++0x301b88e)
 #<!-- -->2 0x000056a0458f75f9 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d1275445320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #<!-- -->4 0x000056a045c33a68 clang::CodeGen::CodeGenFunction::EmitReturnBlock() (/opt/wandbox/clang-head/bin/clang+++0x32d7a68)
 #<!-- -->5 0x000056a045d32f3c clang::CodeGen::isAggregateTypeForABI(clang::QualType) (/opt/wandbox/clang-head/bin/clang+++0x33d6f3c)
 #<!-- -->6 0x000056a045d27c9e (anonymous namespace)::X86_64ABIInfo::getIndirectResult(clang::QualType, unsigned int) const X86.cpp:0:0
 #<!-- -->7 0x000056a045d2533d (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&amp;) const X86.cpp:0:0
 #<!-- -->8 0x000056a045e7aab1 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/wandbox/clang-head/bin/clang+++0x351eab1)
 #<!-- -->9 0x000056a045e7ae8e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt;&gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->10 0x000056a045e7c3aa clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::FunctionDecl const*) (/opt/wandbox/clang-head/bin/clang+++0x35203aa)
#<!-- -->11 0x000056a045c5d188 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x3301188)
#<!-- -->12 0x000056a045c565fa clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x32fa5fa)
#<!-- -->13 0x000056a045c5a48b clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/wandbox/clang-head/bin/clang+++0x32fe48b)
#<!-- -->14 0x000056a045c55600 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/opt/wandbox/clang-head/bin/clang+++0x32f9600)
#<!-- -->15 0x000056a04601a5fc (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->16 0x000056a046013b7d clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/wandbox/clang-head/bin/clang+++0x36b7b7d)
#<!-- -->17 0x000056a047df028f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x549428f)
#<!-- -->18 0x000056a047ca8ee0 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&amp;) (/opt/wandbox/clang-head/bin/clang+++0x534cee0)
#<!-- -->19 0x000056a04767dc34 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d21c34)
#<!-- -->20 0x000056a04767bfb1 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d1ffb1)
#<!-- -->21 0x000056a04770a4ee clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dae4ee)
#<!-- -->22 0x000056a0477092d3 clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dad2d3)
#<!-- -->23 0x000056a0477091d6 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/wandbox/clang-head/bin/clang+++0x4dad1d6)
#<!-- -->24 0x000056a04767ab95 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/wandbox/clang-head/bin/clang+++0x4d1eb95)
#<!-- -->25 0x000056a04765c84e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/wandbox/clang-head/bin/clang+++0x4d0084e)
#<!-- -->26 0x000056a04765b876 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/wandbox/clang-head/bin/clang+++0x4cff876)
#<!-- -->27 0x000056a0476580de clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x4cfc0de)
#<!-- -->28 0x000056a046287e66 clang::FrontendAction::Execute() (/opt/wandbox/clang-head/bin/clang+++0x392be66)
#<!-- -->29 0x000056a046201234 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/wandbox/clang-head/bin/clang+++0x38a5234)
#<!-- -->30 0x000056a04634ea6e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/wandbox/clang-head/bin/clang+++0x39f2a6e)
#<!-- -->31 0x000056a0438ddca0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/wandbox/clang-head/bin/clang+++0xf81ca0)
#<!-- -->32 0x000056a0438dac61 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x000056a04605a649 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->34 0x000056a0458f73dc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/wandbox/clang-head/bin/clang+++0x2f9b3dc)
#<!-- -->35 0x000056a04605a086 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/opt/wandbox/clang-head/bin/clang+++0x36fe086)
#<!-- -->36 0x000056a04601eb07 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2b07)
#<!-- -->37 0x000056a04601ed6e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2d6e)
#<!-- -->38 0x000056a04603a6e0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/wandbox/clang-head/bin/clang+++0x36de6e0)
#<!-- -->39 0x000056a0438da40f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/wandbox/clang-head/bin/clang+++0xf7e40f)
#<!-- -->40 0x000056a0438e7f37 main (/opt/wandbox/clang-head/bin/clang+++0xf8bf37)
#<!-- -->41 0x00007d127542a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->42 0x00007d127542a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->43 0x000056a0438d8a05 _start (/opt/wandbox/clang-head/bin/clang+++0xf7ca05)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git b84218526d364cb9fda23bfca164d25a7b03a641)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
clang++: note: diagnostic msg: Error generating preprocessed source(s) - ignoring input from stdin.
clang++: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

### EXPECTED COMPILER OUTPUT
(non-ICE error)

</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Hubert Tong (hubert-reinterpretcast)

<details>
The following source attempts to explicitly instantiate something that is not completely-specialized.
Clang ICEs.

With assertions enabled, we see "non-canonical or dependent type in IR-generation".

Online compiler link: https://godbolt.org/z/WvKobK4P7

### SOURCE (`&lt;stdin&gt;`)
```cpp
template &lt;typename T&gt;
void f(auto) {}

template void f&lt;int&gt;(auto);
```

### COMPILER INVOCATION
```
clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
```

### ACTUAL COMPILER OUTPUT (without assertions enabled)
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/wandbox/clang-head/bin/clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
1.      &lt;stdin&gt;:4:27: current parser token ';'
2.      &lt;stdin&gt;:2:6: instantiating function definition 'f&lt;int, type-parameter-0-0&gt;'
3.      &lt;stdin&gt;:2:6: LLVM IR generation of declaration 'f'
4.      &lt;stdin&gt;:2:6: Generating code for declaration 'f'
 #<!-- -->0 0x000056a045979a98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/wandbox/clang-head/bin/clang+++0x301da98)
 #<!-- -->1 0x000056a04597788e llvm::sys::RunSignalHandlers() (/opt/wandbox/clang-head/bin/clang+++0x301b88e)
 #<!-- -->2 0x000056a0458f75f9 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d1275445320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #<!-- -->4 0x000056a045c33a68 clang::CodeGen::CodeGenFunction::EmitReturnBlock() (/opt/wandbox/clang-head/bin/clang+++0x32d7a68)
 #<!-- -->5 0x000056a045d32f3c clang::CodeGen::isAggregateTypeForABI(clang::QualType) (/opt/wandbox/clang-head/bin/clang+++0x33d6f3c)
 #<!-- -->6 0x000056a045d27c9e (anonymous namespace)::X86_64ABIInfo::getIndirectResult(clang::QualType, unsigned int) const X86.cpp:0:0
 #<!-- -->7 0x000056a045d2533d (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&amp;) const X86.cpp:0:0
 #<!-- -->8 0x000056a045e7aab1 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/wandbox/clang-head/bin/clang+++0x351eab1)
 #<!-- -->9 0x000056a045e7ae8e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt;&gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->10 0x000056a045e7c3aa clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::FunctionDecl const*) (/opt/wandbox/clang-head/bin/clang+++0x35203aa)
#<!-- -->11 0x000056a045c5d188 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x3301188)
#<!-- -->12 0x000056a045c565fa clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x32fa5fa)
#<!-- -->13 0x000056a045c5a48b clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/wandbox/clang-head/bin/clang+++0x32fe48b)
#<!-- -->14 0x000056a045c55600 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/opt/wandbox/clang-head/bin/clang+++0x32f9600)
#<!-- -->15 0x000056a04601a5fc (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->16 0x000056a046013b7d clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/wandbox/clang-head/bin/clang+++0x36b7b7d)
#<!-- -->17 0x000056a047df028f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x549428f)
#<!-- -->18 0x000056a047ca8ee0 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&amp;) (/opt/wandbox/clang-head/bin/clang+++0x534cee0)
#<!-- -->19 0x000056a04767dc34 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d21c34)
#<!-- -->20 0x000056a04767bfb1 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d1ffb1)
#<!-- -->21 0x000056a04770a4ee clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dae4ee)
#<!-- -->22 0x000056a0477092d3 clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dad2d3)
#<!-- -->23 0x000056a0477091d6 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/wandbox/clang-head/bin/clang+++0x4dad1d6)
#<!-- -->24 0x000056a04767ab95 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/wandbox/clang-head/bin/clang+++0x4d1eb95)
#<!-- -->25 0x000056a04765c84e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/wandbox/clang-head/bin/clang+++0x4d0084e)
#<!-- -->26 0x000056a04765b876 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/wandbox/clang-head/bin/clang+++0x4cff876)
#<!-- -->27 0x000056a0476580de clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x4cfc0de)
#<!-- -->28 0x000056a046287e66 clang::FrontendAction::Execute() (/opt/wandbox/clang-head/bin/clang+++0x392be66)
#<!-- -->29 0x000056a046201234 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/wandbox/clang-head/bin/clang+++0x38a5234)
#<!-- -->30 0x000056a04634ea6e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/wandbox/clang-head/bin/clang+++0x39f2a6e)
#<!-- -->31 0x000056a0438ddca0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/wandbox/clang-head/bin/clang+++0xf81ca0)
#<!-- -->32 0x000056a0438dac61 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x000056a04605a649 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->34 0x000056a0458f73dc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/wandbox/clang-head/bin/clang+++0x2f9b3dc)
#<!-- -->35 0x000056a04605a086 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/opt/wandbox/clang-head/bin/clang+++0x36fe086)
#<!-- -->36 0x000056a04601eb07 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2b07)
#<!-- -->37 0x000056a04601ed6e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2d6e)
#<!-- -->38 0x000056a04603a6e0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/wandbox/clang-head/bin/clang+++0x36de6e0)
#<!-- -->39 0x000056a0438da40f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/wandbox/clang-head/bin/clang+++0xf7e40f)
#<!-- -->40 0x000056a0438e7f37 main (/opt/wandbox/clang-head/bin/clang+++0xf8bf37)
#<!-- -->41 0x00007d127542a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->42 0x00007d127542a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->43 0x000056a0438d8a05 _start (/opt/wandbox/clang-head/bin/clang+++0xf7ca05)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git b84218526d364cb9fda23bfca164d25a7b03a641)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
clang++: note: diagnostic msg: Error generating preprocessed source(s) - ignoring input from stdin.
clang++: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

### EXPECTED COMPILER OUTPUT
(non-ICE error)

</details>


---

### Comment 4 - hubert-reinterpretcast

@EugeneZelenko, this is a Sema issue because we never should have gotten to codegen.

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hubert Tong (hubert-reinterpretcast)

<details>
The following source attempts to explicitly instantiate something that is not completely-specialized.
Clang ICEs.

With assertions enabled, we see "non-canonical or dependent type in IR-generation".

Online compiler link: https://godbolt.org/z/WvKobK4P7

### SOURCE (`&lt;stdin&gt;`)
```cpp
template &lt;typename T&gt;
void f(auto) {}

template void f&lt;int&gt;(auto);
```

### COMPILER INVOCATION
```
clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
```

### ACTUAL COMPILER OUTPUT (without assertions enabled)
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: /opt/wandbox/clang-head/bin/clang++ -std=c++20 -Wall -Wextra -Werror -pedantic-errors -S -o /dev/null -xc++ -
1.      &lt;stdin&gt;:4:27: current parser token ';'
2.      &lt;stdin&gt;:2:6: instantiating function definition 'f&lt;int, type-parameter-0-0&gt;'
3.      &lt;stdin&gt;:2:6: LLVM IR generation of declaration 'f'
4.      &lt;stdin&gt;:2:6: Generating code for declaration 'f'
 #<!-- -->0 0x000056a045979a98 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/wandbox/clang-head/bin/clang+++0x301da98)
 #<!-- -->1 0x000056a04597788e llvm::sys::RunSignalHandlers() (/opt/wandbox/clang-head/bin/clang+++0x301b88e)
 #<!-- -->2 0x000056a0458f75f9 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007d1275445320 (/lib/x86_64-linux-gnu/libc.so.6+0x45320)
 #<!-- -->4 0x000056a045c33a68 clang::CodeGen::CodeGenFunction::EmitReturnBlock() (/opt/wandbox/clang-head/bin/clang+++0x32d7a68)
 #<!-- -->5 0x000056a045d32f3c clang::CodeGen::isAggregateTypeForABI(clang::QualType) (/opt/wandbox/clang-head/bin/clang+++0x33d6f3c)
 #<!-- -->6 0x000056a045d27c9e (anonymous namespace)::X86_64ABIInfo::getIndirectResult(clang::QualType, unsigned int) const X86.cpp:0:0
 #<!-- -->7 0x000056a045d2533d (anonymous namespace)::X86_64ABIInfo::computeInfo(clang::CodeGen::CGFunctionInfo&amp;) const X86.cpp:0:0
 #<!-- -->8 0x000056a045e7aab1 clang::CodeGen::CodeGenTypes::arrangeLLVMFunctionInfo(clang::CanQual&lt;clang::Type&gt;, clang::CodeGen::FnInfoOpts, llvm::ArrayRef&lt;clang::CanQual&lt;clang::Type&gt;&gt;, clang::FunctionType::ExtInfo, llvm::ArrayRef&lt;clang::FunctionType::ExtParameterInfo&gt;, clang::CodeGen::RequiredArgs) (/opt/wandbox/clang-head/bin/clang+++0x351eab1)
 #<!-- -->9 0x000056a045e7ae8e arrangeLLVMFunctionInfo(clang::CodeGen::CodeGenTypes&amp;, bool, llvm::SmallVectorImpl&lt;clang::CanQual&lt;clang::Type&gt;&gt;&amp;, clang::CanQual&lt;clang::FunctionProtoType&gt;) CGCall.cpp:0:0
#<!-- -->10 0x000056a045e7c3aa clang::CodeGen::CodeGenTypes::arrangeFunctionDeclaration(clang::FunctionDecl const*) (/opt/wandbox/clang-head/bin/clang+++0x35203aa)
#<!-- -->11 0x000056a045c5d188 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x3301188)
#<!-- -->12 0x000056a045c565fa clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/wandbox/clang-head/bin/clang+++0x32fa5fa)
#<!-- -->13 0x000056a045c5a48b clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/wandbox/clang-head/bin/clang+++0x32fe48b)
#<!-- -->14 0x000056a045c55600 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/opt/wandbox/clang-head/bin/clang+++0x32f9600)
#<!-- -->15 0x000056a04601a5fc (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->16 0x000056a046013b7d clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/wandbox/clang-head/bin/clang+++0x36b7b7d)
#<!-- -->17 0x000056a047df028f clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x549428f)
#<!-- -->18 0x000056a047ca8ee0 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&amp;) (/opt/wandbox/clang-head/bin/clang+++0x534cee0)
#<!-- -->19 0x000056a04767dc34 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d21c34)
#<!-- -->20 0x000056a04767bfb1 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/wandbox/clang-head/bin/clang+++0x4d1ffb1)
#<!-- -->21 0x000056a04770a4ee clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dae4ee)
#<!-- -->22 0x000056a0477092d3 clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/wandbox/clang-head/bin/clang+++0x4dad2d3)
#<!-- -->23 0x000056a0477091d6 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/wandbox/clang-head/bin/clang+++0x4dad1d6)
#<!-- -->24 0x000056a04767ab95 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/wandbox/clang-head/bin/clang+++0x4d1eb95)
#<!-- -->25 0x000056a04765c84e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/wandbox/clang-head/bin/clang+++0x4d0084e)
#<!-- -->26 0x000056a04765b876 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/wandbox/clang-head/bin/clang+++0x4cff876)
#<!-- -->27 0x000056a0476580de clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/wandbox/clang-head/bin/clang+++0x4cfc0de)
#<!-- -->28 0x000056a046287e66 clang::FrontendAction::Execute() (/opt/wandbox/clang-head/bin/clang+++0x392be66)
#<!-- -->29 0x000056a046201234 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/wandbox/clang-head/bin/clang+++0x38a5234)
#<!-- -->30 0x000056a04634ea6e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/wandbox/clang-head/bin/clang+++0x39f2a6e)
#<!-- -->31 0x000056a0438ddca0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/wandbox/clang-head/bin/clang+++0xf81ca0)
#<!-- -->32 0x000056a0438dac61 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->33 0x000056a04605a649 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) Job.cpp:0:0
#<!-- -->34 0x000056a0458f73dc llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/wandbox/clang-head/bin/clang+++0x2f9b3dc)
#<!-- -->35 0x000056a04605a086 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/opt/wandbox/clang-head/bin/clang+++0x36fe086)
#<!-- -->36 0x000056a04601eb07 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2b07)
#<!-- -->37 0x000056a04601ed6e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/wandbox/clang-head/bin/clang+++0x36c2d6e)
#<!-- -->38 0x000056a04603a6e0 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/wandbox/clang-head/bin/clang+++0x36de6e0)
#<!-- -->39 0x000056a0438da40f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/wandbox/clang-head/bin/clang+++0xf7e40f)
#<!-- -->40 0x000056a0438e7f37 main (/opt/wandbox/clang-head/bin/clang+++0xf8bf37)
#<!-- -->41 0x00007d127542a1ca (/lib/x86_64-linux-gnu/libc.so.6+0x2a1ca)
#<!-- -->42 0x00007d127542a28b __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x2a28b)
#<!-- -->43 0x000056a0438d8a05 _start (/opt/wandbox/clang-head/bin/clang+++0xf7ca05)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 20.0.0git (https://github.com/llvm/llvm-project.git b84218526d364cb9fda23bfca164d25a7b03a641)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/wandbox/clang-head/bin
clang++: note: diagnostic msg: Error generating preprocessed source(s) - ignoring input from stdin.
clang++: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

### EXPECTED COMPILER OUTPUT
(non-ICE error)

</details>


---

### Comment 6 - shafik

Looks like this goes back to clang-10

---

