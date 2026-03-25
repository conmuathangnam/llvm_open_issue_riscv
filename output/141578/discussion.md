# Incorrect result due to -new-struct-path-tbaa on strict-alias analysis

**Author:** dongjianqiang2
**URL:** https://github.com/llvm/llvm-project/issues/141578
**Status:** Open
**Labels:** clang:codegen, miscompilation, TBAA

## Body

Hi I'm trying to use the -new-struct-path-tbaa and find a deja testcase failed.
Reproducer: https://godbolt.org/z/sYG9rvz1T


## Comments

### Comment 1 - dongjianqiang2

cc @antoniofrighetto @kosarev 


---

### Comment 2 - llvmbot


@llvm/issue-subscribers-clang-codegen

Author: dong jianqiang (dongjianqiang2)

<details>
Hi I'm trying to use the -new-struct-path-tbaa and find a deja testcase failed.
Reproducer: https://godbolt.org/z/sYG9rvz1T

</details>


---

### Comment 3 - antoniofrighetto

From https://eel.is/c++draft/class.mem.general#29, I think reading a union member is only well-defined if both structs form a common initial sequence, which doesn't seem what is happening here, as `long_t` and `short_t` do not seem to share any common initial sequence. This should be the reason why InstCombine legitimately simplifies the memcpy over the temporary `short_t` object. Maybe let me tag a few folks to confirm this @rjmccall, @nikic.

---

### Comment 4 - frederick-vs-ja

The posted example seems to be reduced from libc++'s implementation of `basic_string`. So perhaps we need to fix libc++ if `-new-struct-path-tbaa` is considered doing the right thing currently.

---

### Comment 5 - rjmccall

Antonio is correct that `short_t` and `long_t` have a zero-length common initial sequence. The common initial sequence rule specified by the standard does not flatten nested structs, so we immediately get a mismatch between the bit-field `size_t is_short : 1;` and the non-bit-field `short_header h;`. Unfortunately, inlining `short_header` into `short_t` would not solve the problem because `size_t is_short : 1;` and `unsigned char is_short : 1;` are not technically layout-compatible, either. On a little-endian platform using SysV-like bit-field rules, this is only a formal difference, but the actual memory layouts would be completely different on a big-endian target that used the underlying type of the bit-field as the access width (as Windows does). (It's possible that libc++ protects against this in its actual code, making this only a formal problem in the reduction, but it serves to explain why the standard cannot guarantee compatibility here.) I don't think this code can easily be made compliant without losing the storage optimization (which I would assume is very important); libc++ might need to stop using bit-fields and just do byte-loads instead.

With that said, I believe clang doesn't actually model the common initial subsequence rule when emitting TBAA for struct member accesses, since it would make *all* struct accesses more conservative even when a union is not involved. Instead, it just uses relaxed (`char`) TBAA for all accesses where the path of the l-value includes a union member. The problem may be that libc++ is deriving a reference to the union member and then accessing that, which defeats clang's attempt to be conservative because the accesses to the members of the union member no longer involve a union in the immediate l-value expression. All the places that pull out `short_repr()` and `long_repr()` may be subject to this.

This directly touches on the general problem with aliasing under both standards, which is that they want to be relaxed about unions but nobody knows how to square that with abstracted accesses to union members.

---

