# Clang crash on deduced type with typo correction

**Author:** kadircet
**URL:** https://github.com/llvm/llvm-project/issues/59391
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-06-13T10:45:42Z

## Body

```cpp
namespace a {
template <typename b> class c {
  c(b);
  b e;
  class d {
    void f() {
      auto x = a::c(cores);
      for (auto core : a::c(cores)) {
      }
    }
  };
};
} // namespace a
```

is a reproducer for clang with `-std=c++17`.
Assertion that fires and the stack trace:

```console
$ clang -xc++ -fsyntax-only test_it.cc -std=c++17
test_it.cc:7:21: error: use of undeclared identifier 'cores'
      auto x = a::c(cores);
                    ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang-14 -xc++ -c -Xclang -ast-dump test_it.cc -std=c++17 -Xclang -fno-recovery-ast
1.      test_it.cc:8:37: current parser token '{'
2.      test_it.cc:1:1: parsing namespace 'a'
3.      test_it.cc:2:23: parsing struct/union/class body 'a::c'
4.      test_it.cc:6:14: parsing function body 'a::c::d::f'
5.      test_it.cc:6:14: in compound statement ('{}')
 #0 0x00007ffa34aa2671 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/x86_64-linux-gnu/libLLVM-14.so.1+0xea2671)
 #1 0x00007ffa34aa03ce llvm::sys::RunSignalHandlers() (/lib/x86_64-linux-gnu/libLLVM-14.so.1+0xea03ce)
 #2 0x00007ffa34aa1a3b llvm::sys::CleanupOnSignal(unsigned long) (/lib/x86_64-linux-gnu/libLLVM-14.so.1+0xea1a3b)
 #3 0x00007ffa349c98ef (/lib/x86_64-linux-gnu/libLLVM-14.so.1+0xdc98ef)
 #4 0x00007ffa3343daa0 (/lib/x86_64-linux-gnu/libc.so.6+0x3daa0)
 #5 0x00007ffa3b114d83 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xb14d83)
 #6 0x00007ffa3b115cf6 clang::ASTContext::getTypeInfo(clang::Type const*) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xb15cf6)
 #7 0x00007ffa3b114d8e clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xb14d8e)
 #8 0x00007ffa3b115cf6 clang::ASTContext::getTypeInfo(clang::Type const*) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xb15cf6)
 #9 0x00007ffa3b1159b9 clang::ASTContext::getTypeInfoInChars(clang::QualType) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xb159b9)
#10 0x00007ffa3b4607c4 (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xe607c4)
#11 0x00007ffa3b45cd76 (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xe5cd76)
#12 0x00007ffa3b45821d clang::ASTContext::getASTRecordLayout(clang::RecordDecl const*) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xe5821d)
#13 0x00007ffa3b114e6c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xb14e6c)
#14 0x00007ffa3b115cf6 clang::ASTContext::getTypeInfo(clang::Type const*) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xb15cf6)
#15 0x00007ffa3b1163e3 clang::ASTContext::getTypeAlignInChars(clang::QualType) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xb163e3)
#16 0x00007ffa3b7e2eaa clang::Sema::CheckArgAlignment(clang::SourceLocation, clang::NamedDecl*, llvm::StringRef, clang::QualType, clang::QualType) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x11e2eaa)
#17 0x00007ffa3b7e4b5f clang::Sema::CheckConstructorCall(clang::FunctionDecl*, clang::QualType, llvm::ArrayRef<clang::Expr const*>, clang::FunctionProtoType const*, clang::SourceLocation) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x11e4b5f)
#18 0x00007ffa3b964d72 clang::Sema::CompleteConstructorCall(clang::CXXConstructorDecl*, clang::QualType, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, llvm::SmallVectorImpl<clang::Expr*>&, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x1364d72)
#19 0x00007ffa3bb6a075 (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x156a075)
#20 0x00007ffa3bb66534 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x1566534)
#21 0x00007ffa3b7cd53d (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x11cd53d)
#22 0x00007ffa3b7c8d28 (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x11c8d28)
#23 0x00007ffa3b7c8e2a clang::Sema::BuildCXXFunctionalCastExpr(clang::TypeSourceInfo*, clang::QualType, clang::SourceLocation, clang::Expr*, clang::SourceLocation) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x11c8e2a)
#24 0x00007ffa3bab9b8d clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x14b9b8d)
#25 0x00007ffa3badc185 (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x14dc185)
#26 0x00007ffa3bad4f88 (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x14d4f88)
#27 0x00007ffa3bab9e25 clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x14b9e25)
#28 0x00007ffa3b0d6a4b clang::Parser::ParseForStatement(clang::SourceLocation*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xad6a4b)
#29 0x00007ffa3b0d1910 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributesWithRange&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xad1910)
#30 0x00007ffa3b0d0eef clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xad0eef)
#31 0x00007ffa3b0d9841 clang::Parser::ParseCompoundStatementBody(bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xad9841)
#32 0x00007ffa3b0da768 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xada768)
#33 0x00007ffa3b0387d1 clang::Parser::ParseLexedMethodDef(clang::Parser::LexedMethod&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa387d1)
#34 0x00007ffa3b03729a clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa3729a)
#35 0x00007ffa3b03729a clang::Parser::ParseLexedMethodDefs(clang::Parser::ParsingClass&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa3729a)
#36 0x00007ffa3b06a56b clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributesWithRange&, unsigned int, clang::Decl*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa6a56b)
#37 0x00007ffa3b067c26 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributesWithRange&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa67c26)
#38 0x00007ffa3b0477a4 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo const&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa477a4)
#39 0x00007ffa3b0e221a clang::Parser::ParseSingleDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo const&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xae221a)
#40 0x00007ffa3b0e14fd clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xae14fd)
#41 0x00007ffa3b0e0e4f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xae0e4f)
#42 0x00007ffa3b04669b clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributesWithRange&, clang::SourceLocation*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa4669b)
#43 0x00007ffa3b0f1bc4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xaf1bc4)
#44 0x00007ffa3b060c0c clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa60c0c)
#45 0x00007ffa3b060759 clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa60759)
#46 0x00007ffa3b0467de clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributesWithRange&, clang::SourceLocation*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa467de)
#47 0x00007ffa3b0f1bc4 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributesWithRange&, clang::ParsingDeclSpec*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xaf1bc4)
#48 0x00007ffa3b0f0a6d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xaf0a6d)
#49 0x00007ffa3b0f05d7 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xaf05d7)
#50 0x00007ffa3b033726 clang::ParseAST(clang::Sema&, bool, bool) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0xa33726)
#51 0x00007ffa3cb44ac7 clang::FrontendAction::Execute() (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x2544ac7)
#52 0x00007ffa3ca9c316 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x249c316)
#53 0x00007ffa3cbbe43b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x25be43b)
#54 0x0000000000413edb cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-14/bin/clang+0x413edb)
#55 0x000000000041210c (/usr/lib/llvm-14/bin/clang+0x41210c)
#56 0x00007ffa3c71ab92 (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x211ab92)
#57 0x00007ffa349c965d llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/lib/x86_64-linux-gnu/libLLVM-14.so.1+0xdc965d)
#58 0x00007ffa3c71a680 clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x211a680)
#59 0x00007ffa3c6e4e83 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x20e4e83)
#60 0x00007ffa3c6e510a clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) const (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x20e510a)
#61 0x00007ffa3c6ff1c7 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/lib/x86_64-linux-gnu/libclang-cpp.so.14+0x20ff1c7)
#62 0x0000000000411b76 main (/usr/lib/llvm-14/bin/clang+0x411b76)
#63 0x00007ffa3342920a __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:58:16
#64 0x00007ffa334292bc call_init ./csu/../csu/libc-start.c:128:20
#65 0x00007ffa334292bc __libc_start_main ./csu/../csu/libc-start.c:376:5
#66 0x000000000040f00a _start (/usr/lib/llvm-14/bin/clang+0x40f00a)
```

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 2 - shafik

Confirmed.

Assertion:

```console
clang++: /root/llvm-project/clang/lib/AST/ASTContext.cpp:2396:
clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const:
Assertion `!A->getDeducedType().isNull() && "cannot request the size of an undeduced or dependent auto type"' failed.
```

---

