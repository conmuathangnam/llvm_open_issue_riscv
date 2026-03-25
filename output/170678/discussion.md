# clang-21 --target=wasm32 -Og miscompilation

**Author:** TerrorJack
**URL:** https://github.com/llvm/llvm-project/issues/170678
**Status:** Closed
**Labels:** backend:WebAssembly, miscompilation
**Closed Date:** 2025-12-19T22:30:50Z

## Body

```c
void f(void) {
    int i = -1;
    do { i++; } while (i != 0);
}
```

when the above example is compiled with `clang-21 --target=wasm32 -Og`, the resulting object fails validation:

```
error: func 0 failed to validate

Caused by:
    0: type mismatch: values remaining on stack at end of block (at offset 0x43)
```

the disassembled wat content points out that `f` is mis-compiled, missing a `drop` instruction:

```wat
(module
  (type (;0;) (func))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func (;0;) (type 0)
    i32.const 0
  )
  (@custom "linking" (after code) "\02\08\86\80\80\80\00\01\00\04\00\01f")
  (@producers
    (processed-by "Ubuntu clang" "21.1.7 (++20251127113503+9ed1927442a4-1~exp1~20251127113518.64)")
  )
  (@custom "target_features" (after code) "\08+\0bbulk-memory+\0fbulk-memory-opt+\16call-indirect-overlong+\0amultivalue+\0fmutable-globals+\13nontrapping-fptoint+\0freference-types+\08sign-ext")
)
```

this issue doesn't reproduce on `clang-22`, so it might be the case that it's a duplicate issue that's been fixed earlier. still reporting this since 21 is the active release at the moment.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-webassembly

Author: Cheng Shao (TerrorJack)

<details>
```c
void f(void) {
    int i = -1;
    do { i++; } while (i != 0);
}
```

when the above example is compiled with `clang-21 --target=wasm32 -Og`, the resulting object fails validation:

```
error: func 0 failed to validate

Caused by:
    0: type mismatch: values remaining on stack at end of block (at offset 0x43)
```

the disassembled wat content points out that `f` is mis-compiled, missing a `drop` instruction:

```wat
(module
  (type (;0;) (func))
  (import "env" "__linear_memory" (memory (;0;) 0))
  (func (;0;) (type 0)
    i32.const 0
  )
  (@<!-- -->custom "linking" (after code) "\02\08\86\80\80\80\00\01\00\04\00\01f")
  (@<!-- -->producers
    (processed-by "Ubuntu clang" "21.1.7 (++20251127113503+9ed1927442a4-1~exp1~20251127113518.64)")
  )
  (@<!-- -->custom "target_features" (after code) "\08+\0bbulk-memory+\0fbulk-memory-opt+\16call-indirect-overlong+\0amultivalue+\0fmutable-globals+\13nontrapping-fptoint+\0freference-types+\08sign-ext")
)
```

this issue doesn't reproduce on `clang-22`, so it might be the case that it's a duplicate issue that's been fixed earlier. still reporting this since 21 is the active release at the moment.
</details>


---

### Comment 2 - dschuff

I think that  https://github.com/llvm/llvm-project/pull/160768 is the PR that fixed the issue. Can you confirm locally? I'm not sure where we are in the LLVM 21 release cycle right now but if it makes sense maybe we could backport the fix.

---

### Comment 3 - TerrorJack

yes, i've just rebuilt my wasi-sdk fork and can confirm that pr applies cleanly and fixes the issue. it would be helpful if it's officially backported to the upstream 21.x branch.

---

### Comment 4 - TerrorJack

upstream 21.x now contains the fix, thanks everyone!

---

