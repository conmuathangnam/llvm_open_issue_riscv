# [PowerPC] Wrong code at -O1

**Author:** XChy
**URL:** https://github.com/llvm/llvm-project/issues/171571
**Status:** Open
**Labels:** backend:PowerPC, miscompilation

## Body

Reduced tescase: [reduced.c](https://github.com/user-attachments/files/24070790/reduced.c)
The testcase passed the UBSan and ASan: https://godbolt.org/z/dd8Kz49Y5

Reproduce command:
```bash
clang -O1 --target=powerpc64le-unknown-linux-musl -static reduced.c
qemu-ppc64le a.out
clang -O2 --target=powerpc64le-unknown-linux-musl -static reduced.c
qemu-ppc64le a.out
```

Output:
```
BackSmith Checksum = 0x0000000000000005
BackSmith Checksum = 0x0000000000000000
```

clang version 22.0.0git (git@github.com:llvm/llvm-project.git 0e92beb0c0f050e1180c88aa31713f3d6ba9d34e)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: Hongyu Chen (XChy)

<details>
Reduced tescase: [reduced.c](https://github.com/user-attachments/files/24070790/reduced.c)
The testcase passed the UBSan and ASan: https://godbolt.org/z/dd8Kz49Y5

Reproduce command:
```bash
clang -O1 --target=powerpc64le-unknown-linux-musl -static reduced.c
qemu-ppc64le a.out
clang -O2 --target=powerpc64le-unknown-linux-musl -static reduced.c
qemu-ppc64le a.out
```

Output:
```
BackSmith Checksum = 0x0000000000000005
BackSmith Checksum = 0x0000000000000000
```

clang version 22.0.0git (git@<!-- -->github.com:llvm/llvm-project.git 0e92beb0c0f050e1180c88aa31713f3d6ba9d34e)
</details>


---

