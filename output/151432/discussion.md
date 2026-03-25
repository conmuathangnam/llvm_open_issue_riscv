# ICE segfault on pack index in noexcept specifier of generic lambda in requires clause

**Author:** Eczbek
**URL:** https://github.com/llvm/llvm-project/issues/151432

## Body

https://godbolt.org/z/Kcqxaaocq
```cpp
void f(auto... a) requires requires { []<int i = 0> noexcept(noexcept(a...[i])) {}(); } {}

int main() { f(0); }
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 <source> -isystem/opt/compiler-explorer/libs/xieite/main/include
1.	<source>:1:84: current parser token ')'
2.	<source>:1:39: instantiating function definition '(anonymous class)::operator()<0>'
 #0 0x0000000003ca5fb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca5fb8)
 #1 0x0000000003ca398c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca398c)
 #2 0x0000000003bf3cb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078fcc8642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006e6cd56 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, clang::MultiLevelTemplateArgumentList const&, bool&, bool&, clang::UnsignedOrNone&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e6cd56)
 #5 0x0000000006dcaa4d (anonymous namespace)::TemplateInstantiator::TryExpandParameterPacks(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, bool&, bool&, clang::UnsignedOrNone&) SemaTemplateInstantiate.cpp:0:0
 #6 0x0000000006dd071e clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformPackIndexingExpr(clang::PackIndexingExpr*) SemaTemplateInstantiate.cpp:0:0
 #7 0x0000000006db4a25 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #8 0x0000000006dba69a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXNoexceptExpr(clang::CXXNoexceptExpr*) SemaTemplateInstantiate.cpp:0:0
 #9 0x0000000006db46b7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000006db48ac clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#11 0x0000000006dd7c1f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExceptionSpec(clang::SourceLocation, clang::FunctionProtoType::ExceptionSpecInfo&, llvm::SmallVectorImpl<clang::QualType>&, bool&) SemaTemplateInstantiate.cpp:0:0
#12 0x0000000006dd8543 clang::Sema::SubstExceptionSpec(clang::FunctionDecl*, clang::FunctionProtoType const*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6dd8543)
#13 0x0000000006e0a9f1 clang::Sema::InstantiateExceptionSpec(clang::SourceLocation, clang::FunctionDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e0a9f1)
#14 0x000000000672eddc clang::Sema::ResolveExceptionSpec(clang::SourceLocation, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x672eddc)
#15 0x00000000066442bf clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Decl*, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66442bf)
#16 0x0000000006e4e19c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e4e19c)
#17 0x0000000007a71e21 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7a71e21)
#18 0x0000000006cfead1 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cfead1)
#19 0x0000000006786de6 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6786de6)
#20 0x0000000006b17af7 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#21 0x0000000006b5ffea clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b5ffea)
#22 0x00000000067e39a8 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67e39a8)
#23 0x00000000067e7c5c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67e7c5c)
#24 0x00000000062ea5ae clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ea5ae)
#25 0x00000000062e266a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e266a)
#26 0x00000000062e490a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e490a)
#27 0x00000000062e4af9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e4af9)
#28 0x00000000062e9aa9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e9aa9)
#29 0x00000000062f9b43 clang::Parser::ParseRequiresExpression() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f9b43)
#30 0x00000000062e365d clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e365d)
#31 0x00000000062e490a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e490a)
#32 0x00000000062effba clang::Parser::ParseConstraintLogicalAndExpression(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62effba)
#33 0x00000000062f0337 clang::Parser::ParseConstraintLogicalOrExpression(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f0337)
#34 0x00000000062c2be5 clang::Parser::ParseTrailingRequiresClause(clang::Declarator&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62c2be5)
#35 0x00000000062b1fdd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b1fdd)
#36 0x000000000625ebaf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625ebaf)
#37 0x000000000625f997 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625f997)
#38 0x000000000626858f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626858f)
#39 0x0000000006269da0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6269da0)
#40 0x000000000626a2f0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626a2f0)
#41 0x00000000062584e3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62584e3)
#42 0x00000000045f098d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f098d)
#43 0x00000000048fa40a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48fa40a)
#44 0x000000000487546b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487546b)
#45 0x00000000049ee543 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ee543)
#46 0x0000000000dbabf5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbabf5)
#47 0x0000000000db2b4d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#48 0x00000000046628f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#49 0x0000000003bf40d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf40d3)
#50 0x0000000004662b19 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#51 0x0000000004625fbd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625fbd)
#52 0x0000000004626f71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4626f71)
#53 0x000000000463370c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463370c)
#54 0x0000000000db7411 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7411)
#55 0x0000000000c67984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67984)
#56 0x000078fcc8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#57 0x000078fcc8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#58 0x0000000000db25e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb25e5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Eczbek)

<details>
https://godbolt.org/z/Kcqxaaocq
```cpp
void f(auto... a) requires requires { []&lt;int i = 0&gt; noexcept(noexcept(a...[i])) {}(); } {}

int main() { f(0); }
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++26 &lt;source&gt; -isystem/opt/compiler-explorer/libs/xieite/main/include
1.	&lt;source&gt;:1:84: current parser token ')'
2.	&lt;source&gt;:1:39: instantiating function definition '(anonymous class)::operator()&lt;0&gt;'
 #<!-- -->0 0x0000000003ca5fb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca5fb8)
 #<!-- -->1 0x0000000003ca398c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca398c)
 #<!-- -->2 0x0000000003bf3cb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078fcc8642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000000006e6cd56 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef&lt;std::pair&lt;llvm::PointerUnion&lt;clang::TemplateTypeParmType const*, clang::NamedDecl*&gt;, clang::SourceLocation&gt;&gt;, clang::MultiLevelTemplateArgumentList const&amp;, bool&amp;, bool&amp;, clang::UnsignedOrNone&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e6cd56)
 #<!-- -->5 0x0000000006dcaa4d (anonymous namespace)::TemplateInstantiator::TryExpandParameterPacks(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef&lt;std::pair&lt;llvm::PointerUnion&lt;clang::TemplateTypeParmType const*, clang::NamedDecl*&gt;, clang::SourceLocation&gt;&gt;, bool&amp;, bool&amp;, clang::UnsignedOrNone&amp;) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->6 0x0000000006dd071e clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformPackIndexingExpr(clang::PackIndexingExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->7 0x0000000006db4a25 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->8 0x0000000006dba69a clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCXXNoexceptExpr(clang::CXXNoexceptExpr*) SemaTemplateInstantiate.cpp:0:0
 #<!-- -->9 0x0000000006db46b7 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->10 0x0000000006db48ac clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->11 0x0000000006dd7c1f clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExceptionSpec(clang::SourceLocation, clang::FunctionProtoType::ExceptionSpecInfo&amp;, llvm::SmallVectorImpl&lt;clang::QualType&gt;&amp;, bool&amp;) SemaTemplateInstantiate.cpp:0:0
#<!-- -->12 0x0000000006dd8543 clang::Sema::SubstExceptionSpec(clang::FunctionDecl*, clang::FunctionProtoType const*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6dd8543)
#<!-- -->13 0x0000000006e0a9f1 clang::Sema::InstantiateExceptionSpec(clang::SourceLocation, clang::FunctionDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e0a9f1)
#<!-- -->14 0x000000000672eddc clang::Sema::ResolveExceptionSpec(clang::SourceLocation, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x672eddc)
#<!-- -->15 0x00000000066442bf clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Decl*, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66442bf)
#<!-- -->16 0x0000000006e4e19c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e4e19c)
#<!-- -->17 0x0000000007a71e21 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7a71e21)
#<!-- -->18 0x0000000006cfead1 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cfead1)
#<!-- -->19 0x0000000006786de6 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6786de6)
#<!-- -->20 0x0000000006b17af7 CreateFunctionRefExpr(clang::Sema&amp;, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&amp;) SemaOverload.cpp:0:0
#<!-- -->21 0x0000000006b5ffea clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b5ffea)
#<!-- -->22 0x00000000067e39a8 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67e39a8)
#<!-- -->23 0x00000000067e7c5c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67e7c5c)
#<!-- -->24 0x00000000062ea5ae clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ea5ae)
#<!-- -->25 0x00000000062e266a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e266a)
#<!-- -->26 0x00000000062e490a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e490a)
#<!-- -->27 0x00000000062e4af9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e4af9)
#<!-- -->28 0x00000000062e9aa9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e9aa9)
#<!-- -->29 0x00000000062f9b43 clang::Parser::ParseRequiresExpression() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f9b43)
#<!-- -->30 0x00000000062e365d clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e365d)
#<!-- -->31 0x00000000062e490a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e490a)
#<!-- -->32 0x00000000062effba clang::Parser::ParseConstraintLogicalAndExpression(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62effba)
#<!-- -->33 0x00000000062f0337 clang::Parser::ParseConstraintLogicalOrExpression(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f0337)
#<!-- -->34 0x00000000062c2be5 clang::Parser::ParseTrailingRequiresClause(clang::Declarator&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62c2be5)
#<!-- -->35 0x00000000062b1fdd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b1fdd)
#<!-- -->36 0x000000000625ebaf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625ebaf)
#<!-- -->37 0x000000000625f997 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625f997)
#<!-- -->38 0x000000000626858f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626858f)
#<!-- -->39 0x0000000006269da0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6269da0)
#<!-- -->40 0x000000000626a2f0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626a2f0)
#<!-- -->41 0x00000000062584e3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62584e3)
#<!-- -->42 0x00000000045f098d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f098d)
#<!-- -->43 0x00000000048fa40a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48fa40a)
#<!-- -->44 0x000000000487546b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487546b)
#<!-- -->45 0x00000000049ee543 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ee543)
#<!-- -->46 0x0000000000dbabf5 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbabf5)
#<!-- -->47 0x0000000000db2b4d ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->48 0x00000000046628f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->49 0x0000000003bf40d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf40d3)
#<!-- -->50 0x0000000004662b19 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->51 0x0000000004625fbd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625fbd)
#<!-- -->52 0x0000000004626f71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4626f71)
#<!-- -->53 0x000000000463370c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463370c)
#<!-- -->54 0x0000000000db7411 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7411)
#<!-- -->55 0x0000000000c67984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67984)
#<!-- -->56 0x000078fcc8629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->57 0x000078fcc8629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->58 0x0000000000db25e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb25e5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

</details>


---

### Comment 2 - Eczbek

Possibly reduced further: https://godbolt.org/z/3x1on7T4d
```cpp
void f(auto... a) requires requires { []<int = 0>() noexcept(noexcept((..., a))) {}(); } {}

int main() { f(0); }
```
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:1:85: current parser token ')'
2.	<source>:1:39: instantiating function definition '(anonymous class)::operator()<0>'
 #0 0x0000000003ca5fb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca5fb8)
 #1 0x0000000003ca398c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca398c)
 #2 0x0000000003bf3cb8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007467cb642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000000006e6cd56 clang::Sema::CheckParameterPacksForExpansion(clang::SourceLocation, clang::SourceRange, llvm::ArrayRef<std::pair<llvm::PointerUnion<clang::TemplateTypeParmType const*, clang::NamedDecl*>, clang::SourceLocation>>, clang::MultiLevelTemplateArgumentList const&, bool&, bool&, clang::UnsignedOrNone&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e6cd56)
 #5 0x0000000006dd87db clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXFoldExpr(clang::CXXFoldExpr*) SemaTemplateInstantiate.cpp:0:0
 #6 0x0000000006db4699 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #7 0x0000000006dba69a clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCXXNoexceptExpr(clang::CXXNoexceptExpr*) SemaTemplateInstantiate.cpp:0:0
 #8 0x0000000006db46b7 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
 #9 0x0000000006dd7c1f clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExceptionSpec(clang::SourceLocation, clang::FunctionProtoType::ExceptionSpecInfo&, llvm::SmallVectorImpl<clang::QualType>&, bool&) SemaTemplateInstantiate.cpp:0:0
#10 0x0000000006dd8543 clang::Sema::SubstExceptionSpec(clang::FunctionDecl*, clang::FunctionProtoType const*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6dd8543)
#11 0x0000000006e0a9f1 clang::Sema::InstantiateExceptionSpec(clang::SourceLocation, clang::FunctionDecl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e0a9f1)
#12 0x000000000672eddc clang::Sema::ResolveExceptionSpec(clang::SourceLocation, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x672eddc)
#13 0x00000000066442bf clang::Sema::ActOnStartOfFunctionDef(clang::Scope*, clang::Decl*, clang::SkipBodyInfo*, clang::Sema::FnBodyKind) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x66442bf)
#14 0x0000000006e4e19c clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6e4e19c)
#15 0x0000000007a71e21 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x7a71e21)
#16 0x0000000006cfead1 clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6cfead1)
#17 0x0000000006786de6 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6786de6)
#18 0x0000000006b17af7 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#19 0x0000000006b5ffea clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6b5ffea)
#20 0x00000000067e39a8 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67e39a8)
#21 0x00000000067e7c5c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x67e7c5c)
#22 0x00000000062ea5ae clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62ea5ae)
#23 0x00000000062e266a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e266a)
#24 0x00000000062e490a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e490a)
#25 0x00000000062e4af9 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e4af9)
#26 0x00000000062e9aa9 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e9aa9)
#27 0x00000000062f9b43 clang::Parser::ParseRequiresExpression() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f9b43)
#28 0x00000000062e365d clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e365d)
#29 0x00000000062e490a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62e490a)
#30 0x00000000062effba clang::Parser::ParseConstraintLogicalAndExpression(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62effba)
#31 0x00000000062f0337 clang::Parser::ParseConstraintLogicalOrExpression(bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62f0337)
#32 0x00000000062c2be5 clang::Parser::ParseTrailingRequiresClause(clang::Declarator&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62c2be5)
#33 0x00000000062b1fdd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62b1fdd)
#34 0x000000000625ebaf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625ebaf)
#35 0x000000000625f997 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x625f997)
#36 0x000000000626858f clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626858f)
#37 0x0000000006269da0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x6269da0)
#38 0x000000000626a2f0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x626a2f0)
#39 0x00000000062584e3 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62584e3)
#40 0x00000000045f098d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45f098d)
#41 0x00000000048fa40a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48fa40a)
#42 0x000000000487546b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487546b)
#43 0x00000000049ee543 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x49ee543)
#44 0x0000000000dbabf5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdbabf5)
#45 0x0000000000db2b4d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#46 0x00000000046628f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#47 0x0000000003bf40d3 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf40d3)
#48 0x0000000004662b19 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#49 0x0000000004625fbd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4625fbd)
#50 0x0000000004626f71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4626f71)
#51 0x000000000463370c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x463370c)
#52 0x0000000000db7411 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb7411)
#53 0x0000000000c67984 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc67984)
#54 0x00007467cb629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#55 0x00007467cb629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#56 0x0000000000db25e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdb25e5)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```


---

### Comment 3 - shafik

Goes back to clang-10: https://godbolt.org/z/sd8K9GnGP

---

