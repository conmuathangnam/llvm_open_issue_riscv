# Clang assertion failure in ASTStmtReader::VisitConstantExpr()

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/140130
**Status:** Closed
**Labels:** c++20, clang:frontend, clang:modules, crash
**Closed Date:** 2025-05-16T14:29:35Z

## Body

Clang crashes when reading AST with constexpr structs containing unions. A test case reduced from real-world code:
```
$ split-file - q
//--- a.cppmap
module "a" {
 header "a.h"
}
//--- a.h
#include "b.h"
//--- b.h
#ifndef _B_H_
#define _B_H_
struct B {
  consteval B() {}
  union {
    int a;
  };
};
constexpr B b;
#endif
//--- c.cppmap
module "c" {
header "c.h"
}
//--- c.h
#include "b.h"
^D
$ cd q
$ $CLANG -iquote . -fmodule-name=c -Xclang=-fno-cxx-modules -Xclang=-fmodule-map-file-home-is-cwd -xc++ -Xclang=-emit-module -fmodules -fno-implicit-modules -fno-implicit-module-maps -std=c++20 -c c.cppmap -o c.pcm && $CLANG -iquote . -fmodule-name=a -fmodule-map-file=a.cppmap -Xclang=-fno-cxx-modules -Xclang=-fmodule-map-file-home-is-cwd -xc++ -Xclang=-emit-module -fmodules -fno-implicit-modules -fno-implicit-module-maps -Xclang=-fmodule-file=c.pcm -std=c++20 -c a.cppmap -o a.pcm
assert.h assertion failed at llvm/include/llvm/Support/Casting.h:109 in static bool llvm::isa_impl_cl<clang::FieldDecl, const clang::Decl *>::doit(const From *) [To = clang::FieldDecl, From = const clang::Decl *]: Val && "isa<> used on a null pointer"
    @     0x55c79770988a  __assert_fail
    @     0x55c7878e6fb0  llvm::isa_impl_cl<>::doit()
    @     0x55c7878e6f38  llvm::isa_impl_wrap<>::doit()
    @     0x55c7878e6f15  llvm::isa_impl_wrap<>::doit()
    @     0x55c7878e6e88  llvm::CastIsPossible<>::isPossible()
    @     0x55c7878e6e55  llvm::CastInfo<>::isPossible()
    @     0x55c7878e6db8  llvm::isa<>()
    @     0x55c7878e7230  llvm::cast<>()
    @     0x55c78a88b77c  clang::serialization::BasicReaderBase<>::readAPValue()
    @     0x55c78a891260  clang::serialization::ReadDispatcher<>::read<>()
    @     0x55c78a88f2bd  clang::serialization::DataStreamBasicReader<>::readArray<>()
    @     0x55c78a88b399  clang::serialization::BasicReaderBase<>::readAPValue()
    @     0x55c78a9eaaed  clang::ASTStmtReader::VisitConstantExpr()
    @     0x55c78aa186f4  clang::StmtVisitorBase<>::Visit()
    @     0x55c78aa0a095  clang::ASTReader::ReadStmtFromStream()
    @     0x55c78a7e4a2f  clang::ASTReader::GetExternalDeclStmt()
    @     0x55c78a7e4ac1  clang::ASTReader::GetExternalDeclStmt()
    @     0x55c78d7583bf  clang::LazyOffsetPtr<>::get()
    @     0x55c78d742662  clang::VarDecl::getInit()
    @     0x55c78a982e92  clang::ASTReader::PassInterestingDeclsToConsumer()
    @     0x55c78a7f678e  clang::ASTReader::FinishedDeserializing()
    @     0x55c78a7f67b9  clang::ASTReader::FinishedDeserializing()
    @     0x55c78a811032  clang::ExternalASTSource::Deserializing::~Deserializing()
    @     0x55c78a7e6cdc  clang::ASTReader::FindExternalVisibleDeclsByName()
    @     0x55c78a7e6d41  clang::ASTReader::FindExternalVisibleDeclsByName()
    @     0x55c78d77c4fc  clang::DeclContext::lookupImpl()
    @     0x55c78d77c0d3  clang::DeclContext::lookup()
    @     0x55c78b9bba58  LookupDirect()
    @     0x55c78b9b72d5  CppNamespaceLookup()
    @     0x55c78b9b6656  clang::Sema::CppLookupName()
    @     0x55c78b9bb787  clang::Sema::LookupName()
    @     0x55c78b137aae  clang::Sema::HandleDeclarator()
    @     0x55c78b136c99  clang::Sema::ActOnDeclarator()
    @     0x55c78a6959da  clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes()
    @     0x55c78a694413  clang::Parser::ParseDeclGroup()
    @     0x55c78a5ab674  clang::Parser::ParseDeclOrFunctionDefInternal()
    @     0x55c78a5aa4b1  clang::Parser::ParseDeclarationOrFunctionDefinition()
    @     0x55c78a5a9a4c  clang::Parser::ParseExternalDeclaration()
    @     0x55c78a5a6ec1  clang::Parser::ParseTopLevelDecl()
    @     0x55c78a59f616  clang::ParseAST()
    @     0x55c789f41475  clang::ASTFrontendAction::ExecuteAction()
    @     0x55c789f40a2b  clang::FrontendAction::Execute()
    @     0x55c789deca4d  clang::CompilerInstance::ExecuteAction()
    @     0x55c78785dea5  clang::ExecuteCompilerInvocation()
    @     0x55c78782b668  cc1_main()
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexander Kornienko (alexfh)

<details>
Clang crashes when reading AST with constexpr structs containing unions. A test case reduced from real-world code:
```
$ split-file - q
//--- a.cppmap
module "a" {
 header "a.h"
}
//--- a.h
#include "b.h"
//--- b.h
#ifndef _B_H_
#define _B_H_
struct B {
  consteval B() {}
  union {
    int a;
  };
};
constexpr B b;
#endif
//--- c.cppmap
module "c" {
header "c.h"
}
//--- c.h
#include "b.h"
^D
$ cd q
$ $CLANG -iquote . -fmodule-name=c -Xclang=-fno-cxx-modules -Xclang=-fmodule-map-file-home-is-cwd -xc++ -Xclang=-emit-module -fmodules -fno-implicit-modules -fno-implicit-module-maps -std=c++20 -c c.cppmap -o c.pcm &amp;&amp; $CLANG -iquote . -fmodule-name=a -fmodule-map-file=a.cppmap -Xclang=-fno-cxx-modules -Xclang=-fmodule-map-file-home-is-cwd -xc++ -Xclang=-emit-module -fmodules -fno-implicit-modules -fno-implicit-module-maps -Xclang=-fmodule-file=c.pcm -std=c++20 -c a.cppmap -o a.pcm
assert.h assertion failed at llvm/include/llvm/Support/Casting.h:109 in static bool llvm::isa_impl_cl&lt;clang::FieldDecl, const clang::Decl *&gt;::doit(const From *) [To = clang::FieldDecl, From = const clang::Decl *]: Val &amp;&amp; "isa&lt;&gt; used on a null pointer"
    @     0x55c79770988a  __assert_fail
    @     0x55c7878e6fb0  llvm::isa_impl_cl&lt;&gt;::doit()
    @     0x55c7878e6f38  llvm::isa_impl_wrap&lt;&gt;::doit()
    @     0x55c7878e6f15  llvm::isa_impl_wrap&lt;&gt;::doit()
    @     0x55c7878e6e88  llvm::CastIsPossible&lt;&gt;::isPossible()
    @     0x55c7878e6e55  llvm::CastInfo&lt;&gt;::isPossible()
    @     0x55c7878e6db8  llvm::isa&lt;&gt;()
    @     0x55c7878e7230  llvm::cast&lt;&gt;()
    @     0x55c78a88b77c  clang::serialization::BasicReaderBase&lt;&gt;::readAPValue()
    @     0x55c78a891260  clang::serialization::ReadDispatcher&lt;&gt;::read&lt;&gt;()
    @     0x55c78a88f2bd  clang::serialization::DataStreamBasicReader&lt;&gt;::readArray&lt;&gt;()
    @     0x55c78a88b399  clang::serialization::BasicReaderBase&lt;&gt;::readAPValue()
    @     0x55c78a9eaaed  clang::ASTStmtReader::VisitConstantExpr()
    @     0x55c78aa186f4  clang::StmtVisitorBase&lt;&gt;::Visit()
    @     0x55c78aa0a095  clang::ASTReader::ReadStmtFromStream()
    @     0x55c78a7e4a2f  clang::ASTReader::GetExternalDeclStmt()
    @     0x55c78a7e4ac1  clang::ASTReader::GetExternalDeclStmt()
    @     0x55c78d7583bf  clang::LazyOffsetPtr&lt;&gt;::get()
    @     0x55c78d742662  clang::VarDecl::getInit()
    @     0x55c78a982e92  clang::ASTReader::PassInterestingDeclsToConsumer()
    @     0x55c78a7f678e  clang::ASTReader::FinishedDeserializing()
    @     0x55c78a7f67b9  clang::ASTReader::FinishedDeserializing()
    @     0x55c78a811032  clang::ExternalASTSource::Deserializing::~Deserializing()
    @     0x55c78a7e6cdc  clang::ASTReader::FindExternalVisibleDeclsByName()
    @     0x55c78a7e6d41  clang::ASTReader::FindExternalVisibleDeclsByName()
    @     0x55c78d77c4fc  clang::DeclContext::lookupImpl()
    @     0x55c78d77c0d3  clang::DeclContext::lookup()
    @     0x55c78b9bba58  LookupDirect()
    @     0x55c78b9b72d5  CppNamespaceLookup()
    @     0x55c78b9b6656  clang::Sema::CppLookupName()
    @     0x55c78b9bb787  clang::Sema::LookupName()
    @     0x55c78b137aae  clang::Sema::HandleDeclarator()
    @     0x55c78b136c99  clang::Sema::ActOnDeclarator()
    @     0x55c78a6959da  clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes()
    @     0x55c78a694413  clang::Parser::ParseDeclGroup()
    @     0x55c78a5ab674  clang::Parser::ParseDeclOrFunctionDefInternal()
    @     0x55c78a5aa4b1  clang::Parser::ParseDeclarationOrFunctionDefinition()
    @     0x55c78a5a9a4c  clang::Parser::ParseExternalDeclaration()
    @     0x55c78a5a6ec1  clang::Parser::ParseTopLevelDecl()
    @     0x55c78a59f616  clang::ParseAST()
    @     0x55c789f41475  clang::ASTFrontendAction::ExecuteAction()
    @     0x55c789f40a2b  clang::FrontendAction::Execute()
    @     0x55c789deca4d  clang::CompilerInstance::ExecuteAction()
    @     0x55c78785dea5  clang::ExecuteCompilerInvocation()
    @     0x55c78782b668  cc1_main()
```
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Alexander Kornienko (alexfh)

<details>
Clang crashes when reading AST with constexpr structs containing unions. A test case reduced from real-world code:
```
$ split-file - q
//--- a.cppmap
module "a" {
 header "a.h"
}
//--- a.h
#include "b.h"
//--- b.h
#ifndef _B_H_
#define _B_H_
struct B {
  consteval B() {}
  union {
    int a;
  };
};
constexpr B b;
#endif
//--- c.cppmap
module "c" {
header "c.h"
}
//--- c.h
#include "b.h"
^D
$ cd q
$ $CLANG -iquote . -fmodule-name=c -Xclang=-fno-cxx-modules -Xclang=-fmodule-map-file-home-is-cwd -xc++ -Xclang=-emit-module -fmodules -fno-implicit-modules -fno-implicit-module-maps -std=c++20 -c c.cppmap -o c.pcm &amp;&amp; $CLANG -iquote . -fmodule-name=a -fmodule-map-file=a.cppmap -Xclang=-fno-cxx-modules -Xclang=-fmodule-map-file-home-is-cwd -xc++ -Xclang=-emit-module -fmodules -fno-implicit-modules -fno-implicit-module-maps -Xclang=-fmodule-file=c.pcm -std=c++20 -c a.cppmap -o a.pcm
assert.h assertion failed at llvm/include/llvm/Support/Casting.h:109 in static bool llvm::isa_impl_cl&lt;clang::FieldDecl, const clang::Decl *&gt;::doit(const From *) [To = clang::FieldDecl, From = const clang::Decl *]: Val &amp;&amp; "isa&lt;&gt; used on a null pointer"
    @     0x55c79770988a  __assert_fail
    @     0x55c7878e6fb0  llvm::isa_impl_cl&lt;&gt;::doit()
    @     0x55c7878e6f38  llvm::isa_impl_wrap&lt;&gt;::doit()
    @     0x55c7878e6f15  llvm::isa_impl_wrap&lt;&gt;::doit()
    @     0x55c7878e6e88  llvm::CastIsPossible&lt;&gt;::isPossible()
    @     0x55c7878e6e55  llvm::CastInfo&lt;&gt;::isPossible()
    @     0x55c7878e6db8  llvm::isa&lt;&gt;()
    @     0x55c7878e7230  llvm::cast&lt;&gt;()
    @     0x55c78a88b77c  clang::serialization::BasicReaderBase&lt;&gt;::readAPValue()
    @     0x55c78a891260  clang::serialization::ReadDispatcher&lt;&gt;::read&lt;&gt;()
    @     0x55c78a88f2bd  clang::serialization::DataStreamBasicReader&lt;&gt;::readArray&lt;&gt;()
    @     0x55c78a88b399  clang::serialization::BasicReaderBase&lt;&gt;::readAPValue()
    @     0x55c78a9eaaed  clang::ASTStmtReader::VisitConstantExpr()
    @     0x55c78aa186f4  clang::StmtVisitorBase&lt;&gt;::Visit()
    @     0x55c78aa0a095  clang::ASTReader::ReadStmtFromStream()
    @     0x55c78a7e4a2f  clang::ASTReader::GetExternalDeclStmt()
    @     0x55c78a7e4ac1  clang::ASTReader::GetExternalDeclStmt()
    @     0x55c78d7583bf  clang::LazyOffsetPtr&lt;&gt;::get()
    @     0x55c78d742662  clang::VarDecl::getInit()
    @     0x55c78a982e92  clang::ASTReader::PassInterestingDeclsToConsumer()
    @     0x55c78a7f678e  clang::ASTReader::FinishedDeserializing()
    @     0x55c78a7f67b9  clang::ASTReader::FinishedDeserializing()
    @     0x55c78a811032  clang::ExternalASTSource::Deserializing::~Deserializing()
    @     0x55c78a7e6cdc  clang::ASTReader::FindExternalVisibleDeclsByName()
    @     0x55c78a7e6d41  clang::ASTReader::FindExternalVisibleDeclsByName()
    @     0x55c78d77c4fc  clang::DeclContext::lookupImpl()
    @     0x55c78d77c0d3  clang::DeclContext::lookup()
    @     0x55c78b9bba58  LookupDirect()
    @     0x55c78b9b72d5  CppNamespaceLookup()
    @     0x55c78b9b6656  clang::Sema::CppLookupName()
    @     0x55c78b9bb787  clang::Sema::LookupName()
    @     0x55c78b137aae  clang::Sema::HandleDeclarator()
    @     0x55c78b136c99  clang::Sema::ActOnDeclarator()
    @     0x55c78a6959da  clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes()
    @     0x55c78a694413  clang::Parser::ParseDeclGroup()
    @     0x55c78a5ab674  clang::Parser::ParseDeclOrFunctionDefInternal()
    @     0x55c78a5aa4b1  clang::Parser::ParseDeclarationOrFunctionDefinition()
    @     0x55c78a5a9a4c  clang::Parser::ParseExternalDeclaration()
    @     0x55c78a5a6ec1  clang::Parser::ParseTopLevelDecl()
    @     0x55c78a59f616  clang::ParseAST()
    @     0x55c789f41475  clang::ASTFrontendAction::ExecuteAction()
    @     0x55c789f40a2b  clang::FrontendAction::Execute()
    @     0x55c789deca4d  clang::CompilerInstance::ExecuteAction()
    @     0x55c78785dea5  clang::ExecuteCompilerInvocation()
    @     0x55c78782b668  cc1_main()
```
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-c-20

Author: Alexander Kornienko (alexfh)

<details>
Clang crashes when reading AST with constexpr structs containing unions. A test case reduced from real-world code:
```
$ split-file - q
//--- a.cppmap
module "a" {
 header "a.h"
}
//--- a.h
#include "b.h"
//--- b.h
#ifndef _B_H_
#define _B_H_
struct B {
  consteval B() {}
  union {
    int a;
  };
};
constexpr B b;
#endif
//--- c.cppmap
module "c" {
header "c.h"
}
//--- c.h
#include "b.h"
^D
$ cd q
$ $CLANG -iquote . -fmodule-name=c -Xclang=-fno-cxx-modules -Xclang=-fmodule-map-file-home-is-cwd -xc++ -Xclang=-emit-module -fmodules -fno-implicit-modules -fno-implicit-module-maps -std=c++20 -c c.cppmap -o c.pcm &amp;&amp; $CLANG -iquote . -fmodule-name=a -fmodule-map-file=a.cppmap -Xclang=-fno-cxx-modules -Xclang=-fmodule-map-file-home-is-cwd -xc++ -Xclang=-emit-module -fmodules -fno-implicit-modules -fno-implicit-module-maps -Xclang=-fmodule-file=c.pcm -std=c++20 -c a.cppmap -o a.pcm
assert.h assertion failed at llvm/include/llvm/Support/Casting.h:109 in static bool llvm::isa_impl_cl&lt;clang::FieldDecl, const clang::Decl *&gt;::doit(const From *) [To = clang::FieldDecl, From = const clang::Decl *]: Val &amp;&amp; "isa&lt;&gt; used on a null pointer"
    @     0x55c79770988a  __assert_fail
    @     0x55c7878e6fb0  llvm::isa_impl_cl&lt;&gt;::doit()
    @     0x55c7878e6f38  llvm::isa_impl_wrap&lt;&gt;::doit()
    @     0x55c7878e6f15  llvm::isa_impl_wrap&lt;&gt;::doit()
    @     0x55c7878e6e88  llvm::CastIsPossible&lt;&gt;::isPossible()
    @     0x55c7878e6e55  llvm::CastInfo&lt;&gt;::isPossible()
    @     0x55c7878e6db8  llvm::isa&lt;&gt;()
    @     0x55c7878e7230  llvm::cast&lt;&gt;()
    @     0x55c78a88b77c  clang::serialization::BasicReaderBase&lt;&gt;::readAPValue()
    @     0x55c78a891260  clang::serialization::ReadDispatcher&lt;&gt;::read&lt;&gt;()
    @     0x55c78a88f2bd  clang::serialization::DataStreamBasicReader&lt;&gt;::readArray&lt;&gt;()
    @     0x55c78a88b399  clang::serialization::BasicReaderBase&lt;&gt;::readAPValue()
    @     0x55c78a9eaaed  clang::ASTStmtReader::VisitConstantExpr()
    @     0x55c78aa186f4  clang::StmtVisitorBase&lt;&gt;::Visit()
    @     0x55c78aa0a095  clang::ASTReader::ReadStmtFromStream()
    @     0x55c78a7e4a2f  clang::ASTReader::GetExternalDeclStmt()
    @     0x55c78a7e4ac1  clang::ASTReader::GetExternalDeclStmt()
    @     0x55c78d7583bf  clang::LazyOffsetPtr&lt;&gt;::get()
    @     0x55c78d742662  clang::VarDecl::getInit()
    @     0x55c78a982e92  clang::ASTReader::PassInterestingDeclsToConsumer()
    @     0x55c78a7f678e  clang::ASTReader::FinishedDeserializing()
    @     0x55c78a7f67b9  clang::ASTReader::FinishedDeserializing()
    @     0x55c78a811032  clang::ExternalASTSource::Deserializing::~Deserializing()
    @     0x55c78a7e6cdc  clang::ASTReader::FindExternalVisibleDeclsByName()
    @     0x55c78a7e6d41  clang::ASTReader::FindExternalVisibleDeclsByName()
    @     0x55c78d77c4fc  clang::DeclContext::lookupImpl()
    @     0x55c78d77c0d3  clang::DeclContext::lookup()
    @     0x55c78b9bba58  LookupDirect()
    @     0x55c78b9b72d5  CppNamespaceLookup()
    @     0x55c78b9b6656  clang::Sema::CppLookupName()
    @     0x55c78b9bb787  clang::Sema::LookupName()
    @     0x55c78b137aae  clang::Sema::HandleDeclarator()
    @     0x55c78b136c99  clang::Sema::ActOnDeclarator()
    @     0x55c78a6959da  clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes()
    @     0x55c78a694413  clang::Parser::ParseDeclGroup()
    @     0x55c78a5ab674  clang::Parser::ParseDeclOrFunctionDefInternal()
    @     0x55c78a5aa4b1  clang::Parser::ParseDeclarationOrFunctionDefinition()
    @     0x55c78a5a9a4c  clang::Parser::ParseExternalDeclaration()
    @     0x55c78a5a6ec1  clang::Parser::ParseTopLevelDecl()
    @     0x55c78a59f616  clang::ParseAST()
    @     0x55c789f41475  clang::ASTFrontendAction::ExecuteAction()
    @     0x55c789f40a2b  clang::FrontendAction::Execute()
    @     0x55c789deca4d  clang::CompilerInstance::ExecuteAction()
    @     0x55c78785dea5  clang::ExecuteCompilerInvocation()
    @     0x55c78782b668  cc1_main()
```
</details>


---

