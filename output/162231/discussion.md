# crash on __builtin_elementwise_minimumnum

**Author:** k-arrows
**URL:** https://github.com/llvm/llvm-project/issues/162231
**Status:** Open
**Labels:** backend:X86, crash, llvm:SelectionDAG

## Body

Reproducer:
https://godbolt.org/z/93zf9MYc4
```cpp
typedef long double ldouble2 __attribute__((ext_vector_type(2)));

ldouble2 foo(ldouble2 x, ldouble2 y) {
        return __builtin_elementwise_minimumnum(x, y);
}
```

Backtrace:
```console
fatal error: error in backend: Cannot select: 0x2c716180: i32 = X86ISD::FCMP 0x2c7202a0, 0x2c71c910, example.cpp:4:16
  0x2c7202a0: i80,ch = load<(load (s80) from %stack.3, align 16)> 0x2c720230, FrameIndex:i64<3>, undef:i64, example.cpp:4:16
  0x2c71c910: i80,ch = load<(load (s80) from constant-pool, align 16)> 0x2c6c7370, 0x2c715d20, undef:i64
    0x2c715d20: i64 = X86ISD::WrapperRIP TargetConstantPool:i64<i80 -604462909807314587353088> 0
In function: _Z3fooDv2_eS_
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-trunk/bin/clang++ -g -o /app/output.s -mllvm --x86-asm-syntax=intel -fno-verbose-asm -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics <source>
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '<source>'.
4.	Running pass 'X86 DAG->DAG Instruction Selection' on function '@_Z3fooDv2_eS_'
 #0 0x0000000003ca8078 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca8078)
 #1 0x0000000003ca5a4c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3ca5a4c)
 #2 0x0000000003bf5c83 llvm::CrashRecoveryContext::HandleExit(int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf5c83)
 #3 0x0000000003c9d28e llvm::sys::Process::Exit(int, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3c9d28e)
 #4 0x0000000000dc884b LLVMErrorHandler(void*, char const*, bool) cc1_main.cpp:0:0
 #5 0x0000000003bffa13 llvm::report_fatal_error(llvm::Twine const&, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bffa13)
 #6 0x00000000051e1afa llvm::SelectionDAGISel::CannotYetSelect(llvm::SDNode*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e1afa)
 #7 0x00000000051e5d29 llvm::SelectionDAGISel::SelectCodeCommon(llvm::SDNode*, unsigned char const*, unsigned int) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51e5d29)
 #8 0x0000000002797daa (anonymous namespace)::X86DAGToDAGISel::Select(llvm::SDNode*) X86ISelDAGToDAG.cpp:0:0
 #9 0x00000000051dec64 llvm::SelectionDAGISel::DoInstructionSelection() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51dec64)
#10 0x00000000051eb022 llvm::SelectionDAGISel::CodeGenAndEmitDAG() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51eb022)
#11 0x00000000051ed9aa llvm::SelectionDAGISel::SelectAllBasicBlocks(llvm::Function const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51ed9aa)
#12 0x00000000051ef8fe llvm::SelectionDAGISel::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51ef8fe)
#13 0x00000000051de7f1 llvm::SelectionDAGISelLegacy::runOnMachineFunction(llvm::MachineFunction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x51de7f1)
#14 0x000000000309c68d llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x309c68d)
#15 0x0000000003600752 llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3600752)
#16 0x00000000036009e1 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36009e1)
#17 0x00000000036022b7 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x36022b7)
#18 0x0000000003f499e2 clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3f499e2)
#19 0x000000000457823b clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x457823b)
#20 0x00000000061dd88c clang::ParseAST(clang::Sema&, bool, bool) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x61dd88c)
#21 0x0000000004578d95 clang::CodeGenAction::ExecuteAction() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x4578d95)
#22 0x000000000487407a clang::FrontendAction::Execute() (/opt/compiler-explorer/clang-trunk/bin/clang+++0x487407a)
#23 0x00000000047f4a5b clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x47f4a5b)
#24 0x000000000496a14b clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x496a14b)
#25 0x0000000000dca8a5 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdca8a5)
#26 0x0000000000dc27fb ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&, llvm::ToolContext const&, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>) driver.cpp:0:0
#27 0x0000000000dc289d int llvm::function_ref<int (llvm::SmallVectorImpl<char const*>&)>::callback_fn<clang_main(int, char**, llvm::ToolContext const&)::'lambda'(llvm::SmallVectorImpl<char const*>&)>(long, llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#28 0x00000000045e65f9 void llvm::function_ref<void ()>::callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const::'lambda'()>(long) Job.cpp:0:0
#29 0x0000000003bf5b83 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x3bf5b83)
#30 0x00000000045e6819 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const (.part.0) Job.cpp:0:0
#31 0x00000000045a9aa2 clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45a9aa2)
#32 0x00000000045aa981 clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45aa981)
#33 0x00000000045b34bc clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0x45b34bc)
#34 0x0000000000dc72f9 clang_main(int, char**, llvm::ToolContext const&) (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc72f9)
#35 0x0000000000c74544 main (/opt/compiler-explorer/clang-trunk/bin/clang+++0xc74544)
#36 0x000075a101029d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#37 0x000075a101029e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#38 0x0000000000dc2295 _start (/opt/compiler-explorer/clang-trunk/bin/clang+++0xdc2295)
clang++: error: clang frontend command failed with exit code 70 (use -v to see invocation)
Compiler returned: 70
```

## Comments

### Comment 1 - RKSimon

```ll
define <2 x x86_fp80> @foo(ptr %0, ptr  %1) {
entry:
  %x = load <2 x x86_fp80>, ptr %0, align 32
  %y = load <2 x x86_fp80>, ptr %1, align 32
  %elt.minimumnum = tail call <2 x x86_fp80> @llvm.minimumnum.v2f80(<2 x x86_fp80> %x, <2 x x86_fp80> %y)
  ret <2 x x86_fp80> %elt.minimumnum
}
declare <2 x x86_fp80> @llvm.minimumnum.v2f80(<2 x x86_fp80>, <2 x x86_fp80>)
```

---

