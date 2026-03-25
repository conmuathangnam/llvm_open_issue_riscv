# [clang]Clang assertion failure when parsing editor placeholder

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/168517

## Body

The following code has been causing Clang to crash since version 5.0.0 and still does in the current trunk (assertions).

```cpp
class A {};
struct <#struct name#> {};
A a_<#struct name#> = A{<#struct name#>{}};
```

**Output:**  

```
clang++: warning: argument unused during compilation: '-S' [-Wunused-command-line-argument]
<source>:2:8: error: editor placeholder in source file
    2 | struct <#struct name#> {};
      |        ^
<source>:3:5: error: editor placeholder in source file
    3 | A a_<#struct name#> = A{<#struct name#>{}};
      |     ^
<source>:3:5: error: expected ';' after top level declarator
    3 | A a_<#struct name#> = A{<#struct name#>{}};
      |     ^
      |     ;
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Parse/Parser.h:263: clang::SourceLocation clang::Parser::ConsumeToken(): Assertion `!isTokenSpecial() && "Should consume special tokens with Consume*Token"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsyntax-only -Xclang -ast-dump -std=c++17 <source>
1.	<source>:3:5: at annotation token
 #0 0x0000000004240168 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4240168)
 #1 0x000000000423d594 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423d594)
 #2 0x0000000004181358 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000742d95e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000742d95e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000742d95e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000742d95e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000742d95e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000742d95e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000691e5ff clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691e5ff)
#10 0x0000000006929928 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6929928)
#11 0x000000000692a105 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692a105)
#12 0x000000000690b6ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690b6ea)
#13 0x0000000004e759f5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e759f5)
#14 0x0000000004df5bce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df5bce)
#15 0x0000000004f6d91d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6d91d)
#16 0x0000000000dc4ff0 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4ff0)
#17 0x0000000000dbba6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#18 0x0000000000dbbbed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#19 0x0000000004bf11c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#20 0x00000000041817f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41817f4)
#21 0x0000000004bf17df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#22 0x0000000004bb2ce2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb2ce2)
#23 0x0000000004bb3c8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb3c8e)
#24 0x0000000004bbb305 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbb305)
#25 0x0000000000dc1411 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc1411)
#26 0x0000000000c707c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc707c4)
#27 0x0000742d95e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#28 0x0000742d95e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#29 0x0000000000dbb505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbb505)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

see: https://godbolt.org/z/Khce6bhz9

This happens in the parsing phase before Sema is reached. The special tokens <#...#> appear in code snippets generated by IDEs (such as Xcode), so Clang should treat them as invalid identifiers rather than asserting.



## Comments

### Comment 1 - JasonHonKL

Mind to assign me to this issue ?

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
The following code has been causing Clang to crash since version 5.0.0 and still does in the current trunk (assertions).

```cpp
class A {};
struct &lt;#struct name#&gt; {};
A a_&lt;#struct name#&gt; = A{&lt;#struct name#&gt;{}};
```

**Output:**  

```
clang++: warning: argument unused during compilation: '-S' [-Wunused-command-line-argument]
&lt;source&gt;:2:8: error: editor placeholder in source file
    2 | struct &lt;#struct name#&gt; {};
      |        ^
&lt;source&gt;:3:5: error: editor placeholder in source file
    3 | A a_&lt;#struct name#&gt; = A{&lt;#struct name#&gt;{}};
      |     ^
&lt;source&gt;:3:5: error: expected ';' after top level declarator
    3 | A a_&lt;#struct name#&gt; = A{&lt;#struct name#&gt;{}};
      |     ^
      |     ;
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Parse/Parser.h:263: clang::SourceLocation clang::Parser::ConsumeToken(): Assertion `!isTokenSpecial() &amp;&amp; "Should consume special tokens with Consume*Token"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsyntax-only -Xclang -ast-dump -std=c++17 &lt;source&gt;
1.	&lt;source&gt;:3:5: at annotation token
 #<!-- -->0 0x0000000004240168 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4240168)
 #<!-- -->1 0x000000000423d594 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x423d594)
 #<!-- -->2 0x0000000004181358 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000742d95e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000742d95e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000742d95e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000742d95e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000742d95e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000742d95e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000691e5ff clang::Parser::ConsumeToken() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x691e5ff)
#<!-- -->10 0x0000000006929928 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6929928)
#<!-- -->11 0x000000000692a105 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x692a105)
#<!-- -->12 0x000000000690b6ea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x690b6ea)
#<!-- -->13 0x0000000004e759f5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e759f5)
#<!-- -->14 0x0000000004df5bce clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4df5bce)
#<!-- -->15 0x0000000004f6d91d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f6d91d)
#<!-- -->16 0x0000000000dc4ff0 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc4ff0)
#<!-- -->17 0x0000000000dbba6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->18 0x0000000000dbbbed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->19 0x0000000004bf11c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->20 0x00000000041817f4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41817f4)
#<!-- -->21 0x0000000004bf17df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->22 0x0000000004bb2ce2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb2ce2)
#<!-- -->23 0x0000000004bb3c8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb3c8e)
#<!-- -->24 0x0000000004bbb305 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbb305)
#<!-- -->25 0x0000000000dc1411 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc1411)
#<!-- -->26 0x0000000000c707c4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc707c4)
#<!-- -->27 0x0000742d95e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->28 0x0000742d95e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->29 0x0000000000dbb505 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbb505)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

see: https://godbolt.org/z/Khce6bhz9

This happens in the parsing phase before Sema is reached. The special tokens &lt;#...#&gt; appear in code snippets generated by IDEs (such as Xcode), so Clang should treat them as invalid identifiers rather than asserting.


</details>


---

### Comment 3 - JasonHonKL

@Attacker23 Hi sorry for asking some idiotic question haha not really familiar with the codebase. However it seems I'm not really able to reproduce the issue. Mind to tell me how you produce it ? Thanks. 
```
jason: test % clang++ -S main.cpp
main.cpp:2:7: error: editor placeholder in source file
    2 | struct<#struct name #> {};
      |       ^
main.cpp:3:5: error: editor placeholder in source file
    3 | A a_<#struct name #> = A {
      |     ^
main.cpp:3:5: error: expected ';' after top level declarator
    3 | A a_<#struct name #> = A {
      |     ^
      |     ;
main.cpp:3:22: error: expected unqualified-id
    3 | A a_<#struct name #> = A {
      |                      ^
main.cpp:4:3: error: editor placeholder in source file
    4 |   <#struct name #> {}
      |   ^
5 errors generated.
```

---

### Comment 4 - Attacker23

> @Attacker23 Hi sorry for asking some idiotic question haha not really familiar with the codebase. However it seems I'm not really able to reproduce the issue. Mind to tell me how you produce it ? Thanks. 
> ```
> jason: test % clang++ -S main.cpp
> main.cpp:2:7: error: editor placeholder in source file
>     2 | struct<#struct name #> {};
>       |       ^
> main.cpp:3:5: error: editor placeholder in source file
>     3 | A a_<#struct name #> = A {
>       |     ^
> main.cpp:3:5: error: expected ';' after top level declarator
>     3 | A a_<#struct name #> = A {
>       |     ^
>       |     ;
> main.cpp:3:22: error: expected unqualified-id
>     3 | A a_<#struct name #> = A {
>       |                      ^
> main.cpp:4:3: error: editor placeholder in source file
>     4 |   <#struct name #> {}
>       |   ^
> 5 errors generated.
> ```

Thanks for checking! I’ve actually run into the same issue before. Have you tried enabling clang assertions? The crash only happens when assertions are turned on🥰


---

### Comment 5 - Attacker23

@JasonHonKL 

To enable assertions, you’ll need to add the `-DLLVM_ENABLE_ASSERTIONS=ON` flag when running CMake. For example:  

```bash
cmake -DLLVM_ENABLE_ASSERTIONS=ON -G "Ninja" ../llvm
```

Then rebuild Clang, e.g.:  

```bash
ninja clang
```

You can verify it worked by checking `clang --version` — it should say *“assertions enabled”* in the output.  

🥰🥰🥰

---

### Comment 6 - JasonHonKL

Thanks a lot for the guidance. I will take a look with this tonight. Really thanks a lot !!!

---

### Comment 7 - JasonHonKL

Hmmm i tried seems still can't reproduce the bug :((((((((( 
 

---

### Comment 8 - Attacker23

> Hmmm i tried seems still can't reproduce the bug :((((((((( 
>  

@JasonHonKL 
Could you maybe share a bit more info?  
I’m afraid I might not be able to help much even then, since I’m not very familiar with compilers haha.  
Anyway, really appreciate your effort!
🥰🥰🥰

---

