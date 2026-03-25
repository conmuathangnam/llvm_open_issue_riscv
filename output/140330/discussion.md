# Assertion failure in ThreadSafetyReporter::handleNoMutexHeld "Only works for variables"

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/140330
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash-on-valid
**Closed Date:** 2025-07-16T06:45:46Z

## Body

https://gcc.godbolt.org/z/W6MTKqsvz
```
$ cat test.c
struct {
  int v __attribute__((guarded_var));
} p;

int *g() {
  return &p.v;
}
$ ./clang-bad -Wthread-safety-analysis test.c
assertion failed at clang/lib/Sema/AnalysisBasedWarnings.cpp:1948 in virtual void clang::threadSafety::(anonymous namespace)::ThreadSafetyReporter::handleNoMutexHeld(const NamedDecl *, ProtectedOperationKind, AccessKind, SourceLocation): (POK == POK_VarAccess || POK == POK_VarDereference) && "Only works for variables"
    @     0x55c4b4397ec4  __assert_fail
    @     0x55c4af71b522  clang::threadSafety::(anonymous namespace)::ThreadSafetyReporter::handleNoMutexHeld()
    @     0x55c4b00ede2c  (anonymous namespace)::ThreadSafetyAnalyzer::checkAccess()
    @     0x55c4b00debaf  (anonymous namespace)::ThreadSafetyAnalyzer::runAnalysis()
    @     0x55c4b00daca4  clang::threadSafety::runThreadSafetyAnalysis()
    @     0x55c4af711fa2  clang::sema::AnalysisBasedWarnings::IssueWarnings()
    @     0x55c4af705eda  clang::Sema::PopFunctionScopeInfo()
    @     0x55c4af8b63be  clang::Sema::ActOnFinishFunctionBody()
    @     0x55c4af4beed4  clang::Parser::ParseFunctionStatementBody()
    @     0x55c4af41109c  clang::Parser::ParseFunctionDefinition()
    @     0x55c4af479264  clang::Parser::ParseDeclGroup()
    @     0x55c4af410066  clang::Parser::ParseDeclOrFunctionDefInternal()
    @     0x55c4af40f85d  clang::Parser::ParseDeclarationOrFunctionDefinition()
    @     0x55c4af40e6f9  clang::Parser::ParseExternalDeclaration()
    @     0x55c4af40cd74  clang::Parser::ParseTopLevelDecl()
    @     0x55c4af4089be  clang::ParseAST()
    @     0x55c4af153a8a  clang::FrontendAction::Execute()
    @     0x55c4af0cd98d  clang::CompilerInstance::ExecuteAction()
    @     0x55c4ae51a7ab  clang::ExecuteCompilerInvocation()
    @     0x55c4ae50e8a0  cc1_main()
    @     0x55c4ae50bc24  ExecuteCC1Tool()
    @     0x55c4ae50b180  clang_main()
    @     0x55c4ae509674  main
    @     0x7fa386b923d4  __libc_start_main
    @     0x55c4ae5095aa  _start
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexander Kornienko (alexfh)

<details>
https://gcc.godbolt.org/z/W6MTKqsvz
```
$ cat test.c
struct {
  int v __attribute__((guarded_var));
} p;

int *g() {
  return &amp;p.v;
}
$ ./clang-bad -Wthread-safety-analysis test.c
assertion failed at clang/lib/Sema/AnalysisBasedWarnings.cpp:1948 in virtual void clang::threadSafety::(anonymous namespace)::ThreadSafetyReporter::handleNoMutexHeld(const NamedDecl *, ProtectedOperationKind, AccessKind, SourceLocation): (POK == POK_VarAccess || POK == POK_VarDereference) &amp;&amp; "Only works for variables"
    @     0x55c4b4397ec4  __assert_fail
    @     0x55c4af71b522  clang::threadSafety::(anonymous namespace)::ThreadSafetyReporter::handleNoMutexHeld()
    @     0x55c4b00ede2c  (anonymous namespace)::ThreadSafetyAnalyzer::checkAccess()
    @     0x55c4b00debaf  (anonymous namespace)::ThreadSafetyAnalyzer::runAnalysis()
    @     0x55c4b00daca4  clang::threadSafety::runThreadSafetyAnalysis()
    @     0x55c4af711fa2  clang::sema::AnalysisBasedWarnings::IssueWarnings()
    @     0x55c4af705eda  clang::Sema::PopFunctionScopeInfo()
    @     0x55c4af8b63be  clang::Sema::ActOnFinishFunctionBody()
    @     0x55c4af4beed4  clang::Parser::ParseFunctionStatementBody()
    @     0x55c4af41109c  clang::Parser::ParseFunctionDefinition()
    @     0x55c4af479264  clang::Parser::ParseDeclGroup()
    @     0x55c4af410066  clang::Parser::ParseDeclOrFunctionDefInternal()
    @     0x55c4af40f85d  clang::Parser::ParseDeclarationOrFunctionDefinition()
    @     0x55c4af40e6f9  clang::Parser::ParseExternalDeclaration()
    @     0x55c4af40cd74  clang::Parser::ParseTopLevelDecl()
    @     0x55c4af4089be  clang::ParseAST()
    @     0x55c4af153a8a  clang::FrontendAction::Execute()
    @     0x55c4af0cd98d  clang::CompilerInstance::ExecuteAction()
    @     0x55c4ae51a7ab  clang::ExecuteCompilerInvocation()
    @     0x55c4ae50e8a0  cc1_main()
    @     0x55c4ae50bc24  ExecuteCC1Tool()
    @     0x55c4ae50b180  clang_main()
    @     0x55c4ae509674  main
    @     0x7fa386b923d4  __libc_start_main
    @     0x55c4ae5095aa  _start
```
</details>


---

### Comment 2 - shafik

is this generated via fuzzing?

---

### Comment 3 - alexfh

> is this generated via fuzzing?

No, it's reduced from real code.

---

### Comment 4 - alexfh

This appeared after Clang 20: https://gcc.godbolt.org/z/5WhnYnGPW

---

### Comment 5 - alexfh

This appeared between llvmorg-21-init-3070-ga98c2940db and llvmorg-21-init-3211-g0e3ba99ad6. There are two relevant commits in the range: 3c8c0d4d8d9bbc160d160e683f7a74fd28574dc6 and de10e44b6fe7f3d3cfde3afd8e1222d251172ade authored by @melver . Marco, could you take a look?

---

### Comment 6 - melver

Although `__attribute__((guarded_var))` is deprecated, we shouldn't crash on it. Sent a PR with the fix.

---

