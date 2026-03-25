# segfault in `clang::Lexer::Lex`

**Author:** kesyog
**URL:** https://github.com/llvm/llvm-project/issues/167905
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-12-03T15:24:35Z

## Body

The odd snippet below causes a segfault in clang's front-end: 

```cpp
namespace a {b c (a:c::
```

https://godbolt.org/z/eYMf1e6er

#### Backtrace

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/lib/clang/22 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-48912a.o -x c++ <source>
1.	<source>:1:24: current parser token ''
2.	<source>:1:1: parsing namespace 'a'
 #0 0x0000000003d364f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x3d364f8)
 #1 0x0000000003d336c4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000705e05842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000000007a0c717 clang::Lexer::Lex(clang::Token&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7a0c717)
 #4 0x0000000007ab8b6c clang::Preprocessor::Lex(clang::Token&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7ab8b6c)
 #5 0x0000000007a583a8 clang::Preprocessor::CLK_CachingLexer(clang::Preprocessor&, clang::Token&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7a583a8)
 #6 0x0000000007ab8b6c clang::Preprocessor::Lex(clang::Token&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7ab8b6c)
 #7 0x00000000063e41eb clang::Parser::TryConsumeDeclarationSpecifier() (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x63e41eb)
 #8 0x00000000063e7662 clang::Parser::TryParseParameterDeclarationClause(bool*, bool, clang::ImplicitTypenameContext) (.part.0) ParseTentative.cpp:0:0
 #9 0x00000000063e7b54 clang::Parser::isCXXFunctionDeclarator(bool*, clang::ImplicitTypenameContext) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x63e7b54)
#10 0x000000000630acd1 clang::Parser::ParseDirectDeclarator(clang::Declarator&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x630acd1)
#11 0x0000000006303bce clang::Parser::ParseDeclaratorInternal(clang::Declarator&, void (clang::Parser::*)(clang::Declarator&)) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x6303bce)
#12 0x0000000007b299f1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7b299f1)
#13 0x00000000062ffd74 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62ffd74)
#14 0x00000000062acddf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62acddf)
#15 0x00000000062ad517 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62ad517)
#16 0x00000000062b5eef clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62b5eef)
#17 0x000000000631df2d clang::Parser::ParseInnerNamespace(llvm::SmallVector<clang::Parser::InnerNamespaceInfo, 4u> const&, unsigned int, clang::SourceLocation&, clang::ParsedAttributes&, clang::BalancedDelimiterTracker&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x631df2d)
#18 0x00000000063215dd clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&, clang::SourceLocation) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x63215dd)
#19 0x0000000006302e61 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x6302e61)
#20 0x00000000062b5f21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62b5f21)
#21 0x00000000062b76f7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62b76f7)
#22 0x00000000062b7c40 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62b7c40)
#23 0x00000000062905b3 clang::ParseAST(clang::Sema&, bool, bool) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62905b3)
#24 0x000000000460db05 clang::CodeGenAction::ExecuteAction() (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x460db05)
#25 0x000000000490b90a clang::FrontendAction::Execute() (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x490b90a)
#26 0x000000000488af9b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x488af9b)
#27 0x0000000004a0301b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x4a0301b)
#28 0x0000000000de1a25 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0xde1a25)
#29 0x0000000000dd998b ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#30 0x0000000000ddc90f clang_main(int, char**, llvm::ToolContext const&) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0xddc90f)
#31 0x0000000000c88614 main (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0xc88614)
#32 0x0000705e05829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#33 0x0000705e05829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#34 0x0000000000dd9425 _start (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0xdd9425)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```

#### clang version

```
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 362119d03cb5156599825cd43ceff65b4dd54068)
Target: x86_64-unknown-linux-gnu
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Kesavan Yogeswaran (kesyog)

<details>
The odd snippet below causes a segfault in clang's front-end: 

```cpp
namespace a {b c (a:c::
```

https://godbolt.org/z/eYMf1e6er

#### Backtrace

```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/lib/clang/22 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-48912a.o -x c++ &lt;source&gt;
1.	&lt;source&gt;:1:24: current parser token ''
2.	&lt;source&gt;:1:1: parsing namespace 'a'
 #<!-- -->0 0x0000000003d364f8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x3d364f8)
 #<!-- -->1 0x0000000003d336c4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000705e05842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000000007a0c717 clang::Lexer::Lex(clang::Token&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7a0c717)
 #<!-- -->4 0x0000000007ab8b6c clang::Preprocessor::Lex(clang::Token&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7ab8b6c)
 #<!-- -->5 0x0000000007a583a8 clang::Preprocessor::CLK_CachingLexer(clang::Preprocessor&amp;, clang::Token&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7a583a8)
 #<!-- -->6 0x0000000007ab8b6c clang::Preprocessor::Lex(clang::Token&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7ab8b6c)
 #<!-- -->7 0x00000000063e41eb clang::Parser::TryConsumeDeclarationSpecifier() (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x63e41eb)
 #<!-- -->8 0x00000000063e7662 clang::Parser::TryParseParameterDeclarationClause(bool*, bool, clang::ImplicitTypenameContext) (.part.0) ParseTentative.cpp:0:0
 #<!-- -->9 0x00000000063e7b54 clang::Parser::isCXXFunctionDeclarator(bool*, clang::ImplicitTypenameContext) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x63e7b54)
#<!-- -->10 0x000000000630acd1 clang::Parser::ParseDirectDeclarator(clang::Declarator&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x630acd1)
#<!-- -->11 0x0000000006303bce clang::Parser::ParseDeclaratorInternal(clang::Declarator&amp;, void (clang::Parser::*)(clang::Declarator&amp;)) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x6303bce)
#<!-- -->12 0x0000000007b299f1 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x7b299f1)
#<!-- -->13 0x00000000062ffd74 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62ffd74)
#<!-- -->14 0x00000000062acddf clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62acddf)
#<!-- -->15 0x00000000062ad517 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62ad517)
#<!-- -->16 0x00000000062b5eef clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62b5eef)
#<!-- -->17 0x000000000631df2d clang::Parser::ParseInnerNamespace(llvm::SmallVector&lt;clang::Parser::InnerNamespaceInfo, 4u&gt; const&amp;, unsigned int, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::BalancedDelimiterTracker&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x631df2d)
#<!-- -->18 0x00000000063215dd clang::Parser::ParseNamespace(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::SourceLocation) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x63215dd)
#<!-- -->19 0x0000000006302e61 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x6302e61)
#<!-- -->20 0x00000000062b5f21 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62b5f21)
#<!-- -->21 0x00000000062b76f7 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62b76f7)
#<!-- -->22 0x00000000062b7c40 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62b7c40)
#<!-- -->23 0x00000000062905b3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x62905b3)
#<!-- -->24 0x000000000460db05 clang::CodeGenAction::ExecuteAction() (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x460db05)
#<!-- -->25 0x000000000490b90a clang::FrontendAction::Execute() (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x490b90a)
#<!-- -->26 0x000000000488af9b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x488af9b)
#<!-- -->27 0x0000000004a0301b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0x4a0301b)
#<!-- -->28 0x0000000000de1a25 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0xde1a25)
#<!-- -->29 0x0000000000dd998b ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->30 0x0000000000ddc90f clang_main(int, char**, llvm::ToolContext const&amp;) (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0xddc90f)
#<!-- -->31 0x0000000000c88614 main (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0xc88614)
#<!-- -->32 0x0000705e05829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->33 0x0000705e05829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->34 0x0000000000dd9425 _start (/cefs/f7/f7232975acf0cbdf306b27c4_clang-trunk-20251112/bin/clang-22+0xdd9425)
clang++: error: unable to execute command: Segmentation fault (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
```

#### clang version

```
clang version 22.0.0git (https://github.com/llvm/llvm-project.git 362119d03cb5156599825cd43ceff65b4dd54068)
Target: x86_64-unknown-linux-gnu
```
</details>


---

### Comment 2 - ebinjose02

Hi, I would like to work on this issue.

---

