# Clang Crashes When Instantiating a Static Templated Method With void Parameter Pack since first clang (3.0.0)

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/139053

## Body

Clang crashes when instantiating a call to a templated static method with a void type in the parameter pack. The issue appears in the template specialization my_if<0, true>::run<void>(), even though the method does not use the type itself.

## Assertion

```
lang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExpr.cpp:5562: clang::ExprResult clang::Sema::BuildCXXDefaultArgExpr(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*, clang::Expr*): Assertion `Param->hasDefaultArg() && "can't build nonexistent default arg"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:20:31: current parser token ')'
2.	<source>:19:1: parsing function body 'main'
3.	<source>:19:1: in compound statement ('{}')
 #0 0x0000000003f7c0a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7c0a8)
 #1 0x0000000003f79d34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f79d34)
 #2 0x0000000003ebebf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000793fea242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000793fea2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000793fea242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000793fea2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000793fea22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000793fea239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006b1cab5 clang::Sema::BuildCXXDefaultArgExpr(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1cab5)
#10 0x0000000006b3f116 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef<clang::Expr*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::VariadicCallType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3f116)
#11 0x0000000006b40636 clang::Sema::ConvertArgumentsForCall(clang::CallExpr*, clang::Expr*, clang::FunctionDecl*, clang::FunctionProtoType const*, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b40636)
#12 0x0000000006b41fe1 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b41fe1)
#13 0x0000000006f491ba FinishOverloadedCallExpr(clang::Sema&, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#14 0x0000000006f49e90 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f49e90)
#15 0x0000000006b43c86 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b43c86)
#16 0x0000000006b44cfe clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b44cfe)
#17 0x0000000006675ced clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6675ced)
#18 0x000000000666e6fa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666e6fa)
#19 0x0000000006670897 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6670897)
#20 0x0000000006670929 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6670929)
#21 0x0000000006675309 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6675309)
#22 0x00000000066f95e9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f95e9)
#23 0x00000000066f10a2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f10a2)
#24 0x00000000066f1fcd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f1fcd)
#25 0x00000000066f9ce3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f9ce3)
#26 0x00000000066fa4aa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66fa4aa)
#27 0x0000000006601a53 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6601a53)
#28 0x00000000066380fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66380fd)
#29 0x00000000065f558e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f558e)
#30 0x00000000065f5d49 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f5d49)
#31 0x00000000065fd66a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fd66a)
#32 0x00000000065fe60d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fe60d)
#33 0x00000000065f09ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f09ea)
#34 0x000000000491ebe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x491ebe8)
#35 0x0000000004c11f15 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c11f15)
#36 0x0000000004b9131e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9131e)
#37 0x0000000004d05d39 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d05d39)
#38 0x0000000000da788f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda788f)
#39 0x0000000000d9da5a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x00000000049854a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x0000000003ebf094 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebf094)
#42 0x0000000004985abf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x0000000004947f0d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4947f0d)
#44 0x0000000004948f9e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4948f9e)
#45 0x0000000004950df5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4950df5)
#46 0x0000000000da37f8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda37f8)
#47 0x0000000000c28b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc28b74)
#48 0x0000793fea229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#49 0x0000793fea229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#50 0x0000000000d9d505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d505)
```

## Program

```
#include <iostream>
template<int M, bool E = false>
struct my_if
{
    template<typename... _Types>
    static void run(_Types &&... __args) noexcept
    {
    }
};
template<typename... _Types>
struct my_if<0, true>
{
    template<typename... _Args>
    static void run(_Args &&...) noexcept
    {
    }
};
int main()
{
    my_if<0, true>::run<void>();
}
```

## To quickly reproduce

happens from clang 3.0.0 until current

https://gcc.godbolt.org/z/bqM3bd8vx

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
Clang crashes when instantiating a call to a templated static method with a void type in the parameter pack. The issue appears in the template specialization my_if&lt;0, true&gt;::run&lt;void&gt;(), even though the method does not use the type itself.

## Assertion

```
lang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaExpr.cpp:5562: clang::ExprResult clang::Sema::BuildCXXDefaultArgExpr(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*, clang::Expr*): Assertion `Param-&gt;hasDefaultArg() &amp;&amp; "can't build nonexistent default arg"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:20:31: current parser token ')'
2.	&lt;source&gt;:19:1: parsing function body 'main'
3.	&lt;source&gt;:19:1: in compound statement ('{}')
 #<!-- -->0 0x0000000003f7c0a8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f7c0a8)
 #<!-- -->1 0x0000000003f79d34 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f79d34)
 #<!-- -->2 0x0000000003ebebf8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000793fea242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000793fea2969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000793fea242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000793fea2287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000793fea22871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000793fea239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006b1cab5 clang::Sema::BuildCXXDefaultArgExpr(clang::SourceLocation, clang::FunctionDecl*, clang::ParmVarDecl*, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1cab5)
#<!-- -->10 0x0000000006b3f116 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::VariadicCallType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b3f116)
#<!-- -->11 0x0000000006b40636 clang::Sema::ConvertArgumentsForCall(clang::CallExpr*, clang::Expr*, clang::FunctionDecl*, clang::FunctionProtoType const*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b40636)
#<!-- -->12 0x0000000006b41fe1 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b41fe1)
#<!-- -->13 0x0000000006f491ba FinishOverloadedCallExpr(clang::Sema&amp;, clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, clang::OverloadCandidateSet*, clang::OverloadCandidate**, clang::OverloadingResult, bool) SemaOverload.cpp:0:0
#<!-- -->14 0x0000000006f49e90 clang::Sema::BuildOverloadedCallExpr(clang::Scope*, clang::Expr*, clang::UnresolvedLookupExpr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f49e90)
#<!-- -->15 0x0000000006b43c86 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b43c86)
#<!-- -->16 0x0000000006b44cfe clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b44cfe)
#<!-- -->17 0x0000000006675ced clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6675ced)
#<!-- -->18 0x000000000666e6fa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666e6fa)
#<!-- -->19 0x0000000006670897 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypeCastState, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6670897)
#<!-- -->20 0x0000000006670929 clang::Parser::ParseAssignmentExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6670929)
#<!-- -->21 0x0000000006675309 clang::Parser::ParseExpression(clang::TypeCastState) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6675309)
#<!-- -->22 0x00000000066f95e9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f95e9)
#<!-- -->23 0x00000000066f10a2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f10a2)
#<!-- -->24 0x00000000066f1fcd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f1fcd)
#<!-- -->25 0x00000000066f9ce3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f9ce3)
#<!-- -->26 0x00000000066fa4aa clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66fa4aa)
#<!-- -->27 0x0000000006601a53 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6601a53)
#<!-- -->28 0x00000000066380fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66380fd)
#<!-- -->29 0x00000000065f558e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f558e)
#<!-- -->30 0x00000000065f5d49 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f5d49)
#<!-- -->31 0x00000000065fd66a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fd66a)
#<!-- -->32 0x00000000065fe60d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65fe60d)
#<!-- -->33 0x00000000065f09ea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65f09ea)
#<!-- -->34 0x000000000491ebe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x491ebe8)
#<!-- -->35 0x0000000004c11f15 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c11f15)
#<!-- -->36 0x0000000004b9131e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b9131e)
#<!-- -->37 0x0000000004d05d39 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d05d39)
#<!-- -->38 0x0000000000da788f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda788f)
#<!-- -->39 0x0000000000d9da5a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->40 0x00000000049854a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->41 0x0000000003ebf094 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ebf094)
#<!-- -->42 0x0000000004985abf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->43 0x0000000004947f0d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4947f0d)
#<!-- -->44 0x0000000004948f9e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4948f9e)
#<!-- -->45 0x0000000004950df5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4950df5)
#<!-- -->46 0x0000000000da37f8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda37f8)
#<!-- -->47 0x0000000000c28b74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc28b74)
#<!-- -->48 0x0000793fea229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->49 0x0000793fea229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->50 0x0000000000d9d505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9d505)
```

## Program

```
#include &lt;iostream&gt;
template&lt;int M, bool E = false&gt;
struct my_if
{
    template&lt;typename... _Types&gt;
    static void run(_Types &amp;&amp;... __args) noexcept
    {
    }
};
template&lt;typename... _Types&gt;
struct my_if&lt;0, true&gt;
{
    template&lt;typename... _Args&gt;
    static void run(_Args &amp;&amp;...) noexcept
    {
    }
};
int main()
{
    my_if&lt;0, true&gt;::run&lt;void&gt;();
}
```

## To quickly reproduce

happens from clang 3.0.0 until current

https://gcc.godbolt.org/z/bqM3bd8vx
</details>


---

