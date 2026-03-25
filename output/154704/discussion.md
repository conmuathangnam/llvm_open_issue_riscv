# [clang][OpenMP] Assertion `Loc.isValid() && "point of instantiation must be valid!"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/154704

## Body

Reproducer:
https://godbolt.org/z/T3c3dTqnn
```cpp
#include <map>

void foo() {
  int keys[42], data[42];
  std::map<int, int> _map;

#pragma omp target
  {
    for (int i = 0; i < 42; ++i)
      _map[keys[i]] = data[i];
  }
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/DeclTemplate.h:1977: void clang::ClassTemplateSpecializationDecl::setPointOfInstantiation(clang::SourceLocation): Assertion `Loc.isValid() && "point of instantiation must be valid!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp <source>
1.	<source>:12:1: current parser token '}'
2.	<source>:3:12: parsing function body 'foo'
3.	<source>:3:12: in compound statement ('{}')
 #0 0x0000000004039748 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4039748)
 #1 0x0000000004036b74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4036b74)
 #2 0x0000000003f7b288 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000706f7d242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000706f7d2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000706f7d242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000706f7d2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000706f7d22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000706f7d239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000726f064 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x726f064)
#10 0x00000000072727c8 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72727c8)
#11 0x00000000072f265e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#12 0x0000000007fd96c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7fd96c1)
#13 0x00000000072fde33 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72fde33)
#14 0x0000000006dd8781 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::CXXRecordDecl*) SemaLookup.cpp:0:0
#15 0x0000000006dda3fe addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
#16 0x0000000006dd84cc addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
#17 0x0000000006dd870d addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::CXXRecordDecl*) SemaLookup.cpp:0:0
#18 0x0000000006dda3fe addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
#19 0x0000000006dd84cc addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::TemplateArgument const&) SemaLookup.cpp:0:0
#20 0x0000000006dd870d addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::CXXRecordDecl*) SemaLookup.cpp:0:0
#21 0x0000000006dda3fe addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&, clang::QualType) SemaLookup.cpp:0:0
#22 0x0000000006ddace8 clang::Sema::FindAssociatedClassesAndNamespaces(clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, llvm::SmallSetVector<clang::DeclContext*, 16u>&, llvm::SmallSetVector<clang::CXXRecordDecl*, 16u>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ddace8)
#23 0x0000000006e863e7 argumentDependentLookup(clang::Sema&, clang::DeclarationNameInfo const&, clang::SourceLocation, clang::QualType, llvm::SmallVectorImpl<clang::UnresolvedSet<8u>>&) SemaOpenMP.cpp:0:0
#24 0x0000000006ea01a9 buildUserDefinedMapperRef(clang::Sema&, clang::Scope*, clang::CXXScopeSpec&, clang::DeclarationNameInfo const&, clang::QualType, clang::Expr*) SemaOpenMP.cpp:0:0
#25 0x0000000006ef8436 checkMappableExpressionList(clang::Sema&, (anonymous namespace)::DSAStackTy*, llvm::omp::Clause, (anonymous namespace)::MappableVarListInfo&, clang::SourceLocation, clang::CXXScopeSpec&, clang::DeclarationNameInfo, llvm::ArrayRef<clang::Expr*>, clang::OpenMPMapClauseKind, llvm::ArrayRef<clang::OpenMPMapModifierKind>, bool, bool) SemaOpenMP.cpp:0:0
#26 0x0000000006efa2a4 clang::SemaOpenMP::ActOnOpenMPMapClause(clang::Expr*, llvm::ArrayRef<clang::OpenMPMapModifierKind>, llvm::ArrayRef<clang::SourceLocation>, clang::CXXScopeSpec&, clang::DeclarationNameInfo&, clang::OpenMPMapClauseKind, bool, clang::SourceLocation, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::OMPVarListLocTy const&, bool, llvm::ArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6efa2a4)
#27 0x0000000006f0a344 clang::SemaOpenMP::ActOnOpenMPExecutableDirective(llvm::omp::Directive, clang::DeclarationNameInfo const&, llvm::omp::Directive, llvm::ArrayRef<clang::OMPClause*>, clang::Stmt*, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f0a344)
#28 0x00000000067bff8c clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67bff8c)
#29 0x00000000067cfb74 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67cfb74)
#30 0x00000000067ee614 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67ee614)
#31 0x00000000067f0289 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f0289)
#32 0x00000000067f83db clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f83db)
#33 0x00000000067f8bea clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f8bea)
#34 0x00000000066ef0fb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ef0fb)
#35 0x000000000672555d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x672555d)
#36 0x00000000066e1ece clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1ece)
#37 0x00000000066e266f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e266f)
#38 0x00000000066ea4aa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea4aa)
#39 0x00000000066eb445 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66eb445)
#40 0x00000000066dcfca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dcfca)
#41 0x00000000049c71a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c71a8)
#42 0x0000000004cbbd85 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cbbd85)
#43 0x0000000004c37f2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c37f2e)
#44 0x0000000004daddb1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4daddb1)
#45 0x0000000000db34ff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb34ff)
#46 0x0000000000daa1aa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#47 0x0000000004a2f189 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#48 0x0000000003f7b724 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7b724)
#49 0x0000000004a2f79f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#50 0x00000000049f145d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f145d)
#51 0x00000000049f24ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f24ee)
#52 0x00000000049fa125 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fa125)
#53 0x0000000000daf9b5 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf9b5)
#54 0x0000000000c637e4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc637e4)
#55 0x0000706f7d229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#56 0x0000706f7d229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#57 0x0000000000da9c55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda9c55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - k-arrows

Goes back to clang-9.0.0:
https://godbolt.org/z/5znMEznPo

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/T3c3dTqnn
```cpp
#include &lt;map&gt;

void foo() {
  int keys[42], data[42];
  std::map&lt;int, int&gt; _map;

#pragma omp target
  {
    for (int i = 0; i &lt; 42; ++i)
      _map[keys[i]] = data[i];
  }
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/DeclTemplate.h:1977: void clang::ClassTemplateSpecializationDecl::setPointOfInstantiation(clang::SourceLocation): Assertion `Loc.isValid() &amp;&amp; "point of instantiation must be valid!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fopenmp &lt;source&gt;
1.	&lt;source&gt;:12:1: current parser token '}'
2.	&lt;source&gt;:3:12: parsing function body 'foo'
3.	&lt;source&gt;:3:12: in compound statement ('{}')
 #<!-- -->0 0x0000000004039748 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4039748)
 #<!-- -->1 0x0000000004036b74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4036b74)
 #<!-- -->2 0x0000000003f7b288 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000706f7d242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000706f7d2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000706f7d242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000706f7d2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000706f7d22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000706f7d239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000726f064 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x726f064)
#<!-- -->10 0x00000000072727c8 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72727c8)
#<!-- -->11 0x00000000072f265e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->12 0x0000000007fd96c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7fd96c1)
#<!-- -->13 0x00000000072fde33 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72fde33)
#<!-- -->14 0x0000000006dd8781 addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::CXXRecordDecl*) SemaLookup.cpp:0:0
#<!-- -->15 0x0000000006dda3fe addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
#<!-- -->16 0x0000000006dd84cc addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
#<!-- -->17 0x0000000006dd870d addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::CXXRecordDecl*) SemaLookup.cpp:0:0
#<!-- -->18 0x0000000006dda3fe addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
#<!-- -->19 0x0000000006dd84cc addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::TemplateArgument const&amp;) SemaLookup.cpp:0:0
#<!-- -->20 0x0000000006dd870d addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::CXXRecordDecl*) SemaLookup.cpp:0:0
#<!-- -->21 0x0000000006dda3fe addAssociatedClassesAndNamespaces((anonymous namespace)::AssociatedLookup&amp;, clang::QualType) SemaLookup.cpp:0:0
#<!-- -->22 0x0000000006ddace8 clang::Sema::FindAssociatedClassesAndNamespaces(clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::SmallSetVector&lt;clang::DeclContext*, 16u&gt;&amp;, llvm::SmallSetVector&lt;clang::CXXRecordDecl*, 16u&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ddace8)
#<!-- -->23 0x0000000006e863e7 argumentDependentLookup(clang::Sema&amp;, clang::DeclarationNameInfo const&amp;, clang::SourceLocation, clang::QualType, llvm::SmallVectorImpl&lt;clang::UnresolvedSet&lt;8u&gt;&gt;&amp;) SemaOpenMP.cpp:0:0
#<!-- -->24 0x0000000006ea01a9 buildUserDefinedMapperRef(clang::Sema&amp;, clang::Scope*, clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo const&amp;, clang::QualType, clang::Expr*) SemaOpenMP.cpp:0:0
#<!-- -->25 0x0000000006ef8436 checkMappableExpressionList(clang::Sema&amp;, (anonymous namespace)::DSAStackTy*, llvm::omp::Clause, (anonymous namespace)::MappableVarListInfo&amp;, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OpenMPMapClauseKind, llvm::ArrayRef&lt;clang::OpenMPMapModifierKind&gt;, bool, bool) SemaOpenMP.cpp:0:0
#<!-- -->26 0x0000000006efa2a4 clang::SemaOpenMP::ActOnOpenMPMapClause(clang::Expr*, llvm::ArrayRef&lt;clang::OpenMPMapModifierKind&gt;, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo&amp;, clang::OpenMPMapClauseKind, bool, clang::SourceLocation, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OMPVarListLocTy const&amp;, bool, llvm::ArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6efa2a4)
#<!-- -->27 0x0000000006f0a344 clang::SemaOpenMP::ActOnOpenMPExecutableDirective(llvm::omp::Directive, clang::DeclarationNameInfo const&amp;, llvm::omp::Directive, llvm::ArrayRef&lt;clang::OMPClause*&gt;, clang::Stmt*, clang::SourceLocation, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f0a344)
#<!-- -->28 0x00000000067bff8c clang::Parser::ParseOpenMPExecutableDirective(clang::Parser::ParsedStmtContext, llvm::omp::Directive, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67bff8c)
#<!-- -->29 0x00000000067cfb74 clang::Parser::ParseOpenMPDeclarativeOrExecutableDirective(clang::Parser::ParsedStmtContext, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67cfb74)
#<!-- -->30 0x00000000067ee614 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67ee614)
#<!-- -->31 0x00000000067f0289 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f0289)
#<!-- -->32 0x00000000067f83db clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f83db)
#<!-- -->33 0x00000000067f8bea clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67f8bea)
#<!-- -->34 0x00000000066ef0fb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ef0fb)
#<!-- -->35 0x000000000672555d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x672555d)
#<!-- -->36 0x00000000066e1ece clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e1ece)
#<!-- -->37 0x00000000066e266f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e266f)
#<!-- -->38 0x00000000066ea4aa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ea4aa)
#<!-- -->39 0x00000000066eb445 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66eb445)
#<!-- -->40 0x00000000066dcfca clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dcfca)
#<!-- -->41 0x00000000049c71a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49c71a8)
#<!-- -->42 0x0000000004cbbd85 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cbbd85)
#<!-- -->43 0x0000000004c37f2e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c37f2e)
#<!-- -->44 0x0000000004daddb1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4daddb1)
#<!-- -->45 0x0000000000db34ff cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb34ff)
#<!-- -->46 0x0000000000daa1aa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->47 0x0000000004a2f189 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->48 0x0000000003f7b724 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7b724)
#<!-- -->49 0x0000000004a2f79f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->50 0x00000000049f145d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f145d)
#<!-- -->51 0x00000000049f24ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49f24ee)
#<!-- -->52 0x00000000049fa125 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49fa125)
#<!-- -->53 0x0000000000daf9b5 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf9b5)
#<!-- -->54 0x0000000000c637e4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc637e4)
#<!-- -->55 0x0000706f7d229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->56 0x0000706f7d229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->57 0x0000000000da9c55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda9c55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 3 - cabbaken

We can simplify the case as:
```c++
#include <memory>

template <typename T> class a {
    T t;
};

std::allocator<std::pair<const int, int>> t;

void foo() {
    // sizeof(std::pair<const int, int>);

#pragma omp target
    {
        t;
    }
}
```
The key is std::allocator with a nested template argument (In your code, this is `std::map<..., std::allocator<std::pair<const int, int>>>`).
This may caused by lazy instantiation. If we uncomment `sizeof(std::pair<const int, int>);`, it won't crash any more.
Additionally, I create a similar code to reproduce this:
```c++
template <typename T> class a {
    T v;
};

template <typename T> class b {};

b<a<int>> t;
void foo() {
  // sizeof(a<int>);
#pragma omp target
    {
        t;
    }
}
```


---

### Comment 4 - cabbaken

More specifically:
The `Loc` is derived from `MapperId`, which is set within `checkMappableExpressionList()`. Specifically, `ActOnOpenMPMapClause()` invokes `checkMappableExpressionList()` using `OMPVarListLocTy()`, resulting in an empty `Loc`.

However, I suspect this is not the root cause, as `Loc` appears to be intentionally empty in this context. The critical question is why `sizeof(a<int>)` functions correctly under similar conditions.

We know that Argument Dependent Lookup (ADL) occurs during variable mapping. If `a<int>` has not yet been instantiated, lazy instantiation will attempt to instantiate it using an empty `Loc`.

During OpenMP mapping, `ActOnOpenMPExecutableDirective` invokes ADL with this empty `Loc` while mapping the variable `t` within an implicit clause, which eventually triggers the assertion.

---

