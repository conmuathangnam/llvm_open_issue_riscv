# Crash with `#pragma GCC unroll` when calling value which can't be called in template function

**Author:** c44d6195-dfe2-40f4-a90f-bebb7462f3a2
**URL:** https://github.com/llvm/llvm-project/issues/49502
**Status:** Closed
**Labels:** c++, clang:frontend, bugzilla, confirmed, crash
**Closed Date:** 2026-02-24T17:38:11Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [50158](https://llvm.org/bz50158) |
| Version | trunk |
| OS | Linux |
| CC | @DougGregor,@zygoloid |

## Extended Description 
```cpp
template<typename T> void f(T v) {
    #pragma GCC unroll v()
    for (;;) {
    }
} 

int main() {
  f(0);
}
```
https://godbolt.org/z/8bEscGszK

Amusingly, this gets gcc and clang.

## Comments

### Comment 1 - wheatman

this still reproduces on post 16 trunk (12e9c7aaa66b7624b5d7666ce2794d912bf9e4b7)

https://godbolt.org/z/M63cTKTf1

The assert that gets triggered is 
```
clang++: /root/llvm-project/clang/lib/Sema/SemaExpr.cpp:3793: 
bool clang::Sema::CheckLoopHintExpr(clang::Expr*, clang::SourceLocation): 
Assertion `E && "Invalid expression"' failed.

```

The full backtrace is 

```
<source>:2:24: error: called object type 'int' is not a function or function pointer
    2 |     #pragma GCC unroll v()
      |                        ^
<source>:8:3: note: in instantiation of function template specialization 'f<int>' requested here
    8 |   f(0);
      |   ^
clang++: /root/llvm-project/clang/lib/Sema/SemaExpr.cpp:3793: bool clang::Sema::CheckLoopHintExpr(clang::Expr*, clang::SourceLocation): Assertion `E && "Invalid expression"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	<source>:1:27: instantiating function definition 'f<int>'
 #0 0x00005600a53adf2f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c52f2f)
 #1 0x00005600a53abc9c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3c50c9c)
 #2 0x00005600a52f6398 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007fed689f1420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007fed684be00b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007fed6849d859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007fed6849d729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007fed684aefd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x00005600a80c8998 clang::Sema::CheckLoopHintExpr(clang::Expr*, clang::SourceLocation) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x696d998)
 #9 0x00005600a8673002 (anonymous namespace)::TemplateInstantiator::TransformLoopHintAttr(clang::LoopHintAttr const*) SemaTemplateInstantiate.cpp:0:0
#10 0x00005600a86a7b89 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformAttributedStmt(clang::AttributedStmt*, clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::StmtDiscardKind) SemaTemplateInstantiate.cpp:0:0
#11 0x00005600a86a4e19 clang::TreeTransform<(anonymous namespace)::TemplateInstantiator>::TransformCompoundStmt(clang::CompoundStmt*, bool) SemaTemplateInstantiate.cpp:0:0
#12 0x00005600a86a7f6e clang::Sema::SubstStmt(clang::Stmt*, clang::MultiLevelTemplateArgumentList const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f4cf6e)
#13 0x00005600a86f8b68 clang::Sema::InstantiateFunctionDefinition(clang::SourceLocation, clang::FunctionDecl*, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9db68)
#14 0x00005600a86f6f6f clang::Sema::PerformPendingInstantiations(bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6f9bf6f)
#15 0x00005600a7ca23c0 clang::Sema::ActOnEndOfTranslationUnitFragment(clang::Sema::TUFragmentKind) (.part.0) Sema.cpp:0:0
#16 0x00005600a7ca2a8a clang::Sema::ActOnEndOfTranslationUnit() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6547a8a)
#17 0x00005600a7b31243 clang::Parser::ParseTopLevelDecl(clang::OpaquePtr<clang::DeclGroupRef>&, clang::Sema::ModuleImportState&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63d6243)
#18 0x00005600a7b24e2a clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x63c9e2a)
#19 0x00005600a66279a8 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4ecc9a8)
#20 0x00005600a5e71b79 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x4716b79)
#21 0x00005600a5df74c6 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x469c4c6)
#22 0x00005600a5f562a6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x47fb2a6)
#23 0x00005600a28500ed cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10f50ed)
#24 0x00005600a284bdea ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#25 0x00005600a5c5623d void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#26 0x00005600a52f68a0 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3b9b8a0)
#27 0x00005600a5c5685f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#28 0x00005600a5c1d9ec clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44c29ec)
#29 0x00005600a5c1e47d clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44c347d)
#30 0x00005600a5c2655d clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x44cb55d)
#31 0x00005600a284e34a clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10f334a)
#32 0x00005600a27545c5 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xff95c5)
#33 0x00007fed6849f083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#34 0x00005600a2846bce _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x10ebbce)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
Compiler returned: 134
```

---

### Comment 2 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 3 - Endilll

This should be reported to GCC as well: https://godbolt.org/z/drY9W9dGo

---

### Comment 4 - pinskia

> This should be reported to GCC as well: https://godbolt.org/z/drY9W9dGo

Had already been reported https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100326 .

---

