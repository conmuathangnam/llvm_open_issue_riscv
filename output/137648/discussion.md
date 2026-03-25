# Missing dependency from clang-scan-deps after b768a8c1db85b9e84fd8b356570a3a8fbe37acf6

**Author:** tru
**URL:** https://github.com/llvm/llvm-project/issues/137648
**Status:** Closed
**Labels:** clang:frontend
**Closed Date:** 2025-06-27T10:21:22Z

## Body

Hi @jansvoboda11 

I know I am late on the ball here, but we have been using clang-scan-deps from LLVM 18 until I recently started to upgrade to 19 and the 20. We use clang-scan-deps as part of our build system to figure out the dependencies of C++ files to build. When I started to integrate test LLVM 19 I noticed that clang-scan-deps was missing output a specific header. This header is included inside a `#ifdef` statement, but there something else going on as well.

I bisected the problem to b768a8c1db85b9e84fd8b356570a3a8fbe37acf6 and the problem exists within `main` as well. And the issue only exists with the non-preprocessor mode, the preprocessor mode works fine (but is to slow).

The problem here is that I don't know exactly how to make a reproducible issue out of this. the include tree is pretty complicated and the source is not something I can share. 

Let me know how I can provide you with more information or how to debug this issue further.

## Comments

### Comment 1 - tru

I don't think it's clang-tools-extra? It's in clang/lib/Tooling/DependencyScanning

---

### Comment 2 - jansvoboda11

Hi, I don't think I can really help without a reproducer. Some things to consider while making one:
* Does your project use some other file extensions that the ones shown in the commit diff? If so, these would previously not be scanned for dependency directives and instead handed to the preprocessor as-is.
* You can remove all AST contents from the files involved, that shouldn't play a role in a dependency scan and might make it easier for you to share a reproducer.
* I'd also consider removing includes/imports that don't seem to play a role in this bug.

---

### Comment 3 - tru

> Hi, I don't think I can really help without a reproducer. Some things to consider while making one:
> * Does your project use some other file extensions that the ones shown in the commit diff? If so, these would previously not be scanned for dependency directives and instead handed to the preprocessor as-is.

Ah yes. The file that includes the header has the extension .inl - could that explain the difference here? Short of renaming the file (it's in a third party library) is there some workaround?



---

### Comment 4 - jansvoboda11

Yup, file with an `.inl` extension would be preprocessed as-is prior to that commit but go through the dependency directives scan with that commit. I can't think of a workaround on the clang-scan-deps side, unfortunately.

---

### Comment 5 - jansvoboda11

Actually, the scanner bypasses the dependency directives scan for files in the module cache directory. If you set `-fmodules-cache-path=` to the parent of your `.inl` file, that _might_ work around the issue.

---

### Comment 6 - tru

We have our own fork of clang internally - what list of extensions is the scanner working out of now? I can always patch that in on our side until we can ask for a change on the library side. 

---

### Comment 7 - jansvoboda11

The scanner doesn't have a list of extensions - it performs the dependency directives scan on-demand for all files the lexer enters.

---

### Comment 8 - tru

> The scanner doesn't have a list of extensions - it performs the dependency directives scan on-demand for all files the lexer enters.

Aha. But then it's quite confusing - wonder why it won't find the dependency but the pre processor does?

Do I understand the problem correctly that the problem here is not that it misses or ignores the inl file, it was just that before LLVM 19 it scanned the file with the full preprocessor instead of the faster method and that managed to find the dependency. Hence there must be a bug in the fast lexical scanner that doesn't find the dependency and the inl extension is just a happy accident that it worked before? 

I will try to reproduce the issue with a smaller set of headers tomorrow in that case.

Other than that - do you have any suggestions as to how to debug the dependency scanner to understand why it won't find the header?


---

### Comment 9 - tru

Alright, I managed to find a reproducer that I can share. There is a weird construct at the start of the .inl file that seems to mess the fast path up - here is what it looks like:

```cpp
#if 0
R\
"exclude1(";
R"exclude2()exclude1";
#endif

#include <sub/this_must_be_included.h>

#if 0
//)exclude2";
#endif
```

this works in preprocessed mode but not in the fast mode. It also works if I roll back to LLVM 18 and name the file .inl.

---

### Comment 10 - tru

Interestingly enough clangd seems to misunderstand this as well and also marks the code between the first if and the last endif as inactive - are they sharing the same parser for this? cc @HighCommander4 

---

### Comment 11 - jansvoboda11

Wow. Thanks a lot for the reproducer! CC @cyndyishida

---

### Comment 12 - HighCommander4

> Interestingly enough clangd seems to misunderstand this as well and also marks the code between the first if and the last endif as inactive

Hmm, I don't see that -- the middle part with the include is marked as active for me. Could you post [clangd logs](https://clangd.llvm.org/troubleshooting.html#gathering-logs)?

---

### Comment 13 - tru

@cyndyishida any chance you have time to look at this soon? Otherwise I'll start to dig in to try to fix it. 

---

### Comment 14 - tru

Just a quick update - I have a fix for this locally, I am getting it reviewed internally and doing some real tests then I will send a PR.

---

### Comment 15 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Tobias Hieta (tru)

<details>
Hi @<!-- -->jansvoboda11 

I know I am late on the ball here, but we have been using clang-scan-deps from LLVM 18 until I recently started to upgrade to 19 and the 20. We use clang-scan-deps as part of our build system to figure out the dependencies of C++ files to build. When I started to integrate test LLVM 19 I noticed that clang-scan-deps was missing output a specific header. This header is included inside a `#ifdef` statement, but there something else going on as well.

I bisected the problem to b768a8c1db85b9e84fd8b356570a3a8fbe37acf6 and the problem exists within `main` as well. And the issue only exists with the non-preprocessor mode, the preprocessor mode works fine (but is to slow).

The problem here is that I don't know exactly how to make a reproducible issue out of this. the include tree is pretty complicated and the source is not something I can share. 

Let me know how I can provide you with more information or how to debug this issue further.
</details>


---

