# clang++ 19.1.7 crash in clang::Sema::tryCaptureVariable: Assertion `idx < size()' failed.

**Author:** jehelset
**URL:** https://github.com/llvm/llvm-project/issues/137327

## Body

[bug.tar.gz](https://github.com/user-attachments/files/19909223/bug.tar.gz)
Stack dump below, source and run script attached (too big so compressed).
```
0.	Program arguments: /usr/bin/clang++ -ftemplate-depth=1200 -std=gnu++2b -O3 -fvisibility=hidden -DCPPTRACE_DEMANGLE_WITH_CXXABI -DCPPTRACE_GET_SYMBOLS_WITH_LIBDWARF -DCPPTRACE_HAS_CXX_EXCEPTION_TYPE -DCPPTRACE_HAS_DL_FIND_OBJECT -DCPPTRACE_UNWIND_WITH_LIBUNWIND -DFMT_HEADER_ONLY=1 -DUNW_LOCAL_ONLY -I/home/jeh/code/gitlab/jehelset/klo/tests/unit -I/home/jeh/code/gitlab/jehelset/klo/build/clang/release/tests/unit/_hale_klo_generate_sources -I/home/jeh/code/gitlab/jehelset/klo/build/clang/release/_deps/pels-src/sources -I/home/jeh/code/gitlab/jehelset/klo/build/clang/release/_deps/pels-build/sources/_hale_pels_generate_sources -I/home/jeh/code/gitlab/jehelset/klo/sources -I/home/jeh/code/gitlab/jehelset/klo/build/clang/release/sources/_hale_klo_generate_sources -isystem /usr/include/cpptrace -stdlib=libstdc++ -DNDEBUG -c -MD -MT tests/unit/CMakeFiles/klo-tests.dir/coroutines.cpp.o -MF tests/unit/CMakeFiles/klo-tests.dir/coroutines.cpp.o.d -fcolor-diagnostics -o tests/unit/CMakeFiles/klo-tests.dir/coroutines.cpp.o /home/jeh/code/gitlab/jehelset/klo/tests/unit/coroutines.cpp
1.	<eof> parser at end of file
2.	/home/jeh/code/gitlab/jehelset/klo/tests/unit/coroutines.cpp:55:13: instantiating function definition 'klo::coroutines::builtin::DOCTEST_ANON_FUNC_4()::(anonymous class)::operator()<klo::coroutines::trees::Seed<bool>>'
3.	/home/jeh/code/gitlab/jehelset/klo/tests/unit/coroutines.cpp:43:13: instantiating function definition 'klo::coroutines::builtin::DOCTEST_ANON_FUNC_4()::(anonymous class)::operator()<klo::coroutines::trees::Seed<int, bool>, (lambda at /home/jeh/code/gitlab/jehelset/klo/tests/unit/coroutines.cpp:59:15)>'
 #0 0x00007a2185c1a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
 #1 0x00007a2185c17bf5 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
 #2 0x00007a2185c17bf5 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:367:31
 #3 0x00007a2185b01319 HandleCrash /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #4 0x00007a2185b01319 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #5 0x00007a2184e4bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #6 0x00007a218ee064af clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:18760:0
 #7 0x00007a218edd8899 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:18996:0
 #8 0x00007a218edd8ec9 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::SourceLocation, clang::CXXScopeSpec const*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:2191:0
 #9 0x00007a218f27d768 TransformSizeOfPackExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:15022:0
#10 0x00007a218f27cba5 TransformTemplateArgument /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4754:0
#11 0x00007a218f27cba5 TransformTemplateArgument /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1605:0
#12 0x00007a21906dc96b bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4933:0
#13 0x00007a218f274c00 TransformUnresolvedLookupExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:13695:0
#14 0x00007a218f28a004 TransformCallExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:12286:0
#15 0x00007a218f27cba5 TransformTemplateArgument /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4754:0
#16 0x00007a218f27cba5 TransformTemplateArgument /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1605:0
#17 0x00007a21906dc96b bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4933:0
#18 0x00007a218f274c00 TransformUnresolvedLookupExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:13695:0
#19 0x00007a218f28a004 TransformCallExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:12286:0
#20 0x00007a218f282ea0 TransformExprs /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4346:0
#21 0x00007a218f28a08f TransformCallExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:12292:0
#22 0x00007a218f2810cf TransformExprRequirement /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2680:0
#23 0x00007a218f2810cf TransformRequiresExprRequirements /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1736:0
#24 0x00007a218f2810cf TransformRequiresExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:13908:0
#25 0x00007a218f27af27 TransformRequiresExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1708:0
#26 0x00007a218f27af27 TransformExpr /usr/src/debug/clang/clang-19.1.7.src/build/include/clang/AST/StmtNodes.inc:122:0
#27 0x00007a218ec4d317 llvm::DenseMap<clang::Decl*, clang::Decl*, llvm::DenseMapInfo<clang::Decl*, void>, llvm::detail::DenseMapPair<clang::Decl*, clang::Decl*>>::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:798:0
#28 0x00007a218ec4d317 ~TreeTransform /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:109:0
#29 0x00007a218ec4d317 ~TemplateInstantiator /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1337:0
#30 0x00007a218ec4d317 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4263:0
#31 0x00007a218ec4d317 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4255:0
#32 0x00007a218ec4d317 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4270:0
#33 0x00007a218ec4d317 EvaluateAtomicConstraint /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:476:0
#34 0x00007a218ec4d317 calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, const clang::NamedDecl*, clang::SourceLocation, const clang::MultiLevelTemplateArgumentList&, const clang::Expr*, clang::ConstraintSatisfaction&)::ConstraintEvaluator> /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:351:0
#35 0x00007a218ec4ec0b CheckConstraintSatisfaction /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:600:0
#36 0x00007a218ec4f0d1 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:658:0
#37 0x00007a218ec50231 llvm::SmallVectorTemplateCommon<clang::Expr*, void>::isSmall() const /usr/include/llvm/ADT/SmallVector.h:157:0
#38 0x00007a218ec50231 llvm::SmallVectorImpl<clang::Expr*>::~SmallVectorImpl() /usr/include/llvm/ADT/SmallVector.h:617:0
#39 0x00007a218ec50231 llvm::SmallVector<clang::Expr*, 1u>::~SmallVector() /usr/include/llvm/ADT/SmallVector.h:1217:0
#40 0x00007a218ec50231 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:1137:0
#41 0x00007a218f1e88f0 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateDeduction.cpp:3920:0
#42 0x00007a218f1e99f5 callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool(llvm::ArrayRef<clang::QualType>)>)::<lambda()> > /usr/include/llvm/ADT/STLFunctionalExtras.h:46:0
#43 0x00007a218eaee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:569:1
#44 0x00007a218f1f3128 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateDeduction.cpp:4584:0
#45 0x00007a21906e6af2 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaOverload.cpp:7593:0
#46 0x00007a218f0f6ec1 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaOverload.cpp:7374:0
#47 0x00007a218f0f7650 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaOverload.cpp:15573:0
#48 0x00007a218edea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:6375:0
#49 0x00007a218f28a15a clang::Sema::FPFeaturesStateRAII::~FPFeaturesStateRAII() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:2737:0
#50 0x00007a218f28a15a TransformCallExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:12316:0
#51 0x00007a218f2848fe TransformDependentCoawaitExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:8557:0
#52 0x00007a218f284b58 TransformCoreturnStmt /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:8520:0
#53 0x00007a218f29f8de TransformCompoundStmt /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:7831:0
#54 0x00007a218f29eaf7 TransformCoroutineBodyStmt /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:8434:0
#55 0x00007a218f2c88cb llvm::DenseMap<clang::Decl*, clang::Decl*, llvm::DenseMapInfo<clang::Decl*, void>, llvm::detail::DenseMapPair<clang::Decl*, clang::Decl*>>::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:798:0
#56 0x00007a218f2c88cb ~TreeTransform /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:109:0
#57 0x00007a218f2c88cb ~TemplateInstantiator /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1337:0
#58 0x00007a218f2c88cb clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4234:0
#59 0x00007a218f2c88cb clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4226:0
#60 0x00007a218f2c88cb clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:5186:0
#61 0x00007a218f2d49ec clang::FunctionDecl::isDefined() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Decl.h:2210:0
#62 0x00007a218f2d49ec clang::Sema::PerformPendingInstantiations(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:6411:0
#63 0x00007a218f2c89fd clang::LocalInstantiationScope::Exit() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/Template.h:446:0
#64 0x00007a218f2c89fd clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:5209:0
#65 0x00007a218f2d49ec clang::FunctionDecl::isDefined() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Decl.h:2210:0
#66 0x00007a218f2d49ec clang::Sema::PerformPendingInstantiations(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:6411:0
#67 0x00007a218eb3f50d llvm::TimeTraceScope::~TimeTraceScope() /usr/include/llvm/Support/TimeProfiler.h:189:37
#68 0x00007a218eb3f50d clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:1146:3
#69 0x00007a218eb32660 clang::Sema::ActOnEndOfTranslationUnit() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:1184:0
#70 0x00007a218e2ce52a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:732:12
#71 0x00007a218e1d9c18 clang::ParseAST(clang::Sema&, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseAST.cpp:170:0
#72 0x00007a218fefa221 clang::FrontendAction::Execute() /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/FrontendAction.cpp:1078:21
#73 0x00007a218fe9008f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:282:12
#74 0x00007a218fe9008f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:242:22
#75 0x00007a218fe9008f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/CompilerInstance.cpp:1061:42
#76 0x00007a218ff54ba5 std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#77 0x00007a218ff54ba5 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#78 0x00007a218ff54ba5 std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#79 0x00007a218ff54ba5 clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInvocation.h:259:0
#80 0x00007a218ff54ba5 clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInstance.h:312:0
#81 0x00007a218ff54ba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-19.1.7.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:281:0
#82 0x0000560bcd624832 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#83 0x0000560bcd62904e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#84 0x00007a218fb64bb5 operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:440:32
#85 0x00007a218fb64bb5 callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::string*, bool*) const::<lambda()> > /usr/include/llvm/ADT/STLFunctionalExtras.h:45:52
#86 0x00007a2185b0176a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:428:1
#87 0x00007a218fb6bb76 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:444:10
#88 0x00007a218fb22709 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:200:3
#89 0x00007a218fb22afc clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:253:5
#90 0x00007a218fb92a64 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:95:46
#91 0x00007a218fb92a64 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Driver.cpp:1946:28
#92 0x0000560bcd62ad8c llvm::SmallVectorBase<unsigned int>::size() const /usr/include/llvm/ADT/SmallVector.h:92:32
#93 0x0000560bcd62ad8c llvm::SmallVectorTemplateCommon<std::pair<int, clang::driver::Command const*>, void>::end() /usr/include/llvm/ADT/SmallVector.h:283:41
#94 0x0000560bcd62ad8c clang_main(int, char**, llvm::ToolContext const&) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:393:26
#95 0x0000560bcd61cf65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#96 0x00007a2184e35488 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#97 0x00007a2184e3554c call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#98 0x00007a2184e3554c __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#99 0x0000560bcd61cfc5 _start (/usr/bin/clang+++0xafc5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (jehelset)

<details>
[bug.tar.gz](https://github.com/user-attachments/files/19909223/bug.tar.gz)
Stack dump below, source and run script attached (too big so compressed).
```
0.	Program arguments: /usr/bin/clang++ -ftemplate-depth=1200 -std=gnu++2b -O3 -fvisibility=hidden -DCPPTRACE_DEMANGLE_WITH_CXXABI -DCPPTRACE_GET_SYMBOLS_WITH_LIBDWARF -DCPPTRACE_HAS_CXX_EXCEPTION_TYPE -DCPPTRACE_HAS_DL_FIND_OBJECT -DCPPTRACE_UNWIND_WITH_LIBUNWIND -DFMT_HEADER_ONLY=1 -DUNW_LOCAL_ONLY -I/home/jeh/code/gitlab/jehelset/klo/tests/unit -I/home/jeh/code/gitlab/jehelset/klo/build/clang/release/tests/unit/_hale_klo_generate_sources -I/home/jeh/code/gitlab/jehelset/klo/build/clang/release/_deps/pels-src/sources -I/home/jeh/code/gitlab/jehelset/klo/build/clang/release/_deps/pels-build/sources/_hale_pels_generate_sources -I/home/jeh/code/gitlab/jehelset/klo/sources -I/home/jeh/code/gitlab/jehelset/klo/build/clang/release/sources/_hale_klo_generate_sources -isystem /usr/include/cpptrace -stdlib=libstdc++ -DNDEBUG -c -MD -MT tests/unit/CMakeFiles/klo-tests.dir/coroutines.cpp.o -MF tests/unit/CMakeFiles/klo-tests.dir/coroutines.cpp.o.d -fcolor-diagnostics -o tests/unit/CMakeFiles/klo-tests.dir/coroutines.cpp.o /home/jeh/code/gitlab/jehelset/klo/tests/unit/coroutines.cpp
1.	&lt;eof&gt; parser at end of file
2.	/home/jeh/code/gitlab/jehelset/klo/tests/unit/coroutines.cpp:55:13: instantiating function definition 'klo::coroutines::builtin::DOCTEST_ANON_FUNC_4()::(anonymous class)::operator()&lt;klo::coroutines::trees::Seed&lt;bool&gt;&gt;'
3.	/home/jeh/code/gitlab/jehelset/klo/tests/unit/coroutines.cpp:43:13: instantiating function definition 'klo::coroutines::builtin::DOCTEST_ANON_FUNC_4()::(anonymous class)::operator()&lt;klo::coroutines::trees::Seed&lt;int, bool&gt;, (lambda at /home/jeh/code/gitlab/jehelset/klo/tests/unit/coroutines.cpp:59:15)&gt;'
 #<!-- -->0 0x00007a2185c1a730 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:723:22
 #<!-- -->1 0x00007a2185c17bf5 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Signals.cpp:105:20
 #<!-- -->2 0x00007a2185c17bf5 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/Unix/Signals.inc:367:31
 #<!-- -->3 0x00007a2185b01319 HandleCrash /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #<!-- -->4 0x00007a2185b01319 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #<!-- -->5 0x00007a2184e4bcd0 (/usr/lib/libc.so.6+0x3dcd0)
 #<!-- -->6 0x00007a218ee064af clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&amp;, clang::QualType&amp;, unsigned int const*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:18760:0
 #<!-- -->7 0x00007a218edd8899 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&amp;, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:18996:0
 #<!-- -->8 0x00007a218edd8ec9 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::SourceLocation, clang::CXXScopeSpec const*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:2191:0
 #<!-- -->9 0x00007a218f27d768 TransformSizeOfPackExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:15022:0
#<!-- -->10 0x00007a218f27cba5 TransformTemplateArgument /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4754:0
#<!-- -->11 0x00007a218f27cba5 TransformTemplateArgument /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1605:0
#<!-- -->12 0x00007a21906dc96b bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4933:0
#<!-- -->13 0x00007a218f274c00 TransformUnresolvedLookupExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:13695:0
#<!-- -->14 0x00007a218f28a004 TransformCallExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:12286:0
#<!-- -->15 0x00007a218f27cba5 TransformTemplateArgument /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4754:0
#<!-- -->16 0x00007a218f27cba5 TransformTemplateArgument /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1605:0
#<!-- -->17 0x00007a21906dc96b bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4933:0
#<!-- -->18 0x00007a218f274c00 TransformUnresolvedLookupExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:13695:0
#<!-- -->19 0x00007a218f28a004 TransformCallExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:12286:0
#<!-- -->20 0x00007a218f282ea0 TransformExprs /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:4346:0
#<!-- -->21 0x00007a218f28a08f TransformCallExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:12292:0
#<!-- -->22 0x00007a218f2810cf TransformExprRequirement /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:2680:0
#<!-- -->23 0x00007a218f2810cf TransformRequiresExprRequirements /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1736:0
#<!-- -->24 0x00007a218f2810cf TransformRequiresExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:13908:0
#<!-- -->25 0x00007a218f27af27 TransformRequiresExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1708:0
#<!-- -->26 0x00007a218f27af27 TransformExpr /usr/src/debug/clang/clang-19.1.7.src/build/include/clang/AST/StmtNodes.inc:122:0
#<!-- -->27 0x00007a218ec4d317 llvm::DenseMap&lt;clang::Decl*, clang::Decl*, llvm::DenseMapInfo&lt;clang::Decl*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl*, clang::Decl*&gt;&gt;::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:798:0
#<!-- -->28 0x00007a218ec4d317 ~TreeTransform /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:109:0
#<!-- -->29 0x00007a218ec4d317 ~TemplateInstantiator /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1337:0
#<!-- -->30 0x00007a218ec4d317 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4263:0
#<!-- -->31 0x00007a218ec4d317 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4255:0
#<!-- -->32 0x00007a218ec4d317 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4270:0
#<!-- -->33 0x00007a218ec4d317 EvaluateAtomicConstraint /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:476:0
#<!-- -->34 0x00007a218ec4d317 calculateConstraintSatisfaction&lt;calculateConstraintSatisfaction(clang::Sema&amp;, const clang::NamedDecl*, clang::SourceLocation, const clang::MultiLevelTemplateArgumentList&amp;, const clang::Expr*, clang::ConstraintSatisfaction&amp;)::ConstraintEvaluator&gt; /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:351:0
#<!-- -->35 0x00007a218ec4ec0b CheckConstraintSatisfaction /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:600:0
#<!-- -->36 0x00007a218ec4f0d1 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:658:0
#<!-- -->37 0x00007a218ec50231 llvm::SmallVectorTemplateCommon&lt;clang::Expr*, void&gt;::isSmall() const /usr/include/llvm/ADT/SmallVector.h:157:0
#<!-- -->38 0x00007a218ec50231 llvm::SmallVectorImpl&lt;clang::Expr*&gt;::~SmallVectorImpl() /usr/include/llvm/ADT/SmallVector.h:617:0
#<!-- -->39 0x00007a218ec50231 llvm::SmallVector&lt;clang::Expr*, 1u&gt;::~SmallVector() /usr/include/llvm/ADT/SmallVector.h:1217:0
#<!-- -->40 0x00007a218ec50231 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::ConstraintSatisfaction&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaConcept.cpp:1137:0
#<!-- -->41 0x00007a218f1e88f0 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl&lt;clang::DeducedTemplateArgument&gt;&amp;, unsigned int, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, llvm::SmallVectorImpl&lt;clang::Sema::OriginalCallArg&gt; const*, bool, llvm::function_ref&lt;bool ()&gt;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateDeduction.cpp:3920:0
#<!-- -->42 0x00007a218f1e99f5 callback_fn&lt;clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool(llvm::ArrayRef&lt;clang::QualType&gt;)&gt;)::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:46:0
#<!-- -->43 0x00007a218eaee5cd clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:569:1
#<!-- -->44 0x00007a218f1f3128 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::FunctionDecl*&amp;, clang::sema::TemplateDeductionInfo&amp;, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref&lt;bool (llvm::ArrayRef&lt;clang::QualType&gt;)&gt;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateDeduction.cpp:4584:0
#<!-- -->45 0x00007a21906e6af2 clang::Sema::AddMethodTemplateCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, clang::OverloadCandidateParamOrder) (.constprop.0) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaOverload.cpp:7593:0
#<!-- -->46 0x00007a218f0f6ec1 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, clang::OverloadCandidateParamOrder) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaOverload.cpp:7374:0
#<!-- -->47 0x00007a218f0f7650 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaOverload.cpp:15573:0
#<!-- -->48 0x00007a218edea0d8 clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaExpr.cpp:6375:0
#<!-- -->49 0x00007a218f28a15a clang::Sema::FPFeaturesStateRAII::~FPFeaturesStateRAII() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:2737:0
#<!-- -->50 0x00007a218f28a15a TransformCallExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:12316:0
#<!-- -->51 0x00007a218f2848fe TransformDependentCoawaitExpr /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:8557:0
#<!-- -->52 0x00007a218f284b58 TransformCoreturnStmt /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:8520:0
#<!-- -->53 0x00007a218f29f8de TransformCompoundStmt /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:7831:0
#<!-- -->54 0x00007a218f29eaf7 TransformCoroutineBodyStmt /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:8434:0
#<!-- -->55 0x00007a218f2c88cb llvm::DenseMap&lt;clang::Decl*, clang::Decl*, llvm::DenseMapInfo&lt;clang::Decl*, void&gt;, llvm::detail::DenseMapPair&lt;clang::Decl*, clang::Decl*&gt;&gt;::~DenseMap() /usr/include/llvm/ADT/DenseMap.h:798:0
#<!-- -->56 0x00007a218f2c88cb ~TreeTransform /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/TreeTransform.h:109:0
#<!-- -->57 0x00007a218f2c88cb ~TemplateInstantiator /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:1337:0
#<!-- -->58 0x00007a218f2c88cb clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4234:0
#<!-- -->59 0x00007a218f2c88cb clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiate.cpp:4226:0
#<!-- -->60 0x00007a218f2c88cb clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:5186:0
#<!-- -->61 0x00007a218f2d49ec clang::FunctionDecl::isDefined() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Decl.h:2210:0
#<!-- -->62 0x00007a218f2d49ec clang::Sema::PerformPendingInstantiations(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:6411:0
#<!-- -->63 0x00007a218f2c89fd clang::LocalInstantiationScope::Exit() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Sema/Template.h:446:0
#<!-- -->64 0x00007a218f2c89fd clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:5209:0
#<!-- -->65 0x00007a218f2d49ec clang::FunctionDecl::isDefined() const /usr/src/debug/clang/clang-19.1.7.src/include/clang/AST/Decl.h:2210:0
#<!-- -->66 0x00007a218f2d49ec clang::Sema::PerformPendingInstantiations(bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/SemaTemplateInstantiateDecl.cpp:6411:0
#<!-- -->67 0x00007a218eb3f50d llvm::TimeTraceScope::~TimeTraceScope() /usr/include/llvm/Support/TimeProfiler.h:189:37
#<!-- -->68 0x00007a218eb3f50d clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:1146:3
#<!-- -->69 0x00007a218eb32660 clang::Sema::ActOnEndOfTranslationUnit() /usr/src/debug/clang/clang-19.1.7.src/lib/Sema/Sema.cpp:1184:0
#<!-- -->70 0x00007a218e2ce52a clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/Parser.cpp:732:12
#<!-- -->71 0x00007a218e1d9c18 clang::ParseAST(clang::Sema&amp;, bool, bool) /usr/src/debug/clang/clang-19.1.7.src/lib/Parse/ParseAST.cpp:170:0
#<!-- -->72 0x00007a218fefa221 clang::FrontendAction::Execute() /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/FrontendAction.cpp:1078:21
#<!-- -->73 0x00007a218fe9008f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:282:12
#<!-- -->74 0x00007a218fe9008f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:242:22
#<!-- -->75 0x00007a218fe9008f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Frontend/CompilerInstance.cpp:1061:42
#<!-- -->76 0x00007a218ff54ba5 std::__shared_ptr&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2&gt;::get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1667:0
#<!-- -->77 0x00007a218ff54ba5 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::_M_get() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1364:0
#<!-- -->78 0x00007a218ff54ba5 std::__shared_ptr_access&lt;clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false&gt;::operator*() const /usr/include/c++/14.2.1/bits/shared_ptr_base.h:1350:0
#<!-- -->79 0x00007a218ff54ba5 clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInvocation.h:259:0
#<!-- -->80 0x00007a218ff54ba5 clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-19.1.7.src/include/clang/Frontend/CompilerInstance.h:312:0
#<!-- -->81 0x00007a218ff54ba5 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-19.1.7.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:281:0
#<!-- -->82 0x0000560bcd624832 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/cc1_main.cpp:284:40
#<!-- -->83 0x0000560bcd62904e ExecuteCC1Tool /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:215:20
#<!-- -->84 0x00007a218fb64bb5 operator() /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:440:32
#<!-- -->85 0x00007a218fb64bb5 callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt; &gt;, std::string*, bool*) const::&lt;lambda()&gt; &gt; /usr/include/llvm/ADT/STLFunctionalExtras.h:45:52
#<!-- -->86 0x00007a2185b0176a llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) /usr/src/debug/llvm/llvm-19.1.7.src/lib/Support/CrashRecoveryContext.cpp:428:1
#<!-- -->87 0x00007a218fb6bb76 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Job.cpp:444:10
#<!-- -->88 0x00007a218fb22709 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:200:3
#<!-- -->89 0x00007a218fb22afc clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Compilation.cpp:253:5
#<!-- -->90 0x00007a218fb92a64 llvm::SmallVectorBase&lt;unsigned int&gt;::empty() const /usr/include/llvm/ADT/SmallVector.h:95:46
#<!-- -->91 0x00007a218fb92a64 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) /usr/src/debug/clang/clang-19.1.7.src/lib/Driver/Driver.cpp:1946:28
#<!-- -->92 0x0000560bcd62ad8c llvm::SmallVectorBase&lt;unsigned int&gt;::size() const /usr/include/llvm/ADT/SmallVector.h:92:32
#<!-- -->93 0x0000560bcd62ad8c llvm::SmallVectorTemplateCommon&lt;std::pair&lt;int, clang::driver::Command const*&gt;, void&gt;::end() /usr/include/llvm/ADT/SmallVector.h:283:41
#<!-- -->94 0x0000560bcd62ad8c clang_main(int, char**, llvm::ToolContext const&amp;) /usr/src/debug/clang/clang-19.1.7.src/tools/driver/driver.cpp:393:26
#<!-- -->95 0x0000560bcd61cf65 main /usr/src/debug/clang/clang-19.1.7.src/build/tools/driver/clang-driver.cpp:18:1
#<!-- -->96 0x00007a2184e35488 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->97 0x00007a2184e3554c call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->98 0x00007a2184e3554c __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->99 0x0000560bcd61cfc5 _start (/usr/bin/clang+++0xafc5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 19.1.7
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 20 or `main` branch?

---

### Comment 3 - jehelset

> Could you please try 20 or `main` branch?

also crashes on: clang version 21.0.0git (git@github.com:llvm/llvm-project.git 50a767add148601a1494760d02e237fe5cc57555)

---

### Comment 4 - oltolm

I reduced it
```cpp
int nullopt;
template < typename, int > struct array {}
template < typename, auto > struct bind_impl {
                  void  operator()( auto ...b)               requires                 ( CJ< sizeof...b >()          }
                template < int... I > struct bind_fn {
                    auto operator()(auto f) {
                      return bind_impl< decltype(f),
                                          array< int, sizeof...I >{} >{}}
                  }template < int... > bind_fn bind
                     ;
                    template < typename  > void  promise_awaiter() ;
                    template < int = 1 >
                    auto nil = bind<>(nullopt) struct Seed 
                      namespace auto f1 = [&]< typename PP1 >(PP1, auto) {
                        auto p1 = promise_awaiter< PP1 >;
                        nil<>(p1)
                      }auto f0 = [&]< typename PP0 >(PP0) {
                            auto g2 = [] {};
                        f1(promise_awaiter<  PP0  >, g2)
                      }(Seed{})
```
https://godbolt.org/z/G1MnszaYs

---

### Comment 5 - shafik

We have many w/ similar assertions but the backtrace are all different.

Assertion:

```console
clang++: /root/llvm-project/llvm/include/llvm/ADT/SmallVector.h:291:
T& llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::operator[](llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type) [with T = clang::sema::FunctionScopeInfo*; <template-parameter-1-2> = void; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::reference = clang::sema::FunctionScopeInfo*&; llvm::SmallVectorTemplateCommon<T, <template-parameter-1-2> >::size_type = long unsigned int]:
Assertion `idx < size()' failed.
```

backtrace:

```console
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -O2 -std=gnu++23 <source>
1.	<source>:20:31: current parser token ')'
2.	<source>:17:34: instantiating function definition '(anonymous class)::operator()<Seed>'
3.	<source>:14:43: instantiating function definition '(anonymous class)::operator()<void (*)(), (lambda at <source>:18:39)>'
 #0 0x0000000003f6bbd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6bbd8)
 #1 0x0000000003f69864 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f69864)
 #2 0x0000000003eae8e8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000718454242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007184542969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000718454242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007184542287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000071845422871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000718454239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006afd9d4 clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::Sema::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6afd9d4)
#10 0x0000000006affa86 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::DeclarationNameInfo const&, clang::NestedNameSpecifierLoc, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6affa86)
#11 0x0000000006b06db7 clang::Sema::BuildDeclRefExpr(clang::ValueDecl*, clang::QualType, clang::ExprValueKind, clang::SourceLocation, clang::CXXScopeSpec const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b06db7)
#12 0x00000000071ab0f9 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformSizeOfPackExpr(clang::SizeOfPackExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000071893f7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000071a9503 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000071a9d38 (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000071ab6c8 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000071b33dd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
#18 0x000000000718934e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x0000000007195b76 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#20 0x000000000718908a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#21 0x0000000007198cd2 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7198cd2)
#22 0x00000000068c9ccf calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#23 0x00000000068cbcc8 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#24 0x00000000068cc2e5 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68cc2e5)
#25 0x00000000068cdd48 clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68cdd48)
#26 0x000000000711ab98 clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#27 0x000000000711aff7 void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#28 0x0000000007ecaab1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ecaab1)
#29 0x00000000070f2cd5 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70f2cd5)
#30 0x0000000006f1bd91 AddMethodTemplateCandidateImmediately(clang::Sema&, clang::OverloadCandidateSet&, clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::TemplateArgumentListInfo*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, bool, bool, clang::OverloadCandidateParamOrder) SemaOverload.cpp:0:0
#31 0x0000000006f1e6e3 clang::OverloadCandidateSet::InjectNonDeducedTemplateCandidates(clang::Sema&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f1e6e3)
#32 0x0000000006f1e817 clang::OverloadCandidateSet::BestViableFunction(clang::Sema&, clang::SourceLocation, clang::OverloadCandidate*&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f1e817)
#33 0x0000000006f3c5bf clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3c5bf)
#34 0x0000000006b352be clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b352be)
#35 0x0000000006b35bbe clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b35bbe)
#36 0x0000000007195c7c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#37 0x000000000718908a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#38 0x00000000071c9fb7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#39 0x00000000071ca83e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#40 0x00000000071d3e54 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71d3e54)
#41 0x0000000007233807 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7233807)
#42 0x0000000007ecaab1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ecaab1)
#43 0x00000000070c025a clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70c025a)
#44 0x0000000006aec983 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aec983)
#45 0x0000000006efacc7 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#46 0x0000000006f3cd26 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3cd26)
#47 0x0000000006b352be clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b352be)
#48 0x0000000006b35bbe clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b35bbe)
#49 0x0000000007195c7c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#50 0x000000000718908a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#51 0x00000000071c9fb7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#52 0x00000000071ca83e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#53 0x00000000071d3e54 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71d3e54)
#54 0x0000000007233807 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7233807)
#55 0x0000000007ecaab1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ecaab1)
#56 0x00000000070c025a clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70c025a)
#57 0x0000000006aec983 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aec983)
#58 0x0000000006efacc7 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#59 0x0000000006f3cd26 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f3cd26)
#60 0x0000000006b352be clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b352be)
#61 0x0000000006b35bbe clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b35bbe)
#62 0x000000000666865d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666865d)
#63 0x000000000666106a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666106a)
#64 0x0000000006663207 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6663207)
#65 0x0000000006663299 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6663299)
#66 0x000000000661b3f8 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661b3f8)
#67 0x000000000662ac69 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662ac69)
#68 0x00000000065e8b4e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e8b4e)
#69 0x00000000065e9309 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e9309)
#70 0x00000000065f1013 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f1013)
#71 0x00000000065f1f2d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f1f2d)
#72 0x00000000065e3fca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e3fca)
#73 0x000000000490e588 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x490e588)
#74 0x0000000004c02f35 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c02f35)
#75 0x0000000004b8137e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b8137e)
#76 0x0000000004cf7d59 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cf7d59)
#77 0x0000000000da520f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda520f)
#78 0x0000000000d9befa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#79 0x0000000004974e39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#80 0x0000000003eaed84 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eaed84)
#81 0x000000000497544f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#82 0x000000000493789d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493789d)
#83 0x000000000493892e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x493892e)
#84 0x0000000004940785 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4940785)
#85 0x0000000000da1c98 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda1c98)
#86 0x0000000000c274c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc274c4)
#87 0x0000718454229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#88 0x0000718454229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#89 0x0000000000d9b9a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b9a5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

