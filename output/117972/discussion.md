# [Clang] occasional crash compiling a file SIGSEGV stack overflow

**Author:** molar
**URL:** https://github.com/llvm/llvm-project/issues/117972
**Status:** Closed
**Labels:** duplicate, clang:frontend, incomplete, crash
**Closed Date:** 2025-01-02T14:41:38Z

## Body


I am experiencing a crash in clang 19.1.4 when compiling files that clang 18.1.8 compiled without problems.
The crash happens in one out of maybe 200 attempts at compiling the same file. I managed to capture a core-dump 
of the crash and it shows the following

```
    frame #79568: 0x0000564cb93655cf clang`clang::Sema::CheckTemplateArgument(this=0x0000564cbe12e210, Param=0x0000564ccf569768, Arg=<unavailable>, Template=<unavailable>, TemplateLoc=(ID = 47679638), RAngleLoc=<unavailable>, ArgumentPackIndex=0, SugaredConverted=0x00007fff2acc2e80, CanonicalConverted=0x00007fff2acc2ef0, CTAK=CTAK_Specified) at SemaTemplate.cpp:5048:24
    frame #79569: 0x0000564cb935946f clang`clang::Sema::CheckTemplateArgumentList(this=0x0000564cbe12e210, Template=0x0000564ccf569880, TemplateLoc=(ID = 47679638), TemplateArgs=0x00007fff2acc2f60, PartialTemplateArgs=false, SugaredConverted=0x00007fff2acc2e80, CanonicalConverted=0x00007fff2acc2ef0, UpdateArgsWithConversions=<unavailable>, ConstraintsNotSatisfied=0x00007fff2acc2ba7, PartialOrderingTTP=<unavailable>) at SemaTemplate.cpp:5339:11
    frame #79570: 0x0000564cb94987f8 clang`std::enable_if<IsPartialSpecialization<clang::ClassTemplatePartialSpecializationDecl>::value, clang::TemplateDeductionResult>::type FinishTemplateArgumentDeduction<clang::ClassTemplatePartialSpecializationDecl>(S=<unavailable>, Partial=0x0000564ccf56cd68, IsPartialOrdering=<unavailable>, TemplateArgs=ArrayRef<clang::TemplateArgument> @ 0x00005631f2e5a880, Deduced=<unavailable>, Info=0x00007fff2acc35a0) at SemaTemplateDeduction.cpp:3176:9
    frame #79571: 0x0000564cb9497e41 clang`void llvm::function_ref<void ()>::callback_fn<std::enable_if<IsPartialSpecialization<clang::ClassTemplatePartialSpecializationDecl>::value, clang::TemplateDeductionResult>::type DeduceTemplateArguments<clang::ClassTemplatePartialSpecializationDecl>(clang::Sema&, clang::ClassTemplatePartialSpecializationDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::sema::TemplateDeductionInfo&)::'lambda'()>(long) [inlined] std::enable_if<IsPartialSpecialization<clang::ClassTemplatePartialSpecializationDecl>::value, clang::TemplateDeductionResult>::type DeduceTemplateArguments<clang::ClassTemplatePartialSpecializationDecl>(this=0x00007fff2acc3238)::'lambda'()::operator()() const at SemaTemplateDeduction.cpp:3339:14
    frame #79572: 0x0000564cb9497e18 clang`void llvm::function_ref<void ()>::callback_fn<std::enable_if<IsPartialSpecialization<clang::ClassTemplatePartialSpecializationDecl>::value, clang::TemplateDeductionResult>::type DeduceTemplateArguments<clang::ClassTemplatePartialSpecializationDecl>(clang::Sema&, clang::ClassTemplatePartialSpecializationDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::sema::TemplateDeductionInfo&)::'lambda'()>(callable=140733911413304) at STLFunctionalExtras.h:45:12
    frame #79573: 0x0000564cb8b79e8f clang`clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) [inlined] llvm::function_ref<void ()>::operator()(this=<unavailable>) const at STLFunctionalExtras.h:68:12
    frame #79574: 0x0000564cb8b79e89 clang`clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) [inlined] clang::runWithSufficientStackSpace(Diag=function_ref<void ()> @ 0x00005631f2e505a0, Fn=<unavailable>) at Stack.h:46:7
    frame #79575: 0x0000564cb8b79e80 clang`clang::Sema::runWithSufficientStackSpace(this=<unavailable>, Loc=(ID = 2195354835), Fn=function_ref<void ()> @ 0x00005631f2e505a0) at Sema.cpp:568:3
    frame #79576: 0x0000564cb941e3c0 clang`clang::Sema::DeduceTemplateArguments(clang::ClassTemplatePartialSpecializationDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::sema::TemplateDeductionInfo&) [inlined] std::enable_if<IsPartialSpecialization<clang::ClassTemplatePartialSpecializationDecl>::value, clang::TemplateDeductionResult>::type DeduceTemplateArguments<clang::ClassTemplatePartialSpecializationDecl>(S=<unavailable>, Partial=<unavailable>, TemplateArgs=<unavailable>, Info=<unavailable>) at SemaTemplateDeduction.cpp:3338:5
    frame #79577: 0x0000564cb941e07b clang`clang::Sema::DeduceTemplateArguments(this=0x0000564cbe12e210, Partial=0x0000564ccf56cd68, TemplateArgs=<unavailable>, Info=0x00007fff2acc35a0) at SemaTemplateDeduction.cpp:3350:10
    frame #79578: 0x0000564cb9514d34 clang`clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) [inlined] getPatternForClassTemplateSpecialization(S=0x0000564cbe12e210, PointOfInstantiation=(ID = 2195354835), ClassTemplateSpec=0x0000564cd440f5a8, TSK=TSK_ImplicitInstantiation) at SemaTemplateInstantiate.cpp:3843:46
    frame #79579: 0x0000564cb9514b05 clang`clang::Sema::InstantiateClassTemplateSpecialization(this=0x0000564cbe12e210, PointOfInstantiation=(ID = 2195354835), ClassTemplateSpec=0x0000564cd440f5a8, TSK=TSK_ImplicitInstantiation, Complain=true) at SemaTemplateInstantiate.cpp:3968:7
    frame #79580: 0x0000564cb9637aae clang`void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0>(long) [inlined] clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0::operator()(this=0x00007fff2acc3c68) const at SemaType.cpp:9163:23
    frame #79581: 0x0000564cb9637a88 clang`void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0>(callable=140733911415912) at STLFunctionalExtras.h:45:12
    frame #79582: 0x0000564cb8b79e8f clang`clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) [inlined] llvm::function_ref<void ()>::operator()(this=<unavailable>) const at STLFunctionalExtras.h:68:12
    frame #79583: 0x0000564cb8b79e89 clang`clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) [inlined] clang::runWithSufficientStackSpace(Diag=function_ref<void ()> @ 0x00005631f2e50580, Fn=<unavailable>) at Stack.h:46:7
    frame #79584: 0x0000564cb8b79e80 clang`clang::Sema::runWithSufficientStackSpace(this=<unavailable>, Loc=(ID = 2195354835), Fn=function_ref<void ()> @ 0x00005631f2e50580) at Sema.cpp:568:3
    frame #79585: 0x0000564cb961eb45 clang`clang::Sema::RequireCompleteTypeImpl(this=0x0000564cbe12e210, Loc=(ID = 2195354835), T=QualType @ 0x00007fff2acc3c50, Kind=AcceptSizeless, Diagnoser=0x00007fff2acc3de8) at SemaType.cpp:0:9
    frame #79586: 0x0000564cb961e380 clang`clang::Sema::RequireCompleteType(this=<unavailable>, Loc=<unavailable>, T=<unavailable>, Kind=<unavailable>, Diagnoser=<unavailable>) at SemaType.cpp:8888:7
  * frame #79587: 0x0000564cb8d9e8e8 clang`clang::Sema::CheckBaseSpecifier(clang::CXXRecordDecl*, clang::SourceRange, bool, clang::AccessSpecifier, clang::TypeSourceInfo*, clang::SourceLocation) [inlined] clang::Sema::RequireCompleteType(this=0x0000564cbe12e210, Loc=(ID = 2195354835), T=QualType @ rbp, Diagnoser=0x00007fff2acc3de8) at Sema.h:14946:12
    frame #79588: 0x0000564cb8d9e8d4 clang`clang::Sema::CheckBaseSpecifier(clang::CXXRecordDecl*, clang::SourceRange, bool, clang::AccessSpecifier, clang::TypeSourceInfo*, clang::SourceLocation) [inlined] bool clang::Sema::RequireCompleteType<clang::SourceRange>(this=0x0000564cbe12e210, Loc=(ID = 2195354835), T=QualType @ rbp, DiagID=3616, Args=0x00007fff2acc3dc8) at Sema.h:14956:12
    frame #79589: 0x0000564cb8d9e8ad clang`clang::Sema::CheckBaseSpecifier(this=0x0000564cbe12e210, Class=0x0000564cd440f068, SpecifierRange=SourceRange @ 0x00007fff2acc3dc8, Virtual=false, Access=AS_none, TInfo=0x0000564cd440f790, EllipsisLoc=(ID = 0)) at SemaDeclCXX.cpp:2676:9
    frame #79590: 0x0000564cb8d9f2d5 clang`clang::Sema::ActOnBaseSpecifier(this=0x0000564cbe12e210, classdecl=0x0000564cd440f068, SpecifierRange=SourceRange @ 0x00007fff2acc3f20, Attributes=<unavailable>, Virtual=false, Access=AS_none, basetype=(Ptr = 0x0000564cd438fb40), BaseLoc=(ID = 2195354847), EllipsisLoc=(ID = 0)) at SemaDeclCXX.cpp:2796:36
    frame #79591: 0x0000564cb8a7b79c clang`clang::Parser::ParseBaseSpecifier(this=0x0000564cbe139b90, ClassDecl=<unavailable>) at ParseDeclCXX.cpp:2490:18
    frame #79592: 0x0000564cb8a7b06b clang`clang::Parser::ParseBaseClause(this=0x0000564cbe139b90, ClassDecl=0x0000564cd440f068) at ParseDeclCXX.cpp:2394:25
    frame #79593: 0x0000564cb8a7a503 clang`clang::Parser::ParseCXXMemberSpecification(this=0x0000564cbe139b90, RecordLoc=(ID = 2195354821), AttrFixitLoc=<unavailable>, Attrs=0x00007fff2acc4570, TagType=25, TagDecl=0x0000564cd440f068) at ParseDeclCXX.cpp:3770:5
    frame #79594: 0x0000564cb8a78298 clang`clang::Parser::ParseClassSpecifier(this=0x0000564cbe139b90, TagTokKind=kw_struct, StartLoc=(ID = 2195354821), DS=0x00007fff2acc5280, TemplateInfo=0x00007fff2acc51f8, AS=AS_none, EnteringContext=<unavailable>, DSC=<unavailable>, Attributes=<unavailable>) at ParseDeclCXX.cpp:2312:7
    frame #79595: 0x0000564cb8a51e01 clang`clang::Parser::ParseDeclarationSpecifiers(this=0x0000564cbe139b90, DS=0x00007fff2acc5280, TemplateInfo=0x00007fff2acc51f8, AS=AS_none, DSContext=DSC_top_level, LateAttrs=0x0000000000000000, AllowImplicitTypename=Yes) at ParseDecl.cpp:4663:7
    frame #79596: 0x0000564cb8a24b29 clang`clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) [inlined] clang::Parser::ParseDeclarationSpecifiers(this=0x0000564cbe139b90, DS=0x00007fff2acc5280, TemplateInfo=0x00007fff2acc51f8, AS=AS_none, DSC=DSC_top_level, LateAttrs=0x0000000000000000) at Parser.h:2509:12
    frame #79597: 0x0000564cb8a24b06 clang`clang::Parser::ParseDeclOrFunctionDefInternal(this=0x0000564cbe139b90, Attrs=0x00007fff2acc56b8, DeclSpecAttrs=0x00007fff2acc5748, DS=0x00007fff2acc5280, AS=AS_none) at Parser.cpp:1153:3
    frame #79598: 0x0000564cb8a24772 clang`clang::Parser::ParseDeclarationOrFunctionDefinition(this=0x0000564cbe139b90, Attrs=0x00007fff2acc56b8, DeclSpecAttrs=0x00007fff2acc5748, DS=<unavailable>, AS=AS_none) at Parser.cpp:1271:12
    frame #79599: 0x0000564cb8a23559 clang`clang::Parser::ParseExternalDeclaration(this=0x0000564cbe139b90, Attrs=<unavailable>, DeclSpecAttrs=<unavailable>, DS=<unavailable>) at Parser.cpp:1074:14
    frame #79600: 0x0000564cb8a6de4f clang`clang::Parser::ParseInnerNamespace(this=0x0000564cbe139b90, InnerNSs=0x00007fff2acc5a60, index=1, InlineLoc=0x00007fff2acc59f4, attrs=0x00007fff2acc5b98, Tracker=0x00007fff2acc5b50) at ParseDeclCXX.cpp:272:7
    frame #79601: 0x0000564cb8a6df59 clang`clang::Parser::ParseInnerNamespace(this=0x0000564cbe139b90, InnerNSs=0x00007fff2acc5a60, index=1, InlineLoc=0x00007fff2acc59f4, attrs=0x00007fff2acc5b98, Tracker=0x00007fff2acc5b50) at ParseDeclCXX.cpp:294:3
    frame #79602: 0x0000564cb8a6d2a4 clang`clang::Parser::ParseNamespace(this=0x0000564cbe139b90, Context=<unavailable>, DeclEnd=<unavailable>, InlineLoc=(ID = 0)) at ParseDeclCXX.cpp:249:3
    frame #79603: 0x0000564cb8a4b30b clang`clang::Parser::ParseDeclaration(this=0x0000564cbe139b90, Context=File, DeclEnd=0x00007fff2acc5ed8, DeclAttrs=0x00007fff2acc6080, DeclSpecAttrs=0x00007fff2acc5ff0, DeclSpecStart=0x0000000000000000) at ParseDecl.cpp:2044:12
    frame #79604: 0x0000564cb8a230ad clang`clang::Parser::ParseExternalDeclaration(this=0x0000564cbe139b90, Attrs=0x00007fff2acc6080, DeclSpecAttrs=0x00007fff2acc5ff0, DS=0x0000000000000000) at Parser.cpp:0
    frame #79605: 0x0000564cb8a21566 clang`clang::Parser::ParseTopLevelDecl(this=0x0000564cbe139b90, Result=0x00007fff2acc6170, ImportState=0x00007fff2acc6184) at Parser.cpp:763:12
    frame #79606: 0x0000564cb8a1c9fe clang`clang::ParseAST(S=0x0000564cbe12e210, PrintStats=false, SkipFunctionBodies=<unavailable>) at ParseAST.cpp:171:20
    frame #79607: 0x0000564cb70f477f clang`clang::FrontendAction::Execute(this=0x0000564cbe0e56d0) at FrontendAction.cpp:1078:8
    frame #79608: 0x0000564cb7060fed clang`clang::CompilerInstance::ExecuteAction(this=0x0000564cbe0e3360, Act=0x0000564cbe0e56d0) at CompilerInstance.cpp:1061:33
    frame #79609: 0x0000564cb71cd737 clang`clang::ExecuteCompilerInvocation(Clang=0x0000564cbe0e3360) at ExecuteCompilerInvocation.cpp:280:25
    frame #79610: 0x0000564cb4fabe14 clang`cc1_main(Argv=ArrayRef<const char *> @ 0x00005631f2e574f0, Argv0=<unavailable>, MainAddr=0x0000564cb4fa6730) at cc1_main.cpp:284:15
    frame #79611: 0x0000564cb4fa8bbe clang`ExecuteCC1Tool(ArgV=<unavailable>, ToolContext=0x00007fff2acc72f0) at driver.cpp:215:12
    frame #79612: 0x0000564cb6e8e959 clang`void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) [inlined] llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::operator()(this=<unavailable>, params=<unavailable>) const at STLFunctionalExtras.h:68:12
    frame #79613: 0x0000564cb6e8e94c clang`void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(long) [inlined] clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0::operator()(this=0x00007fff2acc6cb8) const at Job.cpp:440:34
    frame #79614: 0x0000564cb6e8e944 clang`void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::$_0>(callable=140733911428280) at STLFunctionalExtras.h:45:12
    frame #79615: 0x0000564cb6625756 clang`llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) [inlined] llvm::function_ref<void ()>::operator()(this=<unavailable>) const at STLFunctionalExtras.h:68:12
    frame #79616: 0x0000564cb6625751 clang`llvm::CrashRecoveryContext::RunSafely(this=<unavailable>, Fn=function_ref<void ()> @ 0x00005631f2e50580) at CrashRecoveryContext.cpp:426:3
    frame #79617: 0x0000564cb6e8e2a2 clang`clang::driver::CC1Command::Execute(this=0x0000564cbe057f30, Redirects=<unavailable>, ErrMsg=<unavailable>, ExecutionFailed=<unavailable>) const at Job.cpp:440:12
    frame #79618: 0x0000564cb6e49f27 clang`clang::driver::Compilation::ExecuteCommand(this=0x0000564cbe0c01e0, C=0x0000564cbe057f30, FailingCommand=0x00007fff2acc71d8, LogOnly=false) const at Compilation.cpp:199:15
    frame #79619: 0x0000564cb6e4a45e clang`clang::driver::Compilation::ExecuteJobs(this=0x0000564cbe0c01e0, Jobs=<unavailable>, FailingCommands=0x00007fff2acc72f0, LogOnly=<unavailable>) const at Compilation.cpp:253:19
    frame #79620: 0x0000564cb6e6ab1f clang`clang::driver::Driver::ExecuteCompilation(this=0x00007fff2acc74b8, C=0x0000564cbe0c01e0, FailingCommands=0x00007fff2acc72f0) at Driver.cpp:1943:5
    frame #79621: 0x0000564cb4fa81d6 clang`clang_main(Argc=<unavailable>, Argv=<unavailable>, ToolContext=0x00007fff2acc86d8) at driver.cpp:391:21
    frame #79622: 0x0000564cb4fb7977 clang`main(argc=1915, argv=0x00007fff2acc88a8) at clang-driver.cpp:17:10
    frame #79623: 0x00007f0db544dd90 libc.so.6`__libc_start_call_main(main=(clang`main at clang-driver.cpp:15), argc=1915, argv=0x00007fff2acc88a8) at libc_start_call_main.h:58:16
    frame #79624: 0x00007f0db544de40 libc.so.6`__libc_start_main_impl(main=(clang`main at clang-driver.cpp:15), argc=1915, argv=0x00007fff2acc88a8, init=<unavailable>, fini=<unavailable>, rtld_fini=<unavailable>, stack_end=0x00007fff2acc8898) at libc-start.c:392:3
``` 


and ends with the last 50 frames being 

```
* thread #1, name = 'clang', stop reason = signal SIGSEGV: invalid address
    frame #0: 0x0000564cb9ecd7c1 clang`(anonymous namespace)::StmtProfilerWithPointers::HandleStmtClass(this=0x00007fff2a4dc088, SC=CallExprClass) at StmtProfile.cpp:93
    frame #1: 0x0000564cb9ed2995 clang`(anonymous namespace)::StmtProfiler::VisitStmt(clang::Stmt const*) [inlined] (anonymous namespace)::StmtProfiler::VisitStmtNoChildren(this=0x00007fff2a4dc088, S=<unavailable>) at StmtProfile.cpp:44:7
    frame #2: 0x0000564cb9ed298a clang`(anonymous namespace)::StmtProfiler::VisitStmt(this=<unavailable>, S=0x0000564cd42c2930) at StmtProfile.cpp:241:3
    frame #3: 0x0000564cb9ecd1f3 clang`clang::Stmt::Profile(this=<unavailable>, ID=<unavailable>, Context=<unavailable>, Canonical=<unavailable>, ProfileLambdaExpr=<unavailable>) const at StmtProfile.cpp:2637:12
    frame #4: 0x0000564cb9f2cbee clang`clang::AutoType::Profile(ID=0x00007fff2a4dc148, Context=0x0000564cbe1f8de0, Deduced=<unavailable>, Keyword=<unavailable>, IsDependent=<unavailable>, CD=<unavailable>, Arguments=ArrayRef<clang::TemplateArgument> @ 0x00007fff2a4dc0e0) at Type.cpp:5091:9
    frame #5: 0x0000564cb9f2ccaa clang`clang::AutoType::Profile(this=<unavailable>, ID=<unavailable>, Context=<unavailable>) at Type.cpp:5095:3
    frame #6: 0x0000564cb9989b94 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&, unsigned int, llvm::FoldingSetNodeID&) [inlined] llvm::DefaultContextualFoldingSetTrait<clang::AutoType, clang::ASTContext&>::Profile(X=<unavailable>, ID=0x00007fff2a4dc148, Context=<unavailable>) at FoldingSet.h:270:7
    frame #7: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&, unsigned int, llvm::FoldingSetNodeID&) [inlined] llvm::DefaultContextualFoldingSetTrait<clang::AutoType, clang::ASTContext&>::Equals(X=<unavailable>, ID=0x00007fff2a4dc2a8, (null)=<unavailable>, TempID=0x00007fff2a4dc148, Context=<unavailable>) at FoldingSet.h:436:3
    frame #8: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(Base=<unavailable>, N=<unavailable>, ID=0x00007fff2a4dc2a8, IDHash=<unavailable>, TempID=<unavailable>) at FoldingSet.h:616:12
    frame #9: 0x0000564cb662ca2c clang`llvm::FoldingSetBase::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dc2a8, InsertPos=0x00007fff2a4dc248, Info=0x0000564cbd0a6e68) at FoldingSet.cpp:288:9
    frame #10: 0x0000564cb996001b clang`clang::ASTContext::getAutoTypeInternal(clang::QualType, clang::AutoTypeKeyword, bool, bool, clang::ConceptDecl*, llvm::ArrayRef<clang::TemplateArgument>, bool) const [inlined] llvm::FoldingSetImpl<llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>, clang::AutoType>::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dc2a8, InsertPos=0x00007fff2a4dc248) at FoldingSet.h:506:45
    frame #11: 0x0000564cb9960004 clang`clang::ASTContext::getAutoTypeInternal(this=0x0000564cbe1f8de0, DeducedType=QualType @ rbx, Keyword=Auto, IsDependent=true, IsPack=false, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=ArrayRef<clang::TemplateArgument> @ 0x00007fff2a4dc500, IsCanon=false) const at ASTContext.cpp:6198:32
    frame #12: 0x0000564cb992b116 clang`clang::ASTContext::getUnconstrainedType(clang::QualType) const [inlined] clang::ASTContext::getAutoType(this=0x0000564cbe1f8de0, DeducedType=QualType @ scalar, Keyword=<unavailable>, IsDependent=<unavailable>, IsPack=<unavailable>, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=<unavailable>) const at ASTContext.cpp:6247:10
    frame #13: 0x0000564cb992b0f2 clang`clang::ASTContext::getUnconstrainedType(this=0x0000564cbe1f8de0, T=QualType @ rbx) const at ASTContext.cpp:6258:29
    frame #14: 0x0000564cb9ecda98 clang`(anonymous namespace)::StmtProfilerWithPointers::VisitDecl(this=<unavailable>, D=<unavailable>) at StmtProfile.cpp:114:29
    frame #15: 0x0000564cb9ed062e clang`(anonymous namespace)::StmtProfiler::VisitDeclRefExpr(this=0x00007fff2a4dc638, S=0x0000564cd42c2910) at StmtProfile.cpp:1328:3
    frame #16: 0x0000564cb9ed2a09 clang`(anonymous namespace)::StmtProfiler::VisitStmt(this=0x00007fff2a4dc638, S=<unavailable>) at StmtProfile.cpp:245:7
    frame #17: 0x0000564cb9ecd1f3 clang`clang::Stmt::Profile(this=<unavailable>, ID=<unavailable>, Context=<unavailable>, Canonical=<unavailable>, ProfileLambdaExpr=<unavailable>) const at StmtProfile.cpp:2637:12
    frame #18: 0x0000564cb9f2cbee clang`clang::AutoType::Profile(ID=0x00007fff2a4dc6f8, Context=0x0000564cbe1f8de0, Deduced=<unavailable>, Keyword=<unavailable>, IsDependent=<unavailable>, CD=<unavailable>, Arguments=ArrayRef<clang::TemplateArgument> @ 0x00007fff2a4dc690) at Type.cpp:5091:9
    frame #19: 0x0000564cb9f2ccaa clang`clang::AutoType::Profile(this=<unavailable>, ID=<unavailable>, Context=<unavailable>) at Type.cpp:5095:3
    frame #20: 0x0000564cb9989b94 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&, unsigned int, llvm::FoldingSetNodeID&) [inlined] llvm::DefaultContextualFoldingSetTrait<clang::AutoType, clang::ASTContext&>::Profile(X=<unavailable>, ID=0x00007fff2a4dc6f8, Context=<unavailable>) at FoldingSet.h:270:7
    frame #21: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&, unsigned int, llvm::FoldingSetNodeID&) [inlined] llvm::DefaultContextualFoldingSetTrait<clang::AutoType, clang::ASTContext&>::Equals(X=<unavailable>, ID=0x00007fff2a4dc858, (null)=<unavailable>, TempID=0x00007fff2a4dc6f8, Context=<unavailable>) at FoldingSet.h:436:3
    frame #22: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(Base=<unavailable>, N=<unavailable>, ID=0x00007fff2a4dc858, IDHash=<unavailable>, TempID=<unavailable>) at FoldingSet.h:616:12
    frame #23: 0x0000564cb662ca2c clang`llvm::FoldingSetBase::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dc858, InsertPos=0x00007fff2a4dc7f8, Info=0x0000564cbd0a6e68) at FoldingSet.cpp:288:9
    frame #24: 0x0000564cb996001b clang`clang::ASTContext::getAutoTypeInternal(clang::QualType, clang::AutoTypeKeyword, bool, bool, clang::ConceptDecl*, llvm::ArrayRef<clang::TemplateArgument>, bool) const [inlined] llvm::FoldingSetImpl<llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>, clang::AutoType>::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dc858, InsertPos=0x00007fff2a4dc7f8) at FoldingSet.h:506:45
    frame #25: 0x0000564cb9960004 clang`clang::ASTContext::getAutoTypeInternal(this=0x0000564cbe1f8de0, DeducedType=QualType @ rbx, Keyword=Auto, IsDependent=true, IsPack=false, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=ArrayRef<clang::TemplateArgument> @ 0x00007fff2a4dcab0, IsCanon=false) const at ASTContext.cpp:6198:32
    frame #26: 0x0000564cb992b116 clang`clang::ASTContext::getUnconstrainedType(clang::QualType) const [inlined] clang::ASTContext::getAutoType(this=0x0000564cbe1f8de0, DeducedType=QualType @ scalar, Keyword=<unavailable>, IsDependent=<unavailable>, IsPack=<unavailable>, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=<unavailable>) const at ASTContext.cpp:6247:10
    frame #27: 0x0000564cb992b0f2 clang`clang::ASTContext::getUnconstrainedType(this=0x0000564cbe1f8de0, T=QualType @ rbx) const at ASTContext.cpp:6258:29
    frame #28: 0x0000564cb9ecda98 clang`(anonymous namespace)::StmtProfilerWithPointers::VisitDecl(this=<unavailable>, D=<unavailable>) at StmtProfile.cpp:114:29
    frame #29: 0x0000564cb9ed062e clang`(anonymous namespace)::StmtProfiler::VisitDeclRefExpr(this=0x00007fff2a4dcbe8, S=0x0000564cd42c2910) at StmtProfile.cpp:1328:3
    frame #30: 0x0000564cb9ed2a09 clang`(anonymous namespace)::StmtProfiler::VisitStmt(this=0x00007fff2a4dcbe8, S=<unavailable>) at StmtProfile.cpp:245:7
    frame #31: 0x0000564cb9ecd1f3 clang`clang::Stmt::Profile(this=<unavailable>, ID=<unavailable>, Context=<unavailable>, Canonical=<unavailable>, ProfileLambdaExpr=<unavailable>) const at StmtProfile.cpp:2637:12
    frame #32: 0x0000564cb9f2cbee clang`clang::AutoType::Profile(ID=0x00007fff2a4dcca8, Context=0x0000564cbe1f8de0, Deduced=<unavailable>, Keyword=<unavailable>, IsDependent=<unavailable>, CD=<unavailable>, Arguments=ArrayRef<clang::TemplateArgument> @ 0x00007fff2a4dcc40) at Type.cpp:5091:9
    frame #33: 0x0000564cb9f2ccaa clang`clang::AutoType::Profile(this=<unavailable>, ID=<unavailable>, Context=<unavailable>) at Type.cpp:5095:3
    frame #34: 0x0000564cb9989b94 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&, unsigned int, llvm::FoldingSetNodeID&) [inlined] llvm::DefaultContextualFoldingSetTrait<clang::AutoType, clang::ASTContext&>::Profile(X=<unavailable>, ID=0x00007fff2a4dcca8, Context=<unavailable>) at FoldingSet.h:270:7
    frame #35: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&, unsigned int, llvm::FoldingSetNodeID&) [inlined] llvm::DefaultContextualFoldingSetTrait<clang::AutoType, clang::ASTContext&>::Equals(X=<unavailable>, ID=0x00007fff2a4dce08, (null)=<unavailable>, TempID=0x00007fff2a4dcca8, Context=<unavailable>) at FoldingSet.h:436:3
    frame #36: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(Base=<unavailable>, N=<unavailable>, ID=0x00007fff2a4dce08, IDHash=<unavailable>, TempID=<unavailable>) at FoldingSet.h:616:12
    frame #37: 0x0000564cb662ca2c clang`llvm::FoldingSetBase::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dce08, InsertPos=0x00007fff2a4dcda8, Info=0x0000564cbd0a6e68) at FoldingSet.cpp:288:9
    frame #38: 0x0000564cb996001b clang`clang::ASTContext::getAutoTypeInternal(clang::QualType, clang::AutoTypeKeyword, bool, bool, clang::ConceptDecl*, llvm::ArrayRef<clang::TemplateArgument>, bool) const [inlined] llvm::FoldingSetImpl<llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>, clang::AutoType>::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dce08, InsertPos=0x00007fff2a4dcda8) at FoldingSet.h:506:45
    frame #39: 0x0000564cb9960004 clang`clang::ASTContext::getAutoTypeInternal(this=0x0000564cbe1f8de0, DeducedType=QualType @ rbx, Keyword=Auto, IsDependent=true, IsPack=false, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=ArrayRef<clang::TemplateArgument> @ 0x00007fff2a4dd060, IsCanon=false) const at ASTContext.cpp:6198:32
    frame #40: 0x0000564cb992b116 clang`clang::ASTContext::getUnconstrainedType(clang::QualType) const [inlined] clang::ASTContext::getAutoType(this=0x0000564cbe1f8de0, DeducedType=QualType @ scalar, Keyword=<unavailable>, IsDependent=<unavailable>, IsPack=<unavailable>, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=<unavailable>) const at ASTContext.cpp:6247:10
    frame #41: 0x0000564cb992b0f2 clang`clang::ASTContext::getUnconstrainedType(this=0x0000564cbe1f8de0, T=QualType @ rbx) const at ASTContext.cpp:6258:29
    frame #42: 0x0000564cb9ecda98 clang`(anonymous namespace)::StmtProfilerWithPointers::VisitDecl(this=<unavailable>, D=<unavailable>) at StmtProfile.cpp:114:29
    frame #43: 0x0000564cb9ed062e clang`(anonymous namespace)::StmtProfiler::VisitDeclRefExpr(this=0x00007fff2a4dd198, S=0x0000564cd42c2910) at StmtProfile.cpp:1328:3
    frame #44: 0x0000564cb9ed2a09 clang`(anonymous namespace)::StmtProfiler::VisitStmt(this=0x00007fff2a4dd198, S=<unavailable>) at StmtProfile.cpp:245:7
    frame #45: 0x0000564cb9ecd1f3 clang`clang::Stmt::Profile(this=<unavailable>, ID=<unavailable>, Context=<unavailable>, Canonical=<unavailable>, ProfileLambdaExpr=<unavailable>) const at StmtProfile.cpp:2637:12
    frame #46: 0x0000564cb9f2cbee clang`clang::AutoType::Profile(ID=0x00007fff2a4dd258, Context=0x0000564cbe1f8de0, Deduced=<unavailable>, Keyword=<unavailable>, IsDependent=<unavailable>, CD=<unavailable>, Arguments=ArrayRef<clang::TemplateArgument> @ 0x00007fff2a4dd1f0) at Type.cpp:5091:9
    frame #47: 0x0000564cb9f2ccaa clang`clang::AutoType::Profile(this=<unavailable>, ID=<unavailable>, Context=<unavailable>) at Type.cpp:5095:3
    frame #48: 0x0000564cb9989b94 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&, unsigned int, llvm::FoldingSetNodeID&) [inlined] llvm::DefaultContextualFoldingSetTrait<clang::AutoType, clang::ASTContext&>::Profile(X=<unavailable>, ID=0x00007fff2a4dd258, Context=<unavailable>) at FoldingSet.h:270:7
    frame #49: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet<clang::AutoType, clang::ASTContext&>::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&, unsigned int, llvm::FoldingSetNodeID&) [inlined] llvm::DefaultContextualFoldingSetTrait<clang::AutoType, clang::ASTContext&>::Equals(X=<unavailable>, ID=0x00007fff2a4dd3b8, (null)=<unavailable>, TempID=0x00007fff2a4dd258, Context=<unavailable>) at FoldingSet.h:436:3

```

If i print the variables in frame 18 i get the following
```
(clang::QualType) Deduced = <Could not evaluate DW_OP_entry_value.>

(clang::AutoTypeKeyword) Keyword = <Could not evaluate DW_OP_entry_value.>

(bool) IsDependent = <variable not available>

(clang::ConceptDecl *) CD = <Could not evaluate DW_OP_entry_value.>

(llvm::ArrayRef<clang::TemplateArgument>) Arguments = {
  Data = 0x0000564cd42f5a00
  Length = 1
}
(const clang::TemplateArgument &) Arg = 0x0000564cd42f5a00: {
   = {
    DeclArg = {
      Kind = Expression
      IsDefaulted = false
      QT = 0x0000564cd42c2930
      D = 0x0000564cd42c28c0
    }
    Integer = {
      Kind = Expression
      IsDefaulted = false
      BitWidth = 0
      IsUnsigned = false
       = {
        VAL = 94887977167152
        pVal = 0x0000564cd42c2930
      }
      Type = 0x0000564cd42c28c0
    }
    Value = {
      Kind = Expression
      IsDefaulted = false
      Value = 0x0000564cd42c2930
      Type = 0x0000564cd42c28c0
    }
    Args = {
      Kind = Expression
      IsDefaulted = false
      NumArgs = 0
      Args = 0x0000564cd42c2930
    }
    TemplateArg = (Kind = Expression, IsDefaulted = false, NumExpansions = 0, Name = 0x0000564cd42c2930)
    TypeOrValue = (Kind = Expression, IsDefaulted = false, V = 94887977167152)
  }
}
``` 

It looks like the template argument values are way off. Given the nature of the crash it is hard to come up with a
minimal reproducer, as most attempts at compiling the file succeeds. 

I have tried on multiple x86_64 machines, and with both the clang from the apt repository and a locally compiled version of (19.1.4)
and they both crash at some point compiling the same file. 




## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Morten larsen (molar)

<details>

I am experiencing a crash in clang 19.1.4 when compiling files that clang 18.1.8 compiled without problems.
The crash happens in one out of maybe 200 attempts at compiling the same file. I managed to capture a core-dump 
of the crash and it shows the following

```
    frame #<!-- -->79568: 0x0000564cb93655cf clang`clang::Sema::CheckTemplateArgument(this=0x0000564cbe12e210, Param=0x0000564ccf569768, Arg=&lt;unavailable&gt;, Template=&lt;unavailable&gt;, TemplateLoc=(ID = 47679638), RAngleLoc=&lt;unavailable&gt;, ArgumentPackIndex=0, SugaredConverted=0x00007fff2acc2e80, CanonicalConverted=0x00007fff2acc2ef0, CTAK=CTAK_Specified) at SemaTemplate.cpp:5048:24
    frame #<!-- -->79569: 0x0000564cb935946f clang`clang::Sema::CheckTemplateArgumentList(this=0x0000564cbe12e210, Template=0x0000564ccf569880, TemplateLoc=(ID = 47679638), TemplateArgs=0x00007fff2acc2f60, PartialTemplateArgs=false, SugaredConverted=0x00007fff2acc2e80, CanonicalConverted=0x00007fff2acc2ef0, UpdateArgsWithConversions=&lt;unavailable&gt;, ConstraintsNotSatisfied=0x00007fff2acc2ba7, PartialOrderingTTP=&lt;unavailable&gt;) at SemaTemplate.cpp:5339:11
    frame #<!-- -->79570: 0x0000564cb94987f8 clang`std::enable_if&lt;IsPartialSpecialization&lt;clang::ClassTemplatePartialSpecializationDecl&gt;::value, clang::TemplateDeductionResult&gt;::type FinishTemplateArgumentDeduction&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(S=&lt;unavailable&gt;, Partial=0x0000564ccf56cd68, IsPartialOrdering=&lt;unavailable&gt;, TemplateArgs=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00005631f2e5a880, Deduced=&lt;unavailable&gt;, Info=0x00007fff2acc35a0) at SemaTemplateDeduction.cpp:3176:9
    frame #<!-- -->79571: 0x0000564cb9497e41 clang`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;std::enable_if&lt;IsPartialSpecialization&lt;clang::ClassTemplatePartialSpecializationDecl&gt;::value, clang::TemplateDeductionResult&gt;::type DeduceTemplateArguments&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(clang::Sema&amp;, clang::ClassTemplatePartialSpecializationDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::sema::TemplateDeductionInfo&amp;)::'lambda'()&gt;(long) [inlined] std::enable_if&lt;IsPartialSpecialization&lt;clang::ClassTemplatePartialSpecializationDecl&gt;::value, clang::TemplateDeductionResult&gt;::type DeduceTemplateArguments&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(this=0x00007fff2acc3238)::'lambda'()::operator()() const at SemaTemplateDeduction.cpp:3339:14
    frame #<!-- -->79572: 0x0000564cb9497e18 clang`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;std::enable_if&lt;IsPartialSpecialization&lt;clang::ClassTemplatePartialSpecializationDecl&gt;::value, clang::TemplateDeductionResult&gt;::type DeduceTemplateArguments&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(clang::Sema&amp;, clang::ClassTemplatePartialSpecializationDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::sema::TemplateDeductionInfo&amp;)::'lambda'()&gt;(callable=140733911413304) at STLFunctionalExtras.h:45:12
    frame #<!-- -->79573: 0x0000564cb8b79e8f clang`clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) [inlined] llvm::function_ref&lt;void ()&gt;::operator()(this=&lt;unavailable&gt;) const at STLFunctionalExtras.h:68:12
    frame #<!-- -->79574: 0x0000564cb8b79e89 clang`clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) [inlined] clang::runWithSufficientStackSpace(Diag=function_ref&lt;void ()&gt; @ 0x00005631f2e505a0, Fn=&lt;unavailable&gt;) at Stack.h:46:7
    frame #<!-- -->79575: 0x0000564cb8b79e80 clang`clang::Sema::runWithSufficientStackSpace(this=&lt;unavailable&gt;, Loc=(ID = 2195354835), Fn=function_ref&lt;void ()&gt; @ 0x00005631f2e505a0) at Sema.cpp:568:3
    frame #<!-- -->79576: 0x0000564cb941e3c0 clang`clang::Sema::DeduceTemplateArguments(clang::ClassTemplatePartialSpecializationDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::sema::TemplateDeductionInfo&amp;) [inlined] std::enable_if&lt;IsPartialSpecialization&lt;clang::ClassTemplatePartialSpecializationDecl&gt;::value, clang::TemplateDeductionResult&gt;::type DeduceTemplateArguments&lt;clang::ClassTemplatePartialSpecializationDecl&gt;(S=&lt;unavailable&gt;, Partial=&lt;unavailable&gt;, TemplateArgs=&lt;unavailable&gt;, Info=&lt;unavailable&gt;) at SemaTemplateDeduction.cpp:3338:5
    frame #<!-- -->79577: 0x0000564cb941e07b clang`clang::Sema::DeduceTemplateArguments(this=0x0000564cbe12e210, Partial=0x0000564ccf56cd68, TemplateArgs=&lt;unavailable&gt;, Info=0x00007fff2acc35a0) at SemaTemplateDeduction.cpp:3350:10
    frame #<!-- -->79578: 0x0000564cb9514d34 clang`clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool) [inlined] getPatternForClassTemplateSpecialization(S=0x0000564cbe12e210, PointOfInstantiation=(ID = 2195354835), ClassTemplateSpec=0x0000564cd440f5a8, TSK=TSK_ImplicitInstantiation) at SemaTemplateInstantiate.cpp:3843:46
    frame #<!-- -->79579: 0x0000564cb9514b05 clang`clang::Sema::InstantiateClassTemplateSpecialization(this=0x0000564cbe12e210, PointOfInstantiation=(ID = 2195354835), ClassTemplateSpec=0x0000564cd440f5a8, TSK=TSK_ImplicitInstantiation, Complain=true) at SemaTemplateInstantiate.cpp:3968:7
    frame #<!-- -->79580: 0x0000564cb9637aae clang`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0&gt;(long) [inlined] clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0::operator()(this=0x00007fff2acc3c68) const at SemaType.cpp:9163:23
    frame #<!-- -->79581: 0x0000564cb9637a88 clang`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::$_0&gt;(callable=140733911415912) at STLFunctionalExtras.h:45:12
    frame #<!-- -->79582: 0x0000564cb8b79e8f clang`clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) [inlined] llvm::function_ref&lt;void ()&gt;::operator()(this=&lt;unavailable&gt;) const at STLFunctionalExtras.h:68:12
    frame #<!-- -->79583: 0x0000564cb8b79e89 clang`clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) [inlined] clang::runWithSufficientStackSpace(Diag=function_ref&lt;void ()&gt; @ 0x00005631f2e50580, Fn=&lt;unavailable&gt;) at Stack.h:46:7
    frame #<!-- -->79584: 0x0000564cb8b79e80 clang`clang::Sema::runWithSufficientStackSpace(this=&lt;unavailable&gt;, Loc=(ID = 2195354835), Fn=function_ref&lt;void ()&gt; @ 0x00005631f2e50580) at Sema.cpp:568:3
    frame #<!-- -->79585: 0x0000564cb961eb45 clang`clang::Sema::RequireCompleteTypeImpl(this=0x0000564cbe12e210, Loc=(ID = 2195354835), T=QualType @ 0x00007fff2acc3c50, Kind=AcceptSizeless, Diagnoser=0x00007fff2acc3de8) at SemaType.cpp:0:9
    frame #<!-- -->79586: 0x0000564cb961e380 clang`clang::Sema::RequireCompleteType(this=&lt;unavailable&gt;, Loc=&lt;unavailable&gt;, T=&lt;unavailable&gt;, Kind=&lt;unavailable&gt;, Diagnoser=&lt;unavailable&gt;) at SemaType.cpp:8888:7
  * frame #<!-- -->79587: 0x0000564cb8d9e8e8 clang`clang::Sema::CheckBaseSpecifier(clang::CXXRecordDecl*, clang::SourceRange, bool, clang::AccessSpecifier, clang::TypeSourceInfo*, clang::SourceLocation) [inlined] clang::Sema::RequireCompleteType(this=0x0000564cbe12e210, Loc=(ID = 2195354835), T=QualType @ rbp, Diagnoser=0x00007fff2acc3de8) at Sema.h:14946:12
    frame #<!-- -->79588: 0x0000564cb8d9e8d4 clang`clang::Sema::CheckBaseSpecifier(clang::CXXRecordDecl*, clang::SourceRange, bool, clang::AccessSpecifier, clang::TypeSourceInfo*, clang::SourceLocation) [inlined] bool clang::Sema::RequireCompleteType&lt;clang::SourceRange&gt;(this=0x0000564cbe12e210, Loc=(ID = 2195354835), T=QualType @ rbp, DiagID=3616, Args=0x00007fff2acc3dc8) at Sema.h:14956:12
    frame #<!-- -->79589: 0x0000564cb8d9e8ad clang`clang::Sema::CheckBaseSpecifier(this=0x0000564cbe12e210, Class=0x0000564cd440f068, SpecifierRange=SourceRange @ 0x00007fff2acc3dc8, Virtual=false, Access=AS_none, TInfo=0x0000564cd440f790, EllipsisLoc=(ID = 0)) at SemaDeclCXX.cpp:2676:9
    frame #<!-- -->79590: 0x0000564cb8d9f2d5 clang`clang::Sema::ActOnBaseSpecifier(this=0x0000564cbe12e210, classdecl=0x0000564cd440f068, SpecifierRange=SourceRange @ 0x00007fff2acc3f20, Attributes=&lt;unavailable&gt;, Virtual=false, Access=AS_none, basetype=(Ptr = 0x0000564cd438fb40), BaseLoc=(ID = 2195354847), EllipsisLoc=(ID = 0)) at SemaDeclCXX.cpp:2796:36
    frame #<!-- -->79591: 0x0000564cb8a7b79c clang`clang::Parser::ParseBaseSpecifier(this=0x0000564cbe139b90, ClassDecl=&lt;unavailable&gt;) at ParseDeclCXX.cpp:2490:18
    frame #<!-- -->79592: 0x0000564cb8a7b06b clang`clang::Parser::ParseBaseClause(this=0x0000564cbe139b90, ClassDecl=0x0000564cd440f068) at ParseDeclCXX.cpp:2394:25
    frame #<!-- -->79593: 0x0000564cb8a7a503 clang`clang::Parser::ParseCXXMemberSpecification(this=0x0000564cbe139b90, RecordLoc=(ID = 2195354821), AttrFixitLoc=&lt;unavailable&gt;, Attrs=0x00007fff2acc4570, TagType=25, TagDecl=0x0000564cd440f068) at ParseDeclCXX.cpp:3770:5
    frame #<!-- -->79594: 0x0000564cb8a78298 clang`clang::Parser::ParseClassSpecifier(this=0x0000564cbe139b90, TagTokKind=kw_struct, StartLoc=(ID = 2195354821), DS=0x00007fff2acc5280, TemplateInfo=0x00007fff2acc51f8, AS=AS_none, EnteringContext=&lt;unavailable&gt;, DSC=&lt;unavailable&gt;, Attributes=&lt;unavailable&gt;) at ParseDeclCXX.cpp:2312:7
    frame #<!-- -->79595: 0x0000564cb8a51e01 clang`clang::Parser::ParseDeclarationSpecifiers(this=0x0000564cbe139b90, DS=0x00007fff2acc5280, TemplateInfo=0x00007fff2acc51f8, AS=AS_none, DSContext=DSC_top_level, LateAttrs=0x0000000000000000, AllowImplicitTypename=Yes) at ParseDecl.cpp:4663:7
    frame #<!-- -->79596: 0x0000564cb8a24b29 clang`clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) [inlined] clang::Parser::ParseDeclarationSpecifiers(this=0x0000564cbe139b90, DS=0x00007fff2acc5280, TemplateInfo=0x00007fff2acc51f8, AS=AS_none, DSC=DSC_top_level, LateAttrs=0x0000000000000000) at Parser.h:2509:12
    frame #<!-- -->79597: 0x0000564cb8a24b06 clang`clang::Parser::ParseDeclOrFunctionDefInternal(this=0x0000564cbe139b90, Attrs=0x00007fff2acc56b8, DeclSpecAttrs=0x00007fff2acc5748, DS=0x00007fff2acc5280, AS=AS_none) at Parser.cpp:1153:3
    frame #<!-- -->79598: 0x0000564cb8a24772 clang`clang::Parser::ParseDeclarationOrFunctionDefinition(this=0x0000564cbe139b90, Attrs=0x00007fff2acc56b8, DeclSpecAttrs=0x00007fff2acc5748, DS=&lt;unavailable&gt;, AS=AS_none) at Parser.cpp:1271:12
    frame #<!-- -->79599: 0x0000564cb8a23559 clang`clang::Parser::ParseExternalDeclaration(this=0x0000564cbe139b90, Attrs=&lt;unavailable&gt;, DeclSpecAttrs=&lt;unavailable&gt;, DS=&lt;unavailable&gt;) at Parser.cpp:1074:14
    frame #<!-- -->79600: 0x0000564cb8a6de4f clang`clang::Parser::ParseInnerNamespace(this=0x0000564cbe139b90, InnerNSs=0x00007fff2acc5a60, index=1, InlineLoc=0x00007fff2acc59f4, attrs=0x00007fff2acc5b98, Tracker=0x00007fff2acc5b50) at ParseDeclCXX.cpp:272:7
    frame #<!-- -->79601: 0x0000564cb8a6df59 clang`clang::Parser::ParseInnerNamespace(this=0x0000564cbe139b90, InnerNSs=0x00007fff2acc5a60, index=1, InlineLoc=0x00007fff2acc59f4, attrs=0x00007fff2acc5b98, Tracker=0x00007fff2acc5b50) at ParseDeclCXX.cpp:294:3
    frame #<!-- -->79602: 0x0000564cb8a6d2a4 clang`clang::Parser::ParseNamespace(this=0x0000564cbe139b90, Context=&lt;unavailable&gt;, DeclEnd=&lt;unavailable&gt;, InlineLoc=(ID = 0)) at ParseDeclCXX.cpp:249:3
    frame #<!-- -->79603: 0x0000564cb8a4b30b clang`clang::Parser::ParseDeclaration(this=0x0000564cbe139b90, Context=File, DeclEnd=0x00007fff2acc5ed8, DeclAttrs=0x00007fff2acc6080, DeclSpecAttrs=0x00007fff2acc5ff0, DeclSpecStart=0x0000000000000000) at ParseDecl.cpp:2044:12
    frame #<!-- -->79604: 0x0000564cb8a230ad clang`clang::Parser::ParseExternalDeclaration(this=0x0000564cbe139b90, Attrs=0x00007fff2acc6080, DeclSpecAttrs=0x00007fff2acc5ff0, DS=0x0000000000000000) at Parser.cpp:0
    frame #<!-- -->79605: 0x0000564cb8a21566 clang`clang::Parser::ParseTopLevelDecl(this=0x0000564cbe139b90, Result=0x00007fff2acc6170, ImportState=0x00007fff2acc6184) at Parser.cpp:763:12
    frame #<!-- -->79606: 0x0000564cb8a1c9fe clang`clang::ParseAST(S=0x0000564cbe12e210, PrintStats=false, SkipFunctionBodies=&lt;unavailable&gt;) at ParseAST.cpp:171:20
    frame #<!-- -->79607: 0x0000564cb70f477f clang`clang::FrontendAction::Execute(this=0x0000564cbe0e56d0) at FrontendAction.cpp:1078:8
    frame #<!-- -->79608: 0x0000564cb7060fed clang`clang::CompilerInstance::ExecuteAction(this=0x0000564cbe0e3360, Act=0x0000564cbe0e56d0) at CompilerInstance.cpp:1061:33
    frame #<!-- -->79609: 0x0000564cb71cd737 clang`clang::ExecuteCompilerInvocation(Clang=0x0000564cbe0e3360) at ExecuteCompilerInvocation.cpp:280:25
    frame #<!-- -->79610: 0x0000564cb4fabe14 clang`cc1_main(Argv=ArrayRef&lt;const char *&gt; @ 0x00005631f2e574f0, Argv0=&lt;unavailable&gt;, MainAddr=0x0000564cb4fa6730) at cc1_main.cpp:284:15
    frame #<!-- -->79611: 0x0000564cb4fa8bbe clang`ExecuteCC1Tool(ArgV=&lt;unavailable&gt;, ToolContext=0x00007fff2acc72f0) at driver.cpp:215:12
    frame #<!-- -->79612: 0x0000564cb6e8e959 clang`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) [inlined] llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::operator()(this=&lt;unavailable&gt;, params=&lt;unavailable&gt;) const at STLFunctionalExtras.h:68:12
    frame #<!-- -->79613: 0x0000564cb6e8e94c clang`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(long) [inlined] clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0::operator()(this=0x00007fff2acc6cb8) const at Job.cpp:440:34
    frame #<!-- -->79614: 0x0000564cb6e8e944 clang`void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::$_0&gt;(callable=140733911428280) at STLFunctionalExtras.h:45:12
    frame #<!-- -->79615: 0x0000564cb6625756 clang`llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) [inlined] llvm::function_ref&lt;void ()&gt;::operator()(this=&lt;unavailable&gt;) const at STLFunctionalExtras.h:68:12
    frame #<!-- -->79616: 0x0000564cb6625751 clang`llvm::CrashRecoveryContext::RunSafely(this=&lt;unavailable&gt;, Fn=function_ref&lt;void ()&gt; @ 0x00005631f2e50580) at CrashRecoveryContext.cpp:426:3
    frame #<!-- -->79617: 0x0000564cb6e8e2a2 clang`clang::driver::CC1Command::Execute(this=0x0000564cbe057f30, Redirects=&lt;unavailable&gt;, ErrMsg=&lt;unavailable&gt;, ExecutionFailed=&lt;unavailable&gt;) const at Job.cpp:440:12
    frame #<!-- -->79618: 0x0000564cb6e49f27 clang`clang::driver::Compilation::ExecuteCommand(this=0x0000564cbe0c01e0, C=0x0000564cbe057f30, FailingCommand=0x00007fff2acc71d8, LogOnly=false) const at Compilation.cpp:199:15
    frame #<!-- -->79619: 0x0000564cb6e4a45e clang`clang::driver::Compilation::ExecuteJobs(this=0x0000564cbe0c01e0, Jobs=&lt;unavailable&gt;, FailingCommands=0x00007fff2acc72f0, LogOnly=&lt;unavailable&gt;) const at Compilation.cpp:253:19
    frame #<!-- -->79620: 0x0000564cb6e6ab1f clang`clang::driver::Driver::ExecuteCompilation(this=0x00007fff2acc74b8, C=0x0000564cbe0c01e0, FailingCommands=0x00007fff2acc72f0) at Driver.cpp:1943:5
    frame #<!-- -->79621: 0x0000564cb4fa81d6 clang`clang_main(Argc=&lt;unavailable&gt;, Argv=&lt;unavailable&gt;, ToolContext=0x00007fff2acc86d8) at driver.cpp:391:21
    frame #<!-- -->79622: 0x0000564cb4fb7977 clang`main(argc=1915, argv=0x00007fff2acc88a8) at clang-driver.cpp:17:10
    frame #<!-- -->79623: 0x00007f0db544dd90 libc.so.6`__libc_start_call_main(main=(clang`main at clang-driver.cpp:15), argc=1915, argv=0x00007fff2acc88a8) at libc_start_call_main.h:58:16
    frame #<!-- -->79624: 0x00007f0db544de40 libc.so.6`__libc_start_main_impl(main=(clang`main at clang-driver.cpp:15), argc=1915, argv=0x00007fff2acc88a8, init=&lt;unavailable&gt;, fini=&lt;unavailable&gt;, rtld_fini=&lt;unavailable&gt;, stack_end=0x00007fff2acc8898) at libc-start.c:392:3
``` 


and ends with the last 50 frames being 

```
* thread #<!-- -->1, name = 'clang', stop reason = signal SIGSEGV: invalid address
    frame #<!-- -->0: 0x0000564cb9ecd7c1 clang`(anonymous namespace)::StmtProfilerWithPointers::HandleStmtClass(this=0x00007fff2a4dc088, SC=CallExprClass) at StmtProfile.cpp:93
    frame #<!-- -->1: 0x0000564cb9ed2995 clang`(anonymous namespace)::StmtProfiler::VisitStmt(clang::Stmt const*) [inlined] (anonymous namespace)::StmtProfiler::VisitStmtNoChildren(this=0x00007fff2a4dc088, S=&lt;unavailable&gt;) at StmtProfile.cpp:44:7
    frame #<!-- -->2: 0x0000564cb9ed298a clang`(anonymous namespace)::StmtProfiler::VisitStmt(this=&lt;unavailable&gt;, S=0x0000564cd42c2930) at StmtProfile.cpp:241:3
    frame #<!-- -->3: 0x0000564cb9ecd1f3 clang`clang::Stmt::Profile(this=&lt;unavailable&gt;, ID=&lt;unavailable&gt;, Context=&lt;unavailable&gt;, Canonical=&lt;unavailable&gt;, ProfileLambdaExpr=&lt;unavailable&gt;) const at StmtProfile.cpp:2637:12
    frame #<!-- -->4: 0x0000564cb9f2cbee clang`clang::AutoType::Profile(ID=0x00007fff2a4dc148, Context=0x0000564cbe1f8de0, Deduced=&lt;unavailable&gt;, Keyword=&lt;unavailable&gt;, IsDependent=&lt;unavailable&gt;, CD=&lt;unavailable&gt;, Arguments=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007fff2a4dc0e0) at Type.cpp:5091:9
    frame #<!-- -->5: 0x0000564cb9f2ccaa clang`clang::AutoType::Profile(this=&lt;unavailable&gt;, ID=&lt;unavailable&gt;, Context=&lt;unavailable&gt;) at Type.cpp:5095:3
    frame #<!-- -->6: 0x0000564cb9989b94 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&amp;, unsigned int, llvm::FoldingSetNodeID&amp;) [inlined] llvm::DefaultContextualFoldingSetTrait&lt;clang::AutoType, clang::ASTContext&amp;&gt;::Profile(X=&lt;unavailable&gt;, ID=0x00007fff2a4dc148, Context=&lt;unavailable&gt;) at FoldingSet.h:270:7
    frame #<!-- -->7: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&amp;, unsigned int, llvm::FoldingSetNodeID&amp;) [inlined] llvm::DefaultContextualFoldingSetTrait&lt;clang::AutoType, clang::ASTContext&amp;&gt;::Equals(X=&lt;unavailable&gt;, ID=0x00007fff2a4dc2a8, (null)=&lt;unavailable&gt;, TempID=0x00007fff2a4dc148, Context=&lt;unavailable&gt;) at FoldingSet.h:436:3
    frame #<!-- -->8: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(Base=&lt;unavailable&gt;, N=&lt;unavailable&gt;, ID=0x00007fff2a4dc2a8, IDHash=&lt;unavailable&gt;, TempID=&lt;unavailable&gt;) at FoldingSet.h:616:12
    frame #<!-- -->9: 0x0000564cb662ca2c clang`llvm::FoldingSetBase::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dc2a8, InsertPos=0x00007fff2a4dc248, Info=0x0000564cbd0a6e68) at FoldingSet.cpp:288:9
    frame #<!-- -->10: 0x0000564cb996001b clang`clang::ASTContext::getAutoTypeInternal(clang::QualType, clang::AutoTypeKeyword, bool, bool, clang::ConceptDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool) const [inlined] llvm::FoldingSetImpl&lt;llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;, clang::AutoType&gt;::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dc2a8, InsertPos=0x00007fff2a4dc248) at FoldingSet.h:506:45
    frame #<!-- -->11: 0x0000564cb9960004 clang`clang::ASTContext::getAutoTypeInternal(this=0x0000564cbe1f8de0, DeducedType=QualType @ rbx, Keyword=Auto, IsDependent=true, IsPack=false, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007fff2a4dc500, IsCanon=false) const at ASTContext.cpp:6198:32
    frame #<!-- -->12: 0x0000564cb992b116 clang`clang::ASTContext::getUnconstrainedType(clang::QualType) const [inlined] clang::ASTContext::getAutoType(this=0x0000564cbe1f8de0, DeducedType=QualType @ scalar, Keyword=&lt;unavailable&gt;, IsDependent=&lt;unavailable&gt;, IsPack=&lt;unavailable&gt;, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=&lt;unavailable&gt;) const at ASTContext.cpp:6247:10
    frame #<!-- -->13: 0x0000564cb992b0f2 clang`clang::ASTContext::getUnconstrainedType(this=0x0000564cbe1f8de0, T=QualType @ rbx) const at ASTContext.cpp:6258:29
    frame #<!-- -->14: 0x0000564cb9ecda98 clang`(anonymous namespace)::StmtProfilerWithPointers::VisitDecl(this=&lt;unavailable&gt;, D=&lt;unavailable&gt;) at StmtProfile.cpp:114:29
    frame #<!-- -->15: 0x0000564cb9ed062e clang`(anonymous namespace)::StmtProfiler::VisitDeclRefExpr(this=0x00007fff2a4dc638, S=0x0000564cd42c2910) at StmtProfile.cpp:1328:3
    frame #<!-- -->16: 0x0000564cb9ed2a09 clang`(anonymous namespace)::StmtProfiler::VisitStmt(this=0x00007fff2a4dc638, S=&lt;unavailable&gt;) at StmtProfile.cpp:245:7
    frame #<!-- -->17: 0x0000564cb9ecd1f3 clang`clang::Stmt::Profile(this=&lt;unavailable&gt;, ID=&lt;unavailable&gt;, Context=&lt;unavailable&gt;, Canonical=&lt;unavailable&gt;, ProfileLambdaExpr=&lt;unavailable&gt;) const at StmtProfile.cpp:2637:12
    frame #<!-- -->18: 0x0000564cb9f2cbee clang`clang::AutoType::Profile(ID=0x00007fff2a4dc6f8, Context=0x0000564cbe1f8de0, Deduced=&lt;unavailable&gt;, Keyword=&lt;unavailable&gt;, IsDependent=&lt;unavailable&gt;, CD=&lt;unavailable&gt;, Arguments=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007fff2a4dc690) at Type.cpp:5091:9
    frame #<!-- -->19: 0x0000564cb9f2ccaa clang`clang::AutoType::Profile(this=&lt;unavailable&gt;, ID=&lt;unavailable&gt;, Context=&lt;unavailable&gt;) at Type.cpp:5095:3
    frame #<!-- -->20: 0x0000564cb9989b94 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&amp;, unsigned int, llvm::FoldingSetNodeID&amp;) [inlined] llvm::DefaultContextualFoldingSetTrait&lt;clang::AutoType, clang::ASTContext&amp;&gt;::Profile(X=&lt;unavailable&gt;, ID=0x00007fff2a4dc6f8, Context=&lt;unavailable&gt;) at FoldingSet.h:270:7
    frame #<!-- -->21: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&amp;, unsigned int, llvm::FoldingSetNodeID&amp;) [inlined] llvm::DefaultContextualFoldingSetTrait&lt;clang::AutoType, clang::ASTContext&amp;&gt;::Equals(X=&lt;unavailable&gt;, ID=0x00007fff2a4dc858, (null)=&lt;unavailable&gt;, TempID=0x00007fff2a4dc6f8, Context=&lt;unavailable&gt;) at FoldingSet.h:436:3
    frame #<!-- -->22: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(Base=&lt;unavailable&gt;, N=&lt;unavailable&gt;, ID=0x00007fff2a4dc858, IDHash=&lt;unavailable&gt;, TempID=&lt;unavailable&gt;) at FoldingSet.h:616:12
    frame #<!-- -->23: 0x0000564cb662ca2c clang`llvm::FoldingSetBase::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dc858, InsertPos=0x00007fff2a4dc7f8, Info=0x0000564cbd0a6e68) at FoldingSet.cpp:288:9
    frame #<!-- -->24: 0x0000564cb996001b clang`clang::ASTContext::getAutoTypeInternal(clang::QualType, clang::AutoTypeKeyword, bool, bool, clang::ConceptDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool) const [inlined] llvm::FoldingSetImpl&lt;llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;, clang::AutoType&gt;::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dc858, InsertPos=0x00007fff2a4dc7f8) at FoldingSet.h:506:45
    frame #<!-- -->25: 0x0000564cb9960004 clang`clang::ASTContext::getAutoTypeInternal(this=0x0000564cbe1f8de0, DeducedType=QualType @ rbx, Keyword=Auto, IsDependent=true, IsPack=false, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007fff2a4dcab0, IsCanon=false) const at ASTContext.cpp:6198:32
    frame #<!-- -->26: 0x0000564cb992b116 clang`clang::ASTContext::getUnconstrainedType(clang::QualType) const [inlined] clang::ASTContext::getAutoType(this=0x0000564cbe1f8de0, DeducedType=QualType @ scalar, Keyword=&lt;unavailable&gt;, IsDependent=&lt;unavailable&gt;, IsPack=&lt;unavailable&gt;, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=&lt;unavailable&gt;) const at ASTContext.cpp:6247:10
    frame #<!-- -->27: 0x0000564cb992b0f2 clang`clang::ASTContext::getUnconstrainedType(this=0x0000564cbe1f8de0, T=QualType @ rbx) const at ASTContext.cpp:6258:29
    frame #<!-- -->28: 0x0000564cb9ecda98 clang`(anonymous namespace)::StmtProfilerWithPointers::VisitDecl(this=&lt;unavailable&gt;, D=&lt;unavailable&gt;) at StmtProfile.cpp:114:29
    frame #<!-- -->29: 0x0000564cb9ed062e clang`(anonymous namespace)::StmtProfiler::VisitDeclRefExpr(this=0x00007fff2a4dcbe8, S=0x0000564cd42c2910) at StmtProfile.cpp:1328:3
    frame #<!-- -->30: 0x0000564cb9ed2a09 clang`(anonymous namespace)::StmtProfiler::VisitStmt(this=0x00007fff2a4dcbe8, S=&lt;unavailable&gt;) at StmtProfile.cpp:245:7
    frame #<!-- -->31: 0x0000564cb9ecd1f3 clang`clang::Stmt::Profile(this=&lt;unavailable&gt;, ID=&lt;unavailable&gt;, Context=&lt;unavailable&gt;, Canonical=&lt;unavailable&gt;, ProfileLambdaExpr=&lt;unavailable&gt;) const at StmtProfile.cpp:2637:12
    frame #<!-- -->32: 0x0000564cb9f2cbee clang`clang::AutoType::Profile(ID=0x00007fff2a4dcca8, Context=0x0000564cbe1f8de0, Deduced=&lt;unavailable&gt;, Keyword=&lt;unavailable&gt;, IsDependent=&lt;unavailable&gt;, CD=&lt;unavailable&gt;, Arguments=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007fff2a4dcc40) at Type.cpp:5091:9
    frame #<!-- -->33: 0x0000564cb9f2ccaa clang`clang::AutoType::Profile(this=&lt;unavailable&gt;, ID=&lt;unavailable&gt;, Context=&lt;unavailable&gt;) at Type.cpp:5095:3
    frame #<!-- -->34: 0x0000564cb9989b94 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&amp;, unsigned int, llvm::FoldingSetNodeID&amp;) [inlined] llvm::DefaultContextualFoldingSetTrait&lt;clang::AutoType, clang::ASTContext&amp;&gt;::Profile(X=&lt;unavailable&gt;, ID=0x00007fff2a4dcca8, Context=&lt;unavailable&gt;) at FoldingSet.h:270:7
    frame #<!-- -->35: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&amp;, unsigned int, llvm::FoldingSetNodeID&amp;) [inlined] llvm::DefaultContextualFoldingSetTrait&lt;clang::AutoType, clang::ASTContext&amp;&gt;::Equals(X=&lt;unavailable&gt;, ID=0x00007fff2a4dce08, (null)=&lt;unavailable&gt;, TempID=0x00007fff2a4dcca8, Context=&lt;unavailable&gt;) at FoldingSet.h:436:3
    frame #<!-- -->36: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(Base=&lt;unavailable&gt;, N=&lt;unavailable&gt;, ID=0x00007fff2a4dce08, IDHash=&lt;unavailable&gt;, TempID=&lt;unavailable&gt;) at FoldingSet.h:616:12
    frame #<!-- -->37: 0x0000564cb662ca2c clang`llvm::FoldingSetBase::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dce08, InsertPos=0x00007fff2a4dcda8, Info=0x0000564cbd0a6e68) at FoldingSet.cpp:288:9
    frame #<!-- -->38: 0x0000564cb996001b clang`clang::ASTContext::getAutoTypeInternal(clang::QualType, clang::AutoTypeKeyword, bool, bool, clang::ConceptDecl*, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, bool) const [inlined] llvm::FoldingSetImpl&lt;llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;, clang::AutoType&gt;::FindNodeOrInsertPos(this=0x0000564cbe1f90b8, ID=0x00007fff2a4dce08, InsertPos=0x00007fff2a4dcda8) at FoldingSet.h:506:45
    frame #<!-- -->39: 0x0000564cb9960004 clang`clang::ASTContext::getAutoTypeInternal(this=0x0000564cbe1f8de0, DeducedType=QualType @ rbx, Keyword=Auto, IsDependent=true, IsPack=false, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007fff2a4dd060, IsCanon=false) const at ASTContext.cpp:6198:32
    frame #<!-- -->40: 0x0000564cb992b116 clang`clang::ASTContext::getUnconstrainedType(clang::QualType) const [inlined] clang::ASTContext::getAutoType(this=0x0000564cbe1f8de0, DeducedType=QualType @ scalar, Keyword=&lt;unavailable&gt;, IsDependent=&lt;unavailable&gt;, IsPack=&lt;unavailable&gt;, TypeConstraintConcept=0x0000000000000000, TypeConstraintArgs=&lt;unavailable&gt;) const at ASTContext.cpp:6247:10
    frame #<!-- -->41: 0x0000564cb992b0f2 clang`clang::ASTContext::getUnconstrainedType(this=0x0000564cbe1f8de0, T=QualType @ rbx) const at ASTContext.cpp:6258:29
    frame #<!-- -->42: 0x0000564cb9ecda98 clang`(anonymous namespace)::StmtProfilerWithPointers::VisitDecl(this=&lt;unavailable&gt;, D=&lt;unavailable&gt;) at StmtProfile.cpp:114:29
    frame #<!-- -->43: 0x0000564cb9ed062e clang`(anonymous namespace)::StmtProfiler::VisitDeclRefExpr(this=0x00007fff2a4dd198, S=0x0000564cd42c2910) at StmtProfile.cpp:1328:3
    frame #<!-- -->44: 0x0000564cb9ed2a09 clang`(anonymous namespace)::StmtProfiler::VisitStmt(this=0x00007fff2a4dd198, S=&lt;unavailable&gt;) at StmtProfile.cpp:245:7
    frame #<!-- -->45: 0x0000564cb9ecd1f3 clang`clang::Stmt::Profile(this=&lt;unavailable&gt;, ID=&lt;unavailable&gt;, Context=&lt;unavailable&gt;, Canonical=&lt;unavailable&gt;, ProfileLambdaExpr=&lt;unavailable&gt;) const at StmtProfile.cpp:2637:12
    frame #<!-- -->46: 0x0000564cb9f2cbee clang`clang::AutoType::Profile(ID=0x00007fff2a4dd258, Context=0x0000564cbe1f8de0, Deduced=&lt;unavailable&gt;, Keyword=&lt;unavailable&gt;, IsDependent=&lt;unavailable&gt;, CD=&lt;unavailable&gt;, Arguments=ArrayRef&lt;clang::TemplateArgument&gt; @ 0x00007fff2a4dd1f0) at Type.cpp:5091:9
    frame #<!-- -->47: 0x0000564cb9f2ccaa clang`clang::AutoType::Profile(this=&lt;unavailable&gt;, ID=&lt;unavailable&gt;, Context=&lt;unavailable&gt;) at Type.cpp:5095:3
    frame #<!-- -->48: 0x0000564cb9989b94 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&amp;, unsigned int, llvm::FoldingSetNodeID&amp;) [inlined] llvm::DefaultContextualFoldingSetTrait&lt;clang::AutoType, clang::ASTContext&amp;&gt;::Profile(X=&lt;unavailable&gt;, ID=0x00007fff2a4dd258, Context=&lt;unavailable&gt;) at FoldingSet.h:270:7
    frame #<!-- -->49: 0x0000564cb9989b89 clang`llvm::ContextualFoldingSet&lt;clang::AutoType, clang::ASTContext&amp;&gt;::NodeEquals(llvm::FoldingSetBase const*, llvm::FoldingSetBase::Node*, llvm::FoldingSetNodeID const&amp;, unsigned int, llvm::FoldingSetNodeID&amp;) [inlined] llvm::DefaultContextualFoldingSetTrait&lt;clang::AutoType, clang::ASTContext&amp;&gt;::Equals(X=&lt;unavailable&gt;, ID=0x00007fff2a4dd3b8, (null)=&lt;unavailable&gt;, TempID=0x00007fff2a4dd258, Context=&lt;unavailable&gt;) at FoldingSet.h:436:3

```

If i print the variables in frame 18 i get the following
```
(clang::QualType) Deduced = &lt;Could not evaluate DW_OP_entry_value.&gt;

(clang::AutoTypeKeyword) Keyword = &lt;Could not evaluate DW_OP_entry_value.&gt;

(bool) IsDependent = &lt;variable not available&gt;

(clang::ConceptDecl *) CD = &lt;Could not evaluate DW_OP_entry_value.&gt;

(llvm::ArrayRef&lt;clang::TemplateArgument&gt;) Arguments = {
  Data = 0x0000564cd42f5a00
  Length = 1
}
(const clang::TemplateArgument &amp;) Arg = 0x0000564cd42f5a00: {
   = {
    DeclArg = {
      Kind = Expression
      IsDefaulted = false
      QT = 0x0000564cd42c2930
      D = 0x0000564cd42c28c0
    }
    Integer = {
      Kind = Expression
      IsDefaulted = false
      BitWidth = 0
      IsUnsigned = false
       = {
        VAL = 94887977167152
        pVal = 0x0000564cd42c2930
      }
      Type = 0x0000564cd42c28c0
    }
    Value = {
      Kind = Expression
      IsDefaulted = false
      Value = 0x0000564cd42c2930
      Type = 0x0000564cd42c28c0
    }
    Args = {
      Kind = Expression
      IsDefaulted = false
      NumArgs = 0
      Args = 0x0000564cd42c2930
    }
    TemplateArg = (Kind = Expression, IsDefaulted = false, NumExpansions = 0, Name = 0x0000564cd42c2930)
    TypeOrValue = (Kind = Expression, IsDefaulted = false, V = 94887977167152)
  }
}
``` 

It looks like the template argument values are way off. Given the nature of the crash it is hard to come up with a
minimal reproducer, as most attempts at compiling the file succeeds. 

I have tried on multiple x86_64 machines, and with both the clang from the apt repository and a locally compiled version of (19.1.4)
and they both crash at some point compiling the same file. 



</details>


---

### Comment 2 - EugeneZelenko

Could you please provide reproducer?

---

### Comment 3 - molar

Duplicate of https://github.com/llvm/llvm-project/issues/110231 

---

