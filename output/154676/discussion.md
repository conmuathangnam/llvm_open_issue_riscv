# Suspicious lit test output for function parameter

**Author:** MythreyaK
**URL:** https://github.com/llvm/llvm-project/issues/154676

## Body

Need to check why `foo2` is broken. But autocomplete seems to work fine :thinking:

```
foo2(float a) vs   [incorrect?]
foo2(<#float a#>)  [expected?]
```

_Originally posted by @MythreyaK in https://github.com/llvm/llvm-project/pull/154041#discussion_r2281844547_
            

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Mythreya Kuricheti (MythreyaK)

<details>
Need to check why `foo2` is broken. But autocomplete seems to work fine :thinking:

```
foo2(float a) vs   [incorrect?]
foo2(&lt;#float a#&gt;)  [expected?]
```

_Originally posted by @<!-- -->MythreyaK in https://github.com/llvm/llvm-project/pull/154041#discussion_r2281844547_
            
</details>


---

