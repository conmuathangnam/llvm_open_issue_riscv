# [Clang] Assertion error in getParamDecl() starting from clang-20

**Author:** hax0kartik
**URL:** https://github.com/llvm/llvm-project/issues/177741
**Status:** Closed
**Labels:** clang:frontend, confirmed, crash, regression:20
**Closed Date:** 2026-01-29T01:13:18Z

## Body

Hi, while playing around with the testcase for #176639, I've noticed that there is a separate assertion error when a `static` keyword is involved.


Reproducer: [https://godbolt.org/z/8oETxqYzd](https://godbolt.org/z/8oETxqYzd)

```cpp
struct S {
  static int operator()(this S) {
    
  }
};

void foo() {
  S s{};
  s(0);
}
```

Assertion:
```
/root/llvm-project/llvm/tools/clang/include/clang/AST/Decl.h:2802: clang::ParmVarDecl* clang::FunctionDecl::getParamDecl(unsigned int): Assertion `i < getNumParams() && "Illegal param #"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/lib/clang/23 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1/backward -internal-isystem /cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-eb83e9.o -x c++ <source>
1.	<source>:10:6: current parser token ')'
2.	<source>:8:12: parsing function body 'foo'
3.	<source>:8:12: in compound statement ('{}'
```

Backtrace:
```
#0 0x0000000004309af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x4309af8)
 #1 0x00000000043069d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000788e6a642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000788e6a6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x0000788e6a642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x0000788e6a6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x0000788e6a62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x0000788e6a639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000006caafa2 (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6caafa2)
 #9 0x0000000007365abb DiagnoseBadConversion(clang::Sema&, clang::OverloadCandidate*, unsigned int, bool) SemaOverload.cpp:0:0
#10 0x0000000007392420 clang::OverloadCandidateSet::NoteCandidates(clang::Sema&, llvm::ArrayRef<clang::Expr*>, llvm::ArrayRef<clang::OverloadCandidate*>, llvm::StringRef, clang::SourceLocation) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x7392420)
#11 0x0000000007392e81 clang::OverloadCandidateSet::NoteCandidates(std::pair<clang::SourceLocation, clang::PartialDiagnostic>, clang::Sema&, clang::OverloadCandidateDisplayKind, llvm::ArrayRef<clang::Expr*>, llvm::StringRef, clang::SourceLocation, llvm::function_ref<bool (clang::OverloadCandidate&)>) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x7392e81)
#12 0x000000000739ec7b clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x739ec7b)
#13 0x0000000006ff6604 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6ff6604)
#14 0x0000000006ffb55c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6ffb55c)
#15 0x0000000006af9737 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult<clang::Expr*, true>) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af9737)
#16 0x0000000006af264a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af264a)
#17 0x0000000006af48a7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af48a7)
#18 0x0000000006af4939 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af4939)
#19 0x0000000006af8f19 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af8f19)
#20 0x0000000006b7f439 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b7f439)
#21 0x0000000006b768e2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::LabelDecl*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b768e2)
#22 0x0000000006b772c6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b772c6)
#23 0x0000000006b7fbb6 clang::Parser::ParseCompoundStatementBody(bool) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b7fbb6)
#24 0x0000000006b803ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b803ca)
#25 0x0000000006a85c0b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a85c0b)
#26 0x0000000006abc07d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6abc07d)
#27 0x0000000006a78cdb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a78cdb)
#28 0x0000000006a794cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a794cf)
#29 0x0000000006a814d1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a814d1)
#30 0x0000000006a823e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a823e5)
#31 0x0000000006a630fa clang::ParseAST(clang::Sema&, bool, bool) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a630fa)
#32 0x0000000004c731e8 clang::CodeGenAction::ExecuteAction() (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x4c731e8)
#33 0x0000000004f6e525 clang::FrontendAction::Execute() (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x4f6e525)
#34 0x0000000004eedf4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x4eedf4e)
#35 0x00000000050696ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x50696ed)
#36 0x0000000000ddd95e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0xddd95e)
#37 0x0000000000dd432a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#38 0x0000000000dd8738 clang_main(int, char**, llvm::ToolContext const&) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0xdd8738)
#39 0x0000000000c96d24 main (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0xc96d24)
#40 0x0000788e6a629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x0000788e6a629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000dd3dc5 _start (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0xdd3dc5)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```

Goes back to `clang 20.1.0`

## Comments

### Comment 1 - hax0kartik

This bisects to f7e9d48a73dd68c8b652692d8a9e559a6ceb722e.

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Kartik (hax0kartik)

<details>
Hi, while playing around with the testcase for #<!-- -->176639, I've noticed that there is a separate assertion error when a `static` keyword is involved.


Reproducer: [https://godbolt.org/z/8oETxqYzd](https://godbolt.org/z/8oETxqYzd)

Assertion:
```
/root/llvm-project/llvm/tools/clang/include/clang/AST/Decl.h:2802: clang::ParmVarDecl* clang::FunctionDecl::getParamDecl(unsigned int): Assertion `i &lt; getNumParams() &amp;&amp; "Illegal param #"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/lib/clang/23 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../include/c++/16.0.1/backward -internal-isystem /cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/lib/clang/23/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -std=c++23 -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fno-implicit-modules -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-eb83e9.o -x c++ &lt;source&gt;
1.	&lt;source&gt;:10:6: current parser token ')'
2.	&lt;source&gt;:8:12: parsing function body 'foo'
3.	&lt;source&gt;:8:12: in compound statement ('{}'
```

Backtrace:
```
#<!-- -->0 0x0000000004309af8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x4309af8)
 #<!-- -->1 0x00000000043069d4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000788e6a642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000788e6a6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x0000788e6a642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x0000788e6a6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x0000788e6a62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x0000788e6a639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000006caafa2 (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6caafa2)
 #<!-- -->9 0x0000000007365abb DiagnoseBadConversion(clang::Sema&amp;, clang::OverloadCandidate*, unsigned int, bool) SemaOverload.cpp:0:0
#<!-- -->10 0x0000000007392420 clang::OverloadCandidateSet::NoteCandidates(clang::Sema&amp;, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::ArrayRef&lt;clang::OverloadCandidate*&gt;, llvm::StringRef, clang::SourceLocation) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x7392420)
#<!-- -->11 0x0000000007392e81 clang::OverloadCandidateSet::NoteCandidates(std::pair&lt;clang::SourceLocation, clang::PartialDiagnostic&gt;, clang::Sema&amp;, clang::OverloadCandidateDisplayKind, llvm::ArrayRef&lt;clang::Expr*&gt;, llvm::StringRef, clang::SourceLocation, llvm::function_ref&lt;bool (clang::OverloadCandidate&amp;)&gt;) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x7392e81)
#<!-- -->12 0x000000000739ec7b clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x739ec7b)
#<!-- -->13 0x0000000006ff6604 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6ff6604)
#<!-- -->14 0x0000000006ffb55c clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6ffb55c)
#<!-- -->15 0x0000000006af9737 clang::Parser::ParsePostfixExpressionSuffix(clang::ActionResult&lt;clang::Expr*, true&gt;) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af9737)
#<!-- -->16 0x0000000006af264a clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af264a)
#<!-- -->17 0x0000000006af48a7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af48a7)
#<!-- -->18 0x0000000006af4939 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af4939)
#<!-- -->19 0x0000000006af8f19 clang::Parser::ParseExpression(clang::TypoCorrectionTypeBehavior) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6af8f19)
#<!-- -->20 0x0000000006b7f439 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b7f439)
#<!-- -->21 0x0000000006b768e2 clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::LabelDecl*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b768e2)
#<!-- -->22 0x0000000006b772c6 clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::LabelDecl*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b772c6)
#<!-- -->23 0x0000000006b7fbb6 clang::Parser::ParseCompoundStatementBody(bool) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b7fbb6)
#<!-- -->24 0x0000000006b803ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6b803ca)
#<!-- -->25 0x0000000006a85c0b clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a85c0b)
#<!-- -->26 0x0000000006abc07d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6abc07d)
#<!-- -->27 0x0000000006a78cdb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a78cdb)
#<!-- -->28 0x0000000006a794cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a794cf)
#<!-- -->29 0x0000000006a814d1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a814d1)
#<!-- -->30 0x0000000006a823e5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a823e5)
#<!-- -->31 0x0000000006a630fa clang::ParseAST(clang::Sema&amp;, bool, bool) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x6a630fa)
#<!-- -->32 0x0000000004c731e8 clang::CodeGenAction::ExecuteAction() (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x4c731e8)
#<!-- -->33 0x0000000004f6e525 clang::FrontendAction::Execute() (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x4f6e525)
#<!-- -->34 0x0000000004eedf4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x4eedf4e)
#<!-- -->35 0x00000000050696ed clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0x50696ed)
#<!-- -->36 0x0000000000ddd95e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0xddd95e)
#<!-- -->37 0x0000000000dd432a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->38 0x0000000000dd8738 clang_main(int, char**, llvm::ToolContext const&amp;) (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0xdd8738)
#<!-- -->39 0x0000000000c96d24 main (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0xc96d24)
#<!-- -->40 0x0000788e6a629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x0000788e6a629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000dd3dc5 _start (/cefs/37/37fa0666bafd90121a73c22e_clang-assertions-trunk-20260124/bin/clang-23+0xdd3dc5)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```

Goes back to `clang 20.1.0`
</details>


---

### Comment 3 - shafik

Please include the source code in the issue next time.

---

