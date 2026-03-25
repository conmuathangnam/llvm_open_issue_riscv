# [clang] Assertion failure in CXXNameMangler::manglePrefix()

**Author:** patrykstefanski
**URL:** https://github.com/llvm/llvm-project/issues/181933

## Body

When compiling the following code, Clang 21.1.0 hits an assertion failure. It happens after 6d00c4297f6714c03ab10f57c10063ebd79264a1 CC @cor3ntin.

https://godbolt.org/z/Ye3KvPxTT

Code:
```cpp
// clang++ -std=c++23 ./bug.cpp
template <typename Predicate>
void foo(Predicate pred) {
  pred(42);
}

template <typename Predicate>
auto bar(Predicate pred) {
  foo(pred);
}

template <typename T>
concept Baz = requires(const T& x) {
  {
    bar([](const auto&) { return true; })
  };
};

static_assert(Baz<int>);
```

Stacktrace:
```
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::NamedDecl; From = const clang::DeclContext]: Assertion `isa<To>(Val) && "cast<Ty>() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 <source>
1.	<eof> parser at end of file
2.	<source>:3:6: instantiating function definition 'foo<(lambda at <source>:15:9)>'
3.	<source>:15:9: instantiating function definition '(anonymous class)::operator()<int>'
4.	<source>:15:9: LLVM IR generation of declaration '(anonymous class)::operator()'
5.	<source>:15:9: Mangling declaration '(anonymous class)::operator()'
 #0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000759668042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007596680969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000759668042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007596680287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000075966802871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000759668039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007b97f9c (anonymous namespace)::CXXNameMangler::manglePrefix(clang::DeclContext const*, bool) ItaniumMangle.cpp:0:0
#10 0x0000000007b97efd (anonymous namespace)::CXXNameMangler::manglePrefix(clang::DeclContext const*, bool) ItaniumMangle.cpp:0:0
#11 0x0000000007b9809d (anonymous namespace)::CXXNameMangler::mangleTemplatePrefix(clang::GlobalDecl, bool) ItaniumMangle.cpp:0:0
#12 0x0000000007bb2a0f (anonymous namespace)::CXXNameMangler::mangleNestedName(clang::GlobalDecl, clang::DeclContext const*, llvm::SmallVector<llvm::StringRef, 4u> const*, bool) ItaniumMangle.cpp:0:0
#13 0x0000000007b99155 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector<llvm::StringRef, 4u> const*) ItaniumMangle.cpp:0:0
#14 0x0000000007b99e1f (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#15 0x0000000007bab291 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#16 0x0000000007bac3d7 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#17 0x0000000007baea61 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&) ItaniumMangle.cpp:0:0
#18 0x0000000007bd367b clang::MangleContext::mangleName(clang::GlobalDecl, llvm::raw_ostream&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x7bd367b)
#19 0x0000000004594c65 getMangledNameImpl(clang::CodeGen::CodeGenModule&, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#20 0x00000000045a8a5d clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x45a8a5d)
#21 0x00000000045e2698 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x45e2698)
#22 0x00000000045ed413 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#23 0x00000000049d3d70 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#24 0x00000000049c33b4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49c33b4)
#25 0x00000000072ab62a clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x72ab62a)
#26 0x0000000007f77341 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x7f77341)
#27 0x000000000713acea clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x713acea)
#28 0x0000000006be8f73 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef<clang::SourceLocation>, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6be8f73)
#29 0x0000000006f78097 CreateFunctionRefExpr(clang::Sema&, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&) SemaOverload.cpp:0:0
#30 0x0000000006fbb222 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6fbb222)
#31 0x0000000006c313a5 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6c313a5)
#32 0x0000000006c3200e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef<clang::Expr*>, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6c3200e)
#33 0x000000000722ab50 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#34 0x00000000071fdf72 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#35 0x0000000007241a47 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformStmt(clang::Stmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#36 0x00000000072420ce clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#37 0x000000000724b764 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x724b764)
#38 0x00000000072ab980 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x72ab980)
#39 0x00000000072a9646 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x72a9646)
#40 0x000000000687bb12 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#41 0x000000000687be92 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x687be92)
#42 0x00000000066d8aa0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d8aa0)
#43 0x00000000066ca73a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca73a)
#44 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#45 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#46 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#47 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#48 0x0000000000d93cff cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#49 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#50 0x0000000004a36b79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#51 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#52 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#53 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#54 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#55 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#56 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)
#57 0x0000000000c444d4 main (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xc444d4)
#58 0x0000759668029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#59 0x0000759668029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#60 0x0000000000d8a365 _start (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd8a365)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Patryk Stefanski (patrykstefanski)

<details>
When compiling the following code, Clang 21.1.0 hits an assertion failure. It happens after 6d00c4297f6714c03ab10f57c10063ebd79264a1 CC @<!-- -->cor3ntin.

https://godbolt.org/z/Ye3KvPxTT

Code:
```cpp
// clang++ -std=c++23 ./bug.cpp
template &lt;typename Predicate&gt;
void foo(Predicate pred) {
  pred(42);
}

template &lt;typename Predicate&gt;
auto bar(Predicate pred) {
  foo(pred);
}

template &lt;typename T&gt;
concept Baz = requires(const T&amp; x) {
  {
    bar([](const auto&amp;) { return true; })
  };
};

static_assert(Baz&lt;int&gt;);
```

Stacktrace:
```
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:578: decltype(auto) llvm::cast(From*) [with To = clang::NamedDecl; From = const clang::DeclContext]: Assertion `isa&lt;To&gt;(Val) &amp;&amp; "cast&lt;Ty&gt;() argument of incompatible type!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-21.1.0/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-15.2.0 -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:3:6: instantiating function definition 'foo&lt;(lambda at &lt;source&gt;:15:9)&gt;'
3.	&lt;source&gt;:15:9: instantiating function definition '(anonymous class)::operator()&lt;int&gt;'
4.	&lt;source&gt;:15:9: LLVM IR generation of declaration '(anonymous class)::operator()'
5.	&lt;source&gt;:15:9: Mangling declaration '(anonymous class)::operator()'
 #<!-- -->0 0x0000000004019ae8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4019ae8)
 #<!-- -->1 0x0000000004016f14 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4016f14)
 #<!-- -->2 0x0000000003f5b698 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000759668042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007596680969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000759668042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007596680287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000075966802871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000759668039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007b97f9c (anonymous namespace)::CXXNameMangler::manglePrefix(clang::DeclContext const*, bool) ItaniumMangle.cpp:0:0
#<!-- -->10 0x0000000007b97efd (anonymous namespace)::CXXNameMangler::manglePrefix(clang::DeclContext const*, bool) ItaniumMangle.cpp:0:0
#<!-- -->11 0x0000000007b9809d (anonymous namespace)::CXXNameMangler::mangleTemplatePrefix(clang::GlobalDecl, bool) ItaniumMangle.cpp:0:0
#<!-- -->12 0x0000000007bb2a0f (anonymous namespace)::CXXNameMangler::mangleNestedName(clang::GlobalDecl, clang::DeclContext const*, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*, bool) ItaniumMangle.cpp:0:0
#<!-- -->13 0x0000000007b99155 (anonymous namespace)::CXXNameMangler::mangleNameWithAbiTags(clang::GlobalDecl, llvm::SmallVector&lt;llvm::StringRef, 4u&gt; const*) ItaniumMangle.cpp:0:0
#<!-- -->14 0x0000000007b99e1f (anonymous namespace)::CXXNameMangler::mangleName(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#<!-- -->15 0x0000000007bab291 (anonymous namespace)::CXXNameMangler::mangleFunctionEncoding(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#<!-- -->16 0x0000000007bac3d7 (anonymous namespace)::CXXNameMangler::mangle(clang::GlobalDecl) ItaniumMangle.cpp:0:0
#<!-- -->17 0x0000000007baea61 (anonymous namespace)::ItaniumMangleContextImpl::mangleCXXName(clang::GlobalDecl, llvm::raw_ostream&amp;) ItaniumMangle.cpp:0:0
#<!-- -->18 0x0000000007bd367b clang::MangleContext::mangleName(clang::GlobalDecl, llvm::raw_ostream&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x7bd367b)
#<!-- -->19 0x0000000004594c65 getMangledNameImpl(clang::CodeGen::CodeGenModule&amp;, clang::GlobalDecl, clang::NamedDecl const*, bool) CodeGenModule.cpp:0:0
#<!-- -->20 0x00000000045a8a5d clang::CodeGen::CodeGenModule::getMangledName(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x45a8a5d)
#<!-- -->21 0x00000000045e2698 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x45e2698)
#<!-- -->22 0x00000000045ed413 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->23 0x00000000049d3d70 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->24 0x00000000049c33b4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49c33b4)
#<!-- -->25 0x00000000072ab62a clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x72ab62a)
#<!-- -->26 0x0000000007f77341 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x7f77341)
#<!-- -->27 0x000000000713acea clang::Sema::DeduceReturnType(clang::FunctionDecl*, clang::SourceLocation, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x713acea)
#<!-- -->28 0x0000000006be8f73 clang::Sema::DiagnoseUseOfDecl(clang::NamedDecl*, llvm::ArrayRef&lt;clang::SourceLocation&gt;, clang::ObjCInterfaceDecl const*, bool, bool, clang::ObjCInterfaceDecl*, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6be8f73)
#<!-- -->29 0x0000000006f78097 CreateFunctionRefExpr(clang::Sema&amp;, clang::FunctionDecl*, clang::NamedDecl*, clang::Expr const*, bool, clang::SourceLocation, clang::DeclarationNameLoc const&amp;) SemaOverload.cpp:0:0
#<!-- -->30 0x0000000006fbb222 clang::Sema::BuildCallToObjectOfClassType(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6fbb222)
#<!-- -->31 0x0000000006c313a5 clang::Sema::BuildCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6c313a5)
#<!-- -->32 0x0000000006c3200e clang::Sema::ActOnCallExpr(clang::Scope*, clang::Expr*, clang::SourceLocation, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::SourceLocation, clang::Expr*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x6c3200e)
#<!-- -->33 0x000000000722ab50 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCallExpr(clang::CallExpr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->34 0x00000000071fdf72 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformExpr(clang::Expr*) SemaTemplateInstantiate.cpp:0:0
#<!-- -->35 0x0000000007241a47 clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformStmt(clang::Stmt*, clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#<!-- -->36 0x00000000072420ce clang::TreeTransform&lt;(anonymous namespace)::TemplateInstantiator&gt;::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#<!-- -->37 0x000000000724b764 clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x724b764)
#<!-- -->38 0x00000000072ab980 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x72ab980)
#<!-- -->39 0x00000000072a9646 clang::Sema::PerformPendingInstantiations(bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x72a9646)
#<!-- -->40 0x000000000687bb12 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::TUFragmentKind) (.part.0) Sema.cpp:0:0
#<!-- -->41 0x000000000687be92 clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x687be92)
#<!-- -->42 0x00000000066d8aa0 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66d8aa0)
#<!-- -->43 0x00000000066ca73a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x66ca73a)
#<!-- -->44 0x00000000049cfa28 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49cfa28)
#<!-- -->45 0x0000000004cc9f95 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4cc9f95)
#<!-- -->46 0x0000000004c4688e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4c4688e)
#<!-- -->47 0x0000000004dbc2e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4dbc2e1)
#<!-- -->48 0x0000000000d93cff cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd93cff)
#<!-- -->49 0x0000000000d8a8ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->50 0x0000000004a36b79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->51 0x0000000003f5bb34 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x3f5bb34)
#<!-- -->52 0x0000000004a3718f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->53 0x00000000049f915d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49f915d)
#<!-- -->54 0x00000000049fa1ee clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x49fa1ee)
#<!-- -->55 0x0000000004a02d85 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0x4a02d85)
#<!-- -->56 0x0000000000d9018f clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd9018f)
#<!-- -->57 0x0000000000c444d4 main (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xc444d4)
#<!-- -->58 0x0000759668029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->59 0x0000759668029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->60 0x0000000000d8a365 _start (/opt/compiler-explorer/clang-assertions-21.1.0/bin/clang+++0xd8a365)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - spider-yamet

@llvmbot May I pick up this issue?
Hope this project is open for contribution.

---

### Comment 3 - shafik

> [@llvmbot](https://github.com/llvmbot) May I pick up this issue? Hope this project is open for contribution.

We assign issues after there is a PR and we check that it is headed in the right direction.

---

