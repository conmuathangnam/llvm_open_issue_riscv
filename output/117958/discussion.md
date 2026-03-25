# Crashes with Template-based Type Checking and Array Size Deduction

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/117958

## Body

This crash can be observed in both LLVM 18 and LLVM trunk versions.
([https://godbolt.org/z/Wzne5GvzW](https://godbolt.org/z/Wzne5GvzW))
# Command
`clang++ bug.cpp`

# Code
```cpp
template<typename A, typename B> struct check_derived_from { 
  static A a; 
  static constexpr B *p = &a;  
  int ar[p-p+1]; 
}; 
struct W {
  check_derived_from<W, int> cdf;
};
struct X : W
{
  check_derived_from<X, W> cdf;
};
struct A { };
struct B : A { };
struct C : B
{
  check_derived_from<C, B> cdf;
};
struct D : C
{
  check_derived_from<D, C> cdf;
};
struct E : D
{
  check_derived_from<E, D> cdf;
};
struct F : E
{
  check_derived_from<F, E> cdf}; 
struct Z : W
{
  check_derived_from<Z, W> cdf;
};
```

# Version
```sh
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

# Error Output
```sh
Stack dump:
0.      Program arguments: /usr/lib/llvm-18/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/bugs -fcoverage-compilation-dir=/home/bugs -resource-dir /usr/lib/llvm-18/lib/clang/18 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/llvm-18/lib/clang/18/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -dwarf-debug-flags "/usr/lib/llvm-18/bin/clang --driver-mode=g++ bug.cpp -dumpdir a-" -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-6eee06.o -x c++ bug.cpp
1.      bug.cpp:21:31: current parser token ';'
2.      bug.cpp:19:1: parsing struct/union/class body 'D'
3.      bug.cpp:1:41: instantiating class definition 'check_derived_from<D, C>'
4.      bug.cpp:3:23: instantiating variable definition 'check_derived_from<D, C>::p'
 #0 0x00007f3d308e5b46 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd93b46)
 #1 0x00007f3d308e3af0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd91af0)
 #2 0x00007f3d308e6210 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94210)
 #3 0x00007f3d2f735090 (/lib/x86_64-linux-gnu/libc.so.6+0x43090)
 #4 0x00007f3d38201939 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x10b1939)
 #5 0x00007f3d382012fd (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x10b12fd)
 #6 0x00007f3d381f8f0f clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x10a8f0f)
 #7 0x00007f3d37ff6eb3 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xea6eb3)
 #8 0x00007f3d37ff380a (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xea380a)
 #9 0x00007f3d3803fd62 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xeefd62)
#10 0x00007f3d37fdb4a9 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xe8b4a9)
#11 0x00007f3d37fd7184 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xe87184)
#12 0x00007f3d37fd7c48 clang::Expr::EvaluateAsInitializer(clang::APValue&, clang::ASTContext const&, clang::VarDecl const*, llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xe87c48)
#13 0x00007f3d37f4ee53 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xdfee53)
#14 0x00007f3d37f4efa1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl<std::pair<clang::SourceLocation, clang::PartialDiagnostic>>&) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xdfefa1)
#15 0x00007f3d386b1d4d clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1561d4d)
#16 0x00007f3d386b0a48 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1560a48)
#17 0x00007f3d38ca5969 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b55969)
#18 0x00007f3d38caab19 clang::Sema::InstantiateVariableDefinition(clang::SourceLocation, clang::VarDecl*, bool, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b5ab19)
#19 0x00007f3d3850e33d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x13be33d)
#20 0x00007f3d38832583 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16e2583)
#21 0x00007f3d38832aef (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16e2aef)
#22 0x00007f3d387f8eea clang::Sema::MarkDeclRefReferenced(clang::DeclRefExpr*, clang::Expr const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a8eea)
#23 0x00007f3d387f887e clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a887e)
#24 0x00007f3d387f8765 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::CXXScopeSpec const*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a8765)
#25 0x00007f3d387fc923 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16ac923)
#26 0x00007f3d38c7ee22 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b2ee22)
#27 0x00007f3d38c76adb (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b26adb)
#28 0x00007f3d38c76adb (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b26adb)
#29 0x00007f3d38c8193f (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b3193f)
#30 0x00007f3d38c61850 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b11850)
#31 0x00007f3d38c6151c clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b1151c)
#32 0x00007f3d38c9a9a1 clang::TemplateDeclInstantiator::VisitFieldDecl(clang::FieldDecl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b4a9a1)
#33 0x00007f3d38c64e96 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b14e96)
#34 0x00007f3d38c66c50 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b16c50)
#35 0x00007f3d38d1501e (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1bc501e)
#36 0x00007f3d3850e33d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x13be33d)
#37 0x00007f3d38d06611 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1bb6611)
#38 0x00007f3d38d05dcf clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1bb5dcf)
#39 0x00007f3d386c071e clang::Sema::CheckFieldDecl(clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::RecordDecl*, clang::SourceLocation, bool, clang::Expr*, clang::InClassInitStyle, clang::SourceLocation, clang::AccessSpecifier, clang::NamedDecl*, clang::Declarator*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x157071e)
#40 0x00007f3d386c0550 clang::Sema::HandleField(clang::Scope*, clang::RecordDecl*, clang::SourceLocation, clang::Declarator&, clang::Expr*, clang::InClassInitStyle, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1570550)
#41 0x00007f3d3873725c clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Expr*, clang::VirtSpecifiers const&, clang::InClassInitStyle) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x15e725c)
#42 0x00007f3d37d139bd clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbc39bd)
#43 0x00007f3d37d15ce5 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbc5ce5)
#44 0x00007f3d37d100c1 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbc00c1)
#45 0x00007f3d37d0d92d clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbbd92d)
#46 0x00007f3d37cef20e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9f20e)
#47 0x00007f3d37d9fb00 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4fb00)
#48 0x00007f3d37d9f843 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4f843)
#49 0x00007f3d37d9eaa6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4eaa6)
#50 0x00007f3d37d9ceda clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4ceda)
#51 0x00007f3d37cd5c7e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb85c7e)
#52 0x00007f3d39b0d415 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29bd415)
#53 0x00007f3d39a87674 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2937674)
#54 0x00007f3d39b8769e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a3769e)
#55 0x000055e36f311c62 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-18/bin/clang+0x12c62)
#56 0x000055e36f30f095 (/usr/lib/llvm-18/bin/clang+0x10095)
#57 0x000055e36f30e256 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-18/bin/clang+0xf256)
#58 0x000055e36f31baf6 main (/usr/lib/llvm-18/bin/clang+0x1caf6)
#59 0x00007f3d2f716083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#60 0x000055e36f30bb0e _start (/usr/lib/llvm-18/bin/clang+0xcb0e)

```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
This crash can be observed in both LLVM 18 and LLVM trunk versions.
([https://godbolt.org/z/Wzne5GvzW](https://godbolt.org/z/Wzne5GvzW))
# Command
`clang++ bug.cpp`

# Code
```cpp
template&lt;typename A, typename B&gt; struct check_derived_from { 
  static A a; 
  static constexpr B *p = &amp;a;  
  int ar[p-p+1]; 
}; 
struct W {
  check_derived_from&lt;W, int&gt; cdf;
};
struct X : W
{
  check_derived_from&lt;X, W&gt; cdf;
};
struct A { };
struct B : A { };
struct C : B
{
  check_derived_from&lt;C, B&gt; cdf;
};
struct D : C
{
  check_derived_from&lt;D, C&gt; cdf;
};
struct E : D
{
  check_derived_from&lt;E, D&gt; cdf;
};
struct F : E
{
  check_derived_from&lt;F, E&gt; cdf}; 
struct Z : W
{
  check_derived_from&lt;Z, W&gt; cdf;
};
```

# Version
```sh
Ubuntu clang version 18.1.8 (++20240731025011+3b5b5c1ec4a3-1~exp1~20240731145104.143)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

# Error Output
```sh
Stack dump:
0.      Program arguments: /usr/lib/llvm-18/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name bug.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/home/bugs -fcoverage-compilation-dir=/home/bugs -resource-dir /usr/lib/llvm-18/lib/clang/18 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/x86_64-linux-gnu/c++/13 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../include/c++/13/backward -internal-isystem /usr/lib/llvm-18/lib/clang/18/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/13/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -dwarf-debug-flags "/usr/lib/llvm-18/bin/clang --driver-mode=g++ bug.cpp -dumpdir a-" -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/bug-6eee06.o -x c++ bug.cpp
1.      bug.cpp:21:31: current parser token ';'
2.      bug.cpp:19:1: parsing struct/union/class body 'D'
3.      bug.cpp:1:41: instantiating class definition 'check_derived_from&lt;D, C&gt;'
4.      bug.cpp:3:23: instantiating variable definition 'check_derived_from&lt;D, C&gt;::p'
 #<!-- -->0 0x00007f3d308e5b46 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd93b46)
 #<!-- -->1 0x00007f3d308e3af0 llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd91af0)
 #<!-- -->2 0x00007f3d308e6210 (/lib/x86_64-linux-gnu/libLLVM-18.so.18.1+0xd94210)
 #<!-- -->3 0x00007f3d2f735090 (/lib/x86_64-linux-gnu/libc.so.6+0x43090)
 #<!-- -->4 0x00007f3d38201939 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x10b1939)
 #<!-- -->5 0x00007f3d382012fd (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x10b12fd)
 #<!-- -->6 0x00007f3d381f8f0f clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x10a8f0f)
 #<!-- -->7 0x00007f3d37ff6eb3 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xea6eb3)
 #<!-- -->8 0x00007f3d37ff380a (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xea380a)
 #<!-- -->9 0x00007f3d3803fd62 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xeefd62)
#<!-- -->10 0x00007f3d37fdb4a9 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xe8b4a9)
#<!-- -->11 0x00007f3d37fd7184 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xe87184)
#<!-- -->12 0x00007f3d37fd7c48 clang::Expr::EvaluateAsInitializer(clang::APValue&amp;, clang::ASTContext const&amp;, clang::VarDecl const*, llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xe87c48)
#<!-- -->13 0x00007f3d37f4ee53 clang::VarDecl::evaluateValueImpl(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;, bool) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xdfee53)
#<!-- -->14 0x00007f3d37f4efa1 clang::VarDecl::checkForConstantInitialization(llvm::SmallVectorImpl&lt;std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;&gt;&amp;) const (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xdfefa1)
#<!-- -->15 0x00007f3d386b1d4d clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1561d4d)
#<!-- -->16 0x00007f3d386b0a48 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1560a48)
#<!-- -->17 0x00007f3d38ca5969 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b55969)
#<!-- -->18 0x00007f3d38caab19 clang::Sema::InstantiateVariableDefinition(clang::SourceLocation, clang::VarDecl*, bool, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b5ab19)
#<!-- -->19 0x00007f3d3850e33d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x13be33d)
#<!-- -->20 0x00007f3d38832583 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16e2583)
#<!-- -->21 0x00007f3d38832aef (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16e2aef)
#<!-- -->22 0x00007f3d387f8eea clang::Sema::MarkDeclRefReferenced(clang::DeclRefExpr*, clang::Expr const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a8eea)
#<!-- -->23 0x00007f3d387f887e clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a887e)
#<!-- -->24 0x00007f3d387f8765 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::CXXScopeSpec const*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16a8765)
#<!-- -->25 0x00007f3d387fc923 clang::Sema::BuildDeclarationNameExpr(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::NamedDecl*, clang::NamedDecl*, clang::TemplateArgumentListInfo const*, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x16ac923)
#<!-- -->26 0x00007f3d38c7ee22 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b2ee22)
#<!-- -->27 0x00007f3d38c76adb (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b26adb)
#<!-- -->28 0x00007f3d38c76adb (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b26adb)
#<!-- -->29 0x00007f3d38c8193f (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b3193f)
#<!-- -->30 0x00007f3d38c61850 (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b11850)
#<!-- -->31 0x00007f3d38c6151c clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b1151c)
#<!-- -->32 0x00007f3d38c9a9a1 clang::TemplateDeclInstantiator::VisitFieldDecl(clang::FieldDecl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b4a9a1)
#<!-- -->33 0x00007f3d38c64e96 clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b14e96)
#<!-- -->34 0x00007f3d38c66c50 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1b16c50)
#<!-- -->35 0x00007f3d38d1501e (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1bc501e)
#<!-- -->36 0x00007f3d3850e33d clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x13be33d)
#<!-- -->37 0x00007f3d38d06611 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1bb6611)
#<!-- -->38 0x00007f3d38d05dcf clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1bb5dcf)
#<!-- -->39 0x00007f3d386c071e clang::Sema::CheckFieldDecl(clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::RecordDecl*, clang::SourceLocation, bool, clang::Expr*, clang::InClassInitStyle, clang::SourceLocation, clang::AccessSpecifier, clang::NamedDecl*, clang::Declarator*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x157071e)
#<!-- -->40 0x00007f3d386c0550 clang::Sema::HandleField(clang::Scope*, clang::RecordDecl*, clang::SourceLocation, clang::Declarator&amp;, clang::Expr*, clang::InClassInitStyle, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x1570550)
#<!-- -->41 0x00007f3d3873725c clang::Sema::ActOnCXXMemberDeclarator(clang::Scope*, clang::AccessSpecifier, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, clang::Expr*, clang::VirtSpecifiers const&amp;, clang::InClassInitStyle) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x15e725c)
#<!-- -->42 0x00007f3d37d139bd clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::ParsingDeclRAIIObject*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbc39bd)
#<!-- -->43 0x00007f3d37d15ce5 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbc5ce5)
#<!-- -->44 0x00007f3d37d100c1 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbc00c1)
#<!-- -->45 0x00007f3d37d0d92d clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xbbd92d)
#<!-- -->46 0x00007f3d37cef20e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb9f20e)
#<!-- -->47 0x00007f3d37d9fb00 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4fb00)
#<!-- -->48 0x00007f3d37d9f843 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4f843)
#<!-- -->49 0x00007f3d37d9eaa6 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4eaa6)
#<!-- -->50 0x00007f3d37d9ceda clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xc4ceda)
#<!-- -->51 0x00007f3d37cd5c7e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0xb85c7e)
#<!-- -->52 0x00007f3d39b0d415 clang::FrontendAction::Execute() (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x29bd415)
#<!-- -->53 0x00007f3d39a87674 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2937674)
#<!-- -->54 0x00007f3d39b8769e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-18/bin/../lib/libclang-cpp.so.18.1+0x2a3769e)
#<!-- -->55 0x000055e36f311c62 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-18/bin/clang+0x12c62)
#<!-- -->56 0x000055e36f30f095 (/usr/lib/llvm-18/bin/clang+0x10095)
#<!-- -->57 0x000055e36f30e256 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-18/bin/clang+0xf256)
#<!-- -->58 0x000055e36f31baf6 main (/usr/lib/llvm-18/bin/clang+0x1caf6)
#<!-- -->59 0x00007f3d2f716083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->60 0x000055e36f30bb0e _start (/usr/lib/llvm-18/bin/clang+0xcb0e)

```
</details>


---

### Comment 2 - shafik

Is this fuzzer generated?

---

### Comment 3 - tbaederr

This runs into an assertion:

```console
clang: /home/tbaeder/code/llvm-project/clang/lib/AST/RecordLayoutBuilder.cpp:3348: const ASTRecordLayout &clang::ASTContext::getASTRecordLayout(const RecordDecl *) const: Assertion `D->isCompleteDefinition() && "Cannot layout type before complete!"' failed.
Process 3888427 stopped
* thread #1, name = 'clang', stop reason = signal SIGABRT
    frame #0: 0x00007fff778ae834 libc.so.6`__pthread_kill_implementation + 276
libc.so.6`__pthread_kill_implementation:
->  0x7fff778ae834 <+276>: movl   %eax, %ebx
    0x7fff778ae836 <+278>: negl   %ebx
    0x7fff778ae838 <+280>: cmpl   $0xfffff000, %eax ; imm = 0xFFFFF000
    0x7fff778ae83d <+285>: movl   $0x0, %eax
(lldb) bt
* thread #1, name = 'clang', stop reason = signal SIGABRT
  frame #0: 0x00007fff778ae834 libc.so.6`__pthread_kill_implementation + 276
  frame #1: 0x00007fff7785c8ee libc.so.6`raise + 30
  frame #2: 0x00007fff778448ff libc.so.6`abort + 223
  frame #3: 0x00007fff7784481b libc.so.6`__assert_fail_base.cold + 14
  frame #4: 0x00007fff77854c57 libc.so.6`__assert_fail + 71
    #5: RecordLayoutBuilder.cpp:3348  clang::ASTContext::getASTRecordLayout(this=0x00007e9f76820200, D=0x00007e0f768c2e18) const
    #6: ExprConstant.cpp:3261  HandleLValueDirectBase(Info=0x00007bff7593f8e0, E=0x00007e0f768c38a8, Obj=0x00007bff75940130, Derived=0x00007e0f768c2e18, Base=0x00007e0f768c2b40, RL=0x0000000000000000)
    #7: ExprConstant.cpp:3275  HandleLValueBase(Info=0x00007bff7593f8e0, E=0x00007e0f768c38a8, Obj=0x00007bff75940130, DerivedDecl=0x00007e0f768c2e18, Base=0x00007e0f768c2fe0)
    #8: ExprConstant.cpp:3299  HandleLValueBasePath(Info=0x00007bff7593f8e0, E=0x00007e0f768c38a8, Type=QualType @ 0x00007bff754a7160, Result=0x00007bff75940130)
    #9: ExprConstant.cpp:9574  (anonymous namespace)::PointerExprEvaluator::VisitCastExpr(this=0x00007bff755e50c0, E=0x00007e0f768c38a8)
    #10: StmtNodes.inc:534  clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::VisitImplicitCastExpr(this=0x00007bff755e50c0, S=0x00007e0f768c38a8)
    #11: StmtNodes.inc:534  clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::PointerExprEvaluator, bool>::Visit(this=0x00007bff755e50c0, S=0x00007e0f768c38a8)
    #12: ExprConstant.cpp:9458  EvaluatePointer(E=0x00007e0f768c38a8, Result=0x00007bff75940130, Info=0x00007bff7593f8e0, InvalidBaseOK=false)
    #13: ExprConstant.cpp:16354  Evaluate(Result=0x00007e0f768c3d10, Info=0x00007bff7593f8e0, E=0x00007e0f768c38a8)
    #14: ExprConstant.cpp:16445  EvaluateInPlace(Result=0x00007e0f768c3d10, Info=0x00007bff7593f8e0, This=0x00007bff7593fd60, E=0x00007e0f768c38a8, AllowNonLiteralTypes=true)
    #15: ExprConstant.cpp:16807  clang::Expr::EvaluateAsInitializer(this=0x00007e0f768c38a8, Value=0x00007e0f768c3d10, Ctx=0x00007e9f76820200, VD=0x00007e0f768c3678, Notes=0x00007bff75ba7b10, IsConstantInitialization=true) const
    #16: Decl.cpp:2573  clang::VarDecl::evaluateValueImpl(this=0x00007e0f768c3678, Notes=0x00007bff75ba7b10, IsConstantInitialization=true) const
    #17: Decl.cpp:2652  clang::VarDecl::checkForConstantInitialization(this=0x00007e0f768c3678, Notes=0x00007bff75ba7b10) const
    #18: SemaDecl.cpp:14507  clang::Sema::CheckCompleteVariableDeclaration(this=0x00007e8f7682a200, var=0x00007e0f768c3678)
    #19: SemaDecl.cpp:13903  clang::Sema::AddInitializerToDecl(this=0x00007e8f7682a200, RealDecl=0x00007e0f768c3678, Init=0x00007e0f768c38a8, DirectInit=false)
    #20: SemaTemplateInstantiateDecl.cpp:5523  clang::Sema::InstantiateVariableInitializer(this=0x00007e8f7682a200, Var=0x00007e0f768c3678, OldVar=0x00007e0f76897bb8, TemplateArgs=0x00007bff75a63050)
[...]
```


---

### Comment 4 - tbaederr

Smaller reproducer:
```c++
template<typename A, typename B>
struct check_derived_from {
  static A a;
  static constexpr B *p = &a;
  int ar[p - p+1];
};
struct B {};
struct C : B
{
  check_derived_from<C, B> cdf;
};
```

---

