# [DirectX] Crash/assert hit in DXILForwardHandleAccesses pass with local resource array

**Author:** hekota
**URL:** https://github.com/llvm/llvm-project/issues/160937
**Status:** Closed
**Labels:** crash, backend:DirectX
**Closed Date:** 2025-10-16T23:19:13Z

## Body

Crash/assert hit in `DXILForwardHandleAccesses` pass on local resource array that was copied from a global multi-dimensional global array and passed into a local function.

https://godbolt.org/z/so9h6eeaP

HLSL:
```
RWStructuredBuffer<float> In[4][2] : register(u0);
RWStructuredBuffer<float> Out : register(u0, space1);

float foo(RWStructuredBuffer<float> A[2], uint Index) {
  return A[0][Index] + A[1][Index];
}

[numthreads(4,1,1)]
void main(uint GI : SV_GroupIndex) {
  for (int i = 0; i < 4; i++) {
    Out[i] = foo(In[i], i);
  }
}
```
Call stack:
```
Assertion failed: detail::isPresent(Val) && "dyn_cast on a non-existent value", file C:\llvm-project\llvm\include\llvm/Support/Casting.h, line 662
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.      Program arguments: c:\\llvm-project\\build\\x64-debug\\bin\\clang-dxc.exe -T cs_6_0 C:\\llvm-project\\build\\x64-Debug\\tools\\OffloadTest\\test\\clang-d3d12\\Feature\\ResourceArrays\\Output\\multi-dim-array-subset.test.tmp/source.hlsl
1.      <eof> parser at end of file
2.      Code generation
3.      Running pass 'Function Pass Manager' on module 'C:\llvm-project\build\x64-Debug\tools\OffloadTest\test\clang-d3d12\Feature\ResourceArrays\Output\multi-dim-array-subset.test.tmp/source.hlsl'.
4.      Running pass 'DXIL Forward Handle Accesses' on function '@main'
Exception Code: 0xC000001D
 #0 0x00007ff7df3cd16c HandleAbort C:\llvm-project\llvm\lib\Support\Windows\Signals.inc:372:0
 #1 0x00007ff8825d9e77 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0x99e77)
 #2 0x00007ff8825dbab9 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0x9bab9)
 #3 0x00007ff8825e1715 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xa1715)
 #4 0x00007ff8825e1387 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xa1387)
 #5 0x00007ff8825df351 (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0x9f351)
 #6 0x00007ff8825e1c7f (C:\WINDOWS\SYSTEM32\ucrtbased.dll+0xa1c7f)
 #7 0x00007ff7dedde550 llvm::dyn_cast<class llvm::GlobalVariable, class llvm::Value>(class llvm::Value *) C:\llvm-project\llvm\include\llvm\Support\Casting.h:663:0
 #8 0x00007ff7e00200d9 forwardHandleAccesses C:\llvm-project\llvm\lib\Target\DirectX\DXILForwardHandleAccesses.cpp:147:0
 #9 0x00007ff7e0020ef4 `anonymous namespace'::DXILForwardHandleAccessesLegacy::runOnFunction C:\llvm-project\llvm\lib\Target\DirectX\DXILForwardHandleAccesses.cpp:194:0
#10 0x00007ff7defaa313 llvm::FPPassManager::runOnFunction(class llvm::Function &) C:\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1398:0
#11 0x00007ff7defaee4d llvm::FPPassManager::runOnModule(class llvm::Module &) C:\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1444:0
#12 0x00007ff7defaac34 `anonymous namespace'::MPPassManager::runOnModule C:\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1513:0
#13 0x00007ff7defaa755 llvm::legacy::PassManagerImpl::run(class llvm::Module &) C:\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:531:0
#14 0x00007ff7defaf111 llvm::legacy::PassManager::run(class llvm::Module &) C:\llvm-project\llvm\lib\IR\LegacyPassManager.cpp:1640:0
...
}```

## Comments

### Comment 1 - farzonl

issue is `FindAvailableLoadedValue` is setting `Loaded` to a `nullptr`.
 https://github.com/llvm/llvm-project/blob/5da28bd331b243b62f30a211927b4e33b8dd943b/llvm/lib/Target/DirectX/DXILForwardHandleAccesses.cpp#L145

I didn't know this but if you try and dyn_cast a nullptr it triggers an assert.
https://github.com/llvm/llvm-project/blob/5da28bd331b243b62f30a211927b4e33b8dd943b/llvm/lib/Target/DirectX/DXILForwardHandleAccesses.cpp#L147

```gdb
* thread #1, queue = 'com.apple.main-thread', stop reason = hit program assert
    frame #4: 0x00000001000cd90c clang-dxc`decltype(auto) llvm::dyn_cast<llvm::GlobalVariable, llvm::Value>(Val=0x0000000000000000) at Casting.h:662:3
   659 
   660  template <typename To, typename From>
   661  [[nodiscard]] inline decltype(auto) dyn_cast(From *Val) {
-> 662    assert(detail::isPresent(Val) && "dyn_cast on a non-existent value");
   663    return CastInfo<To, From *>::doCastIfPossible(Val);
   664  }

* thread #1, queue = 'com.apple.main-thread', stop reason = hit program assert
    frame #0: 0x000000018ee12388 libsystem_kernel.dylib`__pthread_kill + 8
    frame #1: 0x000000018ee4b848 libsystem_pthread.dylib`pthread_kill + 296
    frame #2: 0x000000018ed549e4 libsystem_c.dylib`abort + 124
    frame #3: 0x000000018ed53c18 libsystem_c.dylib`__assert_rtn + 284
  * frame #4: 0x00000001000cd90c clang-dxc`decltype(auto) llvm::dyn_cast<llvm::GlobalVariable, llvm::Value>(Val=0x0000000000000000) at Casting.h:662:3
    frame #5: 0x00000001000e27b8 clang-dxc`forwardHandleAccesses(F=0x000000013702d5d8, DT=0x00000001370279c0) at DXILForwardHandleAccesses.cpp:147:18
    frame #6: 0x00000001000e9c08 clang-dxc`(anonymous namespace)::DXILForwardHandleAccessesLegacy::runOnFunction(this=0x0000000137027980, F=0x000000013702d5d8) at DXILForwardHandleAccesses.cpp:194:12
```

---

### Comment 2 - farzonl

I have a feeling maybe we should be running the `DeadStoreEliminationPass` sooner? @bogner  for thoughts.

If we look at the IR
```llvm
(lldb) expr Load->dump()
  %tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0..3 = load i32, ptr %tmp.i.sroa.0, align 4
```
The ptr we are loading `%tmp.i.sroa.0` is on the function stack and not a global.

Looking at the whole basic block
```llvm
entry:
  %agg.tmp7.i.sroa.0 = alloca target("dx.RawBuffer", float, 1, 0), align 8
  %agg.tmp7.i.sroa.4 = alloca target("dx.RawBuffer", float, 1, 0), align 4
  %tmp.i.sroa.0 = alloca target("dx.RawBuffer", float, 1, 0), align 4
  %tmp.i.sroa.4 = alloca target("dx.RawBuffer", float, 1, 0), align 4
  %0 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 1, i32 0, i32 1, i32 0, ptr nonnull @.str)
  call void @llvm.lifetime.start.p0(ptr nonnull %tmp.i.sroa.0)
  call void @llvm.lifetime.start.p0(ptr nonnull %tmp.i.sroa.4)
  %1 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 0, i32 0, i32 8, i32 0, ptr nonnull @In.str)
  store target("dx.RawBuffer", float, 1, 0) %1, ptr %tmp.i.sroa.0, align 4, !tbaa !4
  %2 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 0, i32 0, i32 8, i32 1, ptr nonnull @In.str)
  store target("dx.RawBuffer", float, 1, 0) %2, ptr %tmp.i.sroa.4, align 4, !tbaa !4
  %tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0. = load i32, ptr %tmp.i.sroa.0, align 4
  %tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.4. = load i32, ptr %tmp.i.sroa.4, align 4
  call void @llvm.lifetime.start.p0(ptr nonnull %agg.tmp7.i.sroa.4)
  store i32 %tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0., ptr %agg.tmp7.i.sroa.0, align 8
  store i32 %tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.4., ptr %agg.tmp7.i.sroa.4, align 4
  %agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0. = load target("dx.RawBuffer", float, 1, 0), ptr %agg.tmp7.i.sroa.0, align 8, !tbaa !4
  %3 = call { float, i1 } @llvm.dx.resource.load.rawbuffer.f32.tdx.RawBuffer_f32_1_0t(target("dx.RawBuffer", float, 1, 0) %agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0., i32 0, i32 0)
  %4 = extractvalue { float, i1 } %3, 0
  %agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.4. = load target("dx.RawBuffer", float, 1, 0), ptr %agg.tmp7.i.sroa.4, align 4, !tbaa !4
  %5 = call { float, i1 } @llvm.dx.resource.load.rawbuffer.f32.tdx.RawBuffer_f32_1_0t(target("dx.RawBuffer", float, 1, 0) %agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.4., i32 0, i32 0)
  %6 = extractvalue { float, i1 } %5, 0
  %add.i.i = fadd reassoc nnan ninf nsz arcp afn float %6, %4
  call void @llvm.lifetime.end.p0(ptr nonnull %agg.tmp7.i.sroa.4)
  call void @llvm.dx.resource.store.rawbuffer.tdx.RawBuffer_f32_1_0t.f32(target("dx.RawBuffer", float, 1, 0) %0, i32 0, i32 0, float %add.i.i)
  call void @llvm.lifetime.end.p0(ptr nonnull %tmp.i.sroa.0)
  call void @llvm.lifetime.end.p0(ptr nonnull %tmp.i.sroa.4)
  call void @llvm.lifetime.start.p0(ptr nonnull %tmp.i.sroa.0)
  call void @llvm.lifetime.start.p0(ptr nonnull %tmp.i.sroa.4)
  %7 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 0, i32 0, i32 8, i32 2, ptr nonnull @In.str)
  store target("dx.RawBuffer", float, 1, 0) %7, ptr %tmp.i.sroa.0, align 4, !tbaa !4
  %8 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 0, i32 0, i32 8, i32 3, ptr nonnull @In.str)
  store target("dx.RawBuffer", float, 1, 0) %8, ptr %tmp.i.sroa.4, align 4, !tbaa !4
  %tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0..1 = load i32, ptr %tmp.i.sroa.0, align 4
  %tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.4..1 = load i32, ptr %tmp.i.sroa.4, align 4
  call void @llvm.lifetime.start.p0(ptr nonnull %agg.tmp7.i.sroa.4)
  store i32 %tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0..1, ptr %agg.tmp7.i.sroa.0, align 8
  store i32 %tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.4..1, ptr %agg.tmp7.i.sroa.4, align 4
  %agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0..1 = load target("dx.RawBuffer", float, 1, 0), ptr %agg.tmp7.i.sroa.0, align 8, !tbaa !4
  %9 = call { float, i1 } @llvm.dx.resource.load.rawbuffer.f32.tdx.RawBuffer_f32_1_0t(target("dx.RawBuffer", float, 1, 0) %agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0..1, i32 1, i32 0)
  %10 = extractvalue { float, i1 } %9, 0
  %agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.4..1 = load target("dx.RawBuffer", float, 1, 0), ptr %agg.tmp7.i.sroa.4, align 4, !tbaa !4
  %11 = call { float, i1 } @llvm.dx.resource.load.rawbuffer.f32.tdx.RawBuffer_f32_1_0t(target("dx.RawBuffer", float, 1, 0) %agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.4..1, i32 1, i32 0)
  %12 = extractvalue { float, i1 } %11, 0
  %add.i.i.1 = fadd reassoc nnan ninf nsz arcp afn float %12, %10
  call void @llvm.lifetime.end.p0(ptr nonnull %agg.tmp7.i.sroa.4)
  call void @llvm.dx.resource.store.rawbuffer.tdx.RawBuffer_f32_1_0t.f32(target("dx.RawBuffer", float, 1, 0) %0, i32 1, i32 0, float %add.i.i.1)
  call void @llvm.lifetime.end.p0(ptr nonnull %tmp.i.sroa.0)
  call void @llvm.lifetime.end.p0(ptr nonnull %tmp.i.sroa.4)
  call void @llvm.lifetime.start.p0(ptr nonnull %tmp.i.sroa.0)
  call void @llvm.lifetime.start.p0(ptr nonnull %tmp.i.sroa.4)
  %13 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 0, i32 0, i32 8, i32 4, ptr nonnull @In.str)
  store target("dx.RawBuffer", float, 1, 0) %13, ptr %tmp.i.sroa.0, align 4, !tbaa !4
  %14 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 0, i32 0, i32 8, i32 5, ptr nonnull @In.str)
  store target("dx.RawBuffer", float, 1, 0) %14, ptr %tmp.i.sroa.4, align 4, !tbaa !4
  %tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0..2 = load i32, ptr %tmp.i.sroa.0, align 4
  %tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.4..2 = load i32, ptr %tmp.i.sroa.4, align 4
  call void @llvm.lifetime.start.p0(ptr nonnull %agg.tmp7.i.sroa.4)
  store i32 %tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0..2, ptr %agg.tmp7.i.sroa.0, align 8
  store i32 %tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.4..2, ptr %agg.tmp7.i.sroa.4, align 4
  %agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0..2 = load target("dx.RawBuffer", float, 1, 0), ptr %agg.tmp7.i.sroa.0, align 8, !tbaa !4
  %15 = call { float, i1 } @llvm.dx.resource.load.rawbuffer.f32.tdx.RawBuffer_f32_1_0t(target("dx.RawBuffer", float, 1, 0) %agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0..2, i32 2, i32 0)
  %16 = extractvalue { float, i1 } %15, 0
  %agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.4..2 = load target("dx.RawBuffer", float, 1, 0), ptr %agg.tmp7.i.sroa.4, align 4, !tbaa !4
  %17 = call { float, i1 } @llvm.dx.resource.load.rawbuffer.f32.tdx.RawBuffer_f32_1_0t(target("dx.RawBuffer", float, 1, 0) %agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.4..2, i32 2, i32 0)
  %18 = extractvalue { float, i1 } %17, 0
  %add.i.i.2 = fadd reassoc nnan ninf nsz arcp afn float %18, %16
  call void @llvm.lifetime.end.p0(ptr nonnull %agg.tmp7.i.sroa.4)
  call void @llvm.dx.resource.store.rawbuffer.tdx.RawBuffer_f32_1_0t.f32(target("dx.RawBuffer", float, 1, 0) %0, i32 2, i32 0, float %add.i.i.2)
  call void @llvm.lifetime.end.p0(ptr nonnull %tmp.i.sroa.0)
  call void @llvm.lifetime.end.p0(ptr nonnull %tmp.i.sroa.4)
  call void @llvm.lifetime.start.p0(ptr nonnull %tmp.i.sroa.0)
  call void @llvm.lifetime.start.p0(ptr nonnull %tmp.i.sroa.4)
  %19 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 0, i32 0, i32 8, i32 6, ptr nonnull @In.str)
  store target("dx.RawBuffer", float, 1, 0) %19, ptr %tmp.i.sroa.0, align 4, !tbaa !4
  %20 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 0, i32 0, i32 8, i32 7, ptr nonnull @In.str)
  store target("dx.RawBuffer", float, 1, 0) %20, ptr %tmp.i.sroa.4, align 4, !tbaa !4
  %tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0..3 = load i32, ptr %tmp.i.sroa.0, align 4
  %tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.4..3 = load i32, ptr %tmp.i.sroa.4, align 4
  call void @llvm.lifetime.start.p0(ptr nonnull %agg.tmp7.i.sroa.4)
  store i32 %tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0.tmp.i.sroa.0.0..3, ptr %agg.tmp7.i.sroa.0, align 8
  store i32 %tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.0.tmp.i.sroa.4.4..3, ptr %agg.tmp7.i.sroa.4, align 4
  %agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0..3 = load target("dx.RawBuffer", float, 1, 0), ptr %agg.tmp7.i.sroa.0, align 8, !tbaa !4
  %21 = call { float, i1 } @llvm.dx.resource.load.rawbuffer.f32.tdx.RawBuffer_f32_1_0t(target("dx.RawBuffer", float, 1, 0) %agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0.agg.tmp7.i.sroa.0.0..3, i32 3, i32 0)
  %22 = extractvalue { float, i1 } %21, 0
  %agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.4..3 = load target("dx.RawBuffer", float, 1, 0), ptr %agg.tmp7.i.sroa.4, align 4, !tbaa !4
  %23 = call { float, i1 } @llvm.dx.resource.load.rawbuffer.f32.tdx.RawBuffer_f32_1_0t(target("dx.RawBuffer", float, 1, 0) %agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.0.agg.tmp7.i.sroa.4.4..3, i32 3, i32 0)
  %24 = extractvalue { float, i1 } %23, 0
  %add.i.i.3 = fadd reassoc nnan ninf nsz arcp afn float %24, %22
  call void @llvm.lifetime.end.p0(ptr nonnull %agg.tmp7.i.sroa.4)
  call void @llvm.dx.resource.store.rawbuffer.tdx.RawBuffer_f32_1_0t.f32(target("dx.RawBuffer", float, 1, 0) %0, i32 3, i32 0, float %add.i.i.3)
  call void @llvm.lifetime.end.p0(ptr nonnull %tmp.i.sroa.0)
  call void @llvm.lifetime.end.p0(ptr nonnull %tmp.i.sroa.4)
  ret void
```
Two things are relevant
- ` %tmp.i.sroa.0 = alloca target("dx.RawBuffer", float, 1, 0), align 4`
-  `%1 = tail call target("dx.RawBuffer", float, 1, 0) @llvm.dx.resource.handlefrombinding.tdx.RawBuffer_f32_1_0t(i32 0, i32 0, i32 8, i32 0, ptr nonnull @In.str)
  store target("dx.RawBuffer", float, 1, 0) %1, ptr %tmp.i.sroa.0, align 4, !tbaa !4`

Deead store elimination should collapse these sroa alloca  and stores from global code. We fixed a similar bug by adding that pass. Now I'm thinking it might need to run sooner?


---

### Comment 3 - damyanp

@bogner - assigned to you to answer Farzon's questions above.

---

### Comment 4 - hekota

This no longer repros.

---

