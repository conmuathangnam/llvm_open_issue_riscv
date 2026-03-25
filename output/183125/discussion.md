# Tried to build FEX in crostini, and got whatever this is.

**Author:** Thunder-ai
**URL:** https://github.com/llvm/llvm-project/issues/183125
**Status:** Closed
**Labels:** llvm:mc, obsolete, incomplete, crash
**Closed Date:** 2026-02-24T19:21:28Z

## Body

```
something@someone:~/FEX/Build$ sudo ninja
[0/2] Re-checking globbed directories...
[4/77] Linking CXX executable Bin/FEXBash
FAILED: Bin/FEXBash 
: && /usr/bin/clang++ -O3 -DNDEBUG -fomit-frame-pointer -flto=thin -fuse-ld=lld -Xlinker --gc-sections -Xlinker --strip-all -Xlinker --as-needed -fPIE -pie Source/Tools/FEXBash/CMakeFiles/FEXBash.dir/FEXBash.cpp.o -o Bin/FEXBash  FEXCore/Source/libFEXCore.a  Source/Common/libCommon.a  FEXCore/Source/libJemallocLibs.a  Source/Common/cpp-optparse/libcpp-optparse.a  FEXCore/Source/libFEXCore_Base.a  External/fmt/libfmt.a  External/xxhash/cmake_unofficial/libxxhash.a  External/cephes/libcephes_128bit.a  External/SoftFloat-3e/libsoftfloat_3e.a  -ldl  External/tiny-json/libtiny-json.a  External/rpmalloc/librpmalloc.a  External/jemalloc_glibc/libFEX_jemalloc_glibc.a  -lpthread && :
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Running pass 'Function Pass Manager' on module 'External/fmt/libfmt.a(format.cc.o at 18400)'.
1.      Running pass 'AArch64 Assembly Printer' on function '@_ZN3fmt3v126detail9write_locENS0_14basic_appenderIcEENS0_9loc_valueERKNS0_12format_specsENS0_10locale_refE'
 #0 0x0000007f9779e4dc llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe9e4dc)
 #1 0x0000007f9779c4b4 llvm::sys::RunSignalHandlers() (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe9c4b4)
 #2 0x0000007f9779ea9c (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe9ea9c)
 #3 0x0000007f9cc0982c (linux-vdso.so.1+0x82c)
 #4 0x0000007f97c2a9f0 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x132a9f0)
 #5 0x0000007f9777b49c llvm::raw_ostream::write(char const*, unsigned long) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe7b49c)
 #6 0x0000007f98b31048 llvm::MCContext::createTempSymbol(llvm::Twine const&, bool) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x2231048)
 #7 0x0000007f98b55bc4 llvm::MCObjectStreamer::emitCFILabel() (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x2255bc4)
 #8 0x0000007f98b66f18 llvm::MCStreamer::emitCFIOffset(long, long) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x2266f18)
 #9 0x0000007f97f3f1b4 llvm::AsmPrinter::emitFunctionBody() (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x163f1b4)
#10 0x0000007f99064ba4 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x2764ba4)
#11 0x0000007f97af122c llvm::MachineFunctionPass::runOnFunction(llvm::Function&) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x11f122c)
#12 0x0000007f978d4390 llvm::FPPassManager::runOnFunction(llvm::Function&) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xfd4390)
#13 0x0000007f978daf70 llvm::FPPassManager::runOnModule(llvm::Module&) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xfdaf70)
#14 0x0000007f978d4d98 llvm::legacy::PassManagerImpl::run(llvm::Module&) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xfd4d98)
#15 0x0000007f98af9b58 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21f9b58)
#16 0x0000007f98afa7c4 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21fa7c4)
#17 0x0000007f98afa660 llvm::lto::thinBackend(llvm::lto::Config const&, unsigned int, std::function<llvm::Expected<std::unique_ptr<llvm::CachedFileStream, std::default_delete<llvm::CachedFileStream> > > (unsigned int)>, llvm::Module&, llvm::ModuleSummaryIndex const&, llvm::StringMap<std::unordered_set<unsigned long, std::hash<unsigned long>, std::equal_to<unsigned long>, std::allocator<unsigned long> >, llvm::MallocAllocator> const&, llvm::DenseMap<unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo<unsigned long, void>, llvm::detail::DenseMapPair<unsigned long, llvm::GlobalValueSummary*> > const&, llvm::MapVector<llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap<llvm::StringRef, unsigned int, llvm::DenseMapInfo<llvm::StringRef, void>, llvm::detail::DenseMapPair<llvm::StringRef, unsigned int> >, std::vector<std::pair<llvm::StringRef, llvm::BitcodeModule>, std::allocator<std::pair<llvm::StringRef, llvm::BitcodeModule> > > >*, std::vector<unsigned char, std::allocator<unsigned char> > const&) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21fa660)
#18 0x0000007f98af51ac (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21f51ac)
#19 0x0000007f98af4d28 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21f4d28)
#20 0x0000007f97be3ba8 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x12e3ba8)
#21 0x0000007f9773ce38 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe3ce38)
#22 0x0000007f964c2030 (/lib/aarch64-linux-gnu/libc.so.6+0x82030)
#23 0x0000007f9652bf1c (/lib/aarch64-linux-gnu/libc.so.6+0xebf1c)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: linker command failed due to signal (use -v to see invocation)
[9/77] Linking CXX executable Bin/FEXRootFSFetcher
ninja: build stopped: subcommand failed.
```

// Happened even when sudo wasn't used. Everything was up to date, I followed the guide almost exactly (minus the qtdeclarative6-dev as that is not a package that exists, qtdeclarative5-dev was assumed.)

## Comments

### Comment 1 - llvmbot


@llvm/issue-subscribers-llvm-mc

Author: Dremelhardwaremod (Thunder-ai)

<details>
something@<!-- -->someone:~/FEX/Build$ sudo ninja
[0/2] Re-checking globbed directories...
[4/77] Linking CXX executable Bin/FEXBash
FAILED: Bin/FEXBash 
: &amp;&amp; /usr/bin/clang++ -O3 -DNDEBUG -fomit-frame-pointer -flto=thin -fuse-ld=lld -Xlinker --gc-sections -Xlinker --strip-all -Xlinker --as-needed -fPIE -pie Source/Tools/FEXBash/CMakeFiles/FEXBash.dir/FEXBash.cpp.o -o Bin/FEXBash  FEXCore/Source/libFEXCore.a  Source/Common/libCommon.a  FEXCore/Source/libJemallocLibs.a  Source/Common/cpp-optparse/libcpp-optparse.a  FEXCore/Source/libFEXCore_Base.a  External/fmt/libfmt.a  External/xxhash/cmake_unofficial/libxxhash.a  External/cephes/libcephes_128bit.a  External/SoftFloat-3e/libsoftfloat_3e.a  -ldl  External/tiny-json/libtiny-json.a  External/rpmalloc/librpmalloc.a  External/jemalloc_glibc/libFEX_jemalloc_glibc.a  -lpthread &amp;&amp; :
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.      Running pass 'Function Pass Manager' on module 'External/fmt/libfmt.a(format.cc.o at 18400)'.
1.      Running pass 'AArch64 Assembly Printer' on function '@<!-- -->_ZN3fmt3v126detail9write_locENS0_14basic_appenderIcEENS0_9loc_valueERKNS0_12format_specsENS0_10locale_refE'
 #<!-- -->0 0x0000007f9779e4dc llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe9e4dc)
 #<!-- -->1 0x0000007f9779c4b4 llvm::sys::RunSignalHandlers() (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe9c4b4)
 #<!-- -->2 0x0000007f9779ea9c (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe9ea9c)
 #<!-- -->3 0x0000007f9cc0982c (linux-vdso.so.1+0x82c)
 #<!-- -->4 0x0000007f97c2a9f0 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x132a9f0)
 #<!-- -->5 0x0000007f9777b49c llvm::raw_ostream::write(char const*, unsigned long) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe7b49c)
 #<!-- -->6 0x0000007f98b31048 llvm::MCContext::createTempSymbol(llvm::Twine const&amp;, bool) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x2231048)
 #<!-- -->7 0x0000007f98b55bc4 llvm::MCObjectStreamer::emitCFILabel() (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x2255bc4)
 #<!-- -->8 0x0000007f98b66f18 llvm::MCStreamer::emitCFIOffset(long, long) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x2266f18)
 #<!-- -->9 0x0000007f97f3f1b4 llvm::AsmPrinter::emitFunctionBody() (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x163f1b4)
#<!-- -->10 0x0000007f99064ba4 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x2764ba4)
#<!-- -->11 0x0000007f97af122c llvm::MachineFunctionPass::runOnFunction(llvm::Function&amp;) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x11f122c)
#<!-- -->12 0x0000007f978d4390 llvm::FPPassManager::runOnFunction(llvm::Function&amp;) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xfd4390)
#<!-- -->13 0x0000007f978daf70 llvm::FPPassManager::runOnModule(llvm::Module&amp;) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xfdaf70)
#<!-- -->14 0x0000007f978d4d98 llvm::legacy::PassManagerImpl::run(llvm::Module&amp;) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xfd4d98)
#<!-- -->15 0x0000007f98af9b58 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21f9b58)
#<!-- -->16 0x0000007f98afa7c4 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21fa7c4)
#<!-- -->17 0x0000007f98afa660 llvm::lto::thinBackend(llvm::lto::Config const&amp;, unsigned int, std::function&lt;llvm::Expected&lt;std::unique_ptr&lt;llvm::CachedFileStream, std::default_delete&lt;llvm::CachedFileStream&gt; &gt; &gt; (unsigned int)&gt;, llvm::Module&amp;, llvm::ModuleSummaryIndex const&amp;, llvm::StringMap&lt;std::unordered_set&lt;unsigned long, std::hash&lt;unsigned long&gt;, std::equal_to&lt;unsigned long&gt;, std::allocator&lt;unsigned long&gt; &gt;, llvm::MallocAllocator&gt; const&amp;, llvm::DenseMap&lt;unsigned long, llvm::GlobalValueSummary*, llvm::DenseMapInfo&lt;unsigned long, void&gt;, llvm::detail::DenseMapPair&lt;unsigned long, llvm::GlobalValueSummary*&gt; &gt; const&amp;, llvm::MapVector&lt;llvm::StringRef, llvm::BitcodeModule, llvm::DenseMap&lt;llvm::StringRef, unsigned int, llvm::DenseMapInfo&lt;llvm::StringRef, void&gt;, llvm::detail::DenseMapPair&lt;llvm::StringRef, unsigned int&gt; &gt;, std::vector&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt;, std::allocator&lt;std::pair&lt;llvm::StringRef, llvm::BitcodeModule&gt; &gt; &gt; &gt;*, std::vector&lt;unsigned char, std::allocator&lt;unsigned char&gt; &gt; const&amp;) (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21fa660)
#<!-- -->18 0x0000007f98af51ac (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21f51ac)
#<!-- -->19 0x0000007f98af4d28 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x21f4d28)
#<!-- -->20 0x0000007f97be3ba8 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0x12e3ba8)
#<!-- -->21 0x0000007f9773ce38 (/lib/aarch64-linux-gnu/libLLVM-14.so.1+0xe3ce38)
#<!-- -->22 0x0000007f964c2030 (/lib/aarch64-linux-gnu/libc.so.6+0x82030)
#<!-- -->23 0x0000007f9652bf1c (/lib/aarch64-linux-gnu/libc.so.6+0xebf1c)
clang: error: unable to execute command: Segmentation fault (core dumped)
clang: error: linker command failed due to signal (use -v to see invocation)
[9/77] Linking CXX executable Bin/FEXRootFSFetcher
ninja: build stopped: subcommand failed.
// Happened even when sudo wasn't used. Everything was up to date, I followed the guide almost exactly (minus the qtdeclarative6-dev as that is not a package that exists, qtdeclarative5-dev was assumed.)
</details>


---

### Comment 2 - EugeneZelenko

Could you please try 22 or `main` branch? Only most recent release is maintained.

Please provide reproducer, preferably on https://godbolt.org.

---

### Comment 3 - Thunder-ai

I have no clue what that means, is it for LLVM? If so, how would I try that?

---

### Comment 4 - EugeneZelenko

You reported bug for LLVM 14 which is four years old. Bug may be fixed in later versions.

---

### Comment 5 - Thunder-ai

Oh.... Lemme see if I can update.

---

### Comment 6 - Thunder-ai

Unable to build, thank you for helping me.

---

