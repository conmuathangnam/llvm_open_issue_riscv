# clang++ crashes with Assertion '(!isNull())' failed on invalid template instantiation with static_cast since clang 17 with -std=c++20

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/142746
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-06-04T09:37:35Z

## Body

Have not being able to remove iostream library due to limited time, it will be tried again for later submissions.

## Assertion

```
clang/include/clang/AST/Type.h:945: Assertion `!isNull() && "Cannot retrieve a NULL type pointer"` failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 <source>
1.	<source>:11:14: current parser token ')'
2.	<source>:10:1: parsing function body 'main'
3.	<source>:10:1: in compound statement ('{}')
4.	<source>:2:6: instantiating function definition 'my_func<int>'
 #0 0x0000000003e64af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e64af8)
 #1 0x0000000003e627b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e627b4)
 #2 0x0000000003daec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007699be442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007699be4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007699be442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007699be4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007699be42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007699be439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006a656fd clang::Sema::BuildCXXNamedCast(clang::SourceLocation, clang::tok::TokenKind, clang::TypeSourceInfo*, clang::Expr*, clang::SourceRange, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6a656fd)
#10 0x00000000073c07d2 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXNamedCastExpr(clang::CXXNamedCastExpr*) SemaTemplateInstantiate.cpp:0:0
#11 0x00000000073a8506 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#12 0x00000000073ab6f5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#13 0x00000000073bb9f0 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#14 0x00000000073a8238 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#15 0x00000000073b6cf8 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformParenExpr(clang::ParenExpr*) SemaTemplateInstantiate.cpp:0:0
#16 0x00000000073a8866 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#17 0x00000000073ab6f5 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#18 0x00000000073abf2b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x73abf2b)
#19 0x0000000007406db6 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7406db6)
#20 0x0000000007453760 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&, llvm::SmallVector<clang::Sema::LateInstantiatedAttribute, 16u>*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7453760)
#21 0x000000000745e68b clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef<clang::BindingDecl*>*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x745e68b)
#22 0x0000000007463974 void llvm::function_ref<void ()>::callback_fn<clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&)::'lambda'()>(long) SemaTemplateInstantiateDecl.cpp:0:0
#23 0x000000000804d981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x804d981)
#24 0x0000000007404c4a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7404c4a)
#25 0x00000000073755ad clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#26 0x00000000073d6cbe clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#27 0x00000000073dfaa4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x73dfaa4)
#28 0x00000000074578e2 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x74578e2)
#29 0x000000000804d981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x804d981)
#30 0x00000000072b8e2a clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x72b8e2a)
#31 0x0000000006d2c215 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d2c215)
#32 0x000000000713b0a4 FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#33 0x000000000713bf6c clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x713bf6c)
#34 0x0000000006d746c6 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d746c6)
#35 0x0000000006d7553c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d7553c)
#36 0x00000000068c1cb2 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c1cb2)
#37 0x00000000068ba491 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68ba491)
#38 0x00000000068bc5f7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc5f7)
#39 0x00000000068bc689 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc689)
#40 0x00000000068c11f9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c11f9)
#41 0x0000000006945929 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6945929)
#42 0x000000000693bc9e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693bc9e)
#43 0x000000000693cbf0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 32u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693cbf0)
#44 0x000000000693dab5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693dab5)
#45 0x000000000693f2ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693f2ba)
#46 0x0000000006850a53 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6850a53)
#47 0x00000000068856ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68856ad)
#48 0x00000000068447be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68447be)
#49 0x0000000006844f79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6844f79)
#50 0x000000000684c723 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684c723)
#51 0x000000000684d5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684d5fd)
#52 0x000000000683fa5a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x683fa5a)
#53 0x00000000047f7e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x47f7e58)
#54 0x0000000004abdd25 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4abdd25)
#55 0x0000000004a407de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4a407de)
#56 0x0000000004babd8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4babd8e)
#57 0x0000000000cfe40f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfe40f)
#58 0x0000000000cf5e9a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#59 0x000000000483e749 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#60 0x0000000003daf134 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3daf134)
#61 0x000000000483ed3f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#62 0x0000000004801bad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4801bad)
#63 0x0000000004802c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4802c2e)
#64 0x000000000480a765 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x480a765)
#65 0x0000000000cfb213 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfb213)
#66 0x0000000000bc3694 main (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xbc3694)
#67 0x00007699be429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#68 0x00007699be429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#69 0x0000000000cf5945 _start (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcf5945)
```

## Program

```
#include <iostream>
auto my_func(auto x) 
{

    auto result{x};
    auto result2 = (result - static_cast<tm>(0.5));

}
int main()
{
    my_func(1);
}
```

## To quickly reproduce

https://godbolt.org/z/donMh19rG

## Comments

### Comment 1 - Mr-Anyone

This seems to be fixed in trunk? 


```
vhe@vhe:~/llvm-project/build-release$ ./bin/clang -x c++ test.cpp -std=c++20
test.cpp:6:28: error: invalid operands to binary expression ('int' and 'tm')
    6 |     auto result2 = (result - static_cast<tm>(0.5));
      |                     ~~~~~~ ^ ~~~~~~~~~~~~~~~~~~~~
test.cpp:11:5: note: in instantiation of function template specialization 'my_func<int>' requested here
   11 |     my_func(1);
      |     ^
1 error generated.


vhe@vhe:~/llvm-project/build-release$ ./bin/clang --version
clang version 21.0.0git (https://github.com/llvm/llvm-project.git aef4373190d024dcb07d928be6e7bc0344433924)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/vhe/llvm-project/build-release/bin
Build config: +assertions
vhe@vhe:~/llvm-project/build-release$```


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
Have not being able to remove iostream library due to limited time, it will be tried again for later submissions.

## Assertion

```
clang/include/clang/AST/Type.h:945: Assertion `!isNull() &amp;&amp; "Cannot retrieve a NULL type pointer"` failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-20.1.0/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-14.2.0 -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:11:14: current parser token ')'
2.	&lt;source&gt;:10:1: parsing function body 'main'
3.	&lt;source&gt;:10:1: in compound statement ('{}')
4.	&lt;source&gt;:2:6: instantiating function definition 'my_func&lt;int&gt;'
 #<!-- -->0 0x0000000003e64af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e64af8)
 #<!-- -->1 0x0000000003e627b4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3e627b4)
 #<!-- -->2 0x0000000003daec88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007699be442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007699be4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007699be442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007699be4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007699be42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007699be439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006a656fd clang::Sema::BuildCXXNamedCast(clang::SourceLocation, clang::tok::TokenKind, clang::TypeSourceInfo*, clang::Expr*, clang::SourceRange, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6a656fd)
#<!-- -->10 0x00000000073c07d2 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXNamedCastExpr(clang::CXXNamedCastExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x00000000073a8506 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x00000000073ab6f5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->13 0x00000000073bb9f0 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformBinaryOperator(clang::BinaryOperator*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->14 0x00000000073a8238 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->15 0x00000000073b6cf8 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformParenExpr(clang::ParenExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->16 0x00000000073a8866 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->17 0x00000000073ab6f5 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x00000000073abf2b clang::Sema::SubstInitializer(clang::Expr*, clang::MultiLevelTemplateArgumentList const&amp;, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x73abf2b)
#<!-- -->19 0x0000000007406db6 clang::Sema::InstantiateVariableInitializer(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7406db6)
#<!-- -->20 0x0000000007453760 clang::Sema::BuildVariableInstantiation(clang::VarDecl*, clang::VarDecl*, clang::MultiLevelTemplateArgumentList const&amp;, llvm::SmallVector&lt;clang::Sema::LateInstantiatedAttribute, 16u&gt;*, clang::DeclContext*, clang::LocalInstantiationScope*, bool, clang::VarTemplateSpecializationDecl*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7453760)
#<!-- -->21 0x000000000745e68b clang::TemplateDeclInstantiator::VisitVarDecl(clang::VarDecl*, bool, llvm::ArrayRef&lt;clang::BindingDecl*&gt;*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x745e68b)
#<!-- -->22 0x0000000007463974 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;)::'lambda'()&gt;(long) SemaTemplateInstantiateDecl.cpp:0:0
#<!-- -->23 0x000000000804d981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x804d981)
#<!-- -->24 0x0000000007404c4a clang::Sema::SubstDecl(clang::Decl*, clang::DeclContext*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x7404c4a)
#<!-- -->25 0x00000000073755ad clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformDeclStmt(clang::DeclStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x00000000073d6cbe clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->27 0x00000000073dfaa4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x73dfaa4)
#<!-- -->28 0x00000000074578e2 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x74578e2)
#<!-- -->29 0x000000000804d981 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x804d981)
#<!-- -->30 0x00000000072b8e2a clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x72b8e2a)
#<!-- -->31 0x0000000006d2c215 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d2c215)
#<!-- -->32 0x000000000713b0a4 FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#<!-- -->33 0x000000000713bf6c clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x713bf6c)
#<!-- -->34 0x0000000006d746c6 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d746c6)
#<!-- -->35 0x0000000006d7553c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6d7553c)
#<!-- -->36 0x00000000068c1cb2 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c1cb2)
#<!-- -->37 0x00000000068ba491 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68ba491)
#<!-- -->38 0x00000000068bc5f7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc5f7)
#<!-- -->39 0x00000000068bc689 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68bc689)
#<!-- -->40 0x00000000068c11f9 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68c11f9)
#<!-- -->41 0x0000000006945929 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6945929)
#<!-- -->42 0x000000000693bc9e clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693bc9e)
#<!-- -->43 0x000000000693cbf0 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693cbf0)
#<!-- -->44 0x000000000693dab5 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693dab5)
#<!-- -->45 0x000000000693f2ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x693f2ba)
#<!-- -->46 0x0000000006850a53 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6850a53)
#<!-- -->47 0x00000000068856ad clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68856ad)
#<!-- -->48 0x00000000068447be clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x68447be)
#<!-- -->49 0x0000000006844f79 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x6844f79)
#<!-- -->50 0x000000000684c723 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684c723)
#<!-- -->51 0x000000000684d5fd clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x684d5fd)
#<!-- -->52 0x000000000683fa5a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x683fa5a)
#<!-- -->53 0x00000000047f7e58 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x47f7e58)
#<!-- -->54 0x0000000004abdd25 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4abdd25)
#<!-- -->55 0x0000000004a407de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4a407de)
#<!-- -->56 0x0000000004babd8e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4babd8e)
#<!-- -->57 0x0000000000cfe40f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfe40f)
#<!-- -->58 0x0000000000cf5e9a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->59 0x000000000483e749 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->60 0x0000000003daf134 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x3daf134)
#<!-- -->61 0x000000000483ed3f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->62 0x0000000004801bad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4801bad)
#<!-- -->63 0x0000000004802c2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x4802c2e)
#<!-- -->64 0x000000000480a765 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0x480a765)
#<!-- -->65 0x0000000000cfb213 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcfb213)
#<!-- -->66 0x0000000000bc3694 main (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xbc3694)
#<!-- -->67 0x00007699be429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->68 0x00007699be429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->69 0x0000000000cf5945 _start (/opt/compiler-explorer/clang-assertions-20.1.0/bin/clang+++0xcf5945)
```

## Program

```
#include &lt;iostream&gt;
auto my_func(auto x) 
{

    auto result{x};
    auto result2 = (result - static_cast&lt;tm&gt;(0.5));

}
int main()
{
    my_func(1);
}
```

## To quickly reproduce

https://godbolt.org/z/donMh19rG
</details>


---

