# ARM: crash in ARMAsmPrinter::GetARMGVSymbol

**Author:** google-yfyang
**URL:** https://github.com/llvm/llvm-project/issues/167577
**Status:** Closed
**Labels:** backend:ARM, release:backport, crash
**Closed Date:** 2025-11-17T14:52:22Z

## Body

Minimal reproducer:
```
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-unknown-linux-android23"

@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 65535, ptr @upb_GeneratedRegistry_Constructor_dont_copy_me__upb_internal_use_only, ptr null }]

; Uncommenting this block will resolve the crash.
;define void @test() {
;ret void
;}

declare void @upb_GeneratedRegistry_Constructor_dont_copy_me__upb_internal_use_only()
```
Instructions to reproduce:
`llc <  reduced.ll`

The crash occurs on Subtarget field of ARMAsmPrinter inside of ARMAsmPrinter::GetARMGVSymbol. This is only initialized when runOnMachineFunction is called. That is why simply adding a function definition is "fixing" the crash. 


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: None (google-yfyang)

<details>
Minimal reproducer:
```
target datalayout = "e-m:e-p:32:32-Fi8-i64:64-v128:64:128-a:0:32-n32-S64"
target triple = "armv7-unknown-linux-android23"

@<!-- -->llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 65535, ptr @<!-- -->upb_GeneratedRegistry_Constructor_dont_copy_me__upb_internal_use_only, ptr null }]

; Uncommenting this block will resolve the crash.
;define void @<!-- -->test() {
;ret void
;}

declare void @<!-- -->upb_GeneratedRegistry_Constructor_dont_copy_me__upb_internal_use_only()
```
Instructions to reproduce:
`llc &lt;  reduced.ll`

The crash occurs on Subtarget field of ARMAsmPrinter inside of ARMAsmPrinter::GetARMGVSymbol. This is only initialized when runOnMachineFunction is called. That is why simply adding a function definition is "fixing" the crash. 

</details>


---

### Comment 2 - davemgreen

I believe there is a fix for this in #166329, as we saw it in #165422 too. I'll try and get that committed.

---

### Comment 3 - davemgreen

/cherry-pick 4d1f2492d26f8c2fad0eee2a141c7e0bbbc4c868

---

### Comment 4 - llvmbot

/pull-request llvm/llvm-project#168380

---

