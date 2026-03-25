# [clang] Assertion `!hasUninstantiatedDefaultArg() && "Default argument is not yet instantiated!"` failed

**Author:** Rush10233
**URL:** https://github.com/llvm/llvm-project/issues/132616

## Body

Goes back to clang-16: https://godbolt.org/z/8TYcah3rs

Test Input:

```c++
template <class T> struct A
{
  template <class U>
  A(U = 42);
};

template <typename>
struct B: A<int>
{
  using A::A;
};

B b(24);
```

Stack Dump:

```c++
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp:382: clang::NamedDecl* {anonymous}::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*): Assertion `Depth || cast<ClassTemplateSpecializationDecl>(FTD->getDeclContext()) ->isExplicitSpecialization()`failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:13:8: current parser token ';'
 #0 0x0000000003e9fee8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9fee8)
 #1 0x0000000003e9db74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9db74)
 #2 0x0000000003de2f18 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070ab83e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070ab83e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070ab83e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070ab83e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070ab83e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070ab83e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007021096 (anonymous namespace)::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*) SemaTemplateDeductionGuide.cpp:0:0
#10 0x0000000007023b5a clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7023b5a)
#11 0x0000000006c05d5f LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#12 0x0000000006bf0ce2 clang::Sema::LookupQualifiedName(clang::LookupResult&, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf0ce2)
#13 0x0000000006bcf1de clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf1de)
#14 0x000000000684ca6d clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684ca6d)
#15 0x0000000006851877 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6851877)
#16 0x0000000006872862 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6872862)
#17 0x00000000064fa2f9 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64fa2f9)
#18 0x0000000006509549 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6509549)
#19 0x00000000064c85ce clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c85ce)
#20 0x00000000064c8d89 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c8d89)
#21 0x00000000064d0653 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d0653)
#22 0x00000000064d156d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d156d)
#23 0x00000000064c3aba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c3aba)
#24 0x00000000048325b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48325b8)
#25 0x0000000004b1ff05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#26 0x0000000004aa1bbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aa1bbe)
#27 0x0000000004c1027e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c1027e)
#28 0x0000000000d5a10f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5a10f)
#29 0x0000000000d518ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#30 0x0000000004897c99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#31 0x0000000003de33b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3de33b4)
#32 0x00000000048982af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#33 0x000000000485ad1d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485ad1d)
#34 0x000000000485bdae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485bdae)
#35 0x0000000004863485 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4863485)
#36 0x0000000000d56f08 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd56f08)
#37 0x0000000000c1dcc4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc1dcc4)
#38 0x000070ab83e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#39 0x000070ab83e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#40 0x0000000000d51375 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd51375)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Rush10233)

<details>
Goes back to clang-16: https://godbolt.org/z/8TYcah3rs

Test Input:

```c++
template &lt;class T&gt; struct A
{
  template &lt;class U&gt;
  A(U = 42);
};

template &lt;typename&gt;
struct B: A&lt;int&gt;
{
  using A::A;
};

B b(24);
```

Stack Dump:

```c++
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp:382: clang::NamedDecl* {anonymous}::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*): Assertion `Depth || cast&lt;ClassTemplateSpecializationDecl&gt;(FTD-&gt;getDeclContext()) -&gt;isExplicitSpecialization()`failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:13:8: current parser token ';'
 #<!-- -->0 0x0000000003e9fee8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9fee8)
 #<!-- -->1 0x0000000003e9db74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9db74)
 #<!-- -->2 0x0000000003de2f18 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070ab83e42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070ab83e969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070ab83e42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070ab83e287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070ab83e2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070ab83e39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000007021096 (anonymous namespace)::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*) SemaTemplateDeductionGuide.cpp:0:0
#<!-- -->10 0x0000000007023b5a clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7023b5a)
#<!-- -->11 0x0000000006c05d5f LookupDirect(clang::Sema&amp;, clang::LookupResult&amp;, clang::DeclContext const*) SemaLookup.cpp:0:0
#<!-- -->12 0x0000000006bf0ce2 clang::Sema::LookupQualifiedName(clang::LookupResult&amp;, clang::DeclContext*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf0ce2)
#<!-- -->13 0x0000000006bcf1de clang::Sema::DeduceTemplateSpecializationFromInitializer(clang::TypeSourceInfo*, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bcf1de)
#<!-- -->14 0x000000000684ca6d clang::Sema::deduceVarTypeFromInitializer(clang::VarDecl*, clang::DeclarationName, clang::QualType, clang::TypeSourceInfo*, clang::SourceRange, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684ca6d)
#<!-- -->15 0x0000000006851877 clang::Sema::DeduceVariableDeclarationType(clang::VarDecl*, bool, clang::Expr*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6851877)
#<!-- -->16 0x0000000006872862 clang::Sema::AddInitializerToDecl(clang::Decl*, clang::Expr*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6872862)
#<!-- -->17 0x00000000064fa2f9 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64fa2f9)
#<!-- -->18 0x0000000006509549 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6509549)
#<!-- -->19 0x00000000064c85ce clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c85ce)
#<!-- -->20 0x00000000064c8d89 clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c8d89)
#<!-- -->21 0x00000000064d0653 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d0653)
#<!-- -->22 0x00000000064d156d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d156d)
#<!-- -->23 0x00000000064c3aba clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c3aba)
#<!-- -->24 0x00000000048325b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48325b8)
#<!-- -->25 0x0000000004b1ff05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#<!-- -->26 0x0000000004aa1bbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aa1bbe)
#<!-- -->27 0x0000000004c1027e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c1027e)
#<!-- -->28 0x0000000000d5a10f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5a10f)
#<!-- -->29 0x0000000000d518ca ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->30 0x0000000004897c99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->31 0x0000000003de33b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3de33b4)
#<!-- -->32 0x00000000048982af clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->33 0x000000000485ad1d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485ad1d)
#<!-- -->34 0x000000000485bdae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485bdae)
#<!-- -->35 0x0000000004863485 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4863485)
#<!-- -->36 0x0000000000d56f08 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd56f08)
#<!-- -->37 0x0000000000c1dcc4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc1dcc4)
#<!-- -->38 0x000070ab83e29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->39 0x000070ab83e29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->40 0x0000000000d51375 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd51375)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```
</details>


---

### Comment 2 - MagentaTreehouse

Adding a deduction guide ~reveals a **regression in 20**~ results in a crash since 20:
```c++
template <class T> struct A
{
  template <class U>
  A(U);
};

template <typename>
struct B: A<int>
{
  using A::A;
};

template <class T>
B(T) -> B<T>;

B b(24);
```

See https://compiler-explorer.com/z/MKGEcMKhb.

Assertion:
```console
clang++: /root/llvm-project/clang/lib/Sema/SemaTemplateDeductionGuide.cpp:382:
clang::NamedDecl* {anonymous}::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*):
Assertion `Depth || cast<ClassTemplateSpecializationDecl>(FTD->getDeclContext()) ->isExplicitSpecialization()' failed.
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++17 <source>
1.	<source>:14:13: current parser token ';'
 #0 0x0000000003e9fee8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9fee8)
 #1 0x0000000003e9db74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3e9db74)
 #2 0x0000000003de2f18 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000720df1242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000720df12969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000720df1242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000720df12287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000720df122871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000720df1239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000007021096 (anonymous namespace)::ConvertConstructorToDeductionGuideTransform::transformConstructor(clang::FunctionTemplateDecl*, clang::CXXConstructorDecl*) SemaTemplateDeductionGuide.cpp:0:0
#10 0x0000000007023b5a clang::Sema::DeclareImplicitDeductionGuides(clang::TemplateDecl*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7023b5a)
#11 0x0000000006c05d5f LookupDirect(clang::Sema&, clang::LookupResult&, clang::DeclContext const*) SemaLookup.cpp:0:0
#12 0x0000000006c068c9 CppNamespaceLookup(clang::Sema&, clang::LookupResult&, clang::ASTContext&, clang::DeclContext const*, (anonymous namespace)::UnqualUsingDirectiveSet&) (.isra.0) SemaLookup.cpp:0:0
#13 0x0000000006bf3e82 clang::Sema::CppLookupName(clang::LookupResult&, clang::Scope*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf3e82)
#14 0x0000000006bf49d5 clang::Sema::LookupName(clang::LookupResult&, clang::Scope*, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6bf49d5)
#15 0x000000000689249c clang::Sema::HandleDeclarator(clang::Scope*, clang::Declarator&, llvm::MutableArrayRef<clang::TemplateParameterList*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x689249c)
#16 0x0000000006ec3921 clang::Sema::ActOnTemplateDeclarator(clang::Scope*, llvm::MutableArrayRef<clang::TemplateParameterList*>, clang::Declarator&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ec3921)
#17 0x00000000064f9aee clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64f9aee)
#18 0x0000000006509549 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6509549)
#19 0x00000000065d5e36 clang::Parser::ParseDeclarationAfterTemplate(clang::DeclaratorContext, clang::Parser::ParsedTemplateInfo&, clang::ParsingDeclRAIIObject&, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65d5e36)
#20 0x00000000065e392f clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (.part.0) ParseTemplate.cpp:0:0
#21 0x00000000065e3cda clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x65e3cda)
#22 0x0000000006512610 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6512610)
#23 0x00000000064d0697 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d0697)
#24 0x00000000064d156d clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64d156d)
#25 0x00000000064c3aba clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x64c3aba)
#26 0x00000000048325b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48325b8)
#27 0x0000000004b1ff05 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b1ff05)
#28 0x0000000004aa1bbe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aa1bbe)
#29 0x0000000004c1027e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c1027e)
#30 0x0000000000d5a10f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd5a10f)
#31 0x0000000000d518ca ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#32 0x0000000004897c99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x0000000003de33b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3de33b4)
#34 0x00000000048982af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x000000000485ad1d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485ad1d)
#36 0x000000000485bdae clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x485bdae)
#37 0x0000000004863485 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4863485)
#38 0x0000000000d56f08 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd56f08)
#39 0x0000000000c1dcc4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc1dcc4)
#40 0x0000720df1229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x0000720df1229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000d51375 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xd51375)
```

</details>

---

### Comment 3 - zyn0217

> Adding a deduction guide reveals a **regression in 20**:

That is strictly not a *regression*, instead it fails a sanity check for the bug that is pre-existing prior to 20:

https://compiler-explorer.com/z/x5d9b83Th

Looking at the AST, the synthesized deduction guide has a template parameter with an incorrect depth:

```
...
|-FunctionTemplateDecl <line:3:3, line:4:6> col:3 implicit <deduction guide for B>
| |-TemplateTypeParmDecl <line:7:11> col:19 typename depth 0 index 0
| |-TemplateTypeParmDecl <line:3:13, col:19> col:19 class depth 32767 index 1 U  <---- overflowed?
| `-CXXDeductionGuideDecl <line:4:3, col:6> col:3 implicit <deduction guide for B> 'auto (U) -> B<type-parameter-0-0>'
|   `-ParmVarDecl <col:5> col:6 'U'
...
```


---

### Comment 4 - shafik

Goes back to clang-5: https://godbolt.org/z/3exff77WW

You need to enable C++20.

---

