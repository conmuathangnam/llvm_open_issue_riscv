# Crash for using deduction guide in template class with parameter pack

**Author:** nosid
**URL:** https://github.com/llvm/llvm-project/issues/186760

## Body

Compiling the following C++ source code with Clang 22 causes a compiler crash:

```c++
template <typename... Args>
struct outer {
    template <typename Type>
    struct inner {
        inner(Args...) { }
    };

    inner(Args...) -> inner<int>; // deduction guide
};

int main() {
    outer();
}
```

The compiler outputs:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++-22 crash.cpp -S
1.	crash.cpp:12:12: current parser token ';'
2.	crash.cpp:11:12: parsing function body 'main'
3.	crash.cpp:11:12: in compound statement ('{}')
4.	crash.cpp:2:8: instantiating class definition 'outer<>'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.22.1      0x00007fbb9d29f3ba llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 58
1  libLLVM.so.22.1      0x00007fbb9d29cbc7 llvm::sys::RunSignalHandlers() + 135
2  libLLVM.so.22.1      0x00007fbb9d1cdad4
3  libc.so.6            0x00007fbb97e4aa70
4  libclang-cpp.so.22.1 0x00007fbba3d9e0d8
5  libclang-cpp.so.22.1 0x00007fbba3d45ca1 clang::Sema::SubstType(clang::TypeLoc, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName) + 209
6  libclang-cpp.so.22.1 0x00007fbba3d3ca61
7  libclang-cpp.so.22.1 0x00007fbba3d0bd2b
8  libclang-cpp.so.22.1 0x00007fbba3d0ab94 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) + 1204
9  libclang-cpp.so.22.1 0x00007fbba3a8901d
10 libclang-cpp.so.22.1 0x00007fbba3a86119 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) + 169
11 libclang-cpp.so.22.1 0x00007fbba3dc4863 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) + 4579
12 libclang-cpp.so.22.1 0x00007fbba3dca178 clang::TemplateDeclInstantiator::VisitCXXDeductionGuideDecl(clang::CXXDeductionGuideDecl*) + 24
13 libclang-cpp.so.22.1 0x00007fbba3d4a151 clang::Sema::InstantiateClassImpl(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) + 1441
14 libclang-cpp.so.22.1 0x00007fbba3d4c24d clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) + 2877
15 libclang-cpp.so.22.1 0x00007fbba3e27072
16 libclang-cpp.so.22.1 0x00007fbba2ad0601 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) + 65
17 libclang-cpp.so.22.1 0x00007fbba3e1675d clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) + 1901
18 libclang-cpp.so.22.1 0x00007fbba3e15f33 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) + 19
19 libclang-cpp.so.22.1 0x00007fbba39faf01 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) + 1617
20 libclang-cpp.so.22.1 0x00007fbba39fa84a clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) + 138
21 libclang-cpp.so.22.1 0x00007fbba2c8edbf clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) + 1055
22 libclang-cpp.so.22.1 0x00007fbba2c7c5f5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) + 4069
23 libclang-cpp.so.22.1 0x00007fbba2c7820d clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) + 285
24 libclang-cpp.so.22.1 0x00007fbba2c780dd clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) + 13
25 libclang-cpp.so.22.1 0x00007fbba2cdac68 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) + 72
26 libclang-cpp.so.22.1 0x00007fbba2cd91e0 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) + 1376
27 libclang-cpp.so.22.1 0x00007fbba2cd899a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) + 394
28 libclang-cpp.so.22.1 0x00007fbba2ce11f6 clang::Parser::ParseCompoundStatementBody(bool) + 2246
29 libclang-cpp.so.22.1 0x00007fbba2ce1e89 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) + 185
30 libclang-cpp.so.22.1 0x00007fbba2cf9b78 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) + 2776
31 libclang-cpp.so.22.1 0x00007fbba2c4a1e0 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 5648
32 libclang-cpp.so.22.1 0x00007fbba2cf8e6e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) + 942
33 libclang-cpp.so.22.1 0x00007fbba2cf88dc clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 588
34 libclang-cpp.so.22.1 0x00007fbba2cf7bdf clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) + 2079
35 libclang-cpp.so.22.1 0x00007fbba2cf63b7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) + 1271
36 libclang-cpp.so.22.1 0x00007fbba2c3513e clang::ParseAST(clang::Sema&, bool, bool) + 798
37 libclang-cpp.so.22.1 0x00007fbba496ca1f clang::FrontendAction::Execute() + 47
38 libclang-cpp.so.22.1 0x00007fbba48dfe74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) + 948
39 libclang-cpp.so.22.1 0x00007fbba49fabda clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 746
40 clang++-22           0x000055cf6ab68cdd cc1_main(llvm::ArrayRef<char const*>, char const*, void*) + 7245
41 clang++-22           0x000055cf6ab6553b
42 clang++-22           0x000055cf6ab66fcc
43 libclang-cpp.so.22.1 0x00007fbba45952ad
44 libLLVM.so.22.1      0x00007fbb9d1cd7a0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) + 144
45 libclang-cpp.so.22.1 0x00007fbba4594d61 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const + 369
46 libclang-cpp.so.22.1 0x00007fbba4553f12 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const + 498
47 libclang-cpp.so.22.1 0x00007fbba45540de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const + 142
48 libclang-cpp.so.22.1 0x00007fbba45759af clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) + 367
49 clang++-22           0x000055cf6ab6502d clang_main(int, char**, llvm::ToolContext const&) + 6557
50 clang++-22           0x000055cf6ab7399a main + 106
51 libc.so.6            0x00007fbb97e33f75
52 libc.so.6            0x00007fbb97e34027 __libc_start_main + 135
53 clang++-22           0x000055cf6ab631d1 _start + 33
clang++-22: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 22.1.0 (2)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-22/bin
clang++-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-22: note: diagnostic msg: /run/user/1000/tmp/crash-9d4581.cpp
clang++-22: note: diagnostic msg: /run/user/1000/tmp/crash-9d4581.sh
clang++-22: note: diagnostic msg: 

********************
```

[crash-9d4581.cpp](https://github.com/user-attachments/files/26019490/crash-9d4581.cpp)
[crash-9d4581.sh](https://github.com/user-attachments/files/26019489/crash-9d4581.sh)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Hubert Schmid (nosid)

<details>
Compiling the following C++ source code with Clang 22 causes a compiler crash:

```c++
template &lt;typename... Args&gt;
struct outer {
    template &lt;typename Type&gt;
    struct inner {
        inner(Args...) { }
    };

    inner(Args...) -&gt; inner&lt;int&gt;; // deduction guide
};

int main() {
    outer();
}
```

The compiler outputs:

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang++-22 crash.cpp -S
1.	crash.cpp:12:12: current parser token ';'
2.	crash.cpp:11:12: parsing function body 'main'
3.	crash.cpp:11:12: in compound statement ('{}')
4.	crash.cpp:2:8: instantiating class definition 'outer&lt;&gt;'
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  libLLVM.so.22.1      0x00007fbb9d29f3ba llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) + 58
1  libLLVM.so.22.1      0x00007fbb9d29cbc7 llvm::sys::RunSignalHandlers() + 135
2  libLLVM.so.22.1      0x00007fbb9d1cdad4
3  libc.so.6            0x00007fbb97e4aa70
4  libclang-cpp.so.22.1 0x00007fbba3d9e0d8
5  libclang-cpp.so.22.1 0x00007fbba3d45ca1 clang::Sema::SubstType(clang::TypeLoc, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName) + 209
6  libclang-cpp.so.22.1 0x00007fbba3d3ca61
7  libclang-cpp.so.22.1 0x00007fbba3d0bd2b
8  libclang-cpp.so.22.1 0x00007fbba3d0ab94 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) + 1204
9  libclang-cpp.so.22.1 0x00007fbba3a8901d
10 libclang-cpp.so.22.1 0x00007fbba3a86119 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) + 169
11 libclang-cpp.so.22.1 0x00007fbba3dc4863 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) + 4579
12 libclang-cpp.so.22.1 0x00007fbba3dca178 clang::TemplateDeclInstantiator::VisitCXXDeductionGuideDecl(clang::CXXDeductionGuideDecl*) + 24
13 libclang-cpp.so.22.1 0x00007fbba3d4a151 clang::Sema::InstantiateClassImpl(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) + 1441
14 libclang-cpp.so.22.1 0x00007fbba3d4c24d clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) + 2877
15 libclang-cpp.so.22.1 0x00007fbba3e27072
16 libclang-cpp.so.22.1 0x00007fbba2ad0601 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) + 65
17 libclang-cpp.so.22.1 0x00007fbba3e1675d clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) + 1901
18 libclang-cpp.so.22.1 0x00007fbba3e15f33 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) + 19
19 libclang-cpp.so.22.1 0x00007fbba39faf01 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) + 1617
20 libclang-cpp.so.22.1 0x00007fbba39fa84a clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) + 138
21 libclang-cpp.so.22.1 0x00007fbba2c8edbf clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) + 1055
22 libclang-cpp.so.22.1 0x00007fbba2c7c5f5 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) + 4069
23 libclang-cpp.so.22.1 0x00007fbba2c7820d clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) + 285
24 libclang-cpp.so.22.1 0x00007fbba2c780dd clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) + 13
25 libclang-cpp.so.22.1 0x00007fbba2cdac68 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) + 72
26 libclang-cpp.so.22.1 0x00007fbba2cd91e0 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) + 1376
27 libclang-cpp.so.22.1 0x00007fbba2cd899a clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) + 394
28 libclang-cpp.so.22.1 0x00007fbba2ce11f6 clang::Parser::ParseCompoundStatementBody(bool) + 2246
29 libclang-cpp.so.22.1 0x00007fbba2ce1e89 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) + 185
30 libclang-cpp.so.22.1 0x00007fbba2cf9b78 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) + 2776
31 libclang-cpp.so.22.1 0x00007fbba2c4a1e0 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) + 5648
32 libclang-cpp.so.22.1 0x00007fbba2cf8e6e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) + 942
33 libclang-cpp.so.22.1 0x00007fbba2cf88dc clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) + 588
34 libclang-cpp.so.22.1 0x00007fbba2cf7bdf clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) + 2079
35 libclang-cpp.so.22.1 0x00007fbba2cf63b7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) + 1271
36 libclang-cpp.so.22.1 0x00007fbba2c3513e clang::ParseAST(clang::Sema&amp;, bool, bool) + 798
37 libclang-cpp.so.22.1 0x00007fbba496ca1f clang::FrontendAction::Execute() + 47
38 libclang-cpp.so.22.1 0x00007fbba48dfe74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) + 948
39 libclang-cpp.so.22.1 0x00007fbba49fabda clang::ExecuteCompilerInvocation(clang::CompilerInstance*) + 746
40 clang++-22           0x000055cf6ab68cdd cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) + 7245
41 clang++-22           0x000055cf6ab6553b
42 clang++-22           0x000055cf6ab66fcc
43 libclang-cpp.so.22.1 0x00007fbba45952ad
44 libLLVM.so.22.1      0x00007fbb9d1cd7a0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) + 144
45 libclang-cpp.so.22.1 0x00007fbba4594d61 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const + 369
46 libclang-cpp.so.22.1 0x00007fbba4553f12 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const + 498
47 libclang-cpp.so.22.1 0x00007fbba45540de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const + 142
48 libclang-cpp.so.22.1 0x00007fbba45759af clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) + 367
49 clang++-22           0x000055cf6ab6502d clang_main(int, char**, llvm::ToolContext const&amp;) + 6557
50 clang++-22           0x000055cf6ab7399a main + 106
51 libc.so.6            0x00007fbb97e33f75
52 libc.so.6            0x00007fbb97e34027 __libc_start_main + 135
53 clang++-22           0x000055cf6ab631d1 _start + 33
clang++-22: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 22.1.0 (2)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-22/bin
clang++-22: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++-22: note: diagnostic msg: /run/user/1000/tmp/crash-9d4581.cpp
clang++-22: note: diagnostic msg: /run/user/1000/tmp/crash-9d4581.sh
clang++-22: note: diagnostic msg: 

********************
```

[crash-9d4581.cpp](https://github.com/user-attachments/files/26019490/crash-9d4581.cpp)
[crash-9d4581.sh](https://github.com/user-attachments/files/26019489/crash-9d4581.sh)
</details>


---

