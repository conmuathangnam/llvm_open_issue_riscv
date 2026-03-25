# Clang crashes on large invalid array initializer (Out of Memory)

**Author:** apbenson00
**URL:** https://github.com/llvm/llvm-project/issues/183095
**Status:** Closed
**Labels:** duplicate, clang:frontend, crash-on-invalid
**Closed Date:** 2026-02-24T19:33:01Z

## Body

## Overview
- bug was found from fuzzing
- crash on invalid
- Does not throw SIGKILL but gives full stack dump on CompilerExplorer for Out of Memory (OOM)

## Code
```c
y0[]= { [3766666666]7}
```
## CompilerExplorer
https://godbolt.org/z/9bxezsqfd

## Notes
- seems to crash on all previous versions of x86-64 clang from CompilerExplorer
- on my 64GB machine, attempted to allocate >50GB of space then crash

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Andy Benson (apbenson00)

<details>
## Overview
- bug was found from fuzzing
- crash on invalid
- Does not throw SIGKILL but gives full stack dump on CompilerExplorer for Out of Memory (OOM)

## Code
```c
y0[]= { [3766666666]7}
```
## CompilerExplorer
https://godbolt.org/z/9bxezsqfd

## Notes
- seems to crash on all previous versions of x86-64 clang from CompilerExplorer
- on my 64GB machine, attempted to allocate &gt;50GB of space then crash
</details>


---

