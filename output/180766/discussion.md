# [CIR] CodeGen crash on EHScopeStack::find

**Author:** bruteforceboy
**URL:** https://github.com/llvm/llvm-project/issues/180766
**Status:** Closed
**Labels:** crash, ClangIR
**Closed Date:** 2026-03-14T14:33:49Z

## Body

The following code snippet crashes
```cpp
struct S {
  int a;
  S(int a) : a(a) {}
  ~S();
};

void foo() {
  {
    S s(1);
  }
  S s(1);
}
```
with (truncated)
```
clang++: ~/llvm-project/clang/lib/CIR/CodeGen/CIRGenCleanup.h:319: clang::CIRGen::EHScopeStack::iterator clang::CIRGen::EHScopeStack::find(stable_iterator) const: Assertion `savePoint.size <= stable_begin().size && "finding savepoint after pop"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: bin/clang++ test.cpp -Xclang -fclangir -Xclang -emit-cir -S -o -
1.	<eof> parser at end of file
 #0 0x000062b98ef2dc70 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (bin/clang+++0x2bb8c70)
 #1 0x000062b98ef2b354 llvm::sys::CleanupOnSignal(unsigned long) (bin/clang+++0x2bb6354)
 #2 0x000062b98ee5fd38 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x0000727551245330 (/lib/x86_64-linux-gnu/libc.so.6+0x45330)
 #4 0x000072755129eb2c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #5 0x000072755129eb2c __pthread_kill_internal ./nptl/pthread_kill.c:78:10
 #6 0x000072755129eb2c pthread_kill ./nptl/pthread_kill.c:89:10
 #7 0x000072755124527e raise ./signal/../sysdeps/posix/raise.c:27:6
 #8 0x00007275512288ff abort ./stdlib/abort.c:81:7
 #9 0x000072755122881b _nl_load_domain ./intl/loadmsgcat.c:1177:9
#10 0x000072755123b517 (/lib/x86_64-linux-gnu/libc.so.6+0x3b517)
#11 0x000062b99011d6a2 (bin/clang+++0x3da86a2)
#12 0x000062b99010e9d8 clang::CIRGen::CIRGenFunction::emitCall(clang::CIRGen::CIRGenFunctionInfo const&, clang::CIRGen::CIRGenCallee const&, clang::CIRGen::ReturnValueSlot, clang::CIRGen::CallArgList const&, cir::CIRCallOpInterface*, mlir::Location) (bin/clang+++0x3d999d8)
#13 0x000062b99011a00e clang::CIRGen::CIRGenFunction::emitCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CXXCtorType, bool, bool, clang::CIRGen::Address, clang::CIRGen::CallArgList&, clang::SourceLocation) (bin/clang+++0x3da500e)
#14 0x000062b99011a34d clang::CIRGen::CIRGenFunction::emitCXXConstructorCall(clang::CXXConstructorDecl const*, clang::CXXCtorType, bool, bool, clang::CIRGen::AggValueSlot, clang::CXXConstructExpr const*) (bin/clang+++0x3da534d)
```
Command to reproduce `bin/clang++ test.cpp -Xclang -fclangir -Xclang -emit-cir -S -o -`

## Comments

### Comment 1 - woruyu

I would like to fix it!

---

### Comment 2 - Ayush3941

Hmm i am not able to reproduce crash  locally at current main

<img width="1366" height="768" alt="Image" src="https://github.com/user-attachments/assets/4e7e64a8-2f8d-41ec-96aa-b7db58d3751b" />

i think it was fixed in #179137


---

### Comment 3 - bruteforceboy

> Hmm i am not able to reproduce crash locally at current main
> <img alt="Image" width="1366" height="768" src="https://private-user-images.githubusercontent.com/132849148/560304883-4e7e64a8-2f8d-41ec-96aa-b7db58d3751b.png?jwt=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3NzM0OTg4NzksIm5iZiI6MTc3MzQ5ODU3OSwicGF0aCI6Ii8xMzI4NDkxNDgvNTYwMzA0ODgzLTRlN2U2NGE4LTJmOGQtNDFlYy05NmFhLWI3ZGI1OGQzNzUxYi5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjYwMzE0JTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI2MDMxNFQxNDI5MzlaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1jNjhiMjU5MzU4ZWE3ZWEzZDQzZDE4MzNmNzlmYTQxNzJmMjJjYzYyNzYyNjZmMjVmYTMyMTI3NGU5ZDQ4YzM2JlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCJ9.SowstdRmPz1CK0_pmcJjOI6e8wUwu0TJDDi9MzYMHCQ">
> 
> i think it was fixed in [#179137](https://github.com/llvm/llvm-project/pull/179137)

okay, thanks. closing this.

---

