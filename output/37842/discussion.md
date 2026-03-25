# clang crashes at -O3 -mllvm -polly -mllvm -polly-parallel

**Author:** llvmbot
**URL:** https://github.com/llvm/llvm-project/issues/37842
**Status:** Closed
**Labels:** bugzilla, llvm:codegen, crash
**Closed Date:** 2026-01-13T15:40:39Z

## Body

|  |  |
| --- | --- |
| Bugzilla Link | [38494](https://llvm.org/bz38494) |
| Version | trunk |
| OS | Linux |
| Reporter | LLVM Bugzilla Contributor |

## Extended Description 
The following is a test program.

------------ foo.c ------------
```
#define NI (32)
#define NJ (32)
#define NK (32)
#define NA (32)

void test(double r[NI][NJ][NK], double s[NI][NJ][NK],
          int II, int JJ, int KK, int A) {
  for (int i = 0; i < II; ++i)
    for (int j = 0; j < JJ; ++j)
      for (int k = 0; k < KK; ++k)
              r[i][j][k] = (s[i][j][k]) * i - j * k + A;
}
```
-------------------------------

```
$ clang -O3 -mllvm -polly -mllvm -polly-parallel foo.c
...
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'foo.c'.
4.      Running pass 'Machine Common Subexpression Elimination' on function '@test_polly_subfn'
#&#8203;0 0x000000000177cfda llvm::sys::PrintStackTrace(llvm::raw_ostream&) (/opt/llvm/svn-trunk/install/bin/clang-8+0x177cfda)
#&#8203;1 0x000000000177b7b9 llvm::sys::RunSignalHandlers() (/opt/llvm/svn-trunk/install/bin/clang-8+0x177b7b9)
#&#8203;2 0x000000000177b8ed SignalHandler(int) (/opt/llvm/svn-trunk/install/bin/clang-8+0x177b8ed)
#&#8203;3 0x00007f33e50c55e0 __restore_rt (/lib64/libpthread.so.0+0xf5e0)
#&#8203;4 0x00000000011e27b6 (anonymous namespace)::MachineCSE::PerformCSE(llvm::DomTreeNodeBase<llvm::MachineBasicBlock>*) (/opt/llvm/svn-trunk/install/bin/clang-8+0x11e27b6)
#&#8203;5 0x00000000010a3397 llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/opt/llvm/svn-trunk/install/bin/clang-8+0x10a3397)
#&#8203;6 0x0000000001375d2f llvm::FPPassManager::runOnFunction(llvm::Function&) (/opt/llvm/svn-trunk/install/bin/clang-8+0x1375d2f)
#&#8203;7 0x0000000001375f13 llvm::FPPassManager::runOnModule(llvm::Module&) (/opt/llvm/svn-trunk/install/bin/clang-8+0x1375f13)
#&#8203;8 0x00000000013765bf llvm::legacy::PassManagerImpl::run(llvm::Module&) (/opt/llvm/svn-trunk/install/bin/clang-8+0x13765bf)
#&#8203;9 0x000000000191f4ff (anonymous namespace)::EmitAssemblyHelper::EmitAssembly(clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >) (/opt/llvm/svn-trunk/install/bin/clang-8+0x191f4ff)
#&#8203;10 0x0000000001920e0b clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::DataLayout const&, llvm::Module*, clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >) (/opt/llvm/svn-trunk/install/bin/clang-8+0x1920e0b)
#&#8203;11 0x00000000020fe676 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/opt/llvm/svn-trunk/install/bin/clang-8+0x20fe676)
#&#8203;12 0x00000000025b651a clang::ParseAST(clang::Sema&, bool, bool) (/opt/llvm/svn-trunk/install/bin/clang-8+0x25b651a)
#&#8203;13 0x00000000020fdea4 clang::CodeGenAction::ExecuteAction() (/opt/llvm/svn-trunk/install/bin/clang-8+0x20fdea4)
#&#8203;14 0x0000000001d0fede clang::FrontendAction::Execute() (/opt/llvm/svn-trunk/install/bin/clang-8+0x1d0fede)
#&#8203;15 0x0000000001ce5c9d clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/opt/llvm/svn-trunk/install/bin/clang-8+0x1ce5c9d)
#&#8203;16 0x0000000001da7a61 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/opt/llvm/svn-trunk/install/bin/clang-8+0x1da7a61)
#&#8203;17 0x0000000000abf0c8 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/opt/llvm/svn-trunk/install/bin/clang-8+0xabf0c8)
#&#8203;18 0x0000000000a4ab69 main (/opt/llvm/svn-trunk/install/bin/clang-8+0xa4ab69)
#&#8203;19 0x00007f33e3ca9c05 __libc_start_main /usr/src/debug/glibc-2.17-c758a686/csu/../csu/libc-start.c:308:0
#&#8203;20 0x0000000000abb687 _start (/opt/llvm/svn-trunk/install/bin/clang-8+0xabb687)
clang-8: error: unable to execute command: Segmentation fault (core dumped)
clang-8: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 8.0.0 (trunk 339303)
Target: x86_64-unknown-linux-gnu
Thread model: posix
```

## Comments

### Comment 1 - foxtran

The issue has been fixed between LLVM 12 and LLVM 13: https://godbolt.org/z/zcMTjKsGM

@EugeneZelenko, whom should I ping for closing such issues?

---

