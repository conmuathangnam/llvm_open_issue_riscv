# [Clang] [Sema] Assertion when trying to `goto` into an attribute

**Author:** Sirraide
**URL:** https://github.com/llvm/llvm-project/issues/133604

## Body

This is horrendous and I hope no-one is actually writing code like this (https://godbolt.org/z/To1Kh13WW):
```c++
void f() {
    goto x;
    [[assume(({ x: 5; }))]];
}
```

Assertion:
```console
clang/lib/Sema/JumpDiagnostics.cpp:939: 
void {anonymous}::JumpScopeChecker::CheckJump(
  clang::Stmt*, clang::Stmt*, clang::SourceLocation, unsigned int, 
  unsigned int, unsigned int): 
Assertion `Permissive || !(!LabelAndGotoScopes.count(To))' failed.
```

Stack trace:
```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:1:10: parsing function body 'f'
 #0 0x0000000003e9f408 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9f408)
 #1 0x0000000003e9d094 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9d094)
 #2 0x0000000003de21d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000710c0fa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000710c0fa969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000710c0fa42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000710c0fa287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000710c0fa2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000710c0fa39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000071bde23 (anonymous namespace)::JumpScopeChecker::CheckJump(clang::Stmt*, clang::Stmt*, clang::SourceLocation, unsigned int, unsigned int, unsigned int) JumpDiagnostics.cpp:0:0
#10 0x00000000071c4749 (anonymous namespace)::JumpScopeChecker::VerifyJumps() JumpDiagnostics.cpp:0:0
#11 0x00000000071c9cae clang::Sema::DiagnoseInvalidJumps(clang::Stmt*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71c9cae)
#12 0x000000000687acc2 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687acc2)
#13 0x00000000065d36ff clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d36ff)
#14 0x00000000064dd163 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64dd163)
#15 0x00000000065126fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65126fd)
#16 0x00000000064d0d7e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d0d7e)
#17 0x00000000064d1539 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d1539)
#18 0x00000000064d8e03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d8e03)
#19 0x00000000064d9d1d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d9d1d)
#20 0x00000000064da1d0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64da1d0)
#21 0x00000000064cc233 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64cc233)
#22 0x0000000004836808 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4836808)
#23 0x0000000004b25075 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b25075)
#24 0x0000000004aa6cbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aa6cbe)
#25 0x0000000004c1563e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c1563e)
#26 0x0000000000d5d5ef cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5d5ef)
#27 0x0000000000d54daa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#28 0x000000000489bf89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x0000000003de2674 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3de2674)
#30 0x000000000489c59f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x000000000485ef5d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485ef5d)
#32 0x000000000485ffee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485ffee)
#33 0x00000000048676f5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48676f5)
#34 0x0000000000d5a3e8 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5a3e8)
#35 0x0000000000c20824 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc20824)
#36 0x0000710c0fa29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x0000710c0fa29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000d54855 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd54855)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Sirraide)

<details>
This is horrendous and I hope no-one is actually writing code like this (https://godbolt.org/z/To1Kh13WW):
```c++
void f() {
    goto x;
    [[assume(({ x: 5; }))]];
}
```

Assertion:
```console
clang/lib/Sema/JumpDiagnostics.cpp:939: 
void {anonymous}::JumpScopeChecker::CheckJump(
  clang::Stmt*, clang::Stmt*, clang::SourceLocation, unsigned int, 
  unsigned int, unsigned int): 
Assertion `Permissive || !(!LabelAndGotoScopes.count(To))' failed.
```

Stack trace:
```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:1:10: parsing function body 'f'
 #<!-- -->0 0x0000000003e9f408 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9f408)
 #<!-- -->1 0x0000000003e9d094 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9d094)
 #<!-- -->2 0x0000000003de21d8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000710c0fa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000710c0fa969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000710c0fa42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000710c0fa287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000710c0fa2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000710c0fa39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000071bde23 (anonymous namespace)::JumpScopeChecker::CheckJump(clang::Stmt*, clang::Stmt*, clang::SourceLocation, unsigned int, unsigned int, unsigned int) JumpDiagnostics.cpp:0:0
#<!-- -->10 0x00000000071c4749 (anonymous namespace)::JumpScopeChecker::VerifyJumps() JumpDiagnostics.cpp:0:0
#<!-- -->11 0x00000000071c9cae clang::Sema::DiagnoseInvalidJumps(clang::Stmt*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x71c9cae)
#<!-- -->12 0x000000000687acc2 clang::Sema::ActOnFinishFunctionBody(clang::Decl*, clang::Stmt*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687acc2)
#<!-- -->13 0x00000000065d36ff clang::Parser::ParseFunctionStatementBody(clang::Decl*, clang::Parser::ParseScope&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d36ff)
#<!-- -->14 0x00000000064dd163 clang::Parser::ParseFunctionDefinition(clang::ParsingDeclarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::LateParsedAttrList*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64dd163)
#<!-- -->15 0x00000000065126fd clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65126fd)
#<!-- -->16 0x00000000064d0d7e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d0d7e)
#<!-- -->17 0x00000000064d1539 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d1539)
#<!-- -->18 0x00000000064d8e03 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d8e03)
#<!-- -->19 0x00000000064d9d1d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d9d1d)
#<!-- -->20 0x00000000064da1d0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64da1d0)
#<!-- -->21 0x00000000064cc233 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64cc233)
#<!-- -->22 0x0000000004836808 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4836808)
#<!-- -->23 0x0000000004b25075 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b25075)
#<!-- -->24 0x0000000004aa6cbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aa6cbe)
#<!-- -->25 0x0000000004c1563e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c1563e)
#<!-- -->26 0x0000000000d5d5ef cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5d5ef)
#<!-- -->27 0x0000000000d54daa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->28 0x000000000489bf89 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->29 0x0000000003de2674 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3de2674)
#<!-- -->30 0x000000000489c59f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->31 0x000000000485ef5d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485ef5d)
#<!-- -->32 0x000000000485ffee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485ffee)
#<!-- -->33 0x00000000048676f5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48676f5)
#<!-- -->34 0x0000000000d5a3e8 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5a3e8)
#<!-- -->35 0x0000000000c20824 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc20824)
#<!-- -->36 0x0000710c0fa29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->37 0x0000710c0fa29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->38 0x0000000000d54855 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd54855)
```
</details>


---

### Comment 2 - pinskia

Note GCC rejects jumps directly into statement expressions.
But GCC does have a similar bug if you use computed gotos (https://gcc.gnu.org/PR119537).

Note you can get a similar ICE with typeof rather than assume:
```
void f() {

    goto x;
    __typeof__(({ x: 5; })) a;
}
```

---

### Comment 3 - Sirraide

> Note GCC rejects jumps directly into statement expressions.

Yeah, we normally do that too, but something about it being the argument of an attribute is causing problems here.

---

