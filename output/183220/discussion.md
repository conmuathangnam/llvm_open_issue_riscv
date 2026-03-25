# clang-21 -moutline failure

**Author:** gawbl
**URL:** https://github.com/llvm/llvm-project/issues/183220
**Status:** Closed
**Labels:** crash, llvm
**Closed Date:** 2026-02-25T18:04:12Z

## Body

Target: aarch64-unknown-linux-gnu

% /path/to/clang-21 -moutline -O1 -c -o QOK.s reduced.bc
...
Using Clang-21 release. Some kind of an assertion. Mentions "@OUTLINED_FUNCTION_0".

Decrypt attached bitcode file with something like "uudecode reduced.bc.uuencode.txt"

[stacktrace.txt](https://github.com/user-attachments/files/25533224/stacktrace.txt)

[reduced.bc.uuencode.txt](https://github.com/user-attachments/files/25533329/reduced.bc.uuencode.txt)



## Comments

### Comment 1 - EugeneZelenko

Could you please try 22 or `main` branch? Only most recent release is maintained.


---

### Comment 2 - gawbl

O.K., will try Clang-22. Closing this bug, will refile if Clang-22 is broken.

---

