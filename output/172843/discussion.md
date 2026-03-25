# Clang crashes with assertion failure in DiagnoseBadDeduction when specializing template with __seg_gs pointer qualifier mismatch

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/172843

## Body

### Link for quick verification: https://godbolt.org/z/reEeanex3
```c
struct S { int i; }; 

template <typename T>
int baz(__seg_gs T* o) {
  return o->i; 
}

template<>
int baz(struct S* s) {
  return s->i ; 
}
```
## Description
Go back to `clang 14.0.0 (assertions)` and crashed only in C++ mode.
The crash occurs during template argument deduction when a specialization omits the `__seg_gs` qualifier that was present in the primary template.

## Stack dump
```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:12266: void DiagnoseBadDeduction(clang::Sema&, clang::NamedDecl*, clang::Decl*, clang::DeductionFailureInfo&, unsigned int, bool): Assertion `S.Context.hasSameType(Param, NonCanonParam)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:9:22: current parser token '{'
 #0 0x000000000429e098 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429e098)
 #1 0x000000000429af44 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429af44)
 #2 0x000000000429b4c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429b4c4)
 #3 0x00000000041e0208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x00007d8dcbc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x00007d8dcbc969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x00007d8dcbc42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x00007d8dcbc287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x00007d8dcbc2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x00007d8dcbc39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x000000000727ed50 DiagnoseBadDeduction(clang::Sema&, clang::NamedDecl*, clang::Decl*, clang::DeductionFailureInfo&, unsigned int, bool) SemaOverload.cpp:0:0
#11 0x000000000727f1dd clang::TemplateSpecCandidateSet::NoteCandidates(clang::Sema&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x727f1dd)
#12 0x00000000074223cb clang::Sema::getMostSpecialized(clang::UnresolvedSetIterator, clang::UnresolvedSetIterator, clang::TemplateSpecCandidateSet&, clang::SourceLocation, clang::PartialDiagnostic const&, clang::PartialDiagnostic const&, clang::PartialDiagnostic const&, bool, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74223cb)
#13 0x00000000073b38a8 clang::Sema::CheckFunctionTemplateSpecialization(clang::FunctionDecl*, clang::TemplateArgumentListInfo*, clang::LookupResult&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73b38a8)
#14 0x0000000006d8ebc9 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d8ebc9)
#15 0x0000000006d91051 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d91051)
#16 0x0000000006d91dc8 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d91dc8)
#17 0x000000000699e46a clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699e46a)
#18 0x00000000069dec8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69dec8d)
#19 0x0000000006aa92a8 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa92a8)
#20 0x0000000006ab6b49 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#21 0x0000000006ab6f5a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab6f5a)
#22 0x00000000069e3de0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e3de0)
#23 0x00000000069a2c17 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a2c17)
#24 0x00000000069a3ae5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3ae5)
#25 0x000000000698176a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698176a)
#26 0x0000000004bf5da8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf5da8)
#27 0x0000000004eea105 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eea105)
#28 0x0000000004e6a3be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6a3be)
#29 0x0000000004fe41ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fe41ed)
#30 0x0000000000dd9ef2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9ef2)
#31 0x0000000000dd091a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#32 0x0000000000dd0a9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#33 0x0000000004c67a89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x00000000041e06a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e06a4)
#35 0x0000000004c680b6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000004c29732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c29732)
#37 0x0000000004c2a6de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2a6de)
#38 0x0000000004c319b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c319b5)
#39 0x0000000000dd62c1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd62c1)
#40 0x0000000000c813d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc813d4)
#41 0x00007d8dcbc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x00007d8dcbc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000dd03b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd03b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/reEeanex3
```c
struct S { int i; }; 

template &lt;typename T&gt;
int baz(__seg_gs T* o) {
  return o-&gt;i; 
}

template&lt;&gt;
int baz(struct S* s) {
  return s-&gt;i ; 
}
```
## Description
Go back to `clang 14.0.0 (assertions)` and crashed only in C++ mode.
The crash occurs during template argument deduction when a specialization omits the `__seg_gs` qualifier that was present in the primary template.

## Stack dump
```
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaOverload.cpp:12266: void DiagnoseBadDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::Decl*, clang::DeductionFailureInfo&amp;, unsigned int, bool): Assertion `S.Context.hasSameType(Param, NonCanonParam)' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:9:22: current parser token '{'
 #<!-- -->0 0x000000000429e098 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429e098)
 #<!-- -->1 0x000000000429af44 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429af44)
 #<!-- -->2 0x000000000429b4c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429b4c4)
 #<!-- -->3 0x00000000041e0208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x00007d8dcbc42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x00007d8dcbc969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x00007d8dcbc42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x00007d8dcbc287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x00007d8dcbc2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x00007d8dcbc39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x000000000727ed50 DiagnoseBadDeduction(clang::Sema&amp;, clang::NamedDecl*, clang::Decl*, clang::DeductionFailureInfo&amp;, unsigned int, bool) SemaOverload.cpp:0:0
#<!-- -->11 0x000000000727f1dd clang::TemplateSpecCandidateSet::NoteCandidates(clang::Sema&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x727f1dd)
#<!-- -->12 0x00000000074223cb clang::Sema::getMostSpecialized(clang::UnresolvedSetIterator, clang::UnresolvedSetIterator, clang::TemplateSpecCandidateSet&amp;, clang::SourceLocation, clang::PartialDiagnostic const&amp;, clang::PartialDiagnostic const&amp;, clang::PartialDiagnostic const&amp;, bool, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x74223cb)
#<!-- -->13 0x00000000073b38a8 clang::Sema::CheckFunctionTemplateSpecialization(clang::FunctionDecl*, clang::TemplateArgumentListInfo*, clang::LookupResult&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73b38a8)
#<!-- -->14 0x0000000006d8ebc9 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d8ebc9)
#<!-- -->15 0x0000000006d91051 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d91051)
#<!-- -->16 0x0000000006d91dc8 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d91dc8)
#<!-- -->17 0x000000000699e46a clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699e46a)
#<!-- -->18 0x00000000069dec8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69dec8d)
#<!-- -->19 0x0000000006aa92a8 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa92a8)
#<!-- -->20 0x0000000006ab6b49 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#<!-- -->21 0x0000000006ab6f5a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ab6f5a)
#<!-- -->22 0x00000000069e3de0 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e3de0)
#<!-- -->23 0x00000000069a2c17 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a2c17)
#<!-- -->24 0x00000000069a3ae5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3ae5)
#<!-- -->25 0x000000000698176a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698176a)
#<!-- -->26 0x0000000004bf5da8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf5da8)
#<!-- -->27 0x0000000004eea105 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eea105)
#<!-- -->28 0x0000000004e6a3be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6a3be)
#<!-- -->29 0x0000000004fe41ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fe41ed)
#<!-- -->30 0x0000000000dd9ef2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9ef2)
#<!-- -->31 0x0000000000dd091a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->32 0x0000000000dd0a9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->33 0x0000000004c67a89 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x00000000041e06a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e06a4)
#<!-- -->35 0x0000000004c680b6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x0000000004c29732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c29732)
#<!-- -->37 0x0000000004c2a6de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2a6de)
#<!-- -->38 0x0000000004c319b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c319b5)
#<!-- -->39 0x0000000000dd62c1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd62c1)
#<!-- -->40 0x0000000000c813d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc813d4)
#<!-- -->41 0x00007d8dcbc29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x00007d8dcbc29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000dd03b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd03b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - akparmar004

hey @shafik,
i'm new to llvm, does this issue need to be resolved?
because i've built the latest code, but not getting the exact stack backtrace..
just got an error


---

### Comment 3 - shafik

> hey [@shafik](https://github.com/shafik), i'm new to llvm, does this issue need to be resolved? because i've built the latest code, but not getting the exact stack backtrace.. just got an error

You need an assertions build to obtain the error.

---

