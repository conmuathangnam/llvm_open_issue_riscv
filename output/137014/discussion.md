# ICE in getLambdaCallOperatorHelper

**Author:** jeremy-rifkin
**URL:** https://github.com/llvm/llvm-project/issues/137014
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, lambda, regression:20
**Closed Date:** 2025-05-02T06:42:31Z

## Body

The following reduced code ICEs clang 20.1.

```cpp
template < template < typename> typename> struct TypeTList;
template < typename >
constexpr auto LambdaThing = []< template < typename> typename... Args >( TypeTList< Args... > ) {};
template < template < typename > typename TheThingT, typename TheParam >
struct TraitApplier {
    template < typename >
    using X = TheThingT< TheParam >;
};
template < typename Traits >
concept FooTraitsConcept = requires {
    LambdaThing< typename Traits::FooTypes >;
};
template < FooTraitsConcept >
class Foo;
struct FooTraits {
    using FooTypes = int;
};
void foo() {
    TraitApplier< Foo, FooTraits> x;
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<source>:19:36: current parser token ';'
2.	<source>:18:12: parsing function body 'foo'
3.	<source>:18:12: in compound statement ('{}')
4.	<source>:5:8: instantiating class definition 'TraitApplier<Foo, FooTraits>'
  #0 0x0000000003a59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a59568)
  #1 0x0000000003a576ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a576ac)
  #2 0x00000000039a69e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x00007235a6442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #4 0x0000000007375203 getLambdaCallOperatorHelper(clang::CXXRecordDecl const&) (.part.0) DeclCXX.cpp:0:0
  #5 0x00000000073760db clang::CXXRecordDecl::getLambdaCallOperator() const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x73760db)
......
```

https://godbolt.org/z/qnGbPzTKv

This appears to be a clang 20.x regression.




## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Jeremy Rifkin (jeremy-rifkin)

<details>
The following reduced code ICEs clang 20.1.

```cpp
template &lt; template &lt; typename&gt; typename&gt; struct TypeTList;
template &lt; typename &gt;
constexpr auto LambdaThing = []&lt; template &lt; typename&gt; typename... Args &gt;( TypeTList&lt; Args... &gt; ) {};
template &lt; template &lt; typename &gt; typename TheThingT, typename TheParam &gt;
struct TraitApplier {
    template &lt; typename &gt;
    using X = TheThingT&lt; TheParam &gt;;
};
template &lt; typename Traits &gt;
concept FooTraitsConcept = requires {
    LambdaThing&lt; typename Traits::FooTypes &gt;;
};
template &lt; FooTraitsConcept &gt;
class Foo;
struct FooTraits {
    using FooTypes = int;
};
void foo() {
    TraitApplier&lt; Foo, FooTraits&gt; x;
}
```

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 &lt;source&gt;
1.	&lt;source&gt;:19:36: current parser token ';'
2.	&lt;source&gt;:18:12: parsing function body 'foo'
3.	&lt;source&gt;:18:12: in compound statement ('{}')
4.	&lt;source&gt;:5:8: instantiating class definition 'TraitApplier&lt;Foo, FooTraits&gt;'
  #<!-- -->0 0x0000000003a59568 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a59568)
  #<!-- -->1 0x0000000003a576ac llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x3a576ac)
  #<!-- -->2 0x00000000039a69e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #<!-- -->3 0x00007235a6442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #<!-- -->4 0x0000000007375203 getLambdaCallOperatorHelper(clang::CXXRecordDecl const&amp;) (.part.0) DeclCXX.cpp:0:0
  #<!-- -->5 0x00000000073760db clang::CXXRecordDecl::getLambdaCallOperator() const (/opt/compiler-explorer/clang-20.1.0/bin/clang+++0x73760db)
......
```

https://godbolt.org/z/qnGbPzTKv

This appears to be a clang 20.x regression.



</details>


---

### Comment 2 - tbaederr

With assertions: https://godbolt.org/z/KhEzE4Yze

Assertion:

```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/DeclCXX.cpp:1699:
clang::NamedDecl* getLambdaCallOperatorHelper(const clang::CXXRecordDecl&):
Assertion `!Calls.empty() && "Missing lambda call operator!"' failed.
```

Backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:19:36: current parser token ';'
2.	<source>:18:12: parsing function body 'foo'
3.	<source>:18:12: in compound statement ('{}')
4.	<source>:5:8: instantiating class definition 'TraitApplier<Foo, FooTraits>'
  #0 0x0000000003ef3f78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef3f78)
  #1 0x0000000003ef1c04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef1c04)
  #2 0x0000000003e36738 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x0000759ab3442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #4 0x0000759ab34969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
  #5 0x0000759ab3442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
  #6 0x0000759ab34287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
  #7 0x0000759ab342871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
  #8 0x0000759ab3439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #9 0x000000000766942b getLambdaCallOperatorHelper(clang::CXXRecordDecl const&) (.part.0) DeclCXX.cpp:0:0
 #10 0x0000000007669e5b clang::CXXRecordDecl::getLambdaCallOperator() const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7669e5b)
 #11 0x00000000070d4e39 clang::Sema::getTemplateInstantiationArgs(clang::NamedDecl const*, clang::DeclContext const*, bool, std::optional<llvm::ArrayRef<clang::TemplateArgument>>, bool, clang::FunctionDecl const*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70d4e39)
 #12 0x0000000006f92335 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f92335)
 #13 0x00000000070365c9 clang::Sema::isTemplateTemplateParameterAtLeastAsSpecializedAs(clang::TemplateParameterList*, clang::TemplateDecl*, clang::TemplateDecl*, clang::DefaultArguments const&, clang::SourceLocation, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70365c9)
 #14 0x0000000006f8a3f3 clang::Sema::CheckTemplateTemplateArgument(clang::TemplateTemplateParmDecl*, clang::TemplateParameterList*, clang::TemplateArgumentLoc&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f8a3f3)
 #15 0x0000000006f8d310 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f8d310)
 #16 0x0000000006f90fc1 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f90fc1)
 #17 0x0000000006f95e85 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f95e85)
 #18 0x000000000710eb1c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #19 0x00000000070fdb43 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #20 0x000000000711cf04 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #21 0x00000000070fcd89 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #22 0x000000000710377a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #23 0x00000000071121ee clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #24 0x000000000711fc0a clang::Sema::SubstParmVarDecl(clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, int, clang::UnsignedOrNone, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x711fc0a)
 #25 0x0000000007120be6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionTypeParams(clang::SourceLocation, llvm::ArrayRef<clang::ParmVarDecl*>, clang::QualType const*, clang::FunctionType::ExtParameterInfo const*, llvm::SmallVectorImpl<clang::QualType>&, llvm::SmallVectorImpl<clang::ParmVarDecl*>*, clang::Sema::ExtParameterInfoBuilder&, unsigned int*) SemaTemplateInstantiate.cpp:0:0
 #26 0x00000000070fbc4c clang::QualType clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType<clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)>(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc, clang::CXXRecordDecl*, clang::Qualifiers, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformFunctionProtoType(clang::TypeLocBuilder&, clang::FunctionProtoTypeLoc)::'lambda'(clang::FunctionProtoType::ExceptionSpecInfo&, bool&)) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #27 0x00000000070fcef7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #28 0x00000000070ea0db clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #29 0x00000000070eb832 (anonymous namespace)::TemplateInstantiator::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #30 0x00000000070ebdbf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #31 0x00000000070eddc5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #32 0x00000000070ee3db clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70ee3db)
 #33 0x0000000007142216 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7142216)
 #34 0x0000000007191810 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7191810)
 #35 0x0000000007192116 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7192116)
 #36 0x000000000719c6e7 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x719c6e7)
 #37 0x0000000006f941d7 clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f941d7)
 #38 0x0000000006f94871 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f94871)
 #39 0x0000000006f9c16b clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9c16b)
 #40 0x00000000071152fe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #41 0x00000000070ebeee clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #42 0x000000000712288c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
 #43 0x0000000007124d26 (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
 #44 0x00000000070ebf79 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #45 0x00000000070fb792 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70fb792)
 #46 0x000000000684058f calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #47 0x0000000006842588 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #48 0x0000000006842ba5 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6842ba5)
 #49 0x0000000006f937bb clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f937bb)
 #50 0x000000000710e570 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
 #51 0x00000000070ebcc7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #52 0x00000000070fb792 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70fb792)
 #53 0x000000000684058f calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #54 0x0000000006842588 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
 #55 0x0000000006842ba5 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6842ba5)
 #56 0x000000000684862c clang::Sema::EnsureTemplateArgumentListConstraints(clang::TemplateDecl*, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684862c)
 #57 0x0000000006f92365 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f92365)
 #58 0x0000000006f95e85 clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f95e85)
 #59 0x000000000710eb1c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
 #60 0x00000000070fdb43 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #61 0x000000000711cf04 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformElaboratedType(clang::TypeLocBuilder&, clang::ElaboratedTypeLoc) SemaTemplateInstantiate.cpp:0:0
 #62 0x00000000070fcd89 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
 #63 0x000000000710377a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
 #64 0x00000000071112f6 clang::Sema::SubstType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71112f6)
 #65 0x000000000719f861 clang::TemplateDeclInstantiator::InstantiateTypedefNameDecl(clang::TypedefNameDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x719f861)
 #66 0x00000000071a7304 clang::TemplateDeclInstantiator::InstantiateTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71a7304)
 #67 0x00000000071a75af clang::TemplateDeclInstantiator::VisitTypeAliasTemplateDecl(clang::TypeAliasTemplateDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71a75af)
 #68 0x00000000071132eb clang::Sema::InstantiateClass(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71132eb)
 #69 0x0000000007135c00 clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7135c00)
 #70 0x00000000071b458e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
 #71 0x0000000007e22701 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7e22701)
 #72 0x00000000071bd37a clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71bd37a)
 #73 0x00000000071bd975 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71bd975)
 #74 0x00000000071bdaf5 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71bdaf5)
 #75 0x00000000068ff1d1 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
 #76 0x0000000006589048 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6589048)
 #77 0x0000000006598a99 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6598a99)
 #78 0x00000000065a1757 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65a1757)
 #79 0x00000000065a1c19 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65a1c19)
 #80 0x0000000006651472 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6651472)
 #81 0x000000000665217d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665217d)
 #82 0x0000000006659e13 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6659e13)
 #83 0x000000000665a5da clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665a5da)
 #84 0x00000000065633d3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65633d3)
 #85 0x000000000659949d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659949d)
 #86 0x0000000006556f0e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6556f0e)
 #87 0x00000000065576c9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65576c9)
 #88 0x000000000655efea clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655efea)
 #89 0x000000000655ff8d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655ff8d)
 #90 0x000000000655236a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655236a)
 #91 0x0000000004891e38 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4891e38)
 #92 0x0000000004b84f85 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84f85)
 #93 0x0000000004b03d4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b03d4e)
 #94 0x0000000004c73e7e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c73e7e)
 #95 0x0000000000d9f3af cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f3af)
 #96 0x0000000000d963ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
 #97 0x00000000048f84f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
 #98 0x0000000003e36bd4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e36bd4)
 #99 0x00000000048f8b0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#100 0x00000000048baf6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48baf6d)
#101 0x00000000048bbffe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48bbffe)
#102 0x00000000048c3e55 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48c3e55)
#103 0x0000000000d9c0f8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9c0f8)
#104 0x0000000000c236a4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc236a4)
#105 0x0000759ab3429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#106 0x0000759ab3429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#107 0x0000000000d95e65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd95e65)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 3 - shafik

@tbaederr we should always provide the assertion and backtrace or searchability, it becomes very hard to find duplicates otherwise.

This means when someone fixes it they may miss opportunities to extend their testing b/c they won't have all the test cases easily available to them.

---

### Comment 4 - shafik

Looks very similar to: https://github.com/llvm/llvm-project/issues/136432 but this one does not reproduce in clang-19 but does in clang-20. Maybe different paths to the same bug though.

CC @zyn0217 

---

