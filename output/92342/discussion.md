# `__builtin_shufflevector` runs into an assertion if no integer expressions are passed

**Author:** tbaederr
**URL:** https://github.com/llvm/llvm-project/issues/92342
**Status:** Closed
**Labels:** good first issue, clang:frontend, crash
**Closed Date:** 2026-03-04T21:12:17Z

## Body

See https://godbolt.org/z/M5x5Kdnc3, which uses a test case adapted from `test/Sema/constant_builtins_vector.cpp`.

```console
clang++: /root/llvm-project/clang/include/clang/AST/Expr.h:4483: llvm::APSInt clang::ShuffleVectorExpr::getShuffleMaskIdx(const clang::ASTContext&, unsigned int) const: Assertion `(N < NumExprs - 2) && "Shuffle idx out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++2b <source>
1.	<source>:22:66: current parser token ';'
 #0 0x00000000039a1548 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39a1548)
 #1 0x000000000399f22c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x399f22c)
 #2 0x00000000038f0288 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #3 0x000072fd01a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x000072fd01a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x000072fd01a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x000072fd01a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x000072fd01a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x000072fd01a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
```

We should just check for this case in Sema and reject it.

## Comments

### Comment 1 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor has already been assigned to this issue. If you believe that no one is actually working on it despite an assignment, ping the person. After one week without a response, the assignee may be changed.
1. In the comments of this issue, request for it to be assigned to you, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 2 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Timm Baeder (tbaederr)

<details>
See https://godbolt.org/z/M5x5Kdnc3, which uses a test case adapted from `test/Sema/constant_builtins_vector.cpp`.

```console
clang++: /root/llvm-project/clang/include/clang/AST/Expr.h:4483: llvm::APSInt clang::ShuffleVectorExpr::getShuffleMaskIdx(const clang::ASTContext&amp;, unsigned int) const: Assertion `(N &lt; NumExprs - 2) &amp;&amp; "Shuffle idx out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++2b &lt;source&gt;
1.	&lt;source&gt;:22:66: current parser token ';'
 #<!-- -->0 0x00000000039a1548 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39a1548)
 #<!-- -->1 0x000000000399f22c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x399f22c)
 #<!-- -->2 0x00000000038f0288 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072fd01a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072fd01a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072fd01a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072fd01a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072fd01a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072fd01a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
```

We should just check for this case in Sema and reject it.
</details>


---

### Comment 3 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timm Baeder (tbaederr)

<details>
See https://godbolt.org/z/M5x5Kdnc3, which uses a test case adapted from `test/Sema/constant_builtins_vector.cpp`.

```console
clang++: /root/llvm-project/clang/include/clang/AST/Expr.h:4483: llvm::APSInt clang::ShuffleVectorExpr::getShuffleMaskIdx(const clang::ASTContext&amp;, unsigned int) const: Assertion `(N &lt; NumExprs - 2) &amp;&amp; "Shuffle idx out of range!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /opt/compiler-explorer/clang-assertions-trunk/bin/clang++ -gdwarf-4 -g -o /app/output.s -mllvm --x86-asm-syntax=intel -S --gcc-toolchain=/opt/compiler-explorer/gcc-snapshot -fcolor-diagnostics -fno-crash-diagnostics -std=c++2b &lt;source&gt;
1.	&lt;source&gt;:22:66: current parser token ';'
 #<!-- -->0 0x00000000039a1548 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x39a1548)
 #<!-- -->1 0x000000000399f22c llvm::sys::CleanupOnSignal(unsigned long) (/opt/compiler-explorer/clang-assertions-trunk/bin/clang+++0x399f22c)
 #<!-- -->2 0x00000000038f0288 CrashRecoverySignalHandler(int) CrashRecoveryContext.cpp:0:0
 #<!-- -->3 0x000072fd01a42520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x000072fd01a969fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x000072fd01a42476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x000072fd01a287f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x000072fd01a2871b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x000072fd01a39e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
```

We should just check for this case in Sema and reject it.
</details>


---

### Comment 4 - vmishelcs

Hello, I'd like to give this issue a try :)

---

### Comment 5 - vmishelcs

@tbaederr Hello! I believe I've located the source of the bug.

In `Sema::BuiltinShuffleVector(CallExpr *TheCall)` we do the following check.

```C++
QualType LHSType = TheCall->getArg(0)->getType();
QualType RHSType = TheCall->getArg(1)->getType();

// ...

if (TheCall->getNumArgs() == 2) {
  if (!RHSType->hasIntegerRepresentation() || RHSType->castAs<VectorType>()->getNumElements() != numElements)
    return ExprError(Diag(TheCall->getBeginLoc(),
                          diag::err_vec_builtin_incompatible_vector)
                     << TheCall->getDirectCallee()
                     << /*isMorethantwoArgs*/ false
                     << SourceRange(TheCall->getArg(1)->getBeginLoc(), TheCall->getArg(1)->getEndLoc()));
  // ...
```

This is done to "support unary `__builtin_shufflevector`" according to the comment written in that same method:

```C++
// Determine which of the following types of shufflevector we're checking:
// 1) unary, vector mask: (lhs, mask)
// 2) binary, scalar mask: (lhs, rhs, index, ..., index)
```

However, I'm a little confused as to why. According to the [clang 19.0.0 documentation](https://clang.llvm.org/docs/LanguageExtensions.html#langext-builtin-shufflevector), `__builtin_shufflevector` has the signature

```
__builtin_shufflevector(vec1, vec2, index1, index2, ...)
```

thus there should be no reason it's called with only 2 vectors provided. Could it be that whoever wrote this comment was confusing clang's `__builtin_shufflevector` with gcc's `__builtin_shuffle` described [here](https://gcc.gnu.org/onlinedocs/gcc-13.2.0/gcc/Vector-Extensions.html)?


---

### Comment 6 - tbaederr

It seems like this was introduced in a011002195b8e21eabc54f73f72156c6ae19dd1c, so seems intentional.

---

### Comment 7 - vmishelcs

@tbaederr So do we want to allow the programmer to use `__builtin_shufflevector` by writing

```C++
constexpr vector4char vector4charConst = {0, 1, 2, 3};
constexpr vector4int vector4intMask = {3, 2, 1, 0};

constexpr vector4char vectorShuffle = __builtin_shufflevector(vector4charConst, vector4intMask);
```

despite the documentation saying this is invalid? Because GCC doesn't allow this either. GCC's implementation expects that you provide two vector arguments followed by indices.

---

### Comment 8 - tbaederr

Even the documentation for `llvm::ShuffleVectorInst` mentions it only takes two input vectors, so I think it's safe to reject the "unary" version with only one vector.

If no tests break, there's an opportunity for cleanup up the `Sema::BuiltinShuffleVector()` function.

---

### Comment 9 - erichkeane

Unassigning as this has been inactive for quite a while.

---

### Comment 10 - wsehjk

Hi, is the issue still open? If so, I would like to give it a try. Thanks


---

### Comment 11 - wsehjk

Hi， sorry for the delay. I'm working on this and have reproduced the bug locally.

---

### Comment 12 - wsehjk

> Hey [@erichkeane](https://github.com/erichkeane) [@EugeneZelenko](https://github.com/EugeneZelenko) [@vmishelcs](https://github.com/vmishelcs) [@tbaederr](https://github.com/tbaederr) [@wsehjk](https://github.com/wsehjk) I would like to work on this issue, would someone assign it to me?

Hi, thank you for your interest. but I‘m working on this. The pr is still under reivew. 

---

### Comment 13 - Thibault-Monnier

This seems to have been fixed in release 22.

---

