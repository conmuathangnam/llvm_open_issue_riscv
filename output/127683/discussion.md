# `volatile` writes are merged for volatile base class subobjects

**Author:** eisenwave
**URL:** https://github.com/llvm/llvm-project/issues/127683
**Status:** Closed
**Labels:** c++, clang:codegen, miscompilation, confirmed
**Closed Date:** 2025-03-19T08:09:36Z

## Body

https://godbolt.org/z/ozEexb66j
```cpp
struct Base {
    int c;
};

struct Obj : Base { };

volatile Obj o;

void awoo() {
    o.c = 1;
    o.c = 2;
    o.c = 3;
}
```
## Expected output (GCC)

```asm
awoo():
        mov     DWORD PTR o[rip], 1
        mov     DWORD PTR o[rip], 2
        mov     DWORD PTR o[rip], 3
        ret
```

## Actual output (Clang)

```asm
awoo():
        mov     dword ptr [rip + o], 3
        ret
```

Note: `o.c` is a [volatile object](https://eel.is/c++draft/basic.type.qualifier#def:object,volatile), so why are its writes getting coalesced? This doesn't happen when `Base` is a data member of `Obj`, so this is almost certainly unintentional.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-c-1

Author: Jan Schultke (Eisenwave)

<details>
https://godbolt.org/z/ozEexb66j
```cpp
struct Base {
    int c;
};

struct Obj : Base { };

volatile Obj o;

void awoo() {
    o.c = 1;
    o.c = 2;
    o.c = 3;
}
```
## Expected output (GCC)

```asm
awoo():
        mov     DWORD PTR o[rip], 1
        mov     DWORD PTR o[rip], 2
        mov     DWORD PTR o[rip], 3
        ret
```

## Actual output (Clang)

```asm
awoo():
        mov     dword ptr [rip + o], 3
        ret
```

Note: `o.c` is a [volatile object](https://eel.is/c++draft/basic.type.qualifier#def:object,volatile), so why are its writes getting coalesced? This doesn't happen when `Base` is a data member of `Obj`, so this is almost certainly unintentional.
</details>


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Jan Schultke (Eisenwave)

<details>
https://godbolt.org/z/ozEexb66j
```cpp
struct Base {
    int c;
};

struct Obj : Base { };

volatile Obj o;

void awoo() {
    o.c = 1;
    o.c = 2;
    o.c = 3;
}
```
## Expected output (GCC)

```asm
awoo():
        mov     DWORD PTR o[rip], 1
        mov     DWORD PTR o[rip], 2
        mov     DWORD PTR o[rip], 3
        ret
```

## Actual output (Clang)

```asm
awoo():
        mov     dword ptr [rip + o], 3
        ret
```

Note: `o.c` is a [volatile object](https://eel.is/c++draft/basic.type.qualifier#def:object,volatile), so why are its writes getting coalesced? This doesn't happen when `Base` is a data member of `Obj`, so this is almost certainly unintentional.
</details>


---

### Comment 3 - nikic

Initial IR already doesn't have volatile stores (https://godbolt.org/z/76x43Gs94), so this is a clang codegen issue.

---

### Comment 4 - AaronBallman

@efriedma-quic -- do you think this might be a good first issue? (If so, can you add the tag and leave some high-level comments explaining what needs to be done?)

---

### Comment 5 - Icaro-Nunes

@AaronBallman If it's the case I'd like to tackle this issue!

---

### Comment 6 - antoniofrighetto

I think the issue here is occurring while generating the final LValue during derived-to-base class conversion:
https://github.com/llvm/llvm-project/blob/761d422441c0e6822abb233d339fab4f3cf7f60a/clang/lib/CodeGen/CGExpr.cpp#L5390-L5407

The LValue for the derived `volatile Obj` correctly encompasses the volatile qualifier, however, the resulting LV for the base class type misses it. Not completely sure here though, intuitively, it could make sense to continue propagating the qualifier:
```diff
--- a/clang/lib/CodeGen/CGExpr.cpp
+++ b/clang/lib/CodeGen/CGExpr.cpp
@@ -5403,8 +5403,12 @@ LValue CodeGenFunction::EmitCastLValue(const CastExpr *E) {
     // TODO: Support accesses to members of base classes in TBAA. For now, we
     // conservatively pretend that the complete object is of the base class
     // type.
-    return MakeAddrLValue(Base, E->getType(), LV.getBaseInfo(),
-                          CGM.getTBAAInfoForSubobject(LV, E->getType()));
+    LValue CastedLV =
+        MakeAddrLValue(Base, E->getType(), LV.getBaseInfo(),
+                       CGM.getTBAAInfoForSubobject(LV, E->getType()));
+    if (LV.isVolatile())
+      CastedLV.getQuals().addVolatile();
+    return CastedLV;
   }
   case CK_ToUnion:
```

---

