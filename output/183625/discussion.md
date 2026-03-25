# opt failure

**Author:** gawbl
**URL:** https://github.com/llvm/llvm-project/issues/183625
**Status:** Closed
**Labels:** crash, llvm::vectorcombine
**Closed Date:** 2026-03-13T13:24:20Z

## Body

Running Clang-22 on an aarch64 Linux system:

% uudecode reduced.bc.uuencoded.txt
% /path/to/opt "-passes=lto<O3>" -o DOK.out reduced.bc -disable-output


[stacktrace.txt](https://github.com/user-attachments/files/25588507/stacktrace.txt)

[reduced.bc.uuencoded.txt](https://github.com/user-attachments/files/25588512/reduced.bc.uuencoded.txt)

## Comments

### Comment 1 - Nathiyaa-Sengodan

I would like to work on this. 

---

