# [SPIRV] lowering select instruction over struct fails

**Author:** simeonschaub
**URL:** https://github.com/llvm/llvm-project/issues/163389
**Status:** Closed
**Labels:** duplicate, crash, backend:SPIR-V
**Closed Date:** 2026-03-09T15:30:15Z

## Body

Reproducer:

```llvm
target datalayout = "e-i64:64-v16:16-v24:32-v32:32-v48:64-v96:128-v192:256-v256:256-v512:512-v1024:1024-G1"
target triple = "spirv64-unknown-unknown-unknown"

define void @julia__24_52968() {
top:
  %0 = select i1 false, { float, i64 } zeroinitializer, { float, i64 } zeroinitializer
  ret void
}
```

Running `llc reduced.bc -filetype=obj -o reduced.spv` yields:

```
Select values must have same type as select instruction!
  %3 = select i1 %2, i32 %1, i32 %1
in function julia__24_52968
LLVM ERROR: Broken function found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Program arguments: /home/simeon/.julia/artifacts/d6a08653cd5335c4a611054885a04977c7f196c0/bin/llc reduced.bc -filetype=obj -o reduced.spv
1.      Running pass 'Function Pass Manager' on module 'reduced.bc'.
2.      Running pass 'Module Verifier' on function '@julia__24_52968'
 #0 0x0000000001dd08e8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/home/simeon/.julia/artifacts/d6a08653cd5335c4a611054885a04977c7f196c0/bin/llc+0x1dd08e8)
 #1 0x0000000001dce184 SignalHandler(int) Signals.cpp:0:0
 #2 0x00007f92d4228070 __restore_rt (/lib64/libc.so.6+0x1a070)
 #3 0x00007f92d4281e9c __pthread_kill_implementation (/lib64/libc.so.6+0x73e9c)
 #4 0x00007f92d4227f3e gsignal (/lib64/libc.so.6+0x19f3e)
 #5 0x00007f92d420f6d0 abort (/lib64/libc.so.6+0x16d0)
 #6 0x0000000000765da4 llvm::UniqueStringSaver::save(llvm::StringRef) (.cold) StringSaver.cpp:0:0
 #7 0x0000000001d26778 (/home/simeon/.julia/artifacts/d6a08653cd5335c4a611054885a04977c7f196c0/bin/llc+0x1d26778)
 #8 0x0000000001436223 (anonymous namespace)::VerifierLegacyPass::runOnFunction(llvm::Function&) Verifier.cpp:0:0
 #9 0x00000000013720c0 llvm::FPPassManager::runOnFunction(llvm::Function&) (/home/simeon/.julia/artifacts/d6a08653cd5335c4a611054885a04977c7f196c0/bin/llc+0x13720c0)
#10 0x000000000137233c llvm::FPPassManager::runOnModule(llvm::Module&) (/home/simeon/.julia/artifacts/d6a08653cd5335c4a611054885a04977c7f196c0/bin/llc+0x137233c)
#11 0x0000000001372cef llvm::legacy::PassManagerImpl::run(llvm::Module&) (/home/simeon/.julia/artifacts/d6a08653cd5335c4a611054885a04977c7f196c0/bin/llc+0x1372cef)
#12 0x0000000000842ba9 compileModule(char**, llvm::LLVMContext&) llc.cpp:0:0
#13 0x000000000077cd96 main (/home/simeon/.julia/artifacts/d6a08653cd5335c4a611054885a04977c7f196c0/bin/llc+0x77cd96)
#14 0x00007f92d4211575 __libc_start_call_main (/lib64/libc.so.6+0x3575)
#15 0x00007f92d4211628 __libc_start_main@GLIBC_2.2.5 (/lib64/libc.so.6+0x3628)
#16 0x000000000083a756 _start /workspace/srcdir/glibc-2.17/csu/../sysdeps/x86_64/start.S:126:0
```

## Comments

### Comment 1 - jmmartinez

This sucks. Emit intrinsics generates invalid LLVM-IR when it generates the calls to handle aggregate types:

```asm
; IR After SPIRV emit intrinsics

define void @julia__24_52968() {
top:
  %0 = call i32 (...) @llvm.spv.const.composite.i32()
  %1 = call i32 @llvm.spv.track.constant.i32.i32(i32 %0, metadata { float, i64 } zeroinitializer)
  call void @llvm.spv.assign.type.i32(i32 %1, metadata { float, i64 } poison)
  %2 = select i1 false, i32 %1, i32 %1
  call void @llvm.spv.value.md(metadata !0)
  call void (...) @llvm.fake.use({ float, i64 } %2)
  ret void
}
```

---

### Comment 2 - jmmartinez

Same issue as https://github.com/llvm/llvm-project/issues/151344. I'm marking this one as duplicate since the former is a bit richer. In any case, generating invalid LLVM-IR is wrong.

---

