# `std::has_unique_object_representations_v<>` does not work correctly with `vector_size` attribute

**Author:** pkasting
**URL:** https://github.com/llvm/llvm-project/issues/119290

## Body

Clang supports the gcc `vector_size` attribute, but `std::has_unique_object_representations_v<>` seems to work incorrectly with it.

This attribute allows declaring a vector object of a given byte size, divided into integral or floating units. The total size must be a positive power of 2 multiple of the unit size, so there are no implicit padding bytes. Vector objects may be compared, and the result is another vector with element-wise comparison results. Representationally, the elements of the result vector are 0 if the comparison succeeded and -1 otherwise. So comparing `vec == vec` gives all zeroes.

Per cppreference, this type trait is supported for arbitrary objects, including arrays, which are considered equal if they are per-element equal. Vector objects would presumably be treated similarly. Therefore, I'd expect vectors to have a unique representation iff their base types are integral.

However, they aren't marked that way. Example: https://godbolt.org/z/r6Thv3v65

Here, there is a single-element vector whose base type is a uint32_t. It is correctly marked trivially-copyable. However, it is falsely marked as not having a unique obj representation.

I wonder if the strange comparison result for == (0 for ==, -1 for !=) is throwing things off. At least the cppreference text does not specify in terms of comparison results being zero/false/etc., but on the values being "the same". So I would assume there needs to be some custom handling of vector types in the compiler here.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-clang-frontend

Author: None (pkasting)

<details>
Clang supports the gcc `vector_size` attribute, but `std::has_unique_object_representations_v&lt;&gt;` seems to work incorrectly with it.

This attribute allows declaring a vector object of a given byte size, divided into integral or floating units. The total size must be a positive power of 2 multiple of the unit size, so there are no implicit padding bytes. Vector objects may be compared, and the result is another vector with element-wise comparison results. Representationally, the elements of the result vector are 0 if the comparison succeeded and -1 otherwise. So comparing `vec == vec` gives all zeroes.

Per cppreference, this type trait is supported for arbitrary objects, including arrays, which are considered equal if they are per-element equal. Vector objects would presumably be treated similarly. Therefore, I'd expect vectors to have a unique representation iff their base types are integral.

However, they aren't marked that way. Example: https://godbolt.org/z/r6Thv3v65

Here, there is a single-element vector whose base type is a uint32_t. It is correctly marked trivially-copyable. However, it is falsely marked as not having a unique obj representation.

I wonder if the strange comparison result for == (0 for ==, -1 for !=) is throwing things off. At least the cppreference text does not specify in terms of comparison results being zero/false/etc., but on the values being equal. So I would assume there needs to be some custom handling of vector types in the compiler here.
</details>


---

### Comment 2 - shafik

Looks like a known issue: https://github.com/llvm/llvm-project/blob/33b910cde3b305a49c98c6de88dbc22ced9dea61/clang/lib/AST/ASTContext.cpp#L2948-L2957

---

