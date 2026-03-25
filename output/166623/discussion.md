# Missing changes for WG14 N3622

**Author:** AaronBallman
**URL:** https://github.com/llvm/llvm-project/issues/166623

## Body

8e60adcaafbd5b6859f54879496e017dd218e541 implemented part of the paper, but missed "shall not contain a definition of a modifiable object with static or thread storage duration" from the removed text. e.g., we still diagnose this:
```
inline void f(void) {
  static int x; // warning: on-constant static local variable in inline function may be different in different files
}
```
https://godbolt.org/z/PhK6YvYnP

This should be accepted in C2y mode and claimed as an extension in earlier language modes.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Aaron Ballman (AaronBallman)

<details>
8e60adcaafbd5b6859f54879496e017dd218e541 implemented part of the paper, but missed "shall not contain a definition of a modifiable object with static or thread storage duration" from the removed text. e.g., we still diagnose this:
```
inline void f(void) {
  static int x; // warning: on-constant static local variable in inline function may be different in different files
}
```
https://godbolt.org/z/PhK6YvYnP

This should be accepted in C2y mode and claimed as an extension in earlier language modes.
</details>


---

### Comment 2 - AaronBallman

I think this may be a good first issue. Basically, we need changes similar to the linked commit, but for `warn_static_local_in_extern_inline`

---

### Comment 3 - llvmbot


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

### Comment 4 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Aaron Ballman (AaronBallman)

<details>
8e60adcaafbd5b6859f54879496e017dd218e541 implemented part of the paper, but missed "shall not contain a definition of a modifiable object with static or thread storage duration" from the removed text. e.g., we still diagnose this:
```
inline void f(void) {
  static int x; // warning: on-constant static local variable in inline function may be different in different files
}
```
https://godbolt.org/z/PhK6YvYnP

This should be accepted in C2y mode and claimed as an extension in earlier language modes.
</details>


---

### Comment 5 - Pranjal095

Hi, I'd be delighted to work on this issue. I'll refer to the linked commit and ensure the required functionality is implemented.

---

### Comment 6 - shivam-dhir

Hi @AaronBallman , I would love to work on this issue

---

