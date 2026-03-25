# [clang] Assertion `operator bool()' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/159563
**Status:** Closed
**Labels:** clang:frontend, crash-on-invalid, regression:21
**Closed Date:** 2025-09-19T19:18:22Z

## Body

Reproducer:
https://godbolt.org/z/dhThazoWj
```cpp
namespace std {
template <typename T> struct tuple_size;
}

struct S {
  int a;
};

template <> struct std::tuple_size<S> {
  static constexpr int value = -1;
};

int e = __builtin_structured_binding_size(S);
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Basic/UnsignedOrNone.h:24: clang::UnsignedOrNone::UnsignedOrNone(unsigned int): Assertion `operator bool()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<source>:13:45: current parser token ';'
 #0 0x0000000004189508 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4189508)
 #1 0x0000000004186934 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4186934)
 #2 0x00000000040cafc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000077f1a1442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000077f1a14969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000077f1a1442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000077f1a14287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000077f1a142871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000077f1a1439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000006cadd01 clang::Sema::GetDecompositionElementCount(clang::QualType, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cadd01)
#10 0x000000000744bf38 clang::Sema::BuildTypeTrait(clang::TypeTrait, clang::SourceLocation, llvm::ArrayRef<clang::TypeSourceInfo*>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x744bf38)
#11 0x000000000744c77b clang::Sema::ActOnTypeTrait(clang::TypeTrait, clang::SourceLocation, llvm::ArrayRef<clang::OpaquePtr<clang::QualType>>, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x744c77b)
#12 0x00000000068d7f60 clang::Parser::ParseTypeTrait() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d7f60)
#13 0x00000000068c8145 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c8145)
#14 0x00000000068ca3d7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68ca3d7)
#15 0x00000000068ca469 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68ca469)
#16 0x00000000068833b8 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&, clang::Parser::ParsedTemplateInfo const&, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68833b8)
#17 0x0000000006890739 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&, clang::DeclaratorContext, clang::ParsedAttributes&, clang::Parser::ParsedTemplateInfo&, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6890739)
#18 0x000000000684da9e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec&, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684da9e)
#19 0x000000000684e23f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684e23f)
#20 0x000000000685607a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&, clang::ParsedAttributes&, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x685607a)
#21 0x0000000006857015 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6857015)
#22 0x0000000006837a8a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6837a8a)
#23 0x0000000004ac34d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ac34d8)
#24 0x0000000004dae905 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dae905)
#25 0x0000000004d2a7de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2a7de)
#26 0x0000000004ea3e7d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea3e7d)
#27 0x0000000000db6f00 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb6f00)
#28 0x0000000000dadb1a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#29 0x0000000004b2c4f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#30 0x00000000040cb464 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40cb464)
#31 0x0000000004b2cb0f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#32 0x0000000004aedf52 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aedf52)
#33 0x0000000004aeeefe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aeeefe)
#34 0x0000000004af6625 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af6625)
#35 0x0000000000db337a clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb337a)
#36 0x0000000000c65044 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc65044)
#37 0x000077f1a1429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#38 0x000077f1a1429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#39 0x0000000000dad5c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdad5c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/dhThazoWj
```cpp
namespace std {
template &lt;typename T&gt; struct tuple_size;
}

struct S {
  int a;
};

template &lt;&gt; struct std::tuple_size&lt;S&gt; {
  static constexpr int value = -1;
};

int e = __builtin_structured_binding_size(S);
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/include/clang/Basic/UnsignedOrNone.h:24: clang::UnsignedOrNone::UnsignedOrNone(unsigned int): Assertion `operator bool()' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;source&gt;:13:45: current parser token ';'
 #<!-- -->0 0x0000000004189508 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4189508)
 #<!-- -->1 0x0000000004186934 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4186934)
 #<!-- -->2 0x00000000040cafc8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000077f1a1442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000077f1a14969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000077f1a1442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000077f1a14287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000077f1a142871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000077f1a1439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000006cadd01 clang::Sema::GetDecompositionElementCount(clang::QualType, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6cadd01)
#<!-- -->10 0x000000000744bf38 clang::Sema::BuildTypeTrait(clang::TypeTrait, clang::SourceLocation, llvm::ArrayRef&lt;clang::TypeSourceInfo*&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x744bf38)
#<!-- -->11 0x000000000744c77b clang::Sema::ActOnTypeTrait(clang::TypeTrait, clang::SourceLocation, llvm::ArrayRef&lt;clang::OpaquePtr&lt;clang::QualType&gt;&gt;, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x744c77b)
#<!-- -->12 0x00000000068d7f60 clang::Parser::ParseTypeTrait() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68d7f60)
#<!-- -->13 0x00000000068c8145 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, bool&amp;, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c8145)
#<!-- -->14 0x00000000068ca3d7 clang::Parser::ParseCastExpression(clang::CastParseKind, bool, clang::TypoCorrectionTypeBehavior, bool, bool*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68ca3d7)
#<!-- -->15 0x00000000068ca469 clang::Parser::ParseAssignmentExpression(clang::TypoCorrectionTypeBehavior) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68ca469)
#<!-- -->16 0x00000000068833b8 clang::Parser::ParseDeclarationAfterDeclaratorAndAttributes(clang::Declarator&amp;, clang::Parser::ParsedTemplateInfo const&amp;, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68833b8)
#<!-- -->17 0x0000000006890739 clang::Parser::ParseDeclGroup(clang::ParsingDeclSpec&amp;, clang::DeclaratorContext, clang::ParsedAttributes&amp;, clang::Parser::ParsedTemplateInfo&amp;, clang::SourceLocation*, clang::Parser::ForRangeInit*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6890739)
#<!-- -->18 0x000000000684da9e clang::Parser::ParseDeclOrFunctionDefInternal(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec&amp;, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684da9e)
#<!-- -->19 0x000000000684e23f clang::Parser::ParseDeclarationOrFunctionDefinition(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*, clang::AccessSpecifier) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x684e23f)
#<!-- -->20 0x000000000685607a clang::Parser::ParseExternalDeclaration(clang::ParsedAttributes&amp;, clang::ParsedAttributes&amp;, clang::ParsingDeclSpec*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x685607a)
#<!-- -->21 0x0000000006857015 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr&lt;clang::DeclGroupRef&gt;&amp;, clang::Sema::ModuleImportState&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6857015)
#<!-- -->22 0x0000000006837a8a clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6837a8a)
#<!-- -->23 0x0000000004ac34d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ac34d8)
#<!-- -->24 0x0000000004dae905 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dae905)
#<!-- -->25 0x0000000004d2a7de clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2a7de)
#<!-- -->26 0x0000000004ea3e7d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ea3e7d)
#<!-- -->27 0x0000000000db6f00 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb6f00)
#<!-- -->28 0x0000000000dadb1a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->29 0x0000000004b2c4f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->30 0x00000000040cb464 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40cb464)
#<!-- -->31 0x0000000004b2cb0f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->32 0x0000000004aedf52 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aedf52)
#<!-- -->33 0x0000000004aeeefe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aeeefe)
#<!-- -->34 0x0000000004af6625 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4af6625)
#<!-- -->35 0x0000000000db337a clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb337a)
#<!-- -->36 0x0000000000c65044 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc65044)
#<!-- -->37 0x000077f1a1429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->38 0x000077f1a1429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->39 0x0000000000dad5c5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdad5c5)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/fc7ja9eas

---

### Comment 3 - tbaederr

https://github.com/llvm/llvm-project/blob/d62505f77d7aad8379b3d6f54b19bac1758ca1c2/clang/lib/Sema/SemaDeclCXX.cpp#L1693-L1701

`TupleSize` here is `-1`. Is that value wrong or should `isTupleLike()` return something else? @cor3ntin 

---

### Comment 4 - shafik

@mizvekov git bisect points to cfee056b4e75cd941591d298e0f8dc303460c57e

---

### Comment 5 - shafik

I wanted to clarify that I consider this a regression even though `isTupleLike` already had a bug, and the change surfaced it (which is ultimately good), crashing is a change in behavior that can be very impactful to existing workflows.

---

