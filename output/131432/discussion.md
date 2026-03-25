# Placement new can modify const-qualified variables

**Author:** tbaederr
**URL:** https://github.com/llvm/llvm-project/issues/131432
**Status:** Closed
**Labels:** good first issue, clang:frontend, constexpr
**Closed Date:** 2025-03-25T19:39:13Z

## Body

Consider:
```c++
namespace std {
  using size_t = decltype(sizeof(0));
}

void *operator new(std::size_t, void *p) { return p; }
void* operator new[] (std::size_t, void* p) {return p;}

constexpr int foo() {
  const int a = 10;

  new ((int*)&a) int(12);

  return a;
}
static_assert(foo() == 12);
```
https://godbolt.org/z/7jj8YsE8r

This should not be accepted of course.

This might be a good first issue, I'm not sure. Maybe @cor3ntin can comment on that.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timm Baeder (tbaederr)

<details>
Consider:
```c++
namespace std {
  using size_t = decltype(sizeof(0));
}

void *operator new(std::size_t, void *p) { return p; }
void* operator new[] (std::size_t, void* p) {return p;}

constexpr int foo() {
  const int a = 10;

  new ((int*)&amp;a) int(12);

  return a;
}
static_assert(foo() == 12);
```
https://godbolt.org/z/7jj8YsE8r

This should not be accepted of course.

This might be a good first issue, I'm not sure. Maybe @<!-- -->cor3ntin can comment on that.
</details>


---

### Comment 2 - tbaederr

On the positive side, `mutable` works:
```c++
struct S {
  mutable int a;
};

constexpr int foo() {

  const S s{10};

  new ((int*)&s.a) int(12);

  return s.a;
}
static_assert(foo() == 12);
```

---

### Comment 3 - shafik

Looks like a clang-20 regression: https://godbolt.org/z/ErnanEjGf

Just a good habit to get into to always see how far back and issue goes.

---

### Comment 4 - shafik

This looks specific to C++26: https://godbolt.org/z/d33qY94Ko

Which is not what I expected.

---

### Comment 5 - tbaederr

It also works before that if you use the `construct_at` special case instead: https://godbolt.org/z/esoYbf73b

Also works in older clangs: https://godbolt.org/z/xoa4EoYWo

---

### Comment 6 - llvmbot


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

### Comment 7 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Timm Baeder (tbaederr)

<details>
Consider:
```c++
namespace std {
  using size_t = decltype(sizeof(0));
}

void *operator new(std::size_t, void *p) { return p; }
void* operator new[] (std::size_t, void* p) {return p;}

constexpr int foo() {
  const int a = 10;

  new ((int*)&amp;a) int(12);

  return a;
}
static_assert(foo() == 12);
```
https://godbolt.org/z/7jj8YsE8r

This should not be accepted of course.

This might be a good first issue, I'm not sure. Maybe @<!-- -->cor3ntin can comment on that.
</details>


---

### Comment 8 - cor3ntin

We should check that `SubobjType` is not constant here

https://github.com/llvm/llvm-project/blob/f4043f451d0e8c30c8a9826ce87a6e76f3ace468/clang/lib/AST/ExprConstant.cpp#L10422-L10423

---

### Comment 9 - imdj

I'd like to work on this, is it possible to assign it to me?

---

### Comment 10 - cor3ntin

@imdj Feel free to make a pr :) Thanks in advance

---

### Comment 11 - shafik

@cor3ntin git bisect points to 6e78aef646c22b7087cbf7939c8016f4f59614a1 as the cause for the regression



---

### Comment 12 - zygoloid

The relevant language rule here is [[basic.life]/12](https://eel.is/c++draft/basic.life#12): "Creating a new object within the storage that a const, complete object with static, thread, or automatic storage duration occupies, or within the storage that such a const object used to occupy before its lifetime ended, results in undefined behavior[.](https://eel.is/c++draft/basic.life#12.sentence-1)"

---

### Comment 13 - zygoloid

[It should probably also not be permitted to use constexpr placement new to create an object that doesn't transparently replace the original (though the language rules don't say that) -- then this would also be covered by [[basic.life]/9.3](https://eel.is/c++draft/basic.life#9). If non-transparent replacement is allowed, we would have a lot more work to do to make it work properly. I've mailed CWG about that.]

---

### Comment 14 - cor3ntin

@shafik, the feature was simply not supported in 19

---

