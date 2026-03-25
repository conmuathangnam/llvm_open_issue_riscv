# [clang] Assertion `(!needsOverloadResolutionForMoveConstructor() || (data().DeclaredSpecialMembers & SMF_MoveConstructor)) && "this property has not yet been computed by Sema"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/155744

## Body

Reproducer:
https://godbolt.org/z/zGcx6jq11
```cpp
struct __attribute__((trivial_abi)) MoveDeleted {
  MoveDeleted(const MoveDeleted &) = default;
  MoveDeleted(MoveDeleted &&) = delete;
};
struct __attribute__((trivial_abi)) S19 {
  MoveDeleted b;
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/DeclCXX.h:707: bool clang::CXXRecordDecl::defaultedMoveConstructorIsDeleted() const: Assertion `(!needsOverloadResolutionForMoveConstructor() || (data().DeclaredSpecialMembers & SMF_MoveConstructor)) && "this property has not yet been computed by Sema"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++03 <source>
1.	<source>:7:2: current parser token ';'
2.	<source>:5:1: parsing struct/union/class body 'S19'
 #0 0x000000000402cd38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x402cd38)
 #1 0x000000000402a164 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x402a164)
 #2 0x0000000003f6e618 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000738ede642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000738ede6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000738ede642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000738ede6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000738ede62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000738ede639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006aff320 clang::Sema::DefineUsedVTables() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aff320)
#10 0x0000000006b0ffec clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b0ffec)
#11 0x0000000006b10f69 clang::Sema::ActOnFinishCXXMemberSpecification(clang::Scope*, clang::SourceLocation, clang::Decl*, clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributesView const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b10f69)
#12 0x00000000066ef900 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ef900)
#13 0x00000000066f2750 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f2750)
#14 0x00000000066c521e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c521e)
#15 0x000000000667fcc8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667fcc8)
#16 0x00000000066806ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66806ff)
#17 0x000000000668853a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668853a)
#18 0x00000000066894d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66894d5)
#19 0x000000000666943a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666943a)
#20 0x0000000004953f98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4953f98)
#21 0x0000000004c4a4d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4a4d5)
#22 0x0000000004bc5d4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc5d4e)
#23 0x0000000004d3e441 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d3e441)
#24 0x0000000000db12ff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb12ff)
#25 0x0000000000da7faa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x00000000049baee9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x0000000003f6eab4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6eab4)
#28 0x00000000049bb4ff clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x000000000497dced clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497dced)
#30 0x000000000497ed7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497ed7e)
#31 0x0000000004986605 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4986605)
#32 0x0000000000dad7b5 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdad7b5)
#33 0x0000000000c625f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc625f4)
#34 0x0000738ede629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x0000738ede629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000da7a55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7a55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/zGcx6jq11
```cpp
struct __attribute__((trivial_abi)) MoveDeleted {
  MoveDeleted(const MoveDeleted &amp;) = default;
  MoveDeleted(MoveDeleted &amp;&amp;) = delete;
};
struct __attribute__((trivial_abi)) S19 {
  MoveDeleted b;
};
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/AST/DeclCXX.h:707: bool clang::CXXRecordDecl::defaultedMoveConstructorIsDeleted() const: Assertion `(!needsOverloadResolutionForMoveConstructor() || (data().DeclaredSpecialMembers &amp; SMF_MoveConstructor)) &amp;&amp; "this property has not yet been computed by Sema"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics --std=c++03 &lt;source&gt;
1.	&lt;source&gt;:7:2: current parser token ';'
2.	&lt;source&gt;:5:1: parsing struct/union/class body 'S19'
 #<!-- -->0 0x000000000402cd38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x402cd38)
 #<!-- -->1 0x000000000402a164 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x402a164)
 #<!-- -->2 0x0000000003f6e618 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000738ede642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000738ede6969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000738ede642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000738ede6287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000738ede62871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000738ede639e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006aff320 clang::Sema::DefineUsedVTables() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6aff320)
#<!-- -->10 0x0000000006b0ffec clang::Sema::CheckCompletedCXXClass(clang::Scope*, clang::CXXRecordDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b0ffec)
#<!-- -->11 0x0000000006b10f69 clang::Sema::ActOnFinishCXXMemberSpecification(clang::Scope*, clang::SourceLocation, clang::Decl*, clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributesView const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b10f69)
#<!-- -->12 0x00000000066ef900 clang::Parser::ParseCXXMemberSpecification(clang::SourceLocation, clang::SourceLocation, clang::ParsedAttributes&amp;, unsigned int, clang::Decl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ef900)
#<!-- -->13 0x00000000066f2750 clang::Parser::ParseClassSpecifier(clang::tok::TokenKind, clang::SourceLocation, clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, bool, clang::Parser::DeclSpecContext, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66f2750)
#<!-- -->14 0x00000000066c521e clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c521e)
#<!-- -->15 0x000000000667fcc8 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667fcc8)
#<!-- -->16 0x00000000066806ff clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66806ff)
#<!-- -->17 0x000000000668853a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x668853a)
#<!-- -->18 0x00000000066894d5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66894d5)
#<!-- -->19 0x000000000666943a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x666943a)
#<!-- -->20 0x0000000004953f98 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4953f98)
#<!-- -->21 0x0000000004c4a4d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c4a4d5)
#<!-- -->22 0x0000000004bc5d4e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bc5d4e)
#<!-- -->23 0x0000000004d3e441 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d3e441)
#<!-- -->24 0x0000000000db12ff cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb12ff)
#<!-- -->25 0x0000000000da7faa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->26 0x00000000049baee9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x0000000003f6eab4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f6eab4)
#<!-- -->28 0x00000000049bb4ff clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x000000000497dced clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497dced)
#<!-- -->30 0x000000000497ed7e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497ed7e)
#<!-- -->31 0x0000000004986605 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4986605)
#<!-- -->32 0x0000000000dad7b5 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdad7b5)
#<!-- -->33 0x0000000000c625f4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc625f4)
#<!-- -->34 0x0000738ede629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x0000738ede629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000da7a55 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7a55)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-11.0.0:
https://godbolt.org/z/z169dWPTa

---

### Comment 3 - k-arrows

The reproducer here is reduced from clang/test/SemaCXX/attr-trivial-abi.cpp

---

### Comment 4 - k-arrows

Note: this is using `-std=c++03`

---

