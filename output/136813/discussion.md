# Clang crashes: "Out-of-bounds Bit access."

**Author:** wangtz0607
**URL:** https://github.com/llvm/llvm-project/issues/136813

## Body

## Example

```cpp
template <typename, typename>
class C {
public:
    template <typename>
    class C2 {
    public:
        C2(C &) {}
    };

    void f() {
        C2(*this);
    }
};

int main() {
    C<int, int>().f();
    return 0;
}
```

```sh
clang++ -std=c++20 example.cpp
```

## Actual Behavior

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:10:10: instantiating function definition 'C<int, int>::f'
 #0 0x0000000003bdfdb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdfdb8)
 #1 0x0000000003bddefc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bddefc)
 #2 0x0000000003b2e7f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071aeece42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00000000076ce50f clang::TemplateArgument::getNonTypeTemplateArgumentType() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76ce50f)
 #5 0x0000000006c43df8 checkDeducedTemplateArguments(clang::ASTContext&, clang::DeducedTemplateArgument const&, clang::DeducedTemplateArgument const&, bool) (.part.0) SemaTemplateDeduction.cpp:0:0
 #6 0x0000000006c85fc5 DeduceTemplateArgumentsByTypeMatch(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, PartialOrderingKind, bool, bool*) SemaTemplateDeduction.cpp:0:0
 #7 0x0000000006c89553 DeduceTemplateArguments(clang::Sema&, clang::TemplateParameterList*, clang::TemplateArgument const&, clang::TemplateArgument, clang::sema::TemplateDeductionInfo&, bool, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, bool*) SemaTemplateDeduction.cpp:0:0
 #8 0x0000000006c89c46 DeduceTemplateArguments(clang::Sema&, clang::TemplateParameterList*, llvm::ArrayRef<clang::TemplateArgument>, llvm::ArrayRef<clang::TemplateArgument>, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, bool, bool, PackFold, bool*) SemaTemplateDeduction.cpp:0:0
 #9 0x0000000006c8b53f DeduceTemplateSpecArguments(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, bool, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, bool*) SemaTemplateDeduction.cpp:0:0
#10 0x0000000006c867e6 DeduceTemplateArgumentsByTypeMatch(clang::Sema&, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, PartialOrderingKind, bool, bool*) SemaTemplateDeduction.cpp:0:0
#11 0x0000000006c91b1a DeduceTemplateArgumentsFromCallArgument(clang::Sema&, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg>&, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#12 0x0000000006c969e4 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c969e4)
#13 0x0000000006a9c46a AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#14 0x0000000006a9cdbf clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a9cdbf)
#15 0x0000000006a9ce17 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a9ce17)
#16 0x0000000006868301 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#17 0x0000000006868c1a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6868c1a)
#18 0x00000000067476eb clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67476eb)
#19 0x0000000006d218be clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#20 0x0000000006d01408 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000006d3bea9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#22 0x0000000006d3c685 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#23 0x0000000006d4335b clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d4335b)
#24 0x0000000006d99e67 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d99e67)
#25 0x0000000006d97f88 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d97f88)
#26 0x00000000062f5709 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#27 0x00000000062f5e9d clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f5e9d)
#28 0x000000000615ae83 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x615ae83)
#29 0x000000000614920a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x614920a)
#30 0x000000000451a5ed clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451a5ed)
#31 0x0000000004828c2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4828c2a)
#32 0x00000000047a535b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a535b)
#33 0x0000000004918a43 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4918a43)
#34 0x0000000000db1801 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb1801)
#35 0x0000000000da9f5d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x000000000458c189 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000003b2ec13 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b2ec13)
#38 0x000000000458c3a9 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x000000000454f7fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x454f7fd)
#40 0x00000000045507c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45507c1)
#41 0x000000000455ab0c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455ab0c)
#42 0x0000000000dae531 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdae531)
#43 0x0000000000c301d4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc301d4)
#44 0x000071aeece29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x000071aeece29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000da99f5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xda99f5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

https://godbolt.org/z/faxEK476f

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: wtz (wangtz0607)

<details>
## Example

```cpp
template &lt;typename, typename&gt;
class C {
public:
    template &lt;typename&gt;
    class C2 {
    public:
        C2(C &amp;) {}
    };

    void f() {
        C2(*this);
    }
};

int main() {
    C&lt;int, int&gt;().f();
    return 0;
}
```

```sh
clang++ -std=c++20 example.cpp
```

## Actual Behavior

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:10:10: instantiating function definition 'C&lt;int, int&gt;::f'
 #<!-- -->0 0x0000000003bdfdb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bdfdb8)
 #<!-- -->1 0x0000000003bddefc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bddefc)
 #<!-- -->2 0x0000000003b2e7f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000071aeece42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00000000076ce50f clang::TemplateArgument::getNonTypeTemplateArgumentType() const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x76ce50f)
 #<!-- -->5 0x0000000006c43df8 checkDeducedTemplateArguments(clang::ASTContext&amp;, clang::DeducedTemplateArgument const&amp;, clang::DeducedTemplateArgument const&amp;, bool) (.part.0) SemaTemplateDeduction.cpp:0:0
 #<!-- -->6 0x0000000006c85fc5 DeduceTemplateArgumentsByTypeMatch(clang::Sema&amp;, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, PartialOrderingKind, bool, bool*) SemaTemplateDeduction.cpp:0:0
 #<!-- -->7 0x0000000006c89553 DeduceTemplateArguments(clang::Sema&amp;, clang::TemplateParameterList*, clang::TemplateArgument const&amp;, clang::TemplateArgument, clang::sema::TemplateDeductionInfo&amp;, bool, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, bool*) SemaTemplateDeduction.cpp:0:0
 #<!-- -->8 0x0000000006c89c46 DeduceTemplateArguments(clang::Sema&amp;, clang::TemplateParameterList*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, bool, bool, PackFold, bool*) SemaTemplateDeduction.cpp:0:0
 #<!-- -->9 0x0000000006c8b53f DeduceTemplateSpecArguments(clang::Sema&amp;, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&amp;, bool, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, bool*) SemaTemplateDeduction.cpp:0:0
#<!-- -->10 0x0000000006c867e6 DeduceTemplateArgumentsByTypeMatch(clang::Sema&amp;, clang::TemplateParameterList*, clang::QualType, clang::QualType, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, PartialOrderingKind, bool, bool*) SemaTemplateDeduction.cpp:0:0
#<!-- -->11 0x0000000006c91b1a DeduceTemplateArgumentsFromCallArgument(clang::Sema&amp;, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt;&amp;, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#<!-- -->12 0x0000000006c969e4 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6c969e4)
#<!-- -->13 0x0000000006a9c46a AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#<!-- -->14 0x0000000006a9cdbf clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a9cdbf)
#<!-- -->15 0x0000000006a9ce17 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6a9ce17)
#<!-- -->16 0x0000000006868301 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#<!-- -->17 0x0000000006868c1a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6868c1a)
#<!-- -->18 0x00000000067476eb clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67476eb)
#<!-- -->19 0x0000000006d218be clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x0000000006d01408 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x0000000006d3bea9 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x0000000006d3c685 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x0000000006d4335b clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d4335b)
#<!-- -->24 0x0000000006d99e67 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d99e67)
#<!-- -->25 0x0000000006d97f88 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6d97f88)
#<!-- -->26 0x00000000062f5709 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->27 0x00000000062f5e9d clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f5e9d)
#<!-- -->28 0x000000000615ae83 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x615ae83)
#<!-- -->29 0x000000000614920a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x614920a)
#<!-- -->30 0x000000000451a5ed clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x451a5ed)
#<!-- -->31 0x0000000004828c2a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4828c2a)
#<!-- -->32 0x00000000047a535b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47a535b)
#<!-- -->33 0x0000000004918a43 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4918a43)
#<!-- -->34 0x0000000000db1801 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb1801)
#<!-- -->35 0x0000000000da9f5d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x000000000458c189 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x0000000003b2ec13 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b2ec13)
#<!-- -->38 0x000000000458c3a9 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x000000000454f7fd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x454f7fd)
#<!-- -->40 0x00000000045507c1 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45507c1)
#<!-- -->41 0x000000000455ab0c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x455ab0c)
#<!-- -->42 0x0000000000dae531 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdae531)
#<!-- -->43 0x0000000000c301d4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc301d4)
#<!-- -->44 0x000071aeece29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x000071aeece29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000da99f5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xda99f5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

https://godbolt.org/z/faxEK476f
</details>


---

### Comment 2 - zyn0217

@mizvekov 

---

### Comment 3 - shafik

@zyn0217 testing assertions is better, we should also copy in the assertion and assertion backtrace or searchability. Otherwise we will have a hard time finding duplicates and this will hamper our ability to write extension regression tests when we fix it.

godbolt: https://godbolt.org/z/qxbEenTv3

Assertion:

```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/SmallBitVector.h:455:
llvm::SmallBitVector::reference llvm::SmallBitVector::operator[](unsigned int):
Assertion `Idx < size() && "Out-of-bounds Bit access."' failed.
```

backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:10:10: instantiating function definition 'C<int, int>::f'
 #0 0x0000000003ef3f78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef3f78)
 #1 0x0000000003ef1c04 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef1c04)
 #2 0x0000000003e36738 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000792290842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007922908969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000792290842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007922908287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000079229082871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000792290839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000066139f5 (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66139f5)
#10 0x000000000701d91a MarkUsedTemplateParameters(clang::ASTContext&, clang::QualType, bool, unsigned int, llvm::SmallBitVector&) SemaTemplateDeduction.cpp:0:0
#11 0x000000000701e44c MarkUsedTemplateParameters(clang::ASTContext&, clang::TemplateArgument const&, bool, unsigned int, llvm::SmallBitVector&) SemaTemplateDeduction.cpp:0:0
#12 0x000000000701d898 MarkUsedTemplateParameters(clang::ASTContext&, clang::QualType, bool, unsigned int, llvm::SmallBitVector&) SemaTemplateDeduction.cpp:0:0
#13 0x000000000701d759 MarkUsedTemplateParameters(clang::ASTContext&, clang::QualType, bool, unsigned int, llvm::SmallBitVector&) SemaTemplateDeduction.cpp:0:0
#14 0x000000000705a3f3 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda0'(clang::QualType, unsigned int, bool)::operator()(clang::QualType, unsigned int, bool) const SemaTemplateDeduction.cpp:0:0
#15 0x000000000705b53d clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x705b53d)
#16 0x0000000006e869a2 AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#17 0x0000000006e872af clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e872af)
#18 0x0000000006e87387 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e87387)
#19 0x0000000006c6377a clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#20 0x0000000006c64667 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c64667)
#21 0x0000000006b503b5 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b503b5)
#22 0x000000000711163e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXUnresolvedConstructExpr(clang::CXXUnresolvedConstructExpr*) SemaTemplateInstantiate.cpp:0:0
#23 0x00000000070ebbfc clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#24 0x000000000712b9c7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#25 0x000000000712c25e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#26 0x0000000007135574 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7135574)
#27 0x0000000007195a97 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7195a97)
#28 0x00000000071938e6 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71938e6)
#29 0x00000000066ef842 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#30 0x00000000066f00e2 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f00e2)
#31 0x000000000656006b clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x656006b)
#32 0x000000000655236a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x655236a)
#33 0x0000000004891e38 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4891e38)
#34 0x0000000004b84f85 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b84f85)
#35 0x0000000004b03d4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b03d4e)
#36 0x0000000004c73e7e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c73e7e)
#37 0x0000000000d9f3af cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f3af)
#38 0x0000000000d963ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#39 0x00000000048f84f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#40 0x0000000003e36bd4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e36bd4)
#41 0x00000000048f8b0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#42 0x00000000048baf6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48baf6d)
#43 0x00000000048bbffe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48bbffe)
#44 0x00000000048c3e55 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48c3e55)
#45 0x0000000000d9c0f8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9c0f8)
#46 0x0000000000c236a4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc236a4)
#47 0x0000792290829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#48 0x0000792290829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#49 0x0000000000d95e65 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd95e65)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

