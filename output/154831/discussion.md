# [crash][clang] Crash when instantiating out-of-class defined class template.

**Author:** cdacamar
**URL:** https://github.com/llvm/llvm-project/issues/154831
**Status:** Closed
**Labels:** duplicate, clang:frontend, confirmed, crash
**Closed Date:** 2025-08-22T00:26:17Z

## Body

Minimal repro:
```c++
template <typename>
struct X1 {
  template <typename>
  struct X3;
};

template <>
template <typename T>
struct X1<int>::X3 {
  void x3() { }
};

int main() {
  X1<int>::X3<int> x3;
  x3.x3();
}
```

Godbolt: https://godbolt.org/z/Y7roheqcz

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaType.cpp:9320:
bool clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool):
Assertion `D && "missing definition for pattern of instantiated definition"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++2a <source> -isystem/opt/compiler-explorer/libs/fmt/trunk/include
1.	<source>:14:22: current parser token ';'
2.	<source>:13:12: parsing function body 'main'
3.	<source>:13:12: in compound statement ('{}')
 #0 0x0000000004039748 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4039748)
 #1 0x0000000004036b74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4036b74)
 #2 0x0000000003f7b288 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007d3ea8e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007d3ea8e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007d3ea8e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007d3ea8e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007d3ea8e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007d3ea8e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000072f97e4 clang::Sema::hasAcceptableDefinition(clang::NamedDecl*, clang::NamedDecl**, clang::Sema::AcceptableKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f97e4)
#10 0x00000000072fdab2 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72fdab2)
#11 0x00000000072fdf67 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72fdf67)
#12 0x00000000072fe2cf clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72fe2cf)
#13 0x00000000072fe8b5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72fe8b5)
#14 0x0000000006aabeba clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#15 0x000000000671575b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671575b)
#16 0x0000000006724b59 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6724b59)
#17 0x000000000672d81f clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x672d81f)
#18 0x000000000672dcec clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x672dcec)
#19 0x00000000067ef5b2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67ef5b2)
#20 0x00000000067f0289 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f0289)
#21 0x00000000067f83db clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f83db)
#22 0x00000000067f8bea clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f8bea)
#23 0x00000000066ef0fb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ef0fb)
#24 0x000000000672555d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x672555d)
#25 0x00000000066e1ece clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1ece)
#26 0x00000000066e266f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e266f)
#27 0x00000000066ea4aa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea4aa)
#28 0x00000000066eb445 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66eb445)
#29 0x00000000066dcfca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dcfca)
#30 0x00000000049c71a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c71a8)
#31 0x0000000004cbbd85 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cbbd85)
#32 0x0000000004c37f2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c37f2e)
#33 0x0000000004daddb1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4daddb1)
#34 0x0000000000db34ff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb34ff)
#35 0x0000000000daa1aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x0000000004a2f189 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000003f7b724 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7b724)
#38 0x0000000004a2f79f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x00000000049f145d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f145d)
#40 0x00000000049f24ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f24ee)
#41 0x00000000049fa125 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fa125)
#42 0x0000000000daf9b5 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf9b5)
#43 0x0000000000c637e4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc637e4)
#44 0x00007d3ea8e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x00007d3ea8e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000da9c55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda9c55)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Cameron DaCamara (cdacamar)

<details>
Minimal repro:
```c++
template &lt;typename&gt;
struct X1 {
  template &lt;typename&gt;
  struct X3;
};

template &lt;&gt;
template &lt;typename T&gt;
struct X1&lt;int&gt;::X3 {
  void x3() { }
};

int main() {
  X1&lt;int&gt;::X3&lt;int&gt; x3;
  x3.x3();
}
```

Godbolt: https://godbolt.org/z/Y7roheqcz
</details>


---

### Comment 2 - shafik

Goes back to clang-9: https://godbolt.org/z/9cvdceE3e :-(

---

### Comment 3 - shafik

I am going to call this a dup of: https://github.com/llvm/llvm-project/issues/63959 b/c the last example has the same backtrace as this one.

---

### Comment 4 - shafik

Reduced: https://godbolt.org/z/hnPqdbo8G

```cpp
template <typename>
struct X1 {
  template <typename>
  struct X3;
};

template <>
template <typename>
struct X1<int>::X3 {} ;

X1<int>::X3<int> x3;
```

---

