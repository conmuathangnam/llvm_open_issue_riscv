# [AArch64] Wrong code at -O0 since clang-18

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/173895
**Status:** Closed
**Labels:** backend:AArch64, miscompilation, release:backport
**Closed Date:** 2026-02-02T09:30:38Z

## Body

Testcase:
```c
#include <stdint.h>
#include <stdio.h>
#define BS_VEC(type, num) type __attribute__((vector_size(num * sizeof(type))))
#define BITCAST(T, F, arg)                                                     \
    ((union {                                                                  \
        F src;                                                                 \
        T dst                                                                  \
    })arg)                                                                     \
        .dst

BS_VEC(int16_t, 4) backsmith_pure_4()
{
    BS_VEC(uint8_t, 8) BS_TEMP_14 = -(BS_VEC(uint8_t, 8)){ 2, 8, 4, 9 };
    BS_VEC(uint8_t, 8) BS_TEMP_23 = { BS_TEMP_14[0] >= 0 && 8 };
    return BITCAST(BS_VEC(int16_t, 4), BS_VEC(uint8_t, 8), BS_TEMP_23);
}
int main()
{
    uint64_t BS_CHECKSUM = backsmith_pure_4()[0];
    printf("BackSmith Checksum = 0x%016llx\n", BS_CHECKSUM);
}
```

Reproducer commands:
```sh
> clang --target=aarch64-unknown-linux-musl -march=armv9-a+sve --gcc-toolchain=/opt/aarch64 --sysroot=/opt/aarch64/aarch64-unknown-linux-musl/sysroot -O0 -static a.c
> qemu-aarch64 a.out
BackSmith Checksum = 0x0000000000000000

> clang --target=aarch64-unknown-linux-musl -march=armv9-a+sve --gcc-toolchain=/opt/aarch64 --sysroot=/opt/aarch64/aarch64-unknown-linux-musl/sysroot -O1 -static a.c
> qemu-aarch64 a.out
BackSmith Checksum = 0x0000000000000001

> gcc a.c && a.out
BackSmith Checksum = 0x0000000000000001
```

The testcase passes UBSan and MSan: https://godbolt.org/z/v6rsKcfGv

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-aarch64

Author: Hongyu Chen (XChy)

<details>
Testcase:
```c
#include &lt;stdint.h&gt;
#include &lt;stdio.h&gt;
#define BS_VEC(type, num) type __attribute__((vector_size(num * sizeof(type))))
#define BITCAST(T, F, arg)                                                     \
    ((union {                                                                  \
        F src;                                                                 \
        T dst                                                                  \
    })arg)                                                                     \
        .dst

BS_VEC(int16_t, 4) backsmith_pure_4()
{
    BS_VEC(uint8_t, 8) BS_TEMP_14 = -(BS_VEC(uint8_t, 8)){ 2, 8, 4, 9 };
    BS_VEC(uint8_t, 8) BS_TEMP_23 = { BS_TEMP_14[0] &gt;= 0 &amp;&amp; 8 };
    return BITCAST(BS_VEC(int16_t, 4), BS_VEC(uint8_t, 8), BS_TEMP_23);
}
int main()
{
    uint64_t BS_CHECKSUM = backsmith_pure_4()[0];
    printf("BackSmith Checksum = 0x%016llx\n", BS_CHECKSUM);
}
```

Reproducer commands:
```sh
&gt; clang --target=aarch64-unknown-linux-musl -march=armv9-a+sve --gcc-toolchain=/opt/aarch64 --sysroot=/opt/aarch64/aarch64-unknown-linux-musl/sysroot -O0 -static a.c
&gt; qemu-aarch64 a.out
BackSmith Checksum = 0x0000000000000000

&gt; clang --target=aarch64-unknown-linux-musl -march=armv9-a+sve --gcc-toolchain=/opt/aarch64 --sysroot=/opt/aarch64/aarch64-unknown-linux-musl/sysroot -O1 -static a.c
&gt; qemu-aarch64 a.out
BackSmith Checksum = 0x0000000000000001

&gt; gcc a.c &amp;&amp; a.out
BackSmith Checksum = 0x0000000000000001
```

The testcase passes UBSan and MSan: https://godbolt.org/z/v6rsKcfGv
</details>


---

### Comment 2 - ItsGunjit

Could you assign me this issue?

---

### Comment 3 - davemgreen

There is a smaller example in https://godbolt.org/z/zor9MEcz9, showing that we are not zexting the value out of a vector extract correctly.

---

### Comment 4 - davemgreen

@ItsGunjit if you don't have anything already I will put together a fix for this, to make sure we don't trigger it on trunk and hopefully get it fixed on the branch.

---

### Comment 5 - davemgreen

/cherry-pick 0321f3eeee5cceddc2541046ee155863f5f59585

---

### Comment 6 - llvmbot

<!--IGNORE-->
Failed to cherry-pick: 0321f3eeee5cceddc2541046ee155863f5f59585

https://github.com/llvm/llvm-project/actions/runs/21584076983

Please manually backport the fix and push it to your github fork.  Once this is done, please create a [pull request](https://github.com/llvm/llvm-project/compare)

---

### Comment 7 - davemgreen

/cherry-pick 8302e8ae6694978806f94aca81cd31258db66 0321f3eeee5cceddc2541046ee155863f5f59585

---

### Comment 8 - llvmbot

/pull-request llvm/llvm-project#179188

---

