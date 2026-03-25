# Compiler crash when incorrectly define template variable with auto type

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/51347
**Status:** Closed
**Labels:** c++20, clang:frontend, bugzilla, crash-on-invalid, confirmed
**Closed Date:** 2025-05-07T00:35:13Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [52005](https://llvm.org/bz52005) |
| Version | 12.0 |
| OS | Windows NT |
| Reporter | LLVM Bugzilla Contributor |
| CC | @AaronBallman,@zygoloid |

## Extended Description 
The code as follows:
```
template <typename T>
auto p = p<T>;

auto x = p<int>;
```

crashes the compiler:
https://gcc.godbolt.org/z/a99aThzjj

PLEASE submit a bug report to https://bugs.llvm.org/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++20 -Wall -Wextra -pedantic-errors <source>
1.	<eof> parser at end of file
2.	<source>:4:6: LLVM IR generation of declaration 'x'
3.	<source>:4:6: Generating code for declaration 'x'
 #&#8203;0 0x0000556e3b7a626f PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #&#8203;1 0x0000556e3b7a4130 llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x35a4130)
 #&#8203;2 0x0000556e3b6f4b38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #&#8203;3 0x00007fd94dedf3c0 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x153c0)
 #&#8203;4 0x0000556e3ae87c04 llvm::ConstantFP::get(llvm::LLVMContext&, llvm::APFloat const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x2c87c04)
 #&#8203;5 0x0000556e3ae91adf llvm::Constant::getNullValue(llvm::Type*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x2c91adf)
 #&#8203;6 0x0000556e3baeda1d clang::CodeGen::CodeGenModule::EmitNullConstant(clang::QualType) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x38eda1d)
 #&#8203;7 0x0000556e3bb8daa7 clang::CodeGen::CodeGenModule::EmitGlobalVarDefinition(clang::VarDecl const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x398daa7)
 #&#8203;8 0x0000556e3bba88f5 clang::CodeGen::CodeGenModule::EmitGlobalDefinition(clang::GlobalDecl, llvm::GlobalValue*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x39a88f5)
 #&#8203;9 0x0000556e3bba913b clang::CodeGen::CodeGenModule::EmitGlobal(clang::GlobalDecl) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x39a913b)
#&#8203;10 0x0000556e3bbaeded clang::CodeGen::CodeGenModule::EmitTopLevelDecl(clang::Decl*) (.part.5289) CodeGenModule.cpp:0:0
#&#8203;11 0x0000556e3c7474c1 (anonymous namespace)::CodeGeneratorImpl::HandleTopLevelDecl(clang::DeclGroupRef) ModuleBuilder.cpp:0:0
#&#8203;12 0x0000556e3c73ab32 clang::BackendConsumer::HandleTopLevelDecl(clang::DeclGroupRef) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x453ab32)
#&#8203;13 0x0000556e3d59f544 clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x539f544)
#&#8203;14 0x0000556e3c7453b2 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45453b2)
#&#8203;15 0x0000556e3c0db771 clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3edb771)
#&#8203;16 0x0000556e3c078382 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3e78382)
#&#8203;17 0x0000556e3c1a8b93 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3fa8b93)
#&#8203;18 0x0000556e394d71ec cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x12d71ec)
#&#8203;19 0x0000556e394d325d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#&#8203;20 0x0000556e3bf1cee5 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#&#8203;21 0x0000556e3b6f5123 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x34f5123)
#&#8203;22 0x0000556e3bf1ef9e clang::driver::CC1Command::Execute(llvm::ArrayRef<llvm::Optional<llvm::StringRef> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >*, bool*) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3d1ef9e)
#&#8203;23 0x0000556e3bef490a clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cf490a)
#&#8203;24 0x0000556e3bef544f clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cf544f)
#&#8203;25 0x0000556e3befe885 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*> >&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3cfe885)
#&#8203;26 0x0000556e393e71de main (/opt/compiler-explorer/clang-trunk/bin/clang+++0x11e71de)
#&#8203;27 0x00007fd94d98f0b3 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x270b3)
#&#8203;28 0x0000556e394d2dda _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0x12d2dda)
clang-14: error: clang frontend command failed with exit code 139 (use -v to see invocation)
ASM generation compiler returned: 139

## Comments

### Comment 1 - llvmbot

@llvm/issue-subscribers-clang-frontend

---

