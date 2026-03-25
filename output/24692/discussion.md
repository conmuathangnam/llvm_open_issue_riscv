# Segmentation fault when using nested templated typedefs

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/24692

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [24318](https://llvm.org/bz24318) |
| Version | 3.6 |
| OS | Linux |
| Attachments | [Source code](https://user-images.githubusercontent.com/60944935/143751794-b6040086-67ba-40b2-be39-8330f08169f9.gz) |
| Reporter | LLVM Bugzilla Contributor |
| CC | @DougGregor,@zmodem |

## Extended Description 


Some of our code makes clang 3.6.2-1 (Arch Linux) crash.
Based on our code, I crafted a very small example demonstrating this bug.

Basically, we have a templated class A with a nested class B that is also templated.
Now, we have another templated class C that defines a typedef for A<T>::B<T>.
Finally, we try to use this typedef in the main function.

Clang bails out on this:

$ clang++ clangcrash.cpp
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 3.6.2 (tags/RELEASE_362/final)
Target: x86_64-unknown-linux-gnu
Thread model: posix
clang: note: diagnostic msg: PLEASE submit a bug report to http://llvm.org/bugs/ and include the crash backtrace, preprocessed source, and associated run script.
clang: note: diagnostic msg: 
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang: note: diagnostic msg: /tmp/clangcrash-21862e.cpp
clang: note: diagnostic msg: /tmp/clangcrash-21862e.sh
clang: note: diagnostic msg: 

********************


Attached, you find the example.

## Comments

### Comment 1 - llvmbot

This bug persists as of clang 4.0.1

---

### Comment 2 - zmodem

Since this occurs on 3.6.2 it's not a regression in 3.7 and I won't consider it a blocker.

Still would be nice to get fixed, of course.

Pasting the example for convenience:
```
template<typename T>
struct A {
	template<typename T2> struct B {};
};

template<typename T>
struct C {
	typedef A<T>::B<T> B;
};

int main() {
	C<int>::B it;
}
```

---

### Comment 3 - zmodem

Reproduces on trunk too.

---

### Comment 4 - Endilll

Still crashing as of post-17 trunk: https://godbolt.org/z/E6MWbrnEG
```
<source>:8:10: warning: missing 'typename' prior to dependent type name A<T>::B; implicit 'typename' is a C++20 extension [-Wc++20-extensions]
    8 |         typedef A<T>::B<T> B;
      |                 ^
      |                 typename 
clang++: /root/build/tools/clang/include/clang/AST/TypeNodes.inc:71:
clang::TypeInfo clang::ASTContext::getTypeInfoImpl(const clang::Type*) const:
Assertion `!T->isDependentType() && "should not see dependent types here"' failed.

PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++11 <source>
1.	<eof> parser at end of file
2.	<source>:11:5: LLVM IR generation of declaration 'main'
3.	<source>:11:5: Generating code for declaration 'main'
 #0 0x000000000367dce8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x367dce8)
 #1 0x000000000367bb6c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x367bb6c)
 #2 0x00000000035c75a8 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x00007f011f483420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f011ef4600b raise (/lib/x86_64-linux-gnu/libc.so.6+0x4300b)
 #5 0x00007f011ef25859 abort (/lib/x86_64-linux-gnu/libc.so.6+0x22859)
 #6 0x00007f011ef25729 (/lib/x86_64-linux-gnu/libc.so.6+0x22729)
 #7 0x00007f011ef36fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x0000000006c6b8b4 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6b8b4)
 #9 0x0000000006c593d7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c593d7)
#10 0x0000000006c6a9c3 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6a9c3)
#11 0x0000000006c593d7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c593d7)
#12 0x0000000006c6adbd clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6adbd)
#13 0x0000000006c593d7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c593d7)
#14 0x0000000006c6a9c3 clang::ASTContext::getTypeInfoImpl(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6a9c3)
#15 0x0000000006c593d7 clang::ASTContext::getTypeInfo(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c593d7)
#16 0x0000000006c6bf2b clang::ASTContext::getPreferredTypeAlign(clang::Type const*) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6bf2b)
#17 0x0000000006c6c85b clang::ASTContext::getDeclAlign(clang::Decl const*, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x6c6c85b)
#18 0x0000000003d469b0 clang::CodeGen::CodeGenFunction::EmitAutoVarAlloca(clang::VarDecl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d469b0)
#19 0x0000000003d47cf3 clang::CodeGen::CodeGenFunction::EmitVarDecl(clang::VarDecl const&) (.part.0) CGDecl.cpp:0:0
#20 0x0000000003d47fa0 clang::CodeGen::CodeGenFunction::EmitDecl(clang::Decl const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3d47fa0)
#21 0x0000000003987537 clang::CodeGen::CodeGenFunction::EmitDeclStmt(clang::DeclStmt const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3987537)
#22 0x000000000399b367 clang::CodeGen::CodeGenFunction::EmitSimpleStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x399b367)
#23 0x00000000039947b5 clang::CodeGen::CodeGenFunction::EmitStmt(clang::Stmt const*, llvm::ArrayRef<clang::Attr const*>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39947b5)
#24 0x000000000399ac9c clang::CodeGen::CodeGenFunction::EmitCompoundStmtWithoutScope(clang::CompoundStmt const&, bool, clang::CodeGen::AggValueSlot) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x399ac9c)
#25 0x00000000039f8476 clang::CodeGen::CodeGenFunction::EmitFunctionBody(clang::Stmt const*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39f8476)
#26 0x0000000003a0b56a clang::CodeGen::CodeGenFunction::GenerateCode(clang::GlobalDecl, llvm::Function*, clang::CodeGen::CGFunctionInfo const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a0b56a)
#27 0x0000000003a6cbed clang::CodeGen::CodeGenModule::EmitGlobalFunctionDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a6cbed)
#28 0x0000000003a67d85 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a67d85)
#29 0x0000000003a68343 clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3a68343)
#30 0x0000000003a71533 clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.0) CodeGenModule.cpp:0:0
#31 0x00000000048da096 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#32 0x00000000048cc838 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48cc838)
#33 0x0000000005d965e4 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x5d965e4)
#34 0x00000000048d7108 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x48d7108)
#35 0x000000000413e729 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x413e729)
#36 0x00000000040c26ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x40c26ee)
#37 0x000000000421edc6 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x421edc6)
#38 0x0000000000bce587 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbce587)
#39 0x0000000000bc6c3a ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#40 0x0000000003f21a89 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#41 0x00000000035c7a54 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x35c7a54)
#42 0x0000000003f2207f clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#43 0x0000000003eea965 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eea965)
#44 0x0000000003eeb3cd clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3eeb3cd)
#45 0x0000000003ef3325 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x3ef3325)
#46 0x0000000000bcc81c clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbcc81c)
#47 0x0000000000ac5c11 main (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xac5c11)
#48 0x00007f011ef27083 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x24083)
#49 0x0000000000bc671e _start (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0xbc671e)
clang++: error: clang frontend command failed with exit code 134 (use -v to see invocation)
```

---

### Comment 5 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

### Comment 6 - jyknight

I believe this bug has become a crash-on-valid bug as of C++20, because, AFAICT, it is valid C++20 to omit `typename` here on line 8, but doing so crashes clang.

Or, if it's not valid, the other compilers haven't noticed -- this code compiles as C++20 with both GCC and MSVC, https://godbolt.org/z/j8jE515Y1:
```
template<typename T>
struct A {
  template<typename T2> struct B {};
};

template<typename T>
struct C {
  typedef A<T>::template B<T> B;
};

int main() {
  C<int>::B it;
}
```

---

### Comment 7 - zwuis

Maybe related: #17300.

---

