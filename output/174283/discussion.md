# [clang] Redefining a struct in a C function definition (& using both)

**Author:** Dante-Broggi
**URL:** https://github.com/llvm/llvm-project/issues/174283
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash
**Closed Date:** 2026-01-03T21:32:35Z

## Body

Reproducer:
https://godbolt.org/z/jjbeY7Ked

```c
struct Test { int x; };
int func(struct Test y, struct Test { int x; } *z) {
    *z = y;
    return y.x;
}
```
The important part seems to be assigning an instance using one of the definitions into an object using the other definition.

Backtrace:
```
clang: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:985: void {anonymous}::AggExprEmitter::VisitCastExpr(clang::CastExpr*): Assertion `CGF.getContext().hasSameUnqualifiedType(E->getSubExpr()->getType(), E->getType()) && "Implicit cast types must be compatible"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -emit-llvm -Xclang -disable-llvm-passes --std=c23 <source>
1.	<eof> parser at end of file
2.	<source>:4:5: LLVM IR generation of declaration 'func'
3.	<source>:4:5: Generating code for declaration 'func'
 #0 0x0000000004308648 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4308648)
 #1 0x0000000004305a74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4305a74)
 #2 0x000000000424a518 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000075d042042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000075d0420969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000075d042042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000075d0420287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000075d04202871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000075d042039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x000000000469570c (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#10 0x00000000046970c1 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#11 0x0000000004697dbe clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4697dbe)
#12 0x00000000046986ce (anonymous namespace)::AggExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprAgg.cpp:0:0
#13 0x000000000469777b (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#14 0x0000000004697dbe clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4697dbe)
#15 0x000000000464dda1 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x464dda1)
#16 0x000000000466555d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x466555d)
#17 0x0000000004812ad7 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4812ad7)
#18 0x000000000481b05b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481b05b)
#19 0x000000000488309e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x488309e)
#20 0x00000000048944f4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48944f4)
#21 0x00000000049022d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49022d0)
#22 0x00000000048fcf74 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48fcf74)
#23 0x00000000048fd753 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48fd753)
#24 0x0000000004908ce3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#25 0x0000000004c6c4c9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#26 0x0000000004c5cff4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c5cff4)
#27 0x0000000006a2ef94 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a2ef94)
#28 0x0000000004c698d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c698d8)
#29 0x0000000004f600a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f600a5)
#30 0x0000000004ee025e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ee025e)
#31 0x000000000505a42d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x505a42d)
#32 0x0000000000de722c cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde722c)
#33 0x0000000000dddc7a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#34 0x0000000000ddddfd int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#35 0x0000000004cdbfa9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#36 0x000000000424a9b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x424a9b4)
#37 0x0000000004cdc5bf clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#38 0x0000000004c9d0a2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9d0a2)
#39 0x0000000004c9e04e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9e04e)
#40 0x0000000004ca54a5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca54a5)
#41 0x0000000000de3621 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde3621)
#42 0x0000000000c8df54 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc8df54)
#43 0x000075d042029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#44 0x000075d042029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#45 0x0000000000ddd715 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xddd715)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Dante Broggi (Dante-Broggi)

<details>
Reproducer:
https://godbolt.org/z/jjbeY7Ked

```c
struct Test { int x; };
int func(struct Test y, struct Test { int x; } *z) {
    *z = y;
    return y.x;
}
```
The important part seems to be assigning an instance using one of the definitions into an object using the other definition.

Backtrace:
```
clang: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExprAgg.cpp:985: void {anonymous}::AggExprEmitter::VisitCastExpr(clang::CastExpr*): Assertion `CGF.getContext().hasSameUnqualifiedType(E-&gt;getSubExpr()-&gt;getType(), E-&gt;getType()) &amp;&amp; "Implicit cast types must be compatible"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -emit-llvm -Xclang -disable-llvm-passes --std=c23 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:4:5: LLVM IR generation of declaration 'func'
3.	&lt;source&gt;:4:5: Generating code for declaration 'func'
 #<!-- -->0 0x0000000004308648 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4308648)
 #<!-- -->1 0x0000000004305a74 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4305a74)
 #<!-- -->2 0x000000000424a518 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000075d042042520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000075d0420969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000075d042042476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000075d0420287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000075d04202871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000075d042039e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x000000000469570c (anonymous namespace)::AggExprEmitter::VisitCastExpr(clang::CastExpr*) CGExprAgg.cpp:0:0
#<!-- -->10 0x00000000046970c1 (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->11 0x0000000004697dbe clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4697dbe)
#<!-- -->12 0x00000000046986ce (anonymous namespace)::AggExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprAgg.cpp:0:0
#<!-- -->13 0x000000000469777b (anonymous namespace)::AggExprEmitter::Visit(clang::Expr*) CGExprAgg.cpp:0:0
#<!-- -->14 0x0000000004697dbe clang::CodeGen::CodeGenFunction::EmitAggExpr(clang::Expr const*, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4697dbe)
#<!-- -->15 0x000000000464dda1 clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x464dda1)
#<!-- -->16 0x000000000466555d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x466555d)
#<!-- -->17 0x0000000004812ad7 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4812ad7)
#<!-- -->18 0x000000000481b05b clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x481b05b)
#<!-- -->19 0x000000000488309e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x488309e)
#<!-- -->20 0x00000000048944f4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48944f4)
#<!-- -->21 0x00000000049022d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x49022d0)
#<!-- -->22 0x00000000048fcf74 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48fcf74)
#<!-- -->23 0x00000000048fd753 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x48fd753)
#<!-- -->24 0x0000000004908ce3 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->25 0x0000000004c6c4c9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->26 0x0000000004c5cff4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c5cff4)
#<!-- -->27 0x0000000006a2ef94 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x6a2ef94)
#<!-- -->28 0x0000000004c698d8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c698d8)
#<!-- -->29 0x0000000004f600a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4f600a5)
#<!-- -->30 0x0000000004ee025e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ee025e)
#<!-- -->31 0x000000000505a42d clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x505a42d)
#<!-- -->32 0x0000000000de722c cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde722c)
#<!-- -->33 0x0000000000dddc7a ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->34 0x0000000000ddddfd int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->35 0x0000000004cdbfa9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->36 0x000000000424a9b4 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x424a9b4)
#<!-- -->37 0x0000000004cdc5bf clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->38 0x0000000004c9d0a2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9d0a2)
#<!-- -->39 0x0000000004c9e04e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4c9e04e)
#<!-- -->40 0x0000000004ca54a5 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0x4ca54a5)
#<!-- -->41 0x0000000000de3621 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xde3621)
#<!-- -->42 0x0000000000c8df54 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xc8df54)
#<!-- -->43 0x000075d042029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->44 0x000075d042029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->45 0x0000000000ddd715 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+0xddd715)
clang: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

