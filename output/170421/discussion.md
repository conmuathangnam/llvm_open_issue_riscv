# Assertion `isHandleInSync() && "invalid iterator access!"` with PCH and -ftime-trace

**Author:** rkirsling
**URL:** https://github.com/llvm/llvm-project/issues/170421
**Status:** Closed
**Labels:** clang:frontend, clang:modules, crash-on-valid, clang:PCH
**Closed Date:** 2025-12-18T10:06:31Z

## Body

### Source

`header.h`
```cpp
#pragma once

inline namespace {
template<typename T> T g(T v) { return v; }
template<typename T> T f(T v) { return g(v); }
template<typename T> T g();
}
```
`main.cpp`
```cpp
#include "header.h"

int x;
void i() { f(x); }
```

### Commands

```
λ bin/clang -x c++-header -c header.h -o pch.pch
λ bin/clang -ftime-trace -c main.cpp -include-pch pch.pch
```

### Crash

```
Assertion failed: (isHandleInSync() && "invalid iterator access!"), function operator*, file DenseMap.h, line 1211.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.  Program arguments: bin/clang -ftime-trace -c main.cpp -include-pch pch.pch
1.  <eof> parser at end of file
2.  .../llvm-project/build/header.h:5:25: instantiating function definition 'f<int>'
 #0 0x00000001070494ec llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (.../llvm-project/build/bin/clang-21+0x102c714ec)
 #1 0x000000010704729c llvm::sys::RunSignalHandlers() (.../llvm-project/build/bin/clang-21+0x102c6f29c)
 #2 0x0000000107048ab4 llvm::sys::CleanupOnSignal(unsigned long) (.../llvm-project/build/bin/clang-21+0x102c70ab4)
 #3 0x0000000106f8b7f8 CrashRecoverySignalHandler(int) (.../llvm-project/build/bin/clang-21+0x102bb37f8)
 #4 0x00000001947dd6a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #5 0x00000001947a388c (/usr/lib/system/libsystem_pthread.dylib+0x18047388c)
 #6 0x00000001946aca3c (/usr/lib/system/libsystem_c.dylib+0x18037ca3c)
 #7 0x00000001946abc70 (/usr/lib/system/libsystem_c.dylib+0x18037bc70)
 #8 0x000000010aef9f7c clang::serialization::MultiOnDiskHashTable<clang::serialization::reader::LazySpecializationInfoLookupTrait>::find(unsigned int const&) (.cold.1) (.../llvm-project/build/bin/clang-21+0x106b21f7c)
 #9 0x0000000107d6b0dc clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap<clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo<clang::Decl const*, void>, llvm::detail::DenseMapPair<clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable>>&, clang::Decl const*, llvm::ArrayRef<clang::TemplateArgument>) (.../llvm-project/build/bin/clang-21+0x1039930dc)
#10 0x0000000107d6b56c non-virtual thunk to clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef<clang::TemplateArgument>) (.../llvm-project/build/bin/clang-21+0x10399356c)
#11 0x000000010965ddec clang::FunctionTemplateDecl::findSpecialization(llvm::ArrayRef<clang::TemplateArgument>, void*&) (.../llvm-project/build/bin/clang-21+0x105285dec)
#12 0x00000001091da9d8 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (.../llvm-project/build/bin/clang-21+0x104e029d8)
#13 0x000000010921d784 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::$_3>(long) (.../llvm-project/build/bin/clang-21+0x104e45784)
#14 0x00000001074cc7dc clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (.../llvm-project/build/bin/clang-21+0x1030f47dc)
#15 0x00000001091e1b48 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (.../llvm-project/build/bin/clang-21+0x104e09b48)
#16 0x00000001090ba5f8 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, bool, llvm::function_ref<bool (bool)>) (.../llvm-project/build/bin/clang-21+0x104ce25f8)
#17 0x0000000109104380 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>)::$_5>(long) (.../llvm-project/build/bin/clang-21+0x104d2c380)
#18 0x00000001074cc7dc clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (.../llvm-project/build/bin/clang-21+0x1030f47dc)
#19 0x00000001090bd054 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>, bool)>) (.../llvm-project/build/bin/clang-21+0x104ce5054)
#20 0x0000000108fca334 AddTemplateOverloadCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.../llvm-project/build/bin/clang-21+0x104bf2334)
#21 0x0000000108fd4b84 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (.../llvm-project/build/bin/clang-21+0x104bfcb84)
#22 0x0000000108fc5e3c clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (.../llvm-project/build/bin/clang-21+0x104bede3c)
#23 0x0000000108fdb448 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (.../llvm-project/build/bin/clang-21+0x104c03448)
#24 0x0000000108d0de88 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (.../llvm-project/build/bin/clang-21+0x104935e88)
#25 0x0000000108d20a20 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (.../llvm-project/build/bin/clang-21+0x104948a20)
#26 0x00000001091986b0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) (.../llvm-project/build/bin/clang-21+0x104dc06b0)
#27 0x00000001091b2040 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) (.../llvm-project/build/bin/clang-21+0x104dda040)
#28 0x000000010919e498 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) (.../llvm-project/build/bin/clang-21+0x104dc6498)
#29 0x000000010914f7e0 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (.../llvm-project/build/bin/clang-21+0x104d777e0)
#30 0x00000001091e3618 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (.../llvm-project/build/bin/clang-21+0x104e0b618)

#31 0x00000001091e5e14 clang::Sema::PerformPendingInstantiations(bool, bool) (.../llvm-project/build/bin/clang-21+0x104e0de14)
#32 0x0000000108abc1d4 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.../llvm-project/build/bin/clang-21+0x1046e41d4)
#33 0x0000000108abc620 clang::Sema::ActOnEndOfTranslationUnit() (.../llvm-project/build/bin/clang-21+0x1046e4620)
#34 0x0000000108a265a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (.../llvm-project/build/bin/clang-21+0x10464e5a8)
#35 0x000000010895d2e0 clang::ParseAST(clang::Sema&, bool, bool) (.../llvm-project/build/bin/clang-21+0x1045852e0)
#36 0x0000000107c916f0 clang::FrontendAction::Execute() (.../llvm-project/build/bin/clang-21+0x1038b96f0)
#37 0x0000000107c17b4c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (.../llvm-project/build/bin/clang-21+0x10383fb4c)
#38 0x0000000107d30600 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (.../llvm-project/build/bin/clang-21+0x103958600)
#39 0x00000001043e11b4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (.../llvm-project/build/bin/clang-21+0x1000091b4)
#40 0x00000001043de660 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) (.../llvm-project/build/bin/clang-21+0x100006660)
#41 0x00000001043dfdf0 int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::$_2>(long, llvm::SmallVectorImpl<char const*>&) (.../llvm-project/build/bin/clang-21+0x100007df0)
#42 0x0000000107a5986c void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const::$_1>(long) (.../llvm-project/build/bin/clang-21+0x10368186c)
#43 0x0000000106f8b500 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (.../llvm-project/build/bin/clang-21+0x102bb3500)
#44 0x0000000107a590d8 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::__1::optional<llvm::StringRef>>, std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>>*, bool*) const (.../llvm-project/build/bin/clang-21+0x1036810d8)
#45 0x0000000107a2141c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (.../llvm-project/build/bin/clang-21+0x10364941c)
#46 0x0000000107a215e4 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&, bool) const (.../llvm-project/build/bin/clang-21+0x1036495e4)
#47 0x0000000107a3bb88 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::__1::pair<int, clang::driver::Command const*>>&) (.../llvm-project/build/bin/clang-21+0x103663b88)
#48 0x00000001043dd9a4 clang_main(int, char**, llvm::ToolContext const&) (.../llvm-project/build/bin/clang-21+0x1000059a4)
#49 0x00000001043ea480 main (.../llvm-project/build/bin/clang-21+0x100012480)
#50 0x0000000194402b98
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 93ebe63f2e7a252038bde01a4399c14e0123cdac)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: .../llvm-project/build/bin
Build config: +assertions
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ross Kirsling (rkirsling)

<details>
### Source

`header.h`
```cpp
#pragma once

inline namespace {
template&lt;typename T&gt; T g(T v) { return v; }
template&lt;typename T&gt; T f(T v) { return g(v); }
template&lt;typename T&gt; T g();
}
```
`main.cpp`
```cpp
#include "header.h"

int x;
void i() { f(x); }
```

### Commands

```
λ bin/clang -x c++-header -c header.h -o pch.pch
λ bin/clang -ftime-trace -c main.cpp -include-pch pch.pch
```

### Crash

```
Assertion failed: (isHandleInSync() &amp;&amp; "invalid iterator access!"), function operator*, file DenseMap.h, line 1211.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.  Program arguments: bin/clang -ftime-trace -c main.cpp -include-pch pch.pch
1.  &lt;eof&gt; parser at end of file
2.  .../llvm-project/build/header.h:5:25: instantiating function definition 'f&lt;int&gt;'
 #<!-- -->0 0x00000001070494ec llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (.../llvm-project/build/bin/clang-21+0x102c714ec)
 #<!-- -->1 0x000000010704729c llvm::sys::RunSignalHandlers() (.../llvm-project/build/bin/clang-21+0x102c6f29c)
 #<!-- -->2 0x0000000107048ab4 llvm::sys::CleanupOnSignal(unsigned long) (.../llvm-project/build/bin/clang-21+0x102c70ab4)
 #<!-- -->3 0x0000000106f8b7f8 CrashRecoverySignalHandler(int) (.../llvm-project/build/bin/clang-21+0x102bb37f8)
 #<!-- -->4 0x00000001947dd6a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->5 0x00000001947a388c (/usr/lib/system/libsystem_pthread.dylib+0x18047388c)
 #<!-- -->6 0x00000001946aca3c (/usr/lib/system/libsystem_c.dylib+0x18037ca3c)
 #<!-- -->7 0x00000001946abc70 (/usr/lib/system/libsystem_c.dylib+0x18037bc70)
 #<!-- -->8 0x000000010aef9f7c clang::serialization::MultiOnDiskHashTable&lt;clang::serialization::reader::LazySpecializationInfoLookupTrait&gt;::find(unsigned int const&amp;) (.cold.1) (.../llvm-project/build/bin/clang-21+0x106b21f7c)
 #<!-- -->9 0x0000000107d6b0dc clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable&gt;&gt;&amp;, clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (.../llvm-project/build/bin/clang-21+0x1039930dc)
#<!-- -->10 0x0000000107d6b56c non-virtual thunk to clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (.../llvm-project/build/bin/clang-21+0x10399356c)
#<!-- -->11 0x000000010965ddec clang::FunctionTemplateDecl::findSpecialization(llvm::ArrayRef&lt;clang::TemplateArgument&gt;, void*&amp;) (.../llvm-project/build/bin/clang-21+0x105285dec)
#<!-- -->12 0x00000001091da9d8 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (.../llvm-project/build/bin/clang-21+0x104e029d8)
#<!-- -->13 0x000000010921d784 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::$_3&gt;(long) (.../llvm-project/build/bin/clang-21+0x104e45784)
#<!-- -->14 0x00000001074cc7dc clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (.../llvm-project/build/bin/clang-21+0x1030f47dc)
#<!-- -->15 0x00000001091e1b48 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (.../llvm-project/build/bin/clang-21+0x104e09b48)
#<!-- -->16 0x00000001090ba5f8 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (.../llvm-project/build/bin/clang-21+0x104ce25f8)
#<!-- -->17 0x0000000109104380 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::$_5&gt;(long) (.../llvm-project/build/bin/clang-21+0x104d2c380)
#<!-- -->18 0x00000001074cc7dc clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (.../llvm-project/build/bin/clang-21+0x1030f47dc)
#<!-- -->19 0x00000001090bd054 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (.../llvm-project/build/bin/clang-21+0x104ce5054)
#<!-- -->20 0x0000000108fca334 AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.../llvm-project/build/bin/clang-21+0x104bf2334)
#<!-- -->21 0x0000000108fd4b84 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (.../llvm-project/build/bin/clang-21+0x104bfcb84)
#<!-- -->22 0x0000000108fc5e3c clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (.../llvm-project/build/bin/clang-21+0x104bede3c)
#<!-- -->23 0x0000000108fdb448 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (.../llvm-project/build/bin/clang-21+0x104c03448)
#<!-- -->24 0x0000000108d0de88 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (.../llvm-project/build/bin/clang-21+0x104935e88)
#<!-- -->25 0x0000000108d20a20 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (.../llvm-project/build/bin/clang-21+0x104948a20)
#<!-- -->26 0x00000001091986b0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) (.../llvm-project/build/bin/clang-21+0x104dc06b0)
#<!-- -->27 0x00000001091b2040 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformReturnStmt(clang::ReturnStmt*) (.../llvm-project/build/bin/clang-21+0x104dda040)
#<!-- -->28 0x000000010919e498 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) (.../llvm-project/build/bin/clang-21+0x104dc6498)
#<!-- -->29 0x000000010914f7e0 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (.../llvm-project/build/bin/clang-21+0x104d777e0)
#<!-- -->30 0x00000001091e3618 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (.../llvm-project/build/bin/clang-21+0x104e0b618)

#<!-- -->31 0x00000001091e5e14 clang::Sema::PerformPendingInstantiations(bool, bool) (.../llvm-project/build/bin/clang-21+0x104e0de14)
#<!-- -->32 0x0000000108abc1d4 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.../llvm-project/build/bin/clang-21+0x1046e41d4)
#<!-- -->33 0x0000000108abc620 clang::Sema::ActOnEndOfTranslationUnit() (.../llvm-project/build/bin/clang-21+0x1046e4620)
#<!-- -->34 0x0000000108a265a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (.../llvm-project/build/bin/clang-21+0x10464e5a8)
#<!-- -->35 0x000000010895d2e0 clang::ParseAST(clang::Sema&amp;, bool, bool) (.../llvm-project/build/bin/clang-21+0x1045852e0)
#<!-- -->36 0x0000000107c916f0 clang::FrontendAction::Execute() (.../llvm-project/build/bin/clang-21+0x1038b96f0)
#<!-- -->37 0x0000000107c17b4c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (.../llvm-project/build/bin/clang-21+0x10383fb4c)
#<!-- -->38 0x0000000107d30600 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (.../llvm-project/build/bin/clang-21+0x103958600)
#<!-- -->39 0x00000001043e11b4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (.../llvm-project/build/bin/clang-21+0x1000091b4)
#<!-- -->40 0x00000001043de660 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) (.../llvm-project/build/bin/clang-21+0x100006660)
#<!-- -->41 0x00000001043dfdf0 int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_2&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) (.../llvm-project/build/bin/clang-21+0x100007df0)
#<!-- -->42 0x0000000107a5986c void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_1&gt;(long) (.../llvm-project/build/bin/clang-21+0x10368186c)
#<!-- -->43 0x0000000106f8b500 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (.../llvm-project/build/bin/clang-21+0x102bb3500)
#<!-- -->44 0x0000000107a590d8 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (.../llvm-project/build/bin/clang-21+0x1036810d8)
#<!-- -->45 0x0000000107a2141c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (.../llvm-project/build/bin/clang-21+0x10364941c)
#<!-- -->46 0x0000000107a215e4 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (.../llvm-project/build/bin/clang-21+0x1036495e4)
#<!-- -->47 0x0000000107a3bb88 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (.../llvm-project/build/bin/clang-21+0x103663b88)
#<!-- -->48 0x00000001043dd9a4 clang_main(int, char**, llvm::ToolContext const&amp;) (.../llvm-project/build/bin/clang-21+0x1000059a4)
#<!-- -->49 0x00000001043ea480 main (.../llvm-project/build/bin/clang-21+0x100012480)
#<!-- -->50 0x0000000194402b98
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 93ebe63f2e7a252038bde01a4399c14e0123cdac)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: .../llvm-project/build/bin
Build config: +assertions
```
</details>


---

### Comment 2 - shafik

CC @ChuanqiXu9 @Bigcheese 

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Ross Kirsling (rkirsling)

<details>
### Source

`header.h`
```cpp
#pragma once

inline namespace {
template&lt;typename T&gt; T g(T v) { return v; }
template&lt;typename T&gt; T f(T v) { return g(v); }
template&lt;typename T&gt; T g();
}
```
`main.cpp`
```cpp
#include "header.h"

int x;
void i() { f(x); }
```

### Commands

```
λ bin/clang -x c++-header -c header.h -o pch.pch
λ bin/clang -ftime-trace -c main.cpp -include-pch pch.pch
```

### Crash

```
Assertion failed: (isHandleInSync() &amp;&amp; "invalid iterator access!"), function operator*, file DenseMap.h, line 1211.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.  Program arguments: bin/clang -ftime-trace -c main.cpp -include-pch pch.pch
1.  &lt;eof&gt; parser at end of file
2.  .../llvm-project/build/header.h:5:25: instantiating function definition 'f&lt;int&gt;'
 #<!-- -->0 0x00000001070494ec llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (.../llvm-project/build/bin/clang-21+0x102c714ec)
 #<!-- -->1 0x000000010704729c llvm::sys::RunSignalHandlers() (.../llvm-project/build/bin/clang-21+0x102c6f29c)
 #<!-- -->2 0x0000000107048ab4 llvm::sys::CleanupOnSignal(unsigned long) (.../llvm-project/build/bin/clang-21+0x102c70ab4)
 #<!-- -->3 0x0000000106f8b7f8 CrashRecoverySignalHandler(int) (.../llvm-project/build/bin/clang-21+0x102bb37f8)
 #<!-- -->4 0x00000001947dd6a4 (/usr/lib/system/libsystem_platform.dylib+0x1804ad6a4)
 #<!-- -->5 0x00000001947a388c (/usr/lib/system/libsystem_pthread.dylib+0x18047388c)
 #<!-- -->6 0x00000001946aca3c (/usr/lib/system/libsystem_c.dylib+0x18037ca3c)
 #<!-- -->7 0x00000001946abc70 (/usr/lib/system/libsystem_c.dylib+0x18037bc70)
 #<!-- -->8 0x000000010aef9f7c clang::serialization::MultiOnDiskHashTable&lt;clang::serialization::reader::LazySpecializationInfoLookupTrait&gt;::find(unsigned int const&amp;) (.cold.1) (.../llvm-project/build/bin/clang-21+0x106b21f7c)
 #<!-- -->9 0x0000000107d6b0dc clang::ASTReader::LoadExternalSpecializationsImpl(llvm::DenseMap&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable, llvm::DenseMapInfo&lt;clang::Decl const*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl const*, clang::serialization::reader::LazySpecializationInfoLookupTable&gt;&gt;&amp;, clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (.../llvm-project/build/bin/clang-21+0x1039930dc)
#<!-- -->10 0x0000000107d6b56c non-virtual thunk to clang::ASTReader::LoadExternalSpecializations(clang::Decl const*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;) (.../llvm-project/build/bin/clang-21+0x10399356c)
#<!-- -->11 0x000000010965ddec clang::FunctionTemplateDecl::findSpecialization(llvm::ArrayRef&lt;clang::TemplateArgument&gt;, void*&amp;) (.../llvm-project/build/bin/clang-21+0x105285dec)
#<!-- -->12 0x00000001091da9d8 clang::TemplateDeclInstantiator::VisitFunctionDecl(clang::FunctionDecl*, clang::TemplateParameterList*, clang::TemplateDeclInstantiator::RewriteKind) (.../llvm-project/build/bin/clang-21+0x104e029d8)
#<!-- -->13 0x000000010921d784 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::$_3&gt;(long) (.../llvm-project/build/bin/clang-21+0x104e45784)
#<!-- -->14 0x00000001074cc7dc clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (.../llvm-project/build/bin/clang-21+0x1030f47dc)
#<!-- -->15 0x00000001091e1b48 clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (.../llvm-project/build/bin/clang-21+0x104e09b48)
#<!-- -->16 0x00000001090ba5f8 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, bool, bool, llvm::function_ref&lt;bool (bool)&gt;) (.../llvm-project/build/bin/clang-21+0x104ce25f8)
#<!-- -->17 0x0000000109104380 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;)::$_5&gt;(long) (.../llvm-project/build/bin/clang-21+0x104d2c380)
#<!-- -->18 0x00000001074cc7dc clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (.../llvm-project/build/bin/clang-21+0x1030f47dc)
#<!-- -->19 0x00000001090bd054 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, bool, clang::QualType, clang::Expr::Classification, bool, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;, bool)&gt;) (.../llvm-project/build/bin/clang-21+0x104ce5054)
#<!-- -->20 0x0000000108fca334 AddTemplateOverloadCandidateImmediately(clang::Sema&amp;, clang::OverloadCandidateSet&amp;, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (.../llvm-project/build/bin/clang-21+0x104bf2334)
#<!-- -->21 0x0000000108fd4b84 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&amp;) (.../llvm-project/build/bin/clang-21+0x104bfcb84)
#<!-- -->22 0x0000000108fc5e3c clang::OverloadCandidateSet::BestViableFunction(clang::Sema&amp;, clang::SourceLocation, clang::OverloadCandidate*&amp;) (.../llvm-project/build/bin/clang-21+0x104bede3c)
#<!-- -->23 0x0000000108fdb448 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (.../llvm-project/build/bin/clang-21+0x104c03448)
#<!-- -->24 0x0000000108d0de88 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (.../llvm-project/build/bin/clang-21+0x104935e88)
#<!-- -->25 0x0000000108d20a20 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (.../llvm-project/build/bin/clang-21+0x104948a20)
#<!-- -->26 0x00000001091986b0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) (.../llvm-project/build/bin/clang-21+0x104dc06b0)
#<!-- -->27 0x00000001091b2040 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformReturnStmt(clang::ReturnStmt*) (.../llvm-project/build/bin/clang-21+0x104dda040)
#<!-- -->28 0x000000010919e498 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) (.../llvm-project/build/bin/clang-21+0x104dc6498)
#<!-- -->29 0x000000010914f7e0 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (.../llvm-project/build/bin/clang-21+0x104d777e0)
#<!-- -->30 0x00000001091e3618 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (.../llvm-project/build/bin/clang-21+0x104e0b618)

#<!-- -->31 0x00000001091e5e14 clang::Sema::PerformPendingInstantiations(bool, bool) (.../llvm-project/build/bin/clang-21+0x104e0de14)
#<!-- -->32 0x0000000108abc1d4 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.../llvm-project/build/bin/clang-21+0x1046e41d4)
#<!-- -->33 0x0000000108abc620 clang::Sema::ActOnEndOfTranslationUnit() (.../llvm-project/build/bin/clang-21+0x1046e4620)
#<!-- -->34 0x0000000108a265a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (.../llvm-project/build/bin/clang-21+0x10464e5a8)
#<!-- -->35 0x000000010895d2e0 clang::ParseAST(clang::Sema&amp;, bool, bool) (.../llvm-project/build/bin/clang-21+0x1045852e0)
#<!-- -->36 0x0000000107c916f0 clang::FrontendAction::Execute() (.../llvm-project/build/bin/clang-21+0x1038b96f0)
#<!-- -->37 0x0000000107c17b4c clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (.../llvm-project/build/bin/clang-21+0x10383fb4c)
#<!-- -->38 0x0000000107d30600 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (.../llvm-project/build/bin/clang-21+0x103958600)
#<!-- -->39 0x00000001043e11b4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (.../llvm-project/build/bin/clang-21+0x1000091b4)
#<!-- -->40 0x00000001043de660 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) (.../llvm-project/build/bin/clang-21+0x100006660)
#<!-- -->41 0x00000001043dfdf0 int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::$_2&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) (.../llvm-project/build/bin/clang-21+0x100007df0)
#<!-- -->42 0x0000000107a5986c void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const::$_1&gt;(long) (.../llvm-project/build/bin/clang-21+0x10368186c)
#<!-- -->43 0x0000000106f8b500 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (.../llvm-project/build/bin/clang-21+0x102bb3500)
#<!-- -->44 0x0000000107a590d8 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::__1::optional&lt;llvm::StringRef&gt;&gt;, std::__1::basic_string&lt;char, std::__1::char_traits&lt;char&gt;, std::__1::allocator&lt;char&gt;&gt;*, bool*) const (.../llvm-project/build/bin/clang-21+0x1036810d8)
#<!-- -->45 0x0000000107a2141c clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (.../llvm-project/build/bin/clang-21+0x10364941c)
#<!-- -->46 0x0000000107a215e4 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (.../llvm-project/build/bin/clang-21+0x1036495e4)
#<!-- -->47 0x0000000107a3bb88 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::__1::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (.../llvm-project/build/bin/clang-21+0x103663b88)
#<!-- -->48 0x00000001043dd9a4 clang_main(int, char**, llvm::ToolContext const&amp;) (.../llvm-project/build/bin/clang-21+0x1000059a4)
#<!-- -->49 0x00000001043ea480 main (.../llvm-project/build/bin/clang-21+0x100012480)
#<!-- -->50 0x0000000194402b98
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 93ebe63f2e7a252038bde01a4399c14e0123cdac)
Target: arm64-apple-darwin24.6.0
Thread model: posix
InstalledDir: .../llvm-project/build/bin
Build config: +assertions
```
</details>


---

### Comment 4 - romanova-ekaterina

Douglas Yung (@dyung) had bisected this issue and found a commit that introduced the regression. Thank you Doug!

https://github.com/llvm/llvm-project/commit/97fdc237ddda7565c7c902cc4fc764f73e70686b



---

### Comment 5 - romanova-ekaterina

Chuanqi Xu (@ChuanqiXu9) - if you have a chance, could you please look at this regression that you commit had introduced? 


---

### Comment 6 - hokein

I think I know the problem, will send out a fix soon.

---

