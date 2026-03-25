# Clang++ crash on nested template specialization instantiation

**Author:** ClubieDong
**URL:** https://github.com/llvm/llvm-project/issues/135801

## Body

This following piece of code makes clang++ crash:

```cpp
template <int Index>
struct Node {
    template <typename = void>
    struct State;
};

template <>
template <typename T>
struct Node<0>::State {};

int main() { Node<0>::State a; }
```

* **Clang++ version**: any version from 10.0.0 to the latest
* **Compilation options**: `-std=c++20`

Compiler explorer link: https://godbolt.org/z/4GaEjnref

I'm not sure if this is valid C++ code:
gcc compiles it successfully; msvc reports an error; clang crashes.

<details>
<summary>Clang++ output</summary>

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:11:30: current parser token ';'
2.	<source>:11:12: parsing function body 'main'
3.	<source>:11:12: in compound statement ('{}')
 #0 0x0000000003bd6b78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bd6b78)
 #1 0x0000000003bd4cbc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bd4cbc)
 #2 0x0000000003b255e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007cfb96442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006d964e0 clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d964e0)
 #5 0x0000000006d9817a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d9817a)
 #6 0x0000000006d98580 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d98580)
 #7 0x000000000683c1a3 TryConstructorInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType, clang::QualType, clang::InitializationSequence&, bool, bool) SemaInit.cpp:0:0
 #8 0x000000000684394e clang::InitializationSequence::InitializeFrom(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x684394e)
 #9 0x00000000064fe2f8 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64fe2f8)
#10 0x0000000006176bec clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6176bec)
#11 0x000000000618cbcc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618cbcc)
#12 0x000000000618f6ca clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618f6ca)
#13 0x000000000618fbe7 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618fbe7)
#14 0x000000000624a3e6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624a3e6)
#15 0x000000000624ad0e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624ad0e)
#16 0x000000000625380d clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625380d)
#17 0x000000000625424a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625424a)
#18 0x00000000061404d6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61404d6)
#19 0x000000000618d02f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618d02f)
#20 0x00000000061388c7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61388c7)
#21 0x00000000061396de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61396de)
#22 0x00000000061431ea clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61431ea)
#23 0x0000000006144a98 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6144a98)
#24 0x0000000006132eda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6132eda)
#25 0x000000000450f27d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450f27d)
#26 0x000000000481d2ea clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x481d2ea)
#27 0x000000000479986b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x479986b)
#28 0x000000000490d663 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x490d663)
#29 0x0000000000db27b1 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb27b1)
#30 0x0000000000daaf0d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#31 0x0000000004580919 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#32 0x0000000003b25a03 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b25a03)
#33 0x0000000004580b39 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#34 0x00000000045440ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45440ed)
#35 0x00000000045450b1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45450b1)
#36 0x000000000454f3fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x454f3fc)
#37 0x0000000000daf4e1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdaf4e1)
#38 0x0000000000c33cf4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc33cf4)
#39 0x00007cfb96429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#40 0x00007cfb96429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#41 0x0000000000daa9a5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdaa9a5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Shichen Dong (ClubieDong)

<details>
This following piece of code makes clang++ crash:

```cpp
template &lt;int Index&gt;
struct Node {
    template &lt;typename = void&gt;
    struct State;
};

template &lt;&gt;
template &lt;typename T&gt;
struct Node&lt;0&gt;::State {};

int main() { Node&lt;0&gt;::State a; }
```

* **Clang++ version**: any version from 10.0.0 to the latest
* **Compilation options**: `-std=c++20`

Compiler explorer link: https://godbolt.org/z/4GaEjnref

I'm not sure if this is valid C++ code:
gcc compiles it successfully; msvc reports an error; clang crashes.

&lt;details&gt;
&lt;summary&gt;Clang++ output&lt;/summary&gt;

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:11:30: current parser token ';'
2.	&lt;source&gt;:11:12: parsing function body 'main'
3.	&lt;source&gt;:11:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003bd6b78 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bd6b78)
 #<!-- -->1 0x0000000003bd4cbc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bd4cbc)
 #<!-- -->2 0x0000000003b255e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007cfb96442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006d964e0 clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d964e0)
 #<!-- -->5 0x0000000006d9817a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d9817a)
 #<!-- -->6 0x0000000006d98580 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d98580)
 #<!-- -->7 0x000000000683c1a3 TryConstructorInitialization(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType, clang::QualType, clang::InitializationSequence&amp;, bool, bool) SemaInit.cpp:0:0
 #<!-- -->8 0x000000000684394e clang::InitializationSequence::InitializeFrom(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x684394e)
 #<!-- -->9 0x00000000064fe2f8 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64fe2f8)
#<!-- -->10 0x0000000006176bec clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6176bec)
#<!-- -->11 0x000000000618cbcc clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618cbcc)
#<!-- -->12 0x000000000618f6ca clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618f6ca)
#<!-- -->13 0x000000000618fbe7 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618fbe7)
#<!-- -->14 0x000000000624a3e6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624a3e6)
#<!-- -->15 0x000000000624ad0e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x624ad0e)
#<!-- -->16 0x000000000625380d clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625380d)
#<!-- -->17 0x000000000625424a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625424a)
#<!-- -->18 0x00000000061404d6 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61404d6)
#<!-- -->19 0x000000000618d02f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x618d02f)
#<!-- -->20 0x00000000061388c7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61388c7)
#<!-- -->21 0x00000000061396de clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61396de)
#<!-- -->22 0x00000000061431ea clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61431ea)
#<!-- -->23 0x0000000006144a98 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6144a98)
#<!-- -->24 0x0000000006132eda clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6132eda)
#<!-- -->25 0x000000000450f27d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x450f27d)
#<!-- -->26 0x000000000481d2ea clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x481d2ea)
#<!-- -->27 0x000000000479986b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x479986b)
#<!-- -->28 0x000000000490d663 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x490d663)
#<!-- -->29 0x0000000000db27b1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb27b1)
#<!-- -->30 0x0000000000daaf0d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->31 0x0000000004580919 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->32 0x0000000003b25a03 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b25a03)
#<!-- -->33 0x0000000004580b39 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->34 0x00000000045440ed clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45440ed)
#<!-- -->35 0x00000000045450b1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45450b1)
#<!-- -->36 0x000000000454f3fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x454f3fc)
#<!-- -->37 0x0000000000daf4e1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdaf4e1)
#<!-- -->38 0x0000000000c33cf4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc33cf4)
#<!-- -->39 0x00007cfb96429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->40 0x00007cfb96429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->41 0x0000000000daa9a5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdaa9a5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

&lt;/details&gt;
</details>


---

### Comment 2 - shafik

This goes back to clang-9: https://godbolt.org/z/rbeYnePv4

---

### Comment 3 - shafik

Maybe related: https://github.com/llvm/llvm-project/issues/63959

---

