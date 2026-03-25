# Clang ICE on assertion "isa<To>(Val) && "cast<Ty>() argument of incompatible type" with templates using -std=c++20

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/138098

## Body

## Summary 

**Using -std=c++20 Flag**

When referencing a dependent static member function inside a templated class without proper qualification or parentheses (e.g., type_c<T> instead of type_c<T>() or this->type_c<T>()), Clang 21 crashes with an internal compiler error (SIGSEGV) during instantiation.
Instead of a diagnostic message about the misuse, the compiler hits a segmentation fault in Sema::ResolveSingleFunctionTemplateSpecialization.

## Assertion

```
clang-21: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionTemplateDecl; From = clang::NamedDecl]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.      Program arguments: /root/llvm-project/build-assert/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/llvm-project/build-assert -fcoverage-compilation-dir=/root/llvm-project/build-assert -resource-dir /root/llvm-project/build-assert/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /root/llvm-project/build-assert/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++20 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-d0d770.o -x c++ prueba.cpp
1.      <eof> parser at end of file
2.      prueba.cpp:3:10: instantiating function definition 'Derived<int>::f'
 #0 0x0000557ed42a527e llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:22
 #1 0x0000557ed42a5694 PrintStackTraceSignalHandler(void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:1
 #2 0x0000557ed42a2bf2 llvm::sys::RunSignalHandlers() /root/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #3 0x0000557ed42a4b0a SignalHandler(int, siginfo_t*, void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:418:13
 #4 0x00007f4a3b959420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #5 0x00007f4a3b42600b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #6 0x00007f4a3b405859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #7 0x00007f4a3b405729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #8 0x00007f4a3b416fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #9 0x0000557ed789d5e9 decltype(auto) llvm::cast<clang::FunctionTemplateDecl, clang::NamedDecl>(clang::NamedDecl*) /root/llvm-project/llvm/include/llvm/Support/Casting.h:578:3
#10 0x0000557ed822727a clang::Sema::ResolveSingleFunctionTemplateSpecialization(clang::OverloadExpr*, bool, clang::DeclAccessPair*, clang::TemplateSpecCandidateSet*) /root/llvm-project/clang/lib/Sema/SemaOverload.cpp:13519:35
#11 0x0000557ed82276cc clang::Sema::ResolveAndFixSingleFunctionTemplateSpecialization(clang::ActionResult<clang::Expr*, true>&, bool, bool, clang::SourceRange, clang::QualType, unsigned int) /root/llvm-project/clang/lib/Sema/SemaOverload.cpp:13575:70
#12 0x0000557ed7c68d31 clang::Sema::CheckPlaceholderExpr(clang::Expr*) /root/llvm-project/clang/lib/Sema/SemaExpr.cpp:21036:58
#13 0x0000557ed7c4ee39 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) /root/llvm-project/clang/lib/Sema/SemaExpr.cpp:15453:58
#14 0x0000557ed8590b01 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::RebuildBinaryOperator(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:2981:3
#15 0x0000557ed856fe3f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:13406:65
#16 0x0000557ed8558308 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) /root/llvm-project/build-assert/tools/clang/include/clang/AST/StmtNodes.inc:798:1
#17 0x0000557ed859bb32 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCondition(clang::SourceLocation, clang::VarDecl*, clang::Expr*, clang::Sema::ConditionKind) /root/llvm-project/clang/lib/Sema/TreeTransform.h:4548:16
#18 0x0000557ed85864b3 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformIfStmt(clang::IfStmt*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:8237:5
#19 0x0000557ed855d608 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /root/llvm-project/build-assert/tools/clang/include/clang/AST/StmtNodes.inc:1604:1
#20 0x0000557ed858c522 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) /root/llvm-project/clang/lib/Sema/TreeTransform.h:8052:16
#21 0x0000557ed85883cf clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:8035:1
#22 0x0000557ed855d760 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) /root/llvm-project/build-assert/tools/clang/include/clang/AST/StmtNodes.inc:1652:1
#23 0x0000557ed85516c4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4493:38
#24 0x0000557ed864d097 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5345:45
#25 0x0000557ed8651d0c clang::Sema::PerformPendingInstantiations(bool) /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6585:32
#26 0x0000557ed7810ca9 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /root/llvm-project/clang/lib/Sema/Sema.cpp:1151:66
#27 0x0000557ed7811091 clang::Sema::ActOnEndOfTranslationUnit() /root/llvm-project/clang/lib/Sema/Sema.cpp:1191:9
#28 0x0000557ed763a533 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) /root/llvm-project/clang/lib/Parse/Parser.cpp:727:12
#29 0x0000557ed7635704 clang::ParseAST(clang::Sema&, bool, bool) /root/llvm-project/clang/lib/Parse/ParseAST.cpp:171:37
#30 0x0000557ed5564366 clang::ASTFrontendAction::ExecuteAction() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1186:11
#31 0x0000557ed5221958 clang::CodeGenAction::ExecuteAction() /root/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1102:5
#32 0x0000557ed5563c21 clang::FrontendAction::Execute() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:38
#33 0x0000557ed5474e8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /root/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:42
#34 0x0000557ed572f57d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /root/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:265:38
#35 0x0000557ecf7e38c4 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /root/llvm-project/clang/tools/driver/cc1_main.cpp:290:40
#36 0x0000557ecf7d440b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) /root/llvm-project/clang/tools/driver/driver.cpp:218:20
#37 0x0000557ecf7d4950 clang_main(int, char**, llvm::ToolContext const&) /root/llvm-project/clang/tools/driver/driver.cpp:259:26
#38 0x0000557ecf810950 main /root/llvm-project/build-assert/tools/clang/tools/driver/clang-driver.cpp:17:20
```

## Program

```
template<typename T>
struct Derived {
    void f(int index) {
        if (index != type_c<T>) {}
    }
    static auto& type_c() {
    }
};

int main() {
    Derived<int> d;
    d.f(0);
}
```

## To quickly reproduce

It happens only with -std=c++20 so it can be reproduced until clang-9.

https://gcc.godbolt.org/z/cETh6s8x7


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary 

**Using -std=c++23 Flag**

When referencing a dependent static member function inside a templated class without proper qualification or parentheses (e.g., type_c&lt;T&gt; instead of type_c&lt;T&gt;() or this-&gt;type_c&lt;T&gt;()), Clang 21 crashes with an internal compiler error (SIGSEGV) during instantiation.
Instead of a diagnostic message about the misuse, the compiler hits a segmentation fault in Sema::ResolveSingleFunctionTemplateSpecialization.

## Assertion

```
clang-21: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionTemplateDecl; From = clang::NamedDecl]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.      Program arguments: /root/llvm-project/build-assert/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/llvm-project/build-assert -fcoverage-compilation-dir=/root/llvm-project/build-assert -resource-dir /root/llvm-project/build-assert/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /root/llvm-project/build-assert/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-d0d770.o -x c++ prueba.cpp
1.      &lt;eof&gt; parser at end of file
2.      prueba.cpp:3:10: instantiating function definition 'Derived&lt;int&gt;::f'
 #<!-- -->0 0x0000557ed42a527e llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:22
 #<!-- -->1 0x0000557ed42a5694 PrintStackTraceSignalHandler(void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:1
 #<!-- -->2 0x0000557ed42a2bf2 llvm::sys::RunSignalHandlers() /root/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #<!-- -->3 0x0000557ed42a4b0a SignalHandler(int, siginfo_t*, void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:418:13
 #<!-- -->4 0x00007f4a3b959420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->5 0x00007f4a3b42600b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #<!-- -->6 0x00007f4a3b405859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #<!-- -->7 0x00007f4a3b405729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #<!-- -->8 0x00007f4a3b416fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #<!-- -->9 0x0000557ed789d5e9 decltype(auto) llvm::cast&lt;clang::FunctionTemplateDecl, clang::NamedDecl&gt;(clang::NamedDecl*) /root/llvm-project/llvm/include/llvm/Support/Casting.h:578:3
#<!-- -->10 0x0000557ed822727a clang::Sema::ResolveSingleFunctionTemplateSpecialization(clang::OverloadExpr*, bool, clang::DeclAccessPair*, clang::TemplateSpecCandidateSet*) /root/llvm-project/clang/lib/Sema/SemaOverload.cpp:13519:35
#<!-- -->11 0x0000557ed82276cc clang::Sema::ResolveAndFixSingleFunctionTemplateSpecialization(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, bool, bool, clang::SourceRange, clang::QualType, unsigned int) /root/llvm-project/clang/lib/Sema/SemaOverload.cpp:13575:70
#<!-- -->12 0x0000557ed7c68d31 clang::Sema::CheckPlaceholderExpr(clang::Expr*) /root/llvm-project/clang/lib/Sema/SemaExpr.cpp:21036:58
#<!-- -->13 0x0000557ed7c4ee39 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) /root/llvm-project/clang/lib/Sema/SemaExpr.cpp:15453:58
#<!-- -->14 0x0000557ed8590b01 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildBinaryOperator(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:2981:3
#<!-- -->15 0x0000557ed856fe3f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformBinaryOperator(clang::BinaryOperator*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:13406:65
#<!-- -->16 0x0000557ed8558308 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /root/llvm-project/build-assert/tools/clang/include/clang/AST/StmtNodes.inc:798:1
#<!-- -->17 0x0000557ed859bb32 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCondition(clang::SourceLocation, clang::VarDecl*, clang::Expr*, clang::Sema::ConditionKind) /root/llvm-project/clang/lib/Sema/TreeTransform.h:4548:16
#<!-- -->18 0x0000557ed85864b3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformIfStmt(clang::IfStmt*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:8237:5
#<!-- -->19 0x0000557ed855d608 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) /root/llvm-project/build-assert/tools/clang/include/clang/AST/StmtNodes.inc:1604:1
#<!-- -->20 0x0000557ed858c522 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) /root/llvm-project/clang/lib/Sema/TreeTransform.h:8052:16
#<!-- -->21 0x0000557ed85883cf clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:8035:1
#<!-- -->22 0x0000557ed855d760 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) /root/llvm-project/build-assert/tools/clang/include/clang/AST/StmtNodes.inc:1652:1
#<!-- -->23 0x0000557ed85516c4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4493:38
#<!-- -->24 0x0000557ed864d097 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5345:45
#<!-- -->25 0x0000557ed8651d0c clang::Sema::PerformPendingInstantiations(bool) /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6585:32
#<!-- -->26 0x0000557ed7810ca9 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /root/llvm-project/clang/lib/Sema/Sema.cpp:1151:66
#<!-- -->27 0x0000557ed7811091 clang::Sema::ActOnEndOfTranslationUnit() /root/llvm-project/clang/lib/Sema/Sema.cpp:1191:9
#<!-- -->28 0x0000557ed763a533 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /root/llvm-project/clang/lib/Parse/Parser.cpp:727:12
#<!-- -->29 0x0000557ed7635704 clang::ParseAST(clang::Sema&amp;, bool, bool) /root/llvm-project/clang/lib/Parse/ParseAST.cpp:171:37
#<!-- -->30 0x0000557ed5564366 clang::ASTFrontendAction::ExecuteAction() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1186:11
#<!-- -->31 0x0000557ed5221958 clang::CodeGenAction::ExecuteAction() /root/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1102:5
#<!-- -->32 0x0000557ed5563c21 clang::FrontendAction::Execute() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:38
#<!-- -->33 0x0000557ed5474e8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /root/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:42
#<!-- -->34 0x0000557ed572f57d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /root/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:265:38
#<!-- -->35 0x0000557ecf7e38c4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /root/llvm-project/clang/tools/driver/cc1_main.cpp:290:40
#<!-- -->36 0x0000557ecf7d440b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /root/llvm-project/clang/tools/driver/driver.cpp:218:20
#<!-- -->37 0x0000557ecf7d4950 clang_main(int, char**, llvm::ToolContext const&amp;) /root/llvm-project/clang/tools/driver/driver.cpp:259:26
#<!-- -->38 0x0000557ecf810950 main /root/llvm-project/build-assert/tools/clang/tools/driver/clang-driver.cpp:17:20
```

## Program

```
template&lt;typename T&gt;
struct Derived {
    void f(int index) {
        if (index != type_c&lt;T&gt;) {}
    }
    static auto&amp; type_c() {
    }
};

int main() {
    Derived&lt;int&gt; d;
    d.f(0);
}
```

## To quickly reproduce

It happens only with -std=c++23 so it can be reproduced until clang-17.

https://gcc.godbolt.org/z/cETh6s8x7

</details>


---

### Comment 2 - shafik

This reproduces w/ C++20 as well and goes back to clang-9: https://gcc.godbolt.org/z/649ocWbGM

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-c-20

Author: None (mariete1223)

<details>
## Summary 

**Using -std=c++23 Flag**

When referencing a dependent static member function inside a templated class without proper qualification or parentheses (e.g., type_c&lt;T&gt; instead of type_c&lt;T&gt;() or this-&gt;type_c&lt;T&gt;()), Clang 21 crashes with an internal compiler error (SIGSEGV) during instantiation.
Instead of a diagnostic message about the misuse, the compiler hits a segmentation fault in Sema::ResolveSingleFunctionTemplateSpecialization.

## Assertion

```
clang-21: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionTemplateDecl; From = clang::NamedDecl]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
```

## Stack dump

```
Stack dump:
0.      Program arguments: /root/llvm-project/build-assert/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root/llvm-project/build-assert -fcoverage-compilation-dir=/root/llvm-project/build-assert -resource-dir /root/llvm-project/build-assert/lib/clang/21 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/x86_64-linux-gnu/c++/9 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../include/c++/9/backward -internal-isystem /root/llvm-project/build-assert/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-d0d770.o -x c++ prueba.cpp
1.      &lt;eof&gt; parser at end of file
2.      prueba.cpp:3:10: instantiating function definition 'Derived&lt;int&gt;::f'
 #<!-- -->0 0x0000557ed42a527e llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:804:22
 #<!-- -->1 0x0000557ed42a5694 PrintStackTraceSignalHandler(void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:880:1
 #<!-- -->2 0x0000557ed42a2bf2 llvm::sys::RunSignalHandlers() /root/llvm-project/llvm/lib/Support/Signals.cpp:105:20
 #<!-- -->3 0x0000557ed42a4b0a SignalHandler(int, siginfo_t*, void*) /root/llvm-project/llvm/lib/Support/Unix/Signals.inc:418:13
 #<!-- -->4 0x00007f4a3b959420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->5 0x00007f4a3b42600b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #<!-- -->6 0x00007f4a3b405859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #<!-- -->7 0x00007f4a3b405729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #<!-- -->8 0x00007f4a3b416fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #<!-- -->9 0x0000557ed789d5e9 decltype(auto) llvm::cast&lt;clang::FunctionTemplateDecl, clang::NamedDecl&gt;(clang::NamedDecl*) /root/llvm-project/llvm/include/llvm/Support/Casting.h:578:3
#<!-- -->10 0x0000557ed822727a clang::Sema::ResolveSingleFunctionTemplateSpecialization(clang::OverloadExpr*, bool, clang::DeclAccessPair*, clang::TemplateSpecCandidateSet*) /root/llvm-project/clang/lib/Sema/SemaOverload.cpp:13519:35
#<!-- -->11 0x0000557ed82276cc clang::Sema::ResolveAndFixSingleFunctionTemplateSpecialization(clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, bool, bool, clang::SourceRange, clang::QualType, unsigned int) /root/llvm-project/clang/lib/Sema/SemaOverload.cpp:13575:70
#<!-- -->12 0x0000557ed7c68d31 clang::Sema::CheckPlaceholderExpr(clang::Expr*) /root/llvm-project/clang/lib/Sema/SemaExpr.cpp:21036:58
#<!-- -->13 0x0000557ed7c4ee39 clang::Sema::BuildBinOp(clang::Scope*, clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) /root/llvm-project/clang/lib/Sema/SemaExpr.cpp:15453:58
#<!-- -->14 0x0000557ed8590b01 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::RebuildBinaryOperator(clang::SourceLocation, clang::BinaryOperatorKind, clang::Expr*, clang::Expr*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:2981:3
#<!-- -->15 0x0000557ed856fe3f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformBinaryOperator(clang::BinaryOperator*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:13406:65
#<!-- -->16 0x0000557ed8558308 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) /root/llvm-project/build-assert/tools/clang/include/clang/AST/StmtNodes.inc:798:1
#<!-- -->17 0x0000557ed859bb32 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCondition(clang::SourceLocation, clang::VarDecl*, clang::Expr*, clang::Sema::ConditionKind) /root/llvm-project/clang/lib/Sema/TreeTransform.h:4548:16
#<!-- -->18 0x0000557ed85864b3 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformIfStmt(clang::IfStmt*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:8237:5
#<!-- -->19 0x0000557ed855d608 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) /root/llvm-project/build-assert/tools/clang/include/clang/AST/StmtNodes.inc:1604:1
#<!-- -->20 0x0000557ed858c522 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) /root/llvm-project/clang/lib/Sema/TreeTransform.h:8052:16
#<!-- -->21 0x0000557ed85883cf clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*) /root/llvm-project/clang/lib/Sema/TreeTransform.h:8035:1
#<!-- -->22 0x0000557ed855d760 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) /root/llvm-project/build-assert/tools/clang/include/clang/AST/StmtNodes.inc:1652:1
#<!-- -->23 0x0000557ed85516c4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiate.cpp:4493:38
#<!-- -->24 0x0000557ed864d097 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:5345:45
#<!-- -->25 0x0000557ed8651d0c clang::Sema::PerformPendingInstantiations(bool) /root/llvm-project/clang/lib/Sema/SemaTemplateInstantiateDecl.cpp:6585:32
#<!-- -->26 0x0000557ed7810ca9 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) /root/llvm-project/clang/lib/Sema/Sema.cpp:1151:66
#<!-- -->27 0x0000557ed7811091 clang::Sema::ActOnEndOfTranslationUnit() /root/llvm-project/clang/lib/Sema/Sema.cpp:1191:9
#<!-- -->28 0x0000557ed763a533 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) /root/llvm-project/clang/lib/Parse/Parser.cpp:727:12
#<!-- -->29 0x0000557ed7635704 clang::ParseAST(clang::Sema&amp;, bool, bool) /root/llvm-project/clang/lib/Parse/ParseAST.cpp:171:37
#<!-- -->30 0x0000557ed5564366 clang::ASTFrontendAction::ExecuteAction() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1186:11
#<!-- -->31 0x0000557ed5221958 clang::CodeGenAction::ExecuteAction() /root/llvm-project/clang/lib/CodeGen/CodeGenAction.cpp:1102:5
#<!-- -->32 0x0000557ed5563c21 clang::FrontendAction::Execute() /root/llvm-project/clang/lib/Frontend/FrontendAction.cpp:1076:38
#<!-- -->33 0x0000557ed5474e8b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) /root/llvm-project/clang/lib/Frontend/CompilerInstance.cpp:1056:42
#<!-- -->34 0x0000557ed572f57d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /root/llvm-project/clang/lib/FrontendTool/ExecuteCompilerInvocation.cpp:265:38
#<!-- -->35 0x0000557ecf7e38c4 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) /root/llvm-project/clang/tools/driver/cc1_main.cpp:290:40
#<!-- -->36 0x0000557ecf7d440b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) /root/llvm-project/clang/tools/driver/driver.cpp:218:20
#<!-- -->37 0x0000557ecf7d4950 clang_main(int, char**, llvm::ToolContext const&amp;) /root/llvm-project/clang/tools/driver/driver.cpp:259:26
#<!-- -->38 0x0000557ecf810950 main /root/llvm-project/build-assert/tools/clang/tools/driver/clang-driver.cpp:17:20
```

## Program

```
template&lt;typename T&gt;
struct Derived {
    void f(int index) {
        if (index != type_c&lt;T&gt;) {}
    }
    static auto&amp; type_c() {
    }
};

int main() {
    Derived&lt;int&gt; d;
    d.f(0);
}
```

## To quickly reproduce

It happens only with -std=c++23 so it can be reproduced until clang-17.

https://gcc.godbolt.org/z/cETh6s8x7

</details>


---

