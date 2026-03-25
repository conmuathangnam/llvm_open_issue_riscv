# [X86] narrowBitOpRMW - post-ra enabled targets not retaining store-load dependency order on

**Author:** RKSimon
**URL:** https://github.com/llvm/llvm-project/issues/166744
**Status:** Closed
**Labels:** backend:X86, miscompilation
**Closed Date:** 2025-11-06T12:11:11Z

## Body

Noticed after #166376 - amd targets (which are the only ones that use post ra scheduling) are breaking the store-load chain for the bittest reload
```ll
define i1 @insertBit(ptr %v, i64 %idx, i1 zeroext %b) {
  %rem = and i64 %idx, 511
  %sh_prom = zext nneg i64 %rem to i512
  %shl = shl nuw i512 1, %sh_prom
  %not = xor i512 %shl, -1
  %load = load i512, ptr %v, align 8
  %and = and i512 %load, %not
  %conv2 = zext i1 %b to i512
  %shl4 = shl nuw i512 %conv2, %sh_prom
  %or = or i512 %and, %shl4
  store i512 %or, ptr %v, align 8
  %cmp = icmp ne i512 %or, 0
  ret i1 %cmp
}
```
llc -mcpu=znver2
```asm
insertBit: # @insertBit
  movl $1029, %eax # imm = 0x405
  shlxl %esi, %edx, %edx
  bextrl %eax, %esi, %eax
  movl (%rdi,%rax,4), %ecx
  btrl %esi, %ecx
  movq 8(%rdi), %rsi   # PREMATURE LOAD
  orq 40(%rdi), %rsi   # PREMATURE LOAD
  orl %ecx, %edx
  movl %edx, (%rdi,%rax,4)   # STORE
  movq 16(%rdi), %rax   # SAFE LOAD
  movq 24(%rdi), %rdx   # SAFE LOAD
  orq 56(%rdi), %rdx   # SAFE LOAD
  orq 48(%rdi), %rax   # SAFE LOAD
  movq (%rdi), %rcx   # SAFE LOAD
  orq 32(%rdi), %rcx   # SAFE LOAD
  orq %rdx, %rsi
  orq %rax, %rcx
  orq %rsi, %rcx
  setne %al
  retq
```
llc -mcpu=haswell
```asm
insertBit: # @insertBit
  movl %esi, %eax
  shrl $3, %eax
  andl $60, %eax
  movl (%rdi,%rax), %ecx
  btrl %esi, %ecx
  shlxl %esi, %edx, %edx
  orl %ecx, %edx
  movl %edx, (%rdi,%rax)  # STORE
  movq 16(%rdi), %rax   # SAFE LOAD
  movq (%rdi), %rcx   # SAFE LOAD
  movq 8(%rdi), %rdx   # SAFE LOAD
  movq 24(%rdi), %rsi   # SAFE LOAD
  orq 56(%rdi), %rsi   # SAFE LOAD
  orq 40(%rdi), %rdx   # SAFE LOAD
  orq 48(%rdi), %rax   # SAFE LOAD
  orq 32(%rdi), %rcx   # SAFE LOAD
  orq %rsi, %rdx
  orq %rax, %rcx
  orq %rdx, %rcx
  setne %al
  retq
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-x86

Author: Simon Pilgrim (RKSimon)

<details>
Noticed after #<!-- -->166376 - amd targets (which are the only ones that use post ra scheduling) are breaking the store-load chain for the bittest reload
```ll
define i1 @<!-- -->insertBit(ptr %v, i64 %idx, i1 zeroext %b) {
  %rem = and i64 %idx, 511
  %sh_prom = zext nneg i64 %rem to i512
  %shl = shl nuw i512 1, %sh_prom
  %not = xor i512 %shl, -1
  %load = load i512, ptr %v, align 8
  %and = and i512 %load, %not
  %conv2 = zext i1 %b to i512
  %shl4 = shl nuw i512 %conv2, %sh_prom
  %or = or i512 %and, %shl4
  store i512 %or, ptr %v, align 8
  %cmp = icmp ne i512 %or, 0
  ret i1 %cmp
}
```
llc -mcpu=znver2
```asm
insertBit: # @<!-- -->insertBit
  movl $1029, %eax # imm = 0x405
  shlxl %esi, %edx, %edx
  bextrl %eax, %esi, %eax
  movl (%rdi,%rax,4), %ecx
  btrl %esi, %ecx
  movq 8(%rdi), %rsi   # PREMATURE LOAD
  orq 40(%rdi), %rsi   # PREMATURE LOAD
  orl %ecx, %edx
  movl %edx, (%rdi,%rax,4)   # STORE
  movq 16(%rdi), %rax   # SAFE LOAD
  movq 24(%rdi), %rdx   # SAFE LOAD
  orq 56(%rdi), %rdx   # SAFE LOAD
  orq 48(%rdi), %rax   # SAFE LOAD
  movq (%rdi), %rcx   # SAFE LOAD
  orq 32(%rdi), %rcx   # SAFE LOAD
  orq %rdx, %rsi
  orq %rax, %rcx
  orq %rsi, %rcx
  setne %al
  retq
```
llc -mcpu=haswell
```asm
insertBit: # @<!-- -->insertBit
  movl %esi, %eax
  shrl $3, %eax
  andl $60, %eax
  movl (%rdi,%rax), %ecx
  btrl %esi, %ecx
  shlxl %esi, %edx, %edx
  orl %ecx, %edx
  movl %edx, (%rdi,%rax)  # STORE
  movq 16(%rdi), %rax   # SAFE LOAD
  movq (%rdi), %rcx   # SAFE LOAD
  movq 8(%rdi), %rdx   # SAFE LOAD
  movq 24(%rdi), %rsi   # SAFE LOAD
  orq 56(%rdi), %rsi   # SAFE LOAD
  orq 40(%rdi), %rdx   # SAFE LOAD
  orq 48(%rdi), %rax   # SAFE LOAD
  orq 32(%rdi), %rcx   # SAFE LOAD
  orq %rsi, %rdx
  orq %rax, %rcx
  orq %rdx, %rcx
  setne %al
  retq
```
</details>


---

