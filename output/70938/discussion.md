# Clang crash: Assertion `materialized_use_empty() && "Uses remain when a value is destroyed!"' failed. (since clang-15)

**Author:** shao-hua-li
**URL:** https://github.com/llvm/llvm-project/issues/70938
**Status:** Closed
**Labels:** llvm:codegen, crash-on-valid
**Closed Date:** 2025-01-30T00:58:24Z

## Body

clang at -O1 crashes on the test case.

Bisected to 702d5de4380b1e1554e5b90863093c3a57f76f70, which was committed by @nikic 

Compiler explorer: https://godbolt.org/z/xrWex8Yoj

```console
% cat a.c
int a, b, c, g;
static int *d;
static int **e = &d;
char f[2][1][2];
char h, i;
void j() {
  int **k[9];
  k[g] = &d;
  for (; h;) {
    c = 0;
    for (; c <= 0; c++) {
      char *l = &f[c + 1][0][c + 1];
      if (b)
        a = 0;
      *l = 0;
      for (; i;)
        ++**e;
    }
  }
}
int main() {}
% clang -O1 a.c
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'reduced.c'.
4.      Running pass 'CodeGen Prepare' on function '@j'
 #0 0x00007fa0eda2a414 PrintStackTraceSignalHandler(void*) Signals.cpp:0:0
 #1 0x00007fa0eda27bb4 SignalHandler(int) Signals.cpp:0:0
 #2 0x00007fa0ecd91090 (/lib/x86_64-linux-gnu/libc.so.6+0x43090)
 #3 0x00007fa0ecd9100b raise /build/glibc-BHL3KM/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #4 0x00007fa0ecd70859 abort /build/glibc-BHL3KM/glibc-2.31/stdlib/abort.c:81:7
 #5 0x00007fa0ecd70729 get_sysdep_segment_value /build/glibc-BHL3KM/glibc-2.31/intl/loadmsgcat.c:509:8
 #6 0x00007fa0ecd70729 _nl_load_domain /build/glibc-BHL3KM/glibc-2.31/intl/loadmsgcat.c:970:34
 #7 0x00007fa0ecd81fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #8 0x00007fa0edc3d710 llvm::Value::~Value() (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libLLVM-15git.so+0xac1710)
 #9 0x00007fa0edc3d782 llvm::Value::deleteValue() (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libLLVM-15git.so+0xac1782)
#10 0x00007fa0edd3800e (anonymous namespace)::CodeGenPrepare::runOnFunction(llvm::Function&) (.part.0) CodeGenPrepare.cpp:0:0
#11 0x00007fa0edbc0df5 llvm::FPPassManager::runOnFunction(llvm::Function&) (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libLLVM-15git.so+0xa44df5)
#12 0x00007fa0edbc1039 llvm::FPPassManager::runOnModule(llvm::Module&) (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libLLVM-15git.so+0xa45039)
#13 0x00007fa0edbc1882 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libLLVM-15git.so+0xa45882)
#14 0x00007fa0f36b879a clang::EmitBackendOutput(clang::DiagnosticsEngine&, clang::HeaderSearchOptions const&, clang::CodeGenOptions const&, clang::TargetOptions const&, clang::LangOptions const&, llvm::StringRef, llvm::Module*, clang::BackendAction, std::unique_ptr<llvm::raw_pwrite_stream, std::default_delete<llvm::raw_pwrite_stream> >) (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libclang-cpp.so.15git+0x1ccc79a)
#15 0x00007fa0f3acf723 clang::BackendConsumer::HandleTranslationUnit(clang::ASTContext&) (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libclang-cpp.so.15git+0x20e3723)
#16 0x00007fa0f23e0ad9 clang::ParseAST(clang::Sema&, bool, bool) (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libclang-cpp.so.15git+0x9f4ad9)
#17 0x00007fa0f3ace0c8 clang::CodeGenAction::ExecuteAction() (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libclang-cpp.so.15git+0x20e20c8)
#18 0x00007fa0f4608dc9 clang::FrontendAction::Execute() (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libclang-cpp.so.15git+0x2c1cdc9)
#19 0x00007fa0f458f0ee clang::CompilerInstance::ExecuteAction(clang::FrontendAction&) (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libclang-cpp.so.15git+0x2ba30ee)
#20 0x00007fa0f4698a73 clang::ExecuteCompilerInvocation(clang::CompilerInstance*) (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/../lib/libclang-cpp.so.15git+0x2caca73)
#21 0x00005601bc01e182 cc1_main(llvm::ArrayRef<char const*>, char const*, void*) (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/clang-15+0x15182)
#22 0x00005601bc01a7b8 ExecuteCC1Tool(llvm::SmallVectorImpl<char const*>&) driver.cpp:0:0
#23 0x00005601bc017381 main (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/clang-15+0xe381)
#24 0x00007fa0ecd72083 __libc_start_main /build/glibc-BHL3KM/glibc-2.31/csu/../csu/libc-start.c:342:3
#25 0x00005601bc01a32e _start (/zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin/clang-15+0x1132e)
clang-15: error: unable to execute command: Aborted
clang-15: error: clang frontend command failed due to signal (use -v to see invocation)
clang version 15.0.0 (https://github.com/llvm/llvm-project.git 702d5de4380b1e1554e5b90863093c3a57f76f70)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /zdata/shaoli/compilers/ccbuilder-compilers/clang-702d5de4380b1e1554e5b90863093c3a57f76f70/bin
clang-15: note: diagnostic msg:
********************

PLEASE ATTACH THE FOLLOWING FILES TO THE BUG REPORT:
Preprocessed source(s) and associated run script(s) are located at:
clang-15: note: diagnostic msg: /tmp/reduced-8597d0.c
clang-15: note: diagnostic msg: /tmp/reduced-8597d0.sh
clang-15: note: diagnostic msg:

********************
%
```

## Comments

### Comment 1 - dtcxzyw

Reduced test case: https://godbolt.org/z/YzvK8saxn

---

