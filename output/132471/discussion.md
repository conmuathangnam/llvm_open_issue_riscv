# [Clang] Crash on naming data member in requires expression in local class

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/132471

## Body

The following C++20 code causes a crash since Clang 16.
```c++
template <class T>
void f(T) {
    struct A {
        int i;
        auto f() const {
            return [] (auto) requires requires { i; } {};
        }
    } a{};
    a.f()(0);
}

int main() {
    f(0);
}
```

See https://compiler-explorer.com/z/voz3eE6ba.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4226:
void clang::Sema::InstantiateClassMembers(clang::SourceLocation, clang::CXXRecordDecl*, const clang::MultiLevelTemplateArgumentList&, clang::TemplateSpecializationKind):
Assertion `(TSK == TSK_ExplicitInstantiationDefinition || TSK == TSK_ExplicitInstantiationDeclaration || (TSK == TSK_ImplicitInstantiation && Instantiation->isLocalClass())) && "Unexpected template specialization kind!"' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:2:6: instantiating function definition 'f<int>'
 #0 0x0000000003e8f628 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e8f628)
 #1 0x0000000003e8d2b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e8d2b4)
 #2 0x0000000003dd2148 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000077d320042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000077d3200969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000077d320042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000077d3200287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000077d32002871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000077d320039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000707a508 clang::Sema::InstantiateClassMembers(clang::SourceLocation, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x707a508)
#10 0x000000000710c7d6 clang::TemplateDeclInstantiator::VisitCXXRecordDecl(clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710c7d6)
#11 0x0000000007119b84 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#12 0x0000000007d8a701 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d8a701)
#13 0x00000000070b847a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b847a)
#14 0x00000000071066a1 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71066a1)
#15 0x000000000701ef1f (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#16 0x0000000007020428 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRecordType(clang::TypeLocBuilder&, clang::RecordTypeLoc) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000007051dbd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000007053c3d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformQualifiedType(clang::TypeLocBuilder&, clang::QualifiedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000705128c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#20 0x0000000007055be1 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformPointerType(clang::TypeLocBuilder&, clang::PointerTypeLoc) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000007051ceb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#22 0x000000000705653a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#23 0x000000000705665c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#24 0x0000000007056b15 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXThisExpr(clang::CXXThisExpr*) SemaTemplateInstantiate.cpp:0:0
#25 0x000000000705a50b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#26 0x0000000007080960 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformMemberExpr(clang::MemberExpr*) SemaTemplateInstantiate.cpp:0:0
#27 0x000000000705a760 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#28 0x0000000007066915 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#29 0x000000000705a990 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#30 0x0000000007082b92 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7082b92)
#31 0x00000000067a3b3c clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#32 0x00000000067a588e CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#33 0x00000000067a5eed clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a5eed)
#34 0x00000000067a6f38 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a6f38)
#35 0x0000000006fc95b5 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#36 0x0000000006fc99f7 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#37 0x0000000007d8a701 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d8a701)
#38 0x0000000006fa1c9d clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa1c9d)
#39 0x0000000006de48c1 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) SemaOverload.cpp:0:0
#40 0x0000000006de5211 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6de5211)
#41 0x0000000006df5882 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df5882)
#42 0x0000000006a01753 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a01753)
#43 0x0000000006a0203c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0203c)
#44 0x000000000706bb18 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#45 0x000000000705a573 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#46 0x00000000070885f7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#47 0x000000000708920e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#48 0x0000000007092254 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7092254)
#49 0x000000000710d8fd clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710d8fd)
#50 0x000000000710b8a6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710b8a6)
#51 0x000000000665a952 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#52 0x000000000665b1f2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665b1f2)
#53 0x00000000064c799b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c799b)
#54 0x00000000064b9e4a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64b9e4a)
#55 0x000000000484c098 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484c098)
#56 0x0000000004b1c685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1c685)
#57 0x0000000004a9e54e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a9e54e)
#58 0x0000000004c0c87e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0c87e)
#59 0x0000000000d5057f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5057f)
#60 0x0000000000d47d3a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#61 0x00000000048945f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#62 0x0000000003dd25e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3dd25e4)
#63 0x0000000004894c0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#64 0x000000000485767d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485767d)
#65 0x000000000485870e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485870e)
#66 0x000000000485fde5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485fde5)
#67 0x0000000000d4d378 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4d378)
#68 0x0000000000c140b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc140b4)
#69 0x000077d320029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#70 0x000077d320029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#71 0x0000000000d477e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd477e5)
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
The following C++20 code causes a crash since Clang 16.
```c++
template &lt;class T&gt;
void f(T) {
    struct A {
        int i;
        auto f() const {
            return [] (auto) requires requires { i; } {};
        }
    } a{};
    a.f()(0);
}

int main() {
    f(0);
}
```

See https://compiler-explorer.com/z/voz3eE6ba.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4226:
void clang::Sema::InstantiateClassMembers(clang::SourceLocation, clang::CXXRecordDecl*, const clang::MultiLevelTemplateArgumentList&amp;, clang::TemplateSpecializationKind):
Assertion `(TSK == TSK_ExplicitInstantiationDefinition || TSK == TSK_ExplicitInstantiationDeclaration || (TSK == TSK_ImplicitInstantiation &amp;&amp; Instantiation-&gt;isLocalClass())) &amp;&amp; "Unexpected template specialization kind!"' failed.
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:2:6: instantiating function definition 'f&lt;int&gt;'
 #<!-- -->0 0x0000000003e8f628 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e8f628)
 #<!-- -->1 0x0000000003e8d2b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e8d2b4)
 #<!-- -->2 0x0000000003dd2148 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000077d320042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000077d3200969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000077d320042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000077d3200287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000077d32002871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000077d320039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000707a508 clang::Sema::InstantiateClassMembers(clang::SourceLocation, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x707a508)
#<!-- -->10 0x000000000710c7d6 clang::TemplateDeclInstantiator::VisitCXXRecordDecl(clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710c7d6)
#<!-- -->11 0x0000000007119b84 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->12 0x0000000007d8a701 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d8a701)
#<!-- -->13 0x00000000070b847a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b847a)
#<!-- -->14 0x00000000071066a1 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71066a1)
#<!-- -->15 0x000000000701ef1f (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x0000000007020428 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRecordType(clang::TypeLocBuilder&amp;, clang::RecordTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x0000000007051dbd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x0000000007053c3d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformQualifiedType(clang::TypeLocBuilder&amp;, clang::QualifiedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x000000000705128c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x0000000007055be1 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformPointerType(clang::TypeLocBuilder&amp;, clang::PointerTypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x0000000007051ceb clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x000000000705653a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x000000000705665c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#<!-- -->24 0x0000000007056b15 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXThisExpr(clang::CXXThisExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x000000000705a50b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x0000000007080960 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformMemberExpr(clang::MemberExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->27 0x000000000705a760 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->28 0x0000000007066915 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->29 0x000000000705a990 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->30 0x0000000007082b92 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7082b92)
#<!-- -->31 0x00000000067a3b3c clang::ActionResult&lt;clang::Expr*, true&gt; calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt;(clang::Sema&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;, calculateConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::Expr const*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator const&amp;) SemaConcept.cpp:0:0
#<!-- -->32 0x00000000067a588e CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->33 0x00000000067a5eed clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a5eed)
#<!-- -->34 0x00000000067a6f38 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a6f38)
#<!-- -->35 0x0000000006fc95b5 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, llvm::function_ref&lt;bool ()&gt;) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->36 0x0000000006fc99f7 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::'lambda1'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->37 0x0000000007d8a701 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d8a701)
#<!-- -->38 0x0000000006fa1c9d clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa1c9d)
#<!-- -->39 0x0000000006de48c1 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) SemaOverload.cpp:0:0
#<!-- -->40 0x0000000006de5211 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6de5211)
#<!-- -->41 0x0000000006df5882 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df5882)
#<!-- -->42 0x0000000006a01753 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a01753)
#<!-- -->43 0x0000000006a0203c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0203c)
#<!-- -->44 0x000000000706bb18 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->45 0x000000000705a573 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->46 0x00000000070885f7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->47 0x000000000708920e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->48 0x0000000007092254 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7092254)
#<!-- -->49 0x000000000710d8fd clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710d8fd)
#<!-- -->50 0x000000000710b8a6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710b8a6)
#<!-- -->51 0x000000000665a952 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->52 0x000000000665b1f2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x665b1f2)
#<!-- -->53 0x00000000064c799b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c799b)
#<!-- -->54 0x00000000064b9e4a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64b9e4a)
#<!-- -->55 0x000000000484c098 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x484c098)
#<!-- -->56 0x0000000004b1c685 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1c685)
#<!-- -->57 0x0000000004a9e54e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a9e54e)
#<!-- -->58 0x0000000004c0c87e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c0c87e)
#<!-- -->59 0x0000000000d5057f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5057f)
#<!-- -->60 0x0000000000d47d3a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->61 0x00000000048945f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->62 0x0000000003dd25e4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3dd25e4)
#<!-- -->63 0x0000000004894c0f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->64 0x000000000485767d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485767d)
#<!-- -->65 0x000000000485870e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485870e)
#<!-- -->66 0x000000000485fde5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485fde5)
#<!-- -->67 0x0000000000d4d378 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd4d378)
#<!-- -->68 0x0000000000c140b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc140b4)
#<!-- -->69 0x000077d320029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->70 0x000077d320029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->71 0x0000000000d477e5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd477e5)
```

&lt;/details&gt;
</details>


---

