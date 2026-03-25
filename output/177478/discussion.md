# [clang] Assertion `getObjCLifetime() == qs.getObjCLifetime() || !hasObjCLifetime() || !qs.hasObjCLifetime()' failed

**Author:** ellishg
**URL:** https://github.com/llvm/llvm-project/issues/177478
**Status:** Closed
**Labels:** clang:frontend, crash, objective-c
**Closed Date:** 2026-02-10T17:35:43Z

## Body

Clang hits an assert when building ObjC with ARC under some specific condition.

According to Claude, "when binding a forwarded id (implicitly `__strong`) to `const id&`, `SemaInit.cpp::TryReferenceInitializationCore()` attempts to combine qualifiers with conflicting ObjC lifetime qualifiers, triggering the assertion." I won't pretend to fully understand this, but I was able to find a minimal reproducer.


$ cat repro.mm
```cpp
void take(const id&);

template <typename... Args>
void call(Args... args) {
  (take(static_cast<Args&&>(args)), ...);
}

static void foo() {
  call<id>(nullptr);
}
```
$ clang -fobjc-arc -std=c++17 -fsyntax-only -target arm64-apple-ios15.0 repro.mm


```
clang: llvm-project/clang/include/clang/AST/TypeBase.h:694: void clang::Qualifiers::addConsistentQualifiers(clang::Qualifiers): Assertion `getObjCLifetime() == qs.getObjCLifetime() || !hasObjCLifetime() || !qs.hasObjCLifetime()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: llvm-project/build/bin/clang -fobjc-arc -std=c++17 -fsyntax-only -target arm64-apple-ios15.0 repro.mm
1.	<eof> parser at end of file
2.     repro.mm:7:6: instantiating function definition 'call<id>'
 #0 0x00007fde46e92b8b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:846:3
 #1 0x00007fde46e8fedb llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #2 0x00007fde46d97dc0 HandleCrash llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #3 0x00007fde46d97dc0 CrashRecoverySignalHandler(int) llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:62
 #4 0x00007fde4643fc30 __restore_rt (/lib64/libc.so.6+0x3fc30)
 #5 0x00007fde4648d03c __pthread_kill_implementation (/lib64/libc.so.6+0x8d03c)
 #6 0x00007fde4643fb86 gsignal (/lib64/libc.so.6+0x3fb86)
 #7 0x00007fde46429873 abort (/lib64/libc.so.6+0x29873)
 #8 0x00007fde4642979b _nl_load_domain.cold (/lib64/libc.so.6+0x2979b)
 #9 0x00007fde464388c6 (/lib64/libc.so.6+0x388c6)
#10 0x00007fde4483ee9b clang::PointerAuthQualifier::fromOpaqueValue(unsigned int) llvm-project/clang/include/clang/AST/TypeBase.h:694:5
#11 0x00007fde4483ee9b clang::Qualifiers::getPointerAuth() const llvm-project/clang/include/clang/AST/TypeBase.h:604:49
#12 0x00007fde4483ee9b clang::Qualifiers::addConsistentQualifiers(clang::Qualifiers) llvm-project/clang/include/clang/AST/TypeBase.h:696:5
#13 0x00007fde4486f917 clang::ASTContext::getExtQualType(clang::Type const*, clang::Qualifiers) const llvm-project/clang/lib/AST/ASTContext.cpp:3163:27
#14 0x00007fde43817cc5 clang::ASTContext::getQualifiedType(clang::QualType, clang::Qualifiers) const llvm-project/clang/include/clang/AST/ASTContext.h:2509:3
#15 0x00007fde43b2c1ff TryReferenceInitializationCore(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, clang::Expr*, clang::QualType, clang::QualType, clang::Qualifiers, clang::QualType, clang::QualType, clang::Qualifiers, clang::InitializationSequence&, bool) llvm-project/clang/lib/Sema/SemaInit.cpp:5655:5
#16 0x00007fde43b378e8 TryReferenceInitialization llvm-project/clang/lib/Sema/SemaInit.cpp:5450:33
#17 0x00007fde43b378e8 clang::InitializationSequence::InitializeFrom(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, bool, bool) llvm-project/clang/lib/Sema/SemaInit.cpp:6762:33
#18 0x00007fde43b48c50 clang::InitializedEntity::getKind() const llvm-project/clang/include/clang/Sema/Initialization.h:446:39
#19 0x00007fde43b48c50 clang::InitializedEntity::isParameterKind() const llvm-project/clang/include/clang/Sema/Initialization.h:480:20
#20 0x00007fde43b48c50 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) llvm-project/clang/lib/Sema/SemaInit.cpp:10015:29
#21 0x00007fde43a14566 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef<clang::Expr*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::VariadicCallType, bool, bool) llvm-project/clang/lib/Sema/SemaExpr.cpp:6152:50
#22 0x00007fde43a15743 clang::Sema::ConvertArgumentsForCall(clang::CallExpr*, clang::Expr*, clang::FunctionDecl*, clang::FunctionProtoType const*, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, bool) llvm-project/clang/lib/Sema/SemaExpr.cpp:6087:3
#23 0x00007fde43a16f17 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) llvm-project/clang/lib/Sema/SemaExpr.cpp:7100:5
#24 0x00007fde43d6a590 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) llvm-project/clang/lib/Sema/SemaOverload.cpp:14928:70
#25 0x00007fde439d7135 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) llvm-project/clang/lib/Sema/SemaExpr.cpp:6744:66
#26 0x00007fde439d8a0e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) llvm-project/clang/lib/Sema/SemaExpr.cpp:6601:3
#27 0x00007fde43fda026 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) llvm-project/clang/lib/Sema/TreeTransform.h:13651:1
#28 0x00007fde43fdc7ec clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) llvm-project/clang/lib/Sema/TreeTransform.h:16718:5
#29 0x00007fde43f7a14b clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) llvm-project/clang/lib/Sema/TreeTransform.h:4403:48
#30 0x00007fde43f7a893 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) llvm-project/clang/lib/Sema/TreeTransform.h:8145:5
#31 0x00007fde43f7b75d llvm::DenseMap<clang::Decl*, clang::Decl*, llvm::DenseMapInfo<clang::Decl*, void>, llvm::detail::DenseMapPair<clang::Decl*, clang::Decl*>>::deallocateBuckets() llvm-project/llvm/include/llvm/ADT/DenseMap.h:826:50
#32 0x00007fde43f7b75d llvm::DenseMap<clang::Decl*, clang::Decl*, llvm::DenseMapInfo<clang::Decl*, void>, llvm::detail::DenseMapPair<clang::Decl*, clang::Decl*>>::~DenseMap() llvm-project/llvm/include/llvm/ADT/DenseMap.h:788:22
#33 0x00007fde43f7b75d ~TreeTransform llvm-project/clang/lib/Sema/TreeTransform.h:122:7
#34 0x00007fde43f7b75d ~TemplateInstantiator llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1272:9
#35 0x00007fde43f7b75d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4329:1
#36 0x00007fde44052b5d clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5949:23
#37 0x00007fde4405031a clang::FunctionDecl::isDefined() const llvm-project/clang/include/clang/AST/Decl.h:2278:21
#38 0x00007fde4405031a clang::Sema::PerformPendingInstantiations(bool, bool) llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:7223:32
#39 0x00007fde4363650e llvm::TimeTraceScope::~TimeTraceScope() llvm-project/llvm/include/llvm/Support/TimeProfiler.h:200:5
#40 0x00007fde4363650e clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) llvm-project/clang/lib/Sema/Sema.cpp:1222:3
#41 0x00007fde43636686 clang::Sema::ActOnEndOfTranslationUnit() llvm-project/clang/lib/Sema/Sema.cpp:1251:37
#42 0x00007fde46be832c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) llvm-project/clang/lib/Parse/Parser.cpp:691:12
#43 0x00007fde46ac677a clang::ParseAST(clang::Sema&, bool, bool) llvm-project/clang/lib/Parse/ParseAST.cpp:169:69
#44 0x00007fde495b2035 clang::FrontendAction::Execute() llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:38
#45 0x00007fde49541645 llvm::Error::setChecked(bool) llvm-project/llvm/include/llvm/Support/Error.h:309:22
#46 0x00007fde49541645 llvm::Error::operator bool() llvm-project/llvm/include/llvm/Support/Error.h:241:15
#47 0x00007fde49541645 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:42
#48 0x00007fde4e30269a std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/11/bits/shared_ptr_base.h:1296:16
#49 0x00007fde4e30269a std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/11/bits/shared_ptr_base.h:993:69
#50 0x00007fde4e30269a std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/11/bits/shared_ptr_base.h:979:2
#51 0x00007fde4e30269a clang::CompilerInvocation::getFrontendOpts() llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#52 0x00007fde4e30269a clang::CompilerInstance::getFrontendOpts() llvm-project/clang/include/clang/Frontend/CompilerInstance.h:301:39
#53 0x00007fde4e30269a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:311:29
#54 0x000000000021bd48 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) llvm-project/clang/tools/driver/cc1_main.cpp:304:40
#55 0x00000000002138f1 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) llvm-project/clang/tools/driver/driver.cpp:226:20
#56 0x0000000000213a9d llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::release() llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#57 0x0000000000213a9d llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::~IntrusiveRefCntPtr() llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#58 0x0000000000213a9d operator() llvm-project/clang/tools/driver/driver.cpp:376:26
#59 0x0000000000213a9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:52
#60 0x00007fde4991dc79 operator() llvm-project/clang/lib/Driver/Job.cpp:442:32
#61 0x00007fde4991dc79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:52
#62 0x00007fde46d97f14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:10
#63 0x00007fde4991e65e clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) llvm-project/clang/lib/Driver/Job.cpp:446:10
#64 0x00007fde498d2bc3 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const llvm-project/clang/lib/Driver/Compilation.cpp:197:3
#65 0x00007fde498d3246 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const llvm-project/clang/lib/Driver/Compilation.cpp:246:5
#66 0x00007fde498e9764 llvm::SmallVectorBase<unsigned int>::empty() const llvm-project/llvm/include/llvm/ADT/SmallVector.h:83:46
#67 0x00007fde498e9764 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) llvm-project/clang/lib/Driver/Driver.cpp:2265:28
#68 0x0000000000217016 llvm::SmallVectorBase<unsigned int>::size() const llvm-project/llvm/include/llvm/ADT/SmallVector.h:80:32
#69 0x0000000000217016 llvm::SmallVectorTemplateCommon<std::pair<int, clang::driver::Command const*>, void>::end() llvm-project/llvm/include/llvm/ADT/SmallVector.h:275:41
#70 0x0000000000217016 clang_main(int, char**, llvm::ToolContext const&) llvm-project/clang/tools/driver/driver.cpp:416:26
#71 0x0000000000229f04 main llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:18:1
#72 0x00007fde4642a610 __libc_start_call_main (/lib64/libc.so.6+0x2a610)
#73 0x00007fde4642a6c0 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x2a6c0)
#74 0x0000000000212c35 _start (llvm-project/build/bin/clang+0x212c35)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 23.0.0git (git@github.com:ellishg/llvm-project.git 1286de408cc4a3ba1bd6cb6fed7d9517c0429462)
Target: arm64-apple-ios15.0
Thread model: posix
InstalledDir: llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg:
********************
```

## Comments

### Comment 1 - shafik

Crashes are always bugs, when adding code snippets, please put them in a their own quoted section since that gives the benefit of quick copy-and-paste.

Adding a godbolt link is also helpful, when possible.

I see ptr auth so @ojhunt 

---

### Comment 2 - ellishg

https://godbolt.org/z/qrGzna16v

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Ellis Hoag (ellishg)

<details>
Clang hits an assert when building ObjC with ARC under some specific condition.

According to Claude, "when binding a forwarded id (implicitly `__strong`) to `const id&amp;`, `SemaInit.cpp::TryReferenceInitializationCore()` attempts to combine qualifiers with conflicting ObjC lifetime qualifiers, triggering the assertion." I won't pretend to fully understand this, but I was able to find a minimal reproducer.


$ cat repro.mm
```cpp
void take(const id&amp;);

template &lt;typename... Args&gt;
void call(Args... args) {
  (take(static_cast&lt;Args&amp;&amp;&gt;(args)), ...);
}

static void foo() {
  call&lt;id&gt;(nullptr);
}
```
$ clang -fobjc-arc -std=c++17 -fsyntax-only -target arm64-apple-ios15.0 repro.mm


```
clang: llvm-project/clang/include/clang/AST/TypeBase.h:694: void clang::Qualifiers::addConsistentQualifiers(clang::Qualifiers): Assertion `getObjCLifetime() == qs.getObjCLifetime() || !hasObjCLifetime() || !qs.hasObjCLifetime()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: llvm-project/build/bin/clang -fobjc-arc -std=c++17 -fsyntax-only -target arm64-apple-ios15.0 repro.mm
1.	&lt;eof&gt; parser at end of file
2.     repro.mm:7:6: instantiating function definition 'call&lt;id&gt;'
 #<!-- -->0 0x00007fde46e92b8b llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) llvm-project/llvm/lib/Support/Unix/Signals.inc:846:3
 #<!-- -->1 0x00007fde46e8fedb llvm::sys::RunSignalHandlers() llvm-project/llvm/lib/Support/Signals.cpp:108:20
 #<!-- -->2 0x00007fde46d97dc0 HandleCrash llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->3 0x00007fde46d97dc0 CrashRecoverySignalHandler(int) llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:390:62
 #<!-- -->4 0x00007fde4643fc30 __restore_rt (/lib64/libc.so.6+0x3fc30)
 #<!-- -->5 0x00007fde4648d03c __pthread_kill_implementation (/lib64/libc.so.6+0x8d03c)
 #<!-- -->6 0x00007fde4643fb86 gsignal (/lib64/libc.so.6+0x3fb86)
 #<!-- -->7 0x00007fde46429873 abort (/lib64/libc.so.6+0x29873)
 #<!-- -->8 0x00007fde4642979b _nl_load_domain.cold (/lib64/libc.so.6+0x2979b)
 #<!-- -->9 0x00007fde464388c6 (/lib64/libc.so.6+0x388c6)
#<!-- -->10 0x00007fde4483ee9b clang::PointerAuthQualifier::fromOpaqueValue(unsigned int) llvm-project/clang/include/clang/AST/TypeBase.h:694:5
#<!-- -->11 0x00007fde4483ee9b clang::Qualifiers::getPointerAuth() const llvm-project/clang/include/clang/AST/TypeBase.h:604:49
#<!-- -->12 0x00007fde4483ee9b clang::Qualifiers::addConsistentQualifiers(clang::Qualifiers) llvm-project/clang/include/clang/AST/TypeBase.h:696:5
#<!-- -->13 0x00007fde4486f917 clang::ASTContext::getExtQualType(clang::Type const*, clang::Qualifiers) const llvm-project/clang/lib/AST/ASTContext.cpp:3163:27
#<!-- -->14 0x00007fde43817cc5 clang::ASTContext::getQualifiedType(clang::QualType, clang::Qualifiers) const llvm-project/clang/include/clang/AST/ASTContext.h:2509:3
#<!-- -->15 0x00007fde43b2c1ff TryReferenceInitializationCore(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, clang::Expr*, clang::QualType, clang::QualType, clang::Qualifiers, clang::QualType, clang::QualType, clang::Qualifiers, clang::InitializationSequence&amp;, bool) llvm-project/clang/lib/Sema/SemaInit.cpp:5655:5
#<!-- -->16 0x00007fde43b378e8 TryReferenceInitialization llvm-project/clang/lib/Sema/SemaInit.cpp:5450:33
#<!-- -->17 0x00007fde43b378e8 clang::InitializationSequence::InitializeFrom(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, bool, bool) llvm-project/clang/lib/Sema/SemaInit.cpp:6762:33
#<!-- -->18 0x00007fde43b48c50 clang::InitializedEntity::getKind() const llvm-project/clang/include/clang/Sema/Initialization.h:446:39
#<!-- -->19 0x00007fde43b48c50 clang::InitializedEntity::isParameterKind() const llvm-project/clang/include/clang/Sema/Initialization.h:480:20
#<!-- -->20 0x00007fde43b48c50 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&amp;, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;, bool, bool) llvm-project/clang/lib/Sema/SemaInit.cpp:10015:29
#<!-- -->21 0x00007fde43a14566 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::VariadicCallType, bool, bool) llvm-project/clang/lib/Sema/SemaExpr.cpp:6152:50
#<!-- -->22 0x00007fde43a15743 clang::Sema::ConvertArgumentsForCall(clang::CallExpr*, clang::Expr*, clang::FunctionDecl*, clang::FunctionProtoType const*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) llvm-project/clang/lib/Sema/SemaExpr.cpp:6087:3
#<!-- -->23 0x00007fde43a16f17 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) llvm-project/clang/lib/Sema/SemaExpr.cpp:7100:5
#<!-- -->24 0x00007fde43d6a590 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) llvm-project/clang/lib/Sema/SemaOverload.cpp:14928:70
#<!-- -->25 0x00007fde439d7135 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) llvm-project/clang/lib/Sema/SemaExpr.cpp:6744:66
#<!-- -->26 0x00007fde439d8a0e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) llvm-project/clang/lib/Sema/SemaExpr.cpp:6601:3
#<!-- -->27 0x00007fde43fda026 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) llvm-project/clang/lib/Sema/TreeTransform.h:13651:1
#<!-- -->28 0x00007fde43fdc7ec clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXFoldExpr(clang::CXXFoldExpr*) llvm-project/clang/lib/Sema/TreeTransform.h:16718:5
#<!-- -->29 0x00007fde43f7a14b clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) llvm-project/clang/lib/Sema/TreeTransform.h:4403:48
#<!-- -->30 0x00007fde43f7a893 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) llvm-project/clang/lib/Sema/TreeTransform.h:8145:5
#<!-- -->31 0x00007fde43f7b75d llvm::DenseMap&lt;clang::Decl*, clang::Decl*, llvm::DenseMapInfo&lt;clang::Decl*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl*, clang::Decl*&gt;&gt;::deallocateBuckets() llvm-project/llvm/include/llvm/ADT/DenseMap.h:826:50
#<!-- -->32 0x00007fde43f7b75d llvm::DenseMap&lt;clang::Decl*, clang::Decl*, llvm::DenseMapInfo&lt;clang::Decl*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl*, clang::Decl*&gt;&gt;::~DenseMap() llvm-project/llvm/include/llvm/ADT/DenseMap.h:788:22
#<!-- -->33 0x00007fde43f7b75d ~TreeTransform llvm-project/clang/lib/Sema/TreeTransform.h:122:7
#<!-- -->34 0x00007fde43f7b75d ~TemplateInstantiator llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:1272:9
#<!-- -->35 0x00007fde43f7b75d clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4329:1
#<!-- -->36 0x00007fde44052b5d clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5949:23
#<!-- -->37 0x00007fde4405031a clang::FunctionDecl::isDefined() const llvm-project/clang/include/clang/AST/Decl.h:2278:21
#<!-- -->38 0x00007fde4405031a clang::Sema::PerformPendingInstantiations(bool, bool) llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:7223:32
#<!-- -->39 0x00007fde4363650e llvm::TimeTraceScope::~TimeTraceScope() llvm-project/llvm/include/llvm/Support/TimeProfiler.h:200:5
#<!-- -->40 0x00007fde4363650e clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) llvm-project/clang/lib/Sema/Sema.cpp:1222:3
#<!-- -->41 0x00007fde43636686 clang::Sema::ActOnEndOfTranslationUnit() llvm-project/clang/lib/Sema/Sema.cpp:1251:37
#<!-- -->42 0x00007fde46be832c clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) llvm-project/clang/lib/Parse/Parser.cpp:691:12
#<!-- -->43 0x00007fde46ac677a clang::ParseAST(clang::Sema&amp;, bool, bool) llvm-project/clang/lib/Parse/ParseAST.cpp:169:69
#<!-- -->44 0x00007fde495b2035 clang::FrontendAction::Execute() llvm-project/clang/lib/Frontend/FrontendAction.cpp:1317:38
#<!-- -->45 0x00007fde49541645 llvm::Error::setChecked(bool) llvm-project/llvm/include/llvm/Support/Error.h:309:22
#<!-- -->46 0x00007fde49541645 llvm::Error::operator bool() llvm-project/llvm/include/llvm/Support/Error.h:241:15
#<!-- -->47 0x00007fde49541645 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1007:42
#<!-- -->48 0x00007fde4e30269a std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/11/bits/shared_ptr_base.h:1296:16
#<!-- -->49 0x00007fde4e30269a std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/11/bits/shared_ptr_base.h:993:69
#<!-- -->50 0x00007fde4e30269a std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/11/bits/shared_ptr_base.h:979:2
#<!-- -->51 0x00007fde4e30269a clang::CompilerInvocation::getFrontendOpts() llvm-project/clang/include/clang/Frontend/CompilerInvocation.h:277:48
#<!-- -->52 0x00007fde4e30269a clang::CompilerInstance::getFrontendOpts() llvm-project/clang/include/clang/Frontend/CompilerInstance.h:301:39
#<!-- -->53 0x00007fde4e30269a clang::ExecuteCompilerInvocation(clang::CompilerInstance*) llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:311:29
#<!-- -->54 0x000000000021bd48 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) llvm-project/clang/tools/driver/cc1_main.cpp:304:40
#<!-- -->55 0x00000000002138f1 ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) llvm-project/clang/tools/driver/driver.cpp:226:20
#<!-- -->56 0x0000000000213a9d llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::release() llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#<!-- -->57 0x0000000000213a9d llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;::~IntrusiveRefCntPtr() llvm-project/llvm/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#<!-- -->58 0x0000000000213a9d operator() llvm-project/clang/tools/driver/driver.cpp:376:26
#<!-- -->59 0x0000000000213a9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:52
#<!-- -->60 0x00007fde4991dc79 operator() llvm-project/clang/lib/Driver/Job.cpp:442:32
#<!-- -->61 0x00007fde4991dc79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) llvm-project/llvm/include/llvm/ADT/STLFunctionalExtras.h:46:52
#<!-- -->62 0x00007fde46d97f14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) llvm-project/llvm/lib/Support/CrashRecoveryContext.cpp:427:10
#<!-- -->63 0x00007fde4991e65e clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) llvm-project/clang/lib/Driver/Job.cpp:446:10
#<!-- -->64 0x00007fde498d2bc3 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const llvm-project/clang/lib/Driver/Compilation.cpp:197:3
#<!-- -->65 0x00007fde498d3246 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const llvm-project/clang/lib/Driver/Compilation.cpp:246:5
#<!-- -->66 0x00007fde498e9764 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const llvm-project/llvm/include/llvm/ADT/SmallVector.h:83:46
#<!-- -->67 0x00007fde498e9764 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) llvm-project/clang/lib/Driver/Driver.cpp:2265:28
#<!-- -->68 0x0000000000217016 llvm::SmallVectorBase&lt;unsigned int&gt;::size() const llvm-project/llvm/include/llvm/ADT/SmallVector.h:80:32
#<!-- -->69 0x0000000000217016 llvm::SmallVectorTemplateCommon&lt;std::pair&lt;int, clang::driver::Command const*&gt;, void&gt;::end() llvm-project/llvm/include/llvm/ADT/SmallVector.h:275:41
#<!-- -->70 0x0000000000217016 clang_main(int, char**, llvm::ToolContext const&amp;) llvm-project/clang/tools/driver/driver.cpp:416:26
#<!-- -->71 0x0000000000229f04 main llvm-project/build/tools/clang/tools/driver/clang-driver.cpp:18:1
#<!-- -->72 0x00007fde4642a610 __libc_start_call_main (/lib64/libc.so.6+0x2a610)
#<!-- -->73 0x00007fde4642a6c0 __libc_start_main@<!-- -->GLIBC_2.2.5 (/lib64/libc.so.6+0x2a6c0)
#<!-- -->74 0x0000000000212c35 _start (llvm-project/build/bin/clang+0x212c35)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
clang version 23.0.0git (git@<!-- -->github.com:ellishg/llvm-project.git 1286de408cc4a3ba1bd6cb6fed7d9517c0429462)
Target: arm64-apple-ios15.0
Thread model: posix
InstalledDir: llvm-project/build/bin
Build config: +assertions
clang: note: diagnostic msg:
********************
```
</details>


---

### Comment 4 - ojhunt

> Crashes are always bugs, when adding code snippets, please put them in a their own quoted section since that gives the benefit of quick copy-and-paste.
> 
> Adding a godbolt link is also helpful, when possible.
> 
> I see ptr auth so [@ojhunt](https://github.com/ojhunt)

The ptrauth failure in the crash trace was super confusing but it looks to just be optimized code stacktraces so that's a small mercy (something going wrong in basic interaction with the qualifier would imply me having truly broken something during upstreaming :D)

---

