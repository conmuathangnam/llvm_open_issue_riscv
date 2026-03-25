# [llvm] cmpxchg16b uses pointer from overwritten rbx

**Author:** vasama
**URL:** https://github.com/llvm/llvm-project/issues/119959
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2025-04-03T13:56:54Z

## Body

Reduced IR:

```ll
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "amd64-pc-windows-msvc19.41.34123"

%struct.anon = type { [2 x %"struct.(anonymous namespace)::mt_shared_object"], %"class.vsm::atomic_intrusive_ptr", [48 x i8], %"struct.std::atomic.3", [56 x i8], %"struct.std::atomic_flag", [60 x i8] }
%"struct.(anonymous namespace)::mt_shared_object" = type { %"class.vsm::detail::basic_intrusive_refcount", %"struct.std::atomic_flag", ptr, [40 x i8] }
%"class.vsm::detail::basic_intrusive_refcount" = type { %"struct.vsm::detail::intrusive_refcount_base" }
%"struct.vsm::detail::intrusive_refcount_base" = type { %"class.vsm::atomic" }
%"class.vsm::atomic" = type { i64 }
%"class.vsm::atomic_intrusive_ptr" = type { %"class.vsm::atomic.2" }
%"class.vsm::atomic.2" = type { %"struct.vsm::atomic_intrusive_ptr<(anonymous namespace)::mt_shared_object>::atom" }
%"struct.vsm::atomic_intrusive_ptr<(anonymous namespace)::mt_shared_object>::atom" = type { ptr, i64 }
%"struct.std::atomic.3" = type { %"struct.std::_Atomic_integral_facade.4" }
%"struct.std::_Atomic_integral_facade.4" = type { %"struct.std::_Atomic_integral.5" }
%"struct.std::_Atomic_integral.5" = type { %"struct.std::_Atomic_storage.6" }
%"struct.std::_Atomic_storage.6" = type { %"struct.std::_Atomic_padded.7" }
%"struct.std::_Atomic_padded.7" = type { i64 }
%"struct.std::atomic_flag" = type { %"struct.std::atomic" }
%"struct.std::atomic" = type { %"struct.std::_Atomic_integral_facade" }
%"struct.std::_Atomic_integral_facade" = type { %"struct.std::_Atomic_integral" }
%"struct.std::_Atomic_integral" = type { %"struct.std::_Atomic_storage" }
%"struct.std::_Atomic_storage" = type { %"struct.std::_Atomic_padded" }
%"struct.std::_Atomic_padded" = type { i32 }

define fastcc void @"?test_case@?A0x7E1854EA@@YAXXZ"() #0 personality ptr @__CxxFrameHandler3 {
  %1 = alloca [0 x [0 x %struct.anon]], i32 0, align 64
  %2 = cmpxchg ptr %1, i128 0, i128 0 monotonic monotonic, align 16
  invoke void @"?_Throw_Cpp_error@std@@YAXH@Z"(i32 0)
          to label %3 unwind label %4

3:                                                ; preds = %0
  unreachable

4:                                                ; preds = %0
  %5 = cleanuppad within none []
  ret void
}

declare i32 @__CxxFrameHandler3(...)

declare void @"?_Throw_Cpp_error@std@@YAXH@Z"()

; uselistorder directives
uselistorder i32 0, { 1, 0 }

attributes #0 = { "target-cpu"="nehalem" }
```

Here is the resulting object code:
(`clang-19 -cc1 -emit-obj -triple "amd64-pc-windows-msvc19.41.34123" -O3 reduced.ll -o - | llvm-objdump-19 -M intel -d -`)
```asm
0000000000000000 <?test_case@?A0x7E1854EA@@YAXXZ>:
       0: 55                            push    rbp
       1: 53                            push    rbx
       2: 48 83 ec 68                   sub     rsp, 0x68
       6: 48 8d 6c 24 60                lea     rbp, [rsp + 0x60]
       b: 48 83 e4 c0                   and     rsp, -0x40
       f: 48 89 e3                      mov     rbx, rsp
      12: 48 89 6b 58                   mov     qword ptr [rbx + 0x58], rbp
      16: 48 c7 45 00 fe ff ff ff       mov     qword ptr [rbp], -0x2
      1e: 49 89 d8                      mov     r8, rbx
      21: 45 31 c9                      xor     r9d, r9d
      24: 31 c0                         xor     eax, eax
      26: 31 d2                         xor     edx, edx
      28: 31 c9                         xor     ecx, ecx
      2a: 4c 89 cb                      mov     rbx, r9
      2d: f0                            lock
      2e: 48 0f c7 4b 40                cmpxchg16b      xmmword ptr [rbx + 0x40]
      33: 4c 89 c3                      mov     rbx, r8
      36: 31 c9                         xor     ecx, ecx
      38: e8 00 00 00 00                call    0x3d <?test_case@?A0x7E1854EA@@YAXXZ+0x3d>
      3d: cc                            int3
      3e: 66 90                         nop
```

Note `mov     rbx, r9` followed by `cmpxchg16b      xmmword ptr [rbx + 0x40]` where `rbx` is used after having just been overwritten for the purposes of `cmpxchg16b` which uses it as an input register.

The original unreduced input produces slightly different object code but has the same problem:
```asm
00007FF786689459  lea         r8,[rbx+100h]  
00007FF786689460  mov         rax,qword ptr [rbx+140h]  
00007FF786689467  mov         rdx,qword ptr [rbx+148h]  
00007FF78668946E  nop  
00007FF786689470  mov         r9,rbx  
00007FF786689473  xor         ecx,ecx  
00007FF786689475  mov         rbx,r8  
00007FF786689478  lock cmpxchg16b oword ptr [rbx+140h]
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Lauri Vasama (vasama)

<details>
Reduced IR:

```ll
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "amd64-pc-windows-msvc19.41.34123"

%struct.anon = type { [2 x %"struct.(anonymous namespace)::mt_shared_object"], %"class.vsm::atomic_intrusive_ptr", [48 x i8], %"struct.std::atomic.3", [56 x i8], %"struct.std::atomic_flag", [60 x i8] }
%"struct.(anonymous namespace)::mt_shared_object" = type { %"class.vsm::detail::basic_intrusive_refcount", %"struct.std::atomic_flag", ptr, [40 x i8] }
%"class.vsm::detail::basic_intrusive_refcount" = type { %"struct.vsm::detail::intrusive_refcount_base" }
%"struct.vsm::detail::intrusive_refcount_base" = type { %"class.vsm::atomic" }
%"class.vsm::atomic" = type { i64 }
%"class.vsm::atomic_intrusive_ptr" = type { %"class.vsm::atomic.2" }
%"class.vsm::atomic.2" = type { %"struct.vsm::atomic_intrusive_ptr&lt;(anonymous namespace)::mt_shared_object&gt;::atom" }
%"struct.vsm::atomic_intrusive_ptr&lt;(anonymous namespace)::mt_shared_object&gt;::atom" = type { ptr, i64 }
%"struct.std::atomic.3" = type { %"struct.std::_Atomic_integral_facade.4" }
%"struct.std::_Atomic_integral_facade.4" = type { %"struct.std::_Atomic_integral.5" }
%"struct.std::_Atomic_integral.5" = type { %"struct.std::_Atomic_storage.6" }
%"struct.std::_Atomic_storage.6" = type { %"struct.std::_Atomic_padded.7" }
%"struct.std::_Atomic_padded.7" = type { i64 }
%"struct.std::atomic_flag" = type { %"struct.std::atomic" }
%"struct.std::atomic" = type { %"struct.std::_Atomic_integral_facade" }
%"struct.std::_Atomic_integral_facade" = type { %"struct.std::_Atomic_integral" }
%"struct.std::_Atomic_integral" = type { %"struct.std::_Atomic_storage" }
%"struct.std::_Atomic_storage" = type { %"struct.std::_Atomic_padded" }
%"struct.std::_Atomic_padded" = type { i32 }

define fastcc void @"?test_case@?A0x7E1854EA@@<!-- -->YAXXZ"() #<!-- -->0 personality ptr @<!-- -->__CxxFrameHandler3 {
  %1 = alloca [0 x [0 x %struct.anon]], i32 0, align 64
  %2 = cmpxchg ptr %1, i128 0, i128 0 monotonic monotonic, align 16
  invoke void @"?_Throw_Cpp_error@<!-- -->std@@<!-- -->YAXH@<!-- -->Z"(i32 0)
          to label %3 unwind label %4

3:                                                ; preds = %0
  unreachable

4:                                                ; preds = %0
  %5 = cleanuppad within none []
  ret void
}

declare i32 @<!-- -->__CxxFrameHandler3(...)

declare void @"?_Throw_Cpp_error@<!-- -->std@@<!-- -->YAXH@<!-- -->Z"()

; uselistorder directives
uselistorder i32 0, { 1, 0 }

attributes #<!-- -->0 = { "target-cpu"="nehalem" }
```

Here is the resulting object code:
(`clang-19 -cc1 -emit-obj -triple "amd64-pc-windows-msvc19.41.34123" -O3 reduced.ll -o - | llvm-objdump-19 -M intel -d -`)
```asm
0000000000000000 &lt;?test_case@?A0x7E1854EA@@<!-- -->YAXXZ&gt;:
       0: 55                            push    rbp
       1: 53                            push    rbx
       2: 48 83 ec 68                   sub     rsp, 0x68
       6: 48 8d 6c 24 60                lea     rbp, [rsp + 0x60]
       b: 48 83 e4 c0                   and     rsp, -0x40
       f: 48 89 e3                      mov     rbx, rsp
      12: 48 89 6b 58                   mov     qword ptr [rbx + 0x58], rbp
      16: 48 c7 45 00 fe ff ff ff       mov     qword ptr [rbp], -0x2
      1e: 49 89 d8                      mov     r8, rbx
      21: 45 31 c9                      xor     r9d, r9d
      24: 31 c0                         xor     eax, eax
      26: 31 d2                         xor     edx, edx
      28: 31 c9                         xor     ecx, ecx
      2a: 4c 89 cb                      mov     rbx, r9
      2d: f0                            lock
      2e: 48 0f c7 4b 40                cmpxchg16b      xmmword ptr [rbx + 0x40]
      33: 4c 89 c3                      mov     rbx, r8
      36: 31 c9                         xor     ecx, ecx
      38: e8 00 00 00 00                call    0x3d &lt;?test_case@?A0x7E1854EA@@<!-- -->YAXXZ+0x3d&gt;
      3d: cc                            int3
      3e: 66 90                         nop
```

Note `mov     rbx, r9` followed by `cmpxchg16b      xmmword ptr [rbx + 0x40]` where `rbx` is used after having just been overwritten for the purposes of `cmpxchg16b` which uses it as an input register.

The original unreduced input produces slightly different object code but has the same problem:
```asm
00007FF786689459  lea         r8,[rbx+100h]  
00007FF786689460  mov         rax,qword ptr [rbx+140h]  
00007FF786689467  mov         rdx,qword ptr [rbx+148h]  
00007FF78668946E  nop  
00007FF786689470  mov         r9,rbx  
00007FF786689473  xor         ecx,ecx  
00007FF786689475  mov         rbx,r8  
00007FF786689478  lock cmpxchg16b oword ptr [rbx+140h]
```

</details>


---

### Comment 2 - becrux

We've been also affected by the same issue and, according to our bisecting, this has been caused by https://github.com/llvm/llvm-project/commit/51fbab134560ece663517bf1e8c2a30300d08f1a (ASAN)

---

### Comment 3 - aaronpuchert

Maybe I'm misreading this, but doesn't the `alloca` allocate nothing?
```llvm
%1 = alloca [0 x [0 x %struct.anon]], i32 0, align 64
```
The type is an array of size 0, and we have 0 elements of that. Indeed, if I compile with `-Rpass-analysis=stack-frame-layout`:
```
remark: <unknown>:0:0:
Function: ?test_case@?A0x7E1854EA@@YAXXZ
Offset: [SP-8], Type: Spill, Align: 16, Size: 8
Offset: [SP-16], Type: Spill, Align: 8, Size: 8
Offset: [SP-24], Type: Fixed, Align: 16, Size: 8
Offset: [SP-32], Type: Spill, Align: 8, Size: 8
Offset: [SP-56], Type: Variable, Align: 64, Size: 1 [-Rpass-analysis=stack-frame-layout]
```
The variable has the correct alignment, and size 1 (probably for unique addresses).

Then it seems like the `cmpxchg` afterwards should be undefined behavior. So I presume the issue is not in code generation for this IR, but a previous optimization that generated the IR, or perhaps even the original IR emitted by the frontend?

---

### Comment 4 - vasama

@aaronpuchert I think that's just llvm-reduce. Anyway, the output is exactly the same if that alloca is changed to `alloca [1 x [1 x %struct.anon]]`.

---

### Comment 5 - aaronpuchert

We also need to change the second argument to be at least 1:
```llvm
%1 = alloca [1 x [1 x %struct.anon]], i32 1, align 64
```
Then we get:
```
remark: <unknown>:0:0:
Function: ?test_case@?A0x7E1854EA@@YAXXZ
Offset: [SP-8], Type: Spill, Align: 16, Size: 8
Offset: [SP-16], Type: Spill, Align: 8, Size: 8
Offset: [SP-24], Type: Fixed, Align: 16, Size: 8
Offset: [SP-32], Type: Spill, Align: 8, Size: 8
Offset: [SP-376], Type: Variable, Align: 64, Size: 320 [-Rpass-analysis=stack-frame-layout]
```
We should also match the parameters of `_Throw_Cpp_error` to the way it's being called:
```llvm
declare void @"?_Throw_Cpp_error@std@@YAXH@Z"(i32)
```
My understanding of exception handling is limited, but https://llvm.org/docs/ExceptionHandling.html#funclet-transitions says

> Note that the `ret` instruction is not a valid way to exit a funclet pad; it is undefined behavior to execute a `ret` when a pad has been entered but not exited.

So let's replace `ret` with
```llvm
cleanupret from %5 unwind to caller
```
That last step did it (output of `-S`):
```
"?test_case@?A0x7E1854EA@@YAXXZ":       # @"?test_case@?A0x7E1854EA@@YAXXZ"
.seh_proc "?test_case@?A0x7E1854EA@@YAXXZ"
# %bb.0:
        pushq   %rbp
        .seh_pushreg %rbp
        pushq   %rbx
        .seh_pushreg %rbx
        subq    $424, %rsp                      # imm = 0x1A8
        .seh_stackalloc 424
        leaq    128(%rsp), %rbp
        .seh_setframe %rbp, 128
        .seh_endprologue
        andq    $-64, %rsp
        xorl    %eax, %eax
        xorl    %edx, %edx
        xorl    %ecx, %ecx
        xorl    %ebx, %ebx
        lock            cmpxchg16b      64(%rsp)
        xorl    %ecx, %ecx
        callq   "?_Throw_Cpp_error@std@@YAXH@Z"
        int3
```
We're using `rsp`, which of course points to the only stack variable. So that looks fine.

---

### Comment 6 - aaronpuchert

Ok, but let's say we don't want to continue unwinding. Even though we're directly throwing here, we also want to catch it and return regularly. Then I think we need to write (continuing at label 4):

```llvm
4:                                                ; preds = %0
  %cs = catchswitch within none [label %handler] unwind to caller

handler:
  %tok = catchpad within %cs [i8* null, i32 64, i8* null]
  catchret from %tok to label %return

return:
  ret void
```

Because the `cleanuppad` can only clean up and continue unwinding. If we want to get rid of the exception, we need to catch it. The magic arguments to the `catchpad` should match `catch(...)`.

With that, the miscompilation reappears:
```
        xorl    %r9d, %r9d
        xorl    %eax, %eax
        xorl    %edx, %edx
        xorl    %ecx, %ecx
        movq    %r9, %rbx
        lock cmpxchg16b 64(%rbx)
```
But I'm not an expert in exception handling (let alone the Windows kind), so I'm not sure if this is valid now.

---

### Comment 7 - aaronpuchert

The cause seems to be that `rbx` is an (implicit) input to `cmpxchg16b`, but also used as frame base pointer in this function. The base pointer is treated specially: the register allocator simply ignores it, assuming that it's never touched, and all accesses to frame variables will blindly use that pointer. Because that doesn't work in the case of `cmpxchg16b`, there was a workaround added in cf9732b4177bba5a073619f8f067cf4a0193b325: a pseudo-instruction `LCMPXCHG16B_SAVE_RBX` is emitted that takes the saved `rbx` and the desired input in `rbx` as additional arguments. It is then expanded at a very late stage into a copy of the `rbx` input into `rbx`, followed by the instruction, followed by a restoration of `rbx` from the saved contents. This happens in "X86 pseudo instruction expansion pass (x86-pseudo)". Currently, in the example, it looks like this:
```diff
   $r8 = MOV64rr $rbx
   renamable $r9d = XOR32rr undef $r9d(tied-def 0), undef $r9d, implicit-def dead $eflags, implicit-def $r9
   dead $eax = XOR32rr undef $eax(tied-def 0), undef $eax, implicit-def dead $eflags, implicit-def $rax
   dead $edx = XOR32rr undef $edx(tied-def 0), undef $edx, implicit-def dead $eflags, implicit-def $rdx
   dead $ecx = XOR32rr undef $ecx(tied-def 0), undef $ecx, implicit-def dead $eflags, implicit-def $rcx
-  dead renamable $r8 = LCMPXCHG16B_SAVE_RBX $rbx, 1, $noreg, 64, $noreg, killed renamable $r9, killed renamable $r8(tied-def 0), implicit-def dead $rax, implicit-def dead $rdx, implicit-def $rbx, implicit-def dead $eflags, implicit $rax, implicit $rcx, implicit $rdx
+  $rbx = MOV64rr $r9
+  LCMPXCHG16B $rbx, 1, $noreg, 64, $noreg, implicit-def $rax, implicit-def $rdx, implicit-def $eflags, implicit $rax, implicit $rbx, implicit $rcx, implicit $rdx
+  $rbx = MOV64rr killed $r8
   EH_LABEL <mcsymbol .Ltmp0>
   CALL64pcrel32 @"?_Throw_Cpp_error@std@@YAXH@Z", <regmask [...]>, implicit $rsp, implicit $ssp, implicit-def $rsp, implicit-def $ssp
   EH_LABEL <mcsymbol .Ltmp1>
```
I think we should rewrite the memory operand and replace `rbx` with the saved `rbx` argument (here `r8`).

---

### Comment 8 - aaronpuchert

I've proposed a fix at #134109. @vasama, maybe you want to test if this solves your original issue.

---

### Comment 9 - aaronpuchert

For what it's worth, in the reproducer we end up expanding the pseudo-instruction like this instead:
```diff
   $r8 = MOV64rr $rbx
   renamable $r9d = XOR32rr undef $r9d(tied-def 0), undef $r9d, implicit-def dead $eflags, implicit-def $r9
   dead $eax = XOR32rr undef $eax(tied-def 0), undef $eax, implicit-def dead $eflags, implicit-def $rax
   dead $edx = XOR32rr undef $edx(tied-def 0), undef $edx, implicit-def dead $eflags, implicit-def $rdx
   dead $ecx = XOR32rr undef $ecx(tied-def 0), undef $ecx, implicit-def dead $eflags, implicit-def $rcx
-  dead renamable $r8 = LCMPXCHG16B_SAVE_RBX $rbx, 1, $noreg, 64, $noreg, killed renamable $r9, killed renamable $r8(tied-def 0), implicit-def dead $rax, implicit-def dead $rdx, implicit-def $rbx, implicit-def dead $eflags, implicit $rax, implicit $rcx, implicit $rdx
+  $rbx = MOV64rr $r9
+  LCMPXCHG16B $r8, 1, $noreg, 64, $noreg, implicit-def $rax, implicit-def $rdx, implicit-def $eflags, implicit $rax, implicit $rbx, implicit $rcx, implicit $rdx
+  $rbx = MOV64rr killed $r8
   EH_LABEL <mcsymbol .Ltmp0>
   CALL64pcrel32 @"?_Throw_Cpp_error@std@@YAXH@Z", <regmask $bh $bl $bp $bph $bpl $bx $di $dih $dil $ebp $ebx $edi $esi $hbp $hbx $hdi $hsi $rbp $rbx $rdi $rsi $si $sih $sil $r12 $r13 $r14 $r15 $xmm6 $xmm7 $xmm8 $xmm9 $xmm10 and 25 more...>, implicit $rsp, implicit $ssp, implicit-def $rsp, implicit-def $ssp
   EH_LABEL <mcsymbol .Ltmp1>
```
Note that we're using `$r8` instead of `$rbx`. The final assembly (just the relevant parts):
```asm
        movq    %rbx, %r8
        xorl    %r9d, %r9d
        xorl    %eax, %eax
        xorl    %edx, %edx
        xorl    %ecx, %ecx
        movq    %r9, %rbx
        lock cmpxchg16b 64(%r8)
        movq    %r8, %rbx
```
I can't run this, but it looks correct to me.

---

### Comment 10 - aaronpuchert

> So let's replace `ret` with
> ```llvm
> cleanupret from %5 unwind to caller
> ```
> That last step did it (output of `-S`):

As a side note: the reason that this works is because an IR optimization removes the no-op cleanup:

```diff

   %1 = alloca %struct.anon, align 64
   %2 = cmpxchg ptr %1, i128 0, i128 0 monotonic monotonic, align 16
-  invoke void @"?_Throw_Cpp_error@std@@YAXH@Z"()
-          to label %3 unwind label %4
+  call void @"?_Throw_Cpp_error@std@@YAXH@Z"()
+  unreachable

-3:                                                ; preds = %0
-  unreachable

-4:                                                ; preds = %0
-  %5 = cleanuppad within none []
-  cleanupret from %5 unwind to caller
```
And without SEH we apparently don't use `rbx` as base pointer.

---

### Comment 11 - aaronpuchert

Fix has been ported back to the LLVM 20 release branch (a0c8959cc880900e5f8ad5721388777984fe7044) and should be contained in 20.1.3.

---

