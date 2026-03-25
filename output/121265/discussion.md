# GlobalOpt breaks !callees metadata

**Author:** nunoplopes
**URL:** https://github.com/llvm/llvm-project/issues/121265
**Status:** Open
**Labels:** miscompilation, ipo

## Body

Running `opt -passes=globalopt` on the example below changes the !callees metadata to `!0 = distinct !{ptr @test, null}`.

```llvm
declare i8 @test()
declare i8 @test2()
@fptr = external global ptr

define i8 @fn() {
  %p = load ptr, ptr @fptr, align 8
  %ret = call i8 %p(), !callees !0
  ret i8 %ret
}

define i8 @keep_ref() {
  %ret = call i8 @test()
  ret i8 %ret
}

!0 = !{ptr @test, ptr @test2}
```

Discovered by @dnet.

## Comments

### Comment 1 - antoniofrighetto

@dnet Out of curiosity, how was this found if you could elaborate?

---

### Comment 2 - dnet

@antoniofrighetto I was fuzzing the Alive2 project

---

