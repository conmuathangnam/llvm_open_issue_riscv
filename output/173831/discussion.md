# [clang] Assertion `Align != 0u && "Align can't be 0."' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/173831
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2025-12-30T00:40:24Z

## Body

Reproducer:
https://godbolt.org/z/bvredrYsv
```cpp
typedef char char16 __attribute__((ext_vector_type(0x11112222)));

typedef struct {
  char16 c0;
} struct_vc1;

struct_vc1 return_struct_vc1(void) {
  struct_vc1 result = {};
  return result;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/MathExtras.h:479: constexpr T llvm::alignTo(U, V) [with U = long int; V = long int; T = long unsigned int]: Assertion `Align != 0u && "Align can't be 0."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:9:16: current parser token ';'
2.	<source>:7:36: parsing function body 'return_struct_vc1'
3.	<source>:7:36: in compound statement ('{}')
 #0 0x0000000004301458 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4301458)
 #1 0x00000000042fe884 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fe884)
 #2 0x0000000004243628 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000073a0e1442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000073a0e14969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000073a0e1442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000073a0e14287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000073a0e142871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000073a0e1439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000080b2dd0 unsigned long llvm::alignTo<unsigned long, unsigned int, unsigned long>(unsigned long, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80b2dd0)
#10 0x00000000080be511 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(clang::FieldDecl const*, bool) RecordLayoutBuilder.cpp:0:0
#11 0x00000000080bff92 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutFields(clang::RecordDecl const*) RecordLayoutBuilder.cpp:0:0
#12 0x00000000080b9cc9 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80b9cc9)
#13 0x0000000007996f27 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7996f27)
#14 0x0000000007993fd7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7993fd7)
#15 0x00000000079970ed clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79970ed)
#16 0x0000000007993fd7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7993fd7)
#17 0x0000000007996a57 clang::ASTContext::getTypeAlignInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7996a57)
#18 0x0000000007381de1 clang::Sema::getNamedReturnInfo(clang::VarDecl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7381de1)
#19 0x0000000007381f2e clang::Sema::getNamedReturnInfo(clang::Expr*&, clang::Sema::SimplerImplicitMoveMode) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7381f2e)
#20 0x0000000007395d4d clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7395d4d)
#21 0x00000000073972a7 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73972a7)
#22 0x0000000006b2c79a clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b2c79a)
#23 0x0000000006b30bdd clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b30bdd)
#24 0x0000000006b31af6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b31af6)
#25 0x0000000006b3a43e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3a43e)
#26 0x0000000006b3ac8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3ac8a)
#27 0x0000000006a40beb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a40beb)
#28 0x0000000006a770ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a770ad)
#29 0x0000000006a33d4b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a33d4b)
#30 0x0000000006a3453f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3453f)
#31 0x0000000006a3c36a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3c36a)
#32 0x0000000006a3d305 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3d305)
#33 0x0000000006a1e38a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e38a)
#34 0x0000000004c62788 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c62788)
#35 0x0000000004f58e05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f58e05)
#36 0x0000000004ed909e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed909e)
#37 0x000000000505318d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505318d)
#38 0x0000000000de63ec cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde63ec)
#39 0x0000000000ddce3a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#40 0x0000000000ddcfbd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#41 0x0000000004cd4e59 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x0000000004243ac4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4243ac4)
#43 0x0000000004cd546f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x0000000004c95f52 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c95f52)
#45 0x0000000004c96efe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c96efe)
#46 0x0000000004c9e355 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9e355)
#47 0x0000000000de27e1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde27e1)
#48 0x0000000000c8d114 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d114)
#49 0x000073a0e1429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x000073a0e1429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000ddc8d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc8d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-11.0.0:
https://godbolt.org/z/re4dT55E1

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/bvredrYsv
```cpp
typedef char char16 __attribute__((ext_vector_type(0x11112222)));

typedef struct {
  char16 c0;
} struct_vc1;

struct_vc1 return_struct_vc1(void) {
  struct_vc1 result = {};
  return result;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/MathExtras.h:479: constexpr T llvm::alignTo(U, V) [with U = long int; V = long int; T = long unsigned int]: Assertion `Align != 0u &amp;&amp; "Align can't be 0."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:9:16: current parser token ';'
2.	&lt;source&gt;:7:36: parsing function body 'return_struct_vc1'
3.	&lt;source&gt;:7:36: in compound statement ('{}')
 #<!-- -->0 0x0000000004301458 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4301458)
 #<!-- -->1 0x00000000042fe884 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42fe884)
 #<!-- -->2 0x0000000004243628 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000073a0e1442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000073a0e14969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000073a0e1442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000073a0e14287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000073a0e142871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000073a0e1439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000080b2dd0 unsigned long llvm::alignTo&lt;unsigned long, unsigned int, unsigned long&gt;(unsigned long, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80b2dd0)
#<!-- -->10 0x00000000080be511 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(clang::FieldDecl const*, bool) RecordLayoutBuilder.cpp:0:0
#<!-- -->11 0x00000000080bff92 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutFields(clang::RecordDecl const*) RecordLayoutBuilder.cpp:0:0
#<!-- -->12 0x00000000080b9cc9 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80b9cc9)
#<!-- -->13 0x0000000007996f27 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7996f27)
#<!-- -->14 0x0000000007993fd7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7993fd7)
#<!-- -->15 0x00000000079970ed clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79970ed)
#<!-- -->16 0x0000000007993fd7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7993fd7)
#<!-- -->17 0x0000000007996a57 clang::ASTContext::getTypeAlignInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7996a57)
#<!-- -->18 0x0000000007381de1 clang::Sema::getNamedReturnInfo(clang::VarDecl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7381de1)
#<!-- -->19 0x0000000007381f2e clang::Sema::getNamedReturnInfo(clang::Expr*&amp;, clang::Sema::SimplerImplicitMoveMode) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7381f2e)
#<!-- -->20 0x0000000007395d4d clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7395d4d)
#<!-- -->21 0x00000000073972a7 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73972a7)
#<!-- -->22 0x0000000006b2c79a clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b2c79a)
#<!-- -->23 0x0000000006b30bdd clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b30bdd)
#<!-- -->24 0x0000000006b31af6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b31af6)
#<!-- -->25 0x0000000006b3a43e clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3a43e)
#<!-- -->26 0x0000000006b3ac8a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3ac8a)
#<!-- -->27 0x0000000006a40beb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a40beb)
#<!-- -->28 0x0000000006a770ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a770ad)
#<!-- -->29 0x0000000006a33d4b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a33d4b)
#<!-- -->30 0x0000000006a3453f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3453f)
#<!-- -->31 0x0000000006a3c36a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3c36a)
#<!-- -->32 0x0000000006a3d305 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3d305)
#<!-- -->33 0x0000000006a1e38a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1e38a)
#<!-- -->34 0x0000000004c62788 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c62788)
#<!-- -->35 0x0000000004f58e05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f58e05)
#<!-- -->36 0x0000000004ed909e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed909e)
#<!-- -->37 0x000000000505318d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505318d)
#<!-- -->38 0x0000000000de63ec cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde63ec)
#<!-- -->39 0x0000000000ddce3a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->40 0x0000000000ddcfbd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->41 0x0000000004cd4e59 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x0000000004243ac4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4243ac4)
#<!-- -->43 0x0000000004cd546f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x0000000004c95f52 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c95f52)
#<!-- -->45 0x0000000004c96efe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c96efe)
#<!-- -->46 0x0000000004c9e355 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9e355)
#<!-- -->47 0x0000000000de27e1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde27e1)
#<!-- -->48 0x0000000000c8d114 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8d114)
#<!-- -->49 0x000073a0e1429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->50 0x000073a0e1429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->51 0x0000000000ddc8d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc8d5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

Goes back to clang-11.0.0:
https://godbolt.org/z/re4dT55E1
</details>


---

