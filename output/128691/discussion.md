# clang assert: "Assertion `getDepthAndIndex(NewParam).first == 0 && "Unexpected template parameter depth"' failed."

**Author:** hokein
**URL:** https://github.com/llvm/llvm-project/issues/128691
**Status:** Closed
**Labels:** clang:frontend, crash
**Closed Date:** 2025-02-26T08:54:20Z

## Body

https://godbolt.org/z/TPjf1YYY3

A regression from https://github.com/llvm/llvm-project/pull/111143

```
template <typename = void>
class NewDeleteAllocator;

template <>
struct NewDeleteAllocator<> {
  template <typename>
  NewDeleteAllocator();
};

template <typename>
struct NewDeleteAllocator : NewDeleteAllocator<> {
  using NewDeleteAllocator<>::NewDeleteAllocator;
};

void test() { NewDeleteAllocator abc; }
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: Haojian Wu (hokein)

<details>
https://godbolt.org/z/TPjf1YYY3

A regression from https://github.com/llvm/llvm-project/pull/111143

```
template &lt;typename = void&gt;
class NewDeleteAllocator;

template &lt;&gt;
struct NewDeleteAllocator&lt;&gt; {
  template &lt;typename&gt;
  NewDeleteAllocator();
};

template &lt;typename&gt;
struct NewDeleteAllocator : NewDeleteAllocator&lt;&gt; {
  using NewDeleteAllocator&lt;&gt;::NewDeleteAllocator;
};

void test() { NewDeleteAllocator abc; }
```
</details>


---

### Comment 2 - hokein

cc @zyn0217 

---

### Comment 3 - zyn0217

```
NewParam->dump()
TemplateTypeParmDecl 0x5555631a8e98 <128691.cpp:8:13> col:21 typename depth 32767 index 1
```

Which suggests the NewDepth we've calculated is wrong.

---

### Comment 4 - zyn0217

So... the constructor for which we're trying to synthesize a deduction guide comes from the explicit specialization `template <> struct NewDeleteAllocator<>`, which has an empty template parameter list. As a result, the function template (the constructor) lvies at depth 0. When building a deduction guide within a class template, we assume there's always at least one level of template depth and therefore use `Param->getDepth() - 1`, which overflows in this case.

(We should have been doing this wrong prior to 20, but it didn't crash because it called `Decl::getTemplateDepth()`, which returns 0 for the transformed template parameter declaration)

---

