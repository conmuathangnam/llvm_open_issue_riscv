# [clang] Assertion `CGF.getContext().hasSameUnqualifiedType(E->getSubExpr()->getType(), E->getType()) && "Implicit cast types must be compatible"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/164062

## Body

Reproducer:
https://godbolt.org/z/jzTf8aMsM
```c
struct h {
  int b;
  int a[] __attribute__((counted_by(b)));
} p;

void foo(void) {
  struct h {
    int b;
    int a[] __attribute__((counted_by(b)));
  } x;

  p = x;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:986: void {anonymous}::AggExprEmitter::VisitCastExpr(clang::CastExpr*): Assertion `CGF.getContext().hasSameUnqualifiedType(E->getSubExpr()->getType(), E->getType()) && "Implicit cast types must be compatible"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c23 <source>
1.	<eof> parser at end of file
2.	<source>:6:6: LLVM IR generation of declaration 'foo'
3.	<source>:6:6: Generating code for declaration 'foo'
 #0 0x00000000042021d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42021d8)
 #1 0x00000000041ff604 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ff604)
 #2 0x0000000004143778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000070f077a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000070f077a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000070f077a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000070f077a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000070f077a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000070f077a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000458d85c (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#10 0x00000000045894f1 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#11 0x000000000458a1ee clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458a1ee)
#12 0x000000000458aace (anonymous namespace)::AggExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprAgg.cpp:0:0
#13 0x0000000004589bab (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#14 0x000000000458a1ee clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458a1ee)
#15 0x0000000004542601 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4542601)
#16 0x0000000004573a6d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4573a6d)
#17 0x00000000046f4257 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46f4257)
#18 0x00000000046fc53c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46fc53c)
#19 0x0000000004764a8e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4764a8e)
#20 0x0000000004776ae4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4776ae4)
#21 0x00000000047e3950 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47e3950)
#22 0x00000000047de824 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47de824)
#23 0x00000000047df6c0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47df6c0)
#24 0x00000000047e9ea3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#25 0x0000000004b4f1e9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#26 0x0000000004b3e314 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3e314)
#27 0x00000000068e74e4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e74e4)
#28 0x0000000004b4abe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4abe8)
#29 0x0000000004e38265 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e38265)
#30 0x0000000004db92fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db92fe)
#31 0x0000000004f2fcfd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2fcfd)
#32 0x0000000000dc2b50 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc2b50)
#33 0x0000000000db96ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x0000000000db983d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#35 0x0000000004bb3d99 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x0000000004143c14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4143c14)
#37 0x0000000004bb43af clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x0000000004b756c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b756c2)
#39 0x0000000004b7666e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7666e)
#40 0x0000000004b7dda5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7dda5)
#41 0x0000000000dbf079 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbf079)
#42 0x0000000000c6ee04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6ee04)
#43 0x000070f077a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x000070f077a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000db9155 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9155)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/jzTf8aMsM
```c
struct h {
  int b;
  int a[] __attribute__((counted_by(b)));
} p;

void foo(void) {
  struct h {
    int b;
    int a[] __attribute__((counted_by(b)));
  } x;

  p = x;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:986: void {anonymous}::AggExprEmitter::VisitCastExpr(clang::CastExpr*): Assertion `CGF.getContext().hasSameUnqualifiedType(E-&gt;getSubExpr()-&gt;getType(), E-&gt;getType()) &amp;&amp; "Implicit cast types must be compatible"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c23 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:6:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:6:6: Generating code for declaration 'foo'
 #<!-- -->0 0x00000000042021d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42021d8)
 #<!-- -->1 0x00000000041ff604 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ff604)
 #<!-- -->2 0x0000000004143778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070f077a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070f077a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070f077a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070f077a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070f077a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070f077a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000458d85c (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#<!-- -->10 0x00000000045894f1 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->11 0x000000000458a1ee clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458a1ee)
#<!-- -->12 0x000000000458aace (anonymous namespace)::AggExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprAgg.cpp:0:0
#<!-- -->13 0x0000000004589bab (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->14 0x000000000458a1ee clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458a1ee)
#<!-- -->15 0x0000000004542601 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4542601)
#<!-- -->16 0x0000000004573a6d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4573a6d)
#<!-- -->17 0x00000000046f4257 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46f4257)
#<!-- -->18 0x00000000046fc53c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46fc53c)
#<!-- -->19 0x0000000004764a8e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4764a8e)
#<!-- -->20 0x0000000004776ae4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4776ae4)
#<!-- -->21 0x00000000047e3950 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47e3950)
#<!-- -->22 0x00000000047de824 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47de824)
#<!-- -->23 0x00000000047df6c0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47df6c0)
#<!-- -->24 0x00000000047e9ea3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->25 0x0000000004b4f1e9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->26 0x0000000004b3e314 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3e314)
#<!-- -->27 0x00000000068e74e4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e74e4)
#<!-- -->28 0x0000000004b4abe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4abe8)
#<!-- -->29 0x0000000004e38265 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e38265)
#<!-- -->30 0x0000000004db92fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db92fe)
#<!-- -->31 0x0000000004f2fcfd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2fcfd)
#<!-- -->32 0x0000000000dc2b50 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc2b50)
#<!-- -->33 0x0000000000db96ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->34 0x0000000000db983d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004bb3d99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000004143c14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4143c14)
#<!-- -->37 0x0000000004bb43af clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x0000000004b756c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b756c2)
#<!-- -->39 0x0000000004b7666e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7666e)
#<!-- -->40 0x0000000004b7dda5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7dda5)
#<!-- -->41 0x0000000000dbf079 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbf079)
#<!-- -->42 0x0000000000c6ee04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6ee04)
#<!-- -->43 0x000070f077a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x000070f077a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000db9155 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9155)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/MnrGqrYbr

---

### Comment 3 - shafik

Simplified: https://godbolt.org/z/7T73bfaYe

```c
struct h {
  int b;
};

void foo(struct h p) {
  struct h {
    int b;
  } x;

  p = x;
}
```

---

### Comment 4 - shafik

CC @AaronBallman 

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/jzTf8aMsM
```c
struct h {
  int b;
  int a[] __attribute__((counted_by(b)));
} p;

void foo(void) {
  struct h {
    int b;
    int a[] __attribute__((counted_by(b)));
  } x;

  p = x;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:986: void {anonymous}::AggExprEmitter::VisitCastExpr(clang::CastExpr*): Assertion `CGF.getContext().hasSameUnqualifiedType(E-&gt;getSubExpr()-&gt;getType(), E-&gt;getType()) &amp;&amp; "Implicit cast types must be compatible"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c -std=c23 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:6:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:6:6: Generating code for declaration 'foo'
 #<!-- -->0 0x00000000042021d8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x42021d8)
 #<!-- -->1 0x00000000041ff604 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41ff604)
 #<!-- -->2 0x0000000004143778 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000070f077a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000070f077a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000070f077a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000070f077a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000070f077a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000070f077a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000458d85c (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#<!-- -->10 0x00000000045894f1 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->11 0x000000000458a1ee clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458a1ee)
#<!-- -->12 0x000000000458aace (anonymous namespace)::AggExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprAgg.cpp:0:0
#<!-- -->13 0x0000000004589bab (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->14 0x000000000458a1ee clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x458a1ee)
#<!-- -->15 0x0000000004542601 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4542601)
#<!-- -->16 0x0000000004573a6d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4573a6d)
#<!-- -->17 0x00000000046f4257 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46f4257)
#<!-- -->18 0x00000000046fc53c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46fc53c)
#<!-- -->19 0x0000000004764a8e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4764a8e)
#<!-- -->20 0x0000000004776ae4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4776ae4)
#<!-- -->21 0x00000000047e3950 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47e3950)
#<!-- -->22 0x00000000047de824 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47de824)
#<!-- -->23 0x00000000047df6c0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47df6c0)
#<!-- -->24 0x00000000047e9ea3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->25 0x0000000004b4f1e9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->26 0x0000000004b3e314 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b3e314)
#<!-- -->27 0x00000000068e74e4 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68e74e4)
#<!-- -->28 0x0000000004b4abe8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b4abe8)
#<!-- -->29 0x0000000004e38265 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e38265)
#<!-- -->30 0x0000000004db92fe clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4db92fe)
#<!-- -->31 0x0000000004f2fcfd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f2fcfd)
#<!-- -->32 0x0000000000dc2b50 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdc2b50)
#<!-- -->33 0x0000000000db96ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->34 0x0000000000db983d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004bb3d99 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x0000000004143c14 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4143c14)
#<!-- -->37 0x0000000004bb43af clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x0000000004b756c2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b756c2)
#<!-- -->39 0x0000000004b7666e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7666e)
#<!-- -->40 0x0000000004b7dda5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b7dda5)
#<!-- -->41 0x0000000000dbf079 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbf079)
#<!-- -->42 0x0000000000c6ee04 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6ee04)
#<!-- -->43 0x000070f077a29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x000070f077a29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000db9155 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9155)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

