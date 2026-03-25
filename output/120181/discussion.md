# LLVM Xcode6.1 compile crash

**Author:** a20251313
**URL:** https://github.com/llvm/llvm-project/issues/120181
**Status:** Closed
**Labels:** llvm:codegen, incomplete, crash, needs-reduction, waiting-for-response
**Closed Date:** 2025-06-17T14:17:03Z

## Body

On mac use swift-project compile:
Steps:
```
step 1.
git clone https://github.com/apple/swift.git swift
cd swift
utils/update-checkout --tag swift-6.0.2-RELEASE --clone
step 2.
utils/build-script --xcode --debug --skip-build-benchmarks  --swift-darwin-supported-archs "$(uname -m)" --sccache  --skip-ios --skip-tvos --skip-watchos --libswift=off
step3
```
use xcode6.1 compile then error occurs
```
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace.
Stack dump:
0.	Program arguments: /Users/ranjingfu/Desktop/swift-project/build/Xcode-DebugAssert/llvm-macosx-x86_64/Debug/bin/llvm-tblgen -gen-instr-info -instr-info-expand-mi-operand-info=0 -I /Users/ranjingfu/Desktop/swift-project/llvm-project/llvm/lib/Target/X86 -I/Users/ranjingfu/Desktop/swift-project/build/Xcode-DebugAssert/llvm-macosx-x86_64/include -I/Users/ranjingfu/Desktop/swift-project/llvm-project/llvm/include -I /Users/ranjingfu/Desktop/swift-project/llvm-project/llvm/lib/Target /Users/ranjingfu/Desktop/swift-project/llvm-project/llvm/lib/Target/X86/X86.td --write-if-changed -o /Users/ranjingfu/Desktop/swift-project/build/Xcode-DebugAssert/llvm-macosx-x86_64/lib/Target/X86/X86GenInstrInfo.inc
Stack dump without symbol names (ensure you have llvm-symbolizer in your PATH or set the environment var `LLVM_SYMBOLIZER_PATH` to point to it):
0  llvm-tblgen              0x0000000100622b4d llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) + 61
1  llvm-tblgen              0x00000001006230bb PrintStackTraceSignalHandler(void*) + 27
2  llvm-tblgen              0x0000000100620d46 llvm::sys::RunSignalHandlers() + 134
3  llvm-tblgen              0x0000000100623a7f SignalHandler(int) + 223
4  libsystem_platform.dylib 0x00007ff808ec2e9d _sigtramp + 29
5  llvm-tblgen              0x00000001007a426f
6  libsystem_c.dylib        0x00007ff808daeb19 abort + 126
7  llvm-tblgen              0x00000001001fcea2 void std::__1::__check_strict_weak_ordering_sorted[abi:dn180100]<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0>(std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*, std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0&) + 610
8  llvm-tblgen              0x00000001001fcb9d void std::__1::__sort_impl[abi:dn180100]<std::__1::_ClassicAlgPolicy, std::__1::__wrap_iter<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*>, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0>(std::__1::__wrap_iter<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*>, std::__1::__wrap_iter<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*>, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0&) + 157
9  llvm-tblgen              0x00000001001fcaf9 void std::__1::sort[abi:dn180100]<std::__1::__wrap_iter<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*>, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0>(std::__1::__wrap_iter<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*>, std::__1::__wrap_iter<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*>, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0) + 57
10 llvm-tblgen              0x00000001001fca4d void llvm::sort<std::__1::__wrap_iter<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*>, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0>(std::__1::__wrap_iter<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*>, std::__1::__wrap_iter<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>*>, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0) + 77
11 llvm-tblgen              0x00000001001f91f3 void llvm::sort<std::__1::vector<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>, std::__1::allocator<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>>>&, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0>(std::__1::vector<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>, std::__1::allocator<std::__1::pair<llvm::Record const*, llvm::OpcodeInfo>>>&, processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&)::$_0) + 83
12 llvm-tblgen              0x00000001001ed3ae processSTIPredicate(llvm::STIPredicateFunction&, llvm::DenseMap<llvm::Record const*, unsigned int, llvm::DenseMapInfo<llvm::Record const*, void>, llvm::detail::DenseMapPair<llvm::Record const*, unsigned int>> const&) + 2558
13 llvm-tblgen              0x00000001001eb96a llvm::CodeGenSchedModels::collectSTIPredicates() + 634
14 llvm-tblgen              0x00000001001e6514 llvm::CodeGenSchedModels::CodeGenSchedModels(llvm::RecordKeeper&, llvm::CodeGenTarget const&) + 644
15 llvm-tblgen              0x00000001001ebf95 llvm::CodeGenSchedModels::CodeGenSchedModels(llvm::RecordKeeper&, llvm::CodeGenTarget const&) + 37
16 llvm-tblgen              0x000000010022cf3d std::__1::__unique_if<llvm::CodeGenSchedModels>::__unique_single std::__1::make_unique[abi:dn180100]<llvm::CodeGenSchedModels, llvm::RecordKeeper&, llvm::CodeGenTarget const&>(llvm::RecordKeeper&, llvm::CodeGenTarget const&) + 61
17 llvm-tblgen              0x000000010022ce98 llvm::CodeGenTarget::getSchedModels() const + 56
18 llvm-tblgen              0x000000010042dcf9 (anonymous namespace)::InstrInfoEmitter::InstrInfoEmitter(llvm::RecordKeeper&) + 121
19 llvm-tblgen              0x000000010042c47d (anonymous namespace)::InstrInfoEmitter::InstrInfoEmitter(llvm::RecordKeeper&) + 29
20 llvm-tblgen              0x000000010042c3c5 EmitInstrInfo(llvm::RecordKeeper&, llvm::raw_ostream&) + 117
21 llvm-tblgen              0x00000001006bf17a llvm::TableGenMain(char const*, std::__1::function<bool (llvm::raw_ostream&, llvm::RecordKeeper&)>) + 1066
22 llvm-tblgen              0x00000001004fde51 main + 225
23 dyld                     0x00007ff808b002cd start + 1805
```


## Comments

### Comment 1 - Endilll

We need a self-contained reproducer.

---

### Comment 2 - fhahn

The first port of call should probably be https://github.com/swiftlang/swift, as this is failing with the LLVM version aligned to the particular swift release. Tagging some people who might be able to help or forward as appropriate @DougGregor @eeckstein

---

### Comment 3 - fhahn

The title is also confusing, this is not related to Xcode 6.1 which is ancient, but Swift 6.0.2?

---

### Comment 4 - rjmansfield

It's a typo. I've reproduced the issue with Xcode 16.1.

---

### Comment 5 - rjmansfield

This issue was fixed in the following commit.

```
commit bf2f9d2b64b82d4deeb96b2ad44e53e84a4536ea
Author: Danila Kutenin <kutdanila@yandex.ru>
Date:   Tue Aug 29 13:54:51 2023 -0700

    [TableGen] Make OpcodeMappings sort comparator strict weak ordering compliant
    
    This did not satisfy equivalence of transitivity. There was an attempt
    to fix it in https://reviews.llvm.org/D58687 but it was not fully
    correct. Masks might not be equivalent but be equal according to LessThan lambda
    
    Reviewed By: aeubanks, MaskRay
    
    Differential Revision: https://reviews.llvm.org/D157955
```

This fix is not present on the llvm-project swift-6.0.2-RELEASE branch but is present on the 6.1 development branch.  If you're building from source, the fix applies cleanly on top of the swift-6.0.2-RELEASE branch so cherry-picking the commit should resolve the issue. 

---

### Comment 6 - AaronBallman

I believe this issue can be closed as fixed; please reopen if the issue still persists on trunk Clang.

---

