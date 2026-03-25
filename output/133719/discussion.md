# [Clang][Regression:20] Crash on constraint for lambda in lambda with init-capture

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/133719
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, lambda, constexpr, regression:20
**Closed Date:** 2025-04-03T03:15:44Z

## Body

The following C++20 code causes a crash since Clang 20:

```c++
template <class T>
constexpr auto f{[] (auto arg) {
    return [a{arg}] {
        [] () requires true {}();
    };
}};

int main() {
    f<int>(0);
}
```

See https://compiler-explorer.com/z/Gh6soKaEE.

See also #128175.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaConcept.cpp:733:
bool clang::Sema::addInstantiatedCapturesToScope(clang::FunctionDecl*, const clang::FunctionDecl*, clang::LocalInstantiationScope&, const clang::MultiLevelTemplateArgumentList&):
Assertion `InstantiatingScope' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:9:13: current parser token ')'
2.	<source>:8:12: parsing function body 'main'
3.	<source>:8:12: in compound statement ('{}')
4.	<source>:2:18: instantiating function definition '(anonymous class)::operator()<int>'
 #0 0x0000000003ea56b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ea56b8)
 #1 0x0000000003ea3344 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ea3344)
 #2 0x0000000003de8488 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007a01e8642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007a01e86969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007a01e8642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007a01e86287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007a01e862871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007a01e8639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000067ad2ee clang::Sema::addInstantiatedCapturesToScope(clang::FunctionDecl*, clang::FunctionDecl const*, clang::LocalInstantiationScope&, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67ad2ee)
#10 0x0000000006be427f clang::Sema::LambdaScopeForCallOperatorInstantiationRAII::LambdaScopeForCallOperatorInstantiationRAII(clang::Sema&, clang::FunctionDecl*, clang::MultiLevelTemplateArgumentList, clang::LocalInstantiationScope&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be427f)
#11 0x00000000067ba8bf clang::Sema::CheckFunctionConstraints(clang::FunctionDecl const*, clang::ConstraintSatisfaction&, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67ba8bf)
#12 0x0000000006df3895 clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, bool, llvm::MutableArrayRef<clang::ImplicitConversionSequence>, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df3895)
#13 0x0000000006dfbea2 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef<clang::Expr*>, clang::OverloadCandidateSet&, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dfbea2)
#14 0x0000000006e13022 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e13022)
#15 0x0000000006a1c643 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c643)
#16 0x0000000006a1cf2c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1cf2c)
#17 0x0000000007088c48 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#18 0x0000000007077642 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#19 0x00000000070a5d87 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#20 0x00000000070a699e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#21 0x00000000070769d4 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#22 0x0000000007077d2e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#23 0x000000000707a865 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#24 0x000000000707ac0c clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#25 0x00000000070a699e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#26 0x00000000070af9d4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70af9d4)
#27 0x000000000712b29d clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x712b29d)
#28 0x0000000007db96d1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7db96d1)
#29 0x0000000006f8924a clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f8924a)
#30 0x00000000069d43c6 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d43c6)
#31 0x0000000006dd1ef7 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#32 0x0000000006e139aa clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e139aa)
#33 0x0000000006a1c643 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c643)
#34 0x0000000006a1cf2c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1cf2c)
#35 0x00000000065581f2 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65581f2)
#36 0x0000000006550b71 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6550b71)
#37 0x0000000006552cd7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552cd7)
#38 0x0000000006552d69 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552d69)
#39 0x0000000006557739 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6557739)
#40 0x00000000065db339 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65db339)
#41 0x00000000065d3052 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d3052)
#42 0x00000000065d3f7d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d3f7d)
#43 0x00000000065dba33 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65dba33)
#44 0x00000000065dc1fa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65dc1fa)
#45 0x00000000064e5c93 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e5c93)
#46 0x000000000651b22d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651b22d)
#47 0x00000000064d98ae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d98ae)
#48 0x00000000064da069 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64da069)
#49 0x00000000064e1933 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e1933)
#50 0x00000000064e284d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e284d)
#51 0x00000000064d4d9a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d4d9a)
#52 0x000000000483cac8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483cac8)
#53 0x0000000004b2b555 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2b555)
#54 0x0000000004aad19e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aad19e)
#55 0x0000000004c1bb1e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c1bb1e)
#56 0x0000000000d5dc0f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5dc0f)
#57 0x0000000000d553ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#58 0x00000000048a2469 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#59 0x0000000003de8924 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3de8924)
#60 0x00000000048a2a7f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#61 0x000000000486543d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486543d)
#62 0x00000000048664ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48664ce)
#63 0x000000000486dbd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486dbd5)
#64 0x0000000000d5aa08 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5aa08)
#65 0x0000000000c20d34 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc20d34)
#66 0x00007a01e8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#67 0x00007a01e8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#68 0x0000000000d54e75 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd54e75)
```

</details>


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
The following C++20 code causes a crash since Clang 20:

```c++
template &lt;class T&gt;
constexpr auto f{[] (auto arg) {
    return [a{arg}] {
        [] () requires true {}();
    };
}};

int main() {
    f&lt;int&gt;(0);
}
```

See https://compiler-explorer.com/z/Gh6soKaEE.

See also #<!-- -->128175.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaConcept.cpp:733:
bool clang::Sema::addInstantiatedCapturesToScope(clang::FunctionDecl*, const clang::FunctionDecl*, clang::LocalInstantiationScope&amp;, const clang::MultiLevelTemplateArgumentList&amp;):
Assertion `InstantiatingScope' failed.
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:9:13: current parser token ')'
2.	&lt;source&gt;:8:12: parsing function body 'main'
3.	&lt;source&gt;:8:12: in compound statement ('{}')
4.	&lt;source&gt;:2:18: instantiating function definition '(anonymous class)::operator()&lt;int&gt;'
 #<!-- -->0 0x0000000003ea56b8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ea56b8)
 #<!-- -->1 0x0000000003ea3344 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ea3344)
 #<!-- -->2 0x0000000003de8488 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007a01e8642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007a01e86969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007a01e8642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007a01e86287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007a01e862871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007a01e8639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000067ad2ee clang::Sema::addInstantiatedCapturesToScope(clang::FunctionDecl*, clang::FunctionDecl const*, clang::LocalInstantiationScope&amp;, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67ad2ee)
#<!-- -->10 0x0000000006be427f clang::Sema::LambdaScopeForCallOperatorInstantiationRAII::LambdaScopeForCallOperatorInstantiationRAII(clang::Sema&amp;, clang::FunctionDecl*, clang::MultiLevelTemplateArgumentList, clang::LocalInstantiationScope&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6be427f)
#<!-- -->11 0x00000000067ba8bf clang::Sema::CheckFunctionConstraints(clang::FunctionDecl const*, clang::ConstraintSatisfaction&amp;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67ba8bf)
#<!-- -->12 0x0000000006df3895 clang::Sema::AddMethodCandidate(clang::CXXMethodDecl*, clang::DeclAccessPair, clang::CXXRecordDecl*, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, bool, llvm::MutableArrayRef&lt;clang::ImplicitConversionSequence&gt;, clang::OverloadCandidateParamOrder, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6df3895)
#<!-- -->13 0x0000000006dfbea2 clang::Sema::AddMethodCandidate(clang::DeclAccessPair, clang::QualType, clang::Expr::Classification, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::OverloadCandidateSet&amp;, bool, clang::OverloadCandidateParamOrder) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6dfbea2)
#<!-- -->14 0x0000000006e13022 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e13022)
#<!-- -->15 0x0000000006a1c643 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c643)
#<!-- -->16 0x0000000006a1cf2c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1cf2c)
#<!-- -->17 0x0000000007088c48 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->18 0x0000000007077642 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->19 0x00000000070a5d87 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->20 0x00000000070a699e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->21 0x00000000070769d4 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformLambdaExpr(clang::LambdaExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->22 0x0000000007077d2e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->23 0x000000000707a865 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaTemplateInstantiate.cpp:0:0
#<!-- -->24 0x000000000707ac0c clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformReturnStmt(clang::ReturnStmt*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->25 0x00000000070a699e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->26 0x00000000070af9d4 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70af9d4)
#<!-- -->27 0x000000000712b29d clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x712b29d)
#<!-- -->28 0x0000000007db96d1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7db96d1)
#<!-- -->29 0x0000000006f8924a clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f8924a)
#<!-- -->30 0x00000000069d43c6 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69d43c6)
#<!-- -->31 0x0000000006dd1ef7 CreateFunctionRefExpr(clang::Sema&amp;, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&amp;) SemaOverload.cpp:0:0
#<!-- -->32 0x0000000006e139aa clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6e139aa)
#<!-- -->33 0x0000000006a1c643 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1c643)
#<!-- -->34 0x0000000006a1cf2c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1cf2c)
#<!-- -->35 0x00000000065581f2 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65581f2)
#<!-- -->36 0x0000000006550b71 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6550b71)
#<!-- -->37 0x0000000006552cd7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552cd7)
#<!-- -->38 0x0000000006552d69 clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6552d69)
#<!-- -->39 0x0000000006557739 clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6557739)
#<!-- -->40 0x00000000065db339 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65db339)
#<!-- -->41 0x00000000065d3052 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d3052)
#<!-- -->42 0x00000000065d3f7d clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d3f7d)
#<!-- -->43 0x00000000065dba33 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65dba33)
#<!-- -->44 0x00000000065dc1fa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65dc1fa)
#<!-- -->45 0x00000000064e5c93 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e5c93)
#<!-- -->46 0x000000000651b22d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x651b22d)
#<!-- -->47 0x00000000064d98ae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d98ae)
#<!-- -->48 0x00000000064da069 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64da069)
#<!-- -->49 0x00000000064e1933 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e1933)
#<!-- -->50 0x00000000064e284d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64e284d)
#<!-- -->51 0x00000000064d4d9a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d4d9a)
#<!-- -->52 0x000000000483cac8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x483cac8)
#<!-- -->53 0x0000000004b2b555 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2b555)
#<!-- -->54 0x0000000004aad19e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aad19e)
#<!-- -->55 0x0000000004c1bb1e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c1bb1e)
#<!-- -->56 0x0000000000d5dc0f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5dc0f)
#<!-- -->57 0x0000000000d553ca ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->58 0x00000000048a2469 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->59 0x0000000003de8924 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3de8924)
#<!-- -->60 0x00000000048a2a7f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->61 0x000000000486543d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486543d)
#<!-- -->62 0x00000000048664ce clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48664ce)
#<!-- -->63 0x000000000486dbd5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486dbd5)
#<!-- -->64 0x0000000000d5aa08 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5aa08)
#<!-- -->65 0x0000000000c20d34 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc20d34)
#<!-- -->66 0x00007a01e8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->67 0x00007a01e8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->68 0x0000000000d54e75 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd54e75)
```

&lt;/details&gt;

</details>


---

### Comment 2 - zyn0217

You'll get a crash on invalid prior to 20 if you somehow use the init-captured variable:

https://compiler-explorer.com/z/cqooEnq4j

---

### Comment 3 - shafik

> You'll get a crash on invalid prior to 20 if you somehow use the init-captured variable:
> 
> https://compiler-explorer.com/z/cqooEnq4j

Do you think they are the same bug or two different bugs?

---

### Comment 4 - shafik

clang accepts this but other implementations don't: https://godbolt.org/z/bzafoj14f

```cpp
void f() {
    auto f{ 
      [](auto arg) {
        return [a = arg]() 
          requires (a == 1)
         { return a;};
      }
    };

    f(10);
}
```

---

### Comment 5 - shafik

@LYP951018 git bisect point to 52126dc72c3f6f4d27e3835b0ad53e162af25e53 as the cause, my guess is that this just uncovers the previous issue.

---

### Comment 6 - shafik

@erichkeane git bisect the crash that goes back further is linked to babdef27c503c0bbbcc017e9f88affddda90ea4e

---

### Comment 7 - zyn0217

> > You'll get a crash on invalid prior to 20 if you somehow use the init-captured variable:
> > https://compiler-explorer.com/z/cqooEnq4j
> 
> Do you think they are the same bug or two different bugs?

They're different. I filed #134193 to track these.

---

