# Clang sequencial incorrect #pragma specification end in ICE since 3.5

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/140687

## Body

## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Parse/ParseStmt.cpp:80: clang::StmtResult clang::Parser::ParseStatementOrDeclaration(clang::Parser::StmtVector&, clang::Parser::ParsedStmtContext, clang::SourceLocation*): Assertion `(CXX11Attrs.empty() || Res.isInvalid() || Res.isUsable()) && "attributes on empty statement"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:4:1: current parser token '}'
2.	<source>:1:12: parsing function body 'main'
3.	<source>:1:12: in compound statement ('{}')
 #0 0x0000000003f9b608 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9b608)
 #1 0x0000000003f99294 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f99294)
 #2 0x0000000003eddec8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000718aa3e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000718aa3e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000718aa3e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000718aa3e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000718aa3e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000718aa3e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000673d29f clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector<clang::Stmt*, 24u>&, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673d29f)
#10 0x0000000006744cf3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6744cf3)
#11 0x00000000067454ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67454ba)
#12 0x000000000663f4f3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663f4f3)
#13 0x0000000006675e8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6675e8d)
#14 0x000000000663303e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663303e)
#15 0x00000000066337f9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66337f9)
#16 0x000000000663b10a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663b10a)
#17 0x000000000663c0ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663c0ad)
#18 0x000000000663c560 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663c560)
#19 0x000000000662e443 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e443)
#20 0x00000000049463b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49463b8)
#21 0x0000000004c3e1d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3e1d5)
#22 0x0000000004bbce1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbce1e)
#23 0x0000000004d30e49 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d30e49)
#24 0x0000000000dac8df cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac8df)
#25 0x0000000000da2aaa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x00000000049ad4b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x0000000003ede364 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ede364)
#28 0x00000000049adacf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x000000000496f89d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496f89d)
#30 0x000000000497092e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497092e)
#31 0x0000000004978c15 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4978c15)
#32 0x0000000000da8848 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8848)
#33 0x0000000000c2e214 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e214)
#34 0x0000718aa3e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x0000718aa3e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000da2555 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2555)
```

## Program

```
int main() {
    #pragma clang loop vectorize(enable) unroll(enable)
    #pragma pack(push, 8)
}
```

## To quickly reproduce

https://godbolt.org/z/nnoWb6jcb

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
## Assertion

```
clang++: /root/llvm-project/llvm/tools/clang/lib/Parse/ParseStmt.cpp:80: clang::StmtResult clang::Parser::ParseStatementOrDeclaration(clang::Parser::StmtVector&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*): Assertion `(CXX11Attrs.empty() || Res.isInvalid() || Res.isUsable()) &amp;&amp; "attributes on empty statement"' failed.
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:4:1: current parser token '}'
2.	&lt;source&gt;:1:12: parsing function body 'main'
3.	&lt;source&gt;:1:12: in compound statement ('{}')
 #<!-- -->0 0x0000000003f9b608 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f9b608)
 #<!-- -->1 0x0000000003f99294 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f99294)
 #<!-- -->2 0x0000000003eddec8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000718aa3e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000718aa3e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000718aa3e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000718aa3e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000718aa3e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000718aa3e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000673d29f clang::Parser::ParseStatementOrDeclaration(llvm::SmallVector&lt;clang::Stmt*, 24u&gt;&amp;, clang::Parser::ParsedStmtContext, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x673d29f)
#<!-- -->10 0x0000000006744cf3 clang::Parser::ParseCompoundStatementBody(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6744cf3)
#<!-- -->11 0x00000000067454ba clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x67454ba)
#<!-- -->12 0x000000000663f4f3 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663f4f3)
#<!-- -->13 0x0000000006675e8d clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6675e8d)
#<!-- -->14 0x000000000663303e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663303e)
#<!-- -->15 0x00000000066337f9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66337f9)
#<!-- -->16 0x000000000663b10a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663b10a)
#<!-- -->17 0x000000000663c0ad clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663c0ad)
#<!-- -->18 0x000000000663c560 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x663c560)
#<!-- -->19 0x000000000662e443 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662e443)
#<!-- -->20 0x00000000049463b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49463b8)
#<!-- -->21 0x0000000004c3e1d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c3e1d5)
#<!-- -->22 0x0000000004bbce1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bbce1e)
#<!-- -->23 0x0000000004d30e49 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d30e49)
#<!-- -->24 0x0000000000dac8df cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdac8df)
#<!-- -->25 0x0000000000da2aaa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->26 0x00000000049ad4b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x0000000003ede364 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ede364)
#<!-- -->28 0x00000000049adacf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x000000000496f89d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496f89d)
#<!-- -->30 0x000000000497092e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497092e)
#<!-- -->31 0x0000000004978c15 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4978c15)
#<!-- -->32 0x0000000000da8848 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda8848)
#<!-- -->33 0x0000000000c2e214 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2e214)
#<!-- -->34 0x0000718aa3e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x0000718aa3e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000da2555 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda2555)
```

## Program

```
int main() {
    #pragma clang loop vectorize(enable) unroll(enable)
    #pragma pack(push, 8)
}
```

## To quickly reproduce

https://godbolt.org/z/nnoWb6jcb
</details>


---

### Comment 2 - shafik

So if we remove assert here:

https://github.com/llvm/llvm-project/blob/57a90edacdf4ef14c6a95531681e8218cd23c4ab/clang/lib/Parse/ParseStmt.cpp#L80-L81

we then crash here:

https://github.com/llvm/llvm-project/blob/9d0614e77ab1c5e2264e07d9b4b5f9780b38781c/clang/lib/Sema/SemaStmtAttr.cpp#L91-L95

b/c `St` is `nullptr`. We can adjust the check but I am not sure what we would replace `St->getBeginLoc()` with.

CC @AaronBallman 

---

### Comment 3 - AaronBallman

I think we want to handle this case the same way we handle there being no statement at all: https://godbolt.org/z/4GT1bYMsW so I don't think we would even get into this code path to begin with?

---

### Comment 4 - shafik

> I think we want to handle this case the same way we handle there being no statement at all: https://godbolt.org/z/4GT1bYMsW so I don't think we would even get into this code path to begin with?

So `err_expected_statement` is done in `ParseStatementOrDeclarationAfterAttributes` and we hit that call here:

https://github.com/llvm/llvm-project/blob/57a90edacdf4ef14c6a95531681e8218cd23c4ab/clang/lib/Parse/ParseStmt.cpp#L74-L75

Currently we hit this case:

https://github.com/llvm/llvm-project/blob/57a90edacdf4ef14c6a95531681e8218cd23c4ab/clang/lib/Parse/ParseStmt.cpp#L351-L355

rather than this one, which is the `err_expected_statement` diagnostic:

https://github.com/llvm/llvm-project/blob/57a90edacdf4ef14c6a95531681e8218cd23c4ab/clang/lib/Parse/ParseStmt.cpp#L225-L228

Which looks as intended. I don't see how I have enough information at that point to do something different.

At this point:

https://github.com/llvm/llvm-project/blob/57a90edacdf4ef14c6a95531681e8218cd23c4ab/clang/lib/Parse/ParseStmt.cpp#L2360-L2361

I will know `S.isUsable()` is *false* 

After that I don't have a lot of specific information until I get back to `handleLoopHintAttr` as I pointed out in my previous comment.

---

