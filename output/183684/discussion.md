# Crash when compiling precompiled module to object file

**Author:** The-Mighty-Cat
**URL:** https://github.com/llvm/llvm-project/issues/183684
**Status:** Closed
**Labels:** crash, llvm
**Closed Date:** 2026-02-27T05:08:38Z

## Body

Stacktrace:
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: clang++ -Wno-unused-command-line-argument --target=x86_64-pc-linux-gnu -std=c++26 -D CRAFTER_BUILD_CONFIGURATION_TYPE_EXECUTABLE -D CRAFTER_BUILD_CONFIGURATION_TARGET_x86_64_pc_linux_gnu -g -D CRAFTER_BUILD_CONFIGURATION_DEBUG -fprebuilt-module-path=/home/jorijn/repos/3DForts/build/executable-debug -fprebuilt-module-path=/home/jorijn/repos/Crafter/Crafter.Build/bin/x86_64-pc-linux-gnu /home/jorijn/repos/3DForts/build/executable-debug/Forts3D-Physics.pcm -c -o /home/jorijn/repos/3DForts/build/executable-debug/Forts3D-Physics.o
1.      <eof> parser at end of file
2.      Code generation
 #0 0x00007fde651081ed llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:834:22
 #1 0x00007fde65105cc7 llvm::sys::RunSignalHandlers() /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Signals.cpp:104:20
 #2 0x00007fde65105cc7 llvm::sys::CleanupOnSignal(unsigned long) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/Unix/Signals.inc:373:31
 #3 0x00007fde64fda6b9 HandleCrash /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/CrashRecoveryContext.cpp:73:5
 #4 0x00007fde64fda6b9 CrashRecoverySignalHandler /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/CrashRecoveryContext.cpp:390:62
 #5 0x00007fde6424d2d0 (/usr/lib/libc.so.6+0x3e2d0)
 #6 0x00007fde65395e51 stripPointerCastsAndOffsets<(<unnamed>::PointerStripKind)0> /usr/src/debug/llvm/llvm-21.1.8.src/lib/IR/Value.cpp:637:3
 #7 0x00007fde65f21026 llvm::GlobalValue::classof(llvm::Value const*) /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/IR/GlobalValue.h:675:53
 #8 0x00007fde65f21026 llvm::isa_impl<llvm::GlobalValue, llvm::Constant, void>::doit(llvm::Constant const&) /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/Support/Casting.h:64:64
 #9 0x00007fde65f21026 llvm::isa_impl_cl<llvm::GlobalValue, llvm::Constant const*>::doit(llvm::Constant const*) /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/Support/Casting.h:110:36
#10 0x00007fde65f21026 llvm::isa_impl_wrap<llvm::GlobalValue, llvm::Constant const*, llvm::Constant const*>::doit(llvm::Constant const* const&) /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/Support/Casting.h:137:41
#11 0x00007fde65f21026 llvm::isa_impl_wrap<llvm::GlobalValue, llvm::Constant* const, llvm::Constant const*>::doit(llvm::Constant* const&) /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/Support/Casting.h:129:13
#12 0x00007fde65f21026 llvm::CastIsPossible<llvm::GlobalValue, llvm::Constant*, void>::isPossible(llvm::Constant* const&) /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/Support/Casting.h:257:62
#13 0x00007fde65f21026 llvm::CastInfo<llvm::GlobalValue, llvm::Constant*, void>::doCastIfPossible(llvm::Constant* const&) /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/Support/Casting.h:493:26
#14 0x00007fde65f21026 decltype(auto) llvm::dyn_cast<llvm::GlobalValue, llvm::Constant>(llvm::Constant*) /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/Support/Casting.h:663:48
#15 0x00007fde65f21026 llvm::AsmPrinter::preprocessXXStructorList(llvm::DataLayout const&, llvm::Constant const*, llvm::SmallVector<llvm::AsmPrinter::Structor, 8u>&) /usr/src/debug/llvm/llvm-21.1.8.src/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:3281:32
#16 0x00007fde65f35d65 llvm::SmallVectorBase<unsigned int>::empty() const /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/ADT/SmallVector.h:82:46
#17 0x00007fde65f35d65 llvm::AsmPrinter::emitXXStructorList(llvm::DataLayout const&, llvm::Constant const*, bool) /usr/src/debug/llvm/llvm-21.1.8.src/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:3297:22
#18 0x00007fde65f3c96b llvm::AsmPrinter::emitSpecialLLVMGlobal(llvm::GlobalVariable const*) /usr/src/debug/llvm/llvm-21.1.8.src/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:3230:23
#19 0x00007fde65f3cea3 llvm::AsmPrinter::emitGlobalVariable(llvm::GlobalVariable const*) /usr/src/debug/llvm/llvm-21.1.8.src/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:740:5
#20 0x00007fde65f2e7c8 llvm::ilist_detail::node_base_prevnext<llvm::ilist_node_base<false, void>, false>::getNext() const /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/ADT/ilist_node_base.h:28:38
#21 0x00007fde65f2e7c8 llvm::ilist_node_impl<llvm::ilist_detail::node_options<llvm::GlobalVariable, false, false, void, false, void>>::getNext() /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/ADT/ilist_node.h:119:66
#22 0x00007fde65f2e7c8 llvm::ilist_iterator<llvm::ilist_detail::node_options<llvm::GlobalVariable, false, false, void, false, void>, false, false>::operator++() /usr/src/debug/llvm/llvm-21.1.8.src/include/llvm/ADT/ilist_iterator.h:187:25
#23 0x00007fde65f2e7c8 llvm::AsmPrinter::doFinalization(llvm::Module&) /usr/src/debug/llvm/llvm-21.1.8.src/lib/CodeGen/AsmPrinter/AsmPrinter.cpp:2539:34
#24 0x00007fde652f91cd llvm::FPPassManager::doFinalization(llvm::Module&) /usr/src/debug/llvm/llvm-21.1.8.src/lib/IR/LegacyPassManager.cpp:1462:13
#25 0x00007fde65304539 runOnModule /usr/src/debug/llvm/llvm-21.1.8.src/lib/IR/LegacyPassManager.cpp:1549:13
#26 0x00007fde65304539 llvm::legacy::PassManagerImpl::run(llvm::Module&) /usr/src/debug/llvm/llvm-21.1.8.src/lib/IR/LegacyPassManager.cpp:531:55
#27 0x00007fde6fc95c6f RunCodegenPipeline /usr/src/debug/clang/clang-21.1.8.src/lib/CodeGen/BackendUtil.cpp:1261:9
#28 0x00007fde6fc95c6f emitAssembly /usr/src/debug/clang/clang-21.1.8.src/lib/CodeGen/BackendUtil.cpp:1284:21
#29 0x00007fde6fc95c6f clang::emitBackendOutput(clang::CompilerInstance&, clang::CodeGenOptions&, llvm::StringRef, llvm::Module*, clang::BackendAction, llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream>>, clang::BackendConsumer*) /usr/src/debug/clang/clang-21.1.8.src/lib/CodeGen/BackendUtil.cpp:1449:25
#30 0x00007fde6fff388d llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::release() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:232:9
#31 0x00007fde6fff388d llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>::~IntrusiveRefCntPtr() /usr/include/llvm/ADT/IntrusiveRefCntPtr.h:196:34
#32 0x00007fde6fff388d clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) /usr/src/debug/clang/clang-21.1.8.src/lib/CodeGen/CodeGenAction.cpp:316:20
#33 0x00007fde6e771736 __gnu_cxx::__normal_iterator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>*, std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>::__normal_iterator(std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>* const&) /usr/include/c++/15.2.1/bits/stl_iterator.h:1059:9
#34 0x00007fde6e771736 std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>::begin() /usr/include/c++/15.2.1/bits/stl_vector.h:999:16
#35 0x00007fde6e771736 void clang::finalize<std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>>(std::vector<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>, std::allocator<std::unique_ptr<clang::TemplateInstantiationCallback, std::default_delete<clang::TemplateInstantiationCallback>>>>&, clang::Sema const&) /usr/src/debug/clang/clang-21.1.8.src/include/clang/Sema/TemplateInstCallback.h:54:3
#36 0x00007fde6e771736 clang::ParseAST(clang::Sema&, bool, bool) /usr/src/debug/clang/clang-21.1.8.src/lib/Parse/ParseAST.cpp:190:11
#37 0x00007fde7084ea9c clang::CompilerInstance::shouldBuildGlobalModuleIndex() const /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:86:58
#38 0x00007fde7084ea9c clang::FrontendAction::Execute() /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/FrontendAction.cpp:1225:38
#39 0x00007fde707aff0f llvm::Error::getPtr() const /usr/include/llvm/Support/Error.h:281:12
#40 0x00007fde707aff0f llvm::Error::operator bool() /usr/include/llvm/Support/Error.h:241:22
#41 0x00007fde707aff0f clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) /usr/src/debug/clang/clang-21.1.8.src/lib/Frontend/CompilerInstance.cpp:1055:42
#42 0x00007fde708abcec std::__shared_ptr<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2>::get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1673:16
#43 0x00007fde708abcec std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::_M_get() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1370:69
#44 0x00007fde708abcec std::__shared_ptr_access<clang::FrontendOptions, (__gnu_cxx::_Lock_policy)2, false, false>::operator*() const /usr/include/c++/15.2.1/bits/shared_ptr_base.h:1358:40
#45 0x00007fde708abcec clang::CompilerInvocation::getFrontendOpts() /usr/src/debug/clang/clang-21.1.8.src/include/clang/Frontend/CompilerInvocation.h:259:48
#46 0x00007fde708abcec clang::CompilerInstance::getFrontendOpts() /usr/src/debug/clang/clang-21.1.8.src/include/clang/Frontend/CompilerInstance.h:304:39
#47 0x00007fde708abcec clang::ExecuteCompilerInvocation(clang::CompilerInstance*) /usr/src/debug/clang/clang-21.1.8.src/lib/FrontendTool/ExecuteCompilerInvocation.cpp:300:29
#48 0x0000556084a26e9a cc1_main(llvm::ArrayRef<char const*>, char const*, void*) /usr/src/debug/clang/clang-21.1.8.src/tools/driver/cc1_main.cpp:297:40
#49 0x0000556084a2ab61 ExecuteCC1Tool /usr/src/debug/clang/clang-21.1.8.src/tools/driver/driver.cpp:223:20
#50 0x00007fde703f3295 operator() /usr/src/debug/clang/clang-21.1.8.src/lib/Driver/Job.cpp:436:32
#51 0x00007fde703f3295 callback_fn<clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef> >, std::string*, bool*) const::<lambda()> > /usr/include/llvm/ADT/STLFunctionalExtras.h:46:52
#52 0x00007fde64fdaaf8 llvm::CrashRecoveryContext::RunSafely(llvm::function_ref<void ()>) /usr/src/debug/llvm/llvm-21.1.8.src/lib/Support/CrashRecoveryContext.cpp:428:1
#53 0x00007fde703fee35 clang::driver::CC1Command::Execute(llvm::ArrayRef<std::optional<llvm::StringRef>>, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*, bool*) const /usr/src/debug/clang/clang-21.1.8.src/lib/Driver/Job.cpp:440:10
#54 0x00007fde7041228b clang::driver::Compilation::ExecuteCommand(clang::driver::Command const&, clang::driver::Command const*&, bool) const /usr/src/debug/clang/clang-21.1.8.src/lib/Driver/Compilation.cpp:196:22
#55 0x00007fde7041268c clang::driver::Compilation::ExecuteJobs(clang::driver::JobList const&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&, bool) const /usr/src/debug/clang/clang-21.1.8.src/lib/Driver/Compilation.cpp:251:5
#56 0x00007fde70424e65 llvm::SmallVectorBase<unsigned int>::empty() const /usr/include/llvm/ADT/SmallVector.h:82:46
#57 0x00007fde70424e65 clang::driver::Driver::ExecuteCompilation(clang::driver::Compilation&, llvm::SmallVectorImpl<std::pair<int, clang::driver::Command const*>>&) /usr/src/debug/clang/clang-21.1.8.src/lib/Driver/Driver.cpp:2246:28
#58 0x0000556084a2cb85 llvm::SmallVectorBase<unsigned int>::size() const /usr/include/llvm/ADT/SmallVector.h:79:32
#59 0x0000556084a2cb85 llvm::SmallVectorTemplateCommon<std::pair<int, clang::driver::Command const*>, void>::end() /usr/include/llvm/ADT/SmallVector.h:270:41
#60 0x0000556084a2cb85 clang_main(int, char**, llvm::ToolContext const&) /usr/src/debug/clang/clang-21.1.8.src/tools/driver/driver.cpp:408:26
#61 0x0000556084a1e2b0 main /usr/src/debug/clang/clang-21.1.8.src/build/tools/driver/clang-driver.cpp:18:1
#62 0x00007fde642366c1 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:83:3
#63 0x00007fde642367f9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#64 0x00007fde642367f9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#65 0x0000556084a1e315 _start (/usr/bin/clang+++0xb315)
clang++: error: clang frontend command failed with exit code 139 (use -v to see invocation)
clang version 21.1.8
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
clang++: note: diagnostic msg: Error generating preprocessed source(s) - no preprocessable inputs.
```

File:
```cpp
module;
#include <vulkan/vulkan.h>
export module Forts3D:Physics;
import Crafter.Graphics;
import std;
using namespace Crafter;

export namespace Forts3D {
    class Node;
    class Physics {
        public:
        inline static std::vector<Node*> nodes;
        static void Update(WindowVulkan& window, VkCommandBuffer cmd);
    };
}
```

The run script up until the crash has been attached because it was too long to include in the body

[run_script.sh](https://github.com/user-attachments/files/25594873/run_script.sh)

## Comments

### Comment 1 - EugeneZelenko

Could you please try 22 or `main` branch? Only most recent release is maintained.

---

### Comment 2 - The-Mighty-Cat

I downloaded the latest version 22.1.0 and that seems to have resolved it, thank you and sorry for holding up your time.

---

