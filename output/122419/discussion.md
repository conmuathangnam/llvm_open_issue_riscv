# [Clang] Crash involving lambda in unevaluated context invoked in a function template with auto return type

**Author:** MagentaTreehouse
**URL:** https://github.com/llvm/llvm-project/issues/122419

## Body

The following C++20 code causes a crash:
```c++
template <class F = decltype([] {})>
struct A {
    F funcObj;
};

template <int>
auto f() {
    A{}.funcObj();
}

int main() {
    f<0>();
}
```

See https://compiler-explorer.com/z/qbfWh9YKd.

Error:
```console
<source>:12:5: error: cannot compile this l-value expression yet
   12 |     f<0>();
      |     ^~~~~~
```

Unreachable:
```console
Unexpected placeholder builtin type!
UNREACHABLE executed at /root/llvm-project/clang/lib/CodeGen/CodeGenTypes.cpp:581!
```

<details>

<summary>Stack dump</summary>

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 <source>
1.	<eof> parser at end of file
2.	<source>:11:5: LLVM IR generation of declaration 'main'
3.	<source>:11:5: Generating code for declaration 'main'
 #0 0x0000000003c8d238 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c8d238)
 #1 0x0000000003c8af44 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c8af44)
 #2 0x0000000003bd8488 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007e7e7a442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007e7e7a4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007e7e7a442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007e7e7a4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003be3dea (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3be3dea)
 #8 0x000000000413c26b clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x413c26b)
 #9 0x00000000044778c0 clang::CodeGen::CodeGenFunction::EmitUnsupportedLValue(clang::Expr const*, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44778c0)
#10 0x00000000044a7c85 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a7c85)
#11 0x00000000044a87e7 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()>(long) CGExpr.cpp:0:0
#12 0x0000000007dd1f61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dd1f61)
#13 0x0000000004477bfa clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4477bfa)
#14 0x00000000044a8052 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a8052)
#15 0x00000000044a87e7 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()>(long) CGExpr.cpp:0:0
#16 0x0000000007dd1f61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dd1f61)
#17 0x0000000004477bfa clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4477bfa)
#18 0x00000000044a1a4e clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a1a4e)
#19 0x0000000003ffb33e clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ffb33e)
#20 0x00000000040030cc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40030cc)
#21 0x000000000406c6bc clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x406c6bc)
#22 0x000000000407f84e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x407f84e)
#23 0x00000000040e8a4a clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40e8a4a)
#24 0x00000000040e3cd5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40e3cd5)
#25 0x00000000040e40bb clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40e40bb)
#26 0x00000000040ee3e3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#27 0x0000000004623366 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#28 0x0000000004613ed8 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4613ed8)
#29 0x0000000006631cb4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631cb4)
#30 0x0000000004620ec8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4620ec8)
#31 0x00000000048ddcc9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ddcc9)
#32 0x000000000486039e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486039e)
#33 0x00000000049cb04e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cb04e)
#34 0x0000000000ced85f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xced85f)
#35 0x0000000000ce532a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#36 0x0000000004666b79 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#37 0x0000000003bd8934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bd8934)
#38 0x000000000466716f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#39 0x000000000462ac3d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x462ac3d)
#40 0x000000000462bcbe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x462bcbe)
#41 0x00000000046337e5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46337e5)
#42 0x0000000000cea6b3 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcea6b3)
#43 0x0000000000bb6854 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbb6854)
#44 0x00007e7e7a429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#45 0x00007e7e7a429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#46 0x0000000000ce4dd5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce4dd5)
```

</details>

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (MagentaTreehouse)

<details>
The following C++20 code causes a crash:
```c++
template &lt;class F = decltype([] {})&gt;
struct A {
    F funcObj;
};

template &lt;int&gt;
auto f() {
    A{}.funcObj();
}

int main() {
    f&lt;0&gt;();
}
```

See https://compiler-explorer.com/z/qbfWh9YKd.

Error:
```concole
&lt;source&gt;:12:5: error: cannot compile this l-value expression yet
   12 |     f&lt;0&gt;();
      |     ^~~~~~
```

Unreachable:
```console
Unexpected placeholder builtin type!
UNREACHABLE executed at /root/llvm-project/clang/lib/CodeGen/CodeGenTypes.cpp:581!
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:11:5: LLVM IR generation of declaration 'main'
3.	&lt;source&gt;:11:5: Generating code for declaration 'main'
 #<!-- -->0 0x0000000003c8d238 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c8d238)
 #<!-- -->1 0x0000000003c8af44 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c8af44)
 #<!-- -->2 0x0000000003bd8488 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e7e7a442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e7e7a4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e7e7a442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e7e7a4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003be3dea (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3be3dea)
 #<!-- -->8 0x000000000413c26b clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x413c26b)
 #<!-- -->9 0x00000000044778c0 clang::CodeGen::CodeGenFunction::EmitUnsupportedLValue(clang::Expr const*, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44778c0)
#<!-- -->10 0x00000000044a7c85 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a7c85)
#<!-- -->11 0x00000000044a87e7 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()&gt;(long) CGExpr.cpp:0:0
#<!-- -->12 0x0000000007dd1f61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dd1f61)
#<!-- -->13 0x0000000004477bfa clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4477bfa)
#<!-- -->14 0x00000000044a8052 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a8052)
#<!-- -->15 0x00000000044a87e7 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()&gt;(long) CGExpr.cpp:0:0
#<!-- -->16 0x0000000007dd1f61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dd1f61)
#<!-- -->17 0x0000000004477bfa clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4477bfa)
#<!-- -->18 0x00000000044a1a4e clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a1a4e)
#<!-- -->19 0x0000000003ffb33e clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ffb33e)
#<!-- -->20 0x00000000040030cc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40030cc)
#<!-- -->21 0x000000000406c6bc clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x406c6bc)
#<!-- -->22 0x000000000407f84e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x407f84e)
#<!-- -->23 0x00000000040e8a4a clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40e8a4a)
#<!-- -->24 0x00000000040e3cd5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40e3cd5)
#<!-- -->25 0x00000000040e40bb clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40e40bb)
#<!-- -->26 0x00000000040ee3e3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->27 0x0000000004623366 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->28 0x0000000004613ed8 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4613ed8)
#<!-- -->29 0x0000000006631cb4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631cb4)
#<!-- -->30 0x0000000004620ec8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4620ec8)
#<!-- -->31 0x00000000048ddcc9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ddcc9)
#<!-- -->32 0x000000000486039e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486039e)
#<!-- -->33 0x00000000049cb04e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cb04e)
#<!-- -->34 0x0000000000ced85f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xced85f)
#<!-- -->35 0x0000000000ce532a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000004666b79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x0000000003bd8934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bd8934)
#<!-- -->38 0x000000000466716f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x000000000462ac3d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x462ac3d)
#<!-- -->40 0x000000000462bcbe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x462bcbe)
#<!-- -->41 0x00000000046337e5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46337e5)
#<!-- -->42 0x0000000000cea6b3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcea6b3)
#<!-- -->43 0x0000000000bb6854 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbb6854)
#<!-- -->44 0x00007e7e7a429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x00007e7e7a429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000ce4dd5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce4dd5)
```

&lt;/details&gt;
</details>


---

### Comment 2 - shafik

Goes back to clang-13: https://compiler-explorer.com/z/5o49ce1jj

Looks to be failing here: https://github.com/llvm/llvm-project/blob/a531800344dc54e9c197a13b22e013f919f3f5e1/clang/lib/CodeGen/CGExpr.cpp#L1549-L1551

b/c we are not handling `UnresolvedLookupExpr`

---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (MagentaTreehouse)

<details>
The following C++20 code causes a crash:
```c++
template &lt;class F = decltype([] {})&gt;
struct A {
    F funcObj;
};

template &lt;int&gt;
auto f() {
    A{}.funcObj();
}

int main() {
    f&lt;0&gt;();
}
```

See https://compiler-explorer.com/z/qbfWh9YKd.

Error:
```concole
&lt;source&gt;:12:5: error: cannot compile this l-value expression yet
   12 |     f&lt;0&gt;();
      |     ^~~~~~
```

Unreachable:
```console
Unexpected placeholder builtin type!
UNREACHABLE executed at /root/llvm-project/clang/lib/CodeGen/CodeGenTypes.cpp:581!
```

&lt;details&gt;

&lt;summary&gt;Stack dump&lt;/summary&gt;

```console
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:11:5: LLVM IR generation of declaration 'main'
3.	&lt;source&gt;:11:5: Generating code for declaration 'main'
 #<!-- -->0 0x0000000003c8d238 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c8d238)
 #<!-- -->1 0x0000000003c8af44 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c8af44)
 #<!-- -->2 0x0000000003bd8488 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007e7e7a442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007e7e7a4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007e7e7a442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007e7e7a4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003be3dea (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3be3dea)
 #<!-- -->8 0x000000000413c26b clang::CodeGen::CodeGenTypes::ConvertType(clang::QualType) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x413c26b)
 #<!-- -->9 0x00000000044778c0 clang::CodeGen::CodeGenFunction::EmitUnsupportedLValue(clang::Expr const*, char const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44778c0)
#<!-- -->10 0x00000000044a7c85 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a7c85)
#<!-- -->11 0x00000000044a87e7 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()&gt;(long) CGExpr.cpp:0:0
#<!-- -->12 0x0000000007dd1f61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dd1f61)
#<!-- -->13 0x0000000004477bfa clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4477bfa)
#<!-- -->14 0x00000000044a8052 clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a8052)
#<!-- -->15 0x00000000044a87e7 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()&gt;(long) CGExpr.cpp:0:0
#<!-- -->16 0x0000000007dd1f61 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x7dd1f61)
#<!-- -->17 0x0000000004477bfa clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4477bfa)
#<!-- -->18 0x00000000044a1a4e clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44a1a4e)
#<!-- -->19 0x0000000003ffb33e clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ffb33e)
#<!-- -->20 0x00000000040030cc clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40030cc)
#<!-- -->21 0x000000000406c6bc clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x406c6bc)
#<!-- -->22 0x000000000407f84e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x407f84e)
#<!-- -->23 0x00000000040e8a4a clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40e8a4a)
#<!-- -->24 0x00000000040e3cd5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40e3cd5)
#<!-- -->25 0x00000000040e40bb clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40e40bb)
#<!-- -->26 0x00000000040ee3e3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->27 0x0000000004623366 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->28 0x0000000004613ed8 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4613ed8)
#<!-- -->29 0x0000000006631cb4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6631cb4)
#<!-- -->30 0x0000000004620ec8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4620ec8)
#<!-- -->31 0x00000000048ddcc9 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48ddcc9)
#<!-- -->32 0x000000000486039e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x486039e)
#<!-- -->33 0x00000000049cb04e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x49cb04e)
#<!-- -->34 0x0000000000ced85f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xced85f)
#<!-- -->35 0x0000000000ce532a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->36 0x0000000004666b79 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->37 0x0000000003bd8934 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3bd8934)
#<!-- -->38 0x000000000466716f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->39 0x000000000462ac3d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x462ac3d)
#<!-- -->40 0x000000000462bcbe clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x462bcbe)
#<!-- -->41 0x00000000046337e5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46337e5)
#<!-- -->42 0x0000000000cea6b3 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xcea6b3)
#<!-- -->43 0x0000000000bb6854 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbb6854)
#<!-- -->44 0x00007e7e7a429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->45 0x00007e7e7a429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->46 0x0000000000ce4dd5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xce4dd5)
```

&lt;/details&gt;
</details>


---

### Comment 4 - zyn0217

We shouldn't see an `UnresolvedLookupExpr` in CG so this is a frontend issue.

---

### Comment 5 - cor3ntin

@mizvekov 

---

