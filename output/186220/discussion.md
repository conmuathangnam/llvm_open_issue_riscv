# Crash in verifier llvm-as

**Author:** jasoncarr0
**URL:** https://github.com/llvm/llvm-project/issues/186220
**Status:** Open
**Labels:** tools:llvm-as, crash

## Body

Version is 21.1.0-rc3 (but also on version 20)
Although some versions seemed to work, the most suspicious recent changes are:
- a [16 x i16] constant is linked as extern uint16_t[] in some C code (pre-compiled with Clang)
- a [16 x ptr] constant is linked as extern **uint64_t[], each element is a pointer to some constant (private) arrays
- llvm.experimental.gc.statepoint.p0 has a variety of ids ,but uses i64 0 most of the time, whereas before we only ever used 0

I believe these work without calling opt, but it may be that opt breaks them.

```
0.	Program arguments: /nix/store/fshzdpaamz1xhakii997bchi0qs89wjz-llvm-21.1.0-rc3/bin/llvm-as test/programs/llvm/nucleic.ll -o test/programs/llvm/nucleic.bc
 #0 0x00007fffeeb41b6b llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/z5ysfna7cr35jy7w6v3fmm8j43rxpxch-llvm-21.1.0-rc3-lib/lib/libLLVM.so.21.1-rc3+0xd41b6b)
 #1 0x00007fffeeb3e9fa SignalHandler(int, siginfo_t*, void*) (/nix/store/z5ysfna7cr35jy7w6v3fmm8j43rxpxch-llvm-21.1.0-rc3-lib/lib/libLLVM.so.21.1-rc3+0xd3e9fa)
 #2 0x00007fffed6419c0 __restore_rt (/nix/store/8p33is69mjdw3bi1wmi8v2zpsxir8nwd-glibc-2.40-66/lib/libc.so.6+0x419c0)
 #3 0x00007fffeedf71b8 (anonymous namespace)::Verifier::visitIntrinsicCall(unsigned int, llvm::CallBase&) (/nix/store/z5ysfna7cr35jy7w6v3fmm8j43rxpxch-llvm-21.1.0-rc3-lib/lib/libLLVM.so.21.1-rc3+0xff71b8)
 #4 0x00007fffeee08930 (anonymous namespace)::Verifier::visitCallBase(llvm::CallBase&) (/nix/store/z5ysfna7cr35jy7w6v3fmm8j43rxpxch-llvm-21.1.0-rc3-lib/lib/libLLVM.so.21.1-rc3+0x1008930)
 #5 0x00007fffeee08ed2 (anonymous namespace)::Verifier::visitCallInst(llvm::CallInst&) (/nix/store/z5ysfna7cr35jy7w6v3fmm8j43rxpxch-llvm-21.1.0-rc3-lib/lib/libLLVM.so.21.1-rc3+0x1008ed2)
 #6 0x00007fffeee0c0bb llvm::InstVisitor<(anonymous namespace)::Verifier, void>::visit(llvm::Function&) (.constprop.0) (/nix/store/z5ysfna7cr35jy7w6v3fmm8j43rxpxch-llvm-21.1.0-rc3-lib/lib/libLLVM.so.21.1-rc3+0x100c0bb)
 #7 0x00007fffeee0d891 (anonymous namespace)::Verifier::verify(llvm::Function const&) (.part.0) (/nix/store/z5ysfna7cr35jy7w6v3fmm8j43rxpxch-llvm-21.1.0-rc3-lib/lib/libLLVM.so.21.1-rc3+0x100d891)
 #8 0x00007fffeee0f697 llvm::verifyModule(llvm::Module const&, llvm::raw_ostream*, bool*) (/nix/store/z5ysfna7cr35jy7w6v3fmm8j43rxpxch-llvm-21.1.0-rc3-lib/lib/libLLVM.so.21.1-rc3+0x100f697)
 #9 0x0000000000404cb7 main (/nix/store/fshzdpaamz1xhakii997bchi0qs89wjz-llvm-21.1.0-rc3/bin/llvm-as+0x404cb7)
#10 0x00007fffed62a4d8 __libc_start_call_main (/nix/store/8p33is69mjdw3bi1wmi8v2zpsxir8nwd-glibc-2.40-66/lib/libc.so.6+0x2a4d8)
#11 0x00007fffed62a59b __libc_start_main@GLIBC_2.2.5 (/nix/store/8p33is69mjdw3bi1wmi8v2zpsxir8nwd-glibc-2.40-66/lib/libc.so.6+0x2a59b)
#12 0x0000000000404fe5 _start (/nix/store/fshzdpaamz1xhakii997bchi0qs89wjz-llvm-21.1.0-rc3/bin/llvm-as+0x404fe5)
'/nix/store/fshzdpaamz1xhakii997bchi0qs89wjz-llvm-21.1.0-rc3/bin/llvm-as test/programs/llvm/nucleic.ll -o test/programs/llvm/nucleic.bc' failed
```

## Comments

### Comment 1 - EugeneZelenko

Could you please try 22 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

---

### Comment 2 - jasoncarr0

> Could you please try 22 or `main` branch? https://godbolt.org should be helpful. Only most recent release is maintained.

Same issue on 22
```
0.	Program arguments: /nix/store/s97jfrljxld3l91npal084xlccak1irj-llvm-22.1.0-rc3/bin/llvm-as test/programs/llvm/nucleic.ll -o test/programs/llvm/nucleic.bc
 #0 0x00007fffee18d788 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/ky89jm2h1dw3dpzcskh15cg4gps96xjr-llvm-22.1.0-rc3-lib/lib/libLLVM.so.22.1-rc3+0xd8d788)
 #1 0x00007fffee18a527 SignalHandler(int, siginfo_t*, void*) (/nix/store/ky89jm2h1dw3dpzcskh15cg4gps96xjr-llvm-22.1.0-rc3-lib/lib/libLLVM.so.22.1-rc3+0xd8a527)
 #2 0x00007fffecc42790 __restore_rt (/nix/store/l0l2ll1lmylczj1ihqn351af2kyp5x19-glibc-2.42-51/lib/libc.so.6+0x42790)
 #3 0x00007fffee433b43 (anonymous namespace)::Verifier::visitIntrinsicCall(unsigned int, llvm::CallBase&) (/nix/store/ky89jm2h1dw3dpzcskh15cg4gps96xjr-llvm-22.1.0-rc3-lib/lib/libLLVM.so.22.1-rc3+0x1033b43)
 #4 0x00007fffee442993 (anonymous namespace)::Verifier::visitCallBase(llvm::CallBase&) (/nix/store/ky89jm2h1dw3dpzcskh15cg4gps96xjr-llvm-22.1.0-rc3-lib/lib/libLLVM.so.22.1-rc3+0x1042993)
 #5 0x00007fffee442c82 (anonymous namespace)::Verifier::visitCallInst(llvm::CallInst&) (/nix/store/ky89jm2h1dw3dpzcskh15cg4gps96xjr-llvm-22.1.0-rc3-lib/lib/libLLVM.so.22.1-rc3+0x1042c82)
 #6 0x00007fffee446722 llvm::InstVisitor<(anonymous namespace)::Verifier, void>::visit(llvm::Function&) (.constprop.0) (/nix/store/ky89jm2h1dw3dpzcskh15cg4gps96xjr-llvm-22.1.0-rc3-lib/lib/libLLVM.so.22.1-rc3+0x1046722)
 #7 0x00007fffee448354 (anonymous namespace)::Verifier::verify(llvm::Function const&) (/nix/store/ky89jm2h1dw3dpzcskh15cg4gps96xjr-llvm-22.1.0-rc3-lib/lib/libLLVM.so.22.1-rc3+0x1048354)
 #8 0x00007fffee449893 llvm::verifyModule(llvm::Module const&, llvm::raw_ostream*, bool*) (/nix/store/ky89jm2h1dw3dpzcskh15cg4gps96xjr-llvm-22.1.0-rc3-lib/lib/libLLVM.so.22.1-rc3+0x1049893)
 #9 0x0000555555558a17 main (/nix/store/s97jfrljxld3l91npal084xlccak1irj-llvm-22.1.0-rc3/bin/llvm-as+0x4a17)
#10 0x00007fffecc2b285 __libc_start_call_main (/nix/store/l0l2ll1lmylczj1ihqn351af2kyp5x19-glibc-2.42-51/lib/libc.so.6+0x2b285)
#11 0x00007fffecc2b338 __libc_start_main@GLIBC_2.2.5 (/nix/store/l0l2ll1lmylczj1ihqn351af2kyp5x19-glibc-2.42-51/lib/libc.so.6+0x2b338)
#12 0x0000555555558ee5 _start (/nix/store/s97jfrljxld3l91npal084xlccak1irj-llvm-22.1.0-rc3/bin/llvm-as+0x4ee5)
'/nix/store/s97jfrljxld3l91npal084xlccak1irj-llvm-22.1.0-rc3/bin/llvm-as test/programs/llvm/nucleic.ll -o test/programs/llvm/nucleic.bc' failed
```

---

### Comment 3 - EugeneZelenko

Could you please provide reproducer, preferably on https://godbolt.org?

---

### Comment 4 - jasoncarr0

@EugeneZelenko 

Is there anyway to tell where the issue might be in the IR? Maybe with a debug build of LLVM. These are generated files with tens of thousands of lines so it's not trivial to find the issue.

---

### Comment 5 - nikic

It should be possible to reduce this using llvm-reduce. Though if the IR is publicly sharable, just attaching the whole thing is fine as well.

---

### Comment 6 - jasoncarr0

[filter.ll](https://github.com/user-attachments/files/25947534/filter.ll.txt)

Here, I don't think it's sensitive at all. I think there's a user error in here, as another file caught the issue without crashing:
Some things are getting reloc instructions that are not locals and which are now not present in the statepoint (but were before). There's a separate enhancement or bugfix (https://github.com/llvm/llvm-project/issues/186229) needed to get that solution to work though.

---

### Comment 7 - nikic

I get this error message before the crash trace:
```llvm
gc.relocate: relocating a pointer shouldn't change its address space
  %g_11842 = call ptr addrspace(1) @llvm.experimental.gc.relocate.p1(token %tok_11840, i32 10, i32 10) ; (@llvm.experimental.gc.statepoint.p0, @llvm.experimental.gc.statepoint.p0)
```

The actual crash happens inside GCRelocateInst::getDerivedPtr(). It looks like the verifier does not check that the indices are in bounds if gc-live operand bundles are not used.

---

### Comment 8 - nikic

Smaller reproducer for the crash:
```llvm
define ptr addrspace(1) @test3(ptr addrspace(1) %a) gc "statepoint-example" {
entry:
  %safepoint_token = tail call token (i64, i32, ptr, i32, i32, ...) @llvm.experimental.gc.statepoint.p0(i64 0, i32 0, ptr elementtype(void (ptr addrspace(1))) @foo, i32 1, i32 0, ptr addrspace(1) %a, i32 0, i32 0)
  %reloc = call ptr addrspace(1) @llvm.experimental.gc.relocate.p1(token %safepoint_token,  i32 100, i32 100)
  ret ptr addrspace(1) %reloc
}
```

TBH I'm not sure if gc.relocate references not into gc-live are even supposed to be supported. LangRef only mentions the gc-live case: https://llvm.org/docs/LangRef.html#llvm-experimental-gc-relocate-intrinsic

---

### Comment 9 - nikic

@preames Do you know whether gc.relocate without gc-live operand bundle is supposed to be supported?

---

### Comment 10 - jasoncarr0

TBH I think we ended up with gc-statepoint bundles instead of gc-live because it worked? I was surprised by that but I didn't write the code originally.

Makes sense though, in this cause we probably skipped the bundle because of inconsistent filtering (the reloc code didn't realize to filter something out)

---

