# [clang] Assertion `UD.shadow_size() && "We shouldn't be codegening an invalid UsingEnumDecl" " containing no decls"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/162485
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Reproducer:
https://godbolt.org/z/o8a5rTb7v
```cpp
namespace foo {
enum class EnumOne {};
}
using foo::EnumOne;

template <class> void t() { using enum EnumOne; }
template void t<void>();
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGDebugInfo.cpp:6253: void clang::CodeGen::CGDebugInfo::EmitUsingEnumDecl(const clang::UsingEnumDecl&): Assertion `UD.shadow_size() && "We shouldn't be codegening an invalid UsingEnumDecl" " containing no decls"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -g <source>
1.	<source>:7:24: current parser token ';'
2.	<source>:6:23: instantiating function definition 't<void>'
3.	<source>:6:23: LLVM IR generation of declaration 't'
4.	<source>:6:23: Generating code for declaration 't'
 #0 0x00000000041d1278 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41d1278)
 #1 0x00000000041ce6a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ce6a4)
 #2 0x0000000004112ff8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f7d87842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f7d878969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f7d87842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f7d878287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007f7d8782871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007f7d87839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000044f63d1 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44f63d1)
#10 0x00000000046b0bec clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46b0bec)
#11 0x00000000046c6feb clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46c6feb)
#12 0x00000000046be671 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46be671)
#13 0x00000000046c697c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46c697c)
#14 0x000000000472efce clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x472efce)
#15 0x0000000004740fe4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4740fe4)
#16 0x00000000047add90 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47add90)
#17 0x00000000047a8c74 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47a8c74)
#18 0x00000000047a9b10 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47a9b10)
#19 0x00000000047b42e3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#20 0x0000000004b18859 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#21 0x0000000004b07984 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b07984)
#22 0x0000000007498d6a clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7498d6a)
#23 0x00000000072d29f2 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72d29f2)
#24 0x00000000068f3d6d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f3d6d)
#25 0x00000000069013f9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69013f9)
#26 0x00000000069d1188 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d1188)
#27 0x00000000069d1cdc clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d1cdc)
#28 0x00000000069de848 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69de848)
#29 0x000000000690a700 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a700)
#30 0x00000000068c6df7 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c6df7)
#31 0x00000000068c7cc5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c7cc5)
#32 0x00000000068a932a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68a932a)
#33 0x0000000004b14258 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b14258)
#34 0x0000000004dffe65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dffe65)
#35 0x0000000004d81abe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d81abe)
#36 0x0000000004ef700d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef700d)
#37 0x0000000000dbe180 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbe180)
#38 0x0000000000db4cea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#39 0x0000000000db4e6d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#40 0x0000000004b7d409 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000004113494 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4113494)
#42 0x0000000004b7da1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x0000000004b3ed22 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3ed22)
#44 0x0000000004b3fcce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3fcce)
#45 0x0000000004b47405 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b47405)
#46 0x0000000000dba6a9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdba6a9)
#47 0x0000000000c6ab74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6ab74)
#48 0x00007f7d87829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x00007f7d87829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000db4785 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb4785)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/o8a5rTb7v
```cpp
namespace foo {
enum class EnumOne {};
}
using foo::EnumOne;

template &lt;class&gt; void t() { using enum EnumOne; }
template void t&lt;void&gt;();
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGDebugInfo.cpp:6253: void clang::CodeGen::CGDebugInfo::EmitUsingEnumDecl(const clang::UsingEnumDecl&amp;): Assertion `UD.shadow_size() &amp;&amp; "We shouldn't be codegening an invalid UsingEnumDecl" " containing no decls"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -g &lt;source&gt;
1.	&lt;source&gt;:7:24: current parser token ';'
2.	&lt;source&gt;:6:23: instantiating function definition 't&lt;void&gt;'
3.	&lt;source&gt;:6:23: LLVM IR generation of declaration 't'
4.	&lt;source&gt;:6:23: Generating code for declaration 't'
 #<!-- -->0 0x00000000041d1278 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41d1278)
 #<!-- -->1 0x00000000041ce6a4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ce6a4)
 #<!-- -->2 0x0000000004112ff8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f7d87842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f7d878969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007f7d87842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007f7d878287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007f7d8782871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007f7d87839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000044f63d1 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44f63d1)
#<!-- -->10 0x00000000046b0bec clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46b0bec)
#<!-- -->11 0x00000000046c6feb clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46c6feb)
#<!-- -->12 0x00000000046be671 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46be671)
#<!-- -->13 0x00000000046c697c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46c697c)
#<!-- -->14 0x000000000472efce clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x472efce)
#<!-- -->15 0x0000000004740fe4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4740fe4)
#<!-- -->16 0x00000000047add90 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47add90)
#<!-- -->17 0x00000000047a8c74 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47a8c74)
#<!-- -->18 0x00000000047a9b10 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47a9b10)
#<!-- -->19 0x00000000047b42e3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->20 0x0000000004b18859 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->21 0x0000000004b07984 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b07984)
#<!-- -->22 0x0000000007498d6a clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7498d6a)
#<!-- -->23 0x00000000072d29f2 clang::Sema::ActOnExplicitInstantiation(clang::Scope*, clang::SourceLocation, clang::SourceLocation, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72d29f2)
#<!-- -->24 0x00000000068f3d6d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68f3d6d)
#<!-- -->25 0x00000000069013f9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69013f9)
#<!-- -->26 0x00000000069d1188 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject&amp;, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d1188)
#<!-- -->27 0x00000000069d1cdc clang::Parser::ParseExplicitInstantiation(clang::DeclaratorContext, clang::SourceLocation, clang::SourceLocation, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d1cdc)
#<!-- -->28 0x00000000069de848 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69de848)
#<!-- -->29 0x000000000690a700 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690a700)
#<!-- -->30 0x00000000068c6df7 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c6df7)
#<!-- -->31 0x00000000068c7cc5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c7cc5)
#<!-- -->32 0x00000000068a932a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68a932a)
#<!-- -->33 0x0000000004b14258 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b14258)
#<!-- -->34 0x0000000004dffe65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dffe65)
#<!-- -->35 0x0000000004d81abe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d81abe)
#<!-- -->36 0x0000000004ef700d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef700d)
#<!-- -->37 0x0000000000dbe180 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbe180)
#<!-- -->38 0x0000000000db4cea ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->39 0x0000000000db4e6d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->40 0x0000000004b7d409 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x0000000004113494 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4113494)
#<!-- -->42 0x0000000004b7da1f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x0000000004b3ed22 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3ed22)
#<!-- -->44 0x0000000004b3fcce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3fcce)
#<!-- -->45 0x0000000004b47405 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b47405)
#<!-- -->46 0x0000000000dba6a9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdba6a9)
#<!-- -->47 0x0000000000c6ab74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6ab74)
#<!-- -->48 0x00007f7d87829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x00007f7d87829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000db4785 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb4785)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-13.0.0:
https://godbolt.org/z/6KonW9dba

---

### Comment 3 - Thibault-Monnier

Here's the assertion's location: https://github.com/llvm/llvm-project/blob/01ac00b6d7c678840f66c456bf37ada24f3d0427/clang/lib/CodeGen/CGDebugInfo.cpp#L6250

And here's a more minimal reproducer: ~~https://godbolt.org/z/E1Mezj66o~~ (oudated).

---

### Comment 4 - Thibault-Monnier

In fact, the issue arises from `using` an empty enum: https://godbolt.org/z/78h89MTKK.

---

### Comment 5 - k-arrows

My reproducer comes from the following:
https://github.com/llvm/llvm-project/blob/main/clang/test/SemaTemplate/using-decl.cpp

---

