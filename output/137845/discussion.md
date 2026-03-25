# Clang 21 crashes when compiling invalid initializer in C (struct array literal cast)

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/137845

## Body

## Summary 

Clang 21 crashes with a stack dump when compiling a small invalid C program involving a struct array literal being cast and passed as an argument.

Assertion:

```console
clang++: /root/llvm-project/clang/include/clang/AST/Expr.h:4881:
clang::Expr* clang::InitListExpr::getInit(unsigned int):
Assertion `Init < getNumInits() && "Initializer access out of range!"' failed.
```

## Stack Trace

```
 #0 0x00000000042ed758 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42ed758)
 #1 0x00000000042eab84 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42eab84)
 #2 0x000000000422f928 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007becb5242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007becb52969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007becb5242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007becb52287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007becb522871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007becb5239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000070c4d27 clang::InitListExpr::getInit(unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70c4d27)
#10 0x00000000070d7d95 (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
#11 0x00000000070d80b7 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&, clang::InitializedEntity const&, clang::InitListExpr*, clang::QualType&, bool, bool, bool, llvm::SmallVectorImpl<clang::QualType>*) SemaInit.cpp:0:0
#12 0x00000000070e1752 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e1752)
#13 0x00000000070e50b2 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x70e50b2)
#14 0x0000000006f9ea21 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef<clang::Expr*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::VariadicCallType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9ea21)
#15 0x0000000006f9fbe0 clang::Sema::ConvertArgumentsForCall(clang::CallExpr*, clang::Expr*, clang::FunctionDecl*, clang::FunctionProtoType const*, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9fbe0)
#16 0x0000000006fa1592 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa1592)
#17 0x0000000006f9c62a clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9c62a)
#18 0x0000000006fa258c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fa258c)
#19 0x0000000006aa0987 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa0987)
#20 0x0000000006a9989a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9989a)
#21 0x0000000006a9baf7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9baf7)
#22 0x0000000006a9bb89 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9bb89)
#23 0x0000000006a9beee clang::Parser::ParseExpressionList(llvm::SmallVectorImpl<clang::Expr*>&, llvm::function_ref<void ()>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9beee)
#24 0x0000000006aa0c4d clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa0c4d)
#25 0x0000000006a9989a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9989a)
#26 0x0000000006a9baf7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9baf7)
#27 0x0000000006a9bb89 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9bb89)
#28 0x0000000006aa0169 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa0169)
#29 0x0000000006b25a29 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b25a29)
#30 0x0000000006b1ced4 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1ced4)
#31 0x0000000006b1d8b6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1d8b6)
#32 0x0000000006b261fe clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b261fe)
#33 0x0000000006b26a4a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b26a4a)
#34 0x0000000006a2c9ab clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2c9ab)
#35 0x0000000006a62e6d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a62e6d)
#36 0x0000000006a1fb0b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a1fb0b)
#37 0x0000000006a202ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a202ff)
#38 0x0000000006a2812a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a2812a)
#39 0x0000000006a290c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a290c5)
#40 0x0000000006a0a14a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a0a14a)
#41 0x0000000004c4ea88 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4ea88)
#42 0x0000000004f45385 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f45385)
#43 0x0000000004ec561e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ec561e)
#44 0x000000000503f70d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x503f70d)
#45 0x0000000000de5b8c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde5b8c)
#46 0x0000000000ddc5da ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#47 0x0000000000ddc75d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#48 0x0000000004cc13d9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#49 0x000000000422fdc4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x422fdc4)
#50 0x0000000004cc19ef clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#51 0x0000000004c824d2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c824d2)
#52 0x0000000004c8347e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8347e)
#53 0x0000000004c8a8d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c8a8d5)
#54 0x0000000000de1f81 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde1f81)
#55 0x0000000000c8cfd4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8cfd4)
#56 0x00007becb5229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#57 0x00007becb5229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#58 0x0000000000ddc075 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddc075)
```

## Program

 ``` 
#include <stdlib.h>
double swap(struct Data *a, struct Data *b)
{
}
int main(){
  printf("%lf\n", swap((struct Data[5]){ {}, 1 },
                       (struct Data[5]){ {}, 4 }));
}
```

## Command

```
clang -x c -std=c2x test.c
```

## To Reproduce Quickly

https://gcc.godbolt.org/z/dG7sb8oW4


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Summary 

Clang 21 crashes with a stack dump when compiling a small invalid C program involving a struct array literal being cast and passed as an argument.

## Stack Trace

```
Stack dump:
0.      Program arguments: /usr/local/bin/clang-21 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir a- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name prueba.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debugger-tuning=gdb -fdebug-compilation-dir=/root -fcoverage-compilation-dir=/root -resource-dir /usr/local/lib/clang/21 -internal-isystem /usr/local/lib/clang/21/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/9/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c2x -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/prueba-cf6791.o -x c prueba.cpp
1.      prueba.cpp:7:49: current parser token ')'
2.      prueba.cpp:5:11: parsing function body 'main'
3.      prueba.cpp:5:11: in compound statement ('{}')
 #<!-- -->0 0x000055fee950018f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/local/bin/clang-21+0x3c4a18f)
 #<!-- -->1 0x000055fee94fdbd4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007f5adbc08420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->3 0x000055feebe06b44 (anonymous namespace)::InitListChecker::CheckExplicitInitList(clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, clang::InitListExpr*, bool) (.constprop.0) SemaInit.cpp:0:0
 #<!-- -->4 0x000055feebe07217 (anonymous namespace)::InitListChecker::InitListChecker(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitListExpr*, clang::QualType&amp;, bool, bool, bool, llvm::SmallVectorImpl&lt;clang::QualType&gt;*) SemaInit.cpp:0:0
 #<!-- -->5 0x000055feebe08f23 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/usr/local/bin/clang-21+0x6552f23)
 #<!-- -->6 0x000055feebe1355e clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&amp;, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;, bool, bool) (/usr/local/bin/clang-21+0x655d55e)
 #<!-- -->7 0x000055feebc70d36 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::Sema::VariadicCallType, bool, bool) (/usr/local/bin/clang-21+0x63bad36)
 #<!-- -->8 0x000055feebc718af clang::Sema::ConvertArgumentsForCall(clang::CallExpr*, clang::Expr*, clang::FunctionDecl*, clang::FunctionProtoType const*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/usr/local/bin/clang-21+0x63bb8af)
 #<!-- -->9 0x000055feebc73760 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/usr/local/bin/clang-21+0x63bd760)
#<!-- -->10 0x000055feebc6f052 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/usr/local/bin/clang-21+0x63b9052)
#<!-- -->11 0x000055feebc7456b clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/usr/local/bin/clang-21+0x63be56b)
#<!-- -->12 0x000055feeb7a6a10 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/usr/local/bin/clang-21+0x5ef0a10)
#<!-- -->13 0x000055feeb79e5a7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/local/bin/clang-21+0x5ee85a7)
#<!-- -->14 0x000055feeb7a072e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/usr/local/bin/clang-21+0x5eea72e)
#<!-- -->15 0x000055feeb7a092d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/local/bin/clang-21+0x5eea92d)
#<!-- -->16 0x000055feeb7a0d42 clang::Parser::ParseExpressionList(llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, llvm::function_ref&lt;void ()&gt;, bool, bool, bool*) (/usr/local/bin/clang-21+0x5eead42)
#<!-- -->17 0x000055feeb7a7142 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/usr/local/bin/clang-21+0x5ef1142)
#<!-- -->18 0x000055feeb79e5a7 clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, bool&amp;, clang::Parser::TypeCastState, bool, bool*) (/usr/local/bin/clang-21+0x5ee85a7)
#<!-- -->19 0x000055feeb7a072e clang::Parser::ParseCastExpression(clang::Parser::CastParseKind, bool, clang::Parser::TypeCastState, bool, bool*) (/usr/local/bin/clang-21+0x5eea72e)
#<!-- -->20 0x000055feeb7a092d clang::Parser::ParseAssignmentExpression(clang::Parser::TypeCastState) (/usr/local/bin/clang-21+0x5eea92d)
#<!-- -->21 0x000055feeb7a5d7d clang::Parser::ParseExpression(clang::Parser::TypeCastState) (/usr/local/bin/clang-21+0x5eefd7d)
#<!-- -->22 0x000055feeb834d3f clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/usr/local/bin/clang-21+0x5f7ed3f)
#<!-- -->23 0x000055feeb8286b2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/usr/local/bin/clang-21+0x5f726b2)
#<!-- -->24 0x000055feeb829104 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 32u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/usr/local/bin/clang-21+0x5f73104)
#<!-- -->25 0x000055feeb82a46c clang::Parser::ParseCompoundStatementBody(bool) (/usr/local/bin/clang-21+0x5f7446c)
#<!-- -->26 0x000055feeb82c7e2 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/usr/local/bin/clang-21+0x5f767e2)
#<!-- -->27 0x000055feeb723e83 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/usr/local/bin/clang-21+0x5e6de83)
#<!-- -->28 0x000055feeb76df05 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/usr/local/bin/clang-21+0x5eb7f05)
#<!-- -->29 0x000055feeb71c62d clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6662d)
#<!-- -->30 0x000055feeb71d16e clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/usr/local/bin/clang-21+0x5e6716e)
#<!-- -->31 0x000055feeb726d73 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/usr/local/bin/clang-21+0x5e70d73)
#<!-- -->32 0x000055feeb7286a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/usr/local/bin/clang-21+0x5e726a8)
#<!-- -->33 0x000055feeb71662a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/local/bin/clang-21+0x5e6062a)
#<!-- -->34 0x000055fee9e84e45 clang::CodeGenAction::ExecuteAction() (/usr/local/bin/clang-21+0x45cee45)
#<!-- -->35 0x000055feea1699b8 clang::FrontendAction::Execute() (/usr/local/bin/clang-21+0x48b39b8)
#<!-- -->36 0x000055feea0e7d3b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/local/bin/clang-21+0x4831d3b)
#<!-- -->37 0x000055feea25bc7b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/local/bin/clang-21+0x49a5c7b)
#<!-- -->38 0x000055fee6758f51 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/local/bin/clang-21+0xea2f51)
#<!-- -->39 0x000055fee67515fd ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->40 0x000055fee675499f clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/local/bin/clang-21+0xe9e99f)
#<!-- -->41 0x000055fee662730b main (/usr/local/bin/clang-21+0xd7130b)
#<!-- -->42 0x00007f5adb6b6083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#<!-- -->43 0x000055fee675107e _start (/usr/local/bin/clang-21+0xe9b07e)
```

## Program

 ``` 
#include &lt;stdlib.h&gt;
#include &lt;stdio.h&gt;

struct Data { int dummy; };

double swap(struct Data *a, struct Data *b) {
}

int main() {
  printf("%lf\n", swap((struct Data[5]){ {}, 1 },
                       (struct Data[5]){ {}, 4 }));
}
```

## Command

```
clang -x c -std=c2x test.c
```

## To Reproduce Quickly

https://gcc.godbolt.org/z/dG7sb8oW4

</details>


---

### Comment 2 - shafik

Goes back to clang-17

---

