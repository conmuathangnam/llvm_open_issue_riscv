# [clang] ICE on NTTP with a constrained placeholder type in static/non-static member function templates of class templates

**Author:** Shuangcheng-Ni
**URL:** https://github.com/llvm/llvm-project/issues/146095

## Body

The [code](https://godbolt.org/z/xM46nMYaM) below causes ICE. The crash happens whether `eq` is declared static or non-static.
```cpp
#include <concepts>

template <int>
struct S {
    template <class T, std::convertible_to<T> auto V>
    constexpr static bool eq(const T &arg) {
        return T{V} == arg;
    }
};

int main() { static_assert(S<0>::eq<long, '2'>(0x32)); }
```

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20250627/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20250627/lib/clang/21 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20250627/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++26 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-b2ab85.o -x c++ <source>
1.	<source>:11:52: current parser token ')'
2.	<source>:11:12: parsing function body 'main'
3.	<source>:11:12: in compound statement ('{}')
 #0 0x0000000003c43078 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x3c43078)
 #1 0x0000000003c40c34 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007ee3d3042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x000000000708e049 clang::ASTContext::getSubstTemplateTypeParmType(clang::QualType, clang::Decl*, unsigned int, clang::UnsignedOrNone, bool) const (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x708e049)
 #4 0x0000000006d4104c (anonymous namespace)::TemplateInstantiator::BuildSubstTemplateTypeParmType(clang::TypeLocBuilder&, bool, bool, clang::Decl*, unsigned int, clang::UnsignedOrNone, clang::TemplateArgument, clang::SourceLocation) SemaTemplateInstantiate.cpp:0:0
 #5 0x0000000006d6f931 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
 #6 0x0000000006d62057 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #7 0x0000000006d63e97 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #8 0x0000000006d693d1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #9 0x0000000006d6a16d bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000006d6b78e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000006d59789 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006d5fb25 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6d5fb25)
#13 0x0000000006512e55 calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#14 0x0000000006514eac CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#15 0x0000000006518157 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (.constprop.0) SemaConcept.cpp:0:0
#16 0x000000000651922a clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x651922a)
#17 0x0000000006cf95f0 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6cf95f0)
#18 0x0000000006cfa657 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#19 0x00000000079d4fb1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x79d4fb1)
#20 0x0000000006cf0c8b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6cf0c8b)
#21 0x0000000006aee0bc AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#22 0x0000000006aef931 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#23 0x0000000006af0ad4 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6af0ad4)
#24 0x0000000006af0d7e clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6af0d7e)
#25 0x0000000006b085d3 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6b085d3)
#26 0x0000000006787cd2 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6787cd2)
#27 0x000000000678c34c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x678c34c)
#28 0x000000000628a86e clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x628a86e)
#29 0x000000000628292a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x628292a)
#30 0x000000000628469d clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x628469d)
#31 0x0000000006290a89 clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypeCastState) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6290a89)
#32 0x000000000626722a clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x626722a)
#33 0x0000000006254e3d clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6254e3d)
#34 0x0000000006321c96 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6321c96)
#35 0x00000000063225ce clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x63225ce)
#36 0x000000000632a35d clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x632a35d)
#37 0x000000000632ad9a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x632ad9a)
#38 0x0000000006205a16 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6205a16)
#39 0x0000000006251f6f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6251f6f)
#40 0x00000000061fddb7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x61fddb7)
#41 0x00000000061febce clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x61febce)
#42 0x000000000620873f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x620873f)
#43 0x0000000006209fa8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6209fa8)
#44 0x00000000061f839a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x61f839a)
#45 0x0000000004590f6d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x4590f6d)
#46 0x00000000048a5e0a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x48a5e0a)
#47 0x000000000482155b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x482155b)
#48 0x000000000499a8e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x499a8e3)
#49 0x0000000000d9fa85 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0xd9fa85)
#50 0x0000000000d9796d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#51 0x0000000000d9a811 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0xd9a811)
#52 0x0000000000c4e4f4 main (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0xc4e4f4)
#53 0x00007ee3d3029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#54 0x00007ee3d3029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#55 0x0000000000d97405 _start (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0xd97405)
```

The code compiles correctly in the following cases:
- A trailing requires clause is used instead of a constrained placeholder type.
  ```cpp
  #include <concepts>
  
  template <int>
  struct S {
      template <class T, auto V>
          requires std::convertible_to<decltype(V), T>
      constexpr static bool eq(const T &arg) {
          return T{V} == arg;
      }
  };
  
  int main() { static_assert(S<0>::eq<long, '2'>(0x32)); }
  ```
- `S` is a class instead of a class template.
  ```cpp
  #include <concepts>
  
  struct S {
      template <class T, std::convertible_to<T> auto V>
      constexpr static bool eq(const T &arg) {
          return T{V} == arg;
      }
  };
  
  int main() { static_assert(S::eq<long, '2'>(0x32)); }
  ```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Shuangcheng-Ni)

<details>
The [code](https://godbolt.org/z/xM46nMYaM) below causes ICE. The crash happens whether `eq` is declared static or non-static.
```cpp
#include &lt;concepts&gt;

template &lt;int&gt;
struct S {
    template &lt;class T, std::convertible_to&lt;T&gt; auto V&gt;
    constexpr static bool eq(const T &amp;arg) {
        return T{V} == arg;
    }
};

int main() { static_assert(S&lt;0&gt;::eq&lt;long, '2'&gt;(0x32)); }
```

Stack dump:
```
0.	Program arguments: /opt/compiler-explorer/clang-trunk-20250627/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-trunk-20250627/lib/clang/21 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /opt/compiler-explorer/clang-trunk-20250627/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++26 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-b2ab85.o -x c++ &lt;source&gt;
1.	&lt;source&gt;:11:52: current parser token ')'
2.	&lt;source&gt;:11:12: parsing function body 'main'
3.	&lt;source&gt;:11:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003c43078 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x3c43078)
 #<!-- -->1 0x0000000003c40c34 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007ee3d3042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x000000000708e049 clang::ASTContext::getSubstTemplateTypeParmType(clang::QualType, clang::Decl*, unsigned int, clang::UnsignedOrNone, bool) const (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x708e049)
 #<!-- -->4 0x0000000006d4104c (anonymous namespace)::TemplateInstantiator::BuildSubstTemplateTypeParmType(clang::TypeLocBuilder&amp;, bool, bool, clang::Decl*, unsigned int, clang::UnsignedOrNone, clang::TemplateArgument, clang::SourceLocation) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->5 0x0000000006d6f931 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&amp;, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->6 0x0000000006d62057 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->7 0x0000000006d63e97 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->8 0x0000000006d693d1 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->9 0x0000000006d6a16d bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x0000000006d6b78e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000006d59789 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x0000000006d5fb25 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6d5fb25)
#<!-- -->13 0x0000000006512e55 calculateConstraintSatisfaction(clang::Sema&amp;, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->14 0x0000000006514eac CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->15 0x0000000006518157 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (.constprop.0) SemaConcept.cpp:0:0
#<!-- -->16 0x000000000651922a clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x651922a)
#<!-- -->17 0x0000000006cf95f0 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6cf95f0)
#<!-- -->18 0x0000000006cfa657 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->19 0x00000000079d4fb1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x79d4fb1)
#<!-- -->20 0x0000000006cf0c8b clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6cf0c8b)
#<!-- -->21 0x0000000006aee0bc AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#<!-- -->22 0x0000000006aef931 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#<!-- -->23 0x0000000006af0ad4 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6af0ad4)
#<!-- -->24 0x0000000006af0d7e clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult&lt;clang::Expr*, true&gt;*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6af0d7e)
#<!-- -->25 0x0000000006b085d3 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6b085d3)
#<!-- -->26 0x0000000006787cd2 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6787cd2)
#<!-- -->27 0x000000000678c34c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x678c34c)
#<!-- -->28 0x000000000628a86e clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x628a86e)
#<!-- -->29 0x000000000628292a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x628292a)
#<!-- -->30 0x000000000628469d clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x628469d)
#<!-- -->31 0x0000000006290a89 clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypeCastState) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6290a89)
#<!-- -->32 0x000000000626722a clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&amp;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x626722a)
#<!-- -->33 0x0000000006254e3d clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6254e3d)
#<!-- -->34 0x0000000006321c96 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6321c96)
#<!-- -->35 0x00000000063225ce clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x63225ce)
#<!-- -->36 0x000000000632a35d clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x632a35d)
#<!-- -->37 0x000000000632ad9a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x632ad9a)
#<!-- -->38 0x0000000006205a16 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6205a16)
#<!-- -->39 0x0000000006251f6f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6251f6f)
#<!-- -->40 0x00000000061fddb7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x61fddb7)
#<!-- -->41 0x00000000061febce clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x61febce)
#<!-- -->42 0x000000000620873f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x620873f)
#<!-- -->43 0x0000000006209fa8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x6209fa8)
#<!-- -->44 0x00000000061f839a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x61f839a)
#<!-- -->45 0x0000000004590f6d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x4590f6d)
#<!-- -->46 0x00000000048a5e0a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x48a5e0a)
#<!-- -->47 0x000000000482155b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x482155b)
#<!-- -->48 0x000000000499a8e3 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0x499a8e3)
#<!-- -->49 0x0000000000d9fa85 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0xd9fa85)
#<!-- -->50 0x0000000000d9796d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->51 0x0000000000d9a811 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0xd9a811)
#<!-- -->52 0x0000000000c4e4f4 main (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0xc4e4f4)
#<!-- -->53 0x00007ee3d3029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->54 0x00007ee3d3029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->55 0x0000000000d97405 _start (/opt/compiler-explorer/clang-trunk-20250627/bin/clang-21+0xd97405)
```

The code compiles correctly in the following cases:
- A trailing requires clause is used instead of a constrained placeholder type.
  ```cpp
  #include &lt;concepts&gt;
  
  template &lt;int&gt;
  struct S {
      template &lt;class T, auto V&gt;
          requires std::convertible_to&lt;decltype(V), T&gt;
      constexpr static bool eq(const T &amp;arg) {
          return T{V} == arg;
      }
  };
  
  int main() { static_assert(S&lt;0&gt;::eq&lt;long, '2'&gt;(0x32)); }
  ```
- `S` is a class instead of a class template.
  ```cpp
  #include &lt;concepts&gt;
  
  struct S {
      template &lt;class T, std::convertible_to&lt;T&gt; auto V&gt;
      constexpr static bool eq(const T &amp;arg) {
          return T{V} == arg;
      }
  };
  
  int main() { static_assert(S::eq&lt;long, '2'&gt;(0x32)); }
  ```
</details>


---

### Comment 2 - shafik

Crash started on clang-16: https://godbolt.org/z/49vYEWsvx

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaTemplateInstantiate.cpp:2701: 
clang::QualType {anonymous}::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool):
Assertion `Arg.getKind() == TemplateArgument::Type && "Template argument kind mismatch"' failed.
```

Backtrace:


```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:11:52: current parser token ')'
2.	<source>:11:12: parsing function body 'main'
3.	<source>:11:12: in compound statement ('{}')
 #0 0x0000000003f854d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f854d8)
 #1 0x0000000003f83164 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f83164)
 #2 0x0000000003ec7c58 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000725024a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000725024a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000725024a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000725024a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000725024a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000725024a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007185743 (anonymous namespace)::TemplateInstantiator::TransformTemplateTypeParmType(clang::TypeLocBuilder&, clang::TemplateTypeParmTypeLoc, bool) SemaTemplateInstantiate.cpp:0:0
#10 0x000000000716fd46 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#11 0x000000000717489a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000717ca6b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#13 0x000000000717d1f8 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000717e1e8 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000717eb0e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000007164e41 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x000000000716e852 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x716e852)
#18 0x000000000692e22e calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#19 0x0000000006930229 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#20 0x0000000006930855 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6930855)
#21 0x0000000006932759 clang::Sema::CheckFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6932759)
#22 0x00000000070f8488 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool (bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70f8488)
#23 0x00000000070f9987 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#24 0x0000000007ec12d1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ec12d1)
#25 0x00000000070d6165 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70d6165)
#26 0x0000000006eff00b AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#27 0x0000000006f01458 clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.constprop.1) SemaOverload.cpp:0:0
#28 0x0000000006f0164b AddOverloadedCallCandidate(clang::Sema&, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool) SemaOverload.cpp:0:0
#29 0x0000000006f02e01 clang::Sema::AddOverloadedCallCandidates(clang::UnresolvedLookupExpr*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f02e01)
#30 0x0000000006f030fe clang::Sema::buildOverloadedCallSet(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::OverloadCandidateSet*, clang::ActionResult<clang::Expr*, true>*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f030fe)
#31 0x0000000006f18290 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f18290)
#32 0x0000000006b948fb clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b948fb)
#33 0x0000000006b95aac clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b95aac)
#34 0x00000000066b755f clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b755f)
#35 0x00000000066b048a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b048a)
#36 0x00000000066b06fe clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b06fe)
#37 0x00000000066bd82e clang::Parser::ParseConstantExpressionInExprEvalContext(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66bd82e)
#38 0x00000000066940eb clang::Parser::ParseStaticAssertDeclaration(clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66940eb)
#39 0x0000000006682816 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6682816)
#40 0x0000000006742652 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6742652)
#41 0x000000000674332d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674332d)
#42 0x000000000674b253 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674b253)
#43 0x000000000674ba1a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x674ba1a)
#44 0x0000000006644293 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6644293)
#45 0x0000000006679f5d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6679f5d)
#46 0x0000000006637d7e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6637d7e)
#47 0x0000000006638529 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6638529)
#48 0x000000000663fe6a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663fe6a)
#49 0x0000000006640e3d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6640e3d)
#50 0x00000000066331aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66331aa)
#51 0x000000000493bf28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493bf28)
#52 0x0000000004c391c5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c391c5)
#53 0x0000000004bb634e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb634e)
#54 0x0000000004d2bdc1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2bdc1)
#55 0x0000000000d8e78f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8e78f)
#56 0x0000000000d8536a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#57 0x00000000049a2d99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#58 0x0000000003ec80f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ec80f4)
#59 0x00000000049a33af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#60 0x000000000496515d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496515d)
#61 0x00000000049661ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49661ee)
#62 0x000000000496e6a5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496e6a5)
#63 0x0000000000d8ac3f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd8ac3f)
#64 0x0000000000c408f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc408f4)
#65 0x0000725024a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#66 0x0000725024a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#67 0x0000000000d84e15 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd84e15)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - Backl1ght

https://github.com/llvm/llvm-project/blob/f3c750f14951266f89148fe1647d04a1f0fadf63/clang/lib/Sema/SemaTemplateInstantiate.cpp#L2664

It seems like the line above wants to get template argument at (0, 0) of template function `eq`, i.e. `T`, but in fact get template argument at (0, 0) of template class `S`, i.e. `0`.

reproducer below makes the problem more clear.

```cpp
template< class From, class To >
concept convertible_to =
    requires (From from, To to) {
        from = to;
    };

template <typename U>
struct S {
    template <class TT, convertible_to<TT> auto V>
    constexpr static bool eq(const TT &arg) {
        return TT{V} == arg;
    }
};

int main() { static_assert(S<double>::eq<long, '2'>(0x32)); }
```

For this reproducer, at some point `Args` is corresponding to `double` while `T` is corrensponding to `TT`.

<img width="1703" height="140" alt="Image" src="https://github.com/user-attachments/assets/091c33bb-a66a-4b9d-b213-f61da8f0f9d6" />

---

### Comment 4 - shafik

CC @erichkeane you look at similar problems to this but maybe this is code @cor3ntin is touching now

---

