# Passing -mllvm -pass-remarks-analysis=* to clang causes fatal error

**Author:** jkorous-apple
**URL:** https://github.com/llvm/llvm-project/issues/125781

## Body

https://godbolt.org/z/EnKE67rYT

```
fatal error: error in backend: Invalid regular expression '*' in -pass-remarks: repetition-operator operand invalid
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -mllvm -pass-remarks-analysis=* <source>
 #0 0x0000000003b14158 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b14158)
 #1 0x0000000003b1229c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b1229c)
 #2 0x0000000003a61b03 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a61b03)
 #3 0x0000000003b09c3e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3b09c3e)
 #4 0x0000000000d4f20b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003a6bbd3 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a6bbd3)
 #6 0x0000000003541a91 (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3541a91)
 #7 0x0000000003541bc6 llvm::cl::opt<(anonymous namespace)::PassRemarksOpt, true, llvm::cl::parser<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>>::handleOccurrence(unsigned int, llvm::StringRef, llvm::StringRef) DiagnosticHandler.cpp:0:0
 #8 0x0000000003a4dff7 ProvideOption(llvm::cl::Option*, llvm::StringRef, llvm::StringRef, int, char const* const*, int&) CommandLine.cpp:0:0
 #9 0x0000000003a5c72f llvm::cl::ParseCommandLineOptions(int, char const* const*, llvm::StringRef, llvm::raw_ostream*, char const*, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a5c72f)
#10 0x00000000048275a4 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x48275a4)
#11 0x0000000000d51441 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd51441)
#12 0x0000000000d49c4d ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&) driver.cpp:0:0
#13 0x00000000044b1139 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#14 0x0000000003a61a03 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3a61a03)
#15 0x00000000044b1359 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#16 0x000000000447557d clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x447557d)
#17 0x0000000004476511 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4476511)
#18 0x000000000448089c clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x448089c)
#19 0x0000000000d4e0d1 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd4e0d1)
#20 0x0000000000c130b4 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc130b4)
#21 0x000075f681c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#22 0x000075f681c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#23 0x0000000000d496e5 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xd496e5)
```

## Comments

### Comment 1 - jroelofs

I think it is expected that backend flags are "I know what I'm doing", and have some sharp edges. That should be spelled: `-mllvm -pass-remarks-analysis=.*`

You'll get better diagnostics if you use the frontend spelling of the flag: `-Rpass-analysis=`: https://godbolt.org/z/Trzr64o7K

---

