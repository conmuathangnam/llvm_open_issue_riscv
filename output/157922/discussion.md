# [HLSL] List Init expects an intializer for unnamed bitfield

**Author:** spall
**URL:** https://github.com/llvm/llvm-project/issues/157922
**Status:** Closed
**Labels:** clang:frontend, HLSL
**Closed Date:** 2025-09-23T16:35:47Z

## Body

```
struct EmptyStruct {
    int : 10;
};

[numthreads(1,1,1)]
void main() {
  EmptyStruct A = {1}; // to demonstrate you aren't supposed to initialize
  EmptyStruct E = {}; // bug here (this should be accepted but currently generates an error)
}
```
https://godbolt.org/z/fWG14TbsW

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Sarah Spall (spall)

<details>
```
struct EmptyStruct {
    int : 10;
};

[numthreads(1,1,1)]
void main() {
  EmptyStruct A = {1}; // to demonstrate you aren't supposed to initialize
  EmptyStruct E = {}; // bug here (this should be accepted but currently generates an error)
}
```
https://godbolt.org/z/fWG14TbsW
</details>


---

