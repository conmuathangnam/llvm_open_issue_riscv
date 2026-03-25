# [RISCV] +zcmp triggers assertion error with inline assembly clobber declarations

**Author:** Kuree
**URL:** https://github.com/llvm/llvm-project/issues/186527

## Body

See godbolt with llc (assertions trunk): https://godbolt.org/z/ra37G4MeT


```llvm
source_filename = "test"
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-rv32imac-unknown"

define dso_local void @test() local_unnamed_addr #23 align 64 {
entry:
  %0 = tail call i32 asm sideeffect "csrr $0, 0x42", "=r"() #25 
  %1 = tail call i32 asm sideeffect "csrr $0, 0x43", "=r"() #25
  tail call void asm sideeffect "", "~{x17},~{x10},~{x11},~{x12},~{x13}"() #25
  %cmp = icmp eq i32 %0, 11
  br i1 %cmp, label %if.else, label %if.else

if.else:                                          ; preds = %entry
  %call = tail call i32 @foo(i32 noundef %0, i32 noundef %1)
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void
}

declare i32 @foo(i32 noundef %arg0, i32 noundef %arg1);

attributes #23 = { nounwind "stack-protector-buffer-size"="8" "target-cpu"="generic-rv32" "target-features"="+32bit,+m,+zcmp" }
attributes #25 = { nounwind }
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-risc-v

Author: Keyi Zhang (Kuree)

<details>
See godbolt with llc (assertions trunk): https://godbolt.org/z/ra37G4MeT


```llvm
source_filename = "test"
target datalayout = "e-m:e-p:32:32-i64:64-n32-S128"
target triple = "riscv32-rv32imac-unknown"

define dso_local void @<!-- -->test() local_unnamed_addr #<!-- -->23 align 64 {
entry:
  %0 = tail call i32 asm sideeffect "csrr $0, 0x42", "=r"() #<!-- -->25 
  %1 = tail call i32 asm sideeffect "csrr $0, 0x43", "=r"() #<!-- -->25
  tail call void asm sideeffect "", "~{x17},~{x10},~{x11},~{x12},~{x13}"() #<!-- -->25
  %cmp = icmp eq i32 %0, 11
  br i1 %cmp, label %if.else, label %if.else

if.else:                                          ; preds = %entry
  %call = tail call i32 @<!-- -->foo(i32 noundef %0, i32 noundef %1)
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void
}

declare i32 @<!-- -->foo(i32 noundef %arg0, i32 noundef %arg1);

attributes #<!-- -->23 = { nounwind "stack-protector-buffer-size"="8" "target-cpu"="generic-rv32" "target-features"="+32bit,+m,+zcmp" }
attributes #<!-- -->25 = { nounwind }
```
</details>


---

### Comment 2 - lenary

Thanks. Do you know which versions this affects? Does it affect 22.x?

---

### Comment 3 - KavinTheG

Hey lenary, is it alright if I try solving this? I believe it's because the `mergeGPRPairInsns` function is being called despite the target not supporting `zdinx` or `P`. 

---

### Comment 4 - Kuree

@lenary I don't have a released version with assertion on, but I believe it works with commit 45a0f9cc1b1547430c9329b95a0090d5722e907e (with assertions on).

---

### Comment 5 - lenary

> Hey lenary, is it alright if I try solving this? I believe it's because the `mergeGPRPairInsns` function is being called despite the target not supporting `zdinx` or `P`.

Sure, if you reduce and add the provided testcase. "Reduce" in this case means:
- Remove source_filename, target triple, target datalayout
- Remove `dso_local`, `local_unnamed_address`, `#23` and `align 64` from `@test`, but add `nounwind` into the declaration.
- Remove the attributes lines at the bottom
- Add a RUN line for rv32, with `-mattr=+m,+zcmp` (this bit is important to get the right target features back).


You can add other run lines, if you think it's justified based on the fix, e.g. xqccmp (which should have the same crash), and maybe add equivalents for when a target has both zcmp and P (zcmp and dinx is less likely)


---

### Comment 6 - KavinTheG

Appreciate the guidance! Will have a fix asap. 

---

