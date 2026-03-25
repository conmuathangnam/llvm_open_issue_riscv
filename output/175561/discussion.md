# [Clang] Crash in `BuildUsingDeclaration` with using-declaration of conversion operator to dependent nested type

**Author:** Attacker23
**URL:** https://github.com/llvm/llvm-project/issues/175561
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2026-01-12T18:40:30Z

## Body

A very old bug: it originates in Clang 3.0.0 (assertions build) and is still reproducible on trunk, while GCC/EDG and MSVC correctly reject the code instead of crashing.

### Minimal Reproduction

```cpp
template <class T>
struct A {
  struct N {};
  operator A<int>::N*();
};

template <class T>
struct B : A<T> {
  using A<T>::operator typename A<T>::N*;
};

B<int> x;
```

Clang crashes when a class template uses a `using` declaration to import a conversion operator from a dependent base class, where the conversion target type involves a nested type from another specialization.

The crash occurs in `Sema::BuildUsingDeclaration` with assertion `IdentLoc.isValid() && "Invalid TargetName location."` — it appears the `DeclarationNameInfo` for the conversion-function-id is not properly populated when the target type is dependent.



### Compiler Explorer

**https://godbolt.org/z/jhW9f6zKo**

### Compiler Output

```
<source>:4:12: error: implicit instantiation of template 'A<int>' within its own definition
    4 |   operator A<int>::N*();
      |            ^
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDeclCXX.cpp:13112: clang::NamedDecl* clang::Sema::BuildUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, bool, clang::SourceLocation, clang::CXXScopeSpec&, clang::DeclarationNameInfo, clang::SourceLocation, const clang::ParsedAttributesView&, bool, bool): Assertion `IdentLoc.isValid() && "Invalid TargetName location."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ferror-limit=0 <source>
1.	<source>:12:9: current parser token ';'
2.	<source>:8:8: instantiating class definition 'B<int>'
 #0 0x000000000430abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430abe8)
 #1 0x0000000004308014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4308014)
 #2 0x000000000424aa08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000763db0242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x0000763db02969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000763db0242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x0000763db02287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000763db022871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000763db0239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006ecf186 clang::Sema::BuildUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, bool, clang::SourceLocation, clang::CXXScopeSpec&, clang::DeclarationNameInfo, clang::SourceLocation, clang::ParsedAttributesView const&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ecf186)
#10 0x0000000007680cbf clang::Decl* clang::TemplateDeclInstantiator::instantiateUnresolvedUsingDecl<clang::UnresolvedUsingValueDecl>(clang::UnresolvedUsingValueDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7680cbf)
#11 0x00000000075ad0cb clang::Sema::InstantiateClassImpl(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75ad0cb)
#12 0x0000000007618fbe clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7618fbe)
#13 0x000000000769a20e void llvm::function_ref<void ()>::callback_fn<clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()>(long) SemaType.cpp:0:0
#14 0x0000000008539031 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8539031)
#15 0x00000000076a5a33 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76a5a33)
#16 0x00000000076a5ecf clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76a5ecf)
#17 0x00000000076a6535 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76a6535)
#18 0x0000000006e361f3 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#19 0x0000000006a8a12b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8a12b)
#20 0x0000000006a97e5a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a97e5a)
#21 0x0000000006a5555b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5555b)
#22 0x0000000006a55d4f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a55d4f)
#23 0x0000000006a5db7a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5db7a)
#24 0x0000000006a5eb15 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5eb15)
#25 0x0000000006a3fb9a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3fb9a)
#26 0x0000000004c6daf8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6daf8)
#27 0x0000000004f66ee5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f66ee5)
#28 0x0000000004ee6d1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee6d1e)
#29 0x000000000506158d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506158d)
#30 0x0000000000de8a9e cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde8a9e)
#31 0x0000000000ddf46a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#32 0x0000000000ddf5ed int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#33 0x0000000004ce0b39 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#34 0x000000000424aea4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424aea4)
#35 0x0000000004ce114f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#36 0x0000000004ca1ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca1ae2)
#37 0x0000000004ca2a8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a8e)
#38 0x0000000004ca9ee5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9ee5)
#39 0x0000000000de4e11 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4e11)
#40 0x0000000000c8f104 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8f104)
#41 0x0000763db0229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#42 0x0000763db0229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#43 0x0000000000ddef05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddef05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (Attacker23)

<details>
A very old bug: it originates in Clang 3.0.0 (assertions build) and is still reproducible on trunk, while GCC/EDG and MSVC correctly reject the code instead of crashing.

### Minimal Reproduction

```cpp
template &lt;class T&gt;
struct A {
  struct N {};
  operator A&lt;int&gt;::N*();
};

template &lt;class T&gt;
struct B : A&lt;T&gt; {
  using A&lt;T&gt;::operator typename A&lt;T&gt;::N*;
};

B&lt;int&gt; x;
```

Clang crashes when a class template uses a `using` declaration to import a conversion operator from a dependent base class, where the conversion target type involves a nested type from another specialization.

The crash occurs in `Sema::BuildUsingDeclaration` with assertion `IdentLoc.isValid() &amp;&amp; "Invalid TargetName location."` — it appears the `DeclarationNameInfo` for the conversion-function-id is not properly populated when the target type is dependent.



### Compiler Explorer

**https://godbolt.org/z/jhW9f6zKo**

### Compiler Output

```
&lt;source&gt;:4:12: error: implicit instantiation of template 'A&lt;int&gt;' within its own definition
    4 |   operator A&lt;int&gt;::N*();
      |            ^
clang++: /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDeclCXX.cpp:13112: clang::NamedDecl* clang::Sema::BuildUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, bool, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo, clang::SourceLocation, const clang::ParsedAttributesView&amp;, bool, bool): Assertion `IdentLoc.isValid() &amp;&amp; "Invalid TargetName location."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -ferror-limit=0 &lt;source&gt;
1.	&lt;source&gt;:12:9: current parser token ';'
2.	&lt;source&gt;:8:8: instantiating class definition 'B&lt;int&gt;'
 #<!-- -->0 0x000000000430abe8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x430abe8)
 #<!-- -->1 0x0000000004308014 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4308014)
 #<!-- -->2 0x000000000424aa08 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000763db0242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x0000763db02969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000763db0242476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x0000763db02287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000763db022871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000763db0239e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006ecf186 clang::Sema::BuildUsingDeclaration(clang::Scope*, clang::AccessSpecifier, clang::SourceLocation, bool, clang::SourceLocation, clang::CXXScopeSpec&amp;, clang::DeclarationNameInfo, clang::SourceLocation, clang::ParsedAttributesView const&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ecf186)
#<!-- -->10 0x0000000007680cbf clang::Decl* clang::TemplateDeclInstantiator::instantiateUnresolvedUsingDecl&lt;clang::UnresolvedUsingValueDecl&gt;(clang::UnresolvedUsingValueDecl*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7680cbf)
#<!-- -->11 0x00000000075ad0cb clang::Sema::InstantiateClassImpl(clang::SourceLocation, clang::CXXRecordDecl*, clang::CXXRecordDecl*, clang::MultiLevelTemplateArgumentList const&amp;, clang::TemplateSpecializationKind, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x75ad0cb)
#<!-- -->12 0x0000000007618fbe clang::Sema::InstantiateClassTemplateSpecialization(clang::SourceLocation, clang::ClassTemplateSpecializationDecl*, clang::TemplateSpecializationKind, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7618fbe)
#<!-- -->13 0x000000000769a20e void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*)::'lambda'()&gt;(long) SemaType.cpp:0:0
#<!-- -->14 0x0000000008539031 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x8539031)
#<!-- -->15 0x00000000076a5a33 clang::Sema::RequireCompleteTypeImpl(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76a5a33)
#<!-- -->16 0x00000000076a5ecf clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, clang::Sema::TypeDiagnoser&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76a5ecf)
#<!-- -->17 0x00000000076a6535 clang::Sema::RequireCompleteType(clang::SourceLocation, clang::QualType, clang::Sema::CompleteTypeKind, unsigned int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x76a6535)
#<!-- -->18 0x0000000006e361f3 clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#<!-- -->19 0x0000000006a8a12b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a8a12b)
#<!-- -->20 0x0000000006a97e5a clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a97e5a)
#<!-- -->21 0x0000000006a5555b clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5555b)
#<!-- -->22 0x0000000006a55d4f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a55d4f)
#<!-- -->23 0x0000000006a5db7a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5db7a)
#<!-- -->24 0x0000000006a5eb15 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a5eb15)
#<!-- -->25 0x0000000006a3fb9a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6a3fb9a)
#<!-- -->26 0x0000000004c6daf8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c6daf8)
#<!-- -->27 0x0000000004f66ee5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f66ee5)
#<!-- -->28 0x0000000004ee6d1e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ee6d1e)
#<!-- -->29 0x000000000506158d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x506158d)
#<!-- -->30 0x0000000000de8a9e cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde8a9e)
#<!-- -->31 0x0000000000ddf46a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->32 0x0000000000ddf5ed int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->33 0x0000000004ce0b39 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->34 0x000000000424aea4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x424aea4)
#<!-- -->35 0x0000000004ce114f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->36 0x0000000004ca1ae2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca1ae2)
#<!-- -->37 0x0000000004ca2a8e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca2a8e)
#<!-- -->38 0x0000000004ca9ee5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ca9ee5)
#<!-- -->39 0x0000000000de4e11 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xde4e11)
#<!-- -->40 0x0000000000c8f104 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc8f104)
#<!-- -->41 0x0000763db0229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->42 0x0000763db0229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->43 0x0000000000ddef05 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xddef05)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```


</details>


---

### Comment 2 - shafik

Looks like a dup: https://github.com/llvm/llvm-project/issues/140364

If you are generating these via fuzzing, then you should be focusing on recent regression rather than old bugs. We have so many fuzzing reports now that it is unlikely we will have the bandwidth to address these. 

---

### Comment 3 - Attacker23

@shafik Got it, thank you for your help and clarification!

---

