# [clang] Clang-22 miscompiles OpenSSH 10.x's mlkem768 key-exchange protocol on ARM when neon is enabled

**Author:** iucoen
**URL:** https://github.com/llvm/llvm-project/issues/185677
**Status:** Closed
**Labels:** backend:ARM, miscompilation, release:backport
**Closed Date:** 2026-03-25T11:38:15Z

## Body

Clang 22 has a regression where compiling OpenSSH 10.x for arm with neon enabled produces ssh client binary is unable to establish SSH connections and reports "incorrect signature" when the "mlkem768x25519-sha256" kex algorithm is used. I've narrowed down to the libcrux_ml_kem_mlkem768_portable_decapsulate() function that's misbehaving and returning the wrong shared secret. I've isolated just the mlkem768 algorithm and produced a small [test case](https://github.com/iucoen/clang-mlkem768-repro) to demonstrate the problem.

I did a git bisect and it pointed to this [commit](https://github.com/llvm/llvm-project/commit/1f78f6a2d629b0dd410a3d20429f76377ac5d6c3#diff-508a379c63df50d04c33ed3ff70ab4e7b5db5d72767ecb5d398df9cef300ef03) being the one causing the regression.

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-arm

Author: None (iucoen)

<details>
Clang 22 has a regression where compiling OpenSSH 10.x for arm with neon enabled produces ssh client binary is unable to establish SSH connections and reports "incorrect signature" when the "mlkem768x25519-sha256" kex algorithm is used. I've narrowed down to the libcrux_ml_kem_mlkem768_portable_decapsulate() function that's misbehaving and returning the wrong shared secret. I've isolated just the mlkem768 algorithm and produced a small [test case](https://github.com/iucoen/clang-mlkem768-repro) to demonstrate the problem.

I did a git bisect and it pointed to this [commit](https://github.com/llvm/llvm-project/commit/1f78f6a2d629b0dd410a3d20429f76377ac5d6c3#diff-508a379c63df50d04c33ed3ff70ab4e7b5db5d72767ecb5d398df9cef300ef03) being the one causing the regression.
</details>


---

### Comment 2 - davemgreen

There is a smaller example in https://godbolt.org/z/z5cz4PME9. The postincs on the ld1/ld2 are incorrect, and are not accounting for the original offset.

---

### Comment 3 - davemgreen

/cherry-pick 5012b76ae273790340283084a3d0de96aeb54866

---

### Comment 4 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: 5012b76ae273790340283084a3d0de96aeb54866

https://github.com/llvm/llvm-project/actions/runs/23537936244

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 5 - davemgreen

/cherry-pick bcdb7d3c8f3fb554938185d1f286941651864664 5012b76ae273790340283084a3d0de96aeb54866

---

### Comment 6 - llvmbot

/pull-request llvm/llvm-project#188466

---

