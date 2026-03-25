# [hexagon] SIGSEGV in `std::set` red-black tree destruction HexagonMCChecker.cpp

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/181461
**Status:** Open
**Labels:** backend:Hexagon, crash

## Body

Found on `release/22.x` (22.1.0-rc3). 

When cross-compiling LLVM for `hexagon-unknown-linux-musl` at `-O1` or above, the resulting native Hexagon LLVM tools exhibited widespread crashes and assertion failures across multiple unrelated source files:

- **HexagonMCChecker.cpp** — SIGSEGV in `std::set` red-black tree destruction.  A tree node pointer is corrupted to `0x1` (only a color bit, no valid address) because an inlined `std::set::__insert_node_at` store-load pair executed out of order.
- **HexagonMCCompound.cpp** — "out of slots" errors on legal compound instruction packets.  The compound instruction splitting logic produced incorrect results due to stale loads.
- **Verifier.cpp** — False "Broken module found" errors on all valid LLVM IR, including trivial functions like `add i32`.  A stored flag was read back through a differently-typed pointer with stale data.
- **LiveIntervals.cpp** — "reserved computation mismatch" assertion.  Two computations of `isReservedRegUnit()` using the same iterators produced different results because iterator state was corrupted by out-of-order memory operations.
- **Systemic** — 58 out of 100 sampled `CodeGen/Hexagon` tests failed with various crashes, assertion failures, and SIGSEGV at addresses like `0x1`, `0xf`, `0xc000001`.

All issues are absent at `-O0` (no packetization) and share this common root cause.

## Reduced tests

The following LLVM IR demonstrates the bug.  On V65+, the store and load land in the same packet **without** `:mem_noshuf`:

```llvm
; RUN: llc -march=hexagon -mcpu=hexagonv65 -O2 < %s | FileCheck %s

; CHECK-LABEL: test_store_imm_load:
; CHECK:       {
; CHECK-DAG:   memw(r0+#0) = #1
; CHECK-DAG:   r0 = memw(r1+#0)
; CHECK:       } :mem_noshuf

define i32 @test_store_imm_load(ptr %p, ptr %q) #0 {
entry:
  store i32 1, ptr %p, align 4, !tbaa !0
  %v = load i32, ptr %q, align 4, !tbaa !3
  ret i32 %v
}

attributes #0 = { nounwind }

!0 = !{!1, !1, i64 0}        ; type_a
!1 = !{!"type_a", !2}
!2 = !{!"tbaa_root"}
!3 = !{!4, !4, i64 0}        ; type_b
!4 = !{!"type_b", !2}
```

**Buggy output** (release/22.x, V65):
```asm
test_store_imm_load:
    {
        jumpr r31
        r0 = memw(r1+#0)
        memw(r0+#0) = #1
    }
```

No `:mem_noshuf` — the hardware may execute the load before the store.  If `%p == %q`, the load returns the old value instead of `1`.

**Expected output:**
```asm
test_store_imm_load:
    {
        jumpr r31
        memw(r0+#0) = #1
        r0 = memw(r1+#0)
    } :mem_noshuf
```

### Additional patterns

The same bug manifests with several variations:

1. **Tree node insertion** — store a pointer to a node field, load a pointer from another node field (models `std::set::__insert_node_at`):
```llvm
define ptr @test_tree_node_insert(ptr %new_node, ptr %parent, ptr %child_ptr) #0 {
  store ptr %parent, ptr %new_node, align 4, !tbaa !0
  %child = load ptr, ptr %child_ptr, align 4, !tbaa !3
  ret ptr %child
}
```

2. **Store-load feeding control flow** — loaded value used for a branch decision:
```llvm
define i32 @test_store_load_branch(ptr %flag_ptr, ptr %data_ptr, i32 %val) #0 {
  store i32 %val, ptr %flag_ptr, align 4, !tbaa !0
  %data = load i32, ptr %data_ptr, align 4, !tbaa !3
  %cmp = icmp eq i32 %data, 0
  br i1 %cmp, label %then, label %else
then:
  ret i32 %val
else:
  ret i32 %data
}
```

3. **Multiple store-load pairs** — each pair uses different TBAA types, all missing `:mem_noshuf`:
```llvm
define i32 @test_multi_store_load(ptr %p1, ptr %p2, ptr %p3, ptr %p4) #0 {
  store i32 10, ptr %p1, align 4, !tbaa !0
  %v1 = load i32, ptr %p2, align 4, !tbaa !3
  store i32 %v1, ptr %p3, align 4, !tbaa !5
  %v2 = load i32, ptr %p4, align 4, !tbaa !7
  %sum = add i32 %v1, %v2
  ret i32 %sum
}
```



## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
Found on `release/22.x` (22.1.0-rc3). 

When cross-compiling LLVM for `hexagon-unknown-linux-musl` at `-O1` or above, the resulting native Hexagon LLVM tools exhibited widespread crashes and assertion failures across multiple unrelated source files:

- **HexagonMCChecker.cpp** — SIGSEGV in `std::set` red-black tree destruction.  A tree node pointer is corrupted to `0x1` (only a color bit, no valid address) because an inlined `std::set::__insert_node_at` store-load pair executed out of order.
- **HexagonMCCompound.cpp** — "out of slots" errors on legal compound instruction packets.  The compound instruction splitting logic produced incorrect results due to stale loads.
- **Verifier.cpp** — False "Broken module found" errors on all valid LLVM IR, including trivial functions like `add i32`.  A stored flag was read back through a differently-typed pointer with stale data.
- **LiveIntervals.cpp** — "reserved computation mismatch" assertion.  Two computations of `isReservedRegUnit()` using the same iterators produced different results because iterator state was corrupted by out-of-order memory operations.
- **Systemic** — 58 out of 100 sampled `CodeGen/Hexagon` tests failed with various crashes, assertion failures, and SIGSEGV at addresses like `0x1`, `0xf`, `0xc000001`.

All issues are absent at `-O0` (no packetization) and share this common root cause.

## Reduced tests

The following LLVM IR demonstrates the bug.  On V65+, the store and load land in the same packet **without** `:mem_noshuf`:

```llvm
; RUN: llc -march=hexagon -mcpu=hexagonv65 -O2 &lt; %s | FileCheck %s

; CHECK-LABEL: test_store_imm_load:
; CHECK:       {
; CHECK-DAG:   memw(r0+#<!-- -->0) = #<!-- -->1
; CHECK-DAG:   r0 = memw(r1+#<!-- -->0)
; CHECK:       } :mem_noshuf

define i32 @<!-- -->test_store_imm_load(ptr %p, ptr %q) #<!-- -->0 {
entry:
  store i32 1, ptr %p, align 4, !tbaa !0
  %v = load i32, ptr %q, align 4, !tbaa !3
  ret i32 %v
}

attributes #<!-- -->0 = { nounwind }

!0 = !{!1, !1, i64 0}        ; type_a
!1 = !{!"type_a", !2}
!2 = !{!"tbaa_root"}
!3 = !{!4, !4, i64 0}        ; type_b
!4 = !{!"type_b", !2}
```

**Buggy output** (release/22.x, V65):
```asm
test_store_imm_load:
    {
        jumpr r31
        r0 = memw(r1+#<!-- -->0)
        memw(r0+#<!-- -->0) = #<!-- -->1
    }
```

No `:mem_noshuf` — the hardware may execute the load before the store.  If `%p == %q`, the load returns the old value instead of `1`.

**Expected output:**
```asm
test_store_imm_load:
    {
        jumpr r31
        memw(r0+#<!-- -->0) = #<!-- -->1
        r0 = memw(r1+#<!-- -->0)
    } :mem_noshuf
```

### Additional patterns

The same bug manifests with several variations:

1. **Tree node insertion** — store a pointer to a node field, load a pointer from another node field (models `std::set::__insert_node_at`):
```llvm
define ptr @<!-- -->test_tree_node_insert(ptr %new_node, ptr %parent, ptr %child_ptr) #<!-- -->0 {
  store ptr %parent, ptr %new_node, align 4, !tbaa !0
  %child = load ptr, ptr %child_ptr, align 4, !tbaa !3
  ret ptr %child
}
```

2. **Store-load feeding control flow** — loaded value used for a branch decision:
```llvm
define i32 @<!-- -->test_store_load_branch(ptr %flag_ptr, ptr %data_ptr, i32 %val) #<!-- -->0 {
  store i32 %val, ptr %flag_ptr, align 4, !tbaa !0
  %data = load i32, ptr %data_ptr, align 4, !tbaa !3
  %cmp = icmp eq i32 %data, 0
  br i1 %cmp, label %then, label %else
then:
  ret i32 %val
else:
  ret i32 %data
}
```

3. **Multiple store-load pairs** — each pair uses different TBAA types, all missing `:mem_noshuf`:
```llvm
define i32 @<!-- -->test_multi_store_load(ptr %p1, ptr %p2, ptr %p3, ptr %p4) #<!-- -->0 {
  store i32 10, ptr %p1, align 4, !tbaa !0
  %v1 = load i32, ptr %p2, align 4, !tbaa !3
  store i32 %v1, ptr %p3, align 4, !tbaa !5
  %v2 = load i32, ptr %p4, align 4, !tbaa !7
  %sum = add i32 %v1, %v2
  ret i32 %sum
}
```


</details>


---

### Comment 2 - androm3da

Fixed by #181456

---

### Comment 3 - quic-akaryaki

The test case shows the correct behavior. With one load and one store in a packet, the load is in slot 1 and it logically executes first.

---

### Comment 4 - iajbar

These two packets are the same 
{        jumpr r31        r0 = memw(r1+#0)        memw(r0+#0) = #1    } versus
{        jumpr r31        memw(r0+#0) = #1        r0 = memw(r1+#0)    }:mem_noshuf
The scalar registers are all updated at commit at the end of the packet. In ISS, the store transaction is logged and at the end of the packet, it is committed. So, the state of r0 is only updated after the packet completes.
 
 

---

### Comment 5 - androm3da

> These two packets are the same
> ```
> { jumpr r31 r0 = memw(r1+#0) memw(r0+#0) = #1 }
> ```
>  versus 
> ```
> { jumpr r31 memw(r0+#0) = #1 r0 = memw(r1+#0) }:mem_noshuf 
> ```
> The scalar registers are all updated at commit at the end of the packet. In ISS, the store transaction is logged and at the end of the packet, it is committed. So, the state of r0 is only updated after the packet completes.

Apologies - I have incorrectly reduced the test cases from the original symptom.

I will start over with a newly reduced test case.



---

### Comment 6 - quic-akaryaki

> > These two packets are the same
> > ```
> > { jumpr r31 r0 = memw(r1+#0) memw(r0+#0) = #1 }
> > ```
> > versus
> > ```
> > { jumpr r31 memw(r0+#0) = #1 r0 = memw(r1+#0) }:mem_noshuf 
> > ```
> >   
> > The scalar registers are all updated at commit at the end of the packet. In ISS, the store transaction is logged and at the end of the packet, it is committed. So, the state of r0 is only updated after the packet completes.
> 
> Apologies - I have incorrectly reduced the test cases from the original symptom.
> 
> I will start over with a newly reduced test case.

These packets are not the same. The first packet returns the previous value and the second one returns `1`, in case r0 and r1 pointers are equal. The instruction in slot 0 executes after the instruction in slot 1 and see the new memory content, if it is a load.
The new value of r0 is not used as a pointer because of normal packet semantics.
However, in the original problem no aliasing is assumed as pointers are of different types, so compiler is allowed to generate any sequence, so any of these packets are correct.



---

### Comment 7 - iajbar

Sorry, my statement was not correct 
{
   memw(R5) = R2    // slot 1 store
   R3 = memh(R6)    // slot 0 load
}**:mem_noshuf**
The PRM stated that unlike most packetized operations, these memory operations do not execute in parallel. Instead, the store instruction in slot 1 effectively executes first, followed by the load instruction in slot 0. If the addresses of the two operations are overlapping, the load receives the newly stored data. 
But since the pointers are of different types, I agree with Alexey.

---

### Comment 8 - androm3da

This program fails on hexagon at O2.  It doesn't fail on native x86_64 clang.

```
__attribute__((noinline))
int store_float_load_int(float *a, int *b, float v) {
    *a = v;
    return *b;
}

__attribute__((noinline, optnone))
int store_float_load_int_safe(float *a, int *b, float v) {
    *a = v;
    return *b;
}

int main(void) {
    union { float f; int i; } u;

    u.i = 0;

    volatile float val = 1.0f;
    int r1 = store_float_load_int(&u.f, &u.i, val);
    /* Reset and call the safe (optnone) version */
    u.i = 0;
    int r2 = store_float_load_int_safe(&u.f, &u.i, val);

    /*
     * 1.0f == 0x3f800000 in IEEE-754.
     * After each store, u.i must read back 0x3f800000.
     */
    return (r1 != 0x3f800000) | (r2 != 0x3f800000);
}
```

```
$ clang -o tbaa tbaa.c -O3
$ ./tbaa; echo $?
0
$ clang -o tbaa tbaa.c -O2
$ ./tbaa; echo $?
0
```

```
$ /local/mnt/workspace/install/clang+llvm-21.1.8-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/hexagon-linux-musl-clang -o tbaa tbaa.c -static -O2
$ /local/mnt/workspace/install/clang+llvm-21.1.8-cross-hexagon-unknown-linux-musl/x86_64-linux-gnu/bin/qemu-hexagon ./tbaa; echo $?
1

```

---

### Comment 9 - androm3da

FWIW @iajbar and @quic-akaryaki - applying the change from #181456 causes the `tbaa.c` test case above to start passing at `-O2`.

---

