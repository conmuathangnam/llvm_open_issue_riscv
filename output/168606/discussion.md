# [clang] Compiler crashes when reflecting inside lambda (reflection - C++26)

**Author:** kamshi
**URL:** https://github.com/llvm/llvm-project/issues/168606
**Status:** Closed
**Labels:** invalid, clang:frontend, crash, lambda, c++26
**Closed Date:** 2025-11-19T03:44:43Z

## Body

The following code crashes the compiler (reflection - C++26):
https://godbolt.org/z/Yj1o1Gnhh

```
#include <experimental/meta>
#include <string_view>
#include <type_traits>

enum class TestEnum { A, B, C };

template <auto Member>
constexpr auto bind_enum_member(auto& to) {
    if constexpr (!is_special_member_function(Member)) {
        to.template registerEnumMember<&[:Member:]>(display_string_of(Member));
    }
}

template <typename U>
class EnumBinder {
    template <auto Member>
    void registerEnumMember([[maybe_unused]] const std::string_view name) {}
};

template <typename MetaEnum>
constexpr auto bind_enum() {
    EnumBinder<MetaEnum> result;
    auto bind_it = [&result]<auto E>() {
        template for (constexpr auto mem : std::define_static_array(enumerators_of(E))) {
            bind_enum_member<mem>(result);
        }
    };
    bind_it.template operator()<^^MetaEnum>();
    return result;
}

int main() { const auto enumBinder = bind_enum<TestEnum>(); }
```



Stack dump:
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S -std=c++26 -freflection -stdlib=libc++ -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 -O3 -fexpansion-statements <source>
1.	<source>:32:58: current parser token ')'
2.	<source>:32:12: parsing function body 'main'
3.	<source>:32:12: in compound statement ('{}')
4.	<source>:21:16: instantiating function definition 'bind_enum<TestEnum>'
5.	<source>:23:20: instantiating function definition 'bind_enum()::(anonymous class)::operator()<^^(type)>'
 #0 0x0000000003afdf78 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x3afdf78)
 #1 0x0000000003afb94c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x3afb94c)
 #2 0x0000000003a4bd68 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007347cc042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006e27340 clang::Decl::castFromDeclContext(clang::DeclContext const*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x6e27340)
 #5 0x00000000062b33fd clang::Sema::tryCaptureVariable(clang::ValueDecl*, clang::SourceLocation, clang::TryCaptureKind, clang::SourceLocation, bool, clang::QualType&, clang::QualType&, unsigned int const*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x62b33fd)
 #6 0x000000000642862d clang::getStackIndexOfNearestEnclosingCaptureCapableLambda(llvm::ArrayRef<clang::sema::FunctionScopeInfo const*>, clang::ValueDecl*, clang::Sema&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x642862d)
 #7 0x000000000637160a void llvm::function_ref<void (clang::ValueDecl*, clang::Expr*)>::callback_fn<CheckIfAnyEnclosingLambdasMustCaptureAnyPotentialCaptures(clang::Expr*, clang::sema::LambdaScopeInfo*, clang::Sema&)::'lambda'(clang::ValueDecl*, clang::Expr*)>(long, clang::ValueDecl*, clang::Expr*) SemaExprCXX.cpp:0:0
 #8 0x0000000006a34b5a clang::sema::LambdaScopeInfo::visitPotentialCaptures(llvm::function_ref<void (clang::ValueDecl*, clang::Expr*)>) const (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x6a34b5a)
 #9 0x000000000638c828 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x638c828)
#10 0x000000000613921f clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x613921f)
#11 0x0000000006935d48 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x6935d48)
#12 0x0000000006981332 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 1u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x6981332)
#13 0x000000000698e593 clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x698e593)
#14 0x000000000699bd07 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#15 0x00000000075c3d41 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x75c3d41)
#16 0x00000000069341ea clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x69341ea)
#17 0x00000000068d09c4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#18 0x00000000069264c5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#19 0x000000000692d7fb clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x692d7fb)
#20 0x00000000062393d6 clang::Sema::FinishCXXExpansionStmt(clang::Stmt*, clang::Stmt*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x62393d6)
#21 0x000000000692d2eb clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXIndeterminateExpansionStmt(clang::CXXIndeterminateExpansionStmt*) SemaTemplateInstantiate.cpp:0:0
#22 0x000000000692d7fb clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x692d7fb)
#23 0x00000000069335e2 clang::TemplateDeclInstantiator::VisitExpansionStmtDecl(clang::ExpansionStmtDecl*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x69335e2)
#24 0x000000000699c0e3 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#25 0x00000000075c3d41 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x75c3d41)
#26 0x00000000069341ea clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x69341ea)
#27 0x00000000068d09c4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#28 0x00000000069264c5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#29 0x000000000692d7fb clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x692d7fb)
#30 0x00000000069864d2 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x69864d2)
#31 0x00000000075c3d41 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x75c3d41)
#32 0x000000000682b4a1 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x682b4a1)
#33 0x000000000628c206 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x628c206)
#34 0x0000000006676018 clang::Sema::BuildCallToMemberFunction(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x6676018)
#35 0x00000000062e527d clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x62e527d)
#36 0x00000000062e9f1c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x62e9f1c)
#37 0x0000000006905033 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#38 0x00000000068e6851 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#39 0x0000000006925e59 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#40 0x00000000069264c5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#41 0x000000000692d7fb clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x692d7fb)
#42 0x00000000069864d2 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x69864d2)
#43 0x00000000075c3d41 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x75c3d41)
#44 0x000000000682b4a1 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x682b4a1)
#45 0x000000000628c206 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x628c206)
#46 0x0000000006678544 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#47 0x0000000006679588 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x6679588)
#48 0x00000000062e5a0f clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x62e5a0f)
#49 0x00000000062e9f1c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x62e9f1c)
#50 0x0000000005de14ae clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5de14ae)
#51 0x0000000005dd938a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5dd938a)
#52 0x0000000005ddb1f1 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5ddb1f1)
#53 0x0000000005ddb7ea clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5ddb7ea)
#54 0x0000000005ddb9d9 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5ddb9d9)
#55 0x0000000005d9177d clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5d9177d)
#56 0x0000000005da7790 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5da7790)
#57 0x0000000005daa56a clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5daa56a)
#58 0x0000000005daaa84 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5daaa84)
#59 0x0000000005e7a7c3 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5e7a7c3)
#60 0x0000000005e7b0f6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5e7b0f6)
#61 0x0000000005e83a2d clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5e83a2d)
#62 0x0000000005e8446a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5e8446a)
#63 0x0000000005d5b326 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5d5b326)
#64 0x0000000005da7be3 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5da7be3)
#65 0x0000000005d53717 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5d53717)
#66 0x0000000005d5452e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5d5452e)
#67 0x0000000005d5dff2 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5d5dff2)
#68 0x0000000005d5f8e8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5d5f8e8)
#69 0x0000000005d4da7a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x5d4da7a)
#70 0x000000000444f2cd clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x444f2cd)
#71 0x000000000476068a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x476068a)
#72 0x00000000046db57b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x46db57b)
#73 0x0000000004855e43 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x4855e43)
#74 0x0000000000d325e5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0xd325e5)
#75 0x0000000000d2a51d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#76 0x00000000044c13f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#77 0x0000000003a4c183 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x3a4c183)
#78 0x00000000044c1619 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#79 0x000000000448445d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x448445d)
#80 0x0000000004485411 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x4485411)
#81 0x00000000044929dc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0x44929dc)
#82 0x0000000000d2ee31 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0xd2ee31)
#83 0x0000000000beed24 main (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0xbeed24)
#84 0x00007347cc029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#85 0x00007347cc029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#86 0x0000000000d29fb5 _start (/opt/compiler-explorer/clang-bb-p2996-trunk/bin/clang+++0xd29fb5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

## Comments

### Comment 1 - shafik

We normally don't take bug reports on forks.

Reflection is currently in the process of being upstreamed into clang but that will take a while. So this may be helpful in adding to the test suite to make sure once it is functional it no longer has this bug.

CC @katzdm @changkhothuychung who will want to track this for future reference. 

---

### Comment 2 - katzdm

> We normally don't take bug reports on forks.
> 
> Reflection is currently in the process of being upstreamed into clang but that will take a while. So this may be helpful in adding to the test suite to make sure once it is functional it no longer has this bug.
> 
> CC [@katzdm](https://github.com/katzdm) [@changkhothuychung](https://github.com/changkhothuychung) who will want to track this for future reference.

Please file here instead: https://github.com/bloomberg/clang-p2996

Thanks!

---

### Comment 3 - frederick-vs-ja

Slightly reduced and reported bloomberg#209. Closing this.

---

### Comment 4 - frederick-vs-ja

Reduced to the following which is unrelated to refection. ([Godbolt link](https://godbolt.org/z/Yze1aWPd7))
```C++
constexpr bool test() {
    static constexpr int arr[]{1, 2, 3};
    [n = 42]<class = void>() {
        template for (constexpr auto _ : arr) {
            (void)n;
        }
    }();
    return true;
}
```

---

