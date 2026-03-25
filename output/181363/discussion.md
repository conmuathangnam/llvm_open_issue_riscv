# Assertion "Must have the same number of vector elements as mask elements!"' in SelectionDAG.cpp

**Author:** androm3da
**URL:** https://github.com/llvm/llvm-project/issues/181363
**Status:** Closed
**Labels:** backend:Hexagon, release:backport, crash
**Closed Date:** 2026-02-23T22:53:41Z

## Body

While building the 7zip test case from llvm-test-suite using clang built from c780d890daf3474c73c4ffa736e155c619c3d0be, we see the assertion below.

## Crash Signature

```
clang: /local/mnt/workspace/bcain-20260212_145304/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:2189: llvm::SDValue llvm::SelectionDAG::getVectorShuffle(llvm::EVT, const llvm::SDLoc &, llvm::SDValue, llvm::SDValue, ArrayRef<int>): Assertion `VT.getVectorNumElements() == Mask.size() && "Must have the same number of vector elements as mask elements!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -disable-free -clear-ast-before-backend -main-file-name 7zIn.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +memops -target-feature -long-calls -target-feature +hvxv68 -target-feature +hvx-length128b -target-feature +hvx-qfloat -mqdsp6-compat -Wreturn-type -fshort-enums -mllvm -machine-sink-split=0 -debugger-tuning=gdb -Werror=date-time -Wno-error=narrowing -w -fdeprecated-macro -ferror-limit 19 -pthread -fshort-enums -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -x c++ /local/mnt/workspace/bcain-reduce-20260212_190946/case-7zIn-acf059/7zIn-acf059.cpp -o /dev/null
1.	<eof> parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '/local/mnt/workspace/bcain-reduce-20260212_190946/case-7zIn-acf059/7zIn-acf059.cpp'.
4.	Running pass 'Hexagon DAG->DAG Pattern Instruction Selection' on function '@_ZN8NArchive3N7z10CInArchive20FindAndReadSignatureEP9IInStreamPKy'
 #0 0x000000000204d877 llvm::sys::PrintStackTrace(llvm::raw_ostream&, int) (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x204d877)
 #1 0x000000000204b0b5 llvm::sys::RunSignalHandlers() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x204b0b5)
 #2 0x000000000204e67e SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #3 0x00002adbd61cd520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #4 0x00002adbd62219fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #5 0x00002adbd61cd476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #6 0x00002adbd61b37f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #7 0x00002adbd61b371b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #8 0x00002adbd61c4e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #9 0x00000000039d1d07 llvm::SelectionDAG::getVectorShuffle(llvm::EVT, llvm::SDLoc const&, llvm::SDValue, llvm::SDValue, llvm::ArrayRef<int>) (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x39d1d07)
#10 0x0000000000ec8dba llvm::HexagonTargetLowering::extractHvxSubvectorPred(llvm::SDValue, llvm::SDValue, llvm::SDLoc const&, llvm::MVT, llvm::SelectionDAG&) const (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0xec8dba)
#11 0x0000000000ecd97c llvm::HexagonTargetLowering::LowerHvxExtractSubvector(llvm::SDValue, llvm::SelectionDAG&) const (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0xecd97c)
```

## Minimal Reproducer Command

```bash
/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +hvx-length128b -fcxx-exceptions -vectorize-slp -x c++ /local/mnt/workspace/bcain-reduce-20260212_190946/case-7zIn-acf059/7zIn-acf059.cpp -o /dev/null
```


## Comments

### Comment 1 - androm3da

Fixed by #181364

---

### Comment 2 - llvmbot


@llvm/issue-subscribers-backend-hexagon

Author: Brian Cain (androm3da)

<details>
While building the 7zip test case from llvm-test-suite using clang built from c780d890daf3474c73c4ffa736e155c619c3d0be, we see the assertion below.

## Crash Signature

```
clang: /local/mnt/workspace/bcain-20260212_145304/llvm-project/llvm/lib/CodeGen/SelectionDAG/SelectionDAG.cpp:2189: llvm::SDValue llvm::SelectionDAG::getVectorShuffle(llvm::EVT, const llvm::SDLoc &amp;, llvm::SDValue, llvm::SDValue, ArrayRef&lt;int&gt;): Assertion `VT.getVectorNumElements() == Mask.size() &amp;&amp; "Must have the same number of vector elements as mask elements!"' failed.
PLEASE submit a bug report to https://github.com/llvm/llvm-project/issues/ and include the crash backtrace, preprocessed source, and associated run script.
Stack dump:
0.	Program arguments: /local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -disable-free -clear-ast-before-backend -main-file-name 7zIn.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -ffp-contract=on -fno-rounding-math -mconstructor-aliases -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +memops -target-feature -long-calls -target-feature +hvxv68 -target-feature +hvx-length128b -target-feature +hvx-qfloat -mqdsp6-compat -Wreturn-type -fshort-enums -mllvm -machine-sink-split=0 -debugger-tuning=gdb -Werror=date-time -Wno-error=narrowing -w -fdeprecated-macro -ferror-limit 19 -pthread -fshort-enums -fno-signed-char -fgnuc-version=4.2.1 -fskip-odr-check-in-gmf -fcxx-exceptions -fexceptions -vectorize-loops -vectorize-slp -faddrsig -fdwarf2-cfi-asm -x c++ /local/mnt/workspace/bcain-reduce-20260212_190946/case-7zIn-acf059/7zIn-acf059.cpp -o /dev/null
1.	&lt;eof&gt; parser at end of file
2.	Code generation
3.	Running pass 'Function Pass Manager' on module '/local/mnt/workspace/bcain-reduce-20260212_190946/case-7zIn-acf059/7zIn-acf059.cpp'.
4.	Running pass 'Hexagon DAG-&gt;DAG Pattern Instruction Selection' on function '@<!-- -->_ZN8NArchive3N7z10CInArchive20FindAndReadSignatureEP9IInStreamPKy'
 #<!-- -->0 0x000000000204d877 llvm::sys::PrintStackTrace(llvm::raw_ostream&amp;, int) (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x204d877)
 #<!-- -->1 0x000000000204b0b5 llvm::sys::RunSignalHandlers() (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x204b0b5)
 #<!-- -->2 0x000000000204e67e SignalHandler(int, siginfo_t*, void*) Signals.cpp:0:0
 #<!-- -->3 0x00002adbd61cd520 (/lib/x86_64-linux-gnu/libc.so.6+0x42520)
 #<!-- -->4 0x00002adbd62219fc pthread_kill (/lib/x86_64-linux-gnu/libc.so.6+0x969fc)
 #<!-- -->5 0x00002adbd61cd476 gsignal (/lib/x86_64-linux-gnu/libc.so.6+0x42476)
 #<!-- -->6 0x00002adbd61b37f3 abort (/lib/x86_64-linux-gnu/libc.so.6+0x287f3)
 #<!-- -->7 0x00002adbd61b371b (/lib/x86_64-linux-gnu/libc.so.6+0x2871b)
 #<!-- -->8 0x00002adbd61c4e96 (/lib/x86_64-linux-gnu/libc.so.6+0x39e96)
 #<!-- -->9 0x00000000039d1d07 llvm::SelectionDAG::getVectorShuffle(llvm::EVT, llvm::SDLoc const&amp;, llvm::SDValue, llvm::SDValue, llvm::ArrayRef&lt;int&gt;) (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0x39d1d07)
#<!-- -->10 0x0000000000ec8dba llvm::HexagonTargetLowering::extractHvxSubvectorPred(llvm::SDValue, llvm::SDValue, llvm::SDLoc const&amp;, llvm::MVT, llvm::SelectionDAG&amp;) const (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0xec8dba)
#<!-- -->11 0x0000000000ecd97c llvm::HexagonTargetLowering::LowerHvxExtractSubvector(llvm::SDValue, llvm::SelectionDAG&amp;) const (/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang+0xecd97c)
```

## Minimal Reproducer Command

```bash
/local/mnt/workspace/bcain-20260212_145304/install/x86_64-linux-gnu/bin/clang -cc1 -triple hexagon-unknown-linux-musl -O3 -emit-obj -mllvm -hexagon-autohvx -target-cpu hexagonv68 -target-feature +hvx-length128b -fcxx-exceptions -vectorize-slp -x c++ /local/mnt/workspace/bcain-reduce-20260212_190946/case-7zIn-acf059/7zIn-acf059.cpp -o /dev/null
```

</details>


---

### Comment 3 - androm3da

/cherry-pick c3a86ff2d0b397d757345fad7e29c2a6e7dbc823

---

### Comment 4 - llvmbot

/pull-request llvm/llvm-project#182955

---

