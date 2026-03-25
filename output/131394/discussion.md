# ppc64le: Undefined temporary symbol .L_MergedGlobals

**Author:** tuliom
**URL:** https://github.com/llvm/llvm-project/issues/131394
**Status:** Closed
**Labels:** llvm:codegen, regression:20
**Closed Date:** 2025-04-02T08:40:55Z

## Body

This has been originally reported on Fedora Linux at: https://bugzilla.redhat.com/show_bug.cgi?id=2352554
It's impacting Chromium on ppc64le.
Can only be reproduced with LLVM >= 20.

Reduced test:

```
__attribute__((annotate(""))) int index_;
```

It fails with the following error:
```
$ clang++ --target=powerpc64le-linux-gnu -O2 -fPIC -pthread -std=c++20 -c test.cc -o test.o
error: Undefined temporary symbol .L_MergedGlobals
1 error generated.
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Tulio Magno Quites Machado Filho (tuliom)

<details>
This has been originally reported on Fedora Linux at: https://bugzilla.redhat.com/show_bug.cgi?id=2352554
It's impacting Chromium on ppc64le.
Can only be reproduced with LLVM &gt;= 20.

Reduced test:

```
__attribute__((annotate(""))) int index_;
```

It fails with the following error:
```
$ clang++ --target=powerpc64le-linux-gnu -O2 -fPIC -pthread -std=c++20 -c test.cc -o test.o
error: Undefined temporary symbol .L_MergedGlobals
1 error generated.
```
</details>


---

### Comment 2 - nikic

We should not try to merge globals in the llvm.metadata section. Unfortunately I'm having trouble reproducing this issue with a standalone global-merge invocation...

---

### Comment 3 - nikic

> Unfortunately I'm having trouble reproducing this issue with a standalone global-merge invocation...

That's because the option handling for the pass is broken in *so* many different ways.

---

### Comment 4 - nikic

https://github.com/llvm/llvm-project/pull/131801 to fix the issue here. https://github.com/llvm/llvm-project/commit/8f66fb784291c897a965a9ee4c280e314dc8cee4 to fix the handling for the constant related options.

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Tulio Magno Quites Machado Filho (tuliom)

<details>
This has been originally reported on Fedora Linux at: https://bugzilla.redhat.com/show_bug.cgi?id=2352554
It's impacting Chromium on ppc64le.
Can only be reproduced with LLVM &gt;= 20.

Reduced test:

```
__attribute__((annotate(""))) int index_;
```

It fails with the following error:
```
$ clang++ --target=powerpc64le-linux-gnu -O2 -fPIC -pthread -std=c++20 -c test.cc -o test.o
error: Undefined temporary symbol .L_MergedGlobals
1 error generated.
```
</details>


---

### Comment 6 - iskunk

In case it helps anyone:

I encountered this issue in building Chromium for ppc64el on Ubuntu 24.04/noble, which has LLVM 20.1.2. That version appears to pre-date the fix described above.

LLVM 21 is not available in Ubuntu noble, LLVM 19 required more Chromium patches, and I didn't want to bring in my own compiler build. So after some digging, I found that adding the following flags allows the build to complete successfully:
```
CXXFLAGS += -mllvm -enable-global-merge=FALSE
LDFLAGS  += -Wl,-mllvm,-enable-global-merge=FALSE
```

---

