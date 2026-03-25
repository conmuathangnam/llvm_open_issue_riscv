# Wrong result for _BitInt bitwise & testcase

**Author:** mikaelholmen
**URL:** https://github.com/llvm/llvm-project/issues/114360
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2024-11-28T19:01:10Z

## Body

llvm commit: 6effab990c5c1b
Reproducer:
```
clang bbi-100697.c -O2 -std=c23
./a.out
```
Result:
```
Fail!
```
This starts happening with https://github.com/llvm/llvm-project/commit/1833d418a04123916c1dbeb0c41c8bc7d06b779b
Before that commit we got
```
Pass!
```
which is the expected result.

I originally reported this in https://github.com/llvm/llvm-project/pull/107461#issuecomment-2446969736 but @alexey-bataev did some analysis in https://github.com/llvm/llvm-project/pull/107461#issuecomment-2448053507 suggesting that the error is somewhere in X86 Codegen rather than in SLP vectorizer.

As I wrote in the original report the input was originally part of a much larger test so I've reduced it a lot but unfortunately it's still quite large (and ugly).

[bbi-100697.c.gz](https://github.com/user-attachments/files/17582314/bbi-100697.c.gz)


## Comments

### Comment 1 - mikaelholmen

Slightly reduced C input in bbi-100697_2.c but it's still not very nice.

[bbi-100697_2.c.gz](https://github.com/user-attachments/files/17585658/bbi-100697_2.c.gz)


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Mikael Holmén (mikaelholmen)

<details>
llvm commit: 6effab990c5c1b
Reproducer:
```
clang bbi-100697.c -O2 -std=c23
./a.out
```
Result:
```
Fail!
```
This starts happening with https://github.com/llvm/llvm-project/commit/1833d418a04123916c1dbeb0c41c8bc7d06b779b
Before that commit we got
```
Pass!
```
which is the expected result.

I originally reported this in https://github.com/llvm/llvm-project/pull/107461#issuecomment-2446969736 but @<!-- -->alexey-bataev did some analysis in https://github.com/llvm/llvm-project/pull/107461#issuecomment-2448053507 suggesting that the error is somewhere in X86 Codegen rather than in SLP vectorizer.

As I wrote in the original report the input was originally part of a much larger test so I've reduced it a lot but unfortunately it's still quite large (and ugly).

[bbi-100697.c.gz](https://github.com/user-attachments/files/17582314/bbi-100697.c.gz)

</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-bug

Author: Mikael Holmén (mikaelholmen)

<details>
llvm commit: 6effab990c5c1b
Reproducer:
```
clang bbi-100697.c -O2 -std=c23
./a.out
```
Result:
```
Fail!
```
This starts happening with https://github.com/llvm/llvm-project/commit/1833d418a04123916c1dbeb0c41c8bc7d06b779b
Before that commit we got
```
Pass!
```
which is the expected result.

I originally reported this in https://github.com/llvm/llvm-project/pull/107461#issuecomment-2446969736 but @<!-- -->alexey-bataev did some analysis in https://github.com/llvm/llvm-project/pull/107461#issuecomment-2448053507 suggesting that the error is somewhere in X86 Codegen rather than in SLP vectorizer.

As I wrote in the original report the input was originally part of a much larger test so I've reduced it a lot but unfortunately it's still quite large (and ugly).

[bbi-100697.c.gz](https://github.com/user-attachments/files/17582314/bbi-100697.c.gz)

</details>


---

### Comment 4 - mikaelholmen

Slightly more reduced C input in bbi-100697_3.c and somewhat cleaned up so it's a bit easier to understand what the C code is actually testing. I really have troubles getting further than this though.

[bbi-100697_3.c.gz](https://github.com/user-attachments/files/17587904/bbi-100697_3.c.gz)


---

### Comment 5 - nikic

Here's a reduction with disabled DAGCombine:
```llvm
; RUN: llc -debug-counter=dagcombine=0 < %s
define i64 @test() {
    %x = bitcast i64 u0xffffffff00000001 to i64
    %t = trunc i64 %x to i32
    %a = and i32 %t, 1
    %e = zext i32 %a to i64
    ret i64 %e
}
```

Compiles to:
```
test:                                   # @test
        movabs  rax, -4294967295
        ret
```

What happens here is that the zext selects to subreg_to_reg because it's the result of an `and` which is going to zero the upper half anyway. But then shrinkAndImmediate will optimize away the and: https://github.com/llvm/llvm-project/blob/3a01b46dae1a00f77ed8c46a4239595803c9e35e/llvm/lib/Target/X86/X86ISelDAGToDAG.cpp#L4834-L4839

So we end up not doing the required clearing of the top bits.

@RKSimon Any thoughts on the best way to fix this? The def32 mechanism looks pretty fragile to me...

---

### Comment 6 - RKSimon

Its supposed to have been caught by this: https://github.com/llvm/llvm-project/blob/26fd693b979f17c83cbd5a3313fdea950ce3d356/llvm/lib/Target/X86/X86InstrExtension.td#L214-L222

---

### Comment 7 - nikic

@RKSimon The problem is that we match the zext first, at which point the and still exists.

---

### Comment 8 - RKSimon

Yes, still just working this out in my head:
```
Optimized legalized selection DAG: %bb.0 'test:'
SelectionDAG has 10 nodes:
    t0: ch,glue = EntryToken
        t3: i32 = truncate OpaqueConstant:i64<-4294967295>
      t5: i32 = and t3, Constant:i32<1>
    t6: i64 = zero_extend t5
  t9: ch,glue = CopyToReg t0, Register:i64 $rax, t6
  t10: ch = X86ISD::RET_GLUE t9, TargetConstant:i32<0>, Register:i64 $rax, t9:1


Selected selection DAG: %bb.0 'test:'
SelectionDAG has 11 nodes:
    t0: ch,glue = EntryToken
        t2: i64 = MOV64ri TargetConstant:i64<-4294967295>
      t3: i32 = EXTRACT_SUBREG t2, TargetConstant:i32<6>
    t6: i64 = SUBREG_TO_REG TargetConstant:i64<0>, t3, TargetConstant:i32<6>
  t9: ch,glue = CopyToReg t0, Register:i64 $rax, t6
  t10: ch = RET TargetConstant:i32<0>, Register:i64 $rax, t9, t9:1
```

---

### Comment 9 - RKSimon

I'm not convinced shrinkAndImmediate is safe in general tbh because of cases like this along with this 'opt out' def32 pattern: https://github.com/llvm/llvm-project/blob/26fd693b979f17c83cbd5a3313fdea950ce3d356/llvm/lib/Target/X86/X86InstrFragments.td#L682-L696

We can probably convert shrinkAndImmediate to a SDNodeXForm to use with `and` patterns but not sure what is best to do with def32.

@topperc can you remember anything about this? 

---

### Comment 10 - RKSimon

This is a (nasty) workaround until I can look at a better solution. One of the problems in this specific testcase is that DAG constant folding won't fold opaque constants, but computeKnownBits will report their contents. We could actually constant fold here if it'd help.

```cpp
  // The variable operand must be all zeros in the top bits to allow using the
  // new, negative constant as the mask.
  KnownBits Known0 = CurDAG->computeKnownBits(And0);
  if (Known0.isConstant() || !HighZeros.isSubsetOf(Known0.Zero))
    return false;
```

---

### Comment 11 - RKSimon

I've confirmed the workaround fixes the original regression cases as well - I'm going to push this and then decide how best to handle both shrinkAndImmediate and def32.

---

### Comment 12 - mikaelholmen

I've verified that the fix solves the original problem I saw.
Thanks @nikic and @RKSimon !

---

### Comment 13 - chapuni

Still failing. https://lab.llvm.org/buildbot/#/builders/63/builds/2834

---

### Comment 14 - RKSimon

Strange I'm not seeing this build failure and I build mainly on a MSVC target. I'll see what I can do.

---

### Comment 15 - nikic

@RKSimon The tests probably needs `REQUIRES: asserts` for `-debug-counter`.

---

