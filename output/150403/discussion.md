# [ObjC] Assertion `(GC_L != GC_R) && "unequal qualifier sets had only equal elements"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/150403

## Body

Reproducer:
https://godbolt.org/z/nEKK8qrbT
```cpp
extern id p3;
extern __weak id p3;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:12089: clang::QualType clang::ASTContext::mergeObjCGCQualifiers(clang::QualType, clang::QualType): Assertion `(GC_L != GC_R) && "unequal qualifier sets had only equal elements"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x objective-c++ <source>
1.	<source>:2:20: current parser token ';'
 #0 0x0000000003fcfbc8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fcfbc8)
 #1 0x0000000003fccff4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fccff4)
 #2 0x0000000003f117c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e7747e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e7747e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e7747e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e7747e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007e7747e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007e7747e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000753d175 clang::ASTContext::mergeObjCGCQualifiers(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x753d175)
#10 0x0000000006a22aff clang::Sema::MergeVarDeclTypes(clang::VarDecl*, clang::VarDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a22aff)
#11 0x0000000006a3cc88 clang::Sema::MergeVarDecl(clang::VarDecl*, clang::LookupResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3cc88)
#12 0x0000000006a3e0ba clang::Sema::CheckVariableDeclaration(clang::VarDecl*, clang::LookupResult&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3e0ba)
#13 0x0000000006a42ebd clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&, llvm::MutableArrayRef<clang::TemplateParameterList*>, bool&, llvm::ArrayRef<clang::BindingDecl*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a42ebd)
#14 0x0000000006a61f7f clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a61f7f)
#15 0x0000000006a62a00 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a62a00)
#16 0x00000000066b1ed3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b1ed3)
#17 0x00000000066c1169 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c1169)
#18 0x000000000667eb3e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667eb3e)
#19 0x000000000667f2d9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667f2d9)
#20 0x0000000006686fda clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6686fda)
#21 0x0000000006687f75 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6687f75)
#22 0x0000000006679cda clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6679cda)
#23 0x000000000497c238 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497c238)
#24 0x0000000004c6f0d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6f0d5)
#25 0x0000000004bebf0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bebf0e)
#26 0x0000000004d62e01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d62e01)
#27 0x0000000000da218f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda218f)
#28 0x0000000000d98dea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x00000000049e3409 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x0000000003f11c64 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f11c64)
#31 0x00000000049e3a1f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x00000000049a5e2d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a5e2d)
#33 0x00000000049a6ebe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a6ebe)
#34 0x00000000049af2d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49af2d5)
#35 0x0000000000d9e63f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e63f)
#36 0x0000000000c52774 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc52774)
#37 0x00007e7747e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x00007e7747e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000d98895 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd98895)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/nEKK8qrbT
```cpp
extern id p3;
extern __weak id p3;
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/AST/ASTContext.cpp:12089: clang::QualType clang::ASTContext::mergeObjCGCQualifiers(clang::QualType, clang::QualType): Assertion `(GC_L != GC_R) &amp;&amp; "unequal qualifier sets had only equal elements"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x objective-c++ &lt;source&gt;
1.	&lt;source&gt;:2:20: current parser token ';'
 #<!-- -->0 0x0000000003fcfbc8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fcfbc8)
 #<!-- -->1 0x0000000003fccff4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3fccff4)
 #<!-- -->2 0x0000000003f117c8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e7747e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e7747e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e7747e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e7747e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007e7747e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007e7747e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000753d175 clang::ASTContext::mergeObjCGCQualifiers(clang::QualType, clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x753d175)
#<!-- -->10 0x0000000006a22aff clang::Sema::MergeVarDeclTypes(clang::VarDecl*, clang::VarDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a22aff)
#<!-- -->11 0x0000000006a3cc88 clang::Sema::MergeVarDecl(clang::VarDecl*, clang::LookupResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3cc88)
#<!-- -->12 0x0000000006a3e0ba clang::Sema::CheckVariableDeclaration(clang::VarDecl*, clang::LookupResult&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3e0ba)
#<!-- -->13 0x0000000006a42ebd clang::Sema::ActOnVariableDeclarator(clang::Scope*, clang::Declarator&amp;, clang::DeclContext*, clang::TypeSourceInfo*, clang::LookupResult&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;, bool&amp;, llvm::ArrayRef&lt;clang::BindingDecl*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a42ebd)
#<!-- -->14 0x0000000006a61f7f clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&amp;, llvm::MutableArrayRef&lt;clang::TemplateParameterList*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a61f7f)
#<!-- -->15 0x0000000006a62a00 clang::Sema::ActOnDeclarator(clang::Scope*, clang::Declarator&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a62a00)
#<!-- -->16 0x00000000066b1ed3 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66b1ed3)
#<!-- -->17 0x00000000066c1169 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66c1169)
#<!-- -->18 0x000000000667eb3e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667eb3e)
#<!-- -->19 0x000000000667f2d9 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x667f2d9)
#<!-- -->20 0x0000000006686fda clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6686fda)
#<!-- -->21 0x0000000006687f75 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6687f75)
#<!-- -->22 0x0000000006679cda clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6679cda)
#<!-- -->23 0x000000000497c238 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x497c238)
#<!-- -->24 0x0000000004c6f0d5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6f0d5)
#<!-- -->25 0x0000000004bebf0e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bebf0e)
#<!-- -->26 0x0000000004d62e01 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d62e01)
#<!-- -->27 0x0000000000da218f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda218f)
#<!-- -->28 0x0000000000d98dea ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x00000000049e3409 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x0000000003f11c64 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f11c64)
#<!-- -->31 0x00000000049e3a1f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x00000000049a5e2d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a5e2d)
#<!-- -->33 0x00000000049a6ebe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49a6ebe)
#<!-- -->34 0x00000000049af2d5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49af2d5)
#<!-- -->35 0x0000000000d9e63f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd9e63f)
#<!-- -->36 0x0000000000c52774 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc52774)
#<!-- -->37 0x00007e7747e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x00007e7747e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000d98895 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd98895)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - shafik

goes back to clang-3.8: https://godbolt.org/z/8MKcqas7x

---

### Comment 3 - shafik

Please in the future, indicate how far back the issue goes. 

Is this fuzzer generated?

---

### Comment 4 - k-arrows

> Is this fuzzer generated?

No. Reduced from the following:
https://github.com/llvm/llvm-project/blob/main/clang/test/SemaObjCXX/objc2-merge-gc-attribue-decl.mm



---

