# error parsing wasm with multimemory

**Author:** benluelo
**URL:** https://github.com/llvm/llvm-project/issues/180435
**Status:** Open
**Labels:** tools:llvm-objdump, crash

## Body

```
Stack dump:
0.	Program arguments: llvm-objdump -ad multimemory.wasm
 #0 0x0000ffffee737ce8 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0xce7ce8)
 #1 0x0000ffffee73516c SignalHandler(int, siginfo_t*, void*) (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0xce516c)
 #2 0x0000fffff7ffa8d8 (linux-vdso.so.1+0x8d8)
 #3 0x0000fffff0e51c44 llvm::object::WasmObjectFile::getSymbolValueImpl(llvm::object::DataRefImpl) const (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0x3401c44)
 #4 0x0000fffff0e38b70 llvm::object::ObjectFile::getSymbolValue(llvm::object::DataRefImpl) const (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0x33e8b70)
 #5 0x0000fffff0e52974 llvm::object::WasmObjectFile::getSymbolAddress(llvm::object::DataRefImpl) const (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0x3402974)
 #6 0x0000aaaaaaad6280 llvm::objdump::createSymbolInfo(llvm::object::ObjectFile const&, llvm::object::SymbolRef const&, bool) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x36280)
 #7 0x0000aaaaaaadcad4 disassembleObject(llvm::object::ObjectFile&, llvm::object::ObjectFile const&, (anonymous namespace)::DisassemblerTarget&, std::optional<(anonymous namespace)::DisassemblerTarget>&, llvm::objdump::SourcePrinter&, bool, llvm::raw_ostream&) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x3cad4)
 #8 0x0000aaaaaaae3578 disassembleObject(llvm::object::ObjectFile*, bool, llvm::raw_ostream&) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x43578)
 #9 0x0000aaaaaaae6938 dumpObject(llvm::object::ObjectFile*, llvm::object::Archive const*, llvm::object::Archive::Child const*) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x46938)
#10 0x0000aaaaaaae8d08 llvm_objdump_main(int, char**, llvm::ToolContext const&) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x48d08)
#11 0x0000aaaaaaabc41c main (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x1c41c)
#12 0x0000ffffed516574 __libc_start_call_main (/nix/store/z6pz4znvbr103fln95y5xg1syb864x1i-glibc-2.42-47/lib/libc.so.6+0x26574)
#13 0x0000ffffed516658 __libc_start_main@GLIBC_2.17 (/nix/store/z6pz4znvbr103fln95y5xg1syb864x1i-glibc-2.42-47/lib/libc.so.6+0x26658)
#14 0x0000aaaaaaabc530 _start (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x1c530)
Segmentation fault         llvm-objdump -ad multimemory.wasm
```

`llvm-objdump --version`:

```
[ben@orb:~/projects/benluelo/db]$ llvm-objdump --version
LLVM (http://llvm.org/):
  LLVM version 22.1.0-rc2
  Optimized build.
```

(issue also occurs on `20.1.8`, fwiw)

full wasm file:

```
AGFzbQEAAAABBgFgAX8BfwMCAQAFBQIAAQABBggBfwFBgIAECwcvAw9leHRlcm5hbF9tZW1vcnkCAQZtZW1vcnkCARByZWFkX2Zyb21fbWVtb3J5AAAKGAEWAQJ/IwAhASAAKEIBACECIAEkACACCw==
```

compiled from this .wat file:

```wat
(module
 (global $global$0 (mut i32) (i32.const 65536))
 (memory $1 1)
 (export "external_memory" (memory $0))
 (memory $0 1)
 (export "memory" (memory $0))
 (export "read_from_memory" (func $1))
 (func $1 (param $0 i32) (result i32)
  (local $1 i32)
  (local $2 i32)
  (local.set $1
   (global.get $global$0)
  )
  (local.set $2
   (i32.load 1 offset=0
    (local.get $0)
   )
  )
  (global.set $global$0
   (local.get $1)
  )
  (local.get $2)
 )
)
```

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-tools-llvm-objdump

Author: ben (benluelo)

<details>
```
Stack dump:
0.	Program arguments: llvm-objdump -ad multimemory.wasm
 #<!-- -->0 0x0000ffffee737ce8 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0xce7ce8)
 #<!-- -->1 0x0000ffffee73516c SignalHandler(int, siginfo_t*, void*) (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0xce516c)
 #<!-- -->2 0x0000fffff7ffa8d8 (linux-vdso.so.1+0x8d8)
 #<!-- -->3 0x0000fffff0e51c44 llvm::object::WasmObjectFile::getSymbolValueImpl(llvm::object::DataRefImpl) const (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0x3401c44)
 #<!-- -->4 0x0000fffff0e38b70 llvm::object::ObjectFile::getSymbolValue(llvm::object::DataRefImpl) const (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0x33e8b70)
 #<!-- -->5 0x0000fffff0e52974 llvm::object::WasmObjectFile::getSymbolAddress(llvm::object::DataRefImpl) const (/nix/store/8irwghgdins4w14ibvrsfxrs5dikk3m0-llvm-22.1.0-rc2-lib/lib/libLLVM.so.22.1-rc2+0x3402974)
 #<!-- -->6 0x0000aaaaaaad6280 llvm::objdump::createSymbolInfo(llvm::object::ObjectFile const&amp;, llvm::object::SymbolRef const&amp;, bool) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x36280)
 #<!-- -->7 0x0000aaaaaaadcad4 disassembleObject(llvm::object::ObjectFile&amp;, llvm::object::ObjectFile const&amp;, (anonymous namespace)::DisassemblerTarget&amp;, std::optional&lt;(anonymous namespace)::DisassemblerTarget&gt;&amp;, llvm::objdump::SourcePrinter&amp;, bool, llvm::raw_ostream&amp;) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x3cad4)
 #<!-- -->8 0x0000aaaaaaae3578 disassembleObject(llvm::object::ObjectFile*, bool, llvm::raw_ostream&amp;) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x43578)
 #<!-- -->9 0x0000aaaaaaae6938 dumpObject(llvm::object::ObjectFile*, llvm::object::Archive const*, llvm::object::Archive::Child const*) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x46938)
#<!-- -->10 0x0000aaaaaaae8d08 llvm_objdump_main(int, char**, llvm::ToolContext const&amp;) (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x48d08)
#<!-- -->11 0x0000aaaaaaabc41c main (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x1c41c)
#<!-- -->12 0x0000ffffed516574 __libc_start_call_main (/nix/store/z6pz4znvbr103fln95y5xg1syb864x1i-glibc-2.42-47/lib/libc.so.6+0x26574)
#<!-- -->13 0x0000ffffed516658 __libc_start_main@<!-- -->GLIBC_2.17 (/nix/store/z6pz4znvbr103fln95y5xg1syb864x1i-glibc-2.42-47/lib/libc.so.6+0x26658)
#<!-- -->14 0x0000aaaaaaabc530 _start (/nix/store/mz8dar5s1gc8j03mga2qn85f8xvqzdj5-llvm-22.1.0-rc2/bin/llvm-objdump+0x1c530)
Segmentation fault         llvm-objdump -ad multimemory.wasm
```

`llvm-objdump --version`:

```
[ben@<!-- -->orb:~/projects/benluelo/db]$ llvm-objdump --version
LLVM (http://llvm.org/):
  LLVM version 22.1.0-rc2
  Optimized build.
```

(issue also occurs on `20.1.8`, fwiw)

full wasm file:

```
AGFzbQEAAAABBgFgAX8BfwMCAQAFBQIAAQABBggBfwFBgIAECwcvAw9leHRlcm5hbF9tZW1vcnkCAQZtZW1vcnkCARByZWFkX2Zyb21fbWVtb3J5AAAKGAEWAQJ/IwAhASAAKEIBACECIAEkACACCw==
```

compiled from this .wat file:

```wat
(module
 (global $global$0 (mut i32) (i32.const 65536))
 (memory $1 1)
 (export "external_memory" (memory $0))
 (memory $0 1)
 (export "memory" (memory $0))
 (export "read_from_memory" (func $1))
 (func $1 (param $0 i32) (result i32)
  (local $1 i32)
  (local $2 i32)
  (local.set $1
   (global.get $global$0)
  )
  (local.set $2
   (i32.load 1 offset=0
    (local.get $0)
   )
  )
  (global.set $global$0
   (local.get $1)
  )
  (local.get $2)
 )
)
```
</details>


---

### Comment 2 - jh7370

@sbc100 / @dschuff probably one for one of you to look at?

---

### Comment 3 - sbc100

IIRC there is no support anywhere in LLVM for multi-memory (yet).  I'm not sure how much sense it would make to had support to libObject without at least some basic support in MC and wams-ld.

I suppose we could at least produce a better error message.

What is you use case for wanting LLVM to parse this file?

---

### Comment 4 - benluelo

i was experimenting with wasm/wat and wanted to see an objdump of the compiled wasm file, and llvm-objdump supports wasm. i was curious if llvm supported multimemory (which i've since learned it does not) and got this crash instead of an error.

---

### Comment 5 - dschuff

I think we could probably at least add disassembly support (where I think all we'd need would be to decode the memarg and know that we need to print a memory index. Plus maybe relaxing some restriction when reading the memory sections). This would require at least some kind of minimal awareness in libObject that this is possible, because IIRC even when running objdump the module gets parsed into an ObjectFile.

---

### Comment 6 - dschuff

And I guess even more bare minimum than that, we could add a real error message. But I think we should at least be able to print a memory index on load instructions.

---

