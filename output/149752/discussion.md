# [clang] crashes when using __attribute__ ((malloc (__VA_ARGS__))) with assertion enabled

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/149752

## Body

Reproducer:
https://godbolt.org/z/Eev6E8x7b
```c
#define A(...) __attribute__ ((malloc (__VA_ARGS__)))
void f(type, void*);
A(f) void* g(void);
```

Backtrace:
```console
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionProtoType; From = const clang::FunctionType]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:3:19: current parser token ';'
 #0 0x0000000003fc1498 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fc1498)
 #1 0x0000000003fbe8c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fbe8c4)
 #2 0x0000000003f03098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000076259f442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000076259f4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000076259f442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000076259f4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076259f42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000076259f439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006a56913 clang::getFunctionOrMethodParamType(clang::Decl const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a56913)
#10 0x0000000006a7c3f0 handleRestrictAttr(clang::Sema&, clang::Decl*, clang::ParsedAttr const&) SemaDeclAttr.cpp:0:0
#11 0x0000000006a92b25 ProcessDeclAttribute(clang::Sema&, clang::Scope*, clang::Decl*, clang::ParsedAttr const&, clang::Sema::ProcessDeclAttributeOptions const&) (.isra.0) SemaDeclAttr.cpp:0:0
#12 0x0000000006a954eb clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a954eb)
#13 0x0000000006a46bdc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a46bdc)
#14 0x0000000006a4d3f1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4d3f1)
#15 0x0000000006a4df40 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4df40)
#16 0x000000000669e4e3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x669e4e3)
#17 0x00000000066ad779 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66ad779)
#18 0x000000000666b14e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x666b14e)
#19 0x000000000666b8e9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x666b8e9)
#20 0x00000000066735ea clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66735ea)
#21 0x0000000006674585 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6674585)
#22 0x00000000066662ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66662ea)
#23 0x0000000004970188 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4970188)
#24 0x0000000004c64025 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c64025)
#25 0x0000000004be0e6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4be0e6e)
#26 0x0000000004d57b01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d57b01)
#27 0x0000000000d9ec4f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd9ec4f)
#28 0x0000000000d9582a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x00000000049d7709 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000000003f03534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f03534)
#31 0x00000000049d7d1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x0000000004999d6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4999d6d)
#33 0x000000000499adfe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x499adfe)
#34 0x00000000049a3945 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a3945)
#35 0x0000000000d9b0ff clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd9b0ff)
#36 0x0000000000c4f5b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc4f5b4)
#37 0x000076259f429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x000076259f429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000d952d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd952d5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/Eev6E8x7b
```c
#define A(...) __attribute__ ((malloc (__VA_ARGS__)))
void f(type, void*);
A(f) void* g(void);
```

Backtrace:
```console
clang: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::FunctionProtoType; From = const clang::FunctionType]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:3:19: current parser token ';'
 #<!-- -->0 0x0000000003fc1498 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fc1498)
 #<!-- -->1 0x0000000003fbe8c4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3fbe8c4)
 #<!-- -->2 0x0000000003f03098 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000076259f442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000076259f4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000076259f442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000076259f4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076259f42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000076259f439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006a56913 clang::getFunctionOrMethodParamType(clang::Decl const*, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a56913)
#<!-- -->10 0x0000000006a7c3f0 handleRestrictAttr(clang::Sema&amp;, clang::Decl*, clang::ParsedAttr const&amp;) SemaDeclAttr.cpp:0:0
#<!-- -->11 0x0000000006a92b25 ProcessDeclAttribute(clang::Sema&amp;, clang::Scope*, clang::Decl*, clang::ParsedAttr const&amp;, clang::Sema::ProcessDeclAttributeOptions const&amp;) (.isra.0) SemaDeclAttr.cpp:0:0
#<!-- -->12 0x0000000006a954eb clang::Sema::ProcessDeclAttributes(clang::Scope*, clang::Decl*, clang::Declarator const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a954eb)
#<!-- -->13 0x0000000006a46bdc clang::Sema::ActOnFunctionDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a46bdc)
#<!-- -->14 0x0000000006a4d3f1 clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4d3f1)
#<!-- -->15 0x0000000006a4df40 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a4df40)
#<!-- -->16 0x000000000669e4e3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x669e4e3)
#<!-- -->17 0x00000000066ad779 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66ad779)
#<!-- -->18 0x000000000666b14e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x666b14e)
#<!-- -->19 0x000000000666b8e9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x666b8e9)
#<!-- -->20 0x00000000066735ea clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66735ea)
#<!-- -->21 0x0000000006674585 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6674585)
#<!-- -->22 0x00000000066662ea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x66662ea)
#<!-- -->23 0x0000000004970188 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4970188)
#<!-- -->24 0x0000000004c64025 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c64025)
#<!-- -->25 0x0000000004be0e6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4be0e6e)
#<!-- -->26 0x0000000004d57b01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4d57b01)
#<!-- -->27 0x0000000000d9ec4f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd9ec4f)
#<!-- -->28 0x0000000000d9582a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x00000000049d7709 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000000003f03534 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x3f03534)
#<!-- -->31 0x00000000049d7d1f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x0000000004999d6d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4999d6d)
#<!-- -->33 0x000000000499adfe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x499adfe)
#<!-- -->34 0x00000000049a3945 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49a3945)
#<!-- -->35 0x0000000000d9b0ff clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd9b0ff)
#<!-- -->36 0x0000000000c4f5b4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc4f5b4)
#<!-- -->37 0x000076259f429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x000076259f429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000d952d5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xd952d5)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

