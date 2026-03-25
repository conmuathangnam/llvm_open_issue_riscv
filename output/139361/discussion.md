# Crash when compiling clc++2021 to spirv64

**Author:** tibetiroka
**URL:** https://github.com/llvm/llvm-project/issues/139361

## Body

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
<details>
<summary>Stack dump:</summary>

```
0.	Program arguments: clang++ -x clcpp -cl-std=clc++2021 -target spirv64 -emit-llvm -O0 -Wno-everything -c vector_add.clcpp -o vector_add.bc -Xclang -fdeclare-opencl-builtins -Xclang -finclude-default-header -Ilibcxx_opencl -Ilibcxx_opencl/x86_64-pc-linux-gnu/
1.	libcxx_opencl/optional:101:5 <Spelling=libcxx_opencl/x86_64-pc-linux-gnu/bits/c++config.h:261:72>: current parser token ')'
2.	libcxx_opencl/optional:59:1: parsing namespace 'std'
3.	libcxx_opencl/optional:101:3: parsing function body 'std::__throw_bad_optional_access'
4.	libcxx_opencl/optional:101:3: in compound statement ('{}')
 #0 0x0000787ff3619920 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
 #1 0x0000787ff3616df5 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
 #2 0x0000787ff3616df5 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:367:31
 #3 0x0000787ff3500459 HandleCrash /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #4 0x0000787ff3500459 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #5 0x0000787ff284def0 (/usr/lib/libc.so.6+0x3def0)
 #6 0x0000787ffe192034 clang::ASTContext::getTypeDeclType(clang::TypeDecl const*, clang::TypeDecl const*) const (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/ASTContext.h:1637:0
 #7 0x0000787ffc9436f8 isNonPlacementDeallocationFunction(clang::Sema&, clang::FunctionDecl*) (.part.0.lto_priv.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1750:0
 #8 0x0000787ffc8cb4ef isNonPlacementDeallocationFunction /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1737:0
 #9 0x0000787ffc8cb4ef resolveDeallocationOverload /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1844:0
#10 0x0000787ffc8d1860 clang::LookupResult::~LookupResult() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/Lookup.h:247:0
#11 0x0000787ffc8d1860 clang::Sema::FindUsualDeallocationFunction(clang::SourceLocation, bool, bool, clang::DeclarationName) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:3271:0
#12 0x0000787ffc75cedb clang::Sema::CheckDestructor(clang::CXXDestructorDecl*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaDeclCXX.cpp:10853:50
#13 0x0000787ffc77bc72 clang::Sema::DefineImplicitDestructor(clang::SourceLocation, clang::CXXDestructorDecl*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaDeclCXX.cpp:14209:0
#14 0x0000787ffc841a3b operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:17957:0
#15 0x0000787ffc841a3b callback_fn<clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::<lambda()> > /usr/include/llvm/ADT/STLFunctionalExtras.h:45:0
#16 0x0000787ffc4eeebd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:569:1
#17 0x0000787ffc82375c clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:17933:0
#18 0x0000787ffc943f2b llvm::detail::PunnedPointer<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>>::PunnedPointer(long) /usr/include/llvm/ADT/PointerIntPair.h:37:0
#19 0x0000787ffc943f2b llvm::PointerIntPair<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>, 3u, unsigned int, llvm::PointerLikeTypeTraits<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>>, llvm::PointerIntPairInfo<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>, 3u, llvm::PointerLikeTypeTraits<llvm::PointerUnion<clang::Type const*, clang::ExtQuals const*>>>>::PointerIntPair() /usr/include/llvm/ADT/PointerIntPair.h:86:0
#20 0x0000787ffc943f2b clang::QualType::QualType() /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:964:0
#21 0x0000787ffc943f2b clang::Sema::MaybeBindToTemporary(clang::Expr*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:7631:0
#22 0x0000787ffc97d07f PerformConstructorInitialization /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaInit.cpp:7294:0
#23 0x0000787ffc9818d5 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaInit.cpp:7696:0
#24 0x0000787ffc8ca553 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1660:0
#25 0x0000787ffc8ca921 clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1525:0
#26 0x0000787ffbc3d2d1 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExprCXX.cpp:2069:45
#27 0x0000787ffbc409c4 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1686:3
#28 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#29 0x0000787ffe0dd22d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:182:0
#30 0x0000787ffe0dd3c5 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:134:0
#31 0x0000787ffbc5142d clang::Parser::ParseCXXAmbiguousParenExpression(clang::Parser::ParenParseOption&, clang::OpaquePtr<clang::QualType>&, clang::BalancedDelimiterTracker&, clang::ColonProtectionRAIIObject&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExprCXX.cpp:4184:27
#32 0x0000787ffbc52dbc clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:3152:37
#33 0x0000787ffbc3fb9b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1099:5
#34 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#35 0x0000787ffbc51b9d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:3268:9
#36 0x0000787ffbc3fb9b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1099:5
#37 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#38 0x0000787ffe0dd22d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:182:0
#39 0x0000787ffbc1c14a clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:575:38
#40 0x0000787ffbc5203d clang::Parser::getLangOpts() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:495:51
#41 0x0000787ffbc5203d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:3337:21
#42 0x0000787ffbc3fb9b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1099:5
#43 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#44 0x0000787ffe0dd22d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:182:0
#45 0x0000787ffe0dd3c5 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:134:0
#46 0x0000787ffbc94d30 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:555:3
#47 0x0000787ffbc907b6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:292:40
#48 0x0000787ffbc90ac7 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:328:5
#49 0x0000787ffbc90ac7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:126:26
#50 0x0000787ffbc98dba clang::Parser::ParseCompoundStatementBody(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:1248:38
#51 0x0000787ffbc9c5cc clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:2532:3
#52 0x0000787ffbcbe52f clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1525:36
#53 0x0000787ffbbef35f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDecl.cpp:2427:0
#54 0x0000787ffbcbb808 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1249:73
#55 0x0000787ffbcbc1ea clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1271:42
#56 0x0000787ffbcccca2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1074:50
#57 0x0000787ffbc2324f clang::AttributePool::~AttributePool() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/ParsedAttr.h:726:41
#58 0x0000787ffbc2324f clang::ParsedAttributes::~ParsedAttributes() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/ParsedAttr.h:958:7
#59 0x0000787ffbc2324f clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDeclCXX.cpp:273:5
#60 0x0000787ffbc2f07d clang::Parser::ParseScope::Exit() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:1197:24
#61 0x0000787ffbc2f07d clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDeclCXX.cpp:252:22
#62 0x0000787ffbbfb168 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDecl.cpp:2044:0
#63 0x0000787ffbccce95 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1022:32
#64 0x0000787ffbccd9a5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:763:36
#65 0x0000787ffbbd9b18 clang::ParseAST(clang::Sema&, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseAST.cpp:170:69
#66 0x0000787ffd9021a1 clang::FrontendAction::Execute() /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/FrontendAction.cpp:1078:21
#67 0x0000787ffd89808f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:282:12
#68 0x0000787ffd89808f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:242:22
#69 0x0000787ffd89808f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/CompilerInstance.cpp:1061:42
#70 0x0000787ffd95ce85 std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#71 0x0000787ffd95ce85 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#72 0x0000787ffd95ce85 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#73 0x0000787ffd95ce85 clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInvocation.h:259:0
#74 0x0000787ffd95ce85 clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInstance.h:312:0
#75 0x0000787ffd95ce85 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-19.1.7.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:281:0
#76 0x00005bf1e82f1832 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#77 0x00005bf1e82f604e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#78 0x0000787ffd56c915 operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:440:32
#79 0x0000787ffd56c915 callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::string*, bool*) const::<lambda()> > /usr/include/llvm/ADT/STLFunctionalExtras.h:45:52
#80 0x0000787ff35008aa llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:428:1
#81 0x0000787ffd5738c6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:444:10
#82 0x0000787ffd529bc9 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:200:3
#83 0x0000787ffd529fbc clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:253:5
#84 0x0000787ffd59a724 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:95:46
#85 0x0000787ffd59a724 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Driver.cpp:1946:28
#86 0x00005bf1e82f7d8c llvm::SmallVectorBase<unsigned int>::size() const /usr/include/llvm/ADT/SmallVector.h:92:32
#87 0x00005bf1e82f7d8c llvm::SmallVectorTemplateCommon<std::pair<int, clang::driver::Command const*>, void>::end() /usr/include/llvm/ADT/SmallVector.h:283:41
#88 0x00005bf1e82f7d8c clang_main(int, char**, llvm::ToolContext const&) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:393:26
#89 0x00005bf1e82e9f65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#90 0x0000787ff28376b5 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#91 0x0000787ff2837769 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#92 0x0000787ff2837769 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#93 0x00005bf1e82e9fc5 _start (/usr/bin/clang+++0xafc5)
```
</details>

clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.7
Target: spirv64
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: [/tmp/vector_add-dca0ad.clcpp](https://github.com/user-attachments/files/20137312/vector_add-dca0ad.txt)
clang++: note: diagnostic msg: [/tmp/vector_add-dca0ad.sh](https://github.com/user-attachments/files/20137308/vector_add-dca0ad.txt)
clang++: note: diagnostic msg: 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (tibetiroka)

<details>
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
&lt;details&gt;
&lt;summary&gt;Stack dump:&lt;/summary&gt;

```
0.	Program arguments: clang++ -x clcpp -cl-std=clc++2021 -target spirv64 -emit-llvm -O0 -Wno-everything -c vector_add.clcpp -o vector_add.bc -Xclang -fdeclare-opencl-builtins -Xclang -finclude-default-header -Ilibcxx_opencl -Ilibcxx_opencl/x86_64-pc-linux-gnu/
1.	libcxx_opencl/optional:101:5 &lt;Spelling=libcxx_opencl/x86_64-pc-linux-gnu/bits/c++config.h:261:72&gt;: current parser token ')'
2.	libcxx_opencl/optional:59:1: parsing namespace 'std'
3.	libcxx_opencl/optional:101:3: parsing function body 'std::__throw_bad_optional_access'
4.	libcxx_opencl/optional:101:3: in compound statement ('{}')
 #<!-- -->0 0x0000787ff3619920 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
 #<!-- -->1 0x0000787ff3616df5 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
 #<!-- -->2 0x0000787ff3616df5 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:367:31
 #<!-- -->3 0x0000787ff3500459 HandleCrash /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->4 0x0000787ff3500459 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #<!-- -->5 0x0000787ff284def0 (/usr/lib/libc.so.6+0x3def0)
 #<!-- -->6 0x0000787ffe192034 clang::ASTContext::getTypeDeclType(clang::TypeDecl const*, clang::TypeDecl const*) const (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/ASTContext.h:1637:0
 #<!-- -->7 0x0000787ffc9436f8 isNonPlacementDeallocationFunction(clang::Sema&amp;, clang::FunctionDecl*) (.part.0.lto_priv.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1750:0
 #<!-- -->8 0x0000787ffc8cb4ef isNonPlacementDeallocationFunction /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1737:0
 #<!-- -->9 0x0000787ffc8cb4ef resolveDeallocationOverload /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1844:0
#<!-- -->10 0x0000787ffc8d1860 clang::LookupResult::~LookupResult() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/Lookup.h:247:0
#<!-- -->11 0x0000787ffc8d1860 clang::Sema::FindUsualDeallocationFunction(clang::SourceLocation, bool, bool, clang::DeclarationName) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:3271:0
#<!-- -->12 0x0000787ffc75cedb clang::Sema::CheckDestructor(clang::CXXDestructorDecl*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaDeclCXX.cpp:10853:50
#<!-- -->13 0x0000787ffc77bc72 clang::Sema::DefineImplicitDestructor(clang::SourceLocation, clang::CXXDestructorDecl*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaDeclCXX.cpp:14209:0
#<!-- -->14 0x0000787ffc841a3b operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:17957:0
#<!-- -->15 0x0000787ffc841a3b callback_fn&lt;clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:45:0
#<!-- -->16 0x0000787ffc4eeebd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:569:1
#<!-- -->17 0x0000787ffc82375c clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:17933:0
#<!-- -->18 0x0000787ffc943f2b llvm::detail::PunnedPointer&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;::PunnedPointer(long) /usr/include/llvm/ADT/PointerIntPair.h:37:0
#<!-- -->19 0x0000787ffc943f2b llvm::PointerIntPair&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;, 3u, unsigned int, llvm::PointerLikeTypeTraits&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;, llvm::PointerIntPairInfo&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;, 3u, llvm::PointerLikeTypeTraits&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;&gt;&gt;::PointerIntPair() /usr/include/llvm/ADT/PointerIntPair.h:86:0
#<!-- -->20 0x0000787ffc943f2b clang::QualType::QualType() /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:964:0
#<!-- -->21 0x0000787ffc943f2b clang::Sema::MaybeBindToTemporary(clang::Expr*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:7631:0
#<!-- -->22 0x0000787ffc97d07f PerformConstructorInitialization /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaInit.cpp:7294:0
#<!-- -->23 0x0000787ffc9818d5 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaInit.cpp:7696:0
#<!-- -->24 0x0000787ffc8ca553 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1660:0
#<!-- -->25 0x0000787ffc8ca921 clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1525:0
#<!-- -->26 0x0000787ffbc3d2d1 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExprCXX.cpp:2069:45
#<!-- -->27 0x0000787ffbc409c4 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1686:3
#<!-- -->28 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#<!-- -->29 0x0000787ffe0dd22d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:182:0
#<!-- -->30 0x0000787ffe0dd3c5 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:134:0
#<!-- -->31 0x0000787ffbc5142d clang::Parser::ParseCXXAmbiguousParenExpression(clang::Parser::ParenParseOption&amp;, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::BalancedDelimiterTracker&amp;, clang::ColonProtectionRAIIObject&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExprCXX.cpp:4184:27
#<!-- -->32 0x0000787ffbc52dbc clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:3152:37
#<!-- -->33 0x0000787ffbc3fb9b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1099:5
#<!-- -->34 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#<!-- -->35 0x0000787ffbc51b9d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:3268:9
#<!-- -->36 0x0000787ffbc3fb9b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1099:5
#<!-- -->37 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#<!-- -->38 0x0000787ffe0dd22d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:182:0
#<!-- -->39 0x0000787ffbc1c14a clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:575:38
#<!-- -->40 0x0000787ffbc5203d clang::Parser::getLangOpts() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:495:51
#<!-- -->41 0x0000787ffbc5203d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:3337:21
#<!-- -->42 0x0000787ffbc3fb9b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1099:5
#<!-- -->43 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#<!-- -->44 0x0000787ffe0dd22d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:182:0
#<!-- -->45 0x0000787ffe0dd3c5 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:134:0
#<!-- -->46 0x0000787ffbc94d30 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:555:3
#<!-- -->47 0x0000787ffbc907b6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:292:40
#<!-- -->48 0x0000787ffbc90ac7 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:328:5
#<!-- -->49 0x0000787ffbc90ac7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:126:26
#<!-- -->50 0x0000787ffbc98dba clang::Parser::ParseCompoundStatementBody(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:1248:38
#<!-- -->51 0x0000787ffbc9c5cc clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:2532:3
#<!-- -->52 0x0000787ffbcbe52f clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1525:36
#<!-- -->53 0x0000787ffbbef35f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDecl.cpp:2427:0
#<!-- -->54 0x0000787ffbcbb808 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1249:73
#<!-- -->55 0x0000787ffbcbc1ea clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1271:42
#<!-- -->56 0x0000787ffbcccca2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1074:50
#<!-- -->57 0x0000787ffbc2324f clang::AttributePool::~AttributePool() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/ParsedAttr.h:726:41
#<!-- -->58 0x0000787ffbc2324f clang::ParsedAttributes::~ParsedAttributes() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/ParsedAttr.h:958:7
#<!-- -->59 0x0000787ffbc2324f clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDeclCXX.cpp:273:5
#<!-- -->60 0x0000787ffbc2f07d clang::Parser::ParseScope::Exit() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:1197:24
#<!-- -->61 0x0000787ffbc2f07d clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDeclCXX.cpp:252:22
#<!-- -->62 0x0000787ffbbfb168 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDecl.cpp:2044:0
#<!-- -->63 0x0000787ffbccce95 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1022:32
#<!-- -->64 0x0000787ffbccd9a5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:763:36
#<!-- -->65 0x0000787ffbbd9b18 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseAST.cpp:170:69
#<!-- -->66 0x0000787ffd9021a1 clang::FrontendAction::Execute() /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/FrontendAction.cpp:1078:21
#<!-- -->67 0x0000787ffd89808f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:282:12
#<!-- -->68 0x0000787ffd89808f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:242:22
#<!-- -->69 0x0000787ffd89808f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/CompilerInstance.cpp:1061:42
#<!-- -->70 0x0000787ffd95ce85 std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#<!-- -->71 0x0000787ffd95ce85 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#<!-- -->72 0x0000787ffd95ce85 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#<!-- -->73 0x0000787ffd95ce85 clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInvocation.h:259:0
#<!-- -->74 0x0000787ffd95ce85 clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInstance.h:312:0
#<!-- -->75 0x0000787ffd95ce85 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-19.1.7.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:281:0
#<!-- -->76 0x00005bf1e82f1832 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#<!-- -->77 0x00005bf1e82f604e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#<!-- -->78 0x0000787ffd56c915 operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:440:32
#<!-- -->79 0x0000787ffd56c915 callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::string*, bool*) const::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:45:52
#<!-- -->80 0x0000787ff35008aa llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:428:1
#<!-- -->81 0x0000787ffd5738c6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:444:10
#<!-- -->82 0x0000787ffd529bc9 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:200:3
#<!-- -->83 0x0000787ffd529fbc clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:253:5
#<!-- -->84 0x0000787ffd59a724 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /usr/include/llvm/ADT/SmallVector.h:95:46
#<!-- -->85 0x0000787ffd59a724 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Driver.cpp:1946:28
#<!-- -->86 0x00005bf1e82f7d8c llvm::SmallVectorBase&lt;unsigned int&gt;::size() const /usr/include/llvm/ADT/SmallVector.h:92:32
#<!-- -->87 0x00005bf1e82f7d8c llvm::SmallVectorTemplateCommon&lt;std::pair&lt;int, clang::driver::Command const*&gt;, void&gt;::end() /usr/include/llvm/ADT/SmallVector.h:283:41
#<!-- -->88 0x00005bf1e82f7d8c clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:393:26
#<!-- -->89 0x00005bf1e82e9f65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#<!-- -->90 0x0000787ff28376b5 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->91 0x0000787ff2837769 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->92 0x0000787ff2837769 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->93 0x00005bf1e82e9fc5 _start (/usr/bin/clang+++0xafc5)
```
&lt;/details&gt;

clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.7
Target: spirv64
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: [/tmp/vector_add-dca0ad.clcpp](https://github.com/user-attachments/files/20137312/vector_add-dca0ad.txt)
clang++: note: diagnostic msg: [/tmp/vector_add-dca0ad.sh](https://github.com/user-attachments/files/20137308/vector_add-dca0ad.txt)
clang++: note: diagnostic msg: 
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 20 or `main` branch?

---

### Comment 3 - tibetiroka

I tried it on commit 49042f2bee9cfa6ab5bfa614cda3a9ccfc1b3b0f, still crashes. The stack trace:

```
0.	Program arguments: llvm/build/bin/clang++ -x clcpp -cl-std=clc++2021 -target spirv64 -emit-llvm -O0 -Wno-everything -c vector_add.clcpp -o vector_add.bc -Xclang -fdeclare-opencl-builtins -Xclang -finclude-default-header -Ilibcxx_opencl -Ilibcxx_opencl/x86_64-pc-linux-gnu/
1.	libcxx_opencl/optional:101:5 <Spelling=libcxx_opencl/x86_64-pc-linux-gnu/bits/c++config.h:261:72>: current parser token ')'
2.	libcxx_opencl/optional:59:1: parsing namespace 'std'
3.	libcxx_opencl/optional:101:3: parsing function body 'std::__throw_bad_optional_access'
4.	libcxx_opencl/optional:101:3: in compound statement ('{}')
 #0 0x000064fd2d768a69 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (llvm/build/bin/clang+++0x3fe2a69)
 #1 0x000064fd2d766794 llvm::sys::CleanupOnSignal(unsigned long) (llvm/build/bin/clang+++0x3fe0794)
 #2 0x000064fd2d6a2468 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076515d24def0 (/usr/lib/libc.so.6+0x3def0)
 #4 0x000064fd302111a5 resolveDeallocationOverload(clang::Sema&, clang::LookupResult&, clang::ImplicitDeallocationParameters const&, clang::SourceLocation, llvm::SmallVectorImpl<(anonymous namespace)::UsualDeallocFnInfo>*) SemaExprCXX.cpp:0:0
 #5 0x000064fd302306f1 clang::Sema::FindUsualDeallocationFunction(clang::SourceLocation, clang::ImplicitDeallocationParameters, clang::DeclarationName) (llvm/build/bin/clang+++0x6aaa6f1)
 #6 0x000064fd30233821 clang::Sema::FindDeallocationFunctionForDestructor(clang::SourceLocation, clang::CXXRecordDecl*, bool) (llvm/build/bin/clang+++0x6aad821)
 #7 0x000064fd3007df60 clang::Sema::CheckDestructor(clang::CXXDestructorDecl*) (llvm/build/bin/clang+++0x68f7f60)
 #8 0x000064fd30080dba clang::Sema::DefineImplicitDestructor(clang::SourceLocation, clang::CXXDestructorDecl*) (llvm/build/bin/clang+++0x68fadba)
 #9 0x000064fd3015b7b6 void llvm::function_ref<void ()>::callback_fn<clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::'lambda'()>(long) SemaExpr.cpp:0:0
#10 0x000064fd31596ae0 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (llvm/build/bin/clang+++0x7e10ae0)
#11 0x000064fd3015aed0 clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (llvm/build/bin/clang+++0x69d4ed0)
#12 0x000064fd3020e22a clang::Sema::MaybeBindToTemporary(clang::Expr*) (.part.0) SemaExprCXX.cpp:0:0
#13 0x000064fd30326456 PerformConstructorInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::InitializationSequence::Step const&, bool&, bool, bool, clang::SourceLocation, clang::SourceLocation) SemaInit.cpp:0:0
#14 0x000064fd30343f8d clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (llvm/build/bin/clang+++0x6bbdf8d)
#15 0x000064fd3021dc29 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (llvm/build/bin/clang+++0x6a97c29)
#16 0x000064fd3028fe0d clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (llvm/build/bin/clang+++0x6b09e0d)
#17 0x000064fd2fc55ab9 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (llvm/build/bin/clang+++0x64cfab9)
#18 0x000064fd2fc43f1e clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (llvm/build/bin/clang+++0x64bdf1e)
#19 0x000064fd2fc44b77 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (llvm/build/bin/clang+++0x64beb77)
#20 0x000064fd2fc44d6a clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (llvm/build/bin/clang+++0x64bed6a)
#21 0x000064fd2fc49d2e clang::Parser::ParseExpression(clang::TypeCastState) (llvm/build/bin/clang+++0x64c3d2e)
#22 0x000064fd2fc57bb5 clang::Parser::ParseCXXAmbiguousParenExpression(clang::ParenParseOption&, clang::OpaquePtr<clang::QualType>&, clang::BalancedDelimiterTracker&, clang::ColonProtectionRAIIObject&) (llvm/build/bin/clang+++0x64d1bb5)
#23 0x000064fd2fc4fb56 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (llvm/build/bin/clang+++0x64c9b56)
#24 0x000064fd2fc42fae clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (llvm/build/bin/clang+++0x64bcfae)
#25 0x000064fd2fc44b77 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (llvm/build/bin/clang+++0x64beb77)
#26 0x000064fd2fc4f638 clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (llvm/build/bin/clang+++0x64c9638)
#27 0x000064fd2fc42fae clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (llvm/build/bin/clang+++0x64bcfae)
#28 0x000064fd2fc44b77 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (llvm/build/bin/clang+++0x64beb77)
#29 0x000064fd2fc44d6a clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (llvm/build/bin/clang+++0x64bed6a)
#30 0x000064fd2fc46b8d clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult<clang::Expr*, true>, clang::prec::Level) (llvm/build/bin/clang+++0x64c0b8d)
#31 0x000064fd2fc4ec5f clang::Parser::ParseParenExpression(clang::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (llvm/build/bin/clang+++0x64c8c5f)
#32 0x000064fd2fc42fae clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (llvm/build/bin/clang+++0x64bcfae)
#33 0x000064fd2fc44b77 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (llvm/build/bin/clang+++0x64beb77)
#34 0x000064fd2fc44d6a clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (llvm/build/bin/clang+++0x64bed6a)
#35 0x000064fd2fc49d2e clang::Parser::ParseExpression(clang::TypeCastState) (llvm/build/bin/clang+++0x64c3d2e)
#36 0x000064fd2fcd0778 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (llvm/build/bin/clang+++0x654a778)
#37 0x000064fd2fcd24b1 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (llvm/build/bin/clang+++0x654c4b1)
#38 0x000064fd2fcd300e clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (llvm/build/bin/clang+++0x654d00e)
#39 0x000064fd2fcd4b6c clang::Parser::ParseCompoundStatementBody(bool) (llvm/build/bin/clang+++0x654eb6c)
#40 0x000064fd2fcd55bf clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (llvm/build/bin/clang+++0x654f5bf)
#41 0x000064fd2fbbcae9 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (llvm/build/bin/clang+++0x6436ae9)
#42 0x000064fd2fc0e540 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (llvm/build/bin/clang+++0x6488540)
#43 0x000064fd2fbb56e4 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (llvm/build/bin/clang+++0x642f6e4)
#44 0x000064fd2fbb6503 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (llvm/build/bin/clang+++0x6430503)
#45 0x000064fd2fbbff1d clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (llvm/build/bin/clang+++0x6439f1d)
#46 0x000064fd2fc2d8f7 clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (llvm/build/bin/clang+++0x64a78f7)
#47 0x000064fd2fc309dc clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (llvm/build/bin/clang+++0x64aa9dc)
#48 0x000064fd2fc106fc clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (llvm/build/bin/clang+++0x648a6fc)
#49 0x000064fd2fbc00ed clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (llvm/build/bin/clang+++0x643a0ed)
#50 0x000064fd2fbc0f05 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (llvm/build/bin/clang+++0x643af05)
#51 0x000064fd2fbaf67c clang::ParseAST(clang::Sema&, bool, bool) (llvm/build/bin/clang+++0x642967c)
#52 0x000064fd2e47f474 clang::FrontendAction::Execute() (llvm/build/bin/clang+++0x4cf9474)
#53 0x000064fd2e3fcfca clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (llvm/build/bin/clang+++0x4c76fca)
#54 0x000064fd2e56662e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (llvm/build/bin/clang+++0x4de062e)
#55 0x000064fd2a609ed0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (llvm/build/bin/clang+++0xe83ed0)
#56 0x000064fd2a6005d4 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#57 0x000064fd2e19c449 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#58 0x000064fd2d6a2854 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (llvm/build/bin/clang+++0x3f1c854)
#59 0x000064fd2e19cb21 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#60 0x000064fd2e15abea clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (llvm/build/bin/clang+++0x49d4bea)
#61 0x000064fd2e15bb72 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (llvm/build/bin/clang+++0x49d5b72)
#62 0x000064fd2e1724fc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (llvm/build/bin/clang+++0x49ec4fc)
#63 0x000064fd2a605667 clang_main(int, char**, llvm::ToolContext const&) (llvm/build/bin/clang+++0xe7f667)
#64 0x000064fd2a4711c0 main (llvm/build/bin/clang+++0xceb1c0)
#65 0x000076515d2376b5 (/usr/lib/libc.so.6+0x276b5)
#66 0x000076515d237769 __libc_start_main (/usr/lib/libc.so.6+0x27769)
#67 0x000064fd2a5fffe5 _start (llvm/build/bin/clang+++0xe79fe5)
```

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-opencl

Author: None (tibetiroka)

<details>
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
&lt;details&gt;
&lt;summary&gt;Stack dump:&lt;/summary&gt;

```
0.	Program arguments: clang++ -x clcpp -cl-std=clc++2021 -target spirv64 -emit-llvm -O0 -Wno-everything -c vector_add.clcpp -o vector_add.bc -Xclang -fdeclare-opencl-builtins -Xclang -finclude-default-header -Ilibcxx_opencl -Ilibcxx_opencl/x86_64-pc-linux-gnu/
1.	libcxx_opencl/optional:101:5 &lt;Spelling=libcxx_opencl/x86_64-pc-linux-gnu/bits/c++config.h:261:72&gt;: current parser token ')'
2.	libcxx_opencl/optional:59:1: parsing namespace 'std'
3.	libcxx_opencl/optional:101:3: parsing function body 'std::__throw_bad_optional_access'
4.	libcxx_opencl/optional:101:3: in compound statement ('{}')
 #<!-- -->0 0x0000787ff3619920 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
 #<!-- -->1 0x0000787ff3616df5 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
 #<!-- -->2 0x0000787ff3616df5 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:367:31
 #<!-- -->3 0x0000787ff3500459 HandleCrash /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->4 0x0000787ff3500459 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #<!-- -->5 0x0000787ff284def0 (/usr/lib/libc.so.6+0x3def0)
 #<!-- -->6 0x0000787ffe192034 clang::ASTContext::getTypeDeclType(clang::TypeDecl const*, clang::TypeDecl const*) const (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/ASTContext.h:1637:0
 #<!-- -->7 0x0000787ffc9436f8 isNonPlacementDeallocationFunction(clang::Sema&amp;, clang::FunctionDecl*) (.part.0.lto_priv.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1750:0
 #<!-- -->8 0x0000787ffc8cb4ef isNonPlacementDeallocationFunction /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1737:0
 #<!-- -->9 0x0000787ffc8cb4ef resolveDeallocationOverload /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1844:0
#<!-- -->10 0x0000787ffc8d1860 clang::LookupResult::~LookupResult() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/Lookup.h:247:0
#<!-- -->11 0x0000787ffc8d1860 clang::Sema::FindUsualDeallocationFunction(clang::SourceLocation, bool, bool, clang::DeclarationName) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:3271:0
#<!-- -->12 0x0000787ffc75cedb clang::Sema::CheckDestructor(clang::CXXDestructorDecl*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaDeclCXX.cpp:10853:50
#<!-- -->13 0x0000787ffc77bc72 clang::Sema::DefineImplicitDestructor(clang::SourceLocation, clang::CXXDestructorDecl*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaDeclCXX.cpp:14209:0
#<!-- -->14 0x0000787ffc841a3b operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:17957:0
#<!-- -->15 0x0000787ffc841a3b callback_fn&lt;clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:45:0
#<!-- -->16 0x0000787ffc4eeebd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:569:1
#<!-- -->17 0x0000787ffc82375c clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:17933:0
#<!-- -->18 0x0000787ffc943f2b llvm::detail::PunnedPointer&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;::PunnedPointer(long) /usr/include/llvm/ADT/PointerIntPair.h:37:0
#<!-- -->19 0x0000787ffc943f2b llvm::PointerIntPair&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;, 3u, unsigned int, llvm::PointerLikeTypeTraits&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;, llvm::PointerIntPairInfo&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;, 3u, llvm::PointerLikeTypeTraits&lt;llvm::PointerUnion&lt;clang::Type const*, clang::ExtQuals const*&gt;&gt;&gt;&gt;::PointerIntPair() /usr/include/llvm/ADT/PointerIntPair.h:86:0
#<!-- -->20 0x0000787ffc943f2b clang::QualType::QualType() /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Type.h:964:0
#<!-- -->21 0x0000787ffc943f2b clang::Sema::MaybeBindToTemporary(clang::Expr*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:7631:0
#<!-- -->22 0x0000787ffc97d07f PerformConstructorInitialization /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaInit.cpp:7294:0
#<!-- -->23 0x0000787ffc9818d5 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaInit.cpp:7696:0
#<!-- -->24 0x0000787ffc8ca553 clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1660:0
#<!-- -->25 0x0000787ffc8ca921 clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExprCXX.cpp:1525:0
#<!-- -->26 0x0000787ffbc3d2d1 clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExprCXX.cpp:2069:45
#<!-- -->27 0x0000787ffbc409c4 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1686:3
#<!-- -->28 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#<!-- -->29 0x0000787ffe0dd22d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:182:0
#<!-- -->30 0x0000787ffe0dd3c5 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:134:0
#<!-- -->31 0x0000787ffbc5142d clang::Parser::ParseCXXAmbiguousParenExpression(clang::Parser::ParenParseOption&amp;, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::BalancedDelimiterTracker&amp;, clang::ColonProtectionRAIIObject&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExprCXX.cpp:4184:27
#<!-- -->32 0x0000787ffbc52dbc clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:3152:37
#<!-- -->33 0x0000787ffbc3fb9b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1099:5
#<!-- -->34 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#<!-- -->35 0x0000787ffbc51b9d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:3268:9
#<!-- -->36 0x0000787ffbc3fb9b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1099:5
#<!-- -->37 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#<!-- -->38 0x0000787ffe0dd22d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:182:0
#<!-- -->39 0x0000787ffbc1c14a clang::Parser::ParseRHSOfBinaryExpression(clang::ActionResult&lt;clang::Expr*, true&gt;, clang::prec::Level) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:575:38
#<!-- -->40 0x0000787ffbc5203d clang::Parser::getLangOpts() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:495:51
#<!-- -->41 0x0000787ffbc5203d clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&amp;, bool, bool, clang::OpaquePtr&lt;clang::QualType&gt;&amp;, clang::SourceLocation&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:3337:21
#<!-- -->42 0x0000787ffbc3fb9b clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:1099:5
#<!-- -->43 0x0000787ffe0dd15a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:718:0
#<!-- -->44 0x0000787ffe0dd22d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:182:0
#<!-- -->45 0x0000787ffe0dd3c5 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseExpr.cpp:134:0
#<!-- -->46 0x0000787ffbc94d30 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:555:3
#<!-- -->47 0x0000787ffbc907b6 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:292:40
#<!-- -->48 0x0000787ffbc90ac7 clang::Parser::MaybeDestroyTemplateIds() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:328:5
#<!-- -->49 0x0000787ffbc90ac7 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:126:26
#<!-- -->50 0x0000787ffbc98dba clang::Parser::ParseCompoundStatementBody(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:1248:38
#<!-- -->51 0x0000787ffbc9c5cc clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseStmt.cpp:2532:3
#<!-- -->52 0x0000787ffbcbe52f clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1525:36
#<!-- -->53 0x0000787ffbbef35f clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDecl.cpp:2427:0
#<!-- -->54 0x0000787ffbcbb808 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1249:73
#<!-- -->55 0x0000787ffbcbc1ea clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1271:42
#<!-- -->56 0x0000787ffbcccca2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1074:50
#<!-- -->57 0x0000787ffbc2324f clang::AttributePool::~AttributePool() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/ParsedAttr.h:726:41
#<!-- -->58 0x0000787ffbc2324f clang::ParsedAttributes::~ParsedAttributes() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/ParsedAttr.h:958:7
#<!-- -->59 0x0000787ffbc2324f clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDeclCXX.cpp:273:5
#<!-- -->60 0x0000787ffbc2f07d clang::Parser::ParseScope::Exit() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Parse/Parser.h:1197:24
#<!-- -->61 0x0000787ffbc2f07d clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDeclCXX.cpp:252:22
#<!-- -->62 0x0000787ffbbfb168 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseDecl.cpp:2044:0
#<!-- -->63 0x0000787ffbccce95 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:1022:32
#<!-- -->64 0x0000787ffbccd9a5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:763:36
#<!-- -->65 0x0000787ffbbd9b18 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseAST.cpp:170:69
#<!-- -->66 0x0000787ffd9021a1 clang::FrontendAction::Execute() /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/FrontendAction.cpp:1078:21
#<!-- -->67 0x0000787ffd89808f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:282:12
#<!-- -->68 0x0000787ffd89808f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:242:22
#<!-- -->69 0x0000787ffd89808f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/CompilerInstance.cpp:1061:42
#<!-- -->70 0x0000787ffd95ce85 std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#<!-- -->71 0x0000787ffd95ce85 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#<!-- -->72 0x0000787ffd95ce85 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#<!-- -->73 0x0000787ffd95ce85 clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInvocation.h:259:0
#<!-- -->74 0x0000787ffd95ce85 clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInstance.h:312:0
#<!-- -->75 0x0000787ffd95ce85 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-19.1.7.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:281:0
#<!-- -->76 0x00005bf1e82f1832 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#<!-- -->77 0x00005bf1e82f604e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#<!-- -->78 0x0000787ffd56c915 operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:440:32
#<!-- -->79 0x0000787ffd56c915 callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::string*, bool*) const::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:45:52
#<!-- -->80 0x0000787ff35008aa llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:428:1
#<!-- -->81 0x0000787ffd5738c6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:444:10
#<!-- -->82 0x0000787ffd529bc9 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:200:3
#<!-- -->83 0x0000787ffd529fbc clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:253:5
#<!-- -->84 0x0000787ffd59a724 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /usr/include/llvm/ADT/SmallVector.h:95:46
#<!-- -->85 0x0000787ffd59a724 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Driver.cpp:1946:28
#<!-- -->86 0x00005bf1e82f7d8c llvm::SmallVectorBase&lt;unsigned int&gt;::size() const /usr/include/llvm/ADT/SmallVector.h:92:32
#<!-- -->87 0x00005bf1e82f7d8c llvm::SmallVectorTemplateCommon&lt;std::pair&lt;int, clang::driver::Command const*&gt;, void&gt;::end() /usr/include/llvm/ADT/SmallVector.h:283:41
#<!-- -->88 0x00005bf1e82f7d8c clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:393:26
#<!-- -->89 0x00005bf1e82e9f65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#<!-- -->90 0x0000787ff28376b5 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->91 0x0000787ff2837769 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->92 0x0000787ff2837769 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->93 0x00005bf1e82e9fc5 _start (/usr/bin/clang+++0xafc5)
```
&lt;/details&gt;

clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.7
Target: spirv64
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang++: note: diagnostic msg: [/tmp/vector_add-dca0ad.clcpp](https://github.com/user-attachments/files/20137312/vector_add-dca0ad.txt)
clang++: note: diagnostic msg: [/tmp/vector_add-dca0ad.sh](https://github.com/user-attachments/files/20137308/vector_add-dca0ad.txt)
clang++: note: diagnostic msg: 
</details>


---

### Comment 5 - Endilll

Reduced by C-Reduce (https://godbolt.org/z/6q7fqWf3d):
```cpp
#pragma OPENCL EXTENSION __cl_clang_function_pointers : enable
struct exception {
  virtual ~exception();
};
void operator delete(void *, void *);
struct S : exception {
} bad_optional_access;
```
Clang 21.0 crash:
```
clang: /root/llvm-project/llvm/tools/clang/include/clang/AST/ASTContext.h:1756:
clang::QualType clang::ASTContext::getTypeDeclType(const clang::TypeDecl*, const clang::TypeDecl*) const:
Assertion `Decl && "Passed null for Decl param"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -cc1 -debug-info-kind=limited -dwarf-version=5 -debugger-tuning=gdb -o /app/output.bc -finclude-default-header -fdeclare-opencl-builtins -triple spir64-unknown-unknown -fcolor-diagnostics -cl-std=clc++2021 <source> -emit-llvm-bc
1.	<source>:7:22: current parser token ';'
 #0 0x0000000003f78848 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f78848)
 #1 0x0000000003f761d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000726b69442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000726b694969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000726b69442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000726b694287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000726b6942871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000726b69439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000006c198a0 clang::ASTContext::getTypeDeclType(clang::TypeDecl const*, clang::TypeDecl const*) const (.constprop.0) SemaExprCXX.cpp:0:0
 #9 0x0000000006c1e068 resolveDeallocationOverload(clang::Sema&, clang::LookupResult&, clang::ImplicitDeallocationParameters const&, clang::SourceLocation, llvm::SmallVectorImpl<(anonymous namespace)::UsualDeallocFnInfo>*) SemaExprCXX.cpp:0:0
#10 0x0000000006c2c3bc clang::Sema::FindUsualDeallocationFunction(clang::SourceLocation, clang::ImplicitDeallocationParameters, clang::DeclarationName) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c2c3bc)
#11 0x0000000006c366c1 clang::Sema::FindDeallocationFunctionForDestructor(clang::SourceLocation, clang::CXXRecordDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c366c1)
#12 0x0000000006ab111c clang::Sema::CheckDestructor(clang::CXXDestructorDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ab111c)
#13 0x0000000006ac00c6 clang::Sema::DefineImplicitDestructor(clang::SourceLocation, clang::CXXDestructorDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6ac00c6)
#14 0x0000000006b6038a void llvm::function_ref<void ()>::callback_fn<clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool)::'lambda'()>(long) SemaExpr.cpp:0:0
#15 0x0000000007eadef1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x7eadef1)
#16 0x0000000006b5e65f clang::Sema::MarkFunctionReferenced(clang::SourceLocation, clang::FunctionDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6b5e65f)
#17 0x0000000006a77782 clang::Sema::FinalizeVarWithDestructor(clang::VarDecl*, clang::RecordType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a77782)
#18 0x00000000069e930f clang::Sema::CheckCompleteVariableDeclaration(clang::VarDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x69e930f)
#19 0x00000000069eb49f clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#20 0x00000000066591be clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66591be)
#21 0x0000000006668e79 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6668e79)
#22 0x00000000066276be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66276be)
#23 0x0000000006627e69 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6627e69)
#24 0x000000000662f7aa clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662f7aa)
#25 0x000000000663077d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x663077d)
#26 0x0000000006622aea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6622aea)
#27 0x000000000492ed48 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x492ed48)
#28 0x0000000004c28c65 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c28c65)
#29 0x0000000004ba5ffe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ba5ffe)
#30 0x0000000004d1b441 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d1b441)
#31 0x0000000000d8e81f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd8e81f)
#32 0x0000000000d853fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#33 0x0000000000d89b19 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd89b19)
#34 0x0000000000c41924 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc41924)
#35 0x0000726b69429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#36 0x0000726b69429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#37 0x0000000000d84ea5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd84ea5)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```

---

