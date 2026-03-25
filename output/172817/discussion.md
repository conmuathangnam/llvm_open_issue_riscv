# opt crashes in llvm-project/llvm/lib/IR/AutoUpgrade.cpp:4999

**Author:** Emilyaxe
**URL:** https://github.com/llvm/llvm-project/issues/172817
**Status:** Closed
**Labels:** crash, llvm:ir
**Closed Date:** 2025-12-19T10:02:21Z

## Body

When running opt on this following IR, opt crashes. 
I am not sure whether this is a valid case. If using opt in this way is not appropriate, I am open to closing this issue.

llvm version 3186ca2

test.ll

```
; Function Attrs: nocallback nofree nosync willreturn memory(argmem: readwrite)
declare i32 @llvm.ctlz.i32.p0(i32, i1 immarg) #0

define i32 @c1(i32 %A) {
  %for.body.i = call i32 @llvm.ctlz.i32.p0(i32 %A, i1 false)
  ret i32 %for.body.i
}

attributes #0 = { nocallback nofree nosync willreturn memory(argmem: readwrite) }

```


commads
./bin/opt test.ll  -S


stacktrace

```
opt: llvm-project/llvm/lib/IR/AutoUpgrade.cpp:4999: void llvm::UpgradeIntrinsicCall(llvm::CallBase*, llvm::Function*): Assertion `CI->arg_size() == 1 && "Mismatch between function args and call args"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: ../llvm-dir/llvm-release/llvm-project/build/bin/opt test.ll -S
 #0 0x00005596d03b959f llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560559f)
 #1 0x00005596d03b620c llvm::sys::RunSignalHandlers() (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x560220c)
 #2 0x00005596d03b63a7 SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00007f6704a95420 __restore_rt (/lib/x86_64-linux-gnu/libpthread.so.0+0x14420)
 #4 0x00007f67040e200b raise /build/glibc-LcI20x/glibc-2.31/signal/../sysdeps/unix/sysv/linux/raise.c:51:1
 #5 0x00007f67040c1859 abort /build/glibc-LcI20x/glibc-2.31/stdlib/abort.c:81:7
 #6 0x00007f67040c1729 get_sysdep_segment_value /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:509:8
 #7 0x00007f67040c1729 _nl_load_domain /build/glibc-LcI20x/glibc-2.31/intl/loadmsgcat.c:970:34
 #8 0x00007f67040d2fd6 (/lib/x86_64-linux-gnu/libc.so.6+0x33fd6)
 #9 0x00005596cff98281 llvm::UpgradeIntrinsicCall(llvm::CallBase*, llvm::Function*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x51e4281)
#10 0x00005596cffa6e3a llvm::UpgradeCallsToIntrinsic(llvm::Function*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x51f2e3a)
#11 0x00005596cfd56776 llvm::LLParser::validateEndOfModule(bool) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4fa2776)
#12 0x00005596cfd9120b llvm::LLParser::Run(bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4fdd20b)
#13 0x00005596cfd36839 parseAssemblyInto(llvm::MemoryBufferRef, llvm::Module*, llvm::ModuleSummaryIndex*, llvm::SMDiagnostic&, llvm::SlotMapping*, bool, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) Parser.cpp:0:0
#14 0x00005596cfd36a05 llvm::parseAssembly(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::SlotMapping*, llvm::function_ref<std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>> (llvm::StringRef, llvm::StringRef)>, llvm::AsmParserContext*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4f82a05)
#15 0x00005596cfd31531 llvm::parseIR(llvm::MemoryBufferRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4f7d531)
#16 0x00005596cfd33906 llvm::parseIRFile(llvm::StringRef, llvm::SMDiagnostic&, llvm::LLVMContext&, llvm::ParserCallbacks, llvm::AsmParserContext*) (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x4f7f906)
#17 0x00005596cb5f5222 optMain (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x841222)
#18 0x00007f67040c3083 __libc_start_main /build/glibc-LcI20x/glibc-2.31/csu/../csu/libc-start.c:342:3
#19 0x00005596cb5edeae _start (../llvm-dir/llvm-release/llvm-project/build/bin/opt+0x839eae)
```


## Comments

### Comment 1 - kper

hmmm it works on my machine and godbolt also works.
https://godbolt.org/z/oxr7TcKqc

---

### Comment 2 - Emilyaxe

> hmmm it works on my machine and godbolt also works. https://godbolt.org/z/oxr7TcKqc
Hi, I have no idea why it doesn't work for me. I have tried in the latest version 50ae726bb349.

<img width="2560" height="1109" alt="Image" src="https://github.com/user-attachments/assets/62049c68-ed85-455f-803b-e0751dcc007b" />

---

### Comment 3 - kper

Both https://github.com/llvm/llvm-project/commit/3186ca25bc6cbd592a7ee25fc89b99354d7fda7e and https://github.com/llvm/llvm-project/commit/50ae726bb349868d9d22b71fb7c24c2f9c2f32bd are fine for me.

---

### Comment 4 - nikic

@kper You need to select the assertions-enabled build: https://godbolt.org/z/Peo7KcfYK

---

