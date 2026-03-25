# [Bug] Clang 17 crashes when default argument refers to another parameter in template class

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/137780
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-05-07T15:54:09Z

## Body

## Description
 
Clang 17.0.6 from LLMV suite sometimes crashes when compiling a C++23 program where a class template constructor defines a default argument that refers to a previous parameter (specifically: T* ptr = new T(), T* p_ptr = ptr).

The crash is non-deterministic: sometimes compilation fails gracefully with an error message (default argument references parameter 'ptr'), but other times Clang segfaults deeply inside ASTContext::getTypeInfoImpl.

This behavior occurs even when the program contains no undefined behavior at runtime.

## Command executed

```
 clang-17  -x c++ -std=c++23 test.cpp
```

## Crash Backtrace

```
Stack dump:
0.      Program arguments: /usr/lib/llvm-17/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fcoverage-compilation-dir=/root -resource-dir /usr/lib/llvm-17/lib/clang/17 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /usr/lib/llvm-17/lib/clang/17/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -fdebug-compilation-dir=/root -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-bf8357.o -x c++ prueba.cpp
1.      prueba.cpp:19:40: current parser token ','
2.      prueba.cpp:18:1: parsing function body 'main'
3.      prueba.cpp:18:1: in compound statement ('{}')
 #0 0x00007f6c291e2ce6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xccbce6)
 #1 0x00007f6c291e0e80 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xcc9e80)
 #2 0x00007f6c291e3380 (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xccc380)
 #3 0x00007f6c33793420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f6c306b770c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb2570c)
 #5 0x00007f6c306b8a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #6 0x00007f6c306b7c36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #7 0x00007f6c306b8a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #8 0x00007f6c306b9143 clang::ASTContext::getTypeAlignInChars(clang::QualType) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb27143)
 #9 0x00007f6c30e2d955 clang::Sema::CheckArgAlignment(clang::SourceLocation, clang::NamedDecl*, llvm::StringRef, clang::QualType, clang::QualType) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x129b955)
#10 0x00007f6c30e2e79e clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef<clang::Expr const*>, bool, clang::SourceLocation, clang::SourceRange, clang::Sema::VariadicCallType) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x129c79e)
#11 0x00007f6c30e2f5d5 clang::Sema::CheckConstructorCall(clang::FunctionDecl*, clang::QualType, llvm::ArrayRef<clang::Expr const*>, clang::FunctionProtoType const*, clang::SourceLocation) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x129d5d5)
#12 0x00007f6c30fd7ac4 clang::Sema::CompleteConstructorCall(clang::CXXConstructorDecl*, clang::QualType, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, llvm::SmallVectorImpl<clang::Expr*>&, bool, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x1445ac4)
#13 0x00007f6c3122fd5e (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x169dd5e)
#14 0x00007f6c3122b93d clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x169993d)
#15 0x00007f6c31186152 clang::Sema::BuildCXXNew(clang::SourceRange, bool, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::SourceRange, clang::QualType, clang::TypeSourceInfo*, std::optional<clang::Expr*>, clang::SourceRange, clang::Expr*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x15f4152)
#16 0x00007f6c314cd14a (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x193b14a)
#17 0x00007f6c314e5fc0 (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x1953fc0)
#18 0x00007f6c30d8bfad clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x11f9fad)
#19 0x00007f6c314c352e clang::Sema::SubstDefaultArgument(clang::SourceLocation, clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x193152e)
#20 0x00007f6c31505a55 clang::Sema::InstantiateDefaultArgument(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x1973a55)
#21 0x00007f6c3107fcef clang::Sema::BuildCXXDefaultArgExpr(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*, clang::Expr*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x14edcef)
#22 0x00007f6c31082453 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef<clang::Expr*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::Sema::VariadicCallType, bool, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x14f0453)
#23 0x00007f6c30fd7a2a clang::Sema::CompleteConstructorCall(clang::CXXConstructorDecl*, clang::QualType, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, llvm::SmallVectorImpl<clang::Expr*>&, bool, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x1445a2a)
#24 0x00007f6c3122fd5e (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x169dd5e)
#25 0x00007f6c3122b93d clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x169993d)
#26 0x00007f6c3118347e clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x15f147e)
#27 0x00007f6c31182b43 clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr<clang::QualType>, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x15f0b43)
#28 0x00007f6c3062721f clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa9521f)
#29 0x00007f6c30614f21 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa82f21)
#30 0x00007f6c3061568a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa8368a)
#31 0x00007f6c3060ee21 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa7ce21)
#32 0x00007f6c305dfdfb clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4ddfb)
#33 0x00007f6c305dd130 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4b130)
#34 0x00007f6c305dc67d clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4a67d)
#35 0x00007f6c305dc200 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4a200)
#36 0x00007f6c3066ef76 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xadcf76)
#37 0x00007f6c3066d3f8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xadb3f8)
#38 0x00007f6c30675850 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xae3850)
#39 0x00007f6c306768ff clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xae48ff)
#40 0x00007f6c30691003 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xaff003)
#41 0x00007f6c305ddf30 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4bf30)
#42 0x00007f6c306900a7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xafe0a7)
#43 0x00007f6c3068faa1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xafdaa1)
#44 0x00007f6c3068ed43 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xafcd43)
#45 0x00007f6c3068d1e9 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xafb1e9)
#46 0x00007f6c305c923e clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa3723e)
#47 0x00007f6c322e9be5 clang::FrontendAction::Execute() (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x2757be5)
#48 0x00007f6c32265a74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x26d3a74)
#49 0x00007f6c32363264 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x27d1264)
#50 0x00005611a560a7e3 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-17/bin/clang+0x127e3)
#51 0x00005611a5608375 (/usr/lib/llvm-17/bin/clang+0x10375)
#52 0x00005611a5607514 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-17/bin/clang+0xf514)
#53 0x00005611a5614522 main (/usr/lib/llvm-17/bin/clang+0x1c522)
#54 0x00007f6c27ffb083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#55 0x00005611a5604dae _start (/usr/lib/llvm-17/bin/clang+0xcdae)
```

(Shortened version)
```
 #0 0x00007f9ab0a1ace6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xccbce6)
  #1 0x00007f9ab0a18e80 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xcc9e80)
  #2 0x00007f9ab0a1b380 (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xccc380)
  #3 0x00007f9abafcb420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
  #4 0x00007f9ab7ef09c6 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb269c6)
  #5 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
  #6 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
  #7 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
  #8 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
  #9 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #10 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #11 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #12 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #13 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #14 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #15 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #16 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #17 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #18 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
```

## Program

```cpp
#include <iostream>
using namespace std;
template <typename T> struct
MyObject
{
    MyObject(T * ptr = new T(), T * p_ptr = ptr)
        : p_ptr(ptr)
    {}
    ~MyObject()
    {
        delete p_ptr;
    }
    T * p_ptr;
};
typedef MyObject<int> MyIntObject;
typedef MyObject<MyIntObject> MyNestedObject;
int main()
{
    MyNestedObject n, n2;
    n2 = MyNestedObject(&n);
}
```

To reproduce quickly:

https://gcc.godbolt.org/z/Whx9rcxKx

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Description
 
Clang 17.0.6 from LLMV suite sometimes crashes when compiling a C++23 program where a class template constructor defines a default argument that refers to a previous parameter (specifically: T* ptr = new T(), T* p_ptr = ptr).

The crash is non-deterministic: sometimes compilation fails gracefully with an error message (default argument references parameter 'ptr'), but other times Clang segfaults deeply inside ASTContext::getTypeInfoImpl.

This behavior occurs even when the program contains no undefined behavior at runtime.

## Command executed

```
 clang-17  -x c++ -std=c++23 test.cpp
```

## Crash Backtrace

```
Stack dump:
0.      Program arguments: /usr/lib/llvm-17/bin/clang -cc1 -triple x86_64-pc-linux-gnu -emit-obj -mrelax-all -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fcoverage-compilation-dir=/root -resource-dir /usr/lib/llvm-17/lib/clang/17 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /usr/lib/llvm-17/lib/clang/17/include -internal-isystem /usr/local/include -internal-isystem /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -fdebug-compilation-dir=/root -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-bf8357.o -x c++ prueba.cpp
1.      prueba.cpp:19:40: current parser token ','
2.      prueba.cpp:18:1: parsing function body 'main'
3.      prueba.cpp:18:1: in compound statement ('{}')
 #<!-- -->0 0x00007f6c291e2ce6 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xccbce6)
 #<!-- -->1 0x00007f6c291e0e80 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xcc9e80)
 #<!-- -->2 0x00007f6c291e3380 (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xccc380)
 #<!-- -->3 0x00007f6c33793420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x00007f6c306b770c clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb2570c)
 #<!-- -->5 0x00007f6c306b8a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #<!-- -->6 0x00007f6c306b7c36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #<!-- -->7 0x00007f6c306b8a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #<!-- -->8 0x00007f6c306b9143 clang::ASTContext::getTypeAlignInChars(clang::QualType) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb27143)
 #<!-- -->9 0x00007f6c30e2d955 clang::Sema::CheckArgAlignment(clang::SourceLocation, clang::NamedDecl*, llvm::StringRef, clang::QualType, clang::QualType) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x129b955)
#<!-- -->10 0x00007f6c30e2e79e clang::Sema::checkCall(clang::NamedDecl*, clang::FunctionProtoType const*, clang::Expr const*, llvm::ArrayRef&lt;clang::Expr const*&gt;, bool, clang::SourceLocation, clang::SourceRange, clang::Sema::VariadicCallType) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x129c79e)
#<!-- -->11 0x00007f6c30e2f5d5 clang::Sema::CheckConstructorCall(clang::FunctionDecl*, clang::QualType, llvm::ArrayRef&lt;clang::Expr const*&gt;, clang::FunctionProtoType const*, clang::SourceLocation) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x129d5d5)
#<!-- -->12 0x00007f6c30fd7ac4 clang::Sema::CompleteConstructorCall(clang::CXXConstructorDecl*, clang::QualType, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x1445ac4)
#<!-- -->13 0x00007f6c3122fd5e (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x169dd5e)
#<!-- -->14 0x00007f6c3122b93d clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x169993d)
#<!-- -->15 0x00007f6c31186152 clang::Sema::BuildCXXNew(clang::SourceRange, bool, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::SourceRange, clang::QualType, clang::TypeSourceInfo*, std::optional&lt;clang::Expr*&gt;, clang::SourceRange, clang::Expr*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x15f4152)
#<!-- -->16 0x00007f6c314cd14a (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x193b14a)
#<!-- -->17 0x00007f6c314e5fc0 (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x1953fc0)
#<!-- -->18 0x00007f6c30d8bfad clang::Sema::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x11f9fad)
#<!-- -->19 0x00007f6c314c352e clang::Sema::SubstDefaultArgument(clang::SourceLocation, clang::ParmVarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x193152e)
#<!-- -->20 0x00007f6c31505a55 clang::Sema::InstantiateDefaultArgument(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x1973a55)
#<!-- -->21 0x00007f6c3107fcef clang::Sema::BuildCXXDefaultArgExpr(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*, clang::Expr*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x14edcef)
#<!-- -->22 0x00007f6c31082453 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::Sema::VariadicCallType, bool, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x14f0453)
#<!-- -->23 0x00007f6c30fd7a2a clang::Sema::CompleteConstructorCall(clang::CXXConstructorDecl*, clang::QualType, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x1445a2a)
#<!-- -->24 0x00007f6c3122fd5e (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x169dd5e)
#<!-- -->25 0x00007f6c3122b93d clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x169993d)
#<!-- -->26 0x00007f6c3118347e clang::Sema::BuildCXXTypeConstructExpr(clang::TypeSourceInfo*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x15f147e)
#<!-- -->27 0x00007f6c31182b43 clang::Sema::ActOnCXXTypeConstructExpr(clang::OpaquePtr&lt;clang::QualType&gt;, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x15f0b43)
#<!-- -->28 0x00007f6c3062721f clang::Parser::ParseCXXTypeConstructExpression(clang::DeclSpec const&amp;) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa9521f)
#<!-- -->29 0x00007f6c30614f21 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa82f21)
#<!-- -->30 0x00007f6c3061568a clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa8368a)
#<!-- -->31 0x00007f6c3060ee21 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa7ce21)
#<!-- -->32 0x00007f6c305dfdfb clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4ddfb)
#<!-- -->33 0x00007f6c305dd130 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4b130)
#<!-- -->34 0x00007f6c305dc67d clang::Parser::ParseSimpleDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, bool, clang::Parser::ForRangeInit*, clang::SourceLocation*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4a67d)
#<!-- -->35 0x00007f6c305dc200 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4a200)
#<!-- -->36 0x00007f6c3066ef76 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xadcf76)
#<!-- -->37 0x00007f6c3066d3f8 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xadb3f8)
#<!-- -->38 0x00007f6c30675850 clang::Parser::ParseCompoundStatementBody(bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xae3850)
#<!-- -->39 0x00007f6c306768ff clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xae48ff)
#<!-- -->40 0x00007f6c30691003 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xaff003)
#<!-- -->41 0x00007f6c305ddf30 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa4bf30)
#<!-- -->42 0x00007f6c306900a7 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xafe0a7)
#<!-- -->43 0x00007f6c3068faa1 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xafdaa1)
#<!-- -->44 0x00007f6c3068ed43 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xafcd43)
#<!-- -->45 0x00007f6c3068d1e9 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xafb1e9)
#<!-- -->46 0x00007f6c305c923e clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xa3723e)
#<!-- -->47 0x00007f6c322e9be5 clang::FrontendAction::Execute() (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x2757be5)
#<!-- -->48 0x00007f6c32265a74 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x26d3a74)
#<!-- -->49 0x00007f6c32363264 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0x27d1264)
#<!-- -->50 0x00005611a560a7e3 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-17/bin/clang+0x127e3)
#<!-- -->51 0x00005611a5608375 (/usr/lib/llvm-17/bin/clang+0x10375)
#<!-- -->52 0x00005611a5607514 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-17/bin/clang+0xf514)
#<!-- -->53 0x00005611a5614522 main (/usr/lib/llvm-17/bin/clang+0x1c522)
#<!-- -->54 0x00007f6c27ffb083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->55 0x00005611a5604dae _start (/usr/lib/llvm-17/bin/clang+0xcdae)
```

(Shortened version)
```
 #<!-- -->0 0x00007f9ab0a1ace6 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xccbce6)
  #<!-- -->1 0x00007f9ab0a18e80 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xcc9e80)
  #<!-- -->2 0x00007f9ab0a1b380 (/usr/lib/llvm-17/bin/../lib/libLLVM-17.so.1+0xccc380)
  #<!-- -->3 0x00007f9abafcb420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
  #<!-- -->4 0x00007f9ab7ef09c6 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb269c6)
  #<!-- -->5 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
  #<!-- -->6 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
  #<!-- -->7 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
  #<!-- -->8 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
  #<!-- -->9 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #<!-- -->10 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #<!-- -->11 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #<!-- -->12 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #<!-- -->13 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #<!-- -->14 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #<!-- -->15 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #<!-- -->16 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
 #<!-- -->17 0x00007f9ab7eefc36 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb25c36)
 #<!-- -->18 0x00007f9ab7ef0a70 clang::ASTContext::getTypeInfo(clang::Type const*) const (/usr/lib/llvm-17/bin/../lib/libclang-cpp.so.17+0xb26a70)
```

## Program

```cpp
#include &lt;iostream&gt;
using namespace std;
template &lt;typename T&gt; struct
MyObject
{
    MyObject(T * ptr = new T(), T * p_ptr = ptr)
        : p_ptr(ptr)
    {}
    ~MyObject()
    {
        delete p_ptr;
    }
    T * p_ptr;
};
typedef MyObject&lt;int&gt; MyIntObject;
typedef MyObject&lt;MyIntObject&gt; MyNestedObject;
int main()
{
    MyNestedObject n, n2;
    n2 = MyNestedObject(&amp;n);
}
```

To reproduce quickly:

https://gcc.godbolt.org/z/Whx9rcxKx
</details>


---

### Comment 2 - EugeneZelenko

`main` does not crash: https://gcc.godbolt.org/z/orcGfaMcv. Only most recent release (20 as of now) is maintained.

---

### Comment 3 - mariete1223

Anyways, would it be interesting to upload found bugs in clang-17? i have a few of them, that maybe haven´t been uploaded yet.

---

### Comment 4 - EugeneZelenko

If these bugs still persist in 20 or `main`.

---

