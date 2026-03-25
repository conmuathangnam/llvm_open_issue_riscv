# [clangd] Crash at clang::Decl::getLocation() DeclBase.h 442:47

**Author:** henryhchchc
**URL:** https://github.com/llvm/llvm-project/issues/140685

## Body

Opening a C++ file with the following content leads to a crash in clangd.

```c++
namespace rdar13395022 {
  struct MoveOnly ;

  void test(MoveOnly mo) {
    auto &&list1 = {mo};
    MoveOnly (&&list2)[1] = {mo};
    std::initializer_list<MoveOnly> &&list3 = {};
    MoveOnly (&&list4)[1] = {};
  }
}

```

Stack trace:
```
I[11:08:00.626] clangd version 20.1.4 (https://github.com/llvm/llvm-project.git ec28b8f9cc7f2ac187d8a617a6d08d5e56f9120e)
I[11:08:00.626] Features: linux+asan

I[11:08:00.626] PID: 5712
I[11:08:00.626] Working directory: /tmp/export/input_118
I[11:08:00.626] argv[0]: /src/build/bin/clangd
I[11:08:00.626] Starting LSP over stdin/stdout

I[11:08:00.627] <-- initialize(1)

I[11:08:00.655] --> reply:initialize(1) 27 ms

I[11:08:00.656] <-- initialized

I[11:08:00.656] <-- textDocument/didOpen

I[11:08:00.657] Failed to find compilation database for /tmp/export/input_118/workspace/main.hpp

I[11:08:00.658] ASTWorker building file /tmp/export/input_118/workspace/main.hpp version 0 with command clangd fallback
[/tmp/export/input_118/workspace]
/usr/bin/clang -resource-dir=/src/build/lib/clang/20 -- /tmp/export/input_118/workspace/main.hpp

I[11:08:00.677] Built preamble of size 275484 for file /tmp/export/input_118/workspace/main.hpp version 0 in 0.01 seconds

I[11:08:00.681] Indexing c++17 standard library in the context of /tmp/export/input_118/workspace/main.hpp

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.\t/tmp/export/input_118/workspace/main.hpp:9:31: current parser token ';'
1.\t/tmp/export/input_118/workspace/main.hpp:2:1: parsing namespace 'rdar13395022'
2.\t/tmp/export/input_118/workspace/main.hpp:5:26: parsing function body 'rdar13395022::test'
3.\t/tmp/export/input_118/workspace/main.hpp:5:26: in compound statement ('{}')

 #0 0x000000000582426a ___interceptor_backtrace (/src/build/bin/clangd+0x582426a)
 #1 0x0000000005dfcc27 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /src/llvm/lib/Support/Unix/Signals.inc:799:13
 #2 0x0000000005dfbfec llvm::sys::RunSignalHandlers() /src/llvm/lib/Support/Signals.cpp:0:5
 #3 0x0000000005dfbfec SignalHandler(int) /src/llvm/lib/Support/Unix/Signals.inc:405:3
 #4 0x00007ffff7ac0730 __restore_rt (/lib64/libc.so.6+0x3e730)
 #5 0x000000000706a07e clang::Decl::getLocation() const /src/clang/include/clang/AST/DeclBase.h:442:47
 #6 0x000000000706a07e clang::Sema::LookupSpecialMember(clang::CXXRecordDecl*, clang::CXXSpecialMemberKind, bool, bool, bool, bool, bool) /src/clang/lib/Sema/SemaLookup.cpp:3372:34
 #7 0x00000000078d777d clang::Sema::LookupDestructor(clang::CXXRecordDecl*) /src/clang/lib/Sema/SemaLookup.cpp:3654:7
 #8 0x00000000078d777d checkDestructorReference(clang::QualType, clang::SourceLocation, clang::Sema&) /src/clang/lib/Sema/SemaInit.cpp:2016:43
 #9 0x00000000078f926e (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, llvm::APSInt, bool, unsigned int&, clang::InitListExpr*, unsigned int&) /src/clang/lib/Sema/SemaInit.cpp:2065:9
#10 0x00000000078af604 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) /src/clang/lib/Sema/SemaInit.cpp:1420:5
#11 0x00000000078abefc (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) /src/clang/lib/Sema/SemaInit.cpp:1306:3
#12 0x00000000078abefc (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) /src/clang/lib/Sema/SemaInit.cpp:1060:3
#13 0x00000000078dafe5 diagnoseListInit(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8723:19
#14 0x00000000078db075 diagnoseListInit(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8715:5
#15 0x00000000078bd2d9 clang::InitializationSequence::Diagnose(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::ArrayRef<clang::Expr*>) /src/clang/lib/Sema/SemaInit.cpp:0:5
#16 0x00000000078c13dc clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) /src/clang/lib/Sema/SemaInit.cpp:0:5
#17 0x00000000074522c4 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13606:33
#18 0x00000000102f6025 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:0:17
#19 0x00000000102eec5e clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
#20 0x00000000102ec939 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:2168:10
#21 0x00000000102eb6f3 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:0:14
#22 0x0000000010385671 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /src/clang/lib/Parse/ParseStmt.cpp:0:0
#23 0x00000000103840af clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /src/clang/lib/Parse/ParseStmt.cpp:0:20
#24 0x000000001038ff6e clang::Parser::ParseCompoundStatementBody(bool) /src/clang/lib/Parse/ParseStmt.cpp:1267:11
#25 0x00000000103a2748 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /src/clang/lib/Parse/ParseStmt.cpp:2577:21
#26 0x00000000101fa0bf clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /src/clang/lib/Parse/Parser.cpp:0:10
#27 0x00000000102f0c67 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:0:17
#28 0x00000000101f3d72 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
#29 0x00000000101f2f20 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:0:12
#30 0x00000000101f0488 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:0:14
#31 0x0000000010334392 clang::AttributePool::~AttributePool() /src/clang/include/clang/Sema/ParsedAttr.h:724:22
#32 0x0000000010334392 clang::ParsedAttributes::~ParsedAttributes() /src/clang/include/clang/Sema/ParsedAttr.h:956:7
#33 0x0000000010334392 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /src/clang/lib/Parse/ParseDeclCXX.cpp:265:5
#34 0x0000000010332775 clang::Parser::ParseScope::Exit() /src/clang/include/clang/Parse/Parser.h:1197:15
#35 0x0000000010332775 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /src/clang/lib/Parse/ParseDeclCXX.cpp:244:18
#36 0x00000000102eb15a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:0:0
#37 0x00000000101ef189 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:0:14
#38 0x00000000101eb48a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /src/clang/lib/Parse/Parser.cpp:0:12
#39 0x00000000101ea309 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /src/clang/lib/Parse/Parser.cpp:615:23
#40 0x00000000101db204 clang::ParseAST(clang::Sema&, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:170:5
#41 0x000000000fd38d28 clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /src/clang/lib/Frontend/CompilerInstance.cpp:86:11
#42 0x000000000fd38d28 clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1076:10
#43 0x0000000008de4a6d llvm::Error::getPtr() const /src/llvm/include/llvm/Support/Error.h:282:12
#44 0x0000000008de4a6d llvm::Error::operator bool() /src/llvm/include/llvm/Support/Error.h:242:16
#45 0x0000000008de4a6d clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:19
#46 0x0000000008fa63e8 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
#47 0x0000000008faabaa void std::_Destroy_aux<false>::__destroy<clang::clangd::Diag*>(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:162:19
#48 0x0000000008faabaa void std::_Destroy<clang::clangd::Diag*>(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:195:7
#49 0x0000000008faabaa void std::_Destroy<clang::clangd::Diag*, clang::clangd::Diag>(clang::clangd::Diag*, clang::clangd::Diag*, std::allocator<clang::clangd::Diag>&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/alloc_traits.h:948:7
#50 0x0000000008faabaa std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>::~vector() /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_vector.h:735:2
#51 0x0000000008faabaa clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::shared_ptr<clang::clangd::PreambleData const>, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
#52 0x0000000008fa8a20 llvm::function_ref<void ()>::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#53 0x0000000008fa8a20 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
#54 0x0000000008fb2991 clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:0:7
#55 0x0000000008fb2991 clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
#56 0x0000000008fb2991 void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0>(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
#57 0x00000000092fc635 clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:103:14
#58 0x00000000092fc635 auto void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)::operator()<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) const /src/llvm/include/llvm/Support/thread.h:43:11
#59 0x00000000092fc635 auto std::__invoke_impl<void, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(std::__invoke_other, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)&
&, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
#60 0x00000000092fc635 std::__invoke_result<auto, auto...>::type std::__invoke<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
#61 0x00000000092fc635 decltype(auto) std::__apply_impl<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, 0ul>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, std::integer_sequence<unsigned long, 0ul>) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
#62 0x00000000092fc635 decltype(auto) std::apply<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
#63 0x00000000092fc635 void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /src/llvm/include/llvm/Support/thread.h:41:5
#64 0x00000000092fc635 void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /src/llvm/include/llvm/Support/thread.h:55:5
#65 0x000000000587695d asan_thread_start(void*) crtstuff.c:0:0
#66 0x00007ffff7b0b7fa start_thread (/lib64/libc.so.6+0x897fa)
#67 0x00007ffff7b90820 __GI___clone3 (/lib64/libc.so.6+0x10e820)
Signalled during AST worker action: Build AST
  Filename: main.hpp
  Directory: /tmp/export/input_118/workspace
  Command Line: /usr/bin/clang -resource-dir=/src/build/lib/clang/20 -- /tmp/export/input_118/workspace/main.hpp
  Version: 0
AddressSanitizer:DEADLYSIGNAL
=================================================================
==5712==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000018 (pc 0x00000706a07e bp 0x7fff5a6e16c0 sp 0x7fff5a6e1520 T130)
==5712==The signal is caused by a READ memory access.
==5712==Hint: address points to the zero page.

    #0 0x706a07e in clang::Decl::getLocation() const /src/clang/include/clang/AST/DeclBase.h:442:47
    #1 0x706a07e in clang::Sema::LookupSpecialMember(clang::CXXRecordDecl*, clang::CXXSpecialMemberKind, bool, bool, bool, bool, bool) /src/clang/lib/Sema/SemaLookup.cpp:3372:34
    #2 0x78d777c in clang::Sema::LookupDestructor(clang::CXXRecordDecl*) /src/clang/lib/Sema/SemaLookup.cpp:3654:7
    #3 0x78d777c in checkDestructorReference(clang::QualType, clang::SourceLocation, clang::Sema&) /src/clang/lib/Sema/SemaInit.cpp:2016:43
    #4 0x78f926d in (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, llvm::APSInt, bool, unsigned int&, clang::InitListExpr*, unsigned int&) /src/clang/lib/Sema/SemaInit.cpp:2065:9
    #5 0x78af603 in (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, unsigned int&, clang::InitListExpr*, unsigned int&, bool) /src/clang/lib/Sema/SemaInit.cpp:1420:5
    #6 0x78abefb in (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) /src/clang/lib/Sema/SemaInit.cpp:1306:3
    #7 0x78abefb in (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) /src/clang/lib/Sema/SemaInit.cpp:1060:3
    #8 0x78dafe4 in diagnoseListInit(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8723:19
    #9 0x78db074 in diagnoseListInit(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8715:5
    #10 0x78bd2d8 in clang::InitializationSequence::Diagnose(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::ArrayRef<clang::Expr*>) /src/clang/lib/Sema/SemaInit.cpp:9197:5
    #11 0x78c13db in clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) /src/clang/lib/Sema/SemaInit.cpp:7648:5
    #12 0x74522c3 in clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13606:33
    #13 0x102f6024 in clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2858:17
    #14 0x102eec5d in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
    #15 0x102ec938 in clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:2168:10
    #16 0x102eb6f2 in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp
    #17 0x10385670 in clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /src/clang/lib/Parse/ParseStmt.cpp
    #18 0x103840ae in clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /src/clang/lib/Parse/ParseStmt.cpp:125:20
    #19 0x1038ff6d in clang::Parser::ParseCompoundStatementBody(bool) /src/clang/lib/Parse/ParseStmt.cpp:1267:11
    #20 0x103a2747 in clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /src/clang/lib/Parse/ParseStmt.cpp:2577:21
    #21 0x101fa0be in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /src/clang/lib/Parse/Parser.cpp:1520:10
    #22 0x102f0c66 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2461:17
    #23 0x101f3d71 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
    #24 0x101f2f1f in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1266:12
    #25 0x101f0487 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:1069:14
    #26 0x10334391 in clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /src/clang/lib/Parse/ParseDeclCXX.cpp:264:7
    #27 0x10332774 in clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /src/clang/lib/Parse/ParseDeclCXX.cpp:241:3
    #28 0x102eb159 in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:2077:12
    #29 0x101ef188 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:985:14
    #30 0x101eb489 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /src/clang/lib/Parse/Parser.cpp:758:12
    #31 0x101ea308 in clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /src/clang/lib/Parse/Parser.cpp:608:26
    #32 0x101db203 in clang::ParseAST(clang::Sema&, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:170:25
    #33 0xfd38d27 in clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1072:3
    #34 0x8de4a6c in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&, std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, llvm::ArrayRef<clang::clangd::Diag>, std::shared_ptr<clang::clangd::PreambleData const>) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:33
    #35 0x8fa63e7 in clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
    #36 0x8faaba9 in clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr<clang::CompilerInvocation, std::default_delete<clang::CompilerInvocation>>, clang::clangd::ParseInputs, std::shared_ptr<clang::clangd::PreambleData const>, std::vector<clang::clangd::Diag, std::allocator<clang::clangd::Diag>>, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
    #37 0x8fa8a1f in llvm::function_ref<void ()>::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #38 0x8fa8a1f in clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref<void ()>) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
    #39 0x8fb2990 in clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:1462:7
    #40 0x8fb2990 in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
    #41 0x8fb2990 in void llvm::detail::UniqueFunctionBase<void>::CallImpl<clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&)::$_0>(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #42 0x92fc634 in llvm::unique_function<void ()>::operator()() /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #43 0x92fc634 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:101:5
    #44 0x92fc634 in auto void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)::operator()<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) const /src/llvm/include/llvm/Support/thread.h:43:11
    #45 0x92fc634 in auto std::__invoke_impl<void, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(std::__invoke_other, void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...)&&, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
    #46 0x92fc634 in std::__invoke_result<auto, auto...>::type std::__invoke<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&>(auto&&, auto&&...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
    #47 0x92fc634 in decltype(auto) std::__apply_impl<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, 0ul>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&, std::integer_sequence<unsigned long, 0ul>) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
    #48 0x92fc634 in decltype(auto) std::apply<void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*)::'lambda'(auto&&, auto&&...), std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&>(auto&&, std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>&) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
    #49 0x92fc634 in void llvm::thread::GenericThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /src/llvm/include/llvm/Support/thread.h:41:5
    #50 0x92fc634 in void* llvm::thread::ThreadProxy<std::tuple<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>>(void*) /src/llvm/include/llvm/Support/thread.h:55:5
    #51 0x587695c in asan_thread_start(void*) crtstuff.c
    #52 0x7ffff7b0b7f9 in start_thread (/lib64/libc.so.6+0x897f9) (BuildId: fc46bc419367003d0e4e399cbe22aade4a1ee7be)
    #53 0x7ffff7b9081f in __GI___clone3 (/lib64/libc.so.6+0x10e81f) (BuildId: fc46bc419367003d0e4e399cbe22aade4a1ee7be)


AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /src/clang/include/clang/AST/DeclBase.h:442:47 in clang::Decl::getLocation() const
Thread T130 created by T0 here:

    #0 0x5860115 in pthread_create (/src/build/bin/clangd+0x5860115) (BuildId: 8b31158490359739)
    #1 0x5dfe9a8 in llvm::llvm_execute_on_thread_impl(void* (*)(void*), void*, std::optional<unsigned int>) /src/llvm/lib/Support/Unix/Threading.inc:96:17
    #2 0x92fc2d1 in llvm::thread::thread<clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1>(std::optional<unsigned int>, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>)::$_1&&) /src/llvm/include/llvm/Support/thread.h:131:12
    #3 0x92fc2d1 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&, llvm::unique_function<void ()>) /src/clang-tools-extra/clangd/support/Threading.cpp:107:16
    #4 0x8fbbadb in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&, clang::clangd::TUScheduler::ASTCache&, clang::clangd::TUScheduler::HeaderIncluderCache&, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&, clang::clangd::TUScheduler::Options const&, clang::clangd::ParsingCallbacks&) /src/clang-tools-extra/clangd/TUScheduler.cpp:825:12
    #5 0x8fbbadb in clang::clangd::TUScheduler::update(llvm::StringRef, clang::clangd::ParseInputs, clang::clangd::WantDiagnostics) /src/clang-tools-extra/clangd/TUScheduler.cpp:1681:30
    #6 0x8a87efe in clang::clangd::ClangdServer::addDocument(llvm::StringRef, llvm::StringRef, llvm::StringRef, clang::clangd::WantDiagnostics, bool) /src/clang-tools-extra/clangd/ClangdServer.cpp:316:33
    #7 0x89da572 in clang::clangd::ClangdLSPServer::onDocumentDidOpen(clang::clangd::DidOpenTextDocumentParams const&) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:726:11
    #8 0x8a1710f in void clang::clangd::LSPBinder::notification<clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer>(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&))::'lambda'(llvm::json::Value)::operator()(llvm::json::Value) const /src/clang-tools-extra/clangd/LSPBinder.h:153:5
    #9 0x8a16efe in void llvm::detail::UniqueFunctionBase<void, llvm::json::Value>::CallImpl<void clang::clangd::LSPBinder::notification<clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer>(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&))::'lambda'(llvm::json::Value)>(void*, llvm::json::Value&) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #10 0x8a3e812 in llvm::unique_function<void (llvm::json::Value)>::operator()(llvm::json::Value) /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #11 0x8a3e812 in clang::clangd::ClangdLSPServer::MessageHandler::onNotify(llvm::StringRef, llvm::json::Value) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:219:7
    #12 0x8d78847 in clang::clangd::(anonymous namespace)::JSONTransport::handleMessage(llvm::json::Value, clang::clangd::Transport::MessageHandler&) /src/clang-tools-extra/clangd/JSONTransport.cpp:195:18
    #13 0x8d78847 in clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&) /src/clang-tools-extra/clangd/JSONTransport.cpp:119:16
    #14 0x8a49859 in clang::clangd::ClangdLSPServer::run() /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:1741:25
    #15 0x8893149 in clang::clangd::clangdMain(int, char**) /src/clang-tools-extra/clangd/tool/ClangdMain.cpp:1049:28
    #16 0x7ffff7aab5cf in __libc_start_call_main (/lib64/libc.so.6+0x295cf) (BuildId: fc46bc419367003d0e4e399cbe22aade4a1ee7be)


==5712==ABORTING
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Henry Chu (henryhchchc)

<details>
Opening a C++ file with the following content leads to a crash in clangd. Compiling this file with clang does not lead to a crash.

```c++
namespace std {
  typedef decltype(sizeof(int)) size_t;

  template &lt;typename E&gt;
  struct initializer_list
  {
    const E *p;
    size_t n;
    initializer_list(const E *p, size_t n) : p(p), n(n) {}
  };

  struct string {
    string(const char *);
  };

  template&lt;typename A, typename B&gt;
  struct pair {
    pair(const A&amp;, const B&amp;);
  };
}
```

Stack trace:
```
I[06:43:53.843] clangd version 20.1.4 (https://github.com/llvm/llvm-project.git ec28b8f9cc7f2ac187d8a617a6d08d5e56f9120e)
I[06:43:53.843] Features: linux+asan
I[06:43:53.843] PID: 3794
I[06:43:53.843] Working directory: /tmp/export/input_118
I[06:43:53.843] argv[0]: /src/build/bin/clangd
I[06:43:53.843] Starting LSP over stdin/stdout
I[06:43:53.844] &lt;-- initialize(0)
I[06:43:53.873] --&gt; reply:initialize(0) 28 ms
Content-Length: 2050

{"id":0,"jsonrpc":"2.0","result":{"capabilities":{"astProvider":true,"callHierarchyProvider":true,"clangdInlayHintsProvider":true,"codeActionProvider":true,"compilationDatabase":{"automaticReload":true},"completionProvider":{"resolveProvider":false,"triggerCharacters":[".","&lt;","&gt;",":","\"","/","*"]},"declarationProvider":true,"definitionProvider":true,"documentFormattingProvider":true,"documentHighlightProvider":true,"documentLinkProvider":{"resolveProvider":false},"documentOnTypeFormattingProvider":{"firstTriggerCharacter":"\n","moreTriggerCharacter":[]},"documentRangeFormattingProvider":true,"documentSymbolProvider":true,"executeCommandProvider":{"commands":["clangd.applyFix","clangd.applyRename","clangd.applyTweak"]},"foldingRangeProvider":true,"hoverProvider":true,"implementationProvider":true,"inactiveRegionsProvider":true,"inlayHintProvider":true,"memoryUsageProvider":true,"referencesProvider":true,"renameProvider":true,"selectionRangeProvider":true,"semanticTokensProvider":{"full":{"delta":true},"legend":{"tokenModifiers":["declaration","definition","deprecated","deduced","readonly","static","abstract","virtual","dependentName","defaultLibrary","usedAsMutableReference","usedAsMutablePointer","constructorOrDestructor","userDefined","functionScope","classScope","fileScope","globalScope"],"tokenTypes":["variable","variable","parameter","function","method","function","property","variable","class","interface","enum","enumMember","type","type","unknown","namespace","typeParameter","concept","type","macro","modifier","operator","bracket","label","comment"]},"range":false},"signatureHelpProvider":{"triggerCharacters":["(",")","{","}","&lt;","&gt;",","]},"standardTypeHierarchyProvider":true,"textDocumentSync":{"change":2,"openClose":true,"save":true},"typeDefinitionProvider":true,"typeHierarchyProvider":true,"workspaceSymbolProvider":true},"serverInfo":{"name":"clangd","version":"clangd version 20.1.4 (https://github.com/llvm/llvm-project.git ec28b8f9cc7f2ac187d8a617a6d08d5e56f9120e) linux+asan x86_64-unknown-linux-gnu"}}}I[06:43:53.874] &lt;-- initialized
I[06:43:53.874] &lt;-- textDocument/didOpen
I[06:43:53.876] Failed to find compilation database for /tmp/export/input_118/workspace/main.hpp
I[06:43:53.876] ASTWorker building file /tmp/export/input_118/workspace/main.hpp version 1 with command clangd fallback
[/tmp/export/input_118/workspace]
/usr/bin/clang -resource-dir=/src/build/lib/clang/20 -- /tmp/export/input_118/workspace/main.hpp
I[06:43:53.899] Built preamble of size 275484 for file /tmp/export/input_118/workspace/main.hpp version 1 in 0.02 seconds
I[06:43:53.900] --&gt; workspace/semanticTokens/refresh(0)
Content-Length: 82

{"id":0,"jsonrpc":"2.0","method":"workspace/semanticTokens/refresh","params":null}I[06:43:53.904] Indexing c++17 standard library in the context of /tmp/export/input_118/workspace/main.hpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      /tmp/export/input_118/workspace/main.hpp:135:31: current parser token ';'
1.      /tmp/export/input_118/workspace/main.hpp:128:1: parsing namespace 'rdar13395022'
2.      /tmp/export/input_118/workspace/main.hpp:131:26: parsing function body 'rdar13395022::test'
3.      /tmp/export/input_118/workspace/main.hpp:131:26: in compound statement ('{}')
 #<!-- -->0 0x000000000582426a ___interceptor_backtrace (/src/build/bin/clangd+0x582426a)
 #<!-- -->1 0x0000000005dfcc27 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /src/llvm/lib/Support/Unix/Signals.inc:799:13
 #<!-- -->2 0x0000000005dfbfec llvm::sys::RunSignalHandlers() /src/llvm/lib/Support/Signals.cpp:0:5
 #<!-- -->3 0x0000000005dfbfec SignalHandler(int) /src/llvm/lib/Support/Unix/Signals.inc:405:3
 #<!-- -->4 0x00007ffff7ac0730 __restore_rt (/lib64/libc.so.6+0x3e730)
 #<!-- -->5 0x000000000706a07e clang::Decl::getLocation() const /src/clang/include/clang/AST/DeclBase.h:442:47
 #<!-- -->6 0x000000000706a07e clang::Sema::LookupSpecialMember(clang::CXXRecordDecl*, clang::CXXSpecialMemberKind, bool, bool, bool, bool, bool) /src/clang/lib/Sema/SemaLookup.cpp:3372:34
 #<!-- -->7 0x00000000078d777d clang::Sema::LookupDestructor(clang::CXXRecordDecl*) /src/clang/lib/Sema/SemaLookup.cpp:3654:7
 #<!-- -->8 0x00000000078d777d checkDestructorReference(clang::QualType, clang::SourceLocation, clang::Sema&amp;) /src/clang/lib/Sema/SemaInit.cpp:2016:43
 #<!-- -->9 0x00000000078f926e (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, llvm::APSInt, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;) /src/clang/lib/Sema/SemaInit.cpp:2065:9
#<!-- -->10 0x00000000078af604 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) /src/clang/lib/Sema/SemaInit.cpp:1420:5
#<!-- -->11 0x00000000078abefc (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) /src/clang/lib/Sema/SemaInit.cpp:1306:3
#<!-- -->12 0x00000000078abefc (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) /src/clang/lib/Sema/SemaInit.cpp:1060:3
#<!-- -->13 0x00000000078dafe5 diagnoseListInit(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8723:19
#<!-- -->14 0x00000000078db075 diagnoseListInit(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8715:5
#<!-- -->15 0x00000000078bd2d9 clang::InitializationSequence::Diagnose(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;) /src/clang/lib/Sema/SemaInit.cpp:0:5
#<!-- -->16 0x00000000078c13dc clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) /src/clang/lib/Sema/SemaInit.cpp:0:5
#<!-- -->17 0x00000000074522c4 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13606:33
#<!-- -->18 0x00000000102f6025 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:0:17
#<!-- -->19 0x00000000102eec5e clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
#<!-- -->20 0x00000000102ec939 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:2168:10
#<!-- -->21 0x00000000102eb6f3 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:0:14
#<!-- -->22 0x0000000010385671 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /src/clang/lib/Parse/ParseStmt.cpp:0:0
#<!-- -->23 0x00000000103840af clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /src/clang/lib/Parse/ParseStmt.cpp:0:20
#<!-- -->24 0x000000001038ff6e clang::Parser::ParseCompoundStatementBody(bool) /src/clang/lib/Parse/ParseStmt.cpp:1267:11
#<!-- -->25 0x00000000103a2748 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /src/clang/lib/Parse/ParseStmt.cpp:2577:21
#<!-- -->26 0x00000000101fa0bf clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /src/clang/lib/Parse/Parser.cpp:0:10
#<!-- -->27 0x00000000102f0c67 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:0:17
#<!-- -->28 0x00000000101f3d72 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
#<!-- -->29 0x00000000101f2f20 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:0:12
#<!-- -->30 0x00000000101f0488 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:0:14
#<!-- -->31 0x0000000010334392 clang::AttributePool::~AttributePool() /src/clang/include/clang/Sema/ParsedAttr.h:724:22
#<!-- -->32 0x0000000010334392 clang::ParsedAttributes::~ParsedAttributes() /src/clang/include/clang/Sema/ParsedAttr.h:956:7
#<!-- -->33 0x0000000010334392 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /src/clang/lib/Parse/ParseDeclCXX.cpp:265:5
#<!-- -->34 0x0000000010332775 clang::Parser::ParseScope::Exit() /src/clang/include/clang/Parse/Parser.h:1197:15
#<!-- -->35 0x0000000010332775 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /src/clang/lib/Parse/ParseDeclCXX.cpp:244:18
#<!-- -->36 0x00000000102eb15a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:0:0
#<!-- -->37 0x00000000101ef189 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:0:14
#<!-- -->38 0x00000000101eb48a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /src/clang/lib/Parse/Parser.cpp:0:12
#<!-- -->39 0x00000000101db4de clang::ParseAST(clang::Sema&amp;, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:170:5
#<!-- -->40 0x000000000fd38d28 clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /src/clang/lib/Frontend/CompilerInstance.cpp:86:11
#<!-- -->41 0x000000000fd38d28 clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1076:10
#<!-- -->42 0x0000000008de4a6d llvm::Error::getPtr() const /src/llvm/include/llvm/Support/Error.h:282:12
#<!-- -->43 0x0000000008de4a6d llvm::Error::operator bool() /src/llvm/include/llvm/Support/Error.h:242:16
#<!-- -->44 0x0000000008de4a6d clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:19
#<!-- -->45 0x0000000008fa63e8 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
#<!-- -->46 0x0000000008faabaa void std::_Destroy_aux&lt;false&gt;::__destroy&lt;clang::clangd::Diag*&gt;(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:162:19
#<!-- -->47 0x0000000008faabaa void std::_Destroy&lt;clang::clangd::Diag*&gt;(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:195:7
#<!-- -->48 0x0000000008faabaa void std::_Destroy&lt;clang::clangd::Diag*, clang::clangd::Diag&gt;(clang::clangd::Diag*, clang::clangd::Diag*, std::allocator&lt;clang::clangd::Diag&gt;&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/alloc_traits.h:948:7
#<!-- -->49 0x0000000008faabaa std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;::~vector() /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_vector.h:735:2
#<!-- -->50 0x0000000008faabaa clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
#<!-- -->51 0x0000000008fa8a20 llvm::function_ref&lt;void ()&gt;::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->52 0x0000000008fa8a20 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
#<!-- -->53 0x0000000008fb2991 clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:0:7
#<!-- -->54 0x0000000008fb2991 clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
#<!-- -->55 0x0000000008fb2991 void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0&gt;(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
#<!-- -->56 0x00000000092fc635 clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:103:14
#<!-- -->57 0x00000000092fc635 auto void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)::operator()&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) const /src/llvm/include/llvm/Support/thread.h:43:11
#<!-- -->58 0x00000000092fc635 auto std::__invoke_impl&lt;void, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(std::__invoke_other, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)&amp;&amp;, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
#<!-- -->59 0x00000000092fc635 std::__invoke_result&lt;auto, auto...&gt;::type std::__invoke&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
#<!-- -->60 0x00000000092fc635 decltype(auto) std::__apply_impl&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, 0ul&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, std::integer_sequence&lt;unsigned long, 0ul&gt;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
#<!-- -->61 0x00000000092fc635 decltype(auto) std::apply&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
#<!-- -->62 0x00000000092fc635 void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:41:5
#<!-- -->63 0x00000000092fc635 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:55:5
#<!-- -->64 0x000000000587695d asan_thread_start(void*) crtstuff.c:0:0
#<!-- -->65 0x00007ffff7b0b7fa start_thread (/lib64/libc.so.6+0x897fa)
#<!-- -->66 0x00007ffff7b90820 __GI___clone3 (/lib64/libc.so.6+0x10e820)
Signalled during AST worker action: Build AST
  Filename: main.hpp
  Directory: /tmp/export/input_118/workspace
  Command Line: /usr/bin/clang -resource-dir=/src/build/lib/clang/20 -- /tmp/export/input_118/workspace/main.hpp
  Version: 1
AddressSanitizer:DEADLYSIGNAL
=================================================================
==3794==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000018 (pc 0x00000706a07e bp 0x7fff5a6e1740 sp 0x7fff5a6e15a0 T130)
==3794==The signal is caused by a READ memory access.
==3794==Hint: address points to the zero page.
    #<!-- -->0 0x706a07e in clang::Decl::getLocation() const /src/clang/include/clang/AST/DeclBase.h:442:47
    #<!-- -->1 0x706a07e in clang::Sema::LookupSpecialMember(clang::CXXRecordDecl*, clang::CXXSpecialMemberKind, bool, bool, bool, bool, bool) /src/clang/lib/Sema/SemaLookup.cpp:3372:34
    #<!-- -->2 0x78d777c in clang::Sema::LookupDestructor(clang::CXXRecordDecl*) /src/clang/lib/Sema/SemaLookup.cpp:3654:7
    #<!-- -->3 0x78d777c in checkDestructorReference(clang::QualType, clang::SourceLocation, clang::Sema&amp;) /src/clang/lib/Sema/SemaInit.cpp:2016:43
    #<!-- -->4 0x78f926d in (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, llvm::APSInt, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;) /src/clang/lib/Sema/SemaInit.cpp:2065:9
    #<!-- -->5 0x78af603 in (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) /src/clang/lib/Sema/SemaInit.cpp:1420:5
    #<!-- -->6 0x78abefb in (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) /src/clang/lib/Sema/SemaInit.cpp:1306:3
    #<!-- -->7 0x78abefb in (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) /src/clang/lib/Sema/SemaInit.cpp:1060:3
    #<!-- -->8 0x78dafe4 in diagnoseListInit(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8723:19
    #<!-- -->9 0x78db074 in diagnoseListInit(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8715:5
    #<!-- -->10 0x78bd2d8 in clang::InitializationSequence::Diagnose(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;) /src/clang/lib/Sema/SemaInit.cpp:9197:5
    #<!-- -->11 0x78c13db in clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) /src/clang/lib/Sema/SemaInit.cpp:7648:5
    #<!-- -->12 0x74522c3 in clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13606:33
    #<!-- -->13 0x102f6024 in clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2858:17
    #<!-- -->14 0x102eec5d in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
    #<!-- -->15 0x102ec938 in clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:2168:10
    #<!-- -->16 0x102eb6f2 in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp
    #<!-- -->17 0x10385670 in clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /src/clang/lib/Parse/ParseStmt.cpp
    #<!-- -->18 0x103840ae in clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /src/clang/lib/Parse/ParseStmt.cpp:125:20
    #<!-- -->19 0x1038ff6d in clang::Parser::ParseCompoundStatementBody(bool) /src/clang/lib/Parse/ParseStmt.cpp:1267:11
    #<!-- -->20 0x103a2747 in clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /src/clang/lib/Parse/ParseStmt.cpp:2577:21
    #<!-- -->21 0x101fa0be in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /src/clang/lib/Parse/Parser.cpp:1520:10
    #<!-- -->22 0x102f0c66 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2461:17
    #<!-- -->23 0x101f3d71 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
    #<!-- -->24 0x101f2f1f in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1266:12
    #<!-- -->25 0x101f0487 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:1069:14
    #<!-- -->26 0x10334391 in clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /src/clang/lib/Parse/ParseDeclCXX.cpp:264:7
    #<!-- -->27 0x10332774 in clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /src/clang/lib/Parse/ParseDeclCXX.cpp:241:3
    #<!-- -->28 0x102eb159 in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:2077:12
    #<!-- -->29 0x101ef188 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:985:14
    #<!-- -->30 0x101eb489 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /src/clang/lib/Parse/Parser.cpp:758:12
    #<!-- -->31 0x101db4dd in clang::ParseAST(clang::Sema&amp;, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:171:20
    #<!-- -->32 0xfd38d27 in clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1072:3
    #<!-- -->33 0x8de4a6c in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:33
    #<!-- -->34 0x8fa63e7 in clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
    #<!-- -->35 0x8faaba9 in clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
    #<!-- -->36 0x8fa8a1f in llvm::function_ref&lt;void ()&gt;::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->37 0x8fa8a1f in clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
    #<!-- -->38 0x8fb2990 in clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:1462:7
    #<!-- -->39 0x8fb2990 in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
    #<!-- -->40 0x8fb2990 in void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0&gt;(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #<!-- -->41 0x92fc634 in llvm::unique_function&lt;void ()&gt;::operator()() /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #<!-- -->42 0x92fc634 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:101:5
    #<!-- -->43 0x92fc634 in auto void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)::operator()&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) const /src/llvm/include/llvm/Support/thread.h:43:11
    #<!-- -->44 0x92fc634 in auto std::__invoke_impl&lt;void, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(std::__invoke_other, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)&amp;&amp;, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
    #<!-- -->45 0x92fc634 in std::__invoke_result&lt;auto, auto...&gt;::type std::__invoke&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
    #<!-- -->46 0x92fc634 in decltype(auto) std::__apply_impl&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, 0ul&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, std::integer_sequence&lt;unsigned long, 0ul&gt;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
    #<!-- -->47 0x92fc634 in decltype(auto) std::apply&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
    #<!-- -->48 0x92fc634 in void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:41:5
    #<!-- -->49 0x92fc634 in void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:55:5
    #<!-- -->50 0x587695c in asan_thread_start(void*) crtstuff.c
    #<!-- -->51 0x7ffff7b0b7f9 in start_thread (/lib64/libc.so.6+0x897f9) (BuildId: fc46bc419367003d0e4e399cbe22aade4a1ee7be)
    #<!-- -->52 0x7ffff7b9081f in __GI___clone3 (/lib64/libc.so.6+0x10e81f) (BuildId: fc46bc419367003d0e4e399cbe22aade4a1ee7be)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /src/clang/include/clang/AST/DeclBase.h:442:47 in clang::Decl::getLocation() const
Thread T130 created by T0 here:
    #<!-- -->0 0x5860115 in pthread_create (/src/build/bin/clangd+0x5860115) (BuildId: 8b31158490359739)
    #<!-- -->1 0x5dfe9a8 in llvm::llvm_execute_on_thread_impl(void* (*)(void*), void*, std::optional&lt;unsigned int&gt;) /src/llvm/lib/Support/Unix/Threading.inc:96:17
    #<!-- -->2 0x92fc2d1 in llvm::thread::thread&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;(std::optional&lt;unsigned int&gt;, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&amp;) /src/llvm/include/llvm/Support/thread.h:131:12
    #<!-- -->3 0x92fc2d1 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;) /src/clang-tools-extra/clangd/support/Threading.cpp:107:16
    #<!-- -->4 0x8fbbadb in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;) /src/clang-tools-extra/clangd/TUScheduler.cpp:825:12
    #<!-- -->5 0x8fbbadb in clang::clangd::TUScheduler::update(llvm::StringRef, clang::clangd::ParseInputs, clang::clangd::WantDiagnostics) /src/clang-tools-extra/clangd/TUScheduler.cpp:1681:30
    #<!-- -->6 0x8a87efe in clang::clangd::ClangdServer::addDocument(llvm::StringRef, llvm::StringRef, llvm::StringRef, clang::clangd::WantDiagnostics, bool) /src/clang-tools-extra/clangd/ClangdServer.cpp:316:33
    #<!-- -->7 0x89da572 in clang::clangd::ClangdLSPServer::onDocumentDidOpen(clang::clangd::DidOpenTextDocumentParams const&amp;) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:726:11
    #<!-- -->8 0x8a1710f in void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)::operator()(llvm::json::Value) const /src/clang-tools-extra/clangd/LSPBinder.h:153:5
    #<!-- -->9 0x8a16efe in void llvm::detail::UniqueFunctionBase&lt;void, llvm::json::Value&gt;::CallImpl&lt;void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)&gt;(void*, llvm::json::Value&amp;) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #<!-- -->10 0x8a3e812 in llvm::unique_function&lt;void (llvm::json::Value)&gt;::operator()(llvm::json::Value) /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #<!-- -->11 0x8a3e812 in clang::clangd::ClangdLSPServer::MessageHandler::onNotify(llvm::StringRef, llvm::json::Value) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:219:7
    #<!-- -->12 0x8d78847 in clang::clangd::(anonymous namespace)::JSONTransport::handleMessage(llvm::json::Value, clang::clangd::Transport::MessageHandler&amp;) /src/clang-tools-extra/clangd/JSONTransport.cpp:195:18
    #<!-- -->13 0x8d78847 in clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&amp;) /src/clang-tools-extra/clangd/JSONTransport.cpp:119:16
    #<!-- -->14 0x8a49859 in clang::clangd::ClangdLSPServer::run() /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:1741:25
    #<!-- -->15 0x8893149 in clang::clangd::clangdMain(int, char**) /src/clang-tools-extra/clangd/tool/ClangdMain.cpp:1049:28
    #<!-- -->16 0x7ffff7aab5cf in __libc_start_call_main (/lib64/libc.so.6+0x295cf) (BuildId: fc46bc419367003d0e4e399cbe22aade4a1ee7be)

==3794==ABORTING

```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clangd

Author: Henry Chu (henryhchchc)

<details>
Opening a C++ file with the following content leads to a crash in clangd. Compiling this file with clang does not lead to a crash.

```c++
namespace std {
  typedef decltype(sizeof(int)) size_t;

  template &lt;typename E&gt;
  struct initializer_list
  {
    const E *p;
    size_t n;
    initializer_list(const E *p, size_t n) : p(p), n(n) {}
  };

  struct string {
    string(const char *);
  };

  template&lt;typename A, typename B&gt;
  struct pair {
    pair(const A&amp;, const B&amp;);
  };
}
```

Stack trace:
```
I[06:43:53.843] clangd version 20.1.4 (https://github.com/llvm/llvm-project.git ec28b8f9cc7f2ac187d8a617a6d08d5e56f9120e)
I[06:43:53.843] Features: linux+asan
I[06:43:53.843] PID: 3794
I[06:43:53.843] Working directory: /tmp/export/input_118
I[06:43:53.843] argv[0]: /src/build/bin/clangd
I[06:43:53.843] Starting LSP over stdin/stdout
I[06:43:53.844] &lt;-- initialize(0)
I[06:43:53.873] --&gt; reply:initialize(0) 28 ms
Content-Length: 2050

{"id":0,"jsonrpc":"2.0","result":{"capabilities":{"astProvider":true,"callHierarchyProvider":true,"clangdInlayHintsProvider":true,"codeActionProvider":true,"compilationDatabase":{"automaticReload":true},"completionProvider":{"resolveProvider":false,"triggerCharacters":[".","&lt;","&gt;",":","\"","/","*"]},"declarationProvider":true,"definitionProvider":true,"documentFormattingProvider":true,"documentHighlightProvider":true,"documentLinkProvider":{"resolveProvider":false},"documentOnTypeFormattingProvider":{"firstTriggerCharacter":"\n","moreTriggerCharacter":[]},"documentRangeFormattingProvider":true,"documentSymbolProvider":true,"executeCommandProvider":{"commands":["clangd.applyFix","clangd.applyRename","clangd.applyTweak"]},"foldingRangeProvider":true,"hoverProvider":true,"implementationProvider":true,"inactiveRegionsProvider":true,"inlayHintProvider":true,"memoryUsageProvider":true,"referencesProvider":true,"renameProvider":true,"selectionRangeProvider":true,"semanticTokensProvider":{"full":{"delta":true},"legend":{"tokenModifiers":["declaration","definition","deprecated","deduced","readonly","static","abstract","virtual","dependentName","defaultLibrary","usedAsMutableReference","usedAsMutablePointer","constructorOrDestructor","userDefined","functionScope","classScope","fileScope","globalScope"],"tokenTypes":["variable","variable","parameter","function","method","function","property","variable","class","interface","enum","enumMember","type","type","unknown","namespace","typeParameter","concept","type","macro","modifier","operator","bracket","label","comment"]},"range":false},"signatureHelpProvider":{"triggerCharacters":["(",")","{","}","&lt;","&gt;",","]},"standardTypeHierarchyProvider":true,"textDocumentSync":{"change":2,"openClose":true,"save":true},"typeDefinitionProvider":true,"typeHierarchyProvider":true,"workspaceSymbolProvider":true},"serverInfo":{"name":"clangd","version":"clangd version 20.1.4 (https://github.com/llvm/llvm-project.git ec28b8f9cc7f2ac187d8a617a6d08d5e56f9120e) linux+asan x86_64-unknown-linux-gnu"}}}I[06:43:53.874] &lt;-- initialized
I[06:43:53.874] &lt;-- textDocument/didOpen
I[06:43:53.876] Failed to find compilation database for /tmp/export/input_118/workspace/main.hpp
I[06:43:53.876] ASTWorker building file /tmp/export/input_118/workspace/main.hpp version 1 with command clangd fallback
[/tmp/export/input_118/workspace]
/usr/bin/clang -resource-dir=/src/build/lib/clang/20 -- /tmp/export/input_118/workspace/main.hpp
I[06:43:53.899] Built preamble of size 275484 for file /tmp/export/input_118/workspace/main.hpp version 1 in 0.02 seconds
I[06:43:53.900] --&gt; workspace/semanticTokens/refresh(0)
Content-Length: 82

{"id":0,"jsonrpc":"2.0","method":"workspace/semanticTokens/refresh","params":null}I[06:43:53.904] Indexing c++17 standard library in the context of /tmp/export/input_118/workspace/main.hpp
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      /tmp/export/input_118/workspace/main.hpp:135:31: current parser token ';'
1.      /tmp/export/input_118/workspace/main.hpp:128:1: parsing namespace 'rdar13395022'
2.      /tmp/export/input_118/workspace/main.hpp:131:26: parsing function body 'rdar13395022::test'
3.      /tmp/export/input_118/workspace/main.hpp:131:26: in compound statement ('{}')
 #<!-- -->0 0x000000000582426a ___interceptor_backtrace (/src/build/bin/clangd+0x582426a)
 #<!-- -->1 0x0000000005dfcc27 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /src/llvm/lib/Support/Unix/Signals.inc:799:13
 #<!-- -->2 0x0000000005dfbfec llvm::sys::RunSignalHandlers() /src/llvm/lib/Support/Signals.cpp:0:5
 #<!-- -->3 0x0000000005dfbfec SignalHandler(int) /src/llvm/lib/Support/Unix/Signals.inc:405:3
 #<!-- -->4 0x00007ffff7ac0730 __restore_rt (/lib64/libc.so.6+0x3e730)
 #<!-- -->5 0x000000000706a07e clang::Decl::getLocation() const /src/clang/include/clang/AST/DeclBase.h:442:47
 #<!-- -->6 0x000000000706a07e clang::Sema::LookupSpecialMember(clang::CXXRecordDecl*, clang::CXXSpecialMemberKind, bool, bool, bool, bool, bool) /src/clang/lib/Sema/SemaLookup.cpp:3372:34
 #<!-- -->7 0x00000000078d777d clang::Sema::LookupDestructor(clang::CXXRecordDecl*) /src/clang/lib/Sema/SemaLookup.cpp:3654:7
 #<!-- -->8 0x00000000078d777d checkDestructorReference(clang::QualType, clang::SourceLocation, clang::Sema&amp;) /src/clang/lib/Sema/SemaInit.cpp:2016:43
 #<!-- -->9 0x00000000078f926e (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, llvm::APSInt, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;) /src/clang/lib/Sema/SemaInit.cpp:2065:9
#<!-- -->10 0x00000000078af604 (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) /src/clang/lib/Sema/SemaInit.cpp:1420:5
#<!-- -->11 0x00000000078abefc (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) /src/clang/lib/Sema/SemaInit.cpp:1306:3
#<!-- -->12 0x00000000078abefc (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) /src/clang/lib/Sema/SemaInit.cpp:1060:3
#<!-- -->13 0x00000000078dafe5 diagnoseListInit(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8723:19
#<!-- -->14 0x00000000078db075 diagnoseListInit(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8715:5
#<!-- -->15 0x00000000078bd2d9 clang::InitializationSequence::Diagnose(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;) /src/clang/lib/Sema/SemaInit.cpp:0:5
#<!-- -->16 0x00000000078c13dc clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) /src/clang/lib/Sema/SemaInit.cpp:0:5
#<!-- -->17 0x00000000074522c4 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13606:33
#<!-- -->18 0x00000000102f6025 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:0:17
#<!-- -->19 0x00000000102eec5e clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
#<!-- -->20 0x00000000102ec939 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:2168:10
#<!-- -->21 0x00000000102eb6f3 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:0:14
#<!-- -->22 0x0000000010385671 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /src/clang/lib/Parse/ParseStmt.cpp:0:0
#<!-- -->23 0x00000000103840af clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /src/clang/lib/Parse/ParseStmt.cpp:0:20
#<!-- -->24 0x000000001038ff6e clang::Parser::ParseCompoundStatementBody(bool) /src/clang/lib/Parse/ParseStmt.cpp:1267:11
#<!-- -->25 0x00000000103a2748 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /src/clang/lib/Parse/ParseStmt.cpp:2577:21
#<!-- -->26 0x00000000101fa0bf clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /src/clang/lib/Parse/Parser.cpp:0:10
#<!-- -->27 0x00000000102f0c67 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:0:17
#<!-- -->28 0x00000000101f3d72 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
#<!-- -->29 0x00000000101f2f20 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:0:12
#<!-- -->30 0x00000000101f0488 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:0:14
#<!-- -->31 0x0000000010334392 clang::AttributePool::~AttributePool() /src/clang/include/clang/Sema/ParsedAttr.h:724:22
#<!-- -->32 0x0000000010334392 clang::ParsedAttributes::~ParsedAttributes() /src/clang/include/clang/Sema/ParsedAttr.h:956:7
#<!-- -->33 0x0000000010334392 clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /src/clang/lib/Parse/ParseDeclCXX.cpp:265:5
#<!-- -->34 0x0000000010332775 clang::Parser::ParseScope::Exit() /src/clang/include/clang/Parse/Parser.h:1197:15
#<!-- -->35 0x0000000010332775 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /src/clang/lib/Parse/ParseDeclCXX.cpp:244:18
#<!-- -->36 0x00000000102eb15a clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:0:0
#<!-- -->37 0x00000000101ef189 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:0:14
#<!-- -->38 0x00000000101eb48a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /src/clang/lib/Parse/Parser.cpp:0:12
#<!-- -->39 0x00000000101db4de clang::ParseAST(clang::Sema&amp;, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:170:5
#<!-- -->40 0x000000000fd38d28 clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /src/clang/lib/Frontend/CompilerInstance.cpp:86:11
#<!-- -->41 0x000000000fd38d28 clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1076:10
#<!-- -->42 0x0000000008de4a6d llvm::Error::getPtr() const /src/llvm/include/llvm/Support/Error.h:282:12
#<!-- -->43 0x0000000008de4a6d llvm::Error::operator bool() /src/llvm/include/llvm/Support/Error.h:242:16
#<!-- -->44 0x0000000008de4a6d clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:19
#<!-- -->45 0x0000000008fa63e8 clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
#<!-- -->46 0x0000000008faabaa void std::_Destroy_aux&lt;false&gt;::__destroy&lt;clang::clangd::Diag*&gt;(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:162:19
#<!-- -->47 0x0000000008faabaa void std::_Destroy&lt;clang::clangd::Diag*&gt;(clang::clangd::Diag*, clang::clangd::Diag*) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_construct.h:195:7
#<!-- -->48 0x0000000008faabaa void std::_Destroy&lt;clang::clangd::Diag*, clang::clangd::Diag&gt;(clang::clangd::Diag*, clang::clangd::Diag*, std::allocator&lt;clang::clangd::Diag&gt;&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/alloc_traits.h:948:7
#<!-- -->49 0x0000000008faabaa std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;::~vector() /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/stl_vector.h:735:2
#<!-- -->50 0x0000000008faabaa clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
#<!-- -->51 0x0000000008fa8a20 llvm::function_ref&lt;void ()&gt;::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:0:12
#<!-- -->52 0x0000000008fa8a20 clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
#<!-- -->53 0x0000000008fb2991 clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:0:7
#<!-- -->54 0x0000000008fb2991 clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
#<!-- -->55 0x0000000008fb2991 void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0&gt;(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
#<!-- -->56 0x00000000092fc635 clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:103:14
#<!-- -->57 0x00000000092fc635 auto void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)::operator()&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) const /src/llvm/include/llvm/Support/thread.h:43:11
#<!-- -->58 0x00000000092fc635 auto std::__invoke_impl&lt;void, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(std::__invoke_other, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)&amp;&amp;, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
#<!-- -->59 0x00000000092fc635 std::__invoke_result&lt;auto, auto...&gt;::type std::__invoke&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
#<!-- -->60 0x00000000092fc635 decltype(auto) std::__apply_impl&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, 0ul&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, std::integer_sequence&lt;unsigned long, 0ul&gt;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
#<!-- -->61 0x00000000092fc635 decltype(auto) std::apply&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
#<!-- -->62 0x00000000092fc635 void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:41:5
#<!-- -->63 0x00000000092fc635 void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:55:5
#<!-- -->64 0x000000000587695d asan_thread_start(void*) crtstuff.c:0:0
#<!-- -->65 0x00007ffff7b0b7fa start_thread (/lib64/libc.so.6+0x897fa)
#<!-- -->66 0x00007ffff7b90820 __GI___clone3 (/lib64/libc.so.6+0x10e820)
Signalled during AST worker action: Build AST
  Filename: main.hpp
  Directory: /tmp/export/input_118/workspace
  Command Line: /usr/bin/clang -resource-dir=/src/build/lib/clang/20 -- /tmp/export/input_118/workspace/main.hpp
  Version: 1
AddressSanitizer:DEADLYSIGNAL
=================================================================
==3794==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000018 (pc 0x00000706a07e bp 0x7fff5a6e1740 sp 0x7fff5a6e15a0 T130)
==3794==The signal is caused by a READ memory access.
==3794==Hint: address points to the zero page.
    #<!-- -->0 0x706a07e in clang::Decl::getLocation() const /src/clang/include/clang/AST/DeclBase.h:442:47
    #<!-- -->1 0x706a07e in clang::Sema::LookupSpecialMember(clang::CXXRecordDecl*, clang::CXXSpecialMemberKind, bool, bool, bool, bool, bool) /src/clang/lib/Sema/SemaLookup.cpp:3372:34
    #<!-- -->2 0x78d777c in clang::Sema::LookupDestructor(clang::CXXRecordDecl*) /src/clang/lib/Sema/SemaLookup.cpp:3654:7
    #<!-- -->3 0x78d777c in checkDestructorReference(clang::QualType, clang::SourceLocation, clang::Sema&amp;) /src/clang/lib/Sema/SemaInit.cpp:2016:43
    #<!-- -->4 0x78f926d in (anonymous namespace)::InitListChecker::CheckArrayType(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, llvm::APSInt, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;) /src/clang/lib/Sema/SemaInit.cpp:2065:9
    #<!-- -->5 0x78af603 in (anonymous namespace)::InitListChecker::CheckListElementTypes(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, unsigned int&amp;, clang::InitListExpr*, unsigned int&amp;, bool) /src/clang/lib/Sema/SemaInit.cpp:1420:5
    #<!-- -->6 0x78abefb in (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) /src/clang/lib/Sema/SemaInit.cpp:1306:3
    #<!-- -->7 0x78abefb in (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) /src/clang/lib/Sema/SemaInit.cpp:1060:3
    #<!-- -->8 0x78dafe4 in diagnoseListInit(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8723:19
    #<!-- -->9 0x78db074 in diagnoseListInit(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*) /src/clang/lib/Sema/SemaInit.cpp:8715:5
    #<!-- -->10 0x78bd2d8 in clang::InitializationSequence::Diagnose(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;) /src/clang/lib/Sema/SemaInit.cpp:9197:5
    #<!-- -->11 0x78c13db in clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) /src/clang/lib/Sema/SemaInit.cpp:7648:5
    #<!-- -->12 0x74522c3 in clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) /src/clang/lib/Sema/SemaDecl.cpp:13606:33
    #<!-- -->13 0x102f6024 in clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2858:17
    #<!-- -->14 0x102eec5d in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2551:7
    #<!-- -->15 0x102ec938 in clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:2168:10
    #<!-- -->16 0x102eb6f2 in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp
    #<!-- -->17 0x10385670 in clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /src/clang/lib/Parse/ParseStmt.cpp
    #<!-- -->18 0x103840ae in clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /src/clang/lib/Parse/ParseStmt.cpp:125:20
    #<!-- -->19 0x1038ff6d in clang::Parser::ParseCompoundStatementBody(bool) /src/clang/lib/Parse/ParseStmt.cpp:1267:11
    #<!-- -->20 0x103a2747 in clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /src/clang/lib/Parse/ParseStmt.cpp:2577:21
    #<!-- -->21 0x101fa0be in clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /src/clang/lib/Parse/Parser.cpp:1520:10
    #<!-- -->22 0x102f0c66 in clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /src/clang/lib/Parse/ParseDecl.cpp:2461:17
    #<!-- -->23 0x101f3d71 in clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1244:10
    #<!-- -->24 0x101f2f1f in clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /src/clang/lib/Parse/Parser.cpp:1266:12
    #<!-- -->25 0x101f0487 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:1069:14
    #<!-- -->26 0x10334391 in clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /src/clang/lib/Parse/ParseDeclCXX.cpp:264:7
    #<!-- -->27 0x10332774 in clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /src/clang/lib/Parse/ParseDeclCXX.cpp:241:3
    #<!-- -->28 0x102eb159 in clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /src/clang/lib/Parse/ParseDecl.cpp:2077:12
    #<!-- -->29 0x101ef188 in clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /src/clang/lib/Parse/Parser.cpp:985:14
    #<!-- -->30 0x101eb489 in clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /src/clang/lib/Parse/Parser.cpp:758:12
    #<!-- -->31 0x101db4dd in clang::ParseAST(clang::Sema&amp;, bool, bool) /src/clang/lib/Parse/ParseAST.cpp:171:20
    #<!-- -->32 0xfd38d27 in clang::FrontendAction::Execute() /src/clang/lib/Frontend/FrontendAction.cpp:1072:3
    #<!-- -->33 0x8de4a6c in clang::clangd::ParsedAST::build(llvm::StringRef, clang::clangd::ParseInputs const&amp;, std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, llvm::ArrayRef&lt;clang::clangd::Diag&gt;, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;) /src/clang-tools-extra/clangd/ParsedAST.cpp:713:33
    #<!-- -->34 0x8fa63e7 in clang::clangd::(anonymous namespace)::ASTWorker::generateDiagnostics(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1214:39
    #<!-- -->35 0x8faaba9 in clang::clangd::(anonymous namespace)::ASTWorker::updatePreamble(std::unique_ptr&lt;clang::CompilerInvocation, std::default_delete&lt;clang::CompilerInvocation&gt;&gt;, clang::clangd::ParseInputs, std::shared_ptr&lt;clang::clangd::PreambleData const&gt;, std::vector&lt;clang::clangd::Diag, std::allocator&lt;clang::clangd::Diag&gt;&gt;, clang::clangd::WantDiagnostics)::$_0::operator()() /src/clang-tools-extra/clangd/TUScheduler.cpp:1148:5
    #<!-- -->36 0x8fa8a1f in llvm::function_ref&lt;void ()&gt;::operator()() const /src/llvm/include/llvm/ADT/STLFunctionalExtras.h:69:12
    #<!-- -->37 0x8fa8a1f in clang::clangd::(anonymous namespace)::ASTWorker::runTask(llvm::StringRef, llvm::function_ref&lt;void ()&gt;) /src/clang-tools-extra/clangd/TUScheduler.cpp:1328:3
    #<!-- -->38 0x8fb2990 in clang::clangd::(anonymous namespace)::ASTWorker::run() /src/clang-tools-extra/clangd/TUScheduler.cpp:1462:7
    #<!-- -->39 0x8fb2990 in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0::operator()() const /src/clang-tools-extra/clangd/TUScheduler.cpp:826:42
    #<!-- -->40 0x8fb2990 in void llvm::detail::UniqueFunctionBase&lt;void&gt;::CallImpl&lt;clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;)::$_0&gt;(void*) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #<!-- -->41 0x92fc634 in llvm::unique_function&lt;void ()&gt;::operator()() /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #<!-- -->42 0x92fc634 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1::operator()() /src/clang-tools-extra/clangd/support/Threading.cpp:101:5
    #<!-- -->43 0x92fc634 in auto void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)::operator()&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) const /src/llvm/include/llvm/Support/thread.h:43:11
    #<!-- -->44 0x92fc634 in auto std::__invoke_impl&lt;void, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(std::__invoke_other, void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...)&amp;&amp;, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:61:14
    #<!-- -->45 0x92fc634 in std::__invoke_result&lt;auto, auto...&gt;::type std::__invoke&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&gt;(auto&amp;&amp;, auto&amp;&amp;...) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/bits/invoke.h:96:14
    #<!-- -->46 0x92fc634 in decltype(auto) std::__apply_impl&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, 0ul&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;, std::integer_sequence&lt;unsigned long, 0ul&gt;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2302:14
    #<!-- -->47 0x92fc634 in decltype(auto) std::apply&lt;void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*)::'lambda'(auto&amp;&amp;, auto&amp;&amp;...), std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;&gt;(auto&amp;&amp;, std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&amp;) /opt/rh/gcc-toolset-13/root/usr/lib/gcc/x86_64-redhat-linux/13/../../../../include/c++/13/tuple:2313:14
    #<!-- -->48 0x92fc634 in void llvm::thread::GenericThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:41:5
    #<!-- -->49 0x92fc634 in void* llvm::thread::ThreadProxy&lt;std::tuple&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;&gt;(void*) /src/llvm/include/llvm/Support/thread.h:55:5
    #<!-- -->50 0x587695c in asan_thread_start(void*) crtstuff.c
    #<!-- -->51 0x7ffff7b0b7f9 in start_thread (/lib64/libc.so.6+0x897f9) (BuildId: fc46bc419367003d0e4e399cbe22aade4a1ee7be)
    #<!-- -->52 0x7ffff7b9081f in __GI___clone3 (/lib64/libc.so.6+0x10e81f) (BuildId: fc46bc419367003d0e4e399cbe22aade4a1ee7be)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV /src/clang/include/clang/AST/DeclBase.h:442:47 in clang::Decl::getLocation() const
Thread T130 created by T0 here:
    #<!-- -->0 0x5860115 in pthread_create (/src/build/bin/clangd+0x5860115) (BuildId: 8b31158490359739)
    #<!-- -->1 0x5dfe9a8 in llvm::llvm_execute_on_thread_impl(void* (*)(void*), void*, std::optional&lt;unsigned int&gt;) /src/llvm/lib/Support/Unix/Threading.inc:96:17
    #<!-- -->2 0x92fc2d1 in llvm::thread::thread&lt;clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&gt;(std::optional&lt;unsigned int&gt;, clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;)::$_1&amp;&amp;) /src/llvm/include/llvm/Support/thread.h:131:12
    #<!-- -->3 0x92fc2d1 in clang::clangd::AsyncTaskRunner::runAsync(llvm::Twine const&amp;, llvm::unique_function&lt;void ()&gt;) /src/clang-tools-extra/clangd/support/Threading.cpp:107:16
    #<!-- -->4 0x8fbbadb in clang::clangd::(anonymous namespace)::ASTWorker::create(llvm::StringRef, clang::clangd::GlobalCompilationDatabase const&amp;, clang::clangd::TUScheduler::ASTCache&amp;, clang::clangd::TUScheduler::HeaderIncluderCache&amp;, clang::clangd::AsyncTaskRunner*, clang::clangd::Semaphore&amp;, clang::clangd::TUScheduler::Options const&amp;, clang::clangd::ParsingCallbacks&amp;) /src/clang-tools-extra/clangd/TUScheduler.cpp:825:12
    #<!-- -->5 0x8fbbadb in clang::clangd::TUScheduler::update(llvm::StringRef, clang::clangd::ParseInputs, clang::clangd::WantDiagnostics) /src/clang-tools-extra/clangd/TUScheduler.cpp:1681:30
    #<!-- -->6 0x8a87efe in clang::clangd::ClangdServer::addDocument(llvm::StringRef, llvm::StringRef, llvm::StringRef, clang::clangd::WantDiagnostics, bool) /src/clang-tools-extra/clangd/ClangdServer.cpp:316:33
    #<!-- -->7 0x89da572 in clang::clangd::ClangdLSPServer::onDocumentDidOpen(clang::clangd::DidOpenTextDocumentParams const&amp;) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:726:11
    #<!-- -->8 0x8a1710f in void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)::operator()(llvm::json::Value) const /src/clang-tools-extra/clangd/LSPBinder.h:153:5
    #<!-- -->9 0x8a16efe in void llvm::detail::UniqueFunctionBase&lt;void, llvm::json::Value&gt;::CallImpl&lt;void clang::clangd::LSPBinder::notification&lt;clang::clangd::DidOpenTextDocumentParams, clang::clangd::ClangdLSPServer&gt;(llvm::StringLiteral, clang::clangd::ClangdLSPServer*, void (clang::clangd::ClangdLSPServer::*)(clang::clangd::DidOpenTextDocumentParams const&amp;))::'lambda'(llvm::json::Value)&gt;(void*, llvm::json::Value&amp;) /src/llvm/include/llvm/ADT/FunctionExtras.h:222:12
    #<!-- -->10 0x8a3e812 in llvm::unique_function&lt;void (llvm::json::Value)&gt;::operator()(llvm::json::Value) /src/llvm/include/llvm/ADT/FunctionExtras.h:387:12
    #<!-- -->11 0x8a3e812 in clang::clangd::ClangdLSPServer::MessageHandler::onNotify(llvm::StringRef, llvm::json::Value) /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:219:7
    #<!-- -->12 0x8d78847 in clang::clangd::(anonymous namespace)::JSONTransport::handleMessage(llvm::json::Value, clang::clangd::Transport::MessageHandler&amp;) /src/clang-tools-extra/clangd/JSONTransport.cpp:195:18
    #<!-- -->13 0x8d78847 in clang::clangd::(anonymous namespace)::JSONTransport::loop(clang::clangd::Transport::MessageHandler&amp;) /src/clang-tools-extra/clangd/JSONTransport.cpp:119:16
    #<!-- -->14 0x8a49859 in clang::clangd::ClangdLSPServer::run() /src/clang-tools-extra/clangd/ClangdLSPServer.cpp:1741:25
    #<!-- -->15 0x8893149 in clang::clangd::clangdMain(int, char**) /src/clang-tools-extra/clangd/tool/ClangdMain.cpp:1049:28
    #<!-- -->16 0x7ffff7aab5cf in __libc_start_call_main (/lib64/libc.so.6+0x295cf) (BuildId: fc46bc419367003d0e4e399cbe22aade4a1ee7be)

==3794==ABORTING

```
</details>


---

### Comment 3 - frederick-vs-ja

Is this reduced from some actual use of the standard library?

---

### Comment 4 - henryhchchc

> Is this reduced from some actual use of the standard library?

It was generated by a fuzzer for LSP servers. We adopted some real-world code as seeds.

---

### Comment 5 - henryhchchc

Sorry I made a mistake when reproducing the crash. I updated the description with the correct one.

---

