#  Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.

**Author:** dcb314
**URL:** https://github.com/llvm/llvm-project/issues/102353

## Body

```
clang-20: /home/dcb40b/llvm/trunk/clang/lib/Sema/SemaTemplateInstantiate.cpp:4400:
llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::VarDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*):
Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.
```
This one has been happening for the last couple of days at least.

[HQ3xLiteScaler-d629c9.sh.gz](https://github.com/user-attachments/files/16533062/HQ3xLiteScaler-d629c9.sh.gz)
[HQ3xLiteScaler-d629c9.cpp.gz](https://github.com/user-attachments/files/16533063/HQ3xLiteScaler-d629c9.cpp.gz)



## Comments

### Comment 1 - Endilll

Reduced by me and C-Reduce (https://godbolt.org/z/sYTW5bhr4):
```cpp
template <typename>
bool is_same_v;

template <typename _Tp, typename>
concept same_as = is_same_v<_Tp>;

template <typename _Lhs>
concept assignable_from = requires {
  { 0 } -> same_as<_Lhs>;
};

template <typename>
struct span;

template <assignable_from _Iter>
span(_Iter) -> span<_Iter>;

template <typename>
void doHQScale2() {
  int srcWidth;
  int edgeBuf_storage[srcWidth];
  span s{ edgeBuf_storage };
}
```
Clang 20.0 stack trace:
```
<source>:21:23: warning: variable length arrays in C++ are a Clang extension [-Wvla-cxx-extension]
   21 |   int edgeBuf_storage[srcWidth];
      |                       ^~~~~~~~
<source>:21:23: note: read of non-const variable 'srcWidth' is not allowed in a constant expression
<source>:20:7: note: declared here
   20 |   int srcWidth;
      |       ^

clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4400:
llvm::PointerUnion<clang::Decl*, llvm::SmallVector<clang::VarDecl*, 4>*>* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*):
Assertion `isa<LabelDecl>(D) && "declaration not instantiated in this scope"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:22:28: current parser token ';'
2.	<source>:19:19: parsing function body 'doHQScale2'
3.	<source>:19:19: in compound statement ('{}')
 #0 0x0000000003ab72f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ab72f8)
 #1 0x0000000003ab4fec llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ab4fec)
 #2 0x00000000039fdfe8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000071d0b6842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000071d0b68969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000071d0b6842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000071d0b68287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000071d0b682871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000071d0b6839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000714622e clang::LocalInstantiationScope::findInstantiationOf(clang::Decl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x714622e)
#10 0x00000000071e6166 clang::Sema::FindInstantiatedDecl(clang::SourceLocation, clang::NamedDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71e6166)
#11 0x00000000071385c1 (anonymous namespace)::TemplateInstantiator::TransformDecl(clang::SourceLocation, clang::Decl*) SemaTemplateInstantiate.cpp:0:0
#12 0x000000000717b350 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#13 0x000000000717b82b (anonymous namespace)::TemplateInstantiator::TransformDeclRefExpr(clang::DeclRefExpr*) SemaTemplateInstantiate.cpp:0:0
#14 0x000000000714ec5c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x000000000714ec09 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#16 0x000000000716076d clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#17 0x0000000007161e6f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDecayedType(clang::TypeLocBuilder&, clang::DecayedTypeLoc) SemaTemplateInstantiate.cpp:0:0
#18 0x000000000715f2cb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000716608a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#20 0x00000000071661ac clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::QualType) SemaTemplateInstantiate.cpp:0:0
#21 0x00000000071602ce clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeLocBuilder&, clang::TypeLoc) SemaTemplateInstantiate.cpp:0:0
#22 0x000000000716608a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformType(clang::TypeSourceInfo*) SemaTemplateInstantiate.cpp:0:0
#23 0x000000000716eba3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#24 0x000000000716f14f (anonymous namespace)::TemplateInstantiator::TransformTemplateArgument(clang::TemplateArgumentLoc const&, clang::TemplateArgumentLoc&, bool) SemaTemplateInstantiate.cpp:0:0
#25 0x00000000071728f8 bool clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformTemplateArguments<clang::TemplateArgumentLoc const*>(clang::TemplateArgumentLoc const*, clang::TemplateArgumentLoc const*, clang::TemplateArgumentListInfo&, bool) (.constprop.0) SemaTemplateInstantiate.cpp:0:0
#26 0x000000000717321e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#27 0x000000000714ec34 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#28 0x0000000007189634 clang::Sema::SubstExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7189634)
#29 0x0000000006b88c01 clang::Sema::BuildExprRequirement(clang::Expr*, bool, clang::SourceLocation, clang::concepts::ExprRequirement::ReturnTypeRequirement) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b88c01)
#30 0x000000000718493a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#31 0x00000000071864de (anonymous namespace)::TemplateInstantiator::TransformRequiresExpr(clang::RequiresExpr*) SemaTemplateInstantiate.cpp:0:0
#32 0x000000000714ef89 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#33 0x000000000715e304 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x715e304)
#34 0x00000000067a63f8 clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#35 0x00000000067a805e CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#36 0x00000000067a8686 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a8686)
#37 0x0000000006f99c38 clang::Sema::CheckConceptTemplateId(clang::CXXScopeSpec const&, clang::SourceLocation, clang::DeclarationNameInfo const&, clang::NamedDecl*, clang::ConceptDecl*, clang::TemplateArgumentListInfo const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f99c38)
#38 0x00000000071732c0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformConceptSpecializationExpr(clang::ConceptSpecializationExpr*) SemaTemplateInstantiate.cpp:0:0
#39 0x000000000714ec34 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#40 0x000000000715e304 clang::Sema::SubstConstraintExpr(clang::Expr*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x715e304)
#41 0x00000000067a63f8 clang::ActionResult<clang::Expr*, true> calculateConstraintSatisfaction<calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator>(clang::Sema&, clang::Expr const*, clang::ConstraintSatisfaction&, calculateConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::Expr const*, clang::ConstraintSatisfaction&)::ConstraintEvaluator const&) SemaConcept.cpp:0:0
#42 0x00000000067a805e CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#43 0x00000000067a8686 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::Expr const*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a8686)
#44 0x00000000067a960b clang::Sema::CheckInstantiatedFunctionTemplateConstraints(clang::SourceLocation, clang::FunctionDecl*, llvm::ArrayRef<clang::TemplateArgument>, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67a960b)
#45 0x000000000709842a clang::Sema::FinishTemplateArgumentDeduction(clang::FunctionTemplateDecl*, llvm::SmallVectorImpl<clang::DeducedTemplateArgument>&, unsigned int, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, llvm::SmallVectorImpl<clang::Sema::OriginalCallArg> const*, bool, llvm::function_ref<bool ()>) (.constprop.0) SemaTemplateDeduction.cpp:0:0
#46 0x0000000007098e4a void llvm::function_ref<void ()>::callback_fn<clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>)::'lambda1'()>(long) SemaTemplateDeduction.cpp:0:0
#47 0x00000000066316a1 clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66316a1)
#48 0x00000000070a3f62 clang::Sema::DeduceTemplateArguments(clang::FunctionTemplateDecl*, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::FunctionDecl*&, clang::sema::TemplateDeductionInfo&, bool, bool, clang::QualType, clang::Expr::Classification, llvm::function_ref<bool (llvm::ArrayRef<clang::QualType>)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70a3f62)
#49 0x0000000006eacd5e clang::Sema::AddTemplateOverloadCandidate(clang::FunctionTemplateDecl*, clang::DeclAccessPair, clang::TemplateArgumentListInfo*, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, bool, clang::CallExpr::ADLCallKind, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6eacd5e)
#50 0x0000000006cd2968 clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>)::'lambda0'(bool)::operator()(bool) const SemaInit.cpp:0:0
#51 0x0000000006cd383f clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cd383f)
#52 0x00000000068461ad clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68461ad)
#53 0x000000000684a947 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684a947)
#54 0x0000000006864bc2 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6864bc2)
#55 0x00000000064f4aad clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64f4aad)
#56 0x000000000650345a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x650345a)
#57 0x000000000650c777 clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x650c777)
#58 0x000000000650cc39 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x650cc39)
#59 0x00000000065b7dfa clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b7dfa)
#60 0x00000000065b8b30 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b8b30)
#61 0x00000000065b9a09 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65b9a09)
#62 0x00000000065bb18a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65bb18a)
#63 0x00000000064cf273 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64cf273)
#64 0x0000000006504148 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6504148)
#65 0x00000000065cc3c6 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65cc3c6)
#66 0x00000000065d9a04 clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#67 0x00000000065d9d1a clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d9d1a)
#68 0x000000000650cde3 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x650cde3)
#69 0x00000000064c9c61 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c9c61)
#70 0x00000000064cab0f clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64cab0f)
#71 0x00000000064bdfaa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64bdfaa)
#72 0x00000000043c9ca8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43c9ca8)
#73 0x00000000046556d9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46556d9)
#74 0x00000000045d9e3e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45d9e3e)
#75 0x000000000473ebfe clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x473ebfe)
#76 0x0000000000c91f9f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc91f9f)
#77 0x0000000000c8b12a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#78 0x000000000440d2e9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#79 0x00000000039fe494 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39fe494)
#80 0x000000000440d8df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#81 0x00000000043d34a5 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43d34a5)
#82 0x00000000043d3f0d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43d3f0d)
#83 0x00000000043dbb25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x43dbb25)
#84 0x0000000000c8f2d5 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8f2d5)
#85 0x0000000000b62ee4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xb62ee4)
#86 0x000071d0b6829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#87 0x000071d0b6829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#88 0x0000000000c8abde _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8abde)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (dcb314)

<details>
```
clang-20: /home/dcb40b/llvm/trunk/clang/lib/Sema/SemaTemplateInstantiate.cpp:4400:
llvm::PointerUnion&lt;clang::Decl*, llvm::SmallVector&lt;clang::VarDecl*, 4&gt;*&gt;* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*):
Assertion `isa&lt;LabelDecl&gt;(D) &amp;&amp; "declaration not instantiated in this scope"' failed.
```
This one has been happening for the last couple of days at least.

[HQ3xLiteScaler-d629c9.sh.gz](https://github.com/user-attachments/files/16533062/HQ3xLiteScaler-d629c9.sh.gz)
[HQ3xLiteScaler-d629c9.cpp.gz](https://github.com/user-attachments/files/16533063/HQ3xLiteScaler-d629c9.cpp.gz)


</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (dcb314)

<details>
```
clang-20: /home/dcb40b/llvm/trunk/clang/lib/Sema/SemaTemplateInstantiate.cpp:4400:
llvm::PointerUnion&lt;clang::Decl*, llvm::SmallVector&lt;clang::VarDecl*, 4&gt;*&gt;* clang::LocalInstantiationScope::findInstantiationOf(const clang::Decl*):
Assertion `isa&lt;LabelDecl&gt;(D) &amp;&amp; "declaration not instantiated in this scope"' failed.
```
This one has been happening for the last couple of days at least.

[HQ3xLiteScaler-d629c9.sh.gz](https://github.com/user-attachments/files/16533062/HQ3xLiteScaler-d629c9.sh.gz)
[HQ3xLiteScaler-d629c9.cpp.gz](https://github.com/user-attachments/files/16533063/HQ3xLiteScaler-d629c9.cpp.gz)


</details>


---

### Comment 4 - Endilll

Bisected to 748371183ae769bfb485f1e7466a864bf1db93d5 (#101782)
CC @mizvekov 

---

### Comment 5 - cor3ntin

@mizvekov so we should close this?

---

### Comment 6 - mizvekov

It might be pre-existing but facilitated by that patch. I am still looking.

---

### Comment 7 - shafik

This does not look like it crashes anymore, @mizvekov did you fix this?

---

### Comment 8 - mizvekov

> This does not look like it crashes anymore, @mizvekov did you fix this?

The patch causing it was reverted. A fixed version has not been reapplied yet.

---

### Comment 9 - shafik

> > This does not look like it crashes anymore, [@mizvekov](https://github.com/mizvekov) did you fix this?
> 
> The patch causing it was reverted. A fixed version has not been reapplied yet.

So maybe we should leave this open and when you apply the new patch, we can close this once we confirm you are testing this case in the patch as well?

---

### Comment 10 - mizvekov

> > > This does not look like it crashes anymore, [@mizvekov](https://github.com/mizvekov) did you fix this?
> > 
> > The patch causing it was reverted. A fixed version has not been reapplied yet.
> 
> So maybe we should leave this open and when you apply the new patch, we can close this once we confirm you are testing this case in the patch as well?

Yeah, sounds good to me!

---

