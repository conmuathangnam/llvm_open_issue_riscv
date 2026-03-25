# [Clang] Crash on invalid initializer naming an overloaded member function

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/163062

## Body

```c++
template <class T>
class span {
public:
    template <class It, class Sentinel>
    constexpr span(It, Sentinel) {}

    template <class Rng>
    constexpr span(Rng&&) {}
};

template <class It, class End>
span(It, End) -> span<int>;

class vector {
public:
    constexpr int *begin() noexcept { return {}; }
    constexpr int const *begin() const noexcept { return {}; }
};

vector v;
span s(v.begin);
```

See https://compiler-explorer.com/z/ne7MWvbfd.

Assertion:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:3990:
clang::QualType clang::ASTContext::getLValueReferenceType(clang::QualType, bool) const:
Assertion `(!T->isPlaceholderType() || T->isSpecificPlaceholderType(BuiltinType::UnknownAny)) && "Unresolved placeholder type"' failed.
```

<details>
<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:21:16: current parser token ';'
 #0 0x00000000041eba08 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41eba08)
 #1 0x00000000041e8e34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e8e34)
 #2 0x000000000412d788 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007c2c29e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007c2c29e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007c2c29e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007c2c29e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007c2c29e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007c2c29e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000077a8e43 clang::ASTContext::getLValueReferenceType(clang::QualType, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77a8e43)
#10 0x000000000737e41c AdjustFunctionParmAndArgTypesForDeduction(clang::Sema&, clang::TemplateParameterList*, unsigned int, clang::QualType&, clang::QualType&, clang::Expr::Classification, clang::Expr*, unsigned int&, clang::TemplateSpecCandidateSet*) (.isra.0) SemaTemplateDeduction.cpp:0:0
#11 0x000000000737e5c8 DeduceTemplateArgumentsFromCallArgument(clang::Sema&, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg>&, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#12 0x000000000737f5ee clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::'lambda0'(clang::QualType, unsigned int, bool)::operator()(clang::QualType, unsigned int, bool) const SemaTemplateDeduction.cpp:0:0
#13 0x000000000738060d clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x738060d)
#14 0x00000000071d18ab AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#15 0x00000000071d239f clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71d239f)
#16 0x00000000071d2573 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71d2573)
#17 0x0000000006fa599c clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#18 0x0000000006fa66c7 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa66c7)
#19 0x0000000006c93935 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c93935)
#20 0x0000000006c98b77 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c98b77)
#21 0x0000000006cc70a2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc70a2)
#22 0x0000000006913939 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6913939)
#23 0x0000000006920daa clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6920daa)
#24 0x00000000068de4ee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68de4ee)
#25 0x00000000068dec8f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68dec8f)
#26 0x00000000068e6aca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e6aca)
#27 0x00000000068e7a65 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e7a65)
#28 0x00000000068c8eda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c8eda)
#29 0x0000000004b30a48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b30a48)
#30 0x0000000004e1c8d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e1c8d5)
#31 0x0000000004d9df1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d9df1e)
#32 0x0000000004f13b8d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f13b8d)
#33 0x0000000000dc14d0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc14d0)
#34 0x0000000000db803a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#35 0x0000000000db81bd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#36 0x0000000004b99bd9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x000000000412dc24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412dc24)
#38 0x0000000004b9a1ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x0000000004b5b502 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5b502)
#40 0x0000000004b5c4ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5c4ae)
#41 0x0000000004b63be5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b63be5)
#42 0x0000000000dbd9f9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbd9f9)
#43 0x0000000000c6d754 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6d754)
#44 0x00007c2c29e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x00007c2c29e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000db7ad5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb7ad5)
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
```c++
template &lt;class T&gt;
class span {
public:
    template &lt;class It, class Sentinel&gt;
    constexpr span(It, Sentinel) {}

    template &lt;class Rng&gt;
    constexpr span(Rng&amp;&amp;) {}
};

template &lt;class It, class End&gt;
span(It, End) -&gt; span&lt;int&gt;;

class vector {
public:
    constexpr int *begin() noexcept { return {}; }
    constexpr int const *begin() const noexcept { return {}; }
};

vector v;
span s(v.begin);
```

See https://compiler-explorer.com/z/ne7MWvbfd.

Assertion:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:3990:
clang::QualType clang::ASTContext::getLValueReferenceType(clang::QualType, bool) const:
Assertion `(!T-&gt;isPlaceholderType() || T-&gt;isSpecificPlaceholderType(BuiltinType::UnknownAny)) &amp;&amp; "Unresolved placeholder type"' failed.
```

&lt;details&gt;
&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:21:16: current parser token ';'
 #<!-- -->0 0x00000000041eba08 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41eba08)
 #<!-- -->1 0x00000000041e8e34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e8e34)
 #<!-- -->2 0x000000000412d788 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007c2c29e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007c2c29e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007c2c29e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007c2c29e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007c2c29e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007c2c29e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000077a8e43 clang::ASTContext::getLValueReferenceType(clang::QualType, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x77a8e43)
#<!-- -->10 0x000000000737e41c AdjustFunctionParmAndArgTypesForDeduction(clang::Sema&amp;, clang::TemplateParameterList*, unsigned int, clang::QualType&amp;, clang::QualType&amp;, clang::Expr::Classification, clang::Expr*, unsigned int&amp;, clang::TemplateSpecCandidateSet*) (.isra.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->11 0x000000000737e5c8 DeduceTemplateArgumentsFromCallArgument(clang::Sema&amp;, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt;&amp;, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#<!-- -->12 0x000000000737f5ee clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::'lambda0'(clang::QualType, unsigned int, bool)::operator()(clang::QualType, unsigned int, bool) const SemaTemplateDeduction.cpp:0:0
#<!-- -->13 0x000000000738060d clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x738060d)
#<!-- -->14 0x00000000071d18ab AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) SemaOverload.cpp:0:0
#<!-- -->15 0x00000000071d239f clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71d239f)
#<!-- -->16 0x00000000071d2573 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71d2573)
#<!-- -->17 0x0000000006fa599c clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#<!-- -->18 0x0000000006fa66c7 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa66c7)
#<!-- -->19 0x0000000006c93935 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c93935)
#<!-- -->20 0x0000000006c98b77 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c98b77)
#<!-- -->21 0x0000000006cc70a2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cc70a2)
#<!-- -->22 0x0000000006913939 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6913939)
#<!-- -->23 0x0000000006920daa clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6920daa)
#<!-- -->24 0x00000000068de4ee clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68de4ee)
#<!-- -->25 0x00000000068dec8f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68dec8f)
#<!-- -->26 0x00000000068e6aca clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e6aca)
#<!-- -->27 0x00000000068e7a65 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e7a65)
#<!-- -->28 0x00000000068c8eda clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c8eda)
#<!-- -->29 0x0000000004b30a48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b30a48)
#<!-- -->30 0x0000000004e1c8d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e1c8d5)
#<!-- -->31 0x0000000004d9df1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d9df1e)
#<!-- -->32 0x0000000004f13b8d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f13b8d)
#<!-- -->33 0x0000000000dc14d0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc14d0)
#<!-- -->34 0x0000000000db803a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->35 0x0000000000db81bd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000004b99bd9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x000000000412dc24 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412dc24)
#<!-- -->38 0x0000000004b9a1ef clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x0000000004b5b502 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5b502)
#<!-- -->40 0x0000000004b5c4ae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5c4ae)
#<!-- -->41 0x0000000004b63be5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b63be5)
#<!-- -->42 0x0000000000dbd9f9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbd9f9)
#<!-- -->43 0x0000000000c6d754 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6d754)
#<!-- -->44 0x00007c2c29e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x00007c2c29e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000db7ad5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb7ad5)
```

&lt;/details&gt;
</details>


---

