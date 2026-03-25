# clangd crashes when attempting to parse code

**Author:** thatrandomguy43
**URL:** https://github.com/llvm/llvm-project/issues/179555
**Status:** Closed
**Labels:** clangd, crash
**Closed Date:** 2026-02-04T20:26:36Z

## Body

[stack_trace.txt](https://github.com/user-attachments/files/25057252/stack_trace.txt)

clangd repeatedly crashes whenever attempting to parse my code. I'm using it through the llvm-vs-code-extensions.vscode-clangd extension. I have no idea what the problem could be, and I would not have filed a report if it wasn't for the error printout directing me here. I don't think I was doing anything particularly unusual.

[The code this happened on, if you care](https://gitlab.com/asgerrr/gravity_simulation/-/commit/3281698428cad8728ba4b088ff4abb1fd4da7d26)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clangd

Author: Asger (thatrandomguy43)

<details>
[stack_trace.txt](https://github.com/user-attachments/files/25057252/stack_trace.txt)

clangd repeatedly crashes whenever attempting to parse my code. I'm using it through the llvm-vs-code-extensions.vscode-clangd extension. I have no idea what the problem could be, and I would not have filed a report if it wasn't for the error printout directing me here. I don't think I was doing anything particularly unusual.

[The code this happened on, if you care](https://gitlab.com/asgerrr/gravity_simulation/-/commit/3281698428cad8728ba4b088ff4abb1fd4da7d26)
</details>


---

### Comment 2 - HighCommander4

Thanks for the report.

A couple of questions:

 1. Could you try with the the latest weekly snapshot build from https://github.com/clangd/clangd/releases/tag/snapshot_20260125 and see if the crash still occurs?
 2. Do you, or can you, build your project with clang as the compiler, and if so, does the crash occur when using the same clang version as a crashing clangd (for example, 21.1.8)?

---

### Comment 3 - thatrandomguy43

1. Yes, that does actually make it not crash, thanks.
2. Yes, I do use clang as my compiler, and trying to compile it does also crash. I hadn't tried to compile because I was in the middle of changing some code that would not make sense to run.

---

### Comment 4 - HighCommander4

Thanks for checking! Based on those findings, it looks like this was a bug in the clang frontend (code shared by clang and clangd) which is already fixed in clang 22.

We're fairly close to the release of clang 22 (release candidates are already available), so I don't think it's worth spending time trying to identify the change the fixed this and seeking a backport to 21, so I'm going to close this.

---

