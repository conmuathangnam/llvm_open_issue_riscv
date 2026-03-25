# Clang ICE / LLVM oom after allocd[32] = '\2'; in C and C++ related to CodeGen

**Author:** mariete1223
**URL:** https://github.com/llvm/llvm-project/issues/140023

## Body

In C++ it happens since clang 15 until trunk and in C since 3.0.0

## Assertion

No assertion given

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 <source>
1.	<eof> parser at end of file
2.	Per-file LLVM IR generation
3.	<source>:2:13: Generating code for declaration 'allocd'
 #0 0x0000000003f98e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98e38)
 #1 0x0000000003f96ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f96ac4)
 #2 0x0000000003edb6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007b2b24642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00007b2b246969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00007b2b24642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00007b2b246287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x0000000003ee6efb (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ee6efb)
 #8 0x0000000003f15ead llvm::SmallVectorBase<unsigned long>::grow_pod(void*, unsigned long, unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f15ead)
 #9 0x000000000450bf3b clang::CodeGen::CodeGenModule::GetConstantArrayFromStringLiteral(clang::StringLiteral const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x450bf3b)
#10 0x000000000434e03c clang::StmtVisitorBase<llvm::make_const_ptr, (anonymous namespace)::ConstExprEmitter, llvm::Constant*, clang::QualType>::Visit(clang::Stmt const*, clang::QualType) CGExprConstant.cpp:0:0
#11 0x0000000004350313 clang::CodeGen::ConstantEmitter::tryEmitPrivateForVarInit(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4350313)
#12 0x0000000004350591 clang::CodeGen::ConstantEmitter::tryEmitForInitializer(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4350591)
#13 0x000000000453a4ce clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x453a4ce)
#14 0x000000000455ef31 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455ef31)
#15 0x000000000456b797 clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456b797)
#16 0x000000000456dd23 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456dd23)
#17 0x000000000494640e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&) ModuleBuilder.cpp:0:0
#18 0x0000000004942825 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4942825)
#19 0x000000000662637c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662637c)
#20 0x0000000004942fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4942fd8)
#21 0x0000000004c39725 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c39725)
#22 0x0000000004bb834e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb834e)
#23 0x0000000004d2d289 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2d289)
#24 0x0000000000daba2f cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaba2f)
#25 0x0000000000da1bfa ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#26 0x00000000049aa2c9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#27 0x0000000003edbb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edbb94)
#28 0x00000000049aa8df clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#29 0x000000000496c6ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496c6ad)
#30 0x000000000496d73e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496d73e)
#31 0x0000000004975a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4975a25)
#32 0x0000000000da7998 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7998)
#33 0x0000000000c2d064 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2d064)
#34 0x00007b2b24629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#35 0x00007b2b24629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#36 0x0000000000da16a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda16a5)
```


## Program

```
#include <stdlib.h>
char static allocd[((size_t)-1) / 256] = "\1";
int main(void)
{
    free(malloc(0));
    allocd[32] = '\2';
}
```

To quickly reproduce:

https://godbolt.org/z/KbjTcdzKM

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: None (mariete1223)

<details>
In C++ it happens since clang 15 until trunk and in C since 3.0.0

## Assertion

No assertion given

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Per-file LLVM IR generation
3.	&lt;source&gt;:2:13: Generating code for declaration 'allocd'
 #<!-- -->0 0x0000000003f98e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98e38)
 #<!-- -->1 0x0000000003f96ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f96ac4)
 #<!-- -->2 0x0000000003edb6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b2b24642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b2b246969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b2b24642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b2b246287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003ee6efb (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ee6efb)
 #<!-- -->8 0x0000000003f15ead llvm::SmallVectorBase&lt;unsigned long&gt;::grow_pod(void*, unsigned long, unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f15ead)
 #<!-- -->9 0x000000000450bf3b clang::CodeGen::CodeGenModule::GetConstantArrayFromStringLiteral(clang::StringLiteral const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x450bf3b)
#<!-- -->10 0x000000000434e03c clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::ConstExprEmitter, llvm::Constant*, clang::QualType&gt;::Visit(clang::Stmt const*, clang::QualType) CGExprConstant.cpp:0:0
#<!-- -->11 0x0000000004350313 clang::CodeGen::ConstantEmitter::tryEmitPrivateForVarInit(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4350313)
#<!-- -->12 0x0000000004350591 clang::CodeGen::ConstantEmitter::tryEmitForInitializer(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4350591)
#<!-- -->13 0x000000000453a4ce clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x453a4ce)
#<!-- -->14 0x000000000455ef31 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455ef31)
#<!-- -->15 0x000000000456b797 clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456b797)
#<!-- -->16 0x000000000456dd23 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456dd23)
#<!-- -->17 0x000000000494640e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&amp;) ModuleBuilder.cpp:0:0
#<!-- -->18 0x0000000004942825 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4942825)
#<!-- -->19 0x000000000662637c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662637c)
#<!-- -->20 0x0000000004942fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4942fd8)
#<!-- -->21 0x0000000004c39725 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c39725)
#<!-- -->22 0x0000000004bb834e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb834e)
#<!-- -->23 0x0000000004d2d289 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2d289)
#<!-- -->24 0x0000000000daba2f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaba2f)
#<!-- -->25 0x0000000000da1bfa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->26 0x00000000049aa2c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x0000000003edbb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edbb94)
#<!-- -->28 0x00000000049aa8df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x000000000496c6ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496c6ad)
#<!-- -->30 0x000000000496d73e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496d73e)
#<!-- -->31 0x0000000004975a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4975a25)
#<!-- -->32 0x0000000000da7998 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7998)
#<!-- -->33 0x0000000000c2d064 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2d064)
#<!-- -->34 0x00007b2b24629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x00007b2b24629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000da16a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda16a5)
```


## Program

```
#include &lt;stdlib.h&gt;
char static allocd[((size_t)-1) / 256] = "\1";
int main(void)
{
    free(malloc(0));
    allocd[32] = '\2';
}
```

To quickly reproduce:

https://godbolt.org/z/KbjTcdzKM
</details>


---

### Comment 2 - AaronBallman

Smaller reproducer without headers:
```
typedef decltype(sizeof(0)) size_t;
char static allocd[((size_t)-1) / 256] = "\1";
int main(void)
{
    allocd[32] = '\2';
}
```

---

### Comment 3 - AaronBallman

I think this is code that should be rejected at Sema for being too large. GCC and MSVC both reject: https://godbolt.org/z/bM55s518T

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (mariete1223)

<details>
In C++ it happens since clang 15 until trunk and in C since 3.0.0

## Assertion

No assertion given

## Stack dump

```
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -x c++ -std=c++23 &lt;source&gt;
1.	&lt;eof&gt; parser at end of file
2.	Per-file LLVM IR generation
3.	&lt;source&gt;:2:13: Generating code for declaration 'allocd'
 #<!-- -->0 0x0000000003f98e38 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f98e38)
 #<!-- -->1 0x0000000003f96ac4 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f96ac4)
 #<!-- -->2 0x0000000003edb6f8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x00007b2b24642520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00007b2b246969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00007b2b24642476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00007b2b246287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x0000000003ee6efb (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ee6efb)
 #<!-- -->8 0x0000000003f15ead llvm::SmallVectorBase&lt;unsigned long&gt;::grow_pod(void*, unsigned long, unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3f15ead)
 #<!-- -->9 0x000000000450bf3b clang::CodeGen::CodeGenModule::GetConstantArrayFromStringLiteral(clang::StringLiteral const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x450bf3b)
#<!-- -->10 0x000000000434e03c clang::StmtVisitorBase&lt;llvm::make_const_ptr, (anonymous namespace)::ConstExprEmitter, llvm::Constant*, clang::QualType&gt;::Visit(clang::Stmt const*, clang::QualType) CGExprConstant.cpp:0:0
#<!-- -->11 0x0000000004350313 clang::CodeGen::ConstantEmitter::tryEmitPrivateForVarInit(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4350313)
#<!-- -->12 0x0000000004350591 clang::CodeGen::ConstantEmitter::tryEmitForInitializer(clang::VarDecl const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4350591)
#<!-- -->13 0x000000000453a4ce clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x453a4ce)
#<!-- -->14 0x000000000455ef31 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x455ef31)
#<!-- -->15 0x000000000456b797 clang::CodeGen::CodeGenModule::EmitDeferred() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456b797)
#<!-- -->16 0x000000000456dd23 clang::CodeGen::CodeGenModule::Release() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x456dd23)
#<!-- -->17 0x000000000494640e (anonymous namespace)::CodeGeneratorImpl::HandleTranslationUnit(clang::ASTContext&amp;) ModuleBuilder.cpp:0:0
#<!-- -->18 0x0000000004942825 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4942825)
#<!-- -->19 0x000000000662637c clang::ParseAST(clang::Sema&amp;, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x662637c)
#<!-- -->20 0x0000000004942fd8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4942fd8)
#<!-- -->21 0x0000000004c39725 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4c39725)
#<!-- -->22 0x0000000004bb834e clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4bb834e)
#<!-- -->23 0x0000000004d2d289 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4d2d289)
#<!-- -->24 0x0000000000daba2f cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xdaba2f)
#<!-- -->25 0x0000000000da1bfa ExecuteCC1Tool(llvm::SmallVectorImpl&lt;char const*&gt;&amp;, llvm::ToolContext const&amp;) driver.cpp:0:0
#<!-- -->26 0x00000000049aa2c9 void llvm::function_ref&lt;void ()&gt;::callback_fn&lt;clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const::'lambda'()&gt;(long) Job.cpp:0:0
#<!-- -->27 0x0000000003edbb94 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3edbb94)
#<!-- -->28 0x00000000049aa8df clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (.part.0) Job.cpp:0:0
#<!-- -->29 0x000000000496c6ad clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496c6ad)
#<!-- -->30 0x000000000496d73e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x496d73e)
#<!-- -->31 0x0000000004975a25 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4975a25)
#<!-- -->32 0x0000000000da7998 clang_main(int, char**, llvm::ToolContext const&amp;) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda7998)
#<!-- -->33 0x0000000000c2d064 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xc2d064)
#<!-- -->34 0x00007b2b24629d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#<!-- -->35 0x00007b2b24629e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#<!-- -->36 0x0000000000da16a5 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xda16a5)
```


## Program

```
#include &lt;stdlib.h&gt;
char static allocd[((size_t)-1) / 256] = "\1";
int main(void)
{
    free(malloc(0));
    allocd[32] = '\2';
}
```

To quickly reproduce:

https://godbolt.org/z/KbjTcdzKM
</details>


---

### Comment 5 - AaronBallman

We already diagnose when the size is too large:https://github.com/llvm/llvm-project/blob/9273091502994f9b68ca0d1fd04fadd02c0a36df/clang/lib/Sema/SemaType.cpp#L2272

The number of active bits for this repro is 56 bits, but the max size we compare against is 61 bits, which is why we don't diagnose.

Maybe we need the max to be the union of what can be allocated on the host platform via `malloc` (that's what the constant expression evaluation needs) and target platform address space?

---

