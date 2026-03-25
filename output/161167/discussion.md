# [clang] Assertion `getContext().hasSameUnqualifiedType( AlignValT->castAsEnumDecl()->getIntegerType(), sizeType) && "wrong type for alignment parameter"' failed.

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/161167
**Status:** Open
**Labels:** clang:codegen, crash

## Body

Reproducer:
https://godbolt.org/z/rYT99TWsv
```cpp
namespace std {
template <class T> struct type_identity {};
#define TI(T) std::type_identity<T>
using size_t = __SIZE_TYPE__;
enum class align_val_t : long {};
} // namespace std

template <class T> void *operator new(TI(T), std::size_t, std::align_val_t);
template <class T>
void operator delete(TI(T), void *, std::size_t, std::align_val_t);

using size_t = __SIZE_TYPE__;
struct TestType {};

void f() {
  TestType *t = new TestType;
  delete t;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExprCXX.cpp:1690: llvm::Value* clang::CodeGen::CodeGenFunction::EmitCXXNewExpr(const clang::CXXNewExpr*): Assertion `getContext().hasSameUnqualifiedType( AlignValT->castAsEnumDecl()->getIntegerType(), sizeType) && "wrong type for alignment parameter"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:15:6: LLVM IR generation of declaration 'f'
3.	<source>:15:6: Generating code for declaration 'f'
 #0 0x00000000041b1728 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b1728)
 #1 0x00000000041aeb54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41aeb54)
 #2 0x00000000040f2e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000078aa2c442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000078aa2c4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000078aa2c442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000078aa2c4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000078aa2c42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000078aa2c439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x0000000004546c97 clang::CodeGen::CodeGenFunction::EmitCXXNewExpr(clang::CXXNewExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4546c97)
#10 0x00000000045807f5 clang::StmtVisitorBase<std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*>::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#11 0x0000000004584a7c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4584a7c)
#12 0x0000000004a3ad71 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a3ad71)
#13 0x0000000004a3cbf4 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a3cbf4)
#14 0x0000000004a463c0 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a463c0)
#15 0x0000000004a4bb29 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#16 0x0000000004a4c1b7 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a4c1b7)
#17 0x000000000468b2dc clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x468b2dc)
#18 0x00000000046a171b clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46a171b)
#19 0x0000000004698dd1 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4698dd1)
#20 0x00000000046a10ac clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46a10ac)
#21 0x00000000047094de clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47094de)
#22 0x000000000471b484 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x471b484)
#23 0x0000000004787220 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4787220)
#24 0x0000000004781f24 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4781f24)
#25 0x0000000004782dc0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4782dc0)
#26 0x000000000478df13 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#27 0x0000000004af27b0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#28 0x0000000004ae1804 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae1804)
#29 0x000000000687a414 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687a414)
#30 0x0000000004aee1b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aee1b8)
#31 0x0000000004dd9fd5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd9fd5)
#32 0x0000000004d55e6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d55e6e)
#33 0x0000000004ed0bad clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed0bad)
#34 0x0000000000db9190 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9190)
#35 0x0000000000dafcfa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#36 0x0000000000dafe7d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#37 0x0000000004b572f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#38 0x00000000040f3324 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f3324)
#39 0x0000000004b5790f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#40 0x0000000004b18c72 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b18c72)
#41 0x0000000004b19c1e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b19c1e)
#42 0x0000000004b21355 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b21355)
#43 0x0000000000db56b9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb56b9)
#44 0x0000000000c67004 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc67004)
#45 0x000078aa2c429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#46 0x000078aa2c429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#47 0x0000000000daf795 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf795)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (k-arrows)

<details>
Reproducer:
https://godbolt.org/z/rYT99TWsv
```cpp
namespace std {
template &lt;class T&gt; struct type_identity {};
#define TI(T) std::type_identity&lt;T&gt;
using size_t = __SIZE_TYPE__;
enum class align_val_t : long {};
} // namespace std

template &lt;class T&gt; void *operator new(TI(T), std::size_t, std::align_val_t);
template &lt;class T&gt;
void operator delete(TI(T), void *, std::size_t, std::align_val_t);

using size_t = __SIZE_TYPE__;
struct TestType {};

void f() {
  TestType *t = new TestType;
  delete t;
}
```

Backtrace:
```console
clang++: /root/llvm-project/llvm/tools/clang/lib/CodeGen/CGExprCXX.cpp:1690: llvm::Value* clang::CodeGen::CodeGenFunction::EmitCXXNewExpr(const clang::CXXNewExpr*): Assertion `getContext().hasSameUnqualifiedType( AlignValT-&gt;castAsEnumDecl()-&gt;getIntegerType(), sizeType) &amp;&amp; "wrong type for alignment parameter"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	&lt;source&gt;:15:6: LLVM IR generation of declaration 'f'
3.	&lt;source&gt;:15:6: Generating code for declaration 'f'
 #<!-- -->0 0x00000000041b1728 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41b1728)
 #<!-- -->1 0x00000000041aeb54 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x41aeb54)
 #<!-- -->2 0x00000000040f2e88 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000078aa2c442520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000078aa2c4969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000078aa2c442476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000078aa2c4287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000078aa2c42871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000078aa2c439e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x0000000004546c97 clang::CodeGen::CodeGenFunction::EmitCXXNewExpr(clang::CXXNewExpr const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4546c97)
#<!-- -->10 0x00000000045807f5 clang::StmtVisitorBase&lt;std::add_pointer, (anonymous namespace)::ScalarExprEmitter, llvm::Value*&gt;::Visit(clang::Stmt*) CGExprScalar.cpp:0:0
#<!-- -->11 0x0000000004584a7c clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4584a7c)
#<!-- -->12 0x0000000004a3ad71 clang::CodeGen::CodeGenFunction::EmitScalarInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a3ad71)
#<!-- -->13 0x0000000004a3cbf4 clang::CodeGen::CodeGenFunction::EmitExprAsInit(clang::Expr const*, clang::ValueDecl const*, clang::CodeGen::LValue, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a3cbf4)
#<!-- -->14 0x0000000004a463c0 clang::CodeGen::CodeGenFunction::EmitAutoVarInit(clang::CodeGen::CodeGenFunction::AutoVarEmission const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a463c0)
#<!-- -->15 0x0000000004a4bb29 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (.part.0) CGDecl.cpp:0:0
#<!-- -->16 0x0000000004a4c1b7 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4a4c1b7)
#<!-- -->17 0x000000000468b2dc clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x468b2dc)
#<!-- -->18 0x00000000046a171b clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46a171b)
#<!-- -->19 0x0000000004698dd1 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4698dd1)
#<!-- -->20 0x00000000046a10ac clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x46a10ac)
#<!-- -->21 0x00000000047094de clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47094de)
#<!-- -->22 0x000000000471b484 clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x471b484)
#<!-- -->23 0x0000000004787220 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4787220)
#<!-- -->24 0x0000000004781f24 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4781f24)
#<!-- -->25 0x0000000004782dc0 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4782dc0)
#<!-- -->26 0x000000000478df13 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#<!-- -->27 0x0000000004af27b0 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#<!-- -->28 0x0000000004ae1804 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ae1804)
#<!-- -->29 0x000000000687a414 clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x687a414)
#<!-- -->30 0x0000000004aee1b8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4aee1b8)
#<!-- -->31 0x0000000004dd9fd5 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4dd9fd5)
#<!-- -->32 0x0000000004d55e6e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d55e6e)
#<!-- -->33 0x0000000004ed0bad clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ed0bad)
#<!-- -->34 0x0000000000db9190 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb9190)
#<!-- -->35 0x0000000000dafcfa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;, llvm::IntrusiveRefCntPtr&lt;llvm::vfs::FileSystem&gt;) driver.cpp:0:0
#<!-- -->36 0x0000000000dafe7d int llvm::function_ref&lt;int (llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;::callback_fn&lt;clang_main(int, char**, llvm::ToolContext const&amp;)::'lambda'(llvm::SmallVectorImpl&lt;char const*&gt;&amp;)&gt;(long, llvm::SmallVectorImpl&lt;char const*&gt;&amp;) driver.cpp:0:0
#<!-- -->37 0x0000000004b572f9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->38 0x00000000040f3324 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40f3324)
#<!-- -->39 0x0000000004b5790f clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->40 0x0000000004b18c72 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b18c72)
#<!-- -->41 0x0000000004b19c1e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b19c1e)
#<!-- -->42 0x0000000004b21355 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4b21355)
#<!-- -->43 0x0000000000db56b9 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdb56b9)
#<!-- -->44 0x0000000000c67004 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc67004)
#<!-- -->45 0x000078aa2c429d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->46 0x000078aa2c429e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->47 0x0000000000daf795 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaf795)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```
</details>


---

### Comment 2 - k-arrows

Goes back to clang-21.1.0:
https://godbolt.org/z/v9sfjnPa3

---

