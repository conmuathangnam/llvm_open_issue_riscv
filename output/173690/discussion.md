# lld 20, 21, 23 make broken binary. mold and ld work. Code compiled with clang.

**Author:** mcarans
**URL:** https://github.com/llvm/llvm-project/issues/173690
**Status:** Open
**Labels:** lld:ELF, crash

## Body

I have updated the game [Oolite](https://github.com/OoliteProject/oolite/) to build with clang (20 or 21). It works fine on Linux when linking with ld or mold. When linking with lld 20, 21 or 23, the resulting executable fails with a segmentation fault. Please advise.



## Comments

### Comment 1 - DhanashreePetare

Greetings @mcarans, I am interested to work on the issue. Can you please assign it to me. Thankyou.

---

### Comment 2 - shafik

@MaskRay any advice here on what we need to help figure out the issue?

---

### Comment 3 - mcarans

@DhanashreePetare I would be happy to assign it to you, but I do not have the permissions to do so. It probably requires an LLVM dev to do it.

---

### Comment 4 - hstk30-hw

First at all, need a reproduce. Add `--reproduce=` for lld, to get a reproduce.

---

### Comment 5 - llvmbot


@llvm/issue-subscribers-lld-elf

Author: Mike (mcarans)

<details>
I have updated the game [Oolite](https://github.com/OoliteProject/oolite/) to build with clang (20 or 21). It works fine on Linux when linking with ld or mold. When linking with lld 20 or 21, the resulting executable fails with a segmentation fault. Please advise.


</details>


---

### Comment 6 - MaskRay

Can you upload the reproduce tarball somewhere?

Either or the following command creates a reproduce tarball
```sh
clang -fuse-ld=lld -Wl,--reproduce=/tmp/rep.tar a.o b.o
LLD_REPRODUCE=/tmp/rep.tar clang -fuse-ld=lld a.o b.o
```

You can then unpack the tarball, navigate to the directory, and invoke LLD with the response file:

```sh
cd /tmp; tar xf rep.tar; cd rep
ld.lld @response.txt # append options like -y foo
```

The response file includes the `--chroot` option, which GNU ld does not support.
In most cases, you can simply remove this option to examine GNU ld's behavior.

```sh
ld.bfd @response.txt
```


---

### Comment 7 - mcarans

@MaskRay Thanks for this explanation. I did not know about the --reproduce option. Please find rep.tar file here:
https://www.filemail.com/d/jpphgknlptloyih

I confirmed after adding `-Wl,--reproduce=/tmp/rep.tar` that running the original executable produced by lld fails with a segmentation fault. 

`ld.bfd @response.txt` gives `ld.bfd: bad -plugin-opt option` I tried deleting `-plugin-opt=mcpu=x86-64` from response.txt, but that gave `home/mcarans/Code/OoliteRelated/oolite_mcarans/obj.spk/oolite.obj/legacy_random.c.o: file not recognized: file format not recognized`

`mold @response.txt` gives `mold: fatal: home/mcarans/Code/OoliteRelated/oolite_mcarans/obj.spk/oolite.obj/legacy_random.c.o: unable to handle this LTO object file because the -plugin option was not provided. Please make sure you added -flto not only when creating object files but also when linking the final executable.` Note that `-flto` was added to both compiler and linker flags.

---

### Comment 8 - mcarans

@hstk30-hw I made another rep.tar without link time optimisation: https://www.filemail.com/d/jswssqfuartjvzk

You also need to add this Resources folder (game resources)  to the unpacked rep folder: https://fromsmash.com/Resources-folder, otherwise the game won't start ie. the Resources folder must be next to the oolite executable.

Using the rep.tar without link time optimisation and the Resources folder, I could replicate the behaviour I'm seeing ie. that bfd and mold produce an executable that works and lld produces one that gives Segmentation fault.

---

### Comment 9 - mcarans

@MaskRay I've made the reproduce available as a GitHub repository for simplicity (and longevity): https://github.com/mcarans/lld_bug

---

### Comment 10 - mcarans

@EugeneZelenko I [was asked](https://github.com/llvm/llvm-project/issues/173690#issuecomment-3693356948) to assign this bug but I don't have that option. Who can do that or what is the process for that?

---

### Comment 11 - EugeneZelenko

@mcarans: Maintainers started to ask for pull request before assignment. I think it's reasonable course of events. It'll also reduce amount of assignments where nothing happened.

---

### Comment 12 - mcarans

@DhanashreePetare @shafik Please see EugeneZelenko's comment above

---

### Comment 13 - cabbaken

It seems the reproduce in github lacks some lib. It would report:
```plain
ld: warning: libOpenGL.so.0, needed by /home/user/my/lld_bug/usr/lib/x86_64-linux-gnu/libGLU.so, not found (try using -rpath or -rpath-link)
ld: warning: libxml2.so.16, needed by /home/user/my/lld_bug/usr/local/lib/libgnustep-base.so, not found (try using -rpath or -rpath-link)
ld: warning: libicui18n.so.76, needed by /home/user/my/lld_bug/usr/local/lib/libgnustep-base.so, not found (try using -rpath or -rpath-link)
ld: warning: libicuuc.so.76, needed by /home/user/my/lld_bug/usr/local/lib/libgnustep-base.so, not found (try using -rpath or -rpath-link)
ld: warning: libicudata.so.76, needed by /home/user/my/lld_bug/usr/local/lib/libgnustep-base.so, not found (try using -rpath or -rpath-link)
...
```
with
```bash
ld @respondse.txt
```

---

### Comment 14 - mcarans

> It seems the reproduce in github lacks some lib. It would report:
> 
> ```
> ld: warning: libOpenGL.so.0, needed by /home/user/my/lld_bug/usr/lib/x86_64-linux-gnu/libGLU.so, not found (try using -rpath or -rpath-link)
> ld: warning: libxml2.so.16, needed by /home/user/my/lld_bug/usr/local/lib/libgnustep-base.so, not found (try using -rpath or -rpath-link)
> ld: warning: libicui18n.so.76, needed by /home/user/my/lld_bug/usr/local/lib/libgnustep-base.so, not found (try using -rpath or -rpath-link)
> ld: warning: libicuuc.so.76, needed by /home/user/my/lld_bug/usr/local/lib/libgnustep-base.so, not found (try using -rpath or -rpath-link)
> ld: warning: libicudata.so.76, needed by /home/user/my/lld_bug/usr/local/lib/libgnustep-base.so, not found (try using -rpath or -rpath-link)
> ...

@cabbaken I'm not sure why `--reproduce` did not include those files, but I have added them (symbolic link + real file) here: https://github.com/mcarans/lld_bug/tree/main/lib/x86_64-linux-gnu. Please can you confirm if this enables `ld @response.txt` to complete.

---

### Comment 15 - cabbaken

I did some [modification](https://paste.ubuntu.com/p/NtS7vSxHtd/) to response.txt in [mcarans/lld_bug](https://github.com/mcarans/lld_bug) to make it run with `lld.ld @response.txt`.
I am unable to reproduce the segmentation fault on the current main branch using response.txt.
This suggests the issue might be environment-specific or already resolved in upstream. Could you please:
- Verify the crash against the latest upstream lld build
- If it still crashes, provide a backtrace from lld

---

### Comment 16 - mcarans

@cabbaken I applied your diff and updated the repo. I  tried lld-23 and the resulting oolite binary still gave a segmentation fault when run:

```
> ld.lld-23 --version
Ubuntu LLD 23.0.0 (compatible with GNU linkers)

> ld.lld-23 @response.txt

> ./oolite
2026-02-13 11:37:23.694 oolite[49404:49405] Truncating thread name 'OOLogOutputHandler logging thread' to 15 characters due to platform limitations
Segmentation fault (core dumped)

139 > apt list lld*23
lld-23/unknown,now 1:23~++20260211083036+1e42c76d6133-1~exp1~20260211083054.315 amd64 [installed]
```

I just want to confirm that you tried running the executable? It isn't lld itself that gives a segmentation fault, but the resulting binary (oolite).  

For me, lld 20, 21 and 23 all make an executable that fails to run. I tried with mold again and the oolite binary it created from response.txt ran fine.

If the binary runs fine for you linked with lld, please can you tell me how to produce a backtrace.

---

### Comment 17 - mcarans

@cabbaken Just wondering if lld generated a broken binary that segfaults for you? If not, you mentioned making a backtrace. Can you give more info on how to do that? 

---

### Comment 18 - cabbaken

> I just want to confirm that you tried running the executable? It isn't lld itself that gives a segmentation fault, but the resulting binary (oolite).

I apologize for not responding sooner. I couldn't run the executable successfully because I don't have the necessary system environment.

---

### Comment 19 - mcarans

> > I just want to confirm that you tried running the executable? It isn't lld itself that gives a segmentation fault, but the resulting binary (oolite).
> 
> I apologize for not responding sooner. I couldn't run the executable successfully because I don't have the necessary system environment.

No worries. Thanks for getting back. 

Please can you tell me what is missing. I may be able to add any missing libraries. If that won't help, could you please advise on any other info I can give that may narrow down this issue?

---

### Comment 20 - mcarans

lld:

```
> nm oolite_lld | grep GCChunkAllocator
0000000000519400 t _GLOBAL__I__ZN2js23defaultGCChunkAllocatorE
0000000000519520 t _ZN2js16GCChunkAllocator6doFreeEPv
0000000000519530 t _ZN2js16GCChunkAllocator7doAllocEv
00000000007dc5e8 b _ZN2js23defaultGCChunkAllocatorE
0000000000721e00 d _ZTVN2js16GCChunkAllocatorE

hexdump -C oolite_lld | grep "00 1e 72 00 00 00 00 00"
00d5e800  a3 1f 0e 00 01 02 17 00  00 1e 72 00 00 00 00 00  |..........r.....|

> readelf -r oolite_lld  | grep -c "RELATIVE"
47800
```

mold:

```
> nm oolite_mold | grep GCChunkAllocator
0000000000520b00 t _GLOBAL__I__ZN2js23defaultGCChunkAllocatorE
0000000000520c20 t _ZN2js16GCChunkAllocator6doFreeEPv
0000000000520c30 t _ZN2js16GCChunkAllocator7doAllocEv
00000000007e27c8 b _ZN2js23defaultGCChunkAllocatorE
00000000007265e0 d _ZTVN2js16GCChunkAllocatorE

> hexdump -C oolite_mold | grep "e0 65 72 00 00 00 00 00"
00ea8bd0  e0 65 72 00 00 00 00 00  20 00 00 00 00 00 00 00  |.er..... .......|

> readelf -r oolite_mold | grep -c "RELATIVE"
48628
```

I relied on AI to tell me what to debug. AI says that the above shows "Proof of Miscalculation". It says about lld:
"Because this is a PIE (Position Independent Executable), the entire program is shifted in memory by ASLR (Address Space Layout Randomization) every time it runs. By "baking in" the static address 0x721e00, ld.lld is creating a pointer that points to the "old" link-time location. When Oolite tries to follow that pointer to find the SpiderMonkey vtable, it hits unmapped memory and Segfaults."

On mold, it says:
"mold has also put an address there (0x7265e0), but the difference is that mold accompanies this with an anonymous relative relocation. You can't see the relocation in a hex dump, but you can prove it exists with one last command."

AI concludes:
"oolite_mold will have a higher count. It created a "Relative" relocation for that vtable, telling the Linux kernel: "Hey, when you load Oolite, add the load-offset to the value at offset 0xea8bd0."

oolite_lld missed this relocation. It left the address "naked," leading to the crash."

Does that make sense?

---

### Comment 21 - mcarans

It is this .a file within the reproduce repo I provided that causes the issue:
lld_bug/home/mcarans/Code/OoliteRelated/oolite_mcarans/deps/Linux-deps/x86_64/mozilla/libjs_static.a

It was built many years ago. mold and ld seem to cope ok with it but lld does not. I rebuilt libjs_static.a with gcc on Ubuntu 22.04 and lld produces a working oolite binary using that one.

Is lld designed to be backwards compatible with older .a files?


---

### Comment 22 - cabbaken

In fact, I'm not familiar with LLD.
Perhaps you could ask someone who knows more about it, such as @MaskRay?

---

