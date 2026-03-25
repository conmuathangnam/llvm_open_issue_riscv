# [HLSL][RootSignature] Incorrectly allows specifying parameters without a comma

**Author:** inbelic
**URL:** https://github.com/llvm/llvm-project/issues/147337
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-07-10T17:52:21Z

## Body

This issue tracks fixing a bug that allows parameters to be specified without an intermediate comma.

For instance:
```
RootFlags(0) CBV(b0)
```

Should be invalid because there is no comma between the two parameters.

This is because of the code pattern as follows:

https://github.com/llvm/llvm-project/blob/3c76a054ac4c7c93b197f3f1aec6c9f72ece1377/clang/lib/Parse/ParseHLSLRootSignature.cpp#L30-L42

It should be an else-if chain so that if we successfully parse the first element type, it will not attempt to parse any other element types right after.

Reproduction here: https://godbolt.org/z/arcExM9rE

AC:
- [ ] All occurrences of the if chains should be replaced with an else-if chain
- [ ] Add unit tests for this scenario

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Finn Plummer (inbelic)

<details>
This issue tracks fixing a bug that allows parameters to be specified without an intermediate comma.

For instance:
```
RootFlags(0) CBV(b0)
```

Should be invalid because there is no comma between the two parameters.

This is because of the code pattern as follows:

https://github.com/llvm/llvm-project/blob/3c76a054ac4c7c93b197f3f1aec6c9f72ece1377/clang/lib/Parse/ParseHLSLRootSignature.cpp#L30-L42

It should be an else-if chain so that if we successfully parse the first element type, it will not attempt to parse any other element types right after.

Reproduction here: https://godbolt.org/z/arcExM9rE

AC:
- [ ] All occurrences of the if chains should be replaced with an else-if chain
- [ ] Add unit tests for this scenario
</details>


---

