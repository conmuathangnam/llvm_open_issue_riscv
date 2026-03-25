# Segfault in llvm-objdump --offloading with LLVM 21.1

**Author:** cgmb
**URL:** https://github.com/llvm/llvm-project/issues/182567
**Status:** Closed
**Labels:** tools:llvm-objdump, crash
**Closed Date:** 2026-03-19T17:04:04Z

## Body

[LLVM 21.1.0](https://github.com/llvm/llvm-project/releases/download/llvmorg-21.1.0/LLVM-21.1.0-Linux-X64.tar.xz) and [LLVM 21.1.8](https://github.com/llvm/llvm-project/releases/download/llvmorg-21.1.8/LLVM-21.1.8-Linux-X64.tar.xz) segfault when I attempt to extract the offload sections from an object file, [rocsparse_axpyi.cpp.o.gz](https://github.com/user-attachments/files/25449568/rocsparse_axpyi.cpp.o.gz). The object file has been compressed with gzip for upload to GitHub.

```
# /root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o  

./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o:	file format elf64-x86-64
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o
 #0 0x0000556b9493f175 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) Signals.cpp:0:0
 #1 0x0000556b9493fa9f SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007fe4ce645f30 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x45f30)
 #3 0x00007fe4ce797222 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x197222)
 #4 0x0000556b9434d65e llvm::dumpOffloadBinary(llvm::object::ObjectFile const&, llvm::StringRef) (/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump+0x199365e)
 #5 0x0000556b942cddb3 dumpObject(llvm::object::ObjectFile*, llvm::object::Archive const*, llvm::object::Archive::Child const*) llvm-objdump.cpp:0:0
 #6 0x0000556b941f2104 llvm_objdump_main(int, char**, llvm::ToolContext const&) (/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump+0x1838104)
 #7 0x0000556b941ec562 main (/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump+0x1832562)
 #8 0x00007fe4ce62a575 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x2a575)
 #9 0x00007fe4ce62a628 __libc_start_main (/usr/lib/x86_64-linux-gnu/libc.so.6+0x2a628)
#10 0x0000556b9420b895 _start (/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump+0x1851895)
Segmentation fault         /root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o
```

It may or may not be unrelated, but it also fails to extract the offload sections from a shared object library, but emits a different error:
```
# /root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0 

./obj-x86_64-linux-gnu/library/librocsparse.so.1.0:	file format elf64-x86-64
/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump: error: './obj-x86_64-linux-gnu/library/librocsparse.so.1.0': while extracting offload FatBin bundles: Failed to decompress input: Could not decompress embedded file contents: Src size is incorrect
```

This problem does not occur with [LLVM 22.1.0-rc3](https://github.com/llvm/llvm-project/releases/download/llvmorg-22.1.0-rc3/LLVM-22.1.0-rc3-Linux-X64.tar.xz):
```
# /root/LLVM-22.1.0-rc3-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o  

./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o:	file format elf64-x86-64
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.host-x86_64-unknown-linux-gnu-
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1010
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1030
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1100
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1101
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1102
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx803
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx900
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx906
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx908
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx90a
```

```
# /root/LLVM-22.1.0-rc3-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0 

./obj-x86_64-linux-gnu/library/librocsparse.so.1.0:	file format elf64-x86-64
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0.0.host-x86_64-unknown-linux-gnu-
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0.0.hipv4-amdgcn-amd-amdhsa--gfx1010
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0.0.hipv4-amdgcn-amd-amdhsa--gfx1030
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0.0.hipv4-amdgcn-amd-amdhsa--gfx1100
<...>
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-tools-llvm-objdump

Author: Cory Bloor (cgmb)

<details>
[LLVM 21.1.0](https://github.com/llvm/llvm-project/releases/download/llvmorg-21.1.0/LLVM-21.1.0-Linux-X64.tar.xz) and [LLVM 21.1.8](https://github.com/llvm/llvm-project/releases/download/llvmorg-21.1.8/LLVM-21.1.8-Linux-X64.tar.xz) segfault when I attempt to extract the offload sections from an object file, [rocsparse_axpyi.cpp.o.gz](https://github.com/user-attachments/files/25449568/rocsparse_axpyi.cpp.o.gz). The object file has been compressed with gzip for upload to GitHub.

```
# /root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o  

./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o:	file format elf64-x86-64
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o
 #<!-- -->0 0x0000556b9493f175 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) Signals.cpp:0:0
 #<!-- -->1 0x0000556b9493fa9f SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->2 0x00007fe4ce645f30 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x45f30)
 #<!-- -->3 0x00007fe4ce797222 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x197222)
 #<!-- -->4 0x0000556b9434d65e llvm::dumpOffloadBinary(llvm::object::ObjectFile const&amp;, llvm::StringRef) (/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump+0x199365e)
 #<!-- -->5 0x0000556b942cddb3 dumpObject(llvm::object::ObjectFile*, llvm::object::Archive const*, llvm::object::Archive::Child const*) llvm-objdump.cpp:0:0
 #<!-- -->6 0x0000556b941f2104 llvm_objdump_main(int, char**, llvm::ToolContext const&amp;) (/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump+0x1838104)
 #<!-- -->7 0x0000556b941ec562 main (/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump+0x1832562)
 #<!-- -->8 0x00007fe4ce62a575 (/usr/lib/x86_64-linux-gnu/libc.so.6+0x2a575)
 #<!-- -->9 0x00007fe4ce62a628 __libc_start_main (/usr/lib/x86_64-linux-gnu/libc.so.6+0x2a628)
#<!-- -->10 0x0000556b9420b895 _start (/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump+0x1851895)
Segmentation fault         /root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o
```

It may or may not be unrelated, but it also fails to extract the offload sections from a shared object library, but emits a different error:
```
# /root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0 

./obj-x86_64-linux-gnu/library/librocsparse.so.1.0:	file format elf64-x86-64
/root/LLVM-21.1.8-Linux-X64/bin/llvm-objdump: error: './obj-x86_64-linux-gnu/library/librocsparse.so.1.0': while extracting offload FatBin bundles: Failed to decompress input: Could not decompress embedded file contents: Src size is incorrect
```

This problem does not occur with [LLVM 22.1.0-rc3](https://github.com/llvm/llvm-project/releases/download/llvmorg-22.1.0-rc3/LLVM-22.1.0-rc3-Linux-X64.tar.xz):
```
# /root/LLVM-22.1.0-rc3-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o  

./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o:	file format elf64-x86-64
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.host-x86_64-unknown-linux-gnu-
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1010
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1030
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1100
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1101
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx1102
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx803
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx900
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx906
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx908
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/CMakeFiles/rocsparse.dir/src/level1/rocsparse_axpyi.cpp.o.0.hipv4-amdgcn-amd-amdhsa--gfx90a
```

```
# /root/LLVM-22.1.0-rc3-Linux-X64/bin/llvm-objdump --offloading ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0 

./obj-x86_64-linux-gnu/library/librocsparse.so.1.0:	file format elf64-x86-64
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0.0.host-x86_64-unknown-linux-gnu-
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0.0.hipv4-amdgcn-amd-amdhsa--gfx1010
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0.0.hipv4-amdgcn-amd-amdhsa--gfx1030
Extracting offload bundle: ./obj-x86_64-linux-gnu/library/librocsparse.so.1.0.0.hipv4-amdgcn-amd-amdhsa--gfx1100
&lt;...&gt;
```
</details>


---

### Comment 2 - EugeneZelenko

Only most recent release (22 as of now) is maintained.

---

### Comment 3 - cgmb

I can bisect to find the change that fixed this problem, but it would be nice if someone with a bit of LLVM expertise could provide a hint as to what is wrong. I suspect it's related to offload compress.

LLVM 22 has not yet released and Ubuntu 26.04 has entered feature freeze. The distro maintainers are going to have to backport the fix into LLVM 21.

---

### Comment 4 - jh7370

Paging @jhuber6 / @david-salinas / @lamb-j 

> I suspect it's related to offload compress.

Looking briefly at the history of llvm-objdump's offloading, this commit stood out to me: https://github.com/llvm/llvm-project/commit/efda523188c45ecedad408cf48267fef97dfb2bc. I have no idea if it's related or not though. 

---

### Comment 5 - cgmb

Bisection confirms the problem is fixed by efda523188c45ecedad408cf48267fef97dfb2bc.

---

### Comment 6 - david-salinas

@cgmb I was going to refer to that patch to fix extracting compressed offload bundles, but you found it already.  :-) 

---

### Comment 7 - david-salinas

Closing as this is fixed with https://github.com/llvm/llvm-project/commit/efda523188c45ecedad408cf48267fef97dfb2bc

---

