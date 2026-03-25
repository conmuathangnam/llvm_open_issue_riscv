# [Clang] `[[clang::objc_direct]]` causes an ICE when used on a method in a category

**Author:** Frityet
**URL:** https://github.com/llvm/llvm-project/issues/172189
**Status:** Open
**Labels:** crash, llvm

## Body

```yaml
Stack dump:
0.      Program arguments: clang -I/home/node/.xmake/packages/o/objfw/1.4.1/b3120e81aff847feb32e5f5d7409fbd3/include -fexceptions -fobjc-exceptions -funwind-tables -fconstant-string-class=OFConstantString -Xclang -fno-constant-cfstrings -Xclang -fobjc-runtime=objfw-1.4 -Xclang -fblocks -Wall -g -std=gnu23 -c -o test.o test.m
1.      <eof> parser at end of file
2.      Code generation
 #0 0x00007f2b3f1bb9b6 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xc189b6)
 #1 0x00007f2b3f1b9203 llvm::sys::RunSignalHandlers() (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xc16203)
 #2 0x00007f2b3f0ea1f0 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xb471f0)
 #3 0x00007f2b3e0d8050 (/lib/x86_64-linux-gnu/libc.so.6+0x3c050)
 #4 0x00007f2b3fba2ebe (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x15ffebe)
 #5 0x00007f2b3fba6c78 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x1603c78)
 #6 0x00007f2b3fba6b78 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x1603b78)
 #7 0x00007f2b3fba6c78 (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x1603c78)
 #8 0x00007f2b3fb95943 llvm::AsmPrinter::emitGlobalVariable(llvm::GlobalVariable const*) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x15f2943)
 #9 0x00007f2b3fb9ea40 llvm::AsmPrinter::doFinalization(llvm::Module&) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0x15fba40)
#10 0x00007f2b3f329491 llvm::FPPassManager::doFinalization(llvm::Module&) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xd86491)
#11 0x00007f2b3f3248a2 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xd818a2)
#12 0x00007f2b48eb2c1a clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x1d03c1a)
#13 0x00007f2b491ba5d9 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x200b5d9)
#14 0x00007f2b47d5ed59 clang::ParseAST(clang::Sema&, bool, bool) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0xbafd59)
#15 0x00007f2b4996632b clang::FrontendAction::Execute() (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x27b732b)
#16 0x00007f2b498d8554 clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x2729554)
#17 0x00007f2b499f13aa clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x28423aa)
#18 0x0000561446c85b89 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/usr/lib/llvm-22/bin/clang+0x12b89)
#19 0x0000561446c8282b (/usr/lib/llvm-22/bin/clang+0xf82b)
#20 0x0000561446c841c8 (/usr/lib/llvm-22/bin/clang+0x111c8)
#21 0x00007f2b4959ab79 (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x23ebb79)
#22 0x00007f2b3f0e9eb8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/usr/lib/llvm-22/bin/../lib/libLLVM.so.22.0+0xb46eb8)
#23 0x00007f2b4959a4fd clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x23eb4fd)
#24 0x00007f2b4956186e clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x23b286e)
#25 0x00007f2b49561a2e clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x23b2a2e)
#26 0x00007f2b4957a62f clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/usr/lib/llvm-22/bin/../lib/libclang-cpp.so.22.0+0x23cb62f)
#27 0x0000561446c82315 clang_main(int, char**, llvm::ToolContext const&) (/usr/lib/llvm-22/bin/clang+0xf315)
#28 0x0000561446c90036 main (/usr/lib/llvm-22/bin/clang+0x1d036)
#29 0x00007f2b3e0c324a (/lib/x86_64-linux-gnu/libc.so.6+0x2724a)
#30 0x00007f2b3e0c3305 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x27305)
#31 0x0000561446c804e1 _start (/usr/lib/llvm-22/bin/clang+0xd4e1)
```

`test-29ef77.m`:

```objc
# 1 "<built-in>"
# 1 "test.m"

@interface Root @end

@interface Root(Cat)
- (void)g: (id)predicate [[clang::objc_direct]];
@end

@implementation Root(Cat)

- (void)g: (id)predicate
{
}

@end
```

[test-29ef77.sh](https://github.com/user-attachments/files/24146693/test-29ef77.sh)

[Godbolt reproducer](https://godbolt.org/z/ac5Mn787P)

