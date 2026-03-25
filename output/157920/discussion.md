# [HLSL] Zero sized struct causes crash in HLSL init list code

**Author:** spall
**URL:** https://github.com/llvm/llvm-project/issues/157920
**Status:** Closed
**Labels:** clang:frontend, crash, HLSL
**Closed Date:** 2025-09-23T21:05:43Z

## Body

```
struct EmptyStruct {
    
};

[numthreads(1,1,1)]
void main() {

EmptyStruct E = {};

}
```

https://godbolt.org/z/G6bna1h6a

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Sarah Spall (spall)

<details>
```
struct EmptyStruct {
    
};

[numthreads(1,1,1)]
void main() {

EmptyStruct E = {};

}
```

https://godbolt.org/z/G6bna1h6a
</details>


---

