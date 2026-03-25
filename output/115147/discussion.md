# [clang] Clang crash when compiling recurring templates

**Author:** killcerr
**URL:** https://github.com/llvm/llvm-project/issues/115147
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-01-27T09:29:43Z

## Body

Compiler Explorer: [code](https://godbolt.org/z/GE79rcGa7)
```
x86-64 clang 19.1.0 (assertions) - cached

clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4417: void clang::LocalInstantiationScope::InstantiatedLocal(const clang::Decl*, clang::Decl*): Assertion `!Current->LocalDecls.contains(D) && "Instantiated local in inner and outer scopes"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -O3 -std=c++23 <source>
1.	<source>:77:27: current parser token ')'
2.	<source>:76:1: parsing function body 'main'
3.	<source>:76:1: in compound statement ('{}')
  #0 0x0000000003ae9978 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae9978)
  #1 0x0000000003ae766c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae766c)
  #2 0x0000000003a308f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #3 0x00007fd713a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #4 0x00007fd713a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
  #5 0x00007fd713a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
  #6 0x00007fd713a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
  #7 0x00007fd713a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
  #8 0x00007fd713a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #9 0x000000000714f3f7 clang::LocalInstantiationScope::InstantiatedLocal(clang::Decl const*, clang::Decl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x714f3f7)
 #10 0x0000000007150545 (anonymous namespace)::TemplateInstantiator::transformedLocalDecl(clang::Decl*, llvm::ArrayRef<clang::Decl*>) (.isra.0) SemaTemplateInstantiate.cpp:0:0
 #11 0x00000000071a3de7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #12 0x000000000715bdcf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #13 0x0000000007167afe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #14 0x000000000715bb9d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #15 0x000000000715d638 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #16 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #17 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
 #18 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
 #19 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
 #20 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
 #21 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
 #22 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
 #23 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
 #24 0x0000000007184dae clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #25 0x000000000715bffd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #26 0x000000000715bc01 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #27 0x000000000717b5a3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #28 0x000000000717bbef (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #29 0x000000000717d888 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #30 0x0000000007184e5e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #31 0x000000000715bffd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #32 0x000000000715d638 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #33 0x000000000715d9ac clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
 #34 0x000000000719ae44 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #35 0x00000000071a2017 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #36 0x00000000071a2017 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #37 0x000000000719ae44 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #38 0x00000000071a5aab clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #39 0x000000000715bdcf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #40 0x0000000007167afe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #41 0x000000000715bb9d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #42 0x000000000715d638 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #43 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #44 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
 #45 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
 #46 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
 #47 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
 #48 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
 #49 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
 #50 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
 #51 0x0000000007184dae clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #52 0x000000000715bffd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #53 0x000000000715d638 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #54 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #55 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
 #56 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
 #57 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
 #58 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
 #59 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
 #60 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
 #61 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
 #62 0x0000000007184dae clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #63 0x000000000715bffd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #64 0x000000000715d638 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #65 0x000000000715d9ac clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
 #66 0x00000000071a1fab clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #67 0x000000000719ae44 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #68 0x00000000071a5aab clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #69 0x000000000715bdcf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #70 0x0000000007167afe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #71 0x000000000715bb9d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #72 0x000000000715d638 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #73 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #74 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
 #75 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
 #76 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
 #77 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
 #78 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
 #79 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
 #80 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
 #81 0x0000000007184dae clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #82 0x000000000715bffd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #83 0x000000000715bc01 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #84 0x000000000717b5a3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #85 0x000000000717bbef (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
 #86 0x000000000717d888 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #87 0x0000000007184e5e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #88 0x000000000715bffd clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #89 0x000000000715d638 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #90 0x000000000715d9ac clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
 #91 0x00000000071a2017 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #92 0x000000000719ae44 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #93 0x00000000071a5aab clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #94 0x000000000715bdcf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #95 0x0000000007167afe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #96 0x000000000715bb9d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #97 0x000000000715d638 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #98 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #99 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
#100 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
#101 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
#102 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
#103 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
#104 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
#105 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
#106 0x0000000006a9b558 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6a9b558)
#107 0x0000000006563ee6 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6563ee6)
#108 0x0000000006564184 clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6564184)
#109 0x00000000065478ca clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65478ca)
#110 0x000000000654708e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x654708e)
#111 0x0000000006548f7a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6548f7a)
#112 0x0000000006549119 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6549119)
#113 0x000000000654947e clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x654947e)
#114 0x000000000654de62 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x654de62)
#115 0x0000000006546e21 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6546e21)
#116 0x0000000006548f7a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6548f7a)
#117 0x0000000006549119 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6549119)
#118 0x000000000654d9a9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x654d9a9)
#119 0x00000000065ce309 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65ce309)
#120 0x00000000065c4d02 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c4d02)
#121 0x00000000065c5cc0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c5cc0)
#122 0x00000000065c6ba9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c6ba9)
#123 0x00000000065c837a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c837a)
#124 0x00000000064dd6c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64dd6c3)
#125 0x0000000006512708 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6512708)
#126 0x00000000064d115e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d115e)
#127 0x00000000064d190e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d190e)
#128 0x00000000064d8077 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d8077)
#129 0x00000000064d8f6f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d8f6f)
#130 0x00000000064cc33a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64cc33a)
#131 0x00000000043f9448 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x43f9448)
#132 0x0000000004684f59 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4684f59)
#133 0x0000000004609f8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4609f8e)
#134 0x000000000476e2be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x476e2be)
#135 0x0000000000c82b7f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc82b7f)
#136 0x0000000000c7bcda ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#137 0x000000000443cba9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#138 0x0000000003a30da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3a30da4)
#139 0x000000000443d19f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#140 0x0000000004402c05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4402c05)
#141 0x000000000440366d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440366d)
#142 0x000000000440b075 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440b075)
#143 0x0000000000c7fe95 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7fe95)
#144 0x0000000000b547a4 main (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xb547a4)
#145 0x00007fd713a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#146 0x00007fd713a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#147 0x0000000000c7b78e _start (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7b78e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
```cpp
template<auto N>
struct Str
{
        char data[N]{0};
        constexpr Str(const char (&s)[N])
        {
                for(decltype(N) i=0;auto c:s)
                        data[i++]=c;
        }
        constexpr Str(char c)
        {
                static_assert(N==2);
                data[0]=c;
        }
        constexpr Str()=default;
        constexpr operator const decltype(data)&()const
        {
                return data;
        }
};
template<typename T>
constexpr auto length_of_str=[]{
        static_assert(false);
}();
template<auto N>
constexpr auto length_of_str<const char(&)[N]> =N;
template<auto N>
constexpr auto length_of_str<Str<N>> =N;
template<Str a,Str ...b>
constexpr auto concat=[]{
        if constexpr(sizeof...(b)==0)
                return a;
        else if constexpr(sizeof...(b)==1)
        {
                Str<length_of_str<decltype(a)>+length_of_str<decltype((b,...))>-1> r;
                decltype(length_of_str<decltype(a)>+length_of_str<decltype((b,...))>) i=0;
                for(auto c:a.data)
                        r.data[i++]=c;
                i--;
                for(auto c:(b,...).data)
                        r.data[i++]=c;
                return r;
        }
        else
        {
                return concat<a,concat<b...>>;
        }
}();
template<auto N>
constexpr auto n_to_str=[]{
        if constexpr(N<0)
                return concat<"-",n_to_str<-N>>;
        else if constexpr(N<10)
                return Str<2>{N+'0'};
        else
                return concat<n_to_str<N/10>,n_to_str<N%10>>;
}();
template<auto a,auto b>
constexpr auto e_to_str=concat<n_to_str<a>,"*",n_to_str<b>,"=",n_to_str<a*b>>;
template<auto l,auto i=1>
constexpr auto gsl=[]{
        if constexpr(i==l)
                return e_to_str<l,i>;
        else
                return concat<e_to_str<l,i>,"\t",gsl<l,i+1>>;
}();
template<auto l,auto i=1>
constexpr auto gml=[]{
        if constexpr(i==l)
                return gsl<i>;
        else
                return concat<gsl<i>,"\n",gml<l,i+1>>;
}();
#include <cstdio>
int main()
{
        std::puts(gml<900>);
        return 0;
}
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (killcerr)

<details>
Compiler Explorer: [code](https://godbolt.org/z/GE79rcGa7)
```
x86-64 clang 19.1.0 (assertions) - cached

clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4417: void clang::LocalInstantiationScope::InstantiatedLocal(const clang::Decl*, clang::Decl*): Assertion `!Current-&gt;LocalDecls.contains(D) &amp;&amp; "Instantiated local in inner and outer scopes"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-19.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -O3 -std=c++23 &lt;source&gt;
1.	&lt;source&gt;:77:27: current parser token ')'
2.	&lt;source&gt;:76:1: parsing function body 'main'
3.	&lt;source&gt;:76:1: in compound statement ('{}')
  #<!-- -->0 0x0000000003ae9978 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae9978)
  #<!-- -->1 0x0000000003ae766c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3ae766c)
  #<!-- -->2 0x0000000003a308f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
  #<!-- -->3 0x00007fd713a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
  #<!-- -->4 0x00007fd713a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
  #<!-- -->5 0x00007fd713a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
  #<!-- -->6 0x00007fd713a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
  #<!-- -->7 0x00007fd713a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
  #<!-- -->8 0x00007fd713a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
  #<!-- -->9 0x000000000714f3f7 clang::LocalInstantiationScope::InstantiatedLocal(clang::Decl const*, clang::Decl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x714f3f7)
 #<!-- -->10 0x0000000007150545 (anonymous namespace)::TemplateInstantiator::transformedLocalDecl(clang::Decl*, llvm::ArrayRef&lt;clang::Decl*&gt;) (.isra.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->11 0x00000000071a3de7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->12 0x000000000715bdcf clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->13 0x0000000007167afe clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->14 0x000000000715bb9d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->15 0x000000000715d638 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->16 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #<!-- -->17 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
 #<!-- -->18 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
 #<!-- -->19 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
 #<!-- -->20 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
 #<!-- -->21 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
 #<!-- -->22 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
 #<!-- -->23 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
 #<!-- -->24 0x0000000007184dae clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->25 0x000000000715bffd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->26 0x000000000715bc01 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->27 0x000000000717b5a3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->28 0x000000000717bbef (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->29 0x000000000717d888 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->30 0x0000000007184e5e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->31 0x000000000715bffd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->32 0x000000000715d638 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->33 0x000000000715d9ac clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->34 0x000000000719ae44 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->35 0x00000000071a2017 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->36 0x00000000071a2017 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->37 0x000000000719ae44 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->38 0x00000000071a5aab clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->39 0x000000000715bdcf clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->40 0x0000000007167afe clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->41 0x000000000715bb9d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->42 0x000000000715d638 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->43 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #<!-- -->44 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
 #<!-- -->45 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
 #<!-- -->46 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
 #<!-- -->47 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
 #<!-- -->48 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
 #<!-- -->49 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
 #<!-- -->50 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
 #<!-- -->51 0x0000000007184dae clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->52 0x000000000715bffd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->53 0x000000000715d638 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->54 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #<!-- -->55 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
 #<!-- -->56 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
 #<!-- -->57 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
 #<!-- -->58 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
 #<!-- -->59 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
 #<!-- -->60 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
 #<!-- -->61 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
 #<!-- -->62 0x0000000007184dae clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->63 0x000000000715bffd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->64 0x000000000715d638 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->65 0x000000000715d9ac clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->66 0x00000000071a1fab clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->67 0x000000000719ae44 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->68 0x00000000071a5aab clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->69 0x000000000715bdcf clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->70 0x0000000007167afe clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->71 0x000000000715bb9d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->72 0x000000000715d638 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->73 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #<!-- -->74 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
 #<!-- -->75 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
 #<!-- -->76 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
 #<!-- -->77 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
 #<!-- -->78 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
 #<!-- -->79 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
 #<!-- -->80 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
 #<!-- -->81 0x0000000007184dae clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->82 0x000000000715bffd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->83 0x000000000715bc01 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->84 0x000000000717b5a3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->85 0x000000000717bbef (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&amp;, clang::TemplateArgumentLoc&amp;, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->86 0x000000000717d888 bool clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformTemplateArguments&lt;clang::TemplateArgumentLoc const*&gt;(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&amp;, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->87 0x0000000007184e5e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->88 0x000000000715bffd clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->89 0x000000000715d638 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->90 0x000000000715d9ac clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->91 0x00000000071a2017 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->92 0x000000000719ae44 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->93 0x00000000071a5aab clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->94 0x000000000715bdcf clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->95 0x0000000007167afe clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->96 0x000000000715bb9d clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->97 0x000000000715d638 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->98 0x000000000715dc11 clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x715dc11)
 #<!-- -->99 0x00000000071cfbdb clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x71cfbdb)
#<!-- -->100 0x0000000007218560 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218560)
#<!-- -->101 0x0000000007218dfe clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&amp;, llvm::ArrayRef&lt;clang::TemplateArgument&gt;, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7218dfe)
#<!-- -->102 0x0000000007222137 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&amp;, llvm::SmallVectorImpl&lt;clang::TemplateArgument&gt;&amp;, clang::SourceLocation, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x7222137)
#<!-- -->103 0x0000000006f9f64c clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9f64c)
#<!-- -->104 0x0000000006f9fb41 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&amp;, clang::DeclarationNameInfo const&amp;, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6f9fb41)
#<!-- -->105 0x0000000006fa8d22 clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&amp;, clang::SourceLocation, clang::LookupResult&amp;, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6fa8d22)
#<!-- -->106 0x0000000006a9b558 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::UnqualifiedId&amp;, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6a9b558)
#<!-- -->107 0x0000000006563ee6 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&amp;, bool, clang::Token&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6563ee6)
#<!-- -->108 0x0000000006564184 clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6564184)
#<!-- -->109 0x00000000065478ca clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65478ca)
#<!-- -->110 0x000000000654708e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x654708e)
#<!-- -->111 0x0000000006548f7a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6548f7a)
#<!-- -->112 0x0000000006549119 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6549119)
#<!-- -->113 0x000000000654947e clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x654947e)
#<!-- -->114 0x000000000654de62 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x654de62)
#<!-- -->115 0x0000000006546e21 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6546e21)
#<!-- -->116 0x0000000006548f7a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6548f7a)
#<!-- -->117 0x0000000006549119 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6549119)
#<!-- -->118 0x000000000654d9a9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x654d9a9)
#<!-- -->119 0x00000000065ce309 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65ce309)
#<!-- -->120 0x00000000065c4d02 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c4d02)
#<!-- -->121 0x00000000065c5cc0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c5cc0)
#<!-- -->122 0x00000000065c6ba9 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c6ba9)
#<!-- -->123 0x00000000065c837a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x65c837a)
#<!-- -->124 0x00000000064dd6c3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64dd6c3)
#<!-- -->125 0x0000000006512708 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x6512708)
#<!-- -->126 0x00000000064d115e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d115e)
#<!-- -->127 0x00000000064d190e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d190e)
#<!-- -->128 0x00000000064d8077 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d8077)
#<!-- -->129 0x00000000064d8f6f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64d8f6f)
#<!-- -->130 0x00000000064cc33a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x64cc33a)
#<!-- -->131 0x00000000043f9448 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x43f9448)
#<!-- -->132 0x0000000004684f59 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4684f59)
#<!-- -->133 0x0000000004609f8e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4609f8e)
#<!-- -->134 0x000000000476e2be clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x476e2be)
#<!-- -->135 0x0000000000c82b7f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc82b7f)
#<!-- -->136 0x0000000000c7bcda ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->137 0x000000000443cba9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->138 0x0000000003a30da4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x3a30da4)
#<!-- -->139 0x000000000443d19f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->140 0x0000000004402c05 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x4402c05)
#<!-- -->141 0x000000000440366d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440366d)
#<!-- -->142 0x000000000440b075 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0x440b075)
#<!-- -->143 0x0000000000c7fe95 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7fe95)
#<!-- -->144 0x0000000000b547a4 main (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xb547a4)
#<!-- -->145 0x00007fd713a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->146 0x00007fd713a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->147 0x0000000000c7b78e _start (/opt/compiler-explorer/clang-assertions-19.1.0/bin/clang+++0xc7b78e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
```cpp
template&lt;auto N&gt;
struct Str
{
        char data[N]{0};
        constexpr Str(const char (&amp;s)[N])
        {
                for(decltype(N) i=0;auto c:s)
                        data[i++]=c;
        }
        constexpr Str(char c)
        {
                static_assert(N==2);
                data[0]=c;
        }
        constexpr Str()=default;
        constexpr operator const decltype(data)&amp;()const
        {
                return data;
        }
};
template&lt;typename T&gt;
constexpr auto length_of_str=[]{
        static_assert(false);
}();
template&lt;auto N&gt;
constexpr auto length_of_str&lt;const char(&amp;)[N]&gt; =N;
template&lt;auto N&gt;
constexpr auto length_of_str&lt;Str&lt;N&gt;&gt; =N;
template&lt;Str a,Str ...b&gt;
constexpr auto concat=[]{
        if constexpr(sizeof...(b)==0)
                return a;
        else if constexpr(sizeof...(b)==1)
        {
                Str&lt;length_of_str&lt;decltype(a)&gt;+length_of_str&lt;decltype((b,...))&gt;-1&gt; r;
                decltype(length_of_str&lt;decltype(a)&gt;+length_of_str&lt;decltype((b,...))&gt;) i=0;
                for(auto c:a.data)
                        r.data[i++]=c;
                i--;
                for(auto c:(b,...).data)
                        r.data[i++]=c;
                return r;
        }
        else
        {
                return concat&lt;a,concat&lt;b...&gt;&gt;;
        }
}();
template&lt;auto N&gt;
constexpr auto n_to_str=[]{
        if constexpr(N&lt;0)
                return concat&lt;"-",n_to_str&lt;-N&gt;&gt;;
        else if constexpr(N&lt;10)
                return Str&lt;2&gt;{N+'0'};
        else
                return concat&lt;n_to_str&lt;N/10&gt;,n_to_str&lt;N%10&gt;&gt;;
}();
template&lt;auto a,auto b&gt;
constexpr auto e_to_str=concat&lt;n_to_str&lt;a&gt;,"*",n_to_str&lt;b&gt;,"=",n_to_str&lt;a*b&gt;&gt;;
template&lt;auto l,auto i=1&gt;
constexpr auto gsl=[]{
        if constexpr(i==l)
                return e_to_str&lt;l,i&gt;;
        else
                return concat&lt;e_to_str&lt;l,i&gt;,"\t",gsl&lt;l,i+1&gt;&gt;;
}();
template&lt;auto l,auto i=1&gt;
constexpr auto gml=[]{
        if constexpr(i==l)
                return gsl&lt;i&gt;;
        else
                return concat&lt;gsl&lt;i&gt;,"\n",gml&lt;l,i+1&gt;&gt;;
}();
#include &lt;cstdio&gt;
int main()
{
        std::puts(gml&lt;900&gt;);
        return 0;
}
```

</details>


---

### Comment 2 - MagentaTreehouse

Reduced:
```c++
template<int,int ...b>
constexpr auto concat=[]{
        if constexpr(sizeof...(b)==0)
                return 0;
        else
                return concat<b...>;
}();

int main(){
        (void)concat<0,0>;
}

```
Output:
```
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4659: void clang::LocalInstantiationScope::InstantiatedLocal(const clang::Decl*, clang::Decl*): Assertion `!Current->LocalDecls.contains(D) && "Instantiated local in inner and outer scopes"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 <source>
1.	<source>:10:26: current parser token ';'
2.	<source>:9:11: parsing function body 'main'
3.	<source>:9:11: in compound statement ('{}')
 #0 0x0000000003bf5af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf5af8)
 #1 0x0000000003bf37fc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bf37fc)
 #2 0x0000000003b40ee8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000077ca29e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000077ca29e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000077ca29e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000077ca29e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000077ca29e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000077ca29e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006fd89da clang::LocalInstantiationScope::InstantiatedLocal(clang::Decl const*, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fd89da)
#10 0x0000000006fd9c25 (anonymous namespace)::TemplateInstantiator::transformedLocalDecl(clang::Decl*, llvm::ArrayRef<clang::Decl*>) (.isra.0) SemaTemplateInstantiate.cpp:0:0
#11 0x000000000700a9b8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000700cac6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#13 0x000000000701ed5e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000700c30e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000700f558 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000700fd9b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x700fd9b)
#17 0x00000000070648b6 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70648b6)
#18 0x00000000070ad7b0 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70ad7b0)
#19 0x00000000070ae086 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70ae086)
#20 0x00000000070b7937 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b7937)
#21 0x0000000006e98503 clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e98503)
#22 0x0000000006e98b71 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e98b71)
#23 0x0000000006ea044b clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ea044b)
#24 0x0000000007031835 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformUnresolvedLookupExpr(clang::UnresolvedLookupExpr*, bool) SemaTemplateInstantiate.cpp:0:0
#25 0x000000000700c632 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#26 0x000000000700f558 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#27 0x000000000700f90c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#28 0x000000000703e8f7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformIfStmt(clang::IfStmt*) SemaTemplateInstantiate.cpp:0:0
#29 0x0000000007039224 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#30 0x000000000700b581 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#31 0x000000000700cac6 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#32 0x000000000701ed5e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#33 0x000000000700c30e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#34 0x000000000700f558 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#35 0x000000000700fd9b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x700fd9b)
#36 0x00000000070648b6 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70648b6)
#37 0x00000000070ad7b0 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70ad7b0)
#38 0x00000000070ae086 clang::TemplateDeclInstantiator::VisitVarTemplateSpecializationDecl(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentListInfo const&, llvm::ArrayRef<clang::TemplateArgument>, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70ae086)
#39 0x00000000070b7937 clang::Sema::BuildVarTemplateInstantiation(clang::VarTemplateDecl*, clang::VarDecl*, clang::TemplateArgumentList const*, clang::TemplateArgumentListInfo const&, llvm::SmallVectorImpl<clang::TemplateArgument>&, clang::SourceLocation, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::LocalInstantiationScope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70b7937)
#40 0x0000000006e98503 clang::Sema::CheckVarTemplateId(clang::VarTemplateDecl*, clang::SourceLocation, clang::SourceLocation, clang::TemplateArgumentListInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e98503)
#41 0x0000000006e98b71 clang::Sema::CheckVarTemplateId(clang::CXXScopeSpec const&, clang::DeclarationNameInfo const&, clang::VarTemplateDecl*, clang::NamedDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e98b71)
#42 0x0000000006ea044b clang::Sema::BuildTemplateIdExpr(clang::CXXScopeSpec const&, clang::SourceLocation, clang::LookupResult&, bool, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ea044b)
#43 0x0000000006a1a5c8 clang::Sema::ActOnIdExpression(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::UnqualifiedId&, bool, bool, clang::CorrectionCandidateCallback*, bool, clang::Token*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1a5c8)
#44 0x00000000065b4716 clang::Parser::tryParseCXXIdExpression(clang::CXXScopeSpec&, bool, clang::Token&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b4716)
#45 0x00000000065b49ba clang::Parser::ParseCXXIdExpression(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b49ba)
#46 0x000000000659768a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659768a)
#47 0x0000000006596e4e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6596e4e)
#48 0x0000000006598d37 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6598d37)
#49 0x00000000065a2366 clang::Parser::ParseParenExpression(clang::Parser::ParenParseOption&, bool, bool, clang::OpaquePtr<clang::QualType>&, clang::SourceLocation&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65a2366)
#50 0x0000000006597a91 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6597a91)
#51 0x0000000006598d37 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6598d37)
#52 0x0000000006598dc9 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6598dc9)
#53 0x000000000659d809 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x659d809)
#54 0x0000000006620949 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6620949)
#55 0x0000000006616afe clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6616afe)
#56 0x0000000006617a50 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6617a50)
#57 0x0000000006618935 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6618935)
#58 0x000000000661a13a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x661a13a)
#59 0x000000000652d723 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652d723)
#60 0x0000000006561efd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6561efd)
#61 0x000000000652146e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652146e)
#62 0x0000000006521c2e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6521c2e)
#63 0x00000000065293c3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65293c3)
#64 0x000000000652a2ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x652a2ad)
#65 0x000000000651c7ca clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651c7ca)
#66 0x0000000004578f98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4578f98)
#67 0x0000000004833eb9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4833eb9)
#68 0x00000000047b3dce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47b3dce)
#69 0x000000000491edde clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x491edde)
#70 0x0000000000caf871 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcaf871)
#71 0x0000000000ca73fa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#72 0x00000000045bc1f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#73 0x0000000003b41394 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b41394)
#74 0x00000000045bc7ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#75 0x0000000004582a7d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4582a7d)
#76 0x0000000004583b6d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4583b6d)
#77 0x000000000458af25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458af25)
#78 0x0000000000cac6a3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcac6a3)
#79 0x0000000000b7fe54 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb7fe54)
#80 0x000077ca29e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#81 0x000077ca29e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#82 0x0000000000ca6eae _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xca6eae)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
See Compiler Explorer: https://godbolt.org/z/je31rsn5h

---

### Comment 3 - shafik

Goes back to clang-3.9: https://godbolt.org/z/7sMrEdssd

---

