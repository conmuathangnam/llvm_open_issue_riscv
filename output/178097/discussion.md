# Clang with C++Modules crashes on template constraints

**Author:** maslowian
**URL:** https://github.com/llvm/llvm-project/issues/178097
**Status:** Closed
**Labels:** clang:modules, crash
**Closed Date:** 2026-01-28T01:46:38Z

## Body

Godbolt: https://godbolt.org/z/b9GaGo1sE
Without using C++Module it works fine.
When I try to compile it with godbolt's clang trunk, it doesn't crash, but sfinae doesn't seem to be working.

Stacktrace:
```
/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0  -I/app/. -fcolor-diagnostics -fno-crash-diagnostics -std=gnu++20 -MD -MT CMakeFiles/args.dir/args.cpp.o -MF CMakeFiles/args.dir/args.cpp.o.d @CMakeFiles/args.dir/args.cpp.o.modmap -o CMakeFiles/args.dir/args.cpp.o -c /app/args.cpp
clang++: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:16995: bool clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, const clang::ASTContext&, clang::Expr::ConstantExprKind) const: Assertion `!isValueDependent() && "Expression evaluator can't be called on a dependent expression."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -I/app/. -fcolor-diagnostics -fno-crash-diagnostics -std=gnu++20 -MD -MT CMakeFiles/args.dir/args.cpp.o -MF CMakeFiles/args.dir/args.cpp.o.d @CMakeFiles/args.dir/args.cpp.o.modmap -o CMakeFiles/args.dir/args.cpp.o -c /app/args.cpp
1.	/app/args.cpp:9:10: at annotation token
 #0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b6874042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b68740969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b6874042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b68740287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00007b687402871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00007b6874039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000796c84a clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&, clang::ASTContext const&, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x796c84a)
#10 0x00000000069c3eb9 calculateConstraintSatisfaction(clang::Sema&, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#11 0x00000000069c55d9 CheckConstraintSatisfaction(clang::Sema&, clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) SemaConcept.cpp:0:0
#12 0x00000000069c5c05 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef<clang::AssociatedConstraint>, llvm::SmallVectorImpl<clang::Expr*>&, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange, clang::ConstraintSatisfaction&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x69c5c05)
#13 0x00000000069d191c clang::Sema::EnsureTemplateArgumentListConstraints(clang::TemplateDecl*, clang::MultiLevelTemplateArgumentList const&, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x69d191c)
#14 0x00000000070a6ab5 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&, clang::DefaultArguments const&, bool, clang::Sema::CheckTemplateArgumentInfo&, bool, bool*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x70a6ab5)
#15 0x00000000070adf7c clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x70adf7c)
#16 0x00000000070b1eb7 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&, clang::SourceLocation, clang::OpaquePtr<clang::TemplateName>, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef<clang::ParsedTemplateArgument>, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x70b1eb7)
#17 0x00000000067ef41b clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&, clang::ImplicitTypenameContext, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67ef41b)
#18 0x00000000067165f0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&, clang::Parser::ParsedTemplateInfo&, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67165f0)
#19 0x00000000066cf304 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66cf304)
#20 0x00000000066cfd39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66cfd39)
#21 0x00000000066d7a2a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7a2a)
#22 0x00000000066d89c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d89c5)
#23 0x00000000066ca73a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca73a)
#24 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#25 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#26 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#27 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#28 0x0000000000d93cff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#29 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x0000000004a36b79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#32 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#34 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#35 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#36 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)
#37 0x0000000000c444d4 main (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xc444d4)
#38 0x00007b6874029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x00007b6874029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000d8a365 _start (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd8a365)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-modules

Author: Masłowian (Maslowian)

<details>
Godbolt: https://godbolt.org/z/b9GaGo1sE
Without using C++Module it works fine.
When I try to compile it with godbolt's clang trunk, it doesn't crash, but sfinae doesn't seem to be working.

Stacktrace:
```
/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0  -I/app/. -fcolor-diagnostics -fno-crash-diagnostics -std=gnu++20 -MD -MT CMakeFiles/args.dir/args.cpp.o -MF CMakeFiles/args.dir/args.cpp.o.d @<!-- -->CMakeFiles/args.dir/args.cpp.o.modmap -o CMakeFiles/args.dir/args.cpp.o -c /app/args.cpp
clang++: /root/llvm-project/clang/lib/AST/ExprConstant.cpp:16995: bool clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, const clang::ASTContext&amp;, clang::Expr::ConstantExprKind) const: Assertion `!isValueDependent() &amp;&amp; "Expression evaluator can't be called on a dependent expression."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -I/app/. -fcolor-diagnostics -fno-crash-diagnostics -std=gnu++20 -MD -MT CMakeFiles/args.dir/args.cpp.o -MF CMakeFiles/args.dir/args.cpp.o.d @<!-- -->CMakeFiles/args.dir/args.cpp.o.modmap -o CMakeFiles/args.dir/args.cpp.o -c /app/args.cpp
1.	/app/args.cpp:9:10: at annotation token
 #<!-- -->0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #<!-- -->1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #<!-- -->2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b6874042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b68740969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b6874042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b68740287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00007b687402871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00007b6874039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000796c84a clang::Expr::EvaluateAsConstantExpr(clang::Expr::EvalResult&amp;, clang::ASTContext const&amp;, clang::Expr::ConstantExprKind) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x796c84a)
#<!-- -->10 0x00000000069c3eb9 calculateConstraintSatisfaction(clang::Sema&amp;, clang::Expr const*, clang::NamedDecl const*, clang::SourceLocation, clang::MultiLevelTemplateArgumentList const&amp;, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->11 0x00000000069c55d9 CheckConstraintSatisfaction(clang::Sema&amp;, clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) SemaConcept.cpp:0:0
#<!-- -->12 0x00000000069c5c05 clang::Sema::CheckConstraintSatisfaction(clang::NamedDecl const*, llvm::ArrayRef&lt;clang::AssociatedConstraint&gt;, llvm::SmallVectorImpl&lt;clang::Expr*&gt;&amp;, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange, clang::ConstraintSatisfaction&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x69c5c05)
#<!-- -->13 0x00000000069d191c clang::Sema::EnsureTemplateArgumentListConstraints(clang::TemplateDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::SourceRange) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x69d191c)
#<!-- -->14 0x00000000070a6ab5 clang::Sema::CheckTemplateArgumentList(clang::TemplateDecl*, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;, clang::DefaultArguments const&amp;, bool, clang::Sema::CheckTemplateArgumentInfo&amp;, bool, bool*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x70a6ab5)
#<!-- -->15 0x00000000070adf7c clang::Sema::CheckTemplateIdType(clang::TemplateName, clang::SourceLocation, clang::TemplateArgumentListInfo&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x70adf7c)
#<!-- -->16 0x00000000070b1eb7 clang::Sema::ActOnTemplateIdType(clang::Scope*, clang::CXXScopeSpec&amp;, clang::SourceLocation, clang::OpaquePtr&lt;clang::TemplateName&gt;, clang::IdentifierInfo const*, clang::SourceLocation, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::ParsedTemplateArgument&gt;, clang::SourceLocation, bool, bool, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x70b1eb7)
#<!-- -->17 0x00000000067ef41b clang::Parser::AnnotateTemplateIdTokenAsType(clang::CXXScopeSpec&amp;, clang::ImplicitTypenameContext, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67ef41b)
#<!-- -->18 0x00000000067165f0 clang::Parser::ParseDeclarationSpecifiers(clang::DeclSpec&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::AccessSpecifier, clang::Parser::DeclSpecContext, clang::Parser::LateParsedAttrList*, clang::ImplicitTypenameContext) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x67165f0)
#<!-- -->19 0x00000000066cf304 clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66cf304)
#<!-- -->20 0x00000000066cfd39 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66cfd39)
#<!-- -->21 0x00000000066d7a2a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d7a2a)
#<!-- -->22 0x00000000066d89c5 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d89c5)
#<!-- -->23 0x00000000066ca73a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca73a)
#<!-- -->24 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#<!-- -->25 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#<!-- -->26 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#<!-- -->27 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#<!-- -->28 0x0000000000d93cff cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#<!-- -->29 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004a36b79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#<!-- -->32 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#<!-- -->34 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#<!-- -->35 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#<!-- -->36 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)
#<!-- -->37 0x0000000000c444d4 main (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xc444d4)
#<!-- -->38 0x00007b6874029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x00007b6874029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000d8a365 _start (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd8a365)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```
</details>


---

### Comment 2 - ChuanqiXu9

We might not look at the crash since we live on trunk. For the failure, would you like to reduce the reproducer?

---

### Comment 3 - maslowian

https://godbolt.org/z/7c3cGd7ef

---

### Comment 4 - ChuanqiXu9

> https://godbolt.org/z/7c3cGd7ef

this looks not related to modules. please file a new issue. Feel free to ask to reopen the issue if I misread anything.

---

