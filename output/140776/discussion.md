# Clang assertion failed in bool clang::Sema::DeduceVariableDeclarationType: VDecl->isLinkageValid()

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/140776
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-valid
**Closed Date:** 2025-05-20T21:10:15Z

## Body

Clang assertion failed at clang/lib/Sema/SemaDecl.cpp:13168 in bool clang::Sema::DeduceVariableDeclarationType(VarDecl *, bool, Expr *): VDecl->isLinkageValid()

Reduced test case:
```
namespace ns {
auto _p = [] {};
}
```
https://gcc.godbolt.org/z/PGdrfbvaM

Stack trace:
```
    @     0x55e82831bd07  clang::Sema::DeduceVariableDeclarationType()
    @     0x55e82831c80e  clang::Sema::AddInitializerToDecl()
    @     0x55e827ee7bd1  clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes()
    @     0x55e827ee5297  clang::Parser::ParseDeclGroup()
    @     0x55e827e7c1eb  clang::Parser::ParseDeclOrFunctionDefInternal()
    @     0x55e827e7b9ea  clang::Parser::ParseDeclarationOrFunctionDefinition()
    @     0x55e827e7a87a  clang::Parser::ParseExternalDeclaration()
    @     0x55e827ebde73  clang::Parser::ParseInnerNamespace()
    @     0x55e827ebd59d  clang::Parser::ParseNamespace()
    @     0x55e827ee3b64  clang::Parser::ParseDeclaration()
    @     0x55e827e7a456  clang::Parser::ParseExternalDeclaration()
    @     0x55e827ebde73  clang::Parser::ParseInnerNamespace()
    @     0x55e827ebd59d  clang::Parser::ParseNamespace()
    @     0x55e827ee3b64  clang::Parser::ParseDeclaration()
    @     0x55e827e7a456  clang::Parser::ParseExternalDeclaration()
    @     0x55e827e78ef4  clang::Parser::ParseTopLevelDecl()
    @     0x55e827e74b2e  clang::ParseAST()
    @     0x55e827bae1aa  clang::FrontendAction::Execute()
    @     0x55e827b271ad  clang::CompilerInstance::ExecuteAction()
    @     0x55e826f58b2b  clang::ExecuteCompilerInvocation()
    @     0x55e826f4c6c6  cc1_main()
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexander Kornienko (alexfh)

<details>
Clang assertion failed at clang/lib/Sema/SemaDecl.cpp:13168 in bool clang::Sema::DeduceVariableDeclarationType(VarDecl *, bool, Expr *): VDecl-&gt;isLinkageValid()

Reduced test case:
```
namespace ns {
auto _p = [] {};
}
```
https://gcc.godbolt.org/z/PGdrfbvaM

Stack trace:
```
    @     0x55e82831bd07  clang::Sema::DeduceVariableDeclarationType()
    @     0x55e82831c80e  clang::Sema::AddInitializerToDecl()
    @     0x55e827ee7bd1  clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes()
    @     0x55e827ee5297  clang::Parser::ParseDeclGroup()
    @     0x55e827e7c1eb  clang::Parser::ParseDeclOrFunctionDefInternal()
    @     0x55e827e7b9ea  clang::Parser::ParseDeclarationOrFunctionDefinition()
    @     0x55e827e7a87a  clang::Parser::ParseExternalDeclaration()
    @     0x55e827ebde73  clang::Parser::ParseInnerNamespace()
    @     0x55e827ebd59d  clang::Parser::ParseNamespace()
    @     0x55e827ee3b64  clang::Parser::ParseDeclaration()
    @     0x55e827e7a456  clang::Parser::ParseExternalDeclaration()
    @     0x55e827ebde73  clang::Parser::ParseInnerNamespace()
    @     0x55e827ebd59d  clang::Parser::ParseNamespace()
    @     0x55e827ee3b64  clang::Parser::ParseDeclaration()
    @     0x55e827e7a456  clang::Parser::ParseExternalDeclaration()
    @     0x55e827e78ef4  clang::Parser::ParseTopLevelDecl()
    @     0x55e827e74b2e  clang::ParseAST()
    @     0x55e827bae1aa  clang::FrontendAction::Execute()
    @     0x55e827b271ad  clang::CompilerInstance::ExecuteAction()
    @     0x55e826f58b2b  clang::ExecuteCompilerInvocation()
    @     0x55e826f4c6c6  cc1_main()
```
</details>


---

### Comment 2 - alexfh

This first started manifesting in clang 14: https://gcc.godbolt.org/z/jxvjdfha7

---

### Comment 3 - shafik

There is some discussion in the duplicate 

---

