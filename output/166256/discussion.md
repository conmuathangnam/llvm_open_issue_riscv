# `lld` says `LLVM ERROR: Broken module found, compilation aborted!` in `vmlinux.o` on compiling Linux 6.17.6.

**Author:** dreirund
**URL:** https://github.com/llvm/llvm-project/issues/166256
**Status:** Open
**Labels:** compiler-rt:asan, crash, LTO

## Body

Ahoj,

a build of Linux 6.17.6 with LLVM's `clang` version 21.1.4, `lld` version 21.1.4 resulted in error outputs from LLVM CLang/ lld that sais that I should report an issue here:

```
LLVM ERROR: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
```

Here is what I did:

* I have an AMD ZEN4 CPU, in detail: AMD 7840U SoC.
  `flags` from `/proc/cpuinfo`:  
  `fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush mmx fxsr sse sse2 ht syscall nx mmxext fxsr_opt pdpe1gb rdtscp lm constant_tsc rep_good amd_lbr_v2 nopl xtopology nonstop_tsc cpuid extd_apicid aperfmperf rapl pni pclmulqdq monitor ssse3 fma cx16 sse4_1 sse4_2 x2apic movbe popcnt aes xsave avx f16c rdrand lahf_lm cmp_legacy svm extapic cr8_legacy abm sse4a misalignsse 3dnowprefetch osvw ibs skinit wdt tce topoext perfctr_core perfctr_nb bpext perfctr_llc mwaitx cpuid_fault cpb cat_l3 cdp_l3 hw_pstate ssbd mba perfmon_v2 ibrs ibpb stibp ibrs_enhanced vmmcall fsgsbase bmi1 avx2 smep bmi2 erms invpcid cqm rdt_a avx512f avx512dq rdseed adx smap avx512ifma clflushopt clwb avx512cd sha_ni avx512bw avx512vl xsaveopt xsavec xgetbv1 xsaves cqm_llc cqm_occup_llc cqm_mbm_total cqm_mbm_local user_shstk avx512_bf16 clzero irperf xsaveerptr rdpru wbnoinvd cppc arat npt lbrv svm_lock nrip_save tsc_scale vmcb_clean flushbyasid decodeassists pausefilter pfthreshold vgif x2avic v_spec_ctrl vnmi avx512vbmi umip pku ospke avx512_vbmi2 gfni vaes vpclmulqdq avx512_vnni avx512_bitalg avx512_vpopcntdq rdpid overflow_recov succor smca fsrm flush_l1d amd_lbr_pmc_freeze`,
* I have [Artix GNU/Linux](https://artixlinux.org/) (rolling release) as my operating system and installed [packages `clang`](https://packages.artixlinux.org/packages/world/x86_64/clang/) and [`lld`](https://packages.artixlinux.org/packages/world/x86_64/lld/) from their repositories,
* I took [vanilla Linux kernel 6.17.6 from kernel.org](https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.17.6.tar.xz),
* added `-mno-sse4a` to `KBUILD_CFLAGS` in Linux sources' `arch/x86/Makefile` to work around [this bug](https://bugs.gentoo.org/962779) which surfaced wirth CLang version 21.x ([this is the patch applied to Linux achieve this](https://github.com/user-attachments/files/23319250/kbuild-disable-sse4a_vanilla.patch.txt)),
* used [this kernel configuration](https://github.com/user-attachments/files/23319184/config-6.17-vanilla-custom-clang.txt). Especially:
  - with LTO,
  - with `CONFIG_X86_NATIVE_CPU=y`.
* compiled by `LLVM=1 LLVM_IAS=1 make V=1 -j1 all`.

Terminal output of `LLVM=1 LLVM_IAS=1 make V=1 -j1 all`:  
```c++
[...]
make -f ./scripts/Makefile.vmlinux_o
# LD      vmlinux.o
  ld.lld -m elf_x86_64 -mllvm -import-instr-limit=5 --compress-debug-sections=zstd -z noexecstack -r -o vmlinux.o  -T .tmp_initcalls.lds --whole-archive vmlinux.a --no-whole-archive --start-group  --end-group  ; ./tools/objtool/objtool --hacks=jump_label --hacks=noinstr --ibt --mcount --mnop --orc --retpoline --rethunk --sls --static-call --uaccess --link vmlinux.o
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
inlinable function call in a function with debug info must have a !dbg location
  call void @__sanitizer_cov_stack_depth() #335
LLVM ERROR: Broken module found, compilation aborted!
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: ld.lld -m elf_x86_64 -mllvm -import-instr-limit=5 --compress-debug-sections=zstd -z noexecstack -r -o vmlinux.o -T .tmp_initcalls.lds --whole-archive vmlinux.a --no-whole-archive --start-group --end-group
1.	Running pass "verify" on module "ld-temp.o"
 #0 0x00007f4d1fcfe91e llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/libLLVM.so.21.1+0x8fe91e)
 #1 0x00007f4d1fcfba67 (/usr/lib/libLLVM.so.21.1+0x8fba67)
 #2 0x00007f4d1ec3e240 (/usr/lib/libc.so.6+0x3e240)
 #3 0x00007f4d1ec9644c __pthread_kill_implementation /usr/src/debug/glibc/glibc/nptl/pthread_kill.c:44:76
 #4 0x00007f4d1ec3e108 raise /usr/src/debug/glibc/glibc/signal/../sysdeps/posix/raise.c:27:6
 #5 0x00007f4d1ec25667 abort /usr/src/debug/glibc/glibc/stdlib/abort.c:85:3
 #6 0x00007f4d1f93f198 (/usr/lib/libLLVM.so.21.1+0x53f198)
 #7 0x00007f4d1fbf34ce (/usr/lib/libLLVM.so.21.1+0x7f34ce)
 #8 0x00007f4d1ff91411 (/usr/lib/libLLVM.so.21.1+0xb91411)
 #9 0x00007f4d22184ba4 (/usr/lib/libLLVM.so.21.1+0x2d84ba4)
#10 0x00007f4d1ff4bc00 llvm::PassManager<llvm::Module, llvm::AnalysisManager<llvm::Module>>::run(llvm::Module&, llvm::AnalysisManager<llvm::Module>&) (/usr/lib/libLLVM.so.21.1+0xb4bc00)
#11 0x00007f4d2218c8c8 (/usr/lib/libLLVM.so.21.1+0x2d8c8c8)
#12 0x00007f4d2218e5e5 llvm::lto::opt(llvm::lto::Config const&, llvm::TargetMachine*, unsigned int, llvm::Module&, bool, llvm::ModuleSummaryIndex*, llvm::ModuleSummaryIndex const*, std::vector<unsigned char, std::allocator<unsigned char>> const&) (/usr/lib/libLLVM.so.21.1+0x2d8e5e5)
#13 0x00007f4d22190e3d llvm::lto::backend(llvm::lto::Config const&, std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, unsigned int, llvm::Module&, llvm::ModuleSummaryIndex&) (/usr/lib/libLLVM.so.21.1+0x2d90e3d)
#14 0x00007f4d2217ec07 llvm::lto::LTO::runRegularLTO(std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>) (/usr/lib/libLLVM.so.21.1+0x2d7ec07)
#15 0x00007f4d22183326 llvm::lto::LTO::run(std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream>>> (unsigned int, llvm::Twine const&)>, llvm::FileCache) (/usr/lib/libLLVM.so.21.1+0x2d83326)
#16 0x00007f4d28f66c1c lld::elf::BitcodeCompiler::compile() (/usr/lib/liblldELF.so.21.1+0x166c1c)
#17 0x00007f4d28ecfa42 (/usr/lib/liblldELF.so.21.1+0xcfa42)
#18 0x00007f4d28eada23 lld::elf::LinkerDriver::linkerMain(llvm::ArrayRef<char const*>) (/usr/lib/liblldELF.so.21.1+0xada23)
#19 0x00007f4d28eaf5f0 lld::elf::link(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, bool, bool) (/usr/lib/liblldELF.so.21.1+0xaf5f0)
#20 0x00007f4d28c10bc0 lld::unsafeLldMain(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, llvm::ArrayRef<lld::DriverDef>, bool) (/usr/lib/liblldCommon.so.21.1+0xcbc0)
#21 0x000055fa94971a7c lld_main(int, char**, llvm::ToolContext const&) (/usr/bin/ld.lld+0x2a7c)
#22 0x000055fa949712b6 main (/usr/bin/ld.lld+0x22b6)
#23 0x00007f4d1ec2773b __libc_start_call_main /usr/src/debug/glibc/glibc/csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#24 0x00007f4d1ec277fc call_init /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:128:20
#25 0x00007f4d1ec277fc __libc_start_main /usr/src/debug/glibc/glibc/csu/../csu/libc-start.c:347:5
#26 0x000055fa94971315 _start (/usr/bin/ld.lld+0x2315)
make[2]: *** [scripts/Makefile.vmlinux_o:72: vmlinux.o] Error 134
make[1]: *** [/var/cache/makepkg/build/linux-dreieck/src/build/vanilla-customconfig-clang/Makefile:1225: vmlinux_o] Error 2
make: *** [Makefile:248: __sub-make] Error 2
```

A GDB session (`gdb --args make LLVM=1 LLVM_IAS=1 V=1 -j1 all`) also does not give more options (no backtrace possible).

[Others also seem to have this](https://aur.archlinux.org/packages/linux-cachyos-headers#comment-1045184).

I do not know C, I do not know about how linking works, I can only report what goes on here, and I report because I am told so in the message that I should report.

Regards!


## Comments

### Comment 1 - dreirund

OK, more context:

I rebuilt Linux as above, starting over from clean source tree, but with `-i` passed to `make`, to not delete the files created during build (as I was educated [here](https://bugs.gentoo.org/962779#c92)):  
`make -j1 -i V=1 LLVM=1 LLVM_IAS=1 2>&1 | tee linux-6.17.6-make.log`.

*(For reference, complete terminal output of that command: [`linux-6.17.6-make.log.gz`](https://github.com/user-attachments/files/23345657/linux-6.17.6-make.log.gz).)*

I attach the generated `vmlinux.a`: [`vmlinux.a.gz`](https://github.com/user-attachments/files/23345677/vmlinux.a.gz) (`vmlinux.o` and `ld-temp.o` I do not find.)

If you want to analyse more, I also attach the complete linux source directory as left after the above failing compile command (only my username I have changed to `CENSORED`): [`linux-6.17.6.tar.zst`](http://eidreck.de/filedrop/2c688e3a-b9c9-11f0-942a-3b9bbfff80db/linux-6.17.6.tar.zst) *(size: Ca. 2.8&thinsp;GiB, [sha256sum: `e6019322dce0364bc11c3bce509fc0f26e3534ddcfaa751f2b886f79f7247b28`](http://eidreck.de/filedrop/2c688e3a-b9c9-11f0-942a-3b9bbfff80db/linux-6.17.6.tar.zst.sha256))*.

Regards!


---

### Comment 2 - sv3nm4n

+1 
I have the same issue with kernel version 6.19-rc1 and iirc 6.18, 6.17 when I tested it a while back.
It can be "fixed" by compiling the kernel without debug info.
But since some people need to have a kernel with debug info a real fix would be nice :)

---

