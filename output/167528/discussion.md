# `llvm-cov` assertion error: index goes out-of-bounds on a `llvm/test/...` object file

**Author:** gburgessiv
**URL:** https://github.com/llvm/llvm-project/issues/167528
**Status:** Open
**Labels:** coverage, crash

## Body

On ToT (0f4c8dd87af61ec5e254bec9f144e9dbc8bc2229) and for quite a while before that, I observed `llvm-cov` crashing on loading an object file. When assertions are enabled, the crash is shown to be an issue with indexing a `BitVector`:

```
[ct] ~/l/main [ 3ms ] ~> build/release/bin/llvm-profdata show /l/min-profile.profdata
Instrumentation level: Front-end
Total functions: 0
Maximum function count: 0
Maximum internal block count: 0
Total number of blocks: 0
Total count: 0


[ct] ~/l/main [ 5ms ] ~> build/release/bin/llvm-cov export -instr-profile=/l/min-profile.profdata --object=llvm/test/tools/llvm-cov/Inputs/mcdc-general-18.o -skip-expansions -skip-functions
llvm-cov: /usr/local/google/home/gbiv/misc/btrfs/llvm/main/llvm/include/llvm/ADT/BitVector.h:469: bool llvm::BitVector::operator[](unsigned int) const: Assertion `Idx < Size && "Out-of-bounds Bit access."' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace and instructions to reproduce the bug.
Stack dump:
0.      Program arguments: build/release/bin/llvm-cov "build/release/bin/llvm-cov export" -instr-profile=/l/min-profile.profdata --object=llvm/test/tools/llvm-cov/Inputs/mcdc-general-18.o -skip-expansions -skip-functions
 #0 0x0000555d553600bb llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (build/release/bin/llvm-cov+0x36d0bb)
 #1 0x0000555d5535d0da SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #2 0x00007f3c41649df0 (/lib/x86_64-linux-gnu/libc.so.6+0x3fdf0)
 #3 0x00007f3c4169e95c __pthread_kill_implementation ./nptl/pthread_kill.c:44:76
 #4 0x00007f3c41649cc2 raise ./signal/../sysdeps/posix/raise.c:27:6
 #5 0x00007f3c416324ac abort ./stdlib/abort.c:81:3
 #6 0x00007f3c41632420 __assert_perror_fail ./assert/assert-perr.c:31:1
 #7 0x0000555d554f3be7 (anonymous namespace)::MCDCRecordProcessor::buildTestVector(llvm::coverage::MCDCRecord::TestVector&, short, int) (.cold) CoverageMapping.cpp:0:0
 #8 0x0000555d554f4150 (anonymous namespace)::MCDCRecordProcessor::buildTestVector(llvm::coverage::MCDCRecord::TestVector&, short, int) CoverageMapping.cpp:0:0
 #9 0x0000555d554f4f11 llvm::coverage::CounterMappingContext::evaluateMCDCRegion(llvm::coverage::CounterMappingRegion const&, llvm::ArrayRef<llvm::coverage::CounterMappingRegion const*>, bool) (build/release/bin/llvm-cov+0x501f11)
#10 0x0000555d554f814a llvm::coverage::CoverageMapping::loadFunctionRecord(llvm::coverage::CoverageMappingRecord const&, std::optional<std::reference_wrapper<llvm::IndexedInstrProfReader>> const&) (build/release/bin/llvm-cov+0x50514a)
#11 0x0000555d554fa409 llvm::coverage::CoverageMapping::loadFromReaders(llvm::ArrayRef<std::unique_ptr<llvm::coverage::CoverageMappingReader, std::default_delete<llvm::coverage::CoverageMappingReader>>>, std::optional<std::reference_wrapper<llvm::IndexedInstrProfReader>>&, llvm::coverage::CoverageMapping&) (build/release/bin/llvm-cov+0x507409)
#12 0x0000555d554fae29 llvm::coverage::CoverageMapping::loadFromFile(llvm::StringRef, llvm::StringRef, llvm::StringRef, std::optional<std::reference_wrapper<llvm::IndexedInstrProfReader>>&, llvm::coverage::CoverageMapping&, bool&, llvm::SmallVectorImpl<llvm::SmallVector<unsigned char, 10u>>*) (build/release/bin/llvm-cov+0x507e29)
#13 0x0000555d554fb711 llvm::coverage::CoverageMapping::load(llvm::ArrayRef<llvm::StringRef>, std::optional<llvm::StringRef>, llvm::vfs::FileSystem&, llvm::ArrayRef<llvm::StringRef>, llvm::StringRef, llvm::object::BuildIDFetcher const*, bool) (build/release/bin/llvm-cov+0x508711)
#14 0x0000555d55266dc5 (anonymous namespace)::CodeCoverageTool::load() CodeCoverage.cpp:0:0
#15 0x0000555d5527153f (anonymous namespace)::CodeCoverageTool::run((anonymous namespace)::CodeCoverageTool::Command, int, char const**) CodeCoverage.cpp:0:0
#16 0x0000555d55272c1d exportMain(int, char const**) (build/release/bin/llvm-cov+0x27fc1d)
#17 0x0000555d552520b0 main (build/release/bin/llvm-cov+0x25f0b0)
#18 0x00007f3c41633ca8 __libc_start_call_main ./csu/../sysdeps/nptl/libc_start_call_main.h:74:3
#19 0x00007f3c41633d65 call_init ./csu/../csu/libc-start.c:128:20
#20 0x00007f3c41633d65 __libc_start_main ./csu/../csu/libc-start.c:347:5
#21 0x0000555d552514e1 _start (build/release/bin/llvm-cov+0x25e4e1)
fish: Job 1, 'build/release/bin/llvm-cov expo…' terminated by signal SIGABRT (Abort)
```

I don't know how this in-tree object file was created, so I'm unsure if this can reasonably occur in practice (we bumped into it via "run llvm-cov with all files listed by roughly `find ${dir} -name '*.o'`, where `${dir}` happened to hold an LLVM tree"), but since it's an out-of-bounds indexing op, seems at least worth raising.

Extra info from my investigation:

The crashing operation is from `llvm/lib/ProfileData/Coverage/CoverageMapping.cpp`:

```
      if (!Bitmap[IsVersion11
                      ? DecisionParams.BitmapIdx * CHAR_BIT + TV.getIndex()
                      : DecisionParams.BitmapIdx - NumTestVectors + NextTVIdx])
```

Using `printf` to dump vars, I saw:

```
Index 4294967289 > bitmap size 4
IsVersion11 == 0
BitmapIdx == 0
NumTestVectors == 7
NextTVIdx == 0
```

So somehow this object file leads to an underflow in the index here.

## Comments

### Comment 1 - oToToT

The object file seems coming from https://github.com/llvm/llvm-project/commit/7ead2d8c7e9114b3f23666209a1654939987cb30 

The test using it is https://github.com/llvm/llvm-project/blob/c1f18a2518898e5f593ce1341d19f61b5ee58cdd/llvm/test/tools/llvm-cov/mcdc-general-18.test

---

