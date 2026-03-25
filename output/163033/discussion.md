# [clang] -fsanitize=undefined causes Assertion `(!isa<llvm::Constant>(EvaluatedGEP.TotalOffset) || EvaluatedGEP.OffsetOverflows == Builder.getFalse()) && "If the offset got constant-folded, we don't expect that there was an " "overflow."' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/163033
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Reproducer:
https://godbolt.org/z/j9dqbEfoT
```cpp
void *operator new(__SIZE_TYPE__ n, void *p) { return p; }

typedef int int32_t __attribute__((mode(__SI__)));

struct A {};

int32_t arr[2];

struct S {
  int32_t i, arr[0];
};

void foo(S *p) { new (&p->arr[(-__PTRDIFF_MAX__ - 1)]) A; }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExprScalar.cpp:6147: llvm::Value* clang::CodeGen::CodeGenFunction::EmitCheckedInBoundsGEP(llvm::Type*, llvm::Value*, llvm::ArrayRef<llvm::Value*>, bool, bool, clang::SourceLocation, const llvm::Twine&): Assertion `(!isa<llvm::Constant>(EvaluatedGEP.TotalOffset) || EvaluatedGEP.OffsetOverflows == Builder.getFalse()) && "If the offset got constant-folded, we don't expect that there was an " "overflow."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=undefined <source>
1.	<eof> parser at end of file
2.	<source>:13:6: LLVM IR generation of declaration 'foo'
3.	<source>:13:6: Generating code for declaration 'foo'
 #0 0x00000000041e9638 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e9638)
 #1 0x00000000041e6a64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e6a64)
 #2 0x000000000412b3b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000762208442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007622084969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x0000762208442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007622084287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000076220842871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x0000762208439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000045b9d4f clang::CodeGen::CodeGenFunction::EmitCheckedInBoundsGEP(llvm::Type*, llvm::Value*, llvm::ArrayRef<llvm::Value*>, bool, bool, clang::SourceLocation, llvm::Twine const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45b9d4f)
#10 0x00000000045b9eb0 clang::CodeGen::CodeGenFunction::EmitCheckedInBoundsGEP(clang::CodeGen::Address, llvm::ArrayRef<llvm::Value*>, llvm::Type*, bool, bool, clang::SourceLocation, clang::CharUnits, llvm::Twine const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45b9eb0)
#11 0x00000000045402e0 emitArraySubscriptGEP(clang::CodeGen::CodeGenFunction&, clang::CodeGen::Address, llvm::ArrayRef<llvm::Value*>, clang::QualType, bool, bool, clang::SourceLocation, clang::QualType*, clang::Expr const*, llvm::Twine const&) CGExpr.cpp:0:0
#12 0x000000000454708a clang::CodeGen::CodeGenFunction::EmitArraySubscriptExpr(clang::ArraySubscriptExpr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x454708a)
#13 0x000000000455f2db clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455f2db)
#14 0x000000000455fd97 void llvm::function_ref<void ()>::callback_fn<clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()>(long) CGExpr.cpp:0:0
#15 0x00000000081aeb91 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81aeb91)
#16 0x0000000004527e03 clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4527e03)
#17 0x0000000004541432 EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t, clang::CodeGen::CodeGenFunction&) CGExpr.cpp:0:0
#18 0x0000000004541fd4 clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4541fd4)
#19 0x00000000045417a0 EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t, clang::CodeGen::CodeGenFunction&) CGExpr.cpp:0:0
#20 0x0000000004541fd4 clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4541fd4)
#21 0x00000000045837df clang::CodeGen::CodeGenFunction::EmitCXXNewExpr(clang::CXXNewExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45837df)
#22 0x00000000045bf3d5 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#23 0x00000000045c36cc clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45c36cc)
#24 0x0000000004526a8e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4526a8e)
#25 0x000000000455807d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455807d)
#26 0x00000000046d7f87 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46d7f87)
#27 0x00000000046e021c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46e021c)
#28 0x000000000474879e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x474879e)
#29 0x000000000475a7c4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x475a7c4)
#30 0x00000000047c73d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c73d0)
#31 0x00000000047c22b4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c22b4)
#32 0x00000000047c3150 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c3150)
#33 0x00000000047cd923 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#34 0x0000000004b32ca9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#35 0x0000000004b21dd4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b21dd4)
#36 0x00000000068c6084 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c6084)
#37 0x0000000004b2e6a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2e6a8)
#38 0x0000000004e1a3a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e1a3a5)
#39 0x0000000004d9bade clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d9bade)
#40 0x0000000004f116bd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f116bd)
#41 0x0000000000dbfa50 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfa50)
#42 0x0000000000db65ba ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#43 0x0000000000db673d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#44 0x0000000004b97839 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#45 0x000000000412b854 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412b854)
#46 0x0000000004b97e4f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#47 0x0000000004b59162 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b59162)
#48 0x0000000004b5a10e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5a10e)
#49 0x0000000004b61845 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b61845)
#50 0x0000000000dbbf79 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbbf79)
#51 0x0000000000c6bf74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6bf74)
#52 0x0000762208429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#53 0x0000762208429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#54 0x0000000000db6055 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb6055)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/j9dqbEfoT
```cpp
void *operator new(__SIZE_TYPE__ n, void *p) { return p; }

typedef int int32_t __attribute__((mode(__SI__)));

struct A {};

int32_t arr[2];

struct S {
  int32_t i, arr[0];
};

void foo(S *p) { new (&amp;p-&gt;arr[(-__PTRDIFF_MAX__ - 1)]) A; }
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExprScalar.cpp:6147: llvm::Value* clang::CodeGen::CodeGenFunction::EmitCheckedInBoundsGEP(llvm::Type*, llvm::Value*, llvm::ArrayRef&lt;llvm::Value*&gt;, bool, bool, clang::SourceLocation, const llvm::Twine&amp;): Assertion `(!isa&lt;llvm::Constant&gt;(EvaluatedGEP.TotalOffset) || EvaluatedGEP.OffsetOverflows == Builder.getFalse()) &amp;&amp; "If the offset got constant-folded, we don't expect that there was an " "overflow."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -fsanitize=undefined &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:13:6: LLVM IR generation of declaration 'foo'
3.	&lt;source&gt;:13:6: Generating code for declaration 'foo'
 #<!-- -->0 0x00000000041e9638 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e9638)
 #<!-- -->1 0x00000000041e6a64 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41e6a64)
 #<!-- -->2 0x000000000412b3b8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x0000762208442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007622084969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x0000762208442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007622084287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000076220842871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x0000762208439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000045b9d4f clang::CodeGen::CodeGenFunction::EmitCheckedInBoundsGEP(llvm::Type*, llvm::Value*, llvm::ArrayRef&lt;llvm::Value*&gt;, bool, bool, clang::SourceLocation, llvm::Twine const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45b9d4f)
#<!-- -->10 0x00000000045b9eb0 clang::CodeGen::CodeGenFunction::EmitCheckedInBoundsGEP(clang::CodeGen::Address, llvm::ArrayRef&lt;llvm::Value*&gt;, llvm::Type*, bool, bool, clang::SourceLocation, clang::CharUnits, llvm::Twine const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45b9eb0)
#<!-- -->11 0x00000000045402e0 emitArraySubscriptGEP(clang::CodeGen::CodeGenFunction&amp;, clang::CodeGen::Address, llvm::ArrayRef&lt;llvm::Value*&gt;, clang::QualType, bool, bool, clang::SourceLocation, clang::QualType*, clang::Expr const*, llvm::Twine const&amp;) CGExpr.cpp:0:0
#<!-- -->12 0x000000000454708a clang::CodeGen::CodeGenFunction::EmitArraySubscriptExpr(clang::ArraySubscriptExpr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x454708a)
#<!-- -->13 0x000000000455f2db clang::CodeGen::CodeGenFunction::EmitLValueHelper(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455f2db)
#<!-- -->14 0x000000000455fd97 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t)::'lambda'()&gt;(long) CGExpr.cpp:0:0
#<!-- -->15 0x00000000081aeb91 clang::StackExhaustionHandler::runWithSufficientStackSpace(clang::SourceLocation, llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x81aeb91)
#<!-- -->16 0x0000000004527e03 clang::CodeGen::CodeGenFunction::EmitLValue(clang::Expr const*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4527e03)
#<!-- -->17 0x0000000004541432 EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t, clang::CodeGen::CodeGenFunction&amp;) CGExpr.cpp:0:0
#<!-- -->18 0x0000000004541fd4 clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4541fd4)
#<!-- -->19 0x00000000045417a0 EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t, clang::CodeGen::CodeGenFunction&amp;) CGExpr.cpp:0:0
#<!-- -->20 0x0000000004541fd4 clang::CodeGen::CodeGenFunction::EmitPointerWithAlignment(clang::Expr const*, clang::CodeGen::LValueBaseInfo*, clang::CodeGen::TBAAAccessInfo*, clang::CodeGen::KnownNonNull_t) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4541fd4)
#<!-- -->21 0x00000000045837df clang::CodeGen::CodeGenFunction::EmitCXXNewExpr(clang::CXXNewExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45837df)
#<!-- -->22 0x00000000045bf3d5 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->23 0x00000000045c36cc clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x45c36cc)
#<!-- -->24 0x0000000004526a8e clang::CodeGen::CodeGenFunction::EmitAnyExpr(clang::Expr const*, clang::CodeGen::AggValueSlot, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4526a8e)
#<!-- -->25 0x000000000455807d clang::CodeGen::CodeGenFunction::EmitIgnoredExpr(clang::Expr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455807d)
#<!-- -->26 0x00000000046d7f87 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46d7f87)
#<!-- -->27 0x00000000046e021c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46e021c)
#<!-- -->28 0x000000000474879e clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x474879e)
#<!-- -->29 0x000000000475a7c4 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x475a7c4)
#<!-- -->30 0x00000000047c73d0 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c73d0)
#<!-- -->31 0x00000000047c22b4 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c22b4)
#<!-- -->32 0x00000000047c3150 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47c3150)
#<!-- -->33 0x00000000047cd923 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->34 0x0000000004b32ca9 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->35 0x0000000004b21dd4 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b21dd4)
#<!-- -->36 0x00000000068c6084 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x68c6084)
#<!-- -->37 0x0000000004b2e6a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b2e6a8)
#<!-- -->38 0x0000000004e1a3a5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4e1a3a5)
#<!-- -->39 0x0000000004d9bade clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d9bade)
#<!-- -->40 0x0000000004f116bd clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4f116bd)
#<!-- -->41 0x0000000000dbfa50 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbfa50)
#<!-- -->42 0x0000000000db65ba ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->43 0x0000000000db673d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->44 0x0000000004b97839 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->45 0x000000000412b854 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x412b854)
#<!-- -->46 0x0000000004b97e4f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->47 0x0000000004b59162 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b59162)
#<!-- -->48 0x0000000004b5a10e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b5a10e)
#<!-- -->49 0x0000000004b61845 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b61845)
#<!-- -->50 0x0000000000dbbf79 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdbbf79)
#<!-- -->51 0x0000000000c6bf74 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc6bf74)
#<!-- -->52 0x0000762208429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->53 0x0000762208429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->54 0x0000000000db6055 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb6055)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-10.0.0:
https://godbolt.org/z/qc1j6nv7E

---

