# SemaDecl Assertion "Scope shouldn't contain decls!" failed

**Author:** gregbedwell
**URL:** https://github.com/llvm/llvm-project/issues/152983

## Body

I happened to notice this while looking at an unrelated LLVM failure.

The following case compiles successfully:

```cpp
#include <utility>
struct c {
  void d() noexcept(std::is_nothrow_swappable_v< int > &&
                    std::is_nothrow_swappable_v< int >);
};
```

However if I remove the include statement then as well as some genuine-looking errors, I also get an assertion failure:

https://godbolt.org/z/b7orMY3jM

```cpp
struct c {
  void d() noexcept(std::is_nothrow_swappable_v< int > &&
                    std::is_nothrow_swappable_v< int >);
};
```



```console
<source>:4:21: error: use of undeclared identifier 'std'
    4 |   void d() noexcept(std::is_nothrow_swappable_v< int > &&
      |                     ^
<source>:4:54: error: expected '(' for function-style cast or type construction
    4 |   void d() noexcept(std::is_nothrow_swappable_v< int > &&
      |                                                  ~~~ ^
<source>:5:24: error: expected ')'
    5 |                     std::is_nothrow_swappable_v< int >);
      |                        ^
<source>:4:20: note: to match this '('
    4 |   void d() noexcept(std::is_nothrow_swappable_v< int > &&
      |                    ^
clang-22: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDecl.cpp:2241: void clang::Sema::ActOnPopScope(clang::SourceLocation, clang::Scope*): Assertion `(S->getFlags() & (Scope::DeclScope | Scope::TemplateParamScope)) && "Scope shouldn't contain decls!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-trunk-20250811/lib/clang/22 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /opt/compiler-explorer/clang-assertions-trunk-20250811/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-19df18.o -x c++ <source>
1.	<source>:6:2: current parser token ';'
2.	<source>:3:1: parsing struct/union/class body 'c'
 #0 0x000000000400b248 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x400b248)
 #1 0x00000000040080f4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007209a6842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007209a68969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007209a6842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007209a68287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00007209a682871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #7 0x00007209a6839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #8 0x0000000006a61cc9 clang::Sema::ActOnPopScope(clang::SourceLocation, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x6a61cc9)
 #9 0x00000000066ad735 clang::Parser::ExitScope() (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66ad735)
#10 0x00000000066d50d8 clang::Parser::ParseLexedMethodDeclaration(clang::Parser::LateParsedMethodDeclaration&) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66d50d8)
#11 0x00000000066ca8fd clang::Parser::ParseLexedMethodDeclarations(clang::Parser::ParsingClass&) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66ca8fd)
#12 0x000000000671dff2 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x671dff2)
#13 0x0000000006720df0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x6720df0)
#14 0x00000000066f413e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66f413e)
#15 0x00000000066af698 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66af698)
#16 0x00000000066b00cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66b00cf)
#17 0x00000000066b78a3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66b78a3)
#18 0x00000000066b87b5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66b87b5)
#19 0x00000000066b8c70 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66b8c70)
#20 0x00000000066aa983 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66aa983)
#21 0x00000000049b0c98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x49b0c98)
#22 0x0000000004ca50a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x4ca50a5)
#23 0x0000000004c213be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x4c213be)
#24 0x0000000004d967f1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x4d967f1)
#25 0x0000000000daf0bf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0xdaf0bf)
#26 0x0000000000da5d6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#27 0x0000000000daa402 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0xdaa402)
#28 0x0000000000c5f604 main (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0xc5f604)
#29 0x00007209a6829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#30 0x00007209a6829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#31 0x0000000000da5815 _start (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0xda5815)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 254
```




## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Greg Bedwell (gregbedwell)

<details>
I happened to notice this while looking at an unrelated LLVM failure.

The following case compiles successfully:

```
// ================================================================
#include &lt;utility&gt;
struct c {
  void d() noexcept(std::is_nothrow_swappable_v&lt; int &gt; &amp;&amp;
                    std::is_nothrow_swappable_v&lt; int &gt;);
};
// ----------------------------------------------------------------
```

However if I remove the include statement then as well as some genuine-looking errors, I also get an assertion failure:

https://godbolt.org/z/b7orMY3jM

```
// ================================================================
//#include &lt;utility&gt;
struct c {
  void d() noexcept(std::is_nothrow_swappable_v&lt; int &gt; &amp;&amp;
                    std::is_nothrow_swappable_v&lt; int &gt;);
};
// ----------------------------------------------------------------
```

```
&lt;source&gt;:4:21: error: use of undeclared identifier 'std'
    4 |   void d() noexcept(std::is_nothrow_swappable_v&lt; int &gt; &amp;&amp;
      |                     ^
&lt;source&gt;:4:54: error: expected '(' for function-style cast or type construction
    4 |   void d() noexcept(std::is_nothrow_swappable_v&lt; int &gt; &amp;&amp;
      |                                                  ~~~ ^
&lt;source&gt;:5:24: error: expected ')'
    5 |                     std::is_nothrow_swappable_v&lt; int &gt;);
      |                        ^
&lt;source&gt;:4:20: note: to match this '('
    4 |   void d() noexcept(std::is_nothrow_swappable_v&lt; int &gt; &amp;&amp;
      |                    ^
clang-22: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDecl.cpp:2241: void clang::Sema::ActOnPopScope(clang::SourceLocation, clang::Scope*): Assertion `(S-&gt;getFlags() &amp; (Scope::DeclScope | Scope::TemplateParamScope)) &amp;&amp; "Scope shouldn't contain decls!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22 -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -dumpdir /app/output.s- -disable-free -clear-ast-before-backend -main-file-name example.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -fno-verbose-asm -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -debug-info-kind=constructor -dwarf-version=4 -debugger-tuning=gdb -fdebug-compilation-dir=/app -fcoverage-compilation-dir=/app -resource-dir /opt/compiler-explorer/clang-assertions-trunk-20250811/lib/clang/22 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0 -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/x86_64-linux-gnu -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../include/c++/16.0.0/backward -internal-isystem /opt/compiler-explorer/clang-assertions-trunk-20250811/lib/clang/22/include -internal-isystem /usr/local/include -internal-isystem /opt/compiler-explorer/gcc-snapshot/lib/gcc/x86_64-linux-gnu/16.0.0/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -ferror-limit 19 -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/example-19df18.o -x c++ &lt;source&gt;
1.	&lt;source&gt;:6:2: current parser token ';'
2.	&lt;source&gt;:3:1: parsing struct/union/class body 'c'
 #<!-- -->0 0x000000000400b248 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x400b248)
 #<!-- -->1 0x00000000040080f4 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007209a6842520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->3 0x00007209a68969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->4 0x00007209a6842476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->5 0x00007209a68287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->6 0x00007209a682871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->7 0x00007209a6839e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->8 0x0000000006a61cc9 clang::Sema::ActOnPopScope(clang::SourceLocation, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x6a61cc9)
 #<!-- -->9 0x00000000066ad735 clang::Parser::ExitScope() (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66ad735)
#<!-- -->10 0x00000000066d50d8 clang::Parser::ParseLexedMethodDeclaration(clang::Parser::LateParsedMethodDeclaration&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66d50d8)
#<!-- -->11 0x00000000066ca8fd clang::Parser::ParseLexedMethodDeclarations(clang::Parser::ParsingClass&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66ca8fd)
#<!-- -->12 0x000000000671dff2 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x671dff2)
#<!-- -->13 0x0000000006720df0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x6720df0)
#<!-- -->14 0x00000000066f413e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66f413e)
#<!-- -->15 0x00000000066af698 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66af698)
#<!-- -->16 0x00000000066b00cf clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66b00cf)
#<!-- -->17 0x00000000066b78a3 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66b78a3)
#<!-- -->18 0x00000000066b87b5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66b87b5)
#<!-- -->19 0x00000000066b8c70 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66b8c70)
#<!-- -->20 0x00000000066aa983 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x66aa983)
#<!-- -->21 0x00000000049b0c98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x49b0c98)
#<!-- -->22 0x0000000004ca50a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x4ca50a5)
#<!-- -->23 0x0000000004c213be clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x4c213be)
#<!-- -->24 0x0000000004d967f1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0x4d967f1)
#<!-- -->25 0x0000000000daf0bf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0xdaf0bf)
#<!-- -->26 0x0000000000da5d6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->27 0x0000000000daa402 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0xdaa402)
#<!-- -->28 0x0000000000c5f604 main (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0xc5f604)
#<!-- -->29 0x00007209a6829d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->30 0x00007209a6829e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->31 0x0000000000da5815 _start (/opt/compiler-explorer/clang-assertions-trunk-20250811/bin/clang-22+0xda5815)
clang++: error: unable to execute command: Aborted (core dumped)
clang++: error: clang frontend command failed due to signal (use -v to see invocation)
Compiler returned: 254
```



</details>


---

### Comment 2 - Yavnikaa

Hi, I believe the expected behavior here is that clang just emits semantic errors and exit cleanly - no crash/assert.
While playing around, I observed the following:

Clang trunk (clang-22 assertions build) crashes with an assertion in `clang::Sema::ActOnPopScope` while parsing late-parsed member noexcept specifications.
- Replacing the swappability trait with `std::is_nothrow_move_constructible_v` is not crashing.
- `noexcept(undeclared_identifier && undeclared_identifier) ` is not crashing.
- A user-mimicked ADL trait in namespace std did not reproduce the crash.
- A single instance of the given trait or in combination with && constant/true is not crashing.
- `std::is_swappable_v<T>` (non-nothrow variant) reproduces the crash as well.

This suggests the issue is tied to the standard-library swappability trait implementation and how name lookup/instantiation interacts with late parsing of noexcept in class-member declarations.

The crash occurs in `Sema::ActOnPopScope` because the parser pops a `DeclScope` that still contains decls from the failed lookup.

To fix this, I am thinking in the direction of:
- Ensure all noexcept clauses parsed in ParseLexedMethodDeclaration use RAII ParseScope handling so that scopes are always exited cleanly, even on error.
- Avoid adding partial decls to such scopes in Sema::ActOnNoexceptSpec when name lookup fails.

Would love to hear more thoughts and other approaches to address this!

I can try a minimal patch based on this understanding and see if that eliminates the assertion.

PS: I am new here but super-interested and excited to contribute to this space.

---

### Comment 3 - gregbedwell

@Yavnikaa Thanks for taking a look at my bug report!

This is very much not my area of the compiler, so I can't offer anything useful, but hopefully someone in the subscriber list will be able to help.  Otherwise, if you do need to run it past someone, take a look at https://llvm.org/docs/GettingInvolved.html#office-hours 

Otherwise, good luck!  As you said:

> PS: I am new here but super-interested and excited to contribute to this space.

I wanted to say welcome to the community!  I look forward to your future contributions :)

-Greg

---

### Comment 4 - shafik

Looks like the crash started after clang-18: https://godbolt.org/z/WK3n3ev86

So this should be bisected first, it may not change the analysis but it may help to understand the root cause better. 

---

### Comment 5 - shafik

@zyn0217 git bisect points to f52a4679e683807d699e105a6139a5a91401667f

---

