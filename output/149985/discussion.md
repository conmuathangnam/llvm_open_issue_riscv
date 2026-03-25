# UNREACHABLE executed at SemaDecl.cpp:5534 since clang 3.5

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/149985
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, confirmed
**Closed Date:** 2025-08-15T16:43:30Z

## Body

If the first struct is removed the code is valid and ICE disappear.


## Assertion

No assertion but:

```
couldn't find in-class initializer
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDecl.cpp:5534!
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ <source>
1.	<source>:15:1: current parser token '}'
2.	<source>:2:1: parsing struct/union/class body 'X'
 #0 0x0000000003fc4898 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc4898)
 #1 0x0000000003fc1cc4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc1cc4)
 #2 0x0000000003f06498 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f08ffa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007f08ffa969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007f08ffa42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007f08ffa287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003f11c5a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f11c5a)
 #8 0x00000000069e695d findDefaultInitializer(clang::CXXRecordDecl const*) SemaDecl.cpp:0:0
 #9 0x00000000069ea957 checkDuplicateDefaultInit(clang::Sema&, clang::CXXRecordDecl*, clang::SourceLocation) SemaDecl.cpp:0:0
#10 0x0000000006a395ce clang::Sema::BuildAnonymousStructOrUnion(clang::Scope*, clang::DeclSpec&, clang::AccessSpecifier, clang::RecordDecl*, clang::PrintingPolicy const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a395ce)
#11 0x0000000006a3ac0a clang::Sema::ParsedFreeStandingDeclSpec(clang::Scope*, clang::AccessSpecifier, clang::DeclSpec&, clang::ParsedAttributesView const&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool, clang::RecordDecl*&, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3ac0a)
#12 0x00000000066db7a9 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66db7a9)
#13 0x00000000066dd861 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&, clang::ParsedAttributes&, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dd861)
#14 0x00000000066df69f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66df69f)
#15 0x00000000066e25c0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e25c0)
#16 0x00000000066b5b70 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b5b70)
#17 0x0000000006670c44 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6670c44)
#18 0x0000000006671679 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6671679)
#19 0x000000000667937a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667937a)
#20 0x000000000667a315 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667a315)
#21 0x000000000667a7d0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667a7d0)
#22 0x000000000666c043 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666c043)
#23 0x0000000004972ad8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4972ad8)
#24 0x0000000004c64eb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64eb5)
#25 0x0000000004be1c7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be1c7e)
#26 0x0000000004d58991 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d58991)
#27 0x0000000000d9f10f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f10f)
#28 0x0000000000d95d6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x00000000049d94a9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000000003f06934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f06934)
#31 0x00000000049d9abf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x000000000499c6bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499c6bd)
#33 0x000000000499d74e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499d74e)
#34 0x00000000049a57b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a57b5)
#35 0x0000000000d9b5bf clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b5bf)
#36 0x0000000000c4fa44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4fa44)
#37 0x00007f08ffa29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x00007f08ffa29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000d95815 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd95815)
```


## Program

```

union  X {
    enum {
        csize = 42,
        cs = sizeof(int)
    };
    struct {
        int data;
        union X *cs[csize] = {};
    };
    struct {
        int data;
        union X *ds[2] = {};
    };
};
```

To quickly reproduce:

https://godbolt.org/z/8WdEjj99r

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
If the first struct is removed the code is valid and ICE disappear.


## Assertion

No assertion but:

```
couldn't find in-class initializer
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDecl.cpp:5534!
```

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ &lt;source&gt;
1.	&lt;source&gt;:15:1: current parser token '}'
2.	&lt;source&gt;:2:1: parsing struct/union/class body 'X'
 #<!-- -->0 0x0000000003fc4898 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc4898)
 #<!-- -->1 0x0000000003fc1cc4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fc1cc4)
 #<!-- -->2 0x0000000003f06498 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f08ffa42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007f08ffa969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007f08ffa42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007f08ffa287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003f11c5a (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f11c5a)
 #<!-- -->8 0x00000000069e695d findDefaultInitializer(clang::CXXRecordDecl const*) SemaDecl.cpp:0:0
 #<!-- -->9 0x00000000069ea957 checkDuplicateDefaultInit(clang::Sema&amp;, clang::CXXRecordDecl*, clang::SourceLocation) SemaDecl.cpp:0:0
#<!-- -->10 0x0000000006a395ce clang::Sema::BuildAnonymousStructOrUnion(clang::Scope*, clang::DeclSpec&amp;, clang::AccessSpecifier, clang::RecordDecl*, clang::PrintingPolicy const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a395ce)
#<!-- -->11 0x0000000006a3ac0a clang::Sema::ParsedFreeStandingDeclSpec(clang::Scope*, clang::AccessSpecifier, clang::DeclSpec&amp;, clang::ParsedAttributesView const&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool, clang::RecordDecl*&amp;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3ac0a)
#<!-- -->12 0x00000000066db7a9 clang::Parser::ParseCXXClassMemberDeclaration(clang::AccessSpecifier, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::ParsingDeclRAIIObject*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66db7a9)
#<!-- -->13 0x00000000066dd861 clang::Parser::ParseCXXClassMemberDeclarationWithPragmas(clang::AccessSpecifier&amp;, clang::ParsedAttributes&amp;, clang::TypeSpecifierType, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66dd861)
#<!-- -->14 0x00000000066df69f clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66df69f)
#<!-- -->15 0x00000000066e25c0 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e25c0)
#<!-- -->16 0x00000000066b5b70 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b5b70)
#<!-- -->17 0x0000000006670c44 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6670c44)
#<!-- -->18 0x0000000006671679 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6671679)
#<!-- -->19 0x000000000667937a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667937a)
#<!-- -->20 0x000000000667a315 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667a315)
#<!-- -->21 0x000000000667a7d0 clang::Parser::ParseFirstTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667a7d0)
#<!-- -->22 0x000000000666c043 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666c043)
#<!-- -->23 0x0000000004972ad8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4972ad8)
#<!-- -->24 0x0000000004c64eb5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c64eb5)
#<!-- -->25 0x0000000004be1c7e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4be1c7e)
#<!-- -->26 0x0000000004d58991 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d58991)
#<!-- -->27 0x0000000000d9f10f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9f10f)
#<!-- -->28 0x0000000000d95d6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x00000000049d94a9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000000003f06934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f06934)
#<!-- -->31 0x00000000049d9abf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x000000000499c6bd clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499c6bd)
#<!-- -->33 0x000000000499d74e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x499d74e)
#<!-- -->34 0x00000000049a57b5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a57b5)
#<!-- -->35 0x0000000000d9b5bf clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9b5bf)
#<!-- -->36 0x0000000000c4fa44 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc4fa44)
#<!-- -->37 0x00007f08ffa29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x00007f08ffa29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000d95815 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd95815)
```


## Program

```

union  X {
    enum {
        csize = 42,
        cs = sizeof(int)
    };
    struct {
        int data;
        union X *cs[csize] = {};
    };
    struct {
        int data;
        union X *ds[2] = {};
    };
};
```

To quickly reproduce:

https://godbolt.org/z/8WdEjj99r
</details>


---

