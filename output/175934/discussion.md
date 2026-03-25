# [clang]Crash in `getASTRecordLayout` when a class template has a field of type `decltype(&static data member)`

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/175934

## Body

The following code causes crash on x86-64 clang(assertions) since 12.0.0, also reproduces on trunk, GCC/EDG/MSVC do not report an error:

### Minimal Reproduction https://godbolt.org/z/Mn7sbxv6x

```cpp
template <class T>
struct Y {
  static const int e = 1;
  decltype(&e) g;
};

int main() {
  Y<int> y;
}
```

Possibly related to #110041 because both involve `decltype(&static data member)`. In my view, these crashes appear to be separate issues, not the same root cause:
- #110041: crashes in CodeGen (`ConstantEmitter`) when emitting a global initializer; requires `auto x = a<int>::c;` to reproduce.  
- This report: crashes during record layout / type information (`ASTContext::getTypeInfoImpl`, `getASTRecordLayout`) and reproduces with only `Y<int> y;`.


### Compiler Output

```
clang++: /root/build/tools/clang/include/clang/AST/TypeNodes.inc:39: clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const: Assertion `!T->isDependentType() && "should not see dependent types here"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:8:11: current parser token ';'
2.	<source>:7:12: parsing function body 'main'
3.	<source>:7:12: in compound statement ('{}')
 #0 0x0000000004303798 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4303798)
 #1 0x0000000004300bf4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4300bf4)
 #2 0x0000000004242e28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e608ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e608ea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e608ea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e608ea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007e608ea2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007e608ea39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000079c0789 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79c0789)
#10 0x00000000079bcc27 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79bcc27)
#11 0x00000000079c0a9b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79c0a9b)
#12 0x00000000079c1793 clang::ASTContext::getTypeInfoInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79c1793)
#13 0x00000000080eb3a5 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(clang::FieldDecl const*, bool) RecordLayoutBuilder.cpp:0:0
#14 0x00000000080ecfd2 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutFields(clang::RecordDecl const*) RecordLayoutBuilder.cpp:0:0
#15 0x00000000080e6d09 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80e6d09)
#16 0x00000000079bfb77 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79bfb77)
#17 0x00000000079bcc27 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79bcc27)
#18 0x00000000079bf6a7 clang::ASTContext::getTypeAlignInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79bf6a7)
#19 0x0000000006ca9ba3 clang::Sema::CheckArgAlignment(clang::SourceLocation, clang::NamedDecl*, llvm::StringRef, clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca9ba3)
#20 0x0000000006cc93aa clang::Sema::CheckConstructorCall(clang::FunctionDecl*, clang::QualType, llvm::ArrayRef<clang::Expr const*>, clang::FunctionProtoType const*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc93aa)
#21 0x0000000006ea49e4 clang::Sema::CompleteConstructorCall(clang::CXXConstructorDecl*, clang::QualType, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, llvm::SmallVectorImpl<clang::Expr*>&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ea49e4)
#22 0x0000000007108d76 PerformConstructorInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::InitializationSequence::Step const&, bool&, bool, bool, clang::SourceLocation, clang::SourceLocation) SemaInit.cpp:0:0
#23 0x0000000007117499 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7117499)
#24 0x0000000006e36e43 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#25 0x0000000006a8b37b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8b37b)
#26 0x0000000006a990aa clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a990aa)
#27 0x0000000006aa1dbf clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa1dbf)
#28 0x0000000006aa228c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa228c)
#29 0x0000000006b537d4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b537d4)
#30 0x0000000006b54066 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b54066)
#31 0x0000000006b5c9ae clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5c9ae)
#32 0x0000000006b5d1fa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5d1fa)
#33 0x0000000006a6364b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6364b)
#34 0x0000000006a99abd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a99abd)
#35 0x0000000006a567ab clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a567ab)
#36 0x0000000006a56f9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a56f9f)
#37 0x0000000006a5edca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5edca)
#38 0x0000000006a5fd65 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5fd65)
#39 0x0000000006a40dea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a40dea)
#40 0x0000000004c6b818 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6b818)
#41 0x0000000004f65025 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f65025)
#42 0x0000000004ee4d3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee4d3e)
#43 0x000000000505f8bd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505f8bd)
#44 0x0000000000dd001e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd001e)
#45 0x0000000000dc69ea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#46 0x0000000000dc6b6d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#47 0x0000000004cdea09 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#48 0x00000000042432c4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42432c4)
#49 0x0000000004cdf01f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#50 0x0000000004c9f9f2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9f9f2)
#51 0x0000000004ca099e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca099e)
#52 0x0000000004ca7de5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca7de5)
#53 0x0000000000dcc391 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcc391)
#54 0x0000000000c89af4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc89af4)
#55 0x00007e608ea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#56 0x00007e608ea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#57 0x0000000000dc6485 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6485)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following code causes crash on x86-64 clang(assertions) since 12.0.0, also reproduces on trunk, GCC/EDG/MSVC do not report an error:

### Minimal Reproduction https://godbolt.org/z/Mn7sbxv6x

```cpp
template &lt;class T&gt;
struct Y {
  static const int e = 1;
  decltype(&amp;e) g;
};

int main() {
  Y&lt;int&gt; y;
}
```

Possibly related to #<!-- -->110041 because both involve `decltype(&amp;static data member)`. In my view, these crashes appear to be separate issues, not the same root cause:
- #<!-- -->110041: crashes in CodeGen (`ConstantEmitter`) when emitting a global initializer; requires `auto x = a&lt;int&gt;::c;` to reproduce.  
- This report: crashes during record layout / type information (`ASTContext::getTypeInfoImpl`, `getASTRecordLayout`) and reproduces with only `Y&lt;int&gt; y;`.


### Compiler Output

```
clang++: /root/build/tools/clang/include/clang/AST/TypeNodes.inc:39: clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const: Assertion `!T-&gt;isDependentType() &amp;&amp; "should not see dependent types here"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:8:11: current parser token ';'
2.	&lt;source&gt;:7:12: parsing function body 'main'
3.	&lt;source&gt;:7:12: in compound statement ('{}')
 #<!-- -->0 0x0000000004303798 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4303798)
 #<!-- -->1 0x0000000004300bf4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4300bf4)
 #<!-- -->2 0x0000000004242e28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e608ea42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e608ea969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e608ea42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e608ea287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007e608ea2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007e608ea39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000079c0789 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79c0789)
#<!-- -->10 0x00000000079bcc27 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79bcc27)
#<!-- -->11 0x00000000079c0a9b clang::ASTContext::getTypeInfoInChars(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79c0a9b)
#<!-- -->12 0x00000000079c1793 clang::ASTContext::getTypeInfoInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79c1793)
#<!-- -->13 0x00000000080eb3a5 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutField(clang::FieldDecl const*, bool) RecordLayoutBuilder.cpp:0:0
#<!-- -->14 0x00000000080ecfd2 (anonymous namespace)::ItaniumRecordLayoutBuilder::LayoutFields(clang::RecordDecl const*) RecordLayoutBuilder.cpp:0:0
#<!-- -->15 0x00000000080e6d09 clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x80e6d09)
#<!-- -->16 0x00000000079bfb77 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79bfb77)
#<!-- -->17 0x00000000079bcc27 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79bcc27)
#<!-- -->18 0x00000000079bf6a7 clang::ASTContext::getTypeAlignInChars(clang::QualType) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79bf6a7)
#<!-- -->19 0x0000000006ca9ba3 clang::Sema::CheckArgAlignment(clang::SourceLocation, clang::NamedDecl*, llvm::StringRef, clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ca9ba3)
#<!-- -->20 0x0000000006cc93aa clang::Sema::CheckConstructorCall(clang::FunctionDecl*, clang::QualType, llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::FunctionProtoType const*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc93aa)
#<!-- -->21 0x0000000006ea49e4 clang::Sema::CompleteConstructorCall(clang::CXXConstructorDecl*, clang::QualType, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ea49e4)
#<!-- -->22 0x0000000007108d76 PerformConstructorInitialization(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::InitializationSequence::Step const&amp;, bool&amp;, bool, bool, clang::SourceLocation, clang::SourceLocation) SemaInit.cpp:0:0
#<!-- -->23 0x0000000007117499 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7117499)
#<!-- -->24 0x0000000006e36e43 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#<!-- -->25 0x0000000006a8b37b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8b37b)
#<!-- -->26 0x0000000006a990aa clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a990aa)
#<!-- -->27 0x0000000006aa1dbf clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa1dbf)
#<!-- -->28 0x0000000006aa228c clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa228c)
#<!-- -->29 0x0000000006b537d4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b537d4)
#<!-- -->30 0x0000000006b54066 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b54066)
#<!-- -->31 0x0000000006b5c9ae clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5c9ae)
#<!-- -->32 0x0000000006b5d1fa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b5d1fa)
#<!-- -->33 0x0000000006a6364b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a6364b)
#<!-- -->34 0x0000000006a99abd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a99abd)
#<!-- -->35 0x0000000006a567ab clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a567ab)
#<!-- -->36 0x0000000006a56f9f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a56f9f)
#<!-- -->37 0x0000000006a5edca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5edca)
#<!-- -->38 0x0000000006a5fd65 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5fd65)
#<!-- -->39 0x0000000006a40dea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a40dea)
#<!-- -->40 0x0000000004c6b818 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6b818)
#<!-- -->41 0x0000000004f65025 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f65025)
#<!-- -->42 0x0000000004ee4d3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee4d3e)
#<!-- -->43 0x000000000505f8bd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x505f8bd)
#<!-- -->44 0x0000000000dd001e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd001e)
#<!-- -->45 0x0000000000dc69ea ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->46 0x0000000000dc6b6d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->47 0x0000000004cdea09 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->48 0x00000000042432c4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42432c4)
#<!-- -->49 0x0000000004cdf01f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->50 0x0000000004c9f9f2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c9f9f2)
#<!-- -->51 0x0000000004ca099e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca099e)
#<!-- -->52 0x0000000004ca7de5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca7de5)
#<!-- -->53 0x0000000000dcc391 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdcc391)
#<!-- -->54 0x0000000000c89af4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc89af4)
#<!-- -->55 0x00007e608ea29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->56 0x00007e608ea29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->57 0x0000000000dc6485 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc6485)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```


</details>


---

### Comment 2 - EugeneZelenko

@zwuis: Please also set `Type` to `Bug` in case of crashes.

---

### Comment 3 - shafik

Can you please focus your fuzzing efforts on recent regressions? We have so many of these fuzzing reports that anything not recent is a lot less likely to get handled. 

---

### Comment 4 - Attacker23

@shafik 
Understood — thanks for the clarification, and for spending time on triage. We’ll prioritize filing recent regressions going forward.  

Our fuzzing is entirely focused on new features/new code paths, but it often triggers unexpected issues as well (we’ve recently been testing *deducing this*; you can see it in #175713). We may also try to file some older bugs that we still consider valuable.

Also, could you clarify what you consider “recent” (release range vs. time window)?

---

### Comment 5 - shafik

> Also, could you clarify what you consider “recent” (release range vs. time window)?

Last five releases is probably a good definition of recent releases wrt to regressions. Going back any further and it is a lot less likely the author is still actively involved. 


---

### Comment 6 - Attacker23


> Last five releases is probably a good definition of recent releases wrt to regressions. Going back any further and it is a lot less likely the author is still actively involved. 
> 

@shafik Got it, thanks for clarifying🥰🥰🥰

---

