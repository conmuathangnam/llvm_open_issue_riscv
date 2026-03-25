# Miscompilation when targeting mipsel with -static-pie

**Author:** Gelbpunkt
**URL:** https://github.com/llvm/llvm-project/issues/124681
**Status:** Open
**Labels:** backend:MIPS, miscompilation

## Body

I'm not 100% sure what is going on exactly, so here's what I'm doing for a minimal reproducer:

I have a very simple program, say `test.c`:

```c
#include <stdio.h>

int main() {
    printf("Hello world!\n");
    return 0;
}
```

I then compile it with clang for mipsel:

`mipsel-unknown-linux-musl-clang --sysroot ./sysroot/usr/mipsel-unknown-linux-musl/ -fuse-ld=lld -static test.c -o test -mips32r2`

...and run it with binfmt-misc using qemu-static-mipsel:

```
$ ./test
Hello world!
```

So far, so good. Now I try `-static-pie` instead of `-static`:

```
$ mipsel-unknown-linux-musl-clang --sysroot ./sysroot/usr/mipsel-unknown-linux-musl/ -fuse-ld=lld -static-pie test.c -o test -mips32r2
$ ./test
Hello world!
qemu: uncaught target signal 11 (Segmentation fault) - core dumped
fish: Job 1, './test' terminated by signal SIGSEGV (Address boundary error)
```

Something is fishy - so I put it in gdb:

![Image](https://github.com/user-attachments/assets/8d6332fd-4345-43fd-a162-76f5311dcca6)

Notice the `bal 0x55550000`. That address is in the middle of nowhere and not at instruction boundary. The jump triggers the fault. Removing the block for `__cxa_finalize` "fixes" the issue. 

```
$ llvm-readelf -Ws test | grep cxa_finalize
   143: 00000000     0 NOTYPE  WEAK   DEFAULT   UND __cxa_finalize
```

I wonder why this resolves to the weak symbol - maybe that is part of the issue? On x86_64 it does as well, but doesn't crash. It definitely is an issue with weak symbols I believe - I had to revert c22828991e7ca7b99048761c078252e94403ba6e to make it stop calling `__register_frame_info` (an unresolved weak symbol because it is not enabled by default in libunwind), calling which triggered basically the same issue that I'm describing here now.

This is all on LLVM 19.1.7 built with Chimera Linux' packaging infrastructure (i.e. a full LLVM toolchain).

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-mips

Author: Jens Reidel (Gelbpunkt)

<details>
I'm not 100% sure what is going on exactly, so here's what I'm doing for a minimal reproducer:

I have a very simple program, say `test.c`:

```c
#include &lt;stdio.h&gt;

int main() {
    printf("Hello world!\n");
    return 0;
}
```

I then compile it with clang for mipsel:

`mipsel-unknown-linux-musl-clang --sysroot ./sysroot/usr/mipsel-unknown-linux-musl/ -fuse-ld=lld -static test.c -o test -mips32r2`

...and run it with binfmt-misc using qemu-static-mipsel:

```
$ ./test
Hello world!
```

So far, so good. Now I try `-static-pie` instead of `-static`:

```
$ mipsel-unknown-linux-musl-clang --sysroot ./sysroot/usr/mipsel-unknown-linux-musl/ -fuse-ld=lld -static-pie test.c -o test -mips32r2
$ ./test
Hello world!
qemu: uncaught target signal 11 (Segmentation fault) - core dumped
fish: Job 1, './test' terminated by signal SIGSEGV (Address boundary error)
```

Something is fishy - so I put it in gdb:

![Image](https://github.com/user-attachments/assets/8d6332fd-4345-43fd-a162-76f5311dcca6)

Notice the `bal 0x55550000`. That address is in the middle of nowhere and not at instruction boundary. The jump triggers the fault. Removing the block for `__cxa_finalize` "fixes" the issue. 

```
$ llvm-readelf -Ws test | grep cxa_finalize
   143: 00000000     0 NOTYPE  WEAK   DEFAULT   UND __cxa_finalize
```

I wonder why this resolves to the weak symbol - maybe that is part of the issue? On x86_64 it does as well, but doesn't crash. It definitely is an issue with weak symbols I believe - I had to revert c22828991e7ca7b99048761c078252e94403ba6e to make it stop calling `__register_frame_info` (an unresolved weak symbol because it is not enabled by default in libunwind), calling which triggered basically the same issue that I'm describing here now.

This is all on LLVM 19.1.7 built with Chimera Linux' packaging infrastructure (i.e. a full LLVM toolchain).
</details>


---

### Comment 2 - Gelbpunkt

Update: My understanding of weak symbols was off. If I add `-Wl,--whole-archive`, `llvm-readelf` shows `  2209: 0006e1e8    36 FUNC    GLOBAL DEFAULT    12 __cxa_finalize`, which is what I would have expected. The miscompilation still occurs, though.

---

### Comment 3 - Gelbpunkt

This issue looks very closely related to https://github.com/llvm/llvm-project/commit/5bab291b7bd043104abf1ca7977e8248684cae95.

---

### Comment 4 - brad0

@yingopq @wzssyqa 

---

### Comment 5 - yingopq

Sorry, I could not reproduce this issue.
The cross lab was x86_64, and the running lab was mips64el-linux-gnuabi64.

```
huangyin@swecovm-s01:/swecoproj/huangying/llvm-project$ ./build/bin/clang --target=mipsel-linux-musl -fuse-ld=/swecoproj/huangying/llvm-project/mipsel-linux-muslsf-cross/bin/mipsel-linux-muslsf-ld -static-pie --sysroot=/usr/mipsel-linux-musl -msoft-float test.c -o test

huangying@Sleepygon:/tmp$ ./test 
Hello world!

```

---

### Comment 6 - Gelbpunkt

@yingopq sorry for taking so long to get back to you. I assume that in your case, no weak symbols are undefined. Here's an easy reproducer:

```c
extern void foo(void) __attribute__((weak));

int main(void) {
    if (foo)
        foo();
    return 0;
}
```

I can also reproduce this issue on mips64, like this:

```
root@cbuild: ~$ mips64-chimera-linux-musl-clang test.c -fuse-ld=lld -o test -static
root@cbuild: ~$ ./test
root@cbuild: ~$ mips64-chimera-linux-musl-clang test.c -fuse-ld=lld -o test -static-pie
root@cbuild: ~$ ./test
qemu: uncaught target signal 11 (Segmentation fault) - core dumped
Segmentation fault (core dumped)
```

I'm very certain that this is a bug in LLD. If I use BFD to link, the binary is fine.

---

### Comment 7 - Gelbpunkt

@yingopq hi, could you try if this reproduces for you with the snippet above?

---

### Comment 8 - yingopq

Thanks, I would try it ASAP.

---

### Comment 9 - yingopq

I can reproduce this Segmentation fault when I use `-fuse-ld=lld` instead of `-fuse-ld=/usr/local/musl/bin/mipsel-linux-musl-ld`.

---

