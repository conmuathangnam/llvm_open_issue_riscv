# Bytecode Interpreter: Union fields are activated too eagerly

**Author:** tbaederr
**URL:** https://github.com/llvm/llvm-project/issues/134789
**Status:** Closed
**Labels:** clang:frontend, clang:bytecode
**Closed Date:** 2025-07-16T07:03:34Z

## Body

See for example: https://godbolt.org/z/453c61feh

How far up the hierarchy from a pointer we should activate is based on the syntactical structure, i.e. we need to get that information from the AST.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Timm Baeder (tbaederr)

<details>
See for example: https://godbolt.org/z/453c61feh

How far up the hierarchy from a pointer we should activate is based on the syntactical structure, i.e. we need to get that information from the AST.
</details>


---

