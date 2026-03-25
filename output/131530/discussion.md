# [clang] ICE in clang 19.1.0, 20.1.0, trunk  when accessing private member in concept

**Author:** n-morales
**URL:** https://github.com/llvm/llvm-project/issues/131530
**Status:** Closed
**Labels:** clang:frontend, crash, regression:19
**Closed Date:** 2025-03-17T08:53:58Z

## Body

Reproducer: https://godbolt.org/z/xon9j74Ke

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -Wall -Wextra -pedantic -Werror <source> -isystem/opt/compiler-explorer/libs/nlohmann_json/v3.11.1/single_include
1.	<eof> parser at end of file
 #0 0x0000000003b6fdb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b6fdb8)
 #1 0x0000000003b6defc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b6defc)
 #2 0x0000000003abd778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f61c8242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000000000719a100 clang::DeclContext::classof(clang::Decl const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x719a100)
 #5 0x00000000062c32de clang::Sema::HandleDelayedAccessCheck(clang::sema::DelayedDiagnostic&, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62c32de)
 #6 0x00000000064a29b1 clang::Sema::PopParsingDeclaration(clang::Sema::DelayedDiagnosticsState, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64a29b1)
 #7 0x00000000061f273d clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f273d)
 #8 0x00000000061f282f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f282f)
 #9 0x00000000061166e2 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&, clang::ParsedAttributes&, clang::ParsedAttributes&, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61166e2)
#10 0x00000000060cb932 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60cb932)
#11 0x00000000060cd1a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60cd1a8)
#12 0x00000000060bb7ea clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60bb7ea)
#13 0x00000000044ed26d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44ed26d)
#14 0x00000000047d10d1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47d10d1)
#15 0x000000000474fb1b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x474fb1b)
#16 0x00000000048c0f03 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48c0f03)
#17 0x0000000000d637b1 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd637b1)
#18 0x0000000000d5bfad ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#19 0x000000000453a569 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#20 0x0000000003abdb93 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3abdb93)
#21 0x000000000453a789 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#22 0x00000000044fdaad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44fdaad)
#23 0x00000000044fea71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44fea71)
#24 0x0000000004508e0c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4508e0c)
#25 0x0000000000d60581 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd60581)
#26 0x0000000000c21bc4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc21bc4)
#27 0x00007f61c8229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#28 0x00007f61c8229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#29 0x0000000000d5ba45 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5ba45)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Details of code that triggers this:

```cpp
class foo
{
    struct bar {};
};

template< typename T >
concept is_foo_concept = std::same_as< foo::bar, T >;
```

Causes the ICE. `foo::bar` should be inaccessible here and this should fail to compile but without the ICE.

e.g.:

```cpp
template< typename T >
constexpr inline bool is_foo_impl = std::same_as< foo::bar, T >;
template< typename T >
concept is_foo_concept = is_foo_impl< T >;
```

Works as expected and gives the following error (which is correct):

```
<source>:16:56: error: 'bar' is a private member of 'foo'
   16 | constexpr inline bool is_foo_impl = std::same_as< foo::bar, T >;
      |                                                        ^
<source>:6:12: note: implicitly declared private here
    6 |     struct bar {};
      |            ^
1 error generated.
```

## Comments

### Comment 1 - MagentaTreehouse

Assertion:
```console
clang++: /root/llvm-project/llvm/include/llvm/Support/Casting.h:109:
static bool llvm::isa_impl_cl<To, const From*>::doit(const From*) [with To = clang::DeclContext; From = clang::NamedDecl]:
Assertion `Val && "isa<> used on a null pointer"' failed
```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (n-morales)

<details>
Reproducer: https://godbolt.org/z/xon9j74Ke

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++23 -Wall -Wextra -pedantic -Werror &lt;source&gt; -isystem/opt/compiler-explorer/libs/nlohmann_json/v3.11.1/single_include
1.	&lt;eof&gt; parser at end of file
 #<!-- -->0 0x0000000003b6fdb8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b6fdb8)
 #<!-- -->1 0x0000000003b6defc llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b6defc)
 #<!-- -->2 0x0000000003abd778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007f61c8242520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000000000719a100 clang::DeclContext::classof(clang::Decl const*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x719a100)
 #<!-- -->5 0x00000000062c32de clang::Sema::HandleDelayedAccessCheck(clang::sema::DelayedDiagnostic&amp;, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x62c32de)
 #<!-- -->6 0x00000000064a29b1 clang::Sema::PopParsingDeclaration(clang::Sema::DelayedDiagnosticsState, clang::Decl*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x64a29b1)
 #<!-- -->7 0x00000000061f273d clang::Parser::ParseTemplateDeclarationOrSpecialization(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f273d)
 #<!-- -->8 0x00000000061f282f clang::Parser::ParseDeclarationStartingWithTemplate(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61f282f)
 #<!-- -->9 0x00000000061166e2 clang::Parser::ParseDeclaration(clang::DeclaratorContext, clang::SourceLocation&amp;, clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::SourceLocation*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61166e2)
#<!-- -->10 0x00000000060cb932 clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60cb932)
#<!-- -->11 0x00000000060cd1a8 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60cd1a8)
#<!-- -->12 0x00000000060bb7ea clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x60bb7ea)
#<!-- -->13 0x00000000044ed26d clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44ed26d)
#<!-- -->14 0x00000000047d10d1 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47d10d1)
#<!-- -->15 0x000000000474fb1b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x474fb1b)
#<!-- -->16 0x00000000048c0f03 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48c0f03)
#<!-- -->17 0x0000000000d637b1 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd637b1)
#<!-- -->18 0x0000000000d5bfad ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->19 0x000000000453a569 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->20 0x0000000003abdb93 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3abdb93)
#<!-- -->21 0x000000000453a789 clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->22 0x00000000044fdaad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44fdaad)
#<!-- -->23 0x00000000044fea71 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x44fea71)
#<!-- -->24 0x0000000004508e0c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4508e0c)
#<!-- -->25 0x0000000000d60581 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd60581)
#<!-- -->26 0x0000000000c21bc4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc21bc4)
#<!-- -->27 0x00007f61c8229d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->28 0x00007f61c8229e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->29 0x0000000000d5ba45 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd5ba45)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Compiler returned: 139
```

Details of code that triggers this:

```cpp
class foo
{
    struct bar {};
};

template&lt; typename T &gt;
concept is_foo_concept = std::same_as&lt; foo::bar, T &gt;;
```

Causes the ICE. `foo::bar` should be inaccessible here and this should fail to compile but without the ICE.

e.g.:

```cpp
template&lt; typename T &gt;
constexpr inline bool is_foo_impl = std::same_as&lt; foo::bar, T &gt;;
template&lt; typename T &gt;
concept is_foo_concept = is_foo_impl&lt; T &gt;;
```

Works as expected and gives the following error (which is correct):

```
&lt;source&gt;:16:56: error: 'bar' is a private member of 'foo'
   16 | constexpr inline bool is_foo_impl = std::same_as&lt; foo::bar, T &gt;;
      |                                                        ^
&lt;source&gt;:6:12: note: implicitly declared private here
    6 |     struct bar {};
      |            ^
1 error generated.
```
</details>


---

### Comment 3 - zyn0217

Started since 19 https://godbolt.org/z/T8e4ocxe1

---

### Comment 4 - shafik

@Endilll git bisect points to a68d20e986053ec571223a9f3ead3e146a27dc82 as the cause of this regression

---

