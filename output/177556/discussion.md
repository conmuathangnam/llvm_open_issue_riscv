# llvm-symbolizer crashes on large binaries when pruneCache is triggered

**Author:** MattHarrisEpic
**URL:** https://github.com/llvm/llvm-project/issues/177556
**Status:** Open
**Labels:** crash, tools:llvm-symbolizer

## Body

When running llvm-symbolizer on exceptionally large debug files, it can cause a crash when pruneCache runs.

The debug file that demonstrates the issue has the following llvm-dwarfdump statistics:
  "#functions": 2967243,
  "#functions with location": 2926638,
  "#inlined functions": 34111408,
  "#inlined functions with abstract origins": 34111408,
  "#unique source variables": 7853978,
  "#source variables": 446186887,
  "#source variables with location": 41196627,

When the application crashes, we spawn an llvm-symbolizer and request the entire callstack's worth of symbols.
The behavior we're seeing is that once the internal cache hits the maximum cache size, LLVMSymbolizer::pruneCache() is run, which frees up LRU data. However, some of that data is still referenced by raw pointers inside the symbolizer, and later on the freed memory is dereferenced, causing a crash.

Raising the MaxCacheSize significantly has allowed us to work around the problem, but I'm raising this issue so that a proper fix for pruneCache can be investigated. Thank you.

## Comments

### Comment 1 - MattHarrisEpic

The callstack we have for the crash within llvm-symbolizer is partly resolved and has some useful information. It appears that the crash occurs inside the constructor for llvm::DWARFContext

``` #0  0x00000000008882fe in ?? ()
#1  0x0000000000000001 in ?? ()
#2  0x000000001b02bc80 in ?? ()
#3  0x000000001ae0e010 in ?? ()
#4  0x00007ffc76b1f048 in ?? ()
#5  0x0000000000000001 in ?? ()
#6  0x000000002a753ee0 in ?? ()
#7  0x00007ffc76b1f1c0 in ?? ()
#8  0x000000000000005e in ?? ()
#9  0x00007e8cafca3bdb in _int_malloc (av=0x2a753ee0, bytes=1) at ./malloc/malloc.c:3965
#10 0x0000000000547b7a in llvm::DWARFContext::DWARFContext(std::unique_ptr<llvm::DWARFObject const, std::default_delete<llvm::DWARFObject const> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::function<void (llvm::Error)>, std::function<void (llvm::Error)>, bool) ()
#11 0x00007ffc76b1f340 in ?? ()
#12 0x00007ffc76b1f1c0 in ?? ()
#13 0x00007ffc76b203d0 in ?? ()
#14 0x000000001c021740 in ?? ()
#15 0x00000000004b1731 in llvm::symbolize::LLVMSymbolizer::createModuleInfo(llvm::object::ObjectFile const*, std::unique_ptr<llvm::DIContext, std::default_delete<llvm::DIContext> >, llvm::StringRef) ()
#16 0x00000000004b512f in llvm::symbolize::LLVMSymbolizer::getOrCreateModuleInfo(llvm::StringRef) ()
#17 0x00000000004b7811 in llvm::Expected<llvm::DIInliningInfo> llvm::symbolize::LLVMSymbolizer::symbolizeInlinedCodeCommon<llvm::StringRef>(llvm::StringRef const&, llvm::object::SectionedAddress) ()
#18 0x00000000004b7b5f in llvm::symbolize::LLVMSymbolizer::symbolizeInlinedCode(llvm::StringRef, llvm::object::SectionedAddress) ()
#19 0x0000000000412ba5 in void executeCommand<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >(llvm::StringRef, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, Command, llvm::StringRef, unsigned long, unsigned long, bool, OutputStyle, llvm::symbolize::LLVMSymbolizer&, llvm::symbolize::DIPrinter&) ()
#20 0x0000000000413ff5 in symbolizeInput(llvm::opt::InputArgList const&, llvm::ArrayRef<unsigned char>, unsigned long, bool, OutputStyle, llvm::StringRef, llvm::symbolize::LLVMSymbolizer&, llvm::symbolize::DIPrinter&) ()
#21 0x00000000004167d8 in llvm_symbolizer_main(int, char**, llvm::ToolContext const&) ()
#22 0x0000000000408d83 in main () ```

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-tools-llvm-symbolizer

Author: None (MattHarrisEpic)

<details>
When running llvm-symbolizer on exceptionally large debug files, it can cause a crash when pruneCache runs.

The debug file that demonstrates the issue has the following llvm-dwarfdump statistics:
  "#functions": 2967243,
  "#functions with location": 2926638,
  "#inlined functions": 34111408,
  "#inlined functions with abstract origins": 34111408,
  "#unique source variables": 7853978,
  "#source variables": 446186887,
  "#source variables with location": 41196627,

When the application crashes, we spawn an llvm-symbolizer and request the entire callstack's worth of symbols.
The behavior we're seeing is that once the internal cache hits the maximum cache size, LLVMSymbolizer::pruneCache() is run, which frees up LRU data. However, some of that data is still referenced by raw pointers inside the symbolizer, and later on the freed memory is dereferenced, causing a crash.

Raising the MaxCacheSize significantly has allowed us to work around the problem, but I'm raising this issue so that a proper fix for pruneCache can be investigated. Thank you.
</details>


---

