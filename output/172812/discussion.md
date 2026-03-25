# opt crashes in llvm/lib/Target/PowerPC/PPCTargetMachine.cpp:251

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/172812
**Status:** Open
**Labels:** backend:PowerPC, crash

## Body

When running opt on this following IR, opt crashes. 
I am not sure whether this is a valid case. If using opt in this way is not appropriate, I am open to closing this issue.

llvm version 3186ca2

test.ll

```
target triple = "powerpc-apple-darwin7.2"

@a = dso_local local_unnamed_addr global i32 0, align 4,!type!0
@g = internal dso_local global ptr null, align 4,!type!2

define void @_Z4testv() {
entry:
  store ptr inttoptr (i32 42 to ptr), ptr @a, align 4
  %0 = load ptr, ptr @a, align 4
  %1 = load i32, ptr %0, align 4
  ret void
}

!0 =!{i32 0,!"_ZTS1A1B"}
!2 =!{i32 0,!"_ZTS1A1D"}

```


commads
./bin/opt test.ll  -S


stacktrace

```
opt: llvm/lib/Target/PowerPC/PPCTargetMachine.cpp:251: llvm::CodeModel::Model getEffectivePPCCodeModel(const llvm::Triple&, std::optional<llvm::CodeModel::Model>, bool): Assertion `TT.isOSBinFormatELF() && "All remaining PPC OSes are ELF based."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ../llvm-dir/llvm-release/llvm-project/build/bin/opt test.ll -S
 #0 0x0000560f616c959f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560559f)
 #1 0x0000560f616c620c llvm::sys::RunSignalHandlers() (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560220c)
 #2 0x0000560f616c63a7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f19332fb420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f193294800b raise /build/glibc-LcI20x/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #5 0x00007f1932927859 abort /build/glibc-LcI20x/glibc-2.31/stdlib/abort.c:81:7
 #6 0x00007f1932927729 get_sysdep_segment_value /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:509:8
 #7 0x00007f1932927729 _nl_load_domain /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:970:34
 #8 0x00007f1932938fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #9 0x0000560f5dc3ca7d (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x1b78a7d)
#10 0x0000560f5dc3cb10 llvm::RegisterTargetMachine<llvm::PPCTargetMachine>::Allocator(llvm::Target const&, llvm::Triple const&, llvm::StringRef, llvm::StringRef, llvm::TargetOptions const&, std::optional<llvm::Reloc::Model>, std::optional<llvm::CodeModel::Model>, llvm::CodeGenOptLevel, bool) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x1b78b10)
#11 0x0000560f5fbaf1c0 llvm::codegen::createTargetMachineForTriple(llvm::StringRef, llvm::CodeGenOptLevel) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x3aeb1c0)
#12 0x0000560f5c90562e optMain (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x84162e)
#13 0x00007f1932929083 __libc_start_main /build/glibc-LcI20x/glibc-2.31/csu/../csu/libc-start.c:342:3
#14 0x0000560f5c8fdeae _start (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x839eae)
```


## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-backend-powerpc

Author: None (Emilyaxe)

<details>
When running opt on this following IR, opt crashes. 
I am not sure whether this is a valid case. If using opt in this way is not appropriate, I am open to closing this issue.

llvm version 3186ca2

test.ll

```
target triple = "powerpc-apple-darwin7.2"

@<!-- -->a = dso_local local_unnamed_addr global i32 0, align 4,!type!0
@<!-- -->g = internal dso_local global ptr null, align 4,!type!2

define void @<!-- -->_Z4testv() {
entry:
  store ptr inttoptr (i32 42 to ptr), ptr @<!-- -->a, align 4
  %0 = load ptr, ptr @<!-- -->a, align 4
  %1 = load i32, ptr %0, align 4
  ret void
}

!0 =!{i32 0,!"_ZTS1A1B"}
!2 =!{i32 0,!"_ZTS1A1D"}

```


commads
./bin/opt test.ll  -S


stacktrace

```
opt: llvm/lib/Target/PowerPC/PPCTargetMachine.cpp:251: llvm::CodeModel::Model getEffectivePPCCodeModel(const llvm::Triple&amp;, std::optional&lt;llvm::CodeModel::Model&gt;, bool): Assertion `TT.isOSBinFormatELF() &amp;&amp; "All remaining PPC OSes are ELF based."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ../llvm-dir/llvm-release/llvm-project/build/bin/opt test.ll -S
 #<!-- -->0 0x0000560f616c959f llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560559f)
 #<!-- -->1 0x0000560f616c620c llvm::sys::RunSignalHandlers() (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560220c)
 #<!-- -->2 0x0000560f616c63a7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00007f19332fb420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #<!-- -->4 0x00007f193294800b raise /build/glibc-LcI20x/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #<!-- -->5 0x00007f1932927859 abort /build/glibc-LcI20x/glibc-2.31/stdlib/abort.c:81:7
 #<!-- -->6 0x00007f1932927729 get_sysdep_segment_value /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:509:8
 #<!-- -->7 0x00007f1932927729 _nl_load_domain /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:970:34
 #<!-- -->8 0x00007f1932938fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #<!-- -->9 0x0000560f5dc3ca7d (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x1b78a7d)
#<!-- -->10 0x0000560f5dc3cb10 llvm::RegisterTargetMachine&lt;llvm::PPCTargetMachine&gt;::Allocator(llvm::Target const&amp;, llvm::Triple const&amp;, llvm::StringRef, llvm::StringRef, llvm::TargetOptions const&amp;, std::optional&lt;llvm::Reloc::Model&gt;, std::optional&lt;llvm::CodeModel::Model&gt;, llvm::CodeGenOptLevel, bool) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x1b78b10)
#<!-- -->11 0x0000560f5fbaf1c0 llvm::codegen::createTargetMachineForTriple(llvm::StringRef, llvm::CodeGenOptLevel) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x3aeb1c0)
#<!-- -->12 0x0000560f5c90562e optMain (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x84162e)
#<!-- -->13 0x00007f1932929083 __libc_start_main /build/glibc-LcI20x/glibc-2.31/csu/../csu/libc-start.c:342:3
#<!-- -->14 0x0000560f5c8fdeae _start (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x839eae)
```

</details>


---

### Comment 2 - akshaykumars614

This is working in godbolt (opt trunk) latest version.
https://godbolt.org/z/PEKf1bEor

---

### Comment 3 - Emilyaxe

> https://godbolt.org/z/PEKf1bEor

Hi! You need to select the assertions-enabled build: https://godbolt.org/z/GETKqPcf9

---

### Comment 4 - nemanjai

This is not a valid bug. We have disabled support for Darwin in PPC a number of years ago. The assert message actually says that it is expecting an ELF-based OS.

---

### Comment 5 - nikic

@nemanjai The assertion should be converted to reportFatalUsageError() to prevent this kind of issue report.

---

### Comment 6 - nemanjai

Fair enough. I'll put up a PR to change it.

---

