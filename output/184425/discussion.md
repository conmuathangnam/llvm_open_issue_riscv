# [clang] bad diagnostic for `>>` instead of `>` at end of alias declaration

**Author:** zygoloid
**URL:** https://github.com/llvm/llvm-project/issues/184425
**Status:** Closed
**Labels:** good first issue, clang:frontend, clang:diagnostics
**Closed Date:** 2026-03-12T16:56:45Z

## Body

```c++
template<typename> struct X {};
using A = X<int>>;
```
[produces an incorrect diagnostic](https://godbolt.org/z/aE66PcjMP):
```console
<source>:2:18: error: expected ';' after alias declaration
    2 | using A = X<int>>;
      |                  ^
      |                  ;
```
Note that the diagnostic points at a `;` and says it expected a `;` there, and the fixit hint suggests inserting a `;` where there already is one. This would be correct if the diagnostic pointed one character earlier (at the second `>`).

It looks like this issue is specific to tokens like `>>` that get split into two `>` tokens when parsing.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Richard Smith (zygoloid)

<details>
```c++
template&lt;typename&gt; struct X {};
using A = X&lt;int&gt;&gt;;
```
[produces an incorrect diagnostic](https://godbolt.org/z/aE66PcjMP):
```console
&lt;source&gt;:2:18: error: expected ';' after alias declaration
    2 | using A = X&lt;int&gt;&gt;;
      |                  ^
      |                  ;
```
Note that the diagnostic points at a `;` and says it expected a `;` there, and the fixit hint suggests inserting a `;` where there already is one. This would be correct if the diagnostic pointed one character earlier (at the second `&gt;`).

It looks like this issue is specific to tokens like `&gt;&gt;` that get split into two `&gt;` tokens when parsing.
</details>


---

### Comment 2 - llvmbot


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

### Comment 3 - llvmbot


@llvm/issue-subscribers-good-first-issue

Author: Richard Smith (zygoloid)

<details>
```c++
template&lt;typename&gt; struct X {};
using A = X&lt;int&gt;&gt;;
```
[produces an incorrect diagnostic](https://godbolt.org/z/aE66PcjMP):
```console
&lt;source&gt;:2:18: error: expected ';' after alias declaration
    2 | using A = X&lt;int&gt;&gt;;
      |                  ^
      |                  ;
```
Note that the diagnostic points at a `;` and says it expected a `;` there, and the fixit hint suggests inserting a `;` where there already is one. This would be correct if the diagnostic pointed one character earlier (at the second `&gt;`).

It looks like this issue is specific to tokens like `&gt;&gt;` that get split into two `&gt;` tokens when parsing.
</details>


---

### Comment 4 - abhijeetsharma200

Hey, I would like to take this issue


---

### Comment 5 - shafik

> Hey, I would like to take this issue

We don't assign issues until there is a PR and we have checked it is moving in the correct direction but expressing your intent to work on it can be helpful to prevent duplicate efforts. 



---

### Comment 6 - luoliwoshang

Hi all,

I have submitted a proposed fix for this issue in [PR #184555](https://github.com/llvm/llvm-project/pull/184555).

Feedback is very welcome. Thank you!

---

