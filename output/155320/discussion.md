# [clang] UNREACHABLE executed at ...SemaDeclCXX.cpp: "should not generate implicit declarations for dependent cases"

**Author:** alexfh
**URL:** https://github.com/llvm/llvm-project/issues/155320

## Body

The [following invalid code](https://github.com/llvm/llvm-project/pull/151654#issuecomment-3207007466) crashes Clang:
```
template<class>
struct Trait;

template<class T>
constexpr bool TraitV = Trait<T>::value;

template<class T>
struct S {
    S() noexcept(TraitV<T>) = default;
};

template<class T>
struct S2 : S<T> {};

S2<char> s;
```

Godbolt link: https://gcc.godbolt.org/z/YEvdq5oha

Clang output:
```
<source>:5:25: error: implicit instantiation of undefined template 'Trait<char>'
    5 | constexpr bool TraitV = Trait<T>::value;
      |                         ^
<source>:9:18: note: in instantiation of variable template specialization 'TraitV<char>' requested here
    9 |     S() noexcept(TraitV<T>) = default;
      |                  ^
<source>:13:13: note: in instantiation of exception specification for 'S' requested here
   13 | struct S2 : S<T> {};
      |             ^
<source>:15:10: note: in evaluation of exception specification for 'S2<char>::S2' needed here
   15 | S2<char> s;
      |          ^
<source>:2:8: note: template is declared here
    2 | struct Trait;
      |        ^
should not generate implicit declarations for dependent cases
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDeclCXX.cpp:237!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<source>:15:11: current parser token ';'
 #0 0x0000000004047698 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4047698)
 #1 0x0000000004044ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4044ac4)
 #2 0x0000000003f89208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007303a1c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007303a1c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007303a1c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007303a1c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003f948da (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f948da)
 #8 0x0000000006b1c384 clang::Sema::ImplicitExceptionSpecification::CalledDecl(clang::SourceLocation, clang::CXXMethodDecl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1c384)
 #9 0x0000000006b1c5c3 (anonymous namespace)::SpecialMemberExceptionSpecInfo::visitClassSubobject(clang::CXXRecordDecl*, llvm::PointerUnion<clang::CXXBaseSpecifier*, clang::FieldDecl*>, unsigned int) SemaDeclCXX.cpp:0:0
#10 0x0000000006b7597d (anonymous namespace)::SpecialMemberExceptionSpecInfo::visitBase(clang::CXXBaseSpecifier*) SemaDeclCXX.cpp:0:0
#11 0x0000000006b75d2a ComputeDefaultedSpecialMemberExceptionSpec(clang::Sema&, clang::SourceLocation, clang::CXXMethodDecl*, clang::CXXSpecialMemberKind, clang::Sema::InheritedConstructorInfo*) SemaDeclCXX.cpp:0:0
#12 0x0000000006b764dd clang::Sema::EvaluateImplicitExceptionSpec(clang::SourceLocation, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b764dd)
#13 0x0000000006ba940d clang::Sema::ResolveExceptionSpec(clang::SourceLocation, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ba940d)
#14 0x0000000006b81ec5 clang::Sema::DefineImplicitDefaultConstructor(clang::SourceLocation, clang::CXXConstructorDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b81ec5)
#15 0x0000000007fe3a61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7fe3a61)
#16 0x0000000006d76d8a PerformConstructorInitialization(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::InitializationSequence::Step const&, bool&, bool, bool, clang::SourceLocation, clang::SourceLocation) SemaInit.cpp:0:0
#17 0x0000000006d86432 clang::InitializationSequence::Perform(clang::Sema&, clang::InitializedEntity const&, clang::InitializationKind const&, llvm::MutableArrayRef<clang::Expr*>, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d86432)
#18 0x0000000006aaf35a clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#19 0x000000000671696b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671696b)
#20 0x0000000006725d69 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6725d69)
#21 0x00000000066e30de clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e30de)
#22 0x00000000066e387f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e387f)
#23 0x00000000066eb6ba clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66eb6ba)
#24 0x00000000066ec655 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ec655)
#25 0x00000000066de1da clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66de1da)
#26 0x00000000049bb8f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bb8f8)
#27 0x0000000004cb2225 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb2225)
#28 0x0000000004c2e51e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2e51e)
#29 0x0000000004da41e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4da41e1)
#30 0x0000000000db3ebf cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb3ebf)
#31 0x0000000000daab6a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#32 0x0000000004a234b9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#33 0x0000000003f896a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f896a4)
#34 0x0000000004a23acf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#35 0x00000000049e5bad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e5bad)
#36 0x00000000049e6c3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e6c3e)
#37 0x00000000049ee475 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ee475)
#38 0x0000000000db0375 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb0375)
#39 0x0000000000c639e4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc639e4)
#40 0x00007303a1c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#41 0x00007303a1c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#42 0x0000000000daa615 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa615)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

The test case was reduced by @frederick-vs-ja from a valid code that includes a libc++ version with a bug (which was fixed since then): https://github.com/llvm/llvm-project/pull/151654#issuecomment-3205410955



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Alexander Kornienko (alexfh)

<details>
The [following invalid code](https://github.com/llvm/llvm-project/pull/151654#issuecomment-3207007466) crashes Clang:
```
template&lt;class&gt;
struct Trait;

template&lt;class T&gt;
constexpr bool TraitV = Trait&lt;T&gt;::value;

template&lt;class T&gt;
struct S {
    S() noexcept(TraitV&lt;T&gt;) = default;
};

template&lt;class T&gt;
struct S2 : S&lt;T&gt; {};

S2&lt;char&gt; s;
```

Godbolt link: https://gcc.godbolt.org/z/YEvdq5oha

Clang output:
```
&lt;source&gt;:5:25: error: implicit instantiation of undefined template 'Trait&lt;char&gt;'
    5 | constexpr bool TraitV = Trait&lt;T&gt;::value;
      |                         ^
&lt;source&gt;:9:18: note: in instantiation of variable template specialization 'TraitV&lt;char&gt;' requested here
    9 |     S() noexcept(TraitV&lt;T&gt;) = default;
      |                  ^
&lt;source&gt;:13:13: note: in instantiation of exception specification for 'S' requested here
   13 | struct S2 : S&lt;T&gt; {};
      |             ^
&lt;source&gt;:15:10: note: in evaluation of exception specification for 'S2&lt;char&gt;::S2' needed here
   15 | S2&lt;char&gt; s;
      |          ^
&lt;source&gt;:2:8: note: template is declared here
    2 | struct Trait;
      |        ^
should not generate implicit declarations for dependent cases
UNREACHABLE executed at /root/llvm-project/llvm/tools/clang/lib/Sema/SemaDeclCXX.cpp:237!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;source&gt;:15:11: current parser token ';'
 #<!-- -->0 0x0000000004047698 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4047698)
 #<!-- -->1 0x0000000004044ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4044ac4)
 #<!-- -->2 0x0000000003f89208 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007303a1c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007303a1c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007303a1c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007303a1c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003f948da (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f948da)
 #<!-- -->8 0x0000000006b1c384 clang::Sema::ImplicitExceptionSpecification::CalledDecl(clang::SourceLocation, clang::CXXMethodDecl const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b1c384)
 #<!-- -->9 0x0000000006b1c5c3 (anonymous namespace)::SpecialMemberExceptionSpecInfo::visitClassSubobject(clang::CXXRecordDecl*, llvm::PointerUnion&lt;clang::CXXBaseSpecifier*, clang::FieldDecl*&gt;, unsigned int) SemaDeclCXX.cpp:0:0
#<!-- -->10 0x0000000006b7597d (anonymous namespace)::SpecialMemberExceptionSpecInfo::visitBase(clang::CXXBaseSpecifier*) SemaDeclCXX.cpp:0:0
#<!-- -->11 0x0000000006b75d2a ComputeDefaultedSpecialMemberExceptionSpec(clang::Sema&amp;, clang::SourceLocation, clang::CXXMethodDecl*, clang::CXXSpecialMemberKind, clang::Sema::InheritedConstructorInfo*) SemaDeclCXX.cpp:0:0
#<!-- -->12 0x0000000006b764dd clang::Sema::EvaluateImplicitExceptionSpec(clang::SourceLocation, clang::FunctionDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b764dd)
#<!-- -->13 0x0000000006ba940d clang::Sema::ResolveExceptionSpec(clang::SourceLocation, clang::FunctionProtoType const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6ba940d)
#<!-- -->14 0x0000000006b81ec5 clang::Sema::DefineImplicitDefaultConstructor(clang::SourceLocation, clang::CXXConstructorDecl*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6b81ec5)
#<!-- -->15 0x0000000007fe3a61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7fe3a61)
#<!-- -->16 0x0000000006d76d8a PerformConstructorInitialization(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::InitializationSequence::Step const&amp;, bool&amp;, bool, bool, clang::SourceLocation, clang::SourceLocation) SemaInit.cpp:0:0
#<!-- -->17 0x0000000006d86432 clang::InitializationSequence::Perform(clang::Sema&amp;, clang::InitializedEntity const&amp;, clang::InitializationKind const&amp;, llvm::MutableArrayRef&lt;clang::Expr*&gt;, clang::QualType*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6d86432)
#<!-- -->18 0x0000000006aaf35a clang::Sema::ActOnUninitializedDecl(clang::Decl*) (.part.0) SemaDecl.cpp:0:0
#<!-- -->19 0x000000000671696b clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x671696b)
#<!-- -->20 0x0000000006725d69 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6725d69)
#<!-- -->21 0x00000000066e30de clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e30de)
#<!-- -->22 0x00000000066e387f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66e387f)
#<!-- -->23 0x00000000066eb6ba clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66eb6ba)
#<!-- -->24 0x00000000066ec655 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66ec655)
#<!-- -->25 0x00000000066de1da clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x66de1da)
#<!-- -->26 0x00000000049bb8f8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49bb8f8)
#<!-- -->27 0x0000000004cb2225 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4cb2225)
#<!-- -->28 0x0000000004c2e51e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c2e51e)
#<!-- -->29 0x0000000004da41e1 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4da41e1)
#<!-- -->30 0x0000000000db3ebf cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb3ebf)
#<!-- -->31 0x0000000000daab6a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->32 0x0000000004a234b9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->33 0x0000000003f896a4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f896a4)
#<!-- -->34 0x0000000004a23acf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->35 0x00000000049e5bad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e5bad)
#<!-- -->36 0x00000000049e6c3e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49e6c3e)
#<!-- -->37 0x00000000049ee475 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49ee475)
#<!-- -->38 0x0000000000db0375 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb0375)
#<!-- -->39 0x0000000000c639e4 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc639e4)
#<!-- -->40 0x00007303a1c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->41 0x00007303a1c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->42 0x0000000000daa615 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaa615)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

The test case was reduced by @<!-- -->frederick-vs-ja from a valid code that includes a libc++ version with a bug (which was fixed since then): https://github.com/llvm/llvm-project/pull/151654#issuecomment-3205410955


</details>


---

### Comment 2 - alexfh

The issue reproduces back to LLVM 16.

There's https://github.com/llvm/llvm-project/issues/112915 with the same manifestation, but the test case seems to be substantially different, so I filed a separated bug.

---

### Comment 3 - shafik

@hokein git bisect points to e0cdafe8d4b2f1585f4756447b677fec37954ec4

---

