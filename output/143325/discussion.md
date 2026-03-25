# relocation diagnostics asserts in response to code that is broken for unrelated reasons #awesome

**Author:** ojhunt
**URL:** https://github.com/llvm/llvm-project/issues/143325
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-06-09T08:29:06Z

## Body

I made an error in one of my test cases and got a crash due to a null value. After much confusion I realized the problem is actually that I had redefined the same struct multiple times, and that's what caused the unexpected state.

The test case:

```cpp
struct Foo  {
  Foo(const Foo&);
  ~Foo();
};

struct Foo {
  Foo();
  int;
};

struct Wrapper {
  union {
    Foo p;
  } u;
};
static_assert(__builtin_is_cpp_trivially_relocatable(Wrapper));
```

The assertion is triggered under `DiagnoseNonTriviallyRelocatableReason` in this code:
```cpp
  if (!D->hasSimpleMoveConstructor() && !D->hasSimpleCopyConstructor()) {
    const auto *Decl = cast<CXXConstructorDecl>(
        LookupSpecialMemberFromXValue(SemaRef, D, /*Assign=*/false));
    if (Decl && Decl->isUserProvided())
      SemaRef.Diag(Loc, diag::note_unsatisfied_trait_reason)
          << diag::TraitNotSatisfiedReason::UserProvidedCtr
          << Decl->isMoveConstructor() << Decl->getSourceRange();
  }
```

The issue is the `cast< CXXConstructorDecl >` as `LookupSpecialMemberFromXValue` can return null. The `cast<>` is followed by a null check so it's presumably expected that `LookupSpecialMemberFromXValue` returns null, in which case the fix is likely just to replace `cast` with `cast_or_null`. If it's expected that the cast itself may fail then I assume there's `dyn_cast_or_null` that should be used.



## Comments

### Comment 1 - llvmbot


Hi!

This issue may be a good introductory issue for people new to working on LLVM. If you would like to work on this issue, your first steps are:

1. Check that no other contributor is working on this issue. If someone is assigned to the issue or claimed to be working on it, ping the person. After one week without a response, the assignee may be changed.
1. Leave a comment indicating that you are working on the issue, or just create a [pull request](https://github.com/llvm/llvm-project/pulls) after following the steps below. [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.
1. Fix the issue locally.
1. [Run the test suite](https://llvm.org/docs/TestingGuide.html#unit-and-regression-tests) locally. Remember that the subdirectories under `test/` create fine-grained testing targets, so you can e.g. use `make check-clang-ast` to only run Clang's AST tests.
1. Create a Git commit.
1. Run [`git clang-format HEAD~1`](https://clang.llvm.org/docs/ClangFormat.html#git-integration) to format your changes.
1. Open a [pull request](https://github.com/llvm/llvm-project/pulls) to the [upstream repository](https://github.com/llvm/llvm-project) on GitHub. Detailed instructions can be found [in GitHub's documentation](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/creating-a-pull-request). [Mention](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) this issue in the description of the pull request.

If you have any further questions about this issue, don't hesitate to ask via a comment in the thread below.



---

### Comment 2 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Oliver Hunt (ojhunt)

<details>
I made an error in one of my test cases and got a crash due to a null value. After much confusion I realized the problem is actually that I had redefined the same struct multiple times, and that's what caused the unexpected state.

The test case:

```cpp
struct Foo  {
  Foo(const Foo&amp;);
  ~Foo();
};

struct Foo {
  Foo();
  int;
};

struct Wrapper {
  union {
    Foo p;
  } u;
};
static_assert(__builtin_is_cpp_trivially_relocatable(Wrapper));
```

The assertion is triggered under `DiagnoseNonTriviallyRelocatableReason` in this code:
```cpp
  if (!D-&gt;hasSimpleMoveConstructor() &amp;&amp; !D-&gt;hasSimpleCopyConstructor()) {
    const auto *Decl = cast&lt;CXXConstructorDecl&gt;(
        LookupSpecialMemberFromXValue(SemaRef, D, /*Assign=*/false));
    if (Decl &amp;&amp; Decl-&gt;isUserProvided())
      SemaRef.Diag(Loc, diag::note_unsatisfied_trait_reason)
          &lt;&lt; diag::TraitNotSatisfiedReason::UserProvidedCtr
          &lt;&lt; Decl-&gt;isMoveConstructor() &lt;&lt; Decl-&gt;getSourceRange();
  }
```

The issue is the `cast&lt; CXXConstructorDecl &gt;` as `LookupSpecialMemberFromXValue` can return null. The `cast&lt;&gt;` is followed by a null check so it's presumably expected that `LookupSpecialMemberFromXValue` returns null, in which case the fix is likely just to replace `cast` with `cast_or_null`. If it's expected that the cast itself may fail then I assume there's `dyn_cast_or_null` that should be used.


</details>


---

### Comment 3 - ojhunt

pre-assigning to @cor3ntin because I blame him for everything, but this is a trivial fix that any one can do if they're wanting to learn how to setup a checkout and build configuration, learn how to add and run tests, etc

---

### Comment 4 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Oliver Hunt (ojhunt)

<details>
I made an error in one of my test cases and got a crash due to a null value. After much confusion I realized the problem is actually that I had redefined the same struct multiple times, and that's what caused the unexpected state.

The test case:

```cpp
struct Foo  {
  Foo(const Foo&amp;);
  ~Foo();
};

struct Foo {
  Foo();
  int;
};

struct Wrapper {
  union {
    Foo p;
  } u;
};
static_assert(__builtin_is_cpp_trivially_relocatable(Wrapper));
```

The assertion is triggered under `DiagnoseNonTriviallyRelocatableReason` in this code:
```cpp
  if (!D-&gt;hasSimpleMoveConstructor() &amp;&amp; !D-&gt;hasSimpleCopyConstructor()) {
    const auto *Decl = cast&lt;CXXConstructorDecl&gt;(
        LookupSpecialMemberFromXValue(SemaRef, D, /*Assign=*/false));
    if (Decl &amp;&amp; Decl-&gt;isUserProvided())
      SemaRef.Diag(Loc, diag::note_unsatisfied_trait_reason)
          &lt;&lt; diag::TraitNotSatisfiedReason::UserProvidedCtr
          &lt;&lt; Decl-&gt;isMoveConstructor() &lt;&lt; Decl-&gt;getSourceRange();
  }
```

The issue is the `cast&lt; CXXConstructorDecl &gt;` as `LookupSpecialMemberFromXValue` can return null. The `cast&lt;&gt;` is followed by a null check so it's presumably expected that `LookupSpecialMemberFromXValue` returns null, in which case the fix is likely just to replace `cast` with `cast_or_null`. If it's expected that the cast itself may fail then I assume there's `dyn_cast_or_null` that should be used.


</details>


---

### Comment 5 - ojhunt

I was looking at my original test case and the duplicate definition failure did not exist, with a bunch of trimming:

```cpp
struct Polymorphic  {
  virtual ~Polymorphic();
};

struct UnionOfPolymorphic {
  union {
    Polymorphic p;
    int i;
  } u;
};

static_assert(__builtin_is_cpp_trivially_relocatable(UnionOfPolymorphic));

```

This triggers the assertion even though there's no error in the code. Note that the static_assert is not necessarily reflecting the actual expected result, but is what is necessary to trigger the diagnostic path.

---

### Comment 6 - cor3ntin

Yeah, I'll look at that today

---

