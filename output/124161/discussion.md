# Clang frontend bug report

**Author:** BlakeKessler
**URL:** https://github.com/llvm/llvm-project/issues/124161
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, regression:17
**Closed Date:** 2025-01-27T15:04:59Z

## Body

# Error Logs

[buf-f965e1.sh](https://github.com/user-attachments/files/18524598/buf-f965e1.sh.txt)
[buf-f965e1.hh](https://github.com/user-attachments/files/18524599/buf-f965e1.hh.txt)
[buf-f965e1.reduced.hh](https://github.com/user-attachments/files/18524601/buf-f965e1.reduced.hh.txt)
[stack-dump-f965e1.txt](https://github.com/user-attachments/files/18524595/stack-dump-f965e1.txt)

[buf-c54075.sh](https://github.com/user-attachments/files/18524600/buf-c54075.sh.txt)
[buf-c54075.hh](https://github.com/user-attachments/files/18524596/buf-c54075.hh.txt)
[stack-dump-c54075.txt](https://github.com/user-attachments/files/18524597/stack-dump-c54075.txt)

# Source Code

[unreachable.hpp](https://github.com/user-attachments/files/18524568/unreachable.hpp.txt)
[throw.hpp](https://github.com/user-attachments/files/18524564/throw.hpp.txt)
[MCSL_MSG_LOCALIZATION.hpp](https://github.com/user-attachments/files/18524566/MCSL_MSG_LOCALIZATION.hpp.txt)
[MCSL.hpp](https://github.com/user-attachments/files/18524563/MCSL.hpp.txt)
[contig_base.hpp](https://github.com/user-attachments/files/18524562/contig_base.hpp.txt)
[concepts.hpp](https://github.com/user-attachments/files/18524565/concepts.hpp.txt)
[buf.hpp](https://github.com/user-attachments/files/18524560/buf.hpp.txt)
[assert.hpp](https://github.com/user-attachments/files/18524567/assert.hpp.txt)
[alloc.hpp](https://github.com/user-attachments/files/18524561/alloc.hpp.txt)
[alloc.cpp](https://github.com/user-attachments/files/18524559/alloc.cpp.txt)

# PS

I acquired clang through the apt package manager.
creduce appears to have done nothing because of the gnu-style line directives in the preprocessed file that clang told me to include.

## Comments

### Comment 1 - MagentaTreehouse

On Clang trunk, the reduced code below reproduces a crash that matches the one by the original code. The difference is that it does not crash Clang 18 like the original.
```c++
struct buf {
    constexpr buf(auto&&... initList) requires (sizeof...(initList) <= 8);
};

constexpr buf::buf(auto&&... initList) requires (sizeof...(initList) <= 8) {}
```

See https://compiler-explorer.com/z/bsEG9MbG3.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6221:
clang::NamedDecl* clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, const clang::MultiLevelTemplateArgumentList&, bool):
Assertion `PackIdx != -1 && "found declaration pack but not pack expanding"' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:5:76: current parser token '{'
 #0 0x0000000003deeb18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3deeb18)
 #1 0x0000000003dec7d4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3dec7d4)
 #2 0x0000000003d38cc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078ea5a842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078ea5a8969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078ea5a842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078ea5a8287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078ea5a82871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000078ea5a839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000073cc1bc clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x73cc1bc)
#10 0x00000000072e7860 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000007324891 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000007337748 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007323ef2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000007332a98 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformParenExpr(clang::ParenExpr*) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000073244f9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000734ce8f clang::Sema::SubstConstraintExprWithoutSatisfaction(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x734ce8f)
#17 0x0000000006a8be6b SubstituteConstraintExpressionWithoutSatisfaction(clang::Sema&, clang::Sema::TemplateCompareNewDeclInfo const&, clang::Expr const*) SemaConcept.cpp:0:0
#18 0x0000000006a956aa clang::Sema::AreConstraintExpressionsEqual(clang::NamedDecl const*, clang::Expr const*, clang::Sema::TemplateCompareNewDeclInfo const&, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a956aa)
#19 0x000000000708c171 IsOverloadOrOverrideImpl(clang::Sema&, clang::FunctionDecl*, clang::FunctionDecl*, bool, bool, bool) SemaOverload.cpp:0:0
#20 0x000000000708c691 clang::Sema::CheckOverload(clang::Scope*, clang::FunctionDecl*, clang::LookupResult const&, clang::NamedDecl*&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x708c691)
#21 0x0000000006b7559e clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b7559e)
#22 0x0000000006b7b116 clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b7b116)
#23 0x0000000006b801d0 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b801d0)
#24 0x0000000006b80b80 clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b80b80)
#25 0x00000000067d2a60 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67d2a60)
#26 0x0000000006807f4d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6807f4d)
#27 0x00000000067c706e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67c706e)
#28 0x00000000067c7829 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67c7829)
#29 0x00000000067cefd3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67cefd3)
#30 0x00000000067cfead clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67cfead)
#31 0x00000000067c230a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67c230a)
#32 0x000000000477f1d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x477f1d8)
#33 0x0000000004a44595 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a44595)
#34 0x00000000049c72ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c72ce)
#35 0x0000000004b322be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b322be)
#36 0x0000000000cfcd4f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcfcd4f)
#37 0x0000000000cf47ea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#38 0x00000000047c5b09 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#39 0x0000000003d39174 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d39174)
#40 0x00000000047c60ff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#41 0x0000000004788f6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4788f6d)
#42 0x0000000004789fee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4789fee)
#43 0x0000000004791b25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4791b25)
#44 0x0000000000cf9b63 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf9b63)
#45 0x0000000000bc2ac4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbc2ac4)
#46 0x000078ea5a829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#47 0x000078ea5a829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#48 0x0000000000cf4295 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcf4295)
```

</details>

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Blake (BlakeKessler)

<details>
# Error Logs

[buf-f965e1.sh](https://github.com/user-attachments/files/18524598/buf-f965e1.sh.txt)
[buf-f965e1.hh](https://github.com/user-attachments/files/18524599/buf-f965e1.hh.txt)
[buf-f965e1.reduced.hh](https://github.com/user-attachments/files/18524601/buf-f965e1.reduced.hh.txt)
[stack-dump-f965e1.txt](https://github.com/user-attachments/files/18524595/stack-dump-f965e1.txt)

[buf-c54075.sh](https://github.com/user-attachments/files/18524600/buf-c54075.sh.txt)
[buf-c54075.hh](https://github.com/user-attachments/files/18524596/buf-c54075.hh.txt)
[stack-dump-c54075.txt](https://github.com/user-attachments/files/18524597/stack-dump-c54075.txt)

# Source Code

[unreachable.hpp](https://github.com/user-attachments/files/18524568/unreachable.hpp.txt)
[throw.hpp](https://github.com/user-attachments/files/18524564/throw.hpp.txt)
[MCSL_MSG_LOCALIZATION.hpp](https://github.com/user-attachments/files/18524566/MCSL_MSG_LOCALIZATION.hpp.txt)
[MCSL.hpp](https://github.com/user-attachments/files/18524563/MCSL.hpp.txt)
[contig_base.hpp](https://github.com/user-attachments/files/18524562/contig_base.hpp.txt)
[concepts.hpp](https://github.com/user-attachments/files/18524565/concepts.hpp.txt)
[buf.hpp](https://github.com/user-attachments/files/18524560/buf.hpp.txt)
[assert.hpp](https://github.com/user-attachments/files/18524567/assert.hpp.txt)
[alloc.hpp](https://github.com/user-attachments/files/18524561/alloc.hpp.txt)
[alloc.cpp](https://github.com/user-attachments/files/18524559/alloc.cpp.txt)

# PS

I acquired clang through the apt package manager.
creduce appears to have done nothing because of the gnu-style line directives in the preprocessed file that clang told me to include.
</details>


---

### Comment 3 - shafik

@MagentaTreehouse your reproducer only crashes on trunk, so I think they must be different or perhaps different paths to the same issue but we need a reproducer that crashes on 18.

---

### Comment 4 - MagentaTreehouse

Understood. Here is the repro that also crashes on 18:
```c++
template <class> struct buf {
    constexpr buf(auto&&... initList) requires (sizeof...(initList) <= 8);
};

template <class T>
constexpr buf<T>::buf(auto&&... initList) requires (sizeof...(initList) <= 8) {}
```

See https://compiler-explorer.com/z/jbr478r4z.

No assertions hit.

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-18.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-13.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:6:79: current parser token '{'
 #0 0x00000000038d53d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x38d53d8)
 #1 0x00000000038d30bc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x38d30bc)
 #2 0x000000000381baa8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007dce10e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006c21be0 clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x6c21be0)
 #5 0x0000000006cca939 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, clang::MultiLevelTemplateArgumentList const&, bool&, bool&, std::optional<unsigned int>&) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x6cca939)
 #6 0x0000000006c43555 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformSizeOfPackExpr(clang::SizeOfPackExpr*) SemaTemplateInstantiate.cpp:0:0
 #7 0x0000000006c29777 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #8 0x0000000006c36928 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
 #9 0x0000000006c290f9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000006c31548 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformParenExpr(clang::ParenExpr*) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000006c296f7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006c5d368 clang::Sema::SubstConstraintExprWithoutSatisfaction(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x6c5d368)
#13 0x000000000639d07c SubstituteConstraintExpressionWithoutSatisfaction(clang::Sema&, clang::Sema::TemplateCompareNewDeclInfo const&, clang::Expr const*) SemaConcept.cpp:0:0
#14 0x00000000063a5402 clang::Sema::AreConstraintExpressionsEqual(clang::NamedDecl const*, clang::Expr const*, clang::Sema::TemplateCompareNewDeclInfo const&, clang::Expr const*) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x63a5402)
#15 0x00000000069ead7e IsOverloadOrOverrideImpl(clang::Sema&, clang::FunctionDecl*, clang::FunctionDecl*, bool, bool, bool) SemaOverload.cpp:0:0
#16 0x00000000069eb136 clang::Sema::CheckOverload(clang::Scope*, clang::FunctionDecl*, clang::LookupResult const&, clang::NamedDecl*&, bool) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x69eb136)
#17 0x00000000064748d3 clang::Sema::CheckFunctionDeclaration(clang::Scope*, clang::FunctionDecl*, clang::LookupResult&, bool, bool) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x64748d3)
#18 0x000000000647b62a clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x647b62a)
#19 0x000000000647f513 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x647f513)
#20 0x00000000064800af clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Sema::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x64800af)
#21 0x00000000061000be clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x61000be)
#22 0x00000000061fb220 clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x61fb220)
#23 0x00000000062006e8 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#24 0x00000000062009a3 clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x62009a3)
#25 0x000000000613dd56 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x613dd56)
#26 0x00000000060fc53e clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x60fc53e)
#27 0x00000000060fd3bd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x60fd3bd)
#28 0x00000000060f02ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x60f02ea)
#29 0x000000000415d928 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x415d928)
#30 0x00000000043cbfe9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x43cbfe9)
#31 0x000000000434ba9e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x434ba9e)
#32 0x00000000044ab01e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x44ab01e)
#33 0x0000000000c10cd6 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0xc10cd6)
#34 0x0000000000c084fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#35 0x000000000419ee49 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x000000000381bf54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x381bf54)
#37 0x000000000419f43f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x00000000041671c5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x41671c5)
#39 0x0000000004167c2d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x4167c2d)
#40 0x000000000416fb65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0x416fb65)
#41 0x0000000000c0e06c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0xc0e06c)
#42 0x0000000000b05134 main (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0xb05134)
#43 0x00007dce10e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x00007dce10e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000c07fde _start (/opt/compiler-explorer/clang-assertions-18.1.0/bin/clang+++0xc07fde)
```

</details>

---

### Comment 5 - shafik

Looks like a clang-17 regression: https://compiler-explorer.com/z/e6eaojP9T

---

### Comment 6 - shafik

git bisect says this regression was caused by 6db007a0654ed7a6ed5c3aa3b61a937c19a6bc6b 

CC @zyn0217 

---

