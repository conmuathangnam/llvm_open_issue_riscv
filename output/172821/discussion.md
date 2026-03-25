# [clang] Assertion "Complain && Not allowed to complain" fails in StructuralEquivalenceContext::Diag2 since clang 21.1.0

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/172821
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-invalid, regression:21
**Closed Date:** 2025-12-18T18:26:52Z

## Body

### Link for quick verification: https://godbolt.org/z/4soxWM3GE
```c
void foo(struct S *p) {
    (p)->a = 1.0;
}

struct __attribute__((aligned(1 << 4))) S {
    double a;
};

int main() {
    struct S s0;
    foo(&s0); 
}
```
Go back to `clang 21.1.0 (assertions)`
Share the same assertion failure as issue [153916](https://github.com/llvm/llvm-project/issues/153916) but with different stack dump.

## Stack dump
```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTStructuralEquivalence.cpp:2556: clang::DiagnosticBuilder clang::StructuralEquivalenceContext::Diag2(clang::SourceLocation, unsigned int): Assertion `Complain && "Not allowed to complain"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2x <source>
1.	<source>:12:12: current parser token ')'
2.	<source>:10:12: parsing function body 'main'
3.	<source>:10:12: in compound statement ('{}')
 #0 0x000000000429e098 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429e098)
 #1 0x000000000429af44 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429af44)
 #2 0x000000000429b4c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429b4c4)
 #3 0x00000000041e0208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #4 0x000073ee6a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #5 0x000073ee6a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #6 0x000073ee6a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #7 0x000073ee6a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #8 0x000073ee6a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #9 0x000073ee6a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#10 0x00000000079cc697 CheckStructurallyEquivalentAttributes(clang::StructuralEquivalenceContext&, clang::Decl const*, clang::Decl const*, clang::Decl const*) ASTStructuralEquivalence.cpp:0:0
#11 0x00000000079dd300 IsStructurallyEquivalent(clang::StructuralEquivalenceContext&, clang::RecordDecl*, clang::RecordDecl*) ASTStructuralEquivalence.cpp:0:0
#12 0x00000000079de7dd clang::StructuralEquivalenceContext::CheckKindSpecificEquivalence(clang::Decl*, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79de7dd)
#13 0x00000000079df313 clang::StructuralEquivalenceContext::Finish() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79df313)
#14 0x00000000079df678 clang::StructuralEquivalenceContext::IsEquivalent(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79df678)
#15 0x0000000007892c78 clang::ASTContext::mergeTagDefinitions(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7892c78)
#16 0x00000000078ecc4a clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78ecc4a)
#17 0x00000000078efaf4 clang::ASTContext::typesAreCompatible(clang::QualType, clang::QualType, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78efaf4)
#18 0x0000000006e950e9 checkPointerTypesForAssignment(clang::Sema&, clang::QualType, clang::QualType, clang::SourceLocation) SemaExpr.cpp:0:0
#19 0x0000000006ea862f clang::Sema::CheckAssignmentConstraints(clang::QualType, clang::ActionResult<clang::Expr*, true>&, clang::CastKind&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ea862f)
#20 0x0000000006f09dd5 clang::Sema::CheckSingleAssignmentConstraints(clang::QualType, clang::ActionResult<clang::Expr*, true>&, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f09dd5)
#21 0x0000000007053b54 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7053b54)
#22 0x0000000007057162 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&, clang::SourceLocation, clang::ActionResult<clang::Expr*, true>, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7057162)
#23 0x0000000006f11fc1 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef<clang::Expr*>, llvm::SmallVectorImpl<clang::Expr*>&, clang::VariadicCallType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f11fc1)
#24 0x0000000006f13180 clang::Sema::ConvertArgumentsForCall(clang::CallExpr*, clang::Expr*, clang::FunctionDecl*, clang::FunctionProtoType const*, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f13180)
#25 0x0000000006f14b32 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f14b32)
#26 0x0000000006f0fc7c clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f0fc7c)
#27 0x0000000006f15b2c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f15b2c)
#28 0x0000000006a17697 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a17697)
#29 0x0000000006a105aa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a105aa)
#30 0x0000000006a12807 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12807)
#31 0x0000000006a12899 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12899)
#32 0x0000000006a16e79 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a16e79)
#33 0x0000000006a9c5a9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9c5a9)
#34 0x0000000006a94d75 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a94d75)
#35 0x0000000006a95739 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95739)
#36 0x0000000006a9cd26 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9cd26)
#37 0x0000000006a9d53a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9d53a)
#38 0x000000000699ebdb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699ebdb)
#39 0x00000000069dec8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69dec8d)
#40 0x000000000699714b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699714b)
#41 0x000000000699793f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699793f)
#42 0x00000000069a2bd3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a2bd3)
#43 0x00000000069a3ae5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3ae5)
#44 0x000000000698176a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698176a)
#45 0x0000000004bf5da8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf5da8)
#46 0x0000000004eea105 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eea105)
#47 0x0000000004e6a3be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6a3be)
#48 0x0000000004fe41ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fe41ed)
#49 0x0000000000dd9ef2 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9ef2)
#50 0x0000000000dd091a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#51 0x0000000000dd0a9d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#52 0x0000000004c67a89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#53 0x00000000041e06a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e06a4)
#54 0x0000000004c680b6 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#55 0x0000000004c29732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c29732)
#56 0x0000000004c2a6de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2a6de)
#57 0x0000000004c319b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c319b5)
#58 0x0000000000dd62c1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd62c1)
#59 0x0000000000c813d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc813d4)
#60 0x000073ee6a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#61 0x000073ee6a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#62 0x0000000000dd03b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd03b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/4soxWM3GE
```c
void foo(struct S *p) {
    (p)-&gt;a = 1.0;
}

struct __attribute__((aligned(1 &lt;&lt; 4))) S {
    double a;
};

int main() {
    struct S s0;
    foo(&amp;s0); 
}
```
Go back to `clang 21.1.0 (assertions)`
Share the same assertion failure as issue [153916](https://github.com/llvm/llvm-project/issues/153916) but with different stack dump.

## Stack dump
```
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTStructuralEquivalence.cpp:2556: clang::DiagnosticBuilder clang::StructuralEquivalenceContext::Diag2(clang::SourceLocation, unsigned int): Assertion `Complain &amp;&amp; "Not allowed to complain"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c2x &lt;source&gt;
1.	&lt;source&gt;:12:12: current parser token ')'
2.	&lt;source&gt;:10:12: parsing function body 'main'
3.	&lt;source&gt;:10:12: in compound statement ('{}')
 #<!-- -->0 0x000000000429e098 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429e098)
 #<!-- -->1 0x000000000429af44 llvm::sys::RunSignalHandlers() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429af44)
 #<!-- -->2 0x000000000429b4c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x429b4c4)
 #<!-- -->3 0x00000000041e0208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->4 0x000073ee6a042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->5 0x000073ee6a0969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->6 0x000073ee6a042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->7 0x000073ee6a0287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->8 0x000073ee6a02871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->9 0x000073ee6a039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
#<!-- -->10 0x00000000079cc697 CheckStructurallyEquivalentAttributes(clang::StructuralEquivalenceContext&amp;, clang::Decl const*, clang::Decl const*, clang::Decl const*) ASTStructuralEquivalence.cpp:0:0
#<!-- -->11 0x00000000079dd300 IsStructurallyEquivalent(clang::StructuralEquivalenceContext&amp;, clang::RecordDecl*, clang::RecordDecl*) ASTStructuralEquivalence.cpp:0:0
#<!-- -->12 0x00000000079de7dd clang::StructuralEquivalenceContext::CheckKindSpecificEquivalence(clang::Decl*, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79de7dd)
#<!-- -->13 0x00000000079df313 clang::StructuralEquivalenceContext::Finish() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79df313)
#<!-- -->14 0x00000000079df678 clang::StructuralEquivalenceContext::IsEquivalent(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x79df678)
#<!-- -->15 0x0000000007892c78 clang::ASTContext::mergeTagDefinitions(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7892c78)
#<!-- -->16 0x00000000078ecc4a clang::ASTContext::mergeTypes(clang::QualType, clang::QualType, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78ecc4a)
#<!-- -->17 0x00000000078efaf4 clang::ASTContext::typesAreCompatible(clang::QualType, clang::QualType, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x78efaf4)
#<!-- -->18 0x0000000006e950e9 checkPointerTypesForAssignment(clang::Sema&amp;, clang::QualType, clang::QualType, clang::SourceLocation) SemaExpr.cpp:0:0
#<!-- -->19 0x0000000006ea862f clang::Sema::CheckAssignmentConstraints(clang::QualType, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, clang::CastKind&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ea862f)
#<!-- -->20 0x0000000006f09dd5 clang::Sema::CheckSingleAssignmentConstraints(clang::QualType, clang::ActionResult&lt;clang::Expr*, true&gt;&amp;, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f09dd5)
#<!-- -->21 0x0000000007053b54 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7053b54)
#<!-- -->22 0x0000000007057162 clang::Sema::PerformCopyInitialization(clang::InitializedEntity const&amp;, clang::SourceLocation, clang::ActionResult&lt;clang::Expr*, true&gt;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7057162)
#<!-- -->23 0x0000000006f11fc1 clang::Sema::GatherArgumentsForCall(clang::SourceLocation, clang::FunctionDecl*, clang::FunctionProtoType const*, unsigned int, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::VariadicCallType, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f11fc1)
#<!-- -->24 0x0000000006f13180 clang::Sema::ConvertArgumentsForCall(clang::CallExpr*, clang::Expr*, clang::FunctionDecl*, clang::FunctionProtoType const*, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f13180)
#<!-- -->25 0x0000000006f14b32 clang::Sema::BuildResolvedCallExpr(clang::Expr*, clang::NamedDecl*, clang::SourceLocation, llvm::ArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, clang::CallExpr::ADLCallKind) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f14b32)
#<!-- -->26 0x0000000006f0fc7c clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f0fc7c)
#<!-- -->27 0x0000000006f15b2c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f15b2c)
#<!-- -->28 0x0000000006a17697 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a17697)
#<!-- -->29 0x0000000006a105aa clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a105aa)
#<!-- -->30 0x0000000006a12807 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12807)
#<!-- -->31 0x0000000006a12899 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a12899)
#<!-- -->32 0x0000000006a16e79 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a16e79)
#<!-- -->33 0x0000000006a9c5a9 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9c5a9)
#<!-- -->34 0x0000000006a94d75 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a94d75)
#<!-- -->35 0x0000000006a95739 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a95739)
#<!-- -->36 0x0000000006a9cd26 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9cd26)
#<!-- -->37 0x0000000006a9d53a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9d53a)
#<!-- -->38 0x000000000699ebdb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699ebdb)
#<!-- -->39 0x00000000069dec8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69dec8d)
#<!-- -->40 0x000000000699714b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699714b)
#<!-- -->41 0x000000000699793f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x699793f)
#<!-- -->42 0x00000000069a2bd3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a2bd3)
#<!-- -->43 0x00000000069a3ae5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a3ae5)
#<!-- -->44 0x000000000698176a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698176a)
#<!-- -->45 0x0000000004bf5da8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bf5da8)
#<!-- -->46 0x0000000004eea105 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4eea105)
#<!-- -->47 0x0000000004e6a3be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e6a3be)
#<!-- -->48 0x0000000004fe41ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fe41ed)
#<!-- -->49 0x0000000000dd9ef2 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd9ef2)
#<!-- -->50 0x0000000000dd091a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->51 0x0000000000dd0a9d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->52 0x0000000004c67a89 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->53 0x00000000041e06a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e06a4)
#<!-- -->54 0x0000000004c680b6 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->55 0x0000000004c29732 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c29732)
#<!-- -->56 0x0000000004c2a6de clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2a6de)
#<!-- -->57 0x0000000004c319b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c319b5)
#<!-- -->58 0x0000000000dd62c1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd62c1)
#<!-- -->59 0x0000000000c813d4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc813d4)
#<!-- -->60 0x000073ee6a029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->61 0x000073ee6a029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->62 0x0000000000dd03b5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd03b5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

The code is different but it really looks like the root cause is the same, if the fix for the previous one does not fix this we can reopen.

---

