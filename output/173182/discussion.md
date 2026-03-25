# Clang ICE: APInt shift assertion failure in enum constant evaluation with __int128 values

**Author:** AbyssStaror
**URL:** https://github.com/llvm/llvm-project/issues/173182

## Body

### Link for quick verification: https://godbolt.org/z/4srefcEKh
```C
enum E {
    V1 = (__int128)0x1000000000000,  // Value exceeds intmax_t; relies on __int128
    V2 = (V1 << 64) + 1             // Ensures complex expressions in enumeration definition are preserved and evaluated correctly
};

__int128 get_val() {
    return (enum E)__int128(V2 >> 4); // Interacting with enum values as part of computation involving type conversion and arithmetic
}

```
### Observations

- Clang 19-20 (assertions):​ Fixed
- Clang 18 and earlier (assertions): crashed in /root/llvm-project/clang/lib/Sema/SemaDecl.cpp
- Clang 21 and later (assertions): crashed in /root/llvm-project/llvm/include/llvm/ADT/APInt.h

## Stack dump
```
clang++: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:787: llvm::APInt& llvm::APInt::operator<<=(unsigned int): Assertion `ShiftAmt <= BitWidth && "Invalid shift amount"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -O3 <source>
1.	<source>:7:37: current parser token ';'
2.	<source>:6:20: parsing function body 'get_val'
3.	<source>:6:20: in compound statement ('{}')
 #0 0x00000000042a6d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42a6d18)
 #1 0x00000000042a4144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42a4144)
 #2 0x00000000041e9638 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b9115642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b91156969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b9115642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b91156287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b911562871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b9115639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007ac62c4 clang::EnumDecl::getValueRange(llvm::APInt&, llvm::APInt&) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ac62c4)
#10 0x0000000007ccc8cd (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#11 0x0000000007c87b2e clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool>::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#12 0x0000000007c615be Evaluate(clang::APValue&, (anonymous namespace)::EvalInfo&, clang::Expr const*) ExprConstant.cpp:0:0
#13 0x0000000007c61ebb EvaluateAsRValue((anonymous namespace)::EvalInfo&, clang::Expr const*, clang::APValue&) ExprConstant.cpp:0:0
#14 0x0000000007c67387 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&, clang::ASTContext const&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c67387)
#15 0x0000000006bdc02b TryGetExprRange(clang::ASTContext&, clang::Expr const*, unsigned int, bool, bool) SemaChecking.cpp:0:0
#16 0x0000000006c1be06 clang::Sema::CheckImplicitConversion(clang::Expr*, clang::QualType, clang::SourceLocation, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1be06)
#17 0x0000000006c1fd19 AnalyzeImplicitConversions(clang::Sema&, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#18 0x0000000006c21ae5 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c21ae5)
#19 0x0000000006fc4277 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc4277)
#20 0x00000000072f904e clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f904e)
#21 0x00000000072f9dd7 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f9dd7)
#22 0x0000000006a98cca clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a98cca)
#23 0x0000000006a9e3eb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9e3eb)
#24 0x0000000006a9f379 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9f379)
#25 0x0000000006aa6966 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa6966)
#26 0x0000000006aa717a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa717a)
#27 0x00000000069a871b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a871b)
#28 0x00000000069e87cd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e87cd)
#29 0x00000000069a0c8b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a0c8b)
#30 0x00000000069a147f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a147f)
#31 0x00000000069ac713 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ac713)
#32 0x00000000069ad625 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ad625)
#33 0x000000000698b2aa clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698b2aa)
#34 0x0000000004bfeac8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfeac8)
#35 0x0000000004ef3475 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef3475)
#36 0x0000000004e7372e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e7372e)
#37 0x0000000004fed63d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fed63d)
#38 0x0000000000ddabbc cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddabbc)
#39 0x0000000000dd160a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#40 0x0000000000dd178d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#41 0x0000000004c70759 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#42 0x00000000041e9ad4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e9ad4)
#43 0x0000000004c70d6f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#44 0x0000000004c32452 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c32452)
#45 0x0000000004c333fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c333fe)
#46 0x0000000004c3a6d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3a6d5)
#47 0x0000000000dd6fb1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd6fb1)
#48 0x0000000000c82094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc82094)
#49 0x00007b9115629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#50 0x00007b9115629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#51 0x0000000000dd10a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd10a5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (AbyssStaror)

<details>
### Link for quick verification: https://godbolt.org/z/4srefcEKh
```C
enum E {
    V1 = (__int128)0x1000000000000,  
    V2 = (V1 &lt;&lt; 64) + 1             

__int128 get_val() {
    return (enum E)__int128(V2 &gt;&gt; 4); 
}
```
### Observations

- Clang 19-20 (assertions):​ Fixed
- Clang 18 and earlier (assertions): crashed in /root/llvm-project/clang/lib/Sema/SemaDecl.cpp
- Clang 21 and later (assertions): crashed in /root/llvm-project/llvm/include/llvm/ADT/APInt.h

## Stack dump
```
clang++: /root/llvm-project/llvm/include/llvm/ADT/APInt.h:787: llvm::APInt&amp; llvm::APInt::operator&lt;&lt;=(unsigned int): Assertion `ShiftAmt &lt;= BitWidth &amp;&amp; "Invalid shift amount"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -O3 &lt;source&gt;
1.	&lt;source&gt;:7:37: current parser token ';'
2.	&lt;source&gt;:6:20: parsing function body 'get_val'
3.	&lt;source&gt;:6:20: in compound statement ('{}')
 #<!-- -->0 0x00000000042a6d18 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42a6d18)
 #<!-- -->1 0x00000000042a4144 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42a4144)
 #<!-- -->2 0x00000000041e9638 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b9115642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b91156969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b9115642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b91156287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b911562871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b9115639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007ac62c4 clang::EnumDecl::getValueRange(llvm::APInt&amp;, llvm::APInt&amp;) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7ac62c4)
#<!-- -->10 0x0000000007ccc8cd (anonymous namespace)::IntExprEvaluator::VisitCastExpr(clang::CastExpr const*) ExprConstant.cpp:0:0
#<!-- -->11 0x0000000007c87b2e clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::IntExprEvaluator, bool&gt;::Visit(clang::Stmt const*) ExprConstant.cpp:0:0
#<!-- -->12 0x0000000007c615be Evaluate(clang::APValue&amp;, (anonymous namespace)::EvalInfo&amp;, clang::Expr const*) ExprConstant.cpp:0:0
#<!-- -->13 0x0000000007c61ebb EvaluateAsRValue((anonymous namespace)::EvalInfo&amp;, clang::Expr const*, clang::APValue&amp;) ExprConstant.cpp:0:0
#<!-- -->14 0x0000000007c67387 clang::Expr::EvaluateAsRValue(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7c67387)
#<!-- -->15 0x0000000006bdc02b TryGetExprRange(clang::ASTContext&amp;, clang::Expr const*, unsigned int, bool, bool) SemaChecking.cpp:0:0
#<!-- -->16 0x0000000006c1be06 clang::Sema::CheckImplicitConversion(clang::Expr*, clang::QualType, clang::SourceLocation, bool*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c1be06)
#<!-- -->17 0x0000000006c1fd19 AnalyzeImplicitConversions(clang::Sema&amp;, clang::Expr*, clang::SourceLocation, bool) (.constprop.0) SemaChecking.cpp:0:0
#<!-- -->18 0x0000000006c21ae5 clang::Sema::CheckCompletedExpr(clang::Expr*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c21ae5)
#<!-- -->19 0x0000000006fc4277 clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6fc4277)
#<!-- -->20 0x00000000072f904e clang::Sema::BuildReturnStmt(clang::SourceLocation, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f904e)
#<!-- -->21 0x00000000072f9dd7 clang::Sema::ActOnReturnStmt(clang::SourceLocation, clang::Expr*, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x72f9dd7)
#<!-- -->22 0x0000000006a98cca clang::Parser::ParseReturnStatement() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a98cca)
#<!-- -->23 0x0000000006a9e3eb clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9e3eb)
#<!-- -->24 0x0000000006a9f379 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a9f379)
#<!-- -->25 0x0000000006aa6966 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa6966)
#<!-- -->26 0x0000000006aa717a clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aa717a)
#<!-- -->27 0x00000000069a871b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a871b)
#<!-- -->28 0x00000000069e87cd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69e87cd)
#<!-- -->29 0x00000000069a0c8b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a0c8b)
#<!-- -->30 0x00000000069a147f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69a147f)
#<!-- -->31 0x00000000069ac713 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ac713)
#<!-- -->32 0x00000000069ad625 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x69ad625)
#<!-- -->33 0x000000000698b2aa clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x698b2aa)
#<!-- -->34 0x0000000004bfeac8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bfeac8)
#<!-- -->35 0x0000000004ef3475 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ef3475)
#<!-- -->36 0x0000000004e7372e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e7372e)
#<!-- -->37 0x0000000004fed63d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4fed63d)
#<!-- -->38 0x0000000000ddabbc cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddabbc)
#<!-- -->39 0x0000000000dd160a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->40 0x0000000000dd178d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->41 0x0000000004c70759 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->42 0x00000000041e9ad4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e9ad4)
#<!-- -->43 0x0000000004c70d6f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->44 0x0000000004c32452 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c32452)
#<!-- -->45 0x0000000004c333fe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c333fe)
#<!-- -->46 0x0000000004c3a6d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3a6d5)
#<!-- -->47 0x0000000000dd6fb1 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd6fb1)
#<!-- -->48 0x0000000000c82094 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc82094)
#<!-- -->49 0x00007b9115629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->50 0x00007b9115629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->51 0x0000000000dd10a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdd10a5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

@efriedma-quic this bisects to 609023213d3fcc35f6ee3d47dceaf37ffa55e66b

---

### Comment 3 - efriedma-quic

This is only very loosely connected to https://github.com/llvm/llvm-project/commit/609023213d3fcc35f6ee3d47dceaf37ffa55e66b .  You can trigger the assertion in other ways.

EnumDecl::getNumPositiveBits() returns "Sets the width in bits required to store all the non-negative enumerators of this enum."  EnumDecl::getValueRange() assumes this can't be larger than the num itself... but currently it can.  Probably Sema::ActOnEnumBody should clamp the value.

---

