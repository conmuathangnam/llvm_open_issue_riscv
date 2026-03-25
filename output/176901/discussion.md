# [BOLT] Crash when building instrumented azahar or using perf2bolt.

**Author:** asyync1024
**URL:** https://github.com/llvm/llvm-project/issues/176901
**Status:** Open
**Labels:** BOLT, crash

## Body

Hello! I hope that you are doing well.

I recently decided to compile [azahar](https://azahar-emu.org) with PGO, so I also decided to complement it with BOLT. I collected it's profile data via `perf record -e cycles:u -j any,u -o perf.data -- gamemoderun /usr/sbin/azahar`. I decided to use `gamemoderun` to get a more accurate demonstration of my normal usage, and used `/usr/sbin/azahar` as it was the path that `which` returned.

When I tried to convert the binary to BOLT compatible data I got this crash:
```
$ perf2bolt -p perf.data -o perf.fdata /usr/sbin/azahar
BOLT-INFO: shared object or position-independent executable detected
PERF2BOLT: Starting data aggregation job for perf.data
PERF2BOLT: spawning perf job to read branch events
PERF2BOLT: spawning perf job to read mem events
PERF2BOLT: spawning perf job to read process events
PERF2BOLT: spawning perf job to read task events
BOLT-INFO: Target architecture: x86_64
BOLT-INFO: BOLT version: 4987a3335718f5617c7c2a33bb9a157e8dd2afb0
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x9400000, offset 0x9400000
BOLT-INFO: enabling relocation mode
BOLT-INFO: enabling strict relocation mode for aggregation purposes
 #0 0x0000559562d7e500 (/usr/bin/llvm-bolt+0x1853500)
 #1 0x0000559562d7bc6b (/usr/bin/llvm-bolt+0x1850c6b)
 #2 0x0000559562d7f371 (/usr/bin/llvm-bolt+0x1854371)
 #3 0x00007f6ba383e4d0 (/usr/bin/../lib/libc.so.6+0x3e4d0)
 #4 0x0000559562e148bb (/usr/bin/llvm-bolt+0x18e98bb)
 #5 0x0000559562e1427e (/usr/bin/llvm-bolt+0x18e927e)
 #6 0x0000559562e0e29d (/usr/bin/llvm-bolt+0x18e329d)
 #7 0x0000559562e06189 (/usr/bin/llvm-bolt+0x18db189)
 #8 0x0000559562e00927 (/usr/bin/llvm-bolt+0x18d5927)
 #9 0x0000559562565c16 (/usr/bin/llvm-bolt+0x103ac16)
#10 0x00007f6ba3827635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#11 0x00007f6ba38276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#12 0x00007f6ba38276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#13 0x0000559562563ca5 (/usr/bin/llvm-bolt+0x1038ca5)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: perf2bolt -p perf.data -o perf.fdata /usr/sbin/azahar
Segmentation fault         (core dumped) perf2bolt -p perf.data -o perf.fdata /usr/sbin/azahar
```

I then tried to create an instrumented binary, got essentially the same error:
```
$ llvm-bolt /usr/sbin/azahar -instrument -o azahar-instr
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: x86_64
BOLT-INFO: BOLT version: 4987a3335718f5617c7c2a33bb9a157e8dd2afb0
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x9400000, offset 0x9400000
BOLT-INFO: enabling relocation mode
BOLT-INFO: forcing -jump-tables=move for instrumentation
BOLT-INFO: enabling lite mode
 #0 0x00005578efa4d500 (/usr/bin/llvm-bolt+0x1853500)
 #1 0x00005578efa4ac6b (/usr/bin/llvm-bolt+0x1850c6b)
 #2 0x00005578efa4e371 (/usr/bin/llvm-bolt+0x1854371)
 #3 0x00007f23ad83e4d0 (/usr/bin/../lib/libc.so.6+0x3e4d0)
 #4 0x00005578efae38bb (/usr/bin/llvm-bolt+0x18e98bb)
 #5 0x00005578efae327e (/usr/bin/llvm-bolt+0x18e927e)
 #6 0x00005578efadd29d (/usr/bin/llvm-bolt+0x18e329d)
 #7 0x00005578efad5189 (/usr/bin/llvm-bolt+0x18db189)
 #8 0x00005578efacf927 (/usr/bin/llvm-bolt+0x18d5927)
 #9 0x00005578ef234c16 (/usr/bin/llvm-bolt+0x103ac16)
#10 0x00007f23ad827635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#11 0x00007f23ad8276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#12 0x00007f23ad8276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#13 0x00005578ef232ca5 (/usr/bin/llvm-bolt+0x1038ca5)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: llvm-bolt /usr/sbin/azahar -instrument -o azahar-instr
Segmentation fault         (core dumped) llvm-bolt /usr/sbin/azahar -instrument -o azahar-instr
```

### Extra Info

- Both [llvm-bolt](https://aur.archlinux.org/pkgbase/llvm-bolt) and [azahar](https://azahar-emu.org) are compiled with `-mtune=generic -march=x86-64 -O2` flags, and are linked with `lld`.
- Azahar is a PIE executable.
- All LLVM tools are of version 21.1.6 (Because Arch Linux hasn't yet packaged 21.1.8)
- Using 21.1.8 version of BOLT results in the same error.
- Azahar's Instrumented binary during PGO was built without `-flto=thin`, while the Optimized final binary was biult with `-flto=thin`
- Both packages are built from the AUR, `llvm-bolt` uses the default PKGBUILD, while `azahar` uses the default PKGBUILD with custom PGO implementation.
- Not using PGO does not affect the issue and crashes with the same code.

Please ask for any other information.
Thanks!
LL

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-bolt

Author: LinuxLover471 (LinuxLover471)

<details>
Hello! I hope that you are doing well.

I recently decided to compile [azahar](https://azahar-emu.org) with PGO, so I also decided to complement it with BOLT. I collected it's profile data via `perf record -e cycles:u -j any,u -o perf.data -- gamemoderun /usr/sbin/azahar`. I decided to use `gamemoderun` to get a more accurate demonstration of my normal usage, and used /usr/sbin/azahar as it was the command that `which` returned.

When I tried to convert the binary to BOLT compatible data I got this crash:
```
$ perf2bolt -p perf.data -o perf.fdata /usr/sbin/azahar
BOLT-INFO: shared object or position-independent executable detected
PERF2BOLT: Starting data aggregation job for perf.data
PERF2BOLT: spawning perf job to read branch events
PERF2BOLT: spawning perf job to read mem events
PERF2BOLT: spawning perf job to read process events
PERF2BOLT: spawning perf job to read task events
BOLT-INFO: Target architecture: x86_64
BOLT-INFO: BOLT version: 4987a3335718f5617c7c2a33bb9a157e8dd2afb0
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x9400000, offset 0x9400000
BOLT-INFO: enabling relocation mode
BOLT-INFO: enabling strict relocation mode for aggregation purposes
 #<!-- -->0 0x0000559562d7e500 (/usr/bin/llvm-bolt+0x1853500)
 #<!-- -->1 0x0000559562d7bc6b (/usr/bin/llvm-bolt+0x1850c6b)
 #<!-- -->2 0x0000559562d7f371 (/usr/bin/llvm-bolt+0x1854371)
 #<!-- -->3 0x00007f6ba383e4d0 (/usr/bin/../lib/libc.so.6+0x3e4d0)
 #<!-- -->4 0x0000559562e148bb (/usr/bin/llvm-bolt+0x18e98bb)
 #<!-- -->5 0x0000559562e1427e (/usr/bin/llvm-bolt+0x18e927e)
 #<!-- -->6 0x0000559562e0e29d (/usr/bin/llvm-bolt+0x18e329d)
 #<!-- -->7 0x0000559562e06189 (/usr/bin/llvm-bolt+0x18db189)
 #<!-- -->8 0x0000559562e00927 (/usr/bin/llvm-bolt+0x18d5927)
 #<!-- -->9 0x0000559562565c16 (/usr/bin/llvm-bolt+0x103ac16)
#<!-- -->10 0x00007f6ba3827635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->11 0x00007f6ba38276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->12 0x00007f6ba38276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->13 0x0000559562563ca5 (/usr/bin/llvm-bolt+0x1038ca5)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: perf2bolt -p perf.data -o perf.fdata /usr/sbin/azahar
Segmentation fault         (core dumped) perf2bolt -p perf.data -o perf.fdata /usr/sbin/azahar
```

I then tried to create an instrumented binary, got essentially the same error:
```
$ llvm-bolt /usr/sbin/azahar -instrument -o azahar-instr
BOLT-INFO: shared object or position-independent executable detected
BOLT-INFO: Target architecture: x86_64
BOLT-INFO: BOLT version: 4987a3335718f5617c7c2a33bb9a157e8dd2afb0
BOLT-INFO: first alloc address is 0x0
BOLT-INFO: creating new program header table at address 0x9400000, offset 0x9400000
BOLT-INFO: enabling relocation mode
BOLT-INFO: forcing -jump-tables=move for instrumentation
BOLT-INFO: enabling lite mode
 #<!-- -->0 0x00005578efa4d500 (/usr/bin/llvm-bolt+0x1853500)
 #<!-- -->1 0x00005578efa4ac6b (/usr/bin/llvm-bolt+0x1850c6b)
 #<!-- -->2 0x00005578efa4e371 (/usr/bin/llvm-bolt+0x1854371)
 #<!-- -->3 0x00007f23ad83e4d0 (/usr/bin/../lib/libc.so.6+0x3e4d0)
 #<!-- -->4 0x00005578efae38bb (/usr/bin/llvm-bolt+0x18e98bb)
 #<!-- -->5 0x00005578efae327e (/usr/bin/llvm-bolt+0x18e927e)
 #<!-- -->6 0x00005578efadd29d (/usr/bin/llvm-bolt+0x18e329d)
 #<!-- -->7 0x00005578efad5189 (/usr/bin/llvm-bolt+0x18db189)
 #<!-- -->8 0x00005578efacf927 (/usr/bin/llvm-bolt+0x18d5927)
 #<!-- -->9 0x00005578ef234c16 (/usr/bin/llvm-bolt+0x103ac16)
#<!-- -->10 0x00007f23ad827635 __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#<!-- -->11 0x00007f23ad8276e9 call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#<!-- -->12 0x00007f23ad8276e9 __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#<!-- -->13 0x00005578ef232ca5 (/usr/bin/llvm-bolt+0x1038ca5)
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: llvm-bolt /usr/sbin/azahar -instrument -o azahar-instr
Segmentation fault         (core dumped) llvm-bolt /usr/sbin/azahar -instrument -o azahar-instr
```

### Extra Info

- Both [llvm-bolt](https://aur.archlinux.org/pkgbase/llvm-bolt) and [azahar](https://azahar-emu.org) are compiled with `-mtune=generic -march=x86-64 -O2` flags, and are linked with `lld`.
- Azahar is a PIE executable.
- All LLVM tools are of version 21.1.6 (Because Arch Linux hasn't yet packaged 21.1.8)
- Using 21.1.8 version of BOLT results in the same error.


Please ask for any other information.
Thanks!
LL
</details>


---

### Comment 2 - maksfb

Thanks for the report. Could you please share `azahar` binary that was passed to BOLT?

---

### Comment 3 - asyync1024

Sure! Here's the binary:
[azahar-O2-generic-with-relocs.zip](https://github.com/user-attachments/files/24785164/azahar-O2-generic-with-relocs.zip)

There is a new version of azahar, 2124.1, though it seems like it's a major bug fix for the Vulkan renderer, and I am not able to collect the profile data for PGO because of another problem:
https://discourse.llvm.org/t/pgo-crash-while-collecting-profile-with-instrumented-binary-for-pgo/89529

---

### Comment 4 - asyync1024

I found the time and updated the binary to 2124.1, though it was quite hard to obtain a profile.
[azahar-2124.1-generic-O2-pgo-thin-lld-with-relocs.zip](https://github.com/user-attachments/files/24813010/azahar-2124.1-generic-O2-pgo-thin-lld-with-relocs.zip)
I hope this can possibly help in fixing this issue.

---

### Comment 5 - asyync1024

Sorry for another ping! But I thought it's might be worth noting that using a non-PGO binary doesn't fix or affect the issue.
Here is the latest, non-PGO binary:
[azahar-2124.3-generic-O2-relocs-nopgo.zip](https://github.com/user-attachments/files/25000233/azahar-2124.3-generic-O2-relocs-nopgo.zip)


---

