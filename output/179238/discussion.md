# clang segmentation fault with -fopenmp and asm goto

**Author:** iamanonymouscs
**URL:** https://github.com/llvm/llvm-project/issues/179238

## Body

clang segmentation fault with `-fopenmp` and `asm goto`

Compiler explorer (assertion trunk): https://godbolt.org/z/TMbGbx37r

`$ cat small.c`

```c
int main() {
#pragma omp assume
  __asm__ goto("" : : : : a);
  int b = 1;
a:
}
```

```
$ clang-trunk -fopenmp small.c -c
<source>:3:27: error: use of undeclared label 'a'
    3 |   __asm__ goto("" : : : : a);
      |                           ^
<source>:6:1: warning: label at end of compound statement is a C23 extension [-Wc23-extensions]
    6 | }
      | ^
<source>:3:3: error: cannot jump from this asm goto statement to one of its possible targets
    3 |   __asm__ goto("" : : : : a);
      |   ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/lib/clang/23 -internal-isystem /cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/lib/clang/23/include -internal-isystem /cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/../include/x86_64-unknown-linux-gnu -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-68eacf.o -x c <source>
1.	<eof> parser at end of file
2.	<source>:1:12: parsing function body 'main'
 #0 0x0000000004330f88 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x4330f88)
 #1 0x000000000432de64 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x0000732964642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00000000077a2cd3 DiagnoseIndirectOrAsmJumpStmt(clang::Sema&, clang::Stmt*, clang::LabelDecl*, bool&) (.part.0) JumpDiagnostics.cpp:0:0
 #4 0x00000000077a51a3 (anonymous namespace)::JumpScopeChecker::DiagnoseIndirectOrAsmJump(clang::Stmt*, unsigned int, clang::LabelDecl*, unsigned int) JumpDiagnostics.cpp:0:0
 #5 0x00000000077a7c6f clang::Sema::DiagnoseInvalidJumps(clang::Stmt*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x77a7c6f)
 #6 0x0000000006eaaf72 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6eaaf72)
 #7 0x0000000006bc60a4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6bc60a4)
 #8 0x0000000006acb8eb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6acb8eb)
 #9 0x0000000006b01d9d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6b01d9d)
#10 0x0000000006abe9bb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6abe9bb)
#11 0x0000000006abf1af clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6abf1af)
#12 0x0000000006ac71b1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6ac71b1)
#13 0x0000000006ac80c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6ac80c5)
#14 0x0000000006ac84b0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6ac84b0)
#15 0x0000000006aa8da3 clang::ParseAST(clang::Sema&, bool, bool) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6aa8da3)
#16 0x0000000004ca0d58 clang::CodeGenAction::ExecuteAction() (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x4ca0d58)
#17 0x0000000004f9d575 clang::FrontendAction::Execute() (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x4f9d575)
#18 0x0000000004f1cd0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x4f1cd0e)
#19 0x000000000509869d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x509869d)
#20 0x0000000000de144e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0xde144e)
#21 0x0000000000dd7e1a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#22 0x0000000000ddc228 clang_main(int, char**, llvm::ToolContext const&) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0xddc228)
#23 0x0000000000c997b4 main (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0xc997b4)
#24 0x0000732964629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#25 0x0000732964629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#26 0x0000000000dd78b5 _start (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0xdd78b5)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 254
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Anonymous (iamanonymouscs)

<details>
clang segmentation fault with `-fopenmp` and `asm goto`

Compiler explorer (assertion trunk): https://godbolt.org/z/TMbGbx37r

```
$ cat small.c
int main() {
#pragma omp assume
  __asm__ goto("" : : : : a);
  int b = 1;
a:
}
```

```
$ clang-trunk -fopenmp small.c -c
&lt;source&gt;:3:27: error: use of undeclared label 'a'
    3 |   __asm__ goto("" : : : : a);
      |                           ^
&lt;source&gt;:6:1: warning: label at end of compound statement is a C23 extension [-Wc23-extensions]
    6 | }
      | ^
&lt;source&gt;:3:3: error: cannot jump from this asm goto statement to one of its possible targets
    3 |   __asm__ goto("" : : : : a);
      |   ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.c -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=5 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/lib/clang/23 -internal-isystem /cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/lib/clang/23/include -internal-isystem /cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/../include/x86_64-unknown-linux-gnu -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.1/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -ferror-limit 19 -fopenmp -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcolor-diagnostics -faddrsig -fdwarf2-cfi-asm -o /tmp/example-68eacf.o -x c &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:12: parsing function body 'main'
 #<!-- -->0 0x0000000004330f88 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x4330f88)
 #<!-- -->1 0x000000000432de64 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x0000732964642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00000000077a2cd3 DiagnoseIndirectOrAsmJumpStmt(clang::Sema&amp;, clang::Stmt*, clang::LabelDecl*, bool&amp;) (.part.0) JumpDiagnostics.cpp:0:0
 #<!-- -->4 0x00000000077a51a3 (anonymous namespace)::JumpScopeChecker::DiagnoseIndirectOrAsmJump(clang::Stmt*, unsigned int, clang::LabelDecl*, unsigned int) JumpDiagnostics.cpp:0:0
 #<!-- -->5 0x00000000077a7c6f clang::Sema::DiagnoseInvalidJumps(clang::Stmt*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x77a7c6f)
 #<!-- -->6 0x0000000006eaaf72 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool, bool) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6eaaf72)
 #<!-- -->7 0x0000000006bc60a4 clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6bc60a4)
 #<!-- -->8 0x0000000006acb8eb clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6acb8eb)
 #<!-- -->9 0x0000000006b01d9d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6b01d9d)
#<!-- -->10 0x0000000006abe9bb clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6abe9bb)
#<!-- -->11 0x0000000006abf1af clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6abf1af)
#<!-- -->12 0x0000000006ac71b1 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6ac71b1)
#<!-- -->13 0x0000000006ac80c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6ac80c5)
#<!-- -->14 0x0000000006ac84b0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6ac84b0)
#<!-- -->15 0x0000000006aa8da3 clang::ParseAST(clang::Sema&amp;, bool, bool) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x6aa8da3)
#<!-- -->16 0x0000000004ca0d58 clang::CodeGenAction::ExecuteAction() (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x4ca0d58)
#<!-- -->17 0x0000000004f9d575 clang::FrontendAction::Execute() (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x4f9d575)
#<!-- -->18 0x0000000004f1cd0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x4f1cd0e)
#<!-- -->19 0x000000000509869d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0x509869d)
#<!-- -->20 0x0000000000de144e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0xde144e)
#<!-- -->21 0x0000000000dd7e1a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->22 0x0000000000ddc228 clang_main(int, char**, llvm::ToolContext const&amp;) (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0xddc228)
#<!-- -->23 0x0000000000c997b4 main (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0xc997b4)
#<!-- -->24 0x0000732964629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->25 0x0000732964629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->26 0x0000000000dd78b5 _start (/cefs/e8/e865498113d226505147d317_clang-assertions-trunk-20260202/bin/clang-23+0xdd78b5)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 254
```
</details>


---

