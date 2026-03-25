# Crash in getTypeInfoImpl when combining _Atomic and __auto_type

**Author:** EvanED
**URL:** https://github.com/llvm/llvm-project/issues/179773

## Body

I work on tooling, and was adapting https://github.com/llvm/llvm-project/blob/e209b8bc2f87d53dc9003f7f09ce14b61894bf6c/clang/test/Sema/auto-type.c as a test case for my setting. I don't think I can really use `-fsyntax-only` (as your test case does), so I was building with `-c` (after removing error-producing lines, etc.); but this causes a crash that does not surface with `-fsyntax-only`. This crash occurs with `-emit-llvm -Xclang -disable-llvm-passes`.

The problem occurs with all the Clang versions I tested, including version 20, 21, and a couple recent-ish dev versions, built by both me and others. The stack trace I've posted below is from the version of clang packaged up in the Dockerhub image `silkeh/clang:21`:
```
root@f393e65a9636:/# clang --version
Debian clang version 21.1.5 (++20251023083151+45afac62e373-1~exp1~20251023083333.51)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
```

Test case:
```c
void Issue53652(void) {
  _Atomic __auto_type aat1 = 4;
}
```
(Note that the name of this function I didn't change from your test case, but obviously the number in that name is independent of *this* issue.)

Command line: `clang -emit-llvm -Xclang -disable-llvm-passes -pedantic -std=c11 -c test.c`

Crash message:
```
test.c:2:11: warning: '__auto_type' is a GNU extension [-Wgnu-auto-type]
    2 |   _Atomic __auto_type aat1 = 4;
      |           ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -emit-llvm -Xclang -disable-llvm-passes -pedantic -std=c11 -c test.c
1.	<eof> parser at end of file
2.	test.c:1:6: LLVM IR generation of declaration 'Issue53652'
3.	test.c:1:6: Generating code for declaration 'Issue53652'
 #0 0x00007c463ba62306 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbcc306)
 #1 0x00007c463ba5fb53 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbc9b53)
 #2 0x00007c463b9982f0 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xb022f0)
 #3 0x00007c463a9d0050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #4 0x00007c4644056b2e clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc5db2e)
 #5 0x00007c4644059033 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc60033)
 #6 0x00007c4644056eab clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc5deab)
 #7 0x00007c4644057ca1 clang::ASTContext::getPreferredTypeAlign(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc5eca1)
 #8 0x00007c46440562c0 clang::ASTContext::getDeclAlign(clang::Decl const*, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc5d2c0)
 #9 0x00007c464518ebb3 clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1d95bb3)
#10 0x00007c464518a7fc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1d917fc)
#11 0x00007c464518a45d clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1d9145d)
#12 0x00007c46453364c0 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1f3d4c0)
#13 0x00007c4645335a63 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1f3ca63)
#14 0x00007c464534109c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1f4809c)
#15 0x00007c46453a253e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fa953e)
#16 0x00007c46453c4c15 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fcbc15)
#17 0x00007c46453be49b clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fc549b)
#18 0x00007c46453c23b5 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fc93b5)
#19 0x00007c46453bd0c1 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fc40c1)
#20 0x00007c4645464eec (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x206beec)
#21 0x00007c4645392369 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1f99369)
#22 0x00007c4643f7005a clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb7705a)
#23 0x00007c4645b2b80b clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x273280b)
#24 0x00007c4645a9cb94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x26a3b94)
#25 0x00007c4645bb3c2e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27bac2e)
#26 0x0000613bc38e9939 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-21/bin/clang+0x12939)
#27 0x0000613bc38e65b5 (/usr/lib/llvm-21/bin/clang+0xf5b5)
#28 0x00007c46457622a9 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x23692a9)
#29 0x00007c463b997fb8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xb01fb8)
#30 0x00007c4645761c2d clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2368c2d)
#31 0x00007c464572488e clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x232b88e)
#32 0x00007c4645724a4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x232ba4e)
#33 0x00007c46457437bd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x234a7bd)
#34 0x0000613bc38e6098 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-21/bin/clang+0xf098)
#35 0x0000613bc38f3e06 main (/usr/lib/llvm-21/bin/clang+0x1ce06)
#36 0x00007c463a9bb24a (/lib/x86_64-linux-gnu/libc.so.6+0x2724a)
#37 0x00007c463a9bb305 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x27305)
#38 0x0000613bc38e42d1 _start (/usr/lib/llvm-21/bin/clang+0xd2d1)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 21.1.5 (++20251023083151+45afac62e373-1~exp1~20251023083333.51)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-184795.c
clang: note: diagnostic msg: /tmp/test-184795.sh
clang: note: diagnostic msg: 

********************
```

[test-184795.c](https://github.com/user-attachments/files/25080865/test-184795.c)
[test-184795.sh](https://github.com/user-attachments/files/25080866/test-184795.sh)


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Evan Driscoll (EvanED)

<details>
I work on tooling, and was adapting https://github.com/llvm/llvm-project/blob/e209b8bc2f87d53dc9003f7f09ce14b61894bf6c/clang/test/Sema/auto-type.c as a test case for my setting. I don't think I can really use `-fsyntax-only` (as your test case does), so I was building with `-c` (after removing error-producing lines, etc.); but this causes a crash that does not surface with `-fsyntax-only`. This crash occurs with `-emit-llvm -Xclang -disable-llvm-passes`.

The problem occurs with all the Clang versions I tested, including version 20, 21, and a couple recent-ish dev versions, built by both me and others. The stack trace I've posted below is from the version of clang packaged up in the Dockerhub image `silkeh/clang:21`:
```
root@<!-- -->f393e65a9636:/# clang --version
Debian clang version 21.1.5 (++20251023083151+45afac62e373-1~exp1~20251023083333.51)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
```

Test case:
```c
void Issue53652(void) {
  _Atomic __auto_type aat1 = 4;
}
```
(Note that the name of this function I didn't change from your test case, but obviously the number in that name is independent of *this* issue.)

Command line: `clang -emit-llvm -Xclang -disable-llvm-passes -pedantic -std=c11 -c test.c`

Crash message:
```
test.c:2:11: warning: '__auto_type' is a GNU extension [-Wgnu-auto-type]
    2 |   _Atomic __auto_type aat1 = 4;
      |           ^
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: clang -emit-llvm -Xclang -disable-llvm-passes -pedantic -std=c11 -c test.c
1.	&lt;eof&gt; parser at end of file
2.	test.c:1:6: LLVM IR generation of declaration 'Issue53652'
3.	test.c:1:6: Generating code for declaration 'Issue53652'
 #<!-- -->0 0x00007c463ba62306 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbcc306)
 #<!-- -->1 0x00007c463ba5fb53 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xbc9b53)
 #<!-- -->2 0x00007c463b9982f0 (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xb022f0)
 #<!-- -->3 0x00007c463a9d0050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #<!-- -->4 0x00007c4644056b2e clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc5db2e)
 #<!-- -->5 0x00007c4644059033 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc60033)
 #<!-- -->6 0x00007c4644056eab clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc5deab)
 #<!-- -->7 0x00007c4644057ca1 clang::ASTContext::getPreferredTypeAlign(clang::Type const*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc5eca1)
 #<!-- -->8 0x00007c46440562c0 clang::ASTContext::getDeclAlign(clang::Decl const*, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xc5d2c0)
 #<!-- -->9 0x00007c464518ebb3 clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1d95bb3)
#<!-- -->10 0x00007c464518a7fc clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1d917fc)
#<!-- -->11 0x00007c464518a45d clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&amp;, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1d9145d)
#<!-- -->12 0x00007c46453364c0 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1f3d4c0)
#<!-- -->13 0x00007c4645335a63 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef&lt;clang::Attr const*&gt;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1f3ca63)
#<!-- -->14 0x00007c464534109c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&amp;, bool, clang::CodeGen::AggValueSlot) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1f4809c)
#<!-- -->15 0x00007c46453a253e clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fa953e)
#<!-- -->16 0x00007c46453c4c15 clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fcbc15)
#<!-- -->17 0x00007c46453be49b clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fc549b)
#<!-- -->18 0x00007c46453c23b5 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fc93b5)
#<!-- -->19 0x00007c46453bd0c1 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1fc40c1)
#<!-- -->20 0x00007c4645464eec (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x206beec)
#<!-- -->21 0x00007c4645392369 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x1f99369)
#<!-- -->22 0x00007c4643f7005a clang::ParseAST(clang::Sema&amp;, bool, bool) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0xb7705a)
#<!-- -->23 0x00007c4645b2b80b clang::FrontendAction::Execute() (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x273280b)
#<!-- -->24 0x00007c4645a9cb94 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x26a3b94)
#<!-- -->25 0x00007c4645bb3c2e clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x27bac2e)
#<!-- -->26 0x0000613bc38e9939 cc1_main(llvm::ArrayRef&lt;char const*&gt;, char const*, void*) (/usr/lib/llvm-21/bin/clang+0x12939)
#<!-- -->27 0x0000613bc38e65b5 (/usr/lib/llvm-21/bin/clang+0xf5b5)
#<!-- -->28 0x00007c46457622a9 (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x23692a9)
#<!-- -->29 0x00007c463b997fb8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref&lt;void ()&gt;) (/usr/lib/llvm-21/bin/../lib/libLLVM.so.21.1+0xb01fb8)
#<!-- -->30 0x00007c4645761c2d clang::driver::CC1Command::Execute(llvm::ArrayRef&lt;std::optional&lt;llvm::StringRef&gt;&gt;, std::__cxx11::basic_string&lt;char, std::char_traits&lt;char&gt;, std::allocator&lt;char&gt;&gt;*, bool*) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x2368c2d)
#<!-- -->31 0x00007c464572488e clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&amp;, clang::driver::Command const*&amp;, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x232b88e)
#<!-- -->32 0x00007c4645724a4e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;, bool) const (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x232ba4e)
#<!-- -->33 0x00007c46457437bd clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&amp;, llvm::SmallVectorImpl&lt;std::pair&lt;int, clang::driver::Command const*&gt;&gt;&amp;) (/usr/lib/llvm-21/bin/../lib/libclang-cpp.so.21.1+0x234a7bd)
#<!-- -->34 0x0000613bc38e6098 clang_main(int, char**, llvm::ToolContext const&amp;) (/usr/lib/llvm-21/bin/clang+0xf098)
#<!-- -->35 0x0000613bc38f3e06 main (/usr/lib/llvm-21/bin/clang+0x1ce06)
#<!-- -->36 0x00007c463a9bb24a (/lib/x86_64-linux-gnu/libc.so.6+0x2724a)
#<!-- -->37 0x00007c463a9bb305 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x27305)
#<!-- -->38 0x0000613bc38e42d1 _start (/usr/lib/llvm-21/bin/clang+0xd2d1)
clang: error: clang frontend command failed with exit code 139 (use -v to see invocation)
Debian clang version 21.1.5 (++20251023083151+45afac62e373-1~exp1~20251023083333.51)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/lib/llvm-21/bin
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/test-184795.c
clang: note: diagnostic msg: /tmp/test-184795.sh
clang: note: diagnostic msg: 

********************
```

[test-184795.c](https://github.com/user-attachments/files/25080865/test-184795.c)
[test-184795.sh](https://github.com/user-attachments/files/25080866/test-184795.sh)

</details>


---

### Comment 2 - Fznamznon

Confirmed https://godbolt.org/z/eMsv7KcTx . gcc accepts.
We reject `_Atomic auto` in C23 instead of crash, gcc accepts as well https://godbolt.org/z/h19f16xcG .
cc @AaronBallman 

---

### Comment 3 - AaronBallman

> Confirmed https://godbolt.org/z/eMsv7KcTx . gcc accepts. We reject `_Atomic auto` in C23 instead of crash, gcc accepts as well https://godbolt.org/z/h19f16xcG . cc [@AaronBallman](https://github.com/AaronBallman)

Agreed.

 `_Atomic auto i = 12;` is something we have to accept because `_Atomic` is a qualifier. `_Atomic(auto) i = 12;` is something we have to reject because `_Atomic` requires a `type-name` in that case and `auto` is not a `type-name` (it's a `storage-class-specifier` still); we do get the latter behavior correct, while the former is incorrectly rejected.

---

