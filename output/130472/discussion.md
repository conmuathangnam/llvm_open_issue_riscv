# llvm-objcopy regression: Invalid iOS Mach-O: __TEXT segment fileoffset is not zero

**Author:** TimNN
**URL:** https://github.com/llvm/llvm-project/issues/130472
**Status:** Closed
**Labels:** miscompilation, tools:llvm-objcopy/strip, release:backport
**Closed Date:** 2025-03-14T20:43:50Z

## Body

`llvm-objcopy good bad` on an iOS binary produces an output that is considered invalid by dyld.

`dyld_info bad` fails with:

```
bad [arm64]:
   __TEXT segment fileoffset is not zero
```

This regressed in https://github.com/llvm/llvm-project/pull/120995, identified by `git bisect` and verified by reverting https://github.com/llvm/llvm-project/commit/1a830aa1fe1e88749b563fefe18382842e0cff90. cc @drodriguez (**edit:** See https://github.com/llvm/llvm-project/pull/120995#issuecomment-2708707433 for what I believe that patch got wrong).

Steps to reproduce (sorry, don't have a version without Rust):

```shell
echo "fn main() {}" >> good.rs

# Rust version doesn't matter for this.
# Tested with the latest nightly & stable.
rustc --target=aarch64-apple-ios good.rs

llvm-objcopy good bad

# This is fine:
dyld_info good

# This fails:
dyld_info bad
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-tools-llvm-objcopy-strip

Author: Tim Neumann (TimNN)

<details>
`llvm-objcopy good bad` on an iOS binary produces an output that it considered invalid by dyld.

`dyld_info bad` fails with:

```
bad [arm64]:
   __TEXT segment fileoffset is not zero
```

This regressed in https://github.com/llvm/llvm-project/pull/120995, identified by `git bisect` and verified by reverting https://github.com/llvm/llvm-project/commit/1a830aa1fe1e88749b563fefe18382842e0cff90. cc @<!-- -->drodriguez (**edit:** See https://github.com/llvm/llvm-project/pull/120995#issuecomment-2708707433 for what I believe that patch got wrong).

Steps to reproduce (sorry, don't have a version without Rust):

```shell
echo "fn main() {}" &gt;&gt; good.rs

# Rust version doesn't matter for this.
# Tested with the latest nightly &amp; stable.
rustc --target=aarch64-apple-ios good.rs

llvm-objcopy good bad

# This is fine:
dyld_info good

# This fails:
dyld_info bad
```
</details>


---

### Comment 2 - nikic

/cherry-pick 8413f4d837a96458104f63bab72c751b8285a458

---

### Comment 3 - llvmbot

/pull-request llvm/llvm-project#131398

---

