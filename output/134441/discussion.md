# [clang][ice-on-invalid] `TemplateArgument::getPackAsArray()` assertion `'getKind() == Pack'`

**Author:** ericniebler
**URL:** https://github.com/llvm/llvm-project/issues/134441

## Body

The following C++20 code causes clang trunk to ICE:

```c++
template <class Int>
struct wrap {
  constexpr wrap(Int i): value(i) {}
  Int value;
};

template <class T, wrap<T>... Is>
struct extents {
};

template <int... Is>
using Shape = extents<int, Is...>;

template <class T, Shape S>
void test() {}

int main() {
  constexpr Shape<256> s;
  auto fn = test<int, s>;
}
```

result:

```
clang++: /root/llvm-project/clang/include/clang/AST/TemplateBase.h:442: llvm::ArrayRef<clang::TemplateArgument> clang::TemplateArgument::getPackAsArray() const: Assertion `getKind() == Pack' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
```

repro:
https://godbolt.org/z/qsn8bcn3e


<details>
<summary>Stack trace</summary>

```
clang++: /root/llvm-project/clang/include/clang/AST/TemplateBase.h:442: llvm::ArrayRef<clang::TemplateArgument> clang::TemplateArgument::getPackAsArray() const: Assertion `getKind() == Pack' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:19:25: current parser token ';'
2.	<source>:17:12: parsing function body 'main'
3.	<source>:17:12: in compound statement ('{}')
 #0 0x0000000003eaa8c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eaa8c8)
 #1 0x0000000003ea8554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ea8554)
 #2 0x0000000003ded098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072df20c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072df20c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072df20c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072df20c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072df20c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072df20c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000711fd46 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, clang::MultiLevelTemplateArgumentList const&, bool&, bool&, clang::UnsignedOrNone&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x711fd46)
#10 0x000000000707d75a bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLocInventIterator<(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*>>(clang::TemplateArgumentLocInventIterator<(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*>, clang::TemplateArgumentLocInventIterator<(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*>, clang::TemplateArgumentListInfo&, bool) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000007081123 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateSpecializationType(clang::TypeLocBuilder&, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000706f23b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#13 0x0000000007074e7a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#14 0x0000000007074f9c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000706e757 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000070968e9 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70968e9)
#17 0x00000000070b135f clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl<clang::ParmVarDecl*>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b135f)
#18 0x000000000710e3e1 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710e3e1)
#19 0x000000000710f913 clang::TemplateDeclInstantiator::VisitCXXDeductionGuideDecl(clang::CXXDeductionGuideDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710f913)
#20 0x0000000007112b04 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#21 0x0000000007d948c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d948c1)
#22 0x00000000070b111a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b111a)
#23 0x00000000070b2d35 clang::Sema::InstantiateFunctionDeclaration(clang::FunctionTemplateDecl*, clang::TemplateArgumentList const*, clang::SourceLocation, clang::Sema::CodeSynthesisContext::SynthesisKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b2d35)
#24 0x0000000006ff99ad (anonymous namespace)::BuildDeductionGuideForTypeAlias(clang::Sema&, clang::TypeAliasTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation) SemaTemplateDeductionGuide.cpp:0:0
#25 0x000000000703e135 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x703e135)
#26 0x0000000006c2374f LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#27 0x0000000006c0e6b2 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0e6b2)
#28 0x0000000006bec67e clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bec67e)
#29 0x0000000006ef8ff8 clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&, clang::TemplateArgument&, bool, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ef8ff8)
#30 0x0000000006efd296 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6efd296)
#31 0x0000000006f01d41 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f01d41)
#32 0x0000000006f923f7 clang::Sema::SubstituteExplicitTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, llvm::SmallVectorImpl<clang::QualType>&, clang::QualType*, clang::sema::TemplateDeductionInfo&) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#33 0x0000000006f93261 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool)::'lambda'()>(long) SemaTemplateDeduction.cpp:0:0
#34 0x0000000007d948c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d948c1)
#35 0x0000000006fc5d2e clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc5d2e)
#36 0x0000000006fc64d7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc64d7)
#37 0x0000000006df5fbb clang::Sema::ResolveSingleFunctionTemplateSpecialization(clang::OverloadExpr*, bool, clang::DeclAccessPair*, clang::TemplateSpecCandidateSet*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df5fbb)
#38 0x0000000006fc743f ResolveOverloadForDeduction(clang::Sema&, clang::TemplateParameterList*, clang::Expr*, clang::QualType, bool, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#39 0x0000000006fc7c1f DeduceTemplateArgumentsFromCallArgument(clang::Sema&, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg>&, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#40 0x0000000006fe4b62 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fe4b62)
#41 0x000000000686b411 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x686b411)
#42 0x00000000068703f7 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68703f7)
#43 0x00000000068913a2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68913a2)
#44 0x0000000006518f0a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6518f0a)
#45 0x00000000065280b9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65280b9)
#46 0x0000000006530b27 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6530b27)
#47 0x0000000006530fe9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6530fe9)
#48 0x00000000065e0945 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e0945)
#49 0x00000000065e164d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e164d)
#50 0x00000000065e92e3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e92e3)
#51 0x00000000065e9aaa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e9aaa)
#52 0x00000000064f34d3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64f34d3)
#53 0x0000000006528abd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6528abd)
#54 0x00000000064e6bae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e6bae)
#55 0x00000000064e7369 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e7369)
#56 0x00000000064ef173 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ef173)
#57 0x00000000064f008d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64f008d)
#58 0x00000000064e201a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e201a)
#59 0x0000000004843468 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4843468)
#60 0x0000000004b31f05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b31f05)
#61 0x0000000004ab3d9e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ab3d9e)
#62 0x0000000004c2211e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2211e)
#63 0x0000000000d5fb1f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5fb1f)
#64 0x0000000000d572da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#65 0x00000000048a8db9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#66 0x0000000003ded534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ded534)
#67 0x00000000048a93cf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#68 0x000000000486be6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486be6d)
#69 0x000000000486cefe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486cefe)
#70 0x0000000004874605 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4874605)
#71 0x0000000000d5c918 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5c918)
#72 0x0000000000c22cf4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc22cf4)
#73 0x000072df20c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#74 0x000072df20c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#75 0x0000000000d56d85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd56d85)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

</details>


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Eric Niebler (ericniebler)

<details>
The following C++20 code causes clang trunk to ICE:

```c++
template &lt;class Int&gt;
struct wrap {
  constexpr wrap(Int i): value(i) {}
  Int value;
};

template &lt;class T, wrap&lt;T&gt;... Is&gt;
struct extents {
};

template &lt;int... Is&gt;
using Shape = extents&lt;int, Is...&gt;;

template &lt;class T, Shape S&gt;
void test() {}

int main() {
  constexpr Shape&lt;256&gt; s;
  auto fn = test&lt;int, s&gt;;
}
```

result:

```
clang++: /root/llvm-project/clang/include/clang/AST/TemplateBase.h:442: llvm::ArrayRef&lt;clang::TemplateArgument&gt; clang::TemplateArgument::getPackAsArray() const: Assertion `getKind() == Pack' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
```

repro:
https://godbolt.org/z/qsn8bcn3e


&lt;details&gt;
&lt;summary&gt;Stack trace&lt;/summary&gt;

```
clang++: /root/llvm-project/clang/include/clang/AST/TemplateBase.h:442: llvm::ArrayRef&lt;clang::TemplateArgument&gt; clang::TemplateArgument::getPackAsArray() const: Assertion `getKind() == Pack' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:19:25: current parser token ';'
2.	&lt;source&gt;:17:12: parsing function body 'main'
3.	&lt;source&gt;:17:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003eaa8c8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eaa8c8)
 #<!-- -->1 0x0000000003ea8554 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ea8554)
 #<!-- -->2 0x0000000003ded098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072df20c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072df20c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072df20c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072df20c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072df20c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072df20c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000711fd46 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef&lt;std::pair&lt;llvm::PointerUnion&lt;clang::TemplateTypeParmType const*, clang::NamedDecl*&gt;, clang::SourceLocation&gt;&gt;, clang::MultiLevelTemplateArgumentList const&amp;, bool&amp;, bool&amp;, clang::UnsignedOrNone&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x711fd46)
#<!-- -->10 0x000000000707d75a bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLocInventIterator&lt;(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*&gt;&gt;(clang::TemplateArgumentLocInventIterator&lt;(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*&gt;, clang::TemplateArgumentLocInventIterator&lt;(anonymous namespace)::TemplateInstantiator, clang::TemplateArgument const*&gt;, clang::TemplateArgumentListInfo&amp;, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000007081123 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateSpecializationType(clang::TypeLocBuilder&amp;, clang::TemplateSpecializationTypeLoc, clang::TemplateName) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x000000000706f23b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x0000000007074e7a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x0000000007074f9c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x000000000706e757 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformType(clang::TypeLocBuilder&amp;, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x00000000070968e9 clang::Sema::SubstFunctionDeclType(clang::TypeSourceInfo*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceLocation, clang::DeclarationName, clang::CXXRecordDecl*, clang::Qualifiers, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70968e9)
#<!-- -->17 0x00000000070b135f clang::TemplateDeclInstantiator::SubstFunctionType(clang::FunctionDecl*, llvm::SmallVectorImpl&lt;clang::ParmVarDecl*&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b135f)
#<!-- -->18 0x000000000710e3e1 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710e3e1)
#<!-- -->19 0x000000000710f913 clang::TemplateDeclInstantiator::VisitCXXDeductionGuideDecl(clang::CXXDeductionGuideDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x710f913)
#<!-- -->20 0x0000000007112b04 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->21 0x0000000007d948c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d948c1)
#<!-- -->22 0x00000000070b111a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b111a)
#<!-- -->23 0x00000000070b2d35 clang::Sema::InstantiateFunctionDeclaration(clang::FunctionTemplateDecl*, clang::TemplateArgumentList const*, clang::SourceLocation, clang::Sema::CodeSynthesisContext::SynthesisKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b2d35)
#<!-- -->24 0x0000000006ff99ad (anonymous namespace)::BuildDeductionGuideForTypeAlias(clang::Sema&amp;, clang::TypeAliasTemplateDecl*, clang::FunctionTemplateDecl*, clang::SourceLocation) SemaTemplateDeductionGuide.cpp:0:0
#<!-- -->25 0x000000000703e135 clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x703e135)
#<!-- -->26 0x0000000006c2374f LookupDirect(clang::Sema&amp;, clang::LookupResult&amp;, clang::DeclContext const*) SemaLookup.cpp:0:0
#<!-- -->27 0x0000000006c0e6b2 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c0e6b2)
#<!-- -->28 0x0000000006bec67e clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bec67e)
#<!-- -->29 0x0000000006ef8ff8 clang::Sema::CheckTemplateArgument(clang::NonTypeTemplateParmDecl*, clang::QualType, clang::Expr*, clang::TemplateArgument&amp;, clang::TemplateArgument&amp;, bool, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ef8ff8)
#<!-- -->30 0x0000000006efd296 clang::Sema::CheckTemplateArgument(clang::NamedDecl*, clang::TemplateArgumentLoc&amp;, clang::NamedDecl*, clang::SourceLocation, clang::SourceLocation, unsigned int, clang::Sema::CheckTemplateArgumentInfo&amp;, clang::Sema::CheckTemplateArgumentKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6efd296)
#<!-- -->31 0x0000000006f01d41 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::DefaultArguments const&amp;, bool, clang::Sema::CheckTemplateArgumentInfo&amp;, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f01d41)
#<!-- -->32 0x0000000006f923f7 clang::Sema::SubstituteExplicitTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::QualType&gt;&amp;, clang::QualType*, clang::sema::TemplateDeductionInfo&amp;) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#<!-- -->33 0x0000000006f93261 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool)::'lambda'()&gt;(long) SemaTemplateDeduction.cpp:0:0
#<!-- -->34 0x0000000007d948c1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7d948c1)
#<!-- -->35 0x0000000006fc5d2e clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc5d2e)
#<!-- -->36 0x0000000006fc64d7 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc64d7)
#<!-- -->37 0x0000000006df5fbb clang::Sema::ResolveSingleFunctionTemplateSpecialization(clang::OverloadExpr*, bool, clang::DeclAccessPair*, clang::TemplateSpecCandidateSet*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df5fbb)
#<!-- -->38 0x0000000006fc743f ResolveOverloadForDeduction(clang::Sema&amp;, clang::TemplateParameterList*, clang::Expr*, clang::QualType, bool, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#<!-- -->39 0x0000000006fc7c1f DeduceTemplateArgumentsFromCallArgument(clang::Sema&amp;, clang::TemplateParameterList*, unsigned int, clang::QualType, clang::QualType, clang::Expr::Classification, clang::Expr*, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt;&amp;, bool, unsigned int, unsigned int, clang::TemplateSpecCandidateSet*) SemaTemplateDeduction.cpp:0:0
#<!-- -->40 0x0000000006fe4b62 clang::Sema::DeduceAutoType(clang::TypeLoc, clang::Expr*, clang::QualType&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::TemplateSpecCandidateSet*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fe4b62)
#<!-- -->41 0x000000000686b411 clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x686b411)
#<!-- -->42 0x00000000068703f7 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68703f7)
#<!-- -->43 0x00000000068913a2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68913a2)
#<!-- -->44 0x0000000006518f0a clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6518f0a)
#<!-- -->45 0x00000000065280b9 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65280b9)
#<!-- -->46 0x0000000006530b27 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6530b27)
#<!-- -->47 0x0000000006530fe9 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6530fe9)
#<!-- -->48 0x00000000065e0945 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e0945)
#<!-- -->49 0x00000000065e164d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e164d)
#<!-- -->50 0x00000000065e92e3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e92e3)
#<!-- -->51 0x00000000065e9aaa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e9aaa)
#<!-- -->52 0x00000000064f34d3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64f34d3)
#<!-- -->53 0x0000000006528abd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6528abd)
#<!-- -->54 0x00000000064e6bae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e6bae)
#<!-- -->55 0x00000000064e7369 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e7369)
#<!-- -->56 0x00000000064ef173 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64ef173)
#<!-- -->57 0x00000000064f008d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64f008d)
#<!-- -->58 0x00000000064e201a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e201a)
#<!-- -->59 0x0000000004843468 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4843468)
#<!-- -->60 0x0000000004b31f05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b31f05)
#<!-- -->61 0x0000000004ab3d9e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ab3d9e)
#<!-- -->62 0x0000000004c2211e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2211e)
#<!-- -->63 0x0000000000d5fb1f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5fb1f)
#<!-- -->64 0x0000000000d572da ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->65 0x00000000048a8db9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->66 0x0000000003ded534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ded534)
#<!-- -->67 0x00000000048a93cf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->68 0x000000000486be6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486be6d)
#<!-- -->69 0x000000000486cefe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486cefe)
#<!-- -->70 0x0000000004874605 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4874605)
#<!-- -->71 0x0000000000d5c918 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5c918)
#<!-- -->72 0x0000000000c22cf4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc22cf4)
#<!-- -->73 0x000072df20c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->74 0x000072df20c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->75 0x0000000000d56d85 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd56d85)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

&lt;/details&gt;

</details>


---

