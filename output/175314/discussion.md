# clang crashed (segmentation fault) with asm goto

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/175314
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash, inline-asm
**Closed Date:** 2026-01-11T03:12:34Z

## Body

clang segmentation fault with asm goto.

Compiler explorer (assertion trunk): https://godbolt.org/z/P1T74xMoM

```
$ cat small.c
void a(int *b) {
  int __attribute__((cleanup(a))) c = 0;       
  __asm__ goto("" : : : : d);
}
```

```
$ clang -O0 small.c
<source>:3:27: error: use of undeclared label 'd'
    3 |   __asm__ goto("" : : : : d);
      |                           ^
<source>:3:3: error: cannot jump from this asm goto statement to one of its possible targets
    3 |   __asm__ goto("" : : : : d);
      |   ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O0 -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/lib/clang/22 -internal-isystem /cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-dfea3b.o -x c <source>
1.	<source>:5:1: current parser token 'int'
2.	<source>:1:16: parsing function body 'a'
 #0 0x0000000004308bd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x4308bd8)
 #1 0x0000000004305a84 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x000073b015a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x0000000007732a33 DiagnoseIndirectOrAsmJumpStmt(clang::Sema&, clang::Stmt*, clang::LabelDecl*, bool&) (.part.0) JumpDiagnostics.cpp:0:0
 #4 0x0000000007734f03 (anonymous namespace)::JumpScopeChecker::DiagnoseIndirectOrAsmJump(clang::Stmt*, unsigned int, clang::LabelDecl*, unsigned int) JumpDiagnostics.cpp:0:0
 #5 0x00000000077379cf clang::Sema::DiagnoseInvalidJumps(clang::Stmt*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x77379cf)
 #6 0x0000000006e3a5a2 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6e3a5a2)
 #7 0x0000000006b57cc4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6b57cc4)
 #8 0x0000000006a5e0db clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5e0db)
 #9 0x0000000006a9454d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a9454d)
#10 0x0000000006a5123b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5123b)
#11 0x0000000006a51a2f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a51a2f)
#12 0x0000000006a5985a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5985a)
#13 0x0000000006a5a7f5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5a7f5)
#14 0x0000000006a5aca0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5aca0)
#15 0x0000000006a3b843 clang::ParseAST(clang::Sema&, bool, bool) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a3b843)
#16 0x0000000004c6ae28 clang::CodeGenAction::ExecuteAction() (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x4c6ae28)
#17 0x0000000004f64215 clang::FrontendAction::Execute() (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x4f64215)
#18 0x0000000004ee404e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x4ee404e)
#19 0x000000000505e8bd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x505e8bd)
#20 0x0000000000de879e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0xde879e)
#21 0x0000000000ddf16a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#22 0x0000000000de3578 clang_main(int, char**, llvm::ToolContext const&) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0xde3578)
#23 0x0000000000c8ee04 main (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0xc8ee04)
#24 0x000073b015a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x000073b015a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000ddec05 _start (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0xddec05)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 254
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
clang segmentation fault with asm goto.

Compiler explorer (assertion trunk): https://godbolt.org/z/P1T74xMoM

```
$ cat small.c
void a(int *b) {
  int __attribute__((cleanup(a))) c = 0;       
  __asm__ goto("" : : : : d);
}
```

```
$ clang -O0 small.c
&lt;source&gt;:3:27: error: use of undeclared label 'd'
    3 |   __asm__ goto("" : : : : d);
      |                           ^
&lt;source&gt;:3:3: error: cannot jump from this asm goto statement to one of its possible targets
    3 |   __asm__ goto("" : : : : d);
      |   ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -O0 -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/lib/clang/22 -internal-isystem /cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-dfea3b.o -x c &lt;source&gt;
1.	&lt;source&gt;:5:1: current parser token 'int'
2.	&lt;source&gt;:1:16: parsing function body 'a'
 #<!-- -->0 0x0000000004308bd8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x4308bd8)
 #<!-- -->1 0x0000000004305a84 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x000073b015a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x0000000007732a33 DiagnoseIndirectOrAsmJumpStmt(clang::Sema&amp;, clang::Stmt*, clang::LabelDecl*, bool&amp;) (.part.0) JumpDiagnostics.cpp:0:0
 #<!-- -->4 0x0000000007734f03 (anonymous namespace)::JumpScopeChecker::DiagnoseIndirectOrAsmJump(clang::Stmt*, unsigned int, clang::LabelDecl*, unsigned int) JumpDiagnostics.cpp:0:0
 #<!-- -->5 0x00000000077379cf clang::Sema::DiagnoseInvalidJumps(clang::Stmt*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x77379cf)
 #<!-- -->6 0x0000000006e3a5a2 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6e3a5a2)
 #<!-- -->7 0x0000000006b57cc4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6b57cc4)
 #<!-- -->8 0x0000000006a5e0db clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5e0db)
 #<!-- -->9 0x0000000006a9454d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a9454d)
#<!-- -->10 0x0000000006a5123b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5123b)
#<!-- -->11 0x0000000006a51a2f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a51a2f)
#<!-- -->12 0x0000000006a5985a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5985a)
#<!-- -->13 0x0000000006a5a7f5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5a7f5)
#<!-- -->14 0x0000000006a5aca0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a5aca0)
#<!-- -->15 0x0000000006a3b843 clang::ParseAST(clang::Sema&amp;, bool, bool) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x6a3b843)
#<!-- -->16 0x0000000004c6ae28 clang::CodeGenAction::ExecuteAction() (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x4c6ae28)
#<!-- -->17 0x0000000004f64215 clang::FrontendAction::Execute() (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x4f64215)
#<!-- -->18 0x0000000004ee404e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x4ee404e)
#<!-- -->19 0x000000000505e8bd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0x505e8bd)
#<!-- -->20 0x0000000000de879e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0xde879e)
#<!-- -->21 0x0000000000ddf16a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->22 0x0000000000de3578 clang_main(int, char**, llvm::ToolContext const&amp;) (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0xde3578)
#<!-- -->23 0x0000000000c8ee04 main (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0xc8ee04)
#<!-- -->24 0x000073b015a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x000073b015a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x0000000000ddec05 _start (/cefs/ce/cea0093d8bf0215dbf8aa30a_clang-assertions-trunk-20260110/bin/clang-22+0xddec05)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 254
```
</details>


---

### Comment 2 - shafik

Is this fuzzer generated? Please make sure you identify it as such. 



---

