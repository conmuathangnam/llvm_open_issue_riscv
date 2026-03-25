# [MergeFunc] Creates calls for calling convention that does not allow calls

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/173355
**Status:** Closed
**Labels:** ipo, crash
**Closed Date:** 2026-01-04T12:35:18Z

## Body

When running opt on the following IR, opt crashes. 
I am not sure whether this is a valid case. If using opt in this way is not appropriate, I am open to closing this issue.
godbolt: https://godbolt.org/z/873edWGzK

llvm version 50ae726bb3498

test.ll

```
define amdgpu_kernel void @hamming_search_inner_loop1() {
entry:
  br label %end

end:                                              ; preds = %entry
  ret void
}

define amdgpu_kernel void @hamming_search_inner_loop2() {
entry:
  br label %end

end:                                              ; preds = %entry
  ret void
}

```


commads
./bin/opt test.ll -passes=mergefunc -S

stacktrace

```
calling convention does not permit calls
  tail call amdgpu_kernel void @hamming_search_inner_loop1()
LLVM ERROR: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/opt -o /app/output.s -S -passes=mergefunc -S <source>
1.	Running pass "verify" on module "<source>"
 #0 0x0000000005a44d98 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5a44d98)
 #1 0x0000000005a41c44 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007bd9c3c42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #3 0x00007bd9c3c969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #4 0x00007bd9c3c42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #5 0x00007bd9c3c287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #6 0x00000000008339a1 llvm::json::operator==(llvm::json::Value const&, llvm::json::Value const&) (.cold) JSON.cpp:0:0
 #7 0x0000000005979301 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5979301)
 #8 0x0000000005862a88 (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x5862a88)
 #9 0x000000000097d40e llvm::detail::PassModel<llvm::Module, llvm::VerifierPass, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97d40e)
#10 0x000000000581f0b1 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x581f0b1)
#11 0x000000000098776a llvm::runPassPipeline(llvm::StringRef, llvm::Module&, llvm::TargetMachine*, llvm::TargetLibraryInfoImpl*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::ToolOutputFile*, llvm::StringRef, llvm::ArrayRef<llvm::PassPlugin>, llvm::ArrayRef<std::function<void (llvm::PassBuilder&)>>, llvm::opt_tool::OutputKind, llvm::opt_tool::VerifierKind, bool, bool, bool, bool, bool, bool, bool, bool) (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x98776a)
#12 0x000000000097b5e8 optMain (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x97b5e8)
#13 0x00007bd9c3c29d90 (/lib/x86_64-linux-gnu/libc.so.6+0x29d90)
#14 0x00007bd9c3c29e40 __libc_start_main (/lib/x86_64-linux-gnu/libc.so.6+0x29e40)
#15 0x0000000000972085 _start (/opt/compiler-explorer/clang-assertions-trunk/bin/opt+0x972085)
Program terminated with signal: SIGSEGV
Compiler returned: 139
```


## Comments

### Comment 1 - nikic

This is basically the same as https://github.com/llvm/llvm-project/issues/39579, though this one produces a verifier error.

---

### Comment 2 - Flakebi

I opened a PR to fix both of these issues: #174254

---

