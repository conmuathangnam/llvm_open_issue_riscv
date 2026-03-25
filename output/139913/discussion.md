# ICE in clang in getTypoExprState, only in trunk version and using -std=c2x

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/139913
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-06-13T10:45:41Z

## Body

The issue disappear when printf is removed.

## Assertion

```
clang: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaLookup.cpp:5791: const clang::Sema::TypoExprState& clang::Sema::getTypoExprState(clang::TypoExpr*) const: Assertion `Entry != DelayedTypos.end() && "Failed to get the state for a TypoExpr!"' failed.
```

## Stack dump 

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c2x <source>
1.	<source>:5:1: current parser token '}'
2.	<source>:2:12: parsing function body 'main'
3.	<source>:2:12: in compound statement ('{}')
 #0 0x0000000003f95368 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f95368)
 #1 0x0000000003f92ff4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f92ff4)
 #2 0x0000000003ed7c28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e5428c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e5428c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e5428c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e5428c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007e5428c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007e5428c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006d56616 clang::Sema::getTypoExprState(clang::TypoExpr*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6d56616)
#10 0x0000000006c4e767 (anonymous namespace)::TransformTypos::TransformTypoExpr(clang::TypoExpr*) SemaExprCXX.cpp:0:0
#11 0x0000000006c64d29 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformRecoveryExpr(clang::RecoveryExpr*) SemaExprCXX.cpp:0:0
#12 0x0000000006c5b915 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformInitializer(clang::Expr*, bool) (.part.0) SemaExprCXX.cpp:0:0
#13 0x0000000006c5b58e clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl<clang::Expr*>&, bool*) SemaExprCXX.cpp:0:0
#14 0x0000000006c67989 clang::TreeTransform<(anonymous namespace)::TransformTypos>::TransformCallExpr(clang::CallExpr*) SemaExprCXX.cpp:0:0
#15 0x0000000006c82f86 (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&) SemaExprCXX.cpp:0:0
#16 0x0000000006c57c8d clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref<clang::ActionResult<clang::Expr*, true> (clang::Expr*)>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c57c8d)
#17 0x0000000006c5928d clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c5928d)
#18 0x0000000006fb6b7e clang::Sema::ActOnExprStmt(clang::ActionResult<clang::Expr*, true>, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fb6b7e)
#19 0x000000000672f7a4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672f7a4)
#20 0x000000000672729b clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672729b)
#21 0x00000000067280dd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67280dd)
#22 0x000000000672fe03 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672fe03)
#23 0x00000000067305ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67305ca)
#24 0x000000000662a573 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662a573)
#25 0x0000000006660efd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6660efd)
#26 0x000000000661e0ae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661e0ae)
#27 0x000000000661e869 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661e869)
#28 0x000000000662618a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662618a)
#29 0x000000000662712d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662712d)
#30 0x000000000661950a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661950a)
#31 0x000000000493b128 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x493b128)
#32 0x0000000004c31795 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c31795)
#33 0x0000000004bb03ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bb03ce)
#34 0x0000000004d25339 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d25339)
#35 0x0000000000dab05f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdab05f)
#36 0x0000000000da122a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#37 0x00000000049a2379 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x0000000003ed80c4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ed80c4)
#39 0x00000000049a298f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x000000000496475d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x496475d)
#41 0x00000000049657ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49657ee)
#42 0x000000000496dad5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x496dad5)
#43 0x0000000000da6fc8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda6fc8)
#44 0x0000000000c2c8f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc2c8f4)
#45 0x00007e5428c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#46 0x00007e5428c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#47 0x0000000000da0cd5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda0cd5)
```

## Program

```
#include <stdio.h>
int main() {
    int i = 0;
    printf(CONCAT(foo, i, ));
}
```

To quickly reproduce:

It happens only in trunk with -std=c2x

https://godbolt.org/z/9fjjhc6c1


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
The issue disappear when printf is removed.

## Assertion

```
clang: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaLookup.cpp:5791: const clang::Sema::TypoExprState&amp; clang::Sema::getTypoExprState(clang::TypoExpr*) const: Assertion `Entry != DelayedTypos.end() &amp;&amp; "Failed to get the state for a TypoExpr!"' failed.
```

## Stack dump 

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c2x &lt;source&gt;
1.	&lt;source&gt;:5:1: current parser token '}'
2.	&lt;source&gt;:2:12: parsing function body 'main'
3.	&lt;source&gt;:2:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003f95368 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f95368)
 #<!-- -->1 0x0000000003f92ff4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f92ff4)
 #<!-- -->2 0x0000000003ed7c28 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e5428c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e5428c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e5428c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e5428c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007e5428c2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007e5428c39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006d56616 clang::Sema::getTypoExprState(clang::TypoExpr*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6d56616)
#<!-- -->10 0x0000000006c4e767 (anonymous namespace)::TransformTypos::TransformTypoExpr(clang::TypoExpr*) SemaExprCXX.cpp:0:0
#<!-- -->11 0x0000000006c64d29 clang::TreeTransform&lt;(anonymous namespace)::TransformTypos&gt;::TransformRecoveryExpr(clang::RecoveryExpr*) SemaExprCXX.cpp:0:0
#<!-- -->12 0x0000000006c5b915 clang::TreeTransform&lt;(anonymous namespace)::TransformTypos&gt;::TransformInitializer(clang::Expr*, bool) (.part.0) SemaExprCXX.cpp:0:0
#<!-- -->13 0x0000000006c5b58e clang::TreeTransform&lt;(anonymous namespace)::TransformTypos&gt;::TransformExprs(clang::Expr* const*, unsigned int, bool, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, bool*) SemaExprCXX.cpp:0:0
#<!-- -->14 0x0000000006c67989 clang::TreeTransform&lt;(anonymous namespace)::TransformTypos&gt;::TransformCallExpr(clang::CallExpr*) SemaExprCXX.cpp:0:0
#<!-- -->15 0x0000000006c82f86 (anonymous namespace)::TransformTypos::RecursiveTransformLoop(clang::Expr*, bool&amp;) SemaExprCXX.cpp:0:0
#<!-- -->16 0x0000000006c57c8d clang::Sema::CorrectDelayedTyposInExpr(clang::Expr*, clang::VarDecl*, bool, llvm::function_ref&lt;clang::ActionResult&lt;clang::Expr*, true&gt; (clang::Expr*)&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c57c8d)
#<!-- -->17 0x0000000006c5928d clang::Sema::ActOnFinishFullExpr(clang::Expr*, clang::SourceLocation, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6c5928d)
#<!-- -->18 0x0000000006fb6b7e clang::Sema::ActOnExprStmt(clang::ActionResult&lt;clang::Expr*, true&gt;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6fb6b7e)
#<!-- -->19 0x000000000672f7a4 clang::Parser::ParseExprStatement(clang::Parser::ParsedStmtContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672f7a4)
#<!-- -->20 0x000000000672729b clang::Parser::ParseStatementOrDeclarationAfterAttributes(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672729b)
#<!-- -->21 0x00000000067280dd clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67280dd)
#<!-- -->22 0x000000000672fe03 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x672fe03)
#<!-- -->23 0x00000000067305ca clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x67305ca)
#<!-- -->24 0x000000000662a573 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662a573)
#<!-- -->25 0x0000000006660efd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6660efd)
#<!-- -->26 0x000000000661e0ae clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661e0ae)
#<!-- -->27 0x000000000661e869 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661e869)
#<!-- -->28 0x000000000662618a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662618a)
#<!-- -->29 0x000000000662712d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x662712d)
#<!-- -->30 0x000000000661950a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x661950a)
#<!-- -->31 0x000000000493b128 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x493b128)
#<!-- -->32 0x0000000004c31795 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c31795)
#<!-- -->33 0x0000000004bb03ce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4bb03ce)
#<!-- -->34 0x0000000004d25339 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d25339)
#<!-- -->35 0x0000000000dab05f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xdab05f)
#<!-- -->36 0x0000000000da122a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->37 0x00000000049a2379 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->38 0x0000000003ed80c4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3ed80c4)
#<!-- -->39 0x00000000049a298f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->40 0x000000000496475d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x496475d)
#<!-- -->41 0x00000000049657ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49657ee)
#<!-- -->42 0x000000000496dad5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x496dad5)
#<!-- -->43 0x0000000000da6fc8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda6fc8)
#<!-- -->44 0x0000000000c2c8f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc2c8f4)
#<!-- -->45 0x00007e5428c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->46 0x00007e5428c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->47 0x0000000000da0cd5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xda0cd5)
```

## Program

```
#include &lt;stdio.h&gt;
int main() {
    int i = 0;
    printf(CONCAT(foo, i, ));
}
```

To quickly reproduce:

It happens only in trunk with -std=c2x

https://godbolt.org/z/9fjjhc6c1

</details>


---

### Comment 2 - AaronBallman

A smaller reproducer without includes:
```
void func(...);
int main() {
    func(CONCAT(foo, ));
}
```
https://godbolt.org/z/PbEcMz43o

---

### Comment 3 - AaronBallman

The crash is happening because we get into this block: https://github.com/llvm/llvm-project/blob/db099f14d9c80110beb4c1999cfdec7a08ae462d/clang/lib/Sema/TreeTransform.h#L4388

Perhaps "Other forms of copy-initialization will be a no-op if the initializer is already the right type." is correct for C++ but not correct for C? I'm not very familiar with typo correction internals, but the internal state seems to be inconsistent.

---

