# Using `__builtin_counted_by_ref` on a struct defined within another struct crashes Clang

**Author:** kees
**URL:** https://github.com/llvm/llvm-project/issues/182575
**Status:** Closed
**Labels:** clang:codegen, crash
**Closed Date:** 2026-02-20T21:34:19Z

## Body

```C
#include <stdlib.h>

struct outer {
        struct inner {
                int counter;
                int ent[] __attribute__((__counted_by__(counter)));
        } *entries;
};

struct inner *test(int count);
struct inner *test(int count)
{
        struct inner *entries = malloc(sizeof(*entries) + count * sizeof(*entries->ent));
        if (entries)
                *__builtin_counted_by_ref(entries->ent) = count;
        return entries;
}
```

This crashes Clang, but only if `struct inner` is defined within another struct:

```
...
 #2 0x00005d5eab1e1b48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0    
 #3 0x000075c96dffb8d0 (/lib/x86_64-linux-gnu/libc.so.6+0x458d0)                                    
 #4 0x00005d5eab6a31bd clang::CodeGen::CodeGenFunction::EmitStoreOfScalar(llvm::Value*, clang::CodeGen::Address, bool, clang::QualType, clang::CodeGen::LValueBaseInfo, clang::CodeGen::TBAAAccessInfo, bool, bool) (/srv/built-compilers/llvm/x86-ceb3b4cea7fd424be738609c2d9938c846445ec6/install/bin/clang-23+0x2ece1bd)                                                                                     
 #5 0x00005d5eab6aff1f clang::CodeGen::CodeGenFunction::EmitStoreThroughLValue(clang::CodeGen::RValue, clang::CodeGen::LValue, bool) (/srv/built-compilers/llvm/x86-ceb3b4cea7fd424be738609c2d9938c846445ec6/install/bin/clang-23+0x2edaf1f)                                                                
 #6 0x00005d5eab734d5c (anonymous namespace)::ScalarExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprScalar.cpp:0:0
 #7 0x00005d5eab72f8ce clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/srv/built-compilers/llvm/x86-ceb3b4cea7fd424be738609c2d9938c846445ec6/install/bin/clang-23+0x2f5a8ce)
...
```

cc @bwendling 

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: Kees Cook (kees)

<details>
```C
#include &lt;stdlib.h&gt;

struct outer {
        struct inner {
                int counter;
                int ent[] __attribute__((__counted_by__(counter)));
        } *entries;
};

struct inner *test(int count);
struct inner *test(int count)
{
        struct inner *entries = malloc(sizeof(*entries) + count * sizeof(*entries-&gt;ent));
        if (entries)
                *__builtin_counted_by_ref(entries-&gt;ent) = count;
        return entries;
}
```

This crashes Clang, but only if `struct inner` is defined within another struct:

```
...
 #<!-- -->2 0x00005d5eab1e1b48 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0    
 #<!-- -->3 0x000075c96dffb8d0 (/lib/x86_64-linux-gnu/libc.so.6+0x458d0)                                    
 #<!-- -->4 0x00005d5eab6a31bd clang::CodeGen::CodeGenFunction::EmitStoreOfScalar(llvm::Value*, clang::CodeGen::Address, bool, clang::QualType, clang::CodeGen::LValueBaseInfo, clang::CodeGen::TBAAAccessInfo, bool, bool) (/srv/built-compilers/llvm/x86-ceb3b4cea7fd424be738609c2d9938c846445ec6/install/bin/clang-23+0x2ece1bd)                                                                                     
 #<!-- -->5 0x00005d5eab6aff1f clang::CodeGen::CodeGenFunction::EmitStoreThroughLValue(clang::CodeGen::RValue, clang::CodeGen::LValue, bool) (/srv/built-compilers/llvm/x86-ceb3b4cea7fd424be738609c2d9938c846445ec6/install/bin/clang-23+0x2edaf1f)                                                                
 #<!-- -->6 0x00005d5eab734d5c (anonymous namespace)::ScalarExprEmitter::VisitBinAssign(clang::BinaryOperator const*) CGExprScalar.cpp:0:0
 #<!-- -->7 0x00005d5eab72f8ce clang::CodeGen::CodeGenFunction::EmitScalarExpr(clang::Expr const*, bool) (/srv/built-compilers/llvm/x86-ceb3b4cea7fd424be738609c2d9938c846445ec6/install/bin/clang-23+0x2f5a8ce)
...
```

cc @<!-- -->bwendling 
</details>


---

