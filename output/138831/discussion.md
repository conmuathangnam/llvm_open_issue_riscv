# [CodeGen] llc silently ignores -{start|stop}-{after|before} options

**Author:** optimisan
**URL:** https://github.com/llvm/llvm-project/issues/138831
**Status:** Open
**Labels:** good first issue, llvm:codegen

## Body

Particularly, `llc -mtriple=amdgcn-- -start-after=dead-mi-elimination,4` should be running remaining passes in the pipeline after the last instance of dead-mi-elimination, but instead this runs no passes.

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

Author: Akshat Oke (optimisan)

<details>
Particularly, `llc -mtriple=amdgcn-- -start-after=dead-mi-elimination,4` should be running remaining passes in the pipeline after the last instance of dead-mi-elimination, but instead this runs no passes.
</details>


---

### Comment 3 - Sriharika1506

I want to work on it, please assign me!!!

---

### Comment 4 - arthur3336

@optimisan Hi! I dont wanna step on any toes but last activity was 2 months ago, is this fair game? are you still actively working on this @Sriharika1506 ? I can take over no pb


---

### Comment 5 - Harikaraja

@arthur3336  I am currently not working on it

---

### Comment 6 - optimisan

Assigned it to @arthur3336, thanks!

---

