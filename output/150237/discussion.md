# [x86] compiling a program for Arm with assertion-enabled clang causes a crash

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/150237
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-07-24T08:42:06Z

## Body

Reproducer:
https://godbolt.org/z/7hefx6sEv
```c
__attribute__((target_clones("sve", "simd"))) void ok_arm_streaming(void) __arm_streaming {}
```

Backtrace:
```console
clang: /root/llvm-project/llvm/tools/clang/include/clang/Basic/Diagnostic.h:1575: clang::DiagnosticsEngine::ArgumentKind clang::Diagnostic::getArgKind(unsigned int) const: Assertion `Idx < getNumArgs() && "Argument index out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:1:91: current parser token '{'
 #0 0x0000000003fc9168 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fc9168)
 #1 0x0000000003fc6594 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fc6594)
 #2 0x0000000003f0ad68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000079174b842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000079174b8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000079174b842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000079174b8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000079174b82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000079174b839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000041f6bec clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl<char>&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f6bec)
#10 0x0000000004ccc2f6 clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ccc2f6)
#11 0x00000000041f4029 clang::DiagnosticsEngine::Report(clang::DiagnosticsEngine::Level, clang::Diagnostic const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f4029)
#12 0x00000000041f4725 clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f4725)
#13 0x00000000041f4805 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f4805)
#14 0x000000000680d1c4 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x680d1c4)
#15 0x0000000006888a62 clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6888a62)
#16 0x00000000067fa3c8 clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67fa3c8)
#17 0x0000000006a6e732 clang::Sema::CheckAttrTarget(clang::ParsedAttr const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a6e732)
#18 0x000000000727a17f handleFunctionTypeAttr((anonymous namespace)::TypeProcessingState&, clang::ParsedAttr&, clang::QualType&, clang::CUDAFunctionTarget) SemaType.cpp:0:0
#19 0x0000000007287e7a processTypeAttrs((anonymous namespace)::TypeProcessingState&, clang::QualType&, TypeAttrLocation, clang::ParsedAttributesView const&, clang::CUDAFunctionTarget) SemaType.cpp:0:0
#20 0x00000000072939c0 GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#21 0x0000000007299e0f clang::Sema::GetTypeForDeclarator(clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7299e0f)
#22 0x0000000006a55523 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a55523)
#23 0x0000000006a56638 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a56638)
#24 0x000000000667eb88 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667eb88)
#25 0x00000000066b522d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66b522d)
#26 0x00000000066721fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66721fe)
#27 0x0000000006672999 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6672999)
#28 0x000000000667a69a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667a69a)
#29 0x000000000667b635 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667b635)
#30 0x000000000667baf0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667baf0)
#31 0x000000000666d363 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x666d363)
#32 0x0000000004977108 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4977108)
#33 0x0000000004c69e45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c69e45)
#34 0x0000000004be6b3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4be6b3e)
#35 0x0000000004d5db71 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d5db71)
#36 0x0000000000d9fd9f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd9fd9f)
#37 0x0000000000d969fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#38 0x00000000049de109 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x0000000003f0b204 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f0b204)
#40 0x00000000049de71f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x00000000049a0ced clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a0ced)
#42 0x00000000049a1d7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a1d7e)
#43 0x00000000049a9de5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a9de5)
#44 0x0000000000d9c24f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd9c24f)
#45 0x0000000000c50784 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc50784)
#46 0x000079174b829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x000079174b829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000d964a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd964a5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/7hefx6sEv
```c
__attribute__((target_clones("sve", "simd"))) void ok_arm_streaming(void) __arm_streaming {}
```

Backtrace:
```console
clang: /root/llvm-project/llvm/tools/clang/include/clang/Basic/Diagnostic.h:1575: clang::DiagnosticsEngine::ArgumentKind clang::Diagnostic::getArgKind(unsigned int) const: Assertion `Idx &lt; getNumArgs() &amp;&amp; "Argument index out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:1:91: current parser token '{'
 #<!-- -->0 0x0000000003fc9168 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fc9168)
 #<!-- -->1 0x0000000003fc6594 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fc6594)
 #<!-- -->2 0x0000000003f0ad68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000079174b842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000079174b8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000079174b842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000079174b8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000079174b82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000079174b839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000041f6bec clang::Diagnostic::FormatDiagnostic(char const*, char const*, llvm::SmallVectorImpl&lt;char&gt;&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f6bec)
#<!-- -->10 0x0000000004ccc2f6 clang::TextDiagnosticPrinter::HandleDiagnostic(clang::DiagnosticsEngine::Level, clang::Diagnostic const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ccc2f6)
#<!-- -->11 0x00000000041f4029 clang::DiagnosticsEngine::Report(clang::DiagnosticsEngine::Level, clang::Diagnostic const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f4029)
#<!-- -->12 0x00000000041f4725 clang::DiagnosticsEngine::ProcessDiag(clang::DiagnosticBuilder const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f4725)
#<!-- -->13 0x00000000041f4805 clang::DiagnosticsEngine::EmitDiagnostic(clang::DiagnosticBuilder const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x41f4805)
#<!-- -->14 0x000000000680d1c4 clang::Sema::EmitDiagnostic(unsigned int, clang::DiagnosticBuilder const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x680d1c4)
#<!-- -->15 0x0000000006888a62 clang::SemaBase::ImmediateDiagBuilder::~ImmediateDiagBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6888a62)
#<!-- -->16 0x00000000067fa3c8 clang::SemaBase::SemaDiagnosticBuilder::~SemaDiagnosticBuilder() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67fa3c8)
#<!-- -->17 0x0000000006a6e732 clang::Sema::CheckAttrTarget(clang::ParsedAttr const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a6e732)
#<!-- -->18 0x000000000727a17f handleFunctionTypeAttr((anonymous namespace)::TypeProcessingState&amp;, clang::ParsedAttr&amp;, clang::QualType&amp;, clang::CUDAFunctionTarget) SemaType.cpp:0:0
#<!-- -->19 0x0000000007287e7a processTypeAttrs((anonymous namespace)::TypeProcessingState&amp;, clang::QualType&amp;, TypeAttrLocation, clang::ParsedAttributesView const&amp;, clang::CUDAFunctionTarget) SemaType.cpp:0:0
#<!-- -->20 0x00000000072939c0 GetFullTypeForDeclarator((anonymous namespace)::TypeProcessingState&amp;, clang::QualType, clang::TypeSourceInfo*) SemaType.cpp:0:0
#<!-- -->21 0x0000000007299e0f clang::Sema::GetTypeForDeclarator(clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7299e0f)
#<!-- -->22 0x0000000006a55523 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a55523)
#<!-- -->23 0x0000000006a56638 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a56638)
#<!-- -->24 0x000000000667eb88 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667eb88)
#<!-- -->25 0x00000000066b522d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66b522d)
#<!-- -->26 0x00000000066721fe clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66721fe)
#<!-- -->27 0x0000000006672999 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6672999)
#<!-- -->28 0x000000000667a69a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667a69a)
#<!-- -->29 0x000000000667b635 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667b635)
#<!-- -->30 0x000000000667baf0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x667baf0)
#<!-- -->31 0x000000000666d363 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x666d363)
#<!-- -->32 0x0000000004977108 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4977108)
#<!-- -->33 0x0000000004c69e45 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c69e45)
#<!-- -->34 0x0000000004be6b3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4be6b3e)
#<!-- -->35 0x0000000004d5db71 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d5db71)
#<!-- -->36 0x0000000000d9fd9f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd9fd9f)
#<!-- -->37 0x0000000000d969fa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->38 0x00000000049de109 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->39 0x0000000003f0b204 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f0b204)
#<!-- -->40 0x00000000049de71f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->41 0x00000000049a0ced clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a0ced)
#<!-- -->42 0x00000000049a1d7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a1d7e)
#<!-- -->43 0x00000000049a9de5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a9de5)
#<!-- -->44 0x0000000000d9c24f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd9c24f)
#<!-- -->45 0x0000000000c50784 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc50784)
#<!-- -->46 0x000079174b829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->47 0x000079174b829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->48 0x0000000000d964a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd964a5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

