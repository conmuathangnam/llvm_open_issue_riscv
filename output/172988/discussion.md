# Error while cross compiling linux kernel

**Author:** sudobomba
**URL:** https://github.com/llvm/llvm-project/issues/172988
**Status:** Open
**Labels:** lld:ELF, incomplete, crash

## Body

Stack dump:
```
0.      Program arguments: ld.lld -m elf_i386 --fatal-warnings -z noexecstack -r -o vmlinux.o --whole-archive vmlinux.a --no-whole-archive --start-group --end-group
 #0 0x0000007c91d44230 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/usr/lib/libLLVM.so.21.1+0xc24230)
 #1 0x0000007c91d414e4 (/usr/lib/libLLVM.so.21.1+0xc214e4)
 #2 0x0000007c9aff284c
 #3 0x0000007c9ae47d18 lld::elf::OutputSection::recordSection(lld::elf::InputSectionBase*) (/usr/lib/liblldELF.so.21.1+0x1d7d18)
 #4 0x0000007c9ae10df0 lld::elf::LinkerScript::addOrphanSections() (/usr/lib/liblldELF.so.21.1+0x1a0df0)
 #5 0x0000007c9ad73f7c void lld::elf::LinkerDriver::link<llvm::object::ELFType<(llvm::endianness)1, false>>(llvm::opt::InputArgList&) (/usr/lib/liblldELF.so.21.1+0x103f7c)
 #6 0x0000007c9ad8004c lld::elf::LinkerDriver::linkerMain(llvm::ArrayRef<char const*>) (/usr/lib/liblldELF.so.21.1+0x11004c)
 #7 0x0000007c9ad81fbc lld::elf::link(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, bool, bool) (/usr/lib/liblldELF.so.21.1+0x111fbc)
 #8 0x0000007c9a85d428 lld::unsafeLldMain(llvm::ArrayRef<char const*>, llvm::raw_ostream&, llvm::raw_ostream&, llvm::ArrayRef<lld::DriverDef>, bool) (/usr/lib/liblldCommon.so.21.1+0xd428)
 #9 0x00000030000026cc main (/usr/bin/ld.lld+0x26cc)
#10 0x0000007c90ca23d4 (/usr/lib/libc.so.6+0x223d4)
#11 0x0000007c90ca24b8 __libc_start_main (/usr/lib/libc.so.6+0x224b8)
#12 0x0000003000002770 _start (/usr/bin/ld.lld+0x2770)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-lld-elf

Author: sudobomba (sudobomba)

<details>
Stack dump:
```
0.      Program arguments: ld.lld -m elf_i386 --fatal-warnings -z noexecstack -r -o vmlinux.o --whole-archive vmlinux.a --no-whole-archive --start-group --end-group
 #<!-- -->0 0x0000007c91d44230 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/usr/lib/libLLVM.so.21.1+0xc24230)
 #<!-- -->1 0x0000007c91d414e4 (/usr/lib/libLLVM.so.21.1+0xc214e4)
 #<!-- -->2 0x0000007c9aff284c
 #<!-- -->3 0x0000007c9ae47d18 lld::elf::OutputSection::recordSection(lld::elf::InputSectionBase*) (/usr/lib/liblldELF.so.21.1+0x1d7d18)
 #<!-- -->4 0x0000007c9ae10df0 lld::elf::LinkerScript::addOrphanSections() (/usr/lib/liblldELF.so.21.1+0x1a0df0)
 #<!-- -->5 0x0000007c9ad73f7c void lld::elf::LinkerDriver::link&lt;llvm::object::ELFType&lt;(llvm::endianness)1, false&gt;&gt;(llvm::opt::InputArgList&amp;) (/usr/lib/liblldELF.so.21.1+0x103f7c)
 #<!-- -->6 0x0000007c9ad8004c lld::elf::LinkerDriver::linkerMain(llvm::ArrayRef&lt;char const*&gt;) (/usr/lib/liblldELF.so.21.1+0x11004c)
 #<!-- -->7 0x0000007c9ad81fbc lld::elf::link(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, bool, bool) (/usr/lib/liblldELF.so.21.1+0x111fbc)
 #<!-- -->8 0x0000007c9a85d428 lld::unsafeLldMain(llvm::ArrayRef&lt;char const*&gt;, llvm::raw_ostream&amp;, llvm::raw_ostream&amp;, llvm::ArrayRef&lt;lld::DriverDef&gt;, bool) (/usr/lib/liblldCommon.so.21.1+0xd428)
 #<!-- -->9 0x00000030000026cc main (/usr/bin/ld.lld+0x26cc)
#<!-- -->10 0x0000007c90ca23d4 (/usr/lib/libc.so.6+0x223d4)
#<!-- -->11 0x0000007c90ca24b8 __libc_start_main (/usr/lib/libc.so.6+0x224b8)
#<!-- -->12 0x0000003000002770 _start (/usr/bin/ld.lld+0x2770)
```
</details>


---

### Comment 2 - EugeneZelenko

Could you please provide reproducer?

---

